#include "StdAfx.h"
#include "NtlDebug.h"

#include "QuestTextDataTable.h"
#include "NtlSerializer.h"

WCHAR* CQuestTextDataTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CQuestTextDataTable::CQuestTextDataTable(void)
{
	Init();
}

CQuestTextDataTable::~CQuestTextDataTable(void)
{
	Destroy();
}

bool CQuestTextDataTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CQuestTextDataTable::Destroy()
{
}

void CQuestTextDataTable::Init()
{
}

void* CQuestTextDataTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = new sQUEST_TEXT_DATA_TBLDAT;
		if (NULL == pQuestTextData)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pQuestTextData;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pQuestTextData;
	}
	
	return NULL;
}

bool CQuestTextDataTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = (sQUEST_TEXT_DATA_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pQuestTextData, sizeof(*pQuestTextData)))
			return false;

		delete pQuestTextData;

		return true;
	}
	
	return false;
}

bool CQuestTextDataTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sQUEST_TEXT_DATA_TBLDAT* pTbldat = (sQUEST_TEXT_DATA_TBLDAT*)pvTable;
	if( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CQuestTextDataTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = (sQUEST_TEXT_DATA_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Quest_Text_Index"))
		{
			pQuestTextData->tblidx = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Quest_Text"))
		{
			if ( false == READ_STR( pQuestTextData->wstrText, bstrData) )
			{
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


sTBLDAT* CQuestTextDataTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

void CQuestTextDataTable::FindDataRange( TBLIDX tblBegin, TBLIDX tblEnd, std::vector<sQUEST_TEXT_DATA_TBLDAT*>& vecRangeList )
{
	TABLEIT iter = m_mapTableList.begin();
	for ( ; iter != m_mapTableList.end(); ++iter )
	{
		TBLIDX tblIdx = iter->first;

		if ( tblIdx < tblBegin )
			continue;

		if ( tblIdx > tblEnd )
			break;

		vecRangeList.push_back( (sQUEST_TEXT_DATA_TBLDAT*)iter->second );
	}
}

bool CQuestTextDataTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	while (0 < serializer.GetDataSize())
	{
		sQUEST_TEXT_DATA_TBLDAT* pTableData = new sQUEST_TEXT_DATA_TBLDAT;
		if (NULL == pTableData)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		WORD wTextLength = 0;
		if (serializer.GetDataSize() < sizeof(pTableData->tblidx) + sizeof(wTextLength))
		{
			//- yoshiki : To log system!
			return true;
		}
		serializer >> pTableData->tblidx;
		serializer >> wTextLength;

	//	printf("tblidx: %d, text-size: %d, text-length: %d \n", pTableData->tblidx, wTextLength, (int)(wTextLength * sizeof(WCHAR)));

		if (serializer.GetDataSize() < (int)(wTextLength * sizeof(WCHAR)))
		{
			return true;
		}
		
		WCHAR* pwszText = new WCHAR[wTextLength + 1];
		if (NULL == pwszText)
		{
			return true;
		}
		serializer.Out(pwszText, wTextLength * sizeof(WCHAR));
		pwszText[wTextLength] = L'\0';

		pTableData->wstrText = pwszText;

		delete [] pwszText;

		//  [4/26/2008 zeroera] : 설명 : 실패하더라도 Load의 종료여부는 File Loading에서 결정한다
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}
	};

	return true;
}

bool CQuestTextDataTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sQUEST_TEXT_DATA_TBLDAT* pTableData = (sQUEST_TEXT_DATA_TBLDAT*)(iter->second);

		serializer << pTableData->tblidx;
		serializer << (WORD)((pTableData->wstrText).size());
		serializer.In((pTableData->wstrText).c_str(), (int)((pTableData->wstrText).size() * sizeof(WCHAR)));
	}

	return true;
}