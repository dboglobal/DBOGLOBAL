/*
 * Types for textures
 * Copyright (c) Criterion Software Limited
 */

/***************************************************************************
 *                                                                         *
 * Module  : batextur.h                                                    *
 *                                                                         *
 * Purpose : Texture map handling                                          *
 *                                                                         *
 **************************************************************************/

#ifndef RWTEXTUR_H
#define RWTEXTUR_H

/****************************************************************************
 Includes
 */

#include "batype.h"
#include "bamemory.h"
#include "baraster.h"
#include "baimmedi.h"
#include "baimage.h"

/* RWPUBLIC */

/****************************************************************************
 Defines
 */

/* Type ID */
#define rwTEXDICTIONARY 6

/* Mipmap Name generation - maximum number of RwChar characters which can
 * be appended to the root name.
 */
#define rwTEXTUREMIPMAPNAMECHARS    16

/* We define texture names to be a maximum of 16 ISO chars */
#define rwTEXTUREBASENAMELENGTH     32

#define rwTEXTUREFILTERMODEMASK     0x000000FF
#define rwTEXTUREADDRESSINGUMASK    0x00000F00
#define rwTEXTUREADDRESSINGVMASK    0x0000F000
#define rwTEXTUREADDRESSINGMASK     (rwTEXTUREADDRESSINGUMASK |  \
                                     rwTEXTUREADDRESSINGVMASK)

/****************************************************************************
 Global Types
 */

/**
 * \ingroup rwtexdict
 * \struct RwTexDictionary 
 * is a texture dictionary containing textures. 
 * This should be considered an opaque type.
 * Use the RwTexDictionary API functions to access.
 */
typedef struct RwTexDictionary RwTexDictionary;

#if (!defined(DOXYGEN))
struct RwTexDictionary
{
    RwObject            object; /* Homogeneous type */
    RwLinkList          texturesInDict; /* List of textures in dictionary */
    RwLLLink            lInInstance; /* Link list of all dicts in system */
};
/* Information is entirely device dependent */
#endif /* (!defined(DOXYGEN)) */


/* Parent is the dictionary */

/**
 * \ingroup rwtexture
 * \struct  RwTexture 
 * is a texture object. 
 * This should be considered an opaque type.
 * Use the RwTexture API functions to access.
 */
typedef struct RwTexture RwTexture;

#if (!defined(DOXYGEN))
struct RwTexture
{
    RwRaster           *raster; /** pointer to RwRaster with data */
    RwTexDictionary    *dict;   /* Dictionary this texture is in */
    RwLLLink            lInDictionary; /* List of textures in this dictionary */

    RwChar              name[rwTEXTUREBASENAMELENGTH];  /* Name of the texture */
    RwChar              mask[rwTEXTUREBASENAMELENGTH];  /* Name of the textures mask */

    /* 31 [xxxxxxxx xxxxxxxx vvvvuuuu ffffffff] 0 */
    RwUInt32            filterAddressing; /* Filtering & addressing mode flags */

