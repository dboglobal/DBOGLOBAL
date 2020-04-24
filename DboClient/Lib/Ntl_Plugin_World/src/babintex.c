/*
 * Handling binary texture representations.
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

/* Abstraction of string functionality - for unicode support */
#include "rwstring.h"

#include "babintex.h"

/****************************************************************************
 Local Types
 */

/* Texture stream format */
typedef struct _rwStreamTexture rwStreamTexture;
struct _rwStreamTexture
{
    RwUInt32            filterAndAddress;
};

/* Texture dictionary stream format */
typedef struct _rwStreamTexDictionary rwStreamTexDictionary;
struct _rwStreamTexDictionary
{
    /* This is required as to be backward compatible, we need to convert
       with the 32 bit endian function.... */
#ifdef rwLITTLEENDIAN
    RwUInt16           numTextures;
    RwUInt16           deviceId;
#else /* rwLITTLEENDIAN */
    RwUInt16           deviceId;
    RwUInt16           numTextures;
#endif /* rwLITTLEENDIAN */
};


typedef union _unionStreamSize unionStreamSize;
union _unionStreamSize
{
    RwStream           *stream;
    RwUInt32            size;
};

typedef struct _nativeTextureWriteInfo nativeTextureWriteInfo;
struct _nativeTextureWriteInfo
{
    unionStreamSize     u;
    RwBool              allOK;
};

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

   Texture Binary Format Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexture
 * \ref RwTextureRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered texture plugin.
 *
 * \param pluginID  A unique \ref RwInt32 value equal to the plugin ID. (It is used
 *       to identify binary chunks).
 * \param readCB  A callback used when a chunk is read that is identified as being for
 *       this plugin
 * \param writeCB  A callback used when a chunk should be written out for this plugin
 * \param getSizeCB  A callback used to determine the binary stream size required for this
 *       plugin (return negative to suppress chunk writing)
 *
 * \return Returns an \ref RwInt32 containing the byte offset within the texture of
 * memory reserved for this plugin, or a negative value if there is an error.
 *
 * \see RwTextureSetStreamAlwaysCallBack
 * \see RwTextureRegisterPlugin
 * \see RwTextureGetPluginOffset
 * \see RwTextureValidatePlugins
 *
 */
