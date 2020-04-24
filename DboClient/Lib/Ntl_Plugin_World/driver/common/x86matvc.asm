; ***************************************************************************
;
; Module        : x86matvc.asm
;
; Purpose       : Matrix-vector products (x86 assembler)
;
; ***************************************************************************

    ifdef   TASM
        warn
    endif ; TASM

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
;   _rwIntelx86AsmV3dTransformPoints
;
; x86 "fast" version of RwV3dTransformPoints()
;
; Arguments in:     RwV3d          *pointsOut
;                   const RwV3d    *pointsIn
;                   RwInt32         numPoints
;                   const RwMatrix *matrix
;
; Values returned:  pointsOut
;

RWASMFUNC _rwIntelx86AsmV3dTransformPoints

    push    ebp
    mov     ebp, esp

ifdef STACK
    push    eax
    push    ebx
    push    ecx
    push    edx
endif ; STACK

    mov     ebx, [ebp + 20]                 ; := attach ebx to matrix
    mov     ecx, [ebp + 16]                 ; := attach ecx to numPoints
    mov     eax, [ebp + 12]                 ; := attach eax to pointsIn
    mov     edx, [ebp + 8]                  ; := attach edx to pointsOut

transformpoints:
    fld     dword ptr [eax + _rwV3d_nX]     ; := u(x)
    fld     dword ptr [ebx + mat_mxx]       ; := R(x)
    fmul    st(0), st(1)                    ; := u(x) * R(x), keep u(x) on the stack
    fld     dword ptr [ebx + mat_mxy]       ; := R(y)
    fmul    st(0), st(2)                    ; := u(x) * R(y), keep u(x) on the stack
    fld     dword ptr [ebx + mat_mxz]       ; := R(z)
    fmulp   st(3), st(0)                    ; := u(x) * R(z)

    fld     dword ptr [eax + _rwV3d_nY]     ; := u(y)
    fld     dword ptr [ebx + mat_myx]       ; := U(x)
    fmul    st(0), st(1)                    ; := u(y) * U(x), keep u(y) on the stack
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x)
    fld     dword ptr [ebx + mat_myy]       ; := U(y)
    fmul    st(0), st(1)                    ; := u(y) * U(y), keep u(y) on the stack
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y)
    fld     dword ptr [ebx + mat_myz]       ; := U(z)
    fmulp   st(1), st(0)                    ; := u(y) * U(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z)

    fld     dword ptr [eax + _rwV3d_nZ]     ; := u(z)
    fld     dword ptr [ebx + mat_mzx]       ; := A(x)
    fmul    st(0), st(1)                    ; := u(z) * A(x), keep u(z) on the stack
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x) + u(z) * A(x)
    fld     dword ptr [ebx + mat_mwx]       ; := P(x)
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x) + u(z) * A(x) + P(x)
    fld     dword ptr [ebx + mat_mzy]       ; := A(y)
    fmul    st(0), st(1)                    ; := u(z) * A(y), keep u(z) on the stack
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y) + u(z) * A(y)
    fld     dword ptr [ebx + mat_mwy]       ; := P(y)
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y) + u(z) * A(y) + P(y)
    fld     dword ptr [ebx + mat_mzz]       ; := A(z)
    fmulp   st(1), st(0)                    ; := u(z) * A(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z) + u(z) * A(z)
    fld     dword ptr [ebx + mat_mwz]       ; := P(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z) + u(z) * A(z) + P(z)

    fxch                                    ; swap u(x) * R(x) + u(y) * U(x) + u(z) * A(x) + P(x) back to st(0)
    fstp    dword ptr [edx + _rwV3d_nX]     ; v(x) = st(0)
    fstp    dword ptr [edx + _rwV3d_nY]     ; v(y) = st(1)
    fstp    dword ptr [edx + _rwV3d_nZ]     ; v(z) = st(2)

    add     eax, sizeof_rwV3d
    add     edx, sizeof_rwV3d
    dec     ecx
    jne     transformpoints

ifdef STACK
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax
endif ; STACK

    pop     ebp

    mov     eax, [esp + 8]  

    ret

; ***************************************************************************
;   _rwIntelx86AsmV3dTransformVectors
;
; x86 "fast" version of RwV3dTransformVectors()
;
; Arguments in:     RwV3d          *pointsOut
;                   const RwV3d    *pointsIn
;                   RwInt32         numPoints
;                   const RwMatrix *matrix
;
; Values returned:  pointsOut
;

RWASMFUNC _rwIntelx86AsmV3dTransformVectors

    push    ebp
    mov     ebp, esp

ifdef STACK
    push    eax
    push    ebx
    push    ecx
    push    edx
endif ; STACK

    mov     ebx, [ebp + 20]                 ; := attach ebx to matrix
    mov     ecx, [ebp + 16]                 ; := attach ecx to numPoints
    mov     eax, [ebp + 12]                 ; := attach eax to pointsIn
    mov     edx, [ebp + 8]                  ; := attach edx to pointsOut

