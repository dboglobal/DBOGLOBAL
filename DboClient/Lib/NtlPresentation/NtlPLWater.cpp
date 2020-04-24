#include "precomp_ntlpresentation.h"
#include "NtlPLEntityFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlPLRenderState.h"
#include "NtlPLWater.h"
#include "NtlPLResourceManager.h"
#include "NtlPLEvent.h"
#include "ntlworldcommon.h"
#include "NtlPLEvent.h"
#include "ceventhandler.h"
#include "NtlPLWaterProp.h"
#include "NtlWorldMaterialPlugin.h"
#include "ntlworldinterface.h"

#include "NtlWorldShadowManager.h"
#include "NtlPLVisualManager.h"
#include "NtlSpecularManager.h"
#include "NtlPLOptionManager.h"

#include "NtlProfiler.h"

void sWATER::Update(RwReal fElapsed)
{
	fElapsed -= (RwReal)(static_cast<RwInt32>(fElapsed));

	RwReal SecPerFrame = 1.0f / _FPS;

	_AccumulatedTime += fElapsed;

	RwInt32 CntFrame = 0;
	while(_AccumulatedTime >= SecPerFrame)
	{
		_AccumulatedTime -= SecPerFrame;
		++CntFrame;
	}

	_IdxFrame += CntFrame;
	while(_IdxFrame >= _NumFrame)
	{
		_IdxFrame -= _NumFrame;
	}
}


RwFrame* CNtlPLWater::s_pWaterFrame = NULL;
RwInt32  CNtlPLWater::m_iWaterFrameRef = 0;

CNtlPLWater::CNtlPLWater()
: m_pWaterProp(NULL)
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD);
	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_WATER);

	if (s_pWaterFrame == NULL)
	{
		s_pWaterFrame = RwFrameCreate();
		RwFrameSetIdentity(s_pWaterFrame);
	}
	++m_iWaterFrameRef;
}

CNtlPLWater::~CNtlPLWater()
{
	if (--m_iWaterFrameRef == 0)
	{
		RwFrameDestroy(s_pWaterFrame);
		s_pWaterFrame = NULL;
	}

	if(m_mapWater.size())
	{
		for(WATER_ITER iter = m_mapWater.begin(); iter != m_mapWater.end(); ++iter)
		{
			sWATER* pWater = iter->second;
			NTL_DELETE(pWater);
		}

		m_mapWater.clear();
	}
}

string&	CNtlPLWater::GetWaterPackName(RwInt32 Idx)
{
	return m_pWaterProp->m_vecWaterProp[Idx]._Name;
}

RwBool CNtlPLWater::OnDelete(CNtlWorldSector* pNtlWorldSector)
{
	NTL_FUNCTION("CNtlPLWater::OnDelete");
	NTL_PRE(pNtlWorldSector);
	NTL_DELETE(pNtlWorldSector->m_pWater);
	NTL_RETURN(TRUE);
}

