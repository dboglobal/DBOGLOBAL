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

/* D3D9 Libraries */
#include <d3d9.h>

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
#include "d3d9device.h"
#include "d3d9raster.h"
#include "d3d9rendst.h"
#include "d3d92drend.h"
#include "d3d9convrt.h"
#include "d3d9texdic.h"
#include "d3d9vertexbuffer.h"

#include "nodeD3D9SubmitNoLight.h"

#include "windows.h"

//#include <d3d9.h>

#if defined( RWMETRICS )
#include "d3d9metric.h"
#endif /* defined( RWMETRICS ) */

#if !defined( NOASM )
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

#define LOCK_AT_VSYNCx

#define FLOATASINT(f) (*((const RwInt32 *)&(f)))
#define FLOATASDWORD(fvariable) (*((const DWORD *)&(fvariable)))

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
typedef struct _rxD3D9DisplayMode _rxD3D9DisplayMode;
struct _rxD3D9DisplayMode
{
    D3DDISPLAYMODE      mode;
    RwInt32             flags;
};

typedef struct RxD3D9Light RxD3D9Light;
struct RxD3D9Light
{
    D3DLIGHT9   light;
    RwBool      enabled;
};

typedef struct _rxD3D9VertexDeclarationItem _rxD3D9VertexDeclarationItem;
struct _rxD3D9VertexDeclarationItem
{
    LPDIRECT3DVERTEXDECLARATION9 vertexdeclaration;
    RwUInt32 numElements;
    D3DVERTEXELEMENT9 *elements;
};

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Globals (across program)
 */


RwBool						bWindowIconic = FALSE;
RwBool						bChangeVideo = FALSE;

RwRwDeviceGlobals           dgGGlobals; /* Defined here, externed in barwtyp.h */

HWND                        WindowHandle = 0;
RwRGBAReal                  AmbientSaturated = { 0.0f, 0.0f, 0.0f, 1.0f };
D3DPRESENT_PARAMETERS       Present;

LPDIRECT3D9                 _RwDirect3DObject = NULL;
RwUInt32                    _RwD3DAdapterIndex = D3DADAPTER_DEFAULT;
RwUInt32                    _RwD3DAdapterType = D3DDEVTYPE_HAL;
D3DCAPS9                    _RwD3D9DeviceCaps;
LPDIRECT3DDEVICE9           _RwD3DDevice = NULL;
LPSURFACE                   _RwD3D9DepthStencilSurface = NULL;
LPSURFACE                   _RwD3D9RenderSurface = NULL;
RwInt32                     _RwD3D9ZBufferDepth;
_rwD3D9AdapterInformation   _RwD3D9AdapterInformation;

static RwBool       NeedToCopyFromBackBuffer = FALSE;
static RwBool       SystemStarted = FALSE;
static RwBool       _RwHasStencilBuffer = FALSE;
static RwInt32      NumDisplayModes = 0;
static RwInt32      _RwD3D9CurrentModeIndex;
static RwUInt32     StencilClearValue = 0;
static _rxD3D9DisplayMode *DisplayModes = NULL; /* Just used during the selection process */

static RwUInt32     MaxMultisamplingLevels        = 1;
static RwUInt32     SelectedMultisamplingLevels   = 1;

static RwUInt32     MaxMultisamplingLevelsNonMask       = 1;
static RwUInt32     SelectedMultisamplingLevelsNonMask  = 1;

static RwUInt32     FullScreenRefreshRateInHz = 0xffffffff;

static RwBool       EnableMultithreadSafe = FALSE;
static RwBool       EnableSoftwareVertexProcessing = FALSE;
static RwBool       EnableFullScreenDialogBoxMode = FALSE;

static RwBool       InsideScene = FALSE;

static D3DDISPLAYMODE DesktopDisplayMode;

#define MAX_BACKBUFFER_FORMATS  3
static const RwUInt32 BackBufferFormats[MAX_BACKBUFFER_FORMATS] =
{
    D3DFMT_R5G6B5,
    D3DFMT_X8R8G8B8,
    D3DFMT_A2R10G10B10
};

/*
 * Shaders cache
 */
RwUInt32     _rwD3D9LastFVFUsed = 0xffffffff;
void         *_rwD3D9LastVertexDeclarationUsed = (void *)0xffffffff;
void         *_rwD3D9LastVertexShaderUsed = (void *) 0xffffffff;
void         *_rwD3D9LastPixelShaderUsed = (void *) 0xffffffff;
void         *_rwD3D9LastIndexBufferUsed = (void *) 0xffffffff;

#define MAX_STREAMS  4
static RxD3D9VertexStream LastVertexStreamUsed[MAX_STREAMS];

static RwUInt32 NumVertexDeclarations = 0;
static RwUInt32 MaxVertexDeclarations = 0;
static _rxD3D9VertexDeclarationItem *VertexDeclarations = NULL;

/*
 * Matrix cache
 */

RWD3D9_ALIGN16 D3DMATRIX _RwD3D9D3D9ViewTransform;
RWD3D9_ALIGN16 D3DMATRIX _RwD3D9D3D9ProjTransform =
    D3DMatrixInitMacro(1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f);

const D3DMATRIX      *_RwD3D9ActiveViewProjTransform = NULL;

/* Matrix indices can have a value of 0..511,
 * but normally it will fall into 0..256*/
#define MAX_INDEX_MATRIX    260

static RwBool       LastWorldMatrixUsedIdentity = FALSE;
static D3DMATRIX    *LastMatrixUsed[MAX_INDEX_MATRIX] =
    { NULL, NULL, NULL, NULL }; /* to clear the view and projection matrix */
static RwFreeList   *MatrixFreeList = NULL;


static const RWD3D9_ALIGN16 D3DMATRIX IdentityMatrix =
    D3DMatrixInitMacro(1.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f);

/*
 * Lights cache
 */
static RwInt32      MaxNumLights = 0;
static RxD3D9Light  *LightsCache = NULL;

/*
 * Rendertargets cache
 */
#define MAX_RENDERTARGETS 4
static LPSURFACE    CurrentDepthStencilSurface = NULL;
static LPSURFACE    CurrentRenderSurface[MAX_RENDERTARGETS] =
{
    NULL,
    NULL,
    NULL,
    NULL
};

/*
 *  CPU features
 */
RwBool   _rwD3D9CPUSupportsMMX = FALSE;
RwBool   _rwD3D9CPUSupportsSSE = FALSE;
RwBool   _rwD3D9CPUSupportsSSE2 = FALSE;
RwBool   _rwD3D9CPUSupports3DNow = FALSE;

/*
 * Release device callback
 */
static rwD3D9DeviceReleaseCallBack D3D9ReleaseDeviceCallback = NULL;

/*
 * Restore device callback
 */
static rwD3D9DeviceRestoreCallBack D3D9RestoreDeviceCallback = NULL;

/****************************************************************************
 Globals (prototypes)
 */

