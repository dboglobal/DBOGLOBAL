
/***************************************************************************
 *                                                                         *
 * Module  : balist.c                                                      *
 *                                                                         *
 * Purpose : List handling for game engine                                 *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"

#include "balist.h"


/****************************************************************************
 Local Types
 */

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

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Simple List handler

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwSListCreate

 On entry       : Entry size
 On exit        : SList (NULL on error)
 */

RwSList            *
_rwSListCreate(RwInt32 size, RwUInt32 hint)
{
    RwSList            *sList;

    RWFUNCTION(RWSTRING("_rwSListCreate"));
    RWASSERT(size > 0);

    sList = (RwSList *) RwMalloc(sizeof(RwSList), rwMEMHINTDUR_EVENT);
    if (!sList)
    {
        RWERROR((E_RW_NOMEM, (sizeof(RwSList))));
        RWRETURN((RwSList *)NULL);
    }

    sList->numElementsFilled = 0;
    sList->numElementsAlloced = SLIST_STARTSIZE;
    sList->entrySize = size;
    sList->listElements =
        (RwUInt8 *) RwMalloc(sList->numElementsAlloced * sList->entrySize,
                             hint | rwMEMHINTFLAG_RESIZABLE);

    if (!sList->listElements)
    {
        RWERROR((E_RW_NOMEM,(sList->numElementsAlloced * sList->entrySize)));
        RwFree(sList);

        RWRETURN((RwSList *)NULL);
    }

    RWRETURN(sList);
}

/****************************************************************************
 _rwSListEmpty

 On entry   : SList
 On exit    :
 */

void
_rwSListEmpty(RwSList * sList)
{
    RWFUNCTION(RWSTRING("_rwSListEmpty"));
    RWASSERT(sList);

    sList->numElementsFilled = 0;

    RWRETURNVOID();
}

/****************************************************************************
 _rwSListGetNewEntry

 On entry       : SList
 On exit        : New entry
 */

void               *
_rwSListGetNewEntry(RwSList * sList, RwUInt32 hint)
{
    void               *pData;

    RWFUNCTION(RWSTRING("_rwSListGetNewEntry"));
    RWASSERT(sList);

    if (sList->numElementsFilled >= sList->numElementsAlloced)
    {
        RwUInt8 *cpNew;

        cpNew = (RwUInt8 *) RwRealloc(sList->listElements,
                                      (sList->numElementsAlloced +
                                       sList->numElementsAlloced /
                                       4) * sList->entrySize,
                                       hint);
        if (!cpNew)
        {
            RWERROR((E_RW_NOMEM,
                     ((sList->numElementsAlloced +
                       sList->numElementsAlloced / 4) *
                      sList->entrySize)));
            RWRETURN(NULL);
        }

        sList->listElements = cpNew;
        sList->numElementsAlloced =
            sList->numElementsAlloced + sList->numElementsAlloced / 4;
    }

    pData = (void *)(&((sList->listElements)
                       [sList->numElementsFilled * sList->entrySize]));
    sList->numElementsFilled++;

    RWRETURN(pData);
}

/****************************************************************************
 _rwSListGetNewEntries

 On entry       : SList
 On exit        : New entry
 */

void *
_rwSListGetNewEntries(RwSList * sList,RwInt32 count, RwUInt32 hint)
{
    void               *pData;

    RWFUNCTION(RWSTRING("_rwSListGetNewEntries"));
    RWASSERT(sList);
    RWASSERT(count > 0);

    if (sList->numElementsFilled + count >= sList->numElementsAlloced)
    {
        RwUInt8 *cpNew;

        cpNew = (RwUInt8 *) RwRealloc(sList->listElements,
                                      (sList->numElementsAlloced +
                                       count +
                                       sList->numElementsAlloced /
                                       4) * sList->entrySize,
                                       hint);
        if (!cpNew)
        {
            RWERROR(
                    (E_RW_NOMEM,
                     ((sList->numElementsAlloced +
                       count +
                       sList->numElementsAlloced / 4) *
                      sList->entrySize)));
            RWRETURN(NULL);
        }

        sList->listElements = cpNew;
        sList->numElementsAlloced =
            sList->numElementsAlloced +
            count + sList->numElementsAlloced / 4;
    }

    pData = (void *) (&((sList->listElements)
                        [sList->numElementsFilled * sList->entrySize]));
    sList->numElementsFilled += count;

    RWRETURN(pData);
}

/****************************************************************************
 _rwSListDestroyEntry

 On entry   : SList
            : Entry to destroy
 On exit    : TRUE on success
 */

RwBool
_rwSListDestroyEntry(RwSList * sList, RwInt32 entry)
{
    RwInt32             nAfter;
    RwUInt8            *listElements;

    RWFUNCTION(RWSTRING("_rwSListDestroyEntry"));
    RWASSERT(sList);
    RWASSERT((entry >= 0) && (entry < sList->numElementsFilled));

    nAfter = sList->numElementsFilled - entry - 1;
    listElements = &((sList->listElements)[entry * sList->entrySize]);

    /* Shift all the other entries back */
    while (nAfter-- > 0)
    {
        memcpy(listElements, listElements + sList->entrySize,
               sList->entrySize);
        listElements += sList->entrySize;
    }

    /* Remove from the list */
    sList->numElementsFilled--;

    /* All done */
    RWRETURN(TRUE);
}

