/*
 * A heap management package optimized for custom pipelines
 *
 * Copyright (c) Criterion Software Limited
 */

/* p2heap.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"

#include "p2core.h"
#include "p2heap.h"

#define MINHEAPSIZE 1024 /* Pff, I dunno... */

#define FREEBLOCKTBLGRANULARITY 32 /* 32 * 8 = 256 */

#define BLOCKSIZE(size) ( ((size) + 31) & ~31 )

/************************************************************************/

#if (!defined(DISABLERWHEAP))

#if (defined(HEAPCHECKS))

/****************************************************************************
 HeapValidate
 */

static void
HeapValidate(RxHeap * heap)
{
    rxHeapBlockHeader  *blockHdr;
    rxHeapSuperBlockDescriptor *superBlock;
    RwUInt32            sum = 0, sumsuperblocks = 0;

    RWFUNCTION(RWSTRING("HeapValidate"));

    RWASSERT(NULL != heap);

    superBlock = heap->head;
    while (superBlock != NULL)
    {
        sumsuperblocks += superBlock->size;
        superBlock = superBlock->next;
    }

    blockHdr = heap->headBlock;
    while (blockHdr != NULL)
    {
        sum += sizeof(rxHeapBlockHeader) + blockHdr->size;
        if (blockHdr->freeEntry != NULL)
        {
            RWASSERT(blockHdr->freeEntry->ptr == blockHdr);
            RWASSERT(blockHdr->freeEntry->size == blockHdr->size);
        }
        blockHdr = blockHdr->next;
    }

    RWASSERT(sum == sumsuperblocks);

    RWRETURNVOID();
}

#define CHKHEAP(heap) HeapValidate(heap)

#endif /* (defined(HEAPCHECKS)) */

#if (!defined(CHKHEAP))
#define CHKHEAP(heap)          /* No op */
#endif /* (!defined(CHKHEAP)) */

#if (defined(LOGGING))
#define FLOG(str, num)                                                  \
MACRO_START                                                             \
{                                                                       \
    FILE *flog = fopen("heap.log", "at");                               \
    fprintf(flog, "%s(%d): %s %i\n", __FILE__, __LINE__, (str), num);   \
    fclose(flog);                                                       \
}
MACRO_STOP
#endif /* (defined(LOGGING)) */
#if (!defined(FLOG))
#define FLOG(str, num)         /* No op */
#endif /* (!defined(FLOG)) */
/****************************************************************************
 HeapFreeBlocksNewEntry
 */
static rxHeapFreeBlock *
HeapFreeBlocksNewEntry(RxHeap *heap)
{
    rxHeapFreeBlock    *freeBlocks;
    RwUInt32            entriesUsed;

    RWFUNCTION(RWSTRING("HeapFreeBlocksNewEntry"));

    RWASSERT(NULL != heap);

    freeBlocks = heap->freeBlocks;
    entriesUsed = heap->entriesUsed;

    /* TODO[5]: replace the outer bits of this func with a macro so that
     * a function only gets called when freeBlocks need reallocing */
    if (heap->entriesAlloced <= entriesUsed)
    {
        freeBlocks = (rxHeapFreeBlock *)
            RwRealloc(heap->freeBlocks,
                      (heap->entriesAlloced +=
                       FREEBLOCKTBLGRANULARITY) *
                      sizeof(rxHeapFreeBlock),
                      rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE |
                      rwID_PIPEMODULE);

        if (freeBlocks == NULL)
        {
            RWERROR((E_RW_NOMEM,
                     heap->entriesAlloced*sizeof(rxHeapFreeBlock)));
            heap->entriesAlloced -= FREEBLOCKTBLGRANULARITY;
        }
        else
        {
            if ((freeBlocks != heap->freeBlocks) && (0 < entriesUsed))
            {
                /* must warn blocks that freeEntrys have moved! slow! */
                rxHeapFreeBlock    *iter = freeBlocks;

                do
                {
                    iter->ptr->freeEntry = iter;
                }
                while (iter++, --entriesUsed);
            }

            heap->freeBlocks = freeBlocks;
        }

    }

    if (freeBlocks) freeBlocks = &freeBlocks[heap->entriesUsed++];

    RWRETURN(freeBlocks);
}

/****************************************************************************
 HeapFreeBlocksDeleteEntry
 */

static void
HeapFreeBlocksDeleteEntry(RxHeap * heap, rxHeapFreeBlock * freeEntry)
{
    RWFUNCTION(RWSTRING("HeapFreeBlocksDeleteEntry"));

    RWASSERT(NULL != heap);

    if (&heap->freeBlocks[heap->entriesUsed - 1] != freeEntry)
    {
        /* overwrite freeentry w/ last free entry */
        *freeEntry = heap->freeBlocks[heap->entriesUsed - 1];

        /* & warn block its entry has moved */
        freeEntry->ptr->freeEntry = freeEntry;
    }

    heap->entriesUsed--;

    RWRETURNVOID();
}

/****************************************************************************
 HeapSuperBlockCreate

 note that largest block that can be allocated from a superblock is
 3*sizeof(rxHeapBlockHeader) bytes less than size
 */

