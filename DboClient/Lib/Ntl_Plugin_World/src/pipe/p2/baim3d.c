/*
 * Immediate mode rendering
 *
 * Copyright (c) Criterion Software Limited
 */

/* Platform-specific documentation is in src/pipe/p2/[target]/im3dpipe.c */

/***************************************************************************
 *                                                                         *
 * Module  : baim3d.c                                                      *
 *                                                                         *
 * Purpose : Replaces baim3com.c, prividing the RwIM3D API                 *
 *                                                                         *
 **************************************************************************/

/**
 * \ingroup rwim3d
 * \page rwim3doverview RwIm3D Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection im3doverview Overview
 *
 * This object exposes RenderWare Graphics' 3D Immediate Mode API.
 *
 * 3D Immediate Mode is the lowest-level 3D graphics rendering API available within
 * RenderWare and is always available. It makes use of hardware acceleration and
 * is most often used together with RenderWare's default scene graph
 * API, \ref RpWorld, to add additional 'chrome' to a rendered scene.
 *
 * Your target platform may be provided with additional, platform-specific Immediate Mode
 * functionality. Check the platform-specific SDK documentation for more details.
 *
 */

/****************************************************************************
 Includes
 */

/* Pick up memset prototype */
#include <string.h>
#if (defined(_MSC_VER))
#if (_MSC_VER>=1000)
#include <memory.h>
#endif /* (_MSC_VER>=1000) */
#endif /* (defined(_MSC_VER)) */

#include "batypes.h"
#include "batypehf.h"
#include "balibtyp.h"
#include "bamemory.h"
#include "badebug.h"

#include "baimmedi.h"
#include "bamatrix.h"
#include "drvmodel.h"

#include "p2core.h"
#include "pip2model.h"
#include "p2stdcls.h"

#include "bapipe.h"
#include "p2define.h"

/* Platform specific construction functions */
#include "im3dpipe.h"

/* This file */
#include "baim3d.h"

#define SUPPRESS_IM3Dx

/****************************************************************************
 Local Defines
 */

#define RWIMMEDIGLOBAL(var)                             \
    (RWPLUGINOFFSET(rwImmediGlobals,                    \
                    RwEngineInstance,                   \
                    _rwIm3DModule.globalsOffset)->var)

/* Bounds-check indices in debug mode: */
#if (defined(RWDEBUG))
static              RwBool
validate_vertices(void *indices, RwInt32 numIndices, RwInt32 numVertices)
{
    RwBool              result = TRUE;
    RwInt32             i;
    const RxVertexIndex *const vertArray = (const RxVertexIndex *) (indices);

    RWFUNCTION(RWSTRING("validate_vertices"));

    RWASSERT(0 <= numVertices);
    RWASSERT(0 <= numIndices);
    RWASSERT((0 == numIndices) || (NULL != indices));

    for (i = 0; i < numIndices; i++)
    {
        const RxVertexIndex vertIndex = vertArray[i];
        const RwBool        validIndex =

            (vertIndex < (RxVertexIndex) numVertices);
        /* ((RxVertexIndex) 0 <= vertIndex) */

        RWASSERT(validIndex);
        result = validIndex && result;
    }

    RWRETURN(result);
}

#define VALIDATE_VERTICES(indices, numIndices, numVertices)     \
    validate_vertices(indices, numIndices, numVertices)
#endif /* (defined(RWDEBUG)) */

#if (!defined(VALIDATE_VERTICES))
#define VALIDATE_VERTICES(indices, numIndices, numVertices) /* No op */
#endif /* (!defined(VALIDATE_VERTICES)) */

#define RWPRIMTYPEVALID(primType)         \
  ( (rwPRIMTYPELINELIST == (primType)) || \
    (rwPRIMTYPEPOLYLINE == (primType)) || \
    (rwPRIMTYPETRIFAN == (primType))   || \
    (rwPRIMTYPETRILIST == (primType))  || \
    (rwPRIMTYPETRISTRIP == (primType)) )

/****************************************************************************
 Local (static) Globals
 */

#if (!defined(SUPPRESS_IM3D))

/* Temporarily non-static so fastim3d.c can get at it */
RwModuleInfo        _rwIm3DModule;
#endif /* (!defined(SUPPRESS_IM3D)) */

