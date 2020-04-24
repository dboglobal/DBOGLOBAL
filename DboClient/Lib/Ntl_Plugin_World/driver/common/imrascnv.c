/***************************************************************************
 *                                                                         *
 * Module  : imrascnv.c                                                    *
 *                                                                         *
 * Purpose : Raster/Image conversion                                       *
 *                                                                         *
 **************************************************************************/

/***************************************************************************
 Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "baraster.h"
#include "baimage.h"
#include "barwtyp.h"

/* String abstraction API for unicode support */
#include "rwstring.h"

/* This files header */
#include "imrascnv.h"


/****************************************************************************
 Local Defines
 */

/* Colour the levels of a mip map so they can be seen */
#define SHOWLEVELx

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static mipmapPixFn mipmapPixFns[32] = 
  {(mipmapPixFn)NULL,               /*  1 */
   (mipmapPixFn)NULL,               /*  2 */
   (mipmapPixFn)NULL,               /*  3 */
   (mipmapPixFn)NULL,               /*  4 */
   (mipmapPixFn)NULL,               /*  5 */
   (mipmapPixFn)NULL,               /*  6 */
   (mipmapPixFn)NULL,               /*  7 */
   _rwRGBAFromImage8,               /*  8 */
   (mipmapPixFn)NULL,               /*  9 */
   (mipmapPixFn)NULL,               /* 10 */
   (mipmapPixFn)NULL,               /* 11 */
   (mipmapPixFn)NULL,               /* 12 */
   (mipmapPixFn)NULL,               /* 13 */
   (mipmapPixFn)NULL,               /* 14 */
   (mipmapPixFn)NULL,               /* 15 */
   (mipmapPixFn)NULL,               /* 16 */
   (mipmapPixFn)NULL,               /* 17 */
   (mipmapPixFn)NULL,               /* 18 */
   (mipmapPixFn)NULL,               /* 19 */
   (mipmapPixFn)NULL,               /* 20 */
   (mipmapPixFn)NULL,               /* 21 */
   (mipmapPixFn)NULL,               /* 22 */
   (mipmapPixFn)NULL,               /* 23 */
   (mipmapPixFn)NULL,               /* 24 */
   (mipmapPixFn)NULL,               /* 25 */
   (mipmapPixFn)NULL,               /* 26 */
   (mipmapPixFn)NULL,               /* 27 */
   (mipmapPixFn)NULL,               /* 28 */
   (mipmapPixFn)NULL,               /* 29 */
   (mipmapPixFn)NULL,               /* 30 */
   (mipmapPixFn)NULL,               /* 31 */
   _rwRGBAFromImage32};             /* 32 */

/****************************************************************************
 Functions
 */

/****************************************************************************
 rwRGBAFromImage8

 Extracts from an 8 bit image a mip mapped pixel

 On entry   : RwRGBA
            : Pixel pointer
            : Image
            : Size (as a power of 2 (1==2 pixels))
 On exit    :
 */

void
_rwRGBAFromImage8(RwRGBA *colOut, const RwUInt8 *pixel, const RwImage *image, RwInt32 size)
{
    RwInt32 red, green, blue, alpha;
    RwInt32 x, y;
    RwRGBA  *palette = RwImageGetPalette(image);
    RwInt32 useSize = (1 << size);

    RWFUNCTION(RWSTRING("_rwRGBAFromImage8"));

    red = green = blue = alpha = 0;
    for (y = 0; y < useSize; y++)
    {
        const RwUInt8 *cur = pixel;
        for (x = 0; x < useSize; x++)
        {
            const RwRGBA  *colIn = &palette[*cur];

            red += colIn->red;
            green += colIn->green;
            blue += colIn->blue;
            alpha += colIn->alpha;

            cur++;
        }

        pixel += RwImageGetStride(image);
    }

    size += size;

    /* Divide by the area (length squared) */
    colOut->red   = (RwUInt8)(red >> size);
    colOut->green = (RwUInt8)(green >> size);
    colOut->blue  = (RwUInt8)(blue >> size);
    colOut->alpha = (RwUInt8)(alpha >> size);

#ifdef SHOWLEVEL
    if (size & 2)
    {
        colOut->red = 255;
    }
    if (size & 4)
    {
        colOut->green = 255;
    }
    if (size & 8)
    {
        colOut->blue = 255;
    }
#endif /* SHOWLEVEL */

    RWRETURNVOID();
}

