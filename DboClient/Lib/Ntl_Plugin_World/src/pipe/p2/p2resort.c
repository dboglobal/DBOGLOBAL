#define SMALLSUBFILETHRESHOLD 5

    /* comment out this define to remove the "small subfile" optimisation;
     * for further information, see:
     *
     * Sedgewick, "Algorithms in C++", Addison-Wesley, 1992, pp. 124 - 126
     *
     * essentially: partitions of size <= this value will be left unsorted,
     * to be handled by a later insertion sort pass
     *
     * DIGEST: seems to be a good thing to do if elements are relatively
     * small (i.e. if swap()s are not excessively costly)
     */

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"

#include "p2resort.h"
#include "p2core.h"


static              RwUInt32
_msbitpos(RwUInt32 un)
{
    RWFUNCTION(RWSTRING("_msbitpos"));

    if (un != 0)
    {
        RwUInt32            pos = 0;

        while ((un >>= 1) != 0)
        {
            pos++;
        }

        RWRETURN(pos);
    }
    else
    {
        RWRETURN((RwUInt32) ~ 0);
    }
}

#define SWAPVIATYPE(type)                                                     \
{                                                                             \
    while ( _elementSize >= sizeof(type) )                                    \
    {                                                                         \
        type t0 = *(type *) _l,                                               \
             t1 = *(type *) _r;                                               \
                                                                              \
        *(type *) _r = t0;                                                    \
        *(type *) _l = t1;                                                    \
                                                                              \
        _l += sizeof(type), _r += sizeof(type), _elementSize -= sizeof(type); \
    }                                                                         \
}

#define _swap(l, r, elementSize)           \
{                                          \
    RwUInt8 *_l = (RwUInt8 *) (l);         \
    RwUInt8 *_r = (RwUInt8 *) (r);         \
    RwUInt32 _elementSize = (elementSize); \
                                           \
    SWAPVIATYPE(RwUInt32);                 \
}

typedef struct tagStackEntry
{
    RwUInt8            *l;
    RwUInt8            *r;
    RwUInt32            bit;
}
StackEntry;

#define PUSH(_l, _r, _bit) ( stackptr->l   = (_l  ), \
                             stackptr->r   = (_r  ), \
                             stackptr->bit = (_bit), \
                             stackptr++ )

#define POP(_l, _r, _bit) ( stackptr--,            \
                            _l   = stackptr->l  ,  \
                            _r   = stackptr->r  ,  \
                            _bit = stackptr->bit )

#define STACKEMPTY() ( stackptr == &stack[0] )

/* for further information on radix exchange sort, see:

   Sedgewick, "Algorithms in C++", Addison-Wesley, 1992, pp. 135 - 139

   interesting feature of this algorithm is its suitably (as qsort()) to multiprocessing;
   once a partition has been created it can be acted on in isolation, so new partitions
   can be farmed out to new threads... */

static void
_repartition(RwUInt8 * l,
             RwUInt8 * r,
             RwUInt32 elementSize,
             RwUInt32 elementKeyOffset, RwUInt32 partitioningBit)
{

    StackEntry          stack[32], *stackptr = &stack[0];

    RWFUNCTION(RWSTRING("_repartition"));

    PUSH(l, r, partitioningBit);

    do
    {                          /* this loop pops deferred partitions off stack */
        POP(l, r, partitioningBit);

        do
        {                      /* this loop explores down through left partitions as far as poss., deferring rights */
            RwUInt8            *savel = l, *saver = r;

            do
            {                  /* this loop sorts a partition on partitioningBit */
                while (
                       ((*(RwUInt32 *) (l + elementKeyOffset)) &
                        partitioningBit) == 0)
                {
                    l += elementSize;

                    if (l > r) /* convergence: < l are established zeros;
                                * > r are established ones */
                    {
                        goto Convergence;
                    }
                }

                while (
                       ((*(RwUInt32 *) (r + elementKeyOffset)) &
                        partitioningBit) != 0)
                {
                    r -= elementSize;

                    if (l > r) /* convergence: < l are established zeros;
                                * > r are established ones */
                    {
                        goto Convergence;
                    }
                }

                _swap(l, r, elementSize);

                l += elementSize;
                r -= elementSize;
            }
            while (!(l > r));

          Convergence:
            if ((partitioningBit >>= 1) == 0)
            {
                break;
            }

            {
                RwUInt8            *defer_l = r + elementSize,
                    *defer_r = saver;

#ifdef SMALLSUBFILETHRESHOLD
                if (defer_r >=
                    defer_l + elementSize * SMALLSUBFILETHRESHOLD)
#else /* SMALLSUBFILETHRESHOLD */
                if (defer_r > defer_l) /* 2 or more elements in sub-partition */
#endif /* SMALLSUBFILETHRESHOLD */
                {
                    PUSH(defer_l, defer_r, partitioningBit);
                }
            }

            r = l - elementSize;
            l = savel;
#ifdef SMALLSUBFILETHRESHOLD
        }
        while (r >= l + elementSize * SMALLSUBFILETHRESHOLD);
#else /* SMALLSUBFILETHRESHOLD */
        }
        while (r > l);         /* 2 or more elements in sub-partition */
#endif /* SMALLSUBFILETHRESHOLD */
    }
    while (!STACKEMPTY());

    RWRETURNVOID();
}

