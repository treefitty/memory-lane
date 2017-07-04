
#ifndef DRAW_H
#define DRAW_H

#define XOR 0x00000010
#define COPY 0x00000000

int x1,y1,x2,y2,col;

extern "C" {

extern void quit_pgm(void);
extern void graph(void);

	}

void rect(unsigned int x1,
			 unsigned int y1,
			 unsigned int x2,
			 unsigned int y2,
			 unsigned int col);

void rect(unsigned int x1,
			 unsigned int y1,
			 unsigned int x2,
			 unsigned int y2,
			 unsigned int col)
			 {
			 x1=x1;y1=y1;x2=x2;y2=y2;
			 col=col;
			 rect(x1,y1,x2,y2,col);
			 }
#endif