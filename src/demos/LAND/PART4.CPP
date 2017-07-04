#include <dos.h>
#include <stdlib.h>
#include <bios.h>

#include <ikeybrd.h>
#include <igraph.h>
#include <ipcx.h>
#include <ieffects.h>

#define a_COS(a)  \
	COS[a]       // Reference SIN, COS tables as macros for increased speed.
#define a_SIN(a)  \
	SIN[a]

#define SPACE	6
#define VISMAPWIDTH 30
#define VISMAPDEPTH 30
#define LEFT 	-1
#define RIGHT	1
#define DOWN	1
#define UP	-1

char far *LandIntro;
char far *VirtScr;
int LinearMulTab[200];
unsigned char TempPal[768];
unsigned char MyPal[768];
int SIN[360],COS[360];
long i;
int x,y,z;
char done;
int xa,ya,za;
int PointRoll,PointPitch,PointYaw;
int PostXadd,PostYadd,PreZadd;
int objz,Loops;

char col;
long k;
int StartX=0,StartY=0;
int wx,wy,wz;
int sx,sy;
int Xdir,Ydir;

typedef struct {

	int x,y;

} LandType;

LandType SavePts[VISMAPWIDTH*VISMAPDEPTH];

int counter=0;

extern "C" {

	void DrawPlasma(void);

}

void ProjectLand(void)
{
	for(y=StartY;y<StartY+VISMAPDEPTH;y++)
		{
		for(x=StartX;x<StartX+VISMAPWIDTH;x++,counter++)
			{
			wx=(x-(VISMAPWIDTH>>1)-StartX)*6;
			wy=(y-(VISMAPDEPTH>>1)-StartY)*6;
			wz=VirtScr[LinearMulTab[y]+x];
			xa=((a_COS(PointRoll) *wx)+(a_SIN(PointRoll) *wy)) >> 8;
			ya=((a_COS(PointRoll) *wy)-(a_SIN(PointRoll) *wx)) >> 8;
			wx=((a_COS(PointYaw)  *xa)-(a_SIN(PointYaw)  *wz)) >> 8;
			za=((a_SIN(PointYaw)  *xa)+(a_COS(PointYaw)  *wz)) >> 8;
			wz=((a_COS(PointPitch)*za)-(a_SIN(PointPitch)*ya)) >> 8;
			wy=((a_SIN(PointPitch)*za)+(a_COS(PointPitch)*ya)) >> 8;
			sx=-(wx)+PostXadd;
			sy=-(wy)+PostYadd;

			_DI=LinearMulTab[SavePts[counter].y]+SavePts[counter].x;
			_AL=0;
			asm mov	bx,0xa000       //Erase old pixel
			asm mov	es,bx
			asm mov	es:[di],al

			if(sy>0 && sy<200 && sx>0 && sx<320)
				{
				_DI=LinearMulTab[sy]+sx;
				_AL=VirtScr[LinearMulTab[y]+x];
				asm mov	cx,0xa000		// Draw new pixel
				asm mov	es,cx
				asm mov	es:[di],al
			}

			SavePts[counter].x=sx;
			SavePts[counter].y=sy;
		}
	}
}

void ClrLinearMem(void)
{
		asm mov	ax,0xa000
		asm mov	es,ax
		asm xor	di,di
		asm mov	cx,64000

ClrLoop:

		asm mov	es:[di],al
		asm inc	di
		asm dec	cx
		asm jnz	ClrLoop
}

