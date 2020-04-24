#include <d3d9.h>

#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rwcore.h"

#include "balight.h"
#include "baworld.h"

#include "bapipew.h"

#include "nodeD3D9AtomicAllInOne.h"
#include "nodeD3D9WorldSectorAllInOne.h"

#include "D3D9lights.h"
#include "D3D9vertexshader.h"
#include "d3d9usage.h"

#include "wrldpipe.h"

typedef struct _D3D9DynamicGeometry _D3D9DynamicGeometry;
struct _D3D9DynamicGeometry
{
    RpGeometry              *geometry;
    _D3D9DynamicGeometry    *next;
};

static rwD3D9DeviceRestoreCallBack D3D9OldRestoreDeviceCallback = NULL;

static _D3D9DynamicGeometry *D3D9DynamicGeometry = NULL;
static RwFreeList           *D3D9DynamicGeometryFreeList = NULL;

/****************************************************************************
 D3D9WorldRestoreCallback
 */
static void
D3D9WorldRestoreCallback(void)
{
    _D3D9DynamicGeometry *current;

    #if defined(RWDEBUG)
    RwUInt32 updatedGeometries = 0;
    #endif

    RWFUNCTION(RWSTRING("D3D9WorldRestoreCallback"));

    if (D3D9OldRestoreDeviceCallback != NULL)
    {
        D3D9OldRestoreDeviceCallback();
    }

    current = D3D9DynamicGeometry;
    while (current != NULL)
    {
        RWASSERT(current->geometry != NULL);
        RWASSERT(RWD9DOBJECTISGEOMETRY(current->geometry));

        current->geometry->lockedSinceLastInst = rpGEOMETRYLOCKALL;

        #if defined(RWDEBUG)
        updatedGeometries++;
        #endif

        current = current->next;
    }

    #if defined(RWDEBUG)
    if (updatedGeometries)
    {
        RwChar  buffer[256];

        rwsprintf(buffer, "Num Geometries Updated: %d", updatedGeometries);

        RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9WorldRestoreCallback",
                           buffer);
    }
    #endif

    RWRETURNVOID();
}

/****************************************************************************
 _rpD3D9AddDynamicGeometry
 */
void
_rpD3D9AddDynamicGeometry(RpGeometry *geometry)
{
    _D3D9DynamicGeometry *current;

    RWFUNCTION(RWSTRING("_rpD3D9AddDynamicGeometry"));
    RWASSERT(geometry != NULL);
    RWASSERT(RWD9DOBJECTISGEOMETRY(geometry));

    /* Check not already on the list */
    current = D3D9DynamicGeometry;
    while (current != NULL)
    {
        RWASSERT(current->geometry != NULL);

        if (current->geometry == geometry)
        {
            RWRETURNVOID();
        }

        current = current->next;
    }

    if (D3D9DynamicGeometryFreeList == NULL)
    {
        D3D9DynamicGeometryFreeList =
            RwFreeListCreate(sizeof(_D3D9DynamicGeometry), 128, sizeof(RwUInt32),
                             rwMEMHINTDUR_EVENT | rwID_WORLDPIPEMODULE);
    }

    /* Add to the beginning */
    current = (_D3D9DynamicGeometry *)RwFreeListAlloc(D3D9DynamicGeometryFreeList,
        rwMEMHINTDUR_EVENT | rwID_WORLDPIPEMODULE);
    current->geometry = geometry;
    current->next = D3D9DynamicGeometry;

    D3D9DynamicGeometry = current;

    RWRETURNVOID();
}

/****************************************************************************
 _rpD3D9RemoveDynamicGeometry
 */
