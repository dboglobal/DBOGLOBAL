/*
 * Memory handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwfreelist
 * \page rwfreelistoverview RwFreeList Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection freelistoverview Overview
 *
 * This object provides a platform-neutral memory allocation API.
 *
 * A free list can be used to allocate memory for
 * lists of identical data types.
 * As the list is filled, the free list mechanism can be set to automatically
 * allocate more memory in developer-defined chunks.
 * Getting the most out of this system can require some fine-tuning work,
 * but the result should be an optimal trade-off between
 * the amount of memory used and speed.
 *
 * RenderWare's internal freelists can be configured as appropriate for your
 * application's usage.
 * \see RwCameraSetFreeListCreateParams
 * \see RwFrameSetFreeListCreateParams
 * \see RwImageSetFreeListCreateParams
 * \see RwImageFormatSetFreeListCreateParams
 * \see RwRasterSetFreeListCreateParams
 * \see RwTextureSetFreeListCreateParams
 * \see RwTexDictionarySetFreeListCreateParams
 * \see RwMatrixSetFreeListCreateParams
 * \see RwStreamSetFreeListCreateParams
 * \see RwPluginRegistrySetFreeListCreateParams
 * \see RxPipelineSetFreeListCreateParams
 * \see RpAtomicSetFreeListCreateParams
 * \see RpClumpSetFreeListCreateParams
 * \see RpLightSetFreeListCreateParams
 * \see RpLightTieSetFreeListCreateParams
 * \see RpMaterialSetFreeListCreateParams
 * \see RpTieSetFreeListCreateParams
 * \see RpHAnimHierarchySetFreeListCreateParams
 * \see RpLODAtomicCacheSetFreeListCreateParams
 * \see RpMatFXMaterialDataSetFreeListCreateParams
 * \see RpPatchAtomicSetFreeListCreateParams
 * \see RpPatchGeometrySetFreeListCreateParams
 * \see RpSkinSetFreeListCreateParams
 * \see Rt2dBrushSetFreeListCreateParams
 * \see Rt2dFontSetFreeListCreateParams
 * \see Rt2dFontDictNodeSetFreeListCreateParams
 * \see Rt2dObjectSetFreeListCreateParams
 * \see Rt2dPathSetFreeListCreateParams
 * \see RtAnimAnimationFreeListCreateParams
 *
 * Freelists -- from Page 131
 * Advanced Animation and Rendering Techniques
 * Alan Watt and Mark Watt
 * Addison-Wesley 1993,
 * ISBN 0-201-54412-1:
 *
 * "Lastly, on a more general note concerning speedups for renderers, the
 *  implementor should be aware that a lot of suggestions for improving
 *  efficiency fall into the category of ingenious, but complex,
 *  algorithms for very specific contexts that may save a few microseconds
 *  but which make your code unreadable.  A more general computer science
 *  perspective that takes a `global view' of the renderer can be more
 *  fruitful.  For example, the renderer devotes a lot of time to
 *  allocating and deallocating chunks of memory for storing data. A lot
 *  of these chunks are always the same size - such as those that are
 *  continually required to store the data structure for fragment lists.
 *  Using memory management techniques that recognize this fact can yield
 *  considerable dividends.  One such scheme would be to hold a series of
 *  empty lists in memory for all the commonly used data structures.  An
 *  empty list for fragments, say, would contain a list of previously
 *  allocated, but no longer needed, fragment structures.  When the
 *  renderer needs memory for a new fragment, it looks first at this empty
 *  list.  If there is nothing there it allocates space directly,
 *  otherwise it takes a fragments off the end of the list and uses that.
 *  Conversely, when the renderer no longer needs a fragment, instead of
 *  freeing it, it goes onto the end of the empty list.  In the authors'
 *  experience, replacing the naive allocate/deallocate scheme with this
 *  way of managing memory can result in 100% speedup. "
 */

/**
 * \ingroup rwmem
 * \page rwmemoverview RwMem Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection memoverview Overview
 *
 * A set of utility functions intended to ease cross-platform compatibility of
 * RenderWare data structures.
 *
 * These functions include conversions from RenderWare's intrinsic
 * types - \ref RwInt32, \ref RwInt16, \ref RwReal, etc. - into platform-neutral forms.
 *
 */

#if (!defined(RWKEEPFREELISTS))
#define RWKEEPFREELISTS
#endif /* (!defined(RWKEEPFREELISTS)) */

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "rwstring.h" /* for unicode support */
#include "bamemory.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
*/
static RwBool FreeListsEnabled = TRUE;

/* retrieving recorded __FILE__ and __LINE__ for all allocations */
#if defined(RWDEBUG)
const RwChar   *_rwMemoryLastAllocFile = (RwChar *)NULL;
RwUInt32 _rwMemoryLastAllocLine = 0;
#endif /* defined(RWDEBUG) */