/* Temporary, for debugging */
rwImmediGlobals    *_rwIm3DGlobals = (rwImmediGlobals *)NULL;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           Immediate mode

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwim3d
 * \ref RwIm3DTransform is used to transform the given vertices into the
 * current camera in preparation for rendering. Following transformation there
 * exists an array of camera space vertices, together with an array of screen
 * space (device-dependent) vertices for those which lie in the camera's
 * frustum.
 *
 * This function executes the immediate mode transform pipeline, which is
 * accessible through \ref RwIm3DGetTransformPipeline. This transforms
 * the vertices into world space and caches them ready for rendering by
 * subsequent calls to RwIm3D render functions. These vertices are cached
 * and can be used by any number of RwIm3D rendering functions until \ref RwIm3DEnd
 * is called, at which point the vertices are discarded.
 *
 * Note that because RwIm3D behaves (from the point of view of the API and
 * the application) exactly as it did with the old RenderWare Graphics pipeline,
 * there are some restrictions due to the details of the implementation using the
 * new flexible pipeline code. In-between a \ref RwIm3DTransform and \ref RwIm3DEnd
 * pair, no other pipeline execution, that is, anything which calls
 * \ref RxPipelineExecute -  can take place. This is because the caching
 * of the vertices requires the global heap - see \ref RxHeapGetGlobalHeap - to
 * remain uncleared. Executing other pipelines would clear the heap. There
 * will be a successor to RwIm3D which will work more cleanly with the new
 * PowerPipe functionality and which will not have these restrictions, but
 * that is not yet available.
 *
 * This function takes an optional matrix which is concatenated onto the
 * world-to-camera matrix before vertices are transformed. This is useful if
 * the vertices are in object space, in which case the argument should be the
 * local transformation matrix.
 *
 * \note The \ref RwIm3DTransformFlags flags \ref rwIM3D_VERTEXXYZ and
 *       \ref rwIM3D_VERTEXRGBA are added automatically by this function, thus
 *       requiring at least positional and color information in the \e pVerts data.
 *
 * \param pVerts  A pointer to the \ref RwIm3DVertex array to transform
 * \param numVerts  An \ref RwInt32 value equal to the number of vertices to transform
 * \param ltm  A pointer to the object-to-world matrix, or
 *       NULL if the vertices are provided in world space
 * \param flags  flags of type \ref RwIm3DTransformFlags.
 *
 * \return Returns the vertex array if successful, otherwise NULL.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 *
 */
void               *
RwIm3DTransform(RwIm3DVertex * pVerts,
                RwUInt32 numVerts, RwMatrix * ltm, RwUInt32 flags)
{
    /* Packages up the vertices/numvertices/flags/matrix  in a struct
     * and then calls RxPipelineExecute() with this struct (potentially
     * several times) at this point during subsequent calls to
     * RwIm3DRender[Indexed]Primitive().
     *
     * Note that the global heap will not be cleared during this time (this
     * function stores a list of packets containing the transformed data to
     * be cached) and thus no other pipelines may be executed until
     * RwIm3DEnd() has been called. Subsequent non-RwIm3DRender* invocations
     * of RxPipelineExecute() will clear the heap (it's also cleared on
     * shutdown). */
    RxPipeline         *result;

    RWAPIFUNCTION(RWSTRING("RwIm3DTransform"));

#if (!defined(SUPPRESS_IM3D))

    /* It's OK to have zero vertices */

    RWASSERT(0 < _rwIm3DModule.numInstances);
    RWASSERT(NULL != pVerts);

    if (numVerts > 65536L)
    {
        RWERROR((E_RX_TOOMANYVERTS));
    }
    else
    {
        RWIMMEDIGLOBAL(curPool).numElements = (RwUInt16) numVerts;
        RWIMMEDIGLOBAL(curPool).elements = pVerts;
        /* Eventually stride may switch on the
         * basis of incoming object type */
        RWIMMEDIGLOBAL(curPool).stride = sizeof(RxObjSpace3DLitVertex);
        /* Flags and transformation matrix put in the stash since they
         * are accessed by the render pipeline (the latter with H/W T&L)
         * and that generally needs the flags (not so the source vertices) */
        RWIMMEDIGLOBAL(curPool).stash.ltm = ltm;
        RWIMMEDIGLOBAL(curPool).stash.flags =
            (flags | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA);

        /* Execute the pipe - packets get stashed in RXPIPELINEGLOBALs */
        result =
            RxPipelineExecute(RWIMMEDIGLOBAL
                              (im3DTransformPipeline),
                              &RWIMMEDIGLOBAL(curPool), TRUE);

        if (result != NULL)
        {
            /*
             * NOTE: The app needs to do an RwIm3DEnd
             * when it's through doing 3D IM
             */
            RWRETURN(pVerts);
        }

    }

    /* Pipeline execution failed for some reason */
    RwDebugSendMessage(rwDEBUGMESSAGE, "RwIm3DTransform",
                       "Pipeline execution failed");

#endif /* (!defined(SUPPRESS_IM3D)) */

    RWRETURN(NULL);
}

/**
 * \ingroup rwim3d
 * \ref RwIm3DEnd is used to indicate that the application has finished
 * using the transformed 3D immediate mode vertices and they can be discarded
 * by the engine. After this is called, other pipelines may be executed since
 * the global heap (see \ref RxHeapGetGlobalHeap) will have been cleared.
 *
 * \return TRUE on success, FALSE otherwise.
 *
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 *
 */

RwBool
RwIm3DEnd(void)
{
    RwBool              im3dactive;

    RWAPIFUNCTION(RWSTRING("RwIm3DEnd"));

#if (!defined(SUPPRESS_IM3D))
    im3dactive = (RWIMMEDIGLOBAL(curPool).elements != NULL);

    if (!im3dactive)
    {
        RWRETURN(FALSE);
    }

    /* No need to reset heap, it'll happen next time a pipe's executed
     * (including RwIM3DTransform, thankfully!) */

    /* Better safe than sorry, reset the global vertex pool info */
    memset(&RWIMMEDIGLOBAL(curPool), 0, sizeof(rwIm3DPool));

#endif /* (!defined(SUPPRESS_IM3D)) */
    RWRETURN(TRUE);
}

