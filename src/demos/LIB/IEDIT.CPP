/*

  First ver of function : (about) 1-4-1995

  Purpose : Line editing function with insert, del, backspace etc.

	- Text MUST be an array of w+1. w specifies how many characters
	  are to be in the string, 1 extra is needed to keep track of the
	  end.
	- Character 254 (˛) is used as an end os string marker.
	- Attr is the attribute used for printing.


  NOTE! :

	- Assumes cursor is already in position.
	- Passing an array of size less than w+1 WILL cause bizzare things
	  to happen! (Probably a crash).
	- Following data types REQUIRE an array of size :

			-> STRING  = w+1    (Specified by programmer)
			-> DATE    = 8      ƒø
							 √ƒƒ Don't require 254 (˛) char
			-> TIME    = 5      ƒŸ
			-> INTEGER = w+1    (Specified by programmer)

	- TIME and DATE do NOT check to see if the input was valid.
*/

#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>
#include <mem.h>

#include <iedit.h>
#include <ikeybrd.h>
#include <text.h>

#define Insert      0x5200
#define Del         0x5300
#define Enter       0x1c0d
#define F1_key      0x3b00
#define home        0x4700
#define end	     0x4f00
#define page_down   0x5100
#define page_up     0x4900
#define enter       0x1c0d
#define backspace   0xe08
#define right_arrow 0x4d00
#define left_arrow  0x4b00
#define up_arrow    0x4800
#define down_arrow  0x5000
#define escape      0x11b

#define PutArray()	while(!done)			\
					{				\
					if(text[q]!=254)	\
						{			\
						q++;			\
						pointer++;	\
					}				\
					else done=1;		\
				}					\
				done=0;				\

void PutBox(void)
{
	int x;

	textattr(0x04);
	gotoxy(54,24);
	cprintf("€€€€€€€€€€€€€€€€€€€€€€€");
	for(x=55;x<79;x++)
		{
		ChangeAttr(x,24,0x08);
	}
}
void EraseBox(void)
{
	int x;

	textattr(0x70);
	gotoxy(54,24);
	cprintf("∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞");
	for(x=55;x<79;x++)
		{
		ChangeAttr(x,24,0x1F);
	}
}

