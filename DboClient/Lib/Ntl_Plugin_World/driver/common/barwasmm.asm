; ***************************************************************************
;
; Module        : barwasmm.asm
;
; Purpose       : Assembler maths
;
; ***************************************************************************

.386

	ifdef	TASM
	warn
	endif

; ***************************************************************************
; Includes

        include bamacros.i
        include baequate.i
        include barwasmg.i

; ***************************************************************************
; Externals

; ***************************************************************************
; Equates

; ***************************************************************************
; Data segment

dseg    segment para public use32 'DATA'
dseg	ends

; ***************************************************************************
; Code segment

cseg	segment dword public use32 'CODE'
	assume cs:cseg,ds:dseg,es:dseg

; ***************************************************************************
; _rwFMul

StartAt 8
ifdef STACK
ParamD nA
ParamD nB
endif ; STACK
        func    _rwFRwMul
    ifdef STACK
        push ebp
        mov ebp,esp
        push edx

        mov eax,[ebp+nA]
        mov edx,[ebp+nB]

        imul edx

        shr eax,16

        shl edx,16

        or eax,edx

        pop edx
        pop ebp
        _ret

    else
    ifdef WATCOM

        ; eax
        ; edx

        push edx
        imul edx

        shr eax,16

        shl edx,16

        or eax,edx
        pop edx
        _ret

    endif
    endif

; ***************************************************************************
; _rwFDiv

; On entry      : a
;               : b
; On exit       : a/b

StartAt	8
    ifdef      STACK
ParamD  nA
ParamD  nB
    endif
        func    _rwFRwDiv
    ifdef      STACK
        push ebp
        mov ebp,esp

        push edx
        push ebx

        mov eax,[ebp+nA]
        mov ebx,[ebp+nB]
    else
    ifdef       WATCOM
        push edx
        push ebx

        mov ebx,edx
    endif
    endif

        mov edx,eax

        shl eax,16

        sar edx,16

        idiv ebx

        pop ebx
        pop edx
ifdef STACK
        pop ebp
endif ; STACK
        _ret

cseg	ends
	end