/**
 * \ingroup rwim3d
 * \ref RwIm3DRenderIndexedPrimitive executes the remainder of the
 * RwIm3D transform pipeline, (internally passing in the cached packets
 * containing the transformed vertices) and specifying the primitive
 * type and index data in the (void *)data parameter of \ref RxPipelineExecute.
 *
 * \param primType  \ref RwPrimitiveType primitive type
 * \param indices  pointer to \ref RwImVertexIndex array of indices
 * \param numIndices  \ref RwInt32 number of Indices in the primitive.
 *
 * \return Returns TRUE on success, FALSE on error.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 *
 */
RwBool
RwIm3DRenderIndexedPrimitive(RwPrimitiveType primType,
                             RwImVertexIndex * indices,
                             RwInt32 numIndices)
{
    RwBool              im3dactive;

    RWAPIFUNCTION(RWSTRING("RwIm3DRenderIndexedPrimitive"));

#if (!defined(SUPPRESS_IM3D))

    RWASSERT(indices);
    im3dactive = (RWIMMEDIGLOBAL(curPool).elements != NULL);

    if (im3dactive)
    {
        _rwIm3DPoolStash    *stash = &(RWIMMEDIGLOBAL(curPool).stash);

        stash->pipeline = (RxPipeline *) NULL;
        stash->primType = (RwPrimitiveType) primType;
        stash->indices = (RxVertexIndex *) indices;
        stash->numIndices = (RwUInt32) numIndices;

        RWASSERT(RWPRIMTYPEVALID(primType));

        switch (primType)
        {
            case rwPRIMTYPETRILIST:
                {
                    /* We need at least 3 verts */
                    RWASSERT(numIndices >= 3);
                    /* We need a multiple of 3 indices */
                    RWASSERT((numIndices % 3) == 0);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).triList;
                    stash->numIndices = numIndices - (numIndices % 3);
                    break;
                }
            case rwPRIMTYPETRIFAN:
                {
                    /* We need at least 3 verts */
                    RWASSERT(numIndices >= 3);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).triFan;
                    break;
                }
            case rwPRIMTYPETRISTRIP:
                {
                    /* We need at least 3 verts */
                    RWASSERT(numIndices >= 3);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).triStrip;
                    break;
                }
            case rwPRIMTYPELINELIST:
                {
                    /* We need at least 2 verts */
                    RWASSERT(numIndices >= 2);
                    /* We need a multiple of 2 indices */
                    RWASSERT((numIndices % 2) == 0);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).lineList;
                    stash->numIndices = numIndices - (numIndices % 2);
                    break;
                }
            case rwPRIMTYPEPOLYLINE:
                {
                    /* We need at least 2 verts */
                    RWASSERT(numIndices >= 2);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).polyLine;
                    break;
                }
            default:
                {
                    RWERROR((E_RX_INVALIDPRIMTYPE, primType));
                    break;
                }
        }

#if (defined(SKY2_DRVMODEL_H) && defined(RWDEBUG))
        /* Either use both the PS2All default transform AND render pipeline
         * or neither - mixing PS2All with non-PS2All is heap bad voodoo
         * [can also use PS2Manager render pipe w/ PS2All transform pipe] */

        /* NOTE: this test may become appropriate for other targets */
        {
            RxPipeline *curTrns, *ps2AllTrns;
            RxPipeline *ps2AllRnd, *ps2AllRnd2;

            curTrns = RWIMMEDIGLOBAL(im3DTransformPipeline);
            ps2AllTrns = RWIMMEDIGLOBAL(platformIm3DTransformPipeline);
            ps2AllRnd = RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triList;
            ps2AllRnd2 = RWIMMEDIGLOBAL(platformIm3DRenderPipelines).lineList;
            /* As with PS2All, PS2Manager works for all primTypes */

            RWASSERT( ((curTrns == ps2AllTrns) &&
                       ((stash->pipeline == ps2AllRnd) ||
                        (stash->pipeline == ps2AllRnd2))) ||
                      ((curTrns != ps2AllTrns) &&
                       ((stash->pipeline != ps2AllRnd) &&
                        (stash->pipeline != ps2AllRnd2))) );
        }
#endif /* (defined(SKY2_DRVMODEL_H) && defined(RWDEBUG)) */

        VALIDATE_VERTICES(stash->indices,
                          stash->numIndices,
                          RWIMMEDIGLOBAL(curPool).numElements);

        if (RxPipelineExecute(stash->pipeline, (void *) stash, FALSE) != NULL)
        {
            RWRETURN(TRUE);
        }
    }
    else
    {
        RWERROR((E_RX_IM3DNOTACTIVE));
    }

    RWRETURN(FALSE);
#else /* (!defined(SUPPRESS_IM3D)) */
    RWRETURN(TRUE);
#endif /* (!defined(SUPPRESS_IM3D)) */

}

/**
 * \ingroup rwim3d
 * \ref RwIm3DRenderPrimitive executes the remainder of the
 * RwIm3D transform pipeline, (internally passing in the cached packets
 * containing the transformed vertices) and specifying the primitive
 * type and index data in the (void *)data parameter of \ref RxPipelineExecute.
 *
 * \param primType  An \ref RwPrimitiveType equal to the primitive type.
 *
 * \return Returns TRUE on success, FALSE on error.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 *
 */

