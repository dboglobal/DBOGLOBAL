/***************************************************************************
 *                                                                         *
 * Module  : D3D9raster.c                                                    *
 *                                                                         *
 * Purpose :                                                               *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

/* D3D9 Libraries */
#include <d3d9.h>

#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "barwtyp.h"
#include "batextur.h"

#include "d3d9device.h"
#include "drvfns.h"
#include "d3d9convrt.h"
#include "d3d9rendst.h"
#include "d3d9raster.h"
#include "d3d92drend.h"

/****************************************************************************
 Defines
 */

#define NUMPIXELFORMATS     11

#define MAX_PIXEL_FORMATS   128

#define SETFORMATINFO(_format, _alpha, _depth, _rwformat)   \
    _rwD3D9PixelFormatInfo[_format].alpha = _alpha; \
    _rwD3D9PixelFormatInfo[_format].depth = _depth; \
    _rwD3D9PixelFormatInfo[_format].rwFormat = _rwformat


#define IS_D3DFORMAT_ZBUFFER(_d3dformat)    ((_d3dformat) >= D3DFMT_D16_LOCKABLE && (_d3dformat) <= D3DFMT_D16)


/****************************************************************************
 Globals (across program)
 */

RwInt32 _RwD3D9RasterExtOffset;    /* Raster extension offset */

const _rwD3D9RasterConvData _rwRasterConvTable[NUMPIXELFORMATS] =
{
    {D3DFMT_UNKNOWN,       0, 0},   /* rwRASTERFORMATDEFAULT */
    {D3DFMT_A1R5G5B5,     16, 1},   /* rwRASTERFORMAT1555    */
    {D3DFMT_R5G6B5,       16, 0},   /* rwRASTERFORMAT565     */
    {D3DFMT_A4R4G4B4,     16, 1},   /* rwRASTERFORMAT4444    */
    {D3DFMT_L8,            8, 0},   /* rwRASTERFORMATLUM8    */
    {D3DFMT_A8R8G8B8,     32, 1},   /* rwRASTERFORMAT8888    */
    {D3DFMT_X8R8G8B8,     32, 0},   /* rwRASTERFORMAT888     */
    {D3DFMT_D16,          16, 0},   /* rwRASTERFORMAT16      */
    {D3DFMT_D24X8,        32, 0},   /* rwRASTERFORMAT24      */
    {D3DFMT_D32,          32, 0},   /* rwRASTERFORMAT32      */
    {D3DFMT_X1R5G5B5,     16, 0}    /* rwRASTERFORMAT555     */
};

/* D3D9 pixel format table */
_rwD3D9FormatInfo _rwD3D9PixelFormatInfo[MAX_PIXEL_FORMATS];

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Local Types
 */
typedef struct _rxD3D9VideoMemoryRaster RxD3D9VideoMemoryRaster;
struct _rxD3D9VideoMemoryRaster
{
    RwRaster                *raster;
    RxD3D9VideoMemoryRaster *next;
};

/****************************************************************************
 Local (static)
 */

/* Current destination raster */
static RwRaster *_RwD3D9CurrentRasterTarget = NULL;

static RwIm2DVertex _RwD3D9QuadVerts[4];

/* empty index for a new pallete */
static RwUInt32 NumPaletteIndexFree = 0;
static RwUInt32 MaxPaletteIndexFree = 0;
static RwUInt32 MaxPaletteIndex = 0;
static RwUInt16 *PaletteIndexFree = NULL;

static RwFreeList *PaletteFreeList = NULL;

/* Video memory raster list */
static RxD3D9VideoMemoryRaster  *VideoMemoryRasters = NULL;
static RwFreeList               *VideoMemoryRastersFreeList = NULL;

static void rxD3D9VideoMemoryRasterListAdd(RwRaster *raster);

static void rxD3D9VideoMemoryRasterListRemove(RwRaster *raster);

static RwBool rxD3D9VideoMemoryRasterListCreate(void);

static void rxD3D9VideoMemoryRasterListDestroy(void);

#if defined(RWDEBUG)
RwUInt32  NumVideoMemoryRasters = 0;
RwUInt32  NumNormalRasters = 0;
RwUInt32  NumTextureRasters = 0;
RwUInt32  NumCameraTextureRasters = 0;
RwUInt32  NumCameraRasters = 0;
RwUInt32  NumZBufferRasters = 0;
#endif /* defined(RWDEBUG) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           Raster Ctor/Dtor

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 rwD3D9RasterCtor

 On entry   : Object, offset, size
 On exit    : Object pointer on success
 */

static void               *
rwD3D9RasterCtor(void *object,
                 RwInt32 offsetInObject __RWUNUSED__,
                 RwInt32 sizeInObject __RWUNUSED__)
{
    _rwD3D9RasterExt    *rasExt = RASTEREXTFROMRASTER(object);

    RWFUNCTION(RWSTRING("rwD3D9RasterCtor"));
    RWASSERT(object);

    /* These are used to detect when we are using a camera for the first time */
    rasExt->swapChain = NULL;

    RWRETURN(object);
}

/****************************************************************************
 rwD3D9RasterDtor

 On entry   : Object, offset, size
 On exit    : Object pointer on success
 */

static void               *
rwD3D9RasterDtor(void *object,
                 RwInt32 offsetInObject __RWUNUSED__,
                 RwInt32 sizeInObject __RWUNUSED__)
{
    _rwD3D9RasterExt    *rasExt = RASTEREXTFROMRASTER(object);

    RWFUNCTION(RWSTRING("rwD3D9RasterDtor"));
    RWASSERT(object);

    if (rasExt->swapChain)
    {
        IDirect3DSwapChain9_Release(rasExt->swapChain);
        rasExt->swapChain = NULL;
    }

    /* Phew! */
    RWRETURN(object);
}

/****************************************************************************
 _rwD3D9RasterPluginAttach

 On entry   :
 On exit    :
 */
