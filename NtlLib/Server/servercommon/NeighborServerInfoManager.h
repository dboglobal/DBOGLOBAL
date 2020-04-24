#ifndef __NEIGHBOR_SERVER_INFO_MANAGER__
#define __NEIGHBOR_SERVER_INFO_MANAGER__

#include "NtlCSArchitecture.h"

struct sAUTHINFO
{
	DWORD		dwTimeRegistered;
	BYTE		byAuthKey[NTL_MAX_SIZE_AUTH_KEY];
};

#include <list>
#include <map>

class CNeighborServerInfoManager
{

public:

	CNeighborServerInfoManager();
	virtual ~CNeighborServerInfoManager();

public:

	bool											Create(eNtlServerType byOwnerServerType); //eNtlServerType

	void											Destroy();

protected:

	void											Init();

public:

	bool											AddAuthServerFarmInfo(sDBO_AUTH_SERVER_FARM_INFO *pServerFarmInfoRef);

	bool											AddCharacterServerFarmInfo(sDBO_CHARACTER_SERVER_FARM_INFO *pServerFarmInfoRef);

	bool											AddServerFarmInfo(sDBO_GAME_SERVER_FARM_INFO *pServerFarmInfoRef);

	bool											AddServerChannelInfo(sDBO_GAME_SERVER_CHANNEL_INFO *pServerChannelInfoRef);

	bool											AddServerInfo(sDBO_SERVER_INFO *pServerInfoRef);
	
public:

	bool											RefreshAuthServerFarmInfo(sDBO_AUTH_SERVER_FARM_INFO *pServerFarmInfoRef);

	bool											RefreshCharacterServerFarmInfo(sDBO_CHARACTER_SERVER_FARM_INFO *pServerFarmInfoRef);

	bool											RefreshServerFarmInfo(sDBO_GAME_SERVER_FARM_INFO *pServerFarmInfoRef);

	bool											RefreshServerChannelInfo(sDBO_GAME_SERVER_CHANNEL_INFO *pServerChannelInfoRef);

	bool											RefreshServerInfo(sDBO_SERVER_INFO *pServerInfoRef);

public:

	bool											DestroyAllServerInfo();

public:

	sAUTH_SERVER_FARM_INFO*							GetAuthServerFarmInfo() { return m_pAuthServerFarmInfo; }

	sCHARACTER_SERVER_FARM_INFO*					GetCharacterServerFarmInfo() { return m_pCharacterServerFarmInfo; }

	sSERVER_FARM_INFO*								GetServerFarmInfo(SERVERFARMID serverFarmId);

	sSERVER_CHANNEL_INFO*							GetServerChannelInfo(SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex);

	sDBO_SERVER_INFO*								GetServerInfo(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex);

	sDBO_SERVER_INFO*								GetServerInfo(BYTE byServerType, WCHAR *pwszServerIP);

	sDBO_SERVER_INFO*								GetServerInfo(BYTE byServerType, char *pszServerIP);

	sDBO_SERVER_INFO*								GetIdlestServerInfo(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex);

	sDBO_SERVER_INFO*								GetIdlestQueryServerInfo();


public:

	bool											SetServerOn(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex);

	bool											SetServerOff(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex);

	bool											SetServerRunningState(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex, BYTE byRunningState);

	bool											UpdateAuthServerFarmStatus(BYTE byServerStatus, DWORD dwMaxLoad, DWORD dwLoad);

	bool											UpdateCharacterServerFarmStatus(BYTE byServerStatus, DWORD dwMaxLoad, DWORD dwLoad);

	bool											UpdateGameServerFarmStatus(SERVERFARMID serverFarmId, BYTE byServerStatus, DWORD dwMaxLoad, DWORD dwLoad);
			
	bool											UpdateGameServerChannelStatus(SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, BYTE byServerStatus, DWORD dwMaxLoad, DWORD dwLoad, bool bIsScrambleChannel, _CHANNEL_BUFF& rsChannelBuff);

