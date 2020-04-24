/*
 * Handling binary core module representations.
 * Core modules provide a very basic RenderWare implementation.  Thus module
 * enhances that implmentation to permit serialising of core objects to binary
 * streams.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"
#include "bastream.h"
#include "babinary.h"
#include "batkreg.h"

#include "batkbin.h"


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

   Plugin Binary Data Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwPluginRegistryAddPluginStream

 On entry   : Registry to add to
            : Plugin ID (32 bit number)
            : read, write and getsize stream callbacks
 On exit    : Offset into structure of reserved memory
 */

RwInt32
_rwPluginRegistryAddPluginStream(RwPluginRegistry * reg, RwUInt32 pluginID,
                                RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryAddPluginStream"));
    RWASSERT(reg);

    /* Search for pluginID in the registry list */
    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            break;
        }
        entry = entry->nextRegEntry;
    }

    if (entry)
    {
        /* Fill in the binary stream stuff */

        entry->readCB = readCB;
        entry->writeCB = writeCB;
        entry->getSizeCB = getSizeCB;

        RWRETURN(entry->offset);
    }

    /* Not in the list */
    RWRETURN(-1);
}

/****************************************************************************
 _rwPluginRegistryAddPlgnStrmlwysCB

 On entry   : Registry to add to
            : Plugin ID (32 bit number)
            : always stream callback
 On exit    : Offset into structure of reserved memory
 */

RwInt32
_rwPluginRegistryAddPlgnStrmlwysCB(
    RwPluginRegistry * reg,
    RwUInt32 pluginID,
    RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryAddPlgnStrmlwysCB"));
    RWASSERT(reg);

    /* Search for pluginID in the registry list */
    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            break;
        }
        entry = entry->nextRegEntry;
    }

    if (entry)
    {
        /* Fill in the binary stream stuff */

        /* For now, we're filling the alwaysCB or the others, not both.
         * Not sure what the deal is with NULL callbacks, so I didn't
         * combine this with _rwPluginRegistryAddPluginStream using NULL
         * to mean "callback not present" */
        entry->alwaysCB = alwaysCB;

        RWRETURN(entry->offset);
    }

    /* Not in the list */
    RWRETURN(-1);
}

/****************************************************************************
 _rwPluginRegistryAddPlgnStrmRightsCB

 On entry   : Registry to add to
            : Plugin ID (32 bit number)
            : rights stream callback
 On exit    : Offset into structure of reserved memory
 */
RwInt32
_rwPluginRegistryAddPlgnStrmRightsCB(RwPluginRegistry *reg, RwUInt32 pluginID,
                                     RwPluginDataChunkRightsCallBack rightsCB)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryAddPlgnStrmRightsCB"));
    RWASSERT(reg);

    /* Search for pluginID in the registry list */
    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            break;
        }
        entry = entry->nextRegEntry;
    }

    if (entry)
    {

        entry->rightsCB = rightsCB;

        RWRETURN(entry->offset);
    }

    /* Not in the list */
    RWRETURN(-1);
}
/****************************************************************************
 rwPluginRegistryReadDataChunk

 On entry   : Registry to read data chunk for
            : Stream to read data chunk from
            : Pointer to object into which to read data
 On exit    : Registry pointer
 */

const RwPluginRegistry *
_rwPluginRegistryReadDataChunks(const RwPluginRegistry * reg, RwStream * stream, void *object)
{
    RwUInt32 length, version;

    RWFUNCTION(RWSTRING("_rwPluginRegistryReadDataChunks"));
    RWASSERT(reg);
    RWASSERT(stream);
    RWASSERT(object);

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, object))
    {
        RWMESSAGE((RWSTRING("Object plugin memory is corrupt before reading binary data")));
    }
