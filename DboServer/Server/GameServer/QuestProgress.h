#pragma once

#include "DboTSCore.h"

class CPlayer;
class CWpsAlgoObject;

class CQuestProgress
{

public:

	CQuestProgress(CPlayer* pOwner);
	CQuestProgress(CPlayer* pOwner, QUESTID tId, NTL_TS_TC_ID tcId, BYTE bySlot);
	virtual ~CQuestProgress();

	
public:

	void							Init();

	void							TickProcess(DWORD dwTickDiff);


public:

	void							SetProgressInfo(sPROGRESS_QUEST_INFO* pProgress) { memcpy(&m_QuestProgressInfo, pProgress, sizeof(sPROGRESS_QUEST_INFO)); }

	QUESTID							GetQuestID() { return m_QuestProgressInfo.tId; }

	sPROGRESS_QUEST_INFO*			GetProgressInfo() { return &m_QuestProgressInfo; }

	void							SetFailOnDeath(bool bFlag) { m_bFailOnDeath = bFlag; }
	bool							GetFailOnDeath() { return m_bFailOnDeath; }

	void							SetFailOnLogout(bool bFlag) { m_bFailOnPlayerLogout = bFlag; }
	bool							IsFailOnLogout() { return m_bFailOnPlayerLogout; }

	void							SetTimer(CDboTSActETimerS* pAct, int nSlot) { m_pQuestTimer[nSlot] = pAct; }
	CDboTSActETimerS*				GetTimer(int nSlot) { return m_pQuestTimer[nSlot]; }

	void							SetWpsScriptID(DWORD dwWpsScript) { m_dwWpsScript = dwWpsScript; }
	DWORD							GetWpsScriptID() { return m_dwWpsScript; }

	bool							IsInProgressState();

	WORD							UpdateQuestObjectItem(TBLIDX ItemTblidx, BYTE byCount);


public:

	void							OnPlayerDied();
	void							OnWpsFailed();

public:

	void							RunGroup(NTL_TS_TG_ID gId);

private:

	sPROGRESS_QUEST_INFO			m_QuestProgressInfo;

	CPlayer*						m_pOwner;

	bool							m_bFailOnDeath;				//Should quest fail when player die

	bool							m_bFailOnPlayerLogout;		//Quest fails when logout?

	CDboTSActETimerS*				m_pQuestTimer[sEXCEPT_TIMER_SLOT::eTIMER_SLOT_MAX];	//if timer end then TS group X will be executed

	DWORD							m_dwWpsScript;

};