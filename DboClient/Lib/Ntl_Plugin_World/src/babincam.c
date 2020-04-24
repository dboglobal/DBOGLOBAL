/*
 * Handling binary camera representations.
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
#include "bavector.h"
#include "badebug.h"
#include "bamemory.h"

#include "bacamera.h"

#include "babinary.h"
#include "batkreg.h"
#include "batkbin.h"
#include "babinmtx.h"

#include "babincam.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RwMemRealToFloat32Macro(_x)     \
    (void)RwMemRealToFloat32(&(_x), sizeof((_x)))

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Camera Binary Format Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwcamera
 * \ref RwCameraRegisterPluginStream is used to add stream capability to
 * a previously registered plugin.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID.
 * \param readCB  Callback used when a chunk is read that is identified as being for
 *       this plugin.
 * \param writeCB  Callback used when a chunk should be written out for this plugin.
 * \param getSizeCB  Callback used to determine the binary stream size required for this
 *       plugin.
 *
 * \return Returns the byte offset within the camera of memory reserved
 * for this plugin or a negative value if there is an error.
 *
 * \see RwCameraSetStreamAlwaysCallBack
 * \see RwCameraValidatePlugins
 * \see RwCameraRegisterPlugin
 * \see RwCameraGetPluginOffset
 *
 */