    RwInt32             refCount; /* Reference count, surprisingly enough */
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rwtexture
 * \ref RwTextureCallBackRead 
 * represents the function used by \ref RwTextureRead to read the specified
 * texture from a disk file. This function should return a pointer to the
 * texture to indicate success.
 * 
 * \param  name   Pointer to a string containing the name of
 * the texture to read.
 *
 * \param  maskName   Pointer to a string containing the name
 * of the mask to read and apply to the texture.
 *
 * \return Pointer to the texture
 *
 * \see RwTextureSetReadCallBack
 * \see RwTextureGetReadCallBack
 */
typedef RwTexture *(*RwTextureCallBackRead)(const RwChar *name,
                                            const RwChar *maskName);

/**
 * \ingroup rwtexture
 * \ref RwTextureCallBackFind
 * represents the function used by \ref RwTextureRead to search for a 
 * texture in memory before attempting to read one from disk. This
 * may involve searching previously loaded texture dictionaries.
 *
 * \param  name   Pointer to a string containing the name of
 * the texture to find.
 *
 * \return Pointer to the texture, or NULL if not found.
 *
 * \see RwTextureSetFindCallBack
 * \see RwTextureGetFindCallBack
 */
typedef RwTexture *(*RwTextureCallBackFind)(const RwChar *name);

/**
 * \ingroup rwtexture
 * \ref RwTextureCallBack
 * represents the function called from \ref RwTexDictionaryForAllTextures
 * for all textures in a given texture dictionary. This function should
 * return the current texture to indicate success. The callback may return
 * NULL to terminate further callbacks on the texture dictionary.
 * 
 * \param  texture   Pointer to the current texture.
 *
 * \param  pData   User-defined data pointer.
 *
 * \return Pointer to the current texture
 *
 * \see RwTexDictionaryForAllTextures
 */
typedef RwTexture *(*RwTextureCallBack)(RwTexture *texture, void *pData);


/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryCallBack
 * represents the function called from \ref RwTexDictionaryForAllTexDictionaries
 * for all texture dictionaries that currently exist. This function should
 * return the current texture dictionary to indicate success. The callback may
 * return NULL to terminate further callbacks on the texture dictionary. It may
 * safely destroy the current texture dictionary without adversely affecting
 * the iteration process.
 * 
 * \param  dict   Pointer to the current texture dictionary.
 *
 * \param  pData   User-defined data pointer.
 *
 * \return Pointer to the current texture dictionary
 *
 * \see RwTexDictionaryForAllTexDictionaries
 */
typedef RwTexDictionary *(*RwTexDictionaryCallBack)(RwTexDictionary *dict, void *data);


/**
 * \ingroup rwtexture
 * \ref RwTextureCallBackMipmapGeneration 
 * is the callback function supplied to \ref RwTextureSetMipmapGenerationCallBack 
 * and returned from \ref RwTextureGetMipmapGenerationCallBack.
 *
 * The supplied function will be passed a pointer to a raster and an image.
 * The raster is the target for the generated mipmap levels and the image 
 * provides the base for their generation.
 * 
 * \param  raster   Pointer to raster, the target for generated mipmap levels
 * \param  image    Pointer to image, used to generate mipmap levels.
 * 
 * \return
 * Returns a pointer to the raster if successful or NULL if an error occurred.
 * 
 * \see RwTextureSetMipmapGenerationCallBack
 * \see RwTextureGetMipmapGenerationCallBack
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureGetAutoMipmapping
 */
typedef RwRaster *(*RwTextureCallBackMipmapGeneration)(RwRaster * raster,
                                                       RwImage * image);

/**
 * \ingroup rwtexture
 * \ref RwTextureCallBackMipmapName
 * is the callback function supplied to \ref RwTextureSetMipmapNameCallBack and
 * returned from \ref RwTextureGetMipmapNameCallBack.
 *
 * The supplied function will be passed a pointer to a root name, a maskName, a mipmap
 * level and a format. The function returns TRUE if successful and the root name will have been 
 * modified to equal the mipmap name.
 * 
 * \param  name       Pointer to a string containing the root name of the texture. The 
 * mipmap level name is put here.
 * \param  maskName   Pointer to a string containing the root mask name of the texture or
 * NULL if no mask name is required.
 * \param  mipLevel   A value equal to the mipmap level for which the name is required.
 * \param  format     A value describing the mipmapping mode. A combination of the bit
 * flags rwRASTERFORMATMIPMAP and rwRASTERFORMATAUTOMIPMAP.
 * 
 * \return
 * Returns TRUE if the name is generated successfully or FALSE if an error occurred.
 * 
 * \see RwTextureGenerateMipmapName
 * \see RwTextureSetMipmapNameCallBack
 * \see RwTextureGetMipmapNameCallBack
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureGetAutoMipmapping
 */
typedef RwBool (*RwTextureCallBackMipmapName)(RwChar *name,
                                              RwChar *maskName,
                                              RwUInt8 mipLevel,
                                              RwInt32 format);

/****************************************************************************
 <macro/inline functionality
 */

#define RwTextureGetRasterMacro(_tex)                       \
    ((_tex)->raster)

#define RwTextureAddRefMacro(_tex)                          \
    (((_tex)->refCount++), (_tex))

#define RwTextureAddRefVoidMacro(_tex)                      \
MACRO_START                                                 \
{                                                           \
    (_tex)->refCount++;                                     \
}                                                           \
MACRO_STOP

#define RwTextureGetNameMacro(_tex)                         \
    ((_tex)->name)

#define RwTextureGetMaskNameMacro(_tex)                     \
    ((_tex)->mask)

#define RwTextureGetDictionaryMacro(_tex)                   \
    ((_tex)->dict)

#define RwTextureSetFilterModeMacro(_tex, _filtering)                       \
    (((_tex)->filterAddressing =                                            \
      ((_tex)->filterAddressing & ~rwTEXTUREFILTERMODEMASK) |               \
      (((RwUInt32)(_filtering)) &  rwTEXTUREFILTERMODEMASK)),               \
     (_tex))

#define RwTextureGetFilterModeMacro(_tex)                                   \
    ((RwTextureFilterMode)((_tex)->filterAddressing &                       \
                           rwTEXTUREFILTERMODEMASK))

#define RwTextureSetAddressingMacro(_tex, _addressing)                      \
    (((_tex)->filterAddressing =                                            \
      ((_tex)->filterAddressing & ~rwTEXTUREADDRESSINGMASK) |               \
      (((((RwUInt32)(_addressing)) <<  8) & rwTEXTUREADDRESSINGUMASK) |     \
       ((((RwUInt32)(_addressing)) << 12) & rwTEXTUREADDRESSINGVMASK))),    \
     (_tex))

#define RwTextureSetAddressingUMacro(_tex, _addressing)                     \
    (((_tex)->filterAddressing =                                            \
      ((_tex)->filterAddressing & ~rwTEXTUREADDRESSINGUMASK) |              \
      (((RwUInt32)(_addressing) << 8) & rwTEXTUREADDRESSINGUMASK)),         \
     (_tex))

#define RwTextureSetAddressingVMacro(_tex, _addressing)                     \
    (((_tex)->filterAddressing =                                            \
      ((_tex)->filterAddressing & ~rwTEXTUREADDRESSINGVMASK) |              \
      (((RwUInt32)(_addressing) << 12) & rwTEXTUREADDRESSINGVMASK)),        \
     (_tex))

#define RwTextureGetAddressingMacro(_tex)                                   \
    (((((_tex)->filterAddressing & rwTEXTUREADDRESSINGUMASK) >>  8) ==      \
      (((_tex)->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12)) ?      \
     ((RwTextureAddressMode)(((_tex)->filterAddressing &                    \
                              rwTEXTUREADDRESSINGVMASK) >> 12)) :           \
     rwTEXTUREADDRESSNATEXTUREADDRESS)

#define RwTextureGetAddressingUMacro(_tex)                                  \
    ((RwTextureAddressMode)(((_tex)->filterAddressing &                     \
                             rwTEXTUREADDRESSINGUMASK) >> 8))

#define RwTextureGetAddressingVMacro(_tex)                                  \
    ((RwTextureAddressMode)(((_tex)->filterAddressing &                     \
                             rwTEXTUREADDRESSINGVMASK) >> 12))


#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RwTextureGetRaster(_tex)                            \
    RwTextureGetRasterMacro(_tex)

#define RwTextureAddRef(_tex)                               \
    RwTextureAddRefMacro(_tex)

#define RwTextureGetName(_tex)                              \
    RwTextureGetNameMacro(_tex)

#define RwTextureGetMaskName(_tex)                          \
    RwTextureGetMaskNameMacro(_tex)

#define RwTextureGetDictionary(_tex)                        \
    RwTextureGetDictionaryMacro(_tex)

#define RwTextureSetFilterMode(_tex, _filtering)            \
    RwTextureSetFilterModeMacro(_tex, _filtering)

#define RwTextureGetFilterMode(_tex)                        \
    RwTextureGetFilterModeMacro(_tex)

#define RwTextureSetAddressing(_tex, _addressing)           \
    RwTextureSetAddressingMacro(_tex, _addressing)

#define RwTextureSetAddressingU(_tex, _addressing)          \
    RwTextureSetAddressingUMacro(_tex, _addressing)

#define RwTextureSetAddressingV(_tex, _addressing)          \
    RwTextureSetAddressingVMacro(_tex, _addressing)

#define RwTextureGetAddressing(_tex)                        \
    RwTextureGetAddressingMacro(_tex)

#define RwTextureGetAddressingU(_tex)                       \
    RwTextureGetAddressingUMacro(_tex)

#define RwTextureGetAddressingV(_tex)                       \
    RwTextureGetAddressingVMacro(_tex)

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/* RWPUBLICEND */
typedef struct rwTextureGlobals rwTextureGlobals;
struct rwTextureGlobals
{
        RwLinkList          texDictList;
        RwFreeList         *textureFreeList;
        RwFreeList         *texDictFreeList;
        RwTexDictionary    *currentTexDict;
        RwTextureCallBackRead textureReadFunc;
        RwTextureCallBackFind textureFindFunc;
        RwBool              haveTexMipmaps;
        RwBool              generateTexMipmaps;
        RwUInt8            *scratchMem;
        RwInt16             scratchMemSize;
        RwInt16             pad;
        RwTextureCallBackMipmapGeneration mipmapCallback;
        RwTextureCallBackMipmapName mipmapNameCallback;
};

/****************************************************************************
 Globals (across program)
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RwPluginRegistry textureTKList;
extern RwPluginRegistry texDictTKList;

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLIC */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

    /* Reading mip maps */

    /* Setting mip mapping states */
extern RwBool       RwTextureSetMipmapping(RwBool enable);
extern RwBool       RwTextureGetMipmapping(void);
extern RwBool       RwTextureSetAutoMipmapping(RwBool enable);
extern RwBool       RwTextureGetAutoMipmapping(void);

    /* Setting and getting the mipmap generation function */
extern              RwBool
RwTextureSetMipmapGenerationCallBack(RwTextureCallBackMipmapGeneration
                                     callback);
extern              RwTextureCallBackMipmapGeneration
RwTextureGetMipmapGenerationCallBack(void);

    /* Setting and getting the mipmap file name generation function */
extern              RwBool
RwTextureSetMipmapNameCallBack(RwTextureCallBackMipmapName callback);
extern RwTextureCallBackMipmapName RwTextureGetMipmapNameCallBack(void);

    /* Generating mipmaps for a raster */
extern RwBool       RwTextureGenerateMipmapName(RwChar * name,
                                                RwChar * maskName,
                                                RwUInt8 mipLevel,
                                                RwInt32 format);
extern RwBool       RwTextureRasterGenerateMipmaps(RwRaster * raster,
                                                   RwImage * image);

    /* LEGACY-SUPPORT mip mapping */
extern RwBool       _rwTextureSetAutoMipMapState(RwBool enable);
extern RwBool       _rwTextureGetAutoMipMapState(void);

    /* Setting and getting the callback function */
extern RwTextureCallBackRead RwTextureGetReadCallBack(void);
extern RwBool       RwTextureSetReadCallBack(RwTextureCallBackRead callBack);

extern RwTextureCallBackFind RwTextureGetFindCallBack(void);
extern RwBool       RwTextureSetFindCallBack(RwTextureCallBackFind callBack);

    /* Texture and mask names */
extern RwTexture   *RwTextureSetName(RwTexture * texture,
                                     const RwChar * name);
extern RwTexture   *RwTextureSetMaskName(RwTexture * texture,
                                         const RwChar * maskName);

    /* Creating/destroying dictionaries */
extern void RwTexDictionarySetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );

extern RwTexDictionary *RwTexDictionaryCreate(void);
extern RwBool       RwTexDictionaryDestroy(RwTexDictionary * dict);

