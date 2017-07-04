;               ! For large memory model !
;
;	      	   MODE-X GRAPHICS LIBRARY
;	             -----------------------
;
;                   (Real mode version)
;
;       This graphics programming library should be
;	considered under the laws of Copyright. No code may
;	  be reproduced in any shape or form without the
;	          authors expressed permission.
;
; All code here after is Copyright (c) 1995-96 Black Marble
;
;   25-11-95 v1.0 :
;		- Basic mode switching
;		- Misc functions such as pel setting and hpp
;           scr width and scan start changing etc...
;
;   27-11-95 v1.01 :
;		- Added first double buffering features
;
;   30-11-95 v1.10 :
;         - Perfected double buffering
;		- Added 	- transparent bitmap putting
;				- activate page switching
;                   - byte/dword mode changes
;				- word driven page clear
;
;   31-11-95 v1.11 :
;		- Fixed some SI, DI saves because C temp vars were being killed

		.model large

_x_data	segment para public 'data'

scr_p_width    dw		?                 ; Physical screen dimensions
scr_width      dw		?                 ; Logical screen dimensions
scr_height     dw		?
page0		dw		?                 ; Page offsets for double buffering
page1		dw		?
dbuf_set		db		?			   ; Are we double buffering?
db_ptr		db		?			   ; Which page are we displaying?
cp_offset		dw		?			   ; Current page's offset	

errmsg1		db 'Dimensions are too big for a double buffer!',13,10,'$'

_x_data   ends

_x_code   segment para public 'code'

		ASSUME cs:_x_code,ds:_x_data

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_setmode
;
; Sets VGA to 'mode-x' by the chain-4 bit
; in the SC_INDEX (port 3c4h)
;
; Destroys : ax        Saves : di
;		   dx
;		   cx
;		   es
;
_x_setmode proc far

		push	di

		mov	ax,13h             ; Let BIOS setup basic mode 13h (linear)
		int	10h

		mov	dx,3c4h
		mov	ax,604h
		out	dx,ax              ; Unchain VGA (YAY!)
		mov	ax,0f02h
		out	dx,ax              ; Turn on dword mode
		mov	ax,0a000h          ; set up to clear video memory
		mov	es,ax              ; this is nessacary as the mode tweaking
		xor	di,di              ; leaves some rubbish behind
		xor	ax,ax
		mov	cx,32000
		cld
		rep  stosw              ; Clear video memory

		mov	dx,3d4h
		mov	ax,14h
		out	dx,ax
		mov	ax,0e317h          ; Switch to byte mode
		out	dx,ax

		mov  [dbuf_set],0	    ; Default to no double-buffering
		mov  [scr_width],320    ; Keep screen dimensions for later
		mov	[scr_height],200
		mov  [scr_p_width],80

		mov	[cp_offset],0

		pop	di
		ret

_x_setmode endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_textmode
;
; Reset the VGA back to text mode (80x25)
;
; Destorys - ax
;
_x_textmode proc

		mov 	ax,03h
		int	10h

		ret

_x_textmode endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_wait_vr
;
; Wait for existing vr to finish and then wait
; for a full one to pass.
;
; Destroys - ax
;		   dx
;
_x_wait_vr proc far

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

_x_wait_vr endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_pel
;
; Sets one pixel in 'mode-x'
;
; Destroys : ax        Saves: bp
;		   dx
;		   cx
;		   bx
;		   es
;
spel_parms struc
		 dd      ?
		 dw      ?        ; return address & pushed BP
	x	 dw      ?        ; x coordinate of pixel
	y     dw      ?        ; y coordinate of pixel
	col	 db	    ?        ; colour of pixel
		 db	    ?
spel_parms ends

