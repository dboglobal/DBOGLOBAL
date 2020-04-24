/* p2heap.h */

#ifndef P2HEAP_H
#define P2HEAP_H

#define DISABLERWHEAPx
#define HEAPCHECKSx
#define LOGGINGx

/* RWPUBLIC */

#if (defined(RWDEBUG) && (defined(RWMEMDEBUG)))

#if (!defined(DISABLERWHEAP))
#define DISABLERWHEAP
#endif /* (!defined(DISABLERWHEAP)) */

#endif /* (defined(RWDEBUG) && (defined(RWMEMDEBUG))) */

typedef struct rxHeapFreeBlock rxHeapFreeBlock;
typedef struct rxHeapSuperBlockDescriptor rxHeapSuperBlockDescriptor;
typedef struct RxHeap RxHeap;
typedef struct rxHeapBlockHeader rxHeapBlockHeader;

#if (!defined(DOXYGEN))
struct rxHeapFreeBlock
{
    RwUInt32            size;
    rxHeapBlockHeader  *ptr;
};

struct rxHeapSuperBlockDescriptor
{
    void *start;
    RwUInt32 size;
    rxHeapSuperBlockDescriptor *next;
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rwcoregeneric
 * \struct RxHeap 
 * structure describing a pipeline execution heap 
 */
struct RxHeap
{
    RwUInt32            superBlockSize; /**< Granularity of heap growth */
    rxHeapSuperBlockDescriptor *head;   /**< Internally used superblock pointer */
    /* RWPUBLICEND
     * headBlock is used mainly for debugging.
     * RWPUBLIC */
    rxHeapBlockHeader  *headBlock;      /**< Internally used block pointer */
    rxHeapFreeBlock    *freeBlocks;     /**< Internally used free blocks pointer */
    RwUInt32            entriesAlloced; /**< Number of entries allocated */
    RwUInt32            entriesUsed;    /**< Number of entries used */
    RwBool              dirty;          /**< Internally used boolean, flags whether
                                         *   the heap needs resetting or not. */
};

#if (!defined(DOXYGEN))
struct rxHeapBlockHeader
{
    /* present in all blocks (used & unused) */
    rxHeapBlockHeader  *prev, *next;
    RwUInt32            size;
    rxHeapFreeBlock    *freeEntry; /* (or null) */
    RwUInt32            pad[4]; /* alignment padding to 32 bytes */
};
#endif /* (!defined(DOXYGEN)) */

/* This wrapper cheaply early-outs when a heap doesn't *need* resetting */
#define RxHeapReset(heap) \
    ((FALSE == (heap)->dirty) ? (TRUE) : (_rxHeapReset(heap)))

#ifdef __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RxHeap      *RxHeapCreate(RwUInt32 size);
extern void         RxHeapDestroy(RxHeap * heap);
extern RwBool       _rxHeapReset(RxHeap * heap);
extern void        *RxHeapAlloc(RxHeap * heap, RwUInt32 size);
extern void         RxHeapFree(RxHeap * heap, void *block);
extern void        *RxHeapRealloc(RxHeap * heap, void *block,
                                  RwUInt32 newSize, RwBool allowCopy);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#if (defined(DISABLERWHEAP))

typedef struct rxHeapMallocTrace rxHeapMallocTrace;
struct rxHeapMallocTrace
{
    rxHeapMallocTrace *next;
    rxHeapBlockHeader *block;
};

#endif                          /* (defined(DISABLERWHEAP)) */

/* RWPUBLICEND */

#endif                          /* P2HEAP_H */
