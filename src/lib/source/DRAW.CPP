
;//--------------------------------------------------------------------------
;//                                                                         |
;//  DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW  |
;//                                                                         |
;//                                                                         |
;// ********                                                                |
;// ******   DRAW graphics library provides -                *****          |
;// ***                                                      **  **         |
;// *****				- Low level assembler drawing       **   **        |
;// *                      functions independant of		 **   **	      |
;// ****                   Borland's Graphics Interface. 	 **	 **	      |
;//				                                       	 **  **	      |
;//                                                          *****          |
;//		             Copyright Paul Carroll (c) - 1994.                   |
;//                                                                         |
;//                                                                         |
;//  DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW-DRAW  |
;//                                                                         |
;//=========================================================================|

		.MODEL	large
		.CODE
		.386
		PUBLIC	_readpixel,_pixel,_zline,_poly,_rect,_fillrect,
		_graph,_quit_pgm;

RETCOL	dw	?
HALFX	dw	?
DELTAX	dw 	?
DELTAY	dw 	?
ERRORTERM dw	?
TEMPX	dw 	?
TEMPY	dw	?

_readpixel	PROC

			ARG 	x:WORD, y:WORD

			push 	ax           ; Save contents of registers.
			push 	cx
			push 	dx
			mov 		ah,0x0D      ; Read pixel function.
			mov 		bh,0         ; Set page number.
			mov 		cx,x         ; X pos.
			mov 		dx,y         ; Y pos.
			int		0x10         ; Call BIOS.
			mov 		retcol,ax    ; Get return color.
			pop 		dx           ; Restore registers.
			pop 		cx
			pop 		ax
			ret 		retcol       ; Return the number.

_readpixel	ENDP

_pixel		PROC

			ARG	xpos:WORD,ypos:WORD,color:WORD

			push 	ax         ; Save contents of registers by
			push 	bx         ; PUSHing them onto the stack.
			push 	cx
			push 	dx
			mov 		ax,color   ; Move function call into AH register.
			mov 		ah,0x0c    ; Move color into AL register.
			mov 		bh,0       ; Page number to draw on.
			mov 		cx,xpos    ; Y Position of pixel.
			mov 		dx,ypos    ; X Position of pixel
			int 		0x10       ; Call BIOS interupt 10h.
			pop 		dx         ; Restore the contents of the resisters.
			pop 		cx
			pop 		bx
			pop 		ax
			ret

_pixel		ENDP

//	***                                                        ***
//	****** Draws line from X1, Y1 to X2, Y2 and attrib COL. ******

_zline		PROC

			ARG x1:WORD,y1:WORD,x2:WORD,y2:WORD,col:WORD

			push 	ax             ; Save registers.
			push 	cx
			push 	dx
			mov 		cx,x2
			sub 		cx,x1       	; Subtract X1 from X2 to find
			mov 		deltax,cx   	; difference - deltax.
			mov 		dx,y2
			sub 		dx,y1       	; Subtract Y1 from Y2 to find
			mov 		deltay,dx   	; difference - deltay.
			mov 		ax,deltax
			shr 		ax,1        	; Shift bits right to divide by 2.
			mov 		halfx,ax    	; Store deltax div 2 in halfx.
			mov 		cx,x1       	; Init values in CX.
			mov 		dx,y1       	; Init values in DX.
			mov 		errorterm,cx	; Set errorterm equal to X1.
NEW_PIXEL:
			mov 		ax,col      	; Move color into AX reg.
			mov 		ah,0x0c     	; Write pixel function.
			int 		0x10     		; Call BIOS.
			inc		cx       		; Increment X value.
			mov 		ax,deltay
			add 		errorterm,ax   ; Add deltay to errorterm.
			mov 		ax,halfx
			cmp 		errorterm,ax   ; If errorterm is less than or equal
			jle		LEAP     		; to halfx then jump.
			mov 		ax,deltax
			sub 		errorterm,ax   ; Subtract deltax from errorterm.
			inc		dx       		; Increment Y value.
