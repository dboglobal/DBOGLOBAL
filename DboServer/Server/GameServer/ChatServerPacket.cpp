#include "stdafx.h"
#include "GameServer.h"
#include "CPlayer.h"
#include "item.h"
#include "DojoManager.h"
#include "NtlPacketTG.h"
#include "NtlPacketGT.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlResultCode.h"
#include "Guild.h"
#include "GameMain.h"
#include "NtlRandom.h"
#include "DojoWar.h"
#include "BudokaiManager.h"
#include "SubNeighborServerInfoManager.h"
#include "SummonPet.h"
#include "freebattle.h"


void CChatServerSession::OnInvalid(CNtlPacket * pPacket)
{
}

//--------------------------------------------------------------------------------------//
//		RECEIVE CHARACTER ENTERED CHAT SERVER CONFIRMATION
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecUserEnterGameAck(CNtlPacket * pPacket)
{
	sTG_USER_ENTER_GAME_ACK * req = (sTG_USER_ENTER_GAME_ACK*)pPacket->GetPacketData();

	
}


void CChatServerSession::RecvUserAuthKeyGeneratedAck(CNtlPacket * pPacket)
{
	sTG_USER_AUTH_KEY_CREATED_ACK * req = (sTG_USER_AUTH_KEY_CREATED_ACK*)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pObjectManager->FindByAccount(req->accountId);
	if (pPlayer)
	{
		//send this packet, then client will send UT_ENTER_CHAT
		CNtlPacket packet(sizeof(sGU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES));
		sGU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES * res = (sGU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES *)packet.GetPacketData();
		res->wOpCode = GU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES;
		res->wResultCode = req->wResultCode;
		memcpy(res->abyAuthKey, req->abyAuthKey, NTL_MAX_SIZE_AUTH_KEY);
		packet.SetPacketLen(sizeof(sGU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES));
		pPlayer->SendPacket(&packet);
	}
}


