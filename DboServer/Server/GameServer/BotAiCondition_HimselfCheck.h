#ifndef __AI_DBOG_BOTCONDITION_HIMSELFCHECK_H__
#define __AI_DBOG_BOTCONDITION_HIMSELFCHECK_H__

#include "BotAiCondition.h"
#include "AISNodeCondition_HimselfCheck.h"

class CBotAiCondition_HimselfCheck : public CBotAiCondition
{

public:
	CBotAiCondition_HimselfCheck(CNpc* pBot);
	virtual	~CBotAiCondition_HimselfCheck();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	void OnHalt();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	int Handle_LP();
	int Handle_SkillHit();

private:

	CAISNodeCondition_HimselfCheck::eAIS_HIMSELF_CHECKTYPE m_eCheckType;
	CAISNodeCondition_HimselfCheck::eAIS_HIMSELF_VALUETYPE m_eValueType;
	float m_fVal;
	DWORD m_dwTime;
	DWORD m_scriptnodeID;
	TBLIDX m_tblidxSkill[10];
	BYTE m_byTblidxSkillCount;

};

#endif