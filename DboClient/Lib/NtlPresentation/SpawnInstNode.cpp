#include "precomp_ntlpresentation.h"
#include "SpawnInstNode.h"
#include "SpawnMOB.h"
#include "SpawnNPC.h"
#include "NtlDebug.h"
#include "NtlPLCharacter.h"
#include "NtlPLSceneManager.h"
#include "NtlWorldFieldManager.h"


CSpawnInstNode::CSpawnInstNode(void)
{
	m_vecSpawn.reserve(10);
}

CSpawnInstNode::~CSpawnInstNode(void)
{
	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		CSpawn* pSpawn = m_vecSpawn[i];
		NTL_DELETE(pSpawn);
	}

	m_vecSpawn.clear();
}

VOID CSpawnInstNode::CreateEntity(CNtlWorldFieldManager* pMgr)
{
	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{	
		m_vecSpawn[i]->CreateEntity(pMgr);
	}
}

VOID CSpawnInstNode::DestroyEntity()
{
	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{	
		m_vecSpawn[i]->DestroyEntity();
	}
}

CSpawn*	CSpawnInstNode::Insert(CSpawn* _pSpawn)
{
	m_vecSpawn.push_back(_pSpawn);

	return _pSpawn;
}

CSpawn* CSpawnInstNode::Insert(CNtlPLEntity* _pNtlPLEntity, eSPAWN_TYPE _eSpawnType)
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
	    pSpawn->SetNtlPLEntity(_pNtlPLEntity);

	m_vecSpawn.push_back(pSpawn);

	return pSpawn;
}	

VOID CSpawnInstNode::DeleteSpawnElement(CSpawn* _pSpawn)
{
	vector<CSpawn*>::iterator The = m_vecSpawn.begin();

	while(The != m_vecSpawn.end())
	{
		if((*The) == _pSpawn)
		{
			m_vecSpawn.erase(The);

			if(!m_vecSpawn.size())
			{
				m_vecSpawn.clear();
			}

			return;
		}

		++The;
	}
}

VOID CSpawnInstNode::Delete(RwV3d& _Pos)
{
	vector<CSpawn*>::iterator The = m_vecSpawn.begin();

	while(The != m_vecSpawn.end())
	{
		CSpawn* pCurSpawn = static_cast<CSpawn*>(*The);

		if(	static_cast<RwInt32>(pCurSpawn->m_T.x) == static_cast<DWORD>(_Pos.x) &&
			static_cast<RwInt32>(pCurSpawn->m_T.y) == static_cast<DWORD>(_Pos.y) &&
			static_cast<RwInt32>(pCurSpawn->m_T.z) == static_cast<DWORD>(_Pos.z))
		{
			NTL_DELETE(pCurSpawn);
			m_vecSpawn.erase(The);

			if(!m_vecSpawn.size())
			{
				m_vecSpawn.clear();
			}

			return;
		}

		++The;
	}
}

VOID CSpawnInstNode::Delete(CSpawn* _pSpawn)
{
	vector<CSpawn*>::iterator The = m_vecSpawn.begin();

	while(The != m_vecSpawn.end())
	{
		if((*The) == _pSpawn)
		{
			NTL_DELETE(_pSpawn);
			m_vecSpawn.erase(The);

			if(!m_vecSpawn.size())
			{
				m_vecSpawn.clear();
			}

			return;
		}

		++The;
	}
}

VOID CSpawnInstNode::Delete(CNtlPLEntity* _pNtlPLEntity)
{
	vector<CSpawn*>::iterator The = m_vecSpawn.begin();

	while(The != m_vecSpawn.end())
	{
		if((*The)->Compare(_pNtlPLEntity))
		{
			m_vecSpawn.erase(The);

			if(!m_vecSpawn.size())
			{
				m_vecSpawn.clear();
			}

			return;
		}

		++The;
	}
}

CSpawn*	CSpawnInstNode::GetSpawn(CNtlPLEntity* _pNtlPLEntity)
{
	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		if(m_vecSpawn[i]->GetNtlPLEntity() == _pNtlPLEntity)
		{
			return m_vecSpawn[i];
		}
	}

	return NULL;
}

VOID CSpawnInstNode::DeleteSpawnLoadAtField(vector<CSpawn*>& _vecSpawnLoad)
{
	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		if(m_vecSpawn[i]->GetNtlPLEntity())
		{
			vector<CSpawn*>::iterator The = _vecSpawnLoad.begin();
			while(The != _vecSpawnLoad.end())
			{
				if((*The) == m_vecSpawn[i])
				{
					_vecSpawnLoad.erase(The);
					break;
				}

				++The;
			}
		}
	}
}

VOID CSpawnInstNode::InsertSpawnLoadAtField(vector<CSpawn*>& _vecSpawnLoad)
{
	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		RwBool IsExisted = FALSE;

		for(RwUInt32 j = 0; j < _vecSpawnLoad.size(); ++j)
		{
			if(m_vecSpawn[i] == _vecSpawnLoad[j])
			{
				IsExisted = TRUE;
				break;
			}
		}

		if(!IsExisted)
		{
			_vecSpawnLoad.push_back(m_vecSpawn[i]);
		}
	}
}

VOID CSpawnInstNode::Save(FILE* _pFile)
{
	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		m_vecSpawn[i]->Save(_pFile);
	}
}

RwInt32 CSpawnInstNode::GetSpawnNestCnt()
{
	RwInt32 RetCnt = 0;

	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		RetCnt += m_vecSpawn[i]->GetNestMemberCnt();
	}

	return RetCnt;
}