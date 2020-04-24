/***************************************************************************
 *                                                                         *
 * Module  : balist.h                                                      *
 *                                                                         *
 * Purpose : list handling for engine                                      *
 *                                                                         *
 **************************************************************************/

#ifndef RWLIST_H
#define RWLIST_H

/****************************************************************************
 Includes
 */

#include "batypes.h"

/****************************************************************************
 Defines
 */

#define SLIST_STARTSIZE 20

/* RWPUBLIC */
/****************************************************************************
 Global Types
 */

typedef struct RwSList RwSList;

#if (!defined(DOXYGEN))
struct RwSList
{
    RwUInt8    *listElements;
    RwInt32     numElementsFilled;
    RwInt32     numElementsAlloced;
    RwInt32     entrySize;
};
#endif /* (!defined(DOXYGEN) */


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* SList functions */
extern RwSList    *_rwSListCreate(RwInt32 size, RwUInt32 hint);
extern RwBool      _rwSListDestroy(RwSList *sList);
extern RwBool      _rwSListDestroyArray(RwUInt8 *array);
extern void         _rwSListDestroyEndEntries(RwSList *sList, RwInt32 amount);
extern RwBool      _rwSListDestroyEntry(RwSList *sList, RwInt32 entry);
extern void         _rwSListEmpty(RwSList *sList);
extern void        *_rwSListGetArray(RwSList *sList);
extern void        *_rwSListGetEntry(RwSList *sList, RwInt32 entry);
extern void        *_rwSListGetNewEntry(RwSList *sList, RwUInt32 hint);
extern void        *_rwSListGetNewEntries(RwSList *sList, RwInt32 entry,
                                          RwUInt32 hint);
extern RwInt32     _rwSListGetNumEntries(const RwSList *sList);
extern RwBool      _rwSListReleaseArray(RwSList *sList);
extern void        *_rwSListToArray(RwSList *sList);
extern void        *_rwSListGetBegin(RwSList *sList);
extern void        *_rwSListGetEnd(RwSList *sList);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* Comparibility macros */

#define rwSListCreate(size, hint) \
       _rwSListCreate(size, hint)
#define rwSListDestroy(sList) \
       _rwSListDestroy(sList)
#define rwSListDestroyArray(array) \
       _rwSListDestroyArray(array)
#define rwSListDestroyEndEntries(sList, amount) \
       _rwSListDestroyEndEntries(sList, amount)
#define rwSListDestroyEntry(sList, entry) \
       _rwSListDestroyEntry(sList, entry)
#define rwSListEmpty(sList) \
       _rwSListEmpty(sList)
#define rwSListGetArray(sList) \
       _rwSListGetArray(sList)
#define rwSListGetEntry(sList, entry) \
       _rwSListGetEntry(sList, entry)
#define rwSListGetNewEntry(sList, hint) \
       _rwSListGetNewEntry(sList, hint)
#define rwSListGetNewEntries(sList, entry, hint) \
       _rwSListGetNewEntries(sList, entry, hint)
#define rwSListGetNumEntries(sList) \
       _rwSListGetNumEntries(sList)
#define rwSListReleaseArray(sList) \
       _rwSListReleaseArray(sList)
#define rwSListToArray(sList) \
       _rwSListToArray(sList)
#define rwSListGetBegin(sList) \
       _rwSListGetBegin(sList)
#define rwSListGetEnd(sList) \
       _rwSListGetEnd(sList)

/* RWPUBLICEND */

#endif /* RWLIST_H */
