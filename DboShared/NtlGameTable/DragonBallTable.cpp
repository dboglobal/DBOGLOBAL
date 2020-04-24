#include "StdAfx.h"
#include "DragonBallTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CDragonBallTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CDragonBallTable::CDragonBallTable(void)
{
	Init();
}

CDragonBallTable::~CDragonBallTable(void)
{
	Destroy();
}

bool CDragonBallTable::Create(DWORD dwCodePage)
{
	::ZeroMemory(m_aDBTbldat, sizeof(m_aDBTbldat));
	m_dragonGroup.clear();
	return CTable::Create(dwCodePage);
}

void CDragonBallTable::Destroy()
{
	::ZeroMemory(m_aDBTbldat, sizeof(m_aDBTbldat));
	m_dragonGroup.clear();
}

void CDragonBallTable::Init()
{
	m_dragonGroup.clear();
}

void* CDragonBallTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDRAGONBALL_TBLDAT* pNewDragonBall = new sDRAGONBALL_TBLDAT;
		if (NULL == pNewDragonBall)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewDragonBall;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewDragonBall;
	}

	return NULL;
}

bool CDragonBallTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDRAGONBALL_TBLDAT* pDragonBall = (sDRAGONBALL_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pDragonBall, sizeof(*pDragonBall)))
			return false;

		delete pDragonBall;

		return true;
	}

	return false;
}

eDRAGON_BALL_TYPE CDragonBallTable::GetDropItemType( UINT dropTblidx ) 
{
    sDRAGONBALL_TBLDAT* pTblData = NULL;
    for(BYTE j = DRAGON_BALL_TYPE_FIRST; j <= DRAGON_BALL_TYPE_LAST; ++j)
    {
        pTblData = (sDRAGONBALL_TBLDAT*)GetDBTbldat(j);
        if(pTblData && pTblData->ballDropTblidx == dropTblidx)
        {
            return (eDRAGON_BALL_TYPE)pTblData->byBallType;
        }

    }	

    return DRAGON_BALL_TYPE_NONE;
}

bool CDragonBallTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sDRAGONBALL_TBLDAT* pTbldat = (sDRAGONBALL_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	if( false == SetDBTbldatByGroup( pTbldat->dwAltarGroup, pTbldat->tblidx ) )
	{
		_ASSERTE( 0 );
		m_mapTableList.erase( pTbldat->tblidx );
		return false;
	}

	if( false == SetDBTbldat( pTbldat->byBallType, pTbldat ) )
	{
		_ASSERTE( 0 );
		m_mapTableList.erase( pTbldat->tblidx );
		return false;
	}

	return true;
}

