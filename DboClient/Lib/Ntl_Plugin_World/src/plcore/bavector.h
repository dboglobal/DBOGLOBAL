
/***************************************************************************
 *                                                                         *
 * Module  : bavector.h                                                    *
 *                                                                         *
 * Purpose : Matrix handling                                               *
 *                                                                         *
 **************************************************************************/

#ifndef RWVECTOR_H
#define RWVECTOR_H

/****************************************************************************
 Includes
 */
#include "batypes.h"            /* picks up rwPI amongst others */
#include "drvmodel.h"           /* Platform dependent rwSqrtMacro() etc */
#include "bamatrix.h"
#include "bamath.h"             /*  */

/* RWPUBLIC */
/****************************************************************************
 Defines
 */

/* If sqrt is overloaded for this platform, we will remove
 * all the sqrt table stuff from the build entirely
 * currently applies to SKY2 and XBOX - IDBS [2/11/2001]
 * [and, if using the intel compiler version 400 or above,
 *  we will use the single-precision float "sqrtf" under
 *  D3D8, OpenGL or SoftRas] */
#if (defined(rwSqrtMacro))
#define RWNOSQRTTABLE
#endif /* (defined(rwSqrtMacro)) */
#if (defined(rwInvSqrtMacro))
#define RWNOINVSQRTTABLE
#endif /* (defined(rwSqrtMacro)) */

#if (!defined(rwSqrtMacro))
#define rwSqrtMacro(_root, _input) \
    ( *(_root) = _rwSqrt(_input) )
#endif /* (!defined(rwSqrtMacro)) */

#if (!defined(rwInvSqrtMacro))
#define rwInvSqrtMacro(_recip, _input) \
    ( *(_recip) = _rwInvSqrt(_input) )
#endif /* (!defined(rwInvSqrtMacro)) */

#if (!defined(rwSqrtInvSqrtMacro))
#define rwSqrtInvSqrtMacro(_root, _recip, _input)       \
MACRO_START                                             \
{                                                       \
    RwReal _tmp = _input;                               \
    rwSqrt((_root), _tmp);                              \
    rwInvSqrt((_recip), _tmp);                          \
}                                                       \
MACRO_STOP
#endif /* (!defined(rwSqrtInvSqrtMacro)) */

/* Vector operations Macros */

#if (!defined(RwV2dAssignMacro))
#define RwV2dAssignMacro(_target, _source)                      \
    ( *(_target) = *(_source) )
#endif /* (!defined(RwV2dAssignMacro)) */

#define RwV2dAddMacro(o, a, b)                                  \
MACRO_START                                                     \
{                                                               \
    (o)->x = (((a)->x) + ( (b)->x));                            \
    (o)->y = (((a)->y) + ( (b)->y));                            \
}                                                               \
MACRO_STOP

#define RwV2dSubMacro(o, a, b)                                  \
MACRO_START                                                     \
{                                                               \
    (o)->x = (((a)->x) - ( (b)->x));                            \
    (o)->y = (((a)->y) - ( (b)->y));                            \
}                                                               \
MACRO_STOP

#define RwV2dScaleMacro(o, i, s)                                \
MACRO_START                                                     \
{                                                               \
    (o)->x = (((i)->x) * ( (s)));                               \
    (o)->y = (((i)->y) * ( (s)));                               \
}                                                               \
MACRO_STOP

#define RwV2dDotProductMacro(a,b)                               \
    (( ((((a)->x) * ( (b)->x))) +                               \
      ( (((a)->y) * ( (b)->y)))))

#define _rwV2dNormalizeMacro(_result, _out, _in)                \
MACRO_START                                                     \
{                                                               \
    RwReal length2 = RwV2dDotProductMacro((_in), (_in));        \
    rwInvSqrtMacro(&(_result), length2);                        \
    RwV2dScaleMacro((_out), (_in), (_result));                  \
}                                                               \
MACRO_STOP

#define RwV2dNormalizeMacro(_result, _out, _in)                 \
MACRO_START                                                     \
{                                                               \
    RwReal length2 = RwV2dDotProductMacro((_in), (_in));        \
    RwReal recip;                                               \
                                                                \
    rwSqrtInvSqrtMacro(&(_result), &recip, length2);            \
    RwV2dScaleMacro((_out), (_in), recip);                      \
}                                                               \
MACRO_STOP

#define RwV2dLengthMacro(_result, _in)                          \
MACRO_START                                                     \
{                                                               \
    (_result) = RwV2dDotProductMacro(_in, _in);                 \
    rwSqrtMacro(&(_result), (_result));                         \
}                                                               \
MACRO_STOP