//--------------------------------------------------------------------------------------//
//		RECEIVE GUILD CREATE RESULT
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildCreateRes(CNtlPacket * pPacket)
{
	sTG_GUILD_CREATE_RES * req = (sTG_GUILD_CREATE_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->creatorCharId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet0(sizeof(sGU_GUILD_CREATE_RES));
		sGU_GUILD_CREATE_RES * res0 = (sGU_GUILD_CREATE_RES *)packet0.GetPacketData();
		res0->wOpCode = GU_GUILD_CREATE_RES;
		res0->wResultCode = req->wResultCode;
		packet0.SetPacketLen(sizeof(sGU_GUILD_CREATE_RES));
		player->SendPacket(&packet0);

		if(req->wResultCode == GAME_SUCCESS)
			player->UpdateZeni(ZENNY_CHANGE_TYPE_GUILD_CREATE, req->dwSpentZenny, false, false);
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE GUILD CREATE NFY
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildCreateNfy(CNtlPacket * pPacket)
{
	sTG_GUILD_CREATED_NFY * req = (sTG_GUILD_CREATED_NFY*)pPacket->GetPacketData();

	g_pGuildManager->CreateGuild(req->guildId, req->wszGuildName, req->guildMark, req->sDogi, req->qwGuildFunctionFlag);
}

//--------------------------------------------------------------------------------------//
//		RECEIVE GUILD INVITATION RESPONSE
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildInviteRes(CNtlPacket * pPacket)
{
	sTG_GUILD_INVITE_RES * req = (sTG_GUILD_INVITE_RES*)pPacket->GetPacketData();

	CPlayer* invitor = g_pObjectManager->FindByChar(req->invitorCharId);
	if(invitor)
	{
		CNtlPacket packet(sizeof(sGU_GUILD_INVITE_RES));
		sGU_GUILD_INVITE_RES * res = (sGU_GUILD_INVITE_RES *)packet.GetPacketData();
		res->wOpCode = GU_GUILD_INVITE_RES;
		res->wResultCode = req->wResultCode;
		wcscpy_s(res->wszTargetName, NTL_MAX_SIZE_CHAR_NAME + 1, req->wszTargetName );
		packet.SetPacketLen( sizeof(sGU_GUILD_INVITE_RES) );
		g_pApp->Send( invitor->GetClientSessionID(), &packet );
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE GUILD DESTROYED
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildDestroyedRes(CNtlPacket * pPacket)
{
	sTG_GUILD_DESTROYED_NFY * req = (sTG_GUILD_DESTROYED_NFY*)pPacket->GetPacketData();
	
	g_pGuildManager->DestroyGuild(req->guildId);
}


void CChatServerSession::RecvGuildInfoRefreshedNfy(CNtlPacket * pPacket)
{
	sTG_GUILD_INFO_REFRESHED_NFY * req = (sTG_GUILD_INFO_REFRESHED_NFY*)pPacket->GetPacketData();

	CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId);
	if (pGuild)
	{
		if (req->sDogi.IsIntialized(false))
		{
		}

		if (req->sMark.IsIntialized())
		{
			pGuild->SetGuildMark(req->sMark);
			g_pGuildManager->UpdateGuildMark(req->guildId, req->sMark);

			g_pDojoManager->UpdateDojoMark(req->guildId, &req->sMark);
		}

		if (req->qwGuildFunctionFlag > 0 && req->qwGuildFunctionFlag != pGuild->GetGuildFunctionFlag())
		{
			pGuild->SetGuildFunctionFlag(req->qwGuildFunctionFlag);
		}

		if (wcslen(req->wszGuildName) > 0)
		{
			pGuild->SetGuildName(req->wszGuildName);
			g_pGuildManager->UpdateGuildName(req->guildId, req->wszGuildName);
		}
	}
}


//--------------------------------------------------------------------------------------//
//		RECEIVE GUILD ID CHANGED
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildIdChangedRes(CNtlPacket * pPacket)
{
	sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY * req = (sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY*)pPacket->GetPacketData();
	
	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		if (req->newGuildId == 0) //leave guild
		{
			if (CGuild* pGuild = g_pGuildManager->GetGuild(player->GetGuildID()))
			{
				player->LeaveGuild();
			}
		}
		else
		{
			if (CGuild* pGuild = g_pGuildManager->GetGuild(req->newGuildId))
			{
				g_pGuildManager->UpdateGuildMember(player->GetID(), req->newGuildId, true);

				player->SetGuildID(req->newGuildId);
				player->SetGuildName(pGuild->GetGuildName());
				player->SetGuildDogiMark(*pGuild->GetGuildMark(), *pGuild->GetGuildDogiData());

				//update guild name for others
				CNtlPacket packet(sizeof(sGU_GUILD_NAME_CHANGED_NFY));
				sGU_GUILD_NAME_CHANGED_NFY * res = (sGU_GUILD_NAME_CHANGED_NFY *)packet.GetPacketData();
				res->wOpCode = GU_GUILD_NAME_CHANGED_NFY;
				res->hSubject = player->GetID();
				wcscpy_s(res->wszGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, pGuild->GetGuildName());
				packet.SetPacketLen(sizeof(sGU_GUILD_NAME_CHANGED_NFY));
				player->Broadcast(&packet, player);

				//update guild dogi
				if (pGuild->GetGuildDogiData()->IsIntialized(false))
				{
					CNtlPacket packet2(sizeof(sGU_DOGI_UPDATE_NFY));
					sGU_DOGI_UPDATE_NFY * res2 = (sGU_DOGI_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_DOGI_UPDATE_NFY;
					res2->handle = player->GetID();
					memcpy(&res2->sData, pGuild->GetGuildDogiData(), sizeof(sDBO_DOGI_DATA));
					packet2.SetPacketLen(sizeof(sGU_DOGI_UPDATE_NFY));
					player->Broadcast(&packet2);
				}

				//dojo mark changed
				if (pGuild->GetGuildMark()->IsIntialized())
				{
					CNtlPacket packet3(sizeof(sGU_GUILD_MARK_CHANGED_NFY));
					sGU_GUILD_MARK_CHANGED_NFY * res3 = (sGU_GUILD_MARK_CHANGED_NFY *)packet3.GetPacketData();
					res3->wOpCode = GU_GUILD_MARK_CHANGED_NFY;
					res3->hSubject = player->GetID();
					memcpy(&res3->sMark, pGuild->GetGuildMark(), sizeof(sDBO_GUILD_MARK));
					packet3.SetPacketLen(sizeof(sGU_GUILD_MARK_CHANGED_NFY));
					player->Broadcast(&packet3);
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE UPDATE GUILD POINTS
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildGiveZenny(CNtlPacket * pPacket)
{
	sTG_GUILD_GIVE_ZENNY_RES * req = (sTG_GUILD_GIVE_ZENNY_RES*)pPacket->GetPacketData();
	
	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			if (player->GetZeni() >= req->dwZenny)
			{
				player->UpdateZeni(ZENNY_CHANGE_TYPE_GUILD_GIVE_ZENNY, req->dwZenny, false, false);
			}
			else
			{
				ERR_LOG(LOG_SYSTEM, "Hacker ! Player dont have enough zeni anymore.");
			}
		}

		CNtlPacket packet(sizeof(sGU_GUILD_GIVE_ZENNY_RES));
		sGU_GUILD_GIVE_ZENNY_RES * res = (sGU_GUILD_GIVE_ZENNY_RES *)packet.GetPacketData();
		res->wOpCode = GU_GUILD_GIVE_ZENNY_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen( sizeof(sGU_GUILD_GIVE_ZENNY_RES) );
		g_pApp->Send( player->GetClientSessionID(), &packet );
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE UPDATE GUILD FUNCTION
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildFunctionAddRes(CNtlPacket * pPacket)
{
	sTG_GUILD_FUNCTION_ADD_RES * req = (sTG_GUILD_FUNCTION_ADD_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->masterCharId);
	if (player && player->IsInitialized())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			if (player->GetZeni() >= req->dwZenny)
			{
				if (req->dwZenny > 0)
				{
					player->UpdateZeni(ZENNY_CHANGE_TYPE_GUILD_GIVE_ZENNY, req->dwZenny, false, false);
				}
			}
			else
			{
				ERR_LOG(LOG_SYSTEM, "Hacker ! Player dont have enough zeni anymore.");
			}
		}

		CNtlPacket packet(sizeof(sGU_GUILD_FUNCTION_ADD_RES));
		sGU_GUILD_FUNCTION_ADD_RES * res = (sGU_GUILD_FUNCTION_ADD_RES *)packet.GetPacketData();
		res->wOpCode = GU_GUILD_FUNCTION_ADD_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_GUILD_FUNCTION_ADD_RES));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}
}

void CChatServerSession::RecvGuildBankStartRes(CNtlPacket * pPacket)
{
	sTG_GUILD_BANK_START_RES * req = (sTG_GUILD_BANK_START_RES*)pPacket->GetPacketData();

	CGameServer* app = (CGameServer*)g_pApp;

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			player->SetUsingBank(true);

			//Load guild items and zeni
			CNtlPacket packetQry(sizeof(sGQ_GUILD_BANK_LOAD_REQ));
			sGQ_GUILD_BANK_LOAD_REQ * resQry = (sGQ_GUILD_BANK_LOAD_REQ *)packetQry.GetPacketData();
			resQry->wOpCode = GQ_GUILD_BANK_LOAD_REQ;
			resQry->handle = player->GetID();
			resQry->charID = player->GetCharID();
			resQry->guildID = player->GetGuildID();
			resQry->hNpcHandle = req->hNpcHandle;
			packetQry.SetPacketLen(sizeof(sGQ_GUILD_BANK_LOAD_REQ));
			app->SendTo(app->GetQueryServerSession(), &packetQry);
		}
		else
		{
			CNtlPacket packet(sizeof(sGU_GUILD_BANK_START_RES));
			sGU_GUILD_BANK_START_RES * res = (sGU_GUILD_BANK_START_RES *)packet.GetPacketData();
			res->wOpCode = GU_GUILD_BANK_START_RES;
			res->wResultCode = req->wResultCode;
			res->handle = req->hNpcHandle;
			packet.SetPacketLen(sizeof(sGU_GUILD_BANK_START_RES));
			app->Send(player->GetClientSessionID(), &packet);
		}
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE CREATE GUILD MARK (EMBLEM)
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildCreateMarkRes(CNtlPacket * pPacket)
{
	sTG_GUILD_CREATE_MARK_RES * req = (sTG_GUILD_CREATE_MARK_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_GUILD_CREATE_MARK_RES));
		sGU_GUILD_CREATE_MARK_RES * res = (sGU_GUILD_CREATE_MARK_RES *)packet.GetPacketData();
		res->wOpCode = GU_GUILD_CREATE_MARK_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_GUILD_CREATE_MARK_RES));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}
}

