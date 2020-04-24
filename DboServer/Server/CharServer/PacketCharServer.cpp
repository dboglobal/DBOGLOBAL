#include "stdafx.h"
#include "CharServer.h"
#include "NtlPacketUC.h"
#include "NtlPacketCU.h"
#include "NtlPacketCM.h"
#include "NtlPacketCQ.h"
#include "NtlResultCode.h"
#include "NtlTeleport.h"
#include "Player.h"
#include "NewbieTable.h"
#include "ItemTable.h"



//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CClientSession::OnInvalid(CNtlPacket * pPacket)
{
	UNREFERENCED_PARAMETER(pPacket);
}


//--------------------------------------------------------------------------------------//
//		Log into Character Server
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharServerReq(CNtlPacket * pPacket)
{
	sUC_LOGIN_REQ * req = (sUC_LOGIN_REQ *)pPacket->GetPacketData();

	CCharServer* app = (CCharServer*)g_pApp;

	if (req->accountId == INVALID_ACCOUNTID)
	{
		this->Disconnect(false);
		return;
	}
	
	CPlayer* pPlayer = g_PlrMgr->CreatePlayer(req->accountId, this);
	if (pPlayer == NULL)
	{
		this->Disconnect(false);
		return;
	}

	pPlayer->SetServerFarmID(req->serverID);
	SetPlayer(pPlayer);

	//send to master server that user logged in and check auth key
	CNtlPacket packet(sizeof(sCM_LOGIN_REQ));
	sCM_LOGIN_REQ * res = (sCM_LOGIN_REQ *)packet.GetPacketData();
	res->wOpCode = CM_LOGIN_REQ;
	res->accountId = req->accountId;
	res->serverId = req->serverID;
	memcpy(res->abyAuthKey, req->abyAuthKey, sizeof(res->abyAuthKey));
	packet.SetPacketLen( sizeof(sCM_LOGIN_REQ) );
	app->SendTo( app->m_pMasterServerSession, &packet );
}


//--------------------------------------------------------------------------------------//
//		Load Characters
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharLoadReq(CNtlPacket * pPacket)
{
	sUC_CHARACTER_LOAD_REQ * req = (sUC_CHARACTER_LOAD_REQ*)pPacket->GetPacketData();

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	if (GetPlayer()->GetServerFarmID() != req->serverFarmId)
		GetPlayer()->SetServerChanged(true);

	if (req->accountId != GetPlayer()->GetAccountID())
	{
		ERR_LOG(LOG_USER, "req->accountId != this->accountID. Can not load characters.", req->accountId, GetPlayer()->GetAccountID());
		this->Disconnect(false);
		return;
	}

	CCharServer* app = (CCharServer*)g_pApp;

	GetPlayer()->SetServerFarmID(req->serverFarmId);

	CNtlPacket pQry(sizeof(sCQ_CHARACTER_LOAD_REQ));
	sCQ_CHARACTER_LOAD_REQ * qRes = (sCQ_CHARACTER_LOAD_REQ *)pQry.GetPacketData();
	qRes->wOpCode = CQ_CHARACTER_LOAD_REQ;
	qRes->accountId = req->accountId;
	qRes->serverFarmId = req->serverFarmId;
	pQry.SetPacketLen(sizeof(sCQ_CHARACTER_LOAD_REQ));
	app->SendTo(app->GetQueryServerSession(), &pQry);
}

//--------------------------------------------------------------------------------------//
//		Create Character
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharCreateReq(CNtlPacket * pPacket)
{
	sUC_CHARACTER_ADD_REQ * req = (sUC_CHARACTER_ADD_REQ*)pPacket->GetPacketData();

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}
	//printf("%u %u %u %u %u %u %u %u \n", req->byBlood, req->byClass, req->byFace, req->byGender, req->byHair, req->byHairColor, req->byRace, req->bySkinColor);
	CCharServer* app = (CCharServer*)g_pApp;

	//check & remove from queue
#ifdef USE_LOGIN_QUEUE_SYSTEM
	if (GetPlayer()->CheckQueue())
	{
		GetPlayer()->SetQueue(false);
		g_pQueueManager->RemoveFromQueue(GetPlayer()->GetAccountID());
	}
