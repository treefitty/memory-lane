#include <stdio.h>
#include <dos.h>
#include <bios.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <alloc.h>
#include <conio.h>

#include <igraph.h>
#include <ieffects.h>

#include <bwsb.h>

#define ChOfs 32

class FONT {

public:

	char ASCIIPos,Defined;
	char Width,Height;
	char far *Dat;
	int temp;

	void Init(char w,char h,char Pos);
	void DeInit(void);

};

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

};

extern int i,k;
extern char done;
extern int LinearMulTab[200];
extern int PointPitch,PointYaw,PointRoll;
extern int PostXadd,PostYadd;
extern int COS[360],SIN[360];
extern int xa,ya,za;
extern int x,y,z;
extern unsigned char TempPal[768];
extern unsigned char MyPal[768];
extern char far *Scr;
extern FONT *MyFonts;
extern Pixel *myPixel;
extern int *WorldPts, *ScrPts;
extern char far *Bitmap;

int StrOfs,pos;
int BounceAngle;
int IncFactor;
int NumPixelsOff=0;
char CharColumn,temp;
char CurrChar;
char Message[]="		   \
AAAHH - OH NO ANOTHER STUPID SCROLLY.....              \
WELL LET'S MAKE THIS ONE AS QUICK AS POSSIBLE  :-)                 \
THE WHOLE DEMO WAS DESIGNED AND CODED INSIDE OF 4-5 WEEKS.....               \
NOT BAD FOR OUR FIRST EFFORT EH?                \
WE FEEL THAT IT COULD HAVE BEEN HEAPS BETTER IF WE HAD MORE\
 TIME, THERE ARE FAR TOO MANY BUGS AND UNFINISHED BITS.....                       \
OK ENOUGH GABBING, NOW ON WITH THE SHOW!                             ";

void RotateBitmap(char far *Bit)
{
	asm push 	ds

	asm lds	si,Bit
	asm les	di,Bit
	asm add	si,2
	asm mov	cx,6720-2
	asm rep	movsw

	asm pop	ds
}

void Update(void)
{
	CharColumn++;
	if(CharColumn>MyFonts[CurrChar-ChOfs].Width)
		{
		StrOfs++;
		if(StrOfs>strlen(Message)-1)
			{
			done=1;
		}
		CharColumn=0;
		CurrChar=Message[StrOfs];
	}
}

void PutNextRow(char far *Bit,int BitOfs)
{
	int k,Limit;

	i=BitOfs;
	Limit=MyFonts[CurrChar-ChOfs].Width*MyFonts[CurrChar-ChOfs].Height;
	for(k=CharColumn;k<Limit;k+=MyFonts[CurrChar-ChOfs].Width,i+=320)
		{
		if(MyFonts[CurrChar-ChOfs].Defined==1)
			{
			Bit[i]=MyFonts[CurrChar-ChOfs].Dat[k];
		}
		else Bit[i]=MyFonts[0].Dat[0];
	}

}

void PutBit(char far *Bit)
{
	asm push 	ds

	asm mov	ax,0xa000
	asm mov	es,ax
	asm mov	di,0
	asm lds	si,Bit
	asm mov	cx,6720
	asm rep	movsw

	asm pop	ds
}

void UpdateSphere(void)
{
	for(i=0;i<1500;i+=3)
		{
		if(ScrPts[i+1]<200 && ScrPts[i+1]>0 && ScrPts[i]>0 && ScrPts[i]<320)
			Scr[LinearMulTab[ScrPts[i+1]]+ScrPts[i]]=Scr[LinearMulTab[ScrPts[i+1]]];
		x=WorldPts[i];
		y=WorldPts[i+1];
		z=WorldPts[i+2];
		xa=((COS[PointRoll] *x) + (SIN[PointRoll] *y)) >> 8;
		ya=((COS[PointRoll] *y) - (SIN[PointRoll] *x)) >> 8;
		x =((COS[PointYaw]  *xa)- (SIN[PointYaw]  *z)) >> 8;
		za=((SIN[PointYaw]  *xa)+ (COS[PointYaw]  *z)) >> 8;
		z =((COS[PointPitch]*za)- (SIN[PointPitch]*ya))>> 8;
		y =((SIN[PointPitch]*za)+ (COS[PointPitch]*ya))>> 8;
		ScrPts[i]=x+PostXadd;
		ScrPts[i+1]=y+PostYadd;
		if(ScrPts[i+1]<200 && ScrPts[i+1]>0 && ScrPts[i]>0 && ScrPts[i]<320)
			{
			z= -z;
			pos=LinearMulTab[ScrPts[i+1]]+ScrPts[i];
			if(z<0)  Scr[pos]=21;
			if(z<-10)Scr[pos]=23;
			if(z<-20)Scr[pos]=24;
			if(z<-30)Scr[pos]=26;
			if(z<-40)Scr[pos]=28;
			if(z<-50)Scr[pos]=30;

			if(z>=0) Scr[pos]=18;
			if(z>25) Scr[pos]=16;
			if(z>50) Scr[pos]=15;
		}
	}
	PointPitch+=1;
	PointYaw  +=2;
	PointRoll +=3;
	if(PointPitch>=360) PointPitch=0;
	if(PointRoll >=360) PointRoll=0;
	if(PointYaw  >=360) PointYaw=0;
}

