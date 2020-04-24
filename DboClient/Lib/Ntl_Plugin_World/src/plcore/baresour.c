/*
 * Resource handling.
 * Resources are used to instance objects into.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/**
 * \ingroup rwresources
 * \page rwresourcesoverview RwResources Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection resourcesoverview Overview
 *
 * This object represents the RenderWare Resource Arena cache.
 *
 * During rendering RenderWare stores platform-dependent instances of model geometry within
 * this cache. If the model is not changed substantially from one cycle to the next, the
 * instanced data is simply fetched and transferred to the rendering hardware, rather than being
 * recomputed.
 *
 * This system improves the speed of the rendering process, but the size of the Resource
 * Arena needs to be fine-tuned for best results. It is possible for a badly-tuned Resource
 * Arena to affect performance.
 *
*/


/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"
#include "resmem.h"

#include "baresour.h"

/* A switch to use dynamic but capped memory allocation rather than single
 * block managed by _rwResHeap functions
 */
#define RWRESOURCESDYNAMICx

/****************************************************************************
 Globals (across program)
 */

RwModuleInfo resourcesModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 rwResourcesInit

 On entry   : Reosurces to init, size
 On exit    : Resources pointer on success
 */

static rwResources *
ResourcesInit(rwResources * res, RwUInt32 size)
{
    RWFUNCTION(RWSTRING("ResourcesInit"));

#if (!defined(RWRESOURCESDYNAMIC))

    if (size > 0)
    {
        /* Set up a single block of memory for the resources heap */
        res->memHeap = RwMalloc(size,
            rwMEMHINTDUR_GLOBAL | rwID_RESOURCESMODULE);
        if (!res->memHeap)
        {
            RWERROR((E_RW_NOMEM, size));
            RWRETURN((rwResources *) NULL);
        }

        if (!_rwResHeapInit(res->memHeap, size))
        {
            RwFree(res->memHeap);
            RWERROR((E_RW_INSUFFICIENTRESOURCES, 0));
            RWRETURN((rwResources *) NULL);
        }
    }
    else
    {
        res->memHeap = NULL;
    }

#endif /* (!defined(RWRESOURCESDYNAMIC)) */

    rwLinkListInitialize(&res->entriesA);
    rwLinkListInitialize(&res->entriesB);

    res->usedEntries = &res->entriesA;
    res->freeEntries = &res->entriesB;

    res->maxSize = size;
    res->currentSize = 0;
    res->reusageSize = 0;

    /* All done */
    RWRETURN(res);
}

/****************************************************************************
 _rwResourcesOpen

 On entry   : instance, offset, size
 On exit    : instance pointer on success
 */

void               *
_rwResourcesOpen(void *instance, RwInt32 offset,
                 RwInt32 __RWUNUSED__ size)
{
    RwUInt32            arenaSize;

    RWFUNCTION(RWSTRING("_rwResourcesOpen"));
    RWASSERT(instance);

    /* Grab the globals offset (same for all instances) */
    resourcesModule.globalsOffset = offset;

    arenaSize = RWSRCGLOBAL(resArenaInitSize);

    RWMESSAGE((RWSTRING("%d bytes allocated for resources arena."),
              arenaSize));

    /* Then continue with the initialization */
    if (!ResourcesInit(&RWRESOURCESGLOBAL(res), arenaSize))
    {
        RWRETURN(NULL);
    }

    /* One more module instance */
    resourcesModule.numInstances++;

    RWRETURN(instance);
}

/****************************************************************************
 _rwResourcesClose

 On entry   : instance, offset, size
 On exit    : instance pointer on success
 */

