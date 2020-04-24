/*****************************************************************************
 *
 * File			: toonmultitex.h
 * Author		: HongHoDong
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 12	
 * Abstract		: NTl Application
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef RPTOONMATFX_MULTITEX_H
#define RPTOONMATFX_MULTITEX_H

#include "rwcore.h"
#include "rpworld.h"


/* RWPUBLIC */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RwInt32 RpToonMultiTexPluginAttach(void);

extern RwBool RpToonMultiTexSetTexture(RpMaterial *pMaterial, RwTexture *pTexture);
extern RwTexture* RpToonMultiTexGetTexture(RpMaterial *pMaterial);

extern void RpToonMaterialSetColor(RpMaterial *pMaterial, RwRGBAReal *pRGBA);
extern RwRGBAReal *RpToonMaterialGetColor(RpMaterial *pMaterial);

extern RpAtomicCallBackRender RpNtlToonNotEdgeTwoSideCallBack;			
extern RpAtomicCallBackRender RpNtlToonEdgeTwoSideCallBack;
extern RpAtomicCallBackRender RpNtlToonDefaultCallBack;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RPMATFX_MULTITEX_H */

/* RWPUBLICEND */
