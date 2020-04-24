/*
 * Image re-sampling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "balibtyp.h"
#include "batype.h"
#include "badebug.h"
#include "bamemory.h"
#include "baimage.h"
#include "baimage.h"
#include "osintf.h"
#include "bamath.h"

#include "baresamp.h"

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

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Re-sampling of images

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 ImageResampleGetSpan

 On entry   : Source
            : Start x
            : End x
            : y
            : RGB of area of span (OUT)
 On exit    : TRUE if pixel else transparent
 */

static  void
ImageResampleGetSpan(const RwImage * _image,
                     RwFixed _nStartX,
                     RwFixed _nEndX,
                     RwFixed _nY,
                     RwRGBAReal * _rrpCol)
{
    RwReal              nArea;
    RwReal              nScale;
    RwRGBA             *rpSpan;

    RWFUNCTION(RWSTRING("ImageResampleGetSpan"));
    RWASSERT((_image));
    RWASSERT((_rrpCol));

    nArea = RwFixedToReal((_nEndX) - (_nStartX));
    rpSpan =  ( ((RwRGBA *) ((_image)->cpPixels +  (RwFixedToInt((_nY)) *
                                                 (_image)->stride))) +
                RwFixedToInt((_nStartX)) );

    if (RwFixedToInt((_nStartX)) == RwFixedToInt((_nEndX)))
    {
        /* Starts and ends in the same pixel */
        RwRGBARealFromRwRGBA((_rrpCol), rpSpan);
        nScale = RwFixedToReal((_nEndX) - (_nStartX));
        RwRGBARealScale((_rrpCol), (_rrpCol), nScale);
    }
    else
    {
        RwRGBAReal rrAdd;
        RwFixed nPos = RwIntToFixed(RwFixedToInt((_nStartX)) + 1);

        /* It has a start and an end and possibly a middle bit */
        RwRGBARealFromRwRGBA((_rrpCol), rpSpan);
        nScale = RwFixedToReal(nPos - (_nStartX));
        RwRGBARealScale((_rrpCol), (_rrpCol), nScale);
        rpSpan++;

        /* Do the middle section */
        while (RwFixedToInt(nPos) != RwFixedToInt((_nEndX)))
        {
            RwRGBARealFromRwRGBA(&rrAdd, rpSpan);
            RwRGBARealAdd((_rrpCol), (_rrpCol), &rrAdd);
            rpSpan++;

            /* Next pixel */

            nPos += RwIntToFixed(1);
        }

        RwRGBARealFromRwRGBA(&rrAdd, rpSpan);
        nScale = RwFixedToReal((_nEndX) - nPos);
        RwRGBARealScale(&rrAdd, &rrAdd, nScale);
        RwRGBARealAdd((_rrpCol), (_rrpCol), &rrAdd);
    }

    /* Work out the colour of this span */
    nScale = (((RwReal)1) / (nArea));
    RwRGBARealScale((_rrpCol), (_rrpCol), nScale);

    /* All done */
    RWRETURNVOID();
}

/****************************************************************************
 ImageResampleGetAvgPixel

 On entry   : Source
            : Start x
            : End x
            : Start y
            : End y
            : RGB (OUT)
 On exit    : TRUE on success
 */

