/*****************************************************************************
 *                                                                           *
 * Module  : D3D9convrt.c                                                      *
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

#include "d3d9device.h"
#include "d3d9raster.h"
#include "d3d9convrt.h"



/*****************************************************************************
 Global prototypes
 */

/*****************************************************************************
 Local Types
 */

typedef RwUInt32 (*D3D9PixConvertFn)(const RwRGBA *colIn);
typedef void (*D3D9PixUnconvertFn)(RwRGBA *pixOut, RwUInt32 pixIn);

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
 _rwD3D9FindMSB

 On entry   : LONG
 On exit    : Bit number of MSB (-1 if 0)
 */
RwInt32
_rwD3D9FindMSB(RwInt32 num)
{
    RwInt32 pos = -1;

    RWFUNCTION(RWSTRING("_rwD3D9FindMSB"));

    while (num)
    {
        pos++;
        num >>= 1;
    }

    RWRETURN(pos);
}

/****************************************************************************
 _rwD3D9CheckRasterSize

 On entry   : width and height to check
  */
void
_rwD3D9CheckRasterSize(RwInt32 *width, RwInt32 *height, RwUInt32 rasterFlags)
{
    RWFUNCTION(RWSTRING("_rwD3D9CheckRasterSize"));

    /* Clamp the raster to the max texture size */
    if (*width > (RwInt32)_RwD3D9DeviceCaps.MaxTextureWidth)
    {
        *width = _RwD3D9DeviceCaps.MaxTextureWidth;
    }

    if (*height > (RwInt32)_RwD3D9DeviceCaps.MaxTextureHeight)
    {
        *height = _RwD3D9DeviceCaps.MaxTextureHeight;
    }

    if ((rasterFlags & rwRASTERTYPEMASK) == rwRASTERTYPENORMAL ||
        (rasterFlags & rwRASTERFORMATMIPMAP) == 0)
    {
        if ( (_RwD3D9DeviceCaps.TextureCaps & D3DPTEXTURECAPS_POW2) != 0 &&
             (_RwD3D9DeviceCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) == 0)
        {
            /* Make it a power of two (round down) */
            *width = 1 << _rwD3D9FindMSB(*width);
            *height = 1 << _rwD3D9FindMSB(*height);
        }
    }
    else
    {
        if (_RwD3D9DeviceCaps.TextureCaps & D3DPTEXTURECAPS_POW2)
        {
            /* Make it a power of two (round down) */
            *width = 1 << _rwD3D9FindMSB(*width);
            *height = 1 << _rwD3D9FindMSB(*height);
        }
    }

    if (_RwD3D9DeviceCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY)
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
 D3D9ConvR8G8B8A8ToR5G5B5

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToR5G5B5(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToR5G5B5"));

    pixOut = (0x00000000) | /* Err slam a 0 in here, it ain't used anyway */
             (((RwUInt32)pixIn->red << 7) & 0x7C00) |
             (((RwUInt32)pixIn->green << 2) & 0x03E0) |
             (((RwUInt32)pixIn->blue >> 3) & 0x001F);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToR5G6B5

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToR5G6B5(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToR5G6B5"));

    pixOut = (((RwUInt32)pixIn->red << 8) & 0xF800) |
             (((RwUInt32)pixIn->green << 3) & 0x07E0) |
             ((RwUInt32)pixIn->blue >> 3);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToA1R5G5B5

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToA1R5G5B5(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToA1R5G5B5"));

    pixOut = (((RwUInt32)pixIn->alpha << 8) & 0x8000) |
             (((RwUInt32)pixIn->red << 7) & 0x7C00) |
             (((RwUInt32)pixIn->green << 2) & 0x03E0) |
             ((RwUInt32)pixIn->blue >> 3);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToA4R4G4B4

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToA4R4G4B4(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToA4R4G4B4"));

    pixOut = (((RwUInt32)pixIn->alpha << 8) & 0xF000) |
             (((RwUInt32)pixIn->red << 4) & 0x0F00) |
             (((RwUInt32)pixIn->green) & 0x00F0) |
             ((RwUInt32)pixIn->blue >> 4);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToA8R3G3B2

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToA8R3G3B2(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToA8R3G3B2"));

    pixOut = ((RwUInt32)pixIn->alpha << 8) |
             (((RwUInt32)pixIn->red) & 0xE0) |
             (((RwUInt32)pixIn->green >> 3) & 0x1C) |
             ((RwUInt32)pixIn->blue >> 6);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToR4G4B4

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToR4G4B4(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToR4G4B4"));

    pixOut = 0xF000 |
             (((RwUInt32)pixIn->red << 4) & 0x0F00) |
             (((RwUInt32)pixIn->green) & 0x00F0) |
             ((RwUInt32)pixIn->blue >> 4);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToL6V5U5

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToL6V5U5(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToL6V5U5"));

    pixOut = (((RwUInt32)pixIn->blue << 8) & 0xFC00) |
             (((RwUInt32)pixIn->green << 2) & 0x03E0) |
             ((RwUInt32)pixIn->red >> 3);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToX8R8G8B8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToX8R8G8B8(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToX8R8G8B8"));

    pixOut = ((RwUInt32)0xFF << 24) |
             ((RwUInt32)pixIn->red << 16) |
             ((RwUInt32)pixIn->green << 8) |
             ((RwUInt32)pixIn->blue);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToX8B8G8R8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToX8B8G8R8(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToX8B8G8R8"));

    pixOut = ((RwUInt32)0xFF << 24) |
             ((RwUInt32)pixIn->blue << 16) |
             ((RwUInt32)pixIn->green << 8) |
             ((RwUInt32)pixIn->red);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToA8R8G8B8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToA8R8G8B8(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToA8R8G8B8"));

    pixOut = ((RwUInt32)pixIn->alpha << 24) |
             ((RwUInt32)pixIn->red << 16) |
             ((RwUInt32)pixIn->green << 8) |
             ((RwUInt32)pixIn->blue);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToA8B8G8R8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToA8B8G8R8(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToA8B8G8R8"));

    pixOut = ((RwUInt32)pixIn->alpha << 24) |
             ((RwUInt32)pixIn->blue << 16) |
             ((RwUInt32)pixIn->green << 8) |
             ((RwUInt32)pixIn->red);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToV8U8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToV8U8(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToV8U8"));

    pixOut = ((RwUInt32)pixIn->red) | ((RwUInt32)pixIn->green << 8);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToL8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToL8(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToL8"));

    pixOut  =   ((pixIn->red * 30 +
                  pixIn->green * 59 +
                  pixIn->blue * 11) / 100);

    pixOut  =  ((pixOut * (pixIn->alpha)) / 0xff);

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToA8L8

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToA8L8(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToA8L8"));

    pixOut  =   ((pixIn->red * 30 +
                  pixIn->green * 59 +
                  pixIn->blue * 11) / 100);

    pixOut  =  (pixOut | (pixIn->alpha << 8));

    RWRETURN(pixOut);
}

/*****************************************************************************
 D3D9ConvR8G8B8A8ToL16

 On entry : A pointer to a RwRGBA to pack.
 On exit  : A RwUInt32 representing the packed RwRGBA.
 */
static RwUInt32
D3D9ConvR8G8B8A8ToL16(const RwRGBA *pixIn)
{
    RwUInt32    pixOut;

    RWFUNCTION(RWSTRING("D3D9ConvR8G8B8A8ToL16"));

    pixOut  =   ( ((pixIn->red * 30 +
                    pixIn->green * 59 +
                    pixIn->blue * 11) * (pixIn->alpha)) / (100 * 0xff));

    pixOut  =   (pixOut * 0xff);

    RWRETURN(pixOut);
}

/*****************************************************************************
 _rwD3D9RGBToPixel

 Convert RwRGBA to device specific format

 On entry   : pixel - device specific colour out
            : col - colour to convert
            : format - pixel format
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RGBToPixel(void *pixelOut, void *colIn, RwInt32 format)
{
    RwRGBA  *rgba = (RwRGBA *)colIn;
    RwInt32 pixVal;

    RWFUNCTION(RWSTRING("_rwD3D9RGBToPixel"));

    switch (format & rwRASTERFORMATPIXELFORMATMASK)
    {
        case rwRASTERFORMATDEFAULT:
        {
            /* Default is 8888 */
            if (!_rwD3D9RGBToPixel(&pixVal, colIn, rwRASTERFORMAT8888))
            {
                RWRETURN(FALSE);
            }
            break;
        }

        case rwRASTERFORMAT555:
            pixVal = D3D9ConvR8G8B8A8ToR5G5B5(rgba);
            break;

        case rwRASTERFORMAT1555:
            pixVal = D3D9ConvR8G8B8A8ToA1R5G5B5(rgba);
            break;

        case rwRASTERFORMAT565:
            pixVal = D3D9ConvR8G8B8A8ToR5G6B5(rgba);
            break;

        case rwRASTERFORMAT4444:
            pixVal = D3D9ConvR8G8B8A8ToA4R4G4B4(rgba);
            break;

        case rwRASTERFORMATLUM8:
            pixVal = D3D9ConvR8G8B8A8ToL8(rgba);
            break;

        case rwRASTERFORMAT8888:
            pixVal = D3D9ConvR8G8B8A8ToA8R8G8B8(rgba);
            break;

        case rwRASTERFORMAT888:
            pixVal = D3D9ConvR8G8B8A8ToX8R8G8B8(rgba);
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
 D3D9ConvR5G5B5ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvR5G5B5ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvR5G5B5ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn >> 7) & 0xF8);
    pixOut->green = (RwUInt8)((pixIn >> 2) & 0xF8);
    pixOut->blue  = (RwUInt8)((pixIn << 3) & 0xF8);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvR5G6B5ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvR5G6B5ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvR5G6B5ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn >> 8) & 0xF8);
    pixOut->green = (RwUInt8)((pixIn >> 3) & 0xFC);
    pixOut->blue  = (RwUInt8)((pixIn << 3) & 0xF8);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvL6V5U5ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvL6V5U5ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvL6V5U5ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn << 3) & 0xF8);
    pixOut->green = (RwUInt8)((pixIn >> 2) & 0xF8);
    pixOut->blue  = (RwUInt8)((pixIn >> 8) & 0xFC);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvA1R5G5B5ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvA1R5G5B5ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvA1R5G5B5ToR8G8B8A8"));

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
 D3D9ConvA4R4G4B4ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvA4R4G4B4ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvA4R4G4B4ToR8G8B8A8"));

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
 D3D9ConvA8R3G3B2ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvA8R3G3B2ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvA8R3G3B2ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn >> 0) & 0xE0);
    pixOut->green = (RwUInt8)((pixIn << 3) & 0xE0);
    pixOut->blue  = (RwUInt8)((pixIn << 6) & 0xC0);
    pixOut->alpha = (RwUInt8)(pixIn >> 8);

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvR3G3B2ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvR3G3B2ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvR3G3B2ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn >> 0) & 0xE0);
    pixOut->green = (RwUInt8)((pixIn << 3) & 0xE0);
    pixOut->blue  = (RwUInt8)((pixIn << 6) & 0xC0);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvX4R4G4B4ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvX4R4G4B4ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvX4R4G4B4ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn >> 4) & 0xF0);
    pixOut->green = (RwUInt8)((pixIn >> 0) & 0xF0);
    pixOut->blue  = (RwUInt8)((pixIn << 4) & 0xF0);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvX8R8G8B8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvX8R8G8B8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvX8R8G8B8ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
    pixOut->green = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->blue  = (RwUInt8)((pixIn) & 0xFF);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvX8B8G8R8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvX8B8G8R8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvX8B8G8R8ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn) & 0xFF);
    pixOut->green = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->blue  = (RwUInt8)((pixIn >> 16) & 0xFF);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvV8U8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvV8U8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvV8U8ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn) & 0xFF);
    pixOut->green = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->blue  = 0;
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvA8R8G8B8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvA8R8G8B8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvA8R8G8B8ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
    pixOut->green = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->blue  = (RwUInt8)((pixIn) & 0xFF);
    pixOut->alpha = (RwUInt8)((pixIn >> 24) & 0xFF);

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvA8B8G8R8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvA8B8G8R8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvA8B8G8R8ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn) & 0xFF);
    pixOut->green = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->blue  = (RwUInt8)((pixIn >> 16) & 0xFF);
    pixOut->alpha = (RwUInt8)((pixIn >> 24) & 0xFF);

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvL8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvL8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvL8ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)pixIn;
    pixOut->green = (RwUInt8)pixIn;
    pixOut->blue  = (RwUInt8)pixIn;
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvA8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvA8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvA8ToR8G8B8A8"));

    pixOut->red   = 0xFF;
    pixOut->green = 0xFF;
    pixOut->blue  = 0xFF;
    pixOut->alpha = (RwUInt8)pixIn;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvA4L4ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvA4L4ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvA4L4ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)(pixIn << 4);
    pixOut->green = (RwUInt8)(pixIn << 4);
    pixOut->blue  = (RwUInt8)(pixIn << 4);

    if (pixIn >= 0xF0)
    {
        pixOut->alpha = 0xFF;
    }
    else
    {
        pixOut->alpha = (RwUInt8)(pixIn & 0xF0);
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvA8L8ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvA8L8ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvA8L8ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)(pixIn & 0xFF);
    pixOut->green = (RwUInt8)(pixIn & 0xFF);
    pixOut->blue  = (RwUInt8)(pixIn & 0xFF);
    pixOut->alpha = (RwUInt8)((pixIn >> 8) & 0xFF);

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ConvL16ToR8G8B8A8

 On entry : A pointer to a RwRGBA that is the unpacked pixel value.
 On exit  : A RwUInt32 representing the packed pixel value.
 */
