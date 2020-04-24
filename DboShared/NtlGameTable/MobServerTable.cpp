#include "StdAfx.h"
#include "MobServerTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"



WCHAR* CMobServerTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CMobServerTable::CMobServerTable(void)
{
	Init();
}

CMobServerTable::~CMobServerTable(void)
{
	Destroy();
}

bool CMobServerTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CMobServerTable::Destroy()
{
}

void CMobServerTable::Init()
{
}

void* CMobServerTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOB_SERVER_TBLDAT* pNewHelp = new sMOB_SERVER_TBLDAT;
		if (NULL == pNewHelp)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewHelp;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewHelp;
	}

	return NULL;
}

bool CMobServerTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOB_SERVER_TBLDAT* pHelp = (sMOB_SERVER_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CMobServerTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sMOB_SERVER_TBLDAT* pTbldat = (sMOB_SERVER_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CMobServerTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOB_SERVER_TBLDAT* pHelp = (sMOB_SERVER_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pHelp->tblidx = READ_DWORD( bstrData );
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


sTBLDAT* CMobServerTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CMobServerTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sMOB_SERVER_TBLDAT* pTableData = new sMOB_SERVER_TBLDAT;
		if (NULL == pTableData)
		{
			Destroy();
			return false;
		}

		if (false == pTableData->LoadFromBinary(serializer))
		{
			delete pTableData;
			bLoop = false;
			break;
		}

	/*	printf("pTableData->tblidx %d %d | %d %d %d | %d %d %d |\n", pTableData->tblidx, 
			pTableData->wUnknown,
			pTableData->byUnknown2,pTableData->byUnknown3,pTableData->byUnknown4,
			pTableData->wUnknown5,pTableData->wUnknown6,pTableData->wUnknown7
			);
	*/
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CMobServerTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sMOB_SERVER_TBLDAT* pTableData = (sMOB_SERVER_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}