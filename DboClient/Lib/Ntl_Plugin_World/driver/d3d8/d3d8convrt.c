/*****************************************************************************
 *                                                                           *
 * Module  : D3D8convrt.c                                                      *
 *                                                                           *
 * Purpose : Converting to and from images to rasters                        *
 *                                                                           *
 ****************************************************************************/

#define MIPCOLx
#define MIPCOLPALx
#define NOPAL4TEXTURESx
#define NOPAL8TEXTURESx

/*****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "barwtyp.h"
#include "baresour.h"
#include "baimage.h"
#include "badebug.h"
#include "baresamp.h"

#include "palquant.h"

#include "drvfns.h"

#include "d3d8convrt.h"
#include "d3d8raster.h"
#include "d3d8device.h"


/*****************************************************************************
 Global prototypes
 */

/*****************************************************************************
 Local Types
 */

typedef RwUInt32 (*D3D8PixConvertFn)(RwRGBA *colIn);
typedef void (*D3D8PixUnconvertFn)(RwRGBA *pixOut, RwUInt32 pixIn);

/*****************************************************************************
 Local (Static) Prototypes
 */

/*****************************************************************************
 Globals (across program)
 */

/*****************************************************************************
 Local (static) Globals
 */

#if defined(MIPCOL) || defined(MIPCOLPAL)
#define NUMMIPCOLS  10

static RwRGBA   MipMapCol[10] = {{0xff, 0x00, 0x00, 0xFF}, /* Red */
                                 {0x00, 0xff, 0x00, 0xFF}, /* Green */
                                 {0x00, 0x00, 0xff, 0xFF}, /* Blue */
                                 {0xff, 0xff, 0x00, 0xFF}, /* Yellow */
                                 {0xff, 0x00, 0xff, 0xFF}, /* Magenta */
                                 {0x00, 0xff, 0xff, 0xFF}, /* Cyan */
                                 {0x80, 0x80, 0x80, 0xFF}, /* Grey */
                                 {0xff, 0x80, 0x80, 0xFF}, /* Pink */
                                 {0xff, 0xff, 0xff, 0xFF}, /* White */
                                 {0x00, 0x00, 0x00, 0xFF}  /* Black*/
                                };
#endif /* defined(MIPCOL) || defined(MIPCOLPAL) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                         Conversion functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwD3D8FindMSB

 On entry   : LONG
 On exit    : Bit number of MSB (-1 if 0)
 */
RwInt32
_rwD3D8FindMSB(RwInt32 num)
{
    RwInt32 pos = -1;

    RWFUNCTION(RWSTRING("_rwD3D8FindMSB"));

    while (num)
    {
        pos++;
        num >>= 1;
    }

    RWRETURN(pos);
}

/****************************************************************************
 _rwD3D8CheckRasterSize

 On entry   : width and height to check
  */
void 
_rwD3D8CheckRasterSize(RwInt32 *width, RwInt32 *height, RwUInt32 rasterFlags)
{
    RWFUNCTION(RWSTRING("_rwD3D8CheckRasterSize"));

    /* Clamp the raster to the max texture size */
    if (*width > (RwInt32)_RwD3D8DeviceCaps.MaxTextureWidth)
    {
        *width = _RwD3D8DeviceCaps.MaxTextureWidth;
    }
    
    if (*height > (RwInt32)_RwD3D8DeviceCaps.MaxTextureHeight)
    {
        *height = _RwD3D8DeviceCaps.MaxTextureHeight;
    }

    if ((rasterFlags & rwRASTERTYPEMASK) == rwRASTERTYPENORMAL ||
        (rasterFlags & rwRASTERFORMATMIPMAP) == 0)
    {
        if ( (_RwD3D8DeviceCaps.TextureCaps & D3DPTEXTURECAPS_POW2) != 0 &&
             (_RwD3D8DeviceCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) == 0)
        {
            /* Make it a power of two (round down) */
            *width = 1 << _rwD3D8FindMSB(*width);
            *height = 1 << _rwD3D8FindMSB(*height);
        }
    }
    else
    {
        if (_RwD3D8DeviceCaps.TextureCaps & D3DPTEXTURECAPS_POW2)
        {
            /* Make it a power of two (round down) */
            *width = 1 << _rwD3D8FindMSB(*width);
            *height = 1 << _rwD3D8FindMSB(*height);
        }
    }

    if (_RwD3D8DeviceCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY)
    {
        /* Make it square of smallest dimension */
        if (*width < *height)
        {
            *height = *width;
        }
        else
        {
            *width = *height;
        }
    }

    RWRETURNVOID();
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*
 *                                                                           *
 *                           - Pixel conversion -                            *
 *                                                                           *
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*****************************************************************************
 rwD3D8ConvR8G8B8A8ToR5G5B5

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32 
rwD3D8ConvR8G8B8A8ToR5G5B5(RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("rwD3D8ConvR8G8B8A8ToR5G5B5"));

    pixOut = (0x00000000) | /* Err slam a 0 in here, it ain't used anyway */
             (((RwUInt32)pixIn->red << 7) & 0x7C00) |
             (((RwUInt32)pixIn->green << 2) & 0x03E0) |
             (((RwUInt32)pixIn->blue >> 3) & 0x001F);

    RWRETURN(pixOut);
}

