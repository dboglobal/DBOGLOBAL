/*
 * Copyright (C) 2000 Criterion Software Limited
 */

/*
 * Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* D3D8 Libraries */
#include <d3d8.h>

/* RenderWare types */
#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "barwtyp.h"
#include "bavector.h"

/* Abstraction of string functionality  -- for unicode support */
#include "rwstring.h"

#include "bamemory.h"
#include "badevice.h"
#include "batextur.h"
#include "baimmedi.h"
#include "baraster.h"

#include "cpuext.h"

#include "drvfns.h"
#include "d3d8device.h"
#include "d3d8raster.h"
#include "d3d8rendst.h"
#include "d3d82drend.h"
#include "d3d8convrt.h"
#include "d3d8texdic.h"
#include "d3d8dxttex.h"

#include "nodeD3D8SubmitNoLight.h"

/* This files header */
#include "d3d8device.h"

#if defined( RWMETRICS )
#include "d3d8metric.h"
#endif /* defined( RWMETRICS ) */

#if !defined( NOASM )
#include "x86matml.h"
#include "x86matml.h"
#include "baprocfp.h"
#include "x86matvc.h"

#if !defined( NOSSEASM )
#include "SSEmatml.h"
#endif

#endif /* !defined( NOASM ) */

/****************************************************************************
 Local defines
 */

#define DEFAULT_SCREEN_WIDTH (640)
#define DEFAULT_SCREEN_HEIGHT (480)

#define UNUSED_D3D8         0

#define LOCK_AT_VSYNCx

#define FLOATASINT(f) (*((const RwInt32 *)&(f)))

#define COLORSCALAR 0.003921568627450980392156862745098f /* 1.0f/ 255.0f */

#if (defined(__GNUC__) && defined(__cplusplus))
#define D3DMatrixInitMacro(_XX, _XY, _XZ, _XW,  \
                           _YX, _YY, _YZ, _YW,  \
                           _ZX, _ZY, _ZZ, _ZW,  \
                           _WX, _WY, _WZ, _WW ) \
  {                                             \
    { { {   (_XX), (_XY), (_XZ), (_XW) },       \
        {   (_YX), (_YY), (_YZ), (_YW) },       \
        {   (_ZX), (_ZY), (_ZZ), (_ZW) },       \
        {   (_WX), (_WY), (_WZ), (_WW) }        \
    } }                                         \
  }
#endif /* (defined(__GNUC__) && defined(__cplusplus)) */


#if (!defined(D3DMatrixInitMacro))
#define D3DMatrixInitMacro(_XX, _XY, _XZ, _XW,  \
                           _YX, _YY, _YZ, _YW,  \
                           _ZX, _ZY, _ZZ, _ZW,  \
                           _WX, _WY, _WZ, _WW ) \
  {                                             \
     (_XX), (_XY), (_XZ), (_XW),                \
     (_YX), (_YY), (_YZ), (_YW),                \
     (_ZX), (_ZY), (_ZZ), (_ZW),                \
     (_WX), (_WY), (_WZ), (_WW)                 \
  }
#endif /* (!defined(D3DMatrixInitMacro)) */


/****************************************************************************
 Local types
 */
typedef struct RxD3D8DynamicVertexBuffer RxD3D8DynamicVertexBuffer;
struct RxD3D8DynamicVertexBuffer
{
    RwUInt32            fvf;
    RwUInt32            size;
    RwUInt32            used;
    LPDIRECT3DVERTEXBUFFER8 vertexBuffer;
    LPDIRECT3DVERTEXBUFFER8 *variableAddress;
    RxD3D8DynamicVertexBuffer *next;
};

typedef struct _rxD3D8VertexShader _rxD3D8VertexShader;
struct _rxD3D8VertexShader
{
    const RwUInt32  *declaration;
    const RwUInt32  *function;
    RwUInt32        *handle;
    RwUInt32        usage;
    _rxD3D8VertexShader *next;
};

typedef struct _rxD3D8PixelShader _rxD3D8PixelShader;
struct _rxD3D8PixelShader
{
    const RwUInt32  *function;
    RwUInt32        *handle;
    _rxD3D8PixelShader *next;
};

typedef struct _rxD3D8DisplayMode _rxD3D8DisplayMode;
struct _rxD3D8DisplayMode
{
    D3DDISPLAYMODE      mode;
    RwInt32             flags;
};

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Globals (across program)
 */

RwRwDeviceGlobals           dgGGlobals; /* Defined here, externed in barwtyp.h */

HWND                        WindowHandle = 0;
RwRGBAReal                  AmbientSaturated = { 0.0f, 0.0f, 0.0f, 1.0f };
D3DPRESENT_PARAMETERS       Present;
D3DCAPS8                    _RwD3D8DeviceCaps;
LPDIRECT3DDEVICE8           _RwD3DDevice = NULL;
LPDIRECT3DSURFACE8          _RwD3D8DepthStencilSurface = NULL;
LPDIRECT3DSURFACE8          _RwD3D8RenderSurface = NULL;
RwInt32                     _RwD3D8ZBufferDepth;
_rwD3D8AdapterInformation   _RwD3D8AdapterInformation;

static LPDIRECT3D8  _RwDirect3DObject = NULL;
static RwUInt32     _RwD3DAdapterType = D3DDEVTYPE_HAL;
static RwBool       NeedToCopyFromBackBuffer = FALSE;
static RwBool       SystemStarted = FALSE;
static RwBool       _RwHasStencilBuffer = FALSE;
static RwInt32      NumDisplayModes = 0;
static RwInt32      _RwD3D8CurrentModeIndex;
static RwUInt32     _RwD3DAdapterIndex = D3DADAPTER_DEFAULT;
static RwUInt32     StencilClearValue = 0;
static _rxD3D8DisplayMode *DisplayModes = NULL; /* Just used during the selection process */

static RwUInt32     MaxMultisamplingLevels = 1;
static RwUInt32     SelectedMultisamplingLevels = 1;

static RwUInt32     FullScreenRefreshRateInHz = 0xffffffff;

static RwBool       EnableMultithreadSafe = FALSE;
static RwBool       EnableSoftwareVertexProcessing = FALSE;

static RwBool       InsideScene = FALSE;

static D3DDISPLAYMODE DesktopDisplayMode;

/*
 * Shaders cache
 */
static RwUInt32     LastVertexShaderUsed = 0xffffffff;
static RwUInt32     LastPixelShaderUsed = 0xffffffff;
static void        *LastVertexBufferUsed = (void *) 0xffffffff;
static RwUInt32     LastVertexBufferStride = 0;
static void        *LastIndexBufferUsed = (void *) 0xffffffff;
static RwUInt32     LastBaseIndex = 0xffffffff;

/*
 * Matrix cache
 */

/* Matrix indices can have a value of 0..511,
 * but normally it will fall into 0..256*/
#define MAX_INDEX_MATRIX    260

static RwBool       LastWorldMatrixUsedIdentity = FALSE;
static D3DMATRIX    *LastMatrixUsed[MAX_INDEX_MATRIX] =
    { NULL, NULL, NULL, NULL }; /* to clear the view and projection matrix */
static RwFreeList   *MatrixFreeList = NULL;


static const D3DMATRIX IdentityMatrix =
    D3DMatrixInitMacro(1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f);

/*
 * Lights cache
 */

typedef struct RxD3D8Light RxD3D8Light;
struct RxD3D8Light
{
    D3DLIGHT8   light;
    RwBool      enabled;
};

static RwInt32      MaxNumLights = 0;
static RxD3D8Light  *LightsCache = NULL;

/*
 * Material cache
 */
static D3DMATERIAL8 LastMaterial;

/*
 * Dynamic Vertex Buffer cache
 */
static RxD3D8DynamicVertexBuffer *DynamicVertexBufferList = NULL;
static RwFreeList                *DynamicVertexBufferFreeList = NULL;

#if defined(RWDEBUG)
static RwUInt32     NumDynamicVertexBuffer = 0;
#endif /* defined(RWDEBUG) */

#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER       4
#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE (256*1024)

static RwUInt32 CurrentDynamicVertexBufferManager = 0;
static RwUInt32 OffSetDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER] = { 0 };
static RwUInt32 SizeDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER] = { 0 };
static LPDIRECT3DVERTEXBUFFER8 DynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER] = { NULL };

/*
 * List of shader to recreate when losing devices
 */
static _rxD3D8VertexShader *VertexShaderList = NULL;
static RwFreeList *VertexShaderFreeList = NULL;

static _rxD3D8PixelShader *PixelShaderList = NULL;
static RwFreeList *PixelShaderFreeList = NULL;

/*
 *  CPU features
 */
RwBool   _rwD3D8CPUSupportsMMX = FALSE;
RwBool   _rwD3D8CPUSupportsSSE = FALSE;
RwBool   _rwD3D8CPUSupportsSSE2 = FALSE;
RwBool   _rwD3D8CPUSupports3DNow = FALSE;

/****************************************************************************
 Globals (prototypes)
 */

