#include <conio.h>
#include <stdio.h>
#include <string.h>

int main()
{
	char array1[3], array2[3], array3[5], arrays[13];

	clrscr();
	printf("This program sets up four arrays of\n");
	printf("type char and copies the numbers one\n");
	printf("one, two and three into the fist three\n");
	printf("arrays, and then combines them into the\n");
	printf("last array\n\n");
	strcpy(array1,"one");
	strcpy(array2,"two");
	strcpy(array3,"three");
	strcat(arrays,array1);
	strcat(arrays," ");
	strcat(arrays,array2);
	strcat(arrays," ");
	strcat(arrays,array3);
	printf("\nThe first array holds the string  - %s",array1);
	printf("\nThe second array holds the string - %s",array2);
	printf("\nThe third array holds the string  - %s",array3);
	printf("\n\nThe final array holds all numbers - %s",arrays);
	getch();
	return 0;
}