/**
 * \ingroup memoryfileinterface
 * \ref RwMemoryGetLastAllocFile is a function available only in an \c RWDEBUG SDK, that
 * retrieves the value of the C preprocessor symbol \c __FILE__ for the most recent call to
 * the default \ref RwMalloc, \ref RwCalloc or \ref RwRealloc.
 *
 * This function does not exist in non-\c RWDEBUG SDK, so preprocessor predication should be
 * used in an application to avoid compilation errors.
 *
 * \return Pointer to \ref RwChar containing the last recorded \c __FILE__ from an
 * \ref RwMalloc, \ref RwCalloc or \ref RwRealloc.
 */
#if defined(RWDEBUG)
const RwChar *
RwMemoryGetLastAllocFile( void )
{
    RWAPIFUNCTION( RWSTRING( "RwMemoryGetLastAllocFile" ) );

    RWRETURN( _rwMemoryLastAllocFile );
}
#endif /* defined(RWDEBUG) */

/**
 * \ingroup memoryfileinterface
 * \ref RwMemoryGetLastAllocLine is a function available only in an \c RWDEBUG SDK, that
 * retrieves the value of the C preprocessor symbol \c __LINE__ for the most recent call to
 * the default \ref RwMalloc, \ref RwCalloc or \ref RwRealloc.
 *
 * This function does not exist in non-\c RWDEBUG SDK, so preprocessor predication should be
 * used in an application to avoid compilation errors.
 *
 * \return \ref RwUInt32 containing the last recorded \c __LINE__ from an \ref RwMalloc,
 * \ref RwCalloc or \ref RwRealloc.
 */
#if defined(RWDEBUG)
RwUInt32
RwMemoryGetLastAllocLine( void )
{
    RWAPIFUNCTION( RWSTRING( "RwMemoryGetLastAllocLine" ) );

    RWRETURN( _rwMemoryLastAllocLine );
}
#endif /* defined(RWDEBUG) */


void
_rwFreeListEnable(RwBool enabled)
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWFUNCTION(RWSTRING("_rwFreeListEnable"));
#endif
    FreeListsEnabled = enabled;
#if 0
    RWRETURNVOID();
#endif
}

#if (!defined(rwMALLOCALIGNMENT))
#define rwMALLOCALIGNMENT 16
#endif /* (!defined(rwMALLOCALIGNMENT) */

#define rwAlign(_addr, _alignment) \
    (( (RwUInt32)(_addr) + ((_alignment) - 1) ) & ~((_alignment) - 1))

static RwLinkList _freeListList;
static RwBool     _freeListModuleOpen = FALSE;
static RwFreeList _masterFreeList; /* free list for the free lists */
static RwFreeList *_masterFreeListPtr = NULL;

/* in debug builds (when RWDEBUG is defined) alloction and free cause the
   functions to stomp memory with the following bit patterns */
#define ALLOCMASK 0xDD
#define FREEMASK  0xEE

/* free the free list */
static void _RwFreeListFree(RwFreeList *freeList);

static RwFreeList *
FreeListCreate(
    RwUInt32    entrySize,
    RwUInt32    entriesPerBlock, /* must be multiple of 8 */
    RwUInt32    alignment,
    RwUInt32    blocks,
    RwFreeList *freeList,
    RwUInt32    hint);



static RwBool
_rwFreeListModuleOpen( void )
{
    RWFUNCTION("_rwFreeListModuleOpen");
    RWASSERT(FALSE == _freeListModuleOpen);

    rwLinkListInitialize(&_freeListList);
    _freeListModuleOpen = TRUE;

#ifdef RWKEEPFREELISTS
    _masterFreeListPtr = FreeListCreate(sizeof(RwFreeList), 16,
                                      rwMALLOCALIGNMENT, 0,
                                      &_masterFreeList, rwMEMHINTDUR_GLOBAL);
    if (NULL == _masterFreeListPtr)
    {
        _freeListModuleOpen = FALSE;
        RWRETURN(FALSE);
    }
    /* remove master free list from list of free lists so it isn't destroyed
       automatically */
    rwLinkListRemoveLLLink(&_masterFreeListPtr->link);
#endif /* RWKEEPFREELISTS */

    RWRETURN(TRUE);
}


static void
_rwFreeListModuleClose ( void )
{
#ifdef RWKEEPFREELISTS
    RwFreeList *freeList;
    RwLLLink    *link, *lastLink;
#endif /* RWKEEPFREELISTS */
    RWFUNCTION("_rwFreeListModuleClose");
    RWASSERT(_freeListModuleOpen);

#ifdef RWKEEPFREELISTS
    /* clear up all free lists */
    link     = rwLinkListGetFirstLLLink(&_freeListList);
    lastLink = rwLinkListGetTerminator(&_freeListList);
    while (link != lastLink)
    {
        freeList = rwLLLinkGetData(link, RwFreeList, link);
        RwFreeListDestroy(freeList);
        link     = rwLinkListGetFirstLLLink(&_freeListList);
        lastLink = rwLinkListGetTerminator(&_freeListList);
    }

    /* destroy master free list */
    RwFreeListDestroy(_masterFreeListPtr);
    _masterFreeListPtr = NULL;
#endif /* RWKEEPFREELISTS */

    _freeListModuleOpen = FALSE;
    RWRETURNVOID();
}