#endif /* RWDEBUG */

    /* Find the extension chunk and get its length */
    if (!RwStreamFindChunk(stream, rwID_EXTENSION, &length, &version))
    {
        RWRETURN((const RwPluginRegistry *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        RwPluginRegEntry *entry;
        /* Read a chunk from the stream.  If we can identify it as one for a registered
         * plugin, then allow the plugin to read it, else skip it
         */
        while (length > 0)
        {
            /* Read a chunk header */
            RwUInt32 readType, readLength;

            if (!_rwStreamReadChunkHeader(stream, &readType, &readLength,
                    (RwUInt32 *) NULL, (RwUInt32 *) NULL))
            {
                RWRETURN((const RwPluginRegistry *)NULL);
            }

            /* Search for chunkID in the registry list */
            entry = reg->firstRegEntry;
            while (entry)
            {
                if (entry->pluginID == readType)
                {
                    break;
                }
                entry = entry->nextRegEntry;
            }

            if (entry && entry->readCB)
            {
                if (!entry->readCB(stream, readLength, object, entry->offset, entry->size))
                {
                    /* Failed to read, return fail */
                    RWRETURN((const RwPluginRegistry *)NULL);
                }
            }
            else
            {
#ifdef RWDEBUG
                {
                    RwChar      tmpStr[128];
                    RwUInt32    venderID, objectID;

                    venderID = GETVENDORID(readType);
                    objectID = GETOBJECTID(readType);

                    rwsprintf(tmpStr,
                         "Plugin [Vendor ID 0x%x, Object ID 0x%x] "
                         "data found but not attached. "
                         "Data will be skipped and not read. ", venderID, objectID);

                    RWMESSAGE(RWSTRING((tmpStr)));
                }

#endif /* RWDEBUG */
                if (!RwStreamSkip(stream, readLength))
                {
                    RWRETURN((const RwPluginRegistry *)NULL);
                }
            }

            length -= (readLength + rwCHUNKHEADERSIZE);
        }

#ifdef RWDEBUG
        if (!_rwPluginRegistryValidateObject(reg, object))
        {
            RWMESSAGE((RWSTRING("Object plugin memory is corrupt after reading binary data")));
        }
#endif /* RWDEBUG */

        /* Call alwaysCBs for all the plugins (including those
         * poor ickle 'uns that didn't get any data to read) */
        entry = reg->firstRegEntry;
        while (entry)
        {
            if (entry->alwaysCB)
            {
                if (!entry->alwaysCB(object, entry->offset, entry->size))
                {
                    /* Failure, return fail */
                    RWRETURN((const RwPluginRegistry *)NULL);
                }
            }
            entry = entry->nextRegEntry;
        }

        RWRETURN(reg);
    }
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((const RwPluginRegistry *)NULL);
    }
}

/****************************************************************************
 _rwPluginRegistryInvokeRights

 On entry   : Registry to invoke the rights callback for
            : Plugin ID to select rights callback
            : Pointer to object
 On exit    : Registry pointer
 */

const RwPluginRegistry*
_rwPluginRegistryInvokeRights(const RwPluginRegistry *reg, RwUInt32 id,
                              void *obj, RwUInt32 extraData)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryInvokeRights"));
    RWASSERT(reg);

    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->pluginID == id)
        {
            break;
        }
        entry = entry->nextRegEntry;
    }
    if ((entry) && (entry->rightsCB))
    {
        if (!entry->rightsCB(obj, entry->offset, entry->size, extraData))
        {
            RWRETURN((const RwPluginRegistry *)NULL);
        }
        RWRETURN(reg);
    }
    else
    {
        RWRETURN((const RwPluginRegistry *)NULL);
    }
}
/****************************************************************************
 _rwPluginRegistryGetSize

 On entry   : Registry to size up chunk for
            : Object to size up chunk within
 On exit    : Size of data chunks for all regsistered toolkits
 */

RwInt32
_rwPluginRegistryGetSize(const RwPluginRegistry * reg, const void *object)
{
    RwInt32 size = 0;
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryGetSize"));
    RWASSERT(reg);

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, object))
    {
        RWMESSAGE((RWSTRING("Object plugin memory is corrupt before calculating binary size")));
    }
