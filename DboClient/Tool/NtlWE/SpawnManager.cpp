#include "stdafx.h"
#include "precomp_ntlpresentation.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"

#include "ntlworldinterface.h"

#include "SpawnManager.h"
#include "SpawnContainer.h"

#include "NtlSLApi.h"

#include "TableContainer.h"
#include "TextAllTable.h"
#include "MObTable.h"
#include "NPCTable.h"

#include "SpawnTreeNode.h"
#include "SpawnInstNode.h"
#include "SpawnParty.h"
#include "Spawn.h"

#include "PathHandler.h"

#include "NtlWorldDI.h"
#include "NtlWorldCommon.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldPaletteDefine.h"

#include "NtlPLWorldState.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLEntity.h"
#include "NtlPLCharacter.h"
#include "NtlPLRenderGeometry.h"

#include "NtlPLEvent.h"

#include "NtlAtomic.h"
#include "NtlText.h"
#include "NtlMail.h"

#include "NtlWorldBrush.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"
#include "NtlWorldBrushFrustum.h"
#include "NtlWorldBrushLine.h"

#include "PaletteSpawn.h"
#include "PaletteSpawnBase.h"

#include "MiniMapManager.h"
	
using std::string;
#include <string>

CSpawnManager* CSpawnManager::g_pSpawnManager = NULL;

CSpawnManager* GetSpawnInstance(VOID)
{
	if(CSpawnManager::g_pSpawnManager)
	{
		return CSpawnManager::g_pSpawnManager;
	}
	else
	{
		CSpawnManager::g_pSpawnManager = NTL_NEW CSpawnManager;
	}

	return CSpawnManager::g_pSpawnManager;
}

VOID DestroySpawnInstance(VOID)
{
	if(CSpawnManager::g_pSpawnManager)
	{
		CSpawnManager::g_pSpawnManager->Destroy();
		NTL_DELETE(CSpawnManager::g_pSpawnManager);	
	}
}

CSpawnManager::CSpawnManager(void)
{
	srand((unsigned)time(NULL));

	m_pBrushNestCircle				= NULL;
	m_pSpawnContainer				= NULL;
	m_pBrushMultiSelector			= NULL;
	m_pBrushMultiSelectorFrustum	= NULL;
	m_pBrushMultiSelection			= NULL;
	m_pBrushNestCircle				= NULL;

	m_PathModeActive				= FALSE;
	m_NavigatorModeActive			= FALSE;
	m_pPathHandler					= NULL;

	m_bMouseMoveSnap				= FALSE;

	m_bVisibleCircle				= FALSE;
	m_bVisibleCircleScan			= FALSE;

	memset(m_Fields6x6, -1, sizeof(RwInt32) * 36);

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY, 0);

	CSpawnMacroManager::LoadSpawnMacro();
}

CSpawnManager::~CSpawnManager(void)
{
	CSpawnMacroManager::SaveSpawnMacro();

	Destroy();

	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY);
}

VOID CSpawnManager::SetNavigatorModeActive(RwBool _Flag/* = TRUE*/)
{
	if(_Flag)
	{
		m_NavigatorModeActive = TRUE;

		PopAllSpawnCaptured();

		dGET_SPAWN_MODE() = eSEM_IDLE;

		if(dGET_BRUSH_ENTITY())
		{
			GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
			dGET_BRUSH_ENTITY() = NULL;
		}
	}
	else
	{
		m_NavigatorModeActive = FALSE;
	}
}

VOID CSpawnManager::SetPathModeActive(RwBool _Flag/* = TRUE*/)
{
	if(_Flag)
	{
		m_PathModeActive = TRUE;

		PopAllSpawnCaptured();

		dGET_SPAWN_MODE() = eSEM_IDLE;

		if(dGET_BRUSH_ENTITY())
		{
			GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
			dGET_BRUSH_ENTITY() = NULL;
		}
	}
	else
	{
		m_PathModeActive = FALSE;
	}
}

VOID CSpawnManager::Create(eSPAWN_TYPE _eSpawnType)
{
	Destroy();

	CNtlWorldFieldManager*	pMgr = dGETMGR();
	RwChar					ModelCode[32];
	string					StrMdlCode, StrMdlCodeXXX;
	CMobTable*				pMobTbl = static_cast<CMobTable*>(API_GetTableContainer()->GetMobTable());
	CNPCTable*				pNpcTbl = static_cast<CNPCTable*>(API_GetTableContainer()->GetNpcTable());
	CTextTable*				pTextMobTbl = static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMobTbl());
	CTextTable*				pTextNpcTbl = static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetNPCTbl());
	CTable::TABLEIT			Iter;
	RwUInt32				MdlIdx;
	sBOT_TBLDAT*			pBotTblDat;
	RwInt32					SortCode;

	// Spawn party container
	CSpawnPartyContainer::Create();

	// Set current spawn type
	m_CurSpawnType = _eSpawnType;

	// Load current spawn type container
	m_pSpawnContainer = NTL_NEW CSpawnContainer;

	if(m_CurSpawnType == eSPAWN_MOB)
	{
		Iter = pMobTbl->Begin();
		while(Iter != pMobTbl->End())
		{
			MdlIdx		= Iter->first;
			pBotTblDat	= static_cast<sBOT_TBLDAT*>(Iter->second);

			sprintf_s(ModelCode, 32, "%d", MdlIdx);
			StrMdlCode = ModelCode;
			ZeroMemory(ModelCode, 32);
			StrMdlCode._Copy_s(ModelCode, 32, 3);

			SortCode = static_cast<RwInt32>(atoi(ModelCode));

			RwChar Buff[NTL_MAX_DIR_PATH];
			::WideCharToMultiByte(GetACP(), 0, pTextMobTbl->GetText(pBotTblDat->Name).c_str(), -1, Buff, NTL_MAX_DIR_PATH, NULL, NULL);
			string Name		= Buff;
			string StrMdl	= pBotTblDat->szModel;
			m_pSpawnContainer->InsertSpawnTree(SortCode, MdlIdx, pBotTblDat->byLevel, Name, StrMdl);

			Iter++;
		}
	}
	else if(m_CurSpawnType == eSPAWN_NPC)
	{
		Iter = pNpcTbl->Begin();
		while(Iter != pNpcTbl->End())
		{
			MdlIdx		= Iter->first;
			pBotTblDat	= static_cast<sBOT_TBLDAT*>(Iter->second);

			sprintf_s(ModelCode, 32, "%d", MdlIdx);
			StrMdlCode = ModelCode;
			ZeroMemory(ModelCode, 32);
			StrMdlCode._Copy_s(ModelCode, 32, 3);

			SortCode = static_cast<RwInt32>(atoi(ModelCode));

			RwChar Buff[NTL_MAX_DIR_PATH];
			::WideCharToMultiByte(GetACP(), 0, pTextNpcTbl->GetText(pBotTblDat->Name).c_str(), -1, Buff, NTL_MAX_DIR_PATH, NULL, NULL);
			string Name		= Buff;
			string StrMdl	= pBotTblDat->szModel;
			m_pSpawnContainer->InsertSpawnTree(SortCode, MdlIdx, pBotTblDat->byLevel, Name, StrMdl);

			Iter++;
		}
	}

	// Spawn Path Handler
	m_pPathHandler = NTL_NEW CPathHandler;
}

VOID CSpawnManager::CreateEntitiesAtCurFields()
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	for(RwUInt32 i = 0; i < 36; ++i)
	{
		m_pSpawnContainer->CreateSpawnEntity(pMgr->m_Fields6x6[1][i], pMgr);
		m_pSpawnContainer->InsertSpawnLoadAtField(pMgr->m_Fields6x6[1][i], m_vecSpawnLoad);
	}

	m_pPathHandler->CullPathList();
}

VOID CSpawnManager::DestroyEntitiesAtCurFields()
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	for(RwUInt32 i = 0; i < 36; ++i)
	{
		m_pSpawnContainer->DestroySpawnEntity(pMgr->m_Fields6x6[1][i]);
	}

	m_vecSpawnSelect.clear();
	m_vecSpawnLoad.clear();

	m_pPathHandler->OnDeleteDummyEntities();
}

VOID CSpawnManager::Destroy()
{
	if(m_pPathHandler)
	{
		NTL_DELETE(m_pPathHandler);
	}

	m_vecSpawnSelect.clear();
	m_vecSpawnLoad.clear();

	NTL_DELETE(m_pSpawnContainer);
	NTL_DELETE(m_pBrushMultiSelector);
	NTL_DELETE(m_pBrushMultiSelectorFrustum);
	NTL_DELETE(m_pBrushMultiSelection);
	NTL_DELETE(m_pBrushNestCircle);

	CSpawnPartyContainer::Destroy();
}	

CSpawnTreeElement* CSpawnManager::GetTreeElement(BYTE Lvl, string& _MdlName, string& _ObjName)
{
	CSpawnContainer::SPAWN_TREE_NODE_ITER The = m_pSpawnContainer->STN_BEGIN();

	while(The != m_pSpawnContainer->STN_END())
	{
		CSpawnTreeNode* pSpawnTreeNode = The->second;
		for(RwUInt32 i = 0; i < pSpawnTreeNode->m_vecSpawnTreeNode.size(); ++i)
		{	
			if(	pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlLvl == Lvl &&
				!pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlName.compare(_MdlName.c_str()) &&
				!pSpawnTreeNode->m_vecSpawnTreeNode[i].m_Name.compare(_ObjName.c_str()))
			{
				return &pSpawnTreeNode->m_vecSpawnTreeNode[i];
			}
		}

		++The;
	}

	return NULL;
}

RwUInt32 CSpawnManager::GetMdlCodeOfTreeItem(RwChar* pName)
{
	string									Tmp = pName;
	CSpawnContainer::SPAWN_TREE_NODE_ITER	The = m_pSpawnContainer->STN_BEGIN();

	while(The != m_pSpawnContainer->STN_END())
	{
		CSpawnTreeNode* pSpawnTreeNode = The->second;
		for(RwUInt32 i = 0; i < pSpawnTreeNode->m_vecSpawnTreeNode.size(); ++i)
		{
			if(!pSpawnTreeNode->m_vecSpawnTreeNode[i].m_Name.compare(Tmp))
			{
				return pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlGUID;
			}
		}

		++The;
	}

	return 0xffffffff;
}

RwBool CSpawnManager::GetMdlNameOfTreeItem(RwChar* pName, BYTE Lvl, string& _MdlName, RwUInt32 _MdlGUID)
{
	string									Tmp = pName;
	CSpawnContainer::SPAWN_TREE_NODE_ITER	The = m_pSpawnContainer->STN_BEGIN();

	while(The != m_pSpawnContainer->STN_END())
	{
		CSpawnTreeNode* pSpawnTreeNode = The->second;
		for(RwUInt32 i = 0; i < pSpawnTreeNode->m_vecSpawnTreeNode.size(); ++i)
		{
			if(	pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlLvl == Lvl &&
				!pSpawnTreeNode->m_vecSpawnTreeNode[i].m_Name.compare(Tmp) &&
				pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlGUID == _MdlGUID)
			{
				_MdlName = pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlName;
				return TRUE;
			}
		}

		++The;
	}

	return FALSE;
}

RwV3d CSpawnManager::GetSpawnIntersectionPos()
{
	if (m_bMouseMoveSnap)
	{
		SWorldPickInfo sWorldPickInfo;
		ZeroMemory(&sWorldPickInfo, sizeof(SWorldPickInfo));
		sWorldPickInfo.bCharacterExclusion	= TRUE;
		sWorldPickInfo.fMinDist				= RwRealMAXVAL;	
		if (GetSceneManager()->PickWorld(dGET_MOUSE()->GetCusPos().x, dGET_MOUSE()->GetCusPos().y, sWorldPickInfo, 512.0f, 512.0f))
		{
			return sWorldPickInfo.vPickPos;
		}
	}
	return dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
}



