/****************************************************************************
 *                                                                          *
 * module : D3D8VertexBufferManager.c                                       *
 *                                                                          *
 * purpose:                                                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#include "D3D8VertexBufferManager.h"

#include "baworld.h"

/****************************************************************************
 Global variables
 */

/****************************************************************************
 local defines
 */

/****************************************************************************
 local types
 */

typedef struct _rxD3D8freeVBlistEntry RxD3D8freeVBlistEntry;
struct _rxD3D8freeVBlistEntry
{
    RwUInt32                offset;
    RwUInt32                size;
    void                    *vertexBuffer;
    RxD3D8freeVBlistEntry  *next;
    RxD3D8freeVBlistEntry  *prev;
};

typedef struct _rxD3D8createdVBlistEntry RxD3D8createdVBlistEntry;
struct _rxD3D8createdVBlistEntry
{
    void        *vertexBuffer;
    RxD3D8createdVBlistEntry  *next;
};

typedef struct _rxD3D8fvfEntry RxD3D8fvfEntry;
struct _rxD3D8fvfEntry
{
    RwUInt32                    fvf;
    RxD3D8freeVBlistEntry      *freelist;
    RxD3D8createdVBlistEntry   *vblist;
    RxD3D8fvfEntry             *next;
};

/****************************************************************************
 Static variables
 */

static RwUInt32         DefaultVBSize = 128 * 1024;

static RxD3D8fvfEntry   *FvfList = NULL;

static RwFreeList       *FvfFreeList = NULL;
static RwFreeList       *FreeVBFreeList = NULL;
static RwFreeList       *CreatedVBFreeList = NULL;

#if defined(RWDEBUG)
static RwInt32          BlocksCreated = 0;
static RwInt32          VBCreated = 0;
static RwInt32          BytesReserved = 0;
#endif /* defined(RWDEBUG) */

/****************************************************************************
 Functions
 */

/****************************************************************************
 D3D8GetFVFVertexSize

 Purpose:   Returns the vertex size in bytes of the FVF

*/
static RwUInt32
D3D8GetFVFVertexSize(RwUInt32 fvf)
{
    RwUInt32 size;
    RwUInt32 numTexCoords;

    RWFUNCTION(RWSTRING("D3D8GetFVFVertexSize"));

    size = 0;

    if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZ)
    {
        size += 3 * sizeof(float);
    }
    else if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZRHW)
    {
        size += 4 * sizeof(float);
    }
    else if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZB1)
    {
        size += 4 * sizeof(float);
    }
    else if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZB2)
    {
        size += 5 * sizeof(float);
    }
    else if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZB3)
    {
        size += 6 * sizeof(float);
    }
    else if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZB4)
    {
        size += 7 * sizeof(float);
    }
    else if ((fvf & D3DFVF_POSITION_MASK) == D3DFVF_XYZB5)
    {
        size += 8 * sizeof(float);
    }

    if (fvf & D3DFVF_NORMAL)
    {
        size += 3 * sizeof(float);
    }

    if (fvf & D3DFVF_PSIZE)
    {
        size += sizeof(float);
    }

    if (fvf & D3DFVF_DIFFUSE)
    {
        size += sizeof(D3DCOLOR);
    }

    if (fvf & D3DFVF_SPECULAR)
    {
        size += sizeof(D3DCOLOR);
    }

    numTexCoords = ((fvf & D3DFVF_TEXCOUNT_MASK)>>D3DFVF_TEXCOUNT_SHIFT);

    while(numTexCoords)
    {
        numTexCoords--;

        if (fvf & D3DFVF_TEXCOORDSIZE1(numTexCoords))
        {
            size += sizeof(float);
        }
        else if (fvf & D3DFVF_TEXCOORDSIZE3(numTexCoords))
        {
            size += 3 * sizeof(float);
        }
        else if (fvf & D3DFVF_TEXCOORDSIZE4(numTexCoords))
        {
            size += 4 * sizeof(float);
        }
        else    /* default value */
        {
            size += 2 * sizeof(float);
        }
    }

    RWRETURN(size);
}

