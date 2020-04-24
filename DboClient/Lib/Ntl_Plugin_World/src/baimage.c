/*
 *
 * Image handling.
 * Images are the device independent representation of rasters.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */


/**
 * \ingroup rwimage
 * \page rwimageoverview RwImage Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection imageoverview Overview
 *
 * This object represents a platform-neutral bitmap.
 *
 * Images are intended for loading and processing of bitmap data.
 * They cannot be rendered directly by RenderWare Graphics and must be converted
 * into raster ( \ref RwRaster) objects prior to rendering.

 * The image object also supports file handling extensions that are completely
 * separate from the RenderWare Binary Stream (see \ref RwStream) system. This allows it to read
 * and write known bitmap file formats, such as .BMP, .RAS and .PNG. By default,
 * no such support is provided. You must link and register the appropriate Toolkits
 * to support these extensions, or write your own.
 *
*/


/*****************************************************************************************
 * NOTE: NOTHING IN THIS FILE SHOULD DEPEND ON RWRASTER - THAT'S WHAT BAIMRAS.C/H IS FOR *
 *****************************************************************************************/

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"

#include "bastream.h"
#include "babinary.h"

#include "bamemory.h"
#include "osintf.h"

/* String abstraction API for unicode support */
#include "rwstring.h"

/* Math abstraction API */
#include "bamath.h"

/* Image handling */
#include "baimage.h"

/****************************************************************************
 Local Types
 */

RwBool RwFexist( const RwChar * name );

typedef RwChar     *rwPathCallBack(RwChar * pathname, void *data);

typedef struct _imageReadData imageReadData;
struct _imageReadData
{
    RwImageCallBackRead readImage;
    RwImage            *image;
};

typedef struct __rwImage _rwImage;
struct __rwImage
{
    RwInt32             width;  /* Device may have different ideas */
    RwInt32             height; /* internally !! */

    RwInt32             depth;  /* Of referenced image */
    RwInt32             stride; /* Stride of image */
};

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define rwSEARCHPATH_SEPARATOR ';'
#define rwDEFAULTGAMMA (RwReal)((1))

#define RWIMAGEGLOBAL(var)                              \
   (RWPLUGINOFFSET(rwImageGlobals,                      \
                   RwEngineInstance,                    \
                   imageModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static  RwPluginRegistry   imageTKList =
{
    sizeof(RwImage),
    sizeof(RwImage),
    0,
    0,
    (RwPluginRegEntry *)NULL,
    (RwPluginRegEntry *)NULL
};

static RwModuleInfo imageModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Image handling

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static RwInt32 _rwImageFreeListBlockSize = 128,
               _rwImageFreeListPreallocBlocks = 1;
static RwFreeList _rwImageFreeList;

/**
 * \ingroup rwimage
 * \ref RwImageSetFreeListCreateParams allows the developer to specify
 * how many \ref RwImage s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwImageSetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc)
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RwImageSetFreeListCreateParams"));
#endif
    _rwImageFreeListBlockSize = blockSize;
    _rwImageFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

static RwInt32 _rwImageFormatFreeListBlockSize = 3,
               _rwImageFormatFreeListPreallocBlocks = 1;
static RwFreeList _rwImageFormatFreeList;

/**
 * \ingroup rwimage
 * \ref RwImageFormatSetFreeListCreateParams allows the developer to specify
 * how many \ref RwImage s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwImageFormatSetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc)
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RwImageFormatSetFreeListCreateParams"));
#endif
    _rwImageFormatFreeListBlockSize = blockSize;
    _rwImageFormatFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

/****************************************************************************
 _rwImageOpen

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwImageOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwImageOpen"));

    /* Store global data offset (same for all instances) */
    imageModule.globalsOffset = offset;

    RWIMAGEGLOBAL(imageFreeList) =
        RwFreeListCreateAndPreallocateSpace(imageTKList.sizeOfStruct,
        _rwImageFreeListBlockSize, sizeof(RwInt32),
        _rwImageFreeListPreallocBlocks, &_rwImageFreeList,
        rwMEMHINTDUR_GLOBAL | rwID_IMAGE);
    if (!RWIMAGEGLOBAL(imageFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    RWIMAGEGLOBAL(imageFormatFreeList) =
        RwFreeListCreateAndPreallocateSpace(sizeof(rwImageFormat), _rwImageFreeListBlockSize, sizeof(RwUInt32),
        _rwImageFormatFreeListPreallocBlocks, &_rwImageFormatFreeList, rwMEMHINTDUR_GLOBAL | rwID_IMAGEMODULE);
    if (!RWIMAGEGLOBAL(imageFormatFreeList))
    {
        /* Failure */
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFreeList));
        RWIMAGEGLOBAL(imageFreeList) = (RwFreeList *)NULL;
        RWRETURN(NULL);
    }

    /* Set up a default imagePath buffer */
    RWIMAGEGLOBAL(imagePathSize) = 256;
    RWIMAGEGLOBAL(imagePath) = (RwChar *)
        RwMalloc(sizeof(RwChar) * RWIMAGEGLOBAL(imagePathSize),
        rwMEMHINTDUR_GLOBAL | rwID_IMAGEMODULE | rwMEMHINTFLAG_RESIZABLE);
    if (!RWIMAGEGLOBAL(imagePath))
    {
        /* Failure */
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFormatFreeList));
        RWIMAGEGLOBAL(imageFormatFreeList) = (RwFreeList *)NULL;
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFreeList));
        RWIMAGEGLOBAL(imageFreeList) = (RwFreeList *)NULL;
        RWRETURN(NULL);
    }
    RWIMAGEGLOBAL(imagePath)[0] = '\0';

    /* One more module instance */
    imageModule.numInstances++;

    /* Set up the gamma correction */
    RwImageSetGamma(rwDEFAULTGAMMA);

    /* No image formats to start with */
    RWIMAGEGLOBAL(imageFormats) = (rwImageFormat *)NULL;

    RWIMAGEGLOBAL(scratchMemSize) = 256;
    RWIMAGEGLOBAL(scratchMem) = (RwUInt8 *)
        RwMalloc(sizeof(RwUInt8) * RWIMAGEGLOBAL(scratchMemSize),
             rwMEMHINTDUR_GLOBAL | rwID_IMAGE | rwMEMHINTFLAG_RESIZABLE);
    if (!RWIMAGEGLOBAL(scratchMem))
    {
        /* Failure */
        RwFree(RWIMAGEGLOBAL(imagePath));
        RWIMAGEGLOBAL(imagePath) = (char *)NULL;
        RWIMAGEGLOBAL(imagePathSize) = 0;
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFormatFreeList));
        RWIMAGEGLOBAL(imageFormatFreeList) = (RwFreeList *)NULL;
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFreeList));
        RWIMAGEGLOBAL(imageFreeList) = (RwFreeList *)NULL;
        RWRETURN(NULL);
    }

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rwImageClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwImageClose(void *instance,
              RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwImageClose"));

    if (RWIMAGEGLOBAL(scratchMem))
    {
        RwFree(RWIMAGEGLOBAL(scratchMem));
        RWIMAGEGLOBAL(scratchMem) = (unsigned char *)NULL;
        RWIMAGEGLOBAL(scratchMemSize) = 0;
    }

    if (RWIMAGEGLOBAL(imagePath))
    {
        RwFree(RWIMAGEGLOBAL(imagePath));
        RWIMAGEGLOBAL(imagePath) = (char *)NULL;
        RWIMAGEGLOBAL(imagePathSize) = 0;
    }

    /* Destroy registered formats */
    while (RWIMAGEGLOBAL(imageFormats))
    {
        rwImageFormat      *formatToDestroy =
            RWIMAGEGLOBAL(imageFormats);

        /* Remove from the list */
        RWIMAGEGLOBAL(imageFormats) = formatToDestroy->nextFormat;

        /* Free it up */
        RwFreeListFree(RWIMAGEGLOBAL(imageFormatFreeList),
                       formatToDestroy);
    }

    if (RWIMAGEGLOBAL(imageFormatFreeList))
    {
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFormatFreeList));
        RWIMAGEGLOBAL(imageFormatFreeList) = (RwFreeList *)NULL;
    }

    if (RWIMAGEGLOBAL(imageFreeList))
    {
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFreeList));
        RWIMAGEGLOBAL(imageFreeList) = (RwFreeList *)NULL;
    }

    /* One less module instance */
    imageModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rwImageResizeScratchMem

 On entry   : size that buffer needs to be
 On exit    : pointer to buffer on success
 */