RwInt32
RwCameraRegisterPluginStream(RwUInt32 pluginID,
                             RwPluginDataChunkReadCallBack readCB,
                             RwPluginDataChunkWriteCallBack writeCB,
                             RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwCameraRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    plug = _rwPluginRegistryAddPluginStream(&cameraTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraSetStreamAlwaysCallBack is used to associate a
 * binary stream functionality with a previously registered camera plugin.
 * This callback is called for all plugins after stream data reading has
 * completed.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID.
 * \param alwaysCB  Callback used when object base and plugin data reading is complete.
 *
 * \return Returns the byte offset within the camera of memory reserved
 * for this plugin or a negative value if there is an error.
 *
 * \see RwCameraRegisterPluginStream
 * \see RwCameraValidatePlugins
 * \see RwCameraRegisterPlugin
 * \see RwCameraGetPluginOffset
 *
 */
RwInt32
RwCameraSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                     RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwCameraSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    plug = _rwPluginRegistryAddPlgnStrmlwysCB(
               &cameraTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraStreamGetSize determines the size in bytes of the binary
 * representation of the specified camera object. This is used in the binary
 * chunk header to indicate the size of the chunk. The size does not include
 * the size of the chunk header.
 *
 * \param camera  Pointer to the camera for which the size is required.
 *
 * \return Returns the chunk size of the specified camera in bytes or zero
 * if there is an error.
 *
 * \see RwCameraStreamRead
 * \see RwCameraStreamWrite
 *
 */
RwUInt32
RwCameraStreamGetSize(const RwCamera *camera)
{
    RwUInt32            size;

    RWAPIFUNCTION(RWSTRING("RwCameraStreamGetSize"));
    RWASSERT(camera);

    /* First get the size of the camera without the extension chunks */
    size = sizeof(rwStreamCamera) + rwCHUNKHEADERSIZE;

    /* Then add the size of the extension chunks */
    size += _rwPluginRegistryGetSize(&cameraTKList,camera) + rwCHUNKHEADERSIZE;

    RWRETURN(size);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraStreamWrite writes the specified camera object to a binary
 * stream. Note that the stream will have been opened prior to this function
 * call.
 *
 * \param camera  Pointer to the camera to be written.
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the specified camera if successful or NULL if
 * there is an error.
 *
 * \see RwCameraStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 *
 */
const RwCamera *
RwCameraStreamWrite(const RwCamera *camera, RwStream *stream)
{
    rwStreamCamera      cam;
    const RwV2d        *v2d;
    RwCameraProjection  projection;

    RWAPIFUNCTION(RWSTRING("RwCameraStreamWrite"));
    RWASSERT(camera);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader(stream, rwID_CAMERA, (RwInt32)RwCameraStreamGetSize(camera)))
    {
        RWRETURN((const RwCamera *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, sizeof(rwStreamCamera)))
    {
        RWRETURN((const RwCamera *)NULL);
    }

    v2d = RwCameraGetViewWindow(camera);
    RwV2dAssign(&cam.viewWindow, v2d);

    v2d = RwCameraGetViewOffset(camera);
    RwV2dAssign(&cam.viewOffset, v2d);

    cam.nearPlane = RwCameraGetNearClipPlane(camera);
    cam.farPlane = RwCameraGetFarClipPlane(camera);
    cam.fogPlane = RwCameraGetFogDistance(camera);
    projection = RwCameraGetProjection(camera);
    cam.projection = (RwUInt32) projection;

    /* Convert to form for writing (its a structure full of reals,
     * except the projection) */
    RwMemRealToFloat32Macro(cam.viewWindow);
    RwMemRealToFloat32Macro(cam.viewOffset);
    RwMemRealToFloat32Macro(cam.nearPlane);
    RwMemRealToFloat32Macro(cam.farPlane);
    RwMemRealToFloat32Macro(cam.fogPlane);

    (void)RwMemLittleEndian32(&cam, sizeof(cam));

    /* Now it's in a binary format, we can write it */
    if (!RwStreamWrite(stream, &cam, sizeof(cam)))
    {
        RWRETURN((const RwCamera *)NULL);
    }

    /* And then the camera extension chunk */
    if (!_rwPluginRegistryWriteDataChunks(&cameraTKList, stream, camera))
    {
        /* Failed to write extension data */
        RWRETURN((const RwCamera *)NULL);
    }

    RWRETURN(camera);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraStreamRead reads a camera object from a binary stream.
 * Note that prior to this function call, a binary camera
 * chunk must be found in the stream using the \ref RwStreamFindChunk API
 * function.
 *
 * \param stream  Pointer to the stream the camera will be read from.
 *
 * \return Returns pointer to the camera if successful or NULL on error.
 *
 * The sequence to locate and read a camera from a binary stream connected
 * to a disk file is as follows:
 * \code
   RwStream *stream;
   RwCamera *newCamera;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if (stream)
   {
       if (RwStreamFindChunk(stream, rwID_CAMERA, NULL, NULL))
       {
           newCamera = RwCameraStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
   \endcode
 *
 * \see RwCameraStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 *
 */
RwCamera *
RwCameraStreamRead(RwStream *stream)
{
    RwCamera    *camera;
    rwStreamCamera   cam;
    RwUInt32    size, version;

    RWAPIFUNCTION(RWSTRING("RwCameraStreamRead"));
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RwCamera *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        /* This protects us against changing structure sizes */
        RWASSERT(size <= sizeof(cam));
        memset(&cam, 0, sizeof(cam));
        if (RwStreamRead(stream, &cam, size) != size)
        {
            RWRETURN((RwCamera *)NULL);
        }

        /* Unconvert it (it's a structure full of reals, except the projection) */
        (void)RwMemNative32(&cam, sizeof(cam));
        (void)RwMemFloat32ToReal(&cam.viewWindow, sizeof(cam.viewWindow));
        (void)RwMemFloat32ToReal(&cam.viewOffset, sizeof(cam.viewOffset));
        (void)RwMemFloat32ToReal(&cam.nearPlane, sizeof(cam.nearPlane));
        (void)RwMemFloat32ToReal(&cam.farPlane, sizeof(cam.farPlane));
        (void)RwMemFloat32ToReal(&cam.fogPlane, sizeof(cam.fogPlane));

        camera = RwCameraCreate();
        if (!camera)
        {
            RWRETURN((RwCamera *)NULL);
        }

        /* Read the extension chunks */
        if (!_rwPluginRegistryReadDataChunks(&cameraTKList, stream, camera))
        {
            RWRETURN((RwCamera *)NULL);
        }

        /* This'll take care of the Z transformation as elements are set */
        RwCameraSetViewWindow(camera, &cam.viewWindow);
        RwCameraSetViewOffset(camera, &cam.viewOffset);
        RwCameraSetNearClipPlane(camera, ((RwReal) cam.nearPlane));
        RwCameraSetFarClipPlane(camera, ((RwReal) cam.farPlane));
        RwCameraSetFogDistance(camera, ((RwReal) cam.fogPlane));
        RwCameraSetProjection(camera,(RwCameraProjection) cam.projection);

        RWRETURN(camera);
    }
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RwCamera *)NULL);
    }
}

/**
 * \ingroup rwcamera
 * \ref RwCameraChunkInfoRead extracts Chunk
 * Info data from a RenderWare stream. The data is converted from its
 * original format and inserted into an \ref RwCameraChunkInfo structure. A
 * pointer to this structure is returned on success.
 *
 * \param stream  Pointer to the stream object to be read.
 *
 * \param cameraChunkInfo  Pointer to the camera ChunkInfo structure to be
 * filled by the data.
 *
 * \param bytesRead  Pointer to an RwInt32 which will hold the number of bytes read from
 * the stream.
 *
 * \return Returns a pointer to the filled structure if successful. Returns
 * NULL if an error occurred.
 *
 * \see RwCameraStreamRead
 * \see RwCameraStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 *
 */
RwCameraChunkInfo *
RwCameraChunkInfoRead(RwStream *stream,
                      RwCameraChunkInfo *cameraChunkInfo,
                      RwInt32 *bytesRead)
{
    RwUInt32    size, readSize;

    RWAPIFUNCTION(RWSTRING("RwCameraChunkInfoRead"));
    RWASSERT(stream);
    RWASSERT(cameraChunkInfo);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                           &size, (RwUInt32 *)NULL))
    {
        RWRETURN((rwStreamCamera *)NULL);
    }

    /* This protects us against changing structure sizes */
    RWASSERT(size <= sizeof(RwCameraChunkInfo));
    readSize = sizeof(RwCameraChunkInfo);
    memset(cameraChunkInfo, 0, readSize);
    if (RwStreamRead(stream, cameraChunkInfo, readSize) != readSize)
    {
        RWRETURN((rwStreamCamera *)NULL);
    }

    *bytesRead = size + (sizeof(RwInt32) * 3);
    /* move on to known place */
    RwStreamSkip(stream, size - readSize);

    /* Unconvert it (it's a structure full of reals, except the projection) */
    (void)RwMemNative32(cameraChunkInfo, sizeof(RwCameraChunkInfo));
    (void)RwMemFloat32ToReal(&cameraChunkInfo->viewWindow,
                       sizeof(cameraChunkInfo->viewWindow));
    (void)RwMemFloat32ToReal(&cameraChunkInfo->viewOffset,
                       sizeof(cameraChunkInfo->viewOffset));
    (void)RwMemFloat32ToReal(&cameraChunkInfo->nearPlane,
                       sizeof(cameraChunkInfo->nearPlane));
    (void)RwMemFloat32ToReal(&cameraChunkInfo->farPlane,
                       sizeof(cameraChunkInfo->farPlane));
    (void)RwMemFloat32ToReal(&cameraChunkInfo->fogPlane,
                       sizeof(cameraChunkInfo->fogPlane));

    RWRETURN(cameraChunkInfo);
}

