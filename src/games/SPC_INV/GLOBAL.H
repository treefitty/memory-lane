#include <stdlib.h>

#include <x_lib.h>
#include <key.h>
#include <ch4_lib.h>

//
// Constant definitions
//
#define LEFT_ARROW 		0x4b
#define RIGHT_ARROW		0x4d
#define ESCAPE			0x01
#define SPACE			0x39

#define MAX_STARS		70
#define MAX_BULLETS		15
#define MAX_X_SHIPS 	11
#define MAX_Y_SHIPS       3
#define DROP_AMOUNT		10
#define ENEMY_WAIT		 1

#define SHIP_WIDTH		20
#define SHIP_HEIGHT		17

//
// Typedefs
//
typedef struct {

	int x,y;
	int tip_x,tip_y;
	signed char inc;
	char active;

} bullet;

typedef struct {

	char far *ship_data;
	char far *bullet_data;
	int x,y;                           // Position on screen
	int bullet_tip_x,bullet_tip_y;	// Tip of bullet (for faster lookup)
	char cannon_delay;			     // Count down to delay cannon firing
								// (time between each round)
	char delay_count;
	char cannon;					// Firing cannon - 0,1 or 2 (both)
	char lives;                        // Lives left
	bullet bullets[MAX_BULLETS];		// Structure to hold all bullets on
								// the screen
} player;

typedef struct {

	char type;					// Shows level of intellegence

	char far *ship_data;			// Bitmap of ship
	char far *bullet_data;			// Bitmap of bullet
	int x,y;						// Screen position of ship
	char cannon_delay;				// ... See player definition
	char delay_count;                  //      "    "       "
	char cannon;					//      "    "       "
	int bullet_tip_x,bullet_tip_y;	//      "    "       "
	char active;					// Is the enemy alive or dead?
	bullet bullets[MAX_BULLETS];		// Structure to hold all bullets on
								// the screen
} enemy;

typedef struct {

	int x,y;
	char col;
	char delay;
	char first_delay;

} star;

//
// Procedure prototypes
//
void init_stars(void);
void update_stars(void);
void spc_inv_game_loop(void);
int  get_plr1_bullet(void);
int  poll_user(void);

//
// Global variable access declarations
//
extern star stars[MAX_STARS];
extern player plr_1;
extern enemy enemy_ships[MAX_X_SHIPS*MAX_Y_SHIPS];
extern char keyflags[128];

