/***************************************************************************
 *                                                                         *
 * Module  : baimras.h                                                     *
 *                                                                         *
 * Purpose : Handling interface between images and rasters                 *
 *                                                                         *
 **************************************************************************/

#ifndef RWIMRAS_H
#define RWIMRAS_H

/****************************************************************************
 Includes
 */

#include "baimage.h"
#include "baraster.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */

/* RWPUBLIC */
/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Images from rasters */
extern RwImage *RwImageSetFromRaster(RwImage *image, RwRaster *raster);

/* Rasters from images */
extern RwRaster *RwRasterSetFromImage(RwRaster *raster, RwImage *image);

/* Read a raster */
extern RwRaster *RwRasterRead(const RwChar *filename);
extern RwRaster *RwRasterReadMaskedRaster(const RwChar *filename, const RwChar *maskname);

/* Finding appropriate raster formats */
extern RwImage *RwImageFindRasterFormat(RwImage *ipImage,RwInt32 nRasterType,
                                        RwInt32 *npWidth,RwInt32 *npHeight,
                                        RwInt32 *npDepth,RwInt32 *npFormat);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWIMAGE_H */
