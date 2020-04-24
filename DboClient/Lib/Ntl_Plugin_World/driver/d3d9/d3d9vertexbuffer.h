/****************************************************************************
 *                                                                          *
 * Module  : d3d9vertexbuffer.h                                             *
 *                                                                          *
 * Purpose : See d3d9vertexbuffer.c                                         *
 *                                                                          *
 ****************************************************************************/

#ifndef D3D9VERTEXBUFFER_H
#define D3D9VERTEXBUFFER_H

/****************************************************************************
 Includes
 */

/****************************************************************************
 Global Defines
 */
#define LPVERTEXBUFFER LPDIRECT3DVERTEXBUFFER9

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

extern RwBool
_rwD3D9VertexBufferManagerOpen(void);

extern void
_rwD3D9VertexBufferManagerClose(void);

extern void
_rwD3D9DynamicVertexBufferRelease(void);

extern RwBool
_rwD3D9DynamicVertexBufferRestore(void);

extern void
_rwD3D9DynamicVertexBufferManagerForceDiscard(void);

/* RWPUBLIC */

extern void
RwD3D9VertexBufferManagerChangeDefaultSize(RwUInt32 defaultSize);

/*
 * To get a pointer to a static vertex buffer
 * Use both functions, Create and Destroy, not only Create
 */
extern RwBool
RwD3D9CreateVertexBuffer(RwUInt32 stride,
                         RwUInt32 size,
                         void **vertexBuffer,
                         RwUInt32 *offset);

extern void
RwD3D9DestroyVertexBuffer(RwUInt32 stride,
                          RwUInt32 size,
                          void *vertexBuffer,
                          RwUInt32 offset);

/*
 * To get a pointer to a Dynamic vertex buffer
 * Use both functions, Create and Destroy, not only Create
 */
extern RwBool RwD3D9DynamicVertexBufferCreate(RwUInt32 size,
                                              void **vertexBuffer);

extern void RwD3D9DynamicVertexBufferDestroy(void *vertexBuffer);

/*
 * To get a temporary pointer to a Dynamic vertex buffer memory
 * Don't use with the previous functions because
 * it mantains an internal list of dinamic vertex buffers
 */
extern RwBool RwD3D9DynamicVertexBufferLock(RwUInt32 vertexSize,
                                            RwUInt32 numVertex,
                                            void **vertexBufferOut,
                                            void **vertexDataOut,
                                            RwUInt32 *baseIndexOut);

extern RwBool RwD3D9DynamicVertexBufferUnlock(void *vertexBuffer);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* D3D9VERTEXBUFFER_H */
