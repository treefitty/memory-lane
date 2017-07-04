/***********************************************************
 *
 *	All code presented here after should be considered to
 *  	  be protected under the laws of Copyright (c) 1996.
 *   Reproduction of any code may only take place with the
 *	            authors expressed permission.
 *
 *
 *   	  Name   : SNAKES.CPP
 *   	  Author : Paul Carroll
 *   Last Modified : 8/5/96
 *     Description : Main file the snakes game
 *	      Callers : None
 *
 *   !!! NOTE !!!  -> MUST BE COMPILED IN THE LARGE MEMORY
 *                    MODEL BECAUSE OF THE STACK FRAME SETUP
 *				  IN THE ASSEMBLY MODULES
 *
 *			    -> COMPILER OPTION TO TREAT ALL CHAR's AS
 *				  UNSIGNED SHOULD ALSO BE ON
 *
 ***********************************************************/
#include "global.h"                          // Global definitions file
#include "snakes.h"                          // Snake player graphics data

#define DEBUG		0

/*************************************
/* Global variables
/*************************************/
snake_plr player_1;						// Player

char keyflags[128];						// Used in keyboard handling ISR
char map_data[MAP_WIDTH*MAP_HEIGHT];		// The 'object' map

/********************************************
/* This routine steps through each section
/* of the snake and calculates the direction
/* that section is to move based on the
/* position of the next section. It will
/* only be called when a NEW movement is occuring.
/* ie. when all sections are in ONE map location
/* and the user input is being polled.
/********************************************/
void calc_new_move(int direction)
{
	point *ptr_1,*ptr_2;
	int temp_x,temp_y;
	int i;

	temp_y=(player_1.parts[player_1.num_parts-1].map_y);
	temp_x=(player_1.parts[player_1.num_parts-1].map_x);
	for(i=player_1.num_parts-1;i!=0;i--)
		{
		ptr_1=&player_1.parts[i];
		ptr_2=&player_1.parts[i-1];

		map_data[(ptr_2->map_y*MAP_WIDTH)+ptr_2->map_x]=OCCUPIED;

		/****************************
		/* For a part moving right
		/****************************/
		if(ptr_1->y==ptr_2->y && ptr_2->x>ptr_1->x)
			{
			ptr_1->direction=RIGHT;
			ptr_1->map_x++;
		}
		/****************************
		/* For a part moving down
		/****************************/
		else if(ptr_1->x==ptr_2->x && ptr_1->y<ptr_2->y)
			{
			ptr_1->direction=DOWN;
			ptr_1->map_y++;
		}
		/****************************
		/* For a part moving left
		/****************************/
		else if(ptr_1->y==ptr_2->y && ptr_2->x<ptr_1->x)
			{
			ptr_1->direction=LEFT;
			ptr_1->map_x--;
		}
		/****************************
		/* For a part moving up
		/****************************/
		else if(ptr_1->x==ptr_2->x && ptr_1->y>ptr_2->y)
			{
			ptr_1->direction=UP;
			ptr_1->map_y--;
		}
	}
	map_data[(temp_y*MAP_WIDTH)+temp_x]=VACANT;
	// This check and update must be performed separate
	// to the last part for the head of the snake.
	switch(direction)
		{
		case RIGHT :
			{
			player_1.parts[0].direction=RIGHT;
			player_1.parts[0].map_x++;
			break;
		}
		case LEFT :
			{
			player_1.parts[0].direction=LEFT;
			player_1.parts[0].map_x--;
			break;
		}
		case UP :
			{
			player_1.parts[0].direction=UP;
			player_1.parts[0].map_y--;
			break;
		}
		case DOWN :
			{
			player_1.parts[0].direction=DOWN;
			player_1.parts[0].map_y++;
			break;
		}
	}
	map_data[(player_1.parts[0].map_y*MAP_WIDTH)+player_1.parts[0].map_x]=OCCUPIED;
}

void main(void)
{
	int i;
	char done;
	char page;
	char check_user=1;

	x_setmode();
	x_set_palette(snake_pal);
	x_set_double_buffer();

	set_kb();

	memset(map_data,VACANT,MAP_WIDTH*MAP_HEIGHT);

	for(i=0;i<START_PARTS;i++)
		{
		player_1.parts[i].x=(PLAYER_STEP*(START_PARTS-i-1));
		player_1.parts[i].y=PLAYER_STEP;
		player_1.parts[i].map_x=START_PARTS-i-1;
		player_1.parts[i].map_y=1;
		player_1.parts[i].direction=RIGHT;
		map_data[i+MAP_WIDTH]=OCCUPIED;
	}

	player_1.num_parts=START_PARTS;

	page=1;
	done=0;
	while(!done)
		{
		x_set_active_page(page);
		x_clr_page();
		if(keyflags[escape]==1)
			{
			done=1;
		}
		else if(check_user)
			{
			if(keyflags[right_arrow]==1)
				{
				player_1.parts[0].direction=RIGHT;
			}
			else if(keyflags[left_arrow]==1)
				{
				player_1.parts[0].direction=LEFT;
			}
			else if(keyflags[up_arrow]==1)
				{
				player_1.parts[0].direction=UP;
			}
			else if(keyflags[down_arrow]==1)
				{
				player_1.parts[0].direction=DOWN;
			}
			calc_new_move(player_1.parts[0].direction);
			check_user=0;
		}
		for(i=0;i<player_1.num_parts;i++)
			{
			switch(player_1.parts[i].direction)
				{
				case RIGHT :
					{
					player_1.parts[i].x++;
					break;
				}
				case LEFT :
					{
					player_1.parts[i].x--;
					break;
				}
				case UP :
					{
					player_1.parts[i].y--;
					break;
				}
				case DOWN :
					{
					player_1.parts[i].y++;
					break;
				}
			}
			if(++player_1.moved==PLAYER_STEP*player_1.num_parts)
				{
				player_1.moved=0;
				check_user=1;
			}
			x_put_pbm(player_1.parts[i].x,player_1.parts[i].y,snake_blt);
		}

		x_flip_page();
		x_wait_vr();

		page=(page+1) & 1;
	}

	reset_kb();
	x_textmode();

/**************************************************/
/********** USED FOR MAP DEBUGGING ****************/
/**************************************************/
// k defined in loop to prevent compiler warning
// when not map debugging.
//
	#if DEBUG

	for(int k=0;k<MAP_HEIGHT;k++)
		{
		for(i=0;i<MAP_WIDTH;i++)
			{
			printf("%c",map_data[(k*MAP_WIDTH)+i]);
		}
		printf("\n");
	}
	getch();

	#endif
/**************************************************/
}