RwBool
_rwD3D9RasterPluginAttach(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9RasterPluginAttach"));

    _RwD3D9RasterExtOffset =
        RwRasterRegisterPlugin(
            sizeof(_rwD3D9RasterExt),
            rwID_DEVICEMODULE,
            rwD3D9RasterCtor,
            rwD3D9RasterDtor,
            NULL);

    if (0 > _RwD3D9RasterExtOffset)
    {
        RWRETURN(FALSE);
    }

    /* Fill format info table */
    memset(_rwD3D9PixelFormatInfo, 0, MAX_PIXEL_FORMATS * sizeof(_rwD3D9FormatInfo));

    SETFORMATINFO(D3DFMT_R8G8B8, FALSE, 24, 0);
    SETFORMATINFO(D3DFMT_A8R8G8B8, TRUE, 32, rwRASTERFORMAT8888);
    SETFORMATINFO(D3DFMT_X8R8G8B8, FALSE, 32, rwRASTERFORMAT888);
    SETFORMATINFO(D3DFMT_R5G6B5, FALSE, 16, rwRASTERFORMAT565);
    SETFORMATINFO(D3DFMT_X1R5G5B5, FALSE, 16, rwRASTERFORMAT555);
    SETFORMATINFO(D3DFMT_A1R5G5B5, TRUE, 16, rwRASTERFORMAT1555);
    SETFORMATINFO(D3DFMT_A4R4G4B4, TRUE, 16, rwRASTERFORMAT4444);
    SETFORMATINFO(D3DFMT_R3G3B2, FALSE, 8, 0);
    SETFORMATINFO(D3DFMT_A8, TRUE, 8, 0);
    SETFORMATINFO(D3DFMT_A8R3G3B2, TRUE, 16, 0);
    SETFORMATINFO(D3DFMT_X4R4G4B4, FALSE, 16, 0);
    SETFORMATINFO(D3DFMT_A2B10G10R10, TRUE, 32, 0);
    SETFORMATINFO(D3DFMT_A8B8G8R8, TRUE, 32, 0);
    SETFORMATINFO(D3DFMT_X8B8G8R8, FALSE, 32, 0);
    SETFORMATINFO(D3DFMT_G16R16, FALSE, 32, 0);
    SETFORMATINFO(D3DFMT_A2R10G10B10, TRUE, 32, 0);
    SETFORMATINFO(D3DFMT_A16B16G16R16, TRUE, 64, 0);
    SETFORMATINFO(D3DFMT_A8P8, TRUE, 16, 0);
    SETFORMATINFO(D3DFMT_P8, FALSE, 8, rwRASTERFORMATPAL8);
    SETFORMATINFO(D3DFMT_L8, FALSE, 8, rwRASTERFORMATLUM8);
    SETFORMATINFO(D3DFMT_A8L8, TRUE, 16, 0);
    SETFORMATINFO(D3DFMT_A4L4, TRUE, 8, 0);
    SETFORMATINFO(D3DFMT_V8U8, FALSE, 16, 0);
    SETFORMATINFO(D3DFMT_L6V5U5, FALSE, 16, 0);
    SETFORMATINFO(D3DFMT_X8L8V8U8, FALSE, 32, 0);
    SETFORMATINFO(D3DFMT_Q8W8V8U8, FALSE, 32, 0);
    SETFORMATINFO(D3DFMT_V16U16, FALSE, 32, 0);
    SETFORMATINFO(D3DFMT_A2W10V10U10, TRUE, 32, 0);
    SETFORMATINFO(D3DFMT_D16_LOCKABLE, FALSE, 16, rwRASTERFORMAT16);
    SETFORMATINFO(D3DFMT_D32, FALSE, 32, rwRASTERFORMAT32);
    SETFORMATINFO(D3DFMT_D15S1, FALSE, 16, rwRASTERFORMAT16);
    SETFORMATINFO(D3DFMT_D24S8, FALSE, 32, rwRASTERFORMAT32);
    SETFORMATINFO(D3DFMT_D24X8, FALSE, 32, rwRASTERFORMAT32);
    SETFORMATINFO(D3DFMT_D24X4S4, FALSE, 32, rwRASTERFORMAT32);
    SETFORMATINFO(D3DFMT_D16, FALSE, 16, rwRASTERFORMAT16);
    SETFORMATINFO(D3DFMT_D32F_LOCKABLE, FALSE, 32, rwRASTERFORMAT32);
    SETFORMATINFO(D3DFMT_D24FS8, FALSE, 32, rwRASTERFORMAT32);
    SETFORMATINFO(D3DFMT_L16, FALSE, 16, 0);
    SETFORMATINFO(D3DFMT_Q16W16V16U16, FALSE, 64, 0);
    SETFORMATINFO(D3DFMT_R16F, FALSE, 16, 0);
    SETFORMATINFO(D3DFMT_G16R16F, FALSE, 32, 0);
    SETFORMATINFO(D3DFMT_A16B16G16R16F, TRUE, 64, 0);
    SETFORMATINFO(D3DFMT_R32F, FALSE, 32, 0);
    SETFORMATINFO(D3DFMT_G32R32F, FALSE, 64, 0);
    SETFORMATINFO(D3DFMT_A32B32G32R32F, TRUE, 128, 0);
    SETFORMATINFO(D3DFMT_CxV8U8, FALSE, 16, 0);

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterOpen

 On entry   :
 On exit    :
 */
void
_rwD3D9RasterOpen(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9RasterOpen"));

    rxD3D9VideoMemoryRasterListCreate();

    #if defined(RWDEBUG)
    NumNormalRasters = 0;
    NumTextureRasters = 0;
    NumCameraTextureRasters = 0;
    NumCameraRasters = 0;
    NumZBufferRasters = 0;
    #endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9RasterClose

 On entry   :
 On exit    :
 */
void
_rwD3D9RasterClose(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9RasterClose"));

    rxD3D9VideoMemoryRasterListDestroy();

    NumPaletteIndexFree = 0;
    MaxPaletteIndexFree = 0;
    MaxPaletteIndex = 0;

    if (PaletteIndexFree != NULL)
    {
        RwFree(PaletteIndexFree);
        PaletteIndexFree = NULL;
    }

    if (PaletteFreeList != NULL)
    {
        RwFreeListDestroy(PaletteFreeList);
        PaletteFreeList = NULL;
    }

    #if defined(RWDEBUG)
    RWASSERT(NumNormalRasters == 0);
    RWASSERT(NumTextureRasters == 0);
    RWASSERT(NumCameraTextureRasters == 0);
    RWASSERT(NumCameraRasters == 0);
    RWASSERT(NumZBufferRasters == 0);
    #endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D9FindRwFormat

 On entry   :
 On exit    :
 */
static RwUInt32
rwD3D9FindRwFormat(D3DFORMAT d3dFormat)
{
    RWFUNCTION(RWSTRING("rwD3D9FindRwFormat"));
    RWASSERT(d3dFormat < MAX_PIXEL_FORMATS);

    RWRETURN(_rwD3D9PixelFormatInfo[d3dFormat].rwFormat);
}

/****************************************************************************
 _rwD3D9RasterHasAlpha

 On entry   :
 On exit    :
 */
RwBool
_rwD3D9RasterHasAlpha(const RwRaster *raster)
{
    const _rwD3D9RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D9RasterHasAlpha"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMCONSTRASTER(raster);

    RWRETURN(rasExt->alpha);
}

/****************************************************************************
 _rwD3D9TextureHasAlpha

 On entry   :
 On exit    :
 */
RwBool
_rwD3D9TextureHasAlpha(const RwTexture *texture)
{
    RWFUNCTION(RWSTRING("_rwD3D9TextureHasAlpha"));
    RWASSERT(texture);

    if (texture->raster != NULL)
    {
        const _rwD3D9RasterExt  *rasExt;

        rasExt = RASTEREXTFROMRASTER(texture->raster);

        RWRETURN(rasExt->alpha);
    }
    else
    {
        RWRETURN(FALSE);
    }
}

/****************************************************************************
 _rwD3D9RasterRemoveAlphaFlag

 On entry   :
 On exit    :
 */
void
_rwD3D9RasterRemoveAlphaFlag(RwRaster *raster)
{
    _rwD3D9RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D9RasterRemoveAlphaFlag"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMRASTER(raster);

    rasExt->alpha = FALSE;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9CubeRasterCreate

 On entry   :
 On exit    :
 */
RwBool
_rwD3D9CubeRasterCreate(RwRaster *raster, RwUInt32 d3dformat, RwInt32 mipmapLevels)
{
    _rwD3D9RasterExt  *rasExt;
    RwUInt32 levels;
    HRESULT hr;

    RWFUNCTION(RWSTRING("_rwD3D9CubeRasterCreate"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMRASTER(raster);

    rasExt->cube = TRUE;

    rasExt->d3dFormat = d3dformat;

    if (_RwD3D9DeviceCaps.CubeTextureFilterCaps & (D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR))
    {
        levels = mipmapLevels;
    }
    else
    {
        #ifdef RWDEBUG
        if (mipmapLevels > 1)
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D9CubeRasterCreate",
                              "Cube texture mip-mapping not supported");
        }
        #endif

        levels = 1;
    }

    if ((raster->cType & rwRASTERTYPEMASK) != rwRASTERTYPECAMERATEXTURE)
    {
        hr = DXCHECK(
            IDirect3DDevice9_CreateCubeTexture(_RwD3DDevice,
                                               raster->width,
                                               levels,
                                               (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0),
                                               rasExt->d3dFormat,
                                               D3DPOOL_MANAGED,
                                               (LPDIRECT3DCUBETEXTURE9 *)&(rasExt->texture),
                                               NULL));
    }
    else
    {
        hr = DXCHECK(
            IDirect3DDevice9_CreateCubeTexture(_RwD3DDevice,
                                               raster->width,
                                               levels,
                                               (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0) |
                                               D3DUSAGE_RENDERTARGET,
                                               rasExt->d3dFormat,
                                               D3DPOOL_DEFAULT,
                                               (LPDIRECT3DCUBETEXTURE9 *)&(rasExt->texture),
                                               NULL));

        if (SUCCEEDED(hr))
        {
            rxD3D9VideoMemoryRasterListAdd(raster);
        }
    }

    if (FAILED(hr))
    {
        RWRETURN(FALSE);
    }

    /* Remove the rwRASTERDONTALLOCATE flag so it will get destroyed */
    raster->cFlags &= ~rwRASTERDONTALLOCATE;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9CubeRasterSelectFace

 On entry   :
 On exit    :
 */
void
_rwD3D9CubeRasterSelectFace(RwRaster *raster, RwUInt32 face)
{
    _rwD3D9RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D9CubeRasterSelectFace"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMRASTER(raster);
    RWASSERT(rasExt->cube);

    RWASSERT(face < 6);
    rasExt->face = face;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9RasterIsCubeRaster

 On entry   :
 On exit    :
 */
RwBool
_rwD3D9RasterIsCubeRaster(const RwRaster *raster)
{
    const _rwD3D9RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D9RasterIsCubeRaster"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMCONSTRASTER(raster);

    RWRETURN(rasExt->cube);
}

/****************************************************************************
 _rwD3D9RasterLock

 Locks a raster

 On entry   : NULL
            : raster - raster to allocate
            : Flags, rwRASTERLOCKREAD,
                     rwRASTERLOCKWRITE,
                     rwRASTERLOCKREADWRITE
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterLock(void *pixelsIn, void *rasterIn, RwInt32 accessMode)
{
    RwUInt8         **pixels = (RwUInt8 **)pixelsIn;
    RwRaster        *raster = (RwRaster*)rasterIn;
    RwRaster        *topRaster;
    _rwD3D9RasterExt  *rasExt;
    _rwD3D9RasterExt  *topRasExt;
    RwUInt8         mipLevel;
    RwUInt32        flags;
    HRESULT         hr = D3D_OK;


    RWFUNCTION(RWSTRING("_rwD3D9RasterLock"));

    /* Can't rasters that have already been locked */
    if (raster->cpPixels != NULL)
    {
        RWERROR((E_RW_INVRASTERLOCKREQ));
        RWRETURN(FALSE);
    }

    rasExt = RASTEREXTFROMRASTER(raster);

    /* Get the top level raster as this is the only one with a valid texture */
    topRaster = raster;
    while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

    topRasExt = RASTEREXTFROMRASTER(topRaster);

    /* Prepare lock info */
    mipLevel = (RwUInt8)((accessMode & (RwInt32)0xFF00) >> 8);

    flags = D3DLOCK_NOSYSLOCK;

    if (!(accessMode & rwRASTERLOCKWRITE))
    {
        flags |= D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_READONLY;
    }

    switch (raster->cType & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
            {
                if (topRasExt->cube)
                {
                    hr = IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)topRasExt->texture,
                                                                (D3DCUBEMAP_FACES)topRasExt->face,
                                                                mipLevel,
                                                                &(rasExt->lockedSurface));
                }
                else
                {
                    hr = IDirect3DTexture9_GetSurfaceLevel(topRasExt->texture, mipLevel, &(rasExt->lockedSurface));
                }

                if (SUCCEEDED(hr))
                {
                    if (topRaster == raster)
                    {
                        hr = IDirect3DSurface9_LockRect(rasExt->lockedSurface,
                                                        &rasExt->lockedRect,
                                                        NULL,
                                                        flags);
                    }
                    else
                    {
                        RECT rect;

                        rect.left = raster->nOffsetX;
                        rect.top = raster->nOffsetY;
                        rect.right = raster->nOffsetX + raster->width;
                        rect.bottom = raster->nOffsetY + raster->height;

                        hr = IDirect3DSurface9_LockRect(rasExt->lockedSurface,
                                            &rasExt->lockedRect,
                                            &rect,
                                            flags);
                   }
                }

                if (SUCCEEDED(hr))
                {
                    if (accessMode & rwRASTERLOCKREAD)
                    {
                        raster->privateFlags |= rwRASTERPIXELLOCKEDREAD;
                    }

                    if (accessMode & rwRASTERLOCKWRITE)
                    {
                        raster->privateFlags |= rwRASTERPIXELLOCKEDWRITE;
                    }
                }
                else
                {
                    RWRETURN(FALSE);
                }
            }
            break;

        case rwRASTERTYPECAMERATEXTURE:
        case rwRASTERTYPECAMERA:
            {
                LPSURFACE       surface;
                D3DSURFACE_DESC d3dDesc;

                if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
                {
                    if (topRasExt->cube)
                    {
                        hr = IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)topRasExt->texture,
                                                                     (D3DCUBEMAP_FACES)topRasExt->face,
                                                                     mipLevel,
                                                                     &surface);
                    }
                    else
                    {
                        hr = IDirect3DTexture9_GetSurfaceLevel(topRasExt->texture, mipLevel, &surface);
                    }
                }
                else
                {
                    if (topRasExt->swapChain)
                    {
                        surface = (LPSURFACE)topRasExt->texture;
                    }
                    else
                    {
                        surface = _RwD3D9RenderSurface;
                    }

                    hr = D3D_OK;
                }

                if (SUCCEEDED(hr))
                {
                    IDirect3DSurface9_GetDesc(surface, &d3dDesc);

                    /* Create a system memory surface */
                    hr = IDirect3DDevice9_CreateOffscreenPlainSurface(_RwD3DDevice,
                                                                        d3dDesc.Width,
                                                                        d3dDesc.Height,
                                                                        d3dDesc.Format,
                                                                        D3DPOOL_SYSTEMMEM,
                                                                        &(rasExt->lockedSurface),
                                                                        NULL);

                    if (SUCCEEDED(hr))
                    {
                        /* Copy the contents if needed */
                        if (accessMode & rwRASTERLOCKREAD)
                        {
                            IDirect3DDevice9_GetRenderTargetData(_RwD3DDevice,
                                                                 surface,
                                                                 rasExt->lockedSurface);
                        }

                        if (SUCCEEDED(hr))
                        {
                            if (topRaster == raster)
                            {
                                hr = IDirect3DSurface9_LockRect(rasExt->lockedSurface,
                                                                &rasExt->lockedRect,
                                                                NULL,
                                                                flags);
                            }
                            else
                            {
                                RECT rect;

                                rect.left = raster->nOffsetX;
                                rect.top = raster->nOffsetY;
                                rect.right = raster->nOffsetX + raster->width;
                                rect.bottom = raster->nOffsetY + raster->height;

                                hr = IDirect3DSurface9_LockRect(rasExt->lockedSurface,
                                                    &rasExt->lockedRect,
                                                    &rect,
                                                    flags);
                            }

                            if (SUCCEEDED(hr))
                            {
                                if (accessMode & rwRASTERLOCKREAD)
                                {
                                    raster->privateFlags |= rwRASTERPIXELLOCKEDREAD;
                                }

                                if (accessMode & rwRASTERLOCKWRITE)
                                {
                                    raster->privateFlags |= rwRASTERPIXELLOCKEDWRITE;
                                }
                            }
                        }

                        if (FAILED(hr))
                        {
                            IDirect3DSurface9_Release(rasExt->lockedSurface);
                        }
                    }

                    if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
                    {
                        IDirect3DSurface9_Release(surface);
                    }
                }

                if (FAILED(hr))
                {
                    RWRETURN(FALSE);
                }
            }
            break;

        case rwRASTERTYPEZBUFFER:
        default:
            RWERROR((E_RW_INVRASTERLOCKREQ));
            RWRETURN(FALSE);
            break;
    }

    /* Pixels */
    raster->cpPixels = (RwUInt8 *)rasExt->lockedRect.pBits;

    /* Cache original width, height & stride */
    raster->originalWidth = raster->width;
    raster->originalHeight = raster->height;

    /* Mip level width, height & stride */
    raster->width = raster->width >> mipLevel;
    raster->height = raster->height >> mipLevel;

    /* Clamp width and height to 1 */
    if (raster->width == 0)
    {
        raster->width = 1;
    }

    if (raster->height == 0)
    {
        raster->height = 1;
    }

    /* Set the stride */
    raster->stride = rasExt->lockedRect.Pitch;

    /* Cache the mip level locked */
    rasExt->lockedMipLevel = mipLevel;

    /* Save off the pixel pointer */
    (*pixels) = raster->cpPixels;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterUnlock

 Unlocks a raster

 On entry   : NULL
            : raster - raster to allocate
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterUnlock(void *unused1 __RWUNUSED__,
                  void *rasterIn,
                  RwInt32 unused3 __RWUNUSED__)
{
    RwRaster        *raster = (RwRaster*)rasterIn;
    _rwD3D9RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D9RasterUnlock"));

    /* Can't unlock rasters that have not been locked */
    if (raster->cpPixels == NULL)
    {
        RWERROR((E_RW_INVRASTERUNLOCKREQ));
        RWRETURN(FALSE);
    }

    rasExt = RASTEREXTFROMRASTER(raster);

    switch (raster->cType & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
            {
                IDirect3DSurface9_UnlockRect(rasExt->lockedSurface);

                IDirect3DSurface9_Release(rasExt->lockedSurface);

                /* Restore the original width, height & stride */
                raster->width = raster->originalWidth;
                raster->height = raster->originalHeight;

                raster->stride = 0;
                raster->cpPixels = NULL;

                if ((raster->privateFlags & rwRASTERPIXELLOCKEDWRITE) &&
                    (raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8)) &&
                    (rasExt->lockedMipLevel == 0) &&
                    (rasExt->automipmapgen == 0))
                {
                    RwRaster    *topRaster;

                    raster->privateFlags = (RwUInt8)(raster->privateFlags & ~rwRASTERPIXELLOCKED);

                    rasExt->lockedMipLevel = (RwUInt8)-1;

                    /* Get the top level raster as this is the only one with a valid texture */
                    topRaster = raster;
                    while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

                    RwTextureRasterGenerateMipmaps(topRaster, NULL);
                }
                else
                {
                    raster->privateFlags = (RwUInt8)(raster->privateFlags & ~rwRASTERPIXELLOCKED);
                }
            }
            break;

        case rwRASTERTYPECAMERATEXTURE:
        case rwRASTERTYPECAMERA:
            {
                /* Restore the original width, height & stride */
                raster->width = raster->originalWidth;
                raster->height = raster->originalHeight;

                raster->stride = 0;
                raster->cpPixels = NULL;

                /* Unlock system memory surface */
                IDirect3DSurface9_UnlockRect(rasExt->lockedSurface);

                /* Update video memory surface if needed */
                if (raster->privateFlags & rwRASTERPIXELLOCKEDWRITE)
                {
                    LPSURFACE           surface;
                    HRESULT             hr;
                    RwRaster            *topRaster;
                    _rwD3D9RasterExt    *topRasExt;


                    /* Get the top level raster as this is the only one with a valid texture */
                    topRaster = raster;
                    while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

                    topRasExt = RASTEREXTFROMRASTER(topRaster);

                    if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
                    {
                        if (topRasExt->cube)
                        {
                            hr = IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)topRasExt->texture,
                                                                        (D3DCUBEMAP_FACES)topRasExt->face,
                                                                        rasExt->lockedMipLevel,
                                                                        &surface);
                        }
                        else
                        {
                            hr = IDirect3DTexture9_GetSurfaceLevel(topRasExt->texture,
                                                                   rasExt->lockedMipLevel,
                                                                   &surface);
                        }
                    }
                    else
                    {
                        if (topRasExt->swapChain)
                        {
                            surface = (LPSURFACE)topRasExt->texture;
                        }
                        else
                        {
                            surface = _RwD3D9RenderSurface;
                        }

                        hr = D3D_OK;
                    }

                    if (SUCCEEDED(hr))
                    {
                        if (topRaster == raster)
                        {
                            hr = DXCHECK(IDirect3DDevice9_UpdateSurface(_RwD3DDevice, rasExt->lockedSurface, NULL, surface, NULL));
                        }
                        else
                        {
                            RECT rect;
                            POINT pt;

                            rect.left = raster->nOffsetX;
                            rect.top = raster->nOffsetY;
                            rect.right = rect.left + raster->width;
                            rect.bottom = rect.top + raster->height;

                            pt.x = raster->nOffsetX;
                            pt.y = raster->nOffsetY;

                            hr = DXCHECK(IDirect3DDevice9_UpdateSurface(_RwD3DDevice, rasExt->lockedSurface, &rect, surface, &pt));
                        }

                        if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
                        {
                            IDirect3DSurface9_Release(surface);
                        }
                    }

                    IDirect3DSurface9_Release(rasExt->lockedSurface);

                    raster->privateFlags = (RwUInt8)(raster->privateFlags & ~rwRASTERPIXELLOCKED);

                    if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
                    {
                        if ((raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8)) &&
                            (rasExt->lockedMipLevel == 0))
                        {
                            rasExt->lockedMipLevel = (RwUInt8)-1;

                            RwTextureRasterGenerateMipmaps(topRaster, NULL);
                        }
                    }
                }
                else
                {
                    IDirect3DSurface9_Release(rasExt->lockedSurface);

                    raster->privateFlags = (RwUInt8)(raster->privateFlags & ~rwRASTERPIXELLOCKED);
                }
            }
            break;

        case rwRASTERTYPEZBUFFER:
        default:
            RWERROR((E_RW_INVRASTERUNLOCKREQ));
            RWRETURN(FALSE);
            break;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterLockPalette

 On entry   : Pixel pointer
            : raster
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterLockPalette(void *paletteInOut,
                       void *rasterIn,
                       RwInt32 accessMode)
{
    RwRaster    *raster = (RwRaster*)rasterIn;

    RWFUNCTION(RWSTRING("_rwD3D9RasterLockPalette"));

    switch (raster->cType & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            {
                PALETTEENTRY        *palette;
                _rwD3D9RasterExt  *rasExt;

                rasExt = RASTEREXTFROMRASTER(raster);

                palette = rasExt->palette->entries;

                raster->palette = (RwUInt8 *)palette;

                if ((accessMode & rwRASTERLOCKWRITE)!=0)
                {
                    raster->privateFlags = (RwUInt8)(raster->privateFlags | rwRASTERPALETTELOCKED);
                }
                else
                {
                    raster->privateFlags = (RwUInt8)(raster->privateFlags | rwRASTERPALETTELOCKEDREAD);
                }

                *((RwUInt8 **)paletteInOut) = (RwUInt8 *)palette;
            }
            break;

        case rwRASTERTYPECAMERA:
        case rwRASTERTYPEZBUFFER:
        default:
            RWERROR((E_RW_INVRASTERLOCKREQ));
            RWRETURN(FALSE);
            break;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterUnlockPalette

 On entry   : NULL
            : Raster
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterUnlockPalette(void *unused1 __RWUNUSED__,
                         void *rasterIn,
                         RwInt32 unused3 __RWUNUSED__)
{
    RwRaster    *raster = (RwRaster*)rasterIn;

    RWFUNCTION(RWSTRING("_rwD3D9RasterUnlockPalette"));

    switch (raster->cType & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            {
                if ((raster->privateFlags & rwRASTERPALETTELOCKEDWRITE)!=0)
                {
                    _rwD3D9RasterExt  *rasExt;

                    rasExt = RASTEREXTFROMRASTER(raster);

                    DXCHECK(IDirect3DDevice9_SetPaletteEntries(_RwD3DDevice, rasExt->palette->globalindex, (PALETTEENTRY *)(rasExt->palette->entries)));
                }

                raster->palette = NULL;

                raster->privateFlags = (RwUInt8)(raster->privateFlags & ~rwRASTERPALETTELOCKED);
            }
            break;

        case rwRASTERTYPECAMERA:
        case rwRASTERTYPEZBUFFER:
        default:
            RWERROR((E_RW_INVRASTERUNLOCKREQ));
            RWRETURN(FALSE);
            break;
    }

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                              Drawing sprites

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 The following functions perfrom raster operations on the current raster set
 from RwRasterPushContext.
*/

/****************************************************************************
 rwD3D9RasterRenderQuad

 Purpose    : Renders a quad into the current raster target.

 On entry   : Source raster
            : Rect (where image is to be written)
            : scaled (does the src raster need to be scaled 'RwRasterRenderScaled')
            : alpha (does the src raster need to preserve alpha 'RwRasterRenderFast')
 On exit    :
 */
static void
rwD3D9RasterRenderQuad(RwRaster *raster, RwRect *rect,
                      RwBool scaled, RwBool alpha)
{
    D3DVIEWPORT9            viewport;
    RwBool                  setrendertargetdone = FALSE;
    RwRaster                *topRaster;
    const _rwD3D9RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("rwD3D9RasterRenderQuad"));

    /* Get the top level raster as this is the only one with a valid texture */
    topRaster = raster;
    while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

    rasExt = RASTEREXTFROMCONSTRASTER(topRaster);

    /* Set right render target */
    if (_RwD3D9CurrentRasterTarget->cType != rwRASTERTYPECAMERA)
    {
        setrendertargetdone = RwD3D9SetRenderTarget(0, _RwD3D9CurrentRasterTarget);
    }

    if (setrendertargetdone == FALSE)
    {
        _rwD3D9SetRenderTarget(0, _RwD3D9RenderSurface);
    }

    _rwD3D9SetDepthStencilSurface(NULL);

    /*
     * Set the viewport
     */
    viewport.X = _RwD3D9CurrentRasterTarget->nOffsetX;
    viewport.Y = _RwD3D9CurrentRasterTarget->nOffsetY;
    viewport.Width = _RwD3D9CurrentRasterTarget->width;
    viewport.Height = _RwD3D9CurrentRasterTarget->height;
    viewport.MinZ = MINZBUFFERVALUE;
    viewport.MaxZ = MAXZBUFFERVALUE;

    DXCHECK(IDirect3DDevice9_SetViewport(_RwD3DDevice, &viewport));

    /*
     * Begin a scene
     */
    if (_rwD3D9BeginScene())
    {
        RwBool                  vertexAlpha;
        RwBlendFunction         srcBlend, dstBlend;
        RwTextureFilterMode     filterMode;
        RwTextureAddressMode    addressModeU;
        RwTextureAddressMode    addressModeV;
        RwBool                  fogEnable;
        RwRaster                *curRaster;
        RwUInt32                oldZEnable;
        RwUInt32                oldCullMode;

        /*
         * Calculate the rectangle to render into
         */
        if (scaled)
        {
            _RwD3D9QuadVerts[0].x = (RwReal)(rect->x);
            _RwD3D9QuadVerts[0].y = (RwReal)(rect->y);

            _RwD3D9QuadVerts[1].x = (RwReal)(rect->x);
            _RwD3D9QuadVerts[1].y = (RwReal)(rect->y + rect->h);

            _RwD3D9QuadVerts[2].x = (RwReal)(rect->x + rect->w);
            _RwD3D9QuadVerts[2].y = (RwReal)(rect->y);

            _RwD3D9QuadVerts[3].x = (RwReal)(rect->x + rect->w);
            _RwD3D9QuadVerts[3].y = (RwReal)(rect->y + rect->h);
        }
        else
        {
            _RwD3D9QuadVerts[0].x = (RwReal)(rect->x);
            _RwD3D9QuadVerts[0].y = (RwReal)(rect->y);

            _RwD3D9QuadVerts[1].x = (RwReal)(rect->x);
            _RwD3D9QuadVerts[1].y = (RwReal)(rect->y + raster->height);

            _RwD3D9QuadVerts[2].x = (RwReal)(rect->x + raster->width);
            _RwD3D9QuadVerts[2].y = (RwReal)(rect->y);

            _RwD3D9QuadVerts[3].x = (RwReal)(rect->x + raster->width);
            _RwD3D9QuadVerts[3].y = (RwReal)(rect->y + raster->height);
        }

        /* Calc UVs for sub rasters */
        _RwD3D9QuadVerts[0].z = 0.f;
        _RwD3D9QuadVerts[0].rhw = 1.f;
        _RwD3D9QuadVerts[0].emissiveColor = 0xFFFFFFFF;
        _RwD3D9QuadVerts[0].u = (RwReal)(raster->nOffsetX) / (RwReal)(topRaster->width);
        _RwD3D9QuadVerts[0].v = (RwReal)(raster->nOffsetY) / (RwReal)(topRaster->height);

        _RwD3D9QuadVerts[1].z = 0.f;
        _RwD3D9QuadVerts[1].rhw = 1.f;
        _RwD3D9QuadVerts[1].emissiveColor = 0xFFFFFFFF;
        _RwD3D9QuadVerts[1].u = (RwReal)(raster->nOffsetX) / (RwReal)(topRaster->width);
        _RwD3D9QuadVerts[1].v = (RwReal)(raster->nOffsetY + raster->height) / (RwReal)(topRaster->height);

        _RwD3D9QuadVerts[2].z = 0.f;
        _RwD3D9QuadVerts[2].rhw = 1.f;
        _RwD3D9QuadVerts[2].emissiveColor = 0xFFFFFFFF;
        _RwD3D9QuadVerts[2].u = (RwReal)(raster->nOffsetX + raster->width) / (RwReal)(topRaster->width);
        _RwD3D9QuadVerts[2].v = (RwReal)(raster->nOffsetY) / (RwReal)(topRaster->height);

        _RwD3D9QuadVerts[3].z = 0.f;
        _RwD3D9QuadVerts[3].rhw = 1.f;
        _RwD3D9QuadVerts[3].emissiveColor = 0xFFFFFFFF;
        _RwD3D9QuadVerts[3].u = (RwReal)(raster->nOffsetX + raster->width) / (RwReal)(topRaster->width);
        _RwD3D9QuadVerts[3].v = (RwReal)(raster->nOffsetY + raster->height) / (RwReal)(topRaster->height);

        /* Get some render state */
        _rwD3D9RWGetRenderState(rwRENDERSTATESRCBLEND, (void *)&srcBlend);
        _rwD3D9RWGetRenderState(rwRENDERSTATEDESTBLEND, (void *)&dstBlend);
        _rwD3D9RWGetRenderState(rwRENDERSTATEVERTEXALPHAENABLE, (void *)&vertexAlpha);
        _rwD3D9RWGetRenderState(rwRENDERSTATEFOGENABLE, (void *)&fogEnable);
        _rwD3D9RWGetRenderState(rwRENDERSTATETEXTUREFILTER, (void *)&filterMode);
        _rwD3D9RWGetRenderState(rwRENDERSTATETEXTUREADDRESSU, (void *)&addressModeU);
        _rwD3D9RWGetRenderState(rwRENDERSTATETEXTUREADDRESSV, (void *)&addressModeV);
        _rwD3D9RWGetRenderState(rwRENDERSTATETEXTURERASTER, (void *)&curRaster);

        RwD3D9GetRenderState(D3DRS_ZENABLE, &oldZEnable);
        RwD3D9GetRenderState(D3DRS_CULLMODE, &oldCullMode);

        _rwD3D9RWSetRasterStage(topRaster, 0);

        if (alpha && rasExt->alpha)
        {
            _rwD3D9RenderStateVertexAlphaEnable(TRUE);
            _rwD3D9RenderStateSrcBlend(rwBLENDSRCALPHA);
            _rwD3D9RenderStateDestBlend(rwBLENDINVSRCALPHA);
        }
        else
        {
            _rwD3D9RenderStateVertexAlphaEnable(FALSE);
        }

        _rwD3D9RWSetRenderState(rwRENDERSTATEFOGENABLE, (void *)FALSE);

        _rwD3D9RWSetRenderState(rwRENDERSTATETEXTUREFILTER,
            (void *)(scaled ? rwFILTERLINEAR : rwFILTERNEAREST));

        _rwD3D9RWSetRenderState(rwRENDERSTATETEXTUREADDRESS, (void *)rwTEXTUREADDRESSCLAMP);

        RwD3D9SetRenderState(D3DRS_ZENABLE, FALSE);
        RwD3D9SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

        /*
         * Render the quad
         */
        _rwD3D9Im2DRenderPrimitive(rwPRIMTYPETRISTRIP,
                                   (RwIm2DVertex *)_RwD3D9QuadVerts,
                                   4);

        /* Restore render state */
        RwD3D9SetRenderState(D3DRS_ZENABLE, oldZEnable);
        RwD3D9SetRenderState(D3DRS_CULLMODE, oldCullMode);

        _rwD3D9RWSetRenderState(rwRENDERSTATESRCBLEND, (void *)srcBlend);
        _rwD3D9RWSetRenderState(rwRENDERSTATEDESTBLEND, (void *)dstBlend);
        _rwD3D9RWSetRenderState(rwRENDERSTATEVERTEXALPHAENABLE, (void *)vertexAlpha);
        _rwD3D9RWSetRenderState(rwRENDERSTATEFOGENABLE, (void *)fogEnable);
        _rwD3D9RWSetRenderState(rwRENDERSTATETEXTUREFILTER, (void *)filterMode);
        _rwD3D9RWSetRasterStage(curRaster, 0);
        _rwD3D9RWSetRenderState(rwRENDERSTATETEXTUREADDRESSU, (void *)addressModeU);
        _rwD3D9RWSetRenderState(rwRENDERSTATETEXTUREADDRESSV, (void *)addressModeV);
    }

    if (_RwD3D9CurrentRasterTarget->cType != rwRASTERTYPECAMERA)
    {
        /* If the set render target fails, we need to copy from the back buffer */
        if (setrendertargetdone == FALSE)
        {
            LPSURFACE   surfaceLevel;
            RECT rectSrc;
            RECT rectDest;
            D3DTEXTUREFILTERTYPE filter;

            /* Get the surface level */
            if (rasExt->cube)
            {
                IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)rasExt->texture,
                                                        (D3DCUBEMAP_FACES)rasExt->face,
                                                        0,
                                                        &surfaceLevel);
            }
            else
            {
                IDirect3DTexture9_GetSurfaceLevel(rasExt->texture, 0, &surfaceLevel);
            }

            rectSrc.left = _RwD3D9CurrentRasterTarget->nOffsetX;
            rectSrc.top = _RwD3D9CurrentRasterTarget->nOffsetY;
            rectSrc.right = rectSrc.left + _RwD3D9CurrentRasterTarget->width;
            rectSrc.bottom = rectSrc.top + _RwD3D9CurrentRasterTarget->height;

            rectDest.left = raster->nOffsetX;
            rectDest.top = raster->nOffsetY;
            rectDest.right = rectDest.left + raster->width;
            rectDest.bottom = rectDest.top + raster->height;

            filter = D3DTEXF_NONE;

            if (scaled)
            {
                if (_RwD3D9DeviceCaps.StretchRectFilterCaps & (D3DPTFILTERCAPS_MINFLINEAR | D3DPTFILTERCAPS_MAGFLINEAR))
                {
                    filter = D3DTEXF_LINEAR;
                }
            }
            else
            {
                if (_RwD3D9DeviceCaps.StretchRectFilterCaps & (D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MAGFPOINT))
                {
                    filter = D3DTEXF_POINT;
                }
            }

            DXCHECK(IDirect3DDevice9_StretchRect(_RwD3DDevice,
                                                 _RwD3D9RenderSurface,
                                                 &rectSrc,
                                                 surfaceLevel,
                                                 &rectDest,
                                                 filter));

            IDirect3DSurface9_Release(surfaceLevel);
        }
        else
        {
            /* Restore the render & depth/stencil surface */
            _rwD3D9SetRenderTarget(0, _RwD3D9RenderSurface);
            _rwD3D9SetDepthStencilSurface(_RwD3D9DepthStencilSurface);
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D9RasterCopyQuad

 Purpose    : Renders a quad into the current raster target.

 On entry   : Source raster
            : Rect (where image is to be written)
            : scaled (does the src raster need to be scaled 'RwRasterRenderScaled')
 On exit    : TRUE on success
 */
static RwBool
rwD3D9RasterCopyQuad(RwRaster *raster, RwRect *rect,
                      RwBool scaled)
{
    RwRaster            *topRasterSrc;
    RwRaster            *topRasterDest;
    const _rwD3D9RasterExt *rasExtSrc;
    const _rwD3D9RasterExt *rasExtDest;
    LPSURFACE           surfaceSrc;
    LPSURFACE           surfaceDest;
    HRESULT             hr;
    RECT                rectSrc;
    RECT                rectDest;

    RWFUNCTION(RWSTRING("rwD3D9RasterCopyQuad"));

    /* Get the top level raster as this is the only one with a valid texture */
    topRasterSrc = raster;
    while (RwRasterGetParent(topRasterSrc) != topRasterSrc)
    {
        topRasterSrc = RwRasterGetParent(topRasterSrc);
    };

    rasExtSrc = RASTEREXTFROMCONSTRASTER(topRasterSrc);

    if (topRasterSrc->cType != rwRASTERTYPECAMERA)
    {
        if (rasExtSrc->cube)
        {
            IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)rasExtSrc->texture,
                                                    (D3DCUBEMAP_FACES)rasExtSrc->face,
                                                    0,
                                                    &surfaceSrc);
        }
        else
        {
            IDirect3DTexture9_GetSurfaceLevel(rasExtSrc->texture, 0, &surfaceSrc);
        }
    }
    else
    {
        if (rasExtSrc->swapChain)
        {
            surfaceSrc = (LPSURFACE)rasExtSrc->texture;
        }
        else
        {
            surfaceSrc = _RwD3D9RenderSurface;
        }
    }

    /* Get the top level raster as this is the only one with a valid texture */
    topRasterDest = _RwD3D9CurrentRasterTarget;
    while (RwRasterGetParent(topRasterDest) != topRasterDest)
    {
        topRasterDest = RwRasterGetParent(topRasterDest);
    };

    rasExtDest = RASTEREXTFROMCONSTRASTER(topRasterDest);

    if (topRasterDest->cType != rwRASTERTYPECAMERA)
    {
        if (rasExtDest->cube)
        {
            IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)rasExtDest->texture,
                                                    (D3DCUBEMAP_FACES)rasExtDest->face,
                                                    0,
                                                    &surfaceDest);
        }
        else
        {
            IDirect3DTexture9_GetSurfaceLevel(rasExtDest->texture, 0, &surfaceDest);
        }
    }
    else
    {
        if (rasExtDest->swapChain)
        {
            surfaceDest = (LPSURFACE)rasExtDest->texture;
        }
        else
        {
            surfaceDest = _RwD3D9RenderSurface;
        }
    }

    rectSrc.left = raster->nOffsetX;
    rectSrc.top = raster->nOffsetY;
    rectSrc.right = rectSrc.left + raster->width;
    rectSrc.bottom = rectSrc.top + raster->height;

    if(scaled)
    {
        rectDest.left = _RwD3D9CurrentRasterTarget->nOffsetX + rect->x;
        rectDest.top = _RwD3D9CurrentRasterTarget->nOffsetY + rect->y;
        rectDest.right = rectDest.left + rect->w;
        rectDest.bottom = rectDest.top + rect->h;
    }
    else
    {
        rectDest.left = _RwD3D9CurrentRasterTarget->nOffsetX + rect->x;
        rectDest.top = _RwD3D9CurrentRasterTarget->nOffsetY + rect->y;
        rectDest.right = rectDest.left + raster->width;
        rectDest.bottom = rectDest.top + raster->height;
    }

    /* Check if destination is video memory */
    if (topRasterDest->cType == rwRASTERTYPECAMERA ||
        topRasterDest->cType == rwRASTERTYPECAMERATEXTURE)
    {
        /* Check if source is video memory */
        if (topRasterSrc->cType == rwRASTERTYPECAMERA ||
            topRasterSrc->cType == rwRASTERTYPECAMERATEXTURE)
        {
            D3DTEXTUREFILTERTYPE filter;

            filter = D3DTEXF_NONE;

            if (scaled)
            {
                if (_RwD3D9DeviceCaps.StretchRectFilterCaps & (D3DPTFILTERCAPS_MINFLINEAR | D3DPTFILTERCAPS_MAGFLINEAR))
                {
                    filter = D3DTEXF_LINEAR;
                }
            }
            else
            {
                if (_RwD3D9DeviceCaps.StretchRectFilterCaps & (D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MAGFPOINT))
                {
                    filter = D3DTEXF_POINT;
                }
            }

            hr =
            DXCHECK(IDirect3DDevice9_StretchRect(_RwD3DDevice,
                                                 surfaceSrc,
                                                 &rectSrc,
                                                 surfaceDest,
                                                 &rectDest,
                                                 filter));
        }
        else
        {
            POINT pt;

            pt.x = raster->nOffsetX;
            pt.y = raster->nOffsetY;

            hr =
            DXCHECK(IDirect3DDevice9_UpdateSurface(_RwD3DDevice,
                                                    surfaceSrc,
                                                    &rectSrc,
                                                    surfaceDest,
                                                    &pt));
        }
    }
    else
    {
        /* Check if source is video memory */
        if (topRasterSrc->cType == rwRASTERTYPECAMERA ||
            topRasterSrc->cType == rwRASTERTYPECAMERATEXTURE)
        {
            hr =
            DXCHECK(IDirect3DDevice9_GetRenderTargetData(_RwD3DDevice,
                                                         surfaceSrc,
                                                         surfaceDest));
        }
        else
        {
            POINT pt;

            pt.x = raster->nOffsetX;
            pt.y = raster->nOffsetY;

            hr =
            DXCHECK(IDirect3DDevice9_UpdateSurface(_RwD3DDevice,
                                                    surfaceSrc,
                                                    &rectSrc,
                                                    surfaceDest,
                                                    &pt));
        }
    }

    if (topRasterDest->cType != rwRASTERTYPECAMERA)
    {
        IDirect3DSurface9_Release(surfaceDest);
    }

    if (topRasterSrc->cType != rwRASTERTYPECAMERA)
    {
        IDirect3DSurface9_Release(surfaceSrc);
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 rwD3D9RasterRenderGeneric

 Purpose    : Renders a raster including alpha.
              No Z buffering take place only a simple blit operation.

 On entry   : Source raster
            : Rect (where image is to be written)
            : Flags
 On exit    :
 */
static RwBool
rwD3D9RasterRenderGeneric(void *rasterIn, void *rectIn,
                         RwBool scaled, RwBool alpha)
{
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwRect      *rect = (RwRect *)rectIn;

    RWFUNCTION(RWSTRING("rwD3D9RasterRenderGeneric"));

    if(!raster)
    {
        RWRETURN(FALSE);
    }

    switch (_RwD3D9CurrentRasterTarget->cType)
    {
        /* DST Raster */
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:

        switch (raster->cType & rwRASTERTYPEMASK)
        {
            /* SRC Raster */
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
        case rwRASTERTYPECAMERA:
            if (rwD3D9RasterCopyQuad(raster, rect, scaled) == FALSE)
            {
                RWERROR((E_RW_DEVICEERROR,
                    RWSTRING("SRC, DST Raster render combination not supported")));
                RWRETURN(FALSE);
            }
            break;
        case rwRASTERTYPEZBUFFER:
            RWERROR((E_RW_DEVICEERROR,
                RWSTRING("SRC, DST Raster render combination not supported")));
            RWRETURN(FALSE);
            break;
        }
        break;

    case rwRASTERTYPECAMERATEXTURE:
    case rwRASTERTYPECAMERA:
        switch (raster->cType & rwRASTERTYPEMASK)
        {
            /* SRC Raster */
        case rwRASTERTYPECAMERA:
            if (rwD3D9RasterCopyQuad(raster, rect, scaled) == FALSE)
            {
                RWERROR((E_RW_DEVICEERROR,
                    RWSTRING("SRC, DST Raster render combination not supported")));
                RWRETURN(FALSE);
            }
            break;

        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            rwD3D9RasterRenderQuad(raster, rect, scaled, alpha);
            break;

        case rwRASTERTYPEZBUFFER:
            RWERROR((E_RW_DEVICEERROR,
                RWSTRING("SRC, DST Raster render combination not supported")));
            RWRETURN(FALSE);
            break;
        }
        break;

    case rwRASTERTYPEZBUFFER:
    default:
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("SRC, DST Raster render combination not supported")));
        RWRETURN(FALSE);
        break;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterRender

 Purpose    : Renders a raster including alpha.
              No Z buffering take place only a simple blit operation.

 On entry   : Source raster
            : Rect (where image is to be written)
            : Flags
 On exit    :
 */
RwBool
_rwD3D9RasterRender(void *rasterIn,
                  void *rectIn,
                  RwInt32 unused3 __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwD3D9RasterRender"));

    RWRETURN(rwD3D9RasterRenderGeneric((RwRaster *)rasterIn,
                                      (RwRect *)rectIn,
                                      FALSE, TRUE));
}

/****************************************************************************
 _rwD3D9RasterRenderFast

 Purpose    : Renders a raster, ignoring any alpha.
              No Z buffering take place only a simple blit operation.

 On entry   : Raster
            : Rect
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterRenderFast(void *rasterIn,
                      void *rectIn,
                      RwInt32 flags __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwD3D9RasterRenderFast"));

    RWRETURN(rwD3D9RasterRenderGeneric((RwRaster *)rasterIn,
                                      (RwRect *)rectIn,
                                      FALSE, FALSE));
}

/****************************************************************************
 _rwD3D9RasterRenderScaled

 Purpose    : Renders a raster excluding alpha.
              No Z buffering take place only a simple blit operation.

 On entry   : Source raster
            : Rect (where image is to be written)
 On exit    :
 */
RwBool
_rwD3D9RasterRenderScaled(void *rasterIn,
                        void *rectIn,
                        RwInt32 flags __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwD3D9RasterRenderScaled"));

    RWRETURN(rwD3D9RasterRenderGeneric((RwRaster *)rasterIn,
                                      (RwRect *)rectIn,
                                      TRUE, TRUE));
}

/****************************************************************************
 D3D9RasterClearGeneric

 Clear a raster to specific value

 On entry   : rectIn - The rectangle to clear
            : packedColor - Value to clear to
 On exit    : TRUE on success
 */
static RwBool
D3D9RasterClearGeneric(RwRect *rect, RwInt32 packedColor)
{
    RWFUNCTION(RWSTRING("D3D9RasterClearGeneric"));

    switch (_RwD3D9CurrentRasterTarget->cType)
    {
        case rwRASTERTYPECAMERA:
            {
                RwRGBA      unpackedColor;
                RwUInt32    packedColor8888;
                D3DRECT     rectClear;

                if (_RwD3D9CurrentRasterTarget->depth != 32)
                {
                    _rwD3D9PixelToRGB(&unpackedColor, &packedColor, RwRasterGetFormat(_RwD3D9CurrentRasterTarget));
                    _rwD3D9RGBToPixel(&packedColor8888, &unpackedColor, rwRASTERFORMAT8888);
                }
                else
                {
                    packedColor8888 = packedColor;
                }

                _rwD3D9SetRenderTarget(0, _RwD3D9RenderSurface);
                _rwD3D9SetDepthStencilSurface(_RwD3D9DepthStencilSurface);

                /* Initialize the rectangle to clear */
                rectClear.x1 = _RwD3D9CurrentRasterTarget->nOffsetX + rect->x;
                rectClear.y1 = _RwD3D9CurrentRasterTarget->nOffsetY + rect->y;
                rectClear.x2 = rectClear.x1 + rect->w;
                rectClear.y2 = rectClear.y1 + rect->h;

                /* Clear */
                DXCHECK(IDirect3DDevice9_Clear(_RwD3DDevice, 1, &rectClear, D3DCLEAR_TARGET,
                    packedColor8888, MAXZBUFFERVALUE, 0));
            }
            break;

        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPENORMAL:
            {
                RwRaster                *topRaster;
                const _rwD3D9RasterExt  *rasterExt;
                D3DSURFACE_DESC         desc;

                /* Get the top level raster as this is the only one with a valid texture */
                topRaster = RwRasterGetParent(_RwD3D9CurrentRasterTarget);

                /* Get the surface level */
                rasterExt = RASTEREXTFROMCONSTRASTER(topRaster);

                RWASSERT(rasterExt->texture != NULL);

                if (rasterExt->cube)
                {
                    IDirect3DCubeTexture9_GetLevelDesc((LPDIRECT3DCUBETEXTURE9)rasterExt->texture,
                                                       0, &desc);
                }
                else
                {
                    IDirect3DTexture9_GetLevelDesc(rasterExt->texture, 0, &desc);
                }

                if (desc.Pool == D3DPOOL_DEFAULT)
                {
                    RwRGBA          unpackedColor;
                    RwUInt32        packedColor8888;
                    RECT            rectClear;
                    LPSURFACE       surfaceLevel;

                    _rwD3D9PixelToRGB(&unpackedColor, &packedColor, RwRasterGetFormat(_RwD3D9CurrentRasterTarget));
                    _rwD3D9RGBToPixel(&packedColor8888, &unpackedColor, rwRASTERFORMAT8888);

                    if (rasterExt->cube)
                    {
                        IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)rasterExt->texture,
                                                                (D3DCUBEMAP_FACES)rasterExt->face,
                                                                0,
                                                                &surfaceLevel);
                    }
                    else
                    {
                        IDirect3DTexture9_GetSurfaceLevel(rasterExt->texture, 0, &surfaceLevel);
                    }

                    /* Initialize the rectangle to clear */
                    rectClear.left = _RwD3D9CurrentRasterTarget->nOffsetX + rect->x;
                    rectClear.top = _RwD3D9CurrentRasterTarget->nOffsetY + rect->y;
                    rectClear.right = rectClear.left + rect->w;
                    rectClear.bottom = rectClear.top + rect->h;

                    IDirect3DDevice9_ColorFill(_RwD3DDevice, surfaceLevel, &rectClear, packedColor8888);

                    IDirect3DSurface9_Release(surfaceLevel);
                }
                else
                {
                    RwBool      rv = FALSE;
                    RwUInt8     *pixels;
                    RwInt32     x, y;

                    if (_rwD3D9RasterLock(&pixels, _RwD3D9CurrentRasterTarget, rwRASTERLOCKWRITE))
                    {
                        if (_RwD3D9CurrentRasterTarget->depth == 32)
                        {
                            for (y = 0; y < rect->h; y++)
                            {
                                RwUInt32 *pixelDest = (RwUInt32 *)(pixels + (rect->x * 4) + ((y + rect->y) * _RwD3D9CurrentRasterTarget->stride));
                                for (x = 0; x < rect->w; x++)
                                {
                                    *pixelDest++ = packedColor;
                                }
                            }

                            rv = TRUE;
                        }
                        else if (_RwD3D9CurrentRasterTarget->depth == 16)
                        {
                            for (y = 0; y < rect->h; y++)
                            {
                                RwUInt16 *pixelDest = (RwUInt16 *)(pixels + (rect->x * 2) + ((y + rect->y) * _RwD3D9CurrentRasterTarget->stride));
                                for (x = 0; x < rect->w; x++)
                                {
                                    *pixelDest++ = (RwUInt16)packedColor;
                                }
                            }

                            rv = TRUE;
                        }
                        else if (_RwD3D9CurrentRasterTarget->depth == 8)
                        {
                            RwRaster            *topRaster;
                            const _rwD3D9RasterExt    *rasExt;
                            PALETTEENTRY        *palette;
                            RwUInt8             indice;
                            RwUInt32            n, minDist;
                            RwUInt8             *colorComps;

                            /* Get the top level raster as this is the only one with a valid palette */
                            topRaster = _RwD3D9CurrentRasterTarget;
                            while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

                            rasExt = RASTEREXTFROMCONSTRASTER(topRaster);

                            palette = rasExt->palette->entries;

                            /* Find the closest mach in the palette */
                            indice = 0;
                            minDist = 0xffffffff;
                            colorComps = (RwUInt8 *)(&packedColor);
                            for (n=0; n < 256; n++)
                            {
                                const RwUInt8   *currColor = (const RwUInt8 *)(palette + n);
                                const RwInt32   currDist0 = (currColor[0] - colorComps[0]);
                                const RwInt32   currDist1 = (currColor[1] - colorComps[1]);
                                const RwInt32   currDist2 = (currColor[2] - colorComps[2]);
                                const RwInt32   currDist3 = (currColor[3] - colorComps[3]);
                                const RwUInt32  currDist = (currDist0 * currDist0 +
                                                            currDist1 * currDist1 +
                                                            currDist2 * currDist2 +
                                                            currDist3 * currDist3);

                                if (currDist < minDist)
                                {
                                    indice = (RwUInt8)n;
                                    minDist = currDist;

                                    if (minDist == 0)
                                    {
                                        break;
                                    }
                                }
                            }

                            /* Fill with the correct index */
                            for (y = 0; y < rect->h; y++)
                            {
                                RwUInt8 *pixelDest = (pixels + (rect->x) + ((y + rect->y) * _RwD3D9CurrentRasterTarget->stride));
                                for (x = 0; x < rect->w; x++)
                                {
                                    *pixelDest++ = indice;
                                }
                            }

                            rv = TRUE;
                        }

                        _rwD3D9RasterUnlock(pixels, _RwD3D9CurrentRasterTarget, rwRASTERLOCKWRITE);
                    }

                    if (!rv)
                    {
                        RWERROR((E_RW_INVRASTERFORMAT));
                        RWRETURN(FALSE);
                    }
                }
            }
            break;

        case rwRASTERTYPECAMERATEXTURE:
            {
                RwUInt32        packedColor8888;
                RECT            rectClear;
                RwRaster        *topRaster;
                const _rwD3D9RasterExt  *rasterExt;
                LPSURFACE       surfaceLevel;

                if (_RwD3D9CurrentRasterTarget->depth != 32)
                {
                    RwRGBA          unpackedColor;

                    _rwD3D9PixelToRGB(&unpackedColor, &packedColor, RwRasterGetFormat(_RwD3D9CurrentRasterTarget));
                    _rwD3D9RGBToPixel(&packedColor8888, &unpackedColor, rwRASTERFORMAT8888);
                }
                else
                {
                    packedColor8888 = packedColor;
                }

                /* Get the top level raster as this is the only one with a valid texture */
                topRaster = _RwD3D9CurrentRasterTarget;
                while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

                /* Get the surface level */
                rasterExt = RASTEREXTFROMCONSTRASTER(topRaster);

                if (rasterExt->cube)
                {
                    IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)rasterExt->texture,
                                                            (D3DCUBEMAP_FACES)rasterExt->face,
                                                            0,
                                                            &surfaceLevel);
                }
                else
                {
                    IDirect3DTexture9_GetSurfaceLevel(rasterExt->texture, 0, &surfaceLevel);
                }

                /* Initialize the rectangle to clear */
                rectClear.left = _RwD3D9CurrentRasterTarget->nOffsetX + rect->x;
                rectClear.top = _RwD3D9CurrentRasterTarget->nOffsetY + rect->y;
                rectClear.right = rectClear.left + rect->w;
                rectClear.bottom = rectClear.top + rect->h;

                IDirect3DDevice9_ColorFill(_RwD3DDevice, surfaceLevel, &rectClear, packedColor8888);

                IDirect3DSurface9_Release(surfaceLevel);
            }
            break;

        case rwRASTERTYPEZBUFFER:
            {
                const _rwD3D9RasterExt  *rasterExt;
                RwRaster        *topRaster;
                LPSURFACE       surfaceLevel;
                D3DRECT         rectClear;

                /* Get the top level raster as this is the only one with a valid texture */
                topRaster = _RwD3D9CurrentRasterTarget;
                while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

                /* Get the surface level */
                rasterExt = RASTEREXTFROMCONSTRASTER(topRaster);
                surfaceLevel = (LPSURFACE)rasterExt->texture;

                _rwD3D9SetDepthStencilSurface(surfaceLevel);

                /* Initialize the rectangle to clear */
                rectClear.x1 = _RwD3D9CurrentRasterTarget->nOffsetX + rect->x;
                rectClear.y1 = _RwD3D9CurrentRasterTarget->nOffsetY + rect->y;
                rectClear.x2 = rectClear.x1 + rect->w;
                rectClear.y2 = rectClear.y1 + rect->h;

                /* Clear */
                DXCHECK(IDirect3DDevice9_Clear(_RwD3DDevice, 1, &rectClear, D3DCLEAR_ZBUFFER, 0, MAXZBUFFERVALUE, 0));

                /* Restore the render & depth/stencil surface */
                _rwD3D9SetDepthStencilSurface(_RwD3D9DepthStencilSurface);
            }
            break;

        default:
            RWERROR((E_RW_INVRASTERFORMAT));
            RWRETURN(FALSE);
            break;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterClearRect

 Clear specified area of raster

 On entry   : NULL
              pRect - region of raster to clear
              nValue - value to clear to
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterClearRect(void *unused1 __RWUNUSED__,
                     void *rectIn,
                     RwInt32 packedColor)
{
    RwRect  *rect = (RwRect *)rectIn;

    RWFUNCTION(RWSTRING("_rwD3D9RasterClearRect"));

    RWRETURN(D3D9RasterClearGeneric(rect, packedColor));
}

