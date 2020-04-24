/***************************************************************************
 *                                                                         *
 * Module  : imrascnv.h                                                    *
 *                                                                         *
 * Purpose : Raster/Image conversion                                       *
 *                                                                         *
 **************************************************************************/

#ifndef IMRASCNV_H
#define IMRASCNV_H

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "balibtyp.h"
#include "baraster.h"
#include "baimage.h"
#include "barwtyp.h"

/****************************************************************************
 Defines
 */

#define CONVRGBATO8888(rgba)                            \
    ((((RwUInt32)(rgba)->alpha) << 24) |                \
     (((RwUInt32)(rgba)->red)   << 16) |                \
     (((RwUInt32)(rgba)->green) <<  8) |                \
     (((RwUInt32)(rgba)->blue)       ))                 

#define CONVRGBATO888(rgba)                             \
    ((0xFF000000) |                                     \
     (((RwUInt32)(rgba)->red)   << 16) |                \
     (((RwUInt32)(rgba)->green) <<  8) |                \
     (((RwUInt32)(rgba)->blue)       ))                 

#define CONVRGBATO4444(rgba)                            \
    (((((RwUInt32)(rgba)->alpha) << 8) & 0xF000) |      \
     ((((RwUInt32)(rgba)->red)   << 4) & 0x0F00) |      \
     ((((RwUInt32)(rgba)->green)     ) & 0x00F0) |      \
     ((((RwUInt32)(rgba)->blue)  >> 4) & 0x000F))       

#define CONVRGBATO1555(rgba)                            \
    (((((RwUInt32)(rgba)->alpha) << 8) & 0x8000) |      \
     ((((RwUInt32)(rgba)->red)   << 7) & 0x7c00) |      \
     ((((RwUInt32)(rgba)->green) << 2) & 0x03e0) |      \
     ((((RwUInt32)(rgba)->blue)  >> 3) & 0x001f))       

#define CONVRGBATO565(rgba)                             \
    (((((RwUInt32)(rgba)->red)   << 8) & 0xF800) |      \
     ((((RwUInt32)(rgba)->green) << 3) & 0x07e0) |      \
     ((((RwUInt32)(rgba)->blue)  >> 3) & 0x001f))       

#define CONV8888TORGBA(rgba, pixel)                     \
MACRO_START                                             \
{                                                       \
    (rgba)->alpha = (RwUInt8)(pixel >> 16);             \
    (rgba)->red   = (RwUInt8)(pixel >> 8);              \
    (rgba)->green = (RwUInt8)(pixel >> 8);              \
    (rgba)->blue  = (RwUInt8)(pixel);                   \
}                                                       \
MACRO_STOP                                              \
                                                        

#define CONV888TORGBA(rgba, pixel)                      \
MACRO_START                                             \
{                                                       \
    (rgba)->alpha = (RwUInt8)255;                       \
    (rgba)->red   = (RwUInt8)(pixel >> 8);              \
    (rgba)->green = (RwUInt8)(pixel >> 8);              \
    (rgba)->blue  = (RwUInt8)(pixel);                   \
}                                                       \
MACRO_STOP                                              

#define CONV4444TORGBA(rgba, pixel)                     \
MACRO_START                                             \
{                                                       \
    (rgba)->alpha = (RwUInt8)((pixel >> 8) & 0xF0);     \
    (rgba)->red   = (RwUInt8)((pixel >> 4) & 0xF0);     \
    (rgba)->green = (RwUInt8)((pixel     ) & 0xF0);     \
    (rgba)->blue  = (RwUInt8)((pixel << 4) & 0xF0);     \
}                                                       \
MACRO_STOP                                              

#define CONV1555TORGBA(rgba, pixel)                     \
MACRO_START                                             \
{                                                       \
    (rgba)->alpha = (pixel & 0x8000) ? 255 : 0;         \
    (rgba)->red   = (RwUInt8)((pixel >> 7) & 0xF8);     \
    (rgba)->green = (RwUInt8)((pixel >> 2) & 0xF8);     \
    (rgba)->blue  = (RwUInt8)((pixel << 3) & 0xF8);     \
}                                                       \
MACRO_STOP                                              

#define CONV565TORGBA(rgba, pixel)                      \
MACRO_START                                             \
{                                                       \
    (rgba)->alpha = 255;                                \
    (rgba)->red   = (RwUInt8)((pixel >> 8) & 0xF8);     \
    (rgba)->green = (RwUInt8)((pixel >> 3) & 0xFC);     \
    (rgba)->blue  = (RwUInt8)((pixel << 3) & 0xF8);     \
}                                                       \
MACRO_STOP
    
/****************************************************************************
 Global Types
 */

