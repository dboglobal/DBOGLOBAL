#include "StdAfx.h"
#include "MascotStatusTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"



WCHAR* CMascotStatusTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CMascotStatusTable::CMascotStatusTable(void)
{
	Init();
}

CMascotStatusTable::~CMascotStatusTable(void)
{
	Destroy();
}

bool CMascotStatusTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CMascotStatusTable::Destroy()
{
}

void CMascotStatusTable::Init()
{
}

void* CMascotStatusTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMASCOT_STATUS_TBLDAT* pNewHelp = new sMASCOT_STATUS_TBLDAT;
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

bool CMascotStatusTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMASCOT_STATUS_TBLDAT* pHelp = (sMASCOT_STATUS_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CMascotStatusTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sMASCOT_STATUS_TBLDAT* pTbldat = (sMASCOT_STATUS_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CMascotStatusTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMASCOT_STATUS_TBLDAT* pHelp = (sMASCOT_STATUS_TBLDAT*)pvTable;

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


sTBLDAT* CMascotStatusTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CMascotStatusTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sMASCOT_STATUS_TBLDAT* pTableData = new sMASCOT_STATUS_TBLDAT;
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

		/*
		printf("pTableData->tblidx %d %d %d %d %d %d %d %d %d %d %d ( %d %d %d )\n", pTableData->tblidx, 
			pTableData->byRank, pTableData->bySlot_Num,
			pTableData->wUnknown, pTableData->wMax_SP,
			pTableData->unkn, pTableData->unsdfk,
			pTableData->wUnknown3, pTableData->wUnknown4,
			pTableData->dwUnknown5, pTableData->dwUnknown6,
			pTableData->trash, pTableData->trash2, pTableData->trash3
			);
		*/
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CMascotStatusTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sMASCOT_STATUS_TBLDAT* pTableData = (sMASCOT_STATUS_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}