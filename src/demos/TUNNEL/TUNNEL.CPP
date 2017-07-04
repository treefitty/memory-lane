
#include <math.h>
#include <conio.h>
#include <bios.h>

#include <x_lib.h>

#define PI 			3.141256
#define NUM_SECTIONS 	40
#define AMPLITUDE 		3

int SIN1[360],SIN2[360],SIN3[360],SIN4[360];

void dot_circle(int x,int y,int r)
{
	int px,py;

	for(py=y-r;py<y+r;py++)
		{
		px=sqrt((r*r)-(py*py));

		x_set_pel(px+x,py,15);
	}
}

void main(void)
{
	int sin_t[360],cos_t[360];
	int pt_array[1][NUM_SECTIONS];
	int phi=0,rho=0,theta=0;
	int i;
	char done=0;
	float radians;

	for(i=0;i<360;i++)
		{
		radians= (float)(i * PI) / 180;

		SIN1[i] = (int)(sin(radians) * AMPLITUDE);
		SIN2[i]= (int)(sin(2*radians) * AMPLITUDE);
		SIN3[i]= (int)(sin(radians) * (AMPLITUDE+3));
		SIN4[i]= (int)(sin(2*radians) * (AMPLITUDE+2));
	}
	x_setmode();

	while(!done)
		{
		if(_bios_keybrd(_KEYBRD_READ)!=0)
			{
			done=1;
		}
		for(i=0;i<NUM_SECTIONS-2;i++)
			{
			pt_array[0][i]=pt_array[0][i+1];
			pt_array[1][i]=pt_array[1][i+1];
		}
		phi++;
		rho+=2;
		theta++;

		pt_array[0][i]=(SIN1[phi]+SIN2[phi]+SIN3[rho]+SIN4[rho])>>1;
		pt_array[1][i]=(SIN1[theta]+SIN2[theta]+SIN3[phi]+SIN4[phi])>>1;

		for(i=0;i<NUM_SECTIONS-1;i++)
			{
			put_pixel(pt_array[0][i]+160,
		}
	}

	x_textmode();
}

