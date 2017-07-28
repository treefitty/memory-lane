#include "global.h"

#include "spc_inv1.h"

//
// Global variables
//
star stars[MAX_STARS];
player plr_1;
enemy enemy_ships[MAX_X_SHIPS*MAX_Y_SHIPS];

//
// Local variables
//
static signed char direction;
static int drop_count=0;
static int start_y;
static char enemy_delay;
static char enemy_speed;
static char enemy_dead;

int  get_plr1_bullet(void)
{
	int i;

	//**************************************
	// Loop through all bullets and return
	// any inactive bullet's array index
	for(i=0;i<MAX_BULLETS;i++)
		{
		if(plr_1.bullets[i].active==0) return i;
	}
	return 0xff;
}

int	get_enemy_bullet(int enemy)
{
	int i;

	//**************************************
	// Loop through all bullets and return
	// any inactive bullet's array index
	for(i=0;i<MAX_BULLETS;i++)
		{
		if(enemy_ships[enemy].bullets[i].active==0) return i;
	}
	return 0xff;
}

// Sets up a grid of enemy along the top of the screen
void setup_grid(void)
{
	int i,x;

	//********************************
	// Setup top row of enemy ships
	for(i=0,x=10;i<MAX_X_SHIPS;i++,x+=SHIP_WIDTH+5)
		{
		enemy_ships[i].x=x;
		enemy_ships[i].y=start_y;
		enemy_ships[i].ship_data=enemy1_ship;
		enemy_ships[i].bullet_data=enemy1_bullet;
		enemy_ships[i].bullet_tip_x=0;
		enemy_ships[i].bullet_tip_y=7;
	}
	//********************************
	// Setup second of enemy ships
	for(i=0,x=10;i<MAX_X_SHIPS;i++,x+=SHIP_WIDTH+5)
		{
		enemy_ships[i+MAX_X_SHIPS].x=x;
		enemy_ships[i+MAX_X_SHIPS].y=SHIP_HEIGHT+5+start_y;
		enemy_ships[i+MAX_X_SHIPS].ship_data=enemy2_ship;
		enemy_ships[i+MAX_X_SHIPS].bullet_data=enemy2_bullet;
		enemy_ships[i].bullet_tip_x=1;
		enemy_ships[i].bullet_tip_y=4;
	}
	//********************************
	// Setup third row of enemy ships
	for(i=0,x=10;i<MAX_X_SHIPS;i++,x+=SHIP_WIDTH+5)
		{
		enemy_ships[i+(MAX_X_SHIPS*2)].x=x;
		enemy_ships[i+(MAX_X_SHIPS*2)].y=((SHIP_HEIGHT+5)*2)+start_y;
		enemy_ships[i+(MAX_X_SHIPS*2)].ship_data=enemy3_ship;
		enemy_ships[i+(MAX_X_SHIPS*2)].bullet_data=enemy3_bullet;
		enemy_ships[i].bullet_tip_x=0;
		enemy_ships[i].bullet_tip_y=5;
	}
}

void refresh_plr_lives(void)
{
	int i,x;

	for(i=0,x=0;i<plr_1.lives;i++,x+=13)
		{
		x_put_pbm(x,0,small_ship);
	}
}

void update_enemy_ships(void)
{
	int i,j,k;
	int enemy_x,enemy_y;
	int bullet_x,bullet_y;
	int plr_x,plr_y;

	if(enemy_delay==0)
		{
		for(i=0;i<MAX_X_SHIPS*MAX_Y_SHIPS;i++)
			{
			if(enemy_ships[i].active==1)
				{
				//**********************************
				// Handle enemy retaliation.
				// Randomly based when deciding how
				// often to fire.
 				if(random(150)==1)
					{
					j=get_enemy_bullet(i);
					if(i!=0xff)
						{
						enemy_ships[i].bullets[j].active=1;
						enemy_ships[i].bullets[j].y=enemy_ships[i].y+SHIP_HEIGHT+enemy_ships[i].bullet_tip_y;
						enemy_ships[i].bullets[j].inc=2;
						enemy_ships[i].delay_count=enemy_ships[i].cannon_delay;

						if(enemy_ships[i].cannon==0)
							{
							enemy_ships[i].bullets[j].x=enemy_ships[i].x+enemy_ships[i].bullet_tip_x;
							enemy_ships[i].cannon=1;
						}
						else {
							enemy_ships[i].bullets[j].x=enemy_ships[i].x+SHIP_WIDTH-3+enemy_ships[i].bullet_tip_x;
							enemy_ships[i].cannon=0;
						}
					}
				}
				if(enemy_ships[i].delay_count!=0) enemy_ships[i].delay_count--;

				//**********************************
				// Check and see if it's time to
				// move down a line
				if((enemy_ships[i].x+SHIP_WIDTH>=320-direction || enemy_ships[i].x<=-direction) && drop_count==0)
					{
					direction=direction<0 ? enemy_speed : -enemy_speed;
					drop_count=DROP_AMOUNT;
				}

				//**********************************
				// Check players bullets didn't hit
				enemy_x=enemy_ships[i].x;
				enemy_y=enemy_ships[i].y;
				for(k=0;k<MAX_BULLETS;k++)
					{
					bullet_x=plr_1.bullets[k].x;
					bullet_y=plr_1.bullets[k].y;

					if(bullet_x>enemy_x && bullet_x<enemy_x+SHIP_WIDTH &&
						bullet_y>enemy_y && bullet_y<enemy_y+SHIP_HEIGHT)
						{
						enemy_ships[i].active=0;
						plr_1.bullets[k].active=0;
						plr_1.bullets[k].x=0;
						plr_1.bullets[k].y=0;
						if(((++enemy_dead) % 6)==0) enemy_speed++;
					}
				}
			}
			//*******************************
			// Update enemy bullets already
			// fired
			plr_x=plr_1.x;
			plr_y=plr_1.y;
			for(j=0;j<MAX_BULLETS;j++)
				{
				if(enemy_ships[i].bullets[j].active==1)
					{
					enemy_ships[i].bullets[j].y+=enemy_ships[i].bullets[j].inc;
					if(enemy_ships[i].bullets[j].y>190) enemy_ships[i].bullets[j].active=0;
					x_put_pbm(enemy_ships[i].bullets[j].x,enemy_ships[i].bullets[j].y,enemy_ships[i].bullet_data);

					bullet_x=enemy_ships[i].bullets[j].x;
					bullet_y=enemy_ships[i].bullets[j].y;
					if(bullet_x>plr_x && bullet_x<plr_x+SHIP_WIDTH &&
						bullet_y>plr_y && bullet_y<plr_y+SHIP_HEIGHT)
						{
						if(plr_1.lives!=0) plr_1.lives--;
						enemy_ships[i].bullets[j].active=0;
						enemy_ships[i].bullets[j].x=0;
						enemy_ships[i].bullets[j].y=0;
					}
				}
			}
		}
		enemy_delay=ENEMY_WAIT;
	}
	else enemy_delay--;

	//*************************************
	// Update all enemy ships on screen
	for(i=0;i<MAX_X_SHIPS*MAX_Y_SHIPS;i++)
		{
		if(enemy_delay==0)
			{
			if(drop_count!=0) enemy_ships[i].y++;
			enemy_ships[i].x+=direction;
		}
		if(enemy_ships[i].active==1)
			{
			x_put_pbm(enemy_ships[i].x,enemy_ships[i].y,enemy_ships[i].ship_data);
		}
	}
	if(drop_count!=0) drop_count--;
}