int Edit(char *text,int w,char attr,char type)
{
	int pointer,xkey;
	int startx,x,temp,tempx,tempy,endx,q,k;;
	char insert, done;
	char TempArray[2];

	startx=wherex();
	pointer=0;
	done=0;
	insert=1;

	q=0;
	if(type==STRING || INTEGER)
		{
		PutArray();
	}
	endx=pointer;
	q=0;
	if(type==DATE)
		{
		if(text[0]!=254)
			{
			PutArray();
		}
		else {
			cprintf("  /  /  ");
			gotoxy(wherex()-8,wherey());
		}
	}
	if(type==TIME)
		{
		if(text[0]!=254)
			{
			PutArray();
		}
		else {
			cprintf("  :  ");
			gotoxy(wherex()-5,wherey());
		}
	}

	done=0;
	pointer=0;
	while(!done)
		{
		flushkeybrd();
		while(_bios_keybrd(_KEYBRD_READY)!=0);
		xkey=_bios_keybrd(_KEYBRD_READ);
		flushkeybrd();
		switch(xkey)
			{
			case enter:
				{
				switch(type)
					{
					case DATE :
						{
						TempArray[0]=text[0];
						TempArray[1]=text[1];
						temp=atoi(TempArray);

						done=1;
						break;
					}
					case TIME :
						{
						TempArray[0]=text[3];
						TempArray[1]=text[4];
						if(atoi(TempArray)<60)
							{
							done=1;
						}
						else {
							tempx=wherex();
							tempy=wherey();
							_setcursortype(_NOCURSOR);
							PutBox();
							textattr(0xCF);
							gotoxy(58,24);
							cprintf("Invalid Time!");
							sound(150);
							delay(300);
							nosound();
							getch();
							EraseBox();
							if(insert==0) _setcursortype(_SOLIDCURSOR);
							else _setcursortype(_NORMALCURSOR);
							gotoxy(tempx,tempy);
							textattr(attr);
						}

						break;
					}
					default :
						{
						done=1;
						break;
					}
				}
				break;
			}
			case backspace:
				{
				if(type==DATE || type==TIME)
					{
					if((pointer) % 3!=0)
						{
						gotoxy(wherex()-1,wherey());
						putch(' ');
						gotoxy(wherex()-1,wherey());
						pointer--;
					}
					else {
						gotoxy(wherex()-2,wherey());
						putch(' ');
						gotoxy(wherex()-1,wherey());
						pointer-=2;
					}
					break;
				}
				if(pointer>endx)
					{
					pointer--;
					gotoxy(wherex()-1,wherey());

					break;
				}
				if(pointer>0)
					{
					if(type==STRING || type==INTEGER)
						{
						endx--;
						_setcursortype(_NOCURSOR);
						memcpy(&text[pointer-1],&text[pointer],w-pointer);

						if(pointer==w) text[w-1]=254;
						text[w]=32;
						pointer--;
						tempx=wherex();
						gotoxy(startx+endx,wherey());
						textattr(attr);
						putch(' ');
						gotoxy(tempx-1,wherey());
						temp=pointer;
						tempx=wherex();
						x=startx+pointer;
						while(!done)
							{
							if(text[temp]==254 || x>=(startx+w)) done=1;
							else {
								gotoxy(x,wherey());
								putch(text[temp]);
								x++;
								temp++;
							}
						}
						done=0;

						gotoxy(tempx,wherey());
						if(insert==0) _setcursortype(_SOLIDCURSOR);
						else _setcursortype(_NORMALCURSOR);
					}
				}
				break;
			}
			case Del:
				{
				if(pointer>endx) break;
				if(pointer<w && text[pointer]!=254 && (type==STRING || type==INTEGER))
					{
					endx--;
					_setcursortype(_NOCURSOR);
					if(pointer==w-1) text[pointer]=32;
					else {
						memcpy(&text[pointer],&text[pointer+1],w-pointer);
					}
					if(text[w-1]!=32) text[w-1]=32;
					tempx=wherex();
					gotoxy(startx+endx,wherey());
					textattr(attr);
					putch(' ');
					gotoxy(tempx,wherey());
					temp=pointer;
					tempx=wherex();
					while(!done)
						{
						if(text[temp]==254) done=1;
						else {
							putch(text[temp]);
							temp++;
						}
					}
					done=0;

					gotoxy(tempx,wherey());
					if(insert==0)_setcursortype(_SOLIDCURSOR);
					else _setcursortype(_NORMALCURSOR);
					text[w-1]=254;
				}
				break;
			}
			case left_arrow:
				{
				if(pointer>0)
					{
					if(type==STRING || type==INTEGER)
						{
						gotoxy(wherex()-1,wherey());
						pointer--;
					}
					else {
						if((pointer) % 3!=0)
							{
							gotoxy(wherex()-1,wherey());
							pointer--;
						}
						else {
							gotoxy(wherex()-2,wherey());
							pointer-=2;
						}
					}
				}
				break;
			}
			case right_arrow:
				{
				if(pointer<w)
					{
					if(type==STRING || type==INTEGER)
						{
						gotoxy(wherex()+1,wherey());
						pointer++;
					}
					else {
						if((pointer-1) % 3!=0 || pointer==w-1)
							{
							gotoxy(wherex()+1,wherey());
							pointer++;
						}
						else {
							if(pointer<7)
								{
								gotoxy(wherex()+2,wherey());
								pointer+=2;
							}
						}
					}
				}
				break;
			}
			case Insert:
				{
				if(insert==0)
					{
					insert=1;
					_setcursortype(_NORMALCURSOR);
				}
				else {
					insert=0;
					_setcursortype(_SOLIDCURSOR);
				}
				break;
			}
			case escape:
				{
				return(1);
			}
			case home:
				{
				pointer=0;
				gotoxy(startx,wherey());
				break;
			}
			case end:
				{
				pointer=endx;
				gotoxy(startx+endx,wherey());
				break;
			}
			default:
				{
				xkey=toascii(xkey);
				if(type==STRING)
					{
					if(xkey<32 || xkey>125) break;
				}
				else {
					if(xkey<48 || xkey>57) break;
				}
				if(pointer<w)
					{
					if(type==STRING || type==INTEGER)
						{
						if(insert==0)
							{
							_setcursortype(_SOLIDCURSOR);
							putch(xkey);
							text[pointer]=xkey;
							pointer++;
							if(pointer>endx)
								{
								if(text[endx]==254) text[endx]=32;
								endx=pointer;
								text[pointer]=254;
							}
							else {
								text[endx]=254;
							}
						}
						else {
							_setcursortype(_NOCURSOR);
							for(q=w-1,k=w-2;q>pointer;q--,k--)
								{
								text[q]=text[k];
							}
							putch(xkey);
							text[pointer]=xkey;
							pointer++;

							text[w+1]=254;
							if(pointer>=endx)
								{
								if(text[endx]==254) text[endx]=32;
								endx=pointer;
								text[pointer]=254;
							}
							else {
								if(endx<w) endx++;
								text[endx]=254;
							}
							x=startx+pointer;
							tempx=wherex();
							temp=pointer;
							while(!done)
								{
								if(text[temp]==254) done=1;
								else {
									gotoxy(x,wherey());
									putch(text[temp]);
									x++;
									temp++;
								}
							}
							done=0;

							gotoxy(tempx,wherey());
							_setcursortype(_NORMALCURSOR);
						}
					}
					if(pointer<w && (type==DATE || type==TIME))
						{
						putch(xkey);
						text[pointer]=xkey;

						if((pointer-1) % 3!=0 || pointer==w-1)
							{
							pointer++;
						}
						else {
							if(pointer<w-1)
								{
								gotoxy(wherex()+1,wherey());
								pointer+=2;
							}
						}
					}
				}
			}
		}
	}
	if(insert==1) _setcursortype(_NORMALCURSOR);
	done=0;
	pointer=0;
	for(pointer=0;pointer<w;pointer++)
		{
		if(text[pointer]==254)
			{
			if(type==DATE) text[pointer]='0';
			pointer++;
			for(;pointer<w;pointer++)
				{
				if(type==DATE) text[pointer]='0';
				else text[pointer]=0;
			}
		}
	}
	if(type==DATE)
		{
		text[2]='/';
		text[5]='/';
	}
	if(type==TIME) text[2]=':';
	END:

	return(0);
}
