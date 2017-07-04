
/* ---:==>>>> DISPLAY.CPP is Copyright (c) 1994 - Paul Carroll.<<<<==:--- */


/*	This file has functions for low level control of the mouse.  */

#include "pmouse.h"

/* Check if mouse is present */
int mousecall::mouseinstalled(void)
{
	int result;

	__asm {
		mov ax,	0x00;
		int		0x33;
		mov result,ax;
	}
	return result;
}

/* Show the default mouse cursor */
void mousecall::showmouse(void)
{
	__asm {
		mov ax, 	0x01;
		int       0x33;
	}
}

/* Hide the mouse cursor */
void mousecall::hidemouse(void)
{
	__asm {
		mov ax,	0x02;
		int		0x33;
	}
}

/* Set the mouse's current position */
void mousecall::setpos(int x,int y)
{
	__asm {
		mov ax,	4;
		mov cx,	x;
		mov dx,	y;
		int 		0x33;
	}
}

/* Get current x coordinate of mouse cursor */
int mousecall::mousexpos(void)
{
	int result;

	__asm {
		mov ax,	0x03;
		int		0x33;
		mov result,cx;
	}
	return result;
}

/* Get current y coordinate of mouse cursor */
int mousecall::mouseypos(void)
{
	int result;

	__asm {
		mov ax,	0x03;
		int 		0x33;
		mov result,dx;
	}
	return result;
}

/* Horizontal boundary for mouse */
void mousecall::hlimit(int left,int right)
{
	__asm {
		mov ax,	0x07;
		mov cx,	left;
		mov dx,	right;
		int		0x33;
	}
}

/* Vertical boundary for mouse */
void mousecall::vlimit(int top,int bottom)
{
	__asm {
		mov ax,	8;
		mov cx,	top;
		mov dx,	bottom;
		int 		0x33;
	}
}

/* Check if mouse button has been pressed */
int mousecall::mousepress(void)
{
	int result;

	__asm {
		mov ax,	0x03;
		int 		0x33;
		mov result,bx;
	}
	return result;
}

/* Check the mouse is installed and handle error if one is encountered */
int mousecall::checkmouse(void)
{
	if(mousecall::mouseinstalled()!= 0) return 0;
	else {
		cprintf("Unable to locate mouse.\n");
		cprintf("This program requires a mouse to run.");
		cprintf("\n\n\rPress any key to continue...");
		getch();
		clrscr();
		exit(1);
		}
	return 0;
}

/* Check if mouse has moved */
int mousecall::mmoved(void)
{
   union REGS regs;

   regs.x.ax = 11;
   int86(MOUSE,&regs,&regs);
   if ( regs.x.cx == 0 && regs.x.dx == 0 )
	 return 0;
   else
	 return 1;
}

void mousecall::hand_cursor(void)
{
  union REGS ireg, oreg;
  struct SREGS segregs;

  static int hand_curs[32] =  /* Screen Mask */
			 { 	 0xf3ff, 0xe1ff, 0xe1ff, 0xe1ff,
				 0xe1ff, 0xf07f, 0xc00f, 0x8001,
				 0x0000, 0x0000, 0x0000, 0x0000,
				 0x0000, 0x8000, 0xc001, 0xe001,
				 /* Cursor Mask */
				 0x0000, 0x0c00, 0x0c00, 0x0c00,
				 0x0c00, 0x0c00, 0x0d80, 0x2db0,
				 0x6db6, 0x6db6, 0x6db6, 0x7ffe,
				 0x7ffe, 0x3ffe, 0x1ffc, 0x0000       };

  /* Hide the cursor while updating it... */
  ireg.x.ax = 2;
  int86(0x33,&ireg,&oreg);
  /* Use the new cursor shape and attributes... */
  ireg.x.ax = 9;
  ireg.x.bx = 4;  /* X position hotspot */
  ireg.x.cx = 0;  /* Y position hotspot */
  ireg.x.dx = (int)hand_curs;
  segread(&segregs);
  segregs.es = segregs.ds;
  int86x(0x33,&ireg,&oreg,&segregs);
  /* Now show the new cursor pointer shape... */
  ireg.x.ax = 1;
  int86(0x33,&ireg,&oreg);
}

