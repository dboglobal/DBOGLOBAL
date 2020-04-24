#include "stdafx.h"
#include "PlayerManager.h"
#include "ChatServer.h"
#include "Party.h"
#include "NtlPacketTU.h"
#include "GuildManager.h"
#include "DojoManager.h"
#include "DojoWar.h"
#include "Utils.h"
#include "NtlPacketTQ.h"
#include "NtlAdmin.h"
#include "NtlStringW.h"


//--------------------------------------------------------------------------------------//
//		CONSTRUCTOR
//--------------------------------------------------------------------------------------//
CPlayer::CPlayer()
{
	m_bReadyForCommunityServer = false;
	m_bCanLoadFromDB = true;

	m_hClientSessionID = INVALID_HSESSION;
	m_pClientSession = NULL;
	m_bInitialized = true;

	m_AccountID = INVALID_ACCOUNTID;
	m_CharID = INVALID_CHARACTERID;
	m_hHandle = INVALID_HOBJECT;
	m_byRace = INVALID_BYTE;
	m_byClass = INVALID_BYTE;
	m_byLevel = INVALID_BYTE;
	m_dwReputation = INVALID_DWORD;
	m_WorldID = INVALID_WORLDID;
	::ZeroMemory(m_awchCharName, NTL_MAX_SIZE_CHAR_NAME + 1);
	m_dwPunishBitFlag = INVALID_DWORD;
	m_mapNameTblidx = INVALID_TBLIDX;
	m_byChannel = INVALID_SERVERCHANNELID;
	m_vCurrentPos.Reset();

	m_dwWaguMachineCoins = 0;
	m_dwEventMachineCoins = 0;
	m_dwWaguWaguPoints = 0;

	m_map_Friends.clear();

	SetGuild(NULL);
	m_GuildInvitedByID = INVALID_CHARACTERID;

	m_PartyID = INVALID_PARTYID;

	m_dwMuteDurationInMilliseconds = 0;

	m_dwGlobalChatLock = 0;
	::ZeroMemory(m_wLastMsg, 0);
	m_nMsgSpamCount = 0;
	m_dwMsgSpamCountResetTick = 30 * 60 * 1000; // 30 minutes
}


//--------------------------------------------------------------------------------------//
//		DESTRUCTOR
//--------------------------------------------------------------------------------------//
CPlayer::~CPlayer()
{
	m_bInitialized = false;

	for (std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.begin(); it != m_map_Friends.end(); it++)
	{
		sFRIEND_FULL_INFO* sFriend = it->second;
		SAFE_DELETE(sFriend);
	}

	m_map_Friends.clear();
}


//--------------------------------------------------------------------------------------//
//		LOAD FROM DATABASE <ASYNC>
//--------------------------------------------------------------------------------------//
void CPlayer::LoadFromDB()
{
	//printf("m_bCanLoadFromDB %i, GetCharID() %u\n", m_bCanLoadFromDB, GetCharID());

	if (m_bCanLoadFromDB)
	{
		m_bCanLoadFromDB = false;

		SQLCallbackBase* pCallBack = new SQLClassCallbackP0<CPlayer>(this, &CPlayer::LoadFromDBProc);
		AsyncQuery * q = new AsyncQuery(pCallBack);
		q->AddQuery("SELECT * FROM friendlist WHERE user_id=%u LIMIT 200", GetCharID()); // LIMIT NTL_MAX_COUNT_FRIEND + NTL_MAX_COUNT_FRIEND
		GetCharDB.QueueAsyncQuery(q);

		SQLCallbackBase* pCallBack2 = new SQLClassCallbackP0<CPlayer>(this, &CPlayer::LoadFromLogDBProc);
		AsyncQuery * q2 = new AsyncQuery(pCallBack2);
		q2->AddQuery("SELECT muteUntil FROM mute_log WHERE CharID=%u", GetCharID());
		GetLogDB.QueueAsyncQuery(q2);
	}
}

