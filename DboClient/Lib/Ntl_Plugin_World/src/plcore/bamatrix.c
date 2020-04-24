/*
 * Matrix handling functions.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/**
 * \ingroup rwmatrix
 * \page rwmatrixoverview RwMatrix Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection matrixoverview Overview
 *
 * This object defines a RenderWare Matrix object.
 *
 * The Matrix is heavily used throughout the API and a full range of
 * functions are provided, including: rotation, multiplication,
 * concatenation, scaling, translation, creation, destruction, stream
 * read/write functions and a number of access functions to access
 * particular vector data within the matrix.
 *
 * RenderWare uses 4x3, row-major affine matrices.
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"
#include "bacamera.h"
#include "batextur.h"
#include "bapipe.h"
#include "baresour.h"
#include "bavector.h"

#include "bamatrix.h"


/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#if (!defined(RWASMAPI_void))
#define RWASMAPI_void RWASMAPI(void)  /* To prevent Doxygen being confused */
#endif /* (!defined(RWASMAPI_void)) */

#define RWMATRIXOPTIMIZETOLERANCE (((RwReal)0.01))

#ifndef _NORMAL_TOL
#define _NORMAL_TOL      (RWMATRIXGLOBAL(tolerance).Normal)
#endif /* _NORMAL_TOL */

#ifndef _ORTHOGONAL_TOL
#define _ORTHOGONAL_TOL  (RWMATRIXGLOBAL(tolerance).Orthogonal)
#endif /* _ORTHOGONAL_TOL */

#ifndef _IDENTITY_TOL
#define _IDENTITY_TOL    (RWMATRIXGLOBAL(tolerance).Identity)
#endif /* _NORMAL_TOL */

#ifndef _DETERMINANT_TOL
#define _DETERMINANT_TOL (((RwReal)1) - RWMATRIXGLOBAL(tolerance).Identity)
#endif /* _DETERMINANT_TOL */

#ifndef  rwTOLSINMULTIPLEPI
#define  rwTOLSINMULTIPLEPI ((RwReal)0.01)
#endif /* rwTOLSINMULTIPLEPI */

#ifndef M_PI

/* #define M_PI 3.1415926535897932 */
#   define M_PI 3.14159265358979323846
#endif /* M_PI */

#define RWMATRIXGLOBAL(var)                     \
    (RWPLUGINOFFSET(rwMatrixGlobals,            \
     RwEngineInstance,                          \
     matrixModule.globalsOffset)->var)

#define RwV3dDotDOWN(_a,_b,_col)                \
  ( ((_a).x) * ((_b).right._col) +              \
    ((_a).y) * ((_b).up._col)    +              \
    ((_a).z) * ((_b).at._col)    )

#define GENERIC_FLAGS(_matrix)                  \
   (~rwMATRIXINTERNALIDENTITY & (rwMatrixGetFlags(_matrix)))

/*
 * RwMatrixMultiplyMacro() can be overloaded per platform in
 * rwsdk/driver/$(RWTARGET)/drvmodel.h
 * The default/generic implementation is below.
 */
#if (!defined(RwMatrixMultiplyMacro))

#define RwMatrixMultiplyMacro(_matrix, _matrixIn1, _matrixIn2)               \
MACRO_START                                                                  \
{                                                                            \
    const RwInt32       matrixIn1Flags = rwMatrixGetFlags(_matrixIn1);       \
    const RwInt32       matrixIn2Flags = rwMatrixGetFlags(_matrixIn2);       \
    const RwInt32       identityOptimization =                               \
        RWMATRIXGLOBAL(matrixOptimizations) & rwMATRIXINTERNALIDENTITY;      \
                                                                             \
    if (matrixIn1Flags & identityOptimization)                               \
    {                                                                        \
        RWASSERT(rwMatrixIsIdentity((_matrixIn1), _IDENTITY_TOL));           \
        RwMatrixCopy((_matrix), (_matrixIn2));                               \
    }                                                                        \
    else if (matrixIn2Flags & identityOptimization)                          \
    {                                                                        \
        RWASSERT(rwMatrixIsIdentity((_matrixIn2), _IDENTITY_TOL));           \
        RwMatrixCopy((_matrix), (_matrixIn1));                               \
    }                                                                        \
    else                                                                     \
    {                                                                        \
        const RwInt32       matrixFlags = (matrixIn1Flags & matrixIn2Flags); \
        RWMATRIXGLOBAL(multMatrix) ((_matrix), (_matrixIn1), (_matrixIn2));  \
        rwMatrixSetFlags((_matrix), matrixFlags);                            \
    }                                                                        \
}                                                                            \
MACRO_STOP

#endif /* (!defined(RwMatrixMultiplyMacro)) */
/*
 * RwMatrixInvertMacro() can be overloaded per platform in
 * rwsdk/driver/$(RWTARGET)/drvmodel.h
 * The default/generic implementation is below.
 */
#if (!defined(RwMatrixInvertMacro))
#define RwMatrixInvertMacro(_dst, _src)                                     \
MACRO_START                                                                 \
{                                                                           \
    /* We might be able to do a really fast inversion if its Identity */    \
    if (rwMatrixTestFlags(_src,                                             \
                          (RWMATRIXGLOBAL(matrixOptimizations) &            \
                           rwMATRIXINTERNALIDENTITY)))                      \
    {                                                                       \
        RWASSERT(rwMatrixIsIdentity(_src, _IDENTITY_TOL));                  \
        RwMatrixCopy(_dst, _src);                                           \
    }                                                                       \
    else if (rwMatrixTestFlags(_src, rwMATRIXTYPEMASK) ==                   \
             rwMATRIXTYPEORTHONORMAL)                                       \
    {                                                                       \
        /* We can do a fast inversion if it's ortho normal */               \
        MatrixInvertOrthoNormalized(_dst, _src);                            \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        MatrixInvertGeneric(_dst, _src);                                    \
    }                                                                       \
}                                                                           \
MACRO_STOP
#endif /* (!defined(RwMatrixInvertMacro)) */
/****************************************************************************
 Globals (across program)
 */
/****************************************************************************
 Local (static) Globals
 */
static RwModuleInfo matrixModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Matrix handling

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 MatrixMultiply

 On entry   : Dest matrix pointer, two source matrix pointers
 On exit    : Matrix pointer contains result
 */

static RWASMAPI_void
MatrixMultiply(RwMatrix * dstMat,
               const RwMatrix * matA, const RwMatrix * matB)
{
    RWFUNCTION(RWSTRING("MatrixMultiply"));
    RWASSERT(dstMat);
    RWASSERT(matA);
    RWASSERT(RWMATRIXALIGNMENT(matA));
    RWASSERT(matB);
    RWASSERT(RWMATRIXALIGNMENT(matB));

    /* Multiply out right */
    dstMat->right.x = ( (matA->right.x * matB->right.x) +
                        (matA->right.y * matB->up.x) +
                        (matA->right.z * matB->at.x) );
    dstMat->right.y = ( (matA->right.x * matB->right.y) +
                        (matA->right.y * matB->up.y) +
                        (matA->right.z * matB->at.y) );
    dstMat->right.z = (  (matA->right.x * matB->right.z) +
                         (matA->right.y * matB->up.z) +
                         (matA->right.z * matB->at.z) );

    /* Then up */
    dstMat->up.x = ( (matA->up.x * matB->right.x) +
                     (matA->up.y * matB->up.x) +
                     (matA->up.z * matB->at.x) );
    dstMat->up.y = ( (matA->up.x * matB->right.y) +
                      (matA->up.y * matB->up.y) +
                      (matA->up.z * matB->at.y) );
    dstMat->up.z = ( (matA->up.x * matB->right.z) +
                     (matA->up.y * matB->up.z) +
                     (matA->up.z * matB->at.z) );

    /* Then at */
    dstMat->at.x = ( (matA->at.x * matB->right.x) +
                     (matA->at.y * matB->up.x) +
                     (matA->at.z * matB->at.x) );
    dstMat->at.y = ( (matA->at.x * matB->right.y) +
                     (matA->at.y * matB->up.y) +
                     (matA->at.z * matB->at.y) );
    dstMat->at.z = ( (matA->at.x * matB->right.z) +
                     (matA->at.y * matB->up.z) +
                     (matA->at.z * matB->at.z) );

    /* Then pos - this is different because there is an extra add
     * (implicit 1 (one) in bottom right of matrix)
     */
    dstMat->pos.x = ( (matA->pos.x * matB->right.x) +
                      (matA->pos.y * matB->up.x) +
                      (matA->pos.z * matB->at.x) +
                      ( /* (1*) */ matB->pos.x) );
    dstMat->pos.y = ( (matA->pos.x * matB->right.y) +
                      (matA->pos.y * matB->up.y) +
                      (matA->pos.z * matB->at.y) +
                      ( /* (1*) */ matB->pos.y) );
    dstMat->pos.z = ( (matA->pos.x * matB->right.z) +
                      (matA->pos.y * matB->up.z) +
                      (matA->pos.z * matB->at.z) +
                      ( /* (1*) */ matB->pos.z) );

    /* And that's all folks */
    RWRETURNVOID();
}