void mousecall::h_glass_cursor(void)
{
  union REGS ireg, oreg;
  struct SREGS segregs;

  static int h_glass_curs[32] =  /* Screen Mask */
			 { 	 0x0000, 0x0000, 0x0000, 0x8001,
				 0x8001, 0x8001, 0x8001, 0x8001,
				 0x8001, 0x8001, 0x8001, 0x8001,
				 0x8001, 0x0000, 0x0000, 0x0000,
				 /* Cursor Mask */
				 0xffff, 0x8001, 0xd7eb, 0x57ea,
				 0x542a, 0x5a5a, 0x5dba, 0x5e7a,
				 0x5e7a, 0x5dba, 0x5bda, 0x566a,
				 0x542a, 0xd00b, 0x8001, 0xffff       };

  /* Hide the cursor while updating it... */
  ireg.x.ax = 2;
  int86(0x33,&ireg,&oreg);
  /* Use the new cursor shape and attributes... */
  ireg.x.ax = 9;
  ireg.x.bx = -1;  /* X position hotspot */
  ireg.x.cx = -1;  /* Y position hotspot */
  ireg.x.dx = (int)h_glass_curs;
  segread(&segregs);
  segregs.es = segregs.ds;
  int86x(0x33,&ireg,&oreg,&segregs);
  /* Now show the new cursor pointer shape... */
  ireg.x.ax = 1;
  int86(0x33,&ireg,&oreg);
}

void mousecall::man_cursor(void)
{
  union REGS ireg, oreg;
  struct SREGS segregs;

  static int man_curs[32] =  /* Screen Mask */
			 { 	 0xf83f, 0xb01f, 0xb01f, 0x383f,
				 0xbc7f, 0xdcff, 0xdcff, 0xe84f,
				 0xf4bf, 0xfcff, 0xfcff, 0xf87f,
				 0xf33f, 0xf7bf, 0x97a7, 0x0783,
				 /* Cursor Mask */
				 0x0000, 0x0380, 0x0540, 0x0380,
				 0x0000, 0x0000, 0x0000, 0x0000,
				 0x0000, 0x0000, 0x0000, 0x0000,
				 0x0000, 0x0000, 0x0000, 0x4010       };

  /* Hide the cursor while updating it... */
  ireg.x.ax = 2;
  int86(0x33,&ireg,&oreg);
  /* Use the new cursor shape and attributes... */
  ireg.x.ax = 9;
  ireg.x.bx = 1;  /* X position hotspot */
  ireg.x.cx = 3;  /* Y position hotspot */
  ireg.x.dx = (int)man_curs;
  segread(&segregs);
  segregs.es = segregs.ds;
  int86x(0x33,&ireg,&oreg,&segregs);
  /* Now show the new cursor pointer shape... */
  ireg.x.ax = 1;
  int86(0x33,&ireg,&oreg);
}

void mousecall::watch_cursor(void)
{
  union REGS ireg, oreg;
  struct SREGS segregs;

  static int watch_curs[32] =  /* Screen Mask */
			 { 	 0x3fff, 0x1fff, 0x0fff, 0x07ff,
				 0x03ff, 0x0000, 0x0000, 0x0000,
				 0x000c, 0x010f, 0x010f, 0x100f,
				 0x300f, 0xf80f, 0xf807, 0xfe07,
				 /* Cursor Mask */
				 0x0000, 0x4000, 0x6000, 0x7000,
				 0x7800, 0x7000, 0x77fe, 0x7462,
				 0x7060, 0x7c60, 0x6c60, 0x4660,
				 0x0660, 0x0360, 0x00f0, 0x0000       };

  /* Hide the cursor while updating it... */
  ireg.x.ax = 2;
  int86(0x33,&ireg,&oreg);
  /* Use the new cursor shape and attributes... */
  ireg.x.ax = 9;
  ireg.x.bx = 6;  /* X position hotspot */
  ireg.x.cx = 0;  /* Y position hotspot */
  ireg.x.dx = (int)watch_curs;
  segread(&segregs);
  segregs.es = segregs.ds;
  int86x(0x33,&ireg,&oreg,&segregs);
  /* Now show the new cursor pointer shape... */
  ireg.x.ax = 1;
  int86(0x33,&ireg,&oreg);
}