static void
D3D9ConvL16ToR8G8B8A8(RwRGBA *pixOut, RwUInt32 pixIn)
{
    RWFUNCTION(RWSTRING("D3D9ConvL16ToR8G8B8A8"));

    pixOut->red   = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->green = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->blue  = (RwUInt8)((pixIn >> 8) & 0xFF);
    pixOut->alpha = 0xFF;

    RWRETURNVOID();
}

/*****************************************************************************
 _rwD3D9PixelToRGB

 Convert device specific colour to RwRGBA

 On entry   : rgbOut - RwRGBA out
            : pixel - Pixel in
            : Pixel format
 On exit    : TRUE on success
 */
RwBool
_rwD3D9PixelToRGB(void *rgbOut, void *pixel, RwInt32 format)
{
    RwRGBA  *rgba = (RwRGBA *)rgbOut;

    RWFUNCTION(RWSTRING("_rwD3D9PixelToRGB"));

    if (format & rwRASTERFORMATPAL8)
    {
        *rgba = *((RwRGBA *)pixel);
    }
    else
    {
        switch (format & rwRASTERFORMATPIXELFORMATMASK)
        {
            /*
            case rwRASTERFORMATDEFAULT:
                D3D9ConvA8R8G8B8ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;
            */

            case rwRASTERFORMAT555:
                D3D9ConvR5G5B5ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT1555:
                D3D9ConvA1R5G5B5ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT565:
                D3D9ConvR5G6B5ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT4444:
                D3D9ConvA4R4G4B4ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMATLUM8:
                D3D9ConvL8ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT8888:
                D3D9ConvA8R8G8B8ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
                break;

            case rwRASTERFORMAT888:
                D3D9ConvX8R8G8B8ToR8G8B8A8(rgba, *(RwInt32 *)pixel);
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


/*****************************************************************************
 D3D9SelectUnconvertFn

 On entry :
 On exit  :
 */
static D3D9PixUnconvertFn
D3D9SelectUnconvertFn(D3DFORMAT d3dFormat)
{
    D3D9PixUnconvertFn    result = NULL;

    RWFUNCTION(RWSTRING("D3D9SelectUnconvertFn"));

    switch (d3dFormat)
    {
        case D3DFMT_X1R5G5B5:
            result = D3D9ConvR5G5B5ToR8G8B8A8;
            break;

        case D3DFMT_R5G6B5:
            result = D3D9ConvR5G6B5ToR8G8B8A8;
            break;

        case D3DFMT_A1R5G5B5:
            result = D3D9ConvA1R5G5B5ToR8G8B8A8;
            break;

        case D3DFMT_A4R4G4B4:
            result = D3D9ConvA4R4G4B4ToR8G8B8A8;
            break;

        case D3DFMT_L8:
            result = D3D9ConvL8ToR8G8B8A8;
            break;

        case D3DFMT_A8:
            result = D3D9ConvA8ToR8G8B8A8;
            break;

        case D3DFMT_X8R8G8B8:
            result = D3D9ConvX8R8G8B8ToR8G8B8A8;
            break;

        case D3DFMT_A8R8G8B8:
            result = D3D9ConvA8R8G8B8ToR8G8B8A8;
            break;

        case D3DFMT_A8R3G3B2:
            result = D3D9ConvA8R3G3B2ToR8G8B8A8;
            break;

        case D3DFMT_R3G3B2:
            result = D3D9ConvR3G3B2ToR8G8B8A8;
            break;

        case D3DFMT_X4R4G4B4:
            result = D3D9ConvX4R4G4B4ToR8G8B8A8;
            break;

        case D3DFMT_A4L4:
            result = D3D9ConvA4L4ToR8G8B8A8;
            break;

        case D3DFMT_A8L8:
            result = D3D9ConvA8L8ToR8G8B8A8;
            break;

        case D3DFMT_L16:
            result = D3D9ConvL16ToR8G8B8A8;
            break;

        case D3DFMT_V8U8:
        case D3DFMT_CxV8U8:
            result = D3D9ConvV8U8ToR8G8B8A8;
            break;

        case D3DFMT_L6V5U5:
            result = D3D9ConvL6V5U5ToR8G8B8A8;
            break;

        case D3DFMT_A8B8G8R8:
        case D3DFMT_Q8W8V8U8:
            result = D3D9ConvA8B8G8R8ToR8G8B8A8;
            break;

        case D3DFMT_X8B8G8R8:
        case D3DFMT_X8L8V8U8:
            result = D3D9ConvX8B8G8R8ToR8G8B8A8;
            break;

        default:
            #if defined(RWDEBUG)
            RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9SelectUnconvertFn",
                                "Pixel format not supported.");
            #endif
            break;
    }

    RWRETURN(result);
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*
 *                                                                           *
 *                      - Raster to image conversion -                       *
 *                                                                           *
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
static void
D3D9Image32GetFromD3D9Raster(RwImage *image, RwRaster *raster)
{
    RwRaster                *topRaster;
    const _rwD3D9RasterExt  *rasExt;
    D3D9PixUnconvertFn  unconvFn;
    RwInt32             j, i;
    RwRGBA              *dstPixel;

    RWFUNCTION(RWSTRING("D3D9Image32GetFromD3D9Raster"));

    topRaster = RwRasterGetParent(raster);

    /* Get the raster plugin data */
    rasExt = RASTEREXTFROMCONSTRASTER(topRaster);

    unconvFn = D3D9SelectUnconvertFn(rasExt->d3dFormat);
    RWASSERT(unconvFn != NULL);

    switch (RwRasterGetDepth(raster))
    {
        case 4:
        case 8:
            {
                const RwUInt8       *srcPixel;

                for (j = 0; j < raster->height; j++)
                {
                    srcPixel = (const RwUInt8 *)(raster->cpPixels + (raster->stride * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    i = raster->width;
                    do
                    {
                        unconvFn(dstPixel, *srcPixel);

                        srcPixel++;
                        dstPixel++;
                    }
                    while(--i);
                }
            }
            break;

        case 16:
            {
                const RwUInt16       *srcPixel;

                for (j = 0; j < raster->height; j++)
                {
                    srcPixel = (const RwUInt16 *)(raster->cpPixels + (raster->stride * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    i = raster->width;
                    do
                    {
                        unconvFn(dstPixel, *srcPixel);

                        srcPixel++;
                        dstPixel++;
                    }
                    while(--i);
                }
            }
            break;

        case 32:
            {
                const RwUInt32       *srcPixel;

                for (j = 0; j < raster->height; j++)
                {
                    srcPixel = (const RwUInt32 *)(raster->cpPixels + (raster->stride * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    i = raster->width;
                    do
                    {
                        unconvFn(dstPixel, *srcPixel);

                        srcPixel++;
                        dstPixel++;
                    }
                    while(--i);
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
 D3D9Image32GetFromRaster

 Convert raster into a 32bit image

 On entry   : image - destination image to receive the raster's image
            : raster - source raster to convert

 On exit    :
 */
static void
D3D9Image32GetFromRaster(RwImage *image, RwRaster *raster)
{
    RWFUNCTION(RWSTRING("D3D9Image32GetFromRaster"));

    /* Non standard format */
    if ((RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK) == 0)
    {
        D3D9Image32GetFromD3D9Raster(image, raster);
    }
    else
    {
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
                    RwRaster                *topRaster;
                    const _rwD3D9RasterExt  *rasExt;
                    const RwUInt32  *srcPixel;
                    RwRGBA          *dstPixel;
                    RwInt32         j, i;

                    topRaster = RwRasterGetParent(raster);

                    /* Get the raster plugin data */
                    rasExt = RASTEREXTFROMCONSTRASTER(topRaster);

                    if (rasExt->d3dFormat == D3DFMT_X8R8G8B8)
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
                    else if (rasExt->d3dFormat == D3DFMT_A8R8G8B8)
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
                    else if (rasExt->d3dFormat == D3DFMT_A2R10G10B10)
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

                                dstPixel->red   = (RwUInt8)((pixIn >> 22) & 0xFF);
                                dstPixel->green = (RwUInt8)((pixIn >> 12) & 0xFF);
                                dstPixel->blue  = (RwUInt8)((pixIn >> 2) & 0xFF);
                                dstPixel->alpha = (RwUInt8)((pixIn >> 30) * (0xff / 0x03));

                                dstPixel++;
                            }
                            while(--i);
                        }
                    }
                    else
                    {
                        D3D9Image32GetFromD3D9Raster(image, raster);
                    }
                }
                break;

            default:
                RWERROR((E_RW_INVRASTERDEPTH));
                break;
        }
    }

    RWRETURNVOID();
}

/*****************************************************************************
 D3D9Image8GetFromRaster

 Convert raster into a palettized image

 On entry   : image - destination image to receive the raster's image
            : raster - source raster to convert

 On exit    :
 */
static void
D3D9Image8GetFromRaster(RwImage *image, RwRaster *raster)
{
    RWFUNCTION(RWSTRING("D3D9Image8GetFromRaster"));

    if (RwRasterGetDepth(raster) == 8 &&
        (RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK) != 0)
    {
        RwInt32             y;

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
    else
    {
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("Conversion from non palettised rasters to 8bit images is not supported")));
    }

    RWRETURNVOID();
}

/*****************************************************************************
 _rwD3D9ImageGetFromRaster

 Convert raster to image

 On entry   : image - destination image to receive the raster's image
            : raster - source raster to convert
 On exit    : TRUE on success
 */
RwBool
_rwD3D9ImageGetFromRaster(void *imageIn,
                          void *rasterIn,
                          RwInt32 unused3 __RWUNUSED__)
{
    RwImage     *image = (RwImage *)imageIn;
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwBool      rasterLocked = FALSE;
    RwBool      paletteLocked = FALSE;
    const _rwD3D9RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D9ImageGetFromRaster"));

    /* Check if raster is compressed and fail */
    rasExt = RASTEREXTFROMCONSTRASTER(raster);
    if (rasExt->compressed)
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
        D3D9Image32GetFromRaster(image, raster);
    }
    else if (8 == RwImageGetDepth(image))
    {
        D3D9Image8GetFromRaster(image, raster);
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
 _rwD3D9ImageFindFormat

 Finds a raster format from analyzing the images pixels

 On entry   : Image - The image to analyze.
            : format - The variable to hold the returned format.
 On exit    :
 */
RwUInt32
_rwD3D9ImageFindFormat(RwImage *image)
{
    RwInt32     depth;
    RwUInt32    format;

    RWFUNCTION(RWSTRING("_rwD3D9ImageFindFormat"));

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
            if (_RwD3D9AdapterInformation.displayDepth > 16)
            {
                if ( _rwD3D9CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                {
                    format = rwRASTERFORMAT8888;
                }
                else if ( _rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
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
                if ( _rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
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
            if (_RwD3D9AdapterInformation.displayDepth > 16)
            {
                if ( _rwD3D9CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
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
            if (_RwD3D9AdapterInformation.displayDepth > 16)
            {
                if ( _rwD3D9CheckValidTextureFormat(D3DFMT_X8R8G8B8) )
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
 _rwD3D9ImageFindRasterFormat

 On entry   : Raster (OUT)
            : Image
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D9ImageFindRasterFormat(void *raster, void *image, RwInt32 flags)
{
    RwRaster    *ras = (RwRaster *)raster;
    RwImage     *im = (RwImage *)image;

    RWFUNCTION(RWSTRING("_rwD3D9ImageFindRasterFormat"));

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
            _rwD3D9CheckRasterSize(&(ras->width), &(ras->height), flags);

            /* Find the image format */
            format = _rwD3D9ImageFindFormat(im);

            /* rwRASTERTYPENORMAL - 'Linear' textures can not have mip maps */
            RWASSERT(!((rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP) & flags));

            /* Only Mipmap if actually requested */
            format |= flags & ~(rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);

            /* Check format */
            RWRETURN(_rwD3D9CheckRasterFormat(raster, format | (flags & rwRASTERTYPEMASK)));
        }

        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
        {
            RwUInt32    format;

            /* Check size */
            _rwD3D9CheckRasterSize(&(ras->width), &(ras->height), flags);

            /* Find the image format */
            format = _rwD3D9ImageFindFormat(im);

            /* Only Mipmap if actually requested */
            format |= flags & (rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);

            /* Check format */
            RWRETURN(_rwD3D9CheckRasterFormat(raster, format | (flags & rwRASTERTYPEMASK)));
        }

        case rwRASTERTYPECAMERA:
        case rwRASTERTYPEZBUFFER:
        {
            /* Just take the default case */
            RWRETURN(_rwD3D9CheckRasterFormat(raster, flags));
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
 D3D9SelectConvertFn

 On entry :
 On exit  :
 */
static D3D9PixConvertFn
D3D9SelectConvertFn(RwUInt32 d3dFormat)
{
    D3D9PixConvertFn  result = NULL;

    RWFUNCTION(RWSTRING("D3D9SelectConvertFn"));

    switch (d3dFormat)
    {
        case D3DFMT_X1R5G5B5:
            result = D3D9ConvR8G8B8A8ToR5G5B5;
            break;

        case D3DFMT_R5G6B5:
            result = D3D9ConvR8G8B8A8ToR5G6B5;
            break;

        case D3DFMT_A1R5G5B5:
            result = D3D9ConvR8G8B8A8ToA1R5G5B5;
            break;

        case D3DFMT_A4R4G4B4:
            result = D3D9ConvR8G8B8A8ToA4R4G4B4;
            break;

        case D3DFMT_A8R3G3B2:
            result = D3D9ConvR8G8B8A8ToA8R3G3B2;
            break;

        case D3DFMT_L8:
            result = D3D9ConvR8G8B8A8ToL8;
            break;

        case D3DFMT_X8R8G8B8:
            result = D3D9ConvR8G8B8A8ToX8R8G8B8;
            break;

        case D3DFMT_A8R8G8B8:
            result = D3D9ConvR8G8B8A8ToA8R8G8B8;
            break;

        case D3DFMT_X4R4G4B4:
            result = D3D9ConvR8G8B8A8ToR4G4B4;
            break;

        case D3DFMT_A8L8:
            result = D3D9ConvR8G8B8A8ToA8L8;
            break;

        case D3DFMT_L16:
            result = D3D9ConvR8G8B8A8ToL16;
            break;

        case D3DFMT_V8U8:
        case D3DFMT_CxV8U8:
            result = D3D9ConvR8G8B8A8ToV8U8;
            break;

        case D3DFMT_L6V5U5:
            result = D3D9ConvR8G8B8A8ToL6V5U5;
            break;

        case D3DFMT_A8B8G8R8:
        case D3DFMT_Q8W8V8U8:
            result = D3D9ConvR8G8B8A8ToA8B8G8R8;
            break;

        case D3DFMT_X8B8G8R8:
        case D3DFMT_X8L8V8U8:
            result = D3D9ConvR8G8B8A8ToX8B8G8R8;
            break;

        default:
            #if defined(RWDEBUG)
            RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9SelectConvertFn",
                                "Pixel format not supported.");
            #endif
            break;
    }

    RWRETURN(result);
}

/*****************************************************************************
 D3D9PalettizeImage

 On entry   : srcImage - The image to palettize, must be 32Bit in depth.
            : depth - The depth of the palletized image.

 On exit    : An RwImage pointer to a new palletized image else NULL.
 */
static RwImage *
D3D9PalettizeImage(RwImage *srcImage, RwInt32 depth)
{
    RwPalQuant  palQuant;
    RwImage     *palImage;

    RWFUNCTION(RWSTRING("D3D9PalettizeImage"));
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
 D3D9RasterPalletizedSetFromImage

 On entry   : Raster
            : Image

            NOTE - CURRENTLY WE DON'T SUPPORT SETTING OF A PARTIAL RASTER
            HERE - IE, IF THE RASTER TO BE SET IS A SUB-RASTER

 On exit    :
 */
static void
D3D9RasterPalletizedSetFromImage(RwRaster *raster, RwImage *image)
{
    _rwD3D9RasterExt    *rasExt;
    RwInt32             y, x;

    RWFUNCTION(RWSTRING("D3D9RasterPalletizedSetFromImage"));
    RWASSERT(raster);
    RWASSERT(image);

    /* Get the raster plugin data */
    rasExt = RASTEREXTFROMRASTER(raster);

    if ( (RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK) != rwRASTERFORMATLUM8 )
    {
        if ((4 == image->depth) || (8 == image->depth))
        {
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

            /* lockedMipLevel - set in _rwD3D9RasterLock */
            if (rasExt->lockedMipLevel == 0 &&
                raster->palette)
            {
    #if defined(MIPCOLPAL)
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

            palImage = D3D9PalettizeImage(image, RwRasterGetDepth(raster));
            RWASSERT(palImage);

            if (!palImage)
            {
                RWRETURNVOID();
            }

            if (rasExt->alpha)
            {
                RwInt32 rasFormat;

                rasFormat = _rwD3D9ImageFindFormat(palImage);

                if ( (rasFormat & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888 )
                {
                    raster->cFormat &= (~rwRASTERFORMAT8888)>>8;

                    raster->cFormat |= (rwRASTERFORMAT888 >> 8);

                    rasExt->alpha = 0;
                }
            }

            D3D9RasterPalletizedSetFromImage(raster, palImage);

            RwImageDestroy(palImage);
        }
    }
    else if (rasExt->d3dFormat == D3DFMT_R3G3B2)
    {
        if ((4 == image->depth) || (8 == image->depth))
        {
            RwUInt8 packed[256];

            x = (1 << image->depth);
            for (y = 0; y < x; y++)
            {
                const RwRGBA    *pixIn = &(image->palette[y]);

                const RwUInt32  pixOut = ( ((pixIn->red >> 5) << 5) |
                                           ((pixIn->green >> 5) << 2) |
                                           (pixIn->blue >> 6) );

                packed[y] = (RwUInt8)pixOut;
            }

            for (y = 0; y < raster->height; y++)
            {
                const RwUInt8   *srcPixel;
                RwUInt8         *dstPixel;

                srcPixel = (const RwUInt8 *)(image->cpPixels + (image->stride * y));
                dstPixel = raster->cpPixels + (raster->stride * y);

                for (x = 0; x < raster->width; x++)
                {
                    *dstPixel = packed[*srcPixel];

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
                    const RwUInt32  pixOut = ( ((srcPixel->red >> 5) << 5) |
                                               ((srcPixel->green >> 5) << 2) |
                                               (srcPixel->blue >> 6) );

                    *dstPixel = (RwUInt8)pixOut;

                    srcPixel++;
                    dstPixel++;
                }
            }
        }
    }
    else if (rasExt->d3dFormat == D3DFMT_A4L4)
    {
        if ((4 == image->depth) || (8 == image->depth))
        {
            RwUInt8 packedgreyscale[256];

            x = (1 << image->depth);
            for (y = 0; y < x; y++)
            {
                const RwRGBA    *pixIn = &(image->palette[y]);
                const RwUInt32  pixOut = (((pixIn->red * 30 +
                                            pixIn->green * 59 +
                                            pixIn->blue * 11) / 100) >> 4);

                packedgreyscale[y] = (RwUInt8)(pixOut | (pixIn->alpha & 0xf0));
            }

            for (y = 0; y < raster->height; y++)
            {
                const RwUInt8   *srcPixel;
                RwUInt8         *dstPixel;

                srcPixel = (const RwUInt8 *)(image->cpPixels + (image->stride * y));
                dstPixel = raster->cpPixels + (raster->stride * y);

                for (x = 0; x < raster->width; x++)
                {
                    *dstPixel = packedgreyscale[*srcPixel];

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
                    const RwUInt32  pixOut = (((srcPixel->red * 30 +
                                                srcPixel->green * 59 +
                                                srcPixel->blue * 11) / 100) >> 4);

                    *dstPixel = (RwUInt8)(pixOut | (srcPixel->alpha & 0xf0));

                    srcPixel++;
                    dstPixel++;
                }
            }
        }
    }
    else if (rasExt->d3dFormat == D3DFMT_A8)
    {
        if ((4 == image->depth) || (8 == image->depth))
        {
            RwUInt8 alpha[256];

            x = (1 << image->depth);
            for (y = 0; y < x; y++)
            {
                const RwRGBA    *pixIn = &(image->palette[y]);

                alpha[y] = pixIn->alpha;
            }

            for (y = 0; y < raster->height; y++)
            {
                const RwUInt8   *srcPixel;
                RwUInt8         *dstPixel;

                srcPixel = (const RwUInt8 *)(image->cpPixels + (image->stride * y));
                dstPixel = raster->cpPixels + (raster->stride * y);

                for (x = 0; x < raster->width; x++)
                {
                    *dstPixel = alpha[*srcPixel];

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
                    *dstPixel = srcPixel->alpha;

                    srcPixel++;
                    dstPixel++;
                }
            }
        }
    }
    else
    {
        /* I suppose it's a LUM8 format */
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
 D3D9Raster16LinearSetFromImage

 On entry   : Raster
            : Image

            NOTE - CURRENTLY WE DON'T SUPPORT SETTING OF A PARTIAL RASTER
            HERE - IE, IF THE RASTER TO BE SET IS A SUB-RASTER

 On exit    : TRUE on success
 */
static void
D3D9Raster16LinearSetFromImage(RwRaster *ras, RwImage *image)
{
    _rwD3D9RasterExt    *rasExt;
    RwInt32             x, y;

    RWFUNCTION(RWSTRING("D3D9Raster16LinearSetFromImage"));
    RWASSERT(ras);
    RWASSERT(image);

    rasExt = RASTEREXTFROMRASTER(ras);

    if ((image->depth == 4) || (image->depth == 8))
    {
        D3D9PixConvertFn    convFn;
        RwUInt16            pal16[256];

        /* Choose a pixel convertor function */
        convFn = D3D9SelectConvertFn(rasExt->d3dFormat);
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
        switch (rasExt->d3dFormat)
        {
            case D3DFMT_X1R5G5B5:
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

            case D3DFMT_R5G6B5:
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

            case D3DFMT_A1R5G5B5:
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

            case D3DFMT_A4R4G4B4:
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

            default:
                {
                    D3D9PixConvertFn    convFn;

                    /* Choose a pixel convertor function */
                    convFn = D3D9SelectConvertFn(rasExt->d3dFormat);
                    RWASSERT(NULL != convFn);

                    for (y = 0; y < ras->height; y++)
                    {
                        srcPixel = (const RwRGBA *)(image->cpPixels + y * image->stride);
                        dstPixel = ((RwUInt16 *)(((RwUInt8 *)(ras->cpPixels)) +
                                            (ras->stride * y)));

                        for (x = 0; x < ras->width; x++)
                        {
                            *dstPixel = (RwUInt16)convFn(srcPixel);
                            srcPixel++;
                            dstPixel++;
                        }
                    }
                }
                break;
        }
    }

    RWRETURNVOID();
}

/*****************************************************************************
 D3D9Raster24_32LinearSetFromImage

 On entry   : Raster
            : Image

            NOTE - CURRENTLY WE DON'T SUPPORT SETTING OF A PARTIAL RASTER
            HERE - IE, IF THE RASTER TO BE SET IS A SUB-RASTER

 On exit    :
 */
static void
D3D9Raster24_32LinearSetFromImage(RwRaster *ras, RwImage *image)
{
    RwInt32             x, y;
    _rwD3D9RasterExt    *rasExt;
    RwUInt8             alpha = 0;

    RWFUNCTION(RWSTRING("D3D9Raster24_32LinearSetFromImage"));
    RWASSERT(ras);
    RWASSERT(image);

    rasExt = RASTEREXTFROMRASTER(ras);

    if ((4 == image->depth) || (8 == image->depth))
    {
        RwUInt32 pal32[256];

        if ((RwRasterGetFormat(ras) & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888)
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
        else if ((RwRasterGetFormat(ras) & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888)
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
        else
        {
            D3D9PixConvertFn    convFn;

            /* Choose a pixel convertor function */
            convFn = D3D9SelectConvertFn(rasExt->d3dFormat);
            RWASSERT(NULL != convFn);

            alpha = _rwD3D9PixelFormatInfo[rasExt->d3dFormat].alpha;

            for (x = 0; x < (1 << image->depth); x++)
            {
                const RwRGBA *pixIn=&(image->palette[x]);

                pal32[x] = convFn(pixIn);
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
        else
        {
            D3D9PixConvertFn    convFn;

            /* Choose a pixel convertor function */
            convFn = D3D9SelectConvertFn(rasExt->d3dFormat);
            RWASSERT(NULL != convFn);

            alpha = _rwD3D9PixelFormatInfo[rasExt->d3dFormat].alpha;

            for (y = 0; y < ras->height; y++)
            {
                RwRGBA      *srcPixel;
                RwUInt32    *dstPixel;

                srcPixel = (RwRGBA *)(image->cpPixels + (image->stride * y));
                dstPixel = (RwUInt32 *)(ras->cpPixels + (ras->stride * y));

                for (x = 0; x < ras->width; x++)
                {
                    *dstPixel = convFn(srcPixel);

                    srcPixel++;
                    dstPixel++;
                }
            }
        }
    }

    if (alpha < 0xff)
    {
        rasExt->alpha = 1;

        /* Only changed the parent if we have an alpha channel */
        if (ras != ras->parent)
        {
            _rwD3D9RasterExt    *rasExtParent;

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
 _rwD3D9RasterSetFromImage

 The raster is only set if it has a valid pixel pointer

 On entry   : Raster (MODIFY)
            : Image
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterSetFromImage(void *rasterIn,
                          void *image,
                          RwInt32 unused3 __RWUNUSED__)
{
    RwImage     *img = (RwImage *)image;
    RwRaster    *raster = (RwRaster *)rasterIn;
    _rwD3D9RasterExt    *rasExt;
    RwInt32             format;
    RwBool              rasterLocked = FALSE;
    RwBool              paletteLocked = FALSE;
    RwImage             *imgSrc;

    RWFUNCTION(RWSTRING("_rwD3D9RasterSetFromImage"));

    rasExt = RASTEREXTFROMRASTER(raster);

    /* Check if raster is compressed and fail */
    if (rasExt->compressed)
    {
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("Conversion to compressed rasters is not supported.")));

        RWRETURN(FALSE);
    }

    /* What kind of raster do we have */
    format = RwRasterGetFormat(raster);
    RWASSERT(rwRASTERFORMAT16 != format);
    RWASSERT(rwRASTERFORMAT24 != format);
    RWASSERT(rwRASTERFORMAT32 != format);

    /* Normal type rasters are created with alpha by default */
    if(raster->cType == rwRASTERTYPENORMAL)
    {
        RwInt32 rasFormat;

        rasFormat = _rwD3D9ImageFindFormat(img);

        rasExt->alpha = _rwRasterConvTable[FMT2TBL(rasFormat)].alpha;
    }
    else
    {
        if (rasExt->d3dFormat == D3DFMT_P8)
        {
            if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888)
            {
                rasExt->alpha = TRUE;
            }
            else
            {
                rasExt->alpha = FALSE;
            }
        }
        else
        {
            rasExt->alpha = _rwD3D9PixelFormatInfo[rasExt->d3dFormat].alpha;
        }
    }

    /* Check the alpha flag of the parent */
    if (rasExt->alpha)
    {
        if (raster != raster->parent)
        {
            _rwD3D9RasterExt *rasExtParent;

            rasExtParent = RASTEREXTFROMRASTER(raster->parent);

            rasExtParent->alpha = 1;
        }
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
            D3D9RasterPalletizedSetFromImage(raster, imgSrc);
            break;

        case 16:
            D3D9Raster16LinearSetFromImage(raster, imgSrc);
            break;

        case 24:
        case 32:
            D3D9Raster24_32LinearSetFromImage(raster, imgSrc);
            break;

        case 64:
        case 128:
            #if defined(RWDEBUG)
            RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D9RasterSetFromImage",
                               "Conversion to wide pixel formats is not supported.");
            #endif
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
