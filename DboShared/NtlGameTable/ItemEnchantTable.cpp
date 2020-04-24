#include "StdAfx.h"
#include "ItemEnchantTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"
#include "NtlBitFlagManager.h"
#include "NtlRandom.h"

WCHAR* CItemEnchantTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CItemEnchantTable::CItemEnchantTable(void)
{
	Init();
}

CItemEnchantTable::~CItemEnchantTable(void)
{
	Destroy();
}

bool CItemEnchantTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CItemEnchantTable::Destroy()
{
}

void CItemEnchantTable::Init()
{
	for (int j = 0; j < ITEM_RANK_COUNT; ++j)
		m_vecByRank[j].clear();
}

void* CItemEnchantTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_ENCHANT_TBLDAT* pNewHelp = new sITEM_ENCHANT_TBLDAT;
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

bool CItemEnchantTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_ENCHANT_TBLDAT* pHelp = (sITEM_ENCHANT_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CItemEnchantTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);;

	sITEM_ENCHANT_TBLDAT* pTbldat = (sITEM_ENCHANT_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}
	
	if (pTbldat->bIsSuperior)
		m_vecByRank[ITEM_RANK_SUPERIOR].push_back(pTbldat);
	if (pTbldat->bIsExcellent)
		m_vecByRank[ITEM_RANK_EXCELLENT].push_back(pTbldat);
	if (pTbldat->bIsRare)
		m_vecByRank[ITEM_RANK_RARE].push_back(pTbldat);
	if (pTbldat->bIsLegendary)
		m_vecByRank[ITEM_RANK_LEGENDARY].push_back(pTbldat);

	if (pTbldat->byGroupNo)
		m_mmapGroupNo.insert(ENCHANTGROUP_VAL(pTbldat->byKind, pTbldat->byGroupNo));

	return true;
}

bool CItemEnchantTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_ENCHANT_TBLDAT* pHelp = (sITEM_ENCHANT_TBLDAT*)pvTable;

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


sTBLDAT* CItemEnchantTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}


bool CItemEnchantTable::GetGroupNoForRandomOption(BYTE byItemType, BYTE *pbyGroupNo)
{
	ENCHANTGROUP::iterator beg = m_mmapGroupNo.lower_bound(byItemType);
	ENCHANTGROUP::iterator end = m_mmapGroupNo.upper_bound(byItemType);

	int nCount = 0;

	for (ENCHANTGROUP::iterator iter = beg; iter != end; iter++)
	{
		nCount++;
	}

	if (nCount == 0)
		return false;

	int nRandomValue = RandomRangeU(0, nCount - 1);

	ENCHANTGROUP::iterator iter = m_mmapGroupNo.begin();;
	std::advance(iter, nRandomValue);

	*pbyGroupNo = iter->second;
	printf("GetGroupNoForRandomOption: first %u second %u \n", iter->first, iter->second);

	return true;
}


