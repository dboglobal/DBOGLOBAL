/***************************************************************************
 *                                                                         *
 * Module  : D3D8rendst.c                                                    *
 *                                                                         *
 * Purpose : Renderstates.                                                 *
 *                                                                         *
 **************************************************************************/

 /**************************************************************************
 Includes
 */
#include <d3d8.h>

#include "batypes.h"
#include "badebug.h"
#include "batextur.h"
#include "baimage.h"
#include "baresour.h"
#include "bamatrix.h"
#include "baimmedi.h"
#include "barwtyp.h"
#include "drvmodel.h"

#include "drvfns.h"
#include "d3d8device.h"
#include "d3d8rendst.h"
#include "d3d8raster.h"
#include "d3d82drend.h"

#if defined( RWMETRICS )
#include "d3d8metric.h"
#endif /* defined( RWMETRICS ) */

#if defined( RWDEBUG )
#include "d3d8convrt.h"
#endif

/****************************************************************************
 Local Defines
 */

#define TEXTURE_STAGE_0 0

#define FLOATASDWORD(fvariable) (*((const DWORD *)&(fvariable)))

#define CONVERTTOZDEVICE(zvalue, nearPlane, farPlane) (((zvalue - nearPlane) * farPlane) / ((farPlane - nearPlane) * zvalue))
#define CONVERTTOZDEVICEPARALLEL(zvalue, nearPlane, farPlane) ((zvalue - nearPlane) / (farPlane - nearPlane))

#define CONSTTEXTUREGETANISOT(tex) \
    (*((const RwUInt8*)(((const RwUInt8*)tex) + D3D8AnisotTextureOffset)))

/****************************************************************************
 Global Types
 */
typedef union D3D8UnionParam D3D8UnionParam;
union D3D8UnionParam
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

typedef struct _rwD3D8StageStateCache _rwD3D8StageStateCache;
struct _rwD3D8StageStateCache
{
    RwRaster                *curTexRaster;
    RwTextureFilterMode     filterMode;
    RwTextureAddressMode    texAddressU;
    RwTextureAddressMode    texAddressV;
    RwUInt32                borderColor;
    RwInt8                  maxAnisotropy;
};

typedef struct _rwD3D8StateCache _rwD3D8StateCache;
struct _rwD3D8StateCache
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
    _rwD3D8StageStateCache  stage[MAX_TEXTURE_STAGES];

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

static _rwD3D8StateCache  _RwD3D8StateCache;

static RwInt32 D3D8AnisotTextureOffset = 0; /* 0 means deactivated */

/****************************************************************************
 Local (static) Globals
 */

#define NUMSHADEMODES   3
static D3DSHADEMODE _RwD3D8ShadeModeConvTable[NUMSHADEMODES] =
{
    (D3DSHADEMODE)0,
    D3DSHADE_FLAT,
    D3DSHADE_GOURAUD
};

#define NUMFOGMODES     4
static D3DFOGMODE _RwD3D8FogConvTable[NUMFOGMODES] =
{
    D3DFOG_NONE,
    D3DFOG_LINEAR,
    D3DFOG_EXP,
    D3DFOG_EXP2
};

#define NUMADDRESSMODES 5
static D3DTEXTUREADDRESS _RwD3D8AddressConvTable[NUMADDRESSMODES] =
{
    (D3DTEXTUREADDRESS)0,
    D3DTADDRESS_WRAP,
    D3DTADDRESS_MIRROR,
    D3DTADDRESS_CLAMP,
    D3DTADDRESS_BORDER
};

#define NUMBLENDMODES   12
static D3DBLEND _RwD3D8BlendConvTable[NUMBLENDMODES] =
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

typedef struct __rwFilterParams _rwFilterParams;
struct __rwFilterParams
{
    D3DTEXTUREFILTERTYPE minmag;
    D3DTEXTUREFILTERTYPE mip;
};

#define NUMFILTERMODES  7
static _rwFilterParams _RwD3D8FilterModeConvTable[NUMFILTERMODES] =
{
    {D3DTEXF_NONE,   D3DTEXF_NONE},
    {D3DTEXF_POINT,  D3DTEXF_NONE},
    {D3DTEXF_LINEAR, D3DTEXF_NONE},
    {D3DTEXF_POINT,  D3DTEXF_POINT},
    {D3DTEXF_LINEAR, D3DTEXF_POINT},
    {D3DTEXF_POINT,  D3DTEXF_LINEAR},
    {D3DTEXF_LINEAR, D3DTEXF_LINEAR}
};

#define NUMCULLMODES    4
static D3DCULL _RwD3D8CullModeConvTable[NUMCULLMODES] =
{
    (D3DCULL)0,
    D3DCULL_NONE,
    D3DCULL_CW,
    D3DCULL_CCW
};

#define NUMSTENCILOPERATIONS    9
static D3DSTENCILOP _RwD3D8StencilOpConvTable[NUMSTENCILOPERATIONS] =
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
static D3DCMPFUNC _RwD3D8CmpFuncConvTable[NUMCMPFUNCTIONS] =
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
#define MAX_RENDERSTATE_CACHE   180

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
#define MAX_TEXTURESTAGESTATE_CACHE   32

static RxStateCache TextureStageStateCache[MAX_TEXTURE_STAGES][MAX_TEXTURESTAGESTATE_CACHE];

static RwUInt32 TextureStageStateD3D[MAX_TEXTURE_STAGES][MAX_TEXTURESTAGESTATE_CACHE];

typedef struct
{
    RwUInt32 stage;
    D3DTEXTURESTAGESTATETYPE type;
} RxTextureStageChangeType;

static RwUInt32 NumTextureStageStateChanges=0;
static RxTextureStageChangeType TextureStageStateChanges[MAX_TEXTURE_STAGES * MAX_TEXTURESTAGESTATE_CACHE];

#if defined(RWDEBUG)
static RwUInt32 RedundantTextureStagesStates=0;
#endif /* defined(RWDEBUG) */

/****************************************************************************
 rxD3D8RenderStateCacheClear

 */

static void
rxD3D8RenderStateCacheClear()
{
    RwUInt32 state;

    RWFUNCTION(RWSTRING("rxD3D8RenderStateCacheClear"));

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
 rxD3D8TextureStageStateCacheClear

 */

static void
rxD3D8TextureStageStateCacheClear()
{
    RwUInt32 stage, type;

    RWFUNCTION(RWSTRING("rxD3D8TextureStageStateCacheClear"));

    memset(TextureStageStateD3D, 0xffffffff, MAX_TEXTURE_STAGES * MAX_TEXTURESTAGESTATE_CACHE * sizeof(RwUInt32));

    /* Initialize the cache */
    for (stage = 0; stage < MAX_TEXTURE_STAGES; stage++)
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
 _rwD3D8RenderStateFlushCache

 */
void
_rwD3D8RenderStateFlushCache()
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateFlushCache"));

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
                IDirect3DDevice8_SetRenderState(_RwD3DDevice, stateChanged, newValue);

                RenderStateD3D[stateChanged] = newValue;
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
        _rwD3D8MetricsGet()->numRenderStateChanges += numRenderStateChangesNotRedundant;
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
                IDirect3DDevice8_SetTextureStageState(_RwD3DDevice, stage, type, newValue);

                TextureStageStateD3D[stage][type] = newValue;
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
        _rwD3D8MetricsGet()->numTextureStageStateChanges += numTextureStageStateChangesNotRedundant;
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
 * \ingroup d3d8renderstates
 * \ref RwD3D8SetRenderState sets a device renderstate parameter.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::SetRenderState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param state Device state variable that is being modified. This parameter
 * can be any member of the D3DRENDERSTATETYPE enumerated type.
 * \param value New value for the device renderstate to be set.
 * The meaning of this parameter is dependent on the value specified
 * for \e state.
 *
 * \return TRUE if the renderstate value was set, FALSE otherwise.
 *
 * \see RwD3D8GetRenderState
 * \see RwD3D8SetTextureStageState
 * \see RwD3D8GetTextureStageState
 *
 */
RwBool
RwD3D8SetRenderState(RwUInt32 state, RwUInt32 value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetRenderState"));

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

    RWRETURN(TRUE);
}

/**
 * \ingroup d3d8renderstates
 * \ref RwD3D8GetRenderState retrieves a renderstate value.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::GetRenderState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param state Device state variable that is being queried. This parameter
 * can be any member of the D3DRENDERSTATETYPE enumerated type.
 * \param value Void pointer to a variable that receives the value of
 * the queried renderstate variable when the function returns.
 *
 * \see RwD3D8SetRenderState
 * \see RwD3D8SetTextureStageState
 * \see RwD3D8GetTextureStageState
 *
 */
void
RwD3D8GetRenderState(RwUInt32 state, void *value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8GetRenderState"));

    RWASSERT(state < MAX_RENDERSTATE_CACHE);

    *((RwUInt32 *)value) = RenderStateCache[state].value;

    RWRETURNVOID();
}

/**
 * \ingroup d3d8renderstates
 * \ref RwD3D8SetTextureStageState sets the state value for the currently
 * assigned texture in the specified stage.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::SetTextureStageState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param stage Stage identifier of the texture for which the state value
 * is set. Stage identifiers are zero-based. Devices can have up to 8
 * textures set, so the maximum value allowed for \e stage is 7.
 * \param type Texture state to set. This parameter can be any member
 * of the D3DTEXTURESTAGESTATETYPE enumerated type.
 * \param value State value to set. The meaning of this value is determined
 * by the \e type parameter.
 *
 * \return TRUE if the texture stage state value was set, FALSE otherwise.
 *
 * \see RwD3D8GetTextureStageState
 * \see RwD3D8SetTexture
 * \see RwD3D8SetRenderState
 * \see RwD3D8GetRenderState
 *
 */
RwBool
RwD3D8SetTextureStageState(RwUInt32 stage, RwUInt32 type, RwUInt32 value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetTextureStageState"));

    RWASSERT((stage < MAX_TEXTURE_STAGES) && (type < MAX_TEXTURESTAGESTATE_CACHE));

    if (TextureStageStateCache[stage][type].value != value)
    {
        TextureStageStateCache[stage][type].value = value;

        if(!TextureStageStateCache[stage][type].changed)
        {
            TextureStageStateCache[stage][type].changed = TRUE;

            RWASSERT(NumTextureStageStateChanges <
                     (MAX_TEXTURE_STAGES * MAX_TEXTURESTAGESTATE_CACHE));

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

    RWRETURN(TRUE);
}

/**
 * \ingroup d3d8renderstates
 * \ref RwD3D8GetTextureStageState retrieves a state value for an assigned
 * texture in the specified stage.
 *
 * It is preferred that this utility function is used rather than the D3D8
 * API function IDirect3DDevice8::GetTextureStageState since RenderWare can
 * optimize performance using it's internal cache. See \ref d3d8cache.
 *
 * \param stage Stage identifier of the texture for which the state value
 * is retrieved. Stage identifiers are zero-based. Devices can have up to 8
 * textures set, so the maximum value allowed for \e stage is 7.
 * \param type Texture state to retrieve. This parameter can be any member
 * of the D3DTEXTURESTAGESTATETYPE enumerated type.
 * \param value Void pointer to a variable to fill with the retrieved state
 * value. The meaning of the retrieved value is determined by the \e type parameter.
 *
 * \see RwD3D8SetTextureStageState
 * \see RwD3D8SetTexture
 * \see RwD3D8SetRenderState
 * \see RwD3D8GetRenderState
 *
 */
void
RwD3D8GetTextureStageState(RwUInt32 stage, RwUInt32 type, void *value)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8GetTextureStageState"));

    RWASSERT((stage < MAX_TEXTURE_STAGES) && (type <MAX_TEXTURESTAGESTATE_CACHE));

    *((RwUInt32 *)value) = TextureStageStateCache[stage][type].value;

    RWRETURNVOID();
}

/****************************************************************************
 rwDlRenderStateOpen

 On entry   :
 On exit    :
 */
void
_rwD3D8RenderStateOpen(void)
{
    RwInt32 i, dither;

    RWFUNCTION(RWSTRING("_rwD3D8RenderStateOpen"));

    rxD3D8RenderStateCacheClear();
    rxD3D8TextureStageStateCacheClear();

    /* Fog */
    _RwD3D8StateCache.fogEnable = FALSE;
    _RwD3D8StateCache.fogType = rwFOGTYPELINEAR;
    _RwD3D8StateCache.fogDensity = 1.0f;
    _RwD3D8StateCache.fogColor = 0;

    RwD3D8SetRenderState(D3DRS_FOGENABLE, FALSE);

    if ((_RwD3D8DeviceCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0 &&
        (_RwD3D8DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG) != 0)
    {
        RwD3D8SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
        RwD3D8SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    }
    else
    {
        RwD3D8SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
        RwD3D8SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    }

    RwD3D8SetRenderState(D3DRS_FOGCOLOR, 0);

    /* Shading mode */
    RwD3D8SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    _RwD3D8StateCache.shadeMode = rwSHADEMODEGOURAUD;

    /* Set Z Compare mode */
    RwD3D8SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    RwD3D8SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    RwD3D8SetRenderState(D3DRS_ZENABLE, TRUE);

    _RwD3D8StateCache.zWriteEnable = TRUE;
    _RwD3D8StateCache.zTestEnable = TRUE;

    /* Stencil */
    RwD3D8SetRenderState(D3DRS_STENCILENABLE, FALSE);
    RwD3D8SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    RwD3D8SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    RwD3D8SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
    RwD3D8SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    RwD3D8SetRenderState(D3DRS_STENCILREF, 0);
    RwD3D8SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
    RwD3D8SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);

    _RwD3D8StateCache.stencilEnable = FALSE;
    _RwD3D8StateCache.stencilFailOp = rwSTENCILOPERATIONKEEP;
    _RwD3D8StateCache.stencilZFailOp = rwSTENCILOPERATIONKEEP;
    _RwD3D8StateCache.stencilPassOp = rwSTENCILOPERATIONKEEP;
    _RwD3D8StateCache.stencilFunction = rwSTENCILFUNCTIONALWAYS;
    _RwD3D8StateCache.stencilRef = 0;
    _RwD3D8StateCache.stencilMask = 0xFFFFFFFF;
    _RwD3D8StateCache.stencilWriteMask = 0xFFFFFFFF;

    /* Texture stages */
    for (i = 0; i < MAX_TEXTURE_STAGES; i++)
    {
        /* Texture */
        DXCHECK(IDirect3DDevice8_SetTexture(_RwD3DDevice, i, NULL));
        _RwD3D8StateCache.stage[i].curTexRaster = NULL;

        /* Texture filter mode - Liner/Trilinear */
        if(_RwD3D8DeviceCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR)
        {
            RwD3D8SetTextureStageState(i, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
            RwD3D8SetTextureStageState(i, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
            _RwD3D8StateCache.stage[i].filterMode = rwFILTERLINEAR;
        }
        else
        {
            RwD3D8SetTextureStageState(i, D3DTSS_MINFILTER, D3DTEXF_POINT);
            RwD3D8SetTextureStageState(i, D3DTSS_MAGFILTER, D3DTEXF_POINT);
            _RwD3D8StateCache.stage[i].filterMode = rwFILTERNEAREST;
        }

        RwD3D8SetTextureStageState(i, D3DTSS_MIPFILTER, D3DTEXF_NONE);

        /* Texture addressing mode */
        RwD3D8SetTextureStageState(i, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
        RwD3D8SetTextureStageState(i, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);

        _RwD3D8StateCache.stage[i].texAddressU = rwTEXTUREADDRESSWRAP;
        _RwD3D8StateCache.stage[i].texAddressV = rwTEXTUREADDRESSWRAP;

        /* Border color */
        RwD3D8SetTextureStageState(i, D3DTSS_BORDERCOLOR, 0xFF000000);
        _RwD3D8StateCache.stage[i].borderColor = 0xFF000000;

        /* Max Anisotropy Level: 1 to disable it */
        _RwD3D8StateCache.stage[i].maxAnisotropy = 1;
        RwD3D8SetTextureStageState(i, D3DTSS_MAXANISOTROPY, 1);

        /* Disable stage if not the first one */
        if (i)
        {
            RwD3D8SetTextureStageState(i, D3DTSS_COLOROP,   D3DTOP_DISABLE);
            RwD3D8SetTextureStageState(i, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
        }
    }

    /* Set default blending modes */
    RwD3D8SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

    RwD3D8SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    _RwD3D8StateCache.srcBlend = rwBLENDSRCALPHA;
    _RwD3D8StateCache.dstBlend = rwBLENDINVSRCALPHA;

    /* Set up the alpha compare function for future use */
    RwD3D8SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    _RwD3D8StateCache.alphaTestFunction = rwALPHATESTFUNCTIONGREATER;
    _RwD3D8StateCache.alphaTestEnable = TRUE;

    RwD3D8SetRenderState(D3DRS_ALPHAREF, 0x00000000);

    /* Disable alpha blending */
    RwD3D8SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    RwD3D8SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    _RwD3D8StateCache.vertexAlphaBlendEnable = FALSE;
    _RwD3D8StateCache.textureAlphaBlendEnable = FALSE;

    /* Turn lighting off */
    RwD3D8SetRenderState(D3DRS_LIGHTING, FALSE);

    /* Cull clockwise facing triangles */
    RwD3D8SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    _RwD3D8StateCache.cullMode = rwCULLMODECULLBACK;

    /* Vertex color only until a texture has been set */
    RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    /* Vertex alpha only until a texture has been set */
    RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    /* Diffuse, Specular & Ambient colors come from the material */
    RwD3D8SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

    RwD3D8SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);

    RwD3D8SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

    /* Use dithering if possible */
    dither = (_RwD3D8DeviceCaps.RasterCaps & D3DPRASTERCAPS_DITHER) ? TRUE : FALSE;
    RwD3D8SetRenderState(D3DRS_DITHERENABLE, dither);

    RwD3D8SetRenderState(D3DRS_SPECULARENABLE, FALSE);

    RwD3D8SetRenderState(D3DRS_LOCALVIEWER, FALSE);

    RwD3D8SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

    _rwD3D8RenderStateFlushCache();

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8RenderStateReset

 On entry   :
 On exit    :
 */
void
_rwD3D8RenderStateReset(void)
{
    RwUInt32 fogStart, fogEnd;
    RwUInt32 alphaRef;
    RwInt32 i, dither;

    RWFUNCTION(RWSTRING("_rwD3D8RenderStateReset"));

    /* Save some parameters */
    fogStart = RenderStateCache[D3DRS_FOGSTART].value;
    fogEnd = RenderStateCache[D3DRS_FOGEND].value;

    alphaRef = RenderStateCache[D3DRS_ALPHAREF].value;

    /* Clear renderstate state */
    rxD3D8RenderStateCacheClear();
    rxD3D8TextureStageStateCacheClear();

    /* Fog */
    RwD3D8SetRenderState(D3DRS_FOGDENSITY, FLOATASDWORD(_RwD3D8StateCache.fogDensity));

    RwD3D8SetRenderState(D3DRS_FOGENABLE, _RwD3D8StateCache.fogEnable);

    if ((_RwD3D8DeviceCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0 &&
        (_RwD3D8DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG) != 0)
    {
        RwD3D8SetRenderState(D3DRS_FOGTABLEMODE, _RwD3D8FogConvTable[_RwD3D8StateCache.fogType]);
        RwD3D8SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    }
    else
    {
        RwD3D8SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
        RwD3D8SetRenderState(D3DRS_FOGVERTEXMODE, _RwD3D8FogConvTable[_RwD3D8StateCache.fogType]);
    }

    RwD3D8SetRenderState(D3DRS_FOGSTART, fogStart);

    RwD3D8SetRenderState(D3DRS_FOGEND, fogEnd);

    RwD3D8SetRenderState(D3DRS_FOGCOLOR, _RwD3D8StateCache.fogColor);

    /* Shading mode */
    RwD3D8SetRenderState(D3DRS_SHADEMODE, _RwD3D8ShadeModeConvTable[_RwD3D8StateCache.shadeMode]);

    /* Set Z Compare mode */
    RwD3D8SetRenderState(D3DRS_ZWRITEENABLE, _RwD3D8StateCache.zWriteEnable);

    if (_RwD3D8StateCache.zTestEnable)
    {
        RwD3D8SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    }
    else
    {
        RwD3D8SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    }

    if (_RwD3D8StateCache.zTestEnable || _RwD3D8StateCache.zWriteEnable)
    {
        RwD3D8SetRenderState(D3DRS_ZENABLE, TRUE);
    }
    else
    {
        RwD3D8SetRenderState(D3DRS_ZENABLE, FALSE);
    }

    /* Stencil */
    RwD3D8SetRenderState(D3DRS_STENCILENABLE, _RwD3D8StateCache.stencilEnable);
    RwD3D8SetRenderState(D3DRS_STENCILFAIL,
                    _RwD3D8StencilOpConvTable[_RwD3D8StateCache.stencilFailOp]);
    RwD3D8SetRenderState(D3DRS_STENCILZFAIL,
                    _RwD3D8StencilOpConvTable[_RwD3D8StateCache.stencilZFailOp]);
    RwD3D8SetRenderState(D3DRS_STENCILPASS,
                    _RwD3D8StencilOpConvTable[_RwD3D8StateCache.stencilPassOp]);
    RwD3D8SetRenderState(D3DRS_STENCILFUNC,
                    _RwD3D8CmpFuncConvTable[_RwD3D8StateCache.stencilFunction]);
    RwD3D8SetRenderState(D3DRS_STENCILREF, _RwD3D8StateCache.stencilRef);
    RwD3D8SetRenderState(D3DRS_STENCILMASK, _RwD3D8StateCache.stencilMask);
    RwD3D8SetRenderState(D3DRS_STENCILWRITEMASK, _RwD3D8StateCache.stencilWriteMask);

    /* Texture states */
    for (i = 0; i < MAX_TEXTURE_STAGES; i++)
    {
        /* Texture */
        DXCHECK(IDirect3DDevice8_SetTexture(_RwD3DDevice, i, NULL));
        _RwD3D8StateCache.stage[i].curTexRaster = NULL;

        /* Texture filter mode - Liner/Trilinear */
        RwD3D8SetTextureStageState(i, D3DTSS_MINFILTER,
                                _RwD3D8FilterModeConvTable[_RwD3D8StateCache.stage[i].filterMode].minmag);
        RwD3D8SetTextureStageState(i, D3DTSS_MAGFILTER,
                                _RwD3D8FilterModeConvTable[_RwD3D8StateCache.stage[i].filterMode].minmag);
        RwD3D8SetTextureStageState(i, D3DTSS_MIPFILTER,
                                _RwD3D8FilterModeConvTable[_RwD3D8StateCache.stage[i].filterMode].mip);

        /* Texture addressing mode */
        RwD3D8SetTextureStageState(i, D3DTSS_ADDRESSU,
                                       _RwD3D8AddressConvTable[_RwD3D8StateCache.stage[i].texAddressU]);

        RwD3D8SetTextureStageState(i, D3DTSS_ADDRESSV,
                                _RwD3D8AddressConvTable[_RwD3D8StateCache.stage[i].texAddressV]);

        /* Border color */
        RwD3D8SetTextureStageState(i, D3DTSS_BORDERCOLOR,
                                _RwD3D8StateCache.stage[i].borderColor);

        /* Max Anisotropy Level: 1 to disable it */
        _RwD3D8StateCache.stage[i].maxAnisotropy = 1;
        RwD3D8SetTextureStageState(i, D3DTSS_MAXANISOTROPY, 1);

        /* Disable stage if not the first one */
        if (i)
        {
            RwD3D8SetTextureStageState(i, D3DTSS_COLOROP,   D3DTOP_DISABLE);
            RwD3D8SetTextureStageState(i, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
        }
    }

    /* Set default blending modes */
    RwD3D8SetRenderState(D3DRS_SRCBLEND,
                         _RwD3D8BlendConvTable[_RwD3D8StateCache.srcBlend]);

    RwD3D8SetRenderState(D3DRS_DESTBLEND,
                         _RwD3D8BlendConvTable[_RwD3D8StateCache.dstBlend]);

    /* Set up the alpha compare function for future use */
    RwD3D8SetRenderState(D3DRS_ALPHAFUNC, _RwD3D8CmpFuncConvTable[_RwD3D8StateCache.alphaTestFunction]);
    RwD3D8SetRenderState(D3DRS_ALPHAREF, alphaRef);

    /* Disable alpha blending */
    if (_RwD3D8StateCache.vertexAlphaBlendEnable)
    {
        RwD3D8SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        RwD3D8SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D8StateCache.alphaTestEnable);
    }
    else
    {
        RwD3D8SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        RwD3D8SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }

    _RwD3D8StateCache.textureAlphaBlendEnable = FALSE;

    /* Turn lighting off */
    RwD3D8SetRenderState(D3DRS_LIGHTING, FALSE);

    /* Cull clockwise facing triangles */
    RwD3D8SetRenderState(D3DRS_CULLMODE, _RwD3D8CullModeConvTable[_RwD3D8StateCache.cullMode]);

    /* Vertex color only until a texture has been set */
    RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
    RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    /* Vertex alpha only until a texture has been set */
    if (_RwD3D8StateCache.vertexAlphaBlendEnable)
    {
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }
    else
    {
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }

    /* Diffuse, Specular & Ambient colors come from the material */
    RwD3D8SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

    RwD3D8SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);

    RwD3D8SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

    /* Use dithering if possible */
    dither = (_RwD3D8DeviceCaps.RasterCaps & D3DPRASTERCAPS_DITHER) ? TRUE : FALSE;
    RwD3D8SetRenderState(D3DRS_DITHERENABLE, dither);

    RwD3D8SetRenderState(D3DRS_SPECULARENABLE, FALSE);

    RwD3D8SetRenderState(D3DRS_LOCALVIEWER, FALSE);

    RwD3D8SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

    _rwD3D8RenderStateFlushCache();

    RWRETURNVOID();
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                            Getting Render States

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwD3D8RWGetRenderState

 On entry   : Render state to get.
            : Parameter (OUT).
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RWGetRenderState(RwRenderState nState, void *param)
{
    RWFUNCTION(RWSTRING("_rwD3D8RWGetRenderState"));

    switch (nState)
    {
        /******************************* FOG ********************************/
        case rwRENDERSTATEFOGENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D8StateCache.fogEnable;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEFOGTYPE:
        {
            /* Return our conveniently cached value */
            *(RwFogType *)param = _RwD3D8StateCache.fogType;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEFOGCOLOR:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D8StateCache.fogColor;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEFOGDENSITY:
        {
            /* Return our conveniently cached value */
            *(RwReal *)param = _RwD3D8StateCache.fogDensity;

            RWRETURN(TRUE);
        }

        /************************ TEXTURE ADDRESSING ************************/

        case rwRENDERSTATETEXTUREADDRESS:
        {
            if (_RwD3D8StateCache.stage[0].texAddressU == _RwD3D8StateCache.stage[0].texAddressV)
            {
                /* Return our conveniently cached value */
                *(RwTextureAddressMode *)param = _RwD3D8StateCache.stage[0].texAddressU;

                RWRETURN(TRUE);
            }

            /* Return and invalid value */
            *(RwTextureAddressMode *)param = rwTEXTUREADDRESSNATEXTUREADDRESS;

            RWRETURN(FALSE);
        }

        case rwRENDERSTATETEXTUREADDRESSU:
        {
            /* Return our conveniently cached value */
            *(RwTextureAddressMode *)param = _RwD3D8StateCache.stage[0].texAddressU;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATETEXTUREADDRESSV:
        {
            /* Return our conveniently cached value */
            *(RwTextureAddressMode *)param = _RwD3D8StateCache.stage[0].texAddressV;

            RWRETURN(TRUE);
        }

        /*********************** TEXTURE FILTER MODE ************************/

        case rwRENDERSTATETEXTUREFILTER:
        {
            /* Return our conveniently cached value */
            *(RwTextureFilterMode *)param = _RwD3D8StateCache.stage[0].filterMode;

            RWRETURN(TRUE);
        }

        /************************** TEXTURE RASTER **************************/

        case rwRENDERSTATETEXTURERASTER:
        {
            /* Return our conveniently cached value */
            *(RwRaster **)param = _RwD3D8StateCache.stage[0].curTexRaster;

            RWRETURN(TRUE);
        }

        /************************** Z BUFFER WRITE **************************/

        case rwRENDERSTATEZWRITEENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D8StateCache.zWriteEnable;

            RWRETURN(TRUE);
        }

        /************************** Z BUFFER TEST ***************************/

        case rwRENDERSTATEZTESTENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D8StateCache.zTestEnable;

            RWRETURN(TRUE);
        }

        /************************** BLENDING MODES **************************/

        case rwRENDERSTATESRCBLEND:
        {
            /* Return our conveniently cached value */
            *(RwBlendFunction *)param = _RwD3D8StateCache.srcBlend;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEDESTBLEND:
        {
            /* Return our conveniently cached value */
            *(RwBlendFunction *)param = _RwD3D8StateCache.dstBlend;

            RWRETURN(TRUE);
        }

        /*************************** SHADING MODE ***************************/

        case rwRENDERSTATESHADEMODE:
        {
            /* Return our conveniently cached value */
            *(RwShadeMode *)param = _RwD3D8StateCache.shadeMode;

            RWRETURN(TRUE);
        }

        /*************************** VERTEX ALPHA ***************************/

        case rwRENDERSTATEVERTEXALPHAENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D8StateCache.vertexAlphaBlendEnable;

            RWRETURN(TRUE);
        }

        /************************** BORDER COLOR ****************************/

        case rwRENDERSTATEBORDERCOLOR:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D8StateCache.stage[0].borderColor;

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
            *(RwCullMode *)param = _RwD3D8StateCache.cullMode;

            RWRETURN(TRUE);
        }

        /************************** STENCIL BUFFER **************************/

        case rwRENDERSTATESTENCILENABLE:
        {
            /* Return our conveniently cached value */
            *(RwBool *)param = _RwD3D8StateCache.stencilEnable;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFAIL:
        {
            /* Return our conveniently cached value */
            *(RwStencilOperation *)param = _RwD3D8StateCache.stencilFailOp;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILZFAIL:
        {
            /* Return our conveniently cached value */
            *(RwStencilOperation *)param = _RwD3D8StateCache.stencilZFailOp;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILPASS:
        {
            /* Return our conveniently cached value */
            *(RwStencilOperation *)param = _RwD3D8StateCache.stencilPassOp;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFUNCTION:
        {
            /* Return our conveniently cached value */
            *(RwStencilFunction *)param = _RwD3D8StateCache.stencilFunction;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFUNCTIONREF:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D8StateCache.stencilRef;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFUNCTIONMASK:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D8StateCache.stencilMask;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATESTENCILFUNCTIONWRITEMASK:
        {
            /* Return our conveniently cached value */
            *(RwInt32 *)param = _RwD3D8StateCache.stencilWriteMask;

            RWRETURN(TRUE);
        }

        case rwRENDERSTATEALPHATESTFUNCTION:
        {
            /* Return our conveniently cached value */
            *(RwAlphaTestFunction *)param = _RwD3D8StateCache.alphaTestFunction;

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
 rwD3D8RenderStateFogEnable

 On entry   : A RwBool value defining the desired fog state.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateFogEnable(RwBool fog)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateFogEnable"));

    if (fog)
    {
        if (!_RwD3D8StateCache.fogEnable)
        {
            if (_RwD3D8DeviceCaps.RasterCaps & (D3DPRASTERCAPS_FOGTABLE | D3DPRASTERCAPS_FOGVERTEX))
            {
                FORCE_RENDERSTATE(D3DRS_FOGENABLE, TRUE);
                _RwD3D8StateCache.fogEnable = TRUE;
            }
        }
    }
    else
    {
        if (_RwD3D8StateCache.fogEnable)
        {
            FORCE_RENDERSTATE(D3DRS_FOGENABLE, FALSE);
            _RwD3D8StateCache.fogEnable = FALSE;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8RenderStateFogColor

 On entry   : A RwUInt32 value defining the desired fog color.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateFogColor(RwUInt32 fogColor)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateFogColor"));

    if (fogColor != _RwD3D8StateCache.fogColor)
    {
        FORCE_RENDERSTATE(D3DRS_FOGCOLOR, fogColor);
        _RwD3D8StateCache.fogColor = fogColor;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8RenderStateFogType

 On entry   : A RwFogType value defining the desired fog type.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateFogType(RwFogType fogType)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateFogType"));
    RWASSERT(fogType < NUMFOGMODES);
    RWASSERT(fogType >= 0);

    if (fogType != _RwD3D8StateCache.fogType)
    {
        if ((_RwD3D8DeviceCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0 &&
            (_RwD3D8DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG) != 0)
        {
            FORCE_RENDERSTATE(D3DRS_FOGTABLEMODE,
                                     _RwD3D8FogConvTable[fogType]);

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
                                     _RwD3D8FogConvTable[fogType]);
        }

        _RwD3D8StateCache.fogType = fogType;
    }

    if (fogType == rwFOGTYPELINEAR)
    {
        const RwReal    fogPlane  = dgGGlobals.curCamera->fogPlane;
        const RwReal    farPlane  = dgGGlobals.curCamera->farPlane;

        if (RenderStateCache[D3DRS_FOGSTART].value != FLOATASDWORD(fogPlane))
        {
            FORCE_RENDERSTATE(D3DRS_FOGSTART, FLOATASDWORD(fogPlane));
        }

        if (RenderStateCache[D3DRS_FOGEND].value != FLOATASDWORD(farPlane))
        {
            FORCE_RENDERSTATE(D3DRS_FOGEND, FLOATASDWORD(farPlane));
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8RenderStateFogDensity

 On entry   : A RwReal value defining the desired fog density.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateFogDensity(RwReal fogDensity)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateFogDensity"));

    if (fogDensity != _RwD3D8StateCache.fogDensity)
    {
        FORCE_RENDERSTATE(D3DRS_FOGDENSITY, FLOATASDWORD(fogDensity));
        _RwD3D8StateCache.fogDensity = fogDensity;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateTextureAddress

 On entry   : A RwTextureAddressMode value defining the desired texture
              addressing mode.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateTextureAddress(RwTextureAddressMode addressMode)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateTextureAddress"));
    RWASSERT(addressMode < NUMADDRESSMODES);
    RWASSERT(addressMode >= 0);

    if (addressMode != _RwD3D8StateCache.stage[0].texAddressU)
    {
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ADDRESSU,
                                       _RwD3D8AddressConvTable[addressMode]);

        _RwD3D8StateCache.stage[0].texAddressU = addressMode;
    }

    if (addressMode != _RwD3D8StateCache.stage[0].texAddressV)
    {
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ADDRESSV,
                                       _RwD3D8AddressConvTable[addressMode]);

        _RwD3D8StateCache.stage[0].texAddressV = addressMode;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateTextureAddressU

 On entry   : A RwTextureAddressMode value defining the desired U texture
              addressing mode.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateTextureAddressU(RwTextureAddressMode addressMode)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateTextureAddressU"));
    RWASSERT(addressMode < NUMADDRESSMODES);
    RWASSERT(addressMode >= 0);

    if (addressMode != _RwD3D8StateCache.stage[0].texAddressU)
    {
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ADDRESSU,
                                       _RwD3D8AddressConvTable[addressMode]);

        _RwD3D8StateCache.stage[0].texAddressU = addressMode;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateTextureAddressV

 On entry   : A RwTextureAddressMode value defining the desired V texture
              addressing mode.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateTextureAddressV(RwTextureAddressMode addressMode)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateTextureAddressV"));
    RWASSERT(addressMode < NUMADDRESSMODES);
    RWASSERT(addressMode >= 0);

    if (addressMode != _RwD3D8StateCache.stage[0].texAddressV)
    {
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_ADDRESSV,
                                       _RwD3D8AddressConvTable[addressMode]);

        _RwD3D8StateCache.stage[0].texAddressV = addressMode;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateTextureFilter

 On entry   : A RwTextureFilterMode value defining the desired filter mode.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateTextureFilter(RwTextureFilterMode filterMode)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateTextureFilter"));
    RWASSERT(filterMode < NUMFILTERMODES);
    RWASSERT(filterMode >= 0);

    if (_RwD3D8StateCache.stage[0].maxAnisotropy > 1)
    {
        RwD3D8SetTextureStageState(0, D3DTSS_MAXANISOTROPY, 1);

        _RwD3D8StateCache.stage[0].maxAnisotropy = 1;

        /* Force refresh filter mode */
        _RwD3D8StateCache.stage[0].filterMode = (RwTextureFilterMode)0xffffffff;
    }

    if (filterMode != _RwD3D8StateCache.stage[0].filterMode)
    {
        RwD3D8SetTextureStageState(0, D3DTSS_MINFILTER,
                                _RwD3D8FilterModeConvTable[filterMode].minmag);

        RwD3D8SetTextureStageState(0, D3DTSS_MAGFILTER,
                                _RwD3D8FilterModeConvTable[filterMode].minmag);

        RwD3D8SetTextureStageState(0, D3DTSS_MIPFILTER,
                                _RwD3D8FilterModeConvTable[filterMode].mip);

        _RwD3D8StateCache.stage[0].filterMode = filterMode;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateSrcBlend

 On entry   : A RwBlendFunction value defining the blend state.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateSrcBlend(RwBlendFunction srcBlend)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateSrcBlend"));
    RWASSERT(srcBlend < NUMBLENDMODES);
    RWASSERT(srcBlend >= 0);

    if (srcBlend != _RwD3D8StateCache.srcBlend)
    {
        FORCE_RENDERSTATE(D3DRS_SRCBLEND,
                                 _RwD3D8BlendConvTable[srcBlend]);

        _RwD3D8StateCache.srcBlend = srcBlend;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateDestBlend

 On entry   : A RwBlendFunction value defining the blend state.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateDestBlend(RwBlendFunction dstBlend)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateDestBlend"));
    RWASSERT(dstBlend < NUMBLENDMODES);
    RWASSERT(dstBlend >= 0);

    if (dstBlend != _RwD3D8StateCache.dstBlend)
    {
        FORCE_RENDERSTATE(D3DRS_DESTBLEND,
                                 _RwD3D8BlendConvTable[dstBlend]);

        _RwD3D8StateCache.dstBlend = dstBlend;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RWSetRasterStage

 On entry   : A RwRaster pointer for the desired texture.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RWSetRasterStage(RwRaster *raster, RwUInt32 stage)
{
    _rwD3D8RasterExt  *rasExt = (_rwD3D8RasterExt *) NULL;

    RWFUNCTION(RWSTRING("_rwD3D8RWSetRasterStage"));
    RWASSERT(stage < MAX_TEXTURE_STAGES);

#if defined(RWDEBUG)
    if (_RwD3D8DeviceCaps.MaxTextureBlendStages <= stage)
    {
        RwChar              buffer[256];

        rwsprintf(buffer, "You are using %d stages. This video card only supports %d stages.",
                  (stage + 1), _RwD3D8DeviceCaps.MaxTextureBlendStages);

        RwDebugSendMessage(rwDEBUGMESSAGE,
                           "_rwD3D8RWSetRasterStage",
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
    }

    if (stage == 0)
    {
        if (raster && rasExt->texture && rasExt->alpha)
        {
            /*
             * Enable Alpha Blending if the texture has alpha
             */
            if (!_RwD3D8StateCache.textureAlphaBlendEnable)
            {
                _RwD3D8StateCache.textureAlphaBlendEnable = TRUE;

                if (!_RwD3D8StateCache.vertexAlphaBlendEnable)
                {
                    RwD3D8SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                    RwD3D8SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D8StateCache.alphaTestEnable);
                }
            }
        }
        else
        {
            if (_RwD3D8StateCache.textureAlphaBlendEnable)
            {
                _RwD3D8StateCache.textureAlphaBlendEnable = FALSE;

                /*
                 * Disable alpha blending if it has not been explicity set
                 */
                if (!_RwD3D8StateCache.vertexAlphaBlendEnable)
                {
                    RwD3D8SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                    RwD3D8SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
                }
            }
        }
    }

    /*
     * Check color
     */
    if (raster != _RwD3D8StateCache.stage[stage].curTexRaster)
    {
        if (raster && rasExt->texture)
        {
            /* Set the texture */
            DXCHECK(IDirect3DDevice8_SetTexture(_RwD3DDevice, stage,
                                 (LPDIRECT3DBASETEXTURE8)(rasExt->texture)));

            /* Set the palette if it is a palettized texture */
            if (rasExt->palette)
            {
                DXCHECK(IDirect3DDevice8_SetCurrentTexturePalette(_RwD3DDevice, rasExt->palette->globalindex));
            }

            if (stage == 0)
            {
                /* Color */
                RwD3D8SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
                RwD3D8SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                RwD3D8SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

                /* Alpha */
                if (_RwD3D8StateCache.textureAlphaBlendEnable)
                {
                    if (_RwD3D8StateCache.vertexAlphaBlendEnable)
                    {
                        RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
                        RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                        RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                    }
                    else
                    {
                        RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
                        RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                    }
                }
                else
                {
                    if (_RwD3D8StateCache.vertexAlphaBlendEnable)
                    {
                        RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
                        RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                    }
                    else
                    {
                        RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
                    }
                }
            }
            else
            {
                /* Color */
                RwD3D8SetTextureStageState(stage, D3DTSS_COLOROP,   D3DTOP_MODULATE);
                RwD3D8SetTextureStageState(stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                RwD3D8SetTextureStageState(stage, D3DTSS_COLORARG2, D3DTA_CURRENT);

                /* Alpha */
                if (_RwD3D8StateCache.textureAlphaBlendEnable)
                {
                    if (rasExt->alpha)
                    {
                        RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
                        RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                        RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
                    }
                    else
                    {
                        RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
                        RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
                    }
                }
                else
                {
                    if (_RwD3D8StateCache.vertexAlphaBlendEnable)
                    {
                        RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
                        RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
                    }
                    else
                    {
                        RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
                    }
                }
            }

            #if defined( RWMETRICS )
            RWSRCGLOBAL(metrics)->numTextureUploads+=1;
            RWSRCGLOBAL(metrics)->sizeTextureUploads+=raster->width * raster->height * (raster->depth / 8);
            #endif
        }
        else
        {
            DXCHECK(IDirect3DDevice8_SetTexture(_RwD3DDevice, stage, NULL));

            if (stage == 0)
            {
                /* Color */
                RwD3D8SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
                RwD3D8SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

                /* Alpha */
                if (_RwD3D8StateCache.vertexAlphaBlendEnable)
                {
                    RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
                    RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                }
                else
                {
                    RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
                }
            }
            else
            {
                RwD3D8SetTextureStageState(stage, D3DTSS_COLOROP,   D3DTOP_DISABLE);
                RwD3D8SetTextureStageState(stage, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
            }
        }

        _RwD3D8StateCache.stage[stage].curTexRaster = raster;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RWGetRasterStage

 On exit    : The raster set in the current stage
 */
RwRaster *
_rwD3D8RWGetRasterStage(RwUInt32 stage)
{
    RWFUNCTION(RWSTRING("_rwD3D8RWGetRasterStage"));
    RWASSERT(stage < MAX_TEXTURE_STAGES);

    RWRETURN(_RwD3D8StateCache.stage[stage].curTexRaster);
}

/****************************************************************************
 _rwD3D8SetTextureAnisotropyOffset

 Set the anisotropy texture offset that enables the anisotropy plugin
 */
void
_rwD3D8SetTextureAnisotropyOffset(RwInt32 anisotTextureOffset)
{
    RWFUNCTION(RWSTRING("_rwD3D8SetTextureAnisotropyOffset"));

    RWASSERT(anisotTextureOffset > 0);

    D3D8AnisotTextureOffset = anisotTextureOffset;

    RWRETURNVOID();
}

/**
 * \ingroup rwengined3d8
 * \ref RwD3D8SetTexture assigns a texture to a stage.
 *
 * This function may change the following texture states for the
 * selected stage, if the states stored in the \ref rwtexture do not match
 * those cached in the D3D8 driver:
 * \li D3DTSS_ADDRESSU
 * \li D3DTSS_ADDRESSV
 * \li D3DTSS_MINFILTER
 * \li D3DTSS_MAGFILTER
 * \li D3DTSS_MIPFILTER
 * \li D3DTSS_MAXANISOTROPY
 * \li D3DTSS_COLOROP
 * \li D3DTSS_COLORARG1
 * \li D3DTSS_COLORARG2
 * \li D3DTSS_ALPHAOP
 * \li D3DTSS_ALPHAARG1
 * \li D3DTSS_ALPHAARG2
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
 * \see RwD3D8SetTextureStageState
 * \see RwD3D8GetTextureStageState
 *
 */
RwBool
RwD3D8SetTexture(RwTexture *texture, RwUInt32 stage)
{
    RWAPIFUNCTION(RWSTRING("RwD3D8SetTexture"));
    RWASSERT(stage < MAX_TEXTURE_STAGES);

    if (texture)
    {
        RwTextureFilterMode     filtering;
        RwTextureAddressMode    addressingU;
        RwTextureAddressMode    addressingV;
        RwInt32                 anisotropy = 1;

        addressingU = RwTextureGetAddressingU(texture);
        addressingV = RwTextureGetAddressingV(texture);
        filtering = RwTextureGetFilterMode(texture);

        /* Raster */
        _rwD3D8RWSetRasterStage(texture->raster, stage);

        /* Addressing mode */
        if (addressingU != _RwD3D8StateCache.stage[stage].texAddressU)
        {
            RwD3D8SetTextureStageState(stage, D3DTSS_ADDRESSU,
                                           _RwD3D8AddressConvTable[addressingU]);

            _RwD3D8StateCache.stage[stage].texAddressU = addressingU;
        }

        if (addressingV != _RwD3D8StateCache.stage[stage].texAddressV)
        {
            RwD3D8SetTextureStageState(stage, D3DTSS_ADDRESSV,
                                           _RwD3D8AddressConvTable[addressingV]);

            _RwD3D8StateCache.stage[stage].texAddressV = addressingV;
        }

        /* Filter mode */
        if (D3D8AnisotTextureOffset > 0)
        {
            anisotropy = CONSTTEXTUREGETANISOT(texture);
        }

        if (anisotropy != _RwD3D8StateCache.stage[stage].maxAnisotropy)
        {
            RwD3D8SetTextureStageState(stage, D3DTSS_MAXANISOTROPY, anisotropy);

            if (anisotropy > 1)
            {
                /* Overwrite texture filter */
                RwD3D8SetTextureStageState(stage, D3DTSS_MINFILTER, D3DTEXF_ANISOTROPIC);

                RwD3D8SetTextureStageState(stage, D3DTSS_MAGFILTER, D3DTEXF_ANISOTROPIC);

                RwD3D8SetTextureStageState(stage, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
            }
            else
            {
                /* restore texture filtering */
                RwD3D8SetTextureStageState(stage, D3DTSS_MINFILTER,
                                        _RwD3D8FilterModeConvTable[filtering].minmag);

                RwD3D8SetTextureStageState(stage, D3DTSS_MAGFILTER,
                                        _RwD3D8FilterModeConvTable[filtering].minmag);

                RwD3D8SetTextureStageState(stage, D3DTSS_MIPFILTER,
                                        _RwD3D8FilterModeConvTable[filtering].mip);

                _RwD3D8StateCache.stage[stage].filterMode = filtering;
            }

            _RwD3D8StateCache.stage[stage].maxAnisotropy = anisotropy;
        }
        else
        {
            if (filtering != _RwD3D8StateCache.stage[stage].filterMode)
            {
                if (anisotropy <= 1)
                {
                    RwD3D8SetTextureStageState(stage, D3DTSS_MINFILTER,
                                            _RwD3D8FilterModeConvTable[filtering].minmag);

                    RwD3D8SetTextureStageState(stage, D3DTSS_MAGFILTER,
                                            _RwD3D8FilterModeConvTable[filtering].minmag);

                    RwD3D8SetTextureStageState(stage, D3DTSS_MIPFILTER,
                                            _RwD3D8FilterModeConvTable[filtering].mip);
                }

                _RwD3D8StateCache.stage[stage].filterMode = filtering;
            }
        }

        /* Check texture parameters */
        #if defined(RWDEBUG)
        if (texture->raster != NULL)
        {
            /* Non-pow2 textures only suppor CLAMP */
            if (addressingU != rwTEXTUREADDRESSCLAMP ||
                addressingV != rwTEXTUREADDRESSCLAMP)
            {
                if ( texture->raster->width != (1 << _rwD3D8FindMSB(texture->raster->width)) ||
                    texture->raster->height != (1 << _rwD3D8FindMSB(texture->raster->height)))
                {
                    RwChar              buffer[256];

                    rwsprintf(buffer, "'%s' has non power of 2 dimensions but is using an addressing mode different than rwTEXTUREADDRESSCLAMP, which is not supported.",
                            texture->name);

                    RwDebugSendMessage(rwDEBUGMESSAGE,
                                    "RwD3D8SetTexture",
                                    buffer);
                }
            }
        }
        #endif
    }
    else
    {
        /* Raster */
        _rwD3D8RWSetRasterStage(NULL, stage);

        /* Remove anisotropy */
        if (_RwD3D8StateCache.stage[stage].maxAnisotropy > 1)
        {
            const RwTextureFilterMode filtering = _RwD3D8StateCache.stage[stage].filterMode;

            RwD3D8SetTextureStageState(stage, D3DTSS_MAXANISOTROPY, 1);

            RwD3D8SetTextureStageState(stage, D3DTSS_MINFILTER,
                                    _RwD3D8FilterModeConvTable[filtering].minmag);

            RwD3D8SetTextureStageState(stage, D3DTSS_MAGFILTER,
                                    _RwD3D8FilterModeConvTable[filtering].minmag);

            RwD3D8SetTextureStageState(stage, D3DTSS_MIPFILTER,
                                    _RwD3D8FilterModeConvTable[filtering].mip);

            _RwD3D8StateCache.stage[stage].maxAnisotropy = 1;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8RenderStateZWriteEnable

 On entry   : A RwBool value defining the desired Z-write state.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateZWriteEnable(RwBool enable)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateZWriteEnable"));

    if (enable)
    {
        if (!_RwD3D8StateCache.zWriteEnable)
        {
            if (!_RwD3D8StateCache.zTestEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ZENABLE, TRUE);
            }

            FORCE_RENDERSTATE(D3DRS_ZWRITEENABLE, TRUE);

            _RwD3D8StateCache.zWriteEnable = TRUE;
        }
    }
    else
    {
        if (_RwD3D8StateCache.zWriteEnable)
        {
            if (!_RwD3D8StateCache.zTestEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ZENABLE, FALSE);
            }

            FORCE_RENDERSTATE(D3DRS_ZWRITEENABLE, FALSE);

            _RwD3D8StateCache.zWriteEnable = FALSE;
        }
    }
    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8RenderStateZTestEnable

 On entry   : A RwBool value defining the desired Z-test state.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateZTestEnable(RwBool enable)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateZTestEnable"));

    if (enable)
    {
        if (!_RwD3D8StateCache.zTestEnable)
        {
            if (!_RwD3D8StateCache.zWriteEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ZENABLE, TRUE);
            }

            FORCE_RENDERSTATE(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

            _RwD3D8StateCache.zTestEnable = TRUE;
        }
    }
    else
    {
        if (_RwD3D8StateCache.zTestEnable)
        {
            if (!_RwD3D8StateCache.zWriteEnable)
            {
                FORCE_RENDERSTATE(D3DRS_ZENABLE, FALSE);
            }

            FORCE_RENDERSTATE(D3DRS_ZFUNC, D3DCMP_ALWAYS);

            _RwD3D8StateCache.zTestEnable = FALSE;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8RenderStateShadeMode

 On entry   : A RwShadeMode value defining the desired shading mode.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateShadeMode(RwShadeMode shadeMode)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateShadeMode"));
    RWASSERT(shadeMode < NUMSHADEMODES);
    RWASSERT(shadeMode >= 0);

    if (shadeMode != _RwD3D8StateCache.shadeMode)
    {
        FORCE_RENDERSTATE(D3DRS_SHADEMODE,
                                 _RwD3D8ShadeModeConvTable[shadeMode]);
        _RwD3D8StateCache.shadeMode = shadeMode;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateVertexAlphaEnable

 On entry   : A RwBool value defining the desired vertex alpha state.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateVertexAlphaEnable(RwBool enable)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateVertexAlphaEnable"));

    if (enable)
    {
        if (!_RwD3D8StateCache.vertexAlphaBlendEnable)
        {
            if (!_RwD3D8StateCache.textureAlphaBlendEnable)
            {
                RwD3D8SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                RwD3D8SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D8StateCache.alphaTestEnable);
            }

            if (_RwD3D8StateCache.textureAlphaBlendEnable)
            {
                RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
                RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
            }
            else
            {
                RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
                RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
            }

            _RwD3D8StateCache.vertexAlphaBlendEnable = TRUE;
        }
    }
    else
    {
        if (_RwD3D8StateCache.vertexAlphaBlendEnable)
        {
            if (!_RwD3D8StateCache.textureAlphaBlendEnable)
            {
                RwD3D8SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                RwD3D8SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
            }

            if (_RwD3D8StateCache.textureAlphaBlendEnable)
            {
                RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
                RwD3D8SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
            }
            else
            {
                RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
            }

            _RwD3D8StateCache.vertexAlphaBlendEnable = FALSE;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateIsVertexAlphaEnable

  On exit    : TRUE is vertex alpha blend is enabled.
 */
RwBool
_rwD3D8RenderStateIsVertexAlphaEnable(void)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateIsVertexAlphaEnable"));

    RWRETURN(_RwD3D8StateCache.vertexAlphaBlendEnable);
}

/****************************************************************************
 rwD3D8RenderStateBorderColor

 On entry   : A RwInt32 value defining the border color.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateBorderColor(RwUInt32 borderColor)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateBorderColor"));

    if (borderColor != _RwD3D8StateCache.stage[0].borderColor)
    {
        RwD3D8SetTextureStageState(TEXTURE_STAGE_0, D3DTSS_BORDERCOLOR,
                                       (D3DCOLOR)borderColor);

        _RwD3D8StateCache.stage[0].borderColor = borderColor;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 rwD3D8RenderStateTexturePerspective

 On entry   : A RwBool value defining the perspective texturing state.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateTexturePerspective(RwBool persp)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateTexturePerspective"));

    /* Only can set on */
    if (persp)
    {
        RWRETURN(TRUE);
    }

    /* If trying to set off, fail */
    RWRETURN(FALSE);
}

/****************************************************************************
 rwD3D8RenderStateCullMode

 On entry   : A RwCullMode value defining the culling mode to use.
 On exit    : TRUE on success else FALSE.
 */
static RwBool
rwD3D8RenderStateCullMode(RwCullMode cullMode)
{
    RWFUNCTION(RWSTRING("rwD3D8RenderStateCullMode"));
    RWASSERT(cullMode < NUMCULLMODES);
    RWASSERT(cullMode >= 0);

    if (cullMode != _RwD3D8StateCache.cullMode)
    {
        FORCE_RENDERSTATE(D3DRS_CULLMODE, _RwD3D8CullModeConvTable[cullMode]);

        _RwD3D8StateCache.cullMode = cullMode;
    }

    RWRETURN(TRUE);
}


/****************************************************************************
 _rwD3D8RenderStateStencilEnable

 On entry   : A RwBool value defining the stencil buffer state.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateStencilEnable(RwBool enable)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateStencilEnable"));

    if (enable != _RwD3D8StateCache.stencilEnable)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILENABLE, enable);

        _RwD3D8StateCache.stencilEnable = enable;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateStencilFail

 On entry   : A RwStencilOperation value defining the operation to perform if
              the stencil test fails.
 On exit    : TRUE on success else FALSE.
*/
RwBool
_rwD3D8RenderStateStencilFail(RwStencilOperation operation)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateStencilFail"));
    RWASSERT(operation < NUMSTENCILOPERATIONS);
    RWASSERT(operation >= 0);

    if (operation != _RwD3D8StateCache.stencilFailOp)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILFAIL,
            _RwD3D8StencilOpConvTable[operation]);

        _RwD3D8StateCache.stencilFailOp = operation;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateStencilZFail

 On entry   : A RwStencilOperation value defining the operation to perform if
              the stencil test passes and the depth test (z-test) fails.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateStencilZFail(RwStencilOperation operation)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateStencilZFail"));
    RWASSERT(operation < NUMSTENCILOPERATIONS);
    RWASSERT(operation >= 0);

    if (operation != _RwD3D8StateCache.stencilZFailOp)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILZFAIL,
            _RwD3D8StencilOpConvTable[operation]);

        _RwD3D8StateCache.stencilZFailOp = operation;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateStencilPass

 On entry   : A RwStencilOperation value defining the operation to perform if
              both the stencil and the depth (z) tests pass.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateStencilPass(RwStencilOperation operation)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateStencilPass"));
    RWASSERT(operation < NUMSTENCILOPERATIONS);
    RWASSERT(operation >= 0);

    if (operation != _RwD3D8StateCache.stencilPassOp)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILPASS,
            _RwD3D8StencilOpConvTable[operation]);

        _RwD3D8StateCache.stencilPassOp = operation;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateStencilFunction

 On entry   : A RwStencilFunction value defining the comparison function for
              the stencil test.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateStencilFunction(RwStencilFunction function)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateStencilFunction"));
    RWASSERT(function < NUMCMPFUNCTIONS);
    RWASSERT(function >= 0);

    if (function != _RwD3D8StateCache.stencilFunction)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILFUNC,
            _RwD3D8CmpFuncConvTable[function]);

        _RwD3D8StateCache.stencilFunction = function;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateStencilFunctionRef

 On entry   : A RwInt32 value defining the reference value for the stencil
              test.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateStencilFunctionRef(RwInt32 ref)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateStencilFunctionRef"));

    if (ref != _RwD3D8StateCache.stencilRef)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILREF, ref);

        _RwD3D8StateCache.stencilRef = ref;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateStencilFunctionMask

 On entry   : A RwInt32 value defining the mask applied to the reference value
              and each stencil buffer entry to determine the significant bits
              for the stencil test.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateStencilFunctionMask(RwInt32 mask)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateStencilFunctionMask"));

    if (mask != _RwD3D8StateCache.stencilMask)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILMASK, mask);

        _RwD3D8StateCache.stencilMask = mask;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D8RenderStateStencilFunctionWriteMask

 On entry   : A RwInt32 value defining the write mask applied to values
              written into the stencil buffer.
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RenderStateStencilFunctionWriteMask(RwInt32 writeMask)
{
    RWFUNCTION(RWSTRING("_rwD3D8RenderStateStencilFunctionWriteMask"));

    if (writeMask != _RwD3D8StateCache.stencilWriteMask)
    {
        FORCE_RENDERSTATE(D3DRS_STENCILWRITEMASK, writeMask);

        _RwD3D8StateCache.stencilWriteMask = writeMask;
    }

    RWRETURN(TRUE);
}


/****************************************************************************
 _rwD3D8RWSetRenderState

 On entry   : Render state to set
            : Parameter
 On exit    : TRUE on success else FALSE.
 */
RwBool
_rwD3D8RWSetRenderState(RwRenderState state, void *param)

{
    RwBool  result = FALSE;
    D3D8UnionParam uparam;

    RWFUNCTION(RWSTRING("_rwD3D8RWSetRenderState"));

    /*
     * Need to be in a camera update cliche for this to work
     */

    uparam.VoidPtr = param;

    switch (state)
    {
            /***************************** FOG ******************************/

        case rwRENDERSTATEFOGENABLE:
            result = rwD3D8RenderStateFogEnable(uparam.Bool);
            break;

        case rwRENDERSTATEFOGCOLOR:
            result = rwD3D8RenderStateFogColor(uparam.UInt32);
            break;

        case rwRENDERSTATEFOGTYPE:
            result = rwD3D8RenderStateFogType(uparam.FogType);
            break;

        case rwRENDERSTATEFOGDENSITY:
            result = rwD3D8RenderStateFogDensity(*uparam.RealPtr);
            break;

            /********************** TEXTURE ADDRESSING **********************/

        case rwRENDERSTATETEXTUREADDRESS:
            result = _rwD3D8RenderStateTextureAddress(uparam.TextureAddressMode);
            break;

        case rwRENDERSTATETEXTUREADDRESSU:
            result = _rwD3D8RenderStateTextureAddressU(uparam.TextureAddressMode);
            break;

        case rwRENDERSTATETEXTUREADDRESSV:
            result = _rwD3D8RenderStateTextureAddressV(uparam.TextureAddressMode);
            break;

            /********************* TEXTURE FILTER MODE **********************/

        case rwRENDERSTATETEXTUREFILTER:
            result = _rwD3D8RenderStateTextureFilter(uparam.TextureFilterMode);
            break;

            /************************ TEXTURE RASTER ************************/

        case rwRENDERSTATETEXTURERASTER:
            RWASSERT(uparam.RasterPtr == NULL || uparam.RasterPtr == RwRasterGetParent(uparam.RasterPtr));
            result = _rwD3D8RWSetRasterStage(uparam.RasterPtr, 0);
            break;

            /************************ Z BUFFER WRITE ************************/

        case rwRENDERSTATEZWRITEENABLE:
            result = rwD3D8RenderStateZWriteEnable(uparam.Bool);
            break;

            /************************ Z BUFFER TEST *************************/

        case rwRENDERSTATEZTESTENABLE:
            result = rwD3D8RenderStateZTestEnable(uparam.Bool);
            break;

            /************************ BLENDING MODES ************************/

        case rwRENDERSTATESRCBLEND:
            result = _rwD3D8RenderStateSrcBlend(uparam.BlendFunction);
            break;

        case rwRENDERSTATEDESTBLEND:
            result = _rwD3D8RenderStateDestBlend(uparam.BlendFunction);
            break;

            /************************* SHADING MODE *************************/

        case rwRENDERSTATESHADEMODE:
            result = rwD3D8RenderStateShadeMode(uparam.ShadeMode);
            break;

            /************************* VERTEX ALPHA *************************/

        case rwRENDERSTATEVERTEXALPHAENABLE:
            result = _rwD3D8RenderStateVertexAlphaEnable(uparam.Bool);
            break;

            /************************ BORDER COLOR **************************/

        case rwRENDERSTATEBORDERCOLOR:
            result = rwD3D8RenderStateBorderColor(uparam.UInt32);
            break;

            /*************** PERSPECTIVELY CORRECT TEXTUREING ***************/

        case rwRENDERSTATETEXTUREPERSPECTIVE:
            result = rwD3D8RenderStateTexturePerspective(uparam.Bool);
            break;

            /************************* CULLING MODE *************************/

        case rwRENDERSTATECULLMODE:
            result = rwD3D8RenderStateCullMode(uparam.CullMode);
            break;

            /************************** STENCIL BUFFER **************************/

        case rwRENDERSTATESTENCILENABLE:
            result = _rwD3D8RenderStateStencilEnable(uparam.Bool);
            break;

        case rwRENDERSTATESTENCILFAIL:
            result = _rwD3D8RenderStateStencilFail(uparam.StencilOperation);
            break;

        case rwRENDERSTATESTENCILZFAIL:
            result = _rwD3D8RenderStateStencilZFail(uparam.StencilOperation);
            break;

        case rwRENDERSTATESTENCILPASS:
            result = _rwD3D8RenderStateStencilPass(uparam.StencilOperation);
            break;

        case rwRENDERSTATESTENCILFUNCTION:
            result = _rwD3D8RenderStateStencilFunction(uparam.StencilFunction);
            break;

        case rwRENDERSTATESTENCILFUNCTIONREF:
            result = _rwD3D8RenderStateStencilFunctionRef(uparam.Int32);
            break;

        case rwRENDERSTATESTENCILFUNCTIONMASK:
            result = _rwD3D8RenderStateStencilFunctionMask(uparam.Int32);
            break;

        case rwRENDERSTATESTENCILFUNCTIONWRITEMASK:
            result = _rwD3D8RenderStateStencilFunctionWriteMask(uparam.Int32);
            break;

        case rwRENDERSTATEALPHATESTFUNCTION:
            if (uparam.AlphaTestFunction != _RwD3D8StateCache.alphaTestFunction)
            {
                _RwD3D8StateCache.alphaTestFunction = uparam.AlphaTestFunction;

                _RwD3D8StateCache.alphaTestEnable = (uparam.AlphaTestFunction != rwALPHATESTFUNCTIONALWAYS);

                if (RenderStateCache[D3DRS_ALPHABLENDENABLE].value)
                {
                    RwD3D8SetRenderState(D3DRS_ALPHATESTENABLE, _RwD3D8StateCache.alphaTestEnable);
                }

                FORCE_RENDERSTATE(D3DRS_ALPHAFUNC,
                    _RwD3D8CmpFuncConvTable[uparam.AlphaTestFunction]);
            }
            result = TRUE;
            break;

        case rwRENDERSTATEALPHATESTFUNCTIONREF:
            RwD3D8SetRenderState(D3DRS_ALPHAREF, uparam.UInt32);
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
