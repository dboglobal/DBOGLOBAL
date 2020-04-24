/*****************************************************************************
 *                                                                           *
 * module : d3d9vertexshader.c                                               *
 *                                                                           *
 * purpose: Compressed vertex support                                        *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 includes
 */

#include <d3d9.h>
#include <d3dx9.h>

#include <string.h>

#include <rwcore.h>
#include "rpdbgerr.h"

#include "d3d9vertexshader.h"
#include "d3d9vertexshaderdefs.h"
#include "d3d9vertexshaderutils.h"

#include "d3d9usage.h"

extern D3DCAPS9 _RwD3D9DeviceCaps;


/*****************************************************************************
 local defines
 */
#define MAX_TEXCOORDS   8

#define MAX_LIGHTS      15

#define MAX_TEXT_SIZE   4096

#define DUMP_SHADERx

#define VERSION1    10
#define VERSION2    20
#define VERSION2X   25

/*
#define VERSION3    30
*/

/*
#define IS_SHADERDESC_EQUAL(_desc1, _desc2) D3D9MemEqual((const RwUInt32 *)&(_desc1), (const RwUInt32 *)&(_desc2), sizeof(_desc1))
#define IS_SHADERDESC_LESS(_desc1, _desc2) D3D9MemLess((const RwUInt32 *)&(_desc1), (const RwUInt32 *)&(_desc2), sizeof(_desc1))
*/
#define IS_SHADERDESC_EQUAL(_desc1, _desc2) (*((const RwUInt32 *)(&(_desc1))) == (*((const RwUInt32 *)(&(_desc2)))))
#define IS_SHADERDESC_LESS(_desc1, _desc2) (*((const RwUInt32 *)(&(_desc1))) < (*((const RwUInt32 *)(&(_desc2)))))

#define TX_AUX(arg) #arg
#define TX(arg) TX_AUX(arg)


/* Vertex Shader */
#ifdef RWDEBUG
#define ADD_VERTEX_SHADER_VERSION1   text += rwsprintf(text, "vs_1_1\n"), NumInstructions = 0
#else
#define ADD_VERTEX_SHADER_VERSION1   text += rwsprintf(text, "vs_1_1\n")
#endif

#ifdef RWDEBUG
#define ADD_VERTEX_SHADER_VERSION2   text += rwsprintf(text, "vs_2_0\n"), NumInstructions = 0
#else
#define ADD_VERTEX_SHADER_VERSION2   text += rwsprintf(text, "vs_2_0\n")
#endif

#ifdef RWDEBUG
#define ADD_VERTEX_SHADER_VERSION2X   text += rwsprintf(text, "vs_2_x\n"), NumInstructions = 0
#else
#define ADD_VERTEX_SHADER_VERSION2X   text += rwsprintf(text, "vs_2_x\n")
#endif

#ifdef VERSION3
#ifdef RWDEBUG
#define ADD_VERTEX_SHADER_VERSION3   text += rwsprintf(text, "vs_3_0\n"), NumInstructions = 0
#else
#define ADD_VERTEX_SHADER_VERSION3   text += rwsprintf(text, "vs_3_0\n")
#endif
#endif

#define ADD_DECLARATION(_type, _register) text += rwsprintf(text, "dcl_" _type " "TX(_register)"\n");
#define ADD_DECLARATION_EXP(_expression, _param1, _param2) text += rwsprintf(text, "dcl_" _expression "\n", _param1, _param2);

#ifdef RWDEBUG
#define ADD_INS NumInstructions++, text += rwsprintf
#else
#define ADD_INS text += rwsprintf
#endif

#ifdef RWDEBUG
#define ADD_M4X4 NumInstructions+=4, text += rwsprintf
#else
#define ADD_M4X4 text += rwsprintf
#endif

#ifdef RWDEBUG
#define ADD_M3X4 NumInstructions+=4, text += rwsprintf
#else
#define ADD_M3X4 text += rwsprintf
#endif

#ifdef RWDEBUG
#define ADD_M3X3 NumInstructions+=3, text += rwsprintf
#else
#define ADD_M3X3 text += rwsprintf
#endif

#ifdef RWDEBUG
#define ADD_M4X3 NumInstructions+=3, text += rwsprintf
#else
#define ADD_M4X3 text += rwsprintf
#endif


/* C Interface */
#ifndef ID3DXBuffer_GetBufferPointer
#define ID3DXBuffer_GetBufferPointer(_buffer) (_buffer)->lpVtbl->GetBufferPointer(_buffer)
#else
#pragma message("ID3DXBuffer_GetBufferPointer is already defined.")
#endif

#ifndef ID3DXBuffer_GetBufferSize
#define ID3DXBuffer_GetBufferSize(_buffer) (_buffer)->lpVtbl->GetBufferSize(_buffer)
#else
#pragma message("ID3DXBuffer_GetBufferSize is already defined.")
#endif

#ifndef ID3DXBuffer_Release
#define ID3DXBuffer_Release(_buffer) (_buffer)->lpVtbl->Release(_buffer)
#else
#pragma message("ID3DXBuffer_Release is already defined.")
#endif

#define FLOATASINT(f) (*((const RwInt32 *)&(f)))
#define COLORSCALAR 0.003921568627450980392156862745098f    /* 1.0f/ 255.0f */

#define RWD3D9_ALIGN16 __declspec(align(16))

/*****************************************************************************
 local types
 */

typedef struct _rpD3D9VertexShaderItemDesc
{
    _rpD3D9VertexShaderDescriptor desc;
    RwUInt32 renderFrame;
    void *shader;
    _rpD3D9VertexShaderDispatchDescriptor dispatch;
} _rpD3D9VertexShaderItemDesc;

typedef struct _rpD3D9VertexShaderLights
{
    RwUInt32 numDirectional;
    RpLight *directional[MAX_LIGHTS];

    RwUInt32 numPoint;
    RpLight *point[MAX_LIGHTS];

    RwUInt32 numSpot;
    RpLight *spot[MAX_LIGHTS];

} _rpD3D9VertexShaderLights;

/*****************************************************************************
 local global variables
 */

#define D3D9_VERTEX_SHADER_CACHE_SIZE   256

static _rpD3D9VertexShaderItemDesc VertexShaderCache[D3D9_VERTEX_SHADER_CACHE_SIZE];
static RwInt32 NumShadersInCache = 0;

static _rpD3D9VertexShaderItemDesc *LastVertexShaderDesc = NULL;

static RwUInt32 D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_MAX];

static RwUInt32 D3D9UseVersion = 0;

#ifdef RWDEBUG
static RwUInt32 NumInstructions = 0;
#endif

RwUInt32 _rwD3D9VertexShaderMaxConstants = 0;

RWD3D9_ALIGN16 RwV4d _rxD3D9VertexShaderConstants[128];

/*****************************************************************************
 local global functions
 */

static __inline RwBool
D3D9MemEqual(const RwUInt32 *buff0,
             const RwUInt32 *buff1,
             RwUInt32 sizeBytes)
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

static __inline RwBool
D3D9MemLess(const RwUInt32 *buff0,
            const RwUInt32 *buff1,
            RwUInt32 sizeBytes)
{
    RWFUNCTION(RWSTRING("D3D9MemLess"));

    RWASSERT((sizeBytes % 4) == 0);

    sizeBytes /= 4;

    while (sizeBytes)
    {
        if (*buff0 >= *buff1)
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
 _rpD3D9GetNumConstantsUsed
 */
RwUInt32
_rpD3D9GetNumConstantsUsed(const _rpD3D9VertexShaderDescriptor *desc)
{
    RwUInt32 numConstants;

    RWFUNCTION(RWSTRING("_rpD3D9GetNumConstantsUsed"));

    numConstants = RWD3D9VSCONST_LIGHTS_OFFSET;

    if (desc->numDirectionalLights)
    {
        numConstants += VSCONST_REG_DIR_LIGHT_SIZE * (desc->numDirectionalLights);
    }

    if (desc->numPointLights)
    {
        numConstants += VSCONST_REG_PNT_LIGHT_SIZE * (desc->numPointLights);
        numConstants += ((desc->numPointLights + 3) / 4);
    }

    if (desc->numSpotLights)
    {
        numConstants += VSCONST_REG_SPOT_LIGHT_SIZE * (desc->numSpotLights);
        numConstants += ((desc->numSpotLights + 3) / 4);
    }

    if (desc->modulateMaterial)
    {
        numConstants += VSCONST_REG_MAT_COLOR_SIZE;
    }

    if (desc->fogMode > 0)
    {
        RWASSERT(desc->fogMode < rwD3D9VERTEXSHADERFOG_MAX);

        numConstants += 1;
    }

    if (desc->effect)
    {
        RWASSERT(desc->effect < rwD3D9VERTEXSHADEREFFECT_MAX);

        numConstants += D3D9NumConstantsEffect[desc->effect];
    }

    if (desc->morphing)
    {
        numConstants += 1;
    }

    RWRETURN(numConstants);
}

/****************************************************************************
 _rpD3D9GenerateVertexShader
 */
static void *
_rpD3D9GenerateVertexShader(const _rpD3D9VertexShaderDescriptor *desc,
                            _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    static const RwChar elements[] = {'x', 'y', 'z', 'w'};
    static const RwChar * const maskelements[] = {"x", "xy", "xyz", "xyzw"};
    static const RwChar * const replicateelements[] = {"xxx", "yyy", "zzz", "www"};
    static RwChar TextArray[MAX_TEXT_SIZE];

    RwChar *text;
    RwUInt32 boneStart, n;
    RwBool normals = FALSE;
    RwBool binormaltangents = FALSE;
    RwUInt32 effect;
    RwUInt32 numWeights;
    RwBool normalizeNormals;
    RwBool modulateMaterial;
    rwD3D9VertexShaderFogMode fogMode;

    const RwChar *inputPositions = NULL;
    const RwChar *inputNormals = NULL;

    LPD3DXBUFFER shaderTokens = NULL;
    HRESULT hr;
    void *shader = NULL;

    RwUInt32 pointOffset = 0xFFFFFFFF;
    RwUInt32 spotOffset = 0xFFFFFFFF;

    RWFUNCTION(RWSTRING("_rpD3D9GenerateVertexShader"));

    /* Unpack descriptor */
    effect = desc->effect;
    numWeights = desc->numWeights;
    normalizeNormals = desc->normalizeNormals;
    modulateMaterial = desc->modulateMaterial;
    fogMode = desc->fogMode;

    /* check normals */
    if ( desc->numDirectionalLights  ||
         desc->numPointLights ||
         desc->numSpotLights  ||
         effect == rwD3D9VERTEXSHADEREFFECT_BUMPMAP ||
         effect == rwD3D9VERTEXSHADEREFFECT_ENVMAP ||
         effect == rwD3D9VERTEXSHADEREFFECT_BUMPENVMAP ||
         effect == rwD3D9VERTEXSHADEREFFECT_NORMALMAP ||
         effect == rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP ||
         effect == rwD3D9VERTEXSHADEREFFECT_CUBEMAP ||
         effect == rwD3D9VERTEXSHADEREFFECT_BUMPCUBEMAP)
    {
        if (desc->normals)
        {
            normals = TRUE;

            if (effect == rwD3D9VERTEXSHADEREFFECT_NORMALMAP ||
                effect == rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP)
            {
                RWASSERT(desc->numTexCoords > 0);
                RWASSERT((desc->numDirectionalLights + desc->numPointLights + desc->numSpotLights) <= 1);

                binormaltangents = TRUE;
            }
        }
    }

    /* Fill dispatch info */
    memset(dispatch, 0xFF, sizeof(_rpD3D9VertexShaderDispatchDescriptor));

    boneStart = RWD3D9VSCONST_LIGHTS_OFFSET;

    if (desc->numDirectionalLights)
    {
        boneStart += VSCONST_REG_DIR_LIGHT_SIZE * (desc->numDirectionalLights);
    }

    if (desc->numPointLights)
    {
        pointOffset = boneStart;

        boneStart += VSCONST_REG_PNT_LIGHT_SIZE * (desc->numPointLights);
        boneStart += ((desc->numPointLights + 3) / 4);
    }

    if (desc->numSpotLights)
    {
        spotOffset = boneStart;

        boneStart += VSCONST_REG_SPOT_LIGHT_SIZE * (desc->numSpotLights);
        boneStart += ((desc->numSpotLights + 3) / 4);
    }

    if (modulateMaterial)
    {
        dispatch->offsetMaterialColor = boneStart;

        boneStart += VSCONST_REG_MAT_COLOR_SIZE;
    }

    if (fogMode > 0)
    {
        RWASSERT(fogMode < rwD3D9VERTEXSHADERFOG_MAX);

        if ((_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG) == 0)
        {
            dispatch->offsetFogRange = boneStart;

            boneStart += 1;
        }
    }

    if (desc->morphing)
    {
        dispatch->offsetMorphingCoef = boneStart;

        boneStart += 1;
    }

    if (numWeights > 0)
    {
        RWASSERT(numWeights <= 4);

        dispatch->offsetBoneMatrices = boneStart;
    }

    RWASSERT(desc->numTexCoords <= MAX_TEXCOORDS);

    if (effect)
    {
        RwUInt32 numConstantsEffect;

        RWASSERT(effect < rwD3D9VERTEXSHADEREFFECT_MAX);

        numConstantsEffect = D3D9NumConstantsEffect[effect];

        if (numConstantsEffect)
        {
            if (numWeights > 0)
            {
                dispatch->offsetEffect = _rwD3D9VertexShaderMaxConstants - numConstantsEffect;
            }
            else
            {
                dispatch->offsetEffect = boneStart;

                boneStart += numConstantsEffect;
            }
        }

        #if RWDEBUG
        /* Check some effects conditions */
        switch (effect)
        {
            case rwD3D9VERTEXSHADEREFFECT_BUMPMAP:
                RWASSERT(desc->numTexCoords > 0);
                break;

            case rwD3D9VERTEXSHADEREFFECT_BUMPENVMAP:
                RWASSERT(desc->numTexCoords > 0);
                break;

            case rwD3D9VERTEXSHADEREFFECT_DUAL:
                RWASSERT(desc->numTexCoords > 1);
                break;

            case rwD3D9VERTEXSHADEREFFECT_DUALREPLICATE:
                RWASSERT(desc->numTexCoords > 0);
                break;

            case rwD3D9VERTEXSHADEREFFECT_UVANIM:
                RWASSERT(desc->numTexCoords > 0);
                break;

            case rwD3D9VERTEXSHADEREFFECT_DUALUVANIM:
                RWASSERT(desc->numTexCoords > 1);
                break;

            case rwD3D9VERTEXSHADEREFFECT_NORMALMAP:
                RWASSERT(desc->numTexCoords > 0);
                break;

            case rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP:
                RWASSERT(desc->numTexCoords > 0);
                break;

            case rwD3D9VERTEXSHADEREFFECT_BUMPCUBEMAP:
                RWASSERT(desc->numTexCoords > 0);
                break;

            default:
                break;
        }
        #endif
    }

    /*
     * Shader header
     */
    text = TextArray;

    switch (D3D9UseVersion)
    {
        case VERSION1:
            ADD_VERTEX_SHADER_VERSION1;
            break;

        case VERSION2:
            ADD_VERTEX_SHADER_VERSION2;
            break;

        case VERSION2X:
            ADD_VERTEX_SHADER_VERSION2X;
            break;

#ifdef VERSION3
        case VERSION3:
            ADD_VERTEX_SHADER_VERSION3;
            break;
#endif

        default:
            RWRETURN(NULL);
            break;
    }

    /* Declarations */
    ADD_DECLARATION("position0", VSIN_REG_POS);

    if (numWeights > 0)
    {
        if (numWeights > 1)
        {
            ADD_DECLARATION("blendweight0", VSIN_REG_WEIGHTS);
        }

        ADD_DECLARATION("blendindices0", VSIN_REG_INDICES);
    }

    if (desc->prelit)
    {
        ADD_DECLARATION("color0", VSIN_REG_COLOR);
    }

    if (normals)
    {
        ADD_DECLARATION("normal0", VSIN_REG_NORMAL);
    }

    if (binormaltangents)
    {
        ADD_DECLARATION("tangent0", VSIN_REG_TANGENT);
    }

    switch (effect)
    {
        case rwD3D9VERTEXSHADEREFFECT_ENVMAP:
        case rwD3D9VERTEXSHADEREFFECT_BUMPENVMAP:
        case rwD3D9VERTEXSHADEREFFECT_BUMPMAP:
        case rwD3D9VERTEXSHADEREFFECT_UVANIM:
        case rwD3D9VERTEXSHADEREFFECT_CUBEMAP:
        case rwD3D9VERTEXSHADEREFFECT_BUMPCUBEMAP:
            if (desc->numTexCoords > 0)
            {
                ADD_DECLARATION("texcoord0", VSIN_REG_TEXCOORD0);
            }
            break;

        case rwD3D9VERTEXSHADEREFFECT_DUAL:
            if (desc->numTexCoords > 1)
            {
                ADD_DECLARATION("texcoord1", VSIN_REG_TEXCOORD1);
            }
            break;

        case rwD3D9VERTEXSHADEREFFECT_DUALREPLICATE:
            if (desc->numTexCoords > 0)
            {
                ADD_DECLARATION("texcoord0", VSIN_REG_TEXCOORD0);
            }
            break;

        case rwD3D9VERTEXSHADEREFFECT_DUALUVANIM:
            if (desc->numTexCoords > 1)
            {
                ADD_DECLARATION("texcoord1", VSIN_REG_TEXCOORD1);
            }
            break;

        default:
            for (n = 0; n < desc->numTexCoords; n++)
            {
                ADD_DECLARATION_EXP("texcoord%u v%u", n, n + VSD_REG_TEXCOORD0);
            }
            break;
    }

    if (desc->morphing)
    {
        ADD_DECLARATION("position1", VSIN_REG_POS2);

        if (normals)
        {
            ADD_DECLARATION("normal1", VSIN_REG_NORMAL2);
        }
    }

    /* Input position transform */
    inputPositions = TX(VSIN_REG_POS);

    if (normals)
    {
        inputNormals = TX(VSIN_REG_NORMAL);
    }

    if (desc->morphing == 0 &&
        numWeights == 0)
    {
        ADD_M4X4(text, "m4x4 oPos.xyzw, "TX(VSIN_REG_POS)", c[0]\n");
    }
    else
    {
        if (desc->morphing)
        {
            const RwUInt32 offsetMorphingCoef = (RwUInt32)dispatch->offsetMorphingCoef;

            if (numWeights == 0)
            {
                ADD_INS(text, "mul "TX(VSTMP_REG_POS_ACCUM)", "TX(VSIN_REG_POS)", c[%d].x\n", offsetMorphingCoef);
                ADD_INS(text, "mad "TX(VSTMP_REG_POS_ACCUM)", "TX(VSIN_REG_POS2)", c[%d].y, "TX(VSTMP_REG_POS_ACCUM)"\n", offsetMorphingCoef);

                inputPositions = TX(VSTMP_REG_POS_ACCUM);

                if (normals)
                {
                    ADD_INS(text, "mul "TX(VSTMP_REG_NORMAL_ACCUM)", "TX(VSIN_REG_NORMAL)", c[%d].x\n", offsetMorphingCoef);
                    ADD_INS(text, "mad "TX(VSTMP_REG_NORMAL_ACCUM)", "TX(VSIN_REG_NORMAL2)", c[%d].y, "TX(VSTMP_REG_NORMAL_ACCUM)"\n", offsetMorphingCoef);

                    inputNormals = TX(VSTMP_REG_NORMAL_ACCUM);
                }
            }
            else
            {
                ADD_INS(text, "mul "TX(VSTMP_REG_POS_TMP)", "TX(VSIN_REG_POS)", c[%d].x\n", offsetMorphingCoef);
                ADD_INS(text, "mad "TX(VSTMP_REG_POS_TMP)", "TX(VSIN_REG_POS2)", c[%d].y, "TX(VSTMP_REG_POS_TMP)"\n", offsetMorphingCoef);

                inputPositions = TX(VSTMP_REG_POS_TMP);

                if (normals)
                {
                    ADD_INS(text, "mul "TX(VSTMP_REG_NORMAL_TMP)", "TX(VSIN_REG_NORMAL)", c[%d].x\n", offsetMorphingCoef);
                    ADD_INS(text, "mad "TX(VSTMP_REG_NORMAL_TMP)", "TX(VSIN_REG_NORMAL2)", c[%d].y, "TX(VSTMP_REG_NORMAL_TMP)"\n", offsetMorphingCoef);

                    inputNormals = TX(VSTMP_REG_NORMAL_TMP);
                }
            }
        }

        if (numWeights > 0)
        {
            if (numWeights == 1)
            {
                if (D3D9UseVersion == VERSION1)
                {
                    ADD_INS(text, "mov a0.x, "TX(VSIN_REG_INDICES)".x\n");
                }
                else
                {
                    ADD_INS(text, "mova a0.x, "TX(VSIN_REG_INDICES)".x\n");
                }

                /* Transform position */
                ADD_M4X3(text, "m4x3 "TX(VSTMP_REG_POS_ACCUM)".xyz, %s, c[a0.x + %d]\n", inputPositions, boneStart + 0);

                if (normals)
                {
                    /* Transform normal */
                    ADD_M3X3(text, "m3x3 "TX(VSTMP_REG_NORMAL_ACCUM)".xyz, %s, c[a0.x + %d]\n", inputNormals, boneStart + 0);
                }
            }
            else
            {
                RwUInt32 wi;

                if (normals)
                {
                    /* I need to reuse some temp registers... */
                    #define VSTMP_REG_TRANS_0 VSTMP_REG_ATTEN_TMP
                    #define VSTMP_REG_TRANS_1 VSTMP_REG_BUMPDIR_TMP
                    #define VSTMP_REG_TRANS_2 VSTMP_REG_TEX

                    if (D3D9UseVersion >= VERSION2)
                    {
                        ADD_INS(text, "mova a0.%s, "TX(VSIN_REG_INDICES)"\n", maskelements[numWeights - 1]);

                        for (wi = 0; wi < numWeights; wi++)
                        {
                            /* Transform position */
                            if (wi)
                            {
                                ADD_INS(text, "mad "TX(VSTMP_REG_TRANS_0)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.%c + %d], "TX(VSTMP_REG_TRANS_0)"\n", elements[wi], elements[wi], boneStart + 0);
                                ADD_INS(text, "mad "TX(VSTMP_REG_TRANS_1)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.%c + %d], "TX(VSTMP_REG_TRANS_1)"\n", elements[wi], elements[wi], boneStart + 1);
                                ADD_INS(text, "mad "TX(VSTMP_REG_TRANS_2)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.%c + %d], "TX(VSTMP_REG_TRANS_2)"\n", elements[wi], elements[wi], boneStart + 2);
                            }
                            else
                            {
                                ADD_INS(text, "mul "TX(VSTMP_REG_TRANS_0)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.%c + %d]\n", elements[wi], elements[wi], boneStart + 0);
                                ADD_INS(text, "mul "TX(VSTMP_REG_TRANS_1)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.%c + %d]\n", elements[wi], elements[wi], boneStart + 1);
                                ADD_INS(text, "mul "TX(VSTMP_REG_TRANS_2)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.%c + %d]\n", elements[wi], elements[wi], boneStart + 2);
                            }
                        }
                    }
                    else
                    {
                        for (wi = 0; wi < numWeights; wi++)
                        {
                            ADD_INS(text, "mov a0.x, "TX(VSIN_REG_INDICES)".%c\n", elements[wi]);

                            /* Transform position */
                            if (wi)
                            {
                                ADD_INS(text, "mad "TX(VSTMP_REG_TRANS_0)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.x + %d], "TX(VSTMP_REG_TRANS_0)"\n", elements[wi], boneStart + 0);
                                ADD_INS(text, "mad "TX(VSTMP_REG_TRANS_1)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.x + %d], "TX(VSTMP_REG_TRANS_1)"\n", elements[wi], boneStart + 1);
                                ADD_INS(text, "mad "TX(VSTMP_REG_TRANS_2)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.x + %d], "TX(VSTMP_REG_TRANS_2)"\n", elements[wi], boneStart + 2);
                            }
                            else
                            {
                                ADD_INS(text, "mul "TX(VSTMP_REG_TRANS_0)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.x + %d]\n", elements[wi], boneStart + 0);
                                ADD_INS(text, "mul "TX(VSTMP_REG_TRANS_1)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.x + %d]\n", elements[wi], boneStart + 1);
                                ADD_INS(text, "mul "TX(VSTMP_REG_TRANS_2)", "TX(VSIN_REG_WEIGHTS)".%c, c[a0.x + %d]\n", elements[wi], boneStart + 2);
                            }
                        }
                    }

                    /* Transform position */
                    ADD_M4X3(text, "m4x3 "TX(VSTMP_REG_POS_ACCUM)".xyz, %s, "TX(VSTMP_REG_TRANS_0)"\n", inputPositions);

                    /* Transform normal */
                    ADD_M3X3(text, "m3x3 "TX(VSTMP_REG_NORMAL_ACCUM)".xyz, %s, "TX(VSTMP_REG_TRANS_0)"\n", inputNormals);
                }
                else
                {
                    /* I need to reuse some temp registers... */
                    #define VSTMP_REG_POS_AUX VSTMP_REG_NORMAL_ACCUM

                    if (D3D9UseVersion >= VERSION2)
                    {
                        ADD_INS(text, "mova a0.%s, "TX(VSIN_REG_INDICES)"\n", maskelements[numWeights - 1]);

                        for (wi = 0; wi < numWeights; wi++)
                        {
                            /* Transform position */
                            ADD_M4X3(text, "m4x3 "TX(VSTMP_REG_POS_AUX)".xyz, %s, c[a0.%c + %d]\n", inputPositions, elements[wi], boneStart + 0);

                            /* Scale transformed point by weight */

                            if (wi == 0)
                            {
                                /* multiply */
                                ADD_INS(text, "mul "TX(VSTMP_REG_POS_ACCUM)".xyz, "TX(VSIN_REG_WEIGHTS)".%c, "TX(VSTMP_REG_POS_AUX)".xyz\n", elements[wi]);
                            }
                            else
                            {
                                /* multiply add */
                                ADD_INS(text, "mad "TX(VSTMP_REG_POS_ACCUM)".xyz, "TX(VSIN_REG_WEIGHTS)".%c, "TX(VSTMP_REG_POS_AUX)".xyz, "TX(VSTMP_REG_POS_ACCUM)".xyz\n", elements[wi]);
                            }
                        }
                    }
                    else
                    {
                        for (wi = 0; wi < numWeights; wi++)
                        {
                            ADD_INS(text, "mov a0.x, "TX(VSIN_REG_INDICES)".%c\n", elements[wi]);

                            /* Transform position */
                            ADD_M4X3(text, "m4x3 "TX(VSTMP_REG_POS_AUX)".xyz, %s, c[a0.x + %d]\n", inputPositions, boneStart + 0);

                            /* Scale transformed point by weight */

                            if (wi == 0)
                            {
                                /* multiply */
                                ADD_INS(text, "mul "TX(VSTMP_REG_POS_ACCUM)".xyz, "TX(VSIN_REG_WEIGHTS)".%c, "TX(VSTMP_REG_POS_AUX)".xyz\n", elements[wi]);
                            }
                            else
                            {
                                /* multiply add */
                                ADD_INS(text, "mad "TX(VSTMP_REG_POS_ACCUM)".xyz, "TX(VSIN_REG_WEIGHTS)".%c, "TX(VSTMP_REG_POS_AUX)".xyz, "TX(VSTMP_REG_POS_ACCUM)".xyz\n", elements[wi]);
                            }
                        }
                    }
                }
            }

            inputPositions = TX(VSTMP_REG_POS_ACCUM);

            if (normals)
            {
                inputNormals = TX(VSTMP_REG_NORMAL_ACCUM);
            }
        }

        ADD_INS(text, "mov "TX(VSTMP_REG_POS_ACCUM)".w, "TX(VSIN_REG_POS)".w\n");
        ADD_M4X4(text, "m4x4 oPos.xyzw, "TX(VSTMP_REG_POS_ACCUM)", c[0]\n");
    }

    /* FOG */
    if (fogMode > 0)
    {
        if (_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG)
        {
            /* Compute distance to camera */
            ADD_INS(text, "dp4 oFog, %s, c[3]\n", inputPositions);
        }
        else
        {
            const RwUInt32 offsetFogRange = dispatch->offsetFogRange;

            /* Compute distance to camera */
            ADD_INS(text, "dp4 r0.w, %s, c[3]\n", inputPositions);

            switch (fogMode)
            {
                case rwD3D9VERTEXSHADERFOG_LINEAR:
                    /* Fog Constant: x = -1/(fog_end-fog_start), y = fog_end/(fog_end-fog_start) */
                    /* Compute fog factor f = (fog_end - dist)*(1/(fog_end-fog_start)) */
                    ADD_INS(text, "mad oFog, r0.w, c[%d].x, c[%d].y\n", offsetFogRange, offsetFogRange);
                    break;

                case rwD3D9VERTEXSHADERFOG_EXP:
                    /* Fog Constant: x = density * log2(e) */
                    /* Compute d * density * log2(e) */
                    ADD_INS(text, "mul r0.w, r0.w, c[%d].x\n", offsetFogRange);
                    /* Compute fog factor f = 1 / (e^(d * density)) */
                    ADD_INS(text, "expp r0.w, r0.w\n");
                    ADD_INS(text, "rcp oFog, r0.w\n");
                    break;

                case rwD3D9VERTEXSHADERFOG_EXP2:
                    /* Fog Constant: x = density * log2(e) */
                    /* Compute d * density * log2(e) */
                    ADD_INS(text, "mul r0.w, r0.w, c[%d].x\n", offsetFogRange);
                    /* Compute (d * density * log2(e)) ^ 2 */
                    ADD_INS(text, "mul r0.w, r0.w, r0.w\n");
                    /* Compute fog factor f = 1 / (e^(d * density)) */
                    ADD_INS(text, "expp r0.w, r0.w\n");
                    ADD_INS(text, "rcp oFog, r0.w\n");
                    break;

                default:
                    break;
            }
        }
    }

    /* Normalize normal */
    if (normals &&
        normalizeNormals)
    {
        ADD_INS(text, "dp3 "TX(VSTMP_REG_NORMAL_ACCUM)".w, %s, %s\n", inputNormals, inputNormals);
        ADD_INS(text, "rsq "TX(VSTMP_REG_NORMAL_ACCUM)".w, "TX(VSTMP_REG_NORMAL_ACCUM)".w\n");
        ADD_INS(text, "mul "TX(VSTMP_REG_NORMAL_ACCUM)".xyz, %s, "TX(VSTMP_REG_NORMAL_ACCUM)".w\n", inputNormals);

        inputNormals = TX(VSTMP_REG_NORMAL_ACCUM);
    }

    /*
     * LIGHTING
     */
    if (binormaltangents == FALSE)
    {
        RwUInt32 numLights = (desc->numDirectionalLights +
                              desc->numPointLights +
                              desc->numSpotLights);

        /* STANDARD LIGHTING */
        if (normals &&
            numLights > 0)
        {
            /* NOTE: the 'W' component of the ambient color MUST be ZERO if prelit and ONE if not prelit */

            /* Ambient plus prelit */
            if (desc->prelit)
            {
                ADD_INS(text, "add "TX(VSTMP_REG_COLOR_TMP)", "TX(VSIN_REG_COLOR)", c["TX(RWD3D9VSCONST_AMBIENT_OFFSET)"]\n");
            }
            else
            {
                ADD_INS(text, "mov "TX(VSTMP_REG_COLOR_TMP)", c["TX(RWD3D9VSCONST_AMBIENT_OFFSET)"]\n");
            }

            /* NOTE: the 'W' component of the light dir and light color constants MUST be ZERO */

            /* DIRECT */
            if (desc->numDirectionalLights)
            {
                const RwUInt32 numDirectionalLights = desc->numDirectionalLights;
                RwUInt32 dir;

                /* do lights in blocks of 4 so we can get the most of the max instruction */
                for (dir = 0; dir < numDirectionalLights; dir += 4)
                {
                    /* if there are less than 4 to go, just pack in as many as we have left */
                    RwUInt32 constantDirOffset;
                    RwUInt32 maxblock, m;
                    const RwChar *mask;

                    maxblock = 4;
                    if (numDirectionalLights - dir < maxblock)
                    {
                        maxblock = numDirectionalLights - dir;
                    }

                    for (m = 0; m < maxblock; m++)
                    {
                        constantDirOffset = (dir + m) * 2 + RWD3D9VSCONST_LIGHTS_OFFSET;

                        /* diffuse intensity of directional light max(N dot L, 0) */
                        ADD_INS(text, "dp3 "TX(VSTMP_REG_CLAMP_TMP)".%c, c[%d], %s\n", elements[m], constantDirOffset, inputNormals);
                    }

                    mask = maskelements[maxblock - 1];

                    ADD_INS(text, "max "TX(VSTMP_REG_CLAMP_TMP)".%s, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w, -"TX(VSTMP_REG_CLAMP_TMP)".%s\n", mask, mask);

                    for (m = 0; m < maxblock; m++)
                    {
                        constantDirOffset = (dir + m) * 2 + 1 + RWD3D9VSCONST_LIGHTS_OFFSET;

                        if (numLights > 1 ||
                            modulateMaterial)
                        {
                            ADD_INS(text, "mad "TX(VSTMP_REG_COLOR_TMP)".xyz, c[%d].xyz, "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_COLOR_TMP)".xyz\n", constantDirOffset, replicateelements[m]);
                        }
                        else
                        {
                            ADD_INS(text, "mad oD0, c[%d], "TX(VSTMP_REG_CLAMP_TMP)".%c, "TX(VSTMP_REG_COLOR_TMP)"\n", constantDirOffset, elements[m]);
                        }

                        numLights--;
                    }
                }
            }

            /* POINTS */
            if (desc->numPointLights)
            {
                const RwUInt32 numPointLights = desc->numPointLights;
                RwUInt32 pnt;

                /* do point lights in blocks of 4 to vectorize intensity calculations where possible */
                for (pnt = 0; pnt < numPointLights; pnt += 4)
                {
                    RwUInt32 maxblock, m;
                    const RwChar *mask;

                    /* if there are less than 4 to go, just pack in as many as we have left */
                    maxblock = 4;
                    if (numPointLights - pnt < maxblock)
                    {
                        maxblock = numPointLights - pnt;
                    }

                    for (m = 0; m < maxblock; m++)
                    {
                        RwUInt32 constantDirOffset;

                        constantDirOffset = pointOffset /* start of block of 4 light constants */
                                            + 1         /* skip radii */
                                            + m * 2;    /* 2 constants per light thereafter */

                        /* vector from vertex to light (pnt + m) */
                        ADD_INS(text, "add "TX(VSTMP_REG_DIST_TMP)".xyz, -c[%d].xyz, %s.xyz\n", constantDirOffset, inputPositions);

                        /* 1/distance to light (pnt + m) */
                        ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".%c, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)"\n", elements[m]);
                        ADD_INS(text, "rsq "TX(VSTMP_REG_ILU_TMP)".%c, "TX(VSTMP_REG_ILU_TMP)".%c\n", elements[m], elements[m]);

                        /* diffuse intensity of light (pnt + m) max(N dot L, 0) */
                        ADD_INS(text, "dp3 "TX(VSTMP_REG_CLAMP_TMP)".%c, %s, "TX(VSTMP_REG_DIST_TMP)"\n", elements[m], inputNormals);
                    }

                    mask = maskelements[maxblock - 1];

                    /* clamp 1/distance to 1/light radius */
                    ADD_INS(text, "max "TX(VSTMP_REG_ATTEN_TMP)".%s, c[%d].%s, "TX(VSTMP_REG_ILU_TMP)".%s\n", mask, pointOffset, mask, mask);

                    /* clamp diffuse intensity */
                    ADD_INS(text, "max "TX(VSTMP_REG_CLAMP_TMP)".%s, c[%d].wwww, -"TX(VSTMP_REG_CLAMP_TMP)".%s\n", mask, pointOffset + 1, mask);

                    /* atten = 1/distance - 1/r */
                    ADD_INS(text, "add "TX(VSTMP_REG_ATTEN_TMP)".%s, -c[%d].%s, "TX(VSTMP_REG_ATTEN_TMP)".%s\n", mask, pointOffset, mask, mask);

                    /* intensity *= attenuation */
                    ADD_INS(text, "mul "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_ATTEN_TMP)".%s\n", mask, mask, mask);

                    for (m = 0; m < maxblock; m++)
                    {
                        RwUInt32 constantDirOffset;

                        constantDirOffset = pointOffset /* start of block of 4 light constants */
                                            + 1         /* skip radii */
                                            + 1         /* skip pos */
                                            + m * 2;    /* 2 constants per light thereafter */

                        /* multiply color by final intensity of light (pnt + m) and accumulate */
                        if (numLights > 1 ||
                            modulateMaterial)
                        {
                            ADD_INS(text, "mad "TX(VSTMP_REG_COLOR_TMP)".xyz, c[%d].xyz, "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_COLOR_TMP)".xyz\n", constantDirOffset, replicateelements[m]);
                        }
                        else
                        {
                            ADD_INS(text, "mad oD0, c[%d], "TX(VSTMP_REG_CLAMP_TMP)".%c, "TX(VSTMP_REG_COLOR_TMP)"\n", constantDirOffset, elements[m]);
                        }

                        numLights--;
                    }

                    pointOffset += 2 * maxblock + 1; /* 2 more constants for each point light we did, one constant for 4 radii */
                }
            }

            /* SPOT */
            if (desc->numSpotLights)
            {
                /* I need to reuse some temp registers... */
                #define VSTMP_REG_ANGLE_ATTEN_TMP   VSTMP_REG_BUMPDIR_TMP

                const RwUInt32 numSpotLights = desc->numSpotLights;
                RwUInt32 pnt;

                /* do point lights in blocks of 4 to vectorize intensity calculations where possible */
                for (pnt = 0; pnt < numSpotLights; pnt += 4)
                {
                    RwUInt32 maxblock, m;
                    const RwChar *mask;

                    /* if there are less than 4 to go, just pack in as many as we have left */
                    maxblock = 4;
                    if (numSpotLights - pnt < maxblock)
                    {
                        maxblock = numSpotLights - pnt;
                    }

                    /* Attenuations */
                    for (m = 0; m < maxblock; m++)
                    {
                        RwUInt32 constantDirOffset;

                        constantDirOffset = spotOffset  /* start of block of 4 light constants */
                                            + 1         /* skip radii */
                                            + m * 4;    /* 4 constants per light thereafter */

                        /*
                         * Calculate distance attenuation
                         */

                        /* vector from light to vertex (pnt + m) */
                        ADD_INS(text, "add "TX(VSTMP_REG_DIST_TMP)".xyz, -c[%d].xyz, %s.xyz\n", constantDirOffset, inputPositions);

                        /* 1/distance to light (pnt + m) */
                        ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".%c, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)"\n", elements[m]);
                        ADD_INS(text, "rsq "TX(VSTMP_REG_ILU_TMP)".%c, "TX(VSTMP_REG_ILU_TMP)".%c\n", elements[m], elements[m]);

                        /* diffuse intensity of light (pnt + m) max(N dot L, 0) */
                        ADD_INS(text, "dp3 "TX(VSTMP_REG_CLAMP_TMP)".%c, %s, "TX(VSTMP_REG_DIST_TMP)"\n", elements[m], inputNormals);

                        /*
                         * Calculate angle attenuation
                         */

                        /* Normalize direction to light */
                        ADD_INS(text, "mul "TX(VSTMP_REG_DIST_TMP)".xyz, "TX(VSTMP_REG_DIST_TMP)".xyz, "TX(VSTMP_REG_ILU_TMP)".%c\n", elements[m]);

                        /* (Direction from light to vector) dot (light direction) */
                        ADD_INS(text, "dp3 "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 1 + constantDirOffset);

                        /* x = 1.f / (1.f - cosangle), y = -cosangle / (1.f - cosangle) */
                        ADD_INS(text, "mad "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".%c, "TX(VSTMP_REG_DIST_TMP)".w, c[%d].x, c[%d].y\n", elements[m], 2 + constantDirOffset, 2 + constantDirOffset);
                    }

                    mask = maskelements[maxblock - 1];

                    /* clamp 1/distance to 1/light radius */
                    ADD_INS(text, "max "TX(VSTMP_REG_ATTEN_TMP)".%s, c[%d].%s, "TX(VSTMP_REG_ILU_TMP)".%s\n", mask, spotOffset, mask, mask);

                    /* clamp angle attenuation */
                    ADD_INS(text, "max "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".%s, c[%d].wwww, "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".%s\n", mask, spotOffset + 1, mask);

                    /* clamp diffuse intensity */
                    ADD_INS(text, "max "TX(VSTMP_REG_CLAMP_TMP)".%s, c[%d].wwww, -"TX(VSTMP_REG_CLAMP_TMP)".%s\n", mask, spotOffset + 1, mask);

                    /* distance attenuation = 1/distance - 1/r */
                    ADD_INS(text, "add "TX(VSTMP_REG_ATTEN_TMP)".%s, -c[%d].%s, "TX(VSTMP_REG_ATTEN_TMP)".%s\n", mask, spotOffset, mask, mask);

                    /* intensity *= distance attenuation */
                    ADD_INS(text, "mul "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_ATTEN_TMP)".%s\n", mask, mask, mask);

                    /* intensity *= angle attenuation */
                    ADD_INS(text, "mul "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".%s\n", mask, mask, mask);

                    for (m = 0; m < maxblock; m++)
                    {
                        RwUInt32 constantDirOffset;

                        constantDirOffset = spotOffset  /* start of block of 4 light constants */
                                            + 1         /* skip radii */
                                            + 1         /* skip pos */
                                            + 1         /* skip dir */
                                            + 1         /* skip angle range */
                                            + m * 4;    /* 4 constants per light thereafter */

                        /* multiply color by final intensity of light (pnt + m) and accumulate */
                        if (numLights > 1 ||
                            modulateMaterial)
                        {
                            ADD_INS(text, "mad "TX(VSTMP_REG_COLOR_TMP)".xyz, c[%d].xyz, "TX(VSTMP_REG_CLAMP_TMP)".%s, "TX(VSTMP_REG_COLOR_TMP)".xyz\n", constantDirOffset, replicateelements[m]);
                        }
                        else
                        {
                            ADD_INS(text, "mad oD0, c[%d], "TX(VSTMP_REG_CLAMP_TMP)".%c, "TX(VSTMP_REG_COLOR_TMP)"\n", constantDirOffset, elements[m]);
                        }

                        numLights--;
                    }

                    spotOffset += 4 * maxblock + 1; /* 3 more constants for each point light we did, one constant for 4 radii */
                }
            }

            RWASSERT(numLights == 0);

            if (modulateMaterial)
            {
                /* Modulate material */
                ADD_INS(text, "mul oD0, "TX(VSTMP_REG_COLOR_TMP)", c[%d]\n", (RwUInt32)(dispatch->offsetMaterialColor));
            }
        }
        else
        {
            if (modulateMaterial)
            {
                /* Ambient plus prelit */
                if (desc->prelit)
                {
                    ADD_INS(text, "add "TX(VSTMP_REG_COLOR_TMP)", "TX(VSIN_REG_COLOR)", c["TX(RWD3D9VSCONST_AMBIENT_OFFSET)"]\n");
                }
                else
                {
                    ADD_INS(text, "mov "TX(VSTMP_REG_COLOR_TMP)", c["TX(RWD3D9VSCONST_AMBIENT_OFFSET)"]\n");
                }

                /* Modulate material */
                ADD_INS(text, "mul oD0, "TX(VSTMP_REG_COLOR_TMP)", c[%d]\n", (RwUInt32)(dispatch->offsetMaterialColor));
            }
            else
            {
                /* Ambient plus prelit */
                if (desc->prelit)
                {
                    ADD_INS(text, "add oD0, "TX(VSIN_REG_COLOR)", c["TX(RWD3D9VSCONST_AMBIENT_OFFSET)"]\n");
                }
                else
                {
                    ADD_INS(text, "mov oD0, c["TX(RWD3D9VSCONST_AMBIENT_OFFSET)"]\n");
                }
            }
        }
    }
    else
    {
        /* I need to reuse some temp registers... */
        #define VSTMP_REG_BINORMAL_ACCUM VSTMP_REG_POS_TMP
        #define VSTMP_REG_TANGENT_ACCUM VSTMP_REG_NORMAL_TMP

        /* NORMAL MAP LIGHTING */
        if (numWeights > 0)
        {
            if (numWeights == 1)
            {
                /* Transform tangent */
                ADD_M3X3(text, "m3x3 "TX(VSTMP_REG_TANGENT_ACCUM)".xyz, "TX(VSIN_REG_TANGENT)", c[a0.x + %d]\n", boneStart + 0);
            }
            else
            {
                /* Transform tangent */
                ADD_M3X3(text, "m3x3 "TX(VSTMP_REG_TANGENT_ACCUM)".xyz, "TX(VSIN_REG_TANGENT)", "TX(VSTMP_REG_TRANS_0)"\n");
            }

            /* normalize tangent */
            if (normalizeNormals)
            {
                ADD_INS(text, "dp3 "TX(VSTMP_REG_TANGENT_ACCUM)".w, "TX(VSTMP_REG_TANGENT_ACCUM)", "TX(VSTMP_REG_TANGENT_ACCUM)"\n");
                ADD_INS(text, "rsq "TX(VSTMP_REG_TANGENT_ACCUM)".w, "TX(VSTMP_REG_TANGENT_ACCUM)".w\n");
                ADD_INS(text, "mul "TX(VSTMP_REG_TANGENT_ACCUM)".xyz, "TX(VSTMP_REG_TANGENT_ACCUM)", "TX(VSTMP_REG_TANGENT_ACCUM)".w\n");
            }

            /* Put a zero on the 'W' component */
            ADD_INS(text, "slt "TX(VSTMP_REG_TANGENT_ACCUM)".w, "TX(VSIN_REG_TANGENT)".w, "TX(VSIN_REG_TANGENT)".w\n");
        }
        else
        {
            ADD_INS(text, "mov "TX(VSTMP_REG_TANGENT_ACCUM)", "TX(VSIN_REG_TANGENT)"\n");
        }

        if (desc->morphing ||
            numWeights > 0)
        {
            /* Initialize 'W' component */
            ADD_INS(text, "mov %s.w, "TX(VSIN_REG_NORMAL)".w\n", inputNormals);
        }

        /* get binormal from dot product of tangent with the normal*/
        ADD_INS(text, "mul "TX(VSTMP_REG_BINORMAL_ACCUM)", "TX(VSTMP_REG_TANGENT_ACCUM)".zxyw, %s.yzxw\n", inputNormals);
        ADD_INS(text, "mad "TX(VSTMP_REG_BINORMAL_ACCUM)", "TX(VSTMP_REG_TANGENT_ACCUM)".yzxw, %s.zxyw, -"TX(VSTMP_REG_BINORMAL_ACCUM)"\n", inputNormals);

        /* normalize binormal */
        if (normalizeNormals)
        {
            ADD_INS(text, "dp3 "TX(VSTMP_REG_BINORMAL_ACCUM)".w, "TX(VSTMP_REG_BINORMAL_ACCUM)", "TX(VSTMP_REG_BINORMAL_ACCUM)"\n");
            ADD_INS(text, "rsq "TX(VSTMP_REG_BINORMAL_ACCUM)".w, "TX(VSTMP_REG_BINORMAL_ACCUM)".w\n");
            ADD_INS(text, "mul "TX(VSTMP_REG_BINORMAL_ACCUM)".xyz, "TX(VSTMP_REG_BINORMAL_ACCUM)", "TX(VSTMP_REG_BINORMAL_ACCUM)".w\n");
        }

        if (desc->prelit)
        {
            ADD_INS(text, "mov oD0, "TX(VSIN_REG_COLOR)"\n");
        }

        if (desc->numDirectionalLights)
        {
            /* move light vector into texture space */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".x, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"], "TX(VSTMP_REG_TANGENT_ACCUM)"\n");
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".y, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"], "TX(VSTMP_REG_BINORMAL_ACCUM)"\n");
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".z, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"], %s\n", inputNormals);

            /* normalize light direction */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".w, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_ILU_TMP)"\n");
            ADD_INS(text, "rsq "TX(VSTMP_REG_ILU_TMP)".w, "TX(VSTMP_REG_ILU_TMP)".w\n");
            ADD_INS(text, "mul "TX(VSTMP_REG_ILU_TMP)".xyz, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_ILU_TMP)".w\n");

            /* Scale to 0..1 */
            if (effect != rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP)
            {
                ADD_INS(text, "mad oT2.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w\n");
            }
            else
            {
                if (desc->prelit)
                {
                    ADD_INS(text, "mad oD1.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w\n");
                }
                else
                {
                    ADD_INS(text, "mad oD0.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w\n");
                }
            }
        }
        else if (desc->numPointLights)
        {
            /* get direction to light */
            ADD_INS(text, "add "TX(VSTMP_REG_DIST_TMP)".xyz, c["TX(1 + RWD3D9VSCONST_LIGHTS_OFFSET)"].xyz, -%s.xyz\n", inputPositions);

            /* get square distance to light and normalize direction vector */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)"\n");
            ADD_INS(text, "rsq "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w\n");
            ADD_INS(text, "mul "TX(VSTMP_REG_DIST_TMP)".xyz, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)".w\n");

            /* get atenuation = 1.f - distance / radius */
            /* {-1.f/radius, 1.f, 0.5f, 0.0f} */
            ADD_INS(text, "rcp "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w\n");
            ADD_INS(text, "mad "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].x, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].y\n");

            /* clamp atenuatioon */
            ADD_INS(text, "max "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w\n");

            /* move light vector into texture space */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".x, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_TANGENT_ACCUM)"\n");
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".y, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_BINORMAL_ACCUM)"\n");
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".z, "TX(VSTMP_REG_DIST_TMP)", %s\n", inputNormals);

            /* normalize light direction */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".w, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_ILU_TMP)"\n");
            ADD_INS(text, "rsq "TX(VSTMP_REG_ILU_TMP)".w, "TX(VSTMP_REG_ILU_TMP)".w\n");
            ADD_INS(text, "mul "TX(VSTMP_REG_ILU_TMP)".xyz, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_ILU_TMP)".w\n");

            /* apply attenuation to direction vector */
            ADD_INS(text, "mul "TX(VSTMP_REG_ILU_TMP)".xyz, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_DIST_TMP)".w\n");

            /* Scale to 0..1 */
            if (effect != rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP)
            {
                ADD_INS(text, "mad oT2.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z\n");
            }
            else
            {
                if (desc->prelit)
                {
                    ADD_INS(text, "mad oD1.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z\n");
                }
                else
                {
                    ADD_INS(text, "mad oD0.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z\n");
                }
            }
        }
        else if (desc->numSpotLights)
        {
            #ifdef VSTMP_REG_ANGLE_ATTEN_TMP
            #undef VSTMP_REG_ANGLE_ATTEN_TMP
            #endif

            #define VSTMP_REG_ANGLE_ATTEN_TMP   VSTMP_REG_BUMPDIR_TMP

            /* get direction from light to vertex */
            ADD_INS(text, "add "TX(VSTMP_REG_DIST_TMP)".xyz, c["TX(1 + RWD3D9VSCONST_LIGHTS_OFFSET)"].xyz, -%s.xyz\n", inputPositions);

            /* get square distance to light and normalize direction vector */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)"\n");
            ADD_INS(text, "rsq "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w\n");
            ADD_INS(text, "mul "TX(VSTMP_REG_DIST_TMP)".xyz, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)".w\n");

            /* (Direction from light to vector) dot (light direction) */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".w, "TX(VSTMP_REG_DIST_TMP)", c["TX(2 + RWD3D9VSCONST_LIGHTS_OFFSET)"]\n");

            /* get angle attenuation */
            /* {1.f / (1.f - cosangle), -cosangle / (1.f - cosangle), 0.5, 0.0f } */
            ADD_INS(text, "mad "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".w, "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".w, c["TX(3 + RWD3D9VSCONST_LIGHTS_OFFSET)"].x, c["TX(3 + RWD3D9VSCONST_LIGHTS_OFFSET)"].y\n");

            /* get distance atenuation = 1.f - distance / radius */
            /* {-1.f/radius, 1.f, 0.5f, 0.0f} */
            ADD_INS(text, "rcp "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w\n");
            ADD_INS(text, "mad "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].x, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].y\n");

            /* clamp distance atenuatioon */
            ADD_INS(text, "max "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w\n");

            /* clamp angle attenuation */
            ADD_INS(text, "max "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".w, "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".w, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].w\n");

            /* move light vector into texture space */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".x, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_TANGENT_ACCUM)"\n");
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".y, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_BINORMAL_ACCUM)"\n");
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".z, "TX(VSTMP_REG_DIST_TMP)", %s\n", inputNormals);

            /* normalize light direction */
            ADD_INS(text, "dp3 "TX(VSTMP_REG_ILU_TMP)".w, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_ILU_TMP)"\n");
            ADD_INS(text, "rsq "TX(VSTMP_REG_ILU_TMP)".w, "TX(VSTMP_REG_ILU_TMP)".w\n");
            ADD_INS(text, "mul "TX(VSTMP_REG_ILU_TMP)".xyz, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_ILU_TMP)".w\n");

            /* apply distance attenuation to direction vector */
            ADD_INS(text, "mul "TX(VSTMP_REG_ILU_TMP)".xyz, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_DIST_TMP)".w\n");

            /* apply angle attenuation to direction vector */
            ADD_INS(text, "mul "TX(VSTMP_REG_ILU_TMP)".xyz, "TX(VSTMP_REG_ILU_TMP)", "TX(VSTMP_REG_ANGLE_ATTEN_TMP)".w\n");

            /* Scale to 0..1 */
            if (effect != rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP)
            {
                ADD_INS(text, "mad oT2.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z\n");
            }
            else
            {
                if (desc->prelit)
                {
                    ADD_INS(text, "mad oD1.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z\n");
                }
                else
                {
                    ADD_INS(text, "mad oD0.xyz, "TX(VSTMP_REG_ILU_TMP)", c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z, c["TX(RWD3D9VSCONST_LIGHTS_OFFSET)"].z\n");
                }
            }
        }

        /* Calculate EnvMap coordinates */
        if (effect == rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP)
        {
            const RwUInt32 offsetEffect = dispatch->offsetEffect;

            RWASSERT(offsetEffect != 0xFF);

            ADD_INS(text, "dp3 oT2.x, c[%d], "TX(VSTMP_REG_TANGENT_ACCUM)"\n", offsetEffect);
            ADD_INS(text, "dp3 oT2.y, c[%d], "TX(VSTMP_REG_BINORMAL_ACCUM)"\n", offsetEffect);
            ADD_INS(text, "dp3 oT2.z, c[%d], %s\n", offsetEffect, inputNormals);

            ADD_INS(text, "dp3 oT3.x, c[%d], "TX(VSTMP_REG_TANGENT_ACCUM)"\n", 1 + offsetEffect);
            ADD_INS(text, "dp3 oT3.y, c[%d], "TX(VSTMP_REG_BINORMAL_ACCUM)"\n", 1 + offsetEffect);
            ADD_INS(text, "dp3 oT3.z, c[%d], %s\n", 1 + offsetEffect, inputNormals);
        }

        /* Output of tex coords */
        ADD_INS(text, "mov oT0.xy, v%d\n", VSD_REG_TEXCOORD0 + (RwUInt32)(desc->numTexCoords) - 1);
        ADD_INS(text, "mov oT1.xy, "TX(VSIN_REG_TEXCOORD0)"\n");
    }

    /*
     * TEXTURE COORDINATES
     */
    if (effect)
    {
        const RwUInt32 offsetEffect = dispatch->offsetEffect;

        switch (effect)
        {
            case rwD3D9VERTEXSHADEREFFECT_ENVMAP:
                if (desc->numTexCoords > 0)
                {
                    ADD_INS(text, "mov oT0.xy, "TX(VSIN_REG_TEXCOORD0)"\n");
                    ADD_INS(text, "dp4 oT1.x, %s, c[%d]\n", inputNormals, offsetEffect);
                    ADD_INS(text, "dp4 oT1.y, %s, c[%d]\n", inputNormals, 1 + offsetEffect);
                }
                else
                {
                    ADD_INS(text, "dp4 oT0.x, %s, c[%d]\n", inputNormals, offsetEffect);
                    ADD_INS(text, "dp4 oT0.y, %s, c[%d]\n", inputNormals, 1 + offsetEffect);
                }
                break;

            case rwD3D9VERTEXSHADEREFFECT_BUMPENVMAP:
                ADD_INS(text, "dp4 oT2.x, %s, c[%d]\n", inputNormals, VSCONST_REG_BUMP_SIZE + offsetEffect);
                ADD_INS(text, "dp4 oT2.y, %s, c[%d]\n", inputNormals, VSCONST_REG_BUMP_SIZE + 1 + offsetEffect);
                /* Fall to shared code */

            case rwD3D9VERTEXSHADEREFFECT_BUMPMAP:
                ADD_INS(text, "dp3 "TX(VSTMP_REG_BUMPDIR_TMP)".x, %s, c[%d]\n", inputNormals, offsetEffect);
                ADD_INS(text, "dp3 "TX(VSTMP_REG_BUMPDIR_TMP)".y, %s, c[%d]\n", inputNormals, 1 + offsetEffect);

                /* bump uv = bumpDir + tex uv */
                ADD_INS(text, "mov oT0.xy, "TX(VSIN_REG_TEXCOORD0)"\n");
                ADD_INS(text, "add oT1.xy, "TX(VSTMP_REG_BUMPDIR_TMP)".xy, "TX(VSIN_REG_TEXCOORD0)".xy\n");
                break;

            case rwD3D9VERTEXSHADEREFFECT_DUAL:
                ADD_INS(text, "mov oT0.xy, "TX(VSIN_REG_TEXCOORD1)"\n");
                break;

            case rwD3D9VERTEXSHADEREFFECT_DUALREPLICATE:
                ADD_INS(text, "mov oT0.xy, "TX(VSIN_REG_TEXCOORD0)"\n");
                ADD_INS(text, "mov oT1.xy, "TX(VSIN_REG_TEXCOORD0)"\n");
                break;

            case rwD3D9VERTEXSHADEREFFECT_UVANIM:
                ADD_INS(text, "dp4 oT0.x, "TX(VSIN_REG_TEXCOORD0)", c[%d]\n", offsetEffect);
                ADD_INS(text, "dp4 oT0.y, "TX(VSIN_REG_TEXCOORD0)", c[%d]\n", 1 + offsetEffect);
                break;

            case rwD3D9VERTEXSHADEREFFECT_DUALUVANIM:
                ADD_INS(text, "dp4 oT0.x, "TX(VSIN_REG_TEXCOORD1)", c[%d]\n", offsetEffect);
                ADD_INS(text, "dp4 oT0.y, "TX(VSIN_REG_TEXCOORD1)", c[%d]\n", 1 + offsetEffect);
                break;

            case rwD3D9VERTEXSHADEREFFECT_NORMALMAP:
                break;

            case rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP:
                break;

            case rwD3D9VERTEXSHADEREFFECT_CUBEMAP:
                /* get direction from vertex to eye */
                ADD_INS(text, "add "TX(VSTMP_REG_DIST_TMP)".xyz, c[%d].xyz, -%s.xyz\n", offsetEffect, inputPositions);

                /* normalize eye vector */
                ADD_INS(text, "dp3 "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)"\n");
                ADD_INS(text, "rsq "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w\n");
                ADD_INS(text, "mul "TX(VSTMP_REG_DIST_TMP)".xyz, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)".w\n");

                /* (normal dot eye ) */
                ADD_INS(text, "dp3 "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)", %s\n", inputNormals);
                /* 2 * (normal dot eye ) */
                ADD_INS(text, "add "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w\n");
                /* 2 * (normal dot eye ) * normal - eye*/
                ADD_INS(text, "mad "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)".w, %s, -"TX(VSTMP_REG_DIST_TMP)"\n", inputNormals);

                if (desc->numTexCoords > 0)
                {
                    ADD_INS(text, "mov oT0.xy, "TX(VSIN_REG_TEXCOORD0)"\n");
                    ADD_INS(text, "dp3 oT1.x, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 1 + offsetEffect);
                    ADD_INS(text, "dp3 oT1.y, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 2 + offsetEffect);
                    ADD_INS(text, "dp3 oT1.z, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 3 + offsetEffect);
                }
                else
                {
                    ADD_INS(text, "dp3 oT0.x, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 1 + offsetEffect);
                    ADD_INS(text, "dp3 oT0.y, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 2 + offsetEffect);
                    ADD_INS(text, "dp3 oT0.z, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 3 + offsetEffect);
                }
                break;

            case rwD3D9VERTEXSHADEREFFECT_BUMPCUBEMAP:
                ADD_INS(text, "dp3 "TX(VSTMP_REG_BUMPDIR_TMP)".x, %s, c[%d]\n", inputNormals, offsetEffect);
                ADD_INS(text, "dp3 "TX(VSTMP_REG_BUMPDIR_TMP)".y, %s, c[%d]\n", inputNormals, 1 + offsetEffect);

                /* bump uv = bumpDir + tex uv */
                ADD_INS(text, "mov oT0.xy, "TX(VSIN_REG_TEXCOORD0)"\n");
                ADD_INS(text, "add oT1.xy, "TX(VSTMP_REG_BUMPDIR_TMP)".xy, "TX(VSIN_REG_TEXCOORD0)".xy\n");

                /* get direction from vertex to eye */
                ADD_INS(text, "add "TX(VSTMP_REG_DIST_TMP)".xyz, c[%d].xyz, -%s.xyz\n", offsetEffect + VSCONST_REG_BUMP_SIZE, inputPositions);

                /* normalize eye vector */
                ADD_INS(text, "dp3 "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)"\n");
                ADD_INS(text, "rsq "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w\n");
                ADD_INS(text, "mul "TX(VSTMP_REG_DIST_TMP)".xyz, "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)".w\n");

                /* (normal dot eye ) */
                ADD_INS(text, "dp3 "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)", %s\n", inputNormals);
                /* 2 * (normal dot eye ) */
                ADD_INS(text, "add "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w, "TX(VSTMP_REG_DIST_TMP)".w\n");
                /* 2 * (normal dot eye ) * normal - eye*/
                ADD_INS(text, "mad "TX(VSTMP_REG_DIST_TMP)", "TX(VSTMP_REG_DIST_TMP)".w, %s, -"TX(VSTMP_REG_DIST_TMP)"\n", inputNormals);

                ADD_INS(text, "dp3 oT2.x, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 1 + offsetEffect + VSCONST_REG_BUMP_SIZE);
                ADD_INS(text, "dp3 oT2.y, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 2 + offsetEffect + VSCONST_REG_BUMP_SIZE);
                ADD_INS(text, "dp3 oT2.z, "TX(VSTMP_REG_DIST_TMP)", c[%d]\n", 3 + offsetEffect + VSCONST_REG_BUMP_SIZE);
                break;

            default:
                for (n = 0; n < desc->numTexCoords; n++)
                {
                    ADD_INS(text, "mov oT%u.xy, v%u\n", n, n + VSD_REG_TEXCOORD0);
                }
                break;
        }
    }
    else
    {
        for (n = 0; n < desc->numTexCoords; n++)
        {
            ADD_INS(text, "mov oT%u.xy, v%u\n", n, n + VSD_REG_TEXCOORD0);
        }
    }

    /* Compile shader */
    RWASSERT((RwUInt32)(text - TextArray) < MAX_TEXT_SIZE);

#ifdef RWDEBUG

    if (D3D9UseVersion == VERSION1)
    {
        RWASSERT(NumInstructions <= 128);
    }
#ifdef VERSION3
    else if (D3D9UseVersion == VERSION3)
    {
        RWASSERT(NumInstructions <= 512);
    }
#endif
    else
    {
        RWASSERT(NumInstructions <= 256);
    }

    #ifdef DUMP_SHADER
    RWSRCGLOBAL(debugFunction)(rwDEBUGMESSAGE, "\n_rpD3D9GenerateVertexShader:");
    RWSRCGLOBAL(debugFunction)(rwDEBUGMESSAGE, TextArray);
    #endif

    hr = D3DXAssembleShader(TextArray,
                            (RwUInt32)(text - TextArray),
                            NULL,
                            NULL,
                            D3DXSHADER_DEBUG,
                            &shaderTokens,
                            NULL);

    #ifdef DUMP_SHADER
    rwsprintf(TextArray, "%d Instructions.\n", NumInstructions);
    RWSRCGLOBAL(debugFunction)(rwDEBUGMESSAGE, TextArray);
    #endif

#else
    hr = D3DXAssembleShader(TextArray,
                            (RwUInt32)(text - TextArray),
                            NULL,
                            NULL,
                            D3DXSHADER_SKIPVALIDATION,
                            &shaderTokens,
                            NULL);
#endif

    if (SUCCEEDED(hr))
    {
        const RwUInt32 *function = ID3DXBuffer_GetBufferPointer(shaderTokens);

        RwD3D9CreateVertexShader(function, &shader);
    }

    if (shaderTokens != NULL)
    {
        ID3DXBuffer_Release(shaderTokens);
    }

    RWRETURN(shader);
}

/****************************************************************************
 D3D9BinaryFindVertexShader

 Return:
    * >= 0 -> the value was found and the result is the index in the array.
    * <  0 -> the value was not found, but the index of the closer higher
              value will be - (index + 1).
 */
static RwInt32
D3D9BinaryFindVertexShader (const _rpD3D9VertexShaderDescriptor *value,
                            const _rpD3D9VertexShaderItemDesc *buffer,
                            RwInt32 numElems)
{
    RwInt32 lowerValue;

    RWFUNCTION(RWSTRING("D3D9BinaryFindVertexShader"));
    RWASSERT(buffer);

    lowerValue = 0;

    if (numElems > 0)
    {
        RwInt32 higherValue;

        higherValue = (numElems - 1);

        do
        {
            if (numElems > 1)
            {
                RwInt32 midleValue = lowerValue + (numElems / 2) -
                                     (1 - ((RwUInt32)(numElems) & 1));

                if (IS_SHADERDESC_EQUAL(*value, buffer[midleValue].desc))
                {
                    RWRETURN(midleValue);
                }
                else if (IS_SHADERDESC_LESS(*value, buffer[midleValue].desc))
                {
                    higherValue = midleValue - 1;
                }
                else
                {
                    lowerValue = midleValue + 1;
                }
            }
            else
            {
                if (IS_SHADERDESC_EQUAL(*value, buffer[lowerValue].desc))
                {
                    RWRETURN(lowerValue);
                }
                else if (IS_SHADERDESC_LESS(*value, buffer[lowerValue].desc))
                {
                    RWRETURN( - (lowerValue + 1) );
                }
                else
                {
                    RWRETURN( - (lowerValue + 1 + 1) );
                }
            }

            numElems = higherValue - lowerValue + 1;
        }
        while (numElems > 0);
    }

    RWRETURN( - (lowerValue + 1) );
}

/*****************************************************************************
 _rpD3D9VertexShaderCachePurge
*/
void
_rpD3D9VertexShaderCachePurge(void)
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("_rpD3D9VertexShaderCachePurge"));

    RWASSERT(sizeof(_rpD3D9VertexShaderDescriptor) == 4);

    for (i = 0; i < NumShadersInCache; i++)
    {
        if (VertexShaderCache[i].shader != NULL)
        {
            RwD3D9DeleteVertexShader(VertexShaderCache[i].shader);
            VertexShaderCache[i].shader = NULL;
        }
    }

    NumShadersInCache = 0;

    LastVertexShaderDesc = VertexShaderCache;

    memset(LastVertexShaderDesc, 0xFF, sizeof(_rpD3D9VertexShaderItemDesc));

    RWRETURNVOID();
}

/*****************************************************************************
 _rpD3D9GetVertexShader
*/
void *
_rpD3D9GetVertexShader(const _rpD3D9VertexShaderDescriptor *desc,
                       _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    RwInt32 index;
    void *shader = NULL;

    RWFUNCTION(RWSTRING("_rpD3D9GetVertexShader"));
    RWASSERT(desc != NULL);
    RWASSERT(dispatch != NULL);
    RWASSERT(D3D9UseVersion >= VERSION1);

    /* Check if it's the same that the latest used shader */
    RWASSERT(LastVertexShaderDesc != NULL);
    if (IS_SHADERDESC_EQUAL(LastVertexShaderDesc->desc, *desc))
    {
        LastVertexShaderDesc->renderFrame = RWSRCGLOBAL(renderFrame);

        *dispatch = LastVertexShaderDesc->dispatch;

        RWRETURN(LastVertexShaderDesc->shader);
    }

    /* Search for existing shader */
    index = D3D9BinaryFindVertexShader(desc, VertexShaderCache, NumShadersInCache);

    if ( index >= 0)
    {
        RWASSERT(IS_SHADERDESC_EQUAL(*desc, VertexShaderCache[index]));

        LastVertexShaderDesc = &(VertexShaderCache[index]);

        LastVertexShaderDesc->renderFrame = RWSRCGLOBAL(renderFrame);

        *dispatch = LastVertexShaderDesc->dispatch;

        RWRETURN(LastVertexShaderDesc->shader);
    }

    /* Create new shader*/
    shader = _rpD3D9GenerateVertexShader(desc, dispatch);

    /* Add to cache */
    if (NumShadersInCache >= D3D9_VERTEX_SHADER_CACHE_SIZE)
    {
        _rpD3D9VertexShaderItemDesc *lessUsed;

        lessUsed = &VertexShaderCache[0];

        for (index = 1; index < NumShadersInCache; index++)
        {
            if (VertexShaderCache[index].renderFrame < lessUsed->renderFrame)
            {
                lessUsed = &(VertexShaderCache[index]);
            }
        }

        lessUsed->desc = *desc;
        lessUsed->renderFrame = RWSRCGLOBAL(renderFrame);

        if (lessUsed->shader != NULL)
        {
            RwD3D9DeleteVertexShader(lessUsed->shader);
        }

        lessUsed->shader = shader;

        lessUsed->dispatch = *dispatch;

        LastVertexShaderDesc = lessUsed;
    }
    else
    {
        /* Add in the right place */
        index = - (index + 1);

        RWASSERT(index <= NumShadersInCache);

        if (index < NumShadersInCache)
        {
            memmove(VertexShaderCache + index + 1,
                    VertexShaderCache + index,
                    (NumShadersInCache - index) * sizeof(_rpD3D9VertexShaderItemDesc));
        }

        LastVertexShaderDesc = &(VertexShaderCache[index]);

        LastVertexShaderDesc->desc = *desc;
        LastVertexShaderDesc->renderFrame = RWSRCGLOBAL(renderFrame);
        LastVertexShaderDesc->shader = shader;
        LastVertexShaderDesc->dispatch = *dispatch;

        NumShadersInCache++;
    }

    RWASSERT(shader != NULL && "No suitable vertex shader can be generated for this combination of lights & materials");
    RWRETURN(shader);
}

/*****************************************************************************
 _rpD3D9VertexShaderCacheOpen
*/
void
_rpD3D9VertexShaderCacheOpen(void)
{
    RWAPIFUNCTION(RWSTRING("_rpD3D9VertexShaderCacheOpen"));

    /* Init cache */
    NumShadersInCache = 0;

    LastVertexShaderDesc = VertexShaderCache;

    memset(LastVertexShaderDesc, 0xFF, sizeof(_rpD3D9VertexShaderItemDesc));

    /* Init effect sizes */
    memset(D3D9NumConstantsEffect, 0, rwD3D9VERTEXSHADEREFFECT_MAX * sizeof(RwUInt32));

    D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_BUMPMAP] =
        VSCONST_REG_BUMP_SIZE;

    D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_ENVMAP] =
        VSCONST_REG_ENV_SIZE;

    D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_BUMPENVMAP] =
        VSCONST_REG_BUMP_SIZE +
        VSCONST_REG_ENV_SIZE;

    D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_UVANIM] =
        VSCONST_REG_ENV_SIZE;

    D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_DUALUVANIM] =
        VSCONST_REG_ENV_SIZE;

    D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP] =
        VSCONST_REG_ENV_SIZE;

    D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_CUBEMAP] = 4;

    D3D9NumConstantsEffect[rwD3D9VERTEXSHADEREFFECT_BUMPCUBEMAP] =
        VSCONST_REG_BUMP_SIZE + 4;

    /* Check vertex shader version */
    if ( (_RwD3D9DeviceCaps.VertexShaderVersion & 0xFFFF) >= 0x0300 )
    {
        #ifdef VERSION3
        RwDebugSendMessage(rwDEBUGMESSAGE, "World plugin", "Device supports vertex shaders version 3_0 and using them.");

        D3D9UseVersion = VERSION3;
        #else
        RwDebugSendMessage(rwDEBUGMESSAGE, "World plugin", "Device supports vertex shaders version 2_x and using them.");

        D3D9UseVersion = VERSION2X;
        #endif
    }
    else if ( (_RwD3D9DeviceCaps.VertexShaderVersion & 0xFFFF) >= 0x0200 )
    {
        if (_RwD3D9DeviceCaps.VS20Caps.DynamicFlowControlDepth > 0)
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "World plugin", "Device supports vertex shaders version 2_x and using them.");

            D3D9UseVersion = VERSION2X;
        }
        else
        {
            RwDebugSendMessage(rwDEBUGMESSAGE, "World plugin", "Device supports vertex shaders version 2_0 and using them.");

            D3D9UseVersion = VERSION2;
        }
    }
    else if ( (_RwD3D9DeviceCaps.VertexShaderVersion & 0xFFFF) >= 0x0100)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "World plugin", "Device supports vertex shaders version 1_1 and using them.");

        D3D9UseVersion = VERSION1;

        /* Fix decl types supported */
        _RwD3D9DeviceCaps.DeclTypes &= ~(D3DDTCAPS_USHORT4N |
                                         D3DDTCAPS_USHORT2N |
                                         D3DDTCAPS_SHORT4N |
                                         D3DDTCAPS_SHORT2N |
                                         D3DDTCAPS_UBYTE4N);
    }
    else
    {
        D3D9UseVersion = 0;
    }

    if (D3D9UseVersion >= VERSION1)
    {
        _rwD3D9VertexShaderMaxConstants = min(_RwD3D9DeviceCaps.MaxVertexShaderConst, 256);
    }

    RWRETURNVOID();
}

/*
 * Vertex shader pipeline
 */

/****************************************************************************
 _rxD3D9VertexShaderDefaultBeginCallBack
 */
void
_rxD3D9VertexShaderDefaultBeginCallBack(void *object,
                                        RwUInt32 type,
                                        _rpD3D9VertexShaderDescriptor  *desc)
{
    RwBool   fogEnable;

    RWFUNCTION(RWSTRING("_rxD3D9VertexShaderDefaultBeginCallBack"));

    /* Initialize descriptor */
    memset(desc, 0, sizeof(_rpD3D9VertexShaderDescriptor));

    if (type == rpATOMIC)
    {
        RpAtomic *atomic;
        const RpGeometry *geometry;
        RwUInt32 geomFlags;

        atomic = (RpAtomic *)object;

        geometry = RpAtomicGetGeometry(atomic);

        geomFlags = RpGeometryGetFlags(geometry);

        if (RpGeometryGetNumMorphTargets(geometry) != 1)
        {
            const RpInterpolator *interp = &(atomic->interpolator);

            if (interp->startMorphTarget != interp->endMorphTarget)
            {
                RwReal scale;

                scale = (interp->recipTime) * (interp->position);

                if (scale != 0.0f)
                {
                    desc->morphing = 1;
                }
            }
        }

        desc->numTexCoords = RpGeometryGetNumTexCoordSets(geometry);

        if (geomFlags & rxGEOMETRY_PRELIT)
        {
            desc->prelit = 1;
        }

        if (geomFlags & rxGEOMETRY_NORMALS)
        {
            RpD3D9GeometryUsageFlag usageFlags;

            usageFlags = RpD3D9GeometryGetUsageFlags(geometry);

            desc->normals  = 1;

            if (usageFlags & rpD3D9GEOMETRYUSAGE_DYNAMICMASK)
            {
                desc->normalizeNormals = 1;
            }
            else
            {
                const RwMatrix *matrix;

                matrix = RwFrameGetLTM(RpAtomicGetFrame((RpAtomic *)object));

                if (!rwMatrixTestFlags(matrix, (rwMATRIXTYPENORMAL | rwMATRIXINTERNALIDENTITY)))
                {
                    const RwReal minlimit = 0.9f;
                    const RwReal maxlimit = 1.1f;
                    RwReal length;

                    length = RwV3dDotProduct(&(matrix->right), &(matrix->right));

                    if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
                    {
                        desc->normalizeNormals = 1;
                    }
                    else
                    {
                        length = RwV3dDotProduct(&(matrix->up), &(matrix->up));

                        if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
                        {
                            desc->normalizeNormals = 1;
                        }
                        else
                        {
                            length = RwV3dDotProduct(&(matrix->at), &(matrix->at));

                            if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
                            {
                                desc->normalizeNormals = 1;
                            }
                        }
                    }
                }
            }
        }

        if (geomFlags & rxGEOMETRY_MODULATE)
        {
            desc->modulateMaterial = 1;
        }
    }
    else
    {
        RwUInt32 flags;

        flags = RpWorldGetFlags((const RpWorld *)RWSRCGLOBAL(curWorld));

        if (flags & rpWORLDTEXTURED2)
        {
            desc->numTexCoords = 2;
        }
        else if (flags & rxGEOMETRY_TEXTURED)
        {
            desc->numTexCoords = 1;
        }

        if (flags & rxGEOMETRY_PRELIT)
        {
            desc->prelit = 1;
        }

        if (flags & rxGEOMETRY_NORMALS)
        {
            desc->normals  = 1;
        }

        if (flags & rxGEOMETRY_MODULATE)
        {
            desc->modulateMaterial = 1;
        }
    }

    RwD3D9GetRenderState(D3DRS_FOGENABLE, &fogEnable);
    if (fogEnable)
    {
        RwFogType   fogType;

        RwRenderStateGet(rwRENDERSTATEFOGTYPE, &fogType);

        switch (fogType)
        {
            case rwFOGTYPELINEAR:
                desc->fogMode = rwD3D9VERTEXSHADERFOG_LINEAR;
                break;

            case rwFOGTYPEEXPONENTIAL:
                desc->fogMode = rwD3D9VERTEXSHADERFOG_EXP;
                break;

            case rwFOGTYPEEXPONENTIAL2:
                desc->fogMode = rwD3D9VERTEXSHADERFOG_EXP2;
                break;
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D9VertexShaderDefaultEndCallBack
 */
void
_rxD3D9VertexShaderDefaultEndCallBack(void *object,
                                      RwUInt32 type,
                                      _rpD3D9VertexShaderDescriptor  *desc)
{
    RWFUNCTION(RWSTRING("_rxD3D9VertexShaderDefaultEndCallBack"));

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D9VertexShaderDefaultGetMaterialShaderCallBack
 */
void *
_rxD3D9VertexShaderDefaultGetMaterialShaderCallBack(const RpMaterial *material,
                                                    _rpD3D9VertexShaderDescriptor *desc,
                                                    _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    RWFUNCTION(RWSTRING("_rxD3D9VertexShaderDefaultGetMaterialShaderCallBack"));
    RWASSERT(material != NULL);
    RWASSERT(desc != NULL);
    RWASSERT(dispatch != NULL);

    RWRETURN(_rpD3D9GetVertexShader(desc, dispatch));
}

/****************************************************************************
 _rxD3D9VertexShaderDefaultMeshRenderCallBack
 */
void
_rxD3D9VertexShaderDefaultMeshRenderCallBack(RxD3D9ResEntryHeader *resEntryHeader,
                                             RxD3D9InstanceData *instancedMesh,
                                             const _rpD3D9VertexShaderDescriptor  *desc,
                                             const _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    const RpMaterial    *material;

    RWFUNCTION(RWSTRING("_rxD3D9VertexShaderDefaultMeshRenderCallBack"));
    RWASSERT(resEntryHeader != NULL);
    RWASSERT(instancedMesh);
    RWASSERT(desc != NULL);
    RWASSERT(dispatch != NULL);
    RWASSERT(instancedMesh->material != NULL);

    /*
     * Set the Default Pixel shader
     */
    RwD3D9SetPixelShader(NULL);

    material = instancedMesh->material;

    if (material->texture != NULL)
    {
        RwD3D9SetTexture(material->texture, 0);

        RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }
    else
    {
        RwD3D9SetTexture(NULL, 0);

        RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }

    /*
    * Vertex shader
    */
    RwD3D9SetVertexShader(instancedMesh->vertexShader);

    /*
     * Render
     */
    if (resEntryHeader->indexBuffer != NULL)
    {
        RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
                                    instancedMesh->baseIndex,
                                    0, instancedMesh->numVertices,
                                    instancedMesh->startIndex, instancedMesh->numPrimitives);
    }
    else
    {
        RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
                            instancedMesh->baseIndex,
                            instancedMesh->numPrimitives);
    }

    RWRETURNVOID();
}

/****************************************************************************
 FindAtomicLights
 */
static RwBool
FindAtomicLights(RpAtomic *atomic,
                 RwV4d *ambientLightConstant,
                 _rpD3D9VertexShaderLights  *lightsArray,
                 RwBool hasNormals)
{
    RwBool      haveLights;
    RwLLLink    *cur, *end;
    RpWorld     *world;

    RWFUNCTION(RWSTRING("FindAtomicLights"));

    haveLights = FALSE;

    /*
     * GLOBAL LIGHTS
     */
    world = (RpWorld *)RWSRCGLOBAL(curWorld);

    cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
    end = rwLinkListGetTerminator(&world->directionalLightList);
    while (cur != end)
    {
        RpLight *light;

        light = rwLLLinkGetData(cur, RpLight, inWorld);

        /* NB light may actually be a dummyTie from a enclosing ForAll */
        if (light != NULL &&
            (rwObjectTestFlags(light, rpLIGHTLIGHTATOMICS)))
        {
            switch (RpLightGetType(light))
            {
                case rpLIGHTAMBIENT:
                    {
                        const RwRGBAReal    *color;

                        color = RpLightGetColor(light);

                        ambientLightConstant->x += color->red;
                        ambientLightConstant->y += color->green;
                        ambientLightConstant->z += color->blue;
                    }
                    break;

                case rpLIGHTDIRECTIONAL:
                    if (hasNormals)
                    {
                        if (lightsArray->numDirectional < MAX_LIGHTS)
                        {
                            lightsArray->directional[lightsArray->numDirectional] = light;

                            lightsArray->numDirectional += 1;

                            haveLights = TRUE;
                        }
                    }
                    break;

                default:
                    RWASSERT(0&&"unsupported light type");
            }
        }

        /* Next */
        cur = rwLLLinkGetNext(cur);
    } /* while */

    /*
     * POINT and SPOT lights
     */
    if (hasNormals)
    {
        /* don't light this atomic with the same light more than once! */
        RWSRCGLOBAL(lightFrame)++;

        /* For all sectors that this atomic lies in, apply all lights within */
        cur = rwLinkListGetFirstLLLink(&atomic->llWorldSectorsInAtomic);
        end = rwLinkListGetTerminator(&atomic->llWorldSectorsInAtomic);
        while (cur != end)
        {
            RpTie       *tpTie = rwLLLinkGetData(cur, RpTie, lWorldSectorInAtomic);
            RwLLLink    *curLight, *endLight;

            /* Lights in the sector */
            curLight = rwLinkListGetFirstLLLink(&tpTie->worldSector->lightsInWorldSector);
            endLight = rwLinkListGetTerminator(&tpTie->worldSector->lightsInWorldSector);

            while (curLight != endLight)
            {
                RpLightTie  *lightTie;
                RpLight     *light;

                lightTie = rwLLLinkGetData(curLight, RpLightTie, lightInWorldSector);

                light = lightTie->light;

                /* NB lightTie may actually be a dummyTie from a enclosing ForAll */

                /* Check to see if the light has already been applied and is set to
                * light atomics
                */
                if ( light != NULL &&
                    (rwObjectTestFlags(light, rpLIGHTLIGHTATOMICS)) &&
                    (light->lightFrame != RWSRCGLOBAL(lightFrame)))
                {
                    const RwMatrix  *matrixLight;
                    const RwV3d     *pos;
                    const RwSphere  *sphere;
                    RwV3d           distanceVector;
                    RwReal          distanceSquare;
                    RwReal          distanceCollision;

                    /* don't light this atomic with the same light again! */
                    light->lightFrame = RWSRCGLOBAL(lightFrame);

                    /* Does the light intersect the atomics bounding sphere */
                    matrixLight = RwFrameGetLTM(RpLightGetFrame(light));

                    pos = &(matrixLight->pos);

                    sphere = RpAtomicGetWorldBoundingSphere(atomic);

                    RwV3dSubMacro(&distanceVector, &(sphere->center), pos);

                    distanceSquare = RwV3dDotProduct(&distanceVector, &distanceVector);

                    distanceCollision = (sphere->radius + RpLightGetRadius(light));

                    if (distanceSquare < (distanceCollision * distanceCollision))
                    {
                        switch (RpLightGetType(light))
                        {
                            case rpLIGHTPOINT:
                                if (lightsArray->numPoint < MAX_LIGHTS)
                                {
                                    lightsArray->point[lightsArray->numPoint] = light;

                                    lightsArray->numPoint += 1;

                                    haveLights = TRUE;
                                }
                                break;

                            case rpLIGHTSPOT:
                            case rpLIGHTSPOTSOFT:
                                if (lightsArray->numSpot < MAX_LIGHTS)
                                {
                                    lightsArray->spot[lightsArray->numSpot] = light;

                                    lightsArray->numSpot += 1;

                                    haveLights = TRUE;
                                }
                                break;

                            default:
                                break;
                        }
                    }
                }

                /* Next */
                curLight = rwLLLinkGetNext(curLight);
            }

            /* Next one */
            cur = rwLLLinkGetNext(cur);
        }
    }

    RWRETURN(haveLights);
}

/****************************************************************************
 FindSectorLights
 */
static RwBool
FindSectorLights(const RpWorldSector *worldSector,
                 RwV4d *ambientLightConstant,
                 _rpD3D9VertexShaderLights  *lightsArray)
{
    RwBool      haveLights;
    RpWorld     *world;
    RwLLLink    *cur, *end;
    const RwLLLink  *curLight, *endLight;

    RWFUNCTION(RWSTRING("FindSectorLights"));

    haveLights = FALSE;

    /*
     * GLOBAL LIGHTS
     */
    world = (RpWorld *)RWSRCGLOBAL(curWorld);

    cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
    end = rwLinkListGetTerminator(&world->directionalLightList);
    while (cur != end)
    {
        RpLight *light;

        light = rwLLLinkGetData(cur, RpLight, inWorld);

        /* NB light may actually be a dummyTie from a enclosing ForAll */
        if (light != NULL &&
            (rwObjectTestFlags(light, rpLIGHTLIGHTWORLD)))
        {
            switch (RpLightGetType(light))
            {
                case rpLIGHTAMBIENT:
                    {
                        const RwRGBAReal    *color;

                        color = RpLightGetColor(light);

                        ambientLightConstant->x += color->red;
                        ambientLightConstant->y += color->green;
                        ambientLightConstant->z += color->blue;
                    }
                    break;

                case rpLIGHTDIRECTIONAL:
                    if (lightsArray->numDirectional < MAX_LIGHTS)
                    {
                        lightsArray->directional[lightsArray->numDirectional] = light;

                        lightsArray->numDirectional += 1;

                        haveLights = TRUE;
                    }
                    break;

                default:
                    RWASSERT(0&&"unsupported light type");
            }
        }

        /* Next */
        cur = rwLLLinkGetNext(cur);
    } /* while */

    /* don't light this sector with the same light more than once! */
    RWSRCGLOBAL(lightFrame)++;

    /*
     * POINT and SPOT lights
     */
    /* Lights in the sector */
    curLight = rwLinkListGetFirstLLLink(&worldSector->lightsInWorldSector);
    endLight = rwLinkListGetTerminator(&worldSector->lightsInWorldSector);
    while (curLight != endLight)
    {
        RpLightTie  *lightTie;
        RpLight     *light;

        lightTie = rwLLLinkGetData(curLight, RpLightTie, lightInWorldSector);

        light = lightTie->light;

        /* NB lightTie may actually be a dummyTie from a enclosing ForAll */

        /* Check to see if the light has already been applied and is set to
         * light sectors
         */
        if (light != NULL &&
            (rwObjectTestFlags(light, rpLIGHTLIGHTWORLD)) &&
            (light->lightFrame != RWSRCGLOBAL(lightFrame)))
        {
            /* don't light this sector with the same light again! */
            light->lightFrame = RWSRCGLOBAL(lightFrame);

            switch (RpLightGetType(light))
            {
                case rpLIGHTPOINT:
                    if (lightsArray->numPoint < MAX_LIGHTS)
                    {
                        lightsArray->point[lightsArray->numPoint] = light;

                        lightsArray->numPoint += 1;

                        haveLights = TRUE;
                    }
                    break;

                case rpLIGHTSPOT:
                case rpLIGHTSPOTSOFT:
                    if (lightsArray->numSpot < MAX_LIGHTS)
                    {
                        lightsArray->spot[lightsArray->numSpot] = light;

                        lightsArray->numSpot += 1;

                        haveLights = TRUE;
                    }
                    break;

                default:
                    break;
            }
        }

        /* Next */
        curLight = rwLLLinkGetNext(curLight);
    }

    RWRETURN(haveLights);
}

/****************************************************************************
 _rxD3D9VertexShaderDefaultLightingCallBack
 */
RwV4d *_rxD3D9VertexShaderDefaultLightingCallBack(void *object,
                                                  RwUInt32 type,
                                                  RwV4d    *shaderConstantPtr,
                                                  _rpD3D9VertexShaderDescriptor  *desc)
{
    RpGeometryFlag  flags;

    RWFUNCTION(RWSTRING("_rxD3D9VertexShaderDefaultLightingCallBack"));

    /* assume no lights until proved otherwise */
    desc->numDirectionalLights = 0;
    desc->numPointLights = 0;
    desc->numSpotLights = 0;

    if (type == rpATOMIC)
    {
        flags = (RpGeometryFlag)RpGeometryGetFlags(RpAtomicGetGeometry((RpAtomic *)object));
    }
    else
    {
        flags = RpWorldGetFlags((const RpWorld *)RWSRCGLOBAL(curWorld));
    }

    /* ...start ambient at black */
    shaderConstantPtr->x = 0.0f;
    shaderConstantPtr->y = 0.0f;
    shaderConstantPtr->z = 0.0f;
    if (flags & rxGEOMETRY_PRELIT)
    {
        shaderConstantPtr->w = 0.0f;
    }
    else
    {
        shaderConstantPtr->w = 1.0f;
    }

    /* if the object's light-able */
    if (flags & rxGEOMETRY_LIGHT)
    {
        /* ...add to it if we find any ambients */
        if (NULL != RWSRCGLOBAL(curWorld))
        {
            _rpD3D9VertexShaderLights  lightsArray;
            RwBool haveLights;

            lightsArray.numDirectional = 0;
            lightsArray.numPoint = 0;
            lightsArray.numSpot = 0;

            if (type == rpATOMIC)
            {
                haveLights = FindAtomicLights((RpAtomic *)object,
                                              shaderConstantPtr,
                                              &lightsArray,
                                              (flags & rxGEOMETRY_NORMALS) != 0);
            }
            else
            {
                RWASSERT(type == ((RwUInt8)rwSECTORATOMIC));

                haveLights = FindSectorLights((const RpWorldSector *)object,
                                              shaderConstantPtr,
                                              &lightsArray);
            }

            shaderConstantPtr += VSCONST_REG_AMBIENT_SIZE;

            if (haveLights)
            {
                const RwRGBAReal    *color;
                RwUInt32 lightBlock, n;
                RpLight *light;
                RwV3d   *at;
                RwV3d   *pos;
                RwReal  *radiiConstant;
                RwReal  angle;
                RwReal  cosangle;

                /* Directionals */
                if (lightsArray.numDirectional)
                {
                    for (n = 0; n < lightsArray.numDirectional; n++)
                    {
                        light = lightsArray.directional[n];

                        /* Set the lights direction, in object space */
                        at = RwMatrixGetAt(RwFrameGetLTM(RpLightGetFrame(light)));

                        _rwD3D9VSGetNormalInLocalSpace(at, (RwV3d *)shaderConstantPtr);

                        shaderConstantPtr->w = 0.0f; /* Use this for clamping */
                        shaderConstantPtr++;

                        /* Set the light color */
                        color = RpLightGetColor(light);
                        shaderConstantPtr->x = color->red;
                        shaderConstantPtr->y = color->green;
                        shaderConstantPtr->z = color->blue;
                        shaderConstantPtr->w = 0.f;
                        shaderConstantPtr++;
                    }

                    desc->numDirectionalLights = lightsArray.numDirectional;
                }

                /* Point */
                if (lightsArray.numPoint)
                {
                    lightBlock = 0;
                    for (n = 0; n < lightsArray.numPoint; n++)
                    {
                        light = lightsArray.point[n];

                        /* pack 4 radii into one constant for better vectorization of intensity calcs in shader */
                        if ( (lightBlock % 4) == 0 )
                        {
                            radiiConstant = (RwReal *)shaderConstantPtr;
                            shaderConstantPtr++;
                        }

                        lightBlock++;

                        /* Set the light's position, in object space */
                        pos = RwMatrixGetPos(RwFrameGetLTM(RpLightGetFrame(light)));

                        _rwD3D9VSGetPointInLocalSpace(pos, (RwV3d *)shaderConstantPtr);

                        shaderConstantPtr->w = 0.0f; /* Use this for clamping */
                        shaderConstantPtr++;

                        /* Set the light color */
                        color = RpLightGetColor(light);
                        shaderConstantPtr->x = color->red;
                        shaderConstantPtr->y = color->green;
                        shaderConstantPtr->z = color->blue;
                        shaderConstantPtr->w = 0.f;
                        shaderConstantPtr++;

                        /* and the radius */
                        _rwD3D9VSGetRadiusInLocalSpace(RpLightGetRadius(light), radiiConstant);

                        *radiiConstant = 1.f / *radiiConstant;

                        radiiConstant++;
                    }

                    desc->numPointLights = lightsArray.numPoint;
                }

                /* Spot */
                if (lightsArray.numSpot)
                {
                    lightBlock = 0;
                    for (n = 0; n < lightsArray.numSpot; n++)
                    {
                        light = lightsArray.spot[n];

                        /* pack 4 radii into one constant for better vectorization of intensity calcs in shader */
                        if ( (lightBlock % 4) == 0 )
                        {
                            radiiConstant = (RwReal *)shaderConstantPtr;
                            shaderConstantPtr++;
                        }

                        lightBlock++;

                        /* Set the light's position, in object space */
                        pos = RwMatrixGetPos(RwFrameGetLTM(RpLightGetFrame(light)));

                        _rwD3D9VSGetPointInLocalSpace(pos, (RwV3d *)shaderConstantPtr);

                        shaderConstantPtr->w = 0.0f; /* Use this for clamping */
                        shaderConstantPtr++;

                        /* Set the lights direction, in object space */
                        at = RwMatrixGetAt(RwFrameGetLTM(RpLightGetFrame(light)));

                        _rwD3D9VSGetNormalInLocalSpace(at, (RwV3d *)shaderConstantPtr);

                        shaderConstantPtr->w = 0.0f; /* Use this for clamping */
                        shaderConstantPtr++;

                        /* Angles range */
                        angle = RpLightGetConeAngle(light);

                        cosangle = (RwReal)RwCos(angle);

                        if (RpLightGetType(light) == rpLIGHTSPOT ||
                            cosangle >= 0.999f)
                        {
                            shaderConstantPtr->x = (1.f / 0.001f);
                            shaderConstantPtr->y = (-cosangle) * (shaderConstantPtr->x);
                            shaderConstantPtr->z = 0.5f;
                            shaderConstantPtr->w = 0.0f;
                        }
                        else
                        {
                            shaderConstantPtr->x = (1.f / (1.f - cosangle));
                            shaderConstantPtr->y = (-cosangle) * (shaderConstantPtr->x);
                            shaderConstantPtr->z = 0.5f;
                            shaderConstantPtr->w = 0.0f;
                        }
                        shaderConstantPtr++;

                        /* Set the light color */
                        color = RpLightGetColor(light);
                        shaderConstantPtr->x = color->red;
                        shaderConstantPtr->y = color->green;
                        shaderConstantPtr->z = color->blue;
                        shaderConstantPtr->w = 0.f;
                        shaderConstantPtr++;

                        /* and the radius */
                        _rwD3D9VSGetRadiusInLocalSpace(RpLightGetRadius(light), radiiConstant);

                        *radiiConstant = 1.f / *radiiConstant;

                        radiiConstant++;
                    }

                    desc->numSpotLights = lightsArray.numSpot;
                }
            }
        }
        else
        {
            shaderConstantPtr += VSCONST_REG_AMBIENT_SIZE;
        }
    }
    else
    {
        shaderConstantPtr += VSCONST_REG_AMBIENT_SIZE;
    }

    RWRETURN(shaderConstantPtr);
}

/****************************************************************************
 _rpD3D9VertexShaderUpdateLightsColors
 */
void
_rpD3D9VertexShaderUpdateLightsColors(RwV4d *shaderConstantPtr,
                                      const _rpD3D9VertexShaderDescriptor  *desc,
                                      RwReal ambientCoef, RwReal diffuseCoef)
{
    RwV4d *basePtr;
    RwUInt32 n, numLights, lightBlock;

    RWFUNCTION(RWSTRING("_rpD3D9VertexShaderUpdateLightsColors"));

    basePtr = shaderConstantPtr;

    /* Ambient */
    shaderConstantPtr->x *= ambientCoef;
    shaderConstantPtr->y *= ambientCoef;
    shaderConstantPtr->z *= ambientCoef;
    shaderConstantPtr++;

    /* Directional lights */
    numLights = desc->numDirectionalLights;
    for (n = 0; n < numLights; n++)
    {
        /* Skip direction */
        shaderConstantPtr++;

        /* Update the light color */
        shaderConstantPtr->x *= diffuseCoef;
        shaderConstantPtr->y *= diffuseCoef;
        shaderConstantPtr->z *= diffuseCoef;
        shaderConstantPtr++;
    }

    /* Point lights */
    lightBlock = 0;
    numLights = desc->numPointLights;
    for (n = 0; n < numLights; n++)
    {
        /* Skip radious */
        if ( (lightBlock % 4) == 0 )
        {
            shaderConstantPtr++;
        }
        lightBlock++;

        /* Skip position */
        shaderConstantPtr++;

        /* Update the light color */
        shaderConstantPtr->x *= diffuseCoef;
        shaderConstantPtr->y *= diffuseCoef;
        shaderConstantPtr->z *= diffuseCoef;
        shaderConstantPtr++;
    }

    /* Spot lights */
    lightBlock = 0;
    numLights = desc->numSpotLights;
    for (n = 0; n < numLights; n++)
    {
        /* Skip radious */
        if ( (lightBlock % 4) == 0 )
        {
            shaderConstantPtr++;
        }
        lightBlock++;

        /* Skip position */
        shaderConstantPtr++;

        /* Skip direction */
        shaderConstantPtr++;

        /* Skip angle range */
        shaderConstantPtr++;

        /* Update the light color */
        shaderConstantPtr->x *= diffuseCoef;
        shaderConstantPtr->y *= diffuseCoef;
        shaderConstantPtr->z *= diffuseCoef;
        shaderConstantPtr++;
    }

    RwD3D9SetVertexShaderConstant( RWD3D9VSCONST_AMBIENT_OFFSET,
                                   basePtr,
                                   ((RwUInt32)shaderConstantPtr - (RwUInt32)basePtr) / sizeof(RwV4d) );

    RWRETURNVOID();
}

/****************************************************************************
 _rpD3D9VertexShaderUpdateMaterialColor
 */
void
_rpD3D9VertexShaderUpdateMaterialColor(const RwRGBA *color,
                                       const _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    RwV4d constantColor;

    RWFUNCTION(RWSTRING("_rpD3D9VertexShaderUpdateMaterialColor"));

    constantColor.x = (RwReal)(color->red) * COLORSCALAR;
    constantColor.y = (RwReal)(color->green) * COLORSCALAR;
    constantColor.z = (RwReal)(color->blue) * COLORSCALAR;
    constantColor.w = (RwReal)(color->alpha) * COLORSCALAR;

    RwD3D9SetVertexShaderConstant(dispatch->offsetMaterialColor,
                                  &constantColor, 1);

    RWRETURNVOID();
}

/****************************************************************************
 _rpD3D9VertexShaderUpdateFogData
 */
void
_rpD3D9VertexShaderUpdateFogData(const _rpD3D9VertexShaderDescriptor  *desc,
                                 const _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    RWFUNCTION(RWSTRING("_rpD3D9VertexShaderUpdateFogData"));

    if ((_RwD3D9DeviceCaps.RasterCaps & D3DPRASTERCAPS_WFOG) == 0)
    {
        RwV4d fogData;

        switch (desc->fogMode)
        {
            case rwD3D9VERTEXSHADERFOG_LINEAR:
                {
                    const RwCamera *camera = RwCameraGetCurrentCamera();
                    const RwReal    fogPlane  = camera->fogPlane;
                    const RwReal    farPlane  = camera->farPlane;

                    /* Fog Constant: x = -1/(fog_end-fog_start), y = fog_end/(fog_end-fog_start) */
                    fogData.x = -1.f / (farPlane - fogPlane);
                    fogData.y = farPlane / (farPlane - fogPlane);
                    fogData.z = 0.0f;
                    fogData.w = 1.0f;
                }
                break;

            case rwD3D9VERTEXSHADERFOG_EXP:
            case rwD3D9VERTEXSHADERFOG_EXP2:
                {
                    RwReal fogDensity;

                    RwD3D9GetRenderState(D3DRS_FOGDENSITY, (RwUInt32 *)(&fogDensity));

                    /* Fog Constant: x = density * log2(e) */
                    fogData.x = fogDensity * 1.4426950408889634073599246810019f;
                    fogData.y = 0.0f;
                    fogData.z = 0.0f;
                    fogData.w = 1.0f;
                }
                break;
        }

        RwD3D9SetVertexShaderConstant(dispatch->offsetFogRange,
                                      &fogData, 1);
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpD3D9VertexShaderUpdateMorphingCoef
 */
void
_rpD3D9VertexShaderUpdateMorphingCoef(RwReal morphingCoef,
                                      const _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    RwV4d morphingCoefVector;

    RWFUNCTION(RWSTRING("_rpD3D9VertexShaderUpdateMorphingCoef"));

    morphingCoefVector.x = morphingCoef;
    morphingCoefVector.y = 1.0f - morphingCoef;
    morphingCoefVector.z = 1.0f;
    morphingCoefVector.w = 0.0f;

    RwD3D9SetVertexShaderConstant(dispatch->offsetMorphingCoef,
                                  &morphingCoefVector, 1);

    RWRETURNVOID();
}

/****************************************************************************
 * _rpD3DVertexShaderSetUVAnimMatrix
 */
void
_rpD3DVertexShaderSetUVAnimMatrix(RwMatrix *matrix,
                                  const _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    RwV4d transform[2];

    RWFUNCTION(RWSTRING("_rpD3DVertexShaderSetUVAnimMatrix"));

    transform[0].x = matrix->right.x;
    transform[0].y = matrix->up.x;
    transform[0].z = matrix->at.x;
    transform[0].w = matrix->pos.x;

    transform[1].x = matrix->right.y;
    transform[1].y = matrix->up.y;
    transform[1].z = matrix->at.y;
    transform[1].w = matrix->pos.y;

    RwD3D9SetVertexShaderConstant(dispatch->offsetEffect, transform, 2);

    RWRETURNVOID();
}

/****************************************************************************
 * _rpD3D9VertexShaderSetEnvMatrix
 */
void
_rpD3D9VertexShaderSetEnvMatrix(RwFrame *frame,
                                const _rpD3D9VertexShaderDescriptor  *desc,
                                const _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    static const RwMatrix texMat =
    {
         0.5f, 0.0f, 0.0f, 0,
         0.0f,-0.5f, 0.0f, 0,
         0.0f, 0.0f, 1.0f, 0,
         0.0f, 0.0f, 0.0f, 0
    };

    RWD3D9_ALIGN16 D3DMATRIX result;
    RwUInt32 effect;

    RWFUNCTION(RWSTRING("_rpD3D9SkinVertexShaderSetEnvMatrix"));

    effect = desc->effect;

    if (effect == rwD3D9VERTEXSHADEREFFECT_ENVMAP ||
        effect == rwD3D9VERTEXSHADEREFFECT_BUMPENVMAP)
    {
        if (frame)
        {
            RwMatrix    *envMtx;
            RwMatrix    invMtx;
            RwMatrix    tmpMtx;

            /* Transfrom the normals by the inverse of the env maps frame */
            envMtx = RwFrameGetLTM(frame);

            RwMatrixInvert(&invMtx, envMtx);

            RwMatrixMultiply(&tmpMtx, &invMtx, &texMat);

            _rwD3D9VSGetWorldNormalizedMultiplyTransposeMatrix(&result, &tmpMtx);
        }
        else
        {
            _rwD3D9VSGetWorldNormalizedViewMultiplyTransposeMatrix(&result, &texMat);
        }

        /* Set right translation */
        result.m[0][3] = 0.5f;
        result.m[1][3] = 0.5f;

        /* pack relevant 2 rows into constant registers */
        if (effect == rwD3D9VERTEXSHADEREFFECT_ENVMAP)
        {
            RwD3D9SetVertexShaderConstant(dispatch->offsetEffect, &result, 2);
        }
        else
        {
            RwD3D9SetVertexShaderConstant(dispatch->offsetEffect + VSCONST_REG_BUMP_SIZE, &result, 2);
        }
    }
    else
    {
        RwCamera *camera = RwCameraGetCurrentCamera();
        RwV3d eyePosition;
        RwMatrix *camMtx;

        camMtx = RwFrameGetLTM(RwCameraGetFrame(camera));

        _rwD3D9VSGetPointInLocalSpace(&(camMtx->pos), &eyePosition);

        _rwD3D9VSGetWorldNormalizedTransposeMatrix(&result);

        /* pack relevant 3 rows into constant registers */
        if (effect == rwD3D9VERTEXSHADEREFFECT_CUBEMAP)
        {
            RwD3D9SetVertexShaderConstant(dispatch->offsetEffect, &eyePosition, 1);

            RwD3D9SetVertexShaderConstant(dispatch->offsetEffect + 1, &result, 3);
        }
        else
        {
            RwD3D9SetVertexShaderConstant(dispatch->offsetEffect + VSCONST_REG_BUMP_SIZE, &eyePosition, 1);

            RwD3D9SetVertexShaderConstant(dispatch->offsetEffect + 1 + VSCONST_REG_BUMP_SIZE, &result, 3);
        }
    }

    RWRETURNVOID();
}

/****************************************************************************
 * _rpD3D9VertexShaderSetBumpMatrix
 */
void
_rpD3D9VertexShaderSetBumpMatrix(RwFrame *frame,
                                 RwReal factor,
                                 const _rpD3D9VertexShaderDispatchDescriptor *dispatch)
{
    RwMatrix *matrix;
    RwMatrix invMtx;

    RWD3D9_ALIGN16 D3DMATRIX result;

    RWFUNCTION(RWSTRING("_rpD3D9VertexShaderSetBumpMatrix"));

    if (frame == NULL)
    {
        frame = RwCameraGetFrame(RwCameraGetCurrentCamera());
        RWASSERT(frame);
    }

    matrix = RwFrameGetLTM(frame);

    RwMatrixInvert(&invMtx, matrix);

    _rwD3D9VSGetWorldMultiplyTransposeMatrix(&result, &invMtx);

    /* Apply factor */
    result.m[0][0] *= factor;
    result.m[0][1] *= factor;
    result.m[0][2] *= factor;
    result.m[0][3] = 0.0f;

    result.m[1][0] *= factor;
    result.m[1][1] *= factor;
    result.m[1][2] *= factor;
    result.m[1][3] = 0.0f;

    RwD3D9SetVertexShaderConstant(dispatch->offsetEffect, &result, 2);

    RWRETURNVOID();
}
