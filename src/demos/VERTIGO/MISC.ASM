

			model	LARGE
			.286
			PUBLIC  	_SetLandPal
			.CODE


_SetLandPal   proc C Palette:DWORD


			 ;mov     ax,0013h                ; set VGA 320x200x256 mode.
			 ;int     10h

			 ;mov     ax,[MemSeg]             ; setup landscape
			 ;mov     [LandSeg],ax            ; memory segment.

			 ;lea     di,[Palette]            ; set black palette.
			 ;mov     ax,ds
			 ;mov     es,ax
			 ;mov     cx,768
			 ;xor     ax,ax
			 ;cld
			 ;rep     stosb
			 ;lea     si,[Palette]
			 ;call    SetPalette

GenPalette:     lea     di,[Palette]            ; generate color palette.
			 mov     ax,3F00h
			 mov     [di+0],al               ; background.
			 mov     [di+1],al
			 mov     [di+2],al
			 add     di,3
			 mov     [di+0],al               ; blue. (water)
			 mov     [di+1],al
			 mov     [di+2],ah
			 add     di,3
			 mov     ax,0000h                ; green to brown.
			 mov     bx,3F00h
			 mov     dx,0000h
			 mov     cx,61
GP0:            mov     [di+0],ah
			 mov     [di+1],bh
			 mov     [di+2],dh
			 add     di,3
			 add     ax,0108h
			 sub     bx,0086h
			 add     dx,0000h
			 loop    GP0
			 mov     ax,3F00h                ; brown to white.
			 mov     bx,2000h                ; (mountain)
			 mov     dx,0000h
			 mov     cx,48
GP1:            mov     [di+0],ah
			 mov     [di+1],bh
			 mov     [di+2],dh
			 add     di,3
			 add     ax,0000h
			 add     bx,00AAh
			 add     dx,0150h
			 loop    GP1
			 mov     ax,3F00h                ; white. (snow)
			 mov     bx,3F00h
			 mov     dx,3F00h
			 mov     cx,145
GP2:            mov     [di+0],ah
			 mov     [di+1],bh
			 mov     [di+2],dh
			 add     di,3
			 add     ax,0000h
			 add     bx,0000h
			 add     dx,0000h
			 loop    GP2

_SetLandPal 	ENDP

			END