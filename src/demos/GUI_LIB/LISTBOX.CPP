

#include <gui.h>

/****************************************
/*
/* All list box code follows from here
/*
/****************************************/

LIST_BOX::LIST_BOX(int x,int y,int w,int rows) : list_btn1(x+w+1,y,14,16,BTN_ARROW_UP,BTN_NO_EXECUTE), list_btn2(x+w+1,y+((rows-1)*16),14,16,BTN_ARROW_DOWN,BTN_NO_EXECUTE)
{
	frame.x=x;
	frame.y=y;
	frame.w=w;
	frame.h=16*rows;

	num_elems=0;
	selected_elem=-1;

	memset(select_list,0,MAX_ELEMS);
}

void LIST_BOX::put(void)
{
	int i;

	//Draw the main list
	fill_rect(frame.x+1,frame.y+1,frame.x+frame.w-1,frame.y+frame.h-1,GREY_8);

	draw_line(frame.x,frame.y,frame.x,frame.y+frame.h,GREY_1);
	draw_line(frame.x,frame.y,frame.x+frame.w,frame.y,GREY_1);

	draw_line(frame.x+frame.w,frame.y,frame.x+frame.w,frame.y+frame.h,GREY_9);
	draw_line(frame.x,frame.y+frame.h,frame.x+frame.w,frame.y+frame.h,GREY_9);

	//Draw the scroll bar
	draw_line(frame.x+frame.w+1 ,frame.y+16,frame.x+frame.w+1 ,frame.y+frame.h-16,GREY_9);
	draw_line(frame.x+frame.w+1 ,frame.y+17,frame.x+frame.w+15,frame.y+17,GREY_9);

	draw_line(frame.x+frame.w+15,frame.y+17,frame.x+frame.w+15,frame.y+frame.h-16,GREY_1);
	draw_line(frame.x+frame.w,frame.y+frame.h-17,frame.x+frame.w+15,frame.y+frame.h-17,GREY_1);

	//Put the scroll bar buttons on the screen
	list_btn1.put();
	list_btn2.put();

	// NOTE **** needs other end of tbl check
	for(i=0;i<num_elems;i++)
		{
		DrawString(frame.x+4,frame.y+3+(i*12),elems[i].text,0,1);
	}
}

void LIST_BOX::in_bounds(int x,int y)
{
	static int i;

	if(list_btn1.in_bounds(x,y)==BTN_NOT_PRESSED)
		{
		i=i;
	}

	if(list_btn2.in_bounds(x,y)==BTN_PRESSED)
		{
		i=i;
	}

	i=in_rect(&frame,x,y);
	if(i==1)
		{
		/* Do list click/list select code here... */
		i=i;
	}
}

void LIST_BOX::pressed(void)
{

}

void LIST_BOX::add_elem(char *text,int value)
{
	elems[num_elems].value=value;
	elems[num_elems].text=text;

	num_elems++;
}
