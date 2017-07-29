
_keydat	segment para public 'data'

extrn	_keyflags:byte
oldkb	dw 0,0

_keydat   ends

_keycode  segment para public 'code'

		PUBLIC _set_ikb,_reset_ikb
		ASSUME cs:_keycode,ds:_keydat

new_int9	proc

		cli
		push cx
		push	bx
		push	ax
		push ds
		mov	ax,seg _keyflags
		mov	ds,ax

wait_key:
		in	al,64h
		test	al,02h
		jnz  wait_key
		in	al,60h

		xor	ah,ah
		shl	ax,1
		shr	al,1
		xor	ah,1
		xor	bh,bh
		mov	bl,al
		mov	_keyflags[bx],ah

		in	al,61h
		mov	ah,al
		or	al,80h
		out	61h,al
		xchg	ah,al
		out	61h,al

		mov	al,20h
		out	20h,al

		pop	ds
		pop	ax
		pop	bx
		pop	cx
		sti
		iret

new_int9	endp

_set_ikb	proc far

		push	ds

		mov	ax,3509h
		int	21h
		mov	ax,seg oldkb
		mov	ds,ax
		mov	oldkb[0],bx
		mov	oldkb[2],es
		mov	ax,seg new_int9
		mov	ds,ax
		mov	dx,offset new_int9
		mov	ax,2509h
		int	21h

		pop	ds
		ret

_set_ikb	endp

_reset_ikb proc far

		push	ds
		mov	ax,seg oldkb
		mov	ds,ax
		mov	dx,oldkb[0]
		mov	ax,oldkb[2]
		mov	ds,ax
		mov	ax,2509h
		int	21h
		pop	ds

		ret

_reset_ikb endp

_keycode  ends

		end