void               *
_rwResourcesClose(void *instance,
                  RwInt32 __RWUNUSED__ offset,
                  RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwResourcesClose"));
    RWASSERT(instance);

    /* Release all the resources */
    RwResourcesEmptyArena();

#if (!defined(RWRESOURCESDYNAMIC))

    /* Free the block of memory used for the resources heap */
    if(NULL != RWRESOURCESGLOBAL(res.memHeap))
    {
        _rwResHeapClose(RWRESOURCESGLOBAL(res.memHeap));
        RwFree(RWRESOURCESGLOBAL(res.memHeap));
        RWRESOURCESGLOBAL(res.memHeap) = NULL;
    }

#endif /* (!defined(RWRESOURCESDYNAMIC)) */

    /* One less module instance */
    resourcesModule.numInstances--;

    RWRETURN(instance);
}

/**
 * \ingroup rwresources
 * \ref RwResourcesFreeResEntry is used to release the memory allocated
 * to the specified resources entry. The destroy notify callback (if one was
 * specified at allocation time) will be called before the memory is freed.
 * The pointer that was filled in at creation time will be set back to NULL
 * to indicate that the memory has been freed.
 *
 * \param entry  Pointer to the resources entry.
 *
 * \return Returns TRUE is successful or FALSE if there is an error.
 *
 * \see RwResourcesAllocateResEntry
 * \see RwResourcesUseResEntry
 *
 */
