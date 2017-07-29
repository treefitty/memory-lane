
	.386p

IMODEX_TEXT	segment byte public use16 'CODE'
IMODEX_TEXT	ends

DGROUP	group	_BSS
	assume	cs:IMODEX_TEXT,ds:DGROUP
_BSS	segment word public use16 'BSS'
b@	label	byte
b@w	label	word
_MulTable	label	word
	db	800 dup (?)
_BSS	ends
IMODEX_TEXT	segment byte public use16 'CODE'

	assume	cs:IMODEX_TEXT
@SetPixel$qv	proc	far

		mov	cx,0a000H
		mov	es,cx
		mov	ch,al
		mov	cl,dl
		shl	bx,1
		mov	bx,word ptr DGROUP:_MulTable[bx]
		sar	dx,2
		add	bx,dx
		and	cl,3
		mov	ax,0102H
		mov	dx,03c4H
		shl	ah,cl
		out	dx,ax
		mov	es:[bx],ch

		ret

@SetPixel$qv	endp

	assume	cs:IMODEX_TEXT
@ModeX320x200$qv	proc	far

		push	di
		mov	ax,0013H
		int	10H
		mov	cx,0

InitMul1:

		mov	ax,cx
		mov	dx,80
		imul	dx
		mov	bx,cx
		shl	bx,1
		mov	word ptr DGROUP:_MulTable[bx],ax
		inc	cx
		cmp	cx,400
		jl	short InitMul1

		mov	dx,03C4H
		mov	ax,0604H
		out	dx,ax
		mov	ax,0F02H
		out	dx,ax
		mov	ax,0a000H
		mov	es,ax
		xor	di,di
		xor	ax,ax
		mov	cx,32000
		cld
		rep  stosw

		mov	dx,03D4H
		mov	ax,00014H
		out	dx,ax
		mov	ax,0e317H
		out	dx,ax
		pop	di

		ret

@ModeX320x200$qv	endp

	assume	cs:IMODEX_TEXT
@ModeX320x400$qv	proc	far

		push	di
		mov	ax,00013H
		int	10H
		mov	cx,0

InitMul2:

		mov	ax,cx
		mov	dx,80
		imul	dx
		mov	bx,cx
		shl	bx,1
		mov	word ptr DGROUP:_MulTable[bx],ax
		inc	cx
		cmp	cx,400
		jl	InitMul2

		mov	dx,03C4H
		mov	ax,0604H
		out	dx,ax
		mov	ax,00F02H
		out	dx,ax
		mov	ax,0a000H
		mov	es,ax
		xor	di,di
		xor	ax,ax
		mov	cx,32000
		cld
		rep 	stosw

		mov	dx,03D4H
		mov	ax,00014H
		out	dx,ax
		mov	ax,0e317H
		out	dx,ax
		mov	ax,00009H
		out	dx,ax
		pop	di

		ret

@ModeX320x400$qv	endp

	assume	cs:IMODEX_TEXT
@put_x_bit$qiinuci	proc	far

	push	bp
	mov	bp,sp
	sub	sp,12
	push	si
	push	di
	mov	word ptr [bp-4],2
	mov	byte ptr [bp-6],0
	mov  dx,[bp+6]
	mov	bx,[bp+8]
	mov	cl,dl
	shl	bx,1
	mov	bx,word ptr DGROUP:_MulTable[bx]
	and	cl,3
	mov	ah,1
	shl	ah,cl
	mov	[bp-5],ah
	mov	dx,[bp+6]
	sar	dx,2
	mov	[bp+6],dx
	mov	bx,0

@4@394:

	push	bx
	mov	dx,03c4H
	mov	al,2
	mov	ah,[bp-5]
	out	dx,ax
	mov	ax,word ptr [bp+8]
	mov	word ptr [bp-2],ax
	mov	ax,word ptr [bp+10]
	add	ax,word ptr [bp-4]
	mov	word ptr [bp-10],ax
	mov	ax,word ptr [bp+8]
	shl	ax,1
	add	ax,offset DGROUP:_MulTable
	mov	word ptr [bp-12],ax
	jmp	short @4@1010

@4@590:

	mov	si,word ptr [bp+6]
	mov	ax,word ptr [bp-10]
	mov	word ptr [bp-8],ax
	jmp	short @4@926

@4@646:

	mov	bx,word ptr [bp-12]
	mov	di,word ptr [bx]
	add	di,si
	add	di,word ptr [bp+14]
	cmp	byte ptr [bp-6],0
	je	short @4@702
	inc	di

@4@702:

	mov	es,word ptr [bp+12]
	mov	bx,word ptr [bp-8]
	mov	al,byte ptr es:[bx]
	or	al,al
	je	short @4@814
	mov		cx,0a000H
	mov		es,cx
	mov		es:[di],al

