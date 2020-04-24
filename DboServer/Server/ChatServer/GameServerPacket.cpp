#include "stdafx.h"
#include "ChatServer.h"
#include "NtlPacketTQ.h"
#include "NtlPacketTU.h"
#include "AuctionHouse.h"
#include "DojoManager.h"
#include "Party.h"
#include "GuildManager.h"
#include "NtlPacketGT.h"
#include "NtlPacketTG.h"
#include "NtlResultCode.h"
#include "DojoWar.h"


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::OnInvalid(CNtlPacket * pPacket)
{
}


//--------------------------------------------------------------------------------------//
//		RECEIVE GAME INFO FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecGameInfo(CNtlPacket * pPacket)
{
	sGT_GAME_INFO * req = (sGT_GAME_INFO *)pPacket->GetPacketData();

	SetChannelID(req->serverChannelId);
	g_pServerInfoManager->AddGameServerChannel(req->serverChannelId, this->GetHandle());

}

//--------------------------------------------------------------------------------------//
//		RECEIVE WORLD CREATE FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecWorldCreateNfy(CNtlPacket * pPacket)
{

}

//--------------------------------------------------------------------------------------//
//		RECEIVE WORLD DESTROY FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecWorldDestroyNfy(CNtlPacket * pPacket)
{

}

