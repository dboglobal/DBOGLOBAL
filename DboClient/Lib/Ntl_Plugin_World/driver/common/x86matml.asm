; ***************************************************************************
;
; Module        : batransf.asm
;
; Purpose       : Transformation 'go-faster' things for x86
;
; ***************************************************************************

	ifdef	TASM
	warn
	endif

; ***************************************************************************
; Includes

	include macros.i
        include baequate.i

RWASM386

; ***************************************************************************
; Data segment

RWASMDATASEGMENTBEGIN
RWASMDATASEGMENTEND

; ***************************************************************************
; Code segment

RWASMCODESEGMENTBEGIN

; ***************************************************************************
; _rwX86AsmMatMul

; On entry      : Matrix (OUT)
;               : Matrix A
;               : Matrix B
; On exit       :

RWASMFUNC _rwX86AsmMatMul
ifdef STACK
	push eax
	push edx
	push ebx

    mov     eax,[esp + 16]
    mov     edx,[esp + 20]
    mov     ebx,[esp + 24]
endif ; STACK
    ; edx src1
    ; ebx src2
    ; eax dst
	; ecx counter

	push ecx

	mov ecx,3

matmul_rep:
    fld DWORD PTR [edx+mat_mxx]     ; 1xx
    fmul DWORD PTR [ebx+mat_mxx]    ; 1xx*2xx
    fld DWORD PTR [edx+mat_mxy]     ; 1xy,1xx*2xx
    fmul DWORD PTR [ebx+mat_myx]    ; 1xy*2yx,1xx*2xx
    fld DWORD PTR [edx+mat_mxz]     ; 1xz,1xy*2yx,1xx*2xx
    fmul DWORD PTR [ebx+mat_mzx]    ; 1xz*2zx,1xy*2yx,1xx*2xx

    fld DWORD PTR [edx+mat_mxx]     ; 1xx,1xz*2zx,1xy*2yx,1xx*2xx
	fxch st(1)                      ; 1xz*2zx,1xx,1xy*2yx,1xx*2xx
	faddp st(2),st                  ; 1xx,1xz*2zx+1xy*2yx,1xx*2xx
    fmul DWORD PTR [ebx+mat_mxy]    ; 1xx*2xy,1xz*2zx+1xy*2yx,1xx*2xx
	fxch st(1)                      ; 1xz*2zx+1xy*2yx,1xx*2xy,1xx*2xx
	faddp st(2),st                  ; 1xx*2xy,R1
    fld DWORD PTR [edx+mat_mxy]     ; 1xy,1xx*2xy,R1
    fmul DWORD PTR [ebx+mat_myy]    ; 1xy*2yy,1xx*2xy,R1
    fld DWORD PTR [edx+mat_mxz]     ; 1xz,1xy*2yy,1xx*2xy,R1
    fmul DWORD PTR [ebx+mat_mzy]    ; 1xz*2zy,1xy*2yy,1xx*2xy,R1

    fld DWORD PTR [edx+mat_mxx]     ; 1xx,1xz*2zy,1xy*2yy,1xx*2xy,R1
	fxch st(1)                      ; 1xz*2zy,1xx,1xy*2yy,1xx*2xy,R1
	faddp st(2),st                  ; 1xx,1xz*2zy+1xy*2yy,1xx*2xy,R1
    fmul DWORD PTR [ebx+mat_mxz]    ; 1xx*2xz,1xz*2zy+1xy*2yy,1xx*2xy,R1
	fxch st(1)                      ; 1xz*2zy+1xy*2yy,1xx*2xz,1xx*2xy,R1
	faddp st(2),st                  ; 1xx*2xz,R2,R1
    fld DWORD PTR [edx+mat_mxy]     ; 1xy,1xx*2xz,R2,R1
    fmul DWORD PTR [ebx+mat_myz]    ; 1xy*2yz,1xx*2xz,R2,R1
    fld DWORD PTR [edx+mat_mxz]     ; 1xz,1xy*2yz,1xx*2xy,R2,R1
    fmul DWORD PTR [ebx+mat_mzz]    ; 1xz*2zy,1xy*2yz,1xx*2xy,R2,R1

	fxch st(4)                      ; R1,1xy*2yz,1xx*2xy,R2,1xz*2zy
    fstp DWORD PTR [eax+mat_mxx]    ; 1xy*2yz,1xx*2xy,R2,1xz*2zy
	faddp st(3),st                  ; 1xx*2xy,R2,1xz*2zy+1xy*2yz
	fxch st(1)                      ; R2,1xx*2xy,1xz*2zy+1xy*2yz
    fstp DWORD PTR [eax+mat_mxy]    ; 1xx*2xy,1xz*2zy+1xy*2yz
	faddp st(1),st                  ; R3

    lea edx,[edx+mat_myx-mat_mxx]   ; Next row
    dec ecx				; Decrease counter

    fstp DWORD PTR [eax+mat_mxz]    ;

    lea eax,[eax+mat_myx-mat_mxx]   ; Next line down!
    jnz matmul_rep

	; Handle the last line differently (Do the adds)

    fld DWORD PTR [edx+mat_mxx]     ; 1xx
    fmul DWORD PTR [ebx+mat_mxx]    ; 1xx*2xx
    fld DWORD PTR [edx+mat_mxy]     ; 1xy,1xx*2xx
    fxch st(1)
    fadd DWORD PTR [ebx+mat_mwx]    ; (Add)
	fxch st(1)
    fmul DWORD PTR [ebx+mat_myx]    ; 1xy*2yx,1xx*2xx
    fld DWORD PTR [edx+mat_mxz]     ; 1xz,1xy*2yx,1xx*2xx
    fmul DWORD PTR [ebx+mat_mzx]    ; 1xz*2zx,1xy*2yx,1xx*2xx

    fld DWORD PTR [edx+mat_mxx]     ; 1xx,1xz*2zx,1xy*2yx,1xx*2xx
	fxch st(1)                      ; 1xz*2zx,1xx,1xy*2yx,1xx*2xx
	faddp st(2),st                  ; 1xx,1xz*2zx+1xy*2yx,1xx*2xx
    fmul DWORD PTR [ebx+mat_mxy]    ; 1xx*2xy,1xz*2zx+1xy*2yx,1xx*2xx
	fxch st(1)                      ; 1xz*2zx+1xy*2yx,1xx*2xy,1xx*2xx
	faddp st(2),st                  ; 1xx*2xy,R1
    fld DWORD PTR [edx+mat_mxy]     ; 1xy,1xx*2xy,R1
	fxch st(1)
    fadd DWORD PTR [ebx+mat_mwy]    ; (Add)
	fxch st(1)
    fmul DWORD PTR [ebx+mat_myy]    ; 1xy*2yy,1xx*2xy,R1
    fld DWORD PTR [edx+mat_mxz]     ; 1xz,1xy*2yy,1xx*2xy,R1
    fmul DWORD PTR [ebx+mat_mzy]    ; 1xz*2zy,1xy*2yy,1xx*2xy,R1

    fld DWORD PTR [edx+mat_mxx]     ; 1xx,1xz*2zy,1xy*2yy,1xx*2xy,R1
	fxch st(1)                      ; 1xz*2zy,1xx,1xy*2yy,1xx*2xy,R1
	faddp st(2),st                  ; 1xx,1xz*2zy+1xy*2yy,1xx*2xy,R1
    fmul DWORD PTR [ebx+mat_mxz]    ; 1xx*2xz,1xz*2zy+1xy*2yy,1xx*2xy,R1
	fxch st(1)                      ; 1xz*2zy+1xy*2yy,1xx*2xz,1xx*2xy,R1
	faddp st(2),st                  ; 1xx*2xz,R2,R1
    fld DWORD PTR [edx+mat_mxy]     ; 1xy,1xx*2xz,R2,R1
	fxch st(1)
    fadd DWORD PTR [ebx+mat_mwz]    ; (Add)
	fxch st(1)
    fmul DWORD PTR [ebx+mat_myz]    ; 1xy*2yz,1xx*2xz,R2,R1
    fld DWORD PTR [edx+mat_mxz]     ; 1xz,1xy*2yz,1xx*2xy,R2,R1
    fmul DWORD PTR [ebx+mat_mzz]    ; 1xz*2zy,1xy*2yz,1xx*2xy,R2,R1

	fxch st(4)                      ; R1,1xy*2yz,1xx*2xy,R2,1xz*2zy
    fstp DWORD PTR [eax+mat_mxx]    ; 1xy*2yz,1xx*2xy,R2,1xz*2zy
	faddp st(3),st                  ; 1xx*2xy,R2,1xz*2zy+1xy*2yz
	fxch st(1)                      ; R2,1xx*2xy,1xz*2zy+1xy*2yz
    fstp DWORD PTR [eax+mat_mxy]    ; 1xx*2xy,1xz*2zy+1xy*2yz
	faddp st(1),st                  ; R3

    ; 1 clock
    fstp DWORD PTR [eax+mat_mxz]    ;

	pop ecx

