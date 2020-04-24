#include "stdafx.h"
#include "QueryServerSession.h"
#include "ChatServer.h"
#include "NtlPacketQT.h"
#include "NtlPacketTG.h"
#include "NtlPacketTU.h"
#include "AuctionHouse.h"
#include "DojoManager.h"
#include "GuildManager.h"


int CQueryServerSession::OnConnect()
{
	NTL_PRINT(PRINT_APP, "CONNECTED TO QUERY SERVER");
	CChatServer * app = (CChatServer*)g_pApp;
	app->SetQueryServerSession(this);

	NTL_PRINT(PRINT_APP, "Init Auctionhouse Manager");
	CAutionhouse* auctionhouse_manager = new CAutionhouse;

	NTL_PRINT(PRINT_APP, "Init Guild Manager");
	CGuildManager* guild_manager = new CGuildManager;

	NTL_PRINT(PRINT_APP, "Init Dojo Manager");
	CDojoManager* dojo_manager = new CDojoManager;

	return CNtlSession::OnConnect();
}


void CQueryServerSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "DISCONNECTED FROM QUERY SERVER");
	CChatServer * app = (CChatServer*)g_pApp;
	app->SetQueryServerSession(NULL);
}


int CQueryServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CChatServer * app = (CChatServer*)g_pApp;
	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("pHeader->wOpCode %d \n", pHeader->wOpCode);
	switch (pHeader->wOpCode)
	{
		case QT_GUILD_CREATE_RES: RecvGuildCreateRes(pPacket, app); break;
		case QT_GUILD_DATA: RecvGuildData(pPacket, app); break;
		case QT_GUILD_MEMBER_DATA: RecvGuildMemberData(pPacket, app); break;
		case QT_GUILD_DISBAND_RES: RecvGuildDisbandRes(pPacket, app); break;
		case QT_GUILD_INVITE_RES: RecvGuildInviteRes(pPacket, app); break;
		case QT_GUILD_LEAVE_RES: RecvGuildLeaveRes(pPacket, app); break;
		case QT_GUILD_KICK_OUT_RES: RecvGuildKickOutRes(pPacket, app); break;
		case QT_GUILD_APPOINT_SECOND_MASTER_RES: RecvGuildAppointSecondMasterRes(pPacket, app); break;
		case QT_GUILD_DISMISS_SECOND_MASTER_RES: RecvGuildDismissSecondMasterRes(pPacket, app); break;
		case QT_GUILD_CHANGE_GUILD_MASTER_RES: RecvGuildChangeGuildMasterRes(pPacket, app); break;
		case QT_GUILD_FUNCTION_ADD_RES: RecvGuildFunctionAddRes(pPacket, app); break;
		case QT_GUILD_GIVE_ZENNY_RES: RecvGuildGiveZeniRes(pPacket, app); break;
		case QT_GUILD_CHANGE_NOTICE_RES: RecvGuildChangeNoticeRes(pPacket, app); break;
		case QT_GUILD_CREATE_MARK_RES: RecvGuildCreateMarkRes(pPacket, app); break;
		case QT_GUILD_CHANGE_MARK_RES: RecvGuildChangeMarkRes(pPacket, app); break;
		case QT_GUILD_CHANGE_NAME_RES: RecvGuildChangeNameRes(pPacket, app); break;

		case QT_DOJO_DATA_RES: RecvDojoDataRes(pPacket, app); break;
		case QT_DOJO_CREATE_RES: RecvDojoCreateRes(pPacket, app); break;
		case QT_DOJO_UPDATE_RES: RecvDojoUpdateRes(pPacket, app); break;
		case QT_DOJO_DEL_RES: RecvDojoDeleteRes(pPacket, app); break;
		case QT_DOJO_FUNCTION_ADD_RES: RecvDojoFunctionAddRes(pPacket, app); break;

		case QT_DOJO_BUDOKAI_SEED_ADD_RES: RecvDojoBudokaiSeedAddRes(pPacket, app); break;
		case QT_DOJO_BUDOKAI_SEED_DEL_RES: RecvDojoBudokaiSeedDelRes(pPacket, app); break;
		case QT_DOJO_SCRAMBLE_REWARD_RES: RecvDojoScrambleRewardRes(pPacket, app); break;
		case QT_DOJO_CHANGE_NOTICE_RES: RecvDojoChangeNoticeRes(pPacket, app); break;
		case QT_TENKAICHIDAISIJYOU_SELL_RES: RecvAuctionHouseSellRes(pPacket, app); break;
		case QT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES: RecvAuctionHouseSellCancelRes(pPacket, app); break;
		case QT_TENKAICHIDAISIJYOU_BUY_RES: RecvAuctionHouseBuyRes(pPacket, app); break;
		case QT_TENKAICHIDAISIJYOU_SERVERDATA_RES: RecvAuctionHouseServerDataRes(pPacket, app); break;
		case QT_TENKAICHIDAISIJYOU_PERIODEND_RES: RecvAuctionHousePeriodEndRes(pPacket, app); break;

		case QT_HLS_SLOT_MACHINE_EXTRACT_RES: RecvHlsSlotMachineExtractRes(pPacket, app); break;

		default: {} break;
	}


	return NTL_SUCCESS;
}


void CQueryServerSession::RecvGuildCreateRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_CREATE_RES * req = (sQT_GUILD_CREATE_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->aMemberData[0].charId);
	if (pPlayer)
	{
		CNtlPacket gamePacket0(sizeof(sTG_GUILD_CREATE_RES));
		sTG_GUILD_CREATE_RES * gameRes0 = (sTG_GUILD_CREATE_RES *)gamePacket0.GetPacketData();
		gameRes0->wOpCode = TG_GUILD_CREATE_RES;
		gameRes0->creatorCharId = req->aMemberData[0].charId;
		gameRes0->wResultCode = req->wResultCode;
		gameRes0->dwSpentZenny = req->dwSpentZenny;
		gamePacket0.SetPacketLen(sizeof(sTG_GUILD_CREATE_RES));
		app->Send(g_pServerInfoManager->GetGsSession(pPlayer->GetChannel()), &gamePacket0);
	}

	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pGuildManager->CreateGuild(req->guildData, req->aMemberData, req->byMemberCount);
	}
}

