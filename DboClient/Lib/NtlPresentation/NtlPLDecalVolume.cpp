#include "precomp_ntlpresentation.h"

#include "NtlAtomic.h"
#include "NtlWorldCommon.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlPLResourceManager.h"

#include "NtlPLDecalVolume.h"

CNtlPLDecalVolume* CNtlPLDecalVolume::s_pNtlPLDecalVolumeTool = NULL;

CNtlPLDecalVolume::CNtlPLDecalVolume()
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
	SetLayer(PLENTITY_LAYER_DECAL);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_DECAL_VOLUME);

	m_pVertices = NULL;
	m_pTexture	= NULL;

	m_vPos.x = m_vPos.z = m_vPos.z = 0.0f;
	m_vRot.x = m_vRot.y = m_vRot.z = 0.0f;
	m_vScale.x = m_vScale.y = m_vScale.z = 1.0f;
	m_clrDiffuse.alpha = m_clrDiffuse.red = m_clrDiffuse.green = m_clrDiffuse.blue = 255;

	m_fAppendOffset				= 0.0f;
	m_fVisibleDist				= 0.0f;
	m_uiDecalVolumeFlags		= 0x00000000;

	m_eFadeState				= EDECAL_FADE_OUT;
	m_fTimeFadeCur				= 0.0f;
	m_fTimeFade					= 0.0f;

	ZeroMemory(m_acTextureName, sizeof(RwChar) * dPL_DECAL_TEXTURE_NAME_LEN);
}

CNtlPLDecalVolume::~CNtlPLDecalVolume()
{
}

RwBool CNtlPLDecalVolume::Create(const SPLEntityCreateParam * pParam /* = NULL */)
{
	NTL_FUNCTION("CNtlPLDecalVolume::Create");

	SPLDecalVolumeCreateParam *pDecalParam = (SPLDecalVolumeCreateParam*)pParam;

	if (!m_pVertices)
	{
		m_pVertices = NTL_NEW RwIm3DVertex [dPL_DECAL_VOLUME_BUFFER_SIZE];
	}
	DBO_ASSERT(m_pVertices, "Memory alloc failed.");

	if (strlen(pDecalParam->acTextureName) > 0)
	{
		strcpy_s(m_acTextureName, dPL_DECAL_TEXTURE_NAME_LEN, pDecalParam->acTextureName);
		m_pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(m_acTextureName, ".\\texture\\ntlwe\\decal\\");
		DBO_ASSERT(m_pTexture, "Texture load failed.");

		RwTextureSetFilterMode(m_pTexture, rwFILTERLINEAR);
		RwTextureSetAddressingU(m_pTexture, rwTEXTUREADDRESSCLAMP);
		RwTextureSetAddressingV(m_pTexture, rwTEXTUREADDRESSCLAMP);
	}
	DBO_ASSERT(m_pTexture, "Textrue load failed.");	

	m_vPos					= *pDecalParam->pPos;
	m_vRot					= pDecalParam->vRotation;
	m_vScale				= pDecalParam->vScale;
	m_fRadius				= RwV3dLength(&m_vScale);
	m_clrDiffuse			= pDecalParam->clrDiffuse;

	m_fAppendOffset			= pDecalParam->fAppendOffset;
	m_fVisibleDist			= pDecalParam->fVisibleDist;

	m_uiDecalVolumeFlags	= pDecalParam->uiDecalVolumeFlags;

	UpdateDecal();
	NTL_RETURN(TRUE);
}

void CNtlPLDecalVolume::Destroy()
{
	NTL_FUNCTION("CNtlPLDecalVolume::Destroy");

	NTL_ARRAY_DELETE(m_pVertices);

	if (m_pTexture)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pTexture);
		m_pTexture = NULL;
	}
	NTL_RETURNVOID();
}

