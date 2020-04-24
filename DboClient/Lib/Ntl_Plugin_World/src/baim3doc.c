/*
 *
 * 3D immediate mode support
 * Immediate mode is for supplementing the retained mode rendering support
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

/****************************************************************************
 *
 * This is for documentation purposes only, so must be '#if 0'ed out
 *
 */

#if (defined(DOXYGEN))

/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"


#include "drvmodel.h"
#include "baimmedi.h"

#include "baim3doc.h"

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertex
 * Typedef for a RenderWare Graphics Immediate Mode 2D Vertex
 */
typedef RwIm2DVertex        RwIm2DVertex;

/**
 * \ingroup rwim2dvertex
 * \ref RxVertexIndex
 *
 * Typedef for a RenderWare Graphics PowerPipe Immediate
 * Mode Vertex
 */
typedef RxVertexIndex       RxVertexIndex;

/**
 * \ingroup rwim2dvertex
 * \ref RwImVertexIndex
 * Typedef for a RenderWare Graphics Immediate Mode Vertex.
 */
typedef RwImVertexIndex     RwImVertexIndex;

/*    ---==* RwIm3DVertex *==---      */


/**
 * \ingroup rwim3dvertex
 * \ref RwIm3DVertexGetNext 
 * is used to retrieve the next vertex following
 * the specified one in an array of 3D immediate mode vertices.
 *
 * \param vertex  A pointer to the 3D immediate mode vertex.
 *
 * \return Returns a pointer to next vertex in the array.
 *
 * \see RwIm3DVertexSetPos
 * \see RwIm3DVertexSetRGBA
 * \see RwIm3DVertexSetU
 * \see RwIm3DVertexSetV
 * \see RwIm3DGetRenderPipeline
 * \see RwIm3DTransform
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderTriangle
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwRenderStateGet
 *
 */
RwIm3DVertex       *
RwIm3DVertexGetNext(RwIm3DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DVertexGetNext"));

    RWRETURN(FALSE);
}

/**
 * \ingroup rwim3dvertex
 * \ref RwIm3DVertexSetPos is used to define the world space position
 * of the specified 3D immediate mode vertex.
 *
 * \param vertex  A pointer to the 3D immediate mode vertex
 * \param worldX  An \ref RwReal value equal to the world space x-coordinate
 * \param worldY  An \ref RwReal value equal to the world space y-coordinate
 * \param worldZ  An \ref RwReal value equal to the world space z-coordinate.
 *
 * \return None.
 *
 * \see RwIm3DVertexGetNext
 * \see RwIm3DVertexSetRGBA
 * \see RwIm3DVertexSetU
 * \see RwIm3DVertexSetV
 * \see RwIm3DGetRenderPipeline
 * \see RwIm3DTransform
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderTriangle
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwRenderStateGet
 *
 */
void 
RwIm3DVertexSetPos(RwIm3DVertex * vertex, RwReal worldX,
                   RwReal worldY, RwReal worldZ)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DVertexSetPos"));

    RWRETURNVOID();
}
/**
 * \ingroup rwim3dvertex
 * \ref RwIm3DVertexGetPos
 * is used to get the position of the specified
 * 3D immediate mode vertex. 
 *
 * \param vertex  A pointer to the 3D immediate mode vertex.
 *
 * \return Returns a pointer to an \ref RwV3d value describing the vertex's position.
 *
 * \see RwIm3DVertexGetNext
 * \see RwIm3DVertexSetPos
 * \see RwIm3DVertexSetRGBA
 * \see RwIm3DVertexSetV
 * \see RwIm3DGetRenderPipeline
 * \see RwIm3DTransform
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderTriangle
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwRenderStateGet
 *
 */
