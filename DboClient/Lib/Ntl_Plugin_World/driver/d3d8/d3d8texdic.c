/***************************************************************************
 *                                                                         *
 * Module  : D3D8texdic.c                                                    *
 *                                                                         *
 * Purpose :                                                               *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

/* D3D8 Libraries */
#include <d3d8.h>

#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "barwtyp.h"
#include "batextur.h"
#include "babinary.h"
#include "baimras.h"

#include "drvfns.h"
#include "d3d8device.h"
#include "d3d8raster.h"
#include "d3d8convrt.h"
#include "d3d8dxttex.h"

#include "d3d8texdic.h"

/****************************************************************************
 Defines
 */
#define IGNORED_D3D8    0

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

typedef struct _rwD3D8NativeTexture _rwD3D8NativeTexture;
struct _rwD3D8NativeTexture
{
    RwInt32 id; /* RwPlatformID,(rwID_D3D8) defined in batype.h */
    RwInt32 filterAndAddress; /* Same as in babintex.c */
    RwChar  name[rwTEXTUREBASENAMELENGTH]; /* Texture name */
    RwChar  mask[rwTEXTUREBASENAMELENGTH]; /* Texture mask name */
};

typedef struct _rwD3D8NativeRaster _rwD3D8NativeRaster;
struct _rwD3D8NativeRaster
{
    RwInt32     format;         /* Raster format flags */
    RwBool      alpha;          /* This raster has an alpha component */
    RwUInt16    width;          /* Raster width */
    RwUInt16    height;         /* Raster height */
    RwUInt8     depth;          /* Raster depth */
    RwUInt8     numMipLevels;   /* The number of mip levels to load */
    RwUInt8     type;           /* The raster type */
    RwUInt8     dxtFormat;      /* 1-5 for DXT format 0 for normal */
};

/*
    rwCHUNKHEADERSIZE
        _rwD3D8NativeTexture
        _rwD3D8NativeRaster

        if ((format & rwRASTERFORMATPAL4) || (format & rwRASTERFORMATPAL8))
        {
            Load/Save palette
        }

        RwUInt32 - mip level size in bytes     --
        Data - mip level data                   | numMipLevels times
                    |                           |
                    --------------<--------------

 */

static RwBool D3D8TexDictionaryEnableRasterFormatConversion = FALSE;

