/***************************************************************************
 *                                                                         *
 * Module  : D3D8dxttex.c                                                  *
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
#include "babinary.h"
#include "baimras.h"
#include "baresamp.h"

#include "osintf.h"

#include "d3d8device.h"
#include "d3d8raster.h"
#include "d3d8convrt.h"

#include "dds.h"

#include "d3d8dxttex.h"

/****************************************************************************
 Defines
 */
#define rwSEARCHPATH_SEPARATOR ';' /* Borrowed from baimage.c */

#define IGNORED_D3D8    0

/****************************************************************************
 Globals (across program)
 */
RwBool RwFexist( const RwChar * name );

/****************************************************************************
 Local (static) types
 */

typedef struct DXTColBlock DXTColBlock;
struct DXTColBlock
{
    RwUInt16 col0;
    RwUInt16 col1;

    /* no bit fields - use bytes*/
    RwUInt8 row[4];
};

typedef struct DXTAlphaBlockExplicit DXTAlphaBlockExplicit;
struct DXTAlphaBlockExplicit
{
    RwUInt16 row[4];
};

typedef struct DXTAlphaBlock3BitLinear DXTAlphaBlock3BitLinear;
struct DXTAlphaBlock3BitLinear
{
    RwUInt8 alpha0;
    RwUInt8 alpha1;

    RwUInt8 stuff[6];
};

/* use cast to struct instead of RGBA_MAKE as struct is much */
typedef struct Color8888 Color8888;
struct Color8888
{
    RwUInt8 r;      /* change the order of names to change the */
    RwUInt8 g;      /*  order of the output ARGB or BGRA, etc...*/
    RwUInt8 b;      /*  Last one is MSB, 1st is LSB.*/
    RwUInt8 a;
};

typedef struct Color565 Color565;
struct Color565
{
    unsigned nBlue  : 5;        /* order of names changes*/
    unsigned nGreen : 6;        /*  byte order of output to 32 bit */
    unsigned nRed   : 5;
};

/****************************************************************************
 Local (static) variables
 */

/****************************************************************************
 GetColorBlockColorsDXT1

 On entry   :
 On exit    :   TRUE if the block has a transparent color
 */
static __inline RwBool
GetColorBlockColorsDXT1(const DXTColBlock *pBlock,
                        Color8888 *col_0, Color8888 *col_1,
                        Color8888 *col_2, Color8888 *col_3,
                        RwUInt16 *wrd)
{
    RwBool  hasAlpha = FALSE;

    /* There are 4 methods to use - see the Time_ functions. */
    /* 1st = shift = does normal approach per byte for color comps */
    /* 2nd = use freak variable bit field color565 for component extraction */
    /* 3rd = use super-freak RwUInt32 adds BEFORE shifting the color components */
    /*  This lets you do only 1 add per color instead of 3 RwUInt8 adds and */
    /*  might be faster */
    /* Call RunTimingSession() to run each of them & output result to txt file */

    /* freak variable bit structure method */
    /* normal math */
    /* This method is fastest */
    const Color565 *pCol;

    RWFUNCTION(RWSTRING("GetColorBlockColorsDXT1"));

    pCol = (const Color565 *) & (pBlock->col0 );

    col_0->a = 0xff;
    col_0->r = pCol->nRed;
    col_0->r <<= 3;             /* shift to full precision */
    col_0->g = pCol->nGreen;
    col_0->g <<= 2;
    col_0->b = pCol->nBlue;
    col_0->b <<= 3;

    pCol = (const Color565 *) & (pBlock->col1 );
    col_1->a = 0xff;
    col_1->r = pCol->nRed;
    col_1->r <<= 3;             /* shift to full precision */
    col_1->g = pCol->nGreen;
    col_1->g <<= 2;
    col_1->b = pCol->nBlue;
    col_1->b <<= 3;


    if( pBlock->col0 > pBlock->col1 )
    {
        /* Four-color block: derive the other two colors.     */
        /* 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3 */
        /* These two bit codes correspond to the 2-bit fields  */
        /* stored in the 64-bit block. */

        *wrd = ((RwUInt16)col_0->r * 2 + (RwUInt16)col_1->r )/3;
                                            /* no +1 for rounding */
                                            /* as bits have been shifted to 888 */
        col_2->r = (RwUInt8)*wrd;

        *wrd = ((RwUInt16)col_0->g * 2 + (RwUInt16)col_1->g )/3;
        col_2->g = (RwUInt8)*wrd;

        *wrd = ((RwUInt16)col_0->b * 2 + (RwUInt16)col_1->b )/3;
        col_2->b = (RwUInt8)*wrd;
        col_2->a = 0xff;

        *wrd = ((RwUInt16)col_0->r + (RwUInt16)col_1->r *2 )/3;
        col_3->r = (RwUInt8)*wrd;

        *wrd = ((RwUInt16)col_0->g + (RwUInt16)col_1->g *2 )/3;
        col_3->g = (RwUInt8)*wrd;

        *wrd = ((RwUInt16)col_0->b + (RwUInt16)col_1->b *2 )/3;
        col_3->b = (RwUInt8)*wrd;
        col_3->a = 0xff;
    }
    else
    {
        /* Three-color block: derive the other color. */
        /* 00 = color_0,  01 = color_1,  10 = color_2,   */
        /* 11 = transparent. */
        /* These two bit codes correspond to the 2-bit fields  */
        /* stored in the 64-bit block.  */

        /* explicit for each component, unlike some refrasts... */

        *wrd = ((RwUInt16)col_0->r + (RwUInt16)col_1->r )/2;
        col_2->r = (RwUInt8)*wrd;
        *wrd = ((RwUInt16)col_0->g + (RwUInt16)col_1->g )/2;
        col_2->g = (RwUInt8)*wrd;
        *wrd = ((RwUInt16)col_0->b + (RwUInt16)col_1->b )/2;
        col_2->b = (RwUInt8)*wrd;
        col_2->a = 0xff;

        col_3->r = 0x00;        /* black color to indicate alpha */
        col_3->g = 0x00;
        col_3->b = 0x00;
        col_3->a = 0x00;

        hasAlpha = TRUE;
    }

    RWRETURN(hasAlpha);
}

/****************************************************************************
 GetColorBlockColors

 On entry   :
 On exit    :
 */
static __inline void
GetColorBlockColors(const DXTColBlock *pBlock,
                    Color8888 *col_0, Color8888 *col_1,
                    Color8888 *col_2, Color8888 *col_3,
                    RwUInt16 *wrd)
{
    /* There are 4 methods to use - see the Time_ functions. */
    /* 1st = shift = does normal approach per byte for color comps */
    /* 2nd = use freak variable bit field color565 for component extraction */
    /* 3rd = use super-freak RwUInt32 adds BEFORE shifting the color components */
    /*  This lets you do only 1 add per color instead of 3 RwUInt8 adds and */
    /*  might be faster */
    /* Call RunTimingSession() to run each of them & output result to txt file */

    /* freak variable bit structure method */
    /* normal math */
    /* This method is fastest */
    const Color565 *pCol;

    RWFUNCTION(RWSTRING("GetColorBlockColors"));

    pCol = (const Color565 *) & (pBlock->col0 );

    col_0->a = 0xff;
    col_0->r = pCol->nRed;
    col_0->r <<= 3;             /* shift to full precision */
    col_0->g = pCol->nGreen;
    col_0->g <<= 2;
    col_0->b = pCol->nBlue;
    col_0->b <<= 3;

    pCol = (const Color565 *) & (pBlock->col1 );
    col_1->a = 0xff;
    col_1->r = pCol->nRed;
    col_1->r <<= 3;             /* shift to full precision */
    col_1->g = pCol->nGreen;
    col_1->g <<= 2;
    col_1->b = pCol->nBlue;
    col_1->b <<= 3;

    if( pBlock->col0 > pBlock->col1 )
    {
        /* Four-color block: derive the other two colors.     */
        /* 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3 */
        /* These two bit codes correspond to the 2-bit fields  */
        /* stored in the 64-bit block. */

        *wrd = ((RwUInt16)col_0->r * 2 + (RwUInt16)col_1->r )/3;
                                            /* no +1 for rounding */
                                            /* as bits have been shifted to 888 */
        col_2->r = (RwUInt8)*wrd;

        *wrd = ((RwUInt16)col_0->g * 2 + (RwUInt16)col_1->g )/3;
        col_2->g = (RwUInt8)*wrd;

        *wrd = ((RwUInt16)col_0->b * 2 + (RwUInt16)col_1->b )/3;
        col_2->b = (RwUInt8)*wrd;
        col_2->a = 0xff;

        *wrd = ((RwUInt16)col_0->r + (RwUInt16)col_1->r *2 )/3;
        col_3->r = (RwUInt8)*wrd;

        *wrd = ((RwUInt16)col_0->g + (RwUInt16)col_1->g *2 )/3;
        col_3->g = (RwUInt8)*wrd;

        *wrd = ((RwUInt16)col_0->b + (RwUInt16)col_1->b *2 )/3;
        col_3->b = (RwUInt8)*wrd;
        col_3->a = 0xff;
    }
    else
    {
        /* Three-color block: derive the other color. */
        /* 00 = color_0,  01 = color_1,  10 = color_2,   */
        /* 11 = transparent. */
        /* These two bit codes correspond to the 2-bit fields  */
        /* stored in the 64-bit block.  */

        /* explicit for each component, unlike some refrasts... */

        *wrd = ((RwUInt16)col_0->r + (RwUInt16)col_1->r )/2;
        col_2->r = (RwUInt8)*wrd;
        *wrd = ((RwUInt16)col_0->g + (RwUInt16)col_1->g )/2;
        col_2->g = (RwUInt8)*wrd;
        *wrd = ((RwUInt16)col_0->b + (RwUInt16)col_1->b )/2;
        col_2->b = (RwUInt8)*wrd;
        col_2->a = 0xff;

        col_3->r = 0x00;        /* random color to indicate alpha */
        col_3->g = 0xff;
        col_3->b = 0xff;
        col_3->a = 0x00;
    }

    RWRETURNVOID();
}