RwFreeList *
FreeListCreate(
    RwUInt32    entrySize,
    RwUInt32    entriesPerBlock, /* must be multiple of 8 */
    RwUInt32    alignment,
    RwUInt32    blocks,
    RwFreeList *freeList,
    RwUInt32    hint)
{
    RwUInt32    alignedBlockSize;
    RwUInt32    heapSize;
    RWFUNCTION("FreeListCreate");
    RWASSERT(_freeListModuleOpen);
    RWASSERT(entrySize);
    RWASSERT(entriesPerBlock);

    /* Don't allocate space if freelists disabled */
    if (!FreeListsEnabled)
    {
        blocks = 0;
    }

    /* check alignment */
    if (alignment == 0)
    {
        RWMESSAGE(("unspecified alignment, defaulting to %d, might be wasting space", rwMALLOCALIGNMENT));
        alignment = rwMALLOCALIGNMENT;
    }

    /* determine whether a free list structure was passed in */
    if (NULL == freeList)
    {
        /* allocate memory for the free list structure */
        /* see if a master free list exists */
        if (NULL != _masterFreeListPtr)
        {
            freeList = RwFreeListAlloc(_masterFreeListPtr,
                RwMemoryHintGetDuration(hint));
        }
        else
        {
            /* allocate using a standard malloc */
            freeList = RwMalloc(sizeof(RwFreeList),
                RwMemoryHintGetDuration(hint));
        }
        if (NULL == freeList)
        {
            RWRETURN(NULL);
        }
        freeList->flags = rwFREELISTFLAG_FREEBLOCKS;
    }
    else
    {
        freeList->flags = rwFREELISTFLAG_STATIC |
                          rwFREELISTFLAG_FREEBLOCKS;
    }

    /* calculate the size of an aligned block */
    alignedBlockSize = rwAlign(entrySize, alignment);

    /* calculate the size of the heap */
    heapSize = rwAlign(entriesPerBlock, 0x8);
    heapSize >>= 3;

    /* setup free list */
#ifdef RWDEBUG
    freeList->nonAlignedEntrySize = entrySize;
#endif /* RWDEBUG */
    freeList->entrySize           = alignedBlockSize;
    freeList->entriesPerBlock     = entriesPerBlock;
    freeList->alignment           = alignment;
    freeList->heapSize            = heapSize;

    /* initialize the block list */
    rwLinkListInitialize(&freeList->blockList);

    while (blocks > 0)
    {
        void *block;

        /* allocate memory for data block and portion of heap */
        block = RwMalloc(sizeof(RwLLLink) + heapSize +
                          (entriesPerBlock * alignedBlockSize) +
                          alignment - 1, hint);
        if (NULL == block)
        {
            _RwFreeListFree(freeList);
            RWRETURN(NULL);
        }

        /* add block to block list */
        {
            RwLLLink *link;

            link = (RwLLLink*)block;
            /* add block to block list */
            rwLLLinkInitialize(link);
            rwLinkListAddLLLink(&freeList->blockList, link);
        }

        /* setup heap */
        memset((RwUInt8*)block + sizeof(RwLLLink), 0, heapSize);
        blocks --;
    }

    /* add to the list of free lists */
    rwLinkListAddLLLink(&_freeListList, &freeList->link);

    RWRETURN(freeList);
}

#if (defined(RWDEBUG) && !defined(DOXYGEN))

RwFreeList *
_rwFreeListCreate(RwInt32 entrySize,
                  RwInt32 entriesPerBlock,
                  RwInt32 alignment,
                  RwUInt32 hint,
                  const RwChar * fileCreate,
                  RwUInt32 lineCreate)
{
    RwFreeList         *freelist;

    RWFUNCTION(RWSTRING("_rwFreeListCreate"));
    RWASSERT(entrySize);
    RWASSERT(entriesPerBlock);
    RWASSERT(!(alignment & (alignment - 1))); /* Assert power of 2 */

    freelist = FreeListCreate(entrySize, entriesPerBlock, alignment, 1, NULL, hint);

    /* If successful, record creation point for posterity/debugging
     */
    if (freelist)
    {
        freelist->fileCreate = fileCreate;
        freelist->lineCreate = lineCreate;
    }
    RWRETURN(freelist);
}

#else /* (defined(RWDEBUG) && !defined(DOXYGEN)) */
/**
 * \ingroup rwfreelist
 * \ref RwFreeListCreate is deprecated. \ref RwFreeListCreateAndPreallocateSpace
 * should be used instead to give better control over freelist allocations.
 *
 * \param entrySize  An \ref RwInt32 value equal to
 *                   the size of each entry (in bytes)
 * \param entriesPerBlock  An \ref RwInt32 value equal to
 *                      the number of entries per block
 * \param alignment  An \ref RwInt32 value equal to
 *                   the desired byte alignment of entries.
 *                   The alignment must be a power of two.
 *                   Zero indicates don't care.
 * \param hint       An \ref RwInt32 hint value. Refer to
 *                   \ref RwMemoryFunctions for more details about
 *                   the hints.
 *
 * \return Returns a pointer to the new free list if successful
 *         or NULL if there is an error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListForAllUsed
 * \see RwFreeListDestroy
 * \see RwFreeListPurgeAllFreeLists
 */
