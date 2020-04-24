#include "precomp_ntlpresentation.h"

#include "NtlPLGlobal.h"
#include "NtlWorldCommon.h"
#include "NtlWorldInterface.h"

#include "NtlPLRenderState.h"

#include "NtlPLRenderGeometry.h"

#include "NtlPLWorldEntity.h"
//#include "NtlPLSoftSpot.h"
#include "NtlPLObject.h"
#include "NtlPLLightObject.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"
#include "NtlWorldRpLightManager.h"

#include "NtlWorldPaletteDefine.h"

CNtlWorldRpLightManager* CNtlWorldRpLightManager::g_pNtlWorldRpLightManager = NULL;

CNtlWorldRpLightManager::CNtlWorldRpLightManager()
:m_pAtomicCtlPt(NULL)
{
	m_pNtlPLLightObjectStaticDummy[0] = m_pNtlPLLightObjectStaticDummy[1] = m_pNtlPLLightObjectStaticDummy[2] = NULL;
}

CNtlWorldRpLightManager::~CNtlWorldRpLightManager()
{
}

void CNtlWorldRpLightManager::CreateInstance()
{
	if (g_pNtlWorldRpLightManager == NULL)
	{
		g_pNtlWorldRpLightManager = NTL_NEW CNtlWorldRpLightManager;
		g_pNtlWorldRpLightManager->Create();
	}
}

void CNtlWorldRpLightManager::DestroyInstance()
{
	if (g_pNtlWorldRpLightManager)
	{
		g_pNtlWorldRpLightManager->Destroy();
		NTL_DELETE(g_pNtlWorldRpLightManager);
	}
}

void CNtlWorldRpLightManager::Create()
{
	LinkMsg(NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY, 0);
#ifdef dNTL_WORLD_TOOL_MODE

	m_pAtomicCtlPt = API_PL_LoadAtomic("object\\mesh\\ud6_c_f1_light_001.dff", "texture\\object\\");
	DBO_ASSERT(m_pAtomicCtlPt, "Resource load failed.");

	for (int i = 0; i < 3; ++i)
	{
		SPLLightObjectCreateParam	sCreateParam;
		RwV3d						vPos; vPos.x = vPos.y = vPos.z = 0.0f;

		sCreateParam.rpLightType		= rpLIGHTDIRECTIONAL;
		sCreateParam.bRealLight			= FALSE;
		sCreateParam.pPos				= &vPos;
		sCreateParam.vRotate.x			= 0.0f;
		sCreateParam.vRotate.y			= 0.0f;
		sCreateParam.vRotate.z			= 0.0f;
		sCreateParam.fAngle				= 0.0f;
		sCreateParam.fRadius			= 0.5f;
		sCreateParam.rgbafColor.alpha	= 1.0f;
		sCreateParam.rgbafColor.red		= 0.0f;
		sCreateParam.rgbafColor.green	= 0.0f;
		sCreateParam.rgbafColor.blue	= 0.0f;
		// CZ-LO
		sCreateParam.bDecal				= FALSE;
		sCreateParam.bWorldLight		= FALSE;
		DBO_WARNING_MESSAGE("Amk");
		m_pNtlPLLightObjectStaticDummy[i] = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, &sCreateParam);
		DBO_ASSERT(m_pNtlPLLightObjectStaticDummy[i], "Entity create failed.");
	}
#endif
}

void CNtlWorldRpLightManager::Destroy()
{
	UnLinkMsg(NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY);

#ifdef dNTL_WORLD_TOOL_MODE
	for (int i = 0; i < 3; ++i)
	{
		if (m_pNtlPLLightObjectStaticDummy[i])
		{
			GetSceneManager()->DeleteEntity(m_pNtlPLLightObjectStaticDummy[i]);
			m_pNtlPLLightObjectStaticDummy[i] = NULL;
		}
	}
#endif
	
	if (m_pAtomicCtlPt)
	{
		API_PL_DeleteAtomic(m_pAtomicCtlPt);
	}

	DBO_ASSERT((m_listRpLight.empty() &&
		m_listRpLightStandBy.empty() &&
		m_listRpLightRealLoaded.empty() &&
		m_listRpLightDummyLoaded.empty() &&
		m_listRpLightRealLoadedStatic.empty() &&
		m_listRpLightDummyLoadedStatic.empty()), "light not empty.");
}