void
_rpD3D9RemoveDynamicGeometry(RpGeometry *geometry)
{
    _D3D9DynamicGeometry *current, *previous;

    RWFUNCTION(RWSTRING("_rpD3D9RemoveDynamicGeometry"));
    RWASSERT(geometry != NULL);
    RWASSERT(RWD9DOBJECTISGEOMETRY(geometry));

    previous = NULL;
    current = D3D9DynamicGeometry;
    while (current != NULL)
    {
        RWASSERT(current->geometry != NULL);
        RWASSERT(RWD9DOBJECTISGEOMETRY(current->geometry));

        if (current->geometry == geometry)
        {
            break;
        }

        previous = current;
        current = current->next;
    }

    if (current != NULL)
    {
        if (current == D3D9DynamicGeometry)
        {
            D3D9DynamicGeometry = current->next;
        }
        else
        {
            previous->next = current->next;
        }

        current->geometry = NULL;

        RwFreeListFree(D3D9DynamicGeometryFreeList, current);
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpD3D9DynamicGeometryClose
 */
static void
_rpD3D9DynamicGeometryClose(void)
{
    RWFUNCTION(RWSTRING("_rpD3D9DynamicGeometryClose"));

    if (D3D9DynamicGeometry != NULL)
    {
        _D3D9DynamicGeometry *current;

        RWASSERT(D3D9DynamicGeometryFreeList != NULL);

        current = D3D9DynamicGeometry;
        do
        {
            RWASSERT(current->geometry != NULL);
            RWASSERT(RWD9DOBJECTISGEOMETRY(current->geometry));

            current->geometry = NULL;

            RwFreeListFree(D3D9DynamicGeometryFreeList, current);

            current = current->next;
        }
        while (current != NULL);

        D3D9DynamicGeometry = NULL;
    }

    if (D3D9DynamicGeometryFreeList != NULL)
    {
        RwFreeListDestroy(D3D9DynamicGeometryFreeList);
        D3D9DynamicGeometryFreeList = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rxWorldDevicePluginAttach()
 */

RwBool
_rxWorldDevicePluginAttach(void)
{
    RwBool result = FALSE; /* fail, unless explicitly set TRUE */

    RWFUNCTION(RWSTRING("_rxWorldDevicePluginAttach"));

    /* device-specific set-up... */
    result = _rpD3D9LightPluginAttach();

    result = result && _rpD3D9UsageFlagPluginAttach();

    D3D9OldRestoreDeviceCallback = _rwD3D9DeviceGetRestoreCallback();

    _rwD3D9DeviceSetRestoreCallback(D3D9WorldRestoreCallback);

    RWRETURN(result);
}

/**
 * \ingroup rpmateriald3d9
 * \page RpMaterialGetDefaultPipelineplatform RpMaterialGetDefaultPipeline (platform-specific)
 *
 * The default material pipeline for D3D9 are ignored.
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
 * \ingroup rpworldsubd3d9
 * \page RpWorldGetDefaultSectorPipelineplatform RpWorldGetDefaultSectorPipeline (platform-specific)
 *
 * The default world sector object pipeline for D3D9 is shown below.
 *
 * The default world sector object pipeline:
 * \verbatim
     nodeD3D9WorldSectorAllInOne.csl
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
#ifdef RWD3D9_USE_VERTEXSHADER_PIPELINE
            if ((_RwD3D9DeviceCaps.VertexShaderVersion & 0xFFFF) >= 0x0200)
            {
                lpipe = RxLockedPipeAddFragment(lpipe,
                    NULL,
                    RxNodeDefinitionGetD3D9VertexShaderWorldSectorAllInOne(),
                    NULL);
            }
            else
#endif
            {
                lpipe = RxLockedPipeAddFragment(lpipe,
                    NULL,
                    RxNodeDefinitionGetD3D9WorldSectorAllInOne(),
                    NULL);
            }

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
 * \ingroup rpatomicd3d9
 * \page RpAtomicGetDefaultPipelineplatform RpAtomicGetDefaultPipeline (platform-specific)
 *
 * The default atomic object pipeline for D3D9 is shown below.
 *
 * The default atomic object pipeline:
 * \verbatim
     nodeD3D9AtomicAllInOne.csl
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
#ifdef RWD3D9_USE_VERTEXSHADER_PIPELINE
            if ((_RwD3D9DeviceCaps.VertexShaderVersion & 0xFFFF) >= 0x0200)
            {
                lpipe = RxLockedPipeAddFragment(lpipe,
                    NULL,
                    RxNodeDefinitionGetD3D9VertexShaderAtomicAllInOne(),
                    NULL);
            }
            else
#endif
            {
                lpipe = RxLockedPipeAddFragment(lpipe,
                    NULL,
                    RxNodeDefinitionGetD3D9AtomicAllInOne(),
                    NULL);
            }

            lpipe = RxLockedPipeUnlock(lpipe);

            RWASSERT(pipe == (RxPipeline *)lpipe);

            if (NULL != lpipe)
            {
                RXPIPELINEGLOBAL(platformAtomicPipeline) = pipe;
                RpAtomicSetDefaultPipeline(pipe);

                if(_rwD3D9LightsOpen())
                {
                    _rpD3D9VertexShaderCacheOpen();

                    RWRETURN(TRUE);
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

    _rpD3D9DynamicGeometryClose();

    _rpD3D9VertexShaderCachePurge();

    _rwD3D9LightsClose();

    RpAtomicSetDefaultPipeline(NULL);
    if (NULL != RXPIPELINEGLOBAL(platformAtomicPipeline))
    {
        RxPipelineDestroy(RXPIPELINEGLOBAL(platformAtomicPipeline));
        RXPIPELINEGLOBAL(platformAtomicPipeline) = NULL;
    }

    RWRETURNVOID();
}