RwV3d              *
RwIm3DVertexGetPos(RwIm3DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DVertexGetPos"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwim3dvertex
 * \ref RwIm3DVertexSetU
 * is used to define the texture u-coordinate of
 * the specified 3D immediate mode vertex.
 *
 * \param vertex  A pointer to the 3D immediate mode vertex
 * \param u  An \ref RwReal value equal to the texture u-coordinate.
 *
 * \return None.
 *
 * \see RwIm3DVertexGetNext
 * \see RwIm3DVertexSetPos
 * \see RwIm3DVertexSetRGBA
 * \see RwIm3DVertexSetV
 * \see RwIm3DGetRenderPipeline
 * \see RwIm3DTransform
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderTriangle
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwRenderStateGet
 *
 */
void
RwIm3DVertexSetU(RwIm3DVertex * vertex, RwReal u)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DVertexSetU"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim3dvertex
 * \ref RwIm3DVertexSetV
 * is used to define the texture v-coordinate of
 * the specified 3D immediate mode vertex.
 *
 * \param vertex  A pointer to the 3D immediate mode vertex
 * \param v  An \ref RwReal value equal to the texture v-coordinate.
 *
 * \return None.
 *
 * \see RwIm3DVertexGetNext
 * \see RwIm3DVertexSetPos
 * \see RwIm3DVertexSetRGBA
 * \see RwIm3DVertexSetU
 * \see RwIm3DGetRenderPipeline
 * \see RwIm3DTransform
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderTriangle
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwRenderStateGet
 *
 */
void
RwIm3DVertexSetV(RwIm3DVertex * vertex, RwReal v)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DVertexSetV"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim3dvertex
 * \ref RwIm3DVertexSetNormal
 * is used to define the normal of
 * the specified 3D immediate mode vertex.
 *
 * \param vertex  A pointer to the 3D immediate mode vertex
 * \param x  An \ref RwReal value equal to the x-coordinate of the normal
 * \param y  An \ref RwReal value equal to the y-coordinate of the normal
 * \param z  An \ref RwReal value equal to the z-coordinate of the normal.
 *
 * \return None.
 *
 * \see RwIm3DVertexGetNext
 * \see RwIm3DVertexSetPos
 * \see RwIm3DVertexSetRGBA
 * \see RwIm3DGetRenderPipeline
 * \see RwIm3DTransform
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderTriangle
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwRenderStateGet
 *
 */
void
RwIm3DVertexSetNormal(RwIm3DVertex * vertex, RwReal x, RwReal y, RwReal z)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DVertexSetNormal"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim3dvertex
 * \ref RwIm3DVertexSetRGBA
 * is used to define the RGBA color of
 * the specified 3D immediate mode vertex. Each color component is specified
 * as an integer in the range 0 to 255.
 *
 * \param vertex  A pointer to the 3D immediate mode vertex
 * \param red    An \ref RwUInt8 value equal to the color's red component
 * \param green  An \ref RwUInt8 value equal to the color's green component
 * \param blue   An \ref RwUInt8 value equal to the color's blue component
 * \param alpha  An \ref RwUInt8 value equal to the color's alpha component.
 *
 * \return None.
 *
 * \see RwIm3DVertexGetNext
 * \see RwIm3DVertexSetPos
 * \see RwIm3DVertexSetU
 * \see RwIm3DVertexSetV
 * \see RwIm3DGetRenderPipeline
 * \see RwIm3DTransform
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderTriangle
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwRenderStateGet
 *
 */
void
RwIm3DVertexSetRGBA(RwIm3DVertex * vertex, RwUInt8 red, RwUInt8 green,
                    RwUInt8 blue, RwUInt8 alpha)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DVertexSetRGBA"));

    RWRETURNVOID();
}



/*    ---==* RWCAMERAVERTEX *==---      */


/**
 * \ingroup rwcameravertex
 * \page rwcameravertexoverview RwCameraVertex Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection cameravertexoverview Overview
 * 
 * An array of Camera Vertices is generated by the rendering pipeline's transformation 
 * and lighting stages. They represent the final, transformed vertices that are sent 
 * to the 3D graphics hardware. On platforms with software transform and lighting pipelines, 
 * the RwCameraVertex API provides a mechanism for accessing these vertices.
 *
 * \subsection clipflags ClipFlags
 *
 * Although the 'ClipFlags' datatype itself is not exposed, the RwCameraVertexGetClipFlags() 
 * function can be used as a simple boolean test to check if a vertex has been clipped. 
 * If the value returned is zero, the vertex is not clipped.
 *
 * \warning
 *
 * The RwCameraVertex API should NOT be used on platforms that support hardware transform 
 * and lighting.
 */

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexGetQuantity
 * is used to determine the number of
 * camera vertices produced as a result of executing the rendering pipeline.
 *
 * \return Returns an \ref RwInt32 value equal to the number of camera vertices.
 *
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
RwInt32
RwCameraVertexGetQuantity(void)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexGetQuantity"));

    RWRETURN(0);
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexGet
 * is used to retrieve the camera vertex
 * corresponding to the given array index.
 *
 * \param index  An \ref RwInt32 value equal to the index of the camera vertex.
 *
 * \return Returns a pointer to the camera vertex.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