/****************************************************************************
 rwRGBAFromImage32

 Extracts from a 32 bit image a mip mapped pixel

 On entry   : RwRGBA
            : Pixel pointer
            : Image
            : Size (as a power of 2 (1==2 pixels))
 On exit    :
 */

void
_rwRGBAFromImage32(RwRGBA *colOut, const RwUInt8 *pixel, const RwImage *image, RwInt32 size)
{
    RwInt32 red, green, blue, alpha;
    RwInt32 x, y;
    RwInt32 useSize = (1 << size);

    RWFUNCTION(RWSTRING("_rwRGBAFromImage32"));

    red = green = blue = alpha = 0;
    for (y = 0; y < useSize; y++)
    {
        const RwRGBA *cur = (const RwRGBA *)pixel;
        for (x = 0; x < useSize; x++)
        {
            red += cur->red;
            green += cur->green;
            blue += cur->blue;
            alpha += cur->alpha;

            cur++;
        }

        pixel += RwImageGetStride(image);
    }

    size += size;

    /* Divide by the area (length squared) */
    colOut->red   = (RwUInt8)(red >> size);
    colOut->green = (RwUInt8)(green >> size);
    colOut->blue  = (RwUInt8)(blue >> size);
    colOut->alpha = (RwUInt8)(alpha >> size);

#ifdef SHOWLEVEL
    if (size & 2)
    {
        colOut->red = 255;
    }
    if (size & 4)
    {
        colOut->green = 255;
    }
    if (size & 8)
    {
        colOut->blue = 255;
    }
#endif /* SHOWLEVEL */

    RWRETURNVOID();
}

/****************************************************************************
 rwImageFindAlphaType

 Extracts from an 32 bit image a mip mapped pixel

 On entry   : RwRGBA
            : Pixel pointer
            : Image
            : Size (as a power of 2 (1==2 pixels))
 On exit    :
 */

