#ifndef __INC_DBOG_ESCORTMANAGER_H__
#define __INC_DBOG_ESCORTMANAGER_H__


class CCharacter;

class CEscortManager
{

public:

	struct SEscortTarget
	{
		HOBJECT hPc;
		CHARACTERID charId;
	};

public:
	CEscortManager();
	virtual	~CEscortManager();

public:

	bool				Create(CCharacter* pBot);
	void				TickProcess(DWORD dwTickTime, float fMultiple);

	bool				AddTarget(bool bShared, eESCORT_TYPE eEscortType, HOBJECT handle, CHARACTERID charId, PARTYID partyID, BYTE byTriggerType, WORD tid, WORD *pwResultCode);
	void				RemoveTarget(HOBJECT handle);

	void				Clear();

	CCharacter*			GetCurrentTarget();
	BYTE				GetTargetCount();
	HOBJECT				GetTargetWithSlot(BYTE byPos);
	CHARACTERID			GetTargetIdWithSlot(BYTE byPos);

	void				SendEvent(BYTE byEventType, BYTE byTriggerType, DWORD dwEventID);

	bool				IsEscortRun();

protected:

	void				Init();
	void				RemoveMemberWithSlot(BYTE byPos);
	BYTE				FindTarget(HOBJECT handle); //return slot


private:

	bool			m_bShared;

	eESCORT_TYPE	m_eEscortType;
	BYTE			m_byEscortTargetCount;
	SEscortTarget	m_ahEscortTarget[5];
	BYTE			m_byCurrentTargetSlot;

	PARTYID			m_escortPartyID;

	BYTE			m_byTriggerType;
	WORD			m_tid;

	CCharacter*		m_pParent;

};

#endif
