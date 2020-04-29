#ifndef __SERVER_NEIGHBORSERVERINFOMANAGER_MANAGER__
#define __SERVER_NEIGHBORSERVERINFOMANAGER_MANAGER__

#include "NtlSingleton.h"
#include <set>
#include <unordered_map>

#include "NeighborServerInfoManager.h"
#include "EventableObject.h"


class CSubNeighborServerInfoManager : public CNeighborServerInfoManager, public EventableObject, public CNtlSingleton<CSubNeighborServerInfoManager>
{

public:
	CSubNeighborServerInfoManager();
	virtual ~CSubNeighborServerInfoManager();

private:

	EventableObjectHolder *		m_pEventHolder;

public:

	void						TickProcess(DWORD dwTickDiff);

	void						UpdateScrambleChannel(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, bool bIsScrambleChannel);

	void						StartEvents();
	void						OnEvent_ServerLoadUpdate();

	/////

//SERVER SESSIONS
private:

	std::map<SERVERINDEX, CAuthServerPassiveSession *>	m_mapAuthServerSession;
	std::map<SERVERINDEX, CCharServerPassiveSession *>	m_mapCharacterServerSession;
	std::map<SERVERINDEX, CChatServerPassiveSession *>	m_mapChatServerSession;
	std::map<_SERVER_PAIR_ID, CGameServerPassiveSession *>	m_mapGameServerSession;

public:

	void						AddAuthServerSession(SERVERINDEX serverIndex, CAuthServerPassiveSession* pSession);
	void						AddCharServerSession(SERVERINDEX serverIndex, CCharServerPassiveSession* pSession);
	void						AddChatServerSession(SERVERINDEX serverIndex, CChatServerPassiveSession* pSession);
	void						AddGameServerSession(_SERVER_PAIR_ID serverPairID, CGameServerPassiveSession* pSession);


// PLAYER
private:

	std::unordered_map<ACCOUNTID, SERVERINDEX> m_map_CharServerPlayers;
	std::unordered_map<ACCOUNTID, _SERVER_PAIR_ID> m_map_GameServerPlayers; //for gameservers

public:

	void						AddPlayerGameServer(ACCOUNTID AccID, SERVERFARMID serverfarm, SERVERCHANNELID channel);
	void						AddPlayerCharServer(ACCOUNTID AccID, SERVERINDEX serverIndex);

	void						RemoveAllPlayerFromCharServer(SERVERINDEX serverIndex);
	void						RemoveAllPlayerFromGameServer(_SERVER_PAIR_ID serverPairID);

	void						DelPlayer(ACCOUNTID AccID, bool bIsGameServer, SERVERFARMID serverfarmId = INVALID_SERVERFARMID, SERVERCHANNELID channelId = INVALID_SERVERCHANNELID);
	bool						IsPlayerOnline(ACCOUNTID AccID);

	void						PrintOnlinePlayers();

	int							GetPlayerInCharServer(SERVERINDEX serverIndex = INVALID_SERVERINDEX);
	int							GetPlayerInGameServer(SERVERCHANNELID byChannel = INVALID_SERVERCHANNELID);


//
public:

	void						SendToServer(BYTE byServerType, SERVERINDEX serverIndex, SERVERFARMID serverFarmID, SERVERCHANNELID serverChannelID, CNtlPacket* pPacket);
	void						BroadcastServer(BYTE byServerType, CNtlPacket* pPacket, SERVERINDEX exceptServerIndex = INVALID_SERVERINDEX, SERVERFARMID excepServerFarmID = INVALID_SERVERFARMID);
	void						BroadcastFarmServer(BYTE byServerType, SERVERFARMID serverFarmID, SERVERCHANNELID exceptionChannelID, CNtlPacket* pPacket);

	virtual bool				OnServerTurnedOn(sDBO_SERVER_INFO *pServerInfo);
	virtual bool				OnServerTurnedOff(sDBO_SERVER_INFO *pServerInfo);

	void						UpdatePlayerCount(BYTE byServerType, SERVERINDEX serverIndex, SERVERFARMID serverFarmID, SERVERCHANNELID serverChannelID, bool bPlus, DWORD dwCount);

};

#define GetServerManager()		CSubNeighborServerInfoManager::GetInstance()
#define g_pSrvMgr				GetServerManager()

#endif