void CNtlWorldRpLightManager::SaveSwap()
{
#ifdef dNTL_WORLD_TOOL_MODE
	//Sector가 삭제 되면서 자동으로 삭제 된다. 하지만 리스트는 삭제 되지 않는다. 정보만 저장한다.
// 	for (int z = -dGET_WORLD_PARAM()->WorldSizeHalf; z < dGET_WORLD_PARAM()->WorldSizeHalf; z += dGET_WORLD_PARAM()->WorldSectorSize)
// 	{
// 		for (int x = -dGET_WORLD_PARAM()->WorldSizeHalf; x < dGET_WORLD_PARAM()->WorldSizeHalf; x += dGET_WORLD_PARAM()->WorldSectorSize)
// 		{
// 			CNtlPLLightObject::SaveSwapFile(static_cast<RwReal>(x + (dGET_WORLD_PARAM()->WorldSectorSize / 2)), 0.0f, static_cast<RwReal>(z + (dGET_WORLD_PARAM()->WorldSectorSize / 2)));
// 		}
// 	}

	// Dummy Static Save
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	if(_chdir("swap") == -1)
	{
		_mkdir("swap");
		_chdir("swap");
	}		

	if(_chdir("ld") == -1)
	{
		_mkdir("ld");
		_chdir("ld");
	}

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "static_sec.ld");

	FILE* pFile;
	if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return;
	}

	for (int i = 0; i < 3; ++i)
	{
		CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)m_pNtlPLLightObjectStaticDummy[i];

		RpLightType eType;
		eType = pLightObject->GetLightType();
		fwrite(&eType, sizeof(RpLightType), 1, pFile);

		RwBool bTemp;
		bTemp = pLightObject->IsRealLight();
		fwrite(&bTemp, sizeof(RwBool), 1, pFile);

		RwV3d vTemp;
		vTemp = pLightObject->GetPosition();
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

		vTemp = *pLightObject->GetRotate();
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

		RwRGBAReal rgbafTemp;
		rgbafTemp = pLightObject->GetColor();
		fwrite(&rgbafTemp, sizeof(RwRGBAReal), 1, pFile);

		RwReal fTemp;
		fTemp = pLightObject->GetRadius();
		fwrite(&fTemp, sizeof(RwReal), 1, pFile);

		fTemp = pLightObject->GetAngle();
		fwrite(&fTemp, sizeof(RwReal), 1, pFile);

		// CZ-LO
		bTemp = pLightObject->IsEnabledDecal();
		fwrite(&bTemp, sizeof(RwBool), 1, pFile);

		bTemp = pLightObject->IsEnabledWorldLight();
		fwrite(&bTemp, sizeof(RwBool), 1, pFile);
	}

	fclose(pFile);
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
#endif
}

void CNtlWorldRpLightManager::LoadSwap()
{
#ifdef dNTL_WORLD_TOOL_MODE
	// Dummy Load

	for (int z = -dGET_WORLD_PARAM()->WorldSizeHalf; z < dGET_WORLD_PARAM()->WorldSizeHalf; z += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for (int x = -dGET_WORLD_PARAM()->WorldSizeHalf; x < dGET_WORLD_PARAM()->WorldSizeHalf; x += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			CNtlPLLightObject::LoadSwapFile(static_cast<RwReal>(x + (dGET_WORLD_PARAM()->WorldSectorSize / 2)), 0.0f, static_cast<RwReal>(z + (dGET_WORLD_PARAM()->WorldSectorSize / 2)));
		}
	}

	// Dummy Static Load
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("swap");
	_chdir("ld");

	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "static_sec.ld");

	FILE* pFile;
	if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return;
	}

	for (int i = 0; i < 3; ++i)
	{
		CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)m_pNtlPLLightObjectStaticDummy[i];

		SPLLightObjectCreateParam CreateParam;
		fread(&CreateParam.rpLightType, sizeof(RpLightType), 1, pFile);
		fread(&CreateParam.bRealLight, sizeof(RwBool), 1, pFile);

		RwV3d vPos;
		fread(&vPos, sizeof(RwV3d), 1, pFile);
		CreateParam.pPos = &vPos;

		fread(&CreateParam.vRotate, sizeof(RwV3d), 1, pFile);
		fread(&CreateParam.rgbafColor, sizeof(RwRGBAReal), 1, pFile);
		fread(&CreateParam.fRadius, sizeof(RwReal), 1, pFile);
		fread(&CreateParam.fAngle, sizeof(RwReal), 1, pFile);

		// CZ-LO
		//CreateParam.bDecal		= FALSE;
		//CreateParam.bWorldLight = FALSE;
 		fread(&CreateParam.bDecal, sizeof(RwBool), 1, pFile);
 		fread(&CreateParam.bWorldLight, sizeof(RwBool), 1, pFile);

		pLightObject->SetRotate(CreateParam.vRotate.x, CreateParam.vRotate.y, CreateParam.vRotate.z);
		pLightObject->SetColor(CreateParam.rgbafColor);
	}

	fclose(pFile);
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
#endif
}

