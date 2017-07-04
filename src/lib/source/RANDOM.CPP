#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

int count, rnd_1, rnd_2, rnd_3;
int rnd_file;
FILE *stream;

void main(void)
{
	clrscr();
	textcolor(LIGHTRED);
	cprintf("Random Number Generator - Paul Carroll 1994.");
	gotoxy(1,3);
	_setcursortype(_NOCURSOR);
	randomize();
	stream= fopen("RANDOM.TXT","w+");
	fseek(stream,0,0);
	for(count=1;count<=120;count++)
		{
		rnd_1= random(30);
		rnd_2= random(30);
		rnd_3= random(30);
		gotoxy(1,3);
		textcolor(WHITE);
		cprintf("Generating Set Number:- %i\n",count);
		delay(15);
		fprintf(stream,"Set %i:-   %i,   %i,   %i\n",count,rnd_1
						,rnd_2,rnd_3);
		}
	fclose(stream);
	gotoxy(1,5);
	textcolor(LIGHTRED);
	cprintf("FINISHED....");
	delay(100);
}