#include "stdafx.h"
#include "ChatServer.h"
#include "NtlChat.h"
#include "Dojo.h"
#include "TableContainerManager.h"
#include "SlotMachineTable.h"
#include "HLSItemTable.h"
#include "Party.h"
#include "GuildManager.h"
#include "NtlRandom.h"
#include "RankBattle.h"
#include "DojoWar.h"
#include "HlsSlotMachine.h"

#include "NtlPacketTU.h"
#include "NtlPacketUT.h"
#include "NtlPacketTG.h"
#include "NtlPacketTQ.h"



DWORD LockGlobalChat(BYTE byLevel)
{
	if (byLevel < 10)
		return 10000; // 10 seconds
	else if (byLevel < 30)
		return 5000; // 5 seconds

	return 1000; // 1 second
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CClientSession::OnInvalid(CNtlPacket * pPacket)
{
}


//--------------------------------------------------------------------------------------//
//		Enter Chat Server
//--------------------------------------------------------------------------------------//
void CClientSession::RecvEnterChat(CNtlPacket * pPacket)
{
	sUT_ENTER_CHAT * req = (sUT_ENTER_CHAT *)pPacket->GetPacketData();

	this->accountId = req->accountId;
	WORD resultcode = CHAT_SUCCESS;

	CPlayer* pPlayer = g_pPlayerManager->FindPlayerWithAccID(req->accountId);

	if (pPlayer)
	{
		if (g_pServerInfoManager->CheckAuthKey(req->abyAuthKey, req->accountId) == true)
		{
			pPlayer->SetClientSessionID(GetHandle());
			pPlayer->SetClientSession(this);

			this->cPlayer = pPlayer;

			cPlayer->LoadFromDB();
		}
		else
		{
			resultcode = COMMUNITY_INVALID_AUTH_KEY;
			ERR_LOG(LOG_USER, "ERROR: Account %u auth-key check failed", req->accountId);
		}
	}
	else
	{
		resultcode = COMMUNITY_ALREADY_IN_SERVER;
		ERR_LOG(LOG_USER, "ERROR: Account %u does not exist", req->accountId);
	}

	CNtlPacket loginP(sizeof(sTU_ENTER_CHAT_RES));
	sTU_ENTER_CHAT_RES * loginRes = (sTU_ENTER_CHAT_RES *)loginP.GetPacketData();
	loginRes->wOpCode = TU_ENTER_CHAT_RES;
	loginRes->wResultCode = resultcode;
	loginP.SetPacketLen(sizeof(sTU_ENTER_CHAT_RES));
	g_pApp->Send(GetHandle(), &loginP);
}

//--------------------------------------------------------------------------------------//
//		Normal Chat
//--------------------------------------------------------------------------------------//
void CClientSession::RecvChatMessageSay(CNtlPacket * pPacket)
{
	sUT_CHAT_MESSAGE_SAY * req = (sUT_CHAT_MESSAGE_SAY *)pPacket->GetPacketData();
	
	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	DWORD dwCurTick = GetTickCount();

	if (dwCurTick < m_dwLastMessage)
		return;

	m_dwLastMessage = dwCurTick + 500;

	WORD wMessageLengthInUnicode = (req->wMessageLengthInUnicode <= NTL_MAX_LENGTH_OF_CHAT_MESSAGE) ? req->wMessageLengthInUnicode : 0;
	if (wMessageLengthInUnicode == 0)
		return;

	if (cPlayer->IsMuted())
		return;

	CNtlPacket packet(sizeof(sTU_CHAT_MESSAGE_SAY));
	sTU_CHAT_MESSAGE_SAY * res = (sTU_CHAT_MESSAGE_SAY *)packet.GetPacketData();
	res->wOpCode = TU_CHAT_MESSAGE_SAY;
	NTL_SAFE_WCSNCPY(res->awchMessage, req->awchMessage, req->wMessageLengthInUnicode);
	res->wMessageLengthInUnicode = req->wMessageLengthInUnicode;
	res->hSubject = cPlayer->GetID();
	NTL_SAFE_WCSCPY(res->awchSenderCharName, cPlayer->GetCharName() );
	packet.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_SAY) );
	g_pPlayerManager->SendMsgToInRange(&packet, cPlayer, NTL_SAY_MESSAGE_RANGE_IN_METER, cPlayer->GetChannel(), cPlayer->GetWorldID(), true);
}

//--------------------------------------------------------------------------------------//
//		Shout Chat
//--------------------------------------------------------------------------------------//
void CClientSession::RecvChatShout(CNtlPacket * pPacket)
{
	sUT_CHAT_MESSAGE_SHOUT * req = (sUT_CHAT_MESSAGE_SHOUT *)pPacket->GetPacketData();

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	DWORD dwCurTick = GetTickCount();

	if (dwCurTick < m_dwLastMessage)
		return;

	m_dwLastMessage = dwCurTick + 500;

	WORD wMessageLengthInUnicode = (req->wMessageLengthInUnicode <= NTL_MAX_LENGTH_OF_CHAT_MESSAGE) ? req->wMessageLengthInUnicode : 0;
	if (wMessageLengthInUnicode == 0)
		return;

	if (cPlayer->IsMuted())
		return;

	CNtlPacket packet(sizeof(sTU_CHAT_MESSAGE_SHOUT));
	sTU_CHAT_MESSAGE_SHOUT * res = (sTU_CHAT_MESSAGE_SHOUT *)packet.GetPacketData();
	res->wOpCode = TU_CHAT_MESSAGE_SHOUT;
	NTL_SAFE_WCSNCPY(res->awchMessage, req->awchMessage, req->wMessageLengthInUnicode);
	res->wMessageLengthInUnicode = req->wMessageLengthInUnicode;
	res->hSubject = cPlayer->GetID();
	NTL_SAFE_WCSCPY(res->awchSenderCharName, cPlayer->GetCharName());
	packet.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_SHOUT) );
	g_pPlayerManager->SendMsgToInRange(&packet, cPlayer, NTL_SHOUT_MESSAGE_RANGE_IN_METER, cPlayer->GetChannel(), cPlayer->GetWorldID(), true);
}

//--------------------------------------------------------------------------------------//
//		whisper Chat
//--------------------------------------------------------------------------------------//
void CClientSession::RecvChatWhisper(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	DWORD dwCurTick = GetTickCount();

	if (dwCurTick < m_dwLastMessage)
		return;

	m_dwLastMessage = dwCurTick + 500;

	sUT_CHAT_MESSAGE_WHISPER * req = (sUT_CHAT_MESSAGE_WHISPER *)pPacket->GetPacketData();

	WORD wMessageLengthInUnicode = (req->wMessageLengthInUnicode <= NTL_MAX_LENGTH_OF_CHAT_MESSAGE) ? req->wMessageLengthInUnicode : 0;
	if (wMessageLengthInUnicode == 0)
		return;

	if (cPlayer->IsMuted())
		return;

	CPlayer* whisperto = g_pPlayerManager->FindPlayerByName( req->awchReceiverCharName );
	if(whisperto && whisperto->IsBlackListed(cPlayer->GetCharID()) == false)
	{
		CNtlPacket packet(sizeof(sTU_CHAT_MESSAGE_WHISPER));
		sTU_CHAT_MESSAGE_WHISPER * res = (sTU_CHAT_MESSAGE_WHISPER *)packet.GetPacketData();
		res->wOpCode = TU_CHAT_MESSAGE_WHISPER;
		NTL_SAFE_WCSNCPY(res->awchMessage, req->awchMessage, req->wMessageLengthInUnicode);
		res->wMessageLengthInUnicode = req->wMessageLengthInUnicode;
		NTL_SAFE_WCSCPY(res->uCharName.wszMessageSenderCharName, cPlayer->GetCharName());
		res->bIsMessageFromYou = false;
		packet.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_WHISPER) );
		g_pApp->Send(whisperto->GetClientSessionID(), &packet);

		CNtlPacket packet2(sizeof(sTU_CHAT_MESSAGE_WHISPER));
		sTU_CHAT_MESSAGE_WHISPER * res2 = (sTU_CHAT_MESSAGE_WHISPER *)packet2.GetPacketData();
		res2->wOpCode = TU_CHAT_MESSAGE_WHISPER;
		NTL_SAFE_WCSNCPY(res2->awchMessage, req->awchMessage, req->wMessageLengthInUnicode);
		res2->wMessageLengthInUnicode = req->wMessageLengthInUnicode;
		NTL_SAFE_WCSCPY(res2->uCharName.wszMessageReceiverCharName, req->awchReceiverCharName );
		res2->bIsMessageFromYou = true;
		packet2.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_WHISPER) );
		app->Send(GetHandle(), &packet2);
	}
	else
	{
		CNtlPacket packet(sizeof(sTU_CHAT_MESSAGE_WHISPER_FAILED_NFY));
		sTU_CHAT_MESSAGE_WHISPER_FAILED_NFY * res = (sTU_CHAT_MESSAGE_WHISPER_FAILED_NFY *)packet.GetPacketData();
		res->wOpCode = TU_CHAT_MESSAGE_WHISPER_FAILED_NFY;
		NTL_SAFE_WCSCPY(res->awchReceiverCharName, req->awchReceiverCharName );
		packet.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_WHISPER_FAILED_NFY) );
		app->Send(GetHandle(), &packet);
	}
}

