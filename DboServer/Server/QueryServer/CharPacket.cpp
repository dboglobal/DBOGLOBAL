#include "stdafx.h"
#include "QueryServer.h"
#include "NtlPacketCQ.h"
#include "NtlPacketQC.h"
#include "NtlResultCode.h"
#include "ItemManager.h"
#include "CharacterManager.h"
#include "PlayerCache.h"



void CCharServerSession::RecvCharacterInfoReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sCQ_CHARACTER_INFO_REQ * req = (sCQ_CHARACTER_INFO_REQ*)pPacket->GetPacketData();

	UNREFERENCED_PARAMETER(app);
	UNREFERENCED_PARAMETER(req);
}


////--------------------------------------------------------------------------------------//
////		CREATE CHARACTER
////--------------------------------------------------------------------------------------//
void CCharServerSession::RecvCreateCharReq(CNtlPacket * pPacket, CQueryServer* app)
{
	sCQ_CHARACTER_ADD_REQ * req = (sCQ_CHARACTER_ADD_REQ*)pPacket->GetPacketData();

	//check if char name already exist
	smart_ptr<QueryResult> namecheck = GetCharDB.Query("SELECT CharID FROM characters WHERE CharName='%ls'", req->awchCharName);
	if (namecheck)
	{
		CNtlPacket packet(sizeof(sQC_CHARACTER_ADD_RES));
		sQC_CHARACTER_ADD_RES * res = (sQC_CHARACTER_ADD_RES *)packet.GetPacketData();
		res->wOpCode = QC_CHARACTER_ADD_RES;
		res->accountId = req->accountId;
		res->wResultCode = CHARACTER_SAMENAME_EXIST;
		packet.SetPacketLen(sizeof(sQC_CHARACTER_ADD_RES));
		app->Send(GetHandle(), &packet);
	}
	else
	{
		CHARACTERID newCharId = g_pCharacterManager->AcquireCharID(); //TO GENERATE CHAR ID

		CNtlPacket packet(sizeof(sQC_CHARACTER_ADD_RES));
		sQC_CHARACTER_ADD_RES * res = (sQC_CHARACTER_ADD_RES *)packet.GetPacketData();
		res->wOpCode = QC_CHARACTER_ADD_RES;
		res->wResultCode = CHARACTER_SUCCESS;
		res->accountId = req->accountId;
		res->sPcDataSummary.charId = newCharId;
		wcscpy_s(res->sPcDataSummary.awchName, NTL_MAX_SIZE_CHAR_NAME + 1, req->awchCharName);
		res->sPcDataSummary.byRace = req->byRace;
		res->sPcDataSummary.byClass = req->byClass;
		res->sPcDataSummary.bIsAdult = false;
		res->sPcDataSummary.byGender = req->byGender;
		res->sPcDataSummary.byFace = req->byFace;
		res->sPcDataSummary.byHair = req->byHair;
		res->sPcDataSummary.byHairColor = req->byHairColor;
		res->sPcDataSummary.bySkinColor = req->bySkinColor;
		res->sPcDataSummary.byLevel = 1;
		res->sPcDataSummary.worldTblidx = req->bindWorldId;
		res->sPcDataSummary.worldId = req->bindWorldId;
		res->sPcDataSummary.fPositionX = req->vSpawn_Loc.x;
		res->sPcDataSummary.fPositionY = req->vSpawn_Loc.y;
		res->sPcDataSummary.fPositionZ = req->vSpawn_Loc.z;
		res->sPcDataSummary.dwMoney = 0;
		res->sPcDataSummary.dwMoneyBank = 0;

		memset(&res->sPcDataSummary.sItem, -1, sizeof(res->sPcDataSummary.sItem));

		for (int i = 0; i < req->byItemCount; i++)
		{
			if ((req->sItem[i].itemNo != 0 || req->sItem[i].itemNo != INVALID_TBLIDX) && req->sItem[i].byPlace == CONTAINER_TYPE_EQUIP)
			{
				res->sPcDataSummary.sItem[req->sItem[i].byPosition].byBattleAttribute = req->sItem[i].byBattleAttribute;
				res->sPcDataSummary.sItem[req->sItem[i].byPosition].byGrade = req->sItem[i].byGrade;
				res->sPcDataSummary.sItem[req->sItem[i].byPosition].byRank = req->sItem[i].byRank;
				res->sPcDataSummary.sItem[req->sItem[i].byPosition].tblidx = req->sItem[i].itemNo;
			}

			req->sItem[i].charId = newCharId;
			g_pItemManager->CreateItem(req->sItem[i]);
		}

		for (int i = 0; i < req->bySkillCount; i++)
		{
			GetCharDB.Execute("INSERT INTO skills (skill_id,owner_id,SlotID) VALUES (%u,%u,%u)", req->aSkill[i], newCharId, i);
		}

		for (int i = 0; i < 3; i++)
		{
			GetCharDB.Execute("INSERT INTO portals (CharID,Point) VALUES (%u,%u)", newCharId, req->defaultPortalId[i]);
		}

		GetCharDB.Execute("INSERT INTO bind (CharID,WorldID,LocX,LocY,LocZ,DirX,DirY,DirZ) VALUES (%u,%u,%f,%f,%f,%f,%f,%f)", 
			newCharId, req->bindWorldId, req->vBind_Loc.x, req->vBind_Loc.y, req->vBind_Loc.z, req->vBind_Dir.x, req->vBind_Dir.y, req->vBind_Dir.z);

		res->sPcDataSummary.dwMapInfoIndex = (DWORD)req->mapNameTblidx;
		res->sPcDataSummary.bTutorialFlag = false; //must change to false when tutorial works
		res->sPcDataSummary.bNeedNameChange = false;
		res->sPcDataSummary.bInvisibleCostume = false;
		res->sPcDataSummary.bySuperiorEffectType = req->bySuperiorType;
		res->sPcDataSummary.charTitle = INVALID_TBLIDX;
		res->sPcDataSummary.sDogi.byDojoColor = INVALID_BYTE;
		res->sPcDataSummary.sDogi.byGuildColor = INVALID_BYTE;
		res->sPcDataSummary.sDogi.byType = INVALID_BYTE;
		res->sPcDataSummary.sDogi.guildId = INVALID_GUILDID;
		res->sPcDataSummary.sMark.byMarkInColor = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkInLine = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkMain = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkMainColor = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkOutColor = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkOutLine = INVALID_BYTE;

		g_pCharacterManager->CreateCharacter(req->accountId, res->sPcDataSummary, req->serverId); //create the char in database

		packet.SetPacketLen(sizeof(sQC_CHARACTER_ADD_RES));
		app->Send(GetHandle(), &packet);
	}
}

