#include "stdafx.h"
#include "QueryServerSession.h"
#include "GameServer.h"
#include "NtlPacketQG.h"
#include "ItemManager.h"
#include "BudokaiManager.h"
#include "item.h"
#include "NtlPacketGU.h"
#include "NtlPacketGT.h"
#include "NtlPacketGQ.h"
#include "CPlayer.h"
#include "NtlResultCode.h"
#include "TableContainerManager.h"
#include "NewbieTable.h"
#include "HLSItemTable.h"
#include "UseItemTable.h"
#include "DynamicFieldSystemEvent.h"
#include "DragonballScramble.h"


void CQueryServerSession::RecvCreateItemRes(CNtlPacket* pPacket)
{
	sQG_ITEM_CREATE_RES * req = (sQG_ITEM_CREATE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return;

	if (pOwner->GetCharID() != req->charID)
		return;


	pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sItem.byPlace, req->sItem.byPosition);

	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pItemManager->CreateItem(pOwner, &req->sItem);
	}
}


void CQueryServerSession::RecvItemMoveRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_ITEM_MOVE_RES * req = (sQG_ITEM_MOVE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		//source item
		CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->hSrcItem);
		if (pSrcItem)
		{
			if (IsBagContainer(req->byDstPlace)) //check if moving bag to bag slot
			{
				pOwner->GetPlayerItemContainer()->InsertActiveBag(req->byDstPos, pSrcItem); //add bag to dest bag slot
			}

			pOwner->GetPlayerItemContainer()->MoveItem(pSrcItem, req->bySrcPlace, req->byDstPlace, req->bySrcPos, req->byDstPos);

			//dest item
			if (CItem* pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->hDstItem))
			{
				if (IsBagContainer(req->bySrcPlace)) //check if switch bag
				{
					pOwner->GetPlayerItemContainer()->InsertActiveBag(req->bySrcPos, pDestItem);
				}

				pOwner->GetPlayerItemContainer()->MoveItem(pDestItem, req->byDstPlace, req->bySrcPlace, req->byDstPos, req->bySrcPos);

				//set place/pos for dest item
				pDestItem->SetPlace(req->bySrcPlace);
				pDestItem->SetPos(req->bySrcPos);
				pDestItem->SetLocked(false);
			}
			else
			{
				//remove reserved place/pos
				pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);

				if (IsBagContainer(req->bySrcPlace)) //if not switch bag then remove bag from old pos
				{
					pOwner->GetPlayerItemContainer()->RemoveActiveBag(req->bySrcPos);
				}
			}

			//set place/post for src item. This has to be last, otherwise the dest item cant be found
			pSrcItem->SetPlace(req->byDstPlace);
			pSrcItem->SetPos(req->byDstPos);
			pSrcItem->SetLocked(false);

			if (req->byDstPlace == CONTAINER_TYPE_EQUIP || req->bySrcPlace == CONTAINER_TYPE_EQUIP)
				pOwner->GetCharAtt()->CalculateAll();
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "ERROR ITEM NOT FOUND !!! Player %u Place %u Pos %u HOBJECT %u", req->charID, req->bySrcPlace, req->bySrcPos, req->hSrcItem);
			req->wResultCode = GAME_ITEM_NOT_FOUND;
		}
	}
	else
	{
		if (CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->hSrcItem))
		{
			pSrcItem->SetLocked(false);
		}

		if (CItem* pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->hDstItem))
		{
			pDestItem->SetLocked(false);
		}
		else
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
	}


	CNtlPacket packet(sizeof(sGU_ITEM_MOVE_RES));
	sGU_ITEM_MOVE_RES * res = (sGU_ITEM_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = GU_ITEM_MOVE_RES;
	res->wResultCode = req->wResultCode;
	res->byDestPlace = req->byDstPlace;
	res->byDestPos = req->byDstPos;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->hDestItem = req->hDstItem;
	res->hSrcItem = req->hSrcItem;
	packet.SetPacketLen(sizeof(sGU_ITEM_MOVE_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvItemMoveStackRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_ITEM_MOVE_STACK_RES * req = (sQG_ITEM_MOVE_STACK_RES*)pPacket->GetPacketData();
	
	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetID() != req->handle)
		return;

	HOBJECT hDestItem = req->hDstItem;

	CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->hSrcItem);
	CItem* pDestItem = NULL;

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (hDestItem == INVALID_HOBJECT) //check if new item has been created (UNSTACK)
		{
			pDestItem = new CItem;
			pDestItem->SetTbldat(pSrcItem->GetTbldat());
			pDestItem->SetID(g_pObjectManager->CreateUID());
			pDestItem->SetItemData(&pSrcItem->GetItemData());

			pDestItem->SetStackCount(req->byStackCount2);
			pDestItem->SetPlace(req->byDstPlace);
			pDestItem->SetPos(req->byDstPos);
			pDestItem->SetItemID(req->splitItemId);

			pDestItem->AddToCharacter(pOwner);

			g_pItemManager->AddItem(pDestItem);

			hDestItem = pDestItem->GetID();

			//remove reserved
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
		}
		else //no new item created (STACK) 
		{
			//set stack count from dest item
			pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->hDstItem);
			if(pDestItem)
				pDestItem->SetStackCount(req->byStackCount2);
		}

		if (req->byStackCount1 > 0)
		{
			pSrcItem->SetStackCount(req->byStackCount1);
		}
		else //if source stack count is 0 then delete it
		{
			pOwner->GetPlayerItemContainer()->RemoveItem(req->bySrcPlace, req->hSrcItem);
			g_pItemManager->DestroyItem(pSrcItem);
		}
	}
	else
	{
		if (hDestItem == INVALID_HOBJECT)
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
	}


	if (pSrcItem)
		pSrcItem->SetLocked(false);
	if (pDestItem)
		pDestItem->SetLocked(false);

	CNtlPacket packet(sizeof(sGU_ITEM_MOVE_STACK_RES));
	sGU_ITEM_MOVE_STACK_RES * res = (sGU_ITEM_MOVE_STACK_RES *)packet.GetPacketData();
	res->wOpCode = GU_ITEM_MOVE_STACK_RES;
	res->wResultCode = req->wResultCode;
	res->hSrcItem = req->hSrcItem;
	res->hDestItem = hDestItem;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byDestPlace = req->byDstPlace;
	res->byDestPos = req->byDstPos;
	res->byStackCount1 = req->byStackCount1;
	res->byStackCount2 = req->byStackCount2;
	packet.SetPacketLen(sizeof(sGU_ITEM_MOVE_STACK_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvItemAutoEquipRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_ITEM_AUTO_EQUIP_RES * req = (sQG_ITEM_AUTO_EQUIP_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;


	pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sItem.byDestPlace, req->sItem.byDestPos);

	if (req->wResultCode == GAME_SUCCESS)
	{
		CItem* pItem = g_pItemManager->CreateItem(pOwner, &req->sItem.sEquipItem);

		pOwner->EquipItem(pItem, req->sItem.byDestPos);
	}
}


void CQueryServerSession::RecvLoadBankDataRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_LOAD_PC_BANK_DATA_RES * req = (sQG_LOAD_PC_BANK_DATA_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (req->charID != pOwner->GetCharID())
		return;

	pOwner->SetBankLoaded(true);

	CNtlPacket packet(sizeof(sGU_BANK_LOAD_RES));
	sGU_BANK_LOAD_RES * res = (sGU_BANK_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = GU_BANK_LOAD_RES;
	res->wResultCode = req->wResultCode;
	res->handle = req->npchandle;
	packet.SetPacketLen(sizeof(sGU_BANK_LOAD_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvLoadBankData(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_LOAD_PC_BANK_DATA * req = (sQG_LOAD_PC_BANK_DATA*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	CNtlPacket packet(sizeof(sGU_BANK_ITEM_INFO));
	sGU_BANK_ITEM_INFO * res = (sGU_BANK_ITEM_INFO *)packet.GetPacketData();
	res->wOpCode = GU_BANK_ITEM_INFO;

	for(int i = 0; i < req->byItemCount; i++)
	{
		res->aBankProfile[i].handle = g_pObjectManager->CreateUID();
		res->aBankProfile[i].tblidx = req->asItemData[i].itemNo;
		res->aBankProfile[i].byPlace = req->asItemData[i].byPlace;
		res->aBankProfile[i].byPos = req->asItemData[i].byPosition;
		res->aBankProfile[i].byStackcount = req->asItemData[i].byStackcount;
		res->aBankProfile[i].byRank = req->asItemData[i].byRank;
		res->aBankProfile[i].byCurDur = req->asItemData[i].byCurrentDurability;
		res->aBankProfile[i].byGrade = req->asItemData[i].byGrade;
		res->aBankProfile[i].bNeedToIdentify = req->asItemData[i].bNeedToIdentify;
		res->aBankProfile[i].byBattleAttribute = req->asItemData[i].byBattleAttribute;
		wcscpy_s(res->aBankProfile[i].awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, req->asItemData[i].awchMaker);
		res->aBankProfile[i].sOptionSet = req->asItemData[i].sOptionSet;
		res->aBankProfile[i].nUseStartTime = req->asItemData[i].nUseStartTime;
		res->aBankProfile[i].nUseEndTime = req->asItemData[i].nUseEndTime;
		res->aBankProfile[i].byRestrictState = req->asItemData[i].byRestrictState;
		res->aBankProfile[i].byDurationType = req->asItemData[i].byDurationType;

		//printf("%I64u, %u,byPlace %u, byPosition %u\n", req->asItemData[i].itemId, res->aBankProfile[i].handle, req->asItemData[i].byPlace, req->asItemData[i].byPosition);

		if (g_pItemManager->CreateFromDB(res->aBankProfile[i].handle, req->asItemData[i], pOwner) == NULL)
		{
			ERR_LOG(LOG_USER, "cannot create item by tblidx %u (id %I64u)", req->asItemData[i].itemNo, req->asItemData[i].itemId);
			g_pObjectManager->DeleteUID(res->aBankProfile[i].handle);
		}
	}

	//printf("req->byCurPacketCount %u, byItemCount %u\n", req->byCurPacketCount, req->byItemCount);
	res->byBeginCount = req->byCurPacketCount;
	res->byItemCount = req->byItemCount;
	packet.SetPacketLen(sizeof(sGU_BANK_ITEM_INFO));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvBankMoveRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_BANK_MOVE_RES * req = (sQG_BANK_MOVE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		//source item
		CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->bySrcPlace, req->bySrcPos);
		if (pSrcItem)
		{
			if (IsBagContainer(req->byDstPlace)) //check if moving bag to bag slot
			{
				pOwner->GetPlayerItemContainer()->InsertActiveBag(req->byDstPos, pSrcItem); //add bag to dest bag slot
			}

			pOwner->GetPlayerItemContainer()->MoveItem(pSrcItem, req->bySrcPlace, req->byDstPlace, req->bySrcPos, req->byDstPos);

			//dest item
			if (CItem* pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->byDstPlace, req->byDstPos))
			{
				if (IsBagContainer(req->bySrcPlace)) //check if switch bag
				{
					pOwner->GetPlayerItemContainer()->InsertActiveBag(req->bySrcPos, pDestItem);
				}

				pOwner->GetPlayerItemContainer()->MoveItem(pDestItem, req->byDstPlace, req->bySrcPlace, req->byDstPos, req->bySrcPos);

				//set place/pos for dest item
				pDestItem->SetPlace(req->bySrcPlace);
				pDestItem->SetPos(req->bySrcPos);
				pDestItem->SetLocked(false);
			}
			else
			{
				//remove reserved place/pos
				pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);

				if (IsBagContainer(req->bySrcPlace)) //if not switch bag then remove bag from old pos
				{
					pOwner->GetPlayerItemContainer()->RemoveActiveBag(req->bySrcPos);
				}
			}

			//set place/post for src item. This has to be last, otherwise the dest item cant be found
			pSrcItem->SetPlace(req->byDstPlace);
			pSrcItem->SetPos(req->byDstPos);
			pSrcItem->SetLocked(false);

			if (req->byDstPlace == CONTAINER_TYPE_EQUIP || req->bySrcPlace == CONTAINER_TYPE_EQUIP)
				pOwner->GetCharAtt()->CalculateAll();
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "ERROR ITEM NOT FOUND !!! Player %u Place %u Pos %u HOBJECT %u", req->charID, req->bySrcPlace, req->bySrcPos, req->hSrcItem);
			req->wResultCode = GAME_ITEM_NOT_FOUND;
		}
	}
	else
	{
		if (CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->bySrcPlace, req->bySrcPos))
		{
			pSrcItem->SetLocked(false);
		}

		if (CItem* pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->byDstPlace, req->byDstPos))
		{
			pDestItem->SetLocked(false);
		}
		else
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
	}

	CNtlPacket packet(sizeof(sGU_BANK_MOVE_RES));
	sGU_BANK_MOVE_RES * res = (sGU_BANK_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = GU_BANK_MOVE_RES;
	res->handle = req->hNpcHandle;
	res->wResultCode = req->wResultCode;
	res->byDestPlace = req->byDstPlace;
	res->byDestPos = req->byDstPos;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->hDstItem = req->hDstItem;
	res->hSrcItem = req->hSrcItem;
	packet.SetPacketLen(sizeof(sGU_BANK_MOVE_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvBankMoveStackRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_BANK_MOVE_STACK_RES * req = (sQG_BANK_MOVE_STACK_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	HOBJECT hDestItem = req->hDstItem;

	CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->bySrcPlace, req->bySrcPos);
	CItem* pDestItem = NULL;

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (hDestItem == INVALID_HOBJECT) //check if new item has been created (UNSTACK)
		{
			pDestItem = new CItem;
			pDestItem->SetTbldat(pSrcItem->GetTbldat());
			pDestItem->SetID(g_pObjectManager->CreateUID());
			pDestItem->SetItemData(&pSrcItem->GetItemData());

			pDestItem->SetStackCount(req->byStackCount2);
			pDestItem->SetPlace(req->byDstPlace);
			pDestItem->SetPos(req->byDstPos);
			pDestItem->SetItemID(req->splitItemId);

			pDestItem->AddToCharacter(pOwner);

			g_pItemManager->AddItem(pDestItem);

			hDestItem = pDestItem->GetID();

			//remove reserved
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
		}
		else //no new item created (STACK) 
		{
			//set stack count from dest item
			pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->byDstPlace, req->byDstPos);

			pDestItem->SetStackCount(req->byStackCount2);
		}

		if (req->byStackCount1 > 0)
		{
			pSrcItem->SetStackCount(req->byStackCount1);
		}
		else //if source stack count is 0 then delete it
		{
			pOwner->GetPlayerItemContainer()->RemoveItem(req->bySrcPlace, req->hSrcItem);
			g_pItemManager->DestroyItem(pSrcItem);
		}
	}
	else
	{
		if (hDestItem == INVALID_HOBJECT)
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
	}

	if (pSrcItem)
		pSrcItem->SetLocked(false);
	if (pDestItem)
		pDestItem->SetLocked(false);

	CNtlPacket packet(sizeof(sGU_BANK_MOVE_STACK_RES));
	sGU_BANK_MOVE_STACK_RES * res = (sGU_BANK_MOVE_STACK_RES *)packet.GetPacketData();
	res->wOpCode = GU_BANK_MOVE_STACK_RES;
	res->handle = req->hNpcHandle;
	res->hSrcItem = req->hSrcItem;
	res->hDestItem = hDestItem;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byDestPlace = req->byDstPlace;
	res->byDestPos = req->byDstPos;
	res->byStackCount1 = req->byStackCount1;
	res->byStackCount2 = req->byStackCount2;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_BANK_MOVE_STACK_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvBankBuyRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_BANK_BUY_RES * req = (sQG_BANK_BUY_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byPlace, req->byPosition);

	CNtlPacket packet(sizeof(sGU_BANK_BUY_RES));
	sGU_BANK_BUY_RES * res = (sGU_BANK_BUY_RES *)packet.GetPacketData();
	res->wOpCode = GU_BANK_BUY_RES;
	res->hNpchandle = req->npchandle;
	res->wResultCode = req->wResultCode;

	if (req->wResultCode == GAME_SUCCESS)
	{
		pOwner->UpdateZeni(ZENNY_CHANGE_TYPE_BANK_BUY, req->dwZenny, false, false);

		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->itemNo);

		CItem* item = new CItem;
		item->SetTbldat(pItemTbldat);
		item->GenerateOptionSet(false);
		item->SetID(g_pObjectManager->CreateUID());

		CNtlPacket packet2(sizeof(sGU_CREATE_BANK_NFY));
		sGU_CREATE_BANK_NFY * res2 = (sGU_CREATE_BANK_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_CREATE_BANK_NFY;
		res2->bIsNew = true;
		res2->handle = item->GetID();
		res2->sItemData.byDurationType = req->byDurationType;
		res2->sItemData.byPlace = req->byPlace;
		res2->sItemData.byPosition = req->byPosition;
		res2->sItemData.byRank = req->byRank;
		res2->sItemData.byStackcount = 1;
		res2->sItemData.charId = req->charId;
		res2->sItemData.itemId = req->itemID;
		res2->sItemData.itemNo = req->itemNo;
		res2->sItemData.nUseEndTime = req->nUseEndTime;
		res2->sItemData.nUseStartTime = req->nUseStartTime;
		res2->sItemData.sOptionSet.Init();
		packet2.SetPacketLen(sizeof(sGU_CREATE_BANK_NFY));
		pOwner->SendPacket(&packet2);

		item->SetItemData(&res2->sItemData);
		item->AddToCharacter(pOwner);

		g_pItemManager->AddItem(item);

		res->hItemhandle = item->GetID();
		memcpy(&res->sData, &item->GetItemData(), sizeof(sITEM_DATA));
	}
	
	packet.SetPacketLen(sizeof(sGU_BANK_BUY_RES));
	pOwner->SendPacket(&packet);
}

void CQueryServerSession::RecvBankAddWithCommandRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_BANK_ADD_WITH_COMMAND_RES * req = (sQG_BANK_ADD_WITH_COMMAND_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		pOwner->GetClientSession()->Disconnect(false);
	}
	else
	{
		pOwner->GetClientSession()->SendResultcode(req->wResultCode);
	}
}