RwBool CNtlPLDecalVolume::SetProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLDecalVolume::SetProperty");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLDecalVolume::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLDecalVolume::Update");

	RwReal fSquaredDist = CNtlMath::MathRwV3dSquaredLength(&(m_vPos - *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))));
	if (m_fVisibleDist * m_fVisibleDist > fSquaredDist)
	{
		if (m_eFadeState == EDECAL_FADE_OUT)
		{
			RunableFade(EDECAL_FADE_IN, 0.5f);
		}
	}
	else
	{
		if (m_eFadeState == EDECAL_FADE_IN)
		{
			RunableFade(EDECAL_FADE_OUT, 1.0f);
		}
	}

	UpdateFade(fElapsed);

	NTL_RETURN(TRUE);
}

void CNtlPLDecalVolume::UpdateFade(RwReal fElapsed)
{
	if (m_fTimeFadeCur > 0.0f)
	{
		RwRGBA clrDiffuse = m_clrDiffuse;

		m_fTimeFadeCur -= fElapsed;
		switch (m_eFadeState)
		{
			case EDECAL_FADE_IN:
				if (m_fTimeFadeCur < 0.0f)
				{
					clrDiffuse.alpha = m_clrDiffuse.alpha;
				}
				else
				{
					clrDiffuse.alpha = static_cast<RwUInt8>((1.0f - (m_fTimeFadeCur / m_fTimeFade)) * static_cast<RwReal>(m_clrDiffuse.alpha));
				}
				break;
			case EDECAL_FADE_OUT:
				if (m_fTimeFadeCur < 0.0f)
				{
					clrDiffuse.alpha = 0;
				}
				else
				{
					clrDiffuse.alpha = static_cast<RwUInt8>((m_fTimeFadeCur / m_fTimeFade) * static_cast<RwReal>(m_clrDiffuse.alpha));
				}
				break;
		}

		UpdateDecalColor(&clrDiffuse);
	}
}

void CNtlPLDecalVolume::RunableFade(RwUInt32 eFadeState, RwReal fTime)
{
	m_eFadeState	= eFadeState;
	m_fTimeFadeCur	= fTime;
	m_fTimeFade		= fTime;
}

RwBool CNtlPLDecalVolume::Render()
{
	NTL_FUNCTION("CNtlPLDecalVolume::Render");

	RwMatrix matTransform;
	CNtlMath::MathGetMatrix(&matTransform, &m_vScale, &m_vRot, &m_vPos);

#ifdef dNTL_WORLD_TOOL_MODE

	if (dGET_CUR_PALETTE() == ePM_DECAL_VOLUME)
	{
		RwIm3DVertex rwIm3DVertex[8];

		RwIm3DVertexSetPos(&rwIm3DVertex[0], 0.5f, 0.5f, 0.5f);
		RwIm3DVertexSetPos(&rwIm3DVertex[1], 0.5f, 0.5f, -0.5f);
		RwIm3DVertexSetPos(&rwIm3DVertex[2], -0.5f, 0.5f, 0.5f);
		RwIm3DVertexSetPos(&rwIm3DVertex[3], -0.5f, 0.5f, -0.5f);

		RwIm3DVertexSetU(&rwIm3DVertex[0], 0.0f);
		RwIm3DVertexSetV(&rwIm3DVertex[0], 0.0f);

		RwIm3DVertexSetU(&rwIm3DVertex[1], 0.0f);
		RwIm3DVertexSetV(&rwIm3DVertex[1], 1.0f);

		RwIm3DVertexSetU(&rwIm3DVertex[2], 1.0f);
		RwIm3DVertexSetV(&rwIm3DVertex[2], 0.0f);

		RwIm3DVertexSetU(&rwIm3DVertex[3], 1.0f);
		RwIm3DVertexSetV(&rwIm3DVertex[3], 1.0f);

		if (s_pNtlPLDecalVolumeTool == this)
		{
			RwD3D9SetTexture(m_pTexture, 0);
			RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLNONE);
			if (RwIm3DTransform(rwIm3DVertex, 4, &matTransform, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_ALLOPAQUE))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
				RwIm3DEnd(); 
			}	
			RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLBACK);
		}

		RwV3d avBBoxSrc[2];
		avBBoxSrc[0].x = avBBoxSrc[0].y = avBBoxSrc[0].z = 0.5f;
		avBBoxSrc[1].x = avBBoxSrc[1].y = avBBoxSrc[1].z = -0.5f;

		RwBBox bbox;
		RwBBoxCalculate(&bbox, avBBoxSrc, 2);

		static RwImVertexIndex indicesLine[24] =
		{
			0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7,
			7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7
		};

		RwRGBA			rgbaColor;
		RwIm3DVertex*	prwIm3DVertex	= rwIm3DVertex;

		if (s_pNtlPLDecalVolumeTool == this)
		{
			rgbaColor.alpha = 255;
			rgbaColor.red	= 255;
			rgbaColor.green	= 0;
			rgbaColor.blue	= 0;
		}
		else
		{
			rgbaColor.alpha = 255;
			rgbaColor.red	= 255;
			rgbaColor.green	= 255;
			rgbaColor.blue	= 255;
		}

		for (RwInt32 i = 0; i < 8; ++i)
		{
			RwIm3DVertexSetPos(prwIm3DVertex, i & 1 ? bbox.sup.x: bbox.inf.x,
				i & 2 ? bbox.sup.y: bbox.inf.y,
				i & 4 ? bbox.sup.z: bbox.inf.z);
			RwIm3DVertexSetRGBA(prwIm3DVertex, rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);

			prwIm3DVertex++;
		}

		RwD3D9SetTexture(NULL, 0);
		if (RwIm3DTransform(rwIm3DVertex, 8, &matTransform, rwIM3D_VERTEXXYZ | rwIM3D_ALLOPAQUE))
		{
			RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indicesLine, 24);
			RwIm3DEnd();
		}

		RwIm3DVertexSetPos(&rwIm3DVertex[0], 0.0f, 0.0f, 0.0f);
		RwIm3DVertexSetPos(&rwIm3DVertex[1], 0.0f, 1.25f, 0.0f);

		if (RwIm3DTransform(rwIm3DVertex, 2, &matTransform, rwIM3D_VERTEXXYZ | rwIM3D_ALLOPAQUE))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPEPOLYLINE);
			RwIm3DEnd();
		}
	}

