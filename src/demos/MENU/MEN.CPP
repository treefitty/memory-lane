
/*                        BLACK MARBLE'S MENU SYSTEM.
					ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´


	1-4-1995 : Version 1.0 finished and ready for testing.
			 Existing structure of data file MENU.DAT

			 1 byte - Starting Item.
			 1 byte - Starting Page.
			 6 bytes- Header BMIMS.

			 169 Fields each field :

					26 bytes - Item description.
					13 bytes - Item's call or executable name.
					60 bytes - For item's working directory.

			 13 Fields each field :

					27 bytes - Description of pages.

			 Menu has the following functions :

					- Up and Down scrolling for 13 pages each 13 items.
					- Help window and other waring windows.
					- Edit windows.
					- View window to view an item's properties.
					- Deleting of either items or pages.
					- Moving of items to other pages.
					- Moving of a page and it's contents to another page.
					- Checking of datafile when program loads.
					- Ability to remain at same position after
					  selected program has been executed.
					- Uses 0 bytes of memory when another program
					  is running.

	2-4-1995 : Version 1.1

			 Small bug fix in the moving of items.

	3-4-1995 : Version 2.0

			 Decision was made that the program field needed 20 chars
			 instead of 13. This meant restructuring the data file and
			 keeping as much of my existing data as possible. Data file
			 is now :
					- Same header as last version
							- 1 byte start item.
							- 1 byte start page.
							- 6 bytes for signature : BMIMS1

					- 8 byte offset to Item data.
					- 17922 byte offset to Page data.

	3-4-1995 : Version 2.1

			 - Sets text mode to clear out modified palette regs at the
			   beginning of the program.
			 - When editing a Page or Item NULL chars will not be input
			   if another key such as F# or an arrow is pressed.

	4-4-1995 : Version 2.2

			 - New edit features. The 'Del' key and 'Ins' key can now
			   both be used as far as I can tell.
			 - Fixed small bug in 'Del' key routine.

	4-4-1995 : Version 2.3

			 - Bug fix in page moving routine. If page was over 10 then
			   it grabbed the last item on the previous page.
			 - Now also checks to see if a VGA card is present.
			 - Sets 80x25 text mode just to be on the safe side.
			 - Fixed small bug in 'Backspace' routine.
			 - Fixed a bug in the moving of items. The temp field
			   wasn't being blanked and was sometimes recieving something
			   like 23 for the move page if the previous move was to 13

	5-4-1995 : Version 2.4

			 - Bug fix in 'Del' key
			 - Typo fix in Help window.
			 - Option to delete every item on the current page.

	10-4-1995: Version 2.5

			 - Added 'End' and 'Home' keys to edit function.
			 - Changed the names so to run the menu you type MENU

	12-4-1995: Version 2.6

			 - Small bug fix. Problem of computer hanging sometimes.
			   Added a small routine to 'flush' the kb buffer after
			   the key pressed had been read.
			 - Char 254 or þ if encountered in a string now signifys
			   the end. (This was needed for and improvement on the
			   'End' key function when editing.

	13-4-1995: Version 2.7

			 - More improvements on the 'End' function and string editing
			   function.
			 - Fixed some more Backspace bugs.
			 - Little bug fix with 'F1' help key routine.

	15-4-1995: Version 2.8

			 - Bug fix in execution of program. Drive wasn't being logged
			   properly. Now logs drive then changes dir.

	16-4-1995: - Bug fix on execution, was only writing 13 chars.
			 - Fix to refresh data file function, was 16 byte too large.

	18-4-1995: Version 2.9

			 - Nice scrolly bit at start and end
			 - No more SLAVE.BAT after execution

	5-5-1995:  Version 2.91

			 - Addition of BLACK MARBLE at the bottom of the screen

	26-5-1995: Version 2.92

			 - Checks if moving is to same position, used to blank the
			   feilds if this happened without checking.
	6-6-1995:  - Found and fixing bug in length of path name, only using 26

	30-6-1995: Version 2.93

			 - Added a more refined editing function, few bugs found.
			 - Every time the menu loads it starts on last menu item/page.

	10-7-1995: Version 4.10    - Skipped ver 3 because windoze is dumb.

			 - Better disk access.
			 - Page deleting bug corrected.

	11-7-1995: Version 4.20á

			 - Red bars showing destination position and warning window
			   on all moving options.
			 - Keyboard poll rate set to maximum
			 - Finally found bug in small window data entry

	1-9-1995: Version 4.21á

			 - Single little bug squashed in line editing function

	3-9-1995: Version 4.22á

			 - Really squahed that bug, and another created in the
			   last version.

	4-9-1995: Version 4.23á

			 - An enhanced line editor, added type definitions

   16-10-1995: Version 4.25í

			 - Now CALLs the program so when a batch file is called
			   it goes back to the menu.

    7-11-1995: Version 4.26í

			 - Cleaned up some of the code and made line editor
			   insert as default
			 - Small bug in writing of SLAVE.BAT squashed as well.

    3-12-1995: Version 4,27í

			 - Final bug squashed in page moving. Old page wasn't getting
			   EOL char!
			 - More code clean up. (It's way too messy!)

    23-12-1995: Version 4.28í

			 - Page moving finally fixed!!!!!!!
*/

#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <bios.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>

#include <iedit.h>
#include <ikeybrd.h>
#include <text.h>

#include "menu.h"
#include "credits.h"

#define PageOffs 	17922
#define FILE_SIZE  	18273
#define Insert      0x5200
#define Del         0x5300
#define Enter       0x1c0d
#define F1_key      0x3b00
#define home        0x4700
#define end	     0x4f00
#define page_down   0x5100
#define page_up     0x4900
#define enter       0x1c0d
#define backspace   0xe08
#define right_arrow 0x4d00
#define left_arrow  0x4b00
#define up_arrow    0x4800
#define down_arrow  0x5000
#define escape      0x11b

extern "C" {

	void ScrlUp(void);
	void ScrlDown(void);
	void SetPage(int page);
	void CopyScreen(void);
}

char far ItemsText[170][26];
char far ItemsProgram[170][20];
char far ItemsPath[170][60];
char far PagesText[14][27];
char far TextTemp[28];
char far ProgTemp[21];
char far PathTemp[61];
FILE *DATAFILE;
FILE *out;

class WINDOW {
private:

