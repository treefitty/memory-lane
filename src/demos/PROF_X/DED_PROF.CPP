#include <stdio.h>
#include <dos.h>
#include <mem.h>
#include <bios.h>
#include "sprite.h"
#include "pcx.h"
#include "screen.h"

#define PROF_RIGHT		0
#define PROF_LEFT		1
#define SHOOT			2

/* Hex return codes for extended keys */
const int escape= 0x11b;
const int rt_arrow= 0x4d00;
const int left_arrow= 0x4b00;
const int up_arrow= 0x4800;
const int down_arrow= 0x5000;
const int space= 0x3920;

const int NUM_SPRITES=9;
const int SPRITE_WIDTH=24;
const int SPRITE_HEIGHT=24;
unsigned char far *screenbuf;
unsigned char far *gamebuf;
int prof_r_walk[]= { 0,1,2,3 }; // Sequence of images for animation
int prof_l_walk[]= { 4,5,6,7 };
int xpos;
int alien_die;

class ACTOR {

//private:


public:

	ACTOR(int NUM_SPRITES,int SPRITE_WIDTH,int SPRITE_HEIGHT,
			int xstart,int ystart);
	int sprites;
	int sprite_width;
	int sprite_height;
	int facing;
	int xpos;
	int ypos;

};

ACTOR::ACTOR(int NUM_SPRITES,int SPRITE_WIDTH,int SPRITE_HEIGHT,
			int xstart,int ystart)
{
	xpos= xstart; ypos= ystart;
	facing= 0;                  // Make actor face right.
	sprites= NUM_SPRITES; sprite_width= SPRITE_WIDTH;
	sprite_height= SPRITE_HEIGHT;
}

ACTOR prof_actor(NUM_SPRITES,SPRITE_WIDTH,SPRITE_HEIGHT,100,133);

Sprite profsprite(NUM_SPRITES,SPRITE_WIDTH,SPRITE_HEIGHT);
Sprite flame(NUM_SPRITES,SPRITE_WIDTH,SPRITE_HEIGHT);
Sprite alien(NUM_SPRITES,SPRITE_WIDTH,SPRITE_HEIGHT);
Pcx profbit,alienbit,introbg,gamebg;
pcx_struct pcx,bg,al_pcx;

void quit_pgm(void);
void putbuffer(unsigned char far *screenbuf,int y1,int height);
void handle_event(int keypress);
void clear_buffer(void);
void update_actors(int action);
void load_files(void);
void grab_sprites(void);

void quit_pgm(void)
{
	delete gamebuf;
	__asm {
		mov ax, 	0x03;         /* Back in text mode */
		int 	   	0x10;
		mov ax,	0x4c00;       /* DOS terminate program function */
		int		0x21;
		}
}

void clear_buffer(void)
{
	int key;

	if(_bios_keybrd(_KEYBRD_READY)!=0)
		{
		do {
			key= _bios_keybrd(_KEYBRD_READ);
			if(key== escape) break;
		} while(_bios_keybrd(_KEYBRD_READY)!=0);
	}
}

void handle_event(int keypress)
{
	switch(keypress)
		{
		case rt_arrow: {
					update_actors(PROF_RIGHT);
					clear_buffer();
					prof_actor.xpos+=12;
					alien.put(0,170,136,gamebuf);
					profsprite.put(prof_r_walk[0],prof_actor.xpos,
							133,gamebuf);
					putbuffer(gamebuf,133,SPRITE_HEIGHT);
					profsprite.erase(gamebuf);
					alien.erase(gamebuf);
					prof_actor.facing= PROF_RIGHT;
					break;
					}
		case left_arrow: {
					update_actors(PROF_LEFT);
					clear_buffer();
					prof_actor.xpos-=12;
					alien.put(0,170,136,gamebuf);
					profsprite.put(prof_l_walk[0],prof_actor.xpos,
							133,gamebuf);
					putbuffer(gamebuf,133,SPRITE_HEIGHT);
					profsprite.erase(gamebuf);
					alien.erase(gamebuf);
					prof_actor.facing= PROF_LEFT;
					break;
					}
		case space:	{
					update_actors(SHOOT);
					clear_buffer();
					break;
					}
		case escape: { quit_pgm(); break; }
	}
}

