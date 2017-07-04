#include <conio.h>
#include <bios.h>
#include <string.h>
#include <stdlib.h>

#include "ipcx.h"
#include "igraph.h"

int key;
char done;
char currBar=1;
int SoundDevice,Looping;
const int rt_arrow= 0x4d00;
const int left_arrow= 0x4b00;
const int up_arrow= 0x4800;
const int down_arrow= 0x5000;
const int Enter= 0x1c0d;
const int Esc=0x11b;
char myPal[768];

// Simple menu bar structure. Could be used in larger structure to create
// a menu with highlight bar.
struct MENU_BAR {

	int x,y;
	char *text[8];
	char length;
	int currItem;

} *myBar;

// Function to abort the demo if the escape key is
// pressed on the setup screen.
void AbortPGM(void)
	{
	_setcursortype(_NORMALCURSOR);
	clrscr();
	delete myBar;
	exit(0);
}

void main(void)
{
	SoundDevice=7;
	Looping=0;
	done=0;
	// Do Demo setup screen, where Sound Source can be chosen and
	// Demo Looping can be set to either ON or OFF.
	settextmode();
	// The attribute of a char can have a 3 bit nibble for it's background
	// color and 4 bit for foreground. DARKGRAY is not allowed for
	// background color selection, so BLUE is used and the palette
	// register for BLUE is modified here to ressemble a DARKGRAY color.
	char col=13;          // R,G and B are equal because it's a shade
					  // of gray.
	asm mov dx, 	0x3c8  // Set DAC register to write
	asm mov al, 	1      // to 1 or color BLUE.
	asm out dx,	al
	asm inc		dx     // Set DAC register to write.
	asm mov cx,	3      // 3 loops to set Red, Green and Blue.
	asm mov al,	col    // Color.

setBlue:

	asm out dx,	al     // Out the color to 0x3c9
	asm loop		setBlue// Decrement CX if not 0 jump back.

	_setcursortype(_NOCURSOR);
	textbackground(BLACK);
	clrscr();
	textcolor(LIGHTBLUE);
	gotoxy(5,24);
	cprintf("          - Change Configuration        - Start Demo     - Exit Demo");
	textcolor(LIGHTCYAN);
	gotoxy(5,24); cprintf("  %c %c %c %c",24,25,26,27);
	gotoxy(39,24);cprintf("Enter"); gotoxy(59,24);cprintf("Esc");
	textcolor(LIGHTBLUE);
	for(int i=8;i<14;i+=2) { gotoxy(i,24); cprintf("%c",250); }
	textcolor(DARKGRAY);
	gotoxy(1,1);
	cprintf("                       %c",218);
	cprintf("                              %c\n\r",191);
	textcolor(LIGHTBLUE);
	cprintf("                              M C S Demonstration\n\r");
	textcolor(LIGHTCYAN);
	gotoxy(32,2); cprintf("%c",250); gotoxy(34,2); cprintf("%c",250);
	textcolor(DARKGRAY);
	gotoxy(24,2);
	cprintf("%c\n\r",180);
	cprintf("                                                      %c",217);
	gotoxy(1,3); cprintf("                       %c",192);
	gotoxy(55,2); cprintf("%c",195);
	for(i=25;i<55;i++) {
		gotoxy(i,1);cprintf("%c",196);gotoxy(i,3);cprintf("%c",196);}
	for(i=1;i<24;i++)
		{gotoxy(i,2);cprintf("%c",196);gotoxy(i+55,2);cprintf("%c",196);}
	textcolor(LIGHTBLUE);
	gotoxy(31,5); cprintf("Demo Configuration.");
	textcolor(LIGHTCYAN);
	gotoxy(1,6);
	cprintf("\n\r                        Use the arrow keys to highlight");
	cprintf("\n\r                        and change the demo settings to");
	cprintf("\n\r                            best suit your machine.\n\r");
	textcolor(DARKGRAY);
	gotoxy(22,4); cprintf("%c",218);gotoxy(58,4); cprintf("%c",191);
	gotoxy(22,10);cprintf("%c",192);gotoxy(58,10);cprintf("%c",217);
	gotoxy(58,5); cprintf("%c",193);gotoxy(22,5); cprintf("%c",193);
	gotoxy(58,9); cprintf("%c",194);gotoxy(22,9); cprintf("%c",194);
	for(i=23;i<58;i++)
		{gotoxy(i,4);cprintf("%c",196);gotoxy(i,10);cprintf("%c",196);}
	textcolor(CYAN);
	for(i=31;i<50;i++) { gotoxy(i,6);cprintf("%c",196); }
	textcolor(DARKGRAY);
	gotoxy(1,2);  cprintf("%c",195); gotoxy(79,2); cprintf("%c",180);
	gotoxy(1,14);
	cprintf("                The No Sound option should be chosen by people\n\r");
	cprintf("                who are running slow 386's as the demo will run\n\r");
	cprintf("                   faster. This demo was written for 486's,\n\r");
	cprintf("                        specifically a 486DX2.");
	gotoxy(1,21);
	cprintf("                Demo Looping will continuously repeat the demo\n\r");
	cprintf("                            once it has finished.");

	// Two menu bars with different items (for left/right arrow key
	// scrolling) are defined. This is NOT the normal way the MENU_BAR
	// structure would be used as normally there would be no left/right
	// scrolling.
	myBar=new MENU_BAR[2];   // Allocate two BARS

	myBar[1].x=10;           // Define position of bar1 and
	myBar[1].y=12;           // what it is to display.
	myBar[1].text[1]="Sound Source - ";
	myBar[1].text[2]="PC Speaker";
	myBar[1].text[3]="D/A Converter On LPT1";
	myBar[1].text[4]="Sound Blaster Or Compatible";
	myBar[1].text[5]="Disney Sound Source On LPT1";
	myBar[1].text[6]="No Sound";
	myBar[1].currItem=4;     // Default setting Sound Blaster.

	myBar[2].x=10;           // Define position of bar2 and
	myBar[2].y=19;           // what it is to display.
	myBar[2].text[1]="Demo Looping - ";
	myBar[2].text[2]="On";
	myBar[2].text[3]="Off";
	myBar[2].currItem=3;     // Default setting Off.

	textbackground(BLUE);             // Display first bar and highlighter.
	gotoxy(myBar[1].x-2,myBar[1].y);
	textcolor(CYAN);
	cprintf("  %s",myBar[1].text[1]);
	textcolor(LIGHTCYAN);
	cprintf("%s",myBar[1].text[4]);
	for(i=myBar[currBar].x+strlen(myBar[1].text[(myBar[1].currItem)]);i<55;i++) cprintf(" ");
	textbackground(BLACK);

	gotoxy(myBar[2].x-2,myBar[2].y);  // Display second bar.
	textcolor(CYAN);
	cprintf("  %s",myBar[2].text[1]);
	textcolor(LIGHTCYAN);
	cprintf("%s",myBar[2].text[3]);

	while(!done)
		{
		if(_bios_keybrd(_KEYBRD_READY))    // If a key was pressed
			{
			key=_bios_keybrd(_KEYBRD_READ);   // get it.
		}
		if((currBar==1 && key==up_arrow) || (currBar==1 && key==down_arrow))
			{
			currBar=2;               // Current Bar number.
			textcolor(CYAN);                   // Erase old highlighted
			gotoxy(myBar[1].x-2,myBar[1].y);   // bar.
			cprintf("  %s",myBar[1].text[1]);
			textcolor(LIGHTCYAN);
			cprintf("%s",myBar[1].text[(myBar[1].currItem)]);
			for(i=myBar[currBar].x+strlen(myBar[1].text[(myBar[1].currItem)]);i<55;i++) cprintf(" ");

			textbackground(BLUE);              // Display new current
			gotoxy(myBar[2].x-2,myBar[2].y);   // bar with highlighter
			textcolor(CYAN);
			cprintf("  %s",myBar[2].text[1]);
			textcolor(LIGHTCYAN);
			cprintf("%s",myBar[2].text[(myBar[2].currItem)]);
			for(i=myBar[currBar].x+strlen(myBar[2].text[(myBar[2].currItem)]);i<55;i++) cprintf(" ");
			textbackground(BLACK);
			key=0;                             // Reset keypress to 0
		}
		if((currBar==2 && key==up_arrow) || (currBar==2 && key==down_arrow))
			{
			currBar=1;                         // Set Current bar number.
			textcolor(CYAN);                   // Erase old bar.
			gotoxy(myBar[2].x-2,myBar[2].y);
			cprintf("  %s",myBar[2].text[1]);
			textcolor(LIGHTCYAN);
			cprintf("%s",myBar[2].text[(myBar[2].currItem)]);
			for(i=myBar[currBar].x+strlen(myBar[2].text[(myBar[2].currItem)]);i<55;i++) cprintf(" ");

			textbackground(BLUE);              // Draw new current bar
			gotoxy(myBar[1].x-2,myBar[1].y);   // bar with highlighter
			textcolor(CYAN);
			cprintf("  %s",myBar[1].text[1]);
			textcolor(LIGHTCYAN);
			cprintf("%s",myBar[1].text[(myBar[1].currItem)]);
			for(i=myBar[currBar].x+strlen(myBar[1].text[(myBar[1].currItem)]);i<55;i++) cprintf(" ");
			textbackground(BLACK);
			key=0;                             // Reset keypress to 0
		}
		if((currBar==1 && key==left_arrow)||(currBar==1 && key==rt_arrow))
			{
			//**** Determine what the new item is for bar1.
			if(key==left_arrow)
				{
				myBar[currBar].currItem--;
				if(myBar[currBar].currItem==1) myBar[currBar].currItem=6;
			}
			else {
				myBar[currBar].currItem++;
				if(myBar[currBar].currItem==7) myBar[currBar].currItem=2;
			}
			//*******
			textbackground(BLUE);             // Display this new item
			gotoxy(myBar[1].x-2,myBar[1].y);  // with highlighter on
			textcolor(CYAN);                  // the current bar.
			cprintf("  %s",myBar[1].text[1]);
			textcolor(LIGHTCYAN);
			cprintf("%s",myBar[1].text[(myBar[1].currItem)]);
			for(i=myBar[currBar].x+strlen(myBar[1].text[(myBar[1].currItem)]);i<55;i++) cprintf(" ");
			textbackground(BLACK);
			key=0;
		}
		if((currBar==2 && key==left_arrow)||(currBar==2 && key==rt_arrow))
			{
			//***** Determine what the new iten is for bar2
			if(key==left_arrow)
				{
				myBar[currBar].currItem--;
				if(myBar[currBar].currItem==1) myBar[currBar].currItem=3;
			}
			else {
				myBar[currBar].currItem++;
				if(myBar[currBar].currItem==4) myBar[currBar].currItem=2;
			}
			//******
			textbackground(BLUE);             // Display the new item
			gotoxy(myBar[2].x-2,myBar[2].y);  // with highligher on
			textcolor(CYAN);                  // the current bar.
			cprintf("  %s",myBar[2].text[1]);
			textcolor(LIGHTCYAN);
			cprintf("%s",myBar[2].text[(myBar[2].currItem)]);
			for(i=myBar[currBar].x+strlen(myBar[2].text[(myBar[2].currItem)]);i<53;i++) cprintf(" ");
			textbackground(BLACK);
			key=0;
		}
		if(key== Enter) done=1;  // If Enter pressed exit loop, start demo.
		if(key== Esc) AbortPGM();// If Escape pressed abort back to DOS.
	}

	if(myBar[1].currItem==2) SoundDevice=0;   // PC Speaker
	if(myBar[1].currItem==3) SoundDevice=1;   // D/A Converter On LPT1
	if(myBar[1].currItem==4) SoundDevice=7;   // Sound Blaster
	if(myBar[1].currItem==5) SoundDevice=11;  // Disney Snd Scr On LPT1
	if(myBar[1].currItem==6) SoundDevice=12;  // No Sound

	if(myBar[2].currItem==2) Looping=1;       // Looping set ON
	if(myBar[2].currItem==3) Looping=0;       // Looping set OFF
	delete myBar;                             // Deallocate menu bars.
	_setcursortype(_NORMALCURSOR);
}








