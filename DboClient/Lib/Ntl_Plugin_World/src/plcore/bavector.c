/*
 * 2D / 3D Vector maths.
 * Performs all Vector based maths within library
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/***************************************************************************
 *                                                                         *
 * Module  : bavector.c                                                    *
 *                                                                         *
 * Purpose : Vector handling                                               *
 *                                                                         *
 **************************************************************************/

/**
 * \ingroup rwv3d
 * \page rwv3doverview RwV3d Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection v3doverview Overview
 *
 * This is RenderWare's 3D vector object.
 *
 * The API covers the 3D vector basics, including: addition, subtraction,
 * cross and dot products, length determination, negation, normalization
 * and more.
 * It is primarily used in the context of RenderWare's 3D
 * Immediate Mode API \ref rwim3d
 * and the scene graph API \ref scenemanagement .
 * It is also used to represent a 3D coordinate
 * or point in other parts of the API, such as \ref RwBBox.
 *
*/

/**
 * \ingroup rwv2d
 * \page rwv2doverview RwV2d Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection v2doverview Overview
 *
 * This is RenderWare's 2D vector object.
 *
 * RwV2d's API covers most 2D vector needs, including: addition, dot product,
 * length, assignment, normalization, scaling and subtraction. The object is
 * primarily used in the context of RenderWare's 2D Immediate Mode API
 * \ref rwim2d .
 * It is also used to represent a 2D coordinate or point in other parts
 * of the API.
 *
 */


/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"

/*
 * "bapipe.h" requires
 * typedef struct _RwTexture RwTexture
 * from  "batextur.h"
 */
#include "bacamera.h"
#include "batextur.h"
#include "bapipe.h"
#include "baresour.h"
#include "bamath.h"
#include "rpcriter.h"
#include "bavector.h"

/****************************************************************************
 Local Defines
 */

#define rwSQRTTABLEPRECISION           11L /* bits of precision */
#define rwSQRTTABLESIZE                (1L << rwSQRTTABLEPRECISION)
#define rwSQRTTABLESHIFT               (23L - rwSQRTTABLEPRECISION)
#define rwSQRTTABLEBYTES               (sizeof(RwUInt32) << (rwSQRTTABLEPRECISION + 1))

#define RWVECTORGLOBAL(var) (RWPLUGINOFFSET(rwVectorGlobals, RwEngineInstance, vectorModule.globalsOffset)->var)

/*
 * RwV3dTransformPointMacro() can be overloaded per platform in
 * rwsdk/driver/$(RWTARGET)/drvmodel.h
 * The default/generic implementation is below.
 */
#if (!defined(RwV3dTransformPointMacro))
#define RwV3dTransformPointMacro(_pntsOut, _pntsIn, _matrix)                \
MACRO_START                                                                 \
{                                                                           \
    const RwV3d     *__pntsIn = (_pntsIn);                                  \
    const RwMatrix  *__matrix = (_matrix);                                  \
                                                                            \
    RWVECTORGLOBAL(multPoint)(_pntsOut, __pntsIn, __matrix);                \
}                                                                           \
MACRO_STOP
#endif /* (!defined(RwV3dTransformPointsMacro)) */

/*
 * RwV3dTransformPointsMacro() can be overloaded per platform in
 * rwsdk/driver/$(RWTARGET)/drvmodel.h
 * The default/generic implementation is below.
 */
#if (!defined(RwV3dTransformPointsMacro))
#define RwV3dTransformPointsMacro(_pntsOut, _pntsIn, _count, _matrix)       \
MACRO_START                                                                 \
{                                                                           \
    const RwV3d     *__pntsIn = (_pntsIn);                                  \
    const RwMatrix  *__matrix = (_matrix);                                  \
                                                                            \
    RWVECTORGLOBAL(multPoints)(_pntsOut, __pntsIn, _count, __matrix);       \
}                                                                           \
MACRO_STOP
#endif /* (!defined(RwV3dTransformPointsMacro)) */

/*
 * RwV3dTransformVectorMacro() can be overloaded per platform in
 * rwsdk/driver/$(RWTARGET)/drvmodel.h
 * The default/generic implementation is below.
 */
#if (!defined(RwV3dTransformVectorMacro))
#define RwV3dTransformVectorMacro(_vecsOut, _vecsIn, _matrix)               \
MACRO_START                                                                 \
{                                                                           \
    const RwV3d     *__vecsIn = (_vecsIn);                                  \
    const RwMatrix  *__matrix = (_matrix);                                  \
                                                                            \
    RWVECTORGLOBAL(multVector)(_vecsOut, __vecsIn, __matrix);               \
}                                                                           \
MACRO_STOP
#endif /* (!defined(RwV3dTransformVectorMacro)) */

/*
 * RwV3dTransformVectorsMacro() can be overloaded per platform in
 * rwsdk/driver/$(RWTARGET)/drvmodel.h
 * The default/generic implementation is below.
 */
#if (!defined(RwV3dTransformVectorsMacro))
#define RwV3dTransformVectorsMacro(_vecsOut, _vecsIn, _count, _matrix)      \
MACRO_START                                                                 \
{                                                                           \
    const RwV3d     *__vecsIn = (_vecsIn);                                  \
    const RwMatrix  *__matrix = (_matrix);                                  \
                                                                            \
    RWVECTORGLOBAL(multVectors)(_vecsOut, __vecsIn, _count, __matrix);      \
}                                                                           \
MACRO_STOP
#endif /* (!defined(RwV3dTransformVectorsMacro)) */

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

 /****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/* This is the same for all instances */
