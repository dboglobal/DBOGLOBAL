/****************************************************************************
 *                                                                          *
 * module : d3d9vertexbuffer.c                                              *
 *                                                                          *
 * purpose:                                                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

/* D3D9 Libraries */
#include <d3d9.h>

/* RenderWare types */
#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "barwtyp.h"

#include "d3d9device.h"

#include "d3d9vertexbuffer.h"

/****************************************************************************
 Global variables
 */

/****************************************************************************
 local defines
 */

/****************************************************************************
 local types
 */
typedef struct RxD3D9DynamicVertexBuffer RxD3D9DynamicVertexBuffer;
struct RxD3D9DynamicVertexBuffer
{
    RwUInt32            size;
    RwUInt32            used;
    LPVERTEXBUFFER vertexBuffer;
    LPVERTEXBUFFER *variableAddress;
    RxD3D9DynamicVertexBuffer *next;
};

typedef struct _rwD3D9freeVBlistEntry RxD3D9freeVBlistEntry;
struct _rwD3D9freeVBlistEntry
{
    RwUInt32                offset;
    RwUInt32                size;
    void                    *vertexBuffer;
    RxD3D9freeVBlistEntry  *next;
    RxD3D9freeVBlistEntry  *prev;
};

typedef struct _rwD3D9createdVBlistEntry RxD3D9createdVBlistEntry;
struct _rwD3D9createdVBlistEntry
{
    void        *vertexBuffer;
    RxD3D9createdVBlistEntry  *next;
};

typedef struct _rwD3D9StrideEntry RxD3D9StrideEntry;
struct _rwD3D9StrideEntry
{
    RwUInt32                    stride;
    RxD3D9freeVBlistEntry       *freelist;
    RxD3D9createdVBlistEntry    *vblist;
    RxD3D9StrideEntry           *next;
};

/****************************************************************************
 Static variables
 */

static RwUInt32         DefaultVBSize = 128 * 1024;

static RxD3D9StrideEntry    *StrideList = NULL;

static RwFreeList       *StrideFreeList = NULL;
static RwFreeList       *FreeVBFreeList = NULL;
static RwFreeList       *CreatedVBFreeList = NULL;

#if defined(RWDEBUG)
static RwInt32          BlocksCreated = 0;
static RwInt32          VBCreated = 0;
static RwInt32          BytesReserved = 0;
#endif /* defined(RWDEBUG) */

/* Dynamic Vertex Buffer cache */
static RxD3D9DynamicVertexBuffer *DynamicVertexBufferList = NULL;
static RwFreeList                *DynamicVertexBufferFreeList = NULL;

#if defined(RWDEBUG)
static RwUInt32     NumDynamicVertexBuffer = 0;
#endif /* defined(RWDEBUG) */

#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER       4
#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE (256*1024)

static RwUInt32 CurrentDynamicVertexBufferManager = 0;
static RwUInt32 OffSetDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER] = { 0 };
static RwUInt32 SizeDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER] = { 0 };
static LPVERTEXBUFFER DynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER] = { NULL };

/****************************************************************************
 Functions
 */

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9CreateVertexBuffer creates a managed vertex buffer of a specified
 * size aligned to the stride provided.
 *
 * The vertex buffer manager tries to share the same vertex buffer
 * across different atomics and world sectors to reduce the number of vertex buffer
 * switches at runtime. That is the reason this function returns an \e offset, to
 * indicate the beginning of the allocated block on the shared vertex buffer.
 *
 * \param stride Size of the vertex in \e bytes. This value must be a multiple of 4.
 * \param size Size of the buffer in \e bytes. This must be large enough to store
 * at least one vertex of the format requested.
 * \param vertexBuffer Void pointer to the LPDIRECT3DVERTEXBUFFER9 that is to be
 * the vertex buffer.
 * \param offset Offset from the beginning of the shared vertex buffer.
 *
 * \return TRUE if the vertex buffer was successfully created, FALSE otherwise.
 *
 * \see RwD3D9DestroyVertexBuffer
 * \see RwD3D9DynamicVertexBufferCreate
 * \see RwD3D9DynamicVertexBufferDestroy
 *
 */
