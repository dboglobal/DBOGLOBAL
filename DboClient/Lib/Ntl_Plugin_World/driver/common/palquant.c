#include <math.h>
#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "barwtyp.h"
#include "baresour.h"
#include "baimage.h"
#include "badebug.h"
#include "palquant.h"

/**
 * \ingroup rwpalquant
 * \page rwpalquantoverview RwPalQuant Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * The API functions in this module can be used to convert a set of
 * input images to a set of palettized images (usually 4 or 8 bit) 
 * that share a single palette. The input images might correspond, 
 * for example, to a set of mipmap levels.
 *
 * This functionality is used internally in certain image to
 * raster conversion processes (e.g. \ref RwTextureRasterGenerateMipmaps).
 *
 * The quantization process involves initializing an \ref RwPalQuant
 * object, accumulating pixel data for a series of images, resolving a
 * palette, then generating the new palettized image data.
 *
 * The following code shows an example of generating a 4-bit palettized
 * image (\c destImage) from a given source image (\c sourceImage).
 *
 * \code
    
    RwImage    *destImage;
    RwPalQuant  pq;

    destImage = RwImageCreate(
        RwImageGetWidth(sourceImage), RwImageGetHeight(sourceImage), 4);
    RwImageAllocatePixels(destImage);

    RwPalQuantInit(&pq);
    RwPalQuantAddImage(&pq, sourceImage, 1.0f);
    RwPalQuantResolvePalette(RwImageGetPalette(destImage), 16, &pq);
    RwPalQuantMatchImage(RwImageGetPixels(destImage), 
        RwImageGetStride(destImage), RwImageGetDepth(destImage), 
        FALSE, &pq, sourceImage);
    RwPalQuantTerm(&pq);

 * \endcode
 *
 * The \ref RwPalQuant structure contains a color quantization tree. This
 * has a maximum depth given by \ref RwPalQuantSetMaxDepth, which can be
 * anything up to 8 (for 32-bit RGBA color depth). The highest values
 * require significantly more memory and processing time. If this value is 
 * modified it should be done before calling \ref RwPalQuantInit. The
 * current value may be queried using \ref RwPalQuantGetMaxDepth.
 *
 */

#if defined (__MWERKS__)
#if (defined(RWVERBOSE))
#pragma message (__FILE__ "/" _SKY_EXPAND(__LINE__) ": __MWERKS__ == " _SKY_EXPAND(__MWERKS__))
#endif /* (defined (__MWERKS__)) */
#if (__option (global_optimizer))
#pragma always_inline off
#endif /* (__option (global_optimizer)) */
#endif /*  defined (__MWERKS__) */

#define RED     0
#define GREEN   1
#define BLUE    2
#define ALPHA   3

#define DEFAULTDEPTH 6
static RwUInt32 QuantDepth = DEFAULTDEPTH;


typedef unsigned long OctantMap;

/* local */
static OctantMap    splice[RWPALQUANT_MAXCOLOR];


/*************************************************************************/
#define RwRGBARealLengthSq(a)                               \
    ((a)->red*(a)->red + (a)->green*(a)->green              \
     + (a)->blue*(a)->blue + (a)->alpha*(a)->alpha)

/*************************************************************************/
/* 0.299 0.587 0.114 RGB ratios are normalized */
static void
ToMatchSpace(RwRGBAReal *col)
{
    RWFUNCTION(RWSTRING("ToMatchSpace"));
    RWASSERT(NULL != col);

    col->red *= 0.299f * (1/0.587f);
    col->green *= 0.587f * (1/0.587f);
    col->blue *= 0.114f * (1/0.587f);
    col->alpha *= 0.587f * (1/0.587f);

    RWRETURNVOID();
}

/*************************************************************************/
static void
FromMatchSpace(RwRGBAReal *ms)
{
    RWFUNCTION(RWSTRING("FromMatchSpace"));
    RWASSERT(NULL != ms);

    ms->red /= 0.299f * (1/0.587f);
    ms->green /= 0.587f * (1/0.587f);
    ms->blue /= 0.114f * (1/0.587f);
    ms->alpha /= 0.587f * (1/0.587f);

    RWRETURNVOID();
}

/*************************************************************************/
static _rwPalQuantLeafNode    *
InitLeaf(_rwPalQuantLeafNode * Leaf)
{
    RWFUNCTION(RWSTRING("InitLeaf"));
    RWASSERT(Leaf);

    Leaf->palIndex = 0;
    Leaf->weight = 0;
    Leaf->ac.red = 0;
    Leaf->ac.green = 0;
    Leaf->ac.blue = 0;
    Leaf->ac.alpha = 0;
    Leaf->var = 0;

    RWRETURN(Leaf);
}

/*************************************************************************/
static void
LeafAddPixel(_rwPalQuantLeafNode *leaf, RwRGBA *color, RwReal weight)
{
    RwRGBAReal  rColor;
    RwRGBA      offset;
    RwUInt8     mask;

    RWFUNCTION(RWSTRING("LeafAddPixel"));
    RWASSERT(leaf);
    RWASSERT(color);

    /* Calc color relative to origin of the quantization volume */
    mask = ((1 << (RWPALQUANT_MAXDEPTH - QuantDepth)) - 1);

    offset.red   = color->red & mask;
    offset.green = color->green & mask;
    offset.blue  = color->blue & mask;
    offset.alpha = color->alpha & mask;

    RwRGBARealFromRwRGBA(&rColor, &offset);
    ToMatchSpace(&rColor);

    /* Accumulate weight, color offset squared, and color offset vector. 
     * These will be converted to weighted variance and absolute color 
     * during the palette resolve stage. We do it this way to minimise
     * precision problems.
     */
    leaf->weight += weight;
    leaf->var += weight * RwRGBARealLengthSq(&rColor);
    RwRGBARealScale(&rColor, &rColor, weight);
    RwRGBARealAdd(&leaf->ac, &leaf->ac, &rColor);

    RWRETURNVOID();
}


