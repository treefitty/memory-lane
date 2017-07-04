;
;    ---* Some basic text routines used in my 3D demo. *---
;
;	            Subject to Copyright (c) 1995
;		  Paul Carroll - Black Marble Productions
;

	.MODEL large
	.386
	.CODE
	PUBLIC _PutChar,_ChangeAttr,_GetChar,_putTextScreen

;// Puts a single char with attrib to x,y point on screen

_PutChar PROC C x:WORD,y:WORD,c:BYTE,attr:BYTE

	push di                 ; Save C register variables

	mov 	ax,y
	mov 	bx,80
	mul 	bx
	add 	ax,x
	mov 	bx,2
	mul 	bx
	mov 	di,ax
	mov 	ax,0b800h
	mov 	es,ax
	mov 	al,c
	mov 	ah,attr
	mov 	es:[di],al
	inc 	di
	mov 	es:[di],ah

	pop	di

	ret

_PutChar ENDP


;// Changes the attrib of a char at a given x,y point

_ChangeAttr PROC C x:WORD,y:WORD,attr:BYTE

	push di                 ; Save C register variables

	mov 	ax,y
	mov 	bx,80
	mul 	bx
	add 	ax,x
	mov 	bx,2
	mul 	bx
	mov 	di,ax
	add 	di,1
	mov 	ax,0b800h
	mov 	es,ax
	mov 	al,attr
	mov 	es:[di],al

	pop	di

	ret

_ChangeAttr ENDP


;// Gets ASCII num and attrib of char at x,y
;//
;// Returns : AL - ASCII num
;// 		    AH - Attrib

_GetChar PROC C x:WORD,y:WORD

	push di                 ; Save C register variables

	mov 	ax,y
	mov 	bx,80
	mul 	bx
	add 	ax,x
	mov 	bx,2
	mul 	bx
	mov 	di,ax
	mov 	ax,0b800h
	mov 	es,ax
	mov 	al,es:[di]
	inc 	di
	mov 	ah,es:[di]

	pop	di

	ret

_GetChar ENDP

_putTextScreen PROC C text:DWORD, ofs:WORD

	push ds si di

	lds	si,text
	mov	ax,0b800h
	mov	es,ax
	mov	ax,ofs
	mov	bx,80
	mul  bx
	shl	ax,1
	mov	di,ax
	mov	cx,2000
	rep	movsw

	pop  di si ds
	ret

_putTextScreen ENDP

	end

