
/* ----:==>>>> DISPLAY.H is Copyright (c) 1994 - Paul Carroll.<<<<==:---- */

/*
	This file is for use in programming graphics functions using
	the borlandc graphics library -

				GRAPHICS.H

	Needed additions -

		1) Assembler functions for checking hardware.
		2) Low level drawing functions written in assembler.

	Both additions needed to speed run time of programs.

*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <conio.h>
#include <graphics.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

class display
{
private:
	int maxx;
	int maxy;
	int graphics_ok;
	int old_col;
	int gdriver, gmode, errorcode;
	struct fillsettingstype oldfill;
	unsigned size_bitmap;
	void *bitmap;

public:
	display();
	~display();
	void check_graph(void);
	void draw_window(int x1,int y1,int x2,int y2);
	/*void radio_off(int x,int y,char text[]);
	void init_radio_on(int x,int y,char text[]);
	void radio_on(int x,int y);  */
	void write(int x,int y,char text[],int color);
	void lwrite(int x,int y,char text[],int color);
};

#endif