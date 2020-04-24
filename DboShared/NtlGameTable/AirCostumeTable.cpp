#include "StdAfx.h"
#include "AirCostumeTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"



WCHAR* CAirCostumeTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CAirCostumeTable::CAirCostumeTable(void)
{
	Init();
}

CAirCostumeTable::~CAirCostumeTable(void)
{
	Destroy();
}

bool CAirCostumeTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CAirCostumeTable::Destroy()
{
}

void CAirCostumeTable::Init()
{
}

void* CAirCostumeTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sAIR_COSTUME_TBLDAT* pNewHelp = new sAIR_COSTUME_TBLDAT;
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

bool CAirCostumeTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sAIR_COSTUME_TBLDAT* pHelp = (sAIR_COSTUME_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CAirCostumeTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sAIR_COSTUME_TBLDAT* pTbldat = (sAIR_COSTUME_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CAirCostumeTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sAIR_COSTUME_TBLDAT* pHelp = (sAIR_COSTUME_TBLDAT*)pvTable;

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


sTBLDAT* CAirCostumeTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CAirCostumeTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sAIR_COSTUME_TBLDAT* pTableData = new sAIR_COSTUME_TBLDAT;
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

bool CAirCostumeTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sAIR_COSTUME_TBLDAT* pTableData = (sAIR_COSTUME_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}