#pragma once

#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlCharacter.h"

// Server Type Enumeration
enum eNtlServerType
{
	NTL_SERVER_TYPE_MASTER = 0,
	NTL_SERVER_TYPE_AUTH,
	NTL_SERVER_TYPE_CHARACTER,
	NTL_SERVER_TYPE_GAME,
	NTL_SERVER_TYPE_NPC,
	NTL_SERVER_TYPE_QUERY,
	NTL_SERVER_TYPE_COMMUNITY,
	NTL_SERVER_TYPE_LOG,
	NTL_SERVER_TYPE_OPERATING,
	NTL_SERVER_TYPE_REPORT,
	NTL_SERVER_TYPE_SERVER_AGENT,
	NTL_SERVER_TYPE_SERVER_AGENT_CONTROLLER,
	NTL_SERVER_TYPE_TEXTLOCAL,
	
	NTL_SERVER_TYPE_INVALID = 0xFF,

	NTL_SERVER_TYPE_FIRST = NTL_SERVER_TYPE_MASTER,
	NTL_SERVER_TYPE_LAST = NTL_SERVER_TYPE_SERVER_AGENT_CONTROLLER,
	NTL_SERVER_TYPE_COUNT = NTL_SERVER_TYPE_LAST - NTL_SERVER_TYPE_FIRST + 1,

	NTL_SERVER_TYPE_SLAVE_FIRST = NTL_SERVER_TYPE_AUTH,
	NTL_SERVER_TYPE_SLAVE_LAST = NTL_SERVER_TYPE_REPORT,
	NTL_SERVER_TYPE_SLAVE_COUNT = NTL_SERVER_TYPE_SLAVE_LAST - NTL_SERVER_TYPE_SLAVE_FIRST + 1
};

// Server Type Enumeration [11/20/2007 SGpro]
enum eNtlServerGroupType
{
	NTL_SERVER_GROUP_TYPE_AUTH = 0,
	NTL_SERVER_GROUP_TYPE_CHARACTER,
	NTL_SERVER_GROUP_TYPE_SERVERFARM_TOP,//모든 서버팜에 최상위
	NTL_SERVER_GROUP_TYPE_SERVERFARM, //각각의 서버팜
	NTL_SERVER_GROUP_TYPE_CHANNEL,//서버팜 밑에 채널

	NTL_SERVER_GROUP_TYPE_INVALID = 0xFF,

	NTL_SERVER_GROUP_TYPE_FIRST = NTL_SERVER_GROUP_TYPE_AUTH,
	NTL_SERVER_GROUP_TYPE_LAST = NTL_SERVER_GROUP_TYPE_CHANNEL,
	NTL_SERVER_GROUP_TYPE_COUNT = NTL_SERVER_TYPE_LAST - NTL_SERVER_TYPE_FIRST + 1,

	NTL_SERVER_GROUP_TYPE_SLAVE_FIRST = NTL_SERVER_GROUP_TYPE_SERVERFARM_TOP,
	NTL_SERVER_GROUP_TYPE_SLAVE_LAST = NTL_SERVER_GROUP_TYPE_CHANNEL,
	NTL_SERVER_GROUP_TYPE_SLAVE_COUNT = NTL_SERVER_GROUP_TYPE_SLAVE_LAST - NTL_SERVER_GROUP_TYPE_SLAVE_FIRST + 1
};

enum eDBO_SERVER_STATUS
{
	DBO_SERVER_STATUS_UP = 0,			//정상 동작
	DBO_SERVER_STATUS_DOWN,				//점검중
	DBO_SERVER_STATUS_LOCKED,			//잠김
	DBO_SERVER_STATUS_OPEN
};



enum eDBO_SERVER_RUNNING_STATE
{
	DBO_SERVER_RUNNING_STATE_NOT_RUNNING,
	DBO_SERVER_RUNNING_STATE_STARTING_UP,
	DBO_SERVER_RUNNING_STATE_RUNNING,
	DBO_SERVER_RUNNING_STATE_SHUTTING_DOWN,
	DBO_SERVER_RUNNING_STATE_SHUTDOWN,
	DBO_SERVER_RUNNING_STATE_COUNT,
	DBO_SERVER_RUNNING_STATE_FIRST = DBO_SERVER_RUNNING_STATE_NOT_RUNNING,
	DBO_SERVER_RUNNING_STATE_LAST = DBO_SERVER_RUNNING_STATE_COUNT - 1,
};

