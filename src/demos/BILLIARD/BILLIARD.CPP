/*
   This is a copyright version of Virtual Billiards. The source code,
   game or any other part of this program may not be altered or
   distributed in any way, shape or form including shareware.This program
   was written in Borland C++ version 3.1. Several other header files
   were used that have been created by the programmer.
   These include :

	- PMOUSE.H  - File containing all mouse related routines.
	- DISPLAY.H - File containing all graphics related functions not
			 already defined in the Borland GRAPHICS.H file.

   This program is Copyright (C) 1994 and was programmed by Paul Carroll.
*/

#include <display.h>
#include <pmouse.h>
#include <bios.h>
#include <stdio.h>
#include <conio.h>

#define ABORTED			0
#define PRESSED			1

#define TRUE                  1
#define FALSE                 0
#define CARPET_LEFT          15
#define CARPET_RIGHT        309
#define CARPET_TOP           15
#define CARPET_BOTTOM 	   339
#define CARPET_WIDTH	   294
#define CARPET_LENGTH       324
#define CARPET_X_MIDDLE 	   162
#define CARPET_Y_MIDDLE     177
#define CUSH_WIDTH            5

int i,j,k, maxx,maxy,currx, curry,mbutton;
int redx, redy, rhit, lhit, white_on, oldx, oldy;
int newx, newy,power;
int whitex, whitey;


void mainmenu(void);
void drawtable(void);
void init_screen(void);
void game_loop(void);
void shot(void);
void zline(int x1,int y1,int x2,int y2,unsigned char col,unsigned char page);
void drawball(int x,int y,unsigned char color);
void checkgraph(void);
void quit_pgm(void);

display *scr;
mousecall pmouse;

class BUTTON : mousecall{

private:

	int x1, y1;
	int x2, y2;
	int currx;
	int curry;
	int button_status;
	int button_up_down;
	char text[30];
	void button_up(void);
	void button_down(void);

public:

	BUTTON(int left,int top,int right,int bottom,char ztext[]);
	int poll_mouse(void);
};

BUTTON::BUTTON(int left,int top,int right,int bottom,char ztext[])
{
	int i;

	x1=left; y1=top;
	x2=right; y2=bottom;
	for(i=0;i<=30;i++) text[i]= ztext[i];
	button_up();
}

void BUTTON::button_up(void)
{
	settextstyle(2,0,0);
	setusercharsize(2,2,2,2);
	settextjustify(CENTER_TEXT,1);
	setfillstyle(1,WHITE);
	setcolor(WHITE);
	bar(x1,y1,x2,y2);
	setcolor(BLACK);
	rectangle(x1,y1,x2,y2);
	setcolor(DARKGRAY);
	line(x1+1,y2-1,x2-1,y2-1);
	line(x1+2,y2-2,x2-2,y2-2);
	line(x2-1,y1+1,x2-1,y2-1);
	line(x2-2,y1+2,x2-2,y2-2);
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(x1+3,y1+3,x2-3,y2-3);
	setcolor(BLACK);
	outtextxy(((x2-x1)/2)+x1,((y2-y1)/2)+y1-1,text);
}

void BUTTON::button_down(void)
{
	settextstyle(2,0,0);
	setusercharsize(2,2,2,2);
	settextjustify(CENTER_TEXT,1);
	setfillstyle(1,DARKGRAY);
	setcolor(DARKGRAY);
	bar(x1,y1,x2,y2);
	setcolor(BLACK);
	rectangle(x1,y1,x2,y2);
	setfillstyle(1,LIGHTGRAY);
	setcolor(LIGHTGRAY);
	bar(x1+3,y1+3,x2-1,y2-1);
	setcolor(BLACK);
	outtextxy(((x2-x1)/2)+x1+1,((y2-y1)/2)+y1,text);
}

int  BUTTON::poll_mouse(void)
{
	currx= mousexpos();
	curry= mouseypos();
	if((currx>x1)&&(currx<x2)&&(curry>y1)&&(curry<y2)&&(mousepress()))
		{
		hidemouse();
		button_down();
		showmouse();
		do
			{
			if((currx<x1)||(currx>x2)||(curry<y1)||(curry>y2))
				{
				hidemouse();
				button_up();
				showmouse();
				setpos(currx,curry);
				return(ABORTED);
				}
			currx= mousexpos();
			curry= mouseypos();
			}
		while(mousepress()== TRUE);
		hidemouse();
		button_up();
		showmouse();
		setpos(currx,curry);
		return(PRESSED);
		}
	setpos(currx,curry);
	return(0);
}