/****************************************************************************
 DecodeColorBlockCheckColor3

 On entry   :
 On exit    :   TRUE if it use the 3rd color
 */
static __inline RwBool
DecodeColorBlockCheckColor3(RwUInt32 * pImPos, const DXTColBlock * pColorBlock,
                 RwInt32 width,
                 RwUInt32 * col_0, RwUInt32 * col_1,
                 RwUInt32 * col_2, RwUInt32 * col_3 )
{
    /* width is width of image in pixels */
    RwInt32     r,n;
    RwBool      usedThird = FALSE;

    RWFUNCTION(RWSTRING("DecodeColorBlockCheckColor3"));

    /* r steps through lines in y */
    for( r=0; r < 4; r++, pImPos += width-4 )   /* no width*4 as RwUInt32 ptr inc will *4 */
    {
        RwUInt32 bits = pColorBlock->row[r];

        /* width * 4 bytes per pixel per line */
        /* each j dxtc row is 4 lines of pixels */

        /* pImPos = (RwUInt32*)((RwUInt32)pBase + i*16 + (r+j*4) * m_nWidth * 4 ); */

        /* n steps through pixels */
        for( n=0; n < 4; n++ )
        {
            switch( bits & 0x3)
            {
            case 0 :
                *pImPos = *col_0;
                pImPos++;       /* increment to next RwUInt32 */
                break;
            case 1 :
                *pImPos = *col_1;
                pImPos++;
                break;
            case 2 :
                *pImPos = *col_2;
                pImPos++;
                break;
            case 3 :
                usedThird = TRUE;
                *pImPos = *col_3;
                pImPos++;
                break;
            default:
                pImPos++;
                break;
            }

            bits >>= 2;
        }
    }

    RWRETURN(usedThird);
}

/****************************************************************************
 DecodeColorBlock

 On entry   :
 On exit    :
 */
