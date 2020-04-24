#include "StdAfx.h"
#include "ItemGroupListTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"
#include "NtlRandom.h"


WCHAR* CItemGroupListTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CItemGroupListTable::CItemGroupListTable(void)
{
	Init();
}

CItemGroupListTable::~CItemGroupListTable(void)
{
	Destroy();
}

bool CItemGroupListTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CItemGroupListTable::Destroy()
{
}

void CItemGroupListTable::Init()
{
}

void* CItemGroupListTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_GROUP_LIST_TBLDAT* pNewHelp = new sITEM_GROUP_LIST_TBLDAT;
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

bool CItemGroupListTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_GROUP_LIST_TBLDAT* pHelp = (sITEM_GROUP_LIST_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CItemGroupListTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sITEM_GROUP_LIST_TBLDAT* pTbldat = (sITEM_GROUP_LIST_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	if (pTbldat->mob_Index != INVALID_TBLIDX && pTbldat->mob_Index > 0)
		AddItemGroupListByMobTblidx(pTbldat->mob_Index, pTbldat);
	else if (pTbldat->dwWorld_Rule_Type != INVALID_DWORD && pTbldat->dwWorld_Rule_Type > 0)
		AddItemGroupListByWorldRuleType(pTbldat->dwWorld_Rule_Type, pTbldat->byLevel, pTbldat);

	if (pTbldat->dwMob_Type != INVALID_DWORD && pTbldat->dwMob_Type > 0)
		AddItemGroupListByMobType(pTbldat->dwMob_Type, pTbldat->byLevel, pTbldat);

	return true;
}

bool CItemGroupListTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_GROUP_LIST_TBLDAT* pHelp = (sITEM_GROUP_LIST_TBLDAT*)pvTable;

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

void CItemGroupListTable::AddItemGroupListByWorldRuleType(DWORD dwWorldRuleType, BYTE byMaxLevel, sITEM_GROUP_LIST_TBLDAT* ItemGroupList)
{
	for (BYTE byWorldRuleType = 0; byWorldRuleType < GAMERULE_TYPE_COUNT; byWorldRuleType++)
	{
		if (byWorldRuleType >= dwWorldRuleType)
			break;

		if ((dwWorldRuleType & (1 << byWorldRuleType)) != 0)
			m_mmapItemGroupByWorldRuleType[byWorldRuleType].insert(itemWorldRuleGroup::value_type(byMaxLevel, ItemGroupList));
	}
}

void CItemGroupListTable::AddItemGroupListByMobType(DWORD dwMobType, BYTE byMaxLevel, sITEM_GROUP_LIST_TBLDAT* ItemGroupList)
{
	for (int k = 0; k < MOB_TYPE_COUNT; k++)
	{
		if ((dwMobType & ( 1 << k)) != 0)
		{
			m_mmapItemGroupByMobType[k].insert(itemMobTypeGroup::value_type(byMaxLevel, ItemGroupList));
		}
	}
}

void CItemGroupListTable::AddItemGroupListByMobTblidx(TBLIDX mobTblidx, sITEM_GROUP_LIST_TBLDAT* ItemGroupList)
{
	m_mmapItemGroupByMobTblidx.insert(itemMobTblidxGroup::value_type(mobTblidx, ItemGroupList));
}


sTBLDAT* CItemGroupListTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

void CItemGroupListTable::AccumulateItemGroupListByWorldRuleType(BYTE byMobLevel, BYTE byWorldRuleType, BYTE byMaxItemLevel, std::deque<sITEM_GROUP_LIST_TBLDAT*> & rdequeItemGroupList)
{
	if (byWorldRuleType >= GAMERULE_TYPE_COUNT)
		return;

	std::deque<sITEM_GROUP_LIST_TBLDAT*> tempDeque;
	BYTE byTryLevelMin = 0;
	if (byMobLevel >= 5)
		byTryLevelMin = byMobLevel - 5;

	itemWorldRuleGroup::iterator lower = m_mmapItemGroupByWorldRuleType[byWorldRuleType].lower_bound(byTryLevelMin);

	for (; lower != m_mmapItemGroupByWorldRuleType[byWorldRuleType].end(); lower++)
	{
		sITEM_GROUP_LIST_TBLDAT* tbldat = lower->second;

		if (tbldat->byLevel > byMaxItemLevel || tbldat->byLevel - byTryLevelMin > 10)
			break;

		tempDeque.push_back(tbldat);
	}

	if (tempDeque.size() > 0)
		rdequeItemGroupList.push_back(tempDeque[RandomRange(0, (int)(tempDeque.size() - 1))]);
}

void CItemGroupListTable::AccumulateItemGroupListByMobType(BYTE byMobLevel, BYTE byMobType, BYTE byMaxItemLevel, std::deque<sITEM_GROUP_LIST_TBLDAT*> &rdequeItemGroupList)
{
	if (byMobType >= MOB_TYPE_COUNT)
		return;

	BYTE byTryLevelMin = 0;
	if (byMobLevel >= 5)
		byTryLevelMin = byMobLevel - 5;

	std::deque<sITEM_GROUP_LIST_TBLDAT*> tempDeque;
	itemMobTypeGroup::iterator lower = m_mmapItemGroupByMobType[byMobType].lower_bound(byTryLevelMin);

	for (; lower != m_mmapItemGroupByMobType[byMobType].end(); lower++)
	{
		sITEM_GROUP_LIST_TBLDAT* tbldat = lower->second;

		if (tbldat->byLevel > byMaxItemLevel || tbldat->byLevel - byTryLevelMin > 10)
			break;

		tempDeque.push_back(tbldat);
	}

	if (tempDeque.size() > 0)
		rdequeItemGroupList.push_back(tempDeque[RandomRange(0, (int)(tempDeque.size() - 1))]);

}

void CItemGroupListTable::AccumulateItemGroupListByMobTblidx(BYTE byMobLevel, TBLIDX mobTblidx, std::deque<sITEM_GROUP_LIST_TBLDAT*> &rdequeItemGroupList)
{
	BYTE byTryLevelMin = 0, byTryLevelMax = 0;

	if (byMobLevel >= 5)
	{
		if (byMobLevel <= 250)
		{
			byTryLevelMin = byMobLevel - 5;
			byTryLevelMax = byMobLevel + 5;
		}
		else
		{
			byTryLevelMin = byMobLevel - 5;
			byTryLevelMax = INVALID_BYTE;
		}
	}
	else
	{
		byTryLevelMin = 0;
		byTryLevelMax = byMobLevel + 5;
	}


	itemMobTblidxGroup::iterator iterLowerBound = m_mmapItemGroupByMobTblidx.lower_bound(mobTblidx);
	itemMobTblidxGroup::iterator iterUpperBound = m_mmapItemGroupByMobTblidx.upper_bound(mobTblidx);

	for (itemMobTblidxGroup::iterator iter = iterLowerBound; iter != iterUpperBound; iter++)
	{
		sITEM_GROUP_LIST_TBLDAT* pItemGroupListTableData = iter->second;
		bool bIsTableDataCandidate = false;

		if (pItemGroupListTableData->byLevel == INVALID_BYTE)
		{
			bIsTableDataCandidate = true;
		}
		else if (byTryLevelMin <= pItemGroupListTableData->byLevel && byTryLevelMax >= pItemGroupListTableData->byLevel)
		{
			bIsTableDataCandidate = true;
		}

		if (bIsTableDataCandidate)
		{
			rdequeItemGroupList.push_back(pItemGroupListTableData);
		}
	}
}


bool CItemGroupListTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sITEM_GROUP_LIST_TBLDAT* pTableData = new sITEM_GROUP_LIST_TBLDAT;
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

	//	printf("CItemGroupListTable: pTableData->tblidx %d \n", pTableData->tblidx);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CItemGroupListTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sITEM_GROUP_LIST_TBLDAT* pTableData = (sITEM_GROUP_LIST_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}