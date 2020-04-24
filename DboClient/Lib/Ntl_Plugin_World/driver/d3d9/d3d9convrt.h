/***************************************************************************
 *                                                                         *
 * Module  : d3d9convrt.h                                                  *
 *                                                                         *
 * Purpose : Image <-> raster conversions                                  *
 *                                                                         *
 **************************************************************************/

#ifndef D3D9CONVRT_H
#define D3D9CONVRT_H

/****************************************************************************
 Includes
 */

/****************************************************************************
 Defines
 */

 /****************************************************************************
 Global Types
 */

/****************************************************************************
 Globals
 */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Finding the MSB */
extern RwInt32 _rwD3D9FindMSB(RwInt32 num);

/* Raster <--> Image conversions */
extern RwBool _rwD3D9ImageGetFromRaster(void *imageIn, void *rasterIn, RwInt32 num);
extern RwBool _rwD3D9RasterSetFromImage(void *rasterIn, void *image, RwInt32 flags);

/* Pixel conversions */
extern RwBool _rwD3D9RGBToPixel(void *pixel, void *col, RwInt32 format);
extern RwBool _rwD3D9PixelToRGB(void *rgb, void *pixel, RwInt32 format);

/* Finding a suitable format for a raster based on an image */
extern RwUInt32 _rwD3D9ImageFindFormat(RwImage *image);
extern RwBool _rwD3D9ImageFindRasterFormat(void *raster, void *image, RwInt32 flags);

/* Check and change if needed the width and height values */
extern void _rwD3D9CheckRasterSize(RwInt32 *width, RwInt32 *height, RwUInt32 rasterFlags);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D9CONVRT_H */
