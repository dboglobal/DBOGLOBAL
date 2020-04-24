#include "StdAfx.h"
#include "ShoreLineIOHandler.h"
#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"
#include "NtlApplication.h"
#include "ntlworldcommon.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlWorldSector.h"
#include "NtlWorldField.h"
#include "NtlPLResourceManager.h"
#include "NtlPLEntity.h"
#include "NtlPLObject.h"
#include "NtlWorldBrush.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"
#include "NtlWorldBrushController.h"
#include "NtlWorldMouse.h"
#include "ProgressWnd.h"
#include "NtlWorldSLManger.h"
#include "PaletteTerrain.h"
#include "NtlPLRenderState.h"
#include "AppConfigManager.h"
#include "NtlCoreUtil.h"
#include "NtlWorldBrushTile.h"
#include "NtlPLEvent.h"
#include "PaletteNMProp.h"
#include "NtlWorldEntityPreview.h"
#include "ntlsharedtype.h"
#include "NtlSLApi.h"
#include "ShoreLineAGenerator.h"
#include "ShoreLinePGenerator.h"
#include "ShoreLineCP.h"
#include "NtlPLShoreLineProp.h"
#include "NtlPLShoreLine.h"


CShoreLineIOHandler::CShoreLineIOHandler(void)
{
	m_Mode				= eSLM_IDLE_MODE;
	m_pBrushSelector	= NULL;
	m_pSLAGen			= NULL;
	m_pSLPGen			= NULL;
	m_pSnapMarker		= NULL;	
}

CShoreLineIOHandler::~CShoreLineIOHandler(void)
{
	Delete();
}

VOID CShoreLineIOHandler::Delete()
{
	for(RwUInt32 i = 0; i < m_vecSelectedSector.size(); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecSelectedSector[i]);
	}
	m_vecSelectedSector.clear();

	NTL_DELETE(m_pBrushSelector);
	NTL_DELETE(m_pSLAGen);
	NTL_DELETE(m_pSLPGen);
	NTL_DELETE(m_pSnapMarker);

	m_Mode = eSLM_IDLE_MODE;
}

RpCollisionTriangle* CShoreLineIOHandler::IntersectionWaterTriCB(RpIntersection* _pIntersection, RpCollisionTriangle* _pCollTriangle, RwReal _Distance, void* _pData)
{
	sNtlWorldCollisionInfo*	pNtlWorldCollisionInfo = static_cast<sNtlWorldCollisionInfo*>(_pData);

	if(_Distance < pNtlWorldCollisionInfo->fDist)
	{
		pNtlWorldCollisionInfo->fDist				= _Distance;
		pNtlWorldCollisionInfo->IsCollidedAtSectors = TRUE;
		pNtlWorldCollisionInfo->RayIntersectionPt4Terrain.x	= _pIntersection->t.line.start.x + _Distance * (_pIntersection->t.line.end.x - _pIntersection->t.line.start.x);
		pNtlWorldCollisionInfo->RayIntersectionPt4Terrain.y	= _pIntersection->t.line.start.y + _Distance * (_pIntersection->t.line.end.y - _pIntersection->t.line.start.y);
		pNtlWorldCollisionInfo->RayIntersectionPt4Terrain.z	= _pIntersection->t.line.start.z + _Distance * (_pIntersection->t.line.end.z - _pIntersection->t.line.start.z);
	}

	return _pCollTriangle;
}

