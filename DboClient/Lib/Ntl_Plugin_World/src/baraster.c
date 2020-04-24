/*
 * Raster handling
 * Rasters are device specific representations of bitmaps
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/**
 * \ingroup rwraster
 * \page rwrasteroverview RwRaster Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection rasteroverview Overview
 *
 * The Raster is a platform-dependent representation of bitmaps.
 * Raster formats are largely dictated by the underlying hardware and
 * are not even guaranteed to use the RGB color model - some platforms
 * may even use compressed formats.
 *
 * Rasters need to be locked if you need to access or edit their contents
 * and should be unlocked after processing.
 *
 * It is possible to tell RenderWare Graphics that you only wish to read
 * data, or access structures like palettes, so that unlocking does not
 * impact too much on performance.
 *
 * These hints are used to determine whether RenderWare needs to perform
 * conversions on the data before and/or after unlocking.
 *
 * Rasters are closely connected with \see RwCamera \see RwImage and
 * \see RwTexture objects.
 *
 */

/***************************************************************************/

/* NOTE: NOTHING IN THIS FILE SHOULD DEPEND ON RWIMAGE
 * - THAT'S WHAT BAIMRAS.C/H IS FOR */

/***************************************************************************/

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "batkreg.h"
#include "badevice.h"
#include "baresour.h"

#include "bamemory.h"

/* Abstraction of string functionality -- for unicode support */
#include "rwstring.h"

