#include "stdafx.h"
#include "ChatServer.h"
#include "AuctionHouse.h"
#include "NtlPacketGT.h"
#include "NtlPacketTG.h"
#include "NtlPacketTQ.h"
#include "NtlMail.h"
#include "Utils.h"
#include "NtlMarket.h"


struct sAuction_sorting
{
	BYTE bySortType;

	sAuction_sorting(BYTE sortType) { bySortType = sortType; }

	inline bool operator() (const sTENKAICHIDAISIJYOU_DATA* struct1, const sTENKAICHIDAISIJYOU_DATA* struct2)
	{
		switch (bySortType)
		{
			case eAUCTION_SORT_TYPE_RANK_ASC:
			{
				return (struct1->byRank < struct2->byRank);
			}
			break;
			case eAUCTION_SORT_TYPE_NAME_DESC:
			{
				return wcscmp(struct1->awchItemName, struct2->awchItemName) == 0;
			}
			break;
			case eAUCTION_SORT_TYPE_PRICE_DESC:
			{
				return (struct1->dwPrice / struct1->byCount > struct2->dwPrice / struct2->byCount);
			}
			break;
			case eAUCTION_SORT_TYPE_LEVEL_DESC:
			{
				return (struct1->byItemLevel > struct2->byItemLevel);
			}
			break;
			case eAUCTION_SORT_TYPE_RANK_DESC:
			{
				return (struct1->byRank > struct2->byRank);
			}
			break;
			case eAUCTION_SORT_TYPE_NAME_ASC:
			{
				return wcscmp(struct2->awchItemName, struct1->awchItemName) > 0;
			}
			break;
			case eAUCTION_SORT_TYPE_PRICE_ASC:
			{
				return (struct1->dwPrice / struct1->byCount < struct2->dwPrice / struct2->byCount);
			}
			break;
			case eAUCTION_SORT_TYPE_LEVEL_ASC:
			{
				return (struct1->byItemLevel < struct2->byItemLevel);
			}
			break;
			case eAUCTION_SORT_TYPE_TIMELEFT_ASC:
			{
				return (struct1->nStartSellTime > struct2->nStartSellTime);
			}
			break;
			case eAUCTION_SORT_TYPE_TIMELEFT_DESC:
			{
				return (struct1->nStartSellTime < struct2->nStartSellTime);
			}
			break;

			default:
			{
				return true;
			}
			break;
		}
	}
};


CAutionhouse::CAutionhouse()
{
	Init();
}

CAutionhouse::~CAutionhouse()
{
}


