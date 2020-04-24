/*
 * Plugin registry
 * Plugin registries keep track of allocated memory in objects for toolkits
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

#if (!defined(RWKEEPFREELISTS))
#define RWKEEPFREELISTS
#endif  /* (!defined(RWKEEPFREELISTS)) */

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <stdio.h>

#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "badevice.h"
#include "bamemory.h"

#include "batkreg.h"


/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#ifdef RWDEBUG
#define rwTKMEMORYMAGIC 0x12345678
#endif /* RWDEBUG */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwFreeList *toolkitRegEntries, toolkitRegEntriesSpace;

static RwPluginRegistry **toolkitNonFLRegList = (RwPluginRegistry **)NULL;

static RwUInt32 numRegToolkits = 0;

/****************************************************************************
 Functions
 */

static RwInt32 _rwPluginRegFreeListBlockSize = 64,
               _rwPluginRegListPreallocBlocks = 1;

/**
 * \ingroup rwplugin
 * \page rwpluginoverview RwPlugin Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore
 *
 * \subsection pluginoverview Overview
 *
 * Plugin attachment API.
 *
 * RwPlugin provides an API to extend a base object with additional data
 * to provide extra functionality. All of RenderWare's plugins extends
 * their base object using this method.
 *
 * A base object is extended by registering a unique ID along with the custom
 * data size and a set of callbacks. For example, see \ref
 * RwCameraRegisterPlugin and \ref RwCameraRegisterPluginStream. This reserves
 * memory in the base object's memory block and attaches the callbacks to be
 * called for general object management, such as creation and destruction.
 * The callbacks and data size are optional. A plugin does not have to
 * reserve memory area in the base object's memory block or provide callbacks
 * to a particular function, such as streaming, if it is not required.
 *
 * All instances of the base object will have the custom data. The custom
 * data cannot vary in size between instances of the base object because it
 * is embedded within the base object's memory block. If the custom data can
 * vary in size, then this must be stored in an external memory block and
 * referenced via a pointer within the base object's memory block.
 *
 */

/****************************************************************************
 _rwPluginRegistryOpen

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rwPluginRegistryOpen(void)
{
    RWFUNCTION(RWSTRING("_rwPluginRegistryOpen"));

    toolkitRegEntries = RwFreeListCreateAndPreallocateSpace(sizeof(RwPluginRegEntry), _rwPluginRegFreeListBlockSize,
        sizeof(RwUInt32), _rwPluginRegListPreallocBlocks,
        &toolkitRegEntriesSpace, rwMEMHINTDUR_GLOBAL);
    if (!toolkitRegEntries)
    {
        /* Failed to create free list */
        RWRETURN(FALSE);
    }

    numRegToolkits = 0;
    RWASSERT( NULL == toolkitNonFLRegList );

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwDestroyEntry

 Destroy's freelist entry, and resets head of list to original size
 (so that we can shut and re-open the library)

 On entry   : object to destroy, freelist object is in
 On exit    : TRUE on success
 */

static void
rwDestroyEntry(void *object, void *freelist)
{
    RwPluginRegEntry *entry = (RwPluginRegEntry *) object;

    RWFUNCTION(RWSTRING("rwDestroyEntry"));

    /* Reset the size of the structure */
    if (entry->parentRegistry->firstRegEntry != NULL)
    {
        entry->parentRegistry->sizeOfStruct = entry->parentRegistry->origSizeOfStruct;
        entry->parentRegistry->firstRegEntry = (RwPluginRegEntry *)NULL;
        entry->parentRegistry->lastRegEntry = (RwPluginRegEntry *)NULL;
    }

    /* Free the entry up from the free list */
    RwFreeListFree((RwFreeList *) freelist, entry);

    RWRETURNVOID();
}