    /* Textures */
void RwTextureSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );

extern RwTexture   *RwTextureCreate(RwRaster * raster);
extern RwBool       RwTextureDestroy(RwTexture * texture);

    /* Setting and getting texture map rasters */
extern RwTexture   *RwTextureSetRaster(RwTexture * texture,
                                       RwRaster * raster);

    /* Dictionary access */
extern RwTexture   *RwTexDictionaryAddTexture(RwTexDictionary * dict,
                                              RwTexture * texture);
extern RwTexture   *RwTexDictionaryRemoveTexture(RwTexture * texture);
extern RwTexture   *RwTexDictionaryFindNamedTexture(RwTexDictionary *
                                                    dict,
                                                    const RwChar * name);

    /* Reading a texture */
extern RwTexture   *RwTextureRead(const RwChar * name,
                                  const RwChar * maskName);

    /* Setting the current dictionary */
extern RwTexDictionary *RwTexDictionaryGetCurrent(void);
extern RwTexDictionary *RwTexDictionarySetCurrent(RwTexDictionary * dict);

    /* Enumerating textures */
extern const RwTexDictionary *RwTexDictionaryForAllTextures(const
                                                            RwTexDictionary
                                                            * dict,
                                                            RwTextureCallBack
                                                            fpCallBack,
                                                            void *pData);

    /* Enumerating the texture dictionaries currently in the system */