static __inline void
DecodeColorBlock(RwUInt32 * pImPos, const DXTColBlock * pColorBlock,
                 RwInt32 width,
                 RwUInt32 * col_0, RwUInt32 * col_1,
                 RwUInt32 * col_2, RwUInt32 * col_3 )
{
    /* width is width of image in pixels */
    RwInt32     r,n;

    RWFUNCTION(RWSTRING("DecodeColorBlock"));

    /* r steps through lines in y */
    for( r=0; r < 4; r++, pImPos += width-4 )   /* no width*4 as RwUInt32 ptr inc will *4 */
    {
        RwUInt32 bits = pColorBlock->row[r];

        /* width * 4 bytes per pixel per line */
        /* each j dxtc row is 4 lines of pixels */

        /* pImPos = (RwUInt32*)((RwUInt32)pBase + i*16 + (r+j*4) * m_nWidth * 4 ); */

        /* n steps through pixels */
        for( n=0; n < 4; n++ )
        {
            switch( bits & 0x3)
            {
            case 0 :
                *pImPos = *col_0;
                pImPos++;       /* increment to next RwUInt32 */
                break;
            case 1 :
                *pImPos = *col_1;
                pImPos++;
                break;
            case 2 :
                *pImPos = *col_2;
                pImPos++;
                break;
            case 3 :
                *pImPos = *col_3;
                pImPos++;
                break;
            default:
                pImPos++;
                break;
            }

            bits >>= 2;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 DXT1BlockHasAlpha

 On entry   :
 On exit    :
 */
static __inline RwBool
DXT1BlockHasAlpha(const DXTColBlock *pColorBlock)
{
    /* width is width of image in pixels */
    RwInt32     r,n;

    RWFUNCTION(RWSTRING("DXT1BlockHasAlpha"));

    /* r steps through lines in y */
    for( r=0; r < 4; r++)
    {
        RwUInt32 bits = pColorBlock->row[r];

        /* n steps through pixels */
        for( n=0; n < 4; n++ )
        {
            if ((bits & 0x3) == 0x3)
            {
                /* this color is the only one with non-255 alpha */
                RWRETURN(TRUE);
            }

            bits >>= 2;
        }
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 DXT1BlockNonSquareHasAlpha

 On entry   :
 On exit    :
 */
static __inline RwBool
DXT1BlockNonSquareHasAlpha(const DXTColBlock *pColorBlock, RwInt32 width, RwInt32 height)
{
    /* width is width of image in pixels */
    RwInt32     r,n;

    RWFUNCTION(RWSTRING("DXT1BlockNonSquareHasAlpha"));

    /* r steps through lines in y */
    for( r=0; r < height; r++)
    {
        RwUInt32 bits = pColorBlock->row[r];

        /* n steps through pixels */
        for( n=0; n < width; n++ )
        {
            if ((bits & 0x3) == 0x3)
            {
                /* this color is the only one with non-255 alpha */
                RWRETURN(TRUE);
            }

            bits >>= 2;
        }
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 DecodeAlphaExplicit

 On entry   :
 On exit    :
 */
static __inline void
DecodeAlphaExplicit(RwUInt32 *pImPos, const DXTAlphaBlockExplicit *pAlphaBlock,
                    RwInt32 width, RwUInt32 alphazero)
{
    /* alphazero is a bit mask that when & with the image color */
    /*  will zero the alpha bits, so if the image DWORDs  are */
    /*  ARGB then alphazero will be 0x00ffffff or if */
    /*  RGBA then alphazero will be 0xffffff00 */
    /*  alphazero constructed automaticaly from field order of Color8888 structure */

    /* decodes to 32 bit format only */
    RwInt32 row, pix;

    RwUInt16 wrd;

    Color8888 col;

    RWFUNCTION(RWSTRING("DecodeAlphaExplicit"));

    col.r = col.g = col.b = 0;

    for( row=0; row < 4; row++, pImPos += width-4 )
    {
        /* pImPow += pImPos += width-4 moves to next row down */

        wrd = pAlphaBlock->row[ row ];

        for( pix = 0; pix < 4; pix++ )
        {
            /* zero the alpha bits of image pixel */
            *pImPos &= alphazero;

            col.a = wrd & 0x000f;       /* get only low 4 bits */
/*          col.a <<= 4;                // shift to full byte precision */
                                        /* NOTE:  with just a << 4 you'll never have alpha */
                                        /* of 0xff,  0xf0 is max so pure shift doesn't quite */
                                        /* cover full alpha range. */
                                        /* It's much cheaper than divide & scale though. */
                                        /* To correct for this, and get 0xff for max alpha, */
                                        /*  or the low bits back in after left shifting */
            col.a = col.a | (col.a << 4 );  /* This allows max 4 bit alpha to be 0xff alpha */
                                            /*  in final image, and is crude approach to full  */
                                            /*  range scale */

            *pImPos |= *((RwUInt32*)&col);  /* or the bits into the prev. nulled alpha */

            wrd >>= 4;      /* move next bits to lowest 4 */

            pImPos++;       /* move to next pixel in the row */

        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 DecodeAlpha3BitLinear

 On entry   :
 On exit    :
 */
static __inline void
DecodeAlpha3BitLinear(RwUInt32 *pImPos, const DXTAlphaBlock3BitLinear *pAlphaBlock,
                      RwInt32 width, RwUInt32 alphazero)
{
    const RwUInt32 mask = 0x00000007;       /* bits = 00 00 01 11 */

    RwUInt8     gBits[4][4];
    RwUInt16    gAlphas[8];
    RwUInt8     gACol[4][4];
    RwUInt32    bits;
    RwInt32     row, pix;

    RWFUNCTION(RWSTRING("DecodeAlpha3BitLinear"));

    gAlphas[0] = pAlphaBlock->alpha0;
    gAlphas[1] = pAlphaBlock->alpha1;

    /* 8-alpha or 6-alpha block?     */
    if( gAlphas[0] > gAlphas[1] )
    {
        /* 8-alpha block:  derive the other 6 alphas.     */
        /* 000 = alpha_0, 001 = alpha_1, others are interpolated */

        gAlphas[2] = ( 6 * gAlphas[0] +     gAlphas[1]) / 7;    /* bit code 010 */
        gAlphas[3] = ( 5 * gAlphas[0] + 2 * gAlphas[1]) / 7;    /* Bit code 011     */
        gAlphas[4] = ( 4 * gAlphas[0] + 3 * gAlphas[1]) / 7;    /* Bit code 100     */
        gAlphas[5] = ( 3 * gAlphas[0] + 4 * gAlphas[1]) / 7;    /* Bit code 101 */
        gAlphas[6] = ( 2 * gAlphas[0] + 5 * gAlphas[1]) / 7;    /* Bit code 110     */
        gAlphas[7] = (     gAlphas[0] + 6 * gAlphas[1]) / 7;    /* Bit code 111 */
    }
    else
    {
        /* 6-alpha block:  derive the other alphas.     */
        /* 000 = alpha_0, 001 = alpha_1, others are interpolated */

        gAlphas[2] = (4 * gAlphas[0] +     gAlphas[1]) / 5; /* Bit code 010 */
        gAlphas[3] = (3 * gAlphas[0] + 2 * gAlphas[1]) / 5; /* Bit code 011     */
        gAlphas[4] = (2 * gAlphas[0] + 3 * gAlphas[1]) / 5; /* Bit code 100     */
        gAlphas[5] = (    gAlphas[0] + 4 * gAlphas[1]) / 5; /* Bit code 101 */
        gAlphas[6] = 0;                                     /* Bit code 110 */
        gAlphas[7] = 255;                                   /* Bit code 111 */
    }

    /* Decode 3-bit fields into array of 16 BYTES with same value */

    /* first two rows of 4 pixels each: */
    /* pRows = (Alpha3BitRows*) & ( pAlphaBlock->stuff[0] ); */

    bits = *( (const RwUInt32 *) & ( pAlphaBlock->stuff[0] ));

    gBits[0][0] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[0][1] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[0][2] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[0][3] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[1][0] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[1][1] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[1][2] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[1][3] = (RwUInt8)( bits & mask );

    /* now for last two rows: */

    bits = *( (const RwUInt32 *) & ( pAlphaBlock->stuff[3] ));      /* last 3 bytes */

    gBits[2][0] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[2][1] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[2][2] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[2][3] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[3][0] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[3][1] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[3][2] = (RwUInt8)( bits & mask );
    bits >>= 3;
    gBits[3][3] = (RwUInt8)( bits & mask );

    /* decode the codes into alpha values */
    for( row = 0; row < 4; row++ )
    {
        for( pix=0; pix < 4; pix++ )
        {
            gACol[row][pix] = (RwUInt8) gAlphas[ gBits[row][pix] ];
        }
    }

    /* Write out alpha values to the image bits */
    for( row=0; row < 4; row++, pImPos += width-4 )
    {
        /* pImPow += pImPos += width-4 moves to next row down */

        for( pix = 0; pix < 4; pix++ )
        {
            /* zero the alpha bits of image pixel */
            *pImPos &=  alphazero;

            *pImPos |=  ((RwUInt32)gACol[row][pix]) << 24;  /* or the bits into the prev. nulled alpha */
            pImPos++;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8DecompressDXT1

 On entry   :
 On exit    :   TRUE is there is any transparent color
 */
RwBool
_rwD3D8DecompressDXT1(RwInt32 width, RwInt32 height, RwUInt32 *destPixels, const RwUInt16 *srcPixels)
{
    /* This was hacked up pretty quick & slopily */
    /* decompresses to 32 bit format 0xARGB */
    const RwInt32  xblocks = width / 4;
    const RwInt32  yblocks = height / 4;

    RwUInt32        *pBase  = destPixels;
    RwUInt32        *pImPos = pBase;            /* pos in decompressed data */


    const DXTColBlock   *pBlock;

    Color8888 col_0, col_1, col_2, col_3;

    RwUInt16 wrd;

    RwInt32 i, j;

    RwBool  hasAlpha = FALSE;

#if (0)
    const RwUInt16  *pPos   = srcPixels;    /* pos in compressed data */
#endif /* (0) */

    RWFUNCTION(RWSTRING("_rwD3D8DecompressDXT1"));

    for( j=0; j < yblocks; j++ )
    {
        /* 8 bytes per block */
        pBlock = (const DXTColBlock *) ( (const RwUInt8 *)srcPixels + j * xblocks * 8 );

        for( i=0; i < xblocks; i++, pBlock++ )
        {
            /* inline func: */
            const RwBool hasAlphaBlock = GetColorBlockColorsDXT1(pBlock,
                                                                &col_0,
                                                                &col_1,
                                                                &col_2,
                                                                &col_3,
                                                                &wrd);

            /* now decode the color block into the bitmap bits inline func: */
            pImPos = (RwUInt32 *)((RwUInt8 *)pBase + i*16 + (j*4) * width * 4 );

            if (hasAlphaBlock)
            {
                hasAlpha |= DecodeColorBlockCheckColor3(pImPos,
                                                        pBlock,
                                                        width,
                                                        (RwUInt32 *)&col_0,
                                                        (RwUInt32 *)&col_1,
                                                        (RwUInt32 *)&col_2,
                                                        (RwUInt32 *)&col_3 );
            }
            else
            {
                DecodeColorBlock(pImPos,
                                 pBlock,
                                 width,
                                 (RwUInt32 *)&col_0,
                                 (RwUInt32 *)&col_1,
                                 (RwUInt32 *)&col_2,
                                 (RwUInt32 *)&col_3 );
            }

            /* Set to RGB test pattern */
            /*  pImPos = (RwUInt32*)((RwUInt32)pBase + i*4 + j*m_nWidth*4); */
            /*  *pImPos = ((i*4) << 16) | ((j*4) << 8 ) | ( (63-i)*4 ); */

            /* checkerboard of only col_0 and col_1 basis colors: */
            /*  pImPos = (RwUInt32*)((RwUInt32)pBase + i*8 + j*m_nWidth*8); */
            /*  *pImPos = *((RwUInt32*)&col_0); */
            /*  pImPos += 1 + m_nWidth; */
            /*  *pImPos = *((RwUInt32*)&col_1); */
        }
    }

    RWRETURN(hasAlpha);
}

/****************************************************************************
 _rwD3D8DecompressDXT3

 On entry   :
 On exit    :
 */
void
_rwD3D8DecompressDXT3(RwInt32 width, RwInt32 height, RwUInt32 *destPixels, const RwUInt16 *srcPixels)
{
    const RwInt32 xblocks = width / 4;
    const RwInt32 yblocks = height / 4;

    RwUInt32        *pBase  = destPixels;
    RwUInt32        *pImPos = pBase;            /* pos in decompressed data */

    const DXTColBlock           *pBlock;
    const DXTAlphaBlockExplicit *pAlphaBlock;

    Color8888   col_0, col_1, col_2, col_3;
    RwUInt32    alphazero;

    RwUInt16 wrd;

    RwInt32 i,j;

#if (0)
    const RwUInt16  *pPos   = srcPixels;    /* pos in compressed data */
#endif /* (0) */

    RWFUNCTION(RWSTRING("_rwD3D8DecompressDXT3"));

    /* fill alphazero with appropriate value to zero out alpha when */
    /*  alphazero is ANDed with the image color 32 bit RwUInt32: */
    col_0.a = 0;
    col_0.r = col_0.g = col_0.b = 0xff;
    alphazero = *((RwUInt32*) &col_0);

    for( j=0; j < yblocks; j++ )
    {
        /* 8 bytes per block */
        /* 1 block for alpha, 1 block for color */
        pBlock = (const DXTColBlock *) ( (const RwUInt8 *)srcPixels + j * xblocks * 16 );

        for( i=0; i < xblocks; i++, pBlock ++ )
        {
            /* inline */
            /* Get alpha block */

            pAlphaBlock = (const DXTAlphaBlockExplicit*) pBlock;

            /* inline func: */
            /* Get color block & colors */
            pBlock++;

            GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3, &wrd);

            /* Decode the color block into the bitmap bits */
            /* inline func: */

            pImPos = (RwUInt32*)((RwUInt8 *)pBase + i*16 + (j*4) * width * 4 );


            DecodeColorBlock(pImPos, pBlock, width,
                             (RwUInt32 *)&col_0, (RwUInt32 *)&col_1,
                             (RwUInt32 *)&col_2, (RwUInt32 *)&col_3 );

            /* Overwrite the previous alpha bits with the alpha block */
            /*  info */
            /* inline func: */
            DecodeAlphaExplicit( pImPos, pAlphaBlock, width, alphazero );
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8DecompressDXT5

 On entry   :
 On exit    :
 */
void
_rwD3D8DecompressDXT5(RwInt32 width, RwInt32 height, RwUInt32 *destPixels, const RwUInt16 *srcPixels)
{
    const RwInt32 xblocks = width / 4;
    const RwInt32 yblocks = height / 4;

    RwUInt32        *pBase  = destPixels;
    RwUInt32        *pImPos = pBase;            /* pos in decompressed data */

    const DXTColBlock               *pBlock;
    const DXTAlphaBlock3BitLinear   *pAlphaBlock;

    Color8888   col_0, col_1, col_2, col_3;
    RwUInt32    alphazero;

    RwUInt16 wrd;

    RwInt32 i,j;

#if (0)
    const RwUInt16  *pPos   = srcPixels;    /* pos in compressed data */
#endif /* (0) */

    RWFUNCTION(RWSTRING("_rwD3D8DecompressDXT5"));

    /* fill alphazero with appropriate value to zero out alpha when */
    /*  alphazero is ANDed with the image color 32 bit RwUInt32: */
    col_0.a = 0;
    col_0.r = col_0.g = col_0.b = 0xff;

    alphazero = *((RwUInt32*) &col_0);

    for( j=0; j < yblocks; j++ )
    {
        /* 8 bytes per block */
        /* 1 block for alpha, 1 block for color */
        pBlock = (const DXTColBlock *) ( (const RwUInt8 *)srcPixels + j * xblocks * 16 );

        for( i=0; i < xblocks; i++, pBlock ++ )
        {
            /* inline */
            /* Get alpha block */

            pAlphaBlock = (const DXTAlphaBlock3BitLinear *) pBlock;

            /* inline func: */
            /* Get color block & colors */
            pBlock++;

            GetColorBlockColors( pBlock, &col_0, &col_1, &col_2, &col_3, &wrd);

            /* Decode the color block into the bitmap bits */
            /* inline func: */

            pImPos = (RwUInt32*)((RwUInt8 *)pBase + i*16 + (j*4) * width * 4 );

            DecodeColorBlock(pImPos, pBlock, width,
                             (RwUInt32 *)&col_0, (RwUInt32 *)&col_1,
                             (RwUInt32 *)&col_2, (RwUInt32 *)&col_3 );

            /* Overwrite the previous alpha bits with the alpha block */
            /*  info */
            DecodeAlpha3BitLinear( pImPos, pAlphaBlock, width, alphazero);
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D8RasterDXT1HasAlpha

 On entry   :
 On exit    :
 */
static RwBool
D3D8RasterDXT1HasAlpha(RwRaster *raster)
{
    RwInt32 numMipLevels;
    RwInt32 i;

    RWFUNCTION(RWSTRING("D3D8RasterDXT1HasAlpha"));

    /* Load the surfaces */
    numMipLevels = RwRasterGetNumLevels(raster);
    for (i = 0; i < numMipLevels; i++)
    {
        RwBool              squareBlocks = TRUE;
        RwInt32             sizeblockX, sizeblockY, totalBlocks;
        const DXTColBlock   *pixels;

        pixels = (const DXTColBlock *)RwRasterLock(raster, (RwUInt8)i, rwRASTERLOCKREAD);
        RWASSERT(NULL != pixels);

        if (raster->width < 4)
        {
            sizeblockX = raster->width;
            totalBlocks = 1;

            squareBlocks = FALSE;
        }
        else
        {
            sizeblockX = 4;
            totalBlocks = raster->width / 4;
        }

        if (raster->height < 4)
        {
            sizeblockY = raster->height;

            squareBlocks = FALSE;
        }
        else
        {
            sizeblockY = 4;
            totalBlocks *= raster->height / 4;
        }

        if (squareBlocks)
        {
            do
            {
                /*
                 * If the first color is greater than the second, it implies that
                 * only opaque texels are defined
                 */
                if (!(pixels->col0 > pixels->col1) &&
                    /* but not all of those have non-opaque alpha */
                    DXT1BlockHasAlpha(pixels))
                {
                    RwRasterUnlock(raster);

                    RWRETURN(TRUE);
                }

                pixels ++;
            }
            while(--totalBlocks);
        }
        else
        {
            do
            {
                /*
                 * If the first color is greater than the second, it implies that
                 * only opaque texels are defined
                 */
                if (!(pixels->col0 > pixels->col1) &&
                    /* but not all of those have non-opaque alpha */
                    DXT1BlockNonSquareHasAlpha(pixels, sizeblockX, sizeblockY))
                {
                    RwRasterUnlock(raster);

                    RWRETURN(TRUE);
                }

                pixels ++;
            }
            while(--totalBlocks);
        }

        RwRasterUnlock(raster);
    }

    RWRETURN(FALSE);
}


/****************************************************************************
 D3D8RasterGetSize

 Description: Gets the size of a rasters mip level, the raster must not be
              locked.

 On entry   : raster - The locked raster.
            : mipLevel - The mip level to get the size for.
 On exit    : The size of the mip level in bytes.
 */
static RwUInt32
D3D8RasterGetSize(RwRaster *raster, RwUInt8 mipLevel)
{
    RwUInt32            blocksX, blocksY, totalBlocks;
    RwUInt32            width, height;
    RwUInt32            shift, size;
    _rwD3D8RasterExt    *rasExt;

    RWFUNCTION(RWSTRING("D3D8RasterGetSize"));

    rasExt = RASTEREXTFROMRASTER(raster);

    /* Calc mip level size */
    width = raster->width >> mipLevel;
    height = raster->height >> mipLevel;

    /* Clamp width and height to 1 */
    width = ((0 == width) ? 1 : width);
    height = ((0 == height) ? 1 : height);

    /* Is the raster a DXT compressed format */
    if (0 == rasExt->dxtFormat)
    {
        size = width * height * (raster->depth >> 3);

        RWRETURN(size);
    }
    else
    {
        /* A DXT compressed format */
        if (D3DFMT_DXT1 == rasExt->dxtFormat)
        {
            shift = 3; /* 64Bits / 8 = 8. 2^3 = 8 */
        }
        else
        {
            shift = 4; /* 128Bits / 8 = 16. 2^4 = 16 */
        }

        blocksX = width >> 2;
        blocksY = height >> 2;

        blocksX = ((0 == blocksX) ? 1 : blocksX);
        blocksY = ((0 == blocksY) ? 1 : blocksY);

        totalBlocks = blocksX * blocksY;

        size = totalBlocks << shift;
        RWASSERT(8 <= size);

        RWRETURN(size);
    }

    RWRETURN(-1);
}
/**
 * \ingroup rwrasterd3d8
 * \ref RwD3D8RasterStreamReadDDS creates an \ref RwRaster from a DDS stored in
 * the open stream argument. This function currently supports all different flavors
 * of DDS files, including DXT compressed textures, cube textures and mipmaps.
 *
 * If you try to load a raster with a format not supported by the video
 * card drivers, RWD3D8 will automatically convert the texture to the most
 * suitable raster format.
 *
 * For more information on DDS files, DXT compressed textures and cube textures,
 * see the D3D8 SDK documentation.
 *
 * \param stream   \ref RwStream where the DDS file is stored.
 *
 * \return The \ref RwRaster on success or a NULL pointer on error.
 *
 * \see RwD3D8DDSTextureRead
 * \see RwTextureRead
 */
RwRaster *
RwD3D8RasterStreamReadDDS(RwStream *stream)
{
    HRESULT         hr;
    RwRaster        *raster;
    _rwD3D8RasterExt  *rasExt;
    RwUInt32        magic;
    DDS_HEADER      header;
    RwInt32         mipMapped;
    RwInt32         numMipLevels;
    RwUInt32        size;
    RwInt32         i;
    RwBool          cubeTexture;
    RwBool          convert;

    RWAPIFUNCTION(RWSTRING("RwD3D8RasterStreamReadDDS"));

    raster = NULL;
    cubeTexture = FALSE;
    convert = FALSE;

    size = sizeof(RwUInt32);
    if (RwStreamRead(stream, (void *)&magic, size) != size)
    {
        RWRETURN(NULL);
    }

    /* Check it's a "DDS " file */
    if (0x20534444 != magic)
    {
        RWRETURN(NULL);
    }

    /* Read the header */
    size = sizeof(DDS_HEADER);
    if (RwStreamRead(stream, (void *)&header, size) != size)
    {
        RWRETURN(NULL);
    }

    /* Is it a texture ? */
    if (!(header.dwHeaderFlags & DDS_HEADER_FLAGS_TEXTURE))
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                           "This is not a texture");
        RWRETURN(NULL);
    }

    /* Is this texture mipmapped? */
    mipMapped = header.dwHeaderFlags & DDS_HEADER_FLAGS_MIPMAP ? rwRASTERFORMATMIPMAP : 0;

    /* Is this a cube texture */
    cubeTexture = ( (header.dwSurfaceFlags & DDS_SURFACE_FLAGS_CUBEMAP) != 0 &&
                    (header.dwCubemapFlags & DDS_CUBEMAP_ALLFACES) != 0 );

    /* Handle DXT1-5 format DDS files */
    if (DDS_FOURCC == header.ddspf.dwFlags)
    {
        if (cubeTexture)
        {
            convert = (_rwD3D8CheckValidCubeTextureFormat(header.ddspf.dwFourCC) ==
                       FALSE);

            if (convert)
            {
                RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                   "Cube texture format not supported");
                RWRETURN(NULL);
            }
        }
        else
        {
            convert = (_rwD3D8CheckValidTextureFormat(header.ddspf.dwFourCC) ==
                       FALSE);
        }

        if (!convert)
        {
            RwUInt32 levels;

            /* Create a raster */
            raster = RwRasterCreate(header.dwWidth, header.dwHeight, 0,
            rwRASTERTYPETEXTURE | rwRASTERDONTALLOCATE | mipMapped);

            if (NULL == raster)
            {
                RWRETURN(NULL);
            }

            /* Remove the rwRASTERDONTALLOCATE flag so it will get destroyed */
            raster->cFlags &= ~rwRASTERDONTALLOCATE;

            rasExt = RASTEREXTFROMRASTER(raster);

            /* Find the surface format */
            rasExt->dxtFormat = header.ddspf.dwFourCC;

            /* Create the texture, if mipped 0 gets all the mip levels */
            if (cubeTexture)
            {
                if (_RwD3D8DeviceCaps.CubeTextureFilterCaps & (D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR))
                {
                    levels = ( (header.dwHeaderFlags & DDS_HEADER_FLAGS_MIPMAP) ?
                               header.dwMipMapCount : 1 );
                }
                else
                {
                    #ifdef RWDEBUG
                    if (header.dwHeaderFlags & DDS_HEADER_FLAGS_MIPMAP)
                    {
                        RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                          "Cube texture mip-mapping not supported");
                    }
                    #endif

                    levels = 1;
                }

                rasExt->cube = TRUE;

                hr = DXCHECK(IDirect3DDevice8_CreateCubeTexture(_RwD3DDevice,
                                                            raster->width,
                                                            levels,
                                                            IGNORED_D3D8,
                                                            (D3DFORMAT)rasExt->dxtFormat,
                                                            D3DPOOL_MANAGED,
                                                            (LPDIRECT3DCUBETEXTURE8 *)
                                                            &(rasExt->texture)));
            }
            else
            {
                levels = ( (header.dwHeaderFlags & DDS_HEADER_FLAGS_MIPMAP) ?
                           header.dwMipMapCount : 1 );

                hr = DXCHECK(IDirect3DDevice8_CreateTexture(_RwD3DDevice,
                                                            raster->width,
                                                            raster->height,
                                                            levels,
                                                            IGNORED_D3D8,
                                                            (D3DFORMAT)rasExt->dxtFormat,
                                                            D3DPOOL_MANAGED,
                                                            &(rasExt->texture)));
            }

            if (FAILED(hr))
            {
                RWRETURN(NULL);
            }
        }
    }
    else if (DDS_RGB == header.ddspf.dwFlags)
    {
        if (header.ddspf.dwRGBBitCount == 32)
        {
            RwRasterFormat    rasFormat = rwRASTERFORMATDEFAULT;

            if (cubeTexture)
            {
                D3DFORMAT d3dFormat;

                if (0x00ff0000 == header.ddspf.dwRBitMask)
                {
                    d3dFormat = D3DFMT_X8R8G8B8;

                    if (_rwD3D8CheckValidCubeTextureFormat(D3DFMT_X8R8G8B8))
                    {
                        rasFormat = rwRASTERFORMAT888;
                    }
                    else
                    {
                        RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                        "Cube texture format not supported");
                        RWRETURN(NULL);
                    }
                }
                else if (0x0000f800 == header.ddspf.dwRBitMask)
                {
                    d3dFormat = D3DFMT_R5G6B5;

                    if (_rwD3D8CheckValidCubeTextureFormat(D3DFMT_R5G6B5))
                    {
                        rasFormat = rwRASTERFORMAT565;
                    }
                    else
                    {
                        RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                        "Cube texture format not supported");
                        RWRETURN(NULL);
                    }
                }
                else
                {
                    RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                    "Cube texture format not supported");
                    RWRETURN(NULL);
                }

                if (!convert)
                {
                    RwUInt32 width, height;

                    RWASSERT(rwRASTERFORMATDEFAULT != rasFormat);

                    /* Check size */
                    width = header.dwWidth;
                    height = header.dwHeight;

                    _rwD3D8CheckRasterSize((RwInt32 *)&width, (RwInt32 *)&height, rwRASTERTYPETEXTURE | rasFormat | mipMapped);

                    if (header.dwWidth != width ||
                        header.dwHeight != height)
                    {
                        RWRETURN(NULL);
                    }
                    else
                    {
                        raster = RwRasterCreate(header.dwWidth, header.dwHeight, 0,
                            rwRASTERTYPETEXTURE | rwRASTERDONTALLOCATE |
                            rasFormat | mipMapped);

                        if (NULL == raster)
                        {
                            RWRETURN(NULL);
                        }
                        else
                        {
                            RwUInt32 levels;

                            /* Remove the rwRASTERDONTALLOCATE flag so it will get destroyed */
                            raster->cFlags &= ~rwRASTERDONTALLOCATE;

                            rasExt = RASTEREXTFROMRASTER(raster);

                            rasExt->cube = TRUE;

                            if (_RwD3D8DeviceCaps.CubeTextureFilterCaps & (D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR))
                            {
                                levels = ( (header.dwHeaderFlags & DDS_HEADER_FLAGS_MIPMAP) ?
                                        header.dwMipMapCount : 1 );
                            }
                            else
                            {
                                #ifdef RWDEBUG
                                if (header.dwHeaderFlags & DDS_HEADER_FLAGS_MIPMAP)
                                {
                                    RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
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
                                                                (D3DFORMAT)d3dFormat,
                                                                D3DPOOL_MANAGED,
                                                                (LPDIRECT3DCUBETEXTURE8 *)
                                                                &(rasExt->texture)));
                            if (FAILED(hr))
                            {
                                RWRETURN(NULL);
                            }
                        }
                    }
                }
            }
            else
            {
                if (0x00ff0000 == header.ddspf.dwRBitMask)
                {
                    if (_rwD3D8CheckValidTextureFormat(D3DFMT_X8R8G8B8))
                    {
                        rasFormat = rwRASTERFORMAT888;
                    }
                    else
                    {
                        convert = TRUE;
                    }
                }
                else if (0x0000f800 == header.ddspf.dwRBitMask)
                {
                    rasFormat = rwRASTERFORMAT565;
                }
                else
                {
                    RWRETURN(NULL);
                }

                if (!convert)
                {
                    RwUInt32 width, height;

                    RWASSERT(rwRASTERFORMATDEFAULT != rasFormat);

                    /* Check size */
                    width = header.dwWidth;
                    height = header.dwHeight;

                    _rwD3D8CheckRasterSize((RwInt32 *)&width, (RwInt32 *)&height, rwRASTERTYPETEXTURE | rasFormat | mipMapped);

                    if (header.dwWidth != width ||
                        header.dwHeight != height)
                    {
                        convert = TRUE;
                    }
                    else
                    {
                        raster = RwRasterCreate(header.dwWidth, header.dwHeight, 0,
                            rwRASTERTYPETEXTURE | rasFormat | mipMapped);

                        if (NULL == raster)
                        {
                            RWRETURN(NULL);
                        }
                    }
                }
            }
        }
        else
        {
            convert = TRUE;
        }
    }
    else if (DDS_RGBA == header.ddspf.dwFlags)
    {
        RwRasterFormat    rasFormat = rwRASTERFORMATDEFAULT;

        if (cubeTexture)
        {
            D3DFORMAT d3dFormat;

            if (0x00ff0000 == header.ddspf.dwRBitMask)
            {
                d3dFormat = D3DFMT_A8R8G8B8;

                if (_rwD3D8CheckValidCubeTextureFormat(D3DFMT_A8R8G8B8))
                {
                    rasFormat = rwRASTERFORMAT8888;
                }
                else
                {
                    RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                      "Cube texture format not supported");
                    RWRETURN(NULL);
                }
            }
            else if (0x00007c00 == header.ddspf.dwRBitMask)
            {
                d3dFormat = D3DFMT_A1R5G5B5;

                if (_rwD3D8CheckValidCubeTextureFormat(D3DFMT_A1R5G5B5))
                {
                    rasFormat = rwRASTERFORMAT1555;
                }
                else
                {
                    RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                      "Cube texture format not supported");
                    RWRETURN(NULL);
                }
            }
            else if (0x00000f00 == header.ddspf.dwRBitMask)
            {
                d3dFormat = D3DFMT_A4R4G4B4;

                if (_rwD3D8CheckValidCubeTextureFormat(D3DFMT_A4R4G4B4))
                {
                    rasFormat = rwRASTERFORMAT4444;
                }
                else
                {
                    RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                      "Cube texture format not supported");
                    RWRETURN(NULL);
                }
            }
            else
            {
                RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                                  "Cube texture format not supported");
                RWRETURN(NULL);
            }

            if (!convert)
            {
                RwUInt32 width, height;

                RWASSERT(rwRASTERFORMATDEFAULT != rasFormat);

                /* Check size */
                width = header.dwWidth;
                height = header.dwHeight;

                _rwD3D8CheckRasterSize((RwInt32 *)&width, (RwInt32 *)&height, rwRASTERTYPETEXTURE | rasFormat | mipMapped);

                if (header.dwWidth != width ||
                    header.dwHeight != height)
                {
                    RWRETURN(NULL);
                }
                else
                {
                    raster = RwRasterCreate(header.dwWidth, header.dwHeight, 0,
                        rwRASTERTYPETEXTURE | rwRASTERDONTALLOCATE |
                        rasFormat | mipMapped);

                    if (NULL == raster)
                    {
                        RWRETURN(NULL);
                    }
                    else
                    {
                        RwUInt32 levels;

                        /* Remove the rwRASTERDONTALLOCATE flag so it will get destroyed */
                        raster->cFlags &= ~rwRASTERDONTALLOCATE;

                        rasExt = RASTEREXTFROMRASTER(raster);

                        rasExt->cube = TRUE;

                        if (_RwD3D8DeviceCaps.CubeTextureFilterCaps & (D3DPTFILTERCAPS_MIPFPOINT | D3DPTFILTERCAPS_MIPFLINEAR))
                        {
                            levels = ( (header.dwHeaderFlags & DDS_HEADER_FLAGS_MIPMAP) ?
                                       header.dwMipMapCount : 1 );
                        }
                        else
                        {
                            #ifdef RWDEBUG
                            if (header.dwHeaderFlags & DDS_HEADER_FLAGS_MIPMAP)
                            {
                                RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
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
                                                               (D3DFORMAT)d3dFormat,
                                                               D3DPOOL_MANAGED,
                                                               (LPDIRECT3DCUBETEXTURE8 *)
                                                               &(rasExt->texture)));
                        if (FAILED(hr))
                        {
                            RWRETURN(NULL);
                        }
                    }
                }
            }
        }
        else
        {
            if (0x00ff0000 == header.ddspf.dwRBitMask)
            {
                if (_rwD3D8CheckValidTextureFormat(D3DFMT_A8R8G8B8))
                {
                    rasFormat = rwRASTERFORMAT8888;
                }
                else
                {
                    convert = TRUE;
                }
            }
            else if (0x00007c00 == header.ddspf.dwRBitMask)
            {
                rasFormat = rwRASTERFORMAT1555;
            }
            else if (0x00000f00 == header.ddspf.dwRBitMask)
            {
                if (_rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4))
                {
                    rasFormat = rwRASTERFORMAT4444;
                }
                else
                {
                    convert = TRUE;
                }
            }
            else
            {
                RWRETURN(NULL);
            }

            if (!convert)
            {
                RwUInt32 width, height;

                RWASSERT(rwRASTERFORMATDEFAULT != rasFormat);

                /* Check size */
                width = header.dwWidth;
                height = header.dwHeight;

                _rwD3D8CheckRasterSize((RwInt32 *)&width, (RwInt32 *)&height, rwRASTERTYPETEXTURE | rasFormat | mipMapped);

                if (header.dwWidth != width ||
                    header.dwHeight != height)
                {
                    convert = TRUE;
                }
                else
                {
                    raster = RwRasterCreate(header.dwWidth, header.dwHeight, 0,
                        rwRASTERTYPETEXTURE | rasFormat | mipMapped);

                    if (NULL == raster)
                    {
                        RWRETURN(NULL);
                    }
                }
            }
        }
    }
    else
    {
        RWRETURN(NULL);
    }

    if (!convert)
    {
        RwInt32 rasterNumMipLevels;

        rasExt = RASTEREXTFROMRASTER(raster);

        /* Load the surfaces */
        rasterNumMipLevels = RwRasterGetNumLevels(raster);

        if (mipMapped)
        {
            numMipLevels = header.dwMipMapCount;
        }
        else
        {
            numMipLevels = 1;
        }

        RWASSERT( numMipLevels == rasterNumMipLevels);

        do
        {
            if (cubeTexture)
            {
                if (header.dwCubemapFlags & DDS_CUBEMAP_POSITIVEX)
                {
                    rasExt->face = D3DCUBEMAP_FACE_POSITIVE_X;

                    header.dwCubemapFlags &= ~DDS_CUBEMAP_POSITIVEX;
                }
                else if (header.dwCubemapFlags & DDS_CUBEMAP_NEGATIVEX)
                {
                    rasExt->face = D3DCUBEMAP_FACE_NEGATIVE_X;

                    header.dwCubemapFlags &= ~DDS_CUBEMAP_NEGATIVEX;
                }
                else if (header.dwCubemapFlags & DDS_CUBEMAP_POSITIVEY)
                {
                    rasExt->face = D3DCUBEMAP_FACE_POSITIVE_Y;

                    header.dwCubemapFlags &= ~DDS_CUBEMAP_POSITIVEY;
                }
                else if (header.dwCubemapFlags & DDS_CUBEMAP_NEGATIVEY)
                {
                    rasExt->face = D3DCUBEMAP_FACE_NEGATIVE_Y;

                    header.dwCubemapFlags &= ~DDS_CUBEMAP_NEGATIVEY;
                }
                else if (header.dwCubemapFlags & DDS_CUBEMAP_POSITIVEZ)
                {
                    rasExt->face = D3DCUBEMAP_FACE_POSITIVE_Z;

                    header.dwCubemapFlags &= ~DDS_CUBEMAP_POSITIVEZ;
                }
                else if (header.dwCubemapFlags & DDS_CUBEMAP_NEGATIVEZ)
                {
                    rasExt->face = D3DCUBEMAP_FACE_NEGATIVE_Z;

                    header.dwCubemapFlags = 0;
                }
                else
                {
                    break;
                }
            }

            for (i = 0; i < numMipLevels; i++)
            {
                if (i >= rasterNumMipLevels)
                {
                    /* Skip mip level */
                    size = D3D8RasterGetSize(raster, i);

                    RwStreamSkip(stream, size);
                }
                else
                {
                    D3DSURFACE_DESC surfaceDesc;
                    RwUInt8         *pixels;

                    pixels = RwRasterLock(raster, (RwUInt8)i, rwRASTERLOCKWRITE);
                    RWASSERT(NULL != pixels);

                    /* Get the surface descriptor to find the size */
                    IDirect3DTexture8_GetLevelDesc(rasExt->texture, i, &surfaceDesc);

                    /* Load the surface */
                    if (DDS_FOURCC == header.ddspf.dwFlags)
                    {
                        size = surfaceDesc.Size;

                        if (RwStreamRead(stream, (void *)pixels, size) != size)
                        {
                            RWRETURN(NULL);
                        }
                    }
                    else
                    {
                        size = surfaceDesc.Height * surfaceDesc.Width * (raster->depth / 8);

                        if (RwStreamRead(stream, (void *)pixels, size) != size)
                        {
                            RWRETURN(NULL);
                        }
                    }

                    RwRasterUnlock(raster);
                }
            }
        }
        while(cubeTexture && header.dwCubemapFlags);

        /* Does this raster format have an alpha component */
        if (DDS_FOURCC == header.ddspf.dwFlags)
        {
            RwRasterFormat pixelFormat;

            pixelFormat = (RwRasterFormat)
                (RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK);

            if (D3DFMT_DXT1 == rasExt->dxtFormat)
            {
                rasExt->alpha = D3D8RasterDXT1HasAlpha(raster);

                if (rasExt->alpha == FALSE)
                {
                    raster->cFormat &= ~(rwRASTERFORMATPIXELFORMATMASK >> 8);
                    raster->cFormat |= (rwRASTERFORMAT565 >> 8);
                }
                else
                {
                    raster->cFormat &= ~(rwRASTERFORMATPIXELFORMATMASK >> 8);
                    raster->cFormat |= (rwRASTERFORMAT1555 >> 8);
                    raster->depth = 16;
                }
            }
            else
            {
                rasExt->alpha = TRUE;

                raster->cFormat &= ~(rwRASTERFORMATPIXELFORMATMASK >> 8);
                raster->cFormat |= (rwRASTERFORMAT4444 >> 8);
            }

            raster->depth = 16;
        }
        else if (DDS_RGBA == header.ddspf.dwFlags)
        {
            rasExt->alpha = TRUE;
        }
        else
        {
            rasExt->alpha = FALSE;
        }
    }
    else
    {
        RwUInt8         *pixels;
        RwImage         *image;
        RwRasterFormat  rasFormat = rwRASTERFORMATDEFAULT;
        RwUInt32        pitch;

        /*
         * Read to a 32 bits image and later create a raster from it
         */

        /* Load the first mipmap level */
        if (DDS_FOURCC == header.ddspf.dwFlags)
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                               "Compressed texture format not supported, converting to a default texture format.");

            if (header.dwPitchOrLinearSize == 0)
            {
                RwUInt32 shift, blocksX, blocksY, totalBlocks;

                if (D3DFMT_DXT1 == header.ddspf.dwFourCC)
                {
                    shift = 3; /* 64Bits / 8 = 8. 2^3 = 8 */
                }
                else
                {
                    shift = 4; /* 128Bits / 8 = 16. 2^4 = 16 */
                }

                blocksX = header.dwWidth >> 2;
                if (blocksX == 0)
                {
                    blocksX = 1;
                }

                blocksY = header.dwHeight >> 2;
                if (blocksY == 0)
                {
                    blocksY = 1;
                }

                totalBlocks = blocksX * blocksY;

                size = totalBlocks << shift;
            }
            else
            {
                size = header.dwPitchOrLinearSize;
            }
        }
        else
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8RasterStreamReadDDS",
                               "Texture format or dimensions not supported, converting to a default texture.");

            if (header.dwHeaderFlags & DDS_HEADER_FLAGS_LINEARSIZE)
            {
                size = header.dwPitchOrLinearSize;

                if (header.ddspf.dwRGBBitCount == 24)
                {
                    pitch = header.dwWidth * 3;
                }
                else if (0x00ff0000 == header.ddspf.dwRBitMask)
                {
                    pitch = header.dwWidth * 4;
                }
                else
                {
                    pitch = header.dwWidth * 2;
                }
            }
            else
            {
                if (header.dwPitchOrLinearSize == 0)
                {
                    if (header.ddspf.dwRGBBitCount == 24)
                    {
                        header.dwPitchOrLinearSize = header.dwWidth * 3;
                    }
                    else if (0x00ff0000 == header.ddspf.dwRBitMask)
                    {
                        header.dwPitchOrLinearSize = header.dwWidth * 4;
                    }
                    else
                    {
                        header.dwPitchOrLinearSize = header.dwWidth * 2;
                    }
                }

                size = header.dwPitchOrLinearSize * header.dwHeight;

                pitch = header.dwPitchOrLinearSize;
            }
        }

        pixels = (RwUInt8 *)RwMalloc(size,
            rwID_IMAGE | rwMEMHINTDUR_FUNCTION);

        if (RwStreamRead(stream, (void *)pixels, size) != size)
        {
            RwFree(pixels);

            RWRETURN(NULL);
        }

        /* Create an image */
        image = RwImageCreate(header.dwWidth, header.dwHeight, 32);

        if (NULL == image)
        {
            RwFree(pixels);

            RWRETURN(NULL);
        }

        RwImageAllocatePixels(image);

        /* Convert the dds to 32 bits */
        if (DDS_FOURCC == header.ddspf.dwFlags)
        {
            switch(header.ddspf.dwFourCC)
            {
                case D3DFMT_DXT1:
                    if (_rwD3D8DecompressDXT1(header.dwWidth, header.dwHeight, (RwUInt32 *)image->cpPixels, (const RwUInt16 *)pixels))
                    {
                        rasFormat = rwRASTERFORMAT1555;
                    }
                    else
                    {
                        rasFormat = rwRASTERFORMAT565;
                    }
                    break;

                case D3DFMT_DXT2:
                case D3DFMT_DXT3:
                    if (_rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4))
                    {
                        rasFormat = rwRASTERFORMAT4444;
                    }
                    else
                    {
                        rasFormat = rwRASTERFORMAT1555;
                    }

                    _rwD3D8DecompressDXT3(header.dwWidth, header.dwHeight, (RwUInt32 *)image->cpPixels, (const RwUInt16 *)pixels);
                    break;

                case D3DFMT_DXT4:
                case D3DFMT_DXT5:
                    if (_rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4))
                    {
                        rasFormat = rwRASTERFORMAT4444;
                    }
                    else
                    {
                        rasFormat = rwRASTERFORMAT1555;
                    }

                    _rwD3D8DecompressDXT5(header.dwWidth, header.dwHeight, (RwUInt32 *)image->cpPixels, (const RwUInt16 *)pixels);
                    break;
            }
        }
        else if (DDS_RGB == header.ddspf.dwFlags)
        {
            RwRGBA          *dstPixel;
            RwUInt32        j, i;

            if (header.ddspf.dwRGBBitCount == 24)
            {
                const RwUInt8  *srcPixel;

                if (_rwD3D8CheckValidTextureFormat(D3DFMT_X8R8G8B8))
                {
                    rasFormat = rwRASTERFORMAT888;
                }
                else
                {
                    rasFormat = rwRASTERFORMAT565;
                }

                for (j = 0; j < header.dwHeight; j++)
                {
                    srcPixel = (const RwUInt8 *)(pixels + (pitch * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    for (i = 0; i < header.dwWidth; i++)
                    {
                        dstPixel->red   = srcPixel[2];
                        dstPixel->green = srcPixel[1];
                        dstPixel->blue  = srcPixel[0];
                        dstPixel->alpha = 0xFF;

                        srcPixel += 3;
                        dstPixel++;
                    }
                }
            }
            else if (0x00ff0000 == header.ddspf.dwRBitMask)
            {
                const RwUInt32  *srcPixel;

                if (_rwD3D8CheckValidTextureFormat(D3DFMT_X8R8G8B8))
                {
                    rasFormat = rwRASTERFORMAT888;
                }
                else
                {
                    rasFormat = rwRASTERFORMAT565;
                }

                for (j = 0; j < header.dwHeight; j++)
                {
                    srcPixel = (const RwUInt32 *)(pixels + (pitch * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    for (i = 0; i < header.dwWidth; i++)
                    {
                        const RwUInt32 pixIn = *srcPixel;

                        srcPixel++;

                        dstPixel->alpha = 0xFF;
                        dstPixel->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
                        dstPixel->green = (RwUInt8)((pixIn >> 8) & 0xFF);
                        dstPixel->blue  = (RwUInt8)((pixIn) & 0xFF);

                        dstPixel++;
                    }
                }
            }
            else if (0x0000f800 == header.ddspf.dwRBitMask)
            {
                const RwUInt16  *srcPixel;

                rasFormat = rwRASTERFORMAT565;

                for (j = 0; j < header.dwHeight; j++)
                {
                    srcPixel = (const RwUInt16 *)(pixels + (pitch * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    for (i = 0; i < header.dwWidth; i++)
                    {
                        const RwUInt16 pixIn = *srcPixel;

                        dstPixel->alpha = 0xFF;
                        dstPixel->red   = (RwUInt8)((pixIn >> 8) & 0xF8);
                        dstPixel->green = (RwUInt8)((pixIn >> 3) & 0xFC);
                        dstPixel->blue  = (RwUInt8)((pixIn << 3) & 0xF8);

                        srcPixel++;
                        dstPixel++;
                    }
                }
            }
        }
        else if (DDS_RGBA == header.ddspf.dwFlags)
        {
            RwRGBA          *dstPixel;
            RwUInt32        j, i;

            if (0x00ff0000 == header.ddspf.dwRBitMask)
            {
                const RwUInt32  *srcPixel;

                if (_rwD3D8CheckValidTextureFormat(D3DFMT_A8R8G8B8))
                {
                    rasFormat = rwRASTERFORMAT8888;
                }
                else if (_rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4))
                {
                    rasFormat = rwRASTERFORMAT4444;
                }
                else
                {
                    rasFormat = rwRASTERFORMAT1555;
                }

                for (j = 0; j < header.dwHeight; j++)
                {
                    srcPixel = (const RwUInt32 *)(pixels + (pitch * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    for (i = 0; i < header.dwWidth; i++)
                    {
                        const RwUInt32 pixIn = *srcPixel;

                        srcPixel++;

                        dstPixel->red   = (RwUInt8)((pixIn >> 16) & 0xFF);
                        dstPixel->green = (RwUInt8)((pixIn >> 8) & 0xFF);
                        dstPixel->blue  = (RwUInt8)((pixIn) & 0xFF);
                        dstPixel->alpha = (RwUInt8)(pixIn >> 24);

                        dstPixel++;
                    }
                }
            }
            else if (0x00007c00 == header.ddspf.dwRBitMask)
            {
                const RwUInt16  *srcPixel;

                rasFormat = rwRASTERFORMAT1555;

                for (j = 0; j < header.dwHeight; j++)
                {
                    srcPixel = (const RwUInt16 *)(pixels + (pitch * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    for (i = 0; i < header.dwWidth; i++)
                    {
                        const RwUInt16 pixIn = *srcPixel;

                        if ((pixIn >> 8) & 0x80)
                        {
                            dstPixel->alpha = 0xFF;
                        }
                        else
                        {
                            dstPixel->alpha = 0x00;
                        }

                        dstPixel->red   = (RwUInt8)((pixIn >> 7) & 0xF8);
                        dstPixel->green = (RwUInt8)((pixIn >> 2) & 0xF8);
                        dstPixel->blue  = (RwUInt8)((pixIn << 3) & 0xF8);

                        srcPixel++;
                        dstPixel++;
                    }
                }
            }
            else if (0x00000f00 == header.ddspf.dwRBitMask)
            {
                const RwUInt16  *srcPixel;

                if (_rwD3D8CheckValidTextureFormat(D3DFMT_A4R4G4B4))
                {
                    rasFormat = rwRASTERFORMAT4444;
                }
                else
                {
                    rasFormat = rwRASTERFORMAT1555;
                }

                for (j = 0; j < header.dwHeight; j++)
                {
                    srcPixel = (const RwUInt16 *)(pixels + (pitch * j));
                    dstPixel = (RwRGBA *)(image->cpPixels + (image->stride * j));

                    for (i = 0; i < header.dwWidth; i++)
                    {
                        const RwUInt16 pixIn = *srcPixel;

                        dstPixel->alpha = (RwUInt8)((pixIn >> 8) & 0xF0);
                        dstPixel->red   = (RwUInt8)((pixIn >> 4) & 0xF0);
                        dstPixel->green = (RwUInt8)((pixIn >> 0) & 0xF0);
                        dstPixel->blue  = (RwUInt8)((pixIn << 4) & 0xF0);

                        srcPixel++;
                        dstPixel++;
                    }
                }
            }
        }

        RwFree(pixels);

        if (mipMapped)
        {
            mipMapped |= rwRASTERFORMATAUTOMIPMAP;
        }

        /* Create a raster */
        raster = RwRasterCreate(header.dwWidth, header.dwHeight, 0,
                                rwRASTERTYPETEXTURE | rasFormat | mipMapped);

        if (NULL == raster)
        {
            RwImageDestroy(image);

            RWRETURN(NULL);
        }

        /* Check dimensions */
        if (RwImageGetWidth(image) != RwRasterGetWidth(raster) ||
            RwImageGetHeight(image) != RwRasterGetHeight(raster))
        {
            RwImage *resampledimage;

            resampledimage = RwImageCreateResample(image,
                                                   RwRasterGetWidth(raster),
                                                   RwRasterGetHeight(raster));

            RwImageDestroy(image);

            image = resampledimage;
        }

        /* Convert image to raster */
        RwRasterSetFromImage(raster, image);

        RwImageDestroy(image);
    }

    RWRETURN(raster);
}

/****************************************************************************
 rwD3D8DXTTextureRead

 On entry   :
 On exit    :
 */
static RwTexture *
rwD3D8DXTTextureRead(const RwChar *name, const RwChar *fullPathName)
{
    RwStream        *stream;
    RwRaster        *raster;
    RwTexture       *texture;

    RWFUNCTION(RWSTRING("rwD3D8DXTTextureRead"));

    if (RwFexist(fullPathName) == FALSE)
    {
        RWRETURN(NULL);
    }

    stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, fullPathName);
    if (!stream)
    {
        RWRETURN(NULL);
    }

    /* Create raster from stream */
    raster = RwD3D8RasterStreamReadDDS(stream);

    /* Close the stream */
    RwStreamClose(stream, NULL);

    /* Check raster */
    if (raster == NULL)
    {
        RWRETURN(NULL);
    }

    /* Create the texture */
    texture = RwTextureCreate(raster);
    if (!texture)
    {
        RWASSERT(NULL != texture);
        RWRETURN(NULL);
    }

    if (RwRasterGetFormat(raster) & rwRASTERFORMATMIPMAP)
    {
        RwTextureSetFilterMode(texture, rwFILTERLINEARMIPLINEAR);
    }
    else
    {
        RwTextureSetFilterMode(texture, rwFILTERLINEAR);
    }

    /* Set the textures name */
    RwTextureSetName(texture, name);

    RWRETURN(texture);
}

/**
 * \ingroup rwtextured3d8
 * \ref RwD3D8DDSTextureRead creates an \ref RwTexture from a DDS file.
 * This function currently supports all different flavors of DDS files,
 * including DXT compressed textures, cube textures and mipmaps. The
 * function will append a .dds extension to the filename. For example,
 * the following line of code will search for test.dds.dds:
 \code RwD3D8DDSTextureRead( "test.dds", NULL );
 \endcode
 *
 * If you try to load a texture with a format not supported by the video
 * card drivers, RWD3D8 will automatically convert the texture to the most
 * suitable texture format.
 *
 * For more information on DDS files, DXT compressed textures and cube textures,
 * see the D3D8 SDK documentation.
 *
 * \param name     Name of the DDS file to be opened. Can be an absolute path
 *                 or just a file name. In this case, the function will search
 *                 for the file in the search path.
 * \param maskname Currently unused
 *
 * \return The \ref RwTexture on success or a NULL pointer on error.
 *
 * \see RwD3D8RasterStreamReadDDS
 * \see RwTextureRead
 */
RwTexture *
RwD3D8DDSTextureRead(const RwChar *name, const RwChar *maskname __RWUNUSED__)
{
    RwChar      fullPathName[256];
    RwChar      *pathElement;
    RwTexture   *texture;

    RWAPIFUNCTION(RWSTRING("RwD3D8DDSTextureRead"));
    RWASSERT(NULL != name);

    pathElement = RwImageGetPath();

    /*
     * If we have an absolute path, ignore the search directory.
     * If we have no search path, use the current directory.
     * Either way, just copy the name.
     */
    if ((_rwpathisabsolute(name) == TRUE) ||
        (pathElement == NULL) ||
        (pathElement[0] == '\0'))
    {
        const RwChar    *curChar;

        curChar = name + rwstrlen(name);

        do
        {
            curChar--;
        }
        while (curChar >= name &&
               '\\' != *curChar);

        rwstrcpy(fullPathName, name);
        rwstrcat(fullPathName, ".dds");

        /* Try to load the texture */
        RWRETURN(rwD3D8DXTTextureRead(curChar + 1, fullPathName));
    }
    else
    {
        /* While we have a search path to try */
        while (pathElement && pathElement[0] != '\0')
        {
            RwChar      *nextPathElement = rwstrchr(pathElement, rwSEARCHPATH_SEPARATOR);
            RwInt32     pathElementLength;

            nextPathElement = rwstrchr(pathElement, rwSEARCHPATH_SEPARATOR);
            if (nextPathElement)
            {
                /* Length of pathstring (in bytes) */
                pathElementLength = (RwUInt8 *)nextPathElement - (RwUInt8*)pathElement;
                nextPathElement++;  /* Skip the separator */
            }
            else
            {
                pathElementLength = rwstrlen(pathElement) * sizeof(RwChar);
            }

            /* Full path + full name */
            memcpy(fullPathName, pathElement, pathElementLength);
            rwstrcpy((RwChar *)(((RwUInt8 *)fullPathName) + pathElementLength), name);
            rwstrcat(fullPathName, ".dds");

            /* Try to load the texture */
            texture = rwD3D8DXTTextureRead(name, fullPathName);
            if (texture)
            {
                RWRETURN(texture);
            }

            /* On to the next path */
            pathElement = nextPathElement;
        }
    }

    RWRETURN(NULL);
}

/**
 * \ingroup rwrasterd3d8
 * \ref RwD3D8RasterIsCompressed is used to check if a raster is compressed or not.
 *
 * \param raster   Const pointer to the raster that is to be checked.
 *
 * \return Returns TRUE if the raster is compressed, FALSE otherwise.
 *
 * \see RwD3D8DDSTextureRead
 */
RwBool
RwD3D8RasterIsCompressed(const RwRaster *raster)
{
    const _rwD3D8RasterExt  *rasExt;

    RWAPIFUNCTION(RWSTRING("RwD3D8RasterIsCompressed"));
    RWASSERT(NULL != raster);

    rasExt = RASTEREXTFROMCONSTRASTER(raster);

    RWRETURN(rasExt->dxtFormat != 0);
}