rwImageAlphaType 
_rwImageFindAlphaType(RwImage *image)
{
    RwInt32 x, y;
    RwBool  masked = FALSE;
    RwInt32 width, height, stride;

    RWFUNCTION(RWSTRING("_rwImageFindAlphaType"));
    RWASSERT(image);

    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);
    stride = RwImageGetStride(image);

    switch (RwImageGetDepth(image))
    {
        case (4):
        case (8):
        {
            /* Its a 4 or 8 bit image */
            RwUInt8     *imagePixels = image->cpPixels;
            RwRGBA      *palette = image->palette;

            for (y = 0; y < height; y++)
            {
                RwUInt8     *cpInCur = imagePixels;

                for (x = 0; x < width; x++)
                {
                    if (palette[*cpInCur].alpha < 0xf0)
                    {
                        masked = TRUE;

                        if (palette[*cpInCur].alpha > 0xf)
                        {
                            /* Masked and has an alpha channel */
                            RWRETURN(rwIMAGEALPHA_SMOOTH);
                        }
                    }

                    /* Next pixel */
                    cpInCur++;
                }

                imagePixels += stride;
            }
            break;
        }
        case (32):
        {
            /* Its a 32 bit image */
            RwUInt8     *imagePixels = image->cpPixels;

            for (y = 0; y < height; y++)
            {
                RwRGBA *rpInCur = (RwRGBA *)imagePixels;

                for (x = 0; x < width; x++)
                {
                    if (rpInCur->alpha != 0xff)
                    {
                        masked = TRUE;

                        if (rpInCur->alpha > 0xf)
                        {
                            /* Masked and has an alpha channel */
                            RWRETURN(rwIMAGEALPHA_SMOOTH);
                        }
                    }

                    /* Next pixel */
                    rpInCur++;
                }

                imagePixels += stride;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    if (masked)
    {
        RWRETURN(rwIMAGEALPHA_MASKED);
    }
    else
    {
        RWRETURN(rwIMAGEALPHA_OPAQUE);
    }
}

/****************************************************************************
 rwImageFindNearestPaletteColor

 Finds nearest color in a palette to a given color

 On entry   : Palette to match to
            : Color to match
            : Size of the palette
 On exit    : Palette index of nearest color
 */

RwInt32 
_rwImageFindNearestPaletteColor(RwRGBA *palette, RwRGBA *color, RwInt32 palSize)
{
    RwInt32 nearestColor, nearestError, i;

    RWFUNCTION(RWSTRING("_rwImageFindNearestPaletteColor"));
    RWASSERT(palette);
    RWASSERT(color);
    RWASSERT(palSize);

    nearestError = 0x7FFFFFFF;
    nearestColor = 0;

    for (i = 0; i < palSize; i++)
    {
        RwRGBA  colorError;
        RwInt32 error;

        /* Calculate error, and scale according to importance of RGBA components.
         * Assign a touchy/feely value to the alpha importance.
         */
        colorError.red   = color->red   - palette->red;
        colorError.green = color->green - palette->green;
        colorError.blue  = color->blue  - palette->blue;
        colorError.alpha = color->alpha - palette->alpha;
        error = (30 * (colorError.red   * colorError.red)) +
                (59 * (colorError.green * colorError.green)) +
                (11 * (colorError.blue  * colorError.blue)) +
                (50 * (colorError.alpha * colorError.alpha));
        if (error < nearestError)
        {
            /* Found a new color */
            nearestColor = i;
            nearestError = error;
        }
        palette++;
    }

    RWRETURN(nearestColor);
}

/****************************************************************************
 rwImageSetFromRaster8888

 Fill a 32 bit image from 8888 raster pixels

 On entry   : Image (MODIFY)
            : Raster pixel pointer
            : Raster pixel stride
 On exit    : Image pointer on success
 */
RwImage *
_rwImageSetFromRaster8888(RwImage *image, RwUInt8 *rasPixels, RwInt32 rasStride)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwImageSetFromRaster8888"));
    RWASSERT(image);
    RWASSERT(rasPixels);

    imagePixels = RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);
    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);

    for (y = 0; y < height; y++)
    {
        RwRGBA *rpOut = (RwRGBA *)imagePixels;
        RwUInt32 *npIn = (RwUInt32 *)rasPixels;

        for (x = 0; x < width; x++)
        {
            RwUInt32 pixel = (*npIn);

            CONV8888TORGBA(rpOut, pixel);

            rpOut++;
            npIn++;
        }

        imagePixels += imageStride;
        rasPixels += rasStride;
    }

    RWRETURN(image);
}

/****************************************************************************
 rwImageSetFromRaster888

 Fill a 32 bit image from 888 raster pixels

 On entry   : Image (MODIFY)
            : Raster pixel pointer
            : Raster pixel stride
 On exit    : Image pointer on success
 */
RwImage *
_rwImageSetFromRaster888(RwImage *image, RwUInt8 *rasPixels, RwInt32 rasStride)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwImageSetFromRaster888"));
    RWASSERT(image);
    RWASSERT(rasPixels);

    imagePixels = RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);
    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);

    for (y = 0; y < height; y++)
    {
        RwRGBA *rpOut = (RwRGBA *)imagePixels;
        RwUInt32 *npIn = (RwUInt32 *)rasPixels;

        for (x = 0; x < width; x++)
        {
            RwUInt32 pixel = (*npIn);

            CONV888TORGBA(rpOut, pixel);

            rpOut++;
            npIn++;
        }

        imagePixels += imageStride;
        rasPixels += rasStride;
    }

    RWRETURN(image);
}

/****************************************************************************
 rwImageSetFromRaster4444

 Fill a 32 bit image from 4444 raster pixels

 On entry   : Image (MODIFY)
            : Raster pixel pointer
            : Raster pixel stride
 On exit    : Image pointer on success
 */