extern RwBool RwTexDictionaryForAllTexDictionaries(
    RwTexDictionaryCallBack fpCallBack, void *pData);


    /* Attaching toolkits */
extern RwInt32      RwTextureRegisterPlugin(RwInt32 size,
                                            RwUInt32 pluginID,
                                            RwPluginObjectConstructor
                                            constructCB,
                                            RwPluginObjectDestructor
                                            destructCB,
                                            RwPluginObjectCopy copyCB);
extern RwInt32      RwTexDictionaryRegisterPlugin(RwInt32 size,
                                                  RwUInt32 pluginID,
                                                  RwPluginObjectConstructor
                                                  constructCB,
                                                  RwPluginObjectDestructor
                                                  destructCB,
                                                  RwPluginObjectCopy
                                                  copyCB);
extern RwInt32      RwTextureGetPluginOffset(RwUInt32 pluginID);
extern RwInt32      RwTexDictionaryGetPluginOffset(RwUInt32 pluginID);
extern RwBool       RwTextureValidatePlugins(const RwTexture * texture);
extern RwBool       RwTexDictionaryValidatePlugins(const RwTexDictionary *
                                                   dict);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
/* Textures */
extern RwRaster *RwTextureGetRaster(const RwTexture *texture);
extern RwTexture *RwTextureAddRef(RwTexture *texture);

