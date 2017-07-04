/*
	File that contains all functions used
	to load data files into memory.
*/

#include "AN_DEFS.H"

#include "floor.h"
#include "wall1.h"
#include "wall2.h"
#include "wall3.h"
#include "wall4.h"
#include "wall5.h"
#include "wall6.h"
#include "plr1.h"
#include "plr2.h"
#include "plr3.h"

void LoadSprites(void)
{
	int i;
	char far *PicBuf;

	if((PicBuf=(char *)malloc(64000))==0) TerminalError(NOMEM,NULL);
	if((player.data[0]=(char *)malloc((16*16)+2))==0) TerminalError(NOMEM,NULL);
	if((player.data[1]=(char *)malloc((16*16)+2))==0) TerminalError(NOMEM,NULL);
	if((player.data[2]=(char *)malloc((16*16)+2))==0) TerminalError(NOMEM,NULL);
	if((StaticObjects[0]=(char *)malloc(16*16))==0) TerminalError(NOMEM,NULL);
	if((StaticObjects[1]=(char *)malloc(16*16))==0) TerminalError(NOMEM,NULL);
	if((StaticObjects[2]=(char *)malloc(16*16))==0) TerminalError(NOMEM,NULL);
	if((StaticObjects[3]=(char *)malloc(16*16))==0) TerminalError(NOMEM,NULL);
	if((StaticObjects[4]=(char *)malloc(16*16))==0) TerminalError(NOMEM,NULL);
	if((StaticObjects[5]=(char *)malloc(16*16))==0) TerminalError(NOMEM,NULL);
	if((StaticObjects[6]=(char *)malloc(16*16))==0) TerminalError(NOMEM,NULL);
	if((StaticObjects[7]=(char *)malloc(16*16))==0) TerminalError(NOMEM,NULL);
	loadPCX("SPRITES.PCX",PicBuf,LOADPAL);

	player.mapx=0;
	player.mapy=0;
	player.x=0;
	player.y=0;
	player.moving=0;
	player.steps=0;
	player.frame=0;
	player.delay=10;
	for(i=0;i<(16*16)+2;i++)
		{
		player.data[0][i]=Plr1[i];
	}
	for(i=0;i<(16*16)+2;i++)
		{
		player.data[1][i]=Plr2[i];
	}
	for(i=0;i<(16*16)+2;i++)
		{
		player.data[2][i]=Plr3[i];
	}

	for(i=0;i<(16*16)+2;i++)
		{
		StaticObjects[0][i]=0;
	}
	StaticObjects[0][0]=4;
	StaticObjects[0][1]=16;
	for(i=0;i<(16*16)+2;i++)
		{
		StaticObjects[1][i]=Floor1[i];
	}
	for(i=0;i<(16*16)+2;i++)
		{
		StaticObjects[2][i]=Wall1Bit[i];
	}
	for(i=0;i<(16*16)+2;i++)
		{
		StaticObjects[3][i]=Wall2Bit[i];
	}
	for(i=0;i<(16*16)+2;i++)
		{
		StaticObjects[4][i]=Wall3Bit[i];
	}
	for(i=0;i<(16*16)+2;i++)
		{
		StaticObjects[5][i]=Wall4Bit[i];
	}
	for(i=0;i<(16*16)+2;i++)
		{
		StaticObjects[6][i]=Wall5Bit[i];
	}
	for(i=0;i<(16*16)+2;i++)
		{
		StaticObjects[7][i]=Wall6Bit[i];
	}

	free(PicBuf);
}

void ReleaseSprites(void)
{
	free(player.data);
}