#include "stdafx.h"
#include "GameServer.h"
#include "CPlayer.h" //#include "PlayerItemContainer.h"
#include "ItemManager.h"
#include "TableContainerManager.h"
#include "ItemOptionTable.h"
#include "ItemEnchantTable.h"
#include "SystemEffectTable.h"
#include "SetItemTable.h"
#include "calcs.h"
#include "NtlRandom.h"
#include "NtlPacketGQ.h"


CPlayerItemContainer::CPlayerItemContainer()
{
	m_map_CharItems.clear();
	memset(m_arr_ActiveBags, NULL, sizeof(m_arr_ActiveBags));

	for (int i = 0; i < EQUIP_SLOT_TYPE_COUNT; i++)
		UnsetItemBrief(i);

	m_map_BankItems.clear();

	m_bIsUsingGuildBank = false;
	m_dwGuildZeni = 0;
	m_map_GuildBankItems.clear();

	m_dwDurationUpdateTick = 1000;
}


CPlayerItemContainer::~CPlayerItemContainer()
{
	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
	{
		CItem* pItem = it->second;
		if (pItem)
		{
			g_pItemManager->RemoveItem(it->first);
			SAFE_DELETE(pItem);
		}
		else
		{
			ERR_LOG(LOG_USER, "ERROR: inventory item does not exist \n");
		}
	}
	m_map_CharItems.clear();

	for (auto it = m_map_BankItems.begin(); it != m_map_BankItems.end(); it++)
	{
		CItem* pItem = it->second;
		if (pItem)
		{
			g_pItemManager->RemoveItem(it->first);
			SAFE_DELETE(pItem);
		}
		else
		{
			ERR_LOG(LOG_USER, "ERROR: bank item does not exist \n");
		}
	}
	m_map_BankItems.clear();
}


void CPlayerItemContainer::Init(CPlayer * pOwner)
{
	m_pOwner = pOwner;
}


void CPlayerItemContainer::AddItem(CItem * pItem)
{
	sITEM_DATA& rItemData = pItem->GetItemData();

	//add duration item (if it is required)
	if (rItemData.nUseEndTime > 0)
		m_mapItemsWithDuration.insert({ pItem->GetID(), pItem });

	switch (rItemData.byPlace)
	{
		case CONTAINER_TYPE_BAGSLOT:
		{
			if (pItem->IsBag())
			{
				InsertActiveBag(rItemData.byPosition, pItem);
				InsertItem(pItem->GetID(), pItem);
			}
			else
				ERR_LOG(LOG_USER, "ERROR: Item has Place 0 but is not a bag !!!");
		}
		break;

		case CONTAINER_TYPE_BAG1:
		case CONTAINER_TYPE_BAG2:
		case CONTAINER_TYPE_BAG3:
		case CONTAINER_TYPE_BAG4:
		case CONTAINER_TYPE_BAG5:
		{
			InsertItem(pItem->GetID(), pItem);
		}
		break;

		case CONTAINER_TYPE_EQUIP:
		{
			InsertItem(pItem->GetID(), pItem);

			m_sItemBrief[rItemData.byPosition].byBattleAttribute = rItemData.byBattleAttribute;
			m_sItemBrief[rItemData.byPosition].byGrade = rItemData.byGrade;
			m_sItemBrief[rItemData.byPosition].byRank = rItemData.byRank;
			m_sItemBrief[rItemData.byPosition].tblidx = rItemData.itemNo;

			pItem->SetEquipped(true);
		}
		break;

		case CONTAINER_TYPE_BANKSLOT:
		case CONTAINER_TYPE_BANK1:
		case CONTAINER_TYPE_BANK2:
		case CONTAINER_TYPE_BANK3:
		case CONTAINER_TYPE_BANK4:
		{
			InsertBankItem(pItem->GetID(), pItem);
		}
		break;

		case CONTAINER_TYPE_GUILD1:
		case CONTAINER_TYPE_GUILD2:
		case CONTAINER_TYPE_GUILD3:
		{
			InsertGuildBankItem(pItem->GetID(), pItem);
		}
		break;

		default: ERR_LOG(LOG_USER, "ERROR: Invalid Place. Place = %u, ItemID = %I64u", rItemData.byPlace, pItem->GetItemID()); break;
	}
}

void CPlayerItemContainer::MoveItem(CItem * pItem, BYTE byCurPlace, BYTE byNewPlace, BYTE byOldPos, BYTE byNewPos)
{
	//check if move item inventory <-> inventory
	if (
		(IsInvenContainer(byCurPlace) || IsBagContainer(byCurPlace) || IsEquipContainer(byCurPlace))
		&& (IsInvenContainer(byNewPlace) || IsBagContainer(byNewPlace) || IsEquipContainer(byNewPlace))
	)
	{
		return;
	}

	//check if move item bank <-> bank
	else if (IsBankContainer(byCurPlace) && IsBankContainer(byNewPlace))
	{
		return;
	}

	//check if move item guild bank <-> guild bank
	else if (IsGuildContainer(byCurPlace) && IsGuildContainer(byNewPlace))
	{
		return;
	}

	//remove from old place
	if (IsInvenContainer(byCurPlace) || IsBagContainer(byCurPlace) || IsEquipContainer(byCurPlace))
	{
		m_map_CharItems.erase(pItem->GetID());
	}

	else if (IsBankContainer(byCurPlace))
	{
		m_map_BankItems.erase(pItem->GetID());
	}

	else if (IsGuildContainer(byCurPlace))
	{
		m_map_GuildBankItems.erase(pItem->GetID());
	}

	//add to new place
	if (IsInvenContainer(byNewPlace) || IsBagContainer(byNewPlace) || IsEquipContainer(byNewPlace))
	{
		m_map_CharItems.insert({ pItem->GetID() , pItem });
	}

	else if (IsBankContainer(byNewPlace))
	{
		m_map_BankItems.insert({ pItem->GetID() , pItem });
	}

	else if (IsGuildContainer(byNewPlace))
	{
		m_map_GuildBankItems.insert({ pItem->GetID() , pItem });
	}
}


