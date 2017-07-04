
          .286
          MODEL small
          STACK 100h

Code		segment para public 'CODE'

		Assume cs:Code,ds:Code

;ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ
; DATA
;ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ

          include descript.inc
          include title.inc

          SCRWIDTH   equ 164         ; logical width
          MAXSCRL    equ 80
          SCRRASTERS equ 398
          NUMRASTERS equ 17

          SplitLine dw 240
          errormsg  db 'You need a VGA to run this demo!$'
          rastbuf   db 1200 dup (0)
          bgrrast   db 1200 dup (0)
          oldKB     db 4 dup (0)
          exitd     db 0
          scrofs    dw 0
          lwrslide  db 1
          titledrop db 0
          lwroff    db 0
          titleoff  db 0
          angle     dw 90
          barangle  dw 0
          tempofs   dw ?
          sign      db 1

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

          endmsg    db 'Code & Design - Stark (a.k.a Paul Carroll)',13,10
                    db 'This is a Black Marble production of 1995',13,10,13,10
                    db 'BTW this is ALL in text mode!',13,10,13,10,'$'

;ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ
; CODE
;ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ

;ออออออออออออออออออออออออออออออออออออ

SetSplitScreen:

          mov  dx,3d4h                  ; Set the split screen
          mov  ah,byte ptr [SplitLine]
          mov  al,18h
          out  dx,ax
          mov  ah,byte ptr [SplitLine+1]
          and  ah,1
          shl  ah,4
          mov  al,7
          out  dx,al
          inc  dx
          in   al,dx
          and  al,not 10h
          or   al,ah
          out  dx,al
          dec  dx
          mov  ah,byte ptr [SplitLine+1]
          and  ah,2
          mov  cl,3
          ror  ah,cl
          mov  al,9
          out  dx,al
          inc  dx
          in   al,dx
          and  al,not 40h
          or   al,ah
          out  dx,al

          ret

;ออออออออออออออออออออออออออออออออออออ

setoffset proc

          cli
          mov  dx,3dah

v1:
          in   al,dx
          and  al,8
          jnz   v1

          mov  bx,ds:scrofs
          mov  dx,3d4h
          mov  al,0ch
          mov  ah,bh
          out  dx,ax
          mov  al,0dh
          mov  ah,bl
          out  dx,ax
          sti

          ret

setoffset endp

;ออออออออออออออออออออออออออออออออออออ

putdesc   proc

          mov  ax,0b800h                ; Put the lower box to screen
          mov  es,ax
          mov  si,0
          mov  cx,66
          mov  dx,10
pdesl1:
          mov  al,ds:DESCRIPT[si]
          mov  ah,ds:DESCRIPT[si+1]
          mov  es:[di],ax
          add  si,2
          add  di,2
          dec  cx
          jnz  pdesl1
          add  di,(2*(SCRWIDTH-66))
          mov  cx,66
          dec  dx
          jnz  pdesl1

putdesc   endp

;ออออออออออออออออออออออออออออออออออออ

srvrast   proc

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

          add  barangle,2
          cmp  barangle,180
          jle  b0pl
          mov  bx,2
          mov  barangle,bx
          neg  sign

b0pl:
          xor  ax,ax
          mov  cx,4           ; number of sinus bars

b1pl:
          push ax
          mov  bx,barangle
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

          mov  cx,SCRRASTERS-48       ; Update background
          mov  si,offset rastbuf

          mov  dx,3dah
          in   al,dx
          and  al,8
          jnz  short $-3
          mov  dx,3dah
          in   al,dx
          and  al,8
          jz   short $-3

prl01:
          mov  dx,3c8h
          mov  al,0
          out  dx,al
          inc  dx
          outsb
          outsb
          outsb

          mov  dx,03dah
          in   al,dx
          and  al,1
          jnz  short $-3

          in   al,dx
          and  al,1
          jz   short $-3

          dec  cx
          jnz  prl01

          ret

srvrast endp

;ออออออออออออออออออออออออออออออออออออ

KBVect :
          push ax
          in   al,60h
          cmp  al,1
          jne  NotDone

          cmp  lwrslide,0
          jne  NotDone

          mov  titleoff,1
          mov  titledrop,0

