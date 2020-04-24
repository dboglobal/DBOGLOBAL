/* The documentation in here is platform-specific versions of function-
   -descriptions for functions in bapipew.c */
/*
 * Manipulating world object custom pipelines (platform-specific)
 *
 * Copyright (c) Criterion Software Limited
 */
/* #include <assert.h> */
#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rwcore.h"

#include "balight.h"
#include "baworld.h"

#include "bapipew.h"

#include "wrldpipe.h"



/****************************************************************************
 _rxWorldDevicePluginAttach()
 */

RwBool
_rxWorldDevicePluginAttach(void)
{
    RwBool result = FALSE; /* fail, unless explicitly set TRUE */

    RWFUNCTION(RWSTRING("_rxWorldDevicePluginAttach"));

    /* device-specific set-up... */

    result = TRUE;

    RWRETURN(result);
}

/**
 * \ingroup rpworldnull
 * \page RpMaterialGetDefaultPipelineplatform RpMaterialGetDefaultPipeline (platform-specific)
 *
 * The default material pipeline for the NULL driver is empty,
 * as the NULL driver is not meant to render anything.
 *
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldSetDefaultSectorPipeline
 *
 */
RwBool
_rpCreatePlatformMaterialPipelines(void)
{
    RxPipeline        *pipe;

    RWFUNCTION(RWSTRING("_rpCreatePlatformMaterialPipelines"));

    pipe = RxPipelineCreate();
    if (pipe)
    {
        RxLockedPipe       *lpipe;

        pipe->pluginId = rwID_WORLDPLUGIN;
        lpipe = RxPipelineLock(pipe);

        if (NULL != lpipe)
        {
            pipe = RxLockedPipeUnlock(lpipe);

            RWASSERT(NULL != pipe);
            RWASSERT(pipe == lpipe);

            if (pipe != NULL)
            {
                RXPIPELINEGLOBAL(platformMaterialPipeline) = pipe;
                RpMaterialSetDefaultPipeline(pipe);

                RWRETURN(TRUE);
            }
        }

        RxPipelineDestroy(pipe);
    }

    RWRETURN(FALSE);
}

void
_rpDestroyPlatformMaterialPipelines(void)
{
    RWFUNCTION(RWSTRING("_rpDestroyPlatformMaterialPipelines"));

    RpMaterialSetDefaultPipeline(NULL);
    if (NULL != RXPIPELINEGLOBAL(platformMaterialPipeline))
    {
        RxPipelineDestroy(RXPIPELINEGLOBAL(platformMaterialPipeline));
        RXPIPELINEGLOBAL(platformMaterialPipeline) = NULL;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rpworldnull
 * \page RpWorldGetDefaultSectorPipelineplatform RpWorldGetDefaultSectorPipeline (platform-specific)
 *
 * The default world sector object pipeline for the NULL driver is empty,
 * as the NULL driver is not meant to render anything.
 *
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see RpWorldSetDefaultSectorPipeline
 *
 */
RwBool
_rpCreatePlatformWorldSectorPipelines(void)
{
    RxPipeline        *pipe;

    RWFUNCTION(RWSTRING("_rpCreatePlatformWorldSectorPipelines"));

    pipe = RxPipelineCreate();
    if (pipe)
    {
        RxLockedPipe       *lpipe;

        pipe->pluginId = rwID_WORLDPLUGIN;
        lpipe = RxPipelineLock(pipe);

        if (NULL != lpipe)
        {
            pipe = RxLockedPipeUnlock(lpipe);

            RWASSERT(NULL != pipe);
            RWASSERT(pipe == lpipe);

            if (pipe != NULL)
            {
                RXPIPELINEGLOBAL(platformWorldSectorPipeline) = pipe;
                RpWorldSetDefaultSectorPipeline(pipe);

                RWRETURN(TRUE);
            }
        }

        RxPipelineDestroy(pipe);
    }

    RWRETURN(FALSE);
}

void
_rpDestroyPlatformWorldSectorPipelines(void)
{
    RWFUNCTION(RWSTRING("_rpDestroyPlatformWorldSectorPipelines"));

    RpWorldSetDefaultSectorPipeline(NULL);
    if (NULL != RXPIPELINEGLOBAL(platformWorldSectorPipeline))
    {
        RxPipelineDestroy(RXPIPELINEGLOBAL(platformWorldSectorPipeline));
        RXPIPELINEGLOBAL(platformWorldSectorPipeline) = NULL;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rpworldnull
 * \page RpAtomicGetDefaultPipelineplatform RpAtomicGetDefaultPipeline (platform-specific)
 *
 * The default atomic object pipeline for the NULL driver is empty,
 * as the NULL driver is not meant to render anything.
 *
 * \see RpAtomicGetDefaultPipeline
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 */
RwBool
_rpCreatePlatformAtomicPipelines(void)
{
    RxPipeline        *pipe;

    RWFUNCTION(RWSTRING("_rpCreatePlatformAtomicPipelines"));

    pipe = RxPipelineCreate();
    if (pipe)
    {
        RxLockedPipe       *lpipe;

        pipe->pluginId = rwID_WORLDPLUGIN;
        lpipe = RxPipelineLock(pipe);

        if (NULL != lpipe)
        {
            pipe = RxLockedPipeUnlock(lpipe);

            RWASSERT(NULL != pipe);
            RWASSERT(pipe == lpipe);

            if (pipe != NULL)
            {
                RXPIPELINEGLOBAL(platformAtomicPipeline) = pipe;
                RpAtomicSetDefaultPipeline(pipe);

                RWRETURN(TRUE);
            }
        }

        RxPipelineDestroy(pipe);
    }

    RWRETURN(FALSE);
}

void
_rpDestroyPlatformAtomicPipelines(void)
{
    RWFUNCTION(RWSTRING("_rpDestroyPlatformAtomicPipelines"));

    RpAtomicSetDefaultPipeline(NULL);
    if (NULL != RXPIPELINEGLOBAL(platformAtomicPipeline))
    {
        RxPipelineDestroy(RXPIPELINEGLOBAL(platformAtomicPipeline));
        RXPIPELINEGLOBAL(platformAtomicPipeline) = NULL;
    }

    RWRETURNVOID();
}

