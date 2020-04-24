/* 
 * SSE Matrix Multiply
 *
 * Copyright (c) 2001 Criterion Software Ltd.
 */

#include "batypes.h"
#include "barwtyp.h"
#include "badebug.h"

#include "SSEmatml.h"

#define X0X0X0X0      0x00
#define X1X1X1X1      0x55
#define X2X2X2X2      0xAA
#define X2X2X1X0      0xA4

#define _SHUFFLE(fp3,fp2,fp1,fp0) (((fp3) << 6) | ((fp2) << 4) | \
                                     ((fp1) << 2) | ((fp0)))

#define accum         xmm0
#define term          xmm1
#define altterm       xmm2
#define BR            xmm3
#define BA            xmm4
#define BU            xmm5
#define BP            xmm6

#define OUTptr        eax
#define Aptr          ebx
#define Bptr          ecx

#if !defined( NOASM ) && !defined( NOSSEASM )
/*
 * _rwIntelSSEMatrixMultiply
 *
 * Optimized SSE matrix multiplication
 *
 * On entry      : Matrix dstMat    (OUT)
 *               : Matrix matA      (LHS)
 *               : Matrix matB      (RHS)
 * On exit       : Nothing
 *
 * Note: this function is using un-aligned reads.
 */
void
_rwIntelSSEMatrixMultiply(RwMatrix *dstMat,
                          const RwMatrix *matA,
                          const RwMatrix *matB)
{
    RWFUNCTION(RWSTRING("_rwIntelSSEMatrixMultiply"));

    /*
    dstMat->right = ARXXX * matB->right + ARYYY * matB->up + ARZZZ * matB->at;
    dstMat->up = AUXXX * matB->right + AUYYY * matB->up + AUZZZ * matB->at;
    dstMat->at = AAXXX * matB->right + AAYYY * matB->up + AAZZZ * matB->at;
    dstMat->pos = APXXX * matB->right + APYYY * matA->up + APZZZ * matB->at + matB->pos;
    */

    __asm
    {
        mov     Bptr,    matB
        mov     Aptr,    matA
        mov     OUTptr,  dstMat

        prefetchnta [Bptr + 32]

        /* Load Matrix B */
        test    Bptr,  0x0f
        jz short readBaligned

        movups  BR, [ Bptr ]
        movups  BA, [ Bptr + 16 ]
        movups  BU, [ Bptr + 32 ]
        movups  BP, [ Bptr + 48 ]
        jmp short endreadB

readBaligned:

        movaps  BR, [ Bptr ]
        movaps  BA, [ Bptr + 16 ]
        movaps  BU, [ Bptr + 32 ]
        movaps  BP, [ Bptr + 48 ]

endreadB:

        test    Aptr, 0x0f
        jz short readaligned1

        movups  term, [ Aptr ]           /* Get A r */
        jmp short endread1
        
readaligned1:
        movaps  term, [ Aptr ]           /* Get A r */

endread1:

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
        shufps  BR, BR, X2X2X1X0
        shufps  BA, BA, X2X2X1X0
        shufps  BU, BU, X2X2X1X0
        shufps  BP, BP, X2X2X1X0

        /* dstMat->right = ARXXX * matB->right + ARYYY * matB->up + ARZZZ * matB->at; */

        movaps accum,   term             
        movaps altterm, term            

        shufps accum,   accum,   X0X0X0X0 /* accum = A r.xxxx */
        shufps term,    term,    X1X1X1X1 /* term = A r.yyyy */
        shufps altterm, altterm, X2X2X2X2 /* altterm = A r.zzzz */

        mulps  accum, BR                /* accum *= BR */      

        mulps  term, BA                 /* term *= BA */

        mulps  altterm, BU              /* altterm *= BU */

        addps  accum, term              /* accum += term */

        prefetchnta [Aptr + 32]
        
        test    Aptr, 0x0f
        jz short readaligned2

        movups  term,   [ Aptr + 16 ]    /* Get A u */
        jmp short endread2
        
readaligned2:
        movaps  term,   [ Aptr + 16 ]    /* Get A u */

endread2:

        addps  accum, altterm           /* accum += altterm */

        test    OUTptr, 0x0f
        jz short writealigned1

        movups  XMMWORD PTR[ OUTptr ], accum /* store r */
        jmp short endwrite1
        
writealigned1:
        movaps  XMMWORD PTR[ OUTptr ], accum /* store r */

endwrite1:

        /* dstMat->up = AUXXX * matB->right + AUYYY * matB->up + AUZZZ * matB->at; */

        movaps accum,   term             
        movaps altterm, term            

        shufps accum,   accum,   X0X0X0X0 /* accum = A r.xxxx */
        shufps term,    term,    X1X1X1X1 /* term = A r.yyyy */
        shufps altterm, altterm, X2X2X2X2 /* altterm = A r.zzzz */

        mulps  accum, BR                /* accum *= BR */

        mulps  term, BA                 /* term *= BA */

        mulps  altterm, BU              /* altterm *= BU */

        addps  accum, term              /* accum += term */

        test    Aptr, 0x0f
        jz short readaligned3

        movups  term,   [ Aptr + 32 ]    /* Get A a */
        jmp short endread3
        
readaligned3:
        movaps  term,   [ Aptr + 32 ]    /* Get A a */

endread3:

        addps  accum, altterm           /* accum += altterm */

        test    OUTptr, 0x0f
        jz short writealigned2

        movups  XMMWORD PTR[ OUTptr + 16 ], accum /* store u */
        jmp short endwrite2
        
writealigned2:
        movaps  XMMWORD PTR[ OUTptr + 16 ], accum /* store r */

endwrite2:

        /* dstMat->at = AAXXX * matB->right + AAYYY * matB->up + AAZZZ * matB->at; */

        movaps accum,   term             
        movaps altterm, term            

        shufps accum,   accum,   X0X0X0X0 /* accum = A r.xxxx */
        shufps term,    term,    X1X1X1X1 /* term = A r.yyyy */
        shufps altterm, altterm, X2X2X2X2 /* altterm = A r.zzzz */

        mulps  accum, BR                /* accum *= BR */       

        mulps  term, BA                 /* term *= BA */

        mulps  altterm, BU                 /* altterm *= BU */
        
        addps  accum, term              /* accum += term */

        test    Aptr, 0x0f
        jz short readaligned4

        movups  term,   [ Aptr + 48 ]    /* Get A p */
        jmp short endread4
        
readaligned4:
        movaps  term,   [ Aptr + 48 ]    /* Get A p */

endread4:

        addps  accum, altterm              /* accum += altterm */

        test    OUTptr, 0x0f
        jz short writealigned3

        movups  XMMWORD PTR[ OUTptr + 32 ], accum /* store u */
        jmp short endwrite3
        
writealigned3:
        movaps  XMMWORD PTR[ OUTptr + 32 ], accum /* store r */

endwrite3:

        /* dstMat->pos = APXXX * matB->right + APYYY * matA->up + APZZZ * matB->at + matB->pos; */

        movaps accum,   term             
        movaps altterm, term            

        shufps accum,   accum,   X0X0X0X0 /* accum = A r.xxxx */
        shufps term,    term,    X1X1X1X1 /* term = A r.yyyy */
        shufps altterm, altterm, X2X2X2X2 /* altterm = A r.zzzz */

        mulps  accum, BR                /* accum *= BR */
    
        mulps  term, BA                 /* term *= BA */

        mulps  altterm, BU                 /* altterm *= BU */

        addps  accum, term              /* accum += term */

        /* rwMatrixSetFlags( dstMat, (matA->flags & matB->flags) ); */
        mov edx, dword ptr [Aptr+0Ch]

        addps  altterm, BP              /* accum += altterm */

        and edx, dword ptr [Bptr+0Ch]

        addps  accum, altterm           /* accum += BP */

        mov dword ptr [OUTptr+0Ch], edx

        test    OUTptr, 0x0f
        jz short writealigned4

        movups  XMMWORD PTR[ OUTptr + 48 ], accum /* store u */
        jmp short endwrite4
        
writealigned4:
        movaps  XMMWORD PTR[ OUTptr + 48 ], accum /* store r */

endwrite4:
    }

    RWRETURNVOID();
}


