#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <igraph.h>
#include <ipcx.h>

#define getx(x) x=(random(320))
#define gety(y) y=(random(180))

unsigned char far *buffer;

class star
{
public:
	int pixel;
	int waited;
	int x, y, xIncrement, yIncrement, size;
	void initialize(void);
	void advance(void);
	star(void) { initialize();};
};

void star::advance(void)
{
	waited++;
	if(waited<=2) goto end;
	xIncrement = (x - 150)/10;
	yIncrement = (y-100)/10;
	if(yIncrement==0 && xIncrement==0) initialize();
	x+=xIncrement;
	y+=yIncrement;
	size++;
	if ((x < 0) || (x > 320)) initialize();
	if ((y < 0) || (y > 200)) initialize();
	waited=0;
	end:
};

void star::initialize(void)
{
	waited=0;
	getx(x);
	gety(y);
	size=5;
	if((x-150)/10==0 && (y-100)/10==0) initialize();
}

void main(void)
{
	setgraphics();
	buffer= new unsigned char[64000];
	loadPCX("ikon.pcx",buffer);
	for(long k=0;k<64000;k++) buffer[k]=0;
	star mystar[100];
	while(!kbhit())
		{
		for(int count=0;count<100;++count)
			{
			mystar[count].advance();
			mystar[count].pixel=buffer[(mystar[count].y*320)+mystar[count].x];
			if(!mystar[count].pixel) buffer[(mystar[count].y*320)+mystar[count].x]=22;
		}
		putbuffer(buffer);
		for(count=0;count<100;++count)
			{
			if(!mystar[count].pixel)buffer[(mystar[count].y*320)+mystar[count].x]=0;
		}
	}
	getch();
	delete buffer;
	settextmode();
}
