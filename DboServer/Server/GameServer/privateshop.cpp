#include "stdafx.h"
#include "privateshop.h"
#include "CPlayer.h"
#include "item.h"
#include "ItemManager.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlResultCode.h"
#include "GameServer.h"


CShopManager::CShopManager()
{
}

CShopManager::~CShopManager()
{
}

CPrivateShop* CShopManager::CreatePrivateShop(CPlayer* ch)
{
	if (GetPrivateShop(ch->GetID()))
		return NULL;

	CPrivateShop* pkShop = new CPrivateShop;
	pkShop->SetOwner(ch);

	m_map_pkShop.insert(std::make_pair(ch->GetID(), pkShop));
	return pkShop;
}

CPrivateShop* CShopManager::GetPrivateShop(HOBJECT hOwner)
{
	TPrivateShopMap::iterator it = m_map_pkShop.find(hOwner);

	if (it == m_map_pkShop.end())
		return NULL;

	return it->second;
}

bool CShopManager::DestroyPrivateShop(CPlayer* ch)
{
	CPrivateShop* pkShop = GetPrivateShop(ch->GetID());

	if (!pkShop)
	{
		ERR_LOG(LOG_USER, "Player %u destroy private shop. pkShop == NULL", ch->GetCharID());
		return false;
	}

	m_map_pkShop.erase(ch->GetID());
	delete pkShop;

	return true;
}






/* ------------------------------------------------------------------------------------ */
CPrivateShop::CPrivateShop()
{
	shop_data.sSummaryPrivateShopData.bIsOwnerEmpty = true;
	shop_data.sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_NULL;
}

CPrivateShop::~CPrivateShop()
{
	for (GuestSetType::iterator it = m_set_guest.begin(); it != m_set_guest.end(); it++)
	{
		CPlayer* ch = *it;
		if (ch && ch->IsInitialized())
		{
			CNtlPacket packet(sizeof(sGU_PRIVATESHOP_LEAVE_NFY));
			sGU_PRIVATESHOP_LEAVE_NFY* res = (sGU_PRIVATESHOP_LEAVE_NFY*)packet.GetPacketData();
			res->wOpCode = GU_PRIVATESHOP_LEAVE_NFY;
			packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_LEAVE_NFY));
			ch->SendPacket(&packet);

			ch->SetVisitPrivateShop(NULL);
		}
	}
	m_set_guest.clear();

	for (ItemsMapType::iterator it = m_map_items.begin(); it != m_map_items.end(); it++)
	{
		sPRIVATESHOP_ITEM_DATA* shopitem = it->second;
		if (shopitem)
		{
			CItem* item = m_pkPC->GetPlayerItemContainer()->GetItem(shopitem->sItem.byPlace, shopitem->sItem.byPosition);
			if (item)
				item->SetTrading(false);
			else if (GetOwner())
				ERR_LOG(LOG_USER, "ERROR: USER %u COULD NOT FIND ITEM IN INVENTORY WHICH IS INSIDE PRIVATE SHOP!!", GetOwner()->GetCharID());
		}
		delete shopitem;
	}

	m_map_items.clear();
}

