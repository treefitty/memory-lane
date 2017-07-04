#include <stdio.h>
#include <conio.h>

int main()
{
	int x;

	clrscr();
	for (x = 1; x <= 12; x++)
	{
		printf("\nNumber - %d",x);
		printf(" - has the inversion %0.5d",(1/x));
	}
	getch();
	return 0;
}