RwBool
RwD3D9CreateVertexBuffer(RwUInt32 stride,
                         RwUInt32 size,
                         void **vertexBuffer,
                         RwUInt32 *offset)
{
    RxD3D9StrideEntry           *strideList;
    RxD3D9freeVBlistEntry       *freelist;
    RxD3D9createdVBlistEntry    *vblist;
    RwUInt32                    nextOffset;

    RWAPIFUNCTION(RWSTRING("RwD3D9CreateVertexBuffer"));
    RWASSERT(stride <= size);
    RWASSERT((stride & 0x3) == 0);
    RWASSERT(vertexBuffer != NULL);
    RWASSERT(offset != NULL);

    /* Search for an available stride */
    strideList = StrideList;
    while (strideList != NULL)
    {
        RxD3D9StrideEntry  *next = strideList->next;

        if (stride == strideList->stride)
        {
            break;
        }

        strideList = next;
    }

    /* create one if there isn't any */
    if (strideList == NULL)
    {
        strideList = (RxD3D9StrideEntry *)RwFreeListAlloc(StrideFreeList,
            rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

        strideList->stride = stride;
        strideList->freelist = NULL;
        strideList->vblist = NULL;

        /* Add it to the begining */
        strideList->next = StrideList;

        StrideList = strideList;
    }

    /* Search for an available free space */
    freelist = strideList->freelist;
    while (freelist != NULL)
    {
        RxD3D9freeVBlistEntry   *nextFreelist = freelist->next;

        if (freelist->size >= size)
        {
            break;
        }

        freelist = nextFreelist;
    }

    /* If there is not free space, create a new VB */
    if (freelist == NULL)
    {
        freelist = (RxD3D9freeVBlistEntry *)RwFreeListAlloc(FreeVBFreeList,
            rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

        freelist->offset = 0;

        freelist->size = (((DefaultVBSize + (stride - 1)) / stride) * stride);

        if (size >= freelist->size)
        {
            freelist->size = size;
        }

        if (D3D_OK !=
            IDirect3DDevice9_CreateVertexBuffer(_RwD3DDevice,
                                                freelist->size,
                                                D3DUSAGE_WRITEONLY,
                                                0,
                                                D3DPOOL_MANAGED,
                                                (LPVERTEXBUFFER *)&(freelist->vertexBuffer),
                                                NULL))
        {
            RwFreeListFree(FreeVBFreeList, freelist);

            RWRETURN(FALSE);
        }

        /* Add it to the begining */
        freelist->next = strideList->freelist;
        freelist->prev = NULL;

        if (strideList->freelist != NULL)
        {
            strideList->freelist->prev = freelist;
        }

        strideList->freelist = freelist;

        /* Add an entry to the vb list */
        vblist = (RxD3D9createdVBlistEntry *)RwFreeListAlloc(CreatedVBFreeList,
            rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

        vblist->vertexBuffer = freelist->vertexBuffer;

        vblist->next = strideList->vblist;

        strideList->vblist = vblist;

        #if defined(RWDEBUG)
        VBCreated++;
        #endif /* defined(RWDEBUG) */
    }

    *vertexBuffer = freelist->vertexBuffer;
    *offset = freelist->offset;

    nextOffset = (*offset) + size;

    RWASSERT(freelist->size >= (nextOffset - freelist->offset));
    freelist->size -= nextOffset - freelist->offset;

    freelist->offset = nextOffset;

    if (freelist->size <= 0)
    {
        if (freelist->prev != NULL)
        {
            freelist->prev->next = freelist->next;
        }

        if (freelist->next != NULL)
        {
            freelist->next->prev = freelist->prev;
        }

        if (strideList->freelist == freelist)
        {
            strideList->freelist = freelist->next;
        }

        RwFreeListFree(FreeVBFreeList, freelist);
    }

    #if defined(RWDEBUG)
    BlocksCreated++;
    BytesReserved+=size;
    #endif /* defined(RWDEBUG) */

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DestroyVertexBuffer releases a block of a previously allocated
 * managed vertex buffer. This block could either be a part of or the whole of
 * the vertex buffer. The stride, size and offset parameters are used to identify
 * the block in the vertex buffer.
 *
 * \param stride Size of the vertex in \e bytes. This value must be a multiple of 4.
 * \param size Size of the buffer in \e bytes. This must be large enough to store
 * at least one vertex of the format requested.
 * \param vertexBuffer Void pointer to the LPDIRECT3DVERTEXBUFFER9 that is to be
 * released.
 * \param offset Offset from the beginning of the shared vertex buffer.
 *
 * \see RwD3D9CreateVertexBuffer
 * \see RwD3D9DynamicVertexBufferCreate
 * \see RwD3D9DynamicVertexBufferDestroy
 *
 */
void
RwD3D9DestroyVertexBuffer(RwUInt32 stride,
                          RwUInt32 size,
                          void *vertexBuffer,
                          RwUInt32 offset)
{
    RxD3D9StrideEntry       *strideList;
    RxD3D9freeVBlistEntry   *freelist;

    RWAPIFUNCTION(RWSTRING("RwD3D9DestroyVertexBuffer"));
    RWASSERT(stride <= size);
    RWASSERT((stride & 0x3) == 0);
    RWASSERT(vertexBuffer != NULL);

    /* Search for the stride */
    strideList = StrideList;
    while (strideList != NULL)
    {
        RxD3D9StrideEntry  *next = strideList->next;

        if (stride == strideList->stride)
        {
            break;
        }

        strideList = next;
    }

    if (strideList != NULL)
    {
        /* Search for an available free space to add */
        freelist = strideList->freelist;
        while (freelist != NULL)
        {
            RxD3D9freeVBlistEntry   *nextFreelist = freelist->next;

            if (freelist->vertexBuffer == vertexBuffer)
            {
                /* Could be added to the end of the free space? */
                if ( (freelist->offset + freelist->size) == offset )
                {
                    break;
                }
                else
                {
                    /* Could be added to the begin of the free space? */
                    if ( offset + size == freelist->offset )
                    {
                        break;
                    }
                }
            }

            freelist = nextFreelist;
        }

        if (freelist != NULL)
        {
            RwBool      canCompactMore = FALSE;
            RwUInt32    newoffset = offset;
            RwUInt32    newSize = size;
            RxD3D9freeVBlistEntry   *oldFreelist = freelist;

            do
            {
                /* Could be added to the end of the free space? */
                if ( (freelist->offset + freelist->size) == newoffset )
                {
                    freelist->size += newSize;
                }
                else
                {
                    /* Could be added to the begin of the free space? */
                    if ( newoffset + newSize == freelist->offset )
                    {
                        freelist->offset = newoffset;

                        freelist->size += newSize;
                    }
                }

                /* Could be compact more? */
                newoffset = freelist->offset;
                newSize = freelist->size;
                oldFreelist = freelist;

                canCompactMore = FALSE;
                freelist = strideList->freelist;
                while (freelist != NULL)
                {
                    RxD3D9freeVBlistEntry   *nextFreelist = freelist->next;

                    if (freelist->vertexBuffer == vertexBuffer)
                    {
                        /* Could be added to the end of the free space? */
                        if ( (freelist->offset + freelist->size) == newoffset )
                        {
                            canCompactMore = TRUE;
                            break;
                        }
                        else
                        {
                            /* Could be added to the begin of the free space? */
                            if ( newoffset + newSize == freelist->offset )
                            {
                                canCompactMore = TRUE;
                                break;
                            }
                        }
                    }

                    freelist = nextFreelist;
                }

                /* Remove old list */
                if (canCompactMore)
                {
                    if (oldFreelist->prev != NULL)
                    {
                        oldFreelist->prev->next = oldFreelist->next;
                    }

                    if (oldFreelist->next != NULL)
                    {
                        oldFreelist->next->prev = oldFreelist->prev;
                    }

                    if (strideList->freelist == oldFreelist)
                    {
                        strideList->freelist = oldFreelist->next;
                    }

                    RwFreeListFree(FreeVBFreeList, oldFreelist);
                }
            }
            while(canCompactMore);
        }
        else
        {
            freelist = (RxD3D9freeVBlistEntry *)RwFreeListAlloc(FreeVBFreeList,
                                        rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

            freelist->offset = offset;

            freelist->size = size;

            freelist->vertexBuffer = vertexBuffer;

            /* Add it to the begining */
            freelist->next = strideList->freelist;
            freelist->prev = NULL;

            if (strideList->freelist != NULL)
            {
                strideList->freelist->prev = freelist;
            }

            strideList->freelist = freelist;
        }

        #if defined(RWDEBUG)
        BlocksCreated--;
        BytesReserved-=size;
        #endif /* defined(RWDEBUG) */
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9VertexBufferManagerChangeDefaultSize is used to change the default
 * vertex buffer size used by the vertex buffer manager to generate new
 * vertex buffers.
 *
 * The vertex buffer manager tries to share the same vertex buffer
 * across different atomics and world sectors to reduce the number of vertex buffer
 * switches at runtime. So the greater the default size, the more objects can
 * fit in the same vertex buffer, but you should be aware that big vertex buffers
 * may fail to be created depending on the video card. Also, some video card
 * drivers have problems to render primitives when the base index does not fit into
 * a 16-bit number. As a general rule we recommend to use sizes under 1 MB and
 * in multiples of 128 KB.
 *
 * \param defaultSize  New default size in bytes for all managed vertex buffers
 * created after calling this function. This value will be rounded up to a multiple
 * of the vertex size.
 *
 */
void
RwD3D9VertexBufferManagerChangeDefaultSize(RwUInt32 defaultSize)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9VertexBufferManagerChangeDefaultSize"));

    DefaultVBSize = defaultSize;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9DynamicVertexBufferManagerDestroy

 */
static void
D3D9DynamicVertexBufferManagerDestroy(void)
{
    RwUInt32            n;

    RWFUNCTION(RWSTRING("D3D9DynamicVertexBufferManagerDestroy"));

    CurrentDynamicVertexBufferManager = 0;

    for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        OffSetDynamicVertexBufferManager[n] = 0;

        SizeDynamicVertexBufferManager[n] = 0;

        if (DynamicVertexBufferManager[n])
        {
            RwD3D9DynamicVertexBufferDestroy(DynamicVertexBufferManager[n]);

            DynamicVertexBufferManager[n] = NULL;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D9DynamicVertexBufferManagerCreate

 */
static RwBool
D3D9DynamicVertexBufferManagerCreate(void)
{
    RwBool      created;
    RwUInt32    n;

    RWFUNCTION(RWSTRING("D3D9DynamicVertexBufferManagerCreate"));

    CurrentDynamicVertexBufferManager = 0;

    if (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
        {
            OffSetDynamicVertexBufferManager[n] = 0;

            SizeDynamicVertexBufferManager[n] = MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE;

            created = RwD3D9DynamicVertexBufferCreate(MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE,
                                            (void **)&(DynamicVertexBufferManager[n]));

            if (!created)
            {
                RWRETURN(FALSE);
            }
        }
    }
    else
    {
        OffSetDynamicVertexBufferManager[0] = 0;

        SizeDynamicVertexBufferManager[0] = MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE;

        created = RwD3D9DynamicVertexBufferCreate(MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE,
                                        (void **)&(DynamicVertexBufferManager[0]));
        if (!created)
        {
            RWRETURN(FALSE);
        }

        for (n = 1; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
        {
            OffSetDynamicVertexBufferManager[n] = 0;

            SizeDynamicVertexBufferManager[n] = 0;

            DynamicVertexBufferManager[n] = NULL;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9DynamicVertexBufferManagerForceDiscard

*/
void
_rwD3D9DynamicVertexBufferManagerForceDiscard(void)
{
    RwUInt32 n;

    RWFUNCTION(RWSTRING("_rwD3D9DynamicVertexBufferManagerForceDiscard"));

    /* Force discard of Dynamic VB */
    for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        OffSetDynamicVertexBufferManager[n] = 0;
    }

    CurrentDynamicVertexBufferManager++;

    if (CurrentDynamicVertexBufferManager >=
        MAX_DYNAMIC_VERTEX_BUFFER_MANAGER)
    {
        CurrentDynamicVertexBufferManager = 0;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9DynamicVertexBufferRelease

 Purpose:   Release all video memory Dinamic vertex buffers

*/
void
_rwD3D9DynamicVertexBufferRelease(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9DynamicVertexBufferRelease"));

    D3D9DynamicVertexBufferManagerDestroy();

    if (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        RxD3D9DynamicVertexBuffer *currentVertexBuffer;

        currentVertexBuffer = DynamicVertexBufferList;
        while (currentVertexBuffer)
        {
            RxD3D9DynamicVertexBuffer *next = currentVertexBuffer->next;

            if (currentVertexBuffer->vertexBuffer)
            {
                IDirect3DVertexBuffer9_Release(currentVertexBuffer->vertexBuffer);

                currentVertexBuffer->vertexBuffer = NULL;

                if (currentVertexBuffer->variableAddress)
                {
                    *currentVertexBuffer->variableAddress = NULL;
                }
            }

            currentVertexBuffer = next;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9DynamicVertexBufferRestore

 Purpose:   Restore all video memory Dinamic vertex buffers

*/
RwBool
_rwD3D9DynamicVertexBufferRestore(void)
{
    HRESULT hr = D3D_OK;

    RWFUNCTION(RWSTRING("_rwD3D9DynamicVertexBufferRestore"));

#if defined(RWDEBUG)
    if (NumDynamicVertexBuffer)
    {
        RwChar              buffer[256];

        rwsprintf(buffer, "Num Dynamic VertexBuffers Recreated: %d",
                  NumDynamicVertexBuffer);

        RwDebugSendMessage(rwDEBUGMESSAGE,
                           "_rwD3D9DynamicVertexBufferRestore",
                           buffer);
    }
#endif

    if (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        RxD3D9DynamicVertexBuffer *currentVertexBuffer;

        currentVertexBuffer = DynamicVertexBufferList;
        while (currentVertexBuffer)
        {
            RxD3D9DynamicVertexBuffer *next = currentVertexBuffer->next;

            if (!currentVertexBuffer->vertexBuffer)
            {
                hr = DXCHECK(IDirect3DDevice9_CreateVertexBuffer
                             (_RwD3DDevice, currentVertexBuffer->size,
                              D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
                              0, D3DPOOL_DEFAULT,
                              &(currentVertexBuffer->vertexBuffer), NULL));

                if (currentVertexBuffer->variableAddress)
                {
                    *currentVertexBuffer->variableAddress =
                        currentVertexBuffer->vertexBuffer;
                }
            }

            if (FAILED(hr))
            {
                RWRETURN(FALSE);
            }

            currentVertexBuffer = next;
        }
    }

    RWRETURN(D3D9DynamicVertexBufferManagerCreate());
}

/****************************************************************************
 D3D9DynamicVertexBufferListCreate

 Purpose:   Initialize the list of Dinamic vertex buffers

*/
static RwBool
D3D9DynamicVertexBufferListCreate(void)
{
    RWFUNCTION(RWSTRING("D3D9DynamicVertexBufferListCreate"));

    DynamicVertexBufferFreeList = RwFreeListCreate(
                                            sizeof(RxD3D9DynamicVertexBuffer),
                                            42, /* It results in ~1024 Bytes */
                                            4,
                                            rwID_DRIVERMODULE |
                                            rwMEMHINTDUR_GLOBAL);

    RWRETURN(DynamicVertexBufferFreeList != NULL);
}

/****************************************************************************
 rwD3D9DynamicVertexBufferListDestroy

 Purpose:   Destroy the list of Dinamic vertex buffers

*/
static void
D3D9DynamicVertexBufferListDestroy(void)
{
    RWFUNCTION(RWSTRING("D3D9DynamicVertexBufferListDestroy"));

    while (DynamicVertexBufferList)
    {
        RxD3D9DynamicVertexBuffer *next = DynamicVertexBufferList->next;

        if (DynamicVertexBufferList->vertexBuffer)
        {
            IDirect3DVertexBuffer9_Release(DynamicVertexBufferList->vertexBuffer);

            DynamicVertexBufferList->vertexBuffer = NULL;

            if (DynamicVertexBufferList->variableAddress)
            {
                *DynamicVertexBufferList->variableAddress = NULL;
            }
        }

        RwFreeListFree(DynamicVertexBufferFreeList, DynamicVertexBufferList);

        DynamicVertexBufferList = next;
    }

    if (DynamicVertexBufferFreeList != NULL)
    {
        RwFreeListDestroy(DynamicVertexBufferFreeList);
        DynamicVertexBufferFreeList = NULL;
    }

#if defined(RWDEBUG)
    NumDynamicVertexBuffer = 0;
#endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DynamicVertexBufferCreate creates a vertex buffer of a specified
 * size.
 *
 * If an "unused" buffer already exists, of the same size,
 * then this will be used in preference to constructing a new vertex buffer.
 *
 * Otherwise, a new vertex buffer is created. If the host video card driver
 * supports hardware TnL then it will created in video memory, otherwise in system
 * memory.
 *
 * While RWD3D9 maintains it's internal cache, vertex buffers will function at
 * optimum performance.
 *
 * \param size Size of the buffer in \e bytes. This must be large enough to store
 * at least one vertex of the format requested.
 * \param vertexBuffer Void pointer to the LPDIRECT3DVERTEXBUFFER9 that is to be
 * the vertex buffer.
 *
 * \return TRUE if the vertex buffer was successfully created, FALSE otherwise.
 *
 * \see RwD3D9DynamicVertexBufferDestroy
 * \see RwD3D9CreateVertexBuffer
 * \see RwD3D9DestroyVertexBuffer
 *
 */
RwBool
RwD3D9DynamicVertexBufferCreate(RwUInt32 size,
                                void **vertexBuffer)
{
    RxD3D9DynamicVertexBuffer *currentVertexBuffer;
    RxD3D9DynamicVertexBuffer *freeVertexBuffer = NULL;

    RWAPIFUNCTION(RWSTRING("RwD3D9DynamicVertexBufferCreate"));

    currentVertexBuffer = DynamicVertexBufferList;
    while (currentVertexBuffer)
    {
        RxD3D9DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (!currentVertexBuffer->used)
        {
            if (currentVertexBuffer->size == size)
            {
                break;
            }
            else if (freeVertexBuffer == NULL
                     || freeVertexBuffer->size <
                     currentVertexBuffer->size)
            {
                freeVertexBuffer = currentVertexBuffer;
            }
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = TRUE;

        *vertexBuffer = currentVertexBuffer->vertexBuffer;

        currentVertexBuffer->variableAddress =
            (LPVERTEXBUFFER *) vertexBuffer;
    }
    else
    {
        D3DPOOL             d3dPool;

        /* Destroy a free vertex buffer to make room for the new one */
        if (freeVertexBuffer)
        {
            IDirect3DVertexBuffer9_Release(freeVertexBuffer->vertexBuffer);
            freeVertexBuffer->vertexBuffer = NULL;

            currentVertexBuffer = freeVertexBuffer;

            #if defined(RWDEBUG)
            NumDynamicVertexBuffer--;
            #endif /* defined(RWDEBUG) */
        }
        else
        {
            /* Create a new item */
            currentVertexBuffer = (RxD3D9DynamicVertexBuffer *)
                                RwFreeListAlloc(DynamicVertexBufferFreeList,
                                    rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL);

            /* Add it to the beginnig of the list */
            currentVertexBuffer->next = DynamicVertexBufferList;

            DynamicVertexBufferList = currentVertexBuffer;
        }

        if (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        {
            d3dPool = D3DPOOL_DEFAULT;
        }
        else
        {
            d3dPool = D3DPOOL_SYSTEMMEM;
        }

        if (D3D_OK !=
            DXCHECK(IDirect3DDevice9_CreateVertexBuffer
                    (_RwD3DDevice, size,
                     D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
                     0, d3dPool,
                     ((LPVERTEXBUFFER *) vertexBuffer),
                     NULL)))
        {
            RWRETURN(FALSE);
        }

        /* Fill item info */
        currentVertexBuffer->size = size;
        currentVertexBuffer->used = TRUE;
        currentVertexBuffer->vertexBuffer =
            ((LPVERTEXBUFFER) *vertexBuffer);
        currentVertexBuffer->variableAddress =
            ((LPVERTEXBUFFER *) vertexBuffer) ;

        #if defined(RWDEBUG)
        NumDynamicVertexBuffer++;
        #endif /* defined(RWDEBUG) */
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DynamicVertexBufferDestroy destroys a previously created vertex buffer.
 *
 * To be precise, the vertex buffer is not deallocated from memory, but is instead
 * declared to be "unused", and may therefore be reused by a future call to
 * \ref RwD3D9DynamicVertexBufferCreate should the size and vertex formats requested
 * coincide.
 *
 * \param vertexBuffer Void pointer to the vertex buffer to destroy.
 *
 * \see RwD3D9DynamicVertexBufferCreate
 * \see RwD3D9CreateVertexBuffer
 * \see RwD3D9DestroyVertexBuffer
 *
 */
void
RwD3D9DynamicVertexBufferDestroy(void *vertexBuffer)
{
    RxD3D9DynamicVertexBuffer *currentVertexBuffer;

    RWAPIFUNCTION(RWSTRING("RwD3D9DynamicVertexBufferDestroy"));

    currentVertexBuffer = DynamicVertexBufferList;
    while (currentVertexBuffer)
    {
        RxD3D9DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (currentVertexBuffer->vertexBuffer ==
            ((LPVERTEXBUFFER) vertexBuffer))
        {
            break;
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = FALSE;

        currentVertexBuffer->variableAddress = NULL;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DynamicVertexBufferLock locks an available section of an internally
 * managed dynamic vertex buffer ready for use by an application. The vertex format size
 * and number of vertices required defines the size of the section in the vertex buffer.
 * Pointers to the vertex buffer and the start of the vertex data are returned through
 * \e vertexBufferOut and \e vertexDataOut, respectively. The base index of the vertex
 * data block in the vertex buffer is also returned in \e baseIndexOut.
 *
 * \warning It is advisable \e not to use any D3D9 functions on \e vertexBufferOut since this
 * will interfere with the internal management of the vertex buffers resulting in
 * undefined behavior from RWD3D9.
 *
 * \note The vertex buffer manager running on these internally defined vertex buffers
 * is different from that which manages user-created vertex buffers with
 * \ref RwD3D9DynamicVertexBufferCreate and \ref RwD3D9DynamicVertexBufferDestroy.
 *
 * \param vertexSize Size of the vertex format to use in the vertex buffer, in bytes.
 * \param numVertex Number of vertices of the desired format to use in the vertex buffer.
 * \param vertexBufferOut Void pointer to the locked vertex buffer.
 * \param vertexDataOut Void pointer to the available vertex data block.
 * \param baseIndexOut Base index to the available vertex data block.
 *
 * \return TRUE upon successfully locking a vertex buffer, FALSE otherwise.
 *
 * \see RwD3D9DynamicVertexBufferCreate
 * \see RwD3D9DynamicVertexBufferDestroy
 * \see RwD3D9DynamicVertexBufferUnlock
 * \see RwD3D9CreateVertexBuffer
 * \see RwD3D9DestroyVertexBuffer
 *
 */
RwBool
RwD3D9DynamicVertexBufferLock(RwUInt32 vertexSize,
                              RwUInt32 numVertex,
                              void **vertexBufferOut,
                              void **vertexDataOut,
                              RwUInt32 * baseIndexOut)
{
    RwUInt32            size;
    RwUInt32            offsetAligned;
    RwUInt32            nextOffsetAligned;
    HRESULT             hr;

    RWAPIFUNCTION(RWSTRING("RwD3D9DynamicVertexBufferLock"));
    RWASSERT(vertexBufferOut);
    RWASSERT(vertexDataOut);
    RWASSERT(baseIndexOut);

    size = numVertex * vertexSize;

    if (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        offsetAligned =
            ((OffSetDynamicVertexBufferManager[CurrentDynamicVertexBufferManager] +
            (vertexSize - 1)) / vertexSize) *
            vertexSize;

        nextOffsetAligned = (offsetAligned + size);

        if (nextOffsetAligned >
            SizeDynamicVertexBufferManager[CurrentDynamicVertexBufferManager])
        {
            RwUInt32            n;

            /* Find if there is a VB with empty space */
            for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
            {
                if (n != CurrentDynamicVertexBufferManager)
                {
                    offsetAligned =
                        ((OffSetDynamicVertexBufferManager[n] +
                          (vertexSize - 1)) / vertexSize) * vertexSize;

                    nextOffsetAligned = (offsetAligned + size);

                    if (nextOffsetAligned <=
                        SizeDynamicVertexBufferManager[n])
                    {
                        CurrentDynamicVertexBufferManager = n;
                        break;
                    }
                }
            }

            if (n >= MAX_DYNAMIC_VERTEX_BUFFER_MANAGER)
            {
                CurrentDynamicVertexBufferManager++;

                if (CurrentDynamicVertexBufferManager >=
                    MAX_DYNAMIC_VERTEX_BUFFER_MANAGER)
                {
                    CurrentDynamicVertexBufferManager = 0;
                }

                offsetAligned = 0;

                nextOffsetAligned = size;
            }
        }
    }
    else
    {
        offsetAligned = 0;

        nextOffsetAligned = size;
    }

    /* Check if we need a greater vb */
    if (size > SizeDynamicVertexBufferManager[CurrentDynamicVertexBufferManager])
    {
        RwD3D9DynamicVertexBufferDestroy(DynamicVertexBufferManager[CurrentDynamicVertexBufferManager]);

        SizeDynamicVertexBufferManager[CurrentDynamicVertexBufferManager] = size;

        RwD3D9DynamicVertexBufferCreate(size,
                                        (void **)
                                        &(DynamicVertexBufferManager[CurrentDynamicVertexBufferManager]));
    }

    if (offsetAligned == 0)
    {
        hr = IDirect3DVertexBuffer9_Lock(DynamicVertexBufferManager[CurrentDynamicVertexBufferManager],
                                         0, size,
                                         (RwUInt8 **) vertexDataOut,
                                         D3DLOCK_DISCARD |
                                         D3DLOCK_NOSYSLOCK);

        OffSetDynamicVertexBufferManager[CurrentDynamicVertexBufferManager] = size;

        *baseIndexOut = 0;
    }
    else
    {
        hr = IDirect3DVertexBuffer9_Lock(DynamicVertexBufferManager[CurrentDynamicVertexBufferManager],
                                         offsetAligned, size,
                                         (RwUInt8 **) vertexDataOut,
                                         D3DLOCK_NOOVERWRITE |
                                         D3DLOCK_NOSYSLOCK);

        OffSetDynamicVertexBufferManager[CurrentDynamicVertexBufferManager] = nextOffsetAligned;

        *baseIndexOut = offsetAligned / vertexSize;
    }

    *vertexBufferOut =
        DynamicVertexBufferManager[CurrentDynamicVertexBufferManager];

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DynamicVertexBufferUnlock unlocks a previously locked internally
 * managed vertex buffer that was provided through \ref RwD3D9DynamicVertexBufferLock.
 *
 * \note The vertex buffer manager running on these internally defined vertex buffers
 * is different from that which manages user-created vertex buffers with
 * \ref RwD3D9DynamicVertexBufferCreate and \ref RwD3D9DynamicVertexBufferDestroy.
 *
 * \param vertexBufferIn Void pointer to the vertex buffer to unlock.
 *
 * \return TRUE upon successfully unlocking the vertex buffer, FALSE otherwise.
 *
 * \see RwD3D9DynamicVertexBufferCreate
 * \see RwD3D9DynamicVertexBufferDestroy
 * \see RwD3D9DynamicVertexBufferLock
 *
 */
RwBool
RwD3D9DynamicVertexBufferUnlock(void *vertexBufferIn)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9DynamicVertexBufferUnlock"));

    RWRETURN(SUCCEEDED
             (IDirect3DVertexBuffer9_Unlock
              ((LPVERTEXBUFFER) vertexBufferIn)));
}

/****************************************************************************
 _rwD3D9VertexBufferManagerOpen

 Purpose:   Initializes the vertex buffer manager

 On exit :  TRUE on success else FALSE.
*/
RwBool
_rwD3D9VertexBufferManagerOpen(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9VertexBufferManagerOpen"));

    _rwD3D9VertexBufferManagerClose();

    StrideFreeList = RwFreeListCreate(sizeof(RxD3D9StrideEntry), 16, sizeof(RwUInt32),
        rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL);
    FreeVBFreeList = RwFreeListCreate(sizeof(RxD3D9freeVBlistEntry), 100, sizeof(RwUInt32),
        rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL);
    CreatedVBFreeList = RwFreeListCreate(sizeof(RxD3D9createdVBlistEntry), 100, sizeof(RwUInt32),
        rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL);

    D3D9DynamicVertexBufferListCreate();

    D3D9DynamicVertexBufferManagerCreate();

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9VertexBufferManagerClose

 Purpose:   Destroy the vertex buffer manager

*/
void
_rwD3D9VertexBufferManagerClose(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9VertexBufferManagerClose"));

#if defined(RWDEBUG)
    RWASSERT(BlocksCreated == 0);
    RWASSERT(BytesReserved == 0);
#endif /* defined(RWDEBUG) */

    D3D9DynamicVertexBufferManagerDestroy();

    D3D9DynamicVertexBufferListDestroy();

    while (StrideList != NULL)
    {
        RxD3D9StrideEntry  *next = StrideList->next;

        /*  Destroy the free list */
        while (StrideList->freelist != NULL)
        {
            RxD3D9freeVBlistEntry   *nextFreelist = StrideList->freelist->next;

            RwFreeListFree(FreeVBFreeList, StrideList->freelist);

            StrideList->freelist = nextFreelist;
        }

        /*  Destroy the VB list */
        while (StrideList->vblist != NULL)
        {
            RxD3D9createdVBlistEntry   *nextvblist = StrideList->vblist->next;

            if (StrideList->vblist->vertexBuffer != NULL)
            {
                IDirect3DVertexBuffer9_Release((LPVERTEXBUFFER)(StrideList->vblist->vertexBuffer));
            }

            RwFreeListFree(CreatedVBFreeList, StrideList->vblist);

            StrideList->vblist = nextvblist;
        }

        RwFreeListFree(StrideFreeList, StrideList);

        StrideList = next;
    }

    if (CreatedVBFreeList != NULL)
    {
        RwFreeListDestroy(CreatedVBFreeList);
        CreatedVBFreeList = NULL;
    }

    if (FreeVBFreeList != NULL)
    {
        RwFreeListDestroy(FreeVBFreeList);
        FreeVBFreeList = NULL;
    }

    if (StrideFreeList != NULL)
    {
        RwFreeListDestroy(StrideFreeList);
        StrideFreeList = NULL;
    }

#if defined(RWDEBUG)
    BlocksCreated = 0;
    VBCreated = 0;
    BytesReserved = 0;
#endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}