//--------------------------------------------------------------------------------------//
//		RECEIVE UPDATE GUILD MARK (EMBLEM)
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecGuildChangeMarkRes(CNtlPacket * pPacket)
{
	sTG_GUILD_CHANGE_MARK_RES * req = (sTG_GUILD_CHANGE_MARK_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			if (player->GetZeni() >= req->dwZenny)
			{
				if (req->dwZenny > 0)
				{
					player->UpdateZeni(ZENNY_CHANGE_TYPE_GUILD_MARK_CHANGE, req->dwZenny, false, false);
				}
			}
			else
			{
				ERR_LOG(LOG_SYSTEM, "Hacker ! Player dont have enough zeni anymore.");
			}
		}

		CNtlPacket packet(sizeof(sGU_GUILD_CHANGE_MARK_RES));
		sGU_GUILD_CHANGE_MARK_RES * res = (sGU_GUILD_CHANGE_MARK_RES *)packet.GetPacketData();
		res->wOpCode = GU_GUILD_CHANGE_MARK_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_GUILD_CHANGE_MARK_RES));
		player->SendPacket(&packet);
	}
}


void CChatServerSession::RecGuildChangeNameRes(CNtlPacket * pPacket)
{
	sTG_GUILD_CHANGE_NAME_RES * req = (sTG_GUILD_CHANGE_NAME_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_GUILD_CHANGE_NAME_RES));
		sGU_GUILD_CHANGE_NAME_RES * res = (sGU_GUILD_CHANGE_NAME_RES *)packet.GetPacketData();
		res->wOpCode = GU_GUILD_CHANGE_NAME_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_GUILD_CHANGE_NAME_RES));
		player->SendPacket(&packet);

		if (req->wResultCode == GAME_SUCCESS)
		{
			CItem* pItem = player->GetPlayerItemContainer()->GetItem(req->itemId);
			if (pItem)
			{
				pItem->SetCount(pItem->GetCount() - 1, false, false);
			}
			else
			{
				ERR_LOG(LOG_HACK, "HACKER: ChaneGuildName Item does not exist anymore. ItemID %I64u, Place %u, Pos %u, Char %u, GuildID %u", req->itemId, req->byPlace, req->byPos, req->charId, req->guildId);
			}
		}
	}
}


void CChatServerSession::RecvGuildBankEndNfy(CNtlPacket * pPacket)
{
	sTG_GUILD_BANK_END_NFY * req = (sTG_GUILD_BANK_END_NFY*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			player->GetPlayerItemContainer()->FreeGuildBank();
			player->SetUsingBank(false);
			player->GetPlayerItemContainer()->SetUsingGuildBank(false);
		}

		CNtlPacket packet(sizeof(sGU_GUILD_BANK_END_RES));
		sGU_GUILD_BANK_END_RES * res = (sGU_GUILD_BANK_END_RES *)packet.GetPacketData();
		res->wOpCode = GU_GUILD_BANK_END_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_GUILD_BANK_END_RES));
		player->SendPacket(&packet);
	}
}

void CChatServerSession::RecvBudokaiChannelStateInfoNfy(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_CHANNEL_STATE_INFO_NFY * req = (sTG_BUDOKAI_CHANNEL_STATE_INFO_NFY*)pPacket->GetPacketData();

	//info: Packet is not received on budokai server

	g_pBudokaiManager->UpdateChannelStateInfo(req->tmDefaultOpenTime, req->bRankbattleAllow, req->sStateInfo, req->aMatchStateInfo);
}

void CChatServerSession::RecvBudokaiChannelUpdateState(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_CHANNEL_UPDATE_STATE * req = (sTG_BUDOKAI_CHANNEL_UPDATE_STATE*)pPacket->GetPacketData();

	g_pBudokaiManager->UpdateChannelState(req->tmDefaultOpenTime, req->bRankbattleAllow, req->sStateInfo);
}

void CChatServerSession::RecvBudokaiChannelUpdateMatchState(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE * req = (sTG_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE*)pPacket->GetPacketData();

	g_pBudokaiManager->UpdateMatchState(req->byMatchType, req->sStateInfo);
}

void CChatServerSession::RecvBudokaiTournamentIndividualListReq(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ * req = (sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->SendTournamentIndividualListToChat(req->charId, req->byReceivedListCount, GetHandle());
}

void CChatServerSession::RecvBudokaiTournamentIndividualInfoReq(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ * req = (sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->SendTournamentIndividualInfoToChat(req->charId, req->awJoinId, GetHandle());
}

void CChatServerSession::RecvBudokaiTournamentTeamListReq(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ * req = (sTG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->SendTournamentTeamListToChat(req->charId, req->byReceivedListCount, GetHandle());
}

void CChatServerSession::RecvBudokaiTournamentTeamInfoReq(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ * req = (sTG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->SendTournamentTeamInfoToChat(req->charId, req->awJoinId, GetHandle());
}

void CChatServerSession::RecvBudokaiMinorMatchIndividualListNfy(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY * req = (sTG_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY*)pPacket->GetPacketData();

	for (BYTE i = 0; i < BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER; i++)
	{
		if (req->aTeleportData[i].byPlayerCount == 0)
			break;

		for (BYTE ii = 0; ii < req->aTeleportData[i].byPlayerCount; ii++)
		{
			if (req->aTeleportData[i].aPlayers[ii] == 0)
				break;

			CPlayer* player = g_pObjectManager->FindByChar(req->aTeleportData[i].aPlayers[ii]);
			if (player && player->IsInitialized())
			{
				if (player->GetJoinID() != INVALID_JOINID)
				{
					if (g_pBudokaiManager->GetType() == BUDOKAI_TYPE_JUNIOR && player->GetLevel() > 29)
						continue;

					CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ));
					sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ * res = (sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ *)packet.GetPacketData();
					res->wOpCode = GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ;
					res->charId = player->GetCharID();
					res->handle = player->GetID();
					res->joinId = player->GetJoinID();
					res->byMatchType = g_pBudokaiManager->GetMatchType();
					packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ));
					g_pApp->Send(GetHandle(), &packet);


				}
			}
		}
	}
}

