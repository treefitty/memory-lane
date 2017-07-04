#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <io.h>
#include <process.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dos.h>

#include <bwsb.h>

#include <ipcx.h>

#define ChOfs 32

extern int DoMenu(void);

typedef struct
{ int SoundCard;
  int BaseIO;
  int IRQ;
  int DMA;
  int SoundQuality;
} MSEConfigFile;

char far *Scr=(char *)MK_FP(0xa000,0);
unsigned char TempPal[768];
unsigned char MyPal[768];
unsigned char SkullPal[768];
unsigned char VertPal[768];
char far *VirtScr;
char far *SkullPic;
char far *TransFormBuf;
char far *TempBuf;
char far *VBallIntro;
char far *LandIntro;
char *Intro1;
char *Intro2;
char *Intro3;
char *Intro4;
char *Intro5;
int x,y,i,z;
char done;
float radians;
const float PI = 3.1415927;
int COS[360], SIN[360];
int xa,ya,za;
int PointRoll=0,PointPitch=0,PointYaw=0;
int PostXadd,PostYadd,PreZadd;
int objz=0,Loops;
int LinearMulTab[200];
char Volume;
int *ScrPts;
int *WorldPts;
char far *Bitmap;
FILE *RawPCX;
FILE *myFile;

int module;               // handle of GDM file
char modfile1[80]="DEMINTR.GDM";
char modfile2[80]="PARTB1.GDM";         // ASCIIZ filename of file to load
char modfile3[80]="COMPLICA.GDM";
char *msefile[] = { "GUS.MSE",
				"SB1X.MSE",
				"SB2X.MSE",
				"SBPRO.MSE",
				"SB16.MSE",
				"PAS.MSE"  };
GDMHeader modhead;
MSEConfigFile MSEConfig; // Configuration for MSE
char OverRate;
int BufferSize;
int ErrCode, j;
unsigned char channels;

struct Pixel {

	int y;               // Y position on screen.
	int angle;           // Current reference angle of SIN curve.
	int bounced;         // If bounced==1 moving right else moving left.
	int yact;            // Y value of a_SIN(angle)
	float delimeter;     // Delimiting factor to reduce max height reached.
	float x;             // X position on screen.
	float startdelimeter;// Used when starting from max position.
	float XIncFactor;    // Left/Right movement factor.
	float OrigInc;       // Original Left/Right movement factor;
	char starting;       // Is the pixel just starting to fall????
	char status;         // Status==1 when pixel is still on, or 0
					 // when it has finished "bouncing"
	char bgCol;

} *myPixel;

FILE *FontFile;
FILE *PalFile;

class FONT {

public:

	char ASCIIPos,Defined;
	char Width,Height;
	char far *Dat;
	int temp;

	void Init(char w,char h,char Pos);
	void DeInit(void);

} *MyFonts;

void FONT::Init(char w,char h,char Pos)
{
	Width=w;
	Height=h;
	ASCIIPos=Pos;
	Defined=1;

	if((Dat=(char far*)malloc(w*h))==0)
		{
		asm mov	ax,0x03
		asm int	0x10
		printf("Font memory allocation error.");
		asm mov	ax,0x4c00
		asm int	0x21
	}
	for(temp=0;temp<w*h;temp++)
		{
		Dat[temp]=0;
	}
}

void FONT::DeInit(void)
{
	if(Defined==1)
		{
		free(Dat);
	}
}

void LoadFont(char *FileName)
{
	char ChPos;
	char w,h;
	int count;
	char SpaceDone=0;

	extern int _fmode;
	_fmode=O_BINARY;
	if((FontFile=fopen(FileName,"r"))==NULL)
		{
		asm mov	ax,0x03
		asm int	0x10
		printf("Error loading font file!");
		asm mov	ax,0x4c00
		asm int	0x21
	}
	fseek(FontFile,20,0);
	for(i=' '-ChOfs;i<=']'-ChOfs;i++)
		{
		if(feof(FontFile)) goto END;
		ChPos=fgetc(FontFile);
		w=fgetc(FontFile);
		h=fgetc(FontFile);
		if(SpaceDone==0)
			{
			MyFonts[0].Init(w,h,32);
			for(count=0;count<w*h;count++)
				{
				MyFonts[0].Dat[count]=0;
			}
			SpaceDone=1;
		}

		MyFonts[ChPos-ChOfs].Init(w,h,ChPos);

		for(count=0;count<w*h;count++)
			{
			MyFonts[ChPos-ChOfs].Dat[count]=fgetc(FontFile);
		}
	}
	END:

	fclose(FontFile);
}

