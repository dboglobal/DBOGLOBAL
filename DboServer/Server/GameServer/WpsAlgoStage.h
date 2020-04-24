#ifndef __SCRIPT_ALGO_DBOG_STATE_STAGE_H__
#define __SCRIPT_ALGO_DBOG_STATE_STAGE_H__

#include "ScriptAlgoState.h"


class CWpsAlgoStage : public CScriptAlgoStage
{

public:
	CWpsAlgoStage(BYTE byStageNumber, CWpsAlgoObject* pOwner);
	virtual	~CWpsAlgoStage();


public:

	virtual void		OnEnter();

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

	void				AddCCBDPattern(BYTE byIndex, CControlState* pPattern);
	CControlState*		GetCCBDPattern(BYTE byIndex);
	void				RemCCBDPattern(BYTE byIndex);

	void				InitMobList();
	void				SetMobList(CControlScriptNode* pNode, int nIndex);

	CControlScriptNode*	GetMobList(int nIndex) { return m_vecMobList[nIndex]; }

private:

	std::map<BYTE, CControlState*>		m_ccbdPattern;

	std::vector<CControlScriptNode*>	m_vecMobList;

};

#endif