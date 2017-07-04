
#include <gui.h>

BUTTON::BUTTON(int but_x,int but_y,int width,int height,char *text,char type)
{
	button_text=text;
	frame.x=but_x;
	frame.y=but_y;
	frame.w=width;
	frame.h=height;
	state=BTN_UP;
	btn_type=type;
     arrow_dir=BTN_NO_ARROW;
}

BUTTON::BUTTON(int but_x,int but_y,int width,int height,char dir,char type)
{
	frame.x=but_x;
	frame.y=but_y;
	frame.w=width;
	frame.h=height;
	state=BTN_UP;
	btn_type=type;
	arrow_dir=dir;
}

void BUTTON::put(void)
{
	fill_rect(frame.x,frame.y,frame.x+frame.w,frame.y+frame.h,GREY_9);

	if(state==BTN_UP)
		{
		draw_line(frame.x,frame.y,frame.x,frame.y+frame.h,GREY_6);
		draw_line(frame.x,frame.y,frame.x+frame.w,frame.y,GREY_6);

		draw_line(frame.x+frame.w,frame.y,frame.x+frame.w,frame.y+frame.h,GREY_1);
		draw_line(frame.x,frame.y+frame.h,frame.x+frame.w,frame.y+frame.h,GREY_1);

		draw_line(frame.x+frame.w-1,frame.y+1,frame.x+frame.w-1,frame.y+frame.h-1,GREY_2);
		draw_line(frame.x+1,frame.y+frame.h-1,frame.x+frame.w-1,frame.y+frame.h-1,GREY_2);

		switch(arrow_dir)
			{
			case BTN_NO_ARROW :
				{
				DrawString(frame.x+((frame.w>>1)-((strlen(button_text)>>1)*8)),
						frame.y+(frame.h>>1)-4,button_text,0,GREY_9);

				break;
			}
			case BTN_ARROW_UP :
				{
				draw_line(frame.x+7,frame.y+4,frame.x+7,frame.y+frame.h-5,GREY_1);
				draw_line(frame.x+6,frame.y+4,frame.x+6,frame.y+frame.h-5,GREY_1);

				draw_line(frame.x+5,frame.y+5,frame.x+8,frame.y+5,GREY_1);
				draw_line(frame.x+4,frame.y+6,frame.x+9,frame.y+6,GREY_1);

				break;
			}
			case BTN_ARROW_DOWN :
				{
				draw_line(frame.x+7,frame.y+4,frame.x+7,frame.y+frame.h-5,GREY_1);
				draw_line(frame.x+6,frame.y+4,frame.x+6,frame.y+frame.h-5,GREY_1);

				draw_line(frame.x+5,frame.y+frame.h-6,frame.x+8,frame.y+frame.h-6,GREY_1);
				draw_line(frame.x+4,frame.y+frame.h-7,frame.x+9,frame.y+frame.h-7,GREY_1);

				break;
			}
		}
	}
	else {
		draw_line(frame.x,frame.y,frame.x,frame.y+frame.h,GREY_1);
		draw_line(frame.x,frame.y,frame.x+frame.w,frame.y,GREY_1);

		draw_line(frame.x+frame.w,frame.y,frame.x+frame.w,frame.y+frame.h,GREY_6);
		draw_line(frame.x,frame.y+frame.h,frame.x+frame.w,frame.y+frame.h,GREY_6);

		draw_line(frame.x+1,frame.y+1,frame.x+1,frame.y+frame.h-2,GREY_2);
		draw_line(frame.x+1,frame.y+1,frame.x+frame.w-2,frame.y+1,GREY_2);

		switch(arrow_dir)
			{
			case BTN_NO_ARROW :
				{
				DrawString(frame.x+1+((frame.w>>1)-((strlen(button_text)>>1)*8)),
				frame.y+1+(frame.h>>1)-4,button_text,0,GREY_9);

				break;
			}
			case BTN_ARROW_UP :
				{
				draw_line(frame.x+8,frame.y+5,frame.x+8,frame.y+frame.h-4,GREY_1);
				draw_line(frame.x+7,frame.y+5,frame.x+7,frame.y+frame.h-4,GREY_1);

				draw_line(frame.x+6,frame.y+6,frame.x+9,frame.y+6,GREY_1);
				draw_line(frame.x+5,frame.y+7,frame.x+10,frame.y+7,GREY_1);

				break;
			}
			case BTN_ARROW_DOWN :
				{
				draw_line(frame.x+8,frame.y+5,frame.x+8,frame.y+frame.h-4,GREY_1);
				draw_line(frame.x+7,frame.y+5,frame.x+7,frame.y+frame.h-4,GREY_1);

				draw_line(frame.x+6,frame.y+frame.h-5,frame.x+9,frame.y+frame.h-5,GREY_1);
				draw_line(frame.x+5,frame.y+frame.h-6,frame.x+10,frame.y+frame.h-6,GREY_1);

				break;
			}
		}
	}
}

// Changes and displays the buttons alternate state.
void BUTTON::change_state(void)
{
	if(state==BTN_UP)
		{
		state=BTN_DOWN;
	}
	else {
		state=BTN_UP;
	}
	put();
}

// Handles when the button has been pressed.
// Changes the button's apperance and then makes sure the
// mouse button was released over the screen button. This
// gives the user a back out option.
// Returns 1 on success, else 0
int  BUTTON::pressed(void)
{
	point mouse_pt;
	int mouse_ok;

	change_state();

	while(mouse_down(&mouse_pt)==1)
		{
		mouse_ok=in_rect(&frame,mouse_pt.x,mouse_pt.y);
		if((mouse_ok==1 && state==BTN_UP) || (mouse_ok==0 && state==BTN_DOWN))
			{
			change_state();
		}
	}
	if(state==BTN_DOWN)
		{
		change_state();
	}
	if(in_rect(&frame,mouse_pt.x,mouse_pt.y)==1)
		{
		if(btn_type!=BTN_NO_EXECUTE) execute();
		return BTN_PRESSED;
	}
	return BTN_NOT_PRESSED;
}

// Checks whether the point specified is in the
// bounds of the button.
// Returns 1 (true) if it is, else 0
int  BUTTON::in_bounds(int x,int y)
{
	if(in_rect(&frame,x,y))
		{
		return pressed();
	}
	return BTN_NOT_PRESSED;
}