RwBool
RwIm3DRenderPrimitive(RwPrimitiveType primType)
{
    RwBool              im3dactive;
    RxHeap             *heap;

    RWAPIFUNCTION(RWSTRING("RwIm3DRenderPrimitive"));

#if (!defined(SUPPRESS_IM3D))
    im3dactive = (RWIMMEDIGLOBAL(curPool).elements != NULL);

    heap = RxHeapGetGlobalHeap();
    RWASSERT(NULL != heap);

    if (im3dactive)
    {
        _rwIm3DPoolStash    *stash = &(RWIMMEDIGLOBAL(curPool).stash);

        stash->pipeline = (RxPipeline *) NULL;
        stash->primType = (RwPrimitiveType) primType;
        stash->indices = (RxVertexIndex *) NULL;
        stash->numIndices = (RwUInt32) RWIMMEDIGLOBAL(curPool).numElements;

        RWASSERT(RWPRIMTYPEVALID(primType));

        switch (primType)
        {
            case rwPRIMTYPETRILIST:
                {
                    /* We need at least 3 verts */
                    RWASSERT(RWIMMEDIGLOBAL(curPool).numElements >= 3);
                    /* We need a multiple of 3 vertices */
                    RWASSERT((RWIMMEDIGLOBAL(curPool).numElements % 3) == 0);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).triList;

                    break;
                }
            case rwPRIMTYPETRIFAN:
                {
                    /* We need at least 3 verts */
                    RWASSERT(RWIMMEDIGLOBAL(curPool).numElements >= 3);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).triFan;

                    break;
                }
            case rwPRIMTYPETRISTRIP:
                {
                    /* We need at least 3 verts */
                    RWASSERT(RWIMMEDIGLOBAL(curPool).numElements >= 3);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).triStrip;

                    break;
                }
            case rwPRIMTYPELINELIST:
                {
                    /* We need at least 2 verts */
                    RWASSERT(RWIMMEDIGLOBAL(curPool).numElements >= 2);
                    /* We need a multiple of 2 vertices */
                    RWASSERT((RWIMMEDIGLOBAL(curPool).numElements % 2) == 0);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).lineList;

                    break;
                }
            case rwPRIMTYPEPOLYLINE:
                {
                    /* We need at least 2 verts */
                    RWASSERT(RWIMMEDIGLOBAL(curPool).numElements >= 2);

                    stash->pipeline =
                        RWIMMEDIGLOBAL(im3DRenderPipelines).polyLine;

                    break;
                }

            default:
                {
                    RWERROR((E_RX_INVALIDPRIMTYPE, primType));
                    break;
                }
        }

#if (defined(SKY2_DRVMODEL_H) && defined(RWDEBUG))
        /* Either use both the PS2All default transform AND render pipeline
         * or neither - mixing PS2All with non-PS2All is heap bad voodoo
         * [can also use PS2Manager render pipe w/ PS2All transform pipe] */

        /* NOTE: this test may become appropriate for other targets */
        {
            RxPipeline *curTrns, *ps2AllTrns;
            RxPipeline *ps2AllRnd, *ps2AllRnd2;

            curTrns = RWIMMEDIGLOBAL(im3DTransformPipeline);
            ps2AllTrns = RWIMMEDIGLOBAL(platformIm3DTransformPipeline);
            ps2AllRnd = RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triList;
            ps2AllRnd2 = RWIMMEDIGLOBAL(platformIm3DRenderPipelines).lineList;

            RWASSERT( ((curTrns == ps2AllTrns) &&
                       ((stash->pipeline == ps2AllRnd) ||
                        (stash->pipeline == ps2AllRnd2))) ||
                      ((curTrns != ps2AllTrns) &&
                       ((stash->pipeline != ps2AllRnd) &&
                        (stash->pipeline != ps2AllRnd2))) );
        }
#endif /* (defined(SKY2_DRVMODEL_H) && defined(RWDEBUG)) */

        if (RxPipelineExecute(stash->pipeline, (void *) stash, FALSE) != NULL)
        {
            RWRETURN(TRUE);
        }
    }
    else
    {
        RWERROR((E_RX_IM3DNOTACTIVE));
    }

    RWRETURN(FALSE);
#else /* (!defined(SUPPRESS_IM3D)) */
    RWRETURN(TRUE);
#endif /* (!defined(SUPPRESS_IM3D)) */

}

/**
 * \ingroup rwim3d
 * \ref RwIm3DRenderTriangle executes the remainder of the
 * RwIm3D transform pipeline, (internally passing in the cached packets
 * containing the transformed vertices) and specifying the primitive
 * type and index data in the (void *) data parameter of \ref RxPipelineExecute.
 * It effectively renders a one-triangle triangle list.
 *
 * This function is very inefficient and should not be used to render many
 * individual triangles - for that purpose use \ref RwIm3DRenderPrimitive or
 * \ref RwIm3DRenderIndexedPrimitive.
 *
 * \param vert1  \ref RwInt32 index of the first vertex in the triangle
 * \param vert2  \ref RwInt32 index of the second vertex in the triangle
 * \param vert3  \ref RwInt32 index of the third vertex in the triangle.
 *
 * \return Returns TRUE on success, FALSE on error.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 *
 */