enum rwImageAlphaType
{
    rwIMAGEALPHA_OPAQUE = 1,
    rwIMAGEALPHA_MASKED = 2,
    rwIMAGEALPHA_SMOOTH = 3,
    rwIMAGEALPHATYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum rwImageAlphaType rwImageAlphaType;

typedef void (*mipmapPixFn)(RwRGBA *colOut, const RwUInt8 *pixel,
                            const RwImage *image, RwInt32 size);

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern void
_rwRGBAFromImage8(RwRGBA *colOut,
                 const RwUInt8 *pixel,
                 const RwImage *image,
                 RwInt32 size);

extern void
_rwRGBAFromImage32(RwRGBA *colOut,
                  const RwUInt8 *pixel,
                  const RwImage *image,
                  RwInt32 size);

extern rwImageAlphaType
_rwImageFindAlphaType(RwImage *image);

extern RwInt32
_rwImageFindNearestPaletteColor(RwRGBA *palette,
                               RwRGBA *color,
                               RwInt32 palSize);

extern RwImage *
_rwImageSetFromRaster8888(RwImage *image,
                         RwUInt8 *rasPixels,
                         RwInt32 rasStride);

extern RwImage *
_rwImageSetFromRaster888(RwImage *image,
                        RwUInt8 *rasPixels,
                        RwInt32 rasStride);

extern RwImage *
_rwImageSetFromRaster4444(RwImage *image,
                         RwUInt8 *rasPixels,
                         RwInt32 rasStride);

extern RwImage *
_rwImageSetFromRaster1555(RwImage *image,
                         RwUInt8 *rasPixels,
                         RwInt32 rasStride);

extern RwImage *
_rwImageSetFromRaster565(RwImage *image,
                        RwUInt8 *rasPixels,
                        RwInt32 rasStride);

extern RwBool
_rwMipmapRasterSetFromImage8888(RwUInt8 *rasPixels,
                               RwInt32 rasStride,
                               RwImage *image,
                               RwInt32 level);

extern RwBool
_rwMipmapRasterSetFromImage888(RwUInt8 *rasPixels,
                              RwInt32 rasStride,
                              RwImage *image,
                              RwInt32 level);

extern RwBool
_rwMipmapRasterSetFromImage4444(RwUInt8 *rasPixels,
                               RwInt32 rasStride,
                               RwImage *image,
                               RwInt32 level);

extern RwBool
_rwMipmapRasterSetFromImage1555(RwUInt8 *rasPixels,
                               RwInt32 rasStride,
                               RwImage *image,
                               RwInt32 level);

extern RwBool
_rwMipmapRasterSetFromImage565(RwUInt8 *rasPixels,
                              RwInt32 rasStride,
                              RwImage *image,
                              RwInt32 level);

extern RwBool
_rwRasterSetFromImage8888(RwUInt8 *rasPixels,
                         RwInt32 rasStride,
                         RwImage *image);

extern RwBool
_rwRasterSetFromImage888(RwUInt8 *rasPixels,
                        RwInt32 rasStride,
                        RwImage *image);

extern RwBool
_rwRasterSetFromImage4444(RwUInt8 *rasPixels,
                         RwInt32 rasStride,
                         RwImage *image);

extern RwBool
_rwRasterSetFromImage1555(RwUInt8 *rasPixels,
                         RwInt32 rasStride,
                         RwImage *image);

extern RwBool
_rwRasterSetFromImage565(RwUInt8 *rasPixels,
                        RwInt32 rasStride,
                        RwImage *image);


#define rwRGBAFromImage8(colOut, pixel, image, size) \
       _rwRGBAFromImage8(colOut, pixel, image, size)  

#define rwRGBAFromImage32(colOut, pixel, image, size) \
       _rwRGBAFromImage32(colOut, pixel, image, size)  

#define rwImageFindAlphaType(image) \
       _rwImageFindAlphaType(image)  

#define rwImageFindNearestPaletteColor(palette, color, palSize) \
       _rwImageFindNearestPaletteColor(palette, color, palSize)  

#define rwImageSetFromRaster8888(image, rasPixels, rasStride) \
       _rwImageSetFromRaster8888(image, rasPixels, rasStride)  

#define rwImageSetFromRaster888(image, rasPixels, rasStride) \
       _rwImageSetFromRaster888(image, rasPixels, rasStride)  

#define rwImageSetFromRaster4444(image, rasPixels, rasStride) \
       _rwImageSetFromRaster4444(image, rasPixels, rasStride)  

#define rwImageSetFromRaster1555(image, rasPixels, rasStride) \
       _rwImageSetFromRaster1555(image, rasPixels, rasStride)  

#define rwImageSetFromRaster565(image, rasPixels, rasStride) \
       _rwImageSetFromRaster565(image, rasPixels, rasStride)  

#define rwMipmapRasterSetFromImage8888(rasPixels, rasStride, image, level) \
       _rwMipmapRasterSetFromImage8888(rasPixels, rasStride, image, level)  

#define rwMipmapRasterSetFromImage888(rasPixels, rasStride, image, level) \
       _rwMipmapRasterSetFromImage888(rasPixels, rasStride, image, level)  

#define rwMipmapRasterSetFromImage4444(rasPixels, rasStride, image, level) \
       _rwMipmapRasterSetFromImage4444(rasPixels, rasStride, image, level)  

#define rwMipmapRasterSetFromImage1555(rasPixels, rasStride, image, level) \
       _rwMipmapRasterSetFromImage1555(rasPixels, rasStride, image, level)  

#define rwMipmapRasterSetFromImage565(rasPixels, rasStride, image, level) \
       _rwMipmapRasterSetFromImage565(rasPixels, rasStride, image, level)  

#define rwRasterSetFromImage8888(rasPixels, rasStride, image) \
       _rwRasterSetFromImage8888(rasPixels, rasStride, image)  

#define rwRasterSetFromImage888(rasPixels, rasStride, image) \
       _rwRasterSetFromImage888(rasPixels, rasStride, image)  

#define rwRasterSetFromImage4444(rasPixels, rasStride, image) \
       _rwRasterSetFromImage4444(rasPixels, rasStride, image)  

#define rwRasterSetFromImage1555(rasPixels, rasStride, image) \
       _rwRasterSetFromImage1555(rasPixels, rasStride, image)  

#define rwRasterSetFromImage565(rasPixels, rasStride, image) \
       _rwRasterSetFromImage565(rasPixels, rasStride, image)  

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* IMRASCNV_H */

