///////////////////////////////////////////
///////////////////////////////////////////
//                                       //
// Slimy Texture Mapping.                //
//                                       //
// by F.Becker aka -=* SliQ *=-          //
//                                       //
// Greet me, if you use it...            //
//                                       //
//                                       //
// C++ commenting style - I prefer it,   //
// what can I say?                       //
//                                       //
///////////////////////////////////////////
///////////////////////////////////////////

#include <stdio.h>
#include <math.h>

// Fdegrees to radians rad=deg*Fd2r
// Fdegrees range from 0 to 1023.
#define		Fd2r	3.1415926535/512

#define         Xsize	160
#define		Ysize   120

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgbpal;


// NOTE: longs represent 16.16 fixed point values

unsigned char far thepic[65535];
rgbpal            pal[256];
long		  slimeS[128],slimeC[128];
long 		  xo,yo;

void Scron(){
asm{
	  //set mode 13h
	  pusha
	  mov	ax,0x13
	  int	0x10
	  popa
   }
}


void Scroff(){
asm{
	  //set mode 03h
	  pusha
	  mov	ax,0x03
	  int	0x10
	  popa
   }
}


void SetCol(char ColReg, char Red, char Green, char Blue ){
asm{
	  mov	dx,0x03c8
	  mov   al,[ColReg]
	  out   dx,al

	  inc   dx

	  mov   al,[Red]
	  out   dx,al
	  mov   al,[Green]
	  out   dx,al
	  mov   al,[Blue]
	  out   dx,al
    }
}


void load_pic(){

  FILE *fp;
  int i,x,y,gray;

  fp = fopen( "sliq.raw" , "rb" );
  fread( thepic, 1, 32, fp );      // skip header
  fread( pal   , 1, 768  , fp );   // load palette
  fread( thepic, 1, 65535, fp );   // load picture
  fclose( fp );

  // set palette

  for( i=0 ; i<256 ; i++ )
    SetCol(i,pal[i].r>>2,pal[i].g>>2,pal[i].b>>2);
}

void TexScreen( long dx , long dy,
		long adx, long ady){

  // postiton on screen
  int  x,y;

  // start position and current position in 16.16 fixed point
  long ystart,xstart,xpos,ypos;

  // color!
  unsigned int color;

  // slime adjustments for every vertical line
  // NOTE: picture is drawn by columns not rows.
  int  tmp[256];

  // lower precision versions of adx/ady and xpos/ypos ie 8.8 fixed point.
  //   it's sufficient for the relative small (orig.) picture size of 256x256
  int  adx2,ady2,xp2,yp2;

  // make a 8.8 version of ad..
  // we dont worry about the upper 8 bits of adx/ady, because
  // they are just sign extensions in this program...
  adx2=adx>>8;
  ady2=ady>>8;

  // precalculate slime values for vertical line (used in inner loop)
  for( y=0; y<Ysize ; y++ ){
    tmp[y]=ady2+(slimeS[y&127]>>8);
  }

  //offset on screen - use register
  _SI = 0;

  xstart=xo;
  ystart=yo;
  for( x=0; x<Xsize ; x++ ){
    xpos=xstart;
    ypos=ystart;

    // make a 8.8 version of .pos.
    xp2=xpos>>8;
    yp2=ypos>>8;

    for( y=0; y<Ysize ; y++ ){

      //calc position within original
      //which has the convenient size of 256x256
      //  use the hi-bytes which is the integer part of the 8.8 fixed pt.
      //bh is y and bl is x.
      asm{
	  mov	bx,[yp2]
	  mov	bl,byte ptr [xp2]+1
      }

      // get the color from the original
      color = thepic[ _BX ];

      //set the pixel
      asm{
	  // Set es to video segment
	  mov	ax,0xa000
	  mov	es,ax
	  mov   ax,[color]
	  mov   es:[si],al
      }

      //move to next position
      xp2+=adx2;
      yp2+=tmp[y];
      //move one line down
      _SI+=320;
    }
    // calc new start position for vertical line plus slime adjustment...
    xstart+=dx+slimeC[x&127];
    ystart+=dy;
    // back to top and one right
    _SI-=(320*Ysize)-1;
  }
}


int main(){

  int deg;
  long dx,dy;

  Scron();
  load_pic();

  for( deg=0 ; deg <128; deg++ ){
    slimeS[deg]=(sin((deg<<3)*Fd2r)*65536.0);
    slimeC[deg]=(cos((deg<<3)*Fd2r)*65536.0);

//  Note: you dont have to use sin and cos to get a
//        slime effect. Essentially any (cont.) function/curve
//	  which will start and end with the same Y-value
// 	  will work. (ie. slime[0]=slime[127].) Play around with
//	  it - use a bezier curve, maybe. Be creative...
//    slimeS[deg]=(127-deg)*log(deg+1)*536;
//    slimeC[deg]=(127-deg)*log(deg*deg+1)*136;
  }

  //tilt in Fdegrees
  deg=0;

  //offset into (original) picture
  xo=0;
  yo=0;

  while( !kbhit() ){

    //calc delta X and delta Y (the slope)
    //ie basic direction across (orig.) picture
    //  in texscreen we'll follow a sine-like curve to
    //  create slime effect.
    dy=((sin(deg*Fd2r)*160000.0));
    dx=((cos(deg*Fd2r)*160000.0));

    //the vector (-dy,dx) is perpendicular to (dx,dy)
    //  the basic shape should be a rectangle.
    //  (remove the minus sign and you'll see what I mean.-)

    TexScreen(  dx,dy,  -dy,dx  );

    //keep on turning
    deg+=2;

    // move offset a bit across (orig.) picture
    xo+=(1253L<<5);
    yo+=(1534L<<5);
  }

  // back to text
  Scroff();

  return(0);

}
