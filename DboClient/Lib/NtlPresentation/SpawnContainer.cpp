#include "precomp_ntlpresentation.h"
#include "SpawnContainer.h"
#include "SpawnTreeNode.h"
#include "NtlDebug.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLEntity.h"
#include "SpawnInstNode.h"
#include "SpawnParty.h"
#include "SpawnMOB.h"
#include "SpawnNPC.h"


CSpawnContainer::CSpawnContainer(void)
{
}

CSpawnContainer::~CSpawnContainer(void)
{
	SPAWN_TREE_NODE_ITER TheTree = m_mapSpawnTreeNode.begin();
	while(TheTree != m_mapSpawnTreeNode.end())
	{
		CSpawnTreeNode* p4Del = TheTree->second;
		NTL_DELETE(p4Del);

		++TheTree;
	}

	m_mapSpawnTreeNode.clear();

	SPAWN_INST_NODE_ITER TheInst = m_mapSpawnInstNode.begin();
	while(TheInst != m_mapSpawnInstNode.end())
	{
		CSpawnInstNode* p4Del = TheInst->second;
		NTL_DELETE(p4Del);

		++TheInst;
	}

	m_mapSpawnInstNode.clear();
}

CSpawnContainer::SPAWN_INST_NODE_ITER CSpawnContainer::FindSpawnInst(RwInt32 _FieldIdx)
{
	SPAWN_INST_NODE_ITER The = m_mapSpawnInstNode.begin();
	while(The != m_mapSpawnInstNode.end())
	{
		if(The->first == _FieldIdx)
		{
			return The;
		}

		++The;
	}

	return m_mapSpawnInstNode.end();
}

CSpawnContainer::SPAWN_TREE_NODE_ITER CSpawnContainer::FindSpawnTree(RwInt32 _SortCodeIdx)
{
	SPAWN_TREE_NODE_ITER The = m_mapSpawnTreeNode.begin();
	while(The != m_mapSpawnTreeNode.end())
	{
		if(The->first == _SortCodeIdx)
		{
			return The;
		}

		++The;
	}

	return m_mapSpawnTreeNode.end();
}

RwBool CSpawnContainer::InsertSpawnTree(RwInt32 _SortCode, RwUInt32 _MdlGUID, BYTE _MdlLvl, string& _Name, string& _MdlName)
{
	SPAWN_TREE_NODE_ITER The = FindSpawnTree(_SortCode);
	if(The == m_mapSpawnTreeNode.end())
	{
		CSpawnTreeNode* pSpawnTreeNode = NTL_NEW CSpawnTreeNode();
		pSpawnTreeNode->Insert(_MdlGUID, _MdlLvl, _Name, _MdlName);

		m_mapSpawnTreeNode.insert(SPAWN_TREE_NODE_VALUE(_SortCode, pSpawnTreeNode));
	}
	else
	{
		CSpawnTreeNode* pSpawnTreeNode = The->second;
		pSpawnTreeNode->Insert(_MdlGUID, _MdlLvl, _Name, _MdlName);
	}

	return TRUE;
}

CSpawn* CSpawnContainer::InsertSpawnLoad(CSpawn* _pSpawn, CNtlWorldFieldManager* pFMgr)
{
	RwInt32					FieldIdx	= pFMgr->GetFieldIdx(_pSpawn->m_T);
	SPAWN_INST_NODE_ITER	The			= FindSpawnInst(FieldIdx);
	CSpawn*					Ret			= NULL;

	if(The == m_mapSpawnInstNode.end())
	{
		CSpawnInstNode* pSpawnInstNode = NTL_NEW CSpawnInstNode;
		Ret = pSpawnInstNode->Insert(_pSpawn);
		m_mapSpawnInstNode.insert(SPAWN_INST_NODE_VALUE(FieldIdx, pSpawnInstNode));
	}
	else
	{
		Ret = The->second->Insert(_pSpawn);
	}

	return Ret;
}

CSpawn* CSpawnContainer::InsertSpawnInst(CSpawn* _pSpawn, CNtlWorldFieldManager* pFMgr)
{
	RwInt32					FieldIdx	= pFMgr->GetFieldIdx(_pSpawn->GetNtlPLEntity()->GetPosition());
	SPAWN_INST_NODE_ITER	The			= FindSpawnInst(FieldIdx);
	CSpawn*					Ret			= NULL;

	if(The == m_mapSpawnInstNode.end())
	{
		CSpawnInstNode* pSpawnInstNode = NTL_NEW CSpawnInstNode;
		Ret = pSpawnInstNode->Insert(_pSpawn);
		m_mapSpawnInstNode.insert(SPAWN_INST_NODE_VALUE(FieldIdx, pSpawnInstNode));
	}
	else
	{
		Ret = The->second->Insert(_pSpawn);
	}

	return Ret;
}