RwBool CShoreLineIOHandler::PickWater(RwV3d& _PickPos)
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					SPos;
	RwInt32					l, m;
	RwV3d					SectorSPos;
	RwInt32					SectorIdx;

	// collision param. definition
	sNtlWorldCollisionInfo	NtlWorldCollisionInfo;
	NtlWorldCollisionInfo.IsCollidedAtSectors		= FALSE;
	NtlWorldCollisionInfo.fDist						= RwRealMAXVAL;

	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

	RwLine cameraRay;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

	for(RwInt32 i = 0; i < 36; ++i)
	{
		RwInt32 CurFieldIdx = pMgr->m_Fields6x6[1][i];
		if(CurFieldIdx == -1)
		{
			continue;
		}
		
		SPos = pFields[CurFieldIdx].GetSPos();

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x	= (RwReal)m;
				SectorSPos.z	= (RwReal)l;
				SectorIdx		= pMgr->GetSectorIdx(SectorSPos);

				if(SectorIdx == -1)
				{
					continue;
				}

				RpAtomic* pCurAtomic = pMgr->m_pSectors[SectorIdx].m_pWater ? pMgr->m_pSectors[SectorIdx].m_pWater->_pAtom : NULL;
				if(pCurAtomic)
				{
					RpIntersection TempIS;
					TempIS.t.line.start = cameraRay.start;
					TempIS.t.line.end	= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
					TempIS.type			= rpINTERSECTLINE;

					RpAtomicForAllIntersections(pCurAtomic, &TempIS, IntersectionWaterTriCB, &NtlWorldCollisionInfo);
				}
			}
		}
	}

	if(NtlWorldCollisionInfo.IsCollidedAtSectors)
	{
		RwV3d	CamPt = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
		RwReal	Cam2T = CNtlMath::MathRwV3dSquaredLength(&(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain - CamPt));
		RwReal	Cam2W = CNtlMath::MathRwV3dSquaredLength(&(NtlWorldCollisionInfo.RayIntersectionPt4Terrain - CamPt));
		
		if(Cam2W < Cam2T)
		{
			_PickPos = NtlWorldCollisionInfo.RayIntersectionPt4Terrain;
		}
		else
		{
			_PickPos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
		}

		return TRUE;
	}

	return FALSE;
}

VOID CShoreLineIOHandler::OnSLM_IDLE()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dGET_MOUSE()->GetLBOnce())
	{
		CNtlPLShoreLineProp* pSLProp = static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));
		if(pSLProp->GetCurType() == eST_NOTHING)
		{
			AfxMessageBox("please select scripts");
			return;
		}

		// select sectors
		if(dKEY(DIK_LMENU))
		{
			m_Mode = eSLM_PICK_MODE;
			OnSLM_PICK();
		}
		else if(dKEY(DIK_LCONTROL))
		{
			OnSLM_DSCM();
		}
		else
		{
			m_Mode = eSLM_PGEN_MODE;

			sSHORELINE_CREATE_PARAM SCP;

			if(dKEY(DIK_LSHIFT))
			{
				RwV3d SSnap, ESnap;
				if(m_pSLPGen->GetSnapVert(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, SSnap, ESnap))
				{
					SCP._Snap	= TRUE;
					SCP._SSnap	= SSnap;
					SCP._ESnap	= ESnap;
					SCP._Pos	= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

					// we don't need pick water here

					CreateShoreLinePGenerator(SCP);
				}
				else
				{
					AfxMessageBox("There're no shoreline client meshes for snapping around here.", MB_OK);
					m_Mode = eSLM_IDLE_MODE;
				}
			}
			else
			{
				SCP._Snap	= FALSE;
				SCP._Pos	= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

				RwV3d PickPos;
				if(PickWater(PickPos))
				{
					PickPos.y += dHEIGHT_GAP;
					SCP._Pos = PickPos;
				}

				CreateShoreLinePGenerator(SCP);
			}

			return;
		}
	}
}

