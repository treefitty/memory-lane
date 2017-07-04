#include <stdio.h>
#include <alloc.h>
#include <mem.h>

#include <conio.h>
#include <sprite.h>
#include <bitmap.h>
#include <igraph.h>

#define SCREEN_WIDTH 320

Sprite::Sprite(int w,int h)
{
	width=w;
	height=h;
	image=new unsigned char[width*height];
	savebuffer=new unsigned char[width*height];
	memset(savebuffer,0,width*height);
}

void Sprite::grab(unsigned char far *buffer,int x1,int y1)
{
	if((x1+width>SCREEN_WIDTH)||(y1+height>SCREEN_WIDTH)) return;

	getbitmap(image,buffer,width,height,x1,y1);
}

void Sprite::putscaled(int newWidth,int newHeight,
				unsigned char far *dest_bitmap,int x,int y)
{
	unsigned char *PtrScreen, *Screen;
	float XRatio, YRatio, X, Y;
	int Xc, Yc;
	int *XPoints, *YPoints, Offset;

	oldHeight=height;
	oldWidth=width;
	delete savebuffer;
	savebuffer=new unsigned char[newWidth*newHeight];
	getbitmap(savebuffer,dest_bitmap,newWidth,newHeight,x,y);

	Screen=(unsigned char *)malloc(newWidth*newHeight);

	// Compute the scaling ratios
	XRatio = (float) ( (float)width / (float)newWidth );

	YRatio = (float) ( (float)height / (float)newHeight );

	//alloc the look up table
	XPoints = new int [newWidth * sizeof(int)];

	for(Xc=0, X = 0; Xc< newWidth ;X+=XRatio,Xc++)
		{
		XPoints[Xc] = X;
	}
	//same for the y axis
	YPoints = new int [newHeight * sizeof(int)];

	for(Yc=0, Y = 0; Yc< newHeight ;Y+=YRatio,Yc++)
		{
		YPoints[Yc] = Y;
	}
	//use a pointer to run through the array
	PtrScreen = Screen;

	//This is it, actually scales the bitmap using the look up table
	//to go from the dest to the source...
	for( Yc = 0 ; Yc < newHeight ; Yc++ )
		{
		int test= YPoints[Yc];
		Offset = test * width;
		for( Xc = 0 ; Xc < newWidth ; Xc++)
			{
			*PtrScreen++ = image[Offset + XPoints[Xc]];
		}
	}
	tc_putbitmap(dest_bitmap,Screen,newWidth,newHeight,x,y);
	//free up the allocated memory
	free(Screen);
	delete YPoints;
	delete XPoints;
	xsprite=x;
	ysprite=y;
	width=newWidth;
	height=newHeight;
}

void Sprite::put(int x,int y,unsigned char far *screen)
{
	int vact;
	unsigned char far *tempSprite;

	xsprite = x;
	ysprite = y;

	if((x+width)<320)
		{
		getbitmap(savebuffer,screen,width,height,x,y);
		tc_putbitmap(screen,image,width,height,x,y);
	}
	if((x+width)>320)
		{
		vact=320-x;
		tempSprite=new unsigned char[vact*height];
		for(int k=0;k<height;k++)
			for(int u=0;u<vact;u++)
				tempSprite[(vact*k)+u]=image[(width*k)+u];

		getbitmap(savebuffer,screen,width,height,x,y);
		tc_putbitmap(screen,tempSprite,vact,height,x,y);
		delete tempSprite;
	}
}                                       

void Sprite::erase(unsigned char far *screen)
{
	copybitmap(screen,savebuffer,width,height,xsprite,ysprite);
}