CSpawn* CSpawnContainer::InsertSpawnInst(CNtlPLEntity* pNtlPLEntity, CNtlWorldFieldManager* pFMgr, eSPAWN_TYPE eSpawnType)
{
	RwInt32					FieldIdx	= pFMgr->GetFieldIdx(pNtlPLEntity->GetPosition());
	SPAWN_INST_NODE_ITER	The			= FindSpawnInst(FieldIdx);
	CSpawn*					Ret			= NULL;

	if(The == m_mapSpawnInstNode.end())
	{
		CSpawnInstNode* pSpawnInstNode = NTL_NEW CSpawnInstNode;
		Ret = pSpawnInstNode->Insert(pNtlPLEntity, eSpawnType);
		m_mapSpawnInstNode.insert(SPAWN_INST_NODE_VALUE(FieldIdx, pSpawnInstNode));
	}
	else
	{
		Ret = The->second->Insert(pNtlPLEntity, eSpawnType);
	}

	return Ret;
}

VOID CSpawnContainer::DeleteSpawnInst(RwV3d& _Pos, CNtlWorldFieldManager* pFMgr)
{
	RwInt32					FieldIdx	= pFMgr->GetFieldIdx(_Pos);
	SPAWN_INST_NODE_ITER	The			= FindSpawnInst(FieldIdx);

	if(The != m_mapSpawnInstNode.end())
	{
		The->second->Delete(_Pos);

		if(!The->second->GetSpawnCnt())
		{
			CSpawnInstNode* pDel = The->second;
			NTL_DELETE(pDel);

			m_mapSpawnInstNode.erase(The);

			if(!m_mapSpawnInstNode.size())
			{
				m_mapSpawnInstNode.clear();
			}
		}
	}
}

VOID CSpawnContainer::DeleteSpawnInst(CSpawn* pSpawn, CNtlWorldFieldManager* pFMgr)
{
	RwInt32					FieldIdx	= pFMgr->GetFieldIdx(pSpawn->GetNtlPLEntity()->GetPosition());
	SPAWN_INST_NODE_ITER	The			= FindSpawnInst(FieldIdx);

	The->second->Delete(pSpawn);
	if(!The->second->GetSpawnCnt())
	{
		CSpawnInstNode* pDel = The->second;
		NTL_DELETE(pDel);

		m_mapSpawnInstNode.erase(The);

		if(!m_mapSpawnInstNode.size())
		{
			m_mapSpawnInstNode.clear();
		}
	}
}

VOID CSpawnContainer::CreateSpawnEntity(RwInt32 FieldIdx, CNtlWorldFieldManager* pFMgr)
{
	SPAWN_INST_NODE_ITER The = FindSpawnInst(FieldIdx);
	if(The != m_mapSpawnInstNode.end())
	{
		The->second->CreateEntity(pFMgr);
	}
}

VOID CSpawnContainer::DestroySpawnEntity(RwInt32 FieldIdx)
{
	SPAWN_INST_NODE_ITER The = FindSpawnInst(FieldIdx);
	if(The != m_mapSpawnInstNode.end())
	{
		The->second->DestroyEntity();
	}
}

CSpawn* CSpawnContainer::GetSpawn(CNtlPLEntity* _pNtlPLEntity, CNtlWorldFieldManager* pFMgr)
{
	RwInt32 FieldIdx = pFMgr->GetFieldIdx(_pNtlPLEntity->GetPosition());
	if(FieldIdx == -1)
	{
		return NULL;
	}

	SPAWN_INST_NODE_ITER The = FindSpawnInst(FieldIdx);
	if(The != m_mapSpawnInstNode.end())
	{
		return The->second->GetSpawn(_pNtlPLEntity);
	}

	return NULL;
}

VOID CSpawnContainer::MoveSpawn(RwV3d& Pos, CSpawn* pSpawn, CNtlWorldFieldManager* pFMgr, eSPAWN_TYPE _CurSpawnType)
{
	RwInt32 CurFieldIdx = pFMgr->GetFieldIdx(pSpawn->GetNtlPLEntity()->GetPosition());
	RwInt32 NewFieldIdx = pFMgr->GetFieldIdx(Pos);

	if(CurFieldIdx == NewFieldIdx)
	{
		pSpawn->Move(Pos, pFMgr);
	}
	else
	{
		SPAWN_INST_NODE_ITER The = FindSpawnInst(CurFieldIdx);
		The->second->DeleteSpawnElement(pSpawn);
		if(!The->second->GetSpawnCnt())
		{
			NTL_DELETE(The->second);

			m_mapSpawnInstNode.erase(The);

			if(!m_mapSpawnInstNode.size())
			{
				m_mapSpawnInstNode.clear();
			}
		}

		pSpawn->Move(Pos, pFMgr);
		InsertSpawnInst(pSpawn, pFMgr);
	}
}