VOID CShoreLineIOHandler::OnSLM_PICK()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// handle height only for client meshes
	if(dKEY(DIK_PGDN))
	{
		SetHeightClientMesh(-dHEIGHT_MOVE_LEN);
	}
	else if(dKEY(DIK_PGUP))
	{
		SetHeightClientMesh(dHEIGHT_MOVE_LEN);
	}

	// delete client mesh
	if(dOKEY(DIK_DELETE))
	{
		if(AfxMessageBox("Are you sure?", MB_YESNO) == IDNO)
		{
			return;
		}

		if(m_vecSelectedSector.size())
		{
			for(unsigned int p = 0; p < m_vecSelectedSector.size(); ++p)
			{
				RwInt32 SectorIdx = pMgr->GetSectorIdx(m_vecSelectedSector[p]->GetPosition());

				if(pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine)
				{
					GetSceneManager()->DeleteEntity(pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine);
					pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine = NULL;
				}

				GetSceneManager()->DeleteEntity(m_vecSelectedSector[p]);
			}
			m_vecSelectedSector.clear();

			m_Mode = eSLM_IDLE_MODE;
		}

		return;
	}

	// cancel selecting
	if(dOKEY(DIK_SPACE))
	{
		if(m_vecSelectedSector.size())
		{
			for(unsigned int p = 0; p < m_vecSelectedSector.size(); ++p)
			{
				RwInt32 SectorIdx = pMgr->GetSectorIdx(m_vecSelectedSector[p]->GetPosition());
				GetSceneManager()->DeleteEntity(m_vecSelectedSector[p]);
			}

			m_vecSelectedSector.clear();

			m_Mode = eSLM_IDLE_MODE;
		}

		return;
	}

	if(dKEY(DIK_LMENU))
	{
		// create
		if(dGET_MOUSE()->GetLBUp())
		{
			if(m_pBrushSelector)
			{
				CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());

				RwV3d OldPosS = pCurBrush->GetPosS();
				RwV3d PosS = pCurBrush->GetPosS();
				RwV3d PosE = pCurBrush->GetPosE();
				RwV3d FPosS;
				RwV3d FPosE;

				if(PosS.x <= PosE.x)
				{
					FPosS.x = PosS.x;
					FPosE.x = PosE.x;
				}
				else
				{
					FPosS.x = PosE.x;
					FPosE.x = PosS.x;
				}

				if(PosS.z <= PosE.z)
				{
					FPosS.z = PosS.z;
					FPosE.z = PosE.z;
				}
				else
				{
					FPosS.z = PosE.z;
					FPosE.z = PosS.z;
				}

				PosS = FPosS;
				PosE = FPosE;

				vector<RwInt32> vecTmpSectorIdx;

				for(RwReal i = PosS.z; i < PosE.z; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
				{
					for(RwReal j = PosS.x; j < PosE.x; j += dGET_WORLD_PARAM()->WorldSectorTileSize)
					{
						RwV3d TmpPos;
						TmpPos.x = j;
						TmpPos.z = i;
						TmpPos.y = pMgr->GetWorldSectorHeight(TmpPos);
						unsigned int Idx = static_cast<unsigned int>(pMgr->GetSectorIdx(TmpPos));

						RwBool Flag = FALSE;
						for(unsigned int b = 0; b < vecTmpSectorIdx.size(); ++b)
						{
							if(vecTmpSectorIdx[b] == Idx)
							{
								Flag = TRUE;
								break;
							}
						}

						if(!Flag)
						{
							vecTmpSectorIdx.push_back(Idx);
						}						
					}
				}

				for(unsigned int k = 0; k < vecTmpSectorIdx.size(); ++k)
				{
					RwV3d TmpPos;
					TmpPos.x = pMgr->CNtlWorldSectorManager::m_pSectors[vecTmpSectorIdx[k]].DatumPoint.x;
					TmpPos.z = pMgr->CNtlWorldSectorManager::m_pSectors[vecTmpSectorIdx[k]].DatumPoint.z;
					TmpPos.y = pMgr->GetWorldSectorHeight(TmpPos);

					SPLEntityCreateParam PLEntityCreateParam;
					PLEntityCreateParam.pPos = &TmpPos;

					CNtlPLObject *pNtlPLObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_WAVE", &PLEntityCreateParam));						
					DBO_ASSERT(pNtlPLObject, "Entity create failed.");

					pNtlPLObject->SetFadeEnable(FALSE);
					m_vecSelectedSector.push_back(pNtlPLObject);
				}

				vecTmpSectorIdx.clear();

				NTL_DELETE(m_pBrushSelector);
			}
		}

		// creating
		if(dGET_MOUSE()->GetLBOnce())
		{
			if(!m_pBrushSelector)
			{
				for(unsigned int p = 0; p < m_vecSelectedSector.size(); ++p)
				{
					GetSceneManager()->DeleteEntity(m_vecSelectedSector[p]);
				}
				m_vecSelectedSector.clear();

				m_pBrushSelector = NTL_NEW CNtlWorldBrush;

				CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
				m_pBrushSelector->SetController(pController);

				CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
				pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
			}
		}

		// dragging
		if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
		{
			CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushSelector->GetController());
			pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
		}
	}
}

VOID CShoreLineIOHandler::OnSLM_AGEN()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();
}

VOID CShoreLineIOHandler::OnSLM_PGEN()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// 1. create client mesh
	if(dOKEY(DIK_RETURN))
	{
		if(AfxMessageBox("Would you like to create?", MB_YESNO) == IDYES)
		{
			CreateClientMesh();

			m_Mode = eSLM_IDLE_MODE;
			NTL_DELETE(m_pSLPGen);

			return;
		}
	}

	if(m_pSLPGen)
	{
		m_pSLPGen->Update();

		if(dOKEY(DIK_BACK))
		{
			m_Mode = eSLM_IDLE_MODE;
			NTL_DELETE(m_pSLPGen);		
		}
	}
}