/* Raster handling */
#include "baraster.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWRASTERGLOBAL(var)                             \
    (RWPLUGINOFFSET(rwRasterGlobals,                    \
                    RwEngineInstance,                   \
                    rasterModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwPluginRegistry rasterTKList =
 { sizeof(RwRaster),
    sizeof(RwRaster),
    0,
    0,
    (RwPluginRegEntry *) NULL,
    (RwPluginRegEntry *) NULL
 };

static RwModuleInfo rasterModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Accessing member fields

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwraster
 * \ref RwRasterGetWidth is used to retrieve the width of the specified
 * raster in pixels.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param raster  a pointer to the raster.
 *
 * \return Returns an \ref RwInt32 equal to the raster's width.
 *
 * \see RwRasterGetDepth
 * \see RwRasterGetHeight
 * \see RwRasterGetStride
 */
RwInt32
RwRasterGetWidth(const RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterGetWidth"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    RWRETURN(RwRasterGetWidthMacro(raster));
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetHeight is used to retrieve the height of the
 * specified raster in pixels.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns an \ref RwInt32 equal to the raster's height.
 *
 * \see RwRasterGetDepth
 * \see RwRasterGetWidth
 * \see RwRasterGetStride
 *
 */
RwInt32
RwRasterGetHeight(const RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterGetHeight"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    RWRETURN(RwRasterGetHeightMacro(raster));
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetStride is used to retrieve the stride of the
 * specified raster. The stride is measured in bytes per raster line and is
 * the number of bytes that separate two vertically adjacent pixels. If the
 * specified raster is a sub-raster this function will return the stride of
 * its parent.
 *
 * Note that the stride is only defined when the raster is locked. Calling
 * \ref RwRasterGetStride when the raster has not been locked will produce
 * undefined results.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns an \ref RwInt32 equal to the raster's stride.
 *
 * \see RwRasterGetDepth
 * \see RwRasterGetHeight
 * \see RwRasterGetWidth
 * \see RwRasterLock
 * \see RwRasterUnlock
 *
 */
RwInt32
RwRasterGetStride(const RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterGetStride"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(raster->privateFlags & rwRASTERPIXELLOCKED);

    RWRETURN(RwRasterGetStrideMacro(raster));
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetDepth is used to retrieve the depth of the specified
 * raster in bits per pixel.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns an \ref RwInt32 value equal to the raster's depth if successful
 *        or -1 if there is an error.
 *
 * \see RwRasterGetWidth
 * \see RwRasterGetHeight
 * \see RwRasterGetStride
 *
 */
RwInt32
RwRasterGetDepth(const RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterGetDepth"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    RWRETURN(RwRasterGetDepthMacro(raster));
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetFormat is used to retrieve the \ref RwRasterFormat of the
 * specified raster. This describes the pixel color format, and where
 * applicable, the palette depth and mipmap settings. These are set when
 * the raster is created. For example, a raster created on a 16-bit device with
 * type rwRASTERTYPECAMERA may have a format rwRASTERFORMAT565. This indicates
 * that it stores color information at 16-bits per pixel with 5-bits allocated
 * to the red channel, 6-bits to the green and 5-bits to the blue. See
 * \ref RwRasterFormat for more information.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns an \ref RwInt32 value describing the raster's format if
 *        successful or -1 if there is an error. See \ref RwRasterFormat.
 *
 * \see RwRasterCreate
 * \see RwRasterGetDepth
 * \see RwRasterGetHeight
 * \see RwRasterGetStride
 * \see RwRasterGetType
 * \see RwRasterGetWidth
 *
 */
RwInt32
RwRasterGetFormat(const RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterGetFormat"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    RWRETURN(RwRasterGetFormatMacro(raster));
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetType is used to retrieve the \ref RwRasterType of the
 * specified raster. This describes the type of the raster, such as whether
 * it is a texture raster (\ref rwRASTERTYPETEXTURE), a camera raster
 * (\ref rwRASTERTYPECAMERA) and so on. The type of a raster is set when
 * it is created.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns an \ref RwInt32 value describing the raster's type if
 *        successful or -1 if there is an error. See \ref RwRasterType.
 *
 * \see RwRasterCreate
 * \see RwRasterGetDepth
 * \see RwRasterGetFormat
 * \see RwRasterGetHeight
 * \see RwRasterGetStride
 * \see RwRasterGetWidth
 *
 */
RwInt32
RwRasterGetType(const RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterGetType"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    RWRETURN(RwRasterGetTypeMacro(raster));
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetParent is used to retrieve the raster that the
 * specified sub-raster references for its pixel data. If the given raster
 * is not a sub-raster it references its own pixels and is self-parenting.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns a pointer to the raster's parent if successful or NULL if
 *        there is an error.
 *
 * \see RwRasterSubRaster
 * \see RwRasterGetOffset
 *
 */
RwRaster *
RwRasterGetParent(const RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterGetParent"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    RWRETURN(RwRasterGetParentMacro(raster));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwraster
 * \ref RwRasterGetPluginOffset is used to get the offset of a
 * previously registered raster plugin.
 *
 * \param pluginID  An \ref RwUInt32 value equal to the plugin ID.
 *
 * \return Returns an \ref RwInt32 value equal to the data block offset if
 *        successful or -1 if the plugin is not registered.
 *
 * \see RwRasterRegisterPlugin
 * \see RwRasterValidatePlugins
 */
RwInt32
RwRasterGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             result;

    RWAPIFUNCTION(RWSTRING("RwRasterGetPluginOffset"));
    result = _rwPluginRegistryGetPluginOffset(&rasterTKList, pluginID);

    RWRETURN(result);
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetCurrentContext is used to determine which, if any,
 * raster is the current target for raster rendering and clearing.
 *
 * The current raster is only defined between calls to
 * \ref RwRasterPushContext and \ref RwRasterPopContext.
 *
 * \return Returns a pointer to the current raster if successful or NULL if
 *        there is an error or if there is no current raster (context stack
 *        is empty).
 *
 * \see RwRasterPushContext
 * \see RwRasterPopContext
 *
 */
RwRaster           *
RwRasterGetCurrentContext(void)
{
    RwInt32             top;
    RwRaster           *raster;

    RWAPIFUNCTION(RWSTRING("RwRasterGetCurrentContext"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(RWRASTERGLOBAL(rasterSP) > 0);

    top = RWRASTERGLOBAL(rasterSP);
    raster = RWRASTERGLOBAL(rasterStack[top]);
    RWRETURN(raster);
}

/**
 * \ingroup rwraster
 * \ref RwRasterRenderScaled is used to render the specified raster to
 * the given rectangular region in the currently selected render target. The
 * current render target is the raster on top of the context stack. If the
 * raster has a mask defining transparency in its alpha-channel this is
 * taken into account. No Z-buffering of the raster with its destination is
 * considered - this function only performs a simple blit operation. The
 * size of the specified rectangle determines the area in the destination
 * raster where rendering takes place. The raster is scaled to fit this
 * region, using pixel replication or averaging, as appropriate. Areas of
 * the raster (after scaling) which fall outside the destination are clipped
 * before rendering takes place.
 *
 * The raster itself is not changed in any way.
 *
 * Use the API function \ref RwRasterPushContext to set the current raster.
 *
 * \param raster  A pointer to the raster.
 * \param rect  A pointer to an \ref RwRect value describing the region in the destination
 *       raster where rendering takes place.
 *
 * \return Returns pointer to the raster if successful or NULL if there
 * is an error.
 *
 * \see RwRasterRender
 * \see RwRasterRenderFast
 * \see RwRasterPopContext
 * \see RwRasterPushContext
 *
 */
RwRaster           *
RwRasterRenderScaled(RwRaster * raster, RwRect * rect)
{
    RWAPIFUNCTION(RWSTRING("RwRasterRenderScaled"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(rect);
    RWASSERT(RWRASTERGLOBAL(rasterSP) > 0);
    RWASSERT(RWSRCGLOBAL(curCamera) == NULL);

    RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERRENDERSCALED])(raster, rect,0);
    RWRETURN(raster);
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetOffset is used to retrieve the x- and y-offset of the
 * specified raster relative to the top-level parent raster i.e. the raster
 * that holds the pixel data. If the specified raster is not a sub-raster
 * the offsets are zero.
 *
 * \param raster  Pointer to the raster.
 * \param xOffset  Pointer to an \ref RwInt16 value that will receive the x-offset.
 * \param yOffset  Pointer to an \ref RwInt16 value that will receive the y-offset.
 *
 * \return Returns pointer to the raster if successful or NULL if there
 *        is an error.
 *
 * \see RwRasterSubRaster
 * \see RwRasterGetParent
 *
 */
RwRaster           *
RwRasterGetOffset(RwRaster * raster, RwInt16 * xOffset,
                  RwInt16 * yOffset)
{
    RWAPIFUNCTION(RWSTRING("RwRasterGetOffset"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(xOffset);
    RWASSERT(yOffset);

    *xOffset = raster->nOffsetX;
    *yOffset = raster->nOffsetY;

    RWRETURN(raster);
}

/**
 * \ingroup rwraster
 * \ref RwRasterUnlock is used to unlock the specified raster, following
 * a call to \ref RwRasterLock, allowing it to be used by the device again.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns a pointer to the raster if successful or NULL if there is
 *        an error.
 *
 * \see RwRasterLock
 * \see RwRasterLockPalette
 * \see RwRasterUnlockPalette
 *
 */
RwRaster           *
RwRasterUnlock(RwRaster * raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterUnlock"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(raster->privateFlags & rwRASTERPIXELLOCKED);

    RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERUNLOCK]) (NULL, (void *) raster,
                                                  0);
    /* raster->privateFlags =
     * raster->privateFlags & ~rwRASTERPIXELLOCKED;
     */
    RWRETURN(raster);
}

/**
 * \ingroup rwraster
 * \ref RwRasterClear is used to clear the entire area of the current
 * render target to the given device specific pixel value. The current render
 * target is the raster at the top of the context stack.
 *
 * \param pixelValue  An \ref RwInt32 value equal to the device specific pixel value
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwRasterClearRect
 * \see RwRasterPopContext
 * \see RwRasterPushContext
 * \see RwRGBAToPixel
 *
 */
RwBool
RwRasterClear(RwInt32 pixelValue)
{
    RwBool              result;

    RWAPIFUNCTION(RWSTRING("RwRasterClear"));
    RWASSERT(rasterModule.numInstances);

    result = (RWRASTERGLOBAL(rasterSP) > 0);

    if (result)
    {
        RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERCLEAR]) (NULL, NULL,
                                                     pixelValue);
    }
    else
    {
        RWERROR((E_RW_RASTERSTACKEMPTY));
    }

    RWRETURN(result);
}

/**
 * \ingroup rwraster
 * \ref RwRasterValidatePlugins is used to validate the plugin memory
 * allocated within the specified raster. This function is useful for
 * determining where memory trampling may be occurring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * \param  raster   A pointer to the raster to validate.
 *
 * \return Returns TRUE is the raster data is valid or FALSE if there is an
 * error or if the raster data has become corrupt.
 *
 * \see RwRasterRegisterPlugin
 * \see RwRasterGetPluginOffset
 *
 */
RwBool
RwRasterValidatePlugins(const RwRaster * raster __RWUNUSEDRELEASE__)
{
    RwBool              ret = TRUE;

    RWAPIFUNCTION(RWSTRING("RwRasterValidatePlugins"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

#ifdef RWDEBUG
    ret = _rwPluginRegistryValidateObject(&rasterTKList, raster);
#endif /* RWDEBUG */

    RWRETURN(ret);
}

/**
 * \ingroup rwraster
 * \ref RwRasterRenderFast is used to render the specified raster to the
 * currently selected render target. The current render target is the raster
 * on top of the context stack. Unlike \ref RwRasterRender, transparency
 * information in the raster, if any, is not considered in the rendering,
 * making this function considerably faster. The rendering takes place such
 * that the upper-left corner of the raster falls at the specified position
 * in the destination raster. No Z-buffering of the raster with its
 * destination is considered - this function only performs a simple blit
 * operation. Areas of the raster which fall outside the destination are
 * clipped before rendering takes place.
 *
 * The raster itself is not changed in any way.
 *
 * Use the API function \ref RwRasterPushContext to set the current raster.
 *
 * \param raster  A pointer to the raster
 * \param x  An \ref RwInt32 value equal to the destination x-position
 * \param y  An \ref RwInt32 value equal to the destination y-position.
 *
 * \return Returns pointer to the raster if successful or NULL if there
 *        is an error.
 *
 * \see RwRasterRender
 * \see RwRasterRenderScaled
 * \see RwRasterPopContext
 * \see RwRasterPushContext
 *
 */
RwRaster           *
RwRasterRenderFast(RwRaster * raster, RwInt32 x, RwInt32 y)
{
    RwRect              rRect;

    RWAPIFUNCTION(RWSTRING("RwRasterRenderFast"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(RWRASTERGLOBAL(rasterSP) > 0);
    RWASSERT(RWSRCGLOBAL(curCamera) == NULL);

    rRect.x = x;
    rRect.y = y;

    RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERRENDERFAST]) (raster, &rRect,
                                                      0);
    RWRETURN(raster);
}

/**
 * \ingroup rwraster
 * \ref RwRasterClearRect is used to clear the specified rectangular
 * sub-region of the current render target to the given device specific pixel
 * value. The current render target is the raster on top of the context stack.
 * The sub-region must be wholly contained within the bounds of the
 * destination raster.
 *
 * \param rect  An \ref RwRect value describing the rectangular sub-region.
 * \param pixelValue  An \ref RwInt32 value equal to the device specific pixel value
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwRasterClear
 * \see RwRasterPopContext
 * \see RwRasterPushContext
 * \see RwRGBAToPixel
 *
 */
RwBool
RwRasterClearRect(RwRect * rect, RwInt32 pixelValue)
{
    RwInt32             top;
    RwStandardFunc RasterClearRectFunc;
    RwRaster      *curRaster;

    RWAPIFUNCTION(RWSTRING("RwRasterClearRect"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(RWRASTERGLOBAL(rasterSP) > 0);

    top = RWRASTERGLOBAL(rasterSP);
    curRaster = RWRASTERGLOBAL(rasterStack[top]);
    RWASSERT(NULL != curRaster);
    RWASSERT(rect);
    RWASSERT(rect->x >= 0);
    RWASSERT(rect->y >= 0);
    RWASSERT(rect->w >= 0);
    RWASSERT(rect->h >= 0);
    RWASSERT((rect->x + rect->w) <= RwRasterGetWidth(curRaster));
    RWASSERT((rect->y + rect->h) <= RwRasterGetHeight(curRaster));

    RasterClearRectFunc = RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERCLEARRECT]);
    RWASSERT(NULL != RasterClearRectFunc);

    RasterClearRectFunc(NULL, (void *) rect, pixelValue);
    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Sprite rendering

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwraster
 * \ref RwRasterRender is used to render the specified raster to the
 * currently selected render target. The current render target is the raster
 * on top of the context stack. The rendering takes place such that the
 * upper-left corner of the raster falls at the specified position in the
 * destination raster and if the raster has a mask defining transparency in
 * its alpha- channel this is taken into account. No Z-buffering of the
 * raster with its destination is considered - this function only performs
 * a simple blit operation. Areas of the raster which fall outside the
 * destination are clipped before rendering takes place.
 *
 * The raster itself is not changed in any way.
 *
 * Use the API function \ref RwRasterPushContext to set the current raster.
 *
 * \param raster  Pointer to the raster.
 * \param x  A \ref RwInt32 value equal to the destination x-position.
 * \param y  A \ref RwInt32 value equal to the destination y-position.
 *
 * \return Returns pointer to the raster if successful or NULL if there
 *        is an error.
 *
 * \see RwRasterRenderScaled
 * \see RwRasterRenderFast
 * \see RwRasterPopContext
 * \see RwRasterPushContext
 *
 */
RwRaster           *
RwRasterRender(RwRaster * raster, RwInt32 x, RwInt32 y)
{
    RwRect              rRect;

    RWAPIFUNCTION(RWSTRING("RwRasterRender"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(RWRASTERGLOBAL(rasterSP) > 0);
    RWASSERT(RWSRCGLOBAL(curCamera) == NULL);

    rRect.x = x;
    rRect.y = y;

    RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERRENDER]) (raster, &rRect, 0);

    RWRETURN(raster);
}

/**
 * \ingroup rwraster
 * \ref RwRasterUnlockPalette is used to unlock the specified raster’s
 * palette, following a call to \ref RwRasterLockPalette, allowing it to be used
 * by the device again.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns a pointer to the raster if successful or NULL if there is
 *        an error.
 *
 * \see RwRasterLock
 * \see RwRasterLockPalette
 * \see RwRasterUnlock
 *
 */
RwRaster           *
RwRasterUnlockPalette(RwRaster * raster)
{
    RwStandardFunc      RasterUnlockPaletteFunc;

    RWAPIFUNCTION(RWSTRING("RwRasterUnlockPalette"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(raster->privateFlags & rwRASTERPALETTELOCKED);

    RasterUnlockPaletteFunc =
        RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERUNLOCKPALETTE]);
    RWASSERT(NULL != RasterUnlockPaletteFunc);
    RasterUnlockPaletteFunc(NULL, (void *) raster, 0);

    raster->privateFlags =
        raster->privateFlags & ~rwRASTERPALETTELOCKED;
    RWRETURN(raster);
}

/**
 * \ingroup rwraster
 * \ref RwRasterDestroy is used to destroy the specified raster and free
 * any resources it may have used.
 *
 * \note If the raster is a parent raster, its sub raster must be destroyed
 * as it will no longer be valid. Destruction of the sub raster does not occur
 * when the parent raster is destroyed and therefore RwRasterDestroy must
 * be called again to destroy the sub raster.
 *
 * \param raster  A pointer to the raster
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwRasterCreate
 *
 */
RwBool
RwRasterDestroy(RwRaster * raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterDestroy"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    /* De-initialize the plugin data */
    _rwPluginRegistryDeInitObject(&rasterTKList, raster);

    /* Give driver a look in */
    RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERDESTROY]) (NULL, raster, 0);

    /* Blow it away */
    RwFreeListFree(RWRASTERGLOBAL(rasterFreeList), raster);

    /* All done */
    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Rendering to a raster

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwraster
 * \ref RwRasterPushContext is used to indicate that the specified
 * raster is to be the target of subsequent clears and blits. Whenever any
 * 2D rendering is performed the graphical output is stored in the raster on
 * the top of the context stack. This function therefore selects the raster
 * that will be rendered to.
 *
 * This function cannot be used between \ref RwCameraBeginUpdate and
 * \ref RwCameraEndUpdate.
 *
 * \param raster  A pointer to the raster.
 *
 * \return Returns a pointer to the raster if successful or NULL if there
 *        is an error.
 *
 * \see RwRasterGetCurrentContext
 * \see RwRasterPopContext
 * \see RwRasterRender
 * \see RwRasterRenderFast
 * \see RwRasterRenderScaled
 *
 */
RwRaster           *
RwRasterPushContext(RwRaster * raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterPushContext"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(RWRASTERGLOBAL(rasterSP) < (rwRASTERCONTEXTSTACKSIZE - 1));

    RWASSERTM((RWSRCGLOBAL(curCamera) == NULL),
        (RWSTRING("RwRasterPushContext has been used between "
                 "RwCameraBeginUpdate and RwCameraEndUpdate.")));

    /* Push onto the stack */
    if (RWSRCGLOBAL(stdFunc[rwSTANDARDSETRASTERCONTEXT])
        (NULL, (void *) raster, 0))
    {
        RwInt32             top = ++RWRASTERGLOBAL(rasterSP);
        RWRASTERGLOBAL(rasterStack[top]) = raster;
        RWRETURN(raster);
    }

    /* Device failure */
    RWRETURN((RwRaster *) NULL);
}

/**
 * \ingroup rwraster
 * \ref RwRasterRegisterPlugin is used to register a plugin and
 * reserve some space within a raster. This must happen after the engine has
 * been initialized but before the engine is opened.
 *
 * \param size  An \ref RwInt32 value equal to the size of the memory block to reserve.
 * \param pluginID  An \ref RwUInt32 value equal to the plugin ID (must be unique - used to
 *         identify binary chunks).
 * \param constructCB  A constructor for the plugin data block.
 * \param destructCB  A destructor for the plugin data block.
 * \param copyCB  A copy constructor for the plugin data block.
 *
 * \return Returns an \ref RwInt32 value equal to the byte offset within the raster
 *        of memory reserved for this plugin if successful or -1 if there
 *        is an error.
 *
 * \see RwRasterGetPluginOffset
 * \see RwRasterValidatePlugins
 *
 */
RwInt32
RwRasterRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                       RwPluginObjectConstructor
                       constructCB,
                       RwPluginObjectDestructor
                       destructCB, RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwRasterRegisterPlugin"));
    RWASSERT(!rasterModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlugin(&rasterTKList, size, pluginID,
                                      constructCB, destructCB, copyCB);
    RWRETURN(plug);
}

/**
 * \ingroup rwraster
 * \ref RwRasterLockPalette is used to lock the specified raster for
 * direct palette access according to the given lock mode. The lock prevents
 * the device changing the palette’s data while it is under the control of
 * the application.
 *
 * The palette must be unlocked when the application has finished updating
 * the palette data so that the device is made aware of the changes.
 *
 * \param raster  Pointer to the raster.
 * \param lockMode  A value equal to the lock mode. See \ref RwRasterLockMode.
 *
 * \return Returns pointer to the raster's palette data if successful or NULL
 *        if there is an error.
 *
 * \see RwRasterLock
 * \see RwRasterUnlock
 * \see RwRasterUnlockPalette
 *
 */
RwUInt8            *
RwRasterLockPalette(RwRaster * raster, RwInt32 lockMode)
{
    RwUInt8            *palettePtr;

    RWAPIFUNCTION(RWSTRING("RwRasterLockPalette"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(!(raster->privateFlags & rwRASTERPALETTELOCKED));

    if (RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERLOCKPALETTE])
        ((void *) &palettePtr, (void *) raster, (RwInt32) lockMode))
    {
        /*if (palettePtr)
         * {
         * raster->privateFlags |= rwRASTERPALETTELOCKED;
         * } */
        RWRETURN(palettePtr);
    }

    /* Lock or device failure */
    RWRETURN((unsigned char *) NULL);
}

/**
 * \ingroup rwraster
 * \ref RwRasterPopContext is used to indicate that the raster on the
 * top of the context stack is no longer to be targeted with subsequent
 * clears and blits.
 *
 * This function cannot be used between \ref RwCameraBeginUpdate and
 * \ref RwCameraEndUpdate.
 *
 * \return Returns pointer to the raster which has been popped if successful
 *        or NULL if there is an error or if the context stack is empty.
 *
 * \see RwRasterGetCurrentContext
 * \see RwRasterPushContext
 * \see RwRasterRender
 * \see RwRasterRenderFast
 * \see RwRasterRenderScaled
 *
 */
RwRaster           *
RwRasterPopContext(void)
{
    RwInt32             top;
    RwRaster           *raster;
    RwStandardFunc      SetRasterContextFunc;
    RwRaster           *StackTop;

    RWAPIFUNCTION(RWSTRING("RwRasterPopContext"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(RWRASTERGLOBAL(rasterSP) > 0);

    RWASSERTM((RWSRCGLOBAL(curCamera) == NULL),
        (RWSTRING("RwRasterPushContext has been used between "
                 "RwCameraBeginUpdate and RwCameraEndUpdate.")));

    top = RWRASTERGLOBAL(rasterSP)--;
    raster = RWRASTERGLOBAL(rasterStack[top]);

    SetRasterContextFunc =
        RWSRCGLOBAL(stdFunc[rwSTANDARDSETRASTERCONTEXT]);

    top = RWRASTERGLOBAL(rasterSP);
    StackTop = RWRASTERGLOBAL(rasterStack[top]);

    RWASSERT(NULL != SetRasterContextFunc);

    /* Set up the new */
    if (SetRasterContextFunc(NULL, StackTop, 0))
    {
        RWRETURN(raster);
    }

    /* Device failure */
    RWRETURN((RwRaster *) NULL);
}

/**
 * \ingroup rwraster
 * \ref RwRasterGetNumLevels is used to retrieve the number of mipmap levels
 * in a specified raster (usually of type rwRASTERTYPETEXTURE). This can
 * depend on platform and raster size.
 *
 * \param raster  A pointer to the raster
 *
 * \return Returns the number of mipmap levels present in a raster or -1 if
 * there is an error.
 *
 */
RwInt32
RwRasterGetNumLevels(RwRaster * raster)
{
    RwStandardFunc      GetMipLevelsFunc;
    RwInt32             numMipLevels;

    RWAPIFUNCTION(RWSTRING("RwRasterGetNumLevels"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    if (!(RwRasterGetFormat(raster) & rwRASTERFORMATMIPMAP))
    {
        RWRETURN(1);
    }

    GetMipLevelsFunc =
        RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERGETMIPLEVELS]);

    RWASSERT(NULL != GetMipLevelsFunc);

    /* Give it to the driver */
    if (GetMipLevelsFunc(&numMipLevels, raster, 0))
    {
        RWRETURN(numMipLevels);
    }

    RWRETURN(-1);
}

/**
 * \ingroup rwraster
 * \ref RwRasterShowRaster is used to copy the specified raster to the
 * display device. The raster must be of type rwRASTERTYPECAMERA.
 *
 * \param raster  A pointer to the raster.
 * \param dev  A device-dependent parameter, e.g. for Windows applications the
 *       handle of the output window returned by CreateWindow (type HWND).
 * \param flags  An \ref RwUInt32 bit-field value equal to the raster display options
 *       (type \ref RwRasterFlipMode):
 *       \li rwRASTERFLIPWAITVSYNC - Wait for the next vertical retrace on
 *           the output device, if possible, before displaying (full-screen
 *           applications only). Specify zero otherwise.
 *
 * \return Returns a pointer to the raster if successful or NULL if there
 *        is an error.
 *
 * \see RwCameraShowRaster
 * \see RwRasterCreate
 * \see RwRasterGetFormat
 * \see RwRasterGetType
 *
 */
RwRaster           *
RwRasterShowRaster(RwRaster * raster, void *dev, RwUInt32 flags)
{
    RwStandardFunc      RasterShowRasterFunc;

    RWAPIFUNCTION(RWSTRING("RwRasterShowRaster"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(raster->cType == rwRASTERTYPECAMERA);

    RasterShowRasterFunc =
        RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERSHOWRASTER]);

    /* Purge the cache */
    _rwResourcesPurge();

    RWASSERT(NULL != RasterShowRasterFunc);
    if (RasterShowRasterFunc(raster, dev, (RwInt32) flags))
    {
#ifdef RWMETRICS
        RwMetrics          *metrics = RWSRCGLOBAL(metrics);

        /* Reset the metrics */
        metrics->numTriangles = 0;
        metrics->numProcTriangles = 0;
        metrics->numVertices = 0;
        metrics->numTextureUploads = 0;
        metrics->sizeTextureUploads = 0;
        metrics->numResourceAllocs = 0;
#endif /* RWMETRICS */

        RWRETURN(raster);
    }

    /* Device failure */
    RWRETURN((RwRaster *) NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Accessing sub rasters

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwraster
 * \ref RwRasterSubRaster is used to define a raster that references a
 * rectangular sub-region in another raster. The sub-region must be wholly
 * contained within the bounds of the raster it references. It is necessary
 * that the sub-raster is created with zero width and height so that no
 * memory is actually allocated for the raster's pixel data (which would
 * become unused). Hence, the sub-raster acts as a place-holder whose pixel
 * pointer references the pixel data in another raster covered by the
 * specified rectangle.
 *
 * \param subRaster  A pointer to the sub-raster.
 * \param raster  A pointer to the parent raster.
 * \param rect  A pointer to a RwRect describing the rectangular sub-region.
 *
 * \return Returns a pointer to the sub-raster if successful or NULL if
 *        there is an error.
 *
 * \see RwRasterCreate
 * \see RwRasterGetParent
 * \see RwRasterGetOffset
 *
 */
RwRaster           *
RwRasterSubRaster(RwRaster * subRaster, RwRaster * raster,
                  RwRect * rect)
{
    RWAPIFUNCTION(RWSTRING("RwRasterSubRaster"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(subRaster);
    RWASSERT(raster);
    RWASSERT(rect);
    RWASSERT((rect->x >= 0) && (rect->x < raster->width));
    RWASSERT((rect->y >= 0) && (rect->y < raster->height));
    RWASSERT((rect->x + rect->w) <= raster->width);
    RWASSERT((rect->y + rect->h) <= raster->height);

    if (!(subRaster->cFlags & (RwUInt8) rwRASTERDONTALLOCATE))
    {
        /* This raster has pixels, we can't make a sub raster from it */
        RWRETURN((RwRaster *) NULL);
    }

    /* Set up the size and the offset (same for all drivers) */
    subRaster->width = rect->w;
    subRaster->height = rect->h;
    subRaster->nOffsetX = raster->nOffsetX + (RwInt16) rect->x;
    subRaster->nOffsetY = raster->nOffsetY + (RwInt16) rect->y;

    if (RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERSUBRASTER])
        (subRaster, raster, 0))
    {
        /* Someone else has the pixels now */
        subRaster->parent = raster->parent;

        RWRETURN(subRaster);
    }

    /* Device failed to create sub raster */
    RWRETURN((RwRaster *) NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Raster handling

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwraster
 * \ref RwRasterCreate is used to create a new raster of the specified
 * type with the given width, height and depth. The width and height are
 * specified in pixels, the depth is the number of bits per pixel, and the
 * flags indicate the type, and possibly the format, of the raster to create.
 * Specifying zero for the depth indicates that the device default value
 * should be used.
 *
 * \param width  An \ref RwInt32 value equal to the width of the raster
 * \param height  An \ref RwInt32 value equal to the height of the raster
 * \param depth  An \ref RwInt32 value equal to the depth of the raster
 * \param flags  An \ref RwInt32 value specifying the type and format of raster to create,
 *       combined using a bit-wise OR. The formats are as described under
 *       \ref RwRasterGetFormat. The following types are defined:
 *       \li rwRASTERTYPENORMAL - Device default raster
 *       \li rwRASTERTYPEZBUFFER - Camera Z-buffer raster
 *       \li rwRASTERTYPECAMERA - Camera frame buffer raster
 *       \li rwRASTERTYPETEXTURE - Texture raster
 *       \li rwRASTERTYPECAMERATEXTURE - Camera texture raster.
 *
 * \return Returns a pointer to the new raster if successful or NULL if there
 *        is an error.
 *
 * \see RwRasterGetWidth
 * \see RwRasterGetHeight
 * \see RwRasterGetDepth
 * \see RwRasterGetStride
 * \see RwRasterGetFormat
 * \see RwRasterGetType
 * \see RwRasterDestroy
 * \see RwRasterRead
 * \see RwRasterType
 *
 */

RwRaster           *
RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth,
               RwInt32 flags)
{
    RwRaster           *raster;

    RWAPIFUNCTION(RWSTRING("RwRasterCreate"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(0 <= width);
    RWASSERT(0 <= height);
    RWASSERT(0 <= depth);

    raster =
        (RwRaster *) RwFreeListAlloc(RWRASTERGLOBAL(rasterFreeList),
                                     rwID_RASTERMODULE | rwMEMHINTDUR_EVENT);
    RWASSERT(RWFREELISTALIGNED(raster, RWRASTERGLOBAL(rasterFreeList)));

    if (raster)
    {
        const RwStandardFunc RasterCreateFunc =
            RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERCREATE]);
        RWASSERT(NULL != RasterCreateFunc);

        raster->privateFlags = 0;
        raster->cFlags = 0;
        raster->width = width;
        raster->height = height;
        raster->nOffsetX = 0;
        raster->nOffsetY = 0;
        raster->depth = depth;
        raster->parent = raster; /* It contains its own pixels */
        raster->cpPixels = (unsigned char *) NULL;
        raster->palette = (unsigned char *) NULL;

        if (!RasterCreateFunc(NULL, raster, flags))
        {
            RwFreeListFree(RWRASTERGLOBAL(rasterFreeList), raster);

            RWRETURN((RwRaster *) NULL);
        }

        /* Initialize the plugin data */
        _rwPluginRegistryInitObject(&rasterTKList, raster);

        RWRETURN(raster);
    }

    /* Failed to alloc free list entry */
    RWRETURN((RwRaster *) NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Accessing a rasters contents

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwraster
 * \ref RwRasterLock is used to lock the specified raster for direct
 * pixel access according to the given lock mode. The lock prevents the
 * device changing the raster's data while it is under the control of the
 * application. This function also puts a valid stride in the raster for
 * retrieval.
 *
 * If the specified raster is a sub-raster, this function returns a pointer
 * to the first sub- raster pixel. The stride of the sub-raster, however,
 * is equal to that of the parent raster and must be used for accessing
 * successive scan lines in the sub-raster.
 *
 * The raster must be unlocked when the application has finished updating
 * the pixel data so that the device is made aware of the changes.
 *
 * \param raster  Pointer to the raster.
 * \param level  A value equal to the mipmap level to be locked. 0 is the
 * source raster.
 * \param lockMode  A value equal to the lock mode. See \ref RwRasterLockMode.
 *
 * \return Returns pointer to the raster's pixel data if successful or NULL
 *        if there is an error.
 *
 * \see RwRasterGetStride
 * \see RwRasterLockPalette
 * \see RwRasterUnlock
 * \see RwRasterUnlockPalette
 *
 */
RwUInt8 *
RwRasterLock(RwRaster *raster, RwUInt8 level, RwInt32 lockMode)
{
    RwUInt8            *pixelPtr;

    RWAPIFUNCTION(RWSTRING("RwRasterLock"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);
    RWASSERT(!(raster->privateFlags & rwRASTERPIXELLOCKED));

#ifdef RWDEBUG
    /* ensure the lock request is valid */
    if (raster->cFormat & (rwRASTERFORMATMIPMAP >> 8))
    {
        if (raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8))
        {
            if ((level > 0) && (lockMode & rwRASTERLOCKWRITE))
            {
                RWERROR((E_RW_INVRASTERLOCKREQ));
                RWRETURN((RwUInt8 *)NULL);
            }
        }
    }
    else
    {
        if (level > 0)
        {
            RWERROR((E_RW_INVRASTERMIPLEVEL));
            RWRETURN((RwUInt8 *)NULL);
        }
    }
#endif

    /* Ask the driver to lock the raster.
     * This might still fail if that mipmap level doesn't exist etc.
     */
    if (RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERLOCK])
        ((void *) &pixelPtr, (void *) raster,
         ((RwInt32) lockMode) + (((RwInt32) level) << 8)))
    {
        RWRETURN(pixelPtr);
    }

    /* Lock or device failure */
    RWRETURN((unsigned char *) NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening/Closing

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwRasterClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwRasterClose(void *instance,
               RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwRasterClose"));

    if (RWRASTERGLOBAL(rasterFreeList))
    {
        RwFreeListDestroy(RWRASTERGLOBAL(rasterFreeList));
        RWRASTERGLOBAL(rasterFreeList) = (RwFreeList *) NULL;
    }

    /* One less module instance */
    rasterModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}


static RwInt32 _rwRasterFreeListBlockSize = 128,
               _rwRasterFreeListPreallocBlocks = 1;
static RwFreeList _rwRasterFreeList;

/**
 * \ingroup rwraster
 * \ref RwRasterSetFreeListCreateParams allows the developer to specify
 * how many \ref RwRaster s to preallocate space for.
 * Call before \ref RwEngineOpen.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwRasterSetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc)
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RwRasterSetFreeListCreateParams"));
#endif
    _rwRasterFreeListBlockSize = blockSize;
    _rwRasterFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}



/****************************************************************************
 _rwRasterOpen

 On entry   :
 One exit   : TRUE on success
 */

void               *
_rwRasterOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwRasterOpen"));

    /* Save the globals offset (same for all instances) */
    rasterModule.globalsOffset = offset;

    /* And get on with the setup */
    memset(&RWRASTERGLOBAL(dummyRaster), 0,
           sizeof(RWRASTERGLOBAL(dummyRaster)));
    RWRASTERGLOBAL(dummyRaster).width = 0;
    RWRASTERGLOBAL(dummyRaster).height = 0;
    RWRASTERGLOBAL(dummyRaster).depth = 0;
    RWRASTERGLOBAL(dummyRaster).cFlags = (RwUInt8) rwRASTERDONTALLOCATE;
    RWRASTERGLOBAL(dummyRaster).cpPixels = (unsigned char *) NULL;
    RWRASTERGLOBAL(dummyRaster).palette = (unsigned char *) NULL;
    RWRASTERGLOBAL(dummyRaster).cType = (RwUInt8) rwRASTERTYPENORMAL;

    RWRASTERGLOBAL(rasterSP) = 0;
    RWRASTERGLOBAL(rasterStack[0]) = &RWRASTERGLOBAL(dummyRaster);

    RWRASTERGLOBAL(rasterFreeList) =
        RwFreeListCreateAndPreallocateSpace(rasterTKList.sizeOfStruct, _rwRasterFreeListBlockSize,
        sizeof(RwUInt32), _rwRasterFreeListPreallocBlocks, &_rwRasterFreeList,
        rwID_RASTERMODULE | rwMEMHINTDUR_GLOBAL);
    if (!RWRASTERGLOBAL(rasterFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* One more module instance */
    rasterModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}