transformvectors:
    fld     dword ptr [eax + _rwV3d_nX]     ; := u(x)
    fld     dword ptr [ebx + mat_mxx]       ; := R(x)
    fmul    st(0), st(1)                    ; := u(x) * R(x), keep u(x) on the stack
    fld     dword ptr [ebx + mat_mxy]       ; := R(y)
    fmul    st(0), st(2)                    ; := u(x) * R(y), keep u(x) on the stack
    fld     dword ptr [ebx + mat_mxz]       ; := R(z)
    fmulp   st(3), st(0)                    ; := u(x) * R(z)

    fld     dword ptr [eax + _rwV3d_nY]     ; := u(y)
    fld     dword ptr [ebx + mat_myx]       ; := U(x)
    fmul    st(0), st(1)                    ; := u(y) * U(x), keep u(y) on the stack
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x)
    fld     dword ptr [ebx + mat_myy]       ; := U(y)
    fmul    st(0), st(1)                    ; := u(y) * U(y), keep u(y) on the stack
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y)
    fld     dword ptr [ebx + mat_myz]       ; := U(z)
    fmulp   st(1), st(0)                    ; := u(y) * U(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z)

    fld     dword ptr [eax + _rwV3d_nZ]     ; := u(z)
    fld     dword ptr [ebx + mat_mzx]       ; := A(x)
    fmul    st(0), st(1)                    ; := u(z) * A(x), keep u(z) on the stack
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x) + u(z) * A(x)
    fld     dword ptr [ebx + mat_mzy]       ; := A(y)
    fmul    st(0), st(1)                    ; := u(z) * A(y), keep u(z) on the stack
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y) + u(z) * A(y)
    fld     dword ptr [ebx + mat_mzz]       ; := A(z)
    fmulp   st(1), st(0)                    ; := u(z) * A(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z) + u(z) * A(z)

    fxch                                    ; swap u(x) * R(x) + u(y) * U(x) + u(z) * A(x) + P(x) back to st(0)
    fstp    dword ptr [edx + _rwV3d_nX]     ; v(x) = st(0)
    fstp    dword ptr [edx + _rwV3d_nY]     ; v(y) = st(1)
    fstp    dword ptr [edx + _rwV3d_nZ]     ; v(z) = st(2)

    add     eax, sizeof_rwV3d
    add     edx, sizeof_rwV3d
    dec     ecx
    jne     transformvectors

ifdef STACK
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax
endif ; STACK

    pop     ebp

    mov     eax, [esp + 8]  

    ret


; ***************************************************************************
;   _rwIntelx86AsmV3dTransformPoint
;
; x86 "fast" version of RwV3dTransformPoint()
;
; Arguments in:     RwV3d          *pointOut
;                   const RwV3d    *pointIn
;                   const RwMatrix *matrix
;
; Values returned:  pointOut
;

RWASMFUNC _rwIntelx86AsmV3dTransformPoint

    push    ebp
    mov     ebp, esp

ifdef STACK
    push    edx
    push    ebx
    push    eax
endif ; STACK

    mov     ebx, [ebp + 16]                 ; := attach ebx to matrix
    mov     eax, [ebp + 12]                 ; := attach eax to pointIn
    mov     edx, [ebp + 8]                  ; := attach edx to pointOut

    fld     dword ptr [eax + _rwV3d_nX]     ; := u(x)
    fld     dword ptr [ebx + mat_mxx]       ; := R(x)
    fmul    st(0), st(1)                    ; := u(x) * R(x), keep u(x) on the stack
    fld     dword ptr [ebx + mat_mxy]       ; := R(y)
    fmul    st(0), st(2)                    ; := u(x) * R(y), keep u(x) on the stack
    fld     dword ptr [ebx + mat_mxz]       ; := R(z)
    fmulp   st(3), st(0)                    ; := u(x) * R(z)

    fld     dword ptr [eax + _rwV3d_nY]     ; := u(y)
    fld     dword ptr [ebx + mat_myx]       ; := U(x)
    fmul    st(0), st(1)                    ; := u(y) * U(x), keep u(y) on the stack
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x)
    fld     dword ptr [ebx + mat_myy]       ; := U(y)
    fmul    st(0), st(1)                    ; := u(y) * U(y), keep u(y) on the stack
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y)
    fld     dword ptr [ebx + mat_myz]       ; := U(z)
    fmulp   st(1), st(0)                    ; := u(y) * U(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z)

    fld     dword ptr [eax + _rwV3d_nZ]     ; := u(z)
    fld     dword ptr [ebx + mat_mzx]       ; := A(x)
    fmul    st(0), st(1)                    ; := u(z) * A(x), keep u(z) on the stack
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x) + u(z) * A(x)
    fld     dword ptr [ebx + mat_mwx]       ; := P(x)
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x) + u(z) * A(x) + P(x)
    fld     dword ptr [ebx + mat_mzy]       ; := A(y)
    fmul    st(0), st(1)                    ; := u(z) * A(y), keep u(z) on the stack
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y) + u(z) * A(y)
    fld     dword ptr [ebx + mat_mwy]       ; := P(y)
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y) + u(z) * A(y) + P(y)
    fld     dword ptr [ebx + mat_mzz]       ; := A(z)
    fmulp   st(1), st(0)                    ; := u(z) * A(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z) + u(z) * A(z)
    fld     dword ptr [ebx + mat_mwz]       ; := P(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z) + u(z) * A(z) + P(z)

    fxch                                    ; swap u(x) * R(x) + u(y) * U(x) + u(z) * A(x) + P(x) back to st(0)
    fstp    dword ptr [edx + _rwV3d_nX]     ; v(x) = st(0)
    fstp    dword ptr [edx + _rwV3d_nY]     ; v(y) = st(1)
    fstp    dword ptr [edx + _rwV3d_nZ]     ; v(z) = st(2)

