/***************************************************************************
 *                                                                         *
 * Module  : batkbin.h                                                     *
 *                                                                         *
 * Purpose : Binary tk module handling functions.                          *
 *                                                                         *
 **************************************************************************/

#ifndef RWTKBIN_H
#define RWTKBIN_H

/****************************************************************************
 Includes
 */

#include "batkreg.h"
#include "bastream.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global types
 */

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

/* Plugin binary stream stuff */
extern RwInt32 _rwPluginRegistryAddPluginStream(
                   RwPluginRegistry *reg,
                   RwUInt32 pluginID,
                   RwPluginDataChunkReadCallBack readCB,
                   RwPluginDataChunkWriteCallBack writeCB,
                   RwPluginDataChunkGetSizeCallBack getSizeCB);
extern RwInt32 _rwPluginRegistryAddPlgnStrmlwysCB(
                   RwPluginRegistry *reg,
                   RwUInt32 pluginID,
                   RwPluginDataChunkAlwaysCallBack alwaysCB);
extern RwInt32 _rwPluginRegistryAddPlgnStrmRightsCB(
                   RwPluginRegistry *reg,
                   RwUInt32 pluginID,
                   RwPluginDataChunkRightsCallBack rightsCB);
extern const RwPluginRegistry *_rwPluginRegistryReadDataChunks(const RwPluginRegistry *reg,
                                                              RwStream *stream,
                                                              void *object);
extern const RwPluginRegistry *_rwPluginRegistryWriteDataChunks(const RwPluginRegistry *reg,
                                                               RwStream *stream,
                                                               const void *object);
extern const RwPluginRegistry *_rwPluginRegistrySkipDataChunks(const RwPluginRegistry * reg,
                                                              RwStream * stream);
extern RwInt32 _rwPluginRegistryGetSize(const RwPluginRegistry *reg, const void *object);
extern const RwPluginRegistry *_rwPluginRegistryInvokeRights(const RwPluginRegistry *reg,
                                                             RwUInt32 id,
                                                             void *obj,
                                                             RwUInt32 extraData);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* Compatibility macros */

#define rwPluginRegistryAddPluginStream(reg, pluginID, readCB, writeCB, getSizeCB) \
       _rwPluginRegistryAddPluginStream(reg, pluginID, readCB, writeCB, getSizeCB)
#define rwPluginRegistryAddPluginStreamAlwaysCB(reg, pluginID, alwaysCB) \
       _rwPluginRegistryAddPlgnStrmlwysCB(reg, pluginID, alwaysCB)
#define rwPluginRegistryReadDataChunks(reg, stream, object) \
       _rwPluginRegistryReadDataChunks(reg, stream, object)
#define rwPluginRegistryWriteDataChunks(reg, stream, object) \
       _rwPluginRegistryWriteDataChunks(reg, stream, object)
#define rwPluginRegistrySkipDataChunks(reg, stream) \
       _rwPluginRegistrySkipDataChunks(reg, stream)
#define rwPluginRegistryGetSize(reg, object) \
       _rwPluginRegistryGetSize(reg, object)


/* RWPUBLICEND */

#endif /* RWTKBIN_H */