//--------------------------------------------------------------------------------------//
//		LOAD FROM DATABASE <RESULT>
//--------------------------------------------------------------------------------------//
void CPlayer::LoadFromDBProc(QueryResultVector & results)
{
	int i = 0;

	if (g_pPlayerManager->FindPlayerWithCharID(GetCharID()) == NULL)
	{
		ERR_LOG(LOG_USER, "Player %u not found in player map", GetCharID());
		return;
	}

	if (GetClientSession() == NULL || GetClientSessionID() == INVALID_HSESSION)
	{
		ERR_LOG(LOG_USER, "Player %u has no session", GetCharID());
		return;
	}


	//GUILD
	if (GetGuildID() > 0)
	{
		SetGuild(g_pGuildManager->GetGuild(GetGuildID()));

		if (GetGuild())
			GetGuild()->SetMember(true, this);
	}
	else
	{
		SetGuild(NULL);
	}

	//friend & blacklist info
	QueryResult *result = results[0].result;
	if (result)
	{
		i = 0;
		int totalcount = result->GetRowCount();
	//	ERR_LOG(LOG_SYSTEM, "Load friends from user %u COUNT %i \n", GetCharID(), totalcount);
		if (totalcount > 0)
		{
			CNtlPacket packet(sizeof(sTU_FRIEND_LIST_INFO));
			sTU_FRIEND_LIST_INFO* res = (sTU_FRIEND_LIST_INFO*)packet.GetPacketData();
			res->wOpCode = TU_FRIEND_LIST_INFO;
			do
			{
				Field *f = result->Fetch();

				res->asInfo[i].charID = f[1].GetUInt32();
				NTL_SAFE_WCSCPY(res->asInfo[i].wchName, s2ws(f[2].GetString()).c_str());
				res->asInfo[i].bIsBlack = f[3].GetBool();

				this->AddFriend(&res->asInfo[i++]);

				if (i == NTL_MAX_COUNT_FRIEND_PACKET || i == totalcount)
				{
					res->byCount = i;
					packet.SetPacketLen(sizeof(sTU_FRIEND_LIST_INFO));
					g_pApp->Send(GetClientSessionID(), &packet);

					totalcount -= i;

					i = 0;
					memset(res->asInfo, 0, sizeof(res->asInfo)); //set it to NULL
				}
			} while (result->NextRow());

			//send friends info to me
			ReceiveFriendInfo();
		}
	}

	//send my info to friends (This is send, even if player does not have any friends.. Maybe other have player as friend)
	CNtlPacket pFriendInfo2(sizeof(sTU_FRIEND_INFO));
	sTU_FRIEND_INFO* rFriendInfo2 = (sTU_FRIEND_INFO*)pFriendInfo2.GetPacketData();
	rFriendInfo2->wOpCode = TU_FRIEND_INFO;
	rFriendInfo2->bIsFirst = false; //false = show that I have logged in
	rFriendInfo2->sInfo.byChannel = GetChannel();
	rFriendInfo2->sInfo.byClass = GetClass();
	rFriendInfo2->sInfo.byLevel = GetLevel();
	rFriendInfo2->sInfo.charID = GetCharID();
	rFriendInfo2->sInfo.hHandle = GetID();
	rFriendInfo2->sInfo.mapNameTblidx = GetMapNameTblidx();
	pFriendInfo2.SetPacketLen(sizeof(sTU_FRIEND_INFO));
	g_pPlayerManager->UserSendMsgToFriends(&pFriendInfo2, GetCharID());

	if (GetGuildID() != 0)
	{
		CDojo* pDojo = g_pDojoWarManager->GetDojo(GetGuildID());
		if (pDojo)
		{
			pDojo->SendDojoState(GetClientSessionID());
		}
		else
		{
			g_pDojoWarManager->SendDojoWarState(GetClientSessionID());
		}
	}
}