bool CItemEnchantTable::FindOrVerifyRandomEnchantData(sITEM_TBLDAT *pItemTableData, BYTE byItemRank, int nRandomOptionCount, int nRandomOptionIndex, CNtlBitFlagManager *pFlag, float *pfRemainingItemWorth, sITEM_RANDOM_OPTION& sItemRandomOption)
{
	if (!pItemTableData || !pFlag || !pfRemainingItemWorth)
		return false;

	if (byItemRank < ITEM_RANK_SUPERIOR || byItemRank > ITEM_RANK_LEGENDARY)
		return false;
	
	DWORD dwItemCategoryFlag = 1 << Dbo_GetItemCategoryByItemType(pItemTableData->byItem_Type);
	sITEM_ENCHANT_TBLDAT* pFinalEnchantTableData = NULL;

	if (sItemRandomOption.wOptionIndex != INVALID_WORD)
	{
		pFinalEnchantTableData = (sITEM_ENCHANT_TBLDAT*)FindData(sItemRandomOption.wOptionIndex);
		if (!pFinalEnchantTableData)
		{
			printf("(NULL == pFinalEnchantTableData), pItemTableData->tblidx = %u sItemRandomOption.wOptionIndex = %u \n", pItemTableData->tblidx, sItemRandomOption.wOptionIndex);
			return false;
		}

		if (!IsProperItemOption(pFinalEnchantTableData, sItemRandomOption.optionValue, dwItemCategoryFlag, pItemTableData->byNeed_Min_Level, *pfRemainingItemWorth, pFlag))
			return false;

		*pfRemainingItemWorth -= (float)((int)pFinalEnchantTableData->wEnchant_Value * sItemRandomOption.optionValue);
		pFlag->Set(pFinalEnchantTableData->byExclIdx);

		return true;
	}

	DWORD dwTotalFrequency = 0;
	std::vector<sITEM_ENCHANT_TBLDAT*> vecTableData;

	for (std::vector<sITEM_ENCHANT_TBLDAT *>::iterator it = m_vecByRank[byItemRank].begin(); it != m_vecByRank[byItemRank].end(); it++)
	{
		sITEM_ENCHANT_TBLDAT* pItemEnchantTableData = *it;

		if (IsProperItemOption(pItemEnchantTableData, -1, dwItemCategoryFlag, pItemTableData->byNeed_Min_Level, *pfRemainingItemWorth, pFlag))
		{
			vecTableData.push_back(pItemEnchantTableData);
			dwTotalFrequency += (DWORD)pItemEnchantTableData->byFrequency;
		}
	}

	if (vecTableData.size())
	{
		DWORD dwRandomValue = RandomRangeU(1, dwTotalFrequency);

		for (std::vector<sITEM_ENCHANT_TBLDAT*>::iterator it = vecTableData.begin(); it != vecTableData.end(); it++)
		{
			sITEM_ENCHANT_TBLDAT* pItemEnchantTableData = *it;

			if (dwRandomValue <= pItemEnchantTableData->byFrequency)
			{
				pFinalEnchantTableData = pItemEnchantTableData;
				break;
			}

			dwRandomValue -= (DWORD)pItemEnchantTableData->byFrequency;
		}

		if (!pFinalEnchantTableData)
		{
			printf("(NULL == pFinalEnchantTableData), pItemTableData->tblidx = %u, dwTotalFrequency = %u, dwRandomValue = %u \n", pItemTableData->tblidx, dwTotalFrequency, dwRandomValue);
			return false;
		}

		sItemRandomOption.wOptionIndex = (WORD)pFinalEnchantTableData->tblidx;

		int maxOptionValue = (int)floor(*pfRemainingItemWorth) / (int)pFinalEnchantTableData->wEnchant_Value;
		if (maxOptionValue <= 0)
		{
			printf("The max option value is not supposed to be 0., pItemTableData->tblidx = %u, pFinalEnchantTableData->tblidx = %u, *pfRemainingItemWorth = %f, pFinalEnchantTableData->wEnchant_Value = %u",
				pItemTableData->tblidx, pFinalEnchantTableData->tblidx, *pfRemainingItemWorth, pFinalEnchantTableData->wEnchant_Value);
			return false;
		}

		if (nRandomOptionCount - 1 <= nRandomOptionIndex)
			sItemRandomOption.optionValue = maxOptionValue;
		else
			sItemRandomOption.optionValue = RandomRange(1, maxOptionValue);

		*pfRemainingItemWorth -= (float)((int)pFinalEnchantTableData->wEnchant_Value * sItemRandomOption.optionValue);
		pFlag->Set(pFinalEnchantTableData->byExclIdx);
	}

	if (nRandomOptionIndex == 0)
		return false;

	return true;
}


