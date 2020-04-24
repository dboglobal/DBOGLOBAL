/***************************************************************************
 *                                                                         *
 * Module  : bamatrix.h                                                    *
 *                                                                         *
 * Purpose : Matrix handling                                               *
 *                                                                         *
 **************************************************************************/

#ifndef RWMATRIX_H
#define RWMATRIX_H

/****************************************************************************
 Includes
 */

#include "bamemory.h"
#include "bamath.h"

/* RWPUBLIC */

/****************************************************************************
 Defines
 */

#define RWMATRIXALIGNMENT(_mat) \
   (! (((rwMATRIXALIGNMENT)-1) & ((RwUInt32)(_mat))))

#if (defined(RWMATRIXMONITOR))
# if (defined(_MSC_VER))
#  if ((_MSC_VER>=1000) && defined(_DEBUG))

typedef char        MatrixString[1024];

#define RWMATRIXPRINT(_matrix)                             \
MACRO_START                                                \
{                                                          \
    MatrixString        message;                           \
    MatrixString        output;                            \
                                                           \
    if (NULL != (_matrix))                                 \
    {                                                      \
        const RwV3d * const _x = &(_matrix)->right;        \
        const RwV3d * const _y = &(_matrix)->up;           \
        const RwV3d * const _z = &(_matrix)->at;           \
        const RwV3d * const _w = &(_matrix)->pos;          \
                                                           \
        _snprintf(message, sizeof(MatrixString),           \
                  "[ [ %8.4f, %8.4f, %8.4f, %8.4f ]\n"     \
                  "  [ %8.4f, %8.4f, %8.4f, %8.4f ]\n"     \
                  "  [ %8.4f, %8.4f, %8.4f, %8.4f ]\n"     \
                  "  [ %8.4f, %8.4f, %8.4f, %8.4f ] ]\n"   \
                  "  %08x == flags\n",                     \
                  _x->x, _x->y, _x->z, (RwReal) 0,         \
                  _y->x, _y->y, _y->z, (RwReal) 0,         \
                  _z->x, _z->y, _z->z, (RwReal) 0,         \
                  _w->x, _w->y, _w->z, (RwReal) 1,         \
                  (_matrix)->flags);                       \
    }                                                      \
    else                                                   \
    {                                                      \
        _snprintf(message, sizeof(MatrixString),           \
                  "NULL");                                 \
    }                                                      \
                                                           \
    _snprintf(output, sizeof(MatrixString),                \
              "%s(%d): %s [%p] ==\n%s\n",                  \
              __FILE__, __LINE__,                          \
              #_matrix, _matrix, message);                 \
                                                           \
    OutputDebugString(RWSTRING(output));                   \
}                                                          \
MACRO_STOP

#  endif /* ((_MSC_VER>=1000) && defined(_DEBUG)) */
# endif /* (defined(_MSC_VER)) */
#endif /* (defined(RWMATRIXMONITOR)) */

#if (!(defined(RWMATRIXPRINT)))
#define RWMATRIXPRINT(_matrix) /* No op */
#endif /* (!(defined(RWMATRIXPRINT))) */

#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwmatrix
 * enum RwOpCombineType 
 * This type represents a combination operator which 
 * can be applied to frames and matrices.
 * The operator determines the order 
 * in which one object is combined with another 
 */
#endif /* RWADOXYGENEXTERNAL */
enum RwOpCombineType
{
    rwCOMBINEREPLACE = 0,   /**<Replace - 
                                all previous transformations are lost */
    rwCOMBINEPRECONCAT,     /**<Pre-concatenation - 
                                the given transformation is applied 
                                before all others */
    rwCOMBINEPOSTCONCAT,    /**<Post-concatenation - 
                                the given transformation is applied 
                                after all others */
    rwOPCOMBINETYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

/*
 * RwOpCombineType typedef for enum RwOpCombineType
 */
typedef enum RwOpCombineType RwOpCombineType;

/* External flags (bits 0-15) */

/* Internal flags (bits 16-31) */
enum RwMatrixType
{
    rwMATRIXTYPENORMAL = 0x00000001,
    rwMATRIXTYPEORTHOGONAL = 0x00000002,
    rwMATRIXTYPEORTHONORMAL = 0x00000003,
    rwMATRIXTYPEMASK = 0x00000003,
    rwMATRIXTYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwMatrixType RwMatrixType;

enum RwMatrixFlag
{
    rwMATRIXINTERNALIDENTITY = 0x00020000,
    rwMATRIXFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwMatrixFlag RwMatrixFlag;

/* Flags describing what will optimize for */
enum RwMatrixOptimizations
{
    rwMATRIXOPTIMIZE_IDENTITY = 0x00020000,
    rwMATRIXOPTIMIZATIONSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwMatrixOptimizations RwMatrixOptimizations;

/****************************************************************************
 Global Types
 */

#if (!defined(DOXYGEN))
struct RwMatrixTag
{
    /* These are padded to be 16 byte quantities per line */
    RwV3d               right;
    RwUInt32            flags;
    RwV3d               up;
    RwUInt32            pad1;
    RwV3d               at;
    RwUInt32            pad2;
    RwV3d               pos;
    RwUInt32            pad3;
};

/*
 * RwMatrix & RwMatrixTag must be different otherwise the alignment
 * directive is ignored when compiling under C++ on Visual C
 */
typedef struct RwMatrixTag RWALIGN(RwMatrix, rwMATRIXALIGNMENT);

#else

/*
 * RwMatrix must appear as the struct name & the typedef,
 * otherwise Doxygen puts RwMatrixTag in the API ref instead of RwMatrix!
 */

/**
 * \ingroup rwmatrix
 * \struct RwMatrix
 * Matrix to define transformations. 
 * This should be considered an opaque type.
 * Use the RwMatrix API functions to access.
 */
typedef struct RwMatrix RWALIGN(RwMatrix, rwMATRIXALIGNMENT);
#endif /* (!defined(DOXYGEN)) */

#if (!defined(RwMatrixCopyMacro))
#define RwMatrixCopyMacro(_target, _source)             \
    ( *(_target) = *(_source) )
#endif /* (!defined(RwMatrixCopyMacro)) */

#if (!defined(RwMatrixSetIdentityMacro))
#define RwMatrixSetIdentityMacro(m)                                     \
MACRO_START                                                             \
{                                                                       \
    (m)->right.x = (m)->up.y    = (m)->at.z    = (RwReal)((1.0));       \
    (m)->right.y = (m)->right.z = (m)->up.x    = (RwReal)((0.0));       \
    (m)->up.z    = (m)->at.x    = (m)->at.y    = (RwReal)((0.0));       \
    (m)->pos.x   = (m)->pos.y   = (m)->pos.z   = (RwReal)((0.0));       \
    rwMatrixSetFlags((m),                                               \
                     rwMatrixGetFlags(m) |                              \
                     (rwMATRIXINTERNALIDENTITY |                        \
                      rwMATRIXTYPEORTHONORMAL));                        \
}                                                                       \
MACRO_STOP
#endif /* (!defined(RwMatrixSetIdentityMacro)) */

typedef void (RWASMCALL * rwMatrixMultFn) (RwMatrix * dstMat,
                                           const RwMatrix * matA,
                                           const RwMatrix * matB);

typedef struct RwMatrixTolerance RwMatrixTolerance;

#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwmatrix
 * \struct RwMatrixTolerance
 * Holds tolerances for matrix optimizations with \ref RwMatrixOptimize
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwMatrixTolerance
{
    RwReal              Normal;
        /**< Tolerance within which matrix is deemed to be normal   */
    RwReal              Orthogonal;
        /**< Tolerance within which matrix is deemed to be orthogonal */
    RwReal              Identity;
        /**< Tolerance within which matrix is deemed to be identity */
};

/* RWPUBLICEND */

typedef struct rwMatrixGlobals rwMatrixGlobals;
struct rwMatrixGlobals
{
    RwFreeList         *matrixFreeList;
    RwInt32             matrixOptimizations;
    rwMatrixMultFn      multMatrix;
    RwMatrixTolerance   tolerance;
};

typedef struct RwMatrixFixed RwMatrixFixed;
struct RwMatrixFixed
{
    RwV3dFixed          right;
    RwUInt32            flags;
    RwV3dFixed          up;
    RwUInt32            pad1;
    RwV3dFixed          at;
    RwUInt32            pad2;
    RwV3dFixed          pos;
    RwUInt32            pad3;
};

/* RWPUBLIC */

/****************************************************************************
 Function prototypes
 */

/* Matrix operations */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern RwBool
RwEngineGetMatrixTolerances(RwMatrixTolerance * const tolerance);

extern RwBool
RwEngineSetMatrixTolerances(const RwMatrixTolerance * const tolerance);

/* Update */
#define rwMatrixSetFlags(m, flagsbit)     ((m)->flags = (flagsbit))
#define rwMatrixGetFlags(m)               ((m)->flags)
#define rwMatrixTestFlags(m, flagsbit)    ((m)->flags & (RwInt32)(flagsbit))

/* Creation/destruction */
extern void
RwMatrixSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );

extern RwBool       
RwMatrixDestroy(RwMatrix * mpMat);

extern RwMatrix *
RwMatrixCreate(void);

#ifdef RWDEBUG

/* Functions for debug */
extern void
RwMatrixCopy(RwMatrix * dstMatrix, const RwMatrix * srcMatrix);

extern void         
RwMatrixSetIdentity(RwMatrix * matrix);

#else /* RWDEBUG */

#define RwMatrixCopy(dst, src)   RwMatrixCopyMacro(dst, src)
#define RwMatrixSetIdentity(m)   RwMatrixSetIdentityMacro(m)

#endif /* RWDEBUG */

/* Matrix multiply */
extern RwMatrix *
RwMatrixMultiply(RwMatrix * matrixOut,
                 const RwMatrix * MatrixIn1,
                 const RwMatrix * matrixIn2);

extern RwMatrix *
RwMatrixTransform(RwMatrix * matrix,
                  const RwMatrix * transform,
                  RwOpCombineType combineOp);

/* Normalise */
extern RwMatrix *
RwMatrixOrthoNormalize(RwMatrix * matrixOut,
                       const RwMatrix * matrixIn);

/* Inversion */
extern RwMatrix *
RwMatrixInvert(RwMatrix * matrixOut,
               const RwMatrix * matrixIn);

/* Unary matrix operations */
extern RwMatrix *
RwMatrixScale(RwMatrix * matrix,
              const RwV3d * scale,
              RwOpCombineType combineOp);

extern RwMatrix *
RwMatrixTranslate(RwMatrix * matrix,
                  const RwV3d * translation,
                  RwOpCombineType combineOp);

extern RwMatrix *
RwMatrixRotate(RwMatrix * matrix,
               const RwV3d * axis,
               RwReal angle,
               RwOpCombineType combineOp);

extern RwMatrix *
RwMatrixRotateOneMinusCosineSine(RwMatrix * matrix,
                                 const RwV3d * unitAxis,
                                 RwReal oneMinusCosine,
                                 RwReal sine,
                                 RwOpCombineType combineOp);

/* Query what the matrix is */
extern const RwMatrix *RwMatrixQueryRotate(const RwMatrix * matrix,
                                           RwV3d * unitAxis,
                                           RwReal * angle,
                                           RwV3d * center);

/* Get components */
#ifndef RWDEBUG

#define RwMatrixGetRight(m)    (&(m)->right)
#define RwMatrixGetUp(m)       (&(m)->up)
#define RwMatrixGetAt(m)       (&(m)->at)
#define RwMatrixGetPos(m)      (&(m)->pos)

#else /* RWDEBUG */

extern RwV3d *
RwMatrixGetRight(RwMatrix * matrix);

extern RwV3d *
RwMatrixGetUp(RwMatrix * matrix);

extern RwV3d *
RwMatrixGetAt(RwMatrix * matrix);

extern RwV3d *
RwMatrixGetPos(RwMatrix * matrix);

#endif /* RWDEBUG */

/* Update the internal matrix state wrt its elements */
extern RwMatrix *
RwMatrixUpdate(RwMatrix * matrix);

/* Update the internal matrix flags wrt its elements */
extern RwMatrix *
RwMatrixOptimize(RwMatrix * matrix,
                 const RwMatrixTolerance *
                 tolerance);

extern RwReal
_rwMatrixDeterminant(const RwMatrix * matrix);

extern RwReal
_rwMatrixNormalError(const RwMatrix * matrix);

extern RwReal
_rwMatrixOrthogonalError(const RwMatrix * matrix);

extern RwReal
_rwMatrixIdentityError(const RwMatrix * matrix);

/* RWPUBLICEND */

extern RwBool
_rwMatrixSetOptimizations(RwInt32 optimizeFlags);

extern RwBool 
_rwMatrixSetMultFn(rwMatrixMultFn multMat);

extern void        *
_rwMatrixOpen(void *instance,
              RwInt32 offset, 
              RwInt32 size);

extern void        *
_rwMatrixClose(void *instance,
               RwInt32 offset, 
               RwInt32 size);
/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* Compatibility macros */

#define rwMatrixSetOptimizations(optimizeFlags) \
       _rwMatrixSetOptimizations(optimizeFlags)

#define rwMatrixSetMultFn(multMat) \
       _rwMatrixSetMultFn(multMat)

#define rwMatrixOpen(instance, offset, size) \
       _rwMatrixOpen(instance, offset, size)

#define rwMatrixClose(instance, offset, size) \
       _rwMatrixClose(instance, offset, size)

/* Initialisation/deinitialisation */
#define rwMatrixInitialize(m, t)                \
MACRO_START                                     \
{                                               \
    rwMatrixSetFlags((m), (t));                 \
}                                               \
MACRO_STOP

#define rwMatrixIsNormal(_matrix, _epsilon)        \
  ( (_epsilon) >= _rwMatrixNormalError(_matrix) )

#define rwMatrixIsOrthogonal(_matrix, _epsilon)        \
  ( (_epsilon) >= _rwMatrixOrthogonalError(_matrix) )

#define rwMatrixIsOrthonormal(_matrix, _epsilon)        \
  ( rwMatrixIsNormal(_matrix, _epsilon) &&              \
    rwMatrixIsOrthogonal(_matrix, _epsilon)  )

#define rwMatrixIsOrthonormalPositive(_matrix, _epsilon)                \
  ( rwMatrixIsOrthonormal(_matrix, _epsilon) &&                         \
( (((RwReal)1) - (_epsilon)) <= _rwMatrixDeterminant(_matrix) ) )

#define rwMatrixIsIdentity(_matrix, _epsilon)           \
   ( (_epsilon) >= _rwMatrixIdentityError(_matrix) )

#define rwMatrixValidFlags(_matrix, _epsilon)                           \
   ( (_matrix) &&                       /* valid pointer */             \
 ( ( !( rwMatrixGetFlags(_matrix) &    /* not flagged as identity */    \
        rwMATRIXINTERNALIDENTITY) ) || /* .. or actually is */          \
   rwMatrixIsIdentity(_matrix, _epsilon)) &&                            \
 ( ( !( rwMatrixGetFlags(_matrix) &    /* not flagged as normal */      \
        rwMATRIXTYPENORMAL) ) ||  /* ... or actually is */              \
   rwMatrixIsNormal(_matrix, _epsilon)) &&                              \
 ( ( !( rwMatrixGetFlags(_matrix) &    /* not flagged as orthogonal */  \
        rwMATRIXTYPEORTHOGONAL) ) ||  /* ... or actually is */          \
   rwMatrixIsOrthogonal(_matrix, _epsilon)) )

#define      rwMat01Det(_mAA)                                           \
  ( (_mAA) )

#define      rwMat02Det(_mAA, _mAB,                                     \
                        _mBA, _mBB)                                     \
  ( (_mAA) * rwMat01Det(_mBB)                                           \
  - (_mAB) * rwMat01Det(_mBA)                                           \
  )

#define      rwMat03Det(_mAA, _mAB, _mAC,                               \
                        _mBA, _mBB, _mBC,                               \
                        _mCA, _mCB, _mCC)                               \
  ( (_mAA) * rwMat02Det(_mBB, _mBC,                                     \
                        _mCB, _mCC)                                     \
  - (_mAB) * rwMat02Det(_mBA, _mBC,                                     \
                        _mCA, _mCC)                                     \
  + (_mAC) * rwMat02Det(_mBA, _mBB,                                     \
                        _mCA, _mCB)                                     \
  )

#define      rwMat04Det(_mAA, _mAB, _mAC, _mAD,                         \
                        _mBA, _mBB, _mBC, _mBD,                         \
                        _mCA, _mCB, _mCC, _mCD,                         \
                        _mDA, _mDB, _mDC, _mDD)                         \
  ( (_mAA) * rwMat03Det(_mBB, _mBC, _mBD,                               \
                        _mCB, _mCC, _mCD,                               \
                        _mDB, _mDC, _mDD)                               \
  - (_mAB) * rwMat03Det(_mBA, _mBC, _mBD,                               \
                        _mCA, _mCC, _mCD,                               \
                        _mDA, _mDC, _mDD)                               \
  + (_mAC) * rwMat03Det(_mBA, _mBB, _mBD,                               \
                        _mCA, _mCB, _mCD,                               \
                        _mDA, _mDB, _mDD)                               \
  - (_mAD) * rwMat03Det(_mBA, _mBB, _mBC,                               \
                        _mCA, _mCB, _mCC,                               \
                        _mDA, _mDB, _mDC)                               \
  )


#define rwMat02Inv(_rAA, _rAB,                                          \
                   _rBA, _rBB)                                          \
                   _mAA, _mAB,                                          \
                   _mBA, _mBB)                                          \