static void
ImageResampleGetAvgPixel(const RwImage * _image,
                         RwFixed _nXStart,
                         RwFixed _nXEnd,
                         RwFixed _nYStart,
                         RwFixed __nYEnd,
                         RwRGBAReal * _rrpCol)
{
    RwReal              nArea;
    RwReal              nScale;

    RWFUNCTION(RWSTRING("ImageResampleGetAvgPixel"));
    RWASSERT((_image));
    RWASSERT((_rrpCol));

    nArea = RwFixedToReal((__nYEnd) - (_nYStart));

    if (RwFixedToInt((_nYStart)) == RwFixedToInt((__nYEnd)))
    {
        /* All in one pixel row */
        ImageResampleGetSpan((_image), (_nXStart), (_nXEnd), (_nYStart), (_rrpCol));
        nScale = RwFixedToReal((__nYEnd) - (_nYStart));
        RwRGBARealScale((_rrpCol), (_rrpCol), nScale);
    }
    else
    {
        RwRGBAReal          rrAdd;
        RwFixed             nPos = RwIntToFixed(RwFixedToInt((_nYStart)) + 1);

        /* Has a top and bottom and possibly a bit in the middle */
        /* Do top first */
        ImageResampleGetSpan((_image), (_nXStart), (_nXEnd), (_nYStart), (_rrpCol));
        nScale = RwFixedToReal(nPos - (_nYStart));
        RwRGBARealScale((_rrpCol), (_rrpCol), nScale);

        /* Do the middle bit */
        while (RwFixedToInt(nPos) != RwFixedToInt((__nYEnd)))
        {
            ImageResampleGetSpan((_image), (_nXStart), (_nXEnd), nPos, &rrAdd);
            RwRGBARealAdd((_rrpCol), &rrAdd, (_rrpCol));

            nPos += RwIntToFixed(1);
        }

        /* Do the bottom */
        ImageResampleGetSpan((_image), (_nXStart), (_nXEnd), nPos, &rrAdd);
        nScale = RwFixedToReal((__nYEnd) - nPos);
        RwRGBARealScale(&rrAdd, &rrAdd, nScale);
        RwRGBARealAdd((_rrpCol), (_rrpCol), &rrAdd);
    }

    /* Work out the final pixel value */
    nScale = (((RwReal)1) / (nArea));
    RwRGBARealScale((_rrpCol), (_rrpCol), nScale);

    RWRETURNVOID();
}

/**
 * \ingroup rwimage
 * \ref RwImageResample re-samples the specified source image to a
 * different size. The re-sampled size is that specified by the destination
 * image which will receive the new image data. Both the source and
 * destination images must be in 32-bit format. If the destination image is
 * smaller than the source, new pixels in the destination are calculated by
 * averaging groups of pixels in the source image. If the re-sampling results
 * in a larger image, pixel replication is used to determine new pixels in the
 * destination image. Note that the source and destination image pointers must
 * not refer to the same image.
 *
 * \param dstImage  A pointer to the destination image
 * \param srcImage  A pointer to the source image.
 *
 * \return Returns pointer to the destination image if successful or NULL
 * if there is an error.
 *
 * \see RwImageCreateResample
 * \see RwImageCreate
 * \see RwImageCopy
 *
 */
RwImage            *
RwImageResample(RwImage * dstImage, const RwImage * srcImage)
{
    RwInt32             nX, nY;
    RwFixed             nXPos;
    RwFixed             nXDelta;
    RwFixed             nYPos;
    RwFixed             nYDelta;
    RwInt32             dstWidth, dstHeight;
    RwInt32             srcWidth, srcHeight;

    RWAPIFUNCTION(RWSTRING("RwImageResample"));

    RWASSERT(dstImage);
    RWASSERT(srcImage);
    RWASSERT((RwImageGetDepth(srcImage) == 32) &&
             (RwImageGetDepth(dstImage) == 32));

    dstWidth = RwImageGetWidth(dstImage);
    dstHeight = RwImageGetHeight(dstImage);
    srcWidth = RwImageGetWidth(srcImage);
    srcHeight = RwImageGetHeight(srcImage);

    /* Copy the gamma correction flag */
    dstImage->flags |= (srcImage->flags & rwIMAGEGAMMACORRECTED);

    /* Set up X */
    nXDelta = RwRealToFixed((RwReal) srcWidth / (RwReal) dstWidth);

    /* Set up Y */
    nYDelta = RwRealToFixed((RwReal) srcHeight / (RwReal) dstHeight);

    /* Do the re-sampling !! */
    nYPos = RwIntToFixed(0);
    for (nY = 0; nY < dstHeight; nY++)
    {
        RwRGBA    *const rpDstSpan = ((RwRGBA *)((dstImage->stride * nY) +
                                                    dstImage->cpPixels ));

        nXPos = RwIntToFixed(0);
        for (nX = 0; nX < dstWidth; nX++)
        {
            RwRGBAReal          rrCol;

            /* Get pixel value */
            ImageResampleGetAvgPixel(srcImage, nXPos,
                                     nXPos + nXDelta - 1, nYPos,
                                     nYPos + nYDelta - 1, &rrCol);

            /* Write off the pixel */
            RwRGBAFromRwRGBAReal(&rpDstSpan[nX], &rrCol);

            nXPos += nXDelta;
        }

        nYPos += nYDelta;
    }

    /* All done */
    RWRETURN(dstImage);
}

