#include "StdAfx.h"
#include "MobMovePatternTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CMobMovePatternTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CMobMovePatternTable::CMobMovePatternTable(void)
{
	Init();
}

CMobMovePatternTable::~CMobMovePatternTable(void)
{
	Destroy();
}

bool CMobMovePatternTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CMobMovePatternTable::Destroy()
{
}

void CMobMovePatternTable::Init()
{
}

void* CMobMovePatternTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOVE_PATTERN_TBLDAT* pPattern = new sMOVE_PATTERN_TBLDAT;
		if (NULL == pPattern)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pPattern;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pPattern;
	}

	return NULL;
}

bool CMobMovePatternTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOVE_PATTERN_TBLDAT* pPattern = (sMOVE_PATTERN_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pPattern, sizeof(*pPattern)))
			return false;

		delete pPattern;

		return true;
	}

	return false;
}

bool CMobMovePatternTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bUpdate);

	sMOVE_PATTERN_TBLDAT * pTbldat = (sMOVE_PATTERN_TBLDAT*) pvTable;
	sMOVE_PATTERN_TBLDAT * pExistTbldat = NULL;

	// Reload인경우 Data를 찾아 Update해준다
	if( bReload )
	{
		pExistTbldat = (sMOVE_PATTERN_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 해제를 위한 false 반환
			return true; 
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

bool CMobMovePatternTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOVE_PATTERN_TBLDAT* pPattern = (sMOVE_PATTERN_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pPattern->tblidx = READ_DWORD( bstrData );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Pattern_", wcslen(L"Pattern_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_MOVE_PATTERN; i++ )
			{
				swprintf( szBuffer, 1024, L"Pattern_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pPattern->abyPattern[ i ] = READ_BYTE( bstrData, pstrDataName->c_str() );

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
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
//			READ_STR( pPattern->wstrNote, bstrData);	// 메모리에 로드하지 않는다.
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


sTBLDAT* CMobMovePatternTable::FindData(TBLIDX tblidx)
{
	if (INVALID_TBLIDX == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CMobMovePatternTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sMOVE_PATTERN_TBLDAT* pTableData = new sMOVE_PATTERN_TBLDAT;
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

bool CMobMovePatternTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sMOVE_PATTERN_TBLDAT* pTableData = (sMOVE_PATTERN_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}