/*
 * _rwIntelSSEAlignedMatrixMultiply
 *
 * Optimized SSE aligned matrix multiplication
 *
 * On entry      : Matrix dstMat    (OUT)
 *               : Matrix matA      (LHS)
 *               : Matrix matB      (RHS)
 * On exit       : Nothing
 *
 */
void
_rwIntelSSEAlignedMatrixMultiply( RwMatrix *dstMat,
                                  const RwMatrix *matA,
                                  const RwMatrix *matB )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEAlignedMatrixMultiply"));

    RWASSERT( RWMATRIXALIGNMENT( dstMat ) );
    RWASSERT( RWMATRIXALIGNMENT( matA ) );
    RWASSERT( RWMATRIXALIGNMENT( matB ) );

    /*
    dstMat->right = ARXXX * matB->right + ARYYY * matB->up + ARZZZ * matB->at;
    dstMat->up = AUXXX * matB->right + AUYYY * matB->up + AUZZZ * matB->at;
    dstMat->at = AAXXX * matB->right + AAYYY * matB->up + AAZZZ * matB->at;
    dstMat->pos = APXXX * matB->right + APYYY * matA->up + APZZZ * matB->at + matB->pos;
    */

    __asm
    {
        mov     Bptr,    matB
        mov     Aptr,    matA
        mov     OUTptr,  dstMat

        prefetchnta [Bptr + 32]

        /* Load Matrix B */
        movaps  BR, [ Bptr ]
        movaps  BA, [ Bptr + 16 ]
        movaps  BU, [ Bptr + 32 ]
        movaps  BP, [ Bptr + 48 ]

        movaps  term, [ Aptr ]           /* Get A r */

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
        shufps  BR, BR, X2X2X1X0
        shufps  BA, BA, X2X2X1X0
        shufps  BU, BU, X2X2X1X0
        shufps  BP, BP, X2X2X1X0

        /* dstMat->right = ARXXX * matB->right + ARYYY * matB->up + ARZZZ * matB->at; */

        movaps accum,   term             
        movaps altterm, term            

        shufps accum,   accum,   X0X0X0X0 /* accum = A r.xxxx */
        shufps term,    term,    X1X1X1X1 /* term = A r.yyyy */
        shufps altterm, altterm, X2X2X2X2 /* altterm = A r.zzzz */

        mulps  accum, BR                /* accum *= BR */      

        mulps  term, BA                 /* term *= BA */

        mulps  altterm, BU              /* altterm *= BU */

        addps  accum, term              /* accum += term */

        prefetchnta [Aptr + 32]
        
        movaps  term,   [ Aptr + 16 ]    /* Get A u */

        addps  accum, altterm           /* accum += altterm */

        movaps  XMMWORD PTR[ OUTptr ], accum /* store r */

        /* dstMat->up = AUXXX * matB->right + AUYYY * matB->up + AUZZZ * matB->at; */

        movaps accum,   term             
        movaps altterm, term            

        shufps accum,   accum,   X0X0X0X0 /* accum = A r.xxxx */
        shufps term,    term,    X1X1X1X1 /* term = A r.yyyy */
        shufps altterm, altterm, X2X2X2X2 /* altterm = A r.zzzz */

        mulps  accum, BR                /* accum *= BR */

        mulps  term, BA                 /* term *= BA */

        mulps  altterm, BU              /* altterm *= BU */

        addps  accum, term              /* accum += term */

        movaps  term,   [ Aptr + 32 ]    /* Get A a */

        addps  accum, altterm           /* accum += altterm */

        movaps  XMMWORD PTR[ OUTptr + 16 ], accum /* store r */

        /* dstMat->at = AAXXX * matB->right + AAYYY * matB->up + AAZZZ * matB->at; */

        movaps accum,   term             
        movaps altterm, term            

        shufps accum,   accum,   X0X0X0X0 /* accum = A r.xxxx */
        shufps term,    term,    X1X1X1X1 /* term = A r.yyyy */
        shufps altterm, altterm, X2X2X2X2 /* altterm = A r.zzzz */

        mulps  accum, BR                /* accum *= BR */       

        mulps  term, BA                 /* term *= BA */

        mulps  altterm, BU                 /* altterm *= BU */
        
        addps  accum, term              /* accum += term */

        movaps  term,   [ Aptr + 48 ]    /* Get A p */

        addps  accum, altterm              /* accum += altterm */

        movaps  XMMWORD PTR[ OUTptr + 32 ], accum /* store r */

        /* dstMat->pos = APXXX * matB->right + APYYY * matA->up + APZZZ * matB->at + matB->pos; */

        movaps accum,   term             
        movaps altterm, term            

        shufps accum,   accum,   X0X0X0X0 /* accum = A r.xxxx */
        shufps term,    term,    X1X1X1X1 /* term = A r.yyyy */
        shufps altterm, altterm, X2X2X2X2 /* altterm = A r.zzzz */

        mulps  accum, BR                /* accum *= BR */
    
        mulps  term, BA                 /* term *= BA */

        mulps  altterm, BU                 /* altterm *= BU */

        addps  accum, term              /* accum += term */

        /* rwMatrixSetFlags( dstMat, (matA->flags & matB->flags) ); */
        mov edx, dword ptr [Aptr+0Ch]

        addps  altterm, BP              /* accum += altterm */

        and edx, dword ptr [Bptr+0Ch]

        addps  accum, altterm           /* accum += BP */

        mov dword ptr [OUTptr+0Ch], edx

        movaps  XMMWORD PTR[ OUTptr + 48 ], accum /* store r */
    }

    RWRETURNVOID();
}


