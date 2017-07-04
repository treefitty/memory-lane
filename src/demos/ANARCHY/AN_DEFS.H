// *                                                         *//
// *   Global definitions file that contains information     *//
// *   about every global variable and function definitions  *//
// *   for functions needed to be used in multiple modules   *//
// *                                                         *//
// *                (c) Black Marble 1995                    *//

#include <stdlib.h>
#include <conio.h>
#include <mem.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <alloc.h>
#include <bios.h>

#include <ipcx.h>

#include "xlib_all.h"

//
// Error code macros
//
#define NOMEM        0
#define BADFILE      1

//
// Enhanced scan codes only use with
// game key interrupt handler
//
#define rightArrow 	 0x4d
#define leftArrow  	 0x4b
#define upArrow      0x48
#define downArrow    0x50
#define esc		 0x01

//
// BIOS key return codes
//

#define b_enter       0x1c0d
#define b_upArrow     0x4800
#define b_downArrow   0x5000
#define b_escape      0x11b

//
// Defn's for map and game screen etc
//

#define MAXOBJECTS   100
#define MAPWIDTH     24
#define MAPHEIGHT    15
#define SCREENWIDTH  18
#define SCREENHEIGHT 10

//
// Type defs for all objects
//
typedef struct playerType
{
	int x,y;
	int mapx,mapy;
	char moving;
	char frame;
	char delay;
	char far *data[4];
	signed char steps;
	signed char xstep,ystep;

};

typedef struct screenType
{
	int x,y;
	int mapx,mapy;
	char moving;
	signed char steps;
	signed char xstep,ystep;

};

typedef struct FontType
{
	char Defined;
	char Width;
	char Height;

};

//
// Global variable definitions
//
extern playerType player;
extern screenType screen;
extern char keyflags[128];
extern char far *StaticObjects[MAXOBJECTS];
extern FILE *FontFile;
extern FontType myFonts[']'-' '];
extern char far *FontDat[']'-' '];
//
// Low level graphics and sprite routines
//
extern "C"
{
	void set_ikb(void);
	void reset_ikb(void);
	void set_palette(char *Array);
}

//
// Error handling functions
//
void TerminalError(char code,char *filename);

//
// Game (AI, updating etc)
//
void GameLoop(void);

//
// Data related functions (file loading, memory allocation etc)
//
void LoadSprites(void);
void ReleaseSprites(void);
void LoadFont(char *Filename);



