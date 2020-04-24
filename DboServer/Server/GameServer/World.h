#ifndef __WORLD_H__
#define __WORLD_H__

#include "NtlWorld.h"
#include "WorldTable.h"
#include "WorldZoneTable.h"
#include "WorldObjectList.h"
#include "NpcPartyManager.h"


class CObjectMsg;
class CWorldCell;
class CSpawnObject;
class CNtlPacket;
class CWorldZone;
class CTriggerObject;
class CNpc;
class CScriptAlgoObject;
class CTqsAlgoObject;
class CPlayer;
enum eSSD_SCRIPT_TYPE;

class CWorld
{

public:

	CWorld();
	virtual ~CWorld();

protected:

	void					Init();

public:

	virtual int				Create(WORLDID worldID, sWORLD_TBLDAT* pTbldat, CWorldZoneTable* pWorldZoneTable);
	virtual void			Destroy();

	virtual void			TickProcess(DWORD dwTickDiff, float fMultiple);

	virtual int				OnObjectMsg(CObjectMsg* pObjMsg);

	virtual int				Enter(CSpawnObject* pSpawnObject, bool bIsServerStart = false);
	virtual int				Leave(CSpawnObject* pSpawnObject);

	virtual void			OnPreCreate();
	virtual void			OnCreate();
	virtual void			OnDestroy();

	void					SpawnNpcMob();
	void					DespawnNpcMob();

	virtual int				CopyToInfo(sWORLD_INFO* pWorldInfo);

	virtual bool			CanDoBattle(CSpawnObject* pSubject, CSpawnObject* pTarget);
	virtual bool			CanMoveTo(CNtlVector& rLoc, CSpawnObject* pObject);
	virtual bool			IsSwimmingArea(CNtlVector& rLoc);
	virtual bool			CanFly();

	int						Split(sWORLD_TBLDAT* pTbldat);
	void					AdjustLocation(CNtlVector& rLocation);
	bool					IsInBoundary(CNtlVector& rLoc);

	virtual void			Broadcast(CNtlPacket* pPacket, CSpawnObject* pSubject, float fRange);
	virtual void			Broadcast(CNtlPacket* pPacket, CSpawnObject* pExcept = NULL);

	void					AddEventIdToAllPlayers(DWORD dwEventID);
	void					AddEventIdToAllPlayers(DWORD dwEventID, CSpawnObject* pSubject, float fRange);

	bool					HasPlayer();

	WORLDID					GetID();
	TBLIDX					GetIdx();
	const char*				GetName();
	eGAMERULE_TYPE			GetRuleType();

	virtual CWorldCell*		GetWorldCell(CELLID cellID);
	CWorldCell*				GetWorldCellWithLoc(CNtlVector& rhs);
	CELLID					GetCellID(CNtlVector& rhs);
	void					GetCellStartPos(CELLID cellID, CNtlVector* pvStart);
	void					GetCellEndPos(CELLID cellID, CNtlVector* pvEnd);
	DWORD					GetCellHorizontalCount();
	DWORD					GetCellVerticalCount();
	DWORD					GetCellTotalCount();

	float					GetSplitSize();

	CWorldObjectList*		GetObjectList();

	sWORLD_TBLDAT*			GetTbldat();

	CWorldZone*				FindWorldZone(ZONEID zoneId);

	void*					GetNaviInstanceHandle();

	DWORD					GetZoneTotalCount();

	CTriggerObject*			FindStaticObject(HOBJECT id);
	CTriggerObject*			FindStaticObjectByIdx(TBLIDX tblidx);

	CNpc*					FindNpc(TBLIDX tblidx);

	CNpcPartyManager*		GetNpcPartyManager() { return &m_partyManager; }

	inline HOBJECT			GetTiggerObjectOffset() { return m_hTriggerObjectOffset; }

public:

	bool					AddScript(CScriptAlgoObject* pScript);
	void					RemScript(DWORD dwScriptID);
	CScriptAlgoObject*		GetScript(DWORD dwScriptID);
	CScriptAlgoObject*		GetFirstScript(eSSD_SCRIPT_TYPE scripType);

	void					AddScriptToPlayer(CPlayer* pPlayer);

	void					RemAllScript();

private:

	std::map<DWORD, CScriptAlgoObject*> m_mapScript;

protected:

	bool					CreateZoneMap(CWorldZoneTable *pWorldZoneTable);

	void					CreateWorldCellArray(DWORD dwTotalCount);
	void					DestroyWorldCellArray();

	virtual CWorldZone*		CreateWorldZone(sWORLD_ZONE_TBLDAT *pWorldZoneData, CWorld *pWorldRef);

	bool					BroadcastPacket(CNtlPacket *pPacket, CSpawnObject *pSubject, float fRange);
	bool					BroadcastPacket(CNtlPacket *pPacket, CSpawnObject *pExcept);


private:

	typedef std::map<ZONEID, CWorldZone*> ZONEMAP;
	typedef ZONEMAP::value_type ZONEVAL;

	WORLDID				m_worldID;
	sWORLD_TBLDAT*		m_pTbldat;
	CWorldCell*			m_paWorldCell;

	CNtlVector			m_vStart;
	CNtlVector			m_vEnd;

	int					m_nStX;
	int					m_nStY;
	int					m_nEdX;
	int					m_nEdY;

	CNtlVector			m_vStartBoundary;
	CNtlVector			m_vEndBoundary;

	HOBJECT				m_hTriggerObjectOffset;
	std::map<HOBJECT, CTriggerObject*>		m_map_TriggerObj;

	CNtlVector			m_vWorldSize;

	DWORD				m_dwCellHorizontalCount;
	DWORD				m_dwCellVerticalCount;
	DWORD				m_dwTotalCount;

	CWorldObjectList	m_objectList;

	ZONEMAP				m_mapZone;

	void*				m_hNaviInstance;

	CNpcPartyManager	m_partyManager;

};


#endif