CItem * CPlayerItemContainer::GetItem(BYTE byPlace, BYTE byPos)
{
	if (IsBagContainer(byPlace) || IsInvenContainer(byPlace) || IsEquipContainer(byPlace)) //check if place is from bag/inventory. Search char items
	{
		for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
		{
			CItem* itemdata = it->second;
			if (itemdata->GetPlace() == byPlace && itemdata->GetPos() == byPos)
				return itemdata;
		}

		return NULL;
	}

	else if (IsBankContainer(byPlace))	//check if place is from bank. Search bank items
	{
		for (auto it = m_map_BankItems.begin(); it != m_map_BankItems.end(); it++)
		{
			CItem* itemdata = it->second;
			if (itemdata->GetPlace() == byPlace && itemdata->GetPos() == byPos)
				return itemdata;
		}

		return NULL;
	}

	else if (IsGuildContainer(byPlace))
	{
		for (auto it = m_map_GuildBankItems.begin(); it != m_map_GuildBankItems.end(); it++)
		{
			CItem* itemdata = it->second;
			if (itemdata->GetPlace() == byPlace && itemdata->GetPos() == byPos)
				return itemdata;
		}

		return NULL;
	}

	return NULL;
}

void CPlayerItemContainer::RemoveItem(BYTE byPlace, HOBJECT hHandle)
{
	m_mapItemsWithDuration.erase(hHandle); //remove from duration

	if (IsBagContainer(byPlace) || IsInvenContainer(byPlace) || IsEquipContainer(byPlace)) //check if place is from bag/inventory. Search char items
	{
		m_map_CharItems.erase(hHandle);
		return;
	}

	else if (IsBankContainer(byPlace))	//check if place is from bank. Search bank items
	{
		m_map_BankItems.erase(hHandle);
		return;
	}

	else if (IsGuildContainer(byPlace))
	{
		m_map_GuildBankItems.erase(hHandle);
		return;
	}

	ERR_LOG(LOG_USER, "ERROR: Could not remove item. User %u, place %u, handle %u \n", byPlace, hHandle);
}

void CPlayerItemContainer::InsertItem(HOBJECT hHandle, CItem * item)
{
	m_map_CharItems.insert(std::make_pair(hHandle, item));
}


CItem * CPlayerItemContainer::GetItem(HOBJECT hHandle)
{
	auto it = m_map_CharItems.find(hHandle);

	if (m_map_CharItems.end() == it)
	{
		return NULL;
	}

	return it->second;
}

CItem * CPlayerItemContainer::GetItemByIdx(TBLIDX itemTblidx)
{
	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
	{
		CItem* itemdata = it->second;
		if (itemdata->GetTblidx() == itemTblidx)
			return itemdata;
	}

	return NULL;
}

CItem * CPlayerItemContainer::GetItem(ITEMID itemId)
{
	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
	{
		CItem* itemdata = it->second;
		if (itemdata->GetItemID() == itemId)
			return itemdata;
	}

	return NULL;
}

CItem * CPlayerItemContainer::CheckStackItem(TBLIDX itemidx, BYTE byCount, BYTE byMaxStack, BYTE byRestrictState)
{
	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
	{
		CItem* itemdata = it->second;

		if (itemdata->IsLocked() == false && itemdata->NeedToIdentify() == false && itemdata->GetRestrictState() == byRestrictState && itemdata->GetTblidx() == itemidx && itemdata->GetCount() + byCount <= byMaxStack)
			return itemdata;
	}

	return NULL;
}

void CPlayerItemContainer::DeleteAllItems()
{
	int nInfiniteLoop = 0;

	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); )
	{
		++nInfiniteLoop;
		if (nInfiniteLoop > 1000)
		{
			ERR_LOG(LOG_SYSTEM, "INFINITE LOOP DETECTED");
		}

		CItem* item = it->second;
		if (item->GetPlace() == CONTAINER_TYPE_BAG1 || item->GetPlace() == CONTAINER_TYPE_BAG2 || item->GetPlace() == CONTAINER_TYPE_BAG3 || item->GetPlace() == CONTAINER_TYPE_BAG4 || item->GetPlace() == CONTAINER_TYPE_BAG5)
		{
			item->SetCount(0, false, true);
			it = m_map_CharItems.begin();
		}
		else ++it;
	}
}