#endif

	//DO SOME CHECKS FIRST
	sNEWBIE_TBLDAT* NewbieTblData = NULL;
	WORD resultcode = CHARACTER_SUCCESS;
	char* chname = Ntl_WC2MB(req->awchCharName);
	std::string charname = chname;
	//printf("byRace:%u,byClass:%u,byFace:%u,byGender:%u,byHair:%u,byHairColor:%u,bySkinColor:%u \n", req->byRace, req->byClass, req->byFace, req->byGender, req->byHair, req->byHairColor, req->bySkinColor);
	if(req->byRace > RACE_LAST || req->byClass > PC_CLASS_1_LAST)
		resultcode = CHARACTER_FAIL;
	else if(req->byFace == 0 || req->byFace > DBO_CHAR_FACE_SHAPE_COUNT) //all races have 10 faces
		resultcode = CHARACTER_FAIL;
	else if (req->byHair == 0 || req->byHair > DBO_CHAR_HAIR_SHAPE_COUNT) //all races have 10 hairs
		resultcode = CHARACTER_FAIL;
	else if (req->byHairColor == 0)
		resultcode = CHARACTER_FAIL;
	else if (req->bySkinColor == 0 || req->bySkinColor > DBO_CHAR_SKIN_COLOR_COUNT) //all races have 5 hairs
		resultcode = CHARACTER_FAIL;
	else if (req->byRace == RACE_HUMAN)
	{
		if (req->byClass != PC_CLASS_HUMAN_FIGHTER && req->byClass != PC_CLASS_HUMAN_MYSTIC) //check if creating valid human class
			resultcode = CHARACTER_FAIL;
		else if (req->byHairColor > DBO_CHAR_HAIR_COLOR_COUNT) // human has more than 1 hair color
			resultcode = CHARACTER_FAIL;
	}
	else if (req->byRace == RACE_NAMEK)
	{
		if(req->byClass != PC_CLASS_NAMEK_FIGHTER && req->byClass != PC_CLASS_NAMEK_MYSTIC) //check if creating valid namekian class
			resultcode = CHARACTER_FAIL;
		else if (req->byHairColor != req->bySkinColor)
			resultcode = CHARACTER_FAIL; // nameks hair color is the same as skin color
	}
	else if (req->byRace == RACE_MAJIN)
	{
		if(req->byClass != PC_CLASS_MIGHTY_MAJIN && req->byClass != PC_CLASS_WONDER_MAJIN) //check if creating valid majin class
			resultcode = CHARACTER_FAIL;
		else if (req->byHairColor != req->bySkinColor)
			resultcode = CHARACTER_FAIL; // majin hair color is the same as skin color
	}
	else
	{
		resultcode = CHARACTER_FAIL;
	}

	if (resultcode == CHARACTER_SUCCESS)
	{
		NewbieTblData = (sNEWBIE_TBLDAT*)app->g_pTableContainer->GetNewbieTable()->GetNewbieTbldat(req->byRace, req->byClass);
		if (NewbieTblData == NULL)
			resultcode = CHARACTER_RACE_NOT_ALLOWED;
		else if (charname.length() < NTL_MIN_SIZE_CHAR_NAME)
			resultcode = CHARACTER_TOO_SHORT_NAME;
		else if (charname.length() > NTL_MAX_SIZE_CHAR_NAME)
			resultcode = CHARACTER_TOO_LONG_NAME;
		else if (charname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos)
			resultcode = CHARACTER_BLOCK_STRING_INCLUDED;
		else if (GetPlayer()->GetCharCount() > NTL_MAX_COUNT_USER_CHAR_SLOT)
			resultcode = CHARACTER_COUNT_OVER;
		else if (GetPlayer()->IsCharsLoaded() == false)
			resultcode = CHARACTER_FAIL;
	}

	if (resultcode == CHARACTER_SUCCESS)
	{
		CNtlPacket packet(sizeof(sCQ_CHARACTER_ADD_REQ));
		sCQ_CHARACTER_ADD_REQ * res = (sCQ_CHARACTER_ADD_REQ *)packet.GetPacketData();
		res->wOpCode = CQ_CHARACTER_ADD_REQ;
		res->accountId = GetPlayer()->GetAccountID();
		wcscpy_s(res->awchCharName, NTL_MAX_SIZE_CHAR_NAME + 1, req->awchCharName);
		res->byRace = req->byRace;
		res->byClass = req->byClass;
		res->byGender = req->byGender;
		res->byFace = req->byFace;
		res->byHair = req->byHair;
		res->byHairColor = req->byHairColor;
		res->bySkinColor = req->bySkinColor;
		res->byBlood = 0;
		NewbieTblData->vSpawn_Loc.CopyTo(res->vSpawn_Loc);
		NewbieTblData->vSpawn_Dir.CopyTo(res->vSpawn_Dir);
		res->byBindType = DBO_BIND_TYPE_POPO_STONE;
		res->bindWorldId = NewbieTblData->world_Id;
		NewbieTblData->vBind_Loc.CopyTo(res->vBind_Loc);
		NewbieTblData->vBind_Dir.CopyTo(res->vBind_Dir);
		res->mapNameTblidx = NewbieTblData->mapNameTblidx;


		for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
		{
			res->sItem[i].itemNo = INVALID_TBLIDX;

			if (NewbieTblData->aitem_Tblidx[i] == INVALID_TBLIDX)
				continue;

			sITEM_TBLDAT* ItemTblDat = (sITEM_TBLDAT*)app->g_pTableContainer->GetItemTable()->FindData(NewbieTblData->aitem_Tblidx[i]);
			if (ItemTblDat)
			{
				res->sItem[i].itemNo = ItemTblDat->tblidx;

				if (ItemTblDat->byEquip_Type >= EQUIP_TYPE_MAIN_WEAPON &&  ItemTblDat->byEquip_Type <= EQUIP_TYPE_ACCESSORY)
				{
					res->sItem[i].byPlace = CONTAINER_TYPE_EQUIP;
					res->sItem[i].byPosition = NewbieTblData->abyPos[i];

				}
				else if (ItemTblDat->byItem_Type == ITEM_TYPE_BAG)
				{
					res->sItem[i].byPlace = CONTAINER_TYPE_BAGSLOT;
					res->sItem[i].byPosition = 0;
				}
				else if (ItemTblDat->byItem_Type == ITEM_TYPE_WAREHOUSE)
				{
					res->sItem[i].byPlace = CONTAINER_TYPE_BANKSLOT;
					res->sItem[i].byPosition = 0;
				}
				else
				{
					res->sItem[i].byPlace = CONTAINER_TYPE_BAG1;
					res->sItem[i].byPosition = NewbieTblData->abyPos[i];
				}

				res->sItem[i].byStackcount = NewbieTblData->abyStack_Quantity[i];
				res->sItem[i].byRank = ItemTblDat->byRank;
				res->sItem[i].byCurrentDurability = ItemTblDat->byDurability;
				res->sItem[i].bNeedToIdentify = false;
				res->sItem[i].byBattleAttribute = ItemTblDat->byBattle_Attribute;
				res->sItem[i].byRestrictState = ItemTblDat->byRestrictType;
				res->sItem[i].sOptionSet.Init();
				res->sItem[i].byDurationType = ItemTblDat->byDurationType;

				res->byItemCount += 1;
			}
		}

		for (int i = 0; i < NTL_MAX_NEWBIE_SKILL; i++)
		{
			if (NewbieTblData->aSkillTblidx[i] != INVALID_TBLIDX)
			{
				res->aSkill[i] = NewbieTblData->aSkillTblidx[i];
				res->bySkillCount += 1;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			res->defaultPortalId[i] = NewbieTblData->defaultPortalId[i];
		}

		res->bySuperiorType = 0;
		res->bySuperiorValue = 0;
		res->serverId = GetPlayer()->GetServerFarmID();
		packet.SetPacketLen(sizeof(sCQ_CHARACTER_ADD_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}
	else
	{
		CNtlPacket packet(sizeof(sCU_CHARACTER_ADD_RES));
		sCU_CHARACTER_ADD_RES * res = (sCU_CHARACTER_ADD_RES *)packet.GetPacketData();
		res->wOpCode = CU_CHARACTER_ADD_RES;
		res->wResultCode = resultcode;
		packet.SetPacketLen(sizeof(sCU_CHARACTER_ADD_RES));
		app->Send(GetHandle(), &packet);
	}

	Ntl_CleanUpHeapString(chname);
}

//--------------------------------------------------------------------------------------//
//		Exit Character selection
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharExitReq(CNtlPacket * pPacket)
{
	CCharServer* app = (CCharServer*)g_pApp;
	sUC_CHARACTER_EXIT_REQ * req = (sUC_CHARACTER_EXIT_REQ*)pPacket->GetPacketData();

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	// req->byMoveType = 0 == MOVE TO AUTH SERVER
	// req->byMoveType = 2 == MOVE TO GAMESERVER

	CNtlPacket packet(sizeof(sCM_CHARACTER_EXIT_REQ));
	sCM_CHARACTER_EXIT_REQ * res = (sCM_CHARACTER_EXIT_REQ *)packet.GetPacketData();
	res->wOpCode = CM_CHARACTER_EXIT_REQ;
	res->accountId = GetPlayer()->GetAccountID();
	packet.SetPacketLen( sizeof(sCM_CHARACTER_EXIT_REQ) );
	app->SendTo(app->m_pMasterServerSession, &packet);

	if(req->byMoveType == 2 && GetPlayer()->IsServerChanged() && GetPlayer()->GetServerFarmID() != INVALID_SERVERFARMID) //if move type 2 (move to gameserver) and if serverid changed
	{
		GetAccDB.Execute("UPDATE accounts SET lastServerFarmId=%u WHERE AccountID=%u LIMIT 1", GetPlayer()->GetServerFarmID(), GetPlayer()->GetAccountID());
	}
}

//--------------------------------------------------------------------------------------//
//		Delete Character
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharDeleteReq(CNtlPacket * pPacket)
{
	sUC_CHARACTER_DEL_REQ * req = (sUC_CHARACTER_DEL_REQ*)pPacket->GetPacketData();

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	WORD resultcode = CHARACTER_SUCCESS;

	std::string strpassword = req->achPasswd;

	if(!GetPlayer()->IsCharsLoaded())
		resultcode = CHARACTER_FAIL;
	else if (strpassword.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos)
		resultcode = CHARACTER_BLOCK_STRING_INCLUDED;
	else if (GetPlayer()->HasCharacter(req->charId) == false)
	{
		ERR_LOG(LOG_USER, "Account: %u tried to delete character %u that he dont own !!", GetPlayer()->GetAccountID(), req->charId);
		resultcode = CHARACTER_FAIL;
	}
	else if (strlen((const char*)req->achPasswd) < NTL_MAX_SIZE_USERPW_ENCRYPT)
		resultcode = AUTH_TOO_LONG_PASSWORD;
	else
	{
		//check if del password is correct
		smart_ptr<QueryResult> delpwcheck = GetAccDB.Query("SELECT del_char_pw FROM accounts WHERE AccountID=%u LIMIT 1", GetPlayer()->GetAccountID());
		if (delpwcheck)
		{
			Field* delpw = delpwcheck->Fetch();

			if (0 != strcmp(delpw[0].GetString(), (const char*)req->achPasswd)) //check password
				resultcode = CHARACTER_DELETE_CHAR_FAIL_NOT_MATCH_CODE;
			else
			{
				smart_ptr<QueryResult> check = GetCharDB.Query("SELECT GameMaster, GuildID FROM characters WHERE CharID=%u AND AccountID=%u LIMIT 1", req->charId, GetPlayer()->GetAccountID());
				if (check)
				{
					Field* f = check->Fetch();
					if (f[1].GetUInt32() == 0)
					{
						time_t DelTime = time(0) + 43200;

						if(f[0].GetBool() == true)
							DelTime = time(0) + 60;

						if (GetCharDB.Execute("UPDATE characters SET DelCharTime=%I64u WHERE CharID=%u AND AccountID=%u LIMIT 1", DelTime, req->charId, GetPlayer()->GetAccountID()) == false)
							resultcode = CHARACTER_DELETE_CHAR_FAIL;
						else
						{
							GetPlayer()->SetCharDeleted(req->charId, DelTime);
						}
					}
					else resultcode = CHARACTER_DELETE_GUILD_LEADER_FAIL;
				}
				else resultcode = CHARACTER_DB_QUERY_FAIL;
			}
		}
		else resultcode = CHARACTER_DB_QUERY_FAIL;
	}

	CNtlPacket packet(sizeof(sCU_CHARACTER_DEL_RES));
	sCU_CHARACTER_DEL_RES * res = (sCU_CHARACTER_DEL_RES *)packet.GetPacketData();
	res->wOpCode = CU_CHARACTER_DEL_RES;
	res->charId = req->charId;
	res->wResultCode = resultcode;
	res->dwRemainTick = 43200 * 1000; //time into ms
	packet.SetPacketLen(sizeof(sCU_CHARACTER_DEL_RES));
	g_pApp->Send(GetHandle(), &packet);
}

//--------------------------------------------------------------------------------------//
//		Cancel char deletion
//--------------------------------------------------------------------------------------//
void CClientSession::SendCancelCharDeleteReq(CNtlPacket * pPacket)
{
	sUC_CHARACTER_DEL_CANCEL_REQ * req = (sUC_CHARACTER_DEL_CANCEL_REQ*)pPacket->GetPacketData();
	
	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	WORD resultcode = CHARACTER_SUCCESS;

	std::string strpassword = req->achPasswd;

	if (!GetPlayer()->IsCharsLoaded())
		resultcode = CHARACTER_FAIL;
	else if (strpassword.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos)
		resultcode = CHARACTER_BLOCK_STRING_INCLUDED;
	else if (GetPlayer()->HasDelPendingCharacter(req->charId) == false)
	{
		ERR_LOG(LOG_USER, "Account: %u tried to cancel deletion character %u that he dont own !!", GetPlayer()->GetAccountID(), req->charId);
		resultcode = CHARACTER_FAIL;
	}
	else if (strlen((const char*)req->achPasswd) < NTL_MAX_SIZE_USERPW_ENCRYPT)
		resultcode = AUTH_TOO_LONG_PASSWORD;
	else
	{
		//check if del password is correct
		smart_ptr<QueryResult> delpwcheck = GetAccDB.Query("SELECT del_char_pw FROM accounts WHERE AccountID=%u LIMIT 1", GetPlayer()->GetAccountID());
		if (delpwcheck)
		{
			Field* delpw = delpwcheck->Fetch();

			if (0 != strcmp(delpw[0].GetString(), (const char*)req->achPasswd)) //check password
				resultcode = CHARACTER_DELETE_CHAR_FAIL_NOT_MATCH_CODE;
			else
			{
				if (GetCharDB.Execute("UPDATE characters SET DelCharTime=0 WHERE CharID=%u AND AccountID=%u LIMIT 1", req->charId, GetPlayer()->GetAccountID()) == false)
					resultcode = CHARACTER_DB_QUERY_FAIL;
				else
				{
					GetPlayer()->UnsetCharDeleted(req->charId);
				}
			}
		}
		else resultcode = CHARACTER_DB_QUERY_FAIL;
	}

	CNtlPacket packet(sizeof(sCU_CHARACTER_DEL_CANCEL_RES));
	sCU_CHARACTER_DEL_CANCEL_RES * res = (sCU_CHARACTER_DEL_CANCEL_RES *)packet.GetPacketData();
	res->wOpCode = CU_CHARACTER_DEL_CANCEL_RES;
	res->charId = req->charId;
	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sCU_CHARACTER_DEL_CANCEL_RES));
	g_pApp->Send(GetHandle(), &packet);
}

