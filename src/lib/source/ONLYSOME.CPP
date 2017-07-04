#include <stdio.h>
#include <conio.h>

int main()
{
	int x;

	clrscr();
	printf("This program counts to 100 only\n");
	printf("printing numbers between 32 and 39\n\n");
	for (x = 0; x <= 100; x++)
	{
		if (x<=39 && x>=32) {
			printf("The number - %d",x);
			printf(" - is <32 and >39\n");
		}
	}
	printf("\nPress any key to continue....");
	getch();
	return 0;
}