void CQueryServerSession::RecvGuildData(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_DATA * req = (sQT_GUILD_DATA *)pPacket->GetPacketData();

	g_pGuildManager->CreateGuild(req->guildData);

	//send to all channels (required if chat server restarts)
	CNtlPacket gamePacket(sizeof(sTG_GUILD_CREATED_NFY));
	sTG_GUILD_CREATED_NFY * gameRes = (sTG_GUILD_CREATED_NFY *)gamePacket.GetPacketData();
	gameRes->wOpCode = TU_GUILD_CREATED_NFY;
	gameRes->guildId = req->guildData.guildId;
	NTL_SAFE_WCSCPY(gameRes->wszGuildName, req->guildData.wszName);
	memcpy(&gameRes->guildMark, &req->guildData.sMark, sizeof(sDBO_GUILD_MARK));
	memcpy(&gameRes->sDogi, &req->guildData.sDogi, sizeof(sDBO_DOGI_DATA));
	gameRes->qwGuildFunctionFlag = req->guildData.qwGuildFunctionFlag;
	gamePacket.SetPacketLen(sizeof(sTG_GUILD_CREATED_NFY));
	g_pServerInfoManager->Broadcast(&gamePacket);
}

void CQueryServerSession::RecvGuildMemberData(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_MEMBER_DATA * req = (sQT_GUILD_MEMBER_DATA *)pPacket->GetPacketData();

	if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
	{
		pGuild->AddMemberInfo(req->guildMemberData);
	}
}


void CQueryServerSession::RecvGuildDisbandRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_DISBAND_RES * req = (sQT_GUILD_DISBAND_RES *)pPacket->GetPacketData();

	if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
	{
		if (req->bByUser)
		{
			if (CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(pGuild->GetGuildMaster()))
			{
				CNtlPacket packet(sizeof(sTU_GUILD_DISBAND_RES));
				sTU_GUILD_DISBAND_RES * res = (sTU_GUILD_DISBAND_RES *)packet.GetPacketData();
				res->wOpCode = TU_GUILD_DISBAND_RES;
				res->wResultCode = req->wResultCode;
				packet.SetPacketLen(sizeof(sTU_GUILD_DISBAND_RES));
				app->Send(pPlayer->GetClientSessionID(), &packet);
			}
		}

		if (req->wResultCode == CHAT_SUCCESS)
		{
			pGuild->DisbandGuild();

			CNtlPacket packet2(sizeof(sTG_GUILD_DESTROYED_NFY));
			sTG_GUILD_DESTROYED_NFY * res2 = (sTG_GUILD_DESTROYED_NFY *)packet2.GetPacketData();
			res2->wOpCode = TG_GUILD_DESTROYED_NFY;
			res2->guildId = req->guildId;
			packet2.SetPacketLen(sizeof(sTG_GUILD_DESTROYED_NFY));
			g_pServerInfoManager->Broadcast(&packet2);
		}
	}
}


void CQueryServerSession::RecvGuildInviteRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_INVITE_RES * req = (sQT_GUILD_INVITE_RES *)pPacket->GetPacketData();

	if (req->wResultCode == GAME_SUCCESS)
	{
		CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pGuild)
		{
			CPlayer* pNewMember = g_pPlayerManager->FindPlayerWithCharID(req->inviteeCharId);

			//send new member joined info to guild
			CNtlPacket packet(sizeof(sTU_GUILD_MEMBER_JOINED_NFY));
			sTU_GUILD_MEMBER_JOINED_NFY * res = (sTU_GUILD_MEMBER_JOINED_NFY *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_MEMBER_JOINED_NFY;

			if (pNewMember)
			{
				res->memberInfo.bIsOnline = true;
				res->memberInfo.byClass = pNewMember->GetClass();
				res->memberInfo.byLevel = pNewMember->GetLevel();
				res->memberInfo.byRace = pNewMember->GetRace();
				res->memberInfo.charId = pNewMember->GetCharID();
				res->memberInfo.dwReputation = pNewMember->GetReputation();
				res->memberInfo.mapNameTblidx = pNewMember->GetMapNameTblidx();
				NTL_SAFE_WCSCPY(res->memberInfo.wszMemberName, pNewMember->GetCharName());
			}
			else res->memberInfo.bIsOnline = false;

			packet.SetPacketLen(sizeof(sTU_GUILD_MEMBER_JOINED_NFY));
			pGuild->Broadcast(&packet);

			pGuild->AddMemberInfo(&res->memberInfo);

			//send to gameserver
			CNtlPacket gamePacket(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY * gameRes = (sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY *)gamePacket.GetPacketData();
			gameRes->wOpCode = TG_GUILD_PLAYER_GUILDID_CHANGED_NFY;
			gameRes->newGuildId = req->guildId;
			gameRes->charId = req->inviteeCharId;
			gamePacket.SetPacketLen(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			g_pServerInfoManager->Broadcast(&gamePacket);

			if (pNewMember)
			{
				pGuild->InsertOnlineMember(pNewMember->GetCharID(), pNewMember);
				pNewMember->SetGuild(pGuild);

				//load guild info
				CNtlPacket packet2(sizeof(sTU_GUILD_INFO));
				sTU_GUILD_INFO * res2 = (sTU_GUILD_INFO *)packet2.GetPacketData();
				res2->wOpCode = TU_GUILD_INFO;
				memcpy(&res2->guildInfo, pGuild->GetInfo(), sizeof(sDBO_GUILD_INFO));
				packet2.SetPacketLen(sizeof(sTU_GUILD_INFO));
				g_pApp->Send(pNewMember->GetClientSessionID(), &packet2);

				//load member info
				pGuild->LoadGuildMembers(pNewMember);
			}
		}
	}
}

void CQueryServerSession::RecvGuildLeaveRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_LEAVE_RES * req = (sQT_GUILD_LEAVE_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->leavingMemberCharId);

	if (req->wResultCode == CHAT_SUCCESS)
	{
		if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
		{
			if(pPlayer)
				pPlayer->SetGuild(NULL);

			pGuild->RemoveSecondGuildMaster(req->leavingMemberCharId);
			pGuild->DelMemberInfo(req->leavingMemberCharId);
			pGuild->EraseOnlineMember(req->leavingMemberCharId);

			CNtlPacket packet(sizeof(sTU_GUILD_MEMBER_LEFT_NFY));
			sTU_GUILD_MEMBER_LEFT_NFY * res = (sTU_GUILD_MEMBER_LEFT_NFY *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_MEMBER_LEFT_NFY;
			res->memberCharId = req->leavingMemberCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_MEMBER_LEFT_NFY));
			pGuild->Broadcast(&packet);

			CNtlPacket packet2(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY * res2 = (sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY *)packet2.GetPacketData();
			res2->wOpCode = TG_GUILD_PLAYER_GUILDID_CHANGED_NFY;
			res2->charId = req->leavingMemberCharId;
			res2->newGuildId = 0;
			packet2.SetPacketLen(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			g_pServerInfoManager->Broadcast(&packet2);
		}
	}

	if (pPlayer)
	{
		CNtlPacket packet(sizeof(sTU_GUILD_LEAVE_RES));
		sTU_GUILD_LEAVE_RES * res = (sTU_GUILD_LEAVE_RES *)packet.GetPacketData();
		res->wOpCode = TU_GUILD_LEAVE_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTU_GUILD_LEAVE_RES));
		app->Send(pPlayer->GetClientSessionID(), &packet);
	}
}