/****************************************************************************
 _rwPluginRegistryClose

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rwPluginRegistryClose(void)
{
    RWFUNCTION(RWSTRING("_rwPluginRegistryClose"));

    /* Shut down the list of plugin registries.  Also reset their sizes */
    if (toolkitRegEntries)
    {
        /* Destroy all entries in the free list */
        RwFreeListForAllUsed(toolkitRegEntries, rwDestroyEntry, toolkitRegEntries);

        /* ... making sure that all entries are catered for when
         * freelists are not used
         */
        if ( RWSRCGLOBAL(memoryAlloc) != _rwFreeListAllocReal )
        {
            RwUInt32    i;


            for ( i = 0; i < numRegToolkits; i += 1 )
            {
                RwPluginRegEntry    *entry;

                RwPluginRegistry    *parentReg;


                entry = toolkitNonFLRegList[i]->firstRegEntry;

                /* get the parent registry now, if it exists */
                parentReg = (RwPluginRegistry *)NULL;
                if ( NULL != entry )
                {
                    parentReg = entry->parentRegistry;
                }

                while ( NULL != entry )
                {
                    RwPluginRegEntry    *nextEntry;


                    nextEntry = entry->nextRegEntry;
                    /* free the entry up from the free list
                     * but since there isn't a freelist and this is just a wrapper
                     * onto RwFree, a NULL freelist pointer will work here */
                    RwFreeListFree( (RwFreeList *)NULL, entry );

                    entry = nextEntry;
                }

                /* reset the parent registry */
                if ( (NULL != parentReg) && (NULL != parentReg->firstRegEntry ) )
                {
                    parentReg->sizeOfStruct = parentReg->origSizeOfStruct;
                    parentReg->firstRegEntry = (RwPluginRegEntry *)NULL;
                    parentReg->lastRegEntry = (RwPluginRegEntry *)NULL;
                }
            }

            /* remove the registry list */
            if ( NULL != toolkitNonFLRegList )
            {
                RwFree( toolkitNonFLRegList );
                toolkitNonFLRegList = (RwPluginRegistry **)NULL;
            }
        }

        /* Then shut down the free list */
        RwFreeListDestroy(toolkitRegEntries);
        toolkitRegEntries = (RwFreeList *)NULL;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 PluginDefaultConstructor

 On entry   : All the usual - it's not too important since we do the dumb thing
 On exit    : stream pointer
 */

static void    *
PluginDefaultConstructor( void *object,
                          RwInt32 offsetInObject __RWUNUSED__,
                          RwInt32 sizeInObject __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("PluginDefaultConstructor"));

    /* Do nothing */
    RWRETURN(object);
}

/****************************************************************************
 PluginDefaultDestructor

 On entry   : All the usual - it's not too important since we do the dumb thing
 On exit    : stream pointer
 */

static void    *
PluginDefaultDestructor( void *object,
                         RwInt32 offsetInObject __RWUNUSED__,
                         RwInt32 sizeInObject __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("PluginDefaultDestructor"));

    /* Do nothing */
    RWRETURN(object);
}

/****************************************************************************
 PluginDefaultCopy

 On entry   : All the usual - it's not too important since we do the dumb thing
 On exit    : stream pointer
 */

static void    *
PluginDefaultCopy( void *dstObject,
                   const void * srcObject __RWUNUSED__,
                   RwInt32 offsetInObject __RWUNUSED__,
                   RwInt32 sizeInObject __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("PluginDefaultCopy"));

    /* Do nothing */
    RWRETURN(dstObject);
}

/****************************************************************************
 _rwPluginRegistrySetStaticPluginsSize

 On entry   : Registry defining toolkits registered with object
            : Size of static plugin data (User defined).
 On exit    : void
 */

RwBool
_rwPluginRegistrySetStaticPluginsSize(RwPluginRegistry * reg, RwInt32 size)
{
    RWFUNCTION(RWSTRING("_rwPluginRegistrySetStaticPluginsSize"));
    RWASSERT(reg);
    RWASSERT(size >= 0);

    if (reg->sizeOfStruct == reg->origSizeOfStruct)
    {
        if (reg->maxSizeOfStruct > 0)
        {
            if (size < reg->maxSizeOfStruct)
            {
                reg->maxSizeOfStruct = reg->sizeOfStruct + size;
            }
        }
        else
        {
            reg->maxSizeOfStruct = reg->sizeOfStruct + size;
        }

        reg->staticAlloc = 1;

        RWRETURN(1);
    }

    RWRETURN(0);
}


/****************************************************************************
 _rwPluginRegistryGetPluginOffset

 On entry   : Registry to serach for pluginID
            : Plugin ID to search for
 On exit    : Offset if plugin is found, or -1 if not found
 */

RwBool
_rwPluginRegistryGetPluginOffset(const RwPluginRegistry * reg,
                                 RwUInt32 pluginID)
{
    const RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryGetPluginOffset"));
    RWASSERT(reg);

    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            RWRETURN(entry->offset);
        }

        entry = entry->nextRegEntry;
    }

    RWRETURN(-1);
}

