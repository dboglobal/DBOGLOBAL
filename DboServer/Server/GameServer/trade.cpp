#include "Stdafx.h"
#include "trade.h"
#include "CPlayer.h"
#include "item.h"
#include "ItemManager.h"
#include "NtlPacketGU.h"
#include "NtlResultCode.h"
#include "NtlPacketGQ.h"
#include "GameServer.h"


unsigned int TRADE_BLOCK_AFTER_UPDATE_TICK = 2000;

CTradeManager::CTradeManager()
{
	m_dwTradeCount = 0;
}

CTradeManager::~CTradeManager()
{
}

void CTradeManager::TickProcess(DWORD dwCurTick) //should be called once every second
{
	for (TSETTRADE::iterator it = m_set_Trade.begin(); it != m_set_Trade.end();)
	{
		CTrade* trade = *it;
		if (trade)
		{
			if(trade->GetTradeState() == eTRADE_STATE_WAIT_FOR_ACCEPT)
			{
				if(trade->GetStartTick() + NTL_TRADE_REPLY_WAIT_TIME < dwCurTick) //check if request time ended
				{
					EndTradeByTimeOut(trade);
					it = m_set_Trade.erase(it);
					continue;
				}
			}
			else if (trade->GetTradeState() == eTRADE_STATE_FINISHING_TRADE)
			{
			}
		}
		
		++it;
	}
}



void CTradeManager::StartTrade(CPlayer* requestor, CPlayer* target)
{
	m_dwTradeCount++;

	CTrade* trade = new CTrade(requestor);
	m_set_Trade.insert(trade);

	CTrade* trade2 = new CTrade(target);
	m_set_Trade.insert(trade2);

	requestor->GetTrade()->SetCompany(target->GetTrade());
	target->GetTrade()->SetCompany(requestor->GetTrade());

	//SEND REQUEST WINDOW TO TARGET
	CNtlPacket packet(sizeof(sGU_TRADE_OK_REQ));
	sGU_TRADE_OK_REQ * res = (sGU_TRADE_OK_REQ *)packet.GetPacketData();
	res->handle = requestor->GetID();
	res->wOpCode = GU_TRADE_OK_REQ;
	packet.SetPacketLen(sizeof(sGU_TRADE_OK_REQ));
	target->SendPacket(&packet);
}

void CTradeManager::DestroyTrade(CTrade* trade)
{
	if (trade)
	{
		TSETTRADE::iterator it = m_set_Trade.find(trade);
		if (m_set_Trade.end() == it)
		{
			ERR_LOG(LOG_SYSTEM, "Cant destroy trade. Trade not found.");
			return;
		}

		m_set_Trade.erase(*it);
		SAFE_DELETE(trade);
	}
}