bool CItemEnchantTable::ProcessRandomOption(sITEM_TBLDAT* itemtbl, BYTE byItemRank, bool bFirst, bool bLast, CNtlBitFlagManager* flagmgr, float* pfRemainingWorth, sITEM_RANDOM_OPTION &randomOption)
{
	if (!itemtbl || !flagmgr || !pfRemainingWorth)
		return false;
	
	if (byItemRank < ITEM_RANK_SUPERIOR || byItemRank > ITEM_RANK_LEGENDARY)
		return false;

	DWORD dwItemCategoryFlag = 1 << Dbo_GetItemCategoryByItemType(itemtbl->byItem_Type);
	
	std::vector<sITEM_ENCHANT_TBLDAT*> vecTableData;
	sITEM_ENCHANT_TBLDAT* pFinalEnchantTableData = NULL;
	
	if (randomOption.wOptionIndex == INVALID_WORD)
	{
		DWORD dwTotalFrequency = 0;

		for (std::vector<sITEM_ENCHANT_TBLDAT *>::iterator it = m_vecByRank[byItemRank].begin(); it != m_vecByRank[byItemRank].end(); it++)
		{
			sITEM_ENCHANT_TBLDAT* pItemEnchantTableData = *it;

			if (IsProperItemOption(pItemEnchantTableData, -1, dwItemCategoryFlag, itemtbl->byNeed_Min_Level, *pfRemainingWorth, flagmgr))
			{
				vecTableData.push_back(pItemEnchantTableData);
				dwTotalFrequency += (DWORD)pItemEnchantTableData->byFrequency;
			}
		}
	//	printf("vecTableData.size() %u \n", vecTableData.size());
		if (vecTableData.size() == 0)
			return bFirst == false || bLast == false;

		DWORD dwRandomValue = RandomRangeU(1, dwTotalFrequency);

		for (std::vector<sITEM_ENCHANT_TBLDAT *>::iterator it = vecTableData.begin(); it != vecTableData.end(); it++)
		{
			sITEM_ENCHANT_TBLDAT* pItemEnchantTableData = *it;

			if (dwRandomValue <= pItemEnchantTableData->byFrequency)
			{
				pFinalEnchantTableData = pItemEnchantTableData;
				break;
			}

			dwRandomValue -= (DWORD)pItemEnchantTableData->byFrequency;
		}

		if (pFinalEnchantTableData == NULL)
		{
			printf("(NULL == pFinalEnchantTableData), itemtbl->tblidx = %u, dwTotalFrequency = %u, dwRandomValue = %u \n", itemtbl->tblidx, dwTotalFrequency, dwRandomValue);
			return false;
		}
		//printf("dwItemCategoryFlag %u, tblidx %u, byExclIdx %u, byKind %u \n", dwItemCategoryFlag, pFinalEnchantTableData->tblidx, pFinalEnchantTableData->byExclIdx, pFinalEnchantTableData->byKind);
		randomOption.wOptionIndex = (WORD)pFinalEnchantTableData->tblidx;

		int maxOptionValue = (int)*pfRemainingWorth / (int)pFinalEnchantTableData->wEnchant_Value;

		if (maxOptionValue <= 0)
		{
			printf("The max option value is not supposed to be 0., itemtbl->tblidx = %u, pFinalEnchantTableData->tblidx = %u, *pfRemainingWorth = %f, pFinalEnchantTableData->wEnchant_Value = %u \n",
				itemtbl->tblidx, pFinalEnchantTableData->tblidx, *pfRemainingWorth, pFinalEnchantTableData->wEnchant_Value);
			return false;
		}
		//printf("maxOptionValue %u \n", maxOptionValue);
		if (bLast)
			randomOption.optionValue = maxOptionValue;
		else
			randomOption.optionValue = RandomRange(1, maxOptionValue);
	}
	else
	{
		pFinalEnchantTableData = (sITEM_ENCHANT_TBLDAT*)FindData(randomOption.wOptionIndex);
		if (pFinalEnchantTableData == NULL)
		{
			printf("(NULL == pFinalEnchantTableData), itemtbl->tblidx = %u \n", itemtbl->tblidx);
			return false;
		}

		if (pFinalEnchantTableData->wEnchant_Value == 0)
		{
			printf("pFinalEnchantTableData->wEnchant_Value is zero \n");
			return false;
		}

		int maxOptionValue = (int)floor(*pfRemainingWorth) / (int)pFinalEnchantTableData->wEnchant_Value;

		randomOption.optionValue = maxOptionValue; // RandomRange(1, maxOptionValue);

		//if (!IsProperItemOption(pFinalEnchantTableData, randomOption.optionValue, dwItemCategoryFlag, itemtbl->byNeed_Min_Level, *pfRemainingWorth, flagmgr))
		//	return false;
	}

	*pfRemainingWorth -= (float)((int)pFinalEnchantTableData->wEnchant_Value * randomOption.optionValue);
	flagmgr->Set(pFinalEnchantTableData->byExclIdx);

	return true;
}