void CNtlWorldRpLightManager::HandleEvents(RWS::CMsg &pMsg)
{
}

void CNtlWorldRpLightManager::Update(RwReal fElapsed)
{
	UpdateRpLightList(m_listRpLightRealLoaded, fElapsed);

	//DistRpLightList(m_listRpLight);
	SortRpLightList(m_listRpLight);
	//DistRpLightList(m_listRpLightStandBy);	
	SortRpLightList(m_listRpLightStandBy);

	RwInt32			iCnt		= 0;
	RPLIGHT_LIST_IT it			= m_listRpLightStandBy.begin();
	while (it != m_listRpLightStandBy.end())
	{
		SRpLightData& sRpLightData = *it;
		if (!RpLightGetWorld(sRpLightData.pRpLight))
		{
			if (iCnt < 8)
			{
				if (RpLightGetType(sRpLightData.pRpLight) == rpLIGHTAMBIENT ||
					RpLightGetType(sRpLightData.pRpLight) == rpLIGHTDIRECTIONAL)
				{
					if (m_listRpLight.size() >= 8)
					{
						SRpLightData& sRpLightDataRemove = m_listRpLight.back();
						RequestRemoveRpLight(sRpLightDataRemove.pRpLight);
					}
					AddRpLight(sRpLightData);
					AddRpLightList(m_listRpLight, sRpLightData.pRpLight, sRpLightData.pProxy);					
				}
				else if (m_listRpLight.size() < 8)
				{
					AddRpLight(sRpLightData);
					AddRpLightList(m_listRpLight, sRpLightData.pRpLight, sRpLightData.pProxy);
				}
			}
		}
		else if (iCnt >= 8)
		{
			sRpLightData.pProxy->OnRequestRemoveRpLight(sRpLightData.pRpLight);
		}
		++iCnt;
		++it;		
	}
}

void CNtlWorldRpLightManager::Render()
{
#ifdef dNTL_WORLD_TOOL_MODE
	
	//RenderRpLightList()를 쓰지 않은 이유는 근처 Sector 만 그려주기 위해서다.
	RwV3d vCur;
	if (GetSceneManager()->GetWorld()->GetPlayerPosition(vCur))
	{
		if (dGET_BRUSH_ENTITY())
		{
			if (dGET_BRUSH_ENTITY()->GetClassType() == PLENTITY_LIGHT_OBJECT)
			{
				((CNtlPLLightObject*)dGET_BRUSH_ENTITY())->OnRender(NULL);
			}			
			// CZ-SS
// 			else if (dGET_BRUSH_ENTITY()->GetClassType() == PLENTITY_SPOT_LIGHT)
// 			{
// 				((CNtlPLSpot*)dGET_BRUSH_ENTITY())->OnRender(NULL);
// 			}
		}

		CNtlWorldFieldManager*	pWFMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();

		if(dGET_CUR_PALETTE() == ePM_LIGHTOBJECT)
		{
			for (int i = 0; i < (int)g_vecVisibleSectors.size(); ++i)
			{
				RwInt32 iIdx = g_vecVisibleSectors.at(i);
				if (iIdx == -1)
				{
					continue;
				}

				CNtlWorldSector* pNtlWorldSector = &pWFMgr->m_pSectors[iIdx];
				for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityLightObject.size(); ++j)
				{
					CNtlPLLightObject* pPLLightObject = ((CNtlPLLightObject*)pNtlWorldSector->m_vecNtlPLEntityLightObject.at(j));
					pPLLightObject->OnRender(NULL);
				}

				for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityLightObjectDummy.size(); ++j)
				{
					CNtlPLLightObject* pPLLightObject = ((CNtlPLLightObject*)pNtlWorldSector->m_vecNtlPLEntityLightObjectDummy.at(j));
					pPLLightObject->OnRender(NULL);
				}

				for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntityOrnament.size(); ++j)
				{
					CNtlPLObject* pPLObject = ((CNtlPLObject*)pNtlWorldSector->m_vecNtlPLEntityOrnament.at(j));

					for (ListAttachAttr::iterator it = pPLObject->GetAttachAttrList()->begin(); it != pPLObject->GetAttachAttrList()->end(); ++it)
					{
						SPLAttachAttr* pPLAttachAttr = *it;
						if (pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
						{
							CNtlPLLightObject* pPLLightObject = (CNtlPLLightObject*)pPLAttachAttr->pPLEntity;
							pPLLightObject->OnRender(NULL);
						}
					}					
				}
			}
		}
		// CZ-SS