//--------------------------------------------------------------------------------------//
//		Character Rename
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharRenameReq(CNtlPacket * pPacket)
{
	sUC_CHARACTER_RENAME_REQ * req = (sUC_CHARACTER_RENAME_REQ*)pPacket->GetPacketData();

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	CNtlPacket packet(sizeof(sCU_CHARACTER_RENAME_RES));
	sCU_CHARACTER_RENAME_RES * res = (sCU_CHARACTER_RENAME_RES *)packet.GetPacketData();
	res->wOpCode = CU_CHARACTER_RENAME_RES;
	res->charId = req->charId;
	res->wResultCode = CHARACTER_SUCCESS;

	char* chname = Ntl_WC2MB(req->awchCharName);
	std::string charname = chname;

	if (!GetPlayer()->IsCharsLoaded())
		res->wResultCode = CHARACTER_FAIL;
	else if (GetPlayer()->HasCharacter(req->charId) == false)
	{
		ERR_LOG(LOG_USER, "Account: %u tried to rename character %u that he dont own !!", GetPlayer()->GetAccountID(), req->charId);
		res->wResultCode = CHARACTER_FAIL;
	}
	else if (charname.length() < NTL_MIN_SIZE_CHAR_NAME)
		res->wResultCode = CHARACTER_TOO_SHORT_NAME;
	else if (charname.length() > NTL_MAX_SIZE_CHAR_NAME)
		res->wResultCode = CHARACTER_TOO_LONG_NAME;
	else if (charname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos)
		res->wResultCode = CHARACTER_BLOCK_STRING_INCLUDED;
	else
	{
		smart_ptr<QueryResult> check = GetCharDB.Query("SELECT NameChange FROM characters WHERE CharID=%u AND AccountID=%u LIMIT 1", req->charId, GetPlayer()->GetAccountID());
		if (check)
		{
			Field *f = check->Fetch();
			if (f[0].GetBool())
			{
				//check name if available
				smart_ptr<QueryResult> namecheck = GetCharDB.Query("SELECT CharID FROM characters WHERE CharName='%s' AND NameChange=1 LIMIT 1", charname.c_str());
				if (!namecheck)
				{
					GetCharDB.Execute("UPDATE characters SET CharName='%s', NameChange=0 WHERE CharID=%u LIMIT 1", charname.c_str(), req->charId);

					GetPlayer()->SetNewCharName(req->charId, req->awchCharName);
				}
				else res->wResultCode = CHARACTER_SAMENAME_EXIST;
			}
			else res->wResultCode = CHARACTER_FAIL;
		}
		else res->wResultCode = CHARACTER_DB_QUERY_FAIL;
	}

	packet.SetPacketLen(sizeof(sCU_CHARACTER_RENAME_RES));
	g_pApp->Send(GetHandle(), &packet);

	Ntl_CleanUpHeapString(chname);
}

