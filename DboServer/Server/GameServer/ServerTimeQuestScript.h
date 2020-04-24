#ifndef __SERVER_TQS_SCRIPT__
#define __SERVER_TQS_SCRIPT__

#include "ControlScript.h"
#include "TQSNodeGameStage.h"


class CServerTimeQuestScript : public CControlScript
{

public:
	CServerTimeQuestScript(CControlScriptManager* pManager, char const* lpszFileName);
	virtual ~CServerTimeQuestScript();


public:

	virtual bool			Load(char * achBuffer) { return true; }
	virtual bool			Load(const char* lpszScriptFile);
	virtual bool			CheckIntegrity();

public:

	void						Destroy() {}

	CControlScriptNodeState*	Find(DWORD dwStageNumber);

	bool						AddStage(DWORD dwStageNumber, CControlScriptNodeState * pGameStage);

	bool						AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwStageNumber);

	static bool					LuaBind();

private:

	void						Init();


private:

	std::map<DWORD, CControlScriptNodeState*> m_gameStageList;

};


#endif