void CQueryServerSession::RecvShopBuyRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_SHOP_BUY_RES * req = (sQG_SHOP_BUY_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charId);
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId || pOwner->GetID() != req->handle)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		for (BYTE i = 0; i < req->byBuyCount; i++)
		{
			sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->sInven[i].tblItem);

			CItem* pItem = new CItem;
			pItem->SetTbldat(pItemTbldat);
			pItem->SetID(g_pObjectManager->CreateUID());
			pItem->SetItemData(&req->sInven[i], req->itemID[i]);
			pItem->AddToCharacter(pOwner);

			g_pItemManager->AddItem(pItem);

			CNtlPacket packet3(sizeof(sGU_ITEM_CREATE));
			sGU_ITEM_CREATE * res3 = (sGU_ITEM_CREATE *)packet3.GetPacketData();
			res3->wOpCode = GU_ITEM_CREATE;
			res3->bIsNew = true;
			res3->handle = pItem->GetID();
			memcpy(&res3->sItemData, &pItem->GetItemData(), sizeof(sITEM_DATA));
			packet3.SetPacketLen(sizeof(sGU_ITEM_CREATE));
			pOwner->SendPacket(&packet3);

			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sInven[i].byPlace, req->sInven[i].byPos);
		}
	}
	else
	{
		for (BYTE i = 0; i < req->byBuyCount; i++)
		{
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sInven[i].byPlace, req->sInven[i].byPos);
		}
	}

	CNtlPacket packet(sizeof(sGU_SHOP_BUY_RES));
	sGU_SHOP_BUY_RES * res = (sGU_SHOP_BUY_RES *)packet.GetPacketData();
	res->wOpCode = GU_SHOP_BUY_RES;
	res->wResultCode = req->wResultCode;
	res->handle = req->hNpchandle;
	packet.SetPacketLen(sizeof(sGU_SHOP_BUY_RES));
	pOwner->SendPacket(&packet);
}

void CQueryServerSession::RecvTradeRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_TRADE_RES * req = (sQG_TRADE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->hTarget); //get target player who receive item
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->dstcharID)
		return;

	for (BYTE i = 0; i < req->byRecvCount; i++)
	{
		if (req->asRecvData[i].itemNewSerial != INVALID_ITEMID)
		{
			CItem* pItem = g_pItemManager->Find(req->asRecvData[i].hItem);
			if (pItem)
			{
				pItem->SetTrading(false);
				pItem->SetItemID(req->asRecvData[i].itemNewSerial);
				pItem->AddToCharacter(pOwner);

				ERR_LOG(LOG_USER, "ITEM-TRADE: Owner: %u, ItemID %I64u, Tblidx %u, Target %u", req->charID, req->asRecvData[i].itemNewSerial, pItem->GetTblidx(), req->dstcharID);

				CNtlPacket packet2(sizeof(sGU_ITEM_CREATE));
				sGU_ITEM_CREATE * res2 = (sGU_ITEM_CREATE *)packet2.GetPacketData();
				res2->wOpCode = GU_ITEM_CREATE;
				res2->bIsNew = true;
				res2->handle = pItem->GetID();
				memcpy(&res2->sItemData, &pItem->GetItemData(), sizeof(sITEM_DATA));
				packet2.SetPacketLen(sizeof(sGU_ITEM_CREATE));
				pOwner->SendPacket(&packet2);
			}
		}

		pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->asRecvData[i].byDstPlace, req->asRecvData[i].byDstPos);
	}
}


void CQueryServerSession::RecvMailStartRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_MAIL_START_RES * req = (sQG_MAIL_START_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	CNtlPacket packet2(sizeof(sGU_MAIL_START_RES));
	sGU_MAIL_START_RES * res2 = (sGU_MAIL_START_RES *)packet2.GetPacketData();
	res2->wOpCode = GU_MAIL_START_RES;
	res2->hObject = req->hObject;
	res2->bIsAway = req->bIsAway;
	res2->wResultCode = req->wResultCode;
	packet2.SetPacketLen(sizeof(sGU_MAIL_START_RES));
	app->Send(pOwner->GetClientSessionID(), &packet2);
}

