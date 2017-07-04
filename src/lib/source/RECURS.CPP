#include <conio.h>
#include <stdio.h>

int Sum(int num)
{
	if(num==0) return 0;

	return num + Sum(num-1);
}

void main(void)
{
	clrscr();
	printf("Sum is : %d",Sum(410));
	getch();
}