/****************************************************************************
 _rwD3D9RasterClear

 Clear a raster to specific value

 On entry   : NULL
            : NULL
            : packedColor - value to clear to
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterClear(void *unused1 __RWUNUSED__,
                 void *unused2 __RWUNUSED__,
                 RwInt32 packedColor)
{
    RwRect  rect;

    RWFUNCTION(RWSTRING("_rwD3D9RasterClear"));

    rect.x = 0;
    rect.y = 0;
    rect.w = _RwD3D9CurrentRasterTarget->width;
    rect.h = _RwD3D9CurrentRasterTarget->height;

    RWRETURN(D3D9RasterClearGeneric(&rect, packedColor));
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           Setting the context

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwD3D9SetRasterContext(void *out, void *rasIn, RwInt32 flags)

 Purpose    : Set the current raster as the destination raster to which
              raster operations will be applied.

 On entry   : rasIn - raster to do all subsequent rendering to
 On exit    : TRUE on success
 */
RwBool
_rwD3D9SetRasterContext(void *unused1 __RWUNUSED__,
                      void *rasIn,
                      RwInt32 unused3 __RWUNUSED__)
{
    RwRaster *raster = (RwRaster *)rasIn;

    RWFUNCTION(RWSTRING("_rwD3D9SetRasterContext"));

    _RwD3D9CurrentRasterTarget = raster;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9FindCorrectRasterFormat

 Puprose    : Finds the rasters pixel format

 On entry   : flags -

 On exit    : correct pixel format
 */
RwInt32
_rwD3D9FindCorrectRasterFormat(RwRasterType type, RwInt32 flags)
{
    RwUInt32    format = flags & rwRASTERFORMATMASK;

    RWFUNCTION(RWSTRING("_rwD3D9FindCorrectRasterFormat"));

    switch (type)
    {
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            /* Check if we are requesting a default pixel format palette texture */
            if (format & rwRASTERFORMATPAL8)
            {
                if (_rwD3D9CheckValidTextureFormat(D3DFMT_P8))
                {
                    if (_RwD3D9DeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE)
                    {
                        format |= rwRASTERFORMAT8888;
                    }
                    else
                    {
                        format |= rwRASTERFORMAT888;
                    }
                }
                else
                {
                    format &= (~rwRASTERFORMATPAL8);
                }
            }

            if ( (format & rwRASTERFORMATPAL8) == 0 )
            {
                /*
                 * By default textures are 8888, but make mipmap generation flag and
                 * palletized flags persist
                 */
                if( _rwD3D9CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                {
                    format |= rwRASTERFORMAT8888;
                }
                else
                {
                    if( _rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                    {
                        format |= rwRASTERFORMAT4444;
                    }
                    else
                    {
                        format |= rwRASTERFORMAT1555;
                    }
                }
            }
        }
        else
        {
            /* No support for 4 bits palettes */
            if (format & rwRASTERFORMATPAL4)
            {
                /* Change it to a 8 bits palette */
                format &= (~rwRASTERFORMATPAL4);

                format |= rwRASTERFORMATPAL8;
            }

            if (format & rwRASTERFORMATPAL8)
            {
                if( !_rwD3D9CheckValidTextureFormat(D3DFMT_P8) ||
                    ( ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888) &&
                      ((_RwD3D9DeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE) == 0)) )
                {
                    /* Change it to a default format */
                    format &= (~rwRASTERFORMATPAL8);

                    if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888)
                    {
                        if (_RwD3D9AdapterInformation.displayDepth > 16)
                        {
                            if( _rwD3D9CheckValidTextureFormat(D3DFMT_X8R8G8B8) )
                            {
                                format |= rwRASTERFORMAT888;
                            }
                            else
                            {
                                format &= (~rwRASTERFORMAT888);

                                format |= rwRASTERFORMAT565;
                            }
                        }
                        else
                        {
                            format &= (~rwRASTERFORMAT888);

                            format |= rwRASTERFORMAT565;
                        }
                    }
                    else
                    {
                        if (_RwD3D9AdapterInformation.displayDepth > 16)
                        {
                            if( _rwD3D9CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                            {
                                format |= rwRASTERFORMAT8888;
                            }
                            else
                            {
                                format &= (~rwRASTERFORMAT8888);

                                if( _rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                                {
                                    format |= rwRASTERFORMAT4444;
                                }
                                else
                                {
                                    format |= rwRASTERFORMAT1555;
                                }
                            }
                        }
                        else
                        {
                            format &= (~rwRASTERFORMAT8888);

                            if( _rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                            {
                                format |= rwRASTERFORMAT4444;
                            }
                            else
                            {
                                format |= rwRASTERFORMAT1555;
                            }
                        }
                    }
                }
            }
            else
            {
                if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888)
                {
                    if(!_rwD3D9CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                    {
                        format &= (~rwRASTERFORMAT8888);

                        if( _rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                        {
                            format |= rwRASTERFORMAT4444;
                        }
                        else
                        {
                            format |= rwRASTERFORMAT1555;
                        }
                    }
                }
                else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888)
                {
                    if(!_rwD3D9CheckValidTextureFormat(D3DFMT_X8R8G8B8) )
                    {
                        format &= (~rwRASTERFORMAT888);

                        format |= rwRASTERFORMAT565;
                    }
                }
                else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT4444)
                {
                    if( !_rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                    {
                        format &= (~rwRASTERFORMAT4444);

                        format |= rwRASTERFORMAT1555;
                    }
                }
                else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATLUM8)
                {
                    if(!_rwD3D9CheckValidTextureFormat(D3DFMT_L8) )
                    {
                        format &= (~rwRASTERFORMATLUM8);

                        if( _rwD3D9CheckValidTextureFormat(D3DFMT_X8R8G8B8) )
                        {
                            format |= D3DFMT_X8R8G8B8;
                        }
                        else
                        {
                            format |= rwRASTERFORMAT565;
                        }
                    }
                }
            }
        }
        break;

    case rwRASTERTYPECAMERATEXTURE:
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            if (16 == _RwD3D9ZBufferDepth)
            {
                format |= rwRASTERFORMAT565;
            }
            else
            {
                if(_rwD3D9CheckValidCameraTextureFormat(D3DFMT_X8R8G8B8))
                {
                    format |= rwRASTERFORMAT888;
                }
                else
                {
                    format |= rwRASTERFORMAT565;
                }
            }
        }
        else
        {
            const RwInt32 depth = _rwRasterConvTable[FMT2TBL(format)].depth;

            if (16 == _RwD3D9ZBufferDepth)
            {
                if (depth == 32)
                {
                    if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888)
                    {
                        format &= (~rwRASTERFORMAT8888);

                        format |= rwRASTERFORMAT4444;
                    }
                    else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888)
                    {
                        format &= (~rwRASTERFORMAT888);

                        format |= rwRASTERFORMAT565;
                    }
                    else
                    {
                        format = rwRASTERFORMAT565;
                    }
                }

                if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT4444)
                {
                    if( !_rwD3D9CheckValidCameraTextureFormat(D3DFMT_A4R4G4B4) )
                    {
                        format &= (~rwRASTERFORMAT4444);

                        format |= rwRASTERFORMAT1555;
                    }
                }

                if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT1555)
                {
                    if( !_rwD3D9CheckValidCameraTextureFormat(D3DFMT_A1R5G5B5) )
                    {
                        format &= (~rwRASTERFORMAT1555);

                        format |= rwRASTERFORMAT565;
                    }
                }
            }
            else
            {
                if (depth == 16)
                {
                    if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT4444)
                    {
                        format &= (~rwRASTERFORMAT4444);

                        format |= rwRASTERFORMAT8888;
                    }
                    else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT1555)
                    {
                        format &= (~rwRASTERFORMAT1555);

                        format |= rwRASTERFORMAT8888;
                    }
                    else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT565)
                    {
                        format &= (~rwRASTERFORMAT565);

                        format |= rwRASTERFORMAT888;
                    }
                    else
                    {
                        format = rwRASTERFORMAT888;
                    }
                }

                if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888)
                {
                    if(!_rwD3D9CheckValidCameraTextureFormat(D3DFMT_A8R8G8B8))
                    {
                        format &= (~rwRASTERFORMAT8888);

                        format |= rwRASTERFORMAT888;
                    }
                }
            }
        }
        break;

    case rwRASTERTYPECAMERA:
        /* Always force default */
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            format = rwD3D9FindRwFormat(Present.BackBufferFormat);
        }
        else
        {
            RWERROR((E_RW_INVRASTERFORMAT));
        }
        break;

    case rwRASTERTYPEZBUFFER:
        /* Always force default */
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            if (16 == _RwD3D9ZBufferDepth)
            {
                format = rwRASTERFORMAT16;
            }
            else if (32 == _RwD3D9ZBufferDepth)
            {
                format = rwRASTERFORMAT32;
            }
        }
        else
        {
            RWERROR((E_RW_INVRASTERFORMAT));
        }
        break;

    default:
        RWERROR((E_RW_INVRASTERFORMAT));
        break;
    }

    RWRETURN(format);
}

