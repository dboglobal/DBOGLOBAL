#include "stdafx.h"
#include "AuctionHouse.h"
#include "QueryServer.h"


CAutionhouse::CAutionhouse()
{
	Init();
}

CAutionhouse::~CAutionhouse()
{
	Destroy();
}


void CAutionhouse::Init()
{
	ullHighestAuctionID = 0;

	smart_ptr<QueryResult> result = GetCharDB.Query("SELECT * FROM auctionhouse");
	if (result)
	{
		do
		{
			Field* f = result->Fetch();

			smart_ptr<QueryResult> item = GetCharDB.Query("SELECT * FROM items WHERE id=%I64u", f[6].GetUInt64());
			if (item)
			{
				Field* fi = item->Fetch();

				sTENKAICHIDAISIJYOU_DATA* data = new sTENKAICHIDAISIJYOU_DATA;

				data->nItem = f[0].GetUInt64();
				data->charId = f[1].GetUInt32();
				data->byTabType = f[2].GetBYTE();
				data->byItemType = f[11].GetBYTE();
				data->byItemLevel = f[9].GetBYTE();
				NTL_SAFE_WCSCPY(data->awchItemName, s2ws(f[3].GetString()).c_str());
				NTL_SAFE_WCSCPY(data->awchSeller, s2ws(f[4].GetString()).c_str());
				data->dwPrice = f[5].GetDWORD();
				data->nStartSellTime = (DBOTIME)f[7].GetUInt64();
				data->nEndSellTime = (DBOTIME)f[8].GetUInt64();
				data->itemId = f[6].GetUInt64();
				data->itemNo = fi[1].GetUInt32();
				data->byCount = fi[5].GetBYTE();
				data->dwNeed_Class_Bit_Flag = f[10].GetDWORD();
				data->byRank = fi[6].GetBYTE();
				data->byGrade = fi[8].GetBYTE();
				data->byCurrentDurability = fi[7].GetBYTE();
				data->byBattleAttribute = fi[10].GetBYTE();
				NTL_SAFE_WCSCPY(data->awchMaker, s2ws(fi[11].GetString()).c_str());
				data->sOptionSet.aOptionTblidx[0] = fi[12].GetUInt32();
				data->sOptionSet.aOptionTblidx[1] = fi[13].GetUInt32();
				data->sOptionSet.aRandomOption[0].wOptionIndex = fi[14].GetWORD();
				data->sOptionSet.aRandomOption[0].optionValue = fi[15].GetINT();
				data->sOptionSet.aRandomOption[1].wOptionIndex = fi[16].GetWORD();
				data->sOptionSet.aRandomOption[1].optionValue = fi[17].GetINT();
				data->sOptionSet.aRandomOption[2].wOptionIndex = fi[18].GetWORD();
				data->sOptionSet.aRandomOption[2].optionValue = fi[19].GetINT();
				data->sOptionSet.aRandomOption[3].wOptionIndex = fi[20].GetWORD();
				data->sOptionSet.aRandomOption[3].optionValue = fi[21].GetINT();
				data->sOptionSet.aRandomOption[4].wOptionIndex = fi[22].GetWORD();
				data->sOptionSet.aRandomOption[4].optionValue = fi[23].GetINT();
				data->sOptionSet.aRandomOption[5].wOptionIndex = fi[24].GetWORD();
				data->sOptionSet.aRandomOption[5].optionValue = fi[25].GetINT();
				data->sOptionSet.aRandomOption[6].wOptionIndex = fi[26].GetWORD();
				data->sOptionSet.aRandomOption[6].optionValue = fi[27].GetINT();
				data->sOptionSet.aRandomOption[7].wOptionIndex = fi[28].GetWORD();
				data->sOptionSet.aRandomOption[7].optionValue = fi[29].GetINT();
				data->nUseEndTime = (DBOTIME)fi[31].GetUInt64();
				data->byRestrictState = fi[32].GetBYTE();

				m_mapAuctionhouse.insert(std::make_pair(data->nItem, data));

				if (data->nItem > ullHighestAuctionID)
					ullHighestAuctionID = data->nItem;
			}
			else
			{
				GetCharDB.Execute("DELETE FROM auctionhouse WHERE id=%I64u", f[0].GetUInt64());
			}

		} while (result->NextRow());
	}
}


void CAutionhouse::Destroy()
{
	for (auto it = m_mapAuctionhouse.begin(); it != m_mapAuctionhouse.end(); )
	{
		delete it->second;

		it = m_mapAuctionhouse.erase(it);
	}
}

void CAutionhouse::InsertItem(sTENKAICHIDAISIJYOU_DATA* pData)
{
	m_mapAuctionhouse.insert(std::make_pair(pData->nItem, pData));
}

void CAutionhouse::EraseItem(ITEMID nItem)
{
	m_mapAuctionhouse.erase(nItem);
}

sTENKAICHIDAISIJYOU_DATA * CAutionhouse::GetItem(ITEMID nItem)
{
	auto it = m_mapAuctionhouse.find(nItem);
	if (it != m_mapAuctionhouse.end())
		return it->second;

	return nullptr;
}