void CPrivateShop::AddItem(BYTE invPlace, BYTE invPos, BYTE shopPos)
{
	WORD resultcode = GAME_SUCCESS;

	CNtlPacket packet(sizeof(sGU_PRIVATESHOP_ITEM_INSERT_RES));
	sGU_PRIVATESHOP_ITEM_INSERT_RES* res = (sGU_PRIVATESHOP_ITEM_INSERT_RES*)packet.GetPacketData();
	res->wOpCode = GU_PRIVATESHOP_ITEM_INSERT_RES;

	if (IsShopEditAble())
	{
		if (IsInvenContainer(invPlace))
		{
			CItem* item = m_pkPC->GetPlayerItemContainer()->GetItem(invPlace, invPos);
			if (item)
			{
				if (item->CanPrivateshop())//check if item can be put into private shop
				{
					if (item->GetCount() > 0 && item->GetCount() <= item->GetTbldat()->byMax_Stack)
					{
						if (!GetItem(shopPos))//check if slot already used
						{
							bool bFound = false;
							/*Check if item already exist*/
							for (ItemsMapType::iterator it = m_map_items.begin(); it != m_map_items.end(); it++)
							{
								sPRIVATESHOP_ITEM_DATA* pItemData = it->second;

								if (item->GetItemID() == pItemData->sItem.itemId || item->GetID() == pItemData->hItem)
								{
									bFound = true;
									break;
								}
							}

							if (bFound == false)
							{
								item->SetTrading(true);

								res->sPrivateShopItemData.bIsSaveItem = false;
								res->sPrivateShopItemData.byPrivateShopInventorySlotPos = shopPos;
								res->sPrivateShopItemData.byItemState = sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL;
								res->sPrivateShopItemData.dwZenny = NTL_PRIVATESHOP_ITEM_BUY_ZENNY_DEFAULT;
								res->sPrivateShopItemData.hItem = item->GetID();
								memcpy(&res->sPrivateShopItemData.sItem, &item->GetItemData(), sizeof(sITEM_DATA));

								sPRIVATESHOP_ITEM_DATA* psitemdata = new sPRIVATESHOP_ITEM_DATA;
								memcpy(psitemdata, &res->sPrivateShopItemData, sizeof(sPRIVATESHOP_ITEM_DATA));
								m_map_items.insert(std::make_pair(shopPos, psitemdata));
							}
							else
							{
								ERR_LOG(LOG_USER, "Player: %u tried to put the same item multiple times into the private shop", m_pkPC->GetCharID());
							}
						}
						else resultcode = GAME_PRIVATESHOP_PRIVATESHOP_INVENTORY_SLOT_ALREADY_ITEM;
					}
					else resultcode = GAME_ITEM_STACK_FAIL;
				}
				else resultcode = GAME_PRIVATESHOP_ITEM_NOTVALID;
			}
			else resultcode = GAME_PRIVATESHOP_ITEM_NULL;
		}
		else resultcode = GAME_FAIL;
	}
	else resultcode = GAME_PRIVATESHOP_NOTAUTH;

	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_INSERT_RES));
	m_pkPC->SendPacket(&packet);
}

void CPrivateShop::DelItem(BYTE bySlot)
{
	WORD resultcode = GAME_SUCCESS;

	CNtlPacket packet(sizeof(sGU_PRIVATESHOP_ITEM_DELETE_RES));
	sGU_PRIVATESHOP_ITEM_DELETE_RES* res = (sGU_PRIVATESHOP_ITEM_DELETE_RES*)packet.GetPacketData();
	res->wOpCode = GU_PRIVATESHOP_ITEM_DELETE_RES;

	if (IsShopEditAble())
	{
		sPRIVATESHOP_ITEM_DATA* shopitem = GetItem(bySlot);
		if (shopitem)
		{
			CItem* item = m_pkPC->GetPlayerItemContainer()->GetItem(shopitem->sItem.byPlace, shopitem->sItem.byPosition);
			if (item)
			{
				item->SetTrading(false);
			}
			else
			{
				ERR_LOG(LOG_HACK, "USER %u TRIED TO DELETE ITEM FROM PRIVATE SHOP WHICH DOES NOT EXIST ANYMORE!", GetOwner()->GetCharID());
			}

			memcpy(&res->sPrivateShopItemData, shopitem, sizeof(sPRIVATESHOP_ITEM_DATA));

			delete shopitem;
			m_map_items.erase(bySlot);
		}
		else resultcode = GAME_PRIVATESHOP_ITEM_NOTVALID;
	}
	else resultcode = GAME_PRIVATESHOP_NOTAUTH;

	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_DELETE_RES));
	m_pkPC->SendPacket(&packet);
}

sPRIVATESHOP_ITEM_DATA* CPrivateShop::GetItem(BYTE bySlot)
{
	ItemsMapType::iterator it = m_map_items.find(bySlot);

	if (m_map_items.end() == it)
		return NULL;

	return it->second;
}