void CQueryServerSession::RecvGuildKickOutRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_KICK_OUT_RES * req = (sQT_GUILD_KICK_OUT_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->kickedOutMemberCharId);

	if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
	{
		if (req->wResultCode == CHAT_SUCCESS)
		{
			CNtlPacket packet(sizeof(sTU_GUILD_MEMBER_KICKED_OUT_NFY));
			sTU_GUILD_MEMBER_KICKED_OUT_NFY * res = (sTU_GUILD_MEMBER_KICKED_OUT_NFY *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_MEMBER_KICKED_OUT_NFY;
			res->memberCharId = req->kickedOutMemberCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_MEMBER_KICKED_OUT_NFY));
			pGuild->Broadcast(&packet);

			if (pPlayer)
				pPlayer->SetGuild(NULL);

			pGuild->RemoveSecondGuildMaster(req->kickedOutMemberCharId);
			pGuild->DelMemberInfo(req->kickedOutMemberCharId);
			pGuild->EraseOnlineMember(req->kickedOutMemberCharId);

			CNtlPacket packet2(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY * res2 = (sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY *)packet2.GetPacketData();
			res2->wOpCode = TG_GUILD_PLAYER_GUILDID_CHANGED_NFY;
			res2->charId = req->kickedOutMemberCharId;
			res2->newGuildId = 0;
			packet2.SetPacketLen(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			g_pServerInfoManager->Broadcast(&packet2);
		}

		if (CPlayer* pMaster = g_pPlayerManager->FindPlayerWithCharID(pGuild->GetGuildMaster()))
		{
			CNtlPacket packet(sizeof(sTU_GUILD_KICK_OUT_RES));
			sTU_GUILD_KICK_OUT_RES * res = (sTU_GUILD_KICK_OUT_RES *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_KICK_OUT_RES;
			res->wResultCode = req->wResultCode;
			res->kickedOutMemberCharId = req->kickedOutMemberCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_KICK_OUT_RES));
			g_pApp->Send(pMaster->GetClientSessionID(), &packet);
		}
	}
}

void CQueryServerSession::RecvGuildAppointSecondMasterRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_APPOINT_SECOND_MASTER_RES * req = (sQT_GUILD_APPOINT_SECOND_MASTER_RES *)pPacket->GetPacketData();

	if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
	{
		if (req->wResultCode == CHAT_SUCCESS)
		{
			pGuild->AddSecondGuildMaster(req->targetMemberCharId);

			CNtlPacket packet(sizeof(sTU_GUILD_SECOND_MASTER_APPOINTED_NFY));
			sTU_GUILD_SECOND_MASTER_APPOINTED_NFY * res = (sTU_GUILD_SECOND_MASTER_APPOINTED_NFY *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_SECOND_MASTER_APPOINTED_NFY;
			res->memberCharId = req->targetMemberCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_SECOND_MASTER_APPOINTED_NFY));
			pGuild->Broadcast(&packet, pGuild->GetGuildMaster());
		}

		if (CPlayer* pMaster = g_pPlayerManager->FindPlayerWithCharID(pGuild->GetGuildMaster()))
		{
			CNtlPacket packet(sizeof(sTU_GUILD_APPOINT_SECOND_MASTER_RES));
			sTU_GUILD_APPOINT_SECOND_MASTER_RES * res = (sTU_GUILD_APPOINT_SECOND_MASTER_RES *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_APPOINT_SECOND_MASTER_RES;
			res->wResultCode = req->wResultCode;
			res->secondMasterCharId = req->targetMemberCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_APPOINT_SECOND_MASTER_RES));
			g_pApp->Send(pMaster->GetClientSessionID(), &packet);
		}
	}
}

void CQueryServerSession::RecvGuildDismissSecondMasterRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_DISMISS_SECOND_MASTER_RES * req = (sQT_GUILD_DISMISS_SECOND_MASTER_RES *)pPacket->GetPacketData();

	if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
	{
		if (req->wResultCode == CHAT_SUCCESS)
		{
			pGuild->RemoveSecondGuildMaster(req->targetMemberCharId);

			CNtlPacket packet(sizeof(sTU_GUILD_SECOND_MASTER_DISMISSED_NFY));
			sTU_GUILD_SECOND_MASTER_DISMISSED_NFY * res = (sTU_GUILD_SECOND_MASTER_DISMISSED_NFY *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_SECOND_MASTER_DISMISSED_NFY;
			res->memberCharId = req->targetMemberCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_SECOND_MASTER_DISMISSED_NFY));
			pGuild->Broadcast(&packet, pGuild->GetGuildMaster());
		}

		if (CPlayer* pMaster = g_pPlayerManager->FindPlayerWithCharID(pGuild->GetGuildMaster()))
		{
			CNtlPacket packet(sizeof(sTU_GUILD_DISMISS_SECOND_MASTER_RES));
			sTU_GUILD_DISMISS_SECOND_MASTER_RES * res = (sTU_GUILD_DISMISS_SECOND_MASTER_RES *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_DISMISS_SECOND_MASTER_RES;
			res->wResultCode = req->wResultCode;
			res->secondMasterCharId = req->targetMemberCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_DISMISS_SECOND_MASTER_RES));
			g_pApp->Send(pMaster->GetClientSessionID(), &packet);
		}
	}
}

void CQueryServerSession::RecvGuildChangeGuildMasterRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_CHANGE_GUILD_MASTER_RES * req = (sQT_GUILD_CHANGE_GUILD_MASTER_RES *)pPacket->GetPacketData();

	if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
	{
		if (CPlayer* pMaster = g_pPlayerManager->FindPlayerWithCharID(pGuild->GetGuildMaster()))
		{
			CNtlPacket packet(sizeof(sTU_GUILD_CHANGE_GUILD_MASTER_RES));
			sTU_GUILD_CHANGE_GUILD_MASTER_RES * res = (sTU_GUILD_CHANGE_GUILD_MASTER_RES *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_CHANGE_GUILD_MASTER_RES;
			res->wResultCode = req->wResultCode;
			res->newMasterCharId = req->newMasterCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_CHANGE_GUILD_MASTER_RES));
			g_pApp->Send(pMaster->GetClientSessionID(), &packet);
		}

		if (req->wResultCode == CHAT_SUCCESS)
		{
			CNtlPacket packet(sizeof(sTU_GUILD_GUILD_MASTER_CHANGED_NFY));
			sTU_GUILD_GUILD_MASTER_CHANGED_NFY * res = (sTU_GUILD_GUILD_MASTER_CHANGED_NFY *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_GUILD_MASTER_CHANGED_NFY;
			res->memberCharId = req->newMasterCharId;
			packet.SetPacketLen(sizeof(sTU_GUILD_GUILD_MASTER_CHANGED_NFY));
			pGuild->Broadcast(&packet, pGuild->GetGuildMaster());

			pGuild->SetGuildMaster(req->newMasterCharId);
		}
	}
}


