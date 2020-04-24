/***************************************************************************
 *                                                                         *
 * Module  : banull.c                                                      *
 *                                                                         *
 * Purpose : Null driver device layer                                      *
 *                                                                         *
 **************************************************************************/

/***************************************************************************
 Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "batypes.h"
#include "badevice.h"
#include "balibtyp.h"
#include "baraster.h"
#include "baimage.h"
#include "batextur.h"
#include "bacamera.h"
#include "baraster.h"
#include "bamemory.h"
#include "barwtyp.h"
#include "palquant.h"
#include "baim3d.h"
#include "baresour.h"
#include "p2core.h"

/* String abstraction API for unicode support */
#include "rwstring.h"

/* Drag in the vertex format */
#include "drvmodel.h"
#include "drvfns.h"

/* This files header */

#include "banull.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/* The device globals */
RwRwDeviceGlobals   dgGGlobals;

/* NOT STATIC!!
 * But is the link from the library side to the device side of the system
 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                          Dummy render functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 NullSetRenderState

 On entry   : State
            : pParam
 On exit    :
 */

static RwBool
NullSetRenderState(RwRenderState nState __RWUNUSED__,
                   void * pParam __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullSetRenderState"));
    RWRETURN(TRUE);
}

/****************************************************************************
 NullGetRenderState

 On entry   : State
            : Param
 On exit    : TRUE
 */

static RwBool
NullGetRenderState(RwRenderState nState __RWUNUSED__,
                   void * pParam __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullGetRenderState"));
    RWRETURN(TRUE);
}

/****************************************************************************
 NullIm2DRenderLine

 On entry   : verts
 On exit    :
 */

static RwBool
NullIm2DRenderLine(RwIm2DVertex * verts __RWUNUSED__,
                   RwInt32 numVerts __RWUNUSED__,
                   RwInt32 v1 __RWUNUSED__,
                   RwInt32 v2 __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullIm2DRenderLine"));

    RWRETURN(TRUE);
}

/****************************************************************************
 NullIm2DRenderTriangle

 On entry   : verts
 On exit    :
 */

static RwBool
NullIm2DRenderTriangle(RwIm2DVertex * verts __RWUNUSED__,
                       RwInt32 numVerts __RWUNUSED__,
                       RwInt32 v1 __RWUNUSED__,
                       RwInt32 v2 __RWUNUSED__,
                       RwInt32 v3 __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullIm2DRenderTriangle"));

    RWRETURN(TRUE);
}

/****************************************************************************
 NullIm2DRenderPrimitive

 On entry   : verts, indices
 On exit    :
 */