	int height;
	int width;
	int x1,y1,x2,y2;
	int pointer;
	int WinAttr;
	unsigned char far background[4000];

public:

	int x,y;
	void GetBackground(void);
	void RestoreBackground(void);
	/* Attr defines the attribute for the border and background */
	void PutWin(int x1,int y1,int x2,int y2,char Attr);
	void PutHeader(char Header[],char HAttr,int size,int offset);
	void DeleteWin(void);
};

void WINDOW::GetBackground(void)
{
	char far *Scr=(char *)MK_FP(0xb800,0);
	int i;

	for(i=0;i<4000;i++) background[i]=Scr[i];
}

void WINDOW::RestoreBackground(void)
{
	char far *Scr=(char *)MK_FP(0xb800,0);
	int i;

	for(i=0;i<4000;i++) Scr[i]=background[i];
}

void WINDOW::PutWin(int ax1,int ay1,int ax2,int ay2,char Attr)
{
	WinAttr=Attr;
	width=ax2-ax1;
	height=ay2-ay1;
	x1=ax1;y1=ay1;
	x2=ax2;y2=ay2;
	GetBackground();
	for(y=y1;y<=y2;y++)                        // Fill area for window to go
		{
		for(x=x1;x<=x2;x++)
			{
			PutChar(x,y,32,WinAttr);
		}
	}
	for(x=x1+1;x<x1+width;x++)
		{
		PutChar(x,y1,205,WinAttr);
		PutChar(x,y1+height,205,WinAttr);
	}
	for(y=y1;y<y1+height;y++)
		{
		PutChar(x1,y,186,WinAttr);
		PutChar(x1+width,y,186,WinAttr);
	}
	for(y=y2+1,x=x1+2;x<=x2+1;x++)             // Bottom shadow
		{
		ChangeAttr(x,y,0x08);
	}
	for(x=x2+1,y=y1+1;y<=y2+1;y++)             // Right side shadow
		{
		ChangeAttr(x,y,0x08);
		ChangeAttr(x+1,y,0x08);
	}
	PutChar(x1,y1,201,WinAttr);                // Corners    t-l
	PutChar(x2,y1,187,WinAttr);                //            t-r
	PutChar(x1,y2,200,WinAttr);                //            b-l
	PutChar(x2,y2,188,WinAttr);                //		  b-r
}

void WINDOW::PutHeader(char Header[],char HAttr,int size,int offset)
{
	int i;

	x=x1+offset;
	for(i=0;i<size;i++)
		{
		PutChar(x+i,y1,Header[i],HAttr);
	}
	PutChar(x-1,y1,181,WinAttr);
	PutChar(x+size,y1,198,WinAttr);
}

void WINDOW::DeleteWin(void)
{
	RestoreBackground();
}

WINDOW HelpWin;
WINDOW Warning;
WINDOW EditWin;
WINDOW MoveWin;

class MENU {
private:

	int i;
	int x,y;

public:

	int CurrItem;
	int cursorx,cursory;
	int width;
	int maxx;
	int ItemCount;

	MENU(int startx,int starty,int w);
	void PutCursor(void);
	void EraseCursor(void);

};

MENU::MENU (int startx,int starty,int w)
{
	cursorx=startx;
	cursory=starty;
	ItemCount=1;
	CurrItem=1;
	width=w;
	y=starty;
	maxx=startx+width-1;
}

void MENU::PutCursor(void)
	{
	for(y=cursory,x=cursorx;x<=maxx;x++)
		{
		ChangeAttr(x,y,0x1A);
	}
	PutChar(cursorx,cursory,175,0x1F);
	PutChar(maxx,cursory,174,0x1F);
}

void MENU::EraseCursor(void)
{
	for(y=cursory,x=cursorx;x<=maxx;x++)
		{
		ChangeAttr(x,y,0x3F);
	}
	PutChar(cursorx+5,cursory,'-',0x3B);
	PutChar(cursorx,cursory,' ',0x3F);
	PutChar(maxx,cursory,' ',0x3F);
}

MENU ItemsMenu(2,8,36);
MENU PagesMenu(41,8,37);

void RefreshDataFile(void)
{
	int i,l;

	FILE *out;
	out=fopen("MENU.DAT","wb");
	fseek(out,0,0);
	fputc(1,out);
	fputc(1,out);
	fprintf(out,"BMIMS1");
	for(l=1;l<=169;l++)
		{
		for(i=1;i<26;i++) ItemsText[l][i]   =32;
		for(i=1;i<20;i++) ItemsProgram[l][i]=32;
		for(i=1;i<60;i++) ItemsPath[l][i]   =32;
		ItemsText[l][0]   =254;
		ItemsProgram[l][0]=254;
		ItemsPath[l][0]   =254;
	}
	for(l=1;l<=13;l++)
		{
		PagesText[l][0]=254;
		for(i=1;i<27;i++)
			{
			PagesText[l][i]=32;
		}
	}
	fseek(out,8,0);
	for(l=1;l<=169;l++)
		{
		fwrite(&ItemsText[l],26,1,out);
		fwrite(&ItemsProgram[l],20,1,out);
		fwrite(&ItemsPath[l],60,1,out);
	}
	for(l=1;l<=13;l++)
		{
		fwrite(&PagesText[l],27,1,out);
	}
	fclose(out);
}

void ReadDataFile(void)
{
	int i;
	FILE *in;

	in=fopen("MENU.DAT","rb");
	fseek(in,8,0);
	for(i=1;i<=169;i++)
		{
		fread(ItemsText[i],26,1,in);
		fread(ItemsProgram[i],20,1,in);
		fread(ItemsPath[i],60,1,in);
	}
	for(i=1;i<=13;i++)
		{
		fread(PagesText[i],27,1,in);
	}
	fclose(in);
}

void RefreshItems(int CurrPage)
{
	int i,k,l;

	char startItem,endItem;

	startItem=((CurrPage-1)*13)+1;
	endItem=CurrPage*13;
	for(i=startItem,k=8;i<=endItem;i++,k++)
		{
		for(l=0;l<25;l++)
			{
			if(ItemsText[i][l]==254) break;
			PutChar(10+l,k,ItemsText[i][l],0x3F);
		}
		if(l!=25)
			for(;l<25;l++)
				{
				PutChar(10+l,k,32,0x3F);
			}
	}
}