void CAutionhouse::Init()
{
	CChatServer * app = (CChatServer*)g_pApp;
	
	CNtlPacket packet(sizeof(sTQ_TENKAICHIDAISIJYOU_SERVERDATA_REQ));
	sTQ_TENKAICHIDAISIJYOU_SERVERDATA_REQ * res = (sTQ_TENKAICHIDAISIJYOU_SERVERDATA_REQ *)packet.GetPacketData();
	res->wOpCode = TQ_TENKAICHIDAISIJYOU_SERVERDATA_REQ;
	res->bDBConnect = false;
	packet.SetPacketLen(sizeof(sTQ_TENKAICHIDAISIJYOU_SERVERDATA_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet);
}

void CAutionhouse::Update()
{
	CChatServer* app = (CChatServer*)g_pApp;
	DBOTIME curTime = time(NULL);
	std::vector<sTENKAICHIDAISIJYOU_DATA*> vecDelData;

	for (auto it = m_mapAuctionhouse.begin(); it != m_mapAuctionhouse.end(); )
	{
		sTENKAICHIDAISIJYOU_DATA* pData = it->second;

		//DBOTIME dwSecondsDif = UnsignedSafeDecrease<DBOTIME>(curTime, pData->nStartSellTime);

		pData->nEndSellTime = UnsignedSafeDecrease<DBOTIME>(pData->nEndSellTime, CAutionhouse::UPDATE_AUCTIONHOUSE_TICK / 1000);

		//if (dwSecondsDif > pData->nEndSellTime)
		if(pData->nEndSellTime == 0)
		{
			it = m_mapAuctionhouse.erase(it);
			vecDelData.push_back(pData);
		}
		else ++it;
	}


	for (std::vector<sTENKAICHIDAISIJYOU_DATA*>::iterator it = vecDelData.begin(); it != vecDelData.end(); it++)
	{
		sTENKAICHIDAISIJYOU_DATA* pData = *it;

		CNtlPacket packet(sizeof(sTQ_TENKAICHIDAISIJYOU_PERIODEND_REQ));
		sTQ_TENKAICHIDAISIJYOU_PERIODEND_REQ * res = (sTQ_TENKAICHIDAISIJYOU_PERIODEND_REQ *)packet.GetPacketData();
		res->wOpCode = TQ_TENKAICHIDAISIJYOU_PERIODEND_REQ;
		res->charId = pData->charId;
		res->nItem = pData->nItem;
		res->bySenderType = eMAIL_SENDER_TYPE_SYSTEM;
		res->byMailType = eMAIL_TYPE_ITEM;
		NTL_SAFE_WCSCPY(res->wchText, L"Auction House Sale End");
		res->itemId = pData->itemId;
		res->dwPrice = pData->dwPrice;
		packet.SetPacketLen(sizeof(sTQ_TENKAICHIDAISIJYOU_PERIODEND_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		delete pData;
	}

	vecDelData.clear();
}

void CAutionhouse::AddItem(sTENKAICHIDAISIJYOU_DATA& rData)
{
	sTENKAICHIDAISIJYOU_DATA* pData = new sTENKAICHIDAISIJYOU_DATA;
	memcpy(pData, &rData, sizeof(sTENKAICHIDAISIJYOU_DATA));
	m_mapAuctionhouse.insert(std::make_pair(pData->nItem, pData));
}

void CAutionhouse::DelItem(ITEMID nItem)
{
	auto it = m_mapAuctionhouse.find(nItem);
	if (it != m_mapAuctionhouse.end())
	{
		delete it->second;
		m_mapAuctionhouse.erase(it);
	}
}

void CAutionhouse::LoadAuctionHouseData(CPlayer* pPlayer, sGT_TENKAICHIDAISIJYOU_LIST_REQ* req)
{
	int i = 0;
	BYTE byListNum = 0;
	unsigned int uiCurPage = 1;
	unsigned int uiMaxPage = 1;
	std::vector<sTENKAICHIDAISIJYOU_DATA*> temp;

	//printf("byPage: %i, byTabType: %u, uiPage: %u, bySortType: %u, byClass:%u, byItemType:%u, byRank:%u, byMinLevel:%u,byMaxLevel:%u \n",
	//	req->byPage, req->byTabType, req->uiPage, req->bySortType, req->byClass, req->byItemType, req->byRank, req->byMinLevel, req->byMaxLevel);

	HSESSION gameServerSession = g_pServerInfoManager->GetGsSession(pPlayer->GetChannel());
	//ERR_LOG(LOG_USER, "Account %u load auction house. Channel %u, Channel-Session %u", req->accountId, pPlayer->GetChannel(), gameServerSession);

	for (auto it = m_mapAuctionhouse.begin(); it != m_mapAuctionhouse.end(); it++)
	{
		sTENKAICHIDAISIJYOU_DATA* data = it->second;
		
		if (req->byPage == eAUCTION_TAB_TYPE_BUY)
		{
			if (req->bySortType == eAUCTION_SORT_TYPE_NONE)
				temp.push_back(it->second);
			else
			{
				std::string strSearch;
				std::string strItemName;

				if (wcslen(req->awchItemName) > 0)
				{
					strSearch = ws2s(req->awchItemName);
					strItemName = ws2s(data->awchItemName);
				}

				if (
					(req->byTabType == INVALID_BYTE || req->byTabType == data->byTabType) &&
					(req->byItemType == INVALID_BYTE || req->byItemType == data->byItemType) &&
					(req->byClass == INVALID_BYTE || Dbo_CheckClass(req->byClass, data->dwNeed_Class_Bit_Flag)) &&
					(req->byMinLevel == INVALID_BYTE || req->byMinLevel <= data->byItemLevel) &&
					(req->byMaxLevel == INVALID_BYTE || req->byMaxLevel >= data->byItemLevel) &&
					(req->byRank == INVALID_BYTE || req->byRank == data->byRank) &&
					(wcslen(req->awchItemName) == 0 || strItemName.find(strSearch) != std::string::npos)
					)
				{
					temp.push_back(it->second);
				}
			}
		}
		else if (req->byPage == eAUCTION_TAB_TYPE_SELL)
		{
			if (data->charId == pPlayer->GetCharID())
			{
				temp.push_back(it->second);
			}
		}
	}

	//printf("load auction house \n sorttype %u tabtype %u itemtype %u class %u min-level %u max-level %u rank %u uiPage %u\n", req->bySortType, req->byTabType, req->byItemType, req->byClass, req->byMinLevel, req->byMaxLevel, req->byRank, req->uiPage);

	//do sort the 10 items we will show
	if (req->bySortType != eAUCTION_SORT_TYPE_NONE)
		std::sort(temp.begin(), temp.end(), sAuction_sorting(req->bySortType));

	//add 10 items to temp list and calculate max pages
	for (std::vector<sTENKAICHIDAISIJYOU_DATA*>::iterator it2 = temp.begin(); it2 != temp.end(); it2++)
	{
		if (byListNum <= DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE && uiCurPage == req->uiPage) //only show max 10 items and items from the page we are at
		{
			byListNum++;

			sTENKAICHIDAISIJYOU_DATA* data = *it2;

			CNtlPacket packet(sizeof(sTG_TENKAICHIDAISIJYOU_LIST_DATA));
			sTG_TENKAICHIDAISIJYOU_LIST_DATA * res = (sTG_TENKAICHIDAISIJYOU_LIST_DATA *)packet.GetPacketData();
			res->wOpCode = TG_TENKAICHIDAISIJYOU_LIST_DATA;
			res->charId = req->charId;
			res->sData.nItem = data->nItem;
			res->sData.byTabType = data->byTabType;
			res->sData.byItemType = data->byItemType;
			res->sData.byItemLevel = data->byItemLevel;
			wcscpy(res->sData.awchSeller, data->awchSeller);
			res->sData.dwPrice = data->dwPrice;
			res->sData.nStartSellTime = data->nStartSellTime;
			res->sData.nEndSellTime = data->nEndSellTime;
			res->sData.itemNo = data->itemNo;
			res->sData.byCount = data->byCount;
			res->sData.dwNeed_Class_Bit_Flag = data->dwNeed_Class_Bit_Flag;
			res->sData.byRank = data->byRank;
			res->sData.byGrade = data->byGrade;
			res->sData.byCurrentDurability = data->byCurrentDurability;
			res->sData.byBattleAttribute = data->byBattleAttribute;
			wcscpy(res->sData.awchMaker, data->awchMaker);
			memcpy(&res->sData.sOptionSet, &data->sOptionSet, sizeof(sITEM_OPTION_SET));
			res->sData.nUseEndTime = data->nUseEndTime;
			res->sData.byRestrictState = data->byRestrictState;
			packet.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_LIST_DATA));
			g_pApp->Send(gameServerSession, &packet);
		}

		++i;

		if (i == DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE)
		{
			++uiCurPage;
		}

		if (i == DBO_MAX_MARKET_ITEM_COUNT_PER_PAGE + 1) //page counter // +1 so we increase page only if more than limit exist example only when 11 and not 10 exist.
		{
			uiMaxPage++;
			i = 0;
		}
	}

	CNtlPacket packet2(sizeof(sTG_TENKAICHIDAISIJYOU_LIST_RES));
	sTG_TENKAICHIDAISIJYOU_LIST_RES * res2 = (sTG_TENKAICHIDAISIJYOU_LIST_RES *)packet2.GetPacketData();
	res2->wOpCode = TG_TENKAICHIDAISIJYOU_LIST_RES;
	res2->byPage = req->byPage;
	res2->byListNum = byListNum; 
	res2->charId = req->charId;
	res2->uiMaxPage = uiMaxPage;
	res2->uiPage = req->uiPage;
	res2->wResultCode = GAME_SUCCESS;
	packet2.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_LIST_RES));
	g_pApp->Send(gameServerSession, &packet2);
}

DWORD CAutionhouse::ReturnPrice(ITEMID nItem)
{
	auto it = m_mapAuctionhouse.find(nItem);
	if (it != m_mapAuctionhouse.end())
		return it->second->dwPrice;

	return INVALID_DWORD;
}

sTENKAICHIDAISIJYOU_DATA* CAutionhouse::GetItem(ITEMID nitem)
{
	auto it = m_mapAuctionhouse.find(nitem);
	if (it != m_mapAuctionhouse.end())
	{
		return it->second;
	}

	return NULL;
}