_x_set_pel proc far

		push bp                  ; Preserve callers stack frame
		mov  bp,sp

		mov	ax,0a000h
		mov	es,ax
		mov	dx,3c4h             ; find and set plane pixel
		mov	ah,2                ; is on.
		mov	cx,[bp+x]			; plane = 1 << (x&3)  (or x mod 4)
		and	cx,3
		mov	al,1
		shl	al,cl
		xchg	ah,al
		out	dx,ax               ; Set VGA write plane

		mov	ax,[scr_p_width]    ; Calculate physical offset
		mul	[bp+y]              ; screen_width*y_offset

		mov	bx,[bp+x]
		shr	bx,2                ; divide x by 4 to get physical x offset
		add	bx,ax               ; add x to y offset
		add	bx,[cp_offset]
		mov	al,[bp+col]         ; get the pixel colour
		mov	es:[bx],al          ; put pixel to screen

		pop	bp
		ret

_x_set_pel endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_get_pel
;
; Returns value of a pixel at x,y in 'mode-x'
;
; Destroys : ax	Saves : bp
;		   bx
;		   cx
;		   dx
;		   es
;
getp_parms struc
		 dd	    ?
		 dw	    ?
	x	 dw	    ?               ; pixel's x pos
	y	 dw	    ?               ; it's y pos
getp_parms ends

_x_get_pel proc far

		push	bp                  ; preserve callers stack frame
		mov	bp,sp

		mov	ax,0a000h
		mov	es,ax
		mov	dx,03ceh            ; find and set plane pixel
		mov	al,4                ; is on.
		mov	bx,[bp+x]			; plane = 1 << (x&3)  (or x mod 4)
		and	bx,3
		mov	ah,bl
		out	dx,ax               ; Set VGA read plane

		mov	ax,[scr_p_width]    ; Calculate physical offset
		mul	[bp+y]

		mov	bx,[bp+x]           ; divide x by 4 to get physical x offset
		shr	bx,2                ; add x to y offset
		add	bx,ax
		add	bx,[cp_offset]
		mov	al,es:[bx]          ; read pixel and return it through ax
		xor	ah,ah

		pop	bp
		ret

_x_get_pel endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_hpp
;
; Sets start scan some pel ofs to the right.
; c_ofs offsets : 1 for bit 0, 2 for bit 1  etc..
; max is 4 (bit 3) for 4 planes in a 256 colour mode
;
; Destroys : ax	Saves : bp
;		   dx
;
shpp_parms struc
		 dd      ?
		 dw      ?        		; return address & pushed BP
	c_ofs db	    ?
		 db	    ?
shpp_parms ends

_x_set_hpp proc far

		push	bp
		mov	bp,sp

		mov	dx,3c0h      		; Set hpp to p_val*2
		mov	al,13h
		out	dx,al
		mov	al,[bp+c_ofs]       ; Pixels to move right
		shl	al,1                ; *2 because we are in 256 colours
		out	dx,al               ; set hpp

		mov  al,20h       		; Clear bit 5 in Attribute index
		out  dx,al               ; this stops everything from
		inc  dx                  ; screwing up!
		in   al,dx               ; What it actually does is enable the
		dec  dx                  ; display. Clearing bit 5 will of course
		or   al,00100000b        ; disable all output to the screen
		out  dx,al

		pop	bp
		ret

_x_set_hpp endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_rgb
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

_x_set_rgb proc far

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

_x_set_rgb endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_scr_width
;
; Changes logical screen width to width
; specified in the argument.
;
; NOTE : Avoid using this when double buffering!
;
; Destroys : ax  	Saves : bp
;		   bx
;		   dx
;
sw_parms	struc
		dd		?
		dw		?              ; return address & pushed BP
   xwidth dw		?              ; new screen width to set to
sw_parms	ends

_x_scr_width proc far

		push	bp
		mov	bp,sp

		mov	dx,3d4h
		mov	al,13h
		mov	bx,[bp+xwidth]      ; get the new width
		shr	bx,2                ; divide by 4 to store in scr_p_width variable
		mov	[scr_p_width],bx    ; so it's compatible with other functions
		shr	bx,1                ; divide a further 2
		mov	ah,bl
		out	dx,ax               ; set new width

		pop	bp
		ret

_x_scr_width endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_palette
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

_x_set_palette proc

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

_x_set_palette endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_scan_start
;
; This little function will set the VGA's scan start
; register so that it is starts scanning (ofs) bytes
; to the right in the VGA refresh buffer (0a000h)
; In a mode with screen width 320 ofs would be
; between 0 and 80
;
; Destroys : ax   	Saves : bp
;		   bx
;		   dx
;
scs_parms	struc
		dd		?
		dw		?           	; return address & pushed BP
 scan_ofs	dw		?              ; new scan offset