/****************************************************************************
 _rwD3D9CheckRasterFormat

 Puprose    : Finds the rasters pixel format & depth.

 On entry   : raster - raster to allocate
            : flags -

 On exit    : TRUE on success
 */
RwBool
_rwD3D9CheckRasterFormat(void *rasterIn, RwInt32 flags)
{
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwUInt32    format = flags & rwRASTERFORMATMASK;

    RWFUNCTION(RWSTRING("_rwD3D9CheckRasterFormat"));

    /* Copy over types */
    raster->cType = (RwUInt8)(flags & rwRASTERTYPEMASK);
    raster->cFlags = (RwUInt8)(flags & ~rwRASTERTYPEMASK);

    format = (RwRasterType)
        _rwD3D9FindCorrectRasterFormat((RwRasterType)raster->cType,
                                       flags);

    raster->cFormat = (RwUInt8)(format >> 8);

    raster->depth = _rwRasterConvTable[FMT2TBL(format)].depth;

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9SetRasterFormat

 Puprose    : Sets the rasters pixel format & depth.

 On entry   : raster - raster to allocate
            : flags -

 On exit    : TRUE on success
 */
static RwBool
rwD3D9SetRasterFormat(void *rasterIn, RwInt32 flags)
{
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwUInt32    format = flags & rwRASTERFORMATMASK;

    RWFUNCTION(RWSTRING("rwD3D9SetRasterFormat"));

    /* Copy over types */
    raster->cType = (RwUInt8)(flags & rwRASTERTYPEMASK);
    raster->cFlags = (RwUInt8)(flags & ~rwRASTERTYPEMASK);

    switch (raster->cType & rwRASTERTYPEMASK)
    {
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            /* Check if we are requesting a default pixel format palette texture */
            if (format & rwRASTERFORMATPAL8)
            {
                if (_rwD3D9CheckValidTextureFormat(D3DFMT_P8))
                {
                    if (_RwD3D9DeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE)
                    {
                        format |= rwRASTERFORMAT8888;
                    }
                    else
                    {
                        format |= rwRASTERFORMAT888;
                    }

                    raster->depth = 8;
                }
                else
                {
                    RWRETURN(FALSE);
                }
            }
            else
            {
                /*
                 * By default textures are 8888, but make mipmap generation flag and
                 * palletized flags persist
                 */
                if (_RwD3D9AdapterInformation.displayDepth > 16)
                {
                    if( _rwD3D9CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                    {
                        format |= rwRASTERFORMAT8888;

                        raster->depth = 32;
                    }
                    else
                    {
                        if( _rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                        {
                            format |= rwRASTERFORMAT4444;
                        }
                        else
                        {
                            format |= rwRASTERFORMAT1555;
                        }

                        raster->depth = 16;
                    }
                }
                else
                {
                    if( _rwD3D9CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                    {
                        format |= rwRASTERFORMAT4444;
                    }
                    else
                    {
                        format |= rwRASTERFORMAT1555;
                    }

                    raster->depth = 16;
                }
            }
        }
        else
        {
            /* No support for 4 bits palettes */
            if (format & rwRASTERFORMATPAL4)
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
            else
            {
                if (format & rwRASTERFORMATPAL8)
                {
                    if( _rwD3D9CheckValidTextureFormat(D3DFMT_P8) &&
                        ( ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888) ||
                          (((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888) &&
                          (_RwD3D9DeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE) )) )
                    {
                        raster->depth = 8;
                    }
                    else
                    {
                        RWRETURN(FALSE);
                    }
                }
                else
                {
                    D3DFORMAT d3dFormat;

                    d3dFormat = _rwRasterConvTable[FMT2TBL(format)].format;

                    if (IS_D3DFORMAT_ZBUFFER(d3dFormat))
                    {
                        if (_rwD3D9CheckValidZBufferTextureFormat(d3dFormat))
                        {
                            raster->depth = _rwRasterConvTable[FMT2TBL(format)].depth;
                        }
                        else
                        {
                            RWRETURN(FALSE);
                        }
                    }
                    else
                    {
                        if( _rwD3D9CheckValidTextureFormat(d3dFormat) )
                        {
                            raster->depth = _rwRasterConvTable[FMT2TBL(format)].depth;
                        }
                        else
                        {
                            RWRETURN(FALSE);
                        }
                    }
                }
            }
        }

        /* check automipmapgen */
        if ((format & rwRASTERFORMATPAL8) == 0 &&
            (flags & (rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP)) == (rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP))
        {
            if (_rwD3D9CheckAutoMipmapGenTextureFormat(_rwRasterConvTable[FMT2TBL(format)].format))
            {
                _rwD3D9RasterExt    *rasExt = RASTEREXTFROMRASTER(raster);

                rasExt->automipmapgen = 1;
            }
        }
        break;

    case rwRASTERTYPECAMERATEXTURE:
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            /*
             * Raster should match back buffer bit depth
             */
            if (16 == _RwD3D9AdapterInformation.displayDepth)
            {
                format |= rwRASTERFORMAT565;

                raster->depth = 16;
            }
            else
            {
                if(_rwD3D9CheckValidCameraTextureFormat(D3DFMT_X8R8G8B8))
                {
                    format |= rwRASTERFORMAT888;

                    raster->depth = 32;
                }
                else
                {
                    format |= rwRASTERFORMAT565;

                    raster->depth = 16;
                }
            }
        }
        else
        {
            RwInt32 depth;

            depth = _rwRasterConvTable[FMT2TBL(format)].depth;

            if(_rwD3D9CheckValidCameraTextureFormat(_rwRasterConvTable[FMT2TBL(format)].format))
            {
                raster->depth = depth;
            }
            else
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
        }

        /* check automipmapgen */
        if ((flags & (rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP)) == (rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP))
        {
            if (_rwD3D9CheckAutoMipmapGenCameraTextureFormat(_rwRasterConvTable[FMT2TBL(format)].format))
            {
                _rwD3D9RasterExt    *rasExt = RASTEREXTFROMRASTER(raster);

                rasExt->automipmapgen = 1;
            }
        }
        break;

    case rwRASTERTYPECAMERA:
        /* Always force default */
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            format = rwD3D9FindRwFormat(Present.BackBufferFormat);
        }
        else
        {
            const RwUInt32 mainCameraFormat =
                     rwD3D9FindRwFormat(Present.BackBufferFormat);

            if (mainCameraFormat != format)
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
        }

        raster->depth = _RwD3D9AdapterInformation.displayDepth;
        break;

    case rwRASTERTYPEZBUFFER:
        /* Always force default */
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            if (16 == _RwD3D9ZBufferDepth)
            {
                raster->depth = 16;
                format = rwRASTERFORMAT16;
            }
            else if (32 == _RwD3D9ZBufferDepth)
            {
                raster->depth = 32;
                format = rwRASTERFORMAT32;
            }
        }
        else
        {
            if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT16 && _RwD3D9ZBufferDepth == 16)
            {
                raster->depth = 16;
            }
            else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT32 && _RwD3D9ZBufferDepth == 32)
            {
                raster->depth = 32;
            }
            else
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
        }
        break;

    default:
        RWERROR((E_RW_INVRASTERFORMAT));
        RWRETURN(FALSE);
        break;
    }

    raster->cFormat = (RwUInt8)(format >> 8);

    RWRETURN(TRUE);
}

