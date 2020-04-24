/***************************************************************************
 *                                                                         *
 * Module  : d3d8dxttex.h                                                  *
 *                                                                         *
 * Purpose : D3D8 DXT texture support                                      *
 *                                                                         *
 **************************************************************************/

#ifndef D3D8DXTTEX_H
#define D3D8DXTTEX_H

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "baimmedi.h"
#include "batextur.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Global variables (across program)
 */


/****************************************************************************
 Function prototypes
 */

/* RWPUBLIC */
#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern RwTexture *RwD3D8DDSTextureRead(const RwChar *name, const RwChar *maskname);

extern RwBool RwD3D8RasterIsCompressed(const RwRaster *raster);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */
/* RWPUBLICEND */


#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern RwBool
_rwD3D8DecompressDXT1(RwInt32 width, RwInt32 height, RwUInt32 *destPixels, const RwUInt16 *srcPixels);

extern void
_rwD3D8DecompressDXT3(RwInt32 width, RwInt32 height, RwUInt32 *destPixels, const RwUInt16 *srcPixels);

extern void
_rwD3D8DecompressDXT5(RwInt32 width, RwInt32 height, RwUInt32 *destPixels, const RwUInt16 *srcPixels);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


#endif /* D3D8DXTTEX_H */
