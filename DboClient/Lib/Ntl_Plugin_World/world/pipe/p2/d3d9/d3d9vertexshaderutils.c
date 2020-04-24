/****************************************************************************
 *                                                                          *
 * module : d3d9vertexshaderutils.c                                         *
 *                                                                          *
 * purpose:                                                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#define USE_D3DX

/* D3D9 Libraries */
#include <d3d9.h>

#ifdef USE_D3DX
#include <d3dx9.h>
#endif

#include "rwcore.h"
#include "baworld.h"

#include "../../../../driver/d3d9/d3d9device.h"

#include "d3d9vertexshaderutils.h"

/****************************************************************************
 Global variables
 */

/****************************************************************************
 local defines
 */

#if (defined(__GNUC__) && defined(__cplusplus))
#define D3DMatrixInitMacro(_XX, _XY, _XZ, _XW,  \
                           _YX, _YY, _YZ, _YW,  \
                           _ZX, _ZY, _ZZ, _ZW,  \
                           _WX, _WY, _WZ, _WW ) \
  {                                             \
    { { {   (_XX), (_XY), (_XZ), (_XW) },       \
        {   (_YX), (_YY), (_YZ), (_YW) },       \
        {   (_ZX), (_ZY), (_ZZ), (_ZW) },       \
        {   (_WX), (_WY), (_WZ), (_WW) }        \
    } }                                         \
  }
#endif /* (defined(__GNUC__) && defined(__cplusplus)) */


#if (!defined(D3DMatrixInitMacro))
#define D3DMatrixInitMacro(_XX, _XY, _XZ, _XW,  \
                           _YX, _YY, _YZ, _YW,  \
                           _ZX, _ZY, _ZZ, _ZW,  \
                           _WX, _WY, _WZ, _WW ) \
  {                                             \
     (_XX), (_XY), (_XZ), (_XW),                \
     (_YX), (_YY), (_YZ), (_YW),                \
     (_ZX), (_ZY), (_ZZ), (_ZW),                \
     (_WX), (_WY), (_WZ), (_WW)                 \
  }
#endif /* (!defined(D3DMatrixInitMacro)) */

/****************************************************************************
 local types
 */

/****************************************************************************
 Static variables
 */
static RWD3D9_ALIGN16 D3DMATRIX D3D9ViewProjTransform = {0};

static const RwMatrix *D3D9ActiveTransform = NULL;
static const RwMatrix *D3D9ActiveInverseTransform = NULL;
static const RwMatrix *D3D9ActiveInverseNormalizedTransform = NULL;
static const RwMatrix *D3D9ActiveNormalizedTransform = NULL;

static RWD3D9_ALIGN16 D3DMATRIX   D3D9Transform =
    D3DMatrixInitMacro(1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f);

static RWD3D9_ALIGN16 RwMatrix    D3D9InverseTransform =
{
    1.0f, 0.0f, 0.0f, 0,
    0.0f, 1.0f, 0.0f, 0,
    0.0f, 0.0f, 1.0f, 0,
    0.0f, 0.0f, 0.0f, 0x3f800000
};

static RWD3D9_ALIGN16 RwMatrix    D3D9InverseNormalizedTransform =
{
    1.0f, 0.0f, 0.0f, 0,
    0.0f, 1.0f, 0.0f, 0,
    0.0f, 0.0f, 1.0f, 0,
    0.0f, 0.0f, 0.0f, 0x3f800000
};

static RWD3D9_ALIGN16 RwMatrix    D3D9NormalizedTransform =
{
    1.0f, 0.0f, 0.0f, 0,
    0.0f, 1.0f, 0.0f, 0,
    0.0f, 0.0f, 1.0f, 0,
    0.0f, 0.0f, 0.0f, 0x3f800000
};

static const RWD3D9_ALIGN16 RwMatrix D3D9IdentityMatrix =
{
    1.0f, 0.0f, 0.0f, rwMATRIXINTERNALIDENTITY | rwMATRIXTYPEORTHONORMAL,
    0.0f, 1.0f, 0.0f, 0,
    0.0f, 0.0f, 1.0f, 0,
    0.0f, 0.0f, 0.0f, 0x3f800000
};

/****************************************************************************
 Functions
 */
#ifndef USE_D3DX

/****************************************************************************
 D3D9MatrixMultiply

 On entry   : Dest matrix pointer, two source matrix pointers
 On exit    : Matrix pointer contains result
 */