void CPrivateShop::UpdatePrice(BYTE bySlot, DWORD dwNewPrice)
{
	WORD resultcode = GAME_SUCCESS;

	CNtlPacket packet(sizeof(sGU_PRIVATESHOP_ITEM_UPDATE_RES));
	sGU_PRIVATESHOP_ITEM_UPDATE_RES* res = (sGU_PRIVATESHOP_ITEM_UPDATE_RES*)packet.GetPacketData();
	res->wOpCode = GU_PRIVATESHOP_ITEM_UPDATE_RES;

	if (IsShopEditAble())
	{
		if (dwNewPrice <= NTL_MAX_USE_ZENI && dwNewPrice > 0)
		{
			sPRIVATESHOP_ITEM_DATA* shopitem = GetItem(bySlot);
			if (shopitem)
			{
				res->byPrivateShopInventorySlotPos = bySlot;
				res->dwZenny = dwNewPrice;

				shopitem->dwZenny = dwNewPrice;
			}
			else resultcode = GAME_PRIVATESHOP_ITEM_NOTVALID;
		}
		else resultcode = GAME_FAIL;
	}
	else resultcode = GAME_PRIVATESHOP_NOTAUTH;

	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_UPDATE_RES));
	m_pkPC->SendPacket(&packet);
}

void CPrivateShop::OpenShop(bool bIsOwnerEmpty, BYTE byNoticeSize, WCHAR* wcNotice, WCHAR* wcPrivateShopName)
{
	if (wcslen(wcNotice) >= NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE || wcslen(wcPrivateShopName) > NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE)
	{
		CNtlPacket packet(sizeof(sGU_PRIVATESHOP_OPEN_RES));
		sGU_PRIVATESHOP_OPEN_RES* res = (sGU_PRIVATESHOP_OPEN_RES*)packet.GetPacketData();
		res->wOpCode = GU_PRIVATESHOP_OPEN_RES;
		res->wResultCode = GAME_FAIL;
		packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_OPEN_RES));
		m_pkPC->SendPacket(&packet);

		ERR_LOG(LOG_GENERAL, "ERROR PRIVATE-SHOP NOTICE OR NAME TOO LONG. Notice-size %u, name-size %u, Player %u", wcslen(wcNotice), wcslen(wcPrivateShopName), m_pkPC->GetCharID());

		return;
	}

	WORD wResultCode = GAME_SUCCESS;

	if (IsShopEditAble() == false)
		wResultCode = GAME_PRIVATESHOP_NOTAUTH;

	CNtlPacket packet(sizeof(sGU_PRIVATESHOP_OPEN_RES));
	sGU_PRIVATESHOP_OPEN_RES* res = (sGU_PRIVATESHOP_OPEN_RES*)packet.GetPacketData();
	res->wOpCode = GU_PRIVATESHOP_OPEN_RES;
	res->wResultCode = wResultCode;
	res->byNoticeSize = byNoticeSize;
	wcscpy_s(res->wcNotice, NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1, wcNotice);
	res->sSummaryPrivateShopData.bIsOwnerEmpty = bIsOwnerEmpty;
	res->sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_OPEN;
	wcscpy_s(res->sSummaryPrivateShopData.wcPrivateShopName, NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1, wcPrivateShopName);
	packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_OPEN_RES));
	m_pkPC->SendPacket(&packet);

	if (wResultCode == GAME_SUCCESS)
	{
		CNtlPacket packet2(sizeof(sGU_PRIVATESHOP_OPEN_NFY));
		sGU_PRIVATESHOP_OPEN_NFY* res2 = (sGU_PRIVATESHOP_OPEN_NFY*)packet2.GetPacketData();
		res2->wOpCode = GU_PRIVATESHOP_OPEN_NFY;
		res2->hOwner = m_pkPC->GetID();
		res2->sSummaryPrivateShopData.bIsOwnerEmpty = bIsOwnerEmpty;
		res2->sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_OPEN;
		wcscpy_s(res2->sSummaryPrivateShopData.wcPrivateShopName, NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1, wcPrivateShopName);
		packet2.SetPacketLen(sizeof(sGU_PRIVATESHOP_OPEN_NFY));
		m_pkPC->Broadcast(&packet2);

		shop_data.byNoticeSize = byNoticeSize;
		wcscpy_s(shop_data.wcNotice, NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1, wcNotice);
		shop_data.sSummaryPrivateShopData.bIsOwnerEmpty = bIsOwnerEmpty;
		shop_data.sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_OPEN;
		wcscpy_s(shop_data.sSummaryPrivateShopData.wcPrivateShopName, NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1, wcPrivateShopName);
		shop_data.hOwner = m_pkPC->GetID();

		m_pkPC->SendCharStatePrivateShop(bIsOwnerEmpty, PRIVATESHOP_STATE_OPEN, wcPrivateShopName);
	}
}


