#ifndef __SCRIPT_ALGO_DBOG_ACTION_CCBD_STAGE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CCBD_STAGE_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_CCBD_stage : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_CCBD_stage(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_CCBD_stage();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void				OnEnter();

	virtual void				OnExit();

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	void						SpawnNextStage();
	void						TeleportToBoss();
	void						TeleportOut();

	bool						CheckPlayersState();
	bool						IsEveryoneReady();

public:

	BYTE						m_byStage;
	bool						m_bDirectPlay; //if false then it is boss stage !
	DWORD						m_dwFailTimer;

	bool						m_bIsEveryoneReady;
};

#endif