#pragma once

#include "NtlSharedType.h"
#include "NtlSingleton.h"
#include <mutex>


class CPlayer;
class CNtlPacket;


class CPlayerManager : public CNtlSingleton<CPlayerManager>
{

public:

	CPlayerManager();
	virtual ~CPlayerManager();

	void Release();


private:

	std::vector<CPlayer*>							m_vecDeleteAccount;
	std::map<ACCOUNTID, CPlayer*>					m_map_Player;

public:

	size_t						GetPlayerCount() { return m_map_Player.size(); }

public:

	void						TickProcess(DWORD dwTickDiff);
		
	void						DisconnectAll();

	void						RemovePlayer(CPlayer* pPlayer);

	CPlayer*					GetPlayer(ACCOUNTID accid);

	CPlayer*					CreatePlayer(ACCOUNTID AccountID, CClientSession* pSession);

	void						SendToAccount(CNtlPacket * pPacket, ACCOUNTID AccountID);
	void						SendToAll(CNtlPacket * pPacket);
	void						SendToAll(CNtlPacket * pPacket, SERVERFARMID serverid);


private:

	std::mutex					m_mutex;

};
#define GetPlayerManager()		CPlayerManager::GetInstance()
#define g_PlrMgr				GetPlayerManager()