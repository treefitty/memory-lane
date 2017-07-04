
		.model LARGE

		.DATA

VGA_SEGMENT	equ	0a000h	;display memory segment
SC_INDEX		equ	3c4h	;Sequence Controller Index register
GC_INDEX		equ	3ceh	;Graphics Controller Index register
MAP_MASK		equ	2	;Map Mask register index in SC
READ_MAP		equ	4	;Read Map register index in GC
SCREEN_WIDTH	equ	360	;# of pixels across screen
WORD_OUTS_OK	equ	0	;set to 0 to assemble for

vptbl   dw      06b00h  ; horz total
	   dw      05901h  ; horz displayed
	   dw      05a02h  ; start horz blanking
	   dw      08e03h  ; end horz blanking
	   dw      05e04h  ; start h sync
	   dw      08a05h  ; end h sync
	   dw      00d06h  ; vertical total
	   dw      03e07h  ; overflow
	   dw      04009h  ; cell height
	   dw      0ea10h  ; v sync start
	   dw      0ac11h  ; v sync end and protect cr0-cr7
	   dw      0df12h  ; vertical displayed
	   dw      02d13h  ; offset
	   dw      00014h  ; turn off dword mode
	   dw      0e715h  ; v blank start
	   dw      00616h  ; v blank end
	   dw      0e317h  ; turn on byte mode
vpend   label   word

	.CODE

	public _Set360x480Mode

_Set360x480Mode proc C

	push	si		;preserve C register vars
	push	di
	mov     ax,12h          ; start with mode 12h
	int     10h             ; let the bios clear the video memory

	mov     ax,13h          ; start with standard mode 13h
	int     10h             ; let the bios set the mode

	mov     dx,3c4h         ; alter sequencer registers
	mov     ax,0604h        ; disable chain 4
	out     dx,ax

	mov     ax,0100h        ; synchronous reset
	out     dx,ax           ; asserted
	mov     dx,3c2h         ; misc output
	mov     al,0e7h         ; use 28 mHz dot clock
	out     dx,al           ; select it
	mov     dx,3c4h         ; sequencer again
	mov     ax,0300h        ; restart sequencer
	out     dx,ax           ; running again

	mov     dx,3d4h         ; alter crtc registers

	mov     al,11h          ; cr11
	out     dx,al           ; current value
	inc     dx              ; point to data
	in      al,dx           ; get cr11 value
	and     al,7fh          ; remove cr0 -> cr7
	out     dx,al           ;    write protect
	dec     dx              ; point to index
	cld
	mov     si,offset vptbl
	mov     cx,((offset vpend)-(offset vptbl)) shr 1

@b:
	lodsw
	out     dx,ax
	loop    @b
	pop	di			    ;restore C register vars
	pop	si
	ret

_Set360x480Mode	endp

	public _Draw360x480Dot

_Draw360x480Dot proc C DrawX:WORD,DrawY:WORD,Color:BYTE

	push	si				;preserve C register vars
	push	di
	mov	ax,VGA_SEGMENT
	mov	es,ax			;point to display memory
	mov	ax,SCREEN_WIDTH/4
						;there are 4 pixels at each address, so
						; each 360-pixel row is 90 bytes wide
						; in each plane
	mul	[DrawY] 			;point to start of desired row
	mov	di,[DrawX] 		;get the X coordinate
	shr	di,1				;there are 4 pixels at each address
	shr	di,1				; so divide the X coordinate by 4
	add	di,ax			;point to the pixel's address
	mov	cl,byte ptr [DrawX] ;get the X coordinate again
	and	cl,3				;get the plane # of the pixel
	mov	ah,1
	shl	ah,cl			;set the bit corresponding to the plane
						; the pixel is in
	mov	al,MAP_MASK
	mov	dx,SC_INDEX
	out  dx,ax

	mov	al,byte ptr [Color]	;get the color
	stosb				;draw the pixel
	pop	di				;restore C register vars
	pop	si
	ret

_Draw360x480Dot	endp

	end