/****************************************************************************
 rxD3D9VideoMemoryRasterListAdd

 Add current raster to the video memory list

 On entry   : raster - Raster to add
 */
static void
rxD3D9VideoMemoryRasterListAdd(RwRaster *raster)
{
    RxD3D9VideoMemoryRaster *videoRaster;

    RWFUNCTION(RWSTRING("rxD3D9VideoMemoryRasterListAdd"));

    videoRaster = (RxD3D9VideoMemoryRaster *)
                                RwFreeListAlloc(VideoMemoryRastersFreeList,
                                    rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

    videoRaster->raster = raster;
    videoRaster->next = VideoMemoryRasters;

    VideoMemoryRasters = videoRaster;

#if defined(RWDEBUG)
    NumVideoMemoryRasters++;
#endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/****************************************************************************
 rxD3D9VideoMemoryRasterListRemove

 Remove current raster of the video memory list

 On entry   : raster - Raster to add
 */
static void
rxD3D9VideoMemoryRasterListRemove(RwRaster *raster)
{
    RxD3D9VideoMemoryRaster *currentVideoRaster;
    RxD3D9VideoMemoryRaster *prevVideoRaster;

    RWFUNCTION(RWSTRING("rxD3D9VideoMemoryRasterListRemove"));

    currentVideoRaster = VideoMemoryRasters;
    prevVideoRaster = NULL;

    while (currentVideoRaster)
    {
        if (currentVideoRaster->raster == raster)
        {
            break;
        }

        prevVideoRaster = currentVideoRaster;
        currentVideoRaster = currentVideoRaster->next;
    }

    if (currentVideoRaster)
    {
        if (prevVideoRaster)
        {
            prevVideoRaster->next = currentVideoRaster->next;
        }
        else
        {
            if (VideoMemoryRasters == currentVideoRaster)
            {
                VideoMemoryRasters = currentVideoRaster->next;
            }
        }

        RwFreeListFree(VideoMemoryRastersFreeList, currentVideoRaster);

    #if defined(RWDEBUG)
        NumVideoMemoryRasters--;
    #endif /* defined(RWDEBUG) */
    }

    RWRETURNVOID();
}

/****************************************************************************
 rxD3D9VideoMemoryRasterListCreate

 Creates the list

  */
static RwBool
rxD3D9VideoMemoryRasterListCreate(void)
{
    RWFUNCTION(RWSTRING("rxD3D9VideoMemoryRasterListCreate"));

    VideoMemoryRastersFreeList = RwFreeListCreate(
                                        sizeof(RxD3D9VideoMemoryRaster),
                                        127,    /* It results in ~1024 Bytes */
                                        4,
                                        rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

    RWRETURN(VideoMemoryRastersFreeList != NULL);
}

/****************************************************************************
 rxD3D9VideoMemoryRasterListDestroy

 Destroy the list

  */
static void
rxD3D9VideoMemoryRasterListDestroy(void)
{
    RWFUNCTION(RWSTRING("rxD3D9VideoMemoryRasterListDestroy"));

#if defined(RWDEBUG)
    RWASSERT(NumVideoMemoryRasters == 0);
#endif /* defined(RWDEBUG) */

    while (VideoMemoryRasters)
    {
        RxD3D9VideoMemoryRaster *next = VideoMemoryRasters->next;

        RwFreeListFree(VideoMemoryRastersFreeList, VideoMemoryRasters);

        VideoMemoryRasters = next;
    }

    if (VideoMemoryRastersFreeList != NULL)
    {
        RwFreeListDestroy(VideoMemoryRastersFreeList);
        VideoMemoryRastersFreeList = NULL;
    }

#if defined(RWDEBUG)
    NumVideoMemoryRasters = 0;
#endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D9VideoMemoryRasterListRelease

 Release all the video memory

  */
void
_rxD3D9VideoMemoryRasterListRelease(void)
{
    RxD3D9VideoMemoryRaster *currentVideoRaster;

    RWFUNCTION(RWSTRING("_rxD3D9VideoMemoryRasterListRelease"));

    currentVideoRaster = VideoMemoryRasters;
    while (currentVideoRaster)
    {
        RwRaster                *raster = currentVideoRaster->raster;
        _rwD3D9RasterExt        *rasExt = RASTEREXTFROMRASTER(raster);
        RxD3D9VideoMemoryRaster *next = currentVideoRaster->next;

        if (raster->parent == raster)
        {
            switch(raster->cType)
            {
                case rwRASTERTYPENORMAL:
                case rwRASTERTYPETEXTURE:
                {
                    if (rasExt->texture)
                    {
                        if (IS_D3DFORMAT_ZBUFFER(rasExt->d3dFormat))
                        {
                            IDirect3DTexture9_Release(rasExt->texture);
                            rasExt->texture = NULL;
                        }
                    }
                }
                break;

                case rwRASTERTYPECAMERATEXTURE:
                {
                    if (rasExt->texture)
                    {
                        IDirect3DTexture9_Release(rasExt->texture);
                        rasExt->texture = NULL;
                    }
                }
                break;

                case rwRASTERTYPEZBUFFER:
                {
                    if (rasExt->texture)
                    {
                        if ((LPSURFACE)rasExt->texture != _RwD3D9DepthStencilSurface)
                        {
                            IDirect3DSurface9_Release((LPSURFACE)rasExt->texture);
                            rasExt->texture = NULL;
                        }
                    }
                }
                break;

                case rwRASTERTYPECAMERA:
                {
                    if (rasExt->swapChain)
                    {
                        IDirect3DSwapChain9_Release(rasExt->swapChain);
                    }
                }
                break;

                default:
                    break;
            }
        }

        currentVideoRaster = next;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D9VideoMemoryRasterListRestore

 Release all the video memory

  */
RwBool
_rxD3D9VideoMemoryRasterListRestore(void)
{
    RwBool                  result;
    HRESULT                 hr;
    RxD3D9VideoMemoryRaster *currentVideoRaster;

    RWFUNCTION(RWSTRING("_rxD3D9VideoMemoryRasterListRestore"));

#if defined(RWDEBUG)
    if (NumVideoMemoryRasters)
    {
        RwChar          buffer[256];

        rwsprintf(buffer, "Num Rasters Recreated: %d", NumVideoMemoryRasters);

        RwDebugSendMessage(rwDEBUGMESSAGE, "_rxD3D9VideoMemoryRasterListRestore", buffer);
    }
#endif

    result = TRUE;
    hr = D3D_OK;
    currentVideoRaster = VideoMemoryRasters;
    while (currentVideoRaster)
    {
        RwRaster                *raster = currentVideoRaster->raster;
        _rwD3D9RasterExt        *rasExt = RASTEREXTFROMRASTER(raster);
        RxD3D9VideoMemoryRaster *next = currentVideoRaster->next;

        if (raster->parent == raster)
        {
            switch(raster->cType)
            {
                case rwRASTERTYPENORMAL:
                case rwRASTERTYPETEXTURE:
                    {
                        RwUInt32    rasFormat = RwRasterGetFormat(raster);

                        if ((rasFormat & rwRASTERFORMATPAL4) || (rasFormat & rwRASTERFORMATPAL8))
                        {
                            if (rasExt->palette)
                            {
                                hr = DXCHECK(IDirect3DDevice9_SetPaletteEntries(_RwD3DDevice,
                                                                   rasExt->palette->globalindex,
                                                                   (PALETTEENTRY *)(rasExt->palette->entries)));
                            }
                        }
                        else if (IS_D3DFORMAT_ZBUFFER(rasExt->d3dFormat))
                        {
                            hr = DXCHECK(IDirect3DDevice9_CreateTexture(_RwD3DDevice,
                                                                        raster->width,
                                                                        raster->height,
                                                                        1,
                                                                        D3DUSAGE_DEPTHSTENCIL,
                                                                        rasExt->d3dFormat,
                                                                        D3DPOOL_DEFAULT,
                                                                        &(rasExt->texture),
                                                                        NULL));
                        }
                    }
                    break;

                case rwRASTERTYPECAMERATEXTURE:
                    {
                        if (rasExt->texture == NULL)
                        {
                            RwUInt32    rasFormat = RwRasterGetFormat(raster);
                            RwUInt32    levels = ((rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1);

                            if (rasExt->cube)
                            {
                                hr = DXCHECK(IDirect3DDevice9_CreateCubeTexture(_RwD3DDevice,
                                                                                raster->width,
                                                                                levels,
                                                                                (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0) | D3DUSAGE_RENDERTARGET,
                                                                                rasExt->d3dFormat,
                                                                                D3DPOOL_DEFAULT,
                                                                                (LPDIRECT3DCUBETEXTURE9 *)&(rasExt->texture),
                                                                                NULL));
                            }
                            else
                            {
                                hr = DXCHECK(IDirect3DDevice9_CreateTexture(_RwD3DDevice,
                                                               raster->width,
                                                               raster->height,
                                                               levels,
                                                               (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0) | D3DUSAGE_RENDERTARGET,
                                                               rasExt->d3dFormat,
                                                               D3DPOOL_DEFAULT,
                                                               &(rasExt->texture),
                                                               NULL));
                            }

                            if (FAILED(hr))
                            {
                                if ( hr != D3DERR_OUTOFVIDEOMEMORY )
                                {
                                    if (rasExt->cube)
                                    {
                                        hr = DXCHECK(IDirect3DDevice9_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                                                    (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                                                    (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0),
                                                                                    Present.BackBufferFormat,
                                                                                    D3DPOOL_DEFAULT,
                                                                                    (LPDIRECT3DCUBETEXTURE9 *)&(rasExt->texture),
                                                                                    NULL));
                                    }
                                    else
                                    {
                                        hr = DXCHECK(IDirect3DDevice9_CreateTexture(_RwD3DDevice, raster->width,
                                                                   raster->height,
                                                                   (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                                   (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0),
                                                                   Present.BackBufferFormat,
                                                                   D3DPOOL_DEFAULT,
                                                                   &(rasExt->texture),
                                                                   NULL));
                                    }

                                    raster->cFormat = (RwUInt8)(_rwD3D9PixelFormatInfo[Present.BackBufferFormat].rwFormat >> 8);
                                    raster->depth = _rwD3D9PixelFormatInfo[Present.BackBufferFormat].depth;

                                    rasExt->d3dFormat = Present.BackBufferFormat;
                                    rasExt->alpha = _rwD3D9PixelFormatInfo[Present.BackBufferFormat].alpha;
                                }
                            }
                        }
                    }
                    break;

                case rwRASTERTYPEZBUFFER:
                    {
                        if (rasExt->texture == NULL)
                        {
                            /* Find the Z-Buffer depth */
                            if (16 == _RwD3D9ZBufferDepth)
                            {
                                if( _rwD3D9CheckValidZBufferFormat(D3DFMT_D15S1) )
                                {
                                    rasExt->d3dFormat = D3DFMT_D15S1;
                                }
                                else
                                {
                                    rasExt->d3dFormat = D3DFMT_D16;
                                }
                            }
                            else if (32 == _RwD3D9ZBufferDepth)
                            {
                                if( _rwD3D9CheckValidZBufferFormat(D3DFMT_D24S8) )
                                {
                                    rasExt->d3dFormat = D3DFMT_D24S8;
                                }
                                else if( _rwD3D9CheckValidZBufferFormat(D3DFMT_D24X4S4) )
                                {
                                    rasExt->d3dFormat = D3DFMT_D24X4S4;
                                }
                                else if( _rwD3D9CheckValidZBufferFormat(D3DFMT_D32) )
                                {
                                    rasExt->d3dFormat = D3DFMT_D32;
                                }
                                else
                                {
                                    rasExt->d3dFormat = D3DFMT_D24X8;
                                }
                            }

                            hr = DXCHECK(IDirect3DDevice9_CreateDepthStencilSurface(_RwD3DDevice,
                                                                       max(1, raster->width),
                                                                       max(1, raster->height),
                                                                       rasExt->d3dFormat,
                                                                       Present.MultiSampleType,
                                                                       Present.MultiSampleQuality,
                                                                       FALSE,
                                                                       (LPSURFACE *)&(rasExt->texture),
                                                                       NULL));
                        }
                        else
                        {
                            RECT rect;

                            GetClientRect(WindowHandle, &rect);

                            raster->width = rect.right;
                            raster->height = rect.bottom;

                            rasExt->texture = (LPDIRECT3DTEXTURE9)_RwD3D9DepthStencilSurface;
                            rasExt->d3dFormat = Present.AutoDepthStencilFormat;
                        }
                    }
                    break;

                case rwRASTERTYPECAMERA:
                    {
                        if (rasExt->swapChain)
                        {
                            D3DPRESENT_PARAMETERS   present;
                            RECT rect;

                            present = Present;

                            present.hDeviceWindow = (HWND)rasExt->window;

                            GetClientRect(present.hDeviceWindow, &rect);

                            if (rect.right)
                            {
                                present.BackBufferWidth = 0;
                            }
                            else
                            {
                                present.BackBufferWidth = 1;
                            }

                            if (rect.bottom)
                            {
                                present.BackBufferHeight = 0;
                            }
                            else
                            {
                                present.BackBufferHeight = 1;
                            }

                            present.EnableAutoDepthStencil = FALSE;

                            rasExt->swapChain = NULL;

                            if(D3D_OK == DXCHECK(IDirect3DDevice9_CreateAdditionalSwapChain(_RwD3DDevice, &present, &(rasExt->swapChain))))
                            {
                                IDirect3DSurface9** ppBackBuffer =
                                    (IDirect3DSurface9**) &(rasExt->texture);

                                hr = IDirect3DSwapChain9_GetBackBuffer(rasExt->swapChain, 0, D3DBACKBUFFER_TYPE_MONO, ppBackBuffer);

                                IDirect3DSurface9_Release((LPSURFACE)rasExt->texture);

                                rasExt->d3dFormat = present.BackBufferFormat;
                            }
                        }
                        else
                        {
                            if (_RwD3D9AdapterInformation.mode.Width &&
                                (RwUInt32)raster->width != Present.BackBufferWidth)
                            {
                                raster->width = (raster->width * Present.BackBufferWidth) / _RwD3D9AdapterInformation.mode.Width;
                            }

                            if (_RwD3D9AdapterInformation.mode.Height &&
                                (RwUInt32)raster->height != Present.BackBufferHeight)
                            {
                                raster->height = (raster->height * Present.BackBufferHeight) / _RwD3D9AdapterInformation.mode.Height;
                            }

                            raster->depth = _rwD3D9PixelFormatInfo[Present.BackBufferFormat].depth;
                            raster->cFormat = (RwUInt8)(_rwD3D9PixelFormatInfo[Present.BackBufferFormat].rwFormat >> 8);

                            rasExt->d3dFormat = Present.BackBufferFormat;
                            rasExt->alpha = _rwD3D9PixelFormatInfo[Present.BackBufferFormat].alpha;
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        if (FAILED(hr))
        {
            result = FALSE;
        }

        currentVideoRaster = next;
    }

    RWRETURN(result);
}

/****************************************************************************
 FindAvailablePaletteIndex
 */
static RwUInt32
FindAvailablePaletteIndex(void)
{
    RwUInt32 index;

    RWFUNCTION(RWSTRING("FindAvailablePaletteIndex"));

    if (NumPaletteIndexFree)
    {
        NumPaletteIndexFree--;

        RWASSERT(PaletteIndexFree != NULL);
        index = PaletteIndexFree[NumPaletteIndexFree];
    }
    else
    {
        index = MaxPaletteIndex;

        MaxPaletteIndex ++;
    }

    RWRETURN(index);
}

/****************************************************************************
 AddAvailablePaletteIndex
 */
static void
AddAvailablePaletteIndex(RwUInt32 index)
{
    RWFUNCTION(RWSTRING("AddAvailablePaletteIndex"));
    RWASSERT(index < MaxPaletteIndex);

    if (NumPaletteIndexFree >= MaxPaletteIndexFree)
    {
        MaxPaletteIndexFree += 512;

        if (PaletteIndexFree == NULL)
        {
            PaletteIndexFree = (RwUInt16 *)
                               RwMalloc(MaxPaletteIndexFree * sizeof(RwUInt16),
                                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT |
                                rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            PaletteIndexFree = (RwUInt16 *)
                                RwRealloc(PaletteIndexFree,
                                          MaxPaletteIndexFree * sizeof(RwUInt16),
                                          rwID_DRIVERMODULE     |
                                          rwMEMHINTDUR_EVENT    |
                                          rwMEMHINTFLAG_RESIZABLE);
        }
    }

    RWASSERT(PaletteIndexFree != NULL);
    PaletteIndexFree[NumPaletteIndexFree++] = (RwUInt16)index;

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D9CreateTextureRaster

 Create a texture raster

 On entry   : raster - Raster to allocate
 On exit    : TRUE on success
 */
static RwBool
rwD3D9CreateTextureRaster(RwRaster *raster,
                         _rwD3D9RasterExt *rasExt)
{
    RwUInt32    rasFormat;
    RwUInt32    pixelFormat;
    HRESULT     hr;

    RWFUNCTION(RWSTRING("rwD3D9CreateTextureRaster"));

    rasFormat = RwRasterGetFormat(raster);
    pixelFormat = rasFormat & rwRASTERFORMATPIXELFORMATMASK;

    if ((rasFormat & rwRASTERFORMATPAL4) || (rasFormat & rwRASTERFORMATPAL8))
    {
        if (!((rwRASTERFORMAT888 == pixelFormat) ||
              (rwRASTERFORMAT8888 == pixelFormat)))
        {
            RWERROR((E_RW_DEVICEERROR,
                RWSTRING("Palletized texture can only be rwRASTERFORMAT888 or rwRASTERFORMAT8888.")));
            RWRETURN(FALSE);
        }

        /* Create the palette */
        if (PaletteFreeList == NULL)
        {
            PaletteFreeList = RwFreeListCreate(sizeof(_rwD3D9Palette), 64, 4,
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
        }

        rasExt->palette = (_rwD3D9Palette *)RwFreeListAlloc(PaletteFreeList,
            rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

        if(rasExt->palette)
        {
            rasExt->palette->globalindex = FindAvailablePaletteIndex();
        }

        rasExt->d3dFormat = D3DFMT_P8;
    }
    else
    {
        rasExt->d3dFormat = _rwRasterConvTable[FMT2TBL(rasFormat)].format;
    }

    /* Does this raster format have an alpha component */
    rasExt->alpha = _rwRasterConvTable[FMT2TBL(rasFormat)].alpha;

    /* Check the size of the texture */
    _rwD3D9CheckRasterSize(&(raster->width), &(raster->height), raster->cType | rasFormat);

    /* Create the texture, if mipped 0 gets all the mip levels */
    if (rasExt->cube)
    {
        hr = DXCHECK(IDirect3DDevice9_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                    (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                    (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0),
                                                    rasExt->d3dFormat,
                                                    D3DPOOL_MANAGED,
                                                    (LPDIRECT3DCUBETEXTURE9 *)&(rasExt->texture),
                                                    NULL));
    }
    else
    {
        if (IS_D3DFORMAT_ZBUFFER(rasExt->d3dFormat))
        {
            hr = DXCHECK(IDirect3DDevice9_CreateTexture(_RwD3DDevice, raster->width,
                                                        raster->height,
                                                        1,
                                                        D3DUSAGE_DEPTHSTENCIL,
                                                        rasExt->d3dFormat,
                                                        D3DPOOL_DEFAULT,
                                                        &(rasExt->texture),
                                                        NULL));

            if (SUCCEEDED(hr))
            {
                rxD3D9VideoMemoryRasterListAdd(raster);
            }
        }
        else
        {
            hr = DXCHECK(IDirect3DDevice9_CreateTexture(_RwD3DDevice, raster->width,
                                                        raster->height,
                                                        (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                        (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0),
                                                        rasExt->d3dFormat,
                                                        D3DPOOL_MANAGED,
                                                        &(rasExt->texture),
                                                        NULL));
        }
    }

    if (FAILED(hr))
    {
        if (D3DFMT_P8 == rasExt->d3dFormat)
        {
            if(rasExt->palette)
            {
                AddAvailablePaletteIndex(rasExt->palette->globalindex);

                RwFreeListFree(PaletteFreeList, rasExt->palette);
                rasExt->palette = NULL;
            }
        }

        RWERROR((E_RW_DEVICEERROR, RWSTRING("Failed to create texture.")));
        RWRETURN(FALSE);
    }

    if (rasExt->palette != NULL)
    {
        rxD3D9VideoMemoryRasterListAdd(raster);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9CreateCameraTextureRaster

 Create a texture raster

 On entry   : raster - Raster to allocate
 On exit    : TRUE on success
 */
static RwBool
rwD3D9CreateCameraTextureRaster(RwRaster *raster, _rwD3D9RasterExt *rasExt)
{
    RwUInt32    rasFormat;
    RwUInt32    pixelFormat;
    HRESULT     hr;

    RWFUNCTION(RWSTRING("rwD3D9CreateCameraTextureRaster"));

    rasFormat = RwRasterGetFormat(raster);
    pixelFormat = rasFormat & rwRASTERFORMATPIXELFORMATMASK;

    /* rwRASTERTYPECAMERATEXTURE can not be palletized */
    if ((rasFormat & rwRASTERFORMATPAL4) || (rasFormat & rwRASTERFORMATPAL8))
    {
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("rwRASTERTYPECAMERATEXTURE can not be palletized.")));
        RWRETURN(FALSE);
    }

    rasExt->d3dFormat = _rwRasterConvTable[FMT2TBL(rasFormat)].format;

    /* Does this raster format have an alpha component */
    rasExt->alpha = _rwRasterConvTable[FMT2TBL(rasFormat)].alpha;

    /* Check the size of the texture */
    _rwD3D9CheckRasterSize(&(raster->width), &(raster->height), raster->cType | rasFormat);

    /* Remove managed resources from memory when creating a video memory resource */
    IDirect3DDevice9_EvictManagedResources(_RwD3DDevice);

    /* Create the texture, if mipped 0 gets all the mip levels */
    if (rasExt->cube)
    {
        hr = DXCHECK(IDirect3DDevice9_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                    (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                    (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0) | D3DUSAGE_RENDERTARGET,
                                                    rasExt->d3dFormat,
                                                    D3DPOOL_DEFAULT,
                                                    (LPDIRECT3DCUBETEXTURE9 *)&(rasExt->texture),
                                                    NULL));
    }
    else
    {
        hr = IDirect3DDevice9_CreateTexture(_RwD3DDevice, raster->width,
                                          raster->height,
                                          (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                          (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0) | D3DUSAGE_RENDERTARGET,
                                          rasExt->d3dFormat,
                                          D3DPOOL_DEFAULT,
                                          &(rasExt->texture),
                                          NULL);
    }

    if ( hr == D3DERR_OUTOFVIDEOMEMORY )
    {
        RWERROR((E_RW_DEVICEERROR, RWSTRING("Out of video memory. Failed to create raster.")));
        RWRETURN(FALSE);
    }
    else if (FAILED(hr))
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "rwD3D9CreateCameraTextureRaster", "Device doesn't support camera textures, using normal textures. Backbuffer will be used for rendering");

        if (rasExt->cube)
        {
            hr = DXCHECK(IDirect3DDevice9_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                            (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                            (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0),
                                                            Present.BackBufferFormat,
                                                            D3DPOOL_DEFAULT,
                                                            (LPDIRECT3DCUBETEXTURE9 *)&(rasExt->texture),
                                                            NULL));
        }
        else
        {
            hr = DXCHECK(IDirect3DDevice9_CreateTexture(_RwD3DDevice, raster->width,
                                                        raster->height,
                                                        (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                        (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0),
                                                        Present.BackBufferFormat,
                                                        D3DPOOL_DEFAULT,
                                                        &(rasExt->texture),
                                                        NULL));
        }

        if (FAILED(hr))
        {
            RWERROR((E_RW_DEVICEERROR, RWSTRING("Failed to create raster.")));
            RWRETURN(FALSE);
        }
        else
        {
            raster->cFormat = (RwUInt8)(rwD3D9FindRwFormat(Present.BackBufferFormat) >> 8);
            raster->depth = _RwD3D9AdapterInformation.displayDepth;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9CreateZBufferRaster

 Create a Z-Buffer

 On entry   : raster - Raster to allocate
 On exit    : TRUE on success
 */
static RwBool
rwD3D9CreateZBufferRaster(RwRaster *raster, _rwD3D9RasterExt *rasExt)
{
    RECT        rect;

    RWFUNCTION(RWSTRING("rwD3D9CreateZBufferRaster"));

    /* Find the Z-Buffer depth */
    if (16 == _RwD3D9ZBufferDepth)
    {
        if( _rwD3D9CheckValidZBufferFormat(D3DFMT_D15S1) )
        {
            rasExt->d3dFormat = D3DFMT_D15S1;
        }
        else
        {
            rasExt->d3dFormat = D3DFMT_D16;
        }
    }
    else if (32 == _RwD3D9ZBufferDepth)
    {
        if( _rwD3D9CheckValidZBufferFormat(D3DFMT_D24S8) )
        {
            rasExt->d3dFormat = D3DFMT_D24S8;
        }
        else if( _rwD3D9CheckValidZBufferFormat(D3DFMT_D24X4S4) )
        {
            rasExt->d3dFormat = D3DFMT_D24X4S4;
        }
        else if( _rwD3D9CheckValidZBufferFormat(D3DFMT_D32) )
        {
            rasExt->d3dFormat = D3DFMT_D32;
        }
        else
        {
            rasExt->d3dFormat = D3DFMT_D24X8;
        }
    }

    rasExt->alpha = 0;

    GetClientRect(WindowHandle, &rect);

    if (rect.right != raster->width || rect.bottom != raster->height )
    {
        HRESULT hr;

        /* Remove managed resources from memory when creating a video memory resource */
        IDirect3DDevice9_EvictManagedResources(_RwD3DDevice);

        hr = IDirect3DDevice9_CreateDepthStencilSurface(_RwD3DDevice,
                                                raster->width,
                                                raster->height,
                                                rasExt->d3dFormat,
                                                Present.MultiSampleType,
                                                Present.MultiSampleQuality,
                                                FALSE,
                                                (LPSURFACE *)&(rasExt->texture),
                                                NULL);

        if ( hr == D3DERR_OUTOFVIDEOMEMORY )
        {
            RWERROR((E_RW_DEVICEERROR, RWSTRING("Out of video memory. Failed to create raster.")));
            RWRETURN(FALSE);
        }
        else if ( FAILED(hr) )
        {
            RWERROR((E_RW_DEVICEERROR, RWSTRING("Failed to create raster.")));
            RWRETURN(FALSE);
        }
    }
    else
    {
        rasExt->texture = (LPDIRECT3DTEXTURE9)_RwD3D9DepthStencilSurface;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterCreate

 Create a raster

 On entry   : NULL
            : raster - raster to allocate
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterCreate(void *unused1 __RWUNUSED__, void *rasterIn, RwInt32 flags)
{
    RwRaster        *raster = (RwRaster *)rasterIn;
    _rwD3D9RasterExt  *rasExt = RASTEREXTFROMRASTER(raster);

    RWFUNCTION(RWSTRING("_rwD3D9RasterCreate"));

    /* Initialise structure to something sensible */
    raster->cType    = (RwUInt8)(flags & rwRASTERTYPEMASK);
    raster->cFlags   = (RwUInt8)(flags & ~rwRASTERTYPEMASK);
    raster->cpPixels = NULL;
    raster->palette  = NULL;

    rasExt->texture = NULL;
    rasExt->palette = NULL;
    rasExt->alpha = 0;
    rasExt->cube = 0;
    rasExt->automipmapgen = 0;
    rasExt->compressed = 0;
    rasExt->face = 0;
    rasExt->lockedMipLevel = (RwUInt8)-1;
    rasExt->lockedSurface = NULL;
    rasExt->d3dFormat = 0;

    /* Set up the raster format and depth */
    if (!rwD3D9SetRasterFormat(raster, flags))
    {
        RWRETURN(FALSE);
    }

    /*
     * If it is not a camera buffer, then we need to allocate real memory for it
     */
    if ((raster->width) && (raster->height))
    {
        switch (raster->cType & rwRASTERTYPEMASK)
        {
            case rwRASTERTYPENORMAL:
            {
                if (!(raster->cFlags & rwRASTERDONTALLOCATE))
                {
                    #if defined(RWDEBUG)
                    NumNormalRasters++;
                    #endif /* defined(RWDEBUG) */

                    if (!rwD3D9CreateTextureRaster(raster, rasExt))
                    {
                        RWRETURN(FALSE);
                    }
                }
            }
            break;

            case rwRASTERTYPETEXTURE:
            {
                if (!(raster->cFlags & rwRASTERDONTALLOCATE))
                {
                    #if defined(RWDEBUG)
                    NumTextureRasters++;
                    #endif /* defined(RWDEBUG) */

                    if (!rwD3D9CreateTextureRaster(raster, rasExt))
                    {
                        RWRETURN(FALSE);
                    }
                }
            }
            break;

            case rwRASTERTYPECAMERATEXTURE:
            {
                if (!(raster->cFlags & rwRASTERDONTALLOCATE))
                {
                    #if defined(RWDEBUG)
                    NumCameraTextureRasters++;
                    #endif /* defined(RWDEBUG) */

                    if (!rwD3D9CreateCameraTextureRaster(raster, rasExt))
                    {
                        RWRETURN(FALSE);
                    }

                    rxD3D9VideoMemoryRasterListAdd(raster);
                }
            }
            break;

            case rwRASTERTYPEZBUFFER:
            {
                if (!(raster->cFlags & rwRASTERDONTALLOCATE))
                {
                    #if defined(RWDEBUG)
                    NumZBufferRasters++;
                    #endif /* defined(RWDEBUG) */

                    if (!rwD3D9CreateZBufferRaster(raster, rasExt))
                    {
                        RWRETURN(FALSE);
                    }

                    rxD3D9VideoMemoryRasterListAdd(raster);
                }
            }
            break;

            case rwRASTERTYPECAMERA:
            {
                RECT    rect;

                GetClientRect(WindowHandle, &rect);

                if ( (raster->cFlags & rwRASTERDONTALLOCATE) != 0 ||
                    (rect.right >= raster->width && rect.bottom >= raster->height) )
                {
                    #if defined(RWDEBUG)
                    NumCameraRasters++;
                    #endif /* defined(RWDEBUG) */

                    /* Find the depth of the backbuffer */
                    if (Present.BackBufferFormat == D3DFMT_A8R8G8B8 ||
                        Present.BackBufferFormat == D3DFMT_X8R8G8B8 ||
                        Present.BackBufferFormat == D3DFMT_A2R10G10B10)
                    {
                        raster->depth = 32;
                    }
                    else if (Present.BackBufferFormat == D3DFMT_R5G6B5 ||
                             Present.BackBufferFormat == D3DFMT_X1R5G5B5 ||
                             Present.BackBufferFormat == D3DFMT_A1R5G5B5)
                    {
                        raster->depth = 16;
                    }
                    else
                    {
                        raster->depth = 0;
                    }

                    /* Very little to do here - the camera is global */
                    raster->stride   = 0;
                    raster->cpPixels = NULL;
                    raster->cFlags = (RwUInt8)rwRASTERDONTALLOCATE;

                    raster->originalWidth = raster->width;
                    raster->originalHeight = raster->height;

                    rasExt->d3dFormat = Present.BackBufferFormat;

                    rxD3D9VideoMemoryRasterListAdd(raster);

                    RWRETURN(TRUE);
                }
                else
                {
                    RWERROR((E_RW_DEVICEERROR, RWSTRING("Camera raster is too big.")));
                    RWRETURN(FALSE);
                }
            }
            break;

        }
    }
    else
    {
        raster->cFlags = rwRASTERDONTALLOCATE;   /* Not allocated */

        raster->stride = 0;
        if (raster->depth == 0)
        {
            raster->depth = 16;
        }

        /* Camera Sub rasters need to be added */
        if (raster->cType == rwRASTERTYPECAMERA)
        {
            #if defined(RWDEBUG)
            NumCameraRasters++;
            #endif /* defined(RWDEBUG) */

            rxD3D9VideoMemoryRasterListAdd(raster);
        }

        RWRETURN(TRUE);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterDestroy

 Destroy a raster

 On entry   : NULL
            : raster - raster to destroy
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterDestroy(void *unused1 __RWUNUSED__,
                   void *raster,
                   RwInt32 unused3 __RWUNUSED__)
{
    RwRaster            *ras = (RwRaster *)raster;
    _rwD3D9RasterExt    *rasExt;
    RwUInt32            n;

    RWFUNCTION(RWSTRING("_rwD3D9RasterDestroy"));

    rasExt = RASTEREXTFROMRASTER(ras);

    /* Check if it's the current rendering raster */
    for (n = 0; n < RWD3D9_MAX_TEXTURE_STAGES; n++)
    {
        if (_rwD3D9RWGetRasterStage(n) == ras)
        {
            _rwD3D9RWSetRasterStage(NULL, n);
        }
    }

    /*
     * Only free up the surface if this raster owns it (ie, it's not a sub
     * raster and it's not created with dontallocate flag)
     */
    if ((ras->parent == ras))
    {
        switch (ras->cType)
        {
            case (rwRASTERTYPENORMAL):
                {
                    if (!(ras->cFlags & rwRASTERDONTALLOCATE))
                    {
                        #if defined(RWDEBUG)
                        NumNormalRasters--;
                        #endif /* defined(RWDEBUG) */

                        if (rasExt->palette != NULL)
                        {
                            rxD3D9VideoMemoryRasterListRemove(ras);

                            AddAvailablePaletteIndex(rasExt->palette->globalindex);

                            RwFreeListFree(PaletteFreeList, rasExt->palette);
                            rasExt->palette = NULL;
                        }
                        else if (IS_D3DFORMAT_ZBUFFER(rasExt->d3dFormat))
                        {
                            rxD3D9VideoMemoryRasterListRemove(ras);
                        }

                        /* Destroy the texture */
                        if (rasExt->texture)
                        {
                            IDirect3DTexture9_Release(rasExt->texture);
                        }
                    }
                }
                break;

            case (rwRASTERTYPETEXTURE):
                {
                    if (!(ras->cFlags & rwRASTERDONTALLOCATE))
                    {
                        #if defined(RWDEBUG)
                        NumTextureRasters--;
                        #endif /* defined(RWDEBUG) */

                        if (rasExt->palette != NULL)
                        {
                            rxD3D9VideoMemoryRasterListRemove(ras);

                            AddAvailablePaletteIndex(rasExt->palette->globalindex);

                            RwFreeListFree(PaletteFreeList, rasExt->palette);
                            rasExt->palette = NULL;
                        }
                        else if (IS_D3DFORMAT_ZBUFFER(rasExt->d3dFormat))
                        {
                            rxD3D9VideoMemoryRasterListRemove(ras);
                        }

                        /* Destroy the texture */
                        if (rasExt->texture)
                        {
                            IDirect3DTexture9_Release(rasExt->texture);
                        }
                    }
                }
                break;

            case (rwRASTERTYPECAMERATEXTURE):
                {
                    if (!(ras->cFlags & rwRASTERDONTALLOCATE))
                    {
                        #if defined(RWDEBUG)
                        NumCameraTextureRasters--;
                        #endif /* defined(RWDEBUG) */

                        rxD3D9VideoMemoryRasterListRemove(ras);

                        /* Destroy the texture */
                        if (rasExt->texture)
                        {
                            IDirect3DTexture9_Release(rasExt->texture);
                        }
                    }
                }
                break;

            case (rwRASTERTYPEZBUFFER):
                {
                    if (!(ras->cFlags & rwRASTERDONTALLOCATE))
                    {
                        #if defined(RWDEBUG)
                        NumZBufferRasters--;
                        #endif /* defined(RWDEBUG) */

                        rxD3D9VideoMemoryRasterListRemove(ras);

                        /* Destroy the surface */
                        if ((LPSURFACE)rasExt->texture!=_RwD3D9DepthStencilSurface)
                        {
                            if (rasExt->texture)
                            {
                                IDirect3DSurface9_Release((LPSURFACE)rasExt->texture);
                            }
                        }
                    }
                }
                break;

            case (rwRASTERTYPECAMERA):
                {
                    #if defined(RWDEBUG)
                    NumCameraRasters--;
                    #endif /* defined(RWDEBUG) */

                    rxD3D9VideoMemoryRasterListRemove(ras);
                }
                break;

            default:
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
                break;
        }
    }
    else
    {
        /* Camera Sub rasters need to be removed */
        if (ras->cType == rwRASTERTYPECAMERA)
        {
            #if defined(RWDEBUG)
            NumCameraRasters--;
            #endif /* defined(RWDEBUG) */

            rxD3D9VideoMemoryRasterListRemove(ras);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterGetMipLevels

 Gets the number of mip levels for the given raster

 On entry   : NULL
            : pRaster - raster
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterGetMipLevels(void *mipLevels,
                        void *ras,
                        RwInt32 unused3 __RWUNUSED__)
{
    RwInt32         *numMipLevels = (RwInt32 *)mipLevels;
    RwRaster        *raster = (RwRaster *)ras;

    RWFUNCTION(RWSTRING("_rwD3D9RasterGetMipLevels"));

    RWASSERT(numMipLevels);
    RWASSERT(raster);

    /* Get the top level raster as this is the only one with a valid texture */
    while (raster != RwRasterGetParent(raster))
    {
        raster = RwRasterGetParent(raster);
    }

    if ( (raster->cType == rwRASTERTYPETEXTURE) ||
         (raster->cType == rwRASTERTYPECAMERATEXTURE) ||
         (raster->cType == rwRASTERTYPENORMAL) )
    {
        _rwD3D9RasterExt  *rasExt = RASTEREXTFROMRASTER(raster);

        if (rasExt->texture != NULL)
        {
            *numMipLevels = IDirect3DTexture9_GetLevelCount(rasExt->texture);
        }
        else
        {
            RwUInt32 size, levels;

            levels = 0;

            size = max(raster->width, raster->height);

            while(size)
            {
                levels ++;

                size >>= 1;
            }

            *numMipLevels = levels;
        }
    }
    else
    {
        *numMipLevels = 1;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9TextureSetRaster


 On entry   :
 On exit    : TRUE on success
 */
RwBool
_rwD3D9TextureSetRaster(void *tex, void *ras, RwInt32 unused3 __RWUNUSED__)
{
    RwTexture          *texture = (RwTexture *) tex;
    RwRaster           *raster = (RwRaster *) ras;

    RWFUNCTION(RWSTRING("_rwD3D9TextureSetRaster"));

    /* Set the raster */
    texture->raster = raster;

    /* All done */
    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterSubRaster

 Get the pixel pointer from a raster

 On entry   : raster - raster to make subraster
              pIn - raster to make subraster of
              nIn - none
 On exit    : TRUE on success
 */
RwBool
_rwD3D9RasterSubRaster(void *rasterIn,
                     void *parentRasterIn,
                     RwInt32 unused3 __RWUNUSED__)
{
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwRaster    *parentRaster = (RwRaster *)parentRasterIn;

    RWFUNCTION(RWSTRING("_rwD3D9RasterSubRaster"));

    /*
     * The raster won't have old stuff - it should have
     * been created with rwRASTERDONTALLOCATE
     */

    /*
     * Core already set up offset and size
     */

    /*
     * Inherit the raster stride, depth, type and format
     */
    raster->stride   = parentRaster->stride;
    raster->depth    = parentRaster->depth;
    raster->cType    = parentRaster->cType;
    raster->cFormat  = parentRaster->cFormat;
    raster->cpPixels = NULL;           /* Get this when we lock the raster */

    RWRETURN(TRUE);
}

/**
 * \ingroup rwrasterd3d9
 * \ref RwD3D9RasterCreate is used to create a new raster of the specified
 * type with the given width, height and a D3D9 pixel format.
 * The width and height are specified in pixels, and the flags indicate
 * the type of the raster to create.
 *
 * \note This function may fail if the specified pixel format is not
 * supported by the video card.
 *
 * \param width  An \ref RwUInt32 value equal to the width of the raster.
 * \param height  An \ref RwUInt32 value equal to the height of the raster.
 * \param d3dFormat  A D3DFORMAT value describing the pixel format required.
 * See the D3D9 API documentation from Microsoft for more details.
 * \param flags  An \ref RwUInt32 value specifying the type of raster to create,
 *       combined using a bit-wise OR. The following flags are supported:
 *       \li rwRASTERTYPENORMAL - Device default raster.
 *       \li rwRASTERTYPETEXTURE - Texture raster.
 *       \li rwRASTERTYPECAMERATEXTURE - Camera texture raster.
 *       \li rwRASTERFORMATAUTOMIPMAP - RenderWare Graphics generates the mip levels.
 *       \li rwRASTERFORMATMIPMAP - Mip mapping on.
 *
 * \return Returns a pointer to the new raster if successful or NULL
 * if there is an error.
 */
RwRaster *
RwD3D9RasterCreate(RwUInt32 width,
                   RwUInt32 height,
                   RwUInt32 d3dFormat,
                   RwUInt32 flags)
{
    RwRaster            *raster = NULL;
    _rwD3D9RasterExt    *rasExt;
    HRESULT             hr;
    RwUInt32            usage, pool;

    RWAPIFUNCTION(RWSTRING("RwD3D9RasterCreate"));
    RWASSERT(width != 0);
    RWASSERT(height != 0);
    RWASSERT(d3dFormat != 0);
    RWASSERT(flags != 0);

    /* Remove mipmap if raster type is normal or zbuffer */
    if ((flags & rwRASTERTYPEMASK) == rwRASTERTYPENORMAL ||
        (flags & rwRASTERTYPEMASK) == rwRASTERTYPEZBUFFER)
    {
        flags &= ~(rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP);
    }

    /* Create a raster */
    raster = RwRasterCreate(width,
                            height,
                            0,
                            flags | rwRASTERDONTALLOCATE);

    if (NULL == raster)
    {
        RWRETURN(NULL);
    }

    /* Get the raster extension */
    rasExt = RASTEREXTFROMRASTER(raster);

    /* check automipmapgen */
    if ((flags & (rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP)) == (rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP))
    {
        if (_rwD3D9CheckAutoMipmapGenTextureFormat(d3dFormat))
        {
            rasExt->automipmapgen = 1;
        }
    }

    hr = E_FAIL;

    usage = (rasExt->automipmapgen ? D3DUSAGE_AUTOGENMIPMAP : 0);

    if ((flags & rwRASTERTYPECAMERATEXTURE) == rwRASTERTYPECAMERATEXTURE)
    {
        pool = D3DPOOL_DEFAULT;

        if (_rwD3D9CheckValidCameraTextureFormat(d3dFormat))
        {
            usage |= D3DUSAGE_RENDERTARGET;

            hr = D3D_OK;
        }
    }
    else
    {
        if (IS_D3DFORMAT_ZBUFFER(d3dFormat))
        {
            pool = D3DPOOL_DEFAULT;

            if (_rwD3D9CheckValidZBufferTextureFormat(d3dFormat))
            {
                usage |= D3DUSAGE_DEPTHSTENCIL;

                hr = D3D_OK;
            }
        }
        else
        {
            pool = D3DPOOL_MANAGED;

            if (_rwD3D9CheckValidTextureFormat(d3dFormat))
            {
                hr = D3D_OK;
            }
        }
    }

    if (FAILED(hr))
    {
        RwRasterDestroy(raster);

        RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D9RasterCreate",
                           "Pixel format not supported for this raster type.");

        RWRETURN(NULL);
    }

    hr = IDirect3DDevice9_CreateTexture(_RwD3DDevice,
                                        raster->width,
                                        raster->height,
                                        (flags & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                        usage,
                                        (D3DFORMAT)d3dFormat,
                                        pool,
                                        &(rasExt->texture),
                                        NULL);

    /* Create the texture, if mipped 0 gets all the mip levels */
    if (FAILED(hr))
    {
        RwRasterDestroy(raster);

        RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D9RasterCreate",
                           "Pixel format not supported.");

        RWRETURN(NULL);
    }
    else
    {
        if (pool == D3DPOOL_DEFAULT)
        {
            rxD3D9VideoMemoryRasterListAdd(raster);
        }

        #if defined(RWDEBUG)
        if ((flags & rwRASTERTYPECAMERATEXTURE) == rwRASTERTYPECAMERATEXTURE)
        {
            NumCameraTextureRasters++;
        }
        else
        {
            NumTextureRasters++;
        }
        #endif /* defined(RWDEBUG) */

        /* Remove the rwRASTERDONTALLOCATE flag so it will get destroyed */
        raster->cFlags &= ~rwRASTERDONTALLOCATE;

        /* Remove any raster pixel format */
        raster->cFormat &= ~(rwRASTERFORMATPIXELFORMATMASK >> 8);

        rasExt->d3dFormat = d3dFormat;
        if (d3dFormat >= D3DFMT_DXT1 && d3dFormat <= D3DFMT_DXT5)
        {
            rasExt->compressed = TRUE;

            if (d3dFormat == D3DFMT_DXT1)
            {
                rasExt->alpha = FALSE;
                raster->cFormat |= (rwRASTERFORMAT565 >> 8);
            }
            else
            {
                rasExt->alpha = TRUE;
                raster->cFormat |= (rwRASTERFORMAT4444 >> 8);
            }

            raster->depth = 16;
        }
        else
        {
            rasExt->compressed = FALSE;

            if (d3dFormat < MAX_PIXEL_FORMATS)
            {
                rasExt->alpha = _rwD3D9PixelFormatInfo[d3dFormat].alpha;
                raster->depth = _rwD3D9PixelFormatInfo[d3dFormat].depth;
                raster->cFormat |= (_rwD3D9PixelFormatInfo[d3dFormat].rwFormat >> 8);

                if (d3dFormat == D3DFMT_P8)
                {
                    /* Create the palette */
                    if (PaletteFreeList == NULL)
                    {
                        PaletteFreeList = RwFreeListCreate(sizeof(_rwD3D9Palette), 64, 4,
                                                           rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
                    }

                    rasExt->palette = (_rwD3D9Palette *)RwFreeListAlloc(PaletteFreeList,
                                                        rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

                    if(rasExt->palette)
                    {
                        rasExt->palette->globalindex = FindAvailablePaletteIndex();
                    }
                }
            }
        }
    }

    RWRETURN(raster);
}

/****************************************************************************
 _rwD3D9CheckValidZBufferFormat

 On entry   : Check if the z-buffer format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckValidZBufferFormat(RwInt32 format)
{
    HRESULT             hr;
    const D3DFORMAT  adapterFormat = _RwD3D9AdapterInformation.mode.Format;

    RWFUNCTION(RWSTRING("_rwD3D9CheckValidZBufferFormat"));

    hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      D3DUSAGE_DEPTHSTENCIL,
                                      D3DRTYPE_SURFACE,
                                      (D3DFORMAT)format);
    if (SUCCEEDED(hr))
    {

        hr = IDirect3D9_CheckDepthStencilMatch(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               adapterFormat,
                                               adapterFormat,
                                               (D3DFORMAT)format);
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D9CheckValidTextureFormat

 On entry   : Check if the texture format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckValidTextureFormat(RwInt32 format)
{
    const D3DFORMAT  adapterFormat = _RwD3D9AdapterInformation.mode.Format;
    HRESULT             hr;

    RWFUNCTION(RWSTRING("_rwD3D9CheckValidTextureFormat"));

    hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      0,
                                      D3DRTYPE_TEXTURE,
                                      (D3DFORMAT)format);
    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D9CheckValidCameraTextureFormat

 On entry   : Check if the rendertarget format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckValidCameraTextureFormat(RwInt32 format)
{
    HRESULT             hr;
    const D3DFORMAT  adapterFormat = _RwD3D9AdapterInformation.mode.Format;

    RWFUNCTION(RWSTRING("_rwD3D9CheckValidCameraTextureFormat"));

    hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      D3DUSAGE_RENDERTARGET,
                                      D3DRTYPE_TEXTURE,
                                      (D3DFORMAT)format);

    if (SUCCEEDED(hr))
    {
        hr = IDirect3D9_CheckDepthStencilMatch(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               adapterFormat,
                                               (D3DFORMAT)format,
                                               Present.AutoDepthStencilFormat);
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D9CheckValidZBufferTextureFormat

 On entry   : Check if the z-buffer format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckValidZBufferTextureFormat(RwInt32 format)
{
    HRESULT             hr;
    const D3DFORMAT  adapterFormat = _RwD3D9AdapterInformation.mode.Format;

    RWFUNCTION(RWSTRING("_rwD3D9CheckValidZBufferTextureFormat"));

    hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      D3DUSAGE_DEPTHSTENCIL,
                                      D3DRTYPE_TEXTURE,
                                      (D3DFORMAT)format);
    if (SUCCEEDED(hr))
    {

        hr = IDirect3D9_CheckDepthStencilMatch(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               adapterFormat,
                                               adapterFormat,
                                               (D3DFORMAT)format);
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D9CheckValidCubeTextureFormat

 On entry   : Check if the texture format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckValidCubeTextureFormat(RwInt32 format)
{
    const D3DFORMAT adapterFormat = _RwD3D9AdapterInformation.mode.Format;
    HRESULT         hr;

    RWFUNCTION(RWSTRING("_rwD3D9CheckValidCubeTextureFormat"));

    hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      0,
                                      D3DRTYPE_CUBETEXTURE,
                                      (D3DFORMAT)format);
    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D9CheckValidCameraCubeTextureFormat

 On entry   : Check if the rendertarget format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckValidCameraCubeTextureFormat(RwInt32 format)
{
    HRESULT             hr;
    const D3DFORMAT  adapterFormat = _RwD3D9AdapterInformation.mode.Format;

    RWFUNCTION(RWSTRING("_rwD3D9CheckValidCameraCubeTextureFormat"));

    hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      D3DUSAGE_RENDERTARGET,
                                      D3DRTYPE_CUBETEXTURE,
                                      (D3DFORMAT)format);

    if (SUCCEEDED(hr))
    {
        hr = IDirect3D9_CheckDepthStencilMatch(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               adapterFormat,
                                               (D3DFORMAT)format,
                                               Present.AutoDepthStencilFormat);
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D9CheckAutoMipmapGenTextureFormat

 On entry   : Check if the texture format supports automipmapgen
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckAutoMipmapGenTextureFormat(RwInt32 format)
{
    RWFUNCTION(RWSTRING("_rwD3D9CheckAutoMipmapGenTextureFormat"));

    if (_RwD3D9DeviceCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
    {
        const D3DFORMAT  adapterFormat = _RwD3D9AdapterInformation.mode.Format;
        HRESULT             hr;

        hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                        _RwD3DAdapterIndex,
                                        _RwD3DAdapterType,
                                        adapterFormat,
                                        D3DUSAGE_AUTOGENMIPMAP,
                                        D3DRTYPE_TEXTURE,
                                        (D3DFORMAT)format);
        RWRETURN(hr == D3D_OK);
    }
    else
    {
        RWRETURN(FALSE);
    }
}

/****************************************************************************
 _rwD3D9CheckAutoMipmapGenCameraTextureFormat

 On entry   : Check if the rendertarget format supports automipmapgen
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckAutoMipmapGenCameraTextureFormat(RwInt32 format)
{
    RWFUNCTION(RWSTRING("_rwD3D9CheckAutoMipmapGenCameraTextureFormat"));

    if (_RwD3D9DeviceCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
    {
        const D3DFORMAT  adapterFormat = _RwD3D9AdapterInformation.mode.Format;
        HRESULT             hr;

        hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                        _RwD3DAdapterIndex,
                                        _RwD3DAdapterType,
                                        adapterFormat,
                                        D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_RENDERTARGET,
                                        D3DRTYPE_TEXTURE,
                                        (D3DFORMAT)format);

        RWRETURN(hr == D3D_OK);
    }
    else
    {
        RWRETURN(FALSE);
    }
}

/****************************************************************************
 _rwD3D9CheckAutoMipmapGenCubeTextureFormat

 On entry   : Check if the cube texture format supports automipmapgen
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CheckAutoMipmapGenCubeTextureFormat(RwInt32 format)
{
    RWFUNCTION(RWSTRING("_rwD3D9CheckAutoMipmapGenCubeTextureFormat"));

    if (_RwD3D9DeviceCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
    {
        const D3DFORMAT adapterFormat = _RwD3D9AdapterInformation.mode.Format;
        HRESULT         hr;

        hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                        _RwD3DAdapterIndex,
                                        _RwD3DAdapterType,
                                        adapterFormat,
                                        D3DUSAGE_AUTOGENMIPMAP,
                                        D3DRTYPE_CUBETEXTURE,
                                        (D3DFORMAT)format);
        RWRETURN(hr == D3D_OK);
    }
    else
    {
        RWRETURN(FALSE);
    }
}

/****************************************************************************
 _rwD3D9CheckAutoMipmapGenCameraCubeTextureFormat

 On entry   : Check if the cube rendertarget format supports automipmapgen
            : Parameter
 On exit    : TRUE on success
 */
RwBool
_rwD3D9CheckAutoMipmapGenCameraCubeTextureFormat(RwInt32 format)
{
    RWFUNCTION(RWSTRING("_rwD3D9CheckAutoMipmapGenCameraCubeTextureFormat"));

    if (_RwD3D9DeviceCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
    {
        const D3DFORMAT  adapterFormat = _RwD3D9AdapterInformation.mode.Format;
        HRESULT             hr;

        hr = IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                        _RwD3DAdapterIndex,
                                        _RwD3DAdapterType,
                                        adapterFormat,
                                        D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_RENDERTARGET,
                                        D3DRTYPE_CUBETEXTURE,
                                        (D3DFORMAT)format);

        RWRETURN(hr == D3D_OK);
    }
    else
    {
        RWRETURN(FALSE);
    }
}

/****************************************************************************
 _rwD3D9CheckValidFormat
 */
static RwBool
_rwD3D9CheckValidFormat(RwUInt32 type,
                        RwBool isCube,
                        RwUInt32 d3dFormat)
{
    RwBool valid;

    RWFUNCTION(RWSTRING("_rwD3D9CheckValidFormat"));

    valid = FALSE;

    switch (type & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
            if (isCube)
            {
                valid = _rwD3D9CheckValidCubeTextureFormat(d3dFormat);
            }
            else
            {
                valid = _rwD3D9CheckValidTextureFormat(d3dFormat);
            }
            break;

        case rwRASTERTYPECAMERATEXTURE:
            if (isCube)
            {
                valid = _rwD3D9CheckValidCameraCubeTextureFormat(d3dFormat);
            }
            else
            {
                valid = _rwD3D9CheckValidCameraTextureFormat(d3dFormat);
            }
            break;

        default:
            break;
    }

    RWRETURN(valid);
}

/****************************************************************************
 _rwD3D9RasterConvertToNonPalettized
 */
void
_rwD3D9RasterConvertToNonPalettized(RwRaster *raster)
{
    RwImage *image;

    RWFUNCTION(RWSTRING("_rwD3D9RasterConvertToNonPalettized"));
    RWASSERT(raster != NULL);

    image = RwImageCreate(raster->width, raster->height, 32);
    if (image != NULL)
    {
        _rwD3D9RasterExt    *rasExt = RASTEREXTFROMRASTER(raster);
        RwUInt32            format;

        RwImageAllocatePixels(image);

        _rwD3D9ImageGetFromRaster(image, raster->parent, 0);

        /* Check pixel format */
        if (rasExt->alpha)
        {
            if (_RwD3D9AdapterInformation.displayDepth > 16)
            {
                if( _rwD3D9CheckValidFormat(raster->cType, rasExt->cube, D3DFMT_A8R8G8B8) )
                {
                    format = rwRASTERFORMAT8888;
                }
                else
                {
                    if( _rwD3D9CheckValidFormat(raster->cType, rasExt->cube, D3DFMT_A4R4G4B4) )
                    {
                        format = rwRASTERFORMAT4444;
                    }
                    else
                    {
                        format = rwRASTERFORMAT1555;
                    }
                }
            }
            else
            {
                if( _rwD3D9CheckValidFormat(raster->cType, rasExt->cube, D3DFMT_A4R4G4B4) )
                {
                    format = rwRASTERFORMAT4444;
                }
                else
                {
                    format = rwRASTERFORMAT1555;
                }
            }
        }
        else
        {
            if (_RwD3D9AdapterInformation.displayDepth > 16)
            {
                if( _rwD3D9CheckValidFormat(raster->cType, rasExt->cube, D3DFMT_X8R8G8B8) )
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

        /* Destroy old texture */
        _rwD3D9RasterDestroy(NULL, raster, 0);

        /* Create a new one */
        _rwD3D9RasterCreate(NULL, raster, (raster->cType & rwRASTERTYPEMASK) |
                                          format |
                                          rwRASTERFORMATAUTOMIPMAP | rwRASTERFORMATMIPMAP);

        _rwD3D9RasterSetFromImage(raster, image, 0);

        RwImageDestroy(image);
    }

    RWRETURNVOID();
}
