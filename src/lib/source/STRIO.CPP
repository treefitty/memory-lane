#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

main()
{
	FILE *file_ptr;
	char store[80];
	file_ptr= fopen("test.txt","rt");
	if(file_ptr== NULL)
		{
		clrscr();
		printf("Open failed.");
		exit(1);
		}
	clrscr();
	while(NULL!= fgets(store,80,file_ptr))
		fputs(store,stdout);
	fclose(file_ptr);
        return 0;
}