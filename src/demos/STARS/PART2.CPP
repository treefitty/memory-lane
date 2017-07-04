#include <stdlib.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>

#include <ipcx.h>
#include <igraph.h>
#include <bitmap.h>

extern int LinearMulTab[200];
extern char done;
extern int i,x,y;

int WStarsX[300];
int WStarsY[300];
int WStarsZ[300];
int ScrStarsX[300];
int ScrStarsY[300];
int Line1Ofs=320,Line2Ofs=577,Line3Ofs=847;
int Line4Ofs=320,Line5Ofs=618,Line6Ofs=944,Line7Ofs=1154;
int Line8Ofs=320,Line9Ofs=645;
int Line1Done=0,Line2Done=0,Line3Done=0;
int Line4Done=0,Line5Done=0,Line6Done=0,Line7Done=0;
int Line8Done=0,Line9Done=0;

char far *Line1Dat,far *Line2Dat,far *Line3Dat;
char far *Line4Dat,far *Line5Dat,far *Line6Dat,far *Line7Dat;
char far *Line8Dat,far *Line9Dat;

char far *Scr=(char *)MK_FP(0xa000,0);

int WhichUpdate=1;

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

void UpdateScroller(void)
{
	int i1,i2,i3,i4;
	int SaveI;
	int Condition;

	i1=0;
	i2=0;
	i3=0;
	i4=0;

	if(WhichUpdate==1)
		{
		for(y=90;y<107;y++)
			{
			if(Line1Done==0)
				{
				SaveI=i1;
				Condition=Line1Ofs+242;
				if(Condition>320)Condition=320;
				for(x=Line1Ofs;x<Condition;x++,i1++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line1Dat[i1];
					}
				}
				i1=SaveI+242;
			}
			if(Line2Done==0)
				{
				SaveI=i2;
				Condition=Line2Ofs+255;
				if(Condition>320)Condition=320;

				for(x=Line2Ofs;x<Condition;x++,i2++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line2Dat[i2];
					}
				}
				i2=SaveI+255;
			}
			if(Line3Done==0)
				{
				SaveI=i3;
				Condition=Line3Ofs+266;
				if(Condition>320)Condition=320;

				for(x=Line3Ofs;x<Condition;x++,i3++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line3Dat[i3];
					}
				}
				i3=SaveI+266;
			}
		}
		Line1Ofs--;
		if((Line1Ofs+242)<0) Line1Done=1;
		Line2Ofs--;
		if((Line2Ofs+255)<0) Line2Done=1;
		Line3Ofs--;
		if((Line3Ofs+266)<0)
			{
			Line3Done=1;
			WhichUpdate=2;
		}
	}
	else if(WhichUpdate==2)
		{
		for(y=90;y<107;y++)
			{
			if(Line4Done==0)
				{
				SaveI=i1;
				Condition=Line4Ofs+284;
				if(Condition>320)Condition=320;

				for(x=Line4Ofs;x<Condition;x++,i1++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line4Dat[i1];
					}
				}
				i1=SaveI+284;
			}
			if(Line5Done==0)
				{
				SaveI=i2;
				Condition=Line5Ofs+312;
				if(Condition>320)Condition=320;

				for(x=Line5Ofs;x<Condition;x++,i2++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line5Dat[i2];
					}
				}
				i2=SaveI+312;
			}
			if(Line6Done==0)
				{
				SaveI=i3;
				Condition=Line6Ofs+197;
				if(Condition>320)Condition=320;

				for(x=Line6Ofs;x<Condition;x++,i3++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line6Dat[i3];
					}
				}
				i3=SaveI+197;
			}
			if(Line7Done==0)
				{
				SaveI=i4;
				Condition=Line7Ofs+230;
				if(Condition>320)Condition=320;

				for(x=Line7Ofs;x<Condition;x++,i4++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line7Dat[i4];
					}
				}
				i4=SaveI+230;
			}
		}
		Line4Ofs--;
		if((Line4Ofs+284)<0) Line4Done=1;
		Line5Ofs--;
		if((Line5Ofs+312)<0) Line5Done=1;
		Line6Ofs--;
		if((Line6Ofs+197)<0) Line6Done=1;
		Line7Ofs--;
		if((Line7Ofs+230)<0)
			{
			Line7Done=1;
			WhichUpdate=3;
		}
	}
	else if(WhichUpdate==3)
		{
		for(y=90;y<107;y++)
			{
			if(Line8Done==0)
				{
				SaveI=i1;
				Condition=Line8Ofs+310;
				if(Condition>320)Condition=320;

				for(x=Line8Ofs;x<Condition;x++,i1++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line8Dat[i1];
					}
				}
				i1=SaveI+310;
			}
			if(Line9Done==0)
				{
				SaveI=i2;
				Condition=Line9Ofs+105;
				if(Condition>320)Condition=320;

				for(x=Line9Ofs;x<Condition;x++,i2++)
					{
					if(x>0 && x<320)
						{
						Scr[LinearMulTab[y]+x]=Line9Dat[i2];
					}
				}
				i2=SaveI+105;
			}
		}
		Line8Ofs--;
		if((Line8Ofs+310)<0) Line8Done=1;

		Line9Ofs--;
		if((Line9Ofs+105)<0)
			{
			Line9Done=1;
			WhichUpdate=4;
		}
	}
}

void Part2(void)
{
	int timer=0;
	char far *FileBuf;
	char col;

	FileBuf=new char[64000];

	Line1Dat=new char[242*17];
	Line2Dat=new char[255*17];
	Line3Dat=new char[266*17];
	Line4Dat=new char[284*17];
	Line5Dat=new char[312*17];
	Line6Dat=new char[197*17];
	Line7Dat=new char[230*17];
	Line8Dat=new char[310*17];
	Line9Dat=new char[105*17];

	asm mov	ax,0x13
	asm int	0x10

	loadPCX("STARMESS.PCX",FileBuf,1);

	getbitmap(Line1Dat,FileBuf,242,17,0,1);
	getbitmap(Line2Dat,FileBuf,255,17,0,22);
	getbitmap(Line3Dat,FileBuf,266,17,0,42);
	getbitmap(Line4Dat,FileBuf,284,17,0,63);
	getbitmap(Line5Dat,FileBuf,312,17,0,82);
	getbitmap(Line6Dat,FileBuf,197,17,0,103);
	getbitmap(Line7Dat,FileBuf,230,17,0,124);
	getbitmap(Line8Dat,FileBuf,310,17,0,145);
	getbitmap(Line9Dat,FileBuf,105,17,0,166);

	SetGray(16,60);
	SetGray(18,46);
	SetGray(20,33);
	SetGray(22,21);
	SetGray(24,10);
	randomize();
	for(i=0;i<300;i++)
		{
		CalcNewStar(i);
	}
	delete FileBuf;
	while(!done)
		{
		delay(3);
		UpdateScroller();
		if(WhichUpdate==4)
			{
			timer++;
			if(timer==900) done=1;
		}
		for(i=0;i<300;i++)
			{
			WStarsZ[i]--;

			if(Scr[LinearMulTab[ScrStarsY[i]]+ScrStarsX[i]]>=16)
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
			if(WStarsZ[i]>10) col=16;
			if(WStarsZ[i]>70) col=18;
			if(WStarsZ[i]>150) col=20;
			if(WStarsZ[i]>210) col=22;
			if(WStarsZ[i]>270) col=24;
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
			done=1;
		}
	}
	delete Line1Dat;
	delete Line2Dat;
	delete Line3Dat;
	delete Line4Dat;
	delete Line5Dat;
	delete Line6Dat;
	delete Line7Dat;
}