/****************************************************************************
 _rwPluginRegistryAddPlugin

 On entry   : Registry to add to
            : Size of block to reserve,
            : Plugin ID (32 bit number)
            : constructor, destructor and copy callbacks
 On exit    : Offset into structure of reserved memory
 */

RwInt32
_rwPluginRegistryAddPlugin(RwPluginRegistry * reg,
                           RwInt32 size, RwUInt32 pluginID,
                           RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB,
                           RwPluginObjectCopy copyCB)
{
    RwPluginRegEntry *entry;
    RwInt32 newStructSize;

    RWFUNCTION(RWSTRING("_rwPluginRegistryAddPlugin"));
    RWASSERT(reg);
    RWASSERT(toolkitRegEntries);

    if (!toolkitRegEntries)
    {
        /* Not a good time to do this - there is no free list, we can
         * infer from this that the library has not been initialised.
         */
        RWRETURN(-1);
    }

    /* Make sure that the library has not been opened yet */
    RWASSERT(_rwGetNumEngineInstances() == 0);
    if (_rwGetNumEngineInstances() != 0)
    {
        /* Not good, the engine has been opened */
        RWERROR((E_RW_PLUGININIT));
        RWRETURN(-1);
    }

    /* add the registry to the list if freelists aren't used */
    if ( RWSRCGLOBAL(memoryAlloc) != _rwFreeListAllocReal )
    {
        RwUInt32    i;


        for ( i = 0; i < numRegToolkits; i += 1 )
        {
            RWASSERT( NULL != toolkitNonFLRegList );

            if ( reg == toolkitNonFLRegList[i] )
            {
                break;
            }
        }

        if ( numRegToolkits == i )
        {
            RwPluginRegistry    **newRegistryList;

            RwUInt32            j;


            newRegistryList = (RwPluginRegistry **)RwMalloc(sizeof(RwPluginRegistry *) * (numRegToolkits + 1),
                                                            rwMEMHINTDUR_GLOBAL );

            j = 0;
            if ( NULL != toolkitNonFLRegList )
            {
                for ( ; j < numRegToolkits; j += 1 )
                {
                    newRegistryList[j] = toolkitNonFLRegList[j];
                }

                RwFree( toolkitNonFLRegList );
                toolkitNonFLRegList = (RwPluginRegistry **)NULL;
            }

            newRegistryList[j] = reg;

            numRegToolkits += 1;

            toolkitNonFLRegList = newRegistryList;
        }
    }

    /* Make sure it's not in the list yet */
    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            /* Can't register twice, return the already registered offset */

#ifdef RWDEBUG
            {
                RwChar  msg[80];

                sprintf(msg, "Plugin ,Vendor ID = 0x%02x, Object ID = 0x%02x, already registered.\n",
                        GETVENDORID(pluginID), GETOBJECTID(pluginID));

                RWMESSAGE(("%s", msg));
            }
#endif /* RWDEBUG */

            RWERROR((E_RW_PLUGININIT));
            RWRETURN(entry->offset);
        }
        entry = entry->nextRegEntry;
    }

    /* Increase structure size, but keep longword alignment */
#ifdef RWDEBUG
    /* Allow for a longword front and back for stomp detection */
    newStructSize = reg->sizeOfStruct + ((size + (sizeof (RwUInt32) * 2) + 3) & ~3);
#else /* RWDEBUG */
    newStructSize = reg->sizeOfStruct + ((size + 3) & ~3);
