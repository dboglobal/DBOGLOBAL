/***************************************************************************
 *                                                                         *
 * Module  : drvmodel.h (null)                                             *
 *                                                                         *
 * Purpose : Driver model description (device specific vertices/polys)     *
 *                                                                         *
 **************************************************************************/

/* RWPUBLIC */
#ifndef NULL_DRVMODEL_H
#define NULL_DRVMODEL_H

/**
 * \defgroup drivernull NULL
 * \ingroup rwcore
 *
 * NULL drivers
 */

/**
 * \defgroup nullunsupported Unsupported NULL Functions
 * \ingroup drivernull
 *
 * Unsupported functions
 */

/* RWPUBLICEND */

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "bacolor.h"

/* RWPUBLIC */

/****************************************************************************
 Defines
 */

#if (!defined(_MSC_VER))
#define IsBadReadPtr(_lp, _ucb ) \
  (NULL != (_lp))
#else /* (defined(_MSC_VER)) */
/*
 * IsBadReadPtr() is defined in
 * /Program Files/Microsoft Visual Studio/VC98/Include/WINBASE.H
 */
#endif /* (defined(_MSC_VER)) */

/* Set true depth information (for fogging, eg) */
#define RwIm2DVertexSetCameraX(vert, camx)      ((vert)->camera.x = (camx))
#define RwIm2DVertexSetCameraY(vert, camy)      ((vert)->camera.y = (camy))
#define RwIm2DVertexSetCameraZ(vert, camz)      ((vert)->camera.z = (camz))
#define RwIm2DVertexSetRecipCameraZ(vert, recipz)     ((vert)->recipZ = (recipz))

#define RwIm2DVertexGetCameraX(vert)            ((vert)->camera.x)
#define RwIm2DVertexGetCameraY(vert)            ((vert)->camera.y)
#define RwIm2DVertexGetCameraZ(vert)            ((vert)->camera.z)
#define RwIm2DVertexGetRecipCameraZ(vert)             ((vert)->recipZ)

/* Set screen space coordinates in a device vertex */
#define RwIm2DVertexSetScreenX(vert, scrnx)     ((vert)->screen.x = (scrnx))
#define RwIm2DVertexSetScreenY(vert, scrny)     ((vert)->screen.y = (scrny))
#define RwIm2DVertexSetScreenZ(vert, scrnz)     ((vert)->screen.z = (scrnz))
#define RwIm2DVertexGetScreenX(vert)            ((vert)->screen.x)
#define RwIm2DVertexGetScreenY(vert)            ((vert)->screen.y)
#define RwIm2DVertexGetScreenZ(vert)            ((vert)->screen.z)

/* Set texture coordinates in a device vertex */
#define RwIm2DVertexSetU(vert, u, recipz)       ((vert)->texU = (u))
#define RwIm2DVertexSetV(vert, v, recipz)       ((vert)->texV = (v))
#define RwIm2DVertexGetU(vert)                  ((vert)->texU)
#define RwIm2DVertexGetV(vert)                  ((vert)->texV)

/* Modify the luminance stuff */
#define RwIm2DVertexSetRealRGBA(vert, r, g, b, a)   \
        {                                           \
            ((vert)->col.red   = (r));              \
            ((vert)->col.green = (g));              \
            ((vert)->col.blue  = (b));              \
            ((vert)->col.alpha = (a));              \
        }
#define RwIm2DVertexSetIntRGBA(vert, r, g, b, a)    \
        {                                           \
            ((vert)->col.red   = (RwReal)(r));      \
            ((vert)->col.green = (RwReal)(g));      \
            ((vert)->col.blue  = (RwReal)(b));      \
            ((vert)->col.alpha = (RwReal)(a));      \
        }


#define RwIm2DVertexGetRed(vert)                    \
               (RwInt32FromRealMacro((vert)->col.red))
#define RwIm2DVertexGetGreen(vert)                  \
             (RwInt32FromRealMacro((vert)->col.green))
#define RwIm2DVertexGetBlue(vert)                   \
              (RwInt32FromRealMacro((vert)->col.blue))
#define RwIm2DVertexGetAlpha(vert)                  \
             (RwInt32FromRealMacro((vert)->col.alpha))

#define RwIm2DVertexCopyRGBA(dst, src)              \
        {                                           \
            ((dst)->col.red   = (src)->col.red);    \
            ((dst)->col.green = (src)->col.green);  \
            ((dst)->col.blue  = (src)->col.blue);   \
            ((dst)->col.alpha = (src)->col.alpha);  \
        }

/* Clipper stuff */
#define RwIm2DVertexClipRGBA(out, interp, near, far)                                        \
        {                                                                                   \
            (out)->col.red   = RWSHADCLIP((interp), (near)->col.red,   (far)->col.red);     \
            (out)->col.green = RWSHADCLIP((interp), (near)->col.green, (far)->col.green);   \
            (out)->col.blue  = RWSHADCLIP((interp), (near)->col.blue,  (far)->col.blue);    \
            (out)->col.alpha = RWSHADCLIP((interp), (near)->col.alpha, (far)->col.alpha);   \
        }

/****************************************************************************
 Global Types
 */

/* Define types used */

typedef struct _RwNull2DVertex RwNull2DVertex;
struct _RwNull2DVertex
{
    RwV3d           screen;  /* Screen space coordinate */
    RwRGBAReal      col;  /* Vertex color */
    RwReal          texU, texV;  /* Texture coordinates */
    RwReal          recipZ;  /* Reciprocal Z */
    RwV3d           camera;  /* Camera space coordinate */
};

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertex
 * Typedef for a RenderWare Graphics Immediate Mode 2D Vertex
 */
typedef RwNull2DVertex  RwIm2DVertex;

/**
 * \ingroup rwcoregeneric
 * \ref RxVertexIndex
 * Typedef for a RenderWare Graphics PowerPipe Immediate Mode Vertex
 */
typedef RwUInt16        RxVertexIndex;

/**
 * \ingroup rwcoregeneric
 * \ref RwImVertexIndex
 * Typedef for a RenderWare Graphics Immediate Mode Vertex
 */
typedef RxVertexIndex   RwImVertexIndex;

#endif /* NULL_DRVMODEL_H */
/* RWPUBLICEND */