/*****************************************************************************
 rwD3D8ConvR8G8B8A8ToR5G6B5

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32 
rwD3D8ConvR8G8B8A8ToR5G6B5(RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("rwD3D8ConvR8G8B8A8ToR5G6B5"));

    pixOut = (((RwUInt32)pixIn->red << 8) & 0xF800) |
             (((RwUInt32)pixIn->green << 3) & 0x07E0) |
             ((RwUInt32)pixIn->blue >> 3);

    RWRETURN(pixOut);
}

/*****************************************************************************
 rwD3D8ConvR8G8B8A8ToA1R5G5B5

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32 
rwD3D8ConvR8G8B8A8ToA1R5G5B5(RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("rwD3D8ConvR8G8B8A8ToA1R5G5B5"));

    pixOut = (((RwUInt32)pixIn->alpha << 8) & 0x8000) |
             (((RwUInt32)pixIn->red << 7) & 0x7C00) |
             (((RwUInt32)pixIn->green << 2) & 0x03E0) |
             ((RwUInt32)pixIn->blue >> 3);

    RWRETURN(pixOut);
}

/*****************************************************************************
 rwD3D8ConvR8G8B8A8ToA4R4G4B4

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32 
rwD3D8ConvR8G8B8A8ToA4R4G4B4(RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("rwD3D8ConvR8G8B8A8ToA4R4G4B4"));

    pixOut = (((RwUInt32)pixIn->alpha << 8) & 0xF000) |
             (((RwUInt32)pixIn->red << 4) & 0x0F00) |
             (((RwUInt32)pixIn->green) & 0x00F0) |
             ((RwUInt32)pixIn->blue >> 4);

    RWRETURN(pixOut);
}

/*****************************************************************************
 rwD3D8ConvR8G8B8A8ToX8R8G8B8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32 
rwD3D8ConvR8G8B8A8ToX8R8G8B8(RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("rwD3D8ConvR8G8B8A8ToX8R8G8B8"));

    pixOut = (((RwUInt32)0xFF << 24) & 0xFF000000) |
             (((RwUInt32)pixIn->red << 16) & 0x00FF0000) |
             (((RwUInt32)pixIn->green << 8) & 0x0000FF00) |
             ((RwUInt32)pixIn->blue);

    RWRETURN(pixOut);
}

/*****************************************************************************
 rwD3D8ConvR8G8B8A8ToA8R8G8B8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32 
rwD3D8ConvR8G8B8A8ToA8R8G8B8(RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("rwD3D8ConvR8G8B8A8ToA8R8G8B8"));

    pixOut = (((RwUInt32)pixIn->alpha << 24) & 0xFF000000) |
             (((RwUInt32)pixIn->red << 16) & 0x00FF0000) |
             (((RwUInt32)pixIn->green << 8) & 0x0000FF00) |
             ((RwUInt32)pixIn->blue);

    RWRETURN(pixOut);
}

/*****************************************************************************
 rwD3D8ConvR8G8B8A8ToL8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32 
rwD3D8ConvR8G8B8A8ToL8(RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("rwD3D8ConvR8G8B8A8ToL8"));

    pixOut  =   ((pixIn->red * 30 +
                  pixIn->green * 59 +
                  pixIn->blue * 11) / 100);

    pixOut  =  ((pixOut * (pixIn->alpha)) / 0xff);

    RWRETURN(pixOut);
}
/*****************************************************************************
 _rwD3D8RGBToPixel

 Convert RwRGBA to device specific format

 On entry   : pixel - device specific colour out
            : col - colour to convert
            : format - pixel format
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RGBToPixel(void *pixelOut, void *colIn, RwInt32 format)
{
    RwRGBA  *rgba = (RwRGBA *)colIn;
    RwInt32 pixVal;

    RWFUNCTION(RWSTRING("_rwD3D8RGBToPixel"));

    switch (format & rwRASTERFORMATPIXELFORMATMASK)
    {
        case rwRASTERFORMATDEFAULT:
        {
            /* Default is 8888 */
            if (!_rwD3D8RGBToPixel(&pixVal, colIn, rwRASTERFORMAT8888))
            {
                RWRETURN(FALSE);
            }
            break;
        }

        case rwRASTERFORMAT555:
            pixVal = rwD3D8ConvR8G8B8A8ToR5G5B5(rgba);
            break;

        case rwRASTERFORMAT1555:
            pixVal = rwD3D8ConvR8G8B8A8ToA1R5G5B5(rgba);
            break;

        case rwRASTERFORMAT565:
            pixVal = rwD3D8ConvR8G8B8A8ToR5G6B5(rgba);
            break;

        case rwRASTERFORMAT4444:
            pixVal = rwD3D8ConvR8G8B8A8ToA4R4G4B4(rgba);
            break;

        case rwRASTERFORMATLUM8:
            pixVal = rwD3D8ConvR8G8B8A8ToL8(rgba);
            break;

        case rwRASTERFORMAT8888:
            pixVal = rwD3D8ConvR8G8B8A8ToA8R8G8B8(rgba);
            break;

        case rwRASTERFORMAT888:
            pixVal = rwD3D8ConvR8G8B8A8ToX8R8G8B8(rgba);
            break;

        default:
        {
            RWERROR((E_RW_INVRASTERFORMAT));
            RWRETURN(FALSE);
        }
    }

    *(RwInt32 *)pixelOut = pixVal;

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8ConvR5G5B5ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
rwD3D8ConvR5G5B5ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("rwD3D8ConvR5G5B5ToR8G8B8A8"));

    pixOut->alpha = 0xFF;
    pixOut->red   = (RwUInt8)((pixIn >> 7) & 0xF8);
    pixOut->green = (RwUInt8)((pixIn >> 2) & 0xF8);
    pixOut->blue  = (RwUInt8)((pixIn << 3) & 0xF8);

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ConvR5G6B5ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
rwD3D8ConvR5G6B5ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("rwD3D8ConvR5G6B5ToR8G8B8A8"));

    pixOut->alpha = 0xFF;
    pixOut->red   = (RwUInt8)((pixIn >> 8) & 0xF8);
    pixOut->green = (RwUInt8)((pixIn >> 3) & 0xFC);
    pixOut->blue  = (RwUInt8)((pixIn << 3) & 0xF8);

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ConvA1R5G5B5ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
rwD3D8ConvA1R5G5B5ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("rwD3D8ConvA1R5G5B5ToR8G8B8A8"));

    if (pixIn & 0x8000)
    {
        pixOut->alpha = 0xFF;
    }
    else
    {
        pixOut->alpha = 0x00;
    }

    pixOut->red   = (RwUInt8)((pixIn >> 7) & 0xF8);
    pixOut->green = (RwUInt8)((pixIn >> 2) & 0xF8);
    pixOut->blue  = (RwUInt8)((pixIn << 3) & 0xF8);

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ConvA4R4G4B4ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
rwD3D8ConvA4R4G4B4ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("rwD3D8ConvA4R4G4B4ToR8G8B8A8"));

    if ((pixIn & 0xF000) == 0xF000)
    {
        pixOut->alpha = 0xFF;
    }
    else
    {
        pixOut->alpha = (RwUInt8)((pixIn >> 8) & 0xF0);
    }

    pixOut->red   = (RwUInt8)((pixIn >> 4) & 0xF0);
    pixOut->green = (RwUInt8)((pixIn >> 0) & 0xF0);
    pixOut->blue  = (RwUInt8)((pixIn << 4) & 0xF0);

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ConvX8R8G8B8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
rwD3D8ConvX8R8G8B8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("rwD3D8ConvX8R8G8B8ToR8G8B8A8"));
    
    pixOut->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
    pixOut->green = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->blue  = (RwUInt8)((pixIn) & 0xFF);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ConvA8R8G8B8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
rwD3D8ConvA8R8G8B8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("rwD3D8ConvA8R8G8B8ToR8G8B8A8"));
    
    pixOut->alpha = (RwUInt8)((pixIn >> 24) & 0xFF);
    pixOut->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
    pixOut->green = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->blue  = (RwUInt8)((pixIn) & 0xFF);
    

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ConvL8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
rwD3D8ConvL8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("rwD3D8ConvL8ToR8G8B8A8"));
    
    pixOut->alpha = 0xFF;
    pixOut->red   = (RwUInt8)(pixIn & 0xFF);
    pixOut->green = (RwUInt8)(pixIn & 0xFF);
    pixOut->blue  = (RwUInt8)(pixIn & 0xFF);
    

    RWRETURNVOID();
}

/*****************************************************************************
 _rwD3D8PixelToRGB

 Convert device specific colour to RwRGBA

 On entry   : rgbOut - RwRGBA out
            : pixel - Pixel in
            : Pixel format
 On exit    : TRUE on success
 */
RwBool
_rwD3D8PixelToRGB(void *rgbOut, void *pixel, RwInt32 format)
{
    RwRGBA  *rgba = (RwRGBA *)rgbOut;

    RWFUNCTION(RWSTRING("_rwD3D8PixelToRGB"));

    if (format & rwRASTERFORMATPAL8)
    {
        *rgba = *((RwRGBA *)pixel);
    }
    else
    {
        switch (format & rwRASTERFORMATPIXELFORMATMASK)
        {
            case rwRASTERFORMATDEFAULT:
                if (!_rwD3D8PixelToRGB(rgba, pixel, rwRASTERFORMAT8888))
                {
                    RWRETURN(FALSE);
                }
                break;

            case rwRASTERFORMAT555:
                rwD3D8ConvR5G5B5ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT1555:
                rwD3D8ConvA1R5G5B5ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT565:
                rwD3D8ConvR5G6B5ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT4444:
                rwD3D8ConvA4R4G4B4ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMATLUM8:
                rwD3D8ConvL8ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT8888:
                rwD3D8ConvA8R8G8B8ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT888:
                rwD3D8ConvX8R8G8B8ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            default:
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
        }
    }

    RWRETURN(TRUE);
}


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*
 *                                                                           *
 *                      - Raster to image conversion -                       *
 *                                                                           *
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*****************************************************************************
 rwD3D8SelectUnconvertFn

 On entry :
 On exit  :
 */