void main(void)
{
	char far *Scr=(char *)MK_FP(0xa000,0);

	asm mov	ax,0x13
	asm int	0x10

	if(!(LandIntro=new char[64000])) exit(1);

	loadPCX("land.pcx",LandIntro,0);

	for(i=0;i<768;i++) TempPal[i]=0;
	setpalette(TempPal);

	GetRawPCXPal(TempPal,LandIntro,18245);

	for(i=0;i<64000;i++)
		{
     	Scr[i]=LandIntro[i];
	}

	FadeIn(TempPal);

	sleep(4);

	FadeOff(TempPal);

	ClrLinearMem();

		//VirtScr=new char[64000];

		asm lea   di,[MyPal]            // set black palette.
		asm mov   ax,ds
		asm mov   es,ax
		asm mov   cx,768
		asm xor   ax,ax
		asm cld
		asm rep   stosb

		setpalette(MyPal);

		DrawPlasma();

GenPalette:

		asm lea   di,[MyPal]            // generate color palette.
		asm mov   ax,0x3F00
		asm mov   [di+0],al               // background.
		asm mov   [di+1],al
		asm mov   [di+2],al
		asm add   di,3
		asm mov   [di+0],al               // blue. (water)
		asm mov   [di+1],al
		asm mov   [di+2],ah
		asm add   di,3
		asm mov   ax,0x0000                // green to brown.
		asm mov   bx,0x3F00
		asm mov   dx,0x0000
		asm mov   cx,61
GP0:      asm mov   [di+0],ah
		asm mov   [di+1],bh
		asm mov   [di+2],dh
		asm add   di,3
		asm add   ax,0x0108
		asm sub   bx,0x0086
		asm add   dx,0x0000
		asm loop  GP0

		asm mov   ax,0x3F00                // brown to white.
		asm mov   bx,0x2000                // (mountain)
		asm mov   dx,0x0000
		asm mov   cx,48
GP1:      asm mov   [di+0],ah
		asm mov   [di+1],bh
		asm mov   [di+2],dh
		asm add   di,3
		asm add   ax,0x0000
		asm add   bx,0x00AA
		asm add   dx,0x0150
		asm loop  GP1
		asm mov   ax,0x3F00                // white. (snow)
		asm mov   bx,0x3F00
		asm mov   dx,0x3F00
		asm mov   cx,145
GP2:      asm mov   [di+0],ah
		asm mov   [di+1],bh
		asm mov   [di+2],dh
		asm add   di,3
		asm add   ax,0x0000
		asm add   bx,0x0000
		asm add   dx,0x0000
		asm loop  GP2

		asm mov	ax,0xa000
		asm mov	es,ax
		asm mov	di,0

AdjLoop:
		asm mov   al,es:[di]
		asm sub   al,128
		asm test  al,al
		asm jge   AdjLevel
		asm xor   al,al

AdjLevel:
		asm xor   ah,ah

		asm mov	bx,192
		asm mul   bx

		asm inc   ah
		asm mov   es:[di],ah
		asm inc   di
		asm loop  AdjLoop

		for(k=0;k<64000;k++)
			{
			VirtScr[k]=Scr[k];
		}

		ClrLinearMem();

	PostXadd=160;
	PostYadd=140;
	PreZadd=-100;

	PointRoll=0;
	PointYaw=0;
	PointPitch=60;

	StartX=0;
	StartY=0;

	done=0;

	Ydir=DOWN;
	Xdir=RIGHT;

	ProjectLand();
	FadeIn(MyPal);

	Loops=0;
	while(!done)
		{
		delay(25);
		counter=0;

		ProjectLand();

		Loops++;
		PointRoll++;
		if(PointRoll==360) PointRoll=1;

		StartX+=Xdir;
		if(StartX+VISMAPWIDTH>=320 && Xdir==RIGHT) Xdir=LEFT;
		if(StartX<=0 && Xdir==LEFT) Xdir=RIGHT;

		StartY+=Ydir;
		if(StartY+VISMAPDEPTH>=200 && Ydir==DOWN) Ydir=UP;
		if(StartY<=0 && Ydir==UP) Ydir=DOWN;

		if(Loops==1100) done=1;

		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			done=1;
		}
	}
	delete VirtScr;

     FadeOff(MyPal);

	flushkeybrd();
}