#endif /* RWDEBUG */

    if (reg->maxSizeOfStruct && (newStructSize > reg->maxSizeOfStruct))
    {
        /* We've exceeded our limit, so fail */
        RWRETURN(-1);
    }

    entry = (RwPluginRegEntry *) RwFreeListAlloc(toolkitRegEntries, rwMEMHINTDUR_GLOBAL);
    RWASSERT(RWFREELISTALIGNED(entry, toolkitRegEntries));

    if (entry)
    {
        /* Fill it the entry */

#ifdef RWDEBUG
        /* Allow for a longword front and back for stomp detection */
        entry->offset = reg->sizeOfStruct + sizeof (RwUInt32);
#else /* RWDEBUG */
        entry->offset = reg->sizeOfStruct;
#endif /* RWDEBUG */

        reg->sizeOfStruct = newStructSize;

        entry->size = size;
        entry->pluginID = pluginID;
        entry->readCB = (RwPluginDataChunkReadCallBack) NULL;
        entry->writeCB = (RwPluginDataChunkWriteCallBack) NULL;
        entry->getSizeCB = (RwPluginDataChunkGetSizeCallBack) NULL;
        entry->alwaysCB = (RwPluginDataChunkAlwaysCallBack) NULL;
        entry->rightsCB = (RwPluginDataChunkRightsCallBack) NULL;
        entry->constructCB = constructCB ? constructCB : PluginDefaultConstructor;
        entry->destructCB = destructCB ? destructCB : PluginDefaultDestructor;
        entry->copyCB = copyCB ? copyCB : PluginDefaultCopy;
        entry->errStrCB = (RwPluginErrorStrCallBack) NULL;
        entry->nextRegEntry = (RwPluginRegEntry *) NULL;
        entry->prevRegEntry = (RwPluginRegEntry *) NULL;

        /* Point back to the parent */
        entry->parentRegistry = reg;

        /* Append it to the list */
        if (reg->firstRegEntry == NULL)
        {
            /* First in the list, just shove it in */
            reg->firstRegEntry = entry;
            reg->lastRegEntry = entry;
        }
        else
        {
            /* Whack it on the end */
            reg->lastRegEntry->nextRegEntry = entry;
            entry->prevRegEntry = reg->lastRegEntry;

            /* New end of the list */
            reg->lastRegEntry = entry;
        }

        RWRETURN(entry->offset);
    }

    /* Failed to allocate a freelist entry */
    RWRETURN(-1);
}

#ifdef RWDEBUG

/****************************************************************************
 _rwPluginRegistryPlaceValidationCodes

 On entry   : Registry defining toolkits registered with object
            : Object to place validation codes in
 On exit    :
 */

void
_rwPluginRegistryPlaceValidationCodes(const RwPluginRegistry * reg, void *object)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryPlaceValidationCodes"));
    RWASSERT(reg);
    RWASSERT(object);

    entry = reg->firstRegEntry;
    while (entry)
    {
        RwUInt32        frontOfMemoryOffset = entry->offset - 4;
        RwUInt32        backOfMemoryOffset = (entry->offset +
                                              entry->size + 3) & ~3;
        RwUInt32       *frontOfMemory = (RwUInt32 *) (((RwUInt8 *)
                                                       object) + frontOfMemoryOffset);
        RwUInt32       *backOfMemory = (RwUInt32 *) (((RwUInt8 *)
                                                      object) + backOfMemoryOffset);

        *frontOfMemory = rwTKMEMORYMAGIC;
        *backOfMemory = rwTKMEMORYMAGIC;

        entry = entry->nextRegEntry;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwPluginRegistryValidateObject

 On entry   : Registry defining toolkits registered with object
            : Object to validate plugin memory within
 On exit    : TRUE if data is valid
 */

RwBool
_rwPluginRegistryValidateObject(const RwPluginRegistry * reg, const void *object)
{
    RwPluginRegEntry *entry;
    RwBool          valid;

    RWFUNCTION(RWSTRING("_rwPluginRegistryValidateObject"));
    RWASSERT(reg);
    RWASSERT(object);

    entry = reg->firstRegEntry;
    valid = TRUE;
    while (entry)
    {
        RwUInt32        frontOfMemoryOffset = entry->offset - 4;
        RwUInt32        backOfMemoryOffset =
            (entry->offset + entry->size + 3) & ~3;
        const RwUInt32 *frontOfMemory =
            (const RwUInt32 *) (((const RwUInt8 *) object) +frontOfMemoryOffset);
        const RwUInt32 *backOfMemory =
            (const RwUInt32 *) (((const RwUInt8 *) object) + backOfMemoryOffset);

        if ((*frontOfMemory != rwTKMEMORYMAGIC) || (*backOfMemory != rwTKMEMORYMAGIC))
        {
            valid = FALSE;
        }

        entry = entry->nextRegEntry;
    }

    RWRETURN(valid);
}

#endif /* RWDEBUG */

/****************************************************************************
 _rwPluginRegistryInitObject

 On entry   : Registry defining toolkits registered with object
            : Object to call constructors for
 On exit    : Registry pointer on success
 */

const RwPluginRegistry *
_rwPluginRegistryInitObject(const RwPluginRegistry * reg, void *object)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryInitObject"));
    RWASSERT(reg);
    RWASSERT(object);

#ifdef RWDEBUG
    _rwPluginRegistryPlaceValidationCodes(reg, object);
#endif /* RWDEBUG */

    /* Run through the list forwards */
    entry = reg->firstRegEntry;
    while (entry)
    {
        if (!entry->constructCB(object, entry->offset, entry->size))
        {
            /* Failed, go back along the constructor list */
            entry = entry->prevRegEntry;

            while (entry)
            {
                entry->destructCB(object, entry->offset, entry->size);
                entry = entry->prevRegEntry;
            }

            RWRETURN((const RwPluginRegistry *)NULL);
        }

        entry = entry->nextRegEntry;
    }

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, object))
    {

        RWMESSAGE((RWSTRING("Object plugin memory has become corrupt during construction")));
    }