RwFreeList *
RwFreeListCreate(RwInt32 entrySize, RwInt32 entriesPerBlock,
                 RwInt32 alignment, RwUInt32 hint)
{
    RWAPIFUNCTION(RWSTRING("RwFreeListCreate"));
    RWASSERT(entrySize);
    RWASSERT(entriesPerBlock);
    RWASSERT(!(alignment & (alignment - 1))); /* Assert power of 2 */
    RWRETURN(FreeListCreate(entrySize, entriesPerBlock, alignment, 1, NULL, hint));
}
#endif /* (defined(RWDEBUG) && !defined(DOXYGEN)) */




/**
 * \ingroup rwfreelist
 * \ref RwFreeListCreateAndPreallocateSpace is used to create a new free list that will
 * contain the specified number of entries, each of the specified size (in
 * bytes), in a single block of memory.  More blocks can be be allocated or freed as
 * necessary in response to \ref RwFreeListAlloc or \ref RwFreeListFree.
 *
 * \param entrySize  An \ref RwInt32 value equal to
 *                   the size of each entry (in bytes)
 * \param entriesPerBlock  An \ref RwInt32 value equal to
 *                      the number of entries per block
 * \param alignment  An \ref RwInt32 value equal to
 *                   the desired byte alignment of entries.
 *                   The alignment must be a power of two.
 * \param numBlocksToPreallocate  An \ref RwInt32 value
 *                   specifying how many blocks to allocate now.
 *                   0 will wait until the first \ref RwFreeListAlloc called
 *                   on this list.  Note that this isn't always a
 *                   good policy and can cause fragmentation - allocating
 *                   at least one block is often better.
 * \param inPlaceSpaceForFreeListStruct A pointer to enough memory
 *                   to store an RwFreeList structure.  If NULL is passed
 *                   memory will instead be allocated for the structure with RwMalloc.
 * \param hint       An \ref RwInt32 hint value. Refer to \ref RwMemoryFunctions
 *                   for more details about the hints.
 *
 * \return Returns a pointer to the new free list if successful
 *         or NULL if there is an error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListForAllUsed
 * \see RwFreeListDestroy
 * \see RwFreeListPurgeAllFreeLists
 */
RwFreeList*
RwFreeListCreateAndPreallocateSpace(RwInt32 entrySize,
                                    RwInt32 entriesPerBlock,
                                    RwInt32 alignment,
                                    RwInt32 numBlocksToPreallocate,
                                    RwFreeList *inPlaceSpaceForFreeListStruct,
                                    RwUInt32 hint)
{
    RWAPIFUNCTION(RWSTRING("RwFreeListCreateAndPreallocateSpace"));
    RWASSERT(entrySize);
    RWASSERT(entriesPerBlock);
    RWASSERT(!(alignment & (alignment - 1))); /* Assert power of 2 */
    RWRETURN(FreeListCreate(entrySize, entriesPerBlock, alignment,
        numBlocksToPreallocate, inPlaceSpaceForFreeListStruct, hint));
}





void
_RwFreeListFree(RwFreeList *freeList)
{
    RwLLLink *link, *lastLink;
    RWFUNCTION("_RwFreeListFree");
    RWASSERT(freeList);

    /* free blocks */
    link     = rwLinkListGetFirstLLLink(&freeList->blockList);
    lastLink = rwLinkListGetTerminator(&freeList->blockList);
    while (link != lastLink)
    {
        rwLinkListRemoveLLLink(link);
        RwFree(link);
        link     = rwLinkListGetFirstLLLink(&freeList->blockList);
        lastLink = rwLinkListGetTerminator(&freeList->blockList);
    }

    /* free the freelist */
    if (!(freeList->flags & rwFREELISTFLAG_STATIC))
    {
        if (_masterFreeListPtr == freeList || _masterFreeListPtr == NULL)
        {
            RwFree(freeList);
        }
        else
        {
            RwFreeListFree(_masterFreeListPtr, freeList);
        }
    }
    RWRETURNVOID();
}


/**
 * \ingroup rwfreelist
 * \ref RwFreeListDestroy is used to release the block (or blocks) of
 * memory used by the specified free list.
 *
 * \param freeList  A pointer to the free list
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwFreeListAlloc
 * \see RwFreeListCreate
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListForAllUsed
 * \see RwFreeListPurgeAllFreeLists
 *
 */
RwBool
RwFreeListDestroy(RwFreeList * freeList)
{
    RWAPIFUNCTION(RWSTRING("RwFreeListDestroy"));
    RWASSERT(_freeListModuleOpen);

    /* remove from list of freelists */
    rwLinkListRemoveLLLink(&freeList->link);

    /* free it */
    _RwFreeListFree(freeList);

    RWRETURN(TRUE);
}


