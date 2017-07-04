int stangle= 0, endangle= 360;
	char number[6];
	struct textsettingstype textinfo;

	pmouse.hidemouse();

	setfillstyle(1,CARPETCOLOUR);
	floodfill(1,1,CARPETCOLOUR);  /* whole area */
	setcolor(2);

	for ( i = 0, j = 0, k = 0; i <= maxx;  )
	{
	     if ( i < TABLE_LEFT-CUSH_WIDTH || i > TABLE_RIGHT+CUSH_WIDTH )
		  {
		     line(i,0,i,maxy);
		  }
	     else
		  {
		     line(i,0,i,TABLE_TOP-CUSH_WIDTH);
		     line(i,TABLE_BOTTOM+CUSH_WIDTH,i,maxy);
		  }

	     if ( j > 2 )
		 k = 1;
	     if ( j < 1 )
		 k = 0;
	     if ( k == 0 )
		 j++;
	     else
		 j--;

	     i += j;
	}

	for ( i = 0,j = 0, k = 0; i <= maxy;  )
	{
	     if ( i < TABLE_TOP-CUSH_WIDTH || i > TABLE_BOTTOM+CUSH_WIDTH )
		  {
		     line(0,i,maxx,i);
		  }
		else
		  {
		     line(0,i,TABLE_LEFT-CUSH_WIDTH,i);
		     line(TABLE_RIGHT+CUSH_WIDTH,i,maxx,i);
		  }

	     if ( j > 2 )
		  k = 1;
	     if ( j < 1 )
		  k = 0;
		if ( k == 0 )
		  j++;
	     else
		  j--;

	     i += j;
	}
	/* Draw Copyright window */

	scr->draw_window(maxx-320,maxy-70,maxx-30,maxy-30);
	setcolor(11);
	settextstyle(2,0,0);
	setusercharsize(2,2,2,2);
	settextjustify(2,2);
	outtextxy(maxx-75,maxy-56,"Copyright 1994 by Paul Carroll.");
	settextstyle(0,0,0);

	/* Draw EXIT button */

	setviewport(TABLE_LEFT-5,TABLE_TOP-5,TABLE_RIGHT+5,TABLE_BOTTOM+5,1);
	getviewsettings(&table_view);

	setpalette(2,0);/* show lines(drawn as 2=carpetcolour)as black now */

	setfillstyle(1,RED);   /* Draw wood around  table */
	bar(0,0,TABLE_RIGHT,TABLE_BOTTOM);

	setviewport(CARPET_LEFT-4,CARPET_TOP-4,CARPET_RIGHT+4,CARPET_BOTTOM+4,1);
	getviewsettings(&carpet_view);
	clearviewport();
	setcolor(2);
	rectangle(0,0,CARPET_RIGHT,CARPET_BOTTOM);
	setcolor(LIGHTRED);
	line(CARPET_RIGHT-12,0,CARPET_RIGHT-12,CARPET_BOTTOM-12);
	line(0,CARPET_BOTTOM-12,CARPET_RIGHT-12,CARPET_BOTTOM-12);

	/* functions to draw the pockets, lines and spots */

	setviewport(TABLE_LEFT,TABLE_TOP,TABLE_RIGHT,TABLE_BOTTOM,0);

	/* Gold rings for all pockets */

	setcolor(YELLOW);
	circle(2,2,6); /* Top left pocket */
	circle(2,2,7);
	circle(TABLE_WIDTH-2,2,6); /* Top right pocket */
	circle(TABLE_WIDTH-2,2,7);
	circle(2,TABLE_HEIGHT-2,6); /* Bottom left pocket */
	circle(2,TABLE_HEIGHT-2,7);
	circle(TABLE_WIDTH-2, TABLE_HEIGHT-2,6); /* Bottom right pocket */
	circle(TABLE_WIDTH-2, TABLE_HEIGHT-2,7);
	circle(-2,TABLE_HEIGHT/2,6); /* Middle left pocket */
	circle(-2,TABLE_HEIGHT/2,7);
	circle(TABLE_WIDTH+2,TABLE_HEIGHT/2,6); /* Middle right pocket */
	circle(TABLE_WIDTH+2,TABLE_HEIGHT/2,7);

	/* Black insides of four corner pockets: */

	setcolor(2);
	setfillstyle(1,2);
	pieslice(3,3,stangle,endangle,6);
	pieslice(TABLE_WIDTH-3,3,stangle,endangle,6);
	pieslice(3,TABLE_HEIGHT-3,stangle,endangle,6);
	pieslice(TABLE_WIDTH-3,TABLE_HEIGHT-3,stangle,endangle,6);
	pieslice(0,TABLE_HEIGHT/2,stangle,endangle,6);
	pieslice(TABLE_WIDTH,TABLE_HEIGHT/2,stangle,endangle,6);

	/* Draw cushions around table */

	setviewport(carpet_view.left,carpet_view.top,carpet_view.right
				,carpet_view.bottom,1);
	setcolor(DARKGRAY); /* Cushions for TOP and LEFT of table */
	line(CUSH_WIDTH,CUSH_WIDTH+10,CUSH_WIDTH,(CARPET_LENGTH/2)-8);
	line(CUSH_WIDTH,(CARPET_LENGTH/2)+16,CUSH_WIDTH,CARPET_LENGTH-10);
	line(CUSH_WIDTH+10,CUSH_WIDTH,CARPET_WIDTH-10,CUSH_WIDTH);
	/* Lines joining cushions to wood, LEFT and TOP of table */
	line(CUSH_WIDTH,CUSH_WIDTH+10,0,CUSH_WIDTH+2);
	line(CUSH_WIDTH,(CARPET_LENGTH/2)-8,0,(CARPET_LENGTH/2)-2);
	line(CUSH_WIDTH,(CARPET_LENGTH/2)+16,0,(CARPET_LENGTH/2)+10);
	line(CUSH_WIDTH,CARPET_LENGTH-10,0,CARPET_LENGTH-2);
	line(CUSH_WIDTH+10,CUSH_WIDTH,CUSH_WIDTH+2,0);
	line(CARPET_WIDTH-10,CUSH_WIDTH,CARPET_WIDTH-1,0);

	setcolor(LIGHTGREEN); /* Cushions for RIGHT and BOTTOM of table */
	line(CARPET_WIDTH+2,CUSH_WIDTH+10,CARPET_WIDTH+2,(CARPET_LENGTH/2)-8);
	line(CARPET_WIDTH+2,(CARPET_LENGTH/2)+16,CARPET_WIDTH+2,CARPET_LENGTH-10);
	line(CUSH_WIDTH+10,CARPET_LENGTH+2,CARPET_WIDTH-10,CARPET_LENGTH+2);
	/* Lines joining cushions to wood, RIGHT and BOTTOM of table */
	line(CARPET_WIDTH+2,CUSH_WIDTH+10,CARPET_WIDTH+8,CUSH_WIDTH+2);
	line(CARPET_WIDTH+2,(CARPET_LENGTH/2)-8,CARPET_WIDTH+8,(CARPET_LENGTH/2)-2);
	line(CARPET_WIDTH+2,(CARPET_LENGTH/2)+16,CARPET_WIDTH+8,(CARPET_LENGTH/2)+11);
	line(CARPET_WIDTH+2,CARPET_LENGTH-10,CARPET_WIDTH+8,CARPET_LENGTH-2);
	line(CARPET_WIDTH-10,CARPET_LENGTH+2,CARPET_WIDTH,CARPET_LENGTH+8);
	line(CUSH_WIDTH+10,CARPET_LENGTH+2,CUSH_WIDTH+2,CARPET_LENGTH+8);

	drawspots();
	setviewport(0,0,maxx,maxy,1);
	scr->draw_window(maxx-320,maxy-270,maxx-30,maxy-90);
	scr->button_up(maxx-300,maxy-120,maxx-50,maxy-100,"Exit To Main Menu.");
	setcolor(2);
	setfillstyle(1,2);
	bar(maxx-300,maxy-200,maxx-50,maxy-190);
	setcolor(EGA_DARKGRAY);
	for(i=maxx-300,k=0;i<=maxx-50;i+=25,k++)
		{
		itoa(k,number,6);
		outtextxy(i,maxy-212,number);
		line(i,maxy-205,i,maxy-200);
		}
	scr->button_up(maxx-290,maxy-170,maxx-180,maxy-150,"<< Decrease");
	scr->button_up(maxx-170,maxy-170,maxx-60,maxy-150,"Increase >>");
	gettextsettings(&textinfo);
	settextstyle(8,0,4);
	setcolor(RED);
	outtextxy(maxx-175,maxy-245,"Power Setting.");
	setviewport(carpet_view.left+7,carpet_view.top+7,carpet_view.right-7
				,carpet_view.bottom-7,1);
	pmouse.showmouse();