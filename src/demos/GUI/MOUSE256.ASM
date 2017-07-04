;               ! For large memory model !
;
;	      	    SVGA GRAPHICS LIBRARY
;	              ---------------------
;
;                   (Real mode version)
;
;       This graphics programming library should be
;	considered under the laws of Copyright. No code may
;	  be reproduced in any shape or form without the
;	          authors expressed permission.
;
;             Copyright (c) 1996 Paul Carroll
;
;
			.model large
			JUMPS

_aoff		equ	6	;stack offset to first arg

paradise		equ	005fh
ati			equ	0062h
tseng		equ	002eh
trident		equ	005dh
oak			equ	0053h
vesa			equ	0101h

cursorwide	equ	16	;cursor dimensions
cursordeep	equ	16

tabledeep	equ	480		;number of items in the screen table

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; start of macro definitions
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;normalize the pointer in ds:si, screen width in ax
normalize	macro
		local	lab1

		push	ax
		push	cx
		mov	cx,0ffffh
		sub	cx,ax
		mov	ax,cx
		cmp	si,ax
		jb	lab1
		mov	ax,si			;see how many segments are
		mov	cl,4			;...in si (si div 16)
		shr	ax,cl

		mov	cx,ds			;add them to the data segment
		add	cx,ax			;...(you can't just add ds,ax)
		mov	ds,cx
		and	si,000fh		;adjust si (si mod 16)
lab1:
		pop	cx
		pop	ax

		endm

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; checkpage
checkpage	macro	;handle page boundaries
		local	lab1
		cmp	di,0000h
		jne	lab1
		push	ax
		mov	ax,[codepage]
		inc	ax
		call	cs:[setbank]
		pop	ax
lab1:
		endm

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; grafadd_di
grafadd_di	macro	arg1	;add the argument to di with page switching
		local	lab1
		push	ax
		mov	ax,di
		add	di,arg1
		cmp	ax,di
		jbe	lab1
		mov	ax,[codepage]
		inc	ax
		call	cs:[setbank]
lab1:
		pop	ax
		endm

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; graf_stosb
graf_stosb	macro	;store a bit at ds:si with page switch
		cld
		stosb
		checkpage
		endm

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; graf_repnestosb
graf_repnestosb	macro	;do store string to es:di with page switch
		local	lab1,lab2
		cld
		cmp	[pagebreak],00h
		jne	lab1
		repne stosb
		jmp	lab2
lab1:
		stosb
		checkpage
		loop	lab1
lab2:
		endm

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; graf_repnemovsb
graf_repnemovsb	macro	;do move string from ds:si to es:di with page switch
		local	lab1,lab2
		cld
		cmp	[pagebreak],00h
		jne	lab1
		repne movsb
		jmp	lab2
lab1:
		movsb
		checkpage
		loop	lab1
lab2:
		endm

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; ldataseg
;this macro fetches the data segement
ldataseg	macro
		push	ax
		mov	ax,_data
		mov	ds,ax
		pop	ax
		endm

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; start actual code segment here!!!!!!!!!
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
mouse256_text	segment word public 'code'

		assume	cs:mouse256_text,ds:_data

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
		public	_get_image
;
; Copies an image fragment from the screen to a region
; of memory
;
; Destroys :
;
gi_parms	struc
		dd		?
		dw		?
 gi_width dw		?
gi_height dw		?
	dest dd		?
gi_parms	ends

_get_image	proc	far

		push	bp
		mov	bp,sp
		push	ds di si

		ldataseg

		call	hidecursor

		mov	ax,[_screenwide]
		mov  dx,[bp+_aoff+4] 	;get width
		sub	dx,[bp+_aoff+0]
		mov	cx,[bp+_aoff+6]		;get depth
		sub	cx,[bp+_aoff+2]
		mov	si,[bp+_aoff+8]		;get image
		mov	ds,[bp+_aoff+10]

		normalize				;normalize pointer

		mov	ds:[si],dx			;save size values
		mov	ds:[si+2],cx
		add	si,0004h

		inc	cx				;get real size values
		inc	dx

gi1:		push	cx
		push	dx

		push	ds
		ldataseg
		mov	bx,[bp+_aoff+2]
		call	vgaline
		grafadd_di [bp+_aoff+0]
		pop	ds
		inc	word ptr [bp+_aoff+2]

		pop	cx
		push	cx

gi2:		mov	al,es:[di]
		mov	ds:[si],al
		inc	si
		push	ds
		ldataseg
		grafadd_di 0001h
		pop	ds
		loop	gi2

		push	ds
		ldataseg
		mov	ax,[_screenwide]
		pop	ds
		normalize

		pop	dx
		pop	cx
		loop	gi1

		ldataseg

		call	showcursor

		pop	si di ds
		pop	bp

		ret

_get_image	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;this function copies an image fragment to the screen
;	args	-       x
;			y
;			image
;	returns	-	void
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

		public	_put_image
_put_image	proc	far

		push	bp
		mov	bp,sp
		push	ds di si

		call	hidecursor

		mov     si,[bp+_aoff+4]
		mov	ds,[bp+_aoff+6]

		mov	dx,ds:[si + 0]		;get width
		inc	dx
		mov	cx,ds:[si + 2]		;get depth
		inc 	cx
		add	si,0004h

pi1:		push    cx
		push	dx

		push	ds
		ldataseg
		mov	bx,[bp+_aoff+2]
		call	vgaline
		grafadd_di [bp+_aoff+0]
		pop	ds

		inc	word ptr [bp+_aoff+2]
		pop	cx
		push	cx

		cld
pi2:		mov	al,ds:[si]

		push	ds
		ldataseg
		graf_stosb
		pop	ds

		inc	si
		loop	pi2

		push	ds
		ldataseg
		mov	ax,[_screenwide]
		pop	ds
		normalize

		pop	dx
		pop	cx
		loop	pi1

		ldataseg
		call	showcursor

		pop	si di ds
		pop	bp

		ret

_put_image	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
		public _setpixel
;
; This function sets the pixel at (x,y) to colour
;
; Destroys :
;
_spel_parms struc
		dd		?
		dw		?
	   x dw		?
	   y dw		?
	 col db		?
		db		?
_spel_parms ends

_setpixel	proc	far

		push	bp
		mov	bp,sp
		push	di

		mov	dx,[bp+x]
		mov	bx,[bp+y]

		shl	bx,2

		mov	di,[_screentbl + bx]
		mov	ax,[_screentbl + 2 + bx]

		call	cs:[setbank]
		mov	[pagebreak],ah
		mov	es,[_screenseg]

		grafadd_di dx

		mov	al,[bp+col]
		stosb

		pop	di
		pop	bp

		ret

_setpixel	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
		public	_framerect
;
; This function draws a rectangle
;
; Destroys :
;
fr_parms	struc
		dd		?
		dw		?
   fra_x1 dw		?
   fra_y1	dw		?
   fra_x2 dw		?
   fra_y2 dw		?
   colour db		?
		db		?
fr_parms	ends

_framerect	proc	far

		push	bp
		mov	bp,sp
		push	di si

		call	hidecursor

		mov	bx,[bp+fra_y1]		;top line
		mov	cx,[bp+fra_y2]		;bottom line
		sub	cx,bx

		mov	dx,[bp+fra_x2]
		sub	dx,[bp+fra_x1]		;get width

;		draw top line
		push	cx
		push	bx
		call	vgaline
		grafadd_di [bp+fra_x1]

		mov	cx,dx
		inc	cx
		mov	al,[bp+colour]		;colour

		graf_repnestosb

		pop	bx
		pop	cx

;		draw sides
fr1:		push	cx
		push	dx
		inc	bx

		push	dx
		mov	dx,[bp+fra_x1]		;left
		mov	al,[bp+colour]		;colour

		push	ax
		call	vgaline
		grafadd_di dx
		pop	ax
		stosb

		pop	cx
		add	dx,cx

		push	ax
		call	vgaline
		grafadd_di dx
		pop	ax
		stosb

		pop	dx
		pop	cx
		loop	fr1

;		draw bottom line
		call	vgaline
		grafadd_di [bp+fra_x1]

		mov	cx,dx
		inc	cx
		mov	al,[bp+colour]		;colour

		graf_repnestosb
		call	showcursor

		pop	si di
		pop	bp

		ret

_framerect	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
		public	_fill_rect
;
; Fills a rectangle
;
; Destroys :
;
fill_parms struc
		dd		?
		dw		?
  fill_x1 dw		?
  fill_y1	dw		?
  fill_x2 dw		?
  fill_y2 dw		?
   colour db		?
		db		?
fill_parms ends

_fill_rect proc far

		push	bp
		mov	bp,sp

		call	hidecursor

		mov	bx,[bp+fill_y1]	;top line
		mov	cx,[bp+fill_y2]	;bottom line
		sub	cx,bx
		inc	cx

		mov	dx,[bp+fill_x2]
		sub	dx,[bp+fill_x1]	;get width
ir1:
		push	cx
		push	bx
		call	vgaline
		grafadd_di [bp+fill_x1]

		mov	cx,dx
		inc	cx
		mov	al,[bp+colour]		;colour

		graf_repnestosb

		pop	bx
		pop	cx
		inc	bx
		loop ir1

		call	showcursor

		pop	bp

		ret

_fill_rect	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
		public _set_rgb
;
; Redefines DAC index to r,g,b thus changing the colour
;
; Destroys : dx       Saves : bp
;            ax
;
srgb_parms struc
		dd		?
		dw		?             	; return address & pushed BP
    index db		?              ; colour to redefine
		db		?
    red	db		?              ; red element
		db		?
    green	db		?              ; green element
		db		?
    blue	db		?              ; blue element
		db		?
srgb_parms ends

_set_rgb proc far

		push	bp
		mov	bp,sp

		mov	dx,3c8h               ; set DAC write register
		mov	al,byte ptr[bp+index]
		out	dx,al
		inc	dx
		mov	al,byte ptr[bp+red]   ; set red,green,components of colour
		out	dx,al
		mov	al,byte ptr[bp+green]
		out	dx,al
		mov	al,byte ptr[bp+blue]
		out	dx,al

		pop	bp
		ret

_set_rgb endp

line_deltax	equ	2
line_deltay	equ	4
line_halfx	equ	6
line_halfy	equ	8
line_count	equ	10
line_adj	equ	12

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
		public	_draw_line
;
; Draws a line
;
dl_parms	struc
   deltax dw		?
   deltay dw		?
    halfx dw		?
    halfy dw		?
    count dw		?
	 adj dw		?
		dd		?
		dw		?
    dl_x1 dw		?
    dl_y1 dw		?
    dl_x2 dw		?
    dl_y2 dw		?
   dl_col db		?
		db		?
dl_parms	ends

_draw_line	proc	far

		push	bp
		mov	bp,sp
		sub	sp,line_adj
		push	ds
		push	es

		ldataseg

		call	hidecursor

		mov	ax,[bp+_aoff+6]
		sub	ax,[bp+_aoff+2]	;bottom - top = delta y

		mov	si,1
		jge	line1
		mov	si,-1
		neg	ax
line1:
		mov	[bp - line_deltay],ax
		mov	ax,[bp+_aoff+4]
		sub	ax,[bp+_aoff+0]	;right - left = delta x
		mov	di,1
		jge	line2
		mov	di,-1
		neg	ax
line2:
		mov	[bp - line_deltax],ax
		mov	ax,[bp - line_deltax]
		cmp	ax,[bp - line_deltay]
		jl	line3

		;handle slope < 1
		mov	ax,[bp - line_deltax]
		shr	ax,1
		mov	[bp - line_halfx],ax

		mov	cx,[bp+_aoff+0]
		mov	dx,[bp+_aoff+2]
		mov	bx,0000h
		mov	ax,[bp - line_deltax]
		mov	[bp - line_count],ax

line5:
		push	bx			;plot the pixel
		push	di
		push	dx
		mov	bx,dx
		mov	dx,cx
		mov	ax,[bp+_aoff+8]

		push	ax
		call	vgaline
		grafadd_di dx
		pop	ax
		stosb

		pop	dx
		pop	di
		pop	bx

		add	cx,di
		add	bx,[bp - line_deltay]
		cmp	bx,[bp - line_halfx]
		jle	line6
		sub	bx,[bp - line_deltax]
		add	dx,si
line6:
		dec	word ptr [bp - line_count]
		jge	line5
		jmp	line4

		; handle slope > 1
line3:
		mov	ax,[bp - line_deltay]
		shr	ax,1
		mov	[bp - line_halfy],ax

		mov	cx,[bp+_aoff+0]
		mov	dx,[bp+_aoff+2]
		mov	bx,0000h
		mov	ax,[bp - line_deltay]
		mov	[bp - line_count],ax
line7:
		push	bx			;plot the pixel
		push	di
		push	dx
		mov	bx,dx
		mov	dx,cx
		mov	ax,[bp+_aoff+8]

		push	ax
		call	vgaline
		grafadd_di dx
		pop	ax
		stosb

		pop	dx
		pop	di
		pop	bx

		add	dx,si
		add	bx,[bp - line_deltax]
		cmp	bx,[bp - line_halfy]
		jle	line8
		sub	bx,[bp - line_deltay]
		add	cx,di
line8:
		dec	word ptr [bp - line_count]
		jge	line7
line4:
		ldataseg
		call	showcursor

		pop	es
		pop	ds
		add	sp,line_adj
		pop	bp


		ret

_draw_line	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
		public	_DrawString
;
; This function draws a string of text
;
_dstr_parms struc
		dd		?
		dw		?
	  sx dw		?
	  sy dw		?
	xstr dd 		?
	xcol db		?
		db		?
 back_col db		?
		db		?
_dstr_parms ends

_DrawString	proc	far

		push	bp
		mov	bp,sp
		push	si di

		call	hidecursor
		mov	cx,[_fontdepth]
ds0:
		push	cx
		dec	cx

		mov	bx,[bp+sy]
		add	bx,cx

		ldataseg
		call	vgaline
		grafadd_di [bp+sx]

		lds	si,[bp+xstr]
ds1:
		mov	ah,00h
		mov	al,ds:[si]
		cmp	al,00h
		je	ds5
ds2:
		push	ds
		push	si

		mul  [_fontdepth]
		mov	bx,ax
		add	bx,cx
		mov	si,[_fontoff]
		mov	ds,[_fontseg]
		mov	ah,ds:[si+bx]

		push	cx
		mov	cx,0008h
		mov	bx,0000h

		ldataseg
ds3:
		mov	al,byte ptr [bp+xcol]
		test	ah,[_masktable+bx]
		jnz	ds4
		mov	al,byte ptr [bp+back_col]
ds4:
		inc	bx
		graf_stosb
		loop	ds3

		pop	cx

		pop	si
		pop	ds
		inc	si
		jmp	ds1
ds5:
		pop	cx
		loop	ds0

		call	showcursor

		pop	di si
		pop	bp

		ret

_DrawString	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;this function initializes the mouse and graphic mode
;	args	-	screenwide
;			screendeep
;			card
;	returns	-	true for intialized
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

		public	_InitMouse
_InitMouse	proc	far

		push	bp
		mov	bp,sp

		mov	[codepage],0ffffh

		mov	ax,[bp + _aoff + 0]
		mov	[_screenwide],ax

		mov	ax,[bp + _aoff + 2]
		mov	[_screendeep],ax

		mov	ax,[bp + _aoff + 4]
		mov	[_card],ax

		mov	ax,cs
		mov	cs:[setbank_seg],ax

		cmp	[_card],paradise
		jne	initmouse1

		mov	ax,offset setbank_par
		mov	cs:[setbank_off],ax
		call	maketable

		mov	ax,[_card]
		int	10h

		jmp	initmouse7
initmouse1:
		cmp	[_card],ati
		jne	initmouse2

		mov	ax,offset setbank_ati
		mov	cs:[setbank_off],ax
		call	maketable

		mov	ax,0c000h
		mov	es,ax
		mov	ax,es:[0010h]
		mov	cs:[atireg],ax
		mov	ax,[_card]
		int	10h

		jmp	initmouse7
initmouse2:
		cmp	[_card],tseng
		jne	initmouse3

		mov	ax,offset setbank_tng
		mov	cs:[setbank_off],ax
		call	maketable

		mov  dx,03c4h                	;set tseng registers
		mov  al,0
		out  dx,al
		mov  dx,03c5h
		in   al,dx
		and  al,0feh
		out  dx,al
		mov  dx,03ceh
		mov  al,06h
		out  dx,al
		mov  dx,03cfh
		in   al,dx
		xor  al,06h
		or   al,02h
		out  dx,al

		mov	ax,[_card]
		int	10h

		jmp	initmouse7
initmouse3:
		cmp	[_card],trident
		jne	initmouse4
		mov	ax,offset setbank_tri
		mov	cs:[setbank_off],ax
		call	maketable
		mov	ax,[_card]
		int	10h
		jmp	initmouse7
initmouse4:
		cmp	[_card],oak
		jne	initmouse5

		mov	ax,offset setbank_oak
		mov	cs:[setbank_off],ax
		call	maketable
		mov	ax,[_card]
		int	10h

		jmp	initmouse7
initmouse5:
		cmp	[_card],vesa
		jne	initmouse6

		mov	ax,offset setbank_vsa
		mov	cs:[setbank_off],ax
		call	initvesa

		jmp	initmouse7
initmouse6:
		mov	ax,0000h
		jmp	initmouse8

initmouse7:	;intitalize the font pointer
		mov	ax,1130h
		mov	bx,0300h
		int	10h
		mov	[_fontseg],es
		mov	[_fontoff],bp
		mov	[_fontdepth],0008h

		;see if there's a mouse handler
		mov	ax,3533h
		int	21h
		mov	ax,es
		or	ax,bx
		jz	initmouse8

		;initialize the mouse
		mov	ax,0000h
		int	33h

		;check to see it's there
		cmp	ax,0000h
		je	initmouse8

		;set the horizontal range
		mov	ax,0007h
		mov	cx,0000h
		mov	dx,[_screenwide]
		sub	dx,cursorwide
		shl	dx,1
		shl	dx,1
		int	33h

		;set the vertical range
		mov	ax,0008h
		mov	cx,0000h
		mov	dx,[_screendeep]
		sub	dx,cursordeep
		int	33h

		;put the cursor in the centre of the screen
		mov	ax,0004h
		mov	cx,[_screenwide]
		shl	cx,1
		mov	dx,[_screendeep]
		shr	dx,1
		int	33h

		;set the mouse sensitivity
		mov	ax,000fh
		mov	cx,0003h
		mov	dx,0010h
		int	33h

		;set the initial position
		mov	ax,[_screenwide]
		shr	ax,1
		mov	[mousex],ax

		mov	ax,[_screendeep]
		shr	ax,1
		mov	[mousey],ax

		;show the cursor
		call	showcursor

		;set the mouse handler hook
		mov	ax,cs
		mov	es,ax
		mov	ax,000ch
		mov	cx,001fh
		mov	dx,offset mousehandler
		int	33h

		;say all is well
		mov	ax,0ffffh

initmouse8:

		pop	bp

		ret

_InitMouse	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
		public	_DeinitMouse
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

_DeinitMouse	proc	far

		push	bp
		mov	bp,sp

		mov	ax,0000h			;mouse off
		int	33h

		cmp	[_card],vesa
		jne     dm3
		cmp	[oldvesamode],0ffffh
		je	dm3

		mov	di,word ptr [videomode+0]	;if the old mode
		mov	es,word ptr [videomode+2]	;is a legal vesa
							;mode, change to
dm1:		mov	ax,es:[di]			;it... otherwise
		add	di,0002h			;return to the
							;standard text
		cmp	ax,0ffffh			;mode
		je	dm2
		cmp	ax,[oldvesamode]
		je	dm2
		jmp	dm1

dm2:      cmp	ax,0ffffh			;mode not found
		je	dm3

		mov	ax,4f02h
		mov	bx,[oldvesamode]
		int	10h

		jmp	dm4

dm3:		mov	ax,0003h
		int	10h

dm4:
		pop	bp

		ret

_DeinitMouse	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;		arg1 = colour to clear to
		public	_ClearScreen
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
_ClearScreen	proc	far

		push	bp
		mov	bp,sp

		call	hidecursor

		mov	cx,[_screendeep]
		mov	bx,0000h

cs1:		push	cx
		push	bx

		call	vgaline
		mov	al,byte ptr [bp + _aoff + 0]
		mov	cx,[_screenwide]
		graf_repnestosb

		pop	bx
		pop	cx
		inc	bx
		loop	cs1

		ldataseg
		call	showcursor

		pop	bp

		ret

_ClearScreen	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;		internal functions
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;make a line start table with flags for page breaks
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
maketable	proc	near

		mov	cx,[_screendeep]		;depth of screen
		mov	bx,0000h				;zero line counter
		mov	si,offset _screentbl	;point to table
mt1:
		push	cx
		mov	ax,[_screenwide]		;width of screeen
		mul	bx                       ;times line number

		and	dx,00ffh

		mov	cx,0fffeh
		sub	cx,[_screenwide]

		cmp	ax,cx				;see if we will
		jb	mt2					;exceed a page boundary

		mov	dh,0ffh
mt2:
		mov	ds:[si],ax			;save the values
		mov	ds:[si+2],dx			;in our lookup table
		add	si,4					;and point to the
		inc	bx					;next line and

		pop	cx					;entry
		loop	mt1

		ret

maketable	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; handle mouse movement - this function called whenever the mouse
; changes state. it's called by the mouse driver
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

mousehandler	proc	far

		pushf
		cli

		ldataseg

		mov	[mouseb],bx

		cmp	[mousef],0000h
		jl	mh1

		push	ax
		push	bx
		push	cx
		push	dx
		call	erasecursor
		pop	dx
		pop	cx
		pop	bx
		pop	ax

		mov	[mousex],cx
		mov	[mousey],dx

		shr	word ptr [mousex],2

		call	cursorback
		call	drawcursor
mh1:
		popf
		retf

mousehandler	endp

		public _mouse_down
_mouse_down proc far

		push bp
		mov 	bp,sp

		mov	di,[bp+_aoff+0]
		mov	es,[bp+_aoff+2]

		mov	ax,[mousex]
		add	ax,[spotx]
		stosw

		mov	ax,[mousey]
		add	ax,[spoty]
		stosw

		mov	ax,[mouseb]
		and	ax,0003H

		pop	bp

		ret

_mouse_down	ENDP

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

drc_struc struc
  h_count dw 		?
  v_count dw		?
    scr_l dw		?
		dd		?
		dw		?
drc_struc ends

drawcursor	proc	near

		push bp
		sub	sp,6
		mov	bp,sp

		mov	[bp+v_count],cursordeep
		mov	ax,[mousey]
		mov	[bp+scr_l],ax

		mov	si,offset defaultcursor
dc1:
		mov	bx,[bp+scr_l]

		call	vgaline
		grafadd_di [mousex]

		mov	[bp+h_count],cursorwide
		xor	bx,bx
dc2:
		mov	cx,bx
		mov  dx,8000h
		shr	dx,cl
		mov	ax,ds:[si]
		xchg	ah,al
		test	ax,dx
		jnz	dc3
		mov	ax,[_vgablack]
		graf_stosb
		jmp	dc4
dc3:
		grafadd_di	1
dc4:
		inc	bx
		dec	[bp+h_count]
		jnz	dc2

		mov	bx,[bp+scr_l]

		call	vgaline
		grafadd_di [mousex]

		mov	[bp+h_count],cursorwide
		xor	bx,bx
dc5:
		mov	cx,bx
		mov  dx,8000h
		shr	dx,cl
		mov	ax,ds:[si+32]
		xchg	ah,al
		test	ax,dx
		jz	dc6
		mov	ax,[_vgawhite]
		graf_stosb
		jmp	dc7
dc6:
		grafadd_di	1
dc7:
		inc	bx
		dec	[bp+h_count]
		jnz	dc5

		inc	[bp+scr_l]
		add	si,0002h

		dec	[bp+v_count]
		jnz	dc1

		pop	bp
		add	sp,6
		ret

drawcursor	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;erase the cursor
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
erasecursor	proc	near

		ldataseg

		mov	cx,cursordeep
		mov	bx,[mousey]

		mov	si,offset mouseback
ec1:
		push	cx

		call	vgaline
		grafadd_di [mousex]

		mov	cx,cursorwide
		graf_repnemovsb

		inc	bx
		pop	cx
		loop	ec1

		ret

erasecursor	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;copy the cursor background to memory
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
cb_struc	struc
 v_count2 dw		?
		dd		?
		dw		?
cb_struc	ends

cursorback proc near

		push	bp
		sub	sp,2
		mov	bp,sp

		mov	ax,cursordeep
		mov	[bp+v_count2],ax
		mov	bx,[mousey]

		mov	si,offset [mouseback]
cc1:
		call	vgaline
		grafadd_di	[mousex]

		mov	cx,cursorwide
cc2:
		mov	al,es:[di]
		mov	ds:[si],al
		inc	si
		grafadd_di 0001h
		loop	cc2

		inc	bx
		dec	[bp+v_count2]
		jnz	cc1

		add	sp,2
		pop	bp
		ret

cursorback endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; make the cursor disappear
		public	hidecursor
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
hidecursor proc far

		pushf
		cli
		dec	[mousef]
		cmp	[mousef],-1
		jl	hc1
		call erasecursor
hc1:
		popf
		ret

hidecursor	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
; show the cursor
		public	showcursor
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
showcursor	proc	far

		pushf
		cli
		inc	[mousef]
		cmp	[mousef],0000h
		jg	sc1
		call	cursorback
		call	drawcursor
sc1:
		popf
		ret

showcursor	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;initialize a vesa board
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
initvesa	proc	near

		mov	ax,_data
		mov	es,ax
		mov	ax,4f00h
		mov	di,offset vesablock
		int	10h
		cmp	ax,004fh
		je	iv1
		mov	ax,0000h
		ret
iv1:
		mov	si,word ptr [videomode]
		mov	es,word ptr [videomode+2]
iv2:
		mov     ax,es:[si]
		add	si,0002h
		cmp	ax,0ffffh
		je	iv3
		cmp	ax,vesa
		je	iv4
		jmp	iv2
iv3:
		mov	ax,0000h
		ret
iv4:
		mov	ax,_data
		mov	es,ax
		mov	ax,4f01h
		mov	cx,vesa
		mov	di,offset vesainfo
		int	10h

		mov	ax,[win_aseg]
		mov	[_screenseg],ax

		mov	ax,4f03h
		int	10h
		mov	[oldvesamode],bx

		mov	ax,4f02h
		mov	bx,vesa
		int	10h

		call	findvesashift
		mov	[bankshift],ax

		mov	ax,[winsize]
		mov	bx,0400h
		mul	bx
		sub	ax,0002h
		sub	ax,[bytesperline]
		mov	[banksize],ax

		mov	cx,[yres]			;depth of screen
		sub	bx,bx                           ;zero line counter
		mov	si,offset _screentbl		;point to table
iv5:
		push	cx
		mov	ax,[bytesperline]			;width of screeen
		mul	bx                              ;times line number
		and	dx,7fffh
		cmp	ax,[banksize]			;see if we will exceed the block
		jb	iv6
		or	dx,1000h
iv6:
		mov	ds:[si],ax			;save the values
		mov	ds:[si+2],dx			;in our lookup table
		add	si,4				;and point to the
		inc	bx				;next line and
		pop	cx				;entry
		loop	iv5
		mov	ax,0ffffh

		ret

initvesa	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;find the shift value for vesa bank numbers
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
findvesashift	proc	near

		mov	dx,0000h
		mov	ax,cs:[winsize]
		div	cs:[wingran]

		cmp	ax,0040h
		jne	fvs1
		mov	ax,0006h
		ret
fvs1:
		cmp	ax,0020h
		jne	fvs2
		mov	ax,0005h
		ret
fvs2:
		cmp	ax,0010h
		jne	fvs3
		mov	ax,0004h
		ret
fvs3:
		cmp	ax,0008h
		jne	fvs4
		mov	ax,0003h
		ret
fvs4:
		cmp	ax,0004h
		jne	fvs6
		mov	ax,0002h
		ret
fvs6:
		cmp	ax,0002h
		jne	fvs7
		mov	ax,0001h
		ret
fvs7:
		mov	ax,0000h
		ret

findvesashift	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;		return a pointer in es:di to the screen line in bx
		public	vgaline
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
vgaline   proc	far

		push	bx
		push	cx

		shl	bx,2

		mov	di,[_screentbl + bx]
		mov	ax,[_screentbl + 2 + bx]
		call	cs:[setbank]

		mov	[pagebreak],ah
		mov	es,[_screenseg]

		pop	cx
		pop	bx

		ret

vgaline	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
setbank_ati	proc	far
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
		cmp	ax,[codepage]
		je	setb_ati1

		mov	[codepage],ax

		push	ax
		push	cx
		push	dx

		mov	dx,[atireg]

		mov	cx,ax
		pushf
		cli
		mov	al,0b2h
		out	dx,al
		inc	dl
		in	al,dx
		mov	ah,al
		and	ah,0e1h
		shl	cl,1
		or	ah,cl
		mov	al,0b2h
		dec	dl
		out	dx,ax
		popf
		pop	dx
		pop	cx
		pop	ax

setb_ati1:
		ret

setbank_ati	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
setbank_tri	proc	far
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
		cmp	ax,[codepage]
		je	setb_tri1

		mov	[codepage],ax

		push	ax
		push	bx
		push	dx

		mov	dx,3ceh		;set page size to 64k
		mov	al,6
		out	dx,al
		inc	dl
		in	al,dx
		dec	dl
		or	al,4
		mov	ah,al
		mov	al,6
		out	dx,ax

		mov	dl,0c4h		;switch to bps mode
		mov	al,0bh
		out	dx,al
		inc	dl
		in	al,dx
		dec	dl

		mov	bx,[codepage]
		mov	ah,bl
		xor	ah,2
		mov	dx,3c4h
		mov	al,0eh
		out	dx,ax

		pop	dx
		pop	bx
		pop	ax

setb_tri1:
		ret

setbank_tri	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
setbank_tng	proc	far
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
		cmp	ax,[codepage]
		je	setb_tng1

		mov	[codepage],ax
		push	ax
		push	dx
		mov	ah,al
		mov	dx,3bfh
		mov	al,3
		out	dx,al
		mov	dl,0d8h
		mov	al,0a0h
		out	dx,al
		and	ah,15
		mov	al,ah
		shl	al,1
		shl	al,1
		shl	al,1
		shl	al,1
		or	al,ah
		mov	dl,0cdh
		out	dx,al
		pop	dx
		pop	ax

setb_tng1:
		ret

setbank_tng	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
setbank_par	proc	far
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
		cmp	ax,[codepage]
		je	setb_par1

		push	ax
		push	cx
		push	dx

		and	ax,00ffh
		mov	[codepage],ax

		mov	dx,03ceh
		mov	ax,050fh
		out	dx,ax

		mov	ax,[codepage]
		mov	cl,12
		shl	ax,cl
		or	ax,0009h
		out	dx,ax

		pop	dx
		pop	cx
		pop	ax

setb_par1:
		ret

setbank_par	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
setbank_oak	proc	far
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
		cmp	ax,[codepage]
		je	setb_oak1

		mov	[codepage],ax

		push	ax
		push	dx
		and	al,15
		mov	ah,al
		shl	al,1
		shl	al,1
		shl	al,1
		shl	al,1
		or	ah,al
		mov	al,11h
		mov	dx,3deh
		out	dx,ax
		pop	dx
		pop	ax

setb_oak1:
		ret

setbank_oak	endp

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
setbank_vsa	proc	far
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
		push	ax

		cmp	ax,[codepage]
		je	setb_vsa4

		mov	[codepage],ax

		push	bx
		push	cx
		push	dx

		mov	dx,ax
		and 	dx,00ffh
		mov	cx,[bankshift]
		shl	dx,cl
		mov	bx,0000h
		mov	ax,4f05h
		cmp	word ptr cs:[winfunptr+2],0000h
		jne  setb_vsa2

		mov	dx,[codepage]
		and	dx,00ffh
		int	10h

		jmp	setb_vsa3

setb_vsa2:
		call	cs:[winfunptr]
setb_vsa3:
		pop	dx
		pop	cx
		pop	bx

setb_vsa4:
		pop	ax
		ret

setbank_vsa	endp

mouse256_text	ends

;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
; all data declared here
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
_data	segment word public 'data'

		public	_screentbl,_screenseg,
		public	_screenwide,_screendeep
		public	_masktable,_bittable
		public	_fontseg,_fontoff,_fontdepth
		public	_VGAWHITE,_VGABLACK,_VGAGREY
		public	_VGALIGHTGREY,_VGADARKGREY
		public	_card
		public	codepage
; screen dimensions
_screentbl	dw	(tabledeep * 2) dup (?)
_screenseg	dw	0a000h
_screenwide	dw	640
_screendeep	dw	480

; font dimensions and pointer
_fontseg		dw	?
_fontoff		dw	?
_fontdepth	dw	?

; mask objects
_masktable	db	80h,40h,20h,10h,08h,04h,02h,01h
_bittable		db   01h,02h,04h,08h,10h,20h,40h,80h

;colours
_VGABLACK		dw	0
_VGAWHITE		dw	0
_VGAGREY		dw	0
_VGALIGHTGREY	dw	0
_VGADARKGREY	dw	0

;vga card type
_card		dw	0

codepage		dw	0
atireg		dw	0
pagebreak		db	0

; where the cursor lives
defaultcursor	db	03fh,0ffh,01fh,0ffh
		db	00fh,0ffh,007h,0ffh
		db	003h,0ffh,001h,0ffh
		db	000h,0ffh,000h,07fh
		db	000h,03fh,000h,01fh
		db	001h,0ffh,010h,0ffh
		db	030h,0ffh,0f8h,07fh
		db	0f8h,07fh,0fch,07fh
		db	000h,000h,040h,000h
		db	060h,000h,070h,000h
		db	078h,000h,07ch,000h
		db	07eh,000h,07fh,000h
		db	07fh,080h,07ch,000h
		db	06ch,000h,046h,000h
		db	006h,000h,003h,000h
		db	003h,000h,000h,000h

spotx		dw	0000h
spoty		dw	0000h

; mouse position and buttons
mousex		dw	?
mousey		dw	?
mouseb		dw	?

; mouse cursor flag
mousef		dw	-1		;if >= 0, mouse is visible

; where the area behind the mouse cursor is stored
mouseback		db	(cursorwide * cursordeep) dup (?)

vesablock	label	word
vesasig		db	4 dup (0)
vesaversion	dw	0
oemstring		dd	0
capabilities	db	4 dup (0)
videomode		dd	0
totalmemory	dw	0
			db	242 dup (0)

vesainfo		label	word
mode_attr		dw	?
wina_attr		db	?
winb_attr		db	?
wingran		dw	?
winsize		dw	?
win_aseg		dw	?
win_bseg		dw	?
winfunptr		dd	?
bytesperline	dw	?
xres			dw	?
yres			dw	?
xcharsize		db	?
ycharsize		db	?
planecount	db	?
bitxperpixel	db	?
banckcount	db	?
memorymodel	db	?
bankvsize		db	?
pagecount		db	?
reserved1		db	?
masks		db	8 dup (0)
directcolour	db	?
reserved2		db	216 dup (0)

oldvesamode	dw	0ffffh
bankshift		dw	0
banksize		dw	0

setbank        label dword
setbank_off    dw	?
setbank_seg	dw	?

_data		ends

		end

ÿ