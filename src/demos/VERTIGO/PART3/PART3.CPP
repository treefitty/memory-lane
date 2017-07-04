#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <mem.h>
#include <stdlib.h>

#include <bwsb.h>

#include ".\part3\yellow.h"
#include ".\part3\red.h"
#include ".\part3\green.h"
#include ".\part3\blank.h"

#include <igraph.h>
#include <ikeybrd.h>
#include <ipcx.h>
#include <ieffects.h>
#include <bitmap.h>
#include <imodex.h>

#define NumBalls 14

extern char far *VBallIntro;
extern unsigned char TempPal[768];
extern unsigned char MyPal[768];
extern int i,x,y,z;
extern char done;
extern int COS[360], SIN[360];
extern int xa,ya,za;
extern int PointRoll,PointPitch,PointYaw;
extern int PostXadd,PostYadd;
extern int Loops;

typedef struct {

	char flag;
	int wx,wy,wz;
	int sx,sy,sz;
	char col;

} BallType;

BallType Box[15];
BallType Triangle[15];
BallType Sphere[15];
BallType Cross[15];
BallType Wave[15];

BallType *CurrObjectPtr;
BallType *NextObjectPtr;

int SortBalls[15];

long f;
int a, VisPage=1,XOffset=0;
int temp1,temp2,temp3;
int dx,dy,dz;
char Transform;
char NumDone=0;
char PalCount=0,Fading=1;

void InitSortList(void)
{
	// point each element to an initial ball
	for(int i=0;i<15;i++)
		{
		SortBalls[i]=i;
	}
}

int CompareCurrObjects(const void *a, const void *b)
{
	if(CurrObjectPtr[*(int *)a].sz < CurrObjectPtr[*(int *)b].sz)
		{
		return -1;
	}
	else if(CurrObjectPtr[*(int *)a].sz > CurrObjectPtr[*(int *)b].sz)
		{
		return +1;
	}
	else
		{
		return 0;
	}
}

void SetStartAddr(void)
{
	asm push	dx
	asm mov	dx,0x3d4
	asm mov	al,0x0c
	asm mov	ah,bh
	asm out	dx,ax
	asm inc	al
	asm mov	ah,bl
	asm out	dx,ax
	asm pop	dx
}

void WaitVR(void)
{
	asm mov	dx,0x3da

VR1:
	asm in	al,dx
	asm test	al,8
	asm jz	VR1

VR2:
	asm in	al,dx
	asm test	al,8
	asm jnz	VR2
}