//--------------------------------------------------------------------------------------//
//		Party Chat
//--------------------------------------------------------------------------------------//
void CClientSession::RecvChatParty(CNtlPacket * pPacket)
{
	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_CHAT_MESSAGE_PARTY * req = (sUT_CHAT_MESSAGE_PARTY *)pPacket->GetPacketData();

	if (cPlayer->GetPartyID() == INVALID_PARTYID)
		return;
	
	if (req->byChattingType > DBO_CHATTING_TYPE_PARTY_NOTICE)
		return;

	DWORD dwCurTick = GetTickCount();

	if (dwCurTick < m_dwLastMessage)
		return;

	m_dwLastMessage = dwCurTick + 500;

	WORD wMessageLengthInUnicode = (req->wMessageLengthInUnicode <= NTL_MAX_LENGTH_OF_CHAT_MESSAGE) ? req->wMessageLengthInUnicode : 0;
	if (wMessageLengthInUnicode == 0)
		return;

	if (cPlayer->IsMuted())
		return;

	CNtlPacket packet(sizeof(sTU_CHAT_MESSAGE_PARTY));
	sTU_CHAT_MESSAGE_PARTY * res = (sTU_CHAT_MESSAGE_PARTY *)packet.GetPacketData();
	res->wOpCode = TU_CHAT_MESSAGE_PARTY;
	NTL_SAFE_WCSNCPY(res->awchMessage, req->awchMessage, req->wMessageLengthInUnicode);
	res->wMessageLengthInUnicode = req->wMessageLengthInUnicode;
	res->hSubject = cPlayer->GetID();
	NTL_SAFE_WCSCPY(res->awchSenderCharName, cPlayer->GetCharName());
	res->byChattingType = req->byChattingType;
	packet.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_PARTY) );
	g_pPartyManager->Broadcast(cPlayer->GetPartyID(), &packet);
}

//--------------------------------------------------------------------------------------//
//		Guild Chat
//--------------------------------------------------------------------------------------//
void CClientSession::RecvChatGuild(CNtlPacket * pPacket)
{
	sUT_CHAT_MESSAGE_GUILD * req = (sUT_CHAT_MESSAGE_GUILD *)pPacket->GetPacketData();

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	if (req->byChattingType != DBO_CHATTING_TYPE_GENERAL && req->byChattingType != DBO_CHATTING_TYPE_GUILD_SCRAMBLE_NOTICE)
		return;

	DWORD dwCurTick = GetTickCount();

	if (dwCurTick < m_dwLastMessage)
		return;

	m_dwLastMessage = dwCurTick + 500;

	WORD wMessageLengthInUnicode = (req->wMessageLengthInUnicode <= NTL_MAX_LENGTH_OF_CHAT_MESSAGE) ? req->wMessageLengthInUnicode : 0;
	if (wMessageLengthInUnicode == 0)
		return;

	if(cPlayer->GetGuild())
	{
		if (cPlayer->IsMuted())
			return;

		CNtlPacket packet(sizeof(sTU_CHAT_MESSAGE_GUILD));
		sTU_CHAT_MESSAGE_GUILD * res = (sTU_CHAT_MESSAGE_GUILD *)packet.GetPacketData();
		res->wOpCode = TU_CHAT_MESSAGE_GUILD;
		NTL_SAFE_WCSNCPY(res->awchMessage, req->awchMessage, req->wMessageLengthInUnicode);
		res->wMessageLengthInUnicode = req->wMessageLengthInUnicode;
		NTL_SAFE_WCSCPY(res->wszSenderCharName, cPlayer->GetCharName());
		res->byChattingType = req->byChattingType;
		packet.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_GUILD) );
		cPlayer->GetGuild()->Broadcast(&packet);
	}
}

//--------------------------------------------------------------------------------------//
//		Trade Chat
//--------------------------------------------------------------------------------------//
void CClientSession::RecvChatTrade(CNtlPacket * pPacket)
{
	sUT_CHAT_MESSAGE_TRADE * req = (sUT_CHAT_MESSAGE_TRADE *)pPacket->GetPacketData();

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	if (cPlayer->GetLevel() < 5)
		return;

	DWORD dwCurTick = GetTickCount();

	if (dwCurTick < m_dwLastGlobalMessage)
		return;

	m_dwLastGlobalMessage = dwCurTick + LockGlobalChat(cPlayer->GetLevel());

	WORD wMessageLengthInUnicode = (req->wMessageLengthInUnicode <= NTL_MAX_LENGTH_OF_CHAT_MESSAGE) ? req->wMessageLengthInUnicode : 0;
	if (wMessageLengthInUnicode == 0)
		return;

	if (cPlayer->IsMuted())
		return;

	if (cPlayer->SpamCheck(req->awchMessage) == false)
		return;

	CNtlPacket packet(sizeof(sTU_CHAT_MESSAGE_TRADE));
	sTU_CHAT_MESSAGE_TRADE * res = (sTU_CHAT_MESSAGE_TRADE *)packet.GetPacketData();
	res->wOpCode = TU_CHAT_MESSAGE_TRADE;
	NTL_SAFE_WCSNCPY(res->awchMessage, req->awchMessage, req->wMessageLengthInUnicode);
	res->wMessageLengthInUnicode = req->wMessageLengthInUnicode;
	res->hSubject = cPlayer->GetID();
	NTL_SAFE_WCSCPY(res->awchSenderCharName, cPlayer->GetCharName());
	res->serverChannelId = cPlayer->GetChannel();
	packet.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_TRADE) );
	g_pPlayerManager->SendMsgToAll(&packet, cPlayer->GetCharID(), true);
}

