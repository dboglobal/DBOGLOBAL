/***************************************************************************
 *                                                                         *
 * Module  : D3D8lights.h                                                    *
 *                                                                         *
 * Purpose : See D3D8lights.c                                                *
 *                                                                         *
 **************************************************************************/

#ifndef D3D8LIGHTS_H
#define D3D8LIGHTS_H

/****************************************************************************
 Includes
 */

#include <d3d8.h>

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
 * typedef for struct RpD3D8AttenuationParams
 */
typedef struct RpD3D8AttenuationParams RpD3D8AttenuationParams;
/**
 * \ingroup rplightd3d8
 * \struct RpD3D8AttenuationParams
 *  This type represents the attenuation model of a spot or point light.
 */
struct RpD3D8AttenuationParams
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
_rpD3D8LightPluginAttach(void);

/* RWPUBLIC */
extern void
RpD3D8LightSetAttenuationParams(RpLight *light,
                                const RpD3D8AttenuationParams *params);

extern void
RpD3D8LightGetAttenuationParams(const RpLight *light,
                                RpD3D8AttenuationParams *params);

extern RwBool
_rwD3D8LightsOpen(void);

extern RwBool
_rwD3D8LightsGlobalEnable(RpLightFlag flags);

extern RwBool
_rwD3D8LightDirectionalEnable(RpLight *light);

extern RwBool
_rwD3D8LightLocalEnable(RpLight *light);

extern void
_rwD3D8LightsEnable(RwBool enable, RwUInt32 type);

extern void
_rwD3D8LightsClose(void);

/* RWPUBLICEND */

extern RwRGBAReal   AmbientSaturated;


/* RWPUBLIC */

#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#endif /* D3D8LIGHTS_H */
