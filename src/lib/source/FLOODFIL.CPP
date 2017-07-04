 #include <graphics.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <conio.h>

 int main(void)
 {
    /* request auto detection */
    int gdriver = DETECT, gmode, errorcode;
    int maxx, maxy;

    /* initialize graphics, local variables
*/
    initgraph(&gdriver, &gmode, "\\borlandc\\bgi");

    /* read result of initialization */
    errorcode = graphresult();
    if (errorcode != grOk)
    /* an error occurred */
    {
       printf("Graphics error: %s\n", grapherrormsg(errorcode));
       printf("Press any key to halt:");
       getch();
       exit(1);
       /* terminate with an error code */
    }

    maxx = getmaxx();
    maxy = getmaxy();

    /* select drawing color */
    setcolor(RED);

    /* select fill color */
    setfillstyle(SOLID_FILL, RED);

    /* draw a border around the screen */
    rectangle(0, 0, maxx, maxy);

    /* draw some circles */
    circle(maxx / 3, maxy /2, 50);
    setcolor(GREEN);
    circle(maxx / 2, 20, 100);
    circle(maxx-20, maxy-50, 75);
    circle(20, maxy-20, 25);

    /* wait for a key */
    getch();

    /* fill in bounded region */
    floodfill(maxx/3, maxy/2, RED);

    /* clean up */
    getch();
    closegraph();
    return 0;
 }