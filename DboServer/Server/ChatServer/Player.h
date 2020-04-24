#pragma once

#include "NtlCharacter.h"
#include "NtlFriend.h"
#include "ClientSession.h"
#include "CallBack.h"



class CGuild;

class CPlayer
{

private:

	bool						m_bReadyForCommunityServer;
	bool						m_bCanLoadFromDB;

	HSESSION					m_hClientSessionID;
	CClientSession*				m_pClientSession;
	bool						m_bInitialized;

	ACCOUNTID					m_AccountID;
	CHARACTERID					m_CharID;
	HOBJECT						m_hHandle;
	BYTE						m_byRace;
	BYTE						m_byClass;
	BYTE						m_byLevel;
	DWORD						m_dwReputation;
	WORLDID						m_WorldID;

	WCHAR						m_awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];

	DWORD						m_dwPunishBitFlag;
	TBLIDX						m_mapNameTblidx;
	SERVERCHANNELID				m_byChannel;
	CNtlVector					m_vCurrentPos;

	DWORD						m_dwWaguMachineCoins;
	DWORD						m_dwEventMachineCoins;

	DWORD						m_dwWaguWaguPoints;

	BYTE*						m_pbyChatServerAuthKey;

	DWORD						m_dwMuteDurationInMilliseconds;

	DWORD						m_dwGlobalChatLock;

	WCHAR						m_wLastMsg[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
	int							m_nMsgSpamCount;
	DWORD						m_dwMsgSpamCountResetTick;
	
	WORD						m_wSlotMachineUsage;

private:

	std::map<CHARACTERID, sFRIEND_FULL_INFO*>			m_map_Friends;

public:
	CPlayer();
	virtual ~CPlayer();


	/************************************************************************/
	/* Player loading and savings                                           */
	void			LoadFromDB();	// Called on login
	void			LoadFromDBProc(QueryResultVector & results);
	void			LoadFromLogDBProc(QueryResultVector & results);

	/************************************************************************/

	void									TickProcess(DWORD dwTickDiff);

	void									Logout(bool bForce = false);

	void									SetReadyForCommunityServer(bool bFlag) { m_bReadyForCommunityServer = bFlag; }
	bool									GetReadyForCommunityServer() { return m_bReadyForCommunityServer; }

	void									SetCanLoadFromDB(bool bFlag) { m_bCanLoadFromDB = bFlag; }
	bool									GetCanLoadFromDB() { return m_bCanLoadFromDB; }

	void									SetClientSessionID(HSESSION hClientSessionID)	{ m_hClientSessionID = hClientSessionID; }
	HSESSION								GetClientSessionID()							{ return m_hClientSessionID; }

	void									SetClientSession(CClientSession* pSession)		{ m_pClientSession = pSession; }
	CClientSession*							GetClientSession()								{ return m_pClientSession; }

	void									SetChatServerAuthKey(BYTE* pbyKey)				{ m_pbyChatServerAuthKey = pbyKey; }
	BYTE*									GetChatServerAuthKey()							{ return m_pbyChatServerAuthKey; }

	void									UpdatePcInitState(bool bFlag)					{ m_bInitialized = bFlag; }
	bool									GetPcInitState()								{ return m_bInitialized; }


	void									SetAccountID(ACCOUNTID id)	{ m_AccountID = id; }
	ACCOUNTID								GetAccountID()				{ return m_AccountID; }

	void									SetCharID(CHARACTERID id)	{ m_CharID = id; }
	CHARACTERID								GetCharID()					{ return m_CharID; }

	void									SetID(HOBJECT hHandle)		{ m_hHandle = hHandle; }
	HOBJECT									GetID()						{ return m_hHandle; }

	void									SetRace(BYTE id)			{ m_byRace = id; }
	BYTE									GetRace()					{ return m_byRace; }

	void									SetClass(BYTE id)			{ m_byClass = id; }
	BYTE									GetClass()					{ return m_byClass; }

	void									SetLevel(BYTE id)			{ m_byLevel = id; }
	BYTE									GetLevel()					{ return m_byLevel; }

	void									SetReputation(DWORD id)		{ m_dwReputation = id; }
	DWORD									GetReputation()				{ return m_dwReputation; }

	void									SetWorldID(WORLDID id)		{ m_WorldID = id; }
	WORLDID									GetWorldID()				{ return m_WorldID; }

	void									SetCharName(WCHAR* awchName);
	WCHAR*									GetCharName()					{ return m_awchCharName; }

	void									SetPunishBitFlag(DWORD flag)	{ m_dwPunishBitFlag = flag; }
	DWORD									GetPunishBitFlag()				{ return m_dwPunishBitFlag; }

	void									SetMapNameTblidx(TBLIDX id)		{ m_mapNameTblidx = id; }
	TBLIDX									GetMapNameTblidx()				{ return m_mapNameTblidx; }

	void									SetLocation(sVECTOR3& v3Pos)	{ m_vCurrentPos = v3Pos; }
	void									SetLocation(CNtlVector& vPos)	{ m_vCurrentPos = vPos; }
	CNtlVector&								GetLocation()					{ return m_vCurrentPos; }

	void									SetChannel(SERVERCHANNELID id)	{ m_byChannel = id; }
	SERVERCHANNELID							GetChannel()					{ return m_byChannel; }

	inline void								SetEventMachineCoin(DWORD cash) { m_dwEventMachineCoins = cash; }
	inline DWORD							GetEventMachineCoin() { return m_dwEventMachineCoins; }

	inline void								SetWaguMachineCoin(DWORD cash) { m_dwWaguMachineCoins = cash; }
	inline DWORD							GetWaguMachineCoin() { return m_dwWaguMachineCoins; }

	inline void								SetWaguWaguPoints(DWORD cash) { m_dwWaguWaguPoints = cash; }
	inline DWORD							GetWaguWaguPoints() { return m_dwWaguWaguPoints; }

	inline void								SetMuteDuration(DWORD dwDuration) { m_dwMuteDurationInMilliseconds = dwDuration; }
	inline DWORD							GetMuteDuration() { return m_dwMuteDurationInMilliseconds; }

	inline void								IncreaseSlotMachineCount() { m_wSlotMachineUsage++; }
	inline WORD								GetSlotMachineCount() { return m_wSlotMachineUsage; }
	


public:

	void									SendPacket(CNtlPacket* pPacket);

	void									Unmute();

	bool									IsMuted();
	
	bool									SpamCheck(WCHAR* wMsg);

//friend
public:

	void									AddFriend(sFRIEND_FULL_INFO* friendinfo);
	void									DelFriend(CHARACTERID CharID);
	void									UpdateBlackFriend(CHARACTERID CharID);
	sFRIEND_FULL_INFO*						GetFriendData(CHARACTERID CharID);
	sFRIEND_FULL_INFO*						GetFriendData(WCHAR* awchCharName);
	void									UpdateFriendInfo(BYTE byChangeType, DWORD dwChangeVlaue);
	void									ReceiveFriendInfo();
	int										GetFriendsCount();
	int										GetBlacklistCount();

	bool									IsBlackListed(CHARACTERID charId);

//guild
private:
	CGuild*									m_pGuild;

	GUILDID									m_GuildID;
	CHARACTERID								m_GuildInvitedByID;

public:

	void									SetGuildID(GUILDID id)	{ m_GuildID = id; }
	GUILDID									GetGuildID()			{ return m_GuildID; }

	void									SetGuild(CGuild* _guild);
	inline CGuild*							GetGuild() { return m_pGuild; }

	void									SetGuildInvitedByID(CHARACTERID CharId)	{ m_GuildInvitedByID = CharId; }
	CHARACTERID								GetGuildInvitedByID()	{ return m_GuildInvitedByID; }


//party
private:

	PARTYID									m_PartyID;

public:

	void									SetPartyID(PARTYID id)	{ m_PartyID = id; }
	PARTYID									GetPartyID()			{ return m_PartyID; }

};