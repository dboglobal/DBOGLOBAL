#ifndef __SERVER_PLAYER_MANAGER__
#define __SERVER_PLAYER_MANAGER__

#include "Player.h"
#include "NtlSingleton.h"

#include <unordered_map>


class CPlayerManager : public CNtlSingleton<CPlayerManager>
{

public:
	CPlayerManager();
	virtual ~CPlayerManager();

private:

	std::unordered_map<ACCOUNTID, CPlayer*> m_map_Player;

public:
	void			Init();

	void			TickProcess(DWORD dwTickDiff);

	CPlayer*		CreatePlayer(ACCOUNTID AccID);
	void			RemovePlayer(ACCOUNTID AccID);

	int				GetPlayerCount() { return (int)m_map_Player.size(); }

	void			RemoveAllPlayers(SERVERCHANNELID channelId = INVALID_SERVERCHANNELID);

	CPlayer*		Find(HSESSION hClientSession);
	CPlayer*		FindPlayer(HOBJECT hPlayerID);
	CPlayer*		FindPlayerWithAccID(ACCOUNTID AccID);
	CPlayer*		FindPlayerWithCharID(CHARACTERID CharID);
	CPlayer*		FindPlayerByName(WCHAR* wchName);


	//packet
	void			UserSendMsgToFriends(CNtlPacket* pPacket, CHARACTERID CharId);

	void			SendMsgToAll(CNtlPacket* pPacket, CHARACTERID senderCharId = -1, bool bCheckBlacklist = false);
	void			SendMsgToInRange(CNtlPacket* pPacket, CPlayer* pPlayer, float fRange, SERVERCHANNELID channel, WORLDID worldid, bool bCheckBlacklist = false);


};

#define GetPlayerManager()		CPlayerManager::GetInstance()
#define g_pPlayerManager		GetPlayerManager()

#endif