VOID CShoreLineIOHandler::DeleteClientMeshPoly(CNtlPLShoreLine* _pSLEntity)
{
	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

	RwLine cameraRay;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

	RwV3d RwRayDir, RayDeltha;
	RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);
	RwV3dSubMacro(&RayDeltha, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, &cameraRay.start);

	RwIm3DVertex*	pVB = _pSLEntity->GetVB();
	RwInt32			nVN = _pSLEntity->GetVN();
	RwReal			TmpDist;

	for(RwInt32 i = 0; i < nVN; i += 6)
	{
		if(	RtIntersectionLineTriangle(&cameraRay.start, &RayDeltha,	&pVB[i + 0].objVertex,
																		&pVB[i + 1].objVertex,
																		&pVB[i + 2].objVertex,
																		&TmpDist) ||
			RtIntersectionLineTriangle(&cameraRay.start, &RayDeltha,	&pVB[i + 3].objVertex,
																		&pVB[i + 4].objVertex,
																		&pVB[i + 5].objVertex,
																		&TmpDist))
		{
			_pSLEntity->Delete(i);

			return;
		}
	}
}

VOID CShoreLineIOHandler::OnSLM_DSCM()
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	RwV3d					SPos;
	RwInt32					l, m;
	RwV3d					SectorSPos;
	RwInt32					SectorIdx;

	for(RwInt32 i = 0; i < 36; ++i)
	{
		RwInt32 CurFieldIdx = pMgr->m_Fields6x6[1][i];
		if(CurFieldIdx == -1)
		{
			continue;
		}

		SPos = pFields[CurFieldIdx].GetSPos();

		for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x	= (RwReal)m;
				SectorSPos.z	= (RwReal)l;
				SectorIdx		= pMgr->GetSectorIdx(SectorSPos);

				if(SectorIdx == -1)
				{
					continue;
				}

				if(pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine)
				{
					CNtlPLShoreLine* pSLEntity = static_cast<CNtlPLShoreLine*>(pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine);

					DeleteClientMeshPoly(pSLEntity);

					if(!pSLEntity->GetVN())
					{
						GetSceneManager()->DeleteEntity(pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine);
						pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine = NULL;
					}
				}
			}
		}
	}
}

VOID CShoreLineIOHandler::CreateShoreLinePGenerator(sSHORELINE_CREATE_PARAM& _sSCP)
{
	NTL_DELETE(m_pSLAGen);

	if(_sSCP._Snap)
	{
		m_pSLPGen = NTL_NEW CShoreLinePGenerator(_sSCP._SSnap, _sSCP._ESnap);
	}
	else
	{
		m_pSLPGen = NTL_NEW CShoreLinePGenerator(_sSCP._Pos);
	}
}

VOID CShoreLineIOHandler::CreateShoreLineAGenerator()
{
}

VOID CShoreLineIOHandler::DeleteClientMesh(RwInt32 SectorIdx)
{

}

RwBool CShoreLineIOHandler::InsertClientMesh(RwInt32 _SectorIdx, RwIm3DVertex* _pIm3DVertex, RwInt32 _Size)
{
	CNtlWorldFieldManager*	pMgr = dGETMGR();
	CNtlPLShoreLine*		pNtlPLSL;

	if(!pMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine)
	{
		SPLEntityCreateParam	PLEntityCreateParam;
		RwV3d					Pos;

		Pos.x = pMgr->m_pSectors[_SectorIdx].DatumPoint.x;
		Pos.y = pMgr->m_pSectors[_SectorIdx].DatumPoint.y;
		Pos.z = pMgr->m_pSectors[_SectorIdx].DatumPoint.z;
		PLEntityCreateParam.pPos = &Pos;

		pMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine = static_cast<CNtlPLShoreLine*>(GetSceneManager()->CreateEntity(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE, &PLEntityCreateParam));
		DBO_ASSERT(pMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine, "Entity create failed.");
	}

	pNtlPLSL = static_cast<CNtlPLShoreLine*>(pMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine);

	// get current script name
	CNtlPLShoreLineProp*	pSLProp = static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));
	RwChar					KeyName[32];

	switch(pSLProp->GetCurType())
	{
	case eST_UV_SINGLE:
		{
			strcpy_s(KeyName, 32, pSLProp->GetUVSContainer()->m_KeyName);
		}
		break;

	case eST_UV_DUAL:
		{
			strcpy_s(KeyName, 32, pSLProp->GetUVDContainer()->m_KeyName);
		}
		break;

	case eST_SEQUENCE:
		{
			strcpy_s(KeyName, 32, pSLProp->GetSeqContainer()->m_KeyName);
		}
		break;
	}

	return pNtlPLSL->Insert(_pIm3DVertex, _Size, KeyName);
}

