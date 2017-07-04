#include <bios.h>
#include <dos.h>
#include <math.h>

#define PI 3.1415927
#define RightArrow 0x4d00
#define LeftArrow  0x4b00
#define UpArrow    0x4800
#define DownArrow  0x5000
#define Escape     0x11b

char far *Scr=(char *)MK_FP(0xa000,0);
char done;
int i;
int COS[360],SIN[360];
int TankWPts[4];
int TankSPts[4];
int TankX,TankY;
int angle;
int key;
int x,y,xa,ya;

void UpdateTank(void)
{
	if(angle>359) angle=0;
	if(angle<0) angle=359;
	for(i=0;i<4;i+=2)
		{
		Scr[(TankSPts[i+1]*320)+TankSPts[i]]=0;
		x=TankWPts[i];
		y=TankWPts[i+1];

		xa=((COS[angle] *x) + (SIN[angle] *y)) >> 8;
		ya=((COS[angle] *y) - (SIN[angle] *x)) >> 8;

		TankSPts[i]=xa+TankX;
		TankSPts[i+1]=ya+TankY;
		Scr[(TankSPts[i+1]*320)+TankSPts[i]]=15;
	}
}

void InitAngles(void)
{
	float radians;

	for(i=0;i<360;i++)
		{
		radians= (float)(i * PI) / 180;
		COS[i] = (int)(cos(radians) * 256);
		SIN[i] = (int)(sin(radians) * 256);
	}
}

void main(void)
{
	asm mov	ax,0x13
	asm int	0x10

	InitAngles();

	TankWPts[0]=0;TankWPts[1]=-20;
	TankWPts[2]=0;TankWPts[3]= 20;

	done=0;
	angle=0;
	TankX=160;
	TankY=100;

	asm mov	ax,0x0305
	asm mov	bx,0x0000
	asm int	0x16

	while(!done)
		{
///*
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			key=_bios_keybrd(_KEYBRD_READ);
			switch(key)
				{
				case RightArrow:
					{
					angle-=2;
					UpdateTank();
					break;
				}
				case LeftArrow:
					{
					angle+=2;
					UpdateTank();
					break;
				}
				case Escape:
					{
					done=1;
					break;
				}
			}
		}
//*/
	}
	asm mov	ax,0x03
	asm int   0x10
}