std::pair<BYTE, BYTE> CPlayerItemContainer::GetEmptyInventory()
{
	for (int byBagPos = 0; byBagPos < NTL_MAX_BAGSLOT_COUNT; byBagPos++) //loop bags
	{
		CItem* pBag = m_arr_ActiveBags[byBagPos];

		if (pBag && pBag->IsExpired() == false) //check if bag exist
		{
			if (pBag->GetPlace() == 0 && pBag->GetPos() == byBagPos)
			{
				for (BYTE bypos = 0; bypos < pBag->GetBagSize(); bypos++) //loop pos inside bag
				{
					CItem* bag_check = GetItem(byBagPos + 1, bypos); //check if item exist on that pos inside bag
					if (bag_check == NULL && IsInventoryReserved(byBagPos + 1, bypos) == false)
					{
						return std::make_pair(byBagPos + 1, bypos); //byBagPos + 1 == free inventory place
					}
				}
			}
			else
			{
				ERR_LOG(LOG_USER, "ERROR: Player %u has bag in array but place %u/pos %u dont match. byBagPos = %u. pos should be same as byBagPos!!", m_pOwner->GetCharID(), pBag->GetPlace(), pBag->GetPos(), byBagPos);
			}
		}
	}

	return std::make_pair(INVALID_BYTE, INVALID_BYTE);
}


BYTE CPlayerItemContainer::CountBags()
{
	BYTE bags = 0;
	for (BYTE byBagPos = 0; byBagPos < NTL_MAX_BAGSLOT_COUNT; byBagPos++)
	{
		CItem* pBag = m_arr_ActiveBags[byBagPos];
		if (pBag && pBag->IsExpired() == false)
		{
			if (pBag->GetPlace() == 0 && pBag->GetPos() == byBagPos)
			{
				++bags;
			}
			else
			{
				ERR_LOG(LOG_USER, "ERROR: Player %u has bag in array but place %u/pos %u dont match. byBagPos = %u. pos should be same as byBagPos!!", m_pOwner->GetCharID(), pBag->GetPlace(), pBag->GetPos(), byBagPos);
			}
		}
	}

	return bags;
}


BYTE CPlayerItemContainer::CountEmptyInventory()
{
	BYTE count = 0;

	for (BYTE byBagPos = 0; byBagPos < NTL_MAX_BAGSLOT_COUNT; byBagPos++) //loop bags
	{
		CItem* pBag = m_arr_ActiveBags[byBagPos];

		if (pBag && pBag->IsExpired() == false) //check if bag exist
		{
			if (pBag->GetPlace() == 0 && pBag->GetPos() == byBagPos)
			{
				for (BYTE bypos = 0; bypos < pBag->GetBagSize(); bypos++) //loop through the bag to find free pos
				{
					if (GetItem(byBagPos + 1, bypos) == NULL && IsInventoryReserved(byBagPos + 1, bypos) == false) //check if item exist on that pos
						++count;
				}
			}
			else
			{
				ERR_LOG(LOG_USER, "ERROR: Player %u has bag in array but place %u/pos %u dont match. byBagPos = %u. pos should be same as byBagPos!!", m_pOwner->GetCharID(), pBag->GetPlace(), pBag->GetPos(), byBagPos);
			}
		}
	}

	return count;
}


bool CPlayerItemContainer::IsBagEmpty(BYTE byBagPos)
{
	if(byBagPos >= NTL_MAX_BAGSLOT_COUNT)
		return false;

	CItem* pBag = m_arr_ActiveBags[byBagPos]; //get bag
	if (pBag == NULL || pBag->IsExpired() == true) //check if bag exist
	{
		false; //bag dont exist, return false.
	}

	if (pBag->GetPlace() == 0 && pBag->GetPos() == byBagPos)
	{
		for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
		{
			CItem* pItem = it->second;
			if (pItem->GetPlace() == byBagPos + 1)
			{
				return false;
			}
		}
	}
	else
	{
		ERR_LOG(LOG_USER, "ERROR: Player %u has bag in array but place %u/pos %u dont match. byBagPos = %u. pos should be same as byBagPos!!", m_pOwner->GetCharID(), pBag->GetPlace(), pBag->GetPos(), byBagPos);
		return false;
	}

	return true;
}


bool CPlayerItemContainer::HasRequiredItem(TBLIDX itemTblidx, BYTE byCount, int nMaxLoopCount/* = ITEM_BULK_DELETE_COUNT*/)
{
	int nCount = 0;
	int nLoop = 0;
	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
	{
		CItem* item = it->second;

		if (item->IsLocked() == false && item->IsTrading() == false && item->GetTblidx() == itemTblidx) //check if its bag & check if in use
		{
			nCount += (int)item->GetCount();

			if (nCount >= (int)byCount)
				return true;

			if (++nLoop >= nMaxLoopCount)
				break;
		}
	}

	return nCount >= (int)byCount;
}


bool CPlayerItemContainer::RemoveRequiredItem(TBLIDX itemTblidx, BYTE byCount, BYTE& rbyUpdateCount, BYTE& rbyDeleteCount, sITEM_BASIC_DATA* apUpdate, sITEM_DELETE_DATA* apDelete)
{
	BYTE count = byCount;
	auto it = m_map_CharItems.begin();

	int nloopCount = 0;

	while (it != m_map_CharItems.end())
	{
		++nloopCount;

		if (nloopCount > 5000)
		{
			ERR_LOG(LOG_SYSTEM, "INFINITE LOOP FOUND");
		}

		CItem* item = it->second;
		if (item->IsLocked() == false && item->IsTrading() == false && item->GetTblidx() == itemTblidx) //check if its bag & check if in use
		{
			if (item->GetCount() > count)
			{
				item->SetCount(item->GetCount() - count, false, false);

				apUpdate[rbyUpdateCount].byPlace = item->GetPlace();
				apUpdate[rbyUpdateCount].byPos = item->GetPos();
				apUpdate[rbyUpdateCount].byStack = item->GetCount();
				apUpdate[rbyUpdateCount++].nItemID = item->GetItemID();

				count = 0;
				break;
			}
			else
			{
				apDelete[rbyDeleteCount].byPlace = item->GetPlace();
				apDelete[rbyDeleteCount].byPos = item->GetPos();
				apDelete[rbyDeleteCount].hItem = item->GetID();
				apDelete[rbyDeleteCount++].itemId = item->GetItemID();

				count -= item->GetCount();
				item->SetCount(0, false, false);
				it = m_map_CharItems.begin(); //restart loop because one item removed
			}
		}
		else it++;
	}

	return count == 0;
}