//--------------------------------------------------------------------------------------//
//		Connection check and login queue function
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharWaitCheckReq(CNtlPacket * pPacket)
{
	sUC_CONNECT_WAIT_CHECK_REQ * req = (sUC_CONNECT_WAIT_CHECK_REQ*)pPacket->GetPacketData();

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	GetPlayer()->SetServerChannelID(req->serverChannelId);

	CCharServer* app = (CCharServer*)g_pApp;

	WORD wResultCode = CHARACTER_SUCCESS;

	sSERVER_CHANNEL_INFO* pChannel = g_pServerInfoManager->GetServerChannelInfo(GetPlayer()->GetServerFarmID(), req->serverChannelId);
	if (pChannel == NULL || pChannel->bIsVisible == false)
	{
		wResultCode = GAME_SERVER_LOCKED;
	}
	else if (pChannel->byServerStatus != DBO_SERVER_STATUS_UP)
	{
		wResultCode = GAME_SERVER_LOCKED;
	}
	else if (pChannel->dwLoad >= pChannel->dwMaxLoad && !GetPlayer()->IsGM()) //check if server full and check if is gm. Allow gm connect even when server is full!
	{
		wResultCode = GAME_SERVER_LOCKED;
	}

	if (!GetPlayer()->IsCharsLoaded())
		wResultCode = CHARACTER_FAIL;

	if (wResultCode != CHARACTER_SUCCESS)
	{
		CNtlPacket packet2(sizeof(sCU_CONNECT_WAIT_CHECK_RES));
		sCU_CONNECT_WAIT_CHECK_RES * res = (sCU_CONNECT_WAIT_CHECK_RES *)packet2.GetPacketData();
		res->wOpCode = CU_CONNECT_WAIT_CHECK_RES;
		res->wResultCode = wResultCode;
		packet2.SetPacketLen(sizeof(sCU_CONNECT_WAIT_CHECK_RES));
		app->Send(GetHandle(), &packet2);
		return;
	}

#ifdef USE_LOGIN_QUEUE_SYSTEM
	
		if (!GetPlayer()->CheckQueue())
		{
			CNtlPacket packet(sizeof(sCU_CONNECT_WAIT_COUNT_NFY));
			sCU_CONNECT_WAIT_COUNT_NFY * res = (sCU_CONNECT_WAIT_COUNT_NFY *)packet.GetPacketData();
			res->wOpCode = CU_CONNECT_WAIT_COUNT_NFY;
			res->dwCountWaiting = INVALID_DWORD;
			packet.SetPacketLen(sizeof(sCU_CONNECT_WAIT_COUNT_NFY));
			app->Send(GetHandle(), &packet);

			g_pQueueManager->InsertIntoQueue(GetPlayer());
			GetPlayer()->SetQueue(true);

			return;
		}
		else
		{
			ERR_LOG(LOG_USER, "User %u is already in queue..", GetPlayer()->GetAccountID());
		}
	
#else

	CNtlPacket packet(sizeof(sCU_CONNECT_WAIT_COUNT_NFY));
	sCU_CONNECT_WAIT_COUNT_NFY * res = (sCU_CONNECT_WAIT_COUNT_NFY *)packet.GetPacketData();
	res->wOpCode = CU_CONNECT_WAIT_COUNT_NFY;
	res->dwCountWaiting = 0;
	packet.SetPacketLen(sizeof(sCU_CONNECT_WAIT_COUNT_NFY));
	app->Send(GetHandle(), &packet);

#endif

}