//--------------------------------------------------------------------------------------//
//		RECEIVE USER ENTER GAME FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecUserEnterGame(CNtlPacket * pPacket)
{
	sGT_USER_ENTER_GAME * req = (sGT_USER_ENTER_GAME *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	
	if(pPlayer == NULL)
		pPlayer = g_pPlayerManager->CreatePlayer(req->accountId);

	pPlayer->SetCharID(req->charId);
	pPlayer->SetAccountID(req->accountId);
	pPlayer->SetClass(req->byClass);
	pPlayer->SetLevel(req->byLevel);
	pPlayer->SetRace(req->byRace);
	pPlayer->SetReputation(req->dwReputation);
	pPlayer->SetID(req->hHandle);
	pPlayer->SetCharName(req->awchName);
	pPlayer->SetWorldID(req->worldId);
	pPlayer->SetLocation(req->vCurrentPosition);
	pPlayer->SetGuildID(req->guildId);
	pPlayer->SetMapNameTblidx(req->mapNameTblidx);
	pPlayer->SetWaguMachineCoin(req->wWaguWaguCoin);
	pPlayer->SetEventMachineCoin(req->wEventCoin);

	pPlayer->SetChannel(req->channelId);
	pPlayer->SetWaguWaguPoints(req->dwWaguPoints);


	if (req->bForSyncCommunity) //I guess only when we sync. Otherwise it wouldnt be required
	{
		CNtlPacket packet(sizeof(sTG_USER_ENTER_GAME_ACK));
		sTG_USER_ENTER_GAME_ACK * res = (sTG_USER_ENTER_GAME_ACK *)packet.GetPacketData();
		res->wOpCode = TG_USER_ENTER_GAME_ACK;
		res->wResultCode = GAME_SUCCESS;
		res->accountId = req->accountId;
		res->bForSyncCommunity = req->bForSyncCommunity;

		g_pServerInfoManager->GenerateAuthKey(res->abyAuthKey, req->accountId);

		packet.SetPacketLen(sizeof(sTG_USER_ENTER_GAME_ACK));
		g_pApp->Send(GetHandle(), &packet);
	}

}

//--------------------------------------------------------------------------------------//
//		RECEIVE USER LEAVE GAME FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecUserLeaveGame(CNtlPacket * pPacket)
{
	sGT_USER_LEAVE_GAME * req = (sGT_USER_LEAVE_GAME *)pPacket->GetPacketData();
	
	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	if (pPlayer && pPlayer->GetClientSession())
	{
		CDojo* pDojo = g_pDojoManager->GetDojo(pPlayer->GetGuildID());
		if (pDojo)
		{
			if (pDojo->GetWarRequest() && pDojo->GetWarInProgress() == false && pDojo->GetDojoData()->challengeGuildId != INVALID_GUILDID)
				pDojo->BeginWarPreparation();
		}

		if (req->bIsKickOut || req->eCharLeavingType != CHARLEAVING_CHANNEL_CHANGE)
		{
			CClientSession* pSession = pPlayer->GetClientSession();
			g_pPlayerManager->RemovePlayer(req->accountId);

			pSession->Disconnect(false);
		}
		else
		{
			pPlayer->SetChannel(req->destGameServerChannelID);
			pPlayer->SetReadyForCommunityServer(false);
			pPlayer->SetCanLoadFromDB(true);
		}
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE USER AUTH KEY GAME SERVER
//--------------------------------------------------------------------------------------//
void CServerPassiveSession::RecvUserAuthKeyCreatedNfy(CNtlPacket * pPacket)
{
	sGT_USER_AUTH_KEY_CREATED_NFY * req = (sGT_USER_AUTH_KEY_CREATED_NFY *)pPacket->GetPacketData();

	g_pServerInfoManager->StoreAuthKey(req->abyAuthKey, req->accountId);

	CNtlPacket packet(sizeof(sTG_USER_AUTH_KEY_CREATED_ACK));
	sTG_USER_AUTH_KEY_CREATED_ACK * res = (sTG_USER_AUTH_KEY_CREATED_ACK *)packet.GetPacketData();
	res->wOpCode = TG_USER_AUTH_KEY_CREATED_ACK;
	res->wResultCode = GAME_SUCCESS;
	res->accountId = req->accountId;
	memcpy(res->abyAuthKey, req->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
	packet.SetPacketLen(sizeof(sTG_USER_AUTH_KEY_CREATED_ACK));
	g_pApp->Send(GetHandle(), &packet);
}


//--------------------------------------------------------------------------------------//
//		RECEIVE PC POS FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecUpdatePcPos(CNtlPacket * pPacket)
{
	sGT_UPDATE_PLAYER_POSITION * req = (sGT_UPDATE_PLAYER_POSITION *)pPacket->GetPacketData();
	
	CPlayer* player = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	if(player)
	{
		player->SetWorldID(req->worldId);
		player->SetLocation(req->vCurrentPosition);

		//if map name changed then send to friend, guild & store 
		if (player->GetMapNameTblidx() != req->mapNameTblidx)
		{
			player->SetMapNameTblidx(req->mapNameTblidx);

			player->UpdateFriendInfo(eFRIEND_CHANGE_TYPE_POSITION, req->mapNameTblidx);

			if (player->GetGuild())
			{
				player->GetGuild()->UpdateMemberInfoMapName(player->GetCharID(), req->mapNameTblidx);
			}
		}
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE PC CLASS FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecUpdatePcClass(CNtlPacket * pPacket)
{
	sGT_UPDATE_PLAYER_CLASS * req = (sGT_UPDATE_PLAYER_CLASS *)pPacket->GetPacketData();
	
	CPlayer* player = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	if(player)
	{
		player->SetClass(req->byNewClass);

		//send to friend
		player->UpdateFriendInfo(eFRIEND_CHANGE_TYPE_CLASS,req->byNewClass);

		//send to guild
		if(player->GetGuild())
		{
			player->GetGuild()->UpdateMemberInfoClass(player->GetCharID(), req->byNewClass);
		}
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE PC LEVEL FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecUpdatePcLevel(CNtlPacket * pPacket)
{
	sGT_UPDATE_PLAYER_LEVEL * req = (sGT_UPDATE_PLAYER_LEVEL *)pPacket->GetPacketData();
	
	CPlayer* player = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	if(player)
	{
		player->SetLevel(req->byNewLevel);

		//send to friend
		player->UpdateFriendInfo(eFRIEND_CHANGE_TYPE_LEVEL,req->byNewLevel);

		//send to guild
		if(player->GetGuild())
		{
			player->GetGuild()->UpdateMemberInfoLevel(player->GetCharID(), req->byNewLevel);
		}
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE PC REPUTATION FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecUpdatePcReputation(CNtlPacket * pPacket)
{
	sGT_UPDATE_PLAYER_REPUTATION * req = (sGT_UPDATE_PLAYER_REPUTATION *)pPacket->GetPacketData();
	
	CPlayer* player = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	if(player)
	{
		player->SetReputation(req->dwNewReputation);

		//send to guild
		if(player->GetGuild())
		{
			player->GetGuild()->UpdateMemberInfoReputation(player->GetCharID(), req->dwNewReputation);
		}
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE PC PUNISH FROM GAME SERVER
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecUpdatePcPunish(CNtlPacket * pPacket)
{
	sGT_UPDATE_PUNISH * req = (sGT_UPDATE_PUNISH *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;

	CPlayer* player = g_pPlayerManager->FindPlayerByName(req->awchCharName);
	if (player)
	{
		if (req->dwDurationInMinute > 0) //check if we mute
		{
			player->SetMuteDuration(req->dwDurationInMinute * 60 * 1000); //convert minutes to milliseconds

			CNtlPacket packet(sizeof(sTQ_MUTE_PLAYER_NFY));
			sTQ_MUTE_PLAYER_NFY * res = (sTQ_MUTE_PLAYER_NFY *)packet.GetPacketData();
			res->wOpCode = TQ_MUTE_PLAYER_NFY;
			res->accountId = req->accountId;
			res->dwDurationInMinute = req->dwDurationInMinute;
			NTL_SAFE_WCSCPY(res->awchGmCharName, req->awchGmCharName);
			NTL_SAFE_WCSCPY(res->awchCharName, req->awchCharName);
			NTL_SAFE_WCSCPY(res->wchReason, req->wchReason);
			res->charId = player->GetCharID();
			packet.SetPacketLen(sizeof(sTQ_MUTE_PLAYER_NFY));
			app->SendTo(app->GetQueryServerSession(), &packet);
		}
		else // unmute
		{
			player->SetMuteDuration(0);
			player->Unmute();
		}
	}
	else
	{
		if (req->dwDurationInMinute > 0) //check if we mute
		{
			CNtlPacket packet(sizeof(sTQ_MUTE_PLAYER_NFY));
			sTQ_MUTE_PLAYER_NFY * res = (sTQ_MUTE_PLAYER_NFY *)packet.GetPacketData();
			res->wOpCode = TQ_MUTE_PLAYER_NFY;
			res->accountId = req->accountId;
			res->dwDurationInMinute = req->dwDurationInMinute;
			NTL_SAFE_WCSCPY(res->awchGmCharName, req->awchGmCharName);
			NTL_SAFE_WCSCPY(res->awchCharName, req->awchCharName);
			NTL_SAFE_WCSCPY(res->wchReason, req->wchReason);
			res->charId = INVALID_CHARACTERID;
			packet.SetPacketLen(sizeof(sTQ_MUTE_PLAYER_NFY));
			app->SendTo(app->GetQueryServerSession(), &packet);
		}
	}
}


//--------------------------------------------------------------------------------------//
//		SWITCH GAMESERVER CHANNEL
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecvConnectChannelChangeReq(CNtlPacket * pPacket)
{
	sGT_CONNECT_CHANNEL_CHANGE_REQ * req = (sGT_CONNECT_CHANNEL_CHANGE_REQ *)pPacket->GetPacketData();

	WORD wResultcode = GAME_SUCCESS;


	//Currently disabled because we do not send game server data to the chat server yet
	/*sSERVER_CHANNEL_INFO* pChannel = g_pServerInfoManager->GetServerChannelInfo(req->gameServerIndex, req->gameServerChannelID);
	if (pChannel == NULL)
		wResultcode = GAME_WRONG_SERVER_CHANNEL_HAS_BEEN_SPECIFIED;
	else if (pChannel->byServerStatus != DBO_SERVER_STATUS_UP)
		wResultcode = GAME_SERVER_LOCKED;
	else if (pChannel->dwLoad >= pChannel->dwMaxLoad)
		wResultcode = GAME_CANNOT_CONNECT_TARGET_CHANNEL_FULL;
	else
	{*/

		CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithAccID(req->accountID);
		if (pPlayer == NULL || pPlayer->GetPcInitState() == false)
			wResultcode = GAME_CANNOT_CONNECT_DUE_TO_INVAlID_CHARACTER;
		else if(pPlayer->GetCharID() != req->characterID)
			wResultcode = GAME_CANNOT_CONNECT_DUE_TO_INVAlID_CHARACTER;
		else
		{
			pPlayer->SetChannel(req->gameServerChannelID); //set new channel id
			pPlayer->SetReadyForCommunityServer(false);
			pPlayer->SetCanLoadFromDB(true);
		}

	//}
	
	//send request

	CNtlPacket packet(sizeof(sTG_CONNECT_CHANNEL_CHANGE_RES));
	sTG_CONNECT_CHANNEL_CHANGE_RES * res = (sTG_CONNECT_CHANNEL_CHANGE_RES *)packet.GetPacketData();
	res->wOpCode = TG_CONNECT_CHANNEL_CHANGE_RES;
	res->accountID = req->accountID;
	res->characterID = req->characterID;
	res->hObject = req->hObject;
	res->wResultCode = wResultcode;
	packet.SetPacketLen(sizeof(sTG_CONNECT_CHANNEL_CHANGE_RES));
	g_pApp->Send(GetHandle(), &packet);
}



//--------------------------------------------------------------------------------------//
//		RECEIVE PARTY CREATED
//--------------------------------------------------------------------------------------//
void CServerPassiveSession::RecvPartyCreated(CNtlPacket * pPacket)
{
	sGT_PARTY_CREATED * req = (sGT_PARTY_CREATED *)pPacket->GetPacketData();

	g_pPartyManager->CreateParty(req->partyId, req->leaderCharId);
}


//--------------------------------------------------------------------------------------//
//		RECEIVE PARTY DISBANDED
//--------------------------------------------------------------------------------------//
void CServerPassiveSession::RecvPartyDisbanded(CNtlPacket * pPacket)
{
	sGT_PARTY_DISBANDED * req = (sGT_PARTY_DISBANDED *)pPacket->GetPacketData();

	g_pPartyManager->DisbandParty(req->partyId);
}


//--------------------------------------------------------------------------------------//
//		RECEIVE USER JOIN PARTY
//--------------------------------------------------------------------------------------//
void CServerPassiveSession::RecvPartyMemberJoined(CNtlPacket * pPacket)
{
	sGT_PARTY_MEMBER_JOINED * req = (sGT_PARTY_MEMBER_JOINED *)pPacket->GetPacketData();

	g_pPartyManager->AddMember(req->partyId, req->memberCharId);
}

//--------------------------------------------------------------------------------------//
//		RECEIVE USER LEAVE PARTY
//--------------------------------------------------------------------------------------//
void CServerPassiveSession::RecvPartyMemberLeft(CNtlPacket * pPacket)
{
	sGT_PARTY_MEMBER_LEFT * req = (sGT_PARTY_MEMBER_LEFT *)pPacket->GetPacketData();

	g_pPartyManager->RemoveMember(req->partyId, req->memberCharId);
}

//--------------------------------------------------------------------------------------//
//		RECEIVE PARTY LEADER CHANGED NFY
//--------------------------------------------------------------------------------------//
void CServerPassiveSession::RecvPartyLeaderChanged(CNtlPacket * pPacket)
{
	sGT_PARTY_LEADER_CHANGED * req = (sGT_PARTY_LEADER_CHANGED *)pPacket->GetPacketData();

	g_pPartyManager->ChangePartyLeader(req->partyId, req->newLeaderCharId);
}


//--------------------------------------------------------------------------------------//
//		RECEIVE USER CREATE GUILD
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecPcCreateGuild(CNtlPacket * pPacket)
{
	sGT_GUILD_CREATE * req = (sGT_GUILD_CREATE *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;
	WORD wResultcode = GAME_SUCCESS;
	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->creatorCharId);

	if (pPlayer == NULL || pPlayer->GetPcInitState() == false)
		wResultcode = GAME_FAIL;
	else if (pPlayer->GetPartyID() == INVALID_PARTYID)
		wResultcode = GAME_PARTY_YOU_ARE_NOT_IN_PARTY;
	else
	{
		smart_ptr<QueryResult> namecheck = GetCharDB.Query("SELECT GuildID FROM guilds WHERE GuildName= \"%ls\" ", req->wszGuildName);
		if (namecheck)
			wResultcode = 2107; // GAME_GUILD_SAME_GUILD_NAME_EXIST
		else if (CParty* pParty = g_pPartyManager->GetParty(pPlayer->GetPartyID()))
		{
			CNtlPacket packet(sizeof(sTQ_GUILD_CREATE_REQ));
			sTQ_GUILD_CREATE_REQ * res = (sTQ_GUILD_CREATE_REQ *)packet.GetPacketData();
			res->wOpCode = TQ_GUILD_CREATE_REQ;
			res->byServerChannelIndex = pPlayer->GetChannel();
			NTL_SAFE_WCSCPY(res->wszGuildName, req->wszGuildName);
			res->qwGuildFunctionFlag = 0x01 << DBO_GUILD_FUNCTION_MAX_MEMBER_30 | DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_1 | DBO_GUILD_FUNCTION_NOTICE_BOARD | DBO_GUILD_FUNCTION_POST | DBO_GUILD_FUNCTION_WAREHOUSE;
			res->dwRequiredZenny = DBO_ZENNY_FOR_NEW_GUILD;
			res->creatorAccountId = pPlayer->GetAccountID();
			res->partyId = pPlayer->GetPartyID();
			res->memberCharId[res->byMemberCount++] = pPlayer->GetCharID();
			pParty->FillMembersCharID(res->memberCharId, res->byMemberCount, pPlayer->GetCharID());
			packet.SetPacketLen(sizeof(sTQ_GUILD_CREATE_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);

			return;
		}
		else wResultcode = GAME_PARTY_YOU_ARE_NOT_IN_PARTY;
	}
	
	CNtlPacket gamePacket0(sizeof(sTG_GUILD_CREATE_RES));
	sTG_GUILD_CREATE_RES * gameRes0 = (sTG_GUILD_CREATE_RES *)gamePacket0.GetPacketData();
	gameRes0->wOpCode = TG_GUILD_CREATE_RES;
	gameRes0->creatorCharId = req->creatorCharId;
	gameRes0->wResultCode = wResultcode;
	gamePacket0.SetPacketLen(sizeof(sTG_GUILD_CREATE_RES));
	app->Send(GetHandle(), &gamePacket0);
}
//--------------------------------------------------------------------------------------//
//		RECEIVE USER INVITE GUILD
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecPcInviteGuild(CNtlPacket * pPacket)
{
	sGT_GUILD_INVITE_REQ * req = (sGT_GUILD_INVITE_REQ *)pPacket->GetPacketData();

	CPlayer* invitor = g_pPlayerManager->FindPlayerWithCharID(req->invitorCharId);
	CPlayer* target = g_pPlayerManager->FindPlayerWithCharID(req->targetCharId);

	WORD rescode = GAME_SUCCESS;

	if (target && target->GetPcInitState() && invitor && invitor->GetPcInitState())
	{
		if(target->GetGuild() || target->GetGuildID () != 0)
			rescode = COMMUNITY_GUILD_TARGET_IS_ALREADY_IN_A_GUILD;
		else if (target->GetGuildInvitedByID() != INVALID_CHARACTERID)
			rescode = COMMUNITY_GUILD_TARGET_ALREADY_HAS_INVITATION;
		else if (invitor->GetGuild() == NULL || invitor->GetGuildID() == 0)
			rescode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;
		else if (invitor->GetGuild()->GetTotalMemberCount() >= Dbo_GetMaxGuildMemberCount(invitor->GetGuild()->GetInfo()->qwGuildFunctionFlag))
			rescode = COMMUNITY_GUILD_MAX_NUMBER_OF_MEMBERS;
		else
		{
			target->SetGuildInvitedByID(req->invitorCharId);

			//send request
			CNtlPacket packet(sizeof(sTU_GUILD_INVITED_NFY));
			sTU_GUILD_INVITED_NFY * res = (sTU_GUILD_INVITED_NFY *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_INVITED_NFY;
			NTL_SAFE_WCSCPY(res->wszGuildName, invitor->GetGuild()->GetGuildName());
			packet.SetPacketLen( sizeof(sTU_GUILD_INVITED_NFY) );
			g_pApp->Send(target->GetClientSessionID(), &packet);
		}
	}
	else rescode = GAME_TARGET_NOT_FOUND;

	CNtlPacket gPacket(sizeof(sTG_GUILD_INVITE_RES));
	sTG_GUILD_INVITE_RES * gRes = (sTG_GUILD_INVITE_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_GUILD_INVITE_RES;
	gRes->invitorCharId = req->invitorCharId;
	NTL_SAFE_WCSCPY(gRes->wszTargetName, req->wszTargetName);
	gRes->wResultCode = rescode;
	gPacket.SetPacketLen(sizeof(sTG_GUILD_INVITE_RES));
	g_pApp->Send(GetHandle(), &gPacket);
}

void CServerPassiveSession::RecvSystemDisplayText(CNtlPacket * pPacket)
{
	sGT_SYSTEM_DISPLAY_TEXT * req = (sGT_SYSTEM_DISPLAY_TEXT *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;

	CNtlPacket packet(sizeof(sTU_SYSTEM_DISPLAY_TEXT));
	sTU_SYSTEM_DISPLAY_TEXT * res = (sTU_SYSTEM_DISPLAY_TEXT *)packet.GetPacketData();
	res->wOpCode = TU_SYSTEM_DISPLAY_TEXT;
	res->byDisplayType = req->byDisplayType;
	res->wMessageLengthInUnicode = (WORD)wcslen(req->wszMessage);
	NTL_SAFE_WCSCPY(res->wszMessage, req->wszMessage);
	packet.SetPacketLen(sizeof(sTU_SYSTEM_DISPLAY_TEXT));
	g_pPlayerManager->SendMsgToAll(&packet);
}


//--------------------------------------------------------------------------------------//
//		RECEIVE USER GIVE ZENNY TO GUILD
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecGuildGiveZenny(CNtlPacket * pPacket)
{
	sGT_GUILD_GIVE_ZENNY_REQ * req = (sGT_GUILD_GIVE_ZENNY_REQ *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if(player)
	{
		if (player->GetGuild() == NULL)
			resultcode = GAME_GUILD_NO_GUILD_FOUND;
		else
		{
			CNtlPacket qPacket(sizeof(sTQ_GUILD_GIVE_ZENNY_REQ));
			sTQ_GUILD_GIVE_ZENNY_REQ * qRes = (sTQ_GUILD_GIVE_ZENNY_REQ *)qPacket.GetPacketData();
			qRes->wOpCode = TQ_GUILD_GIVE_ZENNY_REQ;
			qRes->charId = req->charId;
			qRes->dwZenny = req->dwZenny;
			qRes->byServerChannelIndex = player->GetChannel();
			qRes->byServerIndex = 0;
			qRes->guildId = player->GetGuildID();
			qPacket.SetPacketLen(sizeof(sTQ_GUILD_GIVE_ZENNY_REQ));
			app->SendTo(app->GetQueryServerSession(), &qPacket);

			return;
		}
	}
	else resultcode = GAME_FAIL;

	CNtlPacket gPacket(sizeof(sTG_GUILD_GIVE_ZENNY_RES));
	sTG_GUILD_GIVE_ZENNY_RES * gRes = (sTG_GUILD_GIVE_ZENNY_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_GUILD_GIVE_ZENNY_RES;
	gRes->wResultCode = resultcode;
	gRes->charId = req->charId;
	gRes->dwZenny = req->dwZenny;
	gPacket.SetPacketLen(sizeof(sTG_GUILD_GIVE_ZENNY_RES));
	g_pApp->Send(GetHandle(), &gPacket);
}

//--------------------------------------------------------------------------------------//
//		RECEIVE ADD FUNCTION REQUEST
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecGuildFunctionAddReq(CNtlPacket * pPacket)
{
	sGT_GUILD_FUNCTION_ADD_REQ * req = (sGT_GUILD_FUNCTION_ADD_REQ *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->masterCharId);
	if (player)
	{
		CGuild* pGuild = player->GetGuild();

		if (pGuild == NULL)
			resultcode = GAME_GUILD_NO_GUILD_FOUND;
		else
		{
			eDBO_GUILD_FUNCTION eGuildFunction = static_cast<eDBO_GUILD_FUNCTION>(req->byFunction);

			if (pGuild->IsGuildMaster(req->masterCharId) == false)
				resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
			else if (Dbo_CanGetGuildFunction(pGuild->GetInfo()->qwGuildFunctionFlag, pGuild->GetInfo()->dwGuildReputation, req->dwZenny, eGuildFunction, &resultcode) == false)
				;
			else
			{
				CNtlPacket packet(sizeof(sTQ_GUILD_FUNCTION_ADD_REQ));
				sTQ_GUILD_FUNCTION_ADD_REQ * res = (sTQ_GUILD_FUNCTION_ADD_REQ *)packet.GetPacketData();
				res->wOpCode = TQ_GUILD_FUNCTION_ADD_REQ;
				res->byServerChannelIndex = player->GetChannel();
				res->byFunction = req->byFunction;
				res->byServerIndex = 0;
				res->dwZenny = req->dwZenny;
				res->guildId = player->GetGuildID();
				res->masterCharId = req->masterCharId;
				res->qwGuildFunctionFlag = AddGuildFunction(pGuild->GetInfo()->qwGuildFunctionFlag, req->byFunction);
				packet.SetPacketLen(sizeof(sTQ_GUILD_FUNCTION_ADD_REQ));
				app->SendTo(app->GetQueryServerSession(), &packet);

				return;
			}
		}
	}
	else resultcode = GAME_FAIL;

	//send to gameserver
	CNtlPacket gPacket(sizeof(sTG_GUILD_FUNCTION_ADD_RES));
	sTG_GUILD_FUNCTION_ADD_RES * gRes = (sTG_GUILD_FUNCTION_ADD_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_GUILD_FUNCTION_ADD_RES;
	gRes->wResultCode = resultcode;
	gRes->masterCharId = req->masterCharId;
	gRes->dwZenny = req->dwZenny;
	gRes->qwGuildFunctionFlag = resultcode;
	gPacket.SetPacketLen(sizeof(sTG_GUILD_FUNCTION_ADD_RES));
	g_pApp->Send(GetHandle(), &gPacket);
}


void CServerPassiveSession::RecvGuildBankStartReq(CNtlPacket * pPacket)
{
	sGT_GUILD_BANK_START_REQ * req = (sGT_GUILD_BANK_START_REQ *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;
	WORD wResultCode = GAME_SUCCESS;

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer && pPlayer->GetPcInitState())
	{
		if (pPlayer->GetGuild() == NULL)
			wResultCode = GAME_GUILD_NO_GUILD_FOUND;
		else
		{
			wResultCode = pPlayer->GetGuild()->CanOpenGuildBank(req->charId);
		}
	}
	else wResultCode = GAME_FAIL;


	//send to gameserver
	CNtlPacket gPacket(sizeof(sTG_GUILD_BANK_START_RES));
	sTG_GUILD_BANK_START_RES * gRes = (sTG_GUILD_BANK_START_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_GUILD_BANK_START_RES;
	gRes->charId = req->charId;
	gRes->hNpcHandle = req->hNpcHandle;
	gRes->wResultCode = wResultCode;
	gPacket.SetPacketLen(sizeof(sTG_GUILD_BANK_START_RES));
	g_pApp->Send(GetHandle(), &gPacket);
}


void CServerPassiveSession::RecvGuildBankEndNfy(CNtlPacket * pPacket)
{
	sGT_GUILD_BANK_END_NFY * req = (sGT_GUILD_BANK_END_NFY *)pPacket->GetPacketData();

	WORD wResultCode = GAME_SUCCESS;

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer && pPlayer->GetPcInitState())
	{
		if (pPlayer->GetGuild() == NULL)
			wResultCode = GAME_GUILD_NO_GUILD_FOUND;
		else if(pPlayer->GetGuild()->GetGuildBankUser() != req->charId)
			wResultCode = COMMUNITY_GUILD_BANK_QUERY;
		else
		{
			pPlayer->GetGuild()->CloseGuildBank();
		}
	}
	else wResultCode = GAME_FAIL;

	//send to gameserver
	CNtlPacket gPacket(sizeof(sTG_GUILD_BANK_END_NFY));
	sTG_GUILD_BANK_END_NFY * gRes = (sTG_GUILD_BANK_END_NFY *)gPacket.GetPacketData();
	gRes->wOpCode = TG_GUILD_BANK_END_NFY;
	gRes->charId = req->charId;
	gRes->wResultCode = wResultCode;
	gPacket.SetPacketLen(sizeof(sTG_GUILD_BANK_END_NFY));
	g_pApp->Send(this->GetHandle(), &gPacket);
}


//--------------------------------------------------------------------------------------//
//		CREATE GUILD MARK (EMBLEM)
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecGuildCreateMarkReq(CNtlPacket * pPacket)
{
	sGT_GUILD_CREATE_MARK_REQ * req = (sGT_GUILD_CREATE_MARK_REQ *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		if (player->GetGuild() == NULL)
			resultcode = GAME_GUILD_NO_GUILD_FOUND;
		else if (!player->GetGuild()->IsGuildMaster(player->GetCharID()))
			resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
		else if (!Dbo_HasGuildFunction(player->GetGuild()->GetInfo()->qwGuildFunctionFlag, DBO_GUILD_FUNCTION_CAN_SET_EMBLEM))
			resultcode = COMMUNITY_GUILD_FUNCTION_NOT_FOUND;
		else
		{
			CNtlPacket qPacket(sizeof(sTQ_GUILD_CREATE_MARK_REQ));
			sTQ_GUILD_CREATE_MARK_REQ * qRes = (sTQ_GUILD_CREATE_MARK_REQ *)qPacket.GetPacketData();
			qRes->wOpCode = TQ_GUILD_CREATE_MARK_REQ;
			qRes->charId = req->charId;
			qRes->byServerChannelIndex = player->GetChannel();
			qRes->byServerIndex = 0;
			qRes->guildId = player->GetGuildID();
			memcpy(&qRes->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));
			qPacket.SetPacketLen(sizeof(sTQ_GUILD_CREATE_MARK_REQ));
			app->SendTo(app->GetQueryServerSession(), &qPacket);

			return;
		}
	}
	else resultcode = GAME_FAIL;

	//send to gameserver
	CNtlPacket gPacket(sizeof(sTG_GUILD_CREATE_MARK_RES));
	sTG_GUILD_CREATE_MARK_RES * gRes = (sTG_GUILD_CREATE_MARK_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_GUILD_CREATE_MARK_RES;
	gRes->wResultCode = resultcode;
	gRes->charId = req->charId;
	gPacket.SetPacketLen(sizeof(sTG_GUILD_CREATE_MARK_RES));
	app->Send(GetHandle(), &gPacket);
}

//--------------------------------------------------------------------------------------//
//		CREATE GUILD MARK (EMBLEM)
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecGuildChangeMarkReq(CNtlPacket * pPacket)
{
	sGT_GUILD_CHANGE_MARK_REQ * req = (sGT_GUILD_CHANGE_MARK_REQ *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		if (player->GetGuild() == NULL)
			resultcode = GAME_GUILD_NO_GUILD_FOUND;
		else if (!player->GetGuild()->IsGuildMaster(player->GetCharID()))
			resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
		else if (!Dbo_HasGuildFunction(player->GetGuild()->GetInfo()->qwGuildFunctionFlag, DBO_GUILD_FUNCTION_CAN_SET_EMBLEM))
			resultcode = COMMUNITY_GUILD_FUNCTION_NOT_FOUND;
		else
		{
			CNtlPacket qPacket(sizeof(sTQ_GUILD_CHANGE_MARK_REQ));
			sTQ_GUILD_CHANGE_MARK_REQ * qRes = (sTQ_GUILD_CHANGE_MARK_REQ *)qPacket.GetPacketData();
			qRes->wOpCode = TQ_GUILD_CHANGE_MARK_REQ;
			qRes->charId = req->charId;
			qRes->byServerChannelIndex = player->GetChannel();
			qRes->byServerIndex = 0;
			qRes->guildId = player->GetGuildID();
			memcpy(&qRes->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));
			qRes->dwZenny = req->dwZenny;
			qPacket.SetPacketLen(sizeof(sTQ_GUILD_CHANGE_MARK_REQ));
			app->SendTo(app->GetQueryServerSession(), &qPacket);

			return;
		}
	}
	else resultcode = GAME_FAIL;

	//send to gameserver
	CNtlPacket gPacket(sizeof(sTG_GUILD_CHANGE_MARK_RES));
	sTG_GUILD_CHANGE_MARK_RES * gRes = (sTG_GUILD_CHANGE_MARK_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_GUILD_CHANGE_MARK_RES;
	gRes->wResultCode = resultcode;
	gRes->charId = req->charId;
	gPacket.SetPacketLen(sizeof(sTG_GUILD_CHANGE_MARK_RES));
	g_pApp->Send(this->GetHandle(), &gPacket);
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CServerPassiveSession::RecGuildChangeNameReq(CNtlPacket * pPacket)
{
	sGT_GUILD_CHANGE_NAME_REQ * req = (sGT_GUILD_CHANGE_NAME_REQ *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		if (player->GetGuild() == NULL)
			resultcode = GAME_GUILD_NO_GUILD_FOUND;
		else if (!player->GetGuild()->IsGuildMaster(player->GetCharID()))
			resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
		else if(smart_ptr<QueryResult> namecheck = GetCharDB.Query("SELECT GuildID FROM guilds WHERE GuildName=\"%ls\" ", req->wszGuildName))
			resultcode = GAME_GUILD_SAME_GUILD_NAME_EXIST;
		else
		{
			CNtlPacket qPacket(sizeof(sTQ_GUILD_CHANGE_NAME_REQ));
			sTQ_GUILD_CHANGE_NAME_REQ * qRes = (sTQ_GUILD_CHANGE_NAME_REQ *)qPacket.GetPacketData();
			qRes->wOpCode = TQ_GUILD_CHANGE_NAME_REQ;
			qRes->charId = req->charId;
			qRes->byServerChannelIndex = player->GetChannel();
			qRes->byServerIndex = 0;
			qRes->guildId = player->GetGuildID();
			qRes->byPlace = req->byPlace;
			qRes->byPos = req->byPos;
			qRes->itemId = req->itemId;
			NTL_SAFE_WCSCPY(qRes->wszGuildName, req->wszGuildName);
			qPacket.SetPacketLen(sizeof(sTQ_GUILD_CHANGE_NAME_REQ));
			app->SendTo(app->GetQueryServerSession(), &qPacket);

			return;
		}
	}
	else resultcode = GAME_FAIL;

	CNtlPacket packet(sizeof(sTG_GUILD_CHANGE_NAME_RES));
	sTG_GUILD_CHANGE_NAME_RES * res = (sTG_GUILD_CHANGE_NAME_RES *)packet.GetPacketData();
	res->wOpCode = TG_GUILD_CHANGE_NAME_RES;
	res->wResultCode = resultcode;
	res->charId = req->charId;
	res->byPlace = req->byPlace;
	res->byPos = req->byPos;
	res->guildId = player->GetGuildID();
	res->itemId = req->itemId;
	NTL_SAFE_WCSCPY(res->wszGuildName, req->wszGuildName);
	packet.SetPacketLen(sizeof(sTG_GUILD_CHANGE_NAME_RES));
	g_pApp->Send(GetHandle(), &packet);
}

void CServerPassiveSession::RecvBudokaiChannelSateInfoNfy(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_CHANNEL_STATE_INFO_NFY * req = (sGT_BUDOKAI_CHANNEL_STATE_INFO_NFY *)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_CHANNEL_STATE_INFO_NFY));
	sTG_BUDOKAI_CHANNEL_STATE_INFO_NFY * res = (sTG_BUDOKAI_CHANNEL_STATE_INFO_NFY *)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_CHANNEL_STATE_INFO_NFY;
	res->tmDefaultOpenTime = req->tmDefaultOpenTime;
	res->bRankbattleAllow = req->bRankbattleAllow;
	memcpy(&res->sStateInfo, &req->sStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
	memcpy(res->aMatchStateInfo, req->aMatchStateInfo, sizeof(req->aMatchStateInfo));
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_CHANNEL_STATE_INFO_NFY));
	g_pServerInfoManager->Broadcast(&packet, DOJO_CHANNEL_INDEX);
}

void CServerPassiveSession::RecvBudokaiChannelUpdateState(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_CHANNEL_UPDATE_STATE * req = (sGT_BUDOKAI_CHANNEL_UPDATE_STATE *)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_CHANNEL_UPDATE_STATE));
	sTG_BUDOKAI_CHANNEL_UPDATE_STATE * res = (sTG_BUDOKAI_CHANNEL_UPDATE_STATE *)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_CHANNEL_UPDATE_STATE;
	res->tmDefaultOpenTime = req->tmDefaultOpenTime;
	res->bRankbattleAllow = req->bRankbattleAllow;
	memcpy(&res->sStateInfo, &req->sStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_CHANNEL_UPDATE_STATE));
	g_pServerInfoManager->Broadcast(&packet, DOJO_CHANNEL_INDEX);
}

void CServerPassiveSession::RecvBudokaiChannelUpdateMatchState(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE * req = (sGT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE *)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE));
	sTG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE * res = (sTG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE *)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE;
	res->byMatchType = req->byMatchType;
	memcpy(&res->sStateInfo, &req->sStateInfo, sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO));
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE));
	g_pServerInfoManager->Broadcast(&packet, DOJO_CHANNEL_INDEX);
}

void CServerPassiveSession::RecvBudokaiTournamentIndividualListRes(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES * req = (sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer && pPlayer->GetPcInitState())
	{
		if (req->wResultCode == CHAT_SUCCESS)
		{
			sVARIABLE_DATA sData;
			sData.Init(2000);

			WORD wEntryTeam = INVALID_WORD;

			if (req->bSetEntryTeam)
			{
				sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR info[BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER];
				memset(info, 0, sizeof(info));

				for (int i = 0; i < BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER; i++)
				{
					if (req->aEntryTeam[i].wJoinId == 0)
						break;

					info[i].wJoinId = req->aEntryTeam[i].wJoinId;
					info[i].bDojoRecommender = req->aEntryTeam[i].bDojoRecommender;
					info[i].wTeamName = sData.Write(sizeof(req->aEntryTeam[i].wszTeamName), req->aEntryTeam[i].wszTeamName);
				}

				wEntryTeam = sData.Write(sizeof(info), info);
			}

			WORD wMatchData = sData.Write(sizeof(req->aMatchData), req->aMatchData);

			WORD wPacketSize = sData.GetPacketSize(sizeof(sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES));

			CNtlPacket packet(wPacketSize);
			sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES * res = (sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES *)packet.GetPacketData();
			res->wOpCode = TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES;
			res->wResultCode = req->wResultCode;
			res->wEntryTeam = wEntryTeam;
			res->byMatchDataCount = req->byMatchDataCount;
			res->wMatchData = wMatchData;
			sData.CopyTo(&res->sData, sData.GetDataSize());
			pPlayer->SendPacket(&packet);

			ERR_LOG(LOG_GENERAL, "BUDOKAI: wPacketSize %u, wMatchData %u, GetDataSize() %u, GetTotalSize %u, byTeamCount %u", wPacketSize, wMatchData, sData.GetDataSize(), sData.GetTotalSize(), req->byMatchDataCount);
		}
		else
		{
			CNtlPacket packet(sizeof(sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES));
			sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES * res = (sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES *)packet.GetPacketData();
			res->wOpCode = TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES;
			res->wResultCode = req->wResultCode;
			pPlayer->SendPacket(&packet);
		}
	}
}

void CServerPassiveSession::RecvBudokaiTournamentIndividualInfoRes(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES * req = (sGT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer && pPlayer->GetPcInitState())
	{
		if (req->wResultCode == CHAT_SUCCESS)
		{
			sVARIABLE_DATA sData;
			sData.Init(1024);

			TEAMTYPE awTeamType[2];
			awTeamType[0] = INVALID_WORD;
			awTeamType[1] = INVALID_WORD;

			for (int i = 0; i < 2; i++)
			{
				if (req->asEntryData[i].charId != 0)
				{
					sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA_VAR asEntryData;

					asEntryData.bDojoRecommender = req->asEntryData[i].bDojoRecommender;
					asEntryData.byClass = req->asEntryData[i].byClass;
					asEntryData.byLevel = req->asEntryData[i].byLevel;
					asEntryData.byMinorMatch_MatchIndex = req->asEntryData[i].byMinorMatch_MatchIndex;
					asEntryData.charId = req->asEntryData[i].charId;
					asEntryData.fRankPoint = req->asEntryData[i].fRankPoint;
					asEntryData.wCharName = sData.Write(sizeof(req->asEntryData[i].wszCharName), req->asEntryData[i].wszCharName);
					asEntryData.wGuildName = sData.Write(sizeof(req->asEntryData[i].wszGuildName), req->asEntryData[i].wszGuildName);
					asEntryData.wJoinId = req->asEntryData[i].wJoinId;
					asEntryData.wRankBattleDrawCount = req->asEntryData[i].wRankBattleDrawCount;
					asEntryData.wRankBattleLoseCount = req->asEntryData[i].wRankBattleLoseCount;
					asEntryData.wRankBattleWinCount = req->asEntryData[i].wRankBattleWinCount;

					awTeamType[i] = sData.Write(sizeof(sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA_VAR), &asEntryData);
				}
			}


			WORD wPacketSize = sData.GetPacketSize(sizeof(sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES));

			CNtlPacket packet(wPacketSize);
			sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES * res = (sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES *)packet.GetPacketData();
			res->wOpCode = TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES;
			res->wResultCode = req->wResultCode;
			res->awTeamType[0] = awTeamType[0];
			res->awTeamType[1] = awTeamType[1];
			sData.CopyTo(&res->sData, sData.GetDataSize());
			pPlayer->SendPacket(&packet);
			
			ERR_LOG(LOG_GENERAL, "BUDOKAI: wPacketSize %u, sData.GetDataSize() %u", wPacketSize, sData.GetDataSize());
		}
		else
		{
			CNtlPacket packet(sizeof(sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES));
			sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES * res = (sTU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES *)packet.GetPacketData();
			res->wOpCode = TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES;
			res->wResultCode = req->wResultCode;
			pPlayer->SendPacket(&packet);
		}
	}
}

void CServerPassiveSession::RecvBudokaiTournamentTeamListRes(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES * req = (sGT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer && pPlayer->GetPcInitState())
	{
		if (req->wResultCode == CHAT_SUCCESS)
		{
			sVARIABLE_DATA sData;
			sData.Init(2000);

			WORD wEntryTeam = INVALID_WORD;

			if (req->bSetEntryTeam)
			{
				sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR info[BUDOKAI_MAJOR_MATCH_TEAM_NUMBER];
				memset(info, 0, sizeof(info));

				for (int i = 0; i < BUDOKAI_MAJOR_MATCH_TEAM_NUMBER; i++)
				{
					if (req->aEntryTeam[i].wJoinId == 0)
						break;

					info[i].wJoinId = req->aEntryTeam[i].wJoinId;
					info[i].bDojoRecommender = req->aEntryTeam[i].bDojoRecommender;
					info[i].wTeamName = sData.Write(sizeof(req->aEntryTeam[i].wszTeamName), req->aEntryTeam[i].wszTeamName);
				}

				wEntryTeam = sData.Write(sizeof(info), info);
			}

			WORD wMatchData = sData.Write(sizeof(req->aMatchData), req->aMatchData);

			WORD wPacketSize = sData.GetPacketSize(sizeof(sTU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES));

			CNtlPacket packet(wPacketSize);
			sTU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES * res = (sTU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES *)packet.GetPacketData();
			res->wOpCode = TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES;
			res->wResultCode = req->wResultCode;
			res->wEntryTeam = wEntryTeam;
			res->byMatchDataCount = req->byMatchDataCount;
			res->wMatchData = wMatchData;
			sData.CopyTo(&res->sData, sData.GetDataSize());
			pPlayer->SendPacket(&packet);

			ERR_LOG(LOG_GENERAL, "BUDOKAI: wPacketSize %u, wMatchData %u, GetDataSize() %u, GetTotalSize %u, byTeamCount %u", wPacketSize, wMatchData, sData.GetDataSize(), sData.GetTotalSize(), req->byMatchDataCount);
		}
		else
		{
			CNtlPacket packet(sizeof(sTU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES));
			sTU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES * res = (sTU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES *)packet.GetPacketData();
			res->wOpCode = TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES;
			res->wResultCode = req->wResultCode;
			pPlayer->SendPacket(&packet);
		}
	}
}

void CServerPassiveSession::RecvBudokaiTournamentTeamInfoRes(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES * req = (sGT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer && pPlayer->GetPcInitState())
	{
		if (req->wResultCode == CHAT_SUCCESS)
		{
			sVARIABLE_DATA sData;
			sData.Init(1024);

			TEAMTYPE awTeamType[2];
			awTeamType[0] = INVALID_WORD;
			awTeamType[1] = INVALID_WORD;

			sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA_VAR asEntryData[2];

			for (int i = 0; i < 2; i++)
			{
				if (req->asEntryData[i].wJoinId != 0)
				{
					sBUDOKAI_TOURNAMENT_TEAM_ENTRY_MEMBER_DATA_VAR	aMembers[NTL_MAX_MEMBER_IN_PARTY];

					asEntryData[i].wTeamName = sData.Write(sizeof(req->asEntryData[i].wszTeamName), req->asEntryData[i].wszTeamName);
					asEntryData[i].byMemberCount = req->asEntryData[i].byMemberCount;

					for (BYTE ii = 0; ii < asEntryData[i].byMemberCount; ii++)
					{
						aMembers[ii].byClass = req->asEntryData[i].aMembers[ii].byClass;
						aMembers[ii].byLevel = req->asEntryData[i].aMembers[ii].byLevel;
						aMembers[ii].charId = req->asEntryData[i].aMembers[ii].charId;
						aMembers[ii].fRankPoint = req->asEntryData[i].aMembers[ii].fRankPoint;
						aMembers[ii].wCharName = sData.Write(sizeof(req->asEntryData[i].aMembers[ii].wszCharName), req->asEntryData[i].aMembers[ii].wszCharName);
					}

					asEntryData[i].wMembers = sData.Write(sizeof(aMembers), aMembers);
					asEntryData[i].byMinorMatch_MatchIndex = req->asEntryData[i].byMinorMatch_MatchIndex;
					asEntryData[i].wJoinId = req->asEntryData[i].wJoinId;
					

					awTeamType[i] = sData.Write(sizeof(sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA_VAR), &asEntryData[i]);
				}
			}


			WORD wPacketSize = sData.GetPacketSize(sizeof(sTU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES));

			CNtlPacket packet(wPacketSize);
			sTU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES * res = (sTU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES *)packet.GetPacketData();
			res->wOpCode = TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES;
			res->wResultCode = req->wResultCode;
			res->awTeamType[0] = awTeamType[0];
			res->awTeamType[1] = awTeamType[1];
			sData.CopyTo(&res->sData, sData.GetDataSize());
			pPlayer->SendPacket(&packet);

			ERR_LOG(LOG_GENERAL, "BUDOKAI: wPacketSize %u, sData.GetDataSize() %u", wPacketSize, sData.GetDataSize());
		}
		else
		{
			CNtlPacket packet(sizeof(sTU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES));
			sTU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES * res = (sTU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES *)packet.GetPacketData();
			res->wOpCode = TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES;
			res->wResultCode = req->wResultCode;
			pPlayer->SendPacket(&packet);
		}
	}
}

void CServerPassiveSession::RecvBudokaiMinorMatchIndividualListNfy(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY * req = (sGT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY *)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY));
	sTG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY * res = (sTG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY *)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY;
	res->byServerChannelId = req->byServerChannelId;
	res->byServerIndex = req->byServerIndex;
	memcpy(res->aTeleportData, req->aTeleportData, sizeof(req->aTeleportData));
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY));
	g_pServerInfoManager->Broadcast(&packet, DOJO_CHANNEL_INDEX);
}

void CServerPassiveSession::RecvBudokaiMinorMatchTeamListNfy(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY * req = (sGT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY *)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY));
	sTG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY * res = (sTG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY *)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY;
	res->byServerChannelId = req->byServerChannelId;
	res->byServerIndex = req->byServerIndex;
	memcpy(res->aTeleportData, req->aTeleportData, sizeof(req->aTeleportData));
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY));
	g_pServerInfoManager->Broadcast(&packet, DOJO_CHANNEL_INDEX);
}

void CServerPassiveSession::RecvBudokaiMinorMatchTeleportInfoReq(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ * req = (sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer && pPlayer->GetPcInitState())
	{
		CNtlPacket packet(sizeof(sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ));
		sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ * res = (sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ *)packet.GetPacketData();
		res->wOpCode = TG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ;
		res->handle = req->handle;
		res->charId = req->charId;
		res->byMatchType = req->byMatchType;
		res->joinId = req->joinId;
		res->byServerChannelId = pPlayer->GetChannel();
		res->byServerIndex = 0;
		packet.SetPacketLen(sizeof(sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ));
		g_pApp->Send(g_pServerInfoManager->GetGsSession(DOJO_CHANNEL_INDEX), &packet);
	}
}

void CServerPassiveSession::RecvBudokaiMinorMatchTeleportInfoRes(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES * req = (sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer && pPlayer->GetPcInitState())
	{
		CNtlPacket packet(sizeof(sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES));
		sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES * res = (sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES *)packet.GetPacketData();
		res->wOpCode = TG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES;
		res->handle = req->handle;
		res->charId = req->charId;
		res->byMatchType = req->byMatchType;
		res->joinId = req->joinId;
		res->wResultCode = req->wResultCode;
		res->byTeleportType = req->byTeleportType;
		res->byDestServerChannelId = req->byDestServerChannelId;
		res->byDestServerIndex = req->byDestServerIndex;
		res->worldTblidx = req->worldTblidx;
		res->worldId = req->worldId;
		res->vLoc.x = req->vLoc.x;
		res->vLoc.y = req->vLoc.y;
		res->vLoc.z = req->vLoc.z;
		res->vDir.x = req->vDir.x;
		res->vDir.y = req->vDir.y;
		res->vDir.z = req->vDir.z;
		packet.SetPacketLen(sizeof(sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES));
		g_pApp->Send(g_pServerInfoManager->GetGsSession(req->byServerChannelId), &packet);
	}
}

void CServerPassiveSession::RecvBudokaiProgressMessageNfy(CNtlPacket * pPacket)
{
	sGT_BUDOKAI_PROGRESS_MESSAGE_NFY * req = (sGT_BUDOKAI_PROGRESS_MESSAGE_NFY *)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_PROGRESS_MESSAGE_NFY));
	sTG_BUDOKAI_PROGRESS_MESSAGE_NFY * res = (sTG_BUDOKAI_PROGRESS_MESSAGE_NFY *)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_PROGRESS_MESSAGE_NFY;
	res->byMatchType = req->byMatchType;
	res->byMsgId = req->byMsgId;
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_PROGRESS_MESSAGE_NFY));
	g_pServerInfoManager->Broadcast(&packet, DOJO_CHANNEL_INDEX);
}