/*
 * _rwIntelSSEV3dTransformPoints
 *
 * Optimized SSE points transformation
 *
 * Note: this function is using un-aligned reads.
 */
RwV3d *
_rwIntelSSEV3dTransformPoints(RwV3d          *pointsOut,
                              const RwV3d    *pointsIn,  
                              RwInt32        numPoints,  
                              const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwIntelSSEV3dTransformPoints"));

    __asm
    {
        mov     eax,  matrix
        mov     ecx,  numPoints

        test    eax,  0x0f
        jz short alignedtransformpoints

        movups  xmm3, [eax]
        movups  xmm4, [eax + 16]
        movups  xmm5, [eax + 32]
        movups  xmm6, [eax + 48]
        jmp short cleartransformpoints

alignedtransformpoints:
        
        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]
        movaps  xmm6, [eax + 48]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
cleartransformpoints:

        shufps  xmm3, xmm3, X2X2X1X0

        mov     esi,  pointsIn

        shufps  xmm4, xmm4, X2X2X1X0

        mov     edi,  pointsOut

        shufps  xmm5, xmm5, X2X2X1X0
        
        shufps  xmm6, xmm6, X2X2X1X0

begintransformpoints:

        movss   xmm0, [esi]
        movss   xmm1, [esi + 4]
        movss   xmm2, [esi + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        add     esi, 12

        addps   xmm0, xmm6
        addps   xmm2, xmm1

        add     edi, 12

        addps   xmm2, xmm0

        dec     ecx

        movhlps xmm0, xmm2

        movlps  [edi - 12 + 0], xmm2
        
        movss   [edi - 12 + 8], xmm0

        jnz short begintransformpoints
    }

    RWRETURN(pointsOut);
}