LEAP:
			cmp cx,	x2         	; If all pixels haven't been plotted
			jle		NEW_PIXEL  	; jump back and plot another one.
END:
			pop dx               	; Restore registers.
			pop cx
			pop ax
			ret

_zline		ENDP

//	***                                                           ***
//	****** Draws polygon from X1, Y1 to X2, Y2 and attrib COL. ******

_poly		PROC

			ARG x1:WORD,y1:WORD,x2:WORD,y2:WORD,col:WORD

			zline(x1,y1,x2,y1,col);
	zline(x1,y1,x1,y2,col);
	zline(x1,y2,x2,y2,col);
	zline(x2,y1,x2,y2,col);
}

//	***                                                         ***
//	****** Draws single line rectangle from X1, Y1 to X2, Y2 ******
//	******		and attrib COL, mode XOR or COPY	       ******
//	***												***

void draw::rect(int x1,int y1,int x2,int y2,int col)
{
	asm {
		push 	ax;
		push		dx;
		mov cx,	x1;			// Store first X coordinate.
		mov dx,	y1;            // Store first Y coordinate.
		mov ax,	col;           // Move color into AX reg.
		jmp 		NEXT_X_DOT;
	}

	NEXT_X_DOT:

	asm {
		mov ah,	12;            // Print pixel(dot) function.
		int 		0x10;          // Call BIOS to draw one pixel.
		inc cx;                  // Increment CX reg.
		cmp cx,	x2;            // Compare contents of CX to X2.
		jle  	NEXT_X_DOT;    // Jump if X2 is less than contents.
		cmp dx,	y2;            // Have both Horizontal lines been drawn?
		je		HORIZ_LINE;    // If so start on Vertical lines.

		mov dx,	y2;            // If not set DX to second Y coord.
		mov cx,	x1;            // Reset the CX register to first X coord.
		jmp 		NEXT_X_DOT;    // Jump back and draw second line.
	}

	HORIZ_LINE:

	asm { mov cx,	x1;
		 mov dx,	y1; }

	NEXT_Y_DOT:

	asm {
		// Function and color are already in AX register.
		int 0x10;
		inc dx;
		cmp dx,	y2;
		jle		NEXT_Y_DOT;

		cmp cx,	x2;
		je		END;

		mov cx,	x2;
		mov dx,	y1;
		jmp 		NEXT_Y_DOT;
	}
	END:
	asm {
		pop dx;
		pop ax;
	}
}

//	***                                                         ***
//	****** Draws filled rectangle from X1, Y1 to X2, Y2 with ******
//	******	        draw and fill colors COL.              ******
//   ***												***

void draw::fillrect(int x1,int y1,int x2,int y2, int col)
{
	int tempx, tempy;

	asm {
		push 	ax;
		push 	cx;
		push 	dx;
		mov ax,	col;
		mov ah,	12;
		mov cx,	x1;
		mov dx,	y1;
		mov tempx,cx;
		mov tempy,dx;
	}
	NEXT_X_DOT:
	asm {
		int 		0x10;
		inc       cx;
		cmp cx,	x2;
		jle		NEXT_X_DOT;
		cmp dx,	y2;
		je		END;
		inc 		dx;
		mov cx,	tempx;
		jmp 		NEXT_X_DOT;
	}
	END:
	asm {
		pop		dx;
		pop		cx;
		pop		ax;
	}
}

//	***                                                      ***
//	****** Initialises graphics mode in 320x200x256 mode. ******
//	***											  ***

void draw::graph(void)
{
	asm {
		mov ax,	0x13;
		int 		0x10;
	}
}

//	***                                                                   ***
//	****** Closes graphics mode by putting initialising VGA text mode. ******
// 	***														***

void draw::quit_pgm(void)
{
	asm {
		mov ax,	0x02;
		int		0x10;
	}
}