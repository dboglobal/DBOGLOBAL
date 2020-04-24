#ifndef __AI_DBOG_BOTACTION_PATROL_H__
#define __AI_DBOG_BOTACTION_PATROL_H__

#include "BotAiAction.h"
#include "WayPointList.h"


class CBotAiAction_Patrol : public CBotAiAction
{

public:

	CBotAiAction_Patrol(CNpc* pBot, TBLIDX pathTblidx, bool bRunMode, float fPointRadius);
	CBotAiAction_Patrol(CNpc* pBot);
	virtual	~CBotAiAction_Patrol();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX			m_pathTblidx;
	bool			m_bRunMode;
	CWayPointList	m_wayPointList;
	float			m_fPointRadius;
	bool			m_bIsScript;

};

#endif