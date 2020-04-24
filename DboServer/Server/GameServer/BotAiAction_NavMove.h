#ifndef __AI_DBOG_BOTACTION_NAVMOVE_H__
#define __AI_DBOG_BOTACTION_NAVMOVE_H__

#include "BotAiAction.h"

class CBotAiAction_NavMove : public CBotAiAction
{

public:

	CBotAiAction_NavMove(CNpc* pBot, CNtlVector& rDestLoc, bool bRunMode, bool bHaveSecondDestLoc, CNtlVector& rSecondDestLoc, CNtlVector& rDestDir, float fMoveSpeed);
	CBotAiAction_NavMove(CNpc* pBot);
	virtual	~CBotAiAction_NavMove();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnPause();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

protected:

	void		DoNavMove();

private:

	CNtlVector	m_vDestLoc;
	CNtlVector	m_vSecondDestLoc;
	CNtlVector	m_vDestDir;
	bool		m_bRunMode;
	bool		m_bHaveSecondDestLoc;
	bool		m_bRes;
	float		m_fMoveSpeed;

};

#endif