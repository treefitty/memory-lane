#include <bios.h>
#include <stdio.h>
#include <conio.h>

#include ".\menu\menu1.h"

#define MENU_TOP 11       //alter the vertical position of the menu
#define CURSOR_WIDTH 25

extern int x,y;
extern char done;
const int up_arrow= 0x4800;
const int down_arrow= 0x5000;
const int Enter= 0x1c0d;
const int Escape= 0x11b;
char looping=0;	 // looping is off
char *strloop="Off";			//just used for printing on or off for looping
int device = 1; 			//default sound device = SB
int bPort=0x220;
int bDma=1;                   //  cursor on their menus
int bIrq=5;
char *cPort="220h";
char *cDma="1";
char *cIrq="5";
int MENU_BOTTOM=14;        //alter the number of menu choices bottom - (top-1)
int CurrItem=0;
int key = 0;
int CursorX = 38;         // alter the horizontal position of the menu
int CursorY=20;
int s;
int wordx;
int wordy;
int MenuSet=1;

void ChangeAttr(int x,int y,char attr)
{
	asm mov 	ax,y
	asm mov 	bx,80
	asm mul 	bx
	asm add 	ax,x
	asm mov 	bx,2
	asm mul 	bx
	asm mov 	di,ax
	asm add 	di,1
	asm mov 	ax,0xb800
	asm mov 	es,ax
	asm mov 	al,attr
	asm mov 	es:[di],al
}

void EraseCursor(void)
{
	for (x = CursorX;x < CursorX+CURSOR_WIDTH;x++)
		{
		ChangeAttr(x,CursorY,0x02);
	}
}

void PutCursor(void)
{
	if (CursorY< MENU_TOP )
		{
		CursorY = MENU_BOTTOM;
	}
	else if (CursorY> MENU_BOTTOM)
		{
		CursorY = MENU_TOP;
	}
	for (x = CursorX;x < CursorX+CURSOR_WIDTH;x++)
		{
		ChangeAttr(x,CursorY,0x1E);
	}
}

void CursorPos(void)
{
	CurrItem=CursorY-MENU_TOP;
	CurrItem++;
}

struct MENU_CHOICE
{
	char *text[10];
	int port[10];
	int irq[10];
	int dma[10];
	void (*execute[10])(void);
	int NoEntries;
} *MenuChoice;

void ShowSetup(void)
{
	textcolor(YELLOW);
	gotoxy(17,13);
	cprintf("%s    ",MenuChoice[2].text[device]);
	gotoxy(17,18);
	cprintf("%s    ",strloop);
	if(device==1)
		{
		textcolor(LIGHTMAGENTA);
		gotoxy(3,14);
		cprintf("Base Port   -");
		gotoxy(3,15);
		cprintf("IRQ address -");
		gotoxy(3,16);
		cprintf("DMA channel -");
		textcolor(LIGHTGREEN);
		gotoxy(17,14);
		cprintf("%s",cPort);
		gotoxy(17,15);
		cprintf("%s",cIrq);
		gotoxy(17,16);
		cprintf("%s",cDma);
	}
}

void PutMenu(void)
{
	gotoxy(CursorX,MENU_BOTTOM+2);
	cprintf("                            ");
	for(int l = MENU_TOP+1;l < MENU_BOTTOM+2;l++)
		{
		gotoxy(CursorX,l);
		cprintf(" ");
		gotoxy(CursorX+(CURSOR_WIDTH+2),l);
		cprintf(" ");
	}
	for (wordx=CursorX+1, wordy=MENU_TOP+1;wordy < MENU_BOTTOM+2;wordy++)
		{
		gotoxy(wordx,wordy);
		s = wordy-MENU_TOP;
		printf("                         ");
	}
	MENU_BOTTOM=(MENU_TOP-1)+ MenuChoice[MenuSet].NoEntries;
	for (wordx=CursorX+1, wordy=MENU_TOP+1;wordy < MENU_BOTTOM+2;wordy++)
		{
		gotoxy(wordx,wordy);
		s = wordy-MENU_TOP;
		cprintf("%s",MenuChoice[MenuSet].text[s]);
	}
	textcolor(BLUE);
	gotoxy(CursorX,MENU_BOTTOM+2);
	cprintf("읕컴컴컴컴컴컴컴컴컴컴컴컴켸");
	for(int t = MENU_TOP+1;t < MENU_BOTTOM+2;t++)
		{
		gotoxy(CursorX,t);
		cprintf("�");
		gotoxy(CursorX+(CURSOR_WIDTH+2),t);
		cprintf("�");
	}
	textcolor(GREEN);
	ShowSetup();
}


