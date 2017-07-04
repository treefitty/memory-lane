#include <stdio.h>
#include <bios.h>
#include <ctype.h>
#include <conio.h>

#define RIGHT  0x01
#define LEFT   0x02
#define CTRL   0x04
#define ALT    0x08

char ch;

int main(void)
{
  clrscr();
  for (;;){
  int key, modifiers;

  /* Wait until a key is pressed */
  while (_bios_keybrd(_KEYBRD_READY) == 0);


  /* Fetch the key that is waiting */
  key = _bios_keybrd(_KEYBRD_READ);

  /* Determine if shift keys are used */
  modifiers = _bios_keybrd(_KEYBRD_SHIFTSTATUS);
  if (modifiers)
  {
    printf("[");
    if (modifiers & RIGHT) printf("RIGHT");
    if (modifiers & LEFT)  printf("LEFT");
    if (modifiers & CTRL)  printf("CTRL");
    if (modifiers & ALT)   printf("ALT");
    printf("]");
  }

  /* print out the character read */
  if (isalnum(key & 0xFF))
    printf("%#02x\n", key);
  else
    printf("%#02x\n", key);
    printf("Another? Y/N : ");
    ch= getch();
    if ((ch== 'Y')||(ch== 'y')) printf("\n\n");
    else return 0;
  }
}