void Part3(void)
{
	asm mov	ax,0x13
	asm int	0x10

	memset(TempPal,0,768);
	setpalette(TempPal);

	GetRawPCXPal(TempPal,VBallIntro,16384);
	PutRawPCX(VBallIntro);

	FadeIn(TempPal);
	sleep(4);
	FadeOff(TempPal);

	ModeX320x200();

	setpalette(BallBltPal);

	Box[0] .wx=-30; Box[0] .wy=-30;Box[0] .wz=-30;Box[0] .col=1;
	Box[1] .wx=-30; Box[1] .wy=-30;Box[1] .wz= 30;Box[1] .col=2;
	Box[2] .wx=-30; Box[2] .wy= 30;Box[2] .wz=-30;Box[2] .col=1;
	Box[3] .wx=-30; Box[3] .wy= 30;Box[3] .wz= 30;Box[3] .col=3;
	Box[4] .wx= 30; Box[4] .wy=-30;Box[4] .wz=-30;Box[4] .col=1;
	Box[5] .wx= 30; Box[5] .wy=-30;Box[5] .wz= 30;Box[5] .col=2;
	Box[6] .wx= 30; Box[6] .wy= 30;Box[6] .wz=-30;Box[6] .col=2;
	Box[7] .wx= 30; Box[7] .wy= 30;Box[7] .wz= 30;Box[7] .col=3;
	Box[8] .wx=  0; Box[8] .wy=-30;Box[8] .wz=-30;Box[8] .col=2;
	Box[9] .wx=  0; Box[9] .wy=-30;Box[9] .wz= 30;Box[9] .col=3;
	Box[10].wx=  0; Box[10].wy= 30;Box[10].wz=-30;Box[10].col=2;
	Box[11].wx=  0; Box[11].wy= 30;Box[11].wz= 30;Box[11].col=1;
	Box[12].wx=-20; Box[12].wy=  0;Box[12].wz=  0;Box[12].col=3;
	Box[13].wx= 20; Box[13].wy=  0;Box[13].wz=  0;Box[13].col=3;

	Triangle[0] .wx=  0; Triangle[0] .wy=-50;Triangle[0] .wz=  0;Triangle[0] .col=  1;
	Triangle[1] .wx=-20; Triangle[1] .wy=  0;Triangle[1] .wz=-20;Triangle[1] .col=  2;
	Triangle[2] .wx=-20; Triangle[2] .wy=  0;Triangle[2] .wz= 20;Triangle[2] .col=  1;
	Triangle[3] .wx= 20; Triangle[3] .wy=  0;Triangle[3] .wz=-20;Triangle[3] .col=  3;
	Triangle[4] .wx= 20; Triangle[4] .wy=  0;Triangle[4] .wz= 20;Triangle[4] .col=  1;
	Triangle[5] .wx=-40; Triangle[5] .wy= 50;Triangle[5] .wz=-40;Triangle[5] .col=  2;
	Triangle[6] .wx=-40; Triangle[6] .wy= 50;Triangle[6] .wz=  0;Triangle[6] .col=  2;
	Triangle[7] .wx=-40; Triangle[7] .wy= 50;Triangle[7] .wz= 40;Triangle[7] .col=  3;
	Triangle[8] .wx=  0; Triangle[8] .wy= 50;Triangle[8] .wz=-40;Triangle[8] .col=  2;
	Triangle[9] .wx=  0; Triangle[9] .wy= 50;Triangle[9] .wz=  0;Triangle[9] .col=  3;
	Triangle[10].wx=  0; Triangle[10].wy= 50;Triangle[10].wz= 40;Triangle[10].col=  2;
	Triangle[11].wx= 40; Triangle[11].wy= 50;Triangle[11].wz=-40;Triangle[11].col=  1;
	Triangle[12].wx= 40; Triangle[12].wy= 50;Triangle[12].wz=  0;Triangle[12].col=  3;
	Triangle[13].wx= 40; Triangle[13].wy= 50;Triangle[13].wz= 40;Triangle[13].col=  3;

	Sphere[0] .wx=  0; Sphere[0] .wy=-80;Sphere[0] .wz=  0;Sphere[0] .col=  1;
	Sphere[1] .wx=-55; Sphere[1] .wy=-55;Sphere[1] .wz=  0;Sphere[1] .col=  2;
	Sphere[2] .wx=  0; Sphere[2] .wy=-55;Sphere[2] .wz=-55;Sphere[2] .col=  1;
	Sphere[3] .wx=  0; Sphere[3] .wy=-55;Sphere[3] .wz= 55;Sphere[3] .col=  3;
	Sphere[4] .wx= 55; Sphere[4] .wy=-55;Sphere[4] .wz=  0;Sphere[4] .col=  1;
	Sphere[5] .wx=-80; Sphere[5] .wy=  0;Sphere[5] .wz=  0;Sphere[5] .col=  2;
	Sphere[6] .wx=  0; Sphere[6] .wy=  0;Sphere[6] .wz=-80;Sphere[6] .col=  2;
	Sphere[7] .wx=  0; Sphere[7] .wy=  0;Sphere[7] .wz= 80;Sphere[7] .col=  3;
	Sphere[8] .wx= 80; Sphere[8] .wy=  0;Sphere[8] .wz=  0;Sphere[8] .col=  2;
	Sphere[9] .wx=-55; Sphere[9] .wy= 55;Sphere[9] .wz=  0;Sphere[9] .col=  3;
	Sphere[10].wx=  0; Sphere[10].wy= 55;Sphere[10].wz= 55;Sphere[10].col=  2;
	Sphere[11].wx=  0; Sphere[11].wy= 55;Sphere[11].wz=-55;Sphere[11].col=  1;
	Sphere[12].wx= 55; Sphere[12].wy= 55;Sphere[12].wz=  0;Sphere[12].col=  3;
	Sphere[13].wx=  0; Sphere[13].wy= 80;Sphere[13].wz=  0;Sphere[13].col=  3;

	Cross[0] .wx=-50; Cross[0] .wy=  0;Cross[0] .wz=  0;Cross[0] .col=  1;
	Cross[1] .wx=-25; Cross[1] .wy=  0;Cross[1] .wz=  0;Cross[1] .col=  2;
	Cross[2] .wx= 25; Cross[2] .wy=  0;Cross[2] .wz=  0;Cross[2] .col=  1;
	Cross[3] .wx= 50; Cross[3] .wy=  0;Cross[3] .wz=  0;Cross[3] .col=  3;
	Cross[4] .wx=  0; Cross[4] .wy=-50;Cross[4] .wz=  0;Cross[4] .col=  1;
	Cross[5] .wx=  0; Cross[5] .wy=-25;Cross[5] .wz=  0;Cross[5] .col=  2;
	Cross[6] .wx=  0; Cross[6] .wy= 25;Cross[6] .wz=  0;Cross[6] .col=  2;
	Cross[7] .wx=  0; Cross[7] .wy= 50;Cross[7] .wz=  0;Cross[7] .col=  3;
	Cross[8] .wx=  0; Cross[8] .wy=  0;Cross[8] .wz=-50;Cross[8] .col=  2;
	Cross[9] .wx=  0; Cross[9] .wy=  0;Cross[9] .wz=-25;Cross[9] .col=  3;
	Cross[10].wx=  0; Cross[10].wy=  0;Cross[10].wz= 25;Cross[10].col=  2;
	Cross[11].wx=  0; Cross[11].wy=  0;Cross[11].wz= 50;Cross[11].col=  1;
	Cross[12].wx=-75; Cross[12].wy=  0;Cross[12].wz=  0;Cross[12].col=  3;
	Cross[13].wx= 75; Cross[13].wy=  0;Cross[13].wz=  0;Cross[13].col=  3;

	Wave[0] .wx=-80; Wave[0] .wy=  0;Wave[0] .wz=-25;Wave[0] .col=1;
	Wave[1] .wx=-80; Wave[1] .wy=  0;Wave[1] .wz= 25;Wave[1] .col=2;
	Wave[2] .wx=-50; Wave[2] .wy=-40;Wave[2] .wz=-25;Wave[2] .col=1;
	Wave[3] .wx=-50; Wave[3] .wy=-40;Wave[3] .wz= 25;Wave[3] .col=3;
	Wave[4] .wx=-20; Wave[4] .wy=-40;Wave[4] .wz=-25;Wave[4] .col=1;
	Wave[5] .wx=-20; Wave[5] .wy=-40;Wave[5] .wz= 25;Wave[5] .col=2;
	Wave[6] .wx=  0; Wave[6] .wy=  0;Wave[6] .wz=-25;Wave[6] .col=2;
	Wave[7] .wx=  0; Wave[7] .wy=  0;Wave[7] .wz= 25;Wave[7] .col=3;
	Wave[8] .wx= 20; Wave[8] .wy= 40;Wave[8] .wz=-25;Wave[8] .col=2;
	Wave[9] .wx= 20; Wave[9] .wy= 40;Wave[9] .wz= 25;Wave[9] .col=3;
	Wave[10].wx= 50; Wave[10].wy= 40;Wave[10].wz=-25;Wave[10].col=2;
	Wave[11].wx= 50; Wave[11].wy= 40;Wave[11].wz= 25;Wave[11].col=1;
	Wave[12].wx= 80; Wave[12].wy=  0;Wave[12].wz=-25;Wave[12].col=3;
	Wave[13].wx= 80; Wave[13].wy=  0;Wave[13].wz= 25;Wave[13].col=3;

	InitSortList();

	PostXadd=145;
	PostYadd=90;
	done=0;
	Transform=0;

	CurrObjectPtr=Box;
	NextObjectPtr=Triangle;

	Loops=0;
	done=0;

	flushkeybrd();

	while(!done)
		{
		for(i=0;i<NumBalls;i++)
			{
			x=CurrObjectPtr[i].wx;
			y=CurrObjectPtr[i].wy;
			z=CurrObjectPtr[i].wz;
			xa=((COS[PointRoll] *x) + (SIN[PointRoll] *y)) >> 8;
			ya=((COS[PointRoll] *y) - (SIN[PointRoll] *x)) >> 8;
			x =((COS[PointYaw]  *xa)- (SIN[PointYaw]  *z)) >> 8;
			za=((SIN[PointYaw]  *xa)+ (COS[PointYaw]  *z)) >> 8;
			z =((COS[PointPitch]*za)- (SIN[PointPitch]*ya))>> 8;
			y =((SIN[PointPitch]*za)+ (COS[PointPitch]*ya))>> 8;
			CurrObjectPtr[i].sz= z;
			CurrObjectPtr[i].sx= x+PostXadd;
			CurrObjectPtr[i].sy= y+PostYadd;
		}

		qsort((void *)SortBalls,NumBalls,sizeof(SortBalls[0]),CompareCurrObjects);

		for(i=0;i<NumBalls;i++)
			{
			if(CurrObjectPtr[SortBalls[i]].col==1)
				{
				PutCompiledXBit(CurrObjectPtr[SortBalls[i]].sx,CurrObjectPtr[SortBalls[i]].sy,GreenBlt,XOffset);
			}
			else if(CurrObjectPtr[SortBalls[i]].col==2)
				{
				PutCompiledXBit(CurrObjectPtr[SortBalls[i]].sx,CurrObjectPtr[SortBalls[i]].sy,RedBlt,XOffset);
			}
			else if(CurrObjectPtr[SortBalls[i]].col==3)
				{
				PutCompiledXBit(CurrObjectPtr[SortBalls[i]].sx,CurrObjectPtr[SortBalls[i]].sy,YellowBlt,XOffset);
			}
		}
		switch(VisPage)
			{
			case 0 :
				{
				_BX=XOffset;
				SetStartAddr();

				WaitVR();

				asm mov	ax,0xa000
				asm mov	es,ax
				asm mov	dx,0x3c4
				asm mov	ax,0xF02
				asm out	dx,ax

				asm mov	di,0
				asm mov	cx,8000
				asm mov	ax,0
				asm rep	stosw

				XOffset=0;
				VisPage=1;
				break;
			}
			case 1 :
				{
				_BX=XOffset;
				SetStartAddr();

				WaitVR();

				asm mov	ax,0xa000
				asm mov	es,ax
				asm mov	dx,0x3c4
				asm mov	ax,0xF02
				asm out	dx,ax

				asm mov	ax,0xa000
				asm mov	es,ax
				asm mov	di,16000
				asm mov	cx,8000
				asm mov	ax,0
				asm rep	stosw

				XOffset=16000;
				VisPage=0;
				break;
			}
		}
		Loops++;
		PointRoll+=3;
		PointYaw+=1;
		PointPitch+=2;
		if(PointRoll>=360) PointRoll=0;
		if(PointYaw >=360) PointYaw=0;
		if(PointPitch>=360)PointPitch=0;
		if(Loops==500)
			{
			if(CurrObjectPtr!=Wave) Transform=1;
			else done=1;
		}
		if(Transform==1)
			{
			for(i=0;i<NumBalls;i++)
				{
				if(CurrObjectPtr[i].flag==0)
					{
					dx=NextObjectPtr[i].wx-CurrObjectPtr[i].wx;
					dy=NextObjectPtr[i].wy-CurrObjectPtr[i].wy;
					dz=NextObjectPtr[i].wz-CurrObjectPtr[i].wz;

					if(dx>1) dx=1;
					else if(dx<-1) dx=-1;

					if(dy>1) dy=1;
					else if(dy<-1) dy=-1;

					if(dz>1) dz=1;
					else if(dz<-1) dz=-1;

					if(dx==0 && dy==0 && dz==0)
						{
						CurrObjectPtr[i].flag=1;
						NumDone++;
					}

					CurrObjectPtr[i].wx+=dx;
					CurrObjectPtr[i].wy+=dy;
					CurrObjectPtr[i].wz+=dz;
				}
			}
		}
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			if(CurrObjectPtr!=Wave) Transform=1;
			else done=1;
		}
		if(NumDone==14)
			{
			NumDone=0;
			Loops=0;
			Transform=0;

			if(CurrObjectPtr==Box)
				{
				CurrObjectPtr=Triangle;
				NextObjectPtr=Sphere;
			}
			else if(CurrObjectPtr==Triangle)
				{
				CurrObjectPtr=Sphere;
				NextObjectPtr=Cross;
			}
			else if(CurrObjectPtr==Sphere)
				{
				CurrObjectPtr=Cross;
				NextObjectPtr=Wave;
			}
			else CurrObjectPtr=Wave;
		}
	}
	done=0;
	flushkeybrd();
}










