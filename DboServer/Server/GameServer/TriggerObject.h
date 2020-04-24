#ifndef __SERVER_TRIGGER_OBJECT__
#define __SERVER_TRIGGER_OBJECT__


#include "ObjectTable.h"
#include "SpawnObject.h"


class CCharacter;
class CWorld;
class CWorldCell;
class CNtlPacket;
class CPlayer;

class CTriggerObject : public CSpawnObject
{
	struct sTRIGGER_EXECUTER
	{
		void Init()
		{
			hPc = INVALID_HOBJECT;
			dwStartTickCount = INVALID_DWORD;
			dwWaitTick = INVALID_DWORD;
			questID = INVALID_QUESTID;
			pTSTrigger = NULL;
			tcID = NTL_TS_TC_ID_INVALID;
			taID = NTL_TS_TA_ID_INVALID;
		}

		HOBJECT			hPc;				// PC handler
		QWORD			dwStartTickCount;	// GetTickCount64 when pc start executing this object
		DWORD			dwWaitTick;			// Tick until next process
		QUESTID			questID;			// Has a quest attached to it?
		CNtlTSTrigger*	pTSTrigger;			// TS File
		NTL_TS_TC_ID	tcID;				// Currrent trigger container id
		NTL_TS_TA_ID	taID;				// Current trigger action id
	};


public:

	CTriggerObject();
	virtual ~CTriggerObject();

protected:

	void						Initialize();

public:

	void						Create(HOBJECT hHandle);
	void						SetToObjTable(sOBJECT_TBLDAT* pObjTblData);

	virtual void				TickProcess(DWORD dwTickDiff);

	virtual void				CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID);

public:

	void						UpdateToObjectState(BYTE byMainState, BYTE bySubStateFlag, DWORD dwTime);

	virtual void				EncodeInsertPacket(CPlayer* pPlayer);

public:
	
	inline BYTE					GetMainState() { return tobjectState.byState; }

	inline BYTE					GetSubState()  { return tobjectState.bySubStateFlag; }

	inline DWORD				GetStateTime()  { return tobjectState.dwStateTime; }

public:

	sOBJECT_TBLDAT*				GetTbldat() { return m_pObjTbldat; }

	inline TBLIDX				GetTblidx() { return m_pObjTbldat->tblidx; }

	inline CNtlVector&			GetCurLoc() { return m_pObjTbldat->vLoc; }

	inline BYTE					GetStateType() { return m_pObjTbldat->byStateType; }

	inline TBLIDX				GetContent() { return m_pObjTbldat->contentsTblidx; }

	inline ZONEID				GetZone() { return m_pObjTbldat->ZoneId; }

	inline DWORD				GetFunc() { return m_pObjTbldat->dwFunction; }

public:

	bool						IsBeingExecuted() { return m_bIsExecuting; }

	bool						StartExecuting(CPlayer* pPlayer, bool bIsQuestIdTriggerId, QUESTID questId = INVALID_QUESTID);
	int							OnStartExecuting(CNtlTSTrigger* pEntity, CPlayer* pPlayer, QUESTID questId);

	WORD						ProcessTsEntity(CNtlTSEntity* pEntity, CPlayer* pPlayer);

	void						StopExecuting(bool bReset, bool bUpdate);


private:
	sOBJECT_TBLDAT*				m_pObjTbldat;

	sTOBJECT_STATE				tobjectState;

	bool						m_bIsExecuting; //is this object being executed already?

	sTRIGGER_EXECUTER			m_executer;

};


#endif