scs_parms	ends

_x_scan_start proc

		push bp                  ; preserve callers stack frame
		mov	bp,sp

		mov  bx,[bp+scan_ofs]    ; get the new scan offset
		mov  dx,3d4h
		mov  al,0ch              ; set the higher byte
		mov  ah,bh
		out  dx,ax
		mov  al,0dh              ; set the lower byte
		mov  ah,bl
		out  dx,ax

		pop	bp
		ret

_x_scan_start endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_double_buffer
;
; Sets up double buffering (page switching)
; capabilities.
;
; Destroys : ax	Saves : bp
;		   bx
;		   dx
;
sdb_parms	struc
		dd		?
		dw		?
sdb_parms ends

_x_set_double_buffer proc

		push	bp
		mov	bp,sp

		mov	ax,[scr_width]      ; Check there is enough
		shr	ax,2                ; room for 2 pages inside
		mul	[scr_height]        ; 64K

		cmp	ax,0FFFFh/2
		jle	@s_cnf
		mov	ax,3                ; If there wasn't then exit
		int	10h                 ; back to DOS with an error.
		push ds
		mov	ah,9                ; This should only appear when
		mov	dx,offset errmsg1   ; debugging
		mov	bx,seg errmsg1
		mov  ds,bx
		int	21h
		pop	ds
		mov	ax,4c00h
		int	21h
@s_cnf:
		mov	dx,3d4h
		mov	al,13h
		mov	bx,[scr_width]
		shr	bx,1
		mov	[scr_p_width],bx    ; so it's compatible with other functions
		shr	bx,1                ; divide a further 2
		mov	ah,bl
		out	dx,ax               ; set new width

		mov	[page0],0
		mov	ax,[scr_width]
		shr  ax,2
		mov	[page1],ax

		mov	[dbuf_set],1        ; Set double buffer flag
		mov	[db_ptr],0          ; First page we display is 0
		mov	[cp_offset],0

		pop	bp
		ret

_x_set_double_buffer endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_flip_page
;
; Flips from the visible page to the
; hidden page.
;
; Destroys : ax 	Saves : bp
;            bx
;            dx
;
fp_parms	struc
		dd		?
		dw		?
fp_parms	ends

_x_flip_page proc

		push	bp
		mov	bp,sp

		cmp	[dbuf_set],1
		jne	@flp_end
		cmp	[db_ptr],0          ; get the new scan offset
		je	@flp_1
		mov	bx,[page0]
		mov	[db_ptr],0
		jmp	@flp_set
@flp_1:
		mov  bx,[page1]
		mov	[db_ptr],1
@flp_set:
		mov  dx,3d4h
		mov  al,0ch              ; set the higher byte
		mov  ah,bh
		out  dx,ax
		mov  al,0dh              ; set the lower byte
		mov  ah,bl
		out  dx,ax
@flp_end:
		pop	bp
		ret

_x_flip_page endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_active_page
;
; Sets page that all drawing is to
; done to.
;
; Destroys : 		Saves : bp
;
sp_parms	struc
		dd		?
		dw		?
   a_page db		?
		db		?
sp_parms	ends

_x_set_active_page proc

		push bp
		mov	bp,sp

		cmp	[dbuf_set],1
		jne	@sap_end
		mov	al,[bp+a_page]
		cbw
		cmp	ax,[page0]          ; get the new scan offset
		jne	@sap_1
		mov	bx,[page0]
		jmp	@sap_end
@sap_1:
		mov  bx,[page1]
@sap_end:
		mov	[cp_offset],bx

		pop	bp
		ret

