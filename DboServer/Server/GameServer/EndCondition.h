#ifndef __AI_DBOG_BOTENDCONDITION_H__
#define __AI_DBOG_BOTENDCONDITION_H__

class CNpc;

class CEndCondition
{

public:
	CEndCondition(CNpc* pBot, const char* lpszControlName);
	virtual	~CEndCondition();

public:

	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple) = 0;
	virtual void	OnExit() = 0;

	CNpc*		GetBot() { return m_pBot; }
	const char*		GetEndConditionName() { return m_lpszEndConditionName; }

private:

	CNpc* m_pBot;
	const char* m_lpszEndConditionName;

};

#endif