MACRO_START                                                             \
{                                                                       \
    RwSplitBits determinant;                                            \
                                                                        \
    (_rAA) =  rwMat01Det(_mBB);                                         \
    (_rAB) = -rwMat01Det(_mAB);                                         \
                                                                        \
    determinant.nReal = ( (_rAA) * (_mAA) +                             \
                          (_rAB) * (_mBA) );                            \
                                                                        \
                                                                        \
    {                                                                   \
        const RwReal normalize = ( (determinant.nInt != 0)?             \
                                   (((RwReal)1)/determinant.nReal):     \
                                   ((RwReal)1) );                       \
                                                                        \
        (_rAA) *= normalize;                                            \
        (_rAB) *= normalize;                                            \
                                                                        \
        (_rBA) = -rwMat01Det(_mBA) * normalize;                         \
        (_rBB) =  rwMat01Det(_mAA) * normalize;                         \
    }                                                                   \
}                                                                       \
MACRO_STOP

#define    rwMat03Inv(_rAA, _rAB, _rAC,                                 \
                      _rBA, _rBB, _rBC,                                 \
                      _rCA, _rCB, _rCC,                                 \
                      _mAA, _mAB, _mAC,                                 \
                      _mBA, _mBB, _mBC,                                 \
                      _mCA, _mCB, _mCC)                                 \