static __inline RwBool
D3D8MemEqual(const RwUInt32 * buff0,
             const RwUInt32 * buff1, RwUInt32 sizeBytes)
{
    RWFUNCTION(RWSTRING("D3D8MemEqual"));

    RWASSERT((sizeBytes % 4) == 0);

    sizeBytes /= 4;

    while (sizeBytes)
    {
        if (*buff0 != *buff1)
        {
            RWRETURN(FALSE);
        }

        buff0++;
        buff1++;
        sizeBytes--;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8DynamicVertexBufferListRelease

 Purpose:   Release all video memory Dinamic vertex buffers

*/
static void
D3D8DynamicVertexBufferListRelease(void)
{
    RWFUNCTION(RWSTRING("D3D8DynamicVertexBufferListRelease"));

    if (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        RxD3D8DynamicVertexBuffer *currentVertexBuffer;

        currentVertexBuffer = DynamicVertexBufferList;
        while (currentVertexBuffer)
        {
            RxD3D8DynamicVertexBuffer *next = currentVertexBuffer->next;

            if (currentVertexBuffer->vertexBuffer)
            {
                IDirect3DVertexBuffer8_Release(currentVertexBuffer->vertexBuffer);

                currentVertexBuffer->vertexBuffer = NULL;

                if (currentVertexBuffer->variableAddress)
                {
                    *currentVertexBuffer->variableAddress = NULL;
                }
            }

            currentVertexBuffer = next;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8DynamicVertexBufferListRestore

 Purpose:   Restore all video memory Dinamic vertex buffers

*/
static RwBool
D3D8DynamicVertexBufferListRestore(void)
{
    HRESULT             hr = D3D_OK;

    RWFUNCTION(RWSTRING("D3D8DynamicVertexBufferListRestore"));

#if defined(RWDEBUG)
    {
        RwChar              buffer[256];

        rwsprintf(buffer, "NumDynamicVertexBuffer: %d",
                  NumDynamicVertexBuffer);

        RwDebugSendMessage(rwDEBUGMESSAGE,
                           "D3D8DynamicVertexBufferListRestore",
                           buffer);
    }
#endif

    if (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        RxD3D8DynamicVertexBuffer *currentVertexBuffer;

        currentVertexBuffer = DynamicVertexBufferList;
        while (currentVertexBuffer)
        {
            RxD3D8DynamicVertexBuffer *next = currentVertexBuffer->next;

            if (!currentVertexBuffer->vertexBuffer)
            {
                hr = DXCHECK(IDirect3DDevice8_CreateVertexBuffer
                             (_RwD3DDevice, currentVertexBuffer->size,
                              D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
                              currentVertexBuffer->fvf, D3DPOOL_DEFAULT,
                              &(currentVertexBuffer->vertexBuffer)));

                if (currentVertexBuffer->variableAddress)
                {
                    *currentVertexBuffer->variableAddress =
                        currentVertexBuffer->vertexBuffer;
                }
            }

            if (FAILED(hr))
            {
                RWRETURN(FALSE);
            }

            currentVertexBuffer = next;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8DynamicVertexBufferListDestroy

 Purpose:   Initialize the list of Dinamic vertex buffers

*/
static RwBool
D3D8DynamicVertexBufferListCreate(void)
{
    RWFUNCTION(RWSTRING("D3D8DynamicVertexBufferListCreate"));

    DynamicVertexBufferFreeList = RwFreeListCreate(
                                            sizeof(RxD3D8DynamicVertexBuffer),
                                            42, /* It results in ~1024 Bytes */
                                            4, rwID_DRIVERMODULE |
                                            rwMEMHINTDUR_GLOBAL);

    RWRETURN(DynamicVertexBufferFreeList != NULL);
}

/****************************************************************************
 rwD3D8DynamicVertexBufferListDestroy

 Purpose:   Destroy the list of Dinamic vertex buffers

*/
static void
D3D8DynamicVertexBufferListDestroy(void)
{
    RWFUNCTION(RWSTRING("D3D8DynamicVertexBufferListDestroy"));

    while (DynamicVertexBufferList)
    {
        RxD3D8DynamicVertexBuffer *next = DynamicVertexBufferList->next;

        if (DynamicVertexBufferList->vertexBuffer)
        {
            IDirect3DVertexBuffer8_Release(DynamicVertexBufferList->vertexBuffer);

            DynamicVertexBufferList->vertexBuffer = NULL;

            if (DynamicVertexBufferList->variableAddress)
            {
                *DynamicVertexBufferList->variableAddress = NULL;
            }
        }

        RwFreeListFree(DynamicVertexBufferFreeList, DynamicVertexBufferList);

        DynamicVertexBufferList = next;
    }

    if (DynamicVertexBufferFreeList != NULL)
    {
        RwFreeListDestroy(DynamicVertexBufferFreeList);
        DynamicVertexBufferFreeList = NULL;
    }

#if defined(RWDEBUG)
    NumDynamicVertexBuffer = 0;
#endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}


/****************************************************************************
 D3D8VertexShaderListRelease

 Purpose:   Release all vertex shaders

*/
static void
D3D8VertexShaderListRelease(void)
{
    _rxD3D8VertexShader *currentVertexShader;

    RWFUNCTION(RWSTRING("D3D8VertexShaderListRelease"));

    LastVertexShaderUsed = 0xffffffff;

    currentVertexShader = VertexShaderList;
    while (currentVertexShader)
    {
        _rxD3D8VertexShader *next = currentVertexShader->next;

        if (currentVertexShader->handle != NULL &&
            *(currentVertexShader->handle) != 0)
        {
            DXCHECK(IDirect3DDevice8_DeleteVertexShader(
                                                _RwD3DDevice,
                                               *(currentVertexShader->handle)));

            *(currentVertexShader->handle) = 0;
        }

        currentVertexShader = next;
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D8PixelShaderListRelease

 Purpose:   Release all pixel shaders

*/
static void
D3D8PixelShaderListRelease(void)
{
    _rxD3D8PixelShader *currentPixelShader;

    RWFUNCTION(RWSTRING("D3D8PixelShaderListRelease"));

    currentPixelShader = PixelShaderList;
    while (currentPixelShader)
    {
        _rxD3D8PixelShader *next = currentPixelShader->next;

        if (currentPixelShader->handle != NULL &&
            *(currentPixelShader->handle) != 0)
        {
            DXCHECK(IDirect3DDevice8_DeletePixelShader(
                                                _RwD3DDevice,
                                               *(currentPixelShader->handle)));

            *(currentPixelShader->handle) = 0;
        }

        currentPixelShader = next;
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D8VertexShaderListRestore

 Purpose:   Restore all video memory Dinamic vertex buffers

*/
static RwBool
D3D8VertexShaderListRestore(void)
{
    HRESULT             hr = D3D_OK;
    _rxD3D8VertexShader *currentVertexShader;

    RWFUNCTION(RWSTRING("D3D8VertexShaderListRestore"));

    currentVertexShader = VertexShaderList;
    while (currentVertexShader)
    {
        _rxD3D8VertexShader *next = currentVertexShader->next;

        if (currentVertexShader->handle != NULL &&
            *(currentVertexShader->handle) == 0)
        {
            hr = DXCHECK(IDirect3DDevice8_CreateVertexShader
                         (_RwD3DDevice,
                            (const DWORD *)currentVertexShader->declaration,
                            (const DWORD *)currentVertexShader->function,
                            (DWORD *)currentVertexShader->handle,
                            currentVertexShader->usage));
        }

        if (FAILED(hr))
        {
            RWRETURN(FALSE);
        }

        currentVertexShader = next;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D8PixelShaderListRestore

 Purpose:   Restore all video memory Dinamic vertex buffers

*/
static RwBool
D3D8PixelShaderListRestore(void)
{
    HRESULT             hr = D3D_OK;
    _rxD3D8PixelShader *currentPixelShader;

    RWFUNCTION(RWSTRING("D3D8PixelShaderListRestore"));

    currentPixelShader = PixelShaderList;
    while (currentPixelShader)
    {
        _rxD3D8PixelShader *next = currentPixelShader->next;

        if (currentPixelShader->handle != NULL &&
            *(currentPixelShader->handle) == 0)
        {
            hr = DXCHECK(IDirect3DDevice8_CreatePixelShader
                         (_RwD3DDevice,
                            (const DWORD *)currentPixelShader->function,
                            (DWORD *)currentPixelShader->handle));
        }

        if (FAILED(hr))
        {
            RWRETURN(FALSE);
        }

        currentPixelShader = next;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D8VertexShaderListDestroy

 Purpose:   Destroy the list of Dinamic vertex buffers

*/
static void
D3D8VertexShaderListDestroy(void)
{
    RWFUNCTION(RWSTRING("D3D8VertexShaderListDestroy"));

    while (VertexShaderList)
    {
        _rxD3D8VertexShader *next = VertexShaderList->next;

        if (VertexShaderList->handle != NULL &&
            *(VertexShaderList->handle) != 0)
        {
            DXCHECK(IDirect3DDevice8_DeleteVertexShader(
                                            _RwD3DDevice,
                                           *(VertexShaderList->handle)));

            *(VertexShaderList->handle) = 0;
        }

        RwFreeListFree(VertexShaderFreeList, VertexShaderList);

        VertexShaderList = next;
    }

    if (VertexShaderFreeList != NULL)
    {
        RwFreeListDestroy(VertexShaderFreeList);
        VertexShaderFreeList = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D8PixelShaderListDestroy

 Purpose:   Destroy the list of Dinamic vertex buffers

*/
static void
D3D8PixelShaderListDestroy(void)
{
    RWFUNCTION(RWSTRING("D3D8PixelShaderListDestroy"));

    while (PixelShaderList)
    {
        _rxD3D8PixelShader *next = PixelShaderList->next;

        if (PixelShaderList->handle != NULL &&
            *(PixelShaderList->handle) != 0)
        {
            DXCHECK(IDirect3DDevice8_DeletePixelShader(
                                            _RwD3DDevice,
                                           *(PixelShaderList->handle)));

            *(PixelShaderList->handle) = 0;
        }

        RwFreeListFree(PixelShaderFreeList, PixelShaderList);

        PixelShaderList = next;
    }

    if (PixelShaderFreeList != NULL)
    {
        RwFreeListDestroy(PixelShaderFreeList);
        PixelShaderFreeList = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8DynamicVertexBufferManagerDestroy

 */
static void
D3D8DynamicVertexBufferManagerDestroy(void)
{
    RwUInt32            n;

    RWFUNCTION(RWSTRING("D3D8DynamicVertexBufferManagerDestroy"));

    CurrentDynamicVertexBufferManager = 0;

    for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        OffSetDynamicVertexBufferManager[n] = 0;

        SizeDynamicVertexBufferManager[n] = 0;

        if (DynamicVertexBufferManager[n])
        {
            RwD3D8DynamicVertexBufferDestroy(DynamicVertexBufferManager[n]);

            DynamicVertexBufferManager[n] = NULL;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8DynamicVertexBufferManagerCreate

 */
static RwBool
D3D8DynamicVertexBufferManagerCreate(void)
{
    RwBool      created;
    RwUInt32    n;

    RWFUNCTION(RWSTRING("D3D8DynamicVertexBufferManagerCreate"));

    CurrentDynamicVertexBufferManager = 0;

    if (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
        {
            OffSetDynamicVertexBufferManager[n] = 0;

            SizeDynamicVertexBufferManager[n] = MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE;

            created = RwD3D8DynamicVertexBufferCreate(0,
                                            MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE,
                                            (void **)
                                            &(DynamicVertexBufferManager[n]));

            if (!created)
            {
                RWRETURN(FALSE);
            }
        }
    }
    else
    {
        OffSetDynamicVertexBufferManager[0] = 0;

        SizeDynamicVertexBufferManager[0] = MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE;

        created = RwD3D8DynamicVertexBufferCreate(0,
                                        MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE,
                                        (void **)
                                        &(DynamicVertexBufferManager[0]));
        if (!created)
        {
            RWRETURN(FALSE);
        }

        for (n = 1; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
        {
            OffSetDynamicVertexBufferManager[n] = 0;

            SizeDynamicVertexBufferManager[n] = 0;

            DynamicVertexBufferManager[n] = NULL;
        }
    }

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   'Standard' Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 rwD3D8FindDepth

 On entry   : format

 On exit    : Format depth 16,32 on success else 0 on failure.
 */
static RwInt32
D3D8FindDepth(D3DFORMAT format)
{
    RwInt32             depth;

    RWFUNCTION(RWSTRING("D3D8FindDepth"));

    switch (format)
    {
        case D3DFMT_A8R8G8B8:
        case D3DFMT_X8R8G8B8:
            depth = 32;
            break;

        case D3DFMT_R5G6B5:
        case D3DFMT_A1R5G5B5:
        case D3DFMT_X1R5G5B5:
            depth = 16;
            break;

        default:
            depth = 0;
            break;
    }

    RWRETURN(depth);
}

/****************************************************************************
 rwD3D8FindRWFormat

 On entry   : format

 On exit    : Format depth 16,32 on success else 0 on failure.
 */
static RwInt32
rwD3D8FindRWFormat(D3DFORMAT format)
{
    RwInt32 rwformat;

    RWFUNCTION(RWSTRING("rwD3D8FindRWFormat"));

    switch (format)
    {
        case D3DFMT_A8R8G8B8:
            rwformat = rwRASTERFORMAT8888;
            break;

        case D3DFMT_X8R8G8B8:
            rwformat = rwRASTERFORMAT888;
            break;

        case D3DFMT_R5G6B5:
            rwformat = rwRASTERFORMAT565;
            break;

        case D3DFMT_A1R5G5B5:
            rwformat = rwRASTERFORMAT1555;
            break;

        case D3DFMT_X1R5G5B5:
            rwformat = rwRASTERFORMAT555;
            break;

        default:
            rwformat = 0;
            break;
    }

    RWRETURN(rwformat);
}

/****************************************************************************
 rwD3D8ClearCacheShaders

 On entry   : none
 */
static void
D3D8ClearCacheShaders(void)
{
    RWFUNCTION(RWSTRING("D3D8ClearCacheShaders"));

    LastVertexShaderUsed = 0xffffffff;
    LastPixelShaderUsed = 0xffffffff;
    LastVertexBufferUsed = (void *) 0xffffffff;
    LastVertexBufferStride = 0;
    LastIndexBufferUsed = (void *) 0xffffffff;
    LastBaseIndex = 0xffffffff;

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ClearCacheMatrix

 On entry   : none

 On exit    : The Matrix pointers are cleared.
 */
static void
D3D8ClearCacheMatrix(void)
{
    RWFUNCTION(RWSTRING("D3D8ClearCacheMatrix"));

    LastWorldMatrixUsedIdentity = FALSE;

    if (MatrixFreeList != NULL)
    {
        RwUInt32 n;

        for (n = 0; n < MAX_INDEX_MATRIX; n++)
        {
            if (LastMatrixUsed[n] != NULL)
            {
                RwFreeListFree(MatrixFreeList, LastMatrixUsed[n]);
                LastMatrixUsed[n] = NULL;
            }
        }

        RwFreeListPurge(MatrixFreeList);
    }
    else
    {
        MatrixFreeList = RwFreeListCreate(sizeof(D3DMATRIX), 15, 16,
            rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

        memset(LastMatrixUsed, 0, sizeof(D3DMATRIX *) * MAX_INDEX_MATRIX);
    }

    /* World matrix always created */
    LastMatrixUsed[D3DTS_WORLD] = (D3DMATRIX *) RwFreeListAlloc(MatrixFreeList,
        rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

    memcpy(LastMatrixUsed[D3DTS_WORLD], &IdentityMatrix, sizeof(D3DMATRIX));

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ClearCacheLights

 On entry   : none

 On exit    : The Matrix pointers are cleared.
 */
static void
D3D8ClearCacheLights(void)
{
    RWFUNCTION(RWSTRING("D3D8ClearCacheLights"));

    MaxNumLights = 0;

    if (LightsCache != NULL)
    {
        RwFree(LightsCache);
        LightsCache = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ClearCacheMaterials

 On entry   : none

 */
static void
D3D8ClearCacheMaterials(void)
{
    RWFUNCTION(RWSTRING("D3D8ClearCacheMaterials"));

    memset(&LastMaterial, 0, sizeof(D3DMATERIAL8));

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8ClearCache

 On entry   : none

 */
static void
D3D8ClearCache(void)
{
    RWFUNCTION(RWSTRING("D3D8ClearCache"));

    /* Reset the shaders cache */
    D3D8ClearCacheShaders();

    /* Reset the matrix cache */
    D3D8ClearCacheMatrix();

    /* Reset the lights cache */
    D3D8ClearCacheLights();

    /* Reset the material cache */
    D3D8ClearCacheMaterials();

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8RestoreCacheLights

 On entry   : none

 On exit    : The Matrix pointers are cleared.
 */
static RwBool
D3D8RestoreCacheLights(void)
{
    HRESULT             hr = D3D_OK;
    RwInt32             i;

    RWFUNCTION(RWSTRING("D3D8RestoreCacheLights"));

#if defined(RWDEBUG)
    {
        RwChar              buffer[256];

        rwsprintf(buffer, "NumLights: %d", MaxNumLights);

        RwDebugSendMessage(rwDEBUGMESSAGE, "D3D8RestoreCacheLights",
                           buffer);
    }
#endif

    for (i = 0; i < MaxNumLights; i++)
    {
        hr = DXCHECK(IDirect3DDevice8_SetLight
                     (_RwD3DDevice, i, &(LightsCache[i].light)));

        hr = DXCHECK(IDirect3DDevice8_LightEnable
                     (_RwD3DDevice, i, LightsCache[i].enabled));
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 D3D8CalculateMaxMultisamplingLevels

 On entry   :
 On exit    :
 */
static void
D3D8CalculateMaxMultisamplingLevels(void)
{
    RwUInt32    levels;

    RWFUNCTION(RWSTRING("D3D8CalculateMaxMultisamplingLevels"));

    MaxMultisamplingLevels      = 1;
    SelectedMultisamplingLevels = 1;

    levels = D3DMULTISAMPLE_2_SAMPLES;
    do
    {
        const BOOL Windowed = ((_RwD3D8AdapterInformation.flags &
                                rwVIDEOMODEEXCLUSIVE) == 0);
        const HRESULT status =
            IDirect3D8_CheckDeviceMultiSampleType(_RwDirect3DObject,
                                                  _RwD3DAdapterIndex,
                                                  _RwD3DAdapterType,
                                                  _RwD3D8AdapterInformation.mode.Format,
                                                  Windowed,
                                                  (D3DMULTISAMPLE_TYPE)levels ) ;
        if( SUCCEEDED(status) )
        {
            MaxMultisamplingLevels = levels;
        }

        levels++;
    }
    while(levels <= D3DMULTISAMPLE_16_SAMPLES);

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8DeviceSystemOpen

 On entry   :
 On exit    :
 */
static RwBool
D3D8DeviceSystemOpen(void *out __RWUNUSED__,
                     void *inOut, RwInt32 in __RWUNUSED__)
{
    RwUInt32 numAdapters;

    RWFUNCTION(RWSTRING("D3D8DeviceSystemOpen"));

    /* We're interested in the window handle here, so store it away */
    WindowHandle = (HWND) (((RwEngineOpenParams *) inOut)->displayID);

    /* Create a device */
    _RwDirect3DObject = Direct3DCreate8(D3D_SDK_VERSION);
    if (!_RwDirect3DObject)
    {
        RWRETURN(FALSE);
    }

    /* Name */
    rwstrcpy(_RwD3D8AdapterInformation.name, RWSTRING("d3d"));

    /* Get video card capabilities */
    numAdapters = IDirect3D8_GetAdapterCount(_RwDirect3DObject);

    _RwD3DAdapterIndex = 0;
    _RwD3DAdapterType = D3DDEVTYPE_HAL;

    while ( _RwD3DAdapterIndex < numAdapters)
    {
        if (SUCCEEDED(IDirect3D8_GetDeviceCaps(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               &_RwD3D8DeviceCaps)))
        {
            break;
        }

        _RwD3DAdapterIndex++;
    }

    if (_RwD3DAdapterIndex >= numAdapters)
    {
        #if defined(RWDEBUG)
        RwDebugSendMessage(rwDEBUGERROR, "D3D8DeviceSystemOpen",
                           "Hardware acceleration not supported in this adapter."
                           "Using reference rasterizer.");

        _RwD3DAdapterIndex = 0;
        _RwD3DAdapterType = D3DDEVTYPE_REF;

        IDirect3D8_GetDeviceCaps(_RwDirect3DObject,
                                 _RwD3DAdapterIndex,
                                 _RwD3DAdapterType,
                                 &_RwD3D8DeviceCaps);

        #else
        IDirect3D8_Release(_RwDirect3DObject);

        _RwDirect3DObject = NULL;

        RWRETURN(FALSE);
        #endif
    }

    /* Mode count */
    _RwD3D8AdapterInformation.modeCount =
        IDirect3D8_GetAdapterModeCount(_RwDirect3DObject,
                                       _RwD3DAdapterIndex);

    /* Get the first mode as the default */
    IDirect3D8_GetAdapterDisplayMode(_RwDirect3DObject,
                                     _RwD3DAdapterIndex,
                                     &_RwD3D8AdapterInformation.mode);

    _RwD3D8AdapterInformation.displayDepth =
        D3D8FindDepth(_RwD3D8AdapterInformation.mode.Format);

    _RwD3D8AdapterInformation.flags = 0;

    DesktopDisplayMode = _RwD3D8AdapterInformation.mode;

    /* Choose a default mode, we'll take the first */
    _RwD3D8CurrentModeIndex = 0;

#if (0 && defined(RWDEBUG))
    {
        RwInt32             i;

        for (i = 0; i < _RwD3D8AdapterInformation.modeCount; i++)
        {
            D3DDISPLAYMODE      mode;
            RwChar              buffer[256];

            IDirect3D8_EnumAdapterModes(_RwDirect3DObject,
                                        _RwD3DAdapterIndex, i, &mode);

            rwsprintf(buffer,
                      "Width: %d Height: %d RefreshRate: %d Format: %d",
                      mode.Width, mode.Height, mode.RefreshRate,
                      mode.Format);

            RwDebugSendMessage(rwDEBUGMESSAGE, "D3D8DeviceSystemOpen",
                               buffer);
        }
    }
#endif /* (0 && defined(RWDEBUG)) */

    /* Calculate max multisampling levels */
    D3D8CalculateMaxMultisamplingLevels();

    /* Detect CPU Info */
    _rwD3D8CPUSupportsMMX = _rwCPUHaveMMX();
    _rwD3D8CPUSupportsSSE = _rwCPUHaveSSE();
    _rwD3D8CPUSupportsSSE2 = _rwCPUHaveSSE2();
    _rwD3D8CPUSupports3DNow = _rwCPUHave3DNow();

#if defined(RWDEBUG)
    if (RWSRCGLOBAL(debugFunction))
    {
        RwChar buffer[256];
        RwInt32 len;
        MEMORYSTATUS memstats;

        /* OS info */
        len = rwsprintf(buffer, "\nOS Info: ");
        _rwCPUGetOSName(buffer + len);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE,
            "------------------------------------------------------------------------------");

        GlobalMemoryStatus(&memstats);
        len = (memstats.dwTotalPhys / (1024 * 1024));
        len = ((len + 1) & 0xfffffffe); /* Round up to a multiple of 2 */
        rwsprintf(buffer, "   RAM:    %d MB", len);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        /* CPU info */
        len = rwsprintf(buffer, "\nCPU Info: ");
        _rwCPUGetCPUName(buffer + len);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE,
                  "------------------------------------------------------------------------------");

        rwsprintf(buffer, "   MMX:    %s", (_rwD3D8CPUSupportsMMX ? "Available" : "Not available"));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   SSE:    %s", (_rwD3D8CPUSupportsSSE ? "Available" : "Not available"));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   SSE2:   %s", (_rwD3D8CPUSupportsSSE2 ? "Available" : "Not available"));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   3DNow!: %s\n", (_rwD3D8CPUSupports3DNow ? "Available" : "Not available"));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);
    }
#endif /* defined(RWDEBUG) */

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8DeviceSystemClose

 On entry   :
 On exit    :
 */
static RwBool
D3D8DeviceSystemClose(void *pOut __RWUNUSED__,
                      void *pInOut __RWUNUSED__,
                      RwInt32 nIn __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("D3D8DeviceSystemClose"));

    if (DisplayModes != NULL)
    {
        RwFree(DisplayModes);
        DisplayModes = NULL;

        NumDisplayModes = 0;
    }

    RWASSERT(_RwDirect3DObject);
    if (_RwDirect3DObject)
    {
        IDirect3D8_Release(_RwDirect3DObject);

        _RwDirect3DObject = NULL;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8DeviceSystemFinalizeStart

 On entry   : out (unused)
              inOut (unused)
              in (unused)

 On exit    : TRUE if successful, FALSE otherwise
 */
static RwBool
D3D8DeviceSystemFinalizeStart(void *out __RWUNUSED__,
                              void *inOut __RWUNUSED__,
                              RwInt32 in __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("D3D8DeviceSystemFinalizeStart"));

#if !defined( NOASM )

    /* initialise the FP with single precision mode
     * saving the control register
     */
    _rwProcessorInitialize();

#if !defined( NOSSEASM )
    /* Enable SSE flush-to-zero */
    if (_rwD3D8CPUSupportsSSE)
    {
        RwUInt32 temp;

        _asm
        {
            stmxcsr     temp
            mov         eax, temp
            or          eax, 0x8000
            mov         temp, eax
            ldmxcsr     temp
        }
    }
#endif /* !defined( NOSSEASM ) */

    /* set the matrix multiplication routine to the new assembler code
     * this method allows future overloading */
    _rwMatrixSetMultFn(_rwIntelx86AsmMatrixMultiply);

    /* set the matrix-vector products to the new assembler code */
    _rwVectorSetMultFn(NULL,
                       _rwIntelx86AsmV3dTransformPoints,
                       NULL,
                       _rwIntelx86AsmV3dTransformVectors);

#endif /* !defined( NOASM ) */

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D8SetPresentParameters

 On entry   :
 */
static void
D3D8SetPresentParameters(const D3DDISPLAYMODE *mode,
                         RwUInt32 flags,
                         D3DFORMAT displayFormat)
{
    RWFUNCTION(RWSTRING("D3D8SetPresentParameters"));

    if (flags & rwVIDEOMODEEXCLUSIVE)
    {
        /* fullscreen */
        Present.Windowed = FALSE;

        Present.BackBufferFormat = mode->Format;

        Present.BackBufferCount = 1;

        /* As fast as we can */
        Present.FullScreen_RefreshRateInHz = min(
                                    mode->RefreshRate,
                                    FullScreenRefreshRateInHz);

#ifdef LOCK_AT_VSYNC
        Present.FullScreen_PresentationInterval =
            D3DPRESENT_INTERVAL_ONE;
#else
        Present.FullScreen_PresentationInterval =
            D3DPRESENT_INTERVAL_IMMEDIATE;
#endif

        Present.BackBufferWidth = mode->Width;
        Present.BackBufferHeight = mode->Height;

        Present.SwapEffect = D3DSWAPEFFECT_FLIP;
    }
    else
    {
        RECT rect;

        /* windowed */
        Present.Windowed = TRUE;

        Present.BackBufferFormat = displayFormat;

        Present.BackBufferCount = 1;

        Present.FullScreen_RefreshRateInHz = 0;

        /* Check window size */
        GetWindowRect(WindowHandle, &rect);

        if ((rect.right - rect.left) == 0)
        {
            Present.BackBufferWidth = 1;
        }
        else
        {
            if (SystemStarted)
            {
                Present.BackBufferWidth = (rect.right - rect.left);
            }
            else
            {
                Present.BackBufferWidth = 0;
            }
        }

        if ((rect.bottom - rect.top) == 0)
        {
            Present.BackBufferHeight = 1;
        }
        else
        {
            if (SystemStarted)
            {
                Present.BackBufferHeight = (rect.bottom - rect.top);
            }
            else
            {
                Present.BackBufferHeight = 0;
            }
        }

        Present.SwapEffect = D3DSWAPEFFECT_COPY;
    }

    /* TRUE so depth/stencil buffer are managed for us */
    Present.EnableAutoDepthStencil = TRUE;

    /*
     * Find the Z-Buffer depth
     */

    /* Best perfomance results when using same bit depth than back buffer */
    _RwD3D8ZBufferDepth = D3D8FindDepth(Present.BackBufferFormat);

    if (_RwD3D8ZBufferDepth == 32)
    {
        if (D3D_OK ==
            IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                         _RwD3DAdapterIndex, _RwD3DAdapterType,
                                         displayFormat,
                                         D3DUSAGE_DEPTHSTENCIL,
                                         D3DRTYPE_SURFACE, D3DFMT_D24S8)
            && D3D_OK ==
            IDirect3D8_CheckDepthStencilMatch(_RwDirect3DObject,
                                              _RwD3DAdapterIndex,
                                              _RwD3DAdapterType,
                                              displayFormat,
                                              mode->Format, D3DFMT_D24S8))
        {
            Present.AutoDepthStencilFormat = D3DFMT_D24S8;

            _RwHasStencilBuffer = TRUE;
        }
        else
        {
            if (D3D_OK ==
                IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                             _RwD3DAdapterIndex,
                                             _RwD3DAdapterType,
                                             displayFormat,
                                             D3DUSAGE_DEPTHSTENCIL,
                                             D3DRTYPE_SURFACE,
                                             D3DFMT_D24X4S4)
                && D3D_OK ==
                IDirect3D8_CheckDepthStencilMatch(_RwDirect3DObject,
                                                  _RwD3DAdapterIndex,
                                                  _RwD3DAdapterType,
                                                  displayFormat,
                                                  mode->Format,
                                                  D3DFMT_D24X4S4))
            {
                Present.AutoDepthStencilFormat = D3DFMT_D24X4S4;

                _RwHasStencilBuffer = TRUE;
            }
            else
            {
                /* !!!! we don't have stencil buffer */
                if (D3D_OK ==
                    IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                                 _RwD3DAdapterIndex,
                                                 _RwD3DAdapterType,
                                                 displayFormat,
                                                 D3DUSAGE_DEPTHSTENCIL,
                                                 D3DRTYPE_SURFACE,
                                                 D3DFMT_D32)
                    && D3D_OK ==
                    IDirect3D8_CheckDepthStencilMatch(_RwDirect3DObject,
                                                      _RwD3DAdapterIndex,
                                                      _RwD3DAdapterType,
                                                      displayFormat,
                                                      mode->Format,
                                                      D3DFMT_D32))
                {
                    Present.AutoDepthStencilFormat = D3DFMT_D32;

                    _RwHasStencilBuffer = FALSE;
                }
                else
                {
                    if (D3D_OK ==
                        IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                                     _RwD3DAdapterIndex,
                                                     _RwD3DAdapterType,
                                                     displayFormat,
                                                     D3DUSAGE_DEPTHSTENCIL,
                                                     D3DRTYPE_SURFACE,
                                                     D3DFMT_D24X8)
                        && D3D_OK ==
                        IDirect3D8_CheckDepthStencilMatch
                        (_RwDirect3DObject, _RwD3DAdapterIndex,
                         _RwD3DAdapterType, displayFormat,
                         mode->Format,
                         D3DFMT_D24X8))
                    {
                        Present.AutoDepthStencilFormat = D3DFMT_D24X8;

                        _RwHasStencilBuffer = FALSE;
                    }
                    else
                    {
                        _RwD3D8ZBufferDepth = 16;
                    }
                }
            }
        }
    }

    if (_RwD3D8ZBufferDepth == 16)
    {
        if (D3D_OK ==
            IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                         _RwD3DAdapterIndex,
                                         _RwD3DAdapterType,
                                         displayFormat,
                                         D3DUSAGE_DEPTHSTENCIL,
                                         D3DRTYPE_SURFACE,
                                         D3DFMT_D15S1)
            && D3D_OK ==
            IDirect3D8_CheckDepthStencilMatch(_RwDirect3DObject,
                                              _RwD3DAdapterIndex,
                                              _RwD3DAdapterType,
                                              displayFormat,
                                              mode->Format,
                                              D3DFMT_D15S1))
        {
            Present.AutoDepthStencilFormat = D3DFMT_D15S1;

            _RwHasStencilBuffer = TRUE;
        }
        else
        {
            Present.AutoDepthStencilFormat = D3DFMT_D16;

            _RwHasStencilBuffer = FALSE;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D8DeviceSystemStart

 On entry   :
 On exit    :
 */
static RwBool
D3D8DeviceSystemStart(void *out __RWUNUSED__,
                      void *inOut __RWUNUSED__, RwInt32 in __RWUNUSED__)
{
    D3DDISPLAYMODE      adapterDisplayMode;
    HRESULT             hr;
    RwUInt32            behaviorFlags;

    RWFUNCTION(RWSTRING("D3D8DeviceSystemStart"));

    IDirect3D8_GetAdapterDisplayMode(_RwDirect3DObject,
                                     _RwD3DAdapterIndex,
                                     &adapterDisplayMode);

    memset(&Present, 0, sizeof(D3DPRESENT_PARAMETERS));

    D3D8SetPresentParameters(&_RwD3D8AdapterInformation.mode,
                             _RwD3D8AdapterInformation.flags,
                             adapterDisplayMode.Format);

    Present.hDeviceWindow = WindowHandle;

    if (SelectedMultisamplingLevels > 1)
    {
        Present.MultiSampleType = ( (D3DMULTISAMPLE_TYPE)
                                    SelectedMultisamplingLevels );

        Present.SwapEffect = D3DSWAPEFFECT_DISCARD;
    }
    else
    {
        Present.MultiSampleType = D3DMULTISAMPLE_NONE;
    }

    Present.Flags = 0;

    IDirect3D8_GetDeviceCaps(_RwDirect3DObject, _RwD3DAdapterIndex,
                             _RwD3DAdapterType, &_RwD3D8DeviceCaps);

#if defined(RWDEBUG)
    if (RWSRCGLOBAL(debugFunction))
    {
        D3DADAPTER_IDENTIFIER8 adapterInfo;
        RwChar                 *buffer;

        IDirect3D8_GetAdapterIdentifier(_RwDirect3DObject,
                                        _RwD3DAdapterIndex,
                                        D3DENUM_NO_WHQL_LEVEL,
                                        &adapterInfo);

        buffer = (RwChar *)RwMalloc((2 * MAX_DEVICE_IDENTIFIER_STRING) *
                                    sizeof(RwChar),
                                    rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL);

        rwsprintf(buffer, "\nDevice Selected Info: %s",
                  adapterInfo.Description);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE,
                                    buffer);

        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE,
                  "------------------------------------------------------------------------------");

        rwsprintf(buffer, "   Windowed: %u", Present.Windowed);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Resolution: %ux%ux%u %uHz",
                  _RwD3D8AdapterInformation.mode.Width,
                  _RwD3D8AdapterInformation.mode.Height,
                  D3D8FindDepth(_RwD3D8AdapterInformation.mode.
                                Format),
                  Present.FullScreen_RefreshRateInHz);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   ZBuffer format: %u (%u bits)",
                  Present.AutoDepthStencilFormat,
                  _RwD3D8ZBufferDepth);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Hardware T&L: %u",
                  ((_RwD3D8DeviceCaps.
                    DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) !=
                   0));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Pure Device: %u",
                  ((_RwD3D8DeviceCaps.
                    DevCaps & D3DDEVCAPS_PUREDEVICE) != 0));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DeviceType: %u",
                  _RwD3D8DeviceCaps.DeviceType);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   AdapterOrdinal: %u",
                  _RwD3D8DeviceCaps.AdapterOrdinal);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Driver version: %u.%u.%u.%u",
                  HIWORD(adapterInfo.DriverVersion.HighPart),
                  LOWORD(adapterInfo.DriverVersion.HighPart),
                  HIWORD(adapterInfo.DriverVersion.LowPart),
                  LOWORD(adapterInfo.DriverVersion.LowPart));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Caps: %X", _RwD3D8DeviceCaps.Caps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Caps2: %X", _RwD3D8DeviceCaps.Caps2);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Caps3: %X", _RwD3D8DeviceCaps.Caps3);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   PresentationIntervals: %X",
                  _RwD3D8DeviceCaps.PresentationIntervals);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   CursorCaps: %X",
                  _RwD3D8DeviceCaps.CursorCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DevCaps: %X",
                  _RwD3D8DeviceCaps.DevCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   PrimitiveMiscCaps: %X",
                  _RwD3D8DeviceCaps.PrimitiveMiscCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   RasterCaps: %X",
                  _RwD3D8DeviceCaps.RasterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   ZCmpCaps: %X",
                  _RwD3D8DeviceCaps.ZCmpCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   SrcBlendCaps: %X",
                  _RwD3D8DeviceCaps.SrcBlendCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DestBlendCaps: %X",
                  _RwD3D8DeviceCaps.DestBlendCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   AlphaCmpCaps: %X",
                  _RwD3D8DeviceCaps.AlphaCmpCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   ShadeCaps: %X",
                  _RwD3D8DeviceCaps.ShadeCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   TextureCaps: %X",
                  _RwD3D8DeviceCaps.TextureCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   TextureFilterCaps: %X",
                  _RwD3D8DeviceCaps.TextureFilterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   CubeTextureFilterCaps: %X",
                  _RwD3D8DeviceCaps.CubeTextureFilterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VolumeTextureFilterCaps: %X",
                  _RwD3D8DeviceCaps.VolumeTextureFilterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   TextureAddressCaps: %X",
                  _RwD3D8DeviceCaps.TextureAddressCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VolumeTextureAddressCaps: %X",
                  _RwD3D8DeviceCaps.VolumeTextureAddressCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureWidth: %u",
                  _RwD3D8DeviceCaps.MaxTextureWidth);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureHeight: %u",
                  _RwD3D8DeviceCaps.MaxTextureHeight);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVolumeExtent: %u",
                  _RwD3D8DeviceCaps.MaxVolumeExtent);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureRepeat: %u",
                  _RwD3D8DeviceCaps.MaxTextureRepeat);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureAspectRatio: %u",
                  _RwD3D8DeviceCaps.MaxTextureAspectRatio);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxAnisotropy: %u",
                  _RwD3D8DeviceCaps.MaxAnisotropy);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexW: %f",
                  _RwD3D8DeviceCaps.MaxVertexW);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   GuardBandLeft: %f",
                  _RwD3D8DeviceCaps.GuardBandLeft);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   GuardBandTop: %f",
                  _RwD3D8DeviceCaps.GuardBandTop);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   GuardBandRight: %f",
                  _RwD3D8DeviceCaps.GuardBandRight);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   GuardBandBottom: %f",
                  _RwD3D8DeviceCaps.GuardBandBottom);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   ExtentsAdjust: %f",
                  _RwD3D8DeviceCaps.ExtentsAdjust);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   StencilCaps: %X",
                  _RwD3D8DeviceCaps.StencilCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   FVFCaps: %X",
                  _RwD3D8DeviceCaps.FVFCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   TextureOpCaps: %X",
                  _RwD3D8DeviceCaps.TextureOpCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureBlendStages: %u",
                  _RwD3D8DeviceCaps.MaxTextureBlendStages);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxSimultaneousTextures: %u",
                  _RwD3D8DeviceCaps.MaxSimultaneousTextures);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VertexProcessingCaps: %X",
                  _RwD3D8DeviceCaps.VertexProcessingCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxActiveLights: %u",
                  _RwD3D8DeviceCaps.MaxActiveLights);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxUserClipPlanes: %u",
                  _RwD3D8DeviceCaps.MaxUserClipPlanes);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexBlendMatrices: %u",
                  _RwD3D8DeviceCaps.MaxVertexBlendMatrices);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexBlendMatrixIndex: %u",
                  _RwD3D8DeviceCaps.MaxVertexBlendMatrixIndex);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxPointSize: %f",
                  _RwD3D8DeviceCaps.MaxPointSize);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxPrimitiveCount: %u",
                  _RwD3D8DeviceCaps.MaxPrimitiveCount);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexIndex: %u",
                  _RwD3D8DeviceCaps.MaxVertexIndex);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxStreams: %u",
                  _RwD3D8DeviceCaps.MaxStreams);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxStreamStride: %u",
                  _RwD3D8DeviceCaps.MaxStreamStride);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VertexShaderVersion: %X",
                  _RwD3D8DeviceCaps.VertexShaderVersion);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexShaderConst: %u",
                  _RwD3D8DeviceCaps.MaxVertexShaderConst);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   PixelShaderVersion: %X",
                  _RwD3D8DeviceCaps.PixelShaderVersion);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxPixelShaderValue: %f",
                  _RwD3D8DeviceCaps.MaxPixelShaderValue);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, "\n");

        RwFree(buffer);
    }