enum eDBO_GAME_SERVER_ROLE
{
	DBO_GAME_SERVER_ROLE_GENERAL = 0,
	DBO_GAME_SERVER_ROLE_TENKAICHI_BODOKAI,

	DBO_GAME_SERVER_ROLE_COUNT
};


enum eDBO_STATS_SEX_RANGE
{
	DBO_STATS_SEX_RANGE_MALE = 0,		// Male
	DBO_STATS_SEX_RANGE_FEMALE,			// Female

	DBO_STATS_SEX_RANGE_COUNT,

	DBO_STATS_SEX_RANGE_FIRST = DBO_STATS_SEX_RANGE_MALE,
	DBO_STATS_SEX_RANGE_LAST = DBO_STATS_SEX_RANGE_COUNT - 1
};

enum eDBO_STATS_LEVEL_RANGE
{
	DBO_STATS_LEVEL_RANGE_10 = 0,		//  1 ~ 10
	DBO_STATS_LEVEL_RANGE_20,			// 11 ~ 20
	DBO_STATS_LEVEL_RANGE_30,			// 21 ~ 30
	DBO_STATS_LEVEL_RANGE_40,			// 31 ~ 40
	DBO_STATS_LEVEL_RANGE_50,			// 41 ~

	DBO_STATS_LEVEL_RANGE_COUNT,

	DBO_STATS_LEVEL_RANGE_FIRST = DBO_STATS_LEVEL_RANGE_10,
	DBO_STATS_LEVEL_RANGE_LAST = DBO_STATS_LEVEL_RANGE_COUNT - 1
};

enum eDBO_STATS_AGE_RANGE
{
	DBO_STATS_AGE_RANGE_10 = 0,		//  1 ~ 10
	DBO_STATS_AGE_RANGE_20,			// 11 ~ 20
	DBO_STATS_AGE_RANGE_30,			// 21 ~ 30
	DBO_STATS_AGE_RANGE_40,			// 31 ~ 40
	DBO_STATS_AGE_RANGE_50,			// 41 ~ 50
	DBO_STATS_AGE_RANGE_60,			// 51 ~

	DBO_STATS_AGE_RANGE_COUNT,

	DBO_STATS_AGE_RANGE_FIRST = DBO_STATS_AGE_RANGE_10,
	DBO_STATS_AGE_RANGE_LAST = DBO_STATS_AGE_RANGE_COUNT - 1
};

#define DBO_MAX_CHARACTER_SERVER_COUNT			(10)
#define DBO_MAX_WORLD_COUNT_IN_INFO_PACKET		(100)
#define DBO_MAX_ADAPTER_ADDRESS_LENGTH			(6)

#pragma pack(1)

struct sSERVER_INFO
{
	char		szCharacterServerIP[NTL_MAX_LENGTH_OF_IP + 1];
	WORD		wCharacterServerPortForClient;
	DWORD		dwLoad;
	BYTE		serverfarmID;
	BYTE		serverchannelID;
};

struct sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO
{
	sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO()
	{
		::ZeroMemory(adwPlayerCountBySex, sizeof(adwPlayerCountBySex));
		::ZeroMemory(adwPlayerCountByRace, sizeof(adwPlayerCountByRace));
		::ZeroMemory(adwPlayerCountByLevel, sizeof(adwPlayerCountByLevel));
		::ZeroMemory(adwPlayerCountByAge, sizeof(adwPlayerCountByAge));
	}

	sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO& operator+=(const sDBO_SERVER_CHANNEL_CONNECTED_USER_INFO& rDiff)
	{
		for (BYTE byIndex = 0; byIndex < DBO_STATS_SEX_RANGE_COUNT; byIndex++)
		{
			adwPlayerCountBySex[byIndex] += (rDiff.adwPlayerCountBySex)[byIndex];
		}
		for (BYTE byIndex = 0; byIndex < RACE_COUNT; byIndex++)
		{
			adwPlayerCountByRace[byIndex] += (rDiff.adwPlayerCountByRace)[byIndex];
		}
		for (BYTE byIndex = 0; byIndex < DBO_STATS_LEVEL_RANGE_COUNT; byIndex++)
		{
			adwPlayerCountByLevel[byIndex] += (rDiff.adwPlayerCountByLevel)[byIndex];
		}
		for (BYTE byIndex = 0; byIndex < DBO_STATS_AGE_RANGE_COUNT; byIndex++)
		{
			adwPlayerCountByAge[byIndex] += (rDiff.adwPlayerCountByAge)[byIndex];
		}

		return *this;
	}

