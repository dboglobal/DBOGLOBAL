/*
 * Structure for string ids
 * 
 * 
 * Copyright (c) Criterion Software Limited
 */

/******************************************/
/*                                        */
/*    RenderWare(TM) Graphics Library     */
/*                                        */
/******************************************/

/*
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 1998. Criterion Software Ltd.
 * All Rights Reserved.
 */

/***************************************************************************
 *                                                                         *
 * Module  :    rpid.h                                                     *
 *                                                                         *
 * Purpose :    Id operations                                              *
 *                                                                         *
 **************************************************************************/

#ifndef RPID_H
#define RPID_H

/**
 * \defgroup rpid RpId
 * \ingroup scenemanagement
 *
 * Id Plugin for RenderWare Graphics.
 */

/****************************************************************************
 Includes
 */
#include <rpid.rpe>

/****************************************************************************
 Global Types
 */

/**
 * \ingroup rpid
 * \ref RpId
 * typedef for RpId.
 */
typedef RwUInt32    RpId;

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

/****************************************************************************
 Global varibales
 */
extern const RpId rpIdNull;

/****************************************************************************
 Function prototypes
 */
extern RwBool RpIdPluginAttach(void);

extern RpId
RpIdGenerateFromString(const RwChar *string);

extern RpGeometry *
RpGeometrySetId(RpGeometry *geometry, RpId id);

extern RpWorldSector *
RpWorldSectorSetId(RpWorldSector *ws, RpId id);

extern RwFrame *
RwFrameSetId(RwFrame *frame, RpId id);

extern RwCamera *
RwCameraSetId(RwCamera *camera, RpId id);

extern RpLight *
RpLightSetId(RpLight *light, RpId id);

extern RpMaterial *
RpMaterialSetId(RpMaterial *material, RpId id);

extern RwTexture *
RwTextureSetId(RwTexture *texture, RpId id);

extern RpId
RpGeometryGetId(RpGeometry *geometry);

extern RpId
RpWorldSectorGetId(RpWorldSector *ws);

extern RpId
RwFrameGetId(RwFrame *frame);

extern RpId
RwCameraGetId(RwCamera *camera);

extern RpId
RpLightGetId(RpLight *light);

extern RpId
RpMaterialGetId(RpMaterial *material);

extern RpId
RwTextureGetId(RwTexture *texture);

#ifdef    __cplusplus
}
#endif /* __cplusplus */


#endif /* RPSTRID_H */