VOID CSpawnManager::CreateEntity()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	if(dGET_BRUSH_ENTITY())
	{
		if (m_CurSpawnType == eSPAWN_NPC)
		{
			CSpawnContainer::SPAWN_INST_NODE_ITER itCnt = GetSpawnContainer()->SIN_BEGIN();
			RwInt32 iSpawnCnt = 0;
			while(itCnt != GetSpawnContainer()->SIN_END())
			{
				iSpawnCnt += itCnt->second->GetSpawnCnt();
				++itCnt;
			}
			if (DBO_NPC_MAX_PARTY_NUMBER < iSpawnCnt)
			{
				AfxMessageBox("NPC MAX COUNT OVER!!");
				return;
			}
		}

		RwV3d vDist = GetSpawnIntersectionPos();

		RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vDist);
		if(CurSectorIdx != -1)
		{
			string	ObjName;
			string	MdlName;
			BYTE	Lvl;
			GetCurMdlInfo(ObjName, Lvl, MdlName);

			// create entity
			SPLCharacterCreateParam PLEntityCreateParam;
			PLEntityCreateParam.pPos	= &vDist;
			PLEntityCreateParam.bPcFlag = FALSE;

			CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, dGET_BRUSH_ENTITY()->GetName(), &PLEntityCreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			static_cast<CNtlPLCharacter*>(pNtlPLEntity)->SetBaseScale(1.0f);
			static_cast<CNtlPLCharacter*>(pNtlPLEntity)->SetAngleY(0.0f);
			static_cast<CNtlPLCharacter*>(pNtlPLEntity)->SetPosition(&vDist);

			CSpawn* pSpawn = m_pSpawnContainer->InsertSpawnInst(pNtlPLEntity, pMgr, m_CurSpawnType);
			m_vecSpawnLoad.push_back(pSpawn);
			pSpawn->SetSpawnMacro(GetCurSpawnMacro());
			pSpawn->SetSpawnName(ObjName);

			// register model GUID
			CSpawnContainer::SPAWN_TREE_NODE_ITER The = GetSpawnContainer()->STN_BEGIN();
			while(The != GetSpawnContainer()->STN_END())
			{
				CSpawnTreeNode* pSpawnTreeNode = The->second;
				for(RwUInt32 i = 0; i < pSpawnTreeNode->m_vecSpawnTreeNode.size(); ++i)
				{
					if(	!pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlName.compare(dGET_BRUSH_ENTITY()->GetName()) &&
						!pSpawnTreeNode->m_vecSpawnTreeNode[i].m_Name.compare(ObjName) &&
						pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlLvl == Lvl)
					{
						pSpawn->m_MdlGUID = pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlGUID;
						goto NEXT_STEP;
					}
				}

				++The;
			}

NEXT_STEP:

			// register the last spawn macro
			CSpawnContainer::SPAWN_TREE_NODE_ITER The1 = GetSpawnContainer()->STN_BEGIN();
			while(The1 != GetSpawnContainer()->STN_END())
			{
				CSpawnTreeNode* pSpawnTreeNode = The1->second;
				for(RwUInt32 i = 0; i < pSpawnTreeNode->m_vecSpawnTreeNode.size(); ++i)
				{
					if(	!pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlName.compare(dGET_BRUSH_ENTITY()->GetName()) &&
						!pSpawnTreeNode->m_vecSpawnTreeNode[i].m_Name.compare(ObjName) &&
						pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlLvl == Lvl)
					{
						pSpawnTreeNode->m_vecSpawnTreeNode[i].m_LastMacroIdx = GetCurMacroIndicatorIdx();

						// Make the spawn the first selected one
						PopAllSpawnCaptured();
						PushSpawnSelected(pSpawn);

						return;
					}
				}

				++The1;
			}
		}
	}
}

VOID CSpawnManager::DeleteEntity()
{
	CNtlWorldFieldManager*		pMgr	= dGETMGR();
	vector<CSpawn*>::iterator	The		= m_vecSpawnSelect.begin();

	while(The != m_vecSpawnSelect.end())
	{
		// Remove loaded spawn
		for(RwUInt32 i = 0; i < m_vecSpawnLoad.size(); ++i)
		{
			if((*The) == m_vecSpawnLoad[i])
			{
				m_vecSpawnLoad.erase(m_vecSpawnLoad.begin() + i);
				if(!m_vecSpawnLoad.size())
				{
					m_vecSpawnLoad.clear();
					break;
				}
			}
		}

		// Remove spawn
		m_pSpawnContainer->DeleteSpawnInst((*The), pMgr);

		// Remove selected spawn
		The = m_vecSpawnSelect.erase(The);
	}

	m_vecSpawnSelect.clear();
}

RpAtomic* CSpawnManager::WorldIntersectionSpawnCB(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic));

	if(pPLEntity->GetClassType() != PLENTITY_CHARACTER)
	{
		return pAtomic;
	}

	if(pPLEntity->GetFlags() & NTL_PLEFLAG_NEST_MEMBER)
	{
		return pAtomic;
	}

	sCOLLISION_PARAM*	pCollisionParam = static_cast<sCOLLISION_PARAM*>(pData);
	RwSphere*			pCollSphere		= &pAtomic->worldBoundingSphere;
	RwReal				Dist;
	RwV3d				SubDist;

	RwV3dSubMacro(&SubDist, &pIntersection->t.line.start, &pCollSphere->center);
	Dist = RwV3dLength(&SubDist);
	if(Dist <= pCollisionParam->_Dist)
	{
		pCollisionParam->_Dist			= Dist;
		pCollisionParam->_pNtlPLEntity	= pPLEntity;
	}

	return pAtomic;
}

void CSpawnManager::HandleEvents(RWS::CMsg &pMsg)
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	if(!pMgr)
	{
		return;
	}

	if(!pMgr->GetWorldReady())
	{
		return;
	}

	if(dGET_CUR_PALETTE() != ePM_SPAWN)
	{
		CopyMemory(m_Fields6x6, pMgr->m_Fields6x6[1], sizeof(RwInt32) * 36);
		return;
	}

	RwInt32*				pFieldBlockIndices	= pMgr->m_Fields6x6[1];
	RwBool					SameFieldIdxFlag;
	vector<RwInt32>			vecFieldIndices;
	vecFieldIndices.reserve(40);

	// find the fields to erase
	for(RwInt32 i = 0; i < 36; ++i)	
	{
		if(m_Fields6x6[i] == -1)
		{
			continue;
		}

		SameFieldIdxFlag = FALSE;

		for(RwInt32 j = 0; j < 36; ++j)
		{
			if(m_Fields6x6[i] == pFieldBlockIndices[j])
			{
				SameFieldIdxFlag = TRUE;
				break;
			}
		}

		if(!SameFieldIdxFlag)
		{
			vecFieldIndices.push_back(m_Fields6x6[i]);
		}
	}

	for(RwUInt32 i = 0; i < vecFieldIndices.size(); ++i)
	{
		m_pSpawnContainer->DeleteSpawnLoadAtField(vecFieldIndices[i], m_vecSpawnLoad);
		m_pSpawnContainer->DestroySpawnEntity(vecFieldIndices[i]);
	}

	vecFieldIndices.clear();

	// find the field to load
	for(RwInt32 i = 0; i < 36; ++i)
	{
		if(pFieldBlockIndices[i] == -1)
		{
			continue;
		}

		SameFieldIdxFlag = FALSE;

		for(RwInt32 j = 0; j < 36; ++j)
		{
			if(pFieldBlockIndices[i] == m_Fields6x6[j])
			{
				SameFieldIdxFlag = TRUE;
				break;
			}
		}

		if(!SameFieldIdxFlag)
		{
			vecFieldIndices.push_back(pFieldBlockIndices[i]);
		}
	}

	for(RwUInt32 i = 0; i < vecFieldIndices.size(); ++i)
	{
		m_pSpawnContainer->CreateSpawnEntity(vecFieldIndices[i], pMgr);
		m_pSpawnContainer->InsertSpawnLoadAtField(vecFieldIndices[i], m_vecSpawnLoad);
	}

	vecFieldIndices.clear();

	// Refresh current loaded field blocks
	CopyMemory(m_Fields6x6, pFieldBlockIndices, sizeof(RwInt32) * 36);

	// cull path list
	m_pPathHandler->CullPathList();
}

RwBool CSpawnManager::GetSpawnCapture(RwV3d& _PosObjMid)
{
	CNtlWorldFieldManager*						pMgr		= dGETMGR();
	CNtlWB_Rectangle<CNtlWorldFieldManager>*	pCurBrush	= reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
	RwV3d										PosS		= pCurBrush->GetPosS();
	RwV3d										PosE		= pCurBrush->GetPosE();
	RwBool										AllTrue[2]	= {FALSE, FALSE};

	if(PosS.x > PosE.x)
	{
		if(_PosObjMid.x > PosE.x && _PosObjMid.x < PosS.x)
		{
			AllTrue[0] = TRUE;
		}
	}
	else
	{
		if(_PosObjMid.x < PosE.x && _PosObjMid.x > PosS.x)
		{
			AllTrue[0] = TRUE;
		}
	}

	if(PosS.z > PosE.z)
	{
		if(_PosObjMid.z > PosE.z && _PosObjMid.z < PosS.z)
		{
			AllTrue[1] = TRUE;
		}
	}
	else
	{
		if(_PosObjMid.z < PosE.z && _PosObjMid.z > PosS.z)
		{
			AllTrue[1] = TRUE;
		}
	}

	if(AllTrue[0] && AllTrue[1])
	{
		return TRUE;
	}
	else
		return FALSE;

	return FALSE;
}

VOID CSpawnManager::PopSpawnCaptured(CSpawn* _pSpawn)
{
	vector<CSpawn*>::iterator The = m_vecSpawnSelect.begin();
	while(The != m_vecSpawnSelect.end())
	{
		if((*The) == _pSpawn)
		{
			(*The)->SetNestArrow(eSAT_NEST_DEFAULT);
			m_vecSpawnSelect.erase(The);
			if(!m_vecSpawnSelect.size())
			{
				m_vecSpawnSelect.clear();
			}

			return;
		}

		++The;
	}

	return;
}

RwBool CSpawnManager::GetSpawnCaptured(CSpawn* _pSpawn)
{
	for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
	{
		if(m_vecSpawnSelect[i] == _pSpawn)
		{
			return TRUE;
		}
	}

	return FALSE;
}

VOID CSpawnManager::PopAllSpawnCaptured()
{
	vector<CSpawn*>::iterator The = m_vecSpawnSelect.begin();
	while(The != m_vecSpawnSelect.end())
	{
		(*The)->SetNestArrow(eSAT_NEST_DEFAULT);
		The = m_vecSpawnSelect.erase(The);
	}
	m_vecSpawnSelect.clear();
}

VOID CSpawnManager::PushSpawnSelected(CSpawn* _pSpawn)
{
	_pSpawn->SetNestArrow(eSAT_NEST_SELECT);
	m_vecSpawnSelect.push_back(_pSpawn);
}

VOID CSpawnManager::InsertSpawnLoad(CSpawn* _pSpawn)
{
	RwBool IsExisted = FALSE;
	vector<CSpawn*>::iterator The = m_vecSpawnLoad.begin();
	while(The != m_vecSpawnLoad.end())
	{
		if((*The) == _pSpawn)
		{
			IsExisted = TRUE;
			break;
		}

		++The;
	}	

	if(!IsExisted)
	{
		m_vecSpawnLoad.push_back(_pSpawn);
	}
}

VOID CSpawnManager::DeleteSpawnLoad(CSpawn* _pSpawn)
{
	vector<CSpawn*>::iterator The = m_vecSpawnLoad.begin();
	while(The != m_vecSpawnLoad.end())
	{
		if((*The) == _pSpawn)
		{
			m_vecSpawnLoad.erase(The);
			if(!m_vecSpawnLoad.size())
			{
				m_vecSpawnLoad.clear();
			}
		}
	}
}

VOID CSpawnManager::DestroySelectedSpawn()
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	// Destroy loaded spawns
	vector<CSpawn*>::iterator The = m_vecSpawnLoad.begin();
	while(The != m_vecSpawnLoad.end())
	{
		RwBool IsExisted = FALSE;

		for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
		{
			if((*The) == m_vecSpawnSelect[i])
			{
				IsExisted = TRUE;
				break;
			}
		}

		if(IsExisted)
		{
			The = m_vecSpawnLoad.erase(The);
			if(!m_vecSpawnLoad.size())
			{
				m_vecSpawnLoad.clear();
			}
		}
		else
		{
			++The;
		}
	}

	// Destroy spawn instance
	for(int i = 0; i < static_cast<int>(m_vecSpawnSelect.size()); ++i)
	{
		GetSpawnContainer()->DeleteSpawnInst(m_vecSpawnSelect[i], pMgr);
	}

	// Destroy selected spawn
	m_vecSpawnSelect.clear();
}