/*
 * _rwIntelSSEV3dTransformPointsAlignedMatrix
 *
 * Optimized SSE points transformation with aligned matrix
 *
 */
RwV3d *
_rwIntelSSEV3dTransformPointsAlignedMatrix( RwV3d          *pointsOut,
                                            const RwV3d    *pointsIn,  
                                            RwInt32        numPoints,  
                                            const RwMatrix *matrix )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEV3dTransformPointsAlignedMatrix"));

    RWASSERT( RWMATRIXALIGNMENT( matrix ) );

    __asm
    {
        mov     eax,  matrix
        mov     ecx,  numPoints

        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]
        movaps  xmm6, [eax + 48]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */

        shufps  xmm3, xmm3, X2X2X1X0

        mov     esi,  pointsIn

        shufps  xmm4, xmm4, X2X2X1X0

        mov     edi,  pointsOut

        shufps  xmm5, xmm5, X2X2X1X0
        
        shufps  xmm6, xmm6, X2X2X1X0

begintransformpoints:

        movss   xmm0, [esi]
        movss   xmm1, [esi + 4]
        movss   xmm2, [esi + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        add     esi, 12

        addps   xmm0, xmm6
        addps   xmm2, xmm1

        add     edi, 12

        addps   xmm2, xmm0

        dec     ecx

        movhlps xmm0, xmm2

        movlps  [edi - 12 + 0], xmm2
        
        movss   [edi - 12 + 8], xmm0

        jnz short begintransformpoints
    }

    RWRETURN(pointsOut);
}