/**
 * \ingroup rwimage
 * \ref RwImageCreateResample re-samples the specified image to a
 * different size according to the width and height input arguments. This
 * function creates a new image of the required size and with a depth of
 * 32-bits per pixel, though the input image can have 8- or 32-bits per pixel.
 * If the re-sampled image is smaller than the original, new pixels are
 * calculated by averaging groups of pixels in the source image. If the
 * re-sampling results in a larger image, pixel replication is used to
 * determine new pixels in the output image. The source image is not
 * altered in any way.
 *
 * \param srcImage  A pointer to the image that will be re-sampled
 * \param width  An \ref RwInt32 value equal to the width of the re-sampled image
 * \param height  An \ref RwInt32 value equal to the height of the re-sampled image.
 *
 * \return Returns a pointer to the re-sampled image if successful or NULL
 * if there is an error.
 *
 * \see RwImageResample
 * \see RwImageCreate
 *
 */
RwImage            *
RwImageCreateResample(const RwImage * srcImage,
                      RwInt32 width,
                      RwInt32 height)
{
    RwImage            *dstImage;

    RWAPIFUNCTION(RWSTRING("RwImageCreateResample"));
    RWASSERT(srcImage);
    RWASSERT(width > 0);
    RWASSERT(height > 0);

    /* Create the output image */
    dstImage = RwImageCreate(width, height, 32);
    if (!dstImage)
    {
        RWRETURN((RwImage *)NULL);
    }

    if (!RwImageAllocatePixels(dstImage))
    {
        RwImageDestroy(dstImage);
        RWRETURN((RwImage *)NULL);
    }

    /* Both images must be 32 bit - if necessary remap the source */
    if (RwImageGetDepth(srcImage) != 32)
    {
        RwImage            *ipUse;

        ipUse =
            RwImageCreate(RwImageGetWidth(srcImage),
                          RwImageGetHeight(srcImage), 32);
        if (!ipUse)
        {
            RwImageFreePixels(dstImage);
            RwImageDestroy(dstImage);
            RWRETURN((RwImage *)NULL);
        }

        if (!RwImageAllocatePixels(ipUse))
        {
            RwImageDestroy(ipUse);
            RwImageFreePixels(dstImage);
            RwImageDestroy(dstImage);
            RWRETURN((RwImage *)NULL);
        }

        /* Change the image depth */
        RwImageCopy(ipUse, srcImage);

        /* Resample the image */
        if (!RwImageResample(dstImage, ipUse))
        {
            RwImageFreePixels(ipUse);
            RwImageDestroy(ipUse);
            RwImageFreePixels(dstImage);
            RwImageDestroy(dstImage);
            RWRETURN((RwImage *)NULL);
        }

        RwImageFreePixels(ipUse);
        RwImageDestroy(ipUse);
    }
    else
    {
        if (!RwImageResample(dstImage, srcImage))
        {
            RwImageFreePixels(dstImage);
            RwImageDestroy(dstImage);
            RWRETURN((RwImage *)NULL);
        }
    }

    /* All done */
    RWRETURN(dstImage);
}

