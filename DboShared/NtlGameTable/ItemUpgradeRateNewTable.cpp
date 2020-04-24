#include "StdAfx.h"
#include "ItemUpgradeRateNewTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"


WCHAR* CItemUpgradeRateNewTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CItemUpgradeRateNewTable::CItemUpgradeRateNewTable(void)
{
	Init();
}

CItemUpgradeRateNewTable::~CItemUpgradeRateNewTable(void)
{
	Destroy();
}

bool CItemUpgradeRateNewTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CItemUpgradeRateNewTable::Destroy()
{
}

void CItemUpgradeRateNewTable::Init()
{
}

void* CItemUpgradeRateNewTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_UPGRADE_RATE_NEW_TBLDAT* pNewHelp = new sITEM_UPGRADE_RATE_NEW_TBLDAT;
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

bool CItemUpgradeRateNewTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_UPGRADE_RATE_NEW_TBLDAT* pHelp = (sITEM_UPGRADE_RATE_NEW_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CItemUpgradeRateNewTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sITEM_UPGRADE_RATE_NEW_TBLDAT* pTbldat = (sITEM_UPGRADE_RATE_NEW_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	if (pTbldat->byGrade < NTL_ITEM_MAX_GRADE)
	{
		if (false == SetUpgradeRateNewTbldat(pTbldat->byItem_Type, pTbldat->byGrade, pTbldat))
		{
			_ASSERTE(0);
			m_mapTableList.erase(pTbldat->tblidx);
			return false;
		}
	}

	return true;
}

bool CItemUpgradeRateNewTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_UPGRADE_RATE_NEW_TBLDAT* pHelp = (sITEM_UPGRADE_RATE_NEW_TBLDAT*)pvTable;

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


sTBLDAT* CItemUpgradeRateNewTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}


//-----------------------------------------------------------------------------------
//		Purpose	:	get upgrade rate by item grade and item type
//		Return	:	sITEM_UPGRADE_RATE_NEW_TBLDAT table data
//-----------------------------------------------------------------------------------
sTBLDAT * CItemUpgradeRateNewTable::GetUpgradeRateNewTbldat(BYTE byItem_Type, BYTE byGrade)
{
	if (byGrade >= NTL_ITEM_MAX_GRADE)
	{
		return NULL;
	}

	if (byItem_Type > 1)
	{
		return NULL;
	}

	return m_aUpgradeRateNewTbldat[byItem_Type][byGrade];
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CItemUpgradeRateNewTable::SetUpgradeRateNewTbldat(BYTE byItem_Type, BYTE byGrade, sTBLDAT * pTbldat)
{

	m_aUpgradeRateNewTbldat[byItem_Type][byGrade] = pTbldat;

	return true;
}

bool CItemUpgradeRateNewTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sITEM_UPGRADE_RATE_NEW_TBLDAT* pTableData = new sITEM_UPGRADE_RATE_NEW_TBLDAT;
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
		printf("pTableData->tblidx %d |%d %d| %f | %f %f %f | %f | %f | \n", 
			pTableData->tblidx,
			pTableData->byItem_Type, pTableData->byGrade,
			pTableData->fAdditional_Ability,
			pTableData->fHigh_Upgrade_Success_1, pTableData->fHigh_Upgrade_Success_2, pTableData->fHigh_Upgrade_Success_3,
			pTableData->fUnknown, pTableData->fUnknown2);
*/
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CItemUpgradeRateNewTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sITEM_UPGRADE_RATE_NEW_TBLDAT* pTableData = (sITEM_UPGRADE_RATE_NEW_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}