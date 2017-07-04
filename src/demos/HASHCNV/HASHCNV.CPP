#include <conio.h>
#include <stdio.h>

void main(void)
{
	char filename[15];
	FILE *in,*out;
	int ch;

	clrscr();
	cprintf("Enter file name : ");
	cscanf("%s",filename);

	if((in=fopen(filename, "rt"))== NULL)
		{
		cprintf("Cannot open input file.\n");
		asm mov	ax,0x4c00
		asm int	0x21
	}
	if((out=fopen("convert.txt", "wt"))== NULL)
		{
		cprintf("Cannot open output file.\n");
		asm mov	ax,0x4c00
		asm int	0x21
	}
	while(!feof(in))
		{
		ch=fgetc(in);
		if(ch=='#')
			{
			fputc('\n',out);
		}
		else if(ch=='%')
			{
			fputc('"',out);
		}
		else {
			fputc(ch,out);
		}
	}
	fcloseall();
}