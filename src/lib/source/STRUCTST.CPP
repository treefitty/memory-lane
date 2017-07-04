#include <conio.h>

struct test1{

	int a;
} obj1, obj2;

void main()
{
	int temp;

	obj1.a= 2;
	obj2.a= 10;
	temp= obj1.a + obj2.a;
	cprintf("A + A = %i",temp);
	getch();
}