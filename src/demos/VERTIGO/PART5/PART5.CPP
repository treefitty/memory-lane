#include <dos.h>
#include <bios.h>
#include <stdlib.h>
#include <mem.h>

#include <igraph.h>
#include <bitmap.h>
#include <ipcx.h>
#include <ieffects.h>

#include ".\part5\flame.h"
#include ".\part5\flame1.h"
#include ".\part5\flame2.h"

//extern char far *FireIntro;
extern unsigned char TempPal[768];
extern char done;
extern int x,y;
extern int Loops;
int FinishUp;
int flame[67][80];

void Part5(void)
{
	char far *Scr=(char *)MK_FP(0xa000,0);
	unsigned int i,j;
	int delta;
	int ThisDI=0;

/*	asm mov	ax,0x13
	asm int	0x10

	memset(TempPal,0,768);
	setpalette(TempPal);

	PutRawPCX(FireIntro);
	GetRawPCXPal(TempPal,FireIntro,12177);

	FadeIn(TempPal);
	sleep(4);
	FadeOff(TempPal);
*/
	asm mov	ax,0x13
	asm int	0x10

	tc_putbitmap(Scr,(char far *)Flame1,82,25,0,9);
	tc_putbitmap(Scr,(char far *)Flame2,217,25,100,9);

	for(i=0;i<768;i++)
		{
		FlamePal[i] = FlamePal[i] >> 2;
		if(i>743) FlamePal[i]=FlameFontPal[i];
	}
	FadeIn(FlamePal);
	setpalette(FlamePal);
	randomize();

	// Initialize the screen buffer
	for(i=0;i<66;i++)
		{
		for(j=0;j<80;j++)
			{
			flame[i][j]=0;
		}
	}

	FinishUp=0;
	Loops=0;
	done=0;
	while(!done)
		{

		// transform the current buffer
		asm mov cx,5200
		_DI=(unsigned int)&flame[0][0];
		asm xor ax,ax
		asm add di,160
	D1:
		asm mov ax,ds:[di-2]
		asm add ax,ds:[di]
		asm add ax,ds:[di+2]
		asm add ax,ds:[di+160]
		asm shr ax,2
		asm jz D2

		asm sub ax,1
		asm jz D2
		asm sub ax,1
		asm jz D2
		asm sub ax,1

	D2:
		asm mov word ptr ds:[di-160],ax
		asm add di,2
		asm dec cx
		asm jnz D1

		// Set new bottom line with random white or black color
		delta = 0;
		for(j=0;j<80;j++)   // {set new bottom line}
			{
			if(random(10) < 5)
				{
				delta=random(2)*247;
			}
			flame[64][j]=delta;
			flame[65][j]=delta;
		}

		// Write the buffer to the screen
		_SI=(unsigned int)&flame[0][0];
		asm mov ax,0xa000
		asm mov es,ax
		asm mov di,25600
		asm mov dx,60
	F3:
		asm mov bx,2
	F2:
		asm mov cx,80
	F1:
		asm mov al,[si]
		asm mov ah,al
		asm mov es:[di],ax;     //{word aligned write to display mem}
		asm push di
		asm mov di,ThisDI
		asm add di,320
		asm sub di,cx
		asm sub di,cx
		asm mov al,[si+160]
		asm mov ah,al
		asm mov es:[di],ax;     //{word aligned write to display mem}
		asm pop di
		asm add di,2
		asm add si,2
		asm dec cx
		asm jnz F1

		asm add di,160
		asm mov ThisDI,di
		asm sub si,160
		asm dec bx
		asm jnz F2

		asm add si,160
		asm dec dx
		asm jnz F3

		delay(4);

		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			done=1;
		}
		Loops++;
		if(Loops==900) done=1;
	}
	FadeOff(FlamePal);

	asm mov	ax,0xa000
	asm mov	es,ax
	asm xor	di,di
	asm mov	cx,32000
	asm mov	ax,0
	asm rep 	stosw
}