RwBool CNtlPLWater::OnMove(CNtlWorldSector* pNtlWorldSector, RwReal Append, RwBool ReplaceFlag)
{
	NTL_FUNCTION("CNtlPLWater::OnMove");
	NTL_PRE(pNtlWorldSector);
	NTL_PRE(pNtlWorldSector->m_pWater->_pAtom);

	RpGeometry*		pGeometry		= RpAtomicGetGeometry(pNtlWorldSector->m_pWater->_pAtom);
	RpMorphTarget*	pMorphTarget	= RpGeometryGetMorphTarget(pGeometry, 0);
	RwV3d*			pVList			= RpMorphTargetGetVertices(pMorphTarget);

	RpGeometryLock(pGeometry, rpGEOMETRYLOCKALL);

	if(ReplaceFlag)
	{
		pNtlWorldSector->m_pWater->_Height = Append;

		for(int i = 0; i < 4; ++i)
		{
			pVList->y = Append;
			pVList++;
		}
	}
	else
	{
		pNtlWorldSector->m_pWater->_Height += Append;

		for(int i = 0; i < 4; ++i)
		{
			pVList->y += Append;
			pVList++;
		}
	}

	RpGeometryUnlock(pGeometry);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLWater::OnCreate(CNtlWorldSector* pNtlWorldSector, sSECTOR_WATER_ATTR& SectorWaterAttr)
{
	NTL_FUNCTION("CNtlPLWater::OnCreate");
	NTL_PRE(pNtlWorldSector);

	NTL_DELETE(pNtlWorldSector->m_pWater);

	pNtlWorldSector->m_pWater = NTL_NEW sSECTOR_WATER_ATTR;
	sSECTOR_WATER_ATTR* pSectorWaterAttr = pNtlWorldSector->m_pWater;

	*pSectorWaterAttr = SectorWaterAttr;

	RpGeometry*			pGeometry;
	RpMaterial*			pMaterial;
	RwSurfaceProperties SurfProp;
	RpMorphTarget*		pMorphTarget;
	RwSphere			BoundingSphere;
	RwTexCoords*		pTexCoord;
	RwV3d*				pVList;
	RpTriangle*			pTList;
	RwRGBA*				pPrelights;
	RwV3d				SPos;

	pGeometry		= RpGeometryCreate(4, 2, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYTEXTURED);
	pPrelights		= RpGeometryGetPreLightColors(pGeometry);
	pMorphTarget	= RpGeometryGetMorphTarget(pGeometry, 0);

	pMaterial		= RpMaterialCreate();
	SurfProp.ambient  = 1.0f;
	SurfProp.diffuse  = 1.0f;
	SurfProp.specular = 1.0f;
	RpMaterialSetSurfaceProperties(pMaterial, &SurfProp);

	// initialize material plugin extension
	sRpNtlWorldSectorSplat NtlWorlSectorSplat;
	memset(&NtlWorlSectorSplat, 0, sizeof(sRpNtlWorldSectorSplat));
	NtlWorlSectorSplat.pNtlWorldSector = pNtlWorldSector;
	RpNtlWorldSetSectorSplat(pMaterial, &NtlWorlSectorSplat);

	RpGeometryLock(pGeometry, rpGEOMETRYLOCKALL);

	pVList = RpMorphTargetGetVertices(pMorphTarget);
	pTList = RpGeometryGetTriangles(pGeometry);
	pTexCoord = RpGeometryGetVertexTexCoords(pGeometry, rwTEXTURECOORDINATEINDEX0);

	// 인도어
	//SPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.inf.x;
	//SPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.inf.z;
	SPos.x = pNtlWorldSector->DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
	SPos.y = pNtlWorldSector->DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

	//
	// <3>--<2>
	//  |  / |
	//  | /  |
	// <1>--<0>
	// 

	pVList->x		= SPos.x;
	pVList->y		= SectorWaterAttr._Height;
	pVList->z		= SPos.y;
	pTexCoord->u	= 1.0f;
	pTexCoord->v	= 1.0f;
	::CopyMemory(pPrelights++, &SectorWaterAttr._RGBA, sizeof(RwRGBA));
	pVList++;
	pTexCoord++;

	pVList->x		= SPos.x + dGET_WORLD_PARAM()->WorldSectorSize;
	pVList->y		= SectorWaterAttr._Height;
	pVList->z		= SPos.y;
	pTexCoord->u	= 0.0f;
	pTexCoord->v	= 1.0f;
	::CopyMemory(pPrelights++, &SectorWaterAttr._RGBA, sizeof(RwRGBA));
	pVList++;
	pTexCoord++;

	pVList->x		= SPos.x;
	pVList->y		= SectorWaterAttr._Height;
	pVList->z		= SPos.y + dGET_WORLD_PARAM()->WorldSectorSize;
	pTexCoord->u	= 1.0f;
	pTexCoord->v	= 0.0f;
	::CopyMemory(pPrelights++, &SectorWaterAttr._RGBA, sizeof(RwRGBA));
	pVList++;
	pTexCoord++;

	pVList->x		= SPos.x + dGET_WORLD_PARAM()->WorldSectorSize;
	pVList->y		= SectorWaterAttr._Height;
	pVList->z		= SPos.y + dGET_WORLD_PARAM()->WorldSectorSize;
	::CopyMemory(pPrelights++, &SectorWaterAttr._RGBA, sizeof(RwRGBA));
	pTexCoord->u	= 0.0f;
	pTexCoord->v	= 0.0f;
	pVList++;
	pTexCoord++;


	//
	// <3>--<2>
	//  |  / |
	//  | /  |
	// <0>--<1>
	// 
	RpGeometryTriangleSetVertexIndices(pGeometry, pTList, 1, 2, 3);
	RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);

	RpGeometryTriangleSetVertexIndices(pGeometry, pTList, 2, 1, 0);
	RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);

	RpMorphTargetCalcBoundingSphere(pMorphTarget, &BoundingSphere);
	RpMorphTargetSetBoundingSphere(pMorphTarget, &BoundingSphere);

	RpGeometryUnlock(pGeometry);

	pSectorWaterAttr->_pAtom = RpAtomicCreate();

	RpAtomicSetPipeline(pSectorWaterAttr->_pAtom, D3D9NtlWorldWaterSectorAtomicPipeline);

	// set frame and geometry
	// 인도어
	//RpAtomicSetFrame(pSectorWaterAttr->_pAtom, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(pNtlWorldSector->m_pWorldSector), pParentFrame));
	//RpAtomicSetGeometry(pSectorWaterAttr->_pAtom, pGeometry, 0);