/*
 * _rwIntelSSEV3dTransformVectors
 *
 * Optimized SSE vectors transformation
 *
 * Note: this function is using un-aligned reads.
 */
RwV3d *
_rwIntelSSEV3dTransformVectors(RwV3d          *vectorsOut,
                               const RwV3d    *vectorsIn,  
                               RwInt32        numVectors,  
                               const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwIntelSSEV3dTransformVectors"));

    __asm
    {
        mov     eax,  matrix
        mov     ecx,  numVectors

        test    eax,  0x0f
        jz short alignedtransformvectors

        movups  xmm3, [eax]
        movups  xmm4, [eax + 16]
        movups  xmm5, [eax + 32]
        jmp short cleartransformvectors

alignedtransformvectors:

        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
cleartransformvectors:

        shufps  xmm3, xmm3, X2X2X1X0

        mov     esi,  vectorsIn

        shufps  xmm4, xmm4, X2X2X1X0

        mov     edi,  vectorsOut

        shufps  xmm5, xmm5, X2X2X1X0

begintransformvectors:
        movss   xmm0, [esi]
        movss   xmm1, [esi + 4]
        movss   xmm2, [esi + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        add     esi, 12

        addps   xmm1, xmm0

        add     edi, 12

        addps   xmm2, xmm1

        dec ecx

        movhlps xmm0, xmm2

        movlps  [edi - 12 + 0], xmm2
        
        movss   [edi - 12 + 8], xmm0

        jnz short begintransformvectors
    }

    RWRETURN(vectorsOut);
}


/*
 * _rwIntelSSEV3dTransformVectorsAlignedMatrix
 *
 * Optimized SSE vectors transformation with aligned matrix
 *
 */
RwV3d *
_rwIntelSSEV3dTransformVectorsAlignedMatrix( RwV3d          *vectorsOut,
                                             const RwV3d    *vectorsIn,  
                                             RwInt32        numVectors,  
                                             const RwMatrix *matrix )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEV3dTransformVectorsAlignedMatrix"));

    RWASSERT( RWMATRIXALIGNMENT( matrix ) );

    __asm
    {
        mov     eax,  matrix
        mov     ecx,  numVectors

        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */

        shufps  xmm3, xmm3, X2X2X1X0

        mov     esi,  vectorsIn

        shufps  xmm4, xmm4, X2X2X1X0

        mov     edi,  vectorsOut

        shufps  xmm5, xmm5, X2X2X1X0

begintransformvectors:
        movss   xmm0, [esi]
        movss   xmm1, [esi + 4]
        movss   xmm2, [esi + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        add     esi, 12

        addps   xmm1, xmm0

        add     edi, 12

        addps   xmm2, xmm1

        dec ecx

        movhlps xmm0, xmm2

        movlps  [edi - 12 + 0], xmm2
        
        movss   [edi - 12 + 8], xmm0

        jnz short begintransformvectors
    }

    RWRETURN(vectorsOut);
}

/*
 * _rwIntelSSEV3dTransformPoint
 *
 * Optimized SSE points transformation
 *
 * Note: this function is using un-aligned reads.
 */
RwV3d *
_rwIntelSSEV3dTransformPoint(RwV3d          *pointOut,
                             const RwV3d    *pointIn,  
                             const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwIntelSSEV3dTransformPoint"));

    __asm
    {
        mov     eax,  matrix

        test    eax,  0x0f
        jz short alignedtransformpoints

        movups  xmm3, [eax]
        movups  xmm4, [eax + 16]
        movups  xmm5, [eax + 32]
        movups  xmm6, [eax + 48]
        jmp short cleartransformpoints

alignedtransformpoints:
        
        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]
        movaps  xmm6, [eax + 48]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
cleartransformpoints:

        shufps  xmm3, xmm3, X2X2X1X0

        mov     eax,  pointIn

        shufps  xmm4, xmm4, X2X2X1X0

        shufps  xmm5, xmm5, X2X2X1X0
        
        shufps  xmm6, xmm6, X2X2X1X0

        movss   xmm0, [eax]
        movss   xmm1, [eax + 4]
        movss   xmm2, [eax + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        addps   xmm0, xmm6
        addps   xmm2, xmm1

        mov     eax,  pointOut

        addps   xmm2, xmm0

        movhlps xmm0, xmm2

        movlps  [eax + 0], xmm2
        
        movss   [eax + 8], xmm0
    }

    RWRETURN(pointOut);
}


/*
 * _rwIntelSSEV3dTransformPointAlignedMatrix
 *
 * Optimized SSE points transformation with aligned matrix
 *
 */
RwV3d *
_rwIntelSSEV3dTransformPointAlignedMatrix( RwV3d          *pointOut,
                                           const RwV3d    *pointIn,  
                                           const RwMatrix *matrix )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEV3dTransformPointAlignedMatrix"));

    RWASSERT( RWMATRIXALIGNMENT( matrix ) );

    __asm
    {
        mov     eax,  matrix

        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]
        movaps  xmm6, [eax + 48]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */

        shufps  xmm3, xmm3, X2X2X1X0

        mov     eax,  pointIn

        shufps  xmm4, xmm4, X2X2X1X0

        shufps  xmm5, xmm5, X2X2X1X0
        
        shufps  xmm6, xmm6, X2X2X1X0

        movss   xmm0, [eax + 0]
        movss   xmm1, [eax + 4]
        movss   xmm2, [eax + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        addps   xmm0, xmm6
        addps   xmm2, xmm1

        mov     eax,  pointOut

        addps   xmm2, xmm0

        movhlps xmm0, xmm2

        movlps  [eax + 0], xmm2
        
        movss   [eax + 8], xmm0
    }

    RWRETURN(pointOut);
}