//--------------------------------------------------------------------------------------//
//		Select character and login to game-server
//--------------------------------------------------------------------------------------//
void CClientSession::SendCharSelectReq(CNtlPacket * pPacket)
{
	sUC_CHARACTER_SELECT_REQ * req = (sUC_CHARACTER_SELECT_REQ*)pPacket->GetPacketData();
	
	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	GetPlayer()->SetServerChannelID(req->serverChannelId);

	WORD resultcode = CHARACTER_SUCCESS;

	CCharServer* app = (CCharServer*)g_pApp;

	if(GetPlayer()->GetServerFarmID() >= NTL_MAX_GAME_FARM_SERVER || GetPlayer()->GetServerChannelID() >= NTL_MAX_SERVER_CHANNEL_COUNT_IN_SERVER_FARM)
	{
		NTL_PRINT(PRINT_APP,"ACCOUNT %d SELECT WRONG SERVER/CHANNEL (%d / %d)", GetPlayer()->GetAccountID(), GetPlayer()->GetServerFarmID(), GetPlayer()->GetServerChannelID());
		resultcode = GAME_WRONG_SERVER_CHANNEL_HAS_BEEN_SPECIFIED;
	}
	else if (!GetPlayer()->IsCharsLoaded())
		resultcode = CHARACTER_FAIL;

#ifdef USE_LOGIN_QUEUE_SYSTEM
	//check if is in queue

	if(GetPlayer()->CheckQueue())
		resultcode = CHARACTER_USER_SHOULD_WAIT_FOR_CONNECT;

#endif 

	if (GetPlayer()->HasCharacter(req->charId) == false)
	{
		ERR_LOG(LOG_USER, "Account: %u tried to select character %u that he dont own !!", GetPlayer()->GetAccountID(), req->charId);
		resultcode = CHARACTER_FAIL;
	}
	else if(resultcode == CHARACTER_SUCCESS)
	{
		sSERVER_CHANNEL_INFO* pChannel = g_pServerInfoManager->GetServerChannelInfo(GetPlayer()->GetServerFarmID(), GetPlayer()->GetServerChannelID());
		if (pChannel)
		{
			if (pChannel->byServerStatus == DBO_SERVER_STATUS_UP && pChannel->bIsVisible == true && pChannel->bIsLocked == false)
			{
				if (strcmp(pChannel->sChannelBuff.szServerChannelName, "DEV") == 0 && GetPlayer()->IsGM()
					|| 
					strcmp(pChannel->sChannelBuff.szServerChannelName, "DEV") != 0)
				{
					if (pChannel->dwLoad < pChannel->dwMaxLoad || GetPlayer()->IsGM()) //check if server full and check if is gm. Allow gm connect even when server is full!
					{
						GetPlayer()->SetCharSelect(req->charId);

						CNtlPacket packet(sizeof(sCM_MOVE_REQ));
						sCM_MOVE_REQ * res = (sCM_MOVE_REQ *)packet.GetPacketData();
						res->wOpCode = CM_MOVE_REQ;
						res->accountId = GetPlayer()->GetAccountID();
						packet.SetPacketLen(sizeof(sCM_MOVE_REQ));
						app->SendTo(app->m_pMasterServerSession, &packet);

						//	NTL_PRINT(PRINT_APP, "Account %d Start log to game server %u \n", this->accountID, time(0));
					}
					else resultcode = GAME_CANNOT_CONNECT_TARGET_CHANNEL_FULL;
				}
				else resultcode = CHARACTER_SERVER_LOCKED;
			}
			else resultcode = CHARACTER_SERVER_LOCKED;
		}
		else resultcode = CHARACTER_SERVER_IS_UNDER_CONSTRUCTION;
	}
	
	
	if(resultcode != CHARACTER_SUCCESS)
	{
		CNtlPacket packet(sizeof(sCU_CHARACTER_SELECT_RES));
		sCU_CHARACTER_SELECT_RES * res = (sCU_CHARACTER_SELECT_RES *)packet.GetPacketData();
		res->wOpCode = CU_CHARACTER_SELECT_RES;
		res->wResultCode = resultcode;
		packet.SetPacketLen( sizeof(sCU_CHARACTER_SELECT_RES) );
		app->Send( GetHandle(), &packet );
	}
}

