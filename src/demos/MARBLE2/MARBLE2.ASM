
          MODEL small
          STACK 100h

Code		segment para public 'CODE'

		Assume cs:Code,ds:Code

;北北北北北北北北北北北北北北北北北北
; DATA
;北北北北北北北北北北北北北北北北北北

          SCRWIDTH   equ 160         ; logical width
          MAXSCRL    equ 80
          SCRRASTERS equ 398
          NUMRASTERS equ 17

          errormsg  db 'You need a VGA to run this demo!$'
          rastbuf   db 1200 dup (0)
          bgrrast   db 1200 dup (0)
          oldKB     db 4 dup (0)
          done      db 0
          sign      db 1
          angle     dw 0

          bardat    db 0,6,12,18,26,35,45,55
                    db 63,55,45,35,26,18,12,6,0

          SIN       db    0,   2,   4,   6,   8,  11,  13,  17
                    db   17,  19,  22,  24,  26,  28,  30,  35
                    db   35,  37,  39,  41,  43,  45,  47,  51
                    db   51,  53,  55,  57,  59,  61,  63,  67
                    db   67,  69,  71,  72,  74,  76,  78,  81
                    db   81,  83,  84,  86,  88,  89,  91,  94
                    db   94,  95,  97,  98, 100, 101, 102, 105
                    db  105, 106, 107, 108, 109, 111, 112, 114
                    db  114, 115, 116, 116, 117, 118, 119, 120
                    db  120, 121, 122, 122, 123, 123, 124, 125
                    db  125, 125, 125, 126, 126, 126, 126, 126
                    db  126, 126, 127, 126, 126, 126, 126, 126
                    db  126, 126, 125, 125, 125, 124, 124, 123
                    db  123, 122, 122, 121, 120, 120, 119, 117
                    db  117, 116, 116, 115, 114, 113, 112, 109
                    db  109, 108, 107, 106, 105, 104, 102, 100
                    db  100,  98,  97,  95,  94,  92,  91,  88
                    db   88,  86,  84,  83,  81,  79,  78,  74
                    db   74,  72,  71,  69,  67,  65,  63,  59
                    db   59,  57,  55,  53,  51,  49,  47,  43
                    db   43,  41,  39,  37,  35,  32,  30,  26
                    db   26,  24,  22,  19,  17,  15,  13,   8
                    db    8,   6,   4,   2,   0,  -2,  -4,  -8
                    db   -8, -11, -13, -15, -17, -19, -22, -26
                    db  -26, -28, -30, -32, -35, -37, -39, -43
                    db  -43, -45, -47, -49, -51, -53, -55, -59
                    db  -59, -61, -63, -65, -67, -69, -71, -74
                    db  -74, -76, -78, -79, -81, -83, -84, -88
                    db  -88, -89, -91, -92, -94, -95, -97,-100
                    db -100,-101,-102,-104,-105,-106,-107,-109
                    db -109,-111,-112,-113,-114,-115,-116,-117
                    db -117,-118,-119,-120,-120,-121,-122,-123
                    db -123,-123,-124,-124,-125,-125,-125,-126
                    db -126,-126,-126,-126,-126,-126,-127,-126
                    db -126,-126,-126,-126,-126,-126,-125,-125
                    db -125,-124,-124,-123,-123,-122,-122,-120
                    db -120,-120,-119,-118,-117,-116,-116,-114
                    db -114,-113,-112,-111,-109,-108,-107,-105
                    db -105,-104,-102,-101,-100, -98, -97, -94
                    db  -94, -92, -91, -89, -88, -86, -84, -81
                    db  -81, -79, -78, -76, -74, -72, -71, -67
                    db  -67, -65, -63, -61, -59, -57, -55, -51
                    db  -51, -49, -47, -45, -43, -41, -39, -35
                    db  -35, -32, -30, -28, -26, -24, -22, -17
                    db  -17, -15, -13, -11,  -8,  -6,  -4,   2

          text      db " Well this is the second in our little"
                    db " text demo series.  Remeber if you want"
                    db " to contact me my internet address is"
                    db " CC6PJC@MFS2.BALLARAT.EDU.AU"
                    db " or perhaps some boards in the ballarat area."
                    db " Well what is this doing?  It's keeping the nice"
                    db " shaded background dispite the sinus bars.  It"
                    db " also demonstrates a scrolly in text mode.  Very"
                    db " simple stuff, just remember to credit me if you"
                    db " use any of this stuff!     "
                    db " you liked all that crap?  Well it's about to"
                    db " repeat right now :-) bye....                      "
                    db "                             ",0
          textptr   dw 0
          scrofs    dw 1
          chofs     db 0

;北北北北北北北北北北北北北北北北北北
; CODE
;北北北北北北北北北北北北北北北北北北

;屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯

; Puts sinus bars in rasterbuf

UpdateRasters:

          mov  ax,ds
          mov  es,ax
          mov  si,0
          mov  di,0
          mov  cx,3*(SCRRASTERS/2)

b0pl1:
          mov  ax,word ptr ds:bgrrast[si]
          mov  word ptr ds:rastbuf[di],ax
          add  si,2
          add  di,2
          dec  cx
          jnz  b0pl1

          add  angle,2
          cmp  angle,180
          jle  b0pl
          mov  bx,2
          mov  angle,bx
          neg  sign