#endif /* defined(RWDEBUG) */

    behaviorFlags = 0;

    if (EnableMultithreadSafe)
    {
        behaviorFlags |= D3DCREATE_MULTITHREADED;
    }

    if (EnableSoftwareVertexProcessing)
    {
        behaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

        _RwD3D8DeviceCaps.DevCaps &= ~(D3DDEVCAPS_HWTRANSFORMANDLIGHT);
    }
    else
    {
        if (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        {
            if (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
            {
                behaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING |
                                D3DCREATE_PUREDEVICE;
            }
            else
            {
                behaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
            }
        }
        else
        {
            behaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
    }

    hr = DXCHECK(IDirect3D8_CreateDevice
                    (_RwDirect3DObject, _RwD3DAdapterIndex,
                    _RwD3DAdapterType, WindowHandle,
                    behaviorFlags, &Present,
                    &_RwD3DDevice));

    if (FAILED(hr))
    {
        RWRETURN(FALSE);
    }

#if defined(RWDEBUG)
    if (EnableMultithreadSafe)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "D3D8DeviceSystemStart",
                        "\n   Direct3D Device created as multithread safe.\n"
                        "   This makes Direct3D take its global critical section more frequently, which may degrade performance.\n");
    }

    if (EnableSoftwareVertexProcessing)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "D3D8DeviceSystemStart",
                        "\n   Direct3D Device created to use software vertex processing.\n"
                        "   This permits to debug vertex shades but may degrade performance seriously.\n");
    }
#endif

    /* Get window dimensions */
    if (WindowHandle)
    {
        RECT                rect;

        GetClientRect(WindowHandle, &rect);

        _RwD3D8AdapterInformation.mode.Width = rect.right;
        _RwD3D8AdapterInformation.mode.Height = rect.bottom;
    }

    /*
     * Calling D3DDevice_GetRenderTarget and D3DDevice_GetDepthStencilSurface
     * increase the internal reference count so they need to be Released.
     */

    /* Get the render surface */
    DXCHECK(IDirect3DDevice8_GetRenderTarget
            (_RwD3DDevice, &_RwD3D8RenderSurface));
    IDirect3DSurface8_Release(_RwD3D8RenderSurface);

    /* Get the depth/stencil surface */
    DXCHECK(IDirect3DDevice8_GetDepthStencilSurface
            (_RwD3DDevice, &_RwD3D8DepthStencilSurface));
    IDirect3DSurface8_Release(_RwD3D8DepthStencilSurface);

    /* Reset the cache */
    D3D8ClearCache();

    /*
     * Open some RenderWare stuff
     */

    _rwD3D8RasterOpen();

    _rwD3D8Im2DRenderOpen();

    _rwD3D8RenderStateOpen();

    D3D8DynamicVertexBufferListCreate();

    D3D8DynamicVertexBufferManagerCreate();

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8DeviceSystemStop

 On entry   :
 On exit    :
 */
static RwBool
D3D8DeviceSystemStop(void *out __RWUNUSED__,
                     void *inOut __RWUNUSED__, RwInt32 in __RWUNUSED__)
{
    RwUInt32 n;

    RWFUNCTION(RWSTRING("D3D8DeviceSystemStop"));

    RWASSERT(_RwD3DDevice);

    SystemStarted = FALSE;

    if (DisplayModes != NULL)
    {
        RwFree(DisplayModes);
        DisplayModes = NULL;

        NumDisplayModes = 0;
    }

#if !defined( NOASM )

    /* Restore the saved floating point control register */
    _rwProcessorRelease();

    /* Disable SSE flush-to-zero */
#if !defined( NOSSEASM )
    if (_rwD3D8CPUSupportsSSE)
    {
        RwUInt32 temp;

        _asm
        {
            stmxcsr     temp
            mov         eax, temp
            and         eax, not 0x8000
            mov         temp, eax
            ldmxcsr     temp
        }
    }
#endif /*!defined( NOSSEASM )*/

#endif /* !defined( NOASM ) */

    /* Release the matrix cache */
    for (n = 0; n < MAX_INDEX_MATRIX; n++)
    {
        if (LastMatrixUsed[n] != NULL)
        {
            RwFreeListFree(MatrixFreeList, LastMatrixUsed[n]);
            LastMatrixUsed[n] = NULL;
        }
    }

    RwFreeListDestroy(MatrixFreeList);
    MatrixFreeList = NULL;

    /* Release the lights cache */
    MaxNumLights = 0;
    if (LightsCache != NULL)
    {
        RwFree(LightsCache);
        LightsCache = NULL;
    }

    /* Release active resources */
    for (n = 0; n < MAX_TEXTURE_STAGES; n++)
    {
        DXCHECK(IDirect3DDevice8_SetTexture(_RwD3DDevice, n, NULL));
    }

    DXCHECK(IDirect3DDevice8_SetStreamSource(_RwD3DDevice, 0, NULL, 0));
    DXCHECK(IDirect3DDevice8_SetIndices(_RwD3DDevice, NULL, 0));

    /* Close any remaining surfaces */
    _rwD3D8RasterClose();

    D3D8DynamicVertexBufferManagerDestroy();

    _rwD3D8Im2DRenderClose();

    D3D8VertexShaderListDestroy();

    D3D8PixelShaderListDestroy();

    D3D8DynamicVertexBufferListDestroy();

    IDirect3DDevice8_Release(_RwD3DDevice);
    _RwD3DDevice = NULL;

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8DeviceReleaseVideoMemory

 On entry   :
 On exit    :
 */
static RwBool
D3D8DeviceReleaseVideoMemory(void)
{
    RwUInt32            i;

    RWFUNCTION(RWSTRING("D3D8DeviceReleaseVideoMemory"));

#if defined(RWDEBUG)
    RwDebugSendMessage(rwDEBUGMESSAGE, "D3D8DeviceReleaseVideoMemory",
                       "Releasing video memory");
#endif

    D3D8ClearCacheShaders();

    D3D8ClearCacheMatrix();

    D3D8ClearCacheMaterials();

    for (i = 0; i < MAX_TEXTURE_STAGES; i++)
    {
        DXCHECK(IDirect3DDevice8_SetTexture(_RwD3DDevice, i, NULL));
    }

    DXCHECK(IDirect3DDevice8_SetStreamSource(_RwD3DDevice, 0, NULL, 0));
    DXCHECK(IDirect3DDevice8_SetIndices(_RwD3DDevice, NULL, 0));

    _rwD3D8Im2DRenderClose();

    _rwD3D8Im3DRenderClose();

    D3D8VertexShaderListRelease();

    D3D8PixelShaderListRelease();

    D3D8DynamicVertexBufferManagerDestroy();

    D3D8DynamicVertexBufferListRelease();

    _rxD3D8VideoMemoryRasterListRelease();

    RwFreeListPurgeAllFreeLists();

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8DeviceRestoreVideoMemory

 On entry   :
 On exit    :
 */
static RwBool
D3D8DeviceRestoreVideoMemory(void)
{
    RwBool              rv;

    RWFUNCTION(RWSTRING("D3D8DeviceRestoreVideoMemory"));

#if defined(RWDEBUG)
    RwDebugSendMessage(rwDEBUGMESSAGE, "D3D8DeviceRestoreVideoMemory",
                       "Restoring video memory");
#endif

    /* Get the render surface */
    DXCHECK(IDirect3DDevice8_GetRenderTarget
            (_RwD3DDevice, &_RwD3D8RenderSurface));
    IDirect3DSurface8_Release(_RwD3D8RenderSurface);

    /* Get the depth/stencil surface */
    DXCHECK(IDirect3DDevice8_GetDepthStencilSurface
            (_RwD3DDevice, &_RwD3D8DepthStencilSurface));
    IDirect3DSurface8_Release(_RwD3D8DepthStencilSurface);

    rv = _rxD3D8VideoMemoryRasterListRestore();

    rv = rv && D3D8DynamicVertexBufferListRestore();

    rv = rv && D3D8VertexShaderListRestore();

    rv = rv && D3D8PixelShaderListRestore();

    if (rv)
    {
        _rwD3D8RenderStateReset();
    }

    rv = rv && D3D8RestoreCacheLights();

    rv = rv && _rwD3D8Im2DRenderOpen();

    rv = rv && _rwD3D8Im3DRenderOpen();

    rv = rv && D3D8DynamicVertexBufferManagerCreate();

    RWRETURN(rv);
}

/****************************************************************************
 rwD3D8NullStandard

 On entry   : pOut
            : pInOut
            : nIn
 On exit    : FALSE
 */
static RwBool
D3D8NullStandard(void *pOut __RWUNUSED__,
                 void *pInOut __RWUNUSED__, RwInt32 nIn __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("D3D8NullStandard"));

    RWRETURN(FALSE);
}

/****************************************************************************
 rwD3D8DeviceSystemStandards

 On entry   :
 On exit    :
 */
static RwBool
D3D8DeviceSystemStandards(void *out, void *inOut __RWUNUSED__,
                          RwInt32 numStandardsFunctions)
{
    RwInt32             i;
    RwInt32             numDriverFunctions;
    RwStandardFunc     *standardFunctions;
    RwStandard          rwD3D8Standards[] = {
        /* Camera ops */
        {rwSTANDARDCAMERABEGINUPDATE, _rwD3D8CameraBeginUpdate},
        {rwSTANDARDCAMERAENDUPDATE, _rwD3D8CameraEndUpdate},
        {rwSTANDARDCAMERACLEAR, _rwD3D8CameraClear},

        /* Raster/Pixel operations */
        {rwSTANDARDRASTERSHOWRASTER, _rwD3D8RasterShowRaster},
        {rwSTANDARDRGBTOPIXEL, _rwD3D8RGBToPixel},
        {rwSTANDARDPIXELTORGB, _rwD3D8PixelToRGB},
        {rwSTANDARDRASTERSETIMAGE, _rwD3D8RasterSetFromImage},
        {rwSTANDARDIMAGEGETRASTER, _rwD3D8ImageGetFromRaster},

        /* Raster creation and destruction */
        {rwSTANDARDRASTERDESTROY, _rwD3D8RasterDestroy},
        {rwSTANDARDRASTERCREATE, _rwD3D8RasterCreate},

        /* Finding about a raster type */
        {rwSTANDARDIMAGEFINDRASTERFORMAT, _rwD3D8ImageFindRasterFormat},

        /* Texture operations */
        {rwSTANDARDTEXTURESETRASTER, _rwD3D8TextureSetRaster},

        /* Locking and releasing */
        {rwSTANDARDRASTERLOCK, _rwD3D8RasterLock},
        {rwSTANDARDRASTERUNLOCK, _rwD3D8RasterUnlock},
        {rwSTANDARDRASTERLOCKPALETTE, _rwD3D8RasterLockPalette},
        {rwSTANDARDRASTERUNLOCKPALETTE, _rwD3D8RasterUnlockPalette},

        /* Raster operations */
        {rwSTANDARDRASTERCLEAR, _rwD3D8RasterClear},
        {rwSTANDARDRASTERCLEARRECT, _rwD3D8RasterClearRect},

        /* !! */
        {rwSTANDARDRASTERRENDER, _rwD3D8RasterRender},
        {rwSTANDARDRASTERRENDERSCALED, _rwD3D8RasterRenderScaled},
        {rwSTANDARDRASTERRENDERFAST, _rwD3D8RasterRenderFast},

        /* Setting the context */
        {rwSTANDARDSETRASTERCONTEXT, _rwD3D8SetRasterContext},

        /* Creating sub rasters */
        {rwSTANDARDRASTERSUBRASTER, _rwD3D8RasterSubRaster},

        /* Hint for rendering order */
        /*{rwSTANDARDHINTRENDERF2B,         _rwD3D8HintRenderFront2Back}, */

        /* Native texture serialization */
        {rwSTANDARDNATIVETEXTUREGETSIZE, _rwD3D8NativeTextureGetSize},
        {rwSTANDARDNATIVETEXTUREWRITE, _rwD3D8NativeTextureWrite},
        {rwSTANDARDNATIVETEXTUREREAD, _rwD3D8NativeTextureRead},

        /* Raster Mip Levels */
        {rwSTANDARDRASTERGETMIPLEVELS, _rwD3D8RasterGetMipLevels}
    };

    RWFUNCTION(RWSTRING("D3D8DeviceSystemStandards"));

    standardFunctions = (RwStandardFunc *) out;
    numDriverFunctions = sizeof(rwD3D8Standards) / sizeof(RwStandard);

    /* Clear out all of the standards initially */
    for (i = 0; i < numStandardsFunctions; i++)
    {
        standardFunctions[i] = D3D8NullStandard;
    }

    /* Fill in all of the standards */
    while (numDriverFunctions--)
    {
        if ((rwD3D8Standards->nStandard < numStandardsFunctions) &&
            (rwD3D8Standards->nStandard >= 0))
        {
            standardFunctions[rwD3D8Standards[numDriverFunctions].
                              nStandard] =
                rwD3D8Standards[numDriverFunctions].fpStandard;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwCreateDisplayModesList

 On entry   :
 On exit    :
 */
static void
D3D8CreateDisplayModesList(void)
{
    RwInt32             i, n;

    RWFUNCTION(RWSTRING("D3D8CreateDisplayModesList"));

    if (DisplayModes)
    {
        DisplayModes = ( (_rxD3D8DisplayMode *)
                         RwRealloc(DisplayModes,
                                   (1 +
                                    _RwD3D8AdapterInformation.modeCount) *
                                   sizeof(_rxD3D8DisplayMode),
                                   rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL |
                                   rwMEMHINTFLAG_RESIZABLE) );
    }
    else
    {
        DisplayModes =( (_rxD3D8DisplayMode *)
                        RwMalloc((1 +
                                  _RwD3D8AdapterInformation.modeCount) *
                                 sizeof(_rxD3D8DisplayMode),
                                 rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL |
                                 rwMEMHINTFLAG_RESIZABLE) );
    }

    IDirect3D8_GetAdapterDisplayMode(_RwDirect3DObject,
                                     _RwD3DAdapterIndex,
                                     &DisplayModes[0].mode);

    if (D3D8FindDepth(DisplayModes[0].mode.Format) != 0 &&
        (_RwD3D8DeviceCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) != 0)
    {
        DisplayModes[0].flags = 0;

        NumDisplayModes = 1;
    }
    else
    {
        NumDisplayModes = 0;
    }

    for (i = 0; i < _RwD3D8AdapterInformation.modeCount; i++)
    {
        IDirect3D8_EnumAdapterModes(_RwDirect3DObject,
                                    _RwD3DAdapterIndex, i,
                                    &DisplayModes[NumDisplayModes].
                                    mode);

        /* Find a previous similar mode */
        for (n = 1; n < NumDisplayModes; n++)
        {
            if (DisplayModes[NumDisplayModes].mode.Width ==
                DisplayModes[n].mode.Width
                && DisplayModes[NumDisplayModes].mode.Height ==
                DisplayModes[n].mode.Height
                && DisplayModes[NumDisplayModes].mode.Format ==
                DisplayModes[n].mode.Format)
            {
                break;
            }
        }

        if (n >= NumDisplayModes)
        {
            DisplayModes[NumDisplayModes].flags = rwVIDEOMODEEXCLUSIVE;

            NumDisplayModes++;
        }
        else
        {
            if (DisplayModes[n].mode.RefreshRate <
                DisplayModes[NumDisplayModes].mode.RefreshRate)
            {
                DisplayModes[n].mode.RefreshRate =
                    DisplayModes[NumDisplayModes].mode.RefreshRate;
            }
        }
    }

    if (NumDisplayModes < (1 + _RwD3D8AdapterInformation.modeCount))
    {
        DisplayModes = ( (_rxD3D8DisplayMode *)
                         RwRealloc(DisplayModes,
                                   NumDisplayModes *
                                   sizeof(_rxD3D8DisplayMode),
                                   rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL |
                                   rwMEMHINTFLAG_RESIZABLE) );
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwDeviceRegisterPlugin

 On entry   :
 On exit    : TRUE on success
 */
RwBool
_rwDeviceRegisterPlugin(void)
{
    RWFUNCTION(RWSTRING("_rwDeviceRegisterPlugin"));

    /*
     * Gives the device a change to register it's plugns.
     */

    RWRETURN(_rwD3D8RasterPluginAttach());
}

/****************************************************************************
 rwD3D8System

 On entry   : Request
            : Data out
            : Data in/out
            : Number in
 On exit    : TRUE on success else FALSE.
 */
static RwBool
D3D8System(RwInt32 request, void *out, void *inOut, RwInt32 in)
{
    RWFUNCTION(RWSTRING("D3D8System"));

    switch (request)
    {
        case rwDEVICESYSTEMUSEMODE:
            if (!SystemStarted && (in >= 0) && (in < NumDisplayModes))
            {
                _RwD3D8CurrentModeIndex = in;

                _RwD3D8AdapterInformation.mode = DisplayModes[in].mode;

                _RwD3D8AdapterInformation.flags = DisplayModes[in].flags;

                _RwD3D8AdapterInformation.displayDepth =
                                    D3D8FindDepth(DisplayModes[in].mode.Format);

                /* Calculate max multisampling levels */
                D3D8CalculateMaxMultisamplingLevels();

                RWRETURN(TRUE);
            }
            else
            {
                RWRETURN(FALSE);
            }

        case rwDEVICESYSTEMGETNUMMODES:
            if (!DisplayModes)
            {
                D3D8CreateDisplayModesList();
            }

            *((RwInt32 *) out) = NumDisplayModes;
            RWRETURN(TRUE);

        case rwDEVICESYSTEMGETMODEINFO:
            if (!DisplayModes)
            {
                D3D8CreateDisplayModesList();
            }

            if ((in >= 0) && (in < NumDisplayModes))
            {
                RwVideoMode *videoMode = (RwVideoMode *) out;

                videoMode->width = DisplayModes[in].mode.Width;
                videoMode->height = DisplayModes[in].mode.Height;
                videoMode->depth = D3D8FindDepth(DisplayModes[in].mode.Format);
                videoMode->flags = ((RwVideoModeFlag)
                                    DisplayModes[in].flags );
                videoMode->refRate = DisplayModes[in].mode.RefreshRate;
                videoMode->format =
                              rwD3D8FindRWFormat(DisplayModes[in].mode.Format);

                RWRETURN(TRUE);
            }
            else
            {
                RWRETURN(FALSE);
            }

        case rwDEVICESYSTEMGETMODE:
            *((RwInt32 *) out) = _RwD3D8CurrentModeIndex;
            RWRETURN(TRUE);

        case rwDEVICESYSTEMFOCUS:
            if (_RwD3DDevice != NULL)
            {
                if (!Present.Windowed)
                {
                    if (in)
                    {
                        ShowWindow(WindowHandle, SW_RESTORE);
                    }
                    else
                    {
                        ShowWindow(WindowHandle, SW_HIDE);
                    }
                }
                RWRETURN(TRUE);
            }
            RWRETURN(FALSE);

            /*** Register in the library block ***/

        case rwDEVICESYSTEMREGISTER:
            {
                RwDevice           *D3D8DriverDevice =
                    _rwDeviceGetHandle();
                RwDevice           *deviceOut = (RwDevice *) out;
                RwMemoryFunctions  *memoryFunctions =
                    (RwMemoryFunctions *) inOut;

                *deviceOut = *D3D8DriverDevice;
                dgGGlobals.memFuncs = memoryFunctions;

                RWRETURN(TRUE);
            }

            /******************* Opening/Closing **********************/

        case rwDEVICESYSTEMOPEN:
            RWRETURN(D3D8DeviceSystemOpen(out, inOut, in));

        case rwDEVICESYSTEMCLOSE:
            RWRETURN(D3D8DeviceSystemClose(out, inOut, in));

            /********* Selecting a rendering sub system ********/

        case rwDEVICESYSTEMGETNUMSUBSYSTEMS:
            {
                RwInt32            *numSubSystems = (RwInt32 *) out;

                *numSubSystems =
                    IDirect3D8_GetAdapterCount(_RwDirect3DObject);

                RWRETURN(TRUE);
            }

        case rwDEVICESYSTEMGETSUBSYSTEMINFO:
            {
                RwInt32             subSystemIndex = (RwInt32) in;
                RwSubSystemInfo    *subSystemInfo =
                    (RwSubSystemInfo *) out;
                D3DADAPTER_IDENTIFIER8 adapterInfo;

                IDirect3D8_GetAdapterIdentifier(_RwDirect3DObject,
                                                subSystemIndex,
                                                D3DENUM_NO_WHQL_LEVEL,
                                                &adapterInfo);

                rwstrcpy(subSystemInfo->name, adapterInfo.Description);
                RWRETURN(TRUE);
            }

        case rwDEVICESYSTEMGETCURRENTSUBSYSTEM:
            {
                RwInt32            *currentSubSystem = (RwInt32 *) out;

                *currentSubSystem = _RwD3DAdapterIndex;

                RWRETURN(TRUE);
            }

        case rwDEVICESYSTEMSETSUBSYSTEM:
            {
                _RwD3DAdapterIndex = in;

                /* Mode count */
                _RwD3D8AdapterInformation.modeCount =
                    IDirect3D8_GetAdapterModeCount(_RwDirect3DObject,
                                                   _RwD3DAdapterIndex);

                /* Get the current mode as the default */
                IDirect3D8_GetAdapterDisplayMode(_RwDirect3DObject,
                                                 _RwD3DAdapterIndex,
                                                 &_RwD3D8AdapterInformation.
                                                 mode);

                /* Recreate list of modes */
                D3D8CreateDisplayModesList();

                /* Calculate max multisampling levels */
                D3D8CalculateMaxMultisamplingLevels();

                RWRETURN(TRUE);
            }

            /************** Starting stopping ******************/

            /* Start means that this bit of hardware has been selected for
             * rendering - so get it into a state ready for rendering */

        case rwDEVICESYSTEMSTART:
            {
                SystemStarted = D3D8DeviceSystemStart(out, inOut, in);

                RWRETURN(SystemStarted);
            }

        case rwDEVICESYSTEMSTOP:
            RWRETURN(D3D8DeviceSystemStop(out, inOut, in));

            /************* standard device functions ************************/

        case rwDEVICESYSTEMSTANDARDS:
            RWRETURN(D3D8DeviceSystemStandards(out, inOut, in));

        case rwDEVICESYSTEMINITPIPELINE:
            break;

        case rwDEVICESYSTEMGETTEXMEMSIZE:
            break;

        case rwDEVICESYSTEMFINALIZESTART:
            {
                RWRETURN(D3D8DeviceSystemFinalizeStart(out, inOut, in));
            }
            break;

        case rwDEVICESYSTEMINITIATESTOP:
            break;

        case rwDEVICESYSTEMGETMAXTEXTURESIZE:
            {
                RwInt32     *maxTextureSize = (RwInt32 *) out;


                /* return the maximum dimension of a texture */
                if ( _RwD3D8DeviceCaps.MaxTextureWidth >= _RwD3D8DeviceCaps.MaxTextureHeight )
                {
                    *maxTextureSize = _RwD3D8DeviceCaps.MaxTextureWidth;
                }
                else
                {
                    *maxTextureSize = _RwD3D8DeviceCaps.MaxTextureHeight;
                }

                RWRETURN( TRUE );
            }
            break;

        case rwDEVICESYSTEMRXPIPELINEREQUESTPIPE:
            break;

#if defined( RWMETRICS )
        case rwDEVICESYSTEMGETMETRICBLOCK:
            {
                /* return the platform specific metrics block */
                *((RwD3D8Metrics **) (out)) = _rwD3D8MetricsGet();

                RWRETURN(TRUE);
            }
            break;
#endif /* defined( RWMETRICS ) */
        case rwDEVICESYSTEMGETID:
            {
                *(RwUInt16*)out = rwDEVICE_D3D8;
                RWRETURN(TRUE);
            }
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwD3D8CheckError

 On entry   : DX error value, printf debug info if different of D3D_OK

 On exit    : the same error value
 */
#if defined(RWDEBUG)
HRESULT
_rwD3D8CheckError(HRESULT hr,
                  const RwChar * function,
                  const char *file, RwUInt32 line)
{
    RWFUNCTION(RWSTRING("_rwD3D8CheckError"));

    if (FAILED(hr))
    {
        switch (hr)
        {
            case D3DERR_CONFLICTINGRENDERSTATE:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The currently set render states cannot be used together.");
                break;
            case D3DERR_CONFLICTINGTEXTUREFILTER:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The current texture filters cannot be used together.");
                break;
            case D3DERR_CONFLICTINGTEXTUREPALETTE:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The current textures cannot be used simultaneously. This generally occurs when a multitexture device requires that all palletized textures simultaneously enabled also share the same palette.");
                break;
            case D3DERR_DEVICELOST:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The device is lost and cannot be restored at the current time, so rendering is not possible.");
                break;
            case D3DERR_DEVICENOTRESET:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The device cannot be reset.");
                break;
            case D3DERR_DRIVERINTERNALERROR:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "Internal driver error.");
                break;
            case D3DERR_INVALIDCALL:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The method call is invalid. For example, a method's parameter may have an invalid value.");
                break;
            case D3DERR_INVALIDDEVICE:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The requested device type is not valid.");
                break;
            case D3DERR_MOREDATA:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "There is more data available than the specified buffer size can hold.");
                break;
            case D3DERR_NOTAVAILABLE:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "This device does not support the queried technique.");
                break;
            case D3DERR_NOTFOUND:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The requested item was not found.");
                break;
            case D3DERR_OUTOFVIDEOMEMORY:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "Direct3D does not have enough display memory to perform the operation.");
                break;
            case D3DERR_TOOMANYOPERATIONS:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The application is requesting more texture-filtering operations than the device supports.");
                break;
            case D3DERR_UNSUPPORTEDALPHAARG:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The device does not support a specified texture-blending argument for the alpha channel.");
                break;
            case D3DERR_UNSUPPORTEDALPHAOPERATION:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The device does not support a specified texture-blending operation for the alpha channel.");
                break;
            case D3DERR_UNSUPPORTEDCOLORARG:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The device does not support a specified texture-blending argument for color values.");
                break;
            case D3DERR_UNSUPPORTEDCOLOROPERATION:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The device does not support a specified texture-blending operation for color values.");
                break;
            case D3DERR_UNSUPPORTEDFACTORVALUE:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The device does not support the specified texture factor value.");
                break;
            case D3DERR_UNSUPPORTEDTEXTUREFILTER:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The device does not support the specified texture filter.");
                break;
            case D3DERR_WRONGTEXTUREFORMAT:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The pixel format of the texture surface is not valid.");
                break;
            case E_FAIL:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "An undetermined error occurred inside the Direct3D subsystem.");
                break;
            case E_INVALIDARG:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "An invalid parameter was passed to the returning function");
                break;
            case E_OUTOFMEMORY:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "Direct3D could not allocate sufficient memory to complete the call.");
                break;

            default:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "Unknow error.");
        }
    }

    RWRETURN(hr);
}
#endif

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                            System handler

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwD3D8CameraClear

 On entry   : Camera
            : RwRGBA colour to clear to
            : Buffer mask to clear
 On exit    : TRUE on success
 */

