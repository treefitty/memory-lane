
#include <gui.h>

int  in_rect(rect *a_rect,int x,int y)
{
	if(x>a_rect->x && x<(a_rect->x+a_rect->w) && y>a_rect->y && y<(a_rect->y+a_rect->h))
		{
		return 1;
	}
	return 0;
}


WINDOW::WINDOW(int win_x,int win_y,int width,int height,char *text)
{
	title_text=text;
	frame.x=win_x;
	frame.y=win_y;
	frame.w=width;
	frame.h=height;

	num_frames=0;
}

void WINDOW::put(void)
{
	int i;

	fill_rect(frame.x,frame.y+TITLE_HEIGHT,frame.x+frame.w,frame.y+frame.h+TITLE_HEIGHT,GREY_4);

	framerect(frame.x    ,frame.y    +TITLE_HEIGHT,frame.x+frame.w  ,frame.y+frame.h  +TITLE_HEIGHT,GREY_6);
	framerect(frame.x  +1,frame.y  +1+TITLE_HEIGHT,frame.x+frame.w-1,frame.y+frame.h-1+TITLE_HEIGHT,GREY_6);

	draw_line(frame.x  +1,frame.y+frame.h  +TITLE_HEIGHT,frame.x+frame.w  ,frame.y+frame.h  +TITLE_HEIGHT,GREY_2);              // b
	draw_line(frame.x  +2,frame.y+frame.h-1+TITLE_HEIGHT,frame.x+frame.w  ,frame.y+frame.h-1+TITLE_HEIGHT,GREY_2);              // b-1

	draw_line(frame.x+frame.w  ,frame.y  +1+TITLE_HEIGHT,frame.x+frame.w  ,frame.y+frame.h  +TITLE_HEIGHT,GREY_2);              // r
	draw_line(frame.x+frame.w-1,frame.y  +2+TITLE_HEIGHT,frame.x+frame.w-1,frame.y+frame.h  +TITLE_HEIGHT,GREY_2);              // r-1

	draw_line(frame.x  +3,frame.y  +3+TITLE_HEIGHT,frame.x  +3,frame.y+frame.h-3+TITLE_HEIGHT,GREY_2);              // l+4
	draw_line(frame.x  +3,frame.y+frame.h-3+TITLE_HEIGHT,frame.x+frame.w-3,frame.y+frame.h-3+TITLE_HEIGHT,GREY_6);

	draw_line(frame.x  +3,frame.y  +3+TITLE_HEIGHT,frame.x+frame.w-3,frame.y  +3+TITLE_HEIGHT,GREY_2);
	draw_line(frame.x+frame.w-3,frame.y  +3+TITLE_HEIGHT,frame.x+frame.w-3,frame.y+frame.h-3+TITLE_HEIGHT,GREY_6);

	/* Title Bar */
	fill_rect(frame.x    ,frame.y                 ,frame.x+frame.w  ,frame.y    +TITLE_HEIGHT,TITLE_3);
	DrawString(frame.x+6,frame.y+4,title_text,GREY_9,TITLE_3);

	draw_line(frame.x    ,frame.y                 ,frame.x    ,frame.y    +TITLE_HEIGHT,TITLE_4);
	draw_line(frame.x  +1,frame.y                 ,frame.x  +1,frame.y    +TITLE_HEIGHT,TITLE_4);

	draw_line(frame.x    ,frame.y                 ,frame.x+frame.w  ,frame.y                 ,TITLE_4);
	draw_line(frame.x    ,frame.y  +1             ,frame.x+frame.w  ,frame.y  +1             ,TITLE_4);

	draw_line(frame.x+frame.w  ,frame.y                 ,frame.x+frame.w  ,frame.y    +TITLE_HEIGHT,TITLE_2);
	draw_line(frame.x+frame.w-1,frame.y  +1             ,frame.x+frame.w-1,frame.y    +TITLE_HEIGHT,TITLE_2);

	draw_line(frame.x  +1,frame.y    +TITLE_HEIGHT,frame.x+frame.w  ,frame.y    +TITLE_HEIGHT,TITLE_2);
	draw_line(frame.x  +2,frame.y  -1+TITLE_HEIGHT,frame.x+frame.w  ,frame.y  -1+TITLE_HEIGHT,TITLE_2);

	/* Put all frames */
	for(i=0;i<num_frames;i++)
		{
		framerect(frame.x+frames[i].x1+1,frame.y+frames[i].y1+1,frame.x+frames[i].x2+1,frame.y+frames[i].y2+1,GREY_6);
		framerect(frame.x+frames[i].x1,frame.y+frames[i].y1,frame.x+frames[i].x2,frame.y+frames[i].y2,GREY_1);
	}
}

void WINDOW::def_frame(int x1,int y1,int x2,int y2)
{
	frames[num_frames].x1=x1;
	frames[num_frames].y1=y1;
	frames[num_frames].x2=x2;
	frames[num_frames].y2=y2;

	num_frames++;
}

