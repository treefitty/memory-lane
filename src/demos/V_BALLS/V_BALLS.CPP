#include <bios.h>
#include <stdlib.h>
#include <math.h>

#include "balls.h"
#include "ball_dat.h"

#include <x_lib.h>

#define PI 		3.141592
#define NUM_BALLS 	14
#define MAX_BALLS 	14
#define MAX_OBJECTS 5

#define BOX 		0
#define TRIANGLE	1
#define SPHERE		2
#define CROSS		3
#define WAVE		4

#define esc		0x11b
#define space		0x3920

typedef long fixed_point;

#define int2fixed(a)  	(fixed_point)((fixed_point)(a) << 16)
#define fixed2int(a)  	(int)((a) >> 16)
#define float2fixed(a)  	((fixed_point)((a) * 65536.0))
#define fixed2float(a)  	((a) / 65536.0)

extern "C" {

	fixed_point fixed_mul(fixed_point,fixed_point);
	fixed_point fixed_div(fixed_point,fixed_point);
};

//*****************************************
//
// Typedefs
//
typedef struct {

	char flag;
	long wx,wy,wz;
	int sx,sy,sz;
	char col;

} ball_type;

typedef struct {

	ball_type balls[MAX_BALLS];

} object_type;

//*****************************************
//
// Local variable definitions
//
static object_type objects[MAX_OBJECTS];
static char *ball_list[3];
static int sort_list[15];
static long SIN[360],COS[360];
static char transform_flg;
static int post_x_add,post_y_add;
static char curr_obj=0;
static char next_obj=1;
static int balls_done;

//*****************************************
//
// Start code here....
//
void init_sort_list(void)
{
	// point each element to an initial ball
	for(int i=0;i<15;i++)
		{
		sort_list[i]=i;
	}
}

void init_sin_cos(void)
{
	int i;

	for(i=0;i<360;i++)                 // Pre-calc SIN, COS lookup tables
		{
		COS[i]=float2fixed(cos((float)(i*PI)/180.0));
		SIN[i]=float2fixed(sin((float)(i*PI)/180.0));
	}
}

int comp_objects(const void *a, const void *b)
{
	if(objects[curr_obj].balls[*(int *)a].sz < objects[curr_obj].balls[*(int *)b].sz)
		{
		return -1;
	}
	else if(objects[curr_obj].balls[*(int *)a].sz > objects[curr_obj].balls[*(int *)b].sz)
		{
		return +1;
	}
	else
		{
		return 0;
	}
}

void load_ball(object_type *dest,int *source,int elements)
{
	int i,k;

	for(i=0,k=0;i<elements;i++,k+=4)
		{
		dest->balls[i].wx  = int2fixed(source[k]);
		dest->balls[i].wy  = int2fixed(source[k+1]);
		dest->balls[i].wz  = int2fixed(source[k+2]);
		dest->balls[i].col = source[k+3];
	}
}

void update_balls(void)
{
	static int pt_yaw=0,pt_roll=0,pt_pitch=0;
	object_type *obj;
	ball_type ball;
	long xa,ya,za;
	long x,y,z;
	int i;

	obj=&objects[curr_obj];
	for(i=0;i<NUM_BALLS;i++)
		{
		x=obj->balls[i].wx;
		y=obj->balls[i].wy;
		z=obj->balls[i].wz;
		xa=fixed_mul(COS[pt_roll] ,x) + fixed_mul(SIN[pt_roll] ,y);
		ya=fixed_mul(COS[pt_roll] ,y) - fixed_mul(SIN[pt_roll] ,x);
		x =fixed_mul(COS[pt_yaw]  ,xa)- fixed_mul(SIN[pt_yaw]  ,z);
		za=fixed_mul(SIN[pt_yaw]  ,xa)+ fixed_mul(COS[pt_yaw]  ,z);
		z =fixed_mul(COS[pt_pitch],za)- fixed_mul(SIN[pt_pitch],ya);
		y =fixed_mul(SIN[pt_pitch],za)+ fixed_mul(COS[pt_pitch],ya);
		obj->balls[i].sz= fixed2int(z);
		obj->balls[i].sx= fixed2int(x)+post_x_add;
		obj->balls[i].sy= fixed2int(y)+post_y_add;
	}

	qsort((void *)sort_list,NUM_BALLS,sizeof(sort_list[0]),comp_objects);

	for(i=0;i<NUM_BALLS;i++)
		{
		ball=obj->balls[sort_list[i]];
		x_put_pbm(ball.sx,ball.sy,ball_list[ball.col]);
	}

	if((pt_roll +=3)>=360) pt_roll =0;
	if((pt_yaw  +=1)>=360) pt_yaw  =0;
	if((pt_pitch+=2)>=360) pt_pitch=0;
}

