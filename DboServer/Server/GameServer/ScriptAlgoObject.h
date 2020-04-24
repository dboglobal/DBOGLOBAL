#ifndef __SCRIPT_ALGO_DBOG_OBJECT_H__
#define __SCRIPT_ALGO_DBOG_OBJECT_H__


#include "NtlSharedType.h"
#include "NtlLinkList.h"
#include "ControlScriptEventMap.h"
#include "ScriptAlgoController.h"
#include <vector>


class CNpc;
class CMonster;
class CWorld;
class CPlayer;
class CNtlPacket;

class CScriptAlgoObject
{

public:

	typedef std::multimap<SPAWNGROUPID, HOBJECT> SPAWNGROUP;
	typedef SPAWNGROUP::iterator SPAWNGROUP_IT;

public:

	CScriptAlgoObject(TBLIDX tblidx, DWORD dwHandle, eSSD_SCRIPT_TYPE scripType);
	virtual	~CScriptAlgoObject();

protected:

	void					Init();

public:

	virtual void			Start();
	virtual void			Stop(bool bEnterFailStage = false, bool bRemoveFromWorld = true); // !IMPORTANT! ONLY CALL BY MAIN-THREAD

	virtual int				OnUpdate(DWORD dwTickDiff);
	int						OnObjectMsg(CObjectMsg * pObjMsg);

public:

	CScriptAlgoController*	GetController() { return &m_scriptAlgoController; }

	inline void					SetPaused(bool bFlag) { m_bPaused = bFlag; }
	inline bool					IsPaused() { return m_bPaused; }

	inline void					SetStopped(bool bFlag) { m_bIsStopped = bFlag; }
	inline bool					IsStopped() { return m_bIsStopped; }

	DWORD					GetScriptID() { return m_ScriptID; }

	CNpc*					FindNpc(TBLIDX tblidx);
	void					AddNpc(CNpc* pNpc);
	void					RemoveNpc(TBLIDX npcTblidx);

	void					AddSpawnGroupMob(SPAWNGROUPID spawnGroupId, HOBJECT hMonster);
	HOBJECT					GetSpawnGroupFirst(SPAWNGROUPID spawnGroupId);
	HOBJECT					GetSpawnGroupNext(SPAWNGROUPID spawnGroupId);
	DWORD					GetSpawnGroupCount(SPAWNGROUPID spawnGroupId);

	SPAWNGROUP_IT			GetMobSpawnBegin() { return m_spawnGroup.begin(); }
	SPAWNGROUP_IT			GetMobSpawnEnd() { return m_spawnGroup.end(); }

	DWORD					GetRemovedSpawnGroupCount(SPAWNGROUPID spawnGroupId);

	void					RemoveMob(CMonster* pMob);


	void					SetWorld(CWorld* pWorld) { m_pWorld = pWorld; }
	CWorld*					GetWorld() { return m_pWorld; }

	CControlScriptEventMap*	GetEventMap() { return &m_cEventMap; }

	void					AddPlayer(HOBJECT hPlayer);
	CPlayer*				GetPlayersFirst();
	CPlayer*				GetPlayersNext();
	DWORD					GetPlayersCount();

	DWORD					GetHandle() { return m_dwHandle; }

	eSSD_SCRIPT_TYPE		GetScriptType() { return m_eScriptType; }

	void					SetStartedByQuest(bool bFlag) { m_bStartedByQuest = bFlag; }

	void					Broadcast(CNtlPacket* pPacket);

	void					SendPartyLeader(CNtlPacket* pPacket);

	void					QuestFailed();

public:

	virtual bool			NotifyPlayerLeave(HOBJECT hPlayer, bool bRemoveScriptFromPlayer = true);

protected:

	CScriptAlgoController		m_scriptAlgoController;

	DWORD						m_ScriptID;

	eSSD_SCRIPT_TYPE			m_eScriptType;

	DWORD						m_dwHandle;

	bool						m_bPaused;
	bool						m_bIsStopped;

	std::map<TBLIDX, HOBJECT>	m_map_Npc;

	CWorld*						m_pWorld;		// 100% required else cant spawn mob etc

	CControlScriptEventMap		m_cEventMap;


	typedef std::set<HOBJECT>	PLAYERSET;
	typedef PLAYERSET::iterator	PLAYERSET_IT;

	PLAYERSET					m_setPlayers;
	PLAYERSET_IT				m_setPlayersItBeg;
	PLAYERSET_IT				m_setPlayersItEnd;

	SPAWNGROUP					m_spawnGroup;
	SPAWNGROUP_IT				m_spawnGroupItBeg;
	SPAWNGROUP_IT				m_spawnGroupItEnd;

	std::vector<SPAWNGROUPID>	m_vecRemovedSpawnGroup;

	bool						m_bStartedByQuest;
};

#endif