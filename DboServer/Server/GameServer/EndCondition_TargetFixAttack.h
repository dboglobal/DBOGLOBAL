#ifndef __AI_DBOG_BOTENDCONDITION_TARGETFIXATTACK_H__
#define __AI_DBOG_BOTENDCONDITION_TARGETFIXATTACK_H__

#include "EndCondition.h"
#include "SPSNodeAction_TargetFixAttack.h"

class CEndCondition_TargetFixAttack : public CEndCondition
{

public:
	CEndCondition_TargetFixAttack(CNpc* pBot, HOBJECT hTarget, eOBJTYPE eObjType, CSPSNodeAction_TargetFixAttack::eENDTYPE eEndType, float fEndValue_Percent_Target_LP);
	virtual	~CEndCondition_TargetFixAttack();

public:

	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual void	OnExit();

private:

	HOBJECT		m_hTarget;
	DWORD		m_dwTime;
	eOBJTYPE	m_eObjType;
	CSPSNodeAction_TargetFixAttack::eENDTYPE m_eEndType;
	float		m_fEndValue_Percent_Target_LP;

};

#endif