void CQueryServerSession::RecvGuildFunctionAddRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_FUNCTION_ADD_RES * req = (sQT_GUILD_FUNCTION_ADD_RES *)pPacket->GetPacketData();

	CNtlPacket gamePacket0(sizeof(sTG_GUILD_FUNCTION_ADD_RES));
	sTG_GUILD_FUNCTION_ADD_RES * gameRes0 = (sTG_GUILD_FUNCTION_ADD_RES *)gamePacket0.GetPacketData();
	gameRes0->wOpCode = TG_GUILD_FUNCTION_ADD_RES;
	gameRes0->masterCharId = req->masterCharId;
	gameRes0->wResultCode = req->wResultCode;
	gameRes0->dwZenny = req->dwZenny;
	gameRes0->qwGuildFunctionFlag = req->qwGuildFunctionFlag;
	gamePacket0.SetPacketLen(sizeof(sTG_GUILD_FUNCTION_ADD_RES));
	app->Send(g_pServerInfoManager->GetGsSession(req->byServerChannelIndex), &gamePacket0);

	if (req->wResultCode == GAME_SUCCESS)
	{
		CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pGuild)
		{
			pGuild->SetGuildFunctionFlag(req->qwGuildFunctionFlag);
			pGuild->UpdateReputation(Dbo_GetGuildFunctionInfo(static_cast<eDBO_GUILD_FUNCTION>(req->byFunction))->dwRequiredPoint, false);

			//send to user
			CNtlPacket packet2(sizeof(sTU_GUILD_FUNCTION_CHANGE_NFY));
			sTU_GUILD_FUNCTION_CHANGE_NFY * res2 = (sTU_GUILD_FUNCTION_CHANGE_NFY *)packet2.GetPacketData();
			res2->wOpCode = TU_GUILD_FUNCTION_CHANGE_NFY;
			res2->byNewFunc = req->byFunction;
			res2->qwGuildFunctionFlag = req->qwGuildFunctionFlag;
			packet2.SetPacketLen(sizeof(sTU_GUILD_FUNCTION_CHANGE_NFY));
			pGuild->Broadcast(&packet2);

			//send to all channels
			CNtlPacket packet(sizeof(sTG_GUILD_INFO_REFRESHED_NFY));
			sTG_GUILD_INFO_REFRESHED_NFY * res = (sTG_GUILD_INFO_REFRESHED_NFY *)packet.GetPacketData();
			res->wOpCode = TG_GUILD_INFO_REFRESHED_NFY;
			res->guildId = req->guildId;
			res->qwGuildFunctionFlag = req->qwGuildFunctionFlag;
			::ZeroMemory(res->wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1);
			memset(&res->sDogi, -1, sizeof(sDBO_DOGI_DATA));
			memset(&res->sMark, -1, sizeof(sDBO_GUILD_MARK));
			packet.SetPacketLen(sizeof(sTG_GUILD_INFO_REFRESHED_NFY));
			g_pServerInfoManager->Broadcast(&packet);
		}
	}
}

void CQueryServerSession::RecvGuildGiveZeniRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_GIVE_ZENNY_RES * req = (sQT_GUILD_GIVE_ZENNY_RES *)pPacket->GetPacketData();

	CNtlPacket gamePacket0(sizeof(sTG_GUILD_GIVE_ZENNY_RES));
	sTG_GUILD_GIVE_ZENNY_RES * gameRes0 = (sTG_GUILD_GIVE_ZENNY_RES *)gamePacket0.GetPacketData();
	gameRes0->wOpCode = TG_GUILD_GIVE_ZENNY_RES;
	gameRes0->charId = req->charId;
	gameRes0->wResultCode = req->wResultCode;
	gameRes0->dwZenny = req->dwZenny;
	gamePacket0.SetPacketLen(sizeof(sTG_GUILD_GIVE_ZENNY_RES));
	app->Send(g_pServerInfoManager->GetGsSession(req->byServerChannelIndex), &gamePacket0);

	if (req->wResultCode == GAME_SUCCESS)
	{
		CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pGuild)
		{
			//send to user
			CNtlPacket packet(sizeof(sTU_GUILD_GIVE_ZENNY_NFY));
			sTU_GUILD_GIVE_ZENNY_NFY * res = (sTU_GUILD_GIVE_ZENNY_NFY *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_GIVE_ZENNY_NFY;
			res->charId = req->charId;
			res->dwZenny = req->dwZenny;
			packet.SetPacketLen(sizeof(sTU_GUILD_GIVE_ZENNY_NFY));
			pGuild->Broadcast(&packet);

			//update guild
			pGuild->UpdateReputation(req->dwAddReputaion, true);
		}
	}
}


void CQueryServerSession::RecvGuildChangeNoticeRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_CHANGE_NOTICE_RES * req = (sQT_GUILD_CHANGE_NOTICE_RES *)pPacket->GetPacketData();

	if (CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId))
	{
		CNtlPacket packet(sizeof(sTU_GUILD_CHANGE_NOTICE_RES));
		sTU_GUILD_CHANGE_NOTICE_RES * res = (sTU_GUILD_CHANGE_NOTICE_RES *)packet.GetPacketData();
		res->wOpCode = TU_GUILD_CHANGE_NOTICE_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTU_GUILD_CHANGE_NOTICE_RES));
		g_pApp->Send(pPlayer->GetClientSessionID(), &packet);
	}

	if (req->wResultCode == CHAT_SUCCESS)
	{
		if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
		{
			pGuild->UpdateNotice(req->awchCharName, req->awchNotice);

			CNtlPacket packet2(sizeof(sTU_GUILD_CHANGE_NOTICE_NFY));
			sTU_GUILD_CHANGE_NOTICE_NFY * res2 = (sTU_GUILD_CHANGE_NOTICE_NFY *)packet2.GetPacketData();
			res2->wOpCode = TU_GUILD_CHANGE_NOTICE_NFY;
			NTL_SAFE_WCSCPY(res2->awchNotice, req->awchNotice);
			res2->wNoticeLengthInUnicode = req->wNoticeLengthInUnicode;
			NTL_SAFE_WCSCPY(res2->awchCharName, req->awchCharName);
			packet2.SetPacketLen(sizeof(sTU_GUILD_CHANGE_NOTICE_NFY));
			pGuild->Broadcast(&packet2);
		}
	}
}