void CChatServerSession::RecvBudokaiMinorMatchTeamListNfy(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY * req = (sTG_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY*)pPacket->GetPacketData();

	for (BYTE i = 0; i < BUDOKAI_MAJOR_MATCH_TEAM_NUMBER; i++)
	{
		if (req->aTeleportData[i].byTeamCount == 0)
			break;

		for (BYTE ii = 0; ii < req->aTeleportData[i].byTeamCount; ii++)
		{
			for (BYTE iii = 0; iii < req->aTeleportData[i].aTeamList[ii].byCount; iii++)
			{
				CPlayer* player = g_pObjectManager->FindByChar(req->aTeleportData[i].aTeamList[ii].charId[iii]);
				if (player && player->IsInitialized())
				{
					if (player->GetJoinID() != INVALID_JOINID)
					{
						if (g_pBudokaiManager->GetType() == BUDOKAI_TYPE_JUNIOR && player->GetLevel() > 29)
							continue;

						CNtlPacket packet(sizeof(sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ));
						sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ * res = (sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ *)packet.GetPacketData();
						res->wOpCode = GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ;
						res->charId = player->GetCharID();
						res->handle = player->GetID();
						res->joinId = player->GetJoinID();
						res->byMatchType = g_pBudokaiManager->GetMatchType();
						packet.SetPacketLen(sizeof(sGT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ));
						g_pApp->Send(GetHandle(), &packet);
					}
				}
			}
		}
	}
}

