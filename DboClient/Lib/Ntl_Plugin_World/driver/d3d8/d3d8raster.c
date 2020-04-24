/***************************************************************************
 *                                                                         *
 * Module  : D3D8raster.c                                                    *
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

#include "d3d8device.h"
#include "drvfns.h"
#include "d3d8convrt.h"
#include "d3d8rendst.h"
#include "d3d8raster.h"
#include "d3d82drend.h"

/****************************************************************************
 Defines
 */

#define IGNORED_D3D8    0

/****************************************************************************
 Globals (across program)
 */


RwInt32 _RwD3D8RasterExtOffset;    /* Raster extension offset */

const _rwRasterConvData _rwRasterConvTable[NUMPIXELFORMATS] =
{
    {D3DFMT_UNKNOWN,       0, 0},   /* rwRASTERFORMATDEFAULT */
    {D3DFMT_A1R5G5B5,     16, 1},   /* rwRASTERFORMAT1555    */
    {D3DFMT_R5G6B5,       16, 0},   /* rwRASTERFORMAT565     */
    {D3DFMT_A4R4G4B4,     16, 1},   /* rwRASTERFORMAT4444    */
    {D3DFMT_L8,            8, 0},   /* rwRASTERFORMATLUM8    */
    {D3DFMT_A8R8G8B8,     32, 1},   /* rwRASTERFORMAT8888    */
    {D3DFMT_X8R8G8B8,     32, 0},   /* rwRASTERFORMAT888     */
    {D3DFMT_UNKNOWN,       0, 0},   /* rwRASTERFORMAT16      */
    {D3DFMT_UNKNOWN,       0, 0},   /* rwRASTERFORMAT24      */
    {D3DFMT_UNKNOWN,       0, 0},   /* rwRASTERFORMAT32      */
    {D3DFMT_X1R5G5B5,     16, 0}    /* rwRASTERFORMAT555     */
};

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Local Types
 */
typedef struct _rxD3D8VideoMemoryRaster RxD3D8VideoMemoryRaster;
struct _rxD3D8VideoMemoryRaster
{
    RwRaster                *raster;
    RxD3D8VideoMemoryRaster *next;
};

/****************************************************************************
 Local (static)
 */

/* Current destination raster */
static RwRaster *_RwD3D8CurrentRasterTarget = NULL;

static RwIm2DVertex _RwD3D8QuadVerts[4];

/* empty index for a new pallete */
static RwUInt32 NumPaletteIndexFree = 0;
static RwUInt32 MaxPaletteIndexFree = 0;
static RwUInt32 MaxPaletteIndex = 0;
static RwUInt16 *PaletteIndexFree = NULL;

static RwFreeList *PaletteFreeList = NULL;

/* Video memory raster list */
static RxD3D8VideoMemoryRaster  *VideoMemoryRasters = NULL;
static RwFreeList               *VideoMemoryRastersFreeList = NULL;

static RwBool rxD3D8VideoMemoryRasterListCreate(void);

static void rxD3D8VideoMemoryRasterListAdd(RwRaster *raster);

static void rxD3D8VideoMemoryRasterListDestroy(void);

#if defined(RWDEBUG)
static RwUInt32  NumVideoMemoryRasters = 0;
static RwUInt32  NumNormalRasters = 0;
static RwUInt32  NumTextureRasters = 0;
static RwUInt32  NumCameraTextureRasters = 0;
static RwUInt32  NumCameraRasters = 0;
static RwUInt32  NumZBufferRasters = 0;
#endif /* defined(RWDEBUG) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           Raster Ctor/Dtor

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 rwD3D8RasterCtor

 On entry   : Object, offset, size
 On exit    : Object pointer on success
 */

static void               *
rwD3D8RasterCtor(void *object,
                 RwInt32 offsetInObject __RWUNUSED__,
                 RwInt32 sizeInObject __RWUNUSED__)
{
    _rwD3D8RasterExt    *rasExt = RASTEREXTFROMRASTER(object);

    RWFUNCTION(RWSTRING("rwD3D8RasterCtor"));
    RWASSERT(object);

    /* These are used to detect when we are using a camera for the first time */
    rasExt->swapChain = NULL;

    RWRETURN(object);
}

/****************************************************************************
 rwD3D8RasterDtor

 On entry   : Object, offset, size
 On exit    : Object pointer on success
 */

static void               *
rwD3D8RasterDtor(void *object,
                 RwInt32 offsetInObject __RWUNUSED__,
                 RwInt32 sizeInObject __RWUNUSED__)
{
    _rwD3D8RasterExt    *rasExt = RASTEREXTFROMRASTER(object);

    RWFUNCTION(RWSTRING("rwD3D8RasterDtor"));
    RWASSERT(object);

    if (rasExt->swapChain)
    {
        IDirect3DSwapChain8_Release(rasExt->swapChain);
        rasExt->swapChain = NULL;
    }

    /* Phew! */
    RWRETURN(object);
}

/****************************************************************************
 _rwD3D8RasterPluginAttach

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8RasterPluginAttach(void)
{
    RWFUNCTION(RWSTRING("_rwD3D8RasterPluginAttach"));

    _RwD3D8RasterExtOffset =
        RwRasterRegisterPlugin(
            sizeof(_rwD3D8RasterExt),
            rwID_DEVICEMODULE,
            rwD3D8RasterCtor,
            rwD3D8RasterDtor,
            NULL);

    if (0 > _RwD3D8RasterExtOffset)
    {
        RWRETURN(FALSE);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RasterOpen

 On entry   :
 On exit    :
 */