TBLIDX CPlayerItemContainer::GetMissingDragonball()
{
	TBLIDX existingBallIdx[NTL_ITEM_MAX_DRAGONBALL] = { 200001,200002,200003,200004,200005,200006,200007 };

	std::vector<TBLIDX> vecRandBallIdx;

	for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
	{
		CItem* pItem = GetItemByIdx(existingBallIdx[i]);
		if(pItem == NULL)
			vecRandBallIdx.push_back(existingBallIdx[i]);
	}

	if (vecRandBallIdx.size() == 0)
		return 200008;

	int nRand = RandomRange(0, (int)vecRandBallIdx.size() - 1);
	return vecRandBallIdx[nRand];
}

bool CPlayerItemContainer::IsInventoryReserved(BYTE byPlace, BYTE byPos)
{
	for (std::vector<std::pair<BYTE, BYTE>>::iterator it = m_vecReservedInventory.begin(); it != m_vecReservedInventory.end(); it++)
	{
		std::pair<BYTE, BYTE> pair = *it;

		if (pair.first == byPlace && pair.second == byPos)
			return true;
	}

	return false;
}

void CPlayerItemContainer::AddReservedInventory(BYTE byPlace, BYTE byPos)
{
	m_vecReservedInventory.push_back(std::make_pair(byPlace, byPos));
}

void CPlayerItemContainer::RemoveReservedInventory(BYTE byPlace, BYTE byPos)
{
	for (std::vector<std::pair<BYTE, BYTE>>::iterator it = m_vecReservedInventory.begin(); it != m_vecReservedInventory.end(); it++)
	{
		std::pair<BYTE, BYTE> pair = *it;

		if (pair.first == byPlace && pair.second == byPos)
		{
			m_vecReservedInventory.erase(it);
			break;
		}
	}
}

#include "NtlPacketGU.h"
//--------------------------------------------------------------------------------------//
//		SORT INVENTORY
//--------------------------------------------------------------------------------------//
void CPlayerItemContainer::SortInventory(BYTE byInventoryType, HOBJECT hNpcHandle)
{
	CNtlPacket packet(sizeof(sGU_INVENTORY_SORT_RES));
	sGU_INVENTORY_SORT_RES * res = (sGU_INVENTORY_SORT_RES *)packet.GetPacketData();
	res->wOpCode = GU_INVENTORY_SORT_RES;
	res->wResultCode = 500;
	res->byInventoryType = byInventoryType;
	//res->byMoveCount = 0;
	//res->byDelCount = 0;
	//res->wTotalCount = 1;

	//memset(res->moveItem, 1, sizeof(res->moveItem));
	//memset(res->delItem, 1, sizeof(res->delItem));

	//printf("sGU_INVENTORY_SORT_RES size = %I64u, %I64u, %I64u \n", sizeof(sGU_INVENTORY_SORT_RES), sizeof(res->moveItem), sizeof(res->delItem));
	//if (byInventoryType == 0)
	//{
	//	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
	//	{
	//		CItem* item = it->second;
	//		if (item->GetPlace() == CONTAINER_TYPE_BAG1)
	//		{
	//			res->moveItem[res->byMoveCount].hItem = item->GetID();
	//			res->moveItem[res->byMoveCount].byPlace = item->GetPlace();
	//			res->moveItem[res->byMoveCount].byPos = item->GetPos();

	//			res->byMoveCount++;
	//		}

	//		//if (item->GetPlace() == 0 && item->GetPos() == 1)
	//		//{
	//		//	res->delItem[res->byDelCount].hItem = item->GetID(); //should never be handle of inventory bag
	//		//	res->delItem[res->byDelCount].byUnknown = 2;

	//		//	res->byDelCount++;
	//		//}
	//	}

	//}
	//else if (byInventoryType == 2)
	//{
	//	for (auto it = m_map_BankItems.begin(); it != m_map_BankItems.end(); it++)
	//	{
	//		CItem* item = it->second;

	//	}
	//}
	//else
		res->wResultCode = 501;

	packet.SetPacketLen(sizeof(sGU_INVENTORY_SORT_RES));
	m_pOwner->SendPacket(&packet);
}

