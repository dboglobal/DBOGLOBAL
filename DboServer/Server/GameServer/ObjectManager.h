#ifndef __SERVER_CHAR_MANAGER__
#define __SERVER_CHAR_MANAGER__

#include "NtlSingleton.h"
#include "NtlPacket.h"
#include "NtlObject.h"
#include <unordered_map>


class CPlayer;
class CGameObject;
class CMonster;
class CNpc;
class CCharacterObject;
class CCharacter;
class CSummonPet;

class CObjectManager : public CNtlSingleton<CObjectManager>
{

public:
	CObjectManager();
	virtual ~CObjectManager();

private:

	HOBJECT								uiSerialId;
	std::vector<HOBJECT>				m_vSerialVec;

	void								SpawnNpcAndMob();

	DWORD								m_dwLpEpEventId;

public:

	bool								Create();
	void								Destroy();

	void								TickProcess(DWORD dwTickDiff, float fMultiple);

	HOBJECT								CreateUID();
	void								DeleteUID(HOBJECT hHandle);
	CGameObject*						CreateCharacter(eOBJTYPE objtype, CHARACTERID CharID = INVALID_CHARACTERID);

	CGameObject*						GetObjectA(HOBJECT hHandle);
	CCharacterObject*					GetCharacterObject(HOBJECT hHandle);
	CCharacter*							GetChar(HOBJECT hHandle);
	CCharacter*							GetChar(eOBJTYPE eObjType, HOBJECT hHandle);
	CPlayer*							GetPC(HOBJECT hHandle);
	CNpc*								GetNpc(HOBJECT hHandle);
	CMonster*							GetMob(HOBJECT hHandle);
	CSummonPet*							GetSummonPet(HOBJECT hHandle);

	//player
	CPlayer*							FindByAccount(ACCOUNTID uiAccId);
	CPlayer*							FindByChar(CHARACTERID uiCharId);
	CPlayer*							FindByName(WCHAR* wszCharName);
	CPlayer*							FindByName(const WCHAR* wszCharName);

	void								DestroyCharacter(CGameObject* ch);

	DWORD								AcquireLpEpEventID() { m_dwLpEpEventId = UnsignedSafeIncrease<DWORD>(m_dwLpEpEventId, 1); return m_dwLpEpEventId; }

	//bot respawn system
	void								AddRespawn(CNpc* pBot);
	void								RemoveSpawn(HOBJECT hHandle);
	void								ProcessRespawn();

	void								ProcessDelayDeleteMap();

	size_t								GetPlayerCount() { return m_map_pkChrByPID.size(); }

	void								SendPacketToAll(CNtlPacket * pPacket);
	bool								DisconnectAll();

	void								SendToAllInZone(ZONEID zoneId, CNtlPacket * pPacket);

private:

	std::unordered_map<HOBJECT, CGameObject*>	m_map_CharByUID;

	std::unordered_map<CHARACTERID, CPlayer*>	m_map_pkChrByPID; // only players in this map
	
	std::unordered_map<CGameObject*, DWORD>		m_map_DelayDelete; //DWORD = gCurTime
	std::unordered_map<CNpc*, DWORD>				m_map_Respawn; //DWORD = m_dwCurTickCount

	CGameObject*						m_objectArray[MAX_GAME_OBJECT]; // used to get objects very fast without iterating a map
};


#define GetCObjectManager()			CObjectManager::GetInstance()
#define g_pObjectManager			GetCObjectManager()


#endif