/*
 * _rwIntelSSEV3dTransformVector
 *
 * Optimized SSE vectors transformation
 *
 * Note: this function is using un-aligned reads.
 */
RwV3d *
_rwIntelSSEV3dTransformVector(RwV3d          *vectorOut,
                              const RwV3d    *vectorIn,  
                              const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwIntelSSEV3dTransformVector"));

    __asm
    {
        mov     eax,  matrix

        test    eax,  0x0f
        jz short alignedtransformvectors

        movups  xmm3, [eax]
        movups  xmm4, [eax + 16]
        movups  xmm5, [eax + 32]
        jmp short cleartransformvectors

alignedtransformvectors:

        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
cleartransformvectors:

        shufps  xmm3, xmm3, X2X2X1X0

        mov     eax,  vectorIn

        shufps  xmm4, xmm4, X2X2X1X0

        shufps  xmm5, xmm5, X2X2X1X0

        movss   xmm0, [eax]
        movss   xmm1, [eax + 4]
        movss   xmm2, [eax + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        addps   xmm1, xmm0

        mov     eax,  vectorOut

        addps   xmm2, xmm1

        movhlps xmm0, xmm2

        movlps  [eax + 0], xmm2
        
        movss   [eax + 8], xmm0
    }

    RWRETURN(vectorOut);
}


/*
 * _rwIntelSSEV3dTransformVectorAlignedMatrix
 *
 * Optimized SSE vectors transformation with aligned matrix
 *
 */
RwV3d *
_rwIntelSSEV3dTransformVectorAlignedMatrix( RwV3d          *vectorOut,
                                            const RwV3d    *vectorIn,  
                                            const RwMatrix *matrix )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEV3dTransformVectorAlignedMatrix"));

    RWASSERT( RWMATRIXALIGNMENT( matrix ) );

    __asm
    {
        mov     eax,  matrix

        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */

        shufps  xmm3, xmm3, X2X2X1X0

        mov     eax,  vectorIn

        shufps  xmm4, xmm4, X2X2X1X0

        shufps  xmm5, xmm5, X2X2X1X0

        movss   xmm0, [eax]
        movss   xmm1, [eax + 4]
        movss   xmm2, [eax + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        addps   xmm1, xmm0

        addps   xmm2, xmm1

        mov     eax,  vectorOut

        movhlps xmm0, xmm2

        movlps  [eax + 0], xmm2
        
        movss   [eax + 8], xmm0
    }

    RWRETURN(vectorOut);
}

/*
 * _rwIntelSSEP4V3dTransformPoints
 *
 * Optimized SSE points transformation
 *
 * Note: this function is using un-aligned reads.
 */
RwV3d *
_rwIntelSSEP4V3dTransformPoints(RwV3d          *pointsOut,
                                const RwV3d    *pointsIn,  
                                RwInt32        numPoints,  
                                const RwMatrix *matrix )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEP4V3dTransformPoints"));

    __asm
    {
        mov     eax,  matrix
        mov     ecx,  numPoints

        test    eax,  0x0f
        jz short alignedtransformpoints

        movups  xmm3, [eax]
        movups  xmm4, [eax + 16]
        movups  xmm5, [eax + 32]
        movups  xmm6, [eax + 48]
        jmp short cleartransformpoints

alignedtransformpoints:
        
        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]
        movaps  xmm6, [eax + 48]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
cleartransformpoints:

        shufps  xmm3, xmm3, X2X2X1X0

        mov     esi,  pointsIn

        shufps  xmm4, xmm4, X2X2X1X0

        mov     edi,  pointsOut

        shufps  xmm5, xmm5, X2X2X1X0
        
        shufps  xmm6, xmm6, X2X2X1X0

begintransformpoints:

        movss   xmm0, [esi]
        movss   xmm1, [esi + 4]
        movss   xmm2, [esi + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        add     esi, 12

        addps   xmm0, xmm6
        addps   xmm2, xmm1

        add     edi, 12

        addps   xmm2, xmm0

        dec     ecx

        movss   [edi - 12 + 0], xmm2
        movhlps xmm0, xmm2
        shufps  xmm2, xmm2, _SHUFFLE(3,2,1,1)
        movss   [edi - 12 + 4], xmm2
        movss   [edi - 12 + 8], xmm0

        jnz short begintransformpoints
    }

    RWRETURN(pointsOut);
}