void InitBall(void)
{
	x= 50;
	y= 0;
	z= 0;
	randomize();
	for(i=0;i<1500;i+=3)  // Init the sphere.
		{
		PointRoll = random(359);
		PointPitch= random(359);
		PointYaw  = random(359);
		xa=((COS[PointRoll] *x) + (SIN[PointRoll] *y))  >> 8;
		ya=((COS[PointRoll] *y) - (SIN[PointRoll] *x))  >> 8;
		x =((COS[PointYaw]  *xa)- (SIN[PointYaw]  *z))  >> 8;
		za=((SIN[PointYaw]  *xa)+ (COS[PointYaw]  *z))  >> 8;
		z =((COS[PointPitch]*za)- (SIN[PointPitch]*ya)) >> 8;
		y =((SIN[PointPitch]*za)+ (COS[PointPitch]*ya)) >> 8;
		WorldPts[i]= x;
		WorldPts[i+1]= y;
		WorldPts[i+2]= z;
		ScrPts[i]=x+PostXadd;
		ScrPts[i+1]=y+PostYadd;
		PointPitch=1;
		PointYaw  =2;
		PointRoll =3;
		x= 50;
		y= 0;
		z= 0;
	}
}

void Part2(void)
{
	asm mov ax,0x13
	asm int	0x10

//	sleep(3);

	memset(Bitmap,0,13440);
	memset(MyPal,0,768);
	setpalette(MyPal);

	done=0;
	StrOfs=0;
	CharColumn=0;
	CurrChar=Message[0];

	for(y=113;y<116;y++)
		for(x=0;x<320;x++) Scr[LinearMulTab[y]+x]=157;    // Number 1

	for(y=116;y<=121;y++)
		for(x=0;x<320;x++) Scr[LinearMulTab[y]+x]=156;    // Number 2

	for(y=122;y<=130;y++)
		for(x=0;x<320;x++) Scr[LinearMulTab[y]+x]=155;    // Number 3

	for(y=131;y<=141;y++)
		for(x=0;x<320;x++) Scr[LinearMulTab[y]+x]=154;    // Number 4

	for(y=142;y<=157;y++)
		for(x=0;x<320;x++) Scr[LinearMulTab[y]+x]=153;    // Number 5

	for(y=158;y<=176;y++)
		for(x=0;x<320;x++) Scr[LinearMulTab[y]+x]=152;    // Number 6

	for(y=177;y<=199;y++)
		for(x=0;x<320;x++) Scr[LinearMulTab[y]+x]=151;    // Number 7

	FadeIn(TempPal);

//	sleep(3);

	PostXadd=380;
	PostYadd=100;
	done=0;
	BounceAngle=120;
	IncFactor=1;
	InitBall();
	while(!done)
		{
		PostYadd=155-(0.5*SIN[BounceAngle]);
		BounceAngle+=2;
		if(BounceAngle>=180) BounceAngle=0;
		PostXadd-=IncFactor;
		if(PostXadd==160) done=1;

		asm mov	dx,0x3da

	VRI_1:

		asm in	al,dx
		asm test	al,8
		asm jz	VRI_1

	NVRI_1:

		asm in	al,dx
		asm test	al,8
		asm jnz	NVRI_1

		UpdateSphere();
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			done=1;
		}
	}
	done=0;
	while(!done)
		{
		RotateBitmap(Bitmap);
		Update();
		PutNextRow(Bitmap,318);
		Update();
		PutNextRow(Bitmap,319);

		UpdateSphere();
		PutBit(Bitmap);
		asm mov	dx,0x3da

	XVRI_2:

		asm in	al,dx
		asm test	al,8
		asm jz	XVRI_2
/*

	VRI_2:

		asm in	al,dx
		asm test	al,8
		asm jz	VRI_2
*/
//delay(1);
		//UpdateSphere();
		//PutBit(Bitmap);
//		delay(3);

		asm mov ah,	0x01     // Again check for ESC key pressed
		asm int		0x16     // (same as before)
		asm jz		NoKey1
		asm xor ah,	ah
		asm int		0x16
		asm mov done,	1        // If pressed, get out of here

	NoKey1:

	}
	if(BounceAngle>=180 || BounceAngle<0) BounceAngle=0;
	done=0;
	while(!done)
		{
		PostYadd=155-(0.5*SIN[BounceAngle]);
		BounceAngle+=2;
		if(BounceAngle>=180) BounceAngle=0;
		PostXadd-=IncFactor;
		if(PostXadd<=52) IncFactor=-1;
		if(PostXadd>=165) done=1;

		asm mov	dx,0x3da

	VRI_3:

		asm in	al,dx
		asm test	al,8
		asm jz	VRI_3

	NVRI_3:

		asm in	al,dx
		asm test	al,8
		asm jnz	NVRI_3

		UpdateSphere();
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READ);
			done=1;
		}
	}

	UpdateSphere();

	randomize();
	for(i=0,k=0;i<150;i++,k+=3)
		{
		myPixel[i].x=ScrPts[k];
		myPixel[i].y=ScrPts[k+1];
		myPixel[i].yact= 200-myPixel[i].y;

		myPixel[i].delimeter=(float)myPixel[i].yact/256;
		myPixel[i].startdelimeter=myPixel[i].delimeter+(float)(rand()%15)/45;
		myPixel[i].bounced=random(2)-1;

		myPixel[i].starting=1;

		myPixel[i].status=1;

		myPixel[i].XIncFactor= ((float)(rand()%10)/10)+0.1;

		myPixel[i].OrigInc=myPixel[i].XIncFactor;

		myPixel[i].bgCol=Scr[LinearMulTab[myPixel[i].y]];
	}
	for(i=0;i<150;i++)
		{
		for(int k=0;k<180;k++)
			{
			if(myPixel[i].yact>=myPixel[i].startdelimeter*SIN[k]-2 && myPixel[i].yact<=myPixel[i].startdelimeter*SIN[k]+2)
				{
				myPixel[i].angle=k;
			}
		}
	}
	asm mov	ax,0xa000
	asm mov	es,ax
	asm xor	ax,ax
	asm mov	di,0
	asm mov	cx,16000
	asm rep 	stosw

	i=0;
	done=0;
	while(!done)
		{
		// Only go in here if the pixel is starting and it's on.
		if(myPixel[i].starting==1 && myPixel[i].status==1)
			{
			if(myPixel[i].y>=0)
				{
				Scr[(myPixel[i].y*320)+myPixel[i].x]=myPixel[i].bgCol;
			}

			if(myPixel[i].bounced==0)
				{
				myPixel[i].x+=myPixel[i].XIncFactor;
			}
			else {
				myPixel[i].x-=myPixel[i].XIncFactor;
			}

			if(myPixel[i].x<4)
				{
				myPixel[i].bounced=0;
			}
			else {
				if(myPixel[i].x>318)
					{
					myPixel[i].bounced=1;
				}
			}
			myPixel[i].yact=myPixel[i].startdelimeter*SIN[myPixel[i].angle];
			myPixel[i].y=199-myPixel[i].yact;

			if(myPixel[i].y>=0)
				{
				myPixel[i].bgCol=Scr[(myPixel[i].y*320)];
				Scr[(myPixel[i].y*320)+myPixel[i].x]=15;
			}

			myPixel[i].angle++;                // Increment the angle
		}
		// Only do this if the pixel is not starting and it's on
		if(myPixel[i].starting==0 && myPixel[i].status==1)
			{
			if(myPixel[i].y>=0)
				{
				Scr[(myPixel[i].y*320)+myPixel[i].x]=myPixel[i].bgCol;
			}

			if(myPixel[i].bounced==0)
				{
				myPixel[i].x+=myPixel[i].XIncFactor;
			}
			else {
				myPixel[i].x-=myPixel[i].XIncFactor;
			}
			if(myPixel[i].x<4)
				{
				myPixel[i].bounced=0;
			}
			else {
				if(myPixel[i].x>318)myPixel[i].bounced=1;
			}
			myPixel[i].yact=myPixel[i].delimeter*SIN[myPixel[i].angle];
			myPixel[i].y=199-myPixel[i].yact;

			if(myPixel[i].y>=0)
				{
				myPixel[i].bgCol=Scr[(myPixel[i].y*320)];
				Scr[(myPixel[i].y*320)+myPixel[i].x]=15;
			}

			myPixel[i].angle++;
		}
		// Only do this if the angle =180 and the pixel is on
		if(myPixel[i].angle==180 && myPixel[i].status==1)
			{
			myPixel[i].starting=0;        // Make sure it's NOT starting
			myPixel[i].angle=0;        // Reset angle beginning of bounce.
			myPixel[i].delimeter-=.1;  // Reduce the max height reached

			// Only if the delimeter is less than .1
			if(myPixel[i].delimeter<=.1)
				{
				myPixel[i].status=0;   // Turn the pixel off
				NumPixelsOff++;        // Increment NumPixelsOff

				Scr[(myPixel[i].y*320)+myPixel[i].x]=myPixel[i].bgCol;
			}
			if(myPixel[i].OrigInc>1) myPixel[i].XIncFactor-=.21;

			asm mov ah,	0x01     // Again check for ESC key pressed
			asm int		0x16     // (same as before)
			asm jz		NoKey6
			asm xor ah,	ah
			asm int		0x16
			asm mov done,	1        // If pressed, get out of here

		NoKey6:

		}

			asm mov	dx,0x3da

		HRI:

			asm in	ax,dx
			asm test	ax,1
			asm jz	HRI

		if(NumPixelsOff==150) done=1; // If all the pixels are off exit
		i++;                          // Do next pixel
		if(i==150) i=0;               // if all pixels done start back at 0
	}
	FadeOff(TempPal);
}






