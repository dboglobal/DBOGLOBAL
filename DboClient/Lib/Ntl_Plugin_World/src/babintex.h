/***************************************************************************
 *                                                                         *
 * Module  : babintex.h                                                    *
 *                                                                         *
 * Purpose : Binary texture handling functions.                            *
 *                                                                         *
 **************************************************************************/

#ifndef RWBINTEX_H
#define RWBINTEX_H

/****************************************************************************
 Includes
 */

#include "batextur.h"
#include "bastream.h"

/****************************************************************************
 Defines
 */

/* RWPUBLIC */
/****************************************************************************
 Global types
 */
typedef struct rpTextureChunkInfo RwTextureChunkInfo;

#if (!defined(DOXYGEN))
struct rpTextureChunkInfo
{
    RwTextureFilterMode filtering;
    RwTextureAddressMode addressingU;
    RwTextureAddressMode addressingV;
};
#endif /* (!defined(DOXYGEN)) */

/* Bit flags defining properties of textures when stream */
enum RwTextureStreamFlags
{
    rwNATEXTURESTREAMFLAG = 0x00,
    rwTEXTURESTREAMFLAGSUSERMIPMAPS = 0x01,
    rwTEXTURESTREAMFLAGSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwTextureStreamFlags RwTextureStreamFlags;
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

/* RWPUBLICEND */

/* RWPUBLIC */
/* Texture binary format */
extern RwInt32
RwTextureRegisterPluginStream(RwUInt32 pluginID,
                              RwPluginDataChunkReadCallBack readCB,
                              RwPluginDataChunkWriteCallBack writeCB,
                              RwPluginDataChunkGetSizeCallBack getSizeCB);

extern RwInt32
RwTextureSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                 RwPluginDataChunkAlwaysCallBack alwaysCB);

extern RwUInt32
RwTextureStreamGetSize(const RwTexture *texture);

extern RwTexture *
RwTextureStreamRead(RwStream *stream);

extern const RwTexture *
RwTextureStreamWrite(const RwTexture *texture,
                     RwStream *stream);

/* Texture dictionary binary format */
extern RwInt32
RwTexDictionaryRegisterPluginStream(RwUInt32 pluginID,
                                    RwPluginDataChunkReadCallBack readCB,
                                    RwPluginDataChunkWriteCallBack writeCB,
                                    RwPluginDataChunkGetSizeCallBack getSizeCB);

extern RwInt32
RwTexDictionarySetStreamAlwaysCallBack(RwUInt32 pluginID,
                                       RwPluginDataChunkAlwaysCallBack alwaysCB);

extern RwUInt32
RwTexDictionaryStreamGetSize(const RwTexDictionary *texDict);

extern RwTexDictionary
*RwTexDictionaryStreamRead(RwStream *stream);

extern const RwTexDictionary *
RwTexDictionaryStreamWrite(const RwTexDictionary *texDict,
                           RwStream *stream);

extern RwTextureChunkInfo *
_rwTextureChunkInfoRead(RwStream *stream,
                        RwTextureChunkInfo *textureChunkInfo,
                        RwInt32 *bytesRead);

/* Compatibility macro */

#define  RwTextureChunkInfoRead(_stream, _textureChunkInfo, _bytesRead) \
        _rwTextureChunkInfoRead(_stream, _textureChunkInfo, _bytesRead) 

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWBINTEX_H */