void RunDemo(void)
{
	done=1;
}

void DispText(void)
{
	gotoxy(1,1);
	cprintf("You are reading the text file aren't you lucky");
	PutCursor();
}

void MainMenu(void)
{
	MenuSet= 1;
	PutMenu();
	PutCursor();
}

void PickSound(void)
{
	MenuSet=2;
	PutMenu();
	PutCursor();
}

void SetBlaster(void)
{
	device=1;
	MenuSet=4;
	PutMenu();
	PutCursor();
}

void SetPortMenu(void)
{
	MenuSet=5;
	PutMenu();
	PutCursor();
}

void SetDmaMenu(void)
{
	MenuSet=6;
	PutMenu();
	PutCursor();
}

void SetIrqMenu(void)
{
	MenuSet=7;
	PutMenu();
	PutCursor();
}

void SetSpeaker(void)
{
	device=2;
	PutMenu();
	PutCursor();
}

void SetDisney(void)
{
	device=3;
	PutMenu();
	PutCursor();
}

void SetLoop(void)
{
	MenuSet=3;
	PutMenu();
	PutCursor();
}

void LoopSet(void)
{
	if (CurrItem== 1)
		{
		looping=1;
		strloop="On ";
	}
	else if(CurrItem== 2)
		{
		looping=0;
		strloop="Off";
	}
	PutMenu();
	PutCursor();
}

void ChangeSbSet(void)
{
	if (MenuSet== 5)
		{
		bPort=MenuChoice[MenuSet].port[CurrItem];
		cPort=MenuChoice[MenuSet].text[CurrItem];
		SetBlaster();
	}
	if (MenuSet== 6)
		{
		bDma=MenuChoice[MenuSet].dma[CurrItem];
		cDma=MenuChoice[MenuSet].text[CurrItem];
		SetBlaster();
	}
	if (MenuSet== 7)
		{
		bIrq=MenuChoice[MenuSet].irq[CurrItem];
		cIrq=MenuChoice[MenuSet].text[CurrItem];
		SetBlaster();
	}
}

