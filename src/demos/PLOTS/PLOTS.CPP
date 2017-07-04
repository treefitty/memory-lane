#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <ctype.h>
#include <bios.h>
#include <string.h>

#define FALSE 0
#define TRUE 1
#define PRIVATE 13
#define NATURE 27
#define MEDIAN 41
#define CENTRE 55

char fill_50[]= {85,170,85,170,85,170,85,170};
char fill_100[]= {255,255,255,255,255,255,255,255};
const int rt_arrow= 0x4d00;
const int left_arrow= 0x4b00;
const int up_arrow= 0x4800;
const int down_arrow= 0x5000;
const int F1_key= 0x3b00;
const int F2_key= 0x3c00;
const int Enter= 0x1c0d;
const int Backspace= 0xe08;
const int TAB= 0xf09;
const int space= 0x3920;
const int insert= 0x5200;
int x, y;
int key, site_num;
int s, d;
FILE *stream;

void init_graphics(void);
void init_intro(void);
void quit_pgm(void);
void init_screen(void);
void init_cursor(void);
void cursor_move(void);
void locat_prn(void);
void ed_notes(void);
void type_rec(void);
void beep(int s,int d);

/*struct tree_info {
     fpos_t position;
     int site_id;
     int cur_up, cur_down;
     char notes_str[56];
     char site_type;
     void read();
     void write();
     void open_files();
} tree_obj;

void tree_info:: open_files()
{
	stream= fopen("DBPRIV.DBS", "r+t");
	if (stream== NULL)
	     {
	     clrscr();
	     cprintf("Error when reading data file : DBNOTES.DBS\n");
	     cprintf("File is needed to run program.\n");
	     cprintf("\nPress any key to continue...");
	     while(!kbhit());
	     quit_pgm();
	     }
}

void tree_info:: read()
{
	if(cur_up== TRUE)
	     {
	     fseek(stream, 0, SEEK_SET);
	     fsetpos(stream, &position- (strlen(notes_str)-1));
	     gotoxy(14,41);
	     fgets(notes_str, 56, stream);
	     cputs(notes_str);
	     fsetpos(stream, &position- (strlen(notes_str)-1));
	     }
	if(cur_down== TRUE)
	     {
	     fgets(notes_str, 56, stream);
	     cputs(notes_str);
	     fgetpos(stream, &position);
	     }
}
      */
void init_graphics(void)
{
	int gdriver= DETECT, gmode, errorcode;

	initgraph(&gdriver, &gmode,"\\borlandc\\bgi");
	errorcode= graphresult();
	if (errorcode!= grOk)
	     {
	     printf("Critical graphics error : %s",
				grapherrormsg(errorcode));
	     printf("Press any key to halt program...");
	     while(!kbhit());
	     exit(1);
	     }
}

void init_intro(void)
{
	int x, y;

	x= getmaxx(); y= getmaxy();
	settextstyle(6,0,2);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(GREEN);
	outtextxy(x/2,20,"PLOTS was written and developed by Paul Carroll.");
	outtextxy(x/2,45,"Copyright 1994");
	setfillpattern(fill_100, MAGENTA);bar(0,65,x,85);bar(0,65,20,y);
	bar(x-20,65,x,y);bar(0,y-20,x,y);
	delay(1500);
	cleardevice();
	closegraph();
	textmode(LASTMODE);
}

void quit_pgm(void)
{
	clrscr();
	fclose(stream);
	normvideo();
	_setcursortype(_NORMALCURSOR);
	textmode(C80);
	exit(0);
}

