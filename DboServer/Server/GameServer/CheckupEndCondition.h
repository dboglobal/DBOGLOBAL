#ifndef __AI_DBOG_CHECKUPENDCONDITION_H__
#define __AI_DBOG_CHECKUPENDCONDITION_H__

#include "ControlStateEx.h"

class CEndCondition;
class CNpc;

class CCheckupEndCondition
{
	typedef std::map<DWORD, CEndCondition*> ENDCONDITION_MAP;
	ENDCONDITION_MAP::value_type			ENDCONDITION_VALUE;

public:
	CCheckupEndCondition();
	virtual	~CCheckupEndCondition();

	typedef CControlState::eSTATUS eSTATUS;

public:

	bool				Create(CNpc* pBot);
	void				Destroy();

	void				OnUpdate(DWORD dwTickDiff, float fMultiple);

	bool				AddEndCondition(DWORD aisnodeID, CEndCondition* pEndCondition);
	bool				UpdateEndCondition(DWORD aisnodeID, CEndCondition* pEndCondition);

	CEndCondition*		GetEndCondition(DWORD aisnodeID);

	bool				DeleteEndCondition(DWORD aisnodeID);
	void				AllDeleteEndCondition();

	ENDCONDITION_MAP	m_mapEndCondition;

private:

	CNpc*			m_pBot;

};

#endif