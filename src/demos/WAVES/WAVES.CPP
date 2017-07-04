#include <bios.h>
#include <mem.h>
#include <math.h>
#include <dos.h>

#include <ch4_lib.h>

#define PI 3.1415927
#define AMPLITUDE 3
#define CENTERWAVE 150
#define MAXAMP 7

char far *Scr=(char *)MK_FP(0xa000,0);
char done;
int i,Angle=0,Angle2=0;
int x,y,Limit;
int SIN[360],SIN2[360],SIN3[360],SIN4[360];
float radians;

void main(void)
{
	int temp,temp2;
	int offset=0;

	for(i=0;i<360;i++)
		{
		radians= (float)(i * PI) / 180;
		SIN[i] = (int)(sin(radians) * AMPLITUDE);
		SIN2[i]= (int)(sin(2*radians) * AMPLITUDE);
		SIN3[i]= (int)(sin(radians) * (AMPLITUDE+3));
		SIN4[i]= (int)(sin(2*radians) * (AMPLITUDE+2));
	}
	asm mov	ax,0x13
	asm int	0x10

	memset(&Scr[(150*320)],15,49*320);

	asm mov	dx,0x3c8
	asm mov	al,15
	asm out	dx,al
	asm inc	dx
	asm mov	al,10
	asm out 	dx,al
	asm mov	al,10
	asm out	dx,al
	asm mov	al,18
	asm out	dx,al

	done=0;
	while(!done)
		{
		temp=Angle;
		temp2=Angle2;
		for(x=0;x<320;x++)
			{
			Limit=((SIN[temp]+SIN2[temp]+SIN3[temp2]+SIN4[temp2])>>1)+CENTERWAVE;
			for(y=Limit,offset=(y*320)+x;y>=CENTERWAVE-MAXAMP;y--,offset-=320)
				{
				Scr[offset]=0;
			}
			for(y=150+MAXAMP,offset=(y*320)+x;y>=Limit;y--,offset-=320)
				{
				Scr[offset]=15;
			}
			temp+=4;
			if(temp>=360) temp=0;

			temp2+=2;
			if(temp2>=360) temp2=0;
		}
		Angle2++;
		if(Angle2>=360) Angle2=0;

		Angle--;
		if(Angle<=0) Angle=359;

		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			_bios_keybrd(_KEYBRD_READY);
			done=1;
		}

		c_wait_vr();
	}
	asm mov	ax,0x03
	asm int	0x10
}