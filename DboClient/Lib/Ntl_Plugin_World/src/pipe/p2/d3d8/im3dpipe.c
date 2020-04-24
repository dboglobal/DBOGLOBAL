/*
 * Immediate mode rendering (platform-specific)
 *
 * The documentation in here is platform-specific versions of function-
 * -descriptions for functions in baim3d.c
 *
 * Copyright (c) Criterion Software Limited
 */

/***************************************************************************
 *                                                                         *
 * Module  : im3dpipe.c                                                    *
 *                                                                         *
 * Purpose : Platform specific pipeline construction                       *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

#include "batypes.h"

#include "bapipe.h"
#include "p2core.h"
#include "p2define.h"
#include "p2stdcls.h"

#include "im3dpipe.h"

#include "nodeD3D8SubmitNoLight.h"

/**
 * \ingroup rwim3dd3d8
 * \page RwIm3DGetTransformPipelineplatform RwIm3DGetTransformPipeline (platform-specific)
 *
 * The Im3D transform pipeline for D3D8 is shown below. Note that this
 * pipeline is designed specifically to leverage hardware transformation and
 * lighting support. If you wish to use an Im3D transform pipeline that does
 * transformation itself, then you will need to replace the default Im3D
 * transform pipeline with the generic Im3D transform pipeline.
 *
 * \verbatim
   The default Im3D transform pipeline:

     ImmInstance.csl
     \endverbatim
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
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 *
 */
RwBool
_rwIm3DCreatePlatformTransformPipeline(RxPipeline **globalPipe)
{
    RxPipeline  *pipe;

    RWFUNCTION(RWSTRING("_rwIm3DCreatePlatformTransformPipeline"));

    RWASSERT(globalPipe != NULL);

    pipe = RxPipelineCreate();
    if (pipe)
    {
        RxPipeline  *lpipe;

        pipe->pluginId = rwID_COREPLUGIN;
        lpipe = RxPipelineLock(pipe);
        if (lpipe)
        {
            /* Add the fragment */
            lpipe = RxLockedPipeAddFragment(lpipe,
                        NULL,
                        RxNodeDefinitionGetD3D8ImmInstance(),
                        NULL);  /* Dispatch to mat pipe */
            RWASSERT(lpipe != NULL);

            pipe = RxLockedPipeUnlock(lpipe);
            RWASSERT(pipe != NULL);
            RWASSERT(pipe == lpipe);

            if (pipe != NULL)
            {
               *globalPipe = pipe;
                RwIm3DSetTransformPipeline(pipe);

                RWRETURN(TRUE);
            }
        }

        /* Failed to lock it */
        RxPipelineDestroy(pipe);
    }

    /* Failed to create it - ran out of memory? */
    RWRETURN(FALSE);
}

void
_rwIm3DDestroyPlatformTransformPipeline(RxPipeline **globalPipe)
{
    RWFUNCTION(RWSTRING("_rwIm3DDestroyPlatformTransformPipeline"));

    RWASSERT(globalPipe != NULL);

    /* Resinstate the generic transform pipe */
    RwIm3DSetTransformPipeline(NULL);
    if (NULL != *globalPipe)
    {
        RxPipelineDestroy(*globalPipe);
       *globalPipe = NULL;
    }

    RWRETURNVOID();
}

void
_rwIm3DDestroyPlatformRenderPipelines(rwIm3DRenderPipelines *globalPipes)
{
    RWFUNCTION(RWSTRING("_rwIm3DDestroyPlatformRenderPipelines"));

    RWASSERT(globalPipes != NULL);

    _rwD3D8Im3DRenderClose();

    /* Resinstate the generic lines pipes pipe */
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPETRILIST);
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPETRIFAN);
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPETRISTRIP);
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPELINELIST);
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPEPOLYLINE);

    if (NULL != globalPipes->triList)
    {
        RxPipelineDestroy(globalPipes->triList);
    }
    globalPipes->triList  = NULL;
    globalPipes->triFan   = NULL;
    globalPipes->triStrip = NULL;
    globalPipes->lineList = NULL;
    globalPipes->polyLine = NULL;

    RWRETURNVOID();
}

/**
 * \ingroup rwim3dd3d8
 * \page RwIm3DGetRenderPipelineplatform RwIm3DGetRenderPipeline (platform-specific)
 *
 * The Im3D render pipelines for D3D8 are shown below. There is one for
 * rendering all primitive types. Note that these pipelines are designed
 * specifically to leverage hardware transformation and lighting support. If
 * you wish to use Im3D render pipelines that do transformation and lighting
 * (and anything else additionally) themselves, then you will need to replace
 * the relevant Im3D render pipeline one based on the generic Im3D render
 * pipelines.
 *
 * The D3D8 pipelines support triangle lists, strips and fans and line
 * lists and polylines.
 *
 * \verbatim
   The default Im3D render pipeline:

     nodeD3D8SubmitNoLight.csl
     \endverbatim
 *
 * \see RwIm3DEnd
 * \see RwIm3DRenderIndexedPrimitive
 * \see RwIm3DRenderLine
 * \see RwIm3DRenderPrimitive
 * \see RwIm3DRenderTriangle
 * \see RwIm3DTransform
 * \see RwIm3DGetRenderPipeline
 * \see RwIm3DGetTransformPipeline
 * \see \ref RwIm3DGetTransformPipelineplatform
 * \see RwIm3DSetRenderPipeline
 * \see RwIm3DSetTransformPipeline
 *
 */
RwBool
_rwIm3DCreatePlatformRenderPipelines(rwIm3DRenderPipelines *globalPipes)
{
    RxPipeline  *pipe;

    RWFUNCTION(RWSTRING("_rwIm3DCreatePlatformRenderPipelines"));

    RWASSERT(globalPipes != NULL);

    /* Same pipe for all primitive types, yay! */

    pipe = RxPipelineCreate();
    if (pipe)
    {
        RxLockedPipe    *lpipe;

        pipe->pluginId = rwID_COREPLUGIN;
        lpipe = RxPipelineLock(pipe);
        if (lpipe)
        {
            lpipe = RxLockedPipeAddFragment(lpipe,
                NULL,
                RxNodeDefinitionGetD3D8SubmitNoLight(),
                NULL);
            RWASSERT(lpipe != NULL);

            pipe = RxLockedPipeUnlock(lpipe);
            RWASSERT(pipe != NULL);
            RWASSERT(pipe == lpipe);

            if (NULL != pipe)
            {
                globalPipes->triList  = pipe;
                globalPipes->triFan   = pipe;
                globalPipes->triStrip = pipe;
                globalPipes->lineList = pipe;
                globalPipes->polyLine = pipe;

                /* Set our pipe as the default for all primitives... */
                RwIm3DSetRenderPipeline(pipe, rwPRIMTYPETRILIST);
                RwIm3DSetRenderPipeline(pipe, rwPRIMTYPETRIFAN);
                RwIm3DSetRenderPipeline(pipe, rwPRIMTYPETRISTRIP);
                RwIm3DSetRenderPipeline(pipe, rwPRIMTYPELINELIST);
                RwIm3DSetRenderPipeline(pipe, rwPRIMTYPEPOLYLINE);

                RWRETURN(_rwD3D8Im3DRenderOpen());
            }
        }

        /* Failed to lock/add/unlock the pipe */
        RxPipelineDestroy(pipe);
    }

    /* Failed to create the pipe - ran out of memory? */
    RWRETURN(FALSE);
}