	bool											UpdateServerInfo(BYTE byServerType, SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, SERVERINDEX byServerIndex, sDBO_SERVER_INFO& newServerInfo, bool bIsScrambleChannel, _CHANNEL_BUFF& rChannelBuff);

	bool											UpdateConnectedUserInfo(SERVERFARMID serverFarmId, SERVERCHANNELID serverChannelId, sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO& rConnectedUserInfo);

	bool											GetLoadExcludingSpecialServerChannel(SERVERFARMID serverFarmId, DWORD & rdwMaxLoad, DWORD & rdwLoad);

	bool											GetLoadExcludingSpecialServer(SERVERFARMID serverFarmId, SERVERCHANNELID serverChannelId, DWORD & rdwMaxLoad, DWORD & rdwLoad);

	std::map<SERVERFARMID, sSERVER_FARM_INFO *>		GetRawServerFarmList() { return m_mapGameServerFarmInfo; }

	std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO *>*	GetRawServerChannelList(SERVERFARMID serverFarmId);

	std::list<sDBO_SERVER_INFO* >*					GetRawServerList(BYTE byServerType);


protected:

	virtual bool									OnServerTurnedOn(sDBO_SERVER_INFO *pServerInfo);

	virtual bool									OnServerTurnedOff(sDBO_SERVER_INFO *pServerInfo);

	virtual bool									OnServerRunningStateChanged(sDBO_SERVER_INFO *pServerInfo);

	virtual bool									OnServerLoadChanged(sDBO_SERVER_INFO *pServerInfo);

	sSERVER_FARM_INFO*								GetServerFarmInfo_Internal(SERVERFARMID serverFarmId);

	sSERVER_CHANNEL_INFO*							GetServerChannelInfo_Internal(sSERVER_FARM_INFO *pServerFarmInfo, SERVERCHANNELID byServerChannelIndex);

	std::list<sDBO_SERVER_INFO* >*					GetServerInfoList(SERVERFARMID serverFarmId, SERVERCHANNELID byServerChannelIndex, BYTE byServerType);

	sDBO_SERVER_INFO*								GetServerInfoHelper(std::list<sDBO_SERVER_INFO* >* pServerInfoList, SERVERINDEX byServerIndex);

	sDBO_SERVER_INFO*								GetServerInfoHelper(std::list<sDBO_SERVER_INFO* >* pServerInfoList, char* pszServerIP);


protected:

	std::list<sDBO_SERVER_INFO* >					m_listMasterServerInfo;
	std::list<sDBO_SERVER_INFO *>					m_listAuthServerInfo;
	std::list<sDBO_SERVER_INFO *>					m_listCharacterServerInfo;
	std::list<sDBO_SERVER_INFO *>					m_listGameServerInfo;
	std::list<sDBO_SERVER_INFO *>					m_listQueryServerInfo;
	std::list<sDBO_SERVER_INFO *>					m_listCommunityServerInfo;

	sAUTH_SERVER_FARM_INFO *						m_pAuthServerFarmInfo;			//NOT USED
	sCHARACTER_SERVER_FARM_INFO *					m_pCharacterServerFarmInfo;		//NOT USED

	std::map<SERVERFARMID, sSERVER_FARM_INFO *>		m_mapGameServerFarmInfo;

	BYTE											m_byOwnerServerType;
	BYTE											m_byOwnerServerIndex;


	//Authtentication system
private:

	std::map<ACCOUNTID, sAUTHINFO*>		m_mapAuthKeys;

public:

	bool								GenerateAuthKey(void* pvAuthKey, ACCOUNTID accid);
	void								GenerateAuthKey(BYTE* pvAuthKey);
	void								StoreAuthKey(BYTE* pvAuthKey, ACCOUNTID accid);
	bool								CheckAuthKey(BYTE* pvAuthKey, ACCOUNTID accid);

};


#endif