#endif /* RWDEBUG */

    RWRETURN(reg);
}

/****************************************************************************
 _rwPluginRegistryDeInitObject

 On entry   : Registry defining toolkits registered with object
            : Object to call destructors for
 On exit    : Registry pointer on success
 */

const RwPluginRegistry *
_rwPluginRegistryDeInitObject(const RwPluginRegistry * reg, void *object)
{
    RwPluginRegEntry *entry;
    RWFUNCTION(RWSTRING("_rwPluginRegistryDeInitObject"));

    /* RWNOTIFY("_rwPluginRegistryDeInitObject", " .. called"); */
    RWASSERT(reg);
    RWASSERT(object);

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, object))
    {
        RWMESSAGE((RWSTRING("Object plugin memory is corrupt at destruction")));
    }
#endif /* RWDEBUG */

    /* Run through the list backwards */
    for (entry = reg->lastRegEntry;
         entry;
         entry = entry->prevRegEntry)
    {
        const RwPluginObjectDestructor  destructCB =  entry->destructCB;
        const RwInt32 offsetInObject = entry->offset;
        const RwInt32 sizeInObject = entry->size;

        RWASSERT(destructCB);
        RWASSERT(offsetInObject>=0);
        RWASSERT(sizeInObject>=0);

        destructCB(object, offsetInObject, sizeInObject);

    }

    /* RWNOTIFY("_rwPluginRegistryDeInitObject", " .. got here"); */
    RWRETURN(reg);
}

/****************************************************************************
 _rwPluginRegistryCopyObject

 On entry   : Registry defining toolkits registered with object
            : Object to copy to
            : Object to copy from
 On exit    : Registry pointer on success
 */

const RwPluginRegistry *
_rwPluginRegistryCopyObject(const RwPluginRegistry * reg, void
                            *dstObject, const void *srcObject)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryCopyObject"));
    RWASSERT(reg);
    RWASSERT(dstObject);
    RWASSERT(srcObject);

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, srcObject))
    {
        RWMESSAGE((RWSTRING("Source object plugin memory is corrupt at copy")));
    }
    if (!_rwPluginRegistryValidateObject(reg, dstObject))
    {
        RWMESSAGE((RWSTRING("Destination object plugin memory is corrupt at copy")));
    }
#endif /* RWDEBUG */

    /* Run through the list forwards */
    entry = reg->firstRegEntry;
    while (entry)
    {
        entry->copyCB(dstObject, srcObject, entry->offset, entry->size);
        entry = entry->nextRegEntry;
    }

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, srcObject))
    {
        RWMESSAGE((RWSTRING("Source object plugin memory has become corrupt during copy")));
    }
    if (!_rwPluginRegistryValidateObject(reg, dstObject))
    {
        RWMESSAGE((RWSTRING("Destination object plugin memory has become corrupt during copy")));
    }
#endif /* RWDEBUG */

    RWRETURN(reg);
}

/**
 * \ingroup rwplugin
 * \ref RwPluginRegistrySetFreeListCreateParams allows the developer to specify
 * how many RwPluginRegEntry s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwPluginRegistrySetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
#if 0
    RWAPIFUNCTION(RWSTRING("RwPluginRegistrySetFreeListCreateParams"));
#endif
    _rwPluginRegFreeListBlockSize = blockSize;
    _rwPluginRegListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}