VOID CShoreLineIOHandler::CreateClientMesh()
{
	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pMgr->GetFields());
	CNtlPLShoreLineProp*	pSLProp		= static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));
	RwReal					Size		= 0.0f;

	std::list<CShoreLineCP*>::iterator	The		= m_pSLPGen->SLCP_BEGIN();
	std::list<CShoreLineCP*>::iterator	TheLast = m_pSLPGen->SLCP_END(); --TheLast;
	std::list<CShoreLineCP*>::iterator	SThe, EThe;

	while(The != m_pSLPGen->SLCP_END())
	{
		SThe = The;
		EThe = The;
		++EThe;

		if(EThe == TheLast)
		{
			break;
		}


		CShoreLineCP* pCP = (*The);

		// set size, textures
		switch(pSLProp->GetCurType())
		{
		case eST_UV_SINGLE:
			{
				Size = pSLProp->GetUVSContainer()->m_Size;
			}
			break;

		case eST_UV_DUAL:
			{
				Size = pSLProp->GetUVDContainer()->m_Size;
			}
			break;

		case eST_SEQUENCE:
			{
				Size = pSLProp->GetSeqContainer()->m_Size;
			}
			break;

		case eST_NOTHING:
			{
				DBO_TRACE(FALSE, "CShoreLineIOHandler::CreateClientMesh, eST_NOTHING");
				return;
			}
			break;
		}

		// define vertices and create geometries
		RwV3d	Pos0;
		RwV3d	Pos1; 
		RwV3d	Pos2; 
		RwV3d	Pos3; 

		Pos2 = static_cast<CShoreLineCP*>(*SThe)->m_pCPUIObj->GetPosition();
		Pos3 = Pos2;
		RwV3dIncrementScaled(&Pos3, &static_cast<CShoreLineCP*>(*SThe)->m_Nml, Size);

		Pos0 = static_cast<CShoreLineCP*>(*EThe)->m_pCPUIObj->GetPosition();
		Pos1 = Pos0;
		RwV3dIncrementScaled(&Pos1, &static_cast<CShoreLineCP*>(*EThe)->m_Nml, Size);


		// decide current sector index
		RwV3d	Mid			= (Pos0 + Pos1 + Pos2 + Pos3) / 4.0f;
		RwInt32	SectorIdx	= pMgr->CNtlWorldSectorManager::GetSectorIdx(Mid);

		if(SectorIdx == -1)
		{
			DBO_TRACE(FALSE, "CShoreLineIOHandler::CreateClientMesh, invalid sector index.");
			return;
		}

		//
		// In case of the right
		// 
		// 1----3
		// |  / |
		// | /  |
		// 0----2
		//
		// (1, 0, 3), (3, 0, 2)
		//
		//
		// In case of the left
		//
		// 0----2 
		// |  / |
		// | /  |
		// 1----3
		//
		// (0, 1, 2), (2, 1, 3)
		//

		RwIm3DVertex Vert[6];

		if(m_pSLPGen->m_IsLeft)
		{
			RwIm3DVertexSetU(&Vert[0], 0.0f);		
			RwIm3DVertexSetV(&Vert[0], 1.0f);
			RwIm3DVertexSetU(&Vert[1], 0.0f);		
			RwIm3DVertexSetV(&Vert[1], 0.0f);
			RwIm3DVertexSetU(&Vert[2], 1.0f);		
			RwIm3DVertexSetV(&Vert[2], 1.0f);
			RwIm3DVertexSetRGBA(&Vert[0], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&Vert[1], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&Vert[2], 255, 255, 255, 255);
			RwIm3DVertexSetPos(&Vert[0], Pos0.x, Pos0.y, Pos0.z);
			RwIm3DVertexSetPos(&Vert[1], Pos1.x, Pos1.y, Pos1.z);
			RwIm3DVertexSetPos(&Vert[2], Pos2.x, Pos2.y, Pos2.z);

			RwIm3DVertexSetU(&Vert[3], 1.0f);		
			RwIm3DVertexSetV(&Vert[3], 1.0f);
			RwIm3DVertexSetU(&Vert[4], 0.0f);		
			RwIm3DVertexSetV(&Vert[4], 0.0f);
			RwIm3DVertexSetU(&Vert[5], 1.0f);		
			RwIm3DVertexSetV(&Vert[5], 0.0f);	
			RwIm3DVertexSetRGBA(&Vert[3], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&Vert[4], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&Vert[5], 255, 255, 255, 255);
			RwIm3DVertexSetPos(&Vert[3], Pos2.x, Pos2.y, Pos2.z);
			RwIm3DVertexSetPos(&Vert[4], Pos1.x, Pos1.y, Pos1.z);
			RwIm3DVertexSetPos(&Vert[5], Pos3.x, Pos3.y, Pos3.z);

			// insert sector shoreline polygons
			if(!InsertClientMesh(SectorIdx, Vert, 6))
			{
				AfxMessageBox("different script type of shoreline already exists.", MB_OK);
				return;
			}
		}
		else
		{
			RwIm3DVertexSetU(&Vert[0], 0.0f);		
			RwIm3DVertexSetV(&Vert[0], 0.0f);
			RwIm3DVertexSetU(&Vert[1], 0.0f);		
			RwIm3DVertexSetV(&Vert[1], 1.0f);
			RwIm3DVertexSetU(&Vert[2], 1.0f);		
			RwIm3DVertexSetV(&Vert[2], 0.0f);
			RwIm3DVertexSetRGBA(&Vert[0], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&Vert[1], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&Vert[2], 255, 255, 255, 255);
			RwIm3DVertexSetPos(&Vert[0], Pos1.x, Pos1.y, Pos1.z);
			RwIm3DVertexSetPos(&Vert[1], Pos0.x, Pos0.y, Pos0.z);
			RwIm3DVertexSetPos(&Vert[2], Pos3.x, Pos3.y, Pos3.z);

			RwIm3DVertexSetU(&Vert[3], 1.0f);		
			RwIm3DVertexSetV(&Vert[3], 0.0f);
			RwIm3DVertexSetU(&Vert[4], 0.0f);		
			RwIm3DVertexSetV(&Vert[4], 1.0f);
			RwIm3DVertexSetU(&Vert[5], 1.0f);		
			RwIm3DVertexSetV(&Vert[5], 1.0f);	
			RwIm3DVertexSetRGBA(&Vert[3], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&Vert[4], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&Vert[5], 255, 255, 255, 255);
			RwIm3DVertexSetPos(&Vert[3], Pos3.x, Pos3.y, Pos3.z);
			RwIm3DVertexSetPos(&Vert[4], Pos0.x, Pos0.y, Pos0.z);
			RwIm3DVertexSetPos(&Vert[5], Pos2.x, Pos2.y, Pos2.z);

			// insert sector shoreline polygons
			if(!InsertClientMesh(SectorIdx, Vert, 6))
			{
				AfxMessageBox("different script type of shoreline already exists.", MB_OK);
				return;
			}
		}

		++The;
	}
}

