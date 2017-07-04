
;//				   -* TEXT.ASM *-
;//
;//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออป
;//บ											  บ
;//บ By Paul Carroll 1995                                บ
;//บ                                                     บ
;//บ        - Some handy direct screen write routines    บ
;//บ  	    for text mode;                             บ
;//บ       				     				  บ
;//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออผ

	.MODEL large
	.386
	.CODE
	PUBLIC _PutChar,_ChangeAttr,_GetChar

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

	end