#define RwV2dLineNormalMacro(_o, _a, _b)                        \
MACRO_START                                                     \
{                                                               \
    RwReal recip;                                               \
                                                                \
    (_o)->y = (((_b)->x) - ( (_a)->x));                         \
    (_o)->x = (((_a)->y) - ( (_b)->y));                         \
    _rwV2dNormalizeMacro(recip, _o,_o);                         \
}                                                               \
MACRO_STOP

#define RwV2dPerpMacro(o, a)                                    \
MACRO_START                                                     \
{                                                               \
    (o)->x = -(a)->y;                                           \
    (o)->y = (a)->x;                                            \
}                                                               \
MACRO_STOP

/* RwV3d */

#if (!defined(RwV3dAssignMacro))
#define RwV3dAssignMacro(_target, _source)                     \
    ( *(_target) = *(_source) )
#endif /* (!defined(RwV3dAssignMacro)) */


#define RwV3dAddMacro(o, a, b)                                  \
MACRO_START                                                     \
{                                                               \
    (o)->x = (((a)->x) + ( (b)->x));                            \
    (o)->y = (((a)->y) + ( (b)->y));                            \
    (o)->z = (((a)->z) + ( (b)->z));                            \
}                                                               \
MACRO_STOP

#define RwV3dSubMacro(o, a, b)                                  \
MACRO_START                                                     \
{                                                               \
    (o)->x = (((a)->x) - ( (b)->x));                            \
    (o)->y = (((a)->y) - ( (b)->y));                            \
    (o)->z = (((a)->z) - ( (b)->z));                            \
}                                                               \
MACRO_STOP

#define RwV3dScaleMacro(o, a, s)                                \
MACRO_START                                                     \
{                                                               \
    (o)->x = (((a)->x) * ( (s)));                               \
    (o)->y = (((a)->y) * ( (s)));                               \
    (o)->z = (((a)->z) * ( (s)));                               \
}                                                               \
MACRO_STOP

#define RwV3dIncrementScaledMacro(o, a, s)                      \
MACRO_START                                                     \
{                                                               \
    (o)->x += (((a)->x) * ( (s)));                              \
    (o)->y += (((a)->y) * ( (s)));                              \
    (o)->z += (((a)->z) * ( (s)));                              \
}                                                               \
MACRO_STOP

#define RwV3dNegateMacro(o, a)                                  \
MACRO_START                                                     \
{                                                               \
    (o)->x = -(a)->x;                                           \
    (o)->y = -(a)->y;                                           \
    (o)->z = -(a)->z;                                           \
}                                                               \
MACRO_STOP

#define RwV3dDotProductMacro(a, b)                              \
    ((((( (((a)->x) * ((b)->x))) +                              \
        ( (((a)->y) * ((b)->y))))) +                            \
        ( (((a)->z) * ((b)->z)))))

#define RwV3dCrossProductMacro(o, a, b)                         \
MACRO_START                                                     \
{                                                               \
    (o)->x =                                                    \
        (( (((a)->y) * ( (b)->z))) -                            \
         ( (((a)->z) * ( (b)->y))));                            \
    (o)->y =                                                    \
        (( (((a)->z) * ( (b)->x))) -                            \
         ( (((a)->x) * ( (b)->z))));                            \
    (o)->z =                                                    \
        (( (((a)->x) * ( (b)->y))) -                            \
         ( (((a)->y) * ( (b)->x))));                            \
}                                                               \
MACRO_STOP

#define _rwV3dNormalizeMacro(_result, _out, _in)                \
MACRO_START                                                     \
{                                                               \
    RwReal length2 = RwV3dDotProductMacro(_in, _in);            \
    rwInvSqrtMacro(&(_result), length2);                        \
    RwV3dScaleMacro(_out, _in, _result);                        \
}                                                               \
MACRO_STOP

#define RwV3dNormalizeMacro(_result, _out, _in)                 \
MACRO_START                                                     \
{                                                               \
    RwReal length2 = RwV3dDotProductMacro((_in), (_in));        \
    RwReal recip;                                               \
                                                                \
    rwSqrtInvSqrtMacro(&(_result), &recip, length2);            \
    RwV3dScaleMacro((_out), (_in), recip);                      \
}                                                               \
MACRO_STOP

#define RwV3dLengthMacro(_result, _in)                          \
MACRO_START                                                     \
{                                                               \
    (_result) = RwV3dDotProductMacro(_in, _in);                 \
    rwSqrtMacro(&(_result), _result);                           \
}                                                               \
MACRO_STOP

#if (! ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ))