void init_screen(void)
{
	int i;

	textmode(C4350);
	textcolor(GREEN);
	gotoxy(15,1);
	cprintf("PLOTS was written and developed by Paul Carroll.");
	gotoxy(33,3);
	cprintf("Copyright 1994.");
	textcolor(RED);
	y= 5;
	for (x= 6; x<77; x++)  { gotoxy(x,y); cprintf("%c",205); } y= 40;
	for (x= 6; x<77; x++)  { gotoxy(x,y); cprintf("%c",205); } x= 5;
	for (y= 6; y<40; y++)  { gotoxy(x,y); cprintf("%c",186); } x= 77;
	for (y= 6; y<40; y++)  { gotoxy(x,y); cprintf("%c",186); } y= 45;
	for (x= 6; x<77; x++)  { gotoxy(x,y); cprintf("%c",205); } x= 5;
	for (y= 41; y<45; y++) { gotoxy(x,y); cprintf("%c",186); } x= 77;
	for (y= 41; y<45; y++) { gotoxy(x,y); cprintf("%c",186); } y= 8;
	for (x= 6; x<77; x++)  { gotoxy(x,y); cprintf("%c",205); } x=10;
	for (y= 9; y<40; y++)  { gotoxy(x,y); cprintf("%c",186); }
	x= 6; y= 9; textcolor(YELLOW);
	for (site_num= 1; site_num<=31; site_num++)
	     { gotoxy(x,y); cprintf("%d",site_num); y++; }
	textcolor(RED);
	gotoxy(5,8);   cprintf("%c",204); gotoxy(77,8);  cprintf("%c",185);
	gotoxy(10,40); cprintf("%c",202); gotoxy(10,8);  cprintf("%c",203);
	gotoxy(5, 5);  cprintf("%c",201); gotoxy(77,5);  cprintf("%c",187);
	gotoxy(5,40);  cprintf("%c",204); gotoxy(77,40); cprintf("%c",185);
	gotoxy(5,45);  cprintf("%c",200); gotoxy(77,45); cprintf("%c",188);
	gotoxy(11,8);  cprintf("%c",181); gotoxy(20,8);  cprintf("%c",198);
	gotoxy(26,8);  cprintf("%c",181); gotoxy(33,8);  cprintf("%c",198);
	gotoxy(40,8);  cprintf("%c",181); gotoxy(47,8);  cprintf("%c",198);
	gotoxy(54,8);  cprintf("%c",181); gotoxy(61,8);  cprintf("%c",198);
	gotoxy(8,45);  cprintf("%c",181); gotoxy(27,45); cprintf("%c",198);
	gotoxy(30,45); cprintf("%c",181); gotoxy(49,45); cprintf("%c",198);
	gotoxy(51,45); cprintf("%c",181); gotoxy(72,45); cprintf("%c",198);
	textcolor(LIGHTMAGENTA);
	gotoxy(27,8);  cprintf("Nature"); gotoxy(41,8); cprintf("Median");
	gotoxy(55,8);  cprintf("Centre"); gotoxy(12,8);  cprintf("Private ");
	gotoxy(9,45);  cprintf("Evergreen - ");
	gotoxy(31,45); cprintf("Deciduous - ");
	gotoxy(52,45); cprintf("Future Site - ");
	textcolor(WHITE);
	gotoxy(6,41); cprintf("Notes :");
	gotoxy(6,6);  cprintf("Site Number :");
	gotoxy(30,6); cprintf("Location :");
	gotoxy(53,6); cprintf("Type :");
	textcolor(EGA_LIGHTRED);
	gotoxy(2,47);  cprintf("<F1-Exit>");
	gotoxy(12,47); cprintf("<F2-Help>");
	gotoxy(2,49);  cprintf("<TAB-Edit Notes>");
	gotoxy(19,49); cprintf("<%c %c %c %c Move Cursor>",24,25,27,26);
	gotoxy(22,47); cprintf("<V-Vacant Site>");
	gotoxy(38,47); cprintf("<E-Evergreen> <D-Deciduous>");
	cprintf(" <F-Future Site>");
	textcolor(EGA_LIGHTGREEN);
	gotoxy(21,45);
	for(i= x; i<=(x+5); i++) cprintf("%c", 177);
	textcolor(EGA_YELLOW);
	gotoxy(43,45);
	for(i= x; i<=(x+5); i++) cprintf("%c", 177);
	textcolor(EGA_WHITE);
	gotoxy(66,45);
	for(i= x; i<=(x+5); i++) cprintf("%c", 177);
	_setcursortype(_NOCURSOR);
}
void init_cursor(void)
{
	int i;

	textcolor(CYAN+BLINK);
	gotoxy(x, y);
	for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	site_num= 1;
	textcolor(YELLOW);
	gotoxy(41,6); cprintf("PRIVATE");
	gotoxy(20,6); cprintf("%i",site_num);
	gotoxy(14,41);
	//fgets(tree_obj.notes_str, 56, stream);
	//cputs(tree_obj.notes_str);
	gotoxy(x,y);
}

