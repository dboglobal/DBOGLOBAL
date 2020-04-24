/***************************************************************************
 *                                                                         *
 * Module  : D3D9lights.h                                                    *
 *                                                                         *
 * Purpose : See D3D9lights.c                                                *
 *                                                                         *
 **************************************************************************/

#ifndef D3D9LIGHTS_H
#define D3D9LIGHTS_H

/****************************************************************************
 Includes
 */

#include <d3d9.h>

#include "rwcore.h"

#include "balight.h"

/****************************************************************************
 Global Defines
 */

/****************************************************************************
 Global Types
 */

/* RWPUBLIC */
/*
 * typedef for struct RpD3D9AttenuationParams
 */
typedef struct RpD3D9AttenuationParams RpD3D9AttenuationParams;
/**
 * \ingroup rplightd3d9
 * \struct RpD3D9AttenuationParams
 *  This type represents the attenuation model of a spot or point light.
 */
struct RpD3D9AttenuationParams
{
    RwReal constant;    /**< Constant attenuation coefficient */
    RwReal linear;      /**< Linear attenuation coefficient */
    RwReal quadratic;   /**< Quadratic attenuation coefficient */
};
/* RWPUBLICEND */

/****************************************************************************
 Function prototypes
 */

/* RWPUBLIC */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* RWPUBLICEND */

/* 
 * We don't expose the plugin attach.  It's called internally by the world 
 * module.
 */
extern RwBool     
_rpD3D9LightPluginAttach(void);

/* RWPUBLIC */
extern void
RpD3D9LightSetAttenuationParams(RpLight *light,
                                const RpD3D9AttenuationParams *params);

extern void
RpD3D9LightGetAttenuationParams(const RpLight *light,
                                RpD3D9AttenuationParams *params);

extern RwBool
_rwD3D9LightsOpen(void);

extern RwBool
_rwD3D9LightsGlobalEnable(RpLightFlag flags);

extern RwBool
_rwD3D9LightDirectionalEnable(RpLight *light);

extern RwBool
_rwD3D9LightLocalEnable(RpLight *light);

extern void
_rwD3D9LightsEnable(RwBool enable, RwUInt32 type);

extern void
_rwD3D9LightsClose(void);

/* RWPUBLICEND */

extern RwRGBAReal   AmbientSaturated;


/* RWPUBLIC */

#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#endif /* D3D9LIGHTS_H */
