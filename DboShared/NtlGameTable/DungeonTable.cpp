//***********************************************************************************
//
//	File		:	DungeonTable.cpp
//
//	Begin		:	2008-07-30
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Chung,DooSup   ( mailto:john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "DungeonTable.h"

#include "NtlDebug.h"
#include "NtlSerializer.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WCHAR* CDungeonTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CDungeonTable::CDungeonTable( void )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CDungeonTable::~CDungeonTable( void )
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CDungeonTable::Init( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDungeonTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CDungeonTable::Destroy( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void* CDungeonTable::AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sDUNGEON_TBLDAT* pNewObj = new sDUNGEON_TBLDAT;
		if ( NULL == pNewObj )
		{
			return NULL;
		}

		CPINFO cpInfo;
		if ( !GetCPInfo( dwCodePage, &cpInfo ) )
		{
			delete pNewObj;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewObj;
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDungeonTable::DeallocNewTable( void* pvTable, WCHAR* pwszSheetName )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sDUNGEON_TBLDAT* pObj = (sDUNGEON_TBLDAT*)pvTable;
		if ( IsBadReadPtr( pObj, sizeof(*pObj) ) ) return false;

		delete pObj;

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDungeonTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sDUNGEON_TBLDAT * pTbldat = (sDUNGEON_TBLDAT*)pvTable;

	if ( false == m_mapTableList.insert( std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat) ).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDungeonTable::SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData )
{
	static char szTemp[1024] = { 0x00, };

	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sDUNGEON_TBLDAT * pTbldat = (sDUNGEON_TBLDAT*) pvTable;

		if ( 0 == wcscmp( pstrDataName->c_str(), L"Tblidx" ) )
		{
			pTbldat->tblidx = READ_TBLIDX( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Dungeon_Type" ) )
		{
			pTbldat->byDungeonType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Max_Member" ) )
		{
			pTbldat->byMaxMember = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Link_World" ) )
		{
			pTbldat->linkWorld = READ_TBLIDX( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Min_Level" ) )
		{
			pTbldat->byMinLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Max_Level" ) )
		{
			pTbldat->byMaxLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Need_Item" ) )
		{
			pTbldat->needItemTblidx = READ_TBLIDX( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Honor_Point"))
		{
			pTbldat->dwHonorPoint = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Wps_Tblidx" ) )
		{
			pTbldat->wpsTblidx = READ_TBLIDX( bstrData );
		}		
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Open_Cine" ) )
		{
			pTbldat->openCine = READ_TBLIDX( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Group_Index" ) )
		{
			pTbldat->groupIdx = READ_TBLIDX( bstrData );
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


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
sTBLDAT* CDungeonTable::FindData( TBLIDX tblidx )
{
	TABLEIT iter = m_mapTableList.find( tblidx );
	if ( m_mapTableList.end() == iter )
	{
		return NULL;
	}

	return (sTBLDAT*)(iter->second); 
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
sDUNGEON_TBLDAT* CDungeonTable::FindDataByGroupIdx(GROUPID groupIdx, BYTE byDifficulty)
{
	for (TABLEIT iter = m_mapTableList.begin(); iter != m_mapTableList.end(); iter++)
	{
		sDUNGEON_TBLDAT * pData = (sDUNGEON_TBLDAT*)iter->second;

		if (pData && pData->groupIdx == groupIdx && pData->byDungeonType == byDifficulty + 1)
			return pData;
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDungeonTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sDUNGEON_TBLDAT* pTableData = new sDUNGEON_TBLDAT;
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


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDungeonTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sDUNGEON_TBLDAT* pTableData = (sDUNGEON_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}