VOID CSpawnManager::MoveSpawn()
{
	CNtlWorldFieldManager*	pMgr			= dGETMGR();
	float					DistX			= dGET_MOUSE()->GetXMovementWithRate(10.0f);
	float					DistY			= dGET_MOUSE()->GetYMovementWithRate(10.0f);
	RwFrame*				pCameraFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
	RwMatrix*				pM				= RwFrameGetMatrix(pCameraFrame);
	RwV3d*					pRight			= RwMatrixGetRight(pM);
	RwV3d*					pUp				= RwMatrixGetUp(pM);
	RwV3d*					pAt				= RwMatrixGetAt(pM);
	RwV3d					vFront, vRight;

	if(!dKEY(DIK_Z))
	{
		CNtlMath::MathRwV3dAssign(&vFront, pAt->x, 0.0f, pAt->z);
		CNtlMath::MathRwV3dAssign(&vRight, pRight->x, 0.0f, pRight->z);
		RwV3dNormalize(&vFront, &vFront);
		RwV3dNormalize(&vRight, &vRight);

		for(int i = 0; i < static_cast<int>(m_vecSpawnSelect.size()); ++i)
		{
			CNtlPLCharacter*	pNtlPLChr	= static_cast<CNtlPLCharacter*>(m_vecSpawnSelect[i]->GetNtlPLEntity());
			RwV3d				PosCur		= pNtlPLChr->GetPosition();

			RwReal TotalHeight		= dGET_POSITIVE_VALUE(PosCur.y);
			RwReal TerrainHeight	= dGET_POSITIVE_VALUE(pMgr->GetWorldSectorHeight(PosCur));
			RwReal DiffHeight		= (TotalHeight - TerrainHeight);

			RwV3dIncrementScaled(&PosCur, &vFront, -DistY);
			RwV3dIncrementScaled(&PosCur, &vRight, -DistX);
			PosCur.y = pMgr->GetWorldSectorHeight(PosCur) + DiffHeight;
			
			GetSpawnContainer()->MoveSpawn(PosCur, m_vecSpawnSelect[i], pMgr, m_CurSpawnType);
		}
	}
	else
	{
		for(int i = 0; i < static_cast<int>(m_vecSpawnSelect.size()); ++i)
		{
			CNtlPLCharacter*	pNtlPLChr	= static_cast<CNtlPLCharacter*>(m_vecSpawnSelect[i]->GetNtlPLEntity());
			RwV3d				PosCur		= pNtlPLChr->GetPosition();
			PosCur.y						-= DistY;

			GetSpawnContainer()->MoveSpawn(PosCur, m_vecSpawnSelect[i], pMgr, m_CurSpawnType);
		}
	}
}

VOID CSpawnManager::RotateSpawn()
{
	CNtlWorldFieldManager*	pMgr	= dGETMGR();
	float					DistX	= dGET_MOUSE()->GetXMovementWithRate(1.0f);

	for(int i = 0; i < static_cast<int>(m_vecSpawnSelect.size()); ++i)
	{
		m_vecSpawnSelect[i]->Rotate(DistX);
	}
}

VOID CSpawnManager::DeleteSpawnLoadAtField(RwInt32 FieldIdx, vector<CSpawn*>& _vecSpawnLoad)
{
	m_pSpawnContainer->DeleteSpawnLoadAtField(FieldIdx, _vecSpawnLoad);
}

VOID CSpawnManager::InsertSpawnLoadAtField(RwInt32 FieldIdx, vector<CSpawn*>& _vecSpawnLoad)
{
	m_pSpawnContainer->InsertSpawnLoadAtField(FieldIdx, _vecSpawnLoad);
}

RwInt32 CSpawnManager::GetPathIndex()
{
	if(m_vecSpawnSelect.size() != 1)
	{
		return -1;
	}

	return m_vecSpawnSelect[0]->GetPathIndex();
}

VOID CSpawnManager::SetPathIndex(RwInt32 _PathIndex)
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

// 	if(m_vecSpawnSelect.size() != 1)
// 	{
// 		return;
// 	}

	for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
	{
		m_vecSpawnSelect[i]->SetPathIndex(_PathIndex);
	}
}

VOID CSpawnManager::SetNestMemberCnt(RwInt32 _Cnt)
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	//if(m_vecSpawnSelect.size() != 1)
	//{
	//	return;
	//}
	
	for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
	{
		if (m_vecSpawnSelect[i]->GetSpawnParty())
		{
			continue;
		}
		m_vecSpawnSelect[i]->SetNestMembers(_Cnt, pMgr);
	}
}

RwInt32 CSpawnManager::GetCurNestMemberCnt()
{
	return m_vecSpawnSelect[0]->GetNestMemberCnt();
}

VOID CSpawnManager::OnCreatePartyLeader()
{
	if(m_vecSpawnSelect.size() != 1)
	{
		::AfxMessageBox("PARTY LEADER mode is only available only for one spawn selected.", MB_OK);
		return;
	}

	if(	/*m_vecSpawnSelect[0]->GetNestRange() != 1 ||*/
		m_vecSpawnSelect[0]->GetNestMemberCnt() != 1)
	{
		::AfxMessageBox("NEST RANGE & MEMBER COUNT have to be 1.", MB_OK);
		return;
	}

	if(!m_vecSpawnSelect[0]->GetSpawnParty())
	{
		::AfxMessageBox("The selected spawn must have a PARTY.", MB_OK);
		return;
	}

	int		Ret			= ::AfxMessageBox("Create a LEADER for the current selected spawn?; NO will destroy the leader.", MB_YESNOCANCEL);
	CSpawn*	pCurSpawn	= m_vecSpawnSelect[0];

	if(Ret == IDYES)
	{
		pCurSpawn->SetPartyLeader(TRUE);
		pCurSpawn->RefreshNameBox();
	}
	else if(Ret == IDNO)
	{
		pCurSpawn->SetPartyLeader(FALSE);
		pCurSpawn->RefreshNameBox();
	}
}

VOID CSpawnManager::OnCreatePartyFollow()
{
	if(m_vecSpawnSelect.size() != 1)
	{
		::AfxMessageBox("PARTY FOLLOW mode is only available for one spawn selected.", MB_OK);
		return;
	}

	if(	m_vecSpawnSelect[0]->GetNestRange() != 1 ||
		m_vecSpawnSelect[0]->GetNestMemberCnt() != 1)
	{
		::AfxMessageBox("NEST RANGE & MEMBER COUNT have to be 1.", MB_OK);
		return;
	}

	if(!m_vecSpawnSelect[0]->GetSpawnParty())
	{
		::AfxMessageBox("The selected spawn must have a PARTY.", MB_OK);
		return;
	}

	if(!m_vecSpawnSelect[0]->GetSpawnParty()->GetLeader())
	{
		::AfxMessageBox("The selected spawn must have a LEADER.", MB_OK);
		return;
	}

	int		Ret			= ::AfxMessageBox("Create a FOLLOW for the current selected spawn?; NO will destroy the FOLLOW.", MB_YESNOCANCEL);
	CSpawn*	pCurSpawn	= m_vecSpawnSelect[0];

	if(Ret == IDYES)
	{
		pCurSpawn->SetPartyFollow(TRUE);
		pCurSpawn->RefreshNameBox();
	}
	else if(Ret == IDNO)
	{
		pCurSpawn->SetPartyFollow(FALSE);
		pCurSpawn->RefreshNameBox();
	}
}

VOID CSpawnManager::OnCreateParty()
{
	if(!m_vecSpawnSelect.size())
	{
		::AfxMessageBox("Select spawns at least more than one.", MB_OK);
		return;
	}
	
	if (m_vecSpawnSelect.size() > 5)
	{
		::AfxMessageBox("Select spawns size error.(MAX:5)", MB_OK);
		return;
	}

	int Ret = ::AfxMessageBox("Create a PARTY for the current selected spawns?; NO will destroy the party.", MB_YESNOCANCEL);
	if(Ret == IDYES)
	{
		RwBool			NewPartyUsedAtLeastOnce = FALSE;
		CSpawnParty*	pSpawnParty				= NTL_NEW CSpawnParty;
		RwBool			GetColorFlag			= FALSE;
		RwRGBA			RGBA;

		for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
		{
			CSpawn*	pCurSpawn = m_vecSpawnSelect[i];

			if(pCurSpawn->GetNestMemberCnt() == 1/* && pCurSpawn->GetNestRange() == 1*/)
			{
				NewPartyUsedAtLeastOnce = TRUE;
				pCurSpawn->CreateParty(pSpawnParty);

				if(!GetColorFlag)
				{
					GetColorFlag = TRUE;
					RGBA = CSpawnPartyContainer::GetNextPartyColor();
					pSpawnParty->SetColor(RGBA);	
				}

				pCurSpawn->SetPartyColor(RGBA);
				pCurSpawn->RefreshNameBox();
			}
			else
			{
				continue;
			}
		}

		if(!NewPartyUsedAtLeastOnce)
		{
			NTL_DELETE(pSpawnParty);
		}
	}
	else if(Ret == IDNO)
	{
		for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
		{
			CSpawn*	pCurSpawn = m_vecSpawnSelect[i];

			if(pCurSpawn->GetNestMemberCnt() == 1/* && pCurSpawn->GetNestRange() == 1*/)
			{
				pCurSpawn->DestroyParty();

				RwRGBA RGBA = {255, 255, 255, 255};
				pCurSpawn->SetPartyColor(RGBA);
			}
			else
			{
				continue;
			}
		}
	}
}

RwBool CSpawnManager::SetSpawnMacroToCurSpawns(CSpawnMacro& _SpawnMacro)
{
	if(!m_vecSpawnSelect.size())
	{ 
		return FALSE;
	}

	for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
	{
		m_vecSpawnSelect[i]->SetSpawnMacro(_SpawnMacro);
	}

	return TRUE;
}

VOID CSpawnManager::OnDeleteSpawn(RwV3d& _Pos)
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	// destroy loaded spawns if exists
	vector<CSpawn*>::iterator The = m_vecSpawnLoad.begin();
	while(The != m_vecSpawnLoad.end())
	{
		RwBool	IsExisted = FALSE;
		CSpawn*	pCurSpawn = static_cast<CSpawn*>(*The);

		if(	static_cast<RwInt32>(pCurSpawn->m_T.x) == static_cast<RwInt32>(_Pos.x) &&
			static_cast<RwInt32>(pCurSpawn->m_T.y) == static_cast<RwInt32>(_Pos.y) &&
			static_cast<RwInt32>(pCurSpawn->m_T.z) == static_cast<RwInt32>(_Pos.z))

		{
			IsExisted = TRUE;
		}

		if(IsExisted)
		{
			The = m_vecSpawnLoad.erase(The);
			if(!m_vecSpawnLoad.size())
			{
				m_vecSpawnLoad.clear();
			}

			break;
		}
		else
		{
			++The;
		}
	}

	m_pSpawnContainer->DeleteSpawnInst(_Pos, pMgr);
}

VOID CSpawnManager::OnPortalTo(RwV3d& _Pos)
{
	GetSceneManager()->GetWorld()->SetPortalPosition(_Pos);
}

