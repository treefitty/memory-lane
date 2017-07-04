#include <conio.h>
#include <mem.h>
#include <stdio.h>
#include <dos.h>
#include <bios.h>
#include <math.h>
#include <igraph.h>
#include <ipcx.h>
#include <sprite.h>
#include <ieffects.h>

#define a_COS(a)  \
	COS[a]       // Reference SIN, COS tables as macros for increased speed.
#define a_SIN(a)  \
	SIN[a]
#define PointsN	4*4*4

int Points[PointsN*3];
int Screen[PointsN*2];
int PostXadd,PostYadd,PreZadd;
int cubeYaw=0,cubeRoll=0,cubePitch=0;
int CubeWX=0,CubeWY=0,CubeWZ=120;
int ObjPointRoll=0,ObjPointYaw=0,ObjPointPitch=0;
int ScreenX,ScreenY,objz;
char done=0;
int top_xpos;
int bot_xpos;
int waited;
unsigned char far *buffer;
unsigned char far *mcsPCX;
const float PI = 3.1415927;           // Variables used in 3D rotation
int x,y,z;
int xa,ya,za;
static int COS[370], SIN[370];
long timer=0;

void poll_keybrd(void)
{
	if(_bios_keybrd(_KEYBRD_READY!=0))
		{
		_bios_keybrd(_KEYBRD_READ);
		done=1;
	}
}

void InitAngleArrays(void)            // Initialize SIN, COS arrays.
{
	int a;                           // Variables used only in here.
	float radians;

	for ( a = 0; a < 360; a++ )     // For one full rotation or 360 degrees.
		{
		radians = (float)a * PI / 180; // Convert degrees to radians.
		COS[a] = (int)(cos(radians) * 256);// Multiply by 256 and round
		SIN[a] = (int)(sin(radians) * 256);// off by converting to integer.
	}
}

void FadePaletteIn( char *Array )
{
	char tmpPal[768];
	int c;

	memset( tmpPal, 0, 768 );

	for(x=0;x<64;x++)
		{
		delay(20);
		setpalette(tmpPal);
		for ( c = 0; c < 765; c++ )
			{
			if ( tmpPal[c] != Array[c] ) tmpPal[c]++;
			else tmpPal[c]=0;
		}
	}
}