RwInt32
RwTextureRegisterPluginStream(RwUInt32 pluginID,
                              RwPluginDataChunkReadCallBack readCB,
                              RwPluginDataChunkWriteCallBack writeCB,
                              RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwTextureRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPluginStream(&textureTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetStreamAlwaysCallBack is used to associate a
 * binary stream functionality with a previously registered texture plugin.
 * This callback is called for all plugins after stream data reading has
 * completed.
 *
 * \param pluginID  An \ref RwInt32 value equal to the plugin ID. (This must
 * be unique. It is used to identify binary chunks.)
 * \param alwaysCB  The callback used when object base and plugin data reading is complete
 *
 * \return Returns an \ref RwInt32 containing the byte offset within the texture of
 * memory reserved for this plugin, or a negative value if there is an error
 *
 * \see RwTextureRegisterPluginStream
 * \see RwTextureRegisterPlugin
 * \see RwTextureGetPluginOffset
 * \see RwTextureValidatePlugins
 *
 */
RwInt32
RwTextureSetStreamAlwaysCallBack(
    RwUInt32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwTextureSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlgnStrmlwysCB(
               &textureTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureStreamGetSize is used to determine the size in bytes of
 * the binary representation of the specified texture. This is used in the
 * binary chunk header to indicate the size of the texture chunk.
 * The size does not include the size of the chunk header or the texture's
 * pixel data.
 *
 * \param texture  A pointer to the texture whose binary size is required.
 *
 * \return Returns the chunk size of the texture.
 *
 * \see RwTextureStreamRead
 * \see RwTextureStreamWrite
 * \see RwTextureRead
 */
RwUInt32
RwTextureStreamGetSize(const RwTexture *texture)
{
    RwUInt32            size;

    RWAPIFUNCTION(RWSTRING("RwTextureStreamGetSize"));
    RWASSERT(texture);

    /* First get the size of the texture without the extension chunks */
    size = sizeof(rwStreamTexture) + rwCHUNKHEADERSIZE;

    size += _rwStringStreamGetSize(texture->name) + rwCHUNKHEADERSIZE;
    size += _rwStringStreamGetSize(texture->mask) + rwCHUNKHEADERSIZE;

    /* Then add the size of the extension chunks */
    size += (_rwPluginRegistryGetSize(&textureTKList, texture) + rwCHUNKHEADERSIZE);

    RWRETURN(size);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureStreamWrite is used to write the specified texture to the
 * given binary stream. The stream must have been opened prior to this
 * function call.
 *
 * \param texture  A pointer to the texture to be written
 * \param stream  A pointer to the binary stream
 *
 * \return Returns a pointer to the texture if successful, or NULL if there is
 * an error
 *
 * \note Note that this API call does not serialize the pixel data for the
 *       texture.  To serialize the pixel data you must use texture
 *       dictionaries.
 *
 * \see RwTextureStreamGetSize
 * \see RwTextureStreamRead
 * \see RwTexDictionaryStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 *
 */
const RwTexture *
RwTextureStreamWrite(const RwTexture *texture, RwStream *stream)
{
    RwTextureStreamFlags    flags;
    rwStreamTexture         texFiltAddr;

    RWAPIFUNCTION(RWSTRING("RwTextureStreamWrite"));
    RWASSERT(texture);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader(stream, rwID_TEXTURE, RwTextureStreamGetSize(texture)))
    {
        RWRETURN((const RwTexture *)NULL);
    }

    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, sizeof(rwStreamTexture)))
    {
        RWRETURN((const RwTexture *)NULL);
    }

    /* Combine and put into the binary structure */
    if ((texture->raster) &&
        (!(texture->raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8))))
    {
        flags = rwTEXTURESTREAMFLAGSUSERMIPMAPS;
    }
    else
    {
        flags = rwNATEXTURESTREAMFLAG;
    }

    /* shift up the V an extra 4 bits to pack into 8 bits with addressing U */
    texFiltAddr.filterAndAddress = (texture->filterAddressing &
                                    (rwTEXTUREFILTERMODEMASK |
                                     rwTEXTUREADDRESSINGMASK)) |
                                   ((((RwInt32)flags) & 0xFF) << 16);

    /* Convert it */
    (void)RwMemLittleEndian32(&texFiltAddr, sizeof(texFiltAddr));

    /* And write it out */
    if (!RwStreamWrite(stream, &texFiltAddr, sizeof(texFiltAddr)))
    {
        RWRETURN((const RwTexture *)NULL);
    }

    /* and texture names */
    if (!_rwStringStreamWrite(texture->name, stream))
    {
        RWRETURN((const RwTexture *)NULL);
    }
    if (!_rwStringStreamWrite(texture->mask, stream))
    {
        RWRETURN((const RwTexture *)NULL);
    }

    /* And then the texture extension chunk */
    if (!_rwPluginRegistryWriteDataChunks(&textureTKList, stream, texture))
    {
        /* Failed to write extension data */
        RWRETURN((const RwTexture *)NULL);
    }

    RWRETURN(texture);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureStreamRead is used to read a texture from the specified
 * binary stream. Prior to this function call a binary texture chunk must
 * have been found in the stream.
 *
 * The sequence to locate and read a texture from a binary stream connected
 * to a disk file is as follows:
 *
 * \code
   RwStream *stream;
   RwTexture *newTexture;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_TEXTURE, NULL, NULL) )
       {
           newTexture = RwTextureStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
   \endcode
 *
 * \param stream  A pointer to the binary stream from which the texture will be read
 *
 * \return Returns a pointer to the texture if successful or NULL if there is
 * an error
 *
 * \see RwTextureStreamGetSize
 * \see RwTextureStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 */
RwTexture *
RwTextureStreamRead(RwStream *stream)
{
    RwUInt32    size, version;

    RWAPIFUNCTION(RWSTRING("RwTextureStreamRead"));
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RwTexture *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        RwTexture               *texture;
        RwChar                  textureName[rwTEXTUREBASENAMELENGTH*4];
        RwChar                  textureMask[rwTEXTUREBASENAMELENGTH*4];
        RwTextureFilterMode     filtering;
        RwTextureAddressMode    addressingU;
        RwTextureAddressMode    addressingV;
        rwStreamTexture         texFiltAddr;
        RwBool                  mipmapState;
        RwBool                  autoMipmapState;
        RwTextureStreamFlags    flags;

        
        RWASSERT(size <= sizeof(texFiltAddr));// only works in debug

		if (!(size <= sizeof(texFiltAddr)))
		{
			RWRETURN((RwTexture *)NULL);
		}

		/* Read the filtering mode */
        memset(&texFiltAddr, 0, sizeof(texFiltAddr));
        if (RwStreamRead(stream, &texFiltAddr, size) != size)
        {
            RWRETURN((RwTexture *)NULL);
        }

        /* Convert it */
        (void)RwMemNative32(&texFiltAddr, sizeof(texFiltAddr));

        /* Extract filtering */
        filtering = (RwTextureFilterMode)
                    (texFiltAddr.filterAndAddress & rwTEXTUREFILTERMODEMASK);

        /* Extract addressing */
        addressingU = (RwTextureAddressMode)
                      ((texFiltAddr.filterAndAddress >> 8) & 0x0F);

        addressingV = (RwTextureAddressMode)
                      ((texFiltAddr.filterAndAddress >> 12) & 0x0F);

        /* Make sure addressingV is valid so files old than 3.04 still work */
        if (addressingV == rwTEXTUREADDRESSNATEXTUREADDRESS)
        {
            addressingV = addressingU;
            texFiltAddr.filterAndAddress |= ((addressingV & 0xF) << 12);
        }

        /* Extract user mipmap flags */
        flags = (RwTextureStreamFlags)((texFiltAddr.filterAndAddress >> 16) & 0xFF);

        mipmapState = RwTextureGetMipmapping();
        autoMipmapState = RwTextureGetAutoMipmapping();

        /* Use it */
        if ((filtering == rwFILTERMIPNEAREST) ||
            (filtering == rwFILTERMIPLINEAR) ||
            (filtering == rwFILTERLINEARMIPNEAREST) ||
            (filtering == rwFILTERLINEARMIPLINEAR))
        {
            /* Lets mip map it */
            RwTextureSetMipmapping(TRUE);
            if (flags & rwTEXTURESTREAMFLAGSUSERMIPMAPS)
            {
                RwTextureSetAutoMipmapping(FALSE);
            }
            else
            {
                RwTextureSetAutoMipmapping(TRUE);
            }
        }
        else
        {
            /* Lets not */
            RwTextureSetMipmapping(FALSE);
            RwTextureSetAutoMipmapping(FALSE);
        }

        /* Search for a string or a unicode string */
        if (!_rwStringStreamFindAndRead(textureName, stream))
        {
            RwTextureSetMipmapping(mipmapState);
            RwTextureSetAutoMipmapping(autoMipmapState);

            RWRETURN((RwTexture *)NULL);
        }

        /* Search for a string or a unicode string */
        if (!_rwStringStreamFindAndRead(textureMask,stream))
        {
            RwTextureSetMipmapping(mipmapState);
            RwTextureSetAutoMipmapping(autoMipmapState);

            RWRETURN((RwTexture *)NULL);
        }

        /* Get the textures */
        if (!(texture = RwTextureRead(textureName, textureMask)))
        {
            /* Skip any extension chunks */
            _rwPluginRegistrySkipDataChunks(&textureTKList, stream);

            RwTextureSetMipmapping(mipmapState);
            RwTextureSetAutoMipmapping(autoMipmapState);

            RWRETURN((RwTexture *)NULL);
        }

        /* clean up */
        RwTextureSetMipmapping(mipmapState);
        RwTextureSetAutoMipmapping(autoMipmapState);

        RWASSERT(0 < texture->refCount);

        if (texture->refCount == 1)
        {
            /* By testing the reference count here,
             * we can tell if we just loaded it!!! */
            
            /* Set the filtering and addressing */
            texture->filterAddressing = texFiltAddr.filterAndAddress &
                (rwTEXTUREFILTERMODEMASK | rwTEXTUREADDRESSINGMASK);
            
            /* Read the extension chunks */
            if (!_rwPluginRegistryReadDataChunks(&textureTKList, stream, texture))
            {
                RWRETURN((RwTexture *)NULL);
            }
        }
        else
        {
            if (!_rwPluginRegistrySkipDataChunks(&textureTKList, stream))
            {
                RWRETURN((RwTexture *)NULL);
            }
        }

        RWRETURN(texture);
	}
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RwTexture *)NULL);
    }
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered TexDictionary
 * Plugin.
 *
 * \param pluginID  An \ref RwInt32 value equal to the plugin ID. (This must
 *  be unique. It is used to identify binary chunks)
 * \param readCB  A callback used when a chunk is read that is identified as being for
 *       this plugin
 * \param writeCB  A callback used when a chunk should be written out for this plugin
 * \param getSizeCB  A callback used to determine the binary stream size
 * required for this plugin (return negative to suppress chunk writing)
 *
 * \return Returns an \ref RwInt32 containing the byte offset within the texture
 * of memory reserved for this plugin, or -1 if there is an error
 *
 * \see RwTexDictionarySetStreamAlwaysCallBack
 * \see RwTexDictionaryRegisterPlugin
 * \see RwTexDictionaryGetPluginOffset
 * \see RwTexDictionaryValidatePlugins
 *
 */
