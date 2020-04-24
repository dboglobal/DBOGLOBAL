#include "stdafx.h"
#include "ItemManager.h"
#include "item.h"
#include "CPlayer.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "UseItemTable.h"
#include "TableContainerManager.h"
#include "ItemBagListTable.h"
#include "GameServer.h"
#include "NtlRandom.h"
#include "ItemDrop.h"
#include "ItemGroupListTable.h"
#include "NtlPacketGQ.h"
#include "NtlPacketGU.h"
#include "DragonballHunt.h"
#include "DragonballScramble.h"
#include "QuestProbabilityTable.h"
#include "FormulaTable.h"


const DWORD ITEM_DELETE_DELAY = 5000;


CItemManager::CItemManager()
{
}

CItemManager::~CItemManager()
{
}


void CItemManager::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	for (std::map<CItemDrop*, DWORD>::iterator it = m_mapItemDropDelayedDestroy.begin(); it != m_mapItemDropDelayedDestroy.end(); )
	{
		it->second += dwTickDiff;

		if (it->second > ITEM_DELETE_DELAY)
		{
			CItemDrop* item = it->first;
			if (item && item->GetCurWorld())
			{
				ERR_LOG(LOG_GENERAL, "ERROR!! TRYING TO DELETE ITEMDROP WHILE ITS STILL ON WORLD!!!!");
				item->RemoveFromGround();
				it->second = 0;
				++it;
				continue;
			}
			
			it = m_mapItemDropDelayedDestroy.erase(it);

			g_pObjectManager->DeleteUID(item->GetID());
			SAFE_DELETE(item);
			continue;
		}

		++it;
	}

	for (std::map<CItem*, DWORD>::iterator it = m_mapItemDelayedDestroy.begin(); it != m_mapItemDelayedDestroy.end(); )
	{
		it->second += dwTickDiff;

		if (it->second > ITEM_DELETE_DELAY)
		{
			CItem* item = it->first;

			it = m_mapItemDelayedDestroy.erase(it);

			SAFE_DELETE(item);
			continue;
		}

		++it;
	}
}

void CItemManager::AddItem(CItem* item)
{
	m_map_pkItemByID.insert(std::make_pair(item->GetID(), item));
}

void CItemManager::RemoveItem(HOBJECT hId)
{
	m_map_pkItemByID.erase(hId);
}

void CItemManager::DestroyItem(CItem* item)
{
	m_map_pkItemByID.erase(item->GetID());

	item->SetLocked(true);

	//free item handle so other objects can use the handle and set item handle to invalid.
	g_pObjectManager->DeleteUID(item->GetID());
	item->SetID(INVALID_HOBJECT);

	m_mapItemDelayedDestroy.insert(std::make_pair(item, 0));
}

void CItemManager::DestroyItemDropOverTime(CItemDrop* item)
{
	if (item->GetCurWorld() && item->IsInitialized()) //if on ground
	{
		if (m_map_pkItemDrop.find(item->GetID()) != m_map_pkItemDrop.end())
		{
			m_map_pkItemDrop.erase(item->GetID());
			item->RemoveFromGround();

			m_mapItemDropDelayedDestroy.insert(std::make_pair(item, 0));
		}
	}
}

//When player login
CItem* CItemManager::CreateFromDB(HOBJECT hHandle, sITEM_DATA& rData, CPlayer* ch)
{
	sITEM_TBLDAT* itemtbl = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(rData.itemNo);
	if (itemtbl == NULL)
		return NULL;

	CItem* item = new CItem;
	item->SetTbldat(itemtbl);
	item->SetID(hHandle);
	item->SetItemData(&rData);

	m_map_pkItemByID.insert(std::make_pair(hHandle, item));
	item->AddToCharacter(ch);

	return item;
}