void CQueryServerSession::RecvGuildCreateMarkRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_CREATE_MARK_RES * req = (sQT_GUILD_CREATE_MARK_RES *)pPacket->GetPacketData();

	CNtlPacket gamePacket0(sizeof(sTG_GUILD_CREATE_MARK_RES));
	sTG_GUILD_CREATE_MARK_RES * gameRes0 = (sTG_GUILD_CREATE_MARK_RES *)gamePacket0.GetPacketData();
	gameRes0->wOpCode = TG_GUILD_CREATE_MARK_RES;
	gameRes0->charId = req->charId;
	gameRes0->wResultCode = req->wResultCode;
	memcpy(&gameRes0->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));
	gamePacket0.SetPacketLen(sizeof(sTG_GUILD_CREATE_MARK_RES));
	app->Send(g_pServerInfoManager->GetGsSession(req->byServerChannelIndex), &gamePacket0);

	if (req->wResultCode == GAME_SUCCESS)
	{
		CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pGuild)
		{
			//update guild
			pGuild->SetGuildMark(req->sMark);

			//send to all channels
			CNtlPacket packet(sizeof(sTG_GUILD_INFO_REFRESHED_NFY));
			sTG_GUILD_INFO_REFRESHED_NFY * res = (sTG_GUILD_INFO_REFRESHED_NFY *)packet.GetPacketData();
			res->wOpCode = TG_GUILD_INFO_REFRESHED_NFY;
			res->guildId = req->guildId;
			res->qwGuildFunctionFlag = 0;
			::ZeroMemory(res->wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1);
			memset(&res->sDogi, -1, sizeof(sDBO_DOGI_DATA));
			memcpy(&res->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));
			packet.SetPacketLen(sizeof(sTG_GUILD_INFO_REFRESHED_NFY));
			g_pServerInfoManager->Broadcast(&packet);
		}
	}
}

void CQueryServerSession::RecvGuildChangeMarkRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_CHANGE_MARK_RES * req = (sQT_GUILD_CHANGE_MARK_RES *)pPacket->GetPacketData();

	CNtlPacket gamePacket0(sizeof(sTG_GUILD_CHANGE_MARK_RES));
	sTG_GUILD_CHANGE_MARK_RES * gameRes0 = (sTG_GUILD_CHANGE_MARK_RES *)gamePacket0.GetPacketData();
	gameRes0->wOpCode = TG_GUILD_CHANGE_MARK_RES;
	gameRes0->charId = req->charId;
	gameRes0->wResultCode = req->wResultCode;
	memcpy(&gameRes0->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));
	gameRes0->dwZenny = req->dwZenny;
	gamePacket0.SetPacketLen(sizeof(sTG_GUILD_CHANGE_MARK_RES));
	app->Send(g_pServerInfoManager->GetGsSession(req->byServerChannelIndex), &gamePacket0);

	if (req->wResultCode == GAME_SUCCESS)
	{
		CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pGuild)
		{
			//update guild
			pGuild->SetGuildMark(req->sMark);

			//send to all channels
			CNtlPacket packet(sizeof(sTG_GUILD_INFO_REFRESHED_NFY));
			sTG_GUILD_INFO_REFRESHED_NFY * res = (sTG_GUILD_INFO_REFRESHED_NFY *)packet.GetPacketData();
			res->wOpCode = TG_GUILD_INFO_REFRESHED_NFY;
			res->guildId = req->guildId;
			res->qwGuildFunctionFlag = 0;
			::ZeroMemory(res->wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1);
			memset(&res->sDogi, -1, sizeof(sDBO_DOGI_DATA));
			memcpy(&res->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));
			packet.SetPacketLen(sizeof(sTG_GUILD_INFO_REFRESHED_NFY));
			g_pServerInfoManager->Broadcast(&packet);
		}
	}
}

void CQueryServerSession::RecvGuildChangeNameRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_GUILD_CHANGE_NAME_RES * req = (sQT_GUILD_CHANGE_NAME_RES *)pPacket->GetPacketData();

	CNtlPacket gamePacket0(sizeof(sTG_GUILD_CHANGE_NAME_RES));
	sTG_GUILD_CHANGE_NAME_RES * gameRes0 = (sTG_GUILD_CHANGE_NAME_RES *)gamePacket0.GetPacketData();
	gameRes0->wOpCode = TG_GUILD_CHANGE_NAME_RES;
	gameRes0->charId = req->charId;
	gameRes0->wResultCode = req->wResultCode;
	gameRes0->byPlace = req->byPlace;
	gameRes0->byPos = req->byPos;
	gameRes0->guildId = req->guildId;
	gameRes0->itemId = req->itemId;
	NTL_SAFE_WCSCPY(gameRes0->wszGuildName, req->wszGuildName);
	gamePacket0.SetPacketLen(sizeof(sTG_GUILD_CHANGE_NAME_RES));
	app->Send(g_pServerInfoManager->GetGsSession(req->byServerChannelIndex), &gamePacket0);

	if (req->wResultCode == GAME_SUCCESS)
	{
		CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pGuild)
		{
			//update guild
			pGuild->SetGuildName(req->wszGuildName);

			if (CDojo* pDojo = pGuild->GetDojo())
			{
				NTL_SAFE_WCSCPY(pDojo->GetDojoData()->wszName, req->wszGuildName);
			}

			//send to all channels
			CNtlPacket packet(sizeof(sTG_GUILD_INFO_REFRESHED_NFY));
			sTG_GUILD_INFO_REFRESHED_NFY * res = (sTG_GUILD_INFO_REFRESHED_NFY *)packet.GetPacketData();
			res->wOpCode = TG_GUILD_INFO_REFRESHED_NFY;
			res->guildId = req->guildId;
			res->qwGuildFunctionFlag = 0;
			NTL_SAFE_WCSCPY(res->wszGuildName, req->wszGuildName);
			memset(&res->sDogi, -1, sizeof(sDBO_DOGI_DATA));
			memset(&res->sMark, -1, sizeof(sDBO_GUILD_MARK));
			packet.SetPacketLen(sizeof(sTG_GUILD_INFO_REFRESHED_NFY));
			g_pServerInfoManager->Broadcast(&packet);
		}
	}
}


