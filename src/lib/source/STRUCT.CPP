#include <conio.h>
#include <stdio.h>

void main()
{
	struct A{
	     int j;
	     double x;
	};

	struct B{
	     int i;
	     double d;
	} s, *sptr;

        clrscr();
	s.i= 3;
	sptr->d= 1.23;
	printf("Value of i is : %i\n\n",s.i);
	printf("Value of d is : %d",sptr->d);
        while(!kbhit());
}
