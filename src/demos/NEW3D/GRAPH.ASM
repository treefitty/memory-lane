
;
;		 Low level graphics primitives for 3D demo.
;	    All routines for mode 13h linear mapped mode.
;
;         	  Subject to Copyright (c) 1995
;	       Paul Carroll - Black Marble Productions
;

	.MODEL large
	.386
	.CODE
	PUBLIC _putbuffer,_setpalette,_setColour,_drawHLine

_putbuffer PROC C screenbuf:DWORD

		push ds

		cld                     ;// Clear direction flag to forward.
		xor di,	di            ;// Null di pointer index.
		lds si,	screenbuf     ;// Load data segment.
		mov ax,	0a000h
		mov es,	ax            ;// Load video segment.
		mov cx,	3e80h         ;// Store double words (32-bit)
		rep movsd               ;// Loop till done

		pop ds

		ret

_putbuffer ENDP

_setpalette PROC C Array:DWORD

		push si

		mov  dx,3c8h        ;// DAC index register
		mov  al,0           ;// start with reg 1
		out  dx,al
		mov  cx,768
		push ds
		lds 	si,Array 		;// address segment
setpl:

		mov  dx,3DAh
		in   al,dx
		test al,1
		je  	setpl

		mov 	dx,3c9h
		mov 	al,byte ptr[ds:si]
		xor  ah,ah
		out  dx,al
		inc	si
		dec	cx
		jnz	setpl

		pop ds si
		ret

_setpalette ENDP

_setColour PROC C Col:BYTE,r:BYTE,g:BYTE,b:BYTE

		mov	dx,3c8h
		mov	al,Col
		out	dx,al
		inc	dx
		mov	al,r
		out	dx,al
		mov	al,g
		out	dx,al
		mov	al,b
		out	dx,al

		ret

_setColour ENDP

_drawHLine PROC C dest:DWORD, ofs:WORD, col:BYTE, length:BYTE

		push di si

		les	di,dest
		add	di,ofs
		mov	al,col
		mov	ah,al
		mov	bl,length
		mov	cl,bl
		xor	ch,ch
		and  bl,1
		jnz	drawLine
		stosb						; Draw leading byte
		sub	cl,1

drawLine:
		shr	cl,1
		rep	stosb

		pop	si di

		ret

_drawHLine ENDP

	END

