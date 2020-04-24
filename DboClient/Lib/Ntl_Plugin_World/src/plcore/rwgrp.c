/*
 *
 * Group space functionality.
 * Functions for manage group names for binary streams.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */


/**
 * \ingroup rwchunkgroup
 * \page rwchunkgroupoverview RwChunkGroup Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection rwchunkgrpoverview RwChunkGroup Overview
 *
 * \ref RwChunkGroup is used to provide a mean to identify and group data elements
 * within a stream. The basic RenderWare chunk header provides information on
 * the type, size and version of the data element. It does not provide
 * information on the use or purpose of the data.
 *
 * \ref RwChunkGroup provides a mean to attach identification to objects within the
 * stream without modification to the object's data. This allows the
 * application to sort and arrange the data accordingly after stream using
 * the \ref RwChunkGroup for identification. A \ref RwChunkGroup can be identified
 * by using a name string.
 *
 * \ref RwChunkGroup exists in pairs in a stream. Each group is prefixed by a
 * begin marker using rwID_CHUNKGROUPSTART and postfix by an end marker using
 * rwID_CHUNKGROUPEND. \ref RwChunkGroup data are stored at the begin marker. Any
 * RenderWare data chunks which belongs to a group are written between the
 * markers.
 *
 * \ref RwChunkGroup can exist inside another \ref RwChunkGroup, providing a
 * hierarchical arrangement if required.
 */


/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "babinary.h"
#include "batypes.h"
#include "balibtyp.h"
#include "bastream.h"
#include "osintf.h"
#include "bamemory.h"

#include "rwgrp.h"


/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */
#define RWCHUNKGROUPGLOBAL(var) (RWPLUGINOFFSET(rwChunkGroupGlobals, RwEngineInstance, chunkGroupModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */
static RwModuleInfo chunkGroupModule;

static RwInt32 _rwChunkGroupFListBlockSize = 16;
static RwInt32 _rwChunkGroupFListPreallocBlocks = 1;
static RwFreeList _rwChunkGroupFList;

/*
 * Open this sub module.
 */