RwBool
RwIm3DRenderTriangle(RwInt32 vert1, RwInt32 vert2, RwInt32 vert3)
{
    RwBool              im3dactive;
    RxHeap             *heap;

    RWAPIFUNCTION(RWSTRING("RwIm3DRenderTriangle"));
#if (!defined(SUPPRESS_IM3D))

    im3dactive = (RWIMMEDIGLOBAL(curPool).elements != NULL);

    heap = RxHeapGetGlobalHeap();
    RWASSERT(NULL != heap);

    if (im3dactive)
    {
        _rwIm3DPoolStash    *stash = &RWIMMEDIGLOBAL(curPool).stash;

        stash->pipeline =
            (RxPipeline *) RWIMMEDIGLOBAL(im3DRenderPipelines).triList;
        stash->primType = (RwPrimitiveType) rwPRIMTYPETRILIST;
        stash->indices = (RxVertexIndex *) NULL;
        stash->numIndices = (RwUInt32) 3;

        stash->indices = (RxVertexIndex *)
            RxHeapAlloc(heap, sizeof(RxVertexIndex) * stash->numIndices);
        RWASSERT(NULL != stash->indices);

        /* Set up the indices for a one-triangle tri list */
        stash->indices[0] = (RxVertexIndex) vert1;
        stash->indices[1] = (RxVertexIndex) vert2;
        stash->indices[2] = (RxVertexIndex) vert3;

#if (defined(SKY2_DRVMODEL_H) && defined(RWDEBUG))
        /* Either use both the PS2All default transform AND render pipeline
         * or neither - mixing PS2All with non-PS2All is heap bad voodoo
         * [can also use PS2Manager render pipe w/ PS2All transform pipe] */

        /* NOTE: this test may become appropriate for other targets */
        {
            RxPipeline *curTrns, *ps2AllTrns;
            RxPipeline *ps2AllRnd, *ps2AllRnd2;

            curTrns = RWIMMEDIGLOBAL(im3DTransformPipeline);
            ps2AllTrns = RWIMMEDIGLOBAL(platformIm3DTransformPipeline);
            ps2AllRnd = RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triList;
            ps2AllRnd2 = RWIMMEDIGLOBAL(platformIm3DRenderPipelines).lineList;

            RWASSERT( ((curTrns == ps2AllTrns) &&
                       ((stash->pipeline == ps2AllRnd) ||
                        (stash->pipeline == ps2AllRnd2))) ||
                      ((curTrns != ps2AllTrns) &&
                       ((stash->pipeline != ps2AllRnd) &&
                        (stash->pipeline != ps2AllRnd2))) );
        }
#endif /* (defined(SKY2_DRVMODEL_H) && defined(RWDEBUG)) */

        VALIDATE_VERTICES(stash->indices,
                          stash->numIndices,
                          RWIMMEDIGLOBAL(curPool).numElements);

        if (RxPipelineExecute(stash->pipeline, (void *) stash, FALSE) != NULL)
        {
            RxHeapFree(heap, stash->indices);
            RWRETURN(TRUE);
        }
        RxHeapFree(heap, stash->indices);
    }
    else
    {
        RWERROR((E_RX_IM3DNOTACTIVE));
    }
    RWRETURN(FALSE);
#else /* (!defined(SUPPRESS_IM3D)) */
    RWRETURN(TRUE);
#endif /* (!defined(SUPPRESS_IM3D)) */
}

/**
 * \ingroup rwim3d
 * \ref RwIm3DRenderLine executes the remainder of the
 * RwIm3D transform pipeline, (internally passing in the cached packets
 * containing the transformed vertices) and specifying the primitive
 * type and index data in the (void *)data parameter of \ref RxPipelineExecute.
 * It effectively renders a one-triangle line list.
 *
 * This function is very inefficient and should not be used to render many
 * individual lines - for that purpose use \ref RwIm3DRenderPrimitive or
 * \ref RwIm3DRenderIndexedPrimitive.
 *
 * \param vert1  \ref RwInt32 index of the first vertex in the line
 * \param vert2  \ref RwInt32 index of the second vertex in the line.
 *
 * \return Returns TRUE on success, FALSE on error.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 *
 */