static RwModuleInfo vectorModule;


#if (!defined(RWNOSQRTTABLE))

/****************************************************************************
 SqrtTableDestroy
 */
static void
SqrtTableDestroy(void)
{
    RWFUNCTION(RWSTRING("SqrtTableDestroy"));

    if (NULL != RWVECTORGLOBAL(SqrtTab))
    {
        RwFree(RWVECTORGLOBAL(SqrtTab));
        RWVECTORGLOBAL(SqrtTab) = (RwSplitBits *)NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 SqrtTableCreate
 */
static              RwBool
SqrtTableCreate(void)
{
    RwUInt32            i;
    RwSplitBits        *SqrtTab2to4;
    RwSplitBits        *SqrtTab1to2;
    RwSplitBits         spIn, spOut;

    RWFUNCTION(RWSTRING("SqrtTableCreate"));

    /*
     * X^0.5
     */

    SqrtTab2to4 = (RwSplitBits *) RwMalloc(rwSQRTTABLEBYTES,
        rwID_VECTORMODULE | rwMEMHINTDUR_GLOBAL);
    if (NULL == SqrtTab2to4)
    {
        RWERROR((E_RW_NOMEM, rwSQRTTABLEBYTES));
        RWRETURN(FALSE);
    }

    SqrtTab1to2 = &SqrtTab2to4[rwSQRTTABLESIZE];

    spIn.nReal = ((RwReal)1);

    for (i = 0; i < rwSQRTTABLESIZE; i++)
    {
#if (defined(RW_USE_SPF))
        spOut.nReal = (RwReal) sqrtf(spIn.nReal);
#else /* (defined(RW_USE_SPF)) */
        spOut.nReal = (RwReal) sqrt(spIn.nReal);
#endif /* (defined(RW_USE_SPF)) */

        spOut.nUInt = spOut.nUInt + 0x00400000UL - 0x20000000UL;
        SqrtTab1to2[i].nUInt = spOut.nUInt;
        spIn.nUInt += (1L << rwSQRTTABLESHIFT);
    }

    for (i = 0; i < rwSQRTTABLESIZE; i++)
    {
#if (defined(RW_USE_SPF))
        spOut.nReal = (RwReal) sqrtf(spIn.nReal);
#else /* (defined(RW_USE_SPF)) */
        spOut.nReal = (RwReal) sqrt(spIn.nReal);
#endif /* (defined(RW_USE_SPF)) */

        spOut.nUInt = spOut.nUInt - 0x20000000UL;
        SqrtTab2to4[i].nUInt = spOut.nUInt;
        spIn.nUInt += (1L << rwSQRTTABLESHIFT);
    }

    RWVECTORGLOBAL(SqrtTab) = SqrtTab2to4;

    RWRETURN(TRUE);
}

#endif /* (!defined(RWNOSQRTTABLE)) */

#if (!defined(RWNOINVSQRTTABLE))

/****************************************************************************
 InvSqrtTableDestroy
 */
static void
InvSqrtTableDestroy(void)
{
    RWFUNCTION(RWSTRING("InvSqrtTableDestroy"));

    if (NULL != RWVECTORGLOBAL(InvSqrtTab))
    {
        RwFree(RWVECTORGLOBAL(InvSqrtTab));
        RWVECTORGLOBAL(InvSqrtTab) = (RwSplitBits *)NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 InvSqrtTableCreate
 */
static              RwBool
InvSqrtTableCreate(void)
{
    RwUInt32            i;
    RwSplitBits        *InvSqrtTab1to2;
    RwSplitBits        *InvSqrtTab2to4;
    RwSplitBits         spIn, spOut;

    RWFUNCTION(RWSTRING("InvSqrtTableCreate"));

    /*
     * X^-0.5
     */

    InvSqrtTab2to4 = (RwSplitBits *) RwMalloc(rwSQRTTABLEBYTES,
        rwID_VECTORMODULE | rwMEMHINTDUR_GLOBAL);
    if (NULL == InvSqrtTab2to4)
    {
        RWERROR((E_RW_NOMEM, rwSQRTTABLEBYTES));
        RWRETURN(FALSE);
    }

    InvSqrtTab1to2 = &InvSqrtTab2to4[rwSQRTTABLESIZE];

    spIn.nReal = ((RwReal)1);
    for (i = 0; i < rwSQRTTABLESIZE; i++)
    {
#if (defined(RW_USE_SPF))
        spOut.nReal = (RwReal) (1.0f / sqrtf(spIn.nReal));
#else /* (defined(RW_USE_SPF)) */
        spOut.nReal = (RwReal) (1.0f / sqrt(spIn.nReal));
#endif /* (defined(RW_USE_SPF)) */

        spOut.nUInt = spOut.nUInt - 0x20000000UL;
        InvSqrtTab1to2[i].nUInt = spOut.nUInt;
        spIn.nUInt += (1L << rwSQRTTABLESHIFT);
    }

    for (i = 0; i < rwSQRTTABLESIZE; i++)
    {
#if (defined(RW_USE_SPF))
        spOut.nReal = (RwReal) (1.0f / sqrtf(spIn.nReal));
#else /* (defined(RW_USE_SPF)) */
        spOut.nReal = (RwReal) (1.0f / sqrt(spIn.nReal));
#endif /* (defined(RW_USE_SPF)) */

        spOut.nUInt = spOut.nUInt + 0x00400000UL - 0x20000000UL;
        InvSqrtTab2to4[i].nUInt = spOut.nUInt;
        spIn.nUInt += (1L << rwSQRTTABLESHIFT);
    }

    RWVECTORGLOBAL(InvSqrtTab) = InvSqrtTab2to4;

    RWRETURN(TRUE);
}

#endif /* (!defined(RWNOINVSQRTTABLE)) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening/Closing

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if !defined(RWNOVECMULTFUNCS)
/****************************************************************************
 VectorMultPoints

 On entry   : out array, in array, num of points, matrix
 On exit    : out array of success
 */

static RwV3d *
VectorMultPoints(RwV3d *pointsOut,
                 const RwV3d *pointsIn,
                 RwInt32 numPoints,
                 const RwMatrix *matrix)
{
    RwV3d   *cachedOut = pointsOut;

    RWFUNCTION(RWSTRING("VectorMultPoints"));
    RWASSERT(pointsOut);
    RWASSERT(pointsIn);
    RWASSERT(matrix);

    while (--numPoints >= 0)
    {
        RwReal              scale;
        RwReal              imageX, imageY, imageZ;

        scale = pointsIn->x;   /* X */
        imageX = ((scale) * (matrix->right.x));
        imageY = ((scale) * (matrix->right.y));
        imageZ = ((scale) * (matrix->right.z));

        scale = pointsIn->y;   /* Y */
        imageX = ((imageX) + (((scale) * (matrix->up.x))));
        imageY = ((imageY) + (((scale) * (matrix->up.y))));
        imageZ = ((imageZ) + (((scale) * (matrix->up.z))));

        scale = pointsIn->z;   /* Z */
        imageX = ((imageX) + (((scale) * (matrix->at.x))));
        imageY = ((imageY) + (((scale) * (matrix->at.y))));
        imageZ = ((imageZ) + (((scale) * (matrix->at.z))));

        /* W */
        pointsOut->x = ((imageX) + (matrix->pos.x));
        pointsOut->y = ((imageY) + (matrix->pos.y));
        pointsOut->z = ((imageZ) + (matrix->pos.z));

        /* Next point */
        pointsIn++;
        pointsOut++;
    }

    RWRETURN(cachedOut);
}

/****************************************************************************
 VectorMultPoint

 On entry   : out point, in point, matrix
 On exit    : out point on success
 */

static RwV3d *
VectorMultPoint(RwV3d *pointOut,
                const RwV3d *pointIn,
                const RwMatrix *matrix)
{
    RwReal  scale;
    RwReal  imageX;
    RwReal  imageY;
    RwReal  imageZ;

    RWFUNCTION(RWSTRING("VectorMultPoint"));
    RWASSERT(pointOut);
    RWASSERT(pointIn);
    RWASSERT(matrix);

    scale = pointIn->x;   /* X */
    imageX = ((scale) * (matrix->right.x));
    imageY = ((scale) * (matrix->right.y));
    imageZ = ((scale) * (matrix->right.z));

    scale = pointIn->y;   /* Y */
    imageX = ((imageX) + (((scale) * (matrix->up.x))));
    imageY = ((imageY) + (((scale) * (matrix->up.y))));
    imageZ = ((imageZ) + (((scale) * (matrix->up.z))));

    scale = pointIn->z;   /* Z */
    imageX = ((imageX) + (((scale) * (matrix->at.x))));
    imageY = ((imageY) + (((scale) * (matrix->at.y))));
    imageZ = ((imageZ) + (((scale) * (matrix->at.z))));

    /* W */
    pointOut->x = ((imageX) + (matrix->pos.x));
    pointOut->y = ((imageY) + (matrix->pos.y));
    pointOut->z = ((imageZ) + (matrix->pos.z));

    RWRETURN(pointOut);
}

/****************************************************************************
 VectorMultVectors

 On entry   : out array, in array, num of points, matrix
 On exit    : point on success
 */
static RwV3d *
VectorMultVectors(RwV3d *pointsOut,
                  const RwV3d *pointsIn,
                  RwInt32 numPoints,
                  const RwMatrix *matrix)
{
    RwV3d   *cachedOut = pointsOut;

    RWFUNCTION(RWSTRING("VectorMultVectors"));
    RWASSERT(pointsOut);
    RWASSERT(pointsIn);
    RWASSERT(matrix);

    while (--numPoints >= 0)
    {
        RwReal              scale;
        RwReal              imageX, imageY, imageZ;

        scale = pointsIn->x;   /* X */
        imageX = ((scale) * (matrix->right.x));
        imageY = ((scale) * (matrix->right.y));
        imageZ = ((scale) * (matrix->right.z));

        scale = pointsIn->y;   /* Y */
        imageX = ((imageX) + (((scale) * (matrix->up.x))));
        imageY = ((imageY) + (((scale) * (matrix->up.y))));
        imageZ = ((imageZ) + (((scale) * (matrix->up.z))));

        scale = pointsIn->z;   /* Z */
        pointsOut->x = ((((scale) * (matrix->at.x))) + (imageX));
        pointsOut->y = ((((scale) * (matrix->at.y))) + (imageY));
        pointsOut->z = ((((scale) * (matrix->at.z))) + (imageZ));

        /* Next vector */
        pointsIn++;
        pointsOut++;
    }

    RWRETURN(cachedOut);
}

/****************************************************************************
 VectorMultVector

 On entry   : out point, in point, matrix
 On exit    : point on success
 */
static RwV3d *
VectorMultVector(RwV3d *pointOut,
                 const RwV3d *pointIn,
                 const RwMatrix *matrix)
{
    RwReal  scale;
    RwReal  imageX;
    RwReal  imageY;
    RwReal  imageZ;

    RWFUNCTION(RWSTRING("VectorMultVector"));
    RWASSERT(pointOut);
    RWASSERT(pointIn);
    RWASSERT(matrix);

    scale = pointIn->x;   /* X */
    imageX = ((scale) * (matrix->right.x));
    imageY = ((scale) * (matrix->right.y));
    imageZ = ((scale) * (matrix->right.z));

    scale = pointIn->y;   /* Y */
    imageX = ((imageX) + (((scale) * (matrix->up.x))));
    imageY = ((imageY) + (((scale) * (matrix->up.y))));
    imageZ = ((imageZ) + (((scale) * (matrix->up.z))));

    scale = pointIn->z;   /* Z */
    pointOut->x = ((((scale) * (matrix->at.x))) + (imageX));
    pointOut->y = ((((scale) * (matrix->at.y))) + (imageY));
    pointOut->z = ((((scale) * (matrix->at.z))) + (imageZ));

    RWRETURN(pointOut);
}

/****************************************************************************
 _rwVectorSetMultFn

 On entry   : Vector for multiplying a points (with translation)
              Vector for multiplying points (with translation)
              Vector for multiplying a vector (without translation)
              Vector for multiplying vectors (without translation)
 On exit    : point on success
 */
RwBool
_rwVectorSetMultFn(rwVectorMultFn multPoint,
                   rwVectorsMultFn multPoints,
                   rwVectorMultFn multVector,
                   rwVectorsMultFn multVectors)
{
    RWFUNCTION(RWSTRING("_rwVectorSetMultFn"));

    if (NULL == multPoint)
    {
        multPoint = VectorMultPoint;
    }

    RWVECTORGLOBAL(multPoint) = multPoint;

    if (NULL == multPoints)
    {
        multPoints = VectorMultPoints;
    }

    RWVECTORGLOBAL(multPoints) = multPoints;

    if (NULL == multVector)
    {
        multVector = VectorMultVector;
    }

    RWVECTORGLOBAL(multVector) = multVector;

    if (NULL == multVectors)
    {
        multVectors = VectorMultVectors;
    }

    RWVECTORGLOBAL(multVectors) = multVectors;

    RWRETURN(TRUE);
}
#endif /* !defined(RWNOVECMULTFUNCS) */

RwReal
_rwV3dNormalize(RwV3d * out, const RwV3d * in)
{

    RwReal              recip;
    RWFUNCTION(RWSTRING("_rwV3dNormalize"));

    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(in);

    _rwV3dNormalizeMacro(recip, out, in);

    RWRETURN(recip);
}

/**
 * \ingroup rwv3d
 * \ref RwV3dNormalize is used to normalize the specified 3D vector. The
 * input and output arguments can point to the same 3D vector.
 *
 * \note It is an error to try to normalize a vector whose length is zero.
 * If this condition is detected \ref RwV3dNormalize places a vector with
 * components (0,0,0) in the output argument and returns 0.0.
 *
 * \note This function uses an efficient customized procedure for
 * calculating square roots, which will be accurate enough for most input
 * vectors. However, application developers should be aware that the accuracy
 * of the results produced from \ref RwV3dNormalize may become unacceptable for
 * vectors of very small magnitude.
 *
 * \param out  A pointer to a 3D vector that will receive the normalized vector
 * \param in   A pointer to the 3D vector
 *
 * \return Returns an \ref RwReal value equal to the original length of the vector
 *        if successful or zero if there is an error
 *
 * \see RwV3dLength
 *
 */
RwReal
RwV3dNormalize(RwV3d *out, const RwV3d *in)
{
    RwReal              length = ((RwReal)0);
    RWAPIFUNCTION(RWSTRING("RwV3dNormalize"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(in);

    RwV3dNormalizeMacro(length, out, in);

    if (length <= (RwReal) ((0)))
    {
        RWERROR((E_RW_ZEROLENGTH));
    }

    RWRETURN(length);
}

/**
 * \ingroup rwv3d
 * \ref RwV3dLength is used to calculate the length of the specified 3D
 * vector.
 *
 * \note This function uses an efficient customized procedure for
 * calculating square roots, which will be accurate enough for most input
 * vectors. However, application developers should be aware that the accuracy
 * of the results produced from RwV3dLength may become unacceptable for
 * vectors of very small magnitude.
 *
 * \param in  A pointer to the 3D vector
 *
 * \return Returns an \ref RwReal value equal to the vector's length if successful
 *         or zero if there is an error
 *
 * \see RwV3dNormalize
 *
 */
RwReal
RwV3dLength(const RwV3d * in)
{
    RwReal length = ((RwReal)0);
    RWAPIFUNCTION(RWSTRING("RwV3dLength"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(in);

    RwV3dLengthMacro(length, in);
    RWRETURN(length);
}

#if (!defined(RWNOSQRTTABLE))

/****************************************************************************
 _rwSqrt
 */
RwReal
_rwSqrt(const RwReal num)
{
    RwSplitBits         result;

    RWFUNCTION(RWSTRING("_rwSqrt"));

    RWASSERT(vectorModule.numInstances);
    RWASSERT(num >= ((RwReal)0));

    /* Assumes IEEE floating point layout:
     * 3         2         1          0
     * 10987654321098765432109876543210
     * SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMM
     * |   |               |
     * |   |               23 bits of mantissa
     * |   8 bits of result
     * 1 bit of sign
     */

    result.nReal = num;

    if (result.nUInt)
    {
        const RwSplitBits  *const SqrtTab = ((rwVectorGlobals *)
                                             & (((RwUInt8 *) RwEngineInstance)[vectorModule.globalsOffset]))->SqrtTab;

        /* correction factor */
        result.nUInt += (1L << (22 - rwSQRTTABLEPRECISION));
        RWASSERT(((result.nUInt & 0x007fffffUL) >> rwSQRTTABLESHIFT) < rwSQRTTABLESIZE);

        result.nUInt =
            SqrtTab[(result.nUInt & 0x00ffffffUL) >> rwSQRTTABLESHIFT].nUInt +
            ((0x7f800000UL & result.nUInt) >> 1);
    }

    RWRETURN(result.nReal);
}

#endif /* (!defined(RWNOSQRTTABLE)) */

#if (!defined(RWNOINVSQRTTABLE))

/****************************************************************************
 _rwRInvSqrt
 */
RwReal
_rwInvSqrt(const RwReal num)
{
    RwSplitBits         result;

    RWFUNCTION(RWSTRING("_rwInvSqrt"));

    RWASSERT(vectorModule.numInstances);
    RWASSERT(num >= ((RwReal)0));

    /* Assumes IEEE floating point layout:
     * 3         2         1          0
     * 10987654321098765432109876543210
     * SEEEEEEEEMMMMMMMMMMMMMMMMMMMMMMM
     * |   |               |
     * |   |               23 bits of mantissa
     * |   8 bits of result
     * 1 bit of sign
     */

    result.nReal = num;

    if (result.nUInt)
    {
        const RwSplitBits  *const InvSqrtTab = ((rwVectorGlobals *)
                                                & (((RwUInt8 *) RwEngineInstance)[vectorModule.globalsOffset]))->InvSqrtTab;

        /* correction factor */
        result.nUInt += (1L << (22 - rwSQRTTABLEPRECISION));
        RWASSERT(((result.nUInt & 0x007fffffUL) >> rwSQRTTABLESHIFT) < rwSQRTTABLESIZE);

        result.nUInt = InvSqrtTab[(result.nUInt & 0x00ffffffUL) >> rwSQRTTABLESHIFT].nUInt +
            ((0x7f800000UL & ~result.nUInt) >> 1);
    }

    RWRETURN(result.nReal);
}

#endif /* (!defined(RWNOINVSQRTTABLE)) */

/**
 * \ingroup rwv2d
 * \ref RwV2dLength is used to calculate the length of the specified
 * 2D vector.
 *
 * \param in  A pointer to the 2D vector
 *
 * \return Returns an \ref RwReal value equal to the vector's length if successful
 *
 * \see RwV2dNormalize
 *
 */
RwReal
RwV2dLength(const RwV2d * in)
{
    RwReal length = ((RwReal)0);
    RWAPIFUNCTION(RWSTRING("RwV2dLength"));

    RWASSERT(vectorModule.numInstances);
    RWASSERT(in);

    RwV2dLengthMacro(length, in);
    RWRETURN(length);
}

/**
 * \ingroup rwv2d
 * \ref RwV2dNormalize is used to normalize the specified 2D vector. The
 * input and output arguments can point to the same 2D vector.
 *
 * \note It
 * is an error to try to normalize a vector whose length is zero. If this
 * condition is detected \ref RwV2dNormalize places a vector with components
 * (0.0,0.0) in the output argument and returns 0.0.
 *
 * \param out  A pointer to the 2D vector that will receive the normalized vector
 * \param in  A pointer to the 2D vector that is to be normalized and whose length
 * is required
 *
 * \return Returns an \ref RwReal value equal to the original length of the vector
 *        if successful or zero if there is an error
 *
 * \see RwV2dLength
 *
 */
RwReal
RwV2dNormalize(RwV2d * out, const RwV2d * in)
{
    RwReal              length = ((RwReal)0);
    RWAPIFUNCTION(RWSTRING("RwV2dNormalize"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(in);

    RwV2dNormalizeMacro(length, out, in);

    if (length <= (RwReal) ((0)))
    {
        RWERROR((E_RW_ZEROLENGTH));
    }

    RWRETURN(length);
}

/* All of the Following functions are for Debug version only
  * They each have a macro counterpart which is used in the
  * Release builds.
  */

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )

/**
 * \ingroup rwv2d
 * \ref RwV2dAssign is used to assign the first 2D vectors
 * the value of the second 2D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param target  A pointer to the target 2D vector that will receive the assignment
 * \param source  A pointer to the source 2D vector
 *
 * \return None
 *
 * \see RwV2dAdd
 * \see RwV2dScale
 * \see RwV2dSub
 *
 */
void
RwV2dAssign(RwV2d *target, const RwV2d *source)
{
    RWAPIFUNCTION(RWSTRING("RwV2dAssign"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(target);
    RWASSERT(source);

    RwV2dAssignMacro(target, source);

    RWRETURNVOID();
}

/**
 * \ingroup rwv2d
 * \ref RwV2dAdd is used to add together the specified 2D vectors.
 * The result of the addition is placed in a third 2D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 2D vector that will receive the result of the addition
 * \param ina  A pointer to the first 2D vector
 * \param inb  A pointer to the second 2D vector
 *
 * \return None
 *
 * \see RwV2dScale
 * \see RwV2dSub
 *
 */
void
RwV2dAdd(RwV2d * out, const RwV2d * ina, const RwV2d * inb)
{
    RWAPIFUNCTION(RWSTRING("RwV2dAdd"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(ina);
    RWASSERT(inb);

    RwV2dAddMacro(out, ina, inb);
    RWRETURNVOID();
}

/**
 * \ingroup rwv2d
 * \ref RwV2dSub is used to subtract the specified 2D vectors. The second
 * vector is subtracted from the first. The result of the subtraction is
 * placed in a third 2D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 2D vector that will receive the result of the subtraction
 * \param ina  A pointer to the first 2D vector
 * \param inb  A pointer to the second 2D vector
 *
 * \return None
 *
 * \see RwV2dAdd
 * \see RwV2dScale
 *
 */
void
RwV2dSub(RwV2d * out, const RwV2d * ina, const RwV2d * inb)
{
    RWAPIFUNCTION(RWSTRING("RwV2dSub"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(ina);
    RWASSERT(inb);

    RwV2dSubMacro(out, ina, inb);
    RWRETURNVOID();
}

/**
 * \ingroup rwv2d
 * \ref RwV2dLineNormal is used to find the unit normal to line between
 * the specified 2D vectors.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 2D vector that will receive the unit normal
 * \param ina  A pointer to the first 2D vector
 * \param inb  A pointer to the second 2D vector
 *
 * \return None
 *
 * \see RwV2dAdd
 * \see RwV2dScale
 *
 */
void
RwV2dLineNormal(RwV2d *out, const RwV2d *ina, const RwV2d *inb)
{
    RWAPIFUNCTION(RWSTRING("RwV2dLineNormal"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(ina);
    RWASSERT(inb);

    RwV2dLineNormalMacro(out, ina, inb);
    RWRETURNVOID();
}

/**
 * \ingroup rwv2d
 * \ref RwV2dScale is used to multiply the specified 2D vector
 * by the given scalar. The result is placed in a second 2D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 2D vector that will receive the result
 * \param in  A pointer to the 2D vector that will be scaled
 * \param scalar  An \ref RwReal value equal to the scale factor
 *
 * \return None
 *
 * \see RwV2dAdd
 * \see RwV2dSub
 *
 */
void
RwV2dScale(RwV2d * out, const RwV2d * in, RwReal scalar)
{
    RWAPIFUNCTION(RWSTRING("RwV2dScale"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(in);
    RWASSERT(out);

    RwV2dScaleMacro(out, in, scalar);
    RWRETURNVOID();
}

/**
 * \ingroup rwv2d
 * \ref RwV2dDotProduct is used to calculate the scalar product of the
 * specified 2D vectors.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param ina  A pointer to the first 2D vector
 * \param inb  A pointer to the second 2D vector
 *
 * \return Returns an \ref RwReal value equal to the scalar product of the two
 * input vectors if successful or -1 if there is an error
 *
 */
RwReal
RwV2dDotProduct(const RwV2d * ina, const RwV2d * inb)
{
    RWAPIFUNCTION(RWSTRING("RwV2dDotProduct"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(ina);
    RWASSERT(inb);

    RWRETURN(RwV2dDotProductMacro(ina, inb));
}

/**
 * \ingroup rwv2d
 * \ref RwV2dPerp is used to calculate a 2D vector that is perpendicular
 * to the given 2D vector.
 *
 * \note The input and output arguments must not
 * point to the same 2D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to a 2D vector that will receive the perpendicular vector
 * \param in   A pointer to the 2D vector whose perpendicular is required
 *
 * \return None
 *
 */
void
RwV2dPerp(RwV2d * out, const RwV2d * in)
{
    RWAPIFUNCTION(RWSTRING("RwV2dPerp"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(in);

    RwV2dPerpMacro(out, in);
    RWRETURNVOID();
}

/*********    3D Vector Functions    ***********/

/**
 * \ingroup rwv3d
 * \ref RwV3dAssign is used to assign to the first 3D vectors
 * the value of the second 3D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param target  Pointer to the target 3D vector that will receive the assignment
 * \param source  Pointer to the source 3D vector
 *
 * \return None
 *
 * \see RwV3dAdd
 * \see RwV3dIncrementScaled
 * \see RwV3dScale
 * \see RwV3dSubMacro
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVectors
 *
 */
void
RwV3dAssign(RwV3d *target, const RwV3d *source)
{
    RWAPIFUNCTION(RWSTRING("RwV3dAssign"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(target);
    RWASSERT(source);

    RwV3dAssignMacro(target, source);

    RWRETURNVOID();
}

/**
 * \ingroup rwv3d
 * \ref RwV3dAdd is used to add together the specified 3D vectors.
 * The result of the addition is placed in a third 3D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 3D vector that will receive the result of the addition
 * \param ina  A pointer to the first 3D vector
 * \param inb  A pointer to the second 3D vector
 *
 * \return None
 *
 * \see RwV3dIncrementScaled
 * \see RwV3dScale
 * \see RwV3dSubMacro
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVectors
 *
 */
void
RwV3dAdd(RwV3d *out, const RwV3d *ina, const RwV3d *inb)
{
    RWAPIFUNCTION(RWSTRING("RwV3dAdd"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(ina);
    RWASSERT(inb);

    RwV3dAddMacro(out, ina, inb);
    RWRETURNVOID();
}

/**
 * \ingroup rwv3d
 * \ref RwV3dSubMacro is used to subtract the specified 3D vectors. The second
 * vector is subtracted from the first. The result of the subtraction is
 * placed in a third 3D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 3D vector that will receive the result of the subtraction
 * \param ina  A pointer to the first 3D vector
 * \param inb  A pointer to the second 3D vector
 *
 * \return None
 *
 * \see RwV3dAdd
 * \see RwV3dIncrementScaled
 * \see RwV3dScale
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVectors
 *
 */
void
RwV3dSubMacro(RwV3d *out, const RwV3d *ina, const RwV3d *inb)
{
    RWAPIFUNCTION(RWSTRING("RwV3dSubMacro"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(ina);
    RWASSERT(inb);

    RwV3dSubMacro(out, ina, inb);
    RWRETURNVOID();
}

/**
 * \ingroup rwv3d
 * \ref RwV3dScale is used to multiply the specified 3D vector
 * by the given scalar. The result is placed in a second 3D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 3D vector that will receive the result
 * \param in  A pointer to the 3D vector that will be scaled
 * \param scalar  An \ref RwReal value equal to the scale factor
 *
 * \return None
 *
 * \see RwV3dAdd
 * \see RwV3dIncrementScaled
 * \see RwV3dSubMacro
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVectors
 *
 */
void
RwV3dScale(RwV3d *out, const RwV3d *in, RwReal scalar)
{
    RWAPIFUNCTION(RWSTRING("RwV3dScale"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(in);

    RwV3dScaleMacro(out, in, scalar);
    RWRETURNVOID();
}

/**
 * \ingroup rwv3d
 * \ref RwV3dIncrementScaled multiplies the second 3D vector
 * by the given scalar and increments the first 3D vector by
 * this amount
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 3D vector that will accumulate the result
 * \param in  A pointer to the 3D vector that will be scaled
 * \param scalar  An \ref RwReal value equal to the scaled factor
 *
 * \return None
 *
 * \see RwV3dAdd
 * \see RwV3dScale
 * \see RwV3dSubMacro
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVectors
 *
 */
void
RwV3dIncrementScaled(RwV3d *out, const RwV3d *in, RwReal scalar)
{
    RWAPIFUNCTION(RWSTRING("RwV3dIncrementScaled"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(in);

    RwV3dIncrementScaledMacro(out, in, scalar);
    RWRETURNVOID();
}

/**
 * \ingroup rwv3d
 * \ref RwV3dNegate negates the second 3D vector
 * and returns the result in the first 3D vector.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 3D vector that will receive the result
 * \param in  A pointer to the 3D vector that will be negated
 *
 * \return None
 *
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVectors
 *
 */
void
RwV3dNegate(RwV3d *out, const RwV3d *in)
{
    RWAPIFUNCTION(RWSTRING("RwV3dNegate"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(in);

    RwV3dNegateMacro(out, in);
    RWRETURNVOID();
}

/**
 * \ingroup rwv3d
 * \ref RwV3dDotProduct is used to calculate the scalar product of the
 * specified 3D vectors.
 *
 * \note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param ina  A pointer to the first 3D vector
 * \param inb  A pointer to the second 3D vector
 *
 * \return Returns an \ref RwReal value equal to the scalar product of the two
 *        input vectors if successful or -1 if there is an error
 *
 * \see RwV3dCrossProduct
 *
 */
RwReal
RwV3dDotProduct(const RwV3d *ina, const RwV3d *inb)
{
    RWAPIFUNCTION(RWSTRING("RwV3dDotProduct"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(ina);
    RWASSERT(inb);

    RWRETURN(RwV3dDotProductMacro(ina, inb));
}

/**
 * \ingroup rwv3d
 * \ref RwV3dCrossProduct is used to calculate the cross product of the
 * specified 3D vectors, in the order given. The result of the cross product is
 * placed in a third 3D vector.
 *
 * \note The argument corresponding to the
 * output 3D vector should not point to either of the other arguments.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param out  A pointer to the 3D vector that will receive the cross product vector
 * \param ina  A pointer to the first 3D vector
 * \param inb  A pointer to the second 3D vector
 *
 * \return None
 *
 * \see RwV3dDotProduct
 *
 */
void
RwV3dCrossProduct(RwV3d *out, const RwV3d *ina, const RwV3d *inb)
{
    RWAPIFUNCTION(RWSTRING("RwV3dCrossProduct"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(out);
    RWASSERT(ina);
    RWASSERT(inb);
    RWASSERT(ina != out);
    RWASSERT(inb != out);

    RwV3dCrossProductMacro(out, ina, inb);
    RWRETURNVOID();
}

#endif /* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */

/**
 * \ingroup rwv3d
 * \ref RwV3dTransformPoint uses the given matrix describing a
 * transformation and applies it to the specified point.  The
 * result is placed in another point (which may be the same point as
 * the source).
 *
 * \param pointOut  A pointer to an RwV3d that is to receive the results of
 *                  the point transformation
 * \param pointIn   A pointer to an RwV3d that is to supply the point to be
 *                   transformed
 * \param matrix    A pointer to the matrix supplying the transformation
 *
 * \return Returns a pointer to the transformed point if successful or NULL
 *        if there is an error
 *
 * \see RwMatrixTransform
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVector
 * \see RwV3dTransformVectors
 */
RwV3d *
RwV3dTransformPoint(RwV3d *pointOut,
                    const RwV3d *pointIn,
                    const RwMatrix *matrix)
{
    RWAPIFUNCTION(RWSTRING("RwV3dTransformPoint"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(pointOut);
    RWASSERT(pointIn);
    RWASSERT(matrix);

    RwV3dTransformPointMacro(pointOut, pointIn, matrix);

    RWRETURN(pointOut);
}

/**
 * \ingroup rwv3d
 * \ref RwV3dTransformPoints uses the given matrix describing a
 * transformation and applies it to the specified array of points. The
 * results are then placed in another array (which may be the same array as
 * the source).
 *
 * \note The source and destination array may be separate or coincident, but
 * the result is not defined if the source and destination overlap in any
 * other way (i.e. if they are offset from one another but still overlap).
 *
 * \param pointsOut  A pointer to an array of RwV3ds that are to
 * receive the results of the point transformation
 * \param pointsIn   A pointer to an array of RwV3ds that are to supply
 * to points to be transformed
 * \param numPoints  A number of points to transform
 * \param matrix     A pointer to the matrix supplying the transformation
 *
 * \return Returns a pointer to the transformed point array if successful or NULL
 *        if there is an error
 *
 * \see RwMatrixTransform
 * \see RwV3dTransformPoint
 * \see RwV3dTransformVector
 * \see RwV3dTransformVectors
 */
RwV3d *
RwV3dTransformPoints(RwV3d *pointsOut,
                     const RwV3d *pointsIn,
                     RwInt32 numPoints,
                     const RwMatrix *matrix)
{
    RWAPIFUNCTION(RWSTRING("RwV3dTransformPoints"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(pointsOut);
    RWASSERT(pointsIn);
    RWASSERT(numPoints > 0);
    RWASSERT(matrix);

    RwV3dTransformPointsMacro(pointsOut, pointsIn, numPoints, matrix);

    RWRETURN(pointsOut);
}

/**
 * \ingroup rwv3d
 * \ref RwV3dTransformVector uses the given matrix describing a
 * transformation and applies it to the specified vector. The result
 * is placed in another vector (which may be the same as the source).
 *
 * \param vectorOut A pointer to a RwV3d that is to receive the results of
 *       the vector transformation
 * \param vectorIn  A pointer to a RwV3d that is to supply the vector to be
 *       transformed
 * \param matrix    A pointer to the matrix supplying the transformation
 *
 * \return Returns a pointer to the transformed vector if successful
 *        or NULL if there is an error
 *
 * \see RwMatrixTransform
 * \see RwV3dTransformPoint
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVectors
 */
RwV3d *
RwV3dTransformVector(RwV3d *vectorOut,
                     const RwV3d *vectorIn,
                     const RwMatrix *matrix)
{
    RWAPIFUNCTION(RWSTRING("RwV3dTransformVector"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(vectorOut);
    RWASSERT(vectorIn);
    RWASSERT(matrix);

    RwV3dTransformVectorMacro(vectorOut, vectorIn, matrix);

    RWRETURN(vectorOut);
}

/**
 * \ingroup rwv3d
 * \ref RwV3dTransformVectors uses the given matrix describing a
 * transformation and applies it to the specified array of vectors. The results
 * are then placed in another array (which may be the same array as the source).
 *
 * \note The source and destination array may be separate or coincident, but
 * the result is not defined if the source and destination overlap in any
 * other way (ie, if they are offset from one another but still overlap).
 *
 * \param vectorsOut A pointer to an array of RwV3ds that are to receive the results of
 *       the vector transformation
 * \param vectorsIn  A pointer to an array of RwV3ds that are to supply to vectors to be
 *       transformed
 * \param numPoints  A number of vectors to transform
 * \param matrix     A pointer to the matrix supplying the transformation
 *
 * \return Returns a pointer to the transformed vector if successful
 *        or NULL if there is an error
 *
 * \see RwMatrixTransform
 * \see RwV3dTransformPoint
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVector
 */
RwV3d *
RwV3dTransformVectors(RwV3d *vectorsOut,
                      const RwV3d *vectorsIn,
                      RwInt32 numPoints,
                      const RwMatrix *matrix)
{
    RWAPIFUNCTION(RWSTRING("RwV3dTransformVectors"));
    RWASSERT(vectorModule.numInstances);
    RWASSERT(vectorsOut);
    RWASSERT(vectorsIn);
    RWASSERT(numPoints > 0);
    RWASSERT(matrix);

    RwV3dTransformVectorsMacro(vectorsOut, vectorsIn, numPoints, matrix);

    RWRETURN(vectorsOut);
}

/****************************************************************************
 _rwVectorClose

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwVectorClose(void *instance,
               RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwVectorClose"));

#if (!defined(RWNOINVSQRTTABLE))
    InvSqrtTableDestroy();
#endif /* (!defined(RWNOINVSQRTTABLE)) */
#if (!defined(RWNOSQRTTABLE))
    SqrtTableDestroy();
#endif /* (!defined(RWNOSQRTTABLE)) */

    /* One less module instance */
    --vectorModule.numInstances;

    RWRETURN(instance);
}

/****************************************************************************
 _rwVectorOpen

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwVectorOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwVectorOpen"));

    /* Save the global data offset (same for all instances) */
    vectorModule.globalsOffset = offset;

#if !defined(RWNOVECMULTFUNCS)
    /* Set default vector operation functions */
    _rwVectorSetMultFn((rwVectorMultFn)NULL,
                       (rwVectorsMultFn)NULL,
                       (rwVectorMultFn)NULL,
                       (rwVectorsMultFn)NULL);
#endif /* !defined(RWNOVECMULTFUNCS) */

#if (!defined(RWNOSQRTTABLE))
    if (!SqrtTableCreate())
    {
        /* Failure */
        RWRETURN(NULL);
    }
#endif /* (!defined(RWNOSQRTTABLE)) */

#if (!defined(RWNOINVSQRTTABLE))
    if (!InvSqrtTableCreate())
    {
        /* Failure */
        RWRETURN(NULL);
    }
#endif /* (!defined(RWNOINVSQRTTABLE)) */

    /* This is another module instance */
    vectorModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}