//--------------------------------------------------------------------------------------//
//		Cancel log in request
//--------------------------------------------------------------------------------------//
void CClientSession::SendCancelWaitReq(CNtlPacket * pPacket)
{
	sUC_CONNECT_WAIT_CANCEL_REQ * req = (sUC_CONNECT_WAIT_CANCEL_REQ*)pPacket->GetPacketData();

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	CNtlPacket packet(sizeof(sCU_CONNECT_WAIT_CANCEL_RES));
	sCU_CONNECT_WAIT_CANCEL_RES * res = (sCU_CONNECT_WAIT_CANCEL_RES *)packet.GetPacketData();
	res->wOpCode = CU_CONNECT_WAIT_CANCEL_RES;
	res->wResultCode = CHARACTER_SUCCESS;
	packet.SetPacketLen( sizeof(sCU_CONNECT_WAIT_CANCEL_RES) );
	g_pApp->Send(GetHandle(), &packet);

	GetPlayer()->SetServerChannelID(req->serverChannelId);

	//remove from queue
#ifdef USE_LOGIN_QUEUE_SYSTEM
	if (GetPlayer()->CheckQueue())
	{
		GetPlayer()->SetQueue(false);
		g_pQueueManager->RemoveFromQueue(GetPlayer()->GetAccountID());
	}
#endif
}


