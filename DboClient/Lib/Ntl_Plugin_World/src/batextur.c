/*
 * Texture handling.
 * Textures are special cases of rasters that can be applied to polygons
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwtexdict
 * \page rwtexdictoverview RwTexDictionary Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection texdictoverview Overview
 *
 * This object represents a Texture Dictionary: a database of Textures
 * (\ref RwTexture) and their associated Rasters (\ref RwRaster). They
 * may be used in two main ways:
 *
 * \li As a simple cache to prevent the same textures being loaded more
 *     than once.
 * \li As an object that may be streamed in or out to load
 *     or save a set of textures efficiently when required.
 *
 * Any number of dictionaries may be managed by the system, one of which
 * may be set as the 'current' active dictionary. On initialization,
 * RenderWare Graphics creates a single dictionary as the current one.
 *
 * See:
 *
 * \li \ref RwTexDictionaryCreate
 * \li \ref RwTexDictionaryDestroy
 * \li \ref RwTexDictionarySetCurrent
 * \li \ref RwTexDictionaryGetCurrent
 *
 * <H3> Finding textures </H3>
 *
 * When an attempt is made to load a texture, the current dictionary is
 * first searched if one is defined. If there is no current dictionary, then
 * all dictionaries in the system are searched. If the texture is
 * not found, then the image file is read from disk. See also:
 *
 * \li \ref RwTextureRead
 * \li \ref RwTexDictionaryFindNamedTexture
 *
 * <H3> Adding textures to dictionaries </H3>
 *
 * If a texture is loaded from a file, then it is automatically added to the
 * current dictionary if one is defined. Alternatively, a texture may be
 * added to a dictionary manually using \ref RwTexDictionaryAddTexture, but
 * note that a texture may only be attached to one dictionary at a time.
 *
 * A texture's reference count is not actually incremented when it is added
 * to a dictionary. When the texture is destroyed ( \ref RwTextureDestroy
 * is called for the last reference) it is also removed from the dictionary.
 * Any other objects that are created or loaded and require the texture after
 * this will cause it to be loaded again. To avoid this, a texture may be
 * made to persist in the dictionary by incrementing its reference count with
 * \ref RwTextureAddRef. Note that if a texture dictionary is read in from
 * a stream, then its textures are automatically made persistent.
 *
 * A dictionary should not be destroyed until all objects
 * referencing textures in the dictionary have been destroyed first. After
 * this, the only textures that should remain are those that have been
 * made persistent, and they should have a reference count of 1. They will
 * be destroyed during a call to \ref RwTexDictionaryDestroy since this
 * calls \ref RwTextureDestroy for any remaining textures.
 *
 * See:
 *
 * \li \ref RwTexDictionaryAddTexture
 * \li \ref RwTexDictionaryRemoveTexture
 *
 * <H3> Iterators </H3>
 *
 * These are for iterating over all textures in a dictionary or all
 * dictionaries in the system. See:
 *
 * \li \ref RwTexDictionaryForAllTextures
 * \li \ref RwTexDictionaryForAllTexDictionaries
 *
 * <H3> Streaming texture dictionaries </H3>
 *
 * See:
 *
 * \li \ref RwTexDictionaryStreamRead
 * \li \ref RwTexDictionaryStreamWrite
 * \li \ref RwTexDictionaryStreamGetSize
 *
 * When a saved dictionary is streamed in, the textures do have reference
 * counts for their dictionary entry. This means that objects using the
 * textures may be created and destroyed without the textures being
 * completely destroyed and removed from the dictionary. When the textures
 * are no longer required, destruction of the dictionary will cause them
 * to be destroyed.
 *
 * Developers should note that, when written to a RenderWare Binary Stream,
 * Texture Dictionaries also store the associated Rasters. Rasters are, by
 * definition, platform-dependent, so Texture Dictionaries cannot normally
 * be read on platforms other than those on which they were generated.
 *
 * If a texture dictionary would be usefully read on multiple platforms, then
 * a \e Platform \e Independent Texture Dictionary can be created. See
 * \ref rtpitexd for more details.
 *
 * <H3> Texture dictionary plugins </H3>
 *
 * See:
 *
 * \li \ref RwTexDictionaryRegisterPlugin
 * \li \ref RwTexDictionaryRegisterPluginStream
 * \li \ref RwTexDictionarySetStreamAlwaysCallBack
 * \li \ref RwTexDictionaryValidatePlugins
 * \li \ref RwTexDictionaryGetPluginOffset
 */

/**
 * \ingroup rwtexture
 * \page rwtextureoverview RwTexture Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection textureoverview Overview
 *
 * Represents a standard RenderWare Texture map.
 *
 * Textures reference Rasters (\ref RwRaster), which contain the actual texture and, if applicable,
 * any mip-map data. The texture object is therefore an extension of the basic raster
 * object and some functionality is split across the two APIs because of this.
 * For instance, mip-map functions are found under \ref RwRaster, not RwTexture.
 *
 * Textures rely on the \ref RwImage object to load bitmaps from standard image file formats
 * such as .BMP, .RAS and .PNG.
 *
 *
*/


/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "batkreg.h"
#include "badevice.h"
#include "bamemory.h"
#include "baimage.h"
#include "baraster.h"
#include "baimras.h"
#include "baresamp.h"
#include "baimmedi.h"

/* Abstraction of string functionality [for unicode support] */
#include "rwstring.h"

/* Texture handling */
#include "batextur.h"

#if defined(RWEVALUATION)
#include "bacamval.h"
#endif /* defined(RWEVALUATION) */

#include "../../driver/common/palquant.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWTEXTUREGLOBAL(var)                                    \
      (RWPLUGINOFFSET(rwTextureGlobals,                         \
                      RwEngineInstance,                         \
                      textureModule.globalsOffset)->var)

#if (!defined(__RWUNUSEDUNLESSTEXDICTSTATE__))
#define __RWUNUSEDUNLESSTEXDICTSTATE__ /* No op */
#endif /* (!defined(__RWUNUSEDUNLESSTEXDICTSTATE__)) */

#define RWSTRNSTORE(_dst, _src)                               \
MACRO_START                                                   \
{                                                             \
    const size_t n = sizeof((_dst))/sizeof((_dst)[0]);        \
                                                              \
    rwstrncpy((_dst), (_src), n);                               \
                                                              \
    if ( n <= rwstrlen((_src)))                               \
    {                                                         \
        const size_t trunc = n-1;                             \
                                                              \
        RWERROR((E_RW_STRING_TRUNCATION,                      \
                (_src), n, trunc, (_src)[trunc]));            \
        (_dst)[trunc] = ((RwChar)0);                          \
    }                                                         \
}                                                             \
MACRO_STOP

/****************************************************************************
 Globals (across program)
 */