/*#define SUPERBLOCKALIGNMENT 32U*/
#define SUPERBLOCKALIGNMENT 128U
#define SBAL (SUPERBLOCKALIGNMENT - 1)

static rxHeapSuperBlockDescriptor *
HeapSuperBlockCreate(RwUInt32 size)
{
    rxHeapSuperBlockDescriptor *superBlock;

    RWFUNCTION(RWSTRING("HeapSuperBlockCreate"));

    /* Make sure there's enough room in the superblock for two outer safety
     * blockheaders and one normal blockheader plus some actual memory! */
    RWASSERT(size >= (BLOCKSIZE(1) + 3 * sizeof(rxHeapBlockHeader)));

    /* allocate superblock descriptor + block,
     * & ensure block starts on 32-byte boundary */

    superBlock = (rxHeapSuperBlockDescriptor *)
        RwMalloc(sizeof(rxHeapSuperBlockDescriptor) + SBAL + size,
                 rwMEMHINTDUR_GLOBAL | rwMEMHINTFLAG_RESIZABLE |
                 rwID_PIPEMODULE);

    if (superBlock != NULL)
    {
        superBlock->start = (void *)
            ((((RwUInt32) & superBlock[1]) + SBAL) & ~SBAL);
        superBlock->size = size;
        superBlock->next = (rxHeapSuperBlockDescriptor *) NULL;
    }

    RWRETURN(superBlock);
}

/****************************************************************************
 HeapSuperBlockDestroy

 exists only for symmetry
 */

static void
HeapSuperBlockDestroy(rxHeapSuperBlockDescriptor * superBlock)
{
    RWFUNCTION(RWSTRING("HeapSuperBlockDestroy"));

    if (superBlock != NULL)
    {
        RwFree(superBlock);
    }

    RWRETURNVOID();
}

/****************************************************************************
 HeapSuperBlockReset

 partitions superblock into 3 blocks:
   zero byte used blocks at beg. [1] & end [2] of space +
   remainder as large free block [3]

 adds free block [3] to freeBlocks list

 fails if HeapFreeBlocksNewEntry() fails
 */