void main(void)
{
	char myPal[768];

	buffer= new unsigned char[64000];
	mcsPCX = new unsigned char[64000];
	memset(buffer,0,64000);
	setgraphics();
	loadPCX("MCS.PCX",mcsPCX,1);
	InitAngleArrays();
	melt_on(mcsPCX);
	loadPCXpal("MCS.PCX",myPal);
	Sprite dimLight1(6,9);
	Sprite dimLight2(6,9);
	Sprite brightLight1(6,9);
	Sprite brightLight2(6,9);

	dimLight1.grab(mcsPCX,67,51);
	dimLight2.grab(mcsPCX,56,139);
	brightLight1.grab(mcsPCX,57,51);
	brightLight2.grab(mcsPCX,76,139);

	waited=2;
	for (int i=0;i<4;i++) 					/* z */
		{
			for (int i1=0;i1<4;i1++)		/* y */
			 {
				for (int i2=0;i2<4;i2++)	/* x */
				 {
					Points[i*(16*3)+i1*(4*3)+i2*3+0]=( -40)+i2*30;		/* x */
					Points[i*(16*3)+i1*(4*3)+i2*3+1]=( -40)+i1*30;		/* y */
					Points[i*(16*3)+i1*(4*3)+i2*3+2]=( -40)+ i*30;		/* z */
				 }
			 }
		}
	PreZadd=-893;
	PostXadd=160;
	PostYadd=25;
	for(timer=0;timer<=58;timer++)
		{
		x=CubeWX;
		y=CubeWY;
		z=CubeWZ;
		xa=(int) ((a_COS(ObjPointRoll)*x)  + (a_SIN(ObjPointRoll)*y)) >>8;
		ya=(int) ((a_COS(ObjPointRoll)*y)  - (a_SIN(ObjPointRoll)*x)) >>8;
		x =(int) ((a_COS(ObjPointYaw)*xa)  - (a_SIN(ObjPointYaw)*z))  >>8;
		za=(int) ((a_SIN(ObjPointYaw)*xa)  + (a_COS(ObjPointYaw)*z))  >>8;
		z =(int) ((a_COS(ObjPointPitch)*za)-(a_SIN(ObjPointPitch)*ya))>>8;
		y =(int) ((a_SIN(ObjPointPitch)*za)+(a_COS(ObjPointPitch)*ya))>>8;
		PostXadd=160+x;
		PostYadd=25+y;
		ObjPointYaw+=3;
		if(ObjPointYaw>=360) ObjPointYaw=0;
		for(int i=0;i<PointsN;i++)
			{
			x=Points[i*3+0];
			y=Points[i*3+1];
			z=Points[i*3+2];

			_ES=FP_SEG(mcsPCX);
			_DI=FP_OFF(mcsPCX)+(Screen[(i*2)+1]*320)+Screen[i*2];
			asm xor al,	al
			asm mov es:[di],al

			xa=(int) ((a_COS(cubeRoll)*x)  + (a_SIN(cubeRoll)*y)) >>8;
			ya=(int) ((a_COS(cubeRoll)*y)  - (a_SIN(cubeRoll)*x)) >>8;
			x =(int) ((a_COS(cubeYaw)*xa)  - (a_SIN(cubeYaw)*z))  >>8;
			za=(int) ((a_SIN(cubeYaw)*xa)  + (a_COS(cubeYaw)*z))  >>8;
			z =(int) ((a_COS(cubePitch)*za)-(a_SIN(cubePitch)*ya))>>8;
			y =(int) ((a_SIN(cubePitch)*za)+(a_COS(cubePitch)*ya))>>8;
			objz=z+PreZadd;
			if(objz<0) objz=-PreZadd;
			ScreenX=((x<<8)/objz)+PostXadd;
			ScreenY=((y<<8)/objz)+PostYadd;
			Screen[i*2+0]=ScreenX;
			Screen[i*2+1]=ScreenY;

			_ES=FP_SEG(mcsPCX);
			_DI=FP_OFF(mcsPCX)+(ScreenY*320)+ScreenX;
			asm mov al,	15
			asm mov es:[di],al
		}
		cubeYaw+=3;
		cubeRoll+=2;
		cubePitch+=2;
		if(cubeRoll>=360) cubeRoll=0;
		if(cubeYaw>=360) cubeYaw=0;
		if(cubePitch>=360) cubePitch=0;

		switch(waited)
			{
			case 4 :
				top_xpos=57;
				bot_xpos=256;
				for(int count=0;count<7;count++,top_xpos+=30,bot_xpos-=30)
					{
					brightLight1.put(top_xpos,51,mcsPCX);
					dimLight1.put(top_xpos+10,51,mcsPCX);
					dimLight1.put(top_xpos+20,51,mcsPCX);

					brightLight2.put(bot_xpos,139,mcsPCX);
					dimLight2.put(bot_xpos-10,139,mcsPCX);
					dimLight2.put(bot_xpos-20,139,mcsPCX);
					poll_keybrd();
				}
				waited++;
				break;
			case 8:
				top_xpos=57;
				bot_xpos=256;
				for(count=0;count<7;count++,top_xpos+=30,bot_xpos-=30)
					{
					dimLight1.put(top_xpos,51,mcsPCX);
					brightLight1.put(top_xpos+10,51,mcsPCX);
					dimLight1.put(top_xpos+20,51,mcsPCX);

					dimLight2.put(bot_xpos,139,mcsPCX);
					brightLight2.put(bot_xpos-10,139,mcsPCX);
					dimLight2.put(bot_xpos-20,139,mcsPCX);
					poll_keybrd();
				}
				waited++;
				break;
			case 12:
				waited=0;
				top_xpos=57;
				bot_xpos=256;
				for(count=0;count<7;count++,top_xpos+=30,bot_xpos-=30)
					{
					dimLight1.put(top_xpos,51,mcsPCX);
					dimLight1.put(top_xpos+10,51,mcsPCX);
					brightLight1.put(top_xpos+20,51,mcsPCX);

					dimLight2.put(bot_xpos,139,mcsPCX);
					dimLight2.put(bot_xpos-10,139,mcsPCX);
					brightLight2.put(bot_xpos-20,139,mcsPCX);
					poll_keybrd();
				}
				waited++;
				break;
			default:
				waited++;
				break;
		}

			asm cli
			asm mov dx,0x3da
		l1:
			asm in al,dx
			asm and al,0x08
			asm jnz l1

		l2:
			asm in al,dx
			asm and al,0x08
			asm jz  l2
			asm sti

		putbuffer(mcsPCX);

		poll_keybrd();
		if(done==1) break;
	}
	FadeOff(myPal);
	settextmode();
	delete buffer;
	delete mcsPCX;
}