/**
 * \ingroup rwfreelist
 * \ref RwFreeListSetFlags Set the allocation mode of a freelist.
 * Currently the only flag available is \ref rwFREELISTFLAG_FREEBLOCKS. This
 * flag is set by default and causes free blocks to be deallocated from the
 * heap as soon as a freelist entry is freed.  If this flag is cleared all
 * free freelist blocks will remain.
 *
 * \param *freeList Pointer to the free list to change the flags on.
 * \param flags The new flags for the free list.
 *
 * \return Nothing
 *
 * \see RwFreeListGetFlags
 * \see RwFreeListAlloc
 * \see RwFreeListFree
 */
void
RwFreeListSetFlags(RwFreeList *freeList, RwUInt32 flags)
{
    RWAPIFUNCTION(RWSTRING("RwFreeListSetFlags"));
    RWASSERT(_freeListModuleOpen);
    RWASSERT(freeList);
    /* save allocation bit if set */
    freeList->flags |= flags & ~rwFREELISTFLAG_STATIC;
    RWRETURNVOID();
}


/**
 * \ingroup rwfreelist
 * \ref RwFreeListGetFlags Get the allocation mode of a freelist. For more
 * information see \ref RwFreeListSetFlags
 *
 * \param *freeList Pointer to the free list to retrieve flags from.
 *
 * \return Currently set flags for the specified freelist
 *
 * \see RwFreeListSetFlags
 */
RwUInt32
RwFreeListGetFlags(RwFreeList *freeList)
{
    RWAPIFUNCTION(RWSTRING("RwFreeListGetFlags"));
    RWASSERT(_freeListModuleOpen);
    RWASSERT(freeList);
    RWRETURN(freeList->flags);
}


#if (defined(DOXYGEN))
/**
 * \ingroup rwfreelist
 * \ref RwFreeListAlloc is used to retrieve an unused entry from the
 * specified free list. If, prior to this function call, the free list is
 * empty, the whole free list block is allocated memory. If there are no
 * more unused entries left, new entries are retrieved from another
 * allocated block.
 * Note that this function can be coerced to call \ref RwMalloc through the
 * second argument to \ref RwEngineInit.
 * This is actually implemented
 * as a macro that expands to either _rwFreeListAllocReal or \ref RwMalloc.
 *
 * \param freelist  Pointer to the free list.
 * \param hint      An \ref RwInt32 hint value Refer to \ref RwMemoryFunctions
 *                  for more details about the hints.
 *
 * \return Returns pointer to an unused entry if successful or NULL
 * is there is an error.
 *
 * \see RwFreeListCreate
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListForAllUsed
 * \see RwFreeListDestroy
 * \see RwFreeListPurgeAllFreeLists
 *
 */
void *
RwFreeListAlloc(RwFreeList * freelist, RwUInt32 hint)
{
    RWAPIFUNCTION(RWSTRING("RwFreeListAlloc"));

    RWRETURN(NULL);
}
#endif

void *
_rwFreeListAllocReal(RwFreeList * freeList, RwUInt32 hint)
{
    RwUInt8  heapElement, mask;         /* current heap element and a mask to
                                           find free element */
    void     *freeEntry = NULL;         /* set to a free heap entry if found */
    RwUInt32 i, j;                      /* loop variables */
    RwUInt32 heapEntries;               /* number of entries in a heap */
    RwUInt8  *heap;                     /* pointer to the current heap */
    RwLLLink *link, *lastLink;          /* used to traverse the block list */
    RwUInt32 checkEntries;              /* number of entries not checked */
    RWFUNCTION("_rwFreeListAllocReal");
    RWASSERT(_freeListModuleOpen);

    /* determine the total number of heap entries in the free list */
    heapEntries  = freeList->heapSize;

    /* find a free entry on heap */
    link     = rwLinkListGetFirstLLLink(&freeList->blockList);
    lastLink = rwLinkListGetTerminator (&freeList->blockList);

    while (link != lastLink && NULL == freeEntry)
    {
        heap = (RwUInt8*)link + sizeof(RwLLLink);

        /* check the number of entries per block */
        checkEntries = freeList->entriesPerBlock;

        for (i = 0; i < heapEntries; i ++)
        {
            heapElement = heap[i];

            /* check for a free entry */
            if (heapElement != 0xFF)
            {
                j = 0;
                while(j < 8 && checkEntries > 0)
                {
                    mask = (RwUInt8)(0x80 >> j);
                    if ((heapElement & mask) == 0)
                    {
                        RwUInt8 *aligned;

                        heap[i] = (RwUInt8)(heap[i] | mask);

                        /* align data block */
                        aligned  = (RwUInt8*)link + sizeof(RwLLLink) +
                                   heapEntries + freeList->alignment - 1;
                        aligned = (RwUInt8*)((RwUInt32)aligned &
                                    (RwUInt32)~(freeList->alignment - 1));

                        /* get the entry */
                        freeEntry = (RwUInt8*)aligned +
                            (((i << 3) + j) * freeList->entrySize);

                        break;
                    }
                    j ++;
                    checkEntries --;
                }
            }
            else
            {
                checkEntries -= 8;
            }
            if (NULL != freeEntry)
                break;
        }
        /* move onto the next block */
        link = rwLLLinkGetNext(link);
    }

    /* make sure an entry was found */
    if (NULL == freeEntry)
    {
        /* add new the data block */
        void     *dataBlock;
        RwLLLink *link;

        dataBlock = RwMalloc(sizeof(RwLLLink) + heapEntries +
                      (freeList->entriesPerBlock * freeList->entrySize) +
                      freeList->alignment - 1, hint);
        if (NULL == dataBlock)
        {
            RWRETURN(NULL);
        }

        /* prepare new heap */
        memset((RwUInt8*)dataBlock + sizeof(RwLLLink), 0, heapEntries);

        /* add block to block list */
        link = (RwLLLink*)dataBlock;
        rwLinkListAddLLLink(&freeList->blockList, link);

        /* allocate entry */
        *((RwUInt8*)dataBlock + sizeof(RwLLLink)) = 0x80;

        {
            RwUInt8 *aligned;

            /* align data block */
            aligned  = (RwUInt8*)dataBlock + sizeof(RwLLLink) +
                       heapEntries + freeList->alignment - 1;
            aligned = (RwUInt8*)((RwUInt32)aligned &
                          (RwUInt32)~(freeList->alignment - 1));

            freeEntry = aligned;
        }
    }

#ifdef RWDEBUG
    if (freeEntry)
    {
        memset(freeEntry, ALLOCMASK, freeList->entrySize);
    }
#endif /* RWDEBUG */

    RWRETURN(freeEntry);
}