//--------------------------------------------------------------------------------------//
//		Looking for Group Chat
//--------------------------------------------------------------------------------------//
void CClientSession::RecvChatFindParty(CNtlPacket * pPacket)
{
	sUT_CHAT_MESSAGE_FIND_PARTY * req = (sUT_CHAT_MESSAGE_FIND_PARTY *)pPacket->GetPacketData();

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	if (cPlayer->GetLevel() < 5)
		return;

	DWORD dwCurTick = GetTickCount();

	if (dwCurTick < m_dwLastGlobalMessage)
		return;

	m_dwLastGlobalMessage = dwCurTick + LockGlobalChat(cPlayer->GetLevel());

	WORD wMessageLengthInUnicode = (req->wMessageLengthInUnicode <= NTL_MAX_LENGTH_OF_CHAT_MESSAGE) ? req->wMessageLengthInUnicode : 0;
	if (wMessageLengthInUnicode == 0)
		return;

	if (cPlayer->IsMuted())
		return;

	if (cPlayer->SpamCheck(req->awchMessage) == false)
		return;

	CNtlPacket packet(sizeof(sTU_CHAT_MESSAGE_FIND_PARTY));
	sTU_CHAT_MESSAGE_FIND_PARTY * res = (sTU_CHAT_MESSAGE_FIND_PARTY *)packet.GetPacketData();
	res->wOpCode = TU_CHAT_MESSAGE_FIND_PARTY;
	NTL_SAFE_WCSNCPY(res->awchMessage, req->awchMessage, req->wMessageLengthInUnicode);
	res->wMessageLengthInUnicode = req->wMessageLengthInUnicode;
	res->hSubject = cPlayer->GetID();
	NTL_SAFE_WCSCPY(res->awchSenderCharName, cPlayer->GetCharName());
	res->serverChannelId = cPlayer->GetChannel();
	packet.SetPacketLen( sizeof(sTU_CHAT_MESSAGE_FIND_PARTY) );
	g_pPlayerManager->SendMsgToAll(&packet, cPlayer->GetCharID(), true);
}

void CClientSession::RecvChatUserOnChannelUpdateNfy(CNtlPacket * pPacket)
{
	sUT_CHAT_USER_ON_CHANNEL_UPDATE_NFY * req = (sUT_CHAT_USER_ON_CHANNEL_UPDATE_NFY *)pPacket->GetPacketData();
	//printf("sUT_CHAT_USER_ON_CHANNEL_UPDATE_NFY: dwUserOnChannelFlag %u \n", req->dwUserOnChannelFlag);

	if (cPlayer)
	{
		cPlayer->LoadFromDB();
	}
}