void CPrivateShop::CloseShop()
{
	shop_data.sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_CLOSE;

	//remove all viewers
	for (GuestSetType::iterator it = m_set_guest.begin(); it != m_set_guest.end(); it++)
	{
		CPlayer* ch = *it;
		if (ch)
		{
			CNtlPacket packet(sizeof(sGU_PRIVATESHOP_LEAVE_NFY));
			sGU_PRIVATESHOP_LEAVE_NFY* res = (sGU_PRIVATESHOP_LEAVE_NFY*)packet.GetPacketData();
			res->wOpCode = GU_PRIVATESHOP_LEAVE_NFY;
			packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_LEAVE_NFY));
			ch->SendPacket(&packet);

			ch->SetVisitPrivateShop(NULL);
		}
	}
	m_set_guest.clear();

	//unset selected items
	for (SelectedItemsMapType::iterator it2 = m_map_selectedItems.begin(); it2 != m_map_selectedItems.end(); it2++)
	{
		//reset item state
		sPRIVATESHOP_ITEM_DATA* item = it2->first;
		item->byItemState = sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL;

		//send to all that item has been unselected
		CNtlPacket packet2(sizeof(sGU_PRIVATESHOP_ITEM_SELECT_NFY));
		sGU_PRIVATESHOP_ITEM_SELECT_NFY* res2 = (sGU_PRIVATESHOP_ITEM_SELECT_NFY*)packet2.GetPacketData();
		res2->wOpCode = GU_PRIVATESHOP_ITEM_SELECT_NFY;
		res2->byPrivateShopInventorySlotPos = item->byPrivateShopInventorySlotPos;
		res2->byItemState = sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL;
		packet2.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_SELECT_NFY));
		m_pkPC->SendPacket(&packet2);
	}
	m_map_selectedItems.clear();
}

bool CPrivateShop::IsShopEditAble()
{
	if (shop_data.sSummaryPrivateShopData.byShopState == PRIVATESHOP_STATE_CLOSE
		|| shop_data.sSummaryPrivateShopData.byShopState == PRIVATESHOP_STATE_NONE
		|| shop_data.sSummaryPrivateShopData.byShopState == PRIVATESHOP_STATE_NULL)
		return true;

	return false;
}


void CPrivateShop::BroadcastGuests(CNtlPacket* pPacket, CPlayer* pException/* = NULL*/)
{
	for (GuestSetType::iterator it = m_set_guest.begin(); it != m_set_guest.end(); it++)
	{
		CPlayer* ch = *it;
		if (ch && ch->IsInitialized())
		{
			if (ch == pException)
				continue;

			ch->SendPacket(pPacket);
		}
	}
}


