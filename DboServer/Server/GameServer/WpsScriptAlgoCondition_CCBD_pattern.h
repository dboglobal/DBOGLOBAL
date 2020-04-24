#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CCBD_PATTERN_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CCBD_PATTERN_H__

#include "ScriptAlgoCondition.h"


class CWpsScriptAlgoCondition_CCBD_pattern : public CWpsAlgoCondition
{

public:

	CWpsScriptAlgoCondition_CCBD_pattern(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoCondition_CCBD_pattern();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void				OnEnter();

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

	int							Update(DWORD dwTickDiff, float fMultiple);

public:

	BYTE						m_byIndex;
	TBLIDX						m_titleIdx;
	TBLIDX						m_subTitleIdx;
	bool						m_bUseTimeLimit;
	DWORD						m_dwTimeLimit;
	DWORD						m_scriptnodeID;
	bool						m_bPacketSent;

	bool						m_bStartTimeLimit;
	bool						m_bTimeLimitStarted;
	DWORD						m_dwTimeLimitBeginTime;

};

#endif