void mainmenu(void)
{
	int result;

	pmouse.hidemouse();
	setviewport(0,0,639,349,1);
	setfillstyle(7, EGA_BLUE);
	bar(0, 0, maxx, maxy);
	scr->draw_window(200, 30, maxx- 200, 120);    /* Main Menu            */
	scr->draw_window(200, 140, maxx- 200, 330);   /* Menu selection panel */
	setfillstyle(1, EGA_LIGHTGRAY);
	/* Init Button objects */
	BUTTON prac_but((maxx/2)-45,160,(maxx/2)+45,180,"Practice Shots");

	BUTTON comp_but((maxx/2)-45, 190,(maxx/2)+45, 210,"Play Computer");

	BUTTON human_but((maxx/2)-45, 220,(maxx/2)+45, 240,"Play Human");

	BUTTON leave_but((maxx/2)-45,290,(maxx/2)+45,310,"Leave Game");

	setcolor(EGA_BROWN);
	settextjustify(1,1);
	settextstyle(3,0,5);
	outtextxy(maxx/2,70,"Main Menu");
	settextstyle(3,0,4);
	pmouse.showmouse();

	for(;;)
		{
		if(prac_but.poll_mouse()== PRESSED) game_loop();

		if(comp_but.poll_mouse()== PRESSED);

		if(human_but.poll_mouse()== PRESSED);

		if(leave_but.poll_mouse()== PRESSED) quit_pgm();
		}
}

void drawtable(void)
{
	pmouse.hidemouse();
	clearviewport();              /* Clear viewport and draw carpet */
	setcolor(GREEN);
	setfillstyle(1,GREEN);
	bar(15,15,309,339);

	setcolor(BLACK);              /* Draw spots on table */
	line(CARPET_LEFT+6, 275, CARPET_RIGHT-6, 275);
	arc(CARPET_X_MIDDLE, 275, 180, 360, 40);
	settextstyle(2,2,2);
	outtextxy(CARPET_X_MIDDLE, CARPET_Y_MIDDLE- 45,"+");
	outtextxy(CARPET_X_MIDDLE, CARPET_Y_MIDDLE- 87,"+");
	outtextxy(CARPET_X_MIDDLE, CARPET_Y_MIDDLE- 130,"+");
	outtextxy(CARPET_X_MIDDLE- 50, CARPET_Y_MIDDLE- 130,"+");
	outtextxy(CARPET_X_MIDDLE+ 50, CARPET_Y_MIDDLE- 130,"+");

	/* Draw Window and buttons. */
	scr->draw_window(maxx-320,maxy-270,maxx-30,maxy-90);



	setcolor(BLACK);
	setfillstyle(1,BLACK);
	bar(maxx-300,maxy-200,maxx-50,maxy-190);
	setcolor(EGA_DARKGRAY);
	for(i=maxx-300,k=0;i<=maxx-50;i+=25,k++)
		{
		//itoa(k,number,6);
		//outtextxy(i,maxy-212,number);
		line(i,maxy-205,i,maxy-200);
		}

	

	settextstyle(8,0,4);
	setcolor(RED);
	outtextxy(maxx-175,maxy-245,"Power Setting.");
	pmouse.showmouse();
}