void CQueryServerSession::RecvDojoDataRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_DATA_RES * req = (sQT_DOJO_DATA_RES *)pPacket->GetPacketData();

	for (BYTE i = 0; i < req->byCount; i++)
	{
		sDBO_DOJO_DATA* pDojo = g_pDojoManager->CreateDojo(req->asDojoData[i]);
		if (pDojo)
		{
			//send dojo created nfy to all channels. Only used when chat server starts while channels online
			CNtlPacket gPacket2(sizeof(sTG_DOJO_CREATED_NFY));
			sTG_DOJO_CREATED_NFY * gRes2 = (sTG_DOJO_CREATED_NFY *)gPacket2.GetPacketData();
			gRes2->wOpCode = TG_DOJO_CREATED_NFY;
			memcpy(&gRes2->sDojoData, pDojo, sizeof(sDBO_DOJO_DATA));
			gPacket2.SetPacketLen(sizeof(sTG_DOJO_CREATED_NFY));
			g_pServerInfoManager->Broadcast(&gPacket2);
		}
	}
}

void CQueryServerSession::RecvDojoCreateRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_CREATE_RES * req = (sQT_DOJO_CREATE_RES *)pPacket->GetPacketData();

	sDBO_DOJO_DATA* pDojoData = NULL;

	if (req->wResultCode == GAME_SUCCESS)
	{
		pDojoData = g_pDojoManager->CreateDojo(req->charId, req->guildId, req->dojoTblidx);
	}

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer)
	{
		CNtlPacket packet(sizeof(sTG_DOJO_CREATE_RES));
		sTG_DOJO_CREATE_RES * res = (sTG_DOJO_CREATE_RES *)packet.GetPacketData();
		res->wOpCode = TG_DOJO_CREATE_RES;
		res->charId = req->charId;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTG_DOJO_CREATE_RES));
		app->Send(g_pServerInfoManager->GetGsSession(pPlayer->GetChannel()), &packet);

		if (pDojoData)
		{
			//send to client
			CNtlPacket packet2(sizeof(sTU_DOJO_CREATED_NFY));
			sTU_DOJO_CREATED_NFY * res2 = (sTU_DOJO_CREATED_NFY *)packet2.GetPacketData();
			res2->wOpCode = TU_DOJO_CREATED_NFY;
			memcpy(&res2->sDojoData, pDojoData, sizeof(sDBO_DOJO_DATA));
			packet2.SetPacketLen(sizeof(sTU_DOJO_CREATED_NFY));
			g_pApp->Send(pPlayer->GetClientSessionID(), &packet2);
		}
	}
}

void CQueryServerSession::RecvDojoUpdateRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_UPDATE_RES * req = (sQT_DOJO_UPDATE_RES *)pPacket->GetPacketData();
	//printf("RecvDojoUpdateRes: req->guildId %u, req->sDojoData.guildId %u, req->wResultCode %u\n", req->guildId, req->sDojoData.guildId, req->wResultCode);
	if (req->wResultCode == CHAT_SUCCESS)
	{
		CNtlPacket packet(sizeof(sTG_DOJO_UPDATE_RES));
		sTG_DOJO_UPDATE_RES * res = (sTG_DOJO_UPDATE_RES *)packet.GetPacketData();
		res->wOpCode = TG_DOJO_UPDATE_RES;
		res->guildId = req->guildId;
		res->newGuildId = req->sDojoData.guildId;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTG_DOJO_UPDATE_RES));
		g_pServerInfoManager->Broadcast(&packet);
	}
}

void CQueryServerSession::RecvDojoDeleteRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_DEL_RES * req = (sQT_DOJO_DEL_RES *)pPacket->GetPacketData();

	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pDojoManager->DestroyDojo(req->guildId, req->dojoTblidx);
	}

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (pPlayer)
	{
		CNtlPacket packet(sizeof(sTG_DOJO_DELETE_RES));
		sTG_DOJO_DELETE_RES * res = (sTG_DOJO_DELETE_RES *)packet.GetPacketData();
		res->wOpCode = TG_DOJO_DELETE_RES;
		res->charId = req->charId;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTG_DOJO_DELETE_RES));
		app->Send(g_pServerInfoManager->GetGsSession(pPlayer->GetChannel()), &packet);
	}
}

void CQueryServerSession::RecvDojoFunctionAddRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_FUNCTION_ADD_RES * req = (sQT_DOJO_FUNCTION_ADD_RES *)pPacket->GetPacketData();

	CNtlPacket gamePacket0(sizeof(sTG_DOJO_FUNCTION_ADD_RES));
	sTG_DOJO_FUNCTION_ADD_RES * gameRes0 = (sTG_DOJO_FUNCTION_ADD_RES *)gamePacket0.GetPacketData();
	gameRes0->wOpCode = TG_DOJO_FUNCTION_ADD_RES;
	gameRes0->masterCharId = req->masterCharId;
	gameRes0->wResultCode = req->wResultCode;
	gameRes0->dwZenny = req->dwZenny;
	gameRes0->qwGuildFunctionFlag = req->qwGuildFunctionFlag;
	gameRes0->byFunction = req->byFunction;
	gameRes0->guildId = req->guildId;
	gamePacket0.SetPacketLen(sizeof(sTG_DOJO_FUNCTION_ADD_RES));
	g_pServerInfoManager->Broadcast(&gamePacket0);

	if (req->wResultCode == GAME_SUCCESS)
	{
		CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pGuild)
		{
			pGuild->SetGuildFunctionFlag(req->qwGuildFunctionFlag);
			pGuild->UpdateReputation(Dbo_GetGuildFunctionInfo(static_cast<eDBO_GUILD_FUNCTION>(req->byFunction))->dwRequiredPoint, false);

			BYTE byNewDojoLevel = Dbo_GetDojoLevel(req->byFunction);
			if (pGuild->GetDojo() && byNewDojoLevel > 0)
			{
				if (sDBO_DOJO_DATA* pDojoData = pGuild->GetDojo()->GetDojoData())
				{
					pDojoData->byLevel = byNewDojoLevel;
				}
			}

			//send to user
			CNtlPacket packet2(sizeof(sTU_GUILD_FUNCTION_CHANGE_NFY));
			sTU_GUILD_FUNCTION_CHANGE_NFY * res2 = (sTU_GUILD_FUNCTION_CHANGE_NFY *)packet2.GetPacketData();
			res2->wOpCode = TU_GUILD_FUNCTION_CHANGE_NFY;
			res2->byNewFunc = req->byFunction;
			res2->qwGuildFunctionFlag = req->qwGuildFunctionFlag;
			packet2.SetPacketLen(sizeof(sTU_GUILD_FUNCTION_CHANGE_NFY));
			pGuild->Broadcast(&packet2);
		}
	}
}