/* Texture and mask names */
extern RwChar *RwTextureGetName(RwTexture *texture);
extern RwChar *RwTextureGetMaskName(RwTexture *texture);

/* Get owner dictionary */
extern RwTexDictionary *RwTextureGetDictionary(RwTexture *texture);

/* Filtering */
extern RwTexture *RwTextureSetFilterMode(RwTexture *texture,
                                         RwTextureFilterMode filtering);

extern RwTextureFilterMode RwTextureGetFilterMode(const RwTexture *texture);

/* Addressing */
extern RwTexture *RwTextureSetAddressing(RwTexture *texture,
                                         RwTextureAddressMode addressing);
extern RwTexture *RwTextureSetAddressingU(RwTexture *texture,
                                          RwTextureAddressMode addressing);
extern RwTexture *RwTextureSetAddressingV(RwTexture *texture,
                                          RwTextureAddressMode addressing);

extern RwTextureAddressMode RwTextureGetAddressing(const RwTexture *texture);
extern RwTextureAddressMode RwTextureGetAddressingU(const RwTexture *texture);
extern RwTextureAddressMode RwTextureGetAddressingV(const RwTexture *texture);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


    /* RWPUBLICEND */

    /* Opening and closing */
extern void        *_rwTextureOpen(void *instance, RwInt32 offset,
                                   RwInt32 size);
extern void        *_rwTextureClose(void *instance, RwInt32 offset,
                                    RwInt32 size);

    /* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#define RwTextureSetAutoMipMapState(_enable) \
    _rwTextureSetAutoMipMapState(_enable)

#define RwTextureGetAutoMipMapState() \
    _rwTextureGetAutoMipMapState()

/* RWPUBLICEND */

#endif                          /* RWTEXTUR_H */

