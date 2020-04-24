/*
 * Image/raster coupling handling.
 * This abstracts the interface between the two data types
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"

#include "baimage.h"
#include "baraster.h"
#include "baresamp.h"

/* Image/raster coupling handling */
#include "baimras.h"


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

/**
 * \ingroup rwimage
 * \ref RwImageSetFromRaster is used to convert the specified (device
 * dependent) raster into an equivalent (device independent) image. The image
 * and raster dimensions should be the same and it is recommended that the
 * image be created with a depth be 32-bits per pixel. The image must have
 * pixels allocated for it before calling this function.
 *
 * If the raster is locked before calling this function then the pixel
 * data is taken from the currently locked mip level of the raster, and the
 * lock is not released afterwards. Otherwise the raster is locked at mip
 * level 0 in order to extract the pixel data and then unlocked afterwards.
 *
 * \param image  A pointer to the image that will receive the raster data
 * \param raster  A pointer to the raster that supplies the image data.
 *
 * \return Returns pointer to the image if successful or NULL if
 * there is an error.
 *
 * \verbatim
   To grab and save the camera's image raster to a disk file:

   RwRaster *raster;
   RwImage *image;
   RwInt32 x, y;
   RwChar *filename;

   raster = RwCameraGetRaster(camera);
   if( raster )
   {
       x = RwRasterGetWidth(raster);
       y = RwRasterGetHeight(raster);
       image = RwImageCreate(x, y, 32);
       if( image )
       {
           RwImageAllocatePixels(image);
           RwImageSetFromRaster(image, raster);
           RwImageWrite(image, filename);
           RwImageDestroy(image);
       }
   }
   \endverbatim
 *
 */
RwImage *
RwImageSetFromRaster(RwImage *image, RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwImageSetFromRaster"));
    RWASSERT(image);
    RWASSERT(image->cpPixels && (image->palette || image->depth == 32));
    RWASSERT(raster);
    RWASSERT(RwImageGetWidth(image) == RwRasterGetWidth(raster));
    RWASSERT(RwImageGetHeight(image) == RwRasterGetHeight(raster));

    /* Depth conversions should be handled by the device driver */
    if (RWSRCGLOBAL(stdFunc[rwSTANDARDIMAGEGETRASTER])(image, raster, 0))
    {
        /* Copy across the gamma correction flag */
        if (raster->privateFlags & rwRASTERGAMMACORRECTED)
        {
            image->flags |= rwIMAGEGAMMACORRECTED;
        }
        RWRETURN(image);
    }

    /* Device failure */
    RWRETURN((RwImage *)NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Set the image

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwraster
 * \ref RwRasterSetFromImage is used to change the pixels of the specified
 * raster to those of the given image. The width and height of the image
 * must be equal to those of the raster.
 *
 * If the raster is locked before calling this function then the pixel
 * data is put into the currently locked mip level of the raster, and the
 * lock is not released afterwards. Otherwise the raster is locked at mip
 * level 0 in order to set the pixel data and then unlocked afterwards.
 *
 * \param raster  Pointer to the raster whose pixels will be changed
 * \param image  Pointer to the image providing the pixel data.
 *
 * \return Returns pointer to the raster if successful or NULL if there
 * is an error.
 *
 * \see RwImageSetFromRaster
 * \see RwRasterCreate
 *
 */
RwRaster *
RwRasterSetFromImage(RwRaster *raster, RwImage *image)
{
    RWAPIFUNCTION(RWSTRING("RwRasterSetFromImage"));
    RWASSERT(image);
    RWASSERT(image->cpPixels && (image->palette || image->depth == 32));
    RWASSERT(raster);
    RWASSERT(RwImageGetWidth(image) == RwRasterGetWidth(raster));
    RWASSERT(RwImageGetHeight(image) == RwRasterGetHeight(raster));

    if (RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERSETIMAGE])(raster, image, 0))
    {
        /* Copy across the gamma correction flag */
        if (image->flags & rwIMAGEGAMMACORRECTED)
        {
            raster->privateFlags |= rwRASTERGAMMACORRECTED;
        }
        RWRETURN(raster);
    }

    /* Device failed to set image */
    RWRETURN((RwRaster *)NULL);
}