_x_set_active_page endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_put_pbm
;
; Puts a planar (mode-x) bitmap to the screen on
; PAGE at x, y. Bitmap is put transparently, that is
; all 0 pixels are not drawn (ie. so bitmaps can
; overlap correctly.
;
; Destroys : ax      Saves : bp      Modifies : sp
;		   bx	         ds
;		   cx	         si
;		   dx	         di
;		   es
;
pbm_parms struc
 pix_skew	dw		?              ; Starting plane
  pln_cnt dw		?			; Arbitry counter from 4 -> 0
pbm_width dw		?			; Bitmap width
pbm_height dw		?			; Bitmap height
  pbm_pln dw		?			; Current plane in bitmap
	skip dw		?
		dd		?
		dw		?
    pbm_x dw		?
    pbm_y dw		?
   bitmap dd		?
pbm_parms ends

_x_put_pbm proc

		push	bp
		sub	sp,12		  	; Allocate space for variables
		mov	bp,sp
		push	ds si di

		lds	si,[bp+bitmap]
		mov	ax,0a000h
		mov	es,ax
		mov	ax,[bp+pbm_x]
		and	ax,3
		mov	[bp+pix_skew],ax
		mov  [bp+pln_cnt],4

		mov	al,byte ptr[ds:si]	; Get bitmap width
		cbw
		mov	[bp+pbm_width],ax
		mov	al,byte ptr[ds:si+1]; Get bitmap height
		cbw
		mov  [bp+pbm_height],ax
		mov	ax,[bp+pbm_x]
		mov  [bp+pbm_pln],0

		mov  ax,[cp_offset]
		mov  [bp+skip],ax
@pbm_pl:
		mov	dx,3c4h             ; Find write plane
		mov	ah,2
		mov	cx,[bp+pix_skew]	; plane = 1 << (x&3)  (or x mod 4)
		mov	al,1
		shl	al,cl
		xchg	ah,al
		out	dx,ax               ; Set write plane

		mov	ax,[scr_p_width]
		mul	[bp+pbm_y]
		mov	bx,[bp+pbm_x]
		shr	bx,2
		add	ax,bx
		mov	di,ax
		add	di,[bp+skip]
		mov	bx,2
		mov	ax,[bp+pbm_pln]
		mul	[bp+pbm_height]
		mul  [bp+pbm_width]
		add	bx,ax

		mov	dx,[bp+pbm_height]
@pbm_yl:
		mov	cx,[bp+pbm_width]
@pbm_xl:
		mov	al,[ds:si+bx]
		cmp	al,0
		je	@pix_nv
		mov	[es:di],al
@pix_nv:
		inc	di
		inc	bx
		dec  cx
		jnz	@pbm_xl

		sub 	di,[bp+pbm_width]
		add	di,[scr_p_width]
		dec	dx
		jnz	@pbm_yl

		inc	[bp+pix_skew]
		cmp 	[bp+pix_skew],4
		jl	@skew_ok
		mov	[bp+pix_skew],0
		inc	[bp+skip]
@skew_ok:
		inc	[bp+pbm_pln]
		dec	[bp+pln_cnt]
		jnz	@pbm_pl

		pop	di si ds
		add	sp,12				; De-allocate temp variable space
		pop	bp
		ret

_x_put_pbm endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_dword_mode
;
; Sets the VGA to dword mode so that writing
; one byte writes to all planes
;
; Destroys : ax  	Saves : bp
;		   dx
;
dwm_parms struc
		 dd       ?
		 dw		?
dwm_parms ends

_x_dword_mode proc

		push bp
		mov	bp,sp

		mov	dx,3c4h
		mov	ax,0f02h
		out	dx,ax              ; Turn on dword mode

		pop	bp
		ret

_x_dword_mode endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_byte_mode
;
; Set the VGA to byte writing mode (one byte
; write writes to selected plane.
; It does this by forcing to write plane 0
;
; Destroys : ax 	Saves : bp
;		   dx
;
bm_parms	struc
		dd		?
		dw		?
bm_parms	ends

_x_byte_mode proc

		push bp
		mov	bp,sp

		mov	dx,3c4h
		mov	ax,0102h
		out	dx,ax              ; Turn on byte mode

		pop	bp
		ret

_x_byte_mode endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_byte
;
; Sets a byte address (does not set plane).
; Mainly to be used with previous two routines
;
; Destroys : ax	Saves : bp
;		   di
;		   es
;
sb_parms	struc
		dd		?
		dw		?
	   x dw		?
	   y dw		?
	 col db 		?
		db		?
sb_parms  ends

_x_set_byte proc

		push bp
		mov	bp,sp
		push	di

		mov	ax,0a000h
		mov	es,ax
		mov	ax,[scr_p_width]
		mul	[bp+y]
		add	ax,[bp+x]
		mov	di,ax
		mov	al,[bp+col]
		mov	[es:di],al

		pop	di
		pop	bp
		ret

_x_set_byte endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_get_byte
;
; Returns a byte from address x,y (if a value was
; needed from a certain plane the plane must already
; be set.
;
; Destroys :		Saves : bp
;
gb_parms	struc
		dd		?
		dw		?
	   x	dw		?
	   y	dw		?
gb_parms	ends

_x_get_byte proc

		push bp
		mov	bp,sp
		push	di

		mov	ax,0a000h
		mov	es,ax
		mov	ax,[scr_p_width]
		mul	[bp+y]
		add	ax,[bp+x]
		mov	di,ax
		mov	al,[es:di]

		pop	di bp
		ret

_x_get_byte endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_clr_page
;
; Clears active page!
;
; Destroys : ax 	Saves : bp
;		   bx		   di
;		   dx
;		   es
;
clp_parms	struc
		dd		?
		dw		?
clp_parms ends

_x_clr_page proc

		push bp
		mov	bp,sp
		push	di

		mov	ax,0a000h
		mov	es,ax

		mov	dx,3c4h
		mov	ax,0f02h
		out	dx,ax              ; Turn on dword mode

		mov  di,[cp_offset]
		mov	dx,[scr_height]
		xor	ax,ax
@clp_y_loop:
		mov	cx,[scr_width]
		shr	cx,3
		mov	bx,cx
		shl	bx,1
		rep	stosw
		sub  di,bx
		add	di,[scr_p_width]

		dec	dx
		jnz	@clp_y_loop

		pop	di
		pop	bp
		ret

_x_clr_page endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_write_plane
;
; Sets VGA write plane.
;
; Destroys : 		Saves : bp
;
swp_parms	struc
		dd		?
		dw		?
  w_plane db		?
		db		?
swp_parms ends

_x_set_write_plane proc

		push bp
		mov	bp,sp

		mov	dx,3c4h
		mov	al,2
		mov	ah,[bp+w_plane]	; w_plane=1<<(plane&3)  (or plane mod 4)
		out	dx,ax               ; Set VGA write plane

		pop	bp
		ret

_x_set_write_plane endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_set_read_plane
;
; Sets VGA write plane
;
; Destroys :		Saves : bp
;
srp_parms struc
		dd		?
		dw		?
  r_plane db		?
		db		?
srp_parms	ends

_x_set_read_plane proc

		push bp
		mov	bp,sp

		mov	dx,03ceh            ; find and set plane pixel
		mov	al,4                ; is on.
		mov	ah,[bp+r_plane]	; w_plane = 1<<(plane&3)  (or plane mod 4)
		out	dx,ax               ; Set VGA read plane

		pop	bp
		ret

_x_set_read_plane endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_screen_off
;
; Turns screen off by clearing bit 5 of the
; attribute register, giving all cycles to
; the CPU.
;
; Destroys :
;
soff_parms struc
		dd		?
		dw		?
soff_parms ends

_x_screen_off proc

		push	bp
		mov	bp,sp

		mov	dx,3dah
		in	al,dx
		mov	dx,3c0h
		mov	al,0                ; Clear bit 5
		out	dx,al

		pop	bp
		ret

_x_screen_off endp

;北北北北北北北北北北北北北北北北北北北北北北
;
		public _x_screen_on
;
; Turns screen on, by setting bit 5 of the
; attribute register.
;
; Destroys :
;
son_parms	struc
		dd		?
		dw		?
son_parms ends

_x_screen_on proc

		push bp
		mov	bp,sp

		mov	dx,3dah
		in	al,dx
		mov	dx,3c0h
		mov	al,100000b          ; Set bit 5 of attr register
		out	dx,al

		pop	bp
		ret

_x_screen_on endp

_x_code  ends

	    end