void LoadPalFile(char *FileName)
{
	int ch;

	if((PalFile=fopen(FileName,"rb+"))==NULL)
		{
		asm mov	ax,0x03
		asm int	0x10
		printf("Error loading PAL file!");
		asm mov 	ax,0x4c
		asm int	0x21
	}
	for(ch=0;ch<768;ch++)
		{
		TempPal[ch]=fgetc(PalFile);;
	}
	fclose(PalFile);
}

void Part1(void);
void Part2(void);
void Part3(void);
void Part4(void);
void Part5(void);
void Part6(void);
void Part7(void);
void Part8(void);

void LoadMusic(char *File)
{
	if ((module = open("MSE.CFG", O_RDONLY | O_BINARY)) == -1)
		{
		MSEError:
		printf("No Sound selected in SETUP. Please run SETUP.\n");
		exit(0);
	}
	read(module, &MSEConfig, 10);


	if (MSEConfig.SoundCard==0) goto MSEError;

	MSEConfig.SoundCard--;


	BufferSize = 4096;
	OverRate = 45;
/*

	MSEConfig.SoundCard=2;
	MSEConfig.IRQ=3;
	MSEConfig.DMA=1;
	MSEConfig.BaseIO=2;
	MSEConfig.SoundQuality=3;
*/
	ErrCode=LoadMSE(msefile[MSEConfig.SoundCard],
			   0,                             // File offset
			   OverRate,
			   BufferSize,
			   &MSEConfig.BaseIO,
			   &MSEConfig.IRQ,
			   &MSEConfig.DMA);
	if (ErrCode)
		{
		asm mov 	ax,0x03
		asm int	0x10

		switch(ErrCode)
			{
			case 1: printf("Base I/O address autodetection failure\n");
				break;
			case 2: printf("IRQ level autodetection failure\n");
				break;
			case 3: printf("DMA channel autodetection failure\n");
				break;
			case 4: printf("DMA channel not supported\n");
				break;
			case 6: printf("Sound device does not respond\n");
				break;
			case 7: printf("Memory control blocks destroyed\n");
				break;
			case 8: printf("Insufficient memory for mixing buffers\n");
				break;
			case 9: printf("Insufficient memory for MSE file\n");
				break;
			case 10: printf("MSE has invalid identification string (corrupt/non-existant)\n");
				break;
			case 11: printf("MSE disk read failure\n");
				break;
			case 12: printf("MVSOUND.SYS not loaded (required for PAS use)\n");
				break;
			default: printf("Unknown error on MSE startup %u\n", ErrCode);
		}
		exit(0);
	}

	if ((module=open(File, O_RDONLY | O_BINARY)) == -1)
		{
		printf("Can't find file %s\n", File);
		exit(0);
	}

	ErrCode = EmsExist() & 1;             //Enable EMS use if EMS services found
	LoadGDM(module, 0, &ErrCode, &modhead);  //Load our GDM
	close(module);

	if (ErrCode != 0)
		{
		asm mov	ax,0x03
		asm int	0x10

		switch(ErrCode)
			{
			case 1: printf("Module is corrupt\n");
				break;
			case 2: printf("Could not autodetect module type\n");
				break;
			case 3: printf("Bad format ID\n");
				break;
			case 4: printf("Out of memory\n");
				break;
			case 5: printf("Cannot unpack samples\n");
				break;
			case 6: printf("AdLib samples not supported\n");
				break;
			default: printf("Unknown load error: %u\n", ErrCode);
		}
		exit(0);
	}

	channels = 0;
	//Scan and count number of used music channels
	for (j = 0; j < 32; j++)
		{
		if (modhead.PanMap[j] != 0xFF)
		channels++;                     //increment channels if in use
	}
}

void MemError(void)
{
	asm mov	ax,0x03
	asm int	0x10

	textcolor(15);
	gotoxy(0,0);
	printf("Memory allocation error: Need more memory!");
	gotoxy(0,3);
	exit(0);
}