static D3D8PixUnconvertFn
rwD3D8SelectUnconvertFn(RwRasterFormat format)
{
    D3D8PixUnconvertFn    result = NULL;

    RWFUNCTION(RWSTRING("rwD3D8SelectUnconvertFn"));

    switch (format & rwRASTERFORMATPIXELFORMATMASK)
    {
        case rwRASTERFORMAT555:
            result = rwD3D8ConvR5G5B5ToR8G8B8A8;
            break;

        case rwRASTERFORMAT565:
            result = rwD3D8ConvR5G6B5ToR8G8B8A8;
            break;

        case rwRASTERFORMAT1555:
            result = rwD3D8ConvA1R5G5B5ToR8G8B8A8;
            break;

        case rwRASTERFORMAT4444:
            result = rwD3D8ConvA4R4G4B4ToR8G8B8A8;
            break;

        case rwRASTERFORMATLUM8:
            result = rwD3D8ConvL8ToR8G8B8A8;
            break;

        case rwRASTERFORMAT888:
            result = rwD3D8ConvX8R8G8B8ToR8G8B8A8;
            break;

        case rwRASTERFORMAT8888:
            result = rwD3D8ConvA8R8G8B8ToR8G8B8A8;
            break;

        default:
            break;
    }

    RWRETURN(result);
}

/*****************************************************************************
 rwD3D8Image32GetFromRaster

 Convert raster into a 32bit image

 On entry   : image - destination image to receive the raster's image
            : raster - source raster to convert

 On exit    :
 */
