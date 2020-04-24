#include "precomp_ntlpresentation.h"
#include "NtlPLCharMeshRenderPipeFarAway.h"
#include "NtlDebug.h"
#include "NtlMaterialExt.h"
#include "NtlPLGlobal.h"
#include "NtlDefaultItemData.h"
#include "NtlProfiler.h"


CNtlPLCharMeshRenderPipeFarAway::CNtlPLCharMeshRenderPipeFarAway(void)
{
}

CNtlPLCharMeshRenderPipeFarAway::~CNtlPLCharMeshRenderPipeFarAway(void)
{
}

CNtlPLCharMeshRenderPipe* CNtlPLCharMeshRenderPipeFarAway::GetInstance()
{
	static CNtlPLCharMeshRenderPipeFarAway Instance;

	return static_cast<CNtlPLCharMeshRenderPipe*>(&Instance);
}

void CNtlPLCharMeshRenderPipeFarAway::Execute(void* _pParam)
{
	NTL_SPROFILE("CNtlPLCharMeshRenderPipeFarAway::Execute")

	sMatExtInfo*	pParam = static_cast<sMatExtInfo*>(_pParam);
	RwTexture*		baseTexture;
	RwRGBA*			color = &pParam->mesh->material->color;
	D3DCOLOR		SkinColor, EnvColor, FinalColor;
	RwBlendFunction srcBlend, destBlend;
	RwBool			zWriteEnable;
	RwBool			bAlphaEnable;


	// Base texture
	baseTexture = pParam->mesh->material->texture;
/*
	RwRenderStateGet(rwRENDERSTATESRCBLEND, (void*)&srcBlend);
	RwRenderStateGet(rwRENDERSTATEDESTBLEND, (void*)&destBlend);
	RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, (void*)&zWriteEnable);
	RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)&bAlphaEnable);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTexture(baseTexture, 0);

	// Character effect color(poisonous, dodge), field color, sphere, cube and so on
	EnvColor = (((RwUInt32)color->alpha) << 24) | (((RwUInt32)color->red) << 16) | (((RwUInt32)color->green) << 8) | color->blue;

	// get the current skin colors & textures
	if(RpNtlMaterialExtGetFlag(pParam->mesh->material) & rpNTL_MATERIAL_SKIN_COLOR)
	{
		SkinColor = (((RwUInt32)color->alpha) << 24) | (((RwUInt32)pParam->pSkinColor->red) << 16) | (((RwUInt32)pParam->pSkinColor->green) << 8) | pParam->pSkinColor->blue;

		FinalColor = SkinColor;
	}
	else
	{
		FinalColor = EnvColor;
	}
*/
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0x88888888);

	RwRenderStateGet(rwRENDERSTATESRCBLEND, (void*)&srcBlend);
	RwRenderStateGet(rwRENDERSTATEDESTBLEND, (void*)&destBlend);
	RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, (void*)&zWriteEnable);
	RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)&bAlphaEnable);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTexture(NULL, 0);


	// Character effect color(poisonous, dodge), field color, sphere, cube and so on
	EnvColor = (((RwUInt32)color->alpha) << 24) | (((RwUInt32)color->red) << 16) | (((RwUInt32)color->green) << 8) | color->blue;

	// get the current skin colors & textures
	if(RpNtlMaterialExtGetFlag(pParam->mesh->material) & rpNTL_MATERIAL_SKIN_COLOR)
	{
		SkinColor = (((RwUInt32)color->alpha) << 24) | (((RwUInt32)pParam->pSkinColor->red) << 16) | (((RwUInt32)pParam->pSkinColor->green) << 8) | pParam->pSkinColor->blue;

		FinalColor = SkinColor;
	}
	else
	{
		FinalColor = EnvColor;
	}

	if(pParam->resEntryHeader->indexBuffer)
	{
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)pParam->resEntryHeader->primType, pParam->mesh->baseIndex, 0, pParam->mesh->numVertices, pParam->mesh->startIndex, pParam->mesh->numPrimitives);
	}
	else
	{

		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)pParam->resEntryHeader->primType, pParam->mesh->baseIndex, pParam->mesh->numPrimitives);
	}

	// restore
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)bAlphaEnable);

	// when selecting the target
	if(RpNtlMaterialExtGetFlag(pParam->mesh->material) & rpNTL_MATERIAL_ADD_COLOR)
	{
		const RwRGBA *pAddColor = NULL;
		pAddColor = RpNtlMaterialExtGetAddColor(pParam->mesh->material);

		RwD3D9SetTexture(baseTexture, 0);

		EnvColor = (((RwUInt32)pAddColor->alpha) << 24)	| (((RwUInt32)pAddColor->red) << 16) | (((RwUInt32)pAddColor->green) << 8) | pAddColor->blue;

		RwD3D9SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, EnvColor);

		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
		RwRenderStateSet(rwRENDERSTATESRCBLEND,(void *)rwBLENDONE);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND,(void*)rwBLENDONE);

		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE2X);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		if(pParam->resEntryHeader->indexBuffer != NULL)
		{
			RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)pParam->resEntryHeader->primType, pParam->mesh->baseIndex, 0, pParam->mesh->numVertices, pParam->mesh->startIndex, pParam->mesh->numPrimitives);
		}
		else
		{
			RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)pParam->resEntryHeader->primType,	pParam->mesh->baseIndex, pParam->mesh->numPrimitives);
		}

		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
		RwD3D9SetRenderState(D3DRS_ZWRITEENABLE, zWriteEnable);
	}

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)bAlphaEnable);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcBlend);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destBlend);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)zWriteEnable);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)bAlphaEnable);

	NTL_EPROFILE()
}
