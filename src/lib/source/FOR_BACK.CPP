#include <stdio.h>
#include <conio.h>
#include <string.h>

#define num_letters 12

int main()
{
	char string_prn[12], *ptr_string;
	int xmin, xmax;

	clrscr();
	printf("Using a string of 12 characters this\n");
	printf("program demonstrates some different\n");
	printf("effects that can be achieved by printing\n");
	printf("one character at a time.\n\n");
	strcpy(string_prn,"Paul Carroll");
	printf("String to print is: %s",string_prn);
	printf("\n\nString printed one character at a time:\n\n");
	for (xmin=0; xmin<=num_letters ; xmin++)
	{
		*ptr_string = string_prn[xmin];
		printf("%c\n",*ptr_string);
	}
	printf("\nPress any key to continue...");
	getch();
	clrscr();
	printf("Or it could have been printed like this... \n");
	for (xmax=0; xmax<=num_letters; xmax++)
	{
		for(xmin=0; xmin<xmax; xmin++)
		{
			*ptr_string = string_prn[xmin];
			printf("%c",*ptr_string);
		}
		printf("\n");
	}
	printf("\nPress any key to continue....");
	getch();
	clrscr();
	printf("The string could have been printed backwards...\n\n");
	for (xmax=num_letters - 1; xmax>=0; xmax--)
	{
		*ptr_string = string_prn[xmax];
		printf("%c\n",*ptr_string);
	}
	printf("\n\nPress any key to continue....");
	getch();
	return 0;
}