void CServerPassiveSession::RecvCharReadyForCommunityServerNfy(CNtlPacket * pPacket)
{
	sGT_CHAR_READY_FOR_COMMUNITY_SERVER_NFY * req = (sGT_CHAR_READY_FOR_COMMUNITY_SERVER_NFY *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer)
	{
		//printf("sGT_CHAR_READY_FOR_COMMUNITY_SERVER_NFY \n");
		pPlayer->SetReadyForCommunityServer(true);
	}
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecAuctionHouseSellReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;
	sGT_TENKAICHIDAISIJYOU_SELL_REQ * req = (sGT_TENKAICHIDAISIJYOU_SELL_REQ *)pPacket->GetPacketData();

	WORD wResultCode;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sTQ_TENKAICHIDAISIJYOU_SELL_REQ));
		sTQ_TENKAICHIDAISIJYOU_SELL_REQ * res = (sTQ_TENKAICHIDAISIJYOU_SELL_REQ *)packet.GetPacketData();
		res->wOpCode = TQ_TENKAICHIDAISIJYOU_SELL_REQ;
		res->byServerChannelIndex = 0;
		res->byServerIndex = 0;
		res->charId = req->charId;
		res->itemId = req->itemId;
		res->dwPrice = req->dwPrice;
		res->byCount = req->byCount;
		res->byTabType = req->byTabType;
		res->byItemType = req->byItemType;
		res->byItemLevel = req->byItemLevel;
		NTL_SAFE_WCSCPY(res->awchSeller, req->awchSeller);
		NTL_SAFE_WCSCPY(res->awchItemName, req->awchItemName);
		res->nStartSellTime = time(NULL);
		res->nEndSellTime = req->dwTime;
		res->byPlace = req->byPlace;
		res->byPosition = req->byPosition;
		res->dwFee = req->dwFee;
		res->dwClassBitFlag = req->dwClassBitFlag;
		packet.SetPacketLen(sizeof(sTQ_TENKAICHIDAISIJYOU_SELL_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		return;
	}
	else wResultCode = GAME_FAIL;

	CNtlPacket packet(sizeof(sTG_TENKAICHIDAISIJYOU_SELL_RES));
	sTG_TENKAICHIDAISIJYOU_SELL_RES * res = (sTG_TENKAICHIDAISIJYOU_SELL_RES *)packet.GetPacketData();
	res->wOpCode = TG_TENKAICHIDAISIJYOU_SELL_RES;
	res->wResultCode = wResultCode;
	res->charId = req->charId;
	packet.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_SELL_RES));
	app->Send(GetHandle(), &packet);
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecAuctionHouseSellCancelReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;
	sGT_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ * req = (sGT_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ *)pPacket->GetPacketData();

	WORD wResultCode;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		if (sTENKAICHIDAISIJYOU_DATA* pData = g_pAH->GetItem(req->nItem))
		{
			if (req->charId == pData->charId)
			{
				CNtlPacket packet(sizeof(sTQ_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ));
				sTQ_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ * res = (sTQ_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ *)packet.GetPacketData();
				res->wOpCode = TQ_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ;
				res->charId = req->charId;
				res->nItem = req->nItem;
				res->byMailType = eMAIL_TYPE_ITEM;
				res->bySenderType = eMAIL_SENDER_TYPE_SYSTEM;
				NTL_SAFE_WCSCPY(res->wchText, req->awchText);
				packet.SetPacketLen(sizeof(sTQ_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ));
				app->SendTo(app->GetQueryServerSession(), &packet);

				return;
			}
			else wResultCode = TENKAICHIDAISIJYOU_CANNOT_CANCEL_OTHER_ITEM;
		}
		else wResultCode = TENKAICHIDAISIJYOU_CANNOT_NOT_EXIST;
	}
	else wResultCode = GAME_FAIL;

	CNtlPacket packet(sizeof(sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES));
	sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES * res = (sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES *)packet.GetPacketData();
	res->wOpCode = TG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES;
	res->wResultCode = wResultCode;
	res->charId = req->charId;
	res->nItem = req->nItem;
	packet.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES));
	app->Send(GetHandle(), &packet);
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecAuctionHousePriceInfoReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	sGT_TENKAICHIDAISIJYOU_PRICE_INFO_REQ * req = (sGT_TENKAICHIDAISIJYOU_PRICE_INFO_REQ *)pPacket->GetPacketData();

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sTG_TENKAICHIDAISIJYOU_PRICE_INFO_RES));
		sTG_TENKAICHIDAISIJYOU_PRICE_INFO_RES * res = (sTG_TENKAICHIDAISIJYOU_PRICE_INFO_RES *)packet.GetPacketData();
		res->wOpCode = TG_TENKAICHIDAISIJYOU_PRICE_INFO_RES;
		res->dwPrice = g_pAH->ReturnPrice(req->nItem);
		res->charId = req->charId;
		res->nItem = req->nItem;
		packet.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_PRICE_INFO_RES));
		app->Send(GetHandle(), &packet);
	}
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecAuctionHouseBuyReq(CNtlPacket * pPacket)
{
	sGT_TENKAICHIDAISIJYOU_BUY_REQ * req = (sGT_TENKAICHIDAISIJYOU_BUY_REQ *)pPacket->GetPacketData();
	CChatServer* app = (CChatServer*)g_pApp;

	WORD wResultCode;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		if (sTENKAICHIDAISIJYOU_DATA* pData = g_pAH->GetItem(req->nItem))
		{
			if (req->charId != pData->charId)
			{
				CNtlPacket packet(sizeof(sTQ_TENKAICHIDAISIJYOU_BUY_REQ));
				sTQ_TENKAICHIDAISIJYOU_BUY_REQ * res = (sTQ_TENKAICHIDAISIJYOU_BUY_REQ *)packet.GetPacketData();
				res->wOpCode = TQ_TENKAICHIDAISIJYOU_BUY_REQ;
				res->charId = req->charId;
				res->dwMoney = req->dwMoney;
				res->nItem = req->nItem;
				res->sellcharId = pData->charId;
				NTL_SAFE_WCSCPY(res->wchSellText, req->awchSellText);
				NTL_SAFE_WCSCPY(res->wchBuyText, req->awchBuyText);
				packet.SetPacketLen(sizeof(sTQ_TENKAICHIDAISIJYOU_BUY_REQ));
				app->SendTo(app->GetQueryServerSession(), &packet);

				return;
			}
			else wResultCode = TENKAICHIDAISIJYOU_CANNOT_BUY_MYITEM;
		}
		else wResultCode = TENKAICHIDAISIJYOU_CANNOT_NOT_EXIST;
	}
	else wResultCode = GAME_FAIL;

	CNtlPacket packet(sizeof(sTG_TENKAICHIDAISIJYOU_BUY_RES));
	sTG_TENKAICHIDAISIJYOU_BUY_RES * res = (sTG_TENKAICHIDAISIJYOU_BUY_RES *)packet.GetPacketData();
	res->wOpCode = TG_TENKAICHIDAISIJYOU_BUY_RES;
	res->wResultCode = wResultCode;
	res->charId = req->charId;
	res->nItem = req->nItem;
	res->dwMoney = req->dwMoney;
	packet.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_BUY_RES));
	app->Send(GetHandle(), &packet);
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecAuctionHouseListDataReq(CNtlPacket * pPacket)
{
	sGT_TENKAICHIDAISIJYOU_LIST_REQ * req = (sGT_TENKAICHIDAISIJYOU_LIST_REQ *)pPacket->GetPacketData();

	CPlayer* player = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	if (player)
	{
		g_pAH->LoadAuctionHouseData(player, req);

		return;
	}
	else ERR_LOG(LOG_USER, "Account %u not found. Cant load auction house", req->accountId);

	CNtlPacket packet2(sizeof(sTG_TENKAICHIDAISIJYOU_LIST_RES));
	sTG_TENKAICHIDAISIJYOU_LIST_RES * res2 = (sTG_TENKAICHIDAISIJYOU_LIST_RES *)packet2.GetPacketData();
	res2->wOpCode = TG_TENKAICHIDAISIJYOU_LIST_RES;
	res2->byPage = req->byPage;
	res2->charId = req->charId;
	res2->uiPage = req->uiPage;
	res2->wResultCode = GAME_FAIL;
	packet2.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_LIST_RES));
	g_pApp->Send(GetHandle(), &packet2);
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecAuctionHousePeriodEndText(CNtlPacket * pPacket)
{
	sGT_TENKAICHIDAISIJYOU_PERIODEND_TEXT * req = (sGT_TENKAICHIDAISIJYOU_PERIODEND_TEXT *)pPacket->GetPacketData();

	
}