void update_actors(int action)
{
	int pos;

	if(action== PROF_RIGHT)
		{
		for(int i=0,xpos=0 ;i<4;i++,xpos+=2)
			{
			alien.put(0,170,136,gamebuf);
			profsprite.put(prof_r_walk[i],prof_actor.xpos+i+xpos,
						133,gamebuf);
			putbuffer(gamebuf,133,SPRITE_HEIGHT);
			delay(80);
			alien.erase(gamebuf);
			profsprite.erase(gamebuf);         // Erase image
			}
		}
	if(action== PROF_LEFT)
		{
		for(int i=0,xpos=0 ;i<4;i++,xpos-=2)
			{
			alien.put(0,170,136,gamebuf);
			profsprite.put(prof_l_walk[i],prof_actor.xpos-i+xpos,
						133,gamebuf);
			putbuffer(gamebuf,133,SPRITE_HEIGHT);
			delay(80);
			alien.erase(gamebuf);
			profsprite.erase(gamebuf);         // Erase image
			}
		}
	if(action== SHOOT)
		{
		if(prof_actor.facing== PROF_RIGHT) pos= 0;
		else pos= 4;
		for(int i=0;i<=2;i++)
			{
			alien.put(0,170,136,gamebuf);
			profsprite.put(pos,prof_actor.xpos,133,gamebuf);
			if(prof_actor.facing== PROF_RIGHT)
				flame.put(0,prof_actor.xpos+20,133,gamebuf);

			else flame.put(1,prof_actor.xpos-21,133,gamebuf);

			if(prof_actor.facing== PROF_RIGHT) alien_die= 1;
			putbuffer(gamebuf,133,SPRITE_HEIGHT);
			profsprite.erase(gamebuf);
			flame.erase(gamebuf);
			delay(30);
			profsprite.put(pos,prof_actor.xpos,133,gamebuf);
			putbuffer(gamebuf,133,SPRITE_HEIGHT);
			profsprite.erase(gamebuf);
			alien.erase(gamebuf);
			delay(40);
			}
		}
	if(alien_die== 1)
		{
		for(int i=1; i<4; i++)
			{
			profsprite.put(pos,prof_actor.xpos,133,gamebuf);
			alien.put(i,170+2*i,133,gamebuf);
			putbuffer(gamebuf,133,24);
			delay(90);
			alien.erase(gamebuf);
			profsprite.erase(gamebuf);
			}
		alien_die= 0;
		}
}

void load_files(void)
{
	if(profbit.load("profxint.pcx",&pcx))   goto ERROR;
	if(introbg.load("prof_an2.pcx",&pcx))    goto ERROR;
	setpalette(pcx.palette,0,256);
	screenbuf=new unsigned char[64000];    // Create offscreen
									 //  video buffer
	memmove(screenbuf,pcx.image,64000);	      // Move background
									 //  image into
									 //  buffer
	putbuffer(screenbuf);    // Move offscreen
									 //  buffer to
									 //  vidram
	while(_bios_keybrd(_KEYBRD_READY)==0);
	_bios_keybrd(_KEYBRD_READ);
	if(gamebg.load("screen1.pcx",&bg))      goto ERROR;
	if(alienbit.load("alien1.pcx",&al_pcx)) goto ERROR;

	goto END;

	ERROR:

	__asm {
		mov ax,	0x03;
		int		0x10;
	}
	puts("Error - Cannot load data file!!\n\n");
		puts("Program Aborted.");
		__asm {
			mov ax,	0x4c00;   /* DOS terminate program function call */
			int		0x21;
			}
	END:
}

void grab_sprites(void)
{
	/* Grab sprites for green alien */
	for(int i=1; i<4; i++) alien.grab(al_pcx.image,i,i*SPRITE_WIDTH,26);
	alien.grab(al_pcx.image,0,0,0);

	/* Grab sprites for Professor */
	for(i=0; i<8; i++) profsprite.grab(pcx.image,i,i*SPRITE_WIDTH,0);
	flame.grab(pcx.image,0,0,24);   // Grab Flame Sprites.
	flame.grab(pcx.image,1,24,24);
}

void main(void)
{
	__asm {
		mov ax,	0x13;
		int		0x10;
	}
	load_files();
	grab_sprites();
	setpalette(pcx.palette,0,256);

	delete screenbuf;
	prof_actor.xpos= 100;
	gamebuf= new unsigned char[64000];
	memmove(gamebuf,bg.image,64000);
	putbuffer(gamebuf,0,SCREEN_HEIGHT);

	profsprite.put(prof_r_walk[0],100,233,gamebuf);
	alien.put(0,170,136,gamebuf);
	profsprite.erase(gamebuf);
	alien.erase(gamebuf);

	for(;;)
		{
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			int key= _bios_keybrd(_KEYBRD_READ);
			handle_event(key);
			}
		}
}