void CPlayerItemContainer::ProcessItemWithDuration(DWORD dwTickDiff)
{
	m_dwDurationUpdateTick = UnsignedSafeDecrease<DWORD>(m_dwDurationUpdateTick, dwTickDiff);

	if (m_dwDurationUpdateTick == 0)
	{
		m_dwDurationUpdateTick = 1000;
	//	printf("m_mapItemsWithDuration.size() %u \n", (int)m_mapItemsWithDuration.size());
		CGameServer* app = (CGameServer*)g_pApp;
		DBOTIME curTime = app->GetTime();

		for (std::map<HOBJECT, CItem*>::iterator it = m_mapItemsWithDuration.begin(); it != m_mapItemsWithDuration.end(); )
		{
			CItem* pItem = it->second;
			if (pItem)
			{
				if (pItem->GetUseEndTime() <= curTime)
				{
					it = m_mapItemsWithDuration.erase(it);

					pItem->SetUseEndTime(0);
					pItem->SetLocked(true);

			//		printf("pItem->GetTbldat()->byDurationType %u \n", pItem->GetTbldat()->byDurationType);
					if (pItem->GetTbldat()->byDurationType == eDURATIONTYPE_FLATSUM)
					{
						CNtlPacket pQry(sizeof(sGQ_ITEM_CHANGE_DURATIONTIME_REQ));
						sGQ_ITEM_CHANGE_DURATIONTIME_REQ* rQry = (sGQ_ITEM_CHANGE_DURATIONTIME_REQ*)pQry.GetPacketData();
						rQry->wOpCode = GQ_ITEM_CHANGE_DURATIONTIME_REQ;
						rQry->charId = m_pOwner->GetCharID();
						rQry->handle = m_pOwner->GetID();
						memcpy(&rQry->sItem, &pItem->GetItemData(), sizeof(sITEM_DATA));
						pQry.SetPacketLen(sizeof(sGQ_ITEM_CHANGE_DURATIONTIME_REQ));
						app->SendTo(app->GetQueryServerSession(), &pQry);
					}
					else
					{
						CNtlPacket pQry(sizeof(sGQ_ITEM_GRADEINIT_BY_COUPON_REQ));
						sGQ_ITEM_GRADEINIT_BY_COUPON_REQ * rQry = (sGQ_ITEM_GRADEINIT_BY_COUPON_REQ *)pQry.GetPacketData();
						rQry->wOpCode = GQ_ITEM_GRADEINIT_BY_COUPON_REQ;
						rQry->handle = m_pOwner->GetID();
						rQry->charId = m_pOwner->GetCharID();
						rQry->itemId = pItem->GetItemID();
						rQry->byItemPlace = pItem->GetPlace();
						rQry->byItemPos = pItem->GetPos();
						rQry->byRestrictState = ITEM_RESTRICT_STATE_TYPE_NONE;
						pQry.SetPacketLen(sizeof(sGQ_ITEM_GRADEINIT_BY_COUPON_REQ));
						app->SendTo(app->GetQueryServerSession(), &pQry);
					}

					continue;
				}
			}

			++it;
		}
	}
}

void CPlayerItemContainer::AddItemDuration(HOBJECT hHandle, CItem * pItem)
{
	m_mapItemsWithDuration.insert({ hHandle, pItem });
}

bool CPlayerItemContainer::HasItemInDuration(HOBJECT hHandle)
{
	if (m_mapItemsWithDuration.find(hHandle) != m_mapItemsWithDuration.end())
		return true;

	return false;
}


void CPlayerItemContainer::SetItemBrief(BYTE byPos, sITEM_DATA * pItemData)
{
	m_sItemBrief[byPos].byBattleAttribute = pItemData->byBattleAttribute;
	m_sItemBrief[byPos].byGrade = pItemData->byGrade;
	m_sItemBrief[byPos].tblidx = pItemData->itemNo;
}


void CPlayerItemContainer::UnsetItemBrief(BYTE byPos)
{
	m_sItemBrief[byPos].tblidx = INVALID_TBLIDX;
}

void CPlayerItemContainer::CopyItemBrief(sITEM_BRIEF * pBrief)
{
	memcpy(pBrief, m_sItemBrief, sizeof(m_sItemBrief));
}


void CPlayerItemContainer::OnDecreaseEquipmentDurability(BYTE* pbyDur, BYTE& rbyApplyCount)
{
	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
	{
		CItem* item = it->second;
		if (item->GetPlace() == CONTAINER_TYPE_EQUIP && item->GetDurability() > 0)
		{
			sITEM_TBLDAT* itemTbl = item->GetTbldat();
			pbyDur[item->GetPos()] = itemTbl->byDurability;

			if (item->GetPos() >= EQUIP_SLOT_TYPE_HAND && item->GetPos() <= EQUIP_SLOT_TYPE_SCOUTER)
			{
				if (itemTbl->byDurability != INVALID_BYTE)
				{
					BYTE byNewDurability = UnsignedSafeDecrease<BYTE>(item->GetDurability(), 20);
					item->SetDurability(byNewDurability);

					++rbyApplyCount;

					pbyDur[item->GetPos()] = byNewDurability;
				}
			}
		}
	}
}

void CPlayerItemContainer::CopyBaseItemAttributesTo(sAVATAR_ATTRIBUTE & avt)
{
	for (BYTE a = 0; a <= EQUIP_SLOT_TYPE_BOOTS; a++)
	{
		CItem* item = GetItem(CONTAINER_TYPE_EQUIP, a);
		if (item && item->GetDurability() > 0)
		{
			sITEM_TBLDAT* pItemTbldat = item->GetTbldat();
			if (pItemTbldat)
			{
				//Only add offence from hand and not sub weapon
				if (item->GetPos() == EQUIP_SLOT_TYPE_HAND)
				{
					if (pItemTbldat->wPhysical_Offence != INVALID_WORD)
						avt.wPhysicalOffence += Dbo_GetFinalOffence(pItemTbldat->wPhysical_Offence, item->GetGrade());

					if (pItemTbldat->wEnergy_Offence != INVALID_WORD)
						avt.wEnergyOffence += Dbo_GetFinalOffence(pItemTbldat->wEnergy_Offence, item->GetGrade());
				}

				if (pItemTbldat->wAttack_Speed_Rate != INVALID_WORD)
					avt.wAttackSpeedRate = pItemTbldat->wAttack_Speed_Rate;

				if (pItemTbldat->wPhysical_Defence != INVALID_WORD)
					avt.wPhysicalDefence += Dbo_GetFinalDefence(pItemTbldat->wPhysical_Defence, item->GetGrade());

				if (pItemTbldat->wEnergy_Defence != INVALID_WORD)
					avt.wEnergyDefence += Dbo_GetFinalDefence(pItemTbldat->wEnergy_Defence, item->GetGrade());

				if (pItemTbldat->fAttack_Range_Bonus != INVALID_FLOAT)
					avt.fAttackRange += pItemTbldat->fAttack_Range_Bonus;
			}
		}
	}
}

