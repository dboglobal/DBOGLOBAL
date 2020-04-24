//***********************************************************************************
//
//	File		:	SpawnTable.cpp
//
//	Begin		:	2006-03-27
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Doo  Sup, Chung   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "NtlCharacter.h"
#include "SpawnTable.h"
#include "NtlDebug.h"

#include "NtlSerializer.h"

WCHAR* CSpawnTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CSpawnTable::CSpawnTable(void)
{
	Init();
}

CSpawnTable::~CSpawnTable(void)
{
	Destroy();
}

bool CSpawnTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CSpawnTable::Destroy()
{
	m_spawnGroup.clear();
}

void CSpawnTable::Init()
{
	m_spawnGroupItBeg = m_spawnGroupItEnd = m_spawnGroup.end();
}

void* CSpawnTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSPAWN_TBLDAT* pNewSpawn = new sSPAWN_TBLDAT;
		if (NULL == pNewSpawn)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewSpawn;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewSpawn;
	}

	return NULL;
}

bool CSpawnTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSPAWN_TBLDAT* pSpawn = (sSPAWN_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pSpawn, sizeof(*pSpawn)))
			return false;

		delete pSpawn;

		return true;
	}

	return false;
}

bool CSpawnTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sSPAWN_TBLDAT* pTbldat = (sSPAWN_TBLDAT*)pvTable;

	if (CNtlVector::ZERO == pTbldat->vSpawn_Dir)
	{
		pTbldat->vSpawn_Dir = CNtlVector::UNIT_X;
	}
	else
	{
		if ( false == (pTbldat->vSpawn_Dir).SafeNormalize())
		{
			_ASSERTE( 0 );
			return false;
		}
	}


	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		printf("[File] : %ls\r\n Table Tblidx[%u] is Duplicated. \n",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	if( INVALID_SPAWNGROUPID != pTbldat->spawnGroupId )
	{
		SPAWNGROUP_IT it = m_spawnGroup.insert( SPAWNGROUP_VAL( pTbldat->spawnGroupId, pTbldat ) );
		if( it == m_spawnGroup.end() )
		{
			printf("Tblidx[%u]::Spawn_Group[%u] is Duplicated ", pTbldat->tblidx, pTbldat->spawnGroupId );
			_ASSERTE( 0 );
			return false;
		}
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
sSPAWN_TBLDAT * CSpawnTable::GetSpawnGroupFirst(SPAWNGROUPID spawnGroupId)
{
	m_spawnGroupItBeg = m_spawnGroup.lower_bound( spawnGroupId );
	m_spawnGroupItEnd = m_spawnGroup.upper_bound( spawnGroupId );

	if( m_spawnGroupItBeg == m_spawnGroupItEnd )
	{
		return NULL;
	}


	return m_spawnGroupItBeg->second;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
sSPAWN_TBLDAT * CSpawnTable::GetSpawnGroupNext(SPAWNGROUPID spawnGroupId)
{
	if( m_spawnGroupItBeg == m_spawnGroupItEnd )
	{
		return NULL;
	}

	if( spawnGroupId != m_spawnGroupItBeg->second->spawnGroupId )
	{
		_ASSERT( 0 );
		return NULL;
	}

	++m_spawnGroupItBeg;

	if( m_spawnGroupItBeg == m_spawnGroupItEnd )
	{
		return NULL;
	}


	return m_spawnGroupItBeg->second;
}


bool CSpawnTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSPAWN_TBLDAT* pSpawn = (sSPAWN_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Mob_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->mob_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Loc_X"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->vSpawn_Loc.x = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Loc_Y"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->vSpawn_Loc.y = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Loc_Z"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->vSpawn_Loc.z = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Dir_X"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->vSpawn_Dir.x = READ_FLOAT(bstrData, pstrDataName->c_str(), 0.0f);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Dir_Z"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->vSpawn_Dir.z = READ_FLOAT(bstrData, pstrDataName->c_str(), 0.0f);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Loc_Range"))
		{
			pSpawn->bySpawn_Loc_Range = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Quantity"))
		{
			pSpawn->bySpawn_Quantity = READ_BYTE(bstrData, pstrDataName->c_str(), 1);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Cool_Time"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->wSpawn_Cool_Time = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Move_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSpawn->bySpawn_Move_Type = READ_BYTE(bstrData, pstrDataName->c_str());

			if (SPAWN_MOVE_FIRST > pSpawn->bySpawn_Move_Type || SPAWN_MOVE_LAST < pSpawn->bySpawn_Move_Type)
			{
				_ASSERT(0);
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Wander_Range"))
		{
			pSpawn->byWander_Range = READ_BYTE(bstrData, pstrDataName->c_str(), 1);

			if ( 0 == pSpawn->byWander_Range )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Wander Range Is Zero(0) (Mob And NPC TBLIDX = %u)"
					, m_wszXmlFileName
					, pSpawn->tblidx );
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Move_Range"))
		{
			pSpawn->byMove_Range = READ_BYTE(bstrData, pstrDataName->c_str(), 1);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Path_Table_Index"))
		{
			pSpawn->path_Table_Index = READ_TBLIDX(bstrData);

			//-임시 테이블 추가 전까지
			pSpawn->playScript = INVALID_TBLIDX;
			pSpawn->playScriptScene = INVALID_TBLIDX;
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"PlayScript_Number"))
		{
			pSpawn->playScript = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"PlayScript_Scene_Number"))
		{
			pSpawn->playScriptScene = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"AIScript_Number"))
		{
			pSpawn->aiScript = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"AIScript_Scene_Number"))
		{
			pSpawn->aiScriptScene = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Follow_Distance_Loc_X"))
		{
			pSpawn->vFollowDistance.x = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Follow_Distance_Loc_Z"))
		{
			pSpawn->vFollowDistance.z = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Party_Index"))
		{
			pSpawn->dwParty_Index = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Party_Leader_Able"))
		{
			pSpawn->bParty_Leader = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Group"))
		{
			pSpawn->spawnGroupId = READ_DWORD(bstrData);
		}
		else
		{
			CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


sTBLDAT* CSpawnTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CSpawnTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{
		sSPAWN_TBLDAT* pTableData = new sSPAWN_TBLDAT;
		if (NULL == pTableData)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		if (false == pTableData->LoadFromBinary(serializer))
		{
			delete pTableData;
			bLoop = false;
			break;
		}

	//	printf("pTableData->tblidx %d \n", pTableData->tblidx);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CSpawnTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sSPAWN_TBLDAT* pTableData = (sSPAWN_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}