RwImage *
_rwImageSetFromRaster4444(RwImage *image, RwUInt8 *rasPixels, RwInt32 rasStride)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwImageSetFromRaster4444"));
    RWASSERT(image);
    RWASSERT(rasPixels);

    imagePixels = RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);
    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);

    for (y = 0; y < height; y++)
    {
        RwRGBA *rpOut = (RwRGBA *)imagePixels;
        RwUInt16 *npIn = (RwUInt16 *)rasPixels;

        for (x = 0; x < width; x++)
        {
            RwUInt16 pixel = (*npIn);

            CONV8888TORGBA(rpOut, pixel);

            rpOut++;
            npIn++;
        }

        imagePixels += imageStride;
        rasPixels += rasStride;
    }

    RWRETURN(image);
}

/****************************************************************************
 rwImageSetFromRaster1555

 Fill a 32 bit image from 1555 raster pixels

 On entry   : Image (MODIFY)
            : Raster pixel pointer
            : Raster pixel stride
 On exit    : Image pointer on success
 */
RwImage *
_rwImageSetFromRaster1555(RwImage *image, RwUInt8 *rasPixels, RwInt32 rasStride)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwImageSetFromRaster1555"));
    RWASSERT(image);
    RWASSERT(rasPixels);

    imagePixels = RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);
    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);

    for (y = 0; y < height; y++)
    {
        RwRGBA *rpOut = (RwRGBA *)imagePixels;
        RwUInt16 *npIn = (RwUInt16 *)rasPixels;

        for (x = 0; x < width; x++)
        {
            RwUInt16 pixel = (*npIn);

            CONV1555TORGBA(rpOut, pixel);

            rpOut++;
            npIn++;
        }

        imagePixels += imageStride;
        rasPixels += rasStride;
    }

    RWRETURN(image);
}

/****************************************************************************
 rwImageSetFromRaster565

 Fill a 32 bit image from 565 raster pixels

 On entry   : Image (MODIFY)
            : Raster pixel pointer
            : Raster pixel stride
 On exit    : Image pointer on success
 */
RwImage *
_rwImageSetFromRaster565(RwImage *image, RwUInt8 *rasPixels, RwInt32 rasStride)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwImageSetFromRaster565"));
    RWASSERT(image);
    RWASSERT(rasPixels);

    imagePixels = RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);
    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);

    for (y = 0; y < height; y++)
    {
        RwRGBA *rpOut = (RwRGBA *)imagePixels;
        RwUInt16 *npIn = (RwUInt16 *)rasPixels;

        for (x = 0; x < width; x++)
        {
            RwUInt16 pixel = (*npIn);

            CONV565TORGBA(rpOut, pixel);

            rpOut++;
            npIn++;
        }

        imagePixels += imageStride;
        rasPixels += rasStride;
    }

    RWRETURN(image);
}

/****************************************************************************
 rwMipmapRasterSetFromImage8888

 Set a mipmapped 8888 raster level from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
            : Mipmap level (0->n)
 On exit    : TRUE on success
 */
