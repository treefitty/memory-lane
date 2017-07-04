/********************************************************************/
/*                                                                  */
/*                3D Lambert, Grouraud Shading Demo                 */
/*                ---------------------------------                 */
/*                                                                  */
/*   This code should be subject to copyright (c) 1995 Paul Carroll */
/*		It should also be deemed a breach of copyright            */
/*	  to reproduce any of this code in any form without the        */
/*		author's (Paul Carroll) expressed premission.             */
/*                                                                  */
/*                                                                  */
/*        Copyright (c) 1995-97 Black Marble Productions            */
/*                                                                  */
/********************************************************************/

#include "global.h"
#include "objects.h"
#include "menu.h"

//============== Global Definitions ===============
char keyflags[128];                     // Keyflags for 3d interactive loop
char shadingModel;               		// Shading model being used (ie Lambert)
char far *VirtScr;					// Pointer to virtual screen
int sort_array[5];
int sorted_array[5];
char currObj;						// Object being rendered
convexType userObject;
VertexType LightSource;                 // Light source origin !(0,0,0)

//=============== Local Definitions ================
#define UpArrow     	0x4800
#define DownArrow   	0x5000
#define PageDown    	0x5100
#define PageUp      	0x4900
#define Escape  		0x011b
#define Enter			0x1c0d
#define MAX_DISP_LIST    10
#define NUM_OBJECTS		11
#define CURSOR_WIDTH	37

static const unsigned long *timer=(unsigned long *)0x46c;// Point to system timer
static unsigned long startTime,stopTime;// Used to measure fps
static int cursorX=15,cursorY=11;
static int objectAtTop=0;
static char *menuList[NUM_OBJECTS]={

	"CUBE           ",
	"PYRAMID        ",
	"24 SIDED OBJECT",
	"ROOK           ",
	"THING          ",
	"EPCOT1         ",
	"THING2         ",
	"SOCCER BALL    ",
	"STICKS         ",
	"BANANA         ",
	"MUSHROOM       ",

};

//================== Start Code ==================
//
// Updates any convexType object
//
void updateObjects(convexType *object)
{
	if(shadingModel==LAMBERT || shadingModel==WIREFRAME)
		rotateNormals(object);

	xFormPoints(object);
	drawObject(object);

	if((object->xAn+=object->xinc)>=360) object->xAn=0;
	if((object->yAn+=object->yinc)>=360) object->yAn=0;
	if((object->zAn+=object->zinc)>=360) object->zAn=0;
	if(object->Owz<=object->tolerance) object->Owz+=object->zoom;
}

int comp_points(const void *a,const void *b)
{
	if(sort_array[*(int*)a]>sort_array[*(int*)b])
		{
		return sort_array[*(int*)a];
	}
	else
		{
		return sort_array[*(int*)b];
	}
}

//
// Loads an object from a specified array. Could be easily adapted
// to load from a file.
//
void loadObject(float *array,convexType *object,char conv,float mag)
{
	int k,i,l;

	object->numVerts=array[0];
	object->numPolys=array[1];

	object->Verts=new VertexType[object->numVerts+1];
	object->Polys=new PolygonType[object->numPolys+1];

	for(i=2,k=0;k<object->numVerts;k++,i+=3)         // Read vertices
		{
		object->Verts[k].Ox=Float2Fixed(mag*array[i]);
		object->Verts[k].Oy=Float2Fixed(mag*array[i+1]);
		object->Verts[k].Oz=Float2Fixed(mag*array[i+2]);
	}
	for(k=0,i=(object->numVerts*3)+2;k<object->numPolys;k++)
		{                                           // Setup polygons
		object->Polys[k].numVerts=array[i];
		for(l=0,i++;l<object->Polys[k].numVerts;l++,i++)
			{
			if(conv)
				object->Polys[k].Verts[l]=array[i]-1;
			else object->Polys[k].Verts[l]=array[i];

			sort_array[l]=object->Polys[k].Verts[l];
		}
		sort_array[0]=4;
		sort_array[1]=3;
		sort_array[2]=1;
		sort_array[3]=2;
		qsort((void*)sorted_array,4,sizeof(sorted_array[0]),comp_points);

		delay(0);
	}
	object->xAn=0;                     // Set rotation angles to 0
	object->yAn=0;
	object->zAn=0;

	calcNormals(object); 		 	// Pre-calc normals to planes
								// for use in Lambert shading
}

