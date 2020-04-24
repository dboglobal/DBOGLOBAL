/*
 * Handling binary frame representations.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"
#include "baimmedi.h"

#include "babinary.h"
#include "batkreg.h"
#include "batkbin.h"

#include "babinfrm.h"



#if defined (__MWERKS__)
#if (defined(RWVERBOSE))
#pragma message (__FILE__ "/" _SKY_EXPAND(__LINE__) ": __MWERKS__ == " _SKY_EXPAND(__MWERKS__))
#endif /* (defined (__MWERKS__)) */
#if (__option (global_optimizer))
#pragma always_inline off
#endif /* (__option (global_optimizer)) */
#endif /*  defined (__MWERKS__) */

/****************************************************************************
 Local Types
 */

/* Frame stream format */
typedef struct _rwStreamFrame rwStreamFrame;
struct _rwStreamFrame
{
    RwV3d               right, up, at, pos;
    RwInt32             parentIndex;
    RwUInt32            data;
};

typedef struct _rwStreamFrameList rwStreamFrameList;
struct _rwStreamFrameList
{
    RwInt32             numFrames;
};

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#ifndef _ORTHONORMAL_TOL
#define _ORTHONORMAL_TOL ((RwReal)0.01)
#endif /* _ORTHONORMAL_TOL */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */
static RwBool _rwFrameListDirtyListUpdate = TRUE;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Frame Binary Format Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwframe
 * \ref RwFrameListSetAutoUpdate flags whether RwFrameListStreamRead should add frames
 * to the dirtyList.  During Asyncronous reading this causes problems thus its better to
 * disable adding to dirtyList during loading, and force a frame update on completion.
 *
 * \return Returns nothing
 *
 * \see RwFrameSetStreamAlwaysCallBack
 * \see RwFrameRegisterPlugin
 * \see RwFrameGetPluginOffset
 * \see RwFrameValidatePlugins
 */

void
RwFrameListSetAutoUpdate(RwBool flag)
{
    RWAPIFUNCTION(RWSTRING("RwFrameListSetAutoUpdate"));

    _rwFrameListDirtyListUpdate = flag;

    RWRETURNVOID();
}

/**
 * \ingroup rwframe
 * \ref RwFrameRegisterPluginStream associates a set of binary
 * stream functionality with a previously registered plugin.
 *
 * \param pluginID  The 32 bit unique ID for the plugin (used to identify binary chunks)
 * \param readCB  The callback used when a chunk is read that is identified as being for
 *       this plugin.
 * \param writeCB  The callback used when a chunk should be written out for this plugin
 * \param getSizeCB  The callback used to determine the binary stream size required for this
 *       plugin (return negative to suppress chunk writing)
 *
 * \return Returns a value:
 * \li positive : the byte offset within the frame of memory reserved for this plugin
 * \li negative : on failure
 *
 * \see RwFrameSetStreamAlwaysCallBack
 * \see RwFrameRegisterPlugin
 * \see RwFrameGetPluginOffset
 * \see RwFrameValidatePlugins
 */

