#include <imusic.h>

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void LoadMusic(char *File)
{
	if ((module = open("MSE.CFG", O_RDONLY | O_BINARY)) == -1)
		{
		MSEError:
		printf("No Sound selected in SETUP. Please run SETUP.\n");
		exit(0);
	}
	read(module, &MSEConfig, 10);


	if (MSEConfig.SoundCard==0) goto MSEError;

	MSEConfig.SoundCard--;


	BufferSize = 4096;
	OverRate = 45;

	ErrCode=LoadMSE(msefile[MSEConfig.SoundCard],
			   0,                             // File offset
			   OverRate,
			   BufferSize,
			   &MSEConfig.BaseIO,
			   &MSEConfig.IRQ,
			   &MSEConfig.DMA);
	if (ErrCode)
		{
		asm mov 	ax,0x03
		asm int	0x10

		switch(ErrCode)
			{
			case 1: printf("Base I/O address autodetection failure\n");
				break;
			case 2: printf("IRQ level autodetection failure\n");
				break;
			case 3: printf("DMA channel autodetection failure\n");
				break;
			case 4: printf("DMA channel not supported\n");
				break;
			case 6: printf("Sound device does not respond\n");
				break;
			case 7: printf("Memory control blocks destroyed\n");
				break;
			case 8: printf("Insufficient memory for mixing buffers\n");
				break;
			case 9: printf("Insufficient memory for MSE file\n");
				break;
			case 10: printf("MSE has invalid identification string (corrupt/non-existant)\n");
				break;
			case 11: printf("MSE disk read failure\n");
				break;
			case 12: printf("MVSOUND.SYS not loaded (required for PAS use)\n");
				break;
			default: printf("Unknown error on MSE startup %u\n", ErrCode);
		}
		exit(0);
	}

	if ((module=open(File, O_RDONLY | O_BINARY)) == -1)
		{
		printf("Can't find file %s\n", File);
		exit(0);
	}

	ErrCode = EmsExist() & 1;             //Enable EMS use if EMS services found
	LoadGDM(module, 0, &ErrCode, &modhead);  //Load our GDM
	close(module);

	if (ErrCode != 0)
		{
		asm mov	ax,0x03
		asm int	0x10

		switch(ErrCode)
			{
			case 1: printf("Module is corrupt\n");
				break;
			case 2: printf("Could not autodetect module type\n");
				break;
			case 3: printf("Bad format ID\n");
				break;
			case 4: printf("Out of memory\n");
				break;
			case 5: printf("Cannot unpack samples\n");
				break;
			case 6: printf("AdLib samples not supported\n");
				break;
			default: printf("Unknown load error: %u\n", ErrCode);
		}
		exit(0);
	}

	channels = 0;
	//Scan and count number of used music channels
	for (j = 0; j < 32; j++)
		{
		if (modhead.PanMap[j] != 0xFF)
		channels++;                     //increment channels if in use
	}
}

void StartPlaying(void)
{
	StartOutput(channels, 0);
	StartMusic();
}

void StopPlaying(void)
{
	StopMusic();                         //Disable music processing
	StopOutput();                        //Turn off sound output
	UnloadModule();                      //Free module from memory
	StopBanner();                        //Turn off that damn signoff banner ;)
	FreeMSE();
}