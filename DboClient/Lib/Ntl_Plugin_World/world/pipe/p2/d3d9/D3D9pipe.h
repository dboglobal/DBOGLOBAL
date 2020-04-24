/****************************************************************************
 *                                                                          *
 * Module  : D3D9pipe.h                                                     *
 *                                                                          *
 * Purpose : See D3D9pipe.c                                                 *
 *                                                                          *
 ****************************************************************************/

#ifndef D3D9PIPE_H
#define D3D9PIPE_H

/****************************************************************************
 Includes
 */

#include <d3d9.h>

#include "rwcore.h"
#include "baworld.h"

/* RWPUBLIC */
/****************************************************************************
 Global Defines
 */
#define RWD9DOBJECTGETTYPE(_object) (*((RwUInt8 *)(_object)))

#define RWD9DOBJECTISGEOMETRY(_object) (RWD9DOBJECTGETTYPE(_object) == (RwUInt8)rpGEOMETRY)
#define RWD9DOBJECTISATOMIC(_object) (RWD9DOBJECTGETTYPE(_object) == (RwUInt8)rpATOMIC)
#define RWD9DOBJECTISWORLDSECTOR(_object) (RWD9DOBJECTGETTYPE(_object) == (RwUInt8)-1)

/****************************************************************************
 Global Types
 */
/* RWPUBLICEND */

typedef LPDIRECT3DVERTEXBUFFER9 LPVERTEXBUFFER;
typedef LPDIRECT3DINDEXBUFFER9 LPINDEXBUFFER;

/* RWPUBLIC */

typedef struct RxD3D9ResEntryHeader RxD3D9ResEntryHeader;
/**
 * \ingroup worldextensionsd3d9
 * \struct RxD3D9ResEntryHeader
 * This structure contains D3D9 resource specific components.
 */
struct RxD3D9ResEntryHeader
{
    RwUInt32    serialNumber;   /**< The mesh headers serial number */

    RwUInt32    numMeshes;      /**< The number of meshes */

    void        *indexBuffer;   /**< Index buffer */

    RwUInt32    primType;       /**< Primitive type */

    RxD3D9VertexStream vertexStream[RWD3D9_MAX_VERTEX_STREAMS];   /**< Vertex streams */

    RwBool      useOffsets;      /**< Use vertex buffer offsets when setting the streams */

    void        *vertexDeclaration;   /**< Vertex declaration */

    RwUInt32    totalNumIndex;  /**< Total number of indices. Needed for
                                     reinstancing, not for rendering */

    RwUInt32    totalNumVertex; /**< Total number of vertices. Needed for
                                     reinstancing, not for rendering */
};

/* This is what I keep in memory as part of the instance data setup */
typedef struct RxD3D9InstanceData RxD3D9InstanceData;
/**
 * \ingroup worldextensionsd3d9
 * \struct RxD3D9InstanceData
 * This structure contains D3D9 resource specific components.
 */
struct RxD3D9InstanceData
{
    RwUInt32        numIndex;       /**< Number of indices. Needed for
                                         reinstancing, not for rendering */

    RwUInt32        minVert;        /**< The vertex index where the
                                         meshes vertices begin. Needed for
                                         reinstancing, not for rendering */

    RpMaterial      *material;      /**< The material for this instanced data */

    RwBool          vertexAlpha;    /**< The prelight data contains alpha */

    void            *vertexShader;   /**< Vertex shader */

    RwUInt32        baseIndex;      /**< Needed if we share the vertex buffer */

    RwUInt32        numVertices;    /**< Number of vertices in the vertex buffer */

    RwUInt32        startIndex;     /**< Location in the index array to start
                                         reading vertices. */

