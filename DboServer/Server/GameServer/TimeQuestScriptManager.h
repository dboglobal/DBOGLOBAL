#ifndef __SERVER_TIMEQUESTSCRIPT_MANAGER__
#define __SERVER_TIMEQUESTSCRIPT_MANAGER__


#include "NtlSingleton.h"
#include "TQSManager.h"
#include "TQSNodeFactory.h"


class CTimeQuestScriptManager : public CNtlSingleton<CTimeQuestScriptManager>, public CTQSManager
{

public:
	CTimeQuestScriptManager();
	virtual ~CTimeQuestScriptManager();

public:

	virtual CControlScriptNodeFactory*	GetControlScriptNodeFactory() { return &m_nodeFactory; }


private:

	CTQSNodeFactory m_nodeFactory;

};

#define GetCTimeQuestScriptManager()		CTimeQuestScriptManager::GetInstance()
#define g_pTimeQuestScriptManager			GetCTimeQuestScriptManager()

#endif
