/****************************************************************************
 *                                                                          *
 * Module  : D3D8pipe.h                                                     *
 *                                                                          *
 * Purpose : See D3D8pipe.c                                                 *
 *                                                                          *
 ****************************************************************************/

#ifndef D3D8PIPE_H
#define D3D8PIPE_H

/****************************************************************************
 Includes
 */

#include <d3d8.h>

#include "rwcore.h"
#include "baworld.h"


/****************************************************************************
 Global Defines
 */

/****************************************************************************
 Global Types
 */

/* RWPUBLIC */

typedef struct RxD3D8ResEntryHeader RxD3D8ResEntryHeader;
/**
 * \ingroup worldextensionsd3d8
 * \struct RxD3D8ResEntryHeader
 * This structure contains D3D8 resource specific components.
 */
struct RxD3D8ResEntryHeader
{
    RwUInt16    serialNumber;   /**< The mesh headers serial number */
    RwUInt16    numMeshes;      /**< The number of meshes */
};

/* This is what I keep in memory as part of the instance data setup */
typedef struct RxD3D8InstanceData RxD3D8InstanceData;
/**
 * \ingroup worldextensionsd3d8
 * \struct RxD3D8InstanceData
 * This structure contains D3D8 resource specific components.
 */
struct RxD3D8InstanceData
{
    RwUInt32        minVert;        /**< The vertex index where the
                                         meshes vertices begin. Needed for
                                         reinstancing, not for rendering */

    RwInt32         stride;         /**< Stride of vertex */

    RwInt32         numVertices;    /**< Number of vertices in the vertex buffer */
    RwInt32         numIndices;     /**< Number of indices to render the primitive */

    RpMaterial      *material;      /**< The material for this instanced data */

    RwUInt32        vertexShader;   /**< Vertex shader or FVF flags */

    RwUInt32        primType;       /**< Primitive type */
    
    void            *indexBuffer;   /**< Index buffer */
    void            *vertexBuffer;  /**< Vertex buffer */

    RwUInt32        baseIndex;      /**< Needed if we share the vertex buffer */

    RwUInt8         vertexAlpha;    /**< The prelight data contains alpha */
    RwUInt8         managed;        /**< Created by the Vertex Buffer Manager */
    RwUInt8         remapped;       /**< Vertices where remapped to save VB space */
};

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneInstanceCallBack callback function.
 * 
 * \param object Pointer to the object.
 * \param instancedData Pointer to \ref RxD3D8InstanceData.
 * \param reinstance Boolean whether to reinstance.
 *
 * \return TRUE if successful, FALSE otherwise.
 * 
 * \see RxD3D8AllInOneSetInstanceCallBack
 * \see RxD3D8AllInOneGetInstanceCallBack
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 */
typedef RwBool (*RxD3D8AllInOneInstanceCallBack)(void *object,
                                                 RxD3D8InstanceData *instancedData,
                                                 RwBool reinstance);

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneReinstanceCallBack callback function.
 * 
 * \param object Pointer to the object.
 * \param meshHeader Const pointer to \ref RpMeshHeader.
 * \param resEntry Pointer to \ref RxD3D8ResEntryHeader.
 * \param instanceCallback Pointer to \ref RxD3D8AllInOneInstanceCallBack.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \see RxD3D8AllInOneSetReinstanceCallBack
 * \see RxD3D8AllInOneGetReinstanceCallBack
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 */
typedef RwBool (*RxD3D8AllInOneReinstanceCallBack)(void *object,
                                                   RwResEntry *resEntry,
                                                   const RpMeshHeader *meshHeader,
                                                   RxD3D8AllInOneInstanceCallBack instanceCallback);

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneLightingCallBack callback function.
 * 
 * \param object Void pointer to the object.
 *
 * \see RxD3D8AllInOneSetLightingCallBack
 * \see RxD3D8AllInOneGetLightingCallBack
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 */
typedef void (*RxD3D8AllInOneLightingCallBack)(void *object);

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneRenderCallBack callback function.
 * 
 * \param repEntry Pointer to a resource entry.
 * \param object Pointer to a object that the resource entry belongs to.
 * \param type rpATOMIC or rwSECTORATOMIC.
 * \param flags \ref RpGeometryFlag's or \ref RpWorldFlag's.
 *
 * \see RxD3D8AllInOneSetRenderCallBack
 * \see RxD3D8AllInOneGetRenderCallBack
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 */
typedef void (*RxD3D8AllInOneRenderCallBack)(RwResEntry *repEntry,
                                             void *object,
                                             RwUInt8 type,
                                             RwUInt32 flags);

