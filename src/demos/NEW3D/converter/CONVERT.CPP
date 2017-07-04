#include <stdio.h>

#define SPACES 1

void main(void)
{
	FILE *in,*out;
	int ch, old_ch=0,old_old=0;
	int last_run=0;

	in=fopen("VW.GEM","rt");
	out=fopen("out.h","wt");

	while(!feof(in))
		{
		ch=fgetc(in);
		if((ch==0x09 || ch=='\n')&&old_ch!=0x09 && last_run!=SPACES)
			{
			fputc(',',out);
			last_run=0;
		}
		else if(ch==0x20)
			{
			fputc(',',out);
			fseek(in,3,1);
			last_run=SPACES;
		}
		if(ch==0x09&&old_ch>'0'&&old_ch<'9'&&old_old==0x0a)
			{
			fputc(',',out);
		}
		fputc(ch,out);
		old_old=old_ch;
		old_ch=ch;
	}
     fcloseall();
}