/****************************************************************************
 _rwD3D8NativeTextureGetSize

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8NativeTextureGetSize(void *sizeIn, void *textureIn,
                          RwInt32 unused3 __RWUNUSED__)
{
    RwTexture       *texture = (RwTexture *)textureIn;
    RwRaster        *raster;
    _rwD3D8RasterExt  *rasExt;
    RwInt32         numMipLevels;
    RwUInt32        rasFormat;
    RwUInt32        size;

    RWFUNCTION(RWSTRING("_rwD3D8NativeTextureGetSize"));

    /* Platform specific ID, filter & addressing modes texture and Mask names */
    size = rwCHUNKHEADERSIZE + sizeof(_rwD3D8NativeTexture);

    raster = RwTextureGetRaster(texture);
    if (!raster)
    {
        *((RwUInt32 *)sizeIn) = size;

        RWRETURN(TRUE);
    }

    /* Native raster data structure */
    size += sizeof(_rwD3D8NativeRaster);

    /* Size of the palette if palletized */
    rasFormat = RwRasterGetFormat(raster);
    if (rasFormat & rwRASTERFORMATPAL4)
    {
        /* 32 is the smallest palette size that DX supports */
        size += sizeof(PALETTEENTRY) * 32;
    }
    else if (rasFormat & rwRASTERFORMATPAL8)
    {
        size += sizeof(PALETTEENTRY) * 256;
    }

    rasExt = RASTEREXTFROMRASTER(raster);

    /* Size of pixel data for all mip levels */
    numMipLevels = IDirect3DTexture8_GetLevelCount(rasExt->texture);
    while (numMipLevels--)
    {
        D3DSURFACE_DESC surfaceDesc;

        IDirect3DTexture8_GetLevelDesc(rasExt->texture, numMipLevels, &surfaceDesc);
        size += sizeof(RwUInt32) + surfaceDesc.Size;
    }

    *((RwUInt32 *)sizeIn) = size;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8NativeTextureWrite

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8NativeTextureWrite(void *streamIn, void *textureIn,
                        RwInt32 unused3 __RWUNUSED__)
{
    RwStream            *stream = (RwStream *)streamIn;
    RwTexture           *texture = (RwTexture *)textureIn;
    RwRaster            *raster;
    _rwD3D8RasterExt      *rasExt;
    _rwD3D8NativeTexture  nativeTexture;
    _rwD3D8NativeRaster   nativeRaster;
    RwInt32             bytesLeftToWrite;
    RwInt32             i;

    RWFUNCTION(RWSTRING("_rwD3D8NativeTextureWrite"));

    /* Calc the amount of data to write, excluding the chunk header*/
    _rwD3D8NativeTextureGetSize((void *)&bytesLeftToWrite, textureIn, 0);
    bytesLeftToWrite -= rwCHUNKHEADERSIZE;

    /* Struct header for _rwD3DNativeTexture structure */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, bytesLeftToWrite))
    {
        RWRETURN(FALSE);
    }

    /*
     * Don't have to worry about endianness as this is platform specific
     */

    /* ID, filter & addressing modes */
    nativeTexture.id = rwID_PCD3D8;
    nativeTexture.filterAndAddress =
        (((RwInt32)RwTextureGetFilterMode(texture)) & 0xFF) |
        ((((RwInt32)RwTextureGetAddressingU(texture)) << 8) & 0x0F00) |
        ((((RwInt32)RwTextureGetAddressingV(texture)) << 12) & 0xF000);

    /* Texture name */
    memcpy(nativeTexture.name, RwTextureGetName(texture),
        sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

    /* Mask name */
    memcpy(nativeTexture.mask, RwTextureGetMaskName(texture),
        sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

    if (!RwStreamWrite(stream, (void *)&nativeTexture,
                       sizeof(_rwD3D8NativeTexture)))
    {
        RWRETURN(FALSE);
    }

    bytesLeftToWrite -= sizeof(_rwD3D8NativeTexture);

    /*
     * Write the rasters
     */
    raster = RwTextureGetRaster(texture);
    rasExt = RASTEREXTFROMRASTER(raster);

    nativeRaster.width = (RwUInt16)RwRasterGetWidth(raster);
    nativeRaster.height = (RwUInt16)RwRasterGetHeight(raster);
    nativeRaster.format = RwRasterGetFormat(raster);
    nativeRaster.depth = (RwUInt8)RwRasterGetDepth(raster);
    nativeRaster.numMipLevels = (RwUInt8)RwRasterGetNumLevels(raster);
    nativeRaster.type = (RwUInt8)raster->cType;
    nativeRaster.alpha = rasExt->alpha;

    switch (rasExt->dxtFormat)
    {
        case D3DFMT_DXT1:
            nativeRaster.dxtFormat = 1;
            break;

        case D3DFMT_DXT2:
            nativeRaster.dxtFormat = 2;
            break;

        case D3DFMT_DXT3:
            nativeRaster.dxtFormat = 3;
            break;

        case D3DFMT_DXT4:
            nativeRaster.dxtFormat = 4;
            break;

        case D3DFMT_DXT5:
            nativeRaster.dxtFormat = 5;
            break;

        default:
            nativeRaster.dxtFormat = 0;
            break;
    }

    if (!RwStreamWrite(stream, &nativeRaster, sizeof(_rwD3D8NativeRaster)))
    {
        RWRETURN(FALSE);
    }

    bytesLeftToWrite -= sizeof(_rwD3D8NativeRaster);

    if (nativeRaster.format & rwRASTERFORMATPAL4)
    {
        RwUInt8 *palette;

        palette = RwRasterLockPalette(raster, rwRASTERLOCKREAD);

        /* 32 is the smallest palette size that DX supports */
        if (!RwStreamWrite(stream, (void *)palette, sizeof(PALETTEENTRY) * 32))
        {
            RWRETURN(FALSE);
        }

        RwRasterUnlockPalette(raster);

        bytesLeftToWrite -= sizeof(PALETTEENTRY) * 32;
    }
    else if (nativeRaster.format & rwRASTERFORMATPAL8)
    {
        RwUInt8 *palette;

        palette = RwRasterLockPalette(raster, rwRASTERLOCKREAD);

        /* 32 is the smallest palette size that DX supports */
        if (!RwStreamWrite(stream, (void *)palette, sizeof(PALETTEENTRY) * 256))
        {
            RWRETURN(FALSE);
        }

        RwRasterUnlockPalette(raster);

        bytesLeftToWrite -= sizeof(PALETTEENTRY) * 256;
    }

    for (i = 0; i < nativeRaster.numMipLevels; i++)
    {
        D3DSURFACE_DESC surfaceDesc;
        RwUInt32        size;
        RwUInt8         *pixels;

        IDirect3DTexture8_GetLevelDesc(rasExt->texture, i, &surfaceDesc);
        size = surfaceDesc.Size;

        if (!RwStreamWrite(stream, (void *)&size, sizeof(RwUInt32)))
        {
            RWRETURN(FALSE);
        }

        bytesLeftToWrite -= sizeof(RwUInt32);

        pixels = RwRasterLock(raster, (RwUInt8)i, rwRASTERLOCKREAD);
        RWASSERT(NULL != pixels);

        if (!pixels)
        {
            RWRETURN(FALSE);
        }

        /*
         * Current implementation of fwrite seem to cough on large write
         */

        /* Don't delete these C++ comment John, unless the above is fixed */
        /*
        if (!RwStreamWrite(stream, (void *)pixels, size))
        {
            RWRETURN(FALSE);
        }
        */
        {
            RwInt32 numBlocks;

            numBlocks = size / 1024;

            while (numBlocks--)
            {
                if (!RwStreamWrite(stream, (void *)pixels, 1024))
                {
                    RWRETURN(FALSE);
                }

                pixels += 1024;
                size -= 1024;
            }

            if (0 != size)
            {
                if (!RwStreamWrite(stream, (void *)pixels, size))
                {
                    RWRETURN(FALSE);
                }
            }
        }

        RwRasterUnlock(raster);

        bytesLeftToWrite -= surfaceDesc.Size;
    }

    RWASSERT(0 == bytesLeftToWrite);

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8NativeTextureRead

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8NativeTextureRead(void *streamIn, void *textureIn,
                       RwInt32 unused3 __RWUNUSED__)
{
    RwStream            *stream = (RwStream *)streamIn;
    RwRaster            *raster = (RwRaster *)NULL;
    RwBool              decompress;
    RwInt32             i;
    RwInt32             rasFormat = 0;
    RwTexture           *texture;
    RwUInt32            length, version;
    _rwD3D8NativeRaster   nativeRaster;
    _rwD3D8NativeTexture  nativeTexture;
    _rwD3D8RasterExt      *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D8NativeTextureRead"));

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &length, &version))
    {
        RWRETURN(FALSE);
    }

    RWASSERT(version >= rwLIBRARYBASEVERSION);
    RWASSERT(version <= rwLIBRARYCURRENTVERSION);
    if (!((version >= rwLIBRARYBASEVERSION) &&
         (version <= rwLIBRARYCURRENTVERSION)))
    {
        RWRETURN(FALSE);
    }

    /*
     * Don't have to worry about endianness as this is platform specific
     */

    /* ID, filter & addressing modes */
    if (RwStreamRead(stream, (void *)&nativeTexture,
        sizeof(_rwD3D8NativeTexture)) != sizeof(_rwD3D8NativeTexture))
    {
        RWRETURN(FALSE);
    }

    /* Check native texture ID */
    RWASSERT(rwID_PCD3D8 == nativeTexture.id);

    if (rwID_PCD3D8 != nativeTexture.id)
    {
        RWRETURN(FALSE);
    }

    /* Native raster data structure */
    if (RwStreamRead(stream, (void *)&nativeRaster,
        sizeof(_rwD3D8NativeRaster)) != sizeof(_rwD3D8NativeRaster))
    {
        RWRETURN(FALSE);
    }

    decompress = FALSE;

    if (0 != nativeRaster.dxtFormat)
    {
        RwUInt32 dxtFormat;

        /* Get compressed format */
        switch (nativeRaster.dxtFormat)
        {
            case 1:
                dxtFormat = D3DFMT_DXT1;
                break;

            case 2:
                dxtFormat = D3DFMT_DXT2;
                break;

            case 3:
                dxtFormat = D3DFMT_DXT3;
                break;

            case 4:
                dxtFormat = D3DFMT_DXT4;
                break;

            case 5:
                dxtFormat = D3DFMT_DXT5;
                break;

            default:
                RWRETURN(FALSE);
                break;
        }

        if (_rwD3D8CheckValidTextureFormat(dxtFormat))
        {
            RwUInt32 levels;

            /* Create a raster */
            raster = RwRasterCreate(nativeRaster.width,
                                    nativeRaster.height,
                                    (RwInt32)nativeRaster.depth,
                                    nativeRaster.type | nativeRaster.format |
                                    rwRASTERDONTALLOCATE);

            if (NULL == raster)
            {
                RWRETURN(FALSE);
            }

            /* Get the raster extension */
            rasExt = RASTEREXTFROMRASTER(raster);

            levels = ((RwRasterGetFormat(raster) & rwRASTERFORMATMIPMAP) ?
                      nativeRaster.numMipLevels : 1);

            /* Create the texture, if mipped 0 gets all the mip levels */
            if (FAILED(IDirect3DDevice8_CreateTexture(_RwD3DDevice,
                                                      raster->width,
                                                      raster->height,
                                                      levels,
                                                      IGNORED_D3D8,
                                                      (D3DFORMAT)dxtFormat,
                                                      D3DPOOL_MANAGED,
                                                      &(rasExt->texture))))
            {
                RwRasterDestroy(raster);

                decompress = TRUE;
            }
            else
            {
                /* Remove the rwRASTERDONTALLOCATE flag so it will get destroyed */
                raster->cFlags &= ~rwRASTERDONTALLOCATE;

                /* Copy alpha flag */
                rasExt->alpha = nativeRaster.alpha;

                /* Set its type - only valid used for DXT compressed textures else 0 */
                rasExt->dxtFormat = dxtFormat;
            }
        }
        else
        {
            decompress = TRUE;
        }

        if (decompress)
        {
            /* Create a raster */
            switch(dxtFormat)
            {
                case D3DFMT_DXT1:
                    if (nativeRaster.alpha)
                    {
                        rasFormat = rwRASTERFORMAT1555;
                    }
                    else
                    {
                        rasFormat = rwRASTERFORMAT565;
                    }
                    break;

                case D3DFMT_DXT2:
                case D3DFMT_DXT3:
                    if (_rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4))
                    {
                        rasFormat = rwRASTERFORMAT4444;
                    }
                    else
                    {
                        rasFormat = rwRASTERFORMAT1555;
                    }
                    break;

                case D3DFMT_DXT4:
                case D3DFMT_DXT5:
                    if (_rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4))
                    {
                        rasFormat = rwRASTERFORMAT4444;
                    }
                    else
                    {
                        rasFormat = rwRASTERFORMAT1555;
                    }
                    break;
            }

            rasFormat |= (nativeRaster.format & (rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP));

            raster = RwRasterCreate(nativeRaster.width,
                                    nativeRaster.height,
                                    (RwInt32)nativeRaster.depth,
                                    nativeRaster.type | rasFormat);
            if (!raster)
            {
                RWRETURN(FALSE);
            }

            /* Get the raster extension */
            rasExt = RASTEREXTFROMRASTER(raster);

            rasExt->dxtFormat = 0;
        }
    }
    else
    {
        /* Create a raster */
        rasFormat =
            _rwD3D8FindCorrectRasterFormat((RwRasterType)nativeRaster.type,
                                           nativeRaster.format);

        rasFormat |= (nativeRaster.format & (rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP));

        raster = RwRasterCreate(nativeRaster.width,
                                nativeRaster.height,
                                (RwInt32)nativeRaster.depth,
                                nativeRaster.type | rasFormat);
        if (!raster)
        {
            RWRETURN(FALSE);
        }

        /* Get the raster extension */
        rasExt = RASTEREXTFROMRASTER(raster);

        rasExt->dxtFormat = 0;
    }

    /* This raster has an alpha component */
    /*
    rasExt->alpha = nativeRaster.alpha;
    */

    rasFormat = RwRasterGetFormat(raster);
    if (rasFormat == nativeRaster.format &&
        nativeRaster.width == raster->width &&
        nativeRaster.height == raster->height &&
        decompress == FALSE)
    {
        RwUInt32    autoMipmap;

        /* Load the palette if palletized */
        if (nativeRaster.format & rwRASTERFORMATPAL4)
        {
            RwUInt8     *palette;
            RwUInt32    size;

            palette = RwRasterLockPalette(raster, rwRASTERLOCKWRITE);
            RWASSERT(NULL != palette);

            size = sizeof(PALETTEENTRY) * 32;
            if (RwStreamRead(stream, (void *)palette, size) != size)
            {
                RWRETURN(FALSE);
            }

            RwRasterUnlockPalette(raster);
        }
        else if (nativeRaster.format & rwRASTERFORMATPAL8)
        {
            RwUInt8     *palette;
            RwUInt32    size;

            palette = RwRasterLockPalette(raster, rwRASTERLOCKWRITE);
            RWASSERT(NULL != palette);

            size = sizeof(PALETTEENTRY) * 256;
            if (RwStreamRead(stream, (void *)palette, size) != size)
            {
                RWRETURN(FALSE);
            }

            RwRasterUnlockPalette(raster);
        }

        /* Remove AUTOMIPMAP flag to avoid unlock invoking mipmap create */
        autoMipmap = raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8);
        raster->cFormat &= ~autoMipmap;

        /* Load mips into raster */
        for (i = 0; i < nativeRaster.numMipLevels; i++)
        {
            RwUInt8     *pixels;
            RwUInt32    size;

            pixels = RwRasterLock(raster, (RwUInt8)i, rwRASTERLOCKWRITE);
            RWASSERT(NULL != pixels);

            /* Size in bytes of mip to load */
            if (RwStreamRead(stream, (void *)&size, sizeof(RwUInt32)) != sizeof(RwUInt32))
            {
                RWRETURN(FALSE);
            }

            /* Read the mip level */
            if (RwStreamRead(stream, (void *)pixels, size) != size)
            {
                RWRETURN(FALSE);
            }

            RwRasterUnlock(raster);
        }

        /* Restore auto mip flag */
        raster->cFormat |= autoMipmap;
    }
    else if (D3D8TexDictionaryEnableRasterFormatConversion)
    {
        PALETTEENTRY palette[256];
        RwUInt32    autoMipmap;

        if (decompress)
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D8NativeTextureRead",
                               "Compressed texture format not supported, converting to a default texture format.");
        }
        else
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D8NativeTextureRead",
                               "Texture format or dimensions not supported, converting to a default texture.");
        }

        /* Load the palette if palletized */
        if (nativeRaster.format & rwRASTERFORMATPAL4)
        {
            RwUInt32    size;

            size = sizeof(PALETTEENTRY) * 32;
            if (RwStreamRead(stream, (void *)palette, size) != size)
            {
                RwRasterDestroy(raster);

                RWRETURN(FALSE);
            }
        }
        else if (nativeRaster.format & rwRASTERFORMATPAL8)
        {
            RwUInt32    size;

            size = sizeof(PALETTEENTRY) * 256;
            if (RwStreamRead(stream, (void *)palette, size) != size)
            {
                RwRasterDestroy(raster);

                RWRETURN(FALSE);
            }
        }

        /* Remove AUTOMIPMAP flag to avoid unlock invoking mipmap create */
        autoMipmap = raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8);
        raster->cFormat &= ~autoMipmap;

        /* Load mips into raster */
        for (i = 0; i < nativeRaster.numMipLevels; i++)
        {
            RwUInt32    size;

            /* Size in bytes of mip to load */
            if (RwStreamRead(stream, (void *)&size, sizeof(RwUInt32)) != sizeof(RwUInt32))
            {
                RwRasterDestroy(raster);

                RWRETURN(FALSE);
            }

            if (i < RwRasterGetNumLevels(raster))
            {
                RwImage     *image;
                RwUInt32    width, height;

                width = nativeRaster.width >> i;
                if (width < 1)
                {
                    width = 1;
                }

                height = nativeRaster.height >> i;
                if (height < 1)
                {
                    height = 1;
                }

                if (nativeRaster.dxtFormat == 0)
                {
                    if (nativeRaster.format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
                    {
                        image = RwImageCreate(width, height, 8);
                    }
                    else
                    {
                        image = RwImageCreate(width, height, 32);
                    }
                }
                else
                {
                    image = RwImageCreate(width, height, 32);
                }

                if (image)
                {
                    RwImageAllocatePixels(image);
                }
                else
                {
                    RwRasterDestroy(raster);

                    RWRETURN(FALSE);
                }

                if (nativeRaster.format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
                {
                    RwImageSetPalette(image, (RwRGBA *)palette);

                    /* RWASSERT(size == (width * height)); */

                    /* Read the mip level */
                    if (RwStreamRead(stream, (void *)RwImageGetPixels(image), size) != size)
                    {
                        RwImageDestroy(image);
                        RwRasterDestroy(raster);

                        RWRETURN(FALSE);
                    }
                }
                else
                {
                    RwUInt8         *pixels;
                    RwUInt32        x, y;

                    pixels = (RwUInt8 *)RwMalloc(size,
                        rwID_IMAGE | rwMEMHINTDUR_FUNCTION);

                    /* Read the mip level */
                    if (pixels==NULL || RwStreamRead(stream, (void *)pixels, size) != size)
                    {
                        RwFree(pixels);
                        RwImageDestroy(image);
                        RwRasterDestroy(raster);

                        RWRETURN(FALSE);
                    }

                    if (nativeRaster.dxtFormat)
                    {
                        switch(nativeRaster.dxtFormat)
                        {
                            case 1:
                                _rwD3D8DecompressDXT1(width, height, (RwUInt32 *)image->cpPixels, (const RwUInt16 *)pixels);
                                break;

                            case 2:
                            case 3:
                                _rwD3D8DecompressDXT3(width, height, (RwUInt32 *)image->cpPixels, (const RwUInt16 *)pixels);
                                break;

                            case 4:
                            case 5:
                                _rwD3D8DecompressDXT5(width, height, (RwUInt32 *)image->cpPixels, (const RwUInt16 *)pixels);
                                break;
                        }
                    }
                    else
                    {
                        const RwUInt32 stride = size / height;

                        /* Convert loaded raster to 32 bits */
                        if ((nativeRaster.format & rwRASTERFORMATPIXELFORMATMASK)==rwRASTERFORMAT888)
                        {
                            for (y = 0; y < height; y++)
                            {
                                const RwUInt32  *srcPixel = (const RwUInt32 *)(pixels + (stride * y));
                                RwRGBA          *dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * y));

                                for (x = 0; x < width; x++)
                                {
                                    const RwUInt32 pixIn = *srcPixel;

                                    srcPixel++;

                                    dstPixel->alpha = 0xFF;
                                    dstPixel->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
                                    dstPixel->green = (RwUInt8)((pixIn >> 8) & 0xFF);
                                    dstPixel->blue  = (RwUInt8)((pixIn) & 0xFF);

                                    dstPixel++;
                                }
                            }
                        }
                        else if ((nativeRaster.format & rwRASTERFORMATPIXELFORMATMASK)==rwRASTERFORMAT8888)
                        {
                            for (y = 0; y < height; y++)
                            {
                                const RwUInt32  *srcPixel = (const RwUInt32 *)(pixels + (stride * y));
                                RwRGBA          *dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * y));

                                for (x = 0; x < width; x++)
                                {
                                    const RwUInt32 pixIn = *srcPixel;

                                    srcPixel++;

                                    dstPixel->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
                                    dstPixel->green = (RwUInt8)((pixIn >> 8) & 0xFF);
                                    dstPixel->blue  = (RwUInt8)((pixIn) & 0xFF);
                                    dstPixel->alpha = (RwUInt8)(pixIn >> 24);

                                    dstPixel++;
                                }
                            }
                        }
                        else
                        {
                            const RwUInt32  pixelSize = nativeRaster.depth / 8;

                            for (y = 0; y < height; y++)
                            {
                                RwUInt8 *srcPixel = pixels + y * stride;
                                RwRGBA  *dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * y));

                                for (x = 0; x < width; x++)
                                {
                                    _rwD3D8PixelToRGB(dstPixel, srcPixel, nativeRaster.format);

                                    dstPixel++;

                                    srcPixel += pixelSize;
                                }
                            }
                        }
                    }

                    RwFree(pixels);
                }

                RwRasterLock(raster, (RwUInt8)i, rwRASTERLOCKWRITE);

                RwRasterSetFromImage(raster, image);

                RwRasterUnlock(raster);

                RwImageDestroy(image);
            }
            else
            {
                RwStreamSkip(stream, size);
            }
        }

        /* Restore auto mip flag */
        raster->cFormat |= autoMipmap;
    }
    else
    {
        if (decompress)
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D8NativeTextureRead",
                               "Compressed texture format not supported.");
        }
        else
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D8NativeTextureRead",
                               "Texture format or dimensions not supported.");
        }

        RwRasterDestroy(raster);

        RWRETURN(FALSE);
    }

    texture = RwTextureCreate(raster);
    if (!texture)
    {
        RwRasterDestroy(raster);

        RWRETURN(FALSE);
    }

    RwTextureSetFilterMode(texture, nativeTexture.filterAndAddress & 0xFF);
    RwTextureSetAddressingU(texture, (nativeTexture.filterAndAddress >> 8) & 0x0F);
    RwTextureSetAddressingV(texture, (nativeTexture.filterAndAddress >> 12) & 0x0F);
    RwTextureSetName(texture, nativeTexture.name);
    RwTextureSetMaskName(texture, nativeTexture.mask);

    *((RwTexture **)textureIn) = texture;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8TexDictionaryEnableRasterFormatConversion

 On entry   :
 On exit    :
 */
void
_rwD3D8TexDictionaryEnableRasterFormatConversion(RwBool enable)
{
    RWFUNCTION(RWSTRING("_rwD3D8TexDictionaryEnableRasterFormatConversion"));

    D3D8TexDictionaryEnableRasterFormatConversion = enable;

    RWRETURNVOID();
}
