#include "global.h"

void init_stars(void)
{
	int i;

	randomize();
	for(i=0;i<MAX_STARS;i++)                    // Init background stars
		{
		stars[i].x=random(320);
		stars[i].y=random(200);
		stars[i].delay=random(4)+1;
		stars[i].first_delay=stars[i].delay;
		if(stars[i].delay==1) stars[i].col=15;
		if(stars[i].delay==2) stars[i].col=16;
		if(stars[i].delay==3) stars[i].col=17;
		if(stars[i].delay==4) stars[i].col=18;
	}
}

void update_stars(void)
{
	int i;

	for(i=0;i<MAX_STARS;i++)               // Put stars to hidden page
		{
		if(stars[i].delay==0)
			{
			stars[i].y++;
			stars[i].delay=stars[i].first_delay;
		}
		else stars[i].delay--;
		if(stars[i].y>195)
			{
			stars[i].x=random(320);
			stars[i].y=0;
			stars[i].delay=stars[i].first_delay;
		}
		x_set_pel(stars[i].x,stars[i].y,stars[i].col);
	}
}