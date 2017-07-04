#include <stdlib.h>
#include <string.h>
#include <mem.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>

#include <igraph.h>
#include <bitmap.h>

#include <bwsb.h>

class FONT {

public:

	char ASCIIPos,Defined;
	char Width,Height;
	char far *Dat;
	int temp;

	void Init(char w,char h,char Pos);
	void DeInit(void);

};

extern FONT *MyFonts;
extern char far *VirtScr;
extern char far *Scr;
extern unsigned char TempPal[768];
extern int LinearMulTab[200];
extern char done;
extern int i,x,y;
int x1,x2;

int WStarsX[300];
int WStarsY[300];
int WStarsZ[300];
int ScrStarsX[300];
int ScrStarsY[300];

void CalcNewStar(int star)
{
	WStarsX[star]=random(320)-160;
	WStarsY[star]=random(200)-100;
	WStarsZ[star]=random(300)+10;
	if(WStarsX[star]==0)WStarsX[star]++;
	if(WStarsY[star]==0)WStarsY[star]++;
	ScrStarsX[i]=((WStarsX[i]<<7)/WStarsZ[i])+160;
	ScrStarsY[i]=((WStarsY[i]<<7)/WStarsZ[i])+100;
}

void SetGray(char reg,char shade)
{
	asm mov	dx,0x3c8
	asm mov	al,reg
	asm out	dx,al
	asm inc	dx
	asm mov	al,shade
	asm out	dx,al
	asm out	dx,al
	asm out	dx,al
}

void PrintString(char *String,int y)
{
	int tempx,tempy,count,NumItems,Ofs,x;
	char Char;

	NumItems=strlen(String);
     tempx=0;
	for(count=0;count<NumItems;count++)
		{
		Char=String[count]-32;
		tempx+=MyFonts[Char].Width;
	}
	x=160-(tempx/2);
	tempx=x;
	Ofs=0;
	for(count=0;count<NumItems;count++)
		{
		Char=String[count]-32;
		for(tempy=y;tempy<y+MyFonts[Char].Height;tempy++)
			{
			for(tempx=x;tempx<x+MyFonts[Char].Width;tempx++)
				{
				VirtScr[(tempy*320)+tempx+Ofs]=MyFonts[Char].Dat[((tempy-y)*MyFonts[Char].Width)+tempx-x];
			}
		}
		Ofs+=MyFonts[Char].Width;
	}
}

