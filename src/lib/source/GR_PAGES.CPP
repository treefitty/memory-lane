#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int main(void)
{
  /* select a driver and mode that supports multiple pages. */
  int gdriver = EGA, gmode = EGAHI, errorcode;
  int x, y, ht;

  /* initialize graphics and local variables */
  initgraph(&gdriver, &gmode, "\\borlandc\\bgi");

  /* read result of initialization */
  errorcode = graphresult();
  if (errorcode != grOk)  /* an error occurred */
  {
    printf("Graphics error: %s\n", grapherrormsg(errorcode));
    printf("Press any key to halt:");
    getch();
    exit(1); /* terminate with an error code */
  }

  x = getmaxx() / 2;
  y = getmaxy() / 2;
  ht = textheight("W");

  /*  select the off screen page for drawing */
  setactivepage(1);

  /* draw a line on page #1 */
  line(0, 0, getmaxx(), getmaxy());

  /* output a message on page #1 */
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  outtextxy(x, y, "This is page #1:");
  outtextxy(x, y+ht, "Press any key to halt:");

  /* select drawing to page #0 */
  setactivepage(0);

  /* output a message  on page #0 */
  outtextxy(x, y, "This is page #0.");
  outtextxy(x, y+ht, "Press any key to view page #1:");
  getch();

  /* select page #1 as the visible page */
  setvisualpage(1);

  /* clean up */
  getch();
  closegraph();
  return 0;
}