    RwUInt32        numPrimitives;  /**< Number of primitives to render */
};

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneInstanceCallBack callback function.
 * 
 * \param object Pointer to the object.
 * \param resEntryHeader Pointer to \ref RxD3D9ResEntryHeader.
 * \param reinstance Boolean whether to reinstance.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \see RxD3D9AllInOneSetInstanceCallBack
 * \see RxD3D9AllInOneGetInstanceCallBack
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 */
typedef RwBool (*RxD3D9AllInOneInstanceCallBack)(void *object,
                                                 RxD3D9ResEntryHeader *resEntryHeader,
                                                 RwBool reinstance);

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneReinstanceCallBack callback function.
 * 
 * \param object Pointer to the object.
 * \param resEntry Pointer to \ref RxD3D9ResEntryHeader.
 * \param instanceCallback Pointer to \ref RxD3D9AllInOneInstanceCallBack.
 *
 * \return TRUE if successful, FALSE otherwise.
 *
 * \see RxD3D9AllInOneSetReinstanceCallBack
 * \see RxD3D9AllInOneGetReinstanceCallBack
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 */
typedef RwBool (*RxD3D9AllInOneReinstanceCallBack)(void *object,
                                                   RwResEntry *resEntry,
                                                   RxD3D9AllInOneInstanceCallBack instanceCallback);

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneLightingCallBack callback function.
 * 
 * \param object Void pointer to the object.
 *
 * \see RxD3D9AllInOneSetLightingCallBack
 * \see RxD3D9AllInOneGetLightingCallBack
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 */
typedef void (*RxD3D9AllInOneLightingCallBack)(void *object);

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneRenderCallBack callback function.
 * 
 * \param repEntry Pointer to a resource entry.
 * \param object Pointer to a object that the resource entry belongs to.
 * \param type rpATOMIC or rwSECTORATOMIC.
 * \param flags \ref RpGeometryFlag's or \ref RpWorldFlag's.
 *
 * \see RxD3D9AllInOneSetRenderCallBack
 * \see RxD3D9AllInOneGetRenderCallBack
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 */
typedef void (*RxD3D9AllInOneRenderCallBack)(RwResEntry *repEntry,
                                             void *object,
                                             RwUInt8 type,
                                             RwUInt32 flags);

/* RWPUBLICEND */

typedef struct __rxD3D9InstanceNodeData _rxD3D9InstanceNodeData;
struct __rxD3D9InstanceNodeData
{
    RxD3D9AllInOneInstanceCallBack      instanceCallback;   /* Instance callback */
    RxD3D9AllInOneReinstanceCallBack    reinstanceCallback; /* Instance callback */
    RxD3D9AllInOneLightingCallBack      lightingCallback;   /* Lighting callback */
    RxD3D9AllInOneRenderCallBack        renderCallback;     /* Render callback */
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
_rxD3D9Instance(void *object,
                void *owner,
                RwUInt8 type,
                RwResEntry **resEntryPointer,
                RpMeshHeader *meshHeader,
                RxD3D9AllInOneInstanceCallBack instanceCallback,
                RwBool allocateNative);

extern void
_rxD3D9DefaultRenderCallback(RwResEntry *repEntry,
                             void *object,
                             RwUInt8 type,
                             RwUInt32 flags);

extern void
_rwD3D9MeshGetNumVerticesMinIndex(const RxVertexIndex *indices,
                                  RwUInt32 numIndices,
                                  RwUInt32 *numVertices,
                                  RwUInt32 *min);

extern void
_rwD3D9SortTriListIndices(RxVertexIndex *indices, RwUInt32 numIndices);

extern RwUInt32
_rwD3D9ConvertToTriList(RxVertexIndex *indexDst,
                        const RxVertexIndex *indexSrc,
                        RwUInt32 numIndices,
                        RwUInt32 minVert);


extern void 
_rwD3D9ResourceEntryInstanceDataDestroy(RwResEntry *repEntry);

/* internal driver functions */
extern RwBool _rwD3D9RenderStateVertexAlphaEnable(RwBool enable);
extern RwBool _rwD3D9RenderStateIsVertexAlphaEnable(void);

extern RwRaster *_rwD3D9RWGetRasterStage(RwUInt32 stage);

extern RwBool _rwD3D9TextureHasAlpha(const RwTexture *texture);

/* RWPUBLIC */
extern void
RxD3D9AllInOneSetInstanceCallBack(RxPipelineNode *node,
                                  RxD3D9AllInOneInstanceCallBack callback);

extern RxD3D9AllInOneInstanceCallBack
RxD3D9AllInOneGetInstanceCallBack(RxPipelineNode *node);

extern void
RxD3D9AllInOneSetReinstanceCallBack(RxPipelineNode *node,
                                  RxD3D9AllInOneReinstanceCallBack callback);

extern RxD3D9AllInOneReinstanceCallBack
RxD3D9AllInOneGetReinstanceCallBack(RxPipelineNode *node);

extern void
RxD3D9AllInOneSetLightingCallBack(RxPipelineNode *node,
                                  RxD3D9AllInOneLightingCallBack callback);

extern RxD3D9AllInOneLightingCallBack
RxD3D9AllInOneGetLightingCallBack(RxPipelineNode *node);

extern void
RxD3D9AllInOneSetRenderCallBack(RxPipelineNode *node,
                                RxD3D9AllInOneRenderCallBack callback);

extern RxD3D9AllInOneRenderCallBack
RxD3D9AllInOneGetRenderCallBack(RxPipelineNode *node);


/* Usefull functions */
void
_rwD3D9EnableClippingIfNeeded(void *object,
                              RwUInt32 type);

extern void
_rpD3D9AddDynamicGeometry(RpGeometry *geometry);

extern void
_rpD3D9RemoveDynamicGeometry(RpGeometry *geometry);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* D3D9PIPE_H */