void main(void)
{
	if(DoMenu()==1)
		{
		clrscr();
		printf("Abnormal termination!");
		asm mov	ax,0x4c00
          asm int	0x21
	}

	for(i=0;i<200;i++)
		{
		LinearMulTab[i]=i*320;
	}
	for(i=0;i<360;i++)
		{
		radians= (float)(i * PI) / 180;
		COS[i] = (int)(cos(radians) * 256);
		SIN[i] = (int)(sin(radians) * 256);
	}
	if((VirtScr  =(char *)malloc(sizeof(char)*64000))==0)MemError();
	if((TransFormBuf=(char *)malloc(sizeof(char)*6000 ))==0)MemError();
	if((TempBuf=     (char *)malloc(sizeof(char)*6000 ))==0)MemError();
	if((VBallIntro=  (char *)malloc(sizeof(char)*16384))==0)MemError();
	if((LandIntro=   (char *)malloc(sizeof(char)*18245))==0)MemError();
	if((Intro1=      (char *)malloc(sizeof(char)*7980 ))==0)MemError();
	if((Intro2=      (char *)malloc(sizeof(char)*24320))==0)MemError();
	if((Intro3=      (char *)malloc(sizeof(char)*10640))==0)MemError();
	if((Intro4=      (char *)malloc(sizeof(char)*28520))==0)MemError();
	if((Intro5=      (char *)malloc(sizeof(char)*24056))==0)MemError();

///*
	loadPCX("VERTIGO.PCX",VirtScr,0);
	loadPCXpal("VERTIGO.PCX",VertPal);

	if((RawPCX=fopen("VECTORB.PCX","rb+"))==0) exit(1);
	fseek(RawPCX,0,0);
	for(i=0;i<16384;i++)
		{
		VBallIntro[i]=fgetc(RawPCX);
		if(feof(RawPCX))
			{
			printf("Data file Error: VECTORB.PCX");
			exit(0);
		}
	}
	fclose(RawPCX);

	if((RawPCX=fopen("LANDSCAP.PCX","rb+"))==0) exit(1);
	fseek(RawPCX,0,0);
	for(i=0;i<18245;i++)
		{
		LandIntro[i]=fgetc(RawPCX);
		if(feof(RawPCX))
			{
			printf("Data file Error: LANDSCAP.PCX");
			exit(0);
		}
	}
	fclose(RawPCX);

	if((myFile=fopen("PART1.DAT","rb"))==0) exit(0);

	fread(Intro1,sizeof(char),42 *190,myFile);
	fread(Intro2,sizeof(char),152*160,myFile);
	fread(Intro3,sizeof(char),56 *190,myFile);
	fread(Intro4,sizeof(char),124*230,myFile);
	fread(Intro5,sizeof(char),124*194,myFile);

	fclose(myFile);

	LoadMusic(modfile1);

	StartOutput(channels, 0);
	StartMusic();

	Part1();	// Black Marble presents blah,blah,blah,blah,blah

	StopMusic();                         //Disable music processing
	StopOutput();                        //Turn off sound output
	UnloadModule();                      //Free module from memory
	StopBanner();                        //Turn off that damn signoff banner ;)
	FreeMSE();

	free((char*)Intro1);
	free((char*)Intro2);
	free((char*)Intro3);
	free((char*)Intro4);
	free((char*)Intro5);

	if(!(SkullPic=new char[64000]))   MemError();
	loadPCX("SKULL.PCX",SkullPic,0);
	loadPCXpal("SKULL.PCX",SkullPal);

	if((WorldPts=(int *)malloc(3100))==0 || (ScrPts=(int *)malloc(3100))==0)
		{
		MemError();
	}
	if((myPixel=(Pixel *)malloc(sizeof(Pixel)*151))==0)
		{
		MemError();
	}
	if((MyFonts=(FONT *)malloc(sizeof(FONT)*(']'-' ')))==0)
		{
		MemError();
	}
	if((Bitmap=(char *)malloc(13440))==0)
		{
		MemError();
	}

	LoadPalFile("FONT9.PAL");
	LoadFont("FONT9.TED");

	LoadMusic(modfile2);

	StartOutput(channels, 0);
	StartMusic();

	Part2();  // Star field and scroller with intro greetings
	Part3();	// Vector Balls part
	Part4();	// Pixel "dot" Landscape
	Part5();  // Fire (Plasma)
	Part7();	// Lens effect

	Volume=64;
	for(i=0;i<64;i++)
		{
		Volume--;
		MusicVolume(Volume);
		delay(40);
	}

	StopMusic();                         //Disable music processing
	StopOutput();                        //Turn off sound output
	UnloadModule();                      //Free module from memory
	StopBanner();                        //Turn off that damn signoff banner ;)
	FreeMSE();

	delete SkullPic;
	free(myPixel);
	free(Bitmap);
	free(TransFormBuf);
	free(TempBuf);
	free(VBallIntro);
	free(LandIntro);

	LoadPalFile("FONT9.PAL");
	LoadMusic(modfile3);

	StartOutput(channels, 0);
	StartMusic();

	Part8();	// Credits

	StopMusic();                         //Disable music processing
	StopOutput();                        //Turn off sound output
	UnloadModule();                      //Free module from memory
	StopBanner();                        //Turn off that damn signoff banner ;)
	FreeMSE();                           //Free MSE from memory

	asm mov 	ax,0x03
	asm int	0x10
}