//--------------------------------------------------------------------------------------//
//		Disband Guild
//--------------------------------------------------------------------------------------//
void CClientSession::RecvDisbandGuildReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	WORD resultcode = CHAT_SUCCESS;

	if (CGuild* pGuild = cPlayer->GetGuild())
	{
		if (pGuild->GetDojo() && pGuild->GetDojo()->GetDojoWarStatus() != eDBO_DOJO_STATUS_NORMAL)
			resultcode = COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE;
		else if (g_pDojoWarManager->GetDojo(cPlayer->GetGuildID()))
			resultcode = COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE;
		else if (pGuild->IsGuildBankInUse() == false)
		{
			if (pGuild->IsGuildMaster(cPlayer->GetCharID()))
			{
				CNtlPacket packet2(sizeof(sTQ_GUILD_DISBAND_REQ));
				sTQ_GUILD_DISBAND_REQ * res2 = (sTQ_GUILD_DISBAND_REQ *)packet2.GetPacketData();
				res2->wOpCode = TQ_GUILD_DISBAND_REQ;
				res2->guildId = cPlayer->GetGuildID();
				res2->bByUser = true;
				packet2.SetPacketLen(sizeof(sTQ_GUILD_DISBAND_REQ));
				app->SendTo(app->GetQueryServerSession(), &packet2);

				return;
			}
			else resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
		}
		else resultcode = COMMUNITY_GUILD_BANK_USING_NOW;
	}
	else resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;

	CNtlPacket packet(sizeof(sTU_GUILD_DISBAND_RES));
	sTU_GUILD_DISBAND_RES * res = (sTU_GUILD_DISBAND_RES *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_DISBAND_RES;
	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sTU_GUILD_DISBAND_RES));
	app->Send(GetHandle(), &packet);
}
//--------------------------------------------------------------------------------------//
//		Guild invitation response
//--------------------------------------------------------------------------------------//
void CClientSession::RecvGuildResponseInv(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_GUILD_RESPONSE_INVITATION * req = (sUT_GUILD_RESPONSE_INVITATION *)pPacket->GetPacketData();

	CPlayer* invitor = NULL;

	CNtlPacket packet(sizeof(sTU_GUILD_RESPONSE_INVITATION_RES));
	sTU_GUILD_RESPONSE_INVITATION_RES * res = (sTU_GUILD_RESPONSE_INVITATION_RES *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_RESPONSE_INVITATION_RES;
	res->wResultCode = CHAT_SUCCESS;

	if (cPlayer->GetGuildID() > 0 || cPlayer->GetGuild() != NULL)
		res->wResultCode = COMMUNITY_GUILD_YOU_ARE_ALREADY_IN_A_GUILD;
	else if (cPlayer->GetGuildInvitedByID() == INVALID_CHARACTERID)
		res->wResultCode = COMMUNITY_GUILD_YOU_HAVE_NO_INVITATION;
	else
	{
		invitor = g_pPlayerManager->FindPlayerWithCharID(cPlayer->GetGuildInvitedByID());

		if (invitor == NULL || !invitor->GetPcInitState())
			res->wResultCode = COMMUNITY_FAIL;
		else if(invitor->GetGuild() == NULL)
			res->wResultCode = COMMUNITY_FAIL;
		else if (invitor->GetGuild()->GetTotalMemberCount() >= Dbo_GetMaxGuildMemberCount(invitor->GetGuild()->GetInfo()->qwGuildFunctionFlag))
			res->wResultCode = COMMUNITY_GUILD_MAX_NUMBER_OF_MEMBERS;
		else if (invitor->GetGuild()->GetDojo() && invitor->GetGuild()->GetDojo()->GetDojoWarStatus() != eDBO_DOJO_STATUS_NORMAL)
			res->wResultCode = COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE;
		else if (g_pDojoWarManager->GetDojo(invitor->GetGuildID()))
			res->wResultCode = COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE;
	}

	packet.SetPacketLen(sizeof(sTU_GUILD_RESPONSE_INVITATION_RES));
	app->Send(GetHandle(), &packet);

	if (res->wResultCode == CHAT_SUCCESS)
	{
		if (req->byResponse == NTL_INVITATION_RESPONSE_ACCEPT)
		{
			CNtlPacket packet2(sizeof(sTQ_GUILD_INVITE_REQ));
			sTQ_GUILD_INVITE_REQ * res2 = (sTQ_GUILD_INVITE_REQ *)packet2.GetPacketData();
			res2->wOpCode = TQ_GUILD_INVITE_REQ;
			res2->guildId = invitor->GetGuildID();
			res2->inviteeCharId = cPlayer->GetCharID();
			packet2.SetPacketLen(sizeof(sTQ_GUILD_INVITE_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet2);
		}
		else if (req->byResponse == NTL_INVITATION_RESPONSE_DECLINE)
		{
			CNtlPacket packet2(sizeof(sTU_GUILD_INVITATION_DECLINED_NFY));
			sTU_GUILD_INVITATION_DECLINED_NFY * res2 = (sTU_GUILD_INVITATION_DECLINED_NFY *)packet2.GetPacketData();
			res2->wOpCode = TU_GUILD_INVITATION_DECLINED_NFY;
			NTL_SAFE_WCSCPY(res2->wszTargetName, cPlayer->GetCharName());
			packet2.SetPacketLen(sizeof(sTU_GUILD_INVITATION_DECLINED_NFY));
			g_pApp->Send(invitor->GetClientSessionID(), &packet2);
		}
		else // NTL_INVITATION_RESPONSE_EXPIRE
		{
			CNtlPacket packet2(sizeof(sTU_GUILD_INVITATION_EXPIRED_NFY));
			sTU_GUILD_INVITATION_EXPIRED_NFY * res2 = (sTU_GUILD_INVITATION_EXPIRED_NFY *)packet2.GetPacketData();
			res2->wOpCode = TU_GUILD_INVITATION_EXPIRED_NFY;
			NTL_SAFE_WCSCPY(res2->wszTargetName, cPlayer->GetCharName());
			packet2.SetPacketLen(sizeof(sTU_GUILD_INVITATION_EXPIRED_NFY));
			g_pApp->Send(invitor->GetClientSessionID(), &packet2);
		}
	}

	cPlayer->SetGuildInvitedByID(INVALID_CHARACTERID);
}

//--------------------------------------------------------------------------------------//
//		Change guild notice
//--------------------------------------------------------------------------------------//
void CClientSession::RecvGuildChangeNoticeReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_GUILD_CHANGE_NOTICE_REQ * req = (sUT_GUILD_CHANGE_NOTICE_REQ *)pPacket->GetPacketData();

	if (cPlayer->GetGuild())
	{
		CNtlPacket packet2(sizeof(sTQ_GUILD_CHANGE_NOTICE_REQ));
		sTQ_GUILD_CHANGE_NOTICE_REQ * res2 = (sTQ_GUILD_CHANGE_NOTICE_REQ *)packet2.GetPacketData();
		res2->wOpCode = TQ_GUILD_CHANGE_NOTICE_REQ;
		res2->charId = cPlayer->GetCharID();
		res2->guildId = cPlayer->GetGuildID();
		NTL_SAFE_WCSCPY(res2->awchCharName, cPlayer->GetCharName());
		res2->wNoticeLengthInUnicode = req->wNoticeLengthInUnicode;
		NTL_SAFE_WCSCPY(res2->awchNotice, req->awchNotice);
		packet2.SetPacketLen(sizeof(sTQ_GUILD_CHANGE_NOTICE_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet2);
	}
	else
	{
		CNtlPacket packet(sizeof(sTU_GUILD_CHANGE_NOTICE_RES));
		sTU_GUILD_CHANGE_NOTICE_RES * res = (sTU_GUILD_CHANGE_NOTICE_RES *)packet.GetPacketData();
		res->wOpCode = TU_GUILD_CHANGE_NOTICE_RES;
		res->wResultCode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;
		packet.SetPacketLen(sizeof(sTU_GUILD_CHANGE_NOTICE_RES));
		g_pApp->SendTo(this, &packet);
	}
}

//--------------------------------------------------------------------------------------//
//		Leave Guild
//--------------------------------------------------------------------------------------//
void CClientSession::RecvLeaveGuildReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	WORD resultcode = CHAT_SUCCESS;

	CGuild* guild = cPlayer->GetGuild();

	if (guild == NULL)
		resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;
	else if (guild->IsGuildMaster(cPlayer->GetCharID()))
		resultcode = COMMUNITY_GUILD_LEADER_CANT_LEAVE;
	else if(guild->GetGuildBankUser() == cPlayer->GetCharID())
		resultcode = COMMUNITY_GUILD_BANK_USING_NOW;
	else if (guild->GetDojo() && guild->GetDojo()->GetDojoWarStatus() != eDBO_DOJO_STATUS_NORMAL)
		resultcode = COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE;
	else if (g_pDojoWarManager->GetDojo(cPlayer->GetGuildID()))
		resultcode = COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE;
	else
	{
		CNtlPacket packet(sizeof(sTQ_GUILD_LEAVE_REQ));
		sTQ_GUILD_LEAVE_REQ * res = (sTQ_GUILD_LEAVE_REQ *)packet.GetPacketData();
		res->wOpCode = TQ_GUILD_LEAVE_REQ;
		res->leavingMemberCharId = cPlayer->GetCharID();
		packet.SetPacketLen(sizeof(sTQ_GUILD_LEAVE_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);

		return;
	}

	CNtlPacket packet(sizeof(sTU_GUILD_LEAVE_RES));
	sTU_GUILD_LEAVE_RES * res = (sTU_GUILD_LEAVE_RES *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_LEAVE_RES;
	res->wResultCode = resultcode;
	packet.SetPacketLen( sizeof(sTU_GUILD_LEAVE_RES));
	app->Send(GetHandle(), &packet);
}
//--------------------------------------------------------------------------------------//
//		Kick from Guild
//--------------------------------------------------------------------------------------//
void CClientSession::RecvKickFromGuildReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_GUILD_KICK_OUT_REQ * req = (sUT_GUILD_KICK_OUT_REQ *)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (CGuild* pGuild = cPlayer->GetGuild())
	{
		if (!pGuild->IsGuildMaster(cPlayer->GetCharID()))
			resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
		else if (pGuild->GetMemberInfo(req->targetMemberCharId) == NULL)
			resultcode = COMMUNITY_COMMON_TARGET_IS_NOT_YOUR_GUILD_MEMBER;
		else if (cPlayer->GetCharID() == req->targetMemberCharId)
			resultcode = COMMUNITY_GUILD_CANT_KICK_YOURSELF_OUT;
		else if(pGuild->GetDojo() && pGuild->GetDojo()->GetDojoWarStatus() != eDBO_DOJO_STATUS_NORMAL)
			resultcode = COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE;
		else if(g_pDojoWarManager->GetDojo(cPlayer->GetGuildID()))
			resultcode = COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE;
		else
		{
			CNtlPacket packet(sizeof(sTQ_GUILD_KICK_OUT_REQ));
			sTQ_GUILD_KICK_OUT_REQ * res = (sTQ_GUILD_KICK_OUT_REQ *)packet.GetPacketData();
			res->wOpCode = TQ_GUILD_KICK_OUT_REQ;
			res->kickedOutMemberCharId = req->targetMemberCharId;
			res->guildId = cPlayer->GetGuildID();
			packet.SetPacketLen(sizeof(sTQ_GUILD_KICK_OUT_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);

			return;
		}
	}
	else resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;


	CNtlPacket packet(sizeof(sTU_GUILD_KICK_OUT_RES));
	sTU_GUILD_KICK_OUT_RES * res = (sTU_GUILD_KICK_OUT_RES *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_KICK_OUT_RES;
	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sTU_GUILD_KICK_OUT_RES));
	g_pApp->Send(GetHandle(), &packet);
}
//--------------------------------------------------------------------------------------//
//		Appoint second guildmaster
//--------------------------------------------------------------------------------------//
void CClientSession::RecvNewSecondGuildMasterReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_GUILD_APPOINT_SECOND_MASTER_REQ * req = (sUT_GUILD_APPOINT_SECOND_MASTER_REQ *)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (CGuild* pGuild = cPlayer->GetGuild())
	{
		if (!pGuild->IsGuildMaster(cPlayer->GetCharID()))
			resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
		else if (pGuild->GetMemberInfo(req->targetMemberCharId) == NULL)
			resultcode = COMMUNITY_COMMON_TARGET_IS_NOT_YOUR_GUILD_MEMBER;
		else if (cPlayer->GetCharID() == req->targetMemberCharId)
			resultcode = COMMUNITY_GUILD_TRIED_TO_APPOINT_YOURSELF;
		else if (pGuild->CanAppointSecondMaster(resultcode, req->targetMemberCharId) == false)
			;
		else
		{
			CNtlPacket packet(sizeof(sTQ_GUILD_APPOINT_SECOND_MASTER_REQ));
			sTQ_GUILD_APPOINT_SECOND_MASTER_REQ * res = (sTQ_GUILD_APPOINT_SECOND_MASTER_REQ *)packet.GetPacketData();
			res->wOpCode = TQ_GUILD_APPOINT_SECOND_MASTER_REQ;
			res->targetMemberCharId = req->targetMemberCharId;
			res->guildId = cPlayer->GetGuildID();
			packet.SetPacketLen(sizeof(sTQ_GUILD_APPOINT_SECOND_MASTER_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);

			return;
		}
	}
	else resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;


	CNtlPacket packet(sizeof(sTU_GUILD_APPOINT_SECOND_MASTER_RES));
	sTU_GUILD_APPOINT_SECOND_MASTER_RES * res = (sTU_GUILD_APPOINT_SECOND_MASTER_RES *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_APPOINT_SECOND_MASTER_RES;
	res->wResultCode = resultcode;
	res->secondMasterCharId = req->targetMemberCharId;
	packet.SetPacketLen(sizeof(sTU_GUILD_APPOINT_SECOND_MASTER_RES));
	app->Send(GetHandle(), &packet);
}
//--------------------------------------------------------------------------------------//
//		Remove second guild master
//--------------------------------------------------------------------------------------//
void CClientSession::RecvRemoveSecondGuildMasterReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_GUILD_DISMISS_SECOND_MASTER_REQ * req = (sUT_GUILD_DISMISS_SECOND_MASTER_REQ *)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (CGuild* pGuild = cPlayer->GetGuild())
	{
		if (!pGuild->IsGuildMaster(cPlayer->GetCharID()))
			resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
		else if (pGuild->GetMemberInfo(req->targetMemberCharId) == NULL)
			resultcode = COMMUNITY_COMMON_TARGET_IS_NOT_YOUR_GUILD_MEMBER;
		else if(pGuild->IsSecondGuildMaster(req->targetMemberCharId) == INVALID_BYTE)
			resultcode = COMMUNITY_GUILD_TARGET_IS_NOT_SECOND_MASTER;
		else
		{
			CNtlPacket packet(sizeof(sTQ_GUILD_DISMISS_SECOND_MASTER_REQ));
			sTQ_GUILD_DISMISS_SECOND_MASTER_REQ * res = (sTQ_GUILD_DISMISS_SECOND_MASTER_REQ *)packet.GetPacketData();
			res->wOpCode = TQ_GUILD_DISMISS_SECOND_MASTER_REQ;
			res->targetMemberCharId = req->targetMemberCharId;
			res->guildId = cPlayer->GetGuildID();
			packet.SetPacketLen(sizeof(sTQ_GUILD_DISMISS_SECOND_MASTER_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);

			return;
		}
	}
	else resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;


	CNtlPacket packet(sizeof(sTU_GUILD_DISMISS_SECOND_MASTER_RES));
	sTU_GUILD_DISMISS_SECOND_MASTER_RES * res = (sTU_GUILD_DISMISS_SECOND_MASTER_RES *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_DISMISS_SECOND_MASTER_RES;
	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sTU_GUILD_DISMISS_SECOND_MASTER_RES));
	app->Send(GetHandle(), &packet);
}
//--------------------------------------------------------------------------------------//
//		Change Guild Master
//--------------------------------------------------------------------------------------//
void CClientSession::RecvUpdateGuildMasterReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_GUILD_CHANGE_GUILD_MASTER_REQ * req = (sUT_GUILD_CHANGE_GUILD_MASTER_REQ *)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (CGuild* pGuild = cPlayer->GetGuild())
	{
		if (!pGuild->IsGuildMaster(cPlayer->GetCharID()))
			resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
		else if (pGuild->GetMemberInfo(req->targetMemberCharId) == NULL)
			resultcode = COMMUNITY_COMMON_TARGET_IS_NOT_YOUR_GUILD_MEMBER;
		else if (!pGuild->IsSecondGuildMaster(req->targetMemberCharId))
			resultcode = COMMUNITY_GUILD_TARGET_IS_ALREADY_SECOND_MASTER;
		else
		{
			CNtlPacket packet(sizeof(sTQ_GUILD_CHANGE_GUILD_MASTER_REQ));
			sTQ_GUILD_CHANGE_GUILD_MASTER_REQ * res = (sTQ_GUILD_CHANGE_GUILD_MASTER_REQ *)packet.GetPacketData();
			res->wOpCode = TQ_GUILD_CHANGE_GUILD_MASTER_REQ;
			res->targetMemberCharId = req->targetMemberCharId;
			res->guildId = cPlayer->GetGuildID();
			packet.SetPacketLen(sizeof(sTQ_GUILD_CHANGE_GUILD_MASTER_REQ));
			app->SendTo(app->GetQueryServerSession(), &packet);

			return;
		}
	}
	else resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;

	CNtlPacket packet(sizeof(sTU_GUILD_CHANGE_GUILD_MASTER_RES));
	sTU_GUILD_CHANGE_GUILD_MASTER_RES * res = (sTU_GUILD_CHANGE_GUILD_MASTER_RES *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_CHANGE_GUILD_MASTER_RES;
	res->wResultCode = resultcode;
	res->newMasterCharId = req->targetMemberCharId;
	packet.SetPacketLen(sizeof(sTU_GUILD_CHANGE_GUILD_MASTER_RES));
	app->Send(GetHandle(), &packet);
}

void CClientSession::RecvBudokaiTournamentIndividualListReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ* req = (sUT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ));
	sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ* res = (sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ*)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ;
	res->charId = cPlayer->GetCharID();
	res->byReceivedListCount = req->byReceivedListCount;
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ));
	app->Send(g_pServerInfoManager->GetGsSession(DOJO_CHANNEL_INDEX), &packet);
}