void update_plr1(void)
{
	int i;

	//**********************************
	// Update players bullets already
	// fired
	for(i=0;i<MAX_BULLETS;i++)
		{
		if(plr_1.bullets[i].active==1)
			{
			plr_1.bullets[i].y+=plr_1.bullets[i].inc;
			if(plr_1.bullets[i].y<3) plr_1.bullets[i].active=0;
			x_put_pbm(plr_1.bullets[i].x,plr_1.bullets[i].y,plr_1.bullet_data);
		}
	}
	//**********************************
	// Update player on the screen
	x_put_pbm(plr_1.x,plr_1.y,plr_1.ship_data);
}

void spc_inv_game_loop(void)
{
	int i,j;
	char done,active_pg;

	//******************
	// Setup player 1
	plr_1.x=153;
	plr_1.y=180;
	plr_1.cannon=0;
	plr_1.cannon_delay=25;
	plr_1.delay_count=0;
	plr_1.ship_data=plr1_ship;
	plr_1.bullet_data=plr_bullet;
	plr_1.lives=3;
	plr_1.bullet_tip_x=1;
	plr_1.bullet_tip_y=0;
	for(i=0;i<MAX_BULLETS;i++)
		{
		plr_1.bullets[i].active=0;
	}
	//******************

	//******************
	// Setup enemy ships
	for(i=0;i<MAX_X_SHIPS*MAX_Y_SHIPS;i++)
		{
		enemy_ships[i].x=0;
		enemy_ships[i].y=0;
		enemy_ships[i].cannon=0;
		enemy_ships[i].cannon_delay=1;
		enemy_ships[i].delay_count=0;
		enemy_ships[i].ship_data=enemy1_ship;
		enemy_ships[i].bullet_data=enemy1_bullet;
		enemy_ships[i].active=1;

		for(j=0;j<MAX_BULLETS;j++)
			{
			enemy_ships[i].bullets[j].active=0;
		}
	}
	//******************

	x_setmode();                                // Set mode-x mode
	x_set_palette(spc_pal);                     // Load palette
	x_set_double_buffer();                      // Setup double buffer

	x_set_active_page(0);                       // Make sure we draw to page 0
	x_put_pbm(plr_1.x,plr_1.y,plr_1.ship_data); // Put player to screen

	x_set_rgb(15,63,63,63);                     // Make changes to palette
	x_set_rgb(16,53,53,53);                     // for background stars
	x_set_rgb(17,35,35,35);
	x_set_rgb(18,23,23,23);

	start_y=11;						    // Enemy grid starts at 11
	init_stars();                               // Setup background star field
	setup_grid();                               // Setup enemy grid
	set_kb();                                   // Set keyboard ISR

	direction=1;                                // Start with grid moving right
	active_pg=1;                                // Start drawing on hidden page
	enemy_speed=1;
	done=0;
	while(!done)
		{
		x_set_active_page(active_pg);
		x_clr_page();                          // Clear hidden page

		update_stars();                        // Update background stars

		if(poll_user()==1) done=1;             // If the user wants out

		update_plr1();
		update_enemy_ships();                  // Update players bullets and ship
		refresh_plr_lives();                   // Draw players lives at top of screen

		x_flip_page();                         // Show the hidden page
		active_pg=(active_pg+1) & 1;           // Make other page the drawing page
		x_wait_vr();                           // Wait one vertical retrace
	}
	reset_kb();
}