#include "precomp_ntlpresentation.h"
#include "NtlPLCharMeshRenderPipe.h"
#include "NtlPLCharMeshRenderPipe01.h"
#include "NtlDebug.h"
#include "NtlMaterialExt.h"
#include "NtlPLGlobal.h"
#include "NtlPLResourceManager.h"
#include "NtlDefaultItemData.h"
#include "NtlProfiler.h"
#include "d3d9dxttex_add.h"


CNtlPLCharMeshRenderPipe01::CNtlPLCharMeshRenderPipe01(void)
{
}

CNtlPLCharMeshRenderPipe01::~CNtlPLCharMeshRenderPipe01(void)
{
}

CNtlPLCharMeshRenderPipe* CNtlPLCharMeshRenderPipe01::GetInstance()
{
	static CNtlPLCharMeshRenderPipe01 Instance;

	return static_cast<CNtlPLCharMeshRenderPipe*>(&Instance);
}

VOID CNtlPLCharMeshRenderPipe01::RenderMajinWithVest()
{
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, m_EnvColor);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &CNtlPLGlobal::m_mD3D9IdentityMatrix);

	dRW_DXT1_ALPHA_CHECKER_ON
	RwD3D9SetTexture(m_pBaseTexture, 0);
	dRW_DXT1_ALPHA_CHECKER_OFF

	RwD3D9SetTexture(m_pSkinTexture, 1);
	RwD3D9SetTexture(m_pMultiTexture, 2);

	if(m_EMUVWorking && GetToggleEMUVAni())
	{
		RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 0);
		RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
		RwD3D9SetTextureStageState(4, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		RwD3D9SetTextureStageState(4, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
		RwD3D9SetTextureStageState(5, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		RwD3D9SetTextureStageState(5, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
		RwD3D9SetTextureStageState(6, D3DTSS_TEXCOORDINDEX, 0);
		RwD3D9SetTextureStageState(6, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

		RwD3D9SetTextureStageState(3, D3DTSS_RESULTARG, D3DTA_TEMP);
		RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(4, D3DTSS_RESULTARG, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(4, D3DTSS_COLOROP,   D3DTOP_MODULATEALPHA_ADDCOLOR);
		RwD3D9SetTextureStageState(4, D3DTSS_COLORARG1, D3DTA_TEMP);
		RwD3D9SetTextureStageState(4, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(4, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
		RwD3D9SetTextureStageState(5, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(5, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(5, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(5, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(5, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(5, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(6, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(6, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(6, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(6, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(6, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(6, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

		RwD3D9SetTransform(D3DTS_TEXTURE3, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
		RwD3D9SetTransform(D3DTS_TEXTURE4, m_pTexMat);
		RwD3D9SetTransform(D3DTS_TEXTURE5, m_pParam->pCamSpaceLightMatrix);
		RwD3D9SetTransform(D3DTS_TEXTURE6, &CNtlPLGlobal::m_mD3D9IdentityMatrix);

		RwD3D9SetTexture(m_pMultiTexture, 3);
		RwD3D9SetTexture(m_pEnvMap, 4);
		RwD3D9SetTexture(m_pParam->pToonTex, 5);
		RwD3D9SetTexture(NULL, 6);
	}
	else
	{
		RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
		RwD3D9SetTextureStageState(4, D3DTSS_TEXCOORDINDEX, 0);
		RwD3D9SetTextureStageState(4, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(4, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(4, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(4, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(4, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(4, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(4, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

		RwD3D9SetTransform(D3DTS_TEXTURE3, m_pParam->pCamSpaceLightMatrix);
		RwD3D9SetTransform(D3DTS_TEXTURE4, &CNtlPLGlobal::m_mD3D9IdentityMatrix);

		RwD3D9SetTexture(m_pParam->pToonTex, 3);
		RwD3D9SetTexture(NULL, 4);
	}

	if(m_pParam->resEntryHeader->indexBuffer)
	{
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	}
	else
	{
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
	}
}

VOID CNtlPLCharMeshRenderPipe01::RenderCHARWithDogi()
{
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, m_AllColor);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(4, D3DTSS_TEXCOORDINDEX, 0);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(4, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTextureStageState(4, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(4, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(4, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE3, m_pParam->pCamSpaceLightMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE4, &CNtlPLGlobal::m_mD3D9IdentityMatrix);

	RwD3D9SetTexture(m_pBaseTexture, 0);
	RwD3D9SetTexture(m_pDogiEditedColorTex, 1);
	RwD3D9SetTexture(m_pBaseTexture, 2);
	RwD3D9SetTexture(m_pParam->pToonTex, 3);
	RwD3D9SetTexture(NULL, 4);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe01::RenderCHARWithDogiEmblem()
{
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, m_AllColor);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 1);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(4, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(4, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(5, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(5, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(4, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(4, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(4, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(4, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(5, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(5, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(5, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(5, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(5, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(5, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE3, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE4, m_pParam->pCamSpaceLightMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE5, &CNtlPLGlobal::m_mD3D9IdentityMatrix);

	dRW_DXT1_ALPHA_CHECKER_ON
	RwD3D9SetTexture(m_pBaseTexture, 0);
	dRW_DXT1_ALPHA_CHECKER_OFF

	RwD3D9SetTexture(m_pParam->pEmblemTex, 2);
	RwD3D9SetTexture(m_pDogiEditedColorTex, 1);
	RwD3D9SetTexture(m_pBaseTexture, 3);
	RwD3D9SetTexture(m_pParam->pToonTex, 4);
	RwD3D9SetTexture(NULL, 5);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe01::RenderCHARPlayers()
{
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, m_AllColor);

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

	dRW_DXT1_ALPHA_CHECKER_ON
	RwD3D9SetTexture(m_pBaseTexture, 0);
	dRW_DXT1_ALPHA_CHECKER_OFF

	RwD3D9SetTexture(m_pParam->pToonTex, 1);

	if(m_EMUVWorking && GetToggleEMUVAni())
	{
		RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
		RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 0);
		RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

		RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_ADD);
		RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

		RwD3D9SetTransform(D3DTS_TEXTURE2, m_pTexMat);
		RwD3D9SetTransform(D3DTS_TEXTURE3, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
		RwD3D9SetTexture(m_pEnvMap, 2);
		RwD3D9SetTexture(NULL, 3);
	}
	else
	{
		RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 0);
		RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

		RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

		RwD3D9SetTransform(D3DTS_TEXTURE2, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
		RwD3D9SetTexture(NULL, 2);
	}

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe01::RenderCHARNMVWithSEM()
{
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, m_AllColor);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	
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
	RwD3D9SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, m_pParam->pCamSpaceLightMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, CNtlPLCharMeshRenderPipe::GetSEMTexMat());
	RwD3D9SetTransform(D3DTS_TEXTURE3, &CNtlPLGlobal::m_mD3D9IdentityMatrix);

	dRW_DXT1_ALPHA_CHECKER_ON
	RwD3D9SetTexture(m_pBaseTexture, 0);
	dRW_DXT1_ALPHA_CHECKER_OFF

	RwD3D9SetTexture(m_pParam->pToonTex, 1);
	RwD3D9SetTexture(m_pSEM, 2);
	RwD3D9SetTexture(NULL, 3);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe01::RenderPetrify()
{
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, m_EnvColor);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
 	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 0);
 	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

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

 	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_MODULATE);
 	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TFACTOR);
 	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
 	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
 	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
 	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, m_pParam->pCamSpaceLightMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &CNtlPLGlobal::m_mD3D9IdentityMatrix);

	RwD3D9SetTexture(m_pPetrify, 0);
	RwD3D9SetTexture(m_pParam->pToonTex, 1);
	RwD3D9SetTexture(NULL, 2);

	if(m_pParam->resEntryHeader->indexBuffer)
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, 0, m_pParam->mesh->numVertices, m_pParam->mesh->startIndex, m_pParam->mesh->numPrimitives);
	else
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)m_pParam->resEntryHeader->primType, m_pParam->mesh->baseIndex, m_pParam->mesh->numPrimitives);
}

VOID CNtlPLCharMeshRenderPipe01::RenderCHAR()
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

void CNtlPLCharMeshRenderPipe01::Execute(void* _pParam)
{
	GetRenderCase(_pParam);

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