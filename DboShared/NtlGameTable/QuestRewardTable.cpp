#include "StdAfx.h"
#include "NtlDebug.h"
#include "QuestRewardTable.h"
#include "NtlSerializer.h"


WCHAR* CQuestRewardTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};


CQuestRewardTable::CQuestRewardTable(void)
{
	Init();
}

CQuestRewardTable::~CQuestRewardTable(void)
{
	Destroy();
}

bool CQuestRewardTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CQuestRewardTable::Destroy()
{
}

void CQuestRewardTable::Init()
{
}

void* CQuestRewardTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sQUEST_REWARD_TBLDAT* pQuestReward = new sQUEST_REWARD_TBLDAT;
		if (NULL == pQuestReward)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pQuestReward;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pQuestReward;
	}
	
	return NULL;
}

bool CQuestRewardTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sQUEST_REWARD_TBLDAT* pQuestReward = (sQUEST_REWARD_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pQuestReward, sizeof(*pQuestReward)))
			return false;

		delete pQuestReward;

		return true;
	}

	return false;
}

bool CQuestRewardTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sQUEST_REWARD_TBLDAT* pQuestReward = (sQUEST_REWARD_TBLDAT*)pvTable;

	if( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pQuestReward->tblidx, pQuestReward)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pQuestReward->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CQuestRewardTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sQUEST_REWARD_TBLDAT* pQuestReward = (sQUEST_REWARD_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pQuestReward->tblidx = READ_TBLIDX(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_EXP"))
		{
			pQuestReward->dwDef_Reward_EXP = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Zeny"))
		{
			pQuestReward->dwDef_Reward_Zeny = READ_DWORD(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Type_0"))
		{
			pQuestReward->arsDefRwd[0].byRewardType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Idx_0"))
		{
			pQuestReward->arsDefRwd[0].dwRewardIdx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Val_0"))
		{
			pQuestReward->arsDefRwd[0].dwRewardVal = READ_DWORD(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Type_1"))
		{
			pQuestReward->arsDefRwd[1].byRewardType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Idx_1"))
		{
			pQuestReward->arsDefRwd[1].dwRewardIdx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Val_1"))
		{
			pQuestReward->arsDefRwd[1].dwRewardVal = READ_DWORD(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Type_2"))
		{
			pQuestReward->arsDefRwd[2].byRewardType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Idx_2"))
		{
			pQuestReward->arsDefRwd[2].dwRewardIdx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Val_2"))
		{
			pQuestReward->arsDefRwd[2].dwRewardVal = READ_DWORD(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Type_3"))
		{
			pQuestReward->arsDefRwd[3].byRewardType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Idx_3"))
		{
			pQuestReward->arsDefRwd[3].dwRewardIdx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Def_Reward_Val_3"))
		{
			pQuestReward->arsDefRwd[3].dwRewardVal = READ_DWORD(bstrData);
		}


		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Type_0"))
		{
			pQuestReward->arsSelRwd[0].byRewardType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Idx_0"))
		{
			pQuestReward->arsSelRwd[0].dwRewardIdx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Val_0"))
		{
			pQuestReward->arsSelRwd[0].dwRewardVal = READ_DWORD(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Type_1"))
		{
			pQuestReward->arsSelRwd[1].byRewardType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Idx_1"))
		{
			pQuestReward->arsSelRwd[1].dwRewardIdx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Val_1"))
		{
			pQuestReward->arsSelRwd[1].dwRewardVal = READ_DWORD(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Type_2"))
		{
			pQuestReward->arsSelRwd[2].byRewardType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Idx_2"))
		{
			pQuestReward->arsSelRwd[2].dwRewardIdx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Val_2"))
		{
			pQuestReward->arsSelRwd[2].dwRewardVal = READ_DWORD(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Type_3"))
		{
			pQuestReward->arsSelRwd[3].byRewardType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Idx_3"))
		{
			pQuestReward->arsSelRwd[3].dwRewardIdx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sel_Reward_Val_3"))
		{
			pQuestReward->arsSelRwd[3].dwRewardVal = READ_DWORD(bstrData);
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


sTBLDAT* CQuestRewardTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

bool CQuestRewardTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sQUEST_REWARD_TBLDAT* pTableData = new sQUEST_REWARD_TBLDAT;
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

bool CQuestRewardTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sQUEST_REWARD_TBLDAT* pTableData = (sQUEST_REWARD_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}