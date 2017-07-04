#include <graphics.h>
#include <conio.h>

int huge detectVGA256(void);

int huge detectVGA256(void)
{
   int driver, mode;

   detectgraph(&driver, &mode);
   return mode;
}

void main(void)
{
	int gdriver, gmode;

	gdriver= installuserdriver("SVGA256", detectSuperVGA256);
	gdriver= DETECT;

	initgraph(&gdriver,&gmode,"\\borlandc\\bgi");
	setcolor(126);
	bar(0,0,20,20);
	getch();
	closegraph();
}