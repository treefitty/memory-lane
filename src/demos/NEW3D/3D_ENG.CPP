/********************************************************************/
/*                                                                  */
/*             3D Lambert, Grouraud Rendering Engine                */
/*             -------------------------------------                */
/*                                                                  */
/*   This code should be subject to copyright (c) 1995 Paul Carroll */
/*		It should also be deemed a breach of copyright            */
/*	  to reproduce any of this code in any form without the        */
/*		author's (Paul Carroll) expressed premission.             */
/*                                                                  */
/*                                                                  */
/*	        Copyright (c) 1995 Black Marble Productions            */
/*                                                                  */
/********************************************************************/

#include "global.h"

//================== Define Local Variables ==============
static Fixedpoint Sin[360],Cos[360];    // Sin, Cos lookup tables
static VertexType v0[MAXVERTS];         // Used in polygon drawing for fast lookup
static signed int StartX[SCRHEIGHT];	// Used for polygon
static signed int EndX[SCRHEIGHT];      // drawing routines

//===================== Globals =======================
int *z_buffer;

//
// Rotates all object points of a given object and transforms them
// to their corresponding screen coordinates.
//
void xFormPoints(convexType *object)
{
	Fixedpoint x,y,z,xo,yo,zo;
	Fixedpoint cosx,sinx,cosy,siny,cosz,sinz;
	VertexType *myVertex;
	int i;

	cosx=Cos[object->xAn];            // Faster to use register variables
	cosy=Cos[object->yAn];            // in SIN/COS lookup
	cosz=Cos[object->zAn];
	sinx=Sin[object->xAn];
	siny=Sin[object->yAn];
	sinz=Sin[object->zAn];

	for(i=0;i<object->numVerts;i++)
		{
		myVertex= &object->Verts[i];

		x=myVertex->Ox;
		y=myVertex->Oy;
		z=myVertex->Oz;

		xo=FixedMul(cosz,x) + FixedMul(sinz,y);  // Rotation about Z
		yo=FixedMul(cosz,y) - FixedMul(sinz,x);

		x =FixedMul(cosy,xo)- FixedMul(siny,z);  // Rotation about Y
		zo=FixedMul(siny,xo)+ FixedMul(cosy,z);

		z =FixedMul(cosx,zo)- FixedMul(sinx,yo); // Rotation about X
		y =FixedMul(sinx,zo)+ FixedMul(cosx,yo);
										 // Project to screen
		myVertex->sx=Fixed2Int(FixedMul(FixedDiv(x+object->Owx,z+object->Owz),Int2Fixed(256))) + 160;
		myVertex->sy=Fixed2Int(FixedMul(FixedDiv(y+object->Owy,z+object->Owz),Int2Fixed(256))) + 100;
		myVertex->sz=Fixed2Int(z+object->Owz);
	}
}

//
// Rotate all normals - speaks for itself!
//
void rotateNormals(convexType *object)
{                                      // Transform vert of object by
	Fixedpoint x,y,z,xo,yo,zo;                        // multiplying by rotation matrix
	Fixedpoint cosx,sinx,cosy,siny,cosz,sinz;
	PolygonType *poly;
	int i;

	cosx=Cos[object->xAn];
	cosy=Cos[object->yAn];
	cosz=Cos[object->zAn];
	sinx=Sin[object->xAn];
	siny=Sin[object->yAn];
	sinz=Sin[object->zAn];

	for(i=0;i<object->numPolys;i++)
		{
		poly= &object->Polys[i];

		x=poly->Nx;
		y=poly->Ny;
		z=poly->Nz;

		xo=FixedMul(cosz,x) + FixedMul(sinz,y);   // Rotation about Z
		yo=FixedMul(cosz,y) - FixedMul(sinz,x);

		x =FixedMul(cosy,xo)- FixedMul(siny,z);   // Rotation about Y
		zo=FixedMul(siny,xo)+ FixedMul(cosy,z);

		z =FixedMul(cosx,zo)- FixedMul(sinx,yo);  // Rotation about X
		y =FixedMul(sinx,zo)+ FixedMul(cosx,yo);

		poly->Nwx=x;
		poly->Nwy=y;
		poly->Nwz=z;
	}
}

