
/* ----:==>>>> DISPLAY.H is Copyright (c) 1994 - Paul Carroll.<<<<==:---- */

/*
	This file has functions for low level control of the mouse.

*/

#ifndef PMOUSE_H
#define PMOUSE_H

#include <dos.h>
#include <conio.h>
#include <stdlib.h>

#define MOUSE 0x33
#define TRUE 1

class mousecall
{
private:
	union REGS regs;
	int mouseinstalled(void);

public:
	void showmouse(void);
	void hidemouse(void);
	void setpos(int x,int y);
	int mousexpos(void);
	int mouseypos(void);
	void hlimit(int left,int right);
	void vlimit(int top,int bottom);
	int mousepress(void);
	int checkmouse(void);
	int mmoved(void);
	void textmouse_cursor(void);
	void watch_cursor(void);
     void man_cursor(void);
	void h_glass_cursor(void);
	void hand_cursor(void);
};

#endif