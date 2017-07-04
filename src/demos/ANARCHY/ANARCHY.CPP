/*
	Main file for ANARCHY

	Copyright (c) 1995 Black Marble
*/

#include "AN_DEFS.H"

//
// Local variables
//

static char far *PCXpicture;
static char *myPal[768];

void printString(char *String,int y)
{
	char far *Scr=(char *)MK_FP(0xa000,0);
	int tempx,tempy,count,NumItems,Ofs,x;
	char Char,col;

	NumItems=strlen(String);
	tempx=0;
	for(count=0;count<NumItems;count++)
		{
		Char=String[count]-32;
		tempx+=myFonts[Char].Width;
	}
	x=160-(tempx>>1);
	tempx=x;
	Ofs=0;
	for(count=0;count<NumItems;count++)
		{
		Char=String[count]-32;
		for(tempy=y;tempy<y+myFonts[Char].Height;tempy++)
			{
			for(tempx=x;tempx<x+myFonts[Char].Width;tempx++)
				{
				col=FontDat[Char][((tempy-y)*myFonts[Char].Width)+tempx-x];
				if(col)Scr[(tempy*320)+tempx+Ofs]=col;

			}
		}
		Ofs+=myFonts[Char].Width;
	}
}

void setupMenu(void)
{
	char far *Scr=(char *)MK_FP(0xa000,0);

	if((PCXpicture=(char*)malloc(64000))==0) TerminalError(NOMEM,NULL);
	loadPCX("STARTUP.PCX",PCXpicture,LOADPAL);
	LoadFont("FONT12.FNT");

	memcpy(Scr,PCXpicture,64000);

	printString("NEW GAME",43);
	printString("CONFIGURE",71);
	printString("README",99);
	printString("QUIT",127);
}

void drawCursor(int x,int y,char col)
{
	char far *Scr=(char *)MK_FP(0xa000,0);
	int i,offset;

	for(i=x;i<x+134;i++)
		{
		if(col==0)
			{
			offset=(y*320)+i;
			Scr[offset]=PCXpicture[offset];
			offset=((y+20)*320)+i;
			Scr[offset]=PCXpicture[offset];
		}
		else {
			Scr[(y*320)+i]=col;
			Scr[((y+20)*320)+i]=col;
		}
	}
	for(i=y;i<y+20;i++)
		{
		if(col==0)
			{
			offset=(i*320)+x;
			Scr[offset]=PCXpicture[offset];
			offset=(i*320)+x+133;
			Scr[offset]=PCXpicture[offset];
		}
		else {
			Scr[(i*320)+x]=col;
			Scr[(i*320)+x+133]=col;
		}
	}
}

//
// Free memory used by font and background piccy
//
void freeMemory(void)
{
	int i;

	for(i=0;i<92;i++)
		{
		if(myFonts[i].Defined==1)
			{
			free(FontDat[i]);
		}
	}

	free(PCXpicture);
}

void main(void)
{
	int cursorX,cursorY,colour;
	char selection;
	char done=0;
	int key=0;

	asm mov	ax,0x13
	asm int	0x10

	setupMenu();

	cursorX=93;
	cursorY=39;
	colour=63;
	drawCursor(cursorX,cursorY,colour);
	selection=0;
	while(!done)
		{
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			key=_bios_keybrd(_KEYBRD_READ);
		}
		switch(key)
			{
			case b_enter :
				{
				switch(selection)
					{
					case 0 :
						{
						freeMemory();
						x_set_mode(X_MODE_320x200,640);
						x_set_doublebuffer(240);
						GameLoop();
						asm mov	ax,0x13
						asm int	0x10
						setupMenu();
						break;
					}
					case 1:
						{

						break;
					}
					case 2:
						{

						break;
					}
					case 3:
						{
						done=1;
					}
				}
				drawCursor(cursorX,cursorY,colour);
				break;
			}
			case b_upArrow :
				{
				if(selection>0)
					{
					selection--;
					drawCursor(cursorX,cursorY,0);
					cursorY-=28;
					drawCursor(cursorX,cursorY,colour);
				}
				else {
					selection=3;
					drawCursor(cursorX,cursorY,0);
					cursorY+=3*28;
					drawCursor(cursorX,cursorY,colour);
				}
				break;
			}
			case b_downArrow :
				{
				if(selection<3)
					{
					selection++;
					drawCursor(cursorX,cursorY,0);
					cursorY+=28;
					drawCursor(cursorX,cursorY,colour);
				}
				else {
					selection=0;
					drawCursor(cursorX,cursorY,0);
					cursorY-=3*28;
					drawCursor(cursorX,cursorY,colour);
				}
				break;
			}
		}
		key=0;
	}

	asm mov	ax,0x03
	asm int	0x10
}





