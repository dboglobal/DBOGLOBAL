/****************************************************************************
 *                                                                          *
 * Module  : d3d9vertexshaderutils.h                                        *
 *                                                                          *
 * Purpose : See d3d9vertexshaderutils.c                                    *
 *                                                                          *
 ****************************************************************************/

#ifndef D3D9VERTEXSHADERUTILS_H
#define D3D9VERTEXSHADERUTILS_H

/****************************************************************************
 Includes
 */
#include <d3d9.h>

/****************************************************************************
 Global Defines
 */

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Global Variables
 */

/****************************************************************************
 Function prototypes
 */

/* RWPUBLIC */
#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */
/* RWPUBLICEND */

/* RWPUBLIC */

/* Call this function before calling to any of the following ones */
extern void
_rwD3D9VSSetActiveWorldMatrix(const RwMatrix *worldMatrix);

/*
 * Copy the transposed combined world-view-projection matrix to the
 * requested destination.
 */
extern void
_rwD3D9VSGetComposedTransformMatrix(void *transformMatrix);

/*
 * Copy the transposed combined world-view matrix to the
 * requested destination.
 */
extern void
_rwD3D9VSGetWorldViewTransposedMatrix(void *worldViewMatrix);

extern void
_rwD3D9VSGetWorldViewMatrix(void *worldViewMatrix);

extern void
_rwD3D9VSGetInverseWorldMatrix(void *inverseWorldMatrix);

extern void
_rwD3D9VSGetWorldMultiplyMatrix(RwMatrix *worldMultiplyMatrix,
                                const RwMatrix *matrix);

extern void
_rwD3D9VSGetWorldMultiplyTransposeMatrix(void *worldMultiplyMatrix,
                                         const RwMatrix *matrix);

extern void
_rwD3D9VSGetWorldViewMultiplyTransposeMatrix(void *worldViewMultiplyMatrix,
                                             const RwMatrix *matrix);

extern void
_rwD3D9VSGetWorldNormalizedMultiplyTransposeMatrix(void *worldMultiplyMatrix,
                                                   const RwMatrix *matrix);

extern void
_rwD3D9VSGetWorldNormalizedViewMultiplyTransposeMatrix(void *worldViewMultiplyMatrix,
                                                       const RwMatrix *matrix);

extern void
_rwD3D9VSGetWorldNormalizedTransposeMatrix(void *worldMatrix);

extern void
_rwD3D9VSGetProjectionTransposedMatrix(void *projectionMatrix);

/* Converting light parameters to local space */
extern void
_rwD3D9VSGetNormalInLocalSpace(const RwV3d *normalWorldSpace,
                               RwV3d *normalLocalSpace);

extern void
_rwD3D9VSGetPointInLocalSpace(const RwV3d *pointWorldSpace,
                              RwV3d *pointLocalSpace);

extern void
_rwD3D9VSGetRadiusInLocalSpace(RwReal radiusWorldSpace,
                               RwReal *radiusLocalSpace);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* D3D9VERTEXSHADERUTILS_H */
