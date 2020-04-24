/****************************************************************************
 *                                                                          *
 * Module  : D3D8VertexBufferManager.h                                      *
 *                                                                          *
 * Purpose : See D3D8VertexBufferManager.c                                  *
 *                                                                          *
 ****************************************************************************/

#ifndef D3D8VERTEXBUFFERMANAGER_H
#define D3D8VERTEXBUFFERMANAGER_H

/****************************************************************************
 Includes
 */

#include <d3d8.h>

#include "rwcore.h"

/****************************************************************************
 Global Defines
 */

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Global Variables
 */

extern LPDIRECT3DDEVICE8                _RwD3DDevice;

/****************************************************************************
 Function prototypes
 */

/* RWPUBLIC */
#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */
/* RWPUBLICEND */

extern RwBool _rxD3D8VertexBufferManagerOpen(void);

extern void _rxD3D8VertexBufferManagerClose(void);

/* RWPUBLIC */

extern void RwD3D8VertexBufferManagerChangeDefaultSize(RwUInt32 defaultSize);

extern RwBool _rxD3D8VertexBufferManagerCreate(RwUInt32 fvf,
                                               RwUInt32 size,
                                               void **vertexBuffer,
                                               RwUInt32 *baseIndex);

extern void _rxD3D8VertexBufferManagerDestroy(RwUInt32 fvf,
                                              RwUInt32 size,
                                              void *vertexBuffer,
                                              RwUInt32 baseIndex);

extern RwBool _rxD3D8VertexBufferManagerCreateNoFVF(RwUInt32 stride,
                                               RwUInt32 size,
                                               void **vertexBuffer,
                                               RwUInt32 *baseIndex);

extern void _rxD3D8VertexBufferManagerDestroyNoFVF(RwUInt32 stride,
                                              RwUInt32 size,
                                              void *vertexBuffer,
                                              RwUInt32 baseIndex);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* D3D8VERTEXBUFFERMANAGER_H */