void
_rwD3D8RasterOpen(void)
{
    RWFUNCTION(RWSTRING("_rwD3D8RasterOpen"));

    rxD3D8VideoMemoryRasterListCreate();

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
 _rwD3D8RasterClose

 On entry   :
 On exit    :
 */
void
_rwD3D8RasterClose(void)
{
    RWFUNCTION(RWSTRING("_rwD3D8RasterClose"));

    rxD3D8VideoMemoryRasterListDestroy();

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
 rwD3D8FindRwFormat

 On entry   :
 On exit    :
 */
static RwUInt32
rwD3D8FindRwFormat(D3DFORMAT d3d8Format)
{
    RwUInt32 n;

    RWFUNCTION(RWSTRING("rwD3D8FindRwFormat"));

    for (n = 0; n < NUMPIXELFORMATS; n++)
    {
        if (d3d8Format == _rwRasterConvTable[n].format)
        {
            RWRETURN(n << 8);
        }
    }

    RWERROR((E_RW_INVRASTERFORMAT));

    RWRETURN(0);
}

/****************************************************************************
 _rwD3D8RasterHasAlpha

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8RasterHasAlpha(const RwRaster *raster)
{
    const _rwD3D8RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D8RasterHasAlpha"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMCONSTRASTER(raster);

    RWRETURN(rasExt->alpha);
}

/****************************************************************************
 _rwD3D8TextureHasAlpha

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8TextureHasAlpha(const RwTexture *texture)
{
    RWFUNCTION(RWSTRING("_rwD3D8TextureHasAlpha"));
    RWASSERT(texture);

    if (texture->raster != NULL)
    {
        const _rwD3D8RasterExt  *rasExt;

        rasExt = RASTEREXTFROMRASTER(texture->raster);

        RWRETURN(rasExt->alpha);
    }
    else
    {
        RWRETURN(FALSE);
    }
}

/****************************************************************************
 _rwD3D8RasterRemoveAlphaFlag

 On entry   :
 On exit    :
 */
void
_rwD3D8RasterRemoveAlphaFlag(RwRaster *raster)
{
    _rwD3D8RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D8RasterRemoveAlphaFlag"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMRASTER(raster);

    rasExt->alpha = FALSE;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8CubeRasterCreate

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8CubeRasterCreate(RwRaster *raster, RwUInt32 d3dformat, RwInt32 mipmapLevels)
{
    _rwD3D8RasterExt  *rasExt;
    RwUInt32 levels;
    HRESULT hr;

    RWFUNCTION(RWSTRING("_rwD3D8CubeRasterCreate"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMRASTER(raster);

    rasExt->cube = TRUE;

    if (_RwD3D8DeviceCaps.CubeTextureFilterCaps & (D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR))
    {
        levels = mipmapLevels;
    }
    else
    {
        #ifdef RWDEBUG
        if (mipmapLevels > 1)
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D8CubeRasterCreate",
                              "Cube texture mip-mapping not supported");
        }
        #endif

        levels = 1;
    }

    hr = DXCHECK(
        IDirect3DDevice8_CreateCubeTexture(_RwD3DDevice,
                                           raster->width,
                                           levels,
                                           IGNORED_D3D8,
                                           (D3DFORMAT)d3dformat,
                                           D3DPOOL_MANAGED,
                                           (LPDIRECT3DCUBETEXTURE8 *)
                                           &(rasExt->texture)));
    if (FAILED(hr))
    {
        RWRETURN(FALSE);
    }

    /* Remove the rwRASTERDONTALLOCATE flag so it will get destroyed */
    raster->cFlags &= ~rwRASTERDONTALLOCATE;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8CubeRasterSelectFace

 On entry   :
 On exit    :
 */
void
_rwD3D8CubeRasterSelectFace(RwRaster *raster, RwUInt32 face)
{
    _rwD3D8RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D8CubeRasterSelectFace"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMRASTER(raster);
    RWASSERT(rasExt->cube);

    RWASSERT(face < 6);
    rasExt->face = face;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8RasterIsCubeRaster

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8RasterIsCubeRaster(const RwRaster *raster)
{
    const _rwD3D8RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D8RasterIsCubeRaster"));
    RWASSERT(raster);

    rasExt = RASTEREXTFROMCONSTRASTER(raster);

    RWRETURN(rasExt->cube);
}

/****************************************************************************
 _rwD3D8RasterLock

 Locks a raster

 On entry   : NULL
            : raster - raster to allocate
            : Flags, rwRASTERLOCKREAD,
                     rwRASTERLOCKWRITE,
                     rwRASTERLOCKREADWRITE
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterLock(void *pixelsIn, void *rasterIn, RwInt32 accessMode)
{
    RwUInt8         **pixels = (RwUInt8 **)pixelsIn;
    RwRaster        *raster = (RwRaster*)rasterIn;
    RwRaster        *topRaster;
    _rwD3D8RasterExt  *rasExt;
    _rwD3D8RasterExt  *topRasExt;
    RwUInt8         mipLevel;
    RwUInt32        flags;
    HRESULT         hr = D3D_OK;


    RWFUNCTION(RWSTRING("_rwD3D8RasterLock"));

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
                    hr = IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)topRasExt->texture,
                                                                (D3DCUBEMAP_FACES)topRasExt->face,
                                                                mipLevel,
                                                                &(rasExt->lockedSurface));
                }
                else
                {
                    hr = IDirect3DTexture8_GetSurfaceLevel(topRasExt->texture, mipLevel, &(rasExt->lockedSurface));
                }

                if (SUCCEEDED(hr))
                {
                    if (topRaster == raster)
                    {
                        hr = IDirect3DSurface8_LockRect(rasExt->lockedSurface,
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

                        hr = IDirect3DSurface8_LockRect(rasExt->lockedSurface,
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
                        hr = IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)topRasExt->texture,
                                                                    (D3DCUBEMAP_FACES)topRasExt->face,
                                                                    mipLevel,
                                                                    &surface);
                    }
                    else
                    {
                        hr = IDirect3DTexture8_GetSurfaceLevel(topRasExt->texture, mipLevel, &surface);
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
                        surface = _RwD3D8RenderSurface;
                    }

                    hr = D3D_OK;
                }

                if (SUCCEEDED(hr))
                {
                    IDirect3DSurface8_GetDesc(surface, &d3dDesc);

                    /* Create a system memory surface */
                    if (topRaster == raster)
                    {
                        hr = IDirect3DDevice8_CreateImageSurface(_RwD3DDevice, d3dDesc.Width, d3dDesc.Height, d3dDesc.Format, &(rasExt->lockedSurface));
                    }
                    else
                    {
                        hr = IDirect3DDevice8_CreateImageSurface(_RwD3DDevice, raster->width, raster->height, d3dDesc.Format, &(rasExt->lockedSurface));
                    }

                    if (SUCCEEDED(hr))
                    {
                        /* Copy the contents if needed */
                        if (accessMode & rwRASTERLOCKREAD)
                        {
                            if (topRaster == raster)
                            {
                                hr = DXCHECK(IDirect3DDevice8_CopyRects(_RwD3DDevice, surface, NULL, 0, rasExt->lockedSurface, NULL));
                            }
                            else
                            {
                                RECT rect;
                                POINT pt;

                                rect.left = raster->nOffsetX;
                                rect.top = raster->nOffsetY;
                                rect.right = raster->nOffsetX + raster->width;
                                rect.bottom = raster->nOffsetY + raster->height;

                                pt.x = 0;
                                pt.y = 0;

                                hr = DXCHECK(IDirect3DDevice8_CopyRects(_RwD3DDevice, surface, &rect, 1, rasExt->lockedSurface, &pt));
                            }
                        }

                        if (SUCCEEDED(hr))
                        {
                            hr = IDirect3DSurface8_LockRect(rasExt->lockedSurface,
                                                &rasExt->lockedRect,
                                                NULL,
                                                flags);

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
                            IDirect3DSurface8_Release(rasExt->lockedSurface);
                        }
                    }

                    if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
                    {
                        IDirect3DSurface8_Release(surface);
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
 _rwD3D8RasterUnlock

 Unlocks a raster

 On entry   : NULL
            : raster - raster to allocate
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterUnlock(void *unused1 __RWUNUSED__,
                  void *rasterIn,
                  RwInt32 unused3 __RWUNUSED__)
{
    RwRaster        *raster = (RwRaster*)rasterIn;
    _rwD3D8RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D8RasterUnlock"));

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
                IDirect3DSurface8_UnlockRect(rasExt->lockedSurface);

                IDirect3DSurface8_Release(rasExt->lockedSurface);

                /* Restore the original width, height & stride */
                raster->width = raster->originalWidth;
                raster->height = raster->originalHeight;

                raster->stride = 0;
                raster->cpPixels = NULL;

                if ((raster->privateFlags & rwRASTERPIXELLOCKEDWRITE) &&
                    (raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8)) &&
                    (rasExt->lockedMipLevel == 0))
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
                IDirect3DSurface8_UnlockRect(rasExt->lockedSurface);

                /* Update video memory surface if needed */
                if (raster->privateFlags & rwRASTERPIXELLOCKEDWRITE)
                {
                    LPSURFACE           surface;
                    HRESULT             hr;
                    RwRaster            *topRaster;
                    _rwD3D8RasterExt    *topRasExt;


                    /* Get the top level raster as this is the only one with a valid texture */
                    topRaster = raster;
                    while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

                    topRasExt = RASTEREXTFROMRASTER(topRaster);

                    if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
                    {
                        if (topRasExt->cube)
                        {
                            hr = IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)topRasExt->texture,
                                                                        (D3DCUBEMAP_FACES)topRasExt->face,
                                                                        rasExt->lockedMipLevel,
                                                                        &surface);
                        }
                        else
                        {
                            hr = IDirect3DTexture8_GetSurfaceLevel(topRasExt->texture,
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
                            surface = _RwD3D8RenderSurface;
                        }

                        hr = D3D_OK;
                    }

                    if (SUCCEEDED(hr))
                    {
                        if (topRaster == raster)
                        {
                            hr = DXCHECK(IDirect3DDevice8_CopyRects(_RwD3DDevice, rasExt->lockedSurface, NULL, 0, surface, NULL));
                        }
                        else
                        {
                            RECT rect;
                            POINT pt;

                            rect.left = 0;
                            rect.top = 0;
                            rect.right = raster->width;
                            rect.bottom = raster->height;

                            pt.x = raster->nOffsetX;
                            pt.y = raster->nOffsetY;

                            hr = DXCHECK(IDirect3DDevice8_CopyRects(_RwD3DDevice, rasExt->lockedSurface, &rect, 1, surface, &pt));
                        }

                        if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
                        {
                            IDirect3DSurface8_Release(surface);
                        }
                    }

                    IDirect3DSurface8_Release(rasExt->lockedSurface);

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
                    IDirect3DSurface8_Release(rasExt->lockedSurface);

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
 _rwD3D8RasterLockPalette

 On entry   : Pixel pointer
            : raster
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterLockPalette(void *paletteInOut,
                       void *rasterIn,
                       RwInt32 accessMode)
{
    RwRaster    *raster = (RwRaster*)rasterIn;

    RWFUNCTION(RWSTRING("_rwD3D8RasterLockPalette"));

    switch (raster->cType & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            {
                PALETTEENTRY        *palette;
                _rwD3D8RasterExt  *rasExt;

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
 _rwD3D8RasterUnlockPalette

 On entry   : NULL
            : Raster
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterUnlockPalette(void *unused1 __RWUNUSED__,
                         void *rasterIn,
                         RwInt32 unused3 __RWUNUSED__)
{
    RwRaster    *raster = (RwRaster*)rasterIn;

    RWFUNCTION(RWSTRING("_rwD3D8RasterUnlockPalette"));

    switch (raster->cType & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            {
                if ((raster->privateFlags & rwRASTERPALETTELOCKEDWRITE)!=0)
                {
                    _rwD3D8RasterExt  *rasExt;

                    rasExt = RASTEREXTFROMRASTER(raster);

                    DXCHECK(IDirect3DDevice8_SetPaletteEntries(_RwD3DDevice, rasExt->palette->globalindex, (PALETTEENTRY *)(rasExt->palette->entries)));
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
 rwD3D8RasterRenderQuad

 Purpose    : Renders a quad into the current raster target.

 On entry   : Source raster
            : Rect (where image is to be written)
            : scaled (does the src raster need to be scaled 'RwRasterRenderScaled')
            : alpha (does the src raster need to preserve alpha 'RwRasterRenderFast')
 On exit    :
 */
static void
rwD3D8RasterRenderQuad(RwRaster *raster, RwRect *rect,
                      RwBool scaled, RwBool alpha)
{
    RwRaster                *topRasterDest;
    _rwD3D8RasterExt        *rasExtDest;
    LPSURFACE               surfaceLevel;
    LPSURFACE               currentRenderTarget;
    LPSURFACE               currentDepthStencil;
    D3DVIEWPORT8            viewport;
    HRESULT                 hr = D3D_OK;

    RWFUNCTION(RWSTRING("rwD3D8RasterRenderQuad"));

    if (_RwD3D8CurrentRasterTarget->cType == rwRASTERTYPECAMERATEXTURE)
    {
        /* Get the top level destination raster as this is the only one with a valid texture */
        topRasterDest = _RwD3D8CurrentRasterTarget;
        while ((topRasterDest = RwRasterGetParent(topRasterDest)) != RwRasterGetParent(topRasterDest)) {};

        rasExtDest = RASTEREXTFROMRASTER(topRasterDest);

        /* Get the surface level */
        if (rasExtDest->cube)
        {
            IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)rasExtDest->texture,
                                                    (D3DCUBEMAP_FACES)rasExtDest->face,
                                                    0,
                                                    &surfaceLevel);
        }
        else
        {
            IDirect3DTexture8_GetSurfaceLevel(rasExtDest->texture, 0, &surfaceLevel);
        }

        DXCHECK(IDirect3DDevice8_GetRenderTarget(_RwD3DDevice, &currentRenderTarget));
        DXCHECK(IDirect3DDevice8_GetDepthStencilSurface(_RwD3DDevice, &currentDepthStencil));

        if(currentRenderTarget != surfaceLevel)
        {
            D3DSURFACE_DESC d3d8Desc;

            IDirect3DSurface8_GetDesc(surfaceLevel, &d3d8Desc);

            if (d3d8Desc.Usage == D3DUSAGE_RENDERTARGET)
            {
                /* Set the raster as the current target */
                hr = DXCHECK(IDirect3DDevice8_SetRenderTarget(_RwD3DDevice, surfaceLevel, NULL));
            }
            else
            {
                hr = E_FAIL;
            }
        }
        else
        {
            hr = S_OK;
        }
    }

    /*
     * Set the viewport
     */
    viewport.X = _RwD3D8CurrentRasterTarget->nOffsetX;
    viewport.Y = _RwD3D8CurrentRasterTarget->nOffsetY;
    viewport.Width = _RwD3D8CurrentRasterTarget->width;
    viewport.Height = _RwD3D8CurrentRasterTarget->height;
    viewport.MinZ = MINZBUFFERVALUE;
    viewport.MaxZ = MAXZBUFFERVALUE;

    DXCHECK(IDirect3DDevice8_SetViewport(_RwD3DDevice, &viewport));

    /*
     * Begin a scene
     */
    if (_rwD3D8BeginScene())
    {
        RwRaster                *topRaster;
        _rwD3D8RasterExt        *rasExt;
        RwBool                  vertexAlpha;
        RwBlendFunction         srcBlend, dstBlend;
        RwTextureFilterMode     filterMode;
        RwTextureAddressMode    addressModeU;
        RwTextureAddressMode    addressModeV;
        RwBool                  fogEnable;
        RwRaster                *curRaster;
        RwUInt32                oldZEnable;
        RwUInt32                oldCullMode;

        /* Get the top level raster as this is the only one with a valid texture */
        topRaster = raster;
        while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

        rasExt = RASTEREXTFROMRASTER(topRaster);

        /*
         * Calculate the rectangle to render into
         */
        if (scaled)
        {
            _RwD3D8QuadVerts[0].x = (RwReal)(rect->x);
            _RwD3D8QuadVerts[0].y = (RwReal)(rect->y);

            _RwD3D8QuadVerts[1].x = (RwReal)(rect->x);
            _RwD3D8QuadVerts[1].y = (RwReal)(rect->y + rect->h);

            _RwD3D8QuadVerts[2].x = (RwReal)(rect->x + rect->w);
            _RwD3D8QuadVerts[2].y = (RwReal)(rect->y);

            _RwD3D8QuadVerts[3].x = (RwReal)(rect->x + rect->w);
            _RwD3D8QuadVerts[3].y = (RwReal)(rect->y + rect->h);
        }
        else
        {
            _RwD3D8QuadVerts[0].x = (RwReal)(rect->x);
            _RwD3D8QuadVerts[0].y = (RwReal)(rect->y);

            _RwD3D8QuadVerts[1].x = (RwReal)(rect->x);
            _RwD3D8QuadVerts[1].y = (RwReal)(rect->y + raster->height);

            _RwD3D8QuadVerts[2].x = (RwReal)(rect->x + raster->width);
            _RwD3D8QuadVerts[2].y = (RwReal)(rect->y);

            _RwD3D8QuadVerts[3].x = (RwReal)(rect->x + raster->width);
            _RwD3D8QuadVerts[3].y = (RwReal)(rect->y + raster->height);
        }

        /* Calc UVs for sub rasters */
        _RwD3D8QuadVerts[0].z = 0.f;
        _RwD3D8QuadVerts[0].rhw = 1.f;
        _RwD3D8QuadVerts[0].emissiveColor = 0xFFFFFFFF;
        _RwD3D8QuadVerts[0].u = (RwReal)(raster->nOffsetX) / (RwReal)(topRaster->width);
        _RwD3D8QuadVerts[0].v = (RwReal)(raster->nOffsetY) / (RwReal)(topRaster->height);

        _RwD3D8QuadVerts[1].z = 0.f;
        _RwD3D8QuadVerts[1].rhw = 1.f;
        _RwD3D8QuadVerts[1].emissiveColor = 0xFFFFFFFF;
        _RwD3D8QuadVerts[1].u = (RwReal)(raster->nOffsetX) / (RwReal)(topRaster->width);
        _RwD3D8QuadVerts[1].v = (RwReal)(raster->nOffsetY + raster->height) / (RwReal)(topRaster->height);

        _RwD3D8QuadVerts[2].z = 0.f;
        _RwD3D8QuadVerts[2].rhw = 1.f;
        _RwD3D8QuadVerts[2].emissiveColor = 0xFFFFFFFF;
        _RwD3D8QuadVerts[2].u = (RwReal)(raster->nOffsetX + raster->width) / (RwReal)(topRaster->width);
        _RwD3D8QuadVerts[2].v = (RwReal)(raster->nOffsetY) / (RwReal)(topRaster->height);

        _RwD3D8QuadVerts[3].z = 0.f;
        _RwD3D8QuadVerts[3].rhw = 1.f;
        _RwD3D8QuadVerts[3].emissiveColor = 0xFFFFFFFF;
        _RwD3D8QuadVerts[3].u = (RwReal)(raster->nOffsetX + raster->width) / (RwReal)(topRaster->width);
        _RwD3D8QuadVerts[3].v = (RwReal)(raster->nOffsetY + raster->height) / (RwReal)(topRaster->height);

        /* Get some render state */
        _rwD3D8RWGetRenderState(rwRENDERSTATESRCBLEND, (void *)&srcBlend);
        _rwD3D8RWGetRenderState(rwRENDERSTATEDESTBLEND, (void *)&dstBlend);
        _rwD3D8RWGetRenderState(rwRENDERSTATEVERTEXALPHAENABLE, (void *)&vertexAlpha);
        _rwD3D8RWGetRenderState(rwRENDERSTATEFOGENABLE, (void *)&fogEnable);
        _rwD3D8RWGetRenderState(rwRENDERSTATETEXTUREFILTER, (void *)&filterMode);
        _rwD3D8RWGetRenderState(rwRENDERSTATETEXTUREADDRESSU, (void *)&addressModeU);
        _rwD3D8RWGetRenderState(rwRENDERSTATETEXTUREADDRESSV, (void *)&addressModeV);
        _rwD3D8RWGetRenderState(rwRENDERSTATETEXTURERASTER, (void *)&curRaster);

        RwD3D8GetRenderState(D3DRS_ZENABLE, &oldZEnable);
        RwD3D8GetRenderState(D3DRS_CULLMODE, &oldCullMode);

        _rwD3D8RWSetRasterStage(topRaster, 0);

        RwD3D8SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        RwD3D8SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

        if (alpha && rasExt->alpha)
        {
            _rwD3D8RenderStateVertexAlphaEnable(TRUE);
            _rwD3D8RenderStateSrcBlend(rwBLENDSRCALPHA);
            _rwD3D8RenderStateDestBlend(rwBLENDINVSRCALPHA);
        }
        else
        {
            _rwD3D8RenderStateVertexAlphaEnable(FALSE);
        }

        _rwD3D8RWSetRenderState(rwRENDERSTATEFOGENABLE, (void *)FALSE);

        _rwD3D8RWSetRenderState(rwRENDERSTATETEXTUREFILTER,
            (void *)(scaled ? rwFILTERLINEAR : rwFILTERNEAREST));

        _rwD3D8RWSetRenderState(rwRENDERSTATETEXTUREADDRESS, (void *)rwTEXTUREADDRESSCLAMP);

        RwD3D8SetRenderState(D3DRS_ZENABLE, FALSE);
        RwD3D8SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

        /*
         * Render the quad
         */
        _rwD3D8Im2DRenderPrimitive(rwPRIMTYPETRISTRIP,
                                   (RwIm2DVertex *)_RwD3D8QuadVerts,
                                   4);

        /* Restore render state */
        RwD3D8SetRenderState(D3DRS_ZENABLE, oldZEnable);
        RwD3D8SetRenderState(D3DRS_CULLMODE, oldCullMode);

        _rwD3D8RWSetRenderState(rwRENDERSTATESRCBLEND, (void *)srcBlend);
        _rwD3D8RWSetRenderState(rwRENDERSTATEDESTBLEND, (void *)dstBlend);
        _rwD3D8RWSetRenderState(rwRENDERSTATEVERTEXALPHAENABLE, (void *)vertexAlpha);
        _rwD3D8RWSetRenderState(rwRENDERSTATEFOGENABLE, (void *)fogEnable);
        _rwD3D8RWSetRenderState(rwRENDERSTATETEXTUREFILTER, (void *)filterMode);
        _rwD3D8RWSetRasterStage(curRaster, 0);
        _rwD3D8RWSetRenderState(rwRENDERSTATETEXTUREADDRESSU, (void *)addressModeU);
        _rwD3D8RWSetRenderState(rwRENDERSTATETEXTUREADDRESSV, (void *)addressModeV);
    }

    if (_RwD3D8CurrentRasterTarget->cType == rwRASTERTYPECAMERATEXTURE)
    {
        /* If the set render target fails, we need to copy from the back buffer */
        if (FAILED(hr))
        {
            RECT rectSrc;

            rectSrc.left = _RwD3D8CurrentRasterTarget->nOffsetX;
            rectSrc.top = _RwD3D8CurrentRasterTarget->nOffsetY;
            rectSrc.right = rectSrc.left + _RwD3D8CurrentRasterTarget->width;
            rectSrc.bottom = rectSrc.top + _RwD3D8CurrentRasterTarget->height;

            hr = DXCHECK(IDirect3DDevice8_CopyRects(_RwD3DDevice, _RwD3D8RenderSurface, &rectSrc, 1, surfaceLevel, NULL));
        }
        else
        {
            /* Restore the render & depth/stencil surface */
            if(currentRenderTarget != surfaceLevel)
            {
                DXCHECK(IDirect3DDevice8_SetRenderTarget(_RwD3DDevice,
                                        currentRenderTarget, currentDepthStencil));
            }
        }

        IDirect3DSurface8_Release(currentDepthStencil);
        IDirect3DSurface8_Release(currentRenderTarget);

        IDirect3DSurface8_Release(surfaceLevel);
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8RasterCopyQuad

 Purpose    : Renders a quad into the current raster target.

 On entry   : Source raster
            : Rect (where image is to be written)
            : scaled (does the src raster need to be scaled 'RwRasterRenderScaled')
 On exit    : TRUE on success
 */
static RwBool
rwD3D8RasterCopyQuad(RwRaster *raster, RwRect *rect,
                      RwBool scaled)
{
    RwRaster            *topRasterSrc;
    RwRaster            *topRasterDest;
    _rwD3D8RasterExt    *rasExtSrc;
    _rwD3D8RasterExt    *rasExtDest;
    LPDIRECT3DSURFACE8  surfaceSrc;
    LPDIRECT3DSURFACE8  surfaceDest;
    HRESULT             hr;

    RWFUNCTION(RWSTRING("rwD3D8RasterCopyQuad"));

    /* Get the top level raster as this is the only one with a valid texture */
    topRasterSrc = raster;
    while (RwRasterGetParent(topRasterSrc) != topRasterSrc)
    {
        topRasterSrc = RwRasterGetParent(topRasterSrc);
    };

    rasExtSrc = RASTEREXTFROMRASTER(topRasterSrc);

    if (topRasterSrc->cType != rwRASTERTYPECAMERA)
    {
        if (rasExtSrc->cube)
        {
            IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)rasExtSrc->texture,
                                                    (D3DCUBEMAP_FACES)rasExtSrc->face,
                                                    0,
                                                    &surfaceSrc);
        }
        else
        {
            IDirect3DTexture8_GetSurfaceLevel(rasExtSrc->texture, 0, &surfaceSrc);
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
            hr = DXCHECK(IDirect3DDevice8_GetRenderTarget(_RwD3DDevice, &surfaceSrc));
        }
    }

    /* Get the top level raster as this is the only one with a valid texture */
    topRasterDest = _RwD3D8CurrentRasterTarget;
    while (RwRasterGetParent(topRasterDest) != topRasterDest)
    {
        topRasterDest = RwRasterGetParent(topRasterDest);
    };

    rasExtDest = RASTEREXTFROMRASTER(topRasterDest);

    if (topRasterDest->cType != rwRASTERTYPECAMERA)
    {
        if (rasExtDest->cube)
        {
            IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)rasExtDest->texture,
                                                    (D3DCUBEMAP_FACES)rasExtDest->face,
                                                    0,
                                                    &surfaceDest);
        }
        else
        {
            IDirect3DTexture8_GetSurfaceLevel(rasExtDest->texture, 0, &surfaceDest);
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
            hr = DXCHECK(IDirect3DDevice8_GetRenderTarget(_RwD3DDevice, &surfaceDest));
        }
    }

    if ( scaled || (topRasterSrc->cFormat!=topRasterDest->cFormat) )
    {
#if 0
        RECT rectSrc;
        RECT rectDest;

        rectSrc.left = raster->nOffsetX;
        rectSrc.top = raster->nOffsetY;
        rectSrc.right = rectSrc.left + raster->width;
        rectSrc.bottom = rectSrc.top + raster->height;

        if(scaled)
        {
            rectDest.left = _RwD3D8CurrentRasterTarget->nOffsetX + rect->x;
            rectDest.top = _RwD3D8CurrentRasterTarget->nOffsetY + rect->y;
            rectDest.right = rectDest.left + rect->w;
            rectDest.bottom = rectDest.top + rect->h;
        }
        else
        {
            rectDest.left = _RwD3D8CurrentRasterTarget->nOffsetX + rect->x;
            rectDest.top = _RwD3D8CurrentRasterTarget->nOffsetY + rect->y;
            rectDest.right = rectDest.left + raster->width;
            rectDest.bottom = rectDest.top + raster->height;
        }

        hr = D3DXLoadSurfaceFromSurface(surfaceDest,
            (rasExtDest->palette!=NULL ? rasExtDest->palette->entries : NULL),
            &rectDest,
            surfaceSrc,
            (rasExtSrc->palette!=NULL ? rasExtSrc->palette->entries : NULL),
            &rectSrc,
            D3DX_FILTER_POINT, 0);
#else
        hr = E_FAIL;
#endif
    }
    else
    {
        RECT rectSrc;
        POINT pointDest;

        rectSrc.left = raster->nOffsetX;
        rectSrc.top = raster->nOffsetY;
        rectSrc.right = rectSrc.left + raster->width;
        rectSrc.bottom = rectSrc.top + raster->height;

        pointDest.x = _RwD3D8CurrentRasterTarget->nOffsetX + rect->x;
        pointDest.y = _RwD3D8CurrentRasterTarget->nOffsetY + rect->y;

        hr = DXCHECK(IDirect3DDevice8_CopyRects(_RwD3DDevice, surfaceSrc, &rectSrc, 1, surfaceDest, &pointDest));
    }

    if (!rasExtDest->swapChain)
    {
        IDirect3DSurface8_Release(surfaceDest);
    }

    if (!rasExtSrc->swapChain)
    {
        IDirect3DSurface8_Release(surfaceSrc);
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 rwD3D8RasterRenderGeneric

 Purpose    : Renders a raster including alpha.
              No Z buffering take place only a simple blit operation.

 On entry   : Source raster
            : Rect (where image is to be written)
            : Flags
 On exit    :
 */
static RwBool
rwD3D8RasterRenderGeneric(void *rasterIn, void *rectIn,
                         RwBool scaled, RwBool alpha)
{
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwRect      *rect = (RwRect *)rectIn;

    RWFUNCTION(RWSTRING("rwD3D8RasterRenderGeneric"));

    if(!raster)
    {
        RWRETURN(FALSE);
    }

    switch (_RwD3D8CurrentRasterTarget->cType)
    {
        /* DST Raster */
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:

        switch (raster->cType)
        {
            /* SRC Raster */
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
        case rwRASTERTYPECAMERA:
            if (rwD3D8RasterCopyQuad(raster, rect, scaled) == FALSE)
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
        switch (raster->cType)
        {
            /* SRC Raster */
        case rwRASTERTYPECAMERA:
            if (rwD3D8RasterCopyQuad(raster, rect, scaled) == FALSE)
            {
                RWERROR((E_RW_DEVICEERROR,
                    RWSTRING("SRC, DST Raster render combination not supported")));
                RWRETURN(FALSE);
            }
            break;

        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            rwD3D8RasterRenderQuad(raster, rect, scaled, alpha);
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
 _rwD3D8RasterRender

 Purpose    : Renders a raster including alpha.
              No Z buffering take place only a simple blit operation.

 On entry   : Source raster
            : Rect (where image is to be written)
            : Flags
 On exit    :
 */
RwBool
_rwD3D8RasterRender(void *rasterIn,
                  void *rectIn,
                  RwInt32 unused3 __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwD3D8RasterRender"));

    RWRETURN(rwD3D8RasterRenderGeneric((RwRaster *)rasterIn,
                                      (RwRect *)rectIn,
                                      FALSE, TRUE));
}

/****************************************************************************
 _rwD3D8RasterRenderFast

 Purpose    : Renders a raster, ignoring any alpha.
              No Z buffering take place only a simple blit operation.

 On entry   : Raster
            : Rect
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterRenderFast(void *rasterIn,
                      void *rectIn,
                      RwInt32 flags __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwD3D8RasterRenderFast"));

    RWRETURN(rwD3D8RasterRenderGeneric((RwRaster *)rasterIn,
                                      (RwRect *)rectIn,
                                      FALSE, FALSE));
}

/****************************************************************************
 _rwD3D8RasterRenderScaled

 Purpose    : Renders a raster excluding alpha.
              No Z buffering take place only a simple blit operation.

 On entry   : Source raster
            : Rect (where image is to be written)
 On exit    :
 */
RwBool
_rwD3D8RasterRenderScaled(void *rasterIn,
                        void *rectIn,
                        RwInt32 flags __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwD3D8RasterRenderScaled"));

    RWRETURN(rwD3D8RasterRenderGeneric((RwRaster *)rasterIn,
                                      (RwRect *)rectIn,
                                      TRUE, TRUE));
}

/****************************************************************************
 D3D8RasterClearGeneric

 Clear a raster to specific value

 On entry   : rectIn - The rectangle to clear
            : packedColor - Value to clear to
 On exit    : TRUE on success
 */
static RwBool
D3D8RasterClearGeneric(RwRect *rect, RwInt32 packedColor)
{
    RWFUNCTION(RWSTRING("D3D8RasterClearGeneric"));

    switch (_RwD3D8CurrentRasterTarget->cType)
    {
    case rwRASTERTYPECAMERA:
        {
            RwRGBA      unpackedColor;
            RwUInt32    packedColor8888;
            D3DRECT     rectClear;

            if (_RwD3D8CurrentRasterTarget->depth != 32)
            {
                _rwD3D8PixelToRGB(&unpackedColor, &packedColor, RwRasterGetFormat(_RwD3D8CurrentRasterTarget));
                _rwD3D8RGBToPixel(&packedColor8888, &unpackedColor, rwRASTERFORMAT8888);
            }
            else
            {
                packedColor8888 = packedColor;
            }

            /* Initialize the rectangle to clear */
            rectClear.x1 = _RwD3D8CurrentRasterTarget->nOffsetX + rect->x;
            rectClear.y1 = _RwD3D8CurrentRasterTarget->nOffsetY + rect->y;
            rectClear.x2 = rectClear.x1 + rect->w;
            rectClear.y2 = rectClear.y1 + rect->h;

            /* Clear */
            DXCHECK(IDirect3DDevice8_Clear(_RwD3DDevice, 1, &rectClear, D3DCLEAR_TARGET,
                packedColor8888, MAXZBUFFERVALUE, 0));
        }
        break;

    case rwRASTERTYPETEXTURE:
    case rwRASTERTYPENORMAL:
        {
            RwBool      rv = FALSE;
            RwUInt8     *pixels;
            RwInt32     x, y;

            if (_rwD3D8RasterLock(&pixels, _RwD3D8CurrentRasterTarget, rwRASTERLOCKWRITE))
            {
                if (_RwD3D8CurrentRasterTarget->depth == 32)
                {
                    for (y = 0; y < rect->h; y++)
                    {
                        RwUInt32 *pixelDest = (RwUInt32 *)(pixels + (rect->x * 4) + ((y + rect->y) * _RwD3D8CurrentRasterTarget->stride));
                        for (x = 0; x < rect->w; x++)
                        {
                            *pixelDest++ = packedColor;
                        }
                    }

                    rv = TRUE;
                }
                else if (_RwD3D8CurrentRasterTarget->depth == 16)
                {
                    for (y = 0; y < rect->h; y++)
                    {
                        RwUInt16 *pixelDest = (RwUInt16 *)(pixels + (rect->x * 2) + ((y + rect->y) * _RwD3D8CurrentRasterTarget->stride));
                        for (x = 0; x < rect->w; x++)
                        {
                            *pixelDest++ = (RwUInt16)packedColor;
                        }
                    }

                    rv = TRUE;
                }
                else if (_RwD3D8CurrentRasterTarget->depth == 8)
                {
                    RwRaster            *topRaster;
                    _rwD3D8RasterExt    *rasExt;
                    PALETTEENTRY        *palette;
                    RwUInt8             indice;
                    RwUInt32            n, minDist;
                    RwUInt8             *colorComps;

                    /* Get the top level raster as this is the only one with a valid palette */
                    topRaster = _RwD3D8CurrentRasterTarget;
                    while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

                    rasExt = RASTEREXTFROMRASTER(topRaster);

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
                        RwUInt8 *pixelDest = (pixels + (rect->x) + ((y + rect->y) * _RwD3D8CurrentRasterTarget->stride));
                        for (x = 0; x < rect->w; x++)
                        {
                            *pixelDest++ = indice;
                        }
                    }

                    rv = TRUE;
                }

                _rwD3D8RasterUnlock(pixels, _RwD3D8CurrentRasterTarget, rwRASTERLOCKWRITE);
            }

            if (!rv)
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
        }
        break;

    case rwRASTERTYPECAMERATEXTURE:
        {
            RwRGBA          unpackedColor;
            RwUInt32        packedColor8888;
            D3DRECT         rectClear;
            RwRaster        *topRaster;
            _rwD3D8RasterExt  *rasterExt;
            LPSURFACE       surfaceLevel;
            D3DSURFACE_DESC d3d8Desc;
            LPSURFACE       currentRenderTarget;
            LPSURFACE       currentDepthStencil;
            HRESULT         hr;

            if (_RwD3D8CurrentRasterTarget->depth != 32)
            {
                _rwD3D8PixelToRGB(&unpackedColor, &packedColor, RwRasterGetFormat(_RwD3D8CurrentRasterTarget));
                _rwD3D8RGBToPixel(&packedColor8888, &unpackedColor, rwRASTERFORMAT8888);
            }
            else
            {
                packedColor8888 = packedColor;
            }

            /* Get the top level raster as this is the only one with a valid texture */
            topRaster = _RwD3D8CurrentRasterTarget;
            while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

            /* Get the surface level */
            rasterExt = RASTEREXTFROMRASTER(topRaster);

            if (rasterExt->cube)
            {
                IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)rasterExt->texture,
                                                        (D3DCUBEMAP_FACES)rasterExt->face,
                                                        0,
                                                        &surfaceLevel);
            }
            else
            {
                IDirect3DTexture8_GetSurfaceLevel(rasterExt->texture, 0, &surfaceLevel);
            }

            DXCHECK(IDirect3DDevice8_GetRenderTarget(_RwD3DDevice, &currentRenderTarget));
            DXCHECK(IDirect3DDevice8_GetDepthStencilSurface(_RwD3DDevice, &currentDepthStencil));

            if(currentRenderTarget != surfaceLevel)
            {
                IDirect3DSurface8_GetDesc(surfaceLevel, &d3d8Desc);

                if (d3d8Desc.Usage == D3DUSAGE_RENDERTARGET)
                {
                    /* Set the raster as the current target */
                    hr = DXCHECK(IDirect3DDevice8_SetRenderTarget(_RwD3DDevice, surfaceLevel, NULL));
                }
                else
                {
                    hr = E_FAIL;
                }
            }
            else
            {
                hr = S_OK;
            }

            /* Initialize the rectangle to clear */
            rectClear.x1 = _RwD3D8CurrentRasterTarget->nOffsetX + rect->x;
            rectClear.y1 = _RwD3D8CurrentRasterTarget->nOffsetY + rect->y;
            rectClear.x2 = rectClear.x1 + rect->w;
            rectClear.y2 = rectClear.y1 + rect->h;

            /* Clear */
            DXCHECK(IDirect3DDevice8_Clear(_RwD3DDevice, 1, &rectClear, D3DCLEAR_TARGET, packedColor8888, MAXZBUFFERVALUE, 0));

            /* If the set render target fails, we need to copy from the back buffer */
            if (FAILED(hr))
            {
                RECT rectSrc;

                rectSrc.left = _RwD3D8CurrentRasterTarget->nOffsetX;
                rectSrc.top = _RwD3D8CurrentRasterTarget->nOffsetY;
                rectSrc.right = rectSrc.left + _RwD3D8CurrentRasterTarget->width;
                rectSrc.bottom = rectSrc.top + _RwD3D8CurrentRasterTarget->height;

                hr = DXCHECK(IDirect3DDevice8_CopyRects(_RwD3DDevice, _RwD3D8RenderSurface, &rectSrc, 1, surfaceLevel, NULL));
            }
            else
            {
                /* Restore the render & depth/stencil surface */
                if(currentRenderTarget != surfaceLevel)
                {
                    DXCHECK(IDirect3DDevice8_SetRenderTarget(_RwD3DDevice,
                                            currentRenderTarget, currentDepthStencil));
                }
            }

            IDirect3DSurface8_Release(currentDepthStencil);
            IDirect3DSurface8_Release(currentRenderTarget);

            IDirect3DSurface8_Release(surfaceLevel);
        }
        break;

    case rwRASTERTYPEZBUFFER:
        {
            _rwD3D8RasterExt  *rasterExt;
            RwRaster        *topRaster;
            LPSURFACE       surfaceLevel;
            LPSURFACE       currentRenderTarget;
            LPSURFACE   currentDepthStencil;
            D3DRECT     rectClear;

            /* Get the top level raster as this is the only one with a valid texture */
            topRaster = _RwD3D8CurrentRasterTarget;
            while ((topRaster = RwRasterGetParent(topRaster)) != RwRasterGetParent(topRaster)) {};

            /* Get the surface level */
            rasterExt = RASTEREXTFROMRASTER(topRaster);
            surfaceLevel = (LPSURFACE)rasterExt->texture;

            DXCHECK(IDirect3DDevice8_GetRenderTarget(_RwD3DDevice, &currentRenderTarget));
            DXCHECK(IDirect3DDevice8_GetDepthStencilSurface(_RwD3DDevice, &currentDepthStencil));

            if(currentDepthStencil != surfaceLevel)
            {
                /* Set the raster as the current target */
                DXCHECK(IDirect3DDevice8_SetRenderTarget(_RwD3DDevice, currentRenderTarget, surfaceLevel));
            }

            /* Initialize the rectangle to clear */
            rectClear.x1 = _RwD3D8CurrentRasterTarget->nOffsetX + rect->x;
            rectClear.y1 = _RwD3D8CurrentRasterTarget->nOffsetY + rect->y;
            rectClear.x2 = rectClear.x1 + rect->w;
            rectClear.y2 = rectClear.y1 + rect->h;

            /* Clear */
            DXCHECK(IDirect3DDevice8_Clear(_RwD3DDevice, 1, &rectClear, D3DCLEAR_ZBUFFER, 0, MAXZBUFFERVALUE, 0));

            /* Restore the render & depth/stencil surface */
            if(currentDepthStencil != surfaceLevel)
            {
                DXCHECK(IDirect3DDevice8_SetRenderTarget(_RwD3DDevice, currentRenderTarget, currentDepthStencil));
            }

            IDirect3DSurface8_Release(currentDepthStencil);
            IDirect3DSurface8_Release(currentRenderTarget);
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
 _rwD3D8RasterClearRect

 Clear specified area of raster

 On entry   : NULL
              pRect - region of raster to clear
              nValue - value to clear to
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterClearRect(void *unused1 __RWUNUSED__,
                     void *rectIn,
                     RwInt32 packedColor)
{
    RwRect  *rect = (RwRect *)rectIn;

    RWFUNCTION(RWSTRING("_rwD3D8RasterClearRect"));

    RWRETURN(D3D8RasterClearGeneric(rect, packedColor));
}

/****************************************************************************
 _rwD3D8RasterClear

 Clear a raster to specific value

 On entry   : NULL
            : NULL
            : packedColor - value to clear to
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterClear(void *unused1 __RWUNUSED__,
                 void *unused2 __RWUNUSED__,
                 RwInt32 packedColor)
{
    RwRect  rect;

    RWFUNCTION(RWSTRING("_rwD3D8RasterClear"));

    rect.x = 0;
    rect.y = 0;
    rect.w = _RwD3D8CurrentRasterTarget->width;
    rect.h = _RwD3D8CurrentRasterTarget->height;

    RWRETURN(D3D8RasterClearGeneric(&rect, packedColor));
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           Setting the context

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwD3D8SetRasterContext(void *out, void *rasIn, RwInt32 flags)

 Purpose    : Set the current raster as the destination raster to which
              raster operations will be applied.

 On entry   : rasIn - raster to do all subsequent rendering to
 On exit    : TRUE on success
 */
RwBool
_rwD3D8SetRasterContext(void *unused1 __RWUNUSED__,
                      void *rasIn,
                      RwInt32 unused3 __RWUNUSED__)
{
    RwRaster *raster = (RwRaster *)rasIn;

    RWFUNCTION(RWSTRING("_rwD3D8SetRasterContext"));

    _RwD3D8CurrentRasterTarget = raster;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8FindCorrectRasterFormat

 Puprose    : Finds the rasters pixel format

 On entry   : flags -

 On exit    : correct pixel format
 */
RwInt32
_rwD3D8FindCorrectRasterFormat(RwRasterType type, RwInt32 flags)
{
    RwUInt32    format = flags & rwRASTERFORMATMASK;

    RWFUNCTION(RWSTRING("_rwD3D8FindCorrectRasterFormat"));

    switch (type)
    {
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            /* Check if we are requesting a default pixel format palette texture */
            if (format & rwRASTERFORMATPAL8)
            {
                if (_rwD3D8CheckValidTextureFormat(D3DFMT_P8))
                {
                    if (_RwD3D8DeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE)
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
                if( _rwD3D8CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                {
                    format |= rwRASTERFORMAT8888;
                }
                else
                {
                    if( _rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
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
                if( !_rwD3D8CheckValidTextureFormat(D3DFMT_P8) ||
                    ( ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888) &&
                      ((_RwD3D8DeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE) == 0)) )
                {
                    /* Change it to a default format */
                    format &= (~rwRASTERFORMATPAL8);

                    if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888)
                    {
                        if (_RwD3D8AdapterInformation.displayDepth > 16)
                        {
                            if( _rwD3D8CheckValidTextureFormat(D3DFMT_X8R8G8B8) )
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
                        if (_RwD3D8AdapterInformation.displayDepth > 16)
                        {
                            if( _rwD3D8CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                            {
                                format |= rwRASTERFORMAT8888;
                            }
                            else
                            {
                                format &= (~rwRASTERFORMAT8888);

                                if( _rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
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

                            if( _rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
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
                    if(!_rwD3D8CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                    {
                        format &= (~rwRASTERFORMAT8888);

                        if( _rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
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
                    if(!_rwD3D8CheckValidTextureFormat(D3DFMT_X8R8G8B8) )
                    {
                        format &= (~rwRASTERFORMAT888);

                        format |= rwRASTERFORMAT565;
                    }
                }
                else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT4444)
                {
                    if( !_rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
                    {
                        format &= (~rwRASTERFORMAT4444);

                        format |= rwRASTERFORMAT1555;
                    }
                }
                else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATLUM8)
                {
                    if(!_rwD3D8CheckValidTextureFormat(D3DFMT_L8) )
                    {
                        format &= (~rwRASTERFORMATLUM8);

                        if( _rwD3D8CheckValidTextureFormat(D3DFMT_X8R8G8B8) )
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
            if (16 == _RwD3D8ZBufferDepth)
            {
                format |= rwRASTERFORMAT565;
            }
            else
            {
                if(_rwD3D8CheckValidCameraTextureFormat(D3DFMT_X8R8G8B8))
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

            if (16 == _RwD3D8ZBufferDepth)
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
                    if( !_rwD3D8CheckValidCameraTextureFormat(D3DFMT_A4R4G4B4) )
                    {
                        format &= (~rwRASTERFORMAT4444);

                        format |= rwRASTERFORMAT1555;
                    }
                }

                if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT1555)
                {
                    if( !_rwD3D8CheckValidCameraTextureFormat(D3DFMT_A1R5G5B5) )
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
                    if(!_rwD3D8CheckValidCameraTextureFormat(D3DFMT_A8R8G8B8))
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
            format = rwD3D8FindRwFormat(_RwD3D8AdapterInformation.mode.Format);
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
            if (16 == _RwD3D8ZBufferDepth)
            {
                format = rwRASTERFORMAT16;
            }
            else if (32 == _RwD3D8ZBufferDepth)
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
 _rwD3D8CheckRasterFormat

 Puprose    : Finds the rasters pixel format & depth.

 On entry   : raster - raster to allocate
            : flags -

 On exit    : TRUE on success
 */
RwBool
_rwD3D8CheckRasterFormat(void *rasterIn, RwInt32 flags)
{
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwUInt32    format = flags & rwRASTERFORMATMASK;

    RWFUNCTION(RWSTRING("_rwD3D8CheckRasterFormat"));

    /* Copy over types */
    raster->cType = (RwUInt8)(flags & rwRASTERTYPEMASK);
    raster->cFlags = (RwUInt8)(flags & ~rwRASTERTYPEMASK);

    format = (RwRasterType)
        _rwD3D8FindCorrectRasterFormat((RwRasterType)raster->cType,
                                       flags);

    raster->cFormat = (RwUInt8)(format >> 8);

    raster->depth = _rwRasterConvTable[FMT2TBL(format)].depth;

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8SetRasterFormat

 Puprose    : Sets the rasters pixel format & depth.

 On entry   : raster - raster to allocate
            : flags -

 On exit    : TRUE on success
 */
static RwBool
rwD3D8SetRasterFormat(void *rasterIn, RwInt32 flags)
{
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwUInt32    format = flags & rwRASTERFORMATMASK;

    RWFUNCTION(RWSTRING("rwD3D8SetRasterFormat"));

    /* Copy over types */
    raster->cType = (RwUInt8)(flags & rwRASTERTYPEMASK);
    raster->cFlags = (RwUInt8)(flags & ~rwRASTERTYPEMASK);

    switch (raster->cType)
    {
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            /* Check if we are requesting a default pixel format palette texture */
            if (format & rwRASTERFORMATPAL8)
            {
                if (_rwD3D8CheckValidTextureFormat(D3DFMT_P8))
                {
                    if (_RwD3D8DeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE)
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
                if (_RwD3D8AdapterInformation.displayDepth > 16)
                {
                    if( _rwD3D8CheckValidTextureFormat(D3DFMT_A8R8G8B8) )
                    {
                        format |= rwRASTERFORMAT8888;

                        raster->depth = 32;
                    }
                    else
                    {
                        if( _rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
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
                    if( _rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4) )
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
                    if( _rwD3D8CheckValidTextureFormat(D3DFMT_P8) &&
                        ( ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888) ||
                          (((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888) &&
                          (_RwD3D8DeviceCaps.TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE) )) )
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
                    if( _rwD3D8CheckValidTextureFormat(_rwRasterConvTable[FMT2TBL(format)].format) )
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
        break;

    case rwRASTERTYPECAMERATEXTURE:
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            /*
             * Raster should match back buffer bit depth
             */
            if (16 == _RwD3D8AdapterInformation.displayDepth)
            {
                format |= rwRASTERFORMAT565;

                raster->depth = 16;
            }
            else
            {
                if(_rwD3D8CheckValidCameraTextureFormat(D3DFMT_X8R8G8B8))
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

            if(_rwD3D8CheckValidCameraTextureFormat(_rwRasterConvTable[FMT2TBL(format)].format))
            {
                raster->depth = depth;
            }
            else
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
        }
        break;

    case rwRASTERTYPECAMERA:
        /* Always force default */
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            format = rwD3D8FindRwFormat(_RwD3D8AdapterInformation.mode.Format);
        }
        else
        {
            const RwUInt32 mainCameraFormat =
                     rwD3D8FindRwFormat(_RwD3D8AdapterInformation.mode.Format);

            if (mainCameraFormat != format)
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
        }

        raster->depth = _RwD3D8AdapterInformation.displayDepth;
        break;

    case rwRASTERTYPEZBUFFER:
        /* Always force default */
        if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMATDEFAULT)
        {
            if (16 == _RwD3D8ZBufferDepth)
            {
                raster->depth = 16;
                format = rwRASTERFORMAT16;
            }
            else if (32 == _RwD3D8ZBufferDepth)
            {
                raster->depth = 32;
                format = rwRASTERFORMAT32;
            }
        }
        else
        {
            if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT16 && _RwD3D8ZBufferDepth == 16)
            {
                raster->depth = 16;
            }
            else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT32 && _RwD3D8ZBufferDepth == 32)
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
 rxD3D8VideoMemoryRasterListAdd

 Add current raster to the video memory list

 On entry   : raster - Raster to add
 */
static void
rxD3D8VideoMemoryRasterListAdd(RwRaster *raster)
{
    RxD3D8VideoMemoryRaster *videoRaster;

    RWFUNCTION(RWSTRING("rxD3D8VideoMemoryRasterListAdd"));

    videoRaster = (RxD3D8VideoMemoryRaster *)
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
 rxD3D8VideoMemoryRasterListRemove

 Remove current raster of the video memory list

 On entry   : raster - Raster to add
 */
static void
rxD3D8VideoMemoryRasterListRemove(RwRaster *raster)
{
    RxD3D8VideoMemoryRaster *currentVideoRaster;
    RxD3D8VideoMemoryRaster *prevVideoRaster;

    RWFUNCTION(RWSTRING("rxD3D8VideoMemoryRasterListRemove"));

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
 rxD3D8VideoMemoryRasterListCreate

 Creates the list

  */
static RwBool
rxD3D8VideoMemoryRasterListCreate(void)
{
    RWFUNCTION(RWSTRING("rxD3D8VideoMemoryRasterListCreate"));

    VideoMemoryRastersFreeList = RwFreeListCreate(
                                        sizeof(RxD3D8VideoMemoryRaster),
                                        127,    /* It results in ~1024 Bytes */
                                        4,
                                        rwID_DRIVERMODULE |
                                        rwMEMHINTDUR_GLOBAL);

    RWRETURN(VideoMemoryRastersFreeList != NULL);
}

/****************************************************************************
 rxD3D8VideoMemoryRasterListDestroy

 Destroy the list

  */
static void
rxD3D8VideoMemoryRasterListDestroy(void)
{
    RWFUNCTION(RWSTRING("rxD3D8VideoMemoryRasterListDestroy"));

#if defined(RWDEBUG)
    RWASSERT(NumVideoMemoryRasters == 0);
#endif /* defined(RWDEBUG) */

    while (VideoMemoryRasters)
    {
        RxD3D8VideoMemoryRaster *next = VideoMemoryRasters->next;

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
 _rxD3D8VideoMemoryRasterListRelease

 Release all the video memory

  */
void
_rxD3D8VideoMemoryRasterListRelease(void)
{
    RxD3D8VideoMemoryRaster *currentVideoRaster;

    RWFUNCTION(RWSTRING("_rxD3D8VideoMemoryRasterListRelease"));

    currentVideoRaster = VideoMemoryRasters;
    while (currentVideoRaster)
    {
        RwRaster                *raster = currentVideoRaster->raster;
        _rwD3D8RasterExt        *rasExt = RASTEREXTFROMRASTER(raster);
        RxD3D8VideoMemoryRaster *next = currentVideoRaster->next;

        if (raster->parent == raster)
        {
            switch(raster->cType)
            {
                case rwRASTERTYPECAMERATEXTURE:
                {
                    if (rasExt->texture)
                    {
                        IDirect3DTexture8_Release(rasExt->texture);
                        rasExt->texture = NULL;
                    }
                }
                break;

                case rwRASTERTYPEZBUFFER:
                {
                    if (rasExt->texture)
                    {
                        if ((LPSURFACE)rasExt->texture != _RwD3D8DepthStencilSurface)
                        {
                            IDirect3DSurface8_Release((LPSURFACE)rasExt->texture);
                            rasExt->texture = NULL;
                        }
                    }
                }
                break;

                case rwRASTERTYPECAMERA:
                {
                    if (rasExt->swapChain)
                    {
                        IDirect3DSwapChain8_Release(rasExt->swapChain);
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
 _rxD3D8VideoMemoryRasterListRestore

 Release all the video memory

  */
RwBool
_rxD3D8VideoMemoryRasterListRestore(void)
{
    RwBool                  result;
    HRESULT                 hr;
    RxD3D8VideoMemoryRaster *currentVideoRaster;

    RWFUNCTION(RWSTRING("_rxD3D8VideoMemoryRasterListRestore"));

#if defined(RWDEBUG)
    {
        RwChar          buffer[256];

        rwsprintf(buffer, "NumVideoMemoryRasters: %d", NumVideoMemoryRasters);

        RwDebugSendMessage(rwDEBUGMESSAGE, "_rxD3D8VideoMemoryRasterListRestore", buffer);
    }
#endif

    result = TRUE;
    hr = D3D_OK;
    currentVideoRaster = VideoMemoryRasters;
    while (currentVideoRaster)
    {
        RwRaster                *raster = currentVideoRaster->raster;
        _rwD3D8RasterExt        *rasExt = RASTEREXTFROMRASTER(raster);
        RxD3D8VideoMemoryRaster *next = currentVideoRaster->next;

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
                                hr = DXCHECK(IDirect3DDevice8_SetPaletteEntries(_RwD3DDevice,
                                                                   rasExt->palette->globalindex,
                                                                   (PALETTEENTRY *)(rasExt->palette->entries)));
                            }
                        }
                    }
                    break;

                case rwRASTERTYPECAMERATEXTURE:
                    {
                        if (rasExt->texture == NULL)
                        {
                            RwUInt32    rasFormat = RwRasterGetFormat(raster);
                            RwUInt32    colorFormat = _rwRasterConvTable[FMT2TBL(rasFormat)].format;
                            RwUInt32    levels = ((rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1);

                            if (rasExt->cube)
                            {
                                hr = DXCHECK(IDirect3DDevice8_CreateCubeTexture(_RwD3DDevice,
                                                                                raster->width,
                                                                                levels,
                                                                                D3DUSAGE_RENDERTARGET,
                                                                                (D3DFORMAT)colorFormat,
                                                                                D3DPOOL_DEFAULT,
                                                                                (LPDIRECT3DCUBETEXTURE8 *)&(rasExt->texture)));
                            }
                            else
                            {
                                hr = DXCHECK(IDirect3DDevice8_CreateTexture(_RwD3DDevice,
                                                               raster->width,
                                                               raster->height,
                                                               levels,
                                                               D3DUSAGE_RENDERTARGET,
                                                               (D3DFORMAT)colorFormat,
                                                               D3DPOOL_DEFAULT,
                                                               &(rasExt->texture)));
                            }

                            if (FAILED(hr))
                            {
                                if ( hr != D3DERR_OUTOFVIDEOMEMORY )
                                {
                                    if (rasExt->cube)
                                    {
                                        hr = DXCHECK(IDirect3DDevice8_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                                                    (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                                                    0,
                                                                                    _RwD3D8AdapterInformation.mode.Format,
                                                                                    D3DPOOL_DEFAULT,
                                                                                    (LPDIRECT3DCUBETEXTURE8 *)&(rasExt->texture)));
                                    }
                                    else
                                    {
                                        hr = DXCHECK(IDirect3DDevice8_CreateTexture(_RwD3DDevice, raster->width,
                                                                   raster->height,
                                                                   (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                                   0,
                                                                   _RwD3D8AdapterInformation.mode.Format,
                                                                   D3DPOOL_DEFAULT,
                                                                   &(rasExt->texture)));
                                    }

                                    raster->cFormat = (RwUInt8)(rwD3D8FindRwFormat(_RwD3D8AdapterInformation.mode.Format) >> 8);
                                    raster->depth = _RwD3D8AdapterInformation.displayDepth;
                                }
                            }
                        }
                    }
                    break;

                case rwRASTERTYPEZBUFFER:
                    {
                        if (rasExt->texture == NULL)
                        {
                            D3DFORMAT   depthFormat = (D3DFORMAT) 0;

                        /* Find the Z-Buffer depth */
                            if (16 == _RwD3D8ZBufferDepth)
                            {
                                if( _rwD3D8CheckValidZBufferFormat(D3DFMT_D15S1) )
                                {
                                    depthFormat = D3DFMT_D15S1;
                                }
                                else
                                {
                                    depthFormat = D3DFMT_D16;
                                }
                            }
                            else if (32 == _RwD3D8ZBufferDepth)
                            {
                                if( _rwD3D8CheckValidZBufferFormat(D3DFMT_D24S8) )
                                {
                                    depthFormat = D3DFMT_D24S8;
                                }
                                else if( _rwD3D8CheckValidZBufferFormat(D3DFMT_D24X4S4) )
                                {
                                    depthFormat = D3DFMT_D24X4S4;
                                }
                                else if( _rwD3D8CheckValidZBufferFormat(D3DFMT_D32) )
                                {
                                    depthFormat = D3DFMT_D32;
                                }
                                else
                                {
                                    depthFormat = D3DFMT_D24X8;
                                }
                            }

                            hr = DXCHECK(IDirect3DDevice8_CreateDepthStencilSurface(_RwD3DDevice,
                                                                       max(1, raster->width),
                                                                       max(1, raster->height),
                                                                       depthFormat,
                                                                       D3DMULTISAMPLE_NONE,
                                                                       (LPDIRECT3DSURFACE8 *)(&(rasExt->texture))));
                        }
                        else
                        {
                            RECT rect;

                            GetClientRect(WindowHandle, &rect);

                            rasExt->texture = (LPDIRECT3DTEXTURE8)_RwD3D8DepthStencilSurface;

                            raster->width = rect.right;
                            raster->height = rect.bottom;
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

                            if(D3D_OK == DXCHECK(IDirect3DDevice8_CreateAdditionalSwapChain(_RwD3DDevice, &present, &(rasExt->swapChain))))
                            {
                                IDirect3DSurface8** ppBackBuffer =
                                    (IDirect3DSurface8**) &(rasExt->texture);

                                hr = IDirect3DSwapChain8_GetBackBuffer(rasExt->swapChain, 0, D3DBACKBUFFER_TYPE_MONO, ppBackBuffer);

                                IDirect3DSurface8_Release((LPSURFACE)rasExt->texture);
                            }
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
 rwD3D8CreateTextureRaster

 Create a texture raster

 On entry   : raster - Raster to allocate
 On exit    : TRUE on success
 */
static RwBool
rwD3D8CreateTextureRaster(RwRaster *raster,
                         _rwD3D8RasterExt *rasExt)
{
    RwUInt32    rasFormat;
    RwUInt32    pixelFormat;
    D3DFORMAT   colorFormat;
    HRESULT     hr;

    RWFUNCTION(RWSTRING("rwD3D8CreateTextureRaster"));

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
            PaletteFreeList = RwFreeListCreate(sizeof(_rwD3D8Palette), 64, 4,
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
        }

        rasExt->palette = (_rwD3D8Palette *)RwFreeListAlloc(PaletteFreeList,
            rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

        if(rasExt->palette)
        {
            rasExt->palette->globalindex = FindAvailablePaletteIndex();
        }

        colorFormat = D3DFMT_P8;
    }
    else
    {
        colorFormat = _rwRasterConvTable[FMT2TBL(rasFormat)].format;
    }

    /* Does this raster format have an alpha component */
    rasExt->alpha = _rwRasterConvTable[FMT2TBL(rasFormat)].alpha;

    /* Check the size of the texture */
    _rwD3D8CheckRasterSize(&(raster->width), &(raster->height), raster->cType | rasFormat);

    /* Create the texture, if mipped 0 gets all the mip levels */
    if (rasExt->cube)
    {
        hr = DXCHECK(IDirect3DDevice8_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                    (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                    IGNORED_D3D8,
                                                    colorFormat,
                                                    D3DPOOL_MANAGED,
                                                    (LPDIRECT3DCUBETEXTURE8 *)&(rasExt->texture)));
    }
    else
    {
        hr = DXCHECK(IDirect3DDevice8_CreateTexture(_RwD3DDevice, raster->width,
                                                    raster->height,
                                                    (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                    IGNORED_D3D8,
                                                    colorFormat,
                                                    D3DPOOL_MANAGED,
                                                    &(rasExt->texture)));
    }

    if (FAILED(hr))
    {
        if (D3DFMT_P8 == colorFormat)
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

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8CreateCameraTextureRaster

 Create a texture raster

 On entry   : raster - Raster to allocate
 On exit    : TRUE on success
 */
static RwBool
rwD3D8CreateCameraTextureRaster(RwRaster *raster, _rwD3D8RasterExt *rasExt)
{
    RwUInt32    rasFormat;
    RwUInt32    pixelFormat;
    D3DFORMAT   colorFormat = (D3DFORMAT) 0;
    HRESULT     hr;

    RWFUNCTION(RWSTRING("rwD3D8CreateCameraTextureRaster"));

    rasFormat = RwRasterGetFormat(raster);
    pixelFormat = rasFormat & rwRASTERFORMATPIXELFORMATMASK;

    /* rwRASTERTYPECAMERATEXTURE can not be palletized */
    if ((rasFormat & rwRASTERFORMATPAL4) || (rasFormat & rwRASTERFORMATPAL8))
    {
        RWERROR((E_RW_DEVICEERROR,
            RWSTRING("rwRASTERTYPECAMERATEXTURE can not be palletized.")));
        RWRETURN(FALSE);
    }

    colorFormat = _rwRasterConvTable[FMT2TBL(rasFormat)].format;

    /* Does this raster format have an alpha component */
    rasExt->alpha = _rwRasterConvTable[FMT2TBL(rasFormat)].alpha;

    /* Check the size of the texture */
    _rwD3D8CheckRasterSize(&(raster->width), &(raster->height), raster->cType | rasFormat);

    /* Create the texture, if mipped 0 gets all the mip levels */
    if (rasExt->cube)
    {
        hr = DXCHECK(IDirect3DDevice8_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                    (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                    D3DUSAGE_RENDERTARGET,
                                                    colorFormat,
                                                    D3DPOOL_DEFAULT,
                                                    (LPDIRECT3DCUBETEXTURE8 *)&(rasExt->texture)));
    }
    else
    {
        hr = IDirect3DDevice8_CreateTexture(_RwD3DDevice, raster->width,
                                          raster->height,
                                          (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                          D3DUSAGE_RENDERTARGET,
                                          colorFormat,
                                          D3DPOOL_DEFAULT,
                                          &(rasExt->texture));
    }

    if ( hr == D3DERR_OUTOFVIDEOMEMORY )
    {
        /* Lets try again after removing all the video memory used by managed textures */
        IDirect3DDevice8_ResourceManagerDiscardBytes(_RwD3DDevice, 0);

        if (rasExt->cube)
        {
            hr = DXCHECK(IDirect3DDevice8_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                        (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                        D3DUSAGE_RENDERTARGET,
                                                        colorFormat,
                                                        D3DPOOL_DEFAULT,
                                                        (LPDIRECT3DCUBETEXTURE8 *)&(rasExt->texture)));
        }
        else
        {
            hr = IDirect3DDevice8_CreateTexture(_RwD3DDevice, raster->width,
                                              raster->height,
                                              (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                              D3DUSAGE_RENDERTARGET,
                                              colorFormat,
                                              D3DPOOL_DEFAULT,
                                              &(rasExt->texture));
        }
    }

    if ( hr == D3DERR_OUTOFVIDEOMEMORY )
    {
        RWERROR((E_RW_DEVICEERROR, RWSTRING("Out of video memory. Failed to create raster.")));
        RWRETURN(FALSE);
    }
    else if (FAILED(hr))
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "rwD3D8CreateCameraTextureRaster", "Device doesn't support camera textures, using normal textures. Backbuffer will be used for rendering");

        if (rasExt->cube)
        {
            hr = DXCHECK(IDirect3DDevice8_CreateCubeTexture(_RwD3DDevice, raster->width,
                                                            (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                            0,
                                                            colorFormat,
                                                            D3DPOOL_DEFAULT,
                                                            (LPDIRECT3DCUBETEXTURE8 *)&(rasExt->texture)));
        }
        else
        {
            hr = DXCHECK(IDirect3DDevice8_CreateTexture(_RwD3DDevice, raster->width,
                                                        raster->height,
                                                        (rasFormat & rwRASTERFORMATMIPMAP) ? 0 : 1,
                                                        0,
                                                        _RwD3D8AdapterInformation.mode.Format,
                                                        D3DPOOL_DEFAULT,
                                                        &(rasExt->texture)));
        }

        if (FAILED(hr))
        {
            RWERROR((E_RW_DEVICEERROR, RWSTRING("Failed to create raster.")));
            RWRETURN(FALSE);
        }
        else
        {
            raster->cFormat = (RwUInt8)(rwD3D8FindRwFormat(_RwD3D8AdapterInformation.mode.Format) >> 8);
            raster->depth = _RwD3D8AdapterInformation.displayDepth;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8CreateZBufferRaster

 Create a Z-Buffer

 On entry   : raster - Raster to allocate
 On exit    : TRUE on success
 */
static RwBool
rwD3D8CreateZBufferRaster(RwRaster *raster, _rwD3D8RasterExt *rasExt)
{
    RECT        rect;
    D3DFORMAT   depthFormat = (D3DFORMAT) 0;

    RWFUNCTION(RWSTRING("rwD3D8CreateZBufferRaster"));

    /* Find the Z-Buffer depth */
    if (16 == _RwD3D8ZBufferDepth)
    {
        if( _rwD3D8CheckValidZBufferFormat(D3DFMT_D15S1) )
        {
            depthFormat = D3DFMT_D15S1;
        }
        else
        {
            depthFormat = D3DFMT_D16;
        }
    }
    else if (32 == _RwD3D8ZBufferDepth)
    {
        if( _rwD3D8CheckValidZBufferFormat(D3DFMT_D24S8) )
        {
            depthFormat = D3DFMT_D24S8;
        }
        else if( _rwD3D8CheckValidZBufferFormat(D3DFMT_D24X4S4) )
        {
            depthFormat = D3DFMT_D24X4S4;
        }
        else if( _rwD3D8CheckValidZBufferFormat(D3DFMT_D32) )
        {
            depthFormat = D3DFMT_D32;
        }
        else
        {
            depthFormat = D3DFMT_D24X8;
        }
    }

    rasExt->alpha = 0;

    GetClientRect(WindowHandle, &rect);

    if (rect.right != raster->width || rect.bottom != raster->height )
    {
        HRESULT hr;

        hr = IDirect3DDevice8_CreateDepthStencilSurface(_RwD3DDevice,
                                                raster->width,
                                                raster->height,
                                                depthFormat,
                                                D3DMULTISAMPLE_NONE,
                                                (LPDIRECT3DSURFACE8 *)(&(rasExt->texture)));

        if ( hr == D3DERR_OUTOFVIDEOMEMORY )
        {
            /* Lets try again after removing all the video memory used by managed textures */
            IDirect3DDevice8_ResourceManagerDiscardBytes(_RwD3DDevice, 0);

            hr = IDirect3DDevice8_CreateDepthStencilSurface(_RwD3DDevice,
                                                    raster->width,
                                                    raster->height,
                                                    depthFormat,
                                                    D3DMULTISAMPLE_NONE,
                                                    (LPDIRECT3DSURFACE8 *)(&(rasExt->texture)));
        }

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
        rasExt->texture = (LPDIRECT3DTEXTURE8)_RwD3D8DepthStencilSurface;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RasterCreate

 Create a raster

 On entry   : NULL
            : raster - raster to allocate
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterCreate(void *unused1 __RWUNUSED__, void *rasterIn, RwInt32 flags)
{
    RwRaster        *raster = (RwRaster *)rasterIn;
    _rwD3D8RasterExt  *rasExt = RASTEREXTFROMRASTER(raster);

    RWFUNCTION(RWSTRING("_rwD3D8RasterCreate"));

    /* Initialise structure to something sensible */
    raster->cType    = (RwUInt8)(flags & rwRASTERTYPEMASK);
    raster->cFlags   = (RwUInt8)(flags & ~rwRASTERTYPEMASK);
    raster->cpPixels = NULL;
    raster->palette  = NULL;

    rasExt->texture = NULL;
    rasExt->palette = NULL;
    rasExt->alpha = 0;
    rasExt->cube = 0;
    rasExt->face = 0;
    rasExt->lockedSurface = NULL;
    rasExt->lockedMipLevel = (RwUInt8)-1;
    rasExt->dxtFormat = 0;

    /* Set up the raster format and depth */
    if (!rwD3D8SetRasterFormat(raster, flags))
    {
        RWRETURN(FALSE);
    }

    /*
     * If it is not a camera buffer, then we need to allocate real memory for it
     */
    if ((raster->width) && (raster->height))
    {
        switch (raster->cType)
        {
            case rwRASTERTYPENORMAL:
            {
                if (!(raster->cFlags & rwRASTERDONTALLOCATE))
                {
                    #if defined(RWDEBUG)
                    NumNormalRasters++;
                    #endif /* defined(RWDEBUG) */

                    if (!rwD3D8CreateTextureRaster(raster, rasExt))
                    {
                        RWRETURN(FALSE);
                    }

                    if (rasExt->palette != NULL)
                    {
                        rxD3D8VideoMemoryRasterListAdd(raster);
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

                    if (!rwD3D8CreateTextureRaster(raster, rasExt))
                    {
                        RWRETURN(FALSE);
                    }

                    if (rasExt->palette != NULL)
                    {
                        rxD3D8VideoMemoryRasterListAdd(raster);
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

                    if (!rwD3D8CreateCameraTextureRaster(raster, rasExt))
                    {
                        RWRETURN(FALSE);
                    }

                    rxD3D8VideoMemoryRasterListAdd(raster);
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

                    if (!rwD3D8CreateZBufferRaster(raster, rasExt))
                    {
                        RWRETURN(FALSE);
                    }

                    rxD3D8VideoMemoryRasterListAdd(raster);
                }
            }
            break;

            case rwRASTERTYPECAMERA:
            {
                RECT                rect;

                GetClientRect(WindowHandle, &rect);

                if ( (raster->cFlags & rwRASTERDONTALLOCATE) != 0 ||
                    (rect.right >= raster->width && rect.bottom >= raster->height) )
                {
                    D3DSURFACE_DESC d3dDesc;

                    #if defined(RWDEBUG)
                    NumCameraRasters++;
                    #endif /* defined(RWDEBUG) */

                    /* Find the depth of the backbuffer */
                    IDirect3DSurface8_GetDesc(_RwD3D8RenderSurface, &d3dDesc);

                    if (d3dDesc.Format == D3DFMT_A8R8G8B8 || d3dDesc.Format == D3DFMT_X8R8G8B8)
                    {
                        raster->depth = 32;
                    }
                    else if (d3dDesc.Format == D3DFMT_A1R5G5B5 || d3dDesc.Format == D3DFMT_R5G6B5)
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

                    rxD3D8VideoMemoryRasterListAdd(raster);

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

            rxD3D8VideoMemoryRasterListAdd(raster);
        }

        RWRETURN(TRUE);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RasterDestroy

 Destroy a raster

 On entry   : NULL
            : raster - raster to destroy
            : Flags
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterDestroy(void *unused1 __RWUNUSED__,
                   void *raster,
                   RwInt32 unused3 __RWUNUSED__)
{
    RwRaster            *ras = (RwRaster *)raster;
    _rwD3D8RasterExt    *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D8RasterDestroy"));

    rasExt = RASTEREXTFROMRASTER(ras);

    /* Check if it's the current rendering raster */
    if (_rwD3D8RWGetRasterStage(0) == ras)
    {
        _rwD3D8RWSetRasterStage(NULL, 0);

        if (rasExt->palette != NULL)
        {
            IDirect3DDevice8_SetCurrentTexturePalette(_RwD3DDevice, 0);
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
                            rxD3D8VideoMemoryRasterListRemove(ras);

                            AddAvailablePaletteIndex(rasExt->palette->globalindex);

                            RwFreeListFree(PaletteFreeList, rasExt->palette);
                            rasExt->palette = NULL;
                        }

                        /* Destroy the texture */
                        if (rasExt->texture)
                        {
                            IDirect3DTexture8_Release(rasExt->texture);
                        }
                    }
                }
                break;

            case (rwRASTERTYPETEXTURE):
                {
                    if (!(ras->cFlags & rwRASTERDONTALLOCATE))
                    {
                        #if defined(RWDEBUG)
                        /* Ops!, compresed and cube textures did not increment the counter... */
                        if (rasExt->dxtFormat == 0 &&
                            rasExt->cube == 0)
                        {
                            NumTextureRasters--;
                        }
                        #endif /* defined(RWDEBUG) */

                        if (rasExt->palette != NULL)
                        {
                            rxD3D8VideoMemoryRasterListRemove(ras);

                            AddAvailablePaletteIndex(rasExt->palette->globalindex);

                            RwFreeListFree(PaletteFreeList, rasExt->palette);
                            rasExt->palette = NULL;
                        }

                        /* Destroy the texture */
                        if (rasExt->texture)
                        {
                            IDirect3DTexture8_Release(rasExt->texture);
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

                        rxD3D8VideoMemoryRasterListRemove(ras);

                        /* Destroy the texture */
                        if (rasExt->texture)
                        {
                            IDirect3DTexture8_Release(rasExt->texture);
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

                        rxD3D8VideoMemoryRasterListRemove(ras);

                        /* Destroy the surface */
                        if ((LPSURFACE)rasExt->texture!=_RwD3D8DepthStencilSurface)
                        {
                            if (rasExt->texture)
                            {
                                IDirect3DSurface8_Release((LPSURFACE)rasExt->texture);
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

                    rxD3D8VideoMemoryRasterListRemove(ras);
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

            rxD3D8VideoMemoryRasterListRemove(ras);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RasterGetMipLevels

 Gets the number of mip levels for the given raster

 On entry   : NULL
            : pRaster - raster
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterGetMipLevels(void *mipLevels,
                        void *ras,
                        RwInt32 unused3 __RWUNUSED__)
{
    RwInt32         *numMipLevels = (RwInt32 *)mipLevels;
    RwRaster        *raster = (RwRaster *)ras;

    RWFUNCTION(RWSTRING("_rwD3D8RasterGetMipLevels"));

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
        _rwD3D8RasterExt  *rasExt = RASTEREXTFROMRASTER(raster);

        if (rasExt->texture != NULL)
        {
            *numMipLevels = IDirect3DTexture8_GetLevelCount(rasExt->texture);
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
 _rwD3D8TextureSetRaster


 On entry   :
 On exit    : TRUE on success
 */
RwBool
_rwD3D8TextureSetRaster(void *tex, void *ras, RwInt32 unused3 __RWUNUSED__)
{
    RwTexture          *texture = (RwTexture *) tex;
    RwRaster           *raster = (RwRaster *) ras;

    RWFUNCTION(RWSTRING("_rwD3D8TextureSetRaster"));

    /* Set the raster */
    texture->raster = raster;

    /* All done */
    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RasterSubRaster

 Get the pixel pointer from a raster

 On entry   : raster - raster to make subraster
              pIn - raster to make subraster of
              nIn - none
 On exit    : TRUE on success
 */
RwBool
_rwD3D8RasterSubRaster(void *rasterIn,
                     void *parentRasterIn,
                     RwInt32 unused3 __RWUNUSED__)
{
    RwRaster    *raster = (RwRaster *)rasterIn;
    RwRaster    *parentRaster = (RwRaster *)parentRasterIn;

    RWFUNCTION(RWSTRING("_rwD3D8RasterSubRaster"));

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