void cursor_move(void)
{
	int i;

	gotoxy(14,41);
	for(i= 0;i<=56;i++) printf(" ");
	if (key== rt_arrow)
	     {
	     textcolor(BLACK);
	     gotoxy(x, y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     textcolor(CYAN+BLINK);
	     x+= 14; gotoxy(x,y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     gotoxy(x,y);
	     }
	if (key== left_arrow)
	     {
	     textcolor(BLACK);
	     gotoxy(x,y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     textcolor(CYAN+BLINK);
	     x-= 14; gotoxy(x,y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     gotoxy(x,y);
	     }
	if (key== up_arrow)
	     {
	     textcolor(BLACK);
	     gotoxy(x,y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     textcolor(CYAN+BLINK);
	     y--; gotoxy(x,y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     site_num--;
	     textcolor(YELLOW);
	     gotoxy(20,6); cprintf("%i ",site_num);
	     //tree_obj.cur_up= TRUE;
	     //tree_obj.read();
	     //tree_obj.cur_up= FALSE;
	     gotoxy(x,y);
	     }
	if(key== down_arrow)
	     {
	     textcolor(BLACK);
	     gotoxy(x,y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     textcolor(CYAN+BLINK);
	     y++; gotoxy(x,y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     site_num++;
	     textcolor(YELLOW);
	     gotoxy(20,6); cprintf("%i",site_num);
	     gotoxy(14,41);
	     //tree_obj.cur_down= TRUE;
	     //tree_obj.read();
	     //tree_obj.cur_down= FALSE;
	     gotoxy(x,y);
	     }
}

void locat_prn(void)
{
	int xtemp;

	xtemp= wherex();
	textcolor(YELLOW);
	if (xtemp== PRIVATE) { gotoxy(41,6); cprintf("PRIVATE"); }
	if (xtemp== NATURE)  { gotoxy(41,6); cprintf("NATURE "); }
	if (xtemp== MEDIAN)  { gotoxy(41,6); cprintf("MEDIAN "); }
	if (xtemp== CENTRE)  { gotoxy(41,6); cprintf("CENTRE "); }
	gotoxy(x,y);
}

void ed_notes(void)
{
	int key, read_to, i;
	int xtemp, ytemp;

	textcolor(BLACK);
	for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	_setcursortype(_NORMALCURSOR);
	x= wherex(); y= wherey();
	gotoxy(14,41); textcolor(YELLOW);

	LOOP:
	for(;;)
	     {
	     while(_bios_keybrd(_KEYBRD_READY)== 0);
	     key= _bios_keybrd(_KEYBRD_READ);
	     if (isalnum(key & 0xFF))
		  {
		  xtemp= wherex();
		  if (xtemp== 70) goto LOOP;
		  cprintf("%c",key);
		  }
	     if (key== TAB) goto EXIT;
	     if (key== Backspace)
		  {
		  read_to= 70;
		  xtemp= wherex(); ytemp= wherey();
		  if (xtemp== 14) goto LOOP;
		  gotoxy(xtemp-1, ytemp);
		  movetext(xtemp, ytemp, read_to, ytemp, xtemp-1, ytemp);
		  }
	     if (key== left_arrow)
		  {
		  xtemp= wherex(); ytemp= wherey();
		  if (xtemp-1== 13) goto LOOP;
		  gotoxy(xtemp-1, ytemp);
		  }
	     if (key== rt_arrow)
		  {
		  xtemp= wherex(); ytemp= wherey();
		  if (xtemp+1== 71) goto LOOP;
		  gotoxy(xtemp+1, ytemp);
		  }
	     if (key== insert) goto INSERT_LOOP;
	     key= toascii(key);
	     if ((key<127)&&(key>122)) cprintf("%c",key);
	     if ((key<97) &&(key>90))  cprintf("%c",key);
	     if ((key<65) &&(key>57))  cprintf("%c",key);
	     if ((key<48) &&(key>30))  cprintf("%c",key);
	     }
	INSERT_LOOP:
	     _setcursortype(_SOLIDCURSOR);
	     for(;;)
		  {
		  while(_bios_keybrd(_KEYBRD_READY)== 0);
		  key= _bios_keybrd(_KEYBRD_READ);
		  xtemp= wherex(); ytemp= wherey();
		  if (isalnum(key & 0xFF))
		       {
		       if (xtemp== 70) goto INSERT_LOOP;
		       movetext(xtemp, ytemp, 69, ytemp,xtemp+1,ytemp);
		       gotoxy(xtemp,ytemp);
		       cprintf("%c",key);
		       }
		  if (key== Backspace)
		       {
		       read_to= 70;
		       if (xtemp== 14) goto INSERT_LOOP;
		       gotoxy(70,ytemp); cprintf(" "); gotoxy(xtemp,ytemp);
		       gotoxy(xtemp-1, ytemp);
		       movetext(xtemp, ytemp, read_to, ytemp, xtemp-1, ytemp);
		       }
		  if (key== TAB) goto EXIT;
		  if (key== left_arrow)
		       {
		       if (xtemp-1== 14) goto INSERT_LOOP;
		       gotoxy(xtemp-1, ytemp);
		       }
		  if (key== rt_arrow)
		       {
		       if (xtemp+1== 71) goto INSERT_LOOP;
		       gotoxy(xtemp+1, ytemp);
		       }
		  if (key== insert)
		       {
		       _setcursortype(_NORMALCURSOR);
		       goto LOOP;
		       }
		  key= toascii(key);
		  if ((key<127)&&(key>122)) cprintf("%c",key);
		  if ((key<97) &&(key>90))  cprintf("%c",key);
		  if ((key<65) &&(key>57))  cprintf("%c",key);
		  if ((key<48) &&(key>30))  cprintf("%c",key);
		  }
	EXIT:
	     _setcursortype(_NOCURSOR);
	     textcolor(CYAN+BLINK); gotoxy(x-6,y);
	     for(i= x; i<=(x+5); i++) cprintf("%c",fill_100);
	     gotoxy(x-6,y);
	     x= wherex(); y= wherey();
}

void type_rec(void)
{
	int i;

	if((key== 69)|| (key== 101))
	     {
	     gotoxy(x+7,y); textcolor(EGA_LIGHTGREEN);
	     for(i= x; i<=(x+4); i++) cprintf("%c",177);
	     gotoxy(60,6); textcolor(YELLOW);
	     cprintf("Evergreen   ");
	     }
	if((key== 68)|| (key== 100))
	     {
	     gotoxy(x+7,y); textcolor(EGA_YELLOW);
	     for(i= x; i<=(x+4); i++) cprintf("%c",177);
	     gotoxy(60,6); textcolor(YELLOW);
	     cprintf("Deciduous   ");
	     }
	if((key== 70)|| (key== 102))
	     {
	     gotoxy(x+7,y); textcolor(EGA_WHITE);
	     for(i= x; i<=(x+4); i++) cprintf("%c",177);
	     gotoxy(60,6); textcolor(YELLOW);
	     cprintf("Future Site");
	     }
	if((key== 86)|| (key== 118))
	     {
	     gotoxy(x+7,y); textcolor(BLACK);
	     for(i= x; i<=(x+4); i++) cprintf(" ");
	     gotoxy(60,6); cprintf("           ");
	     }
	gotoxy(x,y);
}

void beep(int s,int d)
{
	sound(s);
	delay(d);
	nosound();
}

int main(void)
{
	int i, count;

	clrscr();
	//tree_obj.open_files();
	//init_graphics();
	//init_intro();
	init_screen();
	x= 13; y=9;
	init_cursor();
	goto MAIN_LOOP;

	MAIN_LOOP:
	for (;;)
	     {
	     while(_bios_keybrd(_KEYBRD_READY)== 0);
	     key= _bios_keybrd(_KEYBRD_READ);
	     switch(key)
		  {
		  case rt_arrow: { x= wherex();
				   if ((x+14)>55) goto MAIN_LOOP;
				   cursor_move();
				   locat_prn();
				   goto MAIN_LOOP;
				 }
		  case left_arrow: { x= wherex();
				     if ((x-14)<10) goto MAIN_LOOP;
				     cursor_move();
				     locat_prn();
				     goto MAIN_LOOP;
				   }
		  case up_arrow:   { y= wherey();
				     if ((y-1)<9) goto MAIN_LOOP;
				     cursor_move();
				     goto MAIN_LOOP;
				   }
		  case down_arrow: { y= wherey();
				     if ((y+1)>39) goto MAIN_LOOP;
				     cursor_move();
				     goto MAIN_LOOP;
				   }
		  case F1_key  :    quit_pgm();
		  case TAB     :  { ed_notes(); goto MAIN_LOOP; }
		  }
	     key= toascii(key);
	     if((key== 69)|| (key== 101)|| (key== 68)|| (key== 100)||
		  (key== 70)||(key== 102)||(key==86)||(key==118)) type_rec();
	     else
		 { s= 180; d= 30; beep(s,d); }
	     }
}











