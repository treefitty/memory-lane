#include <stdio.h>
#include <conio.h>

#define square(x) ((x)*(x))

int main()
{
	int x, number, sqrnum, sumnum, sumsqr;

        clrscr();
	printf("This program produces the\n");
	printf("number and it's square.\n\n");
	sumnum = 0;
	sumsqr = 0;
	for (x=0; x<= 12; x++)
	{
		number = x;
		sumnum = sumnum + x;
		printf("The square of %d",number);
		sqrnum = square(number);
		sumsqr = sumsqr + sqrnum;
		printf(" is %d\n",sqrnum);
	}
	printf("\nThe sum of the numbers is: %d",sumnum);
	printf("\n\nThe sum of the squares is: %d",sumsqr);
	printf("\n\nPress any key to continue....");
	getch();
        return(0);
}