void CTradeManager::TradeEnd(CTrade* trade)
{
	if (trade)
	{
		if (trade->GetOwner())
		{
			if (trade->GetTradeState() == eTRADE_STATE_WAIT_FOR_ACCEPT)
			{
				CNtlPacket packet(sizeof(sGU_TRADE_START_NFY));
				sGU_TRADE_START_NFY* res = (sGU_TRADE_START_NFY *)packet.GetPacketData();
				res->wResultCode = GAME_TRADE_DENY_USE;
				res->wOpCode = GU_TRADE_START_NFY;
				packet.SetPacketLen(sizeof(sGU_TRADE_START_NFY));
				trade->GetOwner()->SendPacket(&packet);
			}
			else
			{
				CNtlPacket packet2(sizeof(sGU_TRADE_CANCEL_NFY));
				sGU_TRADE_CANCEL_NFY* res2 = (sGU_TRADE_CANCEL_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_TRADE_CANCEL_NFY;
				res2->wResultCode = GAME_SUCCESS;
				packet2.SetPacketLen(sizeof(sGU_TRADE_CANCEL_NFY));
				trade->GetOwner()->SendPacket(&packet2);
			}

			DestroyTrade(trade);
		}
	}
}

void CTradeManager::EndTradeByTimeOut(CTrade* trade)
{
	if (trade->GetOwner())
	{
		CNtlPacket packet(sizeof(sGU_TRADE_START_NFY));
		sGU_TRADE_START_NFY * res = (sGU_TRADE_START_NFY *)packet.GetPacketData();
		res->wOpCode = GU_TRADE_START_NFY;
		res->wResultCode = GAME_TRADE_REPLY_WAIT_OVER;
		res->handle = trade->GetOwner()->GetID();
		packet.SetPacketLen(sizeof(sGU_TRADE_START_NFY));
		trade->GetOwner()->SendPacket(&packet);
	}

	SAFE_DELETE(trade);
}

bool CTradeManager::FinishTrade(CTrade* trade)
{
	WORD wResult = GAME_SUCCESS;

	CTrade* trade_two = NULL;
	CPlayer* player_one = NULL;
	CPlayer* player_two = NULL;

	if (trade) //check if both trades still exist
	{
		trade_two = trade->GetCompany();
		if (trade_two)
		{
			player_one = trade->GetOwner();
			player_two = trade_two->GetOwner();

			if (player_one && player_one->IsInitialized() && player_two && player_two->IsInitialized()) //check if both player exist and initialized(online)
			{
				if (player_one->GetTrade()->GetCompany() == player_two->GetTrade()) //check if both trades are company
				{
					if (player_one->GetTrade()->IsReady() && player_two->GetTrade()->IsReady()) //check if both trades are ready
					{
						bool bHasPlayerOneEnoughSpace = player_one->GetPlayerItemContainer()->CountEmptyInventory() >= trade_two->GetItemCount();
						bool bHasPlayerTwoEnoughSpace = player_two->GetPlayerItemContainer()->CountEmptyInventory() >= trade->GetItemCount();

						if (bHasPlayerOneEnoughSpace && bHasPlayerTwoEnoughSpace) //check if both player have enough space
						{
							if (player_one->GetZeni() >= player_one->GetTrade()->GetZeni() && player_two->GetZeni() >= player_two->GetTrade()->GetZeni())
							{
								if (player_two->GetTrade()->GetZeni() == 0 || player_one->CanReceiveZeni(player_two->GetTrade()->GetZeni()))
								{
									if (player_one->GetTrade()->GetZeni() == 0 || player_two->CanReceiveZeni(player_one->GetTrade()->GetZeni()))
									{
										wResult = GAME_SUCCESS;
										trade->SetState(eTRADE_STATE_FINISHING_TRADE);
										trade_two->SetState(eTRADE_STATE_FINISHING_TRADE);
									}
									else wResult = GAME_ZENNY_OVER;
								}
								else wResult = GAME_ZENNY_OVER;
							}
							else wResult = GAME_ZENNY_NOT_ENOUGH;
						}
						else wResult = GAME_ITEM_INVEN_FULL;
					}
					else wResult = GAME_TRADE_WRONG_STATE;
				}
				else wResult = GAME_TRADE_WRONG_STATE;
			}
			else wResult = GAME_TRADE_WRONG_STATE;
		}
		else wResult = GAME_TRADE_WRONG_STATE;
	}
	else wResult = GAME_TRADE_WRONG_STATE;

	CNtlPacket packet(sizeof(sGU_TRADE_END_RES));
	sGU_TRADE_END_RES* res = (sGU_TRADE_END_RES *)packet.GetPacketData();
	res->wOpCode = GU_TRADE_END_RES;
	res->wResultCode = wResult;
	if (wResult == GAME_SUCCESS)
		res->hTarget = player_two->GetID();
	res->bIsSet = trade->IsReady();
	packet.SetPacketLen(sizeof(sGU_TRADE_END_RES));
	player_one->SendPacket(&packet);

	if (player_two && player_two->IsInitialized())
	{
		CNtlPacket packet2(sizeof(sGU_TRADE_END_NFY));
		sGU_TRADE_END_NFY* res2 = (sGU_TRADE_END_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_TRADE_END_NFY;
		res2->wResultCode = wResult;
		if (wResult == GAME_SUCCESS)
		{
			res2->hTarget = player_one->GetID();
			res2->bIsSet = trade_two->IsReady();
		}
		packet2.SetPacketLen(sizeof(sGU_TRADE_END_NFY));
		player_two->SendPacket(&packet2);

		CNtlPacket packet3(sizeof(sGU_TRADE_END_NFY));
		sGU_TRADE_END_NFY* res3 = (sGU_TRADE_END_NFY *)packet3.GetPacketData();
		res3->wOpCode = GU_TRADE_END_NFY;
		res3->wResultCode = wResult;
		if (wResult == GAME_SUCCESS)
		{
			res3->hTarget = player_two->GetID();
			res3->bIsSet = trade->IsReady();
		}
		packet3.SetPacketLen(sizeof(sGU_TRADE_END_NFY));
		player_one->SendPacket(&packet3);
	}
	
	if (wResult == GAME_SUCCESS)
	{
		trade->Trade();
		trade_two->Trade();

		DestroyTrade(trade);
		DestroyTrade(trade_two);

		return true;
	}

	return false;
}

/* ------------------------------------------------------------------------------------ */


CTrade::CTrade(CPlayer* owner)
{
	m_byTradeState = eTRADE_STATE_WAIT_FOR_ACCEPT;
	m_dwStartTick = GetTickCount();
	m_dwPacketCount = 0;
	m_bIsReady = false;
	m_dwZeni = 0;
	m_pCompany = NULL;
	m_dwBlockTime = 0;

	m_pOwner = owner;
	owner->SetTrade(this);
}

CTrade::~CTrade()
{
	if (m_pOwner)
		m_pOwner->SetTrade(NULL);

	m_pCompany = NULL;
	m_pOwner = NULL;
	
	for (MAPITEMDATA::iterator it = p_ItemData.begin(); it != p_ItemData.end(); it++)
	{
		CItem* realitem = NULL;
		if (m_pOwner)
			realitem = m_pOwner->GetPlayerItemContainer()->GetItem(it->first);
		else
			realitem = g_pItemManager->Find(it->first);

		if (realitem)
		{
			realitem->SetTrading(false);
		}
		else
		{
			ERR_LOG(LOG_USER, "ERROR!!! Cant find real-Item. Handle %u", it->first);
		}

		CItem* pItem = it->second;
		if(pItem)
		{
			if (pItem->IsTrading())	//Only delete item if it have not been traded
			{
				SAFE_DELETE(pItem);
			}
		}
		else
		{
			ERR_LOG(LOG_USER,"ERROR!!! Cant find Trade-Item. Handle %u", it->first);
		}
	}

	p_ItemData.clear();
}

void CTrade::AddItem(CItem* item, BYTE byCount)
{
	m_dwBlockTime = GetTickCount() + TRADE_BLOCK_AFTER_UPDATE_TICK; //block trading

	CItem* newitem = new CItem;
	newitem->SetTbldat(item->GetTbldat());
	newitem->SetID(item->GetID());
	newitem->SetItemData(&item->GetItemData());
	newitem->SetOwner(NULL);
	newitem->SetTrading(true);
	newitem->SetStackCount(byCount);

	p_ItemData.insert(std::make_pair(newitem->GetID(), newitem));
}

void CTrade::DelItem(HOBJECT hHandle)
{
	m_dwBlockTime = GetTickCount() + TRADE_BLOCK_AFTER_UPDATE_TICK; //block trading

	MAPITEMDATA::iterator it = p_ItemData.find(hHandle);
	if (p_ItemData.end() == it)
		return;

	CItem* realitem = m_pOwner->GetPlayerItemContainer()->GetItem(it->first);
	if (realitem)
	{
		realitem->SetTrading(false);
	}
	else
	{
		ERR_LOG(LOG_USER, "User %u tried to delete item from trade which does not exist anymore.", m_pOwner->GetCharID());
	}

	CItem* item = it->second;
	p_ItemData.erase(hHandle);
	SAFE_DELETE(item);
}

CItem* CTrade::FindItem(HOBJECT hHandle)
{
	MAPITEMDATA::iterator it = p_ItemData.find(hHandle);
	if (p_ItemData.end() == it)
		return NULL;

	return it->second;
}

void CTrade::UpdateZeni(DWORD zeni)
{
	m_dwZeni = zeni;
	m_dwBlockTime = GetTickCount() + TRADE_BLOCK_AFTER_UPDATE_TICK; //block trading
}

void CTrade::ModifyItem(HOBJECT hHandle, BYTE byCount)
{
	m_dwBlockTime = GetTickCount() + TRADE_BLOCK_AFTER_UPDATE_TICK; //block trading

	MAPITEMDATA::iterator it = p_ItemData.find(hHandle);
	if (p_ItemData.end() == it)
		return;

	CItem* item = it->second;
	item->SetStackCount(byCount);
}

void CTrade::Trade()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CPlayer* target = GetCompany()->GetOwner();

	CNtlPacket pQry(sizeof(sGQ_TRADE_REQ));
	sGQ_TRADE_REQ * rQry = (sGQ_TRADE_REQ *)pQry.GetPacketData();
	rQry->wOpCode = GQ_TRADE_REQ;
	rQry->handle = m_pOwner->GetID();
	rQry->hTarget = target->GetID();
	rQry->charID = m_pOwner->GetCharID();
	rQry->dstcharID = target->GetCharID();
	rQry->byGiveCount = 0;
	rQry->byTakeCount = 0;

	for (MAPITEMDATA::iterator it = p_ItemData.begin(); it != p_ItemData.end(); )
	{
		CItem* item = it->second;
		if (item)
		{
			CItem* realitem = g_pItemManager->Find(it->first);
			if (realitem && item->GetID() == realitem->GetID())
			{
				if (item->TradeItem(target)) // add item to new owner
				{
					rQry->asTakeData[rQry->byTakeCount].itemSerial = realitem->GetItemID();
					rQry->asTakeData[rQry->byTakeCount].byDstCount = UnsignedSafeDecrease<BYTE>(realitem->GetCount(), item->GetCount());

					// remove/update item at owner
					realitem->SetTrading(false);
					realitem->SetCount(rQry->asTakeData[rQry->byTakeCount].byDstCount, false, false);  // update count


					rQry->asGiveData[rQry->byGiveCount].byDstPlace = item->GetPlace();
					rQry->asGiveData[rQry->byGiveCount].byDstPos = item->GetPos();
					rQry->asGiveData[rQry->byGiveCount].byDstCount = item->GetCount();
					rQry->asGiveData[rQry->byGiveCount].hItem = item->GetID();
					rQry->asGiveData[rQry->byGiveCount].itemSerial = rQry->asTakeData[rQry->byTakeCount].itemSerial;
					rQry->asGiveData[rQry->byGiveCount].itemNewSerial = INVALID_ITEMID;
					rQry->asGiveData[rQry->byGiveCount].itemTblidx = item->GetTblidx();

					rQry->byGiveCount++;
					rQry->byTakeCount++;
				}

				it = p_ItemData.erase(it);

				if (rQry->byGiveCount == TRADE_INVEN_MAX_COUNT)
					break;

				continue;
			}
			else
			{
				ERR_LOG(LOG_USER, "<TRADE>Player: %u Could not find real item.. ItemID: %I64u", m_pOwner->GetCharID(), item->GetItemID());
			}
		}

		++it;
	}

	if (m_dwZeni > 0)
	{
		GetOwner()->UpdateZeni(ZENNY_CHANGE_TYPE_TRADE, m_dwZeni, false, false);

		if(target->CanReceiveZeni(m_dwZeni))
			target->UpdateZeni(ZENNY_CHANGE_TYPE_TRADE, m_dwZeni, true, false);

		ERR_LOG(LOG_USER, "ZENI-TRADE: Owner: %u, Zeni %u, Target %u", GetOwner()->GetCharID(), m_dwZeni, target->GetCharID());
	}

	rQry->dwGiveZenny = m_dwZeni;
	rQry->dwTakeZenny = m_dwZeni;
	pQry.SetPacketLen(sizeof(sGQ_TRADE_REQ));
	app->SendTo(app->GetQueryServerSession(), &pQry);
}