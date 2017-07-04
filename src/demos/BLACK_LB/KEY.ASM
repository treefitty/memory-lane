;
; Routines for handling keyboard reading. Should really
; only be used for interactive purposes (not typing etc).
;
; SET_KB : Sets a new ISR on IRQ 9 (the keyboard). The ISR
;		 grabs any key presses from the keyboard buffer
;		 before the BIOS can! This makes the keyboard
;		 reading extra fast and also makes it possible
;		 to detect whether or not more than one key is
; 		 being pressed at once.
;
; RESET_KB : Resets IRQ 9 to it's orignal ISR
;

_keydat	segment para public 'data'

extrn	_keyflags:byte
oldkb	dw 0,0

_keydat   ends

_keycode  segment para public 'code'

		PUBLIC _set_kb,_reset_kb
		ASSUME cs:_keycode,ds:_keydat

;
; This is the new ISR for IRQ 9. When a key is
; pressed it sets the array element (in KEYFLAGS)
; to 1.
;
new_int9	proc

		cli                           ; Don't interrupt us!
		push cx                       ; Save registers we are going to
		push	bx                       ; modify cause it's an ISR
		push	ax
		push ds
		mov	ax,seg _keyflags         ; Address our key flags array
		mov	ds,ax

wait_key:
		in	al,64h                   ; Check for a key
		test	al,02h
		jnz  wait_key                 ; Wait if it's not there yet!
		in	al,60h                   ; Read the key

		xor	ah,ah                    ; Isolate 1 in the key return val by
		shl	ax,1                     ; shifting everything left 1 and
		shr	al,1					; and the lower half right 1
		xor	ah,1                     ; Reverse the bit
		xor	bh,bh                    ; Get the key val in index
		mov	bl,al
		mov	_keyflags[bx],ah         ; Flag the key as pressed

		in	al,61h                   ; Clear out the buffer
		mov	ah,al
		or	al,80h
		out	61h,al
		xchg	ah,al
		out	61h,al

		mov	al,20h                   ; Tell the PIC that the ISR is finished
		out	20h,al

		pop	ds                       ; Restore the registers
		pop	ax
		pop	bx
		pop	cx
		sti                           ; We can be interrupted again
		iret

new_int9	endp

;
; Function to grab the old ISR for IRQ 9 and
; store it for later, as well as set the new
; ISR.
;
_set_kb	proc far

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

_set_kb	endp

;
; This simply resets IRQ 9's ISR to the old one
;
_reset_kb proc far

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

_reset_kb endp

_keycode  ends

		end