static RwBool
NullIm2DRenderPrimitive(RwPrimitiveType nPrimType,
                        RwIm2DVertex * tvpVert, RwInt32 nNumVertices)
{
    RwInt32             index;
    RwInt32             nNumIndices = nNumVertices;

    RWFUNCTION(RWSTRING("NullIm2DRenderPrimitive"));

    switch (nPrimType)
    {
        case rwPRIMTYPELINELIST:
            {
                index = 0;
                while (nNumIndices > 1)
                {
                    NullIm2DRenderLine(tvpVert, nNumVertices, index,
                                       index + 1);

                    index += 2;
                    nNumIndices -= 2;
                }

                break;
            }
        case rwPRIMTYPEPOLYLINE:
            {
                index = 0;
                while (nNumIndices > 1)
                {
                    NullIm2DRenderLine(tvpVert, nNumVertices, index,
                                       index + 1);

                    index++;
                    nNumIndices--;
                }

                break;
            }
        case rwPRIMTYPETRILIST:
            {
                index = 0;
                while (nNumIndices > 2)
                {
                    NullIm2DRenderTriangle(tvpVert, nNumVertices,
                                           index, index + 1, index + 2);

                    index += 3;
                    nNumIndices -= 3;
                }

                break;
            }
        case rwPRIMTYPETRISTRIP:
            {
                RwInt32             vertInd = 1;

                index = 0;
                while (nNumIndices > 2)
                {
                    /* Dispatch a polygon */
                    NullIm2DRenderTriangle(tvpVert, nNumVertices,
                                           index, index + vertInd,
                                           index + (vertInd ^ 3));

                    /* Flip the backface test for next time!!
                       This turns 2=>1 and 1=>2 */
                    vertInd ^= 3;

                    /* Next triple of vertices */
                    index++;
                    nNumIndices--;
                }

                break;
            }
        case rwPRIMTYPETRIFAN:
            {
                index = 1;
                while (nNumIndices > 2)
                {
                    NullIm2DRenderTriangle(tvpVert, nNumVertices, 0,
                                           index, index + 1);

                    index++;
                    nNumIndices--;
                }

                break;
            }
        default:
            {
                break;
            }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullIm2DRenderIndexedPrimitive

 On entry   : verts, indices
 On exit    :
 */

static RwBool
NullIm2DRenderIndexedPrimitive(RwPrimitiveType nPrimType,
                               RwIm2DVertex * tvpVert,
                               RwInt32 nNumVertices,
                               RwImVertexIndex * indices,
                               RwInt32 nNumIndices)
{
    RWFUNCTION(RWSTRING("NullIm2DRenderIndexedPrimitive"));

    switch (nPrimType)
    {
        case rwPRIMTYPELINELIST:
            {
                while (nNumIndices > 1)
                {
                    NullIm2DRenderLine(tvpVert, nNumVertices,
                                       indices[0], indices[1]);

                    indices += 2;
                    nNumIndices -= 2;
                }

                break;
            }
        case rwPRIMTYPEPOLYLINE:
            {
                while (nNumIndices > 1)
                {
                    NullIm2DRenderLine(tvpVert, nNumVertices,
                                       indices[0], indices[1]);

                    indices++;
                    nNumIndices--;
                }

                break;
            }
        case rwPRIMTYPETRILIST:
            {
                while (nNumIndices > 2)
                {
                    NullIm2DRenderTriangle(tvpVert, nNumVertices,
                                           indices[0], indices[1],
                                           indices[2]);

                    indices += 3;
                    nNumIndices -= 3;
                }

                break;
            }
        case rwPRIMTYPETRISTRIP:
            {
                RwInt32             vertInd = 1;

                nNumIndices -= 2;
                while (nNumIndices > 0)
                {
                    /* Dispatch a polygon */
                    NullIm2DRenderTriangle(tvpVert, nNumVertices,
                                           indices[0],
                                           indices[vertInd],
                                           indices[vertInd ^ 3]);

                    /* Flip the backface test for next time!!
                     * This turns 2=>1 and 1=>2 */
                    vertInd ^= 3;

                    /* Next triple of vertices */
                    indices++;
                    nNumIndices--;
                }

                break;
            }
        case rwPRIMTYPETRIFAN:
            {
                RwInt32             startVert = indices[0];

                nNumIndices -= 2;

                indices++;

                while (nNumIndices > 0)
                {
                    NullIm2DRenderTriangle(tvpVert, nNumVertices,
                                           startVert, indices[0],
                                           indices[1]);

                    indices++;
                    nNumIndices--;
                }

                break;
            }
        default:
            {
                break;
            }
    }

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                        Standard functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 NullCameraBeginUpdate

 On entry   : NULL
            : camera pointer (IN)
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullCameraBeginUpdate(void * pOut __RWUNUSED__,
                      void * pCam __RWUNUSED__,
                      RwInt32 nData __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("NullCameraBeginUpdate"));

    /* Let's pretend we can do this - we never render anything anyway */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullCameraEndUpdate

 On entry   : NULL
            : camera pointer (IN)
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullCameraEndUpdate(void *pOut __RWUNUSED__,
                    void *pCam __RWUNUSED__,
                    RwInt32 nData __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullCameraEndUpdate"));

    /* Let's pretend we can do this - we never render anything anyway */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullCameraClear

 On entry   : camera pointer (IN)
            : pointer to RwRGBA giving frame buffer color
            : bit field indicating what to clear (rwCAMERACLEARIMAGE,
                                                  rwCAMERACLEARZ)
 On exit    : TRUE on success
 */
static RwBool
NullCameraClear(void *pCam __RWUNUSED__,
                void *pCol __RWUNUSED__,
                RwInt32 nClearWhich __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullCameraClear"));

    /* Let's pretend we can do this - we never render anything anyway */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterShowRaster

 On entry   : raster pointer (IN)
            : device specific pointer (IN) - HWND for windows, eg
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullRasterShowRaster(void *pRas __RWUNUSED__,
                     void *pDev __RWUNUSED__,
                     RwInt32 nData __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullRasterShowRaster"));

    /* Let's pretend we can do this - we never render anthing anyway */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullRGBToPixel

 On entry   : palette value (OUT)
            : palette entry (IN)
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullRGBToPixel(void *pPixel, void *pCol, RwInt32 nFormat)
{
    RwRGBA             *rpCol = (RwRGBA *) pCol;

    RWFUNCTION(RWSTRING("NullRGBToPixel"));

    switch (nFormat & (RwInt32) rwRASTERFORMATPIXELFORMATMASK)
    {
        case rwRASTERFORMAT8888:
        case rwRASTERFORMATDEFAULT:
            {
                *((RwUInt32 *) pPixel) =
                    (((RwUInt32) rpCol->red) << 24) |
                    (((RwUInt32) rpCol->green) << 16) |
                    (((RwUInt32) rpCol->blue) << 8) |
                    (((RwUInt32) rpCol->alpha) << 0);

                break;
            }
        case rwRASTERFORMAT888:
            {
                *((RwUInt32 *) pPixel) =
                    (((RwUInt32) rpCol->red) << 16) |
                    (((RwUInt32) rpCol->green) << 8) |
                    (((RwUInt32) rpCol->blue) << 0);
                break;
            }
        default:
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
    }

    /* All done */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullPixelToRGB

 On entry   : Palette entry (OUT)
            : NULL
            : Palette entry (IN)
 On exit    : TRUE on success
 */
static RwBool
NullPixelToRGB(void *pRGB, void *pPixel, RwInt32 nFormat)
{
    RwRGBA             *rpRGB = (RwRGBA *) pRGB;
    RwInt32             nPixel = *(RwInt32 *) pPixel;

    RWFUNCTION(RWSTRING("NullPixelToRGB"));
    RWASSERT(pPixel);
    RWASSERT(pRGB);

    switch (nFormat & (RwInt32) rwRASTERFORMATPIXELFORMATMASK)
    {
        case rwRASTERFORMAT8888:
        case rwRASTERFORMATDEFAULT:
            {
                rpRGB->red = (RwUInt8) (nPixel >> 24);
                rpRGB->green = (RwUInt8) (nPixel >> 16);
                rpRGB->blue = (RwUInt8) (nPixel >> 8);
                rpRGB->alpha = (RwUInt8) (nPixel >> 0);

                break;
            }
        case rwRASTERFORMAT888:
            {
                rpRGB->red = (RwUInt8) (nPixel >> 16);
                rpRGB->green = (RwUInt8) (nPixel >> 8);
                rpRGB->blue = (RwUInt8) (nPixel >> 0);
                rpRGB->alpha = 0xFF;

                break;
            }
        default:
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
    }

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                         Texture access

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 NullTextureSetRaster

 On entry   : Texture
            : Raster
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullTextureSetRaster(void *pTex, void *pRas, RwInt32 nIn)
{
    RwTexture          *tex = (RwTexture *) pTex;
    RwRaster           *rpRas = (RwRaster *) pRas;

    RWFUNCTION(RWSTRING("NullTextureSetRaster"));

    /* Stop warnings */
    nIn = nIn;

    /* Try and set the raster */
    tex->raster = rpRas;

    /* All done */
    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                      Finding the appropriate format

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 NullGetRasterFormat

 On entry   :
            : Raster
            : Flags
 On exit    :
 */
static RwBool
NullGetRasterFormat(void *pInOut, void *pRaster, RwInt32 flags)
{
    RwRaster           *rpRas = (RwRaster *) pRaster;
    RwInt32             nFormat = flags & (RwInt32) rwRASTERFORMATMASK;

    RWFUNCTION(RWSTRING("NullGetRasterFormat"));

    /* Stop warnings */
    pInOut = pInOut;

    /* Only 16 bits per pixel supported for Z buffers, 32 bits for all others */

    /* Copy over types */
    rpRas->cType = (RwUInt8) (flags & (RwInt32) rwRASTERTYPEMASK);
    rpRas->cFlags = (RwUInt8) (flags & (RwInt32) (~rwRASTERTYPEMASK));

    /* The types */

    /* If is not a camera buffer, then we need to allocate real memory for it
     */

    switch (rpRas->cType)
    {
        case rwRASTERTYPETEXTURE:
            {
                /* need to handle palettized textures */
                if ((nFormat & rwRASTERFORMATPIXELFORMATMASK) ==
                    rwRASTERFORMATDEFAULT)
                {
                    nFormat |= rwRASTERFORMAT8888;
                    rpRas->depth = 32;
                }
                else
                {
                    if ((nFormat & rwRASTERFORMATPIXELFORMATMASK) ==
                        rwRASTERFORMAT888)
                    {
                        if (0 != (nFormat & rwRASTERFORMATPAL4))
                        {
                            rpRas->depth = 4;
                        }
                        else if (0 != (nFormat & rwRASTERFORMATPAL8))
                        {
                            rpRas->depth = 8;
                        }
                        else
                        {
                            rpRas->depth = 24;
                        }
                    }
                    else if ((nFormat & rwRASTERFORMATPIXELFORMATMASK)
                             == rwRASTERFORMAT8888)
                    {
                        if (0 != (nFormat & rwRASTERFORMATPAL4))
                        {
                            rpRas->depth = 4;
                        }
                        else if (0 != (nFormat & rwRASTERFORMATPAL8))
                        {
                            rpRas->depth = 8;
                        }
                        else
                        {
                            rpRas->depth = 32;
                        }
                    }
                    else
                    {
                        RWERROR((E_RW_INVRASTERFORMAT));
                        RWRETURN(FALSE);
                    }
                }
                rpRas->cFormat = (RwUInt8) (nFormat >> 8);
                break;
            }
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPECAMERA:
            {
                if (!rpRas->depth)
                {
                    rpRas->depth = 32;
                }
                else if (rpRas->depth != 32)
                {
                    RWRETURN(FALSE);
                }

                /* By default textures are 8888 */
                if (nFormat == (RwInt32) rwRASTERFORMATDEFAULT)
                {
                    nFormat = (RwInt32) rwRASTERFORMAT8888;
                }

                rpRas->cFormat = (RwUInt8) (nFormat >> 8);

                break;
            }
        case rwRASTERTYPEZBUFFER:
            {
                if (!rpRas->depth)
                {
                    rpRas->depth = 16;
                }
                else if (rpRas->depth != 16)
                {
                    RWRETURN(FALSE);
                }

                /* By default, Z buffers of 16 bit */
                if (nFormat == (RwInt32) rwRASTERFORMATDEFAULT)
                {
                    nFormat = (RwInt32) rwRASTERFORMAT16;
                }

                rpRas->cFormat = (RwUInt8) (nFormat >> 8);

                break;
            }
        default:
            {
                RWRETURN(FALSE);
            }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullImageFindFormat

 Finds if an image is or has an alpha channel

 On entry   : Image
            : Raster format (888 if not masked, 8888 otherwise) (OUT)
 On exit    :
 */

static void
NullImageFindFormat(RwImage * image, RwInt32 * format)
{
    RwInt32             x, y;
    RwInt32             width, height, stride, depth;
    RwUInt8            *pixels;

    RWFUNCTION(RWSTRING("NullImageFindFormat"));
    RWASSERT(image);

    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);
    stride = RwImageGetStride(image);
    depth = RwImageGetDepth(image);
    pixels = RwImageGetPixels(image);

    *format = 0;

    if ((depth == 4) || (depth == 8))
    {
        /* Its a 4 or 8 bit image */
        RwRGBA             *palette = RwImageGetPalette(image);

        /* handle paletted rasters */
        if (4 == depth)
        {
            (*format) |= rwRASTERFORMATPAL4;
        }
        else
        {
            (*format) |= rwRASTERFORMATPAL8;
        }

        for (y = 0; y < height; y++)
        {
            RwUInt8            *currPixel = pixels;

            for (x = 0; x < width; x++)
            {
                if (palette[*currPixel].alpha != 0xff)
                {
                    /* has an alpha channel */
                    (*format) |= rwRASTERFORMAT8888;

                    RWRETURNVOID();
                }

                /* Next pixel */

                currPixel++;
            }

            pixels += stride;
        }

        /* Give the appropriate format */
        (*format) |= rwRASTERFORMAT888;
    }
    else if (depth == 32)
    {
        /* We have an 32 bit image */
        for (y = 0; y < height; y++)
        {
            RwRGBA             *currPixel = (RwRGBA *) pixels;

            for (x = 0; x < width; x++)
            {
                if (currPixel->alpha != 0xff)
                {
                    /* has an alpha channel */
                    (*format) |= rwRASTERFORMAT8888;

                    RWRETURNVOID();
                }

                /* Next pixel */

                currPixel++;
            }

            pixels += stride;
        }

        /* Give the appropriate format */
        (*format) |= rwRASTERFORMAT888;
    }

    RWRETURNVOID();
}

/****************************************************************************
 NullImageFindRasterFormat

 On entry   : Raster (OUT)
            : Image
            : Flags
 On exit    : TRUE on success
 */
static RwBool
NullImageFindRasterFormat(void *pRaster, void *pImage, RwInt32 flags)
{
    RwRaster           *rpRas = (RwRaster *) pRaster;
    RwImage            *ipImage = (RwImage *) pImage;

    RWFUNCTION(RWSTRING("NullImageFindRasterFormat"));

    /* Do the default thing first */
    rpRas->width = ipImage->width;
    rpRas->height = ipImage->height;
    rpRas->depth = 0;          /* Use the default depth for now... */

    /* Try to find the format with what we've got */

    if (!NullGetRasterFormat(NULL, pRaster, flags))
    {
        RWRETURN(FALSE);
    }

    /* The format is already set up */

    /* See whats possible */

    switch (rpRas->cType)
    {
        case rwRASTERTYPETEXTURE:
            {
                RwInt32             format;

                if (rwRASTERFORMATMIPMAP & flags)
                {
                    /* If mipmapped make it a power of two */
                    rpRas->width = 1 << RWLONGFINDMSB(rpRas->width);
                    rpRas->height = 1 << RWLONGFINDMSB(rpRas->height);
                }

                NullImageFindFormat(ipImage, &format);

                /*
                 * rpRas->cFormat = (RwUInt8)((format|
                 * (flags & (rwRASTERFORMATMIPMAP |
                 * rwRASTERFORMATAUTOMIPMAP)) >> 8));
                 */
                rpRas->cFormat = (RwUInt8) ((format |
                                             (flags &
                                              (rwRASTERFORMATMIPMAP |
                                               rwRASTERFORMATAUTOMIPMAP)))
                                            >> 8);

                /* We have our format */
                RWRETURN(TRUE);
            }
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPECAMERA:
        case rwRASTERTYPEZBUFFER:
            {
                /* Just take the default case */

                RWRETURN(TRUE);
            }
    }

    RWRETURN(FALSE);
}


/****************************************************************************
 NullRasterGetMipLevelSize

 Create a raster

 On entry   : NULL
            : pRaster - raster to allocate
            : miplevel to calculate size
 On exit    : size of miplevel in memory
 */


static RwUInt32
NullRasterGetMipLevelSize(void *pRaster, RwUInt32 miplevel)
{
    RwRaster           *rpRas = (RwRaster *) pRaster;
    RwInt32             size = 0;
    RwUInt32            nFormat = rpRas->cFormat << 8;
    RwInt32             width;
    RwInt32             height;
    RwUInt32            rasterFormat;

    RWFUNCTION(RWSTRING("NullRasterGetMipLevelSize"));

    width = rpRas->width >> miplevel;
    height = rpRas->height >> miplevel;
    width = (0 == width) ? 1 : width;
    height = (0 == height) ? 1 : height;

    rasterFormat = RwRasterGetFormat(rpRas);

    switch (nFormat & rwRASTERFORMATPIXELFORMATMASK)
    {
        case rwRASTERFORMAT8888:
            {
                /* handle palettized textures */
                if ((0 !=
                     (rasterFormat &
                      rwRASTERFORMATPAL4))
                    || (0 !=
                        (rasterFormat &
                         rwRASTERFORMATPAL8)))
                {
                    size = width * height;
                }
                else
                {
                    size = width * height * 4;
                }

                break;
            }
        case rwRASTERFORMAT888:
            {
                /* handle palettized textures */
                if ((0 !=
                     (rasterFormat &
                      rwRASTERFORMATPAL4))
                    || (0 !=
                        (rasterFormat &
                         rwRASTERFORMATPAL8)))
                {
                    size = width * height;
                }
                else
                {
                    size = width * height * 3;
                }

                break;
            }
        default:
            {
                RWERROR((E_RW_INVRASTERFORMAT));
            }
            break;
    }

    RWRETURN(size);
}

/****************************************************************************
 NullRasterGetMipLevels

 On entry   : Pixel pointer
            : raster
            : Flags
 On exit    : TRUE on success
 */
static RwBool
NullRasterGetMipLevels(void *pOut,
                       void *pInOut,
                       RwInt32 nI __RWUNUSED__ )
{
    RwRaster           *raster = (RwRaster *) pInOut;
    RwInt32            *numMipLevels = (RwInt32 *) pOut;

    RWFUNCTION(RWSTRING("NullRasterGetMipLevels"));
    RWASSERT(raster);
    RWASSERT(numMipLevels);

    /* Find the number of mipmaps */
    if (raster->width > raster->height)
    {
        *numMipLevels = ((RwUInt8)RWLONGFINDMSB(raster->width) + 1);
    }
    else
    {
        *numMipLevels = ((RwUInt8)RWLONGFINDMSB(raster->height) + 1);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterCreate

 Create a raster

 On entry   : NULL
            : pRaster - raster to allocate
            : Flags
 On exit    : TRUE on success
 */
static RwBool
NullRasterCreate(void *pInOut, void *pRaster, RwInt32 flags)
{
    RwRaster           *rpRas = (RwRaster *) pRaster;

    RwInt32             nFormat = flags & (RwInt32) rwRASTERFORMATMASK;

    RwUInt32            rasterFormat;

    RWFUNCTION(RWSTRING("NullRasterCreate"));

    /* Stop warnings */
    pInOut = pInOut;

    if (!NullGetRasterFormat(pInOut, pRaster, flags))
    {
        RWRETURN(FALSE);
    }

    rasterFormat = RwRasterGetFormat(rpRas);

    /* Set up */

    rpRas->cpPixels = NULL;

    /* If is not a camera buffer, then we need to allocate real memory for it
     */

    if ((rpRas->width) && (rpRas->height))
    {
        switch (rpRas->cType)
        {
            case rwRASTERTYPETEXTURE:
                {
                    RwInt32 numMipLevels = 0;
                    RwInt32 pixelSize = 0;

                    /* Mipmapped textures must be a power of 2 in width & height */
                    if (rwRASTERFORMATMIPMAP & rasterFormat)
                    {
                        RwUInt32    maxWidthBit, maxHeightBit;

                        maxWidthBit = RWLONGFINDMSB(rpRas->width);
                        maxHeightBit = RWLONGFINDMSB(rpRas->height);

                        if (((1 << maxWidthBit) != rpRas->width) ||
                            ((1 << maxHeightBit) != rpRas->height))
                        {
                            RWERROR((E_RW_DEVICEERROR,
                                RWSTRING("Mipmapped rasters must be power of 2")));
                        }
                    }

                    /* First allocate the pixel data */
                    NullRasterGetMipLevels((void *)&numMipLevels, rpRas, 0);

                    while (numMipLevels--)
                    {
                        pixelSize += NullRasterGetMipLevelSize(rpRas, numMipLevels);
                    }

                    switch (nFormat & rwRASTERFORMATPIXELFORMATMASK)
                    {
                        case rwRASTERFORMAT8888:
                            {
                                /* handle palettized textures */
                                if ((0 !=
                                     (rasterFormat &
                                      rwRASTERFORMATPAL4))
                                    || (0 !=
                                        (rasterFormat &
                                         rwRASTERFORMATPAL8)))
                                {
                                    rpRas->stride = rpRas->width;
                                    rpRas->cpPixels =
                                        (RwUInt8 *) RwDriverMalloc(pixelSize,
                                           rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
                                    if (!rpRas->cpPixels)
                                    {
                                        RWRETURN(FALSE);
                                    }

                                    rpRas->palette =
                                        (RwUInt8 *)
                                        RwDriverMalloc(sizeof(RwRGBA) *
                                           256,
                                           rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
                                    if (NULL == rpRas->palette)
                                    {
                                        RwDriverFree(rpRas->cpPixels);
                                        rpRas->cpPixels =
                                            (RwUInt8 *) NULL;
                                        RWRETURN(FALSE);
                                    }
                                }
                                else
                                {
                                    rpRas->stride = 4 * rpRas->width;
                                    rpRas->cpPixels =
                                        (RwUInt8 *) RwDriverMalloc(pixelSize,
                                           rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
                                    if (!rpRas->cpPixels)
                                    {
                                        RWRETURN(FALSE);
                                    }
                                }

                                break;
                            }
                        case rwRASTERFORMAT888:
                            {
                                /* handle palettized textures */
                                if ((0 !=
                                     (rasterFormat &
                                      rwRASTERFORMATPAL4))
                                    || (0 !=
                                        (rasterFormat &
                                         rwRASTERFORMATPAL8)))
                                {
                                    rpRas->stride = rpRas->width;
                                    rpRas->cpPixels =
                                        (RwUInt8 *) RwDriverMalloc(pixelSize,
                                            rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
                                    if (!rpRas->cpPixels)
                                    {
                                        RWRETURN(FALSE);
                                    }

                                    rpRas->palette =
                                        (RwUInt8 *)
                                        RwDriverMalloc(sizeof(RwRGBA) *
                                           256,
                                           rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
                                    if (NULL == rpRas->palette)
                                    {
                                        RwDriverFree(rpRas->cpPixels);
                                        rpRas->cpPixels =
                                            (RwUInt8 *) NULL;
                                        RWRETURN(FALSE);
                                    }
                                }
                                else
                                {
                                    rpRas->stride = 3 * rpRas->width;
                                    rpRas->cpPixels =
                                        (RwUInt8 *) RwDriverMalloc(pixelSize,
                                           rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
                                    if (!rpRas->cpPixels)
                                    {
                                        RWRETURN(FALSE);
                                    }
                                }

                                break;
                            }
                        default:
                            {
                                RWERROR((E_RW_INVRASTERFORMAT));
                                RWRETURN(FALSE);
                            }
                            break;
                    }
                    break;
                }
            case rwRASTERTYPECAMERA:
            case rwRASTERTYPENORMAL:
                {
                    /* Calculate stride */
                    rpRas->stride = 4 * rpRas->width;

                    /* Allocate space */
                    rpRas->cpPixels =
                        (RwUInt8 *) RwDriverMalloc(rpRas->stride *
                               rpRas->height,
                               rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

                    if (!rpRas->cpPixels)
                    {
                        RWRETURN(FALSE);
                    }
                    break;
                }
            case rwRASTERTYPEZBUFFER:
                {
                    /* Calculate stride */
                    rpRas->stride = 2 * rpRas->width;

                    /* Allocate space */
                    rpRas->cpPixels =
                        (RwUInt8 *) RwDriverMalloc(rpRas->stride *
                           rpRas->height,
                           rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

                    if (!rpRas->cpPixels)
                    {
                        RWRETURN(FALSE);
                    }
                    break;
                }
            default:
                {
                    RWRETURN(FALSE);
                }
        }
    }
    else
    {
        rpRas->cFlags = (RwUInt8) rwRASTERDONTALLOCATE; /* Not allocated */
        rpRas->stride = 0;
    }

    /* cache the original width, height and stride */
    rpRas->originalWidth = rpRas->width;
    rpRas->originalHeight = rpRas->height;
    rpRas->originalStride = rpRas->stride;
    rpRas->originalPixels = rpRas->cpPixels;


    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterSubRaster

 On entry   : Raster (OUT)
            : Raster (IN)
            : None
 On exit    : TRUE on success
 */
static RwBool
NullRasterSubRaster(void *pRas, void *pIn, RwInt32 nIn __RWUNUSED__ )
{
    RwRaster           *rpRas = (RwRaster *) pRas;
    RwRaster           *rpIn = (RwRaster *) pIn;
    RwRaster           *pixelOwner = rpIn->parent;

    RWFUNCTION(RWSTRING("NullRasterSubRaster"));

    /* Core already set up offset and size */

    /* This baby has most certainly not been allocated */
    rpRas->cFormat = rpIn->cFormat;
    rpRas->stride = rpIn->stride;
    rpRas->depth = rpIn->depth;
    rpRas->cType = rpIn->cType;

    switch (rpIn->depth)
    {
        case 4:
            {
                rpRas->cpPixels = (pixelOwner->cpPixels) +
                    (pixelOwner->stride * rpRas->nOffsetY) +
                    (rpRas->nOffsetX);
                rpRas->palette = pixelOwner->palette;
            }
            break;

        case 8:
            {
                rpRas->cpPixels = (pixelOwner->cpPixels) +
                    (pixelOwner->stride * rpRas->nOffsetY) +
                    (rpRas->nOffsetX);
                rpRas->palette = pixelOwner->palette;
            }
            break;

        case 24:
            {
                rpRas->cpPixels = (pixelOwner->cpPixels) +
                    (pixelOwner->stride * rpRas->nOffsetY) +
                    (rpRas->nOffsetX * 3);
                break;
            }
        case 32:
            {
                rpRas->cpPixels = (pixelOwner->cpPixels) +
                    (pixelOwner->stride * rpRas->nOffsetY) +
                    (rpRas->nOffsetX * 4);
                break;
            }
        default:
            {
                RWERROR((E_RW_INVRASTERFORMAT));
                RWRETURN(FALSE);
            }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterDestroy

 On entry   : NULL
            : Raster to destroy
 On exit    : TRUE on success
 */
static RwBool
NullRasterDestroy(void *pOut, void *pRaster, RwInt32 nIn)
{
    RwRaster           *rpRas = (RwRaster *) pRaster;

    RWFUNCTION(RWSTRING("NullRasterDestroy"));

    /* Stop warnings */
    pOut = pOut;
    nIn = nIn;

    if (!(rpRas->cFlags & (RwUInt8) rwRASTERDONTALLOCATE))
    {
        RwUInt32            rasterFormat;

        RwDriverFree(rpRas->cpPixels);
        rpRas->cpPixels = (RwUInt8 *) NULL;

        /* handle palettized textures */
        rasterFormat = RwRasterGetFormat(rpRas);
        if ((0 != (rasterFormat & rwRASTERFORMATPAL4)) ||
            (0 != (rasterFormat & rwRASTERFORMATPAL8)))
        {
            RwDriverFree(rpRas->palette);
            rpRas->palette = (RwUInt8 *) NULL;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterLock

 On entry   : Pixel pointer
            : raster
            : Flags
 On exit    : TRUE on success
 */
static RwBool
NullRasterLock(void *pPixels, void *pRaster, RwInt32 flags)
{
    RwUInt8           **cppPixels = (RwUInt8 **) pPixels;
    RwRaster           *raster = (RwRaster *) pRaster;
    RwUInt8             mipLevel =
        (RwUInt8) (((RwInt32) 0xFF00 & flags) >> 8);
    RwUInt8             level;
    RwUInt32            offset = 0;

    RWFUNCTION(RWSTRING("NullRasterLock"));

    flags = flags & 0xFF;

    if (raster->width && raster->height)
    {
        level = mipLevel;
        while (level--)
        {
            offset += NullRasterGetMipLevelSize(raster, level);
        }

        raster->cpPixels = raster->cpPixels + offset;
        (*cppPixels) = raster->cpPixels;

        raster->width = raster->width >> mipLevel;
        raster->height = raster->height >> mipLevel;
        raster->width = (0 == raster->width) ? 1 : raster->width;
        raster->height = (0 == raster->height) ? 1 : raster->height;
        raster->stride = raster->stride >> (RWLONGFINDMSB(raster->originalWidth)-
                                            RWLONGFINDMSB(raster->width));
    }

    if (flags & rwRASTERLOCKREAD)
    {
        raster->privateFlags |= rwRASTERPIXELLOCKEDREAD;
    }
    if (flags & rwRASTERLOCKWRITE)
    {
        raster->privateFlags |= rwRASTERPIXELLOCKEDWRITE;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterLockPalette

 On entry   : Pixel pointer
            : raster
            : Flags
 On exit    : TRUE on success
 */
static RwBool
NullRasterLockPalette(void *paletteInOut,
                      void *rasterIn, RwInt32 accessMode)
{
    RwRaster           *raster;

    RWFUNCTION(RWSTRING("NullRasterLockPalette"));

    raster = (RwRaster *) rasterIn;

    switch (raster->cType & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            {
                if (0 != (accessMode & rwRASTERLOCKWRITE))
                {
                    raster->privateFlags =
                        (RwUInt8) (raster->
                                   privateFlags |
                                   rwRASTERPALETTELOCKED);
                }
                else
                {
                    raster->privateFlags =
                        (RwUInt8) (raster->
                                   privateFlags |
                                   rwRASTERPALETTELOCKEDREAD);
                }

                *((RwUInt8 **) paletteInOut) = raster->palette;
            }
            break;

        default:
            RWERROR((E_RW_INVRASTERLOCKREQ));
            RWRETURN(FALSE);
            break;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterUnlock

 On entry   : NULL
            : Rastre
            : Flags
 On exit    : TRUE on success
 */
static RwBool
NullRasterUnlock(void *pInOut, void *pRaster, RwInt32 flags)
{
    RwRaster           *raster = (RwRaster *) pRaster;

    RWFUNCTION(RWSTRING("NullRasterUnlock"));

    /* Stop warnings */
    pInOut = pInOut;
    pRaster = pRaster;
    flags = flags;

    if ((raster->privateFlags & rwRASTERPIXELLOCKEDWRITE) &&
        raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8))
    {
        raster->privateFlags &= ~rwRASTERPIXELLOCKED;

        RwTextureRasterGenerateMipmaps(raster, NULL);
    }
    else
    {
        raster->privateFlags &= ~rwRASTERPIXELLOCKED;
    }
    raster->width = raster->originalWidth;
    raster->height = raster->originalHeight;
    raster->stride = raster->originalStride;
    raster->cpPixels = raster->originalPixels;

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterUnlockPalette

 On entry   : NULL
            : Raster
            : Flags
 On exit    : TRUE on success
 */
static RwBool
NullRasterUnlockPalette(void *unused1 __RWUNUSED__,
                        void *rasterIn,
                        RwInt32 unused3 __RWUNUSED__ )
{
    RwRaster           *raster;

    RWFUNCTION(RWSTRING("NullRasterUnlockPalette"));

    raster = (RwRaster *) rasterIn;

    switch (raster->cType & rwRASTERTYPEMASK)
    {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            {
                raster->privateFlags &= ~rwRASTERPALETTELOCKED;
            }
            break;

        default:
            RWERROR((E_RW_INVRASTERLOCKREQ));
            RWRETURN(FALSE);
            break;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterClear

 On entry   : NULL
            : NULL
            : Pixel value defining what to clear the raster to
 On exit    : TRUE on success
 */
static RwBool
NullRasterClear(void *pOut __RWUNUSED__,
                void *pInOut __RWUNUSED__,
                RwInt32 nValue __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullRasterClear"));

    /* Do nothing here */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterClearRect

 On entry   : NULL
            : RwRect defining area to clear
            : Pixel value defining what to clear the raster to
 On exit    : TRUE on success
 */
static RwBool
NullRasterClearRect(void *pOut __RWUNUSED__,
                    void *pRect __RWUNUSED__,
                    RwInt32 nAvlue __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullRasterClearRect"));

    /* Do nothing */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterRender

 On entry   : Raster to render (blit)
            : RwRect defining location to blit to (take x and y only)
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullRasterRender(void *pRas __RWUNUSED__,
                 void *pLoc __RWUNUSED__,
                 RwInt32 nData __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullRasterRender"));

    /* Do nothing */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterRenderScaled

 On entry   : Raster to render (blit)
            : RwRect defining region to blit to (take x,
 y,
 w and h)
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullRasterRenderScaled(void *pRas __RWUNUSED__,
                       void *pRect __RWUNUSED__,
                       RwInt32 nData __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullRasterRenderScaled"));

    /* Do nothing */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterRenderFast

 On entry   : Raster to render (blit) - raster has no mask info, so optimise for this
            : RwRect defining location to blit to (take x and y only)
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullRasterRenderFast(void *pRas __RWUNUSED__,
                     void *pLoc __RWUNUSED__,
                     RwInt32 nData __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullRasterRenderFast"));

    /* Do nothing */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterSetContext

 On entry   : NULL
            : Raster to be the destination of future raster ops.
            : 0
 On exit    : TRUE on success
 */
static RwBool
NullRasterSetContext(void *pOut __RWUNUSED__,
                     void *pRas __RWUNUSED__,
                     RwInt32 nData __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullRasterSetContext"));

    /* Do nothing */
    RWRETURN(TRUE);
}

/****************************************************************************
 NullImageGetRaster

 On entry   : Image (MODIFY)
            : Raster
 On exit    : TRUE on success
 */
static RwBool
NullImageGetRaster(void *pImage,
                   void *pRaster,
                   RwInt32 nNum __RWUNUSED__ )
{
    RwRaster           *raster = (RwRaster *) pRaster;
    RwImage            *image = (RwImage *) pImage;
    RwBool              alreadyPixelLocked = FALSE;
    RwBool              alreadyPaletteLocked = FALSE;
    RwBool              result = TRUE;

    RWFUNCTION(RWSTRING("NullImageGetRaster"));

    if (image->cpPixels)
    {
        RwInt32             x;
        RwInt32             y;
        RwUInt8            *srcPixels = NULL;
        RwUInt8            *dstPixels;
        RwInt32             rasterFormat;
        RwRasterFormat      format;

        RwBool              palettized;

        if ((image->width < raster->width) ||
            (image->height < raster->height))
        {
            RWRETURN(FALSE);
        }

        if (raster->privateFlags & rwRASTERPIXELLOCKEDREAD)
        {
            srcPixels = raster->cpPixels;
            alreadyPixelLocked = TRUE;
        }
        else
        {
            srcPixels = RwRasterLock(raster, 0, rwRASTERLOCKREAD);
            if (srcPixels == NULL)
            {
                RWRETURN(FALSE);
            }
        }

        rasterFormat = RwRasterGetFormat(raster);
        format = (RwRasterFormat) rasterFormat;

        /* handle palettized textures */
        palettized = ((0 != (rasterFormat & rwRASTERFORMATPAL4)) ||
                      (0 != (rasterFormat & rwRASTERFORMATPAL8)));

        if (TRUE == palettized)
        {
            if (raster->privateFlags & rwRASTERPALETTELOCKEDREAD)
            {
                alreadyPaletteLocked = TRUE;
            }

            if (FALSE == alreadyPaletteLocked)
            {
                if (!RwRasterLockPalette(raster, rwRASTERLOCKREAD))
                {
                    RWRETURN(FALSE);
                }
            }
        }

        dstPixels = RwImageGetPixels(image);

        switch (format &
                (rwRASTERFORMATPIXELFORMATMASK | rwRASTERFORMATPAL4 |
                 rwRASTERFORMATPAL8))
        {
            case rwRASTERFORMAT888:
                {
                    RwUInt8            *rasterPixels;

                    RwRGBA             *imageRGBA;

                    for (y = 0; y < raster->height; y++)
                    {
                        rasterPixels = srcPixels;
                        imageRGBA = (RwRGBA *) dstPixels;
                        for (x = 0; x < raster->width; x++)
                        {
                            imageRGBA->red = *(rasterPixels++);
                            imageRGBA->green = *(rasterPixels++);
                            imageRGBA->blue = *(rasterPixels++);
                            imageRGBA->alpha = 0xFF;

                            imageRGBA++;
                        }
                        srcPixels += raster->stride;
                        dstPixels += image->stride;
                    }
                    break;
                }

            case rwRASTERFORMAT8888:
                {
                    RwRGBA             *rasterRGBA;

                    RwRGBA             *imageRGBA;

                    for (y = 0; y < raster->height; y++)
                    {
                        rasterRGBA = (RwRGBA *) srcPixels;
                        imageRGBA = (RwRGBA *) dstPixels;
                        for (x = 0; x < raster->width; x++)
                        {
                            imageRGBA->red = rasterRGBA->red;
                            imageRGBA->green = rasterRGBA->green;
                            imageRGBA->blue = rasterRGBA->blue;
                            imageRGBA->alpha = rasterRGBA->alpha;

                            imageRGBA++;
                            rasterRGBA++;
                        }
                        srcPixels += raster->stride;
                        dstPixels += raster->stride;
                    }
                    break;
                }

            case (rwRASTERFORMAT888 | rwRASTERFORMATPAL4):
                {
                    /* image depth can be 4, 8 or 32 */
                    switch (image->depth)
                    {
                        case 4:
                        case 8:
                            {
                                RwRGBA             *imagePalette;

                                RwUInt8            *rasterPalette;

                                imagePalette = image->palette;
                                rasterPalette =
                                    (RwUInt8 *) (raster->palette);

                                for (x = 0; x < 16; x += 1)
                                {
                                    imagePalette[x].red =
                                        *(rasterPalette++);
                                    imagePalette[x].green =
                                        *(rasterPalette++);
                                    imagePalette[x].blue =
                                        *(rasterPalette++);
                                    imagePalette[x].alpha = 0xFF;
                                }

                                for (y = 0; y < raster->height; y += 1)
                                {
                                    RwUInt8            *imagePixels;

                                    RwUInt8            *rasterPixels;

                                    imagePixels = (RwUInt8 *) dstPixels;
                                    rasterPixels =
                                        (RwUInt8 *) srcPixels;

                                    for (x = 0; x < raster->width;
                                         x += 1)
                                    {
                                        *imagePixels = *rasterPixels;

                                        imagePixels += 1;
                                        rasterPixels += 1;
                                    }

                                    dstPixels += image->stride;
                                    srcPixels += raster->stride;
                                }
                            }
                            break;

                        case 32:
                            {
                                RwUInt8            *rasterPalette;

                                rasterPalette =
                                    (RwUInt8 *) (raster->palette);

                                for (y = 0; y < raster->height; y += 1)
                                {
                                    RwRGBA             *imagePixels;

                                    RwUInt8            *rasterPixels;

                                    imagePixels = (RwRGBA *) dstPixels;
                                    rasterPixels =
                                        (RwUInt8 *) srcPixels;

                                    for (x = 0; x < raster->width;
                                         x += 1)
                                    {
                                        RwUInt8            *rasterPixel;

                                        rasterPixel =
                                            rasterPalette +
                                            3 * (*rasterPixels);

                                        imagePixels->red =
                                            *(rasterPixel++);
                                        imagePixels->green =
                                            *(rasterPixel++);
                                        imagePixels->blue =
                                            *(rasterPixel++);
                                        imagePixels->alpha = 0xFF;

                                        imagePixels += 1;
                                        rasterPixels += 1;
                                    }

                                    dstPixels += image->stride;
                                    srcPixels += raster->stride;
                                }
                            }
                            break;

                        default:
                            {
                                RWERROR((E_RW_INVIMAGEDEPTH));
                            }
                    }
                }
                break;

            case (rwRASTERFORMAT8888 | rwRASTERFORMATPAL4):
                {
                    /* image depth can be 4, 8 or 32 */
                    switch (image->depth)
                    {
                        case 4:
                        case 8:
                            {
                                RwRGBA             *imagePalette;

                                RwRGBA             *rasterPalette;

                                imagePalette = image->palette;
                                rasterPalette =
                                    (RwRGBA *) (raster->palette);

                                for (x = 0; x < 16; x += 1)
                                {
                                    imagePalette[x].red =
                                        rasterPalette[x].red;
                                    imagePalette[x].green =
                                        rasterPalette[x].green;
                                    imagePalette[x].blue =
                                        rasterPalette[x].blue;
                                    imagePalette[x].alpha =
                                        rasterPalette[x].alpha;
                                }

                                for (y = 0; y < raster->height; y += 1)
                                {
                                    RwUInt8            *imagePixels;

                                    RwUInt8            *rasterPixels;

                                    imagePixels = (RwUInt8 *) dstPixels;
                                    rasterPixels =
                                        (RwUInt8 *) srcPixels;

                                    for (x = 0; x < raster->width;
                                         x += 1)
                                    {
                                        *imagePixels = *rasterPixels;

                                        imagePixels += 1;
                                        rasterPixels += 1;
                                    }

                                    dstPixels += image->stride;
                                    srcPixels += raster->stride;
                                }
                            }
                            break;

                        case 32:
                            {
                                RwRGBA             *rasterPalette;

                                rasterPalette =
                                    (RwRGBA *) (raster->palette);

                                for (y = 0; y < raster->height; y += 1)
                                {
                                    RwRGBA             *imagePixels;

                                    RwUInt8            *rasterPixels;

                                    imagePixels = (RwRGBA *) dstPixels;
                                    rasterPixels =
                                        (RwUInt8 *) srcPixels;

                                    for (x = 0; x < raster->width;
                                         x += 1)
                                    {
                                        *imagePixels =
                                            (rasterPalette
                                             [*rasterPixels]);

                                        imagePixels += 1;
                                        rasterPixels += 1;
                                    }

                                    dstPixels += image->stride;
                                    srcPixels += raster->stride;
                                }
                            }
                            break;

                        default:
                            {
                                RWERROR((E_RW_INVIMAGEDEPTH));
                            }
                    }
                }
                break;

            case (rwRASTERFORMAT888 | rwRASTERFORMATPAL8):
                {
                    /* image depth can be 8 or 32 */
                    switch (image->depth)
                    {
                        case 8:
                            {
                                RwRGBA             *imagePalette;

                                RwUInt8            *rasterPalette;

                                imagePalette = image->palette;
                                rasterPalette =
                                    (RwUInt8 *) (raster->palette);

                                for (x = 0; x < 256; x += 1)
                                {
                                    imagePalette[x].red =
                                        *(rasterPalette++);
                                    imagePalette[x].green =
                                        *(rasterPalette++);
                                    imagePalette[x].blue =
                                        *(rasterPalette++);
                                    imagePalette[x].alpha = 0xFF;
                                }

                                for (y = 0; y < raster->height; y += 1)
                                {
                                    RwUInt8            *imagePixels;

                                    RwUInt8            *rasterPixels;

                                    imagePixels = (RwUInt8 *) dstPixels;
                                    rasterPixels =
                                        (RwUInt8 *) srcPixels;

                                    for (x = 0; x < raster->width;
                                         x += 1)
                                    {
                                        *imagePixels = *rasterPixels;

                                        rasterPixels += 1;
                                        imagePixels += 1;
                                    }

                                    dstPixels += image->stride;
                                    srcPixels += raster->stride;
                                }
                            }
                            break;

                        case 32:
                            {
                                RwUInt8            *rasterPalette;

                                rasterPalette =
                                    (RwUInt8 *) (raster->palette);

                                for (y = 0; y < raster->height; y += 1)
                                {
                                    RwRGBA             *imagePixels;

                                    RwUInt8            *rasterPixels;

                                    imagePixels = (RwRGBA *) dstPixels;
                                    rasterPixels =
                                        (RwUInt8 *) srcPixels;

                                    for (x = 0; x < raster->width;
                                         x += 1)
                                    {
                                        RwUInt8            *rasterPixel;

                                        rasterPixel =
                                            rasterPalette +
                                            3 * (*rasterPixels);

                                        imagePixels->red =
                                            *(rasterPixel++);
                                        imagePixels->green =
                                            *(rasterPixel++);
                                        imagePixels->blue =
                                            *(rasterPixel++);
                                        imagePixels->alpha = 0xFF;

                                        imagePixels += 1;
                                        rasterPixels += 1;
                                    }

                                    dstPixels += image->stride;
                                    srcPixels += raster->stride;
                                }
                            }
                            break;

                        default:
                            {
                                RWERROR((E_RW_INVIMAGEDEPTH));
                            }
                    }
                }
                break;

            case (rwRASTERFORMAT8888 | rwRASTERFORMATPAL8):
                {
                    /* image depth can be 8 or 32 */
                    switch (image->depth)
                    {
                        case 8:
                            {
                                RwRGBA             *imagePalette;

                                RwRGBA             *rasterPalette;

                                imagePalette = image->palette;
                                rasterPalette =
                                    (RwRGBA *) raster->palette;

                                for (x = 0; x < 256; x += 1)
                                {
                                    imagePalette[x].red =
                                        rasterPalette[x].red;
                                    imagePalette[x].green =
                                        rasterPalette[x].green;
                                    imagePalette[x].blue =
                                        rasterPalette[x].blue;
                                    imagePalette[x].alpha =
                                        rasterPalette[x].alpha;
                                }

                                for (y = 0; y < raster->height; y += 1)
                                {
                                    RwUInt8            *imagePixels;

                                    RwUInt8            *rasterPixels;

                                    imagePixels = (RwUInt8 *) dstPixels;
                                    rasterPixels =
                                        (RwUInt8 *) srcPixels;

                                    for (x = 0; x < raster->width;
                                         x += 1)
                                    {
                                        *imagePixels = *rasterPixels;

                                        rasterPixels += 1;
                                        imagePixels += 1;
                                    }

                                    dstPixels += image->stride;
                                    srcPixels += raster->stride;
                                }
                            }
                            break;

                        case 32:
                            {
                                RwRGBA             *rasterPalette;

                                rasterPalette =
                                    (RwRGBA *) (raster->palette);

                                for (y = 0; y < raster->height; y += 1)
                                {
                                    RwRGBA             *imagePixels;

                                    RwUInt8            *rasterPixels;

                                    imagePixels = (RwRGBA *) dstPixels;
                                    rasterPixels =
                                        (RwUInt8 *) srcPixels;

                                    for (x = 0; x < raster->width;
                                         x += 1)
                                    {
                                        *imagePixels =
                                            (rasterPalette
                                             [*rasterPixels]);

                                        imagePixels += 1;
                                        rasterPixels += 1;
                                    }

                                    dstPixels += image->stride;
                                    srcPixels += raster->stride;
                                }
                            }
                            break;

                        default:
                            {
                                RWERROR((E_RW_INVIMAGEDEPTH));
                            }
                    }
                }
                break;

            default:
                {
                    RWERROR((E_RW_INVRASTERFORMAT));
                    result = FALSE;
                }
        }

        if (TRUE == palettized)
        {
            if (FALSE == alreadyPaletteLocked)
            {
                RwRasterUnlockPalette(raster);
            }
        }

        if (!alreadyPixelLocked)
        {
            RwRasterUnlock(raster);
        }
        RWRETURN(result);
    }

    RWERROR((E_RW_INVIMAGEFORMAT));
    RWRETURN(FALSE);
}

/****************************************************************************
 NullRasterNormalSetImage

 On entry   : Raster (MODIFY)
            : Image
 On exit    : TRUE on success
 */
static RwBool
NullRasterNormalSetImage(RwRaster * raster, RwImage * image)
{
    RwRasterFormat      format;

    RwInt32             x;

    RwInt32             y;

    RWFUNCTION(RWSTRING("NullRasterNormalSetImage"));

    RWASSERT(NULL != raster);
    RWASSERT(NULL != image);

    format = RwRasterGetFormat(raster);

    switch (image->depth)
    {
        case 4:
        case 8:
            {
                RwRGBA             *imagePal;

                RwUInt8            *imagePix;

                RwUInt8            *rasterPix;

                /* Its a 4 or 8 bit image */
                imagePal = image->palette;
                imagePix = image->cpPixels;
                rasterPix = raster->cpPixels;

                switch (format & rwRASTERFORMATPIXELFORMATMASK)
                {
                    case rwRASTERFORMAT888:
                        {
                            for (y = 0; y < image->height; y += 1)
                            {
                                RwUInt8            *imagePixCur;

                                RwUInt8            *rasterPixCur;

                                imagePixCur = imagePix;
                                rasterPixCur = rasterPix;

                                for (x = 0; x < image->width; x += 1)
                                {
                                    RwRGBA             *col;

                                    col = imagePal + (*imagePixCur);

                                    *(rasterPixCur++) = col->red;
                                    *(rasterPixCur++) = col->green;
                                    *(rasterPixCur++) = col->blue;

                                    imagePixCur += 1;
                                }

                                imagePix += image->stride;
                                rasterPix += raster->stride;
                            }
                        }
                        break;

                    case rwRASTERFORMAT8888:
                        {
                            for (y = 0; y < image->height; y += 1)
                            {
                                RwUInt8            *imagePixCur;

                                RwUInt8            *rasterPixCur;

                                imagePixCur = imagePix;
                                rasterPixCur = rasterPix;

                                for (x = 0; x < image->width; x += 1)
                                {
                                    RwRGBA             *col;

                                    col = imagePal + (*imagePixCur);

                                    *(rasterPixCur++) = col->red;
                                    *(rasterPixCur++) = col->green;
                                    *(rasterPixCur++) = col->blue;
                                    *(rasterPixCur++) = col->alpha;

                                    imagePixCur += 1;
                                }

                                imagePix += image->stride;
                                rasterPix += raster->stride;
                            }
                        }
                        break;

                    default:
                        {
                            RWERROR((E_RW_INVRASTERFORMAT));
                            RWRETURN(FALSE);
                        }
                }
                break;
            }

        case 32:
            {
                RwUInt8            *imagePix;

                RwUInt8            *rasterPix;

                /* Its a 32 bit image */
                imagePix = image->cpPixels;
                rasterPix = raster->cpPixels;

                switch (format & rwRASTERFORMATPIXELFORMATMASK)
                {
                    case rwRASTERFORMAT888:
                        {
                            for (y = 0; y < image->height; y += 1)
                            {
                                RwRGBA             *col;

                                RwUInt8            *rasterPixCur;

                                col = (RwRGBA *) imagePix;
                                rasterPixCur = rasterPix;

                                for (x = 0; x < image->width; x += 1)
                                {
                                    *(rasterPixCur++) = col->red;
                                    *(rasterPixCur++) = col->green;
                                    *(rasterPixCur++) = col->blue;

                                    col += 1;
                                }

                                imagePix += image->stride;
                                rasterPix += raster->stride;
                            }
                        }
                        break;

                    case rwRASTERFORMAT8888:
                        {
                            for (y = 0; y < image->height; y += 1)
                            {
                                RwRGBA             *col;

                                RwUInt8            *rasterPixCur;

                                col = (RwRGBA *) imagePix;
                                rasterPixCur = rasterPix;

                                for (x = 0; x < image->width; x += 1)
                                {
                                    *(rasterPixCur++) = col->red;
                                    *(rasterPixCur++) = col->green;
                                    *(rasterPixCur++) = col->blue;
                                    *(rasterPixCur++) = col->alpha;

                                    col += 1;
                                }

                                imagePix += image->stride;
                                rasterPix += raster->stride;
                            }
                        }
                        break;

                    default:
                        {
                            RWERROR((E_RW_INVRASTERFORMAT));
                            RWRETURN(FALSE);
                        }
                }
                break;
            }
        default:
            {
                RWERROR((E_RW_INVIMAGEDEPTH));
                RWRETURN(FALSE);
            }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterPalettizedSetImage

 On entry   : Raster (MODIFY)
            : Image
 On exit    : TRUE on success
 */
static RwBool
NullRasterPalettizedSetImage(RwRaster * raster, RwImage * image)
{
    RwRasterFormat      format;

    RwInt32             imageDepth;

    RwUInt8            *imagePix;

    RwInt32             rasterPalSize;

    RwUInt8            *rasterPal;

    RwInt32             i;

    RWFUNCTION(RWSTRING("NullRasterPalettizedSetImage"));

    RWASSERT(NULL != raster);
    RWASSERT(NULL != raster->palette);
    RWASSERT(NULL != image);

    format = RwRasterGetFormat(raster);
    imageDepth = RwImageGetDepth(image);
    imagePix = RwImageGetPixels(image);

    rasterPalSize = 0;
    if (0 != (format & rwRASTERFORMATPAL4))
    {
        rasterPalSize = 16;
    }
    else
    {
        rasterPalSize = 256;
    }
    RWASSERT(0 != rasterPalSize);

    rasterPal = (RwUInt8 *) (raster->palette);

    /* copy the palette */
    if (((imageDepth != 4) && (16 == rasterPalSize)) ||
        ((imageDepth != 8) && (256 == rasterPalSize)))
    {
        RwPalQuant          palQuant;

        RwBool              palQuantInitState;

        RwRGBA              palette[256];


        /* image depth and target raster palette are different sizes */
        palQuantInitState = RwPalQuantInit(&palQuant);
        RWASSERT(TRUE == palQuantInitState);

        RwPalQuantAddImage(&palQuant, image, 1.0f);
        RwPalQuantResolvePalette(palette, rasterPalSize, &palQuant);

        /* this should copy pixels too */
        RwPalQuantMatchImage(raster->cpPixels,
                             raster->stride,
                             raster->depth, FALSE, &palQuant, image);

        switch (format & rwRASTERFORMATPIXELFORMATMASK)
        {
            case rwRASTERFORMAT888:
                {
                    for (i = 0; i < rasterPalSize; i += 1)
                    {
                        rasterPal[i * 3] = palette[i].red;
                        rasterPal[i * 3 + 1] = palette[i].green;
                        rasterPal[i * 3 + 2] = palette[i].blue;
                    }
                }
                break;

            case rwRASTERFORMAT8888:
                {
                    for (i = 0; i < rasterPalSize; i += 1)
                    {
                        rasterPal[i * 4] = palette[i].red;
                        rasterPal[i * 4 + 1] = palette[i].green;
                        rasterPal[i * 4 + 2] = palette[i].blue;
                        rasterPal[i * 4 + 3] = palette[i].alpha;
                    }
                }
                break;

            default:
                {
                    RWERROR((E_RW_INVRASTERFORMAT));
                    RWRETURN(FALSE);
                }
        }

        RwPalQuantTerm(&palQuant);
    }
    else
    {
        const RwUInt8 *srcPixels;
        RwUInt8 *dstPixels;
        RwInt32 x, y;

        /* image and raster palette depths are equal */
        switch (format & rwRASTERFORMATPIXELFORMATMASK)
        {
            case rwRASTERFORMAT888:
                {
                    for (i = 0; i < rasterPalSize; i += 1)
                    {
                        rasterPal[i * 3] = image->palette[i].red;
                        rasterPal[i * 3 + 1] = image->palette[i].green;
                        rasterPal[i * 3 + 2] = image->palette[i].blue;
                    }
                }
                break;

            case rwRASTERFORMAT8888:
                {
                    for (i = 0; i < rasterPalSize; i += 1)
                    {
                        rasterPal[i * 4] = image->palette[i].red;
                        rasterPal[i * 4 + 1] = image->palette[i].green;
                        rasterPal[i * 4 + 2] = image->palette[i].blue;
                        rasterPal[i * 4 + 3] = image->palette[i].alpha;
                    }
                }
                break;

            default:
                {
                    RWERROR((E_RW_INVRASTERFORMAT));
                }
        }

        srcPixels = imagePix;
        dstPixels = raster->cpPixels;
        for (y = 0; y < image->height; y += 1)
        {
            for (x = 0; x < image->width; x += 1)
            {
                dstPixels[x] = srcPixels[x];
            }

            srcPixels += image->stride;
            dstPixels += raster->stride;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 NullRasterSetImage

 The raster is only set if it has a valid pixel pointer

 On entry   : Raster (MODIFY)
            : Image
            : Flags
 On exit    : TRUE on success
 */
static RwBool
NullRasterSetImage(void *pRaster, void *pImage, RwInt32 flags)
{
    RwRaster           *rpRas = (RwRaster *) pRaster;
    RwImage            *ipImage = (RwImage *) pImage;

    RwBool              alreadyPixelLocked = FALSE;
    RwBool              alreadyPaletteLocked = FALSE;
    RwBool              result = TRUE;
    RwInt32             rasterFormat;
    RwRasterFormat      nFormat;

#if (0)
    RwUInt8            *cpOut;
    RwUInt8            *cpIn;
#endif /* (0) */

    RWFUNCTION(RWSTRING("NullRasterSetImage"));

    RWASSERT(rpRas);
    RWASSERT(ipImage);

    rasterFormat = RwRasterGetFormat(rpRas);
    nFormat = (RwRasterFormat) rasterFormat;

#if (0)
    cpOut = (RwUInt8 *) rpRas->cpPixels;
    cpIn = (RwUInt8 *) ipImage->cpPixels;
#endif /* (0) */

    /* Stop warnings */
    flags = flags;

    if (rpRas->privateFlags & rwRASTERPIXELLOCKEDWRITE)
    {
        alreadyPixelLocked = TRUE;
    }
    else
    {
        if (!RwRasterLock(rpRas, 0, rwRASTERLOCKWRITE))
        {
            RWRETURN(FALSE);
        }
    }

    if (0 != (nFormat & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8)))
    {
        if (0 != (rpRas->privateFlags & rwRASTERPALETTELOCKEDWRITE))
        {
            alreadyPaletteLocked = TRUE;
        }

        if (FALSE == alreadyPaletteLocked)
        {
            if (FALSE ==
                RwRasterLockPalette(rpRas,
                                    rwRASTERLOCKWRITE |
                                    rwRASTERLOCKNOFETCH))
            {
                RWRETURN(FALSE);
            }
        }
    }

    /* route depends on the type of raster */
    if (0 != (nFormat & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8)))
    {
        result = NullRasterPalettizedSetImage(rpRas, ipImage);
    }
    else
    {
        result = NullRasterNormalSetImage(rpRas, ipImage);
    }

    if (0 != (nFormat & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8)))
    {
        if (FALSE == alreadyPaletteLocked)
        {
            RwRasterUnlockPalette(rpRas);
        }
    }

    if (!alreadyPixelLocked)
    {
        RwRasterUnlock(rpRas);
    }
    RWRETURN(result);
}

static RwBool
NullHintRenderFront2Back(void *pInOut __RWUNUSED__,
                         void *pIn __RWUNUSED__,
                         RwInt32 flags __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("NullHintRenderFront2Back"));
    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                              Default

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/******************* Standard functions ************************************/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                      Setting up of standard functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 NullNoStandard

 On entry   : pOut
            : pInOut
            : nIn
 On exit    : FALSE
 */
static RwBool
NullNoStandard(void *pOut, void *pInOut, RwInt32 nIn)
{
    RWFUNCTION(RWSTRING("NullNoStandard"));

    /* Stop warnings */
    pOut = pOut;
    pInOut = pInOut;
    nIn = nIn;

    RWRETURN(FALSE);
}

/****************************************************************************
 NullSetStandards

 On entry   : Standard functions
            : Space
            : Standards
            : Amount of standards
 On exit    :
 */
static void
NullSetStandards(RwStandardFunc * fppFuncs,
                 RwInt32 nNumFuncs,
                 RwStandard * spStandards, RwInt32 nNumStandards)
{
    RwInt32             nI;

    RWFUNCTION(RWSTRING("NullSetStandards"));

    /* Clear out all of the standards initially */
    for (nI = 0; nI < nNumFuncs; nI++)
    {
        fppFuncs[nI] = NullNoStandard;
    }

    /* Fill in all of the standards */

    while (nNumStandards--)
    {
        if ((spStandards->nStandard < nNumFuncs) &&
            (spStandards->nStandard >= 0))
        {
            fppFuncs[spStandards->nStandard] = spStandards->fpStandard;
        }

        spStandards++;
    }

    RWRETURNVOID();
}

static RwBool
devicesystemrxpipelinerequestpipe(void *pOut,
                                  void *pInOut __RWUNUSED__,
                                  RwInt32 nIn __RWUNUSED__ )
{
    RxPipeline        **pipelineRef = (RxPipeline **) pOut;

    RWFUNCTION(RWSTRING("devicesystemrxpipelinerequestpipe"));

    *pipelineRef = NULL;

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           System access

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwDeviceRegisterPlugin

 On entry   :
 On exit    : TRUE on success
 */
RwBool
_rwDeviceRegisterPlugin(void)
{
    RWFUNCTION(RWSTRING("_rwDeviceRegisterPlugin"));

    /*
     * Gives the device a change to register it's plugns.
     */

    RWRETURN(TRUE);
}

/****************************************************************************
 NullSystem

 On entry   : Request
            : Data out
            : Data in/out
            : Number in
 On exit    : TRUE on success
 */
static RwBool
NullSystem(RwInt32 nRequest, void *pOut, void *pInOut, RwInt32 nIn)
{
    RWFUNCTION(RWSTRING("NullSystem"));

    switch (nRequest)
    {

        /******************* Opening/Closing **********************/

            /* Gives the device access to the libraries device block */

        case rwDEVICESYSTEMREGISTER:
            {
                RwDevice           *dpDevice = (RwDevice *) pOut;
                RwMemoryFunctions  *mfpFuncs =
                    (RwMemoryFunctions *) pInOut;
                RwDevice           *lpdGD3d = _rwDeviceGetHandle();

                /* Set up the fast device access.. */

                *dpDevice = *lpdGD3d;

                dgGGlobals.memFuncs = mfpFuncs;

                RWRETURN(TRUE);
            }

        /******************* Opening/Closing **********************/

            /* The purpose of Open is to put the hardware in a state where
             * it can be queried (about its capabilities). This function also
             * tests if it is a candidate at all (like it can be started at
             * all). NOTE this includes setting up a minimal set of standard
             * handlers that can be used for querying the device. */

        case rwDEVICESYSTEMOPEN:
            {
                /* We're not interested in the window handle here, but if we
                 * were, it would be accessible as:
                 * (HWND)(((RwEngineOpenParams *)pInOut)->displayID);
                 */

                /* Done */
                RWRETURN(TRUE);
            }

            /* The purpose of Close is to remove any set up that was performed
             * by Open */

        case rwDEVICESYSTEMCLOSE:
            {
                /* All done - nothing to do */
                RWRETURN(TRUE);
            }

        /************** Starting stopping ******************/

            /* Start means that this bit of hardware has been selected for
             * rendering - so get it into a state ready for rendering */

        case rwDEVICESYSTEMSTART:
            {
                /* All done */
                RWRETURN(TRUE);
            }

            /* Puts back the hardware into the state it was in immediately
             * after being opened */

        case rwDEVICESYSTEMSTOP:
            {

                /* All done */
                RWRETURN(TRUE);
            }

            /************* pipeline initialisation **************************/

        case rwDEVICESYSTEMINITPIPELINE:
            {
                RWRETURN(TRUE);
            }

        case rwDEVICESYSTEMRXPIPELINEREQUESTPIPE:
            {
                RwBool              result;

                result =
                    devicesystemrxpipelinerequestpipe(pOut, pInOut,
                                                      nIn);
                RWRETURN(result);
            }

        /************* standard device functions ************************/

        case rwDEVICESYSTEMSTANDARDS:
            {
                /* Standard functions */
                static RwStandard   saGStandards[] = {
                    /* Camera ops */
                    {rwSTANDARDCAMERABEGINUPDATE,
                     NullCameraBeginUpdate},
                    {rwSTANDARDCAMERAENDUPDATE, NullCameraEndUpdate},
                    {rwSTANDARDCAMERACLEAR, NullCameraClear},

                    /* Raster/Pixel operations */
                    {rwSTANDARDRASTERSHOWRASTER, NullRasterShowRaster},
                    {rwSTANDARDRGBTOPIXEL, NullRGBToPixel},
                    {rwSTANDARDPIXELTORGB, NullPixelToRGB},
                    {rwSTANDARDRASTERSETIMAGE, NullRasterSetImage},
                    {rwSTANDARDIMAGEGETRASTER, NullImageGetRaster},

                    /* Raster creation and destruction */
                    {rwSTANDARDRASTERDESTROY, NullRasterDestroy},
                    {rwSTANDARDRASTERCREATE, NullRasterCreate},
                    {rwSTANDARDRASTERSUBRASTER, NullRasterSubRaster},

                    /* Finding about a raster type */
                    {rwSTANDARDIMAGEFINDRASTERFORMAT,
                     NullImageFindRasterFormat},

                    /* Texture operations */
                    {rwSTANDARDTEXTURESETRASTER, NullTextureSetRaster},

                    /* Locking and releasing */
                    {rwSTANDARDRASTERLOCK, NullRasterLock},
                    {rwSTANDARDRASTERUNLOCK, NullRasterUnlock},
                    {rwSTANDARDRASTERLOCKPALETTE,
                     NullRasterLockPalette},
                    {rwSTANDARDRASTERUNLOCKPALETTE,
                     NullRasterUnlockPalette},

                    /* Raster operations */
                    {rwSTANDARDRASTERCLEAR, NullRasterClear},
                    {rwSTANDARDRASTERCLEARRECT, NullRasterClearRect},
                    {rwSTANDARDRASTERRENDER, NullRasterRender},
                    {rwSTANDARDRASTERRENDERSCALED,
                     NullRasterRenderScaled},
                    {rwSTANDARDRASTERRENDERFAST, NullRasterRenderFast},
                    {rwSTANDARDSETRASTERCONTEXT, NullRasterSetContext},

                    /* Render order hint */
                    {rwSTANDARDHINTRENDERF2B, NullHintRenderFront2Back},

                    /* Raster mip levels */
                    {rwSTANDARDRASTERGETMIPLEVELS,
                     NullRasterGetMipLevels}
                };

                NullSetStandards((RwStandardFunc *) pOut, nIn,
                                 saGStandards,
                                 sizeof(saGStandards) /
                                 sizeof(RwStandard));

                RWRETURN(TRUE);
            }
        case rwDEVICESYSTEMGETID:
            {
                *(RwUInt16*)pOut = rwDEVICE_NULL;
                RWRETURN(TRUE);
            }


        /******************* not supported ******************************/

        default:
            {
                break;
            }
    }

    RWRETURN(FALSE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                     Getting the device structure

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwDeviceGetHandle

 On entry   :
 On exit    : device block handle
 */
RwDevice           *
_rwDeviceGetHandle(void)
{
    static RwDevice     dGNullDevice = {
        (RwReal) (1),           /* Default gamma correction */

        NullSystem,

        (RwReal) (65535.0), (RwReal) (0.0), /* Z buffer limits - what does it matter? */

        NullSetRenderState,     /* Setting the rendering state */
        NullGetRenderState,     /* Getting the rendering state */

        NullIm2DRenderLine,     /* Line rendering function */
        NullIm2DRenderTriangle, /* Rendering a triangle under the current state */
        NullIm2DRenderPrimitive,
        NullIm2DRenderIndexedPrimitive,

        /* These get set up when the immediate mode module is set up */
        NULL, NULL, NULL, NULL
    };                          /* NULL system */

    RWFUNCTION(RWSTRING("_rwDeviceGetHandle"));

    RWRETURN(&dGNullDevice);
}
