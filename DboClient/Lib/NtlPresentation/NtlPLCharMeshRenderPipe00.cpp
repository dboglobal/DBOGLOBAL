#include "precomp_ntlpresentation.h"
#include "NtlPLCharMeshRenderPipe.h"
#include "NtlPLCharMeshRenderPipe00.h"
#include "NtlDebug.h"
#include "NtlMaterialExt.h"
#include "NtlPLGlobal.h"
#include "NtlDefaultItemData.h"
#include "NtlProfiler.h"
#include "d3d9dxttex_add.h"


CNtlPLCharMeshRenderPipe00::CNtlPLCharMeshRenderPipe00(void)
{
}

CNtlPLCharMeshRenderPipe00::~CNtlPLCharMeshRenderPipe00(void)
{
}

CNtlPLCharMeshRenderPipe* CNtlPLCharMeshRenderPipe00::GetInstance()
{
	static CNtlPLCharMeshRenderPipe00 Instance;

	return static_cast<CNtlPLCharMeshRenderPipe*>(&Instance);
}

VOID CNtlPLCharMeshRenderPipe00::RenderMajinWithVest()
{
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE3, m_pParam->pCamSpaceLightMatrix);

	RwD3D9SetTexture(m_pBaseTexture, 0);
	RwD3D9SetTexture(m_pSkinTexture, 1);
	RwD3D9SetTexture(m_pMultiTexture, 2);
	RwD3D9SetTexture(m_pParam->pToonTex, 3);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe00::RenderCHARWithDogi()
{
	m_EnvColor = (((RwUInt32)m_pParam->pDogiColor->alpha) << 24) | (((RwUInt32)m_pParam->pDogiColor->red) << 16) | (((RwUInt32)m_pParam->pDogiColor->green) << 8) | m_pParam->pDogiColor->blue;
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, m_EnvColor);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, m_pParam->pCamSpaceLightMatrix);

	dRW_DXT1_ALPHA_CHECKER_ON
	RwD3D9SetTexture(m_pBaseTexture, 0);
	dRW_DXT1_ALPHA_CHECKER_OFF

	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTexture(m_pParam->pToonTex, 2);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe00::RenderCHARWithDogiEmblem()
{
	m_EnvColor = (((RwUInt32)m_pParam->pDogiColor->alpha) << 24) | (((RwUInt32)m_pParam->pDogiColor->red) << 16) | (((RwUInt32)m_pParam->pDogiColor->green) << 8) | m_pParam->pDogiColor->blue;
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, m_EnvColor);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 1);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE3, m_pParam->pCamSpaceLightMatrix);

	dRW_DXT1_ALPHA_CHECKER_ON
	RwD3D9SetTexture(m_pBaseTexture, 0);
	dRW_DXT1_ALPHA_CHECKER_OFF

	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTexture(m_pParam->pEmblemTex, 2);
	RwD3D9SetTexture(m_pParam->pToonTex, 3);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe00::RenderCHARNMVWithSEM()
{
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_ADD);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, m_pParam->pCamSpaceLightMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, CNtlPLCharMeshRenderPipe::GetSEMTexMat());

	dRW_DXT1_ALPHA_CHECKER_ON
	RwD3D9SetTexture(m_pBaseTexture, 0);
	dRW_DXT1_ALPHA_CHECKER_OFF

	RwD3D9SetTexture(m_pParam->pToonTex, 1);
	RwD3D9SetTexture(m_pSEM, 2);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe00::RenderCHARPlayers()
{
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, m_pParam->pCamSpaceLightMatrix);

	dRW_DXT1_ALPHA_CHECKER_ON
	RwD3D9SetTexture(m_pBaseTexture, 0);
	dRW_DXT1_ALPHA_CHECKER_OFF

	RwD3D9SetTexture(m_pParam->pToonTex, 1);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe00::RenderPetrify()
{
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, m_pParam->pCamSpaceLightMatrix);

	RwD3D9SetTexture(m_pPetrify, 0);
	RwD3D9SetTexture(m_pParam->pToonTex, 1);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe00::RenderCHAR()
{
	if(m_EmblemWorking)
	{
		RenderCHARWithDogiEmblem();
		return;
	}
	else if(m_DogiWearingOnly)
	{
		RenderCHARWithDogi();
		return;
	}

	if(m_pSEM)
	{
		RenderCHARNMVWithSEM();
		return;
	}
	else
	{
		RenderCHARPlayers();
		return;
	}
}

void CNtlPLCharMeshRenderPipe00::Execute(void* _pParam)
{
	m_DogiWearingOnly = FALSE;

	// material extension
	m_pParam = static_cast<sMatExtInfo*>(_pParam);

	// petrify
	m_PetrifySkillActivated = RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_PETRIFY;

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

	// Base texture
	m_pBaseTexture = m_pParam->mesh->material->texture;

	// MultiTexture only can be for the face or Majin vest
	m_pMultiTexture = RpNtlMaterialExtGetTexture(m_pParam->mesh->material);

	// get the current skin colors & textures
	if(RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_SKIN_COLOR)
		m_SkinColor = (((RwUInt32)m_pParam->mesh->material->color.alpha) << 24) | (((RwUInt32)m_pParam->pSkinColor->red) << 16) | (((RwUInt32)m_pParam->pSkinColor->green) << 8) | m_pParam->pSkinColor->blue;

	RwRenderStateGet(rwRENDERSTATESRCBLEND, (void*)&m_srcBlend);
	RwRenderStateGet(rwRENDERSTATEDESTBLEND, (void*)&m_destBlend);
	RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, (void*)&m_zWriteEnable);
	RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)&m_bAlphaEnable);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);

	if(m_PetrifySkillActivated)
	{
		RenderPetrify();
	}
	else
	{
		if(m_pMultiTexture)
			RenderMajinWithVest();
		else
			RenderCHAR();
	}

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 2);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 3);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(4, D3DTSS_TEXCOORDINDEX, 4);
	RwD3D9SetTextureStageState(4, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(5, D3DTSS_TEXCOORDINDEX, 5);
	RwD3D9SetTextureStageState(5, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(6, D3DTSS_TEXCOORDINDEX, 6);
	RwD3D9SetTextureStageState(6, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(4, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(4, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(4, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(5, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(5, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(5, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(5, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(5, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(5, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(6, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(6, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(6, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(6, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(6, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(6, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE3, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE4, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE5, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE6, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTexture(NULL, 2);
	RwD3D9SetTexture(NULL, 3);
	RwD3D9SetTexture(NULL, 4);
	RwD3D9SetTexture(NULL, 5);
	RwD3D9SetTexture(NULL, 6);

	RwD3D9SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_RESULTARG, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_RESULTARG, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_RESULTARG, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(4, D3DTSS_RESULTARG, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(5, D3DTSS_RESULTARG, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(6, D3DTSS_RESULTARG, D3DTA_CURRENT);

	RenderCharTgtSelecting(m_pBaseTexture);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)m_bAlphaEnable);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)m_srcBlend);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)m_destBlend);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)m_zWriteEnable);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)m_bAlphaEnable);
}