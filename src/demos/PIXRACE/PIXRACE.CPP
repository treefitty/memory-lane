#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>

int x,y;
int RandInc;
int key;
char count,done;
char WinPixel=0;

typedef struct {

	char col;
	int oldx;
	int x;
	int y;

} Pixel;

Pixel MyPixel[10];


void main(void)
{
	count=0;
	for(y=10;count<10;y+=20,count++)
		{
		MyPixel[count].x=5;
		MyPixel[count].y=y;
		MyPixel[count].col=count+2;
	}

	asm mov	ax,0x13
	asm int	0x10

	for(x=305,y=0;y<200;y++)
		{
		_DI=(y*320)+x;
		asm mov	ax,0xa000
		asm mov	es,ax
		asm mov	al,15
		asm mov	es:[di],al
	}

     randomize();

	done=0;
	while(!done)
		{
		delay(40);

		for(count=0;count<10;count++)
			{
			RandInc=random(4);
			MyPixel[count].oldx=MyPixel[count].x;
			MyPixel[count].x+=RandInc;

			_DI=(MyPixel[count].y*320)+MyPixel[count].oldx;
			asm mov	ax,0xa000
			asm mov	es,ax
			asm mov	es:[di],al
			asm inc	di
			asm mov	es:[di],al
			asm dec	di
			asm add	di,320
			asm mov	es:[di],al
			asm inc	di
			asm mov	es:[di],al

			_DI=(MyPixel[count].y*320)+MyPixel[count].x;
			_BL=MyPixel[count].col;
			asm mov	ax,0xa000
			asm mov	es,ax
			asm mov	es:[di],bl
			asm inc	di
			asm mov	es:[di],bl
			asm dec	di
			asm add	di,320
			asm mov	es:[di],bl
			asm inc	di
			asm mov	es:[di],bl


			if(MyPixel[count].x>=304)
				{
				done=1;
				WinPixel=MyPixel[count].col;
			}
		}
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			done=1;
		}
	}

	if(WinPixel!=0)
		{
		gotoxy(10,1);
		switch(WinPixel)
			{
			case 2: {

				printf("Green Pixel WON!");
				break;
			}
			case 3: {

				printf("Cyan Pixel WON!");
				break;
			}
			case 4:{

				printf("Red Pixel WON!");
				break;
			}
			case 5:{

				printf("Magenta Pixel WON!");
				break;
			}
			case 6:{

				printf("Brown Pixel WON!");
				break;
			}
			case 7:{

				printf("Gray Pixel WON!");
				break;
			}
			case 8:{

				printf("Dark Gray Pixel WON!");
				break;
			}
			case 9:{

				printf("Light Blue Pixel WON!");
				break;
			}
			case 10:{

				printf("Light Green Pixel WON!");
				break;
			}
			case 11:{

				printf("Light Cyan Pixel WON!");
				break;
			}
		}
		done=0;
		while(!done)
			{
			if(_bios_keybrd(_KEYBRD_READY)!=0)
				{
				_bios_keybrd(_KEYBRD_READ);
				done=1;
			}
		}
	}
	else {

		gotoxy(10,1);
          printf("Oh no, race aborted!");
		done=0;
		while(!done)
			{
			if(_bios_keybrd(_KEYBRD_READY)!=0)
				{
				_bios_keybrd(_KEYBRD_READ);
				done=1;
			}
		}
	}

	asm mov	ax,0x03
	asm int	0x10

	printf("      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n");
	printf("      ³                                 ³\n");
	printf("      ³    This Really Stupid Program   ³\n");
	printf("      ³      was done in 10 mins by     ³\n");
	printf("      ³                                 ³\n");
	printf("      ³        - Paul Carroll '95       ³\n");
	printf("      ³                                 ³\n");
	printf("      ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n\n");

}