#endif

	if (IsVisible() && CNtlPLGlobal::m_bDecalRender && m_uiVerticesCnt && m_pTexture && m_clrDiffuse.alpha != 0)
	{
		RwSphere sphere; sphere.center = m_vPos; sphere.radius = m_fRadius;
		if (RpNtlCameraFrustumTestSphere(RwCameraGetCurrentCamera(), &sphere, &m_fVisibleDist))
		{
			RwD3D9SetTexture(m_pTexture, 0);
			if (RwIm3DTransform(m_pVertices, m_uiVerticesCnt, &matTransform, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);        
				RwIm3DEnd(); 
			}
			RwD3D9SetTexture(NULL, 0);
		}
	}

	NTL_RETURN(TRUE);
}

void CNtlPLDecalVolume::UpdateDecal()
{
	UpdateDecalVertice();
	UpdateDecalColor(&m_clrDiffuse);
}

void CNtlPLDecalVolume::SetPosition(const RwV3d* pPos)
{
	m_vPos = *pPos;
	UpdateDecal();
}

RwV3d CNtlPLDecalVolume::GetPosition()
{
	return m_vPos;	
}

void CNtlPLDecalVolume::SetScale(const RwV3d* pScale)
{
	m_vScale	= *pScale;
	m_fRadius	= RwV3dLength(&m_vScale);
	UpdateDecal();
}

const RwV3d* CNtlPLDecalVolume::GetScale() const
{
	return &m_vScale;
}

void CNtlPLDecalVolume::SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle)
{
	m_vRot.x = fXAngle;
	m_vRot.y = fYAngle;
	m_vRot.z = fZAngle;
	UpdateDecal();
}

const RwV3d* CNtlPLDecalVolume::GetRotate() const
{
	return &m_vRot;
}

void CNtlPLDecalVolume::SetColor(RwRGBA* pClr)
{
	m_clrDiffuse = *pClr;
	UpdateDecal();
}

const RwRGBA* CNtlPLDecalVolume::GetColor() const
{
	return &m_clrDiffuse;
}