void CPrivateShop::EnterShop(CPlayer* visitor)
{
	WORD resultcode = GAME_SUCCESS;

	//Only allow open shop while standing
	if (m_pkPC->GetCharStateID() != CHARSTATE_PRIVATESHOP || m_pkPC->GetCombatMode())
	{
		m_pkPC->GetClientSession()->SendResultcode(GAME_CHAR_IS_WRONG_STATE);
		resultcode = GAME_FAIL;
	}
	else if (IsInBattleArena(m_pkPC->GetWorldTblidx(), m_pkPC->GetCurLoc()) || m_pkPC->IsPvpZone())
	{
		m_pkPC->GetClientSession()->SendResultcode(GAME_PRIVATESHOP_PRIVATESHOP_NOT_CREATE_PLACE);
		resultcode = GAME_FAIL;
	}
	else if (visitor->GetVisitPrivateShop()) //check if visitor already watch a private shop
	{
		resultcode = GAME_PRIVATESHOP_VISITOR_ALREADYENTER;
	}
	else if (shop_data.sSummaryPrivateShopData.byShopState != PRIVATESHOP_STATE_OPEN)
	{
		resultcode = GAME_PRIVATESHOP_NOTRUNSTATE;
	}

	if (resultcode == GAME_SUCCESS)
	{
		m_set_guest.insert(visitor);
		visitor->SetVisitPrivateShop(this);

		////Show items
		for (ItemsMapType::iterator it = m_map_items.begin(); it != m_map_items.end(); it++)
		{
			sPRIVATESHOP_ITEM_DATA* shopitem = it->second;
			if (shopitem)
			{
				CNtlPacket packet2(sizeof(sGU_PRIVATESHOP_ITEM_DATA_INFO));
				sGU_PRIVATESHOP_ITEM_DATA_INFO* res2 = (sGU_PRIVATESHOP_ITEM_DATA_INFO*)packet2.GetPacketData();
				res2->wOpCode = GU_PRIVATESHOP_ITEM_DATA_INFO;
				res2->hOwnerID = m_pkPC->GetID();
				res2->wResultCode = GAME_SUCCESS;
				memcpy(&res2->sPrivateShopItemData, shopitem, sizeof(sPRIVATESHOP_ITEM_DATA));
				packet2.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_DATA_INFO));
				visitor->SendPacket(&packet2);
			}
		}
	}

	CNtlPacket packet(sizeof(sGU_PRIVATESHOP_ENTER_RES));
	sGU_PRIVATESHOP_ENTER_RES* res = (sGU_PRIVATESHOP_ENTER_RES*)packet.GetPacketData();
	res->wOpCode = GU_PRIVATESHOP_ENTER_RES;
	res->wResultCode = resultcode;
	memcpy(&res->sPrivateShopData, &shop_data, sizeof(sPACKET_PRIVATESHOP_SHOP_DATA));
	packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_ENTER_RES));
	visitor->SendPacket(&packet);
}

void CPrivateShop::LeaveShop(CPlayer* visitor, bool bIsLogout)
{
	if (bIsLogout == false)
	{
		CNtlPacket packet(sizeof(sGU_PRIVATESHOP_LEAVE_RES));
		sGU_PRIVATESHOP_LEAVE_RES* res = (sGU_PRIVATESHOP_LEAVE_RES*)packet.GetPacketData();
		res->wOpCode = GU_PRIVATESHOP_LEAVE_RES;
		res->wResultCode = GAME_SUCCESS;
		packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_LEAVE_RES));
		visitor->SendPacket(&packet);

		visitor->SetVisitPrivateShop(NULL);
	}

	m_set_guest.erase(visitor);

	//unset selected items
	for (SelectedItemsMapType::iterator it = m_map_selectedItems.begin(); it != m_map_selectedItems.end(); )
	{
		if (it->second == visitor->GetID())
		{
			//reset item state
			sPRIVATESHOP_ITEM_DATA* item = it->first;
			item->byItemState = sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL;

			//send to all that item has been unselected
			CNtlPacket packet2(sizeof(sGU_PRIVATESHOP_ITEM_SELECT_NFY));
			sGU_PRIVATESHOP_ITEM_SELECT_NFY* res2 = (sGU_PRIVATESHOP_ITEM_SELECT_NFY*)packet2.GetPacketData();
			res2->wOpCode = GU_PRIVATESHOP_ITEM_SELECT_NFY;
			res2->byPrivateShopInventorySlotPos = item->byPrivateShopInventorySlotPos;
			res2->byItemState = sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL;
			packet2.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_SELECT_NFY));

			//send to shop owner
			m_pkPC->SendPacket(&packet2);
			//send to all viewers
			BroadcastGuests(&packet2);

			it = m_map_selectedItems.erase(it);
			continue;
		}

		++it;
	}
}