void game_loop(void)
{
	int radius, circumference;
	char done;

	drawtable();
	BUTTON exit_to_menu(maxx-300,maxy-120,maxx-50,maxy-100,"Exit To Main Menu.");
	BUTTON decrease(maxx-290,maxy-170,maxx-180,maxy-150,"<< Decrease");
	BUTTON increase(maxx-170,maxy-170,maxx-60,maxy-150,"Increase >>");
	redx= CARPET_X_MIDDLE+2; redy= CARPET_Y_MIDDLE- 130;
	drawball(redx,redy,EGA_RED);
	pmouse.setpos(CARPET_X_MIDDLE, 300);
	setviewport(15,15,309,339,1);

	done=0;
	while(!done)
		{
		mbutton= pmouse.mousepress();
		currx= pmouse.mousexpos();
		curry= pmouse.mouseypos();
		if(currx<310)
			{
			if((white_on== TRUE)&&(pmouse.mmoved()))
				{
				setwritemode(XOR_PUT);
				setcolor(LIGHTGRAY);
				pmouse.hidemouse();
				line(whitex,whitey,oldx,oldy);
				line(whitex,whitey,currx,curry);
				oldx= currx; oldy= curry;
				setwritemode(COPY_PUT);
				pmouse.showmouse();
				}
			else	if(white_on==FALSE)
				{
				drawball(currx+15,curry+15,WHITE);
				whitex= currx;
				whitey= curry;
				pmouse.hidemouse();
				line(whitex,whitey,currx,curry);
				while(pmouse.mousepress()!=0);
				pmouse.showmouse();
				oldx= currx; oldy= curry;
				white_on=TRUE;
				break;
				}
		}
		else {
			setviewport(0,0,639,349,1);
			if(exit_to_menu.poll_mouse())
				{
				setviewport(0,0,639,349,1);
				power=0;
				white_on= FALSE;
				cleardevice();
				mainmenu();
			}
			else if(((currx>=(maxx-310))&&(curry>=(maxy-192))
				&&(currx<=(maxx-200))&&(curry<=(maxy-172)))
				||(mbutton== 2))
				{
				setviewport(0,0,639,349,1);
				if(power<1) { setviewport(15,15,309,339,1); break; }
				power--;
				setfillstyle(1,BLACK);
				setcolor(BLACK);
				bar(maxx-300+(power*25),maxy-200,
				  maxx-300+((power+1)*25),maxy-190);
				setviewport(15,15,309,339,1);
				}
			else if((currx>=(maxx-190))&&(curry>=(maxy-192))
				&&(currx<=(maxx-80))&&(curry<=(maxy-172)))
				{
				setviewport(0,0,639,349,1);
				if(power>9) { setviewport(15,15,309,339,1); break; }
				power++;
				setfillstyle(1,10);
				setcolor(10);
				bar(maxx-325+(power*25),maxy-200,
				  maxx-325+((power+1)*25),maxy-190);
				setviewport(15,15,309,339,1);
				}
			else if(currx<(maxx-260))
				{
				oldx= currx; oldy= curry;
				shot();
				}
			else if(white_on==TRUE) break;

			setviewport(15,15,309,339,1);
		}
		if(_bios_keybrd(_KEYBRD_READY)!= 0)
			{
			_bios_keybrd(_KEYBRD_READ);
			done=1;
		}
	}
}

void shot(void)
{
	int mousex, mousey;
	int lhit, rhit, thit, bhit;

}

void zline(int x1,int y1,int x2,int y2,unsigned char col,unsigned char page)
{
	__asm {
			mov  cx, x1;
			mov	dx, y1;
			mov  bh, page;
			cmp  dx, y2;
			je	NEXT_X_DOT;
			jmp  NEXT_Y_DOT;
		}
	NEXT_X_DOT:
	__asm {
			mov 	al, col;
			mov 	ah, 0x0c;
			int 	0x10;
			inc 	cx;
			cmp  cx, x2;
			jg	END;
			jmp  NEXT_X_DOT;
		}
	NEXT_Y_DOT:
	__asm {
			mov 	al, col;
			mov 	ah, 0x0c;
			int 	0x10;
			inc 	dx;
			cmp	dx, y2;
			jg	END;
			jmp	NEXT_Y_DOT;
	}
	END:
}

void drawball(int x,int y,unsigned char color)
{
	pmouse.hidemouse();
	zline(x-1,y-3,x+1,y-3,color,0);
	zline(x-2,y-2,x+2,y-2,color,0);
	zline(x-3,y-1,x-2,y-1,color,0);  /* left of reflect spot */

	zline(x-1,y-1,x-1,y-1,WHITE,0);  /* reflection spot */

	zline(x,y-1,x+3,y-1,color,0);   /* right of refl spot */
	zline(x-3,y,x+3,y,color,0);
	zline(x-3,y+1,x+3,y+1,color,0);
	zline(x-2,y+2,x+2,y+2,color,0);
	zline(x-1,y+3,x+1,y+3,color,0);
							  /* the shadow - darkgray */
	zline(x+4,y,x+4,y+3,DARKGRAY,0);
	zline(x+3,y+2,x+3,y+4,DARKGRAY,0);
	zline(x+2,y+3,x+2,y+4,DARKGRAY,0);
	zline(x,y+4,x+1,y+4,DARKGRAY,0);
	pmouse.showmouse();
}

void quit_pgm(void)
{
	delete scr;
	closegraph();
	pmouse.hidemouse();
	clrscr();
	exit(0);
}

void checkgraph(void)
{
	int gdriver= VGA, gmode= VGAMED, errorcode;

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

void main(void)
{
	checkgraph();
	pmouse.checkmouse();
	maxx= 640;
	maxy= 350;
	pmouse.vlimit(0, maxy-15);
	pmouse.hlimit(0, maxx-10);
	pmouse.showmouse();
	mainmenu();
}