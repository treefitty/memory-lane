#include <stdlib.h>
#include <bios.h>
#include <dos.h>
#include <stdio.h>
#include <mem.h>

#include <m360x480.h>
#include <igraph.h>
#include <ipcx.h>
#include <ieffects.h>

#include "title.h"

#define NUMFLAKES 500

extern void LoadMusic(char *Filename);
extern void StartPlaying(void);
extern void StopPlaying(void);

struct FLAKE {

	int x;
	int y;
	char speed;

};

char far *Scr=(char *)MK_FP(0xa000,0);
char far *PicBuf;
char TempPal[768];
FLAKE myFlakes[NUMFLAKES];

void MemExit(void)
{
	asm mov	ax,0x03
	asm int	0x10
	printf("Out of memory!\n\n");
	exit(1);
}

void main(void)
{
	int i,x,y;
	char done;

	Set360x480Mode();

	if((PicBuf=(char *)malloc(64000))==0) MemExit();
	loadPCX("SNOW.PCX",PicBuf,NOPAL);

	LoadMusic("CHRIS207.GDM");
	StartPlaying();

	randomize();
	for(i=0;i<NUMFLAKES;i++)
		{
		myFlakes[i].x=random(360);
		myFlakes[i].y=random(480)-480;
		myFlakes[i].speed=random(4)+2;
	}
	memset(TempPal,0,768);
	setpalette(TempPal);

	for(y=0;y<200;y++)
		{
		for(x=0;x<320;x++)
			{
			Draw360x480Dot(x+56,y+345,PicBuf[(y*320)+x]);
		}
	}
	FadeIn(Pal);
	SetColor(15,63,63,63);
	done=0;
	while(!done)
		{
		for(i=0;i<NUMFLAKES;i++)
			{
			if(myFlakes[i].y>=0)
				Draw360x480Dot(myFlakes[i].x,myFlakes[i].y,0);

			myFlakes[i].y+=myFlakes[i].speed;
			myFlakes[i].x+=random(3)-1;

			if(myFlakes[i].y>=480) myFlakes[i].y=0;

			if(myFlakes[i].y>=0)
				Draw360x480Dot(myFlakes[i].x,myFlakes[i].y,15);
		}
		delay(20);

		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			done=1;
		}
	}

	asm mov	ax,0x03
	asm int	0x10

	StopPlaying();
}