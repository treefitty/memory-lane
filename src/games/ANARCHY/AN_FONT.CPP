
/*

    Description of Black Marble file format:

		4 bytes -> Header, must match 'BMMF' or not valid font file.
		2 bytes -> Version. Byte 1 major, byte 2 minor version number
	   768 bytes -> Palette of the fonts

	   Next come the letters, no more letters determined by EOF
	   Each letters format is:

			1 byte  -> Index, or ASCII number eg 'A' would be 95
			1 byte  -> Width of char
			1 byte  -> Height of char
    width*height bytes -> This holds the data for the char

*/

#include "AN_DEFS.H"

FILE *FontFile;

FontType myFonts[']'-' '];
char far *FontDat[']'-' '];

void LoadFont(char *Filename)
	{
	char filesig[4];
	char FontPal[768];
	char SpaceDefined;
	char done;
	char index;
	int  i,count1,count2;

	SpaceDefined=0;
	if((FontFile=fopen(Filename,"rb"))==0)
		{
		asm mov	ax,0x03
		asm int	0x10
		textcolor(15);
		cprintf("\n\rError opening FONT file.");
		cprintf("\n\rAbnormal program termination.\n\n\r");
		asm mov	ax,0x4c00
		asm int	0x21
	}
	fread(filesig,4,1,FontFile);
/*	if(strcmp(filesig,"BMFF")!=0)
		{
		asm mov	ax,0x03
		asm int	0x10
		textcolor(15);
		cprintf("\n\rError reading FONT file.");
		cprintf("\n\rNot Black Marble font file format.");
		cprintf("\n\n\rAbnormal program termination.\n\n\r");
		asm mov	ax,0x4c00
		asm int	0x21
	}
*/
	fseek(FontFile,6,0);

	for(i=0;i<768;i++)
		{
		FontPal[i]=fgetc(FontFile);
	}

	done=0;
	while(!done)
		{
		index=fgetc(FontFile);

		if(feof(FontFile))
			{
			done=1;
			goto END;
		}

		if(index>=' ' && index<=']')
			{
			myFonts[index-32].Defined=1;
			myFonts[index-32].Width=fgetc(FontFile);
			myFonts[index-32].Height=fgetc(FontFile);

			if((FontDat[index-32]=(char *)malloc(sizeof(char)*((myFonts[index-32].Width+1)*(myFonts[index-32].Height+1))) )==0)
				{
				asm mov	ax,0x03
				asm int	0x10
				textcolor(15);
				cprintf("Insufficient memory.\n\r");
				cprintf("Abnormal program termination\n\n\r");
				asm mov	ax,0x4c00
				asm int	0x21
			}
			for(count1=0,count2=0;count1<myFonts[index-32].Width*myFonts[index-32].Height;count1++,count2++)
				{
				FontDat[index-32][count2]=fgetc(FontFile);
			}
		}
		else {
			asm mov	ax,0x03
			asm int	0x10
			textcolor(15);
			cprintf("\n\rInvalid character encountered.");
			cprintf("\n\rAbnormal program termination.");
			asm mov	ax,0x4c00
			asm int	0x21
		}
		if(index>='A' && index<='Z' && SpaceDefined==0)
			{
			myFonts[' '-32].Defined=1;
			myFonts[' '-32].Width=myFonts[index-32].Width;
			myFonts[' '-32].Height=myFonts[index-32].Height;

			if((FontDat[' '-32]=(char *)malloc(sizeof(char)*((myFonts[' '-32].Width+1)*(myFonts[' '-32].Height+1))) )==0)
				{
				asm mov	ax,0x03
				asm int	0x10
				textcolor(15);
				cprintf("Insufficient memory.\n\r");
				cprintf("Abnormal program termination\n\n\r");
				asm mov	ax,0x4c00
				asm int	0x21
			}
			memset(FontDat[' '-32],0,(myFonts[' '-32].Width+1)*(myFonts[' '-32].Height+1));
			SpaceDefined=1;
		}

		END:
	}
	fclose(FontFile);
}