/*
 * _rwIntelSSEP4V3dTransformVectors
 *
 * Optimized SSE vector transformation
 *
 * Note: this function is using un-aligned reads.
 */
RwV3d *
_rwIntelSSEP4V3dTransformVectors(RwV3d          *vectorsOut,
                                 const RwV3d    *vectorsIn,  
                                 RwInt32        numVectors,  
                                 const RwMatrix *matrix )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEP4V3dTransformVectors"));

    __asm
    {
        mov     eax,  matrix
        mov     ecx,  numVectors

        test    eax,  0x0f
        jz short alignedtransformvectors

        movups  xmm3, [eax]
        movups  xmm4, [eax + 16]
        movups  xmm5, [eax + 32]
        jmp short cleartransformvectors

alignedtransformvectors:

        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
cleartransformvectors:

        shufps  xmm3, xmm3, X2X2X1X0

        mov     esi,  vectorsIn

        shufps  xmm4, xmm4, X2X2X1X0

        mov     edi,  vectorsOut

        shufps  xmm5, xmm5, X2X2X1X0

begintransformvectors:
        movss   xmm0, [esi]
        movss   xmm1, [esi + 4]
        movss   xmm2, [esi + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        add     esi, 12

        addps   xmm1, xmm0

        add     edi, 12

        addps   xmm2, xmm1

        dec ecx

        movss   [edi - 12 + 0], xmm2
        movhlps xmm0, xmm2
        shufps  xmm2, xmm2, _SHUFFLE(3,2,1,1)
        movss   [edi - 12 + 4], xmm2
        movss   [edi - 12 + 8], xmm0

        jnz short begintransformvectors
    }

    RWRETURN(vectorsOut);
}