bool CItemManager::CreateItemDrop(CMonster* pkMob, CPlayer* pkKiller, std::vector<CItemDrop*> & vec_item)
{
	if (pkKiller->GetCurWorld() == NULL)
		return false;

	CGameServer* app = (CGameServer*)g_pApp;
	std::deque<sITEM_GROUP_LIST_TBLDAT*> itemGroupList;

	bool bIsLucky = Dbo_CheckProbabilityF(CFormulaTable::m_afRate[5200][1] - ((float)pkMob->GetLevel() * CFormulaTable::m_afRate[5200][2]));

	if (bIsLucky)
	{
		CNtlPacket packet(sizeof(sGU_MOB_LUCKY_DROP_NFY));
		sGU_MOB_LUCKY_DROP_NFY * res = (sGU_MOB_LUCKY_DROP_NFY*)packet.GetPacketData();
		res->wOpCode = GU_MOB_LUCKY_DROP_NFY;
		res->hMobhandle = pkMob->GetID();
		packet.SetPacketLen(sizeof(sGU_MOB_LUCKY_DROP_NFY));
		pkKiller->SendPacket(&packet);
	}

	g_pTableContainer->GetItemGroupListTable()->AccumulateItemGroupListByWorldRuleType(pkMob->GetLevel(), pkKiller->GetCurWorld()->GetRuleType(), pkKiller->GetLevel() + 5, itemGroupList); //drop defined by world place

	g_pTableContainer->GetItemGroupListTable()->AccumulateItemGroupListByMobTblidx(pkMob->GetLevel(), pkMob->GetTblidx(), itemGroupList); //quest , dungeon & field boss
	
	if (pkMob->GetMobType() == MOB_TYPE_ITEM_BOX)
		g_pTableContainer->GetItemGroupListTable()->AccumulateItemGroupListByMobType(pkMob->GetLevel(), pkMob->GetMobType(), pkKiller->GetLevel() + 5, itemGroupList); //item box

	//printf("item group list count %u\n pkMob->GetLevel %u, GetRuleType %u, pkKiller->GetLevel() %u, pkMob->GetMobType() %u\n", itemGroupList.size(), pkMob->GetLevel(), pkKiller->GetCurWorld()->GetRuleType(), pkKiller->GetLevel(), pkMob->GetMobType());


	if (itemGroupList.size() > 0)
	{
		int count = (int)(itemGroupList.size() - 1);
		float fDropRate = 0.0f;

		while (count >= 0)
		{
			sITEM_GROUP_LIST_TBLDAT* itemDrop = itemGroupList[count--];
			
			if (!itemDrop)
			{
				ERR_LOG(LOG_GENERAL, "itemGroupList is empty. deque count %d \n", count + 1);
				break;
			}

			for (BYTE byTryCount = 0; byTryCount < itemDrop->byTry_Count; byTryCount++)
			{
				if (itemDrop->mob_Index != INVALID_TBLIDX && itemDrop->dwItemBagCount > 0)
				{
					std::vector< std::pair<TBLIDX, bool> > vecItemDropIdx;

					for (unsigned int i = 0; i < itemDrop->dwItemBagCount; i++)
					{
						if (itemDrop->aItemBag[i] == INVALID_TBLIDX || itemDrop->aItemBag[i] == 0)
							continue;

						sITEM_BAG_LIST_TBLDAT* dropbag = (sITEM_BAG_LIST_TBLDAT*)g_pTableContainer->GetItemBagListTable()->FindData(itemDrop->aItemBag[i]);
						if (dropbag)
						{
							for (unsigned int ii = 0; ii < dropbag->dwItemCount; ii++)
							{
								if (dropbag->aItem[ii] == INVALID_TBLIDX || dropbag->aItem[ii] == 0)
									continue;

								vecItemDropIdx.push_back(std::make_pair(dropbag->aItem[ii], dropbag->bEnchant_Able));
							}
						}
					}
					//	printf("vecItemDropIdx size = %u \n", vecItemDropIdx.size());
					if (vecItemDropIdx.size() > 0)
					{
						std::pair<TBLIDX, bool> itemDropPair = vecItemDropIdx[RandomRange(0, (int)vecItemDropIdx.size() - 1)];

						sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(itemDropPair.first);
						if (pItemData)
						{
							CItemDrop* item = new CItemDrop(OBJTYPE_DROPITEM);
							item->SetID(g_pObjectManager->CreateUID());
							item->SetTbldat(pItemData, pkKiller->GetCurWorld()->GetTbldat()->bDynamic == false);

							item->GenerateRank(pItemData, pkKiller->GetCurWorld()->GetTbldat()->bDynamic, pkMob->GetMobRank(), itemDrop->dwSuperior, itemDrop->dwExcellent, itemDrop->dwRare, itemDrop->dwLegendary);
							item->GenerateOptionSet(pItemData, itemDropPair.second);

							//	printf("pItemData->tblidx %u, itemDrop->tblidx %u, dropbag->tblidx %u\nitemDrop->dwSuperior %u, itemDrop->dwExcellent %u, itemDrop->dwRare %u, itemDrop->dwLegendary %u\npItemData->bCreateSuperiorAble %u, pItemData->bCreateExcellentAble %u, pItemData->bCreateRareAble %u, pItemData->bCreateLegendaryAble %u \n", 
							//			pItemData->tblidx, itemDrop->tblidx, itemDropPair.first, itemDrop->dwSuperior, itemDrop->dwExcellent, itemDrop->dwRare, itemDrop->dwLegendary, pItemData->bCreateSuperiorAble, pItemData->bCreateExcellentAble, pItemData->bCreateRareAble, pItemData->bCreateLegendaryAble);

							m_map_pkItemDrop.insert(std::make_pair(item->GetID(), item));
							vec_item.push_back(item);
						}
						else ERR_LOG(LOG_SYSTEM, "Couldnt find item tblidx %u from boss drop", itemDropPair.first);
					}
				}
				else
				{
					//here we create the drop and insert into the vector
					for (unsigned int i = 0; i < itemDrop->dwItemBagCount; i++)
					{
						if (itemDrop->aItemBag[i] == INVALID_TBLIDX || itemDrop->aItemBag[i] == 0)
							continue;

						fDropRate = (float)itemDrop->adwProb[i] + ((float)itemDrop->adwProb[i] * (float)app->GetItemDropRate() / 100.f);

						if (Dbo_CheckProbabilityF(fDropRate)) //check percent
						{
							sITEM_BAG_LIST_TBLDAT* dropbag = (sITEM_BAG_LIST_TBLDAT*)g_pTableContainer->GetItemBagListTable()->FindData(itemDrop->aItemBag[i]);
							if (dropbag)
							{
								for (unsigned int ii = 0; ii < dropbag->dwItemCount; ii++)
								{
									if (dropbag->aItem[ii] == INVALID_TBLIDX || dropbag->aItem[ii] == 0)
										continue;

									//	printf("tblidx %u, byLevel %u, byTry_Count %u, dwExcellent %u, dwInterval %u, dwLegendary %u, dwMob_Type %u, dwRare %u, dwSuperior %u, dwTotalProb %u, dwWorld_Rule_Type %u, dwZenny %u \n",
									//		itemDrop->tblidx, itemDrop->byLevel, itemDrop->byTry_Count, itemDrop->dwExcellent, itemDrop->dwInterval, itemDrop->dwLegendary, itemDrop->dwMob_Type, itemDrop->dwRare, itemDrop->dwSuperior, itemDrop->dwTotalProb, itemDrop->dwWorld_Rule_Type, itemDrop->dwZenny);

									fDropRate = (float)dropbag->adwProb[ii] + ((float)dropbag->adwProb[ii] * (float)app->GetItemDropRate() / 100.f);

									if (bIsLucky)
										fDropRate *= CFormulaTable::m_afRate[5300][2];

									if (Dbo_CheckProbabilityF(fDropRate)) //check percent
									{
										sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(dropbag->aItem[ii]);
										if (pItemData)
										{
											CItemDrop* item = new CItemDrop(OBJTYPE_DROPITEM);
											item->SetID(g_pObjectManager->CreateUID());
											item->SetTbldat(pItemData, pkKiller->GetCurWorld()->GetTbldat()->bDynamic == false);

											item->GenerateRank(pItemData, pkKiller->GetCurWorld()->GetTbldat()->bDynamic, pkMob->GetMobRank(), itemDrop->dwSuperior, itemDrop->dwExcellent, itemDrop->dwRare, itemDrop->dwLegendary);
											item->GenerateOptionSet(pItemData, dropbag->bEnchant_Able);

											//	printf("pItemData->tblidx %u, itemDrop->tblidx %u, dropbag->tblidx %u\nitemDrop->dwSuperior %u, itemDrop->dwExcellent %u, itemDrop->dwRare %u, itemDrop->dwLegendary %u\npItemData->bCreateSuperiorAble %u, pItemData->bCreateExcellentAble %u, pItemData->bCreateRareAble %u, pItemData->bCreateLegendaryAble %u \n", 
											//		pItemData->tblidx, itemDrop->tblidx, dropbag->tblidx, itemDrop->dwSuperior, itemDrop->dwExcellent, itemDrop->dwRare, itemDrop->dwLegendary, pItemData->bCreateSuperiorAble, pItemData->bCreateExcellentAble, pItemData->bCreateRareAble, pItemData->bCreateLegendaryAble);

											m_map_pkItemDrop.insert(std::make_pair(item->GetID(), item));
											vec_item.push_back(item);
										}
										else ERR_LOG(LOG_SYSTEM, "Couldnt find item tblidx %u sITEM_BAG_LIST_TBLDAT tblidx %u", dropbag->aItem[ii], dropbag->tblidx);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//zeni drop
	float fDropRate = pkMob->GetZeniDropRate() + (pkMob->GetZeniDropRate() * (float)app->GetZeniDropRate() / 100.f);
	if (pkMob->GetZeniDrop() > 0 && (bIsLucky || Dbo_CheckProbabilityF(fDropRate))) //if lucky drop, then 100% zeni drop rate
	{
		DWORD dwZeniDrop = (DWORD)((float)pkMob->GetZeniDrop() * RandomRangeF(0.5f, 1.0f));

		if (bIsLucky)
			dwZeniDrop *= (DWORD)(CFormulaTable::m_afRate[5500][1]);

		if (dwZeniDrop > 0)
		{
			CItemDrop* dropMoney = new CItemDrop(OBJTYPE_DROPMONEY);
			dropMoney->SetID(g_pObjectManager->CreateUID());
			dropMoney->SetZeni(dwZeniDrop);
			m_map_pkItemDrop.insert(std::make_pair(dropMoney->GetID(), dropMoney));
			vec_item.push_back(dropMoney);
		}
	}


	if (g_pDragonballHuntEvent->IsOn() && pkMob->HasDragonball() && pkMob->GetKillerCharID() == pkKiller->GetCharID())
	{
		if (pkKiller->GetLevel() <= pkMob->GetLevel() + DBO_DRAGONBALL_EVENT_DROP_LEVEL_DIFF
			&& pkKiller->GetLevel() >= pkMob->GetLevel() - DBO_DRAGONBALL_EVENT_DROP_LEVEL_DIFF) //check level difference
		{
			if (Dbo_CheckProbability(4 * (pkMob->GetMobRank() + 1))) //check percent
			{
				sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(DBO_BLANK_DRAGON_BALL_IDX);
				if (pItemData)
				{
					g_pDragonballHuntEvent->IncDragonballDrop();

					CItemDrop* item = new CItemDrop(OBJTYPE_DROPITEM);
					item->SetID(g_pObjectManager->CreateUID());
					item->SetTbldat(pItemData);
					item->GenerateOptionSet(pItemData);

					m_map_pkItemDrop.insert(std::make_pair(item->GetID(), item));
					vec_item.push_back(item);
				}
				else ERR_LOG(LOG_SYSTEM, "Couldnt find dragonball item tblidx 19989 \n");
			}
		}
	}

	if (vec_item.size() == 0)
		return false;
	else
		return true;
}

CItemDrop * CItemManager::CreateSingleDrop(float fRate, TBLIDX itemTblidx)
{
	if (Dbo_CheckProbabilityF(fRate)) //check percent
	{
		sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(itemTblidx);
		if (pItemData)
		{
			CItemDrop* item = new CItemDrop(OBJTYPE_DROPITEM);
			item->SetID(g_pObjectManager->CreateUID());
			item->SetTbldat(pItemData);
			item->GenerateOptionSet(pItemData, false);

			m_map_pkItemDrop.insert(std::make_pair(item->GetID(), item));
			
			return item;
		}
		else ERR_LOG(LOG_SYSTEM, "Couldnt find item tblidx %u", itemTblidx);
	}

	return NULL;
}

void CItemManager::CreateItemDrop(TBLIDX dropItem_ProbabilityTblidx, std::vector<CItemDrop*>& vec_item)
{
	sQUEST_PROBABILITY_TBLDAT* pProbabilityData = (sQUEST_PROBABILITY_TBLDAT*)g_pTableContainer->GetQuestProbabilityTable()->FindData(dropItem_ProbabilityTblidx);
	if (pProbabilityData)
	{
		bool bRewardReceived = false;
		int nCount = 0;
		int nReward[NTL_QUEST_PROBABILITY_MAX_COUNT];
		memset(nReward, NULL, sizeof(nReward));

	LOOP_REPEAT:
		for (int c = 0; c < pProbabilityData->byCount; c++)
		{
			if (nCount >= NTL_QUEST_PROBABILITY_MAX_COUNT)
				nCount = NTL_QUEST_PROBABILITY_MAX_COUNT - 1;

			if (Dbo_CheckProbability(pProbabilityData->asProbabilityData[c].dwRate / 10000))
			{
				nReward[nCount++] = c;
				bRewardReceived = true;
				continue;
			}

			if (c == pProbabilityData->byCount - 1 && !bRewardReceived)
			{
				ERR_LOG(LOG_GENERAL, "Repeat loop.");
				goto LOOP_REPEAT;
			}
		}

		if (bRewardReceived == true)
		{
			int nRandomReward = 0;

			if (nCount > 1)
				nRandomReward = RandomRange(0, nCount - 1);

			if (nRandomReward >= NTL_QUEST_PROBABILITY_MAX_COUNT)
				nRandomReward = NTL_QUEST_PROBABILITY_MAX_COUNT - 1;

			switch (pProbabilityData->asProbabilityData[nReward[nRandomReward]].byType)
			{
				case eREWARD_TYPE_NORMAL_ITEM:
				{
					BYTE randomCount = (BYTE)RandomRange((int)pProbabilityData->asProbabilityData[nReward[nRandomReward]].dwMinValue, (int)pProbabilityData->asProbabilityData[nReward[nRandomReward]].dwMaxValue);
					if (randomCount > 0)
					{
						sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(pProbabilityData->asProbabilityData[nReward[nRandomReward]].tblidx);
						if (pItemData)
						{
							CItemDrop* item = new CItemDrop(OBJTYPE_DROPITEM);
							item->SetID(g_pObjectManager->CreateUID());
							item->SetTbldat(pItemData);
							item->GenerateOptionSet(pItemData, false);

							m_map_pkItemDrop.insert(std::make_pair(item->GetID(), item));
							vec_item.push_back(item);
						}
						else ERR_LOG(LOG_SYSTEM, "Couldnt find item tblidx %u. dropItem_ProbabilityTblidx %u \n", pProbabilityData->asProbabilityData[nReward[nRandomReward]].tblidx, dropItem_ProbabilityTblidx);
					}
				}

				default: break;
			}
		}
	}
}


CItem* CItemManager::CreateItem(CPlayer* pPlayer, sITEM_DATA* pItemData, bool bSendPacket/* = true*/)
{
	sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(pItemData->itemNo);

	CItem* item = new CItem;
	item->SetTbldat(pItemTbldat);
	item->SetID(g_pObjectManager->CreateUID());
	item->SetItemData(pItemData);
	item->AddToCharacter(pPlayer);

	m_map_pkItemByID.insert(std::make_pair(item->GetID(), item));

	if (bSendPacket)
	{
		CNtlPacket packet(sizeof(sGU_ITEM_CREATE));
		sGU_ITEM_CREATE * res = (sGU_ITEM_CREATE *)packet.GetPacketData();
		res->wOpCode = GU_ITEM_CREATE;
		res->bIsNew = true;
		res->handle = item->GetID();
		memcpy(&res->sItemData, &item->GetItemData(), sizeof(sITEM_DATA));
		packet.SetPacketLen(sizeof(sGU_ITEM_CREATE));
		pPlayer->SendPacket(&packet);
	}

	// -- Check if its a scramble ball
	if (pItemTbldat->tblidx >= 200041 && pItemTbldat->tblidx <= 200047)
	{
		g_pDragonballScramble->PickUpBall(pPlayer, item->GetID(), pItemTbldat->tblidx);
	}

	return item;
}


bool CItemManager::CreateItem(CPlayer* ch, TBLIDX uiIdx, BYTE byCount, BYTE byDestPlace/* = 255*/, BYTE byDestPos/* = 255*/, bool bEnchantAble/* = false*/)
{
	sITEM_TBLDAT* itemtbl = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(uiIdx);
	if (itemtbl == NULL)
	{
		ERR_LOG(LOG_USER, "User %u can not create item tblidx %u because it does not exist. byCount = %u", ch->GetCharID(), byCount);
		return false;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	if(byDestPlace == INVALID_BYTE && byDestPos == INVALID_BYTE) //only check if invalid dest place & pos
	{
		//check if same tblidx already exist && can stack
		if(itemtbl->byMax_Stack > 1)
		{
			CItem* itemcheck = ch->GetPlayerItemContainer()->CheckStackItem(uiIdx, byCount, itemtbl->byMax_Stack, GetDefaultRestrictState(itemtbl->byRestrictType, itemtbl->byItem_Type, true));
			if(itemcheck)
			{
				itemcheck->SetCount(itemcheck->GetCount() + byCount, true, true);
				return true;
			}
		}
	}
	
	std::pair<BYTE,BYTE> inv;
	if(byDestPlace == INVALID_BYTE && byDestPos == INVALID_BYTE)
		inv = ch->GetPlayerItemContainer()->GetEmptyInventory();
	else
		inv = std::make_pair(byDestPlace, byDestPos);

	if (inv.first != INVALID_BYTE && inv.second != INVALID_BYTE)
	{
		ch->GetPlayerItemContainer()->AddReservedInventory(inv.first, inv.second);

		CNtlPacket packet(sizeof(sGQ_ITEM_CREATE_REQ));
		sGQ_ITEM_CREATE_REQ * res = (sGQ_ITEM_CREATE_REQ *)packet.GetPacketData();
		res->wOpCode = GQ_ITEM_CREATE_REQ;
		res->charId = ch->GetCharID();
		res->handle = ch->GetID();
		res->sItem.charId = ch->GetCharID();
		res->sItem.itemNo = itemtbl->tblidx;
		res->sItem.byStackcount = byCount;
		res->sItem.byPlace = inv.first;
		res->sItem.byPosition = inv.second;
		res->sItem.byCurrentDurability = itemtbl->byDurability;
		res->sItem.byRank = itemtbl->byRank;
		res->sItem.bNeedToIdentify = false;
		res->sItem.byRestrictState = GetDefaultRestrictState(itemtbl->byRestrictType, itemtbl->byItem_Type, true);
		CItem::GenerateOptionSet(bEnchantAble, itemtbl, &res->sItem);
		
		if (itemtbl->byDurationType == eDURATIONTYPE_FLATSUM)
		{
			res->sItem.byDurationType = eDURATIONTYPE_FLATSUM;
			res->sItem.nUseStartTime = time(0);
			res->sItem.nUseEndTime = res->sItem.nUseStartTime + itemtbl->dwUseDurationMax;
		}
		
		packet.SetPacketLen(sizeof(sGQ_ITEM_CREATE_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		return true;
	}

	return false;
}

bool CItemManager::CreateItem(CPlayer* ch, CItemDrop* pDrop)
{
	TBLIDX itemTblidx = (pDrop->GetTblidx() == DBO_BLANK_DRAGON_BALL_IDX) ? ch->GetMissingDragonball() : pDrop->GetTblidx();

	sITEM_TBLDAT* itemtbl = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(itemTblidx);

	CGameServer* app = (CGameServer*)g_pApp;

	BYTE byRestrictState = ITEM_RESTRICT_STATE_TYPE_NONE;

	if (pDrop->GetCurWorld()) //tmq drop never bound by default
	{
		if (pDrop->GetCurWorld()->GetRuleType() == GAMERULE_TIMEQUEST) // time quest
		{
			byRestrictState = ITEM_RESTRICT_STATE_TYPE_NONE;
		}
		else if (pDrop->GetCurWorld()->GetRuleType() == GAMERULE_HUNT && itemtbl->byRestrictType != ITEM_RESTRICT_TYPE_NONE && itemtbl->byEquip_Type == EQUIP_TYPE_ACCESSORY) // UD
		{
			byRestrictState = ITEM_RESTRICT_STATE_TYPE_LIMIT;
		}
		else byRestrictState = GetDefaultRestrictState(itemtbl->byRestrictType, itemtbl->byItem_Type, true);
	}
	else byRestrictState = GetDefaultRestrictState(itemtbl->byRestrictType, itemtbl->byItem_Type, true);

	//check if same tblidx already exist && can stack
	if (itemtbl->byMax_Stack > 1 && !pDrop->NeedToIdentify())
	{
		CItem* itemcheck = ch->GetPlayerItemContainer()->CheckStackItem(itemTblidx, 1, itemtbl->byMax_Stack, byRestrictState);
		if (itemcheck)
		{
			itemcheck->SetCount(itemcheck->GetCount() + 1, true, true);
			return true;
		}
	}

	std::pair<BYTE, BYTE> inv = ch->GetPlayerItemContainer()->GetEmptyInventory();
	if (inv.first != INVALID_BYTE && inv.second != INVALID_BYTE)
	{
		ch->GetPlayerItemContainer()->AddReservedInventory(inv.first, inv.second);

		CNtlPacket packet(sizeof(sGQ_ITEM_CREATE_REQ));
		sGQ_ITEM_CREATE_REQ * res = (sGQ_ITEM_CREATE_REQ *)packet.GetPacketData();
		res->wOpCode = GQ_ITEM_CREATE_REQ;
		res->charId = ch->GetCharID();
		res->handle = ch->GetID();

		res->sItem.Init();
		res->sItem.charId = ch->GetCharID();
		res->sItem.itemNo = itemtbl->tblidx;
		res->sItem.byStackcount = 1;
		res->sItem.byPlace = inv.first;
		res->sItem.byPosition = inv.second;
		res->sItem.byCurrentDurability = itemtbl->byDurability;
		res->sItem.byRank = pDrop->GetRank();
		res->sItem.byRestrictState = byRestrictState;
		res->sItem.bNeedToIdentify = pDrop->NeedToIdentify();
		res->sItem.byBattleAttribute = pDrop->GetBattleAttribute();
		res->sItem.byGrade = pDrop->GetGrade();
		memcpy(&res->sItem.sOptionSet, pDrop->GetOptionSet(), sizeof(sITEM_OPTION_SET));

		if (itemtbl->byDurationType == eDURATIONTYPE_FLATSUM)
		{
			res->sItem.byDurationType = eDURATIONTYPE_FLATSUM;
			res->sItem.nUseStartTime = time(0);
			res->sItem.nUseEndTime = res->sItem.nUseStartTime + itemtbl->dwUseDurationMax;
		}

		packet.SetPacketLen(sizeof(sGQ_ITEM_CREATE_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);


		//send msg to party members
		if (ch->GetParty() && ch->GetParty()->GetPartyMemberCount() > 1)
		{
			CNtlPacket packet2(sizeof(sGU_PARTY_MEMBER_GAINED_ITEM_NFY));
			sGU_PARTY_MEMBER_GAINED_ITEM_NFY * res2 = (sGU_PARTY_MEMBER_GAINED_ITEM_NFY *)packet2.GetPacketData();
			res2->wOpCode = GU_PARTY_MEMBER_GAINED_ITEM_NFY;
			res2->hMember = ch->GetID();
			res2->itemTblidx = (pDrop->NeedToIdentify() == false) ? itemtbl->tblidx : INVALID_TBLIDX;
			packet2.SetPacketLen(sizeof(sGU_PARTY_MEMBER_GAINED_ITEM_NFY));
			ch->GetParty()->SendMsgToPartyExceptOne(&packet2, ch->GetID());
		}

		return true;
	}

	return false;
}

bool CItemManager::CreateQuestRewardItem(CPlayer * ch, TBLIDX uiIdx, BYTE byCount)
{
	sITEM_TBLDAT* itemtbl = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(uiIdx);
	if (itemtbl == NULL)
	{
		ERR_LOG(LOG_USER, "User %u can not create item tblidx %u because it does not exist. byCount = %u", ch->GetCharID(), byCount);
		return false;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	//check if same tblidx already exist && can stack
	if (itemtbl->byMax_Stack > 1)
	{
		CItem* itemcheck = ch->GetPlayerItemContainer()->CheckStackItem(uiIdx, byCount, itemtbl->byMax_Stack, GetDefaultRestrictState(itemtbl->byRestrictType, itemtbl->byItem_Type, true));
		if (itemcheck)
		{
			itemcheck->SetCount(itemcheck->GetCount() + byCount, true, true);
			return true;
		}
	}

	std::pair<BYTE, BYTE> inv = ch->GetPlayerItemContainer()->GetEmptyInventory();
	if (inv.first != INVALID_BYTE && inv.second != INVALID_BYTE)
	{
		ch->GetPlayerItemContainer()->AddReservedInventory(inv.first, inv.second);

		CNtlPacket packet(sizeof(sGQ_ITEM_CREATE_REQ));
		sGQ_ITEM_CREATE_REQ * res = (sGQ_ITEM_CREATE_REQ *)packet.GetPacketData();
		res->wOpCode = GQ_ITEM_CREATE_REQ;
		res->charId = ch->GetCharID();
		res->handle = ch->GetID();
		res->sItem.charId = ch->GetCharID();
		res->sItem.itemNo = itemtbl->tblidx;
		res->sItem.byStackcount = byCount;
		res->sItem.byPlace = inv.first;
		res->sItem.byPosition = inv.second;
		res->sItem.byCurrentDurability = itemtbl->byDurability;
		res->sItem.byRank = itemtbl->byRank;
		res->sItem.bNeedToIdentify = false;
		res->sItem.byRestrictState = GetDefaultRestrictState(itemtbl->byRestrictType, itemtbl->byItem_Type, true);
		CItem::GenerateOptionSet(false, itemtbl, &res->sItem);

		if (itemtbl->byDurationType == eDURATIONTYPE_FLATSUM)
		{
			res->sItem.byDurationType = eDURATIONTYPE_FLATSUM;
			res->sItem.nUseStartTime = time(0);
			res->sItem.nUseEndTime = res->sItem.nUseStartTime + itemtbl->dwUseDurationMax;
		}

		packet.SetPacketLen(sizeof(sGQ_ITEM_CREATE_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		return true;
	}

	return false;
}

bool CItemManager::CreateShenronRewardItem(CPlayer * ch, TBLIDX uiIdx, BYTE byCount)
{
	sITEM_TBLDAT* itemtbl = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(uiIdx);
	if (itemtbl == NULL)
	{
		ERR_LOG(LOG_USER, "User %u can not create item tblidx %u because it does not exist. byCount = %u", ch->GetCharID(), byCount);
		return false;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	//check if same tblidx already exist && can stack
	if (itemtbl->byMax_Stack > 1)
	{
		CItem* itemcheck = ch->GetPlayerItemContainer()->CheckStackItem(uiIdx, byCount, itemtbl->byMax_Stack, GetDefaultRestrictState(itemtbl->byRestrictType, itemtbl->byItem_Type, true));
		if (itemcheck)
		{
			itemcheck->SetCount(itemcheck->GetCount() + byCount, true, true);
			return true;
		}
	}

	std::pair<BYTE, BYTE> inv = ch->GetPlayerItemContainer()->GetEmptyInventory();

	if (inv.first != INVALID_BYTE && inv.second != INVALID_BYTE)
	{
		ch->GetPlayerItemContainer()->AddReservedInventory(inv.first, inv.second);

		BYTE byItemTypeGroup = GetItemTypeGroup(itemtbl->byItem_Type);

		CNtlPacket packet(sizeof(sGQ_ITEM_CREATE_REQ));
		sGQ_ITEM_CREATE_REQ * res = (sGQ_ITEM_CREATE_REQ *)packet.GetPacketData();
		res->wOpCode = GQ_ITEM_CREATE_REQ;
		res->charId = ch->GetCharID();
		res->handle = ch->GetID();
		res->sItem.charId = ch->GetCharID();
		res->sItem.itemNo = itemtbl->tblidx;
		res->sItem.byStackcount = byCount;
		res->sItem.byPlace = inv.first;
		res->sItem.byPosition = inv.second;
		res->sItem.byCurrentDurability = itemtbl->byDurability;
		res->sItem.byRank = itemtbl->byRank;
		res->sItem.bNeedToIdentify = false;
		res->sItem.byRestrictState = GetDefaultRestrictState(itemtbl->byRestrictType, itemtbl->byItem_Type, true);
		
		CItem::GenerateOptionSet(false, itemtbl, &res->sItem);

		if (itemtbl->byDurationType == eDURATIONTYPE_FLATSUM)
		{
			res->sItem.byDurationType = eDURATIONTYPE_FLATSUM;
			res->sItem.nUseStartTime = time(0);
			res->sItem.nUseEndTime = res->sItem.nUseStartTime + itemtbl->dwUseDurationMax;
		}

		packet.SetPacketLen(sizeof(sGQ_ITEM_CREATE_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		return true;
	}

	return false;
}


CItem* CItemManager::Find(HOBJECT hHandle)
{
	std::map<HOBJECT, CItem*>::iterator it = m_map_pkItemByID.find(hHandle);

	if (m_map_pkItemByID.end() == it)
		return NULL;

	return it->second;
}

CItemDrop* CItemManager::FindDrop(HOBJECT hHandle)
{
	std::map<HOBJECT, CItemDrop*>::iterator it = m_map_pkItemDrop.find(hHandle);

	if (m_map_pkItemDrop.end() == it)
		return NULL;

	return it->second;
}

CItem* CItemManager::SwitchOwner(CPlayer* owner, CPlayer* receiver, HOBJECT hItem)
{
	CItem* item = Find(hItem);
	if (item)
	{
		if (owner->GetPlayerItemContainer()->GetItem(item->GetID())) // check if owner has item
		{
			std::pair<BYTE, BYTE> inv = receiver->GetPlayerItemContainer()->GetEmptyInventory();

			item->SetPlace(inv.first);
			item->SetPos(inv.second);
			item->AddToCharacter(receiver);

			return item;
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "Couldnt find item. owner %u item handle %u. Possible hacker", owner->GetCharID(), hItem);
			return NULL;
		}
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "Couldnt find item. receiver %u item handle %u. Possible hacker", receiver->GetCharID(), hItem);
		return NULL;
	}
}