	DWORD GetPlayerCountTotal()
	{
		DWORD dwTotal = 0;

		for (BYTE byIndex = 0; byIndex < DBO_STATS_SEX_RANGE_COUNT; byIndex++)
		{
			dwTotal += adwPlayerCountBySex[byIndex];
		}

		return dwTotal;
	}

	DWORD				adwPlayerCountBySex[DBO_STATS_SEX_RANGE_COUNT];
	DWORD				adwPlayerCountByRace[RACE_COUNT];
	DWORD				adwPlayerCountByLevel[DBO_STATS_LEVEL_RANGE_COUNT];
	DWORD				adwPlayerCountByAge[DBO_STATS_AGE_RANGE_COUNT];
};

struct _CHANNEL_BUFF
{
	BYTE	byRaceBitFlag;
	BYTE	byMinLevel;
	BYTE	byMaxLevel;
	BYTE	byBonusMoveSpeed;
	BYTE	byBonusVehicleMoveSpeed;
	BYTE	byBonusStat;
	WCHAR	wszServerChannelName[NTL_MAX_SIZE_SERVER_CHANNEL_NAME_UNICODE + 1];
	char	szServerChannelName[NTL_MAX_SIZE_SERVER_CHANNEL_NAME_UNICODE + 1];
};

struct sNTLSERVERINFO
{
	sNTLSERVERINFO() :
			byServerType(NTL_SERVER_TYPE_INVALID),
			serverFarmId(INVALID_SERVERFARMID),
			byServerChannelIndex(INVALID_SERVERCHANNELID),
			byServerIndex(DEFAULT_SERVERINDEX),
			wPortForClient(0),
			wPortForAuthServer(0),
			wPortForCharacterServer(0),
			wPortForGameServer(0),
			wPortForQueryServer(0),
			wPortForCommunityServer(0),
			byGameServerRole(DBO_GAME_SERVER_ROLE_GENERAL)
	{
		::ZeroMemory(achPublicAddress, sizeof(achPublicAddress));
		::ZeroMemory(achInternalAddress, sizeof(achInternalAddress));
	};

	sNTLSERVERINFO(sNTLSERVERINFO* pNtlServerInfo)
	{
		::CopyMemory(this, pNtlServerInfo, sizeof(*this));
	};

	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;

	char				achPublicAddress[NTL_MAX_LENGTH_OF_IP + 1];
	char				achInternalAddress[NTL_MAX_LENGTH_OF_IP + 1];

	WORD				wPortForClient;
	WORD				wPortForAuthServer;
	WORD				wPortForCharacterServer;
	WORD				wPortForGameServer;
	WORD				wPortForQueryServer;
	WORD				wPortForCommunityServer;
	BYTE				byGameServerRole;		// eDBO_GAME_SERVER_ROLE
};

struct sDBO_SERVER_INFO : public sNTLSERVERINFO
{
	sDBO_SERVER_INFO() :
			sNTLSERVERINFO(),
			bIsOn(false),
			byRunningState(DBO_SERVER_RUNNING_STATE_NOT_RUNNING),
			dwMaxLoad(0),
			dwLoad(0),
			dwProcessUsage(0),
			dwSystemUsage(0),
			ui64MemoryUsage(0),
			dwRecvQuery(0),
			dwEndQuery(0),
			dwCurQuery(0),
			dwPing(0),
			bIsOnAgent(false),
			lastUpdatedTime(0)
	{
	};

	sDBO_SERVER_INFO(sDBO_SERVER_INFO* pServerInfo) :
			sNTLSERVERINFO(pServerInfo),
			bIsOn(pServerInfo->bIsOn),
			byRunningState(pServerInfo->byRunningState),
			dwMaxLoad(pServerInfo->dwMaxLoad),
			dwLoad(pServerInfo->dwLoad),
			dwProcessUsage(pServerInfo->dwProcessUsage),
			dwSystemUsage(pServerInfo->dwSystemUsage),
			ui64MemoryUsage(pServerInfo->ui64MemoryUsage),
			dwRecvQuery(pServerInfo->dwRecvQuery),
			dwEndQuery(pServerInfo->dwEndQuery),
			dwCurQuery(pServerInfo->dwCurQuery),
			dwPing(pServerInfo->dwPing),
			bIsOnAgent(pServerInfo->bIsOnAgent),
			lastUpdatedTime(pServerInfo->lastUpdatedTime)
			
	{
	};