#define RwV2dAssign(o, a)               RwV2dAssignMacro(o, a)
#define RwV2dAdd(o, a, b)               RwV2dAddMacro(o, a, b)
#define RwV2dSub(o, a, b)               RwV2dSubMacro(o, a, b)
#define RwV2dLineNormal(_o, _a, _b)     RwV2dLineNormalMacro(_o, _a, _b)
#define RwV2dScale(o, i, s)             RwV2dScaleMacro(o, i, s)
#define RwV2dDotProduct(a,b)            RwV2dDotProductMacro(a,b)
#define RwV2dPerp(o, a)                 RwV2dPerpMacro(o, a)
#define RwV3dAssign(o, a)               RwV3dAssignMacro(o, a)
#define RwV3dAdd(o, a, b)               RwV3dAddMacro(o, a, b)
#define RwV3dSub(o, a, b)               RwV3dSubMacro(o, a, b)
#define RwV3dScale(o, a, s)             RwV3dScaleMacro(o, a, s)
#define RwV3dIncrementScaled(o, a, s)   RwV3dIncrementScaledMacro(o, a, s)
#define RwV3dNegate(o, a)               RwV3dNegateMacro(o, a)
#define RwV3dDotProduct(a, b)           RwV3dDotProductMacro(a, b)
#define RwV3dCrossProduct(o, a, b)      RwV3dCrossProductMacro(o, a, b)

#endif /* (! ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )) */

#define RWRAD2DEG(_x) ((_x) * (((RwReal)180)/(rwPI)))

#if (!defined(rw4OVERPISQ))
#define rw4OVERPISQ  ( ((RwReal)4) / ( rwPI * rwPI ))
#endif /* (!defined(rw4OVERPISQ)) */

#if (!defined(rwPI3))
#define rwPI3  (rwPI * (RwReal)3)
#endif /* (!defined(rwPI3)) */

#if (!defined(rwPI3OVER2))
#define rwPI3OVER2  ( rwPI3 / (RwReal)2 )
#endif /* (!defined(rwPI3OVER2)) */

#if (!defined(rwPI3OVER8))
#define rwPI3OVER8  (rwPI3 / (RwReal)8 )
#endif /* (!defined(rwPI3OVER8)) */

#define RwQuadSin(_x)                                                       \
    ( rw4OVERPISQ *                                                         \
      ( ( (_x) < 0 ) ?                                                      \
        ( ( rwPI + (_x) ) * (_x) ) :                                        \
        ( ( rwPI - (_x) ) * (_x) ) ) )

#define RwQuadASin(_result, _s)                                             \
    ( rwPIOVER2 * ( ((_s)<0) ?                                              \
                    ( rwSqrtMacro((_result), 1.0f + (_s)) - 1 ) :           \
                    ( 1 - rwSqrtMacro((_result), 1.0f - (_s)) ) ) )

#define RwQuadCos(_x)                                                       \
    ( rw4OVERPISQ *                                                         \
      ( ( (_x) < -rwPIOVER2 ) ?                                             \
        ( ( -rwPI3OVER2 - (_x) ) * ( -rwPIOVER2 - (_x) ) ) :                \
        ( ( (_x) < rwPIOVER2) ?                                             \
          ( ( rwPIOVER2 + (_x) ) * ( rwPIOVER2 - (_x) ) ) :                 \
          ( ( rwPIOVER2 - (_x) ) * ( rwPI3OVER2 - (_x) ) ) ) ) )

#define RwQuadACos(_result, _c)                                             \
    ( rwPIOVER2 * ( ((_c)<0) ?                                              \
                    (2.0f - rwSqrtMacro((_result), 1.0f + (_c))):           \
                    rwSqrtMacro((_result), 1.0f - (_c))) )

#define RwQuadTan(_x)                                                       \
    ( rwPI3 * (_x) / ( rwPI * rwPI - (_x) * (_x) * 4.0f ) )

#define RwQuadATan(_result, _t)                                             \
    ( ( rwSqrtMacro((_result), (rwPI3OVER8 * rwPI3OVER8) +                  \
                    (_t) * (_t) * (rwPIOVER2 * rwPIOVER2) ) - rwPI3OVER8 )  \
      / ( _t) )

#define RwQuadATan2(_result, _s, _c)                                        \
    ( ( rwSqrtMacro((_result), (_c) * (_c) * (rwPI3OVER8 * rwPI3OVER8) +    \
                    (_s) * (_s) * (rwPIOVER2 * rwPIOVER2) )                 \
        - (_c) * rwPI3OVER8 ) / ( _s)  )

/****************************************************************************
 Global Types
 */
#if !defined(RWNOVECMULTFUNCS)
/*
 * Typedef for pointer to Vector multiplication by Matrix function
 */
typedef RwV3d *(*rwVectorMultFn) (RwV3d * pointOut,
                                  const RwV3d * pointIn,
                                  const RwMatrix * matrix);