void RefreshPages(void)
{
	int i,k,l;

	for(i=1,k=8;i<=13;i++,k++)
		{
		for(l=0;l<26;l++)
			{
			if(PagesText[i][l]==254) break;
			PutChar(48+l,k,PagesText[i][l],0x3F);
		}
	}
}

void Update(void)
{
	textbackground(0);
	textcolor(15);
	textbackground(3);
	gotoxy(12,6);
	cprintf("%i ",PagesMenu.CurrItem);
	gotoxy(28,6);
	cprintf("%i ",ItemsMenu.ItemCount);
}

void Blank(unsigned char *array,int items)
{
	int i;

	for(i=0;i<items;i++) array[i]=32;
}

void current_directory(char *path)
{
	strcpy(path, "X:\\");
	path[0] = 'A' + getdisk();
	getcurdir(0, path+3);
}

void main(void)
{
	char far *Scr=(char *)MK_FP(0xb800,0);
	char done;
	char curdir[MAXPATH];
	int pointer;
	int key;
	int x,y=0,i,l;
	int result;
	int MovePage;
	int MoveItem;
	int Size,Handle;

	asm mov ax, 0x1a00
	asm int     0x10
	asm cmp bl, 7
	asm jae     Ok

	printf("\nYou need a VGA graphics card\n");
	printf("to run this program.");
	asm mov ax,	0x4c00
	asm int		0x21

Ok:
	flushkeybrd();
	textcolor(15);
	textbackground(0);

	/* Get startup directory so after execution the menu
	   can swap back to it's dir. */
	current_directory(curdir);

	/* Open data file and check the integrity. */
	if((Handle=open("MENU.DAT", O_RDONLY))==-1) result=1;
	Size=filelength(Handle);
	close(Handle);
	if(Size!=FILE_SIZE || result==1)
		{
		Warning.PutWin(20,5,60,12,0x4F);
		Warning.PutHeader("Warning",0x4E,7,17);
		gotoxy(24,8);
		textcolor(15);
		textbackground(4);
		cprintf("Warning! Data file MENU.DAT has been");
		gotoxy(22,9);
		cprintf("damaged and the checksums do not match.");
		gotoxy(26,11);
		cprintf("Create a new data file? Y/N [ ]");
		gotoxy(wherex()-2,wherey());

		done=0;
		while(!done)
			{
			if(_bios_keybrd(_KEYBRD_READY)!=0)
				{
				key=_bios_keybrd(_KEYBRD_READ);
				key=toupper(toascii(key));
				if(key=='Y' || key=='N')
					{
					done=1;
					cprintf("%c",key);
				}
				else {
					if(key>=32 && key<=126)
						{
						cprintf("%c",key);
						gotoxy(wherex()-1,wherey());
					}
				}
			}
		}

		if(key=='Y')
			{
			RefreshDataFile();
			done=1;
		}
		else	{
			gotoxy(1,1);
			textcolor(15);
			textbackground(0);
			clrscr();
			cprintf("Error: File recreation aborted.\r\n\n");
			asm mov ax,0x4c00
			asm int	0x21
		}
		Warning.DeleteWin();
	}

	DATAFILE=fopen("MENU.DAT","rb+");
	fseek(DATAFILE,0,0);
	MoveItem=fgetc(DATAFILE);
	MovePage=fgetc(DATAFILE);
	for(i=0;i<6;i++) TextTemp[i]=fgetc(DATAFILE);
	fclose(DATAFILE);
	result=strcmp(TextTemp,"BMIMS1");
	if(result!=0)
		{
		textcolor(15);
		textbackground(4);
		Warning.PutWin(19,5,61,13,0x4F);
		Warning.PutHeader("Warning",0x4E,7,17);
		gotoxy(23,8);
		textcolor(15);
		textbackground(4);
		cprintf("Warning! The data file MENU.DAT found");
		gotoxy(21,9);
		cprintf("in this directory will not work with this");
		gotoxy(35,10);
		cprintf("menu system");
		gotoxy(26,12);
		cprintf("Create a new data file? Y/N [ ]");
		gotoxy(wherex()-2,wherey());

		while(!done)
			{
			if(_bios_keybrd(_KEYBRD_READY)!=0)
				{
				key=_bios_keybrd(_KEYBRD_READ);
				key=toupper(toascii(key));
				if(key=='Y' || key=='N')
					{
					done=1;
					cprintf("%c",key);
				}
				else {
					if(key>=32 && key<=126)
						{
						cprintf("%c",key);
						gotoxy(wherex()-1,wherey());
					}
				}
			}
		}
		done=0;

		if(key=='Y')
			{
			RefreshDataFile();
			MoveItem=1;
			MovePage=1;
			done=1;
		}
		else	{
			clrscr();
			textcolor(15);
			textbackground(0);
			gotoxy(1,1);
			cprintf("Error: File recreation aborted.\r\n\n");
			asm mov ax,0x4c00                  // Terminat program
			asm int	0x21
		}

		Warning.DeleteWin();
	}
	fclose(DATAFILE);
	/* Finish checking integrity */

	_setcursortype(_NOCURSOR);

	/* Put Menu to screen. */
	for(y=25,i=0;y<=49;y++)
		{
		for(x=0;x<80;x++)
			{
			PutChar(x,y,MENUDATA[i],MENUDATA[i+1]);
			i+=2;
		}
	}

	for(i=0;i<25;i++) ScrlUp();
	SetPage(1);

	for(y=0,i=0;y<=24;y++)
		{
		for(x=0;x<80;x++)
			{
			PutChar(x,y,MENUDATA[i],MENUDATA[i+1]);
			i+=2;
		}
	}

	asm mov 	dx,0x3c8     // Define BLUE to a DARKGRAY color
	asm mov 	al,1
	asm out 	dx,al
	asm inc	dx
	asm mov 	cx,3
	asm mov 	al,15

setBlue:

	asm out 	dx,al
	asm loop	setBlue

	/* Make sure SLAVE.BAT is empty */
	out=fopen("SLAVE.BAT","wt+");
	fclose(out);
	gotoxy(72,1);
	textcolor(15);
	textbackground(0);
	cprintf("Ver 4.28í");                       /* <----- Version number */
	ReadDataFile();
	ItemsMenu.CurrItem=((MovePage-1)*13)+MoveItem;
	ItemsMenu.ItemCount=MoveItem;
	PagesMenu.CurrItem=MovePage;
	PagesMenu.ItemCount=MovePage;
	RefreshItems(PagesMenu.CurrItem);
	RefreshPages();
	ItemsMenu.cursory=7+MoveItem;
	PagesMenu.cursory=7+MovePage;
	ItemsMenu.PutCursor();
	PagesMenu.PutCursor();
	Update();
	done=0;

	asm mov	ax,0x0305             // Set keyboard to high poll rate
	asm mov	bx,0x0000
	asm int	0x16

	while(!done)
		{
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			key=_bios_keybrd(_KEYBRD_READ);
			flushkeybrd();

			switch(key)
				{
				case escape:
					{
					done=1;
					break;
				}
				case down_arrow:
					{
					if(ItemsMenu.CurrItem!=169)
						{
						if(ItemsMenu.ItemCount!=13)
							{
							ItemsMenu.EraseCursor();
							ItemsMenu.cursory++;
							ItemsMenu.PutCursor();
							ItemsMenu.ItemCount++;
							ItemsMenu.CurrItem++;
							Update();
						}
						else {
							if(PagesMenu.ItemCount!=13)
								{
								ItemsMenu.EraseCursor();
								ItemsMenu.cursory=8;
								ItemsMenu.ItemCount=1;
								ItemsMenu.CurrItem++;

								PagesMenu.EraseCursor();
								PagesMenu.cursory++;
								PagesMenu.PutCursor();
								PagesMenu.ItemCount++;
								PagesMenu.CurrItem++;

								RefreshItems(PagesMenu.CurrItem);
								ItemsMenu.PutCursor();
								Update();
							}
						}
					}
					break;
				}
				case up_arrow:
					{
					if(ItemsMenu.CurrItem!=0)
						{
						if(ItemsMenu.ItemCount!=1)
							{
							ItemsMenu.EraseCursor();
							ItemsMenu.cursory--;
							ItemsMenu.PutCursor();
							ItemsMenu.ItemCount--;
							ItemsMenu.CurrItem--;
							Update();
						}
						else {
							if(PagesMenu.ItemCount!=1)
								{
								ItemsMenu.EraseCursor();
								ItemsMenu.cursory=20;
								ItemsMenu.PutCursor();
								ItemsMenu.ItemCount=13;
								ItemsMenu.CurrItem--;

								PagesMenu.EraseCursor();
								PagesMenu.cursory--;
								PagesMenu.PutCursor();
								PagesMenu.ItemCount--;
								PagesMenu.CurrItem--;

								RefreshItems(PagesMenu.CurrItem);
								ItemsMenu.PutCursor();
								Update();
							}
						}
					}
					break;
				}
				case page_down:
					{
					if(PagesMenu.CurrItem!=13)
						{
						ItemsMenu.CurrItem+=13;
						PagesMenu.EraseCursor();
						PagesMenu.cursory++;
						PagesMenu.CurrItem++;
						PagesMenu.ItemCount++;
						PagesMenu.PutCursor();
						RefreshItems(PagesMenu.CurrItem);
						ItemsMenu.PutCursor();
						Update();
					}
					break;
				}
				case page_up:
					{
					if(PagesMenu.CurrItem!=1)
						{
						ItemsMenu.CurrItem-=13;
						PagesMenu.EraseCursor();
						PagesMenu.cursory--;
						PagesMenu.CurrItem--;
						PagesMenu.ItemCount--;
						PagesMenu.PutCursor();
						RefreshItems(PagesMenu.CurrItem);
						ItemsMenu.PutCursor();
						Update();
					}
					break;
				}
				case home:
					{
					ItemsMenu.CurrItem=ItemsMenu.ItemCount;
					PagesMenu.EraseCursor();
					PagesMenu.CurrItem=1;
					PagesMenu.ItemCount=1;
					PagesMenu.cursory=8;
					PagesMenu.PutCursor();
					RefreshItems(PagesMenu.CurrItem);
					ItemsMenu.PutCursor();
					Update();
					break;
				}
				case end:
					{
					ItemsMenu.CurrItem=156+ItemsMenu.ItemCount;
					PagesMenu.EraseCursor();
					PagesMenu.CurrItem=13;
					PagesMenu.ItemCount=13;
					PagesMenu.cursory=20;
					PagesMenu.PutCursor();
					RefreshItems(PagesMenu.CurrItem);
					ItemsMenu.PutCursor();
					Update();
					break;
				}
				case F1_key:
					{
					HelpWin.PutWin(20,3,60,24,0x2F);
					HelpWin.PutHeader("Help",0x26,4,19);
					textbackground(2);
					textcolor(15);
					gotoxy(23,6);
					cprintf("P     - Edit current PAGE.");
					gotoxy(23,7);
					cprintf("E     - Edit current ITEM.");
					gotoxy(23,8);
					cprintf("%c     - Move ITEM cursor up.",24);
					gotoxy(23,9);
					cprintf("%c     - Move ITEM cursor down.",25);
					gotoxy(23,10);
					cprintf("PgDn  - Move PAGE cursor down.");
					gotoxy(23,11);
					cprintf("PgUp  - Move PAGE cursor up.");
					gotoxy(23,12);
					cprintf("Home  - Goto first PAGE.");
					gotoxy(23,13);
					cprintf("End   - Goto last PAGE.");
					gotoxy(23,14);
					cprintf("Esc   - Exit program.");
					gotoxy(23,15);
					cprintf("D     - Delete current ITEM.");
					gotoxy(23,16);
					cprintf("Alt-D - Delete current PAGE.");
					gotoxy(23,17);
					cprintf("V     - View an ITEM'S properties.");
					gotoxy(23,18);
					cprintf("M     - Move single ITEM to another ");
					gotoxy(31,19);
					cprintf("Page,Item.");
					gotoxy(23,20);
					cprintf("ALT-M - Move PAGE and it's ITEMS to ");
					gotoxy(31,21);
					cprintf("another PAGE.");
					gotoxy(23,22);
					cprintf("ALT-C - Clear current PAGE'S ITEMS.");
					gotoxy(23,23);
					cprintf("Enter - Execute current ITEM.");
					flushkeybrd();
					while(!done)
						{
						if(_bios_keybrd(_KEYBRD_READY)!=0)
							{
							key=_bios_keybrd(_KEYBRD_READ);
							done=1;
						}
					}
					done=0;
					HelpWin.DeleteWin();
					break;
				}
				case Enter:
					{
					out=fopen("MENU.DAT","rb+");
					fseek(out,0,0);
					fputc(ItemsMenu.ItemCount,out);
					fputc(PagesMenu.ItemCount,out);
					fclose(out);
					out=fopen("SLAVE.BAT","wt+");
					fprintf(out,"@ECHO OFF \n\r ");
					fprintf(out,"@%c%c \n\r",ItemsPath[ItemsMenu.CurrItem][0],ItemsPath[ItemsMenu.CurrItem][1]);
					fprintf(out,"@cd ");
					for(i=2;i<59;i++)
						{
						if(ItemsPath[ItemsMenu.CurrItem][i]==254)
							{
							goto EndWrite1;
						}
						fputc(ItemsPath[ItemsMenu.CurrItem][i],out);
					}
					EndWrite1:

					fprintf(out," \n\r @call ");
					for(i=0;i<20;i++)
						{
						if(ItemsProgram[ItemsMenu.CurrItem][i]==254)
							{
							goto EndWrite2;
						}
						fputc(ItemsProgram[ItemsMenu.CurrItem][i],out);
					}
					EndWrite2:
					fprintf(out,"\n\r @%c%c",curdir[0],curdir[1]);
					fprintf(out,"\n\r @CD ");
					for(i=2;i<strlen(curdir);i++)
						{
						fputc(curdir[i],out);
					}
					fprintf(out,"\n\r @MENU");
					fclose(out);

					textcolor(15);
					textbackground(0);

					CopyScreen();

					SetPage(2);
					for(y=0,i=0;y<=24;y++)
						{
						for(x=0;x<80;x++)
							{
							PutChar(x,y,32,0x0F);
							i+=2;
						}
					}
					flushkeybrd();
					for(i=0;i<25;i++)
						{
						ScrlDown();
					}
					flushkeybrd();
					_setcursortype(_NORMALCURSOR);

					asm mov ax,0x03          // Clear background color
					asm int 	 0x10          // and modified pal regs
					asm mov ax,0x4c00        // Terminate program
					asm int	 0x21
					break;
				}
			}  // End switch
			if(toascii(key)=='e'||toascii(key)=='E')
				{
				_setcursortype(_NORMALCURSOR);
				EditWin.PutWin(5,5,75,14,0x6E);
				EditWin.PutHeader("Edit Item",0x6F,9,31);
				PutChar(8,10,'[',0x6E);
				PutChar(34,10,']',0x6E);
				gotoxy(19,8);
				textcolor(15);
				textbackground(0x0E);
				cprintf("Enter description for ITEM (25 Chars Max):");
				gotoxy(10,11);
				l=0;
				Blank(TextTemp,26);
				while(!done)
					{
					if((int)ItemsText[ItemsMenu.CurrItem][l]==254 || l==25)
						{
						TextTemp[l]=254;
						done=1;
					}
					else {
						PutChar(9+l,10,ItemsText[ItemsMenu.CurrItem][l],0x6F);
						TextTemp[l]=ItemsText[ItemsMenu.CurrItem][l];
					}
					l++;
				}
				done=0;
				result=Edit(TextTemp,25,0x6F,STRING);

				if(result==0)
					{
					gotoxy(11,8);
					cprintf("Enter NAME of program and parameters (eg. EXAMPLE -r *.mod) :   ");
					gotoxy(10,11);
					cprintf("                                                             ");
					gotoxy(10,11);
					PutChar(28,10,']',0x6E);
					l=0;
					Blank(ProgTemp,20);
					while(!done)
						{
						if((int)ItemsProgram[ItemsMenu.CurrItem][l]==254 || l==26)
							{
							ProgTemp[l]=254;
							done=1;
						}
						else {
							PutChar(9+l,10,ItemsProgram[ItemsMenu.CurrItem][l],0x6F);
							ProgTemp[l]=ItemsProgram[ItemsMenu.CurrItem][l];
						}
						l++;
					}
					done=0;
					result=Edit(ProgTemp,19,0x6F,STRING);

					if(result==0)
						{
						gotoxy(11,8);
						cprintf("   Enter PATH for program (eg. C:\\EXAMPLE):                     ");
						gotoxy(10,11);
						cprintf("                                                             ");
						gotoxy(10,11);
						PutChar(68,10,']',0x6E);
						l=0;
						Blank(PathTemp,60);
						while(!done)
							{
							if((int)ItemsPath[ItemsMenu.CurrItem][l]==254 || l==60)
								{
								PathTemp[l]=254;
								done=1;
							}
							else {
								PutChar(9+l,10,ItemsPath[ItemsMenu.CurrItem][l],0x6F);
								PathTemp[l]=ItemsPath[ItemsMenu.CurrItem][l];
							}
							l++;
						}
						done=0;
						result=Edit(PathTemp,59,0x6F,STRING);

						if(result==0)
							{
							DATAFILE=fopen("MENU.DAT","rb+");
							fseek(DATAFILE,((ItemsMenu.CurrItem-1)*26)+((ItemsMenu.CurrItem-1)*60)+((ItemsMenu.CurrItem-1)*20)+8,0);
							for(i=0;i<26;i++)
								{
								ItemsText[ItemsMenu.CurrItem][i]=TextTemp[i];
							}
							for(i=0;i<20;i++)
								{
								ItemsProgram[ItemsMenu.CurrItem][i]=ProgTemp[i];
							}
							for(i=0;i<60;i++)
								{
								ItemsPath[ItemsMenu.CurrItem][i]=PathTemp[i];
							}

							fwrite(&ItemsText[ItemsMenu.CurrItem],26,1,out);
							fwrite(&ItemsProgram[ItemsMenu.CurrItem],20,1,out);
							fwrite(&ItemsPath[ItemsMenu.CurrItem],60,1,out);

							fclose(DATAFILE);
						}
					}
				}
				EditWin.DeleteWin();
				Blank(TextTemp,27);
				Blank(ProgTemp,13);
				Blank(PathTemp,60);

				_setcursortype(_NOCURSOR);
				RefreshItems(PagesMenu.CurrItem);
				ItemsMenu.PutCursor();
			}
			if(toascii(key)=='p' || toascii(key)=='P')
				{
				EditWin.PutWin(5,5,75,14,0x6E);
				EditWin.PutHeader("Edit Page",0x6F,9,31);
				PutChar(8,10,'[',0x6E);
				PutChar(35,10,']',0x6E);
				_setcursortype(_NORMALCURSOR);
				gotoxy(15,8);
				textcolor(15);
				textbackground(6);
				cprintf("Enter PAGE description (27 Chars Max):");
				gotoxy(10,11);
				l=0;
				Blank(TextTemp,27);
				while(!done)
					{
					if((int)PagesText[PagesMenu.CurrItem][l]==254 || l==26)
						{
						TextTemp[l]=254;
						done=1;
					}
					else {
						PutChar(9+l,10,PagesText[PagesMenu.CurrItem][l],0x6F);
						TextTemp[l]=PagesText[PagesMenu.CurrItem][l];
					}
					l++;
				}
				done=0;
				result=Edit(TextTemp,26,0x6F,STRING);

				if(result==0)
					{
					DATAFILE=fopen("MENU.DAT","rb+");
					fseek(DATAFILE,PageOffs+((PagesMenu.CurrItem-1)*27),0);
					for(i=0;i<27;i++)
						{
						PagesText[PagesMenu.CurrItem][i]=TextTemp[i];
					}

					fwrite(&PagesText[PagesMenu.CurrItem],27,1,out);

					fclose(DATAFILE);
				}
				EditWin.DeleteWin();
				Blank(TextTemp,27);
				_setcursortype(_NOCURSOR);
				RefreshPages();
				PagesMenu.PutCursor();
			}
			if(toascii(key)=='d' || toascii(key)=='D')
				{
				Warning.PutWin(20,5,60,11,0x4F);
				Warning.PutHeader("Warning",0x4E,7,17);
				textcolor(15);
				textbackground(4);
				gotoxy(30,9);
				cprintf("Delete this ITEM? Y/N");
				while(done==0)
					{
					if(_bios_keybrd(_KEYBRD_READY)!=0)
						{
						key=_bios_keybrd(_KEYBRD_READ);
						key=toascii(key);
						if(key=='y'||key=='Y'||key=='n'||key=='N')
							{
							done=1;
						}
					}
				}
				done=0;
				Warning.DeleteWin();
				if(key=='y' || key=='Y')
					{
					Blank(ItemsText[ItemsMenu.CurrItem],26);
					Blank(ItemsProgram[ItemsMenu.CurrItem],20);
					Blank(ItemsPath[ItemsMenu.CurrItem],60);

					ItemsText[ItemsMenu.CurrItem][0]   =254;
					ItemsProgram[ItemsMenu.CurrItem][0]=254;
					ItemsPath[ItemsMenu.CurrItem][0]   =254;

					DATAFILE=fopen("MENU.DAT","rb+");
					fseek(DATAFILE,((ItemsMenu.CurrItem-1)*26)+((ItemsMenu.CurrItem-1)*60)+((ItemsMenu.CurrItem-1)*20)+8,0);

					fwrite(&ItemsText[ItemsMenu.CurrItem],26,1,out);
					fwrite(&ItemsProgram[ItemsMenu.CurrItem],20,1,out);
					fwrite(&ItemsPath[ItemsMenu.CurrItem],60,1,out);

					fclose(DATAFILE);
					RefreshItems(PagesMenu.CurrItem);
					ItemsMenu.PutCursor();
				}
			}
			if(key==8192)        // ALT-D for deleting pages
				{
				Warning.PutWin(20,5,60,11,0x4F);
				Warning.PutHeader("Warning",0x4E,7,17);
				textcolor(15);
				textbackground(4);
				gotoxy(30,9);
				cprintf("Delete this PAGE? Y/N");
				while(done==0)
					{
					if(_bios_keybrd(_KEYBRD_READY)!=0)
						{
						key=_bios_keybrd(_KEYBRD_READ);
						key=toascii(key);
						if(key=='y'||key=='Y'||key=='n'||key=='N')
							{
							done=1;
						}
					}
				}
				done=0;
				Warning.DeleteWin();
				if(key=='y'||key=='Y')
					{
					for(i=0;i<27;i++) PagesText[PagesMenu.CurrItem][i]=32;
					PagesText[PagesMenu.CurrItem][0]=254;

					DATAFILE=fopen("MENU.DAT","rb+");
					fseek(DATAFILE,PageOffs+((PagesMenu.CurrItem-1)*27),0);

					fwrite(&PagesText[PagesMenu.CurrItem],27,1,out);

					fclose(DATAFILE);
					RefreshPages();
					PagesMenu.PutCursor();
				}
			}
			if(key==11776)			// ALT-C for clearing pages
				{
				Warning.PutWin(20,5,60,11,0x4F);
				Warning.PutHeader("Warning",0x4E,7,17);
				textcolor(15);
				textbackground(4);
				gotoxy(26,9);
				cprintf("Delete ITEMS on this PAGE? Y/N");
				while(done==0)
					{
					if(_bios_keybrd(_KEYBRD_READY)!=0)
						{
						key=_bios_keybrd(_KEYBRD_READ);
						key=toascii(key);
						if(key=='y'||key=='Y'||key=='n'||key=='N')
							{
							done=1;
						}
					}
				}
				done=0;
				Warning.DeleteWin();
				if(key=='y'||key=='Y')
					{
					for(l=((PagesMenu.CurrItem-1)*13)+1;l<=(PagesMenu.CurrItem*13);l++)
						{
						Blank(ItemsText[l],26);
						Blank(ItemsProgram[l],20);
						Blank(ItemsPath[l],60);

						ItemsText[l][0]   =254;
						ItemsProgram[l][0]=254;
						ItemsPath[l][0]   =254;
					}
					DATAFILE=fopen("MENU.DAT","rb+");
					fseek(DATAFILE,8,0);
					for(l=1;l<=169;l++)
						{
						fwrite(&ItemsText[l],26,1,out);
						fwrite(&ItemsProgram[l],20,1,out);
						fwrite(&ItemsPath[l],60,1,out);
					}
					fclose(DATAFILE);
					RefreshItems(PagesMenu.CurrItem);
					ItemsMenu.PutCursor();
				}
			}
			if(toascii(key)=='m' || toascii(key)=='M')
				{
				_setcursortype(_NORMALCURSOR);
				MoveWin.PutWin(20,3,60,12,0x6E);
				MoveWin.PutHeader("Move Item",0x6F,9,16);
				textcolor(0x0E);
				textbackground(6);
				gotoxy(31,6);
				cprintf("Move current ITEM to:");
				gotoxy(36,8);
				cprintf("Page - ");
				textcolor(15);
				cprintf("[  ]");
				textcolor(0x0E);
				gotoxy(36,10);
				cprintf("Item - ");
				textcolor(15);
				cprintf("[  ]");
				gotoxy(44,8);
				Blank(TextTemp,26);
				TextTemp[0]=254;
				result=Edit(TextTemp,2,0x6F,INTEGER);
				MovePage=atoi(TextTemp);
				if(result==0 && MovePage>=1 && MovePage<=13 && MoveItem>=1 && MoveItem<=13)
					{
					gotoxy(44,10);
					Blank(TextTemp,26);
					TextTemp[0]=254;
					result=Edit(TextTemp,2,0x6F,INTEGER);
					MoveItem=atoi(TextTemp);
					if(result==0 && MoveItem>=1 && MoveItem<=13 && ((MovePage-1)*13)+MoveItem!=ItemsMenu.CurrItem)
						{
						// Make sure they REALLY want to move it.
						MoveWin.DeleteWin();
						PagesMenu.EraseCursor();
						ItemsMenu.EraseCursor();
						RefreshItems(MovePage);

						// Cursors for page and item put manually
						// because of red background.
						for(y=MoveItem+7,x=ItemsMenu.cursorx;x<=ItemsMenu.maxx;x++)
							{
							ChangeAttr(x,y,0x4A);
						}
						PutChar(ItemsMenu.cursorx,MoveItem+7,175,0x4F);
						PutChar(ItemsMenu.maxx,MoveItem+7,174,0x4F);
						for(y=MovePage+7,x=PagesMenu.cursorx;x<=PagesMenu.maxx;x++)
							{
							ChangeAttr(x,y,0x4A);
						}
						PutChar(PagesMenu.cursorx,MovePage+7,175,0x4F);
						PutChar(PagesMenu.maxx,MovePage+7,174,0x4F);

						Warning.PutWin(20,1,60,7,0x4F);
						Warning.PutHeader("Warning",0x4E,7,17);
						textcolor(15);
						textbackground(4);
						gotoxy(28,5);
						cprintf("Move to this position? Y/N");

						while(!done)
							{
							if(_bios_keybrd(_KEYBRD_READY)!=0)
								{
								key=_bios_keybrd(_KEYBRD_READ);
								key=toascii(key);
								if(key=='y'||key=='Y')
									{
									pointer=((MovePage-1)*13)+MoveItem;
									for(i=0;i<26;i++)
										{
										ItemsText[pointer][i]=ItemsText[ItemsMenu.CurrItem][i];
										ItemsText[ItemsMenu.CurrItem][i]=32;
									}
									ItemsText[ItemsMenu.CurrItem][0]=254;
									for(i=0;i<20;i++)
										{
										ItemsProgram[pointer][i]=ItemsProgram[ItemsMenu.CurrItem][i];
										ItemsProgram[ItemsMenu.CurrItem][i]=32;
									}
									ItemsProgram[ItemsMenu.CurrItem][0]=254;
									for(i=0;i<60;i++)
										{
										ItemsPath[pointer][i]=ItemsPath[ItemsMenu.CurrItem][i];
										ItemsPath[ItemsMenu.CurrItem][i]=32;
									}
									ItemsPath[ItemsMenu.CurrItem][0]=254;
									DATAFILE=fopen("MENU.DAT","rb+");

									fseek(DATAFILE,8,0);
									for(l=1;l<=169;l++)
										{
										fwrite(&ItemsText[l],26,1,out);
										fwrite(&ItemsProgram[l],20,1,out);
										fwrite(&ItemsPath[l],60,1,out);

									}

									fseek(DATAFILE,PageOffs,0);
									for(l=1;l<=13;l++)
										{
										fwrite(&PagesText[l],27,1,out);
									}
									fclose(DATAFILE);
									done=1;
								}
								else if(key=='n'||key=='N')
									{
									done=1;
								}
							}
						}
						done=0;
						Warning.DeleteWin();
						// Deleting of cursor must be done manually
						// because they're in different positions.
						for(y=MoveItem+7,x=ItemsMenu.cursorx;x<=ItemsMenu.maxx;x++)
							{
							ChangeAttr(x,y,0x3F);
						}
						PutChar(ItemsMenu.cursorx+5,MoveItem+7,'-',0x3B);
						PutChar(ItemsMenu.cursorx,MoveItem+7,' ',0x3F);
						PutChar(ItemsMenu.maxx,MoveItem+7,' ',0x3F);

						for(y=MovePage+7,x=PagesMenu.cursorx;x<=PagesMenu.maxx;x++)
							{
							ChangeAttr(x,y,0x3F);
						}
						PutChar(PagesMenu.cursorx+5,MovePage+7,'-',0x3B);
						PutChar(PagesMenu.cursorx,MovePage+7,' ',0x3F);
						PutChar(PagesMenu.maxx,MovePage+7,' ',0x3F);
					}
				}
				MoveWin.DeleteWin();
				RefreshItems(PagesMenu.CurrItem);
				RefreshPages();
				ItemsMenu.PutCursor();
				PagesMenu.PutCursor();
				_setcursortype(_NOCURSOR);
			}
			if(key==12800)    // Alt-M for moving pages
				{
				MoveWin.PutWin(20,3,60,10,0x6E);
				MoveWin.PutHeader("Move Page",0x6F,9,16);
				textcolor(0x0E);
				textbackground(6);
				gotoxy(30,6);
				cprintf("Move PAGE and ITEMS to:");
				gotoxy(36,8);
				cprintf("Page - ");
				textcolor(15);
				cprintf("[  ]");
				gotoxy(wherex()-3,wherey());
				_setcursortype(_NORMALCURSOR);
				Blank(TextTemp,26);
				TextTemp[0]=254;
				result=Edit(TextTemp,2,0x6F,INTEGER);
				MovePage=atoi(TextTemp);

				if(result==0)
					{
					if(MovePage>=1 && MovePage<=13 && MovePage!=PagesMenu.CurrItem)
						{
						MoveWin.DeleteWin();
						PagesMenu.EraseCursor();
						ItemsMenu.EraseCursor();
						RefreshItems(MovePage);

						for(i=8;i<=20;i++)
							{
							for(y=i,x=ItemsMenu.cursorx;x<=ItemsMenu.maxx;x++)
								{
								ChangeAttr(x,y,0x4F);
							}
						}

						for(y=MovePage+7,x=PagesMenu.cursorx;x<=PagesMenu.maxx;x++)
							{
							ChangeAttr(x,y,0x4F);
						}
						PutChar(PagesMenu.cursorx,MovePage+7,175,0x4F);
						PutChar(PagesMenu.maxx,MovePage+7,174,0x4F);

						Warning.PutWin(18,1,61,7,0x4F);
						Warning.PutHeader("Warning",0x4E,7,17);
						textcolor(15);
						textbackground(4);
						gotoxy(22,5);
						cprintf("These ITEMS will be lost. Proceed? Y/N");

						while(!done)
							{
							if(_bios_keybrd(_KEYBRD_READY)!=0)
								{
								key=_bios_keybrd(_KEYBRD_READY);
								key=toascii(key);
								if(key=='y'||key=='Y')
									{
									for(i=1;i<=13;i++)
										{
										for(l=0;l<26;l++)
											{
											ItemsText[((MovePage-1)*13)+i][l]=ItemsText[((PagesMenu.CurrItem-1)*13)+i][l];
											ItemsText[((PagesMenu.CurrItem-1)*13)+i][l]=32;
										}
										for(l=0;l<20;l++)
											{
											ItemsProgram[((MovePage-1)*13)+i][l]=ItemsProgram[((PagesMenu.CurrItem-1)*13)+i][l];
											ItemsProgram[((PagesMenu.CurrItem-1)*13)+i][l]=32;
										}
										for(l=0;l<60;l++)
											{
											ItemsPath[((MovePage-1)*13)+i][l]=ItemsPath[((PagesMenu.CurrItem-1)*13)+i][l];
											ItemsPath[((PagesMenu.CurrItem-1)*13)+i][l]=32;
										}
									}
									for(i=0;i<27;i++)
										{
										PagesText[MovePage][i]=PagesText[PagesMenu.CurrItem][i];
									}
									memset(PagesText[PagesMenu.CurrItem],32,27);

									DATAFILE=fopen("MENU.DAT","rb+");
									fseek(DATAFILE,8,0);
									for(l=1;l<=169;l++)
										{
										fwrite(&ItemsText[l],26,1,out);
										fwrite(&ItemsProgram[l],20,1,out);
										fwrite(&ItemsPath[l],60,1,out);
									}
									fseek(DATAFILE,PageOffs,0);
									for(l=1;l<=13;l++)
										{
										fwrite(&PagesText[l],27,1,out);
									}
									fclose(DATAFILE);
									for(i=1;i<=13;i++)
										{
										ItemsText[((PagesMenu.CurrItem-1)*13)+i][0]=254;
										ItemsProgram[((PagesMenu.CurrItem-1)*13)+i][0]=254;
										ItemsPath[((PagesMenu.CurrItem-1)*13)+i][0]=254;
									}
									PagesText[PagesMenu.CurrItem][i]=254;
									done=1;
								}
								else if(key=='n'||key=='N')
									{
									done=1;
								}
								flushkeybrd();
							}
						}
						done=0;
						Warning.DeleteWin();
					}
				}
				Blank(TextTemp,26);
				MoveWin.DeleteWin();
				RefreshItems(PagesMenu.CurrItem);
				memset(&Scr[(PagesMenu.cursory*160)+(PagesMenu.cursorx*2)],32,26*2);
				RefreshPages();
				ItemsMenu.PutCursor();
				PagesMenu.PutCursor();
				_setcursortype(_NOCURSOR);
			}
			if(toascii(key)=='v' || toascii(key)=='V')
				{
				EditWin.PutWin(5,5,75,18,0x6E);
				EditWin.PutHeader("View Item",0x6F,9,31);
				textcolor(15);
				textbackground(6);
				gotoxy(8,7);
				cprintf("Description :");
				gotoxy(8,11);
				cprintf("Executable name:");
				gotoxy(8,15);
				cprintf("Path name:");
				gotoxy(10,9);
				cprintf("[                         ]");
				for(i=0;i<25;i++)
					{
					if(ItemsText[ItemsMenu.CurrItem][i]==254) goto StopPut1;
					PutChar(10+i,8,ItemsText[ItemsMenu.CurrItem][i],0x6E);
				}
				StopPut1:
				gotoxy(10,13);
				cprintf("[                   ]");
				for(i=0;i<19;i++)
					{
					if(ItemsProgram[ItemsMenu.CurrItem][i]==254) goto StopPut2;
					PutChar(10+i,12,ItemsProgram[ItemsMenu.CurrItem][i],0x6E);
				}
				StopPut2:
				gotoxy(10,17);
				cprintf("[                                                           ]");
				for(i=0;i<59;i++)
					{
					if(ItemsPath[ItemsMenu.CurrItem][i]==254) goto StopPut3;
					PutChar(10+i,16,ItemsPath[ItemsMenu.CurrItem][i],0x6E);
				}
				StopPut3:
				while(!done)
					{
					if(_bios_keybrd(_KEYBRD_READY)!=0)
						{
						_bios_keybrd(_KEYBRD_READ);
						done=1;
					}
				}
				done=0;

				EditWin.DeleteWin();
			}
			flushkeybrd();
		}    // End if - keypressed check
	}    // End do-while
	out=fopen("MENU.DAT","rb+");
	fseek(out,0,0);
	fputc(ItemsMenu.ItemCount,out);
	fputc(PagesMenu.ItemCount,out);
	fclose(out);

	CopyScreen();

	SetPage(2);

	for(i=0;i<4000;i+=2)
		{
		PutChar(i/2,0,CREDITSDATA[i],CREDITSDATA[i+1]);
		SetPage(2);
	}

	for(i=0;i<25;i++)
		{
		ScrlDown();
	}

	textcolor(15);
	textbackground(0);
	gotoxy(1,24);
	_setcursortype(_NORMALCURSOR);

	asm mov 	dx,0x3c8
	asm mov 	ax,1
	asm out 	dx,ax
	asm inc 	dx
	asm mov	al,0
	asm out	dx,al
	asm mov 	al,43
	asm out	dx,al
	asm mov 	al,0
	asm out	dx,al
}