VOID CSpawnManager::Update()
{
	CNtlWorldFieldManager *pMgr = dGETMGR();

	// update current loaded spawns
	for(RwUInt32 i = 0; i < m_vecSpawnLoad.size(); ++i)
	{
		m_vecSpawnLoad[i]->Update();
	}

	if(m_PathModeActive || m_NavigatorModeActive)
	{
		m_pPathHandler->Update();

		return;
	}

	if(dOKEY(DIK_DELETE))
	{
		DestroySelectedSpawn();
	}

	switch(dGET_SPAWN_MODE())
	{
	// selection mode; one or more files can be selected
	case eSEM_IDLE:
		{
			// wanna move current captured doodads
			if(dKEY(DIK_LSHIFT) && dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				dGET_SPAWN_MODE() = eSEM_EDIT;
				return;
			}

			// wanna rotate current captured doodads
			if(dKEY(DIK_SPACE) && dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				dGET_SPAWN_MODE() = eSEM_EDIT;
				return;
			}

			if (dKEY(DIK_SPACE))
			{
				PopAllSpawnCaptured();
				return;
			}

			// wanna change m_NestRange or m_NestSpawnCnt
			if(dGET_MOUSE()->GetWheelMovement())
			{
				dGET_SPAWN_MODE() = eSEM_EDIT;
				return;
			}

			// wanna change m_NestRange or m_NestSpawnCnt
			if(dGET_MOUSE()->GetYMovementWithRate(10.0f) && dKEY(DIK_LCONTROL))
			{
				dGET_SPAWN_MODE() = eSEM_EDIT;
				return;
			}

			if ((dKEY(DIK_PGDN) || dKEY(DIK_PGDN)) && dKEY(DIK_LCONTROL))
			{
				dGET_SPAWN_MODE() = eSEM_EDIT;
				return;
			}

			// Push LControl -> SPAWN_MODE = eSEM_EDIT
			/*if(dKEY(DIK_LCONTROL))
			{
				dGET_SPAWN_MODE() = eSEM_EDIT;
				return;
			}*/

			if(dGET_MOUSE()->GetLBUp())
			{
				NTL_DELETE(m_pBrushMultiSelector);
				NTL_DELETE(m_pBrushMultiSelectorFrustum);
				return;
			}

			if(dOKEY(DIK_P))
			{
				OnCreateParty();
			}

			if(dOKEY(DIK_L))
			{
				OnCreatePartyLeader();
			}

			if(dOKEY(DIK_F))
			{
				OnCreatePartyFollow();
			}

			if (dGET_MOUSE()->GetLBOnce() && dKEY(DIK_LCONTROL))
			{
				CNtlWorldFieldManager *pMgr = dGETMGR();

				// Find unit ray vector
				RwV2d ScreenPos;
				ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
				ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

				RwLine cameraRay;
				RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

				RwV3d RwRayDir;
				RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);
				RwV3dNormalize(&RwRayDir, &RwRayDir);

				// Pick map objects
				RpIntersection Intersection;
				Intersection.t.line	= cameraRay;
				Intersection.type	= rpINTERSECTLINE;

				// Line 2 Sphere
				sCOLLISION_PARAM CollisionParam;
				RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, WorldIntersectionSpawnCB, &CollisionParam);
				if(CollisionParam._pNtlPLEntity)
				{
					RwV3d vDist = GetSpawnIntersectionPos();
					/*vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
					vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
					vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;*/
					SPLCharacterCreateParam PLEntityCreateParam;
					PLEntityCreateParam.pPos	= &vDist;
					PLEntityCreateParam.bPcFlag = FALSE;

					CSpawnTreeElement spawnTreeElement;
					if (!GetSpawnContainer()->FindSpawnTree(GetSpawnContainer()->GetSpawn(CollisionParam._pNtlPLEntity, pMgr)->m_MdlGUID, &spawnTreeElement))
					{
						AfxMessageBox("invalid index spawn data");
					}
					else
					{
						GetSpawnInstance()->SetCurMdlInfo(spawnTreeElement.m_Name, spawnTreeElement.m_MdlLvl, spawnTreeElement.m_MdlName);

						CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, spawnTreeElement.m_MdlName.c_str(), &PLEntityCreateParam);
						DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

						static_cast<CNtlPLCharacter*>(pNtlPLEntity)->SetBaseScale(1.0f);
						static_cast<CNtlPLCharacter*>(pNtlPLEntity)->SetAngleY(0.0f);
						static_cast<CNtlPLCharacter*>(pNtlPLEntity)->SetPosition(&vDist);

						if(dGET_BRUSH_ENTITY())
						{
							GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
							dGET_BRUSH_ENTITY() = NULL;
						}

						dGET_BRUSH_ENTITY() = pNtlPLEntity;
						dGET_SPAWN_MODE()	= eSEM_PLACE;
					}

					return;
				}
			}

			if(dKEY(DIK_LALT) && dGET_MOUSE()->GetLBOnce())	
			{
				// Find unit ray vector
				RwV2d ScreenPos;
				ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
				ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

				RwLine cameraRay;
				RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

				RwV3d RwRayDir;
				RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);
				RwV3dNormalize(&RwRayDir, &RwRayDir);

				// Pick map objects
				RpIntersection Intersection;
				Intersection.t.line	= cameraRay;
				Intersection.type	= rpINTERSECTLINE;

				// Line 2 Sphere
				sCOLLISION_PARAM CollisionParam;
				RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, WorldIntersectionSpawnCB, &CollisionParam);
				if(CollisionParam._pNtlPLEntity)
				{
					// one click multi selecting
					CSpawn* pSpawn	= m_pSpawnContainer->GetSpawn(CollisionParam._pNtlPLEntity, pMgr);
					if(GetSpawnCaptured(pSpawn))
					{
						PopSpawnCaptured(pSpawn);
					}
					else
					{
						PushSpawnSelected(pSpawn);
					}
				}
			}
			else if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE) && !dKEY(DIK_LCONTROL))
			{
				// first time selector create
// 				if(!m_pBrushMultiSelector)
// 				{
// 					PopAllSpawnCaptured();
// 
// 					if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 					{
// 						m_pBrushMultiSelector = NTL_NEW CNtlWorldBrush;
// 
// 						CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
// 						m_pBrushMultiSelector->SetController(pController);
// 
// 						CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
// 						pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
// 					}
// 					else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 					{
// 						m_pBrushMultiSelector = NTL_NEW CNtlWorldBrush;
// 
// 						CNtlWorldBrushController *pController = new CNTLWB_Frustum<CNtlWorldFieldManager>(eNBT_FRUSTUM, pMgr);
// 						m_pBrushMultiSelector->SetController(pController);
// 
// 						RwV2d vPos;
// 						vPos.x = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().x);
// 						vPos.y = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().y);
// 						CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
// 						pCurBrush->SetPosS(vPos);
// 					}	
// 				}
// 
// 				if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 				{
// 					CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
// 					pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
// 
// 					// loop current loaded spawns & check they are being inside rectangle
// 					for(RwUInt32 i = 0; i < m_vecSpawnLoad.size(); ++i)
// 					{
// 						if(GetSpawnCapture(m_vecSpawnLoad[i]->m_T))
// 						{
// 							if(!GetSpawnCaptured(m_vecSpawnLoad[i]))
// 							{
// 								m_vecSpawnLoad[i]->SetNestArrow(eSAT_NEST_SELECT);
// 								m_vecSpawnSelect.push_back(m_vecSpawnLoad[i]);
// 							}
// 						}
// 						else
// 						{
// 							if(GetSpawnCaptured(m_vecSpawnLoad[i]))
// 							{
// 								PopSpawnCaptured(m_vecSpawnLoad[i]);
// 							}
// 						}
// 					}
// 				}
// 				else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 				{
// 					RwV2d vPos;
// 					vPos.x = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().x);
// 					vPos.y = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().y);
// 					CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
// 					pCurBrush->SetPosE(vPos);
// 
// 					pCurBrush->CalcFrustumData();
// 					for(RwUInt32 i = 0; i < m_vecSpawnLoad.size(); ++i)
// 					{
// 						if(pCurBrush->FrustumTest(m_vecSpawnLoad[i]->m_T))
// 						{
// 							if(!GetSpawnCaptured(m_vecSpawnLoad[i]))
// 							{
// 								m_vecSpawnLoad[i]->SetNestArrow(eSAT_NEST_SELECT);
// 								m_vecSpawnSelect.push_back(m_vecSpawnLoad[i]);
// 							}
// 						}
// 						else
// 						{
// 							if(GetSpawnCaptured(m_vecSpawnLoad[i]))
// 							{
// 								PopSpawnCaptured(m_vecSpawnLoad[i]);
// 							}
// 						}
// 					}
// 				}	

				if (!m_pBrushMultiSelector)
				{
					PopAllSpawnCaptured();

					m_pBrushMultiSelector = NTL_NEW CNtlWorldBrush;

					CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
					m_pBrushMultiSelector->SetController(pController);

					CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
					pCurBrush->SetPosS(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);
				}
				if (!m_pBrushMultiSelectorFrustum)
				{
					PopAllSpawnCaptured();

					m_pBrushMultiSelectorFrustum = NTL_NEW CNtlWorldBrush;

					CNtlWorldBrushController *pController = new CNTLWB_Frustum<CNtlWorldFieldManager>(eNBT_FRUSTUM, pMgr);
					m_pBrushMultiSelectorFrustum->SetController(pController);

					RwV2d vPos;
					vPos.x = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().x);
					vPos.y = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().y);
					CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushMultiSelectorFrustum->GetController());
					pCurBrush->SetPosS(vPos);
				}			

				// set end point				
				if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD || CNtlPLWorldState::GetActiveMiniIndoor())
				{
					RwV2d vPos;
					vPos.x = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().x);
					vPos.y = static_cast<RwReal>(dGET_MOUSE()->GetCusPos().y);
					CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushMultiSelectorFrustum->GetController());
					pCurBrush->SetPosE(vPos);

					pCurBrush->CalcFrustumData();
					for(RwUInt32 i = 0; i < m_vecSpawnLoad.size(); ++i)
					{
						if(pCurBrush->FrustumTest(m_vecSpawnLoad[i]->m_T))
						{
							if(!GetSpawnCaptured(m_vecSpawnLoad[i]))
							{
								m_vecSpawnLoad[i]->SetNestArrow(eSAT_NEST_SELECT);
								m_vecSpawnSelect.push_back(m_vecSpawnLoad[i]);
							}
						}
						else
						{
							if(GetSpawnCaptured(m_vecSpawnLoad[i]))
							{
								PopSpawnCaptured(m_vecSpawnLoad[i]);
							}
						}
					}
				}
				else if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
				{
					CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
					pCurBrush->SetPosE(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);

					// loop current loaded spawns & check they are being inside rectangle
					for(RwUInt32 i = 0; i < m_vecSpawnLoad.size(); ++i)
					{
						if(GetSpawnCapture(m_vecSpawnLoad[i]->m_T))
						{
							if(!GetSpawnCaptured(m_vecSpawnLoad[i]))
							{
								m_vecSpawnLoad[i]->SetNestArrow(eSAT_NEST_SELECT);
								m_vecSpawnSelect.push_back(m_vecSpawnLoad[i]);
							}
						}
						else
						{
							if(GetSpawnCaptured(m_vecSpawnLoad[i]))
							{
								PopSpawnCaptured(m_vecSpawnLoad[i]);
							}
						}
					}
				}
			}
		}
		break;

	// placing mode; can place one or more objects at the same time
	case eSEM_PLACE:
		{
			if(dOKEY(DIK_SPACE))
			{
				if(dGET_BRUSH_ENTITY())
				{
					GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
					dGET_BRUSH_ENTITY() = NULL;
					dGET_SPAWN_MODE()	= eSEM_IDLE;
					return;
				}
			}

			if(dGET_MOUSE()->GetLBOnce())
			{
				CreateEntity();
			}
	
			// object moving with picking variation
			dGET_BRUSH_ENTITY()->SetPosition(&GetSpawnIntersectionPos());
		}
		break;

	// editing mode; for one or more objects
	case eSEM_EDIT:
		{
			static RwBool LControlPush = FALSE;

			if(LControlPush)
			{
				if(!dKEY(DIK_LCONTROL))
				{
					for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
					{
						if(!m_vecSpawnSelect[i]->GetSpawnParty())
						{
							m_vecSpawnSelect[i]->RefreshNestMembers(pMgr);
						}
					}

					LControlPush = FALSE;
				}
			}

			// wanna stop editing mode
			if(dGET_MOUSE()->GetLBUp())
			{
				dGET_SPAWN_MODE() = eSEM_IDLE;

				for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
				{
					if(!m_vecSpawnSelect[i]->GetSpawnParty())
					{
						m_vecSpawnSelect[i]->RefreshNestMembers(pMgr);
					}
				}

				LControlPush = FALSE;

				LControlPush = FALSE;

				return;
			}

			if(dKEY(DIK_LSHIFT) && !dKEY(DIK_X) && dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				MoveSpawn();
			}

			if(dKEY(DIK_LSHIFT) && dKEY(DIK_X) && dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
			{
				RotateSpawn();
			}

			float	DistY		  = dGET_MOUSE()->GetYMovementWithRate(10.0f);
			RwInt32 WheelMoveDist = static_cast<RwInt32>(dGET_MOUSE()->GetWheelMovement());
			if(/*m_vecSpawnSelect.size() == 1 &&*/dKEY(DIK_LCONTROL))
			{
				for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
				{
					if(WheelMoveDist)
					{
						//if(!m_vecSpawnSelect[i]->GetSpawnParty())
						{
							if(WheelMoveDist > 0)
							{
								m_vecSpawnSelect[i]->GetNestRange() += 1;
							}
							else
							{
								m_vecSpawnSelect[i]->GetNestRange() -= 1;
							}

							CLAMP(m_vecSpawnSelect[i]->GetNestRange(), 1, dGET_WORLD_PARAM()->WorldSectorSize);
						}
					}
					else if(DistY)
					{
						//if(!m_vecSpawnSelect[i]->GetSpawnParty())
						{
							if(DistY >= 0.0f)
							{
								m_vecSpawnSelect[i]->GetNestRange() += 2;
							}
							else
							{
								m_vecSpawnSelect[i]->GetNestRange() -= 2;
							}

							CLAMP(m_vecSpawnSelect[i]->GetNestRange(), 1, dGET_WORLD_PARAM()->WorldSectorSize);
						}
					}
					m_vecSpawnSelect[i]->RefreshNameBox();
				}
				LControlPush = TRUE;
			}
			if (dKEY(DIK_PGUP) && dKEY(DIK_LCONTROL))
			{
				RwInt32 iRenge = -1;
				for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
				{
					if (iRenge == -1)
					{
						iRenge = m_vecSpawnSelect[i]->GetNestRange();
					}
					else if (m_vecSpawnSelect[i]->GetNestRange() > iRenge)
					{
						iRenge = m_vecSpawnSelect[i]->GetNestRange();
					}
				}
				for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
				{
					m_vecSpawnSelect[i]->GetNestRange() = iRenge;
					m_vecSpawnSelect[i]->RefreshNameBox();
				}
				LControlPush = TRUE;
			}
			if (dKEY(DIK_PGDN) && dKEY(DIK_LCONTROL))
			{
				RwInt32 iRenge = -1;
				for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
				{
					if (iRenge == -1)
					{
						iRenge = m_vecSpawnSelect[i]->GetNestRange();
					}
					else if (m_vecSpawnSelect[i]->GetNestRange() < iRenge)
					{
						iRenge = m_vecSpawnSelect[i]->GetNestRange();
					}
				}
				for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
				{
					m_vecSpawnSelect[i]->GetNestRange() = iRenge;
					m_vecSpawnSelect[i]->RefreshNameBox();
				}
				LControlPush = TRUE;
			}
		}
		break;
	}

	// Update the first spawn obj.
	if(m_vecSpawnSelect.size() >= 1)
	{
		CNtlPLCharacter*	pNtlPLChr	= static_cast<CNtlPLCharacter*>(m_vecSpawnSelect[0]->GetNtlPLEntity());
		RwV3d				Pos			= pNtlPLChr->GetPosition();
		RwReal				Rot			= pNtlPLChr->GetAngleY();
		CSpawnMacro			Macro		= m_vecSpawnSelect[0]->GetSpawnMacro();
		RwV3d				Front;
		RwMatrix			Matrix;

		RwMatrixRotate(&Matrix, &CNtlPLGlobal::m_vYAxisV3, Rot, rwCOMBINEREPLACE);
		RwV3dTransformVector(&Front, &CNtlPLGlobal::m_vZAxisV3, &Matrix);
		RwV3dNormalize(&Front, &Front);

		dGETFRM()->m_pSpawnPalette->m_pPaletteSpawnBase->Refresh(Pos, Front, Macro, m_vecSpawnSelect[0]);
	}
}

