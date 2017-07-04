#include <conio.h>

class test1{
public:
	int a;
} obj1, obj2;

void main()
{
	int temp;

	clrscr();
	obj1.a= 2;
	obj2.a= 10;
	temp= obj1.a + obj2.a;
	cprintf("A + A = %i",temp);
	getch();
}