	sDBO_SERVER_INFO& operator=(sDBO_SERVER_INFO& rServerInfo)
	{
		memcpy(this, &rServerInfo, sizeof(sDBO_SERVER_INFO));
		return (*this);
	}

	bool bIsOn;
	BYTE byRunningState;
	DWORD dwMaxLoad;
	DWORD dwLoad;
	DWORD dwProcessUsage;
	DWORD dwSystemUsage;
	UINT64 ui64MemoryUsage;
	DWORD dwRecvQuery;
	DWORD dwEndQuery;
	DWORD dwCurQuery;
	DWORD dwPing;
	bool bIsOnAgent;
	time_t lastUpdatedTime;
	
};

struct sDBO_AUTH_SERVER_FARM_INFO
{
	sDBO_AUTH_SERVER_FARM_INFO() :
			byServerStatus(DBO_SERVER_STATUS_DOWN),
			dwMaxLoad(0),
			dwLoad(0)
	{
	};

	BYTE			byServerStatus;		// eDBO_SERVER_STATUS
	DWORD			dwMaxLoad;
	DWORD			dwLoad;
};

struct sAUTH_SERVER_FARM_INFO : public sDBO_AUTH_SERVER_FARM_INFO
{
	sAUTH_SERVER_FARM_INFO() :
		sDBO_AUTH_SERVER_FARM_INFO(),
		bIsLocked(false)
	{
		m_listAuthServerInfoRef.clear();
	};

	bool bIsLocked;
	std::list<sDBO_SERVER_INFO*>		m_listAuthServerInfoRef;
};

struct sDBO_CHARACTER_SERVER_FARM_INFO
{
	sDBO_CHARACTER_SERVER_FARM_INFO() :
			byServerStatus(DBO_SERVER_STATUS_DOWN),
			dwMaxLoad(0),
			dwLoad(0)
	{
	};

	BYTE			byServerStatus;	// eDBO_SERVER_STATUS
	DWORD			dwMaxLoad;
	DWORD			dwLoad;
};

struct sCHARACTER_SERVER_FARM_INFO : public sDBO_CHARACTER_SERVER_FARM_INFO
{
	sCHARACTER_SERVER_FARM_INFO() :
		sDBO_CHARACTER_SERVER_FARM_INFO(),
		bIsLocked(false)
	{
		m_listCharacterServerInfoRef.clear();
	};

	bool bIsLocked;
	std::list<sDBO_SERVER_INFO*>		m_listCharacterServerInfoRef;
};

struct sDBO_GAME_SERVER_FARM_INFO
{
	sDBO_GAME_SERVER_FARM_INFO() :
		serverFarmId(INVALID_SERVERFARMID),
		byServerStatus(DBO_SERVER_STATUS_DOWN),
		dwMaxLoad(0),
		dwLoad(0)
	{
		::ZeroMemory(wszGameServerFarmName, sizeof(wszGameServerFarmName));
	};

	SERVERFARMID serverFarmId;
	WCHAR wszGameServerFarmName[NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1];
	BYTE byServerStatus;		// eDBO_SERVER_STATUS
	DWORD dwMaxLoad;
	DWORD dwLoad;
};

struct sDBO_GAME_SERVER_CHANNEL_INFO
{
	sDBO_GAME_SERVER_CHANNEL_INFO() :
			serverFarmId(INVALID_SERVERFARMID),
			byServerChannelIndex(INVALID_SERVERINDEX),
			bIsVisible(false),
			byServerStatus(DBO_SERVER_STATUS_DOWN),
			dwMaxLoad(0),
			dwLoad(0),
			bIsScrambleChannel(false),
			bUnknown(false)
	{
		::ZeroMemory(&sChannelBuff, sizeof(sChannelBuff));
	};

	SERVERFARMID serverFarmId;
	SERVERCHANNELID byServerChannelIndex; //serverChannelId
	bool bIsVisible;
	BYTE byServerStatus;		// eDBO_SERVER_STATUS
	DWORD dwMaxLoad;
	DWORD dwLoad;
	bool bIsScrambleChannel;
	_CHANNEL_BUFF sChannelBuff;
	bool bUnknown;
};

