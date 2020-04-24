#pragma once

#include "NtlTimeQuest.h"
#include "TimeQuestTable.h"

class CPlayer;
class CWorld;
class CTqsAlgoObject;

class CTimeQuest
{

public:

	CTimeQuest(PARTYID partyid, sTIMEQUEST_TBLDAT* pTimeQuestTbldat, BYTE byDifficulty, BYTE byMode);
	virtual ~CTimeQuest();


protected:

	void						Initalize();

public:

	bool						TickProcess(DWORD dwTickDiff); //return true if ends

	bool						Create(CPlayer* pPlayer);

public:

	PARTYID						GetPartyID() { return m_partyId; }

	sTIMEQUEST_TBLDAT*			GetTmqTable() { return m_pTimeQuestTbldat; }

	CWorld*						GetWorld() { return m_pWorld; }

	CTqsAlgoObject*				GetTQS() { return m_pScript; }

	BYTE						GetMode() { return m_ruleInfo.byTimeQuestMode; }

	BYTE						GetState() { return m_ruleInfo.sTimeQuestState.byGameState; }
	void						SetState(BYTE byState) { m_ruleInfo.sTimeQuestState.byGameState = byState; }

	void						SetStage(BYTE byStage) { m_ruleInfo.sTimeQuestState.sTimeQuestStage.byStageNumber = byStage; }

	void						SetCountdownStart(DBOTIME time) { m_timeCountdownStart = time; }
	void						SetCountdownEnd(DBOTIME time) { m_timeCountdownEnd = time; }

	void						DecreaseCoupon() { --m_byCoupons; }
	BYTE						GetCoupons() { return m_byCoupons; }

	void						UpdateCoupon();

public:

	void						CopyRuleInfoTo(sTIMEQUEST_RULE_INFO* pRuleInfo);

	void						AddMember(CPlayer* player);
	
	void						RemoveMember(HOBJECT hHandle);

	void						UpdateTmqPoints();

	void						UpdateTmqFail();

private:

	PARTYID						m_partyId;

	CHARACTERID					memberCharId[DBO_TIMEQUEST_MAX_MEMBER_COUNT];

	sTIMEQUEST_TBLDAT*			m_pTimeQuestTbldat;

	sTIMEQUEST_RULE_INFO		m_ruleInfo;

	CWorld*						m_pWorld;

	CTqsAlgoObject*				m_pScript;

	std::map<HOBJECT, CPlayer*>	m_mapPlayers;

	DWORD						m_dwWaitTime;

	WORD						m_wTmqPoints;

	BYTE						m_byCoupons;

	DBOTIME						m_timeCountdownStart;
	DBOTIME						m_timeCountdownEnd;

	DWORD						m_dwLeaveTickCount;
	DWORD						m_dwExitTickCount;

};