RwBool
RwResourcesFreeResEntry(RwResEntry * entry)
{
    RWAPIFUNCTION(RWSTRING("RwResourcesFreeResEntry"));
    RWASSERT(resourcesModule.numInstances);
    RWASSERT(entry);

    /* About to go away, call the destroy notify if applicable */
    if (entry->destroyNotify)
    {
        entry->destroyNotify(entry);
    }

    /* unlink user of resource */
    /* Do not remove ths test. We now permit orphan ResEntries which have
     * no ownerRef. This is to provide procrastinated destroy */
    if (entry->ownerRef)
    {
        *(entry->ownerRef) = (RwResEntry *) NULL;
    }

    if (entry->link.next)
    {
        /* Remove the link list attachment */
        rwLinkListRemoveLLLink(&entry->link);

        /* Adjust the current size */
        RWRESOURCESGLOBAL(res.currentSize) -= entry->size;

#if defined(PSP_DRVMODEL_H)
        /*
         * Restore the original resource allocation pointer...
         * (Kept 4 bytes before the aligned pointer.)
         */
        entry = (RwResEntry *)
            *((RwUInt32 *)(((RwUInt32)entry) - 4));
#endif /* PSP_DRVMODEL_H */

#ifdef RWRESOURCESDYNAMIC

        /* Free from main memory */
        RwFree(entry);

#else /* RWRESOURCESDYNAMIC */

        /* Free from resources heap */
        _rwResHeapFree(entry);

#endif /* RWRESOURCESDYNAMIC */

    }
    else
    {
        /* This entry is not in a list and must be in main memory */

#if defined(PSP_DRVMODEL_H)
        /*
         * Restore the original resource allocation pointer...
         * (Kept 4 bytes before the aligned pointer.)
         */
        entry = (RwResEntry *)
            *((RwUInt32 *)(((RwUInt32)entry) - 4));
#endif /* PSP_DRVMODEL_H */

        RwFree(entry);
    }

    /* All done */
    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Get some resources

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwResourcesPurge

 Mark all used entries as free

 */

void
_rwResourcesPurge(void)
{
    RwLinkList         *usedEntries, *freeEntries;

    RWFUNCTION(RWSTRING("_rwResourcesPurge"));

    /* Stick the current free on the end of the used list */
    usedEntries = RWRESOURCESGLOBAL(res.usedEntries);
    freeEntries = RWRESOURCESGLOBAL(res.freeEntries);

    /* Copy from free into used */
    if (!rwLinkListEmpty(freeEntries))
    {
        if (rwLinkListEmpty(usedEntries))
        {
            usedEntries->link.next = freeEntries->link.next;
            (usedEntries->link.next)->prev = &(usedEntries->link);

            usedEntries->link.prev = freeEntries->link.prev;
            (usedEntries->link.prev)->next = &(usedEntries->link);

            /* Empty the list */
            rwLinkListInitialize(freeEntries);
        }
        else
        {
            RwLLLink           *first, *last;

            /* Stick together middle */
            last = rwLinkListGetLastLLLink(usedEntries);
            first = rwLinkListGetFirstLLLink(freeEntries);

            last->next = first;
            first->prev = last;

            /* Attach end */
            last = rwLinkListGetLastLLLink(freeEntries);

            last->next = rwLinkListGetTerminator(usedEntries);
            usedEntries->link.prev = last;

            /* Empty the list */
            rwLinkListInitialize(freeEntries);
        }
    }

    /* Swap over */
    RWRESOURCESGLOBAL(res.usedEntries) = freeEntries;
    RWRESOURCESGLOBAL(res.freeEntries) = usedEntries;

    /* Reset the reusage */
    RWRESOURCESGLOBAL(res.reusageSize) = 0;

    /* Done */
    RWRETURNVOID();
}

/**
 * \ingroup rwresources
 * \ref RwResourcesAllocateResEntry is a function used to allocate
 * a resources entry for the specified object of the given size.
 *
 * Also use this function to specify a callback function that will be
 * executed immediately before the resources entry is removed from the
 * resources arena (pass NULL to indicate there is no callback).
 *
 * \param owner  Pointer to the object that will own the resources entry.
 * \param ownerRef  Pointer to a pointer that will receive a reference to
 *       the resources entry.
 * \param size  A RwInt32 value equal to the size of the resources entry
 *       to allocate (in bytes).
 * \param destroyNotify  Pointer to destroy notify callback function.
 *       Specify NULL to ignore.
 *
 * \return Returns pointer to the allocated resources block if successful
 *        or NULL if there is an error.
 *
 * \see RwResourcesFreeResEntry
 * \see RwResourcesUseResEntry
 */
RwResEntry *
RwResourcesAllocateResEntry(void *owner,
                            RwResEntry **ownerRef,
                            RwInt32 size,
                            RwResEntryDestroyNotify destroyNotify)
{
    RwResEntry  *entry;
    RwInt32     allocSize;
    RwBool      exhaustedOptions;

    RWAPIFUNCTION(RWSTRING("RwResourcesAllocateResEntry"));
    RWASSERT(resourcesModule.numInstances);
    RWASSERT(owner);

#if defined(PSP_DRVMODEL_H)
    allocSize = (4 + 63 + size + sizeof(RwResEntry));
#else /* PSP_DRVMODEL_H */
    allocSize =          (size + sizeof(RwResEntry));
#endif /* PSP_DRVMODEL_H */

    exhaustedOptions = FALSE;
    while (!exhaustedOptions)
    {
        RwLLLink           *cur, *end;

#ifdef RWRESOURCESDYNAMIC

        /* Do capped dynamic memory allocation */
        if ((RWRESOURCESGLOBAL(res.currentSize) + size) <=
            RWRESOURCESGLOBAL(res.maxSize))
        {
            entry = (RwResEntry *) RwMalloc(allocSize,
                           rwMEMHINTDUR_EVENT | rwID_RESOURCESMODULE);
            if (!entry)
            {
                RWERROR((E_RW_NOMEM, (allocSize)));
                RWRETURN(NULL);
            }
        }
        else
        {
            entry = (RwResEntry *) NULL;
        }

#else /* RWRESOURCESDYNAMIC */

        /* Allocate from the resources heap */
        entry =
            (RwResEntry *)
            _rwResHeapAlloc(RWRESOURCESGLOBAL(res.memHeap),
                            allocSize);

#endif /* RWRESOURCESDYNAMIC */

        if (entry)
        {
#if defined(PSP_DRVMODEL_H)
            RwUInt32    *originalAddress;
            RwUInt32    entryAlign;

            /* Round the address to the next 64 byte boundry after
             * the original pointer placeholder... */
            entryAlign  = (RwUInt32)entry;
            entryAlign += (4 + 63);
            entryAlign &= ~63;

            /* Store the original allocation pointer, 4 bytes before
             * the aligned pointer...
             */
            originalAddress  = (RwUInt32 *)(entryAlign - 4);
            *originalAddress = (RwUInt32)entry;

            /* DANGER! - Moving allocated pointer to next aligned boundry... */
            entry = (RwResEntry *)entryAlign;
#endif /* PSP_DRVMODEL_H */

            /* We have an entry */
            rwLinkListAddLLLink(RWRESOURCESGLOBAL(res.usedEntries),
                                &entry->link);
            entry->owner = owner;
            entry->size = size;
            entry->ownerRef = ownerRef;
            entry->destroyNotify = destroyNotify;

            RWRESOURCESGLOBAL(res.currentSize) += size;

            /* Do not remove this test. We now permit orphaned ResEntries */
            if (ownerRef)
            {
                *ownerRef = entry;
            }

#ifdef RWMETRICS
            RWSRCGLOBAL(metrics)->numResourceAllocs++;
#endif /* RWMETRICS */

            RWRETURN(entry);
        }

        /* Try freeing up some memory from the not used list */
        cur =
            rwLinkListGetLastLLLink(RWRESOURCESGLOBAL(res.freeEntries));
        end =
            rwLinkListGetTerminator(RWRESOURCESGLOBAL(res.freeEntries));
        if (cur != end)
        {
            /* Free its memory */
            entry = rwLLLinkGetData(cur, RwResEntry, link);

            /* Deallocate the resources */
            RwResourcesFreeResEntry(entry);
        }
        else
        {
            /* Try freeing up some memory from the used list and try to alloc */

            cur =
                rwLinkListGetLastLLLink(RWRESOURCESGLOBAL
                                        (res.usedEntries));
            end =
                rwLinkListGetTerminator(RWRESOURCESGLOBAL
                                        (res.usedEntries));
            if (cur != end)
            {
                /* Free its memory */
                entry = rwLLLinkGetData(cur, RwResEntry, link);

                /* Before we free it though, add its size to the usage, since we wont
                 * be able to count this one later.
                 */
                RWRESOURCESGLOBAL(res.reusageSize) += entry->size;

                /* Deallocate the resources */
                RwResourcesFreeResEntry(entry);

                /* Resource arena thrashing. */
                RwDebugSendMessage(rwDEBUGMESSAGE,
                        "RwResourcesAllocateResEntry",
                        "Resource Arena memory thrashing.");
            }
            else
            {
                /* We've emptied both lists, yet still we can't allocate resources */
                exhaustedOptions = TRUE;

                /* Resource arena exhausion. */
                RwDebugSendMessage(rwDEBUGMESSAGE,
                        "RwResourcesAllocateResEntry",
                        "Resource Arena memory exhausion.");

            }
        }
    }

    /* Couldn't do it */
    /* Do not remove this test. We now permit orphaned ResEntries */
    if (ownerRef)
    {
        *ownerRef = (RwResEntry *) NULL;
    }
    RWERROR((E_RW_INSUFFICIENTRESOURCES, size));
    RWRETURN((RwResEntry *) NULL);
}

#if ((defined(RWDEBUG)) || (defined(RWSUPPRESSINLINE)))

/**
 * \ingroup rwresources
 * \ref RwResourcesUseResEntry is used to mark the specified resources
 * entry as still in use. It should be called every rendering frame that a
 * resources entry is used to protect the resource against being
 * unnecessarily freed.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param entry  Pointer to the resources entry.
 *
 * \return Returns pointer to the resources entry if successful or NULL if
 * there is an error.
 *
 * \see RwResourcesAllocateResEntry
 * \see RwResourcesFreeResEntry
 *
 */
RwResEntry         *
RwResourcesUseResEntry(RwResEntry * entry)
{
    RWAPIFUNCTION(RWSTRING("RwResourcesUseResEntry"));
    RWASSERT(resourcesModule.numInstances);
    RWASSERT(entry);

    if (entry->link.next)
    {
        /* Remove from list -> attach to used list */
        rwLinkListRemoveLLLink(&entry->link);

        /* Attach to used */
        rwLinkListAddLLLink(RWRESOURCESGLOBAL(res.usedEntries),
                            &entry->link);
    }

    /* All done */
    RWRETURN(entry);
}

#endif /* ((defined(RWDEBUG)) || (defined(RWSUPPRESSINLINE))) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Resources

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwresources
 * \ref RwResourcesSetArenaSize is used to specify the size of the memory
 * block reserved for the resources instancing arena.
 *
 * This should be used in between \ref RwEngineInit and
 * \ref RwEngineStart to set the default size for the arena before the
 * memory is allocated. If used at a later stage, the arena will be emptied
 * and the memory reallocated.
 *
 * \param size  A RwUInt32 value equal to the arena size (in bytes)
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwResourcesEmptyArena
 * \see RwResourcesGetArenaSize
 * \see RwResourcesGetArenaUsage
 *
 */
RwBool
RwResourcesSetArenaSize(RwUInt32 size)
{
    rwResources        *res;

    RWAPIFUNCTION(RWSTRING("RwResourcesSetArenaSize"));

    /* If module has not yet been opened, then override the default size */
    if (resourcesModule.numInstances == 0)
    {
        RWSRCGLOBAL(resArenaInitSize)  = size;

        RWRETURN(TRUE);
    }

    /* Otherwise modify the existing arena size */
    res = &RWRESOURCESGLOBAL(res);
    res->maxSize = size;

#ifdef RWRESOURCESDYNAMIC

    /* Shrink down dynamically allocated resources to specified size
     * if necessary */
    while (res->currentSize > res->maxSize)
    {
        RwLLLink           *cur, *end;

        /* Try freeing up some memory from the not used list */
        cur = rwLinkListGetLastLLLink(res->freeEntries);
        end = rwLinkListGetTerminator(res->freeEntries);
        if (cur != end)
        {
            /* Free its memory */
            RwResEntry         *entry =
                rwLLLinkGetData(cur, RwResEntry, link);

            /* Deallocate the resources */
            RwResourcesFreeResEntry(entry);
        }
        else
        {
            /* Try freeing up some memory from the used list */
            cur = rwLinkListGetLastLLLink(res->usedEntries);
            end = rwLinkListGetTerminator(res->usedEntries);
            if (cur != end)
            {
                /* Free its memory */
                RwResEntry         *entry =
                    rwLLLinkGetData(cur, RwResEntry, link);

                /* Deallocate the resources */
                RwResourcesFreeResEntry(entry);
            }
            else
            {
                /* We've emptied both lists,
                 * yet still we can't meet the size target */
                RWRETURN(FALSE);
            }
        }
    }

#else /* RWRESOURCESDYNAMIC */

    /* Empty the arena */
    RwResourcesEmptyArena();

    /* Close and free the resources heap */
    _rwResHeapClose(res->memHeap);
    RwFree(res->memHeap);

    /* Allocate a new heap */
    res->memHeap = RwMalloc(size,
        rwMEMHINTDUR_EVENT | rwID_RESOURCESMODULE);
    if (!res->memHeap)
    {
        res->maxSize = 0;
        RWERROR((E_RW_NOMEM, size));
        RWRETURN(FALSE);
    }

    /* Initialise the heap */
    if (!_rwResHeapInit(res->memHeap, size))
    {
        RwFree(res->memHeap);
        RWERROR((E_RW_INSUFFICIENTRESOURCES, 0));
        RWRETURN(FALSE);
    }

#endif /* RWRESOURCESDYNAMIC */

    RWRETURN(TRUE);
}

/**
 * \ingroup rwresources
 * \ref RwResourcesGetArenaSize is used to determine the size
 * (in bytes) of the current instancing arena.
 *
 * \return Returns a \ref RwInt32 value equal to the size of the resources
 *         arena (in bytes)
 *
 * \see RwResourcesEmptyArena
 * \see RwResourcesGetArenaUsage
 * \see RwResourcesSetArenaSize
 *
 */
RwInt32
RwResourcesGetArenaSize(void)
{
    RWAPIFUNCTION(RWSTRING("RwResourcesGetArenaSize"));
    RWASSERT(resourcesModule.numInstances);

    RWRETURN(RWRESOURCESGLOBAL(res.maxSize));
}

/**
 * \ingroup rwresources
 * \ref RwResourcesGetArenaUsage is used to calculate the total amount
 * of instance arena memory used in the last rendering frame. The number
 * returned may be larger than the arena size if the arena has started
 * thrashing: this will cause performance to degrade and the application
 * may wish to resize the arena to prevent this.
 *
 * Note that the value returned by this function is only meaningful before a
 * call to \ref RwCameraShowRaster (which purges the resources cache).
 *
 * \return Returns a \ref RwInt32 value equal to the size of the resources
 *         arena (in bytes)
 *
 * \see RwResourcesEmptyArena
 * \see RwResourcesGetArenaSize
 * \see RwResourcesSetArenaSize
 *
 */
RwInt32
RwResourcesGetArenaUsage(void)
{
    RwLLLink           *cur, *end;
    RwInt32             usage = RWRESOURCESGLOBAL(res.reusageSize);

    RWAPIFUNCTION(RWSTRING("RwResourcesGetArenaUsage"));
    RWASSERT(resourcesModule.numInstances);

    cur = rwLinkListGetFirstLLLink(RWRESOURCESGLOBAL(res.usedEntries));
    end = rwLinkListGetTerminator(RWRESOURCESGLOBAL(res.usedEntries));
    while (cur != end)
    {
        /* Count them all */
        RwResEntry         *entry =
            rwLLLinkGetData(cur, RwResEntry, link);

        /* Tot up sizes */
        usage += entry->size;

        /* Get next */
        cur = rwLLLinkGetNext(cur);
    }

    RWRETURN(usage);
}

/**
 * \ingroup rwresources
 * \ref RwResourcesEmptyArena is used to release all the memory in
 * the instancing arena. Any resources entries that have registered a
 * destroy notify callback will have their callback function executed
 * before the arena memory is freed.
 *
 * \return Returns TRUE.
 *
 * \see RwResourcesAllocateResEntry
 * \see RwResourcesGetArenaSize
 * \see RwResourcesGetArenaUsage
 * \see RwResourcesSetArenaSize
 *
 */
RwBool
RwResourcesEmptyArena(void)
{
    RwLLLink           *cur, *end;

    RWAPIFUNCTION(RWSTRING("RwResourcesEmptyArena"));
    RWASSERT(resourcesModule.numInstances);

    /* Attach list B onto list A */
    rwLinkListGetLastLLLink(&RWRESOURCESGLOBAL(res.entriesA))->next =
        rwLinkListGetFirstLLLink(&RWRESOURCESGLOBAL(res.entriesB));

    cur = rwLinkListGetFirstLLLink(&RWRESOURCESGLOBAL(res.entriesA));
    end = rwLinkListGetTerminator(&RWRESOURCESGLOBAL(res.entriesB));
    while (cur != end)
    {
        /* Remove them all ! */
        RwResEntry         *entry =
            rwLLLinkGetData(cur, RwResEntry, link);

        /* Next (must come before the resource release) */
        cur = rwLLLinkGetNext(cur);

        /* Deallocate the resources */
        RwResourcesFreeResEntry(entry);
    }

    /* Empty the lists */
    rwLinkListInitialize(&RWRESOURCESGLOBAL(res.entriesA));
    rwLinkListInitialize(&RWRESOURCESGLOBAL(res.entriesB));

    /* reset the usage counter */
    RWRESOURCESGLOBAL(res.reusageSize) = 0;

    /* Should all be empty */
    RWASSERT(RWRESOURCESGLOBAL(res.currentSize) == 0);

    /* Done */
    RWRETURN(TRUE);
}
