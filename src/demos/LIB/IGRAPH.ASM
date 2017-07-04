
;/*************************************************************************/
;/***                                                                   ***/
;/**             This file contains a library of low level               **/
;/*                    graphics functions, including:                     */
;/*                                                                       */
;/*                        - Putpixel routine                             */
;/*                        - Gaphics initialization                       */
;/*                        - Textmode resetting                           */
;/*                        - Hidden page to video memory                  */
;/*                          copying.                                     */
;/*                                                                       */
;/*                All functions callable from BORLAND C++ 3.1            */
;/*                                                                       */
;/*                    Written by Paul Carroll (c) 1994                   */
;/**                       Copyright iKOn graphics.                      **/
;/***                                                                   ***/
;/*************************************************************************/

	.MODEL large
	.386
	.CODE
	PUBLIC _putpixel, _setgraphics, _settextmode, _putbuffer,_setpalette,_SetColor

_putpixel PROC C xpos:WORD,ypos:WORD, colour:BYTE

	push		es
	mov ax,	ypos
	mov bx,	320
	mul bx
	add bx,	xpos
	mov ax,	0a000h
	mov es,	ax         ;// Move video pointer into EXTRA segment.
	mov al,	colour     ;// Colour of pixel to print.
	mov di,	bx         ;// Position of pixel : ( Y * SCREENWIDTH )+X
	mov es:[di],al       ;// Point to pixel location and move in colour.
	pop		es		 ;// Restore ES and AX.
	ret

_putpixel ENDP

_setgraphics PROC

	mov al,	13h       ;// Graphics mode 13h (320x200x256).
	xor ah,	ah        ;// Set ah to 0.
	int 		10h       ;// Call video.
	ret

_setgraphics ENDP

_settextmode PROC

	mov al,	3         ;// Back into text mode 3 : (80x25)Colour.
	xor ah,	ah
	int		10h
	ret

_settextmode ENDP

_putbuffer PROC C screenbuf:DWORD

	push ds
	cld                     ;// Clear direction flag to forward.
	xor di,	di            ;// Null di pointer index.
	lds si,	screenbuf     ;// Load data segment.
	mov ax,	0a000h
	mov es,	ax            ;// Load video segment.
	mov cx,	3e80h         ;// Store double words (32-bit)
	rep movsd               ;// Loop till done

	pop ds
	ret

_putbuffer ENDP

_setpalette PROC C Array:DWORD

	push 	si
	mov  dx,	3c8h         	;// DAC index register
	mov  al,	0            	;// start with reg 1
	out  dx,	al
	mov  cx,	768
	push    	ds
	lds 	si, 	Array 		;// address segment
	cld                     	;// set data transfer flag forward

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

	pop ds
	pop si
	ret

_setpalette ENDP

_SetColor PROC C Col:BYTE,r:BYTE,g:BYTE,b:BYTE

	mov	dx,3c8h
	mov	al,Col
	out	dx,al
	inc	dx
	mov	al,r
	out	dx,al
	mov	al,g
	out	dx,al
	mov	al,b
	out	dx,al

	ret

_SetColor ENDP

	END