RwInt32
RwTexDictionaryRegisterPluginStream(RwUInt32 pluginID,
                                    RwPluginDataChunkReadCallBack readCB,
                                    RwPluginDataChunkWriteCallBack writeCB,
                                    RwPluginDataChunkGetSizeCallBack
                                    getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPluginStream(&texDictTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionarySetStreamAlwaysCallBack is used to
 * associate a binary stream functionality with a previously registered
 * TexDictionary Plugin. This callback is called for all plugins after
 * stream data reading has completed.
 *
 * \param pluginID  An \ref RwInt32 value equal to the plugin ID (This must
 *  be unique. It is used to identify binary chunks)
 * \param alwaysCB  The callback used when object base and plugin data reading is complete
 *
 * \return Returns an \ref RwInt32 containing the byte offset within the texture
 * of memory reserved for this plugin, or -1 if there is an error
 *
 * \see RwTexDictionaryRegisterPluginStream
 * \see RwTexDictionaryRegisterPlugin
 * \see RwTexDictionaryGetPluginOffset
 * \see RwTexDictionaryValidatePlugins
 *
 */
RwInt32
RwTexDictionarySetStreamAlwaysCallBack(
    RwUInt32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwTexDictionarySetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlgnStrmlwysCB(
               &texDictTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/****************************************************************************
 addNativeTextureSize

 On entry   : Texture pointer
            : User data
 On exit    : Texture pointer on success
 */

static RwTexture   *
addNativeTextureSize(RwTexture * texture, void *data)
{
    nativeTextureWriteInfo *info = (nativeTextureWriteInfo *) data;
    RwUInt32            length;

    RWFUNCTION(RWSTRING("addNativeTextureSize"));
    RWASSERT(texture);
    RWASSERT(info);

    /* Call to device to find size of device specific serialised texture */
    if (!RWSRCGLOBAL
        (stdFunc[rwSTANDARDNATIVETEXTUREGETSIZE] (&length, texture, 0)))
    {
        /* It's all gone horribly wrong */
        info->allOK = FALSE;
        RWRETURN((RwTexture *)NULL);
    }

    /* Add the size of the data (and the header we put on) */
    info->u.size += (length + rwCHUNKHEADERSIZE);

    /* And the plugin data
     * (we don't want the driver to be concerned about all this) */
    info->u.size += ( _rwPluginRegistryGetSize(&textureTKList, texture) +
                      rwCHUNKHEADERSIZE );

    RWRETURN(texture);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryStreamGetSize is used to determine the size in
 * bytes of the binary representation of the specified texture dictionary.
 * This is used in the binary chunk header to indicate the size of the texture
 * chunk. The size does not include the size of the chunk header.
 *
 * \note Note that texture dictionaries are device specific, and cannot
 * generally be used on multiple platforms.
 *
 * \param texDict  A pointer to the texture dictionary whose binary size is required
 *
 * \return Returns the chunk size of the texture dictionary if successful or
 * zero if there is an error.
 *
 * \see RwTexDictionaryStreamRead
 * \see RwTexDictionaryStreamWrite
 *
 */
RwUInt32
RwTexDictionaryStreamGetSize(const RwTexDictionary *texDict)
{
    nativeTextureWriteInfo info;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryStreamGetSize"));
    RWASSERT(texDict);

    /* First get the size of the texDictionary without the extension chunks */
    info.u.size = sizeof(rwStreamTexDictionary) + rwCHUNKHEADERSIZE;

    /* Now find out the size of all the textures */
    info.allOK = TRUE;
    RwTexDictionaryForAllTextures(texDict, addNativeTextureSize, &info);

    if (!info.allOK)
    {
        RWRETURN(0);
    }

    /* Then add the size of the extension chunks */
    info.u.size += (_rwPluginRegistryGetSize(&texDictTKList, texDict) + rwCHUNKHEADERSIZE);

    RWRETURN(info.u.size);
}

/****************************************************************************
 countTexturesInDictionary

 On entry   : Texture pointer
            : User data
 On exit    : Texture pointer on success
 */

static RwTexture   *
countTexturesInDictionary( RwTexture * texture __RWUNUSED__,
                           void *data )
{
    RwUInt32          *countPtr = (RwUInt32 *) data;
    RwUInt32           count;

    RWFUNCTION(RWSTRING("countTexturesInDictionary"));
    RWASSERT(texture);
    RWASSERT(countPtr);

    count = *countPtr;
    *countPtr = ++count;

    RWRETURN(texture);
}

/****************************************************************************
 writeNativeTexture

 On entry   : Texture pointer
            : User data
 On exit    : Texture pointer on success
 */

static RwTexture   *
writeNativeTexture(RwTexture * texture, void *data)
{
    nativeTextureWriteInfo *info = (nativeTextureWriteInfo *) data;
    RwUInt32            length;

    RWFUNCTION(RWSTRING("writeNativeTexture"));
    RWASSERT(texture);
    RWASSERT(info);

    /* How big is it? */
    if (!RWSRCGLOBAL
        (stdFunc[rwSTANDARDNATIVETEXTUREGETSIZE] (&length, texture, 0)))
    {
        /* It's all gone horribly wrong */
        info->allOK = FALSE;
        RWRETURN((RwTexture *)NULL);
    }

    /* And the plugin data
     * (we don't want the driver to be concerned about all this) */
    length += ( _rwPluginRegistryGetSize(&textureTKList, texture) +
                rwCHUNKHEADERSIZE );

    /* Chunk header to write... */
    if (!RwStreamWriteChunkHeader(info->u.stream, rwID_TEXTURENATIVE, length))
    {
        info->allOK = FALSE;
        RWRETURN((RwTexture *)NULL);
    }

    /* ...then call the device to write it */
    if (!RWSRCGLOBAL
        (stdFunc[rwSTANDARDNATIVETEXTUREWRITE]
         (info->u.stream, texture, length)))
    {
        /* It's all gone horribly wrong */
        info->allOK = FALSE;
        RWRETURN((RwTexture *)NULL);
    }

    /* And write out the plugin data */
    if (!_rwPluginRegistryWriteDataChunks
        (&textureTKList, info->u.stream, texture))
    {
        /* Failed to write extension data */
        info->allOK = FALSE;
        RWRETURN((RwTexture *)NULL);
    }

    /* All done now */
    RWRETURN(texture);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryStreamWrite is used to write the specified texture
 * dictionary to the given binary stream. The stream must have been opened
 * prior to this function call.
 *
 * \note Note that texture dictionaries are device specific, and cannot
 * generally be used on multiple platforms.
 *
 * \param texDict  A pointer to the texture dictionary to be written
 * \param stream  A pointer to the binary stream
 *
 * \return Returns a pointer to the texture dictionary if successful or NULL if
 * there is an error.
 *
 * \see RwTexDictionaryStreamGetSize
 * \see RwTexDictionaryStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 *
 */
const RwTexDictionary *
RwTexDictionaryStreamWrite(const RwTexDictionary *texDict, RwStream *stream)
{
    rwStreamTexDictionary    binTexDict;
    nativeTextureWriteInfo info;
    RwUInt32            length;
    RwUInt32            numTextures;
    RwDevice *const device = &RWSRCGLOBAL(dOpenDevice);

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryStreamWrite"));
    RWASSERT(texDict);
    RWASSERT(stream);

    length = RwTexDictionaryStreamGetSize(texDict);
    if (!length)
    {
        RWRETURN((const RwTexDictionary *)NULL);
    }

    if (!RwStreamWriteChunkHeader(stream, rwID_TEXDICTIONARY, length))
    {
        RWRETURN((const RwTexDictionary *)NULL);
    }

    if (!RwStreamWriteChunkHeader
        (stream, rwID_STRUCT, sizeof(rwStreamTexDictionary)))
    {
        RWRETURN((const RwTexDictionary *)NULL);
    }

    /* Write out structure with texture count in */
    numTextures = 0;
    binTexDict.numTextures = 0;
    RwTexDictionaryForAllTextures(texDict, countTexturesInDictionary,
                                  &numTextures);
    /* We have changed the size of this field to 16 bits */
    RWASSERT(numTextures < 65536);

    /* This is a function that should fail cleanly */
    if (numTextures >= 65536)
    {
        RWRETURN((const RwTexDictionary *)NULL);
    }

    binTexDict.numTextures = (RwUInt16)numTextures;
    binTexDict.deviceId = 0;
    _rwDeviceSystemRequest(device, rwDEVICESYSTEMGETID, &binTexDict.deviceId,
                           NULL, 0);
    /* Texture dictionaries should be as the "Real" device */
    binTexDict.deviceId = binTexDict.deviceId & 0xff;

    (void)RwMemLittleEndian32(&binTexDict, sizeof(binTexDict));
    if (!RwStreamWrite(stream, &binTexDict, sizeof(binTexDict)))
    {
        RWRETURN((const RwTexDictionary *)NULL);
    }

    /* Now we just need to write all the textures - easy, right? */
    info.u.stream = stream;
    info.allOK = TRUE;
    RwTexDictionaryForAllTextures(texDict, writeNativeTexture, &info);

    if (!_rwPluginRegistryWriteDataChunks(&texDictTKList, stream, texDict))
    {
        /* Oooops */
        info.allOK = FALSE;
    }

    /* Was it good for you? :-) */
    if (!info.allOK)
    {
        RWRETURN((const RwTexDictionary *)NULL);
    }

    /* And we're all done */
    RWRETURN(texDict);
}

/****************************************************************************
 destroyTexture

 On entry   : Texture pointer
            : User data
 On exit    : Texture pointer on success
 */

static RwTexture   *
destroyTexture(RwTexture * texture,
               void * data __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("destroyTexture"));
    RWASSERT(texture);

    RwTextureDestroy(texture);

    RWRETURN(texture);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryStreamRead is used to read a texture dictionary
 * from the specified binary stream. Prior to this function call, a binary
 * texture dictionary chunk must have been found in the stream.
 *
 * \note Note that texture dictionaries are device specific, and cannot
 * generally be used on multiple platforms.
 *
 *
 * The sequence to locate and read a texture dictionary from a binary
 * stream connected to a disk file is as follows:
 * \code
   RwStream *stream;
   RwTexDictionary *newTexDictionary;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_TEXDICTIONARY, NULL, NULL) )
       {
           newTexDictionary = RwTexDictionaryStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
   \endcode
 *
 * \param stream  A pointer to the binary stream from which the texture
 * dictionary will be read
 *
 * \return Returns pointer to the texture dictionary if successful or NULL if
 * there is an error.
 *
 * \see RwTexDictionaryStreamGetSize
 * \see RwTexDictionaryStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 *
 */
RwTexDictionary *
RwTexDictionaryStreamRead(RwStream *stream)
{
    RwUInt32            size, version;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryStreamRead"));
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RwTexDictionary *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        RwTexDictionary    *texDict;
        rwStreamTexDictionary    binTexDict;
        RwDevice *const device = &RWSRCGLOBAL(dOpenDevice);
        RwUInt16 myId;

        /* Read the texture dictionary back */
        RWASSERT(size <= sizeof(binTexDict));
        memset(&binTexDict, 0, sizeof(binTexDict));
        if (RwStreamRead(stream, &binTexDict, size) != size)
        {
            RWRETURN((RwTexDictionary *)NULL);
        }

        (void)RwMemNative32(&binTexDict, sizeof(binTexDict));

        myId = 0;
        _rwDeviceSystemRequest(device, rwDEVICESYSTEMGETID, &myId, NULL, 0);
        myId = myId & 0xff;
        /* We allow for the case where no ids are defined. This will break
           if an old texture dictionary is seen which has more than 65535
           textures, but we hope this is an unusual case */
        if ((myId != 0) && (binTexDict.deviceId != 0)
            && (myId != binTexDict.deviceId))
        {
            RWASSERTM((myId == binTexDict.deviceId),
                      ("Texture dictionary for wrong platform"));
            RWRETURN((RwTexDictionary *)NULL);
        }

        texDict = RwTexDictionaryCreate();
        if (!texDict)
        {
            RWRETURN((RwTexDictionary *)NULL);
        }

        while (binTexDict.numTextures--)
        {
            RwTexture          *newTex;

            /* Find the native texture */
            if (!RwStreamFindChunk(stream, rwID_TEXTURENATIVE, &size, &version))
            {
                /* Tidy up and exit */
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);

                RWRETURN((RwTexDictionary *)NULL);
            }

            if ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION))
            {
                /* Tidy up and exit */
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);

                RWERROR((E_RW_BADVERSION));
                RWRETURN((RwTexDictionary *)NULL);
            }

            /* Call the driver to read the texture */
            if (!RWSRCGLOBAL(stdFunc[rwSTANDARDNATIVETEXTUREREAD](stream, &newTex, size)))
            {
                /* Tidy up and exit */
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);

                RWRETURN((RwTexDictionary *)NULL);
            }

            if (!newTex)
            {
                /* Tidy up and exit */
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);

                RWRETURN((RwTexDictionary *)NULL);
            }

            /* And read in the data chunks for the texture */
            if (!_rwPluginRegistryReadDataChunks(&textureTKList, stream, newTex))
            {
                /* Tidy up and exit */
                RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
                RwTexDictionaryDestroy(texDict);

                RWRETURN((RwTexDictionary *)NULL);
            }

            RwTexDictionaryAddTexture(texDict, newTex);
        }

        /* And read in the data chunks for the texture dictionary */
        if (!_rwPluginRegistryReadDataChunks(&texDictTKList, stream, texDict))
        {
            /* Tidy up and exit */
            RwTexDictionaryForAllTextures(texDict, destroyTexture, NULL);
            RwTexDictionaryDestroy(texDict);

            RWRETURN((RwTexDictionary *)NULL);
        }

        /* And we're all done */
        RWRETURN(texDict);
    }
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RwTexDictionary *)NULL);
    }
}