RwBool
RwIm3DRenderLine(RwInt32 vert1, RwInt32 vert2)
{
    RwBool              im3dactive;
    RxHeap             *heap;

    RWAPIFUNCTION(RWSTRING("RwIm3DRenderLine"));
#if (!defined(SUPPRESS_IM3D))

    heap = RxHeapGetGlobalHeap();
    RWASSERT(NULL != heap);

    im3dactive = (RWIMMEDIGLOBAL(curPool).elements != NULL);

    if (im3dactive)
    {
        _rwIm3DPoolStash    *stash = &RWIMMEDIGLOBAL(curPool).stash;

        stash->pipeline =
            (RxPipeline *) RWIMMEDIGLOBAL(im3DRenderPipelines).lineList;
        stash->primType = (RwPrimitiveType) rwPRIMTYPELINELIST;
        stash->indices = (RxVertexIndex *) NULL;
        stash->numIndices = (RwUInt32) 2;

        stash->indices = (RxVertexIndex *)
            RxHeapAlloc(heap, sizeof(RxVertexIndex) * stash->numIndices);
        RWASSERT(NULL != stash->indices);

        /* Set up the indices for a one-line line list */
        stash->indices[0] = (RxVertexIndex) vert1;
        stash->indices[1] = (RxVertexIndex) vert2;

#if (defined(SKY2_DRVMODEL_H) && defined(RWDEBUG))
        /* Either use both the PS2All default transform AND render pipeline
         * or neither - mixing PS2All with non-PS2All is heap bad voodoo
         * [can also use PS2Manager render pipe w/ PS2All transform pipe] */

        /* NOTE: this test may become appropriate for other targets */
        {
            RxPipeline *curTrns, *ps2AllTrns;
            RxPipeline *ps2AllRnd, *ps2AllRnd2;

            curTrns = RWIMMEDIGLOBAL(im3DTransformPipeline);
            ps2AllTrns = RWIMMEDIGLOBAL(platformIm3DTransformPipeline);
            ps2AllRnd = RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triList;
            ps2AllRnd2 = RWIMMEDIGLOBAL(platformIm3DRenderPipelines).lineList;

            RWASSERT( ((curTrns == ps2AllTrns) &&
                       ((stash->pipeline == ps2AllRnd) ||
                        (stash->pipeline == ps2AllRnd2))) ||
                      ((curTrns != ps2AllTrns) &&
                       ((stash->pipeline != ps2AllRnd) &&
                        (stash->pipeline != ps2AllRnd2))) );
        }
#endif /* (defined(SKY2_DRVMODEL_H) && defined(RWDEBUG)) */

        VALIDATE_VERTICES(stash->indices,
                          stash->numIndices,
                          RWIMMEDIGLOBAL(curPool).numElements);

        if (RxPipelineExecute(stash->pipeline, (void *) stash, FALSE) != NULL)
        {
            RxHeapFree(heap, stash->indices);
            RWRETURN(TRUE);
        }
        RxHeapFree(heap, stash->indices);
    }
    else
    {
        RWERROR((E_RX_IM3DNOTACTIVE));
    }
    RWRETURN(FALSE);
#else /* (!defined(SUPPRESS_IM3D)) */
    RWRETURN(TRUE);
#endif /* (!defined(SUPPRESS_IM3D)) */
}

/**
 * \ingroup rwim3d
 * \ref RwIm3DGetTransformPipeline returns the current RwIm3D
 * transform pipeline.
 *
 * The RwIm3D transform pipeline transforms the vertices submitted to
 * \ref RwIm3DTransform, generating camera-space and screen-space vertices
 * and caching these in a packet when the pipeline is interrupted. When the
 * pipeline is resumed by a call to a RwIm3D render function, primitive
 * information (usually including indices) is added into the packet and
 * it is dispatched to the appropriate RwIm3D render pipeline for the
 * specified primitive type.
 *
 * The default RwIm3D transform pipeline is platform-specific, see
 * \ref RwIm3DGetTransformPipelineplatform for details.
 *
 * \return A pointer to the current RwIm3D transform pipeline on success,
 * otherwise NULL.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 */
RxPipeline         *
RwIm3DGetTransformPipeline(void)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DGetTransformPipeline"));
#if (!defined(SUPPRESS_IM3D))

    RWASSERT(_rwIm3DModule.numInstances);

#endif /* (!defined(SUPPRESS_IM3D)) */
    RWRETURN(RWIMMEDIGLOBAL(im3DTransformPipeline));
}

/**
 * \ingroup rwim3d
 * \ref RwIm3DGetRenderPipeline returns the current RwIm3D
 * render pipeline associated with a given primitive type
 *
 * RwIm3D render pipelines do not perform lighting. They merely clip, cull and
 * submit triangles to the rasterizer.
 *
 * The default RwIm3D transform pipeline is platform-specific, see
 * \ref RwIm3DGetRenderPipelineplatform for details.
 *
 * \param primType  An \ref RwPrimitiveType equal to the primitive type.
 *
 * \return A pointer to the current RwIm3D render pipeline for the specified
 * primitive type on success, otherwise NULL.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 */
RxPipeline         *
RwIm3DGetRenderPipeline(RwPrimitiveType primType)
{
    RxPipeline         *result = (RxPipeline *)NULL;

    RWAPIFUNCTION(RWSTRING("RwIm3DGetRenderPipeline"));

#if (!defined(SUPPRESS_IM3D))
    RWASSERT(_rwIm3DModule.numInstances);

    RWASSERT(RWPRIMTYPEVALID(primType));

    switch (primType)
    {
        case rwPRIMTYPETRILIST:
            result = RWIMMEDIGLOBAL(im3DRenderPipelines).triList;
            RWASSERT(NULL != result);
            break;
        case rwPRIMTYPETRIFAN:
            result = RWIMMEDIGLOBAL(im3DRenderPipelines).triFan;
            RWASSERT(NULL != result);
            break;
        case rwPRIMTYPETRISTRIP:
            result = RWIMMEDIGLOBAL(im3DRenderPipelines).triStrip;
            RWASSERT(NULL != result);
            break;
        case rwPRIMTYPELINELIST:
            result = RWIMMEDIGLOBAL(im3DRenderPipelines).lineList;
            RWASSERT(NULL != result);
            break;
        case rwPRIMTYPEPOLYLINE:
            result = RWIMMEDIGLOBAL(im3DRenderPipelines).polyLine;
            RWASSERT(NULL != result);
            break;
        case rwPRIMTYPEPOINTLIST:
            result = RWIMMEDIGLOBAL(im3DRenderPipelines).pointList;
            RWASSERT(NULL != result);
            break;
        default:
            RWERROR((E_RX_INVALIDPRIMTYPE, primType));
            break;
    }

#endif /* (!defined(SUPPRESS_IM3D)) */
    RWRETURN(result);
}