int DoMenu(void)
{
	clrscr();
	_setcursortype(_NOCURSOR);
	textcolor(GREEN);

	for(int i=0;i<4000;i+=2)
		{
		if(MENU[i]!=' ')
			{
			_DI=i;
			asm mov ax,0xb800
			asm mov es,ax
			_AL=MENU[i];
			_AH=MENU[i+1];
			asm mov es:[di],ax
		}
	}

	MenuChoice=new MENU_CHOICE[7];

	MenuChoice[1].text[1]="Run The Demo";
	MenuChoice[1].text[2]="Choose Sound Device";
	MenuChoice[1].text[3]="Set Demo Looping";
	MenuChoice[1].text[4]="Display text file";
	MenuChoice[1].execute[1]=RunDemo;
	MenuChoice[1].execute[2]=PickSound;
	MenuChoice[1].execute[3]=SetLoop;
	MenuChoice[1].execute[4]=DispText;
	MenuChoice[1].NoEntries=4;


	MenuChoice[2].text[1]="Sound Blaster";
	MenuChoice[2].text[2]="PC speaker";
	MenuChoice[2].text[3]="Disney Sound source";
	MenuChoice[2].text[4]="go back to main menu";
	MenuChoice[2].execute[1]=SetBlaster;
	MenuChoice[2].execute[2]=SetSpeaker;
	MenuChoice[2].execute[3]=SetDisney;
	MenuChoice[2].execute[4]=MainMenu;
	MenuChoice[2].NoEntries=4;


	MenuChoice[3].text[1]="demo looping on";
	MenuChoice[3].text[2]="demo looping off";
	MenuChoice[3].text[3]="go back to main menu";
	MenuChoice[3].execute[1]=LoopSet;
	MenuChoice[3].execute[2]=LoopSet;
	MenuChoice[3].execute[3]=MainMenu;
	MenuChoice[3].NoEntries=3;


	MenuChoice[4].text[1]="Set port address";
	MenuChoice[4].text[2]="Set IRQ address";
	MenuChoice[4].text[3]="Set DMA channel";
	MenuChoice[4].text[4]="go back to sound menu";
	MenuChoice[4].execute[1]=SetPortMenu;
	MenuChoice[4].execute[2]=SetIrqMenu;
	MenuChoice[4].execute[3]=SetDmaMenu;
	MenuChoice[4].execute[4]=PickSound;
	MenuChoice[4].NoEntries=4;


	MenuChoice[5].text[1]="210h";
	MenuChoice[5].text[2]="220h";
	MenuChoice[5].text[3]="230h";
	MenuChoice[5].text[4]="240h";
	MenuChoice[5].text[5]="250h";
	MenuChoice[5].text[6]="260h";
	MenuChoice[5].execute[1]=ChangeSbSet;
	MenuChoice[5].execute[2]=ChangeSbSet;
	MenuChoice[5].execute[3]=ChangeSbSet;
	MenuChoice[5].execute[4]=ChangeSbSet;
	MenuChoice[5].execute[5]=ChangeSbSet;
	MenuChoice[5].execute[6]=ChangeSbSet;
	MenuChoice[5].port[1]=0x210;
	MenuChoice[5].port[2]=0x220;
	MenuChoice[5].port[3]=0x230;
	MenuChoice[5].port[4]=0x240;
	MenuChoice[5].port[5]=0x250;
	MenuChoice[5].port[6]=0x260;
	MenuChoice[5].NoEntries=6;


	MenuChoice[6].text[1]="1";
	MenuChoice[6].text[2]="3";
	MenuChoice[6].text[3]="5";
	MenuChoice[6].text[4]="7";
	MenuChoice[6].execute[1]=ChangeSbSet;
	MenuChoice[6].execute[2]=ChangeSbSet;
	MenuChoice[6].execute[3]=ChangeSbSet;
	MenuChoice[6].execute[4]=ChangeSbSet;
	MenuChoice[6].dma[1]=1;
	MenuChoice[6].dma[2]=3;
	MenuChoice[6].dma[3]=5;
	MenuChoice[6].dma[4]=7;
	MenuChoice[6].NoEntries=4;


	MenuChoice[7].text[1]="2";
	MenuChoice[7].text[2]="3";
	MenuChoice[7].text[3]="5";
	MenuChoice[7].text[4]="7";
	MenuChoice[7].execute[1]=ChangeSbSet;
	MenuChoice[7].execute[2]=ChangeSbSet;
	MenuChoice[7].execute[3]=ChangeSbSet;
	MenuChoice[7].execute[4]=ChangeSbSet;
	MenuChoice[7].irq[1]=2;
	MenuChoice[7].irq[2]=3;
	MenuChoice[7].irq[3]=5;
	MenuChoice[7].irq[4]=7;
	MenuChoice[7].NoEntries=4;


	PutMenu();
	PutCursor();
	done=0;
	while(!done)
		{
		CursorPos();
		if(_bios_keybrd(_KEYBRD_READY))
			{
			key=_bios_keybrd(_KEYBRD_READ);

			switch(key)
				{
					case Enter:
						{
						(MenuChoice[MenuSet].execute[CurrItem])();
						break;
					}
					case Escape:
						{
						return 1;
					}
					case up_arrow:
						{
						EraseCursor();
						CursorY--;
						PutCursor();
						break;
					}
					case down_arrow:
						{
						EraseCursor();
						CursorY++;
						PutCursor();
						break;
					}
			}
			key=0;
		}
	}
	_setcursortype(_NORMALCURSOR);
	clrscr();

	delete MenuChoice;

     return 0;
}