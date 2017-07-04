#include "global.h"

int poll_user(void)
{
	int i;

	if(keyflags[ESCAPE]==1)                // Abort if escape was pressed
		{
		return 1;
	}
	if(keyflags[LEFT_ARROW]==1)
		{
		if(plr_1.x>1) plr_1.x-=2;
	}
	if(keyflags[RIGHT_ARROW]==1)
		{
		if(plr_1.x<319-SHIP_WIDTH) plr_1.x+=2;
	}
	if(keyflags[SPACE]==1)
		{
		if(plr_1.delay_count==0)
			{
			i=get_plr1_bullet();
			if(i!=0xff)
				{
				plr_1.bullets[i].active=1;
				plr_1.bullets[i].y=plr_1.y-4;
				plr_1.bullets[i].inc=-2;
				plr_1.delay_count=plr_1.cannon_delay;

				if(plr_1.cannon==0)
					{
					plr_1.bullets[i].x=plr_1.x;
					plr_1.cannon=1;
				}
				else
					{
					plr_1.bullets[i].x=plr_1.x+SHIP_WIDTH-3;
					plr_1.cannon=0;
				}
			}
		}
		plr_1.delay_count--;
	}
	return 0;
}