void CNtlPLDecalVolume::SetDecalVolumeFlags(RwUInt32 uiFlags)
{
	m_uiDecalVolumeFlags = uiFlags; 
	UpdateDecal();
}

RwUInt32 CNtlPLDecalVolume::GetDecalVolumeFlags()
{
	return m_uiDecalVolumeFlags;
}

void CNtlPLDecalVolume::SetAppendOffset(RwReal fAppendOffset)
{
	m_fAppendOffset = fAppendOffset;
	UpdateDecal();
}

RwReal CNtlPLDecalVolume::GetAppendOffset()
{
	return m_fAppendOffset;
}

void CNtlPLDecalVolume::SetVisibleDist(RwReal fVisibleDist)
{
	m_fVisibleDist = fVisibleDist;
}

RwReal CNtlPLDecalVolume::GetVisibleDist()
{
	return m_fVisibleDist;
}

void CNtlPLDecalVolume::GetDecalBBox(RwBBox* pBBox)
{
	RwMatrix matTransform;
	CNtlMath::MathGetMatrix(&matTransform, &m_vScale, &m_vRot, &m_vPos);

	RwV3d avBox[8];
	for (int i = 0; i < 8; ++i)
	{
		avBox[i].x = (i & 1 ? 0.5f : -0.5f);
		avBox[i].y = (i & 2 ? 0.5f : -0.5f);
		avBox[i].z = (i & 4 ? 0.5f : -0.5f);
	}
	RwV3dTransformPoints(avBox, avBox, 8, &matTransform);
	RwBBoxCalculate(pBBox, avBox, 8);
}

struct SNtlRwIm3DVertexGetDecal
{
	RwIm3DVertex*			pVertices;
	RwUInt32				uiOffset;
	RwUInt32				uiSize;
	RwV3d					vNormal;
	RwReal					fTerrainOffset;
	RwMatrix*				pMatTransfrom;
	RwMatrix*				pMatTransfromInv;
	CNtlPLDecalVolume*		pPLDecalVolume;
};

RpCollisionTriangle* NtlRwIm3DVertexGetDecalBsp(RpIntersection *pIntersection, RpWorldSector* pRpWorldSector, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData)
{
	SNtlRwIm3DVertexGetDecal* pNtlRwIm3DVertexGet = (SNtlRwIm3DVertexGetDecal*)pData;
	if (pNtlRwIm3DVertexGet->uiOffset + 3 > pNtlRwIm3DVertexGet->uiSize)
	{
		return NULL;
	}

	RwIm3DVertex* pVertices = pNtlRwIm3DVertexGet->pVertices + pNtlRwIm3DVertexGet->uiOffset;

	RwV3d v0, v1;
	RwV3dSubMacro(&v0, pRpCollisionTriangle->vertices[1], pRpCollisionTriangle->vertices[0]);
	RwV3dSubMacro(&v1, pRpCollisionTriangle->vertices[2], pRpCollisionTriangle->vertices[0]);
	RwV3dCrossProduct(&pVertices->objNormal, &v0, &v1);
	RwV3dNormalize(&pVertices->objNormal, &pVertices->objNormal);

	if (0.0f < RwV3dDotProduct(&pVertices->objNormal, &pNtlRwIm3DVertexGet->vNormal))
	{
		RwBool bAdd = FALSE;
		for (int i = 0; i < 3; ++i)
		{
			RwIm3DVertexSetPos(&pVertices[i], pRpCollisionTriangle->vertices[i]->x, pRpCollisionTriangle->vertices[i]->y, pRpCollisionTriangle->vertices[i]->z);
			RwV3dIncrementScaled(&pVertices[i].objVertex, &pVertices->objNormal, pNtlRwIm3DVertexGet->fTerrainOffset);

			RwV3dTransformPoint(&pVertices[i].objVertex, &pVertices[i].objVertex, pNtlRwIm3DVertexGet->pMatTransfromInv);
			if (pVertices[i].objVertex.y > -0.5f && pVertices[i].objVertex.y < 0.5f)
			{
				bAdd |= TRUE;
			}
		}

		if (bAdd)
		{
			pNtlRwIm3DVertexGet->uiOffset += 3;
		}
	}

	return pRpCollisionTriangle;
}