void CPrivateShop::SelectItem(BYTE byPos, BYTE byNewItemState, CPlayer* visitor)
{
	WORD resultcode = GAME_SUCCESS;

	if (shop_data.sSummaryPrivateShopData.byShopState != PRIVATESHOP_STATE_OPEN)
	{
		resultcode = GAME_PRIVATESHOP_NOTRUNSTATE;
	}

	sPRIVATESHOP_ITEM_DATA* item = NULL;

	CNtlPacket packet(sizeof(sGU_PRIVATESHOP_ITEM_SELECT_RES));
	sGU_PRIVATESHOP_ITEM_SELECT_RES* res = (sGU_PRIVATESHOP_ITEM_SELECT_RES*)packet.GetPacketData();
	res->wOpCode = GU_PRIVATESHOP_ITEM_SELECT_RES;

	if (resultcode == GAME_SUCCESS)
	{
		res->byPrivateShopInventorySlotPos = byPos;

		item = GetItem(byPos);
		if (item)
		{
			{
				switch (byNewItemState)
				{
				case sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL:
				{
					if (item->byItemState == sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_VISITOR_SELECT_ITEM)
						resultcode = GAME_PRIVATESHOP_VISITOR_SELECTITEM_ALREADY;
					else if (item->byItemState != sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL)
						resultcode = GAME_FAIL;
					else
					{
						res->byItemState = byNewItemState;
						item->byItemState = byNewItemState;
					}
				}
				break;
				case sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_VISITOR_SELECT_ITEM:
				{
					if (item->dwZenny > visitor->GetZeni())
						resultcode = GAME_ZENNY_NOT_ENOUGH;
					else if (item->byItemState != sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL)
						resultcode = GAME_PRIVATESHOP_VISITOR_SELECTITEM_NULL;
					else
					{
						res->byItemState = byNewItemState;
						item->byItemState = byNewItemState;
					}
				}
				break;
				case sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_VISITOR_UNSELECT_ITEM:
				{
					if (item->byItemState != sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_VISITOR_SELECT_ITEM)
						resultcode = GAME_PRIVATESHOP_VISITOR_SELECTITEM_NULL;
					else
					{
						// check if the real owner unselect the item
						bool bFound = false;
						for (SelectedItemsMapType::iterator it = m_map_selectedItems.begin();
							it != m_map_selectedItems.end(); it++)
						{
							if (it->first->hItem == item->hItem)
							{
								bFound = true;
								if (it->second != visitor->GetID())
									resultcode = GAME_FAIL;
								else
									m_map_selectedItems.erase(it);

								break;
							}
						}

						if (bFound == false)
							resultcode = GAME_FAIL;
					}

					if (resultcode == GAME_SUCCESS)
					{
						res->byItemState = sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_VISITOR_UNSELECT_ITEM;
						item->byItemState = sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_GENERAL;
					}
				}
				break;

				default: resultcode = GAME_FAIL; break;
				}
			}

		}
		else resultcode = GAME_PRIVATESHOP_VISITOR_SELECTITEM_NULL;
	}

	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_SELECT_RES));
	visitor->SendPacket(&packet);

	if (resultcode == GAME_SUCCESS)
	{
		if (item->byItemState == sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_VISITOR_SELECT_ITEM)
			m_map_selectedItems.insert(SelectedItemsMapType::value_type(item, visitor->GetID()));

		CNtlPacket packet2(sizeof(sGU_PRIVATESHOP_ITEM_SELECT_NFY));
		sGU_PRIVATESHOP_ITEM_SELECT_NFY* res2 = (sGU_PRIVATESHOP_ITEM_SELECT_NFY*)packet2.GetPacketData();
		res2->wOpCode = GU_PRIVATESHOP_ITEM_SELECT_NFY;
		res2->byPrivateShopInventorySlotPos = byPos;
		res2->byItemState = byNewItemState;
		packet2.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_SELECT_NFY));
		m_pkPC->SendPacket(&packet2);

		BroadcastGuests(&packet2, visitor); //send to all viewers
	}
}

