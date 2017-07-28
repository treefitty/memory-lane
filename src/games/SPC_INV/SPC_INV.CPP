#include "global.h"

//
// Global variables
//
char keyflags[128];

void main(void)
{
	spc_inv_game_loop();

	asm mov	ax,0x03
	asm int	0x10
}