//--------------------------------------------------------------------------------------//
//		LOAD FROM LOG DATABASE <RESULT>
//--------------------------------------------------------------------------------------//
void CPlayer::LoadFromLogDBProc(QueryResultVector & results)
{
	if (g_pPlayerManager->FindPlayerWithCharID(GetCharID()) == NULL)
	{
		ERR_LOG(LOG_USER, "Player %u not found in player map", GetCharID());
		return;
	}

	if (results[0].result)
	{
		Field *f = results[0].result->Fetch();

		m_dwMuteDurationInMilliseconds = f[0].GetDWORD();
	}
}



void CPlayer::TickProcess(DWORD dwTickDiff)
{
	if (m_dwMuteDurationInMilliseconds > 0)
	{
		m_dwMuteDurationInMilliseconds = UnsignedSafeDecrease<DWORD>(m_dwMuteDurationInMilliseconds, dwTickDiff);

		if (m_dwMuteDurationInMilliseconds == 0)
		{
			Unmute();
		}
	}

	m_dwGlobalChatLock = UnsignedSafeDecrease<DWORD>(m_dwGlobalChatLock, dwTickDiff);

	if (m_dwMsgSpamCountResetTick > 0)
	{
		m_dwMsgSpamCountResetTick = UnsignedSafeDecrease<DWORD>(m_dwMsgSpamCountResetTick, dwTickDiff);

		if (m_dwMsgSpamCountResetTick == 0)
		{
			m_nMsgSpamCount = 0;
			m_dwMsgSpamCountResetTick = 30 * 60 * 1000; // 30 minutes
		}
	}
}

//--------------------------------------------------------------------------------------//
//		LOGOUT FROM CHAT SERVER
//--------------------------------------------------------------------------------------//
void CPlayer::Logout(bool bForce/* = false*/)
{
	CChatServer* app = (CChatServer*)g_pApp;

	UpdateFriendInfo(eFRIEND_CHANGE_TYPE_OFFLINE, 1);	//send to friend that player logged out

	//remove player from party
	if (GetPartyID() != INVALID_PARTYID)
		g_pPartyManager->RemoveMember(GetPartyID(), GetCharID());

	if (m_dwMuteDurationInMilliseconds > 0)
	{
		GetLogDB.Execute("UPDATE mute_log SET muteUntil=%u WHERE CharID=%u", m_dwMuteDurationInMilliseconds, GetCharID());
	}

	//notify guild that player logged out
	if (GetGuildID() > 0 && GetGuild())
	{
		//check if inside dojo
		if (GetChannel() == DOJO_CHANNEL_INDEX)
		{
			//decrease dojo player counter
			CDojo* pDojo = g_pDojoWarManager->GetDojo(GetGuildID());
			if (pDojo)
			{
				pDojo->UpdatePlayerCount(GetGuildID(), GetCharID(), false);
			}
		}

		GetGuild()->SetMember(false, this);
		SetGuild(NULL);
	}

	SetClientSession(NULL);
	SetClientSessionID(INVALID_HSESSION);
}


//--------------------------------------------------------------------------------------//
//		SET CHAR NAME
//--------------------------------------------------------------------------------------//
void CPlayer::SetCharName(WCHAR* awchName)
{
	wcscpy_s(m_awchCharName, NTL_MAX_SIZE_CHAR_NAME + 1, awchName);
}


void CPlayer::SendPacket(CNtlPacket * pPacket)
{
	if (GetClientSession())
		g_pApp->Send(GetClientSessionID(), pPacket);
}


void CPlayer::Unmute()
{
	CChatServer* app = (CChatServer*)g_pApp;

	CNtlPacket packet(sizeof(sTQ_MUTE_PLAYER_NFY));
	sTQ_MUTE_PLAYER_NFY * res = (sTQ_MUTE_PLAYER_NFY *)packet.GetPacketData();
	res->wOpCode = TQ_MUTE_PLAYER_NFY;
	res->dwDurationInMinute = 0;
	res->charId = GetCharID();
	packet.SetPacketLen(sizeof(sTQ_MUTE_PLAYER_NFY));
	app->SendTo(app->GetQueryServerSession(), &packet);
}