/* requires that first element is in correct place (otherwise may underrun array);
   _rx_rxRadixExchangeSort() ensures this is the case prior to calling */

static void
_insertionsort(RwUInt8 * elements,
               RwUInt32 numElements,
               RwUInt32 elementSize, RwUInt32 elementKeyOffset)
{
    RWFUNCTION(RWSTRING("_insertionsort"));

    while (elements += elementSize, --numElements)
    {
        RwUInt32            keyToPlace =

            *(RwUInt32 *) (elements + elementKeyOffset);

        RwUInt8            *p = elements;

        /* element is not in correct place? if not, swap back to insertion point */

        while (p -= elementSize,
               *(RwUInt32 *) (p + elementKeyOffset) > keyToPlace)
        {
            _swap(p, p + elementSize, elementSize);
        }
    }

    RWRETURNVOID();
}

void
_rx_rxRadixExchangeSort(void *elements,
                  RwUInt32 numElements,
                  RwUInt32 elementSize,
                  RwUInt32 elementKeyOffset,
                  RwUInt32 keyLo, RwUInt32 keyHi)
{
    RWFUNCTION(RWSTRING("_rx_rxRadixExchangeSort"));

    if (elements != NULL &&
        elementKeyOffset + sizeof(RwUInt32) <= elementSize &&
        keyLo < keyHi)
    {
#ifdef SMALLSUBFILETHRESHOLD
        if (numElements > SMALLSUBFILETHRESHOLD)
#else /* SMALLSUBFILETHRESHOLD */
        if (numElements > 1)   /* 2 or more elements */
#endif /* SMALLSUBFILETHRESHOLD */
        {
            _repartition((RwUInt8 *) elements,
                         (RwUInt8 *) elements + (numElements -
                                                 1) * elementSize,
                         elementSize, elementKeyOffset,
                         1U << _msbitpos(keyHi));
        }

#ifdef SMALLSUBFILETHRESHOLD
        if (numElements > 1)
        {
            /* max. size of unsorted partition is SMALLSUBFILETHRESHOLD, so
             * search from 0 to SMALLSUBFILETHRESHOLD - 1 for lowest key value;
             * having lowest key element in correct place allows elimination of
             * conditional from insertion sort inner loop */

            RwUInt32            i = SMALLSUBFILETHRESHOLD - 1;
            RwUInt32            minKey, minKeyIndex;

            if (i > numElements - 1)
            {
                i = numElements - 1;
            }

            minKey =
                *(RwUInt32 *) (((RwUInt8 *) elements) +
                               i * elementSize + elementKeyOffset);
            minKeyIndex = i;

            i--;

            do
            {
                RwUInt32            key =
                    *(RwUInt32 *) (((RwUInt8 *) elements) +
                                   i * elementSize + elementKeyOffset);

                if (key < minKey)
                {
                    minKey = key;
                    minKeyIndex = i;
                }
            }
            while (i--);

            if (minKeyIndex != 0)
            {
                _swap((RwUInt8 *) elements,
                      ((RwUInt8 *) elements) +
                      minKeyIndex * elementSize, elementSize);
            }

            _insertionsort((RwUInt8 *) elements,
                           numElements, elementSize, elementKeyOffset);
        }
#endif /* SMALLSUBFILETHRESHOLD */
    }

    RWRETURNVOID();
}

void
_rxSelectionSort(void *elements,
              RwUInt32 numElements,
              RwUInt32 elementSize,
              RwUInt32 elementKeyOffset, RwUInt32 keyLo, RwUInt32 keyHi)
{
    RWFUNCTION(RWSTRING("_rxSelectionSort"));

    if (elements != NULL &&
        elementKeyOffset + sizeof(RwUInt32) <= elementSize &&
        keyLo < keyHi && numElements > 1 /* 2 or more elements */ )
    {
        RwUInt8            *elem = (RwUInt8 *) elements;

        do
        {
            RwUInt32            minKey =
                *(RwUInt32 *) (elem + elementKeyOffset);
            RwUInt8            *minKeyPtr = (RwUInt8 *) NULL;

            RwUInt8            *scan = elem;
            RwUInt32            n = numElements;

            while (scan += elementSize, --n)
            {
                RwUInt32            key =

                    *(RwUInt32 *) (scan + elementKeyOffset);

                if (key < minKey)
                {
                    minKey = key;
                    minKeyPtr = scan;
                }
            }

            if (minKeyPtr != NULL)
            {
                _swap(minKeyPtr, elem, elementSize);
            }
        }
        while (elem += elementSize, --numElements);
    }

    RWRETURNVOID();
}
