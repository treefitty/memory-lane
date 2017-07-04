
/* ---:==>>>> DISPLAY.CPP is Copyright (c) 1994 - Paul Carroll.<<<<==:--- */

/*
	This file is for use in programming graphics functions using
	the borlandc graphics library -

				GRAPHICS.H

	Needed additions -

		1) Assembler functions for checking hardware.
		2) Low level drawing functions written in assembler.
		3) GUI Interface.
		4) Functions to read and display PCX files.

	Both additions needed to speed run time of programs.

*/

#include "display.h"

/* Constructor for class DISPLAY */

display::display()
{
	maxx= getmaxx();
	maxy= getmaxy();
	graphics_ok= TRUE;
}

/* Destructor for class DISPLAY */

display::~display()
{
	maxx= 0;
	maxy= 0;
	graphics_ok= FALSE;
}

/* Check graphics and install correct drivers */

void display::check_graph(void)
{
	detectgraph(&gdriver, &gmode);
	if(gdriver!= 9)
		{
		clrscr();
		cprintf("This program REQUIRES a high res VGA card");
		cprintf("\n\n\rPress any key to continue");
		getch();
		}
	gdriver= VGA;
	gmode= VGAHI;
	initgraph(&gdriver,&gmode,"\\borlandc\\bgi");
	errorcode= graphresult();
	if(errorcode!= grOk)
		{
		cprintf("Critical graphics error: %s\n\n\r",
				grapherrormsg(errorcode));
		cprintf("Program requires VGA or");
		cprintf(" compatible graphics adapter\n\n\r");
		cprintf("Press any key to continue...");
		getch();
		clrscr();
		exit(1);
		}
}

/* Draws 3D button in original "UP" position */



/* Draws window in specified x, y coordinates. Slight 3D effect */

void display::draw_window(int x1,int y1,int x2,int y2)
{
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(x1,y1,x2,y2);
	setcolor(BLACK);
	rectangle(x1,y1,x2,y2);
	setcolor(DARKGRAY);
	rectangle(x1+6,y1+6,x2-6,y2-6);
	setcolor(WHITE);
	line(x2-6,y1+6,x2-6,y2-6);
	line(x1+6,y2-6,x2-6,y2-6);
}

/*void display::radio_off(int x,int y,char text[])
{
	button_up(x,y,x+15,y+15,"");
	setcolor(RED);
	line(x+3,y+3,x+12,y+12);
	line(x+12,y+3,x+3,y+12);
	lwrite(x+27,y+14,"->",YELLOW);
	lwrite(x+50,y+14,text,RED);
}

void display::radio_on(int x,int y)
{
	button_down(x,y,x+15,y+15,"");
	setcolor(GREEN);
	line(x+3,y+9,x+8,y+13);
	line(x+8,y+13,x+14,y+2);
}

void display::init_radio_on(int x,int y,char text[])
{
	button_down(x,y,x+15,y+15,"");
	setcolor(GREEN);
	line(x+3,y+9,x+8,y+13);
	line(x+8,y+13,x+14,y+2);
	lwrite(x+27,y+14,"->",YELLOW);
	lwrite(x+50,y+14,text,RED);
}*/

void display::lwrite(int x,int y,char text[],int color)
{
	settextstyle(2,0,0);
	setusercharsize(2,2,3,2);
	settextjustify(0,0);
	setcolor(BLACK);
	outtextxy(x,y,text);
	setcolor(color);
	outtextxy(x,y-1,text);
}

void display::write(int x,int y,char text[],int color)
{
	settextstyle(2,0,0);
	setusercharsize(2,2,3,2);
	setcolor(BLACK);
	outtextxy(x,y,text);
	setcolor(color);
	outtextxy(x,y-1,text);
}