@4@814:

	inc	si
	inc	word ptr [bp-8]
	inc	word ptr [bp-10]
	inc	word ptr [bp-4]

@4@926:

	les	bx,dword ptr [bp+10]
	mov	al,byte ptr es:[bx]
	mov	ah,0
	add	ax,word ptr [bp+6]
	cmp	ax,si
	jg	short @4@646
	add	word ptr [bp-12],2
	inc	word ptr [bp-2]

@4@1010:

	les	bx,dword ptr [bp+10]
	mov	al,byte ptr es:[bx+1]
	mov	ah,0
	add	ax,word ptr [bp+8]
	cmp	ax,word ptr [bp-2]
	jg	short @4@590
	mov		al,[bp-5]
	shl		al,1
	cmp		al,16
	jl	short @4@1206
	mov		al,1
	mov		byte ptr [bp-6],1

@4@1206:

	mov		[bp-5],al
	pop		bx
	inc		bx
	cmp		bx,4
	jl	@4@394
	pop	di
	pop	si
	leave

	ret

@put_x_bit$qiinuci	endp

	assume	cs:IMODEX_TEXT
@ForceCompXBit$qiinuc	proc	far

	push	bp
	mov	bp,sp
	sub	sp,12
	push	si
	push	di
	mov	word ptr [bp-4],2
	mov	byte ptr [bp-6],0
	mov	dx,[bp+6]
	mov	bx,[bp+8]
	mov	cl,dl
	shl	bx,1
	mov	bx,word ptr DGROUP:_MulTable[bx]
	and	cl,3
	mov	ah,01H
	shl	ah,cl
	mov	[bp-5],ah
	mov	dx,[bp+6]
	sar	dx,2
	mov	[bp+6],dx
	mov	bx,0

@5@394:

	push	bx
	mov	dx,03c4H
	mov	al,002H
	mov	ah,[bp-5]
	out	dx,ax
	mov	ax,word ptr [bp+8]
	mov	word ptr [bp-2],ax
	mov	ax,word ptr [bp+10]
	add	ax,word ptr [bp-4]
	mov	word ptr [bp-10],ax
	mov	ax,word ptr [bp+8]
	shl	ax,1
	add	ax,offset DGROUP:_MulTable
	mov	word ptr [bp-12],ax
	jmp	short @5@1010

@5@590:

	mov	si,word ptr [bp+6]
	mov	ax,word ptr [bp-10]
	mov	word ptr [bp-8],ax
	jmp	short @5@926

@5@646:

	mov	bx,word ptr [bp-12]
	mov	di,word ptr [bx]
	add	di,si
	cmp	byte ptr [bp-6],0
	je	short @5@702
	inc	di

@5@702:

	mov	es,word ptr [bp+12]
	mov	bx,word ptr [bp-8]
	mov	al,byte ptr es:[bx]
	mov	cx,0a000H
	mov	es,cx
	mov	es:[di],al
	inc	si
	inc	word ptr [bp-8]
	inc	word ptr [bp-10]
	inc	word ptr [bp-4]

@5@926:

	les	bx,dword ptr [bp+10]
	mov	al,byte ptr es:[bx]
	mov	ah,0
	add	ax,word ptr [bp+6]
	cmp	ax,si
	jg	short @5@646
	add	word ptr [bp-12],2
	inc	word ptr [bp-2]

@5@1010:

	les	bx,dword ptr [bp+10]
	mov	al,byte ptr es:[bx+1]
	mov	ah,0
	add	ax,word ptr [bp+8]
	cmp	ax,word ptr [bp-2]
	jg	short @5@590
	mov	al,[bp-5]
	shl	al,1
	cmp	al,16
	jl	short @5@1206
	mov	al,1
	mov	byte ptr	[bp-6],1

@5@1206:

	mov	[bp-5],al
	pop	bx
	inc	bx
	cmp	bx,4
	jl	@5@394
	pop	di
	pop	si
	leave

	ret

@ForceCompXBit$qiinuc	endp

	assume	cs:IMODEX_TEXT
@ClrVidMem$qv	proc	far

		push	di
		mov	ax,0a000H
		mov	es,ax
		mov	dx,03c4H
		mov	ax,0F02H
		out	dx,ax
		xor	ax,ax
		xor	di,di
		mov	cx,32000
		cld
		rep	stosw

		pop	di

		ret

@ClrVidMem$qv	endp

IMODEX_TEXT	ends

_DATA	segment word public use16 'DATA'
s@	label	byte
_DATA	ends

IMODEX_TEXT	segment byte public use16 'CODE'
IMODEX_TEXT	ends

	public	@ClrVidMem$qv
	public	@ForceCompXBit$qiinuc
	public	@put_x_bit$qiinuci
	public	@ModeX320x400$qv
	public	@ModeX320x200$qv
	public	@SetPixel$qv
	public	_MulTable
	extrn	_i:word
_s@	equ	s@

	end