#if (defined(DOXYGEN))
/**
 * \ingroup rwfreelist
 * \ref RwFreeListFree is used to mark the specified entry contained
 * in the given free list as unused.
 * Note that this function can be coerced to call RwFree through the
 * second argument to \ref RwEngineInit.
 * This function is actually implemented
 * as a macro that expands to either _rwFreeListFreeReal or RwFree.
 *
 * \param freelist  A pointer a to freelist
 * \param pBlock  A pointer to a used entry.
 *
 * \return Returns a pointer to the free list if successful or NULL
 * if there is an error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListCreate
 * \see RwFreeListPurge
 * \see RwFreeListForAllUsed
 * \see RwFreeListDestroy
 * \see RwFreeListPurgeAllFreeLists
 *
 */
RwFreeList *
RwFreeListFree(RwFreeList * freelist, void *pBlock)
{
    RWAPIFUNCTION(RWSTRING("RwFreeListFree"));

    RWRETURN(NULL);
}
#endif

static RwBool
FreeListBlockIsEmpty(RwUInt8 *heap, RwUInt32 heapEntries)
{
    RwUInt32 i;
    RwUInt32 sum = 0;

    RWFUNCTION(RWSTRING("FreeListBlockIsEmpty"));

    for (i = 0; i < heapEntries; i ++)
    {
        sum += heap[i];
    }

    RWRETURN( sum == 0 );
}

RwFreeList *
_rwFreeListFreeReal(RwFreeList * freeList, void *entry)
{
    RwUInt32 heapEntries;
    void     *dataBlock;
    RwLLLink *link, *lastLink;
    RWFUNCTION("_rwFreeListFreeReal");
    RWASSERT(_freeListModuleOpen);
    RWASSERT(freeList);

    heapEntries = freeList->heapSize;

    /* find the block */
    link     = rwLinkListGetFirstLLLink(&freeList->blockList);
    lastLink = rwLinkListGetTerminator(&freeList->blockList);

    while (link != lastLink)
    {
        dataBlock = (void*)((RwUInt8*)link + sizeof(RwLLLink) + heapEntries);

        if (entry >= dataBlock &&
            entry <= (void*)((RwUInt8*)dataBlock +
                (freeList->entriesPerBlock * freeList->entrySize)))
        {
            RwUInt8  *heap;
            RwUInt32 entryIndex;
            RwUInt32 heapElement;
            RwUInt8  mask;

            /* found the block now determine the entry index */
            entryIndex = ((RwUInt8*)entry - (RwUInt8*)dataBlock) /
                            freeList->entrySize;

            /* get the heap element and the bit mask */
            heapElement = entryIndex >> 3;
            mask        = (RwUInt8)(0x80 >> (entryIndex - (heapElement << 3)));

            /* get a pointer to the heap */
            heap = (RwUInt8*)link + sizeof(RwLLLink);

#ifdef RWDEBUG
            if ((heap[heapElement] & mask) == 0)
            {
                RWMESSAGE((RWSTRING("entry 0x%X was not previously allocated"),
                    (RwUInt32)entry));
            }
            RWASSERT(heap[heapElement] & mask);
#endif /* RWDEBUG */

            /* set the heap element as freed */
            heap[heapElement] &= (RwUInt8)(~mask);

#ifdef RWDEBUG
            /* stamp some entry for debug */
            memset(entry, FREEMASK, freeList->nonAlignedEntrySize);
#endif /* RWDEBUG */


            /* if we're allowed to free empty blocks */
            if (freeList->flags & rwFREELISTFLAG_FREEBLOCKS)
            {
                /* if this block is now empty */
                if (FreeListBlockIsEmpty(heap,heapEntries))
                {
                    rwLinkListRemoveLLLink(link);
                    RwFree(link);
                }
            }

            RWRETURN(freeList);
        }
        link = rwLLLinkGetNext(link);
    }

    /* the entry wasn't found */
    RWMESSAGE((RWSTRING("Entry 0x%08X was not allocated from this FreeList"), (RwUInt32)entry));
    RWASSERT(FALSE);

    RWRETURN(NULL);
}