RpCollisionTriangle * NtlRwIm3DVertexGetDecalObjectTri(RpIntersection* pIntersection, RpCollisionTriangle* pRpCollisionTriangle, RwReal fRatio, void* pData)
{
	SNtlRwIm3DVertexGetDecal* pNtlRwIm3DVertexGet = (SNtlRwIm3DVertexGetDecal*)pData;
	if (pNtlRwIm3DVertexGet->uiOffset + 3 > pNtlRwIm3DVertexGet->uiSize)
	{
		return NULL;
	}

	RwIm3DVertex* pVertices = pNtlRwIm3DVertexGet->pVertices + pNtlRwIm3DVertexGet->uiOffset;

	RwV3d v0, v1;
	RwV3dSubMacro(&v0, pRpCollisionTriangle->vertices[1], pRpCollisionTriangle->vertices[0]);
	RwV3dSubMacro(&v1, pRpCollisionTriangle->vertices[2], pRpCollisionTriangle->vertices[0]);
	RwV3dCrossProduct(&pVertices->objNormal, &v0, &v1);
	RwV3dNormalize(&pVertices->objNormal, &pVertices->objNormal);

	if (0.0f < RwV3dDotProduct(&pVertices->objNormal, &pNtlRwIm3DVertexGet->vNormal))
	{
		RwBool bAdd = FALSE;
		for (int i = 0; i < 3; ++i)
		{
			RwIm3DVertexSetPos(&pVertices[i], pRpCollisionTriangle->vertices[i]->x, pRpCollisionTriangle->vertices[i]->y, pRpCollisionTriangle->vertices[i]->z);
			RwV3dIncrementScaled(&pVertices[i].objVertex, &pVertices->objNormal, pNtlRwIm3DVertexGet->fTerrainOffset);

			RwV3dTransformPoint(&pVertices[i].objVertex, &pVertices[i].objVertex, pNtlRwIm3DVertexGet->pMatTransfrom);
			RwV3dTransformPoint(&pVertices[i].objVertex, &pVertices[i].objVertex, pNtlRwIm3DVertexGet->pMatTransfromInv);
			if (pVertices[i].objVertex.y > -0.5f && pVertices[i].objVertex.y < 0.5f)
			{
				bAdd |= TRUE;
			}
		}

		if (bAdd)
		{
			pNtlRwIm3DVertexGet->uiOffset += 3;
		}
	}

	return pRpCollisionTriangle;
}

RpAtomic* NtlRwIm3DVertexGetDecalObject(RpIntersection* pIntersection, RpWorldSector* pRpWorldSector, RpAtomic* pAtomic, RwReal fRatio, void* pData)
{
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic));
	if(pPLEntity && pPLEntity->GetClassType() == PLENTITY_OBJECT)
	{
		SNtlRwIm3DVertexGetDecal* pNtlRwIm3DVertexGet = (SNtlRwIm3DVertexGetDecal*)pData;
		if ((RpNtlAtomicGetFlag(pAtomic) & NTL_DECAL_VISIBLE) &&  
			!(RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE) &&
			pPLEntity->IsVisible())
		{
			RwFrame*	pFrame		= RpAtomicGetFrame(pAtomic);
			RwMatrix*	pMatrix		= RwFrameGetLTM(pFrame);
			RwMatrix*	pMatInvLtm	= RwMatrixCreate();

			RwMatrixInvert(pMatInvLtm, pMatrix);

			RwBBox	bboxTemp	= pIntersection->t.box;
			RwV3d	avBox[8];
			for (int i = 0; i < 8; ++i)
			{
				avBox[i].x = (i & 1 ? bboxTemp.sup.x : bboxTemp.inf.x);
				avBox[i].y = (i & 2 ? bboxTemp.sup.y : bboxTemp.inf.y);
				avBox[i].z = (i & 4 ? bboxTemp.sup.z : bboxTemp.inf.z);
			}
			RwV3dTransformPoints(avBox, avBox, 8, pMatInvLtm);
			RwBBoxCalculate(&pIntersection->t.box, avBox, 8);
			pNtlRwIm3DVertexGet->pMatTransfrom = pMatrix;			

			RwV3d	vNormalTemp	= pNtlRwIm3DVertexGet->vNormal;
			RwV3dTransformVector(&pNtlRwIm3DVertexGet->vNormal, &pNtlRwIm3DVertexGet->vNormal, pMatInvLtm);

			RpCollisionGeometryForAllIntersections(RpAtomicGetGeometry(pAtomic), pIntersection, NtlRwIm3DVertexGetDecalObjectTri, pData);

			pNtlRwIm3DVertexGet->pMatTransfrom	= NULL;
			pIntersection->t.box				= bboxTemp;
			pNtlRwIm3DVertexGet->vNormal		= vNormalTemp;

			RwMatrixDestroy(pMatInvLtm);
		}
	}

	return pAtomic;
}

