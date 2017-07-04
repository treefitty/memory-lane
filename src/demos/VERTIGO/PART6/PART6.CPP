#include <conio.h>

#include <ieffects.h>
#include <igraph.h>

extern char far *SkullPic;
extern unsigned char SkullPal[768];

void Part6(void)
{
	asm mov	ax,0x13
	asm int	0x10

	//loadPCX("SKULL.PCX",thepic,1);
	setpalette(SkullPal);
	melt_on(SkullPic);

	getch();
	melt_off();
}