static void
rwD3D8Image32GetFromRaster(RwImage *image, RwRaster *raster)
{
    RWFUNCTION(RWSTRING("rwD3D8Image32GetFromRaster"));

    switch (RwRasterGetDepth(raster))
    {
    case 4:
    case 8:
        {
            const PALETTEENTRY  *palette;
            const RwUInt8       *srcPixel;
            RwInt32             j;
        
            /* Get the palette and base pixel pointer */
            palette = (const PALETTEENTRY *)raster->palette;

            if (palette)
            {
                for (j = 0; j < raster->height; j++)
                {
                    RwInt32 i;
                    RwRGBA  *dstPixel;

                    srcPixel = (const RwUInt8 *)(raster->cpPixels + (raster->stride * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    i = raster->width;
                    do
                    {
                        const PALETTEENTRY *paletteEntry = &(palette[*srcPixel]);

                        *((RwUInt32 *)dstPixel) = *((const RwUInt32 *)paletteEntry);

                        srcPixel++;
                        dstPixel++;
                    }
                    while(--i);
                }
            }
            else
            {
                for (j = 0; j < raster->height; j++)
                {
                    RwInt32 i;
                    RwRGBA  *dstPixel;

                    srcPixel = (const RwUInt8 *)(raster->cpPixels + (raster->stride * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    i = raster->width;
                    do
                    {
                        const RwUInt32 greyvalue = *srcPixel;

                        *((RwUInt32 *)dstPixel) = (0xff000000 |
                                                   (greyvalue << 16) |
                                                   (greyvalue << 8) |
                                                   (greyvalue));

                        srcPixel++;
                        dstPixel++;
                    }
                    while(--i);
                }
            }
        }
        break;

    case 16:
        {
            const RwUInt16  *srcPixel;
            RwRGBA          *dstPixel;
            RwInt32         j, i;

            switch (RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK)
            {
                case rwRASTERFORMAT555:
                    for (j = 0; j < raster->height; j++)
                    {
                        srcPixel = (const RwUInt16 *)(raster->cpPixels + (raster->stride * j));
                        dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                        i = raster->width;
                        do
                        {
                            const RwUInt16 pixIn = *srcPixel;

                            dstPixel->red   = (RwUInt8)((pixIn >> 7) & 0xF8);
                            dstPixel->green = (RwUInt8)((pixIn >> 2) & 0xF8);
                            dstPixel->blue  = (RwUInt8)((pixIn << 3) & 0xF8);
                            dstPixel->alpha = 0xFF;

                            srcPixel++;
                            dstPixel++;
                        }
                        while(--i);
                    }
                    break;

                case rwRASTERFORMAT565:
                    for (j = 0; j < raster->height; j++)
                    {
                        srcPixel = (const RwUInt16 *)(raster->cpPixels + (raster->stride * j));
                        dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                        i = raster->width;
                        do
                        {
                            const RwUInt16 pixIn = *srcPixel;

                            dstPixel->red   = (RwUInt8)((pixIn >> 8) & 0xF8);
                            dstPixel->green = (RwUInt8)((pixIn >> 3) & 0xFC);
                            dstPixel->blue  = (RwUInt8)((pixIn << 3) & 0xF8);
                            dstPixel->alpha = 0xFF;

                            srcPixel++;
                            dstPixel++;
                        }
                        while(--i);
                    }
                    break;

                case rwRASTERFORMAT1555:
                    for (j = 0; j < raster->height; j++)
                    {
                        srcPixel = (const RwUInt16 *)(raster->cpPixels + (raster->stride * j));
                        dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                        i = raster->width;
                        do
                        {
                            const RwUInt16 pixIn = *srcPixel;

                            dstPixel->red   = (RwUInt8)((pixIn >> 7) & 0xF8);
                            dstPixel->green = (RwUInt8)((pixIn >> 2) & 0xF8);
                            dstPixel->blue  = (RwUInt8)((pixIn << 3) & 0xF8);

                            if (pixIn & 0x8000)
                            {
                                dstPixel->alpha = 0xFF;
                            }
                            else
                            {
                                dstPixel->alpha = 0x00;
                            }

                            srcPixel++;
                            dstPixel++;
                        }
                        while(--i);
                    }
                    break;

                case rwRASTERFORMAT4444:
                    for (j = 0; j < raster->height; j++)
                    {
                        srcPixel = (const RwUInt16 *)(raster->cpPixels + (raster->stride * j));
                        dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                        i = raster->width;
                        do
                        {
                            const RwUInt16 pixIn = *srcPixel;

                            dstPixel->red   = (RwUInt8)((pixIn >> 4) & 0xF0);
                            dstPixel->green = (RwUInt8)((pixIn >> 0) & 0xF0);
                            dstPixel->blue  = (RwUInt8)((pixIn << 4) & 0xF0);

                            if ((pixIn & 0xF000) == 0xF000)
                            {
                                dstPixel->alpha = 0xFF;
                            }
                            else
                            {
                                dstPixel->alpha = (RwUInt8)((pixIn >> 8) & 0xF0);
                            }

                            srcPixel++;
                            dstPixel++;
                        }
                        while(--i);
                    }
                    break;
            }
        }
        break;

    case 24:
    case 32:
        {
            const RwUInt32  *srcPixel;
            RwRGBA          *dstPixel;
            RwInt32         j, i;

            if ((RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK)==rwRASTERFORMAT888)
            {
                for (j = 0; j < raster->height; j++)
                {
                    srcPixel = (const RwUInt32 *)(raster->cpPixels + (raster->stride * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    i = raster->width;
                    do
                    {
                        const RwUInt32 pixIn = *srcPixel;

                        srcPixel++;

                        dstPixel->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
                        dstPixel->green = (RwUInt8)((pixIn >> 8) & 0xFF);
                        dstPixel->blue  = (RwUInt8)((pixIn) & 0xFF);
                        dstPixel->alpha = 0xFF;

                        dstPixel++;
                    }
                    while(--i);
                }
            }
            else if ((RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK)==rwRASTERFORMAT8888)
            {
                for (j = 0; j < raster->height; j++)
                {
                    srcPixel = (const RwUInt32 *)(raster->cpPixels + (raster->stride * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    i = raster->width;
                    do
                    {
                        const RwUInt32 pixIn = *srcPixel;

                        srcPixel++;

                        dstPixel->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
                        dstPixel->green = (RwUInt8)((pixIn >> 8) & 0xFF);
                        dstPixel->blue  = (RwUInt8)((pixIn) & 0xFF);
                        dstPixel->alpha = (RwUInt8)(pixIn >> 24);

                        dstPixel++;
                    }
                    while(--i);
                }
            }
        }
        break;

    default:
        RWERROR((E_RW_INVRASTERDEPTH));
        break;
    }

    RWRETURNVOID();
}

/*****************************************************************************
 rwD3D8Image8GetFromRaster

 Convert raster into a palettized image

 On entry   : image - destination image to receive the raster's image
            : raster - source raster to convert

 On exit    :
 */
static void
rwD3D8Image8GetFromRaster(RwImage *image, RwRaster *raster)
{
    RWFUNCTION(RWSTRING("rwD3D8Image8GetFromRaster"));

    switch (RwRasterGetDepth(raster))
    {
    case 8:
        {
            RwInt32         y;
            _rwD3D8RasterExt  *rasExt;

            /* Get the raster plugin data */
            rasExt = RASTEREXTFROMRASTER(raster);

            /* 8 bit image */
            if (image->stride == raster->stride && raster->width == raster->stride)
            {
                RwUInt8 *srcPixel;
                RwUInt8 *dstPixel;

                srcPixel = raster->cpPixels;
                dstPixel = image->cpPixels;

                memcpy(dstPixel, srcPixel, raster->height * raster->width);
            }
            else
            {
                for (y = 0; y < raster->height; y++)
                {
                    RwUInt8 *srcPixel;
                    RwUInt8 *dstPixel;

                    srcPixel = raster->cpPixels + (raster->stride * y);
                    dstPixel = image->cpPixels + (image->stride * y);

                    memcpy(dstPixel, srcPixel, raster->width);
                }
            }

            /* Set the palette */
            if (raster->palette)
            {
                memcpy(image->palette, raster->palette, sizeof(RwRGBA) * 256);
            }
            else
            {
                for (y = 0; y < 256; y++)
                {
                    image->palette[y].red = y;
                    image->palette[y].green = y;
                    image->palette[y].blue = y;
                    image->palette[y].alpha = 0xff;
                }
            }
        }
        break;

    case 16:
    case 24:
    case 32:
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("Conversion from non palettised rasters to 8bit images is not supported")));
        break;

    default:
        RWERROR((E_RW_INVRASTERDEPTH));
        break;
    }

    RWRETURNVOID();
}

/*****************************************************************************
 _rwD3D8ImageGetFromRaster

 Convert raster to image

 On entry   : image - destination image to receive the raster's image
            : raster - source raster to convert
 On exit    : TRUE on success
 */
RwBool
_rwD3D8ImageGetFromRaster(void *imageIn, 
                        void *rasterIn, 
                        RwInt32 unused3 __RWUNUSED__)
{
    RwImage     *image = (RwImage *)imageIn;
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwBool      rasterLocked = FALSE;
    RwBool      paletteLocked = FALSE;
    const _rwD3D8RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D8ImageGetFromRaster"));

    /* Check if raster is compressed and fail */
    rasExt = RASTEREXTFROMCONSTRASTER(raster);
    if (rasExt->dxtFormat)
    {
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("Conversion from compressed rasters is not supported.")));

        RWRETURN(FALSE);
    }

    /* Lock the raster if it isn't already locked */
    if (!(raster->privateFlags & rwRASTERPIXELLOCKEDREAD))
    {
        RwRasterLock(raster, 0, rwRASTERLOCKREAD);
        rasterLocked = TRUE;
    }

    /* If the raster is palletized then lock the palette */
    if (RwRasterGetFormat(raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        if (!(raster->privateFlags & rwRASTERPALETTELOCKEDREAD))
        {
            RwRasterLockPalette(raster, rwRASTERLOCKREAD);
            paletteLocked = TRUE;
        }
    }

    /* Only 32Bit Images from rasters are supported */
    if (32 == RwImageGetDepth(image))
    {
        rwD3D8Image32GetFromRaster(image, raster);
    }
    else if (8 == RwImageGetDepth(image))
    {
        rwD3D8Image8GetFromRaster(image, raster);
    }
    else
    {
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("Conversion from rasters to 4bit images is not supported")));
    }

    /* Unlock the palette if it was locked above */
    if (TRUE == paletteLocked)
    {
        RwRasterUnlockPalette(raster);
    }

    /* Unlock the raster if it was locked above */
    if (TRUE == rasterLocked)
    {
        RwRasterUnlock(raster);
    }

    /* Something went wrong */
    RWRETURN(TRUE);
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*
 *                                                                           *
 *                 - Finding an appropriate raster format -                  *
 *                                                                           *
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*****************************************************************************
 rwD3D8ImageFindFormat

 Finds a raster format from analyzing the images pixels

 On entry   : Image - The image to analyze.
            : format - The variable to hold the returned format.
 On exit    :
 */
static RwUInt32
rwD3D8ImageFindFormat(RwImage *image)
{
    RwInt32     depth;
    RwUInt32    format;

    RWFUNCTION(RWSTRING("rwD3D8ImageFindFormat"));

    depth = RwImageGetDepth(image);

    if ((4 == depth) || (8 == depth))
    {
        const RwInt32   width = RwImageGetWidth(image);
        const RwInt32   height = RwImageGetHeight(image);
        const RwUInt8   *cpIn = image->cpPixels;
        const RwRGBA    *rpPal = image->palette;
        RwInt32         y;
        RwBool          paletteHasAlpha;

        /*
         * 4 & 8bit palettized images always have 32bit palettes, X888 or 8888
         */

        /* First: check palette for transparent colors */
        paletteHasAlpha = FALSE;
        if (4 == depth)
        {
            for (y = 0; y < 16; y++)
            {
                if (0xFF != rpPal[y].alpha)
                {
                    paletteHasAlpha = TRUE;
                    break;
                }
            }
        }
        else
        {
            for (y = 0; y < 256; y++)
            {
                if (0xFF != rpPal[y].alpha)
                {
                    paletteHasAlpha = TRUE;
                    break;
                }
            }
        }

        if (paletteHasAlpha)
        {
            for (y = 0; y < height; y++)
            {
                const RwUInt8   *cpInCur = cpIn;
                RwInt32         x;

                for (x = 0; x < width; x++)
                {
                    /* Is there any alpha */
                    if (0xFF != rpPal[*cpInCur].alpha)
                    {
                        if (4 == depth)
                        {
    #ifndef NOPAL4TEXTURES
                            format = rwRASTERFORMAT8888 | rwRASTERFORMATPAL4;
    #else /* NOPAL4TEXTURES*/
                            format = rwRASTERFORMAT8888;
    #endif /* NOPAL4TEXTURES */
                        }
                        else
                        {
    #ifndef NOPAL8TEXTURES
                            format = rwRASTERFORMAT8888 | rwRASTERFORMATPAL8;
    #else /* NOPAL8TEXTURES*/
                            format = rwRASTERFORMAT8888;
    #endif /* NOPAL8TEXTURES */
                        }

                        RWRETURN(format);
                    }

                    /* Next pixel */
                    cpInCur++;
                }

                cpIn += RwImageGetStride(image);
            }
        }

        if (4 == depth)
        {
#ifndef NOPAL4TEXTURES
            format = rwRASTERFORMAT888 | rwRASTERFORMATPAL4;
#else /* NOPAL4TEXTURES*/
            format = rwRASTERFORMAT888;
#endif /* NOPAL4TEXTURES */
        }
        else
        {
#ifndef NOPAL8TEXTURES
            format = rwRASTERFORMAT888 | rwRASTERFORMATPAL8;
#else /* NOPAL8TEXTURES*/
            format = rwRASTERFORMAT888;
#endif /* NOPAL8TEXTURES */
        }
    }
    else
    {
        const RwInt32   width = RwImageGetWidth(image);
        const RwInt32   height = RwImageGetHeight(image);
        const RwUInt8   *cpIn = image->cpPixels;
        RwInt32         y;
        RwUInt32        alphaBits = 0;

        for (y = 0; y < height; y++)
        {
            const RwRGBA    *rpInCur = (const RwRGBA *)cpIn;
            RwInt32         x;

            for (x = 0; x < width; x++)
            {
                if (rpInCur->alpha < 0xff)
                {
                    /* lower 4 bits of the alpha channel are discarded in 4444 */
                    if (rpInCur->alpha > 0xf)
                    {
                        alphaBits = 8;
                        break;
                    }
                    else
                    {
                        alphaBits = 1;
                    }
                }

                /* Next pixel */
                rpInCur++;
            }

            if (alphaBits >= 8)
            {
                break;
            }

            cpIn += RwImageGetStride(image);
        }

        if (alphaBits >= 8)
        {
            if (_RwD3D8AdapterInformation.displayDepth > 16)
            {
                if ( _rwD3D8CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                {
                    format = rwRASTERFORMAT8888;
                }
                else if ( _rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                {
                    format = rwRASTERFORMAT4444;
                }
                else
                {
                    format = rwRASTERFORMAT1555;
                }
            }
            else
            {
                if ( _rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                {
                    format = rwRASTERFORMAT4444;
                }
                else
                {
                    format = rwRASTERFORMAT1555;
                }
            }
        }
        else if (alphaBits)
        {
            if (_RwD3D8AdapterInformation.displayDepth > 16)
            {
                if ( _rwD3D8CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                {
                    format = rwRASTERFORMAT8888;
                }
                else
                {
                    format = rwRASTERFORMAT1555;
                }
            }
            else
            {
                format = rwRASTERFORMAT1555;
            }
        }
        else
        {
            if (_RwD3D8AdapterInformation.displayDepth > 16)
            {
                if ( _rwD3D8CheckValidTextureFormat(D3DFMT_X8R8G8B8) )
                {
                    format = rwRASTERFORMAT888;
                }
                else
                {
                    format = rwRASTERFORMAT565;
                }
            }
            else
            {
                format = rwRASTERFORMAT565;
            }
        }
    }

    RWRETURN(format);
}

/*****************************************************************************
 _rwD3D8ImageFindRasterFormat

 On entry   : Raster (OUT)
            : Image
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D8ImageFindRasterFormat(void *raster, void *image, RwInt32 flags)
{
    RwRaster    *ras = (RwRaster *)raster;
    RwImage     *im = (RwImage *)image;

    RWFUNCTION(RWSTRING("_rwD3D8ImageFindRasterFormat"));

    /* Do the default thing first */
    ras->width = im->width;
    ras->height = im->height;
    ras->depth = 0; /* Use the default depth for now... */

    /* Find the rasters format */
    switch (flags & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        {
            RwUInt32    format;

            /* Check size */
            _rwD3D8CheckRasterSize(&(ras->width), &(ras->height), flags);

            /* Find the image format */
            format = rwD3D8ImageFindFormat(im);

            /* rwRASTERTYPENORMAL - 'Linear' textures can not have mip maps */
            RWASSERT(!((rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP) & flags));

            /* Only Mipmap if actually requested */
            format |= flags & ~(rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);

            /* Check format */
            RWRETURN(_rwD3D8CheckRasterFormat(raster, format | (flags & rwRASTERTYPEMASK)));
        }

        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
        {
            RwUInt32    format;

            /* Check size */
            _rwD3D8CheckRasterSize(&(ras->width), &(ras->height), flags);

            /* Find the image format */
            format = rwD3D8ImageFindFormat(im);

            /* Only Mipmap if actually requested */
            format |= flags & (rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);

            /* Check format */
            RWRETURN(_rwD3D8CheckRasterFormat(raster, format | (flags & rwRASTERTYPEMASK)));
        }

        case rwRASTERTYPECAMERA:
        case rwRASTERTYPEZBUFFER:
        {
            /* Just take the default case */
            RWRETURN(_rwD3D8CheckRasterFormat(raster, flags));
        }
        default:
        {
            /* Don't know what one of those is... */
            RWERROR((E_RW_INVRASTERFORMAT));
        }
    }

    RWRETURN(FALSE);
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*
 *                                                                           *
 *                      - Image to raster conversion -                       *
 *                                                                           *
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*****************************************************************************
 rwD3D8SelectConvertFn

 On entry :
 On exit  :
 */
static D3D8PixConvertFn
rwD3D8SelectConvertFn(RwRasterFormat format)
{
    D3D8PixConvertFn  result = NULL;

    RWFUNCTION(RWSTRING("rwD3D8SelectConvertFn"));

    switch (format & rwRASTERFORMATPIXELFORMATMASK)
    {
        case rwRASTERFORMAT555:
            result = rwD3D8ConvR8G8B8A8ToR5G5B5;
            break;

        case rwRASTERFORMAT565:
            result = rwD3D8ConvR8G8B8A8ToR5G6B5;
            break;

        case rwRASTERFORMAT1555:
            result = rwD3D8ConvR8G8B8A8ToA1R5G5B5;
            break;

        case rwRASTERFORMAT4444:
            result = rwD3D8ConvR8G8B8A8ToA4R4G4B4;
            break;

        case rwRASTERFORMATLUM8:
            result = rwD3D8ConvR8G8B8A8ToL8;
            break;

        case rwRASTERFORMAT888:
            result = rwD3D8ConvR8G8B8A8ToX8R8G8B8;
            break;

        case rwRASTERFORMAT8888:
            result = rwD3D8ConvR8G8B8A8ToA8R8G8B8;
            break;

        default:
            break;
    }

    RWRETURN(result);
}

/*****************************************************************************
 D3D8PalettizeImage

 On entry   : srcImage - The image to palettize, must be 32Bit in depth.
            : depth - The depth of the palletized image.

 On exit    : An RwImage pointer to a new palletized image else NULL.
 */
static RwImage *
D3D8PalettizeImage(RwImage *srcImage, RwInt32 depth)
{
    RwPalQuant  palQuant;
    RwImage     *palImage;

    RWFUNCTION(RWSTRING("D3D8PalettizeImage"));
    RWASSERT(32 == RwImageGetDepth(srcImage));
    RWASSERT((4 == depth) | (8 == depth));

    /* Create the new image */
    palImage = RwImageCreate(RwImageGetWidth(srcImage),
                             RwImageGetHeight(srcImage),
                             depth);
    if (!palImage)
    {
        RWRETURN(NULL);
    }

    /* Allocate the pixels & palette */
    RwImageAllocatePixels(palImage);

    if (!RwPalQuantInit(&palQuant))
    {
        RWRETURN(NULL);
    }

    /* add all pixels from the image */
    RwPalQuantAddImage(&palQuant, srcImage, 1.0f);
    RwPalQuantResolvePalette(RwImageGetPalette(palImage),
        (1UL << depth), &palQuant);

    /* match images */
    RwPalQuantMatchImage(RwImageGetPixels(palImage),
                         RwImageGetStride(palImage),
                         RwImageGetDepth(palImage),
                         FALSE,
                         &palQuant,
                         srcImage);

    RwPalQuantTerm(&palQuant);

    RWRETURN(palImage);
}

/*****************************************************************************
 rwD3D8RasterPalletizedSetFromImage

 On entry   : Raster
            : Image

            NOTE - CURRENTLY WE DON'T SUPPORT SETTING OF A PARTIAL RASTER
            HERE - IE, IF THE RASTER TO BE SET IS A SUB-RASTER

 On exit    :
 */
static void
rwD3D8RasterPalletizedSetFromImage(RwRaster *raster, RwImage *image)
{
    RWFUNCTION(RWSTRING("rwD3D8RasterPalletizedSetFromImage"));
    RWASSERT(raster);
    RWASSERT(image);

    if ( (RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK) != rwRASTERFORMATLUM8 )
    {
        if ((4 == image->depth) || (8 == image->depth))
        {
            RwInt32             y;
            _rwD3D8RasterExt    *rasExt;

            /* Get the raster plugin data */
            rasExt = RASTEREXTFROMRASTER(raster);

            /* 8 bit image */
            if (image->stride == raster->stride && raster->width == raster->stride)
            {
                const RwUInt8   *srcPixel;
                RwUInt8         *dstPixel;

                srcPixel = image->cpPixels;
                dstPixel = raster->cpPixels;

    #if defined(MIPCOLPAL)
                memset(dstPixel, (rasExt->lockedMipLevel % NUMMIPCOLS), raster->height * raster->width);
    #else /* defined(MIPCOLPAL) */
                memcpy(dstPixel, srcPixel, raster->height * raster->width);
    #endif /* defined(MIPCOLPAL) */
            }
            else
            {
                for (y = 0; y < raster->height; y++)
                {
                    RwUInt8 *srcPixel;
                    RwUInt8 *dstPixel;

                    srcPixel = image->cpPixels + (image->stride * y);
                    dstPixel = raster->cpPixels + (raster->stride * y);

        #if defined(MIPCOLPAL)
                    memset(dstPixel, (rasExt->lockedMipLevel % NUMMIPCOLS), raster->width);
        #else /* defined(MIPCOLPAL) */
                    memcpy(dstPixel, srcPixel, raster->width);
        #endif /* defined(MIPCOLPAL) */
                }
            }

            /* lockedMipLevel - set in _rwD3D8RasterLock */
            if (rasExt->lockedMipLevel == 0 &&
                raster->palette)
            {
    #if defined(MIPCOLPAL)
                RwInt32         x;
                PALETTEENTRY    *palette;

                /* Get the palette */
                palette = (PALETTEENTRY *)raster->palette;

                /* Set the palette */
                for (x = 0; x < (1 << image->depth); x++, palette++)
                {
                    palette->peRed = (MipMapCol[x % NUMMIPCOLS].red);
                    palette->peGreen = (MipMapCol[x % NUMMIPCOLS].green);
                    palette->peBlue = (MipMapCol[x % NUMMIPCOLS].blue);
                    palette->peFlags = (MipMapCol[x % NUMMIPCOLS].alpha);
                }
    #else /* defined(MIPCOLPAL) */
                /* Set the palette */
                memcpy(raster->palette, image->palette, sizeof(RwRGBA) * (1 << image->depth));
    #endif /* defined(MIPCOLPAL) */
            }
        }
        else
        {
            RwImage             *palImage;
            _rwD3D8RasterExt    *rasExt;       

            palImage = D3D8PalettizeImage(image, RwRasterGetDepth(raster));
            RWASSERT(palImage);

            if (!palImage)
            {
                RWRETURNVOID();
            }

            rasExt = RASTEREXTFROMRASTER(raster);

            if (rasExt->alpha)
            {
                RwInt32 rasFormat;

                rasFormat = rwD3D8ImageFindFormat(palImage);

                if ( (rasFormat & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888 )
                {
                    raster->cFormat &= (~rwRASTERFORMAT8888)>>8;

                    raster->cFormat |= (rwRASTERFORMAT888 >> 8);

                    rasExt->alpha = 0;
                }
            }

            rwD3D8RasterPalletizedSetFromImage(raster, palImage);

            RwImageDestroy(palImage);
        }
    }
    else
    {
        RwInt32             y, x;
        _rwD3D8RasterExt    *rasExt;

        /* Get the raster plugin data */
        rasExt = RASTEREXTFROMRASTER(raster);

        if ((4 == image->depth) || (8 == image->depth))
        {
            RwUInt8 greyscale[256];

            x = (1 << image->depth);
            for (y = 0; y < x; y++)
            {
                const RwRGBA    *pixIn = &(image->palette[y]);
                const RwUInt32  pixOut = ((pixIn->red * 30 +
                                       pixIn->green * 59 +
                                       pixIn->blue * 11) / 100);
            
                greyscale[y] = (RwUInt8)((pixOut * pixIn->alpha) / 0xff);
            }

            for (y = 0; y < raster->height; y++)
            {
                const RwUInt8   *srcPixel;
                RwUInt8         *dstPixel;

                srcPixel = (const RwUInt8 *)(image->cpPixels + (image->stride * y));
                dstPixel = raster->cpPixels + (raster->stride * y);

                for (x = 0; x < raster->width; x++)
                {               
                    *dstPixel = greyscale[*srcPixel];

                    srcPixel++;
                    dstPixel++;
                }
            }
        }
        else
        {
            for (y = 0; y < raster->height; y++)
            {
                const RwRGBA    *srcPixel;
                RwUInt8         *dstPixel;

                srcPixel = (const RwRGBA *)(image->cpPixels + (image->stride * y));
                dstPixel = raster->cpPixels + (raster->stride * y);

                for (x = 0; x < raster->width; x++)
                {
                    const RwUInt32 pixOut = ((srcPixel->red * 30 +
                                           srcPixel->green * 59 +
                                           srcPixel->blue * 11) / 100);
                
                    *dstPixel = (RwUInt8)((pixOut * srcPixel->alpha) / 0xff);

                    srcPixel++;
                    dstPixel++;
                }
            }
        }
    }

    RWRETURNVOID();
}

/*****************************************************************************
 rwD3D8Raster16LinearSetFromImage

 On entry   : Raster
            : Image

            NOTE - CURRENTLY WE DON'T SUPPORT SETTING OF A PARTIAL RASTER
            HERE - IE, IF THE RASTER TO BE SET IS A SUB-RASTER

 On exit    : TRUE on success
 */
static void
rwD3D8Raster16LinearSetFromImage(RwRaster *ras, RwImage *image)
{
    _rwD3D8RasterExt    *rasExt;
    RwInt32             x, y;

    RWFUNCTION(RWSTRING("rwD3D8Raster16LinearSetFromImage"));
    RWASSERT(ras);
    RWASSERT(image);

    rasExt = RASTEREXTFROMRASTER(ras);

    if ((image->depth == 4) || (image->depth == 8))
    {
        RwUInt16 pal16[256];
        D3D8PixConvertFn  convFn;
        const RwRasterFormat format = (RwRasterFormat)
            RwRasterGetFormat(ras);

        /* Choose a pixel convertor function */
        convFn = rwD3D8SelectConvertFn(format);
        RWASSERT(NULL != convFn);

        if (image->depth == 4)
        {
            for (x = 0; x < 16; x++)
            {
    #if defined(MIPCOL)
                pal16[x] = (RwUInt16)convFn(&MipMapCol[(rasExt->lockedMipLevel) % NUMMIPCOLS]);
    #else /* defined(MIPCOL) */
                pal16[x] = (RwUInt16)convFn(&image->palette[x]);
    #endif /* defined(MIPCOL) */
            }
        }
        else
        {
            for (x = 0; x < 256; x++)
            {
    #if defined(MIPCOL)
                pal16[x] = (RwUInt16)convFn(&MipMapCol[(rasExt->lockedMipLevel) % NUMMIPCOLS]);
    #else /* defined(MIPCOL) */
                pal16[x] = (RwUInt16)convFn(&image->palette[x]);
    #endif /* defined(MIPCOL) */
            }
        }

        for (y = 0; y < ras->height; y++)
        {
            const RwUInt8   *srcPixel;
            RwUInt16        *dstPixel;

            srcPixel = image->cpPixels + (y * image->stride);
            dstPixel = ((RwUInt16 *)(((RwUInt8 *)(ras->cpPixels)) +
                                    (ras->stride * y)));

            for (x = 0; x < ras->width; x++)
            {
                *dstPixel = pal16[*srcPixel];

                srcPixel++;
                dstPixel++;
            }
        }
    }
    else if (image->depth == 32)
    {
        const RwRGBA    *srcPixel;
        RwUInt16        *dstPixel;

        /* 32 bit image */
        switch (RwRasterGetFormat(ras) & rwRASTERFORMATPIXELFORMATMASK)
        {
            case rwRASTERFORMAT555:
                for (y = 0; y < ras->height; y++)
                {
                    srcPixel = (const RwRGBA *)(image->cpPixels + y * image->stride);
                    dstPixel = ((RwUInt16 *)(((RwUInt8 *)(ras->cpPixels)) +
                                        (ras->stride * y)));

                    for (x = 0; x < ras->width; x++)
                    {
                        *dstPixel = (RwUInt16)
                                    ((((RwUInt32)srcPixel->red << 7) & 0x7C00) |
                                     (((RwUInt32)srcPixel->green << 2) & 0x03E0) |
                                     ((RwUInt32)srcPixel->blue >> 3));
                        srcPixel++;
                        dstPixel++;
                    }
                }
                break;

            case rwRASTERFORMAT565:
                for (y = 0; y < ras->height; y++)
                {
                    srcPixel = (const RwRGBA *)(image->cpPixels + y * image->stride);
                    dstPixel = ((RwUInt16 *)(((RwUInt8 *)(ras->cpPixels)) +
                                        (ras->stride * y)));

                    for (x = 0; x < ras->width; x++)
                    {
                        *dstPixel = (RwUInt16)
                                    ((((RwUInt32)srcPixel->red << 8) & 0xF800) |
                                     (((RwUInt32)srcPixel->green << 3) & 0x07E0) |
                                     ((RwUInt32)srcPixel->blue >> 3));
                        srcPixel++;
                        dstPixel++;
                    }
                }
                break;

            case rwRASTERFORMAT1555:
                for (y = 0; y < ras->height; y++)
                {
                    srcPixel = (const RwRGBA *)(image->cpPixels + y * image->stride);
                    dstPixel = ((RwUInt16 *)(((RwUInt8 *)(ras->cpPixels)) +
                                        (ras->stride * y)));

                    for (x = 0; x < ras->width; x++)
                    {
                        *dstPixel = (RwUInt16)
                                    ((((RwUInt32)srcPixel->alpha << 8) & 0x8000) |
                                     (((RwUInt32)srcPixel->red << 7) & 0x7C00) |
                                     (((RwUInt32)srcPixel->green << 2) & 0x03E0) |
                                     ((RwUInt32)srcPixel->blue >> 3));
                        srcPixel++;
                        dstPixel++;
                    }
                }
                break;

            case rwRASTERFORMAT4444:
                for (y = 0; y < ras->height; y++)
                {
                    srcPixel = (const RwRGBA *)(image->cpPixels + y * image->stride);
                    dstPixel = ((RwUInt16 *)(((RwUInt8 *)(ras->cpPixels)) +
                                        (ras->stride * y)));

                    for (x = 0; x < ras->width; x++)
                    {
                        *dstPixel = (RwUInt16)
                                    ((((RwUInt32)srcPixel->alpha << 8) & 0xF000) |
                                     (((RwUInt32)srcPixel->red << 4) & 0x0F00) |
                                     (((RwUInt32)srcPixel->green) & 0x00F0) |
                                     ((RwUInt32)srcPixel->blue >> 4));
                        srcPixel++;
                        dstPixel++;
                    }
                }
                break;
        }
    }

    RWRETURNVOID();
}

/*****************************************************************************
 rwD3D8Raster24_32LinearSetFromImage

 On entry   : Raster
            : Image

            NOTE - CURRENTLY WE DON'T SUPPORT SETTING OF A PARTIAL RASTER
            HERE - IE, IF THE RASTER TO BE SET IS A SUB-RASTER

 On exit    :
 */
static void
rwD3D8Raster24_32LinearSetFromImage(RwRaster *ras, RwImage *image)
{
    RwInt32             x, y;
    _rwD3D8RasterExt    *rasExt;
    RwUInt8             alpha = 0;

    RWFUNCTION(RWSTRING("rwD3D8Raster24_32LinearSetFromImage"));
    RWASSERT(ras);
    RWASSERT(image);

    if ((4 == image->depth) || (8 == image->depth))
    {
        RwUInt32 pal32[256];
      
        if ((RwRasterGetFormat(ras) & rwRASTERFORMATPIXELFORMATMASK)==rwRASTERFORMAT888)
        {
            alpha = 0xff;

            for (x = 0; x < (1 << image->depth); x++)
            {
                const RwRGBA *pixIn=&(image->palette[x]);

                pal32[x] = ((RwUInt32)0xFF000000) |
                             ((RwUInt32)pixIn->red << 16) |
                             ((RwUInt32)pixIn->green << 8) |
                             ((RwUInt32)pixIn->blue);
            }

        }
        else if ((RwRasterGetFormat(ras) & rwRASTERFORMATPIXELFORMATMASK)==rwRASTERFORMAT8888)
        {
            alpha = 0;

            for (x = 0; x < (1 << image->depth); x++)
            {
                const RwRGBA *pixIn=&(image->palette[x]);

                pal32[x] = ((RwUInt32)pixIn->alpha << 24) |
                             ((RwUInt32)pixIn->red << 16) |
                             ((RwUInt32)pixIn->green << 8) |
                             ((RwUInt32)pixIn->blue);
            }
        }

        for (y = 0; y < ras->height; y++)
        {
            RwUInt8     *srcPixel;
            RwUInt32    *dstPixel;

            srcPixel = image->cpPixels + (image->stride * y);
            dstPixel = (RwUInt32 *)(ras->cpPixels + (ras->stride * y));

            for (x = 0; x < ras->width; x++)
            {
                *dstPixel = pal32[*srcPixel];

                srcPixel++;
                dstPixel++;
            }
        }
    }
    else if (32 == image->depth)
    {
        /* 32 bit image */
        if ((RwRasterGetFormat(ras) & rwRASTERFORMATPIXELFORMATMASK)==rwRASTERFORMAT888)
        {
            alpha = 0xff;

            for (y = 0; y < ras->height; y++)
            {
                RwRGBA      *srcPixel;
                RwUInt32    *dstPixel;

                srcPixel = (RwRGBA *)(image->cpPixels + (image->stride * y));
                dstPixel = (RwUInt32 *)(ras->cpPixels + (ras->stride * y));

                for (x = 0; x < ras->width; x++)
                {
                    *dstPixel = ((RwUInt32)0xFF000000) |
                                 ((RwUInt32)srcPixel->red << 16) |
                                 ((RwUInt32)srcPixel->green << 8) |
                                 ((RwUInt32)srcPixel->blue);

                    srcPixel++;
                    dstPixel++;
                }
            }
        }
        else if ((RwRasterGetFormat(ras) & rwRASTERFORMATPIXELFORMATMASK)==rwRASTERFORMAT8888)
        {
            alpha = 0;

            for (y = 0; y < ras->height; y++)
            {
                RwRGBA      *srcPixel;
                RwUInt32    *dstPixel;

                srcPixel = (RwRGBA *)(image->cpPixels + (image->stride * y));
                dstPixel = (RwUInt32 *)(ras->cpPixels + (ras->stride * y));

                for (x = 0; x < ras->width; x++)
                {                   
                    *dstPixel = ((RwUInt32)srcPixel->alpha << 24) |
                                 ((RwUInt32)srcPixel->red << 16) |
                                 ((RwUInt32)srcPixel->green << 8) |
                                 ((RwUInt32)srcPixel->blue);

                    srcPixel++;
                    dstPixel++;
                }
            }
        }
    }

    rasExt = RASTEREXTFROMRASTER(ras);

    if (alpha<0xff)
    {
        rasExt->alpha = 1;

        /* Only changed the parent if we have an alpha channel */
        if (ras != ras->parent)
        {
            _rwD3D8RasterExt    *rasExtParent;

            rasExtParent = RASTEREXTFROMRASTER(ras->parent);
        
            rasExtParent->alpha = 1;
        }
    }
    else
    {
        rasExt->alpha = 0;
    }

    RWRETURNVOID();
}

/*****************************************************************************
 _rwD3D8RasterSetFromImage

 The raster is only set if it has a valid pixel pointer

 On entry   : Raster (MODIFY)
            : Image
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterSetFromImage(void *rasterIn,
                        void *image,
                        RwInt32 unused3 __RWUNUSED__)
{
    RwImage     *img = (RwImage *)image;
    RwRaster    *raster = (RwRaster *)rasterIn;
    _rwD3D8RasterExt    *rasExt;
    RwInt32             rasFormat;
    RwInt32             format;
    RwBool              rasterLocked = FALSE;
    RwBool              paletteLocked = FALSE;
    RwImage             *imgSrc;

    RWFUNCTION(RWSTRING("_rwD3D8RasterSetFromImage"));

    rasExt = RASTEREXTFROMRASTER(raster);

    /* Check if raster is compressed and fail */
    if (rasExt->dxtFormat)
    {
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("Conversion to compressed rasters is not supported.")));

        RWRETURN(FALSE);
    }

    /* Normal type rasters are created with alpha by default */
    if(raster->cType == rwRASTERTYPENORMAL)
    {
        rasFormat = rwD3D8ImageFindFormat(img);
    }
    else
    {
        rasFormat = RwRasterGetFormat(raster);
    }

    /* Check the alpha flag */
    if ( _rwRasterConvTable[FMT2TBL(rasFormat)].alpha )
    {
        rasExt->alpha = 1;

        if (raster != raster->parent)
        {
            _rwD3D8RasterExt *rasExtParent;

            rasExtParent = RASTEREXTFROMRASTER(raster->parent);

            rasExtParent->alpha = 1;
        }
    }
    else
    {
        rasExt->alpha = 0;
    }

    /* If the size is different, we need to rescale the image */
    if( raster->width != img->width || raster->height != img->height )
    {
        imgSrc = RwImageCreateResample(img, raster->width, raster->height);
    }
    else
    {
        imgSrc = img;
    }

    /* What kind of raster do we have */
    format = RwRasterGetFormat(raster);
    RWASSERT(rwRASTERFORMAT16 != format);
    RWASSERT(rwRASTERFORMAT24 != format);
    RWASSERT(rwRASTERFORMAT32 != format);

    /* Check for an already existing lock */
    if (raster->privateFlags & rwRASTERPIXELLOCKEDWRITE)
    {
        rasterLocked = TRUE;
    }

    /* Lock for write */
    if (!rasterLocked)
    {
        if (!RwRasterLock(raster, 0, rwRASTERLOCKWRITE | rwRASTERLOCKNOFETCH))
        {
            RWRETURN(FALSE);
        }
    }

    if (format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        if (raster->privateFlags & rwRASTERPALETTELOCKEDWRITE)
        {
            paletteLocked = TRUE;
        }

        if (!paletteLocked)
        {
            if (!RwRasterLockPalette(raster, rwRASTERLOCKWRITE | rwRASTERLOCKNOFETCH))
            {
                RWRETURN(FALSE);
            }
        }
    }

    switch (RwRasterGetDepth(raster))
    {
    case 4:
    case 8:
        /* Palletized normal rasters can not be created so no need to check */
        rwD3D8RasterPalletizedSetFromImage(raster, imgSrc);
        break;

    case 16:
        rwD3D8Raster16LinearSetFromImage(raster, imgSrc);
        break;

    case 24:
    case 32:
        rwD3D8Raster24_32LinearSetFromImage(raster, imgSrc);
        break;

    default:
        RWERROR((E_RW_INVRASTERDEPTH));
        break;
    }

    if (format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        if (!paletteLocked)
        {
            RwRasterUnlockPalette(raster);
        }
    }

    if (!rasterLocked)
    {
        RwRasterUnlock(raster);
    }

    if (imgSrc != img)
    {
        RwImageDestroy(imgSrc);
    }

    RWRETURN(TRUE);
}