void CClientSession::RecvBudokaiTournamentIndividualInfoReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ* req = (sUT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ));
	sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ* res = (sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ*)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ;
	res->charId = cPlayer->GetCharID();
	res->awJoinId[0] = req->awJoinId[0];
	res->awJoinId[1] = req->awJoinId[1];
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ));
	app->Send(g_pServerInfoManager->GetGsSession(DOJO_CHANNEL_INDEX), &packet);
}

void CClientSession::RecvBudokaiTournamentTeamListReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ* req = (sUT_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ));
	sTG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ* res = (sTG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ*)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ;
	res->charId = cPlayer->GetCharID();
	res->byReceivedListCount = req->byReceivedListCount;
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ));
	app->Send(g_pServerInfoManager->GetGsSession(DOJO_CHANNEL_INDEX), &packet);
}

void CClientSession::RecvBudokaiTournamentTeamInfoReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ* req = (sUT_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sTG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ));
	sTG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ* res = (sTG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ*)packet.GetPacketData();
	res->wOpCode = TG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ;
	res->charId = cPlayer->GetCharID();
	res->awJoinId[BUDOKAI_MATCH_TYPE_INDIVIDIAUL] = req->awJoinId[BUDOKAI_MATCH_TYPE_INDIVIDIAUL];
	res->awJoinId[BUDOKAI_MATCH_TYPE_TEAM] = req->awJoinId[BUDOKAI_MATCH_TYPE_TEAM];
	packet.SetPacketLen(sizeof(sTG_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ));
	app->Send(g_pServerInfoManager->GetGsSession(DOJO_CHANNEL_INDEX), &packet);
}