static void
D3D9MatrixMultiply(D3DMATRIX *dstMat,
                   const D3DMATRIX *matA,
                   const D3DMATRIX *matB)
{
    RWFUNCTION(RWSTRING("D3D9MatrixMultiply"));
    RWASSERT(dstMat != NULL);
    RWASSERT(matA != NULL);
    RWASSERT(matB != NULL);

    /* Multiply out right */
    dstMat->m[0][0] =
        (matA->m[0][0] * matB->m[0][0]) +
        (matA->m[0][1] * matB->m[1][0]) +
        (matA->m[0][2] * matB->m[2][0]) +
        (matA->m[0][3] * matB->m[3][0]);
    dstMat->m[0][1] =
        (matA->m[0][0] * matB->m[0][1]) +
        (matA->m[0][1] * matB->m[1][1]) +
        (matA->m[0][2] * matB->m[2][1]) +
        (matA->m[0][3] * matB->m[3][1]);
    dstMat->m[0][2] =
        (matA->m[0][0] * matB->m[0][2]) +
        (matA->m[0][1] * matB->m[1][2]) +
        (matA->m[0][2] * matB->m[2][2]) +
        (matA->m[0][3] * matB->m[3][2]);
    dstMat->m[0][3] =
        (matA->m[0][0] * matB->m[0][3]) +
        (matA->m[0][1] * matB->m[1][3]) +
        (matA->m[0][2] * matB->m[2][3]) +
        (matA->m[0][3] * matB->m[3][3]);

    /* Then up */
    dstMat->m[1][0] =
        (matA->m[1][0] * matB->m[0][0]) +
        (matA->m[1][1] * matB->m[1][0]) +
        (matA->m[1][2] * matB->m[2][0]) +
        (matA->m[1][3] * matB->m[3][0]);
    dstMat->m[1][1] =
        (matA->m[1][0] * matB->m[0][1]) +
        (matA->m[1][1] * matB->m[1][1]) +
        (matA->m[1][2] * matB->m[2][1]) +
        (matA->m[1][3] * matB->m[3][1]);
    dstMat->m[1][2] =
        (matA->m[1][0] * matB->m[0][2]) +
        (matA->m[1][1] * matB->m[1][2]) +
        (matA->m[1][2] * matB->m[2][2]) +
        (matA->m[1][3] * matB->m[3][2]);
    dstMat->m[1][3] =
        (matA->m[1][0] * matB->m[0][3]) +
        (matA->m[1][1] * matB->m[1][3]) +
        (matA->m[1][2] * matB->m[2][3]) +
        (matA->m[1][3] * matB->m[3][3]);

    /* Then at */
    dstMat->m[2][0] =
        (matA->m[2][0] * matB->m[0][0]) +
        (matA->m[2][1] * matB->m[1][0]) +
        (matA->m[2][2] * matB->m[2][0]) +
        (matA->m[2][3] * matB->m[3][0]);
    dstMat->m[2][1] =
        (matA->m[2][0] * matB->m[0][1]) +
        (matA->m[2][1] * matB->m[1][1]) +
        (matA->m[2][2] * matB->m[2][1]) +
        (matA->m[2][3] * matB->m[3][1]);
    dstMat->m[2][2] =
        (matA->m[2][0] * matB->m[0][2]) +
        (matA->m[2][1] * matB->m[1][2]) +
        (matA->m[2][2] * matB->m[2][2]) +
        (matA->m[2][3] * matB->m[3][2]);
    dstMat->m[2][3] =
        (matA->m[2][0] * matB->m[0][3]) +
        (matA->m[2][1] * matB->m[1][3]) +
        (matA->m[2][2] * matB->m[2][3]) +
        (matA->m[2][3] * matB->m[3][3]);

    /* Then pos */
    dstMat->m[3][0] =
        (matA->m[3][0] * matB->m[0][0]) +
        (matA->m[3][1] * matB->m[1][0]) +
        (matA->m[3][2] * matB->m[2][0]) +
        (matA->m[3][3] * matB->m[3][0]);
    dstMat->m[3][1] =
        (matA->m[3][0] * matB->m[0][1]) +
        (matA->m[3][1] * matB->m[1][1]) +
        (matA->m[3][2] * matB->m[2][1]) +
        (matA->m[3][3] * matB->m[3][1]);
    dstMat->m[3][2] =
        (matA->m[3][0] * matB->m[0][2]) +
        (matA->m[3][1] * matB->m[1][2]) +
        (matA->m[3][2] * matB->m[2][2]) +
        (matA->m[3][3] * matB->m[3][2]);
    dstMat->m[3][3] =
        (matA->m[3][0] * matB->m[0][3]) +
        (matA->m[3][1] * matB->m[1][3]) +
        (matA->m[3][2] * matB->m[2][3]) +
        (matA->m[3][3] * matB->m[3][3]);

    /* And that's all folks */
    RWRETURNVOID();
}

/****************************************************************************
 D3D9MatrixMultiplyTranspose

 On entry   : Dest matrix pointer, two source matrix pointers
 On exit    : Matrix pointer contains transposed result
 */