void  *
_rwChunkGroupOpen(void *instance,
             RwInt32 __RWUNUSED__ offset ,
             RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwChunkGroupOpen"));
    RWASSERT(instance);

    /* Store the globals offset (same for all instances) */
    chunkGroupModule.globalsOffset = offset;

    /* Get on with the initialization */
    RWCHUNKGROUPGLOBAL(groupFList) =
        RwFreeListCreateAndPreallocateSpace(sizeof(RwChunkGroup), _rwChunkGroupFListBlockSize, sizeof(RwUInt32),
        _rwChunkGroupFListPreallocBlocks, &_rwChunkGroupFList, rwID_CHUNKGROUPMODULE | rwMEMHINTDUR_GLOBAL);
    if (!RWCHUNKGROUPGLOBAL(groupFList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* One more module instance */
    chunkGroupModule.numInstances++;

    RWRETURN(instance);
}


/*
 * Close this sub module.
 */
void           *
_rwChunkGroupClose(void *instance,
              RwInt32 __RWUNUSED__ offset,
              RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwChunkGroupClose"));
    RWASSERT(instance);

    if (RWCHUNKGROUPGLOBAL(groupFList))
    {
        RwFreeListDestroy(RWCHUNKGROUPGLOBAL(groupFList));
    }

    /* One less module instance */
    chunkGroupModule.numInstances--;

    /* All done */
    RWRETURN(instance);
}


/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupSetFreeListCreateParams allows the developer to specify
 * how many \ref RwChunkGroup s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 */
void
RwChunkGroupSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
#if 0
    RWAPIFUNCTION(RWSTRING("RwChunkGroupSetFreeListCreateParams"));
#endif
    _rwChunkGroupFListBlockSize = blockSize;
    _rwChunkGroupFListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupCreate is used to create an empty \ref RwChunkGroup.
 *
 * \return Pointer to a \ref RwChunkGroup on success, else NULL.
 *
 * \see RwChunkGroupDestroy
 */
RwChunkGroup *
RwChunkGroupCreate( void )
{
    RwChunkGroup         *grp;

    RWAPIFUNCTION(RWSTRING("RwChunkGroupCreate"));

    grp = (RwChunkGroup *)RwFreeListAlloc(RWCHUNKGROUPGLOBAL(groupFList),
                                 rwMEMHINTDUR_EVENT | rwID_CHUNKGROUPMODULE);

    if (grp)
    {
        memset(grp, 0, sizeof(RwChunkGroup));
    }

    RWRETURN (grp);
}

/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupDestroy is used to destroy a \ref RwChunkGroup.
 *
 * \param grp Pointer to \ref RwChunkGroup to be destroyed.
 *
 * \return TRUE
 *
 * \see RwChunkGroupCreate
 */
RwBool
RwChunkGroupDestroy( RwChunkGroup *grp )
{
    RWAPIFUNCTION(RWSTRING("RwChunkGroupDestroy"));

    if (grp)
    {
        RwFreeListFree(RWCHUNKGROUPGLOBAL(groupFList), grp);
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupBeginStreamRead is used to read the start of a name
 * group from the specified binary stream.
 *
 * Note that prior to this function call a binary rwID_CHUNKGROUPSTART chunk must be
 * found in the stream using the \ref RwStreamFindChunk API function.
 *
 * \ref RwChunkGroup always exist in rwID_CHUNKGROUPSTART / rwID_CHUNKGROUPEND pairs in a
 * stream. Thus a rwID_CHUNKGROUPEND marker must be read for each
 * rwID_CHUNKGROUPSTART marker.
 *
 * \param stream Pointer to \ref RwStream to read the group from.
 *
 * \return Pointer to a \ref RwChunkGroup on success, else NULL.
 *
 * The sequence to locate and read a \ref RwChunkGroup pair from a binary stream is
 * as follows:
 * \code

   RwStream *stream;
   RwChunkGroup *grp;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_CHUNKGROUPSTART, NULL, NULL) )
       {
           grp = RwChunkGroupBeginStreamRead(stream);
       }

       if( RwStreamFindChunk(stream, rwID_CHUNKGROUPEND, NULL, NULL) )
       {
           grp = RwChunkGroupEndStreamRead(grp, stream);
       }

       RwStreamClose(stream, NULL);
   }
  \endcode
 *
 * \see RwChunkGroupEndStreamRead
 * \see RwChunkGroupBeginStreamWrite
 * \see RwChunkGroupEndStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 */
RwChunkGroup *
RwChunkGroupBeginStreamRead( RwStream *stream )
{
    RwUInt32            ver;
    RwChunkGroup        *grp;

    RWAPIFUNCTION(RWSTRING("RwChunkGroupBeginStreamRead"));

    RWASSERT(stream);

    grp = RwChunkGroupCreate();

    if (grp)
    {
        /* Read the version stamp. */
        if (RwStreamReadInt32(stream, (RwInt32 *) &ver, sizeof(RwUInt32)) != stream)
        {
            RwChunkGroupDestroy(grp);
            RWRETURN(NULL);
        }

        /* Read the string. */
        if (_rwStringStreamFindAndRead(grp->name, stream) != grp->name)
        {
            RwChunkGroupDestroy(grp);
            RWRETURN(NULL);
        }
    }

    RWRETURN(grp);
}


/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupEndStreamRead is used to read the end of a name
 * group from the specified binary stream.
 *
 * Note that prior to this function call a binary rwID_CHUNKGROUPEND chunk must be
 * found in the stream using the \ref RwStreamFindChunk API function.
 *
 * \ref RwChunkGroup always exist in rwID_CHUNKGROUPSTART / rwID_CHUNKGROUPEND pairs in a
 * stream. Thus a rwID_CHUNKGROUPEND marker must be read for each
 * rwID_CHUNKGROUPSTART marker.
 *
 * \note This is a place holder function to match with \ref RwChunkGroupBeginStreamRead.
 * There are no \ref RwChunkGroup data to read after the chunk header rwID_CHUNKGROUPEND
 * so it can omitted for the present.
 *
 * \param grp Pointer to \ref RwChunkGroup to read the rwID_CHUNKGROUPEND data to.
 * \param stream Pointer to \ref RwStream to read the rwID_CHUNKGROUPEND data from.
 *
 * \return Pointer to a \ref RwChunkGroup on success, else NULL.
 *
 * The sequence to locate and read a \ref RwChunkGroup pair from a binary stream is
 * as follows:
 * \code

   RwStream *stream;
   RwChunkGroup *group;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_CHUNKGROUPSTART, NULL, NULL) )
       {
           group = RwChunkGroupBeginStreamRead(stream);
       }

       if( RwStreamFindChunk(stream, rwID_CHUNKGROUPEND, NULL, NULL) )
       {
           group = RwChunkGroupEndStreamRead(group, stream);
       }

       RwStreamClose(stream, NULL);
   }
  \endcode
 *
 * \see RwChunkGroupBeginStreamRead
 * \see RwChunkGroupBeginStreamWrite
 * \see RwChunkGroupEndStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 */