void CNtlPLDecalVolume::UpdateDecalVertice()
{
	RwMatrix	matTransform, matInvert;
	CNtlMath::MathGetMatrix(&matTransform, &m_vScale, &m_vRot, &m_vPos);
	RwMatrixInvert(&matInvert, &matTransform);

	RpIntersection intersection;
	intersection.type = rpINTERSECTBOX; 
	GetDecalBBox(&intersection.t.box);

	SNtlRwIm3DVertexGetDecal sNtlRwIm3dVertexGet;
	sNtlRwIm3dVertexGet.pVertices		= m_pVertices;
	sNtlRwIm3dVertexGet.uiOffset		= 0;
	sNtlRwIm3dVertexGet.uiSize			= dPL_DECAL_VOLUME_BUFFER_SIZE;
	sNtlRwIm3dVertexGet.fTerrainOffset	= m_fAppendOffset;
	sNtlRwIm3dVertexGet.pMatTransfrom	= &matTransform;
	sNtlRwIm3dVertexGet.pMatTransfromInv= &matInvert;

	RwV3dTransformVector(&sNtlRwIm3dVertexGet.vNormal, &CNtlPLGlobal::m_vYAxisV3, &matTransform);
	RwV3dNormalize(&sNtlRwIm3dVertexGet.vNormal, &sNtlRwIm3dVertexGet.vNormal);

	// Get Collision IndoorWorld Vertices
	if (m_uiDecalVolumeFlags & EPL_DECAL_VOLUME_FLAG_TERRAIN)
	{
		RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, NtlRwIm3DVertexGetDecalBsp, &sNtlRwIm3dVertexGet);
	}

	if (m_uiDecalVolumeFlags & EPL_DECAL_VOLUME_FLAG_OBJECT)
	{
		RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, NtlRwIm3DVertexGetDecalObject, &sNtlRwIm3dVertexGet);
	}

	m_uiVerticesCnt = sNtlRwIm3dVertexGet.uiOffset;
	for (RwUInt32 i = 0; i < m_uiVerticesCnt; ++i)
	{
		RwV3d vTemp = m_pVertices[i].objVertex;
		RwIm3DVertexSetU(&m_pVertices[i], -(vTemp.x) + 0.5f);
		RwIm3DVertexSetV(&m_pVertices[i], -(vTemp.z) + 0.5f);
	}
}

void CNtlPLDecalVolume::UpdateDecalColor(RwRGBA* pRwRGBA)
{
	for (RwUInt32 i = 0; i < m_uiVerticesCnt; ++i)
	{
		RwIm3DVertexSetRGBA(&m_pVertices[i], pRwRGBA->red, pRwRGBA->green, pRwRGBA->blue, pRwRGBA->alpha); 
	}
}