struct sSERVER_CHANNEL_INFO : public sDBO_GAME_SERVER_CHANNEL_INFO
{
	sSERVER_CHANNEL_INFO(sDBO_GAME_SERVER_CHANNEL_INFO *pServerChannelInfo) :
		bIsLocked(false)
	{
		serverFarmId = pServerChannelInfo->serverFarmId;
		byServerChannelIndex = pServerChannelInfo->byServerChannelIndex;
		bIsVisible = pServerChannelInfo->bIsVisible;
		byServerStatus = pServerChannelInfo->byServerStatus;
		dwMaxLoad = pServerChannelInfo->dwMaxLoad;
		dwLoad = pServerChannelInfo->dwLoad;
		bIsScrambleChannel = pServerChannelInfo->bIsScrambleChannel;
		memcpy(&sChannelBuff, &pServerChannelInfo->sChannelBuff, sizeof(_CHANNEL_BUFF));
		bUnknown = pServerChannelInfo->bUnknown;

		listGameServerInfoRef.clear();
		listNpcServerInfoRef.clear();
	};

	sSERVER_CHANNEL_INFO& operator=(sDBO_GAME_SERVER_CHANNEL_INFO& rServerChannelInfo)
	{
		serverFarmId = rServerChannelInfo.serverFarmId;
		byServerChannelIndex = rServerChannelInfo.byServerChannelIndex;
		bIsVisible = rServerChannelInfo.bIsVisible;
		byServerStatus = rServerChannelInfo.byServerStatus;
		dwMaxLoad = rServerChannelInfo.dwMaxLoad;
		dwLoad = rServerChannelInfo.dwLoad;
		bIsScrambleChannel = rServerChannelInfo.bIsScrambleChannel;
		memcpy(&sChannelBuff, &rServerChannelInfo.sChannelBuff, sizeof(_CHANNEL_BUFF));
		bUnknown = rServerChannelInfo.bUnknown;

		return (*this);
	};

	bool												bIsLocked;
	std::list<sDBO_SERVER_INFO*>						listGameServerInfoRef; //always contain only one input for this channel.
	std::list<sDBO_SERVER_INFO*>						listNpcServerInfoRef;
	DWORD												connectedUserInfo;
};


struct sSERVER_FARM_INFO : public sDBO_GAME_SERVER_FARM_INFO
{
	sSERVER_FARM_INFO(sDBO_GAME_SERVER_FARM_INFO *pGameServerFarmInfo) :
		bIsLocked(false)
	{
		serverFarmId = pGameServerFarmInfo->serverFarmId;
		wcscpy_s(wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1, pGameServerFarmInfo->wszGameServerFarmName);
		byServerStatus = pGameServerFarmInfo->byServerStatus;
		dwMaxLoad = pGameServerFarmInfo->dwMaxLoad;
		dwLoad = pGameServerFarmInfo->dwLoad;

		mapGameServerChannelInfo.clear();
		listCommunityServerInfoRef.clear();
		listQueryServerInfoRef.clear();
	};

	sSERVER_FARM_INFO& operator = (sDBO_GAME_SERVER_FARM_INFO& rServerFarmInfo)
	{
		serverFarmId = rServerFarmInfo.serverFarmId;
		wcscpy_s(wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1, rServerFarmInfo.wszGameServerFarmName);
		byServerStatus = rServerFarmInfo.byServerStatus;
		dwMaxLoad = rServerFarmInfo.dwMaxLoad;
		dwLoad = rServerFarmInfo.dwLoad;

		return (*this);
	};

	bool												bIsLocked;
	std::map<SERVERCHANNELID, sSERVER_CHANNEL_INFO*>	mapGameServerChannelInfo;
	std::list<sDBO_SERVER_INFO*>						listCommunityServerInfoRef;
	std::list<sDBO_SERVER_INFO*>						listQueryServerInfoRef;
};

struct sDBO_SERVER_LOCK_INFO
{
	sDBO_SERVER_LOCK_INFO() :
		byServerType(NTL_SERVER_TYPE_INVALID),
		serverFarmId(INVALID_SERVERFARMID),
		byServerChannelIndex(INVALID_SLOTID),
		bIsLock(false)
	{
	};
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	bool				bIsLock;
};

struct _SERVER_PAIR_ID
{
	_SERVER_PAIR_ID(SERVERFARMID farmId, SERVERCHANNELID channelId) :
		serverFarmId(farmId),
		serverChannelId(channelId)
	{
	};

	bool operator < (const _SERVER_PAIR_ID& pair) const
	{
		return ((pair.serverFarmId < serverFarmId) || (pair.serverChannelId < serverChannelId));
	}

	bool operator == (const _SERVER_PAIR_ID& pair) const
	{
		return pair.serverFarmId == serverFarmId && pair.serverChannelId == serverChannelId;
	}

	SERVERFARMID	serverFarmId;
	SERVERCHANNELID	serverChannelId;
};

#pragma pack()