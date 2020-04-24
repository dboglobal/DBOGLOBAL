/***************************************************************************
 *                                                                         *
 * Module  : babincam.h                                                    *
 *                                                                         *
 * Purpose : Binary camera handling functions.                             *
 *                                                                         *
 **************************************************************************/

#ifndef RWBINCAM_H
#define RWBINCAM_H

/****************************************************************************
 Includes
 */

#include "bacamera.h"
#include "bastream.h"

/* A nice high point in the core to make sure these files are parsed by incgen
 * for rwcore.h (found by looking at a generated rwcore.h to see what the last
 * file was - no dependants)
 */
#include "drvmodel.h"
#include "drvfns.h"

/****************************************************************************
 Defines
 */

/* RWPUBLIC */
/****************************************************************************
 Global types
 */

/* Camera stream format */

/**
 * \ingroup rwcamera
 * \ref RwCameraChunkInfo is typedef'd to a structure that holds camera
 * data. This should be considered an opaque type. Use the RwCamera
 * API functions to access it.
 */

typedef struct rwStreamCamera RwCameraChunkInfo;
typedef struct rwStreamCamera rwStreamCamera;

#if (!defined(DOXYGEN))
struct rwStreamCamera
{
    RwV2d viewWindow;
    RwV2d viewOffset;
    RwReal nearPlane, farPlane;
    RwReal fogPlane;
    RwUInt32 projection;
};
#endif /* (!defined(DOXYGEN)) */

/* RWPUBLICEND */

/****************************************************************************
 Global Variables 
 */

/* RWPUBLIC */
/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Camera binary format */
extern RwInt32 RwCameraRegisterPluginStream(RwUInt32 pluginID,
                                            RwPluginDataChunkReadCallBack readCB,
                                            RwPluginDataChunkWriteCallBack writeCB,
                                            RwPluginDataChunkGetSizeCallBack getSizeCB);
extern RwInt32 RwCameraSetStreamAlwaysCallBack(
                   RwUInt32 pluginID,
                   RwPluginDataChunkAlwaysCallBack alwaysCB);
extern RwUInt32 RwCameraStreamGetSize(const RwCamera *camera);
extern RwCamera *RwCameraStreamRead(RwStream *stream);
extern const RwCamera *RwCameraStreamWrite(const RwCamera *camera,
                                           RwStream *stream);
extern RwCameraChunkInfo * RwCameraChunkInfoRead(RwStream *stream,
                                                 RwCameraChunkInfo *cameraChunkInfo,
                                                 RwInt32 *bytesRead);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */
#endif /* RWBINCAM_H */
