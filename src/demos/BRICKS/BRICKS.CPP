#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <stdio.h>
#include <bios.h>

#include <ipcx.h>
#include <igraph.h>
#include <ikeybrd.h>
#include <bitmap.h>

#define numstars 130
#define ORANGE 1
#define PURPLE 2
#define CYAN   3
#define GREEN  4
#define RED    5
#define YELLOW 6

#ifdef __cplusplus
    #define __CPPARGS ...
#else
    #define __CPPARGS
#endif

const int escape=0x01;
const int rt_arrow= 0x4d;
const int left_arrow= 0x4b;

unsigned char far *screen=(char *)MK_FP(0xa000,0);

char far *filebuf;
char far *O_brick;           // Different bricks
char far *P_brick;
char far *C_brick;
char far *G_brick;
char far *R_brick;
char far *Y_brick;
char far *blank;
char far *ball;              // The ball...
char far *bat;               // and the bat
int x,y,i,temp,key;
char done=0,hit=0;
char keyflags[128];

extern "C" void set_ikb(void);
extern "C" void reset_ikb(void);

class StarField                       // This is a simple class for
{                                     // a 2D star field with four "levels"
public:                               // or different degrees of depth.

	char firstdelay;
	char delay;
	int x;
	int y;
	char col;
	void Init(void);                 // First Init proc. Called once only
	void Update(void);               // Determine next position of star
	void ReFresh(void);              // If star off screen reset to right
} *stars;                             // side.

struct Object
{
	int x;
	int y;
	int w;
	int h;
	char status;
	char flag;

} *brick,Bat_Object;

struct Ball
{
	int oldx;
	int oldy;
	int x;
	int y;
	int w;
	int h;
	int xinc;
	int yinc;
	char starting;

}Ball_Object;

void StarField::Init(void)
{
	x=random(320);
	y=random(200);
	delay=random(4)+1;
	firstdelay=delay;                // Determine color depending on depth
	if(delay==1) col=17;
	if(delay==2) col=21;
	if(delay==3) col=25;
	if(delay==4) col=28;
}

void StarField::Update(void)
{
	if(delay==0)                     // Update pos
		{
		x--;
		delay=firstdelay;
	}
	else delay--;
	if(x<=1) ReFresh();              // If off left of screen reset to right
}

void StarField::ReFresh(void)
{
	y=random(200);
	x=319;
	delay=firstdelay;
}

void StarRout(void)
{
	for(i=0;i<numstars-1;i++)           // Update stars
		{
		temp=stars[i].col;
		if(screen[(stars[i].y*320)+stars[i].x]<30)screen[(stars[i].y*320)+stars[i].x]=0;
		stars[i].Update();
		if(screen[(stars[i].y*320)+stars[i].x]==0)screen[(stars[i].y*320)+stars[i].x]=stars[i].col;
	}
}

void CheckKeys(void)
{
	if(keyflags[escape]==1)
		{
		done=1;
	}
	if(keyflags[rt_arrow]==1)
		{
		for(y=Bat_Object.y;y<Bat_Object.y+Bat_Object.h;y++)
			{
			memset(&screen[(y*320)+Bat_Object.x],0,2);
		}
		Bat_Object.x+=2;
		if(Bat_Object.x>300) Bat_Object.x=299;
	}
	if(keyflags[left_arrow]==1)
		{
		for(y=Bat_Object.y;y<Bat_Object.y+Bat_Object.h;y++)
			{
			memset(&screen[(y*320)+Bat_Object.x]+Bat_Object.w-2,0,2);
		}
		Bat_Object.x-=2;
		if(Bat_Object.x<0) Bat_Object.x=0;
	}
	copybitmap(screen,bat,Bat_Object.w,Bat_Object.h,Bat_Object.x,Bat_Object.y);
}