RwChunkGroup *
RwChunkGroupEndStreamRead(RwChunkGroup *grp, RwStream __RWUNUSED__ *stream)
{
    RWAPIFUNCTION(RWSTRING("RwChunkGroupEndStreamRead"));

    RWRETURN(grp);
}

/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupBeginStreamWrite is used to write the specified
 * \ref RwChunkGroup begin marker to the given binary stream.
 *
 * Note that the stream will have been opened prior to this function call.
 *
 * \ref RwChunkGroup always exist in rwID_CHUNKGROUPSTART / rwID_CHUNKGROUPEND pairs in a
 * stream. Thus a rwID_CHUNKGROUPEND marker must be written for each
 * rwID_CHUNKGROUPSTART marker.
 *
 * \param grp Pointer to \ref RwChunkGroup to write.
 * \param stream Pointer to \ref RwStream to write the \ref RwChunkGroup begin
 * maker to.
 *
 * \return Pointer to the \ref RwChunkGroup argument if successful or NULL if there
 *         is an error.
 *
 * \see RwChunkGroupStreamGetSize
 * \see RwChunkGroupBeginStreamRead
 * \see RwChunkGroupEndStreamRead
 * \see RwChunkGroupEndStreamWrite
 */
const RwChunkGroup *
RwChunkGroupBeginStreamWrite( const RwChunkGroup *grp, RwStream *stream )
{
    RwUInt32                size, ver;

    RWAPIFUNCTION(RWSTRING("RwChunkGroupBeginStreamWrite"));

    RWASSERT(stream);
    RWASSERT(grp);

    /* Check if there are data to write. */

    /* Query the size of the name. Should an error be raise for no name ? */
    size = RwChunkGroupStreamGetSize(grp);

    /* Write out the chunk header. */
    if (RwStreamWriteChunkHeader(stream, rwID_CHUNKGROUPSTART, size) != stream)
    {
        RWRETURN((RwChunkGroup *) NULL);
    }

    /* Write out the version. */
    ver = rwCHUNKGROUPVERSION;
    if (RwStreamWriteInt32(stream, (RwInt32 *) &ver, sizeof(RwUInt32)) != stream)
    {
        RWRETURN((RwChunkGroup *) NULL);
    }

    /* Write out the name string. */
    if (_rwStringStreamWrite(grp->name, stream) != grp->name)
    {
        RWRETURN((RwChunkGroup *) NULL);
    }

    RWRETURN(grp);
}

