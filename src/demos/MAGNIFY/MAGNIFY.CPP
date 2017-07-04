#include <math.h>
#include <mem.h>
#include <bios.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>

#include <ipcx.h>
#include <ieffects.h>
#include <igraph.h>
#include <iedit.h>
#include <imouse.h>
#include <bitmap.h>

#include "lens.h"

#define escape      0x11b
#define BOUNCE 0
#define MOUSE 1

const float PI = 3.1415927;
char far *Scr=(char *)MK_FP(0xa000,0);
char far *PCXPicture;
char far *TransFormBuf;
char far *TempBuf;
char far *VirtScr;
char PCXPal[768];
int SIN[360];
int COS[360];
char done;
int temp;
int ix,iy;
int BounceAngle=0;
int IncFactor;
char Control;

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

void InitAngleArrays(void)            // Initialize SIN, COS arrays.
{
	int a;                           // Variables used only in here.
	float radians;

	for ( a = 0; a < 360; a++ )     // For one full rotation or 360 degrees.
		{
		radians = (float)(a * PI) / 180; // Convert degrees to radians.
		COS[a] = (int)(cos(radians) * 256);// Multiply by 256 and round
		SIN[a] = (int)(sin(radians) * 256);// off by converting to integer.
	}
}

void MemError(void)
{
	asm mov  	ax,0x03
	asm int	0x10
	textcolor(WHITE);
	cprintf("Memory alocation error!\n\r");
	cprintf("Abnormal termination.\n\r");
	asm mov	ax,0x4c00
	asm int	0x21
}

void main(void)
{
	char FileName[14];
	char ch;

	asm mov	ax,0x03
	asm int	0x10
	textcolor(CYAN);
	memset(FileName,0,14);
	FileName[0]=254;
	cprintf("PCX Magnifier. Ver 1.0\n\n\r");
	cprintf("(c) Black Marble '95\n\r");
	cprintf("Code : Paul Carroll\n\n\r");
	cprintf("PCX MUST be 320x200x256\n\r");
	cprintf("PCX File name :> ");
	textcolor(WHITE);
	if(Edit(FileName,13,0x0F)==1)
		{
		clrscr();
		textcolor(15);
		cprintf("Escape pressed.\n\r");
		cprintf("Program terminated by user.\n\n\r");
		asm mov	ax,0x4c00
		asm int	0x21
	}

	if(!(PCXPicture  =new char[64000])) MemError();
	if(loadPCX(FileName,PCXPicture,0)!=0)
		{
		clrscr();
		textcolor(WHITE);
		cprintf("Cannot open PCX file!\n\r");
		cprintf("Abnormal termination.\n\n\r");
		asm mov	ax,0x4c00
		asm int	0x21
	}
	loadPCXpal(FileName,PCXPal);

	done=0;
	while(!done)
		{
		gotoxy(0,9);
		textcolor(CYAN);
		cprintf("\n\n\rBouncing Ball or Mouse Control? (B/M) :> ");
		textcolor(WHITE);
		ch=getche();
		if(ch=='b' || ch=='B')
			{
			Control=BOUNCE;
			done=1;
		}
		if(ch=='m' || ch=='M')
			{
			Control=MOUSE;
			checkmouse();
			done=1;
		}
	}
	clrscr();
	if(!(VirtScr     =new char[64000])) MemError();
	if(!(TempBuf     =new char[78*78])) MemError();
	if(!(TransFormBuf=new char[78*78])) MemError();

	asm mov	ax,0x13
	asm int	0x10

	setpalette(PCXPal);
	melt_on(PCXPicture);

	getbitmap(VirtScr,Scr,320,200,0,0);
	InitAngleArrays();
	vlimit(5,125);
	hlimit(0,240);

	done=0;
	IncFactor=1;
	while(!done)
		{
		if(Control==BOUNCE)
			{
			getbitmap(TempBuf,PCXPicture,78,78,ix,iy);
			copybitmap(VirtScr,TempBuf,78,78,ix,iy);

			if(ix>=250) IncFactor=-1;
			if(ix<=2) IncFactor=1;
			iy=120-(0.45*SIN[BounceAngle]);
			BounceAngle+=2;
			if(BounceAngle>=180) BounceAngle=0;
			ix+=IncFactor;

			Magnify(ix,iy);
			putbuffer(VirtScr);
		}
		if(Control==MOUSE)
			{
			getbitmap(TempBuf,PCXPicture,78,78,ix,iy);
			copybitmap(VirtScr,TempBuf,78,78,ix,iy);

			ix=mousexpos();
			iy=mouseypos();

			Magnify(ix,iy);
			putbuffer(VirtScr);
		}

		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			temp=_bios_keybrd(_KEYBRD_READ);
               if(temp==escape) done=1;
		}
	}
	done=0;

	melt_off();

	asm mov 	ax,0x03
	asm int	0x10
}






