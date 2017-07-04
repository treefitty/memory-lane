//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//                     Bells, Whistles, and Sound Boards
//       Copyright (c) 1993-95, Edward Schlunder. All Rights Reserved.
//ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
// PLAYC.C - Example GDM module player.
//           Written by Edward Schlunder (1995)
//
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

#include <bwsb.h>                      //Declare all BWSB subs and functions

#include <stdio.h>
#include <string.h>
#include <io.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>

#include <process.h>

#include <dos.h>
#include <fcntl.h>

typedef struct
{ int SoundCard;
  int BaseIO;
  int IRQ;
  int DMA;
  int SoundQuality;
} MSEConfigFile;

void main(void)
{ int module;               // handle of GDM file
  char modfile[80];         // ASCIIZ filename of file to load
  char *comspec;            // Command processor name and path
  char *msefile[] = { "GUS.MSE",
                      "SB1X.MSE",
                      "SB2X.MSE",
                      "SBPRO.MSE",
                      "SB16.MSE",
                      "PAS.MSE"  };
  GDMHeader modhead;
  MSEConfigFile MSEConfig; // Configuration for MSE
  char OverRate;
  int BufferSize;
  int ErrCode, j;
  unsigned char channels;

  printf("\nBWSB Example Module Player\n");
  printf("Copyright (c) 1993-95, Edward Schlunder\n\n");

  if ((module = open("MSE.CFG", O_RDONLY | O_BINARY)) == -1)
  {
MSEError:
    printf("No Sound selected in SETUP. Please run SETUP.\n");
    return;
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
  { switch(ErrCode)
    { case 1: printf("Base I/O address autodetection failure\n");
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
    return;
  }

  // Display name of sound device
  printf("Sound Device: %s\n", DeviceName());
  // Display the acutal settings *used* in the MSE.
  printf("Addr: %Xh  IRQ: %d  DMA: %d\n",
         MSEConfig.BaseIO, MSEConfig.IRQ, MSEConfig.DMA);

  //Ask for a module to load
  printf("Module file: ");
  if (gets(modfile)==NULL) return;  //abort if nothing entered

  //Append a .GDM if no extension specified
  if (strstr(modfile, ".")==NULL) strncat(modfile, ".GDM", 80);

  if ((module=open(modfile, O_RDONLY | O_BINARY)) == -1)
  { printf("Can't find file %s\n", modfile);
    return;
  }

  printf("Loading Module: %s\n", modfile);
  ErrCode = EmsExist() & 1;             //Enable EMS use if EMS services found
  LoadGDM(module, 0, &ErrCode, &modhead);  //Load our GDM
  close(module);

  if (ErrCode != 0)
  { switch(ErrCode)
    { case 1: printf("Module is corrupt\n");
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
    return;
  }

  channels = 0;
  //Scan and count number of used music channels
  for (j = 0; j < 32; j++)
  { if (modhead.PanMap[j] != 0xFF)
       channels++;                     //increment channels if in use
  }

  printf("\nChannels: %u  Song: %.32s", channels, modhead.SongTitle);
  printf("\nOversampling: %u Hz\n", StartOutput(channels, 0));
  printf("D for DOS Shell or any other key to quit\n\n");
  StartMusic();

  comspec = getenv("COMSPEC");
  if (comspec==NULL) comspec = "COMMAND.COM";
  for (;;)
  { while (!kbhit())
    { printf("Playing Music ÄÄ> Order: %u  Pattern: %u  Row: %u   \r",
             MusicOrder(0xFF),
             MusicPattern(0xFF),
             MusicRow());                                             }

    j = toupper(getch());
    if (j=='D')
    {  printf("\n\nType EXIT [enter] to return..");
       spawnl(P_WAIT, comspec, NULL);
    }
    else break;
  }

  StopMusic();                         //Disable music processing
  StopOutput();                        //Turn off sound output
  UnloadModule();                      //Free module from memory
  StopBanner();                        //Turn off that damn signoff banner ;)
  FreeMSE();                           //Free MSE from memory
}

