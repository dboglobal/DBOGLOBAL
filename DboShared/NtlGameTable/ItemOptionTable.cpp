#include "stdafx.h"
#include "ItemOptionTable.h"
#include "NtlDebug.h"
#include "NtlSkill.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CItemOptionTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CItemOptionTable::CItemOptionTable(void)
{
	Init();
}

CItemOptionTable::~CItemOptionTable(void)
{
	Destroy();
}

bool CItemOptionTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CItemOptionTable::Destroy()
{
}

void CItemOptionTable::Init()
{
	//ZeroMemory(m_aOptionTbldat, sizeof(m_aOptionTbldat) );
}

void* CItemOptionTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_OPTION_TBLDAT* pNewItem = new sITEM_OPTION_TBLDAT;
		if (NULL == pNewItem)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewItem;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewItem;
	}
	
	return NULL;
}

bool CItemOptionTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_OPTION_TBLDAT* pItem = (sITEM_OPTION_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pItem, sizeof(*pItem)))
			return false;

		delete pItem;

		return true;
	}
	
	return false;
}

bool CItemOptionTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	sITEM_OPTION_TBLDAT * pTbldat = (sITEM_OPTION_TBLDAT*) pvTable;
	sITEM_OPTION_TBLDAT * pExistTbldat = NULL;


	if (false == pTbldat->bValidity_Able)
	{
		return false;
	}

	if( bReload )
	{
		pExistTbldat = (sITEM_OPTION_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );

			return true; 
		}
	}

	if( bUpdate) //for tables inside localize folder
	{
	//	printf("UPDATE TABLE %d ! \n", pTbldat->tblidx);
		pExistTbldat = (sITEM_OPTION_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{ //if exist then remove
			m_mapTableList.erase( pTbldat->tblidx );
		}
	}

	if ( false == m_mapTableList.insert( std::map<TBLIDX, sTBLDAT*>::value_type(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CItemOptionTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_OPTION_TBLDAT* pItem = (sITEM_OPTION_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Option_Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );

			READ_STRINGW(bstrData, pItem->wszOption_Name, _countof(pItem->wszOption_Name));
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Validity_Able") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bValidity_Able = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Option_Rank"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byOption_Rank = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Group"))
		{
			pItem->byItem_Group = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Max_Quality"))
		{
			pItem->byMaxQuality = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Quality"))
		{
			pItem->byQuality = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Quality_Index"))
		{
			pItem->byQualityIndex = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Cost"))
		{
			pItem->dwCost = READ_DWORD( bstrData, 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Level"))
		{
			pItem->byLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"System_Effect_", wcslen(L"System_Effect_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"System_Effect_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pItem->system_Effect[ i ] = READ_DWORD( bstrData );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Type_", wcslen(L"Type_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Type_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pItem->bAppliedInPercent[ i ] = READ_BOOL( bstrData, pstrDataName->c_str() );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Value_", wcslen(L"Value_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Value_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pItem->nValue[ i ] = READ_DWORD( bstrData );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Active_Effect"))
		{
			pItem->activeEffect = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Active_Rate"))
		{
			pItem->fActiveRate = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
			READ_STRING(bstrData, pItem->szNote, _countof(pItem->szNote));
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Scouter_Info_", wcslen(L"Scouter_Info_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Scouter_Info_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pItem->byScouterInfo[ i ] = READ_BYTE( bstrData, pstrDataName->c_str(), INVALID_BYTE );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
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


sTBLDAT* CItemOptionTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

bool CItemOptionTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload && bUpdate == false )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{
		sITEM_OPTION_TBLDAT* pTableData = new sITEM_OPTION_TBLDAT;
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

	//	printf("CItemOptionTable: pTableData->tblidx %d %d\n", pTableData->tblidx, pTableData->system_Effect[0]);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CItemOptionTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sITEM_OPTION_TBLDAT* pTableData = (sITEM_OPTION_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}