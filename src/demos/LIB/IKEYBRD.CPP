#include <bios.h>

void flushkeybrd(void)
{
	while(_bios_keybrd(_KEYBRD_READY)!=0)
		 {
		 _bios_keybrd(_KEYBRD_READ);
	}
}

int poll_keybrd(void)
{
	int key;

	if(_bios_keybrd(_KEYBRD_READY!=0))
		{
		key=_bios_keybrd(_KEYBRD_READ);
		return key;
	}
	else return 0;
}