static RwUInt8     *
ImageGetScratchMem(RwInt32 size)
{
    RWFUNCTION(RWSTRING("ImageGetScratchMem"));
    RWASSERT(size > 0);

    /* Need to resize */
    if (size > RWIMAGEGLOBAL(scratchMemSize))
    {
        RwUInt8            *newBuf;

        if (RWIMAGEGLOBAL(scratchMem))
        {
            newBuf =
                (RwUInt8 *) RwRealloc(RWIMAGEGLOBAL(scratchMem),
                size,
                rwMEMHINTDUR_GLOBAL | rwID_IMAGE | rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            newBuf = (RwUInt8 *) RwMalloc(size,
                rwMEMHINTDUR_GLOBAL | rwID_IMAGE | rwMEMHINTFLAG_RESIZABLE);
        }

        if (!newBuf)
        {
            RWERROR((E_RW_NOMEM, size));
            RWRETURN((unsigned char *)NULL);
        }

        RWIMAGEGLOBAL(scratchMem) = newBuf;
        RWIMAGEGLOBAL(scratchMemSize) = size;
    }

    RWRETURN(RWIMAGEGLOBAL(scratchMem));
}

/****************************************************************************
 _rwImageGammaUnCorrectArrayOfRGBA

 Processes array of RwRGBA with given LUT (input can be same as output)

 On entry   : Out array pointer
            : In array pointer
            : LUT
            : Number elements
 On exit    :
 */
void
_rwImageGammaUnCorrectArrayOfRGBA(RwRGBA * rgbaOut,
                                  RwRGBA * rgbaIn, RwInt32 numEls)
{
    RwUInt8            *gammaTab = RWIMAGEGLOBAL(invGammaTable);

    RWFUNCTION(RWSTRING("_rwImageGammaUnCorrectArrayOfRGBA"));
    RWASSERT(rgbaOut);
    RWASSERT(rgbaIn);

    while (numEls--)
    {
        rgbaOut->red = gammaTab[rgbaIn->red];
        rgbaOut->green = gammaTab[rgbaIn->green];
        rgbaOut->blue = gammaTab[rgbaIn->blue];
        rgbaOut->alpha = rgbaIn->alpha;
        rgbaOut++;
        rgbaIn++;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwImageGammaCorrectArrayOfRGBA

 Processes array of RwRGBA with given LUT (input can be same as output)

 On entry   : Out array pointer
            : In array pointer
            : LUT
            : Number elements
 On exit    :
 */
void
_rwImageGammaCorrectArrayOfRGBA(RwRGBA * rgbaOut,
                                RwRGBA * rgbaIn, RwInt32 numEls)
{
    RwUInt8            *gammaTab = RWIMAGEGLOBAL(gammaTable);

    RWFUNCTION(RWSTRING("_rwImageGammaCorrectArrayOfRGBA"));
    RWASSERT(rgbaOut);
    RWASSERT(rgbaIn);

    while (numEls--)
    {
        rgbaOut->red = gammaTab[rgbaIn->red];
        rgbaOut->green = gammaTab[rgbaIn->green];
        rgbaOut->blue = gammaTab[rgbaIn->blue];
        rgbaOut->alpha = rgbaIn->alpha;
        rgbaOut++;
        rgbaIn++;
    }

    RWRETURNVOID();
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Creating and destroying image

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwimage
 * \ref RwImageCreate creates an image with the specified size and depth.
 * Only images with depths 4, 8 and 32-bits per pixel are currently supported.
 * For 4- and 8-bit images, pixel values are indices into a color look-up
 * table or palette.
 *
 * For 32-bit images, pixel values are true colors in
 * red-green-blue-alpha format. Note that this function does not create space
 * for the pixels or the palette, which can be allocated separately using
 * \ref RwImageAllocatePixels. However, API functions exist which can attach
 * pre-existing pixel and palette data to the image, if required. If
 * \ref RwImageAllocatePixels has been used, then the image stride is
 * automatically calculated, otherwise it must be explicitly determined and
 * set.
 *
 * \param width  An \ref RwInt32 value equal to the width of the image
 * \param height  An \ref RwInt32 value equal to the height of the image
 * \param depth  An \ref RwInt32 value equal to the number of bits per pixel.
 *
 * \return Returns a pointer to the newly created image if successful or
 * NULL if there is an error.
 *
 * \see RwImageAllocatePixels
 * \see RwImageDestroy
 * \see RwImageSetPalette
 * \see RwImageSetPixels
 * \see RwImageSetStride
 *
 */
RwImage            *
RwImageCreate(RwInt32 width, RwInt32 height, RwInt32 depth)
{
    RwImage            *image;

    RWAPIFUNCTION(RWSTRING("RwImageCreate"));
    RWASSERT(imageModule.numInstances);
    RWASSERT((depth == 4) || (depth == 8) || (depth == 32));

    RWASSERT(RWIMAGEGLOBAL(imageFreeList));
    image = (RwImage *) RwFreeListAlloc(RWIMAGEGLOBAL(imageFreeList),
                                        rwMEMHINTDUR_EVENT | rwID_IMAGE);
    if (!image)
    {
        RWRETURN((RwImage *)NULL);
    }

    image->width = width;
    image->height = height;
    image->depth = depth;

    image->cpPixels = (unsigned char *)NULL;
    image->palette = (RwRGBA *)NULL;

    image->flags = 0;          /* Can be frigged with */

    /* Initialize the plugin memory */
    _rwPluginRegistryInitObject(&imageTKList, image);

    RWRETURN(image);
}

/**
 * \ingroup rwimage
 * \ref RwImageDestroy destroys the specified image. If the pixel and
 * palette data arrays have been allocated using \ref RwImageAllocatePixels
 * this function frees the space allocated for them too.
 *
 * \param image  Pointer to the image to destroy.
 *
 * \return Returns TRUE.
 *
 * \see RwImageCreate
 *
 */
RwBool
RwImageDestroy(RwImage * image)
{
    RWAPIFUNCTION(RWSTRING("RwImageDestroy"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    if (image->flags & rwIMAGEALLOCATED)
    {
        /* Note this points to both the pixels and the palette */
        RwImageFreePixels(image);
    }

    /* De-Initialize the plugin memory */
    _rwPluginRegistryDeInitObject(&imageTKList, image);

    RWASSERT(RWIMAGEGLOBAL(imageFreeList));
    RwFreeListFree(RWIMAGEGLOBAL(imageFreeList), image);

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Setting and getting fields

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwimage
 * \ref RwImageSetStride is used to specify the stride for the given
 * image. The stride is the number of bytes between the beginning of one
 * scan-line of the image and the beginning of the next, taking account of
 * padding so that each new scan-line falls on an 8-bit boundary. For 8- and
 * 32-bit images, the stride is simply the image width multiplied by the
 * number of bytes per pixel, 1 and 4 respectively.
 *
 * The stride of a 4-bit image is the same as an 8-bit image because multiple
 * pixels are not packed into a single byte.
 *
 * This function should be used if pixels are not set using
 * \ref RwImageAllocatePixels which automatically determines and sets the
 * stride.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param image  A pointer to the image to set stride for
 * \param stride  An \ref RwInt32 value equal to the image stride.
 *
 * \return Returns a pointer to the specified image.
 *
 * \see RwImageGetStride
 * \see RwImageAllocatePixels
 * \see RwImageSetPixels
 * \see RwImageCreate
 *
 */
RwImage *
RwImageSetStride(RwImage *image, RwInt32 stride)
{
    RWAPIFUNCTION(RWSTRING("RwImageSetStride"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageSetStrideMacro(image, stride));
}

/**
 * \ingroup rwimage
 * \ref RwImageSetPixels is used to attach a pre-existing pixel data
 * array to the specified image. The pixel data pointer references an array
 * of allocated bytes large enough to accommodate all of the image data
 * (conforming with the image's width, height, depth and stride). This
 * function is typically used following a call to \ref RwImageCreate to set
 * the pixels of an image when the image data already exists in the required
 * form. The image stride should also be explicitly calculated and set at the
 * same time.
 *
 * If the image depth is 4- or 8-bits, then a palette is also
 * required, which can be attached to the image using \ref RwImageSetPalette.
 *
 * \note Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \note Note that calling \ref RwImageDestroy on the specified image will not destroy
 * the pixel data registered using \ref RwImageSetPixels.  It is the application's
 * responsibility to do this, if required.
 *
 * \param image  A pointer to the image whose pixel data is needed
 * \param pixels  A pointer to the data array specifying the image's pixel values.
 *
 * \return Returns pointer to the specified image.
 *
 * \see RwImageAllocatePixels
 * \see RwImageCreate
 * \see RwImageGetPixels
 * \see RwImageSetPalette
 * \see RwImageSetStride
 *
 */
RwImage *
RwImageSetPixels(RwImage *image, RwUInt8 *pixels)
{
    RWAPIFUNCTION(RWSTRING("RwImageSetPixels"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageSetPixelsMacro(image, pixels));
}

/**
 * \ingroup rwimage
 * \ref RwImageSetPalette is used to attach a pre-existing palette data
 * array to the specified image. The palette data pointer references an array
 * of allocated bytes large enough to accommodate all of the palette data
 * (conforming with the image's depth). This function is typically used
 * following a call to \ref RwImageCreate to set the palette of an image when
 * the palette data already exists in the required form.
 *
 * This function should only be used for images which
 * have image depth of 4- or 8-bits and have had their pixels allocated using
 * \ref RwImageSetPixels.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param image  A pointer to the image whose palette is to be set
 * \param palette  A pointer to the data array specifying the image's new palette.
 *
 * \return Returns pointer to the image.
 *
 * \see RwImageCreate
 * \see RwImageGetPalette
 * \see RwImageSetPixels
 *
 */
RwImage *
RwImageSetPalette(RwImage *image, RwRGBA *palette)
{
    RWAPIFUNCTION(RWSTRING("RwImageSetPalette"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageSetPaletteMacro(image, palette));
}

/**
 * \ingroup rwimage
 * \ref RwImageGetWidth is used to determine the width of the specified
 * image in pixels.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param image  A pointer to the image whose width is required.
 *
 * \return Returns an \ref RwInt32 value equal to the image's width.
 *
 * \see RwImageGetDepth
 * \see RwImageGetHeight
 * \see RwImageGetPalette
 * \see RwImageGetPixels
 * \see RwImageGetStride
 *
 */
RwInt32
RwImageGetWidth(const RwImage *image)
{
    RWAPIFUNCTION(RWSTRING("RwImageGetWidth"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageGetWidthMacro(image));
}

/**
 * \ingroup rwimage
 * \ref RwImageGetHeight is used to determine the height of the specified
 * image in pixels.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param image  A pointer to the image whose height is required.
 *
 * \return Returns an \ref RwInt32 value equal to the image's height.
 *
 * \see RwImageGetDepth
 * \see RwImageGetWidth
 * \see RwImageGetPalette
 * \see RwImageGetPixels
 * \see RwImageGetStride
 *
 */
RwInt32
RwImageGetHeight(const RwImage *image)
{
    RWAPIFUNCTION(RWSTRING("RwImageGetHeight"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageGetHeightMacro(image));
}

/**
 * \ingroup rwimage
 * \ref RwImageGetDepth is used to determine the number of bits per
 * pixel of the specified image. Currently, only images with 8- and 32-bits
 * per pixels are supported.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param image  A pointer to the image whose depth is required.
 *
 * \return Returns an \ref RwInt32 value equal to the image's depth in bits per
 * pixel.
 *
 * \see RwImageGetHeight
 * \see RwImageGetWidth
 * \see RwImageGetPalette
 * \see RwImageGetPixels
 * \see RwImageGetStride
 * \see RwImageGetWidth
 *
 */
RwInt32
RwImageGetDepth(const RwImage *image)
{
    RWAPIFUNCTION(RWSTRING("RwImageGetDepth"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageGetDepthMacro(image));
}

/**
 * \ingroup rwimage
 * \ref RwImageGetStride is used to determine the stride of the specified
 * image. The stride is the number of bytes between the beginning of one
 * scan-line of the image and the beginning of the next, taking account of
 * padding such that each new scan-line falls on a 8-bit boundary.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param image  A pointer to the image whose stride is required.
 *
 * \return Returns an \ref RwInt32 value equal to the image's stride.
 *
 * \see RwImageGetWidth
 * \see RwImageGetHeight
 * \see RwImageGetDepth
 * \see RwImageGetPixels
 * \see RwImageGetPalette
 *
 */
RwInt32
RwImageGetStride(const RwImage *image)
{
    RWAPIFUNCTION(RWSTRING("RwImageGetStride"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageGetStrideMacro(image));
}

/**
 * \ingroup rwimage
 * \ref RwImageGetPixels is used to retrieve the specified image's pixel
 * data.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param image  A pointer to the image whose pixels are required.
 *
 * \return Returns a pointer to the image's pixel data array if successful
 * or NULL if there is an error or if the image does not have any pixel
 * data allocated.
 *
 * \see RwImageGetDepth
 * \see RwImageGetHeight
 * \see RwImageGetPalette
 * \see RwImageGetStride
 * \see RwImageGetWidth
 *
 */
RwUInt8 *
RwImageGetPixels(const RwImage *image)
{
    RWAPIFUNCTION(RWSTRING("RwImageGetPixels"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageGetPixelsMacro(image));
}

/**
 * \ingroup rwimage
 * \ref RwImageGetPalette is used to retrieve the specified image's
 * palette data. This function is only relevant for images with
 * depth of 4- or 8-bits.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param image  A pointer to the image whose palette is required.
 *
 * \return Returns a pointer to the image's palette data array.
 *
 * \see RwImageGetDepth
 * \see RwImageGetHeight
 * \see RwImageGetPixels
 * \see RwImageGetStride
 * \see RwImageGetWidth
 *
 */
RwRGBA *
RwImageGetPalette(const RwImage *image)
{
    RWAPIFUNCTION(RWSTRING("RwImageGetPalette"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    RWRETURN(RwImageGetPaletteMacro(image));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Allocating and freeing memory

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwimage
 * \ref RwImageAllocatePixels is used to allocate memory for pixels for
 * the specified image. This function can be used following a call to
 * \ref RwImageCreate to allocate memory for a data array which will hold the
 * pixel values of the image. If the image depth is 4- or 8-bits, then
 * memory is also allocated to hold the data for the image's palette.
 *
 * Note that calling \ref RwImageDestroy on the specified image will also free
 * the memory allocated using this function.  The pixel memory can also be
 * freed using \ref RwImageFreePixels, without destroying the image structure.
 *
 * \param image  Pointer to the image to allocate pixels for.
 *
 * \return Returns pointer to the image if successful or NULL if there is
 * an error.
 *
 * \see RwImageFreePixels
 * \see RwImageSetPixels
 * \see RwImageSetPalette
 *
 */
RwImage            *
RwImageAllocatePixels(RwImage * image)
{
    RwUInt32            imageDepth;
    RwBool              imagePalette;
    RwUInt32            paletteSize;
    RwUInt32            pixelsSize;
    RwUInt32            totalSize;

    RWAPIFUNCTION(RWSTRING("RwImageAllocatePixels"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);
    RWASSERT(!(image->flags & rwIMAGEALLOCATED) && !image->cpPixels);

    imageDepth = image->depth;
    imagePalette = ((4 == imageDepth) || (8 == imageDepth));
    paletteSize =
        imagePalette ? ((1UL << imageDepth) * sizeof(RwRGBA)) : 0;

    /* Bytes per pixel */
    image->stride = (image->depth + 7) >> 3;
    image->stride *= image->width;
    image->stride = (image->stride + 3) & (~3);

    /* Space required for the image */
    pixelsSize = image->stride * image->height;

    totalSize = pixelsSize + paletteSize;

    /* Allocate it */
    image->cpPixels = (RwUInt8 *) RwMalloc(totalSize,
        rwMEMHINTDUR_EVENT | rwID_IMAGE);

    /* Check the image pointer */
    if (!image->cpPixels)
    {
        RWERROR((E_RW_NOMEM, (totalSize)));
        RWRETURN((RwImage *)NULL);
    }

    /* Set the palette pointer */
    image->palette =
        (imagePalette) ? ((RwRGBA *) (image->cpPixels + pixelsSize)) :
        (RwRGBA *)NULL;

    /* Mark as owned */
    image->flags |= rwIMAGEALLOCATED;

    /* Done */
    RWRETURN(image);
}

/**
 * \ingroup rwimage
 * \ref RwImageFreePixels is used to free memory previously allocated
 * by \ref RwImageAllocatePixels. This includes the pixel data and also the
 * palette data array, where appropriate. It is an error to use this function
 * if the pixel and palette data has been initialized using pre-existing
 * data arrays via the functions \ref RwImageSetPixels and
 * \ref RwImageSetPalette.
 *
 * \param image  A pointer to the image whose pixel and palette data are to be destroyed.
 *
 * \return Returns a pointer to the image.
 *
 * \see RwImageAllocatePixels
 * \see RwImageSetPixels
 * \see RwImageSetPalette.
 *
 */
RwImage            *
RwImageFreePixels(RwImage * image)
{
    RWAPIFUNCTION(RWSTRING("RwImageFreePixels"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);
    RWASSERT((image->flags & rwIMAGEALLOCATED) && (image->cpPixels));

    RwFree(image->cpPixels);
    image->cpPixels = (unsigned char *)NULL;
    image->palette = (RwRGBA *)NULL;

    image->flags &= ~rwIMAGEALLOCATED;

    RWRETURN(image);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Resizing images

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

   /**
 * \ingroup rwimage
 * \ref RwImageResize resizes the specified source image to a
 * different size. If the image already has pixels allocated and the
 * resized image is smaller than the original then pixel data will be
 * clipped out as necessary. If pixels have already been allocated to the
 * image and the resized image is larger than the original then the new pixels
 * will return garbage. For example, if an image 275x120 is resized to
 * 256x256 the image size is changed to 256x120 and the extra pixels are
 * filled with garbage.
 *
 * \param image  A pointer to the image to be resized
 * \param width  A resized width of the image
 * \param height  A resized height of the image.
 *
 * \return Returns a pointer to the resized image if successful or NULL
 * if there is an error.
 *
 * \see RwImageResample
 *
 */
RwImage            *
RwImageResize(RwImage * image, RwInt32 width, RwInt32 height)
{
    RWAPIFUNCTION(RWSTRING("RwImageResize"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    /* do we have pixels to process? */
    if ((image->flags & rwIMAGEALLOCATED) && (image->cpPixels))
    {
        RwInt32             i;
        RwInt32             minWidth;
        RwInt32             minHeight;
        RwInt32             spanLength;
        RwUInt8            *cpNewPixels;
        RwRGBA             *newPalette;
        RwInt32             stride;
        const RwUInt8      *cpSrc;
        RwUInt8            *cpDst;
        RwUInt32            pixelsSize;
        RwUInt32            totalSize;
        const RwBool        imagePalette = ((4 == image->depth)

                                            || (8 == image->depth));
        const RwUInt32      paletteSize =
            imagePalette ? ((1UL << image->depth) * sizeof(RwRGBA)) : 0;

        /* Allocate pixel and palette data for the resized image */
        stride = (image->depth + 7) >> 3;
        stride *= width;
        stride = (stride + 3) & (~3);

        pixelsSize = stride * height;

        totalSize = pixelsSize + paletteSize;

        cpNewPixels = (RwUInt8 *) RwMalloc(totalSize,
            rwMEMHINTDUR_EVENT | rwID_IMAGE);

        if (!cpNewPixels)
        {
            RWERROR((E_RW_NOMEM, (totalSize)));
            RWRETURN((RwImage *)NULL);
        }

        newPalette = (imagePalette) ?
            ((RwRGBA *) (cpNewPixels + pixelsSize)) : (RwRGBA *)NULL;

        /* Copy the palette data */
        if (imagePalette)
        {
            memcpy(newPalette, image->palette, paletteSize);
        }

        /* Copy the pixel data */
        minWidth = (width < image->width) ? width : image->width;
        minHeight = (height < image->height) ? height : image->height;

        spanLength = ((image->depth + 7) >> 3) * minWidth;

        cpDst = cpNewPixels;
        cpSrc = image->cpPixels;

        for (i = 0; i < minHeight; i++)
        {
            memcpy(cpDst, cpSrc, spanLength);

            cpDst += stride;
            cpSrc += image->stride;
        }

        /* Free the original image data */
        RwFree(image->cpPixels);

        image->cpPixels = cpNewPixels;
        image->palette = newPalette;
        image->stride = stride;
    }

    image->width = width;
    image->height = height;

    RWRETURN(image);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Messing around with images

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwimage
 * \ref RwImageMakeMask generates an alpha channel for the specified
 * image based on it's pixel color values. The alpha channel is that part of
 * the image controlling transparency and its value for each pixel is
 * calculated here from the color channel values as follows:
 * max(red, green, blue).
 *
 * \param image  A pointer to the image whose alpha channel will be generated.
 *
 * \return Returns a pointer to the image.
 *
 * \see RwImageApplyMask
 * \see RwImageReadMaskedImage
 *
 */
RwImage            *
RwImageMakeMask(RwImage * image)
{
    RwInt32             i;

    RWAPIFUNCTION(RWSTRING("RwImageMakeMask"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    switch (image->depth)
    {
        case 4:
        case 8:
            {
                RwInt32             palSize = (1 << image->depth);
                RwRGBA             *rpPal = image->palette;

                /* 8 bit image */
                for (i = 0; i < palSize; i++)
                {
                    RwInt32             nOpacity;

                    /* Opacity is max colour channel value */
                    nOpacity = rpPal[i].red;
                    if (rpPal[i].green > nOpacity)
                    {
                        nOpacity = rpPal[i].green;
                    }
                    if (rpPal[i].blue > nOpacity)
                    {
                        nOpacity = rpPal[i].blue;
                    }

                    rpPal[i].alpha = (RwUInt8) nOpacity;
                }
                break;
            }
        case 32:
            {
                RwUInt8            *cpSpan;
                RwInt32             j;

                /* 32 bit image */
                cpSpan = image->cpPixels;

                for (i = 0; i < image->height; i++)
                {
                    RwRGBA             *rpCur = (RwRGBA *) cpSpan;

                    for (j = 0; j < image->width; j++)
                    {
                        RwInt32             nOpacity;

                        /* Opacity is max colour channel value */
                        nOpacity = rpCur[j].red;
                        if (rpCur[j].green > nOpacity)
                        {
                            nOpacity = rpCur[j].green;
                        }
                        if (rpCur[j].blue > nOpacity)
                        {
                            nOpacity = rpCur[j].blue;
                        }

                        rpCur[j].alpha = (RwUInt8) nOpacity;
                    }

                    cpSpan += image->stride;

                }
            }
    }

    RWRETURN(image);
}

/**
 * \ingroup rwimage
 * \ref RwImageApplyMask applies an image's alpha mask to another image.
 * Masking an image has the effect of copying the alpha channel (that part of
 * the image controlling transparency) from the mask image to the destination
 * image.
 *
 * Both image and mask must have the same dimensions.
 *
 * If the input image has 8 bits per pixel, the result of applying the mask
 * is to convert the image to 32 bits per pixel, which allocates its own pixel
 * array.  Therefore, if the pixel data was previously registered using
 * \ref RwImageSetPixels, the original pixel data is not freed and should be destroyed
 * by the application if necessary.  This also means that the pointer returned by
 * \ref RwImageGetPixels will have changed after applying the mask.
 *
 * \param image  Pointer to the image which will receive the mask
 * \param mask  Pointer to the image supplying the mask.
 *
 * \return Returns pointer to the image if successful or NULL if there is
 * an error.
 *
 * \see RwImageMakeMask
 * \see RwImageReadMaskedImage
 *
 */
RwImage            *
RwImageApplyMask(RwImage * image, const RwImage * mask)
{
    RWAPIFUNCTION(RWSTRING("RwImageApplyMask"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);
    RWASSERT(mask);
    RWASSERT(image->width == mask->width);
    RWASSERT(image->height == mask->height);

    /* release build check since bad memory accesses could be
     * made in the code below otherwise
     */
    if ( (image->width != mask->width) ||
         (image->height != mask->height) )
    {
        RWERROR( (E_RW_INVIMAGEMASK) );
        RWRETURN( (RwImage *)NULL );
    }

    switch (image->depth)
    {
        case 4:
        case 8:
            {
                RwImage            *tempImage;

                /* In order to apply a mask to a 4 or 8 bit image, and retain the
                 * true alpha component, we must explode the image to a 32 bit
                 * image first, then do the 32 bit image thing.
                 * We need to make sure that the image is at the same location
                 * though (effectively changing it's depth in place)
                 */

                /* Copy the 4/8 bit image, convert old image to 32 bit, copy
                 * image again
                 */

                tempImage =
                    RwImageCreate(image->width, image->height,
                                  image->depth);
                if (!tempImage)
                {
                    /* Failed to remap to 32 bit */
                    RWRETURN((RwImage *)NULL);
                }

                if (!RwImageAllocatePixels(tempImage))
                {
                    RwImageDestroy(tempImage);
                    RWRETURN((RwImage *)NULL);
                }

                /* Copy the image */
                RwImageCopy(tempImage, image);

                if (image->flags & rwIMAGEALLOCATED)
                {
                    /* Now free the pixels and palette */
                    RwImageFreePixels(image);
                }

                /* Now re-allocate as a 32 bit image (a bit hacky) */
                image->depth = 32;
                RwImageAllocatePixels(image);

                /* Now copy back (this expands bit depth) */
                RwImageCopy(image, tempImage);

                /* And destroy the temporary one */
                RwImageFreePixels(tempImage);
                RwImageDestroy(tempImage);

                /* We now have a 32 bit image to work with,
                 * so DROP THROUGH to the 32 bit code
                 */
            }
        case 32:
            {
                /* 32 bit destination */
                RwInt32             i, j;
                const RwUInt8      *cpSrc = mask->cpPixels;
                const RwRGBA       *rpPal = mask->palette;
                RwUInt8            *cpDst = image->cpPixels;

                for (i = 0; i < image->height; i++)
                {
                    RwRGBA             *dstRGB = (RwRGBA *) cpDst;

                    switch (mask->depth)
                    {
                        case (4):
                        case (8):
                            {
                                const RwUInt8      *srcInd = cpSrc;

                                for (j = 0; j < image->width; j++)
                                {
                                    dstRGB->alpha =
                                        rpPal[*srcInd].alpha;
                                    srcInd++;
                                    dstRGB++;
                                }
                                break;
                            }
                        case (32):
                            {
                                const RwRGBA       *srcRGB =
                                    (const RwRGBA *) cpSrc;

                                for (j = 0; j < image->width; j++)
                                {
                                    dstRGB->alpha = srcRGB->alpha;
                                    srcRGB++;
                                    dstRGB++;
                                }
                                break;
                            }
                        default:
                            {
                                /* Should never happen */
                                break;
                            }
                    }

                    cpDst += image->stride;
                    cpSrc += mask->stride;
                }
                break;
            }
        default:
            {
                RWERROR((E_RW_INVIMAGEFORMAT));
                RWRETURN((RwImage *)NULL);

            }
    }

    RWRETURN(image);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Helper functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwimage
 * \ref RwImageSetPath is used to specify the current search path for
 * reading images from the file system. Used in conjunction with image, raster
 * and texture reading API functions, the search path is defined as the
 * directory or directories in which the image files can be found. If more
 * than one directory is specified, these must be separated using a semi-colon.
 *
 * The search path can be considered to be either absolute or relative. In the
 * latter case the search path is relative to the directory from which the
 * application executable is running (the current directory). If no search
 * path is specified (empty string) only the current directory is used.
 *
 * Always include a trailing path separator in the directory name when setting
 * the search path.
 *
 * \param path  A pointer to a string describing the search path.
 *
 * \return Returns a pointer to the search path if successful or NULL if there
 * is an error.
 *
 * \see RwImageGetPath
 * \see RwImageRead
 * \see RwImageReadMaskedImage
 * \see RwRasterRead
 * \see RwRasterReadMaskedRaster
 * \see RwTextureRead
 *
 */
const RwChar       *
RwImageSetPath(const RwChar * path)
{
    RwInt32             newPathLength;

    RWAPIFUNCTION(RWSTRING("RwImageSetPath"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(path);

    /* How big is the string (in bytes).  Don't forget the NUL */
    newPathLength = (rwstrlen(path) + 1) * sizeof(RwChar);

    /* Is the buffer big enough */
    if (newPathLength > RWIMAGEGLOBAL(imagePathSize))
    {
        RwChar             *newPath;

        /* Need to realloc */
        newPath =  (RwChar *)
            RwRealloc(RWIMAGEGLOBAL(imagePath), newPathLength,
                      rwMEMHINTDUR_GLOBAL | rwID_IMAGEMODULE | rwMEMHINTFLAG_RESIZABLE);

        if (!newPath)
        {
            RWERROR((E_RW_NOMEM, newPathLength));
            RWRETURN((const char *)NULL);
        }

        RWIMAGEGLOBAL(imagePath) = newPath;
        RWIMAGEGLOBAL(imagePathSize) = newPathLength;
    }

    /* Copy the path (including NUL terminator) */
    memcpy(RWIMAGEGLOBAL(imagePath), path, newPathLength);

    RWRETURN(path);
}

/**
 * \ingroup rwimage
 * \ref RwImageGetPath  is used in conjunction with image, raster and texture
 * reading API functions, the search path is defined as the directory or directories
 * in which the image files can be found. If more than one directory is specified,
 * these must be separated using a semi-colon.
 *
 * The search path can be considered to be either absolute or relative.  In the
 * latter case the search path is relative to the directory from which the application
 * executable is running (the current directory). If no search path is specified
 * (empty string) only the current directory is used.
 *
 * \return Returns pointer to a string describing the search path if
 * successful or NULL if there is an error.
 *
 * \see RwImageSetPath
 * \see RwImageRead
 * \see RwImageRegisterImageFormat
 * \see RwRasterRead
 * \see RwRasterReadMaskedRaster
 * \see RwTextureRead
 *
 */
RwChar             *
RwImageGetPath(void)
{
    RWAPIFUNCTION(RWSTRING("RwImageGetPath"));
    RWASSERT(imageModule.numInstances);

    RWRETURN(RWIMAGEGLOBAL(imagePath));
}


/****************************************************************************
void
_rwImageSwapPath(RwChar *path, RwInt32 *size)

 On entry   : path is newPath, size is newSize
 On exit    : path is oldPath, size is oldSize
 */

void
_rwImageSwapPath(RwChar **path, RwInt32 *size)
{
	/* Cz */
	RwChar* path_temp;
	RwInt32 size_temp;

    RWFUNCTION(RWSTRING("_rwImageSwapPath"));


	path_temp = RWIMAGEGLOBAL(imagePath);
	size_temp = RWIMAGEGLOBAL(imagePathSize);

	RWIMAGEGLOBAL(imagePath)     = *path;
	RWIMAGEGLOBAL(imagePathSize) = *size;

	*path = path_temp;
	*size = size_temp;
	

    /*
	*path = RWIMAGEGLOBAL(imagePath);
    *size = RWIMAGEGLOBAL(imagePathSize);

    RWIMAGEGLOBAL(imagePath)     = *path;
    RWIMAGEGLOBAL(imagePathSize) = *size;
	*/
	
	
    /* Success */
    RWRETURNVOID();
}


/****************************************************************************
 ImagePathForAllFullNames

 On entry   : Filename to try on imagepath
            : extra bytes to add (for adding extensions, eg)
            : callback to call with each pathname
            : user data pointer
 On exit    : Filename with pre-pended image path if necessary
 */

static const RwChar *
ImagePathForAllFullNames(const RwChar * filename, RwInt32 extraBytes,
                         rwPathCallBack callBack, void *data)
{
    RwInt32             pathsize;
    RwChar             *fullname;
    RwChar             *pathElement;

    RWFUNCTION(RWSTRING("ImagePathForAllFullNames"));
    RWASSERT(filename);

    pathElement = RWIMAGEGLOBAL(imagePath);

    /* If we have an absolute path, ignore the search directory.
     * If we have no search path, use the current directory.
     * Either way, just copy the name.
     */
    if ((_rwpathisabsolute(filename)) ||
        (pathElement == NULL) || (pathElement[0] == '\0'))
    {
        /* Just need to copy the filename across */
        pathsize = (rwstrlen(filename) * sizeof(RwChar)) + extraBytes;

        /* Just call the callback with the name passed in */
        fullname = (RwChar *) ImageGetScratchMem(pathsize);
        if (!fullname)
        {
            /* Can't even allocate memory, oh dear, we are in trouble */
            RWRETURN((const char *)NULL);
        }

        /* Copy to a non const buffer */
        rwstrcpy(fullname, filename);

        /* Call the callback with it */
        callBack(fullname, data);
    }
    else
    {
        /* While we have a search path to try */
        while (pathElement && pathElement[0] != '\0')
        {
            RwChar             *nextPathElement =
                rwstrchr(pathElement, rwSEARCHPATH_SEPARATOR);
            RwInt32             pathElementLength;

            nextPathElement =
                rwstrchr(pathElement, rwSEARCHPATH_SEPARATOR);
            if (nextPathElement)
            {
                /* Length of pathstring (in bytes) */
                pathElementLength = (RwUInt8 *) nextPathElement -
                    (RwUInt8 *) pathElement;
                nextPathElement++; /* Skip the separator */
            }
            else
            {
                pathElementLength =
                    rwstrlen(pathElement) * sizeof(RwChar);
            }

            /* Just need to assemble a path with the current search path element */
            pathsize = pathElementLength + (rwstrlen(filename) *
                                            sizeof(RwChar)) +
                extraBytes;

            fullname = (RwChar *) ImageGetScratchMem(pathsize);
            if (!fullname)
            {
                /* Can't even allocate memory, oh dear, we are in trouble */
                RWRETURN((const char *)NULL);
            }

            memcpy(fullname, pathElement, pathElementLength);
            rwstrcpy((RwChar *)
                     (((RwUInt8 *) fullname) + pathElementLength),
                     filename);

            /* Call the callback with it */
            if (!callBack(fullname, data))
            {
                /* Early out */
                RWRETURN(filename);
            }

            pathElement = nextPathElement;
        }
    }

    /* All done */
    RWRETURN(filename);
}

/****************************************************************************
 ImageAttempRead

 On entry   : pathname to try and read with
            : pointer to read image data block
 On exit    : NULL if image read (to early out), or pathname if we need to try
            : some more...!
 */

static RwChar      *
ImageAttempRead(RwChar * pathname, void *data)
{
    imageReadData      *imageData = (imageReadData *) data;

    RWFUNCTION(RWSTRING("ImageAttempRead"));
    RWASSERT(pathname);
    RWASSERT(imageData);

    /* Test if it exists first - stops us spitting out an error if it doesn't */
    if (RwFexist(pathname))
    {
        imageData->image = imageData->readImage(pathname);
        if (imageData->image)
        {
            /* Early out, we got it */
            RWRETURN((char *)NULL);
        }
    }

    /* Else keep going */
    RWRETURN(pathname);
}

/**
 * \ingroup rwimage
 * \ref RwImageRead is used to read an image from a disk file. The image
 * file name can either be absolute or relative. In the latter case
 * \ref RwImageRead will use the current search path to determine in which
 * directory the image file can be found. The format modules which have been
 * registered using \ref RwImageRegisterImageFormat, determine the types of
 * files that are supported. Note that the filename must include
 * the file's extension, as this identifies the images type for dispatch to
 * the correct loader module.
 *
 * No gamma correction is applied to the image.
 *
 * \param imageName  A pointer to a string containing the file name of the image.
 *
 * \return Returns a pointer to the image if successful or NULL if there
 * is an error.
 *
 * \see RwImageWrite
 * \see RwImageRegisterImageFormat
 * \see RwImageSetPath
 * \see RwImageGammaCorrect
 * \see RwImageSetGamma
 * \see RwImageGetGamma
 * \see RwRasterRead
 * \see RwRasterReadMaskedRaster
 * \see RwTextureRead
 *
 */
RwImage            *
RwImageRead(const RwChar * imageName)
{
    const RwChar       *lastSeparator, *testSeparator, *extender;

    RWAPIFUNCTION(RWSTRING("RwImageRead"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(imageName);

    /* Look for extender by looking for last dot after the last /, \ or : */
    /* Start by assuming that the start of the string is where it is at */
    lastSeparator = imageName;

    /* Now look for each separator character in turn to find the last one */
    testSeparator = rwstrrchr(lastSeparator, ':');
    lastSeparator = testSeparator ? testSeparator : lastSeparator;

    testSeparator = rwstrrchr(lastSeparator, '/');
    lastSeparator = testSeparator ? testSeparator : lastSeparator;

    testSeparator = rwstrrchr(lastSeparator, '\\');
    lastSeparator = testSeparator ? testSeparator : lastSeparator;

    /* Now find the last . after the last separator */
    extender = rwstrrchr(lastSeparator, '.');
    if (extender)
    {
        rwImageFormat      *imageFormat;

        /* Look for the extender in the format table to find out how to read it */
        imageFormat = RWIMAGEGLOBAL(imageFormats);
        while (imageFormat)
        {
            if (!rwstrcmp(imageFormat->lcExtension, extender) ||
                !rwstrcmp(imageFormat->ucExtension, extender))
            {
                /* Got it, can we read? */
                if (imageFormat->readImage)
                {
                    imageReadData       imageData;

                    imageData.readImage = imageFormat->readImage;
                    imageData.image = (RwImage *)NULL;

                    ImagePathForAllFullNames(imageName, 5 *
                                             sizeof(RwChar),
                                             ImageAttempRead,
                                             &imageData);

                    /* If we got it, great, if not, oh well */
                    RWRETURN(imageData.image);
                }

                /* Well, we found the extender, and we couldn't read
                 * it, so we are in trouble...
                 */
                RWRETURN((RwImage *)NULL);
            }

            imageFormat = imageFormat->nextFormat;
        }

        /* Don't know this format */
        RWRETURN((RwImage *)NULL);
    }

    /* Can't parse the name for the extender, unknown format */
    RWRETURN((RwImage *)NULL);
}

/****************************************************************************
 ImageDetermineExtender

 On entry   : pathname to try and read with
            : pointer to read image data block
 On exit    : NULL if image read (to early out), or pathname if we need to try
            : some more...!
 */

static RwChar      *
ImageDetermineExtender(RwChar * pathname, void *data)
{
    RwChar            **extender = (RwChar **) data;
    rwImageFormat      *imageFormat;
    RwChar             *extPos;

    RWFUNCTION(RWSTRING("ImageDetermineExtender"));
    RWASSERT(pathname);
    RWASSERT(extender);

    /* Try adding each extender to the path - we made sure there was enough
     * space already.
     */
    extPos = pathname + rwstrlen(pathname);

    /* Try each file type in the list */
    imageFormat = RWIMAGEGLOBAL(imageFormats);
    while (imageFormat)
    {
        /* Try the lower case version */
        rwstrcpy(extPos, imageFormat->lcExtension);
        if (RwFexist(pathname))
        {
            /* Got it, cache the pointer and early out */
            *extender = imageFormat->lcExtension;
            RWRETURN((char *)NULL);
        }

        /* Try the upper case version */
        rwstrcpy(extPos, imageFormat->ucExtension);
        if (RwFexist(pathname))
        {
            /* Got it, cache the pointer and early out */
            *extender = imageFormat->ucExtension;
            RWRETURN((char *)NULL);
        }

        /* OK, try the next one */
        imageFormat = imageFormat->nextFormat;
    }

    /* Try some more */
    RWRETURN(pathname);
}

/**
 * \ingroup rwimage
 * \ref RwImageFindFileType is used to search for an image of the
 * specified name, conforming to one of the registered types.  It returns an
 * image extension identifying the type of image found, which when appended to
 * the image name will permit loading of the image using \ref RwImageRead or
 * \ref RwImageReadMaskedImage.
 *
 * \param imageName  A pointer to a string containing the file name of the image without the
 * extension.
 *
 * \return Returns a pointer to an extension, or NULL if no image could be found,
 * or if the image filename already included an extension.
 *
 * \see RwImageRead
 * \see RwImageReadMaskedImage
 * \see RwImageSetPath
 *
 * Related Toolkits:
 *
 * RtBMP, RtPNG, RtRAS and RtTIFF.
 *
 */
const RwChar       *
RwImageFindFileType(const RwChar * imageName)
{
    RwChar             *extender;

    RWAPIFUNCTION(RWSTRING("RwImageFindFileType"));
    RWASSERT(imageName);
    RWASSERT(imageModule.numInstances);

    /* For all imagePaths, try all extenders... */
    extender = (char *)NULL;
    ImagePathForAllFullNames(imageName, 20 * sizeof(RwChar),
                             ImageDetermineExtender, &extender);

    /* If we got it, great, if not, c'est la vie */
    RWRETURN(extender);
}

/**
 * \ingroup rwimage
 * \ref RwImageRegisterImageFormat is used to register a new image format
 * with RenderWare.  This will allow loading and saving of the supplied image
 * format if suitable callbacks are supplied.
 *
 * RenderWare Graphics is supplied with a number of Toolkits containing such callbacks.
 * These Toolkits can be used to add support for the following image formats:
 *
 * - Sun Microsystem's Raster, supported by the RtRAS Toolkit;
 * - Portable Network Graphics, supported by the RtPNG Toolkit;
 * - Microsoft Windows Bitmap, supported by the RtBMP Toolkit;
 * - Tagged Image File Format, supported by the RtTIFF Toolkit;
 *
 * See the \e Rasters, \e Images \e & \e Textures chapter in the User Guide for more in-depth coverage of
 * this API.
 *
 * These Toolkits replace the functionality found in previous releases. As a result, the
 * following functions are now considered obsolete and will be deleted in a later
 * release:
 *
 * - RwImageReadBMP()
 * - RwImageReadRAS()
 * - RwImageWriteBMP()
 * - RwImageWriteRAS()
 *
 *
 * You can write your own image file format parsers by providing your own callback functions.
 * The format of the read and write callbacks are:
 *
 * \verbatim
   RwImage * (*RwImageCallBackRead)(const RwChar *imageName);
   RwImage *(*RwImageCallBackWrite)(RwImage *image, const RwChar *imageName);
  \endverbatim
 *
 * An example of writing your own file format parser is provided in the
 * ".../examples/imgformt/" folder.
 *
 * Note that as a default there are no image formats registered with RenderWare.
 * Also, image formats must be registered for reading textures and rasters from
 * image files as these rely on the RwImage functionality.
 *
 * This function must be used after the RenderWare engine has been started.
 *
 * Sample Usage:
 *
 * To enable reading and writing of Sun raster image files together with reading
 * Windows Bitmap files, we must first make sure we are linking against the
 * appropriate libraries, "rtras.lib" and "rtbmp.lib". Next, we include the
 * header files, "rtras.h" and "rtbmp.h".
 *
 * At this point, we can add the following lines to our code after the call to
 * RwEngineStart():
 * \verbatim

   RwImageRegisterImageFormat("ras", RtRASImageRead, RtRASImageWrite);
   RwImageRegisterImageFormat("bmp", RtBMPImageRead,NULL);
   \endverbatim
 *
 * \param extension  Extension type for the image (e.g. bmp, ras)
 * \param imageRead  Pointer to a callback to allow reading of this image type (NULL for
 * no reading possible)
 * \param imageWrite  Pointer to a callback to allow writing of this image type (NULL for
 * no writing possible).
 *
 * \return Returns TRUE on success, or FALSE on failure.
 *
 * \see RwImageRead
 * \see RwImageReadMaskedImage
 * \see RwImageWrite
 * \see RwImageSetPath
 *
 * Related Toolkits: RtPNG, RtRAS, RtBMP, RtTIFF
 */
RwBool
RwImageRegisterImageFormat(const RwChar * extension,
                           RwImageCallBackRead imageRead,
                           RwImageCallBackWrite imageWrite)
{
    rwImageFormat      *newFormat, *imageFormat;
    RwChar              lcExtension[20]; /* Lower case version */

    RWAPIFUNCTION(RWSTRING("RwImageRegisterImageFormat"));
    RWASSERT(extension);
    RWASSERT(imageModule.numInstances);

    /* Need to make sure it's not already there (compare the LC version) */
    lcExtension[0] = '.';
    rwstrncpy(&lcExtension[1], extension, 18);
    lcExtension[19] = 0;
    rwstrlwr(lcExtension);

    /* If we find it already there, override the methods */
    imageFormat = RWIMAGEGLOBAL(imageFormats);
    newFormat = (rwImageFormat *)NULL;
    while (imageFormat && !newFormat)
    {
        if (!rwstrcmp(imageFormat->lcExtension, lcExtension))
        {
            /* Got it, so prepare to replace operations */
            newFormat = imageFormat;
        }
        else
        {
            /* Look at the next one */
            imageFormat = imageFormat->nextFormat;
        }
    }

    /* If we didn't find one, create one and link it in */
    if (!newFormat)
    {
        /* Time to allocate the new element */
        newFormat = (rwImageFormat *)
            RwFreeListAlloc(RWIMAGEGLOBAL(imageFormatFreeList),
                            rwMEMHINTDUR_EVENT | rwID_IMAGEMODULE);

        if (!newFormat)
        {
            /* Allocation failure */
            RWRETURN(FALSE);
        }

        /* Put in the extension */
        /* We already got the lower case version */
        memcpy(newFormat->lcExtension, lcExtension, 20);

        /* Figure out the upper case version */
        newFormat->ucExtension[0] = '.';
        rwstrncpy(&newFormat->ucExtension[1], extension, 18);
        newFormat->ucExtension[19] = 0;
        rwstrupr(newFormat->ucExtension);

        /* And some default I/O functions (none) */
        newFormat->readImage = (RwImageCallBackRead) NULL;
        newFormat->writeImage = (RwImageCallBackWrite) NULL;

        /* And link it in */
        newFormat->nextFormat = RWIMAGEGLOBAL(imageFormats);
        RWIMAGEGLOBAL(imageFormats) = newFormat;
    }

    /* And the different functions for reading/writing (if specified) */
    if (imageRead)
    {
        newFormat->readImage = imageRead;
    }
    if (imageWrite)
    {
        newFormat->writeImage = imageWrite;
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwimage
 * \ref RwImageWrite is used to write an image to a disk file. The image
 * file name must be absolute and include the extension.  The extension
 * determines the type of file written, and must correspond to one of the
 * registered image formats (registered with \ref RwImageRegisterImageFormat).
 *
 * Note that if the image has been gamma corrected using
 * \ref RwImageGammaCorrect, the gamma correction is removed from the image
 * before writing it to disk. Inverse gamma correction is performed using the
 * current gamma correction value.
 *
 * \param image  A pointer to the image
 * \param imageName  A pointer to a string containing the full path name of the image file.
 *
 * \return Returns a pointer to the image if successful or NULL if there is
 * an error.
 *
 * \see RwImageRead
 * \see RwImageRegisterImageFormat
 * \see RwImageReadMaskedImage
 * \see RwImageGammaCorrect
 * \see RwImageSetGamma
 * \see RwImageGetGamma
 *
 * Related Toolkits:
 *
 * RtBMP, RtPNG, RtRAS and RtTIFF.
 *
 */
RwImage            *
RwImageWrite(RwImage * image, const RwChar * imageName)
{
    RwChar             *extender;

    RWAPIFUNCTION(RWSTRING("RwImageWrite"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);
    RWASSERT(imageName);

    extender = rwstrrchr(imageName, '.');
    if (extender)
    {
        rwImageFormat      *imageFormat;

        imageFormat = RWIMAGEGLOBAL(imageFormats);
        while (imageFormat)
        {
            if (!rwstrcmp(imageFormat->lcExtension, extender) ||
                !rwstrcmp(imageFormat->ucExtension, extender))
            {
                /* Got it, can we read? */
                if (imageFormat->writeImage)
                {
                    RWRETURN(imageFormat->writeImage(image, imageName));
                }
            }

            imageFormat = imageFormat->nextFormat;
        }

        /* Don't know this format */
        RWRETURN((RwImage *)NULL);
    }

    /* Can't parse the name for the extender, unknown format */
    RWRETURN((RwImage *)NULL);
}

/**
 * \ingroup rwimage
 * \ref RwImageReadMaskedImage is used to read an image from a disk file.
 * Unlike \ref RwImageRead this function also reads a second file that is
 * used to create a transparency map in the first image.
 * If the image file names are not
 * absolute the current search path is used to determine in which directory
 * the files can be found.
 *
 * The format
 * modules, which have been registered using \ref RwImageRegisterImageFormat ,
 * determine the types of image files that are supported. Note
 * that the filename must include the file's extension, as this identifies the
 * image’s type for dispatch to the correct loader module.
 *
 * No gamma correction is applied to the images.
 *
 * \param imageName  A pointer to a string containing the file name of the first image
 * \param maskName  A pointer to a string containing the file name of the second image
 *       supplying the mask.
 *
 * \return Returns a pointer to the image with the new mask if successful
 * or NULL if there is an error.
 *
 * \see RwImageMakeMask
 * \see RwImageApplyMask
 * \see RwImageRead
 * \see RwImageRegisterImageFormat
 * \see RwImageSetPath
 * \see RwImageGammaCorrect
 * \see RwImageSetGamma
 * \see RwImageGetGamma
 * \see RwImageWrite
 * \see RwRasterRead
 * \see RwRasterReadMaskedRaster
 * \see RwTextureRead
 *
 */
RwImage            *
RwImageReadMaskedImage(const RwChar * imageName,
                       const RwChar * maskName)
{
    RwImage            *image;

    RWAPIFUNCTION(RWSTRING("RwImageReadMaskedImage"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(imageName);

    image = RwImageRead(imageName);
    if (image)
    {
        if ((maskName) && (maskName[0]))
        {
            RwImage            *mask;

            mask = RwImageRead(maskName);
            if (!mask)
            {
                RwImageDestroy(image);
                RWRETURN((RwImage *)NULL);
            }

            if (!RwImageMakeMask(mask))
            {
                RwImageDestroy(image);
                RwImageDestroy(mask);
                RWRETURN((RwImage *)NULL);
            }

            if (!RwImageApplyMask(image, mask))
            {
                RwImageDestroy(image);
                RwImageDestroy(mask);
                RWRETURN((RwImage *)NULL);
            }

            /* No longer need the mask */
            RwImageDestroy(mask);
        }

        /* All done */
        RWRETURN(image);
    }

    /* Failed to read image */
    RWRETURN((RwImage *)NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Access

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwrgba
 * \ref RwRGBAToPixel is used to convert an \ref RwRGBA value into a device
 * dependent pixel value.
 *
 * \param rgbIn  A pointer to the RwRGBA value to convert
 * \param rasterFormat  An \ref RwInt32 value specifying the raster format to
 * convert for.
 *
 * \return Returns the device dependent pixel value if successful or
 * zero is there is an error.
 *
 * \see RwRGBASetFromPixel
 * \see RwRasterCreate
 * \see RwRasterGetFormat
 *
 */
RwUInt32
RwRGBAToPixel(RwRGBA * rgbIn, RwInt32 rasterFormat)
{
    RwUInt32             nPixel;

    RWAPIFUNCTION(RWSTRING("RwRGBAToPixel"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(rgbIn);

    RWSRCGLOBAL(stdFunc[rwSTANDARDRGBTOPIXEL]) (&nPixel, rgbIn,
                                                rasterFormat);
    RWRETURN(nPixel);
}

/**
 * \ingroup rwrgba
 * \ref RwRGBASetFromPixel is used to convert a device dependent pixel
 * value into an RwRGBA value.
 *
 * \param rgbOut  A pointer to an \ref RwRGBA that will receive the converted value
 * \param pixelValue  An \ref RwInt32 value equal to the device dependent pixel value
 * \param rasterFormat  An \ref RwInt32 value specifying the raster format to convert for.
 *
 * \return Returns pointer to the converted pixel value.
 *
 * \see RwRGBAToPixel
 * \see RwRasterCreate
 * \see RwRasterGetFormat
 *
 */
RwRGBA             *
RwRGBASetFromPixel(RwRGBA * rgbOut, RwUInt32 pixelValue,
                   RwInt32 rasterFormat)
{
    RwUInt32             nInPixel = pixelValue;

    RWAPIFUNCTION(RWSTRING("RwRGBASetFromPixel"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(rgbOut);

    RWSRCGLOBAL(stdFunc[rwSTANDARDPIXELTORGB]) (rgbOut,
                                                &nInPixel,
                                                rasterFormat);
    RWRETURN(rgbOut);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Copying/converting images

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 ImageConvertDepth

 Assumes that the palette in the image is the same as the devices !!!

 On entry   : Destination image
            : Source image
 On exit    : TRUE on success
 */

/****************************************************************************
 ImageStraightCopy

 On entry   : Destination
            : Source
 On exit    : TRUE on success
 */

static              RwBool
ImageStraightCopy(RwImage * ipDestin, const RwImage * ipSource)
{
    RwInt32             i;
    RwInt32             nSpanLength;
    const RwUInt8      *cpSrc;
    RwUInt8            *cpDst;

    RWFUNCTION(RWSTRING("ImageStraightCopy"));
    RWASSERT(ipDestin);
    RWASSERT(ipSource);

    RWASSERT(ipDestin->width == ipSource->width);
    RWASSERT(ipDestin->height == ipSource->height);

    if ((ipDestin->palette) && (ipSource->palette)
        && (ipSource->depth <= 8))
    {
        /* Copy the palette first */
        memcpy(ipDestin->palette, ipSource->palette,
               sizeof(RwRGBA) * (1 << ipSource->depth));
    }

    /* Copy the pixels */
    nSpanLength = ((ipDestin->depth + 7) >> 3) * ipDestin->width;

    cpSrc = ipSource->cpPixels;
    cpDst = ipDestin->cpPixels;

    for (i = 0; i < ipDestin->height; i++)
    {
        memcpy(cpDst, cpSrc, nSpanLength);

        cpDst += ipDestin->stride;
        cpSrc += ipSource->stride;
    }

    /* All done */
    RWRETURN(TRUE);
}

#define GENSWITCHKEY(srcDepth, destDepth) \
    (((srcDepth) << 8) | (destDepth))

static              RwBool
ImageConvertDepth(RwImage * ipDestin, const RwImage * ipSource)
{
    RwBool              result = FALSE;
    RwInt32             i, j;
    RwInt32             width;
    RwInt32             height;
    RwUInt32            switchKey;
    RwUInt8            *cpDst;
    const RwRGBA       *rpSrcPalette;
    const RwUInt8      *cpSrc;

    RWFUNCTION(RWSTRING("ImageConvertDepth"));
    RWASSERT(ipDestin);
    RWASSERT(ipSource);

    width = ipDestin->width;
    height = ipDestin->height;

    RWASSERT(width == ipSource->width);
    RWASSERT(height == ipSource->height);

    rpSrcPalette = ipSource->palette;

    cpSrc = ipSource->cpPixels;
    cpDst = ipDestin->cpPixels;

    /* They are of different depths */
    switchKey = GENSWITCHKEY(ipSource->depth, ipDestin->depth);

    switch (switchKey)
    {
        case (GENSWITCHKEY(4, 4)):
        case (GENSWITCHKEY(8, 8)):
        case (GENSWITCHKEY(32, 32)):
            {

                /* These are trivial - we're already there. */
                result = TRUE;
            }

            break;

        case (GENSWITCHKEY(4, 8)):
            {

                /* Four to 8 bit - straight copy...! */
                for (i = 0; i < height; i++)
                {
                    memcpy(cpDst, cpSrc, width * sizeof(RwUInt8));
                    cpSrc += ipSource->stride;
                    cpDst += ipDestin->stride;
                }
                result = TRUE;
            }

            break;

        case (GENSWITCHKEY(4, 32)):
        case (GENSWITCHKEY(8, 32)):
            {

                /* 4/8->32 bit */
                for (i = 0; i < height; i++)
                {
                    RwRGBA             *rpDst = (RwRGBA *) cpDst;

                    for (j = 0; j < width; j++)
                    {
                        rpDst[j] = rpSrcPalette[cpSrc[j]];
                    }

                    cpSrc += ipSource->stride;
                    cpDst += ipDestin->stride;
                }
                result = TRUE;
            }

            break;

        case (GENSWITCHKEY(8, 4)):
        case (GENSWITCHKEY(32, 4)):
        case (GENSWITCHKEY(32, 8)):
        default:
            {

                RWERROR((E_RW_INVIMAGEFORMAT));
                /* Doesn't have palette matching abilities yet !!! */
            }

            break;
    }

    RWRETURN(result);
}

/**
 * \ingroup rwimage
 * \ref RwImageCopy is used to copy the contents of one image to another.
 * The source and destination images must have the same pixel resolution but
 * they can have different depths. The only currently supported depth
 * conversions are from 4 or 8- to 32-bits per pixels and it is an error to
 * copy a 32-bit image to an 8 or 4-bit image. Note that the destination
 * images must have memory allocated for its pixel data.
 *
 * \param destImage  Pointer to the image receiving the image data
 * \param sourceImage  Pointer to the image supplying the image data.
 *
 * \return Returns a pointer to the destination image if successful or NULL
 * if there is an error.
 *
 * \see RwImageCreate
 *
 */
RwImage            *
RwImageCopy(RwImage * destImage, const RwImage * sourceImage)
{
    RWAPIFUNCTION(RWSTRING("RwImageCopy"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(destImage);
    RWASSERT(sourceImage);
    RWASSERT(destImage->width == sourceImage->width);
    RWASSERT(destImage->height == sourceImage->height);

    if (destImage->depth == sourceImage->depth)
    {
        if (!ImageStraightCopy(destImage, sourceImage))
        {
            /* Copy failed */
            destImage = (RwImage *)NULL;
        }
    }
    else
    {
        if (!ImageConvertDepth(destImage, sourceImage))
        {
            /* Copy failed */
            destImage = (RwImage *)NULL;
        }
    }

    /* mark pixels as gamma corrected or not */
    destImage->flags &= ~rwIMAGEGAMMACORRECTED;
    destImage->flags |= sourceImage->flags & rwIMAGEGAMMACORRECTED;

    RWRETURN(destImage);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Gamma Correction

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 *
 * Plugin registration functions
 */

/**
 * \ingroup rwimage
 * \ref RwImageRegisterPlugin is used to register a plugin and
 * reserve some space within a image. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * \param size  An \ref RwInt32 value equal to the size of the memory block to reserve
 * \param pluginID  An \ref RwInt32 equal to the plugin's ID number
 * \param constructCB  A constructor for the plugin data block
 * \param destructCB  A destructor for the plugin data block
 * \param copyCB  A copy constructor for the plugin data block.
 *
 * \return Returns the byte offset within the image of memory reserved
 * for this plugin or a negative value if there is an error.
 *
 * \see RwImageValidatePlugins
 * \see RwImageGetPluginOffset
 *
 */
RwInt32
RwImageRegisterPlugin(RwInt32 size,
                      RwUInt32 pluginID,
                      RwPluginObjectConstructor constructCB,
                      RwPluginObjectDestructor destructCB,
                      RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RwImageRegisterPlugin"));
    RWASSERT(!imageModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlugin(&imageTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwimage
 * \ref RwImageGetPluginOffset is used to get the offset of a previously
 * registered plugin.
 *
 * \param pluginID  The ID of the plugin whose data is to be retrieved.
 *
 * \return Returns an \ref RwInt32 that equals the data block offset or -1 if the plugin is not registered.
 *
 * \see RwImageRegisterPlugin
 * \see RwImageValidatePlugins
 */
RwInt32
RwImageGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RwImageGetPluginOffset"));

    offset = _rwPluginRegistryGetPluginOffset(&imageTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rwimage
 * \ref RwImageValidatePlugins is used to validate the plugin memory
 * allocated within the specified image. This function is useful for
 * determining where memory trampling may be occurring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * \param  image   A pointer to the image to validate.
 *
 * \return Returns TRUE is the image data is valid or FALSE if there is an
 * error or if the image data has become corrupt.
 *
 * \see RwImageRegisterPlugin
 * \see RwImageGetPluginOffset
 *
 */
RwBool
RwImageValidatePlugins(const RwImage * image __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RwImageValidatePlugins"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

#ifdef RWDEBUG
    {
        RwBool              plug;

        plug = _rwPluginRegistryValidateObject(&imageTKList, image);

        RWRETURN(plug);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

/**
 * \ingroup rwimage
 * \ref RwImageStreamGetSize
 * is used to determine the size in bytes of the binary representation of
 * the given image.
 *
 * This value is used in the binary chunk header to indicate the size of
 * the chunk.
 *
 * The size does not include the size of the chunk header.
 *
 * \param image  A pointer to the image.
 *
 * \return Returns An \ref RwInt32 value equal to the chunk size of the image in bytes.
 *
 * \see RwImageStreamRead
 * \see RwImageStreamWrite
 */
RwInt32
RwImageStreamGetSize(const RwImage * image)
{
    RwInt32             size = 0;

    RWAPIFUNCTION(RWSTRING("RwImageStreamGetSize"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    size += rwCHUNKHEADERSIZE;
    size += sizeof(_rwImage);
    size += RwImageGetHeight(image) * RwImageGetStride(image);
    if (RwImageGetDepth(image) <= 8)
    {
        size += sizeof(RwRGBA) * (1 << RwImageGetDepth(image));
    }

    RWRETURN(size);
}

/**
 * \ingroup rwimage
 * \ref RwImageStreamRead
 * is used to read an image from the specified binary stream.
 *
 * Note that prior to this function call a binary image chunk must be
 * found in the stream using the \ref RwStreamFindChunk API function.
 *
 * The sequence to locate and read an image from a binary stream
 * connected to a disk file is as follows
 * \code
   RwStream *stream;
   RwImage *newImage;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
        if( RwStreamFindChunk(stream, rwID_IMAGE, NULL, NULL) )
        {
                newImage = RwImageStreamRead(stream);
        }

        RwStreamClose(stream, NULL);
  }
  \endcode
 *
 * \param stream  A pointer to the stream.
 *
 * \return A pointer to the image if successful or NULL if there is an error.
 *
 * \see RwImageStreamWrite
 * \see RwImageStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 *
 *
 */
RwImage            *
RwImageStreamRead(RwStream * stream)
{
    RwImage            *image = (RwImage *)NULL;
    _rwImage            imageDesc;
    RwUInt32            size;
    RwUInt32            version;
    RwInt32             newStride;

    RWAPIFUNCTION(RWSTRING("RwImageStreamRead"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RwImage *)NULL);
    }

    RwStreamRead(stream, &imageDesc, sizeof(_rwImage));
    (void)RwMemNative32(&imageDesc, sizeof(_rwImage));

    image =
        RwImageCreate(imageDesc.width, imageDesc.height,
                      imageDesc.depth);
    RwImageAllocatePixels(image);

    newStride = RwImageGetStride(image);

    if (newStride == imageDesc.stride)
    {
        RwStreamRead(stream, RwImageGetPixels(image),
                     RwImageGetHeight(image) * RwImageGetStride(image));
    }
    else
    {
        RwInt32             y;

        for (y = 0; y < imageDesc.height; y++)
        {
            if (newStride > imageDesc.stride)
            {
                RwStreamRead(stream,
                             RwImageGetPixels(image) +
                             (y * newStride), imageDesc.stride);
            }
            else
            {
                RwStreamRead(stream,
                             RwImageGetPixels(image) +
                             (y * newStride), newStride);
                RwStreamSkip(stream, imageDesc.stride - newStride);
            }
        }
    }

    if (RwImageGetDepth(image) <= 8)
    {
        RwStreamRead(stream, RwImageGetPalette(image),
                     (1 << RwImageGetDepth(image)) * sizeof(RwRGBA));
    }

    RWRETURN(image);
}

/**
 * \ingroup rwimage
 * \ref RwImageStreamWrite
 * is used to write the specified image to the given binary stream.
 *
 * Note that the stream will have been opened prior to this function call.
 *
 * \param image  A pointer to the image
 * \param stream  A pointer to the stream.
 *
 * \return Returns a pointer to the image if successful or NULL if there is an error.
 *
 * \see RwImageStreamRead
 * \see RwImageStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 */
const RwImage      *
RwImageStreamWrite(const RwImage * image, RwStream * stream)
{
    _rwImage            imageDesc;

    RWAPIFUNCTION(RWSTRING("RwImageStreamWrite"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader
        (stream, rwID_IMAGE, RwImageStreamGetSize(image)))
    {
        RWRETURN((const RwImage *)NULL);
    }

    if (!RwStreamWriteChunkHeader
        (stream, rwID_STRUCT, sizeof(_rwImage)))
    {
        RWRETURN((const RwImage *)NULL);
    }

    imageDesc.width = RwImageGetWidth(image);
    imageDesc.height = RwImageGetHeight(image);
    imageDesc.depth = RwImageGetDepth(image);
    imageDesc.stride = RwImageGetStride(image);
    (void)RwMemLittleEndian32(&imageDesc, sizeof(_rwImage));

    RwStreamWrite(stream, &imageDesc, sizeof(_rwImage));

    RwStreamWrite(stream, RwImageGetPixels(image),
                  RwImageGetHeight(image) * RwImageGetStride(image));
    if (RwImageGetDepth(image) <= 8)
    {
        RwStreamWrite(stream, RwImageGetPalette(image),
                      (1 << RwImageGetDepth(image)) * sizeof(RwRGBA));
    }

    RWRETURN(image);
}

/**
 * \ingroup rwrgba
 * \ref RwRGBAGammaCorrect is used to gamma correct an individual
 * RwRGBA pixel according to the current gamma correction value. The
 * gamma value is specified using the function
 * \ref RwImageSetGamma.
 *
 * \param rgb  A pinter to the \ref RwRGBA value which is to be gamma corrected.
 *
 * \return Returns a pointer to the gamma corrected RwRGBA value.
 *
 * \see RwImageSetGamma
 * \see RwImageGetGamma
 * \see RwRGBASetFromPixel
 * \see RwRGBAToPixel
 *
 */
RwRGBA             *
RwRGBAGammaCorrect(RwRGBA * rgb)
{
    RWAPIFUNCTION(RWSTRING("RwRGBAGammaCorrect"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(rgb);

    rgb->red = RWIMAGEGLOBAL(gammaTable[rgb->red]);
    rgb->green = RWIMAGEGLOBAL(gammaTable[rgb->green]);
    rgb->blue = RWIMAGEGLOBAL(gammaTable[rgb->blue]);

    RWRETURN(rgb);
}

/**
 * \ingroup rwimage
 * \ref RwImageGammaCorrect is used to apply gamma correction to the
 * specified image according to the currently set gamma correction value. The
 * gamma value is specified using the function \ref RwImageSetGamma.
 *
 * \param image  Pointer to the image to gamma correct.
 *
 * \return Returns pointer to the image if successful or NULL if there is
 * an error.
 *
 * \see RwImageSetGamma
 * \see RwImageGetGamma
 *
 */
RwImage            *
RwImageGammaCorrect(RwImage * image)
{
    RWAPIFUNCTION(RWSTRING("RwImageGammaCorrect"));
    RWASSERT(imageModule.numInstances);
    RWASSERT(image);

    switch (image->depth)
    {
        case (4):
        case (8):
            {
                RwRGBA             *pal = image->palette;
                RwUInt32            palSize = (1 << image->depth);

                if (!pal)
                {
                    RWERROR((E_RW_NULLP));
                    RWRETURN((RwImage *)NULL);
                }

                /* Its 4/8 bit -> just process the palette in place */
                _rwImageGammaCorrectArrayOfRGBA(pal, pal, palSize);

                break;
            }
        case (32):
            {
                RwUInt8            *curLine = image->cpPixels;
                RwInt32             width = image->width;
                RwInt32             height = image->height;
                RwInt32             y;

                if (!curLine)
                {
                    RWERROR((E_RW_NULLP));
                    RWRETURN((RwImage *)NULL);
                }

                /* It must be 32 bit -> process as such */
                for (y = 0; y < height; y++)
                {
                    /* Do the scan line in place */
                    _rwImageGammaCorrectArrayOfRGBA((RwRGBA *) curLine,
                                                    (RwRGBA *) curLine,
                                                    width);

                    curLine += image->stride;
                }
                break;
            }
        default:
            {
                /* Don't recognise this one. */
                RWERROR((E_RW_INVIMAGEDEPTH));
                RWRETURN((RwImage *)NULL);
            }
    }

    /* Set the flag saying that this has been gamma corrected */
    image->flags |= rwIMAGEGAMMACORRECTED;

    RWRETURN(image);
}

/**
 * \ingroup rwimage
 * \ref RwImageGetGamma is used to retrieve the current
 * gamma correction value.
 *
 * \return Returns an \ref RwReal value equal to the gamma correction value
 * currently in force.
 *
 * \see RwImageSetGamma
 * \see RwImageGammaCorrect
 *
 */
RwReal
RwImageGetGamma(void)
{
    RWAPIFUNCTION(RWSTRING("RwImageGetGamma"));
    RWASSERT(imageModule.numInstances);

    RWRETURN(RWIMAGEGLOBAL(gammaVal));
}

/**
 * \ingroup rwimage
 * \ref RwImageSetGamma is used to specify the current
 * gamma correction value. Gamma
 * values greater than 1.0 will lighten an image while values less than 1.0
 * will darken an image. Typically, used to adjust an image prior to display
 * on a monitor whose gamma value differs from unity.
 *
 * The default gamma value is ( \ref RwReal)((1.0)).
 *
 * \param gammaValue  An \ref RwReal value equal to the gamma value to be set.
 *
 * \return Returns TRUE.
 *
 * \see RwImageGetGamma
 * \see RwImageGammaCorrect
 *
 */
RwBool
RwImageSetGamma(RwReal gammaValue)
{
    RwReal              nGammaExp, nGammaInv;
    RwInt32             i;

    RWAPIFUNCTION(RWSTRING("RwImageSetGamma"));
    RWASSERT(imageModule.numInstances);

    RWIMAGEGLOBAL(gammaVal) = gammaValue;

    /* calc gamma tables */
    nGammaExp = ((gammaValue));
    nGammaInv = (1.0f) / nGammaExp;

    RWIMAGEGLOBAL(gammaTable[0]) = 0;
    RWIMAGEGLOBAL(invGammaTable[0]) = 0;
    for (i = 1; i < 256; i++)
    {
        /* We rely on theRwReal being promoted to a double
         * in the call - this keeps the equator compiler happy
         */
        RwReal              nT;
        RwReal              scaled;
        RwInt32             quantize;

        nT = ((float) i) / 255.0f;
        scaled = ((RwReal)RwPow(nT, nGammaInv)) * 255.0f;
        quantize = RwInt32FromRealMacro(scaled + 0.5f);
        RWIMAGEGLOBAL(gammaTable[i]) = (RwUInt8) quantize;

        scaled = ((RwReal)RwPow(nT, nGammaExp)) * 255.0f;
        quantize = RwInt32FromRealMacro(scaled + 0.5f);
        RWIMAGEGLOBAL(invGammaTable[i]) = (RwUInt8) quantize;
    }

    RWRETURN(TRUE);
}