VOID CShoreLineIOHandler::SetHeightClientMesh(RwReal _Value, RwBool _AddFlag/* = TRUE*/)
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();

	if(m_vecSelectedSector.size())
	{
		for(unsigned int p = 0; p < m_vecSelectedSector.size(); ++p)
		{
			RwInt32 SectorIdx = pMgr->GetSectorIdx(m_vecSelectedSector[p]->GetPosition());

			if(pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine)
			{
				RwIm3DVertex*	pVB = static_cast<CNtlPLShoreLine*>(pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine)->GetVB();
				RwInt32			nVN = static_cast<CNtlPLShoreLine*>(pMgr->m_pSectors[SectorIdx].m_pNtlPLEntityShoreLine)->GetVN();

				for(RwInt32 i = 0; i < nVN; ++i)
				{
					if(_AddFlag)
					{
						pVB[i].objVertex.y += _Value;
					}
					else
					{
						pVB[i].objVertex.y = _Value;
					}
				}
			}
		}
	}
}

VOID CShoreLineIOHandler::SetHeight(RwReal _Value, RwBool _AddFlag/* = TRUE*/)
{
	if(m_pSLPGen)
	{
		m_pSLPGen->SetHeight(_Value, _AddFlag);
	}
}

VOID CShoreLineIOHandler::Update()
{
	switch(m_Mode)
	{
		case eSLM_IDLE_MODE:	OnSLM_IDLE();	break;
		case eSLM_PICK_MODE:	OnSLM_PICK();	break;
		case eSLM_AGEN_MODE:	OnSLM_AGEN();	break;
		case eSLM_PGEN_MODE:	OnSLM_PGEN();	break;
	}
}

VOID CShoreLineIOHandler::Render()
{
	if(m_pBrushSelector)
	{
		m_pBrushSelector->Render();
	}

	if(m_pSLPGen)
	{
		m_pSLPGen->Render();
	}
}