void CChatServerSession::RecvBudokaiMinorMatchTeleportInfoReq(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ * req = (sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->TeleportPrelimReq(req->charId, req->handle, req->byServerChannelId, req->byServerIndex, req->byMatchType, req->joinId);
}

void CChatServerSession::RecvBudokaiMinorMatchTeleportInfoRes(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES * req = (sTG_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES*)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pObjectManager->GetPC(req->handle);
	if (pPlayer && pPlayer->IsInitialized())
	{
		if (pPlayer->GetCharID() == req->charId)
		{
			//set join result | NOTE: Maybe put this somewhere else?
			if(req->byTeleportType == TELEPORT_TYPE_MINORMATCH)
				g_pBudokaiManager->SetJoinResult(req->joinId, BUDOKAI_JOIN_RESULT_MINORMATCH);

			if(req->wResultCode == GAME_SUCCESS)
				pPlayer->TeleportAnotherServer(CNtlVector(req->vLoc), CNtlVector(req->vDir), req->worldTblidx, req->worldId, req->byTeleportType, req->byDestServerChannelId, 120); // 120 = 2 minutes
			else
			{
				CNtlPacket packet(sizeof(sGU_BUDOKAI_MUDOSA_TELEPORT_RES));
				sGU_BUDOKAI_MUDOSA_TELEPORT_RES * res = (sGU_BUDOKAI_MUDOSA_TELEPORT_RES *)packet.GetPacketData();
				res->wOpCode = GU_BUDOKAI_MUDOSA_TELEPORT_RES;
				res->wResultCode = req->wResultCode;
				packet.SetPacketLen(sizeof(sGU_BUDOKAI_MUDOSA_TELEPORT_RES));
				pPlayer->SendPacket(&packet);
			}
		}
	}
}

void CChatServerSession::RecvBudokaiProgressMessageNfy(CNtlPacket * pPacket)
{
	sTG_BUDOKAI_PROGRESS_MESSAGE_NFY * req = (sTG_BUDOKAI_PROGRESS_MESSAGE_NFY*)pPacket->GetPacketData();


}



//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecAuctionHouseSellRes(CNtlPacket * pPacket)
{
	sTG_TENKAICHIDAISIJYOU_SELL_RES * req = (sTG_TENKAICHIDAISIJYOU_SELL_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_TENKAICHIDAISIJYOU_SELL_RES));
		sGU_TENKAICHIDAISIJYOU_SELL_RES * res = (sGU_TENKAICHIDAISIJYOU_SELL_RES *)packet.GetPacketData();
		res->wOpCode = GU_TENKAICHIDAISIJYOU_SELL_RES;
		res->wResultCode = req->wResultCode;
		res->byCount = req->byCount;
		res->byPlace = req->byPlace;
		res->byPosition = req->byPosition;
		packet.SetPacketLen(sizeof(sGU_TENKAICHIDAISIJYOU_SELL_RES));
		player->SendPacket(&packet);
	}
}
//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecAuctionHouseSellCancelRes(CNtlPacket * pPacket)
{
	sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES * req = (sTG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES));
		sGU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES * res = (sGU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES *)packet.GetPacketData();
		res->wOpCode = GU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES;
		res->nItem = req->nItem;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES));
		player->SendPacket(&packet);

		player->ReloadMailsStatistic(false);
	}
}
//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecAuctionHousePriceInfoRes(CNtlPacket * pPacket)
{
	sTG_TENKAICHIDAISIJYOU_PRICE_INFO_RES * req = (sTG_TENKAICHIDAISIJYOU_PRICE_INFO_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		if (player->GetZeni() >= req->dwPrice)
		{
			CNtlPacket packet(sizeof(sGT_TENKAICHIDAISIJYOU_BUY_REQ));
			sGT_TENKAICHIDAISIJYOU_BUY_REQ * res = (sGT_TENKAICHIDAISIJYOU_BUY_REQ *)packet.GetPacketData();
			res->wOpCode = GT_TENKAICHIDAISIJYOU_BUY_REQ;
			res->charId = player->GetCharID();
			res->nItem = req->nItem;
			res->dwMoney = req->dwPrice;
			wcscpy_s(res->awchSystem, NTL_MAX_SIZE_CHAR_NAME + 1, L"[DBOG]System");
			wcscpy_s(res->awchBuyText, NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1, L"You have bought an Item");
			wcscpy_s(res->awchSellText, NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1, L"You have sold an Item");
			packet.SetPacketLen(sizeof(sGT_TENKAICHIDAISIJYOU_BUY_REQ));
			g_pApp->Send(GetHandle(), &packet);
		}
		else
		{
			CNtlPacket packet(sizeof(sGU_TENKAICHIDAISIJYOU_BUY_RES));
			sGU_TENKAICHIDAISIJYOU_BUY_RES * res = (sGU_TENKAICHIDAISIJYOU_BUY_RES *)packet.GetPacketData();
			res->wOpCode = GU_TENKAICHIDAISIJYOU_BUY_RES;
			res->nItem = req->nItem;
			res->dwMoney = req->dwPrice;
			res->wResultCode = TENKAICHIDAISIJYOU_CANNOT_BUY_NO_MONEY;
			packet.SetPacketLen(sizeof(sGU_TENKAICHIDAISIJYOU_BUY_RES));
			player->SendPacket(&packet);
		}
	}
}
//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecAuctionHouseBuyRes(CNtlPacket * pPacket)
{
	sTG_TENKAICHIDAISIJYOU_BUY_RES * req = (sTG_TENKAICHIDAISIJYOU_BUY_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			player->SetZeni(req->dwMoney);

			player->ReloadMailsStatistic(false);
		}

		CNtlPacket packet(sizeof(sGU_TENKAICHIDAISIJYOU_BUY_RES));
		sGU_TENKAICHIDAISIJYOU_BUY_RES * res = (sGU_TENKAICHIDAISIJYOU_BUY_RES *)packet.GetPacketData();
		res->wOpCode = GU_TENKAICHIDAISIJYOU_BUY_RES;
		res->nItem = req->nItem;
		res->dwMoney = req->dwMoney;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_TENKAICHIDAISIJYOU_BUY_RES));
		player->SendPacket(&packet);
	}
}
//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecAuctionHouseListData(CNtlPacket * pPacket)
{
	sTG_TENKAICHIDAISIJYOU_LIST_DATA * req = (sTG_TENKAICHIDAISIJYOU_LIST_DATA*)pPacket->GetPacketData();
	
	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_TENKAICHIDAISIJYOU_LIST_DATA));
		sGU_TENKAICHIDAISIJYOU_LIST_DATA * res = (sGU_TENKAICHIDAISIJYOU_LIST_DATA *)packet.GetPacketData();
		res->wOpCode = GU_TENKAICHIDAISIJYOU_LIST_DATA;
		memcpy(&res->sData, &req->sData, sizeof(sTENKAICHIDAISIJYOU_DATA_CLIENT));
		packet.SetPacketLen(sizeof(sGU_TENKAICHIDAISIJYOU_LIST_DATA));
		player->SendPacket(&packet);
	}
}
//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecAuctionHouseListDataRes(CNtlPacket * pPacket)
{
	sTG_TENKAICHIDAISIJYOU_LIST_RES * req = (sTG_TENKAICHIDAISIJYOU_LIST_RES*)pPacket->GetPacketData();
	
	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_TENKAICHIDAISIJYOU_LIST_RES));
		sGU_TENKAICHIDAISIJYOU_LIST_RES * res = (sGU_TENKAICHIDAISIJYOU_LIST_RES *)packet.GetPacketData();
		res->wOpCode = GU_TENKAICHIDAISIJYOU_LIST_RES;
		res->byPage = req->byPage;
		res->byListNum = req->byListNum;
		res->uiMaxPage = req->uiMaxPage;
		res->uiPage = req->uiPage;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_TENKAICHIDAISIJYOU_LIST_RES));
		player->SendPacket(&packet);
	}
}
//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecAuctionHousePeriodEndRes(CNtlPacket * pPacket)
{
	sTG_TENKAICHIDAISIJYOU_PERIODEND_RES * req = (sTG_TENKAICHIDAISIJYOU_PERIODEND_RES*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{

	}
}

void CChatServerSession::RecvWaguCoinDecreaseNfy(CNtlPacket * pPacket)
{
	sTG_WAGUCOIN_DECREASE_NFY * req = (sTG_WAGUCOIN_DECREASE_NFY*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		player->SetWaguMachineCoin((DWORD)req->wWaguWaguCoin);
	}
}

void CChatServerSession::RecvWaguWaguMachineUpdateCashitemInfo(CNtlPacket * pPacket)
{
	sTG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO * req = (sTG_WAGUWAGUMACHINE_UPDATE_CASHITEM_INFO*)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_CASHITEM_ADD_NFY));
		sGU_CASHITEM_ADD_NFY * res = (sGU_CASHITEM_ADD_NFY *)packet.GetPacketData();
		res->wOpCode = GU_CASHITEM_ADD_NFY;
		memcpy(&res->sInfo, &req->sInfo, sizeof(sCASHITEM_BRIEF));
		packet.SetPacketLen(sizeof(sGU_CASHITEM_ADD_NFY));
		player->SendPacket(&packet);

		player->AddCashShopItem(res->sInfo.qwProductId, &res->sInfo);
	}
}



//--------------------------------------------------------------------------------------//
//		SEND USER LEVEL TO CHAT SERVER
//--------------------------------------------------------------------------------------//
void	CChatServerSession::SendUpdatePcLevel(CPlayer* pPlayer)
{
	CNtlPacket packet(sizeof(sGT_UPDATE_PLAYER_LEVEL));
	sGT_UPDATE_PLAYER_LEVEL * res = (sGT_UPDATE_PLAYER_LEVEL *)packet.GetPacketData();
	res->wOpCode = GT_UPDATE_PLAYER_LEVEL;
	res->accountId = pPlayer->GetAccountID();
	res->byNewLevel = pPlayer->GetLevel();
	packet.SetPacketLen( sizeof(sGT_UPDATE_PLAYER_LEVEL) );
	g_pApp->Send( GetHandle(), &packet );
}