RpAtomic* CSpawnManager::NestUIDrawAllAtomicCB2(RpAtomic* _pAtomic, void* _pData)
{
	CNtlPLEntity*	pPLEntity		=	static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(_pAtomic));
	CSpawnManager*	pSpawnManager	=	static_cast<CSpawnManager*>(_pData);

	if(pPLEntity->GetClassType() != PLENTITY_CHARACTER)
	{
		pSpawnManager->m_sIsClumpNest._pNtlPLEntity = NULL;

		return _pAtomic;
	}

	if(pPLEntity->GetFlags() & NTL_PLEFLAG_NEST_MEMBER)
	{
		pSpawnManager->m_sIsClumpNest._pNtlPLEntity = NULL;

		return _pAtomic;
	}

	pSpawnManager->m_sIsClumpNest._pNtlPLEntity = pPLEntity;

	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	CSpawn*					pSpawn		= NULL;
	RwInt32					NestRange;
	RwV3d					NestPos;
	RwUInt32				Color;

	if(pSpawnManager->m_sIsClumpNest._pNtlPLEntity && pSpawnManager->m_sIsClumpNest._pNtlPLEntity != dGET_BRUSH_ENTITY())
	{
 		pSpawn		= pSpawnManager->GetSpawnContainer()->GetSpawn(pSpawnManager->m_sIsClumpNest._pNtlPLEntity, pMgr);
		NestPos		= static_cast<CNtlPLCharacter*>(pSpawnManager->m_sIsClumpNest._pNtlPLEntity)->GetPosition();
		NestRange	= static_cast<RwInt32>(pSpawn->GetSpawnMacro().m_WanderRange);
		Color		= 0xFF0000FF;

		for (int i = 0; i < (int)pSpawnManager->m_vecSpawnSelect.size(); ++i)
		{
			if (pSpawnManager->m_vecSpawnSelect.at(i) == pSpawn)
			{
				NestRange	= pSpawn->GetNestRange();
				Color		= 0xFFFFFFFF;
				break;
			}
		}

		if (pSpawnManager->IsVisibleCircle())
		{
			pSpawnManager->RenderNestCircle(NestPos, NestRange, Color/*((pSpawn->m_MdlGUID * 10) | 0xFF000000)*/);
		}

		if (pSpawnManager->IsVisibleCircleScan() && pSpawn->GetSpawnType() == eSPAWN_MOB)
		{
			CMobTable*		pMobTbl = static_cast<CMobTable*>(API_GetTableContainer()->GetMobTable());
			CNPCTable*		pNpcTbl = static_cast<CNPCTable*>(API_GetTableContainer()->GetNpcTable());
			sMOB_TBLDAT*	pTlbData = (sMOB_TBLDAT*)pMobTbl->FindData(pSpawn->m_MdlGUID);
			if (pTlbData)
			{
				Color		= 0xFFFF0000;
				NestRange	= pTlbData->wScan_Range;
				pSpawnManager->RenderNestCircle(NestPos, NestRange, Color/*((pSpawn->m_MdlGUID * 10) | 0xFF000000)*/);
			}
		}
	}

	return _pAtomic;
}

RpAtomic* CSpawnManager::NestUIDrawAllAtomicCB(RpAtomic* _pAtomic, void* _pData)
{
	CNtlPLEntity*	pPLEntity		=	static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(_pAtomic));
	CSpawnManager*	pSpawnManager	=	static_cast<CSpawnManager*>(_pData);

	if(pPLEntity->GetClassType() != PLENTITY_CHARACTER)
	{
		pSpawnManager->m_sIsClumpNest._pNtlPLEntity = NULL;

		return _pAtomic;
	}

	if(pPLEntity->GetFlags() & NTL_PLEFLAG_NEST_MEMBER)
	{
		pSpawnManager->m_sIsClumpNest._pNtlPLEntity = NULL;

		return _pAtomic;
	}

	/*
	메인 캐릭터 포인터를 얻고 싶을때
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar()
	CNtlSobProxy *pSobProxy = *pSobAvatar->GetSobProxy();
	CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
	*/

	pSpawnManager->m_sIsClumpNest._pNtlPLEntity = pPLEntity;

	return _pAtomic;
}

RpClump* CSpawnManager::NestUIDrawAllClumpCB(RpClump* _pClump, void* _pData)
{
	CNtlWorldFieldManager*	pMgr			= dGETMGR();
	CSpawnManager*			pSpawnManager	= static_cast<CSpawnManager*>(_pData);

	RpClumpForAllAtomics(_pClump, NestUIDrawAllAtomicCB, _pData);

	CSpawn* pSpawn = NULL;
	RwInt32	NestRange;
	RwV3d	NestPos;

	if(pSpawnManager->m_sIsClumpNest._pNtlPLEntity && pSpawnManager->m_sIsClumpNest._pNtlPLEntity != dGET_BRUSH_ENTITY())
	{
		pSpawn		= pSpawnManager->GetSpawnContainer()->GetSpawn(pSpawnManager->m_sIsClumpNest._pNtlPLEntity, pMgr);
		NestPos		= static_cast<CNtlPLCharacter*>(pSpawnManager->m_sIsClumpNest._pNtlPLEntity)->GetPosition();
		NestRange	= pSpawn->GetNestRange();
		pSpawnManager->RenderNestCircle(NestPos, NestRange);
	}

	return _pClump;
}

RwInt32	CSpawnManager::GetSpawnCnt(RwInt32 _FieldIdx)
{
	return m_pSpawnContainer->GetSpawnCnt(_FieldIdx);
}

VOID CSpawnManager::Save()
{
	// Save dialog
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(FALSE, "Save SPAWN files", "*.spn", OFN_HIDEREADONLY, "SPAWN Files (*.spn)|*.spn||");
	FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER\\Spawn";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	// Create file and ready 2 save
	string	FileTitle	= FileDlg.GetFileTitle().GetBuffer(0); 
	string  FileExt		= ".spn";

	FileTitle = FileTitle + FileExt;

	string strReq		= "Save FileName : "; strReq += FileTitle;
	if (AfxMessageBox(strReq.c_str(), MB_YESNO) == IDNO)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	SaveIntoFile(FileTitle.c_str());
	::SetCurrentDirectory(dirBackup);
}

VOID CSpawnManager::SaveIntoFile(const char* pFilemame)
{
	FILE*	pFile		= NULL;
	if(::fopen_s(&pFile, pFilemame, "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << pFilemame << ")");
		AfxMessageBox("Save Failed");		
		return;
	}

	// Macro
	//CSpawnMacroManager::Save(pFile);

	// Spawn type
	fwrite(&m_CurSpawnType,	sizeof(RwInt32), 1, pFile);

	// Spawn container
	m_pSpawnContainer->Save(pFile);

	::fclose(pFile);
}

VOID CSpawnManager::Load()
{
	if(AfxMessageBox("Current spawns will be ignored. Would you like to go on?", MB_YESNO) == IDNO)
	{
		return;
	}

	CNtlWorldFieldManager* pMgr = dGETMGR();

	// Load dialog
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE, "Load SPAWN files", "*.spn", OFN_HIDEREADONLY, "SPAWN Files (*.spn)|*.spn||");
	FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER\\Spawn";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	// Load file and ready 2 load
	string	FileTitle	= FileDlg.GetFileTitle().GetBuffer(0); 
	string  FileExt		= ".spn";

	FileTitle = FileTitle + FileExt;

	LoadFromFile(FileTitle.c_str());
	::SetCurrentDirectory(dirBackup);

	// Refresh current fields
	RWS::CMsg Msg;
	HandleEvents(Msg);

	DestroyEntitiesAtCurFields();
	CreateEntitiesAtCurFields();
}

VOID CSpawnManager::LoadFromFile(const char* pFilemame)
{
	FILE*	pFile		= NULL;

	if(::fopen_s(&pFile, pFilemame, "rb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << pFilemame << ")");
		return;
	}

	Create(m_CurSpawnType);

	// Spawn type
	RwInt32 CurSpawnType;
	fread(&CurSpawnType, sizeof(RwInt32), 1, pFile);

	if(CurSpawnType != static_cast<RwInt32>(m_CurSpawnType))
	{
		AfxMessageBox("The file was used for different spawn type.", MB_OK);
		return;
	}

	// Spawn container
	m_pSpawnContainer->Load(pFile, m_CurSpawnType, dGETMGR());

	// Refresh current fields
// 	RWS::CMsg Msg;
// 	HandleEvents(Msg);
// 
// 	DestroyEntitiesAtCurFields();
// 	CreateEntitiesAtCurFields();

	::fclose(pFile);
}

VOID CSpawnManager::SetGroupIdx4SpawnSelected(DWORD _GroupIdx)
{
	for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
	{
		m_vecSpawnSelect[i]->SetGroupIdx(_GroupIdx);	
	}
}

VOID CSpawnManager::SetPlayScriptIdx4SpawnSelected(DWORD _PlayScriptIdx)
{
	for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
	{
		m_vecSpawnSelect[i]->SetPlayScriptIdx(_PlayScriptIdx);	
	}
}