// 	switch (GetSceneManager()->GetActiveWorldType())
// 	{
// 	case AW_HEGITHFIELD:
// 		RpAtomicSetFrame(pSectorWaterAttr->_pAtom, dNTL_WORLD_SECTOR_LOCAL(CNtlPLGlobal::m_pRpWorld, pParentFrame));
// 		break;
// 	case AW_RWWORLD:
		RpAtomicSetFrame(pSectorWaterAttr->_pAtom, s_pWaterFrame/*dNTL_WORLD_SECTOR_LOCAL(CNtlPLGlobal::m_pRpWorld, pParentFrame)*/);
// 		break;
// 	}	
	RpAtomicSetGeometry(pSectorWaterAttr->_pAtom, pGeometry, 0);

	// Remove all local references to ref-counted objects
	RpGeometryDestroy(pGeometry);
	RpMaterialDestroy(pMaterial);

	// texture matrix
	float			UVWrap = SectorWaterAttr._SclTrans;
	D3DXMATRIXA16	Temp;

	D3DXMatrixIdentity(&pNtlWorldSector->m_pWater->_matTex[0]);
	D3DXMatrixIdentity(&pNtlWorldSector->m_pWater->_matTex[1]);
	pNtlWorldSector->m_pWater->_matTex[1]._11 *= UVWrap;
	pNtlWorldSector->m_pWater->_matTex[1]._22 *= UVWrap;

	float Error = 0.5f * UVWrap;
	D3DXMatrixTranslation(&Temp, -Error, -Error, 0);
	D3DXMatrixMultiply(&pNtlWorldSector->m_pWater->_matTex[1], &pNtlWorldSector->m_pWater->_matTex[1], &Temp);
	D3DXMatrixRotationZ(&Temp, SectorWaterAttr._RotTrans);
	D3DXMatrixMultiply(&pNtlWorldSector->m_pWater->_matTex[1], &pNtlWorldSector->m_pWater->_matTex[1], &Temp);
	D3DXMatrixTranslation(&Temp, Error, Error, 0);
	D3DXMatrixMultiply(&pNtlWorldSector->m_pWater->_matTex[1], &pNtlWorldSector->m_pWater->_matTex[1], &Temp);

	pNtlWorldSector->m_pWater->_matTex[1].m[2][0] = pNtlWorldSector->m_pWater->_matTex[1].m[3][0];
	pNtlWorldSector->m_pWater->_matTex[1].m[2][1] = pNtlWorldSector->m_pWater->_matTex[1].m[3][1];
	pNtlWorldSector->m_pWater->_matTex[1].m[3][0] = 0.0f;
	pNtlWorldSector->m_pWater->_matTex[1].m[3][1] = 0.0f;

	NTL_RETURN(TRUE);
}