static void
D3D9MatrixMultiplyTranspose(D3DMATRIX *dstMat,
                            const D3DMATRIX *matA,
                            const D3DMATRIX *matB)
{
    RWFUNCTION(RWSTRING("D3D9MatrixMultiplyTranspose"));
    RWASSERT(dstMat != NULL);
    RWASSERT(matA != NULL);
    RWASSERT(matB != NULL);

    /* Multiply out right */
    dstMat->m[0][0] =
        (matA->m[0][0] * matB->m[0][0]) +
        (matA->m[0][1] * matB->m[1][0]) +
        (matA->m[0][2] * matB->m[2][0]) +
        (matA->m[0][3] * matB->m[3][0]);
    dstMat->m[1][0] =
        (matA->m[0][0] * matB->m[0][1]) +
        (matA->m[0][1] * matB->m[1][1]) +
        (matA->m[0][2] * matB->m[2][1]) +
        (matA->m[0][3] * matB->m[3][1]);
    dstMat->m[2][0] =
        (matA->m[0][0] * matB->m[0][2]) +
        (matA->m[0][1] * matB->m[1][2]) +
        (matA->m[0][2] * matB->m[2][2]) +
        (matA->m[0][3] * matB->m[3][2]);
    dstMat->m[3][0] =
        (matA->m[0][0] * matB->m[0][3]) +
        (matA->m[0][1] * matB->m[1][3]) +
        (matA->m[0][2] * matB->m[2][3]) +
        (matA->m[0][3] * matB->m[3][3]);

    /* Then up */
    dstMat->m[0][1] =
        (matA->m[1][0] * matB->m[0][0]) +
        (matA->m[1][1] * matB->m[1][0]) +
        (matA->m[1][2] * matB->m[2][0]) +
        (matA->m[1][3] * matB->m[3][0]);
    dstMat->m[1][1] =
        (matA->m[1][0] * matB->m[0][1]) +
        (matA->m[1][1] * matB->m[1][1]) +
        (matA->m[1][2] * matB->m[2][1]) +
        (matA->m[1][3] * matB->m[3][1]);
    dstMat->m[2][1] =
        (matA->m[1][0] * matB->m[0][2]) +
        (matA->m[1][1] * matB->m[1][2]) +
        (matA->m[1][2] * matB->m[2][2]) +
        (matA->m[1][3] * matB->m[3][2]);
    dstMat->m[3][1] =
        (matA->m[1][0] * matB->m[0][3]) +
        (matA->m[1][1] * matB->m[1][3]) +
        (matA->m[1][2] * matB->m[2][3]) +
        (matA->m[1][3] * matB->m[3][3]);

    /* Then at */
    dstMat->m[0][2] =
        (matA->m[2][0] * matB->m[0][0]) +
        (matA->m[2][1] * matB->m[1][0]) +
        (matA->m[2][2] * matB->m[2][0]) +
        (matA->m[2][3] * matB->m[3][0]);
    dstMat->m[1][2] =
        (matA->m[2][0] * matB->m[0][1]) +
        (matA->m[2][1] * matB->m[1][1]) +
        (matA->m[2][2] * matB->m[2][1]) +
        (matA->m[2][3] * matB->m[3][1]);
    dstMat->m[2][2] =
        (matA->m[2][0] * matB->m[0][2]) +
        (matA->m[2][1] * matB->m[1][2]) +
        (matA->m[2][2] * matB->m[2][2]) +
        (matA->m[2][3] * matB->m[3][2]);
    dstMat->m[3][2] =
        (matA->m[2][0] * matB->m[0][3]) +
        (matA->m[2][1] * matB->m[1][3]) +
        (matA->m[2][2] * matB->m[2][3]) +
        (matA->m[2][3] * matB->m[3][3]);

    /* Then pos */
    dstMat->m[0][3] =
        (matA->m[3][0] * matB->m[0][0]) +
        (matA->m[3][1] * matB->m[1][0]) +
        (matA->m[3][2] * matB->m[2][0]) +
        (matA->m[3][3] * matB->m[3][0]);
    dstMat->m[1][3] =
        (matA->m[3][0] * matB->m[0][1]) +
        (matA->m[3][1] * matB->m[1][1]) +
        (matA->m[3][2] * matB->m[2][1]) +
        (matA->m[3][3] * matB->m[3][1]);
    dstMat->m[2][3] =
        (matA->m[3][0] * matB->m[0][2]) +
        (matA->m[3][1] * matB->m[1][2]) +
        (matA->m[3][2] * matB->m[2][2]) +
        (matA->m[3][3] * matB->m[3][2]);
    dstMat->m[3][3] =
        (matA->m[3][0] * matB->m[0][3]) +
        (matA->m[3][1] * matB->m[1][3]) +
        (matA->m[3][2] * matB->m[2][3]) +
        (matA->m[3][3] * matB->m[3][3]);

    /* And that's all folks */
    RWRETURNVOID();
}

/****************************************************************************
 D3D9MatrixTranspose

 On entry   : Dest matrix pointer, one source matrix pointers
 On exit    : Matrix pointer contains transposed result
 */
static void
D3D9MatrixTranspose(D3DMATRIX *dstMat,
                    const D3DMATRIX *srcmat)
{
    RWFUNCTION(RWSTRING("D3D9MatrixTranspose"));
    RWASSERT(dstMat != NULL);
    RWASSERT(srcmat != NULL);

    /* Multiply out right */
    dstMat->m[0][0] = srcmat->m[0][0];
    dstMat->m[1][0] = srcmat->m[0][1];
    dstMat->m[2][0] = srcmat->m[0][2];
    dstMat->m[3][0] = srcmat->m[0][3];

    /* Then up */
    dstMat->m[0][1] = srcmat->m[1][0];
    dstMat->m[1][1] = srcmat->m[1][1];
    dstMat->m[2][1] = srcmat->m[1][2];
    dstMat->m[3][1] = srcmat->m[1][3];

    /* Then at */
    dstMat->m[0][2] = srcmat->m[2][0];
    dstMat->m[1][2] = srcmat->m[2][1];
    dstMat->m[2][2] = srcmat->m[2][2];
    dstMat->m[3][2] = srcmat->m[2][3];

    /* Then pos */
    dstMat->m[0][3] = srcmat->m[3][0];
    dstMat->m[1][3] = srcmat->m[3][1];
    dstMat->m[2][3] = srcmat->m[3][2];
    dstMat->m[3][3] = srcmat->m[3][3];

    /* And that's all folks */
    RWRETURNVOID();
}
#endif