/*     - removed from the API Reference January 2002 -
   RwTextureChunkInfoRead extracts Texture Chunk Info data from
   a RenderWare stream. The data is converted from its original format and
   inserted into an RwTextureChunkInfo structure. A pointer to this structure
   is returned on success.

   param stream  A pointer to the stream from which to read
   param textureChunkInfo  A pointer to the Chunk Info structure to be filled
   param bytesRead  A pointer to an \ref RwInt32 which will be filled with the number of bytes
   read

   return Returns a pointer to the filled Chunk Info structure if successful,
   or NULL if an error occurred.

   see RwTextureStreamRead
   see RwTextureStreamWrite

 */
RwTextureChunkInfo *
_rwTextureChunkInfoRead(RwStream *stream,
                        RwTextureChunkInfo *textureChunkInfo,
                        RwInt32 *bytesRead)
{
    RwUInt32            size, readSize;
    rwStreamTexture     texFiltAddr;

    RWFUNCTION(RWSTRING("_rwTextureChunkInfoRead"));
    RWASSERT(stream);
    RWASSERT(textureChunkInfo);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                           &size, (RwUInt32 *)NULL))
    {
        RWRETURN((RwTextureChunkInfo *)NULL);
    }

    /* Read the filtering mode */
    RWASSERT(size <= sizeof(texFiltAddr));
    readSize = sizeof(rwStreamTexture);
    memset(&texFiltAddr, 0, readSize);
    if (RwStreamRead(stream, &texFiltAddr, readSize) != readSize)
    {
        RWRETURN((RwTextureChunkInfo *)NULL);
    }

    *bytesRead = size + (sizeof(RwInt32) * 3);
    /* move on to known place */
    RwStreamSkip(stream, size - readSize);

    /* Convert it */
    (void)RwMemNative32(&texFiltAddr, sizeof(texFiltAddr));

    /* Extract filtering and addressing */
    textureChunkInfo->filtering = (RwTextureFilterMode)
        (texFiltAddr.filterAndAddress & rwTEXTUREFILTERMODEMASK);

    textureChunkInfo->addressingU = (RwTextureAddressMode)
        ((texFiltAddr.filterAndAddress & rwTEXTUREADDRESSINGUMASK) >> 8);

    textureChunkInfo->addressingV = (RwTextureAddressMode)
        ((texFiltAddr.filterAndAddress & rwTEXTUREADDRESSINGVMASK) >> 12);

    RWRETURN(textureChunkInfo);
}