RwBool
_rwMipmapRasterSetFromImage8888(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image, RwInt32 level)
{
    mipmapPixFn   getMipmapTexel;
    RwUInt8       *imagePixels;
    RwInt32       targetHeight, targetWidth, x, y, xStep, yStep;

    RWFUNCTION(RWSTRING("_rwMipmapRasterSetFromImage8888"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    getMipmapTexel = mipmapPixFns[RwImageGetDepth(image)-1];
    imagePixels = RwImageGetPixels(image);
    targetHeight = RwImageGetHeight(image) >> level;
    targetWidth = RwImageGetWidth(image) >> level;
    yStep = RwImageGetStride(image) << level;
    xStep = 1 << level;

    /* Convert it */
    for (y = 0; y < targetHeight; y++)
    {
        RwUInt8    *inCur = imagePixels;
        RwUInt32   *outCur = (RwUInt32 *)rasPixels;

        for (x = 0; x < targetWidth; x++)
        {
            RwRGBA    colOut;

            getMipmapTexel(&colOut, inCur, image, level);

            (*outCur) = CONVRGBATO8888(&colOut);

            inCur += xStep;
            outCur++;
        }

        imagePixels += yStep;
        rasPixels += rasStride;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwMipmapRasterSetFromImage888

 Set a mipmapped 888 raster level from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
            : Mipmap level (0->n)
 On exit    : TRUE on success
 */
RwBool
_rwMipmapRasterSetFromImage888(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image, RwInt32 level)
{
    mipmapPixFn   getMipmapTexel;
    RwUInt8       *imagePixels;
    RwInt32       targetHeight, targetWidth, x, y, xStep, yStep;

    RWFUNCTION(RWSTRING("_rwMipmapRasterSetFromImage888"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    getMipmapTexel = mipmapPixFns[RwImageGetDepth(image)-1];
    imagePixels = RwImageGetPixels(image);
    targetHeight = RwImageGetHeight(image) >> level;
    targetWidth = RwImageGetWidth(image) >> level;
    yStep = RwImageGetStride(image) << level;
    xStep = 1 << level;

    /* Convert it */
    for (y = 0; y < targetHeight; y++)
    {
        RwUInt8    *inCur = imagePixels;
        RwUInt32   *outCur = (RwUInt32 *)rasPixels;

        for (x = 0; x < targetWidth; x++)
        {
            RwRGBA    colOut;

            getMipmapTexel(&colOut, inCur, image, level);

            (*outCur) = CONVRGBATO888(&colOut);

            inCur += xStep;
            outCur++;
        }

        imagePixels += yStep;
        rasPixels += rasStride;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwMipmapRasterSetFromImage4444

 Set a mipmapped 4444 raster level from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
            : Mipmap level (0->n)
 On exit    : TRUE on success
 */
RwBool
_rwMipmapRasterSetFromImage4444(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image, RwInt32 level)
{
    mipmapPixFn   getMipmapTexel;
    RwUInt8       *imagePixels;
    RwInt32       targetHeight, targetWidth, x, y, xStep, yStep;

    RWFUNCTION(RWSTRING("_rwMipmapRasterSetFromImage4444"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    getMipmapTexel = mipmapPixFns[RwImageGetDepth(image)-1];
    imagePixels = RwImageGetPixels(image);
    targetHeight = RwImageGetHeight(image) >> level;
    targetWidth = RwImageGetWidth(image) >> level;
    yStep = RwImageGetStride(image) << level;
    xStep = 1 << level;

    /* Convert it */
    for (y = 0; y < targetHeight; y++)
    {
        RwUInt8    *inCur = imagePixels;
        RwUInt16   *outCur = (RwUInt16 *)rasPixels;

        for (x = 0; x < targetWidth; x++)
        {
            RwRGBA    colOut;

            getMipmapTexel(&colOut, inCur, image, level);

            (*outCur) = (RwUInt16)CONVRGBATO4444(&colOut);

            inCur += xStep;
            outCur++;
        }

        imagePixels += yStep;
        rasPixels += rasStride;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwMipmapRasterSetFromImage1555

 Set a mipmapped 1555 raster level from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
            : Mipmap level (0->n)
 On exit    : TRUE on success
 */
RwBool
_rwMipmapRasterSetFromImage1555(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image, RwInt32 level)
{
    mipmapPixFn   getMipmapTexel;
    RwUInt8       *imagePixels;
    RwInt32       targetHeight, targetWidth, x, y, xStep, yStep;

    RWFUNCTION(RWSTRING("_rwMipmapRasterSetFromImage1555"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    getMipmapTexel = mipmapPixFns[RwImageGetDepth(image)-1];
    imagePixels = RwImageGetPixels(image);
    targetHeight = RwImageGetHeight(image) >> level;
    targetWidth = RwImageGetWidth(image) >> level;
    yStep = RwImageGetStride(image) << level;
    xStep = 1 << level;

    /* Convert it */
    for (y = 0; y < targetHeight; y++)
    {
        RwUInt8    *inCur = imagePixels;
        RwUInt16   *outCur = (RwUInt16 *)rasPixels;

        for (x = 0; x < targetWidth; x++)
        {
            RwRGBA    colOut;

            getMipmapTexel(&colOut, inCur, image, level);

            (*outCur) = (RwUInt16)CONVRGBATO1555(&colOut);

            inCur += xStep;
            outCur++;
        }

        imagePixels += yStep;
        rasPixels += rasStride;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwMipmapRasterSetFromImage565

 Set a mipmapped 565 raster level from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
            : Mipmap level (0->n)
 On exit    : TRUE on success
 */
RwBool
_rwMipmapRasterSetFromImage565(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image, RwInt32 level)
{
    mipmapPixFn   getMipmapTexel;
    RwUInt8       *imagePixels;
    RwInt32       targetHeight, targetWidth, x, y, xStep, yStep;

    RWFUNCTION(RWSTRING("_rwMipmapRasterSetFromImage565"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    getMipmapTexel = mipmapPixFns[RwImageGetDepth(image)-1];
    imagePixels = RwImageGetPixels(image);
    targetHeight = RwImageGetHeight(image) >> level;
    targetWidth = RwImageGetWidth(image) >> level;
    yStep = RwImageGetStride(image) << level;
    xStep = 1 << level;

    /* Convert it */
    for (y = 0; y < targetHeight; y++)
    {
        RwUInt8    *inCur = imagePixels;
        RwUInt16   *outCur = (RwUInt16 *)rasPixels;

        for (x = 0; x < targetWidth; x++)
        {
            RwRGBA    colOut;

            getMipmapTexel(&colOut, inCur, image, level);

            (*outCur) = (RwUInt16)CONVRGBATO565(&colOut);

            inCur += xStep;
            outCur++;
        }

        imagePixels += yStep;
        rasPixels += rasStride;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwRasterSetFromImage8888

 Set a non mipmapped 8888 raster from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
 On exit    : TRUE on success
 */
RwBool
_rwRasterSetFromImage8888(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwRasterSetFromImage8888"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);
    imagePixels = (RwUInt8 *)RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);

    switch (image->depth)
    {
        case 4:
        case 8:
        {
            /* Its a 4 or 8 bit image */
            RwRGBA *palette = image->palette;

            for (y = 0; y < height; y++)
            {
                RwUInt32 *npOut = (RwUInt32 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    RwRGBA *rpIn = &palette[imagePixels[x]];

                    (*npOut) = CONVRGBATO8888(rpIn);

                    /* Next pixel */
                    npOut++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        case 32:
        {
            /* Its a 32 bit image */
            for (y = 0; y < height; y++)
            {
                RwRGBA *rpIn = (RwRGBA *)imagePixels;
                RwUInt32 *npOut = (RwUInt32 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    (*npOut) = CONVRGBATO8888(rpIn);

                    /* Next pixel */
                    npOut++;
                    rpIn++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        default:
        {
            /* Unrecognised image depth */
            RWRETURN(FALSE);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwRasterSetFromImage888

 Set a non mipmapped 888 raster from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
 On exit    : TRUE on success
 */
RwBool
_rwRasterSetFromImage888(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwRasterSetFromImage888"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);
    imagePixels = (RwUInt8 *)RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);

    switch (image->depth)
    {
        case 4:
        case 8:
        {
            /* Its a 4 or 8 bit image */
            RwRGBA *palette = image->palette;

            for (y = 0; y < height; y++)
            {
                RwUInt32 *npOut = (RwUInt32 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    RwRGBA *rpIn = &palette[imagePixels[x]];

                    (*npOut) = CONVRGBATO888(rpIn);

                    /* Next pixel */
                    npOut++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        case 32:
        {
            /* Its a 32 bit image */
            for (y = 0; y < height; y++)
            {
                RwRGBA *rpIn = (RwRGBA *)imagePixels;
                RwUInt32 *npOut = (RwUInt32 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    (*npOut) = CONVRGBATO888(rpIn);

                    /* Next pixel */
                    npOut++;
                    rpIn++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        default:
        {
            /* Unrecognised image depth */
            RWRETURN(FALSE);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwRasterSetFromImage4444

 Set a non mipmapped 4444 raster from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
 On exit    : TRUE on success
 */
RwBool
_rwRasterSetFromImage4444(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwRasterSetFromImage4444"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);
    imagePixels = (RwUInt8 *)RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);

    switch (image->depth)
    {
        case 4:
        case 8:
        {
            /* Its a 4 or 8 bit image */
            RwRGBA *palette = image->palette;

            for (y = 0; y < height; y++)
            {
                RwUInt16 *npOut = (RwUInt16 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    RwRGBA *rpIn = &palette[imagePixels[x]];

                    (*npOut) = (RwUInt16)CONVRGBATO4444(rpIn);

                    /* Next pixel */
                    npOut++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        case 32:
        {
            /* Its a 32 bit image */
            for (y = 0; y < height; y++)
            {
                RwRGBA *rpIn = (RwRGBA *)imagePixels;
                RwUInt16 *npOut = (RwUInt16 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    (*npOut) = (RwUInt16)CONVRGBATO4444(rpIn);

                    /* Next pixel */
                    npOut++;
                    rpIn++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        default:
        {
            /* Unrecognised image depth */
            RWRETURN(FALSE);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwRasterSetFromImage1555

 Set a non mipmapped 1555 raster from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
 On exit    : TRUE on success
 */
RwBool
_rwRasterSetFromImage1555(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwRasterSetFromImage1555"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);
    imagePixels = (RwUInt8 *)RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);

    switch (image->depth)
    {
        case 4:
        case 8:
        {
            /* Its a 4 or 8 bit image */
            RwRGBA *palette = image->palette;

            for (y = 0; y < height; y++)
            {
                RwUInt16 *npOut = (RwUInt16 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    RwRGBA *rpIn = &palette[imagePixels[x]];

                    (*npOut) = (RwUInt16)CONVRGBATO1555(rpIn);

                    /* Next pixel */
                    npOut++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        case 32:
        {
            /* Its a 32 bit image */
            for (y = 0; y < height; y++)
            {
                RwRGBA *rpIn = (RwRGBA *)imagePixels;
                RwUInt16 *npOut = (RwUInt16 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    (*npOut) = (RwUInt16)CONVRGBATO1555(rpIn);

                    /* Next pixel */
                    npOut++;
                    rpIn++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        default:
        {
            /* Unrecognised image depth */
            RWRETURN(FALSE);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwRasterSetFromImage565

 Set a non mipmapped 565 raster from an image

 On entry   : Raster pixel pointer
            : Raster pixel stride
            : Image
 On exit    : TRUE on success
 */
RwBool
_rwRasterSetFromImage565(RwUInt8 *rasPixels, RwInt32 rasStride, RwImage *image)
{
    RwInt32     x, y, width, height;
    RwUInt8     *imagePixels;
    RwInt32     imageStride;

    RWFUNCTION(RWSTRING("_rwRasterSetFromImage565"));
    RWASSERT(rasPixels);
    RWASSERT(image);

    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);
    imagePixels = (RwUInt8 *)RwImageGetPixels(image);
    imageStride = RwImageGetStride(image);

    switch (image->depth)
    {
        case 4:
        case 8:
        {
            /* Its a 4 or 8 bit image */
            RwRGBA *palette = image->palette;

            for (y = 0; y < height; y++)
            {
                RwUInt16 *npOut = (RwUInt16 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    RwRGBA *rpIn = &palette[imagePixels[x]];

                    (*npOut) = (RwUInt16)CONVRGBATO565(rpIn);

                    /* Next pixel */
                    npOut++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        case 32:
        {
            /* Its a 32 bit image */
            for (y = 0; y < height; y++)
            {
                RwRGBA *rpIn = (RwRGBA *)imagePixels;
                RwUInt16 *npOut = (RwUInt16 *)rasPixels;

                for (x = 0; x < width; x++)
                {
                    (*npOut) = (RwUInt16)CONVRGBATO565(rpIn);

                    /* Next pixel */
                    npOut++;
                    rpIn++;
                }

                rasPixels += rasStride;
                imagePixels += imageStride;
            }

            break;
        }
        default:
        {
            /* Unrecognised image depth */
            RWRETURN(FALSE);
        }
    }

    RWRETURN(TRUE);
}