RwCameraVertex     *
RwCameraVertexGet(RwInt32 index)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexGet"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexGetNext
 * is used to retrieve the next vertex following
 * the specified one in the array of camera vertices.
 *
 * \param vertex  A pointer to the camera vertex.
 *
 * \return Returns a pointer to the next camera vertex.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
RwCameraVertex     *
RwCameraVertexGetNext(RwCameraVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexGetNext"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexGetClipFlags
 * is used to retrieve the view frustum
 * clip flags for the specified camera vertex. Note the clip flags are only
 * valid after passing through the transform stage of the rendering pipeline.
 * The clip flags are returned packed into an RwUInt8 value.
 *
 * \param vertex  A pointer to the camera vertex.
 *
 * \return Returns an \ref RwUInt8 value equal to the view
 * frustum clip flags of the camera vertex.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
RwUInt8
RwCameraVertexGetClipFlags(RwCameraVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexGetClipFlags"));

    RWRETURN(0);
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexSetCameraPos
 * is used to define the camera space
 * position of the specified camera vertex.
 *
 * \param vertex  A pointer to the camera vertex
 * \param pos  A pointer to an \ref RwV3d value equal to the vertex camera position.
 *
 * \return None.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
void
RwCameraVertexSetCameraPos(RwCameraVertex * vertex, RwV3d * pos)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexSetCameraPos"));

    RWRETURNVOID();
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexGetCameraPos
 * is used to retrieve the camera space
 * position of the specified camera vertex.
 *
 * \param vertex  A pointer to a camera vertex.
 *
 * \return Returns a pointer to an \ref RwV3d value equal to the camera space
 * position of the vertex.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
RwV3d              *
RwCameraVertexGetCameraPos(RwCameraVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexGetCameraPos"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexSetU
 * is used to define the texture
 * u-coordinate of the specified camera vertex.
 *
 * \param vertex  A pointer to the camera vertex
 * \param u  An \ref RwReal value equal to the texture u-coordinate.
 *
 * \return None.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
void
RwCameraVertexSetU(RwCameraVertex * vertex, RwReal u)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexSetU"));

    RWRETURNVOID();
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexGetU
 * is used to retrieve the texture u-coordinate
 * of the specified camera vertex.
 *
 * \param vertex  A pointer to the camera vertex.
 *
 * \return Returns an \ref RwReal value equal to the texture u-coordinate.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
RwReal
RwCameraVertexGetU(RwCameraVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexGetU"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexSetV
 * is used to define the texture
 * v-coordinate of the specified camera vertex.
 *
 * \param vertex  A pointer to the camera vertex
 * \param v  An \ref RwReal value equal to the texture v-coordinate.
 *
 * \return None.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
void
RwCameraVertexSetV(RwCameraVertex * vertex, RwReal v)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexSetV"));

    RWRETURNVOID();
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexGetV
 * is used to retrieve the texture v-coordinate
 * of the specified camera vertex.
 *
 * \param vertex  A pointer to the camera vertex.
 *
 * \return Returns an \ref RwReal value equal to the texture v-coordinate.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */
RwReal
RwCameraVertexGetV(RwCameraVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexGetV"));

    RWRETURN((RwReal) 0);
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexSetRGBA
 * is used to define the RGBA color of
 * the specified camera vertex.
 *
 * \param vertex  A pointer to the camera vertex
 * \param red  An \ref RwReal value equal to the color's red component
 * \param green  An \ref RwReal value equal to the color's green component
 * \param blue  An \ref RwReal value equal to the color's blue component
 * \param alpha  An \ref RwReal value equal to the color's alpha component.
 *
 * \return None.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexAddRGBA
 * \see RwCameraVertexGetRGBA
 *
 */

void
RwCameraVertexSetRGBA(RwCameraVertex * vertex, RwReal red, RwReal green,
                      RwReal blue, RwReal alpha)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexSetRGBA"));

    RWRETURNVOID();
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexAddRGBA
 * is used to increment the RGBA color of
 * the specified camera vertex by the given components.
 *
 * \param vertex  A pointer to the camera vertex
 * \param red  An \ref RwReal value equal to the red component
 * \param green  An \ref RwReal value equal to the green component
 * \param blue  An \ref RwReal value equal to the blue component
 * \param alpha  An \ref RwReal value equal to the alpha component.
 *
 * \return None.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexGetRGBA
 */

void
RwCameraVertexAddRGBA(RwCameraVertex * vertex, RwReal red, RwReal green,
                      RwReal blue, RwReal alpha)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexAddRGBA"));

    RWRETURNVOID();
}

/**
 * \ingroup rwcameravertex
 * \ref RwCameraVertexGetRGBA
 * is used to retrieve the color of the
 * specified camera vertex.
 *
 * \param vertex  A pointer to the camera vertex.
 *
 * \return Returns a pointer to an \ref RwRGBAReal value equal to the color of the
 * vertex.
 *
 * \see RwCameraVertexGetQuantity
 * \see RwCameraVertexGet
 * \see RwCameraVertexGetNext
 * \see RwCameraVertexGetClipFlags
 * \see RwCameraVertexSetCameraPos
 * \see RwCameraVertexGetCameraPos
 * \see RwCameraVertexSetU
 * \see RwCameraVertexGetU
 * \see RwCameraVertexSetV
 * \see RwCameraVertexGetV
 * \see RwCameraVertexSetRGBA
 * \see RwCameraVertexAddRGBA
 *
 */
RwRGBAReal         *
RwCameraVertexGetRGBA(RwCameraVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwCameraVertexGetRGBA"));

    RWRETURN(NULL);
}



/*    ---==* RwIm2DCameraVertex *==---      */





/**
 * \ingroup rwim2dcameravertex
 * \ref RwIm2DCameraVertexSetU
 * is used to define the texture u-coordinate
 * of the specified device and corresponding camera vertices.
 *
 * \param devvert  A pointer to the device vertex
 * \param camvert  A pointer to the camera vertex
 * \param u  An \ref RwReal value equal to the texture u-coordinate
 * \param reciprocalZ  An \ref RwReal value equal to the
 * reciprocal z-coordinate (for perspective correction purposes).
 *
 * \return None.
 *
 * \see RwIm2DCameraVertexSetV
 * \see RwIm2DVertexSetU
 * \see RwIm2DVertexSetV
 * \see RwCameraVertexSetU
 * \see RwCameraVertexSetV
 */
void
RwIm2DCameraVertexSetU(RwIm2DVertex * devvert, RwCameraVertex * camvert,
                       RwReal u, RwReal reciprocalZ)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DCameraVertexSetU"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dcameravertex
 * \ref RwIm2DCameraVertexSetV
 * is used to define the texture v-coordinate
 * of the specified device and corresponding camera vertices.
 *
 * \param devvert  A pointer to the device vertex
 * \param camvert  A pointer to the camera vertex
 * \param v  An \ref RwReal value equal to the texture v-coordinate
 * \param reciprocalZ  An \ref RwReal value equal to the reciprocal z-coordinate (for perspective
 * correction purposes).
 *
 * \return None.
 *
 * \see RwIm2DCameraVertexSetU
 * \see RwIm2DVertexSetU
 * \see RwIm2DVertexSetV
 * \see RwCameraVertexSetV
 * \see RwCameraVertexSetU
 */
void
RwIm2DCameraVertexSetV(RwIm2DVertex * devvert, RwCameraVertex * camvert,
                       RwReal v, RwReal reciprocalZ)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DCameraVertexSetV"));

    RWRETURNVOID();
}



/*    ---==* RwIm2DVertex *==---      */


/**
 * \ingroup rwim2dvertex
 * \page rwim2dvertexoverview RwIm2DVertex Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection im2dvertexoverview Overview
 * 
 * While rendering 2D Immediate Mode geometry, a set of 
 * RwIm2DVertex objects is produced. 
 * The functions documented here are exposed to allow access to 
 * these vertices. 
 * The vertices have been transformed to device space.
 * 
 */


RwInt32
RwIm2DVertexGetQuantity(void)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetQuantity"));

    RWRETURN(0);
}

RwIm2DVertex       *
RwIm2DVertexGet(RwInt32 index)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGet"));

    RWRETURN(NULL);
}

RwIm2DVertex       *
RwIm2DVertexGetNext(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetNext"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetCameraX
 * is used to define the camera x-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param cameraX  An \ref RwReal value equal to the camera x-coordinate.
 *
 * \return None.
 *
 * \see RwIm2DVertexSetCameraY
 * \see RwIm2DVertexSetCameraZ
 * \see RwIm2DVertexSetRecipCameraZ
 * \see RwIm2DVertexGetCameraX
 * \see RwIm2DVertexGetCameraY
 * \see RwIm2DVertexGetCameraZ
 * \see RwIm2DVertexGetRecipCameraZ
 *
 */
void
RwIm2DVertexSetCameraX(RwIm2DVertex * vertex, RwReal cameraX)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetCameraX"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetCameraY
 * is used to define the camera y-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param cameraY  An \ref RwReal value equal to the camera y-coordinate.
 *
 * \return None.
 *
 * \see RwIm2DVertexSetCameraX
 * \see RwIm2DVertexSetCameraZ
 * \see RwIm2DVertexSetRecipCameraZ
 * \see RwIm2DVertexGetCameraX
 * \see RwIm2DVertexGetCameraY
 * \see RwIm2DVertexGetCameraZ
 * \see RwIm2DVertexGetRecipCameraZ
 *
 */
void
RwIm2DVertexSetCameraY(RwIm2DVertex * vertex, RwReal cameraY)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetCameraY"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetCameraZ
 * is used to define the camera z-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param cameraZ  An \ref RwReal value equal to the camera z-coordinate.
 *
 * \return None.
 *
 * \see RwIm2DVertexSetCameraX
 * \see RwIm2DVertexSetCameraZ
 * \see RwIm2DVertexSetRecipCameraZ
 * \see RwIm2DVertexGetCameraX
 * \see RwIm2DVertexGetCameraY
 * \see RwIm2DVertexGetCameraZ
 * \see RwIm2DVertexGetRecipCameraZ
 *
 */
void
RwIm2DVertexSetCameraZ(RwIm2DVertex * vertex, RwReal cameraZ)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetCameraZ"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetRecipCameraZ
 * is used to define the reciprocal of
 * the camera z-coordinate of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param reciprocalZ  An \ref RwReal value equal to the reciprocal camera z-coordinate.
 *
 * \return None.
 *
 * \see RwIm2DVertexSetCameraX
 * \see RwIm2DVertexSetCameraY
 * \see RwIm2DVertexSetCameraZ
 * \see RwIm2DVertexGetCameraX
 * \see RwIm2DVertexGetCameraY
 * \see RwIm2DVertexGetCameraZ
 * \see RwIm2DVertexGetRecipCameraZ
 *
 */
void
RwIm2DVertexSetRecipCameraZ(RwIm2DVertex * vertex, RwReal reciprocalZ)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetRecipCameraZ"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetCameraX
 * is used to retrieve the camera x-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the immediate mode 2D vertex.
 *
 * \return Returns an \ref RwReal value equal to the camera x-coordinate.
 *
 * \see RwIm2DVertexGetCameraY
 * \see RwIm2DVertexGetCameraZ
 * \see RwIm2DVertexGetRecipCameraZ
 * \see RwIm2DVertexSetCameraX
 * \see RwIm2DVertexSetCameraY
 * \see RwIm2DVertexSetCameraZ
 * \see RwIm2DVertexSetRecipCameraZ
 *
 */
RwReal
RwIm2DVertexGetCameraX(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetCameraX"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetCameraY
 * is used to retrieve the camera y-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwReal value equal to the camera y-coordinate.
 *
 * \see RwIm2DVertexGetCameraX
 * \see RwIm2DVertexGetCameraZ
 * \see RwIm2DVertexGetRecipCameraZ
 * \see RwIm2DVertexSetCameraX
 * \see RwIm2DVertexSetCameraY
 * \see RwIm2DVertexSetCameraZ
 * \see RwIm2DVertexSetRecipCameraZ
 *
 */
RwReal
RwIm2DVertexGetCameraY(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetCameraY"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetCameraZ
 * is used to retrieve the camera
 * z-coordinate of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwReal value equal to the camera z-coordinate.
 *
 * \see RwIm2DVertexGetCameraX
 * \see RwIm2DVertexGetCameraY
 * \see RwIm2DVertexGetRecipCameraZ
 * \see RwIm2DVertexSetCameraX
 * \see RwIm2DVertexSetCameraY
 * \see RwIm2DVertexSetCameraZ
 * \see RwIm2DVertexSetRecipCameraZ
 *
 */
RwReal
RwIm2DVertexGetCameraZ(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetCameraZ"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetRecipCameraZ
 * is used to retrieve the reciprocal
 * camera z-coordinate of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwReal value equal to the reciprocal camera z-coordinate.
 *
 * \see RwIm2DVertexGetCameraX
 * \see RwIm2DVertexGetCameraY
 * \see RwIm2DVertexGetCameraZ
 * \see RwIm2DVertexSetCameraX
 * \see RwIm2DVertexSetCameraY
 * \see RwIm2DVertexSetCameraZ
 * \see RwIm2DVertexSetRecipCameraZ
 *
 */
RwReal
RwIm2DVertexGetRecipCameraZ(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetRecipCameraZ"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetScreenX
 * is used to define the screen x-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param screenX  An \ref RwReal value equal to the screen x-coordinate.
 *
 * \return None.
 *
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexSetScreenZ
 * \see RwIm2DVertexGetScreenX
 * \see RwIm2DVertexGetScreenY
 * \see RwIm2DVertexGetScreenZ
 *
 */
void
RwIm2DVertexSetScreenX(RwIm2DVertex * vertex, RwReal screenX)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetScreenX"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetScreenY
 * is used to define the screen y-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param screenY  An \ref RwReal value equal to the screen y-coordinate.
 *
 * \return None.
 *
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenZ
 * \see RwIm2DVertexGetScreenX
 * \see RwIm2DVertexGetScreenY
 * \see RwIm2DVertexGetScreenZ
 *
 */
void
RwIm2DVertexSetScreenY(RwIm2DVertex * vertex, RwReal screenY)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetScreenY"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetScreenZ
 * is used to define the screen z-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param screenZ  An \ref RwReal value equal to the screen z-coordinate.
 *
 * \return None.
 *
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexGetScreenX
 * \see RwIm2DVertexGetScreenY
 * \see RwIm2DVertexGetScreenZ
 *
 */
void
RwIm2DVertexSetScreenZ(RwIm2DVertex * vertex, RwReal screenZ)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetScreenZ"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetScreenX
 * is used to retrieve the screen
 * x-coordinate of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwReal value equal to the screen x-coordinate.
 *
 * \see RwIm2DVertexGetScreenY
 * \see RwIm2DVertexGetScreenZ
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexSetScreenZ
 *
 */
RwReal
RwIm2DVertexGetScreenX(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetScreenX"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetScreenY
 * is used to retrieve the screen
 * y-coordinate of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwReal value equal to the screen y-coordinate.
 *
 * \see RwIm2DVertexGetScreenX
 * \see RwIm2DVertexGetScreenZ
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexSetScreenZ
 *
 */
RwReal
RwIm2DVertexGetScreenY(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetScreenY"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetScreenZ
 * is used to retrieve the screen
 * z-coordinate of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwReal value equal to the screen z-coordinate.
 *
 * \see RwIm2DVertexGetScreenX
 * \see RwIm2DVertexGetScreenY
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexSetScreenZ
 *
 */
RwReal
RwIm2DVertexGetScreenZ(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetScreenZ"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetU
 * is used to define the texture u-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param u  An \ref RwReal value equal to the texture u-coordinate
 * \param reciprocalZ  An \ref RwReal value equal to the reciprocal z-coordinate (for perspective
 * correction purposes).
 *
 * \return None.
 *
 * \see RwIm2DVertexSetV
 * \see RwIm2DVertexGetU
 * \see RwIm2DVertexGetV
 *
 */
void
RwIm2DVertexSetU(RwIm2DVertex * vertex, RwReal u, RwReal reciprocalZ)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetU"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetV
 * is used to define the texture v-coordinate
 * of the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex
 * \param v  An \ref RwReal value equal to the texture v-coordinate
 * \param reciprocalZ  An \ref RwReal value equal to the reciprocal z-coordinate (for perspective
 * correction purposes).
 *
 * \return None.
 *
 * \see RwIm2DVertexSetU
 * \see RwIm2DVertexGetU
 * \see RwIm2DVertexGetV
 *
 */
void
RwIm2DVertexSetV(RwIm2DVertex * vertex, RwReal v, RwReal reciprocalZ)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetV"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetU
 * is used to retrieve the texture u-coordinate of
 * the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwReal value equal to the texture u-coordinate.
 *
 * \see RwIm2DVertexGetV
 * \see RwIm2DVertexSetU
 * \see RwIm2DVertexSetV
 *
 */
RwReal
RwIm2DVertexGetU(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetU"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetV
 * is used to retrieve the texture v-coordinate of
 * the specified device vertex.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwReal value equal to the texture v-coordinate.
 *
 * \see RwIm2DVertexGetU
 * \see RwIm2DVertexSetU
 * \see RwIm2DVertexSetV
 *
 */
RwReal
RwIm2DVertexGetV(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetV"));

    RWRETURN((RwReal) ((0.0)));
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetRealRGBA
 * is used to define the RGBA color of
 * the specified device vertex. Each color component is specified
 * as a real value in the range (RwReal)((0.0)) to (RwReal)((255.0)).
 *
 * \param vertex  A pointer to the device vertex
 * \param red  An \ref RwReal value equal to the color's red component
 * \param green  An \ref RwReal value equal to the color's green component
 * \param blue  An \ref RwReal value equal to the color's blue component
 * \param alpha  An \ref RwReal value equal to the color's alpha component.
 *
 * \return None.
 *
 * \see RwIm2DVertexGetRed
 * \see RwIm2DVertexGetGreen
 * \see RwIm2DVertexGetBlue
 * \see RwIm2DVertexGetAlpha
 * \see RwIm2DVertexSetIntRGBA
 *
 */
void
RwIm2DVertexSetRealRGBA(RwIm2DVertex * vertex, RwReal red, RwReal green,
                        RwReal blue, RwReal alpha)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetRealRGBA"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexSetIntRGBA
 * is used to define the RGBA color of the
 * specified device vertex.  Each color component is specified as an integer
 * value in the range 0 to 255.
 *
 * \ref RwIm2DVertexSetIntRGBA should be used in preference to the related macro,
 * \ref RwIm2DVertexSetRealRGBA, if the native hardware stores colors in integer
 * format, as it leads to faster execution.
 *
 * \param vertex  A pointer to the device vertex
 * \param red    An \ref RwUInt8 value equal to the color's red component
 * \param green  An \ref RwUInt8 value equal to the color's green component
 * \param blue   An \ref RwUInt8 value equal to the color's blue component
 * \param alpha  An \ref RwUInt8 value equal to the color's alpha component.
 *
 * \return None.
 *
 * \see RwIm2DVertexGetRed
 * \see RwIm2DVertexGetGreen
 * \see RwIm2DVertexGetBlue
 * \see RwIm2DVertexGetAlpha
 * \see RwIm2DVertexSetRealRGBA
 *
 */
void
RwIm2DVertexSetIntRGBA(RwIm2DVertex * vertex, RwUInt8 red, RwUInt8 green,
                       RwUInt8 blue, RwUInt8 alpha)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexSetIntRGBA"));

    RWRETURNVOID();
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetRed
 * is used to retrieve the red component of the
 * color from the specified device vertex. Component colors are
 * specified as a value in the range 0 to 255.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwUInt32 value equal to the red component.
 *
 * \see RwIm2DVertexGetGreen
 * \see RwIm2DVertexGetBlue
 * \see RwIm2DVertexGetAlpha
 * \see RwIm2DVertexSetRealRGBA
 * \see RwIm2DVertexSetIntRGBA
 *
 */
RwUInt32
RwIm2DVertexGetRed(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetRed"));

    RWRETURN(0);
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetGreen
 * is used to retrieve the green component of the
 * color from the specified device vertex. Component colors are
 * specified as a value in the range 0 to 255.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwUInt32 value equal to the green component.
 *
 * \see RwIm2DVertexGetRed
 * \see RwIm2DVertexGetBlue
 * \see RwIm2DVertexGetAlpha
 * \see RwIm2DVertexSetRealRGBA
 * \see RwIm2DVertexSetIntRGBA
 *
 */
RwUInt32
RwIm2DVertexGetGreen(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetGreen"));

    RWRETURN(0);
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetBlue
 * is used to retrieve the blue component of the
 * color from the specified device vertex. Component colors are
 * specified as a value in the range 0 to 255.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwUInt32 value equal to the blue component.
 *
 * \see RwIm2DVertexGetRed
 * \see RwIm2DVertexGetGreen
 * \see RwIm2DVertexGetAlpha
 * \see RwIm2DVertexSetRealRGBA
 * \see RwIm2DVertexSetIntRGBA
 *
 */
RwUInt32
RwIm2DVertexGetBlue(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetBlue"));

    RWRETURN(0);
}

/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertexGetAlpha
 * is used to retrieve the alpha component of the
 * color from the specified device vertex. Component colors are
 * specified as a value in the range 0 to 255.
 *
 * \param vertex  A pointer to the device vertex.
 *
 * \return Returns an \ref RwUInt32 value equal to the alpha component.
 *
 * \see RwIm2DVertexGetRed
 * \see RwIm2DVertexGetGreen
 * \see RwIm2DVertexGetBlue
 * \see RwIm2DVertexSetRealRGBA
 * \see RwIm2DVertexSetIntRGBA
 *
 */
RwUInt32
RwIm2DVertexGetAlpha(RwIm2DVertex * vertex)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DVertexGetAlpha"));

    RWRETURN(0);
}

/* Overviews */

/**
 * \ingroup rwim3dvertex
 * \page rwim3dvertexoverview RwIm3DVertex Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection im3dvertexoverview Overview
 * 
 * While rendering 3D Immediate Mode geometry, a set of RwIm3DVertex objects is produced. 
 * The functions documented here are exposed to allow access to these vertices. The vertices have 
 * been transformed and clipped to device space.
 * 
 */

/**
 * \ingroup rwim2dcameravertex
 * \page rwim2dcameravertexoverview RwIm2DCameraVertex Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection im2dcameravertexoverview Overview
 * 
 * These two macros are used to set the \ref RwCameraVertex 
 * texture coordinates together with those of an \ref RwIm2DVertex. 
 *
 * As both vertex types are frequently worked on together, using these macros can 
 * save some typing. They are defined as follows:
 *
 * \warning
 *
 * Do \e not use these macros on platforms with hardware-accelerated transform and lighting stages.
 *
 * \verbatim
#define RwIm2DCameraVertexSetU(devvert, camvert, camu, recipz)  \
    MACRO_START                                                 \
    {                                                           \
        RwCameraVertexSetU(camvert, camu);                      \
        RwIm2DVertexSetU(devvert, camu, recipz);                \
    }                                                           \
    MACRO_STOP
#define RwIm2DCameraVertexSetV(devvert, camvert, camu, recipz)  \
    MACRO_START                                                 \
    {                                                           \
        RwCameraVertexSetV(camvert, camu);                      \
        RwIm2DVertexSetV(devvert, camu, recipz);                \
    }                                                           \
    MACRO_STOP
   \endverbatim
 *
 */

#endif /* (defined(DOXYGEN)) */