static              RwBool
HeapSuperBlockReset(rxHeapSuperBlockDescriptor * superBlock,
                    rxHeapSuperBlockDescriptor * attach2SuperBlock,
                    RxHeap * heap)
{
    rxHeapFreeBlock *freeEntry;

    RWFUNCTION(RWSTRING("HeapSuperBlockReset"));

    /* attach2SuperBlock will be given us as a "forward attachment",
     * and we will acquire it as a "backward
     * attachment": this is independent of the superblock l.l.
     * as defined by heap->head; these are links
     * on the RwHeapBlockHeaders of the zero byte used blocks */

    RWASSERT(NULL != heap);

    freeEntry = HeapFreeBlocksNewEntry(heap);

    if (NULL != freeEntry)
    {
        rxHeapBlockHeader  *blockHdrBeg, *blockHdrPrincipal,
            *blockHdrEnd;

        blockHdrBeg = (rxHeapBlockHeader *) superBlock->start;
        blockHdrPrincipal = &blockHdrBeg[1];
        blockHdrEnd = &((rxHeapBlockHeader *)
                        (((RwUInt8 *) superBlock->start) +
                         superBlock->size))[-1];

        blockHdrBeg->prev = (rxHeapBlockHeader *) NULL;
        blockHdrBeg->next = (rxHeapBlockHeader *) NULL;
        blockHdrBeg->size = 0;
        blockHdrBeg->freeEntry = (rxHeapFreeBlock *) NULL;

        *blockHdrEnd = *blockHdrBeg;

        /* blockHdrBeg -> blockHdrPrincipal */
        blockHdrBeg->next = blockHdrPrincipal;
        blockHdrPrincipal->prev = blockHdrBeg;

        /* blockHdrPrincipal-> blockHdrEnd */
        blockHdrPrincipal->next = blockHdrEnd;
        blockHdrEnd->prev = blockHdrPrincipal;

        blockHdrPrincipal->size =
            ((RwUInt8 *) blockHdrEnd) -
            ((RwUInt8 *) & blockHdrPrincipal[1]);
        blockHdrPrincipal->freeEntry = freeEntry;

        freeEntry->ptr = blockHdrPrincipal;
        freeEntry->size = blockHdrPrincipal->size;

        if (attach2SuperBlock != NULL)
        {
            rxHeapBlockHeader  *blockHdrAttach2 =
                &((rxHeapBlockHeader *)
                  (((RwUInt8 *) attach2SuperBlock->start) +
                   attach2SuperBlock->size))[-1];

            /* blockHdrAttach2 -> blockHdrBeg */
            blockHdrAttach2->next = blockHdrBeg;
            blockHdrBeg->prev = blockHdrAttach2;
        }

        RWRETURN(TRUE);
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 HeapGrow

 if toServiceAllocation non-zero, figure will be used to ensure that block
 linked in is large enough to service the given allocation

 returns pointer to the rxHeapBlockHeader of the block that's
 gonna satisfy your allocation
 */

static rxHeapBlockHeader *
HeapGrow(RxHeap * heap, RwUInt32 toServiceAllocation)
{
    RwUInt32            superBlockSize = toServiceAllocation +
        3 * sizeof(rxHeapBlockHeader);
    rxHeapSuperBlockDescriptor *superBlock;
    RwChar              string[256];

    RWFUNCTION(RWSTRING("HeapGrow"));

    /* check against heap->superBlockSize [size passed to RxHeapCreate()],
     * which is size of smallest block we'll create */
    if (superBlockSize < heap->superBlockSize)
    {
        superBlockSize = heap->superBlockSize;
    }

    if ((superBlock = HeapSuperBlockCreate(superBlockSize)))
    {
        /* know that head will have no "forward attachments" */
        if (HeapSuperBlockReset(superBlock, heap->head, heap))
        {
            rxHeapSuperBlockDescriptor *tempSuperBlock;
            RwUInt32            newSize = 0;

            /* created, reset (which will create the freeEntry), now final step
             * is to link into heap's superblock l.l. (insert at head) */
            superBlock->next = heap->head;
            heap->head = superBlock;

            /* Useful for the app to know the heap's changing */
            tempSuperBlock = heap->head;
            while (tempSuperBlock)
            {
                newSize += tempSuperBlock->size;
                tempSuperBlock = tempSuperBlock->next;
            }
            rwsprintf(string, "Heap resized from %d to %d bytes",
                      (newSize - superBlockSize), newSize);
            RwDebugSendMessage(rwDEBUGMESSAGE,
                               RWSTRING("RxHeapAlloc"),
                               RWSTRING(string));

            RWRETURN(&((rxHeapBlockHeader *) superBlock->start)[1]);
        }

        HeapSuperBlockDestroy(superBlock);
    }

    RWRETURN((rxHeapBlockHeader *) NULL);
}

/****************************************************************************
 HeapAllocFrom
 */

static void *
HeapAllocFrom(RxHeap * heap, rxHeapFreeBlock * freeEntry, RwUInt32 size)
{
    /* guaranteed to be a multiple of 32 */
    RwUInt32            delta;

    RWFUNCTION(RWSTRING("HeapAllocFrom"));

    RWASSERT(NULL != freeEntry);

    delta = freeEntry->size - size;

    /* don't bother creating free block for remainder
     * if v. small (< 256 bytes); */
    /* instead HeapAlloc() caller will get some bonus headroom! */

    /* TODO[5]: inline this in RxHeapAlloc() (it's only used there!)
     *          and use a goto there to avoid duplicating the code. */
    if (delta < 256)
    {
        rxHeapBlockHeader *blockHdr = freeEntry->ptr;

        HeapFreeBlocksDeleteEntry(heap, freeEntry);

        /* block is now not free */
        blockHdr->freeEntry = (rxHeapFreeBlock *) NULL;

        FLOG("serviced", blockHdr->size);

        RWRETURN((void *) &blockHdr[1]);
    }
    else
    {
        /* split! */
        /* new free block will be created from excess */

        /* block after us must be non-free,
         * coz we were free & otherwise would have been merged */

        rxHeapBlockHeader  *blockHdr = freeEntry->ptr;
        rxHeapBlockHeader  *newBlock =
            (rxHeapBlockHeader *) (((RwUInt8 *) & blockHdr[1]) + size);

        newBlock->size = delta - sizeof(rxHeapBlockHeader);
        newBlock->prev = blockHdr;
        newBlock->next = blockHdr->next;
        newBlock->freeEntry = freeEntry;

        blockHdr->size = size;
        blockHdr->next = newBlock;

        if (newBlock->next != NULL)
        {
            newBlock->next->prev = newBlock;
        }

        freeEntry->ptr = newBlock;
        freeEntry->size = newBlock->size;

        /* block is now not free */
        blockHdr->freeEntry = (rxHeapFreeBlock *) NULL;

        CHKHEAP(heap);

        FLOG("serviced", blockHdr->size);

        RWRETURN((void *) &blockHdr[1]);
    }
}

static void *
HeapReallocLarger(RxHeap * heap,
                  void *block,
                  RwUInt32 newSize,
                  rxHeapBlockHeader * blockHdr, RwInt32 delta)
{
    RWFUNCTION(RWSTRING("HeapReallocLarger"));

    RWASSERT(heap != NULL);
    RWASSERT(block != NULL);
    RWASSERT(newSize > blockHdr->size);
    RWASSERT(delta > 0);

    /* must grow... can next block make up shortfall? */
    if ((blockHdr->next != NULL) &&
        (blockHdr->next->freeEntry != NULL) &&
        (blockHdr->next->size + sizeof(rxHeapBlockHeader) >=
         (RwUInt32) delta))
    {
        RwUInt32            newBlockSize =
            blockHdr->size + blockHdr->next->size +
            sizeof(rxHeapBlockHeader);
        RwUInt32            xs = newBlockSize - newSize;

        if (xs < 256)
        {
            HeapFreeBlocksDeleteEntry(heap, blockHdr->next->freeEntry);

            blockHdr->size = newBlockSize;
            blockHdr->next = blockHdr->next->next;
            if (blockHdr->next != NULL)
            {
                blockHdr->next->prev = blockHdr;
            }
        }
        else
        {
            rxHeapBlockHeader  *shiftedPartition;

            shiftedPartition = (rxHeapBlockHeader *)
                (((RwUInt8 *) (blockHdr + 1)) + newSize);

            shiftedPartition->freeEntry = blockHdr->next->freeEntry;
            shiftedPartition->size = blockHdr->next->size - delta;
            shiftedPartition->prev = blockHdr;
            shiftedPartition->next = blockHdr->next->next;
            if (shiftedPartition->next != NULL)
            {
                shiftedPartition->next->prev = shiftedPartition;
            }

            blockHdr->size = newSize;
            blockHdr->next = shiftedPartition;

            shiftedPartition->freeEntry->ptr = shiftedPartition;
            shiftedPartition->freeEntry->size = shiftedPartition->size;
        }

        CHKHEAP(heap);

        FLOG("resize serviced", blockHdr->size);

        RWRETURN(block);
    }
    else
    {
        void               *newBlock;

        /* case from hell: must free & alloc & copy */

        FLOG("resize routed to alloc", 0);

        newBlock = RxHeapAlloc(heap, newSize);

        if (newBlock != NULL)
        {
            /* if we fail:
             * RWRETURN(NULL), but leave old block around
             */
            memcpy(newBlock, block, blockHdr->size);

            RxHeapFree(heap, block);

            CHKHEAP(heap);

            RWRETURN(newBlock);
        }
    }

    RWERROR((E_RW_NOMEM, newSize));

    RWRETURN(NULL);
}

static void *
HeapReallocSmaller(RxHeap * heap,
                   void *block,
                   RwUInt32 newSize,
                   rxHeapBlockHeader * blockHdr, RwInt32 delta)
{
    RWFUNCTION(RWSTRING("HeapReallocSmaller"));

    if (delta < 256)
    {
        /* see HeapAllocFrom():
         * not worth creating new free block */
        FLOG("resize no action", 0);

        RWRETURN(block);
    }
    else
    {
        /* this splitting of a block is almost identical
         * to behaviour in alloc() - can merge? */
        rxHeapBlockHeader  *newBlock;
        rxHeapFreeBlock    *freeEntry = HeapFreeBlocksNewEntry(heap);

        if (freeEntry == NULL)
        {
            /* block still original size (bigger than required) */
            RWRETURN(block);
        }

        newBlock = (rxHeapBlockHeader *)
            (((RwUInt8 *) & blockHdr[1]) + newSize);

        newBlock->size = delta - sizeof(rxHeapBlockHeader);
        newBlock->prev = blockHdr;
        newBlock->next = blockHdr->next;
        newBlock->freeEntry = freeEntry;

        blockHdr->size = newSize;
        blockHdr->next = newBlock;

        if (newBlock->next != NULL)
        {
            newBlock->next->prev = newBlock;
        }

        freeEntry->ptr = newBlock;
        freeEntry->size = newBlock->size;

        FLOG("resize serviced", blockHdr->size);

        CHKHEAP(heap);

        RWRETURN(block);
    }
}

static void *
HeapRealloc(RxHeap * heap, void *block,
            RwUInt32 newSize, RwBool __RWUNUSED__ allowCopy)
{
    void               *result = NULL;
    rxHeapBlockHeader  *blockHdr = ((rxHeapBlockHeader *) block) - 1;
    RwInt32             delta;

    RWFUNCTION(RWSTRING("HeapRealloc"));

    CHKHEAP(heap);

    FLOG("resize from", blockHdr->size);
    FLOG("resize to", newSize);

    newSize = BLOCKSIZE(newSize);

    delta = newSize - blockHdr->size;

    if (delta > 0)
    {
        result =
            HeapReallocLarger(heap, block, newSize, blockHdr, delta);
    }
    else
    {
        delta = -delta;

        result =
            HeapReallocSmaller(heap, block, newSize, blockHdr, delta);
    }

    RWRETURN(result);
}

#endif /* (!defined(DISABLERWHEAP)) */

/***************************************************************************
 * API functions
 */

/**
 * \ingroup rwcoregeneric
 * \ref RxHeapFree
 * returns a previously allocated block
 * of memory to an \ref RxHeap
 * for reuse.
 *
 * \param heap  A pointer to the heap within which to free memory
 * \param block  A pointer to the previously allocated block of memory
 *
 * \return None
 *
 * \see RxHeapAlloc
 * \see RxHeapCreate
 * \see RxHeapDestroy
 * \see RxHeapRealloc
 * \see RxHeapReset
 *
 */
void
RxHeapFree(RxHeap * heap, void *block)
{
    rxHeapBlockHeader  *blockHdr;
    RwBool              mergeBack;
    RwBool              mergeFwd;

    RWAPIFUNCTION(RWSTRING("RxHeapFree"));

#if (!defined(DISABLERWHEAP))

    blockHdr = ((rxHeapBlockHeader *) block) - 1;
    mergeBack =
        (blockHdr->prev != NULL) && (blockHdr->prev->freeEntry != NULL);
    mergeFwd =
        (blockHdr->next != NULL) && (blockHdr->next->freeEntry != NULL);

    CHKHEAP(heap);

    if (mergeBack)
    {
        if (mergeFwd)
        {
            /*  B &  F (gonna eliminate us & F) */
            HeapFreeBlocksDeleteEntry(heap, blockHdr->next->freeEntry);

            blockHdr->prev->size +=
                blockHdr->size + blockHdr->next->size +
                sizeof(rxHeapBlockHeader) * 2;
            blockHdr->prev->freeEntry->size = blockHdr->prev->size;
            blockHdr->prev->next = blockHdr->next->next;
            if (blockHdr->next->next != NULL)
            {
                blockHdr->next->next->prev = blockHdr->prev;
            }

            CHKHEAP(heap);
        }
        else
        {
            /*  B & !F (so going to eliminate us & fold into predecessor) */
            blockHdr->prev->size +=
                blockHdr->size + sizeof(rxHeapBlockHeader);
            blockHdr->prev->freeEntry->size = blockHdr->prev->size;
            blockHdr->prev->next = blockHdr->next;
            if (blockHdr->next != NULL)
            {
                blockHdr->next->prev = blockHdr->prev;
            }
            /* that was easy, wasn't it! */

            CHKHEAP(heap);
        }
    }
    else
    {
        if (mergeFwd)
        {
            /* !B &  F (gonna eliminate F) */
            blockHdr->size +=
                blockHdr->next->size + sizeof(rxHeapBlockHeader);
            blockHdr->freeEntry = blockHdr->next->freeEntry;
            blockHdr->next->freeEntry->ptr = blockHdr;
            blockHdr->next->freeEntry->size = blockHdr->size;
            blockHdr->next = blockHdr->next->next;
            if (blockHdr->next != NULL)
            {
                blockHdr->next->prev = blockHdr;
            }

            CHKHEAP(heap);
        }
        else
        {
            /* !B && !F */
            rxHeapFreeBlock *freeEntry = HeapFreeBlocksNewEntry(heap);

            if (freeEntry == NULL)
            {
                /* entry isn't freed,
                 * coz no memory to record the free block created!!! */
                RWRETURNVOID();
            }

            freeEntry->ptr = blockHdr;
            freeEntry->size = blockHdr->size;

            blockHdr->freeEntry = freeEntry;

            CHKHEAP(heap);
        }
    }

#else /*  (!defined(DISABLERWHEAP)) */

    RWASSERT(heap != NULL);
    RWASSERT(block != NULL);

    if ((heap != NULL) && (block != NULL))
    {
        rxHeapMallocTrace  *trace, *prevTrace;

        prevTrace = (rxHeapMallocTrace *)NULL;
        trace = (rxHeapMallocTrace *) heap->headBlock;
        while (trace && (trace->block != block))
        {
            prevTrace = trace;
            trace = trace->next;
        }
        if (trace == NULL)
        {
            /* Dat block not part of dis heap, no sah! */
            RwDebugSendMessage(rwDEBUGMESSAGE, RWSTRING("RxHeapFree"),
                               RWSTRING
                               ("Block doesn't belong to specified heap: cannot free"));
            RWRETURNVOID();
        }
        /* Snip this entry out ofthe chain */
        if (prevTrace != NULL)
        {
            prevTrace->next = trace->next;
        }
        else
        {
            heap->headBlock = (rxHeapBlockHeader *) (trace->next);
        }
        RwFree(block);
        RwFreeListFree((RwFreeList *) heap->freeBlocks, trace);
        heap->entriesUsed--;
        heap->entriesAlloced--;
        RWRETURNVOID();
    }
    RWERROR((E_RW_NULLP));

    /* Prevent unused variable warnings */
    blockHdr = (rxHeapBlockHeader *)NULL;
    mergeBack = FALSE;
    mergeFwd = FALSE;

#endif /*  (!defined(DISABLERWHEAP)) */

    RWRETURNVOID();
}

/**
 * \ingroup rwcoregeneric
 * \ref RxHeapAlloc
 * allocates a block of memory from an \ref RxHeap.
 *
 * \param heap  A pointer to the heap from which to allocate memory
 * \param size  An \ref RwUInt32 equal to the amount of memory to be
 *  allocated in bytes
 *
 * \return A pointer to the allocated block of memory on success, or NULL
 *  if there is an error
 *
 * \see RxHeapCreate
 * \see RxHeapDestroy
 * \see RxHeapFree
 * \see RxHeapRealloc
 * \see RxHeapReset
 *
 */
void *
RxHeapAlloc(RxHeap * heap, RwUInt32 size)
{
    RWAPIFUNCTION(RWSTRING("RxHeapAlloc"));

#if (!defined(DISABLERWHEAP))

    RWASSERT(NULL != heap);
    CHKHEAP(heap);

    FLOG("request", size);

    size = BLOCKSIZE(size);

    /* how to choose our block, eh? */
    /* got a noticeable, repeatable improvement by switching
     * from beg -> end traversal  of freeBlocks
     * to end -> beg traversal
     * (tends to increase reuse of free()-d blocks,
     * with ensuing cache benefit) */

    /* SDM 00/01/06: don't see *any* scenario in which
     *               heap->entriesUsed = 0, so why this test? */
    /* JNS 00/01/07: right, let's check that assertion
     * with an RWASSERT() */
    /* IBS 00/04/07: ok, comment conversations are cool, but since this
     * has never asserted yet, can we resolve this now? */
    RWASSERT(0 != heap->entriesUsed);
    {
        RwUInt32         n = heap->entriesUsed;
        rxHeapFreeBlock *freeEntry =
            &heap->freeBlocks[heap->entriesUsed - 1];

        do
        {
            if (freeEntry->size >= size)
            {
                heap->dirty = TRUE;
                RWRETURN(HeapAllocFrom(heap, freeEntry, size));
            }
        }
        while (freeEntry--, --n);

        /* okay, exhausted all free blocks & can't service allocation;
         * would traditionally have failed at this point, but now... */

        {
            rxHeapBlockHeader  *blockHdr = HeapGrow(heap, size);

            if (blockHdr != NULL)
            {
                heap->dirty = TRUE;
                RWRETURN(HeapAllocFrom
                         (heap, blockHdr->freeEntry, size));
            }
        }
    }

    RWERROR((E_RW_NOMEM, size));

#else /* (!defined(DISABLERWHEAP)) */

    RWASSERT(heap != NULL);
    {
        /* Allocate a block and add it to the list of allocated blocks */
        void    *newBlock = RwMalloc(size,
                    rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE |
                    rwID_PIPEMODULE);

        if (newBlock != NULL)
        {
            rxHeapMallocTrace  *newTrace = (rxHeapMallocTrace  *)
                RwFreeListAlloc((RwFreeList *) heap->freeBlocks,
                                rwMEMHINTDUR_EVENT | rwID_PIPEMODULE);

            if (newTrace)
            {
                /* Add a new rxHeapMallocTrace struct to
                 * the beginning of the linked list */
                newTrace->next = (rxHeapMallocTrace *) heap->headBlock;
                heap->headBlock = (rxHeapBlockHeader *) newTrace;
                newTrace->block = (rxHeapBlockHeader *) newBlock;
                heap->entriesUsed++;
                heap->entriesAlloced++;
                heap->dirty = TRUE;
                RWRETURN(newBlock);
            }
            RWERROR((E_RW_NOMEM, sizeof(rxHeapMallocTrace)));
            RWRETURN(NULL);
        }
        RWERROR((E_RW_NOMEM, size));
        RWRETURN(NULL);
    }
    RWERROR((E_RW_NULLP));

#endif /* (!defined(DISABLERWHEAP)) */

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxHeapRealloc
 * resizes a previously allocated block of memory within an \ref RxHeap,
 * possibly moving the block. The allowCopy parameter determines whether,
 * if the current block cannot be resized in-place, it should be copied
 * to a new location or whether this should be treated as failure (in
 * some cases, the caller may prefer to do something else if realloc'ing
 * requires an expensive copy).
 * \note AllowCopy is currently ignored.
 *
 * \param heap      A pointer to the heap from which the block of
 *                  memory was allocated
 * \param block     A pointer to the block of memory to be resized
 * \param newSize   An \ref RwUInt32 equal to the new size for the block
 * \param allowCopy TRUE if realloc is allowed to copy the block to a new
 *                  location, FALSE otherwise.
 *                  [Note: allowCopy is currently ignored]
 *
 * \return A pointer to the resized and possibly moved block of memory
 * on success, NULL otherwise.
 *
 * \see RxHeapAlloc
 * \see RxHeapCreate
 * \see RxHeapDestroy
 * \see RxHeapFree
 * \see RxHeapReset
 *
 */
void *
RxHeapRealloc(RxHeap * heap, void *block,
              RwUInt32 newSize,  RwBool __RWUNUSED__ allowCopy)
{
    void               *result = NULL;

    RWAPIFUNCTION(RWSTRING("RxHeapRealloc"));

#if (!defined(DISABLERWHEAP))

    RWASSERT(block != NULL);
    RWASSERT(newSize > 0);

    result = HeapRealloc(heap, block, newSize, allowCopy);

#else /* (!defined(DISABLERWHEAP)) */

    RWASSERT(heap != NULL);
    RWASSERT(block != NULL);

    if ((heap != NULL) && (block != NULL))
    {
        rxHeapMallocTrace  *trace;
        void               *reallocResult;

        trace = (rxHeapMallocTrace *) heap->headBlock;
        while (trace && (trace->block != block))
        {
            trace = trace->next;
        }
        if (trace == NULL)
        {
            /* Dat block not part of dis heap, no sah! */
            RwDebugSendMessage(rwDEBUGMESSAGE,
                               RWSTRING("RxHeapRealloc"),
                               RWSTRING
                               ("Block doesn't belong to specified heap: cannot resize"));
            RWRETURN(NULL);
        }
        reallocResult = RwRealloc(block, newSize, rwMEMHINTDUR_EVENT |
            rwMEMHINTFLAG_RESIZABLE | rwID_PIPEMODULE);
        if (reallocResult != NULL)
        {
            trace->block = (rxHeapBlockHeader *)reallocResult;
            RWRETURN(reallocResult);
        }
        RWERROR((E_RW_NOMEM, newSize));
        RWRETURN(NULL);
    }
    RWERROR((E_RW_NULLP));

#endif /* (!defined(DISABLERWHEAP)) */

    RWRETURN(result);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxHeapReset
 * resets an \ref RxHeap,
 * (quickly) marking
 * all memory allocations from that heap as free.
 *
 * \param heap  A pointer to the heap to be reset
 *
 * \return TRUE if successful, FALSE if there is an error
 *
 * \see RxHeapAlloc
 * \see RxHeapCreate
 * \see RxHeapDestroy
 * \see RxHeapFree
 * \see RxHeapRealloc
 *
 */
RwBool
#if (defined(DOXYGEN))
RxHeapReset(RxHeap * heap)
#else  /* (defined(DOXYGEN)) */
_rxHeapReset(RxHeap * heap)
#endif /* (defined(DOXYGEN)) */
{
    rxHeapSuperBlockDescriptor *prev =
        (rxHeapSuperBlockDescriptor *) NULL;
    rxHeapSuperBlockDescriptor *iter;

    RWFUNCTION(RWSTRING("_rxHeapReset"));

#if (!defined(DISABLERWHEAP))

    RWASSERT(NULL != heap);
    RWASSERT(FALSE != heap->dirty);

    heap->entriesUsed = 0;

    /* reset all superblocks in this order: head + 1 -> tail, then head.
     * why? am i mad? :-) coz... rule is that head superblock has
     * no "forward attachments"
     * [for simplicity in linking-in subsequent superblocks]
     */

    /* will have at least 1 superblock, guaranteed! */
    RWASSERT(NULL != heap->head);
    iter = heap->head->next;

    while (iter != NULL)
    {
        if (!HeapSuperBlockReset(iter, prev, heap))
        {
            RWRETURN(FALSE);
        }

        if (prev == NULL)
        {
            heap->headBlock = (rxHeapBlockHeader *) iter->start;
        }

        prev = iter;
        iter = iter->next;
    }

    iter = heap->head;

    if (!HeapSuperBlockReset(iter, prev, heap))
    {
        RWRETURN(FALSE);
    }

    if (prev == NULL)
    {
        heap->headBlock = (rxHeapBlockHeader *) iter->start;
    }

#else /*  (!defined(DISABLERWHEAP)) */

    RWASSERT(heap != NULL);
    RWASSERT(FALSE != heap->dirty);
    RWASSERT(heap->entriesAlloced == heap->entriesUsed);

    {
        /* In RWMEMDEBUG mode we just run thru a linked-list of blocks that
         * have been alloc'ed since the heap was last reset. */
        rxHeapMallocTrace  *trace =
            (rxHeapMallocTrace *) heap->headBlock, *deadTrace;

        while (trace)
        {
            deadTrace = trace;
            trace = trace->next;

            RwFree(deadTrace->block);
            heap->entriesUsed--;
            heap->entriesAlloced--;
            RwFreeListFree((RwFreeList *) heap->freeBlocks, deadTrace);
        }
        heap->headBlock = (rxHeapBlockHeader *)NULL;
        RWASSERT(heap->entriesUsed == 0);
        RWASSERT(heap->entriesAlloced == 0);
    }

    /* Prevent unused variable warnings */
    prev = (rxHeapSuperBlockDescriptor *)NULL;
    iter = (rxHeapSuperBlockDescriptor *)NULL;

#endif /* (!defined(DISABLERWHEAP)) */

    /* No need to reset it again until after something's actually alloc'd */
    heap->dirty = FALSE;

    RWRETURN(TRUE);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxHeapDestroy
 * destroys an \ref RxHeap
 * previously created by \ref RxHeapCreate,
 * freeing any outstanding memory allocations from that heap.
 *
 * \param heap  A pointer to the heap to be destroyed
 *
 * \return None
 *
 * \see RxHeapAlloc
 * \see RxHeapCreate
 * \see RxHeapFree
 * \see RxHeapRealloc
 * \see RxHeapReset
 *
 */
void
RxHeapDestroy(RxHeap * heap)
{
    RWAPIFUNCTION(RWSTRING("RxHeapDestroy"));

#if (!defined(DISABLERWHEAP))

    if (NULL != heap)
    {
        rxHeapSuperBlockDescriptor *superBlock;

        if (NULL != heap->freeBlocks)
        {
            RwFree(heap->freeBlocks);
            heap->freeBlocks = (rxHeapFreeBlock *) NULL;
        }

        superBlock = heap->head;
        /* free superblocks; note: new superblocks are inserted at
         * head of superblock l.l., so by traversing l.l. from head to
         * tail we free newest superblocks first (which is nice) */

        while (NULL != superBlock)
        {
            rxHeapSuperBlockDescriptor *const next = superBlock->next;

            HeapSuperBlockDestroy(superBlock);

            superBlock = next;
        }

        RwFree(heap);
        heap = (RxHeap *) NULL;
    }

#else /* (!defined(DISABLERWHEAP)) */

    RWASSERT(heap != NULL);

    if (heap != NULL)
    {
        RxHeapReset(heap);
        RwFreeListDestroy((RwFreeList *) heap->freeBlocks);
        RwFree(heap);
        RWRETURNVOID();
    }

    RWERROR((E_RW_NULLP));

#endif /* (!defined(DISABLERWHEAP)) */

    RWRETURNVOID();
}

/**
 * \ingroup rwcoregeneric
 * \ref RxHeapCreate
 * creates an \ref RxHeap
 * for dynamic memory management optimized for use during the execution of PowerPipe
 * pipelines. See \ref RxHeapGetGlobalHeap and \ref RxPipelineExecute
 * for details of current usage within PowerPipe.
 *
 * This heap structure is optimized for the following kind of process:
 * blocks of memory are allocated, a minority of these blocks are
 * freed or resized and at the end of the process, the entire set of
 * remaining allocations can be freed with one call (to \ref RxHeapReset).
 *
 * The \ref RxHeap provides very quick allocations but does not do
 * anything clever to deal with fragmentation within the heap. It
 * operates as its fastest if it does not have to resort to reusing
 * freed memory (i.e. if you can allocate all your blocks without
 * filling the heap, then do so without freeing any blocks and
 * merely call \ref RxHeapReset at the end of the process).
 *
 * If the heap is filled and cannot service an allocation, it will
 * automatically grow by the size passed to \ref RxHeapCreate or the
 * size of the allocation (whichever is the larger).
 *
 * \param size  An \ref RwUInt32 equal to the initial heap size in bytes
 *
 * \return A pointer to the created heap on success or NULL if there is
 *  an error
 *
 * \see RxHeapGetGlobalHeap
 * \see RxPipelineExecute
 * \see RxHeapAlloc
 * \see RxHeapDestroy
 * \see RxHeapFree
 * \see RxHeapRealloc
 * \see RxHeapReset
 *
 */
RxHeap *
RxHeapCreate(RwUInt32 size)
{
/* TODO: Allow specification of size and superBlockSize separately */
    RxHeap *heap;

    RWAPIFUNCTION(RWSTRING("RxHeapCreate"));

    if (size < MINHEAPSIZE) size = MINHEAPSIZE;

#if (!defined(DISABLERWHEAP))

    heap = (RxHeap *) RwMalloc(sizeof(RxHeap), rwMEMHINTDUR_GLOBAL |
                               rwID_PIPEMODULE);

    if (heap != NULL)
    {
        rxHeapSuperBlockDescriptor *superBlock;

        /* round size up to allocation granularity (= 32 bytes) */
        size = BLOCKSIZE(size);
        /* Make sure there's enough room in the superblock
         * for two outer safety blockheaders
         * and one normal blockheader plus some actual memory! */
        if (size < (BLOCKSIZE(1) + 3 * sizeof(rxHeapBlockHeader)))
        {
            size = BLOCKSIZE(1) + 3 * sizeof(rxHeapBlockHeader);
        }

        if ((superBlock = HeapSuperBlockCreate(size)) != NULL)
        {
            heap->superBlockSize = size;
            heap->head = superBlock;
            heap->freeBlocks = (rxHeapFreeBlock *) NULL;
            heap->entriesAlloced = 0;
            heap->entriesUsed = 0;
            /* Getting it reset will intialise it */
            heap->dirty = TRUE;

            if (RxHeapReset(heap))
            {
                RWRETURN(heap); /* SUCCESS */
            }

            HeapSuperBlockDestroy(superBlock);
        }

        RwFree(heap);
    }

#else /* (!defined(DISABLERWHEAP)) */

    heap = (RxHeap *) RwMalloc(sizeof(RxHeap), rwMEMHINTDUR_EVENT |
                               rwID_PIPEMODULE);

    if (heap != NULL)
    {
        /* Just use the freeBlocks header as a pointer to a freelist and the
         * headBlock pointer as a pointer to a linked-list allocated therefrom.
         * In DISABLERWHEAP(/RWMEMDEBUG) mode we just want to keep a list of
         * all alloced blocks so we can reset the heap and avoid seeing
         * leaks */
        heap->superBlockSize = 0;
        heap->head = (rxHeapSuperBlockDescriptor *)NULL;
        heap->freeBlocks =
            (rxHeapFreeBlock *)
            RwFreeListCreate(sizeof(rxHeapMallocTrace), 100, sizeof(RwUInt32),
                             rwMEMHINTDUR_EVENT | rwID_PIPEMODULE);
        if (heap->freeBlocks == NULL)
        {
            RwFree(heap);
            RWRETURN((RxHeap *)NULL);
        }
        heap->headBlock = (rxHeapBlockHeader *)NULL; /* The chain of mallocTrace starts here */
        heap->entriesAlloced = 0;
        heap->entriesUsed = 0;

        RWRETURN(heap);        /* SUCCESS */
    }

    RWERROR((E_RW_NOMEM, sizeof(RxHeap)));

    /* Prevent unused argument warnings */
    size = 0;

#endif /* (!defined(DISABLERWHEAP)) */

    RWRETURN((RxHeap *) NULL);
}