//--------------------------------------------------------------------------------------//
//		Send server list || only received when byLastServerFarmID == INVALID_SERVERFARMID
//--------------------------------------------------------------------------------------//
void CClientSession::SendServerListRes(CNtlPacket * pPacket)
{
	UNREFERENCED_PARAMETER(pPacket);

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	GetPlayer()->SetServerFarmID(INVALID_SERVERFARMID);

	// send farm info
	g_pServerInfoManager->LoadServerList(GetHandle()); //send info from all servers

	CNtlPacket packet2(sizeof(sCU_CHARACTER_SERVERLIST_RES));
	sCU_CHARACTER_SERVERLIST_RES * res2 = (sCU_CHARACTER_SERVERLIST_RES *)packet2.GetPacketData();
	res2->wOpCode = CU_CHARACTER_SERVERLIST_RES;
	res2->wResultCode = CHARACTER_SUCCESS;
	packet2.SetPacketLen( sizeof(sCU_CHARACTER_SERVERLIST_RES) );
	g_pApp->Send(GetHandle(), &packet2);
}



//--------------------------------------------------------------------------------------//
//		Load Server list one || only received when byLastServerFarmID != INVALID_SERVERFARMID
//--------------------------------------------------------------------------------------//
void CClientSession::LoadOneServerList(CNtlPacket * pPacket)
{
	UNREFERENCED_PARAMETER(pPacket);

	if (!GetPlayer() || !GetPlayer()->GetSession())
	{
		return;
	}

	// Load farm info
	LoadServerFarmInfo(); // load server info from byLastServerFarmID

	CNtlPacket packet(sizeof(sCU_CHARACTER_SERVERLIST_ONE_RES));
	sCU_CHARACTER_SERVERLIST_ONE_RES * res = (sCU_CHARACTER_SERVERLIST_ONE_RES *)packet.GetPacketData();
	res->wOpCode = CU_CHARACTER_SERVERLIST_ONE_RES;
	res->wResultCode = CHARACTER_SUCCESS;
	packet.SetPacketLen(sizeof(sCU_CHARACTER_SERVERLIST_ONE_RES));
	g_pApp->Send(GetHandle(), &packet);
}

