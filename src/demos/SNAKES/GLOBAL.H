#include <conio.h>
#include <stdio.h>
#include <mem.h>

#include <key.h>
#include <x_lib.h>          

/**************************************
/* Enhanced scan codes only use with
/* game key interrupt handler
/**************************************/
#define right_arrow  	0x4d
#define left_arrow   	0x4b
#define up_arrow     	0x48
#define down_arrow   	0x50
#define escape		 	0x01

/**************************************
/* Player directions
/**************************************/
#define RIGHT			0
#define LEFT			1
#define UP			2
#define DOWN			3

/**************************************
/* Misc defs
/**************************************/
#define PLAYER_STEP		13
#define MAX_PARTS		20
#define START_PARTS		8

#define MAP_WIDTH		24
#define MAP_HEIGHT		15

#define OCCUPIED		42
#define VACANT			250

/*************************************
/* For global variable definition
/*************************************/
extern char keyflags[128];

/*************************************
/* Structure defs
/*************************************/
typedef struct {

	int x;
	int y;
	int map_x;
	int map_y;
     char direction;

} point;

typedef struct {

	point parts[30];
	char num_parts;
	char moved;
	char color;

} snake_plr;