RwBool
_rwD3D8CameraClear(void *camera, void *color, RwInt32 clearFlags)
{
    RwRaster           *raster;
    RwRaster           *topRaster;
    _rwD3D8RasterExt   *topRasterExt;
    RwRaster           *zRaster;
    RwRaster            *zTopRaster;
    _rwD3D8RasterExt    *zrasExt;
    D3DCOLOR            packedColor = 0;
    HRESULT             hr;
    RECT                rect;
    static const DWORD  clearFlagsConvTable[8] = {
        0,                      /* Nothing */
        D3DCLEAR_TARGET,        /* Clear Frame buffer only */
        D3DCLEAR_ZBUFFER,       /* Clear Z buffer only */
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, /* Clear Frame & Z buffer */
        D3DCLEAR_STENCIL,       /* Clear Stencil buffer */
        D3DCLEAR_TARGET | D3DCLEAR_STENCIL, /* Clear Frame & Stencil buffer */
        D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, /* Clear Z Buffer & Stencil */
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, /* Clear Frame, Z & stencil */
    };

    RWFUNCTION(RWSTRING("_rwD3D8CameraClear"));

    /* The clear color */
    if (rwCAMERACLEARIMAGE & clearFlags)
    {
        RwRGBA             *col = (RwRGBA *) color;

        RWASSERT(NULL != color);

        /* Pack the clear color */
        packedColor = (D3DCOLOR) (((col->alpha) << 24) |
                                  ((col->red) << 16) |
                                  ((col->green) << 8) | (col->blue));
    }

    raster = RwCameraGetRaster((RwCamera *) camera);

    /*
     * Got to get the top level raster as this is the only one with a
     * texture surface
     */
    topRaster = RwRasterGetParent(raster);

    topRasterExt = RASTEREXTFROMRASTER(topRaster);

    if (topRasterExt->swapChain)
    {
        D3DVIEWPORT8        viewport;

        GetClientRect(topRasterExt->window, &rect);

        if (!rect.right || !rect.bottom)
        {
            RWRETURN(FALSE);
        }
        else
        {
            if (rect.right != topRaster->originalWidth ||
                rect.bottom != topRaster->originalHeight)
            {
                if (raster == topRaster)
                {
                    RwD3D8CameraAttachWindow(camera, topRasterExt->window);

                    /* Update raster pointers */
                    raster = RwCameraGetRaster((RwCamera *)camera);
                    topRaster = RwRasterGetParent(raster);
                    topRasterExt = RASTEREXTFROMRASTER(topRaster);
                }

                zRaster = RwCameraGetZRaster((RwCamera *) camera);
            }
            else
            {
                zRaster = RwCameraGetZRaster((RwCamera *) camera);

                if (zRaster && zRaster == RwRasterGetParent(zRaster))
                {
                    if (rect.right != zRaster->width ||
                        rect.bottom != zRaster->height)
                    {
                        (void)RwCameraSetZRaster((RwCamera *) camera,
                                               NULL);

                        RwRasterDestroy(zRaster);

                        zRaster =
                            RwRasterCreate(rect.right, rect.bottom, 0,
                                           rwRASTERTYPEZBUFFER);

                        if (zRaster)
                        {
                            (void)RwCameraSetZRaster((RwCamera *) camera,
                                                   zRaster);
                        }
                    }
                }
            }

            /* Set the render & depth/stencil surface */
            if (zRaster)
            {
                zTopRaster = RwRasterGetParent(zRaster);

                zrasExt = RASTEREXTFROMRASTER(zTopRaster);

                DXCHECK(IDirect3DDevice8_SetRenderTarget
                        (_RwD3DDevice, (LPSURFACE) topRasterExt->texture,
                         (LPSURFACE) zrasExt->texture));
            }
            else
            {
                DXCHECK(IDirect3DDevice8_SetRenderTarget
                        (_RwD3DDevice, (LPSURFACE) topRasterExt->texture,
                         NULL));
            }

            /* Initialize the viewport to clear */
            viewport.X = 0;
            viewport.Y = 0;
            viewport.Width = raster->width;
            viewport.Height = raster->height;
            viewport.MinZ = MINZBUFFERVALUE;
            viewport.MaxZ = MAXZBUFFERVALUE;

            /* Set the viewport to clear */
            DXCHECK(IDirect3DDevice8_SetViewport
                    (_RwD3DDevice, &viewport));

            /* Clear */
            DXCHECK(IDirect3DDevice8_Clear
                    (_RwD3DDevice, 0, NULL,
                     clearFlagsConvTable[clearFlags], packedColor,
                     MAXZBUFFERVALUE, StencilClearValue));

            /* Set the render & depth/stencil surface */
            DXCHECK(IDirect3DDevice8_SetRenderTarget
                    (_RwD3DDevice, _RwD3D8RenderSurface,
                     _RwD3D8DepthStencilSurface));
        }
    }
    else
    {
        /* Check if the size of the main window has changed */
        GetClientRect(WindowHandle, &rect);

        if (!rect.right || !rect.bottom || IsIconic(WindowHandle))
        {
            RWRETURN(FALSE);
        }
        else if ((RwUInt32) rect.right !=
                 _RwD3D8AdapterInformation.mode.Width
                 || (RwUInt32) rect.bottom !=
                 _RwD3D8AdapterInformation.mode.Height)
        {
            D3D8DeviceReleaseVideoMemory();

            hr = IDirect3DDevice8_TestCooperativeLevel(_RwD3DDevice);

            if (SUCCEEDED(hr) && Present.Windowed)
            {
                Present.BackBufferWidth = rect.right;
                Present.BackBufferHeight = rect.bottom;
            }

            hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

            if (SUCCEEDED(hr))
            {
                if (!D3D8DeviceRestoreVideoMemory())
                {
                    D3D8DeviceReleaseVideoMemory();

                    hr = E_FAIL;
                }
            }

            if (FAILED(hr))
            {
                if(Present.Windowed)
                {
                    #if defined(RWDEBUG)
                    RwChar              buffer[256];

                    rwsprintf(buffer, "Not enough memory to resize the window to %dx%d."
                                       "Switching back to %dx%d",
                                       rect.right, rect.bottom,
                                       _RwD3D8AdapterInformation.mode.Width,
                                       _RwD3D8AdapterInformation.mode.Height);

                    RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D8CameraClear",
                                       buffer);
                    #endif

                    Present.BackBufferWidth = _RwD3D8AdapterInformation.mode.Width;
                    Present.BackBufferHeight = _RwD3D8AdapterInformation.mode.Height;

                    hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

                    if (FAILED(hr))
                    {
                        RWRETURN(FALSE);
                    }

                    /* Change window size */
                    rect.right = rect.left + _RwD3D8AdapterInformation.mode.Width;
                    rect.bottom = rect.top + _RwD3D8AdapterInformation.mode.Height;

                    if (GetWindowLong(WindowHandle, GWL_STYLE) & WS_MAXIMIZE)
                    {
                        SetWindowLong(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) & ~WS_MAXIMIZE);
                    }

                    AdjustWindowRectEx(&rect,
                                    GetWindowLong(WindowHandle, GWL_STYLE),
                                    (GetMenu(WindowHandle)!=NULL),
                                    GetWindowLong(WindowHandle, GWL_EXSTYLE));

                    SetWindowPos(WindowHandle, 0,
                                rect.left, rect.bottom,
                                rect.right-rect.left,
                                rect.bottom-rect.top,
                                SWP_NOMOVE | SWP_NOZORDER);

                    D3D8DeviceRestoreVideoMemory();
                }
                else
                {
                    RWRETURN(FALSE);
                }
            }
            else
            {
                _RwD3D8AdapterInformation.mode.Width = rect.right;
                _RwD3D8AdapterInformation.mode.Height = rect.bottom;
            }
        }

        /* Check raster type */
        if (rwRASTERTYPECAMERATEXTURE == topRaster->cType)
        {
            LPSURFACE           surfaceLevel;
            D3DSURFACE_DESC     d3d8Desc;
            D3DVIEWPORT8        viewport;
            RwUInt32            d3d8ClearFlags;

            if (topRasterExt->cube)
            {
                IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)topRasterExt->texture,
                                                        (D3DCUBEMAP_FACES)topRasterExt->face,
                                                        0,
                                                        &surfaceLevel);
            }
            else
            {
                IDirect3DTexture8_GetSurfaceLevel(topRasterExt->texture, 0,
                                                  &surfaceLevel);
            }

            IDirect3DSurface8_GetDesc(surfaceLevel, &d3d8Desc);

            /* Set the render & depth/stencil surface */
            if (d3d8Desc.Usage == D3DUSAGE_RENDERTARGET)
            {
                zRaster = RwCameraGetZRaster((RwCamera *) camera);
                if (zRaster)
                {
                    /*
                    * Got to get the top level raster as this is the only one with a
                    * texture surface
                    */
                    zTopRaster = RwRasterGetParent(zRaster);

                    zrasExt = RASTEREXTFROMRASTER(zTopRaster);

                    hr = DXCHECK(IDirect3DDevice8_SetRenderTarget
                                 (_RwD3DDevice, surfaceLevel,
                                  (LPSURFACE) zrasExt->texture));
                }
                else
                {
                    hr = DXCHECK(IDirect3DDevice8_SetRenderTarget
                                 (_RwD3DDevice, surfaceLevel, NULL));
                }
            }
            else
            {
                hr = E_FAIL;
            }

            /* Initialize the viewport to clear */
            viewport.X = raster->nOffsetX;
            viewport.Y = raster->nOffsetY;
            viewport.Width = raster->width;
            viewport.Height = raster->height;
            viewport.MinZ = MINZBUFFERVALUE;
            viewport.MaxZ = MAXZBUFFERVALUE;

            /* Set the viewport to clear */
            DXCHECK(IDirect3DDevice8_SetViewport
                    (_RwD3DDevice, &viewport));

            /* check clear flags */
            d3d8ClearFlags = clearFlagsConvTable[clearFlags];

            if (d3d8ClearFlags & (D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL))
            {
                if (RwCameraGetZRaster((RwCamera *) camera) == NULL)
                {
                    d3d8ClearFlags &= ~(D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL);
                }
            }

            /* Clear */
            DXCHECK(IDirect3DDevice8_Clear
                    (_RwD3DDevice, 0, NULL,
                     d3d8ClearFlags, packedColor,
                     MAXZBUFFERVALUE, StencilClearValue));

            /* If the set render target fails, we need to copy from the back buffer */
            if (FAILED(hr))
            {
                RECT                rectSrc;

                rectSrc.left = raster->nOffsetX;
                rectSrc.top = raster->nOffsetY;
                rectSrc.right = rectSrc.left + raster->width;
                rectSrc.bottom = rectSrc.top + raster->height;

                hr = DXCHECK(IDirect3DDevice8_CopyRects
                             (_RwD3DDevice, _RwD3D8RenderSurface,
                              &rectSrc, 1, surfaceLevel, NULL));
            }

            /* Set the render & depth/stencil surface */
            DXCHECK(IDirect3DDevice8_SetRenderTarget
                    (_RwD3DDevice, _RwD3D8RenderSurface,
                     _RwD3D8DepthStencilSurface));

            /* Release the cleared surface */
            IDirect3DSurface8_Release(surfaceLevel);
        }
        else
        {
            D3DVIEWPORT8        viewport;
            RwUInt32            d3d8ClearFlags = 0;

            /* Initialize the viewport to clear */
            viewport.X = raster->nOffsetX;
            viewport.Y = raster->nOffsetY;
            viewport.Width = raster->width;
            viewport.Height = raster->height;
            viewport.MinZ = MINZBUFFERVALUE;
            viewport.MaxZ = MAXZBUFFERVALUE;

            /* Set the viewport to clear */
            DXCHECK(IDirect3DDevice8_SetViewport
                    (_RwD3DDevice, &viewport));

            /* Clear */
            if (rwCAMERACLEARIMAGE & clearFlags)
            {
                d3d8ClearFlags |= D3DCLEAR_TARGET;
            }

            if (rwCAMERACLEARZ & clearFlags)
            {
                d3d8ClearFlags |= D3DCLEAR_ZBUFFER;

                if (_RwHasStencilBuffer)
                {
                    d3d8ClearFlags |= D3DCLEAR_STENCIL;
                }
            }
            else if (rwCAMERACLEARSTENCIL & clearFlags)
            {
                if (_RwHasStencilBuffer)
                {
                    d3d8ClearFlags |= D3DCLEAR_STENCIL;
                }
            }

            DXCHECK(IDirect3DDevice8_Clear
                    (_RwD3DDevice, 0, NULL, d3d8ClearFlags, packedColor,
                     MAXZBUFFERVALUE, StencilClearValue));
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8BeginScene

 On exit    : TRUE on success
 */
RwBool
_rwD3D8BeginScene(void)
{
    RWFUNCTION(RWSTRING("_rwD3D8BeginScene"));

    if (!InsideScene)
    {
        HRESULT hr;

        hr = IDirect3DDevice8_TestCooperativeLevel(_RwD3DDevice);

        if (hr == D3DERR_DEVICENOTRESET)
        {
    #if defined(RWDEBUG)
            RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D8BeginScene",
                               "DEVICE LOST");
    #endif

            D3D8DeviceReleaseVideoMemory();

            hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

            if (hr == D3DERR_INVALIDCALL)
            {
                /* Check to see if the desktop has changed its depth */
                if (Present.Windowed)
                {
                    D3DDISPLAYMODE      adapterDisplayMode;

                    IDirect3D8_GetAdapterDisplayMode(_RwDirect3DObject,
                                                     _RwD3DAdapterIndex,
                                                     &adapterDisplayMode);

                    Present.BackBufferFormat =
                    _RwD3D8AdapterInformation.mode.Format =
                                                        adapterDisplayMode.Format;

                    _RwD3D8AdapterInformation.displayDepth =
                                D3D8FindDepth(_RwD3D8AdapterInformation.mode.Format);

                    hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));
                }
                else
                {
                    /* Ops!!! really bad situation */
                }
            }

            if (SUCCEEDED(hr))
            {
                D3D8DeviceRestoreVideoMemory();
            }
        }

        if (SUCCEEDED(hr))
        {
            hr = DXCHECK(IDirect3DDevice8_BeginScene(_RwD3DDevice));

            if (SUCCEEDED(hr))
            {
                InsideScene = TRUE;
            }
        }
    }

    RWRETURN(InsideScene);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8EngineGetMaxMultiSamplingLevels returns the maximum available
 * levels of multisampling for the current subsystem and video mode.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * \return Returns maximum levels of multisampling for the current
 * subsystem and video mode. A value of \e 1 means that the current
 * subsystem and video mode doesn't support multisampling.
 *
 * \see RwD3D8EngineSetMultiSamplingLevels
 * \see RwEngineOpen
 * \see RwEngineSetSubSystem
 * \see RwEngineSetVideoMode
 * \see RwEngineStart
 *
 */
RwUInt32
RwD3D8EngineGetMaxMultiSamplingLevels(void)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8EngineGetMaxMultiSamplingLevels"));

    RWRETURN(MaxMultisamplingLevels);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8EngineSetMultiSamplingLevels sets the number of multisampling levels
 * for the current subsystem and video mode.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * If the current subsystem and video mode doesn't support the specified number of
 * multisampling levels, RWD3D8 will choose the highest number of supported multisampling
 * levels \e below that which was specified.
 *
 * Note that changing the current subsystem or the current video mode will reset
 * the number of multisampling levels to 1.
 *
 * \param numLevels Number of multisampling levels.
 * A value of \e 1 means that multisampling is disabled.
 *
 * \see RwD3D8EngineGetMaxMultiSamplingLevels
 * \see RwEngineOpen
 * \see RwEngineSetSubSystem
 * \see RwEngineSetVideoMode
 * \see RwEngineStart
 *
 */