bool CDragonBallTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDRAGONBALL_TBLDAT* pDragonBall = (sDRAGONBALL_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pDragonBall->tblidx = READ_DWORD( bstrData );
		}		
		else if (0 == wcscmp(pstrDataName->c_str(), L"Altar_Group"))
		{
			pDragonBall->dwAltarGroup = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_Type"))
		{
			pDragonBall->byBallType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_1_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->aBallTblidx[0] = READ_DWORD( bstrData );
		}		
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_2_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->aBallTblidx[1] = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_3_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->aBallTblidx[2] = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_4_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->aBallTblidx[3] = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_5_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->aBallTblidx[4] = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_6_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->aBallTblidx[5] = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_7_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->aBallTblidx[6] = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_Drop_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->ballDropTblidx = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ball_Junk_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDragonBall->ballJunkTblidx = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Start_Dialog"))
		{
			pDragonBall->startDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"End_Dialog"))
		{
			pDragonBall->endDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Timeover_End_Dialog"))
		{
			pDragonBall->timeoverEndDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Hurry_Dialog"))
		{
			pDragonBall->hurryDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Timeover_Dialog"))
		{
			pDragonBall->timeoverDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"No_Repeat_Dialog"))
		{
			pDragonBall->noRepeatDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dragon_NPC_Tblidx"))
		{
			pDragonBall->dragonNPCTblidx = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Default_Summon_Chat"))
		{
			pDragonBall->defaultSummonChat = READ_DWORD( bstrData );
		}		
		else if (0 == wcscmp(pstrDataName->c_str(), L"Inventory_Full_Dialog"))
		{
			pDragonBall->inventoryFullDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Overlap_Dialog"))
		{
			pDragonBall->skillOverlapDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Shortage_Of_LV_Dialog"))
		{
			pDragonBall->skillShortageOfLVDialog = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Appear_Dir_X"))
		{
			pDragonBall->fDir.x = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Appear_Dir_Z"))
		{
			pDragonBall->fDir.z = READ_FLOAT( bstrData, pstrDataName->c_str() );
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


sTBLDAT* CDragonBallTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CDragonBallTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sDRAGONBALL_TBLDAT* pTableData = new sDRAGONBALL_TBLDAT;
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

bool CDragonBallTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sDRAGONBALL_TBLDAT* pTableData = (sDRAGONBALL_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:	
//		Return	:	
//-----------------------------------------------------------------------------------
sDRAGONBALL_TBLDAT* CDragonBallTable::GetDBTbldatByGroupAndType(DWORD dwGroupID, BYTE byType)
{
	sDRAGONBALL_TBLDAT* pTblData = NULL;
	UNREFERENCED_PARAMETER(pTblData);

	TABLE::iterator iter;
	for (iter = Begin(); End() != iter; iter++)
	{
		sDRAGONBALL_TBLDAT* pTableData = (sDRAGONBALL_TBLDAT*)(iter->second);

		if (pTableData->dwAltarGroup == dwGroupID && pTableData->byBallType == byType)
			return pTableData;
	}

	return NULL;
}

//-----------------------------------------------------------------------------------
//		Purpose	:	
//		Return	:	The value returned by the table BallType
//-----------------------------------------------------------------------------------
sTBLDAT * CDragonBallTable::GetDBTbldatByGroup(DWORD dwGrouID)					
{
	if (0 == dwGrouID)
		return NULL;

	DRAGONBALLGROUP_IT iter = m_dragonGroup.find(dwGrouID);
	if (m_dragonGroup.end() == iter)
		return NULL;

	sTBLDAT* psTable = FindData(iter->second);
	return psTable; 

}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDragonBallTable::SetDBTbldatByGroup(DWORD dwGroupID, TBLIDX tblidx)
{
	if (0 == dwGroupID)
	{
		return false;
	}

	if ( NULL == FindData(tblidx) )
	{
		return false;
	}

	DRAGONBALLGROUP_IT it = m_dragonGroup.insert( DRAGONBALLGROUP_VAL( dwGroupID, tblidx ) );
	if( it == m_dragonGroup.end() )
	{
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:	
//		Return	:	The value returned by the table BallType
//-----------------------------------------------------------------------------------
sTBLDAT * CDragonBallTable::GetDBTbldat(BYTE byType)					
{
	if( byType > DRAGON_BALL_TYPE_LAST)
	{
		return NULL;
	}

	return m_aDBTbldat[byType];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDragonBallTable::SetDBTbldat(BYTE byType, sTBLDAT * pTbldat)
{
	if( byType > DRAGON_BALL_TYPE_LAST)
	{
		return false;
	}

	// already registered?
	if( m_aDBTbldat[byType] != NULL)
	{
		return true;
	}

	m_aDBTbldat[byType] = pTbldat;

	return true;
}

bool CDragonBallTable::GetType(UINT uiSerial, OUT BYTE& byType, OUT BYTE& byNumber)
{
	sDRAGONBALL_TBLDAT* pTblData = NULL;	

	for(BYTE j = DRAGON_BALL_TYPE_FIRST; j <= DRAGON_BALL_TYPE_LAST; ++j)
	{
		pTblData = (sDRAGONBALL_TBLDAT*)GetDBTbldat(j);
		if(pTblData)
		{
			for(BYTE i = 0; i < NTL_ITEM_MAX_DRAGONBALL; ++i)
			{
				if(pTblData->aBallTblidx[i] == uiSerial)
				{
					byType = j;
					byNumber = i + 1;
					return true;
				}
			}
		}
	}	

	return false;
}