/***************************************************************************
 *                                                                         *
 * Module  : D3D9rendst.c                                                    *
 *                                                                         *
 * Purpose : Renderstates.                                                 *
 *                                                                         *
 **************************************************************************/

 /**************************************************************************
 Includes
 */
#include <d3d9.h>

#include "batypes.h"
#include "badebug.h"
#include "batextur.h"
#include "baimage.h"
#include "baresour.h"
#include "bamatrix.h"
#include "baimmedi.h"
#include "barwtyp.h"
#include "drvmodel.h"
#include "p2stdcls.h"

#include "drvfns.h"
#include "d3d9device.h"
#include "d3d9rendst.h"
#include "d3d9raster.h"
#include "d3d92drend.h"

#if defined( RWMETRICS )
#include "d3d9metric.h"
#endif /* defined( RWMETRICS ) */

#if defined( RWDEBUG )
#include "d3d9convrt.h"
#endif

/****************************************************************************
 Local Defines
 */

#define FLOATASINT(f) (*((const RwInt32 *)&(f)))
#define FLOATASDWORD(fvariable) (*((const DWORD *)&(fvariable)))

#define CONVERTTOZDEVICE(zvalue, nearPlane, farPlane) (((zvalue - nearPlane) * farPlane) / ((farPlane - nearPlane) * zvalue))
#define CONVERTTOZDEVICEPARALLEL(zvalue, nearPlane, farPlane) ((zvalue - nearPlane) / (farPlane - nearPlane))

#define CONSTTEXTUREGETANISOT(tex) \
    (*((const RwUInt8*)(((const RwUInt8*)tex) + D3D9AnisotTextureOffset)))


#define COLORSCALAR 0.003921568627450980392156862745098f /* 1.0f/ 255.0f */

/****************************************************************************
 Global Types
 */
typedef union D3D9UnionParam D3D9UnionParam;
union D3D9UnionParam
{
    RwBlendFunction     BlendFunction;
    RwBool              Bool;
    RwCullMode          CullMode;
    RwFogType           FogType;
    RwInt32             Int32;
    RwRaster           *RasterPtr;
    RwReal             *RealPtr;
    RwShadeMode         ShadeMode;
    RwStencilFunction   StencilFunction;
    RwStencilOperation  StencilOperation;
    RwTextureAddressMode TextureAddressMode;
    RwTextureFilterMode TextureFilterMode;
    RwUInt32            UInt32;
    void               *VoidPtr;
    RwAlphaTestFunction AlphaTestFunction;
};

typedef struct _rwD3D9StageStateCache _rwD3D9StageStateCache;
struct _rwD3D9StageStateCache
{
    RwRaster                *curTexRaster;
    RwTextureAddressMode    texAddressU;
    RwTextureAddressMode    texAddressV;
    RwTextureFilterMode     filterMode;
    RwUInt32                borderColor;
    RwInt32                 maxAnisotropy;
};

typedef struct _rwD3D9StateCache _rwD3D9StateCache;
struct _rwD3D9StateCache
{
    /* Z Buffer */
    RwBool                  zWriteEnable;
    RwBool                  zTestEnable;

    /* Stencil Buffer */
    RwBool                  stencilEnable;
    RwStencilOperation      stencilFailOp;
    RwStencilOperation      stencilZFailOp;
    RwStencilOperation      stencilPassOp;
    RwStencilFunction       stencilFunction;
    RwInt32                 stencilRef;
    RwInt32                 stencilMask;
    RwInt32                 stencilWriteMask;

    /* Alpha blending */
    RwBool                  vertexAlphaBlendEnable;
    RwBool                  textureAlphaBlendEnable;

    /* Culling mode */
    RwCullMode              cullMode;

    /* Fog */
    RwBool                  fogEnable;
    RwFogType               fogType;
    RwReal                  fogDensity;
    RwUInt32                fogColor;

    /* Shading mode */
    RwShadeMode             shadeMode;

    /* Texturing stuff */
    _rwD3D9StageStateCache  stage[RWD3D9_MAX_TEXTURE_STAGES];

    /* SRC, DST blending modes */
    RwBlendFunction         srcBlend;
    RwBlendFunction         dstBlend;

    /* Alpha Test */
    RwAlphaTestFunction     alphaTestFunction;
    RwBool                  alphaTestEnable;
};

/****************************************************************************
 Globals (across program)
 */

static _rwD3D9StateCache  _RwD3D9StateCache;

static RwInt32 D3D9AnisotTextureOffset = 0; /* 0 means deactivated */

/****************************************************************************
 Local (static) Globals
 */

#define NUMSHADEMODES   3
static const D3DSHADEMODE _RwD3D9ShadeModeConvTable[NUMSHADEMODES] =
{
    (D3DSHADEMODE)0,
    D3DSHADE_FLAT,
    D3DSHADE_GOURAUD
};

#define NUMFOGMODES     4
static const D3DFOGMODE _RwD3D9FogConvTable[NUMFOGMODES] =
{
    D3DFOG_NONE,
    D3DFOG_LINEAR,
    D3DFOG_EXP,
    D3DFOG_EXP2
};

#define NUMBLENDMODES   12
static const D3DBLEND _RwD3D9BlendConvTable[NUMBLENDMODES] =
{
    (D3DBLEND)0,
    D3DBLEND_ZERO,
    D3DBLEND_ONE,
    D3DBLEND_SRCCOLOR,
    D3DBLEND_INVSRCCOLOR,
    D3DBLEND_SRCALPHA,
    D3DBLEND_INVSRCALPHA,
    D3DBLEND_DESTALPHA,
    D3DBLEND_INVDESTALPHA,
    D3DBLEND_DESTCOLOR,
    D3DBLEND_INVDESTCOLOR,
    D3DBLEND_SRCALPHASAT
};

#define NUMADDRESSMODES 5
static const D3DTEXTUREADDRESS _RwD3D9AddressConvTable[NUMADDRESSMODES] =
{
    (D3DTEXTUREADDRESS)0,
    D3DTADDRESS_WRAP,
    D3DTADDRESS_MIRROR,
    D3DTADDRESS_CLAMP,
    D3DTADDRESS_BORDER
};

typedef struct __rwFilterParams _rwFilterParams;
struct __rwFilterParams
{
    D3DTEXTUREFILTERTYPE minmag;
    D3DTEXTUREFILTERTYPE mip;
};

#define NUMFILTERMODES  7
static const _rwFilterParams _RwD3D9FilterModeConvTable[NUMFILTERMODES + NUMFILTERMODES] =
{
    {D3DTEXF_NONE,   D3DTEXF_NONE},
    {D3DTEXF_POINT,  D3DTEXF_NONE},
    {D3DTEXF_LINEAR, D3DTEXF_NONE},
    {D3DTEXF_POINT,  D3DTEXF_POINT},
    {D3DTEXF_LINEAR, D3DTEXF_POINT},
    {D3DTEXF_POINT,  D3DTEXF_LINEAR},
    {D3DTEXF_LINEAR, D3DTEXF_LINEAR},
    /* Anisotropic modes */
    {D3DTEXF_ANISOTROPIC, D3DTEXF_NONE},
    {D3DTEXF_ANISOTROPIC, D3DTEXF_NONE},
    {D3DTEXF_ANISOTROPIC, D3DTEXF_NONE},
    {D3DTEXF_ANISOTROPIC, D3DTEXF_POINT},
    {D3DTEXF_ANISOTROPIC, D3DTEXF_POINT},
    {D3DTEXF_ANISOTROPIC, D3DTEXF_LINEAR},
    {D3DTEXF_ANISOTROPIC, D3DTEXF_LINEAR}
};

#define NUMCULLMODES    4
static const D3DCULL _RwD3D9CullModeConvTable[NUMCULLMODES] =
{
    (D3DCULL)0,
    D3DCULL_NONE,
    D3DCULL_CW,
    D3DCULL_CCW
};

#define NUMSTENCILOPERATIONS    9
static const D3DSTENCILOP _RwD3D9StencilOpConvTable[NUMSTENCILOPERATIONS] =
{
    (D3DSTENCILOP)0,
    D3DSTENCILOP_KEEP,
    D3DSTENCILOP_ZERO,
    D3DSTENCILOP_REPLACE,
    D3DSTENCILOP_INCRSAT,
    D3DSTENCILOP_DECRSAT,
    D3DSTENCILOP_INVERT,
    D3DSTENCILOP_INCR,
    D3DSTENCILOP_DECR
};

#define NUMCMPFUNCTIONS    9
static const D3DCMPFUNC _RwD3D9CmpFuncConvTable[NUMCMPFUNCTIONS] =
{
    (D3DCMPFUNC)0,
    D3DCMP_NEVER,
    D3DCMP_LESS,
    D3DCMP_EQUAL,
    D3DCMP_LESSEQUAL,
    D3DCMP_GREATER,
    D3DCMP_NOTEQUAL,
    D3DCMP_GREATEREQUAL,
    D3DCMP_ALWAYS
};

/*
 * Render States Cache
 */
#define MAX_RENDERSTATE_CACHE   210

typedef struct
{
    RwUInt32 value;
    RwBool changed;
} RxStateCache;

static RxStateCache RenderStateCache[MAX_RENDERSTATE_CACHE];

static RwUInt32 RenderStateD3D[MAX_RENDERSTATE_CACHE];

static RwUInt32 NumRenderStateChanges=0;
static D3DRENDERSTATETYPE RenderStateChanges[MAX_RENDERSTATE_CACHE];

#if defined(RWDEBUG)
static RwUInt32 RedundantRenderStates=0;
#endif /* defined(RWDEBUG) */

#define FORCE_RENDERSTATE(state, newvalue)                          \
{                                                                   \
    RenderStateCache[state].value = newvalue;                       \
    if (!RenderStateCache[state].changed)                           \
    {                                                               \
        RenderStateCache[state].changed = TRUE;                     \
        RWASSERT(NumRenderStateChanges < MAX_RENDERSTATE_CACHE);    \
        RenderStateChanges[NumRenderStateChanges++] = state;        \
    }                                                               \
}

/*
 * Texture Stage States Cache
 */
#define MAX_TEXTURESTAGESTATE_CACHE   33

static RxStateCache TextureStageStateCache[RWD3D9_MAX_TEXTURE_STAGES][MAX_TEXTURESTAGESTATE_CACHE];

static RwUInt32 TextureStageStateD3D[RWD3D9_MAX_TEXTURE_STAGES][MAX_TEXTURESTAGESTATE_CACHE];

typedef struct
{
    RwUInt32 stage;
    D3DTEXTURESTAGESTATETYPE type;
} RxTextureStageChangeType;

static RwUInt32 NumTextureStageStateChanges=0;
static RxTextureStageChangeType TextureStageStateChanges[RWD3D9_MAX_TEXTURE_STAGES * MAX_TEXTURESTAGESTATE_CACHE];

#if defined(RWDEBUG)
static RwUInt32 RedundantTextureStagesStates=0;
#endif /* defined(RWDEBUG) */

/*
 * Sampler Stage States Cache
 */
#define MAX_SAMPLERSTAGESTATE_CACHE   14

static RwUInt32 SamplerStageStateD3D[RWD3D9_MAX_TEXTURE_STAGES][MAX_SAMPLERSTAGESTATE_CACHE];