/*
 * _rwIntelSSEP4V3dTransformPoint
 *
 * Optimized SSE point transformation
 *
 * Note: this function is using un-aligned reads.
 */
RwV3d *
_rwIntelSSEP4V3dTransformPoint(RwV3d          *pointOut,
                               const RwV3d    *pointIn,  
                               const RwMatrix *matrix )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEP4V3dTransformPoint"));

    __asm
    {
        mov     eax,  matrix

        test    eax,  0x0f
        jz short alignedtransformpoints

        movups  xmm3, [eax]
        movups  xmm4, [eax + 16]
        movups  xmm5, [eax + 32]
        movups  xmm6, [eax + 48]
        jmp short cleartransformpoints

alignedtransformpoints:
        
        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]
        movaps  xmm6, [eax + 48]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
cleartransformpoints:

        shufps  xmm3, xmm3, X2X2X1X0

        mov     eax,  pointIn

        shufps  xmm4, xmm4, X2X2X1X0

        shufps  xmm5, xmm5, X2X2X1X0
        
        shufps  xmm6, xmm6, X2X2X1X0

        movss   xmm0, [eax]
        movss   xmm1, [eax + 4]
        movss   xmm2, [eax + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        addps   xmm0, xmm6
        addps   xmm2, xmm1

        mov     eax,  pointOut

        addps   xmm2, xmm0

        movss   [eax], xmm2
        movhlps xmm0, xmm2
        shufps  xmm2, xmm2, _SHUFFLE(3,2,1,1)
        movss   [eax + 4], xmm2
        movss   [eax + 8], xmm0
    }

    RWRETURN(pointOut);
}

/*
 * _rwIntelSSEP4V3dTransformVector
 *
 * Optimized SSE vector transformation
 *
 * Note: this function is using un-aligned reads.
 */
RwV3d *
_rwIntelSSEP4V3dTransformVector(RwV3d          *vectorOut,
                                const RwV3d    *vectorIn,  
                                const RwMatrix *matrix )
{
    RWFUNCTION(RWSTRING("_rwIntelSSEP4V3dTransformVector"));

    __asm
    {
        mov     eax,  matrix

        test    eax,  0x0f
        jz short alignedtransformvectors

        movups  xmm3, [eax]
        movups  xmm4, [eax + 16]
        movups  xmm5, [eax + 32]
        jmp short cleartransformvectors

alignedtransformvectors:

        movaps  xmm3, [eax]
        movaps  xmm4, [eax + 16]
        movaps  xmm5, [eax + 32]

        /*
        sort out the garbage pads/flags in B rows
        or you get nasty floating point exceptions
        which slow everything down a lot
        */
cleartransformvectors:

        shufps  xmm3, xmm3, X2X2X1X0

        mov     eax,  vectorIn

        shufps  xmm4, xmm4, X2X2X1X0

        shufps  xmm5, xmm5, X2X2X1X0

        movss   xmm0, [eax]
        movss   xmm1, [eax + 4]
        movss   xmm2, [eax + 8]

        shufps  xmm0, xmm0, X0X0X0X0
        shufps  xmm1, xmm1, X0X0X0X0
        shufps  xmm2, xmm2, X0X0X0X0

        mulps   xmm0, xmm3
        mulps   xmm1, xmm4
        mulps   xmm2, xmm5

        addps   xmm1, xmm0

        mov     eax,  vectorOut

        addps   xmm2, xmm1

        movss   [eax], xmm2
        movhlps xmm0, xmm2
        shufps  xmm2, xmm2, _SHUFFLE(3,2,1,1)
        movss   [eax + 4], xmm2
        movss   [eax + 8], xmm0
    }

    RWRETURN(vectorOut);
}

#endif /* !defined( NOASM ) && !defined( NOSSEASM ) */