void CServerPassiveSession::RecvCharServerTeleport(CNtlPacket * pPacket)
{
	sGT_CHAR_SERVER_TELEPORT * req = (sGT_CHAR_SERVER_TELEPORT *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer)
	{
		//printf("sGT_CHAR_SERVER_TELEPORT \n");
		pPlayer->SetReadyForCommunityServer(false);
		pPlayer->SetCanLoadFromDB(true);

		BYTE byDestChannel = req->sTeleportInfo.serverChannelId;
		WORLDID destWorldId = req->sTeleportInfo.worldId;
		CNtlVector vDestLoc(req->sTeleportInfo.vLoc);

		//check if enter/leave dojo
		if (req->sTeleportInfo.serverChannelId == DOJO_CHANNEL_INDEX)
		{
			//increase dojo player counter
			CDojo* pDojo = g_pDojoWarManager->GetDojo(pPlayer->GetGuildID());
			if (pDojo)
			{
				if(pDojo->CanEnterDojo(pPlayer->GetGuildID()))
					pDojo->UpdatePlayerCount(pPlayer->GetGuildID(), pPlayer->GetCharID(), true);
				else
				{
					byDestChannel = pPlayer->GetChannel();
					destWorldId = pPlayer->GetWorldID();
					vDestLoc = pPlayer->GetLocation();

					ERR_LOG(LOG_USER, "Player %u could not enter dojo. GuildID: %u", req->charId, pPlayer->GetGuildID());
				}
			}
		}
		else if (pPlayer->GetChannel() == DOJO_CHANNEL_INDEX && req->sTeleportInfo.serverChannelId != DOJO_CHANNEL_INDEX)
		{
			//decrease dojo player counter
			CDojo* pDojo = g_pDojoWarManager->GetDojo(pPlayer->GetGuildID());
			if (pDojo)
			{
				pDojo->UpdatePlayerCount(pPlayer->GetGuildID(), pPlayer->GetCharID(), false);
			}
		}

		pPlayer->SetChannel(byDestChannel);
		pPlayer->SetWorldID(destWorldId);
		pPlayer->SetLocation(vDestLoc);

		CNtlPacket packet(sizeof(sTG_CHAR_SERVER_TELEPORT));
		sTG_CHAR_SERVER_TELEPORT * res = (sTG_CHAR_SERVER_TELEPORT *)packet.GetPacketData();
		res->wOpCode = TG_CHAR_SERVER_TELEPORT;
		res->charId = req->charId;
		res->sTeleportInfo = req->sTeleportInfo;
		res->sTeleportInfo.serverChannelId = byDestChannel;
		vDestLoc.CopyTo(res->sTeleportInfo.vLoc);
		res->sTeleportInfo.worldId = destWorldId;
		memcpy(res->abyAuthKey, req->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
		packet.SetPacketLen(sizeof(sTG_CHAR_SERVER_TELEPORT));
		g_pApp->Send(GetHandle(), &packet);
	}
}


//--------------------------------------------------------------------------------------//
//		Change name nfy
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecvChangeNameNfy(CNtlPacket * pPacket)
{
	sGT_CHAR_NAME_CHANGE_NFY * req = (sGT_CHAR_NAME_CHANGE_NFY *)pPacket->GetPacketData();
	
	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sTU_CHAR_NAME_CHANGED_NFY));
		sTU_CHAR_NAME_CHANGED_NFY * res = (sTU_CHAR_NAME_CHANGED_NFY *)packet.GetPacketData();
		res->wOpCode = TU_CHAR_NAME_CHANGED_NFY;
		res->targetID = req->charId;
		NTL_SAFE_WCSCPY(res->wszCharName, req->wszCharName);
		NTL_SAFE_WCSCPY(res->wszOldCharName, player->GetCharName());
		packet.SetPacketLen(sizeof(sTU_CHAR_NAME_CHANGED_NFY));
		g_pPlayerManager->SendMsgToAll(&packet);

		if (player->GetGuild())
			player->GetGuild()->UpdateMemberInfoCharName(player->GetCharID(), req->wszCharName);

		player->SetCharName(req->wszCharName);

		char* chCurName = Ntl_WC2MB(res->wszOldCharName);
		char* chNewName = Ntl_WC2MB(res->wszCharName);

		ERR_LOG(LOG_USER, "Player %u changed name from %s to %s", player->GetCharID(), chCurName, chNewName);

		Ntl_CleanUpHeapString(chCurName);
		Ntl_CleanUpHeapString(chNewName);
	}
}