/*************************************************************************/
static void
LeafCalcStats(_rwPalQuantLeafNode *leaf, RwRGBA *origin)
{
    RwRGBAReal rColor;

    RWFUNCTION(RWSTRING("LeafCalcStats"));
    RWASSERT(leaf);
    RWASSERT(origin);

    /* Datastructure should now have been filled with all pixel data
     * and we need to convert the leaf node values to the working
     * stats for resolving the palette.
     */

    /* Calculate weighted variance */
    leaf->var -= RwRGBARealLengthSq(&leaf->ac) / leaf->weight;
    if (leaf->var < 0.0f)
    {
        leaf->var = 0.0f;
    }

    /* Store ac as an absolute rather than an offset */
    RwRGBARealFromRwRGBA(&rColor, origin);
    ToMatchSpace(&rColor);
    RwRGBARealScale(&rColor, &rColor, leaf->weight);
    RwRGBARealAdd(&leaf->ac, &leaf->ac, &rColor);

    RWRETURNVOID();
}


/*************************************************************************/
static void
StatsAdd(_rwPalQuantLeafNode *combined, 
         _rwPalQuantLeafNode *set1, 
         _rwPalQuantLeafNode *set2)
{
    /* Combine stats for two sets. set1 == combined allowed. */
    RWFUNCTION(RWSTRING("StatsAdd"));
    RWASSERT(combined);
    RWASSERT(set1);
    RWASSERT(set2);

    /* First update variance which relies on weight and ac */
    combined->var = set1->var + set2->var;
    if (set1->weight > 0 && set2->weight > 0)
    {
        RwRGBAReal c1, c2;
        RwReal  recipWeight1, recipWeight2;

        /* Get difference of average colors of both sets */
        recipWeight1 = 1 / set1->weight;
        recipWeight2 = 1 / set2->weight;
        RwRGBARealScale(&c1, &set1->ac, recipWeight1);
        RwRGBARealScale(&c2, &set2->ac, recipWeight2);
        RwRGBARealSub(&c1, &c1, &c2);

        /* Add weighted value squared to variance */
        combined->var += 
            RwRGBARealLengthSq(&c1) / (recipWeight1 + recipWeight2);
    }

    /* Now accumulate color vector and weight */
    RwRGBARealAdd(&combined->ac, &set1->ac, &set2->ac);
    combined->weight = set1->weight + set2->weight;

    RWRETURNVOID();
}


/*************************************************************************/
static void
StatsSub(_rwPalQuantLeafNode *remainder, 
         _rwPalQuantLeafNode *whole, 
         _rwPalQuantLeafNode *subset)
{
    /* Subtract stats for subset from stats for whole. Effectively, this
     * does reverse of StatsAdd.
     */

    RWFUNCTION(RWSTRING("StatsSub"));
    RWASSERT(remainder);
    RWASSERT(whole);
    RWASSERT(subset);
    RWASSERT(remainder != whole && remainder != subset);

    remainder->weight = whole->weight - subset->weight;
    RwRGBARealSub(&remainder->ac, &whole->ac, &subset->ac);

    remainder->var = whole->var - subset->var;
    if (remainder->weight > 0 && subset->weight > 0)
    {
        RwRGBAReal c1, c2;
        RwReal  recipWeight1, recipWeight2;

        /* Get difference of average colors of both sets */
        recipWeight1 = 1 / subset->weight;
        recipWeight2 = 1 / remainder->weight;
        RwRGBARealScale(&c1, &subset->ac, recipWeight1);
        RwRGBARealScale(&c2, &remainder->ac, recipWeight2);
        RwRGBARealSub(&c1, &c1, &c2);

        /* Subtract weighted value squared from variance */
        remainder->var -= 
            RwRGBARealLengthSq(&c1) / (recipWeight1 + recipWeight2);
    }

    RWRETURNVOID();
}

/*************************************************************************/

/*
 * Note the use of 255.9999f value when generating the scale variable.
 * This is used instead of 255.0f to prevent rounding errors.
 */
static void
RepresentativeColor(RwRGBA *rgb, _rwPalQuantLeafNode *node)
{
    RwRGBAReal rColor;
    RwInt32 rgba[4];

    RWFUNCTION(RWSTRING("RepresentativeColor"));
    RWASSERT(node->weight > 0);

    RwRGBARealScale(&rColor, &node->ac, 1.0f/node->weight);
    FromMatchSpace(&rColor);
    
    rgba[0] = RwInt32FromRealMacro(rColor.red * 255.99f);
    rgba[1] = RwInt32FromRealMacro(rColor.green * 255.99f);
    rgba[2] = RwInt32FromRealMacro(rColor.blue * 255.99f);
    rgba[3] = RwInt32FromRealMacro(rColor.alpha * 255.99f);

    /* clamp and assign */
    rgb->red = 255;
    if (rgba[0] < 255)
    {
        rgb->red = rgba[0];
    }

    rgb->green = 255;
    if (rgba[1] < 255)
    {
        rgb->green = rgba[1];
    }

    rgb->blue = 255;
    if (rgba[2] < 255)
    {
        rgb->blue = rgba[2];
    }

    rgb->alpha = 255;
    if (rgba[3] < 255)
    {
        rgb->alpha = rgba[3];
    }

    RWRETURNVOID();
}