void CPrivateShop::BuyItem(BYTE* byPos, CPlayer* visitor)
{
	WORD resultcode = GAME_SUCCESS;
	QWORD qwTotalPrice = 0; //must be bigger than DWORD otherwise if people select items which have total price over DWORD limit then there will be a bug
	std::set<sPRIVATESHOP_ITEM_DATA*> buyitemset;

	//calculate price and check if visitor has enough zeni
	for (int i = 0; i < NTL_MAX_BUY_SHOPPING_CART; i++)
	{
		if (byPos[i] == INVALID_BYTE)
			continue;

		sPRIVATESHOP_ITEM_DATA* item = GetItem(byPos[i]);
		if (item && item->byItemState == sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_VISITOR_SELECT_ITEM)
		{
			for (SelectedItemsMapType::iterator iter = m_map_selectedItems.begin();
				iter != m_map_selectedItems.end(); iter++)
			{
				if (iter->first->hItem == item->hItem
					&& iter->second == visitor->GetID()
					&& iter->first->byItemState == sPRIVATESHOP_ITEM_DATA::eITEMSTATE::PRIVATESHOP_VISITOR_SELECT_ITEM)
				{
					qwTotalPrice += (QWORD)item->dwZenny;
					buyitemset.insert(item);

					break;
				}
			}
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "Could not find item. pos %u. Buyer id %u", byPos[i], visitor->GetCharID());
			resultcode = GAME_FAIL;
			break;
		}
	}

	BYTE itemcount = (BYTE)buyitemset.size();

	if (shop_data.sSummaryPrivateShopData.byShopState != PRIVATESHOP_STATE_OPEN)
		resultcode = GAME_PRIVATESHOP_NOTRUNSTATE;
	//check if buyer has enough zeni
	else if (qwTotalPrice > (QWORD)visitor->GetZeni())
		resultcode = GAME_PRIVATESHOP_VISITOR_NOT_MONEY;
	//check if seller can receive the zeni
	else if (!m_pkPC->CanReceiveZeni((DWORD)qwTotalPrice))
		resultcode = GAME_PRIVATESHOP_OWNER_MONEY_FULL;

	//check inventory if zeni check didnt fail
	if (resultcode == GAME_SUCCESS)
	{
		if (visitor->GetPlayerItemContainer()->CountEmptyInventory() < itemcount)
			resultcode = GAME_PRIVATESHOP_VISITOR_NOT_INVENTORY_EMPTY;
	}

	CNtlPacket packet(sizeof(sGU_PRIVATESHOP_ITEM_BUYING_RES));
	sGU_PRIVATESHOP_ITEM_BUYING_RES* res = (sGU_PRIVATESHOP_ITEM_BUYING_RES*)packet.GetPacketData();
	res->wOpCode = GU_PRIVATESHOP_ITEM_BUYING_RES;
	res->wResultCode = resultcode;

	if (resultcode == GAME_SUCCESS)
	{
		CNtlPacket packet2(sizeof(sGU_PRIVATESHOP_ITEM_BUYING_NFY));
		sGU_PRIVATESHOP_ITEM_BUYING_NFY* res2 = (sGU_PRIVATESHOP_ITEM_BUYING_NFY*)packet2.GetPacketData();
		res2->wOpCode = GU_PRIVATESHOP_ITEM_BUYING_NFY;
		res2->hPrivateShopOwner = m_pkPC->GetID();

		BYTE x = 0;
		for (std::set< sPRIVATESHOP_ITEM_DATA* >::iterator it = buyitemset.begin(); it != buyitemset.end(); )
		{
			sPRIVATESHOP_ITEM_DATA* item = *it;

			CItem* updateItem = g_pItemManager->SwitchOwner(m_pkPC, visitor, item->hItem);
			if (updateItem)
			{
				/*We need to delete here because item place and pos gets updated in SwitchOwner*/
				//del item from seller
				m_pkPC->GetPlayerItemContainer()->RemoveItem(item->sItem.byPlace, item->hItem);
				//del from seller end

				// do this before we update sItem
				memcpy(&res2->sPrivateShopItemData[x], item, sizeof(sPRIVATESHOP_ITEM_DATA));

				updateItem->SetTrading(false);
				item->sItem = updateItem->GetItemData();
				memcpy(&res->sPrivateShopItemData[x], item, sizeof(sPRIVATESHOP_ITEM_DATA));
				x++;
				it++;
			}
			else
			{
				ERR_LOG(LOG_SYSTEM, "item is null");
				qwTotalPrice = UnsignedSafeDecrease<QWORD>(qwTotalPrice, item->dwZenny); //remove price from non-found item
				//erase from shop
				for (SelectedItemsMapType::iterator iter = m_map_selectedItems.begin();
					iter != m_map_selectedItems.end(); iter++)
				{
					if (iter->first->hItem == item->hItem)
					{
						m_map_selectedItems.erase(iter);
						break;
					}
				}
				m_map_items.erase(item->byPrivateShopInventorySlotPos);
				SAFE_DELETE(item);
				//
				it = buyitemset.erase(it);
			}
		}

		res->byBuyItemCount = res2->byBuyItemCount = x;

		packet2.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_BUYING_NFY));
		m_pkPC->SendPacket(&packet2);
		BroadcastGuests(&packet2, visitor); //send to all viewers
	}

	packet.SetPacketLen(sizeof(sGU_PRIVATESHOP_ITEM_BUYING_RES));
	visitor->SendPacket(&packet);


	if (resultcode == GAME_SUCCESS)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		//Deduct zenny from buyer
		visitor->UpdateZeni(ZENNY_CHANGE_TYPE_PRIVATESHOP_ITEM_BUY, (DWORD)qwTotalPrice, false, false);
		//add zenny to seller
		m_pkPC->UpdateZeni(ZENNY_CHANGE_TYPE_PRIVATESHOP_ITEM_SELL, (DWORD)qwTotalPrice, true, false);

		CNtlPacket pQry(sizeof(sGQ_PRIVATESHOP_ITEM_BUYING_REQ));
		sGQ_PRIVATESHOP_ITEM_BUYING_REQ * qRes = (sGQ_PRIVATESHOP_ITEM_BUYING_REQ *)pQry.GetPacketData();
		qRes->wOpCode = GQ_PRIVATESHOP_ITEM_BUYING_REQ;
		qRes->handle = visitor->GetID();
		qRes->hTarget = m_pkPC->GetID();
		qRes->charID_From = m_pkPC->GetCharID();
		qRes->charID_To = visitor->GetCharID();
		qRes->dwAllZenny = (DWORD)qwTotalPrice;

		//clear bought items
		for (std::set< sPRIVATESHOP_ITEM_DATA* >::iterator it = buyitemset.begin(); it != buyitemset.end(); it++)
		{
			sPRIVATESHOP_ITEM_DATA* item = *it;

			qRes->asEmpty[qRes->byCount].byPlace = item->sItem.byPlace;
			qRes->asEmpty[qRes->byCount].byPos = item->sItem.byPosition;
			qRes->asEmpty[qRes->byCount].itemID = item->sItem.itemId;
			qRes->byCount++;

			ERR_LOG(LOG_GENERAL, "<PRIVATE-SHOP>Player %u bought Item %I64u(idx: %u) from Owner %u for Zeni: %u. Shop ID %u. Shop Pos %u",
				visitor->GetCharID(), item->sItem.itemId, item->sItem.itemNo, GetOwner()->GetCharID(), item->dwZenny, GetOwner()->GetID(), item->byPrivateShopInventorySlotPos);


			for (SelectedItemsMapType::iterator iter = m_map_selectedItems.begin();
				iter != m_map_selectedItems.end(); iter++)
			{
				if (iter->first->hItem == item->hItem)
				{
					m_map_selectedItems.erase(iter);
					break;
				}
			}

			m_map_items.erase(item->byPrivateShopInventorySlotPos);
			SAFE_DELETE(item);
			//del from shop end
		}

		pQry.SetPacketLen(sizeof(sGQ_PRIVATESHOP_ITEM_BUYING_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);
	}
}