void Part8(void)
{
	int Shade1=0,Shade2=0,Shade3=0,Shade4=0,Shade5=0;
	int Delay=1200;
	char CreditDone=0,Seq=1;
	char col;
	char Ending=0;
	char ModVol;
	char EndDelay=0;
	char Starting=0;
	char StartDelay=5;

	asm mov	ax,0x13
	asm int	0x10

	setpalette(TempPal);

	SetGray(1,0);
	SetGray(2,0);
	SetGray(3,0);
	SetGray(4,0);
	SetGray(5,0);

	randomize();
	for(i=0;i<300;i++)
		{
		CalcNewStar(i);
	}

	sleep(3);

	memset(VirtScr,0,64000);

	x1=0;
	x2=319;
	while(!done)
		{
		delay(3);

		if(Starting==0)
			{
			if(StartDelay==0)
				{
				if(Shade1<60) { Shade1++; SetGray(1,Shade1); }
				else Starting=1;
				if(Shade1<46) { Shade2++; SetGray(2,Shade2); }
				if(Shade1<33) { Shade3++; SetGray(3,Shade3); }
				if(Shade1<21) { Shade4++; SetGray(4,Shade4); }
				if(Shade1<10) { Shade5++; SetGray(5,Shade5); }
				StartDelay=5;
			}
			StartDelay--;
		}

		if(Delay==0 && Seq==1)
			{
			switch(CreditDone)
				{
				case 0:
					{
					memset(VirtScr,0,64000);

					PrintString("CODE-DESIGN",10);
					PrintString("GRAPHICS",50);
					PrintString("PAUL",110);
					PrintString("CARROLL",150);

					break;
				}
				case 1:
					{
					memset(VirtScr,0,64000);

					PrintString("CODE-DESIGN",50);
					PrintString("SIMON KERR",100);

					break;
				}
				case 2:
					{
					memset(VirtScr,0,64000);

					PrintString("MUSIC-DESIGN",20);
					PrintString("NATHAN",80);
					PrintString("OLDAKER",120);

					break;
				}
				case 3:
					{
					memset(VirtScr,0,64000);

					PrintString("GRAPHICS",20);
					PrintString("DESIGN",60);
					PrintString("JOHN NOVAK",120);
					break;
				}
				case 4:
					{
					memset(VirtScr,0,64000);

					PrintString("WE HOPE YOU",5);
					PrintString("ENJOYED",55);
					PrintString("WATCHING",105);
					PrintString("THIS DEMO",155);

					break;
				}
				case 5:
					{
					memset(VirtScr,0,64000);

					PrintString("AS MUCH",20);
					PrintString("WE ENJOYED",70);
					PrintString("MAKING IT!",120);
					break;
				}
				case 6:
					{
					memset(VirtScr,0,64000);

					PrintString("GOODBYE.",85);
					break;
				}
				case 7:
					{
					memset(VirtScr,0,64000);

					PrintString("AND THANKS",35);
					PrintString("FOR",85);
					PrintString("WATCHING",135);
					break;
				}
				case 8:
					{
					EndDelay=5;
					Ending=1;
					ModVol=MusicVolume(0xFF);
					break;
				}
			}
			x1=0;
			x2=319;
			CreditDone++;
			Delay=800;
			Seq=0;
		}
		else if(Seq==0 && Delay==0)
			{
			x1=0;
			x2=319;
			Delay=800;
			Seq=1;
			memset(VirtScr,0,64000);
		}
		else {
			x1++;
			x2--;

			if(x1<320)
				{
				for(y=0;y<200;y+=2)
					{
					Scr[(y*320)+x1]=VirtScr[(y*320)+x1];
				}
				for(y=1;y<200;y+=2)
					{
					Scr[(y*320)+x2]=VirtScr[(y*320)+x2];
				}
			}
			Delay--;
		}

		if(Ending==1)
			{
			EndDelay--;
			if(EndDelay==0)
				{
				ModVol--;
				MusicVolume(ModVol);
				if(ModVol==0) done=1;
				EndDelay=5;

				Shade1--;
				if(Shade1<0)Shade1=0;

				Shade2--;
				if(Shade2<0)Shade2=0;

				Shade3--;
				if(Shade3<0)Shade3=0;

				Shade4--;
				if(Shade4<0)Shade4=0;

				Shade5--;
				if(Shade5<0)Shade5=0;

				SetGray(1,Shade1);
				SetGray(2,Shade2);
				SetGray(3,Shade3);
				SetGray(4,Shade4);
				SetGray(5,Shade5);
			}
		}

		for(i=0;i<300;i++)
			{
			WStarsZ[i]--;

			if(Scr[LinearMulTab[ScrStarsY[i]]+ScrStarsX[i]]<=5)
				{
				asm mov	ax,0xa000
				asm mov	es,ax
				_DI=LinearMulTab[ScrStarsY[i]]+ScrStarsX[i];
				asm mov	al,0
				asm mov	es:[di],al
			}

			ScrStarsX[i]=((WStarsX[i]<<7)/WStarsZ[i])+160;
			ScrStarsY[i]=((WStarsY[i]<<7)/WStarsZ[i])+100;
			if(WStarsZ[i]<10)
				{
				CalcNewStar(i);
			}
			if(WStarsZ[i]>10)  col=1;
			if(WStarsZ[i]>70)  col=2;
			if(WStarsZ[i]>150) col=3;
			if(WStarsZ[i]>210) col=4;
			if(WStarsZ[i]>270) col=5;
			if(ScrStarsX[i]>0&&ScrStarsX[i]<320&&ScrStarsY[i]>0&&ScrStarsY[i]<200)
				{
				if(Scr[LinearMulTab[ScrStarsY[i]]+ScrStarsX[i]]==0)
					{
					asm mov	ax,0xa000
					asm mov	es,ax
					_DI=LinearMulTab[ScrStarsY[i]]+ScrStarsX[i];
					asm mov	al,col
					asm mov	es:[di],al
				}
			}
		}
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			Ending=1;
			EndDelay=5;
			ModVol=MusicVolume(0xFF);
		}
	}
}