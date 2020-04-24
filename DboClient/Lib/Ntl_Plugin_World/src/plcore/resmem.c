/************************************************************************
 *                                                                      *
 * Module  : resmem.c                                                   *
 *                                                                      *
 * Purpose : Resource memory heap allocator.                            *
 *                                                                      *
 *           Originally from Kamui2 driver.                             *
 *                                                                      *
 *           This is allocating on 32 byte boundaries. We should        *
 *           probably make this definable so that we can alloc on cache *
 *           boundaries.                                                *
 *                                                                      *
 ************************************************************************/

#define MEMORYLOGx
#define EXTRADEBUGx

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"

#include "resmem.h"

/****************************************************************************
 Global prototypes
 */

/****************************************************************************
 Local Types
 */

/* Block flags */
enum rwResHeapBlockFlags
{
    rwRESHEAP_BLOCKUSED = 1,    /* Block is in use */

    rwRESHEAPFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum rwResHeapBlockFlags rwResHeapBlockFlags;

typedef struct rwResHeapBlockHeader rwResHeapBlockHeader;
typedef struct rwResHeapHeader rwResHeapHeader;

/* Heap header */
struct rwResHeapHeader
{
    rwResHeapBlockHeader *firstBlock; /* First block in heap list */
    rwResHeapBlockHeader *firstFreeBlock; /* First free block */
};

/* Block header */
struct rwResHeapBlockHeader
{
    rwResHeapHeader    *heap;   /* Pointer to heap header */
    rwResHeapBlockHeader *next; /* Pointer to next block in heap list */
    rwResHeapBlockHeader *prev; /* Pointer to prev block in heap list */
    RwUInt32            size;   /* Size of this heap block */
    rwResHeapBlockFlags flags;  /* Heap block flags */
    RwUInt32            pad[3]; /* Align this structure to 32 bytes */
};

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/****************************************************************************
 Functions
 */

#ifdef MEMORYLOG
/******************************************************************************
 *
 *  This is currently only compatible with a single heap.
 */

#define MEMLOGBUFFERSIZE 128
#define MEMLOGTEXTENTRYSIZEMAX 30

static RwInt32      nAccessCount = 0;
static void        *nGLogFileID = NULL;
static RwUInt32     nGAddressBuffer[MEMLOGBUFFERSIZE];
static RwUInt8      nGAccessBuffer[MEMLOGBUFFERSIZE];
static RwUInt32     nGCountBuffer[MEMLOGBUFFERSIZE];
static RwUInt32     nGSizeBuffer[MEMLOGBUFFERSIZE];
static RwUInt32     nGBufferIndex = 0;
static char         sGOutputString[MEMLOGBUFFERSIZE *
                                   MEMLOGTEXTENTRYSIZEMAX];

static void
MemoryLogWrite(void)
{
    RwInt32             nIndex;

    RWFUNCTION(RWSTRING("MemoryLogWrite"));
    /* Build up the output string */
    sGOutputString[0] = '\0';
    for (nIndex = 0; nIndex < nGBufferIndex; nIndex++)
    {
        char                sTextBuffer[MEMLOGTEXTENTRYSIZEMAX];

        sprintf(sTextBuffer, "%x %c %d %d\n", nGAddressBuffer[nIndex],
                nGAccessBuffer[nIndex], nGCountBuffer[nIndex],
                nGSizeBuffer[nIndex]);
        strncat(sGOutputString, sTextBuffer, sizeof(sGOutputString));
    }
    RwFwrite(sGOutputString, strlen(sGOutputString), 1, nGLogFileID);
    nGBufferIndex = 0;

    RWRETURNVOID();
}

static void
MemoryLogInit()
{
    RWFUNCTION(RWSTRING("MemoryLogInit"));

    /* Open the file that we will be writing mem thingy's too */
    nGLogFileID = RwFopen("memlog.txt", "w");

    if (nGLogFileID != NULL)
    {
        RWMESSAGE((RWSTRING("rwResHeap:MemoryLogInit:"
                            "Starting to Log Mem accesses to memlog.txt")));
    }

    RWRETURNVOID();
}

static void
MemoryLogFree(void *memory)
{
    RWFUNCTION(RWSTRING("MemoryLogFree"));

    nAccessCount++;

    if (nGLogFileID != NULL)
    {
        /* Buffer up the stats since debug_write is Sloooooooooooowwwwwwwwww */
        nGAddressBuffer[nGBufferIndex] = (RwUInt32) (memory);
        nGAccessBuffer[nGBufferIndex] = 'F';
        nGCountBuffer[nGBufferIndex] = nAccessCount;
        nGSizeBuffer[nGBufferIndex] = 0;
        nGBufferIndex++;

        if (nGBufferIndex == MEMLOGBUFFERSIZE)
        {
            MemoryLogWrite();
        }
    }

    RWRETURNVOID();
}

static void
MemoryLogAlloc(rwResHeapBlockHeader * targetBlock, RwUInt32 size)
{
    RWFUNCTION(RWSTRING("MemoryLogAlloc"));

    nAccessCount++;

    if (nGLogFileID != NULL)
    {
        /* Buffer up the stats since debug_write is Sloooooooooooowwwwwwwwww */
        nGAddressBuffer[nGBufferIndex] = (RwUInt32) (targetBlock + 1);
        nGAccessBuffer[nGBufferIndex] = 'A';
        nGCountBuffer[nGBufferIndex] = nAccessCount;
        nGSizeBuffer[nGBufferIndex] = size;
        nGBufferIndex++;

        if (nGBufferIndex == MEMLOGBUFFERSIZE)
        {
            MemoryLogWrite();
        }

        /* Write zero to the first byte of the new memory so that we can catch
         * this with a memory watch */
        *((RwUInt32 *) (targetBlock + 1)) = 0;
    }

    RWRETURNVOID();
}

static void
MemoryLogClose()
{
    RWFUNCTION(RWSTRING("MemoryLogClose"));

    /* Empty our log and close up */
    if (nGLogFileID != NULL)
    {
        MemoryLogWrite();
        RWMESSAGE(("MemoryLogClose: Completed Mem Log to memlog.txt"));
        RwFclose(nGLogFileID);
    }

    RWRETURNVOID();
}

/*****************************************************************************/
#endif /* MEMORYLOG */

#ifdef EXTRADEBUG
/******************************************************************************
 */

static RwUInt8     *heapEnd = NULL;

static void
DebugHeapCheck(void)
{
    rwResHeapBlockHeader *curBlock;

    RWFUNCTION(RWSTRING("DebugHeapCheck"));

    /* Check to see that we don't have any major memory losses in our heap */
    curBlock = firstBlock;
    while (curBlock)
    {
        if (curBlock->next)
        {
            if (((RwUInt8 *) curBlock + curBlock->size
                 + sizeof(rwResHeapBlockHeader))
                != (RwUInt8 *) curBlock->next)
            {
                /* Oooops, memory loss here */
                while (1) ;
            }
        }
        else
        {
            if (((RwUInt8 *) curBlock + curBlock->size
                 + sizeof(rwResHeapBlockHeader)) != heapEnd)
            {
                /* Oooops, memory loss here */
                while (1) ;
            }
        }

        curBlock = curBlock->next;
    }

    RWRETURNVOID();
}

static void
DebugAllocFailure(rwResHeapHeader * heapInfo)
{
    RwUInt32            totalFree, totalAllocated, numBlocksAllocated;
    RwUInt32            largestFree, smallestFree, numBlocksFree;

    RWFUNCTION(RWSTRING("DebugAllocFailure"));

    curBlock = heapInfo->firstBlock;
    totalFree = 0;
    totalAllocated = 0;
    largestFree = 0;
    smallestFree = 1024000;
    numBlocksAllocated = 0;
    numBlocksFree = 0;
    while (curBlock)
    {
        if (curBlock->flags & rwRESHEAP_BLOCKUSED)
        {
            totalAllocated += curBlock->size;
            numBlocksAllocated++;
        }
        else
        {
            totalFree += curBlock->size;
            numBlocksFree++;
            if (curBlock->size > largestFree)
            {
                largestFree = curBlock->size;
            }
            if (curBlock->size < smallestFree)
            {
                smallestFree = curBlock->size;
            }
        }
        curBlock = curBlock->next;
    }

    while (1) ;

    RWRETURNVOID();
}

/*****************************************************************************/
#endif /* EXTRADEBUG */

/******************************************************************************
 *
 *  splitBlock()
 *
 *  Split a resHeap block, with the first block taking size bytes. The
 *  second part is marked as unused and is merged with any following
 *  unused block.
 */
static void
splitBlock(rwResHeapBlockHeader * block, RwUInt32 size)
{
    rwResHeapBlockHeader *newBlock;

    RWFUNCTION(RWSTRING("splitBlock"));
    RWASSERT(block);
    RWASSERT(size);
    RWASSERT((size + sizeof(rwResHeapBlockHeader)) < block->size);

    newBlock = (rwResHeapBlockHeader *)
        (((RwUInt8 *) block) + size + sizeof(rwResHeapBlockHeader));

    /* Is next block free? */
    if (block->next && (~block->next->flags & rwRESHEAP_BLOCKUSED))
    {
        /* Make the rest part of the next block */
        newBlock->next = block->next->next;
        newBlock->size = (block->size - size) + block->next->size;
    }
    else
    {
        /* Create new block */
        newBlock->next = block->next;
        newBlock->size =
            block->size - size - sizeof(rwResHeapBlockHeader);
    }

    /* Miscellaneous stuff */
    block->next = newBlock;
    newBlock->flags = (rwResHeapBlockFlags) 0;

    /* Link in previous and next of new block */
    newBlock->prev = block;
    if (newBlock->next)
    {
        newBlock->next->prev = newBlock;
    }

    /* Adjust sizes */
    block->size = size;

    /* Copy link to the heap header */
    newBlock->heap = block->heap;

    RWRETURNVOID();
}

/******************************************************************************
 *
 *  _rwResHeapInit()
 *
 *  Initialise the block of memory at resHeap, of the given size, for heap
 *  management. Returns NULL if size is not sufficient.
 */
RwBool
_rwResHeapInit(void *resHeap, RwUInt32 size)
{
    rwResHeapHeader    *heapInfo = (rwResHeapHeader *) resHeap;
    rwResHeapBlockHeader *firstBlock;
    RwUInt32            start, end;
    RwInt32             blockSize;

    RWFUNCTION(RWSTRING("_rwResHeapInit"));
    RWASSERT(resHeap);
    RWASSERT(size);

#ifdef MEMORYLOG
    MemoryLogInit();
#endif /* MEMORYLOG */

    /* Get 32 byte aligned start and end of the heap block */
    start =
        ((RwUInt32) resHeap + sizeof(rwResHeapHeader) +
         31) & 0xFFFFFFE0;
    end = ((RwUInt32) resHeap + size) & 0xFFFFFFE0;

    /* Get size of available storage */
    blockSize = end - start - sizeof(rwResHeapBlockHeader);
    if (blockSize < 32)
    {
        RWRETURN(FALSE);
    }

    /* Initialize the first block header */
    firstBlock = (rwResHeapBlockHeader *) start;
    firstBlock->heap = heapInfo; /* Link back to heap header */
    firstBlock->next = (rwResHeapBlockHeader *) NULL;
    firstBlock->prev = (rwResHeapBlockHeader *) NULL;
    firstBlock->flags = (rwResHeapBlockFlags) 0;
    firstBlock->size = blockSize;

    /* Initialize heap info */
    heapInfo->firstBlock = firstBlock;
    heapInfo->firstFreeBlock = firstBlock;

#ifdef EXTRADEBUG
    heapEnd = (RwUInt8 *) end;
    DebugHeapCheck();
#endif /* EXTRADEBUG */

    RWRETURN(TRUE);
}

/******************************************************************************
 *
 *  _rwResHeapClose()
 *
 *  Terminate management of the heap at resHeap.
 */
RwBool
_rwResHeapClose(void *resHeap __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwResHeapClose"));

#ifdef MEMORYLOG
    MemoryLogClose();
#endif /* MEMORYLOG */

    RWRETURN(TRUE);
}

/******************************************************************************
 *
 *  _rwResHeapFree()
 *
 *  Free block at <memory>. This will merge the free block with its neighbours
 *  if they are unused.
 */
void
_rwResHeapFree(void *memory)
{
    rwResHeapBlockHeader *block =
        (((rwResHeapBlockHeader *) memory) - 1);
    rwResHeapBlockHeader *prevBlock;
    rwResHeapBlockHeader *nextBlock;

    RWFUNCTION(RWSTRING("_rwResHeapFree"));
    RWASSERT(memory);
    RWASSERT(block->flags & rwRESHEAP_BLOCKUSED);
    RWASSERT(block->heap);
    RWASSERT((block == block->heap->firstBlock)
             || (block == block->prev->next));

#ifdef EXTRADEBUG
    DebugHeapCheck();
#endif /* EXTRADEBUG */

#ifdef MEMORYLOG
    MemoryLogFree(memory);
#endif /* MEMORYLOG */

    /* This block has been freed */
    block->flags = (rwResHeapBlockFlags) 0;

    /* Find previous and next (for merging blocks) */
    prevBlock = block->prev;
    nextBlock = block->next;

    /* Check if this is now the first free block */
    if (!block->heap->firstFreeBlock
        || (block < block->heap->firstFreeBlock))
    {
        block->heap->firstFreeBlock = block;
    }

    /* Try and merge with previous block (if there is one) */
    if (prevBlock && (~prevBlock->flags & rwRESHEAP_BLOCKUSED))
    {
        /* Merge it */
        prevBlock->next = nextBlock;
        if (nextBlock)
        {
            nextBlock->prev = prevBlock;
        }

        /* Adjust the size */
        prevBlock->size += (block->size + sizeof(rwResHeapBlockHeader));

        /* The current block now becomes the previous block */
        block = prevBlock;
    }

#ifdef EXTRADEBUG
    DebugHeapCheck();
#endif /* EXTRADEBUG */

    /* try and merge with next block (if there is one) */
    if (nextBlock && (~nextBlock->flags & rwRESHEAP_BLOCKUSED))
    {
        /* Merge it */
        block->next = nextBlock->next;
        if (nextBlock->next)
        {
            nextBlock->next->prev = block;
        }

        /* Adjust the size */
        block->size += (nextBlock->size + sizeof(rwResHeapBlockHeader));
    }

#ifdef EXTRADEBUG
    DebugHeapCheck();
#endif /* EXTRADEBUG */

    RWRETURNVOID();
}

/******************************************************************************
 *
 *  _rwResHeapAlloc()
 *
 *  Allocate size bytes from resHeap, and return pointer or NULL on failure.
 *  Points to 32 byte aligned memory.
 *
 *  resHeap must have been initialized with _rwResHeapInit
 *
#if 0
 *  Finds the smallest compatible block (rather than the first compatible block
 *  which would be faster but less memory efficient).
#endif
 */
void               *
_rwResHeapAlloc(void *resHeap, RwUInt32 size)
{
    rwResHeapHeader    *heapInfo = (rwResHeapHeader *) resHeap;
    rwResHeapBlockHeader *targetBlock, *curBlock;

    RWFUNCTION(RWSTRING("_rwResHeapAlloc"));
    RWASSERT(resHeap);
    RWASSERT(size);
    RWASSERT(heapInfo->firstBlock);
    RWASSERT(heapInfo->firstBlock->heap == heapInfo);

#ifdef EXTRADEBUG
    DebugHeapCheck();
#endif /* EXTRADEBUG */

    /* Round allocation size up to next 32 byte multiple */
    size += 31;
    size &= 0xFFFFFFE0;

    /* Walk the heap...
     * Find the first block that satisfies our needs and
     * return that (splitting off excess if necessary)
     */
    targetBlock = (rwResHeapBlockHeader *) NULL;
    curBlock = heapInfo->firstFreeBlock;
    while (curBlock && !targetBlock)
    {
        /* If it's big enough and is not used, use it */
        if ((~curBlock->flags & rwRESHEAP_BLOCKUSED)
            && curBlock->size >= size)
        {
            /* This might do */
#if 0
            if (!targetBlock || curBlock->size < targetBlock->size)
#endif /* 0 */
            {
                /* This is better than where we are now */
                targetBlock = curBlock;
            }
        }

        curBlock = curBlock->next;
    }

    /* Did memory allocation fail? */
    if (!targetBlock)
    {
#ifdef EXTRADEBUG
        DebugAllocFailure();
#endif /* EXTRADEBUG */

        RWRETURN(NULL);
    }

    /* Split off excess into new block (if it's worth it) */
    if (targetBlock->size > (size + 64))
    {
        splitBlock(targetBlock, size);
    }

    /* May need to find the new first free block */
    if (targetBlock == heapInfo->firstFreeBlock)
    {
        do
        {
            heapInfo->firstFreeBlock = heapInfo->firstFreeBlock->next;
        }
        while (heapInfo->firstFreeBlock &&
               (heapInfo->firstFreeBlock->flags & rwRESHEAP_BLOCKUSED));
    }

    /* Mark the block as allocated and return it */
    targetBlock->flags = rwRESHEAP_BLOCKUSED;

#ifdef EXTRADEBUG
    DebugHeapCheck();
#endif /* EXTRADEBUG */

#ifdef MEMORYLOG
    MemoryLogAlloc(targetBlock, size);
#endif /* MEMORYLOG */

    RWRETURN((void *) (targetBlock + 1));
}

/******************************************************************************
 *
 *  _rwResHeapCalloc()
 *
 *  Allocates num*size bytes from resHeap on 32 byte boundary, and fill
 *  memory with zeros. Return pointer to the block or NULL on failure.
 *
 *  resHeap must have been initialized with _rwResHeapInit
 *
 *  Finds the smallest compatible block (rather than the first compatible block
 *  which would be faster but less memory efficient).
 */
void               *
_rwResHeapCalloc(void *resHeap, RwUInt32 num, RwUInt32 size)
{
    void               *mem;

    RWFUNCTION(RWSTRING("_rwResHeapCalloc"));
    RWASSERT(resHeap);
    RWASSERT(num);
    RWASSERT(size);

    /* Pass to alloc */
    mem = _rwResHeapAlloc(resHeap, num * size);

    /* Zero memory */
    if (mem)
    {
        memset(mem, 0, num * size);
    }

    RWRETURN(mem);
}

/******************************************************************************
 *
 *  _rwResHeapRealloc()
 *
 *  Reallocates the block at <memory>.
 *
 *  If size is smaller, then frees the tail, merging if possible.
 *  If size is greater, then expands into next block if free, otherwise
 *  allocates a new block and copies.
 */
void               *
_rwResHeapRealloc(void *memory, RwUInt32 size)
{
    rwResHeapBlockHeader *block =
        (((rwResHeapBlockHeader *) memory) - 1);

    RWFUNCTION(RWSTRING("_rwResHeapRealloc"));
    RWASSERT(memory);
    RWASSERT(size);
    RWASSERT(block->heap);
    RWASSERT((block == block->heap->firstBlock)
             || (block == block->prev->next));

#ifdef EXTRADEBUG
    DebugHeapCheck();
#endif /* EXTRADEBUG */

    /* Round allocation size up to next 32 byte multiple */
    size += 31;
    size &= 0xFFFFFFE0;

    /* Ideally, we'd have enough space to just use this block */
    if (size <= block->size)
    {
        /* We're shrinking it, so chop off the end of the block to create
         * another - if it's worth it.
         */
        if (block->size > (size + 64))
        {
            rwResHeapHeader    *heap = block->heap;

            splitBlock(block, size);

            /* New block may have become the first free one */
            if (!heap->firstFreeBlock
                || (block->next < heap->firstFreeBlock))
            {
                heap->firstFreeBlock = block->next;
            }
        }

#ifdef EXTRADEBUG
        DebugHeapCheck();
#endif /* EXTRADEBUG */

        RWRETURN(memory);
    }
    else
    {
        RwUInt32            sizeAfter;
        RwUInt32            sizeNeeded;

        /* We're growing the block. Is the next block unused and
         * does it have enough space? We should, at most, only have
         * one free block next.
         */
        sizeNeeded = size - block->size;
        sizeAfter = 0;
        if (block->next && (~block->next->flags & rwRESHEAP_BLOCKUSED))
        {
            /* We have a block we can grow into */
            sizeAfter =
                block->next->size + sizeof(rwResHeapBlockHeader);
        }

        if (sizeAfter >= sizeNeeded)
        {
            rwResHeapHeader    *heap = block->heap;

            /* We can extend the block in situ */
            if (sizeNeeded + 64 >= sizeAfter)
            {
                /* Don't bother with the wast - just merge the blocks */

                /* May need to find the new first free block */
                if (block->next == heap->firstFreeBlock)
                {
                    do
                    {
                        heap->firstFreeBlock =
                            heap->firstFreeBlock->next;
                    }
                    while (heap->firstFreeBlock &&
                           (heap->firstFreeBlock->
                            flags & rwRESHEAP_BLOCKUSED));
                }

                /* Merge the blocks */
                block->next = block->next->next;
                if (block->next)
                {
                    block->next->prev = block;
                }
                block->size += sizeAfter;
            }
            else
            {
                /* Chop off the waste as a separate block */
                rwResHeapBlockHeader *newBlock =
                    (rwResHeapBlockHeader *) (((RwUInt8 *) memory) +
                                              size);

                newBlock->next = block->next->next;
                newBlock->prev = block;
                newBlock->size = sizeAfter - sizeNeeded
                    - sizeof(rwResHeapBlockHeader);
                newBlock->heap = block->heap;
                newBlock->flags = (rwResHeapBlockFlags) 0;

                /* Has new block become the first free block? */
                if (block->next == heap->firstFreeBlock)
                {
                    heap->firstFreeBlock = newBlock;
                }

                /* Links to new block */
                block->next = newBlock;
                if (newBlock->next)
                {
                    newBlock->next->prev = newBlock;
                }

                block->size = size;
            }

#ifdef EXTRADEBUG
            DebugHeapCheck();
#endif /* EXTRADEBUG */

            /* All sorted now */
            RWRETURN(memory);
        }
        else
        {
            void               *newMem;

            /* We must allocate a new block and copy it */
            newMem = _rwResHeapAlloc((void *) block->heap, size);

            if (newMem)
            {
                /* We are growing the block, so copy all the old size */
                RwUInt32            numWords = block->size >> 2;
                RwUInt32           *src = (RwUInt32 *) memory;
                RwUInt32           *dst = (RwUInt32 *) newMem;

                while (numWords--)
                {
                    *dst++ = *src++;
                }

                _rwResHeapFree(memory);
            }

#ifdef EXTRADEBUG
            DebugHeapCheck();
#endif /* EXTRADEBUG */

            RWRETURN(newMem);
        }
    }
}

/******************************************************************************
 *
 *  _rwResHeapGetStats()
 *
 *  Heap usage statistics.
 */
RwBool
_rwResHeapGetStats(void *resHeap, /* Pointer to base of heap */
                   RwUInt32 * totalAllocated, /* OUT: Total memory allocated */
                   RwUInt32 * totalFree, /* OUT: Total memory free */
                   RwUInt32 * largestAllocated, /* OUT: Size of largest allocated block */
                   RwUInt32 * largestFree, /* OUT: Size of largest free block */
                   RwUInt32 * smallestAllocated, /* OUT: Size of smallest allocated block */
                   RwUInt32 * smallestFree) /* OUT: Size of smallest free block */
{
    rwResHeapHeader    *heapInfo = (rwResHeapHeader *) resHeap;
    rwResHeapBlockHeader *curBlock;

    RWFUNCTION(RWSTRING("_rwResHeapGetStats"));
    RWASSERT(resHeap);
    RWASSERT(totalAllocated);
    RWASSERT(totalFree);
    RWASSERT(largestAllocated);
    RWASSERT(largestFree);
    RWASSERT(smallestAllocated);
    RWASSERT(smallestFree);

    /* Initialise with some defaults */
    *totalAllocated = 0;
    *totalFree = 0;
    *largestAllocated = 0;
    *largestFree = 0;
    *smallestAllocated = 0xFFFFFFFF;
    *smallestFree = 0xFFFFFFFF;

    /* Walk the heap, and gather stats */
    curBlock = heapInfo->firstBlock;
    while (curBlock)
    {
        if (curBlock->flags & rwRESHEAP_BLOCKUSED)
        {
            /* This one is used */
            *totalAllocated += curBlock->size;

            /* Update smallest */
            if (curBlock->size < *smallestAllocated)
            {
                *smallestAllocated = curBlock->size;
            }

            /* Update largest */
            if (curBlock->size > *largestAllocated)
            {
                *largestAllocated = curBlock->size;
            }
        }
        else
        {
            /* This one is free */
            *totalFree += curBlock->size;

            /* Update smallest */
            if (curBlock->size < *smallestFree)
            {
                *smallestFree = curBlock->size;
            }

            /* Update largest */
            if (curBlock->size > *largestFree)
            {
                *largestFree = curBlock->size;
            }
        }

        curBlock = curBlock->next;
    }

    RWRETURN(TRUE);
}