bool CPlayer::IsMuted()
{
	if (m_dwMuteDurationInMilliseconds > 0)
	{
		CNtlStringW wString;

		CNtlPacket packet(sizeof(sTU_SYSTEM_DISPLAY_TEXT));
		sTU_SYSTEM_DISPLAY_TEXT * res = (sTU_SYSTEM_DISPLAY_TEXT *)packet.GetPacketData();
		res->wOpCode = TU_SYSTEM_DISPLAY_TEXT;
		res->byDisplayType = SERVER_TEXT_SYSTEM;
		res->wMessageLengthInUnicode = wString.Format(L"You have been muted :) Mute ends in %f minutes", (float)m_dwMuteDurationInMilliseconds / 60.f / 1000.f);
		NTL_SAFE_WCSCPY(res->wszMessage, wString.c_str());
		packet.SetPacketLen(sizeof(sTU_SYSTEM_DISPLAY_TEXT));
		g_pApp->Send(GetClientSessionID(), &packet);

		return true;
	}

	return false;
}

bool CPlayer::SpamCheck(WCHAR * wMsg)
{
	if (m_dwGlobalChatLock > 0)
		return false;

	if (wcscmp(m_wLastMsg, wMsg) == 0)
	{
		m_dwGlobalChatLock = DWORD(++m_nMsgSpamCount * 2000);
	}
	else
	{
		NTL_SAFE_WCSCPY(m_wLastMsg, wMsg);
	}

	return true;
}


//--------------------------------------------------------------------------------------//
//		ADD FRIEND INFO
//--------------------------------------------------------------------------------------//
void	CPlayer::AddFriend(sFRIEND_FULL_INFO* friendinfo)
{
	sFRIEND_FULL_INFO* newfriend = new sFRIEND_FULL_INFO(friendinfo);

	m_map_Friends.insert(std::make_pair(friendinfo->charID, newfriend));
}

//--------------------------------------------------------------------------------------//
//		DEL FRIEND INFO
//--------------------------------------------------------------------------------------//
void	CPlayer::DelFriend(CHARACTERID CharID)
{
	std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.find(CharID);
	if (m_map_Friends.end() != it)
	{
		sFRIEND_FULL_INFO* pFriend = it->second;
		SAFE_DELETE(pFriend);

		m_map_Friends.erase(it);
	}
}

//--------------------------------------------------------------------------------------//
//		SET BLACKLIST
//--------------------------------------------------------------------------------------//
void	CPlayer::UpdateBlackFriend(CHARACTERID CharID)
{
	std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.find(CharID);
	if (it != m_map_Friends.end())
	{
		sFRIEND_FULL_INFO* friendinfo = it->second;
		if (friendinfo)
		{
			if (friendinfo->charID == CharID)
				friendinfo->bIsBlack = true;
		}
	}
}

//--------------------------------------------------------------------------------------//
//		GET FRIEND INFO BY CHAR ID
//--------------------------------------------------------------------------------------//
sFRIEND_FULL_INFO*	CPlayer::GetFriendData(CHARACTERID CharID)
{
	std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.find(CharID);

	if (m_map_Friends.end() == it)
		return NULL;

	return it->second;
}

//--------------------------------------------------------------------------------------//
//		GET FRIEND INFO BY CHAR NAME
//--------------------------------------------------------------------------------------//
sFRIEND_FULL_INFO*	CPlayer::GetFriendData(WCHAR* awchCharName)
{
	for (std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.begin(); it != m_map_Friends.end(); it++)
	{
		sFRIEND_FULL_INFO* info = it->second;
		if (info)
		{
			if (_wcsicmp(info->wchName, awchCharName) == 0)
				return info;
		}
	}

	return NULL;
}