VOID CSpawnManager::SetPlayScriptSceneIdx4SpawnSelected(DWORD _PlayScriptSceneIdx)
{
	for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
	{
		m_vecSpawnSelect[i]->SetPlayScriptSceneIdx(_PlayScriptSceneIdx);	
	}
}

VOID CSpawnManager::SetAIScriptIdx4SpawnSelected(DWORD _AIScriptIdx)
{
	for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
	{
		m_vecSpawnSelect[i]->SetAIScriptIdx(_AIScriptIdx);	
	}
}

VOID CSpawnManager::SetAIScriptSceneIdx4SpawnSelected(DWORD _AIScriptSceneIdx)
{
	for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
	{
		m_vecSpawnSelect[i]->SetAIScriptSceneIdx(_AIScriptSceneIdx);	
	}
}

VOID CSpawnManager::Export()
{
	if(AfxMessageBox("Would you like to create a spawn table?", MB_YESNO) == IDNO)
	{
		return;
	}

	CNtlWorldFieldManager* pMgr = dGETMGR();

	// Export dialog
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(FALSE, "Export SPAWN table", "*.xml", OFN_HIDEREADONLY, "SPAWN table (*.xml)|*.xml||");
	FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER\\Spawn";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	FILE*	pFile		= NULL;
	string	FileTitle	= FileDlg.GetFileTitle().GetBuffer(0); 
	string  FileExt		= ".xml";

	FileTitle = FileTitle + FileExt;

	CSpawnXML SpawnXML;
	SpawnXML.CreateSpawnTbl(FileTitle, this);
	SpawnXML.DestroySpawnTbl();

	dGETFRM()->m_pSpawnPalette->Refresh();

	::SetCurrentDirectory(dirBackup);
}

VOID CSpawnXML::CreateSpawnNode(CSpawn* pSpawn, CSpawnManager* _pMgr, DWORD _CurTblIdx)
{
	USES_CONVERSION;

	IXMLDOMElement* pTable_Data_KOR = NULL;
	IXMLDOMElement* pFieldElem[24];
	IXMLDOMText*    pFieldText[24];

	m_pXMLDocument->createElement(L"Table_Data_KOR", &pTable_Data_KOR);

	WCHAR Name[40] = {0,};  
	for(RwInt32 i = 0; i < 24; ++i)
	{
		swprintf_s(Name, L"F%d", (i + 1));
		m_pXMLDocument->createElement(Name, &pFieldElem[i]);
	}

	DWORD	TblIdx = 0;
	RwChar	Tmp[128] = {0,};  

	// 1 : Tblidx
	sprintf_s(Tmp, 128, "%d", _CurTblIdx);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[0]);

	// 2 : Spawn_Loc_X
	sprintf_s(Tmp, 128, "%.2f", pSpawn->m_T.x);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[1]);

	// 3 : Spawn_Loc_Y
	sprintf_s(Tmp, 128, "%.2f", pSpawn->m_T.y);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[2]);

	// 4 : Spawn_Loc_Z
	sprintf_s(Tmp, 128, "%.2f", pSpawn->m_T.z);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[3]);

	// 5 : Mob_Tblidx
	sprintf_s(Tmp, 128, "%d", pSpawn->m_MdlGUID);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[4]);

	// 6 : Spawn_Loc_Range
	sprintf_s(Tmp, 128, "%d", pSpawn->GetNestRange());
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[5]);

	// 7 : Spawn_Quantity
	sprintf_s(Tmp, 128, "%d", pSpawn->GetNestMemberCnt());
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[6]);

	// 8 : Spawn_Cool_Time
	sprintf_s(Tmp, 128, "%d", pSpawn->GetSpawnMacro().m_CoolTime);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[7]);

	// 9 : Spawn_Move_Type
	sprintf_s(Tmp, 128, "%d", pSpawn->GetSpawnMacro().m_MoveType);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[8]);

	RwReal		Rot = pSpawn->m_R.y;
	RwV3d		Front;
	RwMatrix	YRotMatrix;

	RwMatrixRotate(&YRotMatrix, &CNtlPLGlobal::m_vYAxisV3, Rot, rwCOMBINEREPLACE);
	RwV3dTransformVector(&Front, &CNtlPLGlobal::m_vZAxisV3, &YRotMatrix);
	RwV3dNormalize(&Front, &Front);

	// 10 : Spawn_Dir_X
	sprintf_s(Tmp, 128, "%.2f", Front.x);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[9]);

	// 11 : Spawn_Dir_Z
	sprintf_s(Tmp, 128, "%.2f", Front.z);
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[10]);

	// 12 : Wander_Range
	sprintf_s(Tmp, 128, "%d", static_cast<RwInt32>(pSpawn->GetSpawnMacro().m_WanderRange));
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[11]);

	// 13 : Move_Range
	sprintf_s(Tmp, 128, "%d", static_cast<RwInt32>(pSpawn->GetSpawnMacro().m_MoveRange));
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[12]);

	// 14 : Move_Delay_Time
	sprintf_s(Tmp, 128, "%d", static_cast<RwInt32>(pSpawn->GetSpawnMacro().m_DelayTime));
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[13]);

	// 15 : Path_Table_Index
	if(pSpawn->GetPathIndex() == -999)
	{
		m_pXMLDocument->createTextNode(L"@", &pFieldText[14]);
	}
	else
	{
		sprintf_s(Tmp, 128, "%d", pSpawn->GetPathIndex());
		m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[14]);
	}

	CSpawnParty* pSpawnParty = pSpawn->GetSpawnParty();

	if (pSpawnParty && pSpawnParty->GetLeader())
	{
		RwV3d vLeaderPos = pSpawnParty->GetLeader()->m_T;

		RwV3d vFollowDist;
		vFollowDist.x = static_cast<RwReal>(fabs(vLeaderPos.x - pSpawn->m_T.x));
		vFollowDist.y = 0.0f;
		vFollowDist.z = static_cast<RwReal>(fabs(vLeaderPos.z - pSpawn->m_T.z));
		pSpawn->SetFollowDist(vFollowDist);

		// 16 : Follow_Distance_Loc_X
		sprintf_s(Tmp, 128, "%.2f", pSpawn->GetFollowDist().x);
		m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[15]);

		// 17 : Follow_Distance_Loc_Z
		sprintf_s(Tmp, 128, "%.2f", pSpawn->GetFollowDist().z);
		m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[16]);
	}
	else
	{
		// 16 : Follow_Distance_Loc_X
		m_pXMLDocument->createTextNode(L"@", &pFieldText[15]);

		// 17 : Follow_Distance_Loc_Z
		m_pXMLDocument->createTextNode(L"@", &pFieldText[16]);
	}
	
	// 18 : Party_Index
	if(pSpawnParty)
	{
		sprintf_s(Tmp, 128, "%d", pSpawnParty->GetPartyIdx());
	}
	else
	{
		sprintf_s(Tmp, 128, "@");
	}
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[17]);

	// 19 : Party_Leader_Able
	if(pSpawnParty && pSpawn == pSpawnParty->GetLeader())
	{
		sprintf_s(Tmp, 128, "1");
	}
	else
	{
		sprintf_s(Tmp, 128, "0");
	}
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[18]);

	// 20 : Spawn_Group
	if(!pSpawn->GetGroupIdx())
	{
		sprintf_s(Tmp, 128, "@");
	}
	else
	{
		sprintf_s(Tmp, 128, "%u", pSpawn->GetGroupIdx());
	}
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[19]);

	// 21 : PlayScript_Number
	if(!pSpawn->GetPlayScriptIdx())
	{
		sprintf_s(Tmp, 128, "@");
	}
	else
	{
		sprintf_s(Tmp, 128, "%u", pSpawn->GetPlayScriptIdx());
	}
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[20]);

	// 22 : PlayScript_Scene_Number
	if(!pSpawn->GetPlayScriptSceneIdx())
	{
		sprintf_s(Tmp, 128, "@");
	}
	else
	{
		sprintf_s(Tmp, 128, "%u", pSpawn->GetPlayScriptSceneIdx());
	}
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[21]);

	// 21 : AIScript_Number
	if(!pSpawn->GetAIScriptIdx())
	{
		sprintf_s(Tmp, 128, "@");
	}
	else
	{
		sprintf_s(Tmp, 128, "%u", pSpawn->GetAIScriptIdx());
	}
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[22]);

	// 22 : AIScript_Scene_Number
	if(!pSpawn->GetAIScriptSceneIdx())
	{
		sprintf_s(Tmp, 128, "@");
	}
	else
	{
		sprintf_s(Tmp, 128, "%u", pSpawn->GetAIScriptSceneIdx());
	}
	m_pXMLDocument->createTextNode(A2W(Tmp), &pFieldText[23]);

	for(RwInt32 i = 0; i < 24; ++i)
	{
		pFieldElem[i]->appendChild(pFieldText[i], NULL);
		pTable_Data_KOR->appendChild(pFieldElem[i], NULL);
	}

	m_pRootElem->appendChild(pTable_Data_KOR, NULL);
}

VOID CSpawnXML::CreateSpawnTbl(string& _PathName, CSpawnManager* _pMgr)
{
	Create();

	IXMLDOMProcessingInstruction* pPI = NULL;        

	m_pXMLDocument->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
	m_pXMLDocument->appendChild(pPI, NULL);

	m_pXMLDocument->createElement(L"dataroot", &m_pRootElem);
	m_pRootElem->setAttribute(L"xmlns:od", (_variant_t)"urn:schemas-microsoft-com:officedata");
	m_pRootElem->setAttribute(L"generated", (_variant_t)"2006-12-29T14:55:12");
	m_pXMLDocument->appendChild(m_pRootElem, NULL);

	IXMLDOMElement* pTable_Data_KOR = NULL;
	IXMLDOMElement* pFieldElem[24];
	IXMLDOMText*    pFieldText[24];

	// Dummy Node
	m_pXMLDocument->createElement(L"Table_Data_KOR", &pTable_Data_KOR);

	WCHAR Name[40] = {0,};  
	for(RwInt32 i = 0; i < 24; ++i)
	{
		swprintf_s(Name, L"F%d", (i + 1));
		m_pXMLDocument->createElement(Name, &pFieldElem[i]);
	}

	m_pXMLDocument->createTextNode(L"Tblidx", &pFieldText[0]);
	m_pXMLDocument->createTextNode(L"Spawn_Loc_X", &pFieldText[1]);
	m_pXMLDocument->createTextNode(L"Spawn_Loc_Y", &pFieldText[2]);
	m_pXMLDocument->createTextNode(L"Spawn_Loc_Z", &pFieldText[3]);
	m_pXMLDocument->createTextNode(L"Mob_Tblidx", &pFieldText[4]);
	m_pXMLDocument->createTextNode(L"Spawn_Loc_Range", &pFieldText[5]);
	m_pXMLDocument->createTextNode(L"Spawn_Quantity", &pFieldText[6]);
	m_pXMLDocument->createTextNode(L"Spawn_Cool_Time", &pFieldText[7]);
	m_pXMLDocument->createTextNode(L"Spawn_Move_Type", &pFieldText[8]);
	m_pXMLDocument->createTextNode(L"Spawn_Dir_X", &pFieldText[9]);
	m_pXMLDocument->createTextNode(L"Spawn_Dir_Z", &pFieldText[10]);
	m_pXMLDocument->createTextNode(L"Wander_Range", &pFieldText[11]);
	m_pXMLDocument->createTextNode(L"Move_Range", &pFieldText[12]);
	m_pXMLDocument->createTextNode(L"Move_Delay_Time", &pFieldText[13]);
	m_pXMLDocument->createTextNode(L"Path_Table_Index", &pFieldText[14]);
	m_pXMLDocument->createTextNode(L"Follow_Distance_Loc_X", &pFieldText[15]);
	m_pXMLDocument->createTextNode(L"Follow_Distance_Loc_Z", &pFieldText[16]);
	m_pXMLDocument->createTextNode(L"Party_Index", &pFieldText[17]);
	m_pXMLDocument->createTextNode(L"Party_Leader_Able", &pFieldText[18]);
	m_pXMLDocument->createTextNode(L"Spawn_Group", &pFieldText[19]);
	m_pXMLDocument->createTextNode(L"PlayScript_Number", &pFieldText[20]);
	m_pXMLDocument->createTextNode(L"PlayScript_Scene_Number", &pFieldText[21]);
	m_pXMLDocument->createTextNode(L"AIScript_Number", &pFieldText[22]);
	m_pXMLDocument->createTextNode(L"AIScript_Scene_Number", &pFieldText[23]);

	for(RwInt32 i = 0; i < 24; ++i)
	{
		pFieldElem[i]->appendChild(pFieldText[i], NULL);
		pTable_Data_KOR->appendChild(pFieldElem[i], NULL);
	}

	m_pRootElem->appendChild(pTable_Data_KOR, NULL);

	// Data Node
	DWORD CurTblIdx = 0;
	RwInt32 CurPartyCntIdx = 0;
	CSpawnContainer::SPAWN_INST_NODE_ITER TheInst = _pMgr->GetSpawnContainer()->SIN_BEGIN();
	while(TheInst != _pMgr->GetSpawnContainer()->SIN_END())
	{
		vector<CSpawn*>::iterator The = TheInst->second->BEGIN();
		while(The != TheInst->second->END())
		{
			CreateSpawnNode(*The, _pMgr, ++CurTblIdx);
			++The;
		}

		++TheInst;
	}
	
	// Set indent
	IXMLDOMDocument* pXSL = NULL;
	CoCreateInstance(__uuidof(DOMDocument30), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pXSL);

	VARIANT_BOOL vBool;
	pXSL->put_async(VARIANT_FALSE);
	pXSL->load((_variant_t)L"indent.xsl", &vBool);

	VARIANT vObject;
	VariantInit(&vObject);
	vObject.vt = VT_DISPATCH;
	vObject.pdispVal = m_pXMLDocument;

	m_pXMLDocument->transformNodeToObject(pXSL, vObject);

	if(pXSL)
	{
		pXSL->Release();
		pXSL = NULL;
	}

	// Save
	if (m_pXMLDocument->save((_variant_t)_PathName.c_str()) != S_OK)
	{
		AfxMessageBox("Export Failed");
	}
}