void transform_balls(void)
{
	ball_type *this_ball, next_ball;
	long dx,dy,dz;
	int i;

	for(i=0;i<NUM_BALLS;i++)
		{
		this_ball=&objects[curr_obj].balls[i];
		next_ball= objects[next_obj].balls[i];

		if(this_ball->flag==0)
			{
			dx=next_ball.wx-this_ball->wx;
			dy=next_ball.wy-this_ball->wy;
			dz=next_ball.wz-this_ball->wz;

			dx=dx>int2fixed(1) ? int2fixed(1) : (dx<-int2fixed(1) ? -int2fixed(1) : 0);
			dy=dy>int2fixed(1) ? int2fixed(1) : (dy<-int2fixed(1) ? -int2fixed(1) : 0);
			dz=dz>int2fixed(1) ? int2fixed(1) : (dz<-int2fixed(1) ? -int2fixed(1) : 0);

			if(dx==0 && dy==0 && dz==0)
				{
				this_ball->flag=1;
				balls_done++;
			}

			this_ball->wx+=dx;
			this_ball->wy+=dy;
			this_ball->wz+=dz;
		}
	}
}

int poll_user(void)
{
	int key;

	if(_bios_keybrd(_KEYBRD_READY)!=0)
		{
		key=_bios_keybrd(_KEYBRD_READ);

		switch(key)
			{
			case esc :
				{
				return 1;
			}
			case space :
				{
				transform_flg=1;

				break;
			}
		}
	}
	return 0;
}

void main(void)
{
	int vis_page=1;
	char done;
	int loops,i;

	x_setmode();
	x_set_double_buffer();
	x_set_palette(BallBltPal);

	load_ball(&objects[BOX],box_def,14);
	load_ball(&objects[TRIANGLE],triangle_def,14);
	load_ball(&objects[SPHERE],sphere_def,14);
	load_ball(&objects[CROSS],cross_def,14);
	load_ball(&objects[WAVE],wave_def,14);

	init_sort_list();
	init_sin_cos();
	ball_list[0]=GreenBlt;
	ball_list[1]=RedBlt;
	ball_list[2]=YellowBlt;

	post_x_add=145;
	post_y_add=90;
	done=0;
	transform_flg=0;

	loops=0;
	done=0;
	while(!done)
		{
		update_balls();                         // Calc and draw balls

		x_flip_page();                          // Show hidden page
		x_wait_vr();                            // Wait 1 vertical retrace
										// (time to all systems)
		vis_page=(++vis_page) & 1;      		// Alternate page 0 and 1
		x_set_active_page(vis_page);            // Set and
		x_clr_page();                           // clear hidden page

		if(++loops==500)
			{
			transform_flg=1;
		}
		if(transform_flg==1)
			{
			transform_balls();
		}
		if(poll_user()==1) done=1;

		if(balls_done==NUM_BALLS)
			{
			loops=0;
			transform_flg=0;
			balls_done=0;

			//curr_obj=(++curr_obj) % MAX_OBJECTS;
			//next_obj=(++next_obj) % MAX_OBJECTS;

			if(++curr_obj>=MAX_OBJECTS)
				{
				curr_obj=BOX;
			}
			if(++next_obj>=MAX_OBJECTS)
				{
				next_obj=BOX;
			}

			for(i=0;i<NUM_BALLS;i++)
				{
				objects[curr_obj].balls[i].flag=0;
			}
		}
	}

	asm mov	ax,0x03
	asm int	0x10
}