// 		else if (dGET_CUR_PALETTE() == ePM_SOFTSPOT)
// 		{
// 			for (int i = 0; i < (int)g_vecVisibleSectors.size(); ++i)
// 			{
// 				RwInt32 iIdx = g_vecVisibleSectors.at(i);
// 				if (iIdx == -1)
// 				{
// 					continue;
// 				}
// 
// 				CNtlWorldSector* pNtlWorldSector = &pWFMgr->m_pSectors[iIdx];
// 				for (int j = 0; j < (int)pNtlWorldSector->m_vecNtlPLEntitySS.size(); ++j)
// 				{
// 					CNtlPLSpot* pPLSpot = ((CNtlPLSpot*)pNtlWorldSector->m_vecNtlPLEntitySS.at(j));
// 					pPLSpot->OnRender(NULL);
// 				}
// 			}
// 		}
	}
#endif
}

void CNtlWorldRpLightManager::RequestAddRpLight(RpLight* pRpLight, CNtlWorldRpLightProxy* pProxy)
{
	DBO_ASSERT(pRpLight, "Parameter invalid.");

	if (RpLightGetFlags(pRpLight) & (rpLIGHTLIGHTWORLD | rpLIGHTLIGHTATOMICS))
	{
		AddRpLightList(m_listRpLightStandBy, pRpLight, pProxy);
	}
}

void CNtlWorldRpLightManager::RequestRemoveRpLight(RpLight* pRpLight)
{
	DBO_ASSERT(pRpLight, "Parameter invalid.");

	if (RpLightGetFlags(pRpLight) & (rpLIGHTLIGHTWORLD | rpLIGHTLIGHTATOMICS))
	{
		RPLIGHT_LIST_IT it;
		if (FindRpLightList(m_listRpLight, pRpLight, it))
		{
			SRpLightData& sRpLightData = *it;
			RemoveRpLight(sRpLightData);
		}		
		RemoveRpLightList(m_listRpLight, pRpLight);
		RemoveRpLightList(m_listRpLightStandBy, pRpLight);
	}
}

void CNtlWorldRpLightManager::RequestCreateRpLight(RpLight* pRpLight, CNtlWorldRpLightProxy* pProxy)
{
	DBO_ASSERT(pRpLight, "Parameter invalid.");

	if (RpLightGetFlags(pRpLight) & (rpLIGHTLIGHTWORLD | rpLIGHTLIGHTATOMICS))
	{
		if (RpLightGetType(pRpLight) == rpLIGHTAMBIENT ||
			RpLightGetType(pRpLight) == rpLIGHTDIRECTIONAL)
		{			
			AddRpLightList(m_listRpLightRealLoadedStatic, pRpLight, pProxy);
		}
		else
		{
			AddRpLightList(m_listRpLightRealLoaded, pRpLight, pProxy);
		}
	}
	else
	{
		RpWorldAddLight(CNtlPLGlobal::m_pRpWorld, pRpLight);
		if (RpLightGetType(pRpLight) == rpLIGHTAMBIENT ||
			RpLightGetType(pRpLight) == rpLIGHTDIRECTIONAL)
		{
			AddRpLightList(m_listRpLightDummyLoadedStatic, pRpLight, pProxy);
		}
		else
		{
			AddRpLightList(m_listRpLightDummyLoaded, pRpLight, pProxy);
		}
	}
}

