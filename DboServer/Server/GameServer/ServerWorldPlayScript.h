#ifndef __SERVER_WPS_SCRIPT__
#define __SERVER_WPS_SCRIPT__

#include "ControlScript.h"
#include "ControlScriptNodeState.h"


class CServerWorldPlayScript : public CControlScript
{

public:
	CServerWorldPlayScript(CControlScriptManager* pManager, char const* lpszFileName);
	virtual ~CServerWorldPlayScript();


public:

	virtual bool			Load(char * achBuffer) { return true; }
	virtual bool			Load(const char* lpszScriptFile);
	virtual bool			CheckIntegrity();

public:

	void						Destroy() {}

	CControlScriptNodeState*	Find(BYTE byStageNumber);
	bool						AddStage(BYTE byStageNumber, CControlScriptNodeState* pGameStage);

	bool						AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, BYTE byStageNumber);

	static bool					LuaBind();

private:

	void						Init();

private:

	std::map<BYTE, CControlScriptNodeState*> m_gameStageList;

};


#endif