RwBool CNtlPLDecalVolume::Pick(RwReal& fDist)
{
	RwV3d avBBoxSrc[2];
	avBBoxSrc[0].x = avBBoxSrc[0].y = avBBoxSrc[0].z = 0.5f;
	avBBoxSrc[1].x = avBBoxSrc[1].y = avBBoxSrc[1].z = -0.5f;

	RwMatrix matTransform;
	CNtlMath::MathGetMatrix(&matTransform, &m_vScale, &m_vRot, &m_vPos);
	RwV3dTransformPoints(avBBoxSrc, avBBoxSrc, 2, &matTransform);

	RwBBox bbox;
	RwBBoxCalculate(&bbox, avBBoxSrc, 2);

	static RwImVertexIndex indices[36] =
	{
		0, 2, 1, 1, 2, 3, 4, 6, 0, 0, 6, 2, 5, 7, 4, 4, 7, 6,
		1, 3, 5, 5, 3, 7, 4, 0, 5, 5, 0, 1, 2, 6, 3, 3, 6, 7
	};

	RwV3d avVertices[8];
	for (RwInt32 i = 0; i < 8; ++i)
	{
		avVertices[i].x = (i & 1 ? bbox.sup.x : bbox.inf.x);
		avVertices[i].y = (i & 2 ? bbox.sup.y : bbox.inf.y);
		avVertices[i].z = (i & 4 ? bbox.sup.z : bbox.inf.z);
	}

	RwV2d ptPickScreen;
	ptPickScreen.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ptPickScreen.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

	RwLine rayCamera;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &rayCamera, &ptPickScreen);

	RwV3d vDirRay, vRayDeltha;
	RwV3dSubMacro(&vDirRay, &rayCamera.end, &rayCamera.start);
	RwV3dSubMacro(&vRayDeltha, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, &rayCamera.start);

	RwV3dNormalize(&vDirRay, &vDirRay);

	RwReal fTemp	= 0.0f;
	RwBool bPicked	= FALSE;
	for (int i = 0; i < 36; i += 3)
	{
		if (RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &avVertices[indices[i]], &avVertices[indices[i + 1]], &avVertices[indices[i + 2]], &fTemp))
		{
			fDist	= fTemp;
			bPicked = TRUE;
			break;
		}
	}
	return bPicked;
}

struct SNtlGetDecalNormal
{
	RwV3d	vNormal;
	RwReal	fValue;
};

RpCollisionTriangle* NtlGetDecalNormal(RpIntersection *pIntersection, RpWorldSector* pRpWorldSector, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData)
{
	SNtlGetDecalNormal* pNtlGetDecalNormal = (SNtlGetDecalNormal*)pData;

	RwV3d v0, v1, vN;
	RwV3dSubMacro(&v0, pRpCollisionTriangle->vertices[1], pRpCollisionTriangle->vertices[0]);
	RwV3dSubMacro(&v1, pRpCollisionTriangle->vertices[2], pRpCollisionTriangle->vertices[0]);
	RwV3dCrossProduct(&vN, &v0, &v1);
	RwV3dNormalize(&vN, &vN);

	if (0.0f < RwV3dDotProduct(&(-vN), RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))))
	{
		if (pNtlGetDecalNormal->fValue > fRatio)
		{
			pNtlGetDecalNormal->vNormal	= vN;
			pNtlGetDecalNormal->fValue	= fRatio;
		}
	}
	return pRpCollisionTriangle;
}

void CNtlPLDecalVolume::RotatePickPlane()
{
	RpIntersection intersection;
	intersection.type			= rpINTERSECTLINE;
	intersection.t.line.start	= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	intersection.t.line.end		= intersection.t.line.start + ((m_vPos - intersection.t.line.start) * 2.0f);

	SNtlGetDecalNormal sNtlGetDecalNormal;
	sNtlGetDecalNormal.fValue		= RwRealMAXVAL;
	sNtlGetDecalNormal.vNormal		= CNtlPLGlobal::m_vZeroV3;
	if (RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, NtlGetDecalNormal, &sNtlGetDecalNormal))
	{
		RwMatrix mat;
		CNtlMath::MathGetRotationMatrix(&mat, &CNtlPLGlobal::m_vYAxisV3, &sNtlGetDecalNormal.vNormal);
		CNtlMath::MathGetMatrixEulerAngle(&mat, &m_vRot);
	}
}