//--------------------------------------------------------------------------------------//
//		Load Server Channel info
//--------------------------------------------------------------------------------------//
void CClientSession::LoadServerChannelInfo()
{
	BYTE byCount = 0;

	CNtlPacket packet(sizeof(sCU_SERVER_CHANNEL_INFO));
	sCU_SERVER_CHANNEL_INFO * res = (sCU_SERVER_CHANNEL_INFO *)packet.GetPacketData();
	res->wOpCode = CU_SERVER_CHANNEL_INFO;
	
	sSERVER_FARM_INFO* pFarm = g_pServerInfoManager->GetServerFarmInfo(GetPlayer()->GetServerFarmID());
	if (pFarm)
	{
	//	printf("Channels found %u \n", pFarm->mapGameServerChannelInfo.size());
		for (std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>::iterator it = pFarm->mapGameServerChannelInfo.begin(); it != pFarm->mapGameServerChannelInfo.end(); it++)
		{
			sSERVER_CHANNEL_INFO* pChannel = it->second;
			if (pChannel && pChannel->bIsVisible)
			{
		//		printf("LoadServerChannelInfo: server %u channel %u \n", pChannel->serverFarmId, pChannel->byServerChannelIndex);
				res->serverChannelInfo[byCount].serverFarmId = pChannel->serverFarmId;
				res->serverChannelInfo[byCount].byServerChannelIndex = pChannel->byServerChannelIndex;
				res->serverChannelInfo[byCount].dwLoad = DWORD((float)pChannel->dwLoad / (float)pChannel->dwMaxLoad * 100.f);
				res->serverChannelInfo[byCount].bIsVisible = pChannel->bIsVisible;
				res->serverChannelInfo[byCount].dwMaxLoad = 100;
				res->serverChannelInfo[byCount].byServerStatus = pChannel->byServerStatus;
				res->serverChannelInfo[byCount].bIsScrambleChannel = pChannel->bIsScrambleChannel;
				memcpy(&res->serverChannelInfo[byCount].sChannelBuff, &pChannel->sChannelBuff, sizeof(_CHANNEL_BUFF));
				res->serverChannelInfo[byCount].bUnknown = pChannel->bUnknown;
				byCount++;
			}
		}
	}

	res->byCount = byCount;
	packet.SetPacketLen(sizeof(sCU_SERVER_CHANNEL_INFO));
	g_pApp->Send(GetHandle(), &packet);
}


//--------------------------------------------------------------------------------------//
//		Send farm server info from current server
//--------------------------------------------------------------------------------------//
void CClientSession::LoadServerFarmInfo()
{
	if (GetPlayer()->GetServerFarmID() == INVALID_SERVERFARMID)
	{
		g_pServerInfoManager->LoadServerList(GetHandle());

		ERR_LOG(LOG_USER, "GetPlayer()->GetServerFarmID() == INVALID_SERVERFARMID. Load all server info..");
		return;
	}

	sSERVER_FARM_INFO* pFarm = g_pServerInfoManager->GetServerFarmInfo(GetPlayer()->GetServerFarmID());
	if (pFarm)
	{
		CNtlPacket packet(sizeof(sCU_SERVER_FARM_INFO));
		sCU_SERVER_FARM_INFO * res = (sCU_SERVER_FARM_INFO *)packet.GetPacketData();
		res->wOpCode = CU_SERVER_FARM_INFO;
		res->serverFarmInfo.serverFarmId = pFarm->serverFarmId;
		wcscpy_s(res->serverFarmInfo.wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1, pFarm->wszGameServerFarmName);
		res->serverFarmInfo.byServerStatus = pFarm->byServerStatus;
		res->serverFarmInfo.dwLoad = DWORD((float)pFarm->dwLoad / (float)pFarm->dwMaxLoad * 100.f);
		res->serverFarmInfo.dwMaxLoad = 100;
		packet.SetPacketLen(sizeof(sCU_SERVER_FARM_INFO));
		g_pApp->Send(GetHandle(), &packet);
	}
}

void CClientSession::LoadAllServerFarmInfo()
{
	sSERVER_FARM_INFO* pFarm = g_pServerInfoManager->GetServerFarmInfo(GetPlayer()->GetServerFarmID());
	if (pFarm)
	{
		CNtlPacket packet(sizeof(sCU_SERVER_FARM_INFO));
		sCU_SERVER_FARM_INFO * res = (sCU_SERVER_FARM_INFO *)packet.GetPacketData();
		res->wOpCode = CU_SERVER_FARM_INFO;
		res->serverFarmInfo.serverFarmId = pFarm->serverFarmId;
		wcscpy_s(res->serverFarmInfo.wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1, pFarm->wszGameServerFarmName);
		res->serverFarmInfo.byServerStatus = pFarm->byServerStatus;
		res->serverFarmInfo.dwLoad = DWORD((float)pFarm->dwLoad / (float)pFarm->dwMaxLoad * 100.f);
		res->serverFarmInfo.dwMaxLoad = 100;
		packet.SetPacketLen(sizeof(sCU_SERVER_FARM_INFO));
		g_pApp->Send(GetHandle(), &packet);
	}
}