/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupEndStreamWrite is used to write the specified \ref RwChunkGroup
 * end maker to the given binary stream.
 *
 * Note that the stream will have been opened prior to this function call.
 *
 * \ref RwChunkGroup always exist in rwID_CHUNKGROUPSTART / rwID_CHUNKGROUPEND pairs in a
 * stream. Thus a rwID_CHUNKGROUPEND marker must be written for each
 * rwID_CHUNKGROUPSTART  marker.

 * \param grp Pointer to \ref RwChunkGroup to write.
 * \param stream Pointer to \ref RwStream to write the \ref RwChunkGroup end marker
 * to.
 *
 * \return Pointer to the \ref RwChunkGroup argument if successful or NULL if there
 *         is an error.
 *
 * \see RwChunkGroupStreamGetSize
 * \see RwChunkGroupBeginStreamRead
 * \see RwChunkGroupEndStreamRead
 * \see RwChunkGroupBeginStreamWrite
 */
const RwChunkGroup *
RwChunkGroupEndStreamWrite( const RwChunkGroup *grp, RwStream *stream)
{
    RWAPIFUNCTION(RWSTRING("RwChunkGroupEndStreamWrite"));

    RWASSERT(stream);

    /* Write out the chunk header. */
    if (RwStreamWriteChunkHeader(stream, rwID_CHUNKGROUPEND, 0) != stream)
    {
        RWRETURN((RwChunkGroup *) NULL);
    }

    RWRETURN(grp);
}

/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupStreamGetSize is used to determine the size in bytes of the
 * binary representation of the \ref RwChunkGroup. This is used in the binary
 * chunk header to indicate the size of the data chunk. The size does not
 * include the size of the chunk header. The \ref RwChunkGroup data is written
 * out with the group begin marker with chunk id rwID_CHUNKGROUPSTART.
 *
 * \param grp Pointer to the \ref RwChunkGroup whose binary size is
 * required.
 *
 * \return \ref RwUInt32 value equal to the chunk size, in bytes, of the
 * group.
 *
 * \see RwChunkGroupBeginStreamWrite
 * \see RwChunkGroupEndStreamWrite
 */
RwUInt32
RwChunkGroupStreamGetSize(const RwChunkGroup *grp)
{
    RwUInt32            size;

    RWAPIFUNCTION(RWSTRING("RwChunkGroupStreamGetSize"));
    RWASSERT(grp);

    size = 0;

    /* Version stamp. */
    size += sizeof(RwUInt32);

    /* Size of the string. */
    size += rwCHUNKHEADERSIZE + _rwStringStreamGetSize(grp->name);

    RWRETURN(size);
}

/**
 * \ingroup rwchunkgroup
 * \ref RwChunkGroupSetName is used to set the name of gourp to the specified
 * string. The string is copied internally and must not exceed rwCHUNKGROUPMAXNAME
 * in length. Setting an empty name will delete the existing name.
 *
 * \param grp Pointer to the \ref RwChunkGroup.
 * \param name Pointer to the string to set.
 *
 * \return Pointer to the \ref RwChunkGroup argument if successful or NULL if there
 *         is an error.
 *
 */
RwChunkGroup *
RwChunkGroupSetName( RwChunkGroup *grp, const RwChar * name)
{
    RwInt32     l;

    RWAPIFUNCTION(RWSTRING("RwChunkGroupSetName"));

    RWASSERT(grp);

    if (name)
    {
        /* Copy the string into the group. */
        l = rwstrlen(name);

        if (l >= rwCHUNKGROUPMAXNAME)
        {
#ifdef RWDEBUG
            {
                RwChar      msg[80];

                sprintf(msg, "Name must be less than %d characters.", rwCHUNKGROUPMAXNAME);

                RWMESSAGE((RWSTRING(msg)));
            }
#endif /* RWDEBUG */

            /* Error. String too long. */
            RWRETURN(NULL);
        }

        rwstrcpy(grp->name, name);
    }
    else
    {
        /* NULL string. Erase. */
        grp->name[0] = 0;
    }

    RWRETURN(grp);
}