/**
 * \ingroup rwfreelist
 * \ref RwFreeListPurge is used to the release the memory for any blocks
 * in the specified free list that wholly contain unused entries.  It returns
 * the amount of memory (in bytes) recovered.
 *
 * \param freeList  A pointer to free list.
 *
 * \return Returns the amount of memory freed, or -1 if there is an error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListCreate
 * \see RwFreeListFree
 * \see RwFreeListForAllUsed
 * \see RwFreeListDestroy
 * \see RwFreeListPurgeAllFreeLists
 *
 */
RwInt32
RwFreeListPurge(RwFreeList * freeList)
{
    RwUInt32 heapEntries;
    RwLLLink *link, *nextLink, *lastLink;
    RwUInt32 blocksFreed = 0;
    RWAPIFUNCTION("RwFreeListPurge");
    RWASSERT(_freeListModuleOpen);
    RWASSERT(freeList);

    heapEntries = freeList->heapSize;

    /* find the block */
    link     = rwLinkListGetFirstLLLink(&freeList->blockList);
    lastLink = rwLinkListGetTerminator(&freeList->blockList);

    while (link != lastLink)
    {
        RwUInt8 *heap;

        /* get a pointer to the heap */
        heap = (RwUInt8*)link + sizeof(RwLLLink);

        /* if this is the last block or the don't free flag is set then
           don't free it */
        rwLinkListRemoveLLLink(link);

        nextLink = rwLLLinkGetNext(link);

        if (FreeListBlockIsEmpty(heap, heapEntries))
        {
            /* if this heap is totally empty then free the block */
            RwFree(link);
            blocksFreed ++;
        }
        else
        {
            rwLinkListAddLLLink(&freeList->blockList, link);
        }

        link = nextLink;
    }

    RWRETURN(blocksFreed * freeList->entrySize);
}











/**
 * \ingroup rwfreelist
 * \ref RwFreeListForAllUsed is used to apply the given callback
 * function to all used entries in the specified free list. The format of
 * the callback function is:
 * \verbatim
        void (*RwFreeListCallBack)(void *mem, void *data);
   \endverbatim
 * where "data" is a pointer to user-defined data pointer to pass to the
 * callback function.
 *
 * \param freeList  A pointer to the free list
 * \param fpCallBack  A pointer to the callback to apply to call with each used block
 * \param pData  A pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the free list if successful or NULL
 * if there is an error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListCreate
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListDestroy
 * \see RwFreeListPurgeAllFreeLists
 *
 */
RwFreeList *
RwFreeListForAllUsed(RwFreeList * freeList,
                     RwFreeListCallBack fpCallBack, void *pData)
{
    RwUInt32 heapEntries;
    RwLLLink *link, *lastLink, *nextLink;
    RWAPIFUNCTION(RWSTRING("RwFreeListForAllUsed"));
    RWASSERT(_freeListModuleOpen);
    RWASSERT(freeList);
    RWASSERT(fpCallBack);

    heapEntries = freeList->heapSize;

    /* for each block */
    link     = rwLinkListGetFirstLLLink(&freeList->blockList);
    lastLink = rwLinkListGetTerminator(&freeList->blockList);

    while (link != lastLink)
    {
        RwUInt8 *heap, *heapCopy;
        RwUInt32 heapElement;
        RwUInt8  mask;
        RwUInt32 i;

        /* get a pointer to the heap */
        heap = (RwUInt8*)link + sizeof(RwLLLink);

        /*
        Copy heap elements used flags as an RwFreeListFree
        in the callback can mash them if it (rightly) frees the whole block,
        yet we may go on looping over them when they're dead.
        (I really don't like the idea of the callback calling RwFreeListFree,
        but the old implementation used to support it.)
        */
        heapCopy = (RwUInt8 *)RwMalloc(heapEntries, rwMEMHINTDUR_FUNCTION);
        if (!heapCopy)
        {
            RWRETURN(NULL);
        }
        memcpy(heapCopy, heap, heapEntries);

        /*
        Store pointer to next link now, as some rw code
        apparently expects to be able to remove links from the list
        in the callback function via RwFreeListFree (!)
        */
        nextLink = rwLLLinkGetNext(link);

        /* check each heap element for used entries */
        for (i = 0; i < heapEntries; i ++)
        {
            heapElement = heapCopy[i];
            if (heapElement != 0)
            {
                /* there's a used entry in here somewhere */
                RwUInt32 j;
                for (j=0; j<8; j++)
                {
                    mask = (RwUInt8)(0x80 >> j);
                    if ((heapElement & mask) != 0)
                    {
                        /* this one's used, what's the address of the element? */
                        RwUInt8 *aligned, *entry;

                        /* align data block */
                        aligned  = (RwUInt8*)link + sizeof(RwLLLink) +
                                   heapEntries + freeList->alignment - 1;
                        aligned = (RwUInt8*)((RwUInt32)aligned &
                                    (RwUInt32)~(freeList->alignment - 1));

                        /* get the entry */
                        entry = (RwUInt8*)aligned +
                            (((i << 3) + j) * freeList->entrySize);

                        /* varsagod */
                        fpCallBack((void *)entry, pData);
                    }
                }
            }
        }

        RwFree(heapCopy);

        link = nextLink;
    }

    RWRETURN(freeList);
}