/*
 * John's modified Gram-Schmidt algorithm
 * See also
 * http://www.gamasutra.com/features/programming/19980703/quaternions_03.htm
 */

static RwMatrix *
MatrixOrthoNormalize(RwMatrix * dst, const RwMatrix * src)
{
    RwV3d               right, up, at, pos;
    RwV3d              *vpU, *vpV, *vpW;
    RwV3d               vInner, vOuter;
    RwReal              recip;

    RWFUNCTION(RWSTRING("MatrixOrthoNormalize"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(dst);
    RWASSERT(RWMATRIXALIGNMENT(dst));
    RWASSERT(src);
    RWASSERT(RWMATRIXALIGNMENT(src));

    /* Ortho-normalize a dst in a right handed sense */

    /* Cache copies of src rows
     * to allow for equal addresses
     * dst == src
     */
    RwV3dAssign(&right, &src->right);
    RwV3dAssign(&up, &src->up);
    RwV3dAssign(&at, &src->at);
    RwV3dAssign(&pos, &src->pos);

    _rwV3dNormalizeMacro(vInner.x, &right, &right);
    _rwV3dNormalizeMacro(vInner.y, &up, &up);
    _rwV3dNormalizeMacro(vInner.z, &at, &at);

    /*
     * pivot about non-zero pair of axes
     */
    if (vInner.x > ((RwReal)0))
    {
        if (vInner.y > ((RwReal)0))
        {
            if (vInner.z > ((RwReal)0))
            {
                /*
                 * pivot about most 'right-angular' pair of axes
                 */
                vOuter.x = RwV3dDotProductMacro(&up, &at);
                vOuter.x = RwRealAbs(vOuter.x);

                vOuter.y = RwV3dDotProductMacro(&at, &right);
                vOuter.y = RwRealAbs(vOuter.y);

                vOuter.z = RwV3dDotProductMacro(&right, &up);
                vOuter.z = RwRealAbs(vOuter.z);

                if (vOuter.x < vOuter.y)
                {
                    if (vOuter.x < vOuter.z)
                    {
                        vpU = &up;
                        vpV = &at;
                        vpW = &right;
                    }
                    else
                    {
                        vpU = &right;
                        vpV = &up;
                        vpW = &at;
                    }
                }
                else
                {
                    if (vOuter.y < vOuter.z)
                    {
                        vpU = &at;
                        vpV = &right;
                        vpW = &up;
                    }
                    else
                    {
                        vpU = &right;
                        vpV = &up;
                        vpW = &at;
                    }
                }
            }                  /* (Inner_z > ((RwReal)0) */
            else
            {
                vpU = &right;
                vpV = &up;
                vpW = &at;
            }
        }                      /* (Inner_y > ((RwReal)0) */
        else
        {
            vpU = &at;
            vpV = &right;
            vpW = &up;
        }
    }                          /* (Inner_x > ((RwReal)0) */
    else
    {
        vpU = &up;
        vpV = &at;
        vpW = &right;
    }

    RwV3dCrossProductMacro(vpW, vpU, vpV);
    _rwV3dNormalizeMacro(recip, vpW, vpW);
    RwV3dCrossProductMacro(vpV, vpW, vpU);
    _rwV3dNormalizeMacro(recip, vpV, vpV);

    dst->right = right;
    dst->up = up;
    dst->at = at;
    dst->pos = pos;

    /* It's now dirty, needs it's objects resyncing and is not identity */
    /* It is also orthonormal */
    rwMatrixSetFlags(dst, ( (rwMatrixGetFlags(dst) | rwMATRIXTYPEORTHONORMAL) &
                         ~rwMATRIXINTERNALIDENTITY) );

    RWASSERT(rwMatrixIsNormal(dst, _NORMAL_TOL) &&
             rwMatrixIsOrthogonal(dst, _ORTHOGONAL_TOL) &&
             ((_DETERMINANT_TOL <= _rwMatrixDeterminant(dst))));

    RWRETURN(dst);
}


/****************************************************************************
 *
 * A faster matrix inversion function for orthonormal matrices
 * Tranposition
 *
 */
static RwMatrix *
MatrixInvertOrthoNormalized(RwMatrix * dst,
                            const RwMatrix * src)
{
    RWFUNCTION(RWSTRING("MatrixInvertOrthoNormalized"));
    RWASSERT(dst);
    RWASSERT(RWMATRIXALIGNMENT(dst));
    RWASSERT(src);
    RWASSERT(RWMATRIXALIGNMENT(src));
    RWASSERT(rwMatrixIsNormal(src, _NORMAL_TOL) &&
             rwMatrixIsOrthogonal(src, _ORTHOGONAL_TOL));
    /*
     * Inverse of upper left 3x3 sub matrix
     * is a simple transpose
     */
    dst->right.x = src->right.x;
    dst->right.y = src->up.x;
    dst->right.z = src->at.x;

    dst->up.x = src->right.y;
    dst->up.y = src->up.y;
    dst->up.z = src->at.y;

    dst->at.x = src->right.z;
    dst->at.y = src->up.z;
    dst->at.z = src->at.z;
    /*
     * calculate translation componennt of inverse
     */
    dst->pos.x = -RwV3dDotProductMacro(&src->pos, &src->right);
    dst->pos.y = -RwV3dDotProductMacro(&src->pos, &src->up);
    dst->pos.z = -RwV3dDotProductMacro(&src->pos, &src->at);

    rwMatrixSetFlags(dst, rwMATRIXTYPEORTHONORMAL);

    RWASSERT(rwMatrixIsNormal(dst, _NORMAL_TOL) &&
             rwMatrixIsOrthogonal(dst, _ORTHOGONAL_TOL));

    /* All done */
    RWRETURN(dst);
}

static RwMatrix *
MatrixInvertGeneric(RwMatrix * dst, const RwMatrix * src)
{
    RWFUNCTION(RWSTRING("MatrixInvertGeneric"));

    RWASSERT(dst);
    RWASSERT(RWMATRIXALIGNMENT(dst));
    RWASSERT(src);
    RWASSERT(RWMATRIXALIGNMENT(src));

#undef  m_X_X
#define m_X_X src->right.x
#undef  m_X_Y
#define m_X_Y src->right.y
#undef  m_X_Z
#define m_X_Z src->right.z

#undef  m_Y_X
#define m_Y_X src->up.x
#undef  m_Y_Y
#define m_Y_Y src->up.y
#undef  m_Y_Z
#define m_Y_Z src->up.z

#undef  m_Z_X
#define m_Z_X src->at.x
#undef  m_Z_Y
#define m_Z_Y src->at.y
#undef  m_Z_Z
#define m_Z_Z src->at.z

#define T_X src->pos.x
#define T_Y src->pos.y
#define T_Z src->pos.z

#undef  i_X_X
#define i_X_X dst->right.x
#undef  i_X_Y
#define i_X_Y dst->right.y
#undef  i_X_Z
#define i_X_Z dst->right.z

#undef  i_Y_X
#define i_Y_X dst->up.x
#undef  i_Y_Y
#define i_Y_Y dst->up.y
#undef  i_Y_Z
#define i_Y_Z dst->up.z

#undef  i_Z_X
#define i_Z_X dst->at.x
#undef  i_Z_Y
#define i_Z_Y dst->at.y
#undef  i_Z_Z
#define i_Z_Z dst->at.z

#undef  i_W_X
#define i_W_X dst->pos.x
#undef  i_W_Y
#define i_W_Y dst->pos.y
#undef  i_W_Z
#define i_W_Z dst->pos.z

    /* Assumes input of form
     * [ [ m_X_X, m_X_Y, m_X_Z, 0 ]
     * [ m_Y_X, m_Y_Y, m_Y_Z, 0 ]
     * [ m_Z_X, m_Z_Y, m_Z_Z, 0 ]
     * [ T_X,   T_Y,   T_Z,   1 ] ]
     */

    /* Find inverse of 3 sub matrix
     * via transposed matrix of cofactors (cross-products) */

    rwMat03Inv(i_X_X, i_X_Y, i_X_Z,
               i_Y_X, i_Y_Y, i_Y_Z,
               i_Z_X, i_Z_Y, i_Z_Z,
               m_X_X, m_X_Y, m_X_Z,
               m_Y_X, m_Y_Y, m_Y_Z,
               m_Z_X, m_Z_Y, m_Z_Z);
    /*
     * Find translation component of inverse
     */

    i_W_X = -((T_X) * (i_X_X) + (T_Y) * (i_Y_X) + (T_Z) * (i_Z_X));
    i_W_Y = -((T_X) * (i_X_Y) + (T_Y) * (i_Y_Y) + (T_Z) * (i_Z_Y));
    i_W_Z = -((T_X) * (i_X_Z) + (T_Y) * (i_Y_Z) + (T_Z) * (i_Z_Z));

    /* Mark it as dirty */
    rwMatrixSetFlags(dst, 0);

    RWRETURN(dst);
}

static const RwMatrix *
MatrixQueryRotateDegenerateUnitAxis(const RwMatrix * matrix,
                                    RwV3d * unitAxis)
{
    RwV3d               vTwoSinThetaAxis;
    RwReal              recip;

    RWFUNCTION(RWSTRING("MatrixQueryRotateDegenerateUnitAxis"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWASSERT(unitAxis);

    /*
     * Matrix is:
     * [ [ 2 a_x^2 - 1,  2 a_x a_y,   2 a_x a_z,  0 ]
     *   [  2 a_x a_y,  2 a_y^2 - 1,  2 a_y a_z,  0 ]
     *   [  2 a_x a_z,   2 a_y a_z,  2 a_z^2 - 1, 0 ]
     *   [      0,           0,           0,      1 ] ]
     * Build axis scaled by 4 * component of maximum absolute value
     */
    if (matrix->right.x > matrix->up.y)
    {
        if (matrix->right.x > matrix->at.z)
        {
            vTwoSinThetaAxis.x = ((RwReal)1) + matrix->right.x;
            vTwoSinThetaAxis.x = ( vTwoSinThetaAxis.x +
                                   vTwoSinThetaAxis.x );
            vTwoSinThetaAxis.y = matrix->right.y + matrix->up.x;
            vTwoSinThetaAxis.z = matrix->right.z + matrix->at.x;
        }
        else
        {
            vTwoSinThetaAxis.z = ((RwReal)1) + matrix->at.z;
            vTwoSinThetaAxis.z = ( vTwoSinThetaAxis.z +
                                   vTwoSinThetaAxis.z );
            vTwoSinThetaAxis.x = matrix->at.x + matrix->right.z;
            vTwoSinThetaAxis.y = matrix->at.y + matrix->up.z;
        }
    }
    else
    {
        if (matrix->up.y > matrix->at.z)
        {
            vTwoSinThetaAxis.y = ((RwReal)1) + matrix->up.y;
            vTwoSinThetaAxis.y = ( vTwoSinThetaAxis.y +
                                   vTwoSinThetaAxis.y );
            vTwoSinThetaAxis.z = matrix->up.z + matrix->at.y;
            vTwoSinThetaAxis.x = matrix->up.x + matrix->right.y;
        }
        else
        {
            vTwoSinThetaAxis.z = ((RwReal)1) + matrix->at.z;
            vTwoSinThetaAxis.z = ( vTwoSinThetaAxis.z +
                                   vTwoSinThetaAxis.z );
            vTwoSinThetaAxis.x = matrix->at.x + matrix->right.z;
            vTwoSinThetaAxis.y = matrix->at.y + matrix->up.z;
        }
    }

    /*
     * and normalize the axis
     */

    _rwV3dNormalizeMacro(recip, unitAxis, &vTwoSinThetaAxis);
    RWRETURN(matrix);

}
static const RwMatrix *
MatrixQueryRotate(const RwMatrix * matrix, RwV3d * unitAxis,
                  RwReal * angle, RwV3d * center)
{
    RwReal              nTwoSinTheta, nTwoCosTheta;
    RwV3d               vTwoSinThetaAxis;
    RwReal              nRadians;
    RwMatrix            mISubMat;
    RwMatrix            mISubMatInverse;

    RWFUNCTION(RWSTRING("MatrixQueryRotate"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWASSERT(unitAxis);
    RWASSERT(angle);
    RWASSERT(center);

    rwMatrixInitialize(&mISubMat, rwMATRIXTYPENORMAL);
    rwMatrixInitialize(&mISubMatInverse, rwMATRIXTYPENORMAL);
    nTwoCosTheta = matrix->right.x + matrix->up.y + matrix->at.z - ((RwReal)1);
    vTwoSinThetaAxis.x = matrix->up.z - matrix->at.y;
    vTwoSinThetaAxis.y = matrix->at.x - matrix->right.z;
    vTwoSinThetaAxis.z = matrix->right.y - matrix->up.x;
    nTwoSinTheta = RwV3dLength(&vTwoSinThetaAxis);

    if (nTwoSinTheta > ((RwReal)0))
    {
        const RwReal recipLength = (((RwReal)1) / (nTwoSinTheta));

        RwV3dScaleMacro(unitAxis, &vTwoSinThetaAxis, recipLength);
    }
    else
    {
        unitAxis->x = ((RwReal)0);
        unitAxis->y = ((RwReal)0);
        unitAxis->z = ((RwReal)0);
    }

    nRadians = (RwReal) RwATan2(nTwoSinTheta, nTwoCosTheta);
    (*angle) = nRadians * (((RwReal) 180) / ((RwReal) M_PI));
    if ((nTwoSinTheta <= rwTOLSINMULTIPLEPI) && (nTwoCosTheta <= ((RwReal)0)))
    {
        /*
         * sin theta is 0; cos theta is -1; theta is 180 degrees
         * vTwoSinThetaAxis was degenerate
         * axis will have to be found another way.
         */

        MatrixQueryRotateDegenerateUnitAxis(matrix, unitAxis);
    }

    /*
     * Find center of line of rotation:
     * [ v - c ] * R + c   = v * R + s
     * -> v * R + c - c *  R = v * R + s
     * -> c * [ I - R ]      = s
     * -> c                  = s * [ I - R ] ^ -1
     */

    RwMatrixSetIdentity(&mISubMat);
    /*
     * Find [ I - R ] ^ -1
     */
    RwV3dSubMacro(&mISubMat.right, &mISubMat.right, &matrix->right);
    RwV3dSubMacro(&mISubMat.up, &mISubMat.up, &matrix->up);
    RwV3dSubMacro(&mISubMat.at, &mISubMat.at, &matrix->at);
    rwMatrixSetFlags(&mISubMat, GENERIC_FLAGS(&mISubMatInverse));
    rwMatrixSetFlags(&mISubMatInverse, GENERIC_FLAGS(&mISubMatInverse));
    /* MatrixInvertOrthoNormalized(&mISubMatInverse, &mISubMat); */
    MatrixInvertGeneric(&mISubMatInverse, &mISubMat);

    /*
     * Find s * [ I - R ] ^ -1
     */
    *center = mISubMatInverse.pos;
    RwV3dIncrementScaledMacro(center, &mISubMatInverse.right,
                              matrix->pos.x);
    RwV3dIncrementScaledMacro(center, &mISubMatInverse.up,
                              matrix->pos.y);
    RwV3dIncrementScaledMacro(center, &mISubMatInverse.at,
                              matrix->pos.z);

    RWRETURN(matrix);
}

/****************************************************************************
 _rwMatrixSetMultFn

 On entry   : Matrix multiply function (NULL for default)
 On exit    : TRUE on success
 */

RwBool
_rwMatrixSetMultFn(rwMatrixMultFn multMat)
{
    RWFUNCTION(RWSTRING("_rwMatrixSetMultFn"));

    if (NULL == multMat)
    {
        multMat = MatrixMultiply;
    }
    RWMATRIXGLOBAL(multMatrix) = multMat;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwMatrixSetOptimizations

 On entry   : Optimize flags (currently only rwMATRIXOPTIMIZE_IDENTITY is supported).
 On exit    : TRUE on success
 */

RwBool
_rwMatrixSetOptimizations(RwInt32 optimizeFlags)
{
    RWFUNCTION(RWSTRING("_rwMatrixSetOptimizations"));

    RWMATRIXGLOBAL(matrixOptimizations) = optimizeFlags;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwMatrixDeterminant
 */

RwReal
_rwMatrixDeterminant(const RwMatrix * matrix)
{
    RwReal              result;
    const RwV3d        *mx;
    const RwV3d        *my;
    const RwV3d        *mz;
    RwV3d               cross;

    /*
     * Expanded down implict final column
     */

    RWFUNCTION(RWSTRING("_rwMatrixDeterminant"));
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    mx = &matrix->right;
    my = &matrix->up;
    mz = &matrix->at;

    RwV3dCrossProductMacro(&cross, my, mz);
    result = RwV3dDotProductMacro(&cross, mx);

    RWRETURN(result);
}

/****************************************************************************
 _rwMatrixOrthogonalError
 */

RwReal
_rwMatrixOrthogonalError(const RwMatrix * matrix)
{
    RwReal              result;
    const RwV3d        *mx;
    const RwV3d        *my;
    const RwV3d        *mz;
    RwV3d               dot;

    RWFUNCTION(RWSTRING("_rwMatrixOrthogonalError"));
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    mx = &matrix->right;
    my = &matrix->up;
    mz = &matrix->at;

    dot.x = RwV3dDotProductMacro(my, mz);
    dot.y = RwV3dDotProductMacro(mz, mx);
    dot.z = RwV3dDotProductMacro(mx, my);

    result = RwV3dDotProductMacro(&dot, &dot);

    RWRETURN(result);
}

/****************************************************************************
 _rwMatrixNormalError
 */

RwReal
_rwMatrixNormalError(const RwMatrix * matrix)
{
    RwReal              result;
    const RwV3d        *x;
    const RwV3d        *y;
    const RwV3d        *z;
    RwV3d               dot;

    RWFUNCTION(RWSTRING("_rwMatrixNormalError"));
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    x = &matrix->right;
    y = &matrix->up;
    z = &matrix->at;

    dot.x = RwV3dDotProductMacro(x, x) - ((RwReal)1);
    dot.y = RwV3dDotProductMacro(y, y) - ((RwReal)1);
    dot.z = RwV3dDotProductMacro(z, z) - ((RwReal)1);

    result = RwV3dDotProductMacro(&dot, &dot);

    RWRETURN(result);
}

/****************************************************************************
 _rwMatrixIdentityError
 */

RwReal
_rwMatrixIdentityError(const RwMatrix * matrix)
{
    /*
     * Row-norm implied metric
     */
    RwReal              result;
    const RwV3d        *mx;
    const RwV3d        *my;
    const RwV3d        *mz;
    const RwV3d        *mw;
    RwReal              error_x;
    RwReal              error_y;
    RwReal              error_z;
    RwReal              error_w;

    RWFUNCTION(RWSTRING("_rwMatrixIdentityError"));
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    mx = &matrix->right;
    my = &matrix->up;
    mz = &matrix->at;
    mw = &matrix->pos;

    error_x = (mx->x - ((RwReal)1));
    error_x = error_x * error_x + mx->y * mx->y + mx->z * mx->z;

    error_y = (my->y - ((RwReal)1));
    error_y = my->x * my->x + error_y * error_y + my->z * my->z;

    error_z = (mz->z - ((RwReal)1));
    error_z = mz->x * mz->x + mz->y * mz->y + error_z * error_z;

    error_w = mw->x * mw->x + mw->y * mw->y + mw->z * mw->z;

    result = error_x + error_y + error_z + error_w;

    RWRETURN(result);
}

/****************************************************************************
 _rwMatrixClose

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwMatrixClose(void *instance,
               RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwMatrixClose"));
    if (RWMATRIXGLOBAL(matrixFreeList))
    {
        RwFreeListDestroy(RWMATRIXGLOBAL(matrixFreeList));
        RWMATRIXGLOBAL(matrixFreeList) = (RwFreeList *) NULL;
    }

    /* One less module instance */
    matrixModule.numInstances--;
    /* Success */
    RWRETURN(instance);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening the matrix library

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static RwInt32 _rwMatrixFreeListBlockSize = 256,
               _rwMatrixFreeListPreallocBlocks = 1;
static RwFreeList _rwMatrixFreeList;

/**
 * \ingroup rwmatrix
 * \ref RwMatrixSetFreeListCreateParams allows the developer to specify
 * how many \ref RwMatrix s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwMatrixSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RwMatrixSetFreeListCreateParams"));
#endif
    _rwMatrixFreeListBlockSize = blockSize;
    _rwMatrixFreeListPreallocBlocks = numBlocksToPrealloc;

#if 0
    RWRETURNVOID();
#endif
}


/****************************************************************************
 _rwMatrixOpen

 On entry   :
 On exit    : FALSE on error
 */

void *
_rwMatrixOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwMatrixOpen"));

    /* Save data block offset (same for all instances) */
    matrixModule.globalsOffset = offset;

    /* We just happen to know that the size is a mutiple of 16 bytes, so
     * this doesn't hurt on other platforms. :-)
     */
    RWMATRIXGLOBAL(matrixFreeList) =
        RwFreeListCreateAndPreallocateSpace(sizeof(RwMatrix), _rwMatrixFreeListBlockSize, rwMATRIXALIGNMENT,
        _rwMatrixFreeListPreallocBlocks, &_rwMatrixFreeList, rwMEMHINTDUR_GLOBAL | rwID_MATRIX);
    if (!RWMATRIXGLOBAL(matrixFreeList))
    {
        /* Failure */
        instance = NULL;
    }
    else
    {
        const RwMatrixTolerance tolerance =
            {
                RWMATRIXOPTIMIZETOLERANCE,
                RWMATRIXOPTIMIZETOLERANCE,
                RWMATRIXOPTIMIZETOLERANCE
            };

        /* Set default optimizations */
        _rwMatrixSetOptimizations(rwMATRIXOPTIMIZE_IDENTITY);
        /* Set default multiply function */
        _rwMatrixSetMultFn((rwMatrixMultFn) NULL);
        /* Set default tolerances */
        RwEngineSetMatrixTolerances(&tolerance);

        /* One more module instance */
        matrixModule.numInstances++;
    }

    /* Success */
    RWRETURN(instance);
}

/**
 * \ingroup rwmatrix
 * \ref RwEngineGetMatrixTolerances
 * Returns current tolerances for
 * matrix optimizations with \ref RwMatrixOptimize
 *
 * \param tolerance The destination address to which tolerances are to be written
 *
 * \return Returns TRUE if successful, FALSE if there is an error
 *
 * \see RwEngineSetMatrixTolerances
 * \see RwMatrixOptimize
 */
RwBool
RwEngineGetMatrixTolerances(RwMatrixTolerance * const tolerance)
{
    const RwBool        result = TRUE;
    const RwMatrixTolerance *const source =
        &(RWMATRIXGLOBAL(tolerance));

    RWAPIFUNCTION(RWSTRING("RwEngineGetMatrixTolerances"));

    RWASSERT(tolerance);
    RWASSERT(source);

    *tolerance = *source;

    RWRETURN(result);
}

/**
 * \ingroup rwmatrix
 * \ref RwEngineSetMatrixTolerances
 * Sets current tolerances for
 * matrix optimizations with \ref RwMatrixOptimize
 *
 * This function must be used after the RenderWare engine has been started;
 * a call to RwEngineStart will nullify the effect of any previous call to
 * RwEngineSetMatrixTolerance.
 *
 * \param tolerance The source address from which tolerances are to be read
 *
 * \return Returns TRUE if successful, FALSE if there is an error
 *
 * \see RwEngineGetMatrixTolerances
 * \see RwMatrixOptimize
 */
RwBool
RwEngineSetMatrixTolerances(const RwMatrixTolerance * const tolerance)
{
    const RwBool        result = TRUE;
    RwMatrixTolerance  *const dest = &(RWMATRIXGLOBAL(tolerance));

    RWAPIFUNCTION(RWSTRING("RwEngineSetMatrixTolerances"));

    RWASSERT(tolerance);
    RWASSERT(dest);

    *dest = *tolerance;

    RWRETURN(result);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixOptimize informs the library that the application has
 * changed the specified matrix and that the library should take note of the
 * new matrix values.
 *
 * This is only necessary when the application directly
 * modifies the matrix with
 * \ref RwMatrixGetRight,
 * \ref RwMatrixGetUp,
 * \ref RwMatrixGetAt or
 * \ref RwMatrixGetPos.
 *
 * This differs from \ref RwMatrixUpdate in that extra work is undertaken
 * to identify special properties of the matrix suitable for subsequent
 * optimizations.
 *
 * \param matrix  A pointer to the matrix to update.
 * \param tolerance A pointer to the tolerances for optimizations or NULL
 *                  if the current global tolerances as returned by
 *                  \ref RwEngineGetMatrixTolerances are applied
 *
 * \return Returns a pointer to the optimized matrix
 *
 * \see RwEngineGetMatrixTolerances
 * \see RwEngineSetMatrixTolerances
 * \see RwMatrixGetAt
 * \see RwMatrixGetPos
 * \see RwMatrixGetRight
 * \see RwMatrixGetUp
 * \see RwMatrixUpdate
 */
RwMatrix *
RwMatrixOptimize(RwMatrix * matrix, const RwMatrixTolerance * tolerance)
{
    RwUInt32            flags;
    RwBool              MatrixIsNormal;
    RwBool              MatrixIsOrthogonal;
    RwBool              MatrixIsIdentity;

    RWAPIFUNCTION(RWSTRING("RwMatrixOptimize"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    if (((RwMatrixTolerance *) NULL) == tolerance)
    {
        tolerance = &(RWMATRIXGLOBAL(tolerance));
    }

    /* Identify special properties of the matrix
     * suitable for subsequent optimizations
     */

    MatrixIsNormal =
        (tolerance->Normal >= _rwMatrixNormalError(matrix));
    MatrixIsOrthogonal =
        (tolerance->Orthogonal >= _rwMatrixOrthogonalError(matrix));
    MatrixIsIdentity =
        (MatrixIsNormal &&
         MatrixIsOrthogonal &&
         ( tolerance->Identity >= _rwMatrixIdentityError(matrix)));

    /* Optimize the matrix flags */

    flags = rwMatrixGetFlags(matrix);

    if (MatrixIsNormal)
        flags |= rwMATRIXTYPENORMAL;
    else
        flags &= ~rwMATRIXTYPENORMAL;

    if (MatrixIsOrthogonal)
        flags |= rwMATRIXTYPEORTHOGONAL;
    else
        flags &= ~rwMATRIXTYPEORTHOGONAL;

    if (MatrixIsIdentity)
        flags |= rwMATRIXINTERNALIDENTITY;
    else
        flags &= ~rwMATRIXINTERNALIDENTITY;

    rwMatrixSetFlags(matrix, flags);

    RWRETURN(matrix);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixUpdate informs the library that the application has
 * changed the specified matrix and that the library should take note of the
 * new matrix values. This is only necessary when the application directly
 * modifies the matrix with
 * \ref RwMatrixGetRight,
 * \ref RwMatrixGetUp,
 * \ref RwMatrixGetAt or
 * \ref RwMatrixGetPos.
 *
 * \param matrix  A pointer to the matrix to update
 *
 * \return Returns a pointer to the specified matrix or NULL if there is
 * an error.
 *
 * \see RwMatrixGetRight
 * \see RwMatrixGetUp
 * \see RwMatrixGetAt
 * \see RwMatrixGetPos
 * \see RwMatrixOptimize
 */
RwMatrix *
RwMatrixUpdate(RwMatrix * matrix)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixUpdate"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    /* Update the matrix flags */
    rwMatrixSetFlags(matrix,
                     rwMatrixGetFlags(matrix)
                     & ~(rwMATRIXTYPEORTHONORMAL |
                         rwMATRIXINTERNALIDENTITY));

    RWRETURN(matrix);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixMultiply multiplies two matrices in the order given.
 * \note The matrix used for the result must be different from
 * the input matrices.
 *
 * \param dst  A pointer to the matrix that will receive the matrix product
 * \param src1  A pointer to the first matrix
 * \param src2  A pointer to the second matrix
 *
 * \return Returns a pointer to the product matrix
 *
 * \see RwMatrixRotate
 * \see RwMatrixTranslate
 * \see RwMatrixScale
 * \see RwMatrixTransform
 * \see RwV3dTransformPoint
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVector
 * \see RwV3dTransformVectors
 */
RwMatrix *
RwMatrixMultiply(RwMatrix * dst,
                 const RwMatrix * src1, const RwMatrix * src2)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixMultiply"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(dst);
    RWASSERT(RWMATRIXALIGNMENT(dst));
    RWASSERT(src1);
    RWASSERT(RWMATRIXALIGNMENT(src1));
    RWASSERT(src2);
    RWASSERT(RWMATRIXALIGNMENT(src2));
    RWASSERT(dst != src1);
    RWASSERT(dst != src2);

    RwMatrixMultiplyMacro(dst, src1, src2);

    RWRETURN(dst);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixOrthoNormalize extracts the 'rigid-body'
 * transformation component from a matrix.
 *
 * Whilst RenderWare supports arbitrary 4x3 affine matrices, many
 * applications deal only in 'rigid-body' transformations comprising
 * rotation and translation without scaling or shearing. The 4x3 matrix
 * representing such a transformation has a special form of upper-left 3x3
 * sub-matrix known as an orthonormal matrix, characterized by its inverse
 * being equal to its transpose. Mathematically, this sub-matrix remains
 * orthonormal following any rigid-body transformation. Numerically,
 * however, after extended matrix composition some scale or shear factors
 * begin to accumulate due to rounding. To prevent the significant build-up
 * of such factors, \ref RwMatrixOrthoNormalize should be periodically applied
 * to ensure the matrix keeps its orthonormal form. The minimal satisfactory
 * frequency of orthonormalization will depend on the nature of the
 * application.
 *
 * Typically, a frequency of once every 128 frames is adequate.
 *
 * \param dst  A pointer to the matrix that will receive orthonormal form
 * \param src  A pointer to the matrix to be orthonormalized
 *
 * \return Returns A pointer to the orthonormalized matrix
 *
 * \see RwMatrixInvert
 * \see RwMatrixSetIdentity
 *
 */

RwMatrix *
RwMatrixOrthoNormalize(RwMatrix * dst, const RwMatrix * src)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixOrthoNormalize"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(dst);
    RWASSERT(RWMATRIXALIGNMENT(dst));
    RWASSERT(src);
    RWASSERT(RWMATRIXALIGNMENT(src));

    RWRETURN(MatrixOrthoNormalize(dst, src));
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixRotateOneMinusCosineSine builds a rotation matrix from
 * the given axis and applies it to the specified matrix. The angle of
 * rotation is supplied by giving values for 1-cos(theta) and sin(theta) where
 * theta is the angle of rotation. This function should be used in preference
 * to \ref RwMatrixRotate, where possible, as it is substantially faster. The
 * combine operation may be pre-concatenation, post-concatenation or
 * replacement.
 *
 * \param matrix  A pointer to the matrix to which to apply the rotation
 * \param unitAxis  A pointer to a unit vector describing the axis of rotation
 * \param oneMinusCosine  The value of 1-cos(theta), where theta is the angle of rotation
 * \param sine  The value of sin(theta)
 * \param combineOp  A combination operator that specifies how rotation is applied
 *
 * \return Returns a pointer to the updated matrix.
 *
 * \see RwMatrixRotate
 * \see RwMatrixTranslate
 * \see RwMatrixScale
 * \see RwMatrixTransform
 *
 */
RwMatrix *
RwMatrixRotateOneMinusCosineSine(RwMatrix * matrix,
                                 const RwV3d * unitAxis,
                                 RwReal oneMinusCosine,
                                 RwReal sine, RwOpCombineType combineOp)
{
    RwMatrix            mRotate;
    RwMatrix            mLocal;
    RwV3d               vLeading;
    RwV3d               vScaled;
    RwV3d               vCrossed;

    RWAPIFUNCTION(RWSTRING("RwMatrixRotateOneMinusCosineSine"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWASSERT(unitAxis);

    /*
     * Rotatation matrix, for s =sin(theta), c = cos(theta) around unit
     * axis [a_x, a_y, a_Z] is:
     *
     * [ [ 1, 0, 0, 0 ]     [ [ a_x^2 - 1,  a_y a_x,   a_z a_x,  0 ]               [ [  0,   a_z, -a_y,  0 ]
     * [ 0, 1, 0, 0 ]   +   [  a_x a_y,  a_y^2 - 1,  a_z a_y,  0 ]   * (1 - c) +   [ -a_z,  0,  a_x,   0 ]   * s
     * [ 0, 0, 1, 0 ]       [  a_x a_z,   a_y a_z,  a_z^2 - 1, 0 ]                 [ a_y, -a_x,   0,   0 ]
     * [ 0, 0, 0, 1 ] ]     [     0,         0,         0,     1 ] ]               [  0,    0,    0,   1 ] ]
     * =
     *
     * [ [ 1 + (a_x^2 - 1) (1 - c), a_x a_y(1 - c) + a_z s,  a_x a_z(1 - c) - a_y s,  0 ]
     * [ a_y a_x(1 - c) - a_z s,  1 + (a_y^2 - 1) (1 - c), a_y a_z(1 - c) + a_x s,  0 ]
     * [ a_z a_x(1 - c) + a_y s,  a_z a_y(1 - c) - a_x s,  1 + (a_z^2 - 1) (1 - c), 0 ]
     * [            0,                       0,                       0,            1 ] ]
     */

    /*
     * Common sub expressions
     */
    vLeading.x = (((RwReal)1) - (((unitAxis->x) * (unitAxis->x))));
    vLeading.y = (((RwReal)1) - (((unitAxis->y) * (unitAxis->y))));
    vLeading.z = (((RwReal)1) - (((unitAxis->z) * (unitAxis->z))));
    RwV3dScaleMacro(&vLeading, &vLeading, oneMinusCosine);
    vCrossed.x = ((unitAxis->y) * (unitAxis->z));
    vCrossed.y = ((unitAxis->z) * (unitAxis->x));
    vCrossed.z = ((unitAxis->x) * (unitAxis->y));
    RwV3dScaleMacro(&vCrossed, &vCrossed, oneMinusCosine);
    RwV3dScaleMacro(&vScaled, unitAxis, sine);
    /*
     * Rotate matrix proper
     */
    mRotate.right.x = (((RwReal)1) - (vLeading.x));
    mRotate.right.y = ((vCrossed.z) + (vScaled.z));
    mRotate.right.z = ((vCrossed.y) - (vScaled.y));
    mRotate.up.x = ((vCrossed.z) - (vScaled.z));
    mRotate.up.y = (((RwReal)1) - (vLeading.y));
    mRotate.up.z = ((vCrossed.x) + (vScaled.x));
    mRotate.at.x = ((vCrossed.y) + (vScaled.y));
    mRotate.at.y = ((vCrossed.x) - (vScaled.x));
    mRotate.at.z = (((RwReal)1) - (vLeading.z));
    mRotate.pos.x = ((RwReal)0);
    mRotate.pos.y = ((RwReal)0);
    mRotate.pos.z = ((RwReal)0);

    rwMatrixSetFlags(&mRotate, rwMATRIXTYPEORTHONORMAL);

    RWASSERT(rwMatrixIsNormal(&mRotate, _NORMAL_TOL) &&
             rwMatrixIsOrthogonal(&mRotate, _ORTHOGONAL_TOL) &&
             ((_DETERMINANT_TOL <= _rwMatrixDeterminant(&mRotate))));

    switch (combineOp)
    {
        case rwCOMBINEREPLACE:
            {
                RwMatrixCopy(matrix, &mRotate);
                break;
            }
        case rwCOMBINEPRECONCAT:
            {
                RwMatrixMultiply(&mLocal, &mRotate, matrix);
                RwMatrixCopy(matrix, &mLocal);
                break;
            }
        case rwCOMBINEPOSTCONCAT:
            {
                RwMatrixMultiply(&mLocal, matrix, &mRotate);
                RwMatrixCopy(matrix, &mLocal);
                break;
            }
        default:
            {
                RWERROR((E_RW_BADPARAM,
                         RWSTRING("Invalid combination type")));
                matrix = (RwMatrix *) NULL;
            }
    }

    RWRETURN(matrix);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixRotate builds a rotation matrix from the given axis
 * and angle of rotation and applies it to the specified matrix. The combine
 * operation may be pre-concatenation, post-concatenation or replacement.
 *
 * \param matrix  A pointer to the matrix to which to apply the rotation
 * \param axis  A pointer to the vector that specifies the axis of rotation
 * \param angle  An angle of rotation in degrees
 * \param combineOp  A combination operator that specifies how the rotation is applied
 *
 * \return Returns a pointer to the updated matrix.
 *
 * \see RwMatrixTranslate
 * \see RwMatrixScale
 * \see RwMatrixTransform
 * \see RwMatrixRotateOneMinusCosineSine
 *
 */
RwMatrix *
RwMatrixRotate(RwMatrix * matrix, const RwV3d * axis,
               RwReal angle, RwOpCombineType combineOp)
{
    RwV3d               unitAxis;
    RwReal              sinVal, oneMinusCosVal;
    RwReal              radians =
        ((angle) * ((RwReal) ((rwPI / 180.0))));
    RwReal              recip;

    RWAPIFUNCTION(RWSTRING("RwMatrixRotate"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWASSERT(axis);

    _rwV3dNormalizeMacro(recip, &unitAxis, axis);
    sinVal = (RwReal) ((RwSin(((radians)))));
    oneMinusCosVal = (((RwReal)1) - ((RwReal) ((RwCos(((radians)))))));
    RwMatrixRotateOneMinusCosineSine(matrix,
                                     &unitAxis,
                                     oneMinusCosVal, sinVal, combineOp);
    /* Above function already did state count thing */

    RWRETURN(matrix);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixInvert performs a matrix inversion calculation on the
 * input matrix. Note that the input and output arguments must not point to
 * the same matrix.
 *
 * \param dst  A pointer to the matrix where the inverse will be stored
 * \param src  A pointer to the matrix that is to be inverted
 *
 * \return Returns a pointer to the inverted matrix
 *
 */
RwMatrix *
RwMatrixInvert(RwMatrix * dst, const RwMatrix * src)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixInvert"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(dst);
    RWASSERT(RWMATRIXALIGNMENT(dst));
    RWASSERT(src);
    RWASSERT(RWMATRIXALIGNMENT(src));
    RWASSERT(dst != src);

    RwMatrixInvertMacro(dst, src);

    RWRETURN(dst);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixScale builds a scaling matrix from the given scale factors
 * and applies it to the specified matrix in place. The combine operation may
 * be pre-concatenation, post-concatenation or replacement.
 *
 * \param matrix     A pointer to the matrix to apply scaling to
 * \param scale      A pointer to the vector specifying the scale factors
 * \param combineOp  A combine operator specifiying how the scaling is to be
 *                   applied:
 * \li rwCOMBINEREPLACE     Replacement
 * \li rwCOMBINEPRECONCAT   Pre-concatenation
 * \li rwCOMBINEPOSTCONCAT  Post-concatenation
 *
 * \return Returns a pointer to the updated matrix.
 *
 * \see RwMatrixRotate
 * \see RwMatrixTranslate
 * \see RwMatrixTransform
 * \see RwV3dTransformPoint
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVector
 * \see RwV3dTransformVectors
 */
RwMatrix *
RwMatrixScale(RwMatrix * matrix, const RwV3d * scale,
              RwOpCombineType combineOp)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixScale"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWASSERT(scale);

    switch (combineOp)
    {
        case rwCOMBINEREPLACE:
            {
                RwMatrixSetIdentity(matrix);
                matrix->right.x = scale->x;
                matrix->up.y = scale->y;
                matrix->at.z = scale->z;
                break;
            }
        case rwCOMBINEPRECONCAT:
            {
                RwV3dScaleMacro(&matrix->right, &matrix->right,
                                scale->x);
                RwV3dScaleMacro(&matrix->up, &matrix->up, scale->y);
                RwV3dScaleMacro(&matrix->at, &matrix->at, scale->z);
                break;
            }
        case rwCOMBINEPOSTCONCAT:
            {
                matrix->right.x = ((matrix->right.x) * (scale->x));
                matrix->right.y = ((matrix->right.y) * (scale->y));
                matrix->right.z = ((matrix->right.z) * (scale->z));
                matrix->up.x = ((matrix->up.x) * (scale->x));
                matrix->up.y = ((matrix->up.y) * (scale->y));
                matrix->up.z = ((matrix->up.z) * (scale->z));
                matrix->at.x = ((matrix->at.x) * (scale->x));
                matrix->at.y = ((matrix->at.y) * (scale->y));
                matrix->at.z = ((matrix->at.z) * (scale->z));
                matrix->pos.x = ((matrix->pos.x) * (scale->x));
                matrix->pos.y = ((matrix->pos.y) * (scale->y));
                matrix->pos.z = ((matrix->pos.z) * (scale->z));
                break;
            }

        default:
            {
                RWERROR((E_RW_BADPARAM,
                         RWSTRING("Invalid combination type")));
                matrix = (RwMatrix *) NULL;
            }
    }

    /* Mark as dirty and it's not orthonormal any more */
    rwMatrixSetFlags(matrix,
                     rwMatrixGetFlags(matrix) &
                     ~(rwMATRIXINTERNALIDENTITY |
                       rwMATRIXTYPEORTHONORMAL));

    RWRETURN(matrix);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixTranslate builds a translation matrix from the given
 * vector and applies it to the specified matrix. The combine operation may
 * be pre-concatenation, post-concatenation or replacement.
 *
 * \param matrix  A pointer to the matrix to which to apply the translation
 * \param translation  A pointer to the vector that specifies the components of the translation
 * \param combineOp  A combination operator that specifies how the translation is to be applied
 *  \li  rwCOMBINEREPLACE    Replacement
 *  \li  rwCOMBINEPRECONCAT  Pre-concatenation
 *  \li  rwCOMBINEPOSTCONCAT Post-concatenation
 *
* \return Returns a pointer to the updated matrix.
 *
 * \see RwMatrixRotate
 * \see RwMatrixScale
 * \see RwMatrixTransform
 * \see RwV3dTransformPoint
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVector
 * \see RwV3dTransformVectors
 */
RwMatrix *
RwMatrixTranslate(RwMatrix * matrix,
                  const RwV3d * translation, RwOpCombineType combineOp)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixTranslate"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWASSERT(translation);

#if (0)
    if (!RWMATRIXALIGNMENT(matrix))
    {
        RWMESSAGE(("matrix %p rwMATRIXALIGNMENT %d  (((rwMATRIXALIGNMENT)-1) & ((RwUInt32)(matrix))) %d", matrix, rwMATRIXALIGNMENT, (((rwMATRIXALIGNMENT) - 1) & ((RwUInt32) (matrix)))));
    }
#endif /* (0) */

    switch (combineOp)
    {
        case rwCOMBINEREPLACE:
            {
                RwMatrixSetIdentity(matrix);
                matrix->pos.x = translation->x;
                matrix->pos.y = translation->y;
                matrix->pos.z = translation->z;
                break;
            }
        case rwCOMBINEPRECONCAT:
            {
                matrix->pos.x =
                    ((matrix->pos.x) +
                     (RwV3dDotDOWN((*translation), (*matrix), x)));
                matrix->pos.y =
                    ((matrix->pos.y) +
                     (RwV3dDotDOWN((*translation), (*matrix), y)));
                matrix->pos.z =
                    ((matrix->pos.z) +
                     (RwV3dDotDOWN((*translation), (*matrix), z)));
                break;
            }
        case rwCOMBINEPOSTCONCAT:
            {
                matrix->pos.x = ((matrix->pos.x) + (translation->x));
                matrix->pos.y = ((matrix->pos.y) + (translation->y));;
                matrix->pos.z = ((matrix->pos.z) + (translation->z));
                break;
            }
        default:
            {
                RWERROR((E_RW_BADPARAM,
                         RWSTRING("Invalid combination type")));
                matrix = (RwMatrix *) NULL;
            }
    }

    /* Mark as dirty */
    rwMatrixSetFlags(matrix,
                     rwMatrixGetFlags(matrix) &
                     ~rwMATRIXINTERNALIDENTITY);

    /* Done */
    RWRETURN(matrix);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixTransform calculates the result of applying a general
 * transformation matrix to the given matrix. The combine operation may be
 * pre-concatenation, post-concatenation or replacement.
 *
 * The two input matrices must not be the same.
 *
 * \param matrix  A pointer to the matrix to which to apply the transformation
 * \param transform  A pointer to the matrix which specifies the transformation
 * \param combineOp  A combination operator specifying how the transformation is to be applied
 * \li   rwCOMBINEREPLACE    Replacement
 * \li   rwCOMBINEPRECONCAT  Pre-concatenation
 * \li   rwCOMBINEPOSTCONCAT Post-concatenation.
 *
 * \return Returns a pointer to the updated matrix.
 *
 * \see RwMatrixRotate
 * \see RwMatrixTranslate
 * \see RwMatrixScale
 * \see RwV3dTransformPoint
 * \see RwV3dTransformPoints
 * \see RwV3dTransformVector
 * \see RwV3dTransformVectors
 */
RwMatrix *
RwMatrixTransform(RwMatrix *matrix,
                  const RwMatrix *transform,
                  RwOpCombineType combineOp)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixTransform"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWASSERT(transform);
    RWASSERT(matrix != transform);

    switch (combineOp)
    {
        case rwCOMBINEREPLACE:
            {
                RwMatrixCopy(matrix, transform);
                break;
            }
        case rwCOMBINEPRECONCAT:
            {
                RwMatrix    mTmp;

                RwMatrixMultiply(&mTmp, transform, matrix);
                RwMatrixCopy(matrix, &mTmp);
                break;
            }
        case rwCOMBINEPOSTCONCAT:
            {
                RwMatrix    mTmp;

                RwMatrixMultiply(&mTmp, matrix, transform);
                RwMatrixCopy(matrix, &mTmp);
                break;
            }
        default:
            {
                RWERROR((E_RW_BADPARAM,
                         RWSTRING("Invalid combination type")));
                matrix = (RwMatrix *) NULL;
            }
    }

    RWRETURN(matrix);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixQueryRotate determines the rotation component from a
 * matrix comprising only rotations and translations. The rotation is returned
 * as a unit vector along the axis of rotation, an angle of rotation and a
 * point on the axis which is the nearest to the origin (hence the line
 * joining this point with the origin is perpendicular to the axis).
 *
 * The rotation component has two possible descriptions since a rotation about
 * an axis of theta degrees is equivalent to a rotation about an axis pointing
 * in the opposite direction by an angle of 360-theta in the reverse direction.
 * The angle returned by \ref RwMatrixQueryRotate is always between 0 and 180
 * degrees and the direction of the axis of rotation returned is chosen to
 * ensure the angle is in this range.
 * \note Only matrices known to be
 * composed solely of rotations and translations, i.e. OrthoNormal or rigid-body
 * transformations, should be queried with this function. The results from
 * querying matrices incorporating transforms such as scaling are unlikely to
 * be useful.
 *
 * \param matrix  A pointer to the matrix to be investigated
 * \param unitAxis  A pointer to the vector which will receive the unit
 *  direction vector along axis of rotation
 * \param angle  A pointer to the \ref RwReal that will receive the angle of
 *  rotation in degrees
 * \param center  A pointer to the vector that will receive the center of the
 *  line of rotation
 *
 * \return Returns A pointer to the queried matrix
 *
 * \see RwMatrixRotate
 * \see RwMatrixRotateOneMinusCosineSine
 *
 */
const RwMatrix *
RwMatrixQueryRotate(const RwMatrix * matrix, RwV3d * unitAxis,
                    RwReal * angle, RwV3d * center)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixQueryRotate"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWASSERT(unitAxis);
    RWASSERT(angle);
    RWASSERT(center);

    RWRETURN(MatrixQueryRotate(matrix, unitAxis, angle, center));
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixDestroy is used to delete a transformation matrix previously
 * created by the API function \ref RwMatrixCreate matrix.
 *
 * \param matrix  A pointer to the matrix that is to be destroyed
 *
 * \return Returns TRUE
 *
 * \see RwMatrixCreate
 *
 */
RwBool
RwMatrixDestroy(RwMatrix * matrix)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixDestroy"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    RwFreeListFree(RWMATRIXGLOBAL(matrixFreeList), matrix);

    RWRETURN(TRUE);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixCreate creates a new transformation matrix initialized to
 * the identity matrix.  Note that the matrix must be destroyed when it is no
 * longer required by an application.
 *
 * \return Returns a pointer to created matrix if successful or NULL if
 * there is an error
 *
 * \see RwMatrixCopy
 * \see RwMatrixDestroy
 *
 */
RwMatrix *
RwMatrixCreate(void)
{
    RwMatrix           *matrix = (RwMatrix *) NULL;

    RWAPIFUNCTION(RWSTRING("RwMatrixCreate"));
    RWASSERT(matrixModule.numInstances);

    matrix =
        (RwMatrix *) RwFreeListAlloc(RWMATRIXGLOBAL(matrixFreeList),
                                     rwMEMHINTDUR_EVENT | rwID_MATRIX);
    RWASSERT(RWFREELISTALIGNED(matrix, RWMATRIXGLOBAL(matrixFreeList)));
    if (matrix)
    {
        rwMatrixInitializeIdentity(matrix, rwMATRIXTYPEORTHONORMAL);
    }

    RWASSERT(RWMATRIXALIGNMENT(matrix));
    RWRETURN(matrix);
}

#ifdef RWDEBUG

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Accessing parts of a matrix

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/**
 * \ingroup rwmatrix
 * \ref RwMatrixGetRight is used to retrieve the 'look-right' vector
 * of the specified transformation matrix.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param matrix  A pointer to the matrix from which to get 'look-right' vector
 *
 * \return Returns a pointer to the 'look-right' vector of the matrix
 *
 * \see RwMatrixGetUp
 * \see RwMatrixGetAt
 * \see RwMatrixGetPos
 * \see RwMatrixUpdate
 * \see RwMatrixOptimize
 */
RwV3d *
RwMatrixGetRight(RwMatrix * matrix)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixGetRight"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    RWRETURN(&matrix->right);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixGetUp is used to retrieve the 'look-up' vector of
 * the specified transformation matrix.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param matrix  A pointer to the matrix from which to get 'look-up' vector
 *
 * \return Returns a pointer to the 'look-up' vector of the matrix
 *
 * \see RwMatrixGetRight
 * \see RwMatrixGetAt
 * \see RwMatrixGetPos
 * \see RwMatrixUpdate
 * \see RwMatrixOptimize
 */
RwV3d *
RwMatrixGetUp(RwMatrix * matrix)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixGetUp"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    RWRETURN(&matrix->up);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixGetAt is used to retrieve the 'look-at' vector of the
 * specified transformation matrix.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param matrix  A pointer to the matrix from which to get the 'look-at' vector
 *
 * \return Returns a pointer to the 'look-at' vector
 *
 * \see RwMatrixGetRight
 * \see RwMatrixGetUp
 * \see RwMatrixGetPos
 * \see RwMatrixUpdate
 * \see RwMatrixOptimize
 */
RwV3d *
RwMatrixGetAt(RwMatrix * matrix)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixGetAt"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    RWRETURN(&matrix->at);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixGetPos is used to retrieve the 'position' vector from
 * the specified transformation matrix.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param matrix  A pointer to the matrix from which to get the 'position' vector
 *
 * \return Returns a pointer to the 'position' vector of the matrix
 *
 * \see RwMatrixGetRight
 * \see RwMatrixGetUp
 * \see RwMatrixGetAt
 * \see RwMatrixUpdate
 * \see RwMatrixOptimize
 */
RwV3d *
RwMatrixGetPos(RwMatrix * matrix)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixGetPos"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    RWRETURN(&matrix->pos);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixCopy is used to copy the contents of one matrix to
 * another. Both matrices must exist or have been created using
 * \ref RwMatrixCreate before calling this function.
 *
 * \note This function is used for debug purposes only and,
 * for efficiency, is available as a macro for final release versions
 * of an application.
 *
 * \param dst  A pointer to the destination matrix
 * \param src  A pointer to the source matrix
 *
 * \return None.
 *
 * \see RwMatrixCreate
 * \see RwMatrixMultiply
 * \see RwMatrixInvert
 * \see RwMatrixRotate
 * \see RwMatrixScale
 * \see RwMatrixTranslate
 * \see RwMatrixTransform
 *
 */
void
RwMatrixCopy(RwMatrix * dst, const RwMatrix * src)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixCopy"));

    RWASSERT(matrixModule.numInstances);

    RWASSERT(dst);
    RWASSERT(RWMATRIXALIGNMENT(dst));
    RWASSERT(src);
    RWASSERT(RWMATRIXALIGNMENT(src));

    /* Copy the matrix type */
    RwMatrixCopyMacro(dst, src);

    RWRETURNVOID();
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixSetIdentity is used to convert the specified matrix to
 * the identity matrix.
 *
 * \note This function is used for debug purposes only and, for efficiency,
 * is available as a macro for final release versions of an application.
 *
 * \param matrix  A pointer to the matrix to reset to the identity matrix.
 *
 * \return None
 *
 * \see RwMatrixCreate
 *
 */
void
RwMatrixSetIdentity(RwMatrix * matrix)
{
    const RwInt32 flags = (rwMATRIXINTERNALIDENTITY | rwMATRIXTYPEORTHONORMAL);
    RWAPIFUNCTION(RWSTRING("RwMatrixSetIdentity"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    matrix->right.x = matrix->up.y    = matrix->at.z  = ((RwReal)1);
    matrix->right.y = matrix->right.z = matrix->up.x  = ((RwReal)0);
    matrix->up.z    = matrix->at.x    = matrix->at.y  = ((RwReal)0);
    matrix->pos.x   = matrix->pos.y   = matrix->pos.z = ((RwReal)0);
    rwMatrixSetFlags(matrix, rwMatrixGetFlags(matrix) | flags);

    /*
     * RWASSERT(rwMatrixIsIdentity(matrix, _IDENTITY_TOL));
     */

    RWRETURNVOID();
}

#endif /* RWDEBUG */
