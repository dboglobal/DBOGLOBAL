/***************************************************************************
 *                                                                         *
 * Module  : babinfrm.h                                                    *
 *                                                                         *
 * Purpose : Binary frame handling functions.                              *
 *                                                                         *
 **************************************************************************/

#ifndef RWBINFRM_H
#define RWBINFRM_H

/****************************************************************************
 Includes
 */

#include "baframe.h"
#include "bastream.h"

/****************************************************************************
 Defines
 */

/* RWPUBLIC */
/****************************************************************************
 Global types
 */

typedef struct rwFrameList rwFrameList;

#if (!defined(DOXYGEN))
struct rwFrameList
{
    RwFrame **frames;
    RwInt32 numFrames;
};
#endif /* (!defined(DOXYGEN)) */

/****************************************************************************
 Global Variables 
 */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Frame binary format */

extern void
RwFrameListSetAutoUpdate(RwBool flag);


extern RwInt32 
RwFrameRegisterPluginStream(RwUInt32 pluginID,
                            RwPluginDataChunkReadCallBack readCB,
                            RwPluginDataChunkWriteCallBack writeCB,
                            RwPluginDataChunkGetSizeCallBack getSizeCB);

extern RwInt32 
RwFrameSetStreamAlwaysCallBack(RwUInt32 pluginID,
                               RwPluginDataChunkAlwaysCallBack alwaysCB);


extern rwFrameList *
_rwFrameListInitialize(rwFrameList *frameList,
                      RwFrame *frame);

extern RwBool 
_rwFrameListFindFrame(const rwFrameList *frameList,
                     const RwFrame *frame,
                     RwInt32 *npIndex);

extern rwFrameList *
_rwFrameListDeinitialize(rwFrameList *frameList);

extern RwUInt32 
_rwFrameListStreamGetSize(const rwFrameList *frameList);

extern rwFrameList *
_rwFrameListStreamRead(RwStream *stream,
                      rwFrameList *fl);

extern const rwFrameList *
_rwFrameListStreamWrite(const rwFrameList *frameList,
                       RwStream *stream);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* Comparibility macros */

#define rwFrameListInitialize(frameList,frame) \
       _rwFrameListInitialize(frameList,frame)

#define rwFrameListFindFrame(frameList, frame, index) \
       _rwFrameListFindFrame(frameList, frame, index)

#define rwFrameListDeinitialize(frameList) \
       _rwFrameListDeinitialize(frameList)

#define rwFrameListStreamGetSize(frameList) \
       _rwFrameListStreamGetSize(frameList)

#define rwFrameListStreamRead(stream, fl) \
       _rwFrameListStreamRead(stream, fl)

#define rwFrameListStreamWrite(frameList, stream) \
       _rwFrameListStreamWrite(frameList, stream)

/* RWPUBLICEND */

#endif /* RWBINFRM_H */