//---------------------------------------------
//ADD FRIEND
//---------------------------------------------
void CClientSession::RecvAddFriendReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_FRIEND_ADD_REQ* req = (sUT_FRIEND_ADD_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;
	CHARACTERID charid = INVALID_CHARACTERID;

	if (_wcsicmp(req->wchName, cPlayer->GetCharName()) == 0)
		resultcode = COMMUNITY_FRIEND_SELF_FAIL;
	
	else if (cPlayer->GetFriendData(req->wchName))
		resultcode = COMMUNITY_FRIEND_CHAR_ARLEADY_ADDED;

	else if (cPlayer->GetFriendsCount() >= NTL_MAX_COUNT_FRIEND)
		resultcode = COMMUNITY_FRIEND_MAX_COUNT_OVER;

	if (resultcode == CHAT_SUCCESS)
	{
		char* target = Ntl_WC2MB(req->wchName);
		std::string escapeTarget = GetCharDB.EscapeString(target);
		smart_ptr<QueryResult> result = GetCharDB.Query("SELECT CharID FROM characters WHERE CharName=\"%s\"", escapeTarget.c_str());
		if (result)
		{
			Field* f = result->Fetch();
			charid = f[0].GetUInt32();

			GetCharDB.Execute("INSERT INTO friendlist (user_id,friend_id,friend_name)VALUES(%u,%u,\"%s\")", cPlayer->GetCharID(), charid, escapeTarget.c_str());

			sFRIEND_FULL_INFO friendinfo;
			friendinfo.bIsBlack = false;
			friendinfo.charID = charid;
			NTL_SAFE_WCSCPY(friendinfo.wchName, req->wchName);
			cPlayer->AddFriend(&friendinfo);
		}
		else resultcode = COMMUNITY_FRIEND_CHAR_NOT_FOUND;

		Ntl_CleanUpHeapString(target);
	}

	CNtlPacket packet(sizeof(sTU_FRIEND_ADD_RES));
	sTU_FRIEND_ADD_RES* res = (sTU_FRIEND_ADD_RES*)packet.GetPacketData();
	res->wOpCode = TU_FRIEND_ADD_RES;
	res->wResultCode = resultcode;
	res->targetID = charid;
	NTL_SAFE_WCSCPY(res->wchName, req->wchName);
	packet.SetPacketLen(sizeof(sTU_FRIEND_ADD_RES));
	app->Send(GetHandle(), &packet);

	//send friend info to me
	if (resultcode == CHAT_SUCCESS)
	{
		CPlayer* target = g_pPlayerManager->FindPlayerWithCharID(charid);
		if (target)
		{
			CNtlPacket pFriendInfo(sizeof(sTU_FRIEND_INFO_CHANGE));
			sTU_FRIEND_INFO_CHANGE* rFriendInfo = (sTU_FRIEND_INFO_CHANGE*)pFriendInfo.GetPacketData();
			rFriendInfo->wOpCode = TU_FRIEND_INFO_CHANGE;
			rFriendInfo->targetID = charid;
			rFriendInfo->byChangeType = eFRIEND_CHANGE_TYPE_OFFLINE;
			rFriendInfo->dwChangeValue = 2;
			pFriendInfo.SetPacketLen(sizeof(sTU_FRIEND_INFO_CHANGE));
			g_pApp->Send(GetHandle(), &pFriendInfo);

			CNtlPacket pFriendInfo2(sizeof(sTU_FRIEND_INFO));
			sTU_FRIEND_INFO* rFriendInfo2 = (sTU_FRIEND_INFO*)pFriendInfo2.GetPacketData();
			rFriendInfo2->wOpCode = TU_FRIEND_INFO;
			rFriendInfo2->bIsFirst = false;
			rFriendInfo2->sInfo.byChannel = target->GetChannel();
			rFriendInfo2->sInfo.byClass = target->GetClass();
			rFriendInfo2->sInfo.byLevel = target->GetLevel();
			rFriendInfo2->sInfo.charID = charid;
			rFriendInfo2->sInfo.hHandle = target->GetID();
			rFriendInfo2->sInfo.mapNameTblidx = target->GetMapNameTblidx();
			pFriendInfo2.SetPacketLen(sizeof(sTU_FRIEND_INFO));
			g_pApp->Send(GetHandle(), &pFriendInfo2);
		}
	}
}
//---------------------------------------------
//DELETE FRIEND
//---------------------------------------------
void CClientSession::RecvDelFriendReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_FRIEND_DEL_REQ* req = (sUT_FRIEND_DEL_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (cPlayer->GetFriendData(req->targetID) == NULL)
		resultcode = COMMUNITY_FRIEND_CHAR_NOT_FOUND;

	if (resultcode == CHAT_SUCCESS)
	{
		GetCharDB.Execute("DELETE FROM friendlist WHERE user_id=%u AND friend_id=%u", cPlayer->GetCharID(), req->targetID);
		cPlayer->DelFriend(req->targetID);
	}


	CNtlPacket packet(sizeof(sTU_FRIEND_DEL_RES));
	sTU_FRIEND_DEL_RES* res = (sTU_FRIEND_DEL_RES*)packet.GetPacketData();
	res->wOpCode = TU_FRIEND_DEL_RES;
	res->wResultCode = resultcode;
	res->targetID = req->targetID;
	NTL_SAFE_WCSCPY(res->wchName, req->wchName);
	packet.SetPacketLen(sizeof(sTU_FRIEND_DEL_RES));
	app->Send(GetHandle(), &packet);
}
//----------------------------------------------------------------------
//MOVE FRIEND FROM FRIENDLIST TO BLACKLIST
//----------------------------------------------------------------------
void CClientSession::RecvMoveFriendReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_FRIEND_MOVE_REQ* req = (sUT_FRIEND_MOVE_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;
	sFRIEND_FULL_INFO* finfo = cPlayer->GetFriendData(req->targetID);

	if (finfo)
	{
		if (finfo->bIsBlack == true)
			resultcode = COMMUNITY_FRIEND_BLACK_CHAR_EXIST;
		else if(cPlayer->GetBlacklistCount() >= NTL_MAX_COUNT_FRIEND)
			resultcode = COMMUNITY_FRIEND_MAX_COUNT_OVER;
	}
	else resultcode = COMMUNITY_FRIEND_CHAR_NOT_FOUND;

	if (resultcode == CHAT_SUCCESS)
	{
		GetCharDB.Execute("UPDATE friendlist SET blacklist=1 WHERE user_id=%u AND friend_id=%u", cPlayer->GetCharID(), req->targetID);
		finfo->bIsBlack = true;
	}

	CNtlPacket packet(sizeof(sTU_FRIEND_MOVE_RES));
	sTU_FRIEND_MOVE_RES* res = (sTU_FRIEND_MOVE_RES*)packet.GetPacketData();
	res->wOpCode = TU_FRIEND_MOVE_RES;
	res->wResultCode = resultcode;
	res->targetID = req->targetID;
	NTL_SAFE_WCSCPY(res->wchName, req->wchName);
	packet.SetPacketLen(sizeof(sTU_FRIEND_MOVE_RES));
	app->Send(GetHandle(), &packet);
}
//---------------------------------------------
//ADD USER TO BLACKLIST
//---------------------------------------------
void CClientSession::RecvBlackListAddReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_FRIEND_BLACK_ADD_REQ* req = (sUT_FRIEND_BLACK_ADD_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;
	CHARACTERID charid = INVALID_CHARACTERID;

	if (_wcsicmp(req->awchName, cPlayer->GetCharName()) == 0)
		resultcode = COMMUNITY_FRIEND_SELF_FAIL;

	else if (cPlayer->GetFriendData(req->awchName))
		resultcode = COMMUNITY_FRIEND_CHAR_ARLEADY_ADDED;

	else if (cPlayer->GetBlacklistCount() >= NTL_MAX_COUNT_FRIEND)
		resultcode = COMMUNITY_FRIEND_MAX_COUNT_OVER;

	else
	{
		char* target = Ntl_WC2MB(req->awchName);
		std::string escapeTarget = GetCharDB.EscapeString(target);

		smart_ptr<QueryResult> result = GetCharDB.Query("SELECT CharID FROM characters WHERE CharName=\"%s\"", escapeTarget.c_str());
		if (result)
		{
			Field* f = result->Fetch();
			charid = f[0].GetUInt32();

			GetCharDB.Execute("INSERT INTO friendlist (user_id,friend_id,blacklist,friend_name)VALUES(%u,%u,%i,\"%s\")", cPlayer->GetCharID(), charid, TRUE, escapeTarget.c_str());

			sFRIEND_FULL_INFO friendinfo;
			friendinfo.bIsBlack = true;
			friendinfo.charID = charid;
			NTL_SAFE_WCSCPY(friendinfo.wchName, req->awchName);
			cPlayer->AddFriend(&friendinfo);
		}
		else resultcode = COMMUNITY_FRIEND_CHAR_NOT_FOUND;

		Ntl_CleanUpHeapString(target);
	}

	CNtlPacket packet(sizeof(sTU_FRIEND_BLACK_ADD_RES));
	sTU_FRIEND_BLACK_ADD_RES* res = (sTU_FRIEND_BLACK_ADD_RES*)packet.GetPacketData();

	res->wOpCode = TU_FRIEND_BLACK_ADD_RES;
	res->wResultCode = resultcode;
	res->targetID = charid;
	NTL_SAFE_WCSCPY(res->wchName, req->awchName);
	packet.SetPacketLen(sizeof(sTU_FRIEND_BLACK_ADD_RES));
	app->SendTo(this, &packet);
}
//---------------------------------------------
//DELETE FROM BLACKLIST
//---------------------------------------------
void CClientSession::RecvBlackListDelReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_FRIEND_BLACK_DEL_REQ* req = (sUT_FRIEND_BLACK_DEL_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (cPlayer->GetFriendData(req->targetID) == NULL)
		resultcode = COMMUNITY_FRIEND_CHAR_NOT_FOUND;

	else
	{
		GetCharDB.Execute("DELETE FROM friendlist WHERE user_id=%u AND friend_id=%u", cPlayer->GetCharID(), req->targetID);
		cPlayer->DelFriend(req->targetID);
	}

	CNtlPacket packet(sizeof(sTU_FRIEND_BLACK_DEL_RES));
	sTU_FRIEND_BLACK_DEL_RES* res = (sTU_FRIEND_BLACK_DEL_RES*)packet.GetPacketData();
	res->wOpCode = TU_FRIEND_BLACK_DEL_RES;
	res->wResultCode = resultcode;
	res->targetID = req->targetID;
	NTL_SAFE_WCSCPY(res->wchName, req->wchName);
	packet.SetPacketLen(sizeof(sTU_FRIEND_BLACK_DEL_RES));
	app->SendTo(this, &packet);
}


void CClientSession::RecvRankBattleRankListReq(CNtlPacket * pPacket)
{
	sUT_RANKBATTLE_RANK_LIST_REQ* req = (sUT_RANKBATTLE_RANK_LIST_REQ*)pPacket->GetPacketData();

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	g_pRankBattle->LoadRankBattleList(GetHandle(), req->dwPage, req->byCompareDay);
}


void CClientSession::RecvRankBattleRankFindCharacterReq(CNtlPacket * pPacket)
{
	sUT_RANKBATTLE_RANK_FIND_CHARACTER_REQ* req = (sUT_RANKBATTLE_RANK_FIND_CHARACTER_REQ*)pPacket->GetPacketData();

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	g_pRankBattle->LoadRankBattleListFindName(GetHandle(), req->wszCharName, req->dwPage, req->byCompareDay);
}


void CClientSession::RecvRankBattleRankCompareDayReq(CNtlPacket * pPacket)
{
	sUT_RANKBATTLE_RANK_COMPARE_DAY_REQ* req = (sUT_RANKBATTLE_RANK_COMPARE_DAY_REQ*)pPacket->GetPacketData();

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	CNtlPacket packet(sizeof(sTU_RANKBATTLE_RANK_COMPARE_DAY_RES));
	sTU_RANKBATTLE_RANK_COMPARE_DAY_RES * res = (sTU_RANKBATTLE_RANK_COMPARE_DAY_RES *)packet.GetPacketData();
	res->wOpCode = TU_RANKBATTLE_RANK_COMPARE_DAY_RES;
	res->wResultCode = COMMUNITY_FAIL;
	g_pApp->Send(GetHandle(), &packet);
}


void CClientSession::RecvDojoBudokaiSeedAddReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_DOJO_BUDOKAI_SEED_ADD_REQ* req = (sUT_DOJO_BUDOKAI_SEED_ADD_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (cPlayer->GetGuild())
	{
		CGuild* guild = cPlayer->GetGuild();
		if (guild->IsGuildMaster(cPlayer->GetCharID()))
		{
			CDojo* dojo = guild->GetDojo();
			if (dojo)
			{
				if (guild->GetMemberInfo(req->wszCharName))
				{
					if (_wcsicmp(dojo->GetDojoData()->wszSeedCharName, req->wszCharName) != 0)
					{
						CNtlPacket packet2(sizeof(sTQ_DOJO_BUDOKAI_SEED_ADD_REQ));
						sTQ_DOJO_BUDOKAI_SEED_ADD_REQ * res2 = (sTQ_DOJO_BUDOKAI_SEED_ADD_REQ *)packet2.GetPacketData();
						res2->wOpCode = TQ_DOJO_BUDOKAI_SEED_ADD_REQ;
						res2->charId = cPlayer->GetCharID();
						res2->dojoTblidx = dojo->GetDojoData()->dojoTblidx;
						NTL_SAFE_WCSCPY(res2->wszCharName, req->wszCharName);
						packet2.SetPacketLen(sizeof(sTQ_DOJO_BUDOKAI_SEED_ADD_REQ));
						app->SendTo(app->GetQueryServerSession(), &packet2);

						return;
					}
					else resultcode = COMMUNITY_DOJO_SEED_ALREADY_SETTING;
				}
				else resultcode = COMMUNITY_DOJO_SEED_NAME_FAIL;
			}
			else resultcode = COMMUNITY_DOJO_YOU_ARE_NOT_OWNNER;
		}
		else resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
	}
	else resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;

	CNtlPacket packet(sizeof(sTU_DOJO_BUDOKAI_SEED_ADD_RES));
	sTU_DOJO_BUDOKAI_SEED_ADD_RES* res = (sTU_DOJO_BUDOKAI_SEED_ADD_RES*)packet.GetPacketData();
	res->wOpCode = TU_DOJO_BUDOKAI_SEED_ADD_RES;
	res->wResultCode = resultcode;
	NTL_SAFE_WCSCPY(res->wszCharName, req->wszCharName);
	packet.SetPacketLen(sizeof(sTU_DOJO_BUDOKAI_SEED_ADD_RES));
	app->Send(GetHandle(), &packet);
}

void CClientSession::RecvDojoBudokaiSeedDelReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_DOJO_BUDOKAI_SEED_DEL_REQ* req = (sUT_DOJO_BUDOKAI_SEED_DEL_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (cPlayer->GetGuild())
	{
		CGuild* guild = cPlayer->GetGuild();
		if (guild->IsGuildMaster(cPlayer->GetCharID()))
		{
			CDojo* dojo = guild->GetDojo();
			if (dojo)
			{
				if (_wcsicmp(dojo->GetDojoData()->wszSeedCharName, req->wszCharName) == 0)
				{
					CNtlPacket packet2(sizeof(sTQ_DOJO_BUDOKAI_SEED_DEL_REQ));
					sTQ_DOJO_BUDOKAI_SEED_DEL_REQ * res2 = (sTQ_DOJO_BUDOKAI_SEED_DEL_REQ *)packet2.GetPacketData();
					res2->wOpCode = TQ_DOJO_BUDOKAI_SEED_DEL_REQ;
					res2->charId = cPlayer->GetCharID();
					res2->dojoTblidx = dojo->GetDojoData()->dojoTblidx;
					NTL_SAFE_WCSCPY(res2->wszCharName, req->wszCharName);
					packet2.SetPacketLen(sizeof(sTQ_DOJO_BUDOKAI_SEED_DEL_REQ));
					app->SendTo(app->GetQueryServerSession(), &packet2);

					return;
				}
				else resultcode = COMMUNITY_DOJO_SEED_NAME_FAIL;
			}
			else resultcode = COMMUNITY_DOJO_YOU_ARE_NOT_OWNNER;
		}
		else resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
	}
	else resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;

	CNtlPacket packet(sizeof(sTU_DOJO_BUDOKAI_SEED_DEL_RES));
	sTU_DOJO_BUDOKAI_SEED_DEL_RES* res = (sTU_DOJO_BUDOKAI_SEED_DEL_RES*)packet.GetPacketData();
	res->wOpCode = TU_DOJO_BUDOKAI_SEED_DEL_RES;
	res->wResultCode = resultcode;
	NTL_SAFE_WCSCPY(res->wszCharName, req->wszCharName);
	packet.SetPacketLen(sizeof(sTU_DOJO_BUDOKAI_SEED_DEL_RES));
	app->Send(GetHandle(), &packet);
}

//---------------------------------------------
// DOJO NOTICE CHANGE REQUEST
//---------------------------------------------
void CClientSession::RecvDojoNoticeChangeReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_DOJO_NOTICE_CHANGE_REQ* req = (sUT_DOJO_NOTICE_CHANGE_REQ*)pPacket->GetPacketData();

	WORD resultcode = CHAT_SUCCESS;

	if (cPlayer->GetGuild())
	{
		CGuild* guild = cPlayer->GetGuild();
		if (guild->IsGuildMaster(cPlayer->GetCharID()))
		{
			CDojo* dojo = guild->GetDojo();
			if (dojo)
			{
				CNtlPacket packet2(sizeof(sTQ_DOJO_CHANGE_NOTICE_REQ));
				sTQ_DOJO_CHANGE_NOTICE_REQ * res2 = (sTQ_DOJO_CHANGE_NOTICE_REQ *)packet2.GetPacketData();
				res2->wOpCode = TQ_DOJO_CHANGE_NOTICE_REQ;
				res2->charId = cPlayer->GetCharID();
				res2->dojoTblidx = dojo->GetDojoData()->dojoTblidx;
				NTL_SAFE_WCSCPY(res2->awchCharName, cPlayer->GetCharName());
				res2->wNoticeLengthInUnicode = req->wNoticeLengthInUnicode;
				NTL_SAFE_WCSCPY(res2->awchNotice, req->awchNotice);
				res2->guildId = cPlayer->GetGuildID();
				packet2.SetPacketLen(sizeof(sTQ_DOJO_CHANGE_NOTICE_REQ));
				app->SendTo(app->GetQueryServerSession(), &packet2);

				return;
			}
			else resultcode = COMMUNITY_DOJO_YOU_ARE_NOT_OWNNER;
		}
		else resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
	}
	else resultcode = COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD;

	CNtlPacket packet(sizeof(sTU_DOJO_NOTICE_CHANGE_RES));
	sTU_DOJO_NOTICE_CHANGE_RES* res = (sTU_DOJO_NOTICE_CHANGE_RES*)packet.GetPacketData();
	res->wOpCode = TU_DOJO_NOTICE_CHANGE_RES;
	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sTU_DOJO_NOTICE_CHANGE_RES));
	app->Send(GetHandle(), &packet);
}


//---------------------------------------------
// DOJO NOTICE CHANGE REQUEST
//---------------------------------------------
void CClientSession::RecvHlsSlotMachineInfoReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_HLS_SLOT_MACHINE_INFO_REQ* req = (sUT_HLS_SLOT_MACHINE_INFO_REQ*)pPacket->GetPacketData();

	return;
	g_pHlsSlotMachine->LoadSlotMachines(cPlayer, req->byType);
}


void CClientSession::RecvHlsSlotMachineExtractReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	sUT_HLS_SLOT_MACHINE_EXTRACT_REQ* req = (sUT_HLS_SLOT_MACHINE_EXTRACT_REQ*)pPacket->GetPacketData();

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}
	return;

	CNtlPacket packet(sizeof(sTU_HLS_SLOT_MACHINE_EXTRACT_RES));
	sTU_HLS_SLOT_MACHINE_EXTRACT_RES* res = (sTU_HLS_SLOT_MACHINE_EXTRACT_RES*)packet.GetPacketData();
	res->wOpCode = TU_HLS_SLOT_MACHINE_EXTRACT_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->wMachineIndex = req->wMachineIndex;

	sSLOT_MACHINE* pSlotMachine = (sSLOT_MACHINE*)g_pHlsSlotMachine->GetSlotMachine((TBLIDX)req->wMachineIndex);
	if (pSlotMachine)
	{
		if (pSlotMachine->pTbldat->byType == HLS_MACHINE_TYPE_WAGUWAGU)
		{
			if (req->byExtractCount <= DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS)
			{
				if (pSlotMachine->wCurrentCapsule >= req->byExtractCount)
				{
					if (cPlayer->GetWaguMachineCoin() >= DWORD(pSlotMachine->pTbldat->byCoin * req->byExtractCount)) //check if enough coins
					{
						std::vector<sHLS_SLOT_ITEM*> vecSlotItems;
						g_pHlsSlotMachine->GetSlotItems(pSlotMachine->pTbldat->tblidx, &vecSlotItems);

						if (vecSlotItems.size() > 0)
						{
							cPlayer->IncreaseSlotMachineCount();

							bool bHasTop1 = false;

							CNtlPacket packet2(sizeof(sTQ_HLS_SLOT_MACHINE_EXTRACT_REQ));
							sTQ_HLS_SLOT_MACHINE_EXTRACT_REQ * res2 = (sTQ_HLS_SLOT_MACHINE_EXTRACT_REQ *)packet2.GetPacketData();
							res2->wOpCode = TQ_HLS_SLOT_MACHINE_EXTRACT_REQ;
							res2->accountId = cPlayer->GetAccountID();
							res2->charId = cPlayer->GetCharID();
							res2->machineIndex = req->wMachineIndex;
							res2->byHlsMachineType = HLS_MACHINE_TYPE_WAGUWAGU;
							NTL_SAFE_WCSCPY(res2->wszWinnerName, cPlayer->GetCharName());

							for (BYTE i = 0; i < req->byExtractCount; i++)
							{
								if (vecSlotItems.size() > 0)
								{
									sHLS_SLOT_ITEM* pSlotItem = vecSlotItems[RandomRange(0, (int)vecSlotItems.size() - 1)];

									res2->aItemTblidx[i] = pSlotItem->pHlsItem->tblidx;
									res2->abyStackCount[i] = pSlotItem->pHlsItem->byStackCount;
									res2->abySetCount[i] = 1;
									res2->byRanking[i] = pSlotItem->byRank;

									if (pSlotItem->byRank > 0) // congratulation.. Won top 10 item
									{
										bHasTop1 = pSlotItem->byRank == 1;

										g_pHlsSlotMachine->AddWinner(req->wMachineIndex, pSlotItem->pHlsItem->tblidx, cPlayer);
									}

									pSlotItem->wCountLeft -= 1;

									vecSlotItems.clear();
									g_pHlsSlotMachine->GetSlotItems(pSlotMachine->pTbldat->tblidx, &vecSlotItems);

									res2->byExtractCount++;
								}
							}

							res2->wCoin = WORD(pSlotMachine->pTbldat->byCoin * res2->byExtractCount);

							pSlotMachine->wCurrentCapsule -= res2->byExtractCount;

							if (pSlotMachine->wCurrentCapsule == 0 || bHasTop1)
							{
								g_pHlsSlotMachine->Init();
							}

							packet2.SetPacketLen(sizeof(sTQ_HLS_SLOT_MACHINE_EXTRACT_REQ));
							app->SendTo(app->GetQueryServerSession(), &packet2);

							return;
						}
						else res->wResultCode = WAGUWAGUMACHINE_FAIL;
					}
					else res->wResultCode = WAGUWAGUMACHINE_NOT_ENOUGH_COIN;
				}
				else res->wResultCode = WAGUWAGUMACHINE_NOT_EXIST_QNTT;
			}
			else res->wResultCode = WAGUWAGUMACHINE_NOT_EXIST_QNTT;
		}
		else res->wResultCode = WAGUWAGUMACHINE_NOT_EXIST_MACHINE;
	}
	else res->wResultCode = WAGUWAGUMACHINE_NOT_EXIST_MACHINE;

	res->wNewWaguWaguPoints = 0;
	res->byReallyExtractCount = 0;
	packet.SetPacketLen(sizeof(sTU_HLS_SLOT_MACHINE_EXTRACT_RES));
	app->Send(GetHandle(), &packet);
}


void CClientSession::RecvHlsSlotMachineWinnerInfoReq(CNtlPacket * pPacket)
{
	CChatServer* app = (CChatServer*)g_pApp;

	if (cPlayer == NULL || cPlayer->GetPcInitState() == false)
		return;

	if (cPlayer->GetReadyForCommunityServer() == false)
	{
		return;
	}

	sUT_HLS_SLOT_MACHINE_WINNER_INFO_REQ* req = (sUT_HLS_SLOT_MACHINE_WINNER_INFO_REQ*)pPacket->GetPacketData();

	//g_pHlsSlotMachine->GetWinnerInfo(req->wMachineIndex, cPlayer);
}
