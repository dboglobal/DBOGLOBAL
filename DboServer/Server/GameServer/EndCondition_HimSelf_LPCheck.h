#ifndef __AI_DBOG_BOTENDCONDITION_HIMSELFLPCHECK_H__
#define __AI_DBOG_BOTENDCONDITION_HIMSELFLPCHECK_H__

#include "EndCondition.h"
#include "AISNodeCondition_HimselfCheck.h"

class CEndCondition_HimSelf_LPCheck : public CEndCondition
{

public:
	CEndCondition_HimSelf_LPCheck(CNpc* pBot, CAISNodeCondition_HimselfCheck::eAIS_HIMSELF_VALUETYPE eValueType, float fVal);
	virtual	~CEndCondition_HimSelf_LPCheck();

public:

	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual void	OnExit();

private:

	DWORD		m_dwTime;
	CAISNodeCondition_HimselfCheck::eAIS_HIMSELF_VALUETYPE m_eValueType;
	float		m_fVal;

};

#endif