b0pl:
          xor  ax,ax
          mov  cx,4           ; number of sinus bars

b1pl:
          push ax
          mov  bx,angle
          add  bx,ax
          cmp  bx,360
          jle  b2pl
          sub  bx,360

b2pl:
          mov  al,ds:SIN[bx]     ; messy crappy stuff :-)
          mul  sign
          cbw
          shl  ax,1
          add  ax,173+270

          mov  di,ax
          xor  si,si
          mov  dx,NUMRASTERS

b2pl_ml0:
          mov  al,ds:bardat[si]
          mov  ds:rastbuf[di],al
          mov  ds:rastbuf[di+1],al
          mov  ds:rastbuf[di+2],al
          add  di,3
          inc  si
          dec  dx
          jnz  b2pl_ml0

          pop  ax
          add  ax,26
          dec  cx
          jnz  b1pl

          ret

;屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯

; Updates scrolly on bottom of screen

UpdateScrolly:

          mov  ax,0b800h
          mov  es,ax
          mov  di,(328*22)+160
          mov  ax,ds:scrofs
          shl  ax,1
          add  di,ax

          mov  si,textptr
          cmp  ds:text[si],0
          jne  charok
          mov  ds:textptr,0

charok:
          mov  si,ds:textptr
          mov  al,byte ptr ds:text[si]

          mov  ah,3
          cmp  text[si-1],32
          jne  AttrWhite                  ; Is char white or WHITE?
          mov  ah,11

AttrWhite:
          mov  word ptr es:[di],ax
          mov  word ptr es:[di-162],ax

          mov  bx,ds:scrofs
          mov  dx,3d4h
          mov  al,0ch
          mov  ah,bh
          out  dx,ax
          mov  al,0dh
          mov  ah,bl
          out  dx,ax

          mov  dl,0c0h                    ; Pel panning for scrolly
          mov  al,33h
          out  dx,al
          mov  al,ds:chofs
          out  dx,al

          add  ds:chofs,3
          cmp  ds:chofs,8
          jle  ofsok

          inc  ds:textptr
          mov  ds:chofs,0
          inc  ds:scrofs
          cmp  ds:scrofs,MAXSCRL
          jle  ofsok
          mov  ds:scrofs,0

ofsok:
          ret

;屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯

KBVect :
          push ax
          in   al,60h
          cmp  al,1
          jne  NotDone
          mov  done,1

NotDone:
          mov  al,20h
          out  20h,al
          pop  ax
          iret

;屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯

Start:
          mov  ax,cs
          mov  ds,ax

          mov  ax,1a00h                 ; Do they have a VGA
          int  10h
          cmp  bl, 7
          jge  VGA_OK

          mov  dx,offset errormsg       ; No, IDIOTS!
          mov  ah,09h
          int  21h

          mov  ax,4c00h
          int  21h

VGA_OK:
          mov  ax,03h
          int  10h

          mov  ax,3509h                 ; Set up new KB handler
          int  21h
          mov  word ptr oldKB[0],bx
          mov  word ptr oldKB[2],es

          mov  dx,offset KBVect
          mov  ax,2509h
          int  21h

          mov  dx,3d4h                  ; Set double and a bit screen width
          mov  ax,5213h
          out  dx,ax
          mov  ax,800eh
          out  dx,ax

          mov  bx,5
          mov  cx,34
          mov  al,3
          mov  di,0

srl0:
          mov  ds:bgrrast[di],0
          mov  ds:bgrrast[di+1],al
          mov  ds:bgrrast[di+2],0
          add   di,3
          dec  bx
          jnz  srl0
          inc  al
          mov  bx,5
          dec  cx
          jnz  srl0

          mov  bx,5
          mov  cx,34
          inc  al

srl1:
          mov  ds:bgrrast[di],0
          mov  ds:bgrrast[di+1],al
          mov  ds:bgrrast[di+2],0
          add  di,3
          dec  bx
          jnz  srl1
          dec  al
          mov  bx,5
          dec  cx
          jnz  srl1

MainLoop:
          mov  dx,3dah
          in   al,dx
          test al,8
          jnz  $-3
          in   al,dx
          test al,8
          jz   $-3

          call UpdateRasters
          mov  cx,SCRRASTERS
          mov  si,0

prl01:
          add  si,3
          cmp  si,3*SCRRASTERS
          jl   prl02
          mov  si,0

prl02:
          mov  dx,3c8h
          mov  al,0
          out  dx,al
          inc  dx
          mov  al,rastbuf[si]
          out  dx,al
          mov  al,rastbuf[si+1] ;0
          out  dx,al
          mov  al,rastbuf[si+2]
          out  dx,al
          mov  dx,03dah
          in   al,dx
          test al,1
          jnz   short $-3
          in   al,dx
          test al,1
          jz   short $-3

          dec  cx
          jnz  prl01

          call UpdateScrolly

          cmp  done,1
          je   Finished
          jmp  MainLoop

Finished:
          mov  dx,word ptr oldKB[0]
          mov  ax,word ptr oldKB[2]
          mov  ds,ax
          mov  ax,2509h
          int  21h

          mov  ax,03h
          int  10h

		mov	ax,4c00h
          int	21h

Code		ends

          END  Start
