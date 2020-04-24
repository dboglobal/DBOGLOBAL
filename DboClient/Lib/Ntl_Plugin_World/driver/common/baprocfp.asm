; ***************************************************************************
;
; Module        : baprocfp.asm
;
; Purpose       : Processor floating point mode stuff
;
; ***************************************************************************

; ***************************************************************************
; Includes

    include macros.i

RWASM386

; ***************************************************************************
; Data segment

RWASMDATASEGMENTBEGIN
    _rwFPUCtrl      dd		0	; FPU State
    _rwFPUSaveState dd		0
RWASMDATASEGMENTEND

; ***************************************************************************
; Code segment

RWASMCODESEGMENTBEGIN

; ***************************************************************************
; _rwProcessorInitialize
;
; Save the FP control register, and set the FPU to single precision
; Only called once at RwEngineStart
; 
; Note: This is highly *inefficient* if called upon every RwCameraBeginUpdate.

RWASMFUNC _rwProcessorInitialize
        push eax

        fstcw WORD PTR [_rwFPUCtrl]             ; 2

        mov eax,DWORD PTR [_rwFPUCtrl]

        mov [_rwFPUSaveState],eax               ; For restore later
        and eax, not 300h                       ; single precision
        or  eax, 3fh                            ; disable all exceptions

        ; Done

        mov DWORD PTR [_rwFPUCtrl],eax

        fldcw WORD PTR [_rwFPUCtrl]             ; 7

        pop eax
    ret

; ***************************************************************************
; _rwProcessorRelease
;
; Restore the saved FP control register.
; Only called once at RwEngineStop
; 
; Note: This is highly *inefficient* if called upon every RwCameraEndUpdate.
    
RWASMFUNC _rwProcessorRelease
        fldcw WORD PTR [_rwFPUSaveState]        ; 7
    ret


; ***************************************************************************
; _rwProcessorForceSinglePrecision
;
; Force the FPU into single precision
; Called upon every RwCameraBeginUpdate

RWASMFUNC _rwProcessorForceSinglePrecision
        fstcw WORD PTR [_rwFPUCtrl]             ; 2

        and [_rwFPUCtrl], not 300h              ; single precision
        or  [_rwFPUCtrl], 3fh                   ; disable all exceptions

        fldcw WORD PTR [_rwFPUCtrl]             ; 7
    ret


RWASMCODESEGMENTEND

    end