/****************************************************************************
 *
 * A faster matrix inversion function for orthonormal matrices
 * Tranposition
 *
 */
static void
D3D9MatrixInvertOrthoNormalized(RwMatrix *dst,
                                const RwMatrix *src)
{
    RWFUNCTION(RWSTRING("D3D9MatrixInvertOrthoNormalized"));
    RWASSERT(dst);
    RWASSERT(RWMATRIXALIGNMENT(dst));
    RWASSERT(src);
    RWASSERT(RWMATRIXALIGNMENT(src));
    RWASSERT(rwMatrixIsNormal(src, 0.01f) && rwMatrixIsOrthogonal(src, 0.01f));
    /*
     * Inverse of upper left 3x3 sub matrix
     * is a simple transpose
     */
    /*
     * calculate translation componennt of inverse
     */
    dst->pos.x = -RwV3dDotProductMacro(&src->pos, &src->right);
    dst->right.x = src->right.x;
    dst->up.x = src->right.y;
    dst->at.x = src->right.z;

    dst->pos.y = -RwV3dDotProductMacro(&src->pos, &src->up);
    dst->right.y = src->up.x;
    dst->up.y = src->up.y;
    dst->at.y = src->up.z;

    dst->pos.z = -RwV3dDotProductMacro(&src->pos, &src->at);
    dst->right.z = src->at.x;
    dst->up.z = src->at.y;
    dst->at.z = src->at.z;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSSetActiveWorldMatrix

*/
void
_rwD3D9VSSetActiveWorldMatrix(const RwMatrix *worldMatrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSSetActiveWorldMatrix"));

    D3D9ActiveTransform = worldMatrix;
    if (D3D9ActiveTransform != NULL)
    {
        D3D9Transform.m[0][0] = D3D9ActiveTransform->right.x;
        D3D9Transform.m[0][1] = D3D9ActiveTransform->right.y;
        D3D9Transform.m[0][2] = D3D9ActiveTransform->right.z;

        D3D9Transform.m[1][0] = D3D9ActiveTransform->up.x;
        D3D9Transform.m[1][1] = D3D9ActiveTransform->up.y;
        D3D9Transform.m[1][2] = D3D9ActiveTransform->up.z;

        D3D9Transform.m[2][0] = D3D9ActiveTransform->at.x;
        D3D9Transform.m[2][1] = D3D9ActiveTransform->at.y;
        D3D9Transform.m[2][2] = D3D9ActiveTransform->at.z;

        D3D9Transform.m[3][0] = D3D9ActiveTransform->pos.x;
        D3D9Transform.m[3][1] = D3D9ActiveTransform->pos.y;
        D3D9Transform.m[3][2] = D3D9ActiveTransform->pos.z;
    }

    D3D9ActiveInverseTransform = NULL;
    D3D9ActiveInverseNormalizedTransform = NULL;
    D3D9ActiveNormalizedTransform = NULL;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetComposedTransformMatrix

*/
void
_rwD3D9VSGetComposedTransformMatrix(void *transformMatrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetComposedTransformMatrix"));

    if (_RwD3D9ActiveViewProjTransform == NULL)
    {
        #ifdef USE_D3DX
        D3DXMatrixMultiply(&D3D9ViewProjTransform, &_RwD3D9D3D9ViewTransform, &_RwD3D9D3D9ProjTransform);
        #else
        D3D9MatrixMultiply(&D3D9ViewProjTransform, &_RwD3D9D3D9ViewTransform, &_RwD3D9D3D9ProjTransform);
        #endif

        _RwD3D9ActiveViewProjTransform = &D3D9ViewProjTransform;
    }

    if (D3D9ActiveTransform != NULL)
    {
        #ifdef USE_D3DX
        D3DXMatrixMultiplyTranspose(transformMatrix, &D3D9Transform, _RwD3D9ActiveViewProjTransform);
        #else
        D3D9MatrixMultiplyTranspose(transformMatrix, &D3D9Transform, _RwD3D9ActiveViewProjTransform);
        #endif
    }
    else
    {
        #ifdef USE_D3DX
        D3DXMatrixTranspose(transformMatrix, _RwD3D9ActiveViewProjTransform);
        #else
        D3D9MatrixTranspose(transformMatrix, _RwD3D9ActiveViewProjTransform);
        #endif
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetWorldViewTransposedMatrix

*/
void
_rwD3D9VSGetWorldViewTransposedMatrix(void *worldViewMatrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetWorldViewTransposedMatrix"));
    RWASSERT(worldViewMatrix != NULL);

    if (D3D9ActiveTransform != NULL)
    {
        #ifdef USE_D3DX
        D3DXMatrixMultiplyTranspose(worldViewMatrix, &D3D9Transform, &_RwD3D9D3D9ViewTransform);
        #else
        D3D9MatrixMultiplyTranspose(worldViewMatrix, &D3D9Transform, &_RwD3D9D3D9ViewTransform);
        #endif
    }
    else
    {
        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldViewMatrix, &_RwD3D9D3D9ViewTransform);
        #else
        D3D9MatrixTranspose(worldViewMatrix, &_RwD3D9D3D9ViewTransform);
        #endif
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetWorldViewMatrix

*/
void
_rwD3D9VSGetWorldViewMatrix(void *worldViewMatrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetWorldViewMatrix"));
    RWASSERT(worldViewMatrix != NULL);

    if (D3D9ActiveTransform != NULL)
    {
        RwMatrixMultiply(worldViewMatrix, D3D9ActiveTransform, (RwMatrix *)&_RwD3D9D3D9ViewTransform);

        ((D3DMATRIX *)worldViewMatrix)->_14 = 0.0f;
        ((D3DMATRIX *)worldViewMatrix)->_24 = 0.0f;
        ((D3DMATRIX *)worldViewMatrix)->_34 = 0.0f;
        ((D3DMATRIX *)worldViewMatrix)->_44 = 1.0f;
    }
    else
    {
        memcpy(worldViewMatrix, &_RwD3D9D3D9ViewTransform, sizeof(D3DMATRIX));
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetInverseWorldMatrix

*/
void
_rwD3D9VSGetInverseWorldMatrix(void *inverseWorldMatrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetInverseWorldMatrix"));

    if (D3D9ActiveInverseTransform == NULL)
    {
        if (D3D9ActiveTransform == NULL ||
            (D3D9ActiveTransform->flags & rwMATRIXINTERNALIDENTITY) == rwMATRIXINTERNALIDENTITY)
        {
            RWASSERT(D3D9ActiveTransform == NULL || rwMatrixIsIdentity(D3D9ActiveTransform, 0.01f));

            D3D9ActiveInverseTransform = &D3D9IdentityMatrix;
            D3D9ActiveInverseNormalizedTransform = &D3D9IdentityMatrix;
        }
        else if ((D3D9ActiveTransform->flags & rwMATRIXTYPEORTHONORMAL) == rwMATRIXTYPEORTHONORMAL)
        {
            D3D9MatrixInvertOrthoNormalized(&D3D9InverseTransform, D3D9ActiveTransform);

            D3D9ActiveInverseTransform = &D3D9InverseTransform;
            D3D9ActiveInverseNormalizedTransform = &D3D9InverseTransform;
        }
        else
        {
            RwMatrixInvert(&D3D9InverseTransform, D3D9ActiveTransform);
            ((D3DMATRIX *)&D3D9InverseTransform)->_14 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_24 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_34 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_44 = 1.0f;

            D3D9ActiveInverseTransform = &D3D9InverseTransform;
        }
    }

    memcpy(inverseWorldMatrix, D3D9ActiveInverseTransform, sizeof(D3DMATRIX));

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetWorldMultiplyMatrix

*/
void
_rwD3D9VSGetWorldMultiplyMatrix(RwMatrix *worldMultiplyMatrix,
                                const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetWorldMultiplyMatrix"));

    if (D3D9ActiveTransform != NULL)
    {
        RwMatrixMultiply(worldMultiplyMatrix, D3D9ActiveTransform, matrix);
    }
    else
    {
        RwMatrixCopy(worldMultiplyMatrix, matrix);
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetWorldMultiplyTransposeMatrix

*/
void
_rwD3D9VSGetWorldMultiplyTransposeMatrix(void *worldMultiplyMatrix,
                                         const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetWorldMultiplyTransposeMatrix"));

    if (D3D9ActiveTransform != NULL &&
        (D3D9ActiveTransform->flags & rwMATRIXINTERNALIDENTITY) != rwMATRIXINTERNALIDENTITY)
    {
        RwMatrix tmpMtx;

        RwMatrixMultiply(&tmpMtx, D3D9ActiveTransform, matrix);

        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldMultiplyMatrix, (D3DMATRIX *)&tmpMtx);
        #else
        D3D9MatrixTranspose(worldMultiplyMatrix, (D3DMATRIX *)&tmpMtx);
        #endif
    }
    else
    {
        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldMultiplyMatrix, (D3DMATRIX *)matrix);
        #else
        D3D9MatrixTranspose(worldMultiplyMatrix, (D3DMATRIX *)matrix);
        #endif
    }

    ((D3DMATRIX *)worldMultiplyMatrix)->m[3][0] = 0.0f;
    ((D3DMATRIX *)worldMultiplyMatrix)->m[3][1] = 0.0f;
    ((D3DMATRIX *)worldMultiplyMatrix)->m[3][2] = 0.0f;
    ((D3DMATRIX *)worldMultiplyMatrix)->m[3][3] = 1.0f;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetWorldViewMultiplyTransposeMatrix

*/
void
_rwD3D9VSGetWorldViewMultiplyTransposeMatrix(void *worldViewMultiplyMatrix,
                                             const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetWorldViewMultiplyTransposeMatrix"));

    if (D3D9ActiveTransform != NULL &&
        (D3D9ActiveTransform->flags & rwMATRIXINTERNALIDENTITY) != rwMATRIXINTERNALIDENTITY)
    {
        RwMatrix worldWiew;
        RWD3D9_ALIGN16 D3DMATRIX tmpMtx;

        RwMatrixMultiply(&worldWiew, D3D9ActiveTransform, (RwMatrix *)&_RwD3D9D3D9ViewTransform);

        RwMatrixMultiply((RwMatrix *)&tmpMtx, &worldWiew, matrix);

        tmpMtx.m[0][3] = 0.0f;
        tmpMtx.m[1][3] = 0.0f;
        tmpMtx.m[2][3] = 0.0f;
        tmpMtx.m[3][3] = 1.0f;

        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldViewMultiplyMatrix, &tmpMtx);
        #else
        D3D9MatrixTranspose(worldViewMultiplyMatrix, &tmpMtx);
        #endif
    }
    else
    {
        RWD3D9_ALIGN16 D3DMATRIX tmpMtx;

        RwMatrixMultiply((RwMatrix *)&tmpMtx, (RwMatrix *)&_RwD3D9D3D9ViewTransform, matrix);

        tmpMtx.m[0][3] = 0.0f;
        tmpMtx.m[1][3] = 0.0f;
        tmpMtx.m[2][3] = 0.0f;
        tmpMtx.m[3][3] = 1.0f;

        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldViewMultiplyMatrix, &tmpMtx);
        #else
        D3D9MatrixTranspose(worldViewMultiplyMatrix, &tmpMtx);
        #endif
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetWorldNormalizedMultiplyTransposeMatrix

*/
void
_rwD3D9VSGetWorldNormalizedMultiplyTransposeMatrix(void *worldMultiplyMatrix,
                                                   const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetWorldNormalizedMultiplyTransposeMatrix"));

    if (D3D9ActiveTransform != NULL &&
        (D3D9ActiveTransform->flags & rwMATRIXINTERNALIDENTITY) != rwMATRIXINTERNALIDENTITY)
    {
        RwMatrix tmpMtx;

        if ((D3D9ActiveTransform->flags & rwMATRIXTYPEORTHONORMAL) == rwMATRIXTYPEORTHONORMAL)
        {
            RwMatrixMultiply(&tmpMtx, D3D9ActiveTransform, matrix);
        }
        else
        {
            if (D3D9ActiveNormalizedTransform == NULL)
            {
                RwMatrixOrthoNormalize(&D3D9NormalizedTransform, D3D9ActiveTransform);
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_14 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_24 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_34 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_44 = 1.0f;

                D3D9ActiveNormalizedTransform = &D3D9NormalizedTransform;
            }

            RwMatrixMultiply(&tmpMtx, D3D9ActiveNormalizedTransform, matrix);
        }

        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldMultiplyMatrix, (D3DMATRIX *)&tmpMtx);
        #else
        D3D9MatrixTranspose(worldMultiplyMatrix, (D3DMATRIX *)&tmpMtx);
        #endif
    }
    else
    {
        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldMultiplyMatrix, (D3DMATRIX *)matrix);
        #else
        D3D9MatrixTranspose(worldMultiplyMatrix, (D3DMATRIX *)matrix);
        #endif
    }

    ((D3DMATRIX *)worldMultiplyMatrix)->m[3][0] = 0.0f;
    ((D3DMATRIX *)worldMultiplyMatrix)->m[3][1] = 0.0f;
    ((D3DMATRIX *)worldMultiplyMatrix)->m[3][2] = 0.0f;
    ((D3DMATRIX *)worldMultiplyMatrix)->m[3][3] = 1.0f;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetWorldNormalizedViewMultiplyTransposeMatrix

*/
void
_rwD3D9VSGetWorldNormalizedViewMultiplyTransposeMatrix(void *worldViewMultiplyMatrix,
                                                       const RwMatrix *matrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetWorldNormalizedViewMultiplyTransposeMatrix"));

    if (D3D9ActiveTransform != NULL &&
        (D3D9ActiveTransform->flags & rwMATRIXINTERNALIDENTITY) != rwMATRIXINTERNALIDENTITY)
    {
        RwMatrix worldWiew;
        RWD3D9_ALIGN16 D3DMATRIX tmpMtx;

        if ((D3D9ActiveTransform->flags & rwMATRIXTYPEORTHONORMAL) == rwMATRIXTYPEORTHONORMAL)
        {
            RwMatrixMultiply(&worldWiew, D3D9ActiveTransform, (RwMatrix *)&_RwD3D9D3D9ViewTransform);
        }
        else
        {
            if (D3D9ActiveNormalizedTransform == NULL)
            {
                RwMatrixOrthoNormalize(&D3D9NormalizedTransform, D3D9ActiveTransform);
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_14 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_24 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_34 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_44 = 1.0f;

                D3D9ActiveNormalizedTransform = &D3D9NormalizedTransform;
            }

            RwMatrixMultiply(&worldWiew, D3D9ActiveNormalizedTransform, (RwMatrix *)&_RwD3D9D3D9ViewTransform);
        }

        RwMatrixMultiply((RwMatrix *)&tmpMtx, &worldWiew, matrix);

        tmpMtx.m[0][3] = 0.0f;
        tmpMtx.m[1][3] = 0.0f;
        tmpMtx.m[2][3] = 0.0f;
        tmpMtx.m[3][3] = 1.0f;

        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldViewMultiplyMatrix, &tmpMtx);
        #else
        D3D9MatrixTranspose(worldViewMultiplyMatrix, &tmpMtx);
        #endif
    }
    else
    {
        RWD3D9_ALIGN16 D3DMATRIX tmpMtx;

        RwMatrixMultiply((RwMatrix *)&tmpMtx, (RwMatrix *)&_RwD3D9D3D9ViewTransform, matrix);

        tmpMtx.m[0][3] = 0.0f;
        tmpMtx.m[1][3] = 0.0f;
        tmpMtx.m[2][3] = 0.0f;
        tmpMtx.m[3][3] = 1.0f;

        #ifdef USE_D3DX
        D3DXMatrixTranspose(worldViewMultiplyMatrix, &tmpMtx);
        #else
        D3D9MatrixTranspose(worldViewMultiplyMatrix, &tmpMtx);
        #endif
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetWorldNormalizedTransposeMatrix

 On entry :
 On exit  :
 */
void
_rwD3D9VSGetWorldNormalizedTransposeMatrix(void *worldMatrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetWorldNormalizedTransposeMatrix"));
    RWASSERT(worldMatrix != NULL);

    if (D3D9ActiveTransform != NULL &&
        (D3D9ActiveTransform->flags & rwMATRIXINTERNALIDENTITY) != rwMATRIXINTERNALIDENTITY)
    {
        if ((D3D9ActiveTransform->flags & rwMATRIXTYPEORTHONORMAL) == rwMATRIXTYPEORTHONORMAL)
        {
            #ifdef USE_D3DX
            D3DXMatrixTranspose(worldMatrix, &D3D9Transform);
            #else
            D3D9MatrixTranspose(worldMatrix, &D3D9Transform);
            #endif
        }
        else
        {
            if (D3D9ActiveNormalizedTransform == NULL)
            {
                RwMatrixOrthoNormalize(&D3D9NormalizedTransform, D3D9ActiveTransform);
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_14 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_24 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_34 = 0.0f;
                ((D3DMATRIX *)&D3D9NormalizedTransform)->_44 = 1.0f;

                D3D9ActiveNormalizedTransform = &D3D9NormalizedTransform;
            }

            #ifdef USE_D3DX
            D3DXMatrixTranspose(worldMatrix, (D3DMATRIX *)D3D9ActiveNormalizedTransform);
            #else
            D3D9MatrixTranspose(worldMatrix, D3D9ActiveNormalizedTransform);
            #endif
        }
    }
    else
    {
        memcpy(worldMatrix, &D3D9IdentityMatrix, sizeof(D3DMATRIX));
        ((D3DMATRIX *)worldMatrix)->_14 = 0.0f;
        ((D3DMATRIX *)worldMatrix)->_24 = 0.0f;
        ((D3DMATRIX *)worldMatrix)->_34 = 0.0f;
        ((D3DMATRIX *)worldMatrix)->_44 = 1.0f;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetProjectionTransposedMatrix

*/
void
_rwD3D9VSGetProjectionTransposedMatrix(void *projectionMatrix)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetProjectionTransposedMatrix"));

    #ifdef USE_D3DX
    D3DXMatrixTranspose(projectionMatrix, &_RwD3D9D3D9ProjTransform);
    #else
    D3D9MatrixTranspose(projectionMatrix, &_RwD3D9D3D9ProjTransform);
    #endif

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetNormalInLocalSpace

 On entry :
 On exit  :
 */
void
_rwD3D9VSGetNormalInLocalSpace(const RwV3d *normalWorldSpace,
                               RwV3d *normalLocalSpace)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetNormalInLocalSpace"));
    RWASSERT(normalWorldSpace != NULL);
    RWASSERT(normalLocalSpace != NULL);

    if (D3D9ActiveInverseNormalizedTransform == NULL)
    {
        if (D3D9ActiveTransform == NULL ||
            (D3D9ActiveTransform->flags & rwMATRIXINTERNALIDENTITY) == rwMATRIXINTERNALIDENTITY)
        {
            RWASSERT(D3D9ActiveTransform == NULL || rwMatrixIsIdentity(D3D9ActiveTransform, 0.01f));

            D3D9ActiveInverseTransform = &D3D9IdentityMatrix;
            D3D9ActiveInverseNormalizedTransform = &D3D9IdentityMatrix;
        }
        else if ((D3D9ActiveTransform->flags & rwMATRIXTYPEORTHONORMAL) == rwMATRIXTYPEORTHONORMAL)
        {
            if (D3D9ActiveInverseTransform == NULL)
            {
                D3D9MatrixInvertOrthoNormalized(&D3D9InverseTransform, D3D9ActiveTransform);

                D3D9ActiveInverseTransform = &D3D9InverseTransform;
            }

            D3D9ActiveInverseNormalizedTransform = D3D9ActiveInverseTransform;
        }
        else
        {
            if (D3D9ActiveInverseTransform == NULL)
            {
                RwMatrixInvert(&D3D9InverseTransform, D3D9ActiveTransform);
                ((D3DMATRIX *)&D3D9InverseTransform)->_14 = 0.0f;
                ((D3DMATRIX *)&D3D9InverseTransform)->_24 = 0.0f;
                ((D3DMATRIX *)&D3D9InverseTransform)->_34 = 0.0f;
                ((D3DMATRIX *)&D3D9InverseTransform)->_44 = 1.0f;

                D3D9ActiveInverseTransform = &D3D9InverseTransform;
            }

            RwMatrixOrthoNormalize(&D3D9InverseNormalizedTransform, D3D9ActiveInverseTransform);
            ((D3DMATRIX *)&D3D9InverseNormalizedTransform)->_14 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseNormalizedTransform)->_24 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseNormalizedTransform)->_34 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseNormalizedTransform)->_44 = 1.0f;

            D3D9ActiveInverseNormalizedTransform = &D3D9InverseNormalizedTransform;
        }
    }

    if (D3D9ActiveInverseNormalizedTransform != &D3D9IdentityMatrix)
    {
        #ifndef USE_D3DX
        RwV3dTransformVector(normalLocalSpace, normalWorldSpace, D3D9ActiveInverseNormalizedTransform);
        #else
        D3DXVec3TransformNormal( (D3DXVECTOR3 *)normalLocalSpace,
                                 (D3DXVECTOR3 *)normalWorldSpace,
                                 (const D3DMATRIX *)D3D9ActiveInverseNormalizedTransform );
        #endif
    }
    else
    {
        normalLocalSpace->x = normalWorldSpace->x;
        normalLocalSpace->y = normalWorldSpace->y;
        normalLocalSpace->z = normalWorldSpace->z;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetPointInLocalSpace

 On entry :
 On exit  :
 */
void
_rwD3D9VSGetPointInLocalSpace(const RwV3d *pointWorldSpace,
                              RwV3d *pointLocalSpace)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetPointInLocalSpace"));
    RWASSERT(pointWorldSpace != NULL);
    RWASSERT(pointLocalSpace != NULL);

    if (D3D9ActiveInverseTransform == NULL)
    {
        if (D3D9ActiveTransform == NULL ||
            (D3D9ActiveTransform->flags & rwMATRIXINTERNALIDENTITY) == rwMATRIXINTERNALIDENTITY)
        {
            RWASSERT(D3D9ActiveTransform == NULL || rwMatrixIsIdentity(D3D9ActiveTransform, 0.01f));

            D3D9ActiveInverseTransform = &D3D9IdentityMatrix;
        }
        else if ((D3D9ActiveTransform->flags & rwMATRIXTYPEORTHONORMAL) == rwMATRIXTYPEORTHONORMAL)
        {
            D3D9MatrixInvertOrthoNormalized(&D3D9InverseTransform, D3D9ActiveTransform);

            D3D9ActiveInverseTransform = &D3D9InverseTransform;
        }
        else
        {
            RwMatrixInvert(&D3D9InverseTransform, D3D9ActiveTransform);
            ((D3DMATRIX *)&D3D9InverseTransform)->_14 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_24 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_34 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_44 = 1.0f;

            D3D9ActiveInverseTransform = &D3D9InverseTransform;
        }
    }

    if (D3D9ActiveInverseTransform != &D3D9IdentityMatrix)
    {
        #ifndef USE_D3DX
        RwV3dTransformPoint(pointLocalSpace, pointWorldSpace, D3D9ActiveInverseTransform);
        #else
        D3DXVec3TransformCoord((D3DXVECTOR3 *)pointLocalSpace,
                               (D3DXVECTOR3 *)pointWorldSpace,
                               (const D3DMATRIX *)D3D9ActiveInverseTransform );
        #endif
    }
    else
    {
        pointLocalSpace->x = pointWorldSpace->x;
        pointLocalSpace->y = pointWorldSpace->y;
        pointLocalSpace->z = pointWorldSpace->z;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9VSGetRadiusInLocalSpace

 On entry :
 On exit  :
 */
void
_rwD3D9VSGetRadiusInLocalSpace(RwReal radiusWorldSpace,
                               RwReal *radiusLocalSpace)
{
    RWFUNCTION(RWSTRING("_rwD3D9VSGetRadiusInLocalSpace"));
    RWASSERT(radiusLocalSpace != NULL);

    if (D3D9ActiveTransform != NULL &&
        (D3D9ActiveTransform->flags & rwMATRIXTYPEORTHONORMAL) != rwMATRIXTYPEORTHONORMAL)
    {
        RwV3d radiusVector;
        RwReal squareLength, invlength;

        if (D3D9ActiveInverseTransform == NULL)
        {
            RwMatrixInvert(&D3D9InverseTransform, D3D9ActiveTransform);
            ((D3DMATRIX *)&D3D9InverseTransform)->_14 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_24 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_34 = 0.0f;
            ((D3DMATRIX *)&D3D9InverseTransform)->_44 = 1.0f;

            D3D9ActiveInverseTransform = &D3D9InverseTransform;
        }

        radiusVector.z = radiusVector.y = radiusVector.x = 0.57735026918962576450914878050196f;

        #ifndef USE_D3DX
        RwV3dTransformVector(&radiusVector, &radiusVector, D3D9ActiveInverseTransform);
        #else
        D3DXVec3TransformNormal((D3DXVECTOR3 *)&radiusVector,
                                (D3DXVECTOR3 *)&radiusVector,
                                (D3DMATRIX *)D3D9ActiveInverseTransform );
        #endif

        squareLength = RwV3dDotProductMacro(&radiusVector, &radiusVector);
        rwInvSqrtMacro(&invlength, squareLength);
        *radiusLocalSpace = radiusWorldSpace * invlength;
    }
    else
    {
        *radiusLocalSpace = radiusWorldSpace;
    }

    RWRETURNVOID();
}