RwBool CNtlPLWater::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLWater::Create");
	
	if(!m_pWaterProp)
		NTL_RETURN(FALSE);

	for(unsigned int i = 0; i < m_pWaterProp->m_vecWaterProp.size(); ++i)
	{
		sWATER*	pWater				= NTL_NEW sWATER;
		pWater->_AccumulatedTime	= 0.0f;
		pWater->_IdxElement			= i;
		pWater->_IdxFrame			= 0;
		pWater->_NumFrame			= m_pWaterProp->m_vecWaterProp[i]._NumFrame;
		pWater->_ppTexPack			= NTL_NEW RwTexture* [pWater->_NumFrame];
		pWater->_ppTexSpecular		= NTL_NEW RwTexture* [pWater->_NumFrame];
		pWater->_FPS				= m_pWaterProp->m_vecWaterProp[i]._FPS;

		string  strFolderName	= m_pWaterProp->m_vecWaterProp[i]._Name + string("\\");
		string	strTexPath		= m_pWaterProp->GetTexPath();
		char	FileName[dWATER_PROP_MAX_STRING_SIZE];

		strTexPath = strTexPath + strFolderName;
		for(int j = 0; j < pWater->_NumFrame; ++j)
		{	
			::sprintf_s(FileName, dWATER_PROP_MAX_STRING_SIZE, "%s%d", m_pWaterProp->m_vecWaterProp[i]._Name.c_str(), j);
			pWater->_ppTexPack[j] = CNtlPLResourceManager::GetInstance()->LoadTexture(FileName, strTexPath.c_str());
			DBO_ASSERT(pWater->_ppTexPack[j], "Texture load failed.");

			if (m_pWaterProp->m_vecWaterProp[i]._Specular)
			{
				// specular map
				::sprintf_s(FileName, dWATER_PROP_MAX_STRING_SIZE, "%s%d_s", m_pWaterProp->m_vecWaterProp[i]._Name.c_str(), j);
				pWater->_ppTexSpecular[j] = CNtlPLResourceManager::GetInstance()->LoadTexture(FileName, strTexPath.c_str());
				DBO_ASSERT(pWater->_ppTexPack[j], "Texture load failed.");
			}
			else
			{
				pWater->_ppTexSpecular[j] = NULL;
			}
		}

		m_mapWater.insert(WATER_MAP::value_type(pWater->_IdxElement, pWater));
	}

	NTL_RETURN(TRUE);
}


void CNtlPLWater::Destroy() 
{
	NTL_FUNCTION("CNtlPLWater::Destroy");

	//UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);

	NTL_RETURNVOID();
}

RwBool CNtlPLWater::SetProperty(const CNtlPLProperty* pProperty)
{
	NTL_FUNCTION("CNtlPLWater::SetProperty");
	NTL_PRE(pProperty);

	m_pWaterProp = reinterpret_cast<CNtlPLWaterProp*>(const_cast<CNtlPLProperty*>(pProperty));

	NTL_RETURN(TRUE);
}

void CNtlPLWater::HandleEvents(RWS::CMsg &pMsg)
{
	/*
	if(pMsg.Id == NPEI_IS_ANOTHER_FIELD_CHANGED)
	{
	}*/
}

RwBool CNtlPLWater::Update(RwReal fElapsed)
{
	for(WATER_ITER iter = m_mapWater.begin(); iter != m_mapWater.end(); ++iter)
	{
		iter->second->Update(fElapsed);
	}

	return TRUE;
}

RwBool CNtlPLWater::Render(void)
{
	return TRUE;
}

