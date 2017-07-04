
		.MODEL large
		.CODE
		PUBLIC _ScrlUp,_ScrlDown,_SetPage,_CopyScreen

scroffs dw 0

SetStartAddr MACRO

		mov	bx,[scroffs]
		mov 	dx,3d4h
		mov 	al,0ch
		mov 	ah,bh
		out 	dx,ax
		inc	al
		mov 	ah,bl
		out 	dx,ax

		ENDM

_ScrlUp	PROC

		cli

		add 	[scroffs],80
		mov 	bx,0008h
		mov 	cx,15

Loop1:
		push cx
		mov  cx,900
Jump1:

		mov  dx,03dah

HRI_1:
		in 	al,dx
		test	al,1
		jnz	HRI_1

		dec 	cx
		jnz	Jump1
		pop	cx

		add 	bh,1
		mov 	dx,03d4h
		mov 	ax,bx
		out 	dx,ax
		dec 	cx
		jnz 	Loop1

		SetStartAddr

		mov 	dx,03d4h
		mov 	ax,0008h
		out 	dx,ax

		sti

		ret

_ScrlUp   ENDP



_ScrlDown PROC

		cli

		sub 	[scroffs],80

		mov 	dx,03dah
VRI_2:
		in	al,dx
		test	al,8
		jnz	VRI_2

NVRI_2:
		in	al,dx
		test	al,8
		jz	NVRI_2
		SetStartAddr

		mov 	bx,1008h
		mov 	cx,15

Loop2:
		push cx
		mov  cx,850
Jump2:

		mov  dx,03dah

HRI_2:
		in 	al,dx
		test	al,1
		jnz	HRI_2

		dec 	cx
		jnz	Jump2
		pop	cx

		sub 	bh,1
		mov 	dx,03d4h
		mov 	ax,bx
		out 	dx,ax
		dec 	cx
		jnz 	Loop2

		mov  dx,03dah

HRI_3:
		in 	al,dx
		test	al,1
		jnz	HRI_3

		mov 	dx,03d4h
		mov 	ax,0008h
		out 	dx,ax

		sti

		ret

_ScrlDown ENDP


_SetPage  PROC C PageNo:WORD

		mov  bx,PageNo
		sub	bx,1
		mov  ax,2000
		mul	bx
		mov 	[scroffs],ax
		SetStartAddr

		ret

_SetPage  ENDP

_CopyScreen PROC

		push si
		push di
		push ds
		cld
		mov 	ax,0b800h
		mov  es,ax
		mov  ds,ax
		mov  cx,2000
		mov  di,4000
		xor  si,si
		rep  movsw
		pop	ds
		pop  di
		pop	si

		ret

_CopyScreen ENDP

		END
