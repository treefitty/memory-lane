#include <dos.h>
#include <stdio.h>

#include "intro1.h"
#include "intro2.h"
#include "intro3.h"
#include "intro4.h"
#include "intro5.h"

int i;
FILE *myFile;

void main(void)
{
	myFile=fopen("PART1.DAT","wb+");

	for(i=0;i<42*190;i++)
		{
		fputc(Intro1[i],myFile);
	}

	for(i=0;i<152*160;i++)
		{
		fputc(Intro2[i],myFile);
	}

	for(i=0;i<56*190;i++)
		{
		fputc(Intro3[i],myFile);
	}

	for(i=0;i<124*230;i++)
		{
		fputc(Intro4[i],myFile);
	}

	for(i=0;i<124*194;i++)
		{
		fputc(Intro5[i],myFile);
	}

     fclose(myFile);
}