/* RWPUBLICEND */

typedef struct __rxD3D8InstanceNodeData _rxD3D8InstanceNodeData;
struct __rxD3D8InstanceNodeData
{
    RxD3D8AllInOneInstanceCallBack      instanceCallback;   /* Instance callback */
    RxD3D8AllInOneReinstanceCallBack    reinstanceCallback; /* Instance callback */
    RxD3D8AllInOneLightingCallBack      lightingCallback;   /* Lighting callback */
    RxD3D8AllInOneRenderCallBack        renderCallback;     /* Render callback */
};

/****************************************************************************
 Function prototypes
 */

/* RWPUBLIC */
#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */
/* RWPUBLICEND */

extern RwResEntry *
_rxD3D8Instance(void *object,
                void *owner,
                RwResEntry **resEntryPointer,
                RpMeshHeader *meshHeader,
                RxD3D8AllInOneInstanceCallBack instanceCallback,
                RwBool allocateNative);

extern RwBool
_rxD3D8Reinstance(void *object,
                  const RpMeshHeader *meshHeader,
                  RxD3D8InstanceData *instancedData,
                  RxD3D8AllInOneInstanceCallBack instanceCallback);

extern void
_rxD3D8DefaultRenderCallback(RwResEntry *repEntry,
                             void *object,
                             RwUInt8 type,
                             RwUInt32 flags);

extern void
_rwD3D8MeshGetNumVerticesMinIndex(RxVertexIndex *indices,
                        RwUInt32 numIndices,
                        RwInt32 *numVertices,
                        RwUInt32 *min);

extern void
_rwD3D8SortTriListIndices(RxVertexIndex *indices, RwUInt32 numIndices);


extern void 
_rwD3D8ResourceEntryInstanceDataDestroy(RwResEntry *repEntry);

/* internal driver functions */
extern RwBool _rwD3D8RenderStateVertexAlphaEnable(RwBool enable);
extern RwBool _rwD3D8RenderStateIsVertexAlphaEnable(void);

extern RwRaster *_rwD3D8RWGetRasterStage(RwUInt32 stage);

/* RWPUBLIC */
extern void
RxD3D8AllInOneSetInstanceCallBack(RxPipelineNode *node,
                                  RxD3D8AllInOneInstanceCallBack callback);

extern RxD3D8AllInOneInstanceCallBack
RxD3D8AllInOneGetInstanceCallBack(RxPipelineNode *node);

extern void
RxD3D8AllInOneSetReinstanceCallBack(RxPipelineNode *node,
                                  RxD3D8AllInOneReinstanceCallBack callback);

extern RxD3D8AllInOneReinstanceCallBack
RxD3D8AllInOneGetReinstanceCallBack(RxPipelineNode *node);

extern void
RxD3D8AllInOneSetLightingCallBack(RxPipelineNode *node,
                                  RxD3D8AllInOneLightingCallBack callback);

extern RxD3D8AllInOneLightingCallBack
RxD3D8AllInOneGetLightingCallBack(RxPipelineNode *node);

extern void
RxD3D8AllInOneSetRenderCallBack(RxPipelineNode *node,
                                RxD3D8AllInOneRenderCallBack callback);

extern RxD3D8AllInOneRenderCallBack
RxD3D8AllInOneGetRenderCallBack(RxPipelineNode *node);


#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* D3D8PIPE_H */
