/***************************************************************************
 *                                                                         *
 * Module  : D3D8raster.h                                                    *
 *                                                                         *
 * Purpose : D3D8 raster header                                            *
 *                                                                         *
 **************************************************************************/

#ifndef D3D8RASTER_H
#define D3D8RASTER_H

/****************************************************************************
 Includes
 */

#include <d3d8.h>

#include "batypes.h"
#include "baimmedi.h"

/****************************************************************************
 Global Types
 */
typedef struct _rwD3D8Palette _rwD3D8Palette;
struct _rwD3D8Palette
{
    PALETTEENTRY    entries[256];
    RwInt32     globalindex;
};

typedef LPDIRECT3DSURFACE8 LPSURFACE;
typedef LPDIRECT3DTEXTURE8 LPTEXTURE;

typedef struct _rwD3D8RasterExt _rwD3D8RasterExt;
struct _rwD3D8RasterExt
{
    LPTEXTURE               texture;
    _rwD3D8Palette          *palette;
    RwUInt8                 alpha;          /* This texture has alpha */
    RwUInt8                 cube;           /* This texture is a cube texture */
    RwUInt8                 face;           /* The face of a cube texture */
    RwUInt8                 pad;
    LPSURFACE               lockedSurface;
    D3DLOCKED_RECT          lockedRect;
    RwUInt32                lockedMipLevel;
    RwUInt32                dxtFormat;      /* DXT format */
    HWND                    window;
    LPDIRECT3DSWAPCHAIN8    swapChain;
};

typedef struct _rwRasterConvData _rwRasterConvData;
struct _rwRasterConvData
{
    D3DFORMAT   format;
    RwUInt8     depth;
    RwUInt8     alpha;
};

/****************************************************************************
 Defines
 */
#define RASTEREXTFROMRASTER(raster) \
    ((_rwD3D8RasterExt *)(((RwUInt8 *)(raster)) + _RwD3D8RasterExtOffset))

#define RASTEREXTFROMCONSTRASTER(raster)                        \
    ((const _rwD3D8RasterExt *)                                 \
     (((const RwUInt8 *)(raster)) + _RwD3D8RasterExtOffset))

#define FMT2TBL(_p) (((_p) & rwRASTERFORMATPIXELFORMATMASK) >> 8)

#define NUMPIXELFORMATS 11

/****************************************************************************
 Global variables (across program)
 */

extern RwInt32 _RwD3D8RasterExtOffset;    /* Raster extension offset */

extern const _rwRasterConvData _rwRasterConvTable[NUMPIXELFORMATS];

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Plugin Attach */
extern RwBool _rwD3D8RasterPluginAttach(void);

/* Open/Close */
extern void _rwD3D8RasterOpen(void);
extern void _rwD3D8RasterClose(void);

extern void _rxD3D8VideoMemoryRasterListRelease(void);
extern RwBool _rxD3D8VideoMemoryRasterListRestore(void);

extern RwBool _rwD3D8RasterHasAlpha(const RwRaster *raster);
extern RwBool _rwD3D8TextureHasAlpha(const RwTexture *texture);
extern void _rwD3D8RasterRemoveAlphaFlag(RwRaster *raster);

extern RwBool _rwD3D8CubeRasterCreate(RwRaster *raster, RwUInt32 d3dformat, RwInt32 mipmapLevels);
extern void _rwD3D8CubeRasterSelectFace(RwRaster *raster, RwUInt32 face);
extern RwBool _rwD3D8RasterIsCubeRaster(const RwRaster *raster);

/* Locking and unlocking */
extern RwBool _rwD3D8RasterLock(void *pixelsIn, void *rasterIn, RwInt32 accessMode);
extern RwBool _rwD3D8RasterUnlock(void *unused1, void *rasterIn, RwInt32 unused3);
extern RwBool _rwD3D8RasterLockPalette(void *paletteInOut, void *rasterIn, RwInt32 accessMode);
extern RwBool _rwD3D8RasterUnlockPalette(void *unused1, void *rasterIn, RwInt32 unused3);

/* Raster operations */
extern RwBool _rwD3D8RasterClear(void *unused1, void *unused2, RwInt32 packedColor);
extern RwBool _rwD3D8RasterClearRect(void *unused1, void *rectIn, RwInt32 packedColor);
extern RwBool _rwD3D8RasterRender(void *rasterIn, void *rectIn, RwInt32 unused3);
extern RwBool _rwD3D8RasterRenderScaled(void *rasterIn, void *rectIn, RwInt32 flags);
extern RwBool _rwD3D8RasterRenderFast(void *rasterIn, void *rectIn,  RwInt32 flags);

/* Setting the context */
extern RwBool _rwD3D8SetRasterContext(void *unused1, void *rasIn, RwInt32 unused3);

/* Finding formats */
extern RwInt32 _rwD3D8FindCorrectRasterFormat(RwRasterType type, RwInt32 flags);

extern RwBool _rwD3D8CheckRasterFormat(void *rasterIn, RwInt32 flags);

/* Creating and destroying */
extern RwBool _rwD3D8RasterCreate(void *unused1, void *rasterIn, RwInt32 flags);
extern RwBool _rwD3D8RasterDestroy(void *unused1, void *raster, RwInt32 unused3);

/* Getting a rasters mip levels */
extern RwBool _rwD3D8RasterGetMipLevels(void *mipLevels, void *ras, RwInt32 unused3);

extern RwBool _rwD3D8TextureSetRaster(void *unused1, void *unused2, RwInt32 unused3);
extern RwBool _rwD3D8RasterSubRaster(void *rasterIn, void *parentRasterIn, RwInt32 unused3);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D8RASTER_H */