void main(void)
{
	randomize();
	setgraphics();
	filebuf=new unsigned char[64000];    // Allocate memory
	O_brick=new unsigned char[171];
	P_brick=new unsigned char[171];
	C_brick=new unsigned char[171];
	G_brick=new unsigned char[171];
	R_brick=new unsigned char[171];
	Y_brick=new unsigned char[171];
	blank  =new unsigned char[171];
	ball   =new unsigned char[30];
	bat    =new unsigned char[143];

	memset(screen,0,64000);
	memset(blank,0,171);
	loadPCX("brick.pcx",filebuf,1);        // Load sprites
	for(y=0;y<=8;y++)                    // Grab all colours of brick
		{
		for(x=0;x<=18;x++)
			{
			O_brick[(y*18)+x]=filebuf[(y*320)+x];
			P_brick[(y*18)+x]=filebuf[(y*320)+x+18];
			C_brick[(y*18)+x]=filebuf[(y*320)+x+36];
			G_brick[(y*18)+x]=filebuf[(y*320)+x+54];
			R_brick[(y*18)+x]=filebuf[(y*320)+x+72];
			Y_brick[(y*18)+x]=filebuf[(y*320)+x+90];
		}
	}
	for(y=8;y<=12;y++)                        // Grab the ball
		{
		for(x=0;x<=5;x++)
			{
			ball[((y-8)*6)+x]=filebuf[(y*320)+x];
		}
	}
	for(y=13;y<=15;y++)                      // Grab the bat thingy
		{
		for(x=0;x<=20;x++)
			{
			bat[((y-13)*21)+x]=filebuf[(y*320)+x];
		}
	}
	delete filebuf;
									 // Allocate stars
	stars=new StarField[numstars];
	for(i=0;i<numstars-1;i++)                // Init stars.
		{
		stars[i].Init();
		screen[(stars[i].y*320)+stars[i].x]=stars[i].col;
	}

	brick=new Object[144];
	for(i=0;i<144;i++) brick[i].status=1;
	randomize();
	for(y=30,x=16,i=0;i<=144;i++,x+=18)
		{
		if(x>=295)
			{
			x=16;
			y+=8;
		}
		brick[i].x=x;
		brick[i].y=y;
		brick[i].w=18;
		brick[i].h=8;
		brick[i].flag=random(6)+1;
	}

	Bat_Object.x=150;
	Bat_Object.y=190;
	Bat_Object.w=21;
	Bat_Object.h=3;

	Ball_Object.x=157;
	Ball_Object.y=150;
	Ball_Object.w=6;
	Ball_Object.h=5;
	Ball_Object.starting=1;
	Ball_Object.xinc=0;
	Ball_Object.yinc=1;
	done=0;
	//oldint0x09=getvect(0x09);
	//setvect(0x09,Interrupt0x09);

	set_ikb();

	copybitmap(screen,bat,Bat_Object.w,Bat_Object.h,Bat_Object.x,Bat_Object.y);
	while(!done)
		{
          CheckKeys();
		for(i=0;i<144;i++)
			{
			if(brick[i].status==1 && hit==0)
				{
				if(Ball_Object.x+Ball_Object.w==brick[i].x &&
					Ball_Object.y+4>=brick[i].y && Ball_Object.y<=brick[i].y+brick[i].h)
					{
					Ball_Object.xinc*=-1;
					brick[i].status=0;
					copybitmap(screen,blank,18,8,brick[i].x,brick[i].y);
					hit=1;
				}
				else if(Ball_Object.x==brick[i].x+brick[i].w &&
					Ball_Object.y+4>=brick[i].y && Ball_Object.y<=brick[i].y+brick[i].h)
					{
					Ball_Object.xinc*=-1;
					brick[i].status=0;
					copybitmap(screen,blank,18,8,brick[i].x,brick[i].y);
					hit=1;
				}
				else if(Ball_Object.y==brick[i].y+brick[i].h &&
					Ball_Object.x+5>=brick[i].x && Ball_Object.x<=brick[i].x+brick[i].w)
					{
					Ball_Object.yinc*=-1;
					brick[i].status=0;
					copybitmap(screen,blank,18,8,brick[i].x,brick[i].y);
					hit=1;
				}
				else if(Ball_Object.y+Ball_Object.h==brick[i].y &&
					Ball_Object.x+5>=brick[i].x && Ball_Object.x<=brick[i].x+brick[i].w)
					{
					Ball_Object.yinc*=-1;
					brick[i].status=0;
					copybitmap(screen,blank,18,8,brick[i].x,brick[i].y);
					hit=1;
				}
			}
			if(brick[i].status==1)
				{
				if(brick[i].flag==ORANGE)
					copybitmap(screen,O_brick,18,8,brick[i].x,brick[i].y);
				else if(brick[i].flag==PURPLE)
					copybitmap(screen,P_brick,18,8,brick[i].x,brick[i].y);
				else if(brick[i].flag==CYAN)
					copybitmap(screen,C_brick,18,8,brick[i].x,brick[i].y);
				else if(brick[i].flag==GREEN)
					copybitmap(screen,G_brick,18,8,brick[i].x,brick[i].y);
				else if(brick[i].flag==RED)
					copybitmap(screen,R_brick,18,8,brick[i].x,brick[i].y);
				else if(brick[i].flag==YELLOW)
					copybitmap(screen,Y_brick,18,8,brick[i].x,brick[i].y);
			}
		}
		hit=0;
		Ball_Object.oldx=Ball_Object.x;
		Ball_Object.oldy=Ball_Object.y;
		Ball_Object.x+=Ball_Object.xinc;
		Ball_Object.y+=Ball_Object.yinc;
		if(Ball_Object.starting==1 &&
			(Ball_Object.y+Ball_Object.h)>=Bat_Object.y)
			{
			Ball_Object.starting=0;
			if(random(2)) Ball_Object.xinc=-1;
			else Ball_Object.xinc=1;
		}
		if(Ball_Object.starting==0)
			{
			if((Ball_Object.y+Ball_Object.h)==Bat_Object.y &&
				Ball_Object.x+Ball_Object.w>Bat_Object.x && Ball_Object.x<Bat_Object.x+Bat_Object.w)
				{
				Ball_Object.yinc*=-1;
			}
			if((Ball_Object.y+Ball_Object.h)==Bat_Object.y &&
				Ball_Object.x+Ball_Object.w-1==Bat_Object.x && Ball_Object.x==Bat_Object.x+Bat_Object.w+1)
				{
				Ball_Object.yinc*=-1;
				Ball_Object.xinc*=-1;
			}
			if(Ball_Object.y<=0) Ball_Object.yinc*=-1;
			if(Ball_Object.y>=194)
				done=1;
			if(Ball_Object.x<=0) Ball_Object.xinc*=-1;
			if(Ball_Object.x+Ball_Object.w>=319)Ball_Object.xinc*=-1;
		}

		copybitmap(screen,blank,Ball_Object.w,Ball_Object.h,
					Ball_Object.oldx,Ball_Object.oldy);
		copybitmap(screen,ball,Ball_Object.w,Ball_Object.h,
					Ball_Object.x,Ball_Object.y);

		StarRout();
	}
	delete stars;                  // Release mem
	delete brick;
	delete O_brick;
	delete P_brick;
	delete C_brick;
	delete G_brick;
	delete R_brick;
	delete Y_brick;
	delete blank;
	//setvect(0x09,oldint0x09);

	reset_ikb();

	settextmode();
}