ifdef STACK
    pop     eax
    pop     ebx
    pop     edx
endif ; STACK

    pop     ebp

    mov     eax, [esp + 8]  

    ret

; ***************************************************************************
;   _rwIntelx86AsmV3dTransformVector
;
; x86 "fast" version of RwV3dTransformVector()
;
; Arguments in:     RwV3d          *pointOut
;                   const RwV3d    *pointIn
;                   const RwMatrix *matrix
;
; Values returned:  pointsOut
;

RWASMFUNC _rwIntelx86AsmV3dTransformVector

    push    ebp
    mov     ebp, esp

ifdef STACK
    push    edx
    push    ebx
    push    eax
endif ; STACK

    mov     ebx, [ebp + 16]                 ; := attach ebx to matrix
    mov     eax, [ebp + 12]                 ; := attach eax to pointIn
    mov     edx, [ebp + 8]                  ; := attach edx to pointOut

    fld     dword ptr [eax + _rwV3d_nX]     ; := u(x)
    fld     dword ptr [ebx + mat_mxx]       ; := R(x)
    fmul    st(0), st(1)                    ; := u(x) * R(x), keep u(x) on the stack
    fld     dword ptr [ebx + mat_mxy]       ; := R(y)
    fmul    st(0), st(2)                    ; := u(x) * R(y), keep u(x) on the stack
    fld     dword ptr [ebx + mat_mxz]       ; := R(z)
    fmulp   st(3), st(0)                    ; := u(x) * R(z)

    fld     dword ptr [eax + _rwV3d_nY]     ; := u(y)
    fld     dword ptr [ebx + mat_myx]       ; := U(x)
    fmul    st(0), st(1)                    ; := u(y) * U(x), keep u(y) on the stack
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x)
    fld     dword ptr [ebx + mat_myy]       ; := U(y)
    fmul    st(0), st(1)                    ; := u(y) * U(y), keep u(y) on the stack
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y)
    fld     dword ptr [ebx + mat_myz]       ; := U(z)
    fmulp   st(1), st(0)                    ; := u(y) * U(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z)

    fld     dword ptr [eax + _rwV3d_nZ]     ; := u(z)
    fld     dword ptr [ebx + mat_mzx]       ; := A(x)
    fmul    st(0), st(1)                    ; := u(z) * A(x), keep u(z) on the stack
    faddp   st(3), st(0)                    ; := u(x) * R(x) + u(y) * U(x) + u(z) * A(x)
    fld     dword ptr [ebx + mat_mzy]       ; := A(y)
    fmul    st(0), st(1)                    ; := u(z) * A(y), keep u(z) on the stack
    faddp   st(2), st(0)                    ; := u(x) * R(y) + u(y) * U(y) + u(z) * A(y)
    fld     dword ptr [ebx + mat_mzz]       ; := A(z)
    fmulp   st(1), st(0)                    ; := u(z) * A(z)
    faddp   st(3), st(0)                    ; := u(x) * R(z) + u(y) * U(z) + u(z) * A(z)

    fxch                                    ; swap u(x) * R(x) + u(y) * U(x) + u(z) * A(x) + P(x) back to st(0)
    fstp    dword ptr [edx + _rwV3d_nX]     ; v(x) = st(0)
    fstp    dword ptr [edx + _rwV3d_nY]     ; v(y) = st(1)
    fstp    dword ptr [edx + _rwV3d_nZ]     ; v(z) = st(2)

ifdef STACK
    pop     eax
    pop     ebx
    pop     edx
endif ; STACK

    pop     ebp

    mov     eax, [esp + 8]  

    ret

RWASMCODESEGMENTEND

; ***************************************************************************

    end