/*************************************************************************/
static _rwPalQuantBranchNode  *
InitBranch(_rwPalQuantBranchNode * Branch)
{
    int i;

    RWFUNCTION(RWSTRING("InitBranch"));
    RWASSERT(Branch);

    for (i = 0; i < 16; i++)
    {
        Branch->dir[i] = (_rwPalQuantOctNode *)NULL;
    }

    RWRETURN(Branch);
}

/*************************************************************************/
static OctantMap
GetOctAdr(RwRGBA * c)
{
    int ColShift = RWPALQUANT_MAXDEPTH - QuantDepth;

    RWFUNCTION(RWSTRING("GetOctAdr"));
    RWASSERT(c);

    RWRETURN((splice[c->red >> ColShift] << 3) | (splice[c->green >> ColShift] << 2) |
             (splice[c->blue >> ColShift] << 1) | (splice[c->alpha >> ColShift] << 0));
}

/*************************************************************************/
static _rwPalQuantOctNode     *
AllocateToLeaf(RwPalQuant * pq, _rwPalQuantOctNode * root, OctantMap Octs, int depth)
{

    RWFUNCTION(RWSTRING("AllocateToLeaf"));
    RWASSERT(pq);
    RWASSERT(root);

    /* return leaf */
    if (depth == 0)
    {
        RWRETURN(root);
    }
    else
    {
        /* populate branch */
        if (!root->Branch.dir[Octs & 15])
        {
            _rwPalQuantOctNode            *node;

            node = (_rwPalQuantOctNode *) RwFreeListAlloc(pq->cubefreelist,
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
            root->Branch.dir[Octs & 15] = node;
            InitBranch(&node->Branch);
            if (depth == 1)
            {
                InitLeaf(&node->Leaf);
            }
        }

        RWRETURN(AllocateToLeaf(pq, root->Branch.dir[Octs & 15], Octs >> 4, depth - 1));
    }
}


/**
 * \ingroup rwpalquant
 * \ref RwPalQuantSetMaxDepth sets the maximum depth of the tree used for
 * palette quantization. The deeper the tree, the more memory and time will
 * be used by the quantization process, but the better the results, particularly
 * on images with subtle color gradients. This function should be used before
 * initializing an \ref RwPalQuant object.
 *
 * \param depth     Maximum depth of tree to be used for quantization. Values 
 *                  should be less than or equal to 8.
 *
 * \see RwTextureRasterGenerateMipmaps
 * \see RwPalQuantGetMaxDepth
 */
void
RwPalQuantSetMaxDepth(RwUInt32 depth)
{
    RWAPIFUNCTION(RWSTRING("RwPalQuantSetMaxDepth"));

    RWASSERT((depth > 0) && (depth <= 8));

    QuantDepth = (RwUInt8)depth;

    RWRETURNVOID();
}

/**
 * \ingroup rwpalquant
 * \ref RwPalQuantGetMaxDepth returns the maximum depth of the tree used for
 * palette quantization.
 *
 * \return Current maximum depth of tree used for quantization
 *
 * \see RwTextureRasterGenerateMipmaps
 * \see RwPalQuantSetMaxDepth
 *
 */
RwUInt32
RwPalQuantGetMaxDepth(void)
{
    RWAPIFUNCTION(RWSTRING("RwPalQuantGetMaxDepth"));

    RWRETURN((RwUInt32)QuantDepth);
}

/*************************************************************************/
/**
 * \ingroup rwpalquant
 * \ref RwPalQuantAddImage adds each pixel in the image to the quantization
 * tree population.
 *
 * \param pq        Palquant structure holding the quantization tree. This
 *                  must have been initialized with \ref RwPalQuantInit.
 * \param img       Image to add
 * \param weight    Bias to use for these pixels. 
 *
 * For a fixed weight value (e.g. '1'), the total weight of each image
 * in a series of mipmap levels will naturally decrease due to each
 * level having 4 times less pixels than the previous one. The \p weight
 * value can be used to adjust the contribution of the higher levels.
 *
 * \see RwPalQuantInit
 * \see RwPalQuantResolvePalette
 * \see RwPalQuantMatchImage
 */
void
RwPalQuantAddImage(RwPalQuant * pq, RwImage * img, RwReal weight)
{
    RwInt32             ColShift;
    RwInt32             width, height, stride;
    RwUInt8            *pixels;
    RwRGBA             *palette;

    RWAPIFUNCTION(RWSTRING("RwPalQuantAddImage"));
    RWASSERT(pq);
    RWASSERT(img);

    ColShift = RWPALQUANT_MAXDEPTH - QuantDepth;

#if(defined(RWDEBUG))
    /* Are developers generating palettes with a reduced tree
     * quantization depth? Is so, warn the developer that they
     * are not getting the best visual results. */
    if(QuantDepth < RWPALQUANT_MAXDEPTH)
    {
        RWMESSAGE((RWSTRING("Palettising image with only %i bits of color precision. "
                            "Use RwPalQuantSetMaxDepth to improve color quality."),QuantDepth));
    }
    else
    {
        RWMESSAGE((RWSTRING("Palettising image for maximum color quality.")));
    }
#endif /* (defined(RWDEBUG)) */

    stride = RwImageGetStride(img);
    pixels = RwImageGetPixels(img);
    palette = RwImageGetPalette(img);
    height = RwImageGetHeight(img);

    switch (RwImageGetDepth(img))
    {
        case (4):
        case (8):
            {
                while (height--)
                {
                    RwUInt8 *linePixels = pixels;

                    width = RwImageGetWidth(img);
                    while (width--)
                    {
                        RwRGBA             *color = &palette[*linePixels];
                        _rwPalQuantOctNode            *leaf;
                        OctantMap           Octs;

                        /* build down to leaf */
                        Octs = GetOctAdr(color);
                        leaf = AllocateToLeaf(pq, pq->root, Octs, QuantDepth);
                        LeafAddPixel(&leaf->Leaf, color, weight);

                        linePixels++;
                    }

                    pixels += stride;
                }
                break;
            }

        case (32):
            {
                while (height--)
                {
                    RwRGBA *color = (RwRGBA *) pixels;

                    width = RwImageGetWidth(img);
                    while (width--)
                    {
                        _rwPalQuantOctNode  *leaf;
                        OctantMap           Octs;

                        /* build down to leaf */
                        Octs = GetOctAdr(color);
                        leaf = AllocateToLeaf(pq, pq->root, Octs, QuantDepth);
                        LeafAddPixel(&leaf->Leaf, color, weight);

                        color++;
                    }

                    pixels += stride;
                }
                break;
            }
    }

    RWRETURNVOID();
}

/*************************************************************************/

/*************************************************************************/
static void
assignindex(_rwPalQuantOctNode *root, RwInt32 *origin, int depth, _rwPalQuantRGBABox *region, RwInt32 palIndex)
{
    _rwPalQuantRGBABox testbox;
    int i,dR,dG,dB,dA;

    RWFUNCTION(RWSTRING("assignindex"));
    RWASSERT(origin);
    RWASSERT(region);

    if (!root)
        RWRETURNVOID();

    testbox.col0[RED] = origin[RED];
    testbox.col0[GREEN] = origin[GREEN];
    testbox.col0[BLUE] = origin[BLUE];
    testbox.col0[ALPHA] = origin[ALPHA];
    testbox.col1[RED] = origin[RED] + (1 << depth);
    testbox.col1[GREEN] = origin[GREEN] + (1 << depth);
    testbox.col1[BLUE] = origin[BLUE] + (1 << depth);
    testbox.col1[ALPHA] = origin[ALPHA] + (1 << depth);

    /* stop if testbox disjoint from region */
    dR = testbox.col1[RED] - region->col0[RED];
    dG = testbox.col1[GREEN] - region->col0[GREEN];
    dB = testbox.col1[BLUE] - region->col0[BLUE];
    dA = testbox.col1[ALPHA] - region->col0[ALPHA];
    if (dR <= 0 || dG <= 0 || dB <= 0 || dA <= 0)
    {
        RWRETURNVOID();
    }
    dR = testbox.col0[RED] - region->col1[RED];
    dG = testbox.col0[GREEN] - region->col1[GREEN];
    dB = testbox.col0[BLUE] - region->col1[BLUE];
    dA = testbox.col0[ALPHA] - region->col1[ALPHA];
    if (dR >= 0 || dG >= 0 || dB >= 0 || dA >= 0)
    {
        RWRETURNVOID();
    }

    if (depth == 0)
    {
        root->Leaf.palIndex = (RwUInt8) palIndex;
    }
    else
    {
        depth--;
        for (i = 0; i < 16; i++)
        {
            RwInt32 suborigin[4];

            suborigin[RED] = origin[RED] + (((i >> 3) & 1) << depth);
            suborigin[GREEN] = origin[GREEN] + (((i >> 2) & 1) << depth);
            suborigin[BLUE] = origin[BLUE] + (((i >> 1) & 1) << depth);
            suborigin[ALPHA] = origin[ALPHA] + (((i >> 0) & 1) << depth);

            assignindex(root->Branch.dir[i], suborigin, depth, region, palIndex);
        }
    }

    RWRETURNVOID();
}

/*************************************************************************/

/* Assign palIndex to leaves */
static void
AssignPalIndex(_rwPalQuantOctNode * root, _rwPalQuantRGBABox * cube, RwInt32 palIndex)
{
    RwInt32 origin[4];

    RWFUNCTION(RWSTRING("AssignPalIndex"));
    RWASSERT(root);
    RWASSERT(cube);

    origin[RED] = 0;
    origin[GREEN] = 0;
    origin[BLUE] = 0;
    origin[ALPHA] = 0;
    assignindex(root, origin, QuantDepth, cube, palIndex);
    RWRETURNVOID();
}

/*************************************************************************/
static void
addvolume(_rwPalQuantOctNode *root, RwInt32 *origin, RwInt32 depth, _rwPalQuantRGBABox *region, _rwPalQuantLeafNode *volume)
{
    _rwPalQuantRGBABox testbox;
    RwInt32 i,dR,dG,dB,dA;

    RWFUNCTION(RWSTRING("addvolume"));
    RWASSERT(origin);
    RWASSERT(region);

    if (!root)
        RWRETURNVOID();

    testbox.col0[RED] = origin[RED];
    testbox.col0[GREEN] = origin[GREEN];
    testbox.col0[BLUE] = origin[BLUE];
    testbox.col0[ALPHA] = origin[ALPHA];
    testbox.col1[RED] = origin[RED] + (1 << depth);
    testbox.col1[GREEN] = origin[GREEN] + (1 << depth);
    testbox.col1[BLUE] = origin[BLUE]+ (1 << depth);
    testbox.col1[ALPHA] = origin[ALPHA] + (1 << depth);

    /* stop if testbox disjoint from region */
    dR = testbox.col1[RED] - region->col0[RED];
    dG = testbox.col1[GREEN] - region->col0[GREEN];
    dB = testbox.col1[BLUE] - region->col0[BLUE];
    dA = testbox.col1[ALPHA] - region->col0[ALPHA];
    if (dR <= 0 || dG <= 0 || dB <= 0 || dA <= 0)
    {
        RWRETURNVOID();
    }
    dR = testbox.col0[RED] - region->col1[RED];
    dG = testbox.col0[GREEN] - region->col1[GREEN];
    dB = testbox.col0[BLUE] - region->col1[BLUE];
    dA = testbox.col0[ALPHA] - region->col1[ALPHA];
    if (dR >= 0 || dG >= 0 || dB >= 0 || dA >= 0)
    {
        RWRETURNVOID();
    }

    /* testbox wholly within region? */
    dR = testbox.col0[RED] - region->col0[RED];
    dG = testbox.col0[GREEN] - region->col0[GREEN];
    dB = testbox.col0[BLUE] - region->col0[BLUE];
    dA = testbox.col0[ALPHA] - region->col0[ALPHA];
    if (dR >= 0 && dG >= 0 && dB >= 0 && dA >= 0)
    {
        dR = region->col1[RED] - testbox.col1[RED];
        dG = region->col1[GREEN] - testbox.col1[GREEN];
        dB = region->col1[BLUE] - testbox.col1[BLUE];
        dA = region->col1[ALPHA] - testbox.col1[ALPHA];
        if (dR >= 0 && dG >= 0 && dB >= 0 && dA >= 0)
        {
            StatsAdd(volume, volume, &root->Leaf);
            RWRETURNVOID();
        }
    }

    if (depth > 0)
    {
        depth--;
        for (i = 0; i < 16; i++)
        {
            RwInt32 suborigin[4];

            suborigin[RED] = origin[RED] + (((i >> 3) & 1) << depth);
            suborigin[GREEN] = origin[GREEN] + (((i >> 2) & 1) << depth);
            suborigin[BLUE] = origin[BLUE] + (((i >> 1) & 1) << depth);
            suborigin[ALPHA] = origin[ALPHA] + (((i >> 0) & 1) << depth);

            addvolume(root->Branch.dir[i], suborigin, depth, region, volume);
        }
    }

    RWRETURNVOID();
}

/*************************************************************************/

/* Compute accumulated statistics over a box */
static _rwPalQuantLeafNode *
BoxStats(_rwPalQuantLeafNode *Vol, _rwPalQuantOctNode *root, _rwPalQuantRGBABox *cube)
{
    RwInt32 origin[4];

    RWFUNCTION(RWSTRING("BoxStats"));
    RWASSERT(root);
    RWASSERT(cube);

    origin[RED] = 0;
    origin[GREEN] = 0;
    origin[BLUE] = 0;
    origin[ALPHA] = 0;
    InitLeaf(Vol);
    addvolume(root, origin, QuantDepth, cube, Vol);

    RWRETURN(Vol);
}

/*************************************************************************/

/* We want to minimize the sum of the variances of two subboxes.
 */
static RwReal
FindBestCut(_rwPalQuantOctNode *root, _rwPalQuantRGBABox *cube, RwInt32 chn, RwInt32 *cuts, _rwPalQuantLeafNode *whole)
{
    _rwPalQuantRGBABox      leftcube;
    _rwPalQuantLeafNode     left, right;
    RwReal                  minsum, sumvar;
    RwInt32                 i;

    RWFUNCTION(RWSTRING("FindBestCut"));
    RWASSERT(root);
    RWASSERT(cube);
    RWASSERT(cuts);
    RWASSERT(whole);

    minsum = whole->var;
    leftcube = *cube;

#if 1

    /* binary chop  - NB this appears to not converge on the same result.
     * (Strictly, the minimum of the variance sum does not necessarily
     * occur at the point where the two variances are equal, but it should
     * be pretty close)
     */
    {
        RwInt32 min,max;

        min = cube->col0[chn];
        max = cube->col1[chn];
        while (max - min > 1)
        {
            i = min + ((max - min) >> 1);
            leftcube.col1[chn] = i;

            BoxStats(&left, root, &leftcube);
            StatsSub(&right, whole, &left);

            if (left.weight == 0)
            {
               min = i;
            }
            else
            if (right.weight == 0)
            {
               max = i;
            }
            else
            {
                sumvar = left.var + right.var;
                if (sumvar < minsum)
                {
                    minsum = sumvar;
                    cuts[chn] = leftcube.col1[chn];
                }

                if (left.var < right.var)
                {
                    min = i;
                }
                else
                {
                    max = i;
                }
            }
        }
    }
#else
    {
        RwReal lastsum;

        /* exhaustive search */
        lastsum = minsum;
        for (i = cube->col0[chn]+1; i < cube->col1[chn]; i++)
        {
            leftcube.col1[chn] = i;

            BoxStats(&left, root, &leftcube);
            StatsSub(&right, whole, &left);

            if ((left.weight > 0.0f) && (right.weight > 0.0f))
            {
                sumvar = left.var + right.var;

                if (sumvar < minsum)
                {
                    minsum = sumvar;
                    cuts[chn] = i;
                }
                else 
                if (sumvar > lastsum)
                {
                    /* we've past the minimum*/
                    break;
                }

                lastsum = sumvar;
            }
        }
    }
#endif

    RWRETURN(minsum);
}

/*************************************************************************/
static RwBool
nCut(_rwPalQuantOctNode *root, _rwPalQuantLeafNode *whole, _rwPalQuantRGBABox *set1, _rwPalQuantRGBABox *set2)
{
    RwReal minvar[4];
    RwInt32 cuts[4],i,best;

    RWFUNCTION(RWSTRING("nCut"));
    RWASSERT(whole->weight > 0);
    RWASSERT(set1);
    RWASSERT(set2);

    minvar[RED] = FindBestCut(root, set1, RED, cuts, whole);
    minvar[GREEN] = FindBestCut(root, set1, GREEN, cuts, whole);
    minvar[BLUE] = FindBestCut(root, set1, BLUE, cuts, whole);
    minvar[ALPHA] = FindBestCut(root, set1, ALPHA, cuts, whole);

    best = 0;
    for (i=0; i<4; i++)
    {
        if (minvar[i] < minvar[best])
        {
            best = i;
        }            
    }

    /* did we find any splits? */
    if (minvar[best] < whole->var)
    {
        *set2 = *set1;
        set1->col1[best] = set2->col0[best] = (RwUInt8)cuts[best];

        RWRETURN(TRUE);
    }

    RWRETURN(FALSE);
}

/*************************************************************************/

static _rwPalQuantLeafNode    *
CalcNodeWeights(_rwPalQuantOctNode *root, RwRGBA *origin, RwInt32 depth)
{
    _rwPalQuantLeafNode           *Leaf;
    int                 i;

    RWFUNCTION(RWSTRING("CalcNodeWeights"));

    Leaf = NULL;
    if (root)
    {
        Leaf = &root->Leaf;

        /* is it a branch? */
        if (depth > 0)
        {
            InitLeaf(Leaf);
            for (i = 0; i < 16; i++)
            {   
                RwRGBA suborigin;
                 _rwPalQuantLeafNode *SubNode;
                RwUInt32 shift = depth - 1 + (RWPALQUANT_MAXDEPTH - QuantDepth);

                suborigin.red   = origin->red   + (((i >> 3) & 1) << shift);
                suborigin.green = origin->green + (((i >> 2) & 1) << shift);
                suborigin.blue  = origin->blue  + (((i >> 1) & 1) << shift);
                suborigin.alpha = origin->alpha + (((i >> 0) & 1) << shift);

                SubNode = CalcNodeWeights(root->Branch.dir[i], &suborigin, depth - 1);
                if (SubNode)
                {
                    StatsAdd(Leaf, Leaf, SubNode);
                }
            }
        }
        else
        {
            /* Generate stats for a leaf */
            LeafCalcStats(Leaf, origin);
        }
    }

    RWRETURN(Leaf);
}

/*************************************************************************/
static int
CountLeafs(_rwPalQuantOctNode *root, RwInt32 depth)
{
    RwInt32 i, n;

    RWFUNCTION(RWSTRING("CountLeafs"));

    n = 0;
    if (root)
    {
        /* is it a branch? */
        if (depth > 0)
        {
            for (i = 0; i < 16; i++)
            {
                n += CountLeafs(root->Branch.dir[i], depth - 1);
            }
        }
        else
        {
            n = 1;
        }
    }

    RWRETURN(n);
}

/*************************************************************************/
static int
CountNodes(_rwPalQuantOctNode *root, RwInt32 depth)
{
    RwInt32 i, n;

    RWFUNCTION(RWSTRING("CountNodes"));

    n = 0;
    if (root)
    {
        n = 1;

        /* is it a branch? */
        if (depth > 0)
        {
            for (i = 0; i < 16; i++)
            {
                n += CountNodes(root->Branch.dir[i], depth - 1);
            }
        }
    }

    RWRETURN(n);
}

/*************************************************************************/
static int
ExtractNodes(_rwPalQuantOctNode *root, RwRGBA *palette, RwInt32 nodeIndex, RwInt32 depth)
{
    RWFUNCTION(RWSTRING("ExtractNodes"));

    if (root)
    {
        /* is it a branch? */
        if (depth > 0)
        {
            RwInt32 i;

            for (i = 0; i < 16; i++)
            {
                nodeIndex = ExtractNodes(root->Branch.dir[i], palette, nodeIndex, depth - 1);
            }
        }
        else
        {
            RepresentativeColor(&palette[nodeIndex], &root->Leaf);

            root->Leaf.palIndex = (RwUInt8)nodeIndex;
            nodeIndex++;
        }
    }

    RWRETURN(nodeIndex);
}

/*************************************************************************/
static              RwUInt8
GetIndex(_rwPalQuantOctNode *root, OctantMap Octs, RwInt32 depth)
{
    RwUInt8             result;

    RWFUNCTION(RWSTRING("GetIndex"));
    RWASSERT(root);

    if (depth == 0)
    {
        result = root->Leaf.palIndex;
    }
    else
    {
        result = GetIndex(root->Branch.dir[Octs & 15], Octs >> 4, depth - 1);
    }

    RWRETURN(result);
}

/*************************************************************************/
/**
 * \ingroup rwpalquant
 * \ref RwPalQuantResolvePalette uses the quantization tree in an \ref
 * RwPalQuant object to find a set of representative colors
 * and store them in the specified palette.
 *
 * \param palette   Palette in which to store results of quantization.
 * \param maxcols   Maximum number of palette entries to use.
 * \param pq        \ref RwPalQuant structure holding the quantization tree.
 *
 * \return The number of palette entries used.
 *
 * \ref RwPalQuantMatchImage
 *
 */
RwInt32
RwPalQuantResolvePalette(RwRGBA *palette, RwInt32 maxcols, RwPalQuant *pq)
{
    RwInt32             numcols, uniquecols, i;
    RwRGBA              origin = {0,0,0,0};

    RWAPIFUNCTION(RWSTRING("RwPalQuantResolvePalette"));
    RWASSERT(palette);
    RWASSERT(maxcols <= RWPALQUANT_MAXCOLOR);
    RWASSERT(pq);

    /* cache weightings at every node */
    CalcNodeWeights(pq->root, &origin, QuantDepth);

    /* clear down palette */
    for (i=0; i<maxcols; i++)
    {
        palette[i].red = 0;
        palette[i].green = 0;
        palette[i].blue = 0;
        palette[i].alpha = 0;
    }

    numcols = maxcols;
    uniquecols = CountLeafs(pq->root, QuantDepth);
    if (uniquecols <= numcols)
    {
        numcols = uniquecols;
        ExtractNodes(pq->root, palette, 0, QuantDepth);
    }
    else
    {
        /* divide and conquer */
        pq->Mcube[0].col0[RED] = 0;
        pq->Mcube[0].col0[GREEN] = 0;
        pq->Mcube[0].col0[BLUE] = 0;
        pq->Mcube[0].col0[ALPHA] = 0;
        pq->Mcube[0].col1[RED] = 1 << QuantDepth;
        pq->Mcube[0].col1[GREEN] = 1 << QuantDepth;
        pq->Mcube[0].col1[BLUE] = 1 << QuantDepth;
        pq->Mcube[0].col1[ALPHA] = 1 << QuantDepth;
        BoxStats(&pq->Mvol[0], pq->root, &pq->Mcube[0]);
        pq->Mvv[0] = pq->Mvol[0].var;
        for (i = 1; i < numcols; i++)
        {
            RwInt32 nextsplit,k;
            RwReal maxvar;

            /* find best box to split */
            nextsplit = -1;
            maxvar = 0.0f;
            for (k = 0; k < i; k++)
            {
                if (pq->Mvv[k] > maxvar)
                {
                    maxvar = pq->Mvv[k];
                    nextsplit = k;
                }
            }

            /* stop if we couldn't find a box to split */
            if (nextsplit == -1)
            {
                break;
            }

            /* split _rwPalQuantRGBABox */
            if (nCut(pq->root, &pq->Mvol[nextsplit], &pq->Mcube[nextsplit], &pq->Mcube[i]))
            {
                /* volume test ensures we won't try to cut one-cell _rwPalQuantRGBABox */
                BoxStats(&pq->Mvol[nextsplit], pq->root, &pq->Mcube[nextsplit]);
                BoxStats(&pq->Mvol[i], pq->root, &pq->Mcube[i]);
                pq->Mvv[nextsplit] = pq->Mvol[nextsplit].var;
                pq->Mvv[i] = pq->Mvol[i].var;
            }
            else
            {
                /* don't try to split this box again */
                pq->Mvv[nextsplit] = 0.0f;
                i--;
            }
        }

        /* extract the new palette */
        for (i = 0; i < numcols; i++)
        {
            AssignPalIndex(pq->root, &pq->Mcube[i], i);
            RepresentativeColor(&palette[i], &pq->Mvol[i]);
        }
    }

    RWRETURN(numcols);
}

/*************************************************************************/
/**
 * \ingroup rwpalquant
 * \ref RwPalQuantMatchImage matches an image to the given quantization
 * tree, and writes new palette index image data. This should be called 
 * after using \ref RwPalQuantResolvePalette.
 *
 * \param dstpixels     Array of pixels in destination image. When writing
 *                      to an \ref RwImage, use \ref RwImageGetPixels.
 * \param dststride     Stride of pixel rows in destination image. For
 *                      an \ref RwImage, use \ref RwImageGetStride.
 * \param dstdepth      Bit depth of pixels in destination image. For an
 *                      \ref RwImage, use \ref RwImageGetDepth.
 * \param dstPacked     Specifies that the destination pixels should be 
 *                      packed together (2 pixels per byte using 
 *                      4-bit palette indices). This should always be
 *                      FALSE when writing to an \ref RwImage.
 * \param pq            \ref RwPalQuant structure containing the 
 *                      quantization tree to use.
 * \param img           Source image to match using the quantization tree.
 *
 *
 * \see RwPalQuantResolvePalette
 * \see RwPalQuantTerm
 *
 */
void
RwPalQuantMatchImage(RwUInt8 *dstpixels, RwInt32 dststride, RwInt32 dstdepth,
                     RwBool dstPacked, RwPalQuant *pq, RwImage *img)
{
    RwUInt32            width, x, height, stride, maxcol;
    RwUInt8            *pixels, *dstLinePixels;
    RwUInt8             nodeIndex;
    OctantMap           Octs;

    RWAPIFUNCTION(RWSTRING("RwPalQuantMatchImage"));
    RWASSERT(dstpixels);
    RWASSERT(pq);
    RWASSERT(img);

    stride = RwImageGetStride(img);
    pixels = RwImageGetPixels(img);
    maxcol = 1 << dstdepth;

    /* store two pixels per byte if the destination is packed
     * (i.e is a raster not an image) */
    if ((dstdepth == 4) && (dstPacked == TRUE))
    {
        switch (RwImageGetDepth(img))
        {
        case 4:
        case 8:
            {
                RwRGBA             *palette = RwImageGetPalette(img);
                height = RwImageGetHeight(img);
                while (height--)
                {
                    RwUInt8            *srcLinePixels = pixels;

                    dstLinePixels = dstpixels;

                    width = RwImageGetWidth(img);
                    for (x = 0; x < width; x++)
                    {
                        RwRGBA *color = &palette[*srcLinePixels++];

                        /* map to new index */
                        Octs = GetOctAdr(color);
                        nodeIndex = GetIndex(pq->root, Octs, QuantDepth);
                        RWASSERT(nodeIndex < maxcol);

                        if ((x) & 1)
                        {
                           *dstLinePixels &= 0x0F;
                           *dstLinePixels |= (nodeIndex & 0x0F) << 4;
                            dstLinePixels++;
                        }
                        else
                        {
                           *dstLinePixels &= 0xF0;
                           *dstLinePixels |= nodeIndex & 0x0F;
                        }
                    }

                    pixels += stride;
                    dstpixels += dststride;
                }
                break;
            }
        case 32:
            {
                height = RwImageGetHeight(img);
                while (height--)
                {
                    RwRGBA             *srcLinePixels = (RwRGBA *) pixels;

                    dstLinePixels = dstpixels;

                    width = RwImageGetWidth(img);
                    for (x = 0; x < width; x++)
                    {
                        RwRGBA *color = srcLinePixels++;

                        /* map to new index */
                        Octs = GetOctAdr(color);
                        nodeIndex = GetIndex(pq->root, Octs, QuantDepth);
                        RWASSERT(nodeIndex < maxcol);

                        if ((x) & 1)
                        {
                           *dstLinePixels &= 0x0F;
                           *dstLinePixels |= (nodeIndex & 0x0F) << 4;
                            dstLinePixels++;
                        }
                        else
                        {
                           *dstLinePixels &= 0xF0;
                           *dstLinePixels |= nodeIndex & 0x0F;
                        }
                    }

                    pixels += stride;
                    dstpixels += dststride;
                }
                break;
            }
        }
    }
    /* store one pixel per byte */
    else
    {
        switch (RwImageGetDepth(img))
        {
        case 4:
        case 8:
            {
                RwRGBA             *palette = RwImageGetPalette(img);

                height = RwImageGetHeight(img);
                while (height--)
                {
                    RwUInt8            *srcLinePixels = pixels;

                    dstLinePixels = dstpixels;

                    width = RwImageGetWidth(img);
                    while (width--)
                    {
                        RwRGBA *color = &palette[*srcLinePixels++];

                        /* map to new index */
                        Octs = GetOctAdr(color);
                        nodeIndex = GetIndex(pq->root, Octs, QuantDepth);
                        RWASSERT(nodeIndex < maxcol);
                        *dstLinePixels++ = nodeIndex;
                    }

                    pixels += stride;
                    dstpixels += dststride;
                }
                break;
            }
        case 32:
            {
                height = RwImageGetHeight(img);
                while (height--)
                {
                    RwRGBA             *srcLinePixels = (RwRGBA *) pixels;

                    dstLinePixels = dstpixels;

                    width = RwImageGetWidth(img);
                    while (width--)
                    {
                        RwRGBA             *color = srcLinePixels++;

                        /* map to new index */
                        Octs = GetOctAdr(color);
                        nodeIndex = GetIndex(pq->root, Octs, QuantDepth);
                        RWASSERT(nodeIndex < maxcol);
                        *dstLinePixels++ = nodeIndex;
                    }

                    pixels += stride;
                    dstpixels += dststride;
                }
                break;
            }
        }
    }

    RWRETURNVOID();
}

/*************************************************************************/
/**
 * \ingroup rwpalquant
 * \ref RwPalQuantInit initializes a palette quantization tree structure.
 * This will use the current maximum tree depth set by 
 * \ref RwPalQuantSetMaxDepth. This should not be modified while
 * the \ref RwPalQuant structure is in use.
 *
 * \param pq    Palette quantization structure to be initialized.
 *
 * \return TRUE on success, FALSE on failure
 *
 * \see RwPalQuantTerm
 *
 */
RwBool
RwPalQuantInit(RwPalQuant *pq)
{
    RwUInt32 i, j, maxval;

    RWAPIFUNCTION(RWSTRING("RwPalQuantInit"));
    RWASSERT(pq);

    /* lookup mapping (8) bit-patterns to every 4th bit b31->b00 (least to most) */
    maxval = 1 << QuantDepth;
    for (i = 0; i < maxval; i++)
    {
        OctantMap mask = 0;

        for (j = 0; j < QuantDepth; j++)
        {
            mask |= (i & (1 << j)) ? (1 << ((QuantDepth - 1 - j) * 4)) : 0;
        }
        splice[i] = mask;
    }

    pq->cubefreelist = RwFreeListCreate(sizeof(_rwPalQuantOctNode), 1024,
                                        sizeof(RwUInt32),
                                        rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
    pq->root = (_rwPalQuantOctNode *) RwFreeListAlloc(pq->cubefreelist,
        rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
    InitBranch(&pq->root->Branch);
    RWRETURN(TRUE);
}

/*************************************************************************/
static void
DeleteOctTree(RwPalQuant *pq, _rwPalQuantOctNode *root, int depth)
{
    int                 i;

    RWFUNCTION(RWSTRING("DeleteOctTree"));
    RWASSERT(pq);

    if (root)
    {
        /* is it a branch? */
        if (depth > 0)
        {
            for (i = 0; i < 16; i++)
            {
                DeleteOctTree(pq, root->Branch.dir[i], depth - 1);
            }
        }

        RwFreeListFree(pq->cubefreelist, root);
    }

    RWRETURNVOID();
}

/*************************************************************************/
/**
 * \ingroup rwpalquant
 * \ref RwPalQuantTerm deinitializes a palette quantization tree structure
 *
 * \param pq  Palette quantization structure to be deinitialized.
 *
 * \see RwPalQuantInit
 *
 */
void
RwPalQuantTerm(RwPalQuant *pq)
{
    RWAPIFUNCTION(RWSTRING("RwPalQuantTerm"));
    RWASSERT(pq);

    DeleteOctTree(pq, pq->root, QuantDepth);
    pq->root = (_rwPalQuantOctNode *)NULL;
    RwFreeListDestroy(pq->cubefreelist);
    RWRETURNVOID();
}