void CNtlWorldRpLightManager::RequestDestroyRpLight(RpLight* pRpLight)
{
	DBO_ASSERT(pRpLight, "Parameter invalid.");

	if (RpLightGetWorld(pRpLight))
	{
		RpWorldRemoveLight(RpLightGetWorld(pRpLight), pRpLight);
	}
	
	RemoveRpLightList(m_listRpLight, pRpLight);
	RemoveRpLightList(m_listRpLightStandBy, pRpLight);

	RemoveRpLightList(m_listRpLightRealLoaded, pRpLight);
	RemoveRpLightList(m_listRpLightDummyLoaded, pRpLight);

	RemoveRpLightList(m_listRpLightRealLoadedStatic, pRpLight);
	RemoveRpLightList(m_listRpLightDummyLoadedStatic, pRpLight);
}

RwBool CNtlWorldRpLightManager::AddRpLightList(RPLIGHT_LIST& listRpLight, RpLight* pRpLight, CNtlWorldRpLightProxy* pProxy)
{
	DBO_ASSERT(pRpLight, "Parameter invalid.");

	RPLIGHT_LIST_IT it;
	if (FindRpLightList(listRpLight, pRpLight, it))
	{
		return FALSE;
	}
	if (RpLightGetType(pRpLight) == rpLIGHTAMBIENT ||
		RpLightGetType(pRpLight) == rpLIGHTDIRECTIONAL)
	{
		listRpLight.push_front(SRpLightData(pRpLight, pProxy));
	}
	else
	{
		listRpLight.push_back(SRpLightData(pRpLight, pProxy));
	}
	return TRUE;
}

RwBool CNtlWorldRpLightManager::RemoveRpLightList(RPLIGHT_LIST& listRpLight, RpLight* pRpLight)
{
	DBO_ASSERT(pRpLight, "Parameter invalid.");

	RPLIGHT_LIST_IT it;
	if (!FindRpLightList(listRpLight, pRpLight, it))
	{
		return FALSE;
	}
	listRpLight.erase(it);
	return TRUE;
}

RwBool CNtlWorldRpLightManager::FindRpLightList(RPLIGHT_LIST& listRpLight, RpLight* pRpLight, RPLIGHT_LIST_IT& it)
{
	DBO_ASSERT(pRpLight, "Parameter invalid.");

	it = listRpLight.begin();
	while (it != listRpLight.end())
	{
		SRpLightData& sRpLightData = *it;
		if (sRpLightData.pRpLight == pRpLight)
		{
			return TRUE;
		}
		++it;
	}
	return FALSE;
}

void CNtlWorldRpLightManager::DistRpLightList(RPLIGHT_LIST& listRpLight)
{
	RPLIGHT_LIST_IT it = listRpLight.begin();
	while (it != listRpLight.end())
	{
		SRpLightData& sRpLightData = *it;
		sRpLightData.CalcDistance(RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))));
		++it;
	}
}

bool Compare_RpLight_Dist(CNtlWorldRpLightManager::SRpLightData& dataL, CNtlWorldRpLightManager::SRpLightData& dataR)
{
// 	if (dataL.fDistance != RwRealMINVAL && dataR.fDistance != RwRealMINVAL)
// 	{
//  		return (dataL.fDistance < dataR.fDistance);
// 	}
// 	else if (dataL.fDistance == RwRealMINVAL && dataR.fDistance != RwRealMINVAL)
// 	{
// 		return true;
// 	}
// 	return false;

	if (dataL.pProxy->GetLightValue() != RwRealMINVAL && dataR.pProxy->GetLightValue() != RwRealMINVAL)
	{
		return (dataL.pProxy->GetLightValue() < dataR.pProxy->GetLightValue());
	}
	else if (dataL.pProxy->GetLightValue() == RwRealMINVAL && dataR.pProxy->GetLightValue() != RwRealMINVAL)
	{
		return true;
	}
	return false;
}

void CNtlWorldRpLightManager::SortRpLightList(RPLIGHT_LIST& listRpLight)
{
	listRpLight.sort(Compare_RpLight_Dist);
}

