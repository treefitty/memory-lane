
/* ---:==>>>> DISPLAY.CPP is Copyright (c) 1994 - Paul Carroll.<<<<==:--- */


/*	This file has functions for low level control of the mouse.  */

#include <imouse.h>

/* Show the default mouse cursor */
void showmouse(void)
{
	asm mov 	ax,0x01
	asm int   0x33
}

/* Hide the mouse cursor */
void hidemouse(void)
{
	asm mov 	ax,0x02
	asm int	0x33
}

/* Set the mouse's current position */
void setpos(int x,int y)
{
	asm mov 	ax,4
	asm mov 	cx,x
	asm mov 	dx,y
	asm int 		0x33
}

/* Get current x coordinate of mouse cursor */
int mousexpos(void)
{
	asm mov 	ax,0x03
	asm int	0x33
	asm shr	cx,1
//	asm sub	cx,1

	return _CX;
}

/* Get current y coordinate of mouse cursor */
int mouseypos(void)
{
	asm mov 	ax,0x03
	asm int 	0x33
//	asm sub	dx,1

	return _DX;
}

/* Horizontal boundary for mouse */
void hlimit(int left,int right)
{
	asm mov 	ax,0x07
	asm mov 	cx,left
	asm shl	cx,1
	asm mov 	dx,right
	asm shl	dx,1
	asm int	0x33
}

/* Vertical boundary for mouse */
void vlimit(int top,int bottom)
{
	asm mov 	ax,8
	asm mov 	cx,top
	asm mov 	dx,bottom
	asm int 	0x33
}

/* Check if mouse button has been pressed */
int mousepress(void)
{
	asm mov 	ax,0x03
	asm int 	0x33

	return _BX;
}

/* Check the mouse is installed and handle error if one is encountered */
void checkmouse(void)
{
	asm mov 	ax,0x00
	asm int	0x33

	if(_AX== 0)
		{
		asm mov	ax,0x03
		asm int	0x10
		textcolor(WHITE);
		cprintf("Unable to locate mouse.\n");
		cprintf("This program requires a mouse to run.");
		cprintf("\n\n\rPress any key to continue...");
		getch();
		clrscr();
		exit(1);
	}
	showmouse();
}

/* Check if mouse has moved */
int mmoved(void)
{
	asm mov	ax,11;
	asm int	0x33
	if(_CX==0 && _DX==0)
		return 0;
	else
		return 1;
}

