#ifndef __SERVER_WORLDPLAYSCRIPT_MANAGER__
#define __SERVER_WORLDPLAYSCRIPT_MANAGER__


#include "NtlSingleton.h"
#include "WPSManager.h"
#include "WPSNodeFactory.h"


class CWorldPlayScriptManager : public CNtlSingleton<CWorldPlayScriptManager>, public CWPSManager
{

public:
	CWorldPlayScriptManager();
	virtual ~CWorldPlayScriptManager();

public:

	virtual CControlScriptNodeFactory*	GetControlScriptNodeFactory() { return &m_nodeFactory; }



private:

	CWPSNodeFactory m_nodeFactory;

};

#define GetCWorldPlayScriptManager()		CWorldPlayScriptManager::GetInstance()
#define g_pWorldPlayScriptManager			GetCWorldPlayScriptManager()


#endif