bool CItemEnchantTable::FindOrVerifyRandomSocketEnchantData(sITEM_TBLDAT *pItemTableData, BYTE byItemRank, int nRandomOptionCount, int nIndex, CNtlBitFlagManager *pFlag, float *pfRemainingItemWorth, sITEM_RANDOM_OPTION& sItemRandomOption, DWORD CostumeEquipPostion)
{
	if (!pItemTableData || !pFlag || !pfRemainingItemWorth)
		return false;

	if (byItemRank < ITEM_RANK_SUPERIOR || byItemRank > ITEM_RANK_LEGENDARY)
		return false;

	DWORD dwItemCategoryFlag = 1 << Dbo_GetItemCategoryByItemType(pItemTableData->byItem_Type);

	std::vector<sITEM_ENCHANT_TBLDAT*> vecTableData;
	sITEM_ENCHANT_TBLDAT* pFinalEnchantTableData = NULL;

	if (sItemRandomOption.wOptionIndex != INVALID_WORD)
	{
		pFinalEnchantTableData = (sITEM_ENCHANT_TBLDAT*)FindData(sItemRandomOption.wOptionIndex);
		if (pFinalEnchantTableData == NULL)
		{
			printf("(NULL == pFinalEnchantTableData), pItemTableData->tblidx = %u", pItemTableData->tblidx);
			return false;
		}

		if (!IsProperItemOption(pFinalEnchantTableData, sItemRandomOption.optionValue, dwItemCategoryFlag, pItemTableData->byNeed_Min_Level, *pfRemainingItemWorth, pFlag))
			return false;

		*pfRemainingItemWorth -= (float)((int)pFinalEnchantTableData->wEnchant_Value * sItemRandomOption.optionValue);
		pFlag->Set(pFinalEnchantTableData->byExclIdx);

		return true;
	}

	DWORD dwTotalFrequency = 0;

	for (std::vector<sITEM_ENCHANT_TBLDAT *>::iterator it = m_vecByRank[byItemRank].begin(); it != m_vecByRank[byItemRank].end(); it++)
	{
		sITEM_ENCHANT_TBLDAT* pItemEnchantTableData = *it;

		if ((pItemEnchantTableData->dwEquip & CostumeEquipPostion) != 0)
		{
			if (IsProperItemOption(pItemEnchantTableData, -1, dwItemCategoryFlag, pItemTableData->byNeed_Min_Level, *pfRemainingItemWorth, pFlag))
			{
				vecTableData.push_back(pItemEnchantTableData);
				dwTotalFrequency += (DWORD)pItemEnchantTableData->byFrequency;
			}
		}
	}

	if (vecTableData.size())
	{
		DWORD dwRandomValue = RandomRangeU(1, dwTotalFrequency);

		for (std::vector<sITEM_ENCHANT_TBLDAT *>::iterator it = vecTableData.begin(); it != vecTableData.end(); it++)
		{
			sITEM_ENCHANT_TBLDAT* pItemEnchantTableData = *it;

			if (dwRandomValue <= (DWORD)pItemEnchantTableData->byFrequency)
			{
				pFinalEnchantTableData = pItemEnchantTableData;
				break;
			}

			dwRandomValue -= (DWORD)pItemEnchantTableData->byFrequency;
		}

		if (pFinalEnchantTableData == NULL)
		{
			printf("(NULL == pFinalEnchantTableData), pItemTableData->tblidx = %u, dwTotalFrequency = %u, dwRandomValue = %u \n", pItemTableData->tblidx, dwTotalFrequency, dwRandomValue);
			return false;
		}

		sItemRandomOption.wOptionIndex = (WORD)pFinalEnchantTableData->tblidx;

		int maxOptionValue = (int)floor(*pfRemainingItemWorth) / (int)pFinalEnchantTableData->wEnchant_Value;

		if (maxOptionValue <= 0)
		{
			printf("The max option value is not supposed to be 0., pItemTableData->tblidx = %u, pFinalEnchantTableData->tblidx = %u, *pfRemainingWorth = %f, pFinalEnchantTableData->wEnchant_Value = %u \n",
				pItemTableData->tblidx, pFinalEnchantTableData->tblidx, *pfRemainingItemWorth, pFinalEnchantTableData->wEnchant_Value);
			return false;
		}

		if (nRandomOptionCount - 1 <= nIndex)
			sItemRandomOption.optionValue = maxOptionValue;
		else
			sItemRandomOption.optionValue = RandomRange(1, maxOptionValue);

		*pfRemainingItemWorth -= (float)((int)pFinalEnchantTableData->wEnchant_Value * sItemRandomOption.optionValue);
		pFlag->Set(pFinalEnchantTableData->byExclIdx);

		return true;
	}

	if (nIndex == 0)
		return false;

	return true;
}