static __inline RwBool
D3D9MemEqual(const RwUInt32 * buff0,
             const RwUInt32 * buff1, RwUInt32 sizeBytes)
{
    RWFUNCTION(RWSTRING("D3D9MemEqual"));

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

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   'Standard' Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 rwD3D9FindDepth

 On entry   : format

 On exit    : Format depth 16,32 on success else 0 on failure.
 */
static RwInt32
D3D9FindDepth(D3DFORMAT format)
{
    RwInt32             depth;

    RWFUNCTION(RWSTRING("D3D9FindDepth"));

    switch (format)
    {
        case D3DFMT_A2R10G10B10:
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
 rwD3D9FindRWFormat

 On entry   : format

 On exit    : Format depth 16,32 on success else 0 on failure.
 */
static RwInt32
rwD3D9FindRWFormat(D3DFORMAT format)
{
    RwInt32 rwformat;

    RWFUNCTION(RWSTRING("rwD3D9FindRWFormat"));

    switch (format)
    {
        case D3DFMT_A2R10G10B10:
            rwformat = rwRASTERFORMAT8888;
            break;

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
 _rwD3D9SetDepthStencilSurface
 */
void
_rwD3D9SetDepthStencilSurface(LPSURFACE depthStencilSurface)
{
    RWFUNCTION(RWSTRING("_rwD3D9SetDepthStencilSurface"));

    if (CurrentDepthStencilSurface != depthStencilSurface)
    {
        CurrentDepthStencilSurface = depthStencilSurface;
        DXCHECK(IDirect3DDevice9_SetDepthStencilSurface(_RwD3DDevice, depthStencilSurface));
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9SetRenderTarget
 */
RwBool
_rwD3D9SetRenderTarget(RwUInt32 index, LPSURFACE rendertarget)
{
    RWFUNCTION(RWSTRING("_rwD3D9SetRenderTarget"));
    RWASSERT(index < MAX_RENDERTARGETS);

    if (CurrentRenderSurface[index] != rendertarget)
    {
        HRESULT hr;

        CurrentRenderSurface[index] = rendertarget;

        hr = DXCHECK(IDirect3DDevice9_SetRenderTarget(_RwD3DDevice, index, rendertarget));

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D9ClearCacheShaders

 On entry   : none
 */
static void
D3D9ClearCacheShaders(void)
{
    RwUInt32 n;

    RWFUNCTION(RWSTRING("D3D9ClearCacheShaders"));

    _rwD3D9LastFVFUsed = 0xffffffff;
    _rwD3D9LastVertexDeclarationUsed = (void *)0xffffffff;
    _rwD3D9LastVertexShaderUsed = (void *)0xffffffff;
    _rwD3D9LastPixelShaderUsed = (void *)0xffffffff;

    _rwD3D9LastIndexBufferUsed = (void *) 0xffffffff;

    for (n = 0; n < MAX_STREAMS; n++)
    {
        LastVertexStreamUsed[n].vertexBuffer = (void *) 0xffffffff;
        LastVertexStreamUsed[n].offset = 0;
        LastVertexStreamUsed[n].stride = 0;
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ClearCacheMatrix

 On entry   : none

 On exit    : The Matrix pointers are cleared.
 */
static void
D3D9ClearCacheMatrix(void)
{
    RWFUNCTION(RWSTRING("D3D9ClearCacheMatrix"));

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
 rwD3D9ClearCacheLights

 On entry   : none

 On exit    : The Matrix pointers are cleared.
 */
static void
D3D9ClearCacheLights(void)
{
    RWFUNCTION(RWSTRING("D3D9ClearCacheLights"));

    MaxNumLights = 0;

    if (LightsCache != NULL)
    {
        RwFree(LightsCache);
        LightsCache = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ClearCache

 On entry   : none

 */
static void
D3D9ClearCache(void)
{
    RwUInt32 n;

    RWFUNCTION(RWSTRING("D3D9ClearCache"));

    /* Reset rendertargets cache */
    CurrentDepthStencilSurface = NULL;
    for (n = 0; n < MAX_RENDERTARGETS; n++)
    {
        CurrentRenderSurface[n] = NULL;
    }

    /* Reset the shaders cache */
    D3D9ClearCacheShaders();

    /* Reset the matrix cache */
    D3D9ClearCacheMatrix();

    /* Reset the lights cache */
    D3D9ClearCacheLights();

    RWRETURNVOID();
}

/****************************************************************************
 D3D9RestoreCacheLights

 On entry   : none

 On exit    : The Matrix pointers are cleared.
 */
static RwBool
D3D9RestoreCacheLights(void)
{
    HRESULT             hr = D3D_OK;
    RwInt32             i;

    RWFUNCTION(RWSTRING("D3D9RestoreCacheLights"));

#if defined(RWDEBUG)
    if (MaxNumLights)
    {
        RwChar              buffer[256];

        rwsprintf(buffer, "Num Lights Updated: %d", MaxNumLights);

        RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9RestoreCacheLights",
                           buffer);
    }
#endif

    for (i = 0; i < MaxNumLights; i++)
    {
        hr = DXCHECK(IDirect3DDevice9_SetLight
                     (_RwD3DDevice, i, &(LightsCache[i].light)));

        hr = DXCHECK(IDirect3DDevice9_LightEnable
                     (_RwD3DDevice, i, LightsCache[i].enabled));
    }

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 D3D9CalculateMaxMultisamplingLevels

 On entry   :
 On exit    :
 */
static void
D3D9CalculateMaxMultisamplingLevels(void)
{
    BOOL        Windowed;
    RwUInt32    levels;
    HRESULT     hr;

    RWFUNCTION(RWSTRING("D3D9CalculateMaxMultisamplingLevels"));

    MaxMultisamplingLevels              = 1;
    SelectedMultisamplingLevels         = 1;

    MaxMultisamplingLevelsNonMask       = 1;
    SelectedMultisamplingLevelsNonMask  = 1;

    Windowed = ((_RwD3D9AdapterInformation.flags & rwVIDEOMODEEXCLUSIVE) == 0);

    /* Check maskable levels */
    levels = D3DMULTISAMPLE_2_SAMPLES;
    do
    {
        hr = IDirect3D9_CheckDeviceMultiSampleType(_RwDirect3DObject,
                                                   _RwD3DAdapterIndex,
                                                   _RwD3DAdapterType,
                                                   _RwD3D9AdapterInformation.mode.Format,
                                                   Windowed,
                                                   (D3DMULTISAMPLE_TYPE)levels,
                                                   NULL);
        if (SUCCEEDED(hr))
        {
            MaxMultisamplingLevels = levels;
        }

        levels++;
    }
    while(levels <= D3DMULTISAMPLE_16_SAMPLES);

    /* Check non-maskable levels, some old NVidia video cards only support these ones */
    hr = IDirect3D9_CheckDeviceMultiSampleType(_RwDirect3DObject,
                                                _RwD3DAdapterIndex,
                                                _RwD3DAdapterType,
                                                _RwD3D9AdapterInformation.mode.Format,
                                                Windowed,
                                                D3DMULTISAMPLE_NONMASKABLE,
                                                &levels);
    if (SUCCEEDED(hr))
    {
        MaxMultisamplingLevelsNonMask = (levels + 1);

        if (MaxMultisamplingLevelsNonMask > MaxMultisamplingLevels)
        {
            MaxMultisamplingLevels = MaxMultisamplingLevelsNonMask;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D9DeviceSystemOpen

 On entry   :
 On exit    :
 */
static RwBool
D3D9DeviceSystemOpen(void *out __RWUNUSED__,
                     void *inOut, RwInt32 in __RWUNUSED__)
{
    RwUInt32 numAdapters;
    RwInt32  i;

    RWFUNCTION(RWSTRING("D3D9DeviceSystemOpen"));

    /* We're interested in the window handle here, so store it away */
    WindowHandle = (HWND) (((RwEngineOpenParams *) inOut)->displayID);

    /* Create a device */
    _RwDirect3DObject = Direct3DCreate9(D3D_SDK_VERSION);
    if (!_RwDirect3DObject)
    {
        RWRETURN(FALSE);
    }

    /* Name */
    rwstrcpy(_RwD3D9AdapterInformation.name, RWSTRING("D3D9"));

    /* Get video card capabilities */
    numAdapters = IDirect3D9_GetAdapterCount(_RwDirect3DObject);

    _RwD3DAdapterIndex = 0;
    _RwD3DAdapterType = D3DDEVTYPE_HAL;

    while ( _RwD3DAdapterIndex < numAdapters)
    {
        if (SUCCEEDED(IDirect3D9_GetDeviceCaps(_RwDirect3DObject,
                                               _RwD3DAdapterIndex,
                                               _RwD3DAdapterType,
                                               &_RwD3D9DeviceCaps)))
        {
            break;
        }

        _RwD3DAdapterIndex++;
    }

    if (_RwD3DAdapterIndex >= numAdapters)
    {
        #if defined(RWDEBUG)
        RwDebugSendMessage(rwDEBUGERROR, "D3D9DeviceSystemOpen",
                           "Hardware acceleration not supported in this adapter."
                           "Using reference rasterizer.");

        _RwD3DAdapterIndex = 0;
        _RwD3DAdapterType = D3DDEVTYPE_REF;

        IDirect3D9_GetDeviceCaps(_RwDirect3DObject,
                                 _RwD3DAdapterIndex,
                                 _RwD3DAdapterType,
                                 &_RwD3D9DeviceCaps);

        #else
        IDirect3D9_Release(_RwDirect3DObject);

        _RwDirect3DObject = NULL;

        RWRETURN(FALSE);
        #endif
    }

    /* Mode count */
    _RwD3D9AdapterInformation.modeCount = 0;
    for (i = 0; i < MAX_BACKBUFFER_FORMATS; i++)
    {
        _RwD3D9AdapterInformation.modeCount +=
        IDirect3D9_GetAdapterModeCount(_RwDirect3DObject,
                                       _RwD3DAdapterIndex,
                                       BackBufferFormats[i]);
    }

    /* Get the first mode as the default */
    IDirect3D9_GetAdapterDisplayMode(_RwDirect3DObject,
                                     _RwD3DAdapterIndex,
                                     &_RwD3D9AdapterInformation.mode);

    _RwD3D9AdapterInformation.displayDepth =
        D3D9FindDepth(_RwD3D9AdapterInformation.mode.Format);

    _RwD3D9AdapterInformation.flags = 0;

    DesktopDisplayMode = _RwD3D9AdapterInformation.mode;

    /* Choose a default mode, we'll take the first */
    _RwD3D9CurrentModeIndex = 0;

#if (0 && defined(RWDEBUG))
    {
        RwUInt32 format;

        for (format = 0; format < MAX_BACKBUFFER_FORMATS; format++)
        {
            i = IDirect3D9_GetAdapterModeCount(_RwDirect3DObject,
                                       _RwD3DAdapterIndex,
                                       BackBufferFormats[format]);

            while (i--)
            {
                D3DDISPLAYMODE      mode;
                RwChar              buffer[256];

                IDirect3D9_EnumAdapterModes(_RwDirect3DObject,
                                            _RwD3DAdapterIndex,
                                            BackBufferFormats[format],
                                            i, &mode);

                rwsprintf(buffer,
                        "Width: %d Height: %d RefreshRate: %d Format: %d",
                        mode.Width, mode.Height, mode.RefreshRate,
                        mode.Format);

                RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9DeviceSystemOpen",
                                buffer);
            }
        }
    }
#endif /* (0 && defined(RWDEBUG)) */

    /* Calculate max multisampling levels */
    D3D9CalculateMaxMultisamplingLevels();

    /* Detect CPU Info */
    _rwD3D9CPUSupportsMMX = _rwCPUHaveMMX();
    _rwD3D9CPUSupportsSSE = _rwCPUHaveSSE();
    _rwD3D9CPUSupportsSSE2 = _rwCPUHaveSSE2();
    _rwD3D9CPUSupports3DNow = _rwCPUHave3DNow();

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

        rwsprintf(buffer, "   MMX:    %s", (_rwD3D9CPUSupportsMMX ? "Available" : "Not available"));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   SSE:    %s", (_rwD3D9CPUSupportsSSE ? "Available" : "Not available"));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   SSE2:   %s", (_rwD3D9CPUSupportsSSE2 ? "Available" : "Not available"));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   3DNow!: %s\n", (_rwD3D9CPUSupports3DNow ? "Available" : "Not available"));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);
    }
#endif /* defined(RWDEBUG) */

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9DeviceSystemClose

 On entry   :
 On exit    :
 */
static RwBool
D3D9DeviceSystemClose(void *pOut __RWUNUSED__,
                      void *pInOut __RWUNUSED__,
                      RwInt32 nIn __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("D3D9DeviceSystemClose"));

    if (DisplayModes != NULL)
    {
        RwFree(DisplayModes);
        DisplayModes = NULL;

        NumDisplayModes = 0;
    }

    RWASSERT(_RwDirect3DObject);
    if (_RwDirect3DObject)
    {
        IDirect3D9_Release(_RwDirect3DObject);

        _RwDirect3DObject = NULL;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9DeviceSystemFinalizeStart

 On entry   : out (unused)
              inOut (unused)
              in (unused)

 On exit    : TRUE if successful, FALSE otherwise
 */
static RwBool
D3D9DeviceSystemFinalizeStart(void *out __RWUNUSED__,
                              void *inOut __RWUNUSED__,
                              RwInt32 in __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("D3D9DeviceSystemFinalizeStart"));

#if !defined( NOASM )

    /* initialise the FP with single precision mode
     * saving the control register
     */
    _rwProcessorInitialize();

#if !defined( NOSSEASM )
    /* Enable SSE flush-to-zero */
    if (_rwD3D9CPUSupportsSSE)
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

    /* set the matrix multiplication routine to the new assembler code */
    _rwMatrixSetMultFn(_rwIntelx86AsmMatrixMultiply);

#if !defined( NOSSEASM )
    if (_rwD3D9CPUSupportsSSE)
    {
        /* set the matrix-vector products to the new SSE code */
        if (_rwD3D9CPUSupportsSSE2 &&
            _rwD3D9CPUSupports3DNow == FALSE)
        {
            _rwVectorSetMultFn(_rwIntelSSEP4V3dTransformPoint,
                               _rwIntelSSEP4V3dTransformPoints,
                               _rwIntelSSEP4V3dTransformVector,
                               _rwIntelSSEP4V3dTransformVectors);
        }
        else
        {
            _rwVectorSetMultFn(_rwIntelSSEV3dTransformPoint,
                               _rwIntelSSEV3dTransformPoints,
                               _rwIntelSSEV3dTransformVector,
                               _rwIntelSSEV3dTransformVectors);
        }
    }
    else
#endif /* !defined( NOSSEASM ) */
    {
        /* set the matrix-vector products to the new assembler code */
        _rwVectorSetMultFn(_rwIntelx86AsmV3dTransformPoint,
                           _rwIntelx86AsmV3dTransformPoints,
                           _rwIntelx86AsmV3dTransformVector,
                           _rwIntelx86AsmV3dTransformVectors);
    }

#endif /* !defined( NOASM ) */

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D9SetPresentParameters

 On entry   :
 */
static void
D3D9SetPresentParameters(const D3DDISPLAYMODE *mode,
                         RwUInt32 flags,
                         D3DFORMAT adapterFormat)
{
    RWFUNCTION(RWSTRING("D3D9SetPresentParameters"));

    if (flags & rwVIDEOMODEEXCLUSIVE)
    {
        /* On full screen the adapter format is the display format selected */
        adapterFormat = mode->Format;

        /* fullscreen */
        Present.Windowed = FALSE;

        Present.BackBufferCount = 1;

        /* As fast as we can */
        Present.FullScreen_RefreshRateInHz = min(
                                    mode->RefreshRate,
                                    FullScreenRefreshRateInHz);

#ifdef LOCK_AT_VSYNC
        Present.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
#else
        Present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
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

        Present.BackBufferCount = 1;

        Present.FullScreen_RefreshRateInHz = 0;

        /* As fast as we can */
#ifdef LOCK_AT_VSYNC
        Present.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
#else
        Present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif

        /* Check window size */
        GetWindowRect(WindowHandle, &rect);

        if ((rect.right - rect.left) == 0)
        {
            Present.BackBufferWidth = 1;
        }
        else
        {
            Present.BackBufferWidth = 0;
        }

        if ((rect.bottom - rect.top) == 0)
        {
            Present.BackBufferHeight = 1;
        }
        else
        {
            Present.BackBufferHeight = 0;
        }

        Present.SwapEffect = D3DSWAPEFFECT_COPY;
    }

    if (EnableFullScreenDialogBoxMode)
    {
        Present.SwapEffect = D3DSWAPEFFECT_DISCARD;
        Present.BackBufferFormat = adapterFormat;
    }
    else
    {
        /* Try backbuffer with alpha channel if using standard 32 bits format*/
        if (D3DFMT_X8R8G8B8 == adapterFormat)
        {
            if (SUCCEEDED(IDirect3D9_CheckDeviceType(_RwDirect3DObject,
                                                    _RwD3DAdapterIndex,
                                                    _RwD3DAdapterType,
                                                    adapterFormat,
                                                    D3DFMT_A8R8G8B8,
                                                    Present.Windowed)))
            {
                Present.BackBufferFormat = D3DFMT_A8R8G8B8;
            }
            else
            {
                Present.BackBufferFormat = D3DFMT_X8R8G8B8;
            }
        }
        else
        {
            Present.BackBufferFormat = adapterFormat;
        }
    }

    /* TRUE so depth/stencil buffer are managed for us */
    Present.EnableAutoDepthStencil = TRUE;

    /*
     * Find the Z-Buffer depth
     * Best perfomance results when using same bit depth than back buffer
     */
    _RwD3D9ZBufferDepth = D3D9FindDepth(Present.BackBufferFormat);

    if (_RwD3D9ZBufferDepth == 32)
    {
        if (D3D_OK ==
            IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                         _RwD3DAdapterIndex,
                                         _RwD3DAdapterType,
                                         adapterFormat,
                                         D3DUSAGE_DEPTHSTENCIL,
                                         D3DRTYPE_SURFACE, D3DFMT_D24S8)
            && D3D_OK ==
            IDirect3D9_CheckDepthStencilMatch(_RwDirect3DObject,
                                              _RwD3DAdapterIndex,
                                              _RwD3DAdapterType,
                                              adapterFormat,
                                              Present.BackBufferFormat,
                                              D3DFMT_D24S8))
        {
            Present.AutoDepthStencilFormat = D3DFMT_D24S8;

            _RwHasStencilBuffer = TRUE;
        }
        else
        {
            if (D3D_OK ==
                IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                             _RwD3DAdapterIndex,
                                             _RwD3DAdapterType,
                                             adapterFormat,
                                             D3DUSAGE_DEPTHSTENCIL,
                                             D3DRTYPE_SURFACE,
                                             D3DFMT_D24X4S4)
                && D3D_OK ==
                IDirect3D9_CheckDepthStencilMatch(_RwDirect3DObject,
                                                  _RwD3DAdapterIndex,
                                                  _RwD3DAdapterType,
                                                  adapterFormat,
                                                  Present.BackBufferFormat,
                                                  D3DFMT_D24X4S4))
            {
                Present.AutoDepthStencilFormat = D3DFMT_D24X4S4;

                _RwHasStencilBuffer = TRUE;
            }
            else
            {
                /* !!!! we don't have stencil buffer */
                if (D3D_OK ==
                    IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                                 _RwD3DAdapterIndex,
                                                 _RwD3DAdapterType,
                                                 adapterFormat,
                                                 D3DUSAGE_DEPTHSTENCIL,
                                                 D3DRTYPE_SURFACE,
                                                 D3DFMT_D32)
                    && D3D_OK ==
                    IDirect3D9_CheckDepthStencilMatch(_RwDirect3DObject,
                                                      _RwD3DAdapterIndex,
                                                      _RwD3DAdapterType,
                                                      adapterFormat,
                                                      Present.BackBufferFormat,
                                                      D3DFMT_D32))
                {
                    Present.AutoDepthStencilFormat = D3DFMT_D32;

                    _RwHasStencilBuffer = FALSE;
                }
                else
                {
                    if (D3D_OK ==
                        IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                                     _RwD3DAdapterIndex,
                                                     _RwD3DAdapterType,
                                                     adapterFormat,
                                                     D3DUSAGE_DEPTHSTENCIL,
                                                     D3DRTYPE_SURFACE,
                                                     D3DFMT_D24X8)
                        && D3D_OK ==
                        IDirect3D9_CheckDepthStencilMatch
                        (_RwDirect3DObject, _RwD3DAdapterIndex,
                         _RwD3DAdapterType, adapterFormat,
                         Present.BackBufferFormat,
                         D3DFMT_D24X8))
                    {
                        Present.AutoDepthStencilFormat = D3DFMT_D24X8;

                        _RwHasStencilBuffer = FALSE;
                    }
                    else
                    {
                        _RwD3D9ZBufferDepth = 16;
                    }
                }
            }
        }
    }

    if (_RwD3D9ZBufferDepth == 16)
    {
        if (D3D_OK ==
            IDirect3D9_CheckDeviceFormat(_RwDirect3DObject,
                                         _RwD3DAdapterIndex,
                                         _RwD3DAdapterType,
                                         adapterFormat,
                                         D3DUSAGE_DEPTHSTENCIL,
                                         D3DRTYPE_SURFACE,
                                         D3DFMT_D15S1)
            && D3D_OK ==
            IDirect3D9_CheckDepthStencilMatch(_RwDirect3DObject,
                                              _RwD3DAdapterIndex,
                                              _RwD3DAdapterType,
                                              adapterFormat,
                                              Present.BackBufferFormat,
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

    Present.MultiSampleType = D3DMULTISAMPLE_NONE;
    Present.MultiSampleQuality = 0;

    if ( EnableFullScreenDialogBoxMode == FALSE &&
         (SelectedMultisamplingLevels > 1 ||
          SelectedMultisamplingLevelsNonMask > 1) )
    {
        RwUInt32 maxQualityLevels = 0;
        HRESULT hr;

        hr = IDirect3D9_CheckDeviceMultiSampleType(_RwDirect3DObject,
                                                _RwD3DAdapterIndex,
                                                _RwD3DAdapterType,
                                                Present.BackBufferFormat,
                                                Present.Windowed,
                                                (D3DMULTISAMPLE_TYPE)
                                                SelectedMultisamplingLevels,
                                                &maxQualityLevels);

        if (SUCCEEDED(hr) &&
            maxQualityLevels > 0)
        {
            Present.MultiSampleType = ((D3DMULTISAMPLE_TYPE)
                                    SelectedMultisamplingLevels);

            Present.MultiSampleQuality = maxQualityLevels - 1;
        }
        else
        {
            maxQualityLevels = 0;

            /* Check non-mask types */
            hr = IDirect3D9_CheckDeviceMultiSampleType(_RwDirect3DObject,
                                                       _RwD3DAdapterIndex,
                                                       _RwD3DAdapterType,
                                                       Present.BackBufferFormat,
                                                       Present.Windowed,
                                                       D3DMULTISAMPLE_NONMASKABLE,
                                                       &maxQualityLevels);

            if (SUCCEEDED(hr) &&
                maxQualityLevels > 0)
            {
                RwUInt32 qualityLevel;

                qualityLevel = (SelectedMultisamplingLevelsNonMask - 2);

                if (qualityLevel > (maxQualityLevels - 1))
                {
                    qualityLevel = (maxQualityLevels - 1);
                }

                Present.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
                Present.MultiSampleQuality = qualityLevel;
            }
        }

        if (Present.MultiSampleType != D3DMULTISAMPLE_NONE)
        {
            Present.SwapEffect = D3DSWAPEFFECT_DISCARD;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 rwD3D9DeviceSystemStart

 On entry   :
 On exit    :
 */
static RwBool
D3D9DeviceSystemStart(void *out __RWUNUSED__,
                      void *inOut __RWUNUSED__, RwInt32 in __RWUNUSED__)
{
    D3DADAPTER_IDENTIFIER9	adapterInfo;
    D3DDISPLAYMODE			adapterDisplayMode;
    HRESULT					hr;
    RwUInt32				behaviorFlags;
	UINT					AdapterToUse	= D3DADAPTER_DEFAULT;
	D3DDEVTYPE				DeviceType		= D3DDEVTYPE_HAL;
	D3DADAPTER_IDENTIFIER9	Identifier;
	UINT					Adapter;
	FILE* pFile	= NULL;
	RwBool PerfHUD = FALSE;	
	RwInt32 PerfInt = 0;


	RWFUNCTION(RWSTRING("D3D9DeviceSystemStart"));

    IDirect3D9_GetAdapterDisplayMode(_RwDirect3DObject,
                                     _RwD3DAdapterIndex,
                                     &adapterDisplayMode);

    memset(&Present, 0, sizeof(D3DPRESENT_PARAMETERS));

    D3D9SetPresentParameters(&_RwD3D9AdapterInformation.mode,
                             _RwD3D9AdapterInformation.flags,
                             adapterDisplayMode.Format);

    Present.hDeviceWindow = WindowHandle;

    if (EnableFullScreenDialogBoxMode)
    {
        Present.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    }
    else
    {
        Present.Flags = 0;
    }

    IDirect3D9_GetDeviceCaps(_RwDirect3DObject, _RwD3DAdapterIndex,
                             _RwD3DAdapterType, &_RwD3D9DeviceCaps);

    IDirect3D9_GetAdapterIdentifier(_RwDirect3DObject,
                                    _RwD3DAdapterIndex,
                                    0, &adapterInfo);

    /* dump card info */
#if defined(RWDEBUG)
    if (RWSRCGLOBAL(debugFunction))
    {
        RwChar                 *buffer;

        buffer = (RwChar *)RwMalloc((2 * MAX_DEVICE_IDENTIFIER_STRING) *
                                    sizeof(RwChar),
                                    rwID_DRIVERMODULE |
                                    rwMEMHINTDUR_FUNCTION);

        rwsprintf(buffer, "\nD3D9 Device Selected Info: %s",
                  adapterInfo.Description);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE,
                                    buffer);

        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE,
                  "------------------------------------------------------------------------------");

        rwsprintf(buffer, "   VendorID: 0x%08X", adapterInfo.VendorId);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DeviceID: 0x%08X", adapterInfo.DeviceId);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Driver version: %u.%u.%u.%u",
                  HIWORD(adapterInfo.DriverVersion.HighPart),
                  LOWORD(adapterInfo.DriverVersion.HighPart),
                  HIWORD(adapterInfo.DriverVersion.LowPart),
                  LOWORD(adapterInfo.DriverVersion.LowPart));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Windowed: %u", Present.Windowed);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Resolution: %ux%ux%u %uHz",
                  _RwD3D9AdapterInformation.mode.Width,
                  _RwD3D9AdapterInformation.mode.Height,
                  D3D9FindDepth(_RwD3D9AdapterInformation.mode.
                                Format),
                  Present.FullScreen_RefreshRateInHz);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MultiSample Levels: %u", Present.MultiSampleType);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MultiSample Quality: %u", Present.MultiSampleQuality);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   BackBuffer format: %u (%u bits)",
                  Present.BackBufferFormat,
                  _RwD3D9AdapterInformation.displayDepth);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   ZBuffer format: %u (%u bits)",
                  Present.AutoDepthStencilFormat,
                  _RwD3D9ZBufferDepth);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Hardware T&L: %u",
                  ((_RwD3D9DeviceCaps.
                    DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) !=
                   0));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Pure Device: %u",
                  ((_RwD3D9DeviceCaps.
                    DevCaps & D3DDEVCAPS_PUREDEVICE) != 0));
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DeviceType: %u",
                  _RwD3D9DeviceCaps.DeviceType);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   AdapterOrdinal: %u",
                  _RwD3D9DeviceCaps.AdapterOrdinal);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Caps: %X", _RwD3D9DeviceCaps.Caps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Caps2: %X", _RwD3D9DeviceCaps.Caps2);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   Caps3: %X", _RwD3D9DeviceCaps.Caps3);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   PresentationIntervals: %X",
                  _RwD3D9DeviceCaps.PresentationIntervals);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   CursorCaps: %X",
                  _RwD3D9DeviceCaps.CursorCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DevCaps: %X",
                  _RwD3D9DeviceCaps.DevCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   PrimitiveMiscCaps: %X",
                  _RwD3D9DeviceCaps.PrimitiveMiscCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   RasterCaps: %X",
                  _RwD3D9DeviceCaps.RasterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   ZCmpCaps: %X",
                  _RwD3D9DeviceCaps.ZCmpCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   SrcBlendCaps: %X",
                  _RwD3D9DeviceCaps.SrcBlendCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DestBlendCaps: %X",
                  _RwD3D9DeviceCaps.DestBlendCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   AlphaCmpCaps: %X",
                  _RwD3D9DeviceCaps.AlphaCmpCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   ShadeCaps: %X",
                  _RwD3D9DeviceCaps.ShadeCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   TextureCaps: %X",
                  _RwD3D9DeviceCaps.TextureCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   TextureFilterCaps: %X",
                  _RwD3D9DeviceCaps.TextureFilterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   CubeTextureFilterCaps: %X",
                  _RwD3D9DeviceCaps.CubeTextureFilterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VolumeTextureFilterCaps: %X",
                  _RwD3D9DeviceCaps.VolumeTextureFilterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   TextureAddressCaps: %X",
                  _RwD3D9DeviceCaps.TextureAddressCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VolumeTextureAddressCaps: %X",
                  _RwD3D9DeviceCaps.VolumeTextureAddressCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   LineCaps: %X",
                  _RwD3D9DeviceCaps.LineCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureWidth: %u",
                  _RwD3D9DeviceCaps.MaxTextureWidth);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureHeight: %u",
                  _RwD3D9DeviceCaps.MaxTextureHeight);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVolumeExtent: %u",
                  _RwD3D9DeviceCaps.MaxVolumeExtent);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureRepeat: %u",
                  _RwD3D9DeviceCaps.MaxTextureRepeat);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureAspectRatio: %u",
                  _RwD3D9DeviceCaps.MaxTextureAspectRatio);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxAnisotropy: %u",
                  _RwD3D9DeviceCaps.MaxAnisotropy);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexW: %f",
                  _RwD3D9DeviceCaps.MaxVertexW);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   GuardBandLeft: %f",
                  _RwD3D9DeviceCaps.GuardBandLeft);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   GuardBandTop: %f",
                  _RwD3D9DeviceCaps.GuardBandTop);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   GuardBandRight: %f",
                  _RwD3D9DeviceCaps.GuardBandRight);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   GuardBandBottom: %f",
                  _RwD3D9DeviceCaps.GuardBandBottom);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   ExtentsAdjust: %f",
                  _RwD3D9DeviceCaps.ExtentsAdjust);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   StencilCaps: %X",
                  _RwD3D9DeviceCaps.StencilCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   FVFCaps: %X",
                  _RwD3D9DeviceCaps.FVFCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   TextureOpCaps: %X",
                  _RwD3D9DeviceCaps.TextureOpCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxTextureBlendStages: %u",
                  _RwD3D9DeviceCaps.MaxTextureBlendStages);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxSimultaneousTextures: %u",
                  _RwD3D9DeviceCaps.MaxSimultaneousTextures);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VertexProcessingCaps: %X",
                  _RwD3D9DeviceCaps.VertexProcessingCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxActiveLights: %u",
                  _RwD3D9DeviceCaps.MaxActiveLights);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxUserClipPlanes: %u",
                  _RwD3D9DeviceCaps.MaxUserClipPlanes);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexBlendMatrices: %u",
                  _RwD3D9DeviceCaps.MaxVertexBlendMatrices);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexBlendMatrixIndex: %u",
                  _RwD3D9DeviceCaps.MaxVertexBlendMatrixIndex);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxPointSize: %f",
                  _RwD3D9DeviceCaps.MaxPointSize);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxPrimitiveCount: %u",
                  _RwD3D9DeviceCaps.MaxPrimitiveCount);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexIndex: %u",
                  _RwD3D9DeviceCaps.MaxVertexIndex);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxStreams: %u",
                  _RwD3D9DeviceCaps.MaxStreams);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxStreamStride: %u",
                  _RwD3D9DeviceCaps.MaxStreamStride);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VertexShaderVersion: %X",
                  _RwD3D9DeviceCaps.VertexShaderVersion);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexShaderConst: %u",
                  _RwD3D9DeviceCaps.MaxVertexShaderConst);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   PixelShaderVersion: %X",
                  _RwD3D9DeviceCaps.PixelShaderVersion);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   PixelShader1xMaxValue: %f",
                  _RwD3D9DeviceCaps.PixelShader1xMaxValue);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DevCaps2: %X",
                  _RwD3D9DeviceCaps.DevCaps2);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxNpatchTessellationLevel: %f",
                  _RwD3D9DeviceCaps.MaxNpatchTessellationLevel);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);
/*
        rwsprintf(buffer, "   MinAntialiasedLineWidth: %f",
                  _RwD3D9DeviceCaps.MinAntialiasedLineWidth);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxAntialiasedLineWidth: %f",
                  _RwD3D9DeviceCaps.MaxAntialiasedLineWidth);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);
*/
        rwsprintf(buffer, "   MasterAdapterOrdinal: %u",
                  _RwD3D9DeviceCaps.MasterAdapterOrdinal);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   AdapterOrdinalInGroup: %u",
                  _RwD3D9DeviceCaps.AdapterOrdinalInGroup);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   NumberOfAdaptersInGroup: %u",
                  _RwD3D9DeviceCaps.NumberOfAdaptersInGroup);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   DeclTypes: %X",
                  _RwD3D9DeviceCaps.DeclTypes);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   NumSimultaneousRTs: %u",
                  _RwD3D9DeviceCaps.NumSimultaneousRTs);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   StretchRectFilterCaps: %X",
                  _RwD3D9DeviceCaps.StretchRectFilterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VS20Caps: %X",
                  _RwD3D9DeviceCaps.VS20Caps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   PS20Caps: %X",
                  _RwD3D9DeviceCaps.PS20Caps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   VertexTextureFilterCaps: %X",
                  _RwD3D9DeviceCaps.VertexTextureFilterCaps);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVShaderInstructionsExecuted: %u",
                  _RwD3D9DeviceCaps.MaxVShaderInstructionsExecuted);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxPShaderInstructionsExecuted: %u",
                  _RwD3D9DeviceCaps.MaxPShaderInstructionsExecuted);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxVertexShader30InstructionSlots: %u",
                  _RwD3D9DeviceCaps.MaxVertexShader30InstructionSlots);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        rwsprintf(buffer, "   MaxPixelShader30InstructionSlots: %u",
                  _RwD3D9DeviceCaps.MaxPixelShader30InstructionSlots);
        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

        RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, "\n");

        RwFree(buffer);
    }
