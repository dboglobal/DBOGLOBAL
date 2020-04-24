#include "precomp_ntlpresentation.h"
#include "NtlPLCharMeshRenderPipe.h"
#include "NtlPLResourceManager.h"
#include "NtlDefaultItemData.h"


RwTexture*	CNtlPLCharMeshRenderPipe::m_pPetrify		= NULL;
RwBool		CNtlPLCharMeshRenderPipe::m_ToggleEMUVAni	= TRUE;


CNtlPLCharMeshRenderPipe::CNtlPLCharMeshRenderPipe(void)
{
	RwMatrix _SEMTexMat =
	{
		{((RwReal)0.5), ((RwReal)0.0), ((RwReal)0.0)}, 0,
		{((RwReal)0.0),-((RwReal)0.5), ((RwReal)0.0)}, 0,
		{((RwReal)0.0), ((RwReal)0.0), ((RwReal)1.0)}, 0,
		{((RwReal)0.5), ((RwReal)0.5), ((RwReal)0.0)}, 0
	};

	m_SEMTexMat = _SEMTexMat;

	m_mapTinyColorTex.clear();

	if(!m_pPetrify)
		m_pPetrify = CNtlPLResourceManager::GetInstance()->LoadTexture("petrify", ".\\texture\\effect\\");
}

CNtlPLCharMeshRenderPipe::~CNtlPLCharMeshRenderPipe(void)
{
	for each(std::pair<D3DCOLOR, RwTexture*> pair in m_mapTinyColorTex)
	{
		if(pair.second)
		{
			RwTexture* pDel = static_cast<RwTexture*>(pair.second);
			RwTextureDestroy(pDel);

			// Do Not manage reference counts
			pDel = NULL;
		}
	}

	m_mapTinyColorTex.clear();

	//NTL_DELETE(m_pPetrify);
}

void CNtlPLCharMeshRenderPipe::GetRenderCase(void* _pParam)
{
	m_DogiWearingOnly		= FALSE;
	m_pDogiEditedColorTex	= NULL;

	// material extension
	m_pParam = static_cast<sMatExtInfo*>(_pParam);
	
	// petrify
	m_PetrifySkillActivated = RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_PETRIFY;

	// combination color
	m_pCombinationColor	= &m_pParam->mesh->material->color;

	// We're wearing a dogi without a emblem
	if((m_pParam->flags & rpGEOMETRYTEXTURED2) && (RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_EMBLEM) && !m_pParam->pEmblemTex)
		m_DogiWearingOnly = TRUE;

	// Dogi pants have only rpNTL_MATERIAL_DOGI_PANTS
	if((RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_DOGI_PANTS) || (RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_DOGI_SKIRT))
		m_DogiWearingOnly = TRUE;

	// NPC, MOB, VEHICLE specular enviroment map
	m_pSEM = RpNtlMaterialExtGetEnvMapTexture(m_pParam->mesh->material);

	// emblem check; all races can have dogis and the dogis only can have the emblems
	m_EmblemWorking = ((m_pParam->flags & rpGEOMETRYTEXTURED2) && (RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_EMBLEM) && m_pParam->pEmblemTex);

	// Dogi edited color textures
	if(m_EmblemWorking || m_DogiWearingOnly)
		m_pDogiEditedColorTex = GetTinyColorTex(m_pParam->pDogiColor);

	// Base texture
	m_pBaseTexture = m_pParam->mesh->material->texture;

	// MultiTexture only can be for the face or Majin vest
	m_pMultiTexture = RpNtlMaterialExtGetTexture(m_pParam->mesh->material);

	// Character effect color(poisonous, dodge), field color, sphere, cube and so on
	m_EnvColor = (((RwUInt32)m_pCombinationColor->alpha) << 24) | (((RwUInt32)m_pCombinationColor->red) << 16) | (((RwUInt32)m_pCombinationColor->green) << 8) | m_pCombinationColor->blue;

	// fake enviroment map
	m_EMUVWorking = (RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_EMUV);

	if(m_EMUVWorking)
	{
		m_pEnvMap = RpNtlMaterialExtGetUVTexture(m_pParam->mesh->material);
		m_pTexMat = RpNtlMaterialExtGetUVTransform(m_pParam->mesh->material);
	}

	// Color
	if(RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_SKIN_COLOR)
	{
		// Skin color
		m_SkinColor = (((RwUInt32)m_pParam->pSkinColor->alpha) << 24) | (((RwUInt32)m_pParam->pSkinColor->red) << 16) | (((RwUInt32)m_pParam->pSkinColor->green) << 8) | m_pParam->pSkinColor->blue;

		// All color(skin & enviroment)
		RwRGBAReal Result, EnvColorReal;
		RwRGBARealFromRwRGBA(&Result, m_pParam->pSkinColor);
		RwRGBARealFromRwRGBA(&EnvColorReal, m_pCombinationColor);
		Result.alpha	= EnvColorReal.alpha * Result.alpha;
		Result.red		= EnvColorReal.red * Result.red;
		Result.green	= EnvColorReal.green * Result.green;
		Result.blue		= EnvColorReal.blue * Result.blue;
		m_AllColor		= ((RwUInt32)(Result.alpha * 255.0f) << 24) | ((RwUInt32)(Result.red * 255.0f) << 16) | ((RwUInt32)(Result.green * 255.0f) << 8) | (RwUInt32)(Result.blue * 255.0f);

		// Skin textures
		m_pSkinTexture = CNtlDefaultItemTable::GetInstance().GetSkinTex(m_SkinColor);
		NTL_ASSERTE(m_pSkinTexture);
	}
	else
		m_AllColor = m_EnvColor;
}

RwTexture* CNtlPLCharMeshRenderPipe::GetTinyColorTex(RwRGBA* _RGBA)
{
	D3DCOLOR KeyColor = 0xff000000 | (RwUInt32)_RGBA->red << 16 | (RwUInt32)_RGBA->green << 8 | (RwUInt32)_RGBA->blue;

	if(m_mapTinyColorTex.find(KeyColor) == m_mapTinyColorTex.end())
	{
		RwTexture* CreateTex = CNtlPLResourceManager::GetInstance()->CreateTexture("TinyColorTex", 2, 2, 32, rwRASTERFORMAT8888);
		CNtlPLResourceManager::GetInstance()->SetColor(CreateTex, static_cast<DWORD>(KeyColor));
		m_mapTinyColorTex[KeyColor] = CreateTex;
		return CreateTex;
	}

	return m_mapTinyColorTex[KeyColor];
}

void CNtlPLCharMeshRenderPipe::RenderCharTgtSelecting(RwTexture* basetexture)
{
	if(RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_ADD_COLOR)
	{
		const RwRGBA *pAddColor = NULL;
		pAddColor = RpNtlMaterialExtGetAddColor(m_pParam->mesh->material);

		if(m_PetrifySkillActivated)
			RwD3D9SetTexture(m_pPetrify, 0);
		else
			RwD3D9SetTexture(basetexture, 0);

		RwD3D9SetTexture(NULL, 1);

		D3DCOLOR EnvColor = (((RwUInt32)pAddColor->alpha) << 24) | (((RwUInt32)pAddColor->red) << 16) | (((RwUInt32)pAddColor->green) << 8) | pAddColor->blue;

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

		if(m_pParam->resEntryHeader->indexBuffer != NULL)
			RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
		else
			RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType,	m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);

		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
		RwD3D9SetRenderState(D3DRS_ZWRITEENABLE, m_zWriteEnable);
	}
}