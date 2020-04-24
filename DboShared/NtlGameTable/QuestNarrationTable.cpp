//***********************************************************************************
//
//	File		:	QuestNarrationTable.cpp
//
//	Begin		:	2008-08-21
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Chung, DooSup (mailto:john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "stdafx.h"
#include "QuestNarrationTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

WCHAR* CQuestNarrationTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CQuestNarrationTable::CQuestNarrationTable( void )
{
	Init();
}

CQuestNarrationTable::~CQuestNarrationTable( void )
{
	Destroy();
}

bool CQuestNarrationTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CQuestNarrationTable::Destroy( void )
{
}

void CQuestNarrationTable::Init( void )
{
}

void* CQuestNarrationTable::AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sQUEST_NARRATION_TBLDAT* pNewObj = new sQUEST_NARRATION_TBLDAT;
		if ( NULL == pNewObj ) return NULL;

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

bool CQuestNarrationTable::DeallocNewTable( void* pvTable, WCHAR* pwszSheetName )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sQUEST_NARRATION_TBLDAT* pObj = (sQUEST_NARRATION_TBLDAT*)pvTable;
		if ( IsBadReadPtr( pObj, sizeof(*pObj) ) ) return false;

		delete pObj;

		return true;
	}

	return false;
}

bool CQuestNarrationTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sQUEST_NARRATION_TBLDAT* pTbldat = (sQUEST_NARRATION_TBLDAT*)pvTable;
	if ( false == m_mapTableList.insert( std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat) ).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CQuestNarrationTable::SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sQUEST_NARRATION_TBLDAT* pTbldat = (sQUEST_NARRATION_TBLDAT*)pvTable;

		if ( 0 == wcscmp( pstrDataName->c_str(), L"Tblidx" ) )
		{
			pTbldat->tblidx = READ_TBLIDX( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Type" ) )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pTbldat->bType = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Number" ) )
		{
			pTbldat->byNumber = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Time" ) )
		{
			pTbldat->byTime = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"UIShowHideDirection_", wcslen(L"UIShowHideDirection_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_OF_NARRATION; i++ )
			{
				swprintf( szBuffer, 1024, L"UIShowHideDirection_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pTbldat->asData[i].byUIShowHideDirection = READ_BYTE( bstrData, pstrDataName->c_str() );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"OwnerType_", wcslen(L"OwnerType_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_OF_NARRATION; i++ )
			{
				swprintf( szBuffer, 1024, L"OwnerType_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pTbldat->asData[i].byOwnerType = READ_BYTE( bstrData, pstrDataName->c_str() );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Owner_", wcslen(L"Owner_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_OF_NARRATION; i++ )
			{
				swprintf( szBuffer, 1024, L"Owner_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pTbldat->asData[i].dwOwner = READ_DWORD( bstrData );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Condition_", wcslen(L"Condition_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_OF_NARRATION; i++ )
			{
				swprintf( szBuffer, 1024, L"Condition_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pTbldat->asData[i].byCondition = READ_BYTE( bstrData, pstrDataName->c_str() );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Direction_", wcslen(L"Direction_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_OF_NARRATION; i++ )
			{
				swprintf( szBuffer, 1024, L"Direction_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pTbldat->asData[i].byDirection = READ_BYTE( bstrData, pstrDataName->c_str() );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Dialog_", wcslen(L"Dialog_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_OF_NARRATION; i++ )
			{
				swprintf( szBuffer, 1024, L"Dialog_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pTbldat->asData[i].dwDialog = READ_DWORD( bstrData );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"UiType_", wcslen(L"UiType_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_OF_NARRATION; i++ )
			{
				swprintf( szBuffer, 1024, L"UiType_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pTbldat->asData[i].byUiType = READ_BYTE( bstrData, pstrDataName->c_str() );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"UIDirection_", wcslen(L"UIDirection_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_OF_NARRATION; i++ )
			{
				swprintf( szBuffer, 1024, L"UIDirection_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pTbldat->asData[i].byUIDirection = READ_BYTE( bstrData, pstrDataName->c_str() );

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


sTBLDAT* CQuestNarrationTable::FindData( TBLIDX tblidx )
{
	if ( 0 == tblidx ) return NULL;

	TABLEIT iter = m_mapTableList.find( tblidx );
	if ( m_mapTableList.end() == iter ) return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CQuestNarrationTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sQUEST_NARRATION_TBLDAT* pTableData = new sQUEST_NARRATION_TBLDAT;
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

bool CQuestNarrationTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sQUEST_NARRATION_TBLDAT* pTableData = (sQUEST_NARRATION_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}