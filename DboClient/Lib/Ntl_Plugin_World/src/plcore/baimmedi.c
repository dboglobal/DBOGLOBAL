/*
 *
 * 2D immediate mode support
 * Immediate mode is for supplementing the retained mode rendering support
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwim2d
 * \page rwim2doverview RwIm2D Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection im2doverview Overview
 *
 * This object exposes RenderWare' 2D Immediate Mode API.
 *
 * 2D Immediate Mode graphics support is provided for such basic primitives
 * as lines and triangles. It is primarily intended for such tasks as GUI
 * overlays, superimposing text onto a scene and other similar work.
 *
 * Most developers may prefer to work
 * with the \ref rt2d Toolkit for applications making intensive use of 2D
 * rendering. This Toolkit sits on top of the 3D Immediate Mode API.
 *
 */

/**
 * \ingroup rwrenderstate
 * \page rwrenderstateoverview RwRenderState Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection renderstateoverview Overview
 *
 * RenderWare Graphics contains a set of global state variables that
 * are used to control rendering. Some of these variables are simple
 * boolean switches such as whether to perform Z-buffer tests, some
 * are integer types such as texture addressing mode, or the color used
 * for fogging, and others are pointers to objects such as the current
 * \ref RwRaster to use for texturing in immediate mode.
 *
 * All renderstate values are set and queried through two functions,
 *
 * - \ref RwRenderStateSet
 * - \ref RwRenderStateGet
 *
 * These take, as parameters, an \ref RwRenderState identifier, and
 * an appropriate value or pointer (cast to void *). Refer to the
 * \ref RwRenderState table for the different render states that can be
 * controlled.
 *
 * Most render states are truly global and affect nearly all rendering
 * pipelines. These include back/front face cull mode, z buffer modes, fog
 * modes, and frame buffer blending modes. Any pipelines that do override
 * these states will restore them on completion. Other renderstates,
 * such as texture raster, filtering, and addressing are frequently
 * changed within pipelines (based on \ref RwTexture objects for instance)
 * and are not restored. These must always be set explicitly when rendering
 * in immediate mode.
 *
 * <b> Cross-platform support </b>
 *
 * Since the render states must usually map to hardware states, they are not
 * universally supported on all platforms, nor can they provide complete
 * access to the hardware state of a particular platform. In the cases where
 * a render state or a particular value is not supported, the
 * RwRenderStateSet function will return FALSE. Refer to the platform
 * specific documentation for details of unsupported render states, and
 * also additional renderstate functions which might be available for
 * controlling hardware specific features.
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"

#include "drvmodel.h"

#include "baimmedi.h"


/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#if (defined(RWDEBUG))

static RwBool
validate_vertices(const RwImVertexIndex *indices,
                  RwInt32 numIndices,
                  RwInt32 numVertices)
{
    RwBool          result = TRUE;
    RwInt32         i;

    RWFUNCTION(RWSTRING("validate_vertices"));

    RWASSERT(0 <= numVertices);
    RWASSERT(0 <= numIndices);
    RWASSERT((0 == numIndices) || (NULL != indices));

    for (i = 0; i < numIndices; i++)
    {
       const RwImVertexIndex vertIndex = indices[i];
       const RwBool validIndex = (vertIndex < (RwImVertexIndex)numVertices);
         /* ((RwImVertexIndex)0 <= vertIndex ) */

       RWASSERT(validIndex);
       result = validIndex && result;
    }

    RWRETURN(result);
}

#define VALIDATE_VERTICES(indices, numIndices, numVertices)     \
    validate_vertices(indices, numIndices, numVertices)

#endif /* (defined(RWDEBUG)) */

#if (!defined(VALIDATE_VERTICES))
#define VALIDATE_VERTICES(indices, numIndices, numVertices)  /* No op */
#endif /* (!defined(VALIDATE_VERTICES)) */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/****************************************************************************
 Functions
 */

#if ( ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) )

/**
 * \ingroup rwim2d
 * \ref RwIm2DGetNearScreenZ is used to determine the screen Z value needed
 * to be put into a 2D immediate mode vertex to place the vertex at the front of
 * the Z-buffer range.
 *
 * \note Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \return Returns an \ref RwReal value equal to the Z-buffer value of the near clip
 * plane.
 *
 * \see RwIm2DGetFarScreenZ
 * \see RwCameraGetNearClipPlane
 *
 */
RwReal
RwIm2DGetNearScreenZ(void)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DGetNearScreenZ"));

    RWRETURN(RwIm2DGetNearScreenZMacro());
}