void CQueryServerSession::RecvMailSendRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_MAIL_SEND_RES * req = (sQG_MAIL_SEND_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (req->wResultCode == GAME_SUCCESS)
	{
		pOwner->UpdateZeni(ZENNY_CHANGE_TYPE_MAIL_SEND, req->dwZenny, false, false);

		if (req->byMailType == eMAIL_TYPE_ITEM || req->byMailType == eMAIL_TYPE_ITEM_ZENNY || req->byMailType == eMAIL_TYPE_ITEM_ZENNY_REQ)
		{
			if (CItem* item = pOwner->GetPlayerItemContainer()->GetItem(req->sItemData.byPlace, req->sItemData.byPos))
			{
				item->RemoveFromCharacter();
				//del item from channel
				g_pItemManager->DestroyItem(item);
			}
			else
			{
				ERR_LOG(LOG_USER, "ERROR Item dont exist anymore after send mail ResultCode %u, Player %u Target %u Zeni %u Item %I64u. This error can appear when sending invalid items to mail when player login", req->wResultCode, req->charID, req->targetCharID, req->dwZenny, req->sItemData.itemID);
			}
		}
	}
	else
	{
		ERR_LOG(LOG_USER, "Send mail != GAME_SUCCESS. ResultCode %u, Player %u Target %u Zeni %u Item %I64u", req->wResultCode, req->charID, req->targetCharID, req->dwZenny, req->sItemData.itemID);
	}


	CNtlPacket packet(sizeof(sGU_MAIL_SEND_RES));
	sGU_MAIL_SEND_RES * res = (sGU_MAIL_SEND_RES *)packet.GetPacketData();
	res->wOpCode = GU_MAIL_SEND_RES;
	res->hObject = req->hObject;
	res->wResultCode = req->wResultCode;
	NTL_SAFE_WCSCPY(res->wszTargetName, req->wszTargetName);
	packet.SetPacketLen(sizeof(sGU_MAIL_SEND_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvMailReturnRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_MAIL_RETURN_RES * req = (sQG_MAIL_RETURN_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (!pOwner)
		return; //dont need to log I guess

	if (req->wResultCode == GAME_SUCCESS)
	{
		pOwner->DeleteMail(req->mailID);
	}
	else
	{
		ERR_LOG(LOG_USER, "Return mail != GAME_SUCCESS. ResultCode %u, Player %u", req->wResultCode, req->charID);
		sMAIL_NEW_PROFILE* mail = pOwner->GetMail(req->mailID);
		if (mail)
		{
			mail->bIsLock = false; //unset temp lock
		}
	}

	CNtlPacket packet(sizeof(sGU_MAIL_RETURN_RES));
	sGU_MAIL_RETURN_RES * res = (sGU_MAIL_RETURN_RES *)packet.GetPacketData();
	res->wOpCode = GU_MAIL_RETURN_RES;
	res->hObject = req->hObject;
	res->mailID = req->mailID;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_MAIL_RETURN_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvMailReloadRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_MAIL_RELOAD_RES * req = (sQG_MAIL_RELOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	//printf("bIsSchedule %i, byTotalCount %u, byManagerCount %u, byNormalCount %u \n", req->bIsSchedule, req->byTotalCount, req->byManagerCount, req->byNormalCount);
	if (req->bIsSchedule)
	{
		CNtlPacket packet2(sizeof(sGU_MAIL_RELOAD_NFY));
		sGU_MAIL_RELOAD_NFY * res2 = (sGU_MAIL_RELOAD_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_MAIL_RELOAD_NFY;
		res2->byMailCount = req->byTotalCount;
		res2->byManagerCount = req->byManagerCount;
		res2->byNormalCount = req->byNormalCount;
		memcpy(res2->aMailID, req->asMailID, sizeof(req->asMailID));
		packet2.SetPacketLen(sizeof(sGU_MAIL_RELOAD_NFY));
		app->Send(pOwner->GetClientSessionID(), &packet2);
	}
	else
	{
		CNtlPacket packet2(sizeof(sGU_MAIL_RELOAD_RES));
		sGU_MAIL_RELOAD_RES * res2 = (sGU_MAIL_RELOAD_RES *)packet2.GetPacketData();
		res2->wOpCode = GU_MAIL_RELOAD_RES;
		res2->hObject = req->hObject;
		res2->wResultCode = req->wResultCode;
		res2->byMailCount = req->byTotalCount;
		res2->byManagerCount = req->byManagerCount;
		res2->byNormalCount = req->byNormalCount;
		memcpy(res2->aMailID, req->asMailID, sizeof(req->asMailID));
		packet2.SetPacketLen(sizeof(sGU_MAIL_RELOAD_RES));
		app->Send(pOwner->GetClientSessionID(), &packet2);
	}
}

void CQueryServerSession::RecvMailLoadRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_MAIL_LOAD_RES * req = (sQG_MAIL_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (pOwner->GetID() != req->handle)
		return;

	CNtlPacket packet2(sizeof(sGU_MAIL_LOAD_RES));
	sGU_MAIL_LOAD_RES * res2 = (sGU_MAIL_LOAD_RES *)packet2.GetPacketData();
	res2->wOpCode = GU_MAIL_LOAD_RES;
	res2->hObject = req->hObject;
	res2->wResultCode = req->wResultCode;
	packet2.SetPacketLen(sizeof(sGU_MAIL_LOAD_RES));
	app->Send(pOwner->GetClientSessionID(), &packet2);
}


void CQueryServerSession::RecvMailItemReceiveRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_MAIL_ITEM_RECEIVE_RES * req = (sQG_MAIL_ITEM_RECEIVE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner)
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		sMAIL_NEW_PROFILE* mail = pOwner->GetMail(req->mailID);
		if (mail)
		{
			if (req->dwZenny > 0)
			{
				if (req->byMailType == eMAIL_TYPE_ITEM_ZENNY_REQ || req->byMailType == eMAIL_TYPE_ZENNY_REQ)
				{
					pOwner->UpdateZeni(ZENNY_CHANGE_TYPE_MAIL_RECV_ATTACH_ZENNY, req->dwZenny, false, false);
				}
				else if (req->byMailType == eMAIL_TYPE_ZENNY || req->byMailType == eMAIL_TYPE_ITEM_ZENNY)
				{
					if (pOwner->CanReceiveZeni(req->dwZenny))
					{
						pOwner->UpdateZeni(ZENNY_CHANGE_TYPE_MAIL_RECV_ATTACH_ZENNY, req->dwZenny, true, false);

						ERR_LOG(LOG_USER, "Player: %u receive %u zeni from mail", pOwner->GetCharID(), req->dwZenny);
					}
				}
			}

			if (req->sItemData.itemId > 0 && req->sItemData.itemId != INVALID_ITEMID) //if received item
			{
				mail->sItemProfile.byPlace = req->sItemData.byPlace;
				mail->sItemProfile.byPos = req->sItemData.byPosition;
				mail->sItemProfile.handle = g_pObjectManager->CreateUID();
				CItem* item = g_pItemManager->CreateFromDB(mail->sItemProfile.handle, req->sItemData, pOwner);

				if (item)
				{
					CNtlPacket packet2(sizeof(sGU_ITEM_CREATE));
					sGU_ITEM_CREATE * res2 = (sGU_ITEM_CREATE *)packet2.GetPacketData();
					res2->bIsNew = true;
					res2->wOpCode = GU_ITEM_CREATE;
					res2->handle = item->GetID();
					memcpy(&res2->sItemData, &item->GetItemData(), sizeof(sITEM_DATA));
					packet2.SetPacketLen(sizeof(sGU_ITEM_CREATE));
					app->Send(pOwner->GetClientSessionID(), &packet2);
				}
				else
					ERR_LOG(LOG_GENERAL, "Receive item/zeni from mail failed. Item is null. Receiver %u Sender %u Result %u ItemIdx %u ItemID %I64u, Mail %u", req->charID, req->fromCharId, req->wResultCode, req->sItemData.itemNo, req->sItemData.itemId, req->mailID);

				pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sItemData.byPlace, req->sItemData.byPosition);
			}
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "Receive item/zeni from mail failed. mail is null. Receiver %u Sender %u Result %u, Mail %u", req->charID, req->fromCharId, req->wResultCode, req->mailID);
		}
	}
	else
		ERR_LOG(LOG_GENERAL, "Receive item/zeni from mail failed. Receiver %u Sender %u Result %u, Mail %u", req->charID, req->fromCharId, req->wResultCode, req->mailID);

	CNtlPacket packet(sizeof(sGU_MAIL_ITEM_RECEIVE_RES));
	sGU_MAIL_ITEM_RECEIVE_RES * res = (sGU_MAIL_ITEM_RECEIVE_RES *)packet.GetPacketData();
	res->wOpCode = GU_MAIL_ITEM_RECEIVE_RES;
	res->hObject = req->hObject;
	res->wResultCode = req->wResultCode;
	res->mailID = req->mailID;
	packet.SetPacketLen(sizeof(sGU_MAIL_ITEM_RECEIVE_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvPortalLoadRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_PORTAL_START_RES * req = (sQG_PORTAL_START_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcPortalLoadRes(req->aPortalID, req->byCount);
}


void CQueryServerSession::RecvGuildBankLoadRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_GUILD_BANK_LOAD_RES * req = (sQG_GUILD_BANK_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;


	if (req->wResultCode == GAME_SUCCESS)
	{
		pOwner->GetPlayerItemContainer()->SetUsingGuildBank(true);
		pOwner->GetPlayerItemContainer()->SetGuildBankZeni(req->dwZenny); //set zeni


		CNtlPacket packetZeni(sizeof(sGU_GUILD_BANK_ZENNY_INFO));
		sGU_GUILD_BANK_ZENNY_INFO * resZeni = (sGU_GUILD_BANK_ZENNY_INFO *)packetZeni.GetPacketData();
		resZeni->wOpCode = GU_GUILD_BANK_ZENNY_INFO;
		resZeni->dwZenny = req->dwZenny;
		packetZeni.SetPacketLen(sizeof(sGU_GUILD_BANK_ZENNY_INFO));
		app->Send(pOwner->GetClientSessionID(), &packetZeni);
	}

	CNtlPacket packet(sizeof(sGU_GUILD_BANK_START_RES));
	sGU_GUILD_BANK_START_RES * res = (sGU_GUILD_BANK_START_RES *)packet.GetPacketData();
	res->wOpCode = GU_GUILD_BANK_START_RES;
	res->wResultCode = req->wResultCode;
	res->handle = req->hNpcHandle;
	packet.SetPacketLen(sizeof(sGU_GUILD_BANK_START_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvGuildBankMoveRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_GUILD_BANK_MOVE_RES * req = (sQG_GUILD_BANK_MOVE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		//source item
		CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->bySrcPlace, req->bySrcPos);
		if (pSrcItem)
		{
			ERR_LOG(LOG_USER, "<GUILD_STORAGE>Player: %u move item %I64u. Guild:%u SrcPlace:%u,SrcPos:%u,DstPlace:%u,DstPos:%u", req->charID, pSrcItem->GetItemID(), pOwner->GetGuildID(), req->bySrcPlace, req->bySrcPos, req->byDstPlace, req->byDstPos);

			if (IsBagContainer(req->byDstPlace)) //check if moving item to bag slot
			{
				pOwner->GetPlayerItemContainer()->InsertActiveBag(req->byDstPos, pSrcItem); //add bag to dest bag slot
			}

			pOwner->GetPlayerItemContainer()->MoveItem(pSrcItem, req->bySrcPlace, req->byDstPlace, req->bySrcPos, req->byDstPos);

			//dest item
			CItem* pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->byDstPlace, req->byDstPos);
			if (pDestItem)
			{
				if (IsBagContainer(req->bySrcPlace)) //check if switch bag
				{
					pOwner->GetPlayerItemContainer()->InsertActiveBag(req->bySrcPos, pDestItem);
				}

				pOwner->GetPlayerItemContainer()->MoveItem(pDestItem, req->byDstPlace, req->bySrcPlace, req->byDstPos, req->bySrcPos);

				//set place/pos for dest item
				pDestItem->SetPlace(req->bySrcPlace);
				pDestItem->SetPos(req->bySrcPos);
				pDestItem->SetLocked(false);
			}
			else
			{
				//remove reserved place/pos
				pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);

				if (IsBagContainer(req->bySrcPlace)) //if no dest item then remove bag from old slot
				{
					pOwner->GetPlayerItemContainer()->RemoveActiveBag(req->bySrcPos);
				}
			}

			//set place/post for src item. This has to be last, otherwise the dest item cant be found
			pSrcItem->SetPlace(req->byDstPlace);
			pSrcItem->SetPos(req->byDstPos);
			pSrcItem->SetLocked(false);

			if (req->byDstPlace == CONTAINER_TYPE_EQUIP || req->bySrcPlace == CONTAINER_TYPE_EQUIP)
				pOwner->GetCharAtt()->CalculateAll();
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "ERROR ITEM NOT FOUND !!! Player %u Place %u Pos %u HOBJECT %u", req->charID, req->bySrcPlace, req->bySrcPos, req->hSrcItem);
			req->wResultCode = GAME_ITEM_NOT_FOUND;
		}
	}
	else
	{
		if (CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->bySrcPlace, req->bySrcPos))
		{
			pSrcItem->SetLocked(false);
		}

		if (CItem* pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->byDstPlace, req->byDstPos))
		{
			pDestItem->SetLocked(false);
		}
		else
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
	}

	CNtlPacket packet(sizeof(sGU_GUILD_BANK_MOVE_RES));
	sGU_GUILD_BANK_MOVE_RES * res = (sGU_GUILD_BANK_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = GU_GUILD_BANK_MOVE_RES;
	res->handle = req->hNpcHandle;
	res->wResultCode = req->wResultCode;
	res->byDestPlace = req->byDstPlace;
	res->byDestPos = req->byDstPos;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->hDstItem = req->hDstItem;
	res->hSrcItem = req->hSrcItem;
	packet.SetPacketLen(sizeof(sGU_GUILD_BANK_MOVE_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvGuildBankMoveStackRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_GUILD_BANK_MOVE_STACK_RES * req = (sQG_GUILD_BANK_MOVE_STACK_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	HOBJECT hDestItem = req->hDstItem;

	CItem* pSrcItem = pOwner->GetPlayerItemContainer()->GetItem(req->bySrcPlace, req->bySrcPos);
	CItem* pDestItem = NULL;

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (hDestItem == INVALID_HOBJECT) //check if new item has been created (UNSTACK)
		{
			pDestItem = new CItem;
			pDestItem->SetTbldat(pSrcItem->GetTbldat());
			pDestItem->SetID(g_pObjectManager->CreateUID());
			pDestItem->SetItemData(&pSrcItem->GetItemData());

			pDestItem->SetStackCount(req->byStackCount2);
			pDestItem->SetPlace(req->byDstPlace);
			pDestItem->SetPos(req->byDstPos);
			pDestItem->SetItemID(req->splitItemId);

			pDestItem->AddToCharacter(pOwner);

			g_pItemManager->AddItem(pDestItem);

			hDestItem = pDestItem->GetID();;

			//remove reserved
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
		}
		else //no new item created (STACK) 
		{
			//set stack count from dest item
			pDestItem = pOwner->GetPlayerItemContainer()->GetItem(req->byDstPlace, req->byDstPos);

			pDestItem->SetStackCount(req->byStackCount2);
		}

		if (req->byStackCount1 > 0)
		{
			pSrcItem->SetStackCount(req->byStackCount1);
		}
		else //if source stack count is 0 then delete it
		{
			pOwner->GetPlayerItemContainer()->RemoveItem(req->bySrcPlace, req->hSrcItem);
			g_pItemManager->DestroyItem(pSrcItem);
		}
	}
	else
	{
		if (hDestItem == INVALID_HOBJECT)
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byDstPlace, req->byDstPos);
	}

	if (pSrcItem)
		pSrcItem->SetLocked(false);
	if (pDestItem)
		pDestItem->SetLocked(false);


	CNtlPacket packet(sizeof(sGU_GUILD_BANK_MOVE_STACK_RES));
	sGU_GUILD_BANK_MOVE_STACK_RES * res = (sGU_GUILD_BANK_MOVE_STACK_RES *)packet.GetPacketData();
	res->wOpCode = GU_GUILD_BANK_MOVE_STACK_RES;
	res->handle = req->hNpcHandle;
	res->hSrcItem = req->hSrcItem;
	res->hDestItem = hDestItem;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byDestPlace = req->byDstPlace;
	res->byDestPos = req->byDstPos;
	res->byStackCount1 = req->byStackCount1;
	res->byStackCount2 = req->byStackCount2;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_GUILD_BANK_MOVE_STACK_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvGuildBankZeniRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_GUILD_BANK_ZENNY_RES * req = (sQG_GUILD_BANK_ZENNY_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charID)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (req->bIsSave)
		{
			pOwner->GetPlayerItemContainer()->SetGuildBankZeni(pOwner->GetPlayerItemContainer()->GetGuildBankZeni() + req->dwZenny); //set zeni
			pOwner->UpdateZeni(ZENNY_CHANGE_TYPE_GUILD_BANK_ADD, req->dwZenny, false, false);

			ERR_LOG(LOG_USER, "<GUILD_STORAGE>Player: %u put %u zeni into guild %u storage", pOwner->GetCharID(), req->dwZenny, pOwner->GetGuildID());
		}
		else
		{
			pOwner->GetPlayerItemContainer()->SetGuildBankZeni(pOwner->GetPlayerItemContainer()->GetGuildBankZeni() - req->dwZenny); //set zeni

			ERR_LOG(LOG_USER, "<GUILD_STORAGE>Player: %u takes %u zeni from guild %u storage", pOwner->GetCharID(), req->dwZenny, pOwner->GetGuildID());

			if (pOwner->CanReceiveZeni(req->dwZenny))
			{
				pOwner->UpdateZeni(ZENNY_CHANGE_TYPE_GUILD_BANK_WITHRAW, req->dwZenny, true, false);
			}
		}
	}


	CNtlPacket packet(sizeof(sGU_GUILD_BANK_ZENNY_RES));
	sGU_GUILD_BANK_ZENNY_RES * res = (sGU_GUILD_BANK_ZENNY_RES *)packet.GetPacketData();
	res->wOpCode = GU_GUILD_BANK_ZENNY_RES;
	res->handle = req->npchandle;
	res->wResultCode = req->wResultCode;
	res->dwZenny = req->dwZenny;
	res->bIsSave = req->bIsSave;
	packet.SetPacketLen(sizeof(sGU_GUILD_BANK_ZENNY_RES));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvGuildBankLoadData(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_GUILD_BANK_LOAD_DATA * req = (sQG_GUILD_BANK_LOAD_DATA*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	CNtlPacket packet(sizeof(sGU_GUILD_BANK_ITEM_INFO));
	sGU_GUILD_BANK_ITEM_INFO * res = (sGU_GUILD_BANK_ITEM_INFO *)packet.GetPacketData();
	res->wOpCode = GU_GUILD_BANK_ITEM_INFO;

	for (int i = 0; i < req->byItemCount; i++)
	{
		res->aItemProfile[i].handle = g_pObjectManager->CreateUID();
		res->aItemProfile[i].tblidx = req->asItemData[i].itemNo;
		res->aItemProfile[i].byPlace = req->asItemData[i].byPlace;
		res->aItemProfile[i].byPos = req->asItemData[i].byPosition;
		res->aItemProfile[i].byStackcount = req->asItemData[i].byStackcount;
		res->aItemProfile[i].byRank = req->asItemData[i].byRank;
		res->aItemProfile[i].byCurDur = req->asItemData[i].byCurrentDurability;
		res->aItemProfile[i].byGrade = req->asItemData[i].byGrade;
		res->aItemProfile[i].bNeedToIdentify = req->asItemData[i].bNeedToIdentify;
		res->aItemProfile[i].byBattleAttribute = req->asItemData[i].byBattleAttribute;
		NTL_SAFE_WCSCPY(res->aItemProfile[i].awchMaker, req->asItemData[i].awchMaker);
		res->aItemProfile[i].sOptionSet = req->asItemData[i].sOptionSet;
		res->aItemProfile[i].nUseStartTime = req->asItemData[i].nUseStartTime;
		res->aItemProfile[i].nUseEndTime = req->asItemData[i].nUseEndTime;
		res->aItemProfile[i].byRestrictState = req->asItemData[i].byRestrictState;
		res->aItemProfile[i].byDurationType = req->asItemData[i].byDurationType;

		if (g_pItemManager->CreateFromDB(res->aItemProfile[i].handle, req->asItemData[i], pOwner) == NULL)
		{
			ERR_LOG(LOG_USER, "cannot create item by tblidx %u (id %I64u)", req->asItemData[i].itemNo, req->asItemData[i].itemId);
			g_pObjectManager->DeleteUID(res->aItemProfile[i].handle);
		}
	}

	res->byPlace = req->byCurPacketCount;
	res->byItemCount = req->byItemCount;
	packet.SetPacketLen(sizeof(sGU_GUILD_BANK_ITEM_INFO));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvBudokaiDataRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_DATA_RES * req = (sQG_BUDOKAI_DATA_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiInitDataNfy(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_INIT_DATA_NFY * req = (sQG_BUDOKAI_INIT_DATA_NFY*)pPacket->GetPacketData();
	
	g_pBudokaiManager->RecvInitInfo(req->wSeasonCount, (BUDOKAITIME)req->nDefaultOpenTime);
}

void CQueryServerSession::RecvBudokaiInitDataRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_INIT_DATA_RES * req = (sQG_BUDOKAI_INIT_DATA_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiUpdateStateRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_UPDATE_STATE_RES * req = (sQG_BUDOKAI_UPDATE_STATE_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiUpdateMatchStateRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_UPDATE_MATCH_STATE_RES * req = (sQG_BUDOKAI_UPDATE_MATCH_STATE_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiUpdateClearRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_UPDATE_CLEAR_RES * req = (sQG_BUDOKAI_UPDATE_CLEAR_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiIndividualAllowRegisterRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_INDIVIDUAL_ALLOW_REGISTER_RES * req = (sQG_BUDOKAI_INDIVIDUAL_ALLOW_REGISTER_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiJoinIndividualRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_JOIN_INDIVIDUAL_RES * req = (sQG_BUDOKAI_JOIN_INDIVIDUAL_RES*)pPacket->GetPacketData();
	
	//insert new into individual
	if(req->wResultCode == GAME_SUCCESS)
		g_pBudokaiManager->JoinIndividual(req->charId, req->fPoint, req->wJoinId, req->bDojoRecommender, req->wszCharName);

	//get player. If exist, then send him result packet
	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	pOwner->SetJoinID(req->wJoinId);

	CNtlPacket packet(sizeof(sGU_BUDOKAI_JOIN_INDIVIDUAL_RES));
	sGU_BUDOKAI_JOIN_INDIVIDUAL_RES * res = (sGU_BUDOKAI_JOIN_INDIVIDUAL_RES *)packet.GetPacketData();
	res->wOpCode = GU_BUDOKAI_JOIN_INDIVIDUAL_RES;
	res->bDojoRecommender = req->bDojoRecommender;
	res->wJoinId = req->wJoinId;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_BUDOKAI_JOIN_INDIVIDUAL_RES));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvBudokaiLeaveIndividualRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_LEAVE_INDIVIDUAL_RES * req = (sQG_BUDOKAI_LEAVE_INDIVIDUAL_RES*)pPacket->GetPacketData();
	
	//leave from individual
	if (req->wResultCode == GAME_SUCCESS)
		g_pBudokaiManager->LeaveIndividual(req->joinId);

	//get player. If exist, then send him result packet
	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	pOwner->SetJoinID(INVALID_JOINID);

	CNtlPacket packet(sizeof(sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES));
	sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES * res = (sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES *)packet.GetPacketData();
	res->wOpCode = GU_BUDOKAI_LEAVE_INDIVIDUAL_RES;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_BUDOKAI_LEAVE_INDIVIDUAL_RES));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvBudokaiIndividualSelectionRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_INDIVIDUAL_SELECTION_RES * req = (sQG_BUDOKAI_INDIVIDUAL_SELECTION_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiIndividualListStart(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_INDIVIDUAL_LIST_START * req = (sQG_BUDOKAI_INDIVIDUAL_LIST_START*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiIndividualListData(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_INDIVIDUAL_LIST_DATA * req = (sQG_BUDOKAI_INDIVIDUAL_LIST_DATA*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiIndividualListEnd(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_INDIVIDUAL_LIST_END * req = (sQG_BUDOKAI_INDIVIDUAL_LIST_END*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentIndividualAddEntryListRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES * req = (sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES*)pPacket->GetPacketData();
	
	g_pBudokaiManager->SetJoinData(req->joinData.joinId, req->joinData.byJoinState, req->joinData.byJoinResult);
}

void CQueryServerSession::RecvBudokaiTournamentIndividualEntryListStart(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_START * req = (sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_START*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentIndividualEntryListData(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_DATA * req = (sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_DATA*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentIndividualEntryListEnd(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_END * req = (sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_END*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentIndividualAddMatchResultRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_MATCH_RESULT_RES * req = (sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_MATCH_RESULT_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentIndividualMatchResultRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_MATCH_RESULT_RES * req = (sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_MATCH_RESULT_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTeamAllowRegisterRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TEAM_ALLOW_REGISTER_RES * req = (sQG_BUDOKAI_TEAM_ALLOW_REGISTER_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiJoinTeamRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_JOIN_TEAM_RES * req = (sQG_BUDOKAI_JOIN_TEAM_RES*)pPacket->GetPacketData();
	
	//insert new into team
	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pBudokaiManager->JoinTeam(req->charId, req->wszTeamName, req->byMemberCount, req->aMembers, req->fPoint, req->wJoinId, req->aTeamInfo);

		for (int i = 0; i < req->byMemberCount; i++)
		{
			CPlayer* pMember = g_pObjectManager->FindByChar(req->aMembers[i]);
			if (pMember)
			{
				pMember->SetJoinID(req->wJoinId);

				if (pMember->GetCharID() == req->charId)
					continue;

				CNtlPacket packetNfy(sizeof(sGU_BUDOKAI_JOIN_TEAM_NFY));
				sGU_BUDOKAI_JOIN_TEAM_NFY * res = (sGU_BUDOKAI_JOIN_TEAM_NFY *)packetNfy.GetPacketData();
				res->wOpCode = GU_BUDOKAI_JOIN_TEAM_NFY;
				res->fTotalRankPoint = req->fPoint;
				res->wJoinId = req->wJoinId;
				NTL_SAFE_WCSCPY(res->wszTeamName, req->wszTeamName);
				res->byMemberCount = req->byMemberCount;
				memcpy(res->aTeamInfo, req->aTeamInfo, sizeof(req->aTeamInfo));
				packetNfy.SetPacketLen(sizeof(sGU_BUDOKAI_JOIN_TEAM_NFY));
				g_pApp->Send(pMember->GetClientSessionID(), &packetNfy);
			}
		}
	}

	//get player. If exist, then send him result packet
	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CNtlPacket packet(sizeof(sGU_BUDOKAI_JOIN_TEAM_RES));
	sGU_BUDOKAI_JOIN_TEAM_RES * res = (sGU_BUDOKAI_JOIN_TEAM_RES *)packet.GetPacketData();
	res->wOpCode = GU_BUDOKAI_JOIN_TEAM_RES;
	res->wResultCode = req->wResultCode;
	res->hTroublePc = req->handle;
	res->fTotalRankPoint = req->fPoint;
	res->wJoinId = req->wJoinId;
	NTL_SAFE_WCSCPY(res->wszTeamName, req->wszTeamName);
	res->byMemberCount = req->byMemberCount;
	memcpy(res->aTeamInfo, req->aTeamInfo, sizeof(req->aTeamInfo));
	packet.SetPacketLen(sizeof(sGU_BUDOKAI_JOIN_TEAM_RES));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvBudokaiLeaveTeamRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_LEAVE_TEAM_RES * req = (sQG_BUDOKAI_LEAVE_TEAM_RES*)pPacket->GetPacketData();
	
	//remove team
	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pBudokaiManager->LeaveTeam(req->wJoinId);

		for (int i = 0; i < req->byMemberCount; i++)
		{
			CPlayer* pMember = g_pObjectManager->FindByChar(req->aMembers[i]);
			if (pMember)
			{
				pMember->SetJoinID(INVALID_JOINID);

				if (pMember->GetCharID() == req->charId)
					continue;

				CNtlPacket packetNfy(sizeof(sGU_BUDOKAI_LEAVE_TEAM_NFY));
				sGU_BUDOKAI_LEAVE_TEAM_NFY * res = (sGU_BUDOKAI_LEAVE_TEAM_NFY *)packetNfy.GetPacketData();
				res->wOpCode = GU_BUDOKAI_LEAVE_TEAM_NFY;
				packetNfy.SetPacketLen(sizeof(sGU_BUDOKAI_LEAVE_TEAM_NFY));
				g_pApp->Send(pMember->GetClientSessionID(), &packetNfy);
			}
		}
	}

	//get player. If exist, then send him result packet
	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CNtlPacket packet(sizeof(sGU_BUDOKAI_LEAVE_TEAM_RES));
	sGU_BUDOKAI_LEAVE_TEAM_RES * res = (sGU_BUDOKAI_LEAVE_TEAM_RES *)packet.GetPacketData();
	res->wOpCode = GU_BUDOKAI_LEAVE_TEAM_RES;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_BUDOKAI_LEAVE_TEAM_RES));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvBudokaiLeaveTeamMemberRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_LEAVE_TEAM_MEMBER_RES * req = (sQG_BUDOKAI_LEAVE_TEAM_MEMBER_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTeamSelectionRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TEAM_SELECTION_RES * req = (sQG_BUDOKAI_TEAM_SELECTION_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTeamListStart(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TEAM_LIST_START * req = (sQG_BUDOKAI_TEAM_LIST_START*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTeamListData(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TEAM_LIST_DATA * req = (sQG_BUDOKAI_TEAM_LIST_DATA*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTeamListEnd(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TEAM_LIST_END * req = (sQG_BUDOKAI_TEAM_LIST_END*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentTeamAddEntryListRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES * req = (sQG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES*)pPacket->GetPacketData();
	
	g_pBudokaiManager->SetJoinData(req->joinData.joinId, req->joinData.byJoinState, req->joinData.byJoinResult);
}

void CQueryServerSession::RecvBudokaiTournamentTeamEntryListStart(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_START * req = (sQG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_START*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentTeamEntryListData(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_DATA * req = (sQG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_DATA*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentTeamEntryListEnd(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_END * req = (sQG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_END*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentTeamAddMatchResultRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_TEAM_ADD_MATCH_RESULT_RES * req = (sQG_BUDOKAI_TOURNAMENT_TEAM_ADD_MATCH_RESULT_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiTournamentTeamMatchResultRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_TOURNAMENT_TEAM_MATCH_RESULT_RES * req = (sQG_BUDOKAI_TOURNAMENT_TEAM_MATCH_RESULT_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiJoinInfoRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_JOIN_INFO_RES * req = (sQG_BUDOKAI_JOIN_INFO_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiJoinStateRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_JOIN_STATE_RES * req = (sQG_BUDOKAI_JOIN_STATE_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiHistoryWriteRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_HISTORY_WRITE_RES * req = (sQG_BUDOKAI_HISTORY_WRITE_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);

	g_pBudokaiManager->CloseBudokai();
}

void CQueryServerSession::RecvBudokaiHistoryWinnerPlayerRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_HISTORY_WINNER_PLAYER_RES * req = (sQG_BUDOKAI_HISTORY_WINNER_PLAYER_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiJoinStateListRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_JOIN_STATE_LIST_RES * req = (sQG_BUDOKAI_JOIN_STATE_LIST_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiJoinStateListData(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_JOIN_STATE_LIST_DATA * req = (sQG_BUDOKAI_JOIN_STATE_LIST_DATA*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvBudokaiSetOpenTimeRes(CNtlPacket * pPacket)
{
	sQG_BUDOKAI_SET_OPEN_TIME_RES * req = (sQG_BUDOKAI_SET_OPEN_TIME_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}

void CQueryServerSession::RecvMatchRewardRes(CNtlPacket * pPacket)
{
	sQG_MATCH_REWARD_RES * req = (sQG_MATCH_REWARD_RES*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);
}


void CQueryServerSession::RecvShopEventItemBuyRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_SHOP_EVENTITEM_BUY_RES * req = (sQG_SHOP_EVENTITEM_BUY_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || pOwner->IsInitialized() == false)
		return;

	if (req->charId != pOwner->GetCharID())
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		for (BYTE i = 0; i < req->byBuyCount; i++)
		{
			sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->sInven[i].tblItem);

			CItem* pItem = new CItem;
			pItem->SetTbldat(pItemTbldat);
			pItem->SetID(g_pObjectManager->CreateUID());
			pItem->SetItemData(&req->sInven[i], req->itemID[i]);
			pItem->AddToCharacter(pOwner);

			g_pItemManager->AddItem(pItem);

			CNtlPacket packet3(sizeof(sGU_ITEM_CREATE));
			sGU_ITEM_CREATE * res3 = (sGU_ITEM_CREATE *)packet3.GetPacketData();
			res3->wOpCode = GU_ITEM_CREATE;
			res3->bIsNew = true;
			res3->handle = pItem->GetID();
			memcpy(&res3->sItemData, &pItem->GetItemData(), sizeof(sITEM_DATA));
			packet3.SetPacketLen(sizeof(sGU_ITEM_CREATE));
			pOwner->SendPacket(&packet3);

			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sInven[i].byPlace, req->sInven[i].byPos);
		}
	}
	else
	{
		for (BYTE i = 0; i < req->byBuyCount; i++)
		{
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sInven[i].byPlace, req->sInven[i].byPos);
		}
	}

	CNtlPacket packet(sizeof(sGU_SHOP_EVENTITEM_BUY_RES));
	sGU_SHOP_EVENTITEM_BUY_RES * res = (sGU_SHOP_EVENTITEM_BUY_RES *)packet.GetPacketData();
	res->wOpCode = GU_SHOP_EVENTITEM_BUY_RES;
	res->wResultCode = req->wResultCode;
	res->handle = req->hNpchandle;
	packet.SetPacketLen(sizeof(sGU_SHOP_EVENTITEM_BUY_RES));
	pOwner->SendPacket(&packet);
}

void CQueryServerSession::RecvShopGambleBuyRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_SHOP_GAMBLE_BUY_RES * req = (sQG_SHOP_GAMBLE_BUY_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || pOwner->IsInitialized() == false)
		return;

	if (req->charId != pOwner->GetCharID())
		return;

	CNtlPacket packet2(sizeof(sGU_SHOP_GAMBLE_BUY_RES));
	sGU_SHOP_GAMBLE_BUY_RES * res2 = (sGU_SHOP_GAMBLE_BUY_RES *)packet2.GetPacketData();
	res2->wOpCode = GU_SHOP_GAMBLE_BUY_RES;
	res2->handle = req->hNpchandle;

	if (req->wResultCode == GAME_SUCCESS)
	{
		CItem* pItem = new CItem;
		pItem->SetTbldat((sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->sInven.itemNo));
		pItem->SetID(g_pObjectManager->CreateUID());
		pItem->SetItemData(&req->sInven);
		pItem->AddToCharacter(pOwner);
		g_pItemManager->AddItem(pItem);

		CNtlPacket packet(sizeof(sGU_ITEM_CREATE));
		sGU_ITEM_CREATE * res = (sGU_ITEM_CREATE *)packet.GetPacketData();
		res->wOpCode = GU_ITEM_CREATE;
		res->bIsNew = true;
		res->handle = pItem->GetID();
		memcpy(&res->sItemData, &req->sInven, sizeof(sITEM_DATA));
		packet.SetPacketLen(sizeof(sGU_ITEM_CREATE));
		app->Send(pOwner->GetClientSessionID(), &packet);

		res2->hItem = pItem->GetID();
	}

	pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sInven.byPlace, req->sInven.byPosition);

	res2->wResultCode = req->wResultCode;
	packet2.SetPacketLen(sizeof(sGU_SHOP_GAMBLE_BUY_RES));
	app->Send(pOwner->GetClientSessionID(), &packet2);
}



void CQueryServerSession::RecvSkillInitRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_SKILL_INIT_RES * req = (sQG_SKILL_INIT_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || pOwner->GetCharID() != req->charId)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		CNtlPacket packet2(sizeof(sGU_SKILL_INIT_NFY));
		sGU_SKILL_INIT_NFY * res2 = (sGU_SKILL_INIT_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_SKILL_INIT_NFY;
		packet2.SetPacketLen(sizeof(sGU_SKILL_INIT_NFY));
		app->Send(pOwner->GetClientSessionID(), &packet2);

		pOwner->UpdateCharSP((DWORD)pOwner->GetLevel() - 1);

		//send packet again to remove skills but with this packet the query server will not send a RES packet
		CNtlPacket pQry(sizeof(sGQ_SKILL_INIT_REQ));
		sGQ_SKILL_INIT_REQ* rQry = (sGQ_SKILL_INIT_REQ*)pQry.GetPacketData();
		rQry->wOpCode = GQ_SKILL_INIT_REQ;
		rQry->handle = pOwner->GetID();
		rQry->charId = pOwner->GetCharID();
		rQry->dwSP = pOwner->GetSkillPoints();
		rQry->bySkillResetMethod = INVALID_BYTE;
		pQry.SetPacketLen(sizeof(sGQ_SKILL_INIT_REQ));
		app->Send(GetHandle(), &pQry);

		pOwner->GetSkillManager()->InitSkills(); //remove all and learn default skills


		if (req->bySkillResetMethod == 0)
			pOwner->UpdateZeni(ZENNY_CHANGE_TYPE_INIT_SKILL, req->dwZeni, false, false);
		else if (req->bySkillResetMethod == 1)
		{
			CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->itemId);
			if (pItem)
			{
				pItem->SetLocked(false);
				pItem->SetCount(UnsignedSafeDecrease<BYTE>(pItem->GetCount(), 1), false, false);
			}
		}
	}

	if (req->bySkillResetMethod == 0) // only use zeni (reset by npc)
	{
		CNtlPacket packet(sizeof(sGU_SKILL_INIT_RES));
		sGU_SKILL_INIT_RES* res = (sGU_SKILL_INIT_RES*)packet.GetPacketData();
		res->wOpCode = GU_SKILL_INIT_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_SKILL_INIT_RES));
		pOwner->SendPacket(&packet);
	}
	else //reset by item
	{
		CNtlPacket packet(sizeof(sGU_SKILL_RESET_PLUS_RES));
		sGU_SKILL_RESET_PLUS_RES* res = (sGU_SKILL_RESET_PLUS_RES*)packet.GetPacketData();
		res->wOpCode = GU_SKILL_RESET_PLUS_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_SKILL_RESET_PLUS_RES));
		pOwner->SendPacket(&packet);
	}
}


void CQueryServerSession::RecvHoipoiItemMakeRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_HOIPOIMIX_ITEM_MAKE_RES * req = (sQG_HOIPOIMIX_ITEM_MAKE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (req->charId != pOwner->GetCharID())
		return;

	if (req->bIsNew)
		pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sCreateData.byPlace, req->sCreateData.byPosition);

	CNtlPacket packet(sizeof(sGU_HOIPOIMIX_ITEM_CREATE_RES));
	sGU_HOIPOIMIX_ITEM_CREATE_RES * res = (sGU_HOIPOIMIX_ITEM_CREATE_RES *)packet.GetPacketData();
	res->wOpCode = GU_HOIPOIMIX_ITEM_CREATE_RES;
	res->objHandle = req->objHandle;
	res->recipeTblidx = req->recipeTblidx;
	res->wResultCode = req->wResultCode;

	if (req->wResultCode == GAME_SUCCESS)
	{
		CItem* pItem = NULL;

		if (req->bIsNew) //create item
		{
			pItem = g_pItemManager->CreateItem(pOwner, &req->sCreateData);
		}
		else
		{
			pItem = pOwner->GetPlayerItemContainer()->GetItem(req->sCreateData.byPlace, req->sCreateData.byPosition);
		}

		if (pItem)
		{
			res->itemHandle = pItem->GetID();
			res->itemTblidx = pItem->GetTblidx();
		}
		else
		{
			res->wResultCode = GAME_ITEM_NOT_FOUND;
			ERR_LOG(LOG_GENERAL, "ERROR COULD NOT FIND NEWLY CREATED ITEM. Player %u ItemTblidx %u Item-db-id Place %u Pos %u", req->charId, req->sCreateData.itemNo, req->sCreateData.itemId, req->sCreateData.byPlace, req->sCreateData.byPosition);
		}

		/*Update exp and level*/
		CNtlPacket packet2(sizeof(sGU_HOIPOIMIX_ITEM_CREATE_EXP_NFY));
		sGU_HOIPOIMIX_ITEM_CREATE_EXP_NFY * res2 = (sGU_HOIPOIMIX_ITEM_CREATE_EXP_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_HOIPOIMIX_ITEM_CREATE_EXP_NFY;
		res2->dwExpGained = req->dwExpGained;
		res2->dwCurExp = req->dwMixExp;
		res2->byCurLevel = req->byMixLevel;
		packet2.SetPacketLen(sizeof(sGU_HOIPOIMIX_ITEM_CREATE_EXP_NFY));
		pOwner->SendPacket(&packet2);
	}

	packet.SetPacketLen(sizeof(sGU_HOIPOIMIX_ITEM_CREATE_RES));
	pOwner->SendPacket(&packet);
}

void CQueryServerSession::RecvItemChangeDurationTimeRes(CNtlPacket * pPacket)
{
	sQG_ITEM_CHANGE_DURATIONTIME_RES * req = (sQG_ITEM_CHANGE_DURATIONTIME_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->sItem.byPlace, req->sItem.byPosition);
	if (pItem)
		pItem->SetLocked(false);

	if (req->wResultCode == GAME_SUCCESS)
	{
		pItem->SetUseEndTime(req->sItem.nUseEndTime);
		pItem->SetRestrictState(req->sItem.byRestrictState);
		pItem->SetDurationtype(req->sItem.byDurationType);

		if (pItem->IsExpired()) //check if item expired
		{
			CNtlPacket packet(sizeof(sGU_ITEM_EXPIRED_NFY));
			sGU_ITEM_EXPIRED_NFY * res = (sGU_ITEM_EXPIRED_NFY *)packet.GetPacketData();
			res->wOpCode = GU_ITEM_EXPIRED_NFY;
			res->hItem = pItem->GetID();
			res->byPlace = req->sItem.byPlace;
			res->byPos = req->sItem.byPosition;
			res->byRestrictState = req->sItem.byRestrictState;
			res->byDurationType = req->sItem.byDurationType;
			packet.SetPacketLen(sizeof(sGU_ITEM_EXPIRED_NFY));
			pOwner->SendPacket(&packet);
		}
		else
		{
			CNtlPacket packetItemUpdate(sizeof(sGU_ITEM_UPDATE));
			sGU_ITEM_UPDATE * resIU = (sGU_ITEM_UPDATE *)packetItemUpdate.GetPacketData();
			resIU->wOpCode = GU_ITEM_UPDATE;
			resIU->handle = pItem->GetID();
			memcpy(&resIU->sItemData, &pItem->GetItemData(), sizeof(sITEM_DATA));
			packetItemUpdate.SetPacketLen(sizeof(sGU_ITEM_UPDATE));
			g_pApp->Send(pOwner->GetClientSessionID(), &packetItemUpdate);
		}
	}

}

void CQueryServerSession::RecvShopNetpyItemBuyRes(CNtlPacket * pPacket)
{
	sQG_SHOP_NETPYITEM_BUY_RES * req = (sQG_SHOP_NETPYITEM_BUY_RES*)pPacket->GetPacketData();
}

void CQueryServerSession::RecvDurationItemBuyRes(CNtlPacket * pPacket)
{
	sQG_DURATION_ITEM_BUY_RES * req = (sQG_DURATION_ITEM_BUY_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (req->sItem.byStackcount == 1) //can only buy items with stack count 1. So if its bigger than 1, then we stacked a previous item
		{
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sItem.byPlace, req->sItem.byPosition);

			sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->sItem.itemNo);

			CItem* pItem = new CItem;
			pItem->SetTbldat(pItemTbldat);
			pItem->SetID(g_pObjectManager->CreateUID());
			pItem->SetItemData(&req->sItem);
			pItem->AddToCharacter(pOwner);

			g_pItemManager->AddItem(pItem);

			CNtlPacket packet3(sizeof(sGU_ITEM_CREATE));
			sGU_ITEM_CREATE * res3 = (sGU_ITEM_CREATE *)packet3.GetPacketData();
			res3->wOpCode = GU_ITEM_CREATE;
			res3->bIsNew = true;
			res3->handle = pItem->GetID();
			memcpy(&res3->sItemData, &pItem->GetItemData(), sizeof(sITEM_DATA));
			packet3.SetPacketLen(sizeof(sGU_ITEM_CREATE));
			pOwner->SendPacket(&packet3);
		}
	}
	else pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sItem.byPlace, req->sItem.byPosition);

	CNtlPacket packet(sizeof(sGU_DURATION_ITEM_BUY_RES));
	sGU_DURATION_ITEM_BUY_RES * res = (sGU_DURATION_ITEM_BUY_RES *)packet.GetPacketData();
	res->wOpCode = GU_DURATION_ITEM_BUY_RES;
	res->wResultCode = req->wResultCode;
	res->byPos = req->byPos;
	res->merchantTblidx = req->merchantTblidx;
	packet.SetPacketLen(sizeof(sGU_DURATION_ITEM_BUY_RES));
	pOwner->SendPacket(&packet);
}


void CQueryServerSession::RecvCashitemBuyRes(CNtlPacket* pPacket)
{
	sQG_CASHITEM_BUY_RES * req = (sQG_CASHITEM_BUY_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->characterId);
	if (!pOwner || pOwner->IsInitialized() == false)
		return; //dont need to log I guess

	if (pOwner->GetID() != req->handle || pOwner->GetCharID() != req->characterId)
		return;

	ERR_LOG(LOG_USER, "Account %u Char %u bought %u from cash shop. Price %u, Old Cash %u, New Cash %u", req->accountId, req->characterId, req->HLSitemTblidx, req->dwPrice, pOwner->GetItemShopCash(), req->dwRemainAmount);

	if (req->wResultCode == GAME_SUCCESS)
	{
		pOwner->SetItemShopCash(req->dwRemainAmount);

		CNtlPacket packet(sizeof(sGU_CASHITEM_ADD_NFY));
		sGU_CASHITEM_ADD_NFY * res = (sGU_CASHITEM_ADD_NFY *)packet.GetPacketData();
		res->wOpCode = GU_CASHITEM_ADD_NFY;
		res->sInfo.byStackCount = req->byCount;
		res->sInfo.HLSitemTblidx = req->HLSitemTblidx;
		res->sInfo.qwProductId = req->qwProductId;
		res->sInfo.tRegTime.day = req->tRegTime.day;
		res->sInfo.tRegTime.hour = req->tRegTime.hour;
		res->sInfo.tRegTime.minute = req->tRegTime.minute;
		res->sInfo.tRegTime.month = req->tRegTime.month;
		res->sInfo.tRegTime.second = req->tRegTime.second;
		res->sInfo.tRegTime.year = req->tRegTime.year;
		packet.SetPacketLen(sizeof(sGU_CASHITEM_ADD_NFY));
		pOwner->SendPacket(&packet);

		pOwner->AddCashShopItem(res->sInfo.qwProductId, &res->sInfo);
	}

	CNtlPacket packet(sizeof(sGU_CASHITEM_BUY_RES));
	sGU_CASHITEM_BUY_RES * res = (sGU_CASHITEM_BUY_RES *)packet.GetPacketData();
	res->wOpCode = GU_CASHITEM_BUY_RES;
	res->dwRemainAmount = req->dwRemainAmount;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_CASHITEM_BUY_RES));
	pOwner->SendPacket(&packet);
}


void CQueryServerSession::RecvPcDataLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_DATA_LOAD_RES * req = (sQG_PC_DATA_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->sPcData.charId);
	if (pOwner == NULL)
	{
		printf("player %u not found \n", req->sPcData.charId);
		return;
	}

	if (req->wResultCode != GAME_SUCCESS)
	{
		ERR_LOG(LOG_USER, "player %u does not exist on account %u \n", req->sPcData.charId, req->accountId);

		if (pOwner->GetClientSession())
			pOwner->GetClientSession()->Disconnect(false);

		return;
	}

	pOwner->RecvLoadPcDataRes(&req->sPcData, &req->serverChangeInfo, req->bTutorialFlag, &req->sWarFogInfo, &req->sMailBrief, &req->sRankBattleScore, req->TitleIndexFlag, req->wWaguCoins);
}

void CQueryServerSession::RecvPcItemLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_ITEM_LOAD_RES * req = (sQG_PC_ITEM_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcItemLoadRes(req->asItemData, req->byItemCount, req->wCurPacketCount);
}

void CQueryServerSession::RecvPcSkillLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_SKILL_LOAD_RES * req = (sQG_PC_SKILL_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcSkillLoadRes(req->asSkill, req->bySkillCount);
}

void CQueryServerSession::RecvPcBuffLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_BUFF_LOAD_RES * req = (sQG_PC_BUFF_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcBuffLoadRes(req->asBuff, req->byBuffCount);
}

void CQueryServerSession::RecvPcHtbSkillLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_HTB_LOAD_RES * req = (sQG_PC_HTB_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcHtbSkillLoadRes(req->asHTBSkill, req->byHTBSkillCount);
}

void CQueryServerSession::RecvPcQuestItemLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_QUEST_ITEM_LOAD_RES * req = (sQG_PC_QUEST_ITEM_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcQuestItemLoadRes(req->asQuestInventory, req->byQuestInventoryCount);
}

void CQueryServerSession::RecvPcQuestCompleteLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_QUEST_COMPLETE_LOAD_RES * req = (sQG_PC_QUEST_COMPLETE_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcQuestCompleteLoadRes(req->sQuestCompleteData);
}

void CQueryServerSession::RecvPcQuestProgressLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_QUEST_PROGRESS_LOAD_RES * req = (sQG_PC_QUEST_PROGRESS_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcQuestProgressLoadRes(req->asQuestProgress, req->byQuestProgressCount);
}

void CQueryServerSession::RecvPcQuickSlotLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_QUICK_SLOT_LOAD_RES * req = (sQG_PC_QUICK_SLOT_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcQuickSlotLoadRes(req->asQuickSlotData, req->byQuickSlotCount);
}

void CQueryServerSession::RecvPcShortCutLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_SHORTCUT_LOAD_RES * req = (sQG_PC_SHORTCUT_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcShortcutLoadRes(req->asShortcutData, req->byShortcutCount);
}

void CQueryServerSession::RecvPcItemRecipeLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_ITEM_RECIPE_LOAD_RES * req = (sQG_PC_ITEM_RECIPE_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcItemRecipeLoadRes(req->asRecipeData, req->wRecipeCount);
}

void CQueryServerSession::RecvMailStartInfo(CNtlPacket * pPacket)
{
	sQG_MAIL_START_INFO * req = (sQG_MAIL_START_INFO*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (!pOwner || !pOwner->IsInitialized())
	{
		return;
	}

	for (BYTE i = 0; i < req->byCount; i++)
	{
		if (pOwner->GetMail(req->asData[i].mailID)) //if mail already exist then continue loop because we do not need to send the mail info to client
		{
			continue;
		}

		CNtlPacket packet(sizeof(sGU_MAIL_LOAD_DATA));
		sGU_MAIL_LOAD_DATA * res = (sGU_MAIL_LOAD_DATA *)packet.GetPacketData();
		res->wOpCode = GU_MAIL_LOAD_DATA;
		res->sData.mailID = req->asData[i].mailID;
		res->sData.bySenderType = req->asData[i].bySenderType;
		res->sData.byMailType = req->asData[i].byMailType;
		res->byTextSize = req->asData[i].byTextSize;
		NTL_SAFE_WCSCPY(res->wszText, req->asData[i].wszText);
		res->sData.dwZenny = req->asData[i].dwZenny;
		wcscpy_s(res->sData.wszFromName, NTL_MAX_SIZE_CHAR_NAME + 1, req->asData[i].wszFromName);
		res->sData.bIsAccept = req->asData[i].bIsAccept;
		res->sData.bIsLock = req->asData[i].bIsLock;
		res->sData.bIsRead = req->asData[i].bIsRead;
		res->sData.endTime = req->asData[i].endTime;
		res->sData.byExpired = req->asData[i].byExpired;
		memcpy(&res->sData.tCreateTime, &req->asData[i].tCreateTime, sizeof(sDBO_TIME));

		if (req->asData[i].sItemData.itemId != INVALID_ITEMID)
		{
		//	res->sData.sItemProfile.handle = g_pObjectManager->CreateUID();
			res->sData.sItemProfile.tblidx = req->asData[i].sItemData.itemNo;
			res->sData.sItemProfile.byPlace = req->asData[i].sItemData.byPlace;
			res->sData.sItemProfile.byPos = req->asData[i].sItemData.byPosition;
			res->sData.sItemProfile.byStackcount = req->asData[i].sItemData.byStackcount;
			res->sData.sItemProfile.byRank = req->asData[i].sItemData.byRank;
			res->sData.sItemProfile.byCurDur = req->asData[i].sItemData.byCurrentDurability;
			res->sData.sItemProfile.byGrade = req->asData[i].sItemData.byGrade;
			res->sData.sItemProfile.bNeedToIdentify = req->asData[i].sItemData.bNeedToIdentify;
			res->sData.sItemProfile.byBattleAttribute = req->asData[i].sItemData.byBattleAttribute;
			wcscpy_s(res->sData.sItemProfile.awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, req->asData[i].sItemData.awchMaker);
			memcpy(&res->sData.sItemProfile.sOptionSet, &req->asData[i].sItemData.sOptionSet, sizeof(sITEM_OPTION_SET));
			res->sData.sItemProfile.nUseStartTime = req->asData[i].sItemData.nUseStartTime;
			res->sData.sItemProfile.nUseEndTime = req->asData[i].sItemData.nUseEndTime;
			res->sData.sItemProfile.byRestrictState = req->asData[i].sItemData.byRestrictState;
			res->sData.sItemProfile.byDurationType = req->asData[i].sItemData.byDurationType;
		}

		pOwner->AddMail(&res->sData);

		packet.SetPacketLen(sizeof(sGU_MAIL_LOAD_DATA));
		pOwner->SendPacket(&packet);
	}
}

void CQueryServerSession::RecvMailLoadInfo(CNtlPacket * pPacket)
{
	sQG_MAIL_LOAD_INFO * req = (sQG_MAIL_LOAD_INFO*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (!pOwner || !pOwner->IsInitialized())
	{
		return;
	}

	for (BYTE i = 0; i < req->byCount; i++)
	{
		if (pOwner->GetMail(req->asData[i].mailID)) //if mail already exist then continue loop because we do not need to send the mail info to client
		{
			continue;
		}

		CNtlPacket packet(sizeof(sGU_MAIL_LOAD_INFO));
		sGU_MAIL_LOAD_INFO * res = (sGU_MAIL_LOAD_INFO *)packet.GetPacketData();
		res->wOpCode = GU_MAIL_LOAD_INFO;
		res->sData.mailID = req->asData[i].mailID;
		res->sData.bySenderType = req->asData[i].bySenderType;
		res->sData.byMailType = req->asData[i].byMailType;
		res->byTextSize = req->asData[i].byTextSize;
		NTL_SAFE_WCSCPY(res->wszText, req->asData[i].wszText);
		res->sData.dwZenny = req->asData[i].dwZenny;
		wcscpy_s(res->sData.wszFromName, NTL_MAX_SIZE_CHAR_NAME + 1, req->asData[i].wszFromName);
		res->sData.bIsAccept = req->asData[i].bIsAccept;
		res->sData.bIsLock = req->asData[i].bIsLock;
		res->sData.bIsRead = req->asData[i].bIsRead;
		res->sData.endTime = req->asData[i].endTime;
		res->sData.byExpired = req->asData[i].byExpired;
		memcpy(&res->sData.tCreateTime, &req->asData[i].tCreateTime, sizeof(sDBO_TIME));

		if (req->asData[i].sItemData.itemId != INVALID_ITEMID)
		{
			//	res->sData.sItemProfile.handle = g_pObjectManager->CreateUID();
			res->sData.sItemProfile.tblidx = req->asData[i].sItemData.itemNo;
			res->sData.sItemProfile.byPlace = req->asData[i].sItemData.byPlace;
			res->sData.sItemProfile.byPos = req->asData[i].sItemData.byPosition;
			res->sData.sItemProfile.byStackcount = req->asData[i].sItemData.byStackcount;
			res->sData.sItemProfile.byRank = req->asData[i].sItemData.byRank;
			res->sData.sItemProfile.byCurDur = req->asData[i].sItemData.byCurrentDurability;
			res->sData.sItemProfile.byGrade = req->asData[i].sItemData.byGrade;
			res->sData.sItemProfile.bNeedToIdentify = req->asData[i].sItemData.bNeedToIdentify;
			res->sData.sItemProfile.byBattleAttribute = req->asData[i].sItemData.byBattleAttribute;
			wcscpy_s(res->sData.sItemProfile.awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, req->asData[i].sItemData.awchMaker);
			memcpy(&res->sData.sItemProfile.sOptionSet, &req->asData[i].sItemData.sOptionSet, sizeof(sITEM_OPTION_SET));
			res->sData.sItemProfile.nUseStartTime = req->asData[i].sItemData.nUseStartTime;
			res->sData.sItemProfile.nUseEndTime = req->asData[i].sItemData.nUseEndTime;
			res->sData.sItemProfile.byRestrictState = req->asData[i].sItemData.byRestrictState;
			res->sData.sItemProfile.byDurationType = req->asData[i].sItemData.byDurationType;
		}

		pOwner->AddMail(&res->sData);

		packet.SetPacketLen(sizeof(sGU_MAIL_LOAD_INFO));
		pOwner->SendPacket(&packet);
	}
}


void CQueryServerSession::RecvQuickTeleportInfoLoadRes(CNtlPacket * pPacket)
{
	sQG_QUICK_TELEPORT_INFO_LOAD_RES * req = (sQG_QUICK_TELEPORT_INFO_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charId);
	if (!pOwner || !pOwner->IsInitialized())
	{
		return;
	}

	CNtlPacket packet(sizeof(sGU_QUICK_TELEPORT_LOAD_RES));
	sGU_QUICK_TELEPORT_LOAD_RES * res = (sGU_QUICK_TELEPORT_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = GU_QUICK_TELEPORT_LOAD_RES;
	res->wResultCode = req->wResultCode;
	res->byCount = req->byCount;

	for (BYTE i = 0; i < req->byCount; i++)
	{
		memcpy(&res->asData[i], &req->asData[i], sizeof(sQUICK_TELEPORT_INFO));

		pOwner->LoadAddQuickTeleport(req->asData[i]);
	}

	packet.SetPacketLen(sizeof(sGU_QUICK_TELEPORT_LOAD_RES));
	pOwner->SendPacket(&packet);

	pOwner->SetIsQuickTeleportLoaded(true);
}

void CQueryServerSession::RecvPcItemCoolTimeLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_ITEM_COOL_TIME_LOAD_RES * req = (sQG_PC_ITEM_COOL_TIME_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcItemCoolTimeLoadRes(req->asItemCoolTimeData, req->byItemCoolTimeCount);
}



void CQueryServerSession::RecvCashItemRefreshRes(CNtlPacket * pPacket)
{
	sGQ_CASHITEM_HLSHOP_REFRESH_RES * req = (sGQ_CASHITEM_HLSHOP_REFRESH_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charId);
	if (!pOwner || !pOwner->IsInitialized())
	{
		return;
	}

	if (pOwner->GetID() != req->handle || pOwner->GetCharID() != req->charId)
		return;

	pOwner->SetItemShopCash(req->dwRemainAmount);

	CNtlPacket packet(sizeof(sGU_CASHITEM_HLSHOP_REFRESH_RES));
	sGU_CASHITEM_HLSHOP_REFRESH_RES* res = (sGU_CASHITEM_HLSHOP_REFRESH_RES*)packet.GetPacketData();
	res->wOpCode = GU_CASHITEM_HLSHOP_REFRESH_RES;
	res->wResultCode = req->wResultCode;
	res->dwRemainAmount = pOwner->GetItemShopCash();
	packet.SetPacketLen(sizeof(sGU_CASHITEM_HLSHOP_REFRESH_RES));
	pOwner->SendPacket(&packet);
}

void CQueryServerSession::RecvCashItemInfo(CNtlPacket * pPacket)
{
	sQG_CASHITEM_INFO * req = (sQG_CASHITEM_INFO*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
	{
		return;
	}

	if (pOwner->GetCharID() != req->charId)
		return;

	BYTE byCount = 0;

	CNtlPacket packet(sizeof(sGU_CASHITEM_INFO));
	sGU_CASHITEM_INFO * res = (sGU_CASHITEM_INFO *)packet.GetPacketData();
	res->wOpCode = GU_CASHITEM_INFO;

	for(BYTE i = 0; i < req->byCount; i++)
	{
		memcpy(&res->aInfo[res->byCount], &req->aInfo[i], sizeof(sCASHITEM_BRIEF));

		pOwner->AddCashShopItem(res->aInfo[res->byCount].qwProductId, &res->aInfo[res->byCount]);

		byCount++;
		res->byCount++;

		if (res->byCount == DBO_MAX_CASHITEM_COUNT_IN_PACKET_CLIENT || byCount == req->byCount)
		{
			packet.SetPacketLen(sizeof(sGU_CASHITEM_INFO));
			g_pApp->Send(pOwner->GetClientSessionID(), &packet);

			memset(res->aInfo, NULL, sizeof(res->aInfo));
			res->byCount = 0;
		}

	}
}

void CQueryServerSession::RecvCashItemInfoRes(CNtlPacket * pPacket)
{
	sQG_CASHITEM_INFO_RES * req = (sQG_CASHITEM_INFO_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
	{
		return;
	}

	if (pOwner->GetCharID() != req->charId)
		return;

	CNtlPacket packet(sizeof(sGU_CASHITEM_INFO_RES));
	sGU_CASHITEM_INFO_RES * res = (sGU_CASHITEM_INFO_RES *)packet.GetPacketData();
	res->wOpCode = GU_CASHITEM_INFO_RES;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_CASHITEM_INFO_RES));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvCashItemMoveRes(CNtlPacket * pPacket)
{
	sQG_CASHITEM_MOVE_RES * req = (sQG_CASHITEM_MOVE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
	{
		return;
	}

	if (pOwner->GetCharID() != req->charId)
		return;
	
	if (req->wResultCode == GAME_SUCCESS)
	{
		ERR_LOG(LOG_USER, "Account %u Char %u move %I64u from cash shop to inventory.", pOwner->GetAccountID(), pOwner->GetCharID(), req->qwProductId);

		//create item and shit

		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->sData.tblItem);

		CItem* pItem = new CItem;
		pItem->SetTbldat(pItemTbldat);
		pItem->SetID(g_pObjectManager->CreateUID());
		pItem->SetItemData(&req->sData, req->itemId);
		pItem->AddToCharacter(pOwner);

		g_pItemManager->AddItem(pItem);

		CNtlPacket packet3(sizeof(sGU_ITEM_CREATE));
		sGU_ITEM_CREATE * res3 = (sGU_ITEM_CREATE *)packet3.GetPacketData();
		res3->wOpCode = GU_ITEM_CREATE;
		res3->bIsNew = true;
		res3->handle = pItem->GetID();
		memcpy(&res3->sItemData, &pItem->GetItemData(), sizeof(sITEM_DATA));
		packet3.SetPacketLen(sizeof(sGU_ITEM_CREATE));
		pOwner->SendPacket(&packet3);

		pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sData.byPlace, req->sData.byPos);

		//del item from cash shop
		pOwner->DelCashShopItem(req->qwProductId);
	}

	CNtlPacket packet(sizeof(sGU_CASHITEM_MOVE_RES));
	sGU_CASHITEM_MOVE_RES* res = (sGU_CASHITEM_MOVE_RES*)packet.GetPacketData();
	res->wOpCode = GU_CASHITEM_MOVE_RES;
	res->qwProductId = req->qwProductId;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_CASHITEM_MOVE_RES));
	pOwner->SendPacket(&packet);
}

void CQueryServerSession::RecvCashitemUnpackRes(CNtlPacket * pPacket)
{
	sQG_CASHITEM_UNPACK_RES * req = (sQG_CASHITEM_UNPACK_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charId);
	if (!pOwner || !pOwner->IsInitialized())
	{
		return;
	}

	if (pOwner->GetID() != req->handle )
		return;

}


void CQueryServerSession::RecvCashitemSendGiftRes(CNtlPacket* pPacket)
{
	sQG_CASHITEM_SEND_GIFT_RES * req = (sQG_CASHITEM_SEND_GIFT_RES*)pPacket->GetPacketData();

	//packet for sender
	CPlayer* pSender = g_pObjectManager->FindByChar(req->SenderCharId);
	if (!pSender || pSender->IsInitialized() == false)
		return; //dont need to log I guess

	if (pSender->GetCharID() != req->SenderCharId)
		return;

	pSender->SetItemShopCash(req->dwRemainAmount);

	CNtlPacket packet(sizeof(sGU_CASHITEM_SEND_GIFT_RES));
	sGU_CASHITEM_SEND_GIFT_RES * res = (sGU_CASHITEM_SEND_GIFT_RES *)packet.GetPacketData();
	res->wOpCode = GU_CASHITEM_SEND_GIFT_RES;
	res->wResultCode = req->wResultCode;
	res->dwRemainAmount = pSender->GetItemShopCash();
	packet.SetPacketLen(sizeof(sGU_CASHITEM_SEND_GIFT_RES));
	pSender->SendPacket(&packet);

	//packet for receiver
	if (req->wResultCode == GAME_SUCCESS && req->DestAccountId != INVALID_ACCOUNTID)
	{
		CPlayer* pReceiver = g_pObjectManager->FindByAccount(req->DestAccountId);
		if (!pReceiver || pReceiver->IsInitialized() == false)
			return; //dont need to log I guess

		CNtlPacket packet2(sizeof(sGU_CASHITEM_RECV_GIFT_NFY));
		sGU_CASHITEM_RECV_GIFT_NFY * res2 = (sGU_CASHITEM_RECV_GIFT_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_CASHITEM_RECV_GIFT_NFY;
		res2->sInfo.qwProductId = req->qwProductId;
		res2->sInfo.HLSitemTblidx = req->dwIdxHlsTable;
		res2->sInfo.byStackCount = req->byCount;
		res2->sInfo.tRegTime.day = req->tRegTime.day;
		res2->sInfo.tRegTime.hour = req->tRegTime.hour;
		res2->sInfo.tRegTime.minute = req->tRegTime.minute;
		res2->sInfo.tRegTime.month = req->tRegTime.month;
		res2->sInfo.tRegTime.second = req->tRegTime.second;
		res2->sInfo.tRegTime.year = req->tRegTime.year;
		wcscpy_s(res2->sInfo.wchSenderName, NTL_MAX_SIZE_CHAR_NAME + 1, req->wchSenderName);
		packet2.SetPacketLen(sizeof(sGU_CASHITEM_RECV_GIFT_NFY));
		pReceiver->SendPacket(&packet2);

		pReceiver->AddCashShopItem(res2->sInfo.qwProductId, &res2->sInfo);
	}
}

void CQueryServerSession::RecvPcMascotLoadRes(CNtlPacket * pPacket)
{
	sQG_PC_MASCOT_LOAD_RES * req = (sQG_PC_MASCOT_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charID);
	if (pOwner == NULL)
	{
		return;
	}

	pOwner->RecvPcMascotLoadRes(req->asMascotData, req->byMascotCount);
}


void CQueryServerSession::RecvMaterialDisassembleRes(CNtlPacket* pPacket)
{
	sQG_MATERIAL_DISASSEMBLE_RES * req = (sQG_MATERIAL_DISASSEMBLE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;


	CNtlPacket packet(sizeof(sGU_ITEM_DISASSEMBLE_RES));
	sGU_ITEM_DISASSEMBLE_RES* res = (sGU_ITEM_DISASSEMBLE_RES*)packet.GetPacketData();
	res->wOpCode = GU_ITEM_DISASSEMBLE_RES;
	res->wResultCode = req->wResultCode;
	res->byDeleteItemPlace = req->sDeleteItem.byPlace;
	res->byDeleteItemPos = req->sDeleteItem.byPos;
	res->hDeleteItem = req->sDeleteItem.hItem;
	packet.SetPacketLen(sizeof(sGU_ITEM_DISASSEMBLE_RES));
	pOwner->SendPacket(&packet);

	if (req->wResultCode == GAME_SUCCESS)
	{
		for (int i = 0; i < req->byCount; i++)
		{
			if (pOwner->GetPlayerItemContainer()->IsInventoryReserved(req->asCreateItem[i].byPlace, req->asCreateItem[i].byPosition)) //if reserved, then it means a new item has been created
			{
				sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->asCreateItem[i].itemNo);

				pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->asCreateItem[i].byPlace, req->asCreateItem[i].byPosition);

				CItem* item = new CItem;
				item->SetTbldat(pItemTbldat);
				item->SetID(g_pObjectManager->CreateUID());
				item->SetLocked(false);
				item->SetItemData(&req->asCreateItem[i]);
				item->AddToCharacter(pOwner);

				g_pItemManager->AddItem(item);

				CNtlPacket packet2(sizeof(sGU_ITEM_CREATE));
				sGU_ITEM_CREATE * res2 = (sGU_ITEM_CREATE *)packet2.GetPacketData();
				res2->wOpCode = GU_ITEM_CREATE;
				res2->bIsNew = true;
				res2->handle = item->GetID();
				memcpy(&res2->sItemData, &item->GetItemData(), sizeof(sITEM_DATA));
				packet2.SetPacketLen(sizeof(sGU_ITEM_CREATE));
				pOwner->SendPacket(&packet2);

		//		res->ItemResultId[i] = item->GetID();
			}
			//else
			//{
			//	if (CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->asCreateItem[i].itemId))
			//	{
			//		res->ItemResultId[i] = pItem->GetID();
			//	}
			//	else res->ItemResultId[i] = INVALID_HOBJECT;
			//}
		}
	}
	else
	{
		for (int i = 0; i < req->byCount; i++)
		{
			pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->asCreateItem[i].byPlace, req->asCreateItem[i].byPosition);
		}
	}
}


void CQueryServerSession::RecvItemSocketInsertBeadRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_ITEM_SOCKET_INSERT_BEAD_RES * req = (sQG_ITEM_SOCKET_INSERT_BEAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CNtlPacket packet(sizeof(sGU_ITEM_SOCKET_INSERT_BEAD_RES));
	sGU_ITEM_SOCKET_INSERT_BEAD_RES * res = (sGU_ITEM_SOCKET_INSERT_BEAD_RES *)packet.GetPacketData();
	res->wOpCode = GU_ITEM_SOCKET_INSERT_BEAD_RES;
	res->wResultCode = req->wResultCode;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->byRestrictState = req->byRestrictState;
	res->byDurationType = req->byDurationType;
	res->nUseStartTime = req->nUseStartTime;
	res->nUseEndTime = req->nUseEndTime;
	res->sItemRandomOption[0].optionValue = req->sItemRandomOption[0].optionValue;
	res->sItemRandomOption[0].wOptionIndex = req->sItemRandomOption[0].wOptionIndex;
	res->sItemRandomOption[1].optionValue = req->sItemRandomOption[1].optionValue;
	res->sItemRandomOption[1].wOptionIndex = req->sItemRandomOption[1].wOptionIndex;
	res->byBeadPlace = req->byBeadPlace;
	res->byBeadPos = req->byBeadPos;
	res->byBeadRemainStack = req->byBeadRemainStack;

	if (req->wResultCode == GAME_SUCCESS)
	{
		CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->byItemPlace, req->byItemPos);
		if (pItem && pItem->GetItemID() == req->ItemId)
		{
			pItem->SetSocketBead(req->byRestrictState, req->byDurationType, req->sItemRandomOption, req->nUseStartTime, req->nUseEndTime);
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "ERROR ITEM NOT FOUND !!! Player %u Item ID %I64u", req->charId, req->ItemId);
			res->wResultCode = GAME_ITEM_NOT_FOUND;
		}
	}

	packet.SetPacketLen(sizeof(sGU_ITEM_SOCKET_INSERT_BEAD_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvItemSocketDestroyBeadRes(CNtlPacket* pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_ITEM_SOCKET_DESTROY_BEAD_RES * req = (sQG_ITEM_SOCKET_DESTROY_BEAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->FindByChar(req->charId);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetID() != req->handle)
		return;

	CNtlPacket packet(sizeof(sGU_ITEM_SOCKET_DESTROY_BEAD_RES));
	sGU_ITEM_SOCKET_DESTROY_BEAD_RES * res = (sGU_ITEM_SOCKET_DESTROY_BEAD_RES *)packet.GetPacketData();
	res->wOpCode = GU_ITEM_SOCKET_DESTROY_BEAD_RES;
	res->wResultCode = req->wResultCode;
	res->byDurationType = req->byDurationType;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->byRestrictState = req->byRestrictState;

	if (req->wResultCode == GAME_SUCCESS)
	{
		CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->byItemPlace, req->byItemPos);
		if (pItem)
		{
			pItem->SetSocketBead(req->byRestrictState, req->byDurationType, NULL, 0, 0);
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "ERROR ITEM NOT FOUND !!! Player %u Item ID %I64u", req->charId, req->ItemId);
			res->wResultCode = GAME_ITEM_NOT_FOUND;
		}
	}

	packet.SetPacketLen(sizeof(sGU_ITEM_SOCKET_DESTROY_BEAD_RES));
	app->Send(pOwner->GetClientSessionID(), &packet);
}


void CQueryServerSession::RecvCashItemPublicBankAddRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_CASHITEM_PUBLIC_BANK_ADD_RES * req = (sQG_CASHITEM_PUBLIC_BANK_ADD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->byPlace, req->byPosition);

	if (req->wResultCode == GAME_SUCCESS)
	{
		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->itemNo);

		CItem* item = new CItem;
		item->SetTbldat(pItemTbldat);
		item->GenerateOptionSet(false);
		item->SetID(g_pObjectManager->CreateUID());
		item->SetLocked(false);

		CNtlPacket packet2(sizeof(sGU_CREATE_BANK_NFY));
		sGU_CREATE_BANK_NFY * res2 = (sGU_CREATE_BANK_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_CREATE_BANK_NFY;
		res2->bIsNew = true;
		res2->handle = item->GetID();
		res2->sItemData.byDurationType = req->byDurationType;
		res2->sItemData.byPlace = req->byPlace;
		res2->sItemData.byPosition = req->byPosition;
		res2->sItemData.byRank = req->byRank;
		res2->sItemData.byStackcount = 1;
		res2->sItemData.charId = req->charId;
		res2->sItemData.itemId = req->itemID;
		res2->sItemData.itemNo = req->itemNo;
		res2->sItemData.nUseEndTime = req->nUseEndTime;
		res2->sItemData.nUseStartTime = req->nUseStartTime;
		res2->sItemData.sOptionSet.Init();
		packet2.SetPacketLen(sizeof(sGU_CREATE_BANK_NFY));
		pOwner->SendPacket(&packet2);

		item->SetItemData(&res2->sItemData);
		item->AddToCharacter(pOwner);

		g_pItemManager->AddItem(item);

		pOwner->DelCashShopItem(req->dwProductID);

		CNtlPacket packet3(sizeof(sGU_CASHITEM_DEL_NFY));
		sGU_CASHITEM_DEL_NFY * res3 = (sGU_CASHITEM_DEL_NFY *)packet3.GetPacketData();
		res3->wOpCode = GU_CASHITEM_DEL_NFY;
		res3->qwProductId = req->dwProductID;
		packet3.SetPacketLen(sizeof(sGU_CASHITEM_DEL_NFY));
		pOwner->SendPacket(&packet3);
	}


	CNtlPacket packet(sizeof(sGU_CASHITEM_USE_RES));
	sGU_CASHITEM_USE_RES* res = (sGU_CASHITEM_USE_RES*)packet.GetPacketData();
	res->wOpCode = GU_CASHITEM_USE_RES;
	res->wResultCode = req->wResultCode;
	res->qwProductId = req->dwProductID;
	packet.SetPacketLen(sizeof(sGU_CASHITEM_USE_RES));
	pOwner->SendPacket(&packet);
}


void CQueryServerSession::RecvItemExchangeRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_ITEM_EXCHANGE_RES * req = (sQG_ITEM_EXCHANGE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (req->byCreateCount > 0)
		{
			for (int i = 0; i < req->byCreateCount; i++)
			{
				sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(req->aCreateItem[i].itemNo);

				pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->aCreateItem[i].byPlace, req->aCreateItem[i].byPosition);

				CItem* item = new CItem;
				item->SetTbldat(pItemTbldat);
				item->SetID(g_pObjectManager->CreateUID());
				item->SetLocked(false);
				item->SetItemData(&req->aCreateItem[i]);
				item->AddToCharacter(pOwner);

				g_pItemManager->AddItem(item);

				CNtlPacket packet(sizeof(sGU_ITEM_CREATE));
				sGU_ITEM_CREATE * res = (sGU_ITEM_CREATE *)packet.GetPacketData();
				res->wOpCode = GU_ITEM_CREATE;
				res->bIsNew = true;
				res->handle = item->GetID();
				memcpy(&res->sItemData, &item->GetItemData(), sizeof(sITEM_DATA));
				packet.SetPacketLen(sizeof(sGU_ITEM_CREATE));
				pOwner->SendPacket(&packet);
			}
		}
	}
	else
	{
		if (req->byCreateCount > 0)
		{
			for (int i = 0; i < req->byCreateCount; i++)
			{
				pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->aCreateItem[i].byPlace, req->aCreateItem[i].byPosition);
			}
		}
	}

	CNtlPacket packet2(sizeof(sGU_ITEM_EXCHANGE_RES));
	sGU_ITEM_EXCHANGE_RES * res2 = (sGU_ITEM_EXCHANGE_RES *)packet2.GetPacketData();
	res2->wOpCode = GU_ITEM_EXCHANGE_RES;
	res2->wResultCode = req->wResultCode;
	packet2.SetPacketLen(sizeof(sGU_ITEM_EXCHANGE_RES));
	pOwner->SendPacket(&packet2);
}

void CQueryServerSession::RecvCharacterRenameRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_CHARACTER_RENAME_RES * req = (sQG_CHARACTER_RENAME_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->byPlace, req->byPos);
	if (pItem)
		pItem->SetLocked(false);

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (pItem)
		{
			pItem->SetCount(pItem->GetCount() - 1, false, false);
		}

		pOwner->SetCharName(req->wszCharName);

		CNtlPacket packet2(sizeof(sGT_CHAR_NAME_CHANGE_NFY));
		sGT_CHAR_NAME_CHANGE_NFY * res2 = (sGT_CHAR_NAME_CHANGE_NFY *)packet2.GetPacketData();
		res2->wOpCode = GT_CHAR_NAME_CHANGE_NFY;
		res2->charId = pOwner->GetCharID();
		NTL_SAFE_WCSCPY(res2->wszCharName, req->wszCharName);
		packet2.SetPacketLen(sizeof(sGT_CHAR_NAME_CHANGE_NFY));
		app->SendTo(app->GetChatServerSession(), &packet2);
	}

	CNtlPacket packet(sizeof(sGU_CHARACTER_RENAME_RES));
	sGU_CHARACTER_RENAME_RES * res = (sGU_CHARACTER_RENAME_RES *)packet.GetPacketData();
	res->wOpCode = GU_CHARACTER_RENAME_RES;
	res->wResultCode = req->wResultCode;
	packet.SetPacketLen(sizeof(sGU_CHARACTER_RENAME_RES));
	pOwner->SendPacket(&packet);
}

void CQueryServerSession::RecvItemChangeOptionRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_ITEM_CHANGE_OPTION_RES * req = (sQG_ITEM_CHANGE_OPTION_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->ItemId);
	if (pItem)
	{
		pItem->SetLocked(false);
	}

	CItem* pKit = pOwner->GetPlayerItemContainer()->GetItem(req->KitItemId);
	if (pKit)
	{
		pKit->SetLocked(false);
	}

	CNtlPacket packet2(sizeof(sGU_ITEM_CHANGE_OPTION_RES));
	sGU_ITEM_CHANGE_OPTION_RES * res2 = (sGU_ITEM_CHANGE_OPTION_RES *)packet2.GetPacketData();
	res2->wOpCode = GU_ITEM_CHANGE_OPTION_RES;
	res2->wResultCode = req->wResultCode;
	res2->byItemPlace = req->byItemPlace;
	res2->byItemPos = req->byItemPos;
	res2->byKitPlace = req->byKitPlace;
	res2->byKitPos = req->byKitPos;
	memcpy(&res2->sItemOptionSet, &req->sItemOptionSet, sizeof(sITEM_OPTION_SET));
	res2->byKitRemainStack = req->byKitStack;

	if (req->wResultCode == GAME_SUCCESS)
	{
		CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->ItemId);
		if (pItem)
		{
			pItem->SetOptionSet(req->sItemOptionSet);
		}
	}
	
	packet2.SetPacketLen(sizeof(sGU_ITEM_CHANGE_OPTION_RES));
	pOwner->SendPacket(&packet2);
}

void CQueryServerSession::RecvWaguWaguMachineCoinIncreaseRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sQG_WAGUWAGUMACHINE_COIN_INCREASE_RES * req = (sQG_WAGUWAGUMACHINE_COIN_INCREASE_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->characterId)
		return;

	if (req->wResultCode == GAME_SUCCESS)
	{
		pOwner->SetWaguMachineCoin(req->wTotalWaguCoin);

		//send packet to chat server that waguwagu has increased
		CNtlPacket packetGT(sizeof(sGT_WAGUCOIN_INCREASE_NFY));
		sGT_WAGUCOIN_INCREASE_NFY* resGT = (sGT_WAGUCOIN_INCREASE_NFY*)packetGT.GetPacketData();
		resGT->wOpCode = GT_WAGUCOIN_INCREASE_NFY;
		resGT->charId = pOwner->GetCharID();
		resGT->dwWaguCoin = pOwner->GetWaguMachineCoin();
		resGT->accountId = pOwner->GetAccountID();
		packetGT.SetPacketLen(sizeof(sGT_WAGUCOIN_INCREASE_NFY));
		app->SendTo(app->GetChatServerSession(), &packetGT);

		if (req->qwProductId != INVALID_ITEMID)
		{
			pOwner->DelCashShopItem(req->qwProductId);

			CNtlPacket packet2(sizeof(sGU_CASHITEM_DEL_NFY));
			sGU_CASHITEM_DEL_NFY * res = (sGU_CASHITEM_DEL_NFY *)packet2.GetPacketData();
			res->wOpCode = GU_CASHITEM_DEL_NFY;
			res->qwProductId = req->qwProductId;
			packet2.SetPacketLen(sizeof(sGU_CASHITEM_DEL_NFY));
			pOwner->SendPacket(&packet2);
		}
	}

	if (req->qwProductId != INVALID_ITEMID)
	{
		CNtlPacket packet(sizeof(sGU_CASHITEM_USE_RES));
		sGU_CASHITEM_USE_RES* res = (sGU_CASHITEM_USE_RES*)packet.GetPacketData();
		res->wOpCode = GU_CASHITEM_USE_RES;
		res->wResultCode = req->wResultCode;
		res->qwProductId = req->qwProductId;
		packet.SetPacketLen(sizeof(sGU_CASHITEM_USE_RES));
		pOwner->SendPacket(&packet);
	}
}

void CQueryServerSession::RecvDynamicFieldSystemCountingRes(CNtlPacket * pPacket)
{
	sQG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES * req = (sQG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES*)pPacket->GetPacketData();

	g_pDynamicFieldSystemEvent->UpdateDynamicFieldCount(req->dwTotalCount);
}

void CQueryServerSession::RecvItemSealRes(CNtlPacket * pPacket)
{
	sQG_ITEM_SEAL_RES * req = (sQG_ITEM_SEAL_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->byItemPlace, req->byItemPos);
	CItem* pSealItem = pOwner->GetPlayerItemContainer()->GetItem(req->bySealPlace, req->bySealPos);


	if (pItem)
		pItem->SetLocked(false);
	if (pSealItem)
		pSealItem->SetLocked(false);


	if (req->wResultCode == GAME_SUCCESS)
	{
		if (pItem)
		{
			pItem->SetRestrictState(req->byRestrictState);
		}

		if (pSealItem)
		{
			if (req->bySealRemainStack == 0)
			{
				pOwner->GetPlayerItemContainer()->RemoveItem(req->bySealPlace, pSealItem->GetID());
				g_pItemManager->DestroyItem(pSealItem);
			}
			else
			{
				pSealItem->SetStackCount(req->bySealRemainStack);
			}
		}
	}


	CNtlPacket packet(sizeof(sGU_ITEM_SEAL_RES));
	sGU_ITEM_SEAL_RES* res = (sGU_ITEM_SEAL_RES*)packet.GetPacketData();
	res->wOpCode = GU_ITEM_SEAL_RES;
	res->wResultCode = req->wResultCode;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->bySealPlace = req->bySealPlace;
	res->bySealPos = req->bySealPos;
	res->byRestrictState = req->byRestrictState;
	res->bySealStack = req->bySealRemainStack;
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvItemSealExtractRes(CNtlPacket * pPacket)
{
	sQG_ITEM_SEAL_EXTRACT_RES * req = (sQG_ITEM_SEAL_EXTRACT_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CItem* pItem = pOwner->GetPlayerItemContainer()->GetItem(req->byItemPlace, req->byItemPos);
	if (pItem)
		pItem->SetLocked(false);


	if (req->wResultCode == GAME_SUCCESS)
	{
		if (pItem)
		{
			pItem->SetRestrictState(req->byRestrictState);
		}
	}


	CNtlPacket packet(sizeof(sGU_ITEM_SEAL_EXTRACT_RES));
	sGU_ITEM_SEAL_EXTRACT_RES* res = (sGU_ITEM_SEAL_EXTRACT_RES*)packet.GetPacketData();
	res->wOpCode = GU_ITEM_SEAL_EXTRACT_RES;
	res->wResultCode = req->wResultCode;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->byRestrictState = req->byRestrictState;
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvDragonballScrambleEndNfy(CNtlPacket * pPacket)
{
	UNREFERENCED_PARAMETER(pPacket);

	g_pDragonballScramble->EndEventNfy();
}

void CQueryServerSession::RecvItemUpgradeByCouponRes(CNtlPacket * pPacket)
{
	sQG_ITEM_UPGRADE_BY_COUPON_RES * req = (sQG_ITEM_UPGRADE_BY_COUPON_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CItem* pEquipmentItem = pOwner->GetPlayerItemContainer()->GetItem(req->byItemPlace, req->byItemPos);
	if (pEquipmentItem)
		pEquipmentItem->SetLocked(false);

	CItem* pCouponItem = pOwner->GetPlayerItemContainer()->GetItem(req->byCouponPlace, req->byCouponPos);
	if (pCouponItem)
		pCouponItem->SetLocked(false);

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (pCouponItem)
		{
			pCouponItem->SetCount(req->byStack, false, false);
		}
		else
		{
			ERR_LOG(LOG_USER, "Could not find coupon. User %u, Coupon %I64u", req->charId, req->couponId);
		}

		if (pEquipmentItem)
		{
			pEquipmentItem->SetRestrictState(req->byRestrictState);
			pEquipmentItem->SetGrade(req->byGrade);
			pEquipmentItem->SetUseEndTime(req->nUseEndTime);
			pEquipmentItem->SetUseStartTime(req->nUseStartTime);
			pEquipmentItem->SetDurationtype(req->byDurationType);

			pOwner->GetPlayerItemContainer()->AddItemDuration(pEquipmentItem->GetID(), pEquipmentItem);
		}
		else
		{
			ERR_LOG(LOG_USER, "Could not find equipment. User %u", req->charId);
		}
	}

	CNtlPacket packet(sizeof(sGU_ITEM_UPGRADE_BY_COUPON_RES));
	sGU_ITEM_UPGRADE_BY_COUPON_RES* res = (sGU_ITEM_UPGRADE_BY_COUPON_RES*)packet.GetPacketData();
	res->wOpCode = GU_ITEM_UPGRADE_BY_COUPON_RES;
	res->wResultCode = req->wResultCode;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->byCouponPlace = req->byCouponPlace;
	res->byCouponPos = req->byCouponPos;
	res->byGrade = req->byGrade;
	res->byDurationType = req->byDurationType;
	res->nUseStartTime = req->nUseStartTime;
	res->nUseEndTime = req->nUseEndTime;
	res->byCouponStack = req->byStack;
	res->byRestrictState = req->byRestrictState;
	packet.SetPacketLen(sizeof(sGU_ITEM_UPGRADE_BY_COUPON_RES));
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvItemGradeInitByCouponRes(CNtlPacket * pPacket)
{
	sQG_ITEM_GRADEINIT_BY_COUPON_RES * req = (sQG_ITEM_GRADEINIT_BY_COUPON_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CItem* pEquipmentItem = pOwner->GetPlayerItemContainer()->GetItem(req->byItemPlace, req->byItemPos);
	if (pEquipmentItem)
		pEquipmentItem->SetLocked(false);

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (pEquipmentItem)
		{
			pEquipmentItem->SetRestrictState(req->byRestrictState);
			pEquipmentItem->SetGrade(0);
			pEquipmentItem->SetUseEndTime(0);
			pEquipmentItem->SetUseStartTime(0);
			pEquipmentItem->SetDurationtype(eDURATIONTYPE_NORMAL);

			CNtlPacket packetItemUpdate(sizeof(sGU_ITEM_UPDATE));
			sGU_ITEM_UPDATE * resIU = (sGU_ITEM_UPDATE *)packetItemUpdate.GetPacketData();
			resIU->wOpCode = GU_ITEM_UPDATE;
			resIU->handle = pEquipmentItem->GetID();
			memcpy(&resIU->sItemData, &pEquipmentItem->GetItemData(), sizeof(sITEM_DATA));
			packetItemUpdate.SetPacketLen(sizeof(sGU_ITEM_UPDATE));
			g_pApp->Send(pOwner->GetClientSessionID(), &packetItemUpdate);
		}
	}
}

void CQueryServerSession::RecvEventRewardLoadInfo(CNtlPacket * pPacket)
{
	sQG_EVENT_REWARD_LOAD_INFO * req = (sQG_EVENT_REWARD_LOAD_INFO*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	//pOwner->ClearEventRewardMap(); //clear existing map

	//CNtlPacket packet(sizeof(sGU_EVENT_REWARD_LOAD_INFO));
	//sGU_EVENT_REWARD_LOAD_INFO* res = (sGU_EVENT_REWARD_LOAD_INFO*)packet.GetPacketData();
	//res->wOpCode = GU_EVENT_REWARD_LOAD_INFO;
	//res->nCount = (int)req->byCount;

	//for (BYTE i = 0; i < req->byCount; i++)
	//{
	//	res->aInfo[i].charId = req->asInfo[i].charId;
	//	res->aInfo[i].eventTblidx = req->asInfo[i].eventTblidx;

	//	// insert into players map
	//	pOwner->InsertEventReward(req->asInfo[i].eventTblidx, req->asInfo[i].charId);
	//}

	//g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvEventRewardLoadRes(CNtlPacket * pPacket)
{
	sQG_EVENT_REWARD_LOAD_RES * req = (sQG_EVENT_REWARD_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	CNtlPacket packet(sizeof(sGU_EVENT_REWARD_LOAD_RES));
	sGU_EVENT_REWARD_LOAD_RES* res = (sGU_EVENT_REWARD_LOAD_RES*)packet.GetPacketData();
	res->wOpCode = GU_EVENT_REWARD_LOAD_RES;
	res->wResultCode = req->wResultCode;
	res->byCount = req->byCount;

	if (req->wResultCode == GAME_SUCCESS)
	{
		pOwner->ClearEventRewardMap(); //clear existing map

		CNtlPacket packetInfo(sizeof(sGU_EVENT_REWARD_LOAD_INFO));
		sGU_EVENT_REWARD_LOAD_INFO* resInfo = (sGU_EVENT_REWARD_LOAD_INFO*)packetInfo.GetPacketData();
		resInfo->wOpCode = GU_EVENT_REWARD_LOAD_INFO;
		resInfo->nCount = (int)req->byCount;

		for (BYTE i = 0; i < req->byCount; i++)
		{
			resInfo->aInfo[i].charId = (req->asInfo[i].charId == 0) ? pOwner->GetCharID() : req->asInfo[i].charId;
			resInfo->aInfo[i].eventTblidx = req->asInfo[i].eventTblidx;

			// insert into players map
			req->asNameInfo[i].charId = resInfo->aInfo[i].charId;
			pOwner->InsertEventReward(req->asInfo[i].eventTblidx, req->asNameInfo[i]);
		}

		memcpy(res->asNameInfo, req->asNameInfo, sizeof(sEVENT_PC_NAME) * req->byCount);

		g_pApp->Send(pOwner->GetClientSessionID(), &packetInfo);
	}

	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvEventRewardSelectRes(CNtlPacket * pPacket)
{
	sQG_EVENT_REWARD_SELECT_RES * req = (sQG_EVENT_REWARD_SELECT_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sItem.byPlace, req->sItem.byPosition);

	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pItemManager->CreateItem(pOwner, &req->sItem);
	}

	CNtlPacket packet(sizeof(sGU_EVENT_REWARD_SELECT_RES));
	sGU_EVENT_REWARD_SELECT_RES* res = (sGU_EVENT_REWARD_SELECT_RES*)packet.GetPacketData();
	res->wOpCode = GU_EVENT_REWARD_SELECT_RES;
	res->wResultCode = req->wResultCode;
	res->eventTblidx = req->eventTblidx;
	g_pApp->Send(pOwner->GetClientSessionID(), &packet);
}

void CQueryServerSession::RecvMascotSealSetRes(CNtlPacket * pPacket)
{
	sQG_MASCOT_SEAL_SET_RES * req = (sQG_MASCOT_SEAL_SET_RES*)pPacket->GetPacketData();

	CPlayer* pOwner = g_pObjectManager->GetPC(req->handle);
	if (!pOwner || !pOwner->IsInitialized())
		return; //dont need to log I guess

	if (pOwner->GetCharID() != req->charId)
		return;

	pOwner->GetPlayerItemContainer()->RemoveReservedInventory(req->sNewItemData.byPlace, req->sNewItemData.byPosition);


	CNtlPacket packet(sizeof(sGU_MASCOT_SEAL_SET_RES));
	sGU_MASCOT_SEAL_SET_RES * res = (sGU_MASCOT_SEAL_SET_RES*)packet.GetPacketData();
	res->wOpCode = GU_MASCOT_SEAL_SET_RES;
	res->wResultCode = req->wResultCode;
	res->byMascotPos = req->byMascotPos;
	res->bySealPlace = req->bySealPlace;
	res->bySealPos = req->bySealPos;
	res->bySealStack = req->bySealStack;

	memcpy(&res->sItem, &req->sNewItemData, sizeof(sITEM_DATA));

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (req->bySealStack > 0)
		{
			CItem* pSealItem = pOwner->GetPlayerItemContainer()->GetItem(req->bySealPlace, req->bySealPos);
			if (pSealItem)
			{
				pSealItem->SetLocked(false);
			}
		}

		res->hNewItemHandle = g_pItemManager->CreateItem(pOwner, &req->sNewItemData, false)->GetID();
	}


	packet.SetPacketLen(sizeof(sGU_MASCOT_SEAL_SET_RES));
	pOwner->SendPacket(&packet);
}

void CQueryServerSession::RecvMascotSealClearRes(CNtlPacket * pPacket)
{
}