void CQueryServerSession::RecvDojoBudokaiSeedAddRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_BUDOKAI_SEED_ADD_RES * req = (sQT_DOJO_BUDOKAI_SEED_ADD_RES *)pPacket->GetPacketData();

	if (req->wResultCode == CHAT_SUCCESS)
	{
		CDojo* dojo = g_pDojoManager->GetDojoByDojoTblidx(req->dojoTblidx);
		if (dojo)
		{
			NTL_SAFE_WCSCPY(dojo->GetDojoData()->wszSeedCharName, req->wszCharName);
		}
	}

	if (CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId))
	{
		CNtlPacket packet(sizeof(sTU_DOJO_BUDOKAI_SEED_ADD_RES));
		sTU_DOJO_BUDOKAI_SEED_ADD_RES* res = (sTU_DOJO_BUDOKAI_SEED_ADD_RES*)packet.GetPacketData();
		res->wOpCode = TU_DOJO_BUDOKAI_SEED_ADD_RES;
		res->wResultCode = req->wResultCode;
		res->dojoTblidx = req->dojoTblidx;
		NTL_SAFE_WCSCPY(res->wszCharName, req->wszCharName);
		packet.SetPacketLen(sizeof(sTU_DOJO_BUDOKAI_SEED_ADD_RES));
		app->Send(pPlayer->GetClientSessionID(), &packet);
	}
}

void CQueryServerSession::RecvDojoBudokaiSeedDelRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_BUDOKAI_SEED_DEL_RES * req = (sQT_DOJO_BUDOKAI_SEED_DEL_RES *)pPacket->GetPacketData();

	if (req->wResultCode == CHAT_SUCCESS)
	{
		CDojo* dojo = g_pDojoManager->GetDojoByDojoTblidx(req->dojoTblidx);
		if (dojo)
		{
			memset(dojo->GetDojoData()->wszSeedCharName, 0, sizeof(dojo->GetDojoData()->wszSeedCharName));
		}
	}

	if (CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId))
	{
		CNtlPacket packet(sizeof(sTU_DOJO_BUDOKAI_SEED_DEL_RES));
		sTU_DOJO_BUDOKAI_SEED_DEL_RES* res = (sTU_DOJO_BUDOKAI_SEED_DEL_RES*)packet.GetPacketData();
		res->wOpCode = TU_DOJO_BUDOKAI_SEED_DEL_RES;
		res->wResultCode = req->wResultCode;
		NTL_SAFE_WCSCPY(res->wszCharName, req->wszCharName);
		packet.SetPacketLen(sizeof(sTU_DOJO_BUDOKAI_SEED_DEL_RES));
		app->Send(pPlayer->GetClientSessionID(), &packet);
	}
}


void CQueryServerSession::RecvDojoScrambleRewardRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_SCRAMBLE_REWARD_RES * req = (sQT_DOJO_SCRAMBLE_REWARD_RES *)pPacket->GetPacketData();

	if (req->wResultCode == CHAT_SUCCESS)
	{
		if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
		{
			pGuild->UpdateReputation(req->dwAddGuildReputation, true);

			CNtlPacket packet2(sizeof(sTU_DOJO_SCRAMBLE_REWARD_NFY));
			sTU_DOJO_SCRAMBLE_REWARD_NFY * res2 = (sTU_DOJO_SCRAMBLE_REWARD_NFY *)packet2.GetPacketData();
			res2->wOpCode = TU_DOJO_SCRAMBLE_REWARD_NFY;
			res2->byCount = 0;
			res2->dwAddGuildReputation = req->dwAddGuildReputation;
			res2->itemTblidx = req->itemTblidx;
			packet2.SetPacketLen(sizeof(sTU_DOJO_SCRAMBLE_REWARD_NFY));
			pGuild->Broadcast(&packet2);
		}
	}
}

void CQueryServerSession::RecvDojoChangeNoticeRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_DOJO_CHANGE_NOTICE_RES * req = (sQT_DOJO_CHANGE_NOTICE_RES *)pPacket->GetPacketData();

	if (req->wResultCode == CHAT_SUCCESS)
	{
		if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
		{
			if (CDojo* pDojo = pGuild->GetDojo())
			{
				NTL_SAFE_WCSCPY(pDojo->GetDojoData()->wchLeaderName, req->awchCharName);
				NTL_SAFE_WCSCPY(pDojo->GetDojoData()->wchNotice, req->awchNotice);

				CNtlPacket packet0(sizeof(sTU_DOJO_NOTICE_CHANGE_NFY));
				sTU_DOJO_NOTICE_CHANGE_NFY* res0 = (sTU_DOJO_NOTICE_CHANGE_NFY*)packet0.GetPacketData();
				res0->wOpCode = TU_DOJO_NOTICE_CHANGE_NFY;
				res0->wNoticeLengthInUnicode = req->wNoticeLengthInUnicode;
				res0->dojoTblidx = req->dojoTblidx;
				NTL_SAFE_WCSCPY(res0->awchCharName, req->awchCharName);
				NTL_SAFE_WCSCPY(res0->awchNotice, req->awchNotice);
				packet0.SetPacketLen(sizeof(sTU_DOJO_NOTICE_CHANGE_NFY));
				pGuild->Broadcast(&packet0);
			}
		}
	}

	if (CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithCharID(req->charId))
	{
		CNtlPacket packet(sizeof(sTU_DOJO_NOTICE_CHANGE_RES));
		sTU_DOJO_NOTICE_CHANGE_RES* res = (sTU_DOJO_NOTICE_CHANGE_RES*)packet.GetPacketData();
		res->wOpCode = TU_DOJO_NOTICE_CHANGE_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTU_DOJO_NOTICE_CHANGE_RES));
		app->Send(pPlayer->GetClientSessionID(), &packet);
	}
}

void CQueryServerSession::RecvAuctionHouseSellRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_TENKAICHIDAISIJYOU_SELL_RES * req = (sQT_TENKAICHIDAISIJYOU_SELL_RES *)pPacket->GetPacketData();

	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pAH->AddItem(req->sData);
	}

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sTG_TENKAICHIDAISIJYOU_SELL_RES));
		sTG_TENKAICHIDAISIJYOU_SELL_RES * res = (sTG_TENKAICHIDAISIJYOU_SELL_RES *)packet.GetPacketData();
		res->wOpCode = TG_TENKAICHIDAISIJYOU_SELL_RES;
		res->charId = req->charId;
		res->byCount = req->sData.byCount;
		res->byPlace = req->byPlace;
		res->byPosition = req->byPosition;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_SELL_RES));
		app->Send(g_pServerInfoManager->GetGsSession(player->GetChannel()), &packet);
	}
}