/**
 * \ingroup rwimage
 * \ref RwImageFindRasterFormat is used to determine a suitable raster
 * format for a given image and the current target.  Passing in the image to
 * be considered, and a raster type flag containing type and mipmap
 * format (not pixel format) will return a size, depth and format that can
 * be used to create a suitable target raster for the image.
 *
 * \param ipImage  A pointer to the image for which to determine a suitable
 * raster format
 * \param nRasterType  A hint for the raster type and mipmap format to use
 * \param npWidth  A pointer to an \ref RwInt32 value to receive the target raster's width
 * \param npHeight  A pointer to an \ref RwInt32 value to receive the target raster's height
 * \param npDepth  A pointer to an \ref RwInt32 value to receive the target raster's pixel depth
 * \param npFormat  A pointer to an \ref RwInt32 value to receive the target raster's format.
 *
 * \return Returns a pointer to the image if successful, or NULL on failure.
 *
 * \see RwRasterSetFromImage
 * \see RwRasterCreate
 * \see RwRasterGetFormat
 *
 */
RwImage *
RwImageFindRasterFormat(RwImage *ipImage, RwInt32 nRasterType,
                        RwInt32 *npWidth, RwInt32 *npHeight,
                        RwInt32 *npDepth, RwInt32 *npFormat)
{
    RwRaster            rRaster;

    RWAPIFUNCTION(RWSTRING("RwImageFindRasterFormat"));
    RWASSERT(ipImage);
    RWASSERT(ipImage->cpPixels && (ipImage->palette || ipImage->depth == 32));
    RWASSERT(npWidth);
    RWASSERT(npHeight);
    RWASSERT(npDepth);
    RWASSERT(npFormat);

    /* Hook the device to figure out what the deal is */
    if (!RWSRCGLOBAL(stdFunc[rwSTANDARDIMAGEFINDRASTERFORMAT])(&rRaster, ipImage, nRasterType))
    {
        RWRETURN((RwImage *)NULL);
    }

    /* What about mip mapping ??? */
    (*npFormat) = RwRasterGetFormat(&rRaster) | rRaster.cType;
    (*npWidth) = rRaster.width;
    (*npHeight) = rRaster.height;
    (*npDepth) = rRaster.depth;

#ifdef RWDEBUG
    {
        RwBool rasterPalettised = ((RwRasterGetFormat(&rRaster) &
            (rwRASTERFORMATPAL8 | rwRASTERFORMATPAL4)) != 0);
        RwRasterFormat rasterFormat =
            (RwRasterFormat)( RwRasterGetFormat(&rRaster) | rRaster.cType );
        RwInt32 rasterDepth = RwRasterGetDepth(&rRaster);
        RwInt32 rasterPalDepth = 0;

        RwBool imagePalettised = (RwImageGetPalette(ipImage) != NULL);
        /*RwInt32 imageDepth = RwImageGetDepth(ipImage);*/
        RwInt32 imageDepth = 32;
        RwInt32 imagePalDepth = 0;

        /* Pallettised */
        if (rasterFormat & rwRASTERFORMATPAL4)
        {
            rasterPalDepth = 4;
        }
        else if (rasterFormat & rwRASTERFORMATPAL8)
        {
            rasterPalDepth = 8;
        }

        if ((ipImage->depth == 4) || (ipImage->depth == 8))
        {
            imagePalDepth = ipImage->depth;
        }

        switch( rasterFormat & rwRASTERFORMATPIXELFORMATMASK )
        {
            case rwRASTERFORMAT8888:
            case rwRASTERFORMAT32:
                rasterDepth = 32;
                break;
            case rwRASTERFORMAT888:
            case rwRASTERFORMAT24:
                rasterDepth = 24;
                break;
            case rwRASTERFORMAT16:
            case rwRASTERFORMAT1555:
            case rwRASTERFORMAT565:
            case rwRASTERFORMAT4444:
            case rwRASTERFORMAT555:
                rasterDepth = 16;
                break;
            case rwRASTERFORMATLUM8:
                rasterDepth = 8;
                break;
            default:
                rasterDepth = 0;
        }

        if ( (ipImage->width  != rRaster.width)  ||
             (ipImage->height != rRaster.height) ||
             (imageDepth      != rasterDepth)    ||
             (imagePalDepth   != rasterPalDepth) ||
             (imagePalettised != rasterPalettised) )
        {
            RwChar message[512];

            rwsprintf(message,
                RWSTRING("Image Conversion:\n"
 "Image: width=%d height=%d pixel depth=%d palette depth=%d palettised=%s\n"
 "Raster: width=%d height=%d pixel depth=%d palette depth=%d palettised=%s "
                         "format=0x%x"),
                ipImage->width, ipImage->height,
                imageDepth, imagePalDepth,
                imagePalettised?"Yes":"No",
                rRaster.width, rRaster.height,
                rasterDepth, rasterPalDepth,
                rasterPalettised?"Yes":"No",
                rasterFormat);

            RWMESSAGE(("%s", message));
        }
    }
#endif /* RWDEBUG */

    /* Completed successfully */
    RWRETURN(ipImage);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   File helper functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwraster
 * \ref RwRasterRead is used to read an image from a disk file and
 * convert it into a raster. If the file name is not an absolute path name the
 * current search path is used to determine in which directory the image file
 * can be found.
 *
 * The types of image files that are supported are determined by the format
 * modules which have been registered using \ref RwImageRegisterImageFormat. Note
 * that the filename must include the file's extension, as this identifies the
 * image’s type for dispatch to the correct loader module.
 *
 * The raster is created as type rwRASTERTYPENORMAL and with the default depth.
 *
 * Gamma correction is not applied to the raster’s image
 *
 * \param filename  A pointer to a string specifying the file name of the image.
 *
 * \return Returns a pointer to the resulting raster if successful or NULL
 * if there is an error.
 *
 * \see RwRasterReadMaskedRaster
 * \see RwImageSetPath
 * \see RwImageRead
 * \see RwImageRegisterImageFormat
 * \see RwImageReadMaskedImage
 * \see RwTextureRead
 *
 */
RwRaster *
RwRasterRead(const RwChar *filename)
{
    RwImage     *ipImage;
    RwInt32     width, height, depth, format;
    RwRaster    *raster;

    RWAPIFUNCTION(RWSTRING("RwRasterRead"));
    RWASSERT(filename);

    ipImage = RwImageRead(filename);
    if (!ipImage)
    {
        RWRETURN((RwRaster *)NULL);
    }

    /* Find a suitably size and format */
    RwImageFindRasterFormat(ipImage, rwRASTERTYPENORMAL,
                            &width, &height, &depth, &format);

    if( width != RwImageGetWidth(ipImage) ||
        height != RwImageGetHeight(ipImage) )
    {
        RwImage *imageResampled;

        imageResampled = RwImageCreateResample(ipImage, width, height);

        RwImageDestroy(ipImage);

        ipImage = imageResampled;
    }

    raster = RwRasterCreate(width, height, depth, format | rwRASTERTYPENORMAL);
    if (!raster)
    {
        RwImageDestroy(ipImage);
        RWRETURN((RwRaster *)NULL);
    }

    /* Set the rasters image */
    RwRasterSetFromImage(raster, ipImage);

    /* all has gone to plan so destroy original image */
    RwImageDestroy(ipImage);

    /* All done ! */
    RWRETURN(raster);
}

/**
 * \ingroup rwraster
 * \ref RwRasterReadMaskedRaster is used to read two images from disk
 * files, apply the mask contained in the second to the first and convert the
 * result to a raster. If the file names are not absolute path names the
 * current search path is used to determine in which directory the image files
 * can be found.
 *
 * The types of image files that are supported are determined by the format
 * modules which have been registered using \ref RwImageRegisterImageFormat. Note
 * that the filename must include the file's extension, as this identifies the
 * image’s type for dispatch to the correct loader module.
 *
 * The raster is created as type rwRASTERTYPENORMAL and with the default depth.
 *
 * Gamma correction is not applied to the raster’s image.
 *
 * \param filename  A pointer to a file name of the first image
 * \param maskname  A pointer to a file name of the second image supplying the mask.
 *
 * \return Returns a pointer to the resulting raster if successful
 * or NULL if there is an error.
 *
 * \see RwRasterRead
 * \see RwImageSetPath
 * \see RwImageReadMaskedImage
 * \see RwImageRegisterImageFormat
 * \see RwImageRead
 * \see RwTextureRead
 *
 */
RwRaster *
RwRasterReadMaskedRaster(const RwChar *filename,
                         const RwChar *maskname)
{
    RwRaster    *raster;
    RwImage     *ipImage;
    RwInt32     width, height, depth;

    RWAPIFUNCTION(RWSTRING("RwRasterReadMaskedRaster"));
    RWASSERT(filename);

    ipImage = RwImageReadMaskedImage(filename, maskname);
    if (!ipImage)
    {
        RWRETURN((RwRaster *)NULL);
    }

    width = RwImageGetWidth(ipImage);
    height = RwImageGetHeight(ipImage);
    depth = RwImageGetDepth(ipImage);

    /* Create a suitably sized raster - default raster depth */
    raster = RwRasterCreate(width, height, 0, rwRASTERTYPENORMAL);
    if (!raster)
    {
        RwImageDestroy(ipImage);
        RWRETURN((RwRaster *)NULL);
    }

    /* Set the rasters image */
    RwRasterSetFromImage(raster, ipImage);

    /* all has gone to plan so destroy original image */
    RwImageDestroy(ipImage);

    /* All done ! */
    RWRETURN(raster);
}

