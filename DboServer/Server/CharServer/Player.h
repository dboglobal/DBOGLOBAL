#ifndef __PLAYER___
#define __PLAYER___


#include "NtlCharacter.h"
#include "CallBack.h"


class CClientSession;

class CPlayer
{

public:

	CPlayer(ACCOUNTID accountId, CClientSession* pSession);
	virtual ~CPlayer();

public:

	void						SetSession(CClientSession* pSession) { m_pSession = pSession; }
	CClientSession*				GetSession() { return m_pSession; }

	void						SetSessionHandle(HSESSION handle) { m_hSession = handle; }
	HSESSION					GetSessionHandle() { return m_hSession; }

	ACCOUNTID					GetAccountID() { return m_accountId; }

	void						SetServerFarmID(SERVERFARMID id) { m_serverFarmID = id; }
	SERVERFARMID				GetServerFarmID() { return m_serverFarmID; }

	void						SetServerChannelID(SERVERCHANNELID id) { m_serverChannelID = id; }
	SERVERCHANNELID				GetServerChannelID() { return m_serverChannelID; }

	bool						IsServerChanged() { return m_bIsServerChanged; }
	void						SetServerChanged(bool bFlag) { m_bIsServerChanged = bFlag; }

	bool						CheckQueue() { return m_bIsQueue; }
	void						SetQueue(bool b) { m_bIsQueue = b; }

	void						SetCharSelect(CHARACTERID id) { m_characterSelectId = id; }
	CHARACTERID					GetCharSelect() { return m_characterSelectId; }

	bool						IsGM() { return m_bIsGM; }
	void						SetGM(bool bFlag) { m_bIsGM = bFlag; }

	BYTE						GetCharCount() { return m_byCharCount; }


public:

	void						LoadCharactersFromDB();

	void						LoadFromDBProc(QueryResultVector & results, ACCOUNTID accountID);
	void						LoadFromDBProcSecond(QueryResultVector & results, ACCOUNTID accountID);
	void						LoadFromDBProcEnd(QueryResultVector & results, ACCOUNTID accountID);

	void						SetCharDeleted(CHARACTERID charId, time_t DelTime);
	void						UnsetCharDeleted(CHARACTERID charId);

	void						SetNewCharName(CHARACTERID charId, WCHAR * wchCharName);

	bool						IsCharsLoaded() { return m_bIsCharLoaded; }

	void						DisableLoadCharInfo(bool bFlag) { m_bIsStartCharLoading = bFlag; }
	bool						IsCharLoadRequestPossible() { return m_bIsStartCharLoading; }

	void						LoadFromCache();

	bool						HasCharacter(CHARACTERID charId);
	bool						HasDelPendingCharacter(CHARACTERID charId);

	void						AddCharacter(sPC_SUMMARY* pSummary);

private:

	ACCOUNTID					m_accountId;

	CClientSession*				m_pSession;
	HSESSION					m_hSession;

	SERVERFARMID				m_serverFarmID;
	SERVERCHANNELID				m_serverChannelID;
	bool						m_bIsServerChanged;

	bool						m_bIsQueue;

	CHARACTERID					m_characterSelectId;

	BYTE						m_byPremiumSlotCount;

	bool						m_bIsGM;

	bool						m_bIsStartCharLoading;
	bool						m_bIsCharLoaded;

	BYTE						m_byCharCount;
	sDELETE_WAIT_CHARACTER_INFO	m_asDelData[NTL_MAX_COUNT_USER_CHAR_SLOT];
	sPC_SUMMARY					m_sPcData[NTL_MAX_COUNT_USER_CHAR_SLOT];



//async delete
public:

	bool						DecreaseDeleteTime(DWORD dwTickCount);

private:

	DWORD						m_dwDeleteTime;

};

#endif