VOID CSpawnXML::DestroySpawnTbl()
{

}

VOID CSpawnManager::Render()
{
 	if(dGET_CUR_PALETTE() != ePM_SPAWN)
	{		
		return;
	}

	for (int i = 0; i < (int)m_vecSpawnLoad.size(); ++i)
	{
		CSpawn* pSpawn = m_vecSpawnLoad.at(i);
		{
			CNtlPLEntity* pEntity = pSpawn->m_pNtlPLEntity;

			CNtlWorldFieldManager*	pMgr		= dGETMGR();
			RwInt32					NestRange;
			RwV3d					NestPos;
			RwUInt32				Color;

			if(pEntity && pEntity != dGET_BRUSH_ENTITY() && pEntity->IsVisible())
			{
				NestPos		= static_cast<CNtlPLCharacter*>(pEntity)->GetPosition();
				NestRange	= static_cast<RwInt32>(pSpawn->GetSpawnMacro().m_WanderRange);
				Color		= 0xFF0000FF;

				for (int i = 0; i < (int)m_vecSpawnSelect.size(); ++i)
				{
					if (m_vecSpawnSelect.at(i) == pSpawn)
					{
						NestRange	= pSpawn->GetNestRange();
						Color		= 0xFFFFFFFF;
						break;
					}
				}

				if (IsVisibleCircle())
				{
					RenderNestCircle(NestPos, NestRange, Color/*((pSpawn->m_MdlGUID * 10) | 0xFF000000)*/);
				}

				if (IsVisibleCircleScan() && pSpawn->GetSpawnType() == eSPAWN_MOB)
				{
					CMobTable*		pMobTbl = static_cast<CMobTable*>(API_GetTableContainer()->GetMobTable());
					CNPCTable*		pNpcTbl = static_cast<CNPCTable*>(API_GetTableContainer()->GetNpcTable());
					sMOB_TBLDAT*	pTlbData = (sMOB_TBLDAT*)pMobTbl->FindData(pSpawn->m_MdlGUID);
					if (pTlbData)
					{
						Color		= 0xFFFF0000;
						NestRange	= pTlbData->wScan_Range;
						RenderNestCircle(NestPos, NestRange, Color/*((pSpawn->m_MdlGUID * 10) | 0xFF000000)*/);
					}
				}
			}
		}
	}

	// Multi select rectangle brush
// 	if(m_pBrushMultiSelector)
// 	{
// 		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 		{
// 			CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
// 			pCurBrush->Render();
// 		}
// 		else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 		{
// 			CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
// 			pCurBrush->Render();
// 		}		
// 	}

	if (m_pBrushMultiSelectorFrustum && (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD || CNtlPLWorldState::GetActiveMiniIndoor()))
	{
		CNTLWB_Frustum<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNTLWB_Frustum<CNtlWorldFieldManager>*>(m_pBrushMultiSelectorFrustum->GetController());
		pCurBrush->Render();
	}
	else if (m_pBrushMultiSelector && GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		CNtlWB_Rectangle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pBrushMultiSelector->GetController());
		pCurBrush->Render();
	}
	
	if(m_pPathHandler)
	{
		m_pPathHandler->Render();
	}
}

VOID CSpawnManager::RenderText(RtCharset* _pCharset, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight)
{
	if(dGET_CUR_PALETTE() == ePM_SPAWN && dGET_WORLD_PARAM()->FlgObjInfoShow)
	{
		RwChar Caption[50];

		for(RwUInt32 i = 0; i < m_vecSpawnSelect.size(); ++i)
		{
			RwV3d v2dPoint;
			RwV3d v3dPoint = m_vecSpawnSelect[i]->GetNtlPLEntity()->GetPosition();

			RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
			RwV3dTransformPoints(&v2dPoint, &v3dPoint, 1, pMatView);
			v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
			v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight;

			RwRGBA ClrFont;
			ClrFont.red		= 255;
			ClrFont.green	= 0;
			ClrFont.blue	= 0;

			RwInt32 GroupIdx			= m_vecSpawnSelect[i]->GetGroupIdx();
			RwInt32 PlayScriptIdx		= m_vecSpawnSelect[i]->GetPlayScriptIdx();
			RwInt32 PlayScriptSceneIdx	= m_vecSpawnSelect[i]->GetPlayScriptSceneIdx();

			if(!GroupIdx)
			{
				if(PlayScriptIdx && PlayScriptSceneIdx)
				{
					RtCharsetSetColors(_pCharset, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(Caption, RWSTRING("DN : %u, %u"), m_vecSpawnSelect[i]->GetPlayScriptIdx(), m_vecSpawnSelect[i]->GetPlayScriptSceneIdx());
					RsCharsetPrint(_pCharset, Caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);	
				}
			}
			else if(GroupIdx)
			{
				if(PlayScriptIdx && PlayScriptSceneIdx)
				{
					RtCharsetSetColors(_pCharset, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(Caption, RWSTRING("SG : %u, DN : %u, %u"), m_vecSpawnSelect[i]->GetGroupIdx(), m_vecSpawnSelect[i]->GetPlayScriptIdx(), m_vecSpawnSelect[i]->GetPlayScriptSceneIdx());
					RsCharsetPrint(_pCharset, Caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);	
				}
				else
				{
					RtCharsetSetColors(_pCharset, &ClrFont, &dGETAPP().m_BackgroundColor);
					RsSprintf(Caption, RWSTRING("SG : %u"), m_vecSpawnSelect[i]->GetGroupIdx());
					RsCharsetPrint(_pCharset, Caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);	
				}
			}
		}
	}
}

VOID CSpawnManager::RenderNestLookatLine(RwV3d& _NestPos, RwV3d& _NestRot, RwReal NestScl, D3DCOLOR Color, CNtlWorldBrush* _pBrushLine)
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	CNtlWorldBrushController *pController = new CNtlWB_Line<CNtlWorldFieldManager>(eNBT_LINE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
	_pBrushLine->SetController(pController);

	CNtlWB_Line<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Line<CNtlWorldFieldManager>*>(_pBrushLine->GetController());

	RwV3d vDst;
	vDst.x = 0.0f;
	vDst.y = 0.0f; 
	vDst.z = 1.0f; 

	RwV3d vScl;
	vScl.x = NestScl;
	vScl.y = 1.0f; 
	vScl.z = NestScl; 

	RwMatrix mSRT;    
	RwMatrixSetIdentity(&mSRT);
	RwMatrixScale(&mSRT, &vScl, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&mSRT, &YAxis, _NestRot.y, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&mSRT, &_NestPos, rwCOMBINEPOSTCONCAT);
	RwV3dTransformPoints(&vDst, &vDst, 1, &mSRT);

	pCurBrush->SetPosS(_NestPos);
	pCurBrush->SetPosE(vDst);
	pCurBrush->RenderEx();

	NTL_DELETE(m_pBrushNestCircle);
}

VOID CSpawnManager::RenderNestCircle(RwV3d& Pos, RwInt32 Radius, D3DCOLOR dwClr)
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	if(!m_pBrushNestCircle)
	{
		m_pBrushNestCircle = NTL_NEW CNtlWorldBrush;

		CNtlWorldBrushController *pController = new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, pMgr, &CNtlWorldFieldManager::GetWorldSectorHeight);
		m_pBrushNestCircle->SetController(pController);
	}

	CNtlWB_Circle<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWB_Circle<CNtlWorldFieldManager>*>(m_pBrushNestCircle->GetController());

	RwV3d Scale;
	Scale.x = 1.0f;
	Scale.y = 1.0f;
	Scale.z = 1.0f;

	pCurBrush->SetColor(dwClr);
	pCurBrush->Render(Pos, static_cast<RwReal>(Radius), Scale, 50);
	pCurBrush->SetColor(0xFFFFFFFF);
}

RwBool CSpawnManager::RenderMMProp(DWORD _MMProp, RwInt32 _FieldIdx)
{
	CNtlWorldFieldManager* pMgr = dGETMGR();
	vector<RwInt32>	vecNeighborIndices;

	pMgr->GetNeighborFields(_FieldIdx, vecNeighborIndices);
	vecNeighborIndices.push_back(_FieldIdx);

	// neighbors
	for(RwUInt32 i = 0; i < vecNeighborIndices.size(); ++i)
	{
		RwInt32 iFieldIdx = vecNeighborIndices.at(i);
		if (iFieldIdx != -1)
		{
			if (_MMProp & dMM_PROP_MOB)
			{
				RenderMMPropMob(iFieldIdx);
			}
			if (_MMProp & dMM_PROP_NPC)
			{
				RenderMMPropNpc(iFieldIdx);
			}
			if (_MMProp & dMM_PROP_TRIGGER)
			{
				RenderMMPropTrigger(iFieldIdx);
			}
		}

		
		// verify the field index is being in current world
// 		if(vecNeighborIndices[i] >= 0 && vecNeighborIndices[i] <= (dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum - 1))
// 		{
// 			CSpawnContainer::SPAWN_INST_NODE_ITER The = m_pSpawnContainer->FindSpawnInst(vecNeighborIndices[i]);
// 			if(The != m_pSpawnContainer->SIN_END())
// 			{
// 				vector<CSpawn*>::iterator TheInc = The->second->BEGIN();
// 
// 				while(TheInc != The->second->END())
// 				{
// 					CSpawn* pSpawn = static_cast<CSpawn*>(*TheInc);
// 					{
// 						RwV3d					NestPos		= pSpawn->m_T;
// 						RwV3d					NestRot		= pSpawn->m_R;
// 						RwReal					NestScl		= pSpawn->m_S;
// 						RwInt32					NestRange	= static_cast<RwInt32>(pSpawn->GetSpawnMacro().m_WanderRange);
// 						RwUInt32				Color		= 0xFF0000FF;
// 
// 						CMobTable*		pMobTbl = static_cast<CMobTable*>(API_GetTableContainer()->GetMobTable());
// 						CNPCTable*		pNpcTbl = static_cast<CNPCTable*>(API_GetTableContainer()->GetNpcTable());
// 						sMOB_TBLDAT*	pTlbData = (sMOB_TBLDAT*)pMobTbl->FindData(pSpawn->m_MdlGUID);
// 						if(pTlbData)
// 						{
// 							RwInt32 CurLvl		= static_cast<RwInt32>(pTlbData->byLevel) % 10;
// 							RwInt32 LvlArr[10]	= {0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFFFF, 0xFF000000, 0xFFFF5555, 0xFF55FF55, 0xFF5555FF, 0xAABBCCDD, 0xFF555555};
// 
// 							Color		= LvlArr[CurLvl];
// 							NestRange	= pTlbData->byScan_Range;
// 							RenderNestCircle(NestPos, NestRange, Color/*((pSpawn->m_MdlGUID * 10) | 0xFF000000)*/);
// 
// 							CNtlWorldBrush* pBrushLine = NTL_NEW CNtlWorldBrush;
// 							RenderNestLookatLine(NestPos, NestRot, static_cast<RwReal>(NestRange), Color, pBrushLine);
// 							NTL_DELETE(pBrushLine);
// 						}
// 					}
// 
// 					++TheInc;
// 				}
// 			}
// 		}
	}

	vecNeighborIndices.clear();

	return TRUE;
}

