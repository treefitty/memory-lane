//=======================================================
//
// This file is responsible for making all constants,
// data type definitions and function definitions
// visible to every other file in the demo.
//
//=======================================================

#include <bios.h>
#include <math.h>
#include <dos.h>
#include <mem.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define NOPAL   0
#define LOADPAL 1
#define YES 1
#define NO  0

#define MAXVERTS 	435            // Maximum vertices an object can have
#define MAXPOLYVERTS 6             // Maximum vertices a polygon can have
#define MAXPOLYS 	500            // Maximum polygons an object can have
#define MAXOBJECTS  1 			// Maximum number of objects
#define PI 		3.141592
#define SCRHEIGHT 	200
#define SCRWIDTH    320
#define STARTOFS    CLIP_Y_TOP*320
#define SCRBYTES	(CLIP_Y_BOTTOM-CLIP_Y_TOP)*320
#define ABS(x)		(x<0) ? -x : x
#define SGN(x)		(x<0) ? -1 : 1

//============= Key Scan Codes ==============
#define escape		0x01
#define comma		0x33
#define stop		0x34
#define bigPlus	0x4e
#define bigMinus	0x4a
#define smallPlus	0x0d
#define smallMinus  0x0c
#define pause		0x19

//============== Shading Model Types =================
#define WIREFRAME   0			// Regular default single colour polygons
#define LAMBERT     1    		// Lambert or Diffuse shading model
#define GOURAUD	2              // Smoothed plane Gouraud shading

//=========== Define Clipping Boundarys ============
#define CLIP_X_LEFT		0
#define CLIP_X_RIGHT	320
#define CLIP_Y_TOP		38
#define CLIP_Y_BOTTOM	188

//============== Colour Shading Setup ===============
#define RED     0
#define GREEN   38
#define BLUE    76
#define MAGENTA 114
#define YELLOW  152
#define CYAN    190

//============= Fixed Point Maths Declarations ===============
//
// I have used a method common to graphics programming
// known as 16.16 fixed point mathematics. It elminates
// the need for floating point numbers increasing code
// speed by up to a 100 times! while still providing
// remarkable accuracy (to 16 places to be exact!)
//
typedef long Fixedpoint;

#define Int2Fixed(a)  (Fixedpoint)((Fixedpoint)(a) << 16)
#define Fixed2Int(a)  (int)((a) >> 16)
#define Float2Fixed(a)  ((Fixedpoint)((a) * 65536.0))
#define Fixed2Float(a)  ((a) / 65536.0)

extern "C" {

	Fixedpoint FixedMul(Fixedpoint,Fixedpoint);
	Fixedpoint FixedDiv(Fixedpoint,Fixedpoint);
};

//============= Object Type Definitions =================

typedef struct
{
	Fixedpoint Wx,Wy,Wz;
} WorldPoint;

typedef struct				    	// Object type definitions
{
	Fixedpoint Nx,Ny,Nz;	    	// Approx to normal to vert(Gouraud shading)
	Fixedpoint Nwx,Nwy,Nwz;	    	// Normals in world space!
	Fixedpoint Ox,Oy,Oz;         	// Object coords of vertex
	int sx,sy,sz;                	// Screen coords of vertex
} VertexType;

typedef struct
{
	Fixedpoint Nx,Ny,Nz;	    	// Normal to plane (Lambert shading)
	Fixedpoint Nwx,Nwy,Nwz;	    	// Plane normals in world space
	char shadedCol;              	// Shaded colour, (Lambert shading)
	int numVerts;			    	// Number of verts to polygon
	int centerZ;                 	// Approx Z center of polygon (for sorting)
	int Verts[MAXPOLYVERTS];     	// List of object verts that make polygon
} PolygonType;

typedef struct
{
	char colOfs;			    	// Offset to colour shade eg. shaded green etc
	int numVerts;                	// Number of vertices of object
	int numPolys;                	// Number of polygons in object
	int xAn,yAn,zAn;		    	// x,y,z rotation angles
	int sortedPolygonList[MAXPOLYS];	// Sorted polygon list
	int numSortedPolys;			// Number of sorted polygons
	int xinc,yinc,zinc;		    	// Rotation angle increments
	Fixedpoint tolerance; 	    	// Depth that object should hold
	Fixedpoint zoom;		    	// Z Step to be taken when zooming in
	Fixedpoint Owx,Owy,Owz;      	// World coords of object
	VertexType *Verts;  		// Vertex definitions for object
	PolygonType *Polys; 		// Polygon definitions for object
} convexType;

//
// All Global function definitions can be found here
//
void xFormPoints(convexType *object);
void rotateNormals(convexType *object);
void PolygonLine(int x1,int y1,int x2,int y2);
void Line(int x1,int y1,int x2,int y2,char col);
void DrawConvex(convexType *object,int polyNum);
int  compPolys(const void *a,const void *b);
void findVisiblePolys(convexType *object);
void calcNormals(convexType *object);
void drawObject(convexType *object);
void Init3D();

int  loadPCX(char far *filename,unsigned char far *filebuffer,int Pal);
void loadPCXpal(char far *filename,char *palette);

extern "C" {

	void set_ikb(void);
	void reset_ikb(void);
	void PutChar(int x,int y,unsigned char ch,unsigned char attr);
	void ChangeAttr(int x,int y,unsigned char attr);
	void GetChar(int x,int y);
	void putTextScreen(char far *text,int ofs);
	void putbuffer(unsigned char far *screenbuf);
	void setpalette(char *Array);
	void setColour(char col,char r,char g,char b);
	void drawHLine(char far *dest,int ofs,char col,char length);

};

//
// Variables that have been defined elsewhere but are
// made truely global here.
//
extern char shadingModel;
extern char far *VirtScr;
extern char currObj;
extern char keyflags[128];
extern VertexType LightSource;
extern convexType userObject;