//--------------------------------------------------------------------------------------//
//		DOJO CREATE REs
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecvDojoCreateRes(CNtlPacket * pPacket)
{
	sTG_DOJO_CREATE_RES * req = (sTG_DOJO_CREATE_RES *)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_DOJO_CREATE_RES));
		sGU_DOJO_CREATE_RES * res = (sGU_DOJO_CREATE_RES *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_CREATE_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_DOJO_CREATE_RES));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}
}

void CChatServerSession::RecvCharServerTeleport(CNtlPacket * pPacket)
{
	sTG_CHAR_SERVER_TELEPORT * req = (sTG_CHAR_SERVER_TELEPORT *)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player && player->IsInitialized())
	{
		CGameServer* app = (CGameServer*)g_pApp;

		CNtlPacket packet(sizeof(sGU_CHAR_SERVER_CHANGE_RES));
		sGU_CHAR_SERVER_CHANGE_RES * res = (sGU_CHAR_SERVER_CHANGE_RES *)packet.GetPacketData();
		res->wOpCode = GU_CHAR_SERVER_CHANGE_RES;
		res->wResultCode = GAME_FAIL;

		if (req->sTeleportInfo.serverChannelId == app->GetGsChannel()) //if teleport fails
		{
			player->SendCharStateSpawning(TELEPORT_TYPE_DEFAULT);
			ERR_LOG(LOG_USER, "Player %u failed to teleport to channel %u because its the same as current channel", req->sTeleportInfo.serverChannelId);
		}
		else
		{
			sSERVER_CHANNEL_INFO* pChannel = g_pServerInfoManager->GetServerChannelInfo(req->sTeleportInfo.serverIndex, req->sTeleportInfo.serverChannelId);
			if (pChannel)
			{
				if (pChannel->listGameServerInfoRef.size() > 0)
				{
					pChannel->dwLoad += 1;

					if (player->GetAspectStateId() == ASPECTSTATE_VEHICLE)
						player->EndVehicle(GAME_VEHICLE_END_BY_FORCED);

					if (player->GetPcIsFreeBattle())
						g_pFreeBattleManager->EndFreeBattle(player->GetFreeBattleID(), player->GetFreeBattleTarget());

					if (player->GetCurrentPetId() != INVALID_HOBJECT)
					{
						if (CSummonPet* pPet = g_pObjectManager->GetSummonPet(player->GetCurrentPetId()))
							pPet->Despawn(true);
					}

					player->SendCharStateLeaving(CHARLEAVING_SERVER_CHANGE, false); // MUST BE CALLED BEFORE "SaveToDB" or teleport info not stored.

					//leave world or it might happen that "CNtlSobManager::AddObject => ReCreate SobObject Class Id = 3, Handle = 1997250 Memeroy Leak" appear when login to new channel
					player->SaveToDB(false);
					app->GetGameMain()->GetWorldManager()->LeaveObject(player);

					res->wResultCode = GAME_SUCCESS;

					player->CopyDestServerInfo(&res->serverInfo, res->achAuthKey);
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "pChannel->listGameServerInfoRef.size() == 0. ServerIndex: %u, serverChannelId: %u", req->sTeleportInfo.serverIndex, req->sTeleportInfo.serverChannelId);
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "Could not find channel info. ServerIndex: %u, serverChannelId: %u", req->sTeleportInfo.serverIndex, req->sTeleportInfo.serverChannelId);
			}
		}

		packet.SetPacketLen(sizeof(sGU_CHAR_SERVER_CHANGE_RES));
		player->SendPacket(&packet);
	}
}

