/*
	Header for MOUSE256
*/

#define	PARADISE		0x5f
#define	ATI			0x62
#define	TSENG		0x2e
#define	TRIDENT		0x5d
#define	OAK			0x53
#define	VESA			0x0101

#define	COLOURDEPTH	8

#define	RGB_RED		0
#define	RGB_GREEN	1
#define	RGB_BLUE	2
#define	RGB_SIZE	3

#define	pixels2bytes(n)	((n+7)/8)
#define	ImageWidth(p)	(1+p[0]+(p[1] << 8))
#define	ImageDepth(p)	(1+p[2]+(p[3] << 8))

#define	SetVgaWhite(n)		{ VGAWHITE=(n); }
#define	SetVgaBlack(n)          { VGABLACK=(n); }
#define	SetVgaGrey(n)		{ VGAGREY=(n); }
#define	SetVgaLightGrey(n)	{ VGALIGHTGREY=(n); }
#define	SetVgaDarkGrey(n)	{ VGADARKGREY=(n); }

#define	GetVgaWhite()		VGAWHITE
#define	GetVgaBlack()          	VGABLACK
#define	GetVgaGrey()		VGAGREY
#define	GetVgaLightGrey()	VGALIGHTGREY
#define	GetVgaDarkGrey()	VGADARKGREY

#define	greyvalue(r,g,b)        (((r*30)/100) + ((g*59)/100) + ((b*11)/100))

#define	beep()			putchar(7)

#ifndef max
#define max(a,b)            (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b)            (((a)<(b))?(a):(b))
#endif

#define	ImageSize256(l,t,r,b)	(4L+(long)(1+max(r,l)-min(r,l))*(long)(1+max(b,t)-min(b,t)))

typedef struct {

	int x,y;
	int w,h;

} rect;

typedef struct {

	int x,y;

} point;

extern unsigned int VGAWHITE;
extern unsigned int VGABLACK;

extern "C" {

	unsigned int InitMouse(unsigned int screenwide,unsigned int screendeep,unsigned int card);
	void DeinitMouse(void);
	void MouseOn(void);
	void MouseOff(void);
	//******* unsigned int MouseDown(POINT *p);
	void MouseCursor(char *cursor,unsigned int x,unsigned int y);
	void DrawString(unsigned x,unsigned int y,char *p,unsigned int c,unsigned int b);
	void put_image(unsigned int left,unsigned int top,char *p);
	void get_image(unsigned int left,unsigned int top,unsigned int right,unsigned int bottom,char *p);
	void ClearScreen(char col);
	void framerect(int x,int y,int w,int h,int col);
	void set_rgb(int index,char r,char g,char b);
	void draw_line(int left,int top,int right,int bottom,int col);
	void fill_rect(int left,int top,int right,int bottom,int col);
	int  mouse_down(point *a_pt);

};