RwInt32
RwFrameRegisterPluginStream(RwUInt32 pluginID,
                            RwPluginDataChunkReadCallBack readCB,
                            RwPluginDataChunkWriteCallBack writeCB,
                            RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwFrameRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */

    plug = _rwPluginRegistryAddPluginStream(&frameTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwframe
 * \ref RwFrameSetStreamAlwaysCallBack associates a binary
 * stream callback with a previously registered frame plugin. This callback
 * is called for all plugins after stream data reading has completed.
 *
 * \param pluginID  The 32 bit unique ID for the plugin (used to identify binary chunks)
 * \param alwaysCB  The callback used when the object base and plugin data reading is complete.
 *
 * \return Returns an \ref RwInt32 equal to one of the following values:
 * \li positive - Byte offset within the frame of memory reserved for this plugin
 * \li negative - On failure
 *
 * \see RwFrameRegisterPluginStream
 * \see RwFrameRegisterPlugin
 * \see RwFrameGetPluginOffset
 * \see RwFrameValidatePlugins
 */

RwInt32
RwFrameSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                    RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwFrameSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */

    plug = _rwPluginRegistryAddPlgnStrmlwysCB(
               &frameTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/****************************************************************************
 _rwFrameListMakeBreadthFirst (recursive)

 On entry   : Pointer to position in list reached
            : Frame to add from
 On exit    : TRUE on success
 */

static RwBool
breadthFirst(RwFrame *** frameListPos, RwFrame * frame)
{
    RWFUNCTION(RWSTRING("breadthFirst"));
    RWASSERT(frameListPos);
    RWASSERT(*frameListPos);

    if (frame)
    {
        RWASSERTISTYPE(frame, rwFRAME);

        /* Put in current frame */
        *(*frameListPos) = frame;
        (*frameListPos)++;

        /* Process sibling first (breadth first) */
        breadthFirst(frameListPos, frame->next);

        /* Then process child */
        breadthFirst(frameListPos, frame->child);
    }

    RWRETURN(TRUE);
}

static RwBool
rwFrameListMakeBreadthFirst(RwFrame *** frameListPos, RwFrame * frame)
{
    RWFUNCTION(RWSTRING("rwFrameListMakeBreadthFirst"));
    RWASSERT(frameListPos);
    RWASSERT(*frameListPos);

    if (frame)
    {
        RWASSERTISTYPE(frame, rwFRAME);

        /* Put in current frame */
        *(*frameListPos) = frame;
        (*frameListPos)++;

        /* Only process children for root frame */
        breadthFirst(frameListPos, frame->child);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwFrameListInitialize

 On entry   : Frame list
            : Frame
 On exit    : Frame list pointer on success
 */

rwFrameList        *
_rwFrameListInitialize(rwFrameList * frameList, RwFrame * frame)
{
    RwInt32             numFrames;
    RwFrame           **fppFrames;
    RwFrame           **fppCur;

    RWFUNCTION(RWSTRING("_rwFrameListInitialize"));
    RWASSERT(frameList);
    RWASSERT(frame);
    RWASSERTISTYPE(frame, rwFRAME);

    numFrames = RwFrameCount(frame);

    fppFrames = (RwFrame **) RwMalloc(sizeof(RwFrame *) * numFrames,
                                      rwMEMHINTDUR_GLOBAL | rwID_FRAMELIST);
    if (!fppFrames)
    {
        RWERROR((E_RW_NOMEM, (sizeof(RwFrame *) * numFrames)));
        RWRETURN((rwFrameList *)NULL);
    }

    /* Breadth first scan adding children */
    fppCur = fppFrames;
    rwFrameListMakeBreadthFirst(&fppCur, frame);

    /* Save off the pointer to the array */
    frameList->numFrames = numFrames;
    frameList->frames = fppFrames;

    /* Done */
    RWRETURN(frameList);
}

/****************************************************************************
 _rwFrameListFindFrame

 On entry   : Array of frameList
            : Frame to look for
            : Index
 On exit    : TRUE if found
 */

RwBool
_rwFrameListFindFrame(const rwFrameList * frameList,
                      const RwFrame * frame, RwInt32 * npIndex)
{
    RwInt32             i;

    RWFUNCTION(RWSTRING("_rwFrameListFindFrame"));
    RWASSERT(frameList);
    RWASSERT(frame);
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(npIndex);

    for (i = 0; i < frameList->numFrames; i++)
    {
        if (frameList->frames[i] == frame)
        {
            (*npIndex) = i;
            RWRETURN(TRUE);
        }
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwFrameListDeinitialize

 On entry   : Frame list
 On exit    : Frame list on success
 */

rwFrameList        *
_rwFrameListDeinitialize(rwFrameList * frameList)
{
    RWFUNCTION(RWSTRING("_rwFrameListDeinitialize"));
    RWASSERT(frameList);

    if (frameList->numFrames)
    {
        RwFree(frameList->frames);
    }

    RWRETURN(frameList);
}

/****************************************************************************
 __rwFrameListStreamGetSizeActual

 On entry   :
 On exit    : Size of Binary FrameList
 */

static              RwUInt32
FrameListStreamGetSizeActual(const rwFrameList * frameList)
{
    RwUInt32            size;

    RWFUNCTION(RWSTRING("FrameListStreamGetSizeActual"));
    RWASSERT(frameList);

    size = sizeof(rwStreamFrameList);
    size += frameList->numFrames * sizeof(rwStreamFrame);

    RWRETURN(size);
}

/****************************************************************************
 _rwFrameListStreamGetSize

 On entry   :
 On exit    : Size of Binary FrameList
 */

RwUInt32
_rwFrameListStreamGetSize(const rwFrameList * frameList)
{
    RwUInt32            size;
    RwInt32             i;

    RWFUNCTION(RWSTRING("_rwFrameListStreamGetSize"));
    RWASSERT(frameList);

    /* Size of frame list itself */
    size = FrameListStreamGetSizeActual(frameList) + rwCHUNKHEADERSIZE;

    /* Add the size of the frame extension chunks for each frame in the list */
    for (i = 0; i < frameList->numFrames; i++)
    {
        RwFrame            *frame = frameList->frames[i];

        size += (_rwPluginRegistryGetSize(&frameTKList, frame) + rwCHUNKHEADERSIZE);
    }

    /* Return the size */
    RWRETURN(size);
}

/****************************************************************************
 _rwFrameListStreamWrite

 On entry   : Stream to write to
 On exit    :
 */
const rwFrameList  *
_rwFrameListStreamWrite(const rwFrameList * frameList, RwStream * stream)
{
    rwStreamFrameList   fl;
    RwInt32             i;

    RWFUNCTION(RWSTRING("_rwFrameListStreamWrite"));
    RWASSERT(frameList);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader
        (stream, rwID_FRAMELIST, _rwFrameListStreamGetSize(frameList)))
    {
        RWRETURN((const rwFrameList *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT,
                                  FrameListStreamGetSizeActual(frameList)))
    {
        RWRETURN((const rwFrameList *)NULL);
    }

    /* Fill it */
    fl.numFrames = frameList->numFrames;

    /* Convert it */
    (void)RwMemLittleEndian32(&fl, sizeof(fl));

    /* Write it */
    if (!RwStreamWrite(stream, &fl, sizeof(fl)))
    {
        RWRETURN((const rwFrameList *)NULL);
    }

    /* Now write off all the frames */
    for (i = 0; i < frameList->numFrames; i++)
    {
        rwStreamFrame      f;
        RwFrame            *frame = frameList->frames[i];
        RwFrame            *parent;
        RwMatrix           *mat = RwFrameGetMatrix(frame);

        /* Fill it */
        f.right = *RwMatrixGetRight(mat);
        f.up = *RwMatrixGetUp(mat);
        f.at = *RwMatrixGetAt(mat);
        f.pos = *RwMatrixGetPos(mat);

        /* Save off the parent too */
        parent = RwFrameGetParent(frame);
        if (parent)
        {
            if (!_rwFrameListFindFrame(frameList, parent, &f.parentIndex))
            {
                /* Parent frame is not being streamed out so we can't link
                   it up on stream read. */
                f.parentIndex = -1;
            }
        }
        else
        {
            f.parentIndex = -1;
        }

        /* Convert */
        (void)RwMemRealToFloat32(&f.right, sizeof(f.right));
        (void)RwMemRealToFloat32(&f.up, sizeof(f.up));
        (void)RwMemRealToFloat32(&f.at, sizeof(f.at));
        (void)RwMemRealToFloat32(&f.pos, sizeof(f.pos));
        f.data = rwMatrixGetFlags(mat);
        (void)RwMemLittleEndian32(&f, sizeof(f));

        /* Write */
        if (!RwStreamWrite(stream, &f, sizeof(f)))
        {
            RWRETURN((const rwFrameList *)NULL);
        }
    }

    /* Now write all the extension chunks (one for each frame) */
    for (i = 0; i < frameList->numFrames; i++)
    {
        RwFrame            *frame = frameList->frames[i];

        /* Frame extension data */
        if (!_rwPluginRegistryWriteDataChunks(&frameTKList, stream, frame))
        {
            /* Failed to write extension data */
            RWRETURN((const rwFrameList *)NULL);
        }
    }

    RWRETURN(frameList);
}

/****************************************************************************
 _rwFrameListStreamRead

 On entry   : Stream to read from
 On exit    : FrameList created
 */
rwFrameList        *
_rwFrameListStreamRead(RwStream * stream, rwFrameList * frameList)
{
    rwStreamFrameList   fl;
    RwInt32             i;
    RwUInt32            size;
    RwUInt32            version;

    RWFUNCTION(RWSTRING("_rwFrameListStreamRead"));
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((rwFrameList *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        /* Read it */
        if (RwStreamRead(stream, &fl, sizeof(fl)) != sizeof(fl))
        {
            RWRETURN((rwFrameList *)NULL);
        }

        /* Convert it */
        (void)RwMemNative32(&fl, sizeof(fl));

        /* a little dangerous  - should use ActualSize */
        RWASSERT(size == (sizeof(fl) + fl.numFrames * sizeof(rwStreamFrame)));

        /* Set up the frame list */
        frameList->numFrames = fl.numFrames;

        frameList->frames = (RwFrame **) RwMalloc(sizeof(RwFrame *) * fl.numFrames,
                                              rwMEMHINTDUR_EVENT | rwID_FRAMELIST);

        if (!frameList->frames)
        {
            RWERROR((E_RW_NOMEM, (sizeof(RwFrame *) * fl.numFrames)));
            RWRETURN((rwFrameList *)NULL);
        }

        for (i = 0; i < fl.numFrames; i++)
        {
            rwStreamFrame      f;
            RwFrame            *frame;
            RwMatrix           *mat;

            if (RwStreamRead(stream, &f, sizeof(f)) != sizeof(f))
            {
                RwFree(frameList->frames);
                RWRETURN((rwFrameList *)NULL);
            }
            (void)RwMemNative32(&f, sizeof(f));
            (void)RwMemFloat32ToReal(&f.right, sizeof(f.right));
            (void)RwMemFloat32ToReal(&f.up, sizeof(f.up));
            (void)RwMemFloat32ToReal(&f.at, sizeof(f.at));
            (void)RwMemFloat32ToReal(&f.pos, sizeof(f.pos));

            /* Create the frame */
            frame = RwFrameCreate();

            if (!frame)
            {
                RwFree(frameList->frames);
                RWRETURN((rwFrameList *)NULL);
            }

            mat = RwFrameGetMatrix(frame);
            *RwMatrixGetRight(mat) = f.right;
            *RwMatrixGetUp(mat) = f.up;
            *RwMatrixGetAt(mat) = f.at;
            *RwMatrixGetPos(mat) = f.pos;

            if (rwMatrixIsOrthonormalPositive(mat, _ORTHONORMAL_TOL))
            {
                rwMatrixSetFlags(mat,
                                 rwMatrixGetFlags(mat) &
                                 ~rwMATRIXINTERNALIDENTITY);
            }
            else
            {
                rwMatrixSetFlags(mat,
                                 rwMatrixGetFlags(mat) &
                                 ~(rwMATRIXINTERNALIDENTITY |
                                   rwMATRIXTYPEORTHONORMAL));
            }

            frameList->frames[i] = frame;

            /* Set the frame pointer */
            if (f.parentIndex >= 0)
            {
                if (_rwFrameListDirtyListUpdate)
                {
                    RwFrameAddChild(frameList->frames[f.parentIndex], frame);
                }
                else
                {
                    RwFrameAddChildNoUpdate(frameList->frames[f.parentIndex], frame);
                }
            }
        }
    }
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((rwFrameList *)NULL);
    }

    for (i = 0; i < fl.numFrames; i++)
    {
        RwFrame            *frame = frameList->frames[i];

        /* Frame extension data */
        if (!_rwPluginRegistryReadDataChunks(&frameTKList, stream, frame))
        {
            RwFrameDestroyHierarchy(frameList->frames[0]);
            RwFree(frameList->frames);
            RWRETURN((rwFrameList *)NULL);
        }
        
        if( (frame == RwFrameGetRoot(frame)) && 
            (TRUE == _rwFrameListDirtyListUpdate))
        {
            RwFrameUpdateObjects(frame);
        }   
    }

    RWRETURN(frameList);
}

#if defined (__MWERKS__)
#if (defined(RWVERBOSE))
#pragma message (__FILE__ "/" _SKY_EXPAND(__LINE__) ": __MWERKS__ == " _SKY_EXPAND(__MWERKS__))
#endif /* (defined (__MWERKS__)) */
#if (__option (global_optimizer))
#pragma always_inline on
#endif /* (__option (global_optimizer)) */
#endif /*  defined (__MWERKS__) */

