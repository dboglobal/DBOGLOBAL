#include "StdAfx.h"
#include "ScriptLinkTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CScriptLinkTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CScriptLinkTable::CScriptLinkTable(void)
{
	Init();
}

CScriptLinkTable::~CScriptLinkTable(void)
{
	Destroy();
}

bool CScriptLinkTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CScriptLinkTable::Destroy()
{
}

void CScriptLinkTable::Init()
{
}

void* CScriptLinkTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSCRIPT_LINK_TBLDAT* pLink = new sSCRIPT_LINK_TBLDAT;
		if (NULL == pLink)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pLink;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pLink;
	}

	return NULL;
}

bool CScriptLinkTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSCRIPT_LINK_TBLDAT* pLink = (sSCRIPT_LINK_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pLink, sizeof(*pLink)))
			return false;

		delete pLink;

		return true;
	}

	return false;
}

bool CScriptLinkTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sSCRIPT_LINK_TBLDAT* pTbldat = (sSCRIPT_LINK_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CScriptLinkTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSCRIPT_LINK_TBLDAT* pLink = (sSCRIPT_LINK_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pLink->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value"))
		{
			READ_STRINGW(bstrData, pLink->wszValue, _countof(pLink->wszValue));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Type"))
		{
			pLink->byType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Action"))
		{
			pLink->byAction = READ_BYTE( bstrData, pstrDataName->c_str() );
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


sTBLDAT* CScriptLinkTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CScriptLinkTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sSCRIPT_LINK_TBLDAT* pTableData = new sSCRIPT_LINK_TBLDAT;
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

bool CScriptLinkTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sSCRIPT_LINK_TBLDAT* pTableData = (sSCRIPT_LINK_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}