void CPlayerItemContainer::CopyItemAttributesTo(CCharacterAtt* pCharAtt)
{
	std::vector<TBLIDX> vecSetWeapon; //gloves, sub weapon & necklace
	std::vector<TBLIDX> vecSetArmor;

	int nSetEarringCount = 0;
	sITEM_TBLDAT* setEarringIdx[NTL_SET_ITEM_SEMI_COUNT]; //semi because we can only have max 2
	memset(setEarringIdx, NULL, sizeof(setEarringIdx));

	int nSetRingCount = 0;
	sITEM_TBLDAT* setRingIdx[NTL_SET_ITEM_SEMI_COUNT]; //semi because we can only have max 2
	memset(setRingIdx, NULL, sizeof(setRingIdx));

//	for (auto it = m_map_CharItems.begin(); it != m_map_CharItems.end(); it++)
	for(BYTE a = 0; a < EQUIP_SLOT_TYPE_COUNT; a++)
	{
		CItem* item = GetItem(CONTAINER_TYPE_EQUIP, a);
		if (item /*item->GetPlace() == CONTAINER_TYPE_EQUIP*/ && item->GetDurability() > 0)
		{
			sITEM_TBLDAT* pinvitemdata = item->GetTbldat();
			if (pinvitemdata)
			{
				if (item->GetPos() == EQUIP_SLOT_TYPE_HAND)
				{
					m_pOwner->GetCharAtt()->SetBattleAttributeOffence(item->GetBattleAttribute());

					if (pinvitemdata->set_Item_Tblidx != INVALID_TBLIDX)
						vecSetWeapon.push_back(pinvitemdata->set_Item_Tblidx);
				}
				else if (item->GetPos() == EQUIP_SLOT_TYPE_SUB_WEAPON)
				{
					if (pinvitemdata->set_Item_Tblidx != INVALID_TBLIDX)
						vecSetWeapon.push_back(pinvitemdata->set_Item_Tblidx);
				}
				else if (item->GetPos() == EQUIP_SLOT_TYPE_JACKET)
				{
					if (pinvitemdata->set_Item_Tblidx != INVALID_TBLIDX)
						vecSetArmor.push_back(pinvitemdata->set_Item_Tblidx);
				}
				else if (item->GetPos() == EQUIP_SLOT_TYPE_PANTS)
				{
					if (pinvitemdata->set_Item_Tblidx != INVALID_TBLIDX)
						vecSetArmor.push_back(pinvitemdata->set_Item_Tblidx);
				}
				else if (item->GetPos() == EQUIP_SLOT_TYPE_BOOTS)
				{
					if (pinvitemdata->set_Item_Tblidx != INVALID_TBLIDX)
						vecSetArmor.push_back(pinvitemdata->set_Item_Tblidx);
				}
				else if (item->GetPos() == EQUIP_SLOT_TYPE_NECKLACE)
				{
					if (pinvitemdata->set_Item_Tblidx != INVALID_TBLIDX)
						vecSetWeapon.push_back(pinvitemdata->set_Item_Tblidx); //neck count as weapon
				}
				else if (item->GetPos() == EQUIP_SLOT_TYPE_EARRING_1 || item->GetPos() == EQUIP_SLOT_TYPE_EARRING_2)
				{
					if (pinvitemdata->set_Item_Tblidx != INVALID_TBLIDX)
						setEarringIdx[nSetEarringCount++] = pinvitemdata;
				}
				else if (item->GetPos() == EQUIP_SLOT_TYPE_RING_1 || item->GetPos() == EQUIP_SLOT_TYPE_RING_2)
				{
					if (pinvitemdata->set_Item_Tblidx != INVALID_TBLIDX)
						setRingIdx[nSetRingCount++] = pinvitemdata;
				}
				

				if (item->GetPos() >= EQUIP_SLOT_TYPE_COSTUME_SET) // dogi ball
				{
					for (int i = 6; i < NTL_MAX_RANDOM_OPTION_IN_ITEM; i++) // dogi ball effect is on pos 6 and 7
					{
						if (item->GetOptionSet().GetRandomOptionIndex(i) != INVALID_WORD && item->GetOptionSet().GetRandomOptionValue(i) > 0)
						{
							sITEM_ENCHANT_TBLDAT* pItemEnchantTbldat = (sITEM_ENCHANT_TBLDAT*)g_pTableContainer->GetItemEnchantTable()->FindData(item->GetOptionSet().GetRandomOptionIndex(i));
							if (pItemEnchantTbldat)
							{
								eSYSTEM_EFFECT_CODE effectcode = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pItemEnchantTbldat->seTblidx);
								if (effectcode != INVALID_SYSTEM_EFFECT_CODE)
								{
									Dbo_SetRandomOptionValues(pCharAtt, effectcode, (float)item->GetOptionSet().GetRandomOptionValue(i));
								}
								else
									ERR_LOG(LOG_GENERAL, "error g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[i]) NOT FOUND \n");
							}
						}
						else break;
					}
				}

				//defined option
				for (int i = 0; i < NTL_MAX_OPTION_IN_ITEM; i++)
				{
					if (item->GetOptionSet().aOptionTblidx[i] != INVALID_TBLIDX)
					{
						sITEM_OPTION_TBLDAT* optionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(item->GetOptionSet().aOptionTblidx[i]);
						if (optionTbldat)
						{
							for (int ii = 0; ii < NTL_MAX_SYSTEM_EFFECT_COUNT; ii++)
							{
								if (optionTbldat->system_Effect[ii] == INVALID_TBLIDX)
									break;

								eSYSTEM_EFFECT_CODE effectcode = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[ii]);
								if (effectcode != INVALID_SYSTEM_EFFECT_CODE)
								{
									Dbo_SetAvatarAttributeValue(pCharAtt, effectcode, (float)optionTbldat->nValue[ii], (BYTE)optionTbldat->bAppliedInPercent[ii]);
								}
								else
									ERR_LOG(LOG_GENERAL, "error g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[i]) NOT FOUND \n");
							}
						}
					}
					else break;
				}

				//random option
				for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++)
				{
					if (item->GetOptionSet().GetRandomOptionIndex(i) != INVALID_WORD && item->GetOptionSet().GetRandomOptionValue(i) > 0)
					{
						sITEM_ENCHANT_TBLDAT* pItemEnchantTbldat = (sITEM_ENCHANT_TBLDAT*)g_pTableContainer->GetItemEnchantTable()->FindData(item->GetOptionSet().GetRandomOptionIndex(i));
						if (pItemEnchantTbldat)
						{
							eSYSTEM_EFFECT_CODE effectcode = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pItemEnchantTbldat->seTblidx);
							if (effectcode != INVALID_SYSTEM_EFFECT_CODE)
							{
								Dbo_SetRandomOptionValues(pCharAtt, effectcode, (float)item->GetOptionSet().GetRandomOptionValue(i));
							}
							else
								ERR_LOG(LOG_GENERAL, "error g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[i]) NOT FOUND \n");
						}
					}
					else break;
				}

			}
		}
	}

	//weapon set
	if (vecSetWeapon.size() >= NTL_SET_ITEM_SEMI_COUNT) //only check if we have at least two items
	{
		for (int i = 0; i < NTL_SET_ITEM_SEMI_COUNT; i++) //only need 2 loops because max count is 3 and there is no reason to check 3x
		{
			TBLIDX setIdx = vecSetWeapon[i];
			int nFoundCount = (int)std::count(vecSetWeapon.begin(), vecSetWeapon.end(), setIdx);

			if (nFoundCount == NTL_SET_ITEM_SOLO_COUNT && vecSetWeapon.size() == 2) //if found only 1 and we only have 2 items then we can break loop
				break;
			else if (nFoundCount == NTL_SET_ITEM_SOLO_COUNT) //else if size is 3 and we found only 1 then continue.
				continue;

			if (nFoundCount >= NTL_SET_ITEM_SEMI_COUNT) //check if 2 or more. Because if we have full set, then the semi set will be calculated too
			{
				sSET_ITEM_TBLDAT* pSetItemTbldat = (sSET_ITEM_TBLDAT*)g_pTableContainer->GetSetItemTable()->FindData(setIdx);
				sITEM_OPTION_TBLDAT* optionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(pSetItemTbldat->semiSetOption);

				//always only have 1 effect code
				Dbo_SetAvatarAttributeValue(pCharAtt, g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[0]), (float)optionTbldat->nValue[0], (BYTE)optionTbldat->bAppliedInPercent[0]);

				break;
			}
		}
	}

	//armor set
	if (vecSetArmor.size() >= NTL_SET_ITEM_SEMI_COUNT) //only check if we have at least two items
	{
		bool bSemi = false;
		bool bFull = false;

		for (int i = 0; i < NTL_SET_ITEM_SEMI_COUNT; i++) //only need 2 loops because max count is 3 and there is no reason to check 3x
		{
			TBLIDX setIdx = vecSetArmor[i];
			int nFoundCount = (int)std::count(vecSetArmor.begin(), vecSetArmor.end(), setIdx);

			if (nFoundCount == NTL_SET_ITEM_SOLO_COUNT && vecSetArmor.size() == 2) //if found only 1 and we only have 2 items then we can break loop
				break;
			else if (nFoundCount == NTL_SET_ITEM_SOLO_COUNT) //else if size is 3 and we found only 1 then continue.
				continue;

			if (nFoundCount >= NTL_SET_ITEM_SEMI_COUNT && bSemi == false) //check if 2 or more. Because if we have full set, then the semi set will be calculated too
			{
				bSemi = true;

				sSET_ITEM_TBLDAT* pSetItemTbldat = (sSET_ITEM_TBLDAT*)g_pTableContainer->GetSetItemTable()->FindData(setIdx);
				sITEM_OPTION_TBLDAT* optionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(pSetItemTbldat->semiSetOption);

				//always only have 1 effect code
				Dbo_SetAvatarAttributeValue(pCharAtt, g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[0]), (float)optionTbldat->nValue[0], (BYTE)optionTbldat->bAppliedInPercent[0]);
			}

			if (nFoundCount == NTL_SET_ITEM_FULL_COUNT && bFull == false) //check if found all
			{
				bFull = true;

				sSET_ITEM_TBLDAT* pSetItemTbldat = (sSET_ITEM_TBLDAT*)g_pTableContainer->GetSetItemTable()->FindData(setIdx);
				sITEM_OPTION_TBLDAT* optionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(pSetItemTbldat->fullSetOption);

				//always only have 1 effect code
				Dbo_SetAvatarAttributeValue(pCharAtt, g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[0]), (float)optionTbldat->nValue[0], (BYTE)optionTbldat->bAppliedInPercent[0]);

				break; //break loop here if we have full set.
			}
		}
	}

	//earring set
	if (nSetEarringCount == NTL_SET_ITEM_SEMI_COUNT) //check if 2 earring exist with set tblidx
	{
		if (setEarringIdx[0] && setEarringIdx[1] && (setEarringIdx[0]->tblidx != setEarringIdx[1]->tblidx) && (setEarringIdx[0]->set_Item_Tblidx == setEarringIdx[1]->set_Item_Tblidx)) //check if earring set complete && check if both not the same item
		{
			sSET_ITEM_TBLDAT* pSetItemTbldat = (sSET_ITEM_TBLDAT*)g_pTableContainer->GetSetItemTable()->FindData(setEarringIdx[0]->set_Item_Tblidx);
			if (pSetItemTbldat)
			{
				sITEM_OPTION_TBLDAT* optionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(pSetItemTbldat->semiSetOption);
				if (optionTbldat)
				{
					//always only have 1 effect code
					Dbo_SetAvatarAttributeValue(pCharAtt, g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[0]), (float)optionTbldat->nValue[0], (BYTE)optionTbldat->bAppliedInPercent[0]);
				}
				else ERR_LOG(LOG_GENERAL, "ERROR: Could not find sITEM_OPTION_TBLDAT. Tblidx %u, Item Tblidx %u", pSetItemTbldat->semiSetOption, setEarringIdx[0]->tblidx);
			}
			else ERR_LOG(LOG_GENERAL, "ERROR: Could not find sSET_ITEM_TBLDAT. SetTblidx %u, ItemTblidx %u", setEarringIdx[0]->set_Item_Tblidx, setEarringIdx[0]->tblidx);
		}
	}

	//ring set
	if (nSetRingCount == NTL_SET_ITEM_SEMI_COUNT) //check if 2 ring exist with set tblidx
	{
		if (setRingIdx[0] && setRingIdx[1] && (setRingIdx[0]->tblidx != setRingIdx[1]->tblidx) && (setRingIdx[0]->set_Item_Tblidx == setRingIdx[1]->set_Item_Tblidx)) //check if earring set complete && check if both not the same item
		{
			sSET_ITEM_TBLDAT* pSetItemTbldat = (sSET_ITEM_TBLDAT*)g_pTableContainer->GetSetItemTable()->FindData(setRingIdx[0]->set_Item_Tblidx);
			if (pSetItemTbldat)
			{
				sITEM_OPTION_TBLDAT* optionTbldat = (sITEM_OPTION_TBLDAT*)g_pTableContainer->GetItemOptionTable()->FindData(pSetItemTbldat->semiSetOption);
				if (optionTbldat)
				{
					//always only have 1 effect code
					Dbo_SetAvatarAttributeValue(pCharAtt, g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(optionTbldat->system_Effect[0]), (float)optionTbldat->nValue[0], (BYTE)optionTbldat->bAppliedInPercent[0]);
				}
				else ERR_LOG(LOG_GENERAL, "ERROR: Could not find sITEM_OPTION_TBLDAT. Tblidx %u, Item Tblidx %u", pSetItemTbldat->semiSetOption, setRingIdx[0]->tblidx);


			}
			else ERR_LOG(LOG_GENERAL, "ERROR: Could not find sSET_ITEM_TBLDAT. SetTblidx %u, ItemTblidx %u", setRingIdx[0]->set_Item_Tblidx, setRingIdx[0]->tblidx);
		}
	}
}