void
RwD3D8EngineSetMultiSamplingLevels(RwUInt32 numLevels)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8EngineSetMultiSamplingLevels"));

    if (numLevels < D3DMULTISAMPLE_2_SAMPLES)
    {
        SelectedMultisamplingLevels = 1;
    }
    else
    {
        SelectedMultisamplingLevels = min(numLevels, MaxMultisamplingLevels);
    }

    while(SelectedMultisamplingLevels >= D3DMULTISAMPLE_2_SAMPLES)
    {
        const BOOL Windowed = ((_RwD3D8AdapterInformation.flags &
                                rwVIDEOMODEEXCLUSIVE) == 0);

        const HRESULT status =
            IDirect3D8_CheckDeviceMultiSampleType(_RwDirect3DObject,
                                                  _RwD3DAdapterIndex,
                                                  _RwD3DAdapterType,
                                                  _RwD3D8AdapterInformation.mode.Format,
                                                  Windowed,
                                                  (D3DMULTISAMPLE_TYPE)SelectedMultisamplingLevels );

        if( SUCCEEDED(status) )
        {
            break;
        }

        SelectedMultisamplingLevels--;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8EngineSetRefreshRate sets the rate in Hertz at which the display adapter
 * refreshes the screen.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * If the current subsystem and video mode doesn't support the specified
 * refresh rate, RWD3D8 will choose the highest supported refresh rate \e below
 * that which was specified.
 *
 * \param refreshRate The rate in Hertz at which the display adapter refreshes the screen.
 * A value of \e 0 means use the default video card refresh rate.
 *
 * \see RwEngineOpen
 * \see RwEngineSetSubSystem
 * \see RwEngineSetVideoMode
 * \see RwEngineStart
 *
 */void
RwD3D8EngineSetRefreshRate(RwUInt32 refreshRate)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8EngineSetRefreshRate"));

    FullScreenRefreshRateInHz = refreshRate;

    if (FullScreenRefreshRateInHz)
    {
        RwUInt32        nCloserMatch;
        D3DDISPLAYMODE  mode;
        RwInt32         n;

        nCloserMatch = 0;
        for (n = 0; n < _RwD3D8AdapterInformation.modeCount; n++)
        {
            IDirect3D8_EnumAdapterModes(_RwDirect3DObject,
                                        _RwD3DAdapterIndex, n, &mode);

            if (mode.RefreshRate == FullScreenRefreshRateInHz)
            {
                nCloserMatch = FullScreenRefreshRateInHz;

                break;
            }
            else if (mode.RefreshRate < FullScreenRefreshRateInHz)
            {
                if (nCloserMatch < mode.RefreshRate)
                {
                    nCloserMatch = mode.RefreshRate;
                }
            }
        }

        if (FullScreenRefreshRateInHz != nCloserMatch)
        {
            #if defined(RWDEBUG)
            RwChar  buffer[256];

            if (nCloserMatch)
            {
                rwsprintf(buffer, "A refresh rate of %dHz is not supported, using %dHz instead.",
                       FullScreenRefreshRateInHz,
                       nCloserMatch);
            }
            else
            {
                rwsprintf(buffer, "A refresh rate of %dHz is not supported,"
                                  " using the video card default refresh rate.",
                       FullScreenRefreshRateInHz);
            }

            RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8EngineSetRefreshRate",
                   buffer);
            #endif

            FullScreenRefreshRateInHz = nCloserMatch;
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8EngineSetMultiThreadSafe indicates that the application requests Direct3D
 * to be multithread safe. This makes Direct3D take its global critical section more frequently,
 * which can degrade performance.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * \param enable TRUE if the Direct3D device should be created as multithread safe, FALSE otherwise.
 *
 * \see RwEngineOpen
 * \see RwEngineStart
 *
 */
void
RwD3D8EngineSetMultiThreadSafe(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8EngineSetMultiThreadSafe"));

    EnableMultithreadSafe = enable;

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8EngineSetSoftwareVertexProcessing indicates that the application requests Direct3D
 * to use software vertex processing.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * \param enable TRUE if the Direct3D device should be created to use software vertex processing,
 * FALSE otherwise.
 *
 * \see RwEngineOpen
 * \see RwEngineStart
 *
 */
void
RwD3D8EngineSetSoftwareVertexProcessing(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8EngineSetSoftwareVertexProcessing"));

    EnableSoftwareVertexProcessing = enable;

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8ChangeVideoMode is used to force an immediate change of video mode.
 *
 * This function should be called before any graphics are rendered or
 * immediately after \ref RwRasterShowRaster or \ref RwCameraShowRaster and
 * before the next frame is submitted.
 *
 * \note This function can not be called before \ref RwEngineStart.
 *
 * \param modeIndex  An \ref RwInt32 value equal to the video mode's index.
 *
 * \return Returns TRUE if successful, FALSE otherwise.
 *
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetVideoModeInfo
 */
RwBool
RwD3D8ChangeVideoMode(RwInt32 modeIndex)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8ChangeVideoMode"));
    RWASSERT(SystemStarted != FALSE);
    RWASSERT(InsideScene == FALSE);

    if (SystemStarted &&
        (modeIndex >= 0) &&
        (modeIndex < NumDisplayModes))
    {
        if (_RwD3D8CurrentModeIndex != modeIndex)
        {
            D3DPRESENT_PARAMETERS   oldPresent;
            HRESULT hr;

            oldPresent = Present;

            D3D8DeviceReleaseVideoMemory();

            hr = IDirect3DDevice8_TestCooperativeLevel(_RwD3DDevice);

            D3D8SetPresentParameters(&DisplayModes[modeIndex].mode,
                                     DisplayModes[modeIndex].flags,
                                     DesktopDisplayMode.Format);

            hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

            if (SUCCEEDED(hr))
            {
                if (D3D8DeviceRestoreVideoMemory() == FALSE)
                {
                    D3D8DeviceReleaseVideoMemory();

                    hr = E_FAIL;
                }
            }

            if (FAILED(hr))
            {
                #if defined(RWDEBUG)
                RwChar              buffer[256];

                rwsprintf(buffer, "Not enough memory to resize video mode to %dx%d."
                                    "Switching back to %dx%d",
                                    DisplayModes[modeIndex].mode.Width,
                                    DisplayModes[modeIndex].mode.Height,
                                    _RwD3D8AdapterInformation.mode.Width,
                                    _RwD3D8AdapterInformation.mode.Height);

                RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8ChangeVideoMode",
                                    buffer);
                #endif

                Present = oldPresent;

                hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

                if (FAILED(hr))
                {
                    RWRETURN(FALSE);
                }

                D3D8DeviceRestoreVideoMemory();
            }
            else
            {
                RECT rect;

                _RwD3D8CurrentModeIndex = modeIndex;

                _RwD3D8AdapterInformation.mode = DisplayModes[modeIndex].mode;

                _RwD3D8AdapterInformation.flags = DisplayModes[modeIndex].flags;

                _RwD3D8AdapterInformation.displayDepth =
                                    D3D8FindDepth(DisplayModes[modeIndex].mode.Format);

                /* Change window size */
                if (Present.Windowed == FALSE)
                {
                    rect.left = 0;
                    rect.top = 0;
                    rect.right = rect.left + _RwD3D8AdapterInformation.mode.Width;
                    rect.bottom = rect.top + _RwD3D8AdapterInformation.mode.Height;

                    AdjustWindowRectEx(&rect,
                                       GetWindowLong(WindowHandle, GWL_STYLE),
                                       (GetMenu(WindowHandle)!=NULL),
                                       GetWindowLong(WindowHandle, GWL_EXSTYLE));

                    SetWindowPos(WindowHandle, 0,
                                 rect.left, rect.bottom,
                                 rect.right-rect.left,
                                 rect.bottom-rect.top,
                                 SWP_NOMOVE | SWP_NOZORDER);
                }
                else
                {
                    SetWindowLong(WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

                    rect.left = 0;
                    rect.top = 0;
                    rect.right = rect.left + Present.BackBufferWidth;
                    rect.bottom = rect.top + Present.BackBufferHeight;

                    AdjustWindowRectEx(&rect,
                                       GetWindowLong(WindowHandle, GWL_STYLE),
                                       (GetMenu(WindowHandle)!=NULL),
                                       GetWindowLong(WindowHandle, GWL_EXSTYLE));

                    SetWindowPos(WindowHandle, 0,
                                 rect.left, rect.bottom,
                                 rect.right-rect.left,
                                 rect.bottom-rect.top,
                                 SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

                    GetClientRect(WindowHandle, &rect);

                    _RwD3D8AdapterInformation.mode.Width = rect.right;
                    _RwD3D8AdapterInformation.mode.Height = rect.bottom;
                }
            }
        }
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwcamerad3d8
 * \ref RwD3D8CameraAttachWindow can be used to attach an \ref RwCamera to
 * a specific window handle so that multiple windows may be rendered to in
 * one instance of RenderWare Graphics.
 *
 * Call this function \e before the camera rendering function in your application's
 * render loop. The preferred order of use is:
 * -# create a window
 * -# create a camera
 * -# attach the window
 * -# render through camera
 *
 * \param camera Void pointer to the camera to attach the window to.
 * \param hwnd   Void pointer to the window handle to attach to the camera.
 *
 * \return Returns TRUE if successful, FALSE otherwise.
 *
 * \see RwCameraBeginUpdate
 * \see RwCameraEndUpdate
 * \see RwCameraClear
 * \see RwCameraShowRaster
 * \see RwCameraCreate
 *
 */
RwBool
RwD3D8CameraAttachWindow(void *camera, void *hwnd)
{
    RwBool              rv = FALSE;

    RWAPIFUNCTION(RWSTRING("RwD3D8CameraAttachWindow"));
    RWASSERT(camera);

    if (Present.Windowed)
    {
        if (WindowHandle != (HWND)hwnd)
        {
            RwRaster           *raster;
            RwRaster           *zRaster;
            D3DPRESENT_PARAMETERS present;
            LPDIRECT3DSWAPCHAIN8 swapChain;

            raster = RwCameraGetRaster((RwCamera *) camera);

            if (raster)
            {
                (void)RwCameraSetRaster((RwCamera *) camera, NULL);

                RwRasterDestroy(raster);
            }

            zRaster = RwCameraGetZRaster((RwCamera *) camera);

            if (zRaster)
            {
                (void)RwCameraSetZRaster((RwCamera *) camera, NULL);

                RwRasterDestroy(zRaster);
            }

            present = Present;

            present.hDeviceWindow = (HWND) hwnd;
            present.BackBufferWidth = 0;
            present.BackBufferHeight = 0;
            present.EnableAutoDepthStencil = FALSE;

            if (D3D_OK ==
                DXCHECK(IDirect3DDevice8_CreateAdditionalSwapChain
                        (_RwD3DDevice, &present, &swapChain)))
            {
                LPSURFACE           surfaceLevel;
                D3DSURFACE_DESC     d3dDesc;
                _rwD3D8RasterExt   *rasterExt;

                IDirect3DSwapChain8_GetBackBuffer(swapChain, 0,
                                                  D3DBACKBUFFER_TYPE_MONO,
                                                  &surfaceLevel);

                IDirect3DSurface8_GetDesc(surfaceLevel, &d3dDesc);

                raster =
                    RwRasterCreate(d3dDesc.Width, d3dDesc.Height, 0,
                                   rwRASTERTYPECAMERA |
                                   rwRASTERDONTALLOCATE);
                if (raster)
                {
                    raster->width = d3dDesc.Width;
                    raster->height = d3dDesc.Height;
                    raster->depth = D3D8FindDepth(d3dDesc.Format);

                    rasterExt = RASTEREXTFROMRASTER(raster);

                    rasterExt->texture = (LPDIRECT3DTEXTURE8) surfaceLevel;
                    rasterExt->window = (HWND) hwnd;
                    rasterExt->swapChain = swapChain;

                    (void)RwCameraSetRaster((RwCamera *) camera, raster);
                }

                zRaster =
                    RwRasterCreate(d3dDesc.Width, d3dDesc.Height, 0,
                                   rwRASTERTYPEZBUFFER);

                if (zRaster)
                {
                    (void)RwCameraSetZRaster((RwCamera *) camera, zRaster);
                }

                IDirect3DSurface8_Release(surfaceLevel);

                rv = TRUE;
            }
        }
#if defined(RWDEBUG)
        else
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D8CameraAttachWindow",
                               "This function should not be called for the main window!.");
        }
#endif
    }

    RWRETURN(rv);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           Camera Begin/End

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwD3D8CameraBeginUpdate

 Start update to camera

 On entry   : NULL
            : pCamera - camera to start update to
            : 0
 On exit    : TRUE on success
 */

RwBool
_rwD3D8CameraBeginUpdate(void *out __RWUNUSED__,
                         void *cameraIn, RwInt32 in __RWUNUSED__)
{
    RwCamera            *camera;
    const RwMatrix      *camLTM;
    D3DMATRIX           viewMatrix;
    D3DMATRIX           projMatrix = IdentityMatrix;
    RwRaster            *raster;
    RwRaster            *topRaster;
    _rwD3D8RasterExt    *topRasterExt;
    RwRaster            *zRaster;
    RwRaster            *zTopRaster;
    _rwD3D8RasterExt    *zRasterExt;
    D3DVIEWPORT8        viewport;
    HRESULT             hr;
    RECT                rect;

    RWFUNCTION(RWSTRING("_rwD3D8CameraBeginUpdate"));

#if !defined( NOASM )

    /* set the FPU to single precision */
    _rwProcessorForceSinglePrecision();

#endif /* !defined( NOASM ) */

    /*
     * Save the camera pointer
     */
    camera = (RwCamera *) cameraIn;
    dgGGlobals.curCamera = camera;

    /*
     * View matrix - (camera matrix)
     */
    camLTM = RwFrameGetLTM(RwCameraGetFrame(camera));

    RwMatrixInvert((RwMatrix *)&viewMatrix, camLTM);

    viewMatrix.m[0][0] = -viewMatrix.m[0][0];
    viewMatrix.m[0][3] = 0.0f;

    viewMatrix.m[1][0] = -viewMatrix.m[1][0];
    viewMatrix.m[1][3] = 0.0f;

    viewMatrix.m[2][0] = -viewMatrix.m[2][0];
    viewMatrix.m[2][3] = 0.0f;

    viewMatrix.m[3][0] = -viewMatrix.m[3][0];
    viewMatrix.m[3][3] = 1.0f;

    RwD3D8SetTransform(D3DTS_VIEW, &viewMatrix);

    /*
     * Projection matrix
     */
    projMatrix.m[0][0] = camera->recipViewWindow.x;
    projMatrix.m[1][1] = camera->recipViewWindow.y;

    /* Shear X, Y by view offset with Z invariant */
    projMatrix.m[2][0] =
        camera->recipViewWindow.x * camera->viewOffset.x;
    projMatrix.m[2][1] =
        camera->recipViewWindow.y * camera->viewOffset.y;

    /* Translate to shear origin */
    projMatrix.m[3][0] =
        -camera->recipViewWindow.x * camera->viewOffset.x;

    projMatrix.m[3][1] =
        -camera->recipViewWindow.y * camera->viewOffset.y;

    /* Projection type */
    if (camera->projectionType == rwPARALLEL)
    {
        projMatrix.m[2][2] =
            1.0f / (camera->farPlane - camera->nearPlane);
        projMatrix.m[2][3] = 0.0f;
        projMatrix.m[3][3] = 1.0f;
    }
    else
    {
        projMatrix.m[2][2] =
            camera->farPlane / (camera->farPlane - camera->nearPlane);
        projMatrix.m[2][3] = 1.0f;
        projMatrix.m[3][3] = 0.0f;
    }

    projMatrix.m[3][2] = -projMatrix.m[2][2] * camera->nearPlane;

    RwD3D8SetTransform(D3DTS_PROJECTION, &projMatrix);

    /*
     * Set the render target
     */
    raster = RwCameraGetRaster(camera);

    /*
     * Got to get the top level raster as this is the only one with a
     * texture surface
     */
    topRaster = RwRasterGetParent(raster);

    topRasterExt = RASTEREXTFROMRASTER(topRaster);

    if (topRasterExt->swapChain)
    {
        GetClientRect(topRasterExt->window, &rect);

        if (!rect.right || !rect.bottom)
        {
            RWRETURN(FALSE);
        }
        else
        {
            if (rect.right != topRaster->originalWidth ||
                rect.bottom != topRaster->originalHeight)
            {
                if (raster == topRaster)
                {
                    RwD3D8CameraAttachWindow(camera, topRasterExt->window);

                    /* Update raster pointers */
                    raster = RwCameraGetRaster(camera);
                    topRaster = RwRasterGetParent(raster);
                    topRasterExt = RASTEREXTFROMRASTER(topRaster);
                }

                zRaster = RwCameraGetZRaster((RwCamera *) camera);
            }
            else
            {
                zRaster = RwCameraGetZRaster((RwCamera *) camera);

                if (zRaster && zRaster == RwRasterGetParent(zRaster))
                {
                    if (rect.right != zRaster->width ||
                        rect.bottom != zRaster->height)
                    {
                        RwCameraSetZRaster(camera, NULL);

                        RwRasterDestroy(zRaster);

                        zRaster =
                            RwRasterCreate(rect.right, rect.bottom, 0,
                                           rwRASTERTYPEZBUFFER);

                        if (zRaster)
                        {
                            RwCameraSetZRaster(camera, zRaster);
                        }
                    }
                }
            }

            if (zRaster)
            {
                zTopRaster = RwRasterGetParent(zRaster);

                zRasterExt = RASTEREXTFROMRASTER(zTopRaster);

                DXCHECK(IDirect3DDevice8_SetRenderTarget
                        (_RwD3DDevice,
                         (LPDIRECT3DSURFACE8) topRasterExt->texture,
                         (LPDIRECT3DSURFACE8) zRasterExt->texture));
            }
            else
            {
                DXCHECK(IDirect3DDevice8_SetRenderTarget
                        (_RwD3DDevice,
                         (LPDIRECT3DSURFACE8) topRasterExt->texture, NULL));
            }
        }
    }
    else
    {
        /* Check if the main window has changed its size */
        GetClientRect(WindowHandle, &rect);

        if (!rect.right || !rect.bottom || IsIconic(WindowHandle))
        {
            RWRETURN(FALSE);
        }
        else if ((RwUInt32) rect.right !=
                 _RwD3D8AdapterInformation.mode.Width
                 || (RwUInt32) rect.bottom !=
                 _RwD3D8AdapterInformation.mode.Height)
        {
            D3D8DeviceReleaseVideoMemory();

            hr = IDirect3DDevice8_TestCooperativeLevel(_RwD3DDevice);

            if (SUCCEEDED(hr) && Present.Windowed)
            {
                Present.BackBufferWidth = rect.right;
                Present.BackBufferHeight = rect.bottom;
            }

            hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

            if (SUCCEEDED(hr))
            {
                if (!D3D8DeviceRestoreVideoMemory())
                {
                    D3D8DeviceReleaseVideoMemory();

                    hr = E_FAIL;
                }
            }

            if (FAILED(hr))
            {
                if (Present.Windowed)
                {
                    #if defined(RWDEBUG)
                    RwChar              buffer[256];

                    rwsprintf(buffer, "Not enough memory to resize the window to %dx%d."
                                       "Switching back to %dx%d",
                                       rect.right, rect.bottom,
                                       _RwD3D8AdapterInformation.mode.Width,
                                       _RwD3D8AdapterInformation.mode.Height);

                    RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D8CameraBeginUpdate",
                                       buffer);
                    #endif

                    Present.BackBufferWidth = _RwD3D8AdapterInformation.mode.Width;
                    Present.BackBufferHeight = _RwD3D8AdapterInformation.mode.Height;

                    hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

                    if (FAILED(hr))
                    {
                        RWRETURN(FALSE);
                    }

                    /* Change window size */
                    rect.right = rect.left + _RwD3D8AdapterInformation.mode.Width;
                    rect.bottom = rect.top + _RwD3D8AdapterInformation.mode.Height;

                    if (GetWindowLong(WindowHandle, GWL_STYLE) & WS_MAXIMIZE)
                    {
                        SetWindowLong(WindowHandle, GWL_STYLE, GetWindowLong(WindowHandle, GWL_STYLE) & ~WS_MAXIMIZE);
                    }

                    AdjustWindowRectEx(&rect,
                                    GetWindowLong(WindowHandle, GWL_STYLE),
                                    (GetMenu(WindowHandle)!=NULL),
                                    GetWindowLong(WindowHandle, GWL_EXSTYLE));

                    SetWindowPos(WindowHandle, 0,
                                rect.left, rect.bottom,
                                rect.right-rect.left,
                                rect.bottom-rect.top,
                                SWP_NOMOVE | SWP_NOZORDER);

                    D3D8DeviceRestoreVideoMemory();
                }
                else
                {
                    RWRETURN(FALSE);
                }
            }
            else
            {
                _RwD3D8AdapterInformation.mode.Width = rect.right;
                _RwD3D8AdapterInformation.mode.Height = rect.bottom;
            }
        }

        /* Check if it's a camera texture */
        if (rwRASTERTYPECAMERATEXTURE == topRaster->cType)
        {
            LPSURFACE           surfaceLevel;
            D3DSURFACE_DESC     d3d8Desc;

            /* Set the render & depth/stencil surface */
            if (topRasterExt->cube)
            {
                IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)topRasterExt->texture,
                                                        (D3DCUBEMAP_FACES)topRasterExt->face,
                                                        0,
                                                        &surfaceLevel);
            }
            else
            {
                IDirect3DTexture8_GetSurfaceLevel(topRasterExt->texture, 0,
                                                  &surfaceLevel);
            }

            IDirect3DSurface8_GetDesc(surfaceLevel, &d3d8Desc);

            if (d3d8Desc.Usage == D3DUSAGE_RENDERTARGET)
            {
                zRaster = RwCameraGetZRaster((RwCamera *) camera);
                if (zRaster)
                {
                    /*
                    * Got to get the top level raster as this is the only one with a
                    * texture surface
                    */
                    zTopRaster = RwRasterGetParent(zRaster);

                    zRasterExt = RASTEREXTFROMRASTER(zTopRaster);

                    hr = DXCHECK(IDirect3DDevice8_SetRenderTarget
                                 (_RwD3DDevice, surfaceLevel,
                                  (LPDIRECT3DSURFACE8) zRasterExt->texture));
                }
                else
                {
                    hr = DXCHECK(IDirect3DDevice8_SetRenderTarget
                                 (_RwD3DDevice, surfaceLevel, NULL));
                }
            }
            else
            {
                hr = E_FAIL;
            }

            if (FAILED(hr))
            {
                NeedToCopyFromBackBuffer = TRUE;
            }

            IDirect3DSurface8_Release(surfaceLevel);
        }
    }

    /*
     * Set the viewport
     */
    viewport.X = raster->nOffsetX;
    viewport.Y = raster->nOffsetY;
    viewport.Width = raster->width;
    viewport.Height = raster->height;
    viewport.MinZ = MINZBUFFERVALUE;
    viewport.MaxZ = MAXZBUFFERVALUE;

    DXCHECK(IDirect3DDevice8_SetViewport(_RwD3DDevice, &viewport));

    /*
     * Begin a scene and exit
     */
    RWRETURN(_rwD3D8BeginScene());
}

/****************************************************************************
 _rwD3D8CameraEndUpdate

 End update to camera, and tidy up

 On entry   : NULL
            : pCamera - camera to end update to
            : 0
 On exit    : TRUE on success
 */

RwBool
_rwD3D8CameraEndUpdate(void *out __RWUNUSED__,
                       void *cameraIn, RwInt32 nIn __RWUNUSED__)
{
    RwRaster            *raster;
    RwRaster            *topRaster;
    const _rwD3D8RasterExt  *topRasterExt;

    RWFUNCTION(RWSTRING("_rwD3D8CameraEndUpdate"));

    raster = RwCameraGetRaster(dgGGlobals.curCamera);

    /*
     * Got to get the top level raster as this is the only one with a
     * texture surface
     */
    topRaster = RwRasterGetParent(raster);

    topRasterExt = RASTEREXTFROMCONSTRASTER(topRaster);

    if (topRasterExt->swapChain)
    {
        /* Set the render & depth/stencil surface */
        DXCHECK(IDirect3DDevice8_SetRenderTarget
                (_RwD3DDevice, _RwD3D8RenderSurface,
                 _RwD3D8DepthStencilSurface));
    }
    else
    {
        /* If the cameras raster was a camera texture, set the render target and
         * depth/stencil surfaces back
         */
        if (rwRASTERTYPECAMERATEXTURE == (topRaster->cType))
        {
            if (NeedToCopyFromBackBuffer)
            {
                LPSURFACE           surfaceLevel;
                RECT                rectSrc;

                if (topRasterExt->cube)
                {
                    IDirect3DCubeTexture8_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE8)topRasterExt->texture,
                                                            (D3DCUBEMAP_FACES)topRasterExt->face,
                                                            0,
                                                            &surfaceLevel);
                }
                else
                {
                    IDirect3DTexture8_GetSurfaceLevel(topRasterExt->texture, 0,
                                                      &surfaceLevel);
                }

                /* Copy from the backbuffer to the camera texture */
                rectSrc.left = raster->nOffsetX;
                rectSrc.top = raster->nOffsetY;
                rectSrc.right = rectSrc.left + raster->width;
                rectSrc.bottom = rectSrc.top + raster->height;

                DXCHECK(IDirect3DDevice8_CopyRects
                        (_RwD3DDevice, _RwD3D8RenderSurface, &rectSrc,
                         1, surfaceLevel, NULL));

                IDirect3DSurface8_Release(surfaceLevel);

                NeedToCopyFromBackBuffer = FALSE;
            }

            /* Set the render & depth/stencil surface */
            DXCHECK(IDirect3DDevice8_SetRenderTarget
                    (_RwD3DDevice, _RwD3D8RenderSurface,
                     _RwD3D8DepthStencilSurface));
        }
    }

    /* Remove current camera, so we don't use a camera if it becomes destroyed
     * (since we don't get a look in at destruction time).
     */
    dgGGlobals.curCamera = NULL;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RasterShowRaster

 Show raster to screen

 On entry   : Raster
            : Device specific parameter - HWND
 On exit    : TRUE on success
 */

RwBool
_rwD3D8RasterShowRaster(void *raster __RWUNUSED__,
                        void *inOut __RWUNUSED__,
                        RwInt32 flags)
{
    RwBool              doReset = FALSE;
    RwRaster            *topRaster;
    _rwD3D8RasterExt    *topRasterExt;
    HRESULT             hr;

    RWFUNCTION(RWSTRING("_rwD3D8RasterShowRaster"));

    if (InsideScene)
    {
        hr = DXCHECK(IDirect3DDevice8_EndScene(_RwD3DDevice));

        InsideScene = FALSE;
    }

#ifdef RWMETRICS
    /* We're done with these now */
    _rwD3D8MetricsReset();
#endif /* RWMETRICS */

    /* Check if we need to change the VSync present flags */
    if ((flags & rwRASTERFLIPWAITVSYNC) != 0)
    {
        if (Present.Windowed)
        {
            if (Present.SwapEffect == D3DSWAPEFFECT_COPY)
            {
                Present.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;

                doReset = TRUE;
            }
        }
        else
        {
            if (Present.FullScreen_PresentationInterval ==
                D3DPRESENT_INTERVAL_IMMEDIATE)
            {
                Present.FullScreen_PresentationInterval =
                                    D3DPRESENT_INTERVAL_ONE;

                doReset = TRUE;
            }
        }
    }
    else
    {
        if (Present.Windowed)
        {
            if (Present.SwapEffect == D3DSWAPEFFECT_COPY_VSYNC)
            {
                Present.SwapEffect = D3DSWAPEFFECT_COPY;

                doReset = TRUE;
            }
        }
        else
        {
            if (Present.FullScreen_PresentationInterval ==
                D3DPRESENT_INTERVAL_ONE)
            {
                Present.FullScreen_PresentationInterval =
                                    D3DPRESENT_INTERVAL_IMMEDIATE;

                doReset = TRUE;
            }
        }
    }

    /* Change present flags if needed */
    if (doReset)
    {
        D3D8DeviceReleaseVideoMemory();

        hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

        if (SUCCEEDED(hr))
        {
            D3D8DeviceRestoreVideoMemory();
        }
    }

    /* Do the flip */

    /*
     * Got to get the top level raster as this is the only one with a
     * texture surface
     */
    topRaster = RwRasterGetParent((RwRaster *)raster);

    topRasterExt = RASTEREXTFROMRASTER(topRaster);

    if (topRasterExt->swapChain)
    {
        hr = IDirect3DSwapChain8_Present
                     (topRasterExt->swapChain, NULL, NULL, NULL, NULL);
    }
    else
    {
        hr = IDirect3DDevice8_Present
                     (_RwD3DDevice, NULL, NULL, UNUSED_D3D8,
                      UNUSED_D3D8);
    }

    /* Check for lost devices */
    if (hr == D3DERR_DEVICELOST)
    {
        hr = IDirect3DDevice8_TestCooperativeLevel(_RwD3DDevice);

        if (hr == D3DERR_DEVICENOTRESET)
        {
#if defined(RWDEBUG)
            RwDebugSendMessage(rwDEBUGMESSAGE,
                               "_rwD3D8RasterShowRaster",
                               "DEVICE LOST");
#endif

            D3D8DeviceReleaseVideoMemory();

            hr = DXCHECK(IDirect3DDevice8_Reset(_RwD3DDevice, &Present));

            if (SUCCEEDED(hr))
            {
                D3D8DeviceRestoreVideoMemory();
            }
        }
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwDeviceGetHandle

 On entry   :
 On exit    : Device block handle
 */

RwDevice           *
_rwDeviceGetHandle(void)
{
    static RwDevice     rwD3D8DriverDevice = {
        1.0f,                   /* default gamma correction */
        D3D8System,
        MINZBUFFERVALUE, MAXZBUFFERVALUE, /* near and far Z buffer limits */

        _rwD3D8RWSetRenderState, /* RwRenderStateSetFunction */
        _rwD3D8RWGetRenderState, /* RwRenderStateGetFunction */

        _rwD3D8Im2DRenderLine,  /* RwIm2DRenderLineFunction */
        _rwD3D8Im2DRenderTriangle, /* RwIm2DRenderTriangleFunction */
        _rwD3D8Im2DRenderPrimitive, /* RwIm2DRenderPrimitiveFunction */
        _rwD3D8Im2DRenderIndexedPrimitive, /* RwIm2DRenderIndexedPrimitiveFunction */

        NULL, NULL, NULL, NULL  /* These get set up when the 3D Immediate mode module is setup */
    };

    RWFUNCTION(RWSTRING("_rwDeviceGetHandle"));

    RWRETURN(&rwD3D8DriverDevice);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DeviceSupportsDXTTexture is used to query the
 * D3D8 device as to whether D3D8 compressed textures can be loaded as
 * RenderWare textures. If this function returns TRUE,
 * \ref RwD3D8DDSTextureRead will load compressed DDS files
 * into RenderWare textures.
 *
 * Note that this function must be called after \ref RwEngineStart.
 *
 * \return Returns TRUE if compressed textures can be loaded as RenderWare
 * textures or FALSE otherwise.
 *
 * \see RwD3D8DDSTextureRead
 * \see RwEngineStart
 *
 */
RwBool
RwD3D8DeviceSupportsDXTTexture(void)
{
    RwBool              supported = FALSE;

    RWAPIFUNCTION(RWSTRING("RwD3D8DeviceSupportsDXTTexture"));

    if (SUCCEEDED
        (IDirect3D8_CheckDeviceFormat
         (_RwDirect3DObject, _RwD3DAdapterIndex, _RwD3DAdapterType,
          _RwD3D8AdapterInformation.mode.Format, 0, D3DRTYPE_TEXTURE,
          D3DFMT_DXT1))
        ||
        SUCCEEDED(IDirect3D8_CheckDeviceFormat
                  (_RwDirect3DObject, _RwD3DAdapterIndex,
                   _RwD3DAdapterType,
                   _RwD3D8AdapterInformation.mode.Format, 0,
                   D3DRTYPE_TEXTURE, D3DFMT_DXT2))
        ||
        SUCCEEDED(IDirect3D8_CheckDeviceFormat
                  (_RwDirect3DObject, _RwD3DAdapterIndex,
                   _RwD3DAdapterType,
                   _RwD3D8AdapterInformation.mode.Format, 0,
                   D3DRTYPE_TEXTURE, D3DFMT_DXT3))
        ||
        SUCCEEDED(IDirect3D8_CheckDeviceFormat
                  (_RwDirect3DObject, _RwD3DAdapterIndex,
                   _RwD3DAdapterType,
                   _RwD3D8AdapterInformation.mode.Format, 0,
                   D3DRTYPE_TEXTURE, D3DFMT_DXT4))
        ||
        SUCCEEDED(IDirect3D8_CheckDeviceFormat
                  (_RwDirect3DObject, _RwD3DAdapterIndex,
                   _RwD3DAdapterType,
                   _RwD3D8AdapterInformation.mode.Format, 0,
                   D3DRTYPE_TEXTURE, D3DFMT_DXT5)))
    {
        supported = TRUE;
    }

    RWRETURN(supported);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetStencilClear sets the value to store in each
 * stencil-buffer entry.
 *
 * \ref rwCAMERACLEARSTENCIL can be OR'd into the camera clear flags to
 * clear the stencil buffer.
 *
 * \param stencilClear This parameter can be in the range from 0 through
 * (2 to the power \e n) - 1, where \e n is the bit depth of the stencil buffer.
 *
 * \see RwD3D8GetStencilClear
 * \see RwCameraClear
 * \see RwCameraClearMode
 */
void
RwD3D8SetStencilClear(RwUInt32 stencilClear)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetStencilClear"));

    StencilClearValue = stencilClear;

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8GetStencilClear returns the current value that will be stored in
 * each stencil-buffer entry when it is cleared.
 *
 * \returns Stencil clear value, in the range 0 through (2 to the power \e n) - 1,
 * where \e n is the bit depth of the stencil buffer.
 *
 * \see RwD3D8SetStencilClear
 * \see RwCameraClear
 */
RwUInt32
RwD3D8GetStencilClear(void)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8GetStencilClear"));

    RWRETURN(StencilClearValue);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8GetCurrentD3DDevice is used to determine which (if any)
 * Direct3D8 device is being used for 3D rendering of the current scene.
 *
 * It is expected that, once cast to a LPDIRECT3DDEVICE8 pointer, the
 * device returned by this function can be used to call D3D8 functions from
 * outside of RenderWare. This should be done with caution however as
 * RenderWare maintains an internal cache that will not be updated by
 * external D3D8 calls. In fact, the cache is made invalid when
 * \ref RwD3D8GetCurrentD3DDevice is called. Because of this, if the device
 * is required, it is recommended that it is obtained immediately after
 * \ref RwEngineStart is called since this does not invalidate the cache.
 *
 * \note This function does \e not increment any reference counts.
 *
 * \return Returns a pointer to the Direct3D8 device used for rendering or NULL
 * if there is a error.
 *
 */
void               *
RwD3D8GetCurrentD3DDevice(void)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8GetCurrentD3DDevice"));

    D3D8ClearCache();

    RWRETURN((void *) _RwD3DDevice);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8GetCurrentD3DRenderTarget is used to determine which (if
 * any) LPDIRECT3DSURFACE8 is being used as the target for 3D rendering of the
 * current scene.
 *
 * It is expected that, once cast to a LPDIRECT3DSURFACE8 pointer, the
 * surface returned by this function can be used as a parameter to D3D
 * functions from outside of RenderWare, giving greater flexibility to the
 * programmer at the expense of portability. This should be done with caution
 * however as RenderWare maintains an internal cache that will not be updated
 * by external D3D8 calls.
 *
 * \note Calling this method will increase the internal reference count on the
 * IDirect3DSurface8 interface. Failure to call IUnknown::Release when finished
 * using this IDirect3DSurface8 interface results in a memory leak.
 *
 * \return Returns a pointer to the render target surface used for rendering or NULL
 * if there is a error.
 *
 */
void               *
RwD3D8GetCurrentD3DRenderTarget(void)
{
    LPDIRECT3DSURFACE8  surface = NULL;

    RWAPIFUNCTION(RWSTRING("RwD3D8GetCurrentD3DRenderTarget"));

    DXCHECK(IDirect3DDevice8_GetRenderTarget(_RwD3DDevice, &surface));

    RWRETURN((void *) surface);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetVertexShader is used to set the vertex shader used to process
 * vertices in RWD3D8.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::SetVertexShader since RenderWare can optimize
 * performance using it's internal cache. See \ref d3d8cache.
 *
 * \param handle Handle to the vertex shader to be set. This handle is returned from
 * IDirect3DDevice8::CreateVertexShader or an FVF code.
 *
 * \return TRUE if the vertex shader was set, FALSE otherwise
 *
 * \see RwD3D8CreateVertexShader
 * \see RwD3D8DeleteVertexShader
 * \see RwD3D8SetVertexShaderConstant
 *
 */
RwBool
RwD3D8SetVertexShader(RwUInt32 handle)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetVertexShader"));

    if (LastVertexShaderUsed != handle)
    {
        HRESULT             hr;

        hr = DXCHECK(IDirect3DDevice8_SetVertexShader
                     (_RwD3DDevice, handle));

        if (FAILED(hr))
        {
            LastVertexShaderUsed = 0xffffffff;

            RWRETURN(FALSE);
        }
        else
        {
            LastVertexShaderUsed = handle;
        }
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetPixelShader is used to set the pixel shader used to process
 * pixels in RWD3D8.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8:SetPixelShader since RenderWare can optimize
 * performance using it's internal cache. See \ref d3d8cache.
 *
 * \param handle Handle to the pixel shader to be set. This handle is
 * returned from IDirect3DDevice8::CreatePixelShader or is NULL (zero) if the
 * legacy pipeline is to be used.
 *
 * \return TRUE if the pixel shader was set, FALSE otherwise.
 *
 * \see RwD3D8CreatePixelShader
 * \see RwD3D8DeletePixelShader
 * \see RwD3D8SetPixelShaderConstant
 *
 */
RwBool
RwD3D8SetPixelShader(RwUInt32 handle)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetPixelShader"));

    if (LastPixelShaderUsed != handle)
    {
        HRESULT             hr;

        hr = DXCHECK(IDirect3DDevice8_SetPixelShader
                     (_RwD3DDevice, handle));

        if (FAILED(hr))
        {
            LastPixelShaderUsed = 0xffffffff;

            RWRETURN(FALSE);
        }
        else
        {
            LastPixelShaderUsed = handle;
        }
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetStreamSource binds a vertex buffer to a device data stream.
 *
 * This function creates an association between vertex data and a data
 * stream that feeds RWD3D8 primitive processing functions.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::SetStreamSource since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param streamNumber The stream number to set.
 * \param streamData Void pointer to the LPDIRECT3DVERTEXBUFFER8 vertex
 * buffer to be used as the source to the stream.
 * \param stride The size of the vertex formats in the vertex buffer.
 *
 * \return TRUE if the stream source was set, FALSE otherwise.
 *
 * \see RwD3D8DynamicVertexBufferCreate
 * \see RwD3D8DynamicVertexBufferDestroy
 *
 */
RwBool
RwD3D8SetStreamSource(RwUInt32 streamNumber,
                      void *streamData,
                      RwUInt32 stride)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetStreamSource"));

    if ((LastVertexBufferUsed != streamData) ||
        (LastVertexBufferStride != stride))
    {
        const HRESULT       status =
            IDirect3DDevice8_SetStreamSource(_RwD3DDevice,
                                             streamNumber,
                                             (IDirect3DVertexBuffer8*)streamData,
                                             stride);
        const HRESULT       hr = DXCHECK(status);

        LastVertexBufferUsed = streamData;
        LastVertexBufferStride = stride;

#if defined( RWMETRICS )
        _rwD3D8MetricsGet()->numVBSwitches++;
#endif

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetIndices sets the current index array to an index buffer.
 * The single set of indices is used to index all streams of vertex buffers.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::SetIndices since RenderWare can optimize
 * performance using it's internal cache. See \ref d3d8cache.
 *
 * \param indexData Void pointer to the LPDIRECT3DINDEXBUFFER8,
 * representing the index data to be set.
 * \param baseVertexIndex Base value for vertex indices. This value is
 * added to all indices prior to referencing vertex data, defining a starting
 * position in the vertex streams.
 *
 * \return TRUE if the index buffer was set, FALSE otherwise.
 *
 * \see RwD3D8IndexBufferCreate
 *
 */
RwBool
RwD3D8SetIndices(void *indexData, RwUInt32 baseVertexIndex)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetIndices"));

    if ((LastIndexBufferUsed != indexData)
        || (LastBaseIndex != baseVertexIndex))
    {
        const HRESULT       status =
            IDirect3DDevice8_SetIndices(_RwD3DDevice,
                                        (IDirect3DIndexBuffer8 *)indexData,
                                        baseVertexIndex);
        const HRESULT       hr = DXCHECK(status);

        LastIndexBufferUsed = indexData;
        LastBaseIndex = baseVertexIndex;

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DrawIndexedPrimitiveUP renders the specified geometric
 * primitive, based on indexing into an array of vertices with data specified
 * by a user memory pointer.
 *
 * This function is intended for use in applications that are unable to
 * store their vertex data in vertex buffers. This method supports only a
 * single vertex stream. The effect of this call is to use the provided
 * vertex data pointer and stride for vertex stream zero.
 *
 * It is advised that you batch primitives when calling this function to
 * maximize efficiency. Also it should be noted that the user memory pointer
 * functions are slower than those that use a D3D8 vertex buffer.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::DrawIndexedPrimitiveUP since RenderWare
 * can optimize performance using it's internal cache. See \ref d3d8cache.
 * Note that the D3D8 API function differs from that in RWD3D8 in that the
 * former requires the number of \e primitives as an argument whereas the
 * latter requires the number of \e indices.
 *
 * \param primitiveType Member of the D3DPRIMITIVETYPE enumerated type,
 * describing the type of primitive to render.
 * \param minIndex Minimum vertex index, relative to zero (the start of
 * \e indexData), for vertices used during this call.
 * \param numVertices Number of vertices used during this call; starting
 * from \e minIndex.
 * \param numIndices Number of indices used during this calls.
 * \param indexData User memory pointer to the index data.
 * \param vertexStreamZeroData User memory pointer to the vertex data.
 * \param vertexStreamZeroStride The size of each vertex, in \e bytes.
 *
 * \return TRUE if the primitives were rendered, FALSE otherwise.
 *
 * \see RwD3D8DrawPrimitiveUP
 * \see RwD3D8DrawIndexedPrimitive
 * \see RwD3D8DrawPrimitive
 * \see RwD3D8SetVertexShader
 * \see RwD3D8SetPixelShader
 *
 */
RwBool
RwD3D8DrawIndexedPrimitiveUP(RwUInt32 primitiveType,
                             RwUInt32 minIndex,
                             RwUInt32 numVertices,
                             RwUInt32 numIndices,
                             const void *indexData,
                             const void *vertexStreamZeroData,
                             RwUInt32 vertexStreamZeroStride)
{
    HRESULT             hr;
    const D3DPRIMITIVETYPE type = (D3DPRIMITIVETYPE)primitiveType;
    RwUInt32            numPrimitives;

    RWAPIFUNCTION(RWSTRING("RwD3D8DrawIndexedPrimitiveUP"));

    RwD3D8SetStreamSource(0, NULL, 0);
    RwD3D8SetIndices(NULL, 0);

    switch (primitiveType)
    {
        case D3DPT_LINELIST:
            {
                RWASSERT(numIndices > 1);
                numPrimitives = numIndices / 2;
                break;
            }

        case D3DPT_LINESTRIP:
            {
                RWASSERT(numIndices > 1);
                numPrimitives = numIndices - 1;
                break;
            }

        case D3DPT_TRIANGLELIST:
            {
                RWASSERT(numIndices > 2);
                numPrimitives = numIndices / 3;
                break;
            }

        case D3DPT_TRIANGLESTRIP:
            {
                RWASSERT(numIndices > 2);
                numPrimitives = numIndices - 2;
                break;
            }

        case D3DPT_TRIANGLEFAN:
            {
                RWASSERT(numIndices > 2);
                numPrimitives = numIndices - 2;
                break;
            }

        default:
            RWASSERT(FALSE);
            numPrimitives = 0;
            break;
    }

    _rwD3D8RenderStateFlushCache();

    RWASSERT(numPrimitives <= _RwD3D8DeviceCaps.MaxPrimitiveCount);


    hr = DXCHECK(IDirect3DDevice8_DrawIndexedPrimitiveUP(_RwD3DDevice,
                                                         type,
                                                         minIndex,
                                                         numVertices,
                                                         numPrimitives,
                                                         indexData,
                                                         D3DFMT_INDEX16,
                                                         vertexStreamZeroData,
                                                         vertexStreamZeroStride));

#if defined( RWMETRICS )
    if (SUCCEEDED(hr))
    {
        switch (primitiveType)
        {
            case D3DPT_TRIANGLELIST:
            case D3DPT_TRIANGLESTRIP:
            case D3DPT_TRIANGLEFAN:
                RWSRCGLOBAL(metrics)->numProcTriangles += numPrimitives;
                break;
        }
    }
#endif /* defined( RWMETRICS ) */

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DrawPrimitiveUP renders the specified non-indexed geometric
 * primitive with data specified by a user memory pointer.
 *
 * This function is intended for use in applications that are unable to
 * store their vertex data in vertex buffers. This method supports only
 * a single vertex stream. The effect of this call is to use the provided
 * vertex data pointer and stride for vertex stream zero.
 *
 * It is advised that you batch primitives when calling this function to
 * maximize efficiency. Also it should be noted that the user memory pointer
 * functions are slower than those that use a D3D8 vertex buffer.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::DrawPrimitiveUP since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 * Note that the D3D8 API function differs from that in RWD3D8 in that the
 * former requires the number of \e primitives as an argument whereas the
 * latter requires the number of \e vertices.
 *
 * \param primitiveType Member of the D3DPRIMITIVETYPE enumerated type,
 * describing the type of primitive to render.
 * \param numVertices Number of vertices used during this call.
 * \param vertexStreamZeroData User memory pointer to the vertex data.
 * \param vertexStreamZeroStride The size of each vertex, in \e bytes.
 *
 * \return TRUE if the primitives were rendered, FALSE otherwise.
 *
 * \see RwD3D8DrawIndexedPrimitiveUP
 * \see RwD3D8DrawIndexedPrimitive
 * \see RwD3D8DrawPrimitive
 * \see RwD3D8SetVertexShader
 * \see RwD3D8SetPixelShader
 *
 */
RwBool
RwD3D8DrawPrimitiveUP(RwUInt32 primitiveType,
                      RwUInt32 numVertices,
                      const void *vertexStreamZeroData,
                      RwUInt32 vertexStreamZeroStride)
{
    HRESULT             hr;
    const D3DPRIMITIVETYPE type = (D3DPRIMITIVETYPE)primitiveType;
    RwUInt32            numPrimitives;

    RWAPIFUNCTION(RWSTRING("RwD3D8DrawPrimitiveUP"));

    RwD3D8SetStreamSource(0, NULL, 0);
    RwD3D8SetIndices(NULL, 0);

    switch (primitiveType)
    {
        case D3DPT_LINELIST:
            {
                RWASSERT(numVertices > 1);
                numPrimitives = numVertices / 2;
                break;
            }

        case D3DPT_LINESTRIP:
            {
                RWASSERT(numVertices > 1);
                numPrimitives = numVertices - 1;
                break;
            }

        case D3DPT_TRIANGLELIST:
            {
                RWASSERT(numVertices > 2);
                numPrimitives = numVertices / 3;
                break;
            }

        case D3DPT_TRIANGLESTRIP:
            {
                RWASSERT(numVertices > 2);
                numPrimitives = numVertices - 2;
                break;
            }

        case D3DPT_TRIANGLEFAN:
            {
                RWASSERT(numVertices > 2);
                numPrimitives = numVertices - 2;
                break;
            }

        default:
            RWASSERT(FALSE);
            numPrimitives = 0;
            break;
    }

    _rwD3D8RenderStateFlushCache();

    hr = DXCHECK(IDirect3DDevice8_DrawPrimitiveUP(_RwD3DDevice,
                                                  type,
                                                  numPrimitives,
                                                  vertexStreamZeroData,
                                                  vertexStreamZeroStride));

#if defined( RWMETRICS )
    if (SUCCEEDED(hr))
    {
        switch (primitiveType)
        {
            case D3DPT_TRIANGLELIST:
            case D3DPT_TRIANGLESTRIP:
            case D3DPT_TRIANGLEFAN:
                RWSRCGLOBAL(metrics)->numProcTriangles += numPrimitives;
                break;
        }
    }
#endif /* defined( RWMETRICS ) */

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DrawIndexedPrimitive renders the specified geometric primitive,
 * based on indexing into an array of vertices.
 *
 * This function draws indexed primitives from the current set of data
 * input streams.
 *
 * It is advised that you batch primitives when calling this function to
 * maximize efficiency.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::DrawIndexedPrimitive since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 * Note that the D3D8 API function differs from that in RWD3D8 in that the
 * former requires the number of \e primitives as an argument whereas the
 * latter requires the number of \e indices.
 *
 * \param primitiveType Member of the D3DPRIMITIVETYPE enumerated type,
 * describing the type of primitive to render.
 * \param minIndex Minimum vertex index, relative to \e baseVertexIndex, for
 * vertices used during this call. (\e baseVertexIndex is defined in
 * \ref RwD3D8SetIndices.)
 * \param numVertices The number of vertices used during this call starting
 * from \e baseVertexIndex + \e minIndex.
 * \param startIndex Location in the index array to start reading indices.
 * \param numIndices Number of indices used during this call.
 *
 * \return TRUE if the primitives were rendered, FALSE otherwise.
 *
 * \see RwD3D8DrawPrimitive
 * \see RwD3D8DrawPrimitiveUP
 * \see RwD3D8DrawIndexedPrimitiveUP
 * \see RwD3D8SetVertexShader
 * \see RwD3D8SetPixelShader
 * \see RwD3D8SetStreamSource
 * \see RwD3D8SetIndices
 *
 */
RwBool
RwD3D8DrawIndexedPrimitive(RwUInt32 primitiveType,
                           RwUInt32 minIndex,
                           RwUInt32 numVertices,
                           RwUInt32 startIndex, RwUInt32 numIndices)
{
    HRESULT             hr;
    const D3DPRIMITIVETYPE type = (D3DPRIMITIVETYPE)primitiveType;
    RwUInt32            numPrimitives;

    RWAPIFUNCTION(RWSTRING("RwD3D8DrawIndexedPrimitive"));

    switch (primitiveType)
    {
        case D3DPT_LINELIST:
            {
                RWASSERT(numIndices > 1);
                numPrimitives = numIndices / 2;
                break;
            }

        case D3DPT_LINESTRIP:
            {
                RWASSERT(numIndices > 1);
                numPrimitives = numIndices - 1;
                break;
            }

        case D3DPT_TRIANGLELIST:
            {
                RWASSERT(numIndices > 2);
                numPrimitives = numIndices / 3;
                break;
            }

        case D3DPT_TRIANGLESTRIP:
            {
                RWASSERT(numIndices > 2);
                numPrimitives = numIndices - 2;
                break;
            }

        case D3DPT_TRIANGLEFAN:
            {
                RWASSERT(numIndices > 2);
                numPrimitives = numIndices - 2;
                break;
            }

        default:
            RWASSERT(FALSE);
            numPrimitives = 0;
            break;
    }

    _rwD3D8RenderStateFlushCache();

    RWASSERT(numPrimitives <= _RwD3D8DeviceCaps.MaxPrimitiveCount);


    hr = DXCHECK(IDirect3DDevice8_DrawIndexedPrimitive(_RwD3DDevice,
                                                       type,
                                                       minIndex,
                                                       numVertices,
                                                       startIndex,
                                                       numPrimitives));

#if defined( RWMETRICS )
    if (SUCCEEDED(hr))
    {
        switch (primitiveType)
        {
            case D3DPT_TRIANGLELIST:
            case D3DPT_TRIANGLESTRIP:
            case D3DPT_TRIANGLEFAN:
                RWSRCGLOBAL(metrics)->numProcTriangles += numPrimitives;
                break;
        }
    }
#endif /* defined( RWMETRICS ) */

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DrawPrimitive renders a sequence of non-indexed, geometric
 * primitives of the specified type from the current set of data input
 * streams.
 *
 * It is advised that you batch primitives when calling this function to
 * maximize efficiency.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::DrawPrimitive since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 * Note that the D3D8 API function differs from that in RWD3D8 in that the
 * former requires the number of \e primitives as an argument whereas the
 * latter requires the number of \e vertices.
 *
 * \param primitiveType Member of the D3DPRIMITIVETYPE enumerated type,
 * describing the type of primitive to render.
 * \param startVertex Index of the first vertex to load. Beginning at
 * \e startVertex the correct number of vertices will be read out of the
 * current vertex buffer.
 * \param numVertices The number of vertices used during this call starting
 * from \e startVertex.
 *
 * \return TRUE if the primitives were rendered, FALSE otherwise.
 *
 * \see RwD3D8DrawIndexedPrimitive
 * \see RwD3D8DrawPrimitiveUP
 * \see RwD3D8DrawIndexedPrimitiveUP
 * \see RwD3D8SetVertexShader
 * \see RwD3D8SetPixelShader
 * \see RwD3D8SetStreamSource
 * \see RwD3D8SetIndices
 *
 */
RwBool
RwD3D8DrawPrimitive(RwUInt32 primitiveType,
                    RwUInt32 startVertex, RwUInt32 numVertices)
{
    HRESULT             hr;
    const D3DPRIMITIVETYPE type = (D3DPRIMITIVETYPE)primitiveType;
    RwUInt32            numPrimitives;

    RWAPIFUNCTION(RWSTRING("RwD3D8DrawPrimitive"));

    switch (primitiveType)
    {
        case D3DPT_POINTLIST:
            {
                RWASSERT(numVertices >= 1);
                numPrimitives = numVertices;
                break;
            }

        case D3DPT_LINELIST:
            {
                RWASSERT(numVertices > 1);
                numPrimitives = numVertices / 2;
                break;
            }

        case D3DPT_LINESTRIP:
            {
                RWASSERT(numVertices > 1);
                numPrimitives = numVertices - 1;
                break;
            }

        case D3DPT_TRIANGLELIST:
            {
                RWASSERT(numVertices > 2);
                numPrimitives = numVertices / 3;
                break;
            }

        case D3DPT_TRIANGLESTRIP:
            {
                RWASSERT(numVertices > 2);
                numPrimitives = numVertices - 2;
                break;
            }

        case D3DPT_TRIANGLEFAN:
            {
                RWASSERT(numVertices > 2);
                numPrimitives = numVertices - 2;
                break;
            }

        default:
            RWASSERT(FALSE);
            numPrimitives = 0;
            break;
    }

    _rwD3D8RenderStateFlushCache();

    hr = DXCHECK(IDirect3DDevice8_DrawPrimitive(_RwD3DDevice,
                                                type,
                                                startVertex,
                                                numPrimitives));

#if defined( RWMETRICS )
    if (SUCCEEDED(hr))
    {
        switch (primitiveType)
        {
            case D3DPT_TRIANGLELIST:
            case D3DPT_TRIANGLESTRIP:
            case D3DPT_TRIANGLEFAN:
                RWSRCGLOBAL(metrics)->numProcTriangles += numPrimitives;
                break;
        }
    }
#endif /* defined( RWMETRICS ) */

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetTransform sets a transformation-related state. These states refer
 * to the view, projection, texture, and world matrices in D3D8. See the D3D8 API
 * documentation from Microsoft for more details.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::SetTransform since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param state State variable that is being modified, a member of the
 * D3DTRANSFORMSTATETYPE enumerated type.
 * \param matrix Void pointer to a D3DMATRIX structure that modifies the
 * current transformation.
 *
 * \return TRUE if the matrix was set, FALSE otherwise.
 *
 * \see RwD3D8SetTransformWorld
 * \see RwD3D8GetTransform
 * \see RwD3D8SetRenderState
 *
 */
RwBool
RwD3D8SetTransform(RwUInt32 state, const void *matrix)
{
    HRESULT             hr;

    RWAPIFUNCTION(RWSTRING("RwD3D8SetTransform"));

    if (matrix)
    {
        if (LastMatrixUsed[state] &&
            D3D8MemEqual((const RwUInt32 *) LastMatrixUsed[state],
                         (const RwUInt32 *) matrix,
                         sizeof(D3DMATRIX)))
        {
            RWRETURN(TRUE);
        }
        else
        {
            if (!LastMatrixUsed[state])
            {
                LastMatrixUsed[state] =
                                (D3DMATRIX *) RwFreeListAlloc(MatrixFreeList,
                                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
            }

            hr = DXCHECK(IDirect3DDevice8_SetTransform(_RwD3DDevice,
                                                       (D3DTRANSFORMSTATETYPE)state,
                                                       (const D3DMATRIX*) matrix));

            memcpy(LastMatrixUsed[state], matrix, sizeof(D3DMATRIX));

            if (state == D3DTS_WORLD)
            {
                LastWorldMatrixUsedIdentity = FALSE;
            }
        }
    }
    else
    {
        if (LastMatrixUsed[state] &&
            D3D8MemEqual((const RwUInt32 *) LastMatrixUsed[state],
                         (const RwUInt32 *) &IdentityMatrix,
                         sizeof(D3DMATRIX)))
        {
            RWRETURN(TRUE);
        }
        else
        {
            if (!LastMatrixUsed[state])
            {
                LastMatrixUsed[state] =
                                (D3DMATRIX *) RwFreeListAlloc(MatrixFreeList,
                                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
            }

            hr = DXCHECK(IDirect3DDevice8_SetTransform(_RwD3DDevice,
                                                       (D3DTRANSFORMSTATETYPE)state,
                                                       &IdentityMatrix));

            memcpy(LastMatrixUsed[state], &IdentityMatrix, sizeof(D3DMATRIX));

            if (state == D3DTS_WORLD)
            {
                LastWorldMatrixUsedIdentity = TRUE;
            }
        }
    }

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8GetTransform retrieves a matrix describing a transformation
 * state.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::GetTransform since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param state State variable that is being requested, a member of the
 * D3DTRANSFORMSTATETYPE enumerated type.
 * \param matrix Void pointer to a D3DMATRIX structure, describing the
 * returned transformation state.
 *
 * \see RwD3D8SetTransform
 * \see RwD3D8SetTransformWorld
 * \see RwD3D8SetRenderState
 *
 */
void
RwD3D8GetTransform(RwUInt32 state, void *matrix)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8GetTransform"));
    RWASSERT(matrix != NULL);

    if (matrix)
    {
        if (LastMatrixUsed[state])
        {
            memcpy(matrix, LastMatrixUsed[state], sizeof(D3DMATRIX));
        }
        else
        {
            memcpy(matrix, &IdentityMatrix, sizeof(D3DMATRIX));
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetTransformWorld sets the object-to-world transformation
 * matrix.
 *
 * This function internally converts the RenderWare \ref RwMatrix to the
 * equivalent D3D8 D3DMATRIX structure, and sets this as the current
 * world transformation matrix.
 *
 * \param matrix Pointer to a \ref RwMatrix structure that modifies the
 * object-to-world transformation.
 *
 * \return TRUE if the matrix was set, FALSE otherwise.
 *
 * \see RwD3D8SetTransformWorld
 * \see RwD3D8GetTransform
 * \see RwD3D8SetRenderState
 *
 */
RwBool
RwD3D8SetTransformWorld(const RwMatrix * matrix)
{
    static D3DMATRIX    d3dMatrix =
        D3DMatrixInitMacro(0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);

    RWAPIFUNCTION(RWSTRING("RwD3D8SetTransformWorld"));

    if (matrix && !rwMatrixTestFlags(matrix, rwMATRIXINTERNALIDENTITY))
    {
        d3dMatrix.m[0][0] = matrix->right.x;
        d3dMatrix.m[0][1] = matrix->right.y;
        d3dMatrix.m[0][2] = matrix->right.z;

        d3dMatrix.m[1][0] = matrix->up.x;
        d3dMatrix.m[1][1] = matrix->up.y;
        d3dMatrix.m[1][2] = matrix->up.z;

        d3dMatrix.m[2][0] = matrix->at.x;
        d3dMatrix.m[2][1] = matrix->at.y;
        d3dMatrix.m[2][2] = matrix->at.z;

        d3dMatrix.m[3][0] = matrix->pos.x;
        d3dMatrix.m[3][1] = matrix->pos.y;
        d3dMatrix.m[3][2] = matrix->pos.z;

        RWASSERT(LastMatrixUsed[D3DTS_WORLD] != NULL);

        if (LastWorldMatrixUsedIdentity ||
            !D3D8MemEqual((const RwUInt32 *)
                          LastMatrixUsed[D3DTS_WORLD],
                          (const RwUInt32 *) &d3dMatrix,
                          sizeof(D3DMATRIX)))
        {
            HRESULT             hr;

            hr = DXCHECK(IDirect3DDevice8_SetTransform
                                    (_RwD3DDevice, D3DTS_WORLD, &d3dMatrix));

            memcpy(LastMatrixUsed[D3DTS_WORLD], &d3dMatrix, sizeof(D3DMATRIX));

            LastWorldMatrixUsedIdentity = FALSE;

            RWRETURN(SUCCEEDED(hr));
        }
    }
    else
    {
        if (!LastWorldMatrixUsedIdentity)
        {
            HRESULT             hr;

            RWASSERT(LastMatrixUsed[D3DTS_WORLD] != NULL);

            hr = DXCHECK(IDirect3DDevice8_SetTransform
                                (_RwD3DDevice, D3DTS_WORLD, &IdentityMatrix));

            memcpy(LastMatrixUsed[D3DTS_WORLD], &IdentityMatrix,
                   sizeof(D3DMATRIX));

            LastWorldMatrixUsedIdentity = TRUE;

            RWRETURN(SUCCEEDED(hr));
        }
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetMaterial sets the material properties to be used in
 * subsequent rendering operations.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::SetMaterial since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param material Void pointer to a D3DMATERIAL8 structure, describing
 * the material properties to set.
 *
 * \return TRUE if the material was set, FALSE otherwise.
 *
 */
RwBool
RwD3D8SetMaterial(const void *material)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetMaterial"));

    if (!D3D8MemEqual
        ((const RwUInt32 *) &LastMaterial, (const RwUInt32 *) material,
         sizeof(D3DMATERIAL8)))
    {
        HRESULT             hr;

        hr =
            DXCHECK(IDirect3DDevice8_SetMaterial(_RwD3DDevice,
                                                 (const D3DMATERIAL8 *)material));

        memcpy(&LastMaterial, material, sizeof(D3DMATERIAL8));

#if defined( RWMETRICS )
        _rwD3D8MetricsGet()->numMaterialChanges++;
#endif

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetSurfaceProperties sets the surface properties to be used in
 * subsequent rendering operations.
 *
 * This function internally converts the RenderWare \ref RwSurfaceProperties
 * to the equivalent D3D8 D3DMATERIAL8 structure, and sets this as the current
 * material for subsequent rendering operations.
 *
 * \param color Surface base vertex color.
 * \param surfaceProps Pointer to a \ref RwSurfaceProperties structure
 * with the surface properties to be set.
 * \param modulate TRUE if the surface properties needs to be modulated
 * with the base vertex \e color.
 *
 * \return TRUE if the surface properties were set, FALSE otherwise.
 *
 */
RwBool
RwD3D8SetSurfaceProperties(const RwRGBA * color,
                           const RwSurfaceProperties * surfaceProps,
                           RwBool modulate)
{
    static D3DMATERIAL8 material = {
        {0.0f, 0.0f, 0.0f, 1.0f}, /* Diffuse */
        {0.0f, 0.0f, 0.0f, 1.0f}, /* Ambient */
        {0.0f, 0.0f, 0.0f, 1.0f}, /* Specular */
        {0.0f, 0.0f, 0.0f, 1.0f}, /* Emissive */
        0.0f                   /* Power */
    };

    RWAPIFUNCTION(RWSTRING("RwD3D8SetSurfaceProperties"));

    if (modulate && *((const RwUInt32 *) color) != 0xffffffff)
    {
        RwReal              diffScale;
        RwReal              ambScale;

        diffScale = surfaceProps->diffuse * COLORSCALAR;

        material.Diffuse.r = color->red * diffScale;
        material.Diffuse.g = color->green * diffScale;
        material.Diffuse.b = color->blue * diffScale;
        material.Diffuse.a = color->alpha * COLORSCALAR;

        ambScale = surfaceProps->ambient * COLORSCALAR;

        material.Ambient.r =
            color->red * ambScale * AmbientSaturated.red;
        material.Ambient.g =
            color->green * ambScale * AmbientSaturated.green;
        material.Ambient.b =
            color->blue * ambScale * AmbientSaturated.blue;
        material.Ambient.a = color->alpha * COLORSCALAR;
    }
    else
    {
        material.Diffuse.b = material.Diffuse.g =
            material.Diffuse.r = surfaceProps->diffuse;

        material.Diffuse.a = 1.f;

        if (surfaceProps->ambient != 1.f)
        {
            material.Ambient.r =
                surfaceProps->ambient * AmbientSaturated.red;
            material.Ambient.g =
                surfaceProps->ambient * AmbientSaturated.green;
            material.Ambient.b =
                surfaceProps->ambient * AmbientSaturated.blue;
        }
        else
        {
            material.Ambient.r = AmbientSaturated.red;
            material.Ambient.g = AmbientSaturated.green;
            material.Ambient.b = AmbientSaturated.blue;
        }

        material.Ambient.a = 1.f;
    }

    if (FLOATASINT(LastMaterial.Diffuse.r) !=
        FLOATASINT(material.Diffuse.r)
        || FLOATASINT(LastMaterial.Diffuse.g) !=
        FLOATASINT(material.Diffuse.g)
        || FLOATASINT(LastMaterial.Diffuse.b) !=
        FLOATASINT(material.Diffuse.b)
        || FLOATASINT(LastMaterial.Diffuse.a) !=
        FLOATASINT(material.Diffuse.a)
        || FLOATASINT(LastMaterial.Ambient.r) !=
        FLOATASINT(material.Ambient.r)
        || FLOATASINT(LastMaterial.Ambient.g) !=
        FLOATASINT(material.Ambient.g)
        || FLOATASINT(LastMaterial.Ambient.b) !=
        FLOATASINT(material.Ambient.b)
        || FLOATASINT(LastMaterial.Ambient.a) !=
        FLOATASINT(material.Ambient.a))
    {
        HRESULT             hr;

        hr = DXCHECK(IDirect3DDevice8_SetMaterial
                     (_RwD3DDevice, &material));

        memcpy(&LastMaterial, &material, sizeof(D3DMATERIAL8));

#if defined( RWMETRICS )
        _rwD3D8MetricsGet()->numMaterialChanges++;
#endif

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8CheckValidZBufferFormat

 On entry   : Check if the z-buffer format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D8CheckValidZBufferFormat(RwInt32 format)
{
    HRESULT             hr;
    const D3DFORMAT  adapterFormat =_RwD3D8AdapterInformation.mode.Format;

    RWFUNCTION(RWSTRING("_rwD3D8CheckValidZBufferFormat"));

    hr = IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      D3DUSAGE_DEPTHSTENCIL,
                                      D3DRTYPE_SURFACE,
                                      (D3DFORMAT)format);
    if (SUCCEEDED(hr))
    {

        hr = IDirect3D8_CheckDepthStencilMatch(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               adapterFormat,
                                               adapterFormat,
                                               (D3DFORMAT)format);
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D8CheckValidTextureFormat

 On entry   : Check if the texture format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D8CheckValidTextureFormat(RwInt32 format)
{
    const D3DFORMAT  adapterFormat =_RwD3D8AdapterInformation.mode.Format;
    HRESULT             hr;

    RWFUNCTION(RWSTRING("_rwD3D8CheckValidTextureFormat"));

    hr = IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      0,
                                      D3DRTYPE_TEXTURE,
                                      (D3DFORMAT)format);
    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D8CheckValidCameraTextureFormat

 On entry   : Check if the rendertarget format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D8CheckValidCameraTextureFormat(RwInt32 format)
{
    HRESULT             hr;
    const D3DFORMAT  adapterFormat =_RwD3D8AdapterInformation.mode.Format;

    RWFUNCTION(RWSTRING("_rwD3D8CheckValidCameraTextureFormat"));

    hr = IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      D3DUSAGE_RENDERTARGET,
                                      D3DRTYPE_TEXTURE,
                                      (D3DFORMAT)format);

    if (SUCCEEDED(hr))
    {
        hr = IDirect3D8_CheckDepthStencilMatch(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               adapterFormat,
                                               (D3DFORMAT)format,
                                               Present.AutoDepthStencilFormat);
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D8CheckValidCubeTextureFormat

 On entry   : Check if the texture format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D8CheckValidCubeTextureFormat(RwInt32 format)
{
    const D3DFORMAT adapterFormat =_RwD3D8AdapterInformation.mode.Format;
    HRESULT         hr;

    RWFUNCTION(RWSTRING("_rwD3D8CheckValidCubeTextureFormat"));

    hr = IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      0,
                                      D3DRTYPE_CUBETEXTURE,
                                      (D3DFORMAT)format);
    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D8CheckValidCameraCubeTextureFormat

 On entry   : Check if the rendertarget format is valid
            : Parameter
 On exit    : TRUE on success
 */

RwBool
_rwD3D8CheckValidCameraCubeTextureFormat(RwInt32 format)
{
    HRESULT             hr;
    const D3DFORMAT  adapterFormat =_RwD3D8AdapterInformation.mode.Format;

    RWFUNCTION(RWSTRING("_rwD3D8CheckValidCameraCubeTextureFormat"));

    hr = IDirect3D8_CheckDeviceFormat(_RwDirect3DObject,
                                      _RwD3DAdapterIndex,
                                      _RwD3DAdapterType,
                                      adapterFormat,
                                      D3DUSAGE_RENDERTARGET,
                                      D3DRTYPE_CUBETEXTURE,
                                      (D3DFORMAT)format);

    if (SUCCEEDED(hr))
    {
        hr = IDirect3D8_CheckDepthStencilMatch(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               adapterFormat,
                                               (D3DFORMAT)format,
                                               Present.AutoDepthStencilFormat);
    }

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetLight assigns a set of lighting properties in the requested
 * lights array index.
 *
 * It is advised for efficiency purposes for applications using a large number
 * of lights in an array, that the whole array of lights is sent to D3D8 with
 * those individual lights required enabled, rather than sending individual
 * lights.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::SetLight since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param index Zero-based index of the set of lighting properties to assign.
 * If a set of lighting properties exists at this index, it is overwritten
 * by the new properties specified.
 * \param light Void pointer to a D3DLIGHT8 structure, containing the
 * lighting-parameters to set.
 *
 * \return TRUE if the light was set, FALSE otherwise.
 *
 * \see RwD3D8GetLight
 * \see RwD3D8EnableLight
 *
 */
RwBool
RwD3D8SetLight(RwInt32 index, const void *light)
{
    const D3DLIGHT8 *d3dLight = (const D3DLIGHT8 *)light;
    RwBool          doSet = TRUE;

    RWAPIFUNCTION(RWSTRING("RwD3D8SetLight"));

    if (index >= MaxNumLights)
    {
        RwInt32 n;

        n = MaxNumLights;

        MaxNumLights = index + 1;
        if (LightsCache != NULL)
        {
            LightsCache = (RxD3D8Light *)RwRealloc(LightsCache, MaxNumLights * sizeof(RxD3D8Light),
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            LightsCache = (RxD3D8Light *)RwMalloc(MaxNumLights * sizeof(RxD3D8Light),
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }

        for (; n < MaxNumLights; n++)
        {
            LightsCache[n].enabled = FALSE;
        }
    }
    else
    {
        if (_rwD3D8LightsEqual(d3dLight, &(LightsCache[index].light)))
        {
            doSet = FALSE;
        }
    }

    if (doSet)
    {
        HRESULT hr;

        hr = DXCHECK(IDirect3DDevice8_SetLight
                     (_RwD3DDevice, index, d3dLight));

        memcpy(&(LightsCache[index].light), d3dLight, sizeof(D3DLIGHT8));

        #if defined( RWMETRICS )
        _rwD3D8MetricsGet()->numLightsChanged++;
        #endif

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8ForceLight

 On entry   : Set a light without doing any comparasion

 On exit    : TRUE on success
 */
RwBool
_rwD3D8ForceLight(RwInt32 index, const void *light)
{
    const D3DLIGHT8    *d3dLight = (const D3DLIGHT8 *)light;
    HRESULT            hr;

    RWFUNCTION(RWSTRING("_rwD3D8ForceLight"));

    if (index >= MaxNumLights)
    {
        RwInt32 n;

        n = MaxNumLights;

        MaxNumLights = index + 1;

        if (LightsCache != NULL)
        {
            LightsCache = (RxD3D8Light *)RwRealloc(LightsCache, MaxNumLights * sizeof(RxD3D8Light),
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            LightsCache = (RxD3D8Light *)RwMalloc(MaxNumLights * sizeof(RxD3D8Light),
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }

        for (; n < MaxNumLights; n++)
        {
            LightsCache[n].enabled = FALSE;
        }
    }

    hr = DXCHECK(IDirect3DDevice8_SetLight
                 (_RwD3DDevice, index, d3dLight));

    memcpy(&(LightsCache[index].light), d3dLight, sizeof(D3DLIGHT8));

#if defined( RWMETRICS )
    _rwD3D8MetricsGet()->numLightsChanged++;
#endif

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8GetLight retrieves a set of lighting properties for the requested
 * lights array index.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::GetLight since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param index Zero-based index of the set of lighting properties to retrieve.
 * \param light Void pointer to a D3DLIGHT8 structure that is filled with the
 * retrieved lighting-parameter set.
 *
 * \see RwD3D8SetLight
 * \see RwD3D8EnableLight
 *
 */
void
RwD3D8GetLight(RwInt32 index, void *light)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8GetLight"));

    if (index < MaxNumLights)
    {
        memcpy(light, &(LightsCache[index].light), sizeof(D3DLIGHT8));
    }
    else
    {
        memset(light, 0, sizeof(D3DLIGHT8));
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8EnableLight enables or disables a set of lighting parameters.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::EnableLight since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param index Zero-based index of the set of lighting properties that are
 * the target of this function.
 * \param enable Value that indicates if the set of lighting parameters are
 * being enabled or disabled. Set this parameter to TRUE to enable lighting
 * with the parameters at the specified index, or FALSE to disable it.
 *
 * \return TRUE if the function succeeds, FALSE otherwise.
 *
 * \see RwD3D8SetLight
 * \see RwD3D8GetLight
 *
 */
RwBool
RwD3D8EnableLight(RwInt32 index, RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8EnableLight"));

    if (index < MaxNumLights)
    {
        if (LightsCache[index].enabled != enable)
        {
            HRESULT             hr;

            hr = DXCHECK(IDirect3DDevice8_LightEnable
                         (_RwD3DDevice, index, enable));

            LightsCache[index].enabled = enable;

            RWRETURN(SUCCEEDED(hr));
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8LightsEqual

 Purpose:   compares two lights

*/
RwBool
_rwD3D8LightsEqual(const D3DLIGHT8 * light0, const D3DLIGHT8 * light1)
{
    RWFUNCTION(RWSTRING("_rwD3D8LightsEqual"));

    if (light0->Type != light1->Type)
    {
        RWRETURN(FALSE);
    }
    else if (FLOATASINT(light0->Diffuse.r) !=
             FLOATASINT(light1->Diffuse.r)
             || FLOATASINT(light0->Diffuse.g) !=
             FLOATASINT(light1->Diffuse.g)
             || FLOATASINT(light0->Diffuse.b) !=
             FLOATASINT(light1->Diffuse.b))
    {
        RWRETURN(FALSE);
    }
    else
    {
        switch (light0->Type)
        {
            case D3DLIGHT_DIRECTIONAL:
                if (FLOATASINT(light0->Direction.x) !=
                    FLOATASINT(light1->Direction.x)
                    || FLOATASINT(light0->Direction.y) !=
                    FLOATASINT(light1->Direction.y)
                    || FLOATASINT(light0->Direction.z) !=
                    FLOATASINT(light1->Direction.z))
                {
                    RWRETURN(FALSE);
                }
                break;

            case D3DLIGHT_POINT:
                if (FLOATASINT(light0->Position.x) !=
                    FLOATASINT(light1->Position.x)
                    || FLOATASINT(light0->Position.y) !=
                    FLOATASINT(light1->Position.y)
                    || FLOATASINT(light0->Position.z) !=
                    FLOATASINT(light1->Position.z))
                {
                    RWRETURN(FALSE);
                }
                else if (FLOATASINT(light0->Range) !=
                         FLOATASINT(light1->Range))
                {
                    RWRETURN(FALSE);
                }
                break;

            case D3DLIGHT_SPOT:
                if (FLOATASINT(light0->Position.x) !=
                    FLOATASINT(light1->Position.x)
                    || FLOATASINT(light0->Position.y) !=
                    FLOATASINT(light1->Position.y)
                    || FLOATASINT(light0->Position.z) !=
                    FLOATASINT(light1->Position.z))
                {
                    RWRETURN(FALSE);
                }
                else if (FLOATASINT(light0->Direction.x) !=
                         FLOATASINT(light1->Direction.x)
                         || FLOATASINT(light0->Direction.y) !=
                         FLOATASINT(light1->Direction.y)
                         || FLOATASINT(light0->Direction.z) !=
                         FLOATASINT(light1->Direction.z))
                {
                    RWRETURN(FALSE);
                }
                else if (FLOATASINT(light0->Range) !=
                         FLOATASINT(light1->Range))
                {
                    RWRETURN(FALSE);
                }
                else if (FLOATASINT(light0->Theta) !=
                         FLOATASINT(light1->Theta))
                {
                    RWRETURN(FALSE);
                }
                else if (FLOATASINT(light0->Phi) !=
                         FLOATASINT(light1->Phi))
                {
                    RWRETURN(FALSE);
                }

                break;

            case D3DLIGHT_FORCE_DWORD:
                break;

        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8FindSimilarLight

 Purpose:   return the index of a similar light, 0xffffffff on failure

*/
RwUInt32
_rwD3D8FindSimilarLight(const void *light)
{
    RwInt32             index;

    RWFUNCTION(RWSTRING("_rwD3D8FindSimilarLight"));

    for (index = 0; index < MaxNumLights; index++)
    {
        if (_rwD3D8LightsEqual((const D3DLIGHT8 *)light,
                               &(LightsCache[index].light)))
        {
            break;
        }
    }

    RWRETURN(index < MaxNumLights ? index : 0xffffffff);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DynamicVertexBufferCreate creates a vertex buffer of a specified
 * size with each vertex of the format indicated.
 *
 * If an "unused" buffer already exists, of the same size and using the same vertex
 * format, then this will be used in preference to constructing a new vertex buffer.
 *
 * Otherwise, a new vertex buffer is created. If the host video card driver
 * supports hardware TnL then it will created in video memory, otherwise in system
 * memory.
 *
 * While RWD3D8 maintains it's internal cache, vertex buffers will function at
 * optimum performance.
 *
 * \param fvf Combination of D3D8 flexible vertex format flags describing the
 * vertex format used to construct this vertex buffer.
 * \param size Size of the buffer in \e bytes. This must be large enough to store
 * at least one vertex of the format requested.
 * \param vertexBuffer Void pointer to the LPDIRECT3DVERTEXBUFFER8 that is to be
 * the vertex buffer.
 *
 * \return TRUE if the vertex buffer was successfully created, FALSE otherwise.
 *
 * \see RwD3D8DynamicVertexBufferDestroy
 *
 */
RwBool
RwD3D8DynamicVertexBufferCreate(RwUInt32 fvf, RwUInt32 size,
                                void **vertexBuffer)
{
    RxD3D8DynamicVertexBuffer *currentVertexBuffer;
    RxD3D8DynamicVertexBuffer *freeVertexBuffer = NULL;

    RWAPIFUNCTION(RWSTRING("RwD3D8DynamicVertexBufferCreate"));

    currentVertexBuffer = DynamicVertexBufferList;
    while (currentVertexBuffer)
    {
        RxD3D8DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (!currentVertexBuffer->used)
        {
            if (currentVertexBuffer->fvf == fvf &&
                currentVertexBuffer->size == size)
            {
                break;
            }
            else if (freeVertexBuffer == NULL
                     || freeVertexBuffer->size <
                     currentVertexBuffer->size)
            {
                freeVertexBuffer = currentVertexBuffer;
            }
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = TRUE;

        *vertexBuffer = currentVertexBuffer->vertexBuffer;

        currentVertexBuffer->variableAddress =
            (LPDIRECT3DVERTEXBUFFER8 *) vertexBuffer;
    }
    else
    {
        D3DPOOL             d3dPool;

        /* Destroy a free vertex buffer to make room for the new one */
        if (freeVertexBuffer)
        {
            IDirect3DVertexBuffer8_Release(freeVertexBuffer->vertexBuffer);
            freeVertexBuffer->vertexBuffer = NULL;

            currentVertexBuffer = freeVertexBuffer;

            #if defined(RWDEBUG)
            NumDynamicVertexBuffer--;
            #endif /* defined(RWDEBUG) */
        }
        else
        {
            /* Create a new item */
            currentVertexBuffer = (RxD3D8DynamicVertexBuffer *)
                                RwFreeListAlloc(DynamicVertexBufferFreeList,
                                    rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL);

            /* Add it to the beginnig of the list */
            currentVertexBuffer->next = DynamicVertexBufferList;

            DynamicVertexBufferList = currentVertexBuffer;
        }

        if (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        {
            d3dPool = D3DPOOL_DEFAULT;
        }
        else
        {
            d3dPool = D3DPOOL_SYSTEMMEM;
        }

        if (D3D_OK !=
            DXCHECK(IDirect3DDevice8_CreateVertexBuffer
                    (_RwD3DDevice, size,
                     D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, fvf,
                     d3dPool,
                     ((LPDIRECT3DVERTEXBUFFER8 *) vertexBuffer))))
        {
            RWRETURN(FALSE);
        }

        /* Fill item info */
        currentVertexBuffer->fvf = fvf;
        currentVertexBuffer->size = size;
        currentVertexBuffer->used = TRUE;
        currentVertexBuffer->vertexBuffer =
            ((LPDIRECT3DVERTEXBUFFER8) *vertexBuffer);
        currentVertexBuffer->variableAddress =
            ((LPDIRECT3DVERTEXBUFFER8 *) vertexBuffer) ;

        #if defined(RWDEBUG)
        NumDynamicVertexBuffer++;
        #endif /* defined(RWDEBUG) */
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DynamicVertexBufferDestroy destroys a previously created vertex buffer.
 *
 * To be precise, the vertex buffer is not deallocated from memory, but is instead
 * declared to be "unused", and may therefore be reused by a future call to
 * \ref RwD3D8DynamicVertexBufferCreate should the size and vertex formats requested
 * coincide.
 *
 * \param vertexBuffer Void pointer to the vertex buffer to destroy.
 *
 * \see RwD3D8DynamicVertexBufferCreate
 *
 */
void
RwD3D8DynamicVertexBufferDestroy(void *vertexBuffer)
{
    RxD3D8DynamicVertexBuffer *currentVertexBuffer;

    RWAPIFUNCTION(RWSTRING("RwD3D8DynamicVertexBufferDestroy"));

    currentVertexBuffer = DynamicVertexBufferList;
    while (currentVertexBuffer)
    {
        RxD3D8DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (currentVertexBuffer->vertexBuffer ==
            ((LPDIRECT3DVERTEXBUFFER8) vertexBuffer))
        {
            break;
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = FALSE;

        currentVertexBuffer->variableAddress = NULL;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8IndexBufferCreate creates a 16-bit index buffer that is write-only and managed
 * by D3D8.
 *
 * Index buffers created using this function are \e not managed by RWD3D8, unlike dynamic vertex
 * buffers.
 *
 * \param numIndices Number of indices to create in the buffer.
 * \param indexBuffer Void pointer to the LPDIRECT3DINDEXBUFFER8 structure that is
 * to be the index buffer.
 *
 * \return TRUE if the index buffer was successfully created, FALSE otherwise.
 *
 * \see RwD3D8DynamicVertexBufferCreate
 * \see RwD3D8DynamicVertexBufferDestroy
 *
 */
RwBool
RwD3D8IndexBufferCreate(RwUInt32 numIndices, void **indexBuffer)
{
    HRESULT             hr = D3D_OK;

    RWAPIFUNCTION(RWSTRING("RwD3D8IndexBufferCreate"));
    RWASSERT(indexBuffer);

    hr = DXCHECK(IDirect3DDevice8_CreateIndexBuffer(_RwD3DDevice,
                                                    numIndices *
                                                    sizeof(RwUInt16),
                                                    D3DUSAGE_WRITEONLY,
                                                    D3DFMT_INDEX16,
                                                    D3DPOOL_MANAGED,
                                                    (LPDIRECT3DINDEXBUFFER8
                                                     *) indexBuffer));

    RWRETURN(SUCCEEDED(hr));
}

RwBool
_rwD3D8IndexBuffer32bitsCreate(RwUInt32 numIndices, void **indexBuffer)
{
    HRESULT hr;

    RWFUNCTION(RWSTRING("_rwD3D8IndexBuffer32bitsCreate"));
    RWASSERT(indexBuffer);

    hr = DXCHECK(IDirect3DDevice8_CreateIndexBuffer(_RwD3DDevice,
                                                    numIndices *
                                                    sizeof(RwUInt32),
                                                    D3DUSAGE_WRITEONLY,
                                                    D3DFMT_INDEX32,
                                                    D3DPOOL_MANAGED,
                                                    (LPDIRECT3DINDEXBUFFER8
                                                     *) indexBuffer));

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8CreateVertexShader creates a vertex shader and if created
 * successfully sets that shader as the current shader.
 *
 * A vertex shader is defined by two token arrays that specify the
 * declaration and function of the shader. The token arrays are composed
 * of single or multiple DWORD tokens terminated by a special 0xFFFFFFFF
 * token value.
 *
 * The shader declaration defines the static external interface of the
 * shader, including binding of stream data to vertex register inputs
 * and values loaded into the shader constant memory. The shader function
 * defines the operation of the shader as an array of instructions that
 * are executed in order for each vertex processed during the time the
 * shader is bound to a device. Shaders created without a function array
 * apply the fixed function vertex processing when that shader is current.
 *
 * \param declaration Pointer to the vertex shader declaration token
 * array. This parameter defines the inputs to the shader, including how
 * the vertex elements within the input data streams are used by the shader.
 * \param function Pointer to the vertex shader function token array.
 * This parameter defines the operations to apply to each vertex. If this
 * parameter is set to NULL, a shader is created for the fixed-function
 * pipeline and the parameter declaration indicated by \e declaration is
 * made current and available to be set in a subsequent call to
 * \ref RwD3D8SetVertexShader as long as a handle pointer is provided.
 * \param handle Pointer to the returned vertex shader handle. This value
 * cannot be set as NULL.
 * \param usage Usage controls for the vertex shader. The following flag
 * can be set:
 * \li D3DUSAGE_SOFTWAREPROCESSING - Set to indicate that the vertex shader
 * is to be used with software vertex processing. The
 * D3DUSAGE_SOFTWAREPROCESSING flag must be set for vertex shaders used
 * when the D3DRS_SOFTWAREVERTEXPROCESSING member of the D3DRENDERSTATETYPE
 * enumerated type is TRUE, and removed for vertex shaders used when
 * D3DRS_SOFTWAREVERTEXPROCESSING is FALSE.
 *
 * \return TRUE if the vertex shader was created successfully, FALSE otherwise.
 *
 * \see RwD3D8DeleteVertexShader
 * \see RwD3D8SetVertexShader
 * \see RwD3D8SetVertexShaderConstant
 *
 */
RwBool
RwD3D8CreateVertexShader(const RwUInt32 * declaration,
                         const RwUInt32 * function,
                         RwUInt32 * handle, RwUInt32 usage)
{
    HRESULT             hr = D3D_OK;

    RWAPIFUNCTION(RWSTRING("RwD3D8CreateVertexShader"));

    hr = DXCHECK(IDirect3DDevice8_CreateVertexShader(_RwD3DDevice,
                                                     (const DWORD *)declaration,
                                                     (const DWORD *)function,
                                                     (DWORD *)handle,
                                                     usage));

    if (SUCCEEDED(hr))
    {
        _rxD3D8VertexShader *newVertexShader;

        /* Force to reset any vertex shader again */
        LastVertexShaderUsed = 0xffffffff;

        /* Add it to the list */
        if (VertexShaderFreeList == NULL)
        {
            VertexShaderFreeList = RwFreeListCreate(sizeof(_rxD3D8VertexShader),
                                                    50, 4,
                                                    rwID_DRIVERMODULE |
                                                    rwMEMHINTDUR_EVENT); /* ~1024 Bytes */
        }

        newVertexShader =
                (_rxD3D8VertexShader *)RwFreeListAlloc(VertexShaderFreeList,
                    rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

        newVertexShader->declaration = declaration;
        newVertexShader->function = function;
        newVertexShader->handle = handle;
        newVertexShader->usage = usage;
        newVertexShader->next = VertexShaderList;

        VertexShaderList = newVertexShader;
    }

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DeleteVertexShader deletes the vertex shader referred to by
 * the specified handle and frees up the associated resources.
 *
 * \param handle Vertex shader handle, identifying the vertex shader
 * to be deleted.
 *
 * \see RwD3D8CreateVertexShader
 * \see RwD3D8SetVertexShader
 * \see RwD3D8SetVertexShaderConstant
 *
 */
void
RwD3D8DeleteVertexShader(RwUInt32 handle)
{
    _rxD3D8VertexShader *previous;
    _rxD3D8VertexShader *current;

    RWAPIFUNCTION(RWSTRING("RwD3D8DeleteVertexShader"));

    DXCHECK(IDirect3DDevice8_DeleteVertexShader(_RwD3DDevice, handle));

    /* remove it from the list */
    previous = NULL;
    current = VertexShaderList;
    while (current != NULL)
    {
        if (current->handle != NULL &&
            *(current->handle) == handle)
        {
            break;
        }

        previous = current;
        current = current->next;
    }

    if (current != NULL)
    {
        if (current == VertexShaderList)
        {
            VertexShaderList = VertexShaderList->next;
        }
        else
        {
            RWASSERT(previous != NULL);

            previous->next = current->next;
        }

        RwFreeListFree(VertexShaderFreeList, current);
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetVertexShaderConstant sets values in the vertex constant array.
 *
 * This is the method used to load the constant registers of the vertex shader
 * assembler. When loading transformation matrices, the application should
 * transpose them row/column and load them into consecutive constant registers.
 *
 * \param registerAddress Register address at which to start loading data into
 * the vertex constant array.
 * \param constantData Pointer to the data block holding the values to load
 * into the vertex constant array. The size of the data block is
 * (ConstantCount * 4 * sizeof(float)) bytes.
 * \param constantCount Number of constants to load into the vertex constant
 * array. Each constant is comprised of four floating-point values.
 *
 * \return TRUE if the vertex shader constant was set, FALSE otherwise.
 *
 * \see RwD3D8CreateVertexShader
 * \see RwD3D8DeleteVertexShader
 * \see RwD3D8SetVertexShader
 *
 */
RwBool
RwD3D8SetVertexShaderConstant(RwUInt32 registerAddress,
                              const void *constantData,
                              RwUInt32 constantCount)
{
    HRESULT             hr = D3D_OK;

    RWAPIFUNCTION(RWSTRING("RwD3D8SetVertexShaderConstant"));

    hr = DXCHECK(IDirect3DDevice8_SetVertexShaderConstant(_RwD3DDevice,
                                                          registerAddress,
                                                          constantData,
                                                          constantCount));

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8CreatePixelShader creates a pixel shader.
 *
 * \param function Pointer to the pixel shader function token array,
 * specifying the blending operations. This value cannot be NULL.
 * \param handle Pointer to the returned pixel shader handle.
 *
 * \return TRUE if the pixel shader was created successfully, FALSE otherwise.
 *
 * \see RwD3D8DeletePixelShader
 * \see RwD3D8SetPixelShader
 * \see RwD3D8SetPixelShaderConstant
 *
 */
RwBool
RwD3D8CreatePixelShader(const RwUInt32 * function, RwUInt32 * handle)
{
    HRESULT             hr = D3D_OK;

    RWAPIFUNCTION(RWSTRING("RwD3D8CreatePixelShader"));

    hr = DXCHECK(IDirect3DDevice8_CreatePixelShader(_RwD3DDevice,
                                                    (const DWORD *)function,
                                                    (DWORD *)handle));

    if (SUCCEEDED(hr))
    {
        _rxD3D8PixelShader *newPixelShader;

        /* Add it to the list */
        if (PixelShaderFreeList == NULL)
        {
            PixelShaderFreeList = RwFreeListCreate(sizeof(_rxD3D8PixelShader),
                                                   40, 4,
                                                   rwID_DRIVERMODULE |
                                                   rwMEMHINTDUR_EVENT); /* ~512 Bytes */
        }

        newPixelShader =
                (_rxD3D8PixelShader *)RwFreeListAlloc(PixelShaderFreeList,
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);

        newPixelShader->function = function;
        newPixelShader->handle = handle;
        newPixelShader->next = PixelShaderList;

        PixelShaderList = newPixelShader;
    }

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DeletePixelShader deletes the pixel shader referred
 * to by the specified handle.
 *
 * \param handle Pixel shader handle, identifying the pixel shader
 * to be deleted.
 *
 * \see RwD3D8CreatePixelShader
 * \see RwD3D8SetPixelShader
 * \see RwD3D8SetPixelShaderConstant
 *
 */
void
RwD3D8DeletePixelShader(RwUInt32 handle)
{
    _rxD3D8PixelShader *previous;
    _rxD3D8PixelShader *current;

    RWAPIFUNCTION(RWSTRING("RwD3D8DeletePixelShader"));

    DXCHECK(IDirect3DDevice8_DeletePixelShader(_RwD3DDevice, handle));

    /* remove it from the list */
    previous = NULL;
    current = PixelShaderList;
    while (current != NULL)
    {
        if (current->handle != NULL &&
            *(current->handle) == handle)
        {
            break;
        }

        previous = current;
        current = current->next;
    }

    if (current != NULL)
    {
        if (current == PixelShaderList)
        {
            PixelShaderList = PixelShaderList->next;
        }
        else
        {
            RWASSERT(previous != NULL);

            previous->next = current->next;
        }

        RwFreeListFree(PixelShaderFreeList, current);
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetPixelShaderConstant sets the values in the pixel constant
 * array.
 *
 * This is the method used to load the constant registers of the pixel
 * shader assembler.
 *
 * \param registerAddress Register address at which to start loading data into
 * the pixel constant array.
 * \param constantData Pointer to the data block holding the values to load
 * into the pixel constant array. The size of the data block is
 * (ConstantCount * 4 * sizeof(float)) bytes.
 * \param constantCount Number of constants to load into the pixel constant
 * array. Each constant is comprised of four floating-point values.
 *
 * \return TRUE if the pixel shader constant was set, FALSE otherwise.
 *
 * \see RwD3D8CreatePixelShader
 * \see RwD3D8DeletePixelShader
 * \see RwD3D8SetPixelShader
 *
 */
RwBool
RwD3D8SetPixelShaderConstant(RwUInt32 registerAddress,
                             const void *constantData,
                             RwUInt32 constantCount)
{
    HRESULT             hr = D3D_OK;

    RWAPIFUNCTION(RWSTRING("RwD3D8SetPixelShaderConstant"));

    hr = DXCHECK(IDirect3DDevice8_SetPixelShaderConstant(_RwD3DDevice,
                                                         registerAddress,
                                                         constantData,
                                                         constantCount));

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8GetCaps return a const void pointer to a D3DCAPS8
 * structure for the current active device.
 *
 * Use this function in preference to obtaining the current D3D8
 * device since it will maintain RWD3D8's internal cache.
 *
 * The capabilities of the video card associated with the D3D8
 * device can be queried through the pointer to the structure returned
 * from this function. See the D3D8 API documentation for more
 * information.
 *
 * \return Void pointer to the D3DCAPS8 structure containing the
 * relevant capability information.
 *
 * \see RwD3D8GetCurrentD3DDevice
 *
 */
const void *
RwD3D8GetCaps(void)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8GetCaps"));

    RWRETURN(&_RwD3D8DeviceCaps);
}

/**
 * \ingroup rwcamerad3d8
 * \ref RwD3D8CameraIsSphereFullyInsideFrustum can be used to determine
 * whether a sphere lies entirely within a camera's frustum.
 *
 * This function is similar to \ref RwCameraFrustumTestSphere but is faster
 * since it will only test whether the sphere is \e fully inside the frustum.
 *
 * \param camera Pointer to the camera with the frustum to be tested against.
 * \param sphere Pointer to the sphere that is to be tested.
 *
 * \return Returns TRUE if the sphere is fully inside the frustum, FALSE
 * otherwise.
 *
 * \see RwCameraFrustumTestSphere
 *
 */
RwBool
RwD3D8CameraIsSphereFullyInsideFrustum(const void *camera,
                                       const void *sphere)
{
    const RwCamera     *Camera = (const RwCamera *) camera;
    const RwSphere     *Sphere = (const RwSphere *) sphere;
    const RwFrustumPlane *frustumPlane;
    RwInt32             numPlanes;

    RWAPIFUNCTION(RWSTRING("RwD3D8CameraIsSphereFullyInsideFrustum"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(sphere);

    /* Is it really visible */
    frustumPlane = Camera->frustumPlanes;
    numPlanes = 6;
    do
    {
        RwReal              dot;

        dot = RwV3dDotProductMacro(&Sphere->center,
                                   &frustumPlane->plane.normal);

        dot -= frustumPlane->plane.distance;

        if (dot > (-Sphere->radius))
        {
            /* Crosses the boundary, so not fully inside */
            RWRETURN(FALSE);
        }

        /* Next plane */
        frustumPlane++;
    }
    while (--numPlanes);

    /* Inside all the planes */
    RWRETURN(TRUE);
}

/**
 * \ingroup rwcamerad3d8
 * \ref RwD3D8CameraIsBBoxFullyInsideFrustum can be used to determine
 * whether a bounding box lies entirely within a camera's frustum.
 *
 * \param camera Pointer to the camera with the frustum to be tested against.
 * \param boundingBox Pointer to the bounding box that is to be tested.
 *
 * \return Returns TRUE if the bounding box is fully inside the frustum, FALSE
 * otherwise.
 *
 * \see RwD3D8CameraIsSphereFullyInsideFrustum
 *
 */
RwBool
RwD3D8CameraIsBBoxFullyInsideFrustum(const void *camera,
                                     const void *boundingBox)
{
    const RwCamera     *Camera = (const RwCamera *) camera;
    const RwFrustumPlane *frustumPlane;
    RwInt32             numPlanes;

    RWAPIFUNCTION(RWSTRING("RwD3D8CameraIsBBoxFullyInsideFrustum"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(boundingBox);

    frustumPlane = Camera->frustumPlanes;
    numPlanes = 6;
    do
    {
        RwV3d               vCorner;
        RwReal              distance;

        /* Check against plane */
        vCorner.x =
            ((const RwV3d *) boundingBox)[1 - frustumPlane->closestX].x;
        vCorner.y =
            ((const RwV3d *) boundingBox)[1 - frustumPlane->closestY].y;
        vCorner.z =
            ((const RwV3d *) boundingBox)[1 - frustumPlane->closestZ].z;

        distance =
            RwV3dDotProduct(&vCorner, &frustumPlane->plane.normal);

        /* We only need to detect a boundary case. */
        if (distance > frustumPlane->plane.distance)
        {
            /* Crosses the boundary, so not fully inside */
            RWRETURN(FALSE);

            break;
        }

        /* Next plane */
        frustumPlane++;
    }
    while (--numPlanes);

    /* Inside all the planes */
    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DynamicVertexBufferLock locks an available section of an internally
 * managed dynamic vertex buffer ready for use by an application. The vertex format size
 * and number of vertices required defines the size of the section in the vertex buffer.
 * Pointers to the vertex buffer and the start of the vertex data are returned through
 * \e vertexBufferOut and \e vertexDataOut, respectively. The base index of the vertex
 * data block in the vertex buffer is also returned in \e baseIndexOut.
 *
 * \warning It is advisable \e not to use any D3D8 functions on \e vertexBufferOut since this
 * will interfere with the internal management of the vertex buffers resulting in
 * undefined behavior from RWD3D8.
 *
 * \note The vertex buffer manager running on these internally defined vertex buffers
 * is different from that which manages user-created vertex buffers with
 * \ref RwD3D8DynamicVertexBufferCreate and \ref RwD3D8DynamicVertexBufferDestroy.
 *
 * \param vertexSize Size of the vertex format to use in the vertex buffer, in bytes.
 * \param numVertex Number of vertices of the desired format to use in the vertex buffer.
 * \param vertexBufferOut Void pointer to the locked vertex buffer.
 * \param vertexDataOut Void pointer to the available vertex data block.
 * \param baseIndexOut Base index to the available vertex data block.
 *
 * \return TRUE upon successfully locking a vertex buffer, FALSE otherwise.
 *
 * \see RwD3D8DynamicVertexBufferCreate
 * \see RwD3D8DynamicVertexBufferDestroy
 * \see RwD3D8DynamicVertexBufferUnlock
 *
 */
RwBool
RwD3D8DynamicVertexBufferLock(RwUInt32 vertexSize,
                              RwUInt32 numVertex,
                              void **vertexBufferOut,
                              void **vertexDataOut,
                              RwUInt32 * baseIndexOut)
{
    RwUInt32            size;
    RwUInt32            offsetAligned;
    RwUInt32            nextOffsetAligned;
    HRESULT             hr;

    RWAPIFUNCTION(RWSTRING("RwD3D8DynamicVertexBufferLock"));
    RWASSERT(vertexBufferOut);
    RWASSERT(vertexDataOut);
    RWASSERT(baseIndexOut);

    size = numVertex * vertexSize;

    if (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        offsetAligned =
            ((OffSetDynamicVertexBufferManager[CurrentDynamicVertexBufferManager] +
            (vertexSize - 1)) / vertexSize) *
            vertexSize;

        nextOffsetAligned = (offsetAligned + size);

        if (nextOffsetAligned >
            SizeDynamicVertexBufferManager[CurrentDynamicVertexBufferManager])
        {
            RwUInt32            n;

            /* Find if there is a VB with empty space */
            for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
            {
                if (n != CurrentDynamicVertexBufferManager)
                {
                    offsetAligned =
                        ((OffSetDynamicVertexBufferManager[n] +
                          (vertexSize - 1)) / vertexSize) * vertexSize;

                    nextOffsetAligned = (offsetAligned + size);

                    if (nextOffsetAligned <=
                        SizeDynamicVertexBufferManager[n])
                    {
                        CurrentDynamicVertexBufferManager = n;
                        break;
                    }
                }
            }

            if (n >= MAX_DYNAMIC_VERTEX_BUFFER_MANAGER)
            {
                CurrentDynamicVertexBufferManager++;

                if (CurrentDynamicVertexBufferManager >=
                    MAX_DYNAMIC_VERTEX_BUFFER_MANAGER)
                {
                    CurrentDynamicVertexBufferManager = 0;
                }

                offsetAligned = 0;

                nextOffsetAligned = size;
            }
        }
    }
    else
    {
        offsetAligned = 0;

        nextOffsetAligned = size;
    }

    /* Check if we need a greater vb */
    if (size > SizeDynamicVertexBufferManager[CurrentDynamicVertexBufferManager])
    {
        RwD3D8DynamicVertexBufferDestroy(DynamicVertexBufferManager[CurrentDynamicVertexBufferManager]);

        SizeDynamicVertexBufferManager[CurrentDynamicVertexBufferManager] = size;

        RwD3D8DynamicVertexBufferCreate(0,
                                        size,
                                        (void **)
                                        &(DynamicVertexBufferManager[CurrentDynamicVertexBufferManager]));
    }

    if (offsetAligned == 0)
    {
        hr = IDirect3DVertexBuffer8_Lock(DynamicVertexBufferManager[CurrentDynamicVertexBufferManager],
                                         0, size,
                                         (RwUInt8 **) vertexDataOut,
                                         D3DLOCK_DISCARD |
                                         D3DLOCK_NOSYSLOCK);

        OffSetDynamicVertexBufferManager[CurrentDynamicVertexBufferManager] = size;

        *baseIndexOut = 0;
    }
    else
    {
        hr = IDirect3DVertexBuffer8_Lock(DynamicVertexBufferManager[CurrentDynamicVertexBufferManager],
                                         offsetAligned, size,
                                         (RwUInt8 **) vertexDataOut,
                                         D3DLOCK_NOOVERWRITE |
                                         D3DLOCK_NOSYSLOCK);

        OffSetDynamicVertexBufferManager[CurrentDynamicVertexBufferManager] = nextOffsetAligned;

        *baseIndexOut = offsetAligned / vertexSize;
    }

    *vertexBufferOut =
        DynamicVertexBufferManager[CurrentDynamicVertexBufferManager];

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8DynamicVertexBufferUnlock unlocks a previously locked internally
 * managed vertex buffer that was provided through \ref RwD3D8DynamicVertexBufferLock.
 *
 * \note The vertex buffer manager running on these internally defined vertex buffers
 * is different from that which manages user-created vertex buffers with
 * \ref RwD3D8DynamicVertexBufferCreate and \ref RwD3D8DynamicVertexBufferDestroy.
 *
 * \param vertexBufferIn Void pointer to the vertex buffer to unlock.
 *
 * \return TRUE upon successfully unlocking the vertex buffer, FALSE otherwise.
 *
 * \see RwD3D8DynamicVertexBufferCreate
 * \see RwD3D8DynamicVertexBufferDestroy
 * \see RwD3D8DynamicVertexBufferLock
 *
 */
RwBool
RwD3D8DynamicVertexBufferUnlock(void *vertexBufferIn)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8DynamicVertexBufferUnlock"));

    RWRETURN(SUCCEEDED
             (IDirect3DVertexBuffer8_Unlock
              ((LPDIRECT3DVERTEXBUFFER8) vertexBufferIn)));
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * _rwIntelMMXsupported
 *
 * return TRUE if the CPU supports MMX instructions
 */
RwBool
_rwIntelMMXsupported(void)
{
    RWFUNCTION(RWSTRING("_rwIntelMMXsupported"));

    RWRETURN(_rwD3D8CPUSupportsMMX);
}

/**
 * _rwIntelSSEsupported
 *
 * return TRUE if the CPU supports SSE instructions
 */
RwBool
_rwIntelSSEsupported(void)
{
    RWFUNCTION(RWSTRING("_rwIntelSSEsupported"));

    RWRETURN(_rwD3D8CPUSupportsSSE);
}

/**
 * _rwIntelSSE2supported
 *
 * return TRUE if the CPU supports SSE2 instructions
 */
RwBool
_rwIntelSSE2supported(void)
{
    RWFUNCTION(RWSTRING("_rwIntelSSE2supported"));

    RWRETURN(_rwD3D8CPUSupportsSSE2);
}

/**
 * _rwAMD3DNowSupported
 *
 * return TRUE if the CPU supports 3DNow instructions
 */
RwBool
_rwAMD3DNowSupported(void)
{
    RWFUNCTION(RWSTRING("_rwAMD3DNowSupported"));

    RWRETURN(_rwD3D8CPUSupports3DNow);
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */
