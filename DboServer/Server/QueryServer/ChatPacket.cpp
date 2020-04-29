#include "stdafx.h"
#include "QueryServer.h"
#include "ChatServerSession.h"
#include "NtlPacketTQ.h"
#include "NtlPacketQT.h"
#include "NtlResultCode.h"
#include "PlayerCache.h"
#include "AuctionHouse.h"
#include "Dojo.h"
#include "Guild.h"
#include "Utils.h"
#include "CashshopManager.h"
#include "ItemManager.h"



void CChatServerSession::RecvGuildCreateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_CREATE_REQ * req = (sTQ_GUILD_CREATE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_CREATE_RES));
	sQT_GUILD_CREATE_RES * res = (sQT_GUILD_CREATE_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_CREATE_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelIndex = req->byServerChannelIndex;
	res->dwSpentZenny = req->dwRequiredZenny;
	res->creatorAccountId = req->creatorAccountId;
	res->partyId = req->partyId;
	res->byMemberCount = 0;
	res->aMemberData[0].charId = req->memberCharId[0];

	//create guild
	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->memberCharId[0]))
	{
		if (pCache->GetZeni() >= req->dwRequiredZenny && pCache->GetGuildID() == 0)
		{
			sDBO_GUILD_DATA* pGuildData = g_pGuild->CreateGuild(req->memberCharId[0], req->wszGuildName, req->qwGuildFunctionFlag);
			memcpy(&res->guildData, pGuildData, sizeof(sDBO_GUILD_DATA));

			pCache->SetZeni(pCache->GetZeni() - req->dwRequiredZenny);
			GetCharDB.Execute("UPDATE characters SET Money=%u WHERE CharID=%u", pCache->GetZeni(), req->memberCharId[0]);

			//add guild members
			for (BYTE i = 0; i < req->byMemberCount; i++)
			{
				if (CPlayerCache* pMember = g_pPlayerCache->GetCharacter(req->memberCharId[i]))
				{
					if (pMember->GetGuildID() == 0)
					{
						sDBO_GUILD_MEMBER_DATA* pData = new sDBO_GUILD_MEMBER_DATA;
						pData->accountId = pMember->GetAccountID();
						pData->byClass = pMember->GetClass();
						pData->byLevel = pMember->GetLevel();
						pData->byRace = pMember->GetRace();
						pData->charId = pMember->GetCharID();
						pData->dwReputation = pMember->GetReputation();
						NTL_SAFE_WCSCPY(pData->wszMemberName, pMember->GetCharName());

						g_pGuild->AddGuildMember(pGuildData->guildId, pData, pGuildData->wszName);
						memcpy(&res->aMemberData[res->byMemberCount++], pData, sizeof(sDBO_GUILD_MEMBER_DATA));

						//set member guild data
						pMember->SetGuild(pGuildData->guildId, pGuildData->wszName);
					}
				}
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQT_GUILD_CREATE_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvGuildDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_DATA_REQ * req = (sTQ_GUILD_DATA_REQ*)pPacket->GetPacketData();

	UNREFERENCED_PARAMETER(app);
	UNREFERENCED_PARAMETER(req);

	g_pGuild->SendGuildData(GetHandle());
}

void CChatServerSession::RecvGuildDisbandReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_DISBAND_REQ * req = (sTQ_GUILD_DISBAND_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_DISBAND_RES));
	sQT_GUILD_DISBAND_RES * res = (sQT_GUILD_DISBAND_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_DISBAND_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->bByUser = req->bByUser;
	res->guildId = req->guildId;
	res->timeToDisband = req->timeToDisband;


	if (sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(req->guildId))
	{
		g_pGuild->RemoveGuild(req->guildId);
		delete pGuildData;

		if (g_pDojo->DeleteDojo(req->guildId))
			GetCharDB.Execute("DELETE FROM dojos WHERE GuildId=%u", req->guildId);
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQT_GUILD_DISBAND_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvGuildInviteReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_INVITE_REQ * req = (sTQ_GUILD_INVITE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_INVITE_RES));
	sQT_GUILD_INVITE_RES * res = (sQT_GUILD_INVITE_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_INVITE_RES;
	res->wResultCode = GAME_SUCCESS;
	res->guildId = req->guildId;
	res->inviteeCharId = req->inviteeCharId;

	//create guild
	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->inviteeCharId))
	{
		if (pCache->GetGuildID() == 0) //check if not inside a guild
		{
			if (sDBO_GUILD_DATA* pGuild = g_pGuild->GetGuildData(req->guildId))
			{
				sDBO_GUILD_MEMBER_DATA* pData = new sDBO_GUILD_MEMBER_DATA;
				pData->accountId = pCache->GetAccountID();
				pData->byClass = pCache->GetClass();
				pData->byLevel = pCache->GetLevel();
				pData->byRace = pCache->GetRace();
				pData->charId = pCache->GetCharID();
				pData->dwReputation = pCache->GetReputation();
				NTL_SAFE_WCSCPY(pData->wszMemberName, pCache->GetCharName());

				g_pGuild->AddGuildMember(pGuild->guildId, pData, pGuild->wszName);

				//set member guild data
				pCache->SetGuild(pGuild->guildId, pGuild->wszName);
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQT_GUILD_INVITE_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvGuildLeaveReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_LEAVE_REQ * req = (sTQ_GUILD_LEAVE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_LEAVE_RES));
	sQT_GUILD_LEAVE_RES * res = (sQT_GUILD_LEAVE_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_LEAVE_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->leavingMemberCharId = req->leavingMemberCharId;


	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->leavingMemberCharId))
	{
		if (pCache->GetGuildID() != 0) //check if inside guild
		{
			if (sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(pCache->GetGuildID()))
			{
				if (g_pGuild->RemoveGuildMember(pCache->GetGuildID(), req->leavingMemberCharId)) //remove from guild
				{
					res->guildId = pGuildData->guildId;

					//if is second guild master then unset
					for (int i = 0; i < DBO_MAX_SECOND_MASTER_IN_GUILD; i++)
					{
						if (pGuildData->guildSecondMaster[i] == req->leavingMemberCharId)
						{
							pGuildData->guildSecondMaster[i] = INVALID_CHARACTERID;

							CNtlString rowName("GuildSecondMaster");
							if (i > 0)
								rowName.Format("GuildSecondMaster%i", i + 1);

							GetCharDB.Execute("UPDATE guilds SET %s=%u WHERE GuildID=%u", rowName.c_str(), INVALID_CHARACTERID, pCache->GetGuildID());

							break;
						}
					}

					pCache->SetGuild(0, NULL);
				}
				else res->wResultCode = QUERY_FAIL;
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQT_GUILD_LEAVE_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvGuildKickOutReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_KICK_OUT_REQ * req = (sTQ_GUILD_KICK_OUT_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_KICK_OUT_RES));
	sQT_GUILD_KICK_OUT_RES * res = (sQT_GUILD_KICK_OUT_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_KICK_OUT_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->kickedOutMemberCharId = req->kickedOutMemberCharId;
	res->guildId = req->guildId;


	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->kickedOutMemberCharId))
	{
		if (pCache->GetGuildID() != 0) //check if inside guild
		{
			if (sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(pCache->GetGuildID()))
			{
				if (g_pGuild->RemoveGuildMember(pCache->GetGuildID(), req->kickedOutMemberCharId)) //remove from guild
				{
					//if is second guild master then unset
					for (int i = 0; i < DBO_MAX_SECOND_MASTER_IN_GUILD; i++)
					{
						if (pGuildData->guildSecondMaster[i] == req->kickedOutMemberCharId)
						{
							pGuildData->guildSecondMaster[i] = INVALID_CHARACTERID;

							CNtlString rowName("GuildSecondMaster");
							if (i > 0)
								rowName.Format("GuildSecondMaster%i", i + 1);

							GetCharDB.Execute("UPDATE guilds SET %s=%u WHERE GuildID=%u", rowName.c_str(), INVALID_CHARACTERID, pCache->GetGuildID());

							break;
						}
					}

					pCache->SetGuild(0, NULL);
				}
				else res->wResultCode = QUERY_FAIL;
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else
	{
		if (smart_ptr<QueryResult> result = GetCharDB.Query("SELECT GuildID FROM characters WHERE CharID=%u", req->kickedOutMemberCharId))
		{
			Field* f = result->Fetch();

			if (f[0].GetUInt32() != req->guildId)
				res->wResultCode = COMMUNITY_COMMON_TARGET_IS_NOT_YOUR_GUILD_MEMBER;
			else
			{
				if (sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(req->guildId))
				{
					if (g_pGuild->RemoveGuildMember(req->guildId, req->kickedOutMemberCharId)) //remove from guild
					{
						//if is second guild master then unset
						for (int i = 0; i < DBO_MAX_SECOND_MASTER_IN_GUILD; i++)
						{
							if (pGuildData->guildSecondMaster[i] == req->kickedOutMemberCharId)
							{
								pGuildData->guildSecondMaster[i] = INVALID_CHARACTERID;

								CNtlString rowName("GuildSecondMaster");
								if (i > 0)
									rowName.Format("GuildSecondMaster%i", i + 1);

								GetCharDB.Execute("UPDATE guilds SET %s=%u WHERE GuildID=%u", rowName.c_str(), INVALID_CHARACTERID, req->guildId);

								break;
							}
						}
					}
					else res->wResultCode = QUERY_FAIL;
				}
				else res->wResultCode = QUERY_FAIL;
			}
		}
		else res->wResultCode = COMMUNITY_COMMON_NO_SUCH_A_TARGET;
	}


	packet.SetPacketLen(sizeof(sQT_GUILD_KICK_OUT_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvGuildAppointSecondMasterReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_APPOINT_SECOND_MASTER_REQ * req = (sTQ_GUILD_APPOINT_SECOND_MASTER_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_APPOINT_SECOND_MASTER_RES));
	sQT_GUILD_APPOINT_SECOND_MASTER_RES * res = (sQT_GUILD_APPOINT_SECOND_MASTER_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_APPOINT_SECOND_MASTER_RES;
	res->wResultCode = QUERY_FAIL;
	res->targetMemberCharId = req->targetMemberCharId;
	res->guildId = req->guildId;

	if (sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(req->guildId))
	{
		for (int secondMasterSpot = 0; secondMasterSpot < DBO_MAX_SECOND_MASTER_IN_GUILD; secondMasterSpot++)
		{
			if (pGuildData->guildSecondMaster[secondMasterSpot] == INVALID_CHARACTERID)
			{
				pGuildData->guildSecondMaster[secondMasterSpot] = req->targetMemberCharId;

				CNtlString rowName("GuildSecondMaster");
				if (secondMasterSpot > 0)
					rowName.Format("GuildSecondMaster%i", secondMasterSpot + 1);

				GetCharDB.Execute("UPDATE guilds SET %s=%u WHERE GuildID=%u", rowName.c_str(), req->targetMemberCharId, req->guildId);

				res->wResultCode = CHAT_SUCCESS;
				break;
			}
		}
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_GUILD_APPOINT_SECOND_MASTER_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvGuildDismissSecondMasterReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_DISMISS_SECOND_MASTER_REQ * req = (sTQ_GUILD_DISMISS_SECOND_MASTER_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_DISMISS_SECOND_MASTER_RES));
	sQT_GUILD_DISMISS_SECOND_MASTER_RES * res = (sQT_GUILD_DISMISS_SECOND_MASTER_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_DISMISS_SECOND_MASTER_RES;
	res->wResultCode = QUERY_FAIL;
	res->targetMemberCharId = req->targetMemberCharId;
	res->guildId = req->guildId;

	if (sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(req->guildId))
	{
		for (int secondMasterSpot = 0; secondMasterSpot < DBO_MAX_SECOND_MASTER_IN_GUILD; secondMasterSpot++)
		{
			if (pGuildData->guildSecondMaster[secondMasterSpot] == req->targetMemberCharId)
			{
				pGuildData->guildSecondMaster[secondMasterSpot] = INVALID_CHARACTERID;

				CNtlString rowName("GuildSecondMaster");
				if (secondMasterSpot > 0)
					rowName.Format("GuildSecondMaster%i", secondMasterSpot + 1);

				GetCharDB.Execute("UPDATE guilds SET %s=%u WHERE GuildID=%u", rowName.c_str(), INVALID_CHARACTERID, req->guildId);

				res->wResultCode = CHAT_SUCCESS;
				break;
			}
		}
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_GUILD_DISMISS_SECOND_MASTER_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvGuildChangeGuildMasterReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_CHANGE_GUILD_MASTER_REQ * req = (sTQ_GUILD_CHANGE_GUILD_MASTER_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_CHANGE_GUILD_MASTER_RES));
	sQT_GUILD_CHANGE_GUILD_MASTER_RES * res = (sQT_GUILD_CHANGE_GUILD_MASTER_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_CHANGE_GUILD_MASTER_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->newMasterCharId = req->targetMemberCharId;
	res->guildId = req->guildId;

	if (sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(req->guildId))
	{
		pGuildData->guildMaster = req->targetMemberCharId;
		GetCharDB.Execute("UPDATE guilds SET GuildMaster=%u WHERE GuildID=%u", req->targetMemberCharId, req->guildId);
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_GUILD_CHANGE_GUILD_MASTER_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvGuildFunctionAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_FUNCTION_ADD_REQ * req = (sTQ_GUILD_FUNCTION_ADD_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_FUNCTION_ADD_RES));
	sQT_GUILD_FUNCTION_ADD_RES * res = (sQT_GUILD_FUNCTION_ADD_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_FUNCTION_ADD_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelIndex = req->byServerChannelIndex;
	res->byFunction = req->byFunction;
	res->byServerIndex = req->byServerIndex;
	res->dwZenny = req->dwZenny;
	res->guildId = req->guildId;
	res->masterCharId = req->masterCharId;
	res->qwGuildFunctionFlag = req->qwGuildFunctionFlag;

	//create guild
	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->masterCharId))
	{
		if (pCache->GetZeni() >= req->dwZenny && pCache->GetGuildID() != 0)
		{
			sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(req->guildId);
			if (pGuildData)
			{
				pGuildData->dwGuildReputation = UnsignedSafeDecrease<DWORD>(pGuildData->dwGuildReputation, Dbo_GetGuildFunctionInfo(static_cast<eDBO_GUILD_FUNCTION>(req->byFunction))->dwRequiredPoint);
				pGuildData->qwGuildFunctionFlag = req->qwGuildFunctionFlag;

				GetCharDB.Execute("UPDATE guilds SET GuildReputation=%u, FunctionFlag=%I64u WHERE GuildID=%u", pGuildData->dwGuildReputation, pGuildData->qwGuildFunctionFlag, req->guildId);

				if (req->dwZenny > 0)
				{
					pCache->SetZeni(pCache->GetZeni() - req->dwZenny);
					GetCharDB.Execute("UPDATE characters SET Money=%u WHERE CharID=%u", pCache->GetZeni(), req->masterCharId);
				}
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQT_GUILD_FUNCTION_ADD_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvGuildGiveZeniReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_GIVE_ZENNY_REQ * req = (sTQ_GUILD_GIVE_ZENNY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_GIVE_ZENNY_RES));
	sQT_GUILD_GIVE_ZENNY_RES * res = (sQT_GUILD_GIVE_ZENNY_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_GIVE_ZENNY_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelIndex = req->byServerChannelIndex;
	res->charId = req->charId;
	res->guildId = req->guildId;
	res->byServerIndex = req->byServerIndex;
	res->dwAddReputaion = Dbo_GetGuildPoint(req->dwZenny);
	res->dwZenny = req->dwZenny;

	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId))
	{
		if (pCache->GetZeni() >= req->dwZenny)
		{
			if (sDBO_GUILD_DATA* pGuild = g_pGuild->GetGuildData(req->guildId))
			{
				pCache->SetZeni(pCache->GetZeni() - req->dwZenny);
				GetCharDB.Execute("UPDATE characters SET Money=%u WHERE CharID=%u", pCache->GetZeni(), req->charId);

				pGuild->dwGuildReputation = UnsignedSafeIncrease<DWORD>(pGuild->dwGuildReputation, res->dwAddReputaion);
				pGuild->dwMaxGuildPointEver = UnsignedSafeIncrease<DWORD>(pGuild->dwMaxGuildPointEver, res->dwAddReputaion);

				GetCharDB.Execute("UPDATE guilds SET GuildReputation=%u, GuildPointEver=%u WHERE GuildID=%u", pGuild->dwGuildReputation, pGuild->dwMaxGuildPointEver, req->guildId);
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_GUILD_GIVE_ZENNY_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvGuildChangeNoticeReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_CHANGE_NOTICE_REQ * req = (sTQ_GUILD_CHANGE_NOTICE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_CHANGE_NOTICE_RES));
	sQT_GUILD_CHANGE_NOTICE_RES * res = (sQT_GUILD_CHANGE_NOTICE_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_CHANGE_NOTICE_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->charId = req->charId;
	res->guildId = req->guildId;
	res->wNoticeLengthInUnicode = req->wNoticeLengthInUnicode;
	NTL_SAFE_WCSCPY(res->awchCharName, req->awchCharName);
	NTL_SAFE_WCSCPY(res->awchNotice, req->awchNotice);

	if (sDBO_GUILD_DATA* pGuild = g_pGuild->GetGuildData(req->guildId))
	{
		NTL_SAFE_WCSCPY(pGuild->awchName, req->awchCharName);
		NTL_SAFE_WCSCPY(pGuild->awchNotice, req->awchNotice);

		char* message = Ntl_WC2MB(req->awchNotice);

		GetCharDB.Execute("UPDATE guilds SET NoticeBy=\"%ls\", GuildNotice=\"%s\" WHERE GuildID=%u", pGuild->awchName, GetCharDB.EscapeString(message).c_str(), req->guildId);

		Ntl_CleanUpHeapString(message);
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_GUILD_CHANGE_NOTICE_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvGuildCreateMarkReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_CREATE_MARK_REQ * req = (sTQ_GUILD_CREATE_MARK_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_CREATE_MARK_RES));
	sQT_GUILD_CREATE_MARK_RES * res = (sQT_GUILD_CREATE_MARK_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_CREATE_MARK_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelIndex = req->byServerChannelIndex;
	res->charId = req->charId;
	res->guildId = req->guildId;
	memcpy(&res->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));

	if (sDBO_GUILD_DATA* pGuild = g_pGuild->GetGuildData(req->guildId))
	{
		memcpy(&pGuild->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));

		GetCharDB.Execute("UPDATE guilds SET MarkInColor=%u, MarkInLine=%u, MarkMain=%u, MarkMainColor=%u, MarkOutColor=%u, MarkOutLine=%u WHERE GuildID=%u",
			req->sMark.byMarkInColor, req->sMark.byMarkInLine, req->sMark.byMarkMain, req->sMark.byMarkMainColor, req->sMark.byMarkOutColor, req->sMark.byMarkOutLine, req->guildId);
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_GUILD_CREATE_MARK_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvGuildChangeMarkReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_CHANGE_MARK_REQ * req = (sTQ_GUILD_CHANGE_MARK_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_CHANGE_MARK_RES));
	sQT_GUILD_CHANGE_MARK_RES * res = (sQT_GUILD_CHANGE_MARK_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_CHANGE_MARK_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelIndex = req->byServerChannelIndex;
	res->charId = req->charId;
	res->guildId = req->guildId;
	memcpy(&res->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));
	res->dwZenny = req->dwZenny;

	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId))
	{
		if (pCache->GetZeni() >= req->dwZenny)
		{
			if (sDBO_GUILD_DATA* pGuild = g_pGuild->GetGuildData(req->guildId))
			{
				pCache->SetZeni(pCache->GetZeni() - req->dwZenny);
				GetCharDB.Execute("UPDATE characters SET Money=%u WHERE CharID=%u", pCache->GetZeni(), req->charId);

				memcpy(&pGuild->sMark, &req->sMark, sizeof(sDBO_GUILD_MARK));

				GetCharDB.Execute("UPDATE guilds SET MarkInColor=%u, MarkInLine=%u, MarkMain=%u, MarkMainColor=%u, MarkOutColor=%u, MarkOutLine=%u WHERE GuildID=%u",
					req->sMark.byMarkInColor, req->sMark.byMarkInLine, req->sMark.byMarkMain, req->sMark.byMarkMainColor, req->sMark.byMarkOutColor, req->sMark.byMarkOutLine, req->guildId);
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_GUILD_CHANGE_MARK_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvGuildChangeNameRes(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_GUILD_CHANGE_NAME_REQ * req = (sTQ_GUILD_CHANGE_NAME_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_GUILD_CHANGE_NAME_RES));
	sQT_GUILD_CHANGE_NAME_RES * res = (sQT_GUILD_CHANGE_NAME_RES *)packet.GetPacketData();
	res->wOpCode = QT_GUILD_CHANGE_NAME_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelIndex = req->byServerChannelIndex;
	res->charId = req->charId;
	res->guildId = req->guildId;
	res->byPlace = req->byPlace;
	res->byPos = req->byPos;
	res->itemId = req->itemId;
	NTL_SAFE_WCSCPY(res->wszGuildName, req->wszGuildName);

	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId))
	{
		if (sDBO_GUILD_DATA* pGuild = g_pGuild->GetGuildData(req->guildId))
		{
			if (pCache->RemoveItem(req->itemId))
			{
				GetCharDB.Execute("DELETE FROM items WHERE id=%I64u", req->itemId);

				GetLogDB.Execute("INSERT INTO guild_name_change_log (GuildID,CurrentName,NewName) VALUES (%u,\"%ls\",\"%ls\")", req->guildId, pGuild->wszName, req->wszGuildName);

				NTL_SAFE_WCSCPY(pGuild->wszName, req->wszGuildName);

				GetCharDB.WaitExecute("UPDATE guilds SET GuildName=\"%ls\" WHERE GuildID=%u", req->wszGuildName, req->guildId); //must be wait to avoid 2 people creating the same name in the same time
				GetCharDB.Execute("UPDATE characters SET GuildName=\"%ls\" WHERE GuildID=%u", req->wszGuildName, req->guildId);

				if (sDBO_DOJO_DATA* pDojoData = g_pDojo->GetDojoData(req->guildId))
				{
					GetCharDB.Execute("UPDATE dojos SET GuildName=\"%ls\" WHERE GuildID=%u", req->wszGuildName, req->guildId);
					NTL_SAFE_WCSCPY(pDojoData->wszName, req->wszGuildName);
				}
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_GUILD_CHANGE_NAME_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvDojoDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_DATA_REQ * req = (sTQ_DOJO_DATA_REQ*)pPacket->GetPacketData();

	UNREFERENCED_PARAMETER(app);
	UNREFERENCED_PARAMETER(req);

	g_pDojo->SendDojoData(GetHandle());
}

void CChatServerSession::RecvDojoCreateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_CREATE_REQ * req = (sTQ_DOJO_CREATE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_DOJO_CREATE_RES));
	sQT_DOJO_CREATE_RES * res = (sQT_DOJO_CREATE_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_CREATE_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelId = req->byServerChannelId;
	res->charId = req->charId;
	res->dojoTblidx = req->dojoTblidx;
	res->guildId = req->guildId;
	res->byLevel = 1;

	if (sDBO_GUILD_DATA* pGuild = g_pGuild->GetGuildData(req->guildId))
	{
		g_pDojo->CreateDojo(req->guildId, req->dojoTblidx, pGuild->wszName);
		GetCharDB.Execute("INSERT INTO dojos (GuildId, DojoTblidx, GuildName) VALUES (%u,%u,\"%ls\")", req->guildId, req->dojoTblidx, pGuild->wszName);
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_DOJO_CREATE_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvDojoUpdateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_UPDATE_REQ * req = (sTQ_DOJO_UPDATE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_DOJO_UPDATE_RES));
	sQT_DOJO_UPDATE_RES * res = (sQT_DOJO_UPDATE_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_UPDATE_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->byServerChannelId = req->byServerChannelId;
	memcpy(&res->sDojoData, &req->sDojoData, sizeof(sDBO_DOJO_DATA));

	if (sDBO_DOJO_DATA* pDojo = g_pDojo->GetDojoDataWithIdx(req->sDojoData.dojoTblidx))
	{
		res->guildId = pDojo->guildId;

		memcpy(pDojo, &req->sDojoData, sizeof(sDBO_DOJO_DATA));

		GetCharDB.Execute("UPDATE dojos SET GuildId=%u, Level=0, PeaceStatus=0, PeacePoints=0, GuildName=\"%ls\", LeaderName=(null), Notice=(null), ChallengeGuildId=%u, SeedCharName=(null) WHERE DojoTblidx=%u", 
			pDojo->guildId, pDojo->wszName, INVALID_GUILDID, pDojo->dojoTblidx);

		g_pDojo->RemoveDojo(res->guildId);
		g_pDojo->InsertDojo(req->sDojoData.guildId, pDojo);
	}
	else
		res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_DOJO_UPDATE_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvDojoDeleteReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_DEL_REQ * req = (sTQ_DOJO_DEL_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_DOJO_DEL_RES));
	sQT_DOJO_DEL_RES * res = (sQT_DOJO_DEL_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_DEL_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelId = req->byServerChannelId;
	res->charId = req->charId;
	res->dojoTblidx = req->dojoTblidx;
	res->guildId = req->guildId;
	
	if (g_pDojo->DeleteDojo(req->guildId, req->dojoTblidx))
		GetCharDB.Execute("DELETE FROM dojos WHERE DojoTblidx=%u", req->dojoTblidx);
	else
		res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_DOJO_DEL_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvDojoFunctionAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_FUNCTION_ADD_REQ * req = (sTQ_DOJO_FUNCTION_ADD_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_DOJO_FUNCTION_ADD_RES));
	sQT_DOJO_FUNCTION_ADD_RES * res = (sQT_DOJO_FUNCTION_ADD_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_FUNCTION_ADD_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byServerChannelIndex = req->byServerChannelIndex;
	res->byFunction = req->byFunction;
	res->byServerIndex = req->byServerIndex;
	res->dwZenny = req->dwZenny;
	res->guildId = req->guildId;
	res->masterCharId = req->masterCharId;
	res->qwGuildFunctionFlag = req->qwGuildFunctionFlag;

	//create guild
	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->masterCharId))
	{
		if (pCache->GetZeni() >= req->dwZenny && pCache->GetGuildID() != 0)
		{
			sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(req->guildId);
			if (pGuildData)
			{
				pGuildData->dwGuildReputation = UnsignedSafeDecrease<DWORD>(pGuildData->dwGuildReputation, Dbo_GetGuildFunctionInfo(static_cast<eDBO_GUILD_FUNCTION>(req->byFunction))->dwRequiredPoint);
				pGuildData->qwGuildFunctionFlag = req->qwGuildFunctionFlag;

				GetCharDB.Execute("UPDATE guilds SET GuildReputation=%u, FunctionFlag=%I64u WHERE GuildID=%u", pGuildData->dwGuildReputation, pGuildData->qwGuildFunctionFlag, req->guildId);

				BYTE byNewDojoLevel = Dbo_GetDojoLevel(req->byFunction);
				if (byNewDojoLevel > 0)
				{
					if (sDBO_DOJO_DATA* pDojoData = g_pDojo->GetDojoData(req->guildId))
					{
						pDojoData->byLevel = byNewDojoLevel;
						GetCharDB.Execute("UPDATE dojos SET Level=%u WHERE GuildID=%u", byNewDojoLevel, req->guildId);
					}
				}

				if (req->dwZenny > 0)
				{
					pCache->SetZeni(pCache->GetZeni() - req->dwZenny);
					GetCharDB.Execute("UPDATE characters SET Money=%u WHERE CharID=%u", pCache->GetZeni(), req->masterCharId);
				}
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQT_DOJO_FUNCTION_ADD_RES));
	app->Send(GetHandle(), &packet);
}


void CChatServerSession::RecvDojoBudokaiSeedAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_BUDOKAI_SEED_ADD_REQ * req = (sTQ_DOJO_BUDOKAI_SEED_ADD_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_DOJO_BUDOKAI_SEED_ADD_RES));
	sQT_DOJO_BUDOKAI_SEED_ADD_RES * res = (sQT_DOJO_BUDOKAI_SEED_ADD_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_BUDOKAI_SEED_ADD_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->charId = req->charId;
	NTL_SAFE_WCSCPY(res->wszCharName, req->wszCharName);
	res->dojoTblidx = req->dojoTblidx;

	sDBO_DOJO_DATA* pDojo = g_pDojo->GetDojoDataWithIdx(req->dojoTblidx);
	if (pDojo)
	{
		NTL_SAFE_WCSCPY(pDojo->wszSeedCharName, req->wszCharName);
		GetCharDB.Execute("UPDATE dojos SET SeedCharName=\"%ls\" WHERE DojoTblidx=%u", pDojo->wszSeedCharName, req->dojoTblidx);
	}
	else res->wResultCode = COMMUNITY_DOJO_FIND_FAIL;

	packet.SetPacketLen(sizeof(sQT_DOJO_BUDOKAI_SEED_ADD_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvDojoBudokaiSeedDelReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_BUDOKAI_SEED_DEL_REQ * req = (sTQ_DOJO_BUDOKAI_SEED_DEL_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_DOJO_BUDOKAI_SEED_DEL_RES));
	sQT_DOJO_BUDOKAI_SEED_DEL_RES * res = (sQT_DOJO_BUDOKAI_SEED_DEL_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_BUDOKAI_SEED_DEL_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->charId = req->charId;
	NTL_SAFE_WCSCPY(res->wszCharName, req->wszCharName);

	sDBO_DOJO_DATA* pDojo = g_pDojo->GetDojoDataWithIdx(req->dojoTblidx);
	if (pDojo)
	{
		memset(pDojo->wszSeedCharName, 0, sizeof(pDojo->wszSeedCharName));
		GetCharDB.Execute("UPDATE dojos SET SeedCharName=(Null) WHERE DojoTblidx=%u", req->dojoTblidx);
	}
	else res->wResultCode = COMMUNITY_DOJO_FIND_FAIL;

	packet.SetPacketLen(sizeof(sQT_DOJO_BUDOKAI_SEED_DEL_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvDojoScrambleRewardReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_SCRAMBLE_REWARD_REQ * req = (sTQ_DOJO_SCRAMBLE_REWARD_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_DOJO_SCRAMBLE_REWARD_RES));
	sQT_DOJO_SCRAMBLE_REWARD_RES * res = (sQT_DOJO_SCRAMBLE_REWARD_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_SCRAMBLE_REWARD_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->dojoTblidx = req->dojoTblidx;
	res->guildId = req->guildId;
	res->dwAddGuildReputation = req->dwAddGuildReputation;
	res->qwGuildFunctionFlag = req->qwGuildFunctionFlag;
	res->itemTblidx = req->sItem.itemNo;
	res->byCount = req->sItem.byStackcount;
	res->byLevel = 0;

	if (sDBO_GUILD_DATA* pGuildData = g_pGuild->GetGuildData(req->guildId))
	{
		pGuildData->dwGuildReputation = UnsignedSafeIncrease<DWORD>(pGuildData->dwGuildReputation, req->dwAddGuildReputation);
		pGuildData->dwMaxGuildPointEver = UnsignedSafeIncrease<DWORD>(pGuildData->dwMaxGuildPointEver, req->dwAddGuildReputation);

		GetCharDB.Execute("UPDATE guilds SET GuildReputation=%u, GuildPointEver=%u WHERE GuildID=%u", pGuildData->dwGuildReputation, pGuildData->dwMaxGuildPointEver, req->guildId);

		//create item and put into guild bank
		//TODO
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQT_DOJO_SCRAMBLE_REWARD_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvDojoChangeNoticeReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_DOJO_CHANGE_NOTICE_REQ * req = (sTQ_DOJO_CHANGE_NOTICE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_DOJO_CHANGE_NOTICE_RES));
	sQT_DOJO_CHANGE_NOTICE_RES * res = (sQT_DOJO_CHANGE_NOTICE_RES *)packet.GetPacketData();
	res->wOpCode = QT_DOJO_CHANGE_NOTICE_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->wNoticeLengthInUnicode = req->wNoticeLengthInUnicode;
	res->dojoTblidx = req->dojoTblidx;
	NTL_SAFE_WCSCPY(res->awchCharName, req->awchCharName);
	NTL_SAFE_WCSCPY(res->awchNotice, req->awchNotice);
	res->guildId = req->guildId;
	res->charId = req->charId;

	
	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId))
	{
		sDBO_DOJO_DATA* pDojoData = g_pDojo->GetDojoData(req->guildId);
		if (pDojoData)
		{
			char* message = Ntl_WC2MB(req->awchNotice);

			GetCharDB.Execute("UPDATE dojos SET LeaderName=\"%ls\", Notice=\"%s\" WHERE GuildId=%u", req->awchCharName, GetCharDB.EscapeString(message).c_str(), req->guildId);

			NTL_SAFE_WCSCPY(pDojoData->wchLeaderName, req->awchCharName);
			NTL_SAFE_WCSCPY(pDojoData->wchNotice, req->awchNotice);

			Ntl_CleanUpHeapString(message);
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQT_DOJO_CHANGE_NOTICE_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvAuctionHouseSellReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_TENKAICHIDAISIJYOU_SELL_REQ * req = (sTQ_TENKAICHIDAISIJYOU_SELL_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_TENKAICHIDAISIJYOU_SELL_RES));
	sQT_TENKAICHIDAISIJYOU_SELL_RES * res = (sQT_TENKAICHIDAISIJYOU_SELL_RES *)packet.GetPacketData();
	res->wOpCode = QT_TENKAICHIDAISIJYOU_SELL_RES;
	res->byServerChannelIndex = req->byServerChannelIndex;
	res->byServerIndex = req->byServerIndex;
	res->charId = req->charId;
	res->byPlace = req->byPlace;
	res->byPosition = req->byPosition;
	res->dwFee = req->dwFee;
	res->wResultCode = GAME_SUCCESS;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetZeni() >= req->dwFee)
		{
			if (sITEM_DATA* pItem = pCache->GetItemData(req->itemId))
			{
				if (pItem->byStackcount > req->byCount) // if we have more stack count than we sell
				{
					// create new item
					sITEM_DATA rItemData;
					memcpy(&rItemData, pItem, sizeof(sITEM_DATA));
					rItemData.charId = 0;
					rItemData.byStackcount = req->byCount;
					rItemData.itemId = g_pItemManager->CreateItem(rItemData);


					sTENKAICHIDAISIJYOU_DATA* pData = new sTENKAICHIDAISIJYOU_DATA;
					pData->nItem = g_pAH->AcquireID();
					pData->charId = req->charId;
					pData->byTabType = req->byTabType;
					pData->byItemType = req->byItemType;
					pData->byItemLevel = req->byItemLevel;
					NTL_SAFE_WCSCPY(pData->awchItemName, req->awchItemName);
					NTL_SAFE_WCSCPY(pData->awchSeller, req->awchSeller);
					pData->dwPrice = req->dwPrice;
					pData->nStartSellTime = req->nStartSellTime;
					pData->nEndSellTime = req->nEndSellTime;
					pData->itemId = rItemData.itemId;
					pData->itemNo = pItem->itemNo;
					pData->byCount = req->byCount;
					pData->dwNeed_Class_Bit_Flag = req->dwClassBitFlag;
					pData->byRank = pItem->byRank;
					pData->byGrade = pItem->byGrade;
					pData->byCurrentDurability = pItem->byCurrentDurability;
					pData->byBattleAttribute = pItem->byBattleAttribute;
					NTL_SAFE_WCSCPY(pData->awchMaker, pItem->awchMaker);
					memcpy(&pData->sOptionSet, &pItem->sOptionSet, sizeof(sITEM_OPTION_SET));
					pData->nUseEndTime = pItem->nUseEndTime;
					pData->byRestrictState = pItem->byRestrictState;

					//ADD ITEM TO AH MAP
					g_pAH->InsertItem(pData);

					//add to db
					GetCharDB.Execute("INSERT INTO auctionhouse (id,CharID,TabType,ItemName,Seller,Price,ItemID,TimeStart,TimeEnd,ItemLevel,NeedClass,ItemType) VALUES (%I64u, %u, %u,\"%ls\",\"%ls\", %u, %I64u, %I64u, %u, %u, %u, %u)",
						pData->nItem, req->charId, req->byTabType, req->awchItemName, pData->awchSeller, req->dwPrice, rItemData.itemId, req->nStartSellTime, req->nEndSellTime, req->byItemLevel, req->dwClassBitFlag, req->byItemType);


					// update stack count from original item
					pItem->byStackcount -= req->byCount;
					GetCharDB.Execute("UPDATE items SET count=%u WHERE id=%I64u", pItem->byStackcount, pItem->itemId);

					//update player zeni & db
					pCache->SetZeni(pCache->GetZeni() - req->dwFee);
					GetCharDB.Execute("UPDATE characters SET Money=%u WHERE CharID=%u", pCache->GetZeni(), req->charId);

					// packet
					memcpy(&res->sData, pData, sizeof(sTENKAICHIDAISIJYOU_DATA));
				}
				else if (pItem->byStackcount == req->byCount)
				{
					sTENKAICHIDAISIJYOU_DATA* pData = new sTENKAICHIDAISIJYOU_DATA;
					pData->nItem = g_pAH->AcquireID();
					pData->charId = req->charId;
					pData->byTabType = req->byTabType;
					pData->byItemType = req->byItemType;
					pData->byItemLevel = req->byItemLevel;
					NTL_SAFE_WCSCPY(pData->awchItemName, req->awchItemName);
					NTL_SAFE_WCSCPY(pData->awchSeller, req->awchSeller);
					pData->dwPrice = req->dwPrice;
					pData->nStartSellTime = req->nStartSellTime;
					pData->nEndSellTime = req->nEndSellTime;
					pData->itemId = req->itemId;
					pData->itemNo = pItem->itemNo;
					pData->byCount = req->byCount;
					pData->dwNeed_Class_Bit_Flag = req->dwClassBitFlag;
					pData->byRank = pItem->byRank;
					pData->byGrade = pItem->byGrade;
					pData->byCurrentDurability = pItem->byCurrentDurability;
					pData->byBattleAttribute = pItem->byBattleAttribute;
					NTL_SAFE_WCSCPY(pData->awchMaker, pItem->awchMaker);
					memcpy(&pData->sOptionSet, &pItem->sOptionSet, sizeof(sITEM_OPTION_SET));
					pData->nUseEndTime = pItem->nUseEndTime;
					pData->byRestrictState = pItem->byRestrictState;
					//ADD ITEM TO AH MAP
					g_pAH->InsertItem(pData); 

					//delete & remove item from seller
					pCache->RemoveItem(req->itemId);

					//unset owner
					GetCharDB.Execute("UPDATE items SET owner_id=0 WHERE id=%I64u", req->itemId);

					//add to db
					GetCharDB.Execute("INSERT INTO auctionhouse (id,CharID,TabType,ItemName,Seller,Price,ItemID,TimeStart,TimeEnd,ItemLevel,NeedClass,ItemType) VALUES (%I64u, %u, %u,\"%ls\",\"%ls\", %u, %I64u, %I64u, %u, %u, %u, %u)",
						pData->nItem, req->charId, req->byTabType, req->awchItemName, pData->awchSeller, req->dwPrice, req->itemId, req->nStartSellTime, req->nEndSellTime, req->byItemLevel, req->dwClassBitFlag, req->byItemType);

					//update player zeni & db
					pCache->SetZeni(pCache->GetZeni() - req->dwFee);
					GetCharDB.Execute("UPDATE characters SET Money=%u WHERE CharID=%u", pCache->GetZeni(), req->charId);

					// packet
					memcpy(&res->sData, pData, sizeof(sTENKAICHIDAISIJYOU_DATA));
				}
				else res->wResultCode = TENKAICHIDAISIJYOU_CANNOT_LACK_OF_ITEM_STACK;
			}
			else res->wResultCode = TENKAICHIDAISIJYOU_CANNOT_INVALID_ITEM;
		}
		else res->wResultCode = TENKAICHIDAISIJYOU_CANNOT_SELL_NO_MONEY;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_TENKAICHIDAISIJYOU_SELL_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvAuctionHouseSellCancelReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ * req = (sTQ_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES));
	sQT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES * res = (sQT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES *)packet.GetPacketData();
	res->wOpCode = QT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES;
	res->charId = req->charId;
	res->wResultCode = GAME_SUCCESS;
	res->nItem = req->nItem;
	res->mailId = INVALID_MAILID;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sTENKAICHIDAISIJYOU_DATA* pData = g_pAH->GetItem(req->nItem))
		{
			int mailtextsize = (int)wcslen(req->wchText);
			DBOTIME createtime = time(NULL);
			DBOTIME endtime = createtime + (10 * 86400); // 10 days
			SYSTEMTIME ti;
			GetLocalTime(&ti);

			//enter email to database
			GetCharDB.Execute("INSERT INTO mail (CharID, SenderType, MailType, TextSize, Text, itemId, FromName, CreateTime, EndTime, RemainDay,year,month,day,hour,minute,second) VALUES (%u,%u,%u,%u,\"%ls\",%I64u,'[DBOG]System',%I64u,%I64u,%u,%u,%u,%u,%u,%u,%u)",
				req->charId, eMAIL_SENDER_TYPE_SYSTEM, eMAIL_TYPE_ITEM, mailtextsize, req->wchText, pData->itemId, createtime, endtime, 10, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond);

			//delete & remove from AH & DB
			delete pData;
			g_pAH->EraseItem(req->nItem);
			GetCharDB.Execute("DELETE FROM auctionhouse WHERE id=%I64u", req->nItem);
		}
		else res->wResultCode = TENKAICHIDAISIJYOU_CANNOT_NOT_EXIST;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_TENKAICHIDAISIJYOU_SELL_CANCEL_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvAuctionHouseBuyReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_TENKAICHIDAISIJYOU_BUY_REQ * req = (sTQ_TENKAICHIDAISIJYOU_BUY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_TENKAICHIDAISIJYOU_BUY_RES));
	sQT_TENKAICHIDAISIJYOU_BUY_RES * res = (sQT_TENKAICHIDAISIJYOU_BUY_RES *)packet.GetPacketData();
	res->wOpCode = QT_TENKAICHIDAISIJYOU_BUY_RES;
	res->charId = req->charId;
	res->dwMoney = 0;
	res->nItem = req->nItem;
	res->wResultCode = GAME_SUCCESS;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetZeni() >= req->dwMoney)
		{
			if (sTENKAICHIDAISIJYOU_DATA* pData = g_pAH->GetItem(req->nItem))
			{
				GetLogDB.Execute("INSERT INTO auctionhouse_log (Seller,Buyer,Price,ItemTblidx,ItemID) VALUES (%u,%u,%u,%u,%I64u)", req->sellcharId, res->charId, req->dwMoney, pData->itemNo, pData->itemId);

				//insert into log

				DBOTIME createtime = time(NULL);
				DBOTIME endtime = createtime + (10 * 86400); // 10 days

				SYSTEMTIME ti;
				GetLocalTime(&ti);

				int buyTextSize = (int)wcslen(req->wchBuyText);
				int sellTextSize = (int)wcslen(req->wchSellText);

				//enter email to database <buyer>
				GetCharDB.Execute("INSERT INTO mail (CharID, SenderType, MailType, TextSize, Text, itemId, FromName, CreateTime, EndTime, RemainDay,year,month,day,hour,minute,second) VALUES (%u,%u,%u,%u,\"%ls\",%I64u,'[DBOG]System',%I64u,%I64u,%u,%u,%u,%u,%u,%u,%u)",
					req->charId, eMAIL_SENDER_TYPE_SYSTEM, eMAIL_TYPE_ITEM, buyTextSize, req->wchBuyText, pData->itemId, createtime, endtime, 10, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond);

				//enter email to database <seller>
				GetCharDB.Execute("INSERT INTO mail (CharID, SenderType, MailType, TextSize, Text, Zenny, FromName, CreateTime, EndTime, RemainDay,year,month,day,hour,minute,second) VALUES (%u,%u,%u,%u,\"%ls\",%u,'[DBOG]System',%I64u,%I64u,%u,%u,%u,%u,%u,%u,%u)",
					req->sellcharId, eMAIL_SENDER_TYPE_SYSTEM, eMAIL_TYPE_ZENNY, sellTextSize, req->wchSellText, req->dwMoney, createtime, endtime, 10, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond);

				//delete & remove from AH & DB
				delete pData;
				g_pAH->EraseItem(req->nItem);
				GetCharDB.Execute("DELETE FROM auctionhouse WHERE id=%I64u", req->nItem);

				//remove & update zeni from buyer
				pCache->SetZeni(pCache->GetZeni() - req->dwMoney);
				GetCharDB.Execute("UPDATE characters SET Money=%u WHERE CharID=%u", pCache->GetZeni(), req->charId);

				res->dwMoney = pCache->GetZeni();
			}
			else res->wResultCode = TENKAICHIDAISIJYOU_CANNOT_NOT_EXIST;
		}
		else res->wResultCode = TENKAICHIDAISIJYOU_CANNOT_BUY_NO_MONEY;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_TENKAICHIDAISIJYOU_BUY_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvAuctionHouseServerDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_TENKAICHIDAISIJYOU_SERVERDATA_REQ * req = (sTQ_TENKAICHIDAISIJYOU_SERVERDATA_REQ*)pPacket->GetPacketData();
	UNREFERENCED_PARAMETER(req);

	int nCount = 0;
	int nTotal = g_pAH->GetCount();

	CNtlPacket packet(sizeof(sQT_TENKAICHIDAISIJYOU_SERVERDATA_RES));
	sQT_TENKAICHIDAISIJYOU_SERVERDATA_RES * res = (sQT_TENKAICHIDAISIJYOU_SERVERDATA_RES *)packet.GetPacketData();
	res->wOpCode = QT_TENKAICHIDAISIJYOU_SERVERDATA_RES;
	res->byCurPacketCount = 0;
	res->bEndList = false;

	for (auto it = g_pAH->GetBegin(); it != g_pAH->GetEnd(); it++)
	{
		nCount++;
		memcpy(&res->sData[res->byCurPacketCount++], it->second, sizeof(sTENKAICHIDAISIJYOU_DATA));

		if (res->byCurPacketCount == 10)
		{
			packet.SetPacketLen(sizeof(sQT_TENKAICHIDAISIJYOU_SERVERDATA_RES));
			app->Send(GetHandle(), &packet);

			res->byCurPacketCount = 0;
			memset(res->sData, NULL, sizeof(res->sData));
		}

	}

	res->bEndList = (nCount >= nTotal) ? true : false;
	packet.SetPacketLen(sizeof(sQT_TENKAICHIDAISIJYOU_SERVERDATA_RES));
	app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvAuctionHousePeriodEndReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sTQ_TENKAICHIDAISIJYOU_PERIODEND_REQ * req = (sTQ_TENKAICHIDAISIJYOU_PERIODEND_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_TENKAICHIDAISIJYOU_PERIODEND_RES));
	sQT_TENKAICHIDAISIJYOU_PERIODEND_RES * res = (sQT_TENKAICHIDAISIJYOU_PERIODEND_RES *)packet.GetPacketData();
	res->wOpCode = QT_TENKAICHIDAISIJYOU_PERIODEND_RES;
	res->wResultCode = GAME_SUCCESS;
	res->nItem = req->nItem;
	res->mailId = INVALID_MAILID;
	res->bList = false;
	res->charId = req->charId;
	res->itemId = req->itemId;
	res->dwPrice = req->dwPrice;

	if (sTENKAICHIDAISIJYOU_DATA* pData = g_pAH->GetItem(req->nItem))
	{
		DBOTIME createtime = time(NULL);
		DBOTIME endtime = createtime + (10 * 86400); // 10 days

		SYSTEMTIME ti;
		GetLocalTime(&ti);

		//enter email to database
		GetCharDB.Execute("INSERT INTO mail (CharID, SenderType, MailType, TextSize, Text, itemId, FromName, CreateTime, EndTime, RemainDay,year,month,day,hour,minute,second) VALUES (%u,%u,%u,%u,\"%ls\",%I64u,'[DBOG]System',%I64u,%I64u,%u,%u,%u,%u,%u,%u,%u)",
			pData->charId, eMAIL_SENDER_TYPE_SYSTEM, eMAIL_TYPE_ITEM, (int)wcslen(req->wchText), req->wchText, pData->itemId, createtime, endtime, 10, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond);


		//delete & remove from AH & DB
		delete pData;
		g_pAH->EraseItem(req->nItem);
		GetCharDB.Execute("DELETE FROM auctionhouse WHERE id=%I64u", req->nItem);
	}
	else res->wResultCode = TENKAICHIDAISIJYOU_CANNOT_NOT_EXIST;

	//packet.SetPacketLen(sizeof(sQT_TENKAICHIDAISIJYOU_PERIODEND_RES));
	//app->Send(GetHandle(), &packet);
}

void CChatServerSession::RecvMutePlayerNfy(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sTQ_MUTE_PLAYER_NFY * req = (sTQ_MUTE_PLAYER_NFY*)pPacket->GetPacketData();

	if (req->dwDurationInMinute > 0) //check if we mute
	{
		DBOTIME muteDuration = req->dwDurationInMinute * 60 * 1000; // minutes to milliseconds

		DBOTIME createtime = time(NULL);
		DBOTIME endtime = createtime + (10 * 86400); // 10 days

		SYSTEMTIME ti;
		GetLocalTime(&ti);

		CHARACTERID charid = req->charId;

		if (charid == INVALID_CHARACTERID)
		{
			smart_ptr<QueryResult> result = GetCharDB.Query("SELECT CharID FROM characters WHERE CharName=\"%ls\"", req->awchCharName);
			if (result)
			{
				Field* f = result->Fetch();

				charid = f[0].GetUInt32();
			}
			else
			{
				return;
			}
		}


		//enter log
		GetLogDB.Execute("INSERT INTO mute_log (CharID,GmAccountID,DurationInMinutes,Reason,muteUntil) values (%u, %u, %u, \"%ls\", %I64u)",
			charid, req->accountId, req->dwDurationInMinute, req->wchReason, muteDuration);

		//enter mail
		GetCharDB.Execute("INSERT INTO mail (CharID, SenderType, MailType, TextSize, Text, FromName, CreateTime, EndTime, RemainDay,year,month,day,hour,minute,second) VALUES (%u,%u,%u,%u, \"%ls\", \"%ls\", %I64u,%I64u,%u,%u,%u,%u,%u,%u,%u)",
			charid, eMAIL_SENDER_TYPE_SYSTEM, eMAIL_TYPE_BASIC, (int)wcslen(req->wchReason), req->wchReason, req->awchGmCharName, createtime, endtime, 10, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond);
	}
	else // unmute
	{
		GetLogDB.Execute("DELETE FROM mute_log WHERE CharID=%u", req->charId);
	}
}

void CChatServerSession::RecvHlsSlotMachineExtractReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sTQ_HLS_SLOT_MACHINE_EXTRACT_REQ * req = (sTQ_HLS_SLOT_MACHINE_EXTRACT_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQT_HLS_SLOT_MACHINE_EXTRACT_RES));
	sQT_HLS_SLOT_MACHINE_EXTRACT_RES * res = (sQT_HLS_SLOT_MACHINE_EXTRACT_RES *)packet.GetPacketData();
	res->wOpCode = QT_HLS_SLOT_MACHINE_EXTRACT_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->accountId = req->accountId;
	res->byExtractCount = req->byExtractCount;
	res->byHlsMachineType = req->byHlsMachineType;
	res->charId = req->charId;
	res->wCoin = req->wCoin;
	res->wMachineIndex = req->machineIndex;

	CAccountCache* pCache = g_pPlayerCache->GetAccount(req->accountId);
	if (pCache)
	{
		CPlayerCache* pCharCache = g_pPlayerCache->GetCharacter(req->charId);
		if (pCharCache)
		{
			if (pCache->GetWaguCoin() >= (DWORD)req->wCoin)
			{
				SYSTEMTIME ti;
				GetLocalTime(&ti);

				for (BYTE i = 0; i < req->byExtractCount; i++)
				{
					res->ItemTblidx[i] = req->aItemTblidx[i];
					res->byStackCount[i] = req->abyStackCount[i];
					res->bySetCount[i] = req->abySetCount[i];
					res->byRanking[i] = req->byRanking[i];

					res->aProductId[i] = g_pCashshopManager->AcquireProductId();

					//create & add item to cache
					sCASHITEM_BRIEF* pBrief = new sCASHITEM_BRIEF;
					pBrief->byStackCount = req->abyStackCount[i];
					pBrief->HLSitemTblidx = req->aItemTblidx[i];
					pBrief->qwProductId = res->aProductId[i];
					pBrief->tRegTime.day = (BYTE)ti.wDay;
					pBrief->tRegTime.hour = (BYTE)ti.wHour;
					pBrief->tRegTime.minute = (BYTE)ti.wMinute;
					pBrief->tRegTime.month = (BYTE)ti.wMonth;
					pBrief->tRegTime.second = (BYTE)ti.wSecond;
					pBrief->tRegTime.year = ti.wYear;
					pCache->InsertCashItem(pBrief);

					GetAccDB.Execute("INSERT INTO cashshop_storage (ProductId,AccountID,HLSitemTblidx,StackCount,year,month,day,hour,minute,second,millisecond,Buyer,price)VALUES(%I64u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u)",
						res->aProductId[i], req->accountId, pBrief->HLSitemTblidx, pBrief->byStackCount, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond, ti.wMilliseconds, req->accountId, 1);
				}

				GetLogDB.Execute("INSERT INTO slot_machine_log (accountid,charid,extractCount,type,coin,currentPoints,newPoints,ProductId1,ProductId2,ProductId3,ProductId4,ProductId5,ProductId6,ProductId7,ProductId8,ProductId9,ProductId10)VALUES(%u,%u,%u,%u,%u,%u,%u,%I64u,%I64u,%I64u,%I64u,%I64u,%I64u,%I64u,%I64u,%I64u,%I64u)",
					req->accountId, req->charId, req->byExtractCount, req->byHlsMachineType, req->wCoin, pCache->GetWaguCoin(), pCache->GetWaguCoin() - (DWORD)req->wCoin, res->aProductId[0], res->aProductId[1], res->aProductId[2], res->aProductId[3], res->aProductId[4], res->aProductId[5], res->aProductId[6], res->aProductId[7], res->aProductId[8], res->aProductId[9]);

				pCache->SetWaguCoin(pCache->GetWaguCoin() - (DWORD)req->wCoin);
				GetAccDB.WaitExecute("UPDATE accounts SET WaguCoins=%u WHERE AccountID=%u", pCache->GetWaguCoin(), req->accountId); //if player bought cash and did not update his cash in game.. This is why we do like this
				
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQT_HLS_SLOT_MACHINE_EXTRACT_RES));
	app->Send(GetHandle(), &packet);
}

