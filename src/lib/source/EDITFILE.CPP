#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <bios.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

const int rt_arrow= 0x4d00;
const int left_arrow= 0x4b00;
const int up_arrow= 0x4800;
const int down_arrow= 0x5000;
const int F1_key= 0x3b00;
const int F2_key= 0x3c00;
const int Enter= 0x1c0d;
const int Backspace= 0xe08;

extern int_wscroll;

static char *string_up= "abcdefghijklmnopqrstuvwxyz", *upper_str;
static char cont;
static char new_file_name[12];
static char string_read[80];
static char string_write[80];
static int key, x, y, i;
static int is_new;
static FILE *stream;

void status_line(void);
void new_file(void);

void status_line(void)
{
	clrscr();
	gotoxy(1,24);
	textcolor(GREEN);
	cprintf("<F1 Exit>  <F2 Save>");
	window(1,1,80,22);
}

void new_file(void)
{
	printf("Do you wish to create a new file? Y/N : ");
	cont= getch();
	if ((cont== 'N')|| (cont== 'n')) exit(0);
	else if ((cont== 'Y')|| (cont== 'y'))
	     {
	     printf("\n\nEnter name of file including extension,\n");
	     printf("if no extension is given then a blank\n");
	     printf("extension is assumed.\n\n");
	     printf("New file name : ");
	     gets(new_file_name);
	     printf("\nFile to create is : %s",new_file_name);
	     delay(1500);
	     is_new= TRUE;
	     }
}

int main(int argc, char *argv[])
{
	 char * *handle;

	clrscr();
	if (argc > 2) {
		      printf("Enter ONE file name only, and\n");
		      printf("no other paramters.");
		      exit(1);
		      }
	if (argv[1]== NULL)
	     {
	     printf("Correct usage of program:\n\n");
	     printf("EDITFILE <filename.ext>\n\n");
	     printf("Press any key to continue...\n");
	     while(!kbhit());
	     exit(1);
	     }
	if (argv[1]!= NULL)
	     {
	     printf("ASSEMBLY / TEXT FILE EDITOR.\n\n");
	     printf("Written by Paul Carroll\n");
	     printf("Copyright 1994. version 1.0\n\n");
	     upper_str= strupr(argv[1]);
	     printf("Attempting to read file: %s\n\n", upper_str);
	     }
	handle= &argv[1];
	stream= fopen(argv[1], "r+t");
	if (stream== NULL)
	     {
	     printf("\nError occurred while reading file\n");
	     printf("or file was not found.\n");
	     new_file();
	     if (is_new== TRUE) {
			       status_line();
			       gotoxy(1,1);
			       goto MAIN_LOOP;
			       }
	     clrscr();
	     }
	else {
	     printf("File read was successful.");
	     delay(2000);
	     clrscr();
	     }
	status_line();
	gotoxy(1,1);
	textcolor(LIGHTGRAY);
	do
	   {
	   cputs(string_read);
	   printf("\r");
	   }
	while(fgets(string_read, 80, stream)!= NULL);
	fclose( stream);
	goto MAIN_LOOP;

	MAIN_LOOP:
	_wscroll= 1;
	for (;;)
	     {
	     while(_bios_keybrd(_KEYBRD_READY)== 0);
	     key= _bios_keybrd(_KEYBRD_READ);
	     if (key== F1_key) {
			       window(1,1,80,24);
			       clrscr();
			       return 0;
			       }
	     if (key== F2_key) goto SAVE_FILE;
	     if (isalnum(key & 0xFF)) printf("%c",key);
	     if (key== Enter) cprintf("\r\n");
	     if (key== Backspace)
             	  {
		  x= wherex(); y= wherey();
                  gotoxy(x-1, y);
		  movetext(x, y, 80, y, x-1, y);
		  }
	     switch(key)
		  {
		  case rt_arrow:  {
				  x= (wherex())+ 1; y= wherey();
				  gotoxy(x,y);
				  break;
				  }
		  case left_arrow:  {
				    x= (wherex())-1; y= wherey();
				    gotoxy(x,y);
				    break;
				    }
		  case up_arrow: {
				 x= wherex(); y= (wherey())- 1;
				 gotoxy(x,y);
				 break;
				 }
		  case down_arrow: {
				   x= wherex(); y= (wherey())+ 1;
				   gotoxy(x,y);
				   break;
				   }
		  }
	     key= toascii(key);
	     if ((key<127)&&(key>122)) printf("%c",key);
	     if ((key<97)&&(key>90)) printf("%c",key);
	     if ((key<65)&&(key>57)) printf("%c",key);
	     if ((key<48)&&(key>30)) printf("%c",key);
	     }

	SAVE_FILE:
	     {
	     if (is_new== FALSE)
		  {
		  gotoxy(1,1);
		  //creat(S_IREAD | S_IWRITE);
		  fseek(stream,0,SEEK_SET);
		 /* do
		    {
		    fwrite
		    }
		  while(string_write!= NULL);
		  }
	     else if (is_new== TRUE)
		   {
		   gotoxy(1,1,);  */
		   }
	     }
	return 0;
}