RwPluginRegistry    textureTKList =
    { sizeof(RwTexture),
      sizeof(RwTexture),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

RwPluginRegistry    texDictTKList =
    { sizeof(RwTexDictionary),
      sizeof(RwTexDictionary),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo textureModule;

static RwTexDictionary *dummyTexDict = /* dummy texture dictionary */
    (RwTexDictionary *)NULL;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Static slave functions to _rwTextureOpen

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************/

static              RwChar
CalculateIndexCharacter(RwUInt8 mipLevel)
{
    RwChar              result = 0;

    static const RwChar character[] = RWSTRING("0123456789abcdef");

    RWFUNCTION(RWSTRING("CalculateIndexCharacter"));

    result = ((mipLevel > 0)
              && (mipLevel < 16)) ? character[mipLevel] : 0;

    RWRETURN(result);
}

/****************************************************************************/

static              RwBool
TextureDefaultMipmapName(RwChar * name, RwChar * maskName,
                         RwUInt8 mipLevel, RwInt32 __RWUNUSED__ format)
{

    RwChar              mipCharacterString[3];

    RWFUNCTION(RWSTRING("TextureDefaultMipmapName"));
    RWASSERT(name);
    RWASSERT(rwTEXTUREMIPMAPNAMECHARS >= 2);

    mipCharacterString[0] = 'm';
    mipCharacterString[1] = CalculateIndexCharacter(mipLevel);
    mipCharacterString[2] = ((RwChar) 0);

    if (mipCharacterString[1] != ((RwChar) 0))
    {
        rwstrcat(name, mipCharacterString);
        if (maskName && maskName[0])
        {
            rwstrcat(maskName, mipCharacterString);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************/
static RwBool
PalettizeImage(RwImage **image, RwInt32 depth)
{
    RwRGBA      palette[256];
    RwImage     *palImage;
    RwPalQuant  palQuant;

    RWFUNCTION(RWSTRING("PalettizeImage"));
    RWASSERT(image);
    RWASSERT((depth == 4) | (depth == 8));

    if (!RwPalQuantInit(&palQuant))
    {
        RWRETURN(FALSE);
    }

    /* add all pixels from the image */
    RwPalQuantAddImage(&palQuant, *image, (RwReal)(1));
    RwPalQuantResolvePalette(palette, (1UL << depth), &palQuant);

    /* Create the new image */
    palImage = RwImageCreate(RwImageGetWidth(*image),
                             RwImageGetHeight(*image),
                             depth);
    if (palImage)
    {
        /* Allocate the pixels & palette */
        RwImageAllocatePixels(palImage);

        /* match images */
        RwPalQuantMatchImage(RwImageGetPixels(palImage),
                             RwImageGetStride(palImage),
                             RwImageGetDepth(palImage),
                             FALSE,
                             &palQuant,
                             *image);

        /* Copy the palette */
        memcpy(RwImageGetPalette(palImage), palette,
               sizeof(RwRGBA) * (1UL << depth));

        /* Destroy the old image */
        RwImageDestroy(*image);

        /* Set the new one */
        *image = palImage;
    }
    else
    {
        RWRETURN(FALSE);
    }

    RwPalQuantTerm(&palQuant);

    RWRETURN(TRUE);
}

/****************************************************************************/
static RwBool
PalettizeMipmaps(RwRGBA *MipPalette, RwImage *sourceImage,
                 RwImage **mipArray, RwInt32 totalMipLevels,
                 RwInt32 logpalsize)
{
    RwInt32     mipLevel, i;
    RwPalQuant  palQuant;

    RWFUNCTION(RWSTRING("PalettizeMipmaps"));
    RWASSERT(mipArray);
    RWASSERT(totalMipLevels > 0);
    RWASSERT((logpalsize == 4) | (logpalsize == 8));

    /* first quick check they don't all have the same palette */
    if (RwImageGetPalette(mipArray[0]))
    {
        mipLevel = 1;
        while (mipLevel < totalMipLevels)
        {
            RwUInt32    *pal1;
            RwUInt32    *pal2;

            pal1 = ((RwUInt32 *)RwImageGetPalette(mipArray[0]));
            pal2 = ((RwUInt32 *)RwImageGetPalette(mipArray[mipLevel]));

            /* if any don't have palettes we need to match */
            if (!pal1 || !pal2)
            {
                mipLevel = 64;
                break;
            }
            for (i = 0; i < 1 << logpalsize; i++)
            {
                /* or the palettes are different we need to match */
                if (pal1[i] != pal2[i])
                {
                    mipLevel = 64;
                    break;
                }
            }

            mipLevel++;
        }

        /* they actually have identical palettes - no matching needed */
        if (mipLevel == totalMipLevels)
        {
            memcpy(MipPalette, RwImageGetPalette(mipArray[0]),
                   sizeof(RwRGBA) * (1UL << RwImageGetDepth(mipArray[0])));
            RWRETURN(TRUE);
        }
    }

    if (!RwPalQuantInit(&palQuant))
    {
        RWRETURN(FALSE);
    }

    /* add all pixels from all mips */
    mipLevel = 0;
    while (mipLevel < totalMipLevels)
    {
        RwPalQuantAddImage(&palQuant, mipArray[mipLevel], 1.0f);
        mipLevel++;
    }
    RwPalQuantResolvePalette(MipPalette, 1 << logpalsize, &palQuant);

    /* match images */
    mipLevel = 0;
    while (mipLevel < totalMipLevels)
    {
        RwImage            *dcimg = mipArray[mipLevel];
        RwImage            *palImage;

        palImage = RwImageCreate(RwImageGetWidth(dcimg),
                                 RwImageGetHeight(dcimg), logpalsize);
        if (palImage)
        {
            RwImageAllocatePixels(palImage);
            RwPalQuantMatchImage(RwImageGetPixels(palImage),
                                 RwImageGetStride(palImage),
                                 RwImageGetDepth(palImage),
                                 FALSE,
                                 &palQuant,
                                 dcimg);
            RwImageSetPalette(palImage, MipPalette);
            mipArray[mipLevel] = palImage;
            if (dcimg != sourceImage)
            {
                RwImageDestroy(dcimg);
            }
        }
        else
        {
            RWRETURN(FALSE);
        }

        mipLevel++;
    }
    RwPalQuantTerm(&palQuant);

    RWRETURN(TRUE);
}

/****************************************************************************/

static RwImage     *
TextureImageReadAndSize(const RwChar * name,
                        const RwChar * maskName,
                        RwInt32 format,
                        RwInt32 * rasterWidth,
                        RwInt32 * rasterHeight,
                        RwInt32 * rasterDepth, RwInt32 * rasterFlags)
{
    RwChar              mipName[256];
    RwChar              mipmaskName[256];
    const RwChar       *filenameExt;
    const RwChar       *masknameExt;
    RwImage            *image;

    RWFUNCTION(RWSTRING("TextureImageReadAndSize"));
    RWASSERT(name);

    RWSTRNSTORE(mipName, name);

    filenameExt = RwImageFindFileType(name);
    if (filenameExt)
    {
        rwstrcat(mipName, filenameExt);
    }

    mipmaskName[0] = ((RwChar) 0);

    if (maskName && maskName[0])
    {
        RWSTRNSTORE(mipmaskName, maskName);
        masknameExt = RwImageFindFileType(maskName);
        if (masknameExt)
        {
            rwstrcat(mipmaskName, masknameExt);
        }
    }

    image = RwImageReadMaskedImage(mipName, mipmaskName);
    if (!image)
    {
        RWRETURN((RwImage *)NULL);
    }

    /* do we need to fill in raster format info? */
    if ((*rasterWidth == 0) || (*rasterHeight == 0))
    {
        if (!RwImageFindRasterFormat(image, format,
                                     rasterWidth, rasterHeight,
                                     rasterDepth, rasterFlags))
        {
            RwImageDestroy(image);
            RWERROR((E_RW_INVIMAGEFORMAT));
            RWRETURN((RwImage *)NULL);
        }
    }

    /* do we need to resample? */
    if ((RwImageGetWidth(image) != *rasterWidth)
        || (RwImageGetHeight(image) != *rasterHeight))
    {
        RwImage            *resampledImage;
        RwImage            *origImage;
        RwInt32             originalDepth;

        /* and if we do, we'll need to expand up to 32bpp first */
        originalDepth = RwImageGetDepth(image);
        if (originalDepth != 32)
        {
            /* we'll reuse this later */
            origImage = image;

            image = RwImageCreate(RwImageGetWidth(origImage),
                                  RwImageGetHeight(origImage), 32);
            if (!image)
            {
                RwImageDestroy(origImage);
                RWRETURN((RwImage *)NULL);
            }
            if (!RwImageAllocatePixels(image))
            {
                RwImageDestroy(image);
                RwImageDestroy(origImage);
                RWRETURN((RwImage *)NULL);
            }
            RwImageCopy(image, origImage);
            RwImageDestroy(origImage);
        }

        resampledImage = RwImageCreate(*rasterWidth, *rasterHeight, 32);
        if (!resampledImage)
        {
            RwImageDestroy(image);
            RWRETURN((RwImage *)NULL);
        }
        if (!RwImageAllocatePixels(resampledImage))
        {
            RwImageDestroy(resampledImage);
            RwImageDestroy(image);
            RWRETURN((RwImage *)NULL);
        }

        RwImageResample(resampledImage, image);
        RwImageDestroy(image);
        image = resampledImage;

        /* If original image depth was 4 or 8, RE-PALLETIZE */
        if (4 == originalDepth)
        {
            PalettizeImage(&image, originalDepth);
        }
        else if (8 == originalDepth)
        {
            PalettizeImage(&image, originalDepth);
        }
    }

    RWRETURN(image);
}

/****************************************************************************/

/* Read just Mip Level #0 */
static RwTexture   *
TextureDefaultNormalRead(const RwChar * name, const RwChar * maskName)
{
    RwTexture          *texture;
    RwImage            *image;
    RwRaster           *raster;
    RwRGBA              MipPalette[256];
    RwChar              mipName[256];
    RwChar              mipmaskName[256];
    RwInt32             rasterWidth;
    RwInt32             rasterHeight;
    RwInt32             rasterDepth;
    RwInt32             rasterFlags;

    RWFUNCTION(RWSTRING("TextureDefaultNormalRead"));
    RWASSERT(name);

    /* Construct the filename */
    /* RWASSERT(rwstrlen(name) <
     * rwTEXTUREBASENAMELENGTH + rwTEXTUREMIPMAPNAMECHARS); */

    RWSTRNSTORE(mipName, name);

    /* And the maskname */
    mipmaskName[0] = ((RwChar) 0);
    if (maskName && maskName[0])
    {
        /* RWASSERT(rwstrlen(maskName) <
         * rwTEXTUREBASENAMELENGTH + rwTEXTUREMIPMAPNAMECHARS); */
        RWSTRNSTORE(mipmaskName, maskName);
    }

    RwTextureGenerateMipmapName(mipName, mipmaskName, 0,
                                rwRASTERTYPETEXTURE);

    rasterWidth = 0;
    rasterHeight = 0;
    image =
        TextureImageReadAndSize(mipName, mipmaskName,
                                rwRASTERTYPETEXTURE, &rasterWidth,
                                &rasterHeight, &rasterDepth,
                                &rasterFlags);
    if (!image)
    {
        RWRETURN((RwTexture *)NULL);
    }

    /* Create a raster */
    raster =
        RwRasterCreate(rasterWidth, rasterHeight, rasterDepth,
                       rasterFlags);
    if (!raster)
    {
        RwImageDestroy(image);
        RWRETURN((RwTexture *)NULL);
    }

    /* create a palette */
    if (RwRasterGetFormat(raster) &
        (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        if (RwRasterGetFormat(raster) & rwRASTERFORMATPAL4)
        {
            PalettizeMipmaps((RwRGBA *)MipPalette, (RwImage *)NULL, &image, 1, 4);
        }
        else
        {
            PalettizeMipmaps((RwRGBA *)MipPalette, (RwImage *)NULL, &image, 1, 8);
        }
        RwImageSetPalette(image, MipPalette);
    }

    RwImageGammaCorrect(image);

    /* Convert the image into the raster */
    if (!RwRasterSetFromImage(raster, image))
    {
        RwRasterDestroy(raster);
        RwImageDestroy(image);
        RWRETURN((RwTexture *)NULL);
    }

    RwImageDestroy(image);

    /* Create a texture */
    texture = RwTextureCreate(raster);
    if (!texture)
    {
        RwRasterDestroy(raster);
        RWRETURN((RwTexture *)NULL);
    }

    /* The name [and maskname] */
    RwTextureSetName(texture, name);
    if (maskName)
    {
        RwTextureSetMaskName(texture, maskName);
    }
    else
    {
        RwTextureSetMaskName(texture, RWSTRING(""));
    }

    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(texture);
}

/****************************************************************************/
static RwTexture   *
TextureDefaultMipmapRead(const RwChar * name, const RwChar * maskName)
{
    RwTexture          *texture;
    RwImage            *mipArray[16];
    RwRaster           *raster;
    RwRGBA              MipPalette[256];
    RwInt32             format;
    RwChar              mipName[256];
    RwChar              mipmaskName[256];
    RwInt32             rasterWidth;
    RwInt32             rasterHeight;
    RwInt32             rasterDepth;
    RwInt32             rasterFlags;

    RWFUNCTION(RWSTRING("TextureDefaultMipmapRead"));
    RWASSERT(name);

    /* Construct the filename */
    /* RWASSERT(rwstrlen(name) <
     * rwTEXTUREBASENAMELENGTH + rwTEXTUREMIPMAPNAMECHARS); */

    RWSTRNSTORE(mipName, name);

    /* And the maskname */
    mipmaskName[0] = ((RwChar) 0);
    if (maskName && maskName[0])
    {
        /* RWASSERT(rwstrlen(maskName) <
         * rwTEXTUREBASENAMELENGTH + rwTEXTUREMIPMAPNAMECHARS); */
        RWSTRNSTORE(mipmaskName, maskName);
    }

    format = rwRASTERTYPETEXTURE;

    if (RWTEXTUREGLOBAL(haveTexMipmaps))
    {
        format |= (RwInt32) rwRASTERFORMATMIPMAP;
        if (RWTEXTUREGLOBAL(generateTexMipmaps))
        {
            format |= (RwInt32) rwRASTERFORMATAUTOMIPMAP;
        }
    }

    RwTextureGenerateMipmapName(mipName, mipmaskName, 0, format);

    rasterWidth = 0;
    rasterHeight = 0;
    mipArray[0] = TextureImageReadAndSize(mipName, mipmaskName, format,
                                          &rasterWidth, &rasterHeight,
                                          &rasterDepth, &rasterFlags);

    /* have we a valid mip#0? */
    if (!mipArray[0])
    {
        RWRETURN((RwTexture *)NULL);
    }

    /* Create a raster */
    raster = RwRasterCreate(rasterWidth, rasterHeight, rasterDepth, rasterFlags);
    if (!raster)
    {
        RwImageDestroy(mipArray[0]);
        RWRETURN((RwTexture *)NULL);
    }

    if (rasterFlags & rwRASTERFORMATMIPMAP)
    {
        /* and now the mipmaps if device can support them */
        if (rasterFlags & rwRASTERFORMATAUTOMIPMAP)
        {
            /* Convert the image into the raster [auto mipmap and gamma] */
            if (!RwRasterSetFromImage(raster, mipArray[0]))
            {
                RwRasterDestroy(raster);
                RwImageDestroy(mipArray[0]);
                RWRETURN((RwTexture *)NULL);
            }
            RwImageDestroy(mipArray[0]);
        }
        else
        {
            /* if they weren't automatically generated; we'll read them */
            RwInt32             mipLevel, totalMipLevels;

            /* generate mip array */
            totalMipLevels = RwRasterGetNumLevels(raster);
            mipLevel = 1;
            while (mipLevel < totalMipLevels)
            {
                RWSTRNSTORE(mipName, name);

                /* And the maskname */
                mipmaskName[0] = ((RwChar) 0);
                if (maskName && maskName[0])
                {
                    RWSTRNSTORE(mipmaskName, maskName);
                }

                RwTextureGenerateMipmapName(mipName, mipmaskName,
                                            (RwUInt8) mipLevel, format);

                RwRasterLock(raster, (RwUInt8) mipLevel,
                             (RwRasterLockMode) ( rwRASTERLOCKWRITE |
                                                  rwRASTERLOCKNOFETCH) );
                rasterWidth = RwRasterGetWidth(raster);
                rasterHeight = RwRasterGetHeight(raster);
                rasterDepth = RwRasterGetDepth(raster);
                rasterFlags = RwRasterGetFormat(raster) | raster->cType;
                RwRasterUnlock(raster);

                mipArray[mipLevel] =
                    TextureImageReadAndSize(mipName, mipmaskName,
                                            format, &rasterWidth,
                                            &rasterHeight, &rasterDepth,
                                            &rasterFlags);
                if (!mipArray[mipLevel])
                {
                    while (--mipLevel >= 0)
                    {
                        RwImageDestroy(mipArray[mipLevel]);
                    }
                    RwRasterDestroy(raster);
                    RWRETURN((RwTexture *)NULL);
                }

                mipLevel++;
            }

            /* create a single palette */
            if (RwRasterGetFormat(raster) &
                (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
            {
                if (RwRasterGetFormat(raster) & rwRASTERFORMATPAL4)
                {
                    PalettizeMipmaps((RwRGBA *)MipPalette, (RwImage *)NULL,
                                     mipArray, totalMipLevels, 4);
                }
                else
                {
                    PalettizeMipmaps((RwRGBA *)MipPalette, (RwImage *)NULL,
                                     mipArray, totalMipLevels, 8);
                }

                /* once only on mip0 palette */
                RwImageGammaCorrect(mipArray[0]);
            }
            else
            {
                mipLevel = 0;
                while (mipLevel < totalMipLevels)
                {
                    RwImageGammaCorrect(mipArray[mipLevel]);
                    mipLevel++;
                }
            }

            /* assign to each mipmap */
            mipLevel = 0;
            while (mipLevel < totalMipLevels)
            {
                if (RwRasterLock (raster,
                                  (RwUInt8) mipLevel,
                                  (RwRasterLockMode) ( rwRASTERLOCKWRITE |
                                                       rwRASTERLOCKNOFETCH)) )
                {
                    if (!RwRasterSetFromImage
                        (raster, mipArray[mipLevel]))
                    {
                        while (mipLevel < totalMipLevels)
                        {
                            RwImageDestroy(mipArray[mipLevel]);
                            mipLevel++;
                        }
                        RwRasterDestroy(raster);
                        RWRETURN((RwTexture *)NULL);
                    }
                    RwRasterUnlock(raster);
                }

                RwImageDestroy(mipArray[mipLevel]);
                mipLevel++;
            }
        }
    }
    else
    {
        RwImageGammaCorrect(mipArray[0]);

        /* Convert the image into the raster */
        if (!RwRasterSetFromImage(raster, mipArray[0]))
        {
            RwRasterDestroy(raster);
            RwImageDestroy(mipArray[0]);
            RWRETURN((RwTexture *)NULL);
        }
        RwImageDestroy(mipArray[0]);
    }

    /* Create a texture */
    texture = RwTextureCreate(raster);
    if (!texture)
    {
        RwRasterDestroy(raster);
        RWRETURN((RwTexture *)NULL);
    }

    /* The name [and maskname] */
    RwTextureSetName(texture, name);
    if (maskName)
    {
        RwTextureSetMaskName(texture, maskName);
    }
    else
    {
        RwTextureSetMaskName(texture, RWSTRING(""));
    }

    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(texture);
}

/****************************************************************************
TextureDefaultRead

 On entry   : Name
            : Masks name
 On exit    :
 */

/* this function simply chooses which method
 * is appropriate for reading the texture */
static RwTexture   *
TextureDefaultRead(const RwChar * name, const RwChar * maskName)
{
    RwTexture          *texture = (RwTexture *)NULL;

    RWFUNCTION(RWSTRING("TextureDefaultRead"));

    if (RWTEXTUREGLOBAL(haveTexMipmaps))
    {
        texture = (TextureDefaultMipmapRead(name, maskName));
    }
    else
    {
        texture = (TextureDefaultNormalRead(name, maskName));
    }

    RWRETURN(texture);
}

/****************************************************************************/
static RwRaster    *
TextureRasterDefaultBuildMipmaps(RwRaster * raster,
                                 RwImage * sourceImage)
{
    RwInt32             mipLevel, totalMipLevels;
    RwImage            *mipArray[16];
    RwInt32             rasterWidth;
    RwInt32             rasterHeight;
    RwInt32             rasterDepth;
    RwUInt32            autoMipmap;
    RwRGBA              MipPalette[256];

    RWFUNCTION(RWSTRING("TextureRasterDefaultBuildMipmaps"));
    RWASSERT(raster);

    RWASSERT(raster->cFormat & (rwRASTERFORMATMIPMAP >> 8));

    rasterWidth = RwRasterGetWidth(raster);
    rasterHeight = RwRasterGetHeight(raster);
    rasterDepth = RwRasterGetDepth(raster);
    if (sourceImage == NULL)
    {
        mipArray[0] = RwImageCreate(rasterWidth, rasterHeight, 32);
        if (mipArray[0])
        {
            if (RwImageAllocatePixels(mipArray[0]) == NULL)
                RWRETURN((RwRaster *) NULL);

            RwImageSetFromRaster(mipArray[0], raster);
        }
    }
    else
    {
        if (RwImageGetDepth(sourceImage) != 32)
        {
            mipArray[0] = RwImageCreate(rasterWidth, rasterHeight, 32);
            if (mipArray[0])
            {
                if (RwImageAllocatePixels(mipArray[0]) == NULL)
                    RWRETURN((RwRaster *) NULL);
                RwImageCopy(mipArray[0], sourceImage);
            }
        }
        else
        {
            mipArray[0] = sourceImage;
        }
    }

    /* have we got a valid Mip#0? */
    if (mipArray[0] == NULL)
    {
        RWRETURN((RwRaster *)NULL);
    }

    /* remove AUTOMIPMAP flag to avoid Unlock invoking mipmap create */
    autoMipmap = raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8);
    raster->cFormat &= ~autoMipmap;

    /* generate mip array */
    totalMipLevels = RwRasterGetNumLevels(raster);
    mipLevel = 1;
    while (mipLevel < totalMipLevels)
    {
        mipArray[mipLevel] = (RwImage *)NULL;
        if (RwRasterLock(raster, (RwUInt8) mipLevel, rwRASTERLOCKREAD))
        {
            mipArray[mipLevel] =
                RwImageCreateResample(mipArray[mipLevel - 1],
                                      RwRasterGetWidth(raster),
                                      RwRasterGetHeight(raster));
            RwRasterUnlock(raster);
        }

        /* back out and exit */
        if (mipArray[mipLevel] == NULL)
        {
            while (--mipLevel >= 0)
            {
                if (mipArray[mipLevel] != sourceImage)
                {
                    RwImageDestroy(mipArray[mipLevel]);
                }
            }
            raster->cFormat |= autoMipmap;
            RWRETURN((RwRaster *)NULL);
        }

        mipLevel++;
    }

    /* create a single palette */
    if (RwRasterGetFormat(raster) &
        (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        if (RwRasterGetFormat(raster) & rwRASTERFORMATPAL4)
        {
            if (!PalettizeMipmaps
                (MipPalette, sourceImage, mipArray, totalMipLevels, 4))
            {
                for (mipLevel = 0; mipLevel < totalMipLevels;
                     mipLevel++)
                {
                    if (mipArray[mipLevel] != sourceImage)
                    {
                        RwImageDestroy(mipArray[mipLevel]);
                    }
                    raster->cFormat |= autoMipmap;
                    RWRETURN((RwRaster *)NULL);
                }
            }
        }
        else
        {
            if (!PalettizeMipmaps
                (MipPalette, sourceImage, mipArray, totalMipLevels, 8))
            {
                for (mipLevel = 0; mipLevel < totalMipLevels;
                     mipLevel++)
                {
                    if (mipArray[mipLevel] != sourceImage)
                    {
                        RwImageDestroy(mipArray[mipLevel]);
                    }
                    raster->cFormat |= autoMipmap;
                    RWRETURN((RwRaster *)NULL);
                }
            }
        }

        /* once only on mip0 palette */
        RwImageGammaCorrect(mipArray[0]);
    }
    else
    {
        mipLevel = 0;
        while (mipLevel < totalMipLevels)
        {
            RwImageGammaCorrect(mipArray[mipLevel]);
            mipLevel++;
        }
    }

    /* assign to each mipmap */
    mipLevel = 0;
    while (mipLevel < totalMipLevels)
    {
        if (RwRasterLock(raster,
                         (RwUInt8) mipLevel,
                         (RwRasterLockMode) ( rwRASTERLOCKWRITE |
                                              rwRASTERLOCKNOFETCH)) )
        {
            RwRasterSetFromImage(raster, mipArray[mipLevel]);
            RwRasterUnlock(raster);
        }

        if (mipArray[mipLevel] != sourceImage)
        {
            RwImageDestroy(mipArray[mipLevel]);
        }

        mipLevel++;
    }

    /* restore AUTOMIPMAP flags */
    raster->cFormat |= autoMipmap;
    RWRETURN(raster);
}


/****************************************************************************
 TextureAnnihilate

 On entry   : texture, user data pointer
 On exit    : texture pointer on success
 */

static              RwBool
TextureAnnihilate(RwTexture * texture)
{
    RWFUNCTION(RWSTRING("TextureAnnihilate"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(texture->refCount <= 0);

    /* Temporarily bump up reference count to avoid assertion failures */
    texture->refCount++;

    RWASSERT(texture->refCount > 0);

    /* De-initialize the plugin memory */
    _rwPluginRegistryDeInitObject(&textureTKList, texture);

    if (texture->dict)
    {
        rwLinkListRemoveLLLink(&texture->lInDictionary);
    }

    if (texture->raster)
    {
        /* We still have the raster to destroy */
        RwRasterDestroy(texture->raster);
        texture->raster = (RwRaster *)NULL;
    }

    /* Reinstate reference count */
    --texture->refCount;
    RWASSERT(texture->refCount <= 0);

    RwFreeListFree(RWTEXTUREGLOBAL(textureFreeList), texture);

    RWRETURN(TRUE);
}

/****************************************************************************
 StringCompare

 On entry   : string A
            : string B
 On exit    : TRUE if they are the same
 */

static              RwBool
StringCompare(const RwChar * cpA, const RwChar * cpB)
{
    RWFUNCTION(RWSTRING("StringCompare"));
    RWASSERT(cpA);
    RWASSERT(cpB);

    while ((*cpA) && (*cpB))
    {
        RwChar              cA = (*cpA);
        RwChar              cB = (*cpB);

        if ((cA >= 'a') && (cA <= 'z'))
        {
            cA += 'A' - 'a';
        }

        if ((cB >= 'a') && (cB <= 'z'))
        {
            cB += 'A' - 'a';
        }

        if (cA != cB)
        {
            RWRETURN(FALSE);
        }

        cpA++;
        cpB++;
    }

    if ((*cpA) == (*cpB))
    {
        RWRETURN(TRUE);
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwTextureFind

 On entry   : Name
 On exit    : Texture pointer if found
 */

static RwTexture *
TextureDefaultFind(const RwChar *name)
{
    RwTexDictionary    *dict;
    RwTexture          *result;

    RWFUNCTION(RWSTRING("TextureDefaultFind"));
    RWASSERT(name);

    dict = RWTEXTUREGLOBAL(currentTexDict);
    if (dict)
    {
        result = RwTexDictionaryFindNamedTexture(dict, name);
        RWRETURN(result);
    }
    else
    {
        RwLLLink           *cur, *end;

        cur = rwLinkListGetFirstLLLink(&RWTEXTUREGLOBAL(texDictList));
        end = rwLinkListGetTerminator(&RWTEXTUREGLOBAL(texDictList));
        while (cur != end)
        {
            dict = rwLLLinkGetData(cur, RwTexDictionary, lInInstance);
            result = RwTexDictionaryFindNamedTexture(dict, name);
            if (result)
            {
                RWASSERT(0 < result->refCount);
                RWRETURN(result);
            }

            cur = rwLLLinkGetNext(cur);
        }
    }

    RWRETURN((RwTexture *)NULL);
}


/**
 * \ingroup rwtexture
 * \ref RwTextureSetFindCallBack is used to override the callback that
 * will be used by \ref RwTextureRead to search for a texture
 * in memory before attempting to read one from an image file on disk.
 *
 * The default find function searches the current texture dictionary if one
 * is set, or otherwise searches all texture dictionaries in the system. A
 * user specified callback may implement other types of behavior such as
 * searching a subset of texture dictionaries that are relevant to a
 * particular part of a game.
 *
 * \param callBack  A pointer to the texture find function
 *
 * \return Returns TRUE
 *
 * \see RwTextureGetFindCallBack
 * \see RwTextureSetReadCallBack
 * \see RwTextureGetReadCallBack
 * \see RwTextureRead
 * \see RwImageRead
 *
 */
RwBool
RwTextureSetFindCallBack(RwTextureCallBackFind callBack)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetFindCallBack"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(callBack);

    RWTEXTUREGLOBAL(textureFindFunc) = callBack;
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetFindCallBack is used to enquire which function is
 * currently used by \ref RwTextureRead to search for textures in memory
 * before an attempt is made to read them from an image file on disk.
 *
 * \return Returns a pointer to the current texture find function
 *
 * \see RwTextureSetFindCallBack
 * \see RwTextureSetReadCallBack
 * \see RwTextureGetReadCallBack
 * \see RwTextureRead
 * \see RwImageRead
 *
 */
RwTextureCallBackFind
RwTextureGetFindCallBack(void)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetFindCallBack"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(textureFindFunc));
}


/**
 * \ingroup rwtexture
 * \ref RwTextureSetReadCallBack is used to override the function that
 * will be used to read textures from image files.
 *
 * The default texture loading mechanism can be used to read any image formats
 * that have been registered with \ref RwImageRegisterImageFormat.
 *
 * \param callBack  A pointer to the texture-reading function
 *
 * \return Returns TRUE
 *
 * \see RwTextureGetReadCallBack
 * \see RwTextureSetFindCallBack
 * \see RwTextureGetFindCallBack
 * \see RwTextureRead
 * \see RwImageRead
 *
 */
RwBool
RwTextureSetReadCallBack(RwTextureCallBackRead callBack)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetReadCallBack"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(callBack);

    RWTEXTUREGLOBAL(textureReadFunc) = callBack;
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetReadCallBack is used to enquire which function is
 * currently used for reading textures from image files.
 *
 * \return Returns a pointer to the user defined texture reading function
 *
 * \see RwTextureSetReadCallBack
 * \see RwTextureSetFindCallBack
 * \see RwTextureGetFindCallBack
 * \see RwTextureRead
 * \see RwImageRead
 *
 */
RwTextureCallBackRead
RwTextureGetReadCallBack(void)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetReadCallBack"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(textureReadFunc));
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Mipmapping of textures

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexture
 * \ref RwTextureSetMipmapping is used to indicate whether mipmaps will
 * be available whenever a texture is read from an image file. Use this
 * function to turn-on and turn-off creation of textures with mipmaps.
 *
 * The default setting is FALSE, i.e. textures will be created without
 * mipmaps.
 *
 * \note This function affects only textures loaded with
 *       \ref RwTextureRead. Textures loaded indirectly using
 *       \ref RpWorldStreamRead or \ref RpClumpStreamRead have mipmaps
 *       processed in a different manner. In these cases, the
 *       filter mode stored in the binary file affects whether mipmaps are
 *       created or not. Artists can control this using the filter modes
 *       assigned to each texture in the modeler.  For example, in 3ds max if
 *       a texture is marked for "pyramidal" filtering, on export this is
 *       converted to trilinear, implying a mipmap stack is needed.
 *
 * \param enable  An \ref RwBool value equal to TRUE if mipmaps are required
 *                or FALSE if they are not.
 *
 * \return Returns TRUE
 *
 * \see RwTextureGetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureGetAutoMipmapping
 * \see RwTextureRead
 *
 */
RwBool
RwTextureSetMipmapping(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetMipmapping"));
    RWASSERT(textureModule.numInstances);

    RWTEXTUREGLOBAL(haveTexMipmaps) = enable;
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetMipmapping is used to enquire whether mipmaps will
 * be available when reading textures.
 *
 * The default setting is FALSE, i.e. mipmaps are not available.
 *
 * \return Returns TRUE if mipmaps are available or FALSE if they are not
 *
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureGetAutoMipmapping
 * \see RwTextureRead
 *
 */
RwBool
RwTextureGetMipmapping(void)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetMipmapping"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(haveTexMipmaps));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetAutoMipmapping is used to indicate whether mipmaps
 * (if enabled) should have their contents generated automatically whenever a
 * texture is read from a file, or whether the mipmaps should be read from separate
 * files. If reading from separate files the default naming convention is to postfix
 * the texture and mask name with "mn" where n is the index of the mipmap level, i.e.
 * m0, m1, m2 etc. \ref RwTextureSetMipmapNameCallBack can be called to change the
 * way the mipmap names are generated.
 * Use this function to choose between automatically generating mipmaps (TRUE)
 * or reading mipmaps (FALSE).
 *
 * The default setting is FALSE, i.e. the mipmaps will be read.
 *
 * This setting is only used if mipmaps are available.
 *
 * \param enable  An \ref RwBool value equal to TRUE if mipmaps are to be generated or FALSE
 * if they are to be read from separate files
 *
 * \return Returns TRUE
 *
 * \see RwTextureGetAutoMipmapping
 * \see RwTextureSetMipmapping
 * \see RwTextureGetMipmapping
 * \see RwTextureRead
 *
 */
RwBool
RwTextureSetAutoMipmapping(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetAutoMipmapping"));
    RWASSERT(textureModule.numInstances);

    RWTEXTUREGLOBAL(generateTexMipmaps) = enable;
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetAutoMipmapping is used to enquire whether mipmaps are
 * automatically generated when reading a texture or also read in from
 * separate files.
 *
 * The default setting is FALSE, i.e. the mipmaps will be read in also.
 *
 * This setting is only used if mipmaps are available.
 *
 * \return Returns TRUE if mipmaps are automatically generated and FALSE if they
 * are to be read in from separate files
 *
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureSetMipmapping
 * \see RwTextureGetMipmapping
 * \see RwTextureRead
 *
 */
RwBool
RwTextureGetAutoMipmapping(void)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetAutoMipmapping"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(generateTexMipmaps));
}

/* Legacy functionality left in for now
 */

/****************************************************************************
 *
 * Autoduck comment removed from obsolete function
 *
 */
RwBool
_rwTextureSetAutoMipMapState(RwBool enable)
{
    RWFUNCTION(RWSTRING("_rwTextureSetAutoMipMapState"));
    RWASSERT(textureModule.numInstances);

    RwTextureSetMipmapping(enable);
    RwTextureSetAutoMipmapping(enable);

    RWRETURN(TRUE);
}

/****************************************************************************
 *
 * Autoduck comment removed from obsolete function
 *
 */
RwBool
_rwTextureGetAutoMipMapState(void)
{
    RWFUNCTION(RWSTRING("_rwTextureGetAutoMipMapState"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(generateTexMipmaps));
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Setting and getting texture rasters

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexture
 * \ref RwTextureSetRaster is used to specify a raster that will store
 * the pixels of the given texture. To be used as a texture the raster must
 * have been created with either type flag rwRASTERTYPETEXTURE or
 * rwRASTERTYPECAMERATEXTURE and to be of a size
 * and depth compatible with the particular device it is created for.
 *
 * \param texture  A pointer to the texture whose raster will be set
 * \param raster  A pointer to the raster
 *
 * \return Returns a pointer to the texture if successful or NULL if there is
 * an error
 *
 * \see RwTextureGetRaster
 * \see RwTextureCreate
 * \see RwRasterCreate
 *
 */
RwTexture *
RwTextureSetRaster(RwTexture *texture, RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetRaster"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    if (raster)
    {
        RWASSERT(((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPETEXTURE) ||
                 ((raster->cType & rwRASTERTYPEMASK)
                  == rwRASTERTYPECAMERATEXTURE) ||
                 ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERA));

        if (RWSRCGLOBAL(stdFunc[rwSTANDARDTEXTURESETRASTER])
            (texture, raster, 0))
        {
            RWRETURN(texture);
        }

        /* Device failure */
        RWRETURN((RwTexture *)NULL);
    }
    else
    {
        texture->raster = (RwRaster *)NULL;
        RWRETURN(texture);
    }
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwtexture
 * \ref RwTextureGetRaster is used to retrieve the raster associated with
 * the specified texture.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture to query
 *
 * \return Returns a pointer to the raster
 *
 * \see RwTextureSetRaster
 * \see RwTextureCreate
 *
 */
RwRaster *
RwTextureGetRaster(const RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetRaster"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureGetRasterMacro(texture));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Texture dictionaries

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryCreate is used to create a new texture dictionary
 * for storing textures.
 *
 * \return Returns a pointer to newly created texture dictionary if successful
 * or NULL if there is an error
 *
 * \see RwTexDictionaryDestroy
 * \see RwTexDictionaryAddTexture
 * \see RwTexDictionaryRemoveTexture
 *
 */
RwTexDictionary *
RwTexDictionaryCreate(void)
{
    RwTexDictionary    *dict;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryCreate"));
    RWASSERT(textureModule.numInstances);

    RWASSERT(RWTEXTUREGLOBAL(texDictFreeList));
    dict =
        (RwTexDictionary *)
        RwFreeListAlloc(RWTEXTUREGLOBAL(texDictFreeList),
            rwMEMHINTDUR_EVENT | rwID_TEXDICTIONARY);
    if (!dict)
    {
        RWRETURN((RwTexDictionary *)NULL);
    }

    rwObjectInitialize(dict, rwTEXDICTIONARY, 0);

    rwLinkListAddLLLink(&RWTEXTUREGLOBAL(texDictList),
                        &dict->lInInstance);
    rwLinkListInitialize(&dict->texturesInDict);

    /* Initialize the plugin memory */
    _rwPluginRegistryInitObject(&texDictTKList, dict);

    RWRETURN(dict);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryDestroy is used to destroy the specified texture
 * dictionary. If the dictionary is the current one, the current dictionary
 * is set to NULL.
 *
 * Any objects that reference textures in the dictionary must be destroyed
 * first. The only textures that should then remain in the dictionary are
 * those that were either streamed in with the dictionary, or forced to
 * be persistent by incrementing their reference count. The reference count
 * should be 1. \ref RwTexDictionaryDestroy will call \ref RwTextureDestroy
 * for these remaining textures so that they will be removed from the system.
 *
 * \param dict  A pointer to the texture dictionary to destroy
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwTexDictionaryCreate
 * \see RwTexDictionaryRemoveTexture
 * \see RwTextureDestroy
 *
 */
RwBool
RwTexDictionaryDestroy(RwTexDictionary *dict)
{
    RWAPIFUNCTION(RWSTRING("RwTexDictionaryDestroy"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(dict);
    RWASSERTISTYPE(dict, rwTEXDICTIONARY);

    if (RWTEXTUREGLOBAL(currentTexDict) == dict)
    {
        /* It was the current, so make it not */
        RWTEXTUREGLOBAL(currentTexDict) = (RwTexDictionary *)NULL;
    }

    /* Destroy all the textures */
    RwTexDictionaryForAllTextures(dict,
                                  (RwTextureCallBack)RwTextureDestroy,
                                  NULL);

    /* De-initialize the plugin memory */
    _rwPluginRegistryDeInitObject(&texDictTKList, dict);

    /* Remove from the free list */
    rwLinkListRemoveLLLink(&dict->lInInstance);

    /* No longer need this entry */
    RwFreeListFree(RWTEXTUREGLOBAL(texDictFreeList), dict);

    /* Success */
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryForAllTextures is used to execute the given
 * callback function on all textures in the specified texture dictionary.
 *
 * The format of the callback function is:
 *
 * \verbatim
   RwTexture * (*RwTextureCallBack) (RwTexture *texture, void *data)
   \endverbatim
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * \note Note that if any invocation of the callback function returns a failure status
 * the iteration is terminated.  However, \ref RwTexDictionaryForAllTextures
 * will still return successfully.
 *
 * \param dict  A pointer to the texture dictionary
 * \param fpCallBack  A pointer to the function to call with each texture in the dictionary
 * \param pData  A user data pointer to pass to the callback function
 *
 * \return Returns a pointer to the texture dictionary
 *
 * \see RwTexDictionaryForAllTexDictionaries
 * \see RwTexDictionaryFindNamedTexture
 */
const RwTexDictionary *
RwTexDictionaryForAllTextures(const RwTexDictionary* dict,
                              RwTextureCallBack fpCallBack, void *pData)
{
    RwLLLink           *cur, *next;
    const RwLLLink     *end;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryForAllTextures"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(dict);
    RWASSERTISTYPE(dict, rwTEXDICTIONARY);
    RWASSERT(fpCallBack);

    end = rwLinkListGetTerminator(&dict->texturesInDict);
    cur = rwLinkListGetFirstLLLink(&dict->texturesInDict);

    while (cur != end)
    {
        RwTexture          *texture;

        next = rwLLLinkGetNext(cur);

        texture = rwLLLinkGetData(cur, RwTexture, lInDictionary);
        RWASSERT(texture);
        RWASSERT(0 < texture->refCount);

        if (!fpCallBack(texture, pData))
        {
            /* Early out */
            break;
        }

        cur = next;
    }

    /* All OK */
    RWRETURN(dict);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryForAllTexDictionaries is used to execute the given
 * callback function on all currently existing texture dictionaries.
 *
 * The format of the callback function is:
 *
 * \verbatim
   RwTexDictionary * (*RwTexDictionaryCallBack) (RwTexDictionary *dict, void *data)
   \endverbatim
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * \note Note that if any invocation of the callback function returns a failure status
 * the iteration is terminated.  However, \ref RwTexDictionaryForAllTexDictionaries
 * will still return successfully. Also, the callback function may safely destroy
 * the current texture dictionary without adversely affecting the iteration process.
 *
 * \param fpCallBack  a pointer to the function to call with each texture dictionary
 * \param pData  A user data pointer to pass to callback function
 *
 * \return Returns TRUE on success, FALSE otherwise
 *
 * \see RwTexDictionaryForAllTextures
 * \see RwTexDictionaryFindNamedTexture
 */
RwBool
RwTexDictionaryForAllTexDictionaries(RwTexDictionaryCallBack fpCallBack,
                                     void *pData)
{
    RwLLLink           *cur, *next;
    const RwLLLink     *end;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryForAllTexDictionaries"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(fpCallBack);

    cur = rwLinkListGetFirstLLLink(&RWTEXTUREGLOBAL(texDictList));
    end = rwLinkListGetTerminator(&RWTEXTUREGLOBAL(texDictList));

    while (cur != end)
    {
        RwTexDictionary    *dict = (rwLLLinkGetData(cur,
                                                    RwTexDictionary,
                                                    lInInstance));
        RWASSERTISTYPE(dict, rwTEXDICTIONARY);

        next = rwLLLinkGetNext(cur);

        /* NOTE: callbacks can safely destroy the texDict
         * given we cache next before calling them :) */
        if (!fpCallBack(dict, pData))
        {
            /* Early out */
            break;
        }

        cur = next;
    }

    RWRETURN(TRUE);
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Texture handling

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexture
 * \ref RwTextureCreate is used to create a new texture from the specified
 * raster. The raster must have been created with type flag rwRASTERTYPETEXTURE
 * or rwRASTERTYPECAMERATEXTURE to be used as a texture and to be of a size and
 * depth compatible with the particular device it is created for. If the
 * texture is added to a texture dictionary it is recommended that the texture
 * be given a name so that it can be identified when the dictionary is searched.
 * On creation the texture has a reference count of one.
 *
 * \note Newly created textures have the following default values:
 * \li filtering - rwFILTERNEAREST
 * \li u address - rwTEXTUREADDRESSWRAP
 * \li v address - rwTEXTUREADDRESSWRAP
 *
 * \param raster  A pointer to the raster to use as a texture
 *
 * \return Returns a pointer to the newly created texture if successful or NULL
 * if there is an error
 *
 * \see RwRasterCreate
 * \see RwTexDictionaryAddTexture
 * \see RwTexDictionarySetCurrent
 * \see RwTextureSetName
 * \see RwTextureDestroy
 *
 */
RwTexture *
RwTextureCreate(RwRaster *raster)
{
    RwTexture   *texture;

    RWAPIFUNCTION(RWSTRING("RwTextureCreate"));
    RWASSERT(textureModule.numInstances);

#if defined(RWDEBUG)
    if (raster)
    {
        RWASSERT((raster->cType == rwRASTERTYPETEXTURE) ||
                 (raster->cType == rwRASTERTYPECAMERATEXTURE) ||
                 (raster->cType == rwRASTERTYPECAMERA));
    }
#endif /* defined(RWDEBUG) */

    RWASSERT(RWTEXTUREGLOBAL(textureFreeList));

    texture = (RwTexture *)RwFreeListAlloc(RWTEXTUREGLOBAL(textureFreeList),
                                    rwMEMHINTDUR_EVENT | rwID_TEXTURE);
    if (texture)
    {
        texture->dict = (RwTexDictionary *)NULL;

        texture->name[0] = ((RwChar) 0);
        texture->mask[0] = ((RwChar) 0);

        texture->raster = raster;  /* Set the raster */

        texture->refCount = 1;     /* One reference so far */

        texture->filterAddressing = 0;

        /* Default addressing */
        RwTextureSetAddressing(texture, rwTEXTUREADDRESSWRAP);

        /* Default sampling */
        RwTextureSetFilterMode(texture, rwFILTERNEAREST);

        /* Initialize the plugin memory */
        _rwPluginRegistryInitObject(&textureTKList, texture);
    }

    RWRETURN(texture);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwtexture
 * \ref RwTextureAddRef is used to increment the reference count of the
 * specified texture. Use this function to indicate that a new object depends
 * on the texture's existence. This ensures that the texture is not
 * lost when other objects that use this texture are destroyed.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  The pointer to the texture to increment reference count
 *
 * \return Returns a pointer to the texture
 *
 * \see RwTextureDestroy
 * \see RpMaterialSetTexture
 *
 */
RwTexture *
RwTextureAddRef(RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureAddRef"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureAddRefMacro(texture));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwtexture
 * \ref RwTextureDestroy is used to destroy the specified texture. If
 * the texture is defined in a texture dictionary the texture is removed from
 * that dictionary. The raster associated with the texture is also destroyed
 * by this function.
 *
 * \note Only textures whose reference counts are zero are actually removed
 * from the system. A reference count greater than one indicates that its
 * particular texture is still being used by one or more objects. Hence, this
 * function will only decrement the texture's reference count if this is
 * the case.
 *
 * \param texture  A pointer to the texture to destroy
 *
 * \return Returns TRUE if the texture has been destroyed or FALSE if
 * there is an error
 *
 * \see RwTextureCreate
 * \see RwTextureRead
 * \see RwTextureAddRef
 *
 */
RwBool
RwTextureDestroy(RwTexture *texture)
{
    RwBool              result = TRUE;

    RWAPIFUNCTION(RWSTRING("RwTextureDestroy"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < texture->refCount);

    /* RWCRTCHECKMEMORY(); */
    --texture->refCount;
    /* RWCRTCHECKMEMORY(); */

    if (texture->refCount <= 0)
        result = TextureAnnihilate(texture);

    /* All done */
    RWRETURN(result);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwtexture
 * \ref RwTextureGetName is used to retrieve the name of the specified
 * texture.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture to query.
 *
 * \return Returns a pointer to a string equal to the name of the texture which
 * may be an empty string
 *
 * \see RwTextureGetMaskName
 * \see RwTextureSetName
 * \see RwTextureSetMaskName
 * \see RwTextureCreate
 * \see RwTextureRead
 *
 */
RwChar *
RwTextureGetName(RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetName"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureGetNameMacro(texture));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetMaskName is used to retrieve the mask name of the
 * specified texture. The mask name is usually only defined if the texture
 * has been created by reading an image file from disk together with a mask
 * image file. Otherwise, the mask name is an empty string.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture to query
 *
 * \return Returns a pointer to a string equal to the mask name if successful
 * or NULL if there is an error or if the texture has no mask name
 *
 * \see RwTextureGetName
 * \see RwTextureSetMaskName
 * \see RwTextureSetName
 * \see RwTextureCreate
 * \see RwTextureRead
 *
 */
RwChar *
RwTextureGetMaskName(RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetMaskName"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureGetMaskNameMacro(texture));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwtexture
 * \ref RwTextureSetName is used to give the specified texture the given
 * name. Textures with names can be searched for in texture dictionaries.
 *
 * \param texture  A pointer to the texture whose name will be defined
 * \param name  A pointer to the texture's name to set
 *
 * \return Returns a pointer to the texture
 *
 * \see RwTextureGetName
 * \see RwTextureSetMaskName
 * \see RwTextureCreate
 * \see RwTextureRead
 *
 */
RwTexture *
RwTextureSetName(RwTexture *texture, const RwChar *name)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetName"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));
    RWASSERT(name);

    /* RWASSERT(rwstrlen(name) < rwTEXTUREBASENAMELENGTH); */

    RWSTRNSTORE(texture->name, name);

    RWRETURN(texture);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetMaskName is used to give the specified texture the
 * given mask name.
 *
 * \param texture  A pointer to the texture whose mask name will be defined
 * \param maskName  A pointer to the texture's mask name to set.
 *
 * \return Returns a pointer to the texture
 *
 * \see RwTextureGetMaskName
 * \see RwTextureSetName
 * \see RwTextureCreate
 * \see RwTextureRead
 *
 */
RwTexture *
RwTextureSetMaskName(RwTexture *texture, const RwChar *maskName)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetMaskName"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));
    RWASSERT(maskName);

    /* RWASSERT(rwstrlen(maskName) < rwTEXTUREBASENAMELENGTH); */

    RWSTRNSTORE(texture->mask, maskName);

    RWRETURN(texture);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwtexture
 * \ref RwTextureGetDictionary is used to determine the texture
 * dictionary that the specified texture belongs to.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture to query.
 *
 * \return Returns a pointer to the texture dictionary if successful or NULL if
 * the texture does not belong to any dictionary
 *
 * \see RwTexDictionaryAddTexture
 * \see RwTexDictionaryRemoveTexture
 *
 */
RwTexDictionary *
RwTextureGetDictionary(RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetDictionary"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureGetDictionaryMacro(texture));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryAddTexture is used to add the specified texture
 * to the given texture dictionary. It is recommended that the texture is
 * named so that it can be identified when searching the texture
 * dictionaries.
 *
 * \note If the texture already belongs to another texture dictionary,
 * the texture is first removed from that dictionary before being added to the
 * new one.  A texture may only be a member of one texture dictionary.
 *
 * The texture does not receive a reference count for its dictionary entry
 * (see \ref rwtexdictoverview).
 *
 * \param dict  A pointer to the texture dictionary that will receive the texture
 * \param texture  A pointer to the texture that will be added to the dictionary
 *
 * \return Returns a pointer to the texture
 *
 * \see RwTexDictionaryRemoveTexture
 * \see RwTextureGetDictionary
 * \see RwTextureSetName
 *
 */
RwTexture *
RwTexDictionaryAddTexture(RwTexDictionary *dict, RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTexDictionaryAddTexture"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));
    RWASSERT(dict);
    RWASSERTISTYPE(dict, rwTEXDICTIONARY);

#ifdef RWDEBUG

    /* Check if the texture's name is unique in the texture dictionary. */
    {
        if (RwTexDictionaryFindNamedTexture(dict, texture->name))
        {
            RwChar      msg[80];

            /* Found a texture with the same name. */
            rwsprintf(msg, "Texture name \"%s\" already exist in the dictionary.",
                texture->name);

            RWMESSAGE(RWSTRING((msg)));
        }
    }

#endif /* RWDEBUG */

    if (texture->dict)
    {
        rwLinkListRemoveLLLink(&texture->lInDictionary);
    }

    texture->dict = dict;

    rwLinkListAddLLLink(&dict->texturesInDict, &texture->lInDictionary);

    RWRETURN(texture);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryRemoveTexture is used to remove the specified
 * texture from the texture dictionary it is a member of. Removing the
 * texture from the dictionary does not actually destroy it, so if this is
 * the intention, use \ref RwTextureDestroy in place of this function
 * (see also the discussion in \ref rwtexdictoverview).
 *
 * \param texture  A pointer to the texture to remove from dictionary
 *
 * \return Returns a pointer to the texture
 *
 * \see RwTexDictionaryAddTexture
 * \see RwTextureGetDictionary
 * \see RwTextureDestroy
 *
 */
RwTexture *
RwTexDictionaryRemoveTexture(RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTexDictionaryRemoveTexture"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    if (texture->dict)
    {
        texture->dict = (RwTexDictionary *)NULL;
        rwLinkListRemoveLLLink(&texture->lInDictionary);
    }

    RWRETURN(texture);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryFindNamedTexture is used to find the specified
 * named texture in the given texture dictionary. The search is only performed
 * on the texture's name and not on the texture's pointer. Hence, the texture
 * must have been named prior to this function call.
 *
 * \param dict  Pointer to the texture dictionary that will be searched.
 * \param name  Pointer to the texture name that will be searched for.
 *
 * \return Returns a pointer to the texture if successful or NULL if there is
 * an error or if the texture was not found
 *
 * \see RwTextureSetName
 * \see RwTextureCreate
 * \see RwTextureRead
 * \see RwTexDictionaryForAllTextures
 *
 */
RwTexture *
RwTexDictionaryFindNamedTexture(RwTexDictionary *dict, const RwChar *name)
{
    RwTexture          *result;
    RwLLLink           *cur, *end;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryFindNamedTexture"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(dict);
    RWASSERTISTYPE(dict, rwTEXDICTIONARY);
    RWASSERT(name);

    cur = rwLinkListGetFirstLLLink(&dict->texturesInDict);
    end = rwLinkListGetTerminator(&dict->texturesInDict);

    while (cur != end)
    {
        result = rwLLLinkGetData(cur, RwTexture, lInDictionary);

        if (result->name)
        {
            if (StringCompare(result->name, name))
            {
                /* Gotcha */
                RWASSERT(result);
                RWASSERT(0 < result->refCount);

                RWRETURN(result);
            }
        }

        cur = rwLLLinkGetNext(cur);
    }

    /* Not found */
    RWRETURN((RwTexture *)NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Setting and getting the current dictionary

   if NULL -> Search all of the dictionaries

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionarySetCurrent is used to specify the current texture
 * dictionary. Use NULL to indicate that there is no default texture
 * dictionary. See \ref rwtexdictoverview for details of how the
 * current dictionary is used.
 *
 * \param dict  A pointer to the texture dictionary that will become the
 * current dictionary
 *
 * \return Returns a pointer to the texture dictionary
 *
 * \see RwTexDictionaryGetCurrent
 * \see RwTextureRead
 * \see RwTextureCreate
 *
 */
RwTexDictionary *
RwTexDictionarySetCurrent(RwTexDictionary *dict)
{
    RWAPIFUNCTION(RWSTRING("RwTexDictionarySetCurrent"));
    RWASSERT(textureModule.numInstances);

    if (dict)
    {
        RWASSERTISTYPE(dict, rwTEXDICTIONARY);
    }

    RWTEXTUREGLOBAL(currentTexDict) = dict;

    RWRETURN(dict);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryGetCurrent is used to retrieve the current
 * texture dictionary, if any. See \ref rwtexdictoverview for details of how the
 * current dictionary is used.
 *
 * \return Returns a pointer to the current texture dictionary if successful
 * or NULL if there is no current dictionary defined
 *
 * \see RwTexDictionarySetCurrent
 *
 */
RwTexDictionary *
RwTexDictionaryGetCurrent(void)
{
    RWAPIFUNCTION(RWSTRING("RwTexDictionaryGetCurrent"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(currentTexDict));
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Reading textures

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexture
 * \ref RwTextureGenerateMipmapName is used to generate the name of a
 * mipmap level file.
 *
 * It is possible to supply a user defined procedure for naming mipmap
 * levels. The API function \ref RwTextureSetMipmapNameCallBack should
 * be used before reading of textures.
 *
 * \param name  A pointer to a string containing the root name of the texture. This
 * must have at least rwTEXTUREMIPMAPNAMECHARS free storage of size
 * \ref RwChar free after the name itself. The mipmap level name is put here.
 * \param maskName  A pointer to a string containing the root mask name of the texture or
 * NULL if no mask name is required. This must have at least
 * rwTEXTUREMIPMAPNAMECHARS free storage of size \ref RwChar free after the name
 * itself. The mipmap level mask name is put here.
 * \param mipLevel  A value equal to the mipmap level for which the name is required.
 * \param format  A value describing the mipmapping mode. A combination of the bit
 * flags rwRASTERFORMATMIPMAP and rwRASTERFORMATAUTOMIPMAP.
 *
 * \return Returns TRUE if the operation was successful and FALSE if there was
 * a failure. If the return is FALSE the names are not altered
 *
 * \see RwTextureSetMipmapNameCallBack
 * \see RwTextureGetMipmapNameCallBack
 * \see RwTextureRead
 *
 */
RwBool
RwTextureGenerateMipmapName(RwChar *name, RwChar *maskName,
                            RwUInt8 mipLevel, RwInt32 format)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGenerateMipmapName"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(name);

    if (RWTEXTUREGLOBAL(mipmapNameCallback) != NULL)
    {
        RWRETURN((*(RWTEXTUREGLOBAL(mipmapNameCallback)))
                 (name, maskName, mipLevel, format));
    }
    else
    {
        RWRETURN(FALSE);
    }
}

/**
 * \ingroup rwtexture
 * \ref RwTextureRead is used to read a texture from the specified image
 * file on disk. A second image may also be read containing a mask to be
 * applied to the first image after they have been read from the file system.
 * If the mask is not required NULL should be passed as the mask image
 * file name. The strings supplied for the texture names must form the leaf
 * part (i.e. without path or extension) of the pathname for the texture file.
 * For portability it is best to choose texture file names that are a
 * maximum of 8 characters long and which are acceptable to MS-DOS as file
 * names. The rest of the pathname of the texture file is obtained from
 * the current search path; if the search path is not set it is
 * assumed that the texture resides in the same directory as the application
 * executable.
 *
 * New format modules can be added using \ref RwImageRegisterImageFormat. It is
 * also possible to specify a user defined procedure for loading textures from
 * image files of other formats. The API function \ref RwTextureSetReadCallBack
 * is used for this purpose and should be used before calling
 * \ref RwTextureRead.
 *
 * A texture may have already been loaded and may be in a texture dictionary.
 * Therefore, before the texture file is read from disk, a search is first
 * made. By default, the current texture dictionary is searched for a texture
 * of the same name, or if the current dictionary is not defined, all textures
 * dictionaries are searched. This behavior may be overridden by a user
 * defined callback set with \ref RwTextureSetFindCallBack.
 *
 * Only if the texture has not been found in memory is the disk file actually
 * read. Therefore different textures must have different names.
 *
 * When a new texture has been successfully loaded it is added to the current
 * texture dictionary, if one is defined.
 *
 * \note Gamma correction is automatically applied to the texture as it
 * is loaded. Use \ref RwImageSetGamma to set the current gamma value to 1.0 if
 * gamma correction is not required.
 *
 * \note The texture is created with a reference count of one. Hence, it
 * is the responsibility of the application to destroy the texture if it is no
 * longer required.
 *
 * \param name  A pointer to a string containing the name of the texture to be read
 * \param maskName  A pointer to a string containing the name of the mask or NULL if not
 * required
 *
 * \return Returns a pointer to the new texture if successful or NULL if there
 * is an error
 *
 * \see RwImageSetPath
 * \see RwImageSetGamma
 * \see RwImageRegisterImageFormat
 * \see RwTextureSetReadCallBack
 * \see RwTextureSetFindCallBack
 * \see RwTexDictionarySetCurrent
 * \see RwTextureSetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureDestroy
 *
 */
RwTexture *
RwTextureRead(const RwChar *name, const RwChar *maskName)
{
    RwTexture   *result;

    RWAPIFUNCTION(RWSTRING("RwTextureRead"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(name);

    RWASSERT(RWTEXTUREGLOBAL(textureFindFunc));
    result = RWTEXTUREGLOBAL(textureFindFunc) (name);

    /* If we've got a texture, add ref it */
    if (result)
    {
        RwTextureAddRef(result);
        RWASSERT(result);
        RWASSERT(0 < result->refCount);

        RWRETURN(result);
    }

    /* Try and read the one on the disk */
    result = RWTEXTUREGLOBAL(textureReadFunc) (name, maskName);
    if (!result)
    {
        if (maskName)
        {
            RWERROR((E_RW_READTEXMASK, name, maskName));
        }
        else
        {
            RWERROR((E_RW_READTEXMASK, name, RWSTRING("(null)")));
        }

        RWRETURN((RwTexture *)NULL);
    }

    /* Add the texture to a dictionary */
    if (RWTEXTUREGLOBAL(currentTexDict))
    {
        RwTexDictionaryAddTexture(RWTEXTUREGLOBAL(currentTexDict),
                                  result);
    }

    RWASSERT(result);
    RWASSERT(0 < result->refCount);

    RWRETURN(result);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwtexture
 * \ref RwTextureSetFilterMode is used to specify the texture-filtering
 * mode for the given texture.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture whose filtering mode is to be set
 * \param filtering  A value equal to the filtering mode:
 *               \li rwFILTERNEAREST - Point sampled
 *               \li rwFILTERLINEAR - Bilinear interpolation
 *               \li rwFILTERMIPNEAREST - Point sampled per pixel mipmap
 *               \li rwFILTERMIPLINEAR - Bilinear interpolation per pixel mipmap
 *               \li rwFILTERLINEARMIPNEAREST - Mipmap interpolated, point sampled
 *               \li rwFILTERLINEARMIPLINEAR - Trilinear interpolation
 *
 * \return Returns a pointer to the texture
 *
 * \see RwTextureGetFilterMode
 * \see RwTextureSetAddressing
 * \see RwTextureGetAddressing
 *
 */
RwTexture *
RwTextureSetFilterMode(RwTexture *texture, RwTextureFilterMode filtering)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetFilterMode"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureSetFilterModeMacro(texture, filtering));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetFilterMode is used to retrieve the current
 * texture-filtering mode of the specified texture.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture to query
 *
 * \return Returns the texture's filtering mode if successful or
 * rwFILTERNAFILTERMODE if there is an error. Valid filtering modes are:
 *          \li rwFILTERNEAREST - Point sampled
 *          \li rwFILTERLINEAR - Bilinear interpolation
 *          \li rwFILTERMIPNEAREST - Point sampled per pixel mipmap
 *          \li rwFILTERMIPLINEAR - Bilinear interpolation per pixel mipmap
 *          \li rwFILTERLINEARMIPNEAREST - Mipmap interpolated, point sampled
 *          \li rwFILTERLINEARMIPLINEAR - Trilinear interpolation
 *
 * \see RwTextureSetFilterMode
 * \see RwTextureGetAddressing
 * \see RwTextureSetAddressing
 *
 */
RwTextureFilterMode
RwTextureGetFilterMode(const RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetFilterMode"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureGetFilterModeMacro(texture));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetAddressing is used to specify the texture-addressing
 * mode for the given texture. The texture-addressing mode determines how the
 * UV coordinates are used to generate the texel color. This function set
 * the u & v addressing modes. The same result can be achieved by calling
 * \ref RwTextureSetAddressingU & \ref RwTextureSetAddressingV separately.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture whose filtering mode is to be set
 * \param addressing  A value equal to the addressing mode:
 *               \li rwTEXTUREADDRESSWRAP   Wrap mode enables tiling as usual, and is
 *                                          the default
 *               \li rwTEXTUREADDRESSMIRROR Mirror mode flips alternate copies of
 *                                          the texture
 *               \li rwTEXTUREADDRESSCLAMP  Clamp mode limits the range of UV, so that
 *                                          the texture is stretched at the edges
 *               \li rwTEXTUREADDRESSBORDER Border mode applies the material color to the
 *                                          polygon where UV falls outside the range 0-1
 *
 * \return Returns a pointer to the texture
 *
 * \see RwTextureGetAddressing
 * \see RwTextureGetAddressingU
 * \see RwTextureGetAddressingV
 * \see RwTextureSetAddressingU
 * \see RwTextureSetAddressingV
 *
 */
RwTexture *
RwTextureSetAddressing(RwTexture *texture, RwTextureAddressMode addressing)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetAddressing"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureSetAddressingMacro(texture, addressing));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetAddressingU is used to specify the texture-addressing
 * mode for the given texture. The texture-addressing mode determines how the
 * UV coordinates are used to generate the texel color. This function sets the
 * addressing mode used in the u direction. Note that not all platforms
 * support this feature.
 *
 * \note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture whose texture-addressing mode is to
 *                 be set.
 * \param addressing  A value equal to the addressing mode:
 *               \li rwTEXTUREADDRESSWRAP   Wrap mode enables tiling as usual, and is
 *                                          the default
 *               \li rwTEXTUREADDRESSMIRROR Mirror mode flips alternate copies of
 *                                          the texture
 *               \li rwTEXTUREADDRESSCLAMP  Clamp mode limits the range of UV, so that
 *                                          the texture is stretched at the edges
 *               \li rwTEXTUREADDRESSBORDER Border mode applies the material color to the
 *                                          polygon where UV falls outside the range 0-1
 *
 * \return Returns the pointer to the texture
 *
 * \see RwTextureSetAddressing
 * \see RwTextureSetAddressingV
 * \see RwTextureGetAddressing
 * \see RwTextureGetAddressingU
 * \see RwTextureGetAddressingV
 *
 */
RwTexture *
RwTextureSetAddressingU(RwTexture *texture, RwTextureAddressMode addressing)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetAddressingU"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureSetAddressingUMacro(texture, addressing));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetAddressingV is used to specify the texture-addressing
 * mode for the given texture. The texture-addressing mode determines how the
 * UV coordinates are used to generate the texel color. This function sets the
 * addressing mode used in the v direction. Note that not all platforms
 * support this feature.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture whose texture-addressing mode is
 *                 to be set
 * \param addressing  A value equal to the addressing mode:
 *               \li rwTEXTUREADDRESSWRAP    Wrap mode enables tiling as usual, and is
 *                                           the default
 *               \li rwTEXTUREADDRESSMIRROR  Mirror mode flips alternate copies of
 *                                           the texture
 *               \li rwTEXTUREADDRESSCLAMP   Clamp mode limits the range of UV, so that
 *                                           the texture is stretched at the edges
 *               \li rwTEXTUREADDRESSBORDER  Border mode applies the material color to the
 *                                           polygon where UV falls outside the range 0-1
 *
 * \return Returns the pointer to the texture
 *
 * \see RwTextureSetAddressing
 * \see RwTextureSetAddressingU
 * \see RwTextureGetAddressing
 * \see RwTextureGetAddressingU
 * \see RwTextureGetAddressingV
 *
 */
RwTexture *
RwTextureSetAddressingV(RwTexture *texture, RwTextureAddressMode addressing)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetAddressingV"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureSetAddressingVMacro(texture, addressing));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetAddressing is used to retrieve the current
 * texture-addressing mode of the specified texture. The texture-addressing
 * mode determines how the UV coordinates are used to generate the texel
 * color.
 * \note If the addressing modes in the u & v direction are not the same this
 * function will return rwTEXTUREADDRESSNATEXTUREADDRESS.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture to query.
 *
 * \return Returns the texture's addressing mode if successful or
 * rwTEXTUREADDRESSNATEXTUREADDRESS if there is an error. Valid addressing
 * modes are:
 *     \li rwTEXTUREADDRESSWRAP   Wrap mode enables tiling as usual, and is
 *                                the default
 *     \li rwTEXTUREADDRESSMIRROR Mirror mode flips alternate copies of
 *                                the texture
 *     \li rwTEXTUREADDRESSCLAMP  Clamp mode limits the range of UV, so that
 *                                the texture is stretched at the edges
 *     \li rwTEXTUREADDRESSBORDER Border mode applies the material color to the
 *                                polygon where UV falls outside the range 0-1
 *
 * \see RwTextureGetAddressingU
 * \see RwTextureGetAddressingV
 * \see RwTextureSetAddressing
 * \see RwTextureSetAddressingU
 * \see RwTextureSetAddressingV
 *
 */
RwTextureAddressMode
RwTextureGetAddressing(const RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetAddressing"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureGetAddressingMacro(texture));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetAddressingU is used to retrieve the current
 * texture-addressing mode of the specified texture. The texture-addressing
 * mode determines how the UV coordinates are used to generate the texel
 * color.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture to query
 *
 * \return Returns the texture's addressing mode if successful or
 * rwTEXTUREADDRESSNATEXTUREADDRESS if there is an error. Valid addressing
 * modes are:
 *     \li rwTEXTUREADDRESSWRAP     Wrap mode enables tiling as usual, and is
 *                                  the default
 *     \li rwTEXTUREADDRESSMIRROR   Mirror mode flips alternate copies of
 *                                  the texture
 *     \li rwTEXTUREADDRESSCLAMP    Clamp mode limits the range of UV, so that
 *                                  the texture is stretched at the edges
 *     \li rwTEXTUREADDRESSBORDER   Border mode applies the material color to the
 *                                  polygon where UV falls outside the range 0-1
 *
 * \see RwTextureGetAddressing
 * \see RwTextureGetAddressingV
 * \see RwTextureSetAddressing
 * \see RwTextureSetAddressingU
 * \see RwTextureSetAddressingV
 *
 */
RwTextureAddressMode
RwTextureGetAddressingU(const RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetAddressingU"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureGetAddressingUMacro(texture));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetAddressingV is used to retrieve the current
 * texture-addressing mode of the specified texture. The
 * texture-addressing mode
 * determines how the UV coordinates are used to generate the texel color.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param texture  A pointer to the texture to query.
 *
 * \return Returns the texture's addressing mode if successful or
 * rwTEXTUREADDRESSNATEXTUREADDRESS if there is an error. Valid addressing
 * modes are:
 *     \li rwTEXTUREADDRESSWRAP    Wrap mode enables tiling as usual, and is
 *                                 the default
 *     \li rwTEXTUREADDRESSMIRROR  Mirror mode flips alternate copies of
 *                                 the texture
 *     \li rwTEXTUREADDRESSCLAMP   Clamp mode limits the range of UV, so that
 *                                 the texture is stretched at the edges
 *     \li rwTEXTUREADDRESSBORDER  Border mode applies the material color to the
 *                                 polygon where UV falls outside the range 0-1
 *
 * \see RwTextureGetAddressing
 * \see RwTextureGetAddressingU
 * \see RwTextureSetAddressing
 * \see RwTextureSetAddressingU
 * \see RwTextureSetAddressingV
 *
 */
RwTextureAddressMode
RwTextureGetAddressingV(const RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetAddressingV"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

    RWRETURN(RwTextureGetAddressingVMacro(texture));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwtexture
 * \ref RwTextureRegisterPlugin is used to register a plugin and
 * reserve some space within a texture. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * \param size  An \ref RwInt32 value equal to the size of the memory block to reserve
 * \param pluginID  An \ref RwInt32 value equal to the plugin ID (must be unique; used
 * to identify binary chunks)
 * \param constructCB  A constructor for the plugin data block
 * \param destructCB  A destructor for the plugin data block
 * \param copyCB  A copy constructor for the plugin data block
 *
 * \return Returns the byte offset within the texture of memory reserved
 * for this plugin or a negative value if there is an error
 *
 * \see RwTextureRegisterPluginStream
 * \see RwTextureGetPluginOffset
 * \see RwTextureValidatePlugins
 *
 */
RwInt32
RwTextureRegisterPlugin(RwInt32 size,
                        RwUInt32 pluginID,
                        RwPluginObjectConstructor constructCB,
                        RwPluginObjectDestructor destructCB,
                        RwPluginObjectCopy copyCB)
{
    RwInt32 plug;

    RWAPIFUNCTION(RWSTRING("RwTextureRegisterPlugin"));
    RWASSERT(!textureModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlugin(&textureTKList, size,
                                     pluginID, constructCB,
                                     destructCB, copyCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetPluginOffset is used to get the offset of a previously
 * registered plugin.
 *
 * \param pluginID  The plugin ID for which to get the data offset
 *
 * \return Returns the data block offset or -1 if the plugin is not registered
 *
 * \see RwTextureRegisterPlugin
 * \see RwTextureRegisterPluginStream
 * \see RwTextureValidatePlugins
 */
RwInt32
RwTextureGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RwTextureGetPluginOffset"));

    offset = _rwPluginRegistryGetPluginOffset(&textureTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureValidatePlugins is used to validate the plugin memory
 * allocated within the specified texture. This function is useful
 * for determining where memory trampling may be occurring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * \param  texture   A pointer to the texture to validate
 *
 * \return Returns TRUE is the texture data is valid or FALSE if
 * there is an error or if the texture data has become corrupt
 *
 * \see RwTextureRegisterPlugin
 * \see RwTextureRegisterPluginStream
 * \see RwTextureGetPluginOffset
 *
 */
RwBool
RwTextureValidatePlugins(const RwTexture * texture __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RwTextureValidatePlugins"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < (texture->refCount));

#ifdef RWDEBUG
    {
        RwBool              valid;

        valid = _rwPluginRegistryValidateObject(&textureTKList, texture);
        RWRETURN(valid);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryRegisterPlugin is used to register a plugin and
 * reserve some space within a texture dictionary. This must happen after the
 * engine has been initialized but before the engine is opened.
 *
 * \param size  An \ref RwInt32 value equal to the size of the memory block to reserve
 * \param pluginID  An \ref RwInt32 value equal to the plugin ID. (This must be unique.
 *  It is used to identify binary chunks)
 * \param constructCB  A constructor for the plugin data block
 * \param destructCB  A destructor for the plugin data block
 * \param copyCB  A copy constructor for the plugin data block
 *
 * \return Returns the byte offset within the texture dictionary of memory
 * reserved for this plugin or a negative value if there is an error
 *
 * \see RwTexDictionaryValidatePlugins
 * \see RwTexDictionaryGetPluginOffset
 */
RwInt32
RwTexDictionaryRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                              RwPluginObjectConstructor constructCB,
                              RwPluginObjectDestructor destructCB,
                              RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryRegisterPlugin"));
    RWASSERT(!textureModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlugin(&texDictTKList, size,
                                     pluginID, constructCB,
                                     destructCB, copyCB);
    RWRETURN(plug);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryGetPluginOffset is used to get the offset of a
 * previously registered plugin.
 *
 * \param pluginID  The plugin ID for which to get the data offset
 *
 * \return Returns the data block offset or -1 if the plugin is not registered
 *
 * \see RwTexDictionaryRegisterPlugin
 * \see RwTexDictionaryValidatePlugins
 */
RwInt32
RwTexDictionaryGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryGetPluginOffset"));

    offset = _rwPluginRegistryGetPluginOffset(&texDictTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryValidatePlugins is used to validate the plugin
 * memory allocated within the specified texture dictionary. This function is
 * useful for determining where memory trampling may be occurring within an
 * application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * \param  dict   A pointer to the texture dictionary
 * to validate
 *
 * \return Returns TRUE is the texture dictionary data is valid or FALSE if
 * there is an error or if the texture dictionary data has become corrupt
 *
 * \see RwTexDictionaryRegisterPlugin
 * \see RwTexDictionaryGetPluginOffset
 *
 */
RwBool
RwTexDictionaryValidatePlugins(const RwTexDictionary * dict __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RwTexDictionaryValidatePlugins"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(dict);
    RWASSERTISTYPE(dict, rwTEXDICTIONARY);

#ifdef RWDEBUG
    {
        RwBool              valid;

        valid = _rwPluginRegistryValidateObject(&texDictTKList, dict);
        RWRETURN(valid);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetMipmapGenerationCallBack is used to specify the
 * function to be used to generate mipmaps automatically. This function will
 * be called at texture read time if mipmaps are available and automatic
 * mipmap generation is enabled.
 *
 * \param callback  A pointer to the mipmap generation callback function to be used to
 * generate mipmaps automatically
 *
 * \return Returns TRUE
 *
 * \see RwTextureGetMipmapGenerationCallBack
 * \see RwTextureSetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureRasterGenerateMipmaps
 *
 */
RwBool
RwTextureSetMipmapGenerationCallBack(RwTextureCallBackMipmapGeneration
                                     callback)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetMipmapGenerationCallBack"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(callback);

    RWTEXTUREGLOBAL(mipmapCallback) = callback;
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetMipmapGenerationCallBack
 * This function is called at texture read time if mipmaps are available and
 * are to be automatically generated.
 *
 * \return Returns a pointer to the function currently used to generate mipmaps
 *
 * \see RwTextureSetMipmapGenerationCallBack
 * \see RwTextureSetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureRasterGenerateMipmaps
 *
 */
RwTextureCallBackMipmapGeneration
RwTextureGetMipmapGenerationCallBack(void)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetMipmapGenerationCallBack"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(mipmapCallback));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetMipmapNameCallBack is used to specify the function
 * to be used to generate the names of the files containing mipmap levels.
 *
 * The function will be called with a root name and a given mipmap level at
 * texture load time if mipmaps are available and automatic mipmap generation
 * is disabled, i.e. the texture being loaded has mipmap levels stored.
 *
 * \param callback  A pointer to the mipmap name callback to be used to generate the
 * names of the files containing mipmap levels
 *
 * \return Returns TRUE
 *
 * \see RwTextureGetMipmapNameCallBack
 * \see RwTextureSetMipmapGenerationCallBack
 * \see RwTextureGetMipmapGenerationCallBack
 * \see RwTextureSetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureRasterGenerateMipmaps
 *
 */
RwBool
RwTextureSetMipmapNameCallBack(RwTextureCallBackMipmapName callback)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetMipmapNameCallBack"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(callback);

    RWTEXTUREGLOBAL(mipmapNameCallback) = callback;
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetMipmapNameCallBack queries the function currently
 * used to generate names for mip level files for textures.
 *
 * \return Returns a pointer to the function currently used to generate names
 * for mip level files for textures. Returns NULL if there is no function or
 * there is an error
 *
 * \see RwTextureSetMipmapNameCallBack
 * \see RwTextureSetMipmapGenerationCallBack
 * \see RwTextureGetMipmapGenerationCallBack
 * \see RwTextureSetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureRasterGenerateMipmaps
 */
RwTextureCallBackMipmapName
RwTextureGetMipmapNameCallBack(void)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetMipmapNameCallBack"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(mipmapNameCallback));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureRasterGenerateMipmaps is used to generate mipmaps for
 * a raster, either from the supplied image (all mip levels including 0 are
 * generated), or from mip level 0 of the raster into the remaining mip
 * levels (no image supplied). The mipmaps are generated by the default
 * callback function or a user supplied callback.
 *
 * \param raster  A pointer to the raster that is to have the mipmaps generated.
 * \param image  A pointer to an image from which to generate the mipmaps or NULL.
 *
 * \return Returns TRUE if the mipmaps where generated correctly, or FALSE
 * if there was an error
 *
 * \see RwTextureSetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureSetMipmapGenerationCallBack
 * \see RwPalQuantSetMaxDepth
 * \see RwPalQuantGetMaxDepth
 *
 */
RwBool
RwTextureRasterGenerateMipmaps(RwRaster *raster, RwImage *image)
{
    RWAPIFUNCTION(RWSTRING("RwTextureRasterGenerateMipmaps"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(raster);

    /* image can be NULL */
    if ((*RWTEXTUREGLOBAL(mipmapCallback)) (raster, image))
    {
        RWRETURN(TRUE);
    }
    else
    {
        RWRETURN(FALSE);
    }
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening and closing

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwTextureClose

 On entry   :
 On exit    :
 */

void *
_rwTextureClose(void *instance,
                RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwTextureClose"));

    if (RWTEXTUREGLOBAL(scratchMem))
    {
        RwFree(RWTEXTUREGLOBAL(scratchMem));
        RWTEXTUREGLOBAL(scratchMem) = (unsigned char *)NULL;
        RWTEXTUREGLOBAL(scratchMemSize) = 0;
    }

    if (RWTEXTUREGLOBAL(textureFreeList) &&
        RWTEXTUREGLOBAL(texDictFreeList))
    {
        RwLLLink           *cur, *end;
        RwBool              texDictsExistOnShutdown;


        cur = rwLinkListGetFirstLLLink(&RWTEXTUREGLOBAL(texDictList));
        end = rwLinkListGetTerminator(&RWTEXTUREGLOBAL(texDictList));

        /* All objects should be destroyed by the app before shutdown.
         * This is so that (a) there are no leaks and (b) object plugin
         * destructors are only called when ALL plugins are still open
         * (various combos of open/closed could break). */
        texDictsExistOnShutdown = (cur != end);

        while ( cur != end )
        {
            RwTexDictionary    *dict = (rwLLLinkGetData( cur,
                                                         RwTexDictionary,
                                                         lInInstance ) );

            RwLLLink           *next;


            RWASSERTISTYPE(dict, rwTEXDICTIONARY);

            next = rwLLLinkGetNext( cur );

            /* Only remove the dummy texture dictionary created in
             * _rwTextureOpen if it still exists.
             * All other remaining texture dictionaries (created by the app)
             * should stay so that the assert picks them up. */
            if ( dict == dummyTexDict )
            {
                RwTexDictionaryDestroy( dummyTexDict );
                dummyTexDict = (RwTexDictionary *)NULL;

                /* refetch the extents of the texture dictionary list */
                cur = rwLinkListGetFirstLLLink(&RWTEXTUREGLOBAL(texDictList));
                end = rwLinkListGetTerminator(&RWTEXTUREGLOBAL(texDictList));
                texDictsExistOnShutdown = (cur != end);
                break;
            }

            cur = next;
        }

        RWASSERT(FALSE == texDictsExistOnShutdown);
    }

    if (RWTEXTUREGLOBAL(textureFreeList))
    {
        /* Destroy the free list */
        RwFreeListDestroy(RWTEXTUREGLOBAL(textureFreeList));
        RWTEXTUREGLOBAL(textureFreeList) = (RwFreeList *)NULL;
    }

    if (RWTEXTUREGLOBAL(texDictFreeList))
    {
        /* Destroy the free list */
        RwFreeListDestroy(RWTEXTUREGLOBAL(texDictFreeList));
        RWTEXTUREGLOBAL(texDictFreeList) = (RwFreeList *)NULL;
    }

    /* One less module instance */
    textureModule.numInstances--;

#if (defined(RWEVALUATION))
    _rwCameraValClose();
#endif /* defined(RWEVALUATION) */

    /* Success */
    RWRETURN(instance);
}

static RwInt32 _rwTextureFreeListBlockSize = 128,
               _rwTextureFreeListPreallocBlocks = 1;
static RwFreeList _rwTextureFreeList;

/**
 * \ingroup rwtexture
 * \ref RwTextureSetFreeListCreateParams allows the developer to specify
 * how many \ref RwTexture s to preallocate space for.
 * Call before \ref RwEngineOpen.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwTextureSetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc)
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RwTextureSetFreeListCreateParams"));
#endif
    _rwTextureFreeListBlockSize = blockSize;
    _rwTextureFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

static RwInt32 _rwTexDictionaryFreeListBlockSize = 5,
               _rwTexDictionaryFreeListPreallocBlocks = 1;
static RwFreeList _rwTexDictionaryFreeList;

/**
 * \ingroup rwtexture
 * \ref RwTexDictionarySetFreeListCreateParams allows the developer to specify
 * how many \ref RwTexDictionary s to preallocate space for.
 * Call before \ref RwEngineOpen.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwTexDictionarySetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RwTexDictionarySetFreeListCreateParams"));
#endif
    _rwTexDictionaryFreeListBlockSize = blockSize;
    _rwTexDictionaryFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

/****************************************************************************
 _rwTextureOpen

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwTextureOpen(void *instance, RwInt32 offset,
               RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwTextureOpen"));

#if defined(RWEVALUATION)
    _rwCameraValOpen();
#endif /* defined(RWEVALUATION) */

    /* Save offset of global data (same for all instances) */
    textureModule.globalsOffset = offset;

    RWTEXTUREGLOBAL(textureFreeList) =
        RwFreeListCreateAndPreallocateSpace(textureTKList.sizeOfStruct, _rwTextureFreeListBlockSize,
        sizeof(RwUInt32), _rwTextureFreeListPreallocBlocks, &_rwTextureFreeList,
        rwMEMHINTDUR_GLOBAL | rwID_TEXTURE);
    if (!RWTEXTUREGLOBAL(textureFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    RWTEXTUREGLOBAL(texDictFreeList) =
        RwFreeListCreateAndPreallocateSpace(texDictTKList.sizeOfStruct, _rwTexDictionaryFreeListBlockSize,
        sizeof(RwUInt32), _rwTexDictionaryFreeListPreallocBlocks, &_rwTexDictionaryFreeList,
        rwMEMHINTDUR_GLOBAL | rwID_TEXTUREMODULE);

    if (!RWTEXTUREGLOBAL(texDictFreeList))
    {
        /* Failure */
        RwFreeListDestroy(RWTEXTUREGLOBAL(textureFreeList));
        RWTEXTUREGLOBAL(textureFreeList) = (RwFreeList *)NULL;
        RWRETURN(NULL);
    }

    rwLinkListInitialize(&RWTEXTUREGLOBAL(texDictList));

    /* One more module instance - do this before API calls */
    textureModule.numInstances++;

    /* Create a new dictionary */
    dummyTexDict = RwTexDictionaryCreate();
    RWTEXTUREGLOBAL(currentTexDict) = dummyTexDict;
    if (!RWTEXTUREGLOBAL(currentTexDict))
    {
        RwFreeListDestroy(RWTEXTUREGLOBAL(texDictFreeList));
        RWTEXTUREGLOBAL(texDictFreeList) = (RwFreeList *)NULL;
        RwFreeListDestroy(RWTEXTUREGLOBAL(textureFreeList));
        RWTEXTUREGLOBAL(textureFreeList) = (RwFreeList *)NULL;
        RWRETURN(NULL);
    }

    /* Set default texture reading mode */
    RWTEXTUREGLOBAL(haveTexMipmaps) = FALSE;
    RWTEXTUREGLOBAL(generateTexMipmaps) = FALSE;

    /* Get on with the setup */
    RwTextureSetFindCallBack(TextureDefaultFind);
    RwTextureSetReadCallBack(TextureDefaultRead);
    RwTextureSetMipmapGenerationCallBack
        (TextureRasterDefaultBuildMipmaps);
    RwTextureSetMipmapNameCallBack(TextureDefaultMipmapName);

    /* Initialise the scratch memory */
    RWTEXTUREGLOBAL(scratchMem) = (unsigned char *)NULL;
    RWTEXTUREGLOBAL(scratchMemSize) = 0;

    /* Success */
    RWRETURN(instance);
}
