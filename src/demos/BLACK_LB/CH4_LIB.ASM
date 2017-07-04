;
;				  Chain-4 (Linear) Graphics Library
;				  ---------------------------------
;
;					    (Real mode version)
;
;   All code that follows should be considered the property of Paul Carroll
; and subject to all copyright laws. No part of the code is to be duplicated
;      or published without the authors expressed permission in writing.
;
;       Copyright (c) 1994-96 Paul Carroll - (Black Marble Productions)
;
; 3-1-1996 v1.00
;		- Basic library written with features
;			   	- Sprite putting
;				- Pixel manipulation

		.model large

_ch4_data	segment para public 'data'

_ch4_data ends

_ch4_code segment para public 'code'

		ASSUME cs:_ch4_code,ds:_ch4_data

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _c_set_mode
;
; Sets basic 320x200x256 chain-4 (linear) graphics
; mode.
;
; Destroys : ax	Saves : bp
;
csm_parms struc
		dd		?
		dw		?
csm_parms	ends

_c_set_mode proc

		push	bp
		mov	bp,sp

		mov	ax,13h
		int	10h

		pop	bp
		ret

_c_set_mode endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _c_set_pel
;
; Sets a pixel to colour col at x,y
;
; Destroys : ax	Saves : bp
;
csp_parms	struc
		dd		?
		dw		?
	   x	dw		?
	   y dw		?
	 col db		?
		db		?
csp_parms	ends

_c_set_pel proc

		push	bp
		mov	bp,sp
		push	di

		mov	ax,0a000h
		mov	es,ax
		mov	ax,320
		mul	[bp+y]
		add	ax,[bp+x]
		mov	di,ax
		mov	al,[bp+col]
		mov	[es:di],al

		pop	di bp
		ret

_c_set_pel endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _c_get_pel
;
; Gets a pixel at x,y and returns it
;
; Destroys : ax	Saves : bp
;
cgp_parms	struc
		dd		?
		dw		?
	   x	dw		?
	   y dw		?
cgp_parms	ends

_c_get_pel proc

		push	bp
		mov	bp,sp
		push di

		mov	ax,0a000h
		mov	es,ax
		mov	ax,320
		mul	[bp+y]
		add	ax,[bp+x]
		mov	di,ax
		mov	al,byte ptr[es:di]
		xor	ah,ah

		pop	di bp
		ret

_c_get_pel endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _c_put_lbm
;
; Puts a linear bitmap at x,y transparently
; ie. all 0 pixels are not drawn.
;
; Destroys :		Saves : bp
;
pbm_parms struc
		dd		?
		dw		?
	   x	dw		?
	   y dw		?
   bitmap dd		?
pbm_parms	ends

_c_put_lbm proc

		push bp
		mov	bp,sp
		push	ds di si

		mov	ax,0a000h
		mov	es,ax
		mov	ax,320
		mul	[bp+y]
		add	ax,[bp+x]
		mov	di,ax
		lds	si,[bp+bitmap]
		mov	dx,[ds:si+1]			; Get bitmap height
		xor	dh,dh
		mov	cx,[ds:si]               ; Get bitmap width
		xor	ch,ch
		mov	bx,2
@plbm_yloop:

@plbm_xloop:
		mov	al,[ds:si+bx]            ; Find pixel
		cmp	al,0
		je	@no_put
		mov  [es:di],al               ; Put pixel
@no_put:
		inc	di
		inc	bx
		dec  cx
		jnz	@plbm_xloop              ; Loop back for another pixel

		mov	cx,[ds:si]               ; Get bitmap width
		xor	ch,ch
		sub	di,cx               	; Point to start of next line
		add	di,320

		dec	dx
		jnz	@plbm_yloop              ; Loop back to do another x line

		pop	si di ds bp
		ret

_c_put_lbm endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _c_wait_vr
;
; Wait for existing vr to finish and then wait
; for a full one to pass.
;
; Destroys - ax
;		   dx
;
_c_wait_vr proc far

		mov	dx,3dah            ; VGA status port
@vr_1:
		in	al,dx              ; Get current staus
		test	al,8               ; are we in a retrace?
		jnz	@vr_1              ; yes, so loop back
@vr_2:
		in	al,dx              ; Get status and wait for
		test	al,8               ; a FULL retrace
		jz	@vr_2

		ret

_c_wait_vr endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _c_set_rgb
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

_c_set_rgb proc far

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

_c_set_rgb endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _c_set_palette
;
; Redfines DAC lookup to array passed as pal_array.
;
; Destroys : ax 	Saves : bp
;		   dx             ds
;		   cx             si
;
sp_parms	struc
		dd		?
		dw		?              ; return address & pushed BP
pal_array dd		?              ; pointer to palette array
sp_parms	ends

_c_set_palette proc

		push	bp                  ; preserve callers stack frame
		mov	bp,sp
		push si                  ; save C's register variables
		push ds

		mov  dx,3c8h             ; set dac write register to 0
		xor	al,al
		out  dx,al
		mov  cx,768              ; loop 768 times (256*3)
		lds 	si,[bp+pal_array]   ; point to the palette array
@set_pal:
		mov 	dx,3c9h
		mov 	al,[ds:si]          ; write an element
		out  dx,al
		inc	si                  ; point to next element
		dec	cx                  ; have we done them all?
		jnz	@set_pal            ; no, so loop back

		pop	ds
		pop	si
		pop	bp
		ret

_c_set_palette endp

_ch4_code	ends

		end