#endif /* defined(RWDEBUG) */

    /*
     * Check for doggy video cards
     */

    /* ATI */
    if (adapterInfo.VendorId == 0x00001002)
    {
        /* Radeon 7500 */
        if (adapterInfo.DeviceId == 0x00005157)
        {
            /* Remove support for stream offset */
            _RwD3D9DeviceCaps.DevCaps2 &= ~D3DDEVCAPS2_STREAMOFFSET;
        }
    }

    /* Create device */
    behaviorFlags = 0;

    if (EnableMultithreadSafe)
    {
        behaviorFlags |= D3DCREATE_MULTITHREADED;
    }

    if (EnableSoftwareVertexProcessing)
    {
        behaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

        /* Fix caps */
        _RwD3D9DeviceCaps.DevCaps &= ~(D3DDEVCAPS_HWTRANSFORMANDLIGHT);
        _RwD3D9DeviceCaps.DeclTypes = 0;
    }
    else
    {
        if (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        {
            if (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
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

	if(!fopen_s(&pFile, "./_.ph", "rt"))
	{
		fscanf_s(pFile, "%d", &PerfInt);
		if(PerfInt)
		{
			PerfHUD = TRUE;
		}
		fclose(pFile);
	}

	if(PerfHUD)
	{
		for(Adapter = 0; Adapter < IDirect3D9_GetAdapterCount(_RwDirect3DObject); Adapter++)
		{
			IDirect3D9_GetAdapterIdentifier(_RwDirect3DObject, Adapter, 0, &Identifier);

			if(strstr(Identifier.Description, "PerfHUD") != 0)
			{
				AdapterToUse	= Adapter;
				DeviceType		= D3DDEVTYPE_REF;
				break;
			}
		}

		hr = DXCHECK(IDirect3D9_CreateDevice(_RwDirect3DObject,	AdapterToUse, D3DDEVTYPE_REF, WindowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Present, &_RwD3DDevice));

	}
	else
	{
		hr = DXCHECK(IDirect3D9_CreateDevice(_RwDirect3DObject, _RwD3DAdapterIndex,	_RwD3DAdapterType, WindowHandle, behaviorFlags, &Present, &_RwD3DDevice));
	}
/*
	if(!fopen_s(&pFile, "./script/_.ph", "rt"))
	{
		fgets(PERFHUD, 40, pFile);
		if(!strcmp(PERFHUD, "1"))
		{
			PerfHUD = TRUE;
		}
	}

	fclose(pFile);

	if(PerfHUD)
	{
		// Look for 'NVIDIA PerfHUD' adapter If it is present, override default settings
		for(Adapter = 0; Adapter < IDirect3D9_GetAdapterCount(_RwDirect3DObject); Adapter++)
		{
			IDirect3D9_GetAdapterIdentifier(_RwDirect3DObject, Adapter, 0, &Identifier);
			if(strstr(Identifier.Description, "PerfHUD") != 0)
			{
				AdapterToUse	= Adapter;
				DeviceType		= D3DDEVTYPE_REF;
				break;
			}
		}

		hr = DXCHECK(IDirect3D9_CreateDevice(_RwDirect3DObject,	AdapterToUse, D3DDEVTYPE_REF, WindowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Present, &_RwD3DDevice));
		
	}
	else
	{
		hr = DXCHECK(IDirect3D9_CreateDevice(_RwDirect3DObject, _RwD3DAdapterIndex,	_RwD3DAdapterType, WindowHandle, behaviorFlags, &Present, &_RwD3DDevice));
	}
	*/
	
    if (FAILED(hr))
    {
        RWRETURN(FALSE);
    }

    if (EnableFullScreenDialogBoxMode)
    {
        DXCHECK(IDirect3DDevice9_SetDialogBoxMode(_RwD3DDevice, TRUE));
    }

#if defined(RWDEBUG)
    if (EnableMultithreadSafe)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9DeviceSystemStart",
                        "\n   Direct3D Device created as multithread safe.\n"
                        "   This makes Direct3D take its global critical section more frequently, which may degrade performance.\n");
    }

    if (EnableSoftwareVertexProcessing)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9DeviceSystemStart",
                        "\n   Direct3D Device created to use software vertex processing.\n"
                        "   This permits to debug vertex shades but may degrade performance seriously.\n");
    }

    if (EnableFullScreenDialogBoxMode)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9DeviceSystemStart",
                        "\n   Direct3D Device created to support GDI dialog boxes in fullscreen.\n"
                        "   This may degrade performance.\n");
    }
#endif

    /* Get window dimensions */
    if (WindowHandle)
    {
        RECT                rect;

        GetClientRect(WindowHandle, &rect);

        _RwD3D9AdapterInformation.mode.Width = rect.right;
        _RwD3D9AdapterInformation.mode.Height = rect.bottom;
    }

    /*
     * Calling D3DDevice_GetRenderTarget and D3DDevice_GetDepthStencilSurface
     * increase the internal reference count so they need to be Released.
     */

    /* Get the render surface */
    DXCHECK(IDirect3DDevice9_GetRenderTarget(_RwD3DDevice,
                                             0, &_RwD3D9RenderSurface));
    IDirect3DSurface9_Release(_RwD3D9RenderSurface);

    /* Get the depth/stencil surface */
    DXCHECK(IDirect3DDevice9_GetDepthStencilSurface
            (_RwD3DDevice, &_RwD3D9DepthStencilSurface));
    IDirect3DSurface9_Release(_RwD3D9DepthStencilSurface);

    /* Reset the cache */
    D3D9ClearCache();

    /*
     * Open some RenderWare stuff
     */

    _rwD3D9RasterOpen();

    _rwD3D9Im2DRenderOpen();

    _rwD3D9RenderStateOpen();

    _rwD3D9VertexBufferManagerOpen();

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9DeviceSystemStop

 On entry   :
 On exit    :
 */
