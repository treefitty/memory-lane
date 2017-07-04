#include <imodex.h>

#include <conio.h>

int MulTable[600];
int ScreenWidth=320;

void SetPixel(void)
	{
	asm mov	cx,0xa000
	asm mov	es,cx
	asm mov	ch,al
	asm mov	cl,dl
	_BX=MulTable[_BX];
	asm sar	dx,2
	asm add	bx,dx
	asm and	cl,3
	asm mov	ax,0x102
	asm mov	dx,0x3c4
	asm shl	ah,cl
	asm out	dx,ax
	asm mov	es:[bx],ch
}

void ScrSetup(int width,int height)
{
	int i;

	for(i=0;i<height;i++)
		{
		MulTable[i]=i*(width/4);
	}
	ScreenWidth=width;
}

void ModeX320x200(void)
{
	int i;

	asm mov   ax,0x013              //  Set to 320x200x256 Linear mode
	asm int   0x10

	for(i=0;i<400;i++)
		{
		MulTable[i]=i*80;
	}

	asm mov   dx,0x3C4              //  Unchain that VGA
	asm mov   ax,0x604
	asm out   dx,ax
	asm mov   ax,0xF02              // Enable write to all four planes
	asm out   dx,ax

	asm mov   ax,0xa000             // Clear Display Memory
	asm mov   es,ax
	asm xor   di,di
	asm xor   ax,ax
	asm mov   cx,64000
	asm cld

Clear1:

	asm mov	es:[di],ax
	asm inc	di
	asm dec	cx
	asm jnz	Clear1

	asm mov   dx,0x3D4
	asm mov   ax,0x0014             //  turn off dword mode
	asm out   dx,ax
	asm mov   ax,0xe317             //  turn on byte mode
	asm out   dx,ax
}

void ModeX320x400(void)
{
	int i;

	asm mov   ax,0x0013                // Sets VGA linear 320x200x256
	asm int   0x10

	for(i=0;i<400;i++)
		{
		MulTable[i]=i*80;
	}

	asm mov   dx,0x3C4                 // Unchain that VGA
	asm mov   ax,0x604
	asm out   dx,ax
	asm mov   ax,0x0F02
	asm out   dx,ax

	asm mov   ax,0xa000             //  Clear Display Memory
	asm mov   es,ax
	asm xor   di,di
	asm xor   ax,ax
	asm mov   cx,64000
	asm cld

Clear2:

	asm mov	es:[di],ax
	asm inc	di
	asm dec	cx
	asm jnz	Clear2

	asm mov   dx,0x3D4                 //; Reprogram CRT Controller:
	asm mov   ax,0x0014               //; turn off dword mode
	asm out   dx,ax
	asm mov   ax,0xe317               //; turn on byte mode
	asm out   dx,ax
	asm mov   ax,0x0009               //; cell height
	asm out   dx,ax
}

void PutCompiledXBit(int x,int y,char far *Source,int Ofs)
{
	int 	iy,ix;
	int  Count=2;
	char Plane;
	char Flag=0;

	asm mov	dx,x
	asm mov	bx,y
	asm mov	cl,dl
	_BX=MulTable[_BX];
	asm and	cl,3
	asm mov	ah,1
	asm shl	ah,cl
	asm mov 	Plane,ah
	asm mov	dx,x
	asm sar	dx,2
	asm mov	x,dx
	asm mov	bx,0

NextPlane:

	asm push	bx
	asm mov	dx,0x3c4
	asm mov	al,2
	asm mov   ah,Plane
	asm out	dx,ax
	//asm mov	al,Plane
	//asm xor	ah,ah
	//asm push	ax

	for(iy=y;iy<Source[1]+y;iy++)
		{
		for(ix=x;ix<Source[0]+x;ix++,Count++)
			{
			_DI=MulTable[iy]+ix+Ofs;
			if(Flag)_DI++;
			_AL=Source[Count];
			if(_AL==0) goto Skip;
			asm mov	cx,0xa000
			asm mov	es,cx
			asm mov	es:[di],al

		Skip:

		}
	}
	asm mov	al,Plane
	//asm pop	ax

	asm shl	al,1
	asm cmp	al,16
	asm jl	PlaneOk
	asm mov	al,1
	asm mov	Flag,1

PlaneOk:

	asm mov	Plane,al
	asm pop	bx
	asm inc	bx
	asm cmp	bx,4
	asm jl	NextPlane

}

void ForceCompXBit(int x,int y,char far *Source)
{
	int 	iy,ix;
	int  Count=2;
	char Plane;
	char Flag=0;

	asm mov	dx,x
	asm mov	bx,y
	asm mov	cl,dl
	_BX=MulTable[_BX];
	asm and	cl,3
	asm mov	ah,0x1
	asm shl	ah,cl
	asm mov 	Plane,ah
	asm mov	dx,x
	asm sar	dx,2
	asm mov	x,dx
	asm mov	bx,0

NextPlane:

	asm push	bx
	asm mov	dx,0x3c4
	asm mov	al,0x02
	asm mov   ah,Plane
	asm out	dx,ax

	for(iy=y;iy<Source[1]+y;iy++)
		{
		for(ix=x;ix<Source[0]+x;ix++,Count++)
			{
			_DI=MulTable[iy]+ix;
			if(Flag)_DI++;
			_AL=Source[Count];
			asm mov	cx,0xa000
			asm mov	es,cx
			asm mov	es:[di],al

		Skip:

		}
	}
	asm mov	al,Plane
	asm shl	al,1
	asm cmp	al,16
	asm jl	PlaneOk
	asm mov	al,1
	asm mov	Flag,1

PlaneOk:

	asm mov	Plane,al
	asm pop	bx
	asm inc	bx
	asm cmp	bx,4
	asm jl	NextPlane
}

void ClrVidMem(void)
{
	asm mov	ax,0xa000
	asm mov	es,ax
	asm mov	dx,0x3c4
	asm mov	ax,0xF02
	asm out	dx,ax
	asm xor	ax,ax
	asm xor	di,di
	asm mov	cx,64000

Clear3:

	asm mov	es:[di],al
	asm inc	di
	asm dec	cx
	asm jnz	Clear3
}