#if defined( RWMETRICS )
#define FORCE_SAMPLERSTATE(stage, type, newvalue)                           \
{                                                                           \
    SamplerStageStateD3D[stage][type] = newvalue;                           \
    IDirect3DDevice9_SetSamplerState(_RwD3DDevice, stage, type, newvalue);  \
    _rwD3D9MetricsGet()->numSamplerStageStateChanges++;                     \
}
#else
#define FORCE_SAMPLERSTATE(stage, type, newvalue)                           \
{                                                                           \
    SamplerStageStateD3D[stage][type] = newvalue;                           \
    IDirect3DDevice9_SetSamplerState(_RwD3DDevice, stage, type, newvalue);  \
}
#endif



/*
 * Material cache
 */
static D3DMATERIAL9 LastMaterial;

static RwUInt32 LastMaterialFlags = 0;
static RwUInt32 LastMaterialColor = 0;
static RwReal LastMaterialDiffuse = 0.0f;
static RwReal LastMaterialAmbient = 0.0f;
static RwRGBAReal LastMaterialAmbientSaturated;

/****************************************************************************
 D3D9MemEqual

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

/****************************************************************************
 D3D9RenderStateCacheClear

 */
static void
D3D9RenderStateCacheClear(void)
{
    RwUInt32 state;

    RWFUNCTION(RWSTRING("D3D9RenderStateCacheClear"));

    memset(RenderStateD3D, 0xffffffff, MAX_RENDERSTATE_CACHE * sizeof(RwUInt32));

    /* Change special values */
    RenderStateD3D[D3DRS_FOGCOLOR] = 0;
    RenderStateD3D[D3DRS_AMBIENT] = 0;
    RenderStateD3D[D3DRS_STENCILMASK] = 0;
    RenderStateD3D[D3DRS_STENCILWRITEMASK] = 0;

    /* Initialize the cache */
    for(state = 0; state < MAX_RENDERSTATE_CACHE; state++)
    {
        RenderStateCache[state].value = RenderStateD3D[state];
        RenderStateCache[state].changed = FALSE;
    }

    NumRenderStateChanges = 0;

    #if defined(RWDEBUG)
    RedundantRenderStates = 0;
    #endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/****************************************************************************
 D3D9TextureStageStateCacheClear

 */

static void
D3D9TextureStageStateCacheClear(void)
{
    RwUInt32 stage, type;

    RWFUNCTION(RWSTRING("D3D9TextureStageStateCacheClear"));

    memset(TextureStageStateD3D, 0xffffffff, RWD3D9_MAX_TEXTURE_STAGES * MAX_TEXTURESTAGESTATE_CACHE * sizeof(RwUInt32));

    /* Initialize the cache */
    for (stage = 0; stage < RWD3D9_MAX_TEXTURE_STAGES; stage++)
    {
        for (type = 0; type < MAX_TEXTURESTAGESTATE_CACHE; type++)
        {
            TextureStageStateCache[stage][type].value=0xffffffff;
            TextureStageStateCache[stage][type].changed=FALSE;
        }
    }

    NumTextureStageStateChanges = 0;

#if defined(RWDEBUG)
    RedundantTextureStagesStates = 0;
#endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/****************************************************************************
 D3D9SamplerStageStateCacheClear

 */
static void
D3D9SamplerStageStateCacheClear(void)
{
    RWFUNCTION(RWSTRING("D3D9SamplerStageStateCacheClear"));

    memset(SamplerStageStateD3D, 0xffffffff, RWD3D9_MAX_TEXTURE_STAGES * MAX_SAMPLERSTAGESTATE_CACHE * sizeof(RwUInt32));

    RWRETURNVOID();
}

/****************************************************************************
 D3D9ClearCacheMaterials

 On entry   : none

 */
static void
D3D9ClearCacheMaterials(void)
{
    RWFUNCTION(RWSTRING("D3D9ClearCacheMaterials"));

    memset(&LastMaterial, 0, sizeof(D3DMATERIAL9));

    LastMaterialFlags = 0;
    LastMaterialColor = 0;
    LastMaterialDiffuse = 0.0f;
    LastMaterialAmbient = 0.0f;
    LastMaterialAmbientSaturated.red = 0.0f;
    LastMaterialAmbientSaturated.green = 0.0f;
    LastMaterialAmbientSaturated.blue = 0.0f;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9RenderStateFlushCache

 */
void
_rwD3D9RenderStateFlushCache()
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateFlushCache"));

    if (NumRenderStateChanges)
    {
        RwUInt32 n;
        #if defined( RWMETRICS )
        RwUInt32 numRenderStateChangesNotRedundant = NumRenderStateChanges;
        #endif

        n = 0;
        do
        {
            const D3DRENDERSTATETYPE stateChanged = RenderStateChanges[n];
            const RwUInt32 newValue = RenderStateCache[stateChanged].value;

            RenderStateCache[stateChanged].changed = FALSE;

            if (RenderStateD3D[stateChanged] != newValue)
            {
                RenderStateD3D[stateChanged] = newValue;

                IDirect3DDevice9_SetRenderState(_RwD3DDevice, stateChanged, newValue);
            }
            else
            {
                #if defined(RWDEBUG)
                RedundantRenderStates++;
                #endif /* defined(RWDEBUG) */

                #if defined( RWMETRICS )
                numRenderStateChangesNotRedundant--;
                #endif
            }
        }
        while(++n < NumRenderStateChanges);

        #if defined( RWMETRICS )
        _rwD3D9MetricsGet()->numRenderStateChanges += numRenderStateChangesNotRedundant;
        #endif

        NumRenderStateChanges = 0;
    }

    if (NumTextureStageStateChanges)
    {
        RwUInt32 n;
        #if defined( RWMETRICS )
        RwUInt32 numTextureStageStateChangesNotRedundant = NumTextureStageStateChanges;
        #endif

        n = 0;
        do
        {
            const RwUInt32 stage = TextureStageStateChanges[n].stage;
            const D3DTEXTURESTAGESTATETYPE type = TextureStageStateChanges[n].type;
            const RwUInt32 newValue = TextureStageStateCache[stage][type].value;

            TextureStageStateCache[stage][type].changed = FALSE;

            if (TextureStageStateD3D[stage][type] != newValue)
            {
                TextureStageStateD3D[stage][type] = newValue;

                IDirect3DDevice9_SetTextureStageState(_RwD3DDevice, stage, type, newValue);
            }
            else
            {
                #if defined(RWDEBUG)
                RedundantTextureStagesStates++;
                #endif /* defined(RWDEBUG) */

                #if defined( RWMETRICS )
                numTextureStageStateChangesNotRedundant--;
                #endif
            }
        }
        while(++n < NumTextureStageStateChanges);

        #if defined( RWMETRICS )
        _rwD3D9MetricsGet()->numTextureStageStateChanges += numTextureStageStateChangesNotRedundant;
        #endif

        NumTextureStageStateChanges = 0;
    }

#if defined(RWDEBUG)
    RedundantRenderStates=0;

    RedundantTextureStagesStates=0;
#endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/**
 * \ingroup d3d9renderstates
 * \ref RwD3D9SetRenderState sets a device renderstate parameter.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetRenderState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param state Device state variable that is being modified. This parameter
 * can be any member of the D3DRENDERSTATETYPE enumerated type.
 * \param value New value for the device renderstate to be set.
 * The meaning of this parameter is dependent on the value specified
 * for \e state.
 *
 * \see RwD3D9GetRenderState
 * \see RwD3D9SetSamplerState
 * \see RwD3D9GetSamplerStageState
 * \see RwD3D9SetTextureStageState
 * \see RwD3D9GetTextureStageState
 *
 */
void
RwD3D9SetRenderState(RwUInt32 state, RwUInt32 value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetRenderState"));

    RWASSERT(state < MAX_RENDERSTATE_CACHE);

    if (RenderStateCache[state].value != value)
    {
        RenderStateCache[state].value = value;

        if (!RenderStateCache[state].changed)
        {
            RenderStateCache[state].changed = TRUE;

            RWASSERT(NumRenderStateChanges < MAX_RENDERSTATE_CACHE);
            RenderStateChanges[NumRenderStateChanges++] =
                (D3DRENDERSTATETYPE)state;
        }
        #if defined(RWDEBUG)
        else
        {
            RedundantRenderStates++;
        }
        #endif /* defined(RWDEBUG) */
    }
    #if defined(RWDEBUG)
    else
    {
        RedundantRenderStates++;
    }
    #endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/**
 * \ingroup d3d9renderstates
 * \ref RwD3D9GetRenderState retrieves a renderstate value.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::GetRenderState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param state Device state variable that is being queried. This parameter
 * can be any member of the D3DRENDERSTATETYPE enumerated type.
 * \param value Void pointer to a variable that receives the value of
 * the queried renderstate variable when the function returns.
 *
 * \see RwD3D9SetRenderState
 * \see RwD3D9SetSamplerState
 * \see RwD3D9GetSamplerStageState
 * \see RwD3D9SetTextureStageState
 * \see RwD3D9GetTextureStageState
 *
 */
void
RwD3D9GetRenderState(RwUInt32 state, void *value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetRenderState"));

    RWASSERT(state < MAX_RENDERSTATE_CACHE);

    *((RwUInt32 *)value) = RenderStateCache[state].value;

    RWRETURNVOID();
}

/**
 * \ingroup d3d9renderstates
 * \ref RwD3D9SetTextureStageState sets the state value for the currently
 * assigned texture in the specified stage.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetTextureStageState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param stage Stage identifier of the texture for which the state value
 * is set. Stage identifiers are zero-based. Devices can have up to 8
 * textures set, so the maximum value allowed for \e stage is 7.
 * \param type Texture state to set. This parameter can be any member
 * of the D3DTEXTURESTAGESTATETYPE enumerated type.
 * \param value State value to set. The meaning of this value is determined
 * by the \e type parameter.
 *
 * \see RwD3D9SetSamplerState
 * \see RwD3D9GetSamplerStageState
 * \see RwD3D9GetTextureStageState
 * \see RwD3D9SetTexture
 * \see RwD3D9SetRenderState
 * \see RwD3D9GetRenderState
 *
 */
void
RwD3D9SetTextureStageState(RwUInt32 stage, RwUInt32 type, RwUInt32 value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetTextureStageState"));

    RWASSERT((stage < RWD3D9_MAX_TEXTURE_STAGES) && (type < MAX_TEXTURESTAGESTATE_CACHE));

    if (TextureStageStateCache[stage][type].value != value)
    {
        TextureStageStateCache[stage][type].value = value;

        if(!TextureStageStateCache[stage][type].changed)
        {
            TextureStageStateCache[stage][type].changed = TRUE;

            RWASSERT(NumTextureStageStateChanges <
                     (RWD3D9_MAX_TEXTURE_STAGES * MAX_TEXTURESTAGESTATE_CACHE));

            TextureStageStateChanges[NumTextureStageStateChanges].stage =
                stage;
            TextureStageStateChanges[NumTextureStageStateChanges].type =
                (D3DTEXTURESTAGESTATETYPE)type;
            NumTextureStageStateChanges++;
        }
        #if defined(RWDEBUG)
        else
        {
            RedundantTextureStagesStates++;
        }
        #endif /* defined(RWDEBUG) */
    }
    #if defined(RWDEBUG)
    else
    {
        RedundantTextureStagesStates++;
    }
    #endif /* defined(RWDEBUG) */

    RWRETURNVOID();
}

/**
 * \ingroup d3d9renderstates
 * \ref RwD3D9GetTextureStageState retrieves a state value for an assigned
 * texture in the specified stage.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::GetTextureStageState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param stage Stage identifier of the texture for which the state value
 * is retrieved. Stage identifiers are zero-based. Devices can have up to 8
 * textures set, so the maximum value allowed for \e stage is 7.
 * \param type Texture state to retrieve. This parameter can be any member
 * of the D3DTEXTURESTAGESTATETYPE enumerated type.
 * \param value Void pointer to a variable to fill with the retrieved state
 * value. The meaning of the retrieved value is determined by the \e type parameter.
 *
 * \see RwD3D9SetTextureStageState
 * \see RwD3D9SetTexture
 * \see RwD3D9SetRenderState
 * \see RwD3D9GetRenderState
 * \see RwD3D9SetSamplerState
 * \see RwD3D9GetSamplerStageState
 *
 */
void
RwD3D9GetTextureStageState(RwUInt32 stage, RwUInt32 type, void *value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetTextureStageState"));

    RWASSERT((stage < RWD3D9_MAX_TEXTURE_STAGES) && (type <MAX_TEXTURESTAGESTATE_CACHE));

    *((RwUInt32 *)value) = TextureStageStateCache[stage][type].value;

    RWRETURNVOID();
}

/**
 * \ingroup d3d9renderstates
 * \ref RwD3D9SetSamplerState sets the sampler state value for the currently
 * assigned texture in the specified stage.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetSamplerStageState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param stage Stage identifier of the texture for which the state value
 * is set. Stage identifiers are zero-based. Devices can have up to 8
 * textures set, so the maximum value allowed for \e stage is 7.
 * \param type Sampler state to set. This parameter can be any member
 * of the D3DSAMPLERSTATETYPE enumerated type.
 * \param value State value to set. The meaning of this value is determined
 * by the \e type parameter.
 *
 * \see RwD3D9GetSamplerState
 * \see RwD3D9SetTextureStageState
 * \see RwD3D9GetTextureStageState
 * \see RwD3D9SetTexture
 * \see RwD3D9SetRenderState
 * \see RwD3D9GetRenderState
 *
 */
void
RwD3D9SetSamplerState(RwUInt32 stage, RwUInt32 type, RwUInt32 value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetSamplerState"));

    RWASSERT((stage < RWD3D9_MAX_TEXTURE_STAGES) && (type < MAX_SAMPLERSTAGESTATE_CACHE));

    if (SamplerStageStateD3D[stage][type] != value)
    {
        SamplerStageStateD3D[stage][type] = value;

        IDirect3DDevice9_SetSamplerState(_RwD3DDevice, stage, type, value);

#if defined( RWMETRICS )
        _rwD3D9MetricsGet()->numSamplerStageStateChanges++;
#endif
    }

    RWRETURNVOID();
}

/**
 * \ingroup d3d9renderstates
 * \ref RwD3D9GetSamplerState retrieves a state value for an assigned
 * sampler in the specified stage.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::GetSamplerStageState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param stage Stage identifier of the texture for which the sampler state value
 * is retrieved. Stage identifiers are zero-based. Devices can have up to 8
 * textures set, so the maximum value allowed for \e stage is 7.
 * \param type Sampler state to retrieve. This parameter can be any member
 * of the D3DSAMPLERSTATETYPE enumerated type.
 * \param value Void pointer to a variable to fill with the retrieved state
 * value. The meaning of the retrieved value is determined by the \e type parameter.
 *
 * \see RwD3D9SetTextureStageState
 * \see RwD3D9SetTexture
 * \see RwD3D9SetRenderState
 * \see RwD3D9GetRenderState
 * \see RwD3D9SetSamplerState
 * \see RwD3D9GetTextureStageState
 *
 */
void
RwD3D9GetSamplerState(RwUInt32 stage, RwUInt32 type, void *value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9GetSamplerState"));

    RWASSERT((stage < RWD3D9_MAX_TEXTURE_STAGES) && (type <MAX_SAMPLERSTAGESTATE_CACHE));

    *((RwUInt32 *)value) = SamplerStageStateD3D[stage][type];

    RWRETURNVOID();
}

/*
 *  _rwD3D9ForceRenderState
 */
void
_rwD3D9ForceRenderState(RwUInt32 state, RwUInt32 value)
{
    RWFUNCTION(RWSTRING("_rwD3D9ForceRenderState"));
    RWASSERT(state < MAX_RENDERSTATE_CACHE);

    RenderStateCache[state].value = value;
    RenderStateD3D[state] = value;

    IDirect3DDevice9_SetRenderState(_RwD3DDevice, state, value);

    #if defined( RWMETRICS )
    _rwD3D9MetricsGet()->numRenderStateChanges++;
    #endif

    RWRETURNVOID();
}

/*
 *  _rwD3D9ForceTextureStageState
 */
void
_rwD3D9ForceTextureStageState(RwUInt32 stage, RwUInt32 type, RwUInt32 value)
{
    RWFUNCTION(RWSTRING("_rwD3D9ForceTextureStageState"));
    RWASSERT((stage < RWD3D9_MAX_TEXTURE_STAGES) && (type < MAX_TEXTURESTAGESTATE_CACHE));

    TextureStageStateCache[stage][type].value = value;
    TextureStageStateD3D[stage][type] = value;

    IDirect3DDevice9_SetTextureStageState(_RwD3DDevice, stage, type, value);

    #if defined( RWMETRICS )
    _rwD3D9MetricsGet()->numTextureStageStateChanges++;
    #endif

    RWRETURNVOID();
}

/*
 *  _rwD3D9ForceSamplerState
 */
void
_rwD3D9ForceSamplerState(RwUInt32 stage, RwUInt32 type, RwUInt32 value)
{
    RWFUNCTION(RWSTRING("_rwD3D9ForceSamplerState"));
    RWASSERT((stage < RWD3D9_MAX_TEXTURE_STAGES) && (type <MAX_SAMPLERSTAGESTATE_CACHE));

    SamplerStageStateD3D[stage][type] = value;

    IDirect3DDevice9_SetSamplerState(_RwD3DDevice, stage, type, value);

#if defined( RWMETRICS )
    _rwD3D9MetricsGet()->numSamplerStageStateChanges++;
#endif

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetMaterial sets the material properties to be used in
 * subsequent rendering operations.
 *
 * It is preferred that this utility function is used rather than the D3D9
 * API function IDirect3DDevice9::SetMaterial since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d9cache.
 *
 * \param material Void pointer to a D3DMATERIAL9 structure, describing
 * the material properties to set.
 *
 * \return TRUE if the material was set, FALSE otherwise.
 *
 */
RwBool
RwD3D9SetMaterial(const void *material)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetMaterial"));

    if (!D3D9MemEqual
        ((const RwUInt32 *) &LastMaterial, (const RwUInt32 *) material,
         sizeof(D3DMATERIAL9)))
    {
        HRESULT             hr;

        memcpy(&LastMaterial, material, sizeof(D3DMATERIAL9));

        LastMaterialFlags = 0;

        hr =
            DXCHECK(IDirect3DDevice9_SetMaterial(_RwD3DDevice,
                                                 (const D3DMATERIAL9 *)material));

#if defined( RWMETRICS )
        _rwD3D9MetricsGet()->numMaterialChanges++;
#endif

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetClipPlane sets the coefficients of a user-defined
 * clipping plane.
 *
 * The coefficients that this method sets take the form of the general
 * plane equation. If the values in \e plane were labeled
 * A, B, C, and D in the order that they appear in \ref RwV4d,
 * they would fit into the general plane equation so that Ax + By + Cz + Dw = 0.
 * A point with homogeneous coordinates (x, y, z, w) is visible in the
 * half space of the plane if Ax + By + Cz + Dw >= 0. Points that exist
 * behind the clipping plane are clipped from the scene. When the fixed
 * function pipeline is used the plane equations are assumed to be in world space.
 * When the programmable pipeline is used the plane equations are assumed
 * to be in the clipping space (the same space as output vertices).
 *
 * \note This method does not enable the clipping plane equation being set.
 * To enable a clipping plane, set the corresponding bit in the value applied
 * to the D3DRS_CLIPPLANEENABLE render state. It is highly recomended to disable
 * the clipping plane when it is no longer useful.
 *
 * \param index Index of the clipping plane for which the plane equation
 * coefficients are to be set.
 * \param plane Pointer to a \ref RwV4d that represent the clipping plane
 * coefficients to be set, in the form of the general plane equation.
 *
 * \return TRUE if the clip plane was set, FALSE otherwise.
 *
 * \see RwD3D9SetRenderState
 *
 */
RwBool
RwD3D9SetClipPlane(RwUInt32 index,
                   const RwV4d *plane)
{
    HRESULT hr;

    RWAPIFUNCTION(RWSTRING("RwD3D9SetClipPlane"));

    RWASSERT(index < _RwD3D9DeviceCaps.MaxUserClipPlanes);

    hr = IDirect3DDevice9_SetClipPlane(_RwD3DDevice, index, (const RwReal *)plane);

    RWRETURN(SUCCEEDED(hr));
}


/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetSurfaceProperties sets the surface properties to be used in
 * subsequent rendering operations.
 *
 * This function internally converts the RenderWare \ref RwSurfaceProperties
 * to the equivalent D3D9 D3DMATERIAL9 structure, and sets this as the current
 * material for subsequent rendering operations.
 *
 * \param surfaceProps Pointer to a \ref RwSurfaceProperties structure
 * with the surface properties to be set.
 * \param color Pointer to a \ref RwRGBA structure
 * with the color to be set.
 * \param flags Geometry flags.
 *
 * \return TRUE if the surface properties were set, FALSE otherwise.
 *
 */
RwBool
RwD3D9SetSurfaceProperties(const RwSurfaceProperties *surfaceProps,
                           const RwRGBA *color,
                           RwUInt32 flags)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetSurfaceProperties"));
    RWASSERT(surfaceProps != NULL);
    RWASSERT(color != NULL);
    RWASSERT((flags & rxGEOMETRY_LIGHT) != 0);

    /* Keep only useful flags */
    flags &= (rxGEOMETRY_MODULATE | rxGEOMETRY_PRELIT);

    if ( FLOATASINT(LastMaterialDiffuse) != FLOATASINT(surfaceProps->diffuse) ||
         FLOATASINT(LastMaterialAmbient) != FLOATASINT(surfaceProps->ambient) ||
         LastMaterialColor != *((const RwUInt32 *)color) ||
         LastMaterialFlags != flags ||
         FLOATASINT(LastMaterialAmbientSaturated.red) != FLOATASINT(AmbientSaturated.red) ||
         FLOATASINT(LastMaterialAmbientSaturated.green) != FLOATASINT(AmbientSaturated.green) ||
         FLOATASINT(LastMaterialAmbientSaturated.blue) != FLOATASINT(AmbientSaturated.blue) )
    {
        HRESULT             hr;

        LastMaterialDiffuse = surfaceProps->diffuse;
        LastMaterialAmbient = surfaceProps->ambient;
        LastMaterialColor = *((const RwUInt32 *)color);
        LastMaterialFlags = flags;
        LastMaterialAmbientSaturated.red = AmbientSaturated.red;
        LastMaterialAmbientSaturated.green = AmbientSaturated.green;
        LastMaterialAmbientSaturated.blue = AmbientSaturated.blue;

        if ((flags & rxGEOMETRY_MODULATE) &&
            *((const RwUInt32 *)color) != 0xffffffff)
        {
            RwReal coef;

            coef = surfaceProps->diffuse * COLORSCALAR;

            LastMaterial.Diffuse.r = (color->red) * coef;
            LastMaterial.Diffuse.g = (color->green) * coef;
            LastMaterial.Diffuse.b = (color->blue) * coef;
            LastMaterial.Diffuse.a = (color->alpha) * COLORSCALAR;

            coef = surfaceProps->ambient * COLORSCALAR;

            if (flags & rxGEOMETRY_PRELIT)
            {
                RwD3D9SetRenderState(D3DRS_AMBIENT, ((D3DCOLOR)((((RwUInt32)color->alpha)<<24)|(((RwUInt32)color->red)<<16)|(((RwUInt32)color->green)<<8)|((RwUInt32)color->blue))));
                RwD3D9SetRenderState(D3DRS_COLORVERTEX, TRUE);
                RwD3D9SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
                RwD3D9SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

                LastMaterial.Ambient.r = 0.0f;
                LastMaterial.Ambient.g = 0.0f;
                LastMaterial.Ambient.b = 0.0f;
                LastMaterial.Ambient.a = 1.0f;

                LastMaterial.Emissive.r = (color->red) * AmbientSaturated.red * coef;
                LastMaterial.Emissive.g = (color->green) * AmbientSaturated.green * coef;
                LastMaterial.Emissive.b = (color->blue) * AmbientSaturated.blue * coef;
                LastMaterial.Emissive.a = 1.0f;
            }
            else
            {
                RwD3D9SetRenderState(D3DRS_AMBIENT, 0xffffffff);
                RwD3D9SetRenderState(D3DRS_COLORVERTEX, FALSE);
                RwD3D9SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
                RwD3D9SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

                LastMaterial.Ambient.r = (color->red) * AmbientSaturated.red * coef;
                LastMaterial.Ambient.g = (color->green) * AmbientSaturated.green * coef;
                LastMaterial.Ambient.b = (color->blue) * AmbientSaturated.blue * coef;
                LastMaterial.Ambient.a = 1.0f;

                LastMaterial.Emissive.r = 0.0f;
                LastMaterial.Emissive.g = 0.0f;
                LastMaterial.Emissive.b = 0.0f;
                LastMaterial.Emissive.a = 1.0f;
            }
        }
        else
        {
            LastMaterial.Diffuse.b = LastMaterial.Diffuse.g =
                LastMaterial.Diffuse.r = surfaceProps->diffuse;

            LastMaterial.Diffuse.a = 1.f;

            RwD3D9SetRenderState(D3DRS_AMBIENT, 0xffffffff);
            RwD3D9SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

            if (flags & rxGEOMETRY_PRELIT)
            {
                /* Emmisive color from the vertex colors */
                RwD3D9SetRenderState(D3DRS_COLORVERTEX, TRUE);
                RwD3D9SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1);
            }
            else
            {
                /* Emmisive color from material, set to black */
                RwD3D9SetRenderState(D3DRS_COLORVERTEX, FALSE);
                RwD3D9SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
            }
            LastMaterial.Emissive.r = 0.0f;
            LastMaterial.Emissive.g = 0.0f;
            LastMaterial.Emissive.b = 0.0f;
            LastMaterial.Emissive.a = 1.0f;

            if (surfaceProps->ambient != 1.f)
            {
                LastMaterial.Ambient.r =
                    surfaceProps->ambient * AmbientSaturated.red;
                LastMaterial.Ambient.g =
                    surfaceProps->ambient * AmbientSaturated.green;
                LastMaterial.Ambient.b =
                    surfaceProps->ambient * AmbientSaturated.blue;
            }
            else
            {
                LastMaterial.Ambient.r = AmbientSaturated.red;
                LastMaterial.Ambient.g = AmbientSaturated.green;
                LastMaterial.Ambient.b = AmbientSaturated.blue;
            }
            LastMaterial.Ambient.a = 1.0f;
        }

        LastMaterial.Specular.r = 0.0f;
        LastMaterial.Specular.g = 0.0f;
        LastMaterial.Specular.b = 0.0f;
        LastMaterial.Specular.a = 1.0f;

        LastMaterial.Power = 0.0f;

        hr = DXCHECK(IDirect3DDevice9_SetMaterial(_RwD3DDevice, &LastMaterial));

#if defined( RWMETRICS )
        _rwD3D9MetricsGet()->numMaterialChanges++;
#endif

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwDlRenderStateOpen

 On entry   :
 On exit    :
 */
void
_rwD3D9RenderStateOpen(void)
{
    RwInt32 i, dither;

    RWFUNCTION(RWSTRING("_rwD3D9RenderStateOpen"));

    D3D9RenderStateCacheClear();
    D3D9TextureStageStateCacheClear();
    D3D9SamplerStageStateCacheClear();
    D3D9ClearCacheMaterials();

    /* Fog */
    _RwD3D9StateCache.fogEnable = FALSE;
    _RwD3D9StateCache.fogType = rwFOGTYPELINEAR;
    _RwD3D9StateCache.fogDensity = 1.0f;
    _RwD3D9StateCache.fogColor = 0;

    RwD3D9SetRenderState(D3DRS_FOGENABLE, FALSE);

    if ((_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0 &&
        (_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG) != 0)
    {
        RwD3D9SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
        RwD3D9SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    }
    else
    {
        RwD3D9SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
        RwD3D9SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    }

    RwD3D9SetRenderState(D3DRS_FOGCOLOR, 0);

    /* Shading mode */
    RwD3D9SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    _RwD3D9StateCache.shadeMode = rwSHADEMODEGOURAUD;

    /* Set Z Compare mode */
    RwD3D9SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    RwD3D9SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    RwD3D9SetRenderState(D3DRS_ZENABLE, TRUE);

    _RwD3D9StateCache.zWriteEnable = TRUE;
    _RwD3D9StateCache.zTestEnable = TRUE;

    /* Stencil */
    RwD3D9SetRenderState(D3DRS_STENCILENABLE, FALSE);
    RwD3D9SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    RwD3D9SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    RwD3D9SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
    RwD3D9SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    RwD3D9SetRenderState(D3DRS_STENCILREF, 0);
    RwD3D9SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
    RwD3D9SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);

    _RwD3D9StateCache.stencilEnable = FALSE;
    _RwD3D9StateCache.stencilFailOp = rwSTENCILOPERATIONKEEP;
    _RwD3D9StateCache.stencilZFailOp = rwSTENCILOPERATIONKEEP;
    _RwD3D9StateCache.stencilPassOp = rwSTENCILOPERATIONKEEP;
    _RwD3D9StateCache.stencilFunction = rwSTENCILFUNCTIONALWAYS;
    _RwD3D9StateCache.stencilRef = 0;
    _RwD3D9StateCache.stencilMask = 0xFFFFFFFF;
    _RwD3D9StateCache.stencilWriteMask = 0xFFFFFFFF;

    /* Texture stages */
    for (i = 0; i < RWD3D9_MAX_TEXTURE_STAGES; i++)
    {
        /* Texture */
        DXCHECK(IDirect3DDevice9_SetTexture(_RwD3DDevice, i, NULL));
        _RwD3D9StateCache.stage[i].curTexRaster = NULL;

        /* Texture filter mode - Liner/Trilinear */
        if(_RwD3D9DeviceCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR)
        {
            RwD3D9SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            RwD3D9SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            _RwD3D9StateCache.stage[i].filterMode = rwFILTERLINEAR;
        }
        else
        {
            RwD3D9SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
            RwD3D9SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            _RwD3D9StateCache.stage[i].filterMode = rwFILTERNEAREST;
        }

        RwD3D9SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

        /* Texture addressing mode */
        RwD3D9SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        RwD3D9SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

        _RwD3D9StateCache.stage[i].texAddressU = rwTEXTUREADDRESSWRAP;
        _RwD3D9StateCache.stage[i].texAddressV = rwTEXTUREADDRESSWRAP;

        /* Border color */
        RwD3D9SetSamplerState(i, D3DSAMP_BORDERCOLOR, 0xFF000000);
        _RwD3D9StateCache.stage[i].borderColor = 0xFF000000;

        /* Max Anisotropy Level: 1 to disable it */
        _RwD3D9StateCache.stage[i].maxAnisotropy = 1;
        RwD3D9SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 1);

        /* Disable stage if not the first one */
        if (i)
        {
            RwD3D9SetTextureStageState(i, D3DTSS_COLOROP,   D3DTOP_DISABLE);
            RwD3D9SetTextureStageState(i, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
        }
    }

    /* Set default blending modes */
    RwD3D9SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

    RwD3D9SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    _RwD3D9StateCache.srcBlend = rwBLENDSRCALPHA;
    _RwD3D9StateCache.dstBlend = rwBLENDINVSRCALPHA;

    /* Set up the alpha compare function for future use */
    RwD3D9SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    _RwD3D9StateCache.alphaTestFunction = rwALPHATESTFUNCTIONGREATER;
    _RwD3D9StateCache.alphaTestEnable = TRUE;

    RwD3D9SetRenderState(D3DRS_ALPHAREF, 0x00000000);

    /* Disable alpha blending */
    RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    _RwD3D9StateCache.vertexAlphaBlendEnable = FALSE;
    _RwD3D9StateCache.textureAlphaBlendEnable = FALSE;

    /* Turn lighting off */
    RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);

    /* Cull clockwise facing triangles */
    RwD3D9SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    _RwD3D9StateCache.cullMode = rwCULLMODECULLBACK;

    /* Vertex color only until a texture has been set */
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    /* Diffuse, Specular & Ambient colors come from the material */
    RwD3D9SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

    RwD3D9SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);

    RwD3D9SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

    /* Use dithering if possible */
    dither = (_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_DITHER) ? TRUE : FALSE;
    RwD3D9SetRenderState(D3DRS_DITHERENABLE, dither);

    RwD3D9SetRenderState(D3DRS_SPECULARENABLE, FALSE);

    RwD3D9SetRenderState(D3DRS_LOCALVIEWER, FALSE);

    RwD3D9SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

    _rwD3D9RenderStateFlushCache();

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9RenderStateReset

 On entry   :
 On exit    :
 */
void
_rwD3D9RenderStateReset(void)
{
    RwUInt32 fogStart, fogEnd;
    RwUInt32 alphaRef;
    RwInt32 i, dither;

    RWFUNCTION(RWSTRING("_rwD3D9RenderStateReset"));

    /* Save some parameters */
    fogStart = RenderStateCache[D3DRS_FOGSTART].value;
    fogEnd = RenderStateCache[D3DRS_FOGEND].value;

    alphaRef = RenderStateCache[D3DRS_ALPHAREF].value;

    /* Clear renderstate state */
    D3D9RenderStateCacheClear();
    D3D9TextureStageStateCacheClear();
    D3D9SamplerStageStateCacheClear();
    D3D9ClearCacheMaterials();

    /* Fog */
    RwD3D9SetRenderState(D3DRS_FOGDENSITY, FLOATASDWORD(_RwD3D9StateCache.fogDensity));

    RwD3D9SetRenderState(D3DRS_FOGENABLE, _RwD3D9StateCache.fogEnable);

    if ((_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0 &&
        (_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG) != 0)
    {
        RwD3D9SetRenderState(D3DRS_FOGTABLEMODE, _RwD3D9FogConvTable[_RwD3D9StateCache.fogType]);
        RwD3D9SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    }
    else
    {
        RwD3D9SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
        RwD3D9SetRenderState(D3DRS_FOGVERTEXMODE, _RwD3D9FogConvTable[_RwD3D9StateCache.fogType]);
    }

    RwD3D9SetRenderState(D3DRS_FOGSTART, fogStart);

    RwD3D9SetRenderState(D3DRS_FOGEND, fogEnd);

    RwD3D9SetRenderState(D3DRS_FOGCOLOR, _RwD3D9StateCache.fogColor);

    /* Shading mode */
    RwD3D9SetRenderState(D3DRS_SHADEMODE, _RwD3D9ShadeModeConvTable[_RwD3D9StateCache.shadeMode]);

    /* Set Z Compare mode */
    RwD3D9SetRenderState(D3DRS_ZWRITEENABLE, _RwD3D9StateCache.zWriteEnable);

    if (_RwD3D9StateCache.zTestEnable)
    {
        RwD3D9SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    }
    else
    {
        RwD3D9SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    }

    if (_RwD3D9StateCache.zTestEnable || _RwD3D9StateCache.zWriteEnable)
    {
        RwD3D9SetRenderState(D3DRS_ZENABLE, TRUE);
    }
    else
    {
        RwD3D9SetRenderState(D3DRS_ZENABLE, FALSE);
    }

    /* Stencil */
    RwD3D9SetRenderState(D3DRS_STENCILENABLE, _RwD3D9StateCache.stencilEnable);
    RwD3D9SetRenderState(D3DRS_STENCILFAIL,
                    _RwD3D9StencilOpConvTable[_RwD3D9StateCache.stencilFailOp]);
    RwD3D9SetRenderState(D3DRS_STENCILZFAIL,
                    _RwD3D9StencilOpConvTable[_RwD3D9StateCache.stencilZFailOp]);
    RwD3D9SetRenderState(D3DRS_STENCILPASS,
                    _RwD3D9StencilOpConvTable[_RwD3D9StateCache.stencilPassOp]);
    RwD3D9SetRenderState(D3DRS_STENCILFUNC,
                    _RwD3D9CmpFuncConvTable[_RwD3D9StateCache.stencilFunction]);
    RwD3D9SetRenderState(D3DRS_STENCILREF, _RwD3D9StateCache.stencilRef);
    RwD3D9SetRenderState(D3DRS_STENCILMASK, _RwD3D9StateCache.stencilMask);
    RwD3D9SetRenderState(D3DRS_STENCILWRITEMASK, _RwD3D9StateCache.stencilWriteMask);

    /* Texture states */
    for (i = 0; i < RWD3D9_MAX_TEXTURE_STAGES; i++)
    {
        /* Texture */
        DXCHECK(IDirect3DDevice9_SetTexture(_RwD3DDevice, i, NULL));
        _RwD3D9StateCache.stage[i].curTexRaster = NULL;

        /* Texture filter mode - Liner/Trilinear */
        RwD3D9SetSamplerState(i, D3DSAMP_MINFILTER,
                                _RwD3D9FilterModeConvTable[_RwD3D9StateCache.stage[i].filterMode].minmag);
        RwD3D9SetSamplerState(i, D3DSAMP_MAGFILTER,
                                _RwD3D9FilterModeConvTable[_RwD3D9StateCache.stage[i].filterMode].minmag);
        RwD3D9SetSamplerState(i, D3DSAMP_MIPFILTER,
                                _RwD3D9FilterModeConvTable[_RwD3D9StateCache.stage[i].filterMode].mip);

        /* Texture addressing mode */
        RwD3D9SetSamplerState(i, D3DSAMP_ADDRESSU,
                                       _RwD3D9AddressConvTable[_RwD3D9StateCache.stage[i].texAddressU]);

        RwD3D9SetSamplerState(i, D3DSAMP_ADDRESSV,
                                _RwD3D9AddressConvTable[_RwD3D9StateCache.stage[i].texAddressV]);

        /* Border color */
        RwD3D9SetSamplerState(i, D3DSAMP_BORDERCOLOR,
                                _RwD3D9StateCache.stage[i].borderColor);

        /* Max Anisotropy Level: 1 to disable it */
        RwD3D9SetSamplerState(i, D3DSAMP_MAXANISOTROPY, _RwD3D9StateCache.stage[i].maxAnisotropy);

        /* Disable stage if not the first one */
        if (i)
        {
            RwD3D9SetTextureStageState(i, D3DTSS_COLOROP,   D3DTOP_DISABLE);
            RwD3D9SetTextureStageState(i, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
        }
    }

    /* Set default blending modes */
    RwD3D9SetRenderState(D3DRS_SRCBLEND,
                         _RwD3D9BlendConvTable[_RwD3D9StateCache.srcBlend]);

    RwD3D9SetRenderState(D3DRS_DESTBLEND,
                         _RwD3D9BlendConvTable[_RwD3D9StateCache.dstBlend]);

    /* Set up the alpha compare function for future use */
    RwD3D9SetRenderState(D3DRS_ALPHAFUNC, _RwD3D9CmpFuncConvTable[_RwD3D9StateCache.alphaTestFunction]);
    RwD3D9SetRenderState(D3DRS_ALPHAREF, alphaRef);

    /* Disable alpha blending */
    if (_RwD3D9StateCache.vertexAlphaBlendEnable)
    {
        RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D9StateCache.alphaTestEnable);
    }
    else
    {
        RwD3D9SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }

    _RwD3D9StateCache.textureAlphaBlendEnable = FALSE;

    /* Turn lighting off */
    RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);

    /* Cull clockwise facing triangles */
    RwD3D9SetRenderState(D3DRS_CULLMODE, _RwD3D9CullModeConvTable[_RwD3D9StateCache.cullMode]);

    /* Vertex color only until a texture has been set */
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    /* Diffuse, Specular & Ambient colors come from the material */
    RwD3D9SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

    RwD3D9SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);

    RwD3D9SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

    /* Use dithering if possible */
    dither = (_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_DITHER) ? TRUE : FALSE;
    RwD3D9SetRenderState(D3DRS_DITHERENABLE, dither);

    RwD3D9SetRenderState(D3DRS_SPECULARENABLE, FALSE);

    RwD3D9SetRenderState(D3DRS_LOCALVIEWER, FALSE);

    RwD3D9SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

    _rwD3D9RenderStateFlushCache();

    RWRETURNVOID();
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                            Getting Render States

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwD3D9RWGetRenderState

 On entry   : Render state to get.
            : Parameter (OUT).
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RWGetRenderState(RwRenderState nState, void *param)
{
    RWFUNCTION(RWSTRING("_rwD3D9RWGetRenderState"));

    switch (nState)
    {
        /******************************* FOG ********************************/
        case rwRENDERSTATEFOGENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D9StateCache.fogEnable;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEFOGTYPE:
        {
            /* Return our conveniently cached value */
            *(RwFogType *)param = _RwD3D9StateCache.fogType;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEFOGCOLOR:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D9StateCache.fogColor;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEFOGDENSITY:
        {
            /* Return our conveniently cached value */
            *(RwReal *)param = _RwD3D9StateCache.fogDensity;

            RWRETURN(TRUE);
        }

        /************************ TEXTURE ADDRESSING ************************/

        case rwRENDERSTATETEXTUREADDRESS:
        {
            if (_RwD3D9StateCache.stage[0].texAddressU == _RwD3D9StateCache.stage[0].texAddressV)
            {
                /* Return our conveniently cached value */
                *(RwTextureAddressMode *)param = _RwD3D9StateCache.stage[0].texAddressU;

                RWRETURN(TRUE);
            }

            /* Return and invalid value */
            *(RwTextureAddressMode *)param = rwTEXTUREADDRESSNATEXTUREADDRESS;

            RWRETURN(FALSE);
        }

        case rwRENDERSTATETEXTUREADDRESSU:
        {
            /* Return our conveniently cached value */
            *(RwTextureAddressMode *)param = _RwD3D9StateCache.stage[0].texAddressU;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATETEXTUREADDRESSV:
        {
            /* Return our conveniently cached value */
            *(RwTextureAddressMode *)param = _RwD3D9StateCache.stage[0].texAddressV;

            RWRETURN(TRUE);
        }

        /*********************** TEXTURE FILTER MODE ************************/

        case rwRENDERSTATETEXTUREFILTER:
        {
            /* Return our conveniently cached value */
            *(RwTextureFilterMode *)param = _RwD3D9StateCache.stage[0].filterMode;

            RWRETURN(TRUE);
        }

        /************************** TEXTURE RASTER **************************/

        case rwRENDERSTATETEXTURERASTER:
        {
            /* Return our conveniently cached value */
            *(RwRaster **)param = _RwD3D9StateCache.stage[0].curTexRaster;

            RWRETURN(TRUE);
        }

        /************************** Z BUFFER WRITE **************************/

        case rwRENDERSTATEZWRITEENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D9StateCache.zWriteEnable;

            RWRETURN(TRUE);
        }

        /************************** Z BUFFER TEST ***************************/

        case rwRENDERSTATEZTESTENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D9StateCache.zTestEnable;

            RWRETURN(TRUE);
        }

        /************************** BLENDING MODES **************************/

        case rwRENDERSTATESRCBLEND:
        {
            /* Return our conveniently cached value */
            *(RwBlendFunction *)param = _RwD3D9StateCache.srcBlend;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEDESTBLEND:
        {
            /* Return our conveniently cached value */
            *(RwBlendFunction *)param = _RwD3D9StateCache.dstBlend;

            RWRETURN(TRUE);
        }

        /*************************** SHADING MODE ***************************/

        case rwRENDERSTATESHADEMODE:
        {
            /* Return our conveniently cached value */
            *(RwShadeMode *)param = _RwD3D9StateCache.shadeMode;

            RWRETURN(TRUE);
        }

        /*************************** VERTEX ALPHA ***************************/

        case rwRENDERSTATEVERTEXALPHAENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D9StateCache.vertexAlphaBlendEnable;

            RWRETURN(TRUE);
        }

        /************************** BORDER COLOR ****************************/

        case rwRENDERSTATEBORDERCOLOR:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D9StateCache.stage[0].borderColor;

            RWRETURN(TRUE);
        }

        /***************** PERSPECTIVELY CORRECT TEXTUREING *****************/

        case rwRENDERSTATETEXTUREPERSPECTIVE:
        {
            /* Return our conveniently cached value */

            RWRETURN(FALSE);
        }

        /*************************** CULLING MODE ***************************/

        case rwRENDERSTATECULLMODE:
        {
            /* Return our conveniently cached value */
            *(RwCullMode *)param = _RwD3D9StateCache.cullMode;

            RWRETURN(TRUE);
        }

        /************************** STENCIL BUFFER **************************/

        case rwRENDERSTATESTENCILENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D9StateCache.stencilEnable;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFAIL:
        {
            /* Return our conveniently cached value */
            *(RwStencilOperation *)param = _RwD3D9StateCache.stencilFailOp;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILZFAIL:
        {
            /* Return our conveniently cached value */
            *(RwStencilOperation *)param = _RwD3D9StateCache.stencilZFailOp;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILPASS:
        {
            /* Return our conveniently cached value */
            *(RwStencilOperation *)param = _RwD3D9StateCache.stencilPassOp;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFUNCTION:
        {
            /* Return our conveniently cached value */
            *(RwStencilFunction *)param = _RwD3D9StateCache.stencilFunction;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFUNCTIONREF:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D9StateCache.stencilRef;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFUNCTIONMASK:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D9StateCache.stencilMask;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFUNCTIONWRITEMASK:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D9StateCache.stencilWriteMask;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEALPHATESTFUNCTION:
        {
            /* Return our conveniently cached value */
            *(RwAlphaTestFunction *)param = _RwD3D9StateCache.alphaTestFunction;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEALPHATESTFUNCTIONREF:
        {
            /* Return our conveniently cached value */
            *(RwUInt32 *)param = RenderStateCache[D3DRS_ALPHAREF].value;

            RWRETURN(TRUE);
        }

        /************************** NOT SUPPORTED ***************************/

        default:
        {
            break;
        }
    }

    RWRETURN(FALSE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                            Setting Render States

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 rwD3D9RenderStateFogEnable

 On entry   : A RwBool value defining the desired fog state.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateFogEnable(RwBool fog)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateFogEnable"));

    if (fog)
    {
        if (!_RwD3D9StateCache.fogEnable)
        {
            if (_RwD3D9DeviceCaps.RasterCaps & (D3DPRASTERCAPS_FOGTABLE | D3DPRASTERCAPS_FOGVERTEX))
            {
                FORCE_RENDERSTATE(D3DRS_FOGENABLE, TRUE);
                _RwD3D9StateCache.fogEnable = TRUE;
            }
        }
    }
    else
    {
        if (_RwD3D9StateCache.fogEnable)
        {
            FORCE_RENDERSTATE(D3DRS_FOGENABLE, FALSE);
            _RwD3D9StateCache.fogEnable = FALSE;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9RenderStateFogColor

 On entry   : A RwUInt32 value defining the desired fog color.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateFogColor(RwUInt32 fogColor)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateFogColor"));

    if (fogColor != _RwD3D9StateCache.fogColor)
    {
        FORCE_RENDERSTATE(D3DRS_FOGCOLOR, fogColor);
        _RwD3D9StateCache.fogColor = fogColor;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9RenderStateFogType

 On entry   : A RwFogType value defining the desired fog type.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateFogType(RwFogType fogType)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateFogType"));
    RWASSERT(fogType < NUMFOGMODES);
    RWASSERT(fogType >= 0);

    if (fogType != _RwD3D9StateCache.fogType)
    {
        if ((_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0 &&
            (_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG) != 0)
        {
            FORCE_RENDERSTATE(D3DRS_FOGTABLEMODE,
                                     _RwD3D9FogConvTable[fogType]);

            if (RenderStateCache[D3DRS_FOGVERTEXMODE].value != D3DFOG_NONE)
            {
                FORCE_RENDERSTATE(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
            }
        }
        else
        {
            if (RenderStateCache[D3DRS_FOGTABLEMODE].value != D3DFOG_NONE)
            {
                FORCE_RENDERSTATE(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
            }

            FORCE_RENDERSTATE(D3DRS_FOGVERTEXMODE,
                                     _RwD3D9FogConvTable[fogType]);
        }

        _RwD3D9StateCache.fogType = fogType;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9RenderStateFogDensity

 On entry   : A RwReal value defining the desired fog density.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateFogDensity(RwReal fogDensity)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateFogDensity"));

    if (fogDensity != _RwD3D9StateCache.fogDensity)
    {
        FORCE_RENDERSTATE(D3DRS_FOGDENSITY, FLOATASDWORD(fogDensity));
        _RwD3D9StateCache.fogDensity = fogDensity;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateTextureAddress

 On entry   : A RwTextureAddressMode value defining the desired texture
              addressing mode.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateTextureAddress(RwTextureAddressMode addressMode)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateTextureAddress"));
    RWASSERT(addressMode < NUMADDRESSMODES);
    RWASSERT(addressMode >= 0);

    if (addressMode != _RwD3D9StateCache.stage[0].texAddressU)
    {
        _RwD3D9StateCache.stage[0].texAddressU = addressMode;

        FORCE_SAMPLERSTATE(0, D3DSAMP_ADDRESSU, _RwD3D9AddressConvTable[addressMode]);
    }

    if (addressMode != _RwD3D9StateCache.stage[0].texAddressV)
    {
        _RwD3D9StateCache.stage[0].texAddressV = addressMode;

        FORCE_SAMPLERSTATE(0, D3DSAMP_ADDRESSV, _RwD3D9AddressConvTable[addressMode]);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateTextureAddressU

 On entry   : A RwTextureAddressMode value defining the desired U texture
              addressing mode.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateTextureAddressU(RwTextureAddressMode addressMode)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateTextureAddressU"));
    RWASSERT(addressMode < NUMADDRESSMODES);
    RWASSERT(addressMode >= 0);

    if (addressMode != _RwD3D9StateCache.stage[0].texAddressU)
    {
        _RwD3D9StateCache.stage[0].texAddressU = addressMode;

        FORCE_SAMPLERSTATE(0, D3DSAMP_ADDRESSU, _RwD3D9AddressConvTable[addressMode]);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateTextureAddressV

 On entry   : A RwTextureAddressMode value defining the desired V texture
              addressing mode.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateTextureAddressV(RwTextureAddressMode addressMode)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateTextureAddressV"));
    RWASSERT(addressMode < NUMADDRESSMODES);
    RWASSERT(addressMode >= 0);

    if (addressMode != _RwD3D9StateCache.stage[0].texAddressV)
    {
        _RwD3D9StateCache.stage[0].texAddressV = addressMode;

        FORCE_SAMPLERSTATE(0, D3DSAMP_ADDRESSV, _RwD3D9AddressConvTable[addressMode]);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateTextureFilter

 On entry   : A RwTextureFilterMode value defining the desired filter mode.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateTextureFilter(RwTextureFilterMode filterMode)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateTextureFilter"));
    RWASSERT(filterMode < NUMFILTERMODES);
    RWASSERT(filterMode >= 0);

    if (_RwD3D9StateCache.stage[0].maxAnisotropy > 1)
    {
        _RwD3D9StateCache.stage[0].maxAnisotropy = 1;

        FORCE_SAMPLERSTATE(0, D3DSAMP_MAXANISOTROPY, 1);
    }

    if (filterMode != _RwD3D9StateCache.stage[0].filterMode)
    {
        const RwUInt32 minmag = _RwD3D9FilterModeConvTable[filterMode].minmag;
        const RwUInt32 mip = _RwD3D9FilterModeConvTable[filterMode].mip;

        _RwD3D9StateCache.stage[0].filterMode = filterMode;

        FORCE_SAMPLERSTATE(0, D3DSAMP_MINFILTER, minmag);
        FORCE_SAMPLERSTATE(0, D3DSAMP_MAGFILTER, minmag);

        if (SamplerStageStateD3D[0][D3DSAMP_MIPFILTER] != mip)
        {
            FORCE_SAMPLERSTATE(0, D3DSAMP_MIPFILTER, mip);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateSrcBlend

 On entry   : A RwBlendFunction value defining the blend state.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateSrcBlend(RwBlendFunction srcBlend)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateSrcBlend"));
    RWASSERT(srcBlend < NUMBLENDMODES);
    RWASSERT(srcBlend >= 0);

    if (srcBlend != _RwD3D9StateCache.srcBlend)
    {
        FORCE_RENDERSTATE(D3DRS_SRCBLEND,
                                 _RwD3D9BlendConvTable[srcBlend]);

        _RwD3D9StateCache.srcBlend = srcBlend;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateDestBlend

 On entry   : A RwBlendFunction value defining the blend state.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateDestBlend(RwBlendFunction dstBlend)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateDestBlend"));
    RWASSERT(dstBlend < NUMBLENDMODES);
    RWASSERT(dstBlend >= 0);

    if (dstBlend != _RwD3D9StateCache.dstBlend)
    {
        FORCE_RENDERSTATE(D3DRS_DESTBLEND,
                                 _RwD3D9BlendConvTable[dstBlend]);

        _RwD3D9StateCache.dstBlend = dstBlend;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RWSetRasterStage

 On entry   : A RwRaster pointer for the desired texture.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RWSetRasterStage(RwRaster *raster, RwUInt32 stage)
{
    _rwD3D9RasterExt  *rasExt;

    RWFUNCTION(RWSTRING("_rwD3D9RWSetRasterStage"));
    RWASSERT(stage < RWD3D9_MAX_TEXTURE_STAGES);

#if defined(RWDEBUG)
    if (_RwD3D9DeviceCaps.MaxTextureBlendStages <= stage)
    {
        RwChar              buffer[256];

        rwsprintf(buffer, "You are using %d stages. This video card only supports %d stages.",
                  (stage + 1), _RwD3D9DeviceCaps.MaxTextureBlendStages);

        RwDebugSendMessage(rwDEBUGMESSAGE,
                           "_rwD3D9RWSetRasterStage",
                           buffer);

        RWRETURN(FALSE);
    }
#endif

    /*
     * Check alpha blending
     */
    if(raster)
    {
        rasExt = RASTEREXTFROMRASTER(raster);

        RWASSERT(rasExt->texture != NULL);
    }
    else
    {
        rasExt = (_rwD3D9RasterExt *)NULL;
    }

    if (stage == 0)
    {
        if (raster && rasExt->alpha)
        {
            /*
             * Enable Alpha Blending if the texture has alpha
             */
            if (!_RwD3D9StateCache.textureAlphaBlendEnable)
            {
                _RwD3D9StateCache.textureAlphaBlendEnable = TRUE;

                if (!_RwD3D9StateCache.vertexAlphaBlendEnable)
                {
                    FORCE_RENDERSTATE(D3DRS_ALPHABLENDENABLE, TRUE);

                    RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D9StateCache.alphaTestEnable);
                }
            }
        }
        else
        {
			if(raster && g_DXT1AlphaChecker)
			{
				if (!_RwD3D9StateCache.textureAlphaBlendEnable)
				{
					_RwD3D9StateCache.textureAlphaBlendEnable = TRUE;

					if (!_RwD3D9StateCache.vertexAlphaBlendEnable)
					{
						FORCE_RENDERSTATE(D3DRS_ALPHABLENDENABLE, TRUE);

						RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D9StateCache.alphaTestEnable);
					}
				}	
			}
			else
			{
				if (_RwD3D9StateCache.textureAlphaBlendEnable)
				{
					_RwD3D9StateCache.textureAlphaBlendEnable = FALSE;

					/*
					* Disable alpha blending if it has not been explicity set
					*/
					if (!_RwD3D9StateCache.vertexAlphaBlendEnable)
					{
						FORCE_RENDERSTATE(D3DRS_ALPHABLENDENABLE, FALSE);

						RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
					}
				}
			}
        }
    }

    /*
     * Check color
     */
    if (raster != _RwD3D9StateCache.stage[stage].curTexRaster)
    {
        _RwD3D9StateCache.stage[stage].curTexRaster = raster;

        if (raster)
        {
            /* Set the texture */
            DXCHECK(IDirect3DDevice9_SetTexture(_RwD3DDevice, stage,
                                 (LPDIRECT3DBASETEXTURE9)(rasExt->texture)));

            /* Set the palette if it is a palettized texture */
            if (rasExt->palette)
            {
                DXCHECK(IDirect3DDevice9_SetCurrentTexturePalette(_RwD3DDevice, rasExt->palette->globalindex));
            }

            #if defined( RWMETRICS )
            RWSRCGLOBAL(metrics)->numTextureUploads+=1;
            RWSRCGLOBAL(metrics)->sizeTextureUploads+=raster->width * raster->height * (raster->depth / 8);
            #endif
        }
        else
        {
            DXCHECK(IDirect3DDevice9_SetTexture(_RwD3DDevice, stage, NULL));
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RWGetRasterStage

 On exit    : The raster set in the current stage
 */
RwRaster *
_rwD3D9RWGetRasterStage(RwUInt32 stage)
{
    RWFUNCTION(RWSTRING("_rwD3D9RWGetRasterStage"));
    RWASSERT(stage < RWD3D9_MAX_TEXTURE_STAGES);

    RWRETURN(_RwD3D9StateCache.stage[stage].curTexRaster);
}

/****************************************************************************
 _rwD3D9SetTextureAnisotropyOffset

 Set the anisotropy texture offset that enables the anisotropy plugin
 */
void
_rwD3D9SetTextureAnisotropyOffset(RwInt32 anisotTextureOffset)
{
    RWFUNCTION(RWSTRING("_rwD3D9SetTextureAnisotropyOffset"));

    RWASSERT(anisotTextureOffset > 0);

    D3D9AnisotTextureOffset = anisotTextureOffset;

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d9
 * \ref RwD3D9SetTexture assigns a texture to a stage.
 *
 * This function may change the following texture or sampler states for the
 * selected stage, if the states stored in the \ref rwtexture do not match
 * those cached in the D3D9 driver:
 * \li D3DSAMP_ADDRESSU
 * \li D3DSAMP_ADDRESSV
 * \li D3DSAMP_MINFILTER
 * \li D3DSAMP_MAGFILTER
 * \li D3DSAMP_MIPFILTER
 * \li D3DSAMP_MAXANISOTROPY
 *
 *
 * \param texture Pointer to an \ref rwtexture, representing the texture
 * being set.
 * \param stage Stage identifier to which the texture is set. Stage
 * identifiers are zero-based. Devices can have up to 8
 * textures set, so the maximum value allowed for \e stage is 7.
 *
 * \return TRUE if the texture value was set, FALSE otherwise.
 *
 * \see RwD3D9SetSamplerState
 * \see RwD3D9GetSamplerStageState
 * \see RwD3D9SetTextureStageState
 * \see RwD3D9GetTextureStageState
 *
 */
RwBool
RwD3D9SetTexture(RwTexture *texture, RwUInt32 stage)
{
    RWAPIFUNCTION(RWSTRING("RwD3D9SetTexture"));
    RWASSERT(stage < RWD3D9_MAX_TEXTURE_STAGES);

    if (texture)
    {
        RwTextureAddressMode    addressingU;
        RwTextureAddressMode    addressingV;
        RwTextureFilterMode     filtering;

        /* Addressing mode */
        addressingU = RwTextureGetAddressingU(texture);
        if (addressingU != _RwD3D9StateCache.stage[stage].texAddressU)
        {
            _RwD3D9StateCache.stage[stage].texAddressU = addressingU;

            FORCE_SAMPLERSTATE(stage, D3DSAMP_ADDRESSU, _RwD3D9AddressConvTable[addressingU]);
        }

        addressingV = RwTextureGetAddressingV(texture);
        if (addressingV != _RwD3D9StateCache.stage[stage].texAddressV)
        {
            _RwD3D9StateCache.stage[stage].texAddressV = addressingV;

            FORCE_SAMPLERSTATE(stage, D3DSAMP_ADDRESSV, _RwD3D9AddressConvTable[addressingV]);
        }

        /* Filter mode */
        filtering = RwTextureGetFilterMode(texture);

        if (D3D9AnisotTextureOffset > 0)
        {
            const RwInt32 anisotropy = CONSTTEXTUREGETANISOT(texture);

            if (_RwD3D9StateCache.stage[stage].maxAnisotropy != anisotropy)
            {
                _RwD3D9StateCache.stage[stage].maxAnisotropy = anisotropy;

                FORCE_SAMPLERSTATE(stage, D3DSAMP_MAXANISOTROPY, anisotropy);
            }

            if (anisotropy > 1)
            {
                filtering += NUMFILTERMODES;
            }
        }

        if (_RwD3D9StateCache.stage[stage].filterMode != filtering)
        {
            D3DTEXTUREFILTERTYPE    minmag, mip;

            _RwD3D9StateCache.stage[stage].filterMode = filtering;

            minmag = _RwD3D9FilterModeConvTable[filtering].minmag;
            mip = _RwD3D9FilterModeConvTable[filtering].mip;

            if (minmag == D3DTEXF_ANISOTROPIC)
            {
                FORCE_SAMPLERSTATE(stage, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
                if (_RwD3D9DeviceCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC)
                {
                    FORCE_SAMPLERSTATE(stage, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
                }
                else
                {
                    FORCE_SAMPLERSTATE(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
                }
            }
            else
            {
                FORCE_SAMPLERSTATE(stage, D3DSAMP_MINFILTER, minmag);
                FORCE_SAMPLERSTATE(stage, D3DSAMP_MAGFILTER, minmag);
            }

            FORCE_SAMPLERSTATE(stage, D3DSAMP_MIPFILTER, mip);
        }

        /* Raster */
        _rwD3D9RWSetRasterStage(texture->raster, stage);

        /* Check texture parameters */
        #if defined(RWDEBUG)
        if (texture->raster != NULL)
        {
            /* Non-pow2 textures only suppor CLAMP */
            if (addressingU != rwTEXTUREADDRESSCLAMP ||
                addressingV != rwTEXTUREADDRESSCLAMP)
            {
                if ( texture->raster->width != (1 << _rwD3D9FindMSB(texture->raster->width)) ||
                     texture->raster->height != (1 << _rwD3D9FindMSB(texture->raster->height)))
                {
                    RwChar              buffer[256];

                    rwsprintf(buffer, "'%s' has non power of 2 dimensions but is using an addressing mode different than rwTEXTUREADDRESSCLAMP, which is not supported.",
                              texture->name);

                    RwDebugSendMessage(rwDEBUGMESSAGE,
                                       "RwD3D9SetTexture",
                                       buffer);
                }
            }
        }
        #endif
    }
    else
    {
        if (stage == 0)
        {
            if (_RwD3D9StateCache.textureAlphaBlendEnable)
            {
                _RwD3D9StateCache.textureAlphaBlendEnable = FALSE;

                /*
                * Disable alpha blending if it has not been explicity set
                */
                if (!_RwD3D9StateCache.vertexAlphaBlendEnable)
                {
                    FORCE_RENDERSTATE(D3DRS_ALPHABLENDENABLE, FALSE);

                    RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
                }
            }
        }

        if (NULL != _RwD3D9StateCache.stage[stage].curTexRaster)
        {
            _RwD3D9StateCache.stage[stage].curTexRaster = NULL;

            DXCHECK(IDirect3DDevice9_SetTexture(_RwD3DDevice, stage, NULL));
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9RenderStateZWriteEnable

 On entry   : A RwBool value defining the desired Z-write state.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateZWriteEnable(RwBool enable)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateZWriteEnable"));

    if (enable)
    {
        if (!_RwD3D9StateCache.zWriteEnable)
        {
            if (!_RwD3D9StateCache.zTestEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ZENABLE, TRUE);
            }

            FORCE_RENDERSTATE(D3DRS_ZWRITEENABLE, TRUE);

            _RwD3D9StateCache.zWriteEnable = TRUE;
        }
    }
    else
    {
        if (_RwD3D9StateCache.zWriteEnable)
        {
            if (!_RwD3D9StateCache.zTestEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ZENABLE, FALSE);
            }

            FORCE_RENDERSTATE(D3DRS_ZWRITEENABLE, FALSE);

            _RwD3D9StateCache.zWriteEnable = FALSE;
        }
    }
    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9RenderStateZTestEnable

 On entry   : A RwBool value defining the desired Z-test state.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateZTestEnable(RwBool enable)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateZTestEnable"));

    if (enable)
    {
        if (!_RwD3D9StateCache.zTestEnable)
        {
            if (!_RwD3D9StateCache.zWriteEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ZENABLE, TRUE);
            }

            FORCE_RENDERSTATE(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

            _RwD3D9StateCache.zTestEnable = TRUE;
        }
    }
    else
    {
        if (_RwD3D9StateCache.zTestEnable)
        {
            if (!_RwD3D9StateCache.zWriteEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ZENABLE, FALSE);
            }

            FORCE_RENDERSTATE(D3DRS_ZFUNC, D3DCMP_ALWAYS);

            _RwD3D9StateCache.zTestEnable = FALSE;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9RenderStateShadeMode

 On entry   : A RwShadeMode value defining the desired shading mode.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateShadeMode(RwShadeMode shadeMode)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateShadeMode"));
    RWASSERT(shadeMode < NUMSHADEMODES);
    RWASSERT(shadeMode >= 0);

    if (shadeMode != _RwD3D9StateCache.shadeMode)
    {
        FORCE_RENDERSTATE(D3DRS_SHADEMODE,
                                 _RwD3D9ShadeModeConvTable[shadeMode]);
        _RwD3D9StateCache.shadeMode = shadeMode;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateVertexAlphaEnable

 On entry   : A RwBool value defining the desired vertex alpha state.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateVertexAlphaEnable(RwBool enable)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateVertexAlphaEnable"));

    if (enable)
    {
        if (!_RwD3D9StateCache.vertexAlphaBlendEnable)
        {
            _RwD3D9StateCache.vertexAlphaBlendEnable = TRUE;

            if (!_RwD3D9StateCache.textureAlphaBlendEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ALPHABLENDENABLE, TRUE);

                RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D9StateCache.alphaTestEnable);
            }
        }
    }
    else
    {
        if (_RwD3D9StateCache.vertexAlphaBlendEnable)
        {
            _RwD3D9StateCache.vertexAlphaBlendEnable = FALSE;

            if (!_RwD3D9StateCache.textureAlphaBlendEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ALPHABLENDENABLE, FALSE);

                RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
            }
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateIsVertexAlphaEnable

  On exit    : TRUE is vertex alpha blend is enabled.
 */
RwBool
_rwD3D9RenderStateIsVertexAlphaEnable(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateIsVertexAlphaEnable"));

    RWRETURN(_RwD3D9StateCache.vertexAlphaBlendEnable);
}

/****************************************************************************
 rwD3D9RenderStateBorderColor

 On entry   : A RwInt32 value defining the border color.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateBorderColor(RwUInt32 borderColor)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateBorderColor"));

    if (borderColor != _RwD3D9StateCache.stage[0].borderColor)
    {
        _RwD3D9StateCache.stage[0].borderColor = borderColor;

        FORCE_SAMPLERSTATE(0, D3DSAMP_BORDERCOLOR, borderColor);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D9RenderStateTexturePerspective

 On entry   : A RwBool value defining the perspective texturing state.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateTexturePerspective(RwBool persp)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateTexturePerspective"));

    /* Only can set on */
    if (persp)
    {
        RWRETURN(TRUE);
    }

    /* If trying to set off, fail */
    RWRETURN(FALSE);
}

/****************************************************************************
 rwD3D9RenderStateCullMode

 On entry   : A RwCullMode value defining the culling mode to use.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D9RenderStateCullMode(RwCullMode cullMode)
{
    RWFUNCTION(RWSTRING("rwD3D9RenderStateCullMode"));
    RWASSERT(cullMode < NUMCULLMODES);
    RWASSERT(cullMode >= 0);

    if (cullMode != _RwD3D9StateCache.cullMode)
    {
        FORCE_RENDERSTATE(D3DRS_CULLMODE, _RwD3D9CullModeConvTable[cullMode]);

        _RwD3D9StateCache.cullMode = cullMode;
    }

    RWRETURN(TRUE);
}


/****************************************************************************
 _rwD3D9RenderStateStencilEnable

 On entry   : A RwBool value defining the stencil buffer state.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateStencilEnable(RwBool enable)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateStencilEnable"));

    if (enable != _RwD3D9StateCache.stencilEnable)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILENABLE, enable);

        _RwD3D9StateCache.stencilEnable = enable;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateStencilFail

 On entry   : A RwStencilOperation value defining the operation to perform if
              the stencil test fails.
 On exit    : TRUE on success else FALSE.
*/
RwBool
_rwD3D9RenderStateStencilFail(RwStencilOperation operation)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateStencilFail"));
    RWASSERT(operation < NUMSTENCILOPERATIONS);
    RWASSERT(operation >= 0);

    if (operation != _RwD3D9StateCache.stencilFailOp)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILFAIL,
            _RwD3D9StencilOpConvTable[operation]);

        _RwD3D9StateCache.stencilFailOp = operation;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateStencilZFail

 On entry   : A RwStencilOperation value defining the operation to perform if
              the stencil test passes and the depth test (z-test) fails.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateStencilZFail(RwStencilOperation operation)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateStencilZFail"));
    RWASSERT(operation < NUMSTENCILOPERATIONS);
    RWASSERT(operation >= 0);

    if (operation != _RwD3D9StateCache.stencilZFailOp)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILZFAIL,
            _RwD3D9StencilOpConvTable[operation]);

        _RwD3D9StateCache.stencilZFailOp = operation;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateStencilPass

 On entry   : A RwStencilOperation value defining the operation to perform if
              both the stencil and the depth (z) tests pass.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateStencilPass(RwStencilOperation operation)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateStencilPass"));
    RWASSERT(operation < NUMSTENCILOPERATIONS);
    RWASSERT(operation >= 0);

    if (operation != _RwD3D9StateCache.stencilPassOp)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILPASS,
            _RwD3D9StencilOpConvTable[operation]);

        _RwD3D9StateCache.stencilPassOp = operation;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateStencilFunction

 On entry   : A RwStencilFunction value defining the comparison function for
              the stencil test.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateStencilFunction(RwStencilFunction function)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateStencilFunction"));
    RWASSERT(function < NUMCMPFUNCTIONS);
    RWASSERT(function >= 0);

    if (function != _RwD3D9StateCache.stencilFunction)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILFUNC,
            _RwD3D9CmpFuncConvTable[function]);

        _RwD3D9StateCache.stencilFunction = function;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateStencilFunctionRef

 On entry   : A RwInt32 value defining the reference value for the stencil
              test.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateStencilFunctionRef(RwInt32 ref)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateStencilFunctionRef"));

    if (ref != _RwD3D9StateCache.stencilRef)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILREF, ref);

        _RwD3D9StateCache.stencilRef = ref;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateStencilFunctionMask

 On entry   : A RwInt32 value defining the mask applied to the reference value
              and each stencil buffer entry to determine the significant bits
              for the stencil test.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateStencilFunctionMask(RwInt32 mask)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateStencilFunctionMask"));

    if (mask != _RwD3D9StateCache.stencilMask)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILMASK, mask);

        _RwD3D9StateCache.stencilMask = mask;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9RenderStateStencilFunctionWriteMask

 On entry   : A RwInt32 value defining the write mask applied to values
              written into the stencil buffer.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RenderStateStencilFunctionWriteMask(RwInt32 writeMask)
{
    RWFUNCTION(RWSTRING("_rwD3D9RenderStateStencilFunctionWriteMask"));

    if (writeMask != _RwD3D9StateCache.stencilWriteMask)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILWRITEMASK, writeMask);

        _RwD3D9StateCache.stencilWriteMask = writeMask;
    }

    RWRETURN(TRUE);
}


/****************************************************************************
 _rwD3D9RWSetRenderState

 On entry   : Render state to set
            : Parameter
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D9RWSetRenderState(RwRenderState state, void *param)

{
    RwBool  result = FALSE;
    D3D9UnionParam uparam;

    RWFUNCTION(RWSTRING("_rwD3D9RWSetRenderState"));

    /*
     * Need to be in a camera update cliche for this to work
     */

    uparam.VoidPtr = param;

    switch (state)
    {
            /***************************** FOG ******************************/

        case rwRENDERSTATEFOGENABLE:
            result = rwD3D9RenderStateFogEnable(uparam.Bool);
            break;

        case rwRENDERSTATEFOGCOLOR:
            result = rwD3D9RenderStateFogColor(uparam.UInt32);
            break;

        case rwRENDERSTATEFOGTYPE:
            result = rwD3D9RenderStateFogType(uparam.FogType);
            break;

        case rwRENDERSTATEFOGDENSITY:
            result = rwD3D9RenderStateFogDensity(*uparam.RealPtr);
            break;

            /********************** TEXTURE ADDRESSING **********************/

        case rwRENDERSTATETEXTUREADDRESS:
            result = _rwD3D9RenderStateTextureAddress(uparam.TextureAddressMode);
            break;

        case rwRENDERSTATETEXTUREADDRESSU:
            result = _rwD3D9RenderStateTextureAddressU(uparam.TextureAddressMode);
            break;

        case rwRENDERSTATETEXTUREADDRESSV:
            result = _rwD3D9RenderStateTextureAddressV(uparam.TextureAddressMode);
            break;

            /********************* TEXTURE FILTER MODE **********************/

        case rwRENDERSTATETEXTUREFILTER:
            result = _rwD3D9RenderStateTextureFilter(uparam.TextureFilterMode);
            break;

            /************************ TEXTURE RASTER ************************/

        case rwRENDERSTATETEXTURERASTER:
            RWASSERT(uparam.RasterPtr == NULL || uparam.RasterPtr == RwRasterGetParent(uparam.RasterPtr));
            result = _rwD3D9RWSetRasterStage(uparam.RasterPtr, 0);
            break;

            /************************ Z BUFFER WRITE ************************/

        case rwRENDERSTATEZWRITEENABLE:
            result = rwD3D9RenderStateZWriteEnable(uparam.Bool);
            break;

            /************************ Z BUFFER TEST *************************/

        case rwRENDERSTATEZTESTENABLE:
            result = rwD3D9RenderStateZTestEnable(uparam.Bool);
            break;

            /************************ BLENDING MODES ************************/

        case rwRENDERSTATESRCBLEND:
            result = _rwD3D9RenderStateSrcBlend(uparam.BlendFunction);
            break;

        case rwRENDERSTATEDESTBLEND:
            result = _rwD3D9RenderStateDestBlend(uparam.BlendFunction);
            break;

            /************************* SHADING MODE *************************/

        case rwRENDERSTATESHADEMODE:
            result = rwD3D9RenderStateShadeMode(uparam.ShadeMode);
            break;

            /************************* VERTEX ALPHA *************************/

        case rwRENDERSTATEVERTEXALPHAENABLE:
            result = _rwD3D9RenderStateVertexAlphaEnable(uparam.Bool);
            break;

            /************************ BORDER COLOR **************************/

        case rwRENDERSTATEBORDERCOLOR:
            result = rwD3D9RenderStateBorderColor(uparam.UInt32);
            break;

            /*************** PERSPECTIVELY CORRECT TEXTUREING ***************/

        case rwRENDERSTATETEXTUREPERSPECTIVE:
            result = rwD3D9RenderStateTexturePerspective(uparam.Bool);
            break;

            /************************* CULLING MODE *************************/

        case rwRENDERSTATECULLMODE:
            result = rwD3D9RenderStateCullMode(uparam.CullMode);
            break;

            /************************** STENCIL BUFFER **************************/

        case rwRENDERSTATESTENCILENABLE:
            result = _rwD3D9RenderStateStencilEnable(uparam.Bool);
            break;

        case rwRENDERSTATESTENCILFAIL:
            result = _rwD3D9RenderStateStencilFail(uparam.StencilOperation);
            break;

        case rwRENDERSTATESTENCILZFAIL:
            result = _rwD3D9RenderStateStencilZFail(uparam.StencilOperation);
            break;

        case rwRENDERSTATESTENCILPASS:
            result = _rwD3D9RenderStateStencilPass(uparam.StencilOperation);
            break;

        case rwRENDERSTATESTENCILFUNCTION:
            result = _rwD3D9RenderStateStencilFunction(uparam.StencilFunction);
            break;

        case rwRENDERSTATESTENCILFUNCTIONREF:
            result = _rwD3D9RenderStateStencilFunctionRef(uparam.Int32);
            break;

        case rwRENDERSTATESTENCILFUNCTIONMASK:
            result = _rwD3D9RenderStateStencilFunctionMask(uparam.Int32);
            break;

        case rwRENDERSTATESTENCILFUNCTIONWRITEMASK:
            result = _rwD3D9RenderStateStencilFunctionWriteMask(uparam.Int32);
            break;

        case rwRENDERSTATEALPHATESTFUNCTION:
            if (uparam.AlphaTestFunction != _RwD3D9StateCache.alphaTestFunction)
            {
                _RwD3D9StateCache.alphaTestFunction = uparam.AlphaTestFunction;

                _RwD3D9StateCache.alphaTestEnable = (uparam.AlphaTestFunction != rwALPHATESTFUNCTIONALWAYS);

                if (RenderStateCache[D3DRS_ALPHABLENDENABLE].value)
                {
                    RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D9StateCache.alphaTestEnable);
                }

                FORCE_RENDERSTATE(D3DRS_ALPHAFUNC,
                    _RwD3D9CmpFuncConvTable[uparam.AlphaTestFunction]);
            }
            result = TRUE;
            break;

        case rwRENDERSTATEALPHATESTFUNCTIONREF:
            RwD3D9SetRenderState(D3DRS_ALPHAREF, uparam.UInt32);
            result = TRUE;
            break;

        /************************ NOT SUPPORTED *************************/

        case rwRENDERSTATENARENDERSTATE:
            break;

#ifdef RW4
        case rwRENDERSTATEUSERSHADERS:
            break;
#endif

        case rwRENDERSTATEFORCEENUMSIZEINT:
            break;

#if (0)
        default:
            result = FALSE;
            break;
#endif /* (0) */
    }

    RWRETURN(result);
}
