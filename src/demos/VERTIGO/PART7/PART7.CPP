#include <conio.h>
#include <mem.h>
#include <bios.h>
#include <dos.h>

#include <ieffects.h>
#include <igraph.h>
#include <ipcx.h>
#include <bitmap.h>

#include ".\part7\lens.h"

extern unsigned char TempPal[768];
//extern char far *LensIntro;
extern char far *SkullPic;
extern char far *TransFormBuf;
extern char far *TempBuf;
extern char far *VirtScr;
extern unsigned char SkullPal[768];
extern char far *Scr;
extern int SIN[360];
extern int COS[360];
extern char done;
extern int Loops;

int ix,iy;
int BounceAngle2=0;
int IncFactor2;

void Magnify(int x,int y)
{
	int count;
	int xpos,ypos;

	xpos=x;
	ypos=y;
	getbitmap(TempBuf,VirtScr,75,75,xpos,ypos);
	for(count=0;count<5625;count++)
		{
		TransFormBuf[count]=TempBuf[LensBuf[count]];
	}
	copybitmap(VirtScr,TransFormBuf,74,74,xpos+1,ypos);
}

void Part7(void)
{
/*	asm mov	ax,0x13
	asm int	0x10

	memset(TempPal,0,768);
	setpalette(TempPal);
	PutRawPCX(LensIntro);
	GetRawPCXPal(TempPal,LensIntro,15015);

	FadeIn(TempPal);
	sleep(4);
	FadeOff(TempPal);
*/
	asm mov	ax,0x13
	asm int	0x10
	setpalette(SkullPal);
	melt_on(SkullPic);

	//At point 160,100

	getbitmap(VirtScr,Scr,320,200,0,0);

	done=0;
	IncFactor2=1;
	Loops=0;
	while(!done)
		{
		iy=120-(0.45*SIN[BounceAngle2]);
		BounceAngle2+=2;
		if(BounceAngle2>=180) BounceAngle2=0;
		ix+=IncFactor2;

		getbitmap(TempBuf,SkullPic,78,78,ix-2,iy-2);
		copybitmap(VirtScr,TempBuf,78,78,ix-2,iy-2);
		Magnify(ix,iy);
		putbuffer(VirtScr);

		if(ix>=250) IncFactor2=-1;
		if(ix<=2) IncFactor2=1;

		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			done=1;
		}
		if(Loops>=1600) done=1;
		Loops++;
	}
	done=0;

	melt_off();
}