bool CItemEnchantTable::IsValidOption(BYTE byRank, int nMaxRandomOptionCount, sITEM_TBLDAT *pItemTableData, sITEM_RANDOM_OPTION_SET& rItemRandomOption)
{
	if (pItemTableData->bIsCanHaveOption || rItemRandomOption.GetRandomOption(0).wOptionIndex != INVALID_WORD)
	{
		if (pItemTableData->bIsCanHaveOption == false || pItemTableData->Item_Option_Tblidx == INVALID_TBLIDX)
		{
			BYTE byCurRandomOptionCount = 0;

			for (int i = 0; i < NTL_MAX_RANDOM_OPTION_IN_ITEM && rItemRandomOption.GetRandomOption((BYTE)i).wOptionIndex != INVALID_WORD; i++)
				byCurRandomOptionCount++;

			if (byRank != ITEM_RANK_SUPERIOR || byCurRandomOptionCount > 0)
			{
				if (byCurRandomOptionCount <= (BYTE)nMaxRandomOptionCount)
				{
					DWORD dwItemCategoryFlag = 1 << Dbo_GetItemCategoryByItemType(pItemTableData->byItem_Type);

					for (int i = 0; i < byCurRandomOptionCount; i++)
					{
						bool bIsValid = false;

						for (std::vector<sITEM_ENCHANT_TBLDAT *>::iterator it = m_vecByRank[byRank].begin(); it != m_vecByRank[byRank].end(); it++)
						{
							sITEM_ENCHANT_TBLDAT* pItemEnchantTableData = *it;

							if ((pItemEnchantTableData->dwEquip & dwItemCategoryFlag) != 0 && pItemTableData->byNeed_Min_Level >= pItemEnchantTableData->byMinLevel)
							{
								bIsValid = true;
							}
						}

						if (bIsValid == false)
							return false;
					}
				}
				else return false;
			}
		}
		else
		{
			return rItemRandomOption.GetRandomOption(0).wOptionIndex == INVALID_WORD;
		}
	}

	return true;
}


bool CItemEnchantTable::GetVectorRandomOptionEnchantData(sITEM_TBLDAT *pItemTableData, BYTE byItemRank, std::vector<sITEM_ENCHANT_TBLDAT *> *pvecData)
{
	if (!pItemTableData || !pvecData)
		return false;

	if (byItemRank < ITEM_RANK_SUPERIOR || byItemRank > ITEM_RANK_LEGENDARY)
		return false;

	CNtlBitFlagManager flag;
	flag.Create(256);

	float fItemWorth = Dbo_GetItemWorth(byItemRank, pItemTableData->byNeed_Min_Level);
	DWORD dwItemCategoryFlag = 1 << Dbo_GetItemCategoryByItemType(pItemTableData->byItem_Type);

	for (std::vector<sITEM_ENCHANT_TBLDAT *>::iterator it = m_vecByRank[byItemRank].begin(); it != m_vecByRank[byItemRank].end(); it++)
	{
		sITEM_ENCHANT_TBLDAT* pItemEnchantTableData = *it;

		if (IsProperItemOption(pItemEnchantTableData, -1, dwItemCategoryFlag, pItemTableData->byNeed_Min_Level, fItemWorth, &flag))
		{
			pvecData->push_back(pItemEnchantTableData);
		}
	}

	if (pvecData->size() == 0)
	{
		return false;
	}

	return true;
}


bool CItemEnchantTable::IsProperItemOption(sITEM_ENCHANT_TBLDAT * pItemEnchantTableData, int optionValue, DWORD dwItemCategoryFlag, BYTE byItemLevel, float fRemainingItemWorth, CNtlBitFlagManager* pFlag)
{
	if ((pItemEnchantTableData->dwEquip & dwItemCategoryFlag) != 0)
	{
		if (byItemLevel >= pItemEnchantTableData->byMinLevel)
		{
			if (pItemEnchantTableData->byMaxLevel == INVALID_BYTE || byItemLevel <= pItemEnchantTableData->byMaxLevel)
			{
				if (optionValue == -1)
					optionValue = 1;

				if ((int)floor(fRemainingItemWorth) >= (optionValue * (int)pItemEnchantTableData->wEnchant_Value))
				{
					if (pFlag->IsSet(pItemEnchantTableData->byExclIdx))
						return false;
					else
					{
						return (TBLIDX)optionValue <= pItemEnchantTableData->tblidx;
					}
				}
			}
		}
	}

	return false;
}

bool CItemEnchantTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sITEM_ENCHANT_TBLDAT* pTableData = new sITEM_ENCHANT_TBLDAT;
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

bool CItemEnchantTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sITEM_ENCHANT_TBLDAT* pTableData = (sITEM_ENCHANT_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}