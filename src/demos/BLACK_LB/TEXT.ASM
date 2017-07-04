
;                          Text Mode Library
;                          -----------------
;
;                   Copyright (c) 1994-96 Black Marble
;
;     This source code should be considered to be protected by the
;  laws of copyright (c) 1996 with all rights reserved by the author
;  Paul Carroll. No part of this file may be published or distributed
;  in any way shape or form with the author's expressed permission in
;                              writing.
;
		.MODEL large

_text_data segment para public 'data'

_text_data ends

_text_code segment para public 'code'

		ASSUME cs:_text_code,ds:_text_data

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _t_put_char
;
; Puts a single char with attrib to x,y
;
; Destroys:		Saves:
;
pc_parms	struc
		dd		?
		dw		?
	   x dw		?
	   y dw		?
	char db		?
		db		?
	attr db		?
		db		?
pc_parms  ends

_t_put_char proc

		push	bp   			; Setup stack frame
		mov	bp,sp
		push di

		mov 	ax,[bp+y]
		mov 	bx,80
		mul 	bx
		add 	ax,[bp+x]
		mov 	bx,2
		mul 	bx
		mov 	di,ax               ; Calc offset
		mov 	ax,0b800h
		mov 	es,ax               ; Point to video memory
		mov 	al,[bp+char]        ; Get character to write
		mov 	ah,[bp+attr]        ; Get char's attribute
		mov 	es:[di],al          ; Write the char
		inc 	di
		mov 	es:[di],ah          ; Write the attr

		pop	di bp
		ret

_t_put_char endp


;北北北北北北北北北北北北北北北北北北北北北北
;
		public _t_attr

; Changes the attrib of a char at a given x,y
;
; Destroys :		Saves : bp
;
att_parms	struc
		dd		?
		dw		?
	   x dw		?
	   y	dw		?
   attrib db		?
		db		?
att_parms	ends

_t_attr	proc

		push	bp
		mov	bp,sp
		push di

		mov 	ax,[bp+y]
		mov 	bx,80
		mul 	bx
		add 	ax,[bp+x]
		mov 	bx,2
		mul 	bx
		mov 	di,ax
		add 	di,1
		mov 	ax,0b800h
		mov 	es,ax
		mov 	al,[bp+attrib]
		mov 	es:[di],al

		pop	di bp
		ret

_t_attr	endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _t_get_char
;
; Gets ASCII num and attrib of char at x,y
;
; Destroys :		Saves :
gc_parms	struc
		dd		?
		dw		?
	   x dw		?
	   y dw		?
gc_parms	ends

_t_get_char proc

		push	bp
		mov	bp,sp
		push di

		mov 	ax,[bp+y]                ; Calc ofs to char on screen
		mov 	bx,80
		mul 	bx
		add 	ax,[bp+x]
		mov 	bx,2
		mul 	bx
		mov 	di,ax
		mov 	ax,0b800h                ; Point to vid mem
		mov 	es,ax
		mov 	al,es:[di]               ; Get the char
		inc 	di
		mov 	ah,es:[di]               ; Get the attr

		pop	di bp

		ret

_t_get_char endp


;北北北北北北北北北北北北北北北北北北北北北北
;
		public _t_set_80x50
;
; Sets screen to text mode 80x50
;
; Destroys :		Saves : bp
;
s85_parms	struc
		dd		?
		dw		?
s85_parms ends

_t_set_80x50 proc

		push	bp
		mov	bp,sp

		mov	ax,3  				; Set text mode 3 (80x25)
		int	10h
		mov 	ax,1112h                 ; Load VGA BIOS 8x8 font
		mov 	bl,0                     ; and we'll be in 80x50!
		int 	10h

		pop  bp
		ret

_t_set_80x50 endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _t_put_scr
;
; Puts an array to the whole screen. First two
; bytes of the array show the dimensions.
;
; Destorys :		Saves
;
psc_parms	struc
		dd		?
		dw		?
	 scr dd		?
psc_parms ends

_t_put_scr proc

		push bp
		mov	bp,sp
		push	si

		mov	ax,0b800h            	; Point to text mode video memory
		mov	es,ax
		xor	di,di
		lds	si,[bp+scr]              ; Point to our screen

		mov	ax,[ds:si]               ; Calc bytes to move
		mul	[ds:si+1]
		shl	ax,2                     ; Make sure we do it in words
		mov	cx,ax
		add	si,2                     ; Skip past the width and height

		rep	movsw                    ; Word aligned put to screen

		pop	si bp
		ret

_t_put_scr endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _t_set_rgb
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

_t_set_rgb proc far

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

_t_set_rgb endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _t_hide_cursor
;
; Hides the text mode cursor
;
; Destroys :		Saves :
;
hc_parms	struc
		dd		?
		dw		?
hc_parms	ends

_t_hide_cursor proc

		push	bp
		mov	bp,sp

		mov	dx,3d4h
		in	al,dx
		mov	al,10000b
		mov	ah,0ah
		xchg	ah,al
		out	dx,ax

		pop	bp
		ret

_t_hide_cursor endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _t_show_cursor
;
; Shows the text mode cursor
;
; Destroys :		Saves :
;
sc_parms	struc
		dd		?
		dw		?
sc_parms	ends

_t_show_cursor proc

		push bp
		mov	bp,sp

		mov	dx,3d4h
		in	al,dx
		mov	al,110b
		mov	ah,0ah
		xchg	ah,al
		out	dx,ax

		pop	bp
		ret

_t_show_cursor endp

		end