ifdef STACK
	pop ebx
	pop edx
	pop eax
endif ; STACK
	ret


; ***************************************************************************
; _rwIntelx86AsmMatrixMultiply

; Improved matrix multiply function for Intel (and compatible) x86 processors.
; Near optimized for parallel instruction pipelining. Stalls still occur when
; the result of op N is used as an argument to op N+1.

; Features over the plain vanilla version:
;   * no local variable use
;   * LHS row elements cached in FP registers
;   * no obsolete stack stores
;   * stores to memory moved to avoid stalls with result computation

; Floating-point instruction tally:
;   fld     52
;   fstp    12
;   fmul[p] 36
;   fadd    27

; To do:
;   * eliminate pipeline stalls by ensuring result of op N is not used
;     in the argument of op N+1
;   * take operation cycle length into account to avoid more stalls

; Options eliminated already (for speed):
;   * Loop 4 times around single row calculation.
;   * Use of embedded integer assembler to calculate matrix offsets
;     to avoid additions during fld's and fstp's.

; On entry      : Matrix (OUT)
;               : Matrix A (LHS)
;               : Matrix B (RHS)
; On exit       : Nothing

RWASMFUNC _rwIntelx86AsmMatrixMultiply

    push    ebp
    mov     ebp, esp

ifdef STACK
    push eax
    push ebx
    push ecx