static RwBool
D3D9DeviceSystemStop(void *out __RWUNUSED__,
                     void *inOut __RWUNUSED__, RwInt32 in __RWUNUSED__)
{
    RwUInt32 n;

    RWFUNCTION(RWSTRING("D3D9DeviceSystemStop"));

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
    if (_rwD3D9CPUSupportsSSE)
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
    for (n = 0; n < RWD3D9_MAX_TEXTURE_STAGES; n++)
    {
        DXCHECK(IDirect3DDevice9_SetTexture(_RwD3DDevice, n, NULL));
    }

    DXCHECK(IDirect3DDevice9_SetIndices(_RwD3DDevice, NULL));

    for (n = 0; n < MAX_STREAMS; n++)
    {
        DXCHECK(IDirect3DDevice9_SetStreamSource(_RwD3DDevice, n, NULL, 0, 0));
    }

    DXCHECK(IDirect3DDevice9_SetPixelShader(_RwD3DDevice, NULL));

    DXCHECK(IDirect3DDevice9_SetVertexDeclaration(_RwD3DDevice, NULL));

    DXCHECK(IDirect3DDevice9_SetVertexShader(_RwD3DDevice, NULL));

    /* Release vertex declarations */
    for (n = 0; n < NumVertexDeclarations; n++)
    {
        if (VertexDeclarations[n].vertexdeclaration != NULL)
        {
            while (IDirect3DVertexDeclaration9_Release(VertexDeclarations[n].vertexdeclaration) > 0);
        }

        if (VertexDeclarations[n].elements != NULL)
        {
            RwFree(VertexDeclarations[n].elements);
        }
    }

    if (VertexDeclarations != NULL)
    {
        RwFree(VertexDeclarations);
        VertexDeclarations = NULL;
    }

    NumVertexDeclarations = 0;
    MaxVertexDeclarations = 0;

    /* Close any remaining surfaces */
    _rwD3D9RasterClose();

    _rwD3D9VertexBufferManagerClose();

    _rwD3D9Im2DRenderClose();

    IDirect3DDevice9_Release(_RwD3DDevice);
    _RwD3DDevice = NULL;

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9DeviceReleaseVideoMemory

 On entry   :
 On exit    :
 */
static RwBool
D3D9DeviceReleaseVideoMemory(void)
{
    RwUInt32            i;

    RWFUNCTION(RWSTRING("D3D9DeviceReleaseVideoMemory"));

#if defined(RWDEBUG)
    RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9DeviceReleaseVideoMemory",
                       "Releasing video memory");
#endif

    D3D9ClearCacheShaders();
    D3D9ClearCacheMatrix();

    for (i = 0; i < RWD3D9_MAX_TEXTURE_STAGES; i++)
    {
        DXCHECK(IDirect3DDevice9_SetTexture(_RwD3DDevice, i, NULL));
    }

    DXCHECK(IDirect3DDevice9_SetIndices(_RwD3DDevice, NULL));

    for (i = 0; i < MAX_STREAMS; i++)
    {
        DXCHECK(IDirect3DDevice9_SetStreamSource(_RwD3DDevice, i, NULL, 0, 0));
    }

    DXCHECK(IDirect3DDevice9_SetPixelShader(_RwD3DDevice, NULL));

    DXCHECK(IDirect3DDevice9_SetVertexDeclaration(_RwD3DDevice, NULL));

    DXCHECK(IDirect3DDevice9_SetVertexShader(_RwD3DDevice, NULL));

    _rwD3D9SetRenderTarget(0, _RwD3D9RenderSurface);
    for (i = 1; i < MAX_RENDERTARGETS; i++)
    {
        _rwD3D9SetRenderTarget(i, NULL);
    }

    _rwD3D9SetDepthStencilSurface(_RwD3D9DepthStencilSurface);

    _rwD3D9Im2DRenderClose();

    _rwD3D9Im3DRenderClose();

    _rwD3D9DynamicVertexBufferRelease();

    _rxD3D9VideoMemoryRasterListRelease();

    RwFreeListPurgeAllFreeLists();

    if (D3D9ReleaseDeviceCallback != NULL)
    {
        D3D9ReleaseDeviceCallback();
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9DeviceRestoreVideoMemory

 On entry   :
 On exit    :
 */
static RwBool
D3D9DeviceRestoreVideoMemory(void)
{
    RwBool              rv;

    RWFUNCTION(RWSTRING("D3D9DeviceRestoreVideoMemory"));

#if defined(RWDEBUG)
    RwDebugSendMessage(rwDEBUGMESSAGE, "D3D9DeviceRestoreVideoMemory",
                       "Restoring video memory");
#endif

    /* Get the render surface */
    DXCHECK(IDirect3DDevice9_GetRenderTarget(_RwD3DDevice,
                                             0, &_RwD3D9RenderSurface));
    IDirect3DSurface9_Release(_RwD3D9RenderSurface);

    /* Get the depth/stencil surface */
    DXCHECK(IDirect3DDevice9_GetDepthStencilSurface
            (_RwD3DDevice, &_RwD3D9DepthStencilSurface));
    IDirect3DSurface9_Release(_RwD3D9DepthStencilSurface);

    rv = _rxD3D9VideoMemoryRasterListRestore();

    rv = rv && _rwD3D9DynamicVertexBufferRestore();

    if (rv)
    {
        _rwD3D9RenderStateReset();
    }

    rv = rv && D3D9RestoreCacheLights();

    rv = rv && _rwD3D9Im2DRenderOpen();

    rv = rv && _rwD3D9Im3DRenderOpen();

    if (D3D9RestoreDeviceCallback != NULL)
    {
        D3D9RestoreDeviceCallback();
    }

    RWRETURN(rv);
}

/****************************************************************************
 rwD3D9NullStandard

 On entry   : pOut
            : pInOut
            : nIn
 On exit    : FALSE
 */
static RwBool
D3D9NullStandard(void *pOut __RWUNUSED__,
                 void *pInOut __RWUNUSED__, RwInt32 nIn __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("D3D9NullStandard"));

    RWRETURN(FALSE);
}

/****************************************************************************
 rwD3D9DeviceSystemStandards

 On entry   :
 On exit    :
 */
static RwBool
D3D9DeviceSystemStandards(void *out, void *inOut __RWUNUSED__,
                          RwInt32 numStandardsFunctions)
{
    RwInt32             i;
    RwInt32             numDriverFunctions;
    RwStandardFunc     *standardFunctions;
    RwStandard          rwD3D9Standards[] = {
        /* Camera ops */
        {rwSTANDARDCAMERABEGINUPDATE, _rwD3D9CameraBeginUpdate},
        {rwSTANDARDCAMERAENDUPDATE, _rwD3D9CameraEndUpdate},
        {rwSTANDARDCAMERACLEAR, _rwD3D9CameraClear},

        /* Raster/Pixel operations */
        {rwSTANDARDRASTERSHOWRASTER, _rwD3D9RasterShowRaster},
        {rwSTANDARDRGBTOPIXEL, _rwD3D9RGBToPixel},
        {rwSTANDARDPIXELTORGB, _rwD3D9PixelToRGB},
        {rwSTANDARDRASTERSETIMAGE, _rwD3D9RasterSetFromImage},
        {rwSTANDARDIMAGEGETRASTER, _rwD3D9ImageGetFromRaster},

        /* Raster creation and destruction */
        {rwSTANDARDRASTERDESTROY, _rwD3D9RasterDestroy},
        {rwSTANDARDRASTERCREATE, _rwD3D9RasterCreate},

        /* Finding about a raster type */
        {rwSTANDARDIMAGEFINDRASTERFORMAT, _rwD3D9ImageFindRasterFormat},

        /* Texture operations */
        {rwSTANDARDTEXTURESETRASTER, _rwD3D9TextureSetRaster},

        /* Locking and releasing */
        {rwSTANDARDRASTERLOCK, _rwD3D9RasterLock},
        {rwSTANDARDRASTERUNLOCK, _rwD3D9RasterUnlock},
        {rwSTANDARDRASTERLOCKPALETTE, _rwD3D9RasterLockPalette},
        {rwSTANDARDRASTERUNLOCKPALETTE, _rwD3D9RasterUnlockPalette},

        /* Raster operations */
        {rwSTANDARDRASTERCLEAR, _rwD3D9RasterClear},
        {rwSTANDARDRASTERCLEARRECT, _rwD3D9RasterClearRect},

        /* !! */
        {rwSTANDARDRASTERRENDER, _rwD3D9RasterRender},
        {rwSTANDARDRASTERRENDERSCALED, _rwD3D9RasterRenderScaled},
        {rwSTANDARDRASTERRENDERFAST, _rwD3D9RasterRenderFast},

        /* Setting the context */
        {rwSTANDARDSETRASTERCONTEXT, _rwD3D9SetRasterContext},

        /* Creating sub rasters */
        {rwSTANDARDRASTERSUBRASTER, _rwD3D9RasterSubRaster},

        /* Hint for rendering order */
        /*{rwSTANDARDHINTRENDERF2B,         _rwD3D9HintRenderFront2Back}, */

        /* Native texture serialization */
        {rwSTANDARDNATIVETEXTUREGETSIZE, _rwD3D9NativeTextureGetSize},
        {rwSTANDARDNATIVETEXTUREWRITE, _rwD3D9NativeTextureWrite},
        {rwSTANDARDNATIVETEXTUREREAD, _rwD3D9NativeTextureRead},

        /* Raster Mip Levels */
        {rwSTANDARDRASTERGETMIPLEVELS, _rwD3D9RasterGetMipLevels}
    };

    RWFUNCTION(RWSTRING("D3D9DeviceSystemStandards"));

    standardFunctions = (RwStandardFunc *) out;
    numDriverFunctions = sizeof(rwD3D9Standards) / sizeof(RwStandard);

    /* Clear out all of the standards initially */
    for (i = 0; i < numStandardsFunctions; i++)
    {
        standardFunctions[i] = D3D9NullStandard;
    }

    /* Fill in all of the standards */
    while (numDriverFunctions--)
    {
        if ((rwD3D9Standards->nStandard < numStandardsFunctions) &&
            (rwD3D9Standards->nStandard >= 0))
        {
            standardFunctions[rwD3D9Standards[numDriverFunctions].
                              nStandard] =
                rwD3D9Standards[numDriverFunctions].fpStandard;
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
D3D9CreateDisplayModesList(void)
{
    RwInt32 i, format, n, maxmodesformat;

    RWFUNCTION(RWSTRING("D3D9CreateDisplayModesList"));

    if (DisplayModes)
    {
        DisplayModes = ( (_rxD3D9DisplayMode *)
                         RwRealloc(DisplayModes,
                                   (1 +
                                    _RwD3D9AdapterInformation.modeCount) *
                                   sizeof(_rxD3D9DisplayMode),
                                   rwID_DRIVERMODULE   |
                                   rwMEMHINTDUR_GLOBAL |
                                   rwMEMHINTFLAG_RESIZABLE) );
    }
    else
    {
        DisplayModes =( (_rxD3D9DisplayMode *)
                        RwMalloc((1 +
                                  _RwD3D9AdapterInformation.modeCount) *
                                 sizeof(_rxD3D9DisplayMode),
                                 rwID_DRIVERMODULE   |
                                 rwMEMHINTDUR_GLOBAL |
                                 rwMEMHINTFLAG_RESIZABLE) );
    }

    IDirect3D9_GetAdapterDisplayMode(_RwDirect3DObject,
                                     _RwD3DAdapterIndex,
                                     &DisplayModes[0].mode);

    if (D3D9FindDepth(DisplayModes[0].mode.Format) != 0)
    {
        DisplayModes[0].flags = 0;

        NumDisplayModes = 1;
    }
    else
    {
        NumDisplayModes = 0;
    }

    for (format = 0; format < MAX_BACKBUFFER_FORMATS; format++)
    {
        maxmodesformat = IDirect3D9_GetAdapterModeCount(_RwDirect3DObject,
                                                        _RwD3DAdapterIndex,
                                                        BackBufferFormats[format]);

        for (i = 0; i < maxmodesformat; i++)
        {
            IDirect3D9_EnumAdapterModes(_RwDirect3DObject,
                                        _RwD3DAdapterIndex,
                                        BackBufferFormats[format],
                                        i,
                                        &DisplayModes[NumDisplayModes].mode);

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
    }

    if (NumDisplayModes < (1 + _RwD3D9AdapterInformation.modeCount))
    {
        DisplayModes = ( (_rxD3D9DisplayMode *)
                         RwRealloc(DisplayModes,
                                   NumDisplayModes *
                                   sizeof(_rxD3D9DisplayMode),
                                   rwID_DRIVERMODULE   |
                                   rwMEMHINTDUR_GLOBAL |
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

    RWRETURN(_rwD3D9RasterPluginAttach());
}

/****************************************************************************
 rwD3D9System

 On entry   : Request
            : Data out
            : Data in/out
            : Number in
 On exit    : TRUE on success else FALSE.
 */
static RwBool
D3D9System(RwInt32 request, void *out, void *inOut, RwInt32 in)
{
    RWFUNCTION(RWSTRING("D3D9System"));

    switch (request)
    {
        case rwDEVICESYSTEMUSEMODE:
            if (!SystemStarted && (in >= 0) && (in < NumDisplayModes))
            {
                _RwD3D9CurrentModeIndex = in;

                _RwD3D9AdapterInformation.mode = DisplayModes[in].mode;

                _RwD3D9AdapterInformation.flags = DisplayModes[in].flags;

                _RwD3D9AdapterInformation.displayDepth =
                                    D3D9FindDepth(DisplayModes[in].mode.Format);

                /* Calculate max multisampling levels */
                D3D9CalculateMaxMultisamplingLevels();

                RWRETURN(TRUE);
            }
            else
            {
                RWRETURN(FALSE);
            }

        case rwDEVICESYSTEMGETNUMMODES:
            if (!DisplayModes)
            {
                D3D9CreateDisplayModesList();
            }

            *((RwInt32 *) out) = NumDisplayModes;
            RWRETURN(TRUE);

        case rwDEVICESYSTEMGETMODEINFO:
            if (!DisplayModes)
            {
                D3D9CreateDisplayModesList();
            }

            if ((in >= 0) && (in < NumDisplayModes))
            {
                RwVideoMode *videoMode = (RwVideoMode *) out;

                videoMode->width = DisplayModes[in].mode.Width;
                videoMode->height = DisplayModes[in].mode.Height;
                videoMode->depth = D3D9FindDepth(DisplayModes[in].mode.Format);
                videoMode->flags = ((RwVideoModeFlag)
                                    DisplayModes[in].flags );
                videoMode->refRate = DisplayModes[in].mode.RefreshRate;
                videoMode->format =
                              rwD3D9FindRWFormat(DisplayModes[in].mode.Format);

                RWRETURN(TRUE);
            }
            else
            {
                RWRETURN(FALSE);
            }

        case rwDEVICESYSTEMGETMODE:
            *((RwInt32 *) out) = _RwD3D9CurrentModeIndex;
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
                RwDevice           *D3D9DriverDevice =
                    _rwDeviceGetHandle();
                RwDevice           *deviceOut = (RwDevice *) out;
                RwMemoryFunctions  *memoryFunctions =
                    (RwMemoryFunctions *) inOut;

                *deviceOut = *D3D9DriverDevice;
                dgGGlobals.memFuncs = memoryFunctions;

                RWRETURN(TRUE);
            }

            /******************* Opening/Closing **********************/

        case rwDEVICESYSTEMOPEN:
            RWRETURN(D3D9DeviceSystemOpen(out, inOut, in));

        case rwDEVICESYSTEMCLOSE:
            RWRETURN(D3D9DeviceSystemClose(out, inOut, in));

            /********* Selecting a rendering sub system ********/

        case rwDEVICESYSTEMGETNUMSUBSYSTEMS:
            {
                RwInt32            *numSubSystems = (RwInt32 *) out;

                *numSubSystems =
                    IDirect3D9_GetAdapterCount(_RwDirect3DObject);

                RWRETURN(TRUE);
            }

        case rwDEVICESYSTEMGETSUBSYSTEMINFO:
            {
                RwInt32             subSystemIndex = (RwInt32) in;
                RwSubSystemInfo    *subSystemInfo =
                    (RwSubSystemInfo *) out;
                D3DADAPTER_IDENTIFIER9 adapterInfo;

                IDirect3D9_GetAdapterIdentifier(_RwDirect3DObject,
                                                subSystemIndex,
                                                0, &adapterInfo);

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
                RwInt32 i;

                _RwD3DAdapterIndex = in;

                /* Mode count */
                _RwD3D9AdapterInformation.modeCount = 0;
                for (i = 0; i < MAX_BACKBUFFER_FORMATS; i++)
                {
                    _RwD3D9AdapterInformation.modeCount +=
                    IDirect3D9_GetAdapterModeCount(_RwDirect3DObject,
                                                   _RwD3DAdapterIndex,
                                                   BackBufferFormats[i]);
                }

                /* Get the current mode as the default */
                IDirect3D9_GetAdapterDisplayMode(_RwDirect3DObject,
                                                 _RwD3DAdapterIndex,
                                                 &_RwD3D9AdapterInformation.
                                                 mode);

                /* Recreate list of modes */
                D3D9CreateDisplayModesList();

                /* Calculate max multisampling levels */
                D3D9CalculateMaxMultisamplingLevels();

                RWRETURN(TRUE);
            }

            /************** Starting stopping ******************/

            /* Start means that this bit of hardware has been selected for
             * rendering - so get it into a state ready for rendering */

        case rwDEVICESYSTEMSTART:
            {
                SystemStarted = D3D9DeviceSystemStart(out, inOut, in);

                RWRETURN(SystemStarted);
            }

        case rwDEVICESYSTEMSTOP:
            RWRETURN(D3D9DeviceSystemStop(out, inOut, in));

            /************* standard device functions ************************/

        case rwDEVICESYSTEMSTANDARDS:
            RWRETURN(D3D9DeviceSystemStandards(out, inOut, in));

        case rwDEVICESYSTEMINITPIPELINE:
            break;

        case rwDEVICESYSTEMGETTEXMEMSIZE:
            {
                *((RwUInt32 *)out) = IDirect3DDevice9_GetAvailableTextureMem(_RwD3DDevice);

                RWRETURN(TRUE);
            }
            break;

        case rwDEVICESYSTEMFINALIZESTART:
            {
                RWRETURN(D3D9DeviceSystemFinalizeStart(out, inOut, in));
            }
            break;

        case rwDEVICESYSTEMINITIATESTOP:
            break;

        case rwDEVICESYSTEMGETMAXTEXTURESIZE:
            {
                RwInt32     *maxTextureSize = (RwInt32 *) out;


                /* return the maximum dimension of a texture */
                if ( _RwD3D9DeviceCaps.MaxTextureWidth >= _RwD3D9DeviceCaps.MaxTextureHeight )
                {
                    *maxTextureSize = _RwD3D9DeviceCaps.MaxTextureWidth;
                }
                else
                {
                    *maxTextureSize = _RwD3D9DeviceCaps.MaxTextureHeight;
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
                *((RwD3D9Metrics **) (out)) = _rwD3D9MetricsGet();

                RWRETURN(TRUE);
            }
            break;
#endif /* defined( RWMETRICS ) */
        case rwDEVICESYSTEMGETID:
            {
                *(RwUInt16*)out = rwDEVICE_D3D9;
                RWRETURN(TRUE);
            }
            break;
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwD3D9CheckError

 On entry   : DX error value, printf debug info if different of D3D_OK

 On exit    : the same error value
 */
#if defined(RWDEBUG)
HRESULT
_rwD3D9CheckError(HRESULT hr,
                  const RwChar * function,
                  const char *file, RwUInt32 line)
{
    RWFUNCTION(RWSTRING("_rwD3D9CheckError"));

    if (FAILED(hr))
    {
        switch (hr)
        {
            case D3DOK_NOAUTOGEN:
                _rwDebugSendMessage(rwDEBUGERROR, file, line, function,
                                    "The autogeneration of mipmaps is not supported for this format. Resource creation will succeed but the mipmap levels will not be automatically generated.");
                break;

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
                                    "An invalid parameter was passed to the returning function.");
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
 _rwD3D9CameraClear

 On entry   : Camera
            : RwRGBA colour to clear to
            : Buffer mask to clear
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CameraClear(void *camera, void *color, RwInt32 clearFlags)
{
	BOOL				bCurrIconic;
	RwBool				bIconicRestore = FALSE;
    RwRaster           *raster;
    RwRaster           *topRaster;
    _rwD3D9RasterExt   *topRasterExt;
    RwRaster           *zRaster;
    RwRaster            *zTopRaster;
    const _rwD3D9RasterExt   *zrasExt;
    D3DCOLOR            packedColor = 0;
    HRESULT             hr = D3D_OK;
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

    RWFUNCTION(RWSTRING("_rwD3D9CameraClear"));

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

    raster = RwCameraGetRaster((RwCamera *)camera);

    /*
     * Got to get the top level raster as this is the only one with a
     * texture surface
     */
    topRaster = RwRasterGetParent(raster);

    topRasterExt = RASTEREXTFROMRASTER(topRaster);

    if (topRasterExt->swapChain)
    {
        D3DVIEWPORT9        viewport;

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
                    RwD3D9CameraAttachWindow(camera, topRasterExt->window);

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
            _rwD3D9SetRenderTarget(0, (LPSURFACE)topRasterExt->texture);

            if (zRaster)
            {
                zTopRaster = RwRasterGetParent(zRaster);

                zrasExt = RASTEREXTFROMCONSTRASTER(zTopRaster);

                _rwD3D9SetDepthStencilSurface((LPSURFACE)zrasExt->texture);

            }
            else
            {
                _rwD3D9SetDepthStencilSurface(NULL);
            }

            /* Initialize the viewport to clear */
            viewport.X = 0;
            viewport.Y = 0;
            viewport.Width = raster->width;
            viewport.Height = raster->height;
            viewport.MinZ = MINZBUFFERVALUE;
            viewport.MaxZ = MAXZBUFFERVALUE;

            /* Set the viewport to clear */
            DXCHECK(IDirect3DDevice9_SetViewport(_RwD3DDevice, &viewport));

            /* Clear */
            DXCHECK(IDirect3DDevice9_Clear
                    (_RwD3DDevice, 0, NULL,
                     clearFlagsConvTable[clearFlags], packedColor,
                     MAXZBUFFERVALUE, StencilClearValue));
        }
    }
    else
    {
        /* Check if the size of the main window has changed */
        GetClientRect(WindowHandle, &rect);

		bCurrIconic = IsIconic(WindowHandle);
		if(!bCurrIconic && bWindowIconic)
			bIconicRestore = TRUE;

        if (!rect.right || !rect.bottom || bCurrIconic)
        {
            //RWRETURN(FALSE);
        }
        else if ((RwUInt32) rect.right != _RwD3D9AdapterInformation.mode.Width
                 || (RwUInt32) rect.bottom != _RwD3D9AdapterInformation.mode.Height ||
				 bIconicRestore)
        {
            D3D9DeviceReleaseVideoMemory();

            hr = IDirect3DDevice9_TestCooperativeLevel(_RwD3DDevice);

            if (Present.Windowed)
            {
                Present.BackBufferWidth = rect.right;
                Present.BackBufferHeight = rect.bottom;
            }

            hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

            if (SUCCEEDED(hr))
            {
                if (!D3D9DeviceRestoreVideoMemory())
                {
                    D3D9DeviceReleaseVideoMemory();

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
                                       _RwD3D9AdapterInformation.mode.Width,
                                       _RwD3D9AdapterInformation.mode.Height);

                    RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D9CameraClear",
                                       buffer);
                    #endif

                    Present.BackBufferWidth = _RwD3D9AdapterInformation.mode.Width;
                    Present.BackBufferHeight = _RwD3D9AdapterInformation.mode.Height;

                    hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

                    if (FAILED(hr))
                    {
                        RWRETURN(FALSE);
                    }

                    /* Change window size */
                    rect.right = rect.left + _RwD3D9AdapterInformation.mode.Width;
                    rect.bottom = rect.top + _RwD3D9AdapterInformation.mode.Height;

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

                    D3D9DeviceRestoreVideoMemory();
                }
                else
                {
                    RWRETURN(FALSE);
                }
            }
            else
            {
                _RwD3D9AdapterInformation.mode.Width = rect.right;
                _RwD3D9AdapterInformation.mode.Height = rect.bottom;
            }
        }

        /* Check raster type */
        if (rwRASTERTYPECAMERATEXTURE == topRaster->cType)
        {
            LPSURFACE           surfaceLevel;
            D3DSURFACE_DESC     d3d9Desc;
            D3DVIEWPORT9        viewport;
            RwUInt32            d3d9ClearFlags;

			if (!IsWindowVisible(WindowHandle))
			{
				topRasterExt->texture = topRasterExt->texture;
			}

            if (topRasterExt->cube)
            {
                IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)topRasterExt->texture,
                                                        (D3DCUBEMAP_FACES)topRasterExt->face,
                                                        0,
                                                        &surfaceLevel);
            }
            else
            {
                IDirect3DTexture9_GetSurfaceLevel(topRasterExt->texture, 0,
                                                  &surfaceLevel);
            }

            IDirect3DSurface9_GetDesc(surfaceLevel, &d3d9Desc);

            /* Set the render & depth/stencil surface */
            if (d3d9Desc.Usage & D3DUSAGE_RENDERTARGET)
            {
                _rwD3D9SetRenderTarget(0, surfaceLevel);

                zRaster = RwCameraGetZRaster((RwCamera *) camera);
                if (zRaster)
                {
                    /*
                    * Got to get the top level raster as this is the only one with a
                    * texture surface
                    */
                    zTopRaster = RwRasterGetParent(zRaster);

                    zrasExt = RASTEREXTFROMCONSTRASTER(zTopRaster);

                    if ( (RwRasterGetType(zTopRaster) & rwRASTERTYPEMASK) == rwRASTERTYPEZBUFFER )
                    {
                        _rwD3D9SetDepthStencilSurface((LPSURFACE)zrasExt->texture);
                    }
                    else
                    {
                        LPSURFACE   surfaceLevelZ;

                        IDirect3DTexture9_GetSurfaceLevel(zrasExt->texture, 0,
                                                          &surfaceLevelZ);

                        _rwD3D9SetDepthStencilSurface(surfaceLevelZ);

                        IDirect3DSurface9_Release(surfaceLevelZ);
                    }
                }
                else
                {
                    _rwD3D9SetDepthStencilSurface(NULL);
                }

                hr = D3D_OK;
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
            DXCHECK(IDirect3DDevice9_SetViewport
                    (_RwD3DDevice, &viewport));

            /* check clear flags */
            d3d9ClearFlags = clearFlagsConvTable[clearFlags];

            if (d3d9ClearFlags & (D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL))
            {
                if (RwCameraGetZRaster((RwCamera *) camera) == NULL)
                {
                    d3d9ClearFlags &= ~(D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL);
                }
            }

            /* Clear */
            DXCHECK(IDirect3DDevice9_Clear
                    (_RwD3DDevice, 0, NULL,
                     d3d9ClearFlags, packedColor,
                     MAXZBUFFERVALUE, StencilClearValue));

            /* If the set render target fails, we need to copy from the back buffer */
            if (FAILED(hr))
            {
                RECT    rectSrc;

                rectSrc.left = raster->nOffsetX;
                rectSrc.top = raster->nOffsetY;
                rectSrc.right = rectSrc.left + raster->width;
                rectSrc.bottom = rectSrc.top + raster->height;

                if (_RwD3D9DeviceCaps.StretchRectFilterCaps & (D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MAGFPOINT))
                {
                    hr = DXCHECK(IDirect3DDevice9_StretchRect
                                (_RwD3DDevice,
                                _RwD3D9RenderSurface, &rectSrc,
                                surfaceLevel, &rectSrc,
                                D3DTEXF_POINT));
                }
                else
                {
                    hr = DXCHECK(IDirect3DDevice9_StretchRect
                                (_RwD3DDevice,
                                _RwD3D9RenderSurface, &rectSrc,
                                surfaceLevel, &rectSrc,
                                D3DTEXF_NONE));
                }
            }

            /* Release the cleared surface */
            IDirect3DSurface9_Release(surfaceLevel);
        }
        else
        {
            D3DVIEWPORT9        viewport;
            RwUInt32            d3d9ClearFlags = 0;

            /* Set the render & depth/stencil surface */
            _rwD3D9SetRenderTarget(0, _RwD3D9RenderSurface);
            _rwD3D9SetDepthStencilSurface(_RwD3D9DepthStencilSurface);

            /* Initialize the viewport to clear */
            viewport.X = raster->nOffsetX;
            viewport.Y = raster->nOffsetY;
            viewport.Width = raster->width;
            viewport.Height = raster->height;
            viewport.MinZ = MINZBUFFERVALUE;
            viewport.MaxZ = MAXZBUFFERVALUE;

            /* Set the viewport to clear */
            DXCHECK(IDirect3DDevice9_SetViewport
                    (_RwD3DDevice, &viewport));

            /* Clear */
            if (rwCAMERACLEARIMAGE & clearFlags)
            {
                d3d9ClearFlags |= D3DCLEAR_TARGET;
            }

            if (rwCAMERACLEARZ & clearFlags)
            {
                d3d9ClearFlags |= D3DCLEAR_ZBUFFER;

                if (_RwHasStencilBuffer)
                {
                    d3d9ClearFlags |= D3DCLEAR_STENCIL;
                }
            }
            else if (rwCAMERACLEARSTENCIL & clearFlags)
            {
                if (_RwHasStencilBuffer)
                {
                    d3d9ClearFlags |= D3DCLEAR_STENCIL;
                }
            }

            DXCHECK(IDirect3DDevice9_Clear
                    (_RwD3DDevice, 0, NULL, d3d9ClearFlags, packedColor,
                     MAXZBUFFERVALUE, StencilClearValue));
        }
		bWindowIconic = bCurrIconic;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9TestState

 On exit    : TRUE on success
 */
RwBool
_rwD3D9TestState(void)
{
    HRESULT hr;

    RWFUNCTION(RWSTRING("_rwD3D9TestState"));

    hr = IDirect3DDevice9_TestCooperativeLevel(_RwD3DDevice);

    if (hr == D3DERR_DEVICENOTRESET)
    {
#if defined(RWDEBUG)
        RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D9TestState", "DEVICE LOST");
#endif

        D3D9DeviceReleaseVideoMemory();

        hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

        if (hr == D3DERR_INVALIDCALL)
        {
            /* Check to see if the desktop has changed its depth */
            if (Present.Windowed)
            {
                D3DDISPLAYMODE      adapterDisplayMode;

                IDirect3D9_GetAdapterDisplayMode(_RwDirect3DObject,
                                                 _RwD3DAdapterIndex,
                                                 &adapterDisplayMode);

                Present.BackBufferFormat =
                _RwD3D9AdapterInformation.mode.Format =
                                                    adapterDisplayMode.Format;

                _RwD3D9AdapterInformation.displayDepth =
                            D3D9FindDepth(_RwD3D9AdapterInformation.mode.Format);

                hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));
            }
            else
            {
                /* Ops!!! really bad situation */
            }
        }

        if (SUCCEEDED(hr))
        {
            D3D9DeviceRestoreVideoMemory();
        }
    }
        

    RWRETURN(SUCCEEDED(hr));
}

/****************************************************************************
 _rwD3D9BeginScene

 On exit    : TRUE on success
 */
RwBool
_rwD3D9BeginScene(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9BeginScene"));

    if (_rwD3D9TestState())
    {
        if (!InsideScene)
        {
            HRESULT hr;

            hr = DXCHECK(IDirect3DDevice9_BeginScene(_RwD3DDevice));

            if (SUCCEEDED(hr))
            {
                InsideScene = TRUE;
            }
        }
    }
    else
    {
        InsideScene = FALSE;
    }

    RWRETURN(InsideScene);
}


/**
 * \ingroup rwengined3d9
 * \ref RwD3D9EngineGetMaxMultiSamplingLevels returns the maximum available
 * levels of multisampling for the current subsystem and video mode.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * \return Returns maximum levels of multisampling for the current
 * subsystem and video mode. A value of \e 1 means that the current
 * subsystem and video mode doesn't support multisampling.
 *
 * \see RwD3D9EngineSetMultiSamplingLevels
 * \see RwEngineOpen
 * \see RwEngineSetSubSystem
 * \see RwEngineSetVideoMode
 * \see RwEngineStart
 *
 */
RwUInt32
RwD3D9EngineGetMaxMultiSamplingLevels(void)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9EngineGetMaxMultiSamplingLevels"));

    RWRETURN(MaxMultisamplingLevels);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9EngineSetMultiSamplingLevels sets the number of multisampling levels
 * for the current subsystem and video mode.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * If the current subsystem and video mode doesn't support the specified number of
 * multisampling levels, RWD3D9 will choose the highest number of supported multisampling
 * levels \e below that which was specified.
 *
 * Note that changing the current subsystem or the current video mode will reset
 * the number of multisampling levels to 1.
 *
 * \param numLevels Number of multisampling levels.
 * A value of \e 1 means that multisampling is disabled.
 *
 * \see RwD3D9EngineGetMaxMultiSamplingLevels
 * \see RwEngineOpen
 * \see RwEngineSetSubSystem
 * \see RwEngineSetVideoMode
 * \see RwEngineStart
 *
 */
void
RwD3D9EngineSetMultiSamplingLevels(RwUInt32 numLevels)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9EngineSetMultiSamplingLevels"));

    if (numLevels > MaxMultisamplingLevels)
    {
        numLevels = MaxMultisamplingLevels;
    }

    SelectedMultisamplingLevels = numLevels;
    if (SelectedMultisamplingLevels < D3DMULTISAMPLE_2_SAMPLES)
    {
        SelectedMultisamplingLevels = 1;
        SelectedMultisamplingLevelsNonMask = 1;
    }
    else
    {
        BOOL        Windowed;
        HRESULT     hr;

        Windowed = ((_RwD3D9AdapterInformation.flags & rwVIDEOMODEEXCLUSIVE) == 0);

        /* Check maskable levels */
        do
        {
            hr = IDirect3D9_CheckDeviceMultiSampleType(_RwDirect3DObject,
                                                       _RwD3DAdapterIndex,
                                                       _RwD3DAdapterType,
                                                       _RwD3D9AdapterInformation.mode.Format,
                                                       Windowed,
                                                       (D3DMULTISAMPLE_TYPE)SelectedMultisamplingLevels,
                                                       NULL);

            if (SUCCEEDED(hr))
            {
                break;
            }

            SelectedMultisamplingLevels--;
        }
        while (SelectedMultisamplingLevels >= D3DMULTISAMPLE_2_SAMPLES);

        /* Check non-maskable levels */
        SelectedMultisamplingLevelsNonMask = (numLevels * MaxMultisamplingLevelsNonMask) / MaxMultisamplingLevels;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9EngineSetRefreshRate sets the rate in Hertz at which the display adapter
 * refreshes the screen.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * If the current subsystem and video mode doesn't support the specified
 * refresh rate, RWD3D9 will choose the highest supported refresh rate \e below
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
 */
void
RwD3D9EngineSetRefreshRate(RwUInt32 refreshRate)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9EngineSetRefreshRate"));

    FullScreenRefreshRateInHz = refreshRate;

    if (FullScreenRefreshRateInHz)
    {
        RwUInt32        nCloserMatch;
        D3DDISPLAYMODE  mode;
        RwInt32         format, n;

        nCloserMatch = 0;
        for (format = 0; format < MAX_BACKBUFFER_FORMATS; format++)
        {
            n = IDirect3D9_GetAdapterModeCount(_RwDirect3DObject,
                                            _RwD3DAdapterIndex,
                                            BackBufferFormats[format]);

            while (n--)
            {
                IDirect3D9_EnumAdapterModes(_RwDirect3DObject,
                                            _RwD3DAdapterIndex,
                                            BackBufferFormats[format],
                                            n, &mode);

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

            RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D9EngineSetRefreshRate",
                   buffer);
            #endif

            FullScreenRefreshRateInHz = nCloserMatch;
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9EngineSetMultiThreadSafe indicates that the application requests Direct3D
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
RwD3D9EngineSetMultiThreadSafe(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9EngineSetMultiThreadSafe"));

    EnableMultithreadSafe = enable;

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9EngineSetSoftwareVertexProcessing indicates that the application requests Direct3D
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
RwD3D9EngineSetSoftwareVertexProcessing(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9EngineSetSoftwareVertexProcessing"));

    EnableSoftwareVertexProcessing = enable;

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9EngineSetFullScreenDialogBoxMode indicates that the application requests Direct3D
 * to support GDI dialog boxes when running in fullscreen.
 *
 * Call this function \e after \ref RwEngineOpen and \e before \ref RwEngineStart.
 *
 * \param enable TRUE if the Direct3D device should be created to support GDI dialog boxes,
 * FALSE otherwise.
 *
 * \see RwEngineOpen
 * \see RwEngineStart
 *
 */
void
RwD3D9EngineSetFullScreenDialogBoxMode(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9EngineSetFullScreenDialogBoxMode"));

    if (EnableFullScreenDialogBoxMode != enable)
    {
        EnableFullScreenDialogBoxMode = enable;

        if (SystemStarted)
        {
            IDirect3DDevice9_SetDialogBoxMode(_RwD3DDevice, enable);
        }
        else
        {
            /* Disable multisampling */
            MaxMultisamplingLevels        = 1;
            SelectedMultisamplingLevels   = 1;

            MaxMultisamplingLevelsNonMask       = 1;
            SelectedMultisamplingLevelsNonMask  = 1;
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9ChangeVideoMode is used to force an immediate change of video mode.
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
RwD3D9ChangeVideoMode(RwInt32 modeIndex)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9ChangeVideoMode"));
    RWASSERT(SystemStarted != FALSE);
    RWASSERT(InsideScene == FALSE);

	bChangeVideo = TRUE;

    if (SystemStarted &&
        (modeIndex >= 0) &&
        (modeIndex < NumDisplayModes))
    {
        if (_RwD3D9CurrentModeIndex != modeIndex)
        {
            D3DPRESENT_PARAMETERS   oldPresent;
            HRESULT hr;

            oldPresent = Present;

            D3D9DeviceReleaseVideoMemory();

            hr = IDirect3DDevice9_TestCooperativeLevel(_RwD3DDevice);

            D3D9SetPresentParameters(&DisplayModes[modeIndex].mode,
                                     DisplayModes[modeIndex].flags,
                                     DesktopDisplayMode.Format);

            hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

            if (SUCCEEDED(hr))
            {
                if (D3D9DeviceRestoreVideoMemory() == FALSE)
                {
                    D3D9DeviceReleaseVideoMemory();

                    hr = E_FAIL;
                }
            }

            if (FAILED(hr))
            {
                #if defined(RWDEBUG)
                RwChar              buffer[256];

                rwsprintf(buffer, "Not enough memory to resize video mode to %dx%d."
                                  " Switching back to %dx%d",
                                    DisplayModes[modeIndex].mode.Width,
                                    DisplayModes[modeIndex].mode.Height,
                                    _RwD3D9AdapterInformation.mode.Width,
                                    _RwD3D9AdapterInformation.mode.Height);

                RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D9ChangeVideoMode",
                                    buffer);
                #endif

                Present = oldPresent;

                hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

                if (FAILED(hr))
                {
                    RWRETURN(FALSE);
                }

                D3D9DeviceRestoreVideoMemory();
            }
            else
            {
                RECT rect;

                _RwD3D9CurrentModeIndex = modeIndex;

                _RwD3D9AdapterInformation.mode = DisplayModes[modeIndex].mode;

                _RwD3D9AdapterInformation.flags = DisplayModes[modeIndex].flags;

                _RwD3D9AdapterInformation.displayDepth =
                                    D3D9FindDepth(DisplayModes[modeIndex].mode.Format);

                /* Change window size */
                if (Present.Windowed == FALSE)
                {
                    rect.left = 0;
                    rect.top = 0;
                    rect.right = rect.left + _RwD3D9AdapterInformation.mode.Width;
                    rect.bottom = rect.top + _RwD3D9AdapterInformation.mode.Height;

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

                    _RwD3D9AdapterInformation.mode.Width = rect.right;
                    _RwD3D9AdapterInformation.mode.Height = rect.bottom;
                }
            }
        }
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9ChangeMultiSamplingLevels is used to force an immediate change of the
 * multisampling levels.
 *
 * This function should be called before any graphics are rendered or
 * immediately after \ref RwRasterShowRaster or \ref RwCameraShowRaster and
 * before the next frame is submitted.
 *
 * \note This function can not be called before \ref RwEngineStart.
 *
 * \param numLevels Number of multisampling levels.
 * A value of \e 1 means that multisampling is disabled.
 *
 * \return Returns TRUE if successful, FALSE otherwise.
 *
 * \see RwD3D9EngineGetMaxMultiSamplingLevels
 */
RwBool
RwD3D9ChangeMultiSamplingLevels(RwUInt32 numLevels)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9ChangeMultiSamplingLevels"));
    RWASSERT(SystemStarted != FALSE);
    RWASSERT(InsideScene == FALSE);

    if (SystemStarted &&
        SelectedMultisamplingLevels != numLevels)
    {
        D3DPRESENT_PARAMETERS   oldPresent;
        RwUInt32 oldSelectedMultisamplingLevels;
        RwUInt32 oldSelectedMultisamplingLevelsNonMask;
        HRESULT hr;

        oldPresent = Present;
        oldSelectedMultisamplingLevels = SelectedMultisamplingLevels;
        oldSelectedMultisamplingLevelsNonMask = SelectedMultisamplingLevelsNonMask;

        RwD3D9EngineSetMultiSamplingLevels(numLevels);

        D3D9DeviceReleaseVideoMemory();

        hr = IDirect3DDevice9_TestCooperativeLevel(_RwD3DDevice);

        D3D9SetPresentParameters(&_RwD3D9AdapterInformation.mode,
                                 _RwD3D9AdapterInformation.flags,
                                 DesktopDisplayMode.Format);

        hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

        if (SUCCEEDED(hr))
        {
            if (D3D9DeviceRestoreVideoMemory() == FALSE)
            {
                D3D9DeviceReleaseVideoMemory();

                hr = E_FAIL;
            }
        }

        if (FAILED(hr))
        {
            #if defined(RWDEBUG)
            RwChar              buffer[256];

            rwsprintf(buffer, "%d multisampling levels not supported."
                              " Switching back to %d multisampling levels",
                                SelectedMultisamplingLevels,
                                oldSelectedMultisamplingLevels);

            RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D9ChangeMultiSamplingLevels",
                                buffer);
            #endif

            Present = oldPresent;
            SelectedMultisamplingLevels = oldSelectedMultisamplingLevels;
            SelectedMultisamplingLevelsNonMask = oldSelectedMultisamplingLevelsNonMask;

            hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

            if (FAILED(hr))
            {
                RWRETURN(FALSE);
            }

            D3D9DeviceRestoreVideoMemory();
        }
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwcamerad3d9
 * \ref RwD3D9CameraAttachWindow can be used to attach an \ref RwCamera to
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
RwD3D9CameraAttachWindow(void *camera, void *hwnd)
{
    RwBool              rv = FALSE;

    RWAPIFUNCTION(RWSTRING("RwD3D9CameraAttachWindow"));
    RWASSERT(camera);

    if (Present.Windowed)
    {
        if (WindowHandle != (HWND)hwnd)
        {
            RwRaster           *raster;
            RwRaster           *zRaster;
            D3DPRESENT_PARAMETERS present;
            LPDIRECT3DSWAPCHAIN9 swapChain;
            RECT rect;

            GetWindowRect((HWND)hwnd, &rect);

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

            present.hDeviceWindow = (HWND)hwnd;

            if ((rect.right - rect.left) == 0)
            {
                present.BackBufferWidth = 1;
            }
            else
            {
                present.BackBufferWidth = 0;
            }

            if ((rect.bottom - rect.top) == 0)
            {
                present.BackBufferHeight = 1;
            }
            else
            {
                present.BackBufferHeight = 0;
            }

            present.EnableAutoDepthStencil = FALSE;

            if (D3D_OK ==
                DXCHECK(IDirect3DDevice9_CreateAdditionalSwapChain
                        (_RwD3DDevice, &present, &swapChain)))
            {
                LPSURFACE           surfaceLevel;
                D3DSURFACE_DESC     d3dDesc;
                _rwD3D9RasterExt   *rasterExt;

                IDirect3DSwapChain9_GetBackBuffer(swapChain, 0,
                                                  D3DBACKBUFFER_TYPE_MONO,
                                                  &surfaceLevel);

                IDirect3DSurface9_GetDesc(surfaceLevel, &d3dDesc);

                raster =
                    RwRasterCreate(d3dDesc.Width, d3dDesc.Height, 0,
                                   rwRASTERTYPECAMERA |
                                   rwRASTERDONTALLOCATE);
                if (raster)
                {
                    raster->width = d3dDesc.Width;
                    raster->height = d3dDesc.Height;
                    raster->depth = _rwD3D9PixelFormatInfo[d3dDesc.Format].depth;
                    raster->cFormat = (_rwD3D9PixelFormatInfo[d3dDesc.Format].rwFormat >> 8);

                    rasterExt = RASTEREXTFROMRASTER(raster);

                    rasterExt->texture = (LPDIRECT3DTEXTURE9) surfaceLevel;
                    rasterExt->alpha = _rwD3D9PixelFormatInfo[d3dDesc.Format].alpha;
                    rasterExt->d3dFormat = d3dDesc.Format;
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

                IDirect3DSurface9_Release(surfaceLevel);

                rv = TRUE;
            }
        }
#if defined(RWDEBUG)
        else
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "RwD3D9CameraAttachWindow",
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
 _rwD3D9CameraBeginUpdate

 Start update to camera

 On entry   : NULL
            : pCamera - camera to start update to
            : 0
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CameraBeginUpdate(void *out __RWUNUSED__,
                         void *cameraIn, RwInt32 in __RWUNUSED__)
{
	BOOL				bCurrIconic;
	RwBool				bIconicRestore = FALSE;
    RwCamera            *camera;
    const RwMatrix      *camLTM;
    RwRaster            *raster;
    RwRaster           *topRaster;
    _rwD3D9RasterExt   *topRasterExt;
    RwRaster           *zRaster;
    RwRaster           *zTopRaster;
    const _rwD3D9RasterExt   *zRasterExt;
    D3DVIEWPORT9        viewport;
    HRESULT             hr;
    RECT                rect;

    RWFUNCTION(RWSTRING("_rwD3D9CameraBeginUpdate"));

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

    RwMatrixInvert((RwMatrix *)&_RwD3D9D3D9ViewTransform, camLTM);

    _RwD3D9D3D9ViewTransform.m[0][0] = -_RwD3D9D3D9ViewTransform.m[0][0];
    _RwD3D9D3D9ViewTransform.m[0][3] = 0.0f;

    _RwD3D9D3D9ViewTransform.m[1][0] = -_RwD3D9D3D9ViewTransform.m[1][0];
    _RwD3D9D3D9ViewTransform.m[1][3] = 0.0f;

    _RwD3D9D3D9ViewTransform.m[2][0] = -_RwD3D9D3D9ViewTransform.m[2][0];
    _RwD3D9D3D9ViewTransform.m[2][3] = 0.0f;

    _RwD3D9D3D9ViewTransform.m[3][0] = -_RwD3D9D3D9ViewTransform.m[3][0];
    _RwD3D9D3D9ViewTransform.m[3][3] = 1.0f;

    RwD3D9SetTransform(D3DTS_VIEW, &_RwD3D9D3D9ViewTransform);

    /*
     * Projection matrix
     */
    _RwD3D9D3D9ProjTransform.m[0][0] = camera->recipViewWindow.x;
    _RwD3D9D3D9ProjTransform.m[1][1] = camera->recipViewWindow.y;

    /* Shear X, Y by view offset with Z invariant */
    _RwD3D9D3D9ProjTransform.m[2][0] =
        camera->recipViewWindow.x * camera->viewOffset.x;
    _RwD3D9D3D9ProjTransform.m[2][1] =
        camera->recipViewWindow.y * camera->viewOffset.y;

    /* Translate to shear origin */
    _RwD3D9D3D9ProjTransform.m[3][0] =
        -camera->recipViewWindow.x * camera->viewOffset.x;

    _RwD3D9D3D9ProjTransform.m[3][1] =
        -camera->recipViewWindow.y * camera->viewOffset.y;

    /* Projection type */
    if (camera->projectionType == rwPARALLEL)
    {
        _RwD3D9D3D9ProjTransform.m[2][2] =
            1.0f / (camera->farPlane - camera->nearPlane);
        _RwD3D9D3D9ProjTransform.m[2][3] = 0.0f;
        _RwD3D9D3D9ProjTransform.m[3][3] = 1.0f;
    }
    else
    {
        _RwD3D9D3D9ProjTransform.m[2][2] =
            camera->farPlane / (camera->farPlane - camera->nearPlane);
        _RwD3D9D3D9ProjTransform.m[2][3] = 1.0f;
        _RwD3D9D3D9ProjTransform.m[3][3] = 0.0f;
    }

    _RwD3D9D3D9ProjTransform.m[3][2] = -_RwD3D9D3D9ProjTransform.m[2][2] * camera->nearPlane;

    RwD3D9SetTransform(D3DTS_PROJECTION, &_RwD3D9D3D9ProjTransform);

    /*
     * Prepare Composed matrix useful for vertex shaders
     */
    _RwD3D9ActiveViewProjTransform = NULL;

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
                    RwD3D9CameraAttachWindow(camera, topRasterExt->window);

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
                        (void)RwCameraSetZRaster(camera, NULL);

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

            _rwD3D9SetRenderTarget(0, (LPSURFACE)topRasterExt->texture);

            if (zRaster)
            {
                zTopRaster = RwRasterGetParent(zRaster);

                zRasterExt = RASTEREXTFROMCONSTRASTER(zTopRaster);

                _rwD3D9SetDepthStencilSurface((LPSURFACE)zRasterExt->texture);
            }
            else
            {
                _rwD3D9SetDepthStencilSurface(NULL);
            }
        }
    }
    else
    {
        /* Check if the main window has changed its size */
        GetClientRect(WindowHandle, &rect);

		bCurrIconic = IsIconic(WindowHandle);
		if(!bCurrIconic && bWindowIconic)
			bIconicRestore = TRUE;

		bWindowIconic = bCurrIconic;

        if (!rect.right || !rect.bottom || bCurrIconic)
        {
   //         RWRETURN(FALSE);
        }
        else if ((RwUInt32) rect.right != _RwD3D9AdapterInformation.mode.Width
                 || (RwUInt32) rect.bottom != _RwD3D9AdapterInformation.mode.Height
				 || bIconicRestore || bChangeVideo)
        {
			bChangeVideo = FALSE;

            D3D9DeviceReleaseVideoMemory();

            hr = IDirect3DDevice9_TestCooperativeLevel(_RwD3DDevice);

            if (Present.Windowed)
            {
                Present.BackBufferWidth = rect.right;
                Present.BackBufferHeight = rect.bottom;
            }

            hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

            if (SUCCEEDED(hr))
            {
                if (!D3D9DeviceRestoreVideoMemory())
                {
                    D3D9DeviceReleaseVideoMemory();

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
                                       _RwD3D9AdapterInformation.mode.Width,
                                       _RwD3D9AdapterInformation.mode.Height);

                    RwDebugSendMessage(rwDEBUGMESSAGE, "_rwD3D9CameraBeginUpdate",
                                       buffer);
                    #endif

                    Present.BackBufferWidth = _RwD3D9AdapterInformation.mode.Width;
                    Present.BackBufferHeight = _RwD3D9AdapterInformation.mode.Height;

                    hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

                    if (FAILED(hr))
                    {
                        RWRETURN(FALSE);
                    }

                    /* Change window size */
                    rect.right = rect.left + _RwD3D9AdapterInformation.mode.Width;
                    rect.bottom = rect.top + _RwD3D9AdapterInformation.mode.Height;

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

                    D3D9DeviceRestoreVideoMemory();
                }
                else
                {
                    RWRETURN(FALSE);
                }
            }
            else
            {
                _RwD3D9AdapterInformation.mode.Width = rect.right;
                _RwD3D9AdapterInformation.mode.Height = rect.bottom;
            }
        }

        /* Check if it's a camera texture */
        if (rwRASTERTYPECAMERATEXTURE == topRaster->cType)
        {
            LPSURFACE           surfaceLevel;
            D3DSURFACE_DESC     d3d9Desc;

            /* Set the render & depth/stencil surface */
            if (topRasterExt->cube)
            {
                IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)topRasterExt->texture,
                                                        (D3DCUBEMAP_FACES)topRasterExt->face,
                                                        0,
                                                        &surfaceLevel);
            }
            else
            {
                IDirect3DTexture9_GetSurfaceLevel(topRasterExt->texture, 0,
                                                  &surfaceLevel);
            }

            IDirect3DSurface9_GetDesc(surfaceLevel, &d3d9Desc);

            if (d3d9Desc.Usage & D3DUSAGE_RENDERTARGET)
            {
                _rwD3D9SetRenderTarget(0, surfaceLevel);

                zRaster = RwCameraGetZRaster((RwCamera *) camera);
                if (zRaster)
                {
                    /*
                    * Got to get the top level raster as this is the only one with a
                    * texture surface
                    */
                    zTopRaster = RwRasterGetParent(zRaster);

                    zRasterExt = RASTEREXTFROMCONSTRASTER(zTopRaster);

                    if ( (RwRasterGetType(zTopRaster) & rwRASTERTYPEMASK) == rwRASTERTYPEZBUFFER )
                    {
                        _rwD3D9SetDepthStencilSurface((LPSURFACE)zRasterExt->texture);
                    }
                    else
                    {
                        LPSURFACE   surfaceLevelZ;

                        IDirect3DTexture9_GetSurfaceLevel(zRasterExt->texture, 0,
                                                          &surfaceLevelZ);

                        _rwD3D9SetDepthStencilSurface(surfaceLevelZ);

                        IDirect3DSurface9_Release(surfaceLevelZ);
                    }
                }
                else
                {
                    _rwD3D9SetDepthStencilSurface(NULL);
                }

                hr = D3D_OK;
            }
            else
            {
                hr = E_FAIL;
            }

            if (FAILED(hr))
            {
                NeedToCopyFromBackBuffer = TRUE;

                _rwD3D9SetRenderTarget(0, _RwD3D9RenderSurface);
                _rwD3D9SetDepthStencilSurface(_RwD3D9DepthStencilSurface);
            }

            IDirect3DSurface9_Release(surfaceLevel);
        }
        else
        {
            _rwD3D9SetRenderTarget(0, _RwD3D9RenderSurface);
            _rwD3D9SetDepthStencilSurface(_RwD3D9DepthStencilSurface);
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

    DXCHECK(IDirect3DDevice9_SetViewport(_RwD3DDevice, &viewport));

    /* Update fog range */
    RwD3D9SetRenderState(D3DRS_FOGSTART, FLOATASDWORD(camera->fogPlane));
    RwD3D9SetRenderState(D3DRS_FOGEND, FLOATASDWORD(camera->farPlane));

    /*
     * Begin a scene and exit
     */
    RWRETURN(_rwD3D9BeginScene());
}

/****************************************************************************
 _rwD3D9CameraEndUpdate

 End update to camera, and tidy up

 On entry   : NULL
            : pCamera - camera to end update to
            : 0
 On exit    : TRUE on success
 */

RwBool
_rwD3D9CameraEndUpdate(void *out __RWUNUSED__,
                       void *cameraIn, RwInt32 nIn __RWUNUSED__)
{
    RwRaster            *raster;
    RwRaster            *topRaster;
    const _rwD3D9RasterExt  *topRasterExt;

    RWFUNCTION(RWSTRING("_rwD3D9CameraEndUpdate"));

    raster = RwCameraGetRaster(dgGGlobals.curCamera);

    /*
     * Got to get the top level raster as this is the only one with a
     * texture surface
     */
    topRaster = RwRasterGetParent(raster);

    topRasterExt = RASTEREXTFROMCONSTRASTER(topRaster);

    if (topRasterExt->swapChain == NULL)
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
                    IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)topRasterExt->texture,
                                                            (D3DCUBEMAP_FACES)topRasterExt->face,
                                                            0,
                                                            &surfaceLevel);
                }
                else
                {
                    IDirect3DTexture9_GetSurfaceLevel(topRasterExt->texture, 0,
                                                      &surfaceLevel);
                }

                /* Copy from the backbuffer to the camera texture */
                rectSrc.left = raster->nOffsetX;
                rectSrc.top = raster->nOffsetY;
                rectSrc.right = rectSrc.left + raster->width;
                rectSrc.bottom = rectSrc.top + raster->height;

                if (_RwD3D9DeviceCaps.StretchRectFilterCaps & (D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MAGFPOINT))
                {
                    DXCHECK(IDirect3DDevice9_StretchRect
                                            (_RwD3DDevice,
                                            _RwD3D9RenderSurface, &rectSrc,
                                            surfaceLevel, &rectSrc, D3DTEXF_POINT));
                }
                else
                {
                    DXCHECK(IDirect3DDevice9_StretchRect
                                            (_RwD3DDevice,
                                            _RwD3D9RenderSurface, &rectSrc,
                                            surfaceLevel, &rectSrc, D3DTEXF_NONE));
                }

                IDirect3DSurface9_Release(surfaceLevel);

                NeedToCopyFromBackBuffer = FALSE;
            }
        }
    }

    /* Remove current camera, so we don't use a camera if it becomes destroyed
     * (since we don't get a look in at destruction time).
     */
    dgGGlobals.curCamera = NULL;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RasterShowRaster

 Show raster to screen

 On entry   : Raster
            : Device specific parameter - HWND
 On exit    : TRUE on success
 */

RwBool
_rwD3D9RasterShowRaster(void *raster __RWUNUSED__,
                        void *inOut __RWUNUSED__,
                        RwInt32 flags)
{
    RwBool              doReset = FALSE;
    RwRaster            *topRaster;
    _rwD3D9RasterExt    *topRasterExt;
    HRESULT             hr;

    RWFUNCTION(RWSTRING("_rwD3D9RasterShowRaster"));

    if (InsideScene)
    {
        hr = DXCHECK(IDirect3DDevice9_EndScene(_RwD3DDevice));

        InsideScene = FALSE;
    }

#ifdef RWMETRICS
    /* We're done with these now */
    _rwD3D9MetricsReset();
#endif /* RWMETRICS */

    /* Check if we need to change the VSync present flags */
    if ((flags & rwRASTERFLIPWAITVSYNC) != 0)
    {
        if (Present.PresentationInterval == D3DPRESENT_INTERVAL_IMMEDIATE)
        {
            Present.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

            doReset = TRUE;
        }
    }
    else
    {
        if (Present.PresentationInterval == D3DPRESENT_INTERVAL_ONE)
        {
            Present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

            doReset = TRUE;
        }
    }

    /* Change present flags if needed */
    if (doReset)
    {
        D3D9DeviceReleaseVideoMemory();

        hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

        if (SUCCEEDED(hr))
        {
            D3D9DeviceRestoreVideoMemory();
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
        hr = IDirect3DSwapChain9_Present
                     (topRasterExt->swapChain, NULL, NULL, NULL, NULL, 0);
    }
    else
    {
        hr = IDirect3DDevice9_Present(_RwD3DDevice, NULL, NULL, NULL, NULL);
    }

    /* Check for lost devices */
    if (hr == D3DERR_DEVICELOST)
    {
        hr = IDirect3DDevice9_TestCooperativeLevel(_RwD3DDevice);

        if (hr == D3DERR_DEVICENOTRESET)
        {
#if defined(RWDEBUG)
            RwDebugSendMessage(rwDEBUGMESSAGE,
                               "_rwD3D9RasterShowRaster",
                               "DEVICE LOST");
#endif

            D3D9DeviceReleaseVideoMemory();

            hr = DXCHECK(IDirect3DDevice9_Reset(_RwD3DDevice, &Present));

            if (SUCCEEDED(hr))
            {
                D3D9DeviceRestoreVideoMemory();
            }
        }
    }

    _rwD3D9DynamicVertexBufferManagerForceDiscard();

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
    static RwDevice     rwD3D9DriverDevice = {
        1.0f,                   /* default gamma correction */
        D3D9System,
        MINZBUFFERVALUE, MAXZBUFFERVALUE, /* near and far Z buffer limits */

        _rwD3D9RWSetRenderState, /* RwRenderStateSetFunction */
        _rwD3D9RWGetRenderState, /* RwRenderStateGetFunction */

        _rwD3D9Im2DRenderLine,  /* RwIm2DRenderLineFunction */
        _rwD3D9Im2DRenderTriangle, /* RwIm2DRenderTriangleFunction */
        _rwD3D9Im2DRenderPrimitive, /* RwIm2DRenderPrimitiveFunction */
        _rwD3D9Im2DRenderIndexedPrimitive, /* RwIm2DRenderIndexedPrimitiveFunction */

        NULL, NULL, NULL, NULL  /* These get set up when the 3D Immediate mode module is setup */
    };

    RWFUNCTION(RWSTRING("_rwDeviceGetHandle"));

    RWRETURN(&rwD3D9DriverDevice);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DeviceSupportsDXTTexture is used to query the
 * D3D9 device as to whether D3D9 compressed textures can be loaded as
 * RenderWare textures. If this function returns TRUE,
 * \ref RwD3D9DDSTextureRead will load compressed DDS files
 * into RenderWare textures.
 *
 * Note that this function must be called after \ref RwEngineStart.
 *
 * \return Returns TRUE if compressed textures can be loaded as RenderWare
 * textures or FALSE otherwise.
 *
 * \see RwD3D9DDSTextureRead
 * \see RwEngineStart
 *
 */
RwBool
RwD3D9DeviceSupportsDXTTexture(void)
{
    RwBool              supported = FALSE;

    RWAPIFUNCTION(RWSTRING("RwD3D9DeviceSupportsDXTTexture"));

    if (SUCCEEDED
        (IDirect3D9_CheckDeviceFormat
         (_RwDirect3DObject, _RwD3DAdapterIndex, _RwD3DAdapterType,
          _RwD3D9AdapterInformation.mode.Format, 0, D3DRTYPE_TEXTURE,
          D3DFMT_DXT1))
        ||
        SUCCEEDED(IDirect3D9_CheckDeviceFormat
                  (_RwDirect3DObject, _RwD3DAdapterIndex,
                   _RwD3DAdapterType,
                   _RwD3D9AdapterInformation.mode.Format, 0,
                   D3DRTYPE_TEXTURE, D3DFMT_DXT2))
        ||
        SUCCEEDED(IDirect3D9_CheckDeviceFormat
                  (_RwDirect3DObject, _RwD3DAdapterIndex,
                   _RwD3DAdapterType,
                   _RwD3D9AdapterInformation.mode.Format, 0,
                   D3DRTYPE_TEXTURE, D3DFMT_DXT3))
        ||
        SUCCEEDED(IDirect3D9_CheckDeviceFormat
                  (_RwDirect3DObject, _RwD3DAdapterIndex,
                   _RwD3DAdapterType,
                   _RwD3D9AdapterInformation.mode.Format, 0,
                   D3DRTYPE_TEXTURE, D3DFMT_DXT4))
        ||
        SUCCEEDED(IDirect3D9_CheckDeviceFormat
                  (_RwDirect3DObject, _RwD3DAdapterIndex,
                   _RwD3DAdapterType,
                   _RwD3D9AdapterInformation.mode.Format, 0,
                   D3DRTYPE_TEXTURE, D3DFMT_DXT5)))
    {
        supported = TRUE;
    }

    RWRETURN(supported);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetStencilClear sets the value to store in each
 * stencil-buffer entry.
 *
 * \ref rwCAMERACLEARSTENCIL can be OR'd into the camera clear flags to
 * clear the stencil buffer.
 *
 * \param stencilClear This parameter can be in the range from 0 through
 * (2 to the power \e n) - 1, where \e n is the bit depth of the stencil buffer.
 *
 * \see RwD3D9GetStencilClear
 * \see RwCameraClear
 * \see RwCameraClearMode
 */
void
RwD3D9SetStencilClear(RwUInt32 stencilClear)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetStencilClear"));

    StencilClearValue = stencilClear;

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9GetStencilClear returns the current value that will be stored in
 * each stencil-buffer entry when it is cleared.
 *
 * \returns Stencil clear value, in the range 0 through (2 to the power \e n) - 1,
 * where \e n is the bit depth of the stencil buffer.
 *
 * \see RwD3D9SetStencilClear
 * \see RwCameraClear
 */
RwUInt32
RwD3D9GetStencilClear(void)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetStencilClear"));

    RWRETURN(StencilClearValue);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9GetCurrentD3DDevice is used to determine which (if any)
 * Direct3D9 device is being used for 3D rendering of the current scene.
 *
 * It is expected that, once cast to a LPDIRECT3DDEVICE9 pointer, the
 * device returned by this function can be used to call D3D9 functions from
 * outside of RenderWare. This should be done with caution however as
 * RenderWare maintains an internal cache that will not be updated by
 * external D3D9 calls. In fact, the cache is made invalid when
 * \ref RwD3D9GetCurrentD3DDevice is called. Because of this, if the device
 * is required, it is recommended that it is obtained immediately after
 * \ref RwEngineStart is called since this does not invalidate the cache.
 *
 * \note This function does \e not increment any reference counts.
 *
 * \return Returns a pointer to the Direct3D9 device used for rendering or NULL
 * if there is a error.
 *
 */
void               *
RwD3D9GetCurrentD3DDevice(void)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetCurrentD3DDevice"));

    D3D9ClearCache();

    RWRETURN((void *) _RwD3DDevice);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9GetCurrentD3DRenderTarget is used to determine which (if
 * any) LPSURFACE is being used as the target for 3D rendering of the
 * current scene.
 *
 * It is expected that, once cast to a LPSURFACE pointer, the
 * surface returned by this function can be used as a parameter to D3D
 * functions from outside of RenderWare, giving greater flexibility to the
 * programmer at the expense of portability. This should be done with caution
 * however as RenderWare maintains an internal cache that will not be updated
 * by external D3D9 calls.
 *
 * \param index Index of the render target. The device can support multiple render targets.
 *
 * \note Calling this method will \e not increase the internal reference count on the
 * IDirect3DSurface9 interface.
 *
 * \return Returns a pointer to the render target surface used for rendering or NULL
 * if there is a error.
 *
 */
void               *
RwD3D9GetCurrentD3DRenderTarget(RwUInt32 index)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetCurrentD3DRenderTarget"));
    RWASSERT(index < MAX_RENDERTARGETS);

    RWRETURN((void *)CurrentRenderSurface[index]);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetRenderTarget sets the raster to be used as the target for
 * 3D rendering of the current scene.
 *
 * \param index Index of the render target. The device can support multiple render targets.
 * \param raster Pointer to the \ref RwRaster to set as a render target.
 *
 * \return TRUE if the render target was set, FALSE otherwise.
 *
 */
RwBool
RwD3D9SetRenderTarget(RwUInt32 index, RwRaster *raster)
{
    LPSURFACE   surface = NULL;
    RwBool      rv;

    RWAPIFUNCTION(RWSTRING("RwD3D9SetRenderTarget"));

    if (raster != NULL)
    {
        const _rwD3D9RasterExt *rasExt;

        rasExt = RASTEREXTFROMCONSTRASTER(RwRasterGetParent(raster));

        /* Get the surface level */
        if (rasExt->cube)
        {
            IDirect3DCubeTexture9_GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9)rasExt->texture,
                                                    (D3DCUBEMAP_FACES)rasExt->face,
                                                    0,
                                                    &surface);
        }
        else
        {
            IDirect3DTexture9_GetSurfaceLevel(rasExt->texture, 0, &surface);
        }
    }

    rv = _rwD3D9SetRenderTarget(index, surface);

    if (surface != NULL)
    {
        IDirect3DSurface9_Release(surface);
    }

    RWRETURN(rv);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetFVF sets the current vertex stream declaration in RWD3D9.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetFVF since RenderWare can optimize
 * performance using it's internal cache. See \ref d3d9cache.
 *
 * \param fvf Flexible Vertex Format Constants.
 *
 * \see RwD3D9CreateVertexShader
 * \see RwD3D9DeleteVertexShader
 * \see RwD3D9SetVertexDeclaration
 * \see RwD3D9SetVertexShader
 * \see RwD3D9SetVertexShaderConstant
 *
 */
#if defined(DOXYGEN)
void
RwD3D9SetFVF(RwUInt32 fvf)
{
}
#endif
void
_rwD3D9SetFVF(RwUInt32 fvf)
{
    RWAPIFUNCTION(RWSTRING("_rwD3D9SetFVF"));

    if (_rwD3D9LastFVFUsed != fvf)
    {
        HRESULT             hr;

        hr = DXCHECK(IDirect3DDevice9_SetFVF(_RwD3DDevice, fvf));

        if (FAILED(hr))
        {
            _rwD3D9LastFVFUsed = 0xffffffff;
        }
        else
        {
            _rwD3D9LastFVFUsed = fvf;
        }

        _rwD3D9LastVertexDeclarationUsed = (void *)0xffffffff;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetVertexDeclaration sets the current vertex stream declaration in RWD3D9.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetVertexDeclaration since RenderWare can optimize
 * performance using it's internal cache. See \ref d3d9cache.
 *
 * \param vertexDeclaration Pointer to an IDirect3DVertexDeclaration9 object, which contains the vertex declaration.
 *
 * \see RwD3D9CreateVertexShader
 * \see RwD3D9DeleteVertexShader
 * \see RwD3D9SetVertexFVF
 * \see RwD3D9SetVertexShader
 * \see RwD3D9SetVertexShaderConstant
 *
 */
#if defined(DOXYGEN)
void
RwD3D9SetVertexDeclaration(void *vertexDeclaration)
{
}
#endif
void
_rwD3D9SetVertexDeclaration(void *vertexDeclaration)
{
    RWAPIFUNCTION(RWSTRING("_rwD3D9SetVertexDeclaration"));

    if (_rwD3D9LastVertexDeclarationUsed != vertexDeclaration)
    {
        HRESULT             hr;

        hr = DXCHECK(IDirect3DDevice9_SetVertexDeclaration(_RwD3DDevice, vertexDeclaration));

        if (FAILED(hr))
        {
            _rwD3D9LastVertexDeclarationUsed = (void *)0xffffffff;
        }
        else
        {
            _rwD3D9LastVertexDeclarationUsed = vertexDeclaration;
        }

        _rwD3D9LastFVFUsed = 0xffffffff;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetVertexShader is used to set the vertex shader used to process
 * vertices in RWD3D9.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetVertexShader since RenderWare can optimize
 * performance using it's internal cache. See \ref d3d9cache.
 *
 * \param shader Pointer to the vertex shader to be set. This pointer is returned from
 * IDirect3DDevice9::CreateVertexShader.
 *
 * \see RwD3D9CreateVertexShader
 * \see RwD3D9DeleteVertexShader
 * \see RwD3D9SetFVF
 * \see RwD3D9SetVertexShaderConstant
 *
 */
#if defined(DOXYGEN)
void
RwD3D9SetVertexShader(void *shader)
{
}
#endif
void
_rwD3D9SetVertexShader(void *shader)
{
    RWAPIFUNCTION(RWSTRING("_rwD3D9SetVertexShader"));

    if (_rwD3D9LastVertexShaderUsed != shader)
    {
        HRESULT             hr;

        hr = DXCHECK(IDirect3DDevice9_SetVertexShader
                     (_RwD3DDevice, shader));

        if (FAILED(hr))
        {
            _rwD3D9LastVertexShaderUsed = (void *)0xffffffff;
        }
        else
        {
            _rwD3D9LastVertexShaderUsed = shader;
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetPixelShader is used to set the pixel shader used to process
 * pixels in RWD3D9.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9:SetPixelShader since RenderWare can optimize
 * performance using it's internal cache. See \ref d3d9cache.
 *
 * \param shader Pointer to the vertex shader to be set. This pointer is returned from
 * IDirect3DDevice9::CreateVertexShader.
 *
 * \see RwD3D9CreatePixelShader
 * \see RwD3D9DeletePixelShader
 * \see RwD3D9SetPixelShaderConstant
 *
 */
#if defined(DOXYGEN)
void
RwD3D9SetPixelShader(void *shader)
{
}
#endif
void
_rwD3D9SetPixelShader(void *shader)
{
    RWAPIFUNCTION(RWSTRING("_rwD3D9SetPixelShader"));

    if (_rwD3D9LastPixelShaderUsed != shader)
    {
        HRESULT             hr;

        hr = DXCHECK(IDirect3DDevice9_SetPixelShader
                     (_RwD3DDevice, shader));

        if (FAILED(hr))
        {
            _rwD3D9LastPixelShaderUsed = (void *)0xffffffff;
        }
        else
        {
            _rwD3D9LastPixelShaderUsed = shader;
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetStreamSource binds a vertex buffer to a device data stream.
 *
 * This function creates an association between vertex data and a data
 * stream that feeds RWD3D9 primitive processing functions.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetStreamSource since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param streamNumber The stream number to set.
 * \param streamData Void pointer to the LPDIRECT3DVERTEXBUFFER9 vertex
 * buffer to be used as the source to the stream.
 * \param offset Offset from the beginning of the stream to the beginning of the vertex data, in bytes.
 * \param stride The size of the vertex formats in the vertex buffer.
 *
 * \see RwD3D9CreateVertexBuffer
 * \see RwD3D9DestroyVertexBuffer
 * \see RwD3D9DynamicVertexBufferCreate
 * \see RwD3D9DynamicVertexBufferDestroy
 *
 */
void
RwD3D9SetStreamSource(RwUInt32 streamNumber,
                      void *streamData,
                      RwUInt32 offset,
                      RwUInt32 stride)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetStreamSource"));
    RWASSERT(streamNumber < MAX_STREAMS);

    if ((LastVertexStreamUsed[streamNumber].vertexBuffer != streamData) ||
        (LastVertexStreamUsed[streamNumber].offset != offset) ||
        (LastVertexStreamUsed[streamNumber].stride != stride))
    {
        LastVertexStreamUsed[streamNumber].vertexBuffer = streamData;
        LastVertexStreamUsed[streamNumber].offset = offset;
        LastVertexStreamUsed[streamNumber].stride = stride;

        DXCHECK(IDirect3DDevice9_SetStreamSource(_RwD3DDevice,
                                                 streamNumber,
                                                 (LPDIRECT3DVERTEXBUFFER9)streamData,
                                                 offset,
                                                 stride));

#if defined( RWMETRICS )
        _rwD3D9MetricsGet()->numVBSwitches++;
#endif
    }

    RWRETURNVOID();
}

void
_rwD3D9SetStreams(const RxD3D9VertexStream *streams,
                  RwBool useOffsets)
{
    RwUInt32 n;

    RWFUNCTION(RWSTRING("_rwD3D9SetStreams"));
    RWASSERT(streams != NULL);

    if (useOffsets == FALSE)
    {
        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            LPVERTEXBUFFER vertexBuffer;

            vertexBuffer = (LPVERTEXBUFFER)streams[n].vertexBuffer;

            if (vertexBuffer != NULL)
            {
                if ((LastVertexStreamUsed[n].vertexBuffer != vertexBuffer) ||
                    (LastVertexStreamUsed[n].offset != 0) ||
                    (LastVertexStreamUsed[n].stride != streams[n].stride))
                {
                    LastVertexStreamUsed[n].vertexBuffer = vertexBuffer;
                    LastVertexStreamUsed[n].offset = 0;
                    LastVertexStreamUsed[n].stride = streams[n].stride;

                    DXCHECK(IDirect3DDevice9_SetStreamSource(_RwD3DDevice, n,
                                                        vertexBuffer,
                                                        0,
                                                        streams[n].stride));

                    #if defined( RWMETRICS )
                    _rwD3D9MetricsGet()->numVBSwitches++;
                    #endif
                }
            }
            else
            {
                if (LastVertexStreamUsed[n].vertexBuffer != NULL)
                {
                    LastVertexStreamUsed[n].vertexBuffer = NULL;
                    LastVertexStreamUsed[n].offset = 0;
                    LastVertexStreamUsed[n].stride = 0;

                    DXCHECK(IDirect3DDevice9_SetStreamSource(_RwD3DDevice, n, NULL, 0, 0));

                    #if defined( RWMETRICS )
                    _rwD3D9MetricsGet()->numVBSwitches++;
                    #endif
                }
            }
        }
    }
    else
    {
        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            LPVERTEXBUFFER vertexBuffer;

            vertexBuffer = (LPVERTEXBUFFER)streams[n].vertexBuffer;

            if (vertexBuffer != NULL)
            {
                if ((LastVertexStreamUsed[n].vertexBuffer != vertexBuffer) ||
                    (LastVertexStreamUsed[n].offset != streams[n].offset) ||
                    (LastVertexStreamUsed[n].stride != streams[n].stride))
                {
                    LastVertexStreamUsed[n].vertexBuffer = vertexBuffer;
                    LastVertexStreamUsed[n].offset = streams[n].offset;
                    LastVertexStreamUsed[n].stride = streams[n].stride;

                    RWASSERT(streams[n].offset == 0 ||
                             (_RwD3D9DeviceCaps.DevCaps2 & D3DDEVCAPS2_STREAMOFFSET) != 0);

                    DXCHECK(IDirect3DDevice9_SetStreamSource(_RwD3DDevice, n,
                                                        vertexBuffer,
                                                        streams[n].offset,
                                                        streams[n].stride));

                    #if defined( RWMETRICS )
                    _rwD3D9MetricsGet()->numVBSwitches++;
                    #endif
                }
            }
            else
            {
                if (LastVertexStreamUsed[n].vertexBuffer != NULL)
                {
                    LastVertexStreamUsed[n].vertexBuffer = NULL;
                    LastVertexStreamUsed[n].offset = 0;
                    LastVertexStreamUsed[n].stride = 0;

                    DXCHECK(IDirect3DDevice9_SetStreamSource(_RwD3DDevice, n, NULL, 0, 0));

                    #if defined( RWMETRICS )
                    _rwD3D9MetricsGet()->numVBSwitches++;
                    #endif
                }
            }
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetIndices sets the current index array to an index buffer.
 * The single set of indices is used to index all streams of vertex buffers.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetIndices since RenderWare can optimize
 * performance using it's internal cache. See \ref d3d9cache.
 *
 * \param indexBuffer Void pointer to the LPDIRECT3DINDEXBUFFER9,
 * representing the index data to be set.
 *
 * \see RwD3D9IndexBufferCreate
 *
 */
#if defined(DOXYGEN)
void
RwD3D9SetIndices(void *indexBuffer)
{
}
#endif
void
_rwD3D9SetIndices(void *indexBuffer)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetIndices"));

    if (_rwD3D9LastIndexBufferUsed != indexBuffer)
    {
        _rwD3D9LastIndexBufferUsed = indexBuffer;

        DXCHECK(IDirect3DDevice9_SetIndices(_RwD3DDevice,
                                            (LPDIRECT3DINDEXBUFFER9)indexBuffer));
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DrawIndexedPrimitiveUP renders the specified geometric
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
 * functions are slower than those that use a D3D9 vertex buffer.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::DrawIndexedPrimitiveUP since RenderWare
 * can optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param primitiveType Member of the D3DPRIMITIVETYPE enumerated type,
 * describing the type of primitive to render.
 * \param minIndex Minimum vertex index, relative to zero (the start of
 * \e indexData), for vertices used during this call.
 * \param numVertices Number of vertices used during this call; starting
 * from \e minIndex.
 * \param primitiveCount Number of primitives to render.
 * \param indexData User memory pointer to the index data.
 * \param vertexStreamZeroData User memory pointer to the vertex data.
 * \param vertexStreamZeroStride The size of each vertex, in \e bytes.
 *
 * \see RwD3D9DrawPrimitiveUP
 * \see RwD3D9DrawIndexedPrimitive
 * \see RwD3D9DrawPrimitive
 * \see RwD3D9SetVertexShader
 * \see RwD3D9SetPixelShader
 *
 */
#if defined(DOXYGEN)
void
RwD3D9DrawIndexedPrimitiveUP(RwUInt32 primitiveType,
                             RwUInt32 minIndex,
                             RwUInt32 numVertices,
                             RwUInt32 primitiveCount,
                             const void *indexData,
                             const void *vertexStreamZeroData,
                             RwUInt32 vertexStreamZeroStride)
{
}
#endif
void
_rwD3D9DrawIndexedPrimitiveUP(RwUInt32 primitiveType,
                             RwUInt32 minIndex,
                             RwUInt32 numVertices,
                             RwUInt32 primitiveCount,
                             const void *indexData,
                             const void *vertexStreamZeroData,
                             RwUInt32 vertexStreamZeroStride)
{
    const D3DPRIMITIVETYPE type = (D3DPRIMITIVETYPE)primitiveType;
    HRESULT hr;

    RWAPIFUNCTION(RWSTRING("_rwD3D9DrawIndexedPrimitiveUP"));

    RwD3D9SetStreamSource(0, NULL, 0, 0);
    RwD3D9SetIndices(NULL);

    _rwD3D9RenderStateFlushCache();

    RWASSERT(primitiveCount <= _RwD3D9DeviceCaps.MaxPrimitiveCount);

    hr = DXCHECK(IDirect3DDevice9_DrawIndexedPrimitiveUP(_RwD3DDevice,
                                                         type,
                                                         minIndex,
                                                         numVertices,
                                                         primitiveCount,
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
                RWSRCGLOBAL(metrics)->numProcTriangles += primitiveCount;
                break;
        }
    }
#endif /* defined( RWMETRICS ) */

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DrawPrimitiveUP renders the specified non-indexed geometric
 * primitive with data specified by a user memory pointer.
 *
 * This function is intended for use in applications that are unable to
 * store their vertex data in vertex buffers. This method supports only
 * a single vertex stream. The effect of this call is to use the provided
 * vertex data pointer and stride for vertex stream zero.
 *
 * It is advised that you batch primitives when calling this function to
 * maximize efficiency. Also it should be noted that the user memory pointer
 * functions are slower than those that use a D3D9 vertex buffer.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::DrawPrimitiveUP since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param primitiveType Member of the D3DPRIMITIVETYPE enumerated type,
 * describing the type of primitive to render.
 * \param primitiveCount Number of primitives to render.
 * \param vertexStreamZeroData User memory pointer to the vertex data.
 * \param vertexStreamZeroStride The size of each vertex, in \e bytes.
 *
 * \see RwD3D9DrawIndexedPrimitiveUP
 * \see RwD3D9DrawIndexedPrimitive
 * \see RwD3D9DrawPrimitive
 * \see RwD3D9SetVertexShader
 * \see RwD3D9SetPixelShader
 *
 */
#if defined(DOXYGEN)
void
RwD3D9DrawPrimitiveUP(RwUInt32 primitiveType,
                      RwUInt32 primitiveCount,
                      const void *vertexStreamZeroData,
                      RwUInt32 vertexStreamZeroStride)
{
}
#endif
void
_rwD3D9DrawPrimitiveUP(RwUInt32 primitiveType,
                      RwUInt32 primitiveCount,
                      const void *vertexStreamZeroData,
                      RwUInt32 vertexStreamZeroStride)
{
    const D3DPRIMITIVETYPE type = (D3DPRIMITIVETYPE)primitiveType;
    HRESULT hr;

    RWAPIFUNCTION(RWSTRING("_rwD3D9DrawPrimitiveUP"));

    RwD3D9SetStreamSource(0, NULL, 0, 0);
    RwD3D9SetIndices(NULL);

    _rwD3D9RenderStateFlushCache();

    hr = DXCHECK(IDirect3DDevice9_DrawPrimitiveUP(_RwD3DDevice,
                                                  type,
                                                  primitiveCount,
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
                RWSRCGLOBAL(metrics)->numProcTriangles += primitiveCount;
                break;
        }
    }
#endif /* defined( RWMETRICS ) */

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DrawIndexedPrimitive renders the specified geometric primitive,
 * based on indexing into an array of vertices.
 *
 * This function draws indexed primitives from the current set of data
 * input streams.
 *
 * It is advised that you batch primitives when calling this function to
 * maximize efficiency.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::DrawIndexedPrimitive since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param primitiveType Member of the D3DPRIMITIVETYPE enumerated type,
 * describing the type of primitive to render.
 * \param baseVertexIndex Offset from the start of the vertex buffer to the
 * first vertex index.
 * \param minIndex Minimum vertex index, relative to \e baseVertexIndex, for
 * vertices used during this call.
 * \param numVertices The number of vertices used during this call starting
 * from \e baseVertexIndex + \e minIndex.
 * \param startIndex Location in the index array to start reading indices.
 * \param primitiveCount Number of primitives to render.
 *
 * \see RwD3D9DrawPrimitive
 * \see RwD3D9DrawPrimitiveUP
 * \see RwD3D9DrawIndexedPrimitiveUP
 * \see RwD3D9SetVertexShader
 * \see RwD3D9SetPixelShader
 * \see RwD3D9SetStreamSource
 * \see RwD3D9SetIndices
 *
 */
#if defined(DOXYGEN)
void
RwD3D9DrawIndexedPrimitive(RwUInt32 primitiveType,
                           RwInt32 baseVertexIndex,
                           RwUInt32 minIndex,
                           RwUInt32 numVertices,
                           RwUInt32 startIndex,
                           RwUInt32 primitiveCount)
{
}
#endif
void
_rwD3D9DrawIndexedPrimitive(RwUInt32 primitiveType,
                           RwInt32 baseVertexIndex,
                           RwUInt32 minIndex,
                           RwUInt32 numVertices,
                           RwUInt32 startIndex,
                           RwUInt32 primitiveCount)
{
    const D3DPRIMITIVETYPE type = (D3DPRIMITIVETYPE)primitiveType;
    HRESULT hr;

    RWAPIFUNCTION(RWSTRING("_rwD3D9DrawIndexedPrimitive"));

    _rwD3D9RenderStateFlushCache();

    RWASSERT(primitiveCount <= _RwD3D9DeviceCaps.MaxPrimitiveCount);
    RWASSERT(_rwD3D9LastIndexBufferUsed != NULL);

    hr = DXCHECK(IDirect3DDevice9_DrawIndexedPrimitive(_RwD3DDevice,
                                                       type,
                                                       baseVertexIndex,
                                                       minIndex,
                                                       numVertices,
                                                       startIndex,
                                                       primitiveCount));

#if defined( RWMETRICS )
    if (SUCCEEDED(hr))
    {
        switch (primitiveType)
        {
            case D3DPT_TRIANGLELIST:
            case D3DPT_TRIANGLESTRIP:
            case D3DPT_TRIANGLEFAN:
                RWSRCGLOBAL(metrics)->numProcTriangles += primitiveCount;
                break;
        }
    }
#endif /* defined( RWMETRICS ) */

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DrawPrimitive renders a sequence of non-indexed, geometric
 * primitives of the specified type from the current set of data input
 * streams.
 *
 * It is advised that you batch primitives when calling this function to
 * maximize efficiency.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::DrawPrimitive since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param primitiveType Member of the D3DPRIMITIVETYPE enumerated type,
 * describing the type of primitive to render.
 * \param startVertex Index of the first vertex to load. Beginning at
 * \e startVertex the correct number of vertices will be read out of the
 * current vertex buffer.
 * \param primitiveCount Number of primitives to render.
 *
 * \see RwD3D9DrawIndexedPrimitive
 * \see RwD3D9DrawPrimitiveUP
 * \see RwD3D9DrawIndexedPrimitiveUP
 * \see RwD3D9SetVertexShader
 * \see RwD3D9SetPixelShader
 * \see RwD3D9SetStreamSource
 * \see RwD3D9SetIndices
 *
 */
#if defined(DOXYGEN)
void
RwD3D9DrawPrimitive(RwUInt32 primitiveType,
                    RwUInt32 startVertex,
                    RwUInt32 primitiveCount)
{
}
#endif
void
_rwD3D9DrawPrimitive(RwUInt32 primitiveType,
                    RwUInt32 startVertex,
                    RwUInt32 primitiveCount)
{
    const D3DPRIMITIVETYPE type = (D3DPRIMITIVETYPE)primitiveType;
    HRESULT hr;

    RWAPIFUNCTION(RWSTRING("_rwD3D9DrawPrimitive"));

    _rwD3D9RenderStateFlushCache();

    hr = DXCHECK(IDirect3DDevice9_DrawPrimitive(_RwD3DDevice,
                                                type,
                                                startVertex,
                                                primitiveCount));

#if defined( RWMETRICS )
    if (SUCCEEDED(hr))
    {
        switch (primitiveType)
        {
            case D3DPT_TRIANGLELIST:
            case D3DPT_TRIANGLESTRIP:
            case D3DPT_TRIANGLEFAN:
                RWSRCGLOBAL(metrics)->numProcTriangles += primitiveCount;
                break;
        }
    }
#endif /* defined( RWMETRICS ) */

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetTransform sets a transformation-related state. These states refer
 * to the view, projection, texture, and world matrices in D3D9. See the D3D9 API
 * documentation from Microsoft for more details.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetTransform since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param state State variable that is being modified, a member of the
 * D3DTRANSFORMSTATETYPE enumerated type.
 * \param matrix Void pointer to a D3DMATRIX structure that modifies the
 * current transformation.
 *
 * \return TRUE if the matrix was set, FALSE otherwise.
 *
 * \see RwD3D9SetTransformWorld
 * \see RwD3D9GetTransform
 * \see RwD3D9SetRenderState
 *
 */
RwBool
RwD3D9SetTransform(RwUInt32 state, const void *matrix)
{
    HRESULT             hr;

    RWAPIFUNCTION(RWSTRING("RwD3D9SetTransform"));

    if (matrix)
    {
        if (LastMatrixUsed[state] &&
            D3D9MemEqual((const RwUInt32 *) LastMatrixUsed[state],
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

            memcpy(LastMatrixUsed[state], matrix, sizeof(D3DMATRIX));

            hr = DXCHECK(IDirect3DDevice9_SetTransform(_RwD3DDevice,
                                                       (D3DTRANSFORMSTATETYPE)state,
                                                       (const D3DMATRIX*) matrix));

            if (state == D3DTS_WORLD)
            {
                LastWorldMatrixUsedIdentity = FALSE;
            }
        }
    }
    else
    {
        if (LastMatrixUsed[state] &&
            D3D9MemEqual((const RwUInt32 *) LastMatrixUsed[state],
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

            memcpy(LastMatrixUsed[state], &IdentityMatrix, sizeof(D3DMATRIX));

            hr = DXCHECK(IDirect3DDevice9_SetTransform(_RwD3DDevice,
                                                       (D3DTRANSFORMSTATETYPE)state,
                                                       &IdentityMatrix));

            if (state == D3DTS_WORLD)
            {
                LastWorldMatrixUsedIdentity = TRUE;
            }
        }
    }

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9GetTransform retrieves a matrix describing a transformation
 * state.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::GetTransform since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param state State variable that is being requested, a member of the
 * D3DTRANSFORMSTATETYPE enumerated type.
 * \param matrix Void pointer to a D3DMATRIX structure, describing the
 * returned transformation state.
 *
 * \see RwD3D9SetTransform
 * \see RwD3D9SetTransformWorld
 * \see RwD3D9SetRenderState
 *
 */
void
RwD3D9GetTransform(RwUInt32 state, void *matrix)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetTransform"));
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
 * \ingroup rwengined3d9
 * \ref RwD3D9SetTransformWorld sets the object-to-world transformation
 * matrix.
 *
 * This function internally converts the RenderWare \ref RwMatrix to the
 * equivalent D3D9 D3DMATRIX structure, and sets this as the current
 * world transformation matrix.
 *
 * \param matrix Pointer to a \ref RwMatrix structure that modifies the
 * object-to-world transformation.
 *
 * \return TRUE if the matrix was set, FALSE otherwise.
 *
 * \see RwD3D9SetTransformWorld
 * \see RwD3D9GetTransform
 * \see RwD3D9SetRenderState
 *
 */
RwBool
RwD3D9SetTransformWorld(const RwMatrix * matrix)
{
    static D3DMATRIX    d3dMatrix =
        D3DMatrixInitMacro(0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f);

    RWAPIFUNCTION(RWSTRING("RwD3D9SetTransformWorld"));

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
            !D3D9MemEqual((const RwUInt32 *)
                          LastMatrixUsed[D3DTS_WORLD],
                          (const RwUInt32 *) &d3dMatrix,
                          sizeof(D3DMATRIX)))
        {
            HRESULT             hr;

            memcpy(LastMatrixUsed[D3DTS_WORLD], &d3dMatrix, sizeof(D3DMATRIX));

            hr = DXCHECK(IDirect3DDevice9_SetTransform
                                    (_RwD3DDevice, D3DTS_WORLD, &d3dMatrix));

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

            memcpy(LastMatrixUsed[D3DTS_WORLD], &IdentityMatrix,
                   sizeof(D3DMATRIX));

            hr = DXCHECK(IDirect3DDevice9_SetTransform
                                (_RwD3DDevice, D3DTS_WORLD, &IdentityMatrix));

            LastWorldMatrixUsedIdentity = TRUE;

            RWRETURN(SUCCEEDED(hr));
        }
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetLight assigns a set of lighting properties in the requested
 * lights array index.
 *
 * It is advised for efficiency purposes for applications using a large number
 * of lights in an array, that the whole array of lights is sent to D3D9 with
 * those individual lights required enabled, rather than sending individual
 * lights.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetLight since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param index Zero-based index of the set of lighting properties to assign.
 * If a set of lighting properties exists at this index, it is overwritten
 * by the new properties specified.
 * \param light Void pointer to a D3DLIGHT9 structure, containing the
 * lighting-parameters to set.
 *
 * \return TRUE if the light was set, FALSE otherwise.
 *
 * \see RwD3D9GetLight
 * \see RwD3D9EnableLight
 *
 */
RwBool
RwD3D9SetLight(RwInt32 index, const void *light)
{
    const D3DLIGHT9 *d3dLight = (const D3DLIGHT9 *)light;
    RwBool          doSet = TRUE;

    RWAPIFUNCTION(RWSTRING("RwD3D9SetLight"));

    if (index >= MaxNumLights)
    {
        RwInt32 n;

        n = MaxNumLights;

        MaxNumLights = index + 1;
        if (LightsCache != NULL)
        {
            LightsCache = (RxD3D9Light *)RwRealloc(LightsCache, MaxNumLights * sizeof(RxD3D9Light),
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            LightsCache = (RxD3D9Light *)RwMalloc(MaxNumLights * sizeof(RxD3D9Light),
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }

        for (; n < MaxNumLights; n++)
        {
            LightsCache[n].enabled = FALSE;
        }
    }
    else
    {
        if (_rwD3D9LightsEqual(d3dLight, &(LightsCache[index].light)))
        {
            doSet = FALSE;
        }
    }

    if (doSet)
    {
        HRESULT hr;

        memcpy(&(LightsCache[index].light), d3dLight, sizeof(D3DLIGHT9));

        hr = DXCHECK(IDirect3DDevice9_SetLight
                     (_RwD3DDevice, index, d3dLight));

        #if defined( RWMETRICS )
        _rwD3D9MetricsGet()->numLightsChanged++;
        #endif

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9ForceLight

 On entry   : Set a light without doing any comparasion

 On exit    : TRUE on success
 */
RwBool
_rwD3D9ForceLight(RwInt32 index, const void *light)
{
    const D3DLIGHT9    *d3dLight = (const D3DLIGHT9 *)light;
    HRESULT            hr;

    RWFUNCTION(RWSTRING("_rwD3D9ForceLight"));

    if (index >= MaxNumLights)
    {
        RwInt32 n;

        n = MaxNumLights;

        MaxNumLights = index + 1;

        if (LightsCache != NULL)
        {
            LightsCache = (RxD3D9Light *)RwRealloc(LightsCache, MaxNumLights * sizeof(RxD3D9Light),
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            LightsCache = (RxD3D9Light *)RwMalloc(MaxNumLights * sizeof(RxD3D9Light),
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }

        for (; n < MaxNumLights; n++)
        {
            LightsCache[n].enabled = FALSE;
        }
    }

    memcpy(&(LightsCache[index].light), d3dLight, sizeof(D3DLIGHT9));

    hr = DXCHECK(IDirect3DDevice9_SetLight
                 (_RwD3DDevice, index, d3dLight));

#if defined( RWMETRICS )
    _rwD3D9MetricsGet()->numLightsChanged++;
#endif

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9GetLight retrieves a set of lighting properties for the requested
 * lights array index.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::GetLight since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param index Zero-based index of the set of lighting properties to retrieve.
 * \param light Void pointer to a D3DLIGHT9 structure that is filled with the
 * retrieved lighting-parameter set.
 *
 * \see RwD3D9SetLight
 * \see RwD3D9EnableLight
 *
 */
void
RwD3D9GetLight(RwInt32 index, void *light)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetLight"));

    if (index < MaxNumLights)
    {
        memcpy(light, &(LightsCache[index].light), sizeof(D3DLIGHT9));
    }
    else
    {
        memset(light, 0, sizeof(D3DLIGHT9));
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9EnableLight enables or disables a set of lighting parameters.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::EnableLight since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param index Zero-based index of the set of lighting properties that are
 * the target of this function.
 * \param enable Value that indicates if the set of lighting parameters are
 * being enabled or disabled. Set this parameter to TRUE to enable lighting
 * with the parameters at the specified index, or FALSE to disable it.
 *
 * \return TRUE if the function succeeds, FALSE otherwise.
 *
 * \see RwD3D9SetLight
 * \see RwD3D9GetLight
 *
 */
RwBool
RwD3D9EnableLight(RwInt32 index, RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9EnableLight"));

    if (index < MaxNumLights)
    {
        if (LightsCache[index].enabled != enable)
        {
            HRESULT             hr;

            LightsCache[index].enabled = enable;

            hr = DXCHECK(IDirect3DDevice9_LightEnable
                         (_RwD3DDevice, index, enable));

            RWRETURN(SUCCEEDED(hr));
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9LightsEqual

 Purpose:   compares two lights

*/
RwBool
_rwD3D9LightsEqual(const D3DLIGHT9 * light0, const D3DLIGHT9 * light1)
{
    RWFUNCTION(RWSTRING("_rwD3D9LightsEqual"));

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
 _rwD3D9FindSimilarLight

 Purpose:   return the index of a similar light, 0xffffffff on failure

*/
RwUInt32
_rwD3D9FindSimilarLight(const void *light)
{
    RwInt32             index;

    RWFUNCTION(RWSTRING("_rwD3D9FindSimilarLight"));

    for (index = 0; index < MaxNumLights; index++)
    {
        if (_rwD3D9LightsEqual((const D3DLIGHT9 *)light,
                               &(LightsCache[index].light)))
        {
            break;
        }
    }

    RWRETURN(index < MaxNumLights ? index : 0xffffffff);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9IndexBufferCreate creates a 16-bit index buffer that is write-only and managed
 * by D3D9.
 *
 * Index buffers created using this function are \e not managed by RWD3D9, unlike dynamic vertex
 * buffers.
 *
 * \param numIndices Number of indices to create in the buffer.
 * \param indexBuffer Void pointer to the LPDIRECT3DINDEXBUFFER9 structure that is
 * to be the index buffer.
 *
 * \return TRUE if the index buffer was successfully created, FALSE otherwise.
 *
 * \see RwD3D9DynamicVertexBufferCreate
 * \see RwD3D9DynamicVertexBufferDestroy
 * \see RwD3D9CreateVertexBuffer
 * \see RwD3D9DestroyVertexBuffer
 *
 */
RwBool
RwD3D9IndexBufferCreate(RwUInt32 numIndices, void **indexBuffer)
{
    HRESULT             hr = D3D_OK;

    RWAPIFUNCTION(RWSTRING("RwD3D9IndexBufferCreate"));
    RWASSERT(indexBuffer);

    hr =
    DXCHECK(IDirect3DDevice9_CreateIndexBuffer(_RwD3DDevice,
                                               numIndices *
                                               sizeof(RwUInt16),
                                               D3DUSAGE_WRITEONLY,
                                               D3DFMT_INDEX16,
                                               D3DPOOL_MANAGED,
                                               (LPDIRECT3DINDEXBUFFER9 *)indexBuffer,
                                               NULL));

    RWRETURN(SUCCEEDED(hr));
}

RwBool
_rwD3D9IndexBuffer32bitsCreate(RwUInt32 numIndices, void **indexBuffer)
{
    HRESULT hr;

    RWFUNCTION(RWSTRING("_rwD3D9IndexBuffer32bitsCreate"));
    RWASSERT(indexBuffer);

    hr =
    DXCHECK(IDirect3DDevice9_CreateIndexBuffer(_RwD3DDevice,
                                               numIndices *
                                               sizeof(RwUInt32),
                                               D3DUSAGE_WRITEONLY,
                                               D3DFMT_INDEX32,
                                               D3DPOOL_MANAGED,
                                               (LPDIRECT3DINDEXBUFFER9 *) indexBuffer,
                                               NULL));

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9CreateVertexDeclaration creates a vertex declaration.
 *
 * \param elements Pointer to the vertex declaration elements.
 * \param vertexdeclaration Pointer to the returned vertex declaration. This value
 * cannot be set as NULL.
 *
 * \return TRUE if the vertex shader was created successfully, FALSE otherwise.
 *
 * \see RwD3D9DeleteVertexDeclaration
 * \see RwD3D9SetVertexDeclaration
 *
 */
RwBool
RwD3D9CreateVertexDeclaration(const void *elements,
                              void **vertexdeclaration)
{
    const D3DVERTEXELEMENT9 *d3delements = (const D3DVERTEXELEMENT9 *)elements;
    RwUInt32 numelements, n;
    HRESULT hr;

    RWFUNCTION(RWSTRING("_rpD3D9FindVertexDeclaration"));
    RWASSERT(elements != NULL);
    RWASSERT(vertexdeclaration != NULL);

    numelements = 0;
    while (d3delements[numelements].Type != D3DDECLTYPE_UNUSED)
    {
        numelements++;
    }
    numelements++;  /* Include the "end" element */
    RWASSERT(numelements <= 20);

    for (n = 0; n < NumVertexDeclarations; n++)
    {
        const _rxD3D9VertexDeclarationItem *currentItem = &(VertexDeclarations[n]);

        if (currentItem->numElements == numelements)
        {
            if (D3D9MemEqual((const RwUInt32 *)(currentItem->elements),
                             (const RwUInt32 *)d3delements,
                             numelements * sizeof(D3DVERTEXELEMENT9)))
            {
                break;
            }
        }
    }

    if (n >= NumVertexDeclarations)
    {
        if (NumVertexDeclarations >= MaxVertexDeclarations)
        {
            MaxVertexDeclarations += 16;

            if (VertexDeclarations == NULL)
            {
                VertexDeclarations = (_rxD3D9VertexDeclarationItem *)RwMalloc(MaxVertexDeclarations * sizeof(_rxD3D9VertexDeclarationItem),
                    rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL | rwMEMHINTFLAG_RESIZABLE);
            }
            else
            {
                VertexDeclarations = (_rxD3D9VertexDeclarationItem *)RwRealloc(VertexDeclarations,
                                               MaxVertexDeclarations * sizeof(_rxD3D9VertexDeclarationItem),
                                               rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL | rwMEMHINTFLAG_RESIZABLE);
            }
        }

        hr = DXCHECK(IDirect3DDevice9_CreateVertexDeclaration(_RwD3DDevice,
                                                elements,
                                                (IDirect3DVertexDeclaration9**)vertexdeclaration));

        if (SUCCEEDED(hr))
        {
            VertexDeclarations[NumVertexDeclarations].vertexdeclaration = *vertexdeclaration;

            VertexDeclarations[NumVertexDeclarations].numElements = numelements;

            VertexDeclarations[NumVertexDeclarations].elements = (D3DVERTEXELEMENT9 *)RwMalloc(numelements * sizeof(D3DVERTEXELEMENT9),
                rwID_DRIVERMODULE | rwMEMHINTDUR_GLOBAL);
            memcpy(VertexDeclarations[NumVertexDeclarations].elements, elements, numelements * sizeof(D3DVERTEXELEMENT9));

            NumVertexDeclarations++;
        }
        else
        {
            *vertexdeclaration = NULL;
        }
    }
    else
    {
        if (VertexDeclarations[n].vertexdeclaration != NULL)
        {
            IDirect3DVertexDeclaration9_AddRef(VertexDeclarations[n].vertexdeclaration);
        }
        else
        {
            DXCHECK(IDirect3DDevice9_CreateVertexDeclaration(_RwD3DDevice,
                                                             VertexDeclarations[n].elements,
                                                             &(VertexDeclarations[n].vertexdeclaration)));
        }

        *vertexdeclaration = VertexDeclarations[n].vertexdeclaration;
    }

    RWRETURN(*vertexdeclaration != NULL);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DeleteVertexDeclaration deletes the vertex declaration referred to by
 * the specified pointer and frees up the associated resources.
 *
 * \param vertexdeclaration Vertex declaration pointer, identifying the vertex declaration
 * to be deleted.
 *
 * \see RwD3D9CreateVertexDeclaration
 * \see RwD3D9SetVertexDeclaration
 *
 */
void
RwD3D9DeleteVertexDeclaration(void *vertexdeclaration)
{
    RwUInt32 n;

    RWAPIFUNCTION(RWSTRING("RwD3D9DeleteVertexDeclaration"));
    RWASSERT(vertexdeclaration != NULL);

    for (n = 0; n < NumVertexDeclarations; n++)
    {
        if (VertexDeclarations[n].vertexdeclaration == vertexdeclaration)
        {
            if (IDirect3DVertexDeclaration9_Release(VertexDeclarations[n].vertexdeclaration) <= 0)
            {
                VertexDeclarations[n].vertexdeclaration = NULL;
            }

            break;
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9CreateVertexShader creates a vertex shader and if created
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
 * \param function Pointer to the vertex shader function token array.
 * This parameter defines the operations to apply to each vertex.  This value
 * cannot be set as NULL.
 * \param shader Pointer to the returned vertex shader. This value
 * cannot be set as NULL.
 *
 * \return TRUE if the vertex shader was created successfully, FALSE otherwise.
 *
 * \see RwD3D9DeleteVertexShader
 * \see RwD3D9SetVertexShader
 * \see RwD3D9SetVertexShaderConstant
 *
 */
RwBool
RwD3D9CreateVertexShader(const RwUInt32 * function,
                         void **shader)
{
    HRESULT             hr = D3D_OK;

    RWAPIFUNCTION(RWSTRING("RwD3D9CreateVertexShader"));

    hr = DXCHECK(IDirect3DDevice9_CreateVertexShader(_RwD3DDevice,
                                                     (const DWORD *)function,
                                                     (LPVERTEXSHADER *)shader));

    if (SUCCEEDED(hr))
    {
        /* Force to reset any vertex shader again */
        _rwD3D9LastVertexShaderUsed = (void *)0xffffffff;
    }

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DeleteVertexShader deletes the vertex shader referred to by
 * the specified pointer and frees up the associated resources.
 *
 * \param shader Vertex shader pointer, identifying the vertex shader
 * to be deleted.
 *
 * \see RwD3D9CreateVertexShader
 * \see RwD3D9SetVertexShader
 * \see RwD3D9SetVertexShaderConstant
 *
 */
void
RwD3D9DeleteVertexShader(void *shader)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9DeleteVertexShader"));

    IDirect3DVertexShader9_Release((LPVERTEXSHADER)shader);

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetVertexShaderConstant sets values in the vertex constant array.
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
 * \see RwD3D9CreateVertexShader
 * \see RwD3D9DeleteVertexShader
 * \see RwD3D9SetVertexShader
 *
 */
#if defined(DOXYGEN)
void
RwD3D9SetVertexShaderConstant(RwUInt32 registerAddress,
                              const void *constantData,
                              RwUInt32 constantCount)
{
}
#endif
void
_rwD3D9SetVertexShaderConstant(RwUInt32 registerAddress,
                              const void *constantData,
                              RwUInt32 constantCount)
{
    RWAPIFUNCTION(RWSTRING("_rwD3D9SetVertexShaderConstant"));
    RWASSERT(registerAddress < _RwD3D9DeviceCaps.MaxVertexShaderConst);
    RWASSERT((registerAddress + constantCount) <= _RwD3D9DeviceCaps.MaxVertexShaderConst);

    DXCHECK(IDirect3DDevice9_SetVertexShaderConstantF(_RwD3DDevice,
                                                      registerAddress,
                                                      constantData,
                                                      constantCount));

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9CreatePixelShader creates a pixel shader.
 *
 * \param function Pointer to the pixel shader function token array,
 * specifying the blending operations. This value cannot be NULL.
 * \param shader Pointer to the returned pixel shader.
 *
 * \return TRUE if the pixel shader was created successfully, FALSE otherwise.
 *
 * \see RwD3D9DeletePixelShader
 * \see RwD3D9SetPixelShader
 * \see RwD3D9SetPixelShaderConstant
 *
 */
RwBool
RwD3D9CreatePixelShader(const RwUInt32 * function, void **shader)
{
    HRESULT             hr = D3D_OK;

    RWAPIFUNCTION(RWSTRING("RwD3D9CreatePixelShader"));

    hr = DXCHECK(IDirect3DDevice9_CreatePixelShader(_RwD3DDevice,
                                                    (const DWORD *)function,
                                                    (LPPIXELSHADER *)shader));

    RWRETURN(SUCCEEDED(hr));
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9DeletePixelShader deletes the pixel shader referred
 * to by the specified pointer.
 *
 * \param shader Pixel shader pointer, identifying the pixel shader
 * to be deleted.
 *
 * \see RwD3D9CreatePixelShader
 * \see RwD3D9SetPixelShader
 * \see RwD3D9SetPixelShaderConstant
 *
 */
void
RwD3D9DeletePixelShader(void *shader)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9DeletePixelShader"));

    IDirect3DPixelShader9_Release((LPPIXELSHADER)shader);

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetPixelShaderConstant sets the values in the pixel constant
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
 * \see RwD3D9CreatePixelShader
 * \see RwD3D9DeletePixelShader
 * \see RwD3D9SetPixelShader
 *
 */
#if defined(DOXYGEN)
void
RwD3D9SetPixelShaderConstant(RwUInt32 registerAddress,
                             const void *constantData,
                             RwUInt32 constantCount)
{
}
#endif
void
_rwD3D9SetPixelShaderConstant(RwUInt32 registerAddress,
                              const void *constantData,
                              RwUInt32 constantCount)
{
    RWAPIFUNCTION(RWSTRING("_rwD3D9SetPixelShaderConstant"));

    DXCHECK(IDirect3DDevice9_SetPixelShaderConstantF(_RwD3DDevice,
                                                     registerAddress,
                                                     constantData,
                                                     constantCount));

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9GetCaps return a const void pointer to a D3DCAPS9
 * structure for the current active device.
 *
 * Use this function in preference to obtaining the current D3D9
 * device since it will maintain RWD3D9's internal cache.
 *
 * The capabilities of the video card associated with the D3D9
 * device can be queried through the pointer to the structure returned
 * from this function. See the D3D9 API documentation for more
 * information.
 *
 * \return Void pointer to the D3DCAPS9 structure containing the
 * relevant capability information.
 *
 * \see RwD3D9GetCurrentD3DDevice
 *
 */
const void *
RwD3D9GetCaps(void)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetCaps"));

    RWRETURN(&_RwD3D9DeviceCaps);
}

/**
 * \ingroup rwcamerad3d9
 * \ref RwD3D9CameraIsSphereFullyInsideFrustum can be used to determine
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
RwD3D9CameraIsSphereFullyInsideFrustum(const void *camera,
                                       const void *sphere)
{
    const RwCamera     *Camera = (const RwCamera *) camera;
    const RwSphere     *Sphere = (const RwSphere *) sphere;
    const RwFrustumPlane *frustumPlane;
    RwInt32             numPlanes;

    RWAPIFUNCTION(RWSTRING("RwD3D9CameraIsSphereFullyInsideFrustum"));
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
 * \ingroup rwcamerad3d9
 * \ref RwD3D9CameraIsBBoxFullyInsideFrustum can be used to determine
 * whether a bounding box lies entirely within a camera's frustum.
 *
 * \param camera Pointer to the camera with the frustum to be tested against.
 * \param boundingBox Pointer to the bounding box that is to be tested.
 *
 * \return Returns TRUE if the bounding box is fully inside the frustum, FALSE
 * otherwise.
 *
 * \see RwD3D9CameraIsSphereFullyInsideFrustum
 *
 */
RwBool
RwD3D9CameraIsBBoxFullyInsideFrustum(const void *camera,
                                     const void *boundingBox)
{
    const RwCamera     *Camera = (const RwCamera *) camera;
    const RwFrustumPlane *frustumPlane;
    RwInt32             numPlanes;

    RWAPIFUNCTION(RWSTRING("RwD3D9CameraIsBBoxFullyInsideFrustum"));
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

    RWRETURN(_rwD3D9CPUSupportsMMX);
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

    RWRETURN(_rwD3D9CPUSupportsSSE);
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

    RWRETURN(_rwD3D9CPUSupportsSSE2);
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

    RWRETURN(_rwD3D9CPUSupports3DNow);
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * _rwD3D9DeviceSetReleaseCallback
 *
 */
void
_rwD3D9DeviceSetReleaseCallback(rwD3D9DeviceReleaseCallBack callback)
{
    RWFUNCTION(RWSTRING("_rwD3D9DeviceSetReleaseCallback"));

    D3D9ReleaseDeviceCallback = callback;

    RWRETURNVOID();
}

/**
 * _rwD3D9DeviceGetReleaseCallback
 *
 */
rwD3D9DeviceReleaseCallBack
_rwD3D9DeviceGetReleaseCallback(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9DeviceGetReleaseCallback"));

    RWRETURN(D3D9ReleaseDeviceCallback);
}

/**
 * _rwD3D9DeviceSetRestoreCallback
 *
 */
void
_rwD3D9DeviceSetRestoreCallback(rwD3D9DeviceRestoreCallBack callback)
{
    RWFUNCTION(RWSTRING("_rwD3D9DeviceSetRestoreCallback"));

    D3D9RestoreDeviceCallback = callback;

    RWRETURNVOID();
}

/**
 * _rwD3D9DeviceGetRestoreCallback
 *
 */
rwD3D9DeviceRestoreCallBack
_rwD3D9DeviceGetRestoreCallback(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9DeviceGetRestoreCallback"));

    RWRETURN(D3D9RestoreDeviceCallback);
}
