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
#ifndef RPNTL_MAT_FX_H
#define RPNTL_MAT_FX_H

#include "rwcore.h"
#include "rpworld.h"
#include "d3d9.h"


enum RpNtlMatFlag
{
	rpNTL_MATERIAL_INVALID			= 0x00000000,
	rpNTL_MATERIAL_SKIN_COLOR		= 0x00000001,		/**<Material Color SkinColor */
	rpNTL_MATERIAL_HEAD_COLOR		= 0x00000002,		/**<Material Color HeadColor */
	rpNTL_MATERIAL_ADD_COLOR		= 0x00000004,       /**<NtlMaterialExt AddColor */
	rpNTL_MATERIAL_BELT_COLOR		= 0x00000008,       /** DOGI belt color */
	rpNTL_MATERIAL_UV_MATRIX		= 0x00000010,
	rpNTL_MATERIAL_ENV_MAP			= 0x00000020,
	rpNTL_MATERIAL_EMUV				= 0x00000040,
	rpNTL_MATERIAL_MIXED			= 0x00000080,
	rpNTL_MATERIAL_EMBLEM			= 0x00000100,
	rpNTL_MATERIAL_SIMPLEMATERIAL	= 0x00000200,
	rpNTL_MATERIAL_DXT1_ALPHA_CHECK	= 0x00000400,
	rpNTL_MATERIAL_DOGI_PANTS		= 0x00000800,
	rpNTL_MATERIAL_PETRIFY			= 0x00001000,
	rpNTL_MATERIAL_DOGI_SKIRT		= 0x00002000,
	rpNTL_MATERIAL_COSTUME_EMBLEM	= 0x00004000,
};

#define rwID_NTL_MATERIAL_EXT (0xfc)

/* RWPUBLIC */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RwChar g_ToonErrStr[2][64];

extern RwInt32 RpNtlMatFxPluginAttach(void);

/*
 Item upgrade effect plugin interfaces 
extern void				RpNtlMaterialExtSetCurUpgrade(RpMaterial *pMaterial, unsigned char* _CurGrade);
extern void				RpNtlMaterialExtSetCurUVAniTime(RpMaterial *pMaterial, RwReal* _CurUVAniTime);
extern unsigned char	RpNtlMaterialExtGetCurUpgrade(RpMaterial *pMaterial);
extern RwReal			RpNtlMaterialExtGetCurUVAniTime(RpMaterial *pMaterial);
*/

/*
extern RpWorld     *RpWorldForAllClumps(RpWorld * world,
										RpClumpCallBack fpCallBack,
										void *pData);
*/

struct RpToonPaint;

typedef struct MatExtInfoTag sMatExtInfo;

struct MatExtInfoTag
{
	RpMaterial*					pMaterial;
	const RxD3D9ResEntryHeader* resEntryHeader;
	const RxD3D9InstanceData*	mesh;
	RwUInt32					flags;
	RwTexture*					pToonTex;
	RwTexture*					pEmblemTex;
	D3DMATRIX*					pCamSpaceLightMatrix;
	RwRGBA*						pSkinColor;
	RwRGBA*						pDogiColor;
};

typedef void (*RpMeshRenderCB)(void* _pParam);

extern void RpNtlMaterialExecuteRenderCB(void* _pParam);
extern void RpNtlMaterialSetRenderCB(RpMaterial *pMaterial, RpMeshRenderCB _fpCB);

extern RwBool RpNtlMaterialExtSetTexture(RpMaterial *pMaterial, RwTexture *pTexture);
extern RwTexture *RpNtlMaterialExtGetTexture(RpMaterial *pMaterial);

extern RwBool RpNtlMaterialExtSetEmblemTex(RpMaterial* pMaterial, RwTexture* pTexture);
extern RwTexture* RpNtlMaterialExtGetEmblemTex(RpMaterial* pMaterial);

extern RwBool RpNtlMaterialExtSetUVTexture(RpMaterial *pMaterial, RwTexture *pTexture);
extern RwTexture *RpNtlMaterialExtGetUVTexture(RpMaterial *pMaterial);
extern void RpNtlMaterialExtSetUVTransform(RpMaterial *pMaterial, RwMatrix *pMatrix);
extern RwMatrix *RpNtlMaterialExtGetUVTransform(RpMaterial *pMaterial);
extern void RpNtlMaterialExtApplyTextureMatrix(RwUInt32 stage, RwMatrix *pMatrix);

extern RwBool RpNtlMaterialExtSetEnvMapTexture(RpMaterial *pMaterial, RwTexture *pTexture);
extern RwTexture *RpNtlMaterialExtGetEnvMapTexture(RpMaterial *pMaterial);

extern RwUInt32 RpNtlMaterialExtGetFlag(const RpMaterial *pMaterial);
extern void RpNtlMaterialExtSetFlag(RpMaterial *pMaterial, RwUInt32 uiFlag);

extern const RwRGBA *RpNtlMaterialExtGetAddColor(const RpMaterial *pMaterial);
extern void	RpNtlMaterialExtSetAddColor(RpMaterial *pMaterial, const RwRGBA *pColor);

extern RwRGBA *RpNtlMaterialExtGetSkinColor(RpMaterial *pMaterial);
extern void	RpNtlMaterialExtSetSkinColor(RpMaterial *pMaterial, RwRGBA *pColor);

extern RwRGBA *RpNtlMaterialExtGetDogiColor(RpMaterial *pMaterial);
extern void RpNtlMaterialExtSetDogiColor(RpMaterial *pMaterial, RwRGBA *pColor);

extern RwBool RpNtlMatFxPipelineCreate(void);
extern RwBool RpNtlMatFxPipelineDestroy(void);

extern RpAtomicCallBackRender RpNtlToonNotEdgeTwoSideCallBack;			
extern RpAtomicCallBackRender RpNtlToonEdgeTwoSideCallBack;
extern RpAtomicCallBackRender RpNtlToonDefaultCallBack;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RPNTL_MAT_FX_H */

/* RWPUBLICEND */