void CCharServerSession::RecvCharacterDelReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sCQ_CHARACTER_DEL_REQ * req = (sCQ_CHARACTER_DEL_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		g_pPlayerCache->EraseCharacter(req->charID);
		delete pCache;
	}

	GetLogDB.Execute("INSERT INTO character_delete_log (AccountID, CharID) VALUES (%u, %u)", req->accountID, req->charID);

	GetCharDB.Execute("DELETE FROM characters WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM auctionhouse WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM bind WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM buffs WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM friendlist WHERE user_id=%u OR friend_id=%u", req->charID, req->charID);
	GetCharDB.Execute("DELETE FROM hoipoi_recipe WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM htb_skills WHERE owner_id=%u", req->charID);
	GetCharDB.Execute("DELETE FROM items WHERE owner_id=%u", req->charID);
	GetCharDB.Execute("DELETE FROM mail WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM mascots WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM portals WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM questitems WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM quests WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM quickslot WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM skills WHERE owner_id=%u", req->charID);
	GetCharDB.Execute("DELETE FROM titles WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM warfog WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM items_cd WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM quick_teleport WHERE CharID=%u", req->charID);
	GetCharDB.Execute("DELETE FROM rank_battle WHERE CharID=%u", req->charID);
}

void CCharServerSession::RecvCharacterLoadReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sCQ_CHARACTER_LOAD_REQ * req = (sCQ_CHARACTER_LOAD_REQ*)pPacket->GetPacketData();

	CAccountCache* pAccount = g_pPlayerCache->GetAccount(req->accountId);
	if (pAccount)
	{
		pAccount->SetSession(GetHandle());

		CNtlPacket pQry(sizeof(sQC_CHARACTER_LOAD_RES));
		sQC_CHARACTER_LOAD_RES * qRes = (sQC_CHARACTER_LOAD_RES *)pQry.GetPacketData();
		qRes->wOpCode = QC_CHARACTER_LOAD_RES;
		qRes->accountId = req->accountId;
		qRes->serverFarmId = req->serverFarmId;
		pQry.SetPacketLen(sizeof(sQC_CHARACTER_LOAD_RES));
		app->Send(GetHandle(), &pQry);
	}
	else
	{
		pAccount = new CAccountCache(req->accountId);

		pAccount->SetSession(GetHandle());

		SQLCallbackBase* pCallBack = new SQLClassCallbackP0<CAccountCache>(pAccount, &CAccountCache::OnLoadAccountInfo);
		AsyncQuery * q = new AsyncQuery(pCallBack);
		q->AddQuery("SELECT mallpoints, isGm, PremiumSlots, EventCoins, WaguCoins FROM accounts WHERE AccountID=%u", req->accountId);
		q->AddQuery("SELECT * FROM cashshop_storage WHERE AccountID=%u AND isMoved=0", req->accountId);
		q->AddQuery("SELECT ActionID, wKey FROM shortcuts WHERE AccountID=%u", req->accountId);
		GetAccDB.QueueAsyncQuery(q);

		g_pPlayerCache->InsertAccount(req->accountId, pAccount);
	}
}