//--------------------------------------------------------------------------------------//
//		UPDATE FRIEND INFO
//--------------------------------------------------------------------------------------//
void CPlayer::UpdateFriendInfo(BYTE byChangeType, DWORD dwChangeVlaue)
{
	CNtlPacket packet(sizeof(sTU_FRIEND_INFO_CHANGE));
	sTU_FRIEND_INFO_CHANGE * res = (sTU_FRIEND_INFO_CHANGE *)packet.GetPacketData();
	res->wOpCode = TU_FRIEND_INFO_CHANGE;
	res->targetID = GetCharID();
	res->byChangeType = byChangeType;
	res->dwChangeValue = dwChangeVlaue;
	packet.SetPacketLen(sizeof(sTU_FRIEND_INFO_CHANGE));
	g_pPlayerManager->UserSendMsgToFriends(&packet, GetCharID());
}


//--------------------------------------------------------------------------------------//
//		SEND INFO FROM FRIENDS TO ME (USED AT LOGIN)
//--------------------------------------------------------------------------------------//
void CPlayer::ReceiveFriendInfo()
{
	for (std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.begin(); it != m_map_Friends.end(); it++)
	{
		sFRIEND_FULL_INFO* friendinfo = it->second;
		if (friendinfo)
		{
			if (friendinfo->bIsBlack)
				continue;

			CPlayer* target = g_pPlayerManager->FindPlayerWithCharID(friendinfo->charID);
			if (target && target->GetReadyForCommunityServer())
			{
				CNtlPacket pFriendInfo2(sizeof(sTU_FRIEND_INFO));
				sTU_FRIEND_INFO* rFriendInfo2 = (sTU_FRIEND_INFO*)pFriendInfo2.GetPacketData();
				rFriendInfo2->wOpCode = TU_FRIEND_INFO;
				rFriendInfo2->bIsFirst = true;
				rFriendInfo2->sInfo.byChannel = target->GetChannel();
				rFriendInfo2->sInfo.byClass = target->GetClass();
				rFriendInfo2->sInfo.byLevel = target->GetLevel();
				rFriendInfo2->sInfo.charID = friendinfo->charID;
				rFriendInfo2->sInfo.hHandle = target->GetID();
				rFriendInfo2->sInfo.mapNameTblidx = target->GetMapNameTblidx();
				pFriendInfo2.SetPacketLen(sizeof(sTU_FRIEND_INFO));
				g_pApp->Send(GetClientSessionID(), &pFriendInfo2);
			}
		}
	}
}

int CPlayer::GetFriendsCount()
{
	int nCount = 0;

	for (std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.begin(); it != m_map_Friends.end(); it++)
	{
		sFRIEND_FULL_INFO* friendinfo = it->second;
		if (friendinfo)
		{
			if (friendinfo->bIsBlack == false)
				++nCount;
		}
	}
	//printf("nCount %u \n", nCount);
	return nCount;
}

int CPlayer::GetBlacklistCount()
{
	int nCount = 0;

	for (std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.begin(); it != m_map_Friends.end(); it++)
	{
		sFRIEND_FULL_INFO* friendinfo = it->second;
		if (friendinfo)
		{
			if (friendinfo->bIsBlack == true)
				++nCount;
		}
	}

	return nCount;
}

bool CPlayer::IsBlackListed(CHARACTERID charId)
{
	std::map<CHARACTERID, sFRIEND_FULL_INFO*>::iterator it = m_map_Friends.find(charId);
	if (it != m_map_Friends.end())
	{
		sFRIEND_FULL_INFO* friendinfo = it->second;
		if (friendinfo)
		{
			if (friendinfo->charID == charId && friendinfo->bIsBlack)
				return true;
		}
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//		SET GUILD
//--------------------------------------------------------------------------------------//
void CPlayer::SetGuild(CGuild* _guild)
{
	if (_guild)
	{
		m_pGuild = _guild;
		m_GuildID = _guild->GetInfo()->guildId;
	}
	else
	{
		SetGuildID(0);
		m_pGuild = NULL;
	}
}