RwBool CNtlWorldRpLightManager::AddRpLight(SRpLightData& sRpLightData)
{
	if (RpLightGetWorld(sRpLightData.pRpLight))
	{
		return TRUE;
	}
	RpWorldAddLight(CNtlPLGlobal::m_pRpWorld, sRpLightData.pRpLight);
	if (sRpLightData.pProxy)
	{
		sRpLightData.pProxy->OnAddRpLight(sRpLightData.pRpLight);
	}

	return TRUE;
}

RwBool CNtlWorldRpLightManager::RemoveRpLight(SRpLightData& sRpLightData)
{
	if (!RpLightGetWorld(sRpLightData.pRpLight))
	{
		return TRUE;
	}
	RpWorldRemoveLight(RpLightGetWorld(sRpLightData.pRpLight), sRpLightData.pRpLight);
	if (sRpLightData.pProxy)
	{
		sRpLightData.pProxy->OnRemoveRpLight(sRpLightData.pRpLight);
	}
	return TRUE;
}

void CNtlWorldRpLightManager::UpdateRpLightList(RPLIGHT_LIST& listRpLight, RwReal fElapsed)
{
	RPLIGHT_LIST listTemp = listRpLight;

	RPLIGHT_LIST_IT it = listTemp.begin();
	while (it != listTemp.end())
	{
		SRpLightData& sRpLightData = *it;
		if (sRpLightData.pProxy)
		{
			sRpLightData.pProxy->OnUpdate(sRpLightData.pRpLight, fElapsed);
		}
		++it;
	}
}

void CNtlWorldRpLightManager::RenderRpLightList(RPLIGHT_LIST& listRpLight)
{
	RPLIGHT_LIST listTemp = listRpLight;

	RPLIGHT_LIST_IT it = listTemp.begin();
	while (it != listTemp.end())
	{
		SRpLightData& sRpLightData = *it;
		if (sRpLightData.pProxy)
		{
			sRpLightData.pProxy->OnRender(sRpLightData.pRpLight);
		}
		++it;
	}
}

static RpCollisionTriangle* RtIntersectionAtomic( RpIntersection *pIntersection, RpCollisionTriangle *pCollTriangle, RwReal distance, void *data)
{
	RwReal* fDist = (RwReal*)data;
	if (*fDist - distance > 0.0001f)
	{
		*fDist = distance;
	}

	return pCollTriangle;
}

RwBool CNtlWorldRpLightManager::PickLightMode(SLightModeData& sLightModeData, RwReal& fDist)
{
	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

	RpIntersection intersection;
	intersection.type = rpINTERSECTLINE;

	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &intersection.t.line, &ScreenPos);

	RwMatrix	mat, matTemp;
	RwV3d		vFront, vLeft, vUp;

	RwMatrixRotate(&matTemp, &CNtlPLGlobal::m_vYAxisV3, sLightModeData.vRotation.y, rwCOMBINEREPLACE);
	RwV3dTransformVector(&vFront, &CNtlPLGlobal::m_vZAxisV3, &matTemp);
	RwV3dTransformVector(&vLeft, &CNtlPLGlobal::m_vXAxisV3, &matTemp);

	RwMatrixRotate(&matTemp, &vLeft, sLightModeData.vRotation.x, rwCOMBINEREPLACE);
	RwV3dTransformVector(&vFront, &vFront, &matTemp);
	RwV3dTransformVector(&vUp, &CNtlPLGlobal::m_vYAxisV3, &matTemp);

	RwMatrixRotate(&matTemp, &CNtlPLGlobal::m_vZAxisV3, sLightModeData.vRotation.z, rwCOMBINEREPLACE);        
	RwV3dTransformVector(&vLeft, &vLeft, &matTemp);
	RwV3dTransformVector(&vFront, &vFront, &matTemp);

	RwMatrixSetIdentity(&mat);
	RwMatrixRotate(&mat, &CNtlPLGlobal::m_vYAxisV3, sLightModeData.vRotation.y, rwCOMBINEREPLACE);
	RwMatrixRotate(&mat, &vLeft, sLightModeData.vRotation.x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&mat, &vFront, sLightModeData.vRotation.z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&mat, &sLightModeData.vPosision, rwCOMBINEPOSTCONCAT);

	RwFrameTransform(RpAtomicGetFrame(m_pAtomicCtlPt), &mat, rwCOMBINEREPLACE);

	RwReal fDistTemp = RwRealMAXVAL;
	if (RpAtomicForAllIntersections(m_pAtomicCtlPt, &intersection, RtIntersectionAtomic, &fDistTemp))
	{
		if (fDistTemp == RwRealMAXVAL)
		{
			return FALSE;
		}

		RwV3d vTemp;
		RwV3dSubMacro( &vTemp, &intersection.t.line.end, &intersection.t.line.start);
		RwV3dScale(&vTemp, &vTemp, fDistTemp);
		fDistTemp = RwV3dLength(&vTemp);
		if (fDistTemp < fDist)
		{
			fDist = fDistTemp;
			return TRUE;
		}
	}

	return FALSE;
}

