#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>

int main(void)
{
    int gdriver = DETECT, gmode, errorcode;
    int x, y;

    initgraph (&gdriver, &gmode, "\\borlandc\\bgi");
    errorcode = graphresult();
    if (errorcode != grOk)
    {
	printf("Critical Graphics Error: %s\n ",grapherrormsg(errorcode));
	printf("Press any key to Halt.");
	getch();
	exit(1);
    }                                
    x = getmaxx() / 2;
    y = getmaxy() / 2;
    setcolor(5);
    settextstyle(3,0,8);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(x,y,"TEST TEST TEST");
    while(!kbhit());
    closegraph();
    return 0;
}