//--------------------------------------------------------------------------------------//
//		DOJO CREATED NFY
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecvDojoCreatedNfy(CNtlPacket * pPacket)
{
	sTG_DOJO_CREATED_NFY * req = (sTG_DOJO_CREATED_NFY *)pPacket->GetPacketData();

	if (CGuild* pGuild = g_pGuildManager->GetGuild(req->sDojoData.guildId))
		g_pDojoManager->AddDojo(req->sDojoData, pGuild->GetGuildMark());
}
//--------------------------------------------------------------------------------------//
//		DOJO DELETE RES
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecvDojoDeleteRes(CNtlPacket * pPacket)
{
	sTG_DOJO_DELETE_RES * req = (sTG_DOJO_DELETE_RES *)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sGU_DOJO_DELETE_RES));
		sGU_DOJO_DELETE_RES * res = (sGU_DOJO_DELETE_RES *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_DELETE_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_DOJO_DELETE_RES));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}
}
//--------------------------------------------------------------------------------------//
//		DOJO FUNCTION ADD REs
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecvDojoFunctionAddRes(CNtlPacket * pPacket)
{
	sTG_DOJO_FUNCTION_ADD_RES * req = (sTG_DOJO_FUNCTION_ADD_RES *)pPacket->GetPacketData();

	BYTE byNewDojoLevel = Dbo_GetDojoLevel(req->byFunction);
	TBLIDX dojoIdx = INVALID_TBLIDX;

	if (req->wResultCode == GAME_SUCCESS)
	{
		if (byNewDojoLevel > 0)
		{
			if(sDBO_DOJO_BRIEF* dojobrief = g_pDojoManager->GetDojoBrief(req->guildId))
			{
				dojobrief->byLevel = byNewDojoLevel;
				dojoIdx = dojobrief->dojoTblidx;
			}
		}

		if (CGuild* pGuild = g_pGuildManager->GetGuild(req->guildId))
		{
			pGuild->SetGuildFunctionFlag(req->qwGuildFunctionFlag);
		}
	}

	CPlayer* player = g_pObjectManager->FindByChar(req->masterCharId);
	if (player && player->IsInitialized())
	{
		if (req->wResultCode == GAME_SUCCESS)
		{
			if (player->GetZeni() >= req->dwZenny)
			{
				if (req->dwZenny > 0)
				{
					player->UpdateZeni(ZENNY_CHANGE_TYPE_GUILD_GIVE_ZENNY, req->dwZenny, false, false);
				}
			}
			else
			{
				ERR_LOG(LOG_SYSTEM, "Hacker ! Player dont have enough zeni anymore.");
			}

			if (byNewDojoLevel > 0)
			{
				CNtlPacket packet0(sizeof(sGU_DOJO_LEVEL_CHANGED_NFY));
				sGU_DOJO_LEVEL_CHANGED_NFY * res0 = (sGU_DOJO_LEVEL_CHANGED_NFY *)packet0.GetPacketData();
				res0->wOpCode = GU_DOJO_LEVEL_CHANGED_NFY;
				res0->byLevel = byNewDojoLevel;
				res0->dojoTblidx = dojoIdx;
				g_pApp->Send(player->GetClientSessionID(), &packet0);
			}
		}

		CNtlPacket packet(sizeof(sGU_DOJO_FUNCTION_ADD_RES));
		sGU_DOJO_FUNCTION_ADD_RES * res = (sGU_DOJO_FUNCTION_ADD_RES *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_FUNCTION_ADD_RES;
		res->wResultCode = req->wResultCode;
		res->byFunction = req->byFunction;
		packet.SetPacketLen(sizeof(sGU_DOJO_FUNCTION_ADD_RES));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}
}

void CChatServerSession::RecvDojoUpdateRes(CNtlPacket * pPacket)
{
	sTG_DOJO_UPDATE_RES * req = (sTG_DOJO_UPDATE_RES *)pPacket->GetPacketData();
	//printf("RecvDojoUpdateRes : req->guildId %u, req->newGuildId %u, req->wResultCode %u\n", req->guildId, req->newGuildId, req->wResultCode);
	CDojo* pDojo = g_pDojoManager->GetDojo(req->guildId);
	if (pDojo)
	{
		CGuild* pOldGuild = g_pGuildManager->GetGuild(req->guildId);
		if (pOldGuild)
		{
			CNtlPacket packet(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
			sGU_DOJO_MARK_CHANGED_NFY * res = (sGU_DOJO_MARK_CHANGED_NFY *)packet.GetPacketData();
			res->wOpCode = GU_DOJO_MARK_CHANGED_NFY;
			res->dojoTblidx = pDojo->GetDojoTblidx();
			res->guildId = req->guildId;
			memset(&res->sMark, -1, sizeof(sDBO_GUILD_MARK));
			packet.SetPacketLen(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
			g_pObjectManager->SendPacketToAll(&packet);
		}

		CGuild* pGuild = g_pGuildManager->GetGuild(req->newGuildId);
		if (pGuild)
		{
			pDojo->SetDojoData(req->newGuildId);
			pDojo->SetDojoMark(pGuild->GetGuildMark());

			CNtlPacket packet(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
			sGU_DOJO_MARK_CHANGED_NFY * res = (sGU_DOJO_MARK_CHANGED_NFY *)packet.GetPacketData();
			res->wOpCode = GU_DOJO_MARK_CHANGED_NFY;
			res->dojoTblidx = pDojo->GetDojoTblidx();
			res->guildId = req->newGuildId;
			memcpy(&res->sMark, pGuild->GetGuildMark(), sizeof(sDBO_GUILD_MARK));
			packet.SetPacketLen(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
			g_pObjectManager->SendPacketToAll(&packet);
		}

		g_pDojoManager->RemoveDojo(req->guildId);
		g_pDojoManager->AddDojo(req->newGuildId, pDojo);
	}
}

void CChatServerSession::RecvDojoSendAttGuildId(CNtlPacket * pPacket)
{
	sTG_DOJO_SEND_ATT_GUILD_ID * req = (sTG_DOJO_SEND_ATT_GUILD_ID *)pPacket->GetPacketData();

	CDojo* pDojo = g_pDojoManager->GetDojo(req->defGuildId);
	if (pDojo)
	{
		g_pDojoWarManager->AddDojo(pDojo);

		if (pDojo->GetDojoTblidx() == req->dojoTblidx)
			pDojo->SetAttGuild(req->attGuildId);
		else
			ERR_LOG(LOG_GENERAL, "ERROR: Dojo tblidx does not match. ");
	}
}

//--------------------------------------------------------------------------------------//
//		DOJO NPC INFO RESULT
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecvDojoNpcInfoRes(CNtlPacket * pPacket)
{
	sTG_DOJO_NPC_INFO_RES * req = (sTG_DOJO_NPC_INFO_RES *)pPacket->GetPacketData();

	CPlayer* player = g_pObjectManager->FindByChar(req->charId);
	if (player)
	{
		CNtlPacket packet(sizeof(sGU_DOJO_NPC_INFO_RES));
		sGU_DOJO_NPC_INFO_RES * res = (sGU_DOJO_NPC_INFO_RES *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_NPC_INFO_RES;
		res->wResultCode = req->wResultCode;
		memcpy(&res->sData, &req->sData, sizeof(sDBO_DOJO_NPC_INFO));
		packet.SetPacketLen(sizeof(sGU_DOJO_NPC_INFO_RES));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}
}

void CChatServerSession::RecvDojoSendTeleportProposalNfy(CNtlPacket * pPacket)
{
	sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY * req = (sTG_DOJO_SEND_TELEPORT_PROPOSAL_NFY *)pPacket->GetPacketData();

	for (BYTE i = 0; i < req->byCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->FindByChar(req->aMemberId[i]);
		if (pPlayer && pPlayer->IsInitialized())
		{
			req->vDestLoc.x = req->vDestLoc.x + RandomRangeF(-10.0f, 10.0f);
			req->vDestLoc.z = req->vDestLoc.z + RandomRangeF(-10.0f, 10.0f);

			pPlayer->TeleportAnotherServer(req->vDestLoc, pPlayer->GetCurDir(), req->destWorldTblidx, req->destWorldId, req->byTeleportType, DOJO_CHANNEL_INDEX, req->wWaitTime);
		}
	}
}

void CChatServerSession::RecvDojoUserCountChange(CNtlPacket * pPacket)
{
	sTG_DOJO_USERCOUNT_CHANGE * req = (sTG_DOJO_USERCOUNT_CHANGE *)pPacket->GetPacketData();

	CDojo* pDojo = g_pDojoManager->GetDojoWithTblidx(req->dojoTblidx);
	if (pDojo)
		pDojo->SetUserCount(req->byDefCount, req->byAttCount);
}

//--------------------------------------------------------------------------------------//
//		DOJO NPC INFO RESULT
//--------------------------------------------------------------------------------------//
void	CChatServerSession::RecvDojoDestroyedNfy(CNtlPacket * pPacket)
{
	sTG_DOJO_DESTROYED_NFY * req = (sTG_DOJO_DESTROYED_NFY *)pPacket->GetPacketData();

	g_pDojoManager->DelDojo(req->guildId);
}


void CChatServerSession::RecvDojoScrambleRes(CNtlPacket * pPacket)
{
	sTG_DOJO_SCRAMBLE_RES * req = (sTG_DOJO_SCRAMBLE_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pObjectManager->FindByChar(req->charId);
	if (pPlayer && pPlayer->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_DOJO_SCRAMBLE_RES));
		sGU_DOJO_SCRAMBLE_RES * res = (sGU_DOJO_SCRAMBLE_RES *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_SCRAMBLE_RES;
		res->wResultCode = req->wResultCode;
		packet.SetPacketLen(sizeof(sGU_DOJO_SCRAMBLE_RES));
		pPlayer->SendPacket(&packet);

		if (req->wResultCode == GAME_SUCCESS)
		{
			if (pPlayer->GetZeni() >= DBO_DOJO_SCRAMBLE_REQUEST_FARE)
				pPlayer->UpdateZeni(ZENNY_CHANGE_TYPE_DOJO_SCRAMBLE_REQ, DBO_DOJO_SCRAMBLE_REQUEST_FARE, false, true);
			else
				ERR_LOG(LOG_USER, "ERROR: User %u does not have the required zeni anymore. Hacker..", pPlayer->GetCharID());
		}
	}
	else if (req->wResultCode == GAME_SUCCESS)
		ERR_LOG(LOG_USER, "ERROR: User %u is not online anymore. Hacker..", pPlayer->GetCharID());
}


void CChatServerSession::RecvDojoScrambleResponse(CNtlPacket * pPacket)
{
	sTG_DOJO_SCRAMBLE_RESPONSE * req = (sTG_DOJO_SCRAMBLE_RESPONSE *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pObjectManager->FindByChar(req->charId);
	if (pPlayer && pPlayer->IsInitialized())
	{
		CNtlPacket packet(sizeof(sGU_DOJO_SCRAMBLE_RESPONSE_RES));
		sGU_DOJO_SCRAMBLE_RESPONSE_RES * res = (sGU_DOJO_SCRAMBLE_RESPONSE_RES *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_SCRAMBLE_RESPONSE_RES;
		res->wResultCode = req->wResultCode;
		res->bIsRetry = req->bIsRetry;
		packet.SetPacketLen(sizeof(sGU_DOJO_SCRAMBLE_RESPONSE_RES));
		pPlayer->SendPacket(&packet);
		
		//if we cancel the war successfully
		if (req->wResultCode == GAME_SUCCESS && req->bIsAccept == false)
		{
			if (pPlayer->GetZeni() >= DBO_DOJO_SCRAMBLE_REJECT_FARE) //check if we still have the required zeni
				pPlayer->UpdateZeni(ZENNY_CHANGE_TYPE_DOJO_ANTI_SCRAMBLE_DEL, DBO_DOJO_SCRAMBLE_REJECT_FARE, false, true);
			else
				ERR_LOG(LOG_USER, "ERROR: User %u does not have the required zeni anymore. Hacker..", pPlayer->GetCharID());
		}
	}
	else if (req->wResultCode != GAME_SUCCESS && req->bIsAccept == false)
		ERR_LOG(LOG_USER, "ERROR: User %u is not online anymore. Hacker..", pPlayer->GetCharID());
}

void CChatServerSession::RecvDojoScrambleStateChange(CNtlPacket * pPacket)
{
	sTG_DOJO_SCRAMBLE_STATE_CHANGE * req = (sTG_DOJO_SCRAMBLE_STATE_CHANGE *)pPacket->GetPacketData();

	CDojo* pDojo = g_pDojoManager->GetDojoWithTblidx(req->dojoTblidx);
	if (pDojo)
	{
		pDojo->SetState(req->byState, req->tmNextStepTime);
	}
	else ERR_LOG(LOG_USER, "ERROR: Could not find dojo with tblidx %u", req->dojoTblidx);
}


void CChatServerSession::RecvConnectChannelChangeRes(CNtlPacket * pPacket)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sTG_CONNECT_CHANNEL_CHANGE_RES * req = (sTG_CONNECT_CHANNEL_CHANGE_RES *)pPacket->GetPacketData();

	CPlayer* pPlayer = g_pObjectManager->FindByChar(req->characterID);
	if (pPlayer && pPlayer->IsInitialized())
	{
		HSESSION sessionID = pPlayer->GetClientSessionID();

		//send to connect to new channel and disconnect current channel

		CNtlPacket packet(sizeof(sGU_CHAR_CHANNEL_CHANGE_RES));
		sGU_CHAR_CHANNEL_CHANGE_RES * res = (sGU_CHAR_CHANNEL_CHANGE_RES *)packet.GetPacketData();
		res->wOpCode = GU_CHAR_CHANNEL_CHANGE_RES;
		res->wResultCode = GAME_SUCCESS;
		pPlayer->CopyDestServerInfo(&res->serverInfo, res->achAuthKey);
		packet.SetPacketLen(sizeof(sGU_CHAR_CHANNEL_CHANGE_RES));
		app->Send(sessionID, &packet);
	}
}