/****************************************************************************
 _rwSListDestroyEndEntries

 On entry   : SList
            : Amount of entries from end of list
 On exit    :
 */

void
_rwSListDestroyEndEntries(RwSList * sList, RwInt32 amount)
{
    RWFUNCTION(RWSTRING("_rwSListDestroyEndEntries"));
    RWASSERT(sList);
    RWASSERT(amount <= sList->numElementsFilled);

    sList->numElementsFilled -= amount;
    RWRETURNVOID();
}

/****************************************************************************
 _rwSListGetArray

 On entry   : SList
 On exit    : array of entries
 */

void               *
_rwSListGetArray(RwSList * sList)
{
    RWFUNCTION(RWSTRING("_rwSListGetArray"));
    RWASSERT(sList);

    if (sList->numElementsFilled)
    {
        RWRETURN((void *) (sList->listElements));
    }
    RWRETURN(NULL);
}

/*****************************************************************************
 _rwSListReleaseArray

 Should be performed before any other access on the SList is performed

 On entry   : SList
 On exit    : TRUE on success
 */

RwBool
_rwSListReleaseArray(RwSList * sList __RWUNUSEDRELEASE__)
{
    RWFUNCTION(RWSTRING("_rwSListReleaseArray"));
    RWASSERT(sList);

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwSListDestroy

 On entry       : SList
 On exit        : TRUE if ok
 */

RwBool
_rwSListDestroy(RwSList * sList)
{
    RWFUNCTION(RWSTRING("_rwSListDestroy"));
    RWASSERT(sList);

    if ((sList->listElements) && (sList->numElementsAlloced))
    {
        RwFree(sList->listElements);
        sList->listElements = (unsigned char *)NULL;
        sList->numElementsAlloced = 0;
    }

    RwFree(sList);

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwSListDestroyArray

 On entry       : Arrray generated by _rwSListToArray
 On exit        : TRUE if ok
 */

RwBool
_rwSListDestroyArray(RwUInt8 * array)
{
    RWFUNCTION(RWSTRING("_rwSListDestroyArray"));
    RWASSERT(array);

    RwFree(array);
    RWRETURN(TRUE);
}

/****************************************************************************
 _rwSListGetNumEntries

 On entry       : SList
 On exit        : Num entries
 */

RwInt32
_rwSListGetNumEntries(const RwSList * sList)
{
    RWFUNCTION(RWSTRING("_rwSListGetNumEntries"));
    RWASSERT(sList);

    RWRETURN(sList->numElementsFilled);
}

/****************************************************************************
 _rwSListToArray

 (NOTE THIS DESTROYS THE SLIST !!! )

 On entry       : SList
 On exit        : Array
 */

void               *
_rwSListToArray(RwSList * sList)
{
    RwUInt8            *cpNew = (unsigned char *)NULL;

    RWFUNCTION(RWSTRING("_rwSListToArray"));
    RWASSERT(sList);

    if (sList->numElementsFilled)
    {
        cpNew = sList->listElements;
    }
    else
        if (sList->listElements)
        {
            RwFree(sList->listElements);
            sList->listElements = (unsigned char *)NULL;
            cpNew = (unsigned char *)NULL;
        }

    RwFree(sList);
    RWRETURN((void *) cpNew);
}

/****************************************************************************
 _rwSListGetEntry

 On entry       : SList
                        : Element
 On exit        :
 */

void               *
_rwSListGetEntry(RwSList * sList, RwInt32 entry)
{
    RWFUNCTION(RWSTRING("_rwSListGetEntry"));
    RWASSERT(sList);
    RWASSERT((entry >= 0) && (entry < sList->numElementsFilled));

    RWRETURN((void *)&((sList->listElements)[sList->entrySize * entry]));
}

/****************************************************************************
 _rwSListGetBegin

 On entry       : SList

 On exit        :
 */

void               *
_rwSListGetBegin(RwSList * sList)
{
    RWFUNCTION(RWSTRING("_rwSListGetBegin"));
    RWASSERT(sList);

    RWRETURN((void *)&((sList->listElements)[0]));
}

/****************************************************************************
 _rwSListGetEnd

 On entry       : SList

 On exit        : returns one past-last-element; suitable for use in
                  for/while loops similar to C++ iterators
                     eg  while (current!=_rwSlistGetEnd) ++current;
 */

void               *
_rwSListGetEnd(RwSList * sList)
{
    RWFUNCTION(RWSTRING("_rwSListGetEnd"));
    RWASSERT(sList);

    RWRETURN((void *)&((sList->listElements)[sList->entrySize * sList->numElementsFilled]));
}
