#include <stdio.h>
#include <dos.h>

#include <ipcx.h>
#include <igraph.h>

#define YES 1
#define NO  0

int loadPCX(char far *filename,char far *filebuffer,int Pal)
{
	// MUST Be a 64K filebuffer.
	// ~~~~

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

void PutRawPCX(char far *RawPCX)
{
	unsigned char c, runlen;
	long PCXposition=128;
	long ScrPosition=0;
	int drawing;
	char far *TPCXScr=(char *)MK_FP(0xa000,0);

	drawing= YES;
	while(drawing)
		{
		c=(unsigned char)RawPCX[PCXposition];
		PCXposition++;
		if ((c & 0xc0) == 0xc0)  // Check for a run.
			{
			runlen = c & 0x3f; 	// Strip off high bits to get run length

			c=(unsigned char)RawPCX[PCXposition];
			PCXposition++;
		}
		else runlen= 1;  // if not a run, set the run length to 1.
		do
			{
			TPCXScr[ScrPosition]=c;
			ScrPosition++;

			if(ScrPosition== 64000) drawing= NO;

		} while(drawing && --runlen>0);
	}
}

void GetRawPCXPal(char *Array,char far *RawPCXBuf,int size)
{
	long RawPCXPointer;

	//fptr = fopen(filename,"rb");       // Open the file
	//if(fptr==NULL)                         // In case of error.
	//	{
	//	printf("\nError - Couldn't open PCX file");
	//	return;
	//}
	//fseek(fptr,-768L,SEEK_END);

	RawPCXPointer=size-768;
	for(int i=0;i<256;i++)
		for (int j=0; j<3; j++)
			{
//			palette[i*3+j]=fgetc(fptr);
			Array[(i*3)+j]=RawPCXBuf[RawPCXPointer];
			RawPCXPointer++;
			Array[(i*3)+j]/=4;
		}
}