MACRO_START                                                             \
{                                                                       \
    RwSplitBits determinant;                                            \
                                                                        \
    (_rAA)=  rwMat02Det(_mBB, _mBC,                                     \
                        _mCB, _mCC);                                    \
    (_rAB)= -rwMat02Det(_mAB, _mAC,                                     \
                        _mCB, _mCC);                                    \
    (_rAC)=  rwMat02Det(_mAB, _mAC,                                     \
                        _mBB, _mBC);                                    \
                                                                        \
    determinant.nReal = ( (_rAA) * (_mAA) +                             \
                          (_rAB) * (_mBA) +                             \
                          (_rAC) * (_mCA) );                            \
                                                                        \
    {                                                                   \
        const RwReal normalize = ( (determinant.nInt != 0)?             \
                                   (((RwReal)1)/determinant.nReal):     \
                                   ((RwReal)1) );                       \
        (_rAA) *= normalize;                                            \
        (_rAB) *= normalize;                                            \
        (_rAC) *= normalize;                                            \
                                                                        \
        (_rBA)= -rwMat02Det(_mBA, _mBC,                                 \
                            _mCA, _mCC) * normalize ;                   \
        (_rBB)=  rwMat02Det(_mAA, _mAC,                                 \
                            _mCA, _mCC) * normalize ;                   \
        (_rBC)= -rwMat02Det(_mAA, _mAC,                                 \
                            _mBA, _mBC) * normalize ;                   \
                                                                        \
        (_rCA)=  rwMat02Det(_mBA, _mBB,                                 \
                            _mCA, _mCB) * normalize ;                   \
        (_rCB)= -rwMat02Det(_mAA, _mAB,                                 \
                            _mCA, _mCB) * normalize ;                   \
        (_rCC)=  rwMat02Det(_mAA, _mAB,                                 \
                            _mBA, _mBB) * normalize ;                   \
    }                                                                   \
                                                                        \
}                                                                       \
MACRO_STOP

