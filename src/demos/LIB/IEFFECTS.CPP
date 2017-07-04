#include <stdlib.h>
#include <mem.h>
#include <conio.h>
#include <dos.h>
#include "ieffects.h"
#include "igraph.h"

void melt_on(unsigned char far *meltbuf)
{
	unsigned char far *screen= (char *)MK_FP(0x0a000,0);
	static unsigned char line[320];
	static unsigned char oldline[320];
	static int current;

	randomize();
	for(int k=0;k<=100;k++)
		{
		for(int i=0;i<320;i++)
			{
			line[i]+= random(4)+1;
			if(line[i]>=199) line[i]= 199;
			for(current=oldline[i]+1;current<=line[i];current++)
				{
				screen[(current*320)+i]=meltbuf[(current*320)+i];
			}
			oldline[i]=line[i];
		}
		delay(12);
	}
}

void melt_off(void)
{
	unsigned char far *screen= (char *)MK_FP(0x0a000,0);
	static unsigned char line[320];
	static unsigned char oldline[320];
	static int current;

	randomize();
	for(int k=0;k<=100;k++)
		{
		for(int i=0;i<320;i++)
			{
			line[i]+= random(4)+1;
			if(line[i]>=199) line[i]= 199;
			for(current=oldline[i];current<=line[i];current++)
				{
				screen[(current*320)+i]=0;
			}
			oldline[i]=line[i];
		}
		delay(12);
	}
}

void FadeIn(char *Array)
{
	char tmpPal[768];
	int diff, c;

	for(c=0;c<768;c++) tmpPal[c]=0;

	do {
		diff=0;
		setpalette(tmpPal);
		for ( c = 0; c < 768; c++ )
			{
			if ( tmpPal[c] != Array[c] )
				{
				tmpPal[c]++;
				diff++;
			}
		}

	} while(diff);
}

void FadeOff(char *Array)
{
	char tmpPal[768];
	int diff, c;

	for(c=0;c<768;c++) tmpPal[c]=Array[c];

	do {
		diff = 0;
		setpalette(tmpPal);
		for ( c = 0; c < 768; c++ )
			{
			if ( tmpPal[c] )
				{
				tmpPal[c]--;
				diff++;
			}
		}

	}while (diff);
}

void FadeDown(char *Array)
{
	char tmpPal[768];
	int 	diff,c;

	for(c=0;c<768;c++) tmpPal[c]=63;

	do {
		diff=0;
		setpalette(tmpPal);
		for(c=0;c<768;c++)
			{
			if(tmpPal[c]>Array[c])
				{
				tmpPal[c]--;
				diff++;
			}
		}
		delay(16);

	} while(diff);
}