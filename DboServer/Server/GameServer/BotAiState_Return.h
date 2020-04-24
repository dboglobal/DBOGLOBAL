#ifndef __AI_DBOG_BOT_STATE_RETURN_H__
#define __AI_DBOG_BOT_STATE_RETURN_H__

#include "BotAiState.h"

class CBotAiState_Return : public CBotAiState
{

public:

	CBotAiState_Return(CNpc* pBot, eBOT_RETURNMODE eReturnMode, CNtlVector& rReturnLoc, CNtlVector& rReturnDir);
	virtual	~CBotAiState_Return();

private:

	void	Init();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


public:

	/*OnUpdate_ComebackRun;
	OnUpdate_NormalWalk;
	OnUpdate_NormalRun;
	Soloist_ComebackRun;
	Party_ComebackRun;
	Soloist_Normal;
	Party_Normal;
	Move;
	Action_ComebackRun;*/

private:

	CNtlVector m_vReturnLoc;
	CNtlVector m_vReturnDir;
	eBOT_RETURNMODE m_eReturnMode;
	bool m_IsBotInit;
	QWORD m_qwCharConditionFlag;
	float m_fRunSpeed;
	float m_fWalkSpeed;
	bool m_bIsScan;

};

#endif