/**
 * \ingroup rwim3d
 * \ref RwIm3DSetTransformPipeline sets the RwIm3D transform
 * pipeline.
 *
 * \param pipeline  A pointer to the \ref RxPipeline to make the current RwIm3D transform
 *       pipeline - if NULL is passed, the default pipeline is restored.
 *
 * \return A pointer to the updated current RwIm3D transform pipeline.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 *
 */

RxPipeline         *
RwIm3DSetTransformPipeline(RxPipeline * pipeline)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DSetTransformPipeline"));
#if (!defined(SUPPRESS_IM3D))
    RWASSERT(_rwIm3DModule.numInstances);

    if (pipeline != NULL)
    {
        RWIMMEDIGLOBAL(im3DTransformPipeline) = pipeline;
    }
    else
    {
        if (RWIMMEDIGLOBAL(platformIm3DTransformPipeline))
        {
            RWIMMEDIGLOBAL(im3DTransformPipeline) =
                RWIMMEDIGLOBAL(platformIm3DTransformPipeline);
        }
        else
        {
            RWIMMEDIGLOBAL(im3DTransformPipeline) = NULL;
        }
    }
    RWRETURN(RWIMMEDIGLOBAL(im3DTransformPipeline));

#endif /* (!defined(SUPPRESS_IM3D)) */

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwim3d
 * \ref RwIm3DSetRenderPipeline sets the RwIm3D
 * render pipeline associated with a given primitive type.
 *
 * \param pipeline  A pointer to the \ref RxPipeline to make the current RwIm3D render pipeline for
 *        the specified primitive type - if NULL is passed, the default pipeline is restored
 * \param primType  An \ref RwPrimitiveType equal to the primitive type.
 *
 * \return updated current RwIm3D transform pipeline.
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see \ref RwIm3DGetRenderPipelineplatform
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetTransformPipeline
 *
 */
RxPipeline         *
RwIm3DSetRenderPipeline(RxPipeline * pipeline, RwPrimitiveType primType)
{
    RWAPIFUNCTION(RWSTRING("RwIm3DSetRenderPipeline"));

#if (!defined(SUPPRESS_IM3D))
    RWASSERT(_rwIm3DModule.numInstances);

    if (pipeline != NULL)
    {
        /* Set the appropriate RwIm3D pipeline */

        RWASSERT(RWPRIMTYPEVALID(primType));

        switch (primType)
        {
            case rwPRIMTYPETRILIST:
                RWIMMEDIGLOBAL(im3DRenderPipelines).triList = pipeline;
                RWRETURN(pipeline);
                break;
            case rwPRIMTYPETRIFAN:
                RWIMMEDIGLOBAL(im3DRenderPipelines).triFan = pipeline;
                RWRETURN(pipeline);
                break;
            case rwPRIMTYPETRISTRIP:
                RWIMMEDIGLOBAL(im3DRenderPipelines).triStrip = pipeline;
                RWRETURN(pipeline);
                break;
            case rwPRIMTYPELINELIST:
                RWIMMEDIGLOBAL(im3DRenderPipelines).lineList = pipeline;
                RWRETURN(pipeline);
                break;
            case rwPRIMTYPEPOLYLINE:
                RWIMMEDIGLOBAL(im3DRenderPipelines).polyLine = pipeline;
                RWRETURN(pipeline);
                break;
            case rwPRIMTYPEPOINTLIST:
                RWIMMEDIGLOBAL(im3DRenderPipelines).pointList = pipeline;
                RWRETURN(pipeline);
                break;
            default:
                RWERROR((E_RX_INVALIDPRIMTYPE, primType));
                break;
        }
    }
    else
    {
        /* Set the appropriate RwIm3D pipeline to a default pipeline */

        RWASSERT(RWPRIMTYPEVALID(primType));

        switch (primType)
        {
            case rwPRIMTYPETRILIST:
                if (RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triList
                    != NULL)
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).triList =
                        RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triList;
                }
                else
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).triList = NULL;
                }
                RWASSERT(NULL != RWIMMEDIGLOBAL(im3DRenderPipelines).triList);

                RWRETURN(RWIMMEDIGLOBAL(im3DRenderPipelines).triList);
                break;
            case rwPRIMTYPETRIFAN:
                if (RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triFan
                    != NULL)
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).triFan =
                        RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triFan;
                }
                else
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).triFan = NULL;
                }
                RWASSERT(NULL != RWIMMEDIGLOBAL(im3DRenderPipelines).triFan);

                RWRETURN(RWIMMEDIGLOBAL(im3DRenderPipelines).triFan);
                break;
            case rwPRIMTYPETRISTRIP:
                if (RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triStrip
                    != NULL)
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).triStrip =
                        RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triStrip;
                }
                else
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).triStrip = NULL;
                }
                RWASSERT(NULL !=
                         RWIMMEDIGLOBAL(im3DRenderPipelines).triStrip);

                RWRETURN(RWIMMEDIGLOBAL
                         (platformIm3DRenderPipelines).triStrip);
                break;
            case rwPRIMTYPELINELIST:
                if (RWIMMEDIGLOBAL(platformIm3DRenderPipelines).lineList
                    != NULL)
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).lineList =
                        RWIMMEDIGLOBAL(platformIm3DRenderPipelines).lineList;
                }
                else
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).lineList = NULL;
                }
                RWASSERT(NULL !=
                         RWIMMEDIGLOBAL(im3DRenderPipelines).lineList);

                RWRETURN(RWIMMEDIGLOBAL
                         (platformIm3DRenderPipelines).lineList);
                break;
            case rwPRIMTYPEPOLYLINE:
                if (RWIMMEDIGLOBAL(platformIm3DRenderPipelines).polyLine
                    != NULL)
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).polyLine =
                        RWIMMEDIGLOBAL(platformIm3DRenderPipelines).polyLine;
                }
                else
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).polyLine = NULL;
                }
                RWASSERT(NULL !=
                         RWIMMEDIGLOBAL(im3DRenderPipelines).polyLine);

                RWRETURN(RWIMMEDIGLOBAL
                         (platformIm3DRenderPipelines).polyLine);
                break;
            case rwPRIMTYPEPOINTLIST:
                if (RWIMMEDIGLOBAL(platformIm3DRenderPipelines).pointList
                    != NULL)
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).pointList =
                        RWIMMEDIGLOBAL(platformIm3DRenderPipelines).pointList;
                }
                else
                {
                    RWIMMEDIGLOBAL(im3DRenderPipelines).pointList = NULL;
                }
                RWASSERT(NULL !=
                         RWIMMEDIGLOBAL(im3DRenderPipelines).pointList);

                RWRETURN(RWIMMEDIGLOBAL
                         (platformIm3DRenderPipelines).pointList);
                break;
            default:
                RWERROR((E_RX_INVALIDPRIMTYPE, primType));
                break;

        }
    }