void CQueryServerSession::RecvAuctionHouseSellCancelRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES * req = (sQT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES *)pPacket->GetPacketData();

	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pAH->DelItem(req->nItem);
	}

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES));
		sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES * res = (sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES *)packet.GetPacketData();
		res->wOpCode = TG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES;
		res->charId = req->charId;
		res->mailID = req->mailId;
		res->nItem = req->nItem;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES));
		g_pApp->Send(g_pServerInfoManager->GetGsSession(player->GetChannel()), &packet);
	}
}

void CQueryServerSession::RecvAuctionHouseBuyRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_TENKAICHIDAISIJYOU_BUY_RES * req = (sQT_TENKAICHIDAISIJYOU_BUY_RES *)pPacket->GetPacketData();

	if (req->wResultCode == GAME_SUCCESS)
	{
		g_pAH->DelItem(req->nItem);
	}

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sTG_TENKAICHIDAISIJYOU_BUY_RES));
		sTG_TENKAICHIDAISIJYOU_BUY_RES * res = (sTG_TENKAICHIDAISIJYOU_BUY_RES *)packet.GetPacketData();
		res->wOpCode = TG_TENKAICHIDAISIJYOU_BUY_RES;
		res->charId = req->charId;
		res->nItem = req->nItem;
		res->dwMoney = req->dwMoney;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sTG_TENKAICHIDAISIJYOU_BUY_RES));
		g_pApp->Send(g_pServerInfoManager->GetGsSession(player->GetChannel()), &packet);
	}
}

void CQueryServerSession::RecvAuctionHouseServerDataRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_TENKAICHIDAISIJYOU_SERVERDATA_RES * req = (sQT_TENKAICHIDAISIJYOU_SERVERDATA_RES *)pPacket->GetPacketData();

	for (BYTE i = 0; i < req->byCurPacketCount; i++)
	{
		g_pAH->AddItem(req->sData[i]);
	}

	if (req->bEndList)
	{
		g_pEventMgr->AddEvent(g_pAH, &CAutionhouse::Update, EVENT_SERVER_SESSION_UPDATE, CAutionhouse::UPDATE_AUCTIONHOUSE_TICK, -1, 0);
	}
}

void CQueryServerSession::RecvAuctionHousePeriodEndRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_TENKAICHIDAISIJYOU_PERIODEND_RES * req = (sQT_TENKAICHIDAISIJYOU_PERIODEND_RES *)pPacket->GetPacketData();

}

void CQueryServerSession::RecvHlsSlotMachineExtractRes(CNtlPacket * pPacket, CChatServer * app)
{
	sQT_HLS_SLOT_MACHINE_EXTRACT_RES * req = (sQT_HLS_SLOT_MACHINE_EXTRACT_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pPlayerManager->FindPlayerWithCharID(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sTU_HLS_SLOT_MACHINE_EXTRACT_RES));
		sTU_HLS_SLOT_MACHINE_EXTRACT_RES* res = (sTU_HLS_SLOT_MACHINE_EXTRACT_RES*)packet.GetPacketData();
		res->wOpCode = TU_HLS_SLOT_MACHINE_EXTRACT_RES;
		res->wResultCode = req->wResultCode;
		res->wMachineIndex = req->wMachineIndex;

		for (BYTE i = 0; i < req->byExtractCount; i++)
		{
			res->byRanking[i] = req->byRanking[i];

			res->ItemTblidx[i] = req->ItemTblidx[i];
			res->byStackCount[i] = req->byStackCount[i];
			res->bySetCount[i] = req->bySetCount[i];
		}

		if (req->wResultCode == CHAT_SUCCESS)
		{
			if (req->wCoin > (WORD)player->GetWaguMachineCoin())
			{
				res->wNewWaguWaguPoints = 0;
				ERR_LOG(LOG_HACK, "SLOT-MACHINE: Player %u, Account %u used more wagu coins than available !!!", req->charId, req->accountId);
			}
			else
			{
				res->wNewWaguWaguPoints = 0; //TODO
			}
		}

		res->byReallyExtractCount = req->byExtractCount;
		packet.SetPacketLen(sizeof(sTU_HLS_SLOT_MACHINE_EXTRACT_RES));
		player->SendPacket(&packet);

		if (req->wResultCode == CHAT_SUCCESS)
		{
			player->SetWaguMachineCoin((WORD)player->GetWaguMachineCoin() - req->wCoin);

			SYSTEMTIME ti;
			GetLocalTime(&ti);

			for (BYTE i = 0; i < req->byExtractCount; i++)
			{
				CNtlPacket packet2(sizeof(sTG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO));
				sTG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO* res2 = (sTG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO*)packet2.GetPacketData();
				res2->wOpCode = TG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO;
				res2->charId = req->charId;
				res2->bySetCount = 1;
				res2->sInfo.byStackCount = req->byStackCount[i];
				res2->sInfo.HLSitemTblidx = req->ItemTblidx[i];
				res2->sInfo.qwProductId = req->aProductId[i];
				res2->sInfo.tRegTime.day = (BYTE)ti.wDay;
				res2->sInfo.tRegTime.hour = (BYTE)ti.wHour;
				res2->sInfo.tRegTime.minute = (BYTE)ti.wMinute;
				res2->sInfo.tRegTime.month = (BYTE)ti.wMonth;
				res2->sInfo.tRegTime.second = (BYTE)ti.wSecond;
				res2->sInfo.tRegTime.year = ti.wYear;
				packet2.SetPacketLen(sizeof(sTG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO));
				app->Send(g_pServerInfoManager->GetGsSession(player->GetChannel()), &packet2);
			}

			CNtlPacket packet3(sizeof(sTG_WAGUCOIN_DECREASE_NFY));
			sTG_WAGUCOIN_DECREASE_NFY* res3 = (sTG_WAGUCOIN_DECREASE_NFY*)packet3.GetPacketData();
			res3->wOpCode = TG_WAGUCOIN_DECREASE_NFY;
			res3->charId = req->charId;
			res3->wWaguWaguCoin = (WORD)player->GetWaguMachineCoin();
			packet3.SetPacketLen(sizeof(sTG_WAGUCOIN_DECREASE_NFY));
			app->Send(g_pServerInfoManager->GetGsSession(player->GetChannel()), &packet3);

			CNtlPacket packet4(sizeof(sTU_WAGUWAGUCOIN_UPDATE_INFO));
			sTU_WAGUWAGUCOIN_UPDATE_INFO* res4 = (sTU_WAGUWAGUCOIN_UPDATE_INFO*)packet4.GetPacketData();
			res4->wOpCode = TU_WAGUWAGUCOIN_UPDATE_INFO;
			res4->wWaguWaguCoin = (WORD)player->GetWaguMachineCoin();
			packet4.SetPacketLen(sizeof(sTU_WAGUWAGUCOIN_UPDATE_INFO));
			player->SendPacket(&packet4);
		}
	}
}