bool CPlayerItemContainer::WearGenderRequiredItem(BYTE byNewGender)
{
	for (BYTE a = 0; a < EQUIP_SLOT_TYPE_COUNT; a++)
	{
		CItem* item = GetItem(CONTAINER_TYPE_EQUIP, a);
		if (item && item->GetPlace() == CONTAINER_TYPE_EQUIP)
		{
			sITEM_TBLDAT* pItemData = item->GetTbldat();
			if (pItemData)
			{
				if (pItemData->dwNeed_Gender_Bit_Flag > 0 && BIT_FLAG_TEST(MAKE_BIT_FLAG(byNewGender), pItemData->dwNeed_Gender_Bit_Flag) == false)
					return true;
			}
		}
	}

	return false;
}

void CPlayerItemContainer::InsertBankItem(HOBJECT hHandle, CItem * item)
{
	m_map_BankItems.insert(std::make_pair(hHandle, item));
}

CItem * CPlayerItemContainer::GetBankItem(HOBJECT hHandle)
{
	auto it = m_map_BankItems.find(hHandle);
	if (it != m_map_BankItems.end())
		return it->second;

	return nullptr;
}

void CPlayerItemContainer::FreeGuildBank()
{
	m_dwGuildZeni = 0;

	for (auto it = m_map_GuildBankItems.begin(); it != m_map_GuildBankItems.end(); it++)
	{
		CItem* pItem = it->second;
		pItem->SetOwner(NULL);
		g_pItemManager->DestroyItem(pItem);
	}
	m_map_GuildBankItems.clear();
}






