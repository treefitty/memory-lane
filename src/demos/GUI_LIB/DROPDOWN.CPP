
#include <gui.h>

/************************************************
/*
/* All dropdown object code follows from here
/*
/************************************************/

DROP_DOWN::DROP_DOWN(int x,int y,int w) : list_btn(x+w+1,y,14,16,BTN_ARROW_DOWN,BTN_NO_EXECUTE)
{
	box_frame.x=x;
	box_frame.y=y;
	box_frame.w=w;
	box_frame.h=16;

	list_frame.x=x+2;
	list_frame.y=y+17;
	list_frame.w=w+16;
	list_frame.h=0;

//	list_btn.execute=::list_dropped;

	num_elems=0;
	selected_elem=-1;

	state=LIST_UP;

	if((back_ground=(char *)malloc(ImageSize256(list_frame.x,list_frame.y,list_frame.x+list_frame.w,list_frame.y+(MAX_LIST_ELEMS*12))))==0)
		{
		DeinitMouse();
		clrscr();
		printf("Out of memory!\n\n");
		exit(1);
	}
	memset(select_list,0,MAX_ELEMS);
}

// Changes and displays the buttons alternate state.
void DROP_DOWN::change_state(void)
{
	int i;

	if(state==LIST_UP)
		{
		state=LIST_DOWN;

		if(num_elems<MAX_ELEMS)
			{
			get_image(list_frame.x,list_frame.y,list_frame.x+list_frame.w,list_frame.y+list_frame.h,back_ground);

			fill_rect(list_frame.x,list_frame.y,list_frame.x+list_frame.w,list_frame.y+list_frame.h,1);
			framerect(list_frame.x,list_frame.y,list_frame.x+list_frame.w,list_frame.y+list_frame.h,0);
		}
		else {
			get_image(list_frame.x,list_frame.y,list_frame.x+list_frame.w,list_frame.h,back_ground);

			fill_rect(list_frame.x,list_frame.y,list_frame.x+list_frame.w,list_frame.y+list_frame.h,1);
			framerect(list_frame.x,list_frame.y,list_frame.x+list_frame.w,list_frame.y+list_frame.h,0);
		}
		for(i=0;i<MAX_LIST_ELEMS && i<num_elems;i++)
			{
			DrawString(box_frame.x+5,box_frame.y+20+(i*12),elems[i].text,0,1);
		}
	}
	else {
		state=LIST_UP;
		put_image(box_frame.x+2,box_frame.y+17,back_ground);
	}
	list_btn.change_state();
}

void DROP_DOWN::in_bounds(int x,int y)
{
	if(list_btn.in_bounds(x,y)==BTN_PRESSED)
		{
          list_dropped();
	}
}

void DROP_DOWN::list_dropped(void)
{
	point mouse_pt;
	int btn_down;
	int i;

	change_state();
	if(selected_elem!=-1)
		{
		fill_rect(list_frame.x+1,list_frame.y+(selected_elem*12)+1,list_frame.x+list_frame.w-1,list_frame.y+((selected_elem+1)*12)-1,TITLE_4);
		DrawString(list_frame.x+3,list_frame.y+3+(selected_elem*12),elems[selected_elem].text,1,TITLE_4);
	}

	while(mouse_down(&mouse_pt)==1) {}
	while(state==LIST_DOWN)
		{
		btn_down=mouse_down(&mouse_pt);
		if(btn_down==1 && in_rect(&list_frame,mouse_pt.x,mouse_pt.y))
			{
			if(selected_elem!=-1)
				{
				fill_rect(list_frame.x+1,list_frame.y+(selected_elem*12)+1,list_frame.x+list_frame.w-1,list_frame.y+((selected_elem+1)*12)-1,1);
				DrawString(list_frame.x+3,list_frame.y+3+(selected_elem*12),elems[selected_elem].text,0,1);
			}
			for(i=0;i<MAX_LIST_ELEMS && i<num_elems;i++)
				{
				if(mouse_pt.y>list_frame.y+(i*12) && mouse_pt.y<=list_frame.y+((i+1)*12))
					{
					memset(select_list,0,MAX_ELEMS);
					select_list[i]=1;
					selected_elem=i;
				}
			}
			fill_rect(list_frame.x+1,list_frame.y+(selected_elem*12)+1,list_frame.x+list_frame.w-1,list_frame.y+((selected_elem+1)*12)-1,TITLE_4);
			DrawString(list_frame.x+3,list_frame.y+3+(selected_elem*12),elems[selected_elem].text,1,TITLE_4);
			put();
			while(mouse_down(&mouse_pt)) {}
		}
		else if(btn_down==1)
			{
			while(mouse_down(&mouse_pt)) {}
			change_state();
		}
	}
}

void DROP_DOWN::put(void)
{
	fill_rect(box_frame.x+1,box_frame.y+1,box_frame.x+box_frame.w-1,box_frame.y+box_frame.h-1,GREY_8);

	if(selected_elem!=-1)
		{
		DrawString(box_frame.x+3,box_frame.y+5,elems[selected_elem].text,0,1);
	}

	draw_line(box_frame.x,box_frame.y,box_frame.x,box_frame.y+box_frame.h,GREY_1);
	draw_line(box_frame.x,box_frame.y,box_frame.x+box_frame.w,box_frame.y,GREY_1);

	draw_line(box_frame.x+box_frame.w,box_frame.y,box_frame.x+box_frame.w,box_frame.y+box_frame.h,GREY_6);
	draw_line(box_frame.x,box_frame.y+box_frame.h,box_frame.x+box_frame.w,box_frame.y+box_frame.h,GREY_6);

	list_btn.put();

	/*if(list_btn.state==LIST_UP)
		{
		draw_line(box_frame.x+box_frame.w+8,box_frame.y+4,box_frame.x+box_frame.w+8,box_frame.y+box_frame.h-5,GREY_1);
		draw_line(box_frame.x+box_frame.w+7,box_frame.y+4,box_frame.x+box_frame.w+7,box_frame.y+box_frame.h-5,GREY_1);

		draw_line(box_frame.x+box_frame.w+6,box_frame.y+box_frame.h-6,box_frame.x+box_frame.w+9,box_frame.y+box_frame.h-6,GREY_1);
		draw_line(box_frame.x+box_frame.w+5,box_frame.y+box_frame.h-7,box_frame.x+box_frame.w+10,box_frame.y+box_frame.h-7,GREY_1);
	}
	else {
		draw_line(box_frame.x+box_frame.w+9,box_frame.y+5,box_frame.x+box_frame.w+9,box_frame.y+box_frame.h-4,GREY_1);
		draw_line(box_frame.x+box_frame.w+8,box_frame.y+5,box_frame.x+box_frame.w+8,box_frame.y+box_frame.h-4,GREY_1);

		draw_line(box_frame.x+box_frame.w+7,box_frame.y+box_frame.h-5,box_frame.x+box_frame.w+10,box_frame.y+box_frame.h-5,GREY_1);
		draw_line(box_frame.x+box_frame.w+6,box_frame.y+box_frame.h-6,box_frame.x+box_frame.w+11,box_frame.y+box_frame.h-6,GREY_1);
	} */
}

void DROP_DOWN::add_elem(char *text,int value)
{
	elems[num_elems].value=value;
	elems[num_elems].text=text;

	num_elems++;
	list_frame.h+=12;
}