//
// Used by DrawConvex to trace a line between two vertices.
// X is always the major axis (for speed purposes)
//
void PolygonLine(int x1,int y1,int x2,int y2)
{
	Fixedpoint x,m,tempX;
	int tmp,y;

	if(y2!=y1)
		{
		if(y2<y1)
			{
			tmp=y1;
			y1 =y2;
			y2 =tmp;

			tmp=x1;
			x1 =x2;
			x2 =tmp;
		}
		x =(long)x1<<8;
		m =((long)(x2 - x1)<<8) / ((long)(y2 - y1));
		x += m;
		y1++;

		for(y=y1;y<=y2;y++) /* Go through each row */
			{
			if((y>=CLIP_Y_TOP)&(y<CLIP_Y_BOTTOM)) /* If the coordinate is on the screen */
				{
				tempX=x>>8;
				if(StartX[y]==32767) /* Store the first coordinate */
					{
					if(tempX<CLIP_X_LEFT) StartX[y]=CLIP_X_LEFT;
					else if(tempX>CLIP_X_RIGHT) StartX[y]=CLIP_X_RIGHT;
					else StartX[y]=tempX;
				}
				else
					{
					if(tempX<CLIP_X_LEFT) EndX[y]=CLIP_X_LEFT;
					else if(tempX>CLIP_X_RIGHT) EndX[y]=CLIP_X_RIGHT;
					else EndX[y]=tempX;        /* Store the last coordinate */
				}
			}
			x+=m;		     /* Add our constant to x */
		}
    }
}

//
// Used by DrawConvex to draw a line between two vertices.
// Wire frame rendering.
//
void Line(int x1,int y1,int x2,int y2,char col)
{
	int d,x,y,ax,ay,dx,dy,sx,sy;

	dx=x2-x1;
	ax=ABS(dx)<<1;
	sx=SGN(dx);

	dy=y2-y1;
	ay=ABS(dy)<<1;
	sy=SGN(dy);

	x=x1;
	y=y1;
	if(ax>ay)
		{
		d=ay-(ax>>1);
		while(x!=x2 && y>CLIP_Y_TOP && y<CLIP_Y_BOTTOM)
			{
			VirtScr[(y*320)+x]=col;
			if(d>=0)
				{
				y+=sy;
				d-=ax;
			}
			x+=sx;
			d+=ay;
		}
	}
	else {
		d=ax-(ay>>1);
		while(y!=y2 && y>CLIP_Y_TOP && y<CLIP_Y_BOTTOM)
			{
			VirtScr[(y*320)+x]=col;
			if(d>=0)
				{
				x+=sx;
				d-=ay;
			}
			y+=sy;
			d+=ax;
		}
	}

}

//
// Draw a convex polygon according to the current shading
// model being used.
//
void DrawConvex(convexType *object,int polyNum)
{
	PolygonType *poly;
	int i,Length,ofs;
	char planeCol;

	for(i=0;i<SCRHEIGHT;i++)					// Clear last polys points
		{
		StartX[i]=32767;                        // Set to impossible values
		EndX[i]=-32768;
	}
	poly=&object->Polys[polyNum];
	for(i=0;i<poly->numVerts;i++)
		{
		v0[i]=object->Verts[poly->Verts[i]];
	}
	planeCol=poly->shadedCol;
	if(shadingModel==WIREFRAME)
		{
		for(i=0;i<poly->numVerts-1;i++)
			{
			Line(v0[i].sx,v0[i].sy,v0[i+1].sx,v0[i+1].sy,planeCol);
		}
		Line(v0[i].sx,v0[i].sy,v0[0].sx,v0[0].sy,planeCol);
	}
	else if(shadingModel==LAMBERT)
		{
		for(i=0;i<poly->numVerts-1;i++)
			{
			PolygonLine(v0[i].sx,v0[i].sy,v0[i+1].sx,v0[i+1].sy);
		}
		PolygonLine(v0[i].sx,v0[i].sy,v0[0].sx,v0[0].sy);

		for(i=0,ofs=0;i<SCRHEIGHT;i++,ofs+=SCRWIDTH)
			{
			if(StartX[i]!=32767 && EndX[i]!=-32768)
				{
				Length=EndX[i]-StartX[i];

				if(Length>0)
					{
					memset(&VirtScr[ofs+StartX[i]],planeCol,Length);
					//drawHLine(VirtScr,ofs+StartX[i],planeCol,Length);
				}
				else {
					memset(&VirtScr[ofs+EndX[i]],planeCol,-Length);
				}
			}
		}
	}
}

int compPolys(const void *a,const void *b)
{
	if(userObject.Polys[*(int *)a].centerZ < userObject.Polys[*(int *)b].centerZ)
		{
		return -1;
	}
	else if(userObject.Polys[*(int *)a].centerZ > userObject.Polys[*(int *)b].centerZ)
		{
		return 1;
	}
	else {
		return 0;
	}
}

//
// If normal vector of the polygon is negative it must be facing
// away from us, so DON'T draw it.
// This is known as Back Plane removal
// Also sorts all visible polygons to prevent overlapping in complex objects.
//
void findVisiblePolys(convexType *object)
{
	PolygonType *poly;
	VertexType *v0,*v1,*v2;
	int minz,maxz;
	int i,k;

	for(i=0,k=0;i<object->numPolys;i++)
		{
		poly=&object->Polys[i];

		v0=&object->Verts[poly->Verts[0]];
		v1=&object->Verts[poly->Verts[1]];
		v2=&object->Verts[poly->Verts[2]];
		if(((v1->sx-v0->sx)*(v2->sy-v0->sy)-(v1->sy-v0->sy)*(v2->sx-v0->sx))<0)
			{
			object->sortedPolygonList[k++]=i;
		}
	}
	object->numSortedPolys=k;

	for(i=0;i<object->numSortedPolys;i++)
		{
		poly=&object->Polys[object->sortedPolygonList[i]];

		maxz=-32768;
		minz=32767;
		for(k=0;k<poly->numVerts;k++)
			{
			if(object->Verts[poly->Verts[k]].sz<minz)
				{
				minz=object->Verts[poly->Verts[k]].sz;
			}
			if(object->Verts[poly->Verts[k]].sz>maxz)
				{
				maxz=object->Verts[poly->Verts[k]].sz;
			}
		}
		poly->centerZ=(maxz+minz)>>1;
	}
	qsort((void*)object->sortedPolygonList,object->numSortedPolys,sizeof(object->sortedPolygonList[0]),
			compPolys);
}

//
// Pre-calculates normals to all polygons (planes) to use when
// performing Lambert shading.
// Three points are needed from the plane we wish to find the normal on,
// these I have chosen to be the first, second and last vertices.
// Find vectors between vertex 0 and 1 as well as 0 and (last) and then
// take the cross product of the resulting vectors. Find the square root
// of the normal vector, put it over 1 and multiply by the normal vector.
// The result is a unit vector normal to the surface.
//
//          ¤|   v1
//           |  /          Where ¤ is the normal
//           | /
//           |/
//        v0 \
//		    \
//		     \v2
//
void calcNormals(convexType *object) // Used when performing Lambert shading
{
	WorldPoint av0,av1,av2;
	double normI,normJ,normK;
	double length;
	int i,numVerts;

	for(i=0;i<object->numPolys;i++)
		{
		numVerts=object->Polys[i].numVerts;
		av0.Wx=object->Verts[object->Polys[i].Verts[0]].Ox;    // Setup three vertex
		av0.Wy=object->Verts[object->Polys[i].Verts[0]].Oy;    // point vectors to
		av0.Wz=object->Verts[object->Polys[i].Verts[0]].Oz;    // calculate the normals

		av1.Wx=object->Verts[object->Polys[i].Verts[1]].Ox;
		av1.Wy=object->Verts[object->Polys[i].Verts[1]].Oy;
		av1.Wz=object->Verts[object->Polys[i].Verts[1]].Oz;

		av2.Wx=object->Verts[object->Polys[i].Verts[numVerts-1]].Ox;
		av2.Wy=object->Verts[object->Polys[i].Verts[numVerts-1]].Oy;
		av2.Wz=object->Verts[object->Polys[i].Verts[numVerts-1]].Oz;

		av1.Wx=av0.Wx - av1.Wx;   // Get vector 1
		av1.Wy=av0.Wy - av1.Wy;
		av1.Wz=av0.Wz - av1.Wz;

		av2.Wx=av0.Wx - av2.Wx;   // Get vector 2
		av2.Wy=av0.Wy - av2.Wy;
		av2.Wz=av0.Wz - av2.Wz;

		normI=Fixed2Float(FixedMul(av2.Wy,av1.Wz)-FixedMul(av2.Wz,av1.Wy)); // Calculate normal
		normJ=Fixed2Float(FixedMul(av2.Wz,av1.Wx)-FixedMul(av2.Wx,av1.Wz)); // to plane
		normK=Fixed2Float(FixedMul(av2.Wx,av1.Wy)-FixedMul(av2.Wy,av1.Wx));

		length=sqrt(normI*normI+normJ*normJ+normK*normK); // Normalize found vector
												// to get unit vector
		object->Polys[i].Nx=Float2Fixed(normI/length);
		object->Polys[i].Ny=Float2Fixed(normJ/length);
		object->Polys[i].Nz=Float2Fixed(normK/length);
	}
}

//
// Draws and convex type simple object passed to it.
//
void drawObject(convexType *object)
{
	Fixedpoint dotProduct;
	Fixedpoint nx,ny,nz;
	int k;

	findVisiblePolys(object);

	for(k=0;k<object->numSortedPolys;k++)
		{
		if(shadingModel==LAMBERT || shadingModel==WIREFRAME)
			{
			nx=object->Polys[object->sortedPolygonList[k]].Nwx;
			ny=object->Polys[object->sortedPolygonList[k]].Nwy;
			nz=object->Polys[object->sortedPolygonList[k]].Nwz;

			dotProduct=(FixedMul(nx,LightSource.Ox)+
				FixedMul(ny,LightSource.Oy)+FixedMul(nz,LightSource.Oz));

			dotProduct=17+Fixed2Int(dotProduct);
			object->Polys[object->sortedPolygonList[k]].shadedCol=dotProduct+object->colOfs;
/*			if(dotProduct<=0)
				{
				object->Polys[object->sortedPolygonList[k]].shadedCol=247;
				asm mov	ax,0x03
				asm int	0x10
				printf("Dot product internal panic YYAAARRRR!!!!!");
				asm mov ax,0x4c00
				asm int 0x21
			}*/
		}
		DrawConvex(object,object->sortedPolygonList[k]);
	}
}

void Init3D(void)
{
	int i;

	for(i=0;i<360;i++)                 // Pre-calc SIN, COS lookup tables
		{
		Cos[i]=Float2Fixed(cos((float)(i*PI)/180.0));
		Sin[i]=Float2Fixed(sin((float)(i*PI)/180.0));
	}

	if(!(z_buffer=new int[64000*sizeof(int)]))
		{
		clrscr();

		printf("Critical Stop : Not enough memory for Z Buffer");
	}
}