NotDone:
          mov  al,20h
          out  20h,al
          pop  ax
          iret

;ออออออออออออออออออออออออออออออออออออ

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
          mov  dx,2

srl0:
          mov  ds:bgrrast[di],0
          mov  ds:bgrrast[di+1],al
          mov  ds:bgrrast[di+2],0
          add   di,3
          dec  bx
          jnz  srl0
          add   al,sign
          mov  bx,5
          dec  cx
          jnz  srl0

          mov  bx,5
          mov  cx,34
          inc  al
          neg  sign
          dec  dx
          jnz  srl0

          mov  dx,3dah
          in   al,dx
          test al,8
          jnz  $-3

          mov  dx,3dah
          in   al,dx
          test al,8
          jz   $-3

          mov  di,(2*(15*SCRWIDTH))+160+14        ; +14 to center it!
          call putdesc

;ออออออออออออออออออออออออออออออออออออ
; This is where the main code starts

mainl0:
          call srvrast

          cmp  lwrslide,1
          jne  noslide

          inc  scrofs
          cmp  scrofs,80
          jl   scrok
          mov  lwrslide,0
          call SetSplitScreen
          mov  scrofs,(SCRWIDTH*24)
          call setoffset
          mov  di,14
          call putdesc

          mov  di,2*(SCRWIDTH*15)+4
          mov  ax,0b800h   
          mov  es,ax
          mov  si,0
          mov  cx,76
          mov  dx,9

pdesl0:
          mov  al,ds:TITLEX[si]
          mov  ah,ds:TITLEX[si+1]
          mov  es:[di],ax
          add  si,2
          add  di,2
          dec  cx
          jnz  pdesl0

          add  di,2*(SCRWIDTH-76)
          mov  cx,76
          dec  dx
          jnz  pdesl0

          mov  titledrop,1
          jmp  noslide

scrok:
          inc  scrofs
          call setoffset

noslide:
          cmp  titledrop,1
          jne  nodrop
          inc  angle
          cmp  angle,180
          jl   angleok
          mov  angle,0

angleok:
          mov  si,angle
          mov  al,ds:SIN[si]
          cbw
          mov  bx,155
          add  bx,ax
          mov  cx,bx
          mov  tempofs,bx
          and  bx,00001111b
          mov 	dx,03d4h
		mov 	al,08h
          mov  ah,bl
          out 	dx,ax

          shr  cx,4
          mov  bx,SCRWIDTH
          mov  ax,cx
          mul  bx
          mov  scrofs,ax
          call setoffset

nodrop:
          cmp  titleoff,1
          jne  titleok
          add  tempofs,2
          cmp  tempofs,25*16
          jle  movetitle
          mov  titleoff,0
          mov  lwroff,1

          mov  ax,0b800h
          mov  es,ax
          mov  di,0
          mov  cx,2*(SCRWIDTH*20)
          mov  ax,0
          rep  stosw

          mov  SplitLine,400
          call SetSplitScreen
          mov  di,2*(SCRWIDTH*15)+14
          call putdesc
          mov  scrofs,0

          jmp  titleok

movetitle:
          mov  cx,tempofs
          mov  bx,cx
          and  bx,00001111b
          mov  dx,03d4h
          mov  al,08h
          mov  ah,bl
          out  dx,ax

          shr  cx,4
          mov  bx,SCRWIDTH
          mov  ax,cx
          mul  bx
          mov  scrofs,ax
          call setoffset

titleok:
          cmp  lwroff,1
          jne  lwrok

          inc  scrofs
          cmp  scrofs,80
          jle  scrok2
          mov  exitd,1

scrok2:
          call setoffset

lwrok:
          cmp  exitd,1
          je   finished
          jmp  mainl0

;ออออออออออออออออออออออออออออออออออออ
; Finish up now...

finished:
          mov  dx,word ptr oldKB[0]
          mov  ax,word ptr oldKB[2]
          mov  ds,ax
          mov  ax,2509h
          int  21h

          mov  ax,03h
          int  10h

          mov  ax,cs
          mov  ds,ax
          mov  dx,offset endmsg
          mov  ah,09h
          int  21h

		mov	ax,4c00h
          int	21h

Code		ends

          END  Start