/**
 * \ingroup rwfreelist
 * \ref RwFreeListPurgeAllFreeLists is used to purge all free lists in
 * the system.  It returns the amount of memory freed as a result.
 *
 * \return Returns the amount of memory freed if successful, or -1 if there is an
 * error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListCreate
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListDestroy
 * \see RwFreeListForAllUsed
 *
 */
RwInt32
RwFreeListPurgeAllFreeLists(void)
{
    RwInt32  memoryFreed = 0;
    RwLLLink *cur, *end;
    RWAPIFUNCTION(RWSTRING("RwFreeListPurgeAllFreeLists"));

    cur = rwLinkListGetFirstLLLink(&_freeListList);
    end = rwLinkListGetTerminator(&_freeListList);
    while (cur != end)
    {
        /* Purge this list */
        RwFreeList * const  freeList = ( rwLLLinkGetData(cur,
                                                         RwFreeList,
                                                         link) );
        const RwInt32 thisMemoryFreed = RwFreeListPurge(freeList);

        if (0 < thisMemoryFreed)
        {
            memoryFreed += thisMemoryFreed;
        }

        /* Get next */
        cur = rwLLLinkGetNext(cur);
    }

    RWRETURN(memoryFreed);
}

/* This is just a call to malloc which accepts memory hint too */
static void *
HMalloc(size_t size, RwUInt32 hint __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("HMalloc"));
    RWRETURN(malloc(size));
}

/* This is just a call to realloc which accepts memory hint too */
static void *
HRealloc(void *mem, size_t newSize, RwUInt32 hint __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("HRealloc"));
    RWRETURN(realloc(mem, newSize));
}

/* This is just a call to calloc which accepts memory hint too */
static void *
HCalloc(size_t numObj, size_t sizeObj, RwUInt32 hint __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("HCalloc"));
    RWRETURN(calloc(numObj, sizeObj));
}

RwBool
_rwMemoryOpen(const RwMemoryFunctions * memFuncs)
{
    RWFUNCTION(RWSTRING("_rwMemoryOpen"));

    if (!_rwFreeListModuleOpen())
    {
        RWRETURN(FALSE);
    }

#if (defined(RWDEBUG))
    RWSRCGLOBAL(freeListExtraDebug) = FALSE;
#endif /* defined(RWDEBUG) */

    if (memFuncs)
    {
        /* Install user specified memory system */
        RWSRCGLOBAL(memoryFuncs) = *memFuncs;
    }
    else
    {
        /* Install an ANSI memory system */
        RWSRCGLOBAL(memoryFuncs).rwmalloc = HMalloc;
        RWSRCGLOBAL(memoryFuncs).rwfree = free;
        RWSRCGLOBAL(memoryFuncs).rwrealloc = HRealloc;
        RWSRCGLOBAL(memoryFuncs).rwcalloc = HCalloc;
    }

    RWRETURN(TRUE);
}





void
_rwMemoryClose(void)
{
    RWFUNCTION(RWSTRING("_rwMemoryClose"));

    _rwFreeListModuleClose();

    RWRETURNVOID();
}



/**
 * \ingroup memoryfileinterface
 * \ref RwOsGetMemoryInterface is used to retrieve the memory functions
 * used by RenderWare. The application can install any alternative interface
 * that is compliant with RenderWare functions.
 *
 * If an application wishes to completely overload the memory interface then
 * this can be achieved via the \ref RwEngineInit API function which guarantees
 * that the overload takes place before any memory allocation. The memory
 * function pointers should not be manipulated directly.
 *
 * Note that since RenderWare allocates and frees memory in an asynchronous
 * fashion it is not possible to install a completely different memory handler
 * once the library has been initialized. It is, however, possible to install
 * a daisy chained handler that gathers statistics before calling the default.
 *
 * \return Returns pointer to a \ref RwMemoryFunctions value containing pointers
 * to memory access functions. The elements of this structure may be modified
 * directly to install an alternative memory interface.
 *
 * \see RwEngineInit, RwMemoryFunctions.
 */
RwMemoryFunctions *
RwOsGetMemoryInterface(void)
{
    RWAPIFUNCTION(RWSTRING("RwOsGetMemoryInterface"));

    RWRETURN(&RWSRCGLOBAL(memoryFuncs));
}