endif ; STACK

    mov     ebx,[ebp + 16]  ; ebx == Matrix B
    mov     eax,[ebp + 12]  ; eax == Matrix A
    mov     ecx,[ebp + 8]   ; ecx == Matrix (OUT)

    ; first (right) row
    ; a1, a2, a3 are cached in registers
    fld         dword ptr [eax + mat_mxz]   ; := a3
    fld         dword ptr [eax + mat_mxy]   ; := a2
    fld         dword ptr [eax + mat_mxx]   ; := a1
    ; c1 = a1 * b1 + a2 * b4 + a3 * b7
    fld         dword ptr [ebx + mat_mxx]   ; := b1
    fmul        st(0), st(1)                ; := a1 * b1
    fld         dword ptr [ebx + mat_myx]   ; := b4
    fmul        st(0), st(3)                ; := a2 * b4
    fadd                                    ; := a1 * b1 + a2 * b4
    fld         dword ptr [ebx + mat_mzx]   ; := b7
    fmul        st(0), st(4)                ; := a3 * b7
    fadd                                    ; := a1 * b1 + a2 * b4 + a3 * b7
    ; c2 = a1 * b2 + a2 * b5 + a3 * b8
    fld         dword ptr [ebx + mat_mxy]   ; := b2
    fmul        st(0), st(2)                ; := a1 * b2
    fxch        st(1)                       ; swap a1 * b1 + a2 * b4 + a3 * b7 back to st(0)
    fstp        dword ptr [ecx + mat_mxx]   ; c1 := st(0)
    fld         dword ptr [ebx + mat_myy]   ; := b5
    fmul        st(0), st(3)                ; := a2 * b5
    fadd                                    ; := a1 * b2 + a2 * b5
    fld         dword ptr [ebx + mat_mzy]   ; := b8
    fmul        st(0), st(4)                ; := a3 * b8
    fadd                                    ; := a1 * b2 + a2 * b5 + a3 * b8
    ; c3 = a1 * b3 + a2 * b6 + a3 * b9
    fld         dword ptr [ebx + mat_mxz]   ; := b3
    fmulp       st(2), st(0)                ; := a1 * b3, pop cached a1 matrix element
    fstp        dword ptr [ecx + mat_mxy]   ; c2 := st(0)
    fld         dword ptr [ebx + mat_myz]   ; := b6
    fmulp       st(2), st(0)                ; := a2 * b6, pop cached a2 matrix element
    fadd                                    ; := a1 * b3 + a2 * b6
    fld         dword ptr [ebx + mat_mzz]   ; := b9
    fmulp       st(2), st(0)                ; := a3 * b9, pop cached a3 matrix element
    fadd                                    ; := a1 * b3 + a2 * b6 + a3 * b9

    ; second (up) row
    ; a4, a5, a6 are cached in registers
    fld         dword ptr [eax + mat_myz]   ; := a6
    fxch        st(1)                       ; swap a1 * b3 + a2 * b6 + a3 * b9 back to st(0)
    fld         dword ptr [eax + mat_myy]   ; := a5
    fxch        st(1)                       ; swap a1 * b3 + a2 * b6 + a3 * b9 back to st(0)
    fld         dword ptr [eax + mat_myx]   ; := a4
    fxch        st(1)                       ; swap a1 * b3 + a2 * b6 + a3 * b9 back to st(0)
    fstp        dword ptr [ecx + mat_mxz]   ; c3 := st(0)
    ; c4 = a4 * b1 + a5 * b4 + a6 * b7
    fld         dword ptr [ebx + mat_mxx]   ; := b1
    fmul        st(0), st(1)                ; := a4 * b1
    fld         dword ptr [ebx + mat_myx]   ; := b4
    fmul        st(0), st(3)                ; := a5 * b4
    fadd                                    ; := a4 * b1 + a5 * b4
    fld         dword ptr [ebx + mat_mzx]   ; := b7
    fmul        st(0), st(4)                ; := a6 * b7
    fadd                                    ; := a4 * b1 + a5 * b4 + a6 * b7
    ; c5 = a4 * b2 + a5 * b5 + a6 * b8
    fld         dword ptr [ebx + mat_mxy]   ; := b2
    fmul        st(0), st(2)                ; := a4 * b2
    fxch        st(1)                       ; swap a4 * b1 + a5 * b4 + a6 * b7 back to st(0)
    fstp        dword ptr [ecx + mat_myx]   ; c4 := st(0)
    fld         dword ptr [ebx + mat_myy]   ; := b5
    fmul        st(0), st(3)                ; := a5 * b5
    fadd                                    ; := a4 * b2 + a5 * b5
    fld         dword ptr [ebx + mat_mzy]   ; := b8
    fmul        st(0), st(4)                ; := a6 * b8
    fadd                                    ; := a4 * b2 + a5 * b5 + a6 * b8
    ; c6 = a4 * b3 + a5 * b6 + a6 * b9
    fld         dword ptr [ebx + mat_mxz]   ; := b3
    fmulp       st(2), st(0)                ; := a4 * b3, pop cached a4 matrix element
    fstp        dword ptr [ecx + mat_myy]   ; c5 := st(0)
    fld         dword ptr [ebx + mat_myz]   ; := b6
    fmulp       st(2), st(0)                ; := a5 * b6, pop cached a5 matrix element
    fadd                                    ; := a4 * b3 + a5 * b6
    fld         dword ptr [ebx + mat_mzz]   ; := b9
    fmulp       st(2), st(0)                ; := a6 * b9, pop cached a6 matrix element
    fadd                                    ; := a4 * b3 + a5 * b6 + a6 * b9

    ; third (at) row
    ; a7, a8, a9 are cached in registers
    fld         dword ptr [eax + mat_mzz]   ; := a9
    fxch        st(1)                       ; swap a4 * b3 + a5 * b6 + a6 * b9 back to st(0)
    fld         dword ptr [eax + mat_mzy]   ; := a8
    fxch        st(1)                       ; swap a4 * b3 + a5 * b6 + a6 * b9 back to st(0)
    fld         dword ptr [eax + mat_mzx]   ; := a7
    fxch        st(1)                       ; swap a4 * b3 + a5 * b6 + a6 * b9 back to st(0)
    fstp        dword ptr [ecx + mat_myz]   ; c6 := st(0)
    ; c7 = a7 * b1 + a8 * b4 + a9 * b7
    fld         dword ptr [ebx + mat_mxx]   ; := b1
    fmul        st(0), st(1)                ; := a7 * b1
    fld         dword ptr [ebx + mat_myx]   ; := b4
    fmul        st(0), st(3)                ; := a8 * b4
    fadd                                    ; := a7 * b1 + a8 * b4
    fld         dword ptr [ebx + mat_mzx]   ; := b7
    fmul        st(0), st(4)                ; := a9 * b7
    fadd                                    ; := a7 * b1 + a8 * b4 + a9 * b7
    ; c8 = a7 * b2 + a8 * b5 + a9 * b8
    fld         dword ptr [ebx + mat_mxy]   ; := b2
    fmul        st(0), st(2)                ; := a7 * b2
    fxch        st(1)                       ; swap a7 * b1 + a8 * b4 + a9 * b7 back to st(0)
    fstp        dword ptr [ecx + mat_mzx]   ; c7 := st(0)
    fld         dword ptr [ebx + mat_myy]   ; := b5
    fmul        st(0), st(3)                ; := a8 * b5
    fadd                                    ; := a7 * b2 + a8 * b5
    fld         dword ptr [ebx + mat_mzy]   ; := b8
    fmul        st(0), st(4)                ; := a9 * b8
    fadd                                    ; := a7 * b2 + a8 * b5 + a9 * b8
    ; c9 = a7 * b3 + a8 * b6 + a9 * b9
    fld         dword ptr [ebx + mat_mxz]   ; := b3
    fmulp       st(2), st(0)                ; := a7 * b3, pop cached a7 matrix element
    fstp        dword ptr [ecx + mat_mzy]   ; c8 := st(0)
    fld         dword ptr [ebx + mat_myz]   ; := b6
    fmulp       st(2), st(0)                ; := a8 * b6, pop cached a8 matrix element
    fadd                                    ; := a7 * b3 + a8 * b6
    fld         dword ptr [ebx + mat_mzz]   ; := b9
    fmulp       st(2), st(0)                ; := a9 * b9, pop cached a9 matrix element
    fadd                                    ; := a7 * b3 + a8 * b6 + a9 * b9

    ; fourth (pos) row
    ; a10, a11, a12 are cached in registers
    fld         dword ptr [eax + mat_mwz]   ; := a12
    fxch        st(1)                       ; swap a7 * b3 + a8 * b6 + a9 * b9 back to st(0)
    fld         dword ptr [eax + mat_mwy]   ; := a11
    fxch        st(1)                       ; swap a7 * b3 + a8 * b6 + a9 * b9 back to st(0)
    fld         dword ptr [eax + mat_mwx]   ; := a10
    fxch        st(1)                       ; swap a7 * b3 + a8 * b6 + a9 * b9 back to st(0)
    fstp        dword ptr [ecx + mat_mzz]   ; c9 := st(0)
    ; c10 = a10 * b1 + a11 * b4 + a12 * b7 + b10
    fld         dword ptr [ebx + mat_mxx]   ; := b1
    fmul        st(0), st(1)                ; := a10 * b1
    fld         dword ptr [ebx + mat_myx]   ; := b4
    fmul        st(0), st(3)                ; := a11 * b4
    fadd                                    ; := a10 * b1 + a11 * b4
    fld         dword ptr [ebx + mat_mzx]   ; := b7
    fmul        st(0), st(4)                ; := a12 * b7
    fadd                                    ; := a10 * b1 + a11 * b4 + a12 * b7
    fld         dword ptr [ebx + mat_mwx]   ; := b10
    fadd                                    ; := a10 * b1 + a11 * b4 + a12 * b7 + b10
    ; c11 = a10 * b2 + a11 * b5 + a12 * b8 + b11
    fld         dword ptr [ebx + mat_mxy]   ; := b2
    fmul        st(0), st(2)                ; := a10 * b2
    fxch        st(1)                       ; swap a10 * b1 + a11 * b4 + a12 * b7 + b10 back to st(0)
    fstp        dword ptr [ecx + mat_mwx]   ; c10 := st(0)
    fld         dword ptr [ebx + mat_myy]   ; := b5
    fmul        st(0), st(3)                ; := a11 * b5
    fadd                                    ; := a10 * b2 + a11 * b5
    fld         dword ptr [ebx + mat_mzy]   ; := b8
    fmul        st(0), st(4)                ; := a12 * b8
    fadd                                    ; := a10 * b2 + a11 * b5 + a12 * b8
    fld         dword ptr [ebx + mat_mwy]   ; := b11
    fadd                                    ; := a10 * b2 + a11 * b5 + a12 * b8 + b11
    ; c12 = a10 * b3 + a11 * b6 + a12 * b9 + b12
    fld         dword ptr [ebx + mat_mxz]   ; := b3
    fmulp       st(2), st(0)                ; := a10 * b3, pop cached a10 matrix element
    fstp        dword ptr [ecx + mat_mwy]   ; c11 := st(0)
    fld         dword ptr [ebx + mat_myz]   ; := b6
    fmulp       st(2), st(0)                ; := a11 * b6, pop cached a11 matrix element
    fadd                                    ; := a10 * b3 + a11 * b6
    fld         dword ptr [ebx + mat_mzz]   ; := b9
    fmulp       st(2), st(0)                ; := a12 * b9, pop cached a12 matrix element
    fadd                                    ; := a10 * b3 + a11 * b6 + a12 * b9
    fld         dword ptr [ebx + mat_mwz]   ; := b12
    fadd                                    ; := a10 * b3 + a11 * b6 + a12 * b9 + b12
    fstp        dword ptr [ecx + mat_mwz]   ; c12 := st(0)

ifdef STACK
    pop ecx
    pop ebx
    pop eax
endif ; STACK

    pop ebp

    ret


RWASMCODESEGMENTEND
	end
