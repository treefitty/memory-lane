#include "general.h"

void checkgraph(void)
{
	int gdriver= VGA, gmode= VGAHI, errorcode;

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
void quit_pgm(void)
{
	delete scr;
	closegraph();
	pmouse.hidemouse();
	clrscr();
	exit(0);
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
void zline(int x1,int y1,int x2,int y2,unsigned char col,unsigned char page)
{
	__asm {
			push ax;
			push cx;
			push dx;
			push bx;
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
	__asm {
			pop	bx;
			pop	dx;
			pop	cx;
			pop	ax;
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