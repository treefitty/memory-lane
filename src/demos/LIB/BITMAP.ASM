;//***********************************************************************//
;//**                                                                   **//
;//*   	This file contains a complete library for bitmap to hidden     *//
;//    				page routines. Features :                        //
;//                                                                       //
;//                     - Straight FAST copying.                          //
;//                     - Transparent putting of bitmaps                  //
;//				    - color 0(BLACK) masked.                          //
;//                                                                       //
;//				All functions callable from Borland C++.              //
;//			Function prototypes at beginning of each function.         //
;//                                                                       //
;//					Written by Paul Carroll 1994                     //
;//*					  Copyright iKOn graphics.                      *//
;//**                                                                   **//
;//***********************************************************************//

	.MODEL large                  ;// Large memory model.
	.CODE
	.386
	PUBLIC _tc_putbitmap, _getbitmap, _copybitmap

;//***********************************************************************//
;//                                                                       //
;//    Transparently puts a rectangular bitmap to offscreen DEST from     //
;//					  offscreen buffer SRC.                          //
;//                                                                       //
;//  C callable as :-                                                     //
;//                                                                       //
;//  void(BYTE far *Dest,BYTE far *Src,int width,int height,int x,int y); //
;//                                                                       //
;//			where BYTE has type definition unsigned char.              //
;//                                                                       //
;//***********************************************************************//

_tc_putbitmap PROC C Dest_off:WORD,Dest_Seg:WORD,Src_off:WORD,Src_Seg,w:WORD,h:WORD,x:WORD,y:WORD

	push 	ds

	cld                         ;// Set direction flag to forward.
	mov ax,	Src_Seg
	mov ds,	ax                ;// Get Source segment into DS.
	mov si,   Src_off           ;// Get Source offset into SI.
	mov ax,	Dest_Seg
	mov es,	ax                ;// Get Dest segment into ES.
	mov di,	Dest_off          ;// Get Dest offset into DI.

	mov ax,   y                 ;// Calc dest offset
	mov bx,	320
	imul 	bx
	add ax,   x
	add di,   ax

	mov cx,   w                 ;// width in CX.
	mov dx,	h                 ;// height in DX.

DrawLoop:

	push		di
	push 	cx

LineLoop:

	mov al,	[si]
	or  al,	al                ;// OR the contents of AL
	jnz   	Store             ;// Check for color.

NoPaint:

	inc  	si                ;// No color so
	inc  	di                ;// don't do anything excpet increment to
	dec 		cx
	jg 		LineLoop
	jmp  	NextLine          ;// CX=0 so must be end of line.

Store:

	movsb                       ;// Store the pixel read before.
	dec 		cx
	jg 		LineLoop

NextLine:

	pop		cx
	pop 		di
	dec		dx
	jz  		Exit              ;// Done all lines? then exit.
	add di,	320        	   ;// Not done, then goto next line.
	jmp 		DrawLoop          ;// Start the next line.

Exit:

	pop		ds
	ret

_tc_putbitmap ENDP

;//*******************************************************************//
;//**'                                                              `*//
;//*'         Retrives a rectangular section from the screen         `//
;//* 			       and stores it in DEST.                     //
;//*                                                                  //
;//* C callable as :-                                                 //
;//*                                                                  //
;//* void getbitmap(BYTE far *Dest,int width,int height,int x,int y); //
;//*                                                                  //
;//*.	where BYTE has type definition of  -:* unsigned char *:-.  .//
;//**.                                                              .*//
;//*******************************************************************//

_getbitmap PROC C Dest_off:WORD,Dest_seg:WORD,Src_off:WORD,Src_seg:WORD,w:WORD,h:WORD,x:WORD,y:WORD

	push	si di ds

	cld                  ;// Set direction flag to forward.
	mov ax,	Dest_seg
	mov es,	ax         ;// Get Destination segment in ES.
	mov di,	Dest_off   ;// Get Destination offset in DI.
	mov ax,	Src_seg    ;// Point DS to start of source.
	mov ds,	ax
	mov si,	Src_off

	mov ax,	y          ;// Calc source offset
	mov bx,	320
	imul		bx
	add ax,	x
	add si,	ax
	mov dx,	h
	mov cx,	w

iloop:

	push		si
	push		cx
	rep		movsb      ;// Move entire line from screen to buffer.
	pop		cx
	pop		si
	dec 		dx
	jz		iExit
	add si,	320        ;// Move to next line.
	jmp		iloop      ;// If there are still lines to capture then do
					 ;// them.
iExit:

	pop	ds di si
	ret

_getbitmap ENDP

;//***********************************************************************//
;//**                                                                   **//
;//*  		Puts a rectangular bitmap to the screen without           *//
;//			    masking color 0(BLACK). This function is               //
;//			   useful for restoring a sprites background.              //
;//                                                                       //
;//	C callable as :-                                                     //
;//                                                                       //
;//      void copybitmap(BYTE far *Dest,BYTE far *Src,int width,          //
;//				int height,int x,int y); //                           //
;//                                                                       //
;//*			where BYTE has type definition unsigned char.             *//
;//**                                                                   **//
;//***********************************************************************//

_copybitmap PROC C Dest_off:WORD,Dest_seg:WORD,Src_off:WORD,Src_seg:WORD,w:WORD,h:WORD,x:WORD,y:WORD

	push ds

	cld
	mov ax,	Src_Seg
	mov ds,	ax                ;// Get Source segment into DS.
	mov si,   Src_off           ;// Get Source offset into SI.
	mov ax,	Dest_Seg
	mov es,	ax                ;// Get Dest segment into ES.
	mov di,	Dest_off          ;// Get Dest offset into DI.

	mov ax,   y                 ;// Y goes into AX for offset calculations.
	mov bx,	320               ;// Screenwidth into BX
	imul 	bx           	   ;// BX * AX  OR  y * 320
	add ax,   x                 ;// result (y*320)+x
	add di,   ax                ;// Move DI's position to start position
						   ;// of bitmap.
	mov cx,   w                 ;// Get width in CX.
	mov dx,	h                 ;// Get height in DX.

bLoop:

	push		di
	mov cx,	w
	rep		movsb
	pop		di
	dec 		dx
	jz		bExit
	add di,	320
	jmp		bLoop

bExit:

	pop  ds
	ret

_copybitmap ENDP

	END  ;// **********  END OF FILE  ************//