VOID CSpawnContainer::DeleteSpawnLoadAtField(RwInt32 FieldIdx, vector<CSpawn*>& _vecSpawnLoad)
{
	SPAWN_INST_NODE_ITER The = FindSpawnInst(FieldIdx);

	if(The != m_mapSpawnInstNode.end())
	{
		The->second->DeleteSpawnLoadAtField(_vecSpawnLoad);
	}
}

VOID CSpawnContainer::InsertSpawnLoadAtField(RwInt32 FieldIdx, vector<CSpawn*>& _vecSpawnLoad)
{
	SPAWN_INST_NODE_ITER The = FindSpawnInst(FieldIdx);

	if(The != m_mapSpawnInstNode.end())
	{
		The->second->InsertSpawnLoadAtField(_vecSpawnLoad);
	}
}

RwInt32	CSpawnContainer::GetSpawnCnt(RwInt32 _FieldIdx)
{
	SPAWN_INST_NODE_ITER The = FindSpawnInst(_FieldIdx);

	if(The == m_mapSpawnInstNode.end())
	{
		return 0;
	}

	return The->second->GetSpawnNestCnt();
}

VOID CSpawnContainer::Save(FILE* _pFile)
{
	// Party container.
	CSpawnPartyContainer::Save(_pFile);

	// Total spawn count
	RwInt32 TotSpawnCnt = 0;
	SPAWN_INST_NODE_ITER TheInst = m_mapSpawnInstNode.begin();
	while(TheInst != m_mapSpawnInstNode.end())
	{
		TotSpawnCnt += TheInst->second->GetSpawnCnt();
		++TheInst;
	}

	fwrite(&TotSpawnCnt, sizeof(RwInt32), 1, _pFile);

	// Each spawn
	TheInst = m_mapSpawnInstNode.begin();
	while(TheInst != m_mapSpawnInstNode.end())
	{
		// SpawnInstNode
		TheInst->second->Save(_pFile);

		++TheInst;
	}
}

VOID CSpawnContainer::Load(FILE* _pFile, eSPAWN_TYPE _eSpawnType, CNtlWorldFieldManager* pMgr)
{
	// Party container.
	CSpawnPartyContainer::Load(_pFile);

	// Create all spawn
	RwInt32 TotSpawnCnt;
	fread(&TotSpawnCnt, sizeof(RwInt32), 1, _pFile);

	for(RwInt32 i = 0; i < TotSpawnCnt; ++i)
	{
		CSpawn* pSpawn = NULL;

		if(_eSpawnType == eSPAWN_MOB)
		{
			pSpawn = NTL_NEW CSpawnMOB;
		}
		else if(_eSpawnType == eSPAWN_NPC)
		{
			pSpawn = NTL_NEW CSpawnNPC;
		}

        if(pSpawn)
		    pSpawn->Load(_pFile);

#ifdef dNTL_WORLD_TOOL_MODE

	CSpawnTreeElement spawnTreeElement;
	if (FindSpawnTree(pSpawn->m_MdlGUID, &spawnTreeElement))
	{
		pSpawn->m_MdlName	= spawnTreeElement.m_MdlName;
		pSpawn->m_Name		= spawnTreeElement.m_Name;
	}

#endif

		InsertSpawnLoad(pSpawn, pMgr);
	}
}

RwBool CSpawnContainer::FindSpawnTree(RwUInt32 uiMdlGUID, CSpawnTreeElement* pOutSpawnTreeElement)
{
	RwUInt32 iFirstIdx = uiMdlGUID / 10000;

	SPAWN_TREE_NODE_ITER it = m_mapSpawnTreeNode.find(iFirstIdx);
	if (it != m_mapSpawnTreeNode.end())
	{
		CSpawnTreeNode* pSpawnTreeNode = it->second;
		for (int i = 0; i < (int)pSpawnTreeNode->m_vecSpawnTreeNode.size(); ++i)
		{
			CSpawnTreeElement& spawnTreeNode = pSpawnTreeNode->m_vecSpawnTreeNode.at(i);
			if (spawnTreeNode.m_MdlGUID == uiMdlGUID)
			{
				pOutSpawnTreeElement->m_Name			= spawnTreeNode.m_Name;
				pOutSpawnTreeElement->m_MdlName			= spawnTreeNode.m_MdlName;
				pOutSpawnTreeElement->m_MdlGUID			= spawnTreeNode.m_MdlGUID;
				pOutSpawnTreeElement->m_MdlLvl			= spawnTreeNode.m_MdlLvl;
				pOutSpawnTreeElement->m_LastMacroIdx	= spawnTreeNode.m_LastMacroIdx;

				return TRUE;
			}
		}
	}


	return FALSE;
}