#endif /* RWDEBUG */

    /* Go through the list of toolkits and call each one to get a size */
    entry = reg->firstRegEntry;
    while (entry)
    {
        RwInt32 thisSize;

        if (entry->getSizeCB)
        {
            thisSize = entry->getSizeCB(object, entry->offset, entry->size);

            /* If we get a negative size, we'll skip this plugin - it's obviously just stupid */
            if (thisSize > 0)
            {
                /* Add it on, and don't forget the header which isn't included in the size */
                size += (thisSize + rwCHUNKHEADERSIZE);
            }
        }

        entry = entry->nextRegEntry;
    }

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, object))
    {
        RWMESSAGE((RWSTRING("Object plugin memory is corrupt after calculating binary size")));
    }
#endif /* RWDEBUG */

    RWRETURN(size);
}

/****************************************************************************
 _rwPluginRegistryWriteDataChunks

 On entry   : Registry to write data chunks for
            : Stream to write data chunks out to
            : Object to size up chunk within
 On exit    : Size of data chunk for all regsistered toolkits
 */

const RwPluginRegistry *
_rwPluginRegistryWriteDataChunks(const RwPluginRegistry * reg, RwStream * stream, const void *object)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryWriteDataChunks"));
    RWASSERT(reg);
    RWASSERT(stream);

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, object))
    {
        RWMESSAGE((RWSTRING("Object plugin memory is corrupt before writing binary data")));
    }
#endif /* RWDEBUG */

    /* Extension data chunk header */
    if (!RwStreamWriteChunkHeader(stream, rwID_EXTENSION, _rwPluginRegistryGetSize(reg, object)))
    {
        RWRETURN((const RwPluginRegistry *)NULL);
    }

    /* Go though the list of toolkits and call each one to write out its data */
    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->getSizeCB && entry->writeCB)
        {
            RwInt32 size = entry->getSizeCB(object, entry->offset, entry->size);

            /* If we get a negative size, we'll skip this plugin - it's obviously just stupid */
            if (size > 0)
            {
                /* Write out a header using the size */
                if (!RwStreamWriteChunkHeader(stream, entry->pluginID, size))
                {
                    RWRETURN((const RwPluginRegistry *)NULL);
                }

                /* Then write out the data block */
                if (!entry->writeCB(stream, size, object, entry->offset, entry->size))
                {
                    RWRETURN((const RwPluginRegistry *)NULL);
                }
            }
        }

        entry = entry->nextRegEntry;
    }

#ifdef RWDEBUG
    if (!_rwPluginRegistryValidateObject(reg, object))
    {
        RWMESSAGE((RWSTRING("Object plugin memory is corrupt after writing binary data")));
    }
#endif /* RWDEBUG */

    /* All done */
    RWRETURN(reg);
}

/****************************************************************************
 _rwPluginRegistrySkipDataChunks

 On entry   : Registry to skip data chunk for
            : Stream to read data chunk from
            : Pointer to object into which to read data
 On exit    : Registry pointer
 */

const RwPluginRegistry *
_rwPluginRegistrySkipDataChunks(const RwPluginRegistry * reg, RwStream * stream)
{
    RwUInt32 length;

    RWFUNCTION(RWSTRING("_rwPluginRegistrySkipDataChunks"));
    RWASSERT(reg);
    RWASSERT(stream);

    /* Since we are skipping, and not interpretting these chunks, we don't
     * care about version numbers
     */
    /* Find the extension chunk and get its length */
    if (!RwStreamFindChunk(stream, rwID_EXTENSION, &length,
                           (RwUInt32 *)NULL))
    {
        RWRETURN((const RwPluginRegistry *)NULL);
    }

    /* Read a chunk from the stream, and then skip it.  */
    while (length > 0)
    {
        /* Read a chunk header */
        RwUInt32 readLength;

        if (!_rwStreamReadChunkHeader(stream, (RwUInt32 *)NULL,
                                     &readLength, (RwUInt32 *)NULL,
                                     (RwUInt32 *)NULL))
        {
            RWRETURN((const RwPluginRegistry *)NULL);
        }

        if (!RwStreamSkip(stream, readLength))
        {
            RWRETURN((const RwPluginRegistry *)NULL);
        }

        length -= (readLength + rwCHUNKHEADERSIZE);
    }

    RWRETURN(reg);
}