#define    rwMat04Inv(_rAA, _rAB, _rAC, _rAD,                           \
                      _rBA, _rBB, _rBC, _rBD,                           \
                      _rCA, _rCB, _rCC, _rCD,                           \
                      _rDA, _rDB, _rDC, _rDD,                           \
                      _mAA, _mAB, _mAC, _mAD,                           \
                      _mBA, _mBB, _mBC, _mBD,                           \
                      _mCA, _mCB, _mCC, _mCD,                           \
                      _mDA, _mDB, _mDC, _mDD)                           \
MACRO_START                                                             \
{                                                                       \
    RwSplitBits determinant;                                            \
                                                                        \
    (_rAA)=  rwMat03Det(_mBB, _mBC, _mBD,                               \
                        _mCB, _mCC, _mCD,                               \
                        _mDB, _mDC, _mDD);                              \
    (_rAB)= -rwMat03Det(_mAB, _mAC, _mAD,                               \
                        _mCB, _mCC, _mCD,                               \
                        _mDB, _mDC, _mDD);                              \
    (_rAC)=  rwMat03Det(_mAB, _mAC, _mAD,                               \
                        _mBB, _mBC, _mBD,                               \
                        _mDB, _mDC, _mDD);                              \
    (_rAD)= -rwMat03Det(_mAB, _mAC, _mAD,                               \
                        _mBB, _mBC, _mBD,                               \
                        _mCB, _mCC, _mCD);                              \
                                                                        \
    determinant.nReal = ( (_rAA) * (_mAA) +                             \
                          (_rAB) * (_mBA) +                             \
                          (_rAC) * (_mCA) +                             \
                          (_rAD) * (_mDA) );                            \
                                                                        \
    {                                                                   \
        const RwReal normalize = ( (determinant.nInt != 0)?             \
                                   (((RwReal)1)/determinant.nReal):     \
                                   ((RwReal)1) );                       \
                                                                        \
        (_rAA) *= normalize;                                            \
        (_rAB) *= normalize;                                            \
        (_rAC) *= normalize;                                            \
        (_rAD) *= normalize;                                            \
                                                                        \
        (_rBA)= -rwMat03Det(_mBA, _mBC, _mBD,                           \
                            _mCA, _mCC, _mCD,                           \
                            _mDA, _mDC, _mDD) * normalize ;             \
        (_rBB)=  rwMat03Det(_mAA, _mAC, _mAD,                           \
                            _mCA, _mCC, _mCD,                           \
                            _mDA, _mDC, _mDD) * normalize ;             \
        (_rBC)= -rwMat03Det(_mAA, _mAC, _mAD,                           \
                            _mBA, _mBC, _mBD,                           \
                            _mDA, _mDC, _mDD) * normalize ;             \
        (_rBD)=  rwMat03Det(_mAA, _mAC, _mAD,                           \
                            _mBA, _mBC, _mBD,                           \
                            _mCA, _mCC, _mCD) * normalize ;             \
                                                                        \
        (_rCA)=  rwMat03Det(_mBA, _mBB, _mBD,                           \
                            _mCA, _mCB, _mCD,                           \
                            _mDA, _mDB, _mDD) * normalize ;             \
        (_rCB)= -rwMat03Det(_mAA, _mAB, _mAD,                           \
                            _mCA, _mCB, _mCD,                           \
                            _mDA, _mDB, _mDD) * normalize ;             \
        (_rCC)=  rwMat03Det(_mAA, _mAB, _mAD,                           \
                            _mBA, _mBB, _mBD,                           \
                            _mDA, _mDB, _mDD) * normalize ;             \
        (_rCD)= -rwMat03Det(_mAA, _mAB, _mAD,                           \
                            _mBA, _mBB, _mBD,                           \
                            _mCA, _mCB, _mCD) * normalize ;             \
                                                                        \
        (_rDA)= -rwMat03Det(_mBA, _mBB, _mBC,                           \
                            _mCA, _mCB, _mCC,                           \
                            _mDA, _mDB, _mDC) * normalize ;             \
        (_rDB)=  rwMat03Det(_mAA, _mAB, _mAC,                           \
                            _mCA, _mCB, _mCC,                           \
                            _mDA, _mDB, _mDC) * normalize ;             \
        (_rDC)= -rwMat03Det(_mAA, _mAB, _mAC,                           \
                            _mBA, _mBB, _mBC,                           \
                            _mDA, _mDB, _mDC) * normalize ;             \
        (_rDD)=  rwMat03Det(_mAA, _mAB, _mAC,                           \
                            _mBA, _mBB, _mBC,                           \
                            _mCA, _mCB, _mCC) * normalize ;             \
    }                                                                   \
}                                                                       \
MACRO_STOP

/* RWPUBLICEND */

#define rwMatrixInitializeIdentity(m, t)        \
MACRO_START                                     \
{                                               \
    rwMatrixInitialize((m), (t));               \
    RwMatrixSetIdentity((m));                   \
}                                               \
MACRO_STOP

#endif                          /* RWMATRIX_H */