typedef RwV3d *(*rwVectorsMultFn) (RwV3d * pointsOut,
                                   const RwV3d * pointsIn,
                                   RwInt32 numPoints,
                                   const RwMatrix * matrix);
#endif /* !defined(RWNOVECMULTFUNCS) */
/* RWPUBLICEND */

typedef struct rwVectorGlobals rwVectorGlobals;
struct rwVectorGlobals
{
    RwSplitBits *SqrtTab;
    RwSplitBits *InvSqrtTab;

#if !defined(RWNOVECMULTFUNCS)
    rwVectorMultFn  multPoint;
    rwVectorsMultFn multPoints;
    rwVectorMultFn  multVector;
    rwVectorsMultFn multVectors;
#endif /* !defined(RWNOVECMULTFUNCS) */
};

/* RWPUBLIC */
#ifdef    __cplusplus
extern "C"
{
#endif         /* __cplusplus */

/* Other useful stuff */

/****************************************************************************
 Function prototypes
 */
extern RwReal RwV3dNormalize(RwV3d * out, const RwV3d * in);
extern RwReal RwV3dLength(const RwV3d * in);

extern RwReal RwV2dLength(const RwV2d * in);
extern RwReal RwV2dNormalize(RwV2d * out, const RwV2d * in);

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )

extern void RwV2dAssign(RwV2d * out,
                        const RwV2d * ina);
extern void RwV2dAdd(RwV2d * out,
                     const RwV2d * ina, const RwV2d * inb);
extern void RwV2dLineNormal(RwV2d * out,
                     const RwV2d * ina, const RwV2d * inb);
extern void RwV2dSub(RwV2d * out,
                     const RwV2d * ina, const RwV2d * inb);
extern void RwV2dPerp(RwV2d * out, const RwV2d * in);
extern void RwV2dScale(RwV2d * out,
                       const RwV2d * in, RwReal scalar);
extern RwReal RwV2dDotProduct(const RwV2d * ina, const RwV2d * inb);

extern void RwV3dAssign(RwV3d * out,
                        const RwV3d * ina);
extern void RwV3dAdd(RwV3d * out,
                     const RwV3d * ina, const RwV3d * inb);
extern void RwV3dSubMacro(RwV3d * out,
                     const RwV3d * ina, const RwV3d * inb);
extern void RwV3dScale(RwV3d * out,
                       const RwV3d * in, RwReal scalar);
extern void RwV3dIncrementScaled(RwV3d * out,
                                 const RwV3d * in, RwReal scalar);
extern void RwV3dNegate(RwV3d * out, const RwV3d * in);
extern RwReal RwV3dDotProduct(const RwV3d * ina, const RwV3d * inb);
extern void RwV3dCrossProduct(RwV3d * out,
                              const RwV3d * ina, const RwV3d * inb);

#endif         /* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */

/* Transform points/vectors */
extern RwV3d *RwV3dTransformPoint(RwV3d * pointOut,
                                  const RwV3d * pointIn,
                                  const RwMatrix * matrix);
extern RwV3d *RwV3dTransformPoints(RwV3d * pointsOut,
                                   const RwV3d * pointsIn,
                                   RwInt32 numPoints,
                                   const RwMatrix * matrix);
extern RwV3d *RwV3dTransformVector(RwV3d * vectorOut,
                                   const RwV3d * vectorIn,
                                   const RwMatrix * matrix);
extern RwV3d *RwV3dTransformVectors(RwV3d * vectorsOut,
                                    const RwV3d * vectorsIn,
                                    RwInt32 numPoints,
                                    const RwMatrix * matrix);

/* SPI */

#if (!defined(RWNOSQRTTABLE))
extern RwReal _rwSqrt(const RwReal num);
#endif /* (!defined(RWNOSQRTTABLE)) */
#if (!defined(RWNOINVSQRTTABLE))
extern RwReal _rwInvSqrt(const RwReal num);
#endif /* (!defined(RWNOINVSQRTTABLE)) */

extern RwReal _rwV3dNormalize(RwV3d * out, const RwV3d * in);

/* RWPUBLICEND */

/* Opening and closing */
extern void *_rwVectorOpen(void *instance,
                           RwInt32 offset, RwInt32 size);
extern void *_rwVectorClose(void *instance,
                            RwInt32 offset, RwInt32 size);

#if !defined(RWNOVECMULTFUNCS)
extern RwBool _rwVectorSetMultFn(rwVectorMultFn multPoint,
                                 rwVectorsMultFn multPoints,
                                 rwVectorMultFn multVector,
                                 rwVectorsMultFn multVectors);
#endif /* !defined(RWNOVECMULTFUNCS) */

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif         /* __cplusplus */
/* RWPUBLICEND */

#endif         /* RWVECTOR_H */
