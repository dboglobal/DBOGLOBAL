#include "StdAfx.h"
#include "DragonBallReturnPointTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"



WCHAR* CDragonBallReturnPointTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CDragonBallReturnPointTable::CDragonBallReturnPointTable(void)
{
	Init();
}

CDragonBallReturnPointTable::~CDragonBallReturnPointTable(void)
{
	Destroy();
}

bool CDragonBallReturnPointTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CDragonBallReturnPointTable::Destroy()
{
}

void CDragonBallReturnPointTable::Init()
{
}

void* CDragonBallReturnPointTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDRAGONBALL_RETURN_POINT_TBLDAT* pNewHelp = new sDRAGONBALL_RETURN_POINT_TBLDAT;
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

bool CDragonBallReturnPointTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDRAGONBALL_RETURN_POINT_TBLDAT* pHelp = (sDRAGONBALL_RETURN_POINT_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CDragonBallReturnPointTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sDRAGONBALL_RETURN_POINT_TBLDAT* pTbldat = (sDRAGONBALL_RETURN_POINT_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CDragonBallReturnPointTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDRAGONBALL_RETURN_POINT_TBLDAT* pHelp = (sDRAGONBALL_RETURN_POINT_TBLDAT*)pvTable;

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


sTBLDAT* CDragonBallReturnPointTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CDragonBallReturnPointTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sDRAGONBALL_RETURN_POINT_TBLDAT* pTableData = new sDRAGONBALL_RETURN_POINT_TBLDAT;
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

	//	printf("pTableData->tblidx %d \n", pTableData->tblidx);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CDragonBallReturnPointTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sDRAGONBALL_RETURN_POINT_TBLDAT* pTableData = (sDRAGONBALL_RETURN_POINT_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}