/****************************************************************************
 _rxD3D8VertexBufferManagerOpen

 Purpose:   Initializes the vertex buffer manager

 On exit :  TRUE on success else FALSE.
*/
RwBool
_rxD3D8VertexBufferManagerOpen(void)
{
    RWFUNCTION(RWSTRING("_rxD3D8VertexBufferManagerOpen"));

    _rxD3D8VertexBufferManagerClose();

    FvfFreeList = RwFreeListCreate(sizeof(RxD3D8fvfEntry), 16, sizeof(RwUInt32),
                                   rwMEMHINTDUR_GLOBAL | rwID_WORLDPIPEMODULE);
    FreeVBFreeList = RwFreeListCreate(sizeof(RxD3D8freeVBlistEntry), 100, sizeof(RwUInt32),
                                      rwMEMHINTDUR_GLOBAL | rwID_WORLDPIPEMODULE);
    CreatedVBFreeList = RwFreeListCreate(sizeof(RxD3D8createdVBlistEntry), 100, sizeof(RwUInt32),
                                         rwMEMHINTDUR_GLOBAL | rwID_WORLDPIPEMODULE);

    RWRETURN(TRUE);
}

/****************************************************************************
 _rxD3D8VertexBufferManagerClose

 Purpose:   Destroy the vertex buffer manager

*/
void
_rxD3D8VertexBufferManagerClose(void)
{
    RWFUNCTION(RWSTRING("_rxD3D8VertexBufferManagerClose"));

#if defined(RWDEBUG)
    RWASSERT(BlocksCreated == 0);
    RWASSERT(BytesReserved == 0);
#endif /* defined(RWDEBUG) */

    while (FvfList != NULL)
    {
        RxD3D8fvfEntry  *next = FvfList->next;

        /*  Destroy the free list */
        while (FvfList->freelist != NULL)
        {
            RxD3D8freeVBlistEntry   *nextFreelist = FvfList->freelist->next;

            RwFreeListFree(FreeVBFreeList, FvfList->freelist);

            FvfList->freelist = nextFreelist;
        }

        /*  Destroy the VB list */
        while (FvfList->vblist != NULL)
        {
            RxD3D8createdVBlistEntry   *nextvblist = FvfList->vblist->next;

            if (FvfList->vblist->vertexBuffer != NULL)
            {
                IDirect3DVertexBuffer8_Release((LPDIRECT3DVERTEXBUFFER8)(FvfList->vblist->vertexBuffer));
            }

            RwFreeListFree(CreatedVBFreeList, FvfList->vblist);

            FvfList->vblist = nextvblist;
        }

        RwFreeListFree(FvfFreeList, FvfList);

        FvfList = next;
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

    if (FvfFreeList != NULL)
    {
        RwFreeListDestroy(FvfFreeList);
        FvfFreeList = NULL;
    }

#if defined(RWDEBUG)
    BlocksCreated = 0;
    VBCreated = 0;
    BytesReserved = 0;
#endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D8VertexBufferManagerCreateFVFStride

 Purpose:   Create a vertex buffer or returns one that has free space on it

 On entry:  fvf             - Flexible vertex format desired
            vertexSize      - Size of the vertex in bytes
            size            - Size needed in bytes
            vertexBuffer    - Pointer to the return vertex buffer
            base index      - value to the returned base index for the vertex

 On exit :  TRUE on success else FALSE.
*/
static RwBool
_rxD3D8VertexBufferManagerCreateFVFStride(RwUInt32 fvf,
                                          RwUInt32 vertexSize,
                                          RwUInt32 size,
                                          void **vertexBuffer,
                                          RwUInt32 *baseIndex)
{
    RxD3D8fvfEntry              *fvfList;
    RxD3D8freeVBlistEntry       *freelist;
    RxD3D8createdVBlistEntry    *vblist;
    RwUInt32                    nextOffset;

    RWFUNCTION(RWSTRING("_rxD3D8VertexBufferManagerCreateFVFStride"));

    /* Search for an available fvf */
    fvfList = FvfList;
    while (fvfList != NULL)
    {
        RxD3D8fvfEntry  *next = fvfList->next;

        if (fvf == fvfList->fvf)
        {
            break;
        }

        fvfList = next;
    }

    /* create one if there isn't any */
    if (fvfList == NULL)
    {
        fvfList = (RxD3D8fvfEntry *)RwFreeListAlloc(FvfFreeList,
                     rwID_WORLDPIPEMODULE | rwMEMHINTDUR_EVENT);

        fvfList->fvf = fvf;
        fvfList->freelist = NULL;
        fvfList->vblist = NULL;

        /* Add it to the begining */
        fvfList->next = FvfList;

        FvfList = fvfList;
    }

    /* Search for an available free space */
    freelist = fvfList->freelist;
    while (freelist != NULL)
    {
        RxD3D8freeVBlistEntry   *nextFreelist = freelist->next;

        if (freelist->size >= size)
        {
            break;
        }

        freelist = nextFreelist;
    }

    /* If there is not free space, create a new VB */
    if (freelist == NULL)
    {
        freelist = (RxD3D8freeVBlistEntry *)RwFreeListAlloc(FreeVBFreeList,
            rwID_WORLDPIPEMODULE | rwMEMHINTDUR_EVENT);

        freelist->offset = 0;

        freelist->size = (((DefaultVBSize + (vertexSize - 1)) / vertexSize) * vertexSize);

        if (size >= freelist->size)
        {
            freelist->size = size;
        }

        if (D3D_OK !=
            IDirect3DDevice8_CreateVertexBuffer(_RwD3DDevice,
                                                freelist->size,
                                                D3DUSAGE_WRITEONLY,
                                                (fvf <= 0xffff ? fvf : 0),
                                                D3DPOOL_MANAGED,
                                                (LPDIRECT3DVERTEXBUFFER8 *)&(freelist->vertexBuffer)))
        {
            RwFreeListFree(FreeVBFreeList, freelist);

            RWRETURN(FALSE);
        }

        /* Add it to the begining */
        freelist->next = fvfList->freelist;
        freelist->prev = NULL;

        if (fvfList->freelist != NULL)
        {
            fvfList->freelist->prev = freelist;
        }

        fvfList->freelist = freelist;

        /* Add an entry to the vb list */
        vblist = (RxD3D8createdVBlistEntry *)RwFreeListAlloc(CreatedVBFreeList,
            rwID_WORLDPIPEMODULE | rwMEMHINTDUR_EVENT);

        vblist->vertexBuffer = freelist->vertexBuffer;

        vblist->next = fvfList->vblist;

        fvfList->vblist = vblist;

        #if defined(RWDEBUG)
        VBCreated++;
        #endif /* defined(RWDEBUG) */
    }

    *vertexBuffer = freelist->vertexBuffer;
    *baseIndex = ((freelist->offset + (vertexSize-1)) / vertexSize);

    nextOffset = ((*baseIndex) * vertexSize) + size;

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

        if (fvfList->freelist == freelist)
        {
            fvfList->freelist = freelist->next;
        }

        RwFreeListFree(FreeVBFreeList, freelist);
    }

    #if defined(RWDEBUG)
    BlocksCreated++;
    BytesReserved+=size;
    #endif /* defined(RWDEBUG) */

    RWRETURN(TRUE);
}

/****************************************************************************
 _rxD3D8VertexBufferManagerDestroyFVFStride

 Purpose:   Destroy a vertex buffer or frees the space reserved

 On entry:  fvf             - Flexible vertex format
            vertexSize      - Size of the vertex in bytes
            size            - Size in bytes
            vertexBuffer    - Pointer to the vertex buffer
            base index      - value to the base index for the vertex
*/
static void
_rxD3D8VertexBufferManagerDestroyFVFStride(RwUInt32 fvf, RwUInt32 vertexSize, RwUInt32 size, void *vertexBuffer, RwUInt32 baseIndex)
{
    RxD3D8fvfEntry          *fvfList;
    RxD3D8freeVBlistEntry   *freelist;

    RWFUNCTION(RWSTRING("_rxD3D8VertexBufferManagerDestroyFVFStride"));

    /* Search for the fvf */
    fvfList = FvfList;
    while (fvfList != NULL)
    {
        RxD3D8fvfEntry  *next = fvfList->next;

        if (fvf == fvfList->fvf)
        {
            break;
        }

        fvfList = next;
    }

    if (fvfList != NULL)
    {
        RwUInt32 offset;

        /* Search for an available free space to add */
        offset = baseIndex * vertexSize;

        freelist = fvfList->freelist;
        while (freelist != NULL)
        {
            RxD3D8freeVBlistEntry   *nextFreelist = freelist->next;

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
            RxD3D8freeVBlistEntry   *oldFreelist = freelist;

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
                freelist = fvfList->freelist;
                while (freelist != NULL)
                {
                    RxD3D8freeVBlistEntry   *nextFreelist = freelist->next;

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

                    if (fvfList->freelist == oldFreelist)
                    {
                        fvfList->freelist = oldFreelist->next;
                    }

                    RwFreeListFree(FreeVBFreeList, oldFreelist);
                }
            }
            while(canCompactMore);
        }
        else
        {
            freelist = (RxD3D8freeVBlistEntry *)RwFreeListAlloc(FreeVBFreeList,
                rwID_WORLDPIPEMODULE | rwMEMHINTDUR_EVENT);

            freelist->offset = baseIndex * vertexSize;

            freelist->size = size;

            freelist->vertexBuffer = vertexBuffer;

            /* Add it to the begining */
            freelist->next = fvfList->freelist;
            freelist->prev = NULL;

            if (fvfList->freelist != NULL)
            {
                fvfList->freelist->prev = freelist;
            }

            fvfList->freelist = freelist;
        }

        #if defined(RWDEBUG)
        BlocksCreated--;
        BytesReserved-=size;
        #endif /* defined(RWDEBUG) */
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D8VertexBufferManagerCreate

 Purpose:   Create a vertex buffer or returns one that has free space on it

 On entry:  fvf             - Flexible vertex format desired
            size            - Size needed in bytes
            vertexBuffer    - Pointer to the return vertex buffer
            base index      - value to the returned base index for the vertex

 On exit :  TRUE on success else FALSE.
*/
RwBool
_rxD3D8VertexBufferManagerCreate(RwUInt32 fvf, RwUInt32 size, void **vertexBuffer, RwUInt32 *baseIndex)
{
    RWFUNCTION(RWSTRING("_rxD3D8VertexBufferManagerCreate"));

    RWRETURN(_rxD3D8VertexBufferManagerCreateFVFStride(fvf,
                                              D3D8GetFVFVertexSize(fvf),
                                              size,
                                              vertexBuffer,
                                              baseIndex));
}

/****************************************************************************
 _rxD3D8VertexBufferManagerCreateNoFVF

 Purpose:   Create a vertex buffer or returns one that has free space on it

 On entry:  stride          - Size of the vertex in bytes
            size            - Size needed in bytes
            vertexBuffer    - Pointer to the return vertex buffer
            base index      - value to the returned base index for the vertex

 On exit :  TRUE on success else FALSE.
*/
RwBool
_rxD3D8VertexBufferManagerCreateNoFVF(RwUInt32 stride, RwUInt32 size, void **vertexBuffer, RwUInt32 *baseIndex)
{
    RWFUNCTION(RWSTRING("_rxD3D8VertexBufferManagerCreateNoFVF"));

    /* Create a fake FVF to have separate VB per stride, that way we remove alignment problems */
    RWRETURN(_rxD3D8VertexBufferManagerCreateFVFStride(stride << 16,
                                              stride,
                                              size,
                                              vertexBuffer,
                                              baseIndex));
}

/****************************************************************************
 _rxD3D8VertexBufferManagerDestroy

 Purpose:   Destroy a vertex buffer or frees the space reserved

 On entry:  fvf             - Flexible vertex format
            size            - Size in bytes
            vertexBuffer    - Pointer to the vertex buffer
            base index      - value to the base index for the vertex
*/
void
_rxD3D8VertexBufferManagerDestroy(RwUInt32 fvf, RwUInt32 size, void *vertexBuffer, RwUInt32 baseIndex)
{
    RWFUNCTION(RWSTRING("_rxD3D8VertexBufferManagerDestroy"));

    _rxD3D8VertexBufferManagerDestroyFVFStride(fvf, D3D8GetFVFVertexSize(fvf), size, vertexBuffer, baseIndex);

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D8VertexBufferManagerDestroyNoFVF

 Purpose:   Destroy a vertex buffer or frees the space reserved

 On entry:  stride          - Size of the vertex in bytes
            size            - Size in bytes
            vertexBuffer    - Pointer to the vertex buffer
            base index      - value to the base index for the vertex
*/
void
_rxD3D8VertexBufferManagerDestroyNoFVF(RwUInt32 stride, RwUInt32 size, void *vertexBuffer, RwUInt32 baseIndex)
{
    RWFUNCTION(RWSTRING("_rxD3D8VertexBufferManagerDestroyNoFVF"));

    /* Using a fake FVF to have separate VB per stride, that way we remove alignment problems */
    _rxD3D8VertexBufferManagerDestroyFVFStride(stride << 16, stride, size, vertexBuffer, baseIndex);

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d8
 * \ref RwD3D8VertexBufferManagerChangeDefaultSize is used to change the default
 * vertex buffer size used by the static vertex buffer manager to generate new
 * vertex buffers.
 *
 * The static vertex buffer manager tries to share the same vertex buffer
 * across different atomics and world sectors to reduce the number of vertex buffer
 * switches at runtime. So the greater the default size, the more objects can
 * fit in the same vertex buffer, but you should be aware that big vertex buffers
 * may fail to be created depending on the video card. Also, some video card
 * drivers have problems to render primitives when the base index does not fit into
 * a 16-bit number. As a general rule we recommend to use sizes under 1 MB and
 * in multiples of 128 KB.
 *
 * The world plugin must be attached before using this function.
 *
 * \param defaultSize  New default size in bytes for all static vertex buffers
 * created after calling this function. This value will be rounded up to a multiple
 * of the vertex size.
 *
 */
void
RwD3D8VertexBufferManagerChangeDefaultSize(RwUInt32 defaultSize)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8VertexBufferManagerChangeDefaultSize"));

    DefaultVBSize = defaultSize;

    RWRETURNVOID();
}
