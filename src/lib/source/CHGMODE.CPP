#include <conio.h>

void copyright(void);

void copyright(void)
{
	clrscr();
	textcolor(LIGHTBLUE);
	gotoxy(15,5);
	cprintf("Programmed in Borland C++ 3.1");
	gotoxy(15,6);
	cprintf("By Paul Carroll - Copyright 1994");
}

void main()
{
	int ans, mode;

	clrscr();
	_setcursortype(_NOCURSOR);
	textcolor(YELLOW);

	START:
	gotoxy(15,5);
	cprintf("Change current video mode to:");
	gotoxy(15,7);
	textcolor(MAGENTA);
	cprintf("Color, 40 columns          - 1");
	gotoxy(15,8);
	cprintf("Black and White 80 columns - 2");
	gotoxy(15,9);
	cprintf("Color 80 columns           - 3");
	gotoxy(15,10);
	cprintf("EGA 43-line or VGA 50-line - 4");
	ans=getch();
	if(ans== 49)
		{
		copyright();
		textcolor(YELLOW);
		gotoxy(15,3);
		cprintf("Curret Text Mode is now :");
		gotoxy(15,5); cprintf("Color, 40 columns.");
		textmode(C40);
		}
	else if(ans== 50)
		{
		copyright();
		textcolor(YELLOW);
		gotoxy(15,3);
		cprintf("Curret Text Mode is now :");
		gotoxy(15,5); cprintf("Balck and White 80 columns.");
		textmode(BW80);
		}
	else if(ans== 51)
		{
		copyright();
		textcolor(YELLOW);
		gotoxy(15,3);
		cprintf("Curret Text Mode is now :");
		gotoxy(15,5); cprintf("Color 80 columns");
		textmode(C80);
		}
	else if(ans== 52)
		{
		copyright();
		textcolor(YELLOW);
		gotoxy(15,3);
		cprintf("Curret Text Mode is now :");
		gotoxy(15,5); cprintf("EGA 43-line or VGA 50-line - 4");
		textmode(C4350);
		}
	else goto START;
}