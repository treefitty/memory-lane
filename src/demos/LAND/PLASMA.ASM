
			 .model  large,pascal
			 .286

			 PUBLIC  _DrawPlasma

;===================== Plasma equates ========================================

MAXWIDTH        equ     320                     ; virtual screen dimensions.
MAXHEIGHT       equ     200
MAXCOLOR        equ     255                     ; max colors for plasma.
SHIFTVALUE      equ     2                       ; granularity shift.

;====================== Plasma data ==========================================

			 .data

RandSeed        dw      ?                       ; random generator seed.

;====================== Plasma routines ======================================

			 .code

Random          proc near

			 mov     ax,[RandSeed]
			 mov     dx,8405h
			 mul     dx
			 inc     ax
			 mov     [RandSeed],ax
			 ret

Random          endp


;===================================================================


GetPixel        macro   X,Y

			 mov     bx,Y
			 imul    bx,MAXWIDTH
			 add     bx,X
			 mov     al,es:[bx]

			 endm


;====================================================================


PutPixel        macro   X,Y

			 mov     bx,Y
			 imul    bx,MAXWIDTH
			 add     bx,X
			 mov     es:[bx],al

			 endm



;======================================================================


Adjust          proc near XA:word,YA:word,X:word,Y:word,XB:word,YB:word

			 mov     si,[X]                  ; if already painted
			 mov     di,[Y]                  ; the pixel, use this
			 xor     ax,ax                   ; one instead of new
			 GetPixel si,di                  ; calculation.
			 test    ax,ax
			 jne     AdjExit

			 call    Random                  ; get a random variation
			 mov     bx,[XB]                 ; dependent of the pixels
			 sub     bx,[XA]                 ; distance and granularity
			 add     bx,[YB]                 ; shift factor.
			 sub     bx,[YA]
			 shl     bx,SHIFTVALUE
			 imul    bx
			 xor     ax,ax                   ; adds the average of the
			 GetPixel [XA],[YA]              ; near pixels colors.
			 add     dx,ax
			 GetPixel [XB],[YB]
			 add     ax,dx
			 shr     ax,1
			 cmp     ax,1                    ; check if new color is
			 jge     ColorUp                 ; in the right range.
			 mov     ax,1
ColorUp:        cmp     ax,MAXCOLOR
			 jle     ColorDn
			 mov     ax,MAXCOLOR
ColorDn:        PutPixel si,di                  ; paint pixel color.
AdjExit:        ret

Adjust          endp


;========================================================================


SubDivide       proc near X1:word,Y1:word,X2:word,Y2:word
			 local   X:word,Y:word

			 mov     ax,[X2]                 ; test if this region
			 sub     ax,[X1]                 ; needs a sub-division.
			 cmp     ax,2
			 jge     SubDivCont
			 mov     ax,[Y2]
			 sub     ax,[Y1]
			 cmp     ax,2
			 jge     SubDivCont
			 jmp     SubDivExit

SubDivCont:     mov     ax,[X1]                 ; get the center position
			 add     ax,[X2]                 ; of the region.
			 rcr     ax,1
			 mov     [X],ax
			 mov     ax,[Y1]
			 add     ax,[Y2]
			 rcr     ax,1
			 mov     [Y],ax

			 ; get the sum of the neighboring four pixel colors.
			 xor     cx,cx
			 call    Adjust, [X1],[Y1], [X],[Y1], [X2],[Y1]
			 add     cx,ax
			 call    Adjust, [X2],[Y1], [X2],[Y], [X2],[Y2]
			 add     cx,ax
			 call    Adjust, [X1],[Y2], [X],[Y2], [X2],[Y2]
			 add     cx,ax
			 call    Adjust, [X1],[Y1], [X1],[Y], [X1],[Y2]
			 add     cx,ax

			 mov     si,[X]                  ; test if the center pixel
			 mov     di,[Y]                  ; need to be calculated.
			 GetPixel si,di
			 test    al,al
			 jne     SubDivNow
			 mov     ax,cx                   ; yes, use the average of
			 add     ax,2                    ; the neighboring pixels.
			 shr     ax,2                    ; (don't allow color 0)
			 PutPixel si,di

			 ; sub-divides the new four regions.
SubDivNow:      call    SubDivide, [X1],[Y1], [X],[Y]
			 call    SubDivide, [X],[Y1], [X2],[Y]
			 call    SubDivide, [X],[Y], [X2],[Y2]
			 call    SubDivide, [X1],[Y], [X],[Y2]

SubDivExit:     ret

SubDivide       endp


;=========================================================================


_DrawPlasma   proc C

			 mov     ax,0a000h ;[ScreenSeg]          ; setup virtual screen
			 mov     es,ax                   ; segment,

			 mov     ax,8945h   ;[Seed]               ; and random calculation
			 mov     [RandSeed],ax           ; seed.

			 call    Random                  ; set window corner pixels.
			 or      al,1
			 PutPixel 0,0    ;[XStart],[YStart]

			 call    Random
			 or      al,1
			 PutPixel 319,0    ;[XStop],[YStart]

			 call    Random
			 or      al,1
			 PutPixel 0,199    ;[XStart],[YStop]

			 call    Random
			 or      al,1
			 PutPixel 319,199       ;[XStop],[YStop]

			 call    SubDivide,  0,0,319,199     ;[XStart],[YStart], [XStop],[YStop]
			 ret

_DrawPlasma      endp

			 end