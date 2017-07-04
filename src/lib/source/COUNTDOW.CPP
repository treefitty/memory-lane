#include <conio.h>
#include <stdio.h>

#define min -5
#define max 7

int main()
{
	int x;

	clrscr();
	printf("This program counts down from positive\n");
	printf("7 to negative 5 printing all values.\n\n");
	for(x = max; x>= min; x--)
	{
		printf("\nx = %d",x);
	}
	printf("\n\nPress any key to continue...");
	getch();
	return 0;
}