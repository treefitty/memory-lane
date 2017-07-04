/********************************************************************


						 GUI Demo
						 --------


	Demo program using my personal GUI library in C++.

		Specs - 640x480x256 resolution
			 - Semi event driven programming ability
			 - Graphics engine based on Alchemy Mindworks
			   SVGA graphics engine. Speed and code efficiency
			   modifications made by Paul Carroll.


		All code of library and demo is subject
		  to Copyright (c) 1996 Paul Carroll.


*********************************************************************/
#include "global.h"

#include <gui.h>

// Global variables
char app_terminated;

void my_btn_execute(void)
{
	app_terminated=1;
}

WINDOW my_win(140,100,400,350,"My Window");
BUTTON my_btn(200,240,60,20,"Quit",BTN_EXECUTE);
DROP_DOWN my_ddcombo(200,200,100);
LIST_BOX my_list(150,280,200,7);

void SetVgaPalette(char *p,unsigned int n)
{
	int i;

	outp(0x3c6,0xff);
	for(i=0;i<n;++i)
		{
		outp(0x3c8,i);
		outp(0x3c9,(*p++) >> 2);
		outp(0x3c9,(*p++) >> 2);
		outp(0x3c9,(*p++) >> 2);
	}
}

void main(void)
{
	point mouse_pt;

	/* go for a graphics mode */
	if(!InitMouse(640,480,TRIDENT)) exit(1);

	/* set the palette */
	set_rgb(WHITE,63,63,63);
	set_rgb(BLACK,0,0,0);
	SetVgaWhite(WHITE);
	SetVgaBlack(BLACK);

	set_rgb(GREY_1, 8, 8, 8);
	set_rgb(GREY_2,24,24,24);
	set_rgb(GREY_3,15,15,15);
	set_rgb(GREY_4,41,41,41);
	set_rgb(GREY_5,8,8,8);
	set_rgb(GREY_6,60,60,60);
	set_rgb(GREY_7,8,8,8);
	set_rgb(GREY_8,63,63,63);
	set_rgb(GREY_9,50,50,50);

	set_rgb(TITLE_1,0,0,23);
	set_rgb(TITLE_2,0,0,33);
	set_rgb(TITLE_3,0,0,43);
	set_rgb(TITLE_4,0,0,53);
	set_rgb(TITLE_5,0,0,63);

	/* clear the screen */
	ClearScreen(0);

	my_win.def_frame(20,40,180,130);
	my_win.put();

	my_btn.execute=my_btn_execute;
	my_btn.put();

	my_ddcombo.add_elem("Test I",1);
	my_ddcombo.add_elem("Test ][",2);
	my_ddcombo.add_elem("Test ]I[",3);
	my_ddcombo.add_elem("Test IV",4);
	my_ddcombo.add_elem("Test V",5);
	my_ddcombo.put();

	my_list.add_elem("Test I",1);
	my_list.add_elem("Test ][",1);
	my_list.add_elem("Test ]I[",1);
	my_list.add_elem("Test IV",1);
	my_list.add_elem("Test V",1);
	my_list.put();

	/* loop 'til we're done */
	while(!app_terminated)
		{
		if(mouse_down(&mouse_pt)==1)
			{
			my_btn.in_bounds(mouse_pt.x,mouse_pt.y);

			my_ddcombo.in_bounds(mouse_pt.x,mouse_pt.y);

			my_list.in_bounds(mouse_pt.x,mouse_pt.y);
		}
	}
	DeinitMouse();
}