RwBool CSpawnManager::RenderMMPropMob(RwInt32 _FieldIdx)
{
	if (m_CurSpawnType != eSPAWN_MOB)
	{
		return FALSE;
	}

	CSpawnContainer::SPAWN_INST_NODE_ITER The = m_pSpawnContainer->FindSpawnInst(_FieldIdx);
	if(The != m_pSpawnContainer->SIN_END())
	{
		vector<CSpawn*>::iterator TheInc = The->second->BEGIN();

		BeginFogState(FALSE);
		BeginLightState(FALSE);

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)FALSE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);

		while(TheInc != The->second->END())
		{
			CSpawn* pSpawn = static_cast<CSpawn*>(*TheInc);
			{
				RwV3d					NestPos		= pSpawn->m_T;
				RwV3d					NestRot		= pSpawn->m_R;
				RwReal					NestScl		= pSpawn->m_S;

				CMobTable*		pMobTbl = static_cast<CMobTable*>(API_GetTableContainer()->GetMobTable());
				CNPCTable*		pNpcTbl = static_cast<CNPCTable*>(API_GetTableContainer()->GetNpcTable());
				sMOB_TBLDAT*	pTlbData = (sMOB_TBLDAT*)pMobTbl->FindData(pSpawn->m_MdlGUID);
				if(pTlbData)
				{
					RwInt32		CurLvl		= static_cast<RwInt32>(pTlbData->byLevel) % 10;
					RwUInt32	LvlArr[10]	= {0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFFFF, 0xFF000000, 0xFFFF5555, 0xFF55FF55, 0xFF5555FF, 0xAABBCCDD, 0xFF555555};

					RwUInt32	Color		= LvlArr[CurLvl];
					RwInt32		NestRange	= pTlbData->wScan_Range + static_cast<RwInt32>(pSpawn->GetSpawnMacro().m_WanderRange);
					RenderNestCircle(NestPos, NestRange, Color);

					CNtlWorldBrush* pBrushLine = NTL_NEW CNtlWorldBrush;
					RenderNestLookatLine(NestPos, NestRot, static_cast<RwReal>(NestRange), Color, pBrushLine);
					NTL_DELETE(pBrushLine);

					RwChar acTemp[32];
					sprintf_s(acTemp, 32, "%d", pSpawn->GetNestMemberCnt());
					CNtlWEApplication::GetInstance()->RenderText(NestPos, acTemp, 256, 256, 255, 255, 255);
				}
			}
			++TheInc;
		}

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

		EndFogState();
		EndLightState();
	}
	return TRUE;
}

RwBool CSpawnManager::RenderMMPropNpc(RwInt32 _FieldIdx)
{
	if (m_CurSpawnType != eSPAWN_NPC)
	{
		return FALSE;
	}

	CSpawnContainer::SPAWN_INST_NODE_ITER The = m_pSpawnContainer->FindSpawnInst(_FieldIdx);
	if(The != m_pSpawnContainer->SIN_END())
	{
		vector<CSpawn*>::iterator TheInc = The->second->BEGIN();

		BeginFogState(FALSE);
		BeginLightState(FALSE);

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)FALSE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);

		while(TheInc != The->second->END())
		{
			CSpawn* pSpawn = static_cast<CSpawn*>(*TheInc);
			{
				RwV3d					NestPos		= pSpawn->m_T;
				RwV3d					NestRot		= pSpawn->m_R;
				RwReal					NestScl		= pSpawn->m_S;
				RwInt32					NestRange	= static_cast<RwInt32>(pSpawn->GetSpawnMacro().m_WanderRange);
								
				CMobTable*		pMobTbl = static_cast<CMobTable*>(API_GetTableContainer()->GetMobTable());
				CNPCTable*		pNpcTbl = static_cast<CNPCTable*>(API_GetTableContainer()->GetNpcTable());
				sNPC_TBLDAT*	pTlbData = (sNPC_TBLDAT*)pNpcTbl->FindData(pSpawn->m_MdlGUID);
				if(pTlbData)
				{
					RwUInt32	Color = 0xFF0000FF;
					RwRGBA		rgba;
					rgba.alpha	= 255;
					rgba.red	= 0;
					rgba.green	= 255;
					rgba.blue	= 0;

					RwMatrix mat;
					RwMatrixTranslate(&mat, &NestPos, rwCOMBINEREPLACE);
					
					RenderGeometrySpherePlane(&mat, &rgba, 3.0f);	

					CNtlWorldBrush* pBrushLine = NTL_NEW CNtlWorldBrush;
					RenderNestLookatLine(NestPos, NestRot, 6.0f, Color, pBrushLine);
					NTL_DELETE(pBrushLine);
				}
			}
			++TheInc;
		}

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

		EndFogState();
		EndLightState();
	}
	return TRUE;
}

RwBool CSpawnManager::RenderMMPropTrigger(RwInt32 _FieldIdx)
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	CNtlWorldField*			pFields = const_cast<CNtlWorldField*>(pMgr->GetFields());
	CNtlWorldField*			pField	= &pFields[_FieldIdx];

	RwV3d vPosS = pField->GetSPos();

	BeginFogState(FALSE);
	BeginLightState(FALSE);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);

	for(RwInt32 z = (RwInt32)vPosS.z; z < (RwInt32)vPosS.z + dGET_WORLD_PARAM()->WorldFieldSize; z += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(RwInt32 x = (RwInt32)vPosS.x; x < (RwInt32)vPosS.x + dGET_WORLD_PARAM()->WorldFieldSize; x += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			RwV3d vPos;
			vPos.x = static_cast<RwReal>(x);
			vPos.z = static_cast<RwReal>(z);

			RwInt32 iSectorIdx = pMgr->GetSectorIdx(vPos);
			if (iSectorIdx != -1)
			{
				CNtlWorldSector* pSector = &pMgr->m_pSectors[iSectorIdx];
				for (RwUInt32 i = 0; i < pSector->m_vecNtlPLEntityOrnament.size(); ++i)
				{
					CNtlPLEntity* pEntity = pSector->m_vecNtlPLEntityOrnament.at(i);
					if (pEntity->GetFlags() & NTL_PLEFLAG_TRIGGER)
					{
						RwUInt32	Color = 0xFF0000FF;
						RwRGBA		rgba;
						rgba.alpha	= 255;
						rgba.red	= 255;
						rgba.green	= 0;
						rgba.blue	= 0;

						RwV3d	vEntityPos		= pEntity->GetPosition();
						RwV3d	vEntityRotate	= *pEntity->GetRotate();

						RwMatrix mat;
						RwMatrixTranslate(&mat, &vEntityPos, rwCOMBINEREPLACE);

						RenderGeometrySpherePlane(&mat, &rgba, 3.0f);

						CNtlWorldBrush* pBrushLine = NTL_NEW CNtlWorldBrush;
						RenderNestLookatLine(vEntityPos, vEntityRotate, 6.0f, Color, pBrushLine);
						NTL_DELETE(pBrushLine);

						if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_TRIGGER_INFO))
						{
							RwChar acTriggerInfo[256];
							sprintf_s(acTriggerInfo, 256, "%s(%u)", pEntity->GetName(), pEntity->GetSID());
							CNtlWEApplication::GetInstance()->RenderText(vEntityPos, acTriggerInfo, 256, 256, 255, 0, 0);
							dGETAPP().RenderText(vEntityPos, acTriggerInfo, 255, 0, 0, TRUE);
						}


					}
				}
			}			
		}
	}

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

	EndFogState();
	EndLightState();

	return TRUE;
}

VOID CSpawnManager::ChangeSpawnModel(CSpawn *pSpawn, DWORD dwMdlGUID)
{
	CSpawnContainer::SPAWN_TREE_NODE_ITER The = m_pSpawnContainer->STN_BEGIN();

	while (The != m_pSpawnContainer->STN_END())
	{
		CSpawnTreeNode* pSpawnTreeNode = The->second;
		for (RwUInt32 i = 0; i < pSpawnTreeNode->m_vecSpawnTreeNode.size(); ++i)
		{	
			if (pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlGUID == dwMdlGUID)
			{
				SPLCharacterCreateParam PLEntityCreateParam;
				PLEntityCreateParam.pPos	= &pSpawn->m_T;
				PLEntityCreateParam.bPcFlag = FALSE;

				pSpawn->DestroyEntity();

				pSpawn->m_MdlGUID	= pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlGUID;
				pSpawn->m_Name		= pSpawnTreeNode->m_vecSpawnTreeNode[i].m_Name;
				pSpawn->m_MdlName	= pSpawnTreeNode->m_vecSpawnTreeNode[i].m_MdlName;

				pSpawn->CreateEntity(dGETMGR());
			}
		}

		++The;
	}
}

VOID CSpawnManager::ChangeSpawnNestCount(CSpawn* pSpawn, RwInt32 iNestCount)
{
	if (!pSpawn->GetSpawnParty())
	{
		pSpawn->SetNestMembers(iNestCount, dGETMGR());
	}
}

VOID CSpawnManager::ChangeSpawnCoolTime(CSpawn* pSpawn, RwInt32 iCoolTime)
{
	pSpawn->GetSpawnMacro().m_CoolTime = iCoolTime;
}

VOID CSpawnManager::ChangeSpawnMoveType(CSpawn* pSpawn, RwInt32 iMoveType)
{
	pSpawn->GetSpawnMacro().m_MoveType = iMoveType;
}

VOID CSpawnManager::ChangeSpawnWanderRange(CSpawn* pSpawn, RwReal fWanderRange)
{
	pSpawn->GetSpawnMacro().m_WanderRange = fWanderRange;
}

VOID CSpawnManager::ChangeSpawnMoveRange(CSpawn* pSpawn, RwReal fMoveRange)
{
	pSpawn->GetSpawnMacro().m_MoveRange = fMoveRange;
}

VOID CSpawnManager::ChangeSpawnDelayTime(CSpawn* pSpawn, RwReal fDelayTime)
{
	pSpawn->GetSpawnMacro().m_DelayTime = fDelayTime;
}

VOID CSpawnManager::ChangeSpawnPathIndex(CSpawn* pSpawn, RwInt32 iPathIndex)
{
	pSpawn->SetPathIndex(iPathIndex);
}

VOID CSpawnManager::ChangeSpawnGroupIndex(CSpawn* pSpawn, DWORD dwGroupIndex)
{
	pSpawn->SetGroupIdx(dwGroupIndex);
}

VOID CSpawnManager::ChangeSpawnPlayScriptIndex(CSpawn* pSpawn, DWORD dwPlayScriptIndex)
{
	pSpawn->SetPlayScriptIdx(dwPlayScriptIndex);
}

VOID CSpawnManager::ChangeSpawnPlayScriptSceneIndex(CSpawn* pSpawn, DWORD dwPlayScriptSceneIndex)
{
	pSpawn->SetPlayScriptSceneIdx(dwPlayScriptSceneIndex);
}

VOID CSpawnManager::ChangeSpawnAIScriptIndex(CSpawn* pSpawn, DWORD dwAIScriptIndex)
{
	pSpawn->SetAIScriptIdx(dwAIScriptIndex);
}

VOID CSpawnManager::ChangeSpawnAIScriptSceneIndex(CSpawn* pSpawn, DWORD dwAIScriptSceneIndex)
{
	pSpawn->SetAIScriptSceneIdx(dwAIScriptSceneIndex);
}

RwBool CSpawnManager::IsVisibleCircle()
{
	return m_bVisibleCircle;
}

RwBool CSpawnManager::IsVisibleCircleScan()
{
	return m_bVisibleCircleScan;
}

void CSpawnManager::SetVisibleCircle(RwBool bVisible)
{
	m_bVisibleCircle = bVisible;
}

void CSpawnManager::SetVisibleCircleScan(RwBool bVisible)
{
	m_bVisibleCircleScan = bVisible;
}