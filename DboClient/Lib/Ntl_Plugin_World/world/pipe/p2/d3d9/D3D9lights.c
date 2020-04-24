/****************************************************************************
 *                                                                          *
 * module : D3D9lights.c                                                    *
 *                                                                          *
 * purpose:                                                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#include <d3d9.h>

#include "rwcore.h"
#include "baworld.h"

#include "D3D9lights.h"

/****************************************************************************
 Global variables
 */

/****************************************************************************
 local defines
 */

#define D3D9LIGHTDATAFROMLIGHT(light)                          \
    (rpD3D9LightData*)(((RwInt8*)light) + D3D9LightOffset)

#define D3D9LIGHTDATAFROMCONSTLIGHT(light)                      \
    (const rpD3D9LightData*)(((const RwInt8*)light) + D3D9LightOffset)

/****************************************************************************
 local types
 */

typedef struct RxD3D9WorldLight RxD3D9WorldLight;
struct RxD3D9WorldLight
{
    D3DLIGHT9   light;      /* Light config */
    RwBool      enabled;    /* Enabled in the D3D9 lights cache */
    RwUInt32    index;      /* Index in the D3D9 lights cache */
};

typedef struct rpD3D9LightData rpD3D9LightData;
struct rpD3D9LightData
{
    RwInt32                 index;          /* D3D9 light index, -1 if not set before */
    RpD3D9AttenuationParams attenuation;    /* Attenuation params */
};

/****************************************************************************
 local variables
 */
static RwInt32          D3D9LightOffset = 0;

static RwUInt32         VideoCardMaxActiveLights = 0;

static RwInt32          NumLightIndexFree = 0;
static RwInt32          MaxLightIndexFree = 0;
static RwInt32          MaxLightIndex = 0;
static RwInt32          *LightIndexFree = NULL;

static RwUInt32         *LightsToEnable = NULL;
static RwUInt32         NumLightsToEnable = 0;
static RwUInt32         MaxLightsToEnable = 0;

static RwUInt32         *LightsToDisable = NULL;
static RwUInt32         NumLightsToDisable = 0;
static RwUInt32         MaxLightsToDisable = 0;

static D3DLIGHT9        DirectionalLight;
static D3DLIGHT9        PointLight;

/****************************************************************************
 Functions
 */

static void *
_rpD3D9LightConstructor(void *object,
                        RwInt32 offsetInObject __RWUNUSED__,
                        RwInt32 sizeInObject __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rpD3D9LightConstructor"));
    RWASSERT(object);

    if (object)
    {
        rpD3D9LightData    *lightData;

        lightData = D3D9LIGHTDATAFROMLIGHT((RpLight *) object);

        /* No index yet */
        lightData->index = -1;

        /* No attenuation */
        lightData->attenuation.constant= 1.f;
        lightData->attenuation.linear = 0.f;
        lightData->attenuation.quadratic = 5.f;

        RWRETURN(object);
    }

    RWRETURN(NULL);
}

static void *
_rpD3D9LightDestructor(void *object,
                       RwInt32 offsetInObject __RWUNUSED__,
                       RwInt32 sizeInObject __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rpD3D9LightDestructor"));
    RWASSERT(object);

    if (object)
    {
        rpD3D9LightData    *lightData;

        lightData = D3D9LIGHTDATAFROMLIGHT((RpLight *) object);

        if (lightData->index >= 0)
        {
            RWASSERT(lightData->index < MaxLightIndex);

            if (NumLightIndexFree >= (MaxLightIndex - 1))
            {
                /* All the lights have been deallocated */
                NumLightIndexFree = 0;
                MaxLightIndexFree = 0;
                MaxLightIndex = 0;
                if (LightIndexFree != NULL)
                {
                    RwFree(LightIndexFree);
                    LightIndexFree = NULL;
                }
            }
            else
            {
                if (NumLightIndexFree >= MaxLightIndexFree)
                {
                    MaxLightIndexFree += 256;

                    if (LightIndexFree == NULL)
                    {
                        LightIndexFree = (RwInt32 *)
                                        RwMalloc(MaxLightIndexFree * sizeof(RwInt32),
                                                 rwMEMHINTDUR_EVENT    |
                                                 rwID_WORLDPIPEMODULE  |
                                                 rwMEMHINTFLAG_RESIZABLE);
                    }
                    else
                    {
                        LightIndexFree = (RwInt32 *)
                                            RwRealloc(LightIndexFree,
                                                    MaxLightIndexFree * sizeof(RwInt32),
                                                    rwMEMHINTDUR_EVENT    |
                                                    rwID_WORLDPIPEMODULE  |
                                                    rwMEMHINTFLAG_RESIZABLE);
                    }
                }

                RWASSERT(LightIndexFree != NULL);
                LightIndexFree[NumLightIndexFree++] = (RwInt32)lightData->index;
            }
        }
    }

    RWRETURN(object);
}

static void *
_rpD3D9LightCopy(void *dstObject,
                 const void *srcObject,
                 RwInt32 offsetInObject __RWUNUSED__,
                 RwInt32 sizeInObject __RWUNUSED__)
{

    RWFUNCTION(RWSTRING("_rpD3D9LightCopy"));

    if (srcObject && dstObject)
    {
        const rpD3D9LightData *srcLightData =
                    D3D9LIGHTDATAFROMCONSTLIGHT((const RpLight *)srcObject);

        rpD3D9LightData *dstLightData =
                    D3D9LIGHTDATAFROMLIGHT((RpLight *)dstObject);

        *dstLightData = *srcLightData;
    }

    RWRETURN(dstObject);
}

/****************************************************************************
 _rpD3D9LightPluginAttach

 Purpose:   aAttach lights plugin

*/
RwBool
_rpD3D9LightPluginAttach(void)
{
    RWFUNCTION(RWSTRING("_rpD3D9LightPluginAttach"));

    D3D9LightOffset =
        RpLightRegisterPlugin(sizeof(rpD3D9LightData),
                              rwID_LIGHTMODULE,
                              _rpD3D9LightConstructor,
                              _rpD3D9LightDestructor,
                              _rpD3D9LightCopy);

    if (D3D9LightOffset < 0)
    {
        RWRETURN(FALSE);
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rplightd3d9
 * \ref RpD3D9LightSetAttenuationParams is used to set the
 * attenuation model of the supplied spot or point light.  The default model
 * specifies constant, linear and quadratic attenuation coefficients to be
 * 1, 0, 5 respectively. The attenuation parameters are used as follows:
 *
 * \verbatim

                     1
    -----------------------------------
          L            Q
     C + --- * D + --------- * (D * D)
          R         (R * R)
   \endverbatim
 *
 * where \e D is the distance to the vertex from the light source, R is the
 * radius of the light and C, L and Q are the constant, linear and quadratic
 * attenuation parameters, respectively.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light whose attenuation parameters are going
 * to be changed.
 * \param params Constant pointer to the RpD3D9AttenuationParams.
 *
 * \see RpD3D9LightGetAttenuationParams
 *
 */
void
RpD3D9LightSetAttenuationParams(RpLight *light,
                                const RpD3D9AttenuationParams *params)
{
    rpD3D9LightData     *lightData;

    RWAPIFUNCTION(RWSTRING("RpD3D9LightSetAttenuationParams"));
    RWASSERT(light && params);

    lightData = D3D9LIGHTDATAFROMLIGHT(light);

    lightData->attenuation = *params;

    RWRETURNVOID();
}

/**
 * \ingroup rplightd3d9
 * \ref RpD3D9LightGetAttenuationParams is used to retrieve
 * the attenuation parameters of the specified light.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Constant pointer to the light.
 * \param params Pointer to a RpD3D9AttenuationParams structure that is filled
 * with the retrieved attenuation parameters of the light.
 *
 * \see RpD3D9LightSetAttenuationParams
 *
 */
void
RpD3D9LightGetAttenuationParams(const RpLight *light,
                                RpD3D9AttenuationParams *params)
{
    const rpD3D9LightData     *lightData;

    RWAPIFUNCTION(RWSTRING("RpD3D9LightGetAttenuationParams"));
    RWASSERT(light && params);

    lightData = D3D9LIGHTDATAFROMCONSTLIGHT(light);

    *params = lightData->attenuation;

    RWRETURNVOID();
}


/****************************************************************************
 _rwD3D9LightsOpen

 Purpose:   open lights cache

*/
RwBool
_rwD3D9LightsOpen(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9LightsOpen"));

    _rwD3D9LightsClose();

    VideoCardMaxActiveLights = ((const D3DCAPS9 *)RwD3D9GetCaps())->MaxActiveLights;

    if (VideoCardMaxActiveLights == 0)
    {
        VideoCardMaxActiveLights = 0xffffffff;
    }

    AmbientSaturated.red = 0;
    AmbientSaturated.green = 0;
    AmbientSaturated.blue = 0;
    AmbientSaturated.alpha = 1.f;

    /* Init static light structures with default values*/
    DirectionalLight.Type = D3DLIGHT_DIRECTIONAL;
    DirectionalLight.Diffuse.r = 0.f;
    DirectionalLight.Diffuse.g = 0.f;
    DirectionalLight.Diffuse.b = 0.f;
    DirectionalLight.Diffuse.a = 1.0f;
    DirectionalLight.Direction.x = 0.f;
    DirectionalLight.Direction.y = 0.f;
    DirectionalLight.Direction.z = 0.f;
    DirectionalLight.Specular.r = 0.0f;
    DirectionalLight.Specular.g = 0.0f;
    DirectionalLight.Specular.b = 0.0f;
    DirectionalLight.Specular.a = 1.0f;
    DirectionalLight.Ambient.r = 0.0f;
    DirectionalLight.Ambient.g = 0.0f;
    DirectionalLight.Ambient.b = 0.0f;
    DirectionalLight.Ambient.a = 1.0f;
    DirectionalLight.Position.x = 0.0f;
    DirectionalLight.Position.y = 0.0f;
    DirectionalLight.Position.z = 0.0f;
    DirectionalLight.Range = 0.0f;
    DirectionalLight.Falloff = 0.0f;
    DirectionalLight.Attenuation0 = 0.0f;
    DirectionalLight.Attenuation1 = 0.0f;
    DirectionalLight.Attenuation2 = 0.0f;
    DirectionalLight.Theta = 0.0f;
    DirectionalLight.Phi = 0.0f;

    PointLight.Diffuse.r = 0.0f;
    PointLight.Diffuse.g = 0.0f;
    PointLight.Diffuse.b = 0.0f;
    PointLight.Diffuse.a = 1.0f;
    PointLight.Specular.r = 0.0f;
    PointLight.Specular.g = 0.0f;
    PointLight.Specular.b = 0.0f;
    PointLight.Specular.a = 1.0f;
    PointLight.Ambient.r = 0.0f;
    PointLight.Ambient.g = 0.0f;
    PointLight.Ambient.b = 0.0f;
    PointLight.Ambient.a = 1.0f;
    PointLight.Falloff = 1.0f;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9LightsClose

 Purpose:   close lights cache

*/
void
_rwD3D9LightsClose(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9LightsClose"));

    if (LightIndexFree != NULL)
    {
        RwFree(LightIndexFree);
        LightIndexFree = NULL;
    }

    NumLightIndexFree = 0;
    MaxLightIndexFree = 0;
    MaxLightIndex = 0;

    if (LightsToEnable != NULL)
    {
        RwFree(LightsToEnable);
        LightsToEnable = NULL;
    }

    NumLightsToEnable = 0;
    MaxLightsToEnable = 0;

    if (LightsToDisable != NULL)
    {
        RwFree(LightsToDisable);
        LightsToDisable = NULL;
    }

    NumLightsToDisable = 0;
    MaxLightsToDisable = 0;

    RWRETURNVOID();
}

/****************************************************************************
 D3D9CreateLight

 Purpose:   find a free light index

*/
static void
D3D9CreateLight(rpD3D9LightData *lightData)
{
    RWFUNCTION(RWSTRING("D3D9CreateLight"));

    if (NumLightIndexFree)
    {
        NumLightIndexFree--;

        RWASSERT(LightIndexFree != NULL);
        lightData->index = LightIndexFree[NumLightIndexFree];
    }
    else
    {
        lightData->index = MaxLightIndex;

        MaxLightIndex ++;
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D9AddEnabledLight

 Purpose:   close lights cache

*/
static void
D3D9AddEnabledLight(RwUInt32 index)
{
    RWFUNCTION(RWSTRING("D3D9AddEnabledLight"));

    if (NumLightsToEnable >= MaxLightsToEnable)
    {
        RwUInt32 n;

        n = MaxLightsToEnable;

        MaxLightsToEnable += 8;

        if (LightsToEnable == NULL)
        {
            LightsToEnable = (RwUInt32 *)RwMalloc(
                                        MaxLightsToEnable * sizeof(RwUInt32),
                                        rwMEMHINTDUR_EVENT    |
                                        rwID_WORLDPIPEMODULE  |
                                        rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            LightsToEnable = (RwUInt32 *)RwRealloc(LightsToEnable,
                                        MaxLightsToEnable * sizeof(RwUInt32),
                                        rwMEMHINTDUR_EVENT    |
                                        rwID_WORLDPIPEMODULE  |
                                        rwMEMHINTFLAG_RESIZABLE);
        }

        for (; n < MaxLightsToEnable; n++)
        {
            LightsToEnable[n] = 0;
        }
    }

    LightsToEnable[NumLightsToEnable] = index;
    NumLightsToEnable++;

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9LightsGlobalEnable

 Purpose:   Enable global lights, (Directional & Ambient).

 On entry:  flags - Flags to say if the light is supposed to light geometry
            or world sectors.

 On exit :
*/
RwBool
_rwD3D9LightsGlobalEnable(RpLightFlag flags)
{
    RpWorld         *world;
    RwLLLink        *cur, *end;
    RwBool          lighting = FALSE;

    RWFUNCTION(RWSTRING("_rwD3D9LightsGlobalEnable"));

    AmbientSaturated.red = 0.f;
    AmbientSaturated.green = 0.f;
    AmbientSaturated.blue = 0.f;
    AmbientSaturated.alpha = 1.f;

    world = (RpWorld *)RWSRCGLOBAL(curWorld);

    cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
    end = rwLinkListGetTerminator(&world->directionalLightList);
    while (cur != end)
    {
        RpLight *light;

        light = rwLLLinkGetData(cur, RpLight, inWorld);

        /* NB light may actually be a dummyTie from a enclosing ForAll */
        if (light && (rwObjectTestFlags(light, flags)))
        {
            if (RpLightGetType(light) == rpLIGHTDIRECTIONAL)
            {
                _rwD3D9LightDirectionalEnable(light);

                lighting = TRUE;
            }
            else
            {
                const RwRGBAReal    *color;

                color = RpLightGetColor(light);

                /* Accumulate ambient light color */
                AmbientSaturated.red += color->red;
                AmbientSaturated.green += color->green;
                AmbientSaturated.blue += color->blue;

                lighting = TRUE;
            }
        }

        /* Next */
        cur = rwLLLinkGetNext(cur);
    }

    /* Scale, clamp and then set the ambient color */
    if (lighting == FALSE)
    {
        RwD3D9SetRenderState(D3DRS_AMBIENT, 0);
    }

    RWRETURN(lighting);
}

/****************************************************************************
 _rwD3D9LightDirectionalEnable

 Purpose:   Enable a directional light.

 On entry:  light - Pointer to the light to add.

 On exit :  TRUE if a light has been set else FALSE.
*/
RwBool
_rwD3D9LightDirectionalEnable(RpLight *light)
{
    const RwRGBAReal    *color;
    const RwV3d         *at;
    rpD3D9LightData     *lightData;

    RWFUNCTION(RWSTRING("_rwD3D9LightDirectionalEnable"));

    /* Check light index */
    lightData = D3D9LIGHTDATAFROMLIGHT(light);
    if (lightData->index < 0)
    {
        D3D9CreateLight(lightData);
    }

    /* Set the light color */
    color = RpLightGetColor(light);

    DirectionalLight.Diffuse.r = color->red;
    DirectionalLight.Diffuse.g = color->green;
    DirectionalLight.Diffuse.b = color->blue;

    /* Set the light direction */
    at = RwMatrixGetAt(RwFrameGetLTM(RpLightGetFrame(light)));

    DirectionalLight.Direction.x = at->x;
    DirectionalLight.Direction.y = at->y;
    DirectionalLight.Direction.z = at->z;

    RwD3D9SetLight(lightData->index, &DirectionalLight);

    D3D9AddEnabledLight(lightData->index);

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwD3D9LightLocalEnable

 Purpose:   Enable local light, (Point, Spot or Spot soft).

 On entry:  light - Pointer to the light to add.

 On exit :  TRUE if a light has been set else FALSE.
*/
RwBool
_rwD3D9LightLocalEnable(RpLight *light)
{
    const RwRGBAReal    *color;
    const RwMatrix      *matrixLight;
    const RwV3d         *atPos;
    RwReal              radius;
    RpLightType         type;
    rpD3D9LightData     *lightData;

    RWFUNCTION(RWSTRING("_rwD3D9LightLocalEnable"));

    /* Check light index */
    lightData = D3D9LIGHTDATAFROMLIGHT(light);
    if (lightData->index < 0)
    {
        D3D9CreateLight(lightData);
    }

    /* Set the lights color */
    color = RpLightGetColor(light);

    PointLight.Diffuse.r = color->red;
    PointLight.Diffuse.g = color->green;
    PointLight.Diffuse.b = color->blue;

    /* Set the lights position */
    matrixLight = RwFrameGetLTM(RpLightGetFrame(light));

    atPos = &(matrixLight->pos);

    PointLight.Position.x = atPos->x;
    PointLight.Position.y = atPos->y;
    PointLight.Position.z = atPos->z;

    /* Range */
    radius = RpLightGetRadius(light);
    PointLight.Range = radius;

    if (radius != 0.f)
    {
        /* Attenuation */
        PointLight.Attenuation0 = lightData->attenuation.constant;
        PointLight.Attenuation1 = lightData->attenuation.linear / radius;
        PointLight.Attenuation2 = lightData->attenuation.quadratic / (radius*radius);

        /* Setup light */
        type = RpLightGetType(light);
        if (type == rpLIGHTPOINT)
        {
            PointLight.Type = D3DLIGHT_POINT;

            /* Light direction is ignored */
            PointLight.Direction.x = 0.f;
            PointLight.Direction.y = 0.f;
            PointLight.Direction.z = 0.f;

            /* Angles ignored */
            PointLight.Theta = 0.0f;
            PointLight.Phi = 0.0f;
        }
        else if (type == rpLIGHTSPOT)
        {
            RwReal  angle;

            PointLight.Type = D3DLIGHT_SPOT;

            /* Set the lights direction */
            atPos = &(matrixLight->at);

            PointLight.Direction.x = atPos->x;
            PointLight.Direction.y = atPos->y;
            PointLight.Direction.z = atPos->z;

            /*
             * According to the D3D9 documentation, the cone angle
             * cannot be outside the range 0...Pi
             * But it does seem to work when this _isn't_ the case
             * But we should be cautious here in case it fails somewhere
             *
             * Also, if the angle is larger than Pi, then Light.Theta
             * and Light.Phi cannot be the same... hence the delta offset
             * in the else-branch below.
             */
            angle = RpLightGetConeAngle(light) * 2.0f;

            if(angle<rwPI)
            {
                PointLight.Theta = angle;
            }
            else
            {
                PointLight.Theta = angle-0.001f;
            }

            PointLight.Phi = angle;
        }
        else if (type == rpLIGHTSPOTSOFT)
        {
            PointLight.Type = D3DLIGHT_SPOT;

            /* Set the lights direction */
            atPos = &(matrixLight->at);

            PointLight.Direction.x = atPos->x;
            PointLight.Direction.y = atPos->y;
            PointLight.Direction.z = atPos->z;

            PointLight.Theta = 0.0f;
            PointLight.Phi = RpLightGetConeAngle(light) * 2.0f;
        }
        else
        {
            /* If we've got one of these light types here, were in trouble:
                rpLIGHTDIRECTIONAL
                rpLIGHTAMBIENT
                rpNALIGHTTYPE
            */
            RWRETURN(FALSE);
        }

        RwD3D9SetLight(lightData->index, &PointLight);

        D3D9AddEnabledLight(lightData->index);

        RWRETURN(TRUE);
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwD3D9LightsEnable

 Purpose:   Disable all unwanted lights. If more lights than required are
            currently active they will be disabled.  If no lights are
            required then lighting will be disabled.

 on entry:  enable - TRUE to enable lighting else FALSE.
            type - rpATOMIC or rwSECTORATOMIC (debug purposes)
 on exit :  TRUE if all the active lights were enabled
            FALSE if due to any hardware limit not all the lights were enabled
*/
void
_rwD3D9LightsEnable(RwBool enable, RwUInt32 type)
{
    RWFUNCTION(RWSTRING("_rwD3D9LightsEnable"));

    if (enable)
    {
        RwUInt32    n, c;

        /* Clamp number of enabled lights */
        if (NumLightsToEnable > VideoCardMaxActiveLights)
        {
            #if defined(RWDEBUG)
            RwChar              buffer[256];

            if (type == rpATOMIC)
            {
                rwsprintf(buffer,
                          "There are %d lights attached to this atomic."\
                          "This video card only supports %d.",
                          NumLightsToEnable, VideoCardMaxActiveLights);
            }
            else
            {
                rwsprintf(buffer,
                          "There are %d lights attached to this world sector."\
                          "This video card only supports %d.",
                          NumLightsToEnable, VideoCardMaxActiveLights);
            }

            RwDebugSendMessage(rwDEBUGMESSAGE,
                               "_rwD3D9LightsEnable",
                               buffer);
            #else
            type = type;
            #endif

            NumLightsToEnable = VideoCardMaxActiveLights;
        }
        else
        {

            #if defined(RWDEBUG)
            if (NumLightsToEnable > 8)
            {
                RwDebugSendMessage(rwDEBUGMESSAGE,
                    "_rwD3D9LightsEnable",
                    "Setting more than 8 lights is going to degrade performance seriously.");
            }
            #endif
        }

        /* Disable previous active lights */
        if (NumLightsToDisable)
        {
            n = 0;
            do
            {
                const RwUInt32 index = LightsToDisable[n];

                /* Check if this light is going to be activated again */
                for (c = 0; c < NumLightsToEnable; c++)
                {
                    if (LightsToEnable[c] == index)
                    {
                        break;
                    }
                }

                if (c >= NumLightsToEnable)
                {
                    RwD3D9EnableLight(index, FALSE);
                }

                n++;
            }
            while (n < NumLightsToDisable);
        }

        if (NumLightsToEnable)
        {
            RwUInt32 *pointerAux;

            for (n = 0; n < NumLightsToEnable; n++)
            {
                RwD3D9EnableLight(LightsToEnable[n], TRUE);
            }

            /* swap pointers */
            pointerAux = LightsToDisable;
            LightsToDisable = LightsToEnable;
            LightsToEnable = pointerAux;

            n = MaxLightsToDisable;
            MaxLightsToDisable = MaxLightsToEnable;
            MaxLightsToEnable = n;

            NumLightsToDisable = NumLightsToEnable;
            NumLightsToEnable = 0;
        }
        else
        {
            NumLightsToDisable = 0;
            NumLightsToEnable = 0;
        }

        RwD3D9SetRenderState(D3DRS_LIGHTING, TRUE);
    }
    else
    {
        RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);
    }

    RWRETURNVOID();
}
