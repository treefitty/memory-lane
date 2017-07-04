#include <dos.h>
#include <mem.h>
#include <bios.h>
#include <stdlib.h>
#include <conio.h>

#include <igraph.h>
#include <ieffects.h>
#include <imodex.h>
#include <ikeybrd.h>
#include <ipcx.h>

#include ".\part1\intro.h"

extern char *Intro1;
extern char *Intro2;
extern char *Intro3;
extern char *Intro4;
extern char *Intro5;
extern unsigned char MyPal[768];
extern unsigned char TempPal[768];
extern unsigned char VertPal[768];
extern char far *VirtScr;
extern int i;

void Part1(void)
{
	ModeX320x400();
	memset(TempPal,0,768);
	setpalette(TempPal);

	sleep(5);

	ForceCompXBit(42,190,Intro1);
	FadeIn(IntroPal);
	if(_bios_keybrd(_KEYBRD_READY)!=0)
		{
		_bios_keybrd(_KEYBRD_READ);
		goto SKIP1;
	}
	sleep(5);
	FadeOff(IntroPal);
	sleep(2);

SKIP1:

	ClrVidMem();
	setpalette(TempPal);

	ForceCompXBit(152,160,Intro2);
	ForceCompXBit(56,190,Intro3);
	ForceCompXBit(124,230,Intro4);
	FadeIn(IntroPal);
	if(_bios_keybrd(_KEYBRD_READY)!=0)
		{
		_bios_keybrd(_KEYBRD_READ);
		goto SKIP2;
	}
	sleep(5);
	FadeOff(IntroPal);
	sleep(2);

SKIP2:

	ClrVidMem();
	setpalette(TempPal);

	ForceCompXBit(124,194,Intro5);
	FadeIn(IntroPal);
	if(_bios_keybrd(_KEYBRD_READY)!=0)
		{
		_bios_keybrd(_KEYBRD_READ);
		goto SKIP3;
	}
	sleep(5);
	FadeOff(IntroPal);
	sleep(2);
	delay(100);

SKIP3:

	asm mov	ax,0x13      // No more tweaked, sniff.....
	asm int	0x10

	memset(TempPal,63,768);
	setpalette(TempPal);
	putbuffer(VirtScr);
	FadeDown(VertPal);
	if(_bios_keybrd(_KEYBRD_READY)!=0)
		{
		_bios_keybrd(_KEYBRD_READ);
		goto SKIP4;
	}
	sleep(4);

SKIP4:

	FadeOff(VertPal);

	sleep(1);
}