#include <igraph.h>

#include "global.h"

int loadPCX(char far *filename,char far *filebuffer,int Pal)
{
	FILE *fptr;
	char header[128];
	unsigned char c, runlen;
	int i, r;
	long position=0;
	int drawing;
	char palette[768];

	if((fptr = fopen(filename,"rb"))==0) return 2;       // Open the file
	r = fread( &header, 1, 128, fptr );     // Read the header information.
	if(r!= 128) return 3;
	fseek(fptr,-768L,SEEK_END);
	for(i=0;i<256;i++)
		for (int j=0; j<3; j++)
			{
			palette[i*3+j]=fgetc(fptr);
			palette[i*3+j]/=4;
		}
	if(Pal==1) setpalette(palette);
	fseek(fptr,128L,SEEK_SET);   // Seek position just past header.
	drawing= YES;
	while(drawing)
		{
		c=(unsigned char)fgetc(fptr);
		if ((c & 0xc0) == 0xc0)  // Check for a run.
			{
			runlen = c & 0x3f; 	// Strip off high bits to get run length
			c=fgetc(fptr);  	// Get colour of run.
		}
		else runlen= 1;  // if not a run, set the run length to 1.
		do
			{
			filebuffer[position]=c;
			position++;
			if(position== 64000) drawing= NO;

		} while(drawing&& --runlen> 0);
	}
	fclose(fptr);
	return 0;
}

void loadPCXpal(char far *filename,char *palette)
{
	FILE *fptr;

	fptr = fopen(filename,"rb");       // Open the file
	if(fptr==NULL)                         // In case of error.
		{
		printf("\nError - Couldn't open PCX file");
		return;
	}
	fseek(fptr,-768L,SEEK_END);
	for(int i=0;i<256;i++)
		for (int j=0; j<3; j++)
			{
			palette[i*3+j]=fgetc(fptr);
			palette[i*3+j]/=4;
		}
}