#endif /* (!defined(SUPPRESS_IM3D)) */
    RWRETURN((RxPipeline *)NULL);
}


/****************************************************************************
 rwIm3DPool

  Used to return the pool of vertces for immediate 3D rendering.
  It returns a (rwIm3DPool *) pointer to the pool.
  It's an SPI function.
 */
rwIm3DPool         *
_rwIm3DGetPool(void)
{
    rwIm3DPool         *pool;

    RWFUNCTION(RWSTRING("_rwIm3DGetPool"));

    pool = &(RWIMMEDIGLOBAL(curPool));

    RWRETURN(pool);
}

/****************************************************************************
 _rwIm3DClose
 */
void               *
_rwIm3DClose(void *instance, RwInt32 __RWUNUSED__ offset,
             RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwIm3DClose"));
#if (!defined(SUPPRESS_IM3D))

    RWASSERT(_rwIm3DModule.numInstances);

    /* Clean up target-specific RwIm3D pipelines */
    _rwIm3DDestroyPlatformRenderPipelines(
        &(RWIMMEDIGLOBAL(platformIm3DRenderPipelines)));
    _rwIm3DDestroyPlatformTransformPipeline(
        &(RWIMMEDIGLOBAL(platformIm3DTransformPipeline)));

    _rwIm3DModule.numInstances--;

#endif /* (!defined(SUPPRESS_IM3D)) */

    RWRETURN(instance);

}

/****************************************************************************
 _rwIm3DOpen
 */
void               *
_rwIm3DOpen(void *instance, RwInt32 offset, RwInt32 size)
{
    RwBool              success = TRUE;

    RWFUNCTION(RWSTRING("_rwIm3DOpen"));

#if (!defined(SUPPRESS_IM3D))

    RWASSERT(!_rwIm3DModule.numInstances);

    /* Cache the globals offset (same for all instances) */
    _rwIm3DModule.globalsOffset = offset;

    /* Temporary, for debugging */
    _rwIm3DGlobals =
        (rwImmediGlobals *) & RWIMMEDIGLOBAL(im3DTransformPipeline);

    /* One more module instance */
    _rwIm3DModule.numInstances++;

    /* Pipes and curPool initialised to NULL/0 */
    memset(&(RWIMMEDIGLOBAL(im3DTransformPipeline)), 0,
           sizeof(rwImmediGlobals));

    /* Allow creation of platform-specific pipes to override the
     * generic ones as defaults */
    if (success != FALSE)
    {
        success = _rwIm3DCreatePlatformTransformPipeline(
                      &(RWIMMEDIGLOBAL(platformIm3DTransformPipeline)));
    }

    if (success != FALSE)
    {
        success = _rwIm3DCreatePlatformRenderPipelines(
                      &(RWIMMEDIGLOBAL(platformIm3DRenderPipelines)));
    }

    if (success != FALSE)
    {
        RWRETURN(instance);
    }

    RwDebugSendMessage(rwDEBUGMESSAGE,
                       "_rwIm3DOpen",
                       "Failed to create generic and/or platform-specific default Im3D transform/render pipelines");

    /* This should clean up safely */
    _rwIm3DClose(instance, offset, size);

    /* Oops... */
    RWRETURN(NULL);

#else /* (!defined(SUPPRESS_IM3D)) */

    RWRETURN(instance);

#endif /* (!defined(SUPPRESS_IM3D)) */

}