void CNtlWorldRpLightManager::RenderLightMode(SLightModeData& sLightModeData, RwBool bPick)
{
	/*RwMatrix	mat, matTemp;
	RwV3d		vFront, vLeft, vUp;

	RwMatrixRotate(&matTemp, &CNtlPLGlobal::m_vYAxisV3, sLightModeData.vRotation.y, rwCOMBINEREPLACE);
	RwV3dTransformVector(&vFront, &CNtlPLGlobal::m_vZAxisV3, &matTemp);
	RwV3dTransformVector(&vLeft, &CNtlPLGlobal::m_vXAxisV3, &matTemp);

	RwMatrixRotate(&matTemp, &vLeft, sLightModeData.vRotation.x, rwCOMBINEREPLACE);
	RwV3dTransformVector(&vFront, &vFront, &matTemp);
	RwV3dTransformVector(&vUp, &CNtlPLGlobal::m_vYAxisV3, &matTemp);

	RwMatrixRotate(&matTemp, &CNtlPLGlobal::m_vZAxisV3, sLightModeData.vRotation.z, rwCOMBINEREPLACE);        
	RwV3dTransformVector(&vLeft, &vLeft, &matTemp);
	RwV3dTransformVector(&vFront, &vFront, &matTemp);

	RwMatrixSetIdentity(&mat);
	RwMatrixRotate(&mat, &CNtlPLGlobal::m_vYAxisV3, sLightModeData.vRotation.y, rwCOMBINEREPLACE);
	RwMatrixRotate(&mat, &vLeft, sLightModeData.vRotation.x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&mat, &vFront, sLightModeData.vRotation.z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&mat, &sLightModeData.vPosision, rwCOMBINEPOSTCONCAT);*/

	RwMatrix	mat;
	RwMatrixSetIdentity(&mat);
	RwMatrixRotate(&mat, &CNtlPLGlobal::m_vXAxisV3, sLightModeData.vRotation.x, rwCOMBINEREPLACE);
	RwMatrixRotate(&mat, &CNtlPLGlobal::m_vYAxisV3, sLightModeData.vRotation.y, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&mat, &sLightModeData.vPosision, rwCOMBINEPOSTCONCAT);

	BeginFogState(FALSE);
	BeginLightState(FALSE);
	
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

	RwRGBA rgbaColor;
	RwRGBAFromRwRGBAReal(&rgbaColor, &sLightModeData.rgbafColor);
	rgbaColor.alpha = 128;
	RenderGeometryArrow(&mat, &rgbaColor, 0.5f, FALSE);
	
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

	rgbaColor.alpha = 255;
	if (!bPick)
	{
		rgbaColor.red = 128; rgbaColor.green = 128; rgbaColor.blue = 128;
	}
	RpGeometry *pGeometry = RpAtomicGetGeometry(m_pAtomicCtlPt);
	for (int i = 0; i < RpGeometryGetNumMaterials(pGeometry); ++i)
	{
		RpMaterial *pMaterial = RpGeometryGetMaterial(pGeometry, i);
		RpMaterialSetColor(pMaterial, &rgbaColor);
	}
	RwFrameTransform(RpAtomicGetFrame(m_pAtomicCtlPt), &mat, rwCOMBINEREPLACE);

	RwUInt32 uiFlags = RpGeometryGetFlags(pGeometry);
	RpGeometrySetFlags(pGeometry, ((uiFlags | rpGEOMETRYMODULATEMATERIALCOLOR) & ~(rpGEOMETRYLIGHT)));
	RpAtomicRender(m_pAtomicCtlPt);
	RpGeometrySetFlags(pGeometry, uiFlags);

	EndLightState();
	EndFogState();
}