/**
 * \ingroup rwim2d
 * \ref RwIm2DGetFarScreenZ is used to determine the screen Z value needed
 * to be put into a 2D immediate mode vertex to place the vertex at the back of
 * the Z-buffer range.
 *
 * \note Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \return Returns an \ref RwReal value equal to the Z-buffer value of the far clip
 * plane.
 *
 * \see RwIm2DGetNearScreenZ
 * \see RwCameraGetFarClipPlane
 *
 */
RwReal
RwIm2DGetFarScreenZ(void)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DGetFarScreenZ"));

    RWRETURN(RwIm2DGetFarScreenZMacro());
}

/**
 * \ingroup rwrenderstate
 * \ref RwRenderStateSet is used to set the value of a particular
 * renderstate element.
 *
 * \attention This should only be used between calls to
 * \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate.
 *
 * \if d3d8
 * See also \ref d3d8cache.
 * \endif
 * 
 * \if d3d9
 * See also \ref d3d9cache.
 * \endif
 * 
 * \param state  The \ref RwRenderState to set.
 * \param value  The value of the renderstate, cast to type (void *).
 *
 * \return Returns TRUE if successful or FALSE if there is an error or
 *         the renderstate is not supported.
 *
 * Examples:
 * \code
 *      RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);
 *      RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
 *      RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)raster);
 * \endcode
 *
 * Many of the renderstates may not be supported on certain platforms
 * and this function will return FALSE in such cases. It is recommended
 * that during development you check the return value to check for
 * unsupported states or values. Refer to the platform specific
 * documentation for more details of unsupported states, and also for
 * any details of any platform specific renderstate functions.
 *
 * \see \ref rwrenderstateoverview
 * \see RwRenderStateGet
 * \see RwIm3DGetRenderPipeline
 */
RwBool
RwRenderStateSet(RwRenderState state, void *value)
{
	RWAPIFUNCTION(RWSTRING("RwRenderStateSet"));
    RWASSERT(RWSRCGLOBAL(curCamera));
	
	/* Tell the device */
    RWRETURN(RwRenderStateSetMacro(state, value));
}

/**
 * \ingroup rwrenderstate
 * \ref RwRenderStateGet queries the value of a renderstate element.
 *
 * \param state  The \ref RwRenderState element to be queried.
 * \param value  A pointer to memory to receive the value of the
 *               renderstate element.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * Examples:
 * \code
 *
 *      RwShadeMode  shadeMode;
 *      RwBool       zTest;
 *
 *      RwRenderStateGet(rwRENDERSTATESHADEMODE, (void *)&shadeMode);
 *      RwRenderStateGet(rwRENDERSTATEZTESTENABLE, (void *)&zTest);
 * \endcode
 *
 * \see \ref rwrenderstateoverview
 * \see RwRenderStateSet
 * \see RwIm3DGetRenderPipeline
 */
RwBool
RwRenderStateGet(RwRenderState state, void *value)
{
    RWAPIFUNCTION(RWSTRING("RwRenderStateGet"));

    RWRETURN(RwRenderStateGetMacro(state, value));
}

/**
 * \ingroup rwim2d
 * \ref RwIm2DRenderLine is used to render a screen space 2D immediate
 * mode line defined by the given two vertices. All vertices specified for
 * 2D immediate mode rendering must lie within the screen space otherwise
 * the results are unpredictable.
 *
 * Note that 2D immediate mode routines must initialize their required renderstate
 * before starting to render.  They must share the renderstate resource and must not
 * make any assumptions about what the existing renderstate is.  The renderstate may
 * be queried but in practice on many platforms it is faster to just establish the
 * required renderstate blindly.
 *
 * This API function should only be used between calls to \ref RwCameraBeginUpdate
 * and \ref RwCameraEndUpdate.
 *
 * \param vertices  A pointer to an array of 2D immediate mode vertices
 * \param numVertices  An \ref RwInt32 value equal to the number of vertices in the array
 * \param vert1  An \ref RwInt32 value specifying the index of the first vertex
 * \param vert2  An \ref RwInt32 value specifying the index of the second vertex
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexSetScreenZ
 * \see RwIm2DVertexSetRecipCameraZ
 * \see RwIm2DVertexSetRealRGBA
 * \see RwIm2DVertexSetIntRGBA
 * \see RwIm2DRenderTriangle
 * \see RwIm2DRenderIndexedPrimitive
 * \see RwRenderStateSet
 * \see RwRenderStateGet
 * \see RwIm2DGetNearScreenZ
 * \see RwIm2DGetFarScreenZ
 *
 * \verbatim
   To set up and render a 2D immediate mode line with Gouraud shading
   and alpha-blending:

   RwIm2DVertex vertex[2];

   // Set up the vertices...
   RwIm2DVertexSetScreenX(&vertex[0], (RwReal)((20)) );
   RwIm2DVertexSetScreenY(&vertex[0], (RwReal)((20)) );
   RwIm2DVertexSetScreenZ(&vertex[0], (RwReal)((3276)) );
   RwIm2DVertexSetRecipCameraZ(&vertex[0], (RwReal)((1.0/6.0)) );

   RwIm2DVertexSetScreenX(&vertex[1], (RwReal)((620)) );
   RwIm2DVertexSetScreenY(&vertex[1], (RwReal)((460)) );
   RwIm2DVertexSetScreenZ(&vertex[1], (RwReal)((3276)) );
   RwIm2DVertexSetRecipCameraZ(&vertex[1], (RwReal)((1.0/6.0)) );

   // Opaque red...
   RwIm2DVertexSetIntRGBA(&vertex[0], 255, 0, 0, 255);

   // Semi-transparent red...
   RwIm2DVertexSetIntRGBA(&vertex[1], 255, 0, 0, 128);

   // Texturing off...
   RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);

   // Gouraud shading on...
   RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);

   // Alpha-transparency on...
   RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);

   //Render line...
   RwIm2DRenderLine(vertex, 2, 0, 1);

   \endverbatim
 *
 */
RwBool
RwIm2DRenderLine(RwIm2DVertex *vertices, RwInt32 numVertices,
                 RwInt32 vert1, RwInt32 vert2)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DRenderLine"));
    RWASSERT(vertices);
    RWASSERT(numVertices > 0);
    RWASSERT(vert1 < numVertices);
    RWASSERT(vert2 < numVertices);

    RWRETURN(RwIm2DRenderLineMacro(vertices, numVertices, vert1, vert2));
}

/**
 * \ingroup rwim2d
 * \ref RwIm2DRenderTriangle is used to render a screen space 2D
 * immediate mode triangle defined by the given three vertices. All vertices
 * specified for 2D immediate mode rendering must lie within the screen space
 * otherwise the results are unpredictable.
 *
 * Note that 2D immediate mode routines must initialize their required renderstate
 * before starting to render.  They must share the renderstate resource and must
 * not make any assumptions about what the existing renderstate is.  Failure to do
 * this may result in unexpected rending artifacts.  The renderstate may be queried
 * but in practice on many platforms it is faster to just establish the required
 * renderstate blindly.
 *
 * This API function should only be used between calls to \ref RwCameraBeginUpdate
 * and \ref RwCameraEndUpdate.
 *
 * \param vertices  A pointer to an array of immediate mode vertices
 * \param numVertices  An \ref RwInt32 value equal to the number of vertices in the array
 * \param vert1  An \ref RwInt32 value specifying the index of the first vertex
 * \param vert2  An \ref RwInt32 value specifying the index of the second vertex
 * \param vert3  An \ref RwInt32 value specifying the index of the third vertex
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexSetScreenZ
 * \see RwIm2DVertexSetRecipCameraZ
 * \see RwIm2DVertexSetRealRGBA
 * \see RwIm2DVertexSetIntRGBA
 * \see RwIm2DRenderLine
 * \see RwIm2DRenderIndexedPrimitive
 * \see RwRenderStateSet
 * \see RwRenderStateGet
 * \see RwIm2DGetNearScreenZ
 * \see RwIm2DGetFarScreenZ
 *
 * To set up and render a 2D immediate mode triangle with Gouraud shading
 * and alpha-blending:
 *
 * \verbatim
   RwIm2DVertex vertex[3];

   //Set up the vertices...
   RwIm2DVertexSetScreenX(&vertex[0], (RwReal)((20)) );
   RwIm2DVertexSetScreenY(&vertex[0], (RwReal)((20)) );
   RwIm2DVertexSetScreenZ(&vertex[0], (RwReal)((3276)) );
   RwIm2DVertexSetRecipCameraZ(&vertex[0], (RwReal)((1.0/6.0)) );

   RwIm2DVertexSetScreenX(&vertex[1], (RwReal)((20)) );
   RwIm2DVertexSetScreenY(&vertex[1], (RwReal)((40)) );
   RwIm2DVertexSetScreenZ(&vertex[1], (RwReal)((3276)) );
   RwIm2DVertexSetRecipCameraZ(&vertex[1], (RwReal)((1.0/6.0)) );

   RwIm2DVertexSetScreenX(&vertex[2], (RwReal)((40)) );
   RwIm2DVertexSetScreenY(&vertex[2], (RwReal)((40)) );
   RwIm2DVertexSetScreenZ(&vertex[2], (RwReal)((3276)) );
   RwIm2DVertexSetRecipCameraZ(&vertex[2], (RwReal)((1.0/6.0)) );

   // Opaque yellow...
   RwIm2DVertexSetIntRGBA(&vertex[0], 255, 255, 0, 255);

   // Semi-transparent magenta...
   RwIm2DVertexSetIntRGBA(&vertex[1], 255, 0, 255, 128);

   // Fully-transparent cyan...
   RwIm2DVertexSetIntRGBA(&vertex[2], 0, 255, 255, 0);

   // Texturing off...
   RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);

   // Gouraud shading on...
   RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);

   // Alpha-transparency on...
   RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);

   // Render triangle...
   RwIm2DRenderTriangle(vertex, 3, 0, 1, 2);

   \endverbatim
 *
 */
RwBool
RwIm2DRenderTriangle(RwIm2DVertex *vertices, RwInt32 numVertices,
                     RwInt32 vert1, RwInt32 vert2, RwInt32 vert3)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DRenderTriangle"));
    RWASSERT(vertices);
    RWASSERT(numVertices > 0);
    RWASSERT(vert1 < numVertices);
    RWASSERT(vert2 < numVertices);
    RWASSERT(vert3 < numVertices);

    RWRETURN(RwIm2DRenderTriangleMacro(vertices,
                                       numVertices,
                                       vert1, vert2, vert3));
}

/**
 * \ingroup rwim2d
 * \ref RwIm2DRenderPrimitive is used to render a screen space 2D immediate
 * mode primitive defined by the given vertices. Use this function
 * to render individual line segments, connected poly-lines, discrete triangles,
 * triangle strips and triangle fans. Note that all vertices
 * specified for 2D immediate mode rendering must lie within the screen space
 * otherwise the results are unpredictable.  The result is similar to what would
 * be achieved with \ref RwIm2DRenderIndexedPrimitive, but the indices are implicitly
 * defined as (0, 1, 2, 3, ..., numVertices-1).
 *
 * Note that 2D immediate mode routines must initialize their required renderstate
 * before starting to render.  They must share the renderstate resource and must
 * not make any assumptions about what the existing renderstate is.  Failure to do
 * this may result in unexpected rendering artifacts.  The renderstate may be queried
 * but in practice on many platforms it is faster to just establish the required
 * renderstate blindly.
 *
 * This API function should only be used between calls to \ref RwCameraBeginUpdate
 * and \ref RwCameraEndUpdate.
 *
 * \param primType  An \ref RwInt32 value equal to the type of indexed primitive:
 *        \li rwPRIMTYPELINELIST - Render lines as a list of unconnected line segments
 *        \li rwPRIMTYPEPOLYLINE - Render lines as a connected polyline
 *        \li rwPRIMTYPETRILIST  - Render triangles as a sequence of isolated triangles
 *        \li rwPRIMTYPETRISTRIP - Render triangles as a triangle strip
 *        \li rwPRIMTYPETRIFAN   - Render triangles as a triangle fan
 * \param vertices  A pointer to an array of immediate mode vertices
 * \param numVertices  An \ref RwInt32 value equal to the number of vertices in the array
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexSetScreenZ
 * \see RwIm2DVertexSetRecipCameraZ
 * \see RwIm2DVertexSetRealRGBA
 * \see RwIm2DVertexSetIntRGBA
 * \see RwIm2DRenderTriangle
 * \see RwIm2DRenderLine
 * \see RwRenderStateSet
 * \see RwRenderStateGet
 * \see RwIm2DGetNearScreenZ
 * \see RwIm2DGetFarScreenZ
 *
 */
RwBool
RwIm2DRenderPrimitive(RwPrimitiveType primType,
                      RwIm2DVertex *vertices, RwInt32 numVertices)
{
    RWAPIFUNCTION(RWSTRING("RwIm2DRenderPrimitive"));
    RWASSERT(vertices);
    RWASSERT(numVertices);

    RWRETURN(RwIm2DRenderPrimitiveMacro(primType, vertices, numVertices));
}

/**
 * \ingroup rwim2d
 * \ref RwIm2DRenderIndexedPrimitive is used to render a screen space
 * 2D immediate mode primitive defined by the given vertices. Use this function
 * to render individual line segments, connected poly-lines, discrete triangles,
 * triangle strips and triangle fans. Note that all vertices
 * specified for 2D immediate mode rendering must lie within the screen space
 * otherwise the results are unpredictable.
 *
 * Note that 2D immediate mode routines must initialize their required renderstate
 * before starting to render.  They must share the renderstate resource and must
 * not make any assumptions about what the existing renderstate is.  The renderstate
 * may be queried but in practice on many platforms it is faster to just establish
 * the required renderstate blindly.
 *
 * This API function should only be used between calls to \ref RwCameraBeginUpdate
 * and \ref RwCameraEndUpdate.
 *
 * \param primType  An \ref RwInt32 value equal to the type of indexed primitive:
 *        \li rwPRIMTYPELINELIST - Render lines as a list of unconnected line segments
 *        \li rwPRIMTYPEPOLYLINE - Render lines as a connected polyline
 *        \li rwPRIMTYPETRILIST  - Render triangles as a sequence of isolated triangles
 *        \li rwPRIMTYPETRISTRIP - Render triangles as a triangle strip
 *        \li rwPRIMTYPETRIFAN   - Render triangles as a triangle fan
 * \param vertices  A pointer to an array of immediate mode vertices
 * \param numVertices  An \ref RwInt32 value equal to the number of vertices in the array
 * \param indices  A pointer to an array of vertex indices
 * \param numIndices  An \ref RwInt32 value equal to the number of vertex indices in the array
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwIm2DVertexSetScreenX
 * \see RwIm2DVertexSetScreenY
 * \see RwIm2DVertexSetScreenZ
 * \see RwIm2DVertexSetRecipCameraZ
 * \see RwIm2DVertexSetRealRGBA
 * \see RwIm2DVertexSetIntRGBA
 * \see RwIm2DRenderTriangle
 * \see RwIm2DRenderLine
 * \see RwRenderStateSet
 * \see RwRenderStateGet
 * \see RwIm2DGetNearScreenZ
 * \see RwIm2DGetFarScreenZ
 *
 * \verbatim
   To set up and render a 2D immediate mode polyline square with
   flat shading, but without Z-buffering:

   RwIm2DVertex square[4];
   RwImVertexIndex index[5] = {0, 1, 2, 3, 0};

   // Set up the vertices...
   RwIm2DVertexSetScreenX(&square[0], (RwReal)((220)) );
   RwIm2DVertexSetScreenY(&square[0], (RwReal)((140)) );

   RwIm2DVertexSetScreenX(&square[1], (RwReal)((220)) );
   RwIm2DVertexSetScreenY(&square[1], (RwReal)((240)) );

   RwIm2DVertexSetScreenX(&square[2], (RwReal)((420)) );
   RwIm2DVertexSetScreenY(&square[2], (RwReal)((240)) );

   RwIm2DVertexSetScreenX(&square[3], (RwReal)((420)) );
   RwIm2DVertexSetScreenY(&square[3], (RwReal)((140)) );

   RwIm2DVertexSetIntRGBA(&square[0], 255, 0, 0, 255);
   RwIm2DVertexSetIntRGBA(&square[1], 0, 255, 0, 255);
   RwIm2DVertexSetIntRGBA(&square[2], 0, 0, 255, 255);
   RwIm2DVertexSetIntRGBA(&square[3], 255, 255, 0, 255);

   // Flat shading on...
   RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEFLAT);

   // Z-buffering off..
   RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)FALSE);

   // Render polyline...
   RwIm2DRenderIndexedPrimitive(rwPRIMTYPEPOLYLINE, square, 4, index, 5);

   \endverbatim
 *
 */
RwBool
RwIm2DRenderIndexedPrimitive(RwPrimitiveType primType,
                             RwIm2DVertex *vertices, RwInt32 numVertices,
                             RwImVertexIndex *indices, RwInt32 numIndices)
{
    RwBool result;

    RWAPIFUNCTION(RWSTRING("RwIm2DRenderIndexedPrimitive"));

    RWASSERT(0 <= numVertices);
    RWASSERT((0 == numVertices) || (NULL != vertices));
    RWASSERT(0 <= numIndices);
    RWASSERT((0 == numIndices) || (NULL != indices));

    /* Validate the indices */
    VALIDATE_VERTICES(indices, numIndices, numVertices);

    RWASSERT(NULL != RWSRCGLOBAL(dOpenDevice).fpIm2DRenderIndexedPrimitive);

    result = RwIm2DRenderIndexedPrimitiveMacro(primType,
                                               vertices, numVertices,
                                               indices, numIndices);

    RWRETURN(result);
}

#endif /* ( ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) ) */
