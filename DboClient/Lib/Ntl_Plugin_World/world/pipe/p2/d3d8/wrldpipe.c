#include <d3d8.h>

#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rwcore.h"

#include "balight.h"
#include "baworld.h"

#include "bapipew.h"

#include "nodeD3D8AtomicAllInOne.h"
#include "nodeD3D8WorldSectorAllInOne.h"

#include "D3D8VertexBufferManager.h"
#include "D3D8lights.h"

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

    result = _rpD3D8LightPluginAttach();

    RWRETURN(result);
}

/**
 * \ingroup rpmateriald3d8
 * \page RpMaterialGetDefaultPipelineplatform RpMaterialGetDefaultPipeline (platform-specific)
 *
 * The default material pipeline for D3D8 are ignored.
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
    RWFUNCTION(RWSTRING("_rpCreatePlatformMaterialPipelines"));

    RWRETURN(TRUE);
}

void
_rpDestroyPlatformMaterialPipelines(void)
{
    RWFUNCTION(RWSTRING("_rpDestroyPlatformMaterialPipelines"));

    RWRETURNVOID();
}

/**
 * \ingroup rpworldsubd3d8
 * \page RpWorldGetDefaultSectorPipelineplatform RpWorldGetDefaultSectorPipeline (platform-specific)
 *
 * The default world sector object pipeline for D3D8 is shown below.
 *
 * The default world sector object pipeline:
 * \verbatim
     nodeD3D8WorldSectorAllInOne.csl
   \endverbatim
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
 */
RwBool
_rpCreatePlatformWorldSectorPipelines(void)
{
    RxPipeline  *pipe;

    RWFUNCTION(RWSTRING("_rpCreatePlatformWorldSectorPipelines"));

    pipe = RxPipelineCreate();
    if (pipe)
    {
        RxLockedPipe    *lpipe;

        pipe->pluginId = rwID_WORLDPLUGIN;
        if (NULL != (lpipe = RxPipelineLock(pipe)))
        {
            lpipe = RxLockedPipeAddFragment(lpipe,
                NULL,
                RxNodeDefinitionGetD3D8WorldSectorAllInOne(),
                NULL);

            lpipe = RxLockedPipeUnlock(lpipe);

            RWASSERT(pipe == (RxPipeline *)lpipe);

            if (NULL != lpipe)
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
 * \ingroup rpatomicd3d8
 * \page RpAtomicGetDefaultPipelineplatform RpAtomicGetDefaultPipeline (platform-specific)
 *
 * The default atomic object pipeline for D3D8 is shown below.
 *
 * The default atomic object pipeline:
 * \verbatim
     nodeD3D8AtomicAllInOne.csl
  \endverbatim
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
    RxPipeline  *pipe;

    RWFUNCTION(RWSTRING("_rpCreatePlatformAtomicPipelines"));

    pipe = RxPipelineCreate();
    if (pipe)
    {
        RxLockedPipe    *lpipe;

        pipe->pluginId = rwID_WORLDPLUGIN;
        if (NULL != (lpipe = RxPipelineLock(pipe)))
        {
            lpipe = RxLockedPipeAddFragment(lpipe,
                NULL,
                RxNodeDefinitionGetD3D8AtomicAllInOne(),
                NULL);

            lpipe = RxLockedPipeUnlock(lpipe);

            RWASSERT(pipe == (RxPipeline *)lpipe);

            if (NULL != lpipe)
            {
                RXPIPELINEGLOBAL(platformAtomicPipeline) = pipe;
                RpAtomicSetDefaultPipeline(pipe);

                if(_rxD3D8VertexBufferManagerOpen())
                {
                    if(_rwD3D8LightsOpen())
                    {
                        RWRETURN(TRUE);
                    }
                }
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

    _rwD3D8LightsClose();

    _rxD3D8VertexBufferManagerClose();

    RpAtomicSetDefaultPipeline(NULL);
    if (NULL != RXPIPELINEGLOBAL(platformAtomicPipeline))
    {
        RxPipelineDestroy(RXPIPELINEGLOBAL(platformAtomicPipeline));
        RXPIPELINEGLOBAL(platformAtomicPipeline) = NULL;
    }

    RWRETURNVOID();
}