#include "PerformanceChecker.h"
RwBool CNtlPLWater::OnRender(CNtlWorldSector* pNtlWorldSector, RxD3D9InstanceData* pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	NTL_SPROFILE("CNtlPLWater::OnRender")

		if(!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WATER))
		{
			NTL_RPROFILE(FALSE)
		}

		if (pNtlWorldSector->m_bFullFogged)
		{
			RwD3D9SetTexture(NULL, 0);
			RwD3D9SetTexture(NULL, 1);		
			RwD3D9SetTexture(NULL, 2);
			RwD3D9SetTexture(NULL, 3);
		}
		else
		{
			sSECTOR_WATER_ATTR* pSectorWaterAttr = pNtlWorldSector->m_pWater;

			WATER_ITER	iter;
			sWATER*		pWater;
			iter = m_mapWater.find(WATER_MAP::key_type(pSectorWaterAttr->_IdxSequence));
			if(iter != m_mapWater.end())
			{
				pWater = iter->second;
			}
			else
			{
				NTL_RPROFILE(FALSE)
			}

			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)pNtlWorldSector->m_pWater->_RenderstateSrc);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)pNtlWorldSector->m_pWater->_RenderstateDst);

			RwD3D9SetTexture(pWater->_ppTexPack[pWater->_IdxFrame], 2);
			RwD3D9SetTransform(D3DTS_TEXTURE2, &pNtlWorldSector->m_pWater->_matTex[1]);

			if (CNtlPLOptionManager::GetInstance()->GetWaterSpecular() && pWater->_ppTexSpecular[pWater->_IdxFrame] && CNtlSpecularManager::GetInstance()->IsInSpecualr(&pSectorWaterAttr->_pAtom->boundingSphere))
			{
				RwD3D9SetTexture(CNtlSpecularManager::GetInstance()->GetTexture(), 0);
				RwD3D9SetTexture(pWater->_ppTexSpecular[pWater->_IdxFrame], 1);
				RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlSpecularManager::GetInstance()->GetMatrixTexture());
				RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_pWater->_matTex[1]);

				RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_MODULATECOLOR_ADDALPHA);
			}
			else
			{
				RwD3D9SetTexture(NULL, 0);
				RwD3D9SetTexture(NULL, 1);
				RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_matIden);
				RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_matIden);

				RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_MODULATE);
			}

			if(pNtlWorldSector->m_pWater->_pDepthMap)
			{
				RwD3D9SetTexture(pNtlWorldSector->m_pWater->_pDepthMap, 3);
				RwD3D9SetTransform(D3DTS_TEXTURE3, &pNtlWorldSector->m_pWater->_matTex[0]);

				RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); // CNtlPLWater::OnRender 안에서 결정
				RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			}
			else
			{
				RwD3D9SetTexture(NULL, 3);
				RwD3D9SetTransform(D3DTS_TEXTURE3, &CNtlPLGlobal::m_matIden);

				RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2); // CNtlPLWater::OnRender 안에서 결정
				RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			}
		}
	
	RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, pResEntryHeader->vertexStream->offset, pResEntryHeader->vertexStream->stride);
	RwD3D9SetIndices((IDirect3DVertexBuffer9*)pResEntryHeader->indexBuffer);
	RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	/*CTextureStageStateView::View(0);
	CTextureStageStateView::View(1);
	CTextureStageStateView::View(2);
	CTextureStageStateView::View(3);*/

 	NTL_RPROFILE(TRUE)
}

RwBool CNtlPLWater::BlendingWater(sSECTOR_WATER_ATTR* pSectorWaterAttr, RwReal fNear, RwReal fFar)
{
	NTL_SPROFILE("CNtlPLWater::BlendingWater")

	if(!pSectorWaterAttr)
	{
		NTL_RPROFILE(FALSE)
	}

	RwV3d			vPosCamera		= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RpGeometry*		pGeometry		= pSectorWaterAttr->_pAtom->geometry;
	RpMorphTarget*	pMorphTarget	= RpGeometryGetMorphTarget(pGeometry, 0);
	RwRGBA*			pPrelights		= RpGeometryGetPreLightColors(pGeometry);
	RwV3d*			pVList			= RpMorphTargetGetVertices(pMorphTarget);

	RpGeometryLock(pGeometry, rpGEOMETRYLOCKPRELIGHT);
	for (int i = 0; i < pGeometry->numVertices; ++i)
	{
		vPosCamera.y = pVList->y;
		RwReal fDistance = RwV3dLength(&(pVList[i] - vPosCamera));
		if (fDistance <= fNear)
		{
			pPrelights[i].alpha = pSectorWaterAttr->_RGBA.alpha;
			continue;

		}
		else if (fDistance >= fFar)
		{
			pPrelights[i].alpha = 255;
			continue;
		}

		RwUInt8 uiAlpha = (RwUInt8)((RwReal)(255 - pSectorWaterAttr->_RGBA.alpha) * ((fDistance - fNear) / (fFar - fNear)));
		pPrelights[i].alpha = pSectorWaterAttr->_RGBA.alpha + uiAlpha;
	}
	RpGeometryUnlock(pGeometry);

	NTL_RPROFILE(TRUE)
}