void doDemo(void)
{
	char lastkey=0;
	long frames=0;
	char done,k;
	int i;

	asm mov	ax,0x13                       // Linear mode 13h
	asm int	0x10

	userObject.xinc=1;
	userObject.yinc=1;
	userObject.zinc=1;
	userObject.tolerance=Int2Fixed(-25);
	userObject.zoom=Int2Fixed(10);
	userObject.Owx=0;
	userObject.Owy=0;
	userObject.Owz=Int2Fixed(-25);
	userObject.colOfs=CYAN;

	switch(currObj)
		{
		case 0 : loadObject(Cube,&userObject,0,.1); break;
		case 1 : loadObject(Pyramid,&userObject,0,.1); break;
		case 2 : loadObject(_24_Sided,&userObject,0,.2); break;
		case 3 : loadObject(Rook,&userObject,1,7); break;
		case 4 : loadObject(Thing,&userObject,1,2); break;
		case 5 : loadObject(Epcot1,&userObject,1,3); break;
		case 6 : loadObject(Thing2,&userObject,1,2); break;
		case 7 : loadObject(SoccerBall,&userObject,1,3); break;
		case 8 : loadObject(Sticks,&userObject,1,7); break;
		case 9 : loadObject(Banana,&userObject,1,7); break;
		case 10: loadObject(Mushroom,&userObject,1,7); break;
	}

	LightSource.Ox=Int2Fixed(10);
	LightSource.Oy=Int2Fixed(10);
	LightSource.Oz=Int2Fixed(10);

	loadPCX("3D_BKGR.PCX",VirtScr,LOADPAL); // Load background PCX picture

	Init3D();

	shadingModel=LAMBERT;		       // Default shading to start on
	setColour(0,9,4,12);                  // Set purple bg
	for(k=15,i=1;i<38;i++,k++)      // Define colour ranges for shading
		{
		setColour(i+RED    ,k,0,0);
		setColour(i+GREEN  ,0,k,0);
		setColour(i+BLUE   ,0,0,k);
		setColour(i+MAGENTA,k,0,k);
		setColour(i+YELLOW ,k,k,0);
		setColour(i+CYAN   ,0,k,k);
	}

	set_ikb();
	memset(keyflags,0,128);
	startTime=*timer;                     // Get start time for fps rating
	done=0;
	while(!done)
		{
		memset(VirtScr+STARTOFS,0,SCRBYTES);

		updateObjects(&userObject);

		if(keyflags[lastkey]==0)              // Check for any user
			{                                // keypresses. Don't check
			if(keyflags[escape]==1)          // if the last key has not
				{                           // been released yet!
				done=1;
			}
			else if(keyflags[comma])
				{
				if(shadingModel==LAMBERT) shadingModel=WIREFRAME;
				else shadingModel=LAMBERT;
				lastkey=comma;
			}
			else if(keyflags[stop])
				{
				if(shadingModel==LAMBERT) shadingModel=WIREFRAME;
				else shadingModel=LAMBERT;
				lastkey=stop;
			}
			else if(keyflags[bigPlus] || keyflags[smallPlus])
				{
				if(userObject.colOfs<CYAN)
					userObject.colOfs+=38;
				else userObject.colOfs=0;

				if(keyflags[bigPlus]==0) lastkey=smallPlus;
				else lastkey=bigPlus;
			}
			else if(keyflags[bigMinus] || keyflags[smallMinus])
				{
				if(userObject.colOfs>RED)
					userObject.colOfs-=38;
				else userObject.colOfs=CYAN;

				if(keyflags[bigMinus]==0) lastkey=smallMinus;
				else lastkey=bigMinus;
			}
			else if(keyflags[pause])
				{
				while(keyflags[pause]);
			}
			else lastkey=0;
		}
		frames++;

		putbuffer(VirtScr);	// Using this routine instead of memcpy saves
						// about 8 frames per second on my pc
	}
	stopTime=*timer;                     // Get end time for fps rating
	reset_ikb();

	delete userObject.Polys;
	delete userObject.Verts;

	asm mov	ax,0x03
	asm int	0x10
}

void PutCursor(void)
{
	int x;

	PutChar(cursorX,cursorY,'¯',0x4B);
	PutChar(cursorX+CURSOR_WIDTH+1,cursorY,'®',0x4B);
	for(x=cursorX;x<CURSOR_WIDTH+cursorX+1;x++)
		{
		ChangeAttr(x,cursorY,0x4B);
	}
}

void EraseCursor(void)
{
	int x;

	PutChar(cursorX,cursorY,' ',0x0B);
	PutChar(cursorX+CURSOR_WIDTH+1,cursorY,' ',0x0B);
	for(x=cursorX;x<=CURSOR_WIDTH+cursorX;x++)
		{
		ChangeAttr(x,cursorY,0x0B);
	}
}

void UpdateScreen()
{
	int i;

	textattr(0x0B);
	for(i=0;i<=MAX_DISP_LIST;i++)
		{
		gotoxy(18,12+i);
		cprintf(menuList[objectAtTop+i]);
	}
	PutCursor();
}

//=================== Main =====================

void main(void)
{
	int key;
	char done;

	VirtScr=new char[64000];           // Allocate virtual screen

	_setcursortype(_NOCURSOR);
	setColour(4,14,14,14);
	putTextScreen(MENU_DATA,0);
	PutCursor();
	UpdateScreen();

	currObj=0;
	done=0;
	while(!done)
		{
		if(_bios_keybrd(_KEYBRD_READY)!=0)
			{
			key=_bios_keybrd(_KEYBRD_READ);

			switch(key)
				{
				case Escape :
					{
					done=1;
					break;
				}
				case UpArrow :
					{
					if(cursorY>11)
						{
						EraseCursor();
						currObj--;
						cursorY--;
						UpdateScreen();
					}
					else {
						if(objectAtTop>0)
							{
							currObj--;
							objectAtTop--;
							UpdateScreen();
						}
					}

					break;
				}
				case DownArrow :
					{
					if(cursorY<11+MAX_DISP_LIST && (currObj+(cursorY-19))<NUM_OBJECTS)
						{
						EraseCursor();
						cursorY++;
						currObj++;
						UpdateScreen();
					}
					else {
						if((objectAtTop+MAX_DISP_LIST)<NUM_OBJECTS-1)
							{
							currObj++;
							objectAtTop++;
							UpdateScreen();
						}
					}

					break;
				}
				case Enter :
					{
					doDemo();

					_setcursortype(_NOCURSOR);
					setColour(4,14,14,14);
					putTextScreen(MENU_DATA,0);
					PutCursor();
					UpdateScreen();
				}
			}
		}
	}
	delete VirtScr;

	asm mov	ax,0x03
	asm int	0x10
}










