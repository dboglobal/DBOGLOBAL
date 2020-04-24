/***************************************************************************
 *                                                                         *
 * Module  : resmem.h                                                      *
 *                                                                         *
 * Purpose : Resource memory heap allocator                                *
 *                                                                         *
 **************************************************************************/

#ifndef RESMEM_H
#define RESMEM_H

/****************************************************************************
 Includes
 */

#include "batypes.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* Init and de-init */
extern RwBool   _rwResHeapInit(void *resHeap, RwUInt32 size);
extern RwBool   _rwResHeapClose(void *resHeap);

/* Allocating and freeing memory */
extern void *   _rwResHeapAlloc(void *resHeap, RwUInt32 size);
extern void *   _rwResHeapCalloc(void *resHeap, RwUInt32 num, RwUInt32 size);
extern void *   _rwResHeapRealloc(void *memory, RwUInt32 size);
extern void     _rwResHeapFree(void *memory);

/* Usage statistics */
extern RwBool   _rwResHeapGetStats(void     *resHeap,
                                   RwUInt32 *totalAllocated, 
                                   RwUInt32 *totalFree,
                                   RwUInt32 *largestAllocated,
                                   RwUInt32 *largestFree,
                                   RwUInt32 *smallestAllocated,
                                   RwUInt32 *smallestFree); 

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* RESMEM_H */