//--------------------------------------------------------------------------------------//
//		DOJO CREATE REQ
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecvDojoCreateReq(CNtlPacket * pPacket)
{
	sGT_DOJO_CREATE_REQ * req = (sGT_DOJO_CREATE_REQ *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);

	if (player == NULL || player->GetPcInitState() == false)
		resultcode = GAME_FAIL;
	else if (player->GetGuild() == NULL)
		resultcode = GAME_GUILD_NO_GUILD_FOUND;
	else if (player->GetGuild()->HasDojo())
		resultcode = GAME_GUILD_DOJO_ALREADY_HAVE;
	else if (Dbo_HasGuildFunction(player->GetGuild()->GetInfo()->qwGuildFunctionFlag, DBO_GUILD_FUNCTION_CAN_HAVE_DOJO) == false)
		resultcode = COMMUNITY_GUILD_FUNCTION_NOT_FOUND;
	else if (player->GetGuild()->GetInfo()->sMark.IsIntialized() == false)
		resultcode = GAME_GUILD_MARK_NOT_CREATED;
	else if (g_pDojoManager->GetDojoByDojoTblidx(req->dojoTblidx))
		resultcode = GAME_GUILD_MAKE_DOJO_ALREADY_TAKEN;
	else if (player->GetGuild()->IsGuildMaster(req->charId) == false)
		resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
	else
	{
		CNtlPacket packet(sizeof(sTQ_DOJO_CREATE_REQ));
		sTQ_DOJO_CREATE_REQ * res = (sTQ_DOJO_CREATE_REQ *)packet.GetPacketData();
		res->wOpCode = TQ_DOJO_CREATE_REQ;
		res->byServerChannelId = player->GetChannel();
		res->charId = req->charId;
		res->dojoTblidx = req->dojoTblidx;
		res->guildId = player->GetGuildID();
		packet.SetPacketLen(sizeof(sTQ_DOJO_CREATE_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		return;
	}

	CNtlPacket gPacket(sizeof(sTG_DOJO_CREATE_RES));
	sTG_DOJO_CREATE_RES * gRes = (sTG_DOJO_CREATE_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_DOJO_CREATE_RES;
	gRes->charId = req->charId;
	gRes->wResultCode = resultcode;
	gPacket.SetPacketLen(sizeof(sTG_DOJO_CREATE_RES));
	app->Send(GetHandle(), &gPacket);
}
//--------------------------------------------------------------------------------------//
//		DOJO DELETE REQ
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecvDojoDeleteReq(CNtlPacket * pPacket)
{
	sGT_DOJO_DELETE_REQ * req = (sGT_DOJO_DELETE_REQ *)pPacket->GetPacketData();
	
	CChatServer* app = (CChatServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);

	if (player == NULL || player->GetPcInitState() == false)
		resultcode = GAME_FAIL;
	else if (player->GetGuild() == NULL)
		resultcode = GAME_GUILD_NO_GUILD_FOUND;
	else if (player->GetGuild()->HasDojo() == false)
		resultcode = GAME_GUILD_NEED_DOJO_NOT_FOUND;
	else if (player->GetGuild()->GetDojo()->GetDojoData()->dojoTblidx != req->dojoTblidx)
		resultcode = GAME_GUILD_DOJO_NOT_FOUND;
	else if(g_pDojoWarManager->GetDojo(player->GetGuildID()))
		resultcode = GAME_FAIL;
	else if (player->GetGuild()->IsGuildMaster(req->charId) == false)
		resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
	else
	{
		CNtlPacket packet(sizeof(sTQ_DOJO_DEL_REQ));
		sTQ_DOJO_DEL_REQ * res = (sTQ_DOJO_DEL_REQ *)packet.GetPacketData();
		res->wOpCode = TQ_DOJO_DEL_REQ;
		res->byServerChannelId = player->GetChannel();
		res->charId = req->charId;
		res->dojoTblidx = req->dojoTblidx;
		res->guildId = player->GetGuildID();
		packet.SetPacketLen(sizeof(sTQ_DOJO_DEL_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		return;
	}

	//send result
	CNtlPacket gPacket(sizeof(sTG_DOJO_DELETE_RES));
	sTG_DOJO_DELETE_RES * gRes = (sTG_DOJO_DELETE_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_DOJO_DELETE_RES;
	gRes->charId = req->charId;
	gRes->wResultCode = resultcode;
	gPacket.SetPacketLen(sizeof(sTG_DOJO_DELETE_RES));
	g_pApp->Send(GetHandle(), &gPacket);
}
//--------------------------------------------------------------------------------------//
//		DOJO FUNCTION ADD REQ
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecvDojoFunctionAddReq(CNtlPacket * pPacket)
{
	sGT_DOJO_FUNCTION_ADD_REQ * req = (sGT_DOJO_FUNCTION_ADD_REQ *)pPacket->GetPacketData();
	
	CChatServer* app = (CChatServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;
	BYTE byDojoLv = 0;

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->masterCharId);
	if (player)
	{
		CGuild* pGuild = player->GetGuild();

		if (pGuild == NULL)
			resultcode = GAME_GUILD_NO_GUILD_FOUND;
	/*	else if(pGuild->GetDojo() == NULL)						//DONT ENABLE THIS CHECK! OTHERWISE PEOPLE ARE NOT ABLE TO BUY "OWN DOJO" FUNCTION
			resultcode = GAME_GUILD_NEED_DOJO_NOT_FOUND; */
		else
		{
			eDBO_GUILD_FUNCTION eGuildFunction = static_cast<eDBO_GUILD_FUNCTION>(req->byFunction);

			if (pGuild->IsGuildMaster(req->masterCharId) == false)
				resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
			else if (Dbo_CanGetDojoFunction(pGuild->GetInfo()->qwGuildFunctionFlag, pGuild->GetInfo()->dwGuildReputation, req->dwZenny, eGuildFunction, &resultcode) == false)
				;
			else
			{
				byDojoLv = Dbo_GetDojoLevel(req->byFunction);

				if (byDojoLv > 0 && pGuild->GetDojo() == NULL)
					resultcode = GAME_GUILD_NEED_DOJO_NOT_FOUND;
				else
				{
					CNtlPacket packet(sizeof(sTQ_DOJO_FUNCTION_ADD_REQ));
					sTQ_DOJO_FUNCTION_ADD_REQ * res = (sTQ_DOJO_FUNCTION_ADD_REQ *)packet.GetPacketData();
					res->wOpCode = TQ_DOJO_FUNCTION_ADD_REQ;
					res->byServerChannelIndex = player->GetChannel();
					res->byFunction = req->byFunction;
					res->byServerIndex = 0;
					res->dwZenny = req->dwZenny;
					res->guildId = player->GetGuildID();
					res->masterCharId = req->masterCharId;
					res->qwGuildFunctionFlag = AddGuildFunction(pGuild->GetInfo()->qwGuildFunctionFlag, req->byFunction);
					packet.SetPacketLen(sizeof(sTQ_DOJO_FUNCTION_ADD_REQ));
					app->SendTo(app->GetQueryServerSession(), &packet);

					return;
				}
			}
		}
	}
	else resultcode = GAME_FAIL;
	
	//send to gameserver
	CNtlPacket gPacket(sizeof(sTG_DOJO_FUNCTION_ADD_RES));
	sTG_DOJO_FUNCTION_ADD_RES * gRes = (sTG_DOJO_FUNCTION_ADD_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_DOJO_FUNCTION_ADD_RES;
	gRes->wResultCode = resultcode;
	gRes->masterCharId = req->masterCharId;
	gRes->byFunction = req->byFunction;
	gRes->dwZenny = req->dwZenny;
	gPacket.SetPacketLen(sizeof(sTG_DOJO_FUNCTION_ADD_RES));
	g_pApp->Send(GetHandle(), &gPacket);
}


void CServerPassiveSession::RecvDojoScrambleReq(CNtlPacket * pPacket)
{
	WORD resulcode = GAME_FAIL;
	sGT_DOJO_SCRAMBLE_REQ * req = (sGT_DOJO_SCRAMBLE_REQ *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	
	if (pPlayer == NULL || pPlayer->GetPcInitState() == false)
		resulcode = GAME_FAIL;
	else if(pPlayer->GetGuild() == NULL || pPlayer->GetGuildID() == 0)
		resulcode = GAME_GUILD_NO_GUILD_FOUND;
	else if(Dbo_HasGuildFunction(pPlayer->GetGuild()->GetInfo()->qwGuildFunctionFlag, DBO_GUILD_FUNCTION_CAN_HAVE_DOJO) == false)
		resulcode = GAME_GUILD_DOJO_NEED_FUNCTION;
	else if(pPlayer->GetGuild()->IsGuildMaster(req->charId) == false)
		resulcode = GAME_GUILD_DOJO_YOU_ARE_NOT_A_PARTY_LEADER;
	else if(pPlayer->GetGuild()->HasDojo() == true) //dont allow war request if guild already has a dojo
		resulcode = GAME_GUILD_DOJO_SCRAMBLE_CHALLENGE_HAVE_DOJO;
	else if(pPlayer->GetGuild()->GetDojoTblidxWarRequest() != INVALID_TBLIDX)
		resulcode = GAME_GUILD_DOJO_SCRAMBLE_CHALLENGE_ONLY_ONETIME;
	else if (pPlayer->GetGuild()->CheckGuildReadyForDojo() == false)
		resulcode = COMMUNITY_FAIL;
	else
	{
		CDojo* pDojo = g_pDojoManager->GetDojoByDojoTblidx(req->dojoTblidx);
		
		if (pDojo == NULL)
			resulcode = GAME_GUILD_DOJO_NOT_FOUND;
		else if (pDojo->GetDojoData()->guildId == pPlayer->GetGuildID())
			resulcode = GAME_GUILD_DOJO_SELF_SCRAMBLE_FAIL;
		else if(pDojo->GetWarRequest())
			resulcode = GAME_GUILD_DOJO_SCRAMBLE_CHALLENGER_CANT_DO;
		else if (pDojo->GetWarInProgress())
			resulcode = GAME_GUILD_DOJO_SCRAMBLE_CHALLENGER_CANT_DO;
		else
			pDojo->RequestWar(pPlayer->GetGuildID(), pPlayer->GetCharID(), resulcode);

		if (resulcode == GAME_SUCCESS)
		{
			pPlayer->GetGuild()->SetDojoTblidxWarRequest(req->dojoTblidx);
			//get dojo owner. If owner not online, then war will begin

			CPlayer* pDojoOwner = g_pPlayerManager->FindPlayerWithCharID(pDojo->GetGuild()->GetInfo()->guildMaster);
			if (pDojoOwner && pDojoOwner->GetPcInitState())
			{
				pDojo->SetWarRequest(true); //only do this if the owner is online.. otherwise we wont be able to receive another request if the first one has been rejected
				g_pEventMgr->AddEvent(pDojo, &CDojo::OnEvent_WarRequestTimer, EVENT_DOJO_WAR_REQUEST, DBO_DOJO_SCRAMBLE_MAX_WAIT_TICK, 1, 0); //start timer

				//send nfy to dojo owner that guild requested war. This will pop-up a request window wether if he want to accept or not.
				CNtlPacket packet2(sizeof(sTU_DOJO_SCRAMBLE_NFY));
				sTU_DOJO_SCRAMBLE_NFY * res2 = (sTU_DOJO_SCRAMBLE_NFY *)packet2.GetPacketData();
				res2->wOpCode = TU_DOJO_SCRAMBLE_NFY;
				NTL_SAFE_WCSCPY(res2->wszName, pPlayer->GetGuild()->GetInfo()->wszName);
				packet2.SetPacketLen(sizeof(sTU_DOJO_SCRAMBLE_NFY));
				pDojoOwner->SendPacket(&packet2);
			}
			else
			{
				//begin war Preparation
				pDojo->BeginWarPreparation();
			}
		}
	}

	CNtlPacket packet(sizeof(sTG_DOJO_SCRAMBLE_RES));
	sTG_DOJO_SCRAMBLE_RES * res = (sTG_DOJO_SCRAMBLE_RES *)packet.GetPacketData();
	res->wOpCode = TG_DOJO_SCRAMBLE_RES;
	res->charId = req->charId;
	res->wResultCode = resulcode;
	res->dwReqZenny = req->dwReqZenny;
	packet.SetPacketLen(sizeof(sTG_DOJO_SCRAMBLE_RES));
	g_pApp->Send(GetHandle(), &packet);
}


void CServerPassiveSession::RecvDojoScrambleResponse(CNtlPacket * pPacket)
{
	sGT_DOJO_SCRAMBLE_RESPONSE * req = (sGT_DOJO_SCRAMBLE_RESPONSE *)pPacket->GetPacketData();

	WORD resulcode = GAME_FAIL;
	bool bIsRetry = false;
	bool bIsAccept = req->bIsAccept;

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);

	if (pPlayer == NULL || pPlayer->GetPcInitState() == false)
		resulcode = GAME_FAIL;
	else if (pPlayer->GetGuild() == NULL || pPlayer->GetGuildID() == 0)
		resulcode = GAME_GUILD_NO_GUILD_FOUND;
	else if (pPlayer->GetGuild()->IsGuildMaster(req->charId) == false)
		resulcode = GAME_GUILD_DOJO_YOU_ARE_NOT_A_PARTY_LEADER;
	else
	{
		CDojo* pDojo = pPlayer->GetGuild()->GetDojo();

		if (pDojo == NULL)
			resulcode = GAME_GUILD_DOJO_NOT_FOUND;
		else if (pDojo->GetWarRequest() == false)
			resulcode = GAME_GUILD_DOJO_SCRAMBLE_CHALLENGER_CANT_DO;
		else if (pDojo->GetWarInProgress())
			resulcode = GAME_GUILD_DOJO_SCRAMBLE_CHALLENGER_CANT_DO;
		else if(pDojo->GetDojoData()->challengeGuildId == INVALID_GUILDID)
			resulcode = GAME_FAIL;
		else
			pDojo->WarRequestReponse(bIsAccept, resulcode, bIsRetry); //here we might begin war
	}

	CNtlPacket packet(sizeof(sTG_DOJO_SCRAMBLE_RESPONSE));
	sTG_DOJO_SCRAMBLE_RESPONSE * res = (sTG_DOJO_SCRAMBLE_RESPONSE *)packet.GetPacketData();
	res->wOpCode = TG_DOJO_SCRAMBLE_RESPONSE;
	res->charId = req->charId;
	res->wResultCode = resulcode;
	res->bIsAccept = bIsAccept;
	res->bIsRetry = bIsRetry;
	packet.SetPacketLen(sizeof(sTG_DOJO_SCRAMBLE_RESPONSE));
	g_pApp->Send(GetHandle(), &packet);
}

void CServerPassiveSession::RecvDojoScrambleStateChange(CNtlPacket * pPacket)
{
	sGT_DOJO_SCRAMBLE_STATE_CHANGE * req = (sGT_DOJO_SCRAMBLE_STATE_CHANGE *)pPacket->GetPacketData();

	CDojo* pDojo = g_pDojoManager->GetDojoByDojoTblidx(req->dojoTblidx);
	if (pDojo)
	{
		pDojo->UpdateState(req->byState, req->tmNextStepTime);
	}
	else ERR_LOG(LOG_GENERAL, "Could not find dojo %u, byState %u", req->dojoTblidx, req->byState);
}


void CServerPassiveSession::RecvDojoScrambleRewardReq(CNtlPacket * pPacket)
{
	sGT_DOJO_SCRAMBLE_REWARD_REQ * req = (sGT_DOJO_SCRAMBLE_REWARD_REQ *)pPacket->GetPacketData();

	CChatServer* app = (CChatServer*)g_pApp;

	CDojo* pDojo = g_pDojoManager->GetDojoByDojoTblidx(req->dojoTblidx);
	if (pDojo)
	{
		CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pGuild)
		{
			//printf("RecvDojoScrambleRewardReq: Winner %u \n", req->guildId);
			CNtlPacket packet(sizeof(sTU_DOJO_SCRAMBLE_RESULT_NFY));
			sTU_DOJO_SCRAMBLE_RESULT_NFY * res = (sTU_DOJO_SCRAMBLE_RESULT_NFY *)packet.GetPacketData();
			res->wOpCode = TU_DOJO_SCRAMBLE_RESULT_NFY;
			res->winGuildId = req->guildId;
			packet.SetPacketLen(sizeof(sTU_DOJO_SCRAMBLE_RESULT_NFY));

			pGuild->Broadcast(&packet);

			GUILDID challengeGuildId = (pDojo->GetDojoData()->challengeGuildId == req->guildId) ? pDojo->GetDojoData()->guildId : pDojo->GetDojoData()->challengeGuildId;

			CGuild* pOpponent = g_pGuildManager->GetGuild(challengeGuildId);
			if (pOpponent)
			{
				pOpponent->Broadcast(&packet);
				pOpponent->SetDojo(NULL);
			}

			if (pDojo->GetDojoData()->guildId != req->guildId)
			{
				pDojo->ChangeDojoOwner(pGuild);
			}

			CNtlPacket packet2(sizeof(sTQ_DOJO_SCRAMBLE_REWARD_REQ));
			sTQ_DOJO_SCRAMBLE_REWARD_REQ * res2 = (sTQ_DOJO_SCRAMBLE_REWARD_REQ *)packet2.GetPacketData();
			res2->wOpCode = TQ_DOJO_SCRAMBLE_REWARD_REQ;
			res2->dojoTblidx = req->dojoTblidx;
			res2->dwAddGuildReputation = req->dwAddGuildReputation;
			res2->guildId = req->guildId;
			res2->qwGuildFunctionFlag = pGuild->GetInfo()->qwGuildFunctionFlag;
			memcpy(&res2->sItem, &req->sItem, sizeof(sITEM_DATA));
			packet2.SetPacketLen(sizeof(sTQ_DOJO_SCRAMBLE_REWARD_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet2);
		}
	}
	else ERR_LOG(LOG_GENERAL, "Could not find dojo %u", req->dojoTblidx);
}


void CServerPassiveSession::RecvDojoScrambleReset(CNtlPacket * pPacket)
{
	sGT_DOJO_SCRAMBLE_RESET * req = (sGT_DOJO_SCRAMBLE_RESET *)pPacket->GetPacketData();

	CDojo* pDojo = g_pDojoManager->GetDojoByDojoTblidx(req->dojoTblidx);
	if (pDojo)
	{
		pDojo->Reset(req->byState, req->tmNextStepTime);
	}
	else ERR_LOG(LOG_GENERAL, "Could not find dojo %u, byState %u", req->dojoTblidx, req->byState);
}

void CServerPassiveSession::RecvDojoScramblePointNfy(CNtlPacket * pPacket)
{
	sGT_DOJO_SCRAMBLE_POINT_NFY * req = (sGT_DOJO_SCRAMBLE_POINT_NFY *)pPacket->GetPacketData();

	CDojo* pDojo = g_pDojoManager->GetDojoByDojoTblidx(req->sData.dojoTblidx);
	if (pDojo)
	{
		pDojo->UpdateWarPoints(req->sData);
	}
}

void CServerPassiveSession::RecvDojoScrambleShieftSealStateNfy(CNtlPacket * pPacket)
{
	sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY * req = (sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY *)pPacket->GetPacketData();

	CDojo* pDojo = g_pDojoManager->GetDojoByDojoTblidx(req->dojoTblidx);
	if (pDojo)
	{
		pDojo->UpdateSeal(req->guildId, req->objectTblidx);
	}
}


//--------------------------------------------------------------------------------------//
//		DOJO INFO REQ
//--------------------------------------------------------------------------------------//
void	CServerPassiveSession::RecvDojoNpcInfoReq(CNtlPacket * pPacket)
{
	sGT_DOJO_NPC_INFO_REQ * req = (sGT_DOJO_NPC_INFO_REQ *)pPacket->GetPacketData();

	CNtlPacket gPacket(sizeof(sTG_DOJO_NPC_INFO_RES));
	sTG_DOJO_NPC_INFO_RES * gRes = (sTG_DOJO_NPC_INFO_RES *)gPacket.GetPacketData();
	gRes->wOpCode = TG_DOJO_NPC_INFO_RES;
	gRes->charId = req->charId;
	gRes->wResultCode = GAME_SUCCESS;

	g_pDojoManager->SendDojoInfo(req->dojoTblidx, &gRes->sData, gRes->wResultCode);

	gPacket.SetPacketLen(sizeof(sTG_DOJO_NPC_INFO_RES));
	g_pApp->Send(GetHandle(), &gPacket);
}

void CServerPassiveSession::RecvDojoSendTeleportProposal(CNtlPacket * pPacket)
{
	sGT_DOJO_SEND_TELEPORT_PROPOSAL * req = (sGT_DOJO_SEND_TELEPORT_PROPOSAL *)pPacket->GetPacketData();

	CGuild* pDefGuild = g_pGuildManager->GetGuild(req->defGuildId);

	CNtlPacket gPacket(sizeof(sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY));
	sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY * res = (sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY *)gPacket.GetPacketData();
	res->wOpCode = TG_DOJO_SEND_TELEPORT_PROPOSAL_NFY;
	res->byTeleportType = req->byTeleportType;
	res->wWaitTime = req->wWaitTime;
	res->destWorldId = req->destWorldId;
	res->destWorldTblidx = req->destWorldTblidx;
	res->vDestLoc = req->vDestDefLoc;
	res->destServerChannelId = DOJO_CHANNEL_INDEX;
	res->destServerIndex = 0;
	pDefGuild->FillOnlineMember(res->aMemberId, res->byCount);
	gPacket.SetPacketLen(sizeof(sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY));
	g_pServerInfoManager->Broadcast(&gPacket, DOJO_CHANNEL_INDEX);


	CGuild* pAttGuild = g_pGuildManager->GetGuild(req->attGuildId);

	CNtlPacket gPacket2(sizeof(sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY));
	sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY * res2 = (sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY *)gPacket2.GetPacketData();
	res2->wOpCode = TG_DOJO_SEND_TELEPORT_PROPOSAL_NFY;
	res2->byTeleportType = req->byTeleportType;
	res2->wWaitTime = req->wWaitTime;
	res2->destWorldId = req->destWorldId;
	res2->destWorldTblidx = req->destWorldTblidx;
	res2->vDestLoc = req->vDestAttLoc;
	res2->destServerChannelId = DOJO_CHANNEL_INDEX;
	res2->destServerIndex = 0;
	pAttGuild->FillOnlineMember(res2->aMemberId, res2->byCount);
	gPacket2.SetPacketLen(sizeof(sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY));
	g_pServerInfoManager->Broadcast(&gPacket2, DOJO_CHANNEL_INDEX);
}


//--------------------------------------------------------------------------------------//
//		BROADCAST MESSAGE
//--------------------------------------------------------------------------------------//
void CServerPassiveSession::RecvBroadcastingSystemNfy(CNtlPacket * pPacket)
{
	sGT_BROADCASTING_SYSTEM_NFY * req = (sGT_BROADCASTING_SYSTEM_NFY *)pPacket->GetPacketData();

	//send result
	CNtlPacket packet(sizeof(sTU_BROADCASTING_SYSTEM_NFY));
	sTU_BROADCASTING_SYSTEM_NFY * res = (sTU_BROADCASTING_SYSTEM_NFY *)packet.GetPacketData();
	res->wOpCode = TU_BROADCASTING_SYSTEM_NFY;
	res->byMsgType = req->byMsgType;

	switch (res->byMsgType)
	{
		case DBO_BROADCASTING_MSG_TYPE_ITEMUPGRADE: memcpy(&res->sData.sItemUpgrade, &req->sData.sItemUpgrade, sizeof(req->sData.sItemUpgrade));	break;
		case DBO_BROADCASTING_MSG_TYPE_ITEMMIX: memcpy(&res->sData.sItemMix, &req->sData.sItemMix, sizeof(req->sData.sItemMix));	break;
		case DBO_BROADCASTING_MSG_TYPE_TMQ: memcpy(&res->sData.sTmqRecord, &req->sData.sTmqRecord, sizeof(req->sData.sTmqRecord));	break;
		case DBO_BROADCASTING_MSG_TYPE_BUDOKAI: memcpy(&res->sData.sBudokaiRecord, &req->sData.sBudokaiRecord, sizeof(req->sData.sBudokaiRecord));	break;
		case DBO_BROADCASTING_MSG_TYPE_WAGUWAGU_1ST: memcpy(&res->sData.sWaguWagu1st, &req->sData.sWaguWagu1st, sizeof(req->sData.sWaguWagu1st));	break;
		case DBO_BROADCASTING_MSG_TYPE_CCBD: memcpy(&res->sData.sCCBD, &req->sData.sCCBD, sizeof(req->sData.sCCBD));	break;
	}

	packet.SetPacketLen(sizeof(sTU_BROADCASTING_SYSTEM_NFY));
	g_pPlayerManager->SendMsgToAll(&packet);
}


void CServerPassiveSession::RecvWaguCoinIncreaseNfy(CNtlPacket * pPacket)
{
	sGT_WAGUCOIN_INCREASE_NFY * req = (sGT_WAGUCOIN_INCREASE_NFY *)pPacket->GetPacketData();

	CPlayer* player = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	if (player && player->GetPcInitState())
	{
		player->SetWaguMachineCoin(req->dwWaguCoin);

		CNtlPacket packet(sizeof(sTU_WAGUWAGUCOIN_UPDATE_INFO));
		sTU_WAGUWAGUCOIN_UPDATE_INFO * res = (sTU_WAGUWAGUCOIN_UPDATE_INFO *)packet.GetPacketData();
		res->wOpCode = TU_WAGUWAGUCOIN_UPDATE_INFO;
		res->wWaguWaguCoin = (WORD)req->dwWaguCoin;
		packet.SetPacketLen(sizeof(sTU_WAGUWAGUCOIN_UPDATE_INFO));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}
}


void CServerPassiveSession::RecvEventCoinIncreaseNfy(CNtlPacket * pPacket)
{
	sGT_EVENTCOIN_INCREASE_NFY * req = (sGT_EVENTCOIN_INCREASE_NFY *)pPacket->GetPacketData();

	CPlayer* player = g_pPlayerManager->FindPlayerWithAccID(req->accountId);
	if (player && player->GetPcInitState())
	{
		player->SetEventMachineCoin(req->dwEventCoin);

		CNtlPacket packet(sizeof(sTU_EVENTCOIN_UPDATE_INFO));
		sTU_EVENTCOIN_UPDATE_INFO * res = (sTU_EVENTCOIN_UPDATE_INFO *)packet.GetPacketData();
		res->wOpCode = TU_EVENTCOIN_UPDATE_INFO;
		res->wEventCoin = (WORD)req->dwEventCoin;
		packet.SetPacketLen(sizeof(sTU_EVENTCOIN_UPDATE_INFO));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}
}