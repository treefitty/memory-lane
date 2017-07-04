
#include "AN_DEFS.H"

void TerminalError(char code,char *filename)
{
	asm mov	ax,0x03
	asm int	0x10
	textcolor(WHITE);

	switch(code)
		{
		case NOMEM :
			{
			cprintf("Out of memory!\n\r");
			cprintf("Try unloading some TSR's or refer\n\r");
			cprintf("to documentation for futher tips.\n\n\r");
			break;
		}
		case BADFILE :
			{
			cprintf("Corrupt data file or file not found: %s\n\r",filename);
			break;
		}
	}
	asm mov	ax,0x4c00
	asm int	0x21
}
