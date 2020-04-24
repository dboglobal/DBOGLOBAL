#ifndef __AI_DBOG_BOTACTION_DESTMOVE_H__
#define __AI_DBOG_BOTACTION_DESTMOVE_H__

#include "BotAiAction.h"

class CBotAiAction_DestMove : public CBotAiAction
{

public:

	CBotAiAction_DestMove(CNpc* pBot, BYTE actionPatternIndex, BYTE byDestLocCount, sVECTOR3* pavDestLoc, bool bRunMode, bool bHaveSecondDestLoc, CNtlVector& rSecondDestLoc);
	virtual	~CBotAiAction_DestMove();

public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	BYTE		m_actionPatternIndex;
	BYTE		m_byDestLocCount;

	sVECTOR3	m_avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
	CNtlVector	m_vSecondDestLoc;

	bool		m_bRunMode;
	bool		m_bHaveSecondDestLoc;

};

#endif