#include "AN_DEFS.H"

#include "game1.h"
#include "game2.h"
#include "game3.h"
#include "game4.h"

// Global variable declarations

playerType player;
screenType screen;
char keyflags[128];

char ObjectMap[]=
{
		    1,1,1,1,2,4,4,4,4,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		    1,4,5,1,7,1,1,1,1,7,1,1,2,1,2,4,4,5,1,1,1,1,1,1,
		    1,1,7,1,1,1,2,4,1,3,4,1,7,1,7,0,0,7,1,1,1,1,7,1,
		    7,1,7,1,4,4,6,1,1,1,1,1,7,1,7,0,0,7,1,1,1,1,7,1,
		    7,1,1,1,1,1,1,1,2,4,4,4,6,1,7,0,0,7,1,1,1,1,7,1,
		    3,5,1,2,4,4,5,1,7,1,1,1,1,1,3,4,4,6,1,1,1,1,7,1,
		    1,1,1,7,0,0,7,1,7,1,7,1,2,5,1,1,1,1,1,1,1,1,7,1,
		    1,7,1,3,4,4,6,1,7,1,7,1,7,3,5,1,2,4,5,1,1,1,7,1,
		    1,7,1,1,1,1,1,1,1,1,7,1,7,0,7,1,7,0,7,1,1,1,7,1,
		    1,3,4,4,4,4,4,1,4,4,6,1,3,4,6,1,3,4,6,1,1,1,7,1,
		    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,
		    1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,3,5,
		    1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,1,1,1,7,
		    1,1,1,1,4,4,4,4,4,4,4,6,1,1,1,1,1,1,1,1,1,1,1,7,
		    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7
};

char far *StaticObjects[MAXOBJECTS];

void SetStartAddr(int ofs)
{
	asm mov	bx,ofs
	asm mov	dx,0x3d4
	asm mov	al,0x0c
	asm mov	ah,bh
	asm out	dx,ax
	asm inc	al
	asm mov	ah,bl
	asm out	dx,ax
}

void GameLoop(void)
{
	int bitmapX,bitmapY,mapOffs;
	int i,j;
	char done;

	LoadSprites();
	set_ikb();
	done=0;
	memset(keyflags,0,128);

	screen.mapx=0;
	screen.mapy=0;
	screen.x=0;
	screen.y=0;
	screen.moving=0;
	screen.steps=0;

	TopBound=1;
	LeftBound=1;
	BottomBound=160;
	RightBound=319;

	x_set_cliprect(1,1,319,159);

	for(j=screen.mapy;j<SCREENHEIGHT+screen.mapy;j++)
		{
		for(i=screen.mapx;i<SCREENWIDTH+screen.mapx;i++)
			{
			x_put_masked_pbm((i<<4)+16,(j<<4)+16,VisiblePageOffs,StaticObjects[ObjectMap[(j*MAPWIDTH)+i]]);
		}
	}
	for(j=screen.mapy;j<SCREENHEIGHT+screen.mapy;j++)
		{
		for(i=screen.mapx;i<SCREENWIDTH+screen.mapx;i++)
			{
			x_put_masked_pbm((i<<4)+16,(j<<4)+16,HiddenPageOffs,StaticObjects[ObjectMap[(j*MAPWIDTH)+i]]);
		}
	}
	x_put_pbm(0,0,HiddenPageOffs,Game1);
	x_put_pbm(0,16,HiddenPageOffs,Game2);
	x_put_pbm(304,16,HiddenPageOffs,Game3);
	x_put_pbm(0,176,HiddenPageOffs,Game4);

	while(!done)
		{
		if(player.moving==1)
			{
			player.x+=player.xstep;
			player.y+=player.ystep;

			if(++player.steps==16)
				{
				player.moving=0;
				player.steps=0;
				player.mapx+=player.xstep;
				player.mapy+=player.ystep;
			}
		}
		if(screen.moving==1)
			{
			screen.x+=screen.xstep;
			screen.y+=screen.ystep;

			if(++screen.steps==16)
				{
				screen.moving=0;
				screen.steps=0;
				screen.mapx+=screen.xstep;
				screen.mapy+=screen.ystep;
				player.mapx+=screen.xstep;
				player.mapy+=screen.ystep;
			}
		}
		if(player.moving==0 && screen.moving==0)
			{
			if(keyflags[leftArrow]==1 && player.mapx>0 && ObjectMap[(player.mapy*MAPWIDTH)+player.mapx-1]==1)
				{
				if(screen.mapx==0 || player.mapx>MAPWIDTH-(SCREENWIDTH>>1))
					{
					player.moving=1;
					player.xstep=-1;
					player.ystep=0;
				}
				else {
					screen.moving=1;
					screen.xstep=-1;
					screen.ystep=0;
				}
			}
			else if(keyflags[rightArrow]==1 && player.mapx<MAPWIDTH-1 && ObjectMap[(player.mapy*MAPWIDTH)+player.mapx+1]==1)
				{
				if(screen.mapx==MAPWIDTH-SCREENWIDTH || player.mapx<(SCREENWIDTH>>1))
					{
					player.moving=1;
					player.xstep=1;
					player.ystep=0;
				}
				else {
					screen.moving=1;
					screen.xstep=1;
					screen.ystep=0;
				}
			}
			else if(keyflags[upArrow]==1 && player.mapy>0 && ObjectMap[((player.mapy-1)*MAPWIDTH)+player.mapx]==1)
				{
				if(screen.mapy==0 || player.mapy>MAPHEIGHT-(SCREENHEIGHT>>1))
					{
					player.moving=1;
					player.xstep=0;
					player.ystep=-1;
				}
				else {
					screen.moving=1;
					screen.xstep=0;
					screen.ystep=-1;
				}
			}
			else if(keyflags[downArrow]==1 && player.mapy<MAPHEIGHT-1 && ObjectMap[((player.mapy+1)*MAPWIDTH)+player.mapx]==1)
				{
				if(screen.mapy==MAPHEIGHT-SCREENHEIGHT || player.mapy<((SCREENHEIGHT-1)>>1))
					{
					player.moving=1;
					player.xstep=0;
					player.ystep=1;
				}
				else {
					screen.moving=1;
					screen.xstep=0;
					screen.ystep=1;
				}
			}
			else if(keyflags[esc]==1)
				{
				done=1;
			}
			else if(keyflags[0x19]==1)
				{
				while(keyflags[0x19]==1);
			}
		}

		if(screen.moving)
			{
			if(screen.ystep==0)
				{
				for(j=screen.mapy;j<SCREENHEIGHT+screen.mapy;j++)
					{
					bitmapY=((j-screen.mapy)<<4)+16;
					mapOffs=j*MAPWIDTH;
					if(screen.xstep>0)
						{
						for(i=screen.mapx;i<SCREENWIDTH+screen.mapx+1;i++)
							{
							x_put_pbm(((i-screen.mapx)<<4)-screen.steps+16,bitmapY,HiddenPageOffs,StaticObjects[ObjectMap[mapOffs+i]]);
						}
					}
					else {
						for(i=screen.mapx-1;i<SCREENWIDTH+screen.mapx;i++)
							{
							x_put_pbm(((i-screen.mapx)<<4)+screen.steps+16,bitmapY,HiddenPageOffs,StaticObjects[ObjectMap[mapOffs+i]]);
						}
					}
				}
			}
			else {
				if(screen.ystep>0)
					{
					for(j=screen.mapy;j<SCREENHEIGHT+screen.mapy+1;j++)
						{
						mapOffs=j*MAPWIDTH;
						for(i=screen.mapx;i<SCREENWIDTH+screen.mapx;i++)
							{
							x_put_pbm(((i-screen.mapx)<<4)+16,((j-screen.mapy)<<4)-screen.steps+16,HiddenPageOffs,StaticObjects[ObjectMap[mapOffs+i]]);
						}
					}
				}
				else {
					for(j=screen.mapy-1;j<SCREENHEIGHT+screen.mapy;j++)
						{
						mapOffs=j*MAPWIDTH;
						for(i=screen.mapx;i<SCREENWIDTH+screen.mapx;i++)
							{
							x_put_pbm(((i-screen.mapx)<<4)+16,((j-screen.mapy)<<4)+screen.steps+16,HiddenPageOffs,StaticObjects[ObjectMap[mapOffs+i]]);
						}
					}
				}
			}
		}
		else {
			for(j=screen.mapy;j<SCREENHEIGHT+screen.mapy;j++)
				{
				bitmapY=((j-screen.mapy)<<4)+16;
				mapOffs=j*MAPWIDTH;
				for(i=screen.mapx;i<SCREENWIDTH+screen.mapx;i++)
					{
					x_put_pbm(((i-screen.mapx)<<4)+16,bitmapY,HiddenPageOffs,StaticObjects[ObjectMap[mapOffs+i]]);
				}
			}
		}

		x_put_pbm(0,0,HiddenPageOffs,Game1);
		x_put_pbm(0,16,HiddenPageOffs,Game2);
		x_put_pbm(304,16,HiddenPageOffs,Game3);
		x_put_pbm(0,176,HiddenPageOffs,Game4);

		x_put_masked_pbm(player.x+16,player.y+16,HiddenPageOffs,player.data[player.frame%3]);

		if(--player.delay==0)
			{
			player.frame++;
			player.delay=7;
		}

		x_page_flip(0,0);
	}
	reset_ikb();
	ReleaseSprites();
}






