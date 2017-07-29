;
; Handles low level graphics stuff
;

		.MODEL large
		PUBLIC _setpalette
		.286
		.CODE

;------------------------------------------------------
; Get bitmap from source and put it in dest
;
; Makes first two bytes of dest width and height
;------------------------------------------------------

_get_bit	proc C x:WORD,y:WORD,w:BYTE,h:BYTE,source:DWORD,dest:DWORD

		push ds

		cld
		les  di,[dest]
		lds	si,[source]
		mov 	ax,y
		mov 	bx,320
		imul	bx
		add 	ax,x
		add 	si,ax
		xor  dx,dx
		xor  cx,cx
		mov 	dl,h
		mov 	cl,w
		mov	es:[di],cl
		mov	es:[di+1],dl
		add	di,2
iloop:
		rep	movsb
		dec 	dl
		jz	iexit
		mov	cl,w
		sub	si,cx
		add 	si,320
		jmp	iloop

iexit:
		pop	ds

		ret

_get_bit	endp

;-----------------------------------------------
; Sets VGA palette to Array
;-----------------------------------------------

_setpalette PROC C Array:DWORD

		push ds si
		mov  dx,	3c8h         	;// DAC index register
		mov  al,	0            	;// start with reg 1
		out  dx,	al
		mov  cx,	768
		lds 	si, 	Array 		;// address segment
setpl:
		mov  dx,	3DAh
		in   al,	dx
		test al,	1
		je  		setpl

		mov 	dx,3c9h
		mov 	al,byte ptr[ds:si]
		xor  ah,ah
		out  dx,al
		inc	si
		dec	cx
		jnz	setpl

		pop si ds
		ret

_setpalette ENDP

		end