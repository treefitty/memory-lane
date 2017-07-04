;
; 32-bit Multiply and Divide functions for
; C 32-bit 16.16 fixed point math routines
;

ALIGNMENT equ 2

		.MODEL large
		.386
		.CODE

;=====================================================================
; Multiplies two fixed-point values together.
; C near-callable as:
;       Fixedpoint FixedMul(Fixedpoint M1, Fixedpoint M2);
;=====================================================================
FMparms struc
		   dd      ?
		   dw      ?        ;return address & pushed BP
	M1      dd      ?
	M2      dd      ?
FMparms ends
		align   ALIGNMENT
		public  _fixed_mul
_fixed_mul proc far
		push    bp
		mov     bp,sp

		mov     eax,[bp+M1]
		imul    dword ptr [bp+M2] ;multiply
		shr     eax,16          ;put the fractional part in AX

		pop     bp
		ret

_fixed_mul endp

;=====================================================================
; Divides one fixed-point value by another.
; C near-callable as:
;       Fixedpoint FixedDiv(Fixedpoint Dividend, Fixedpoint Divisor);
;=====================================================================
FDparms struc
		    dd     ?
		    dw     ?        ;return address & pushed BP
	Dividend dd     ?
	Divisor  dd     ?
FDparms ends

		align   ALIGNMENT
		public  _fixed_div
_fixed_div	proc far

		push    bp
		mov     bp,sp

		mov     edx,[bp+Dividend]
		sub     eax,eax
		shrd    eax,edx,16      ;position so that result ends up
		sar     edx,16          ; in EAX
		idiv    dword ptr [bp+Divisor]
		shld    edx,eax,16      ;whole part of result in DX;
						    ; fractional part is already in AX
		pop     bp
		ret
_fixed_div endp

		END





