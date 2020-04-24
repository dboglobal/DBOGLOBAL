#ifndef __SERVER_PLAYSCRIPT_MANAGER__
#define __SERVER_PLAYSCRIPT_MANAGER__


#include "NtlSingleton.h"
#include "SPSManager.h"
#include "SPSNodeFactory.h"


class CPlayScriptManager : public CNtlSingleton<CPlayScriptManager>, public CSPSManager
{

public:
	CPlayScriptManager();
	virtual ~CPlayScriptManager();

public:

	virtual CControlScriptNodeFactory*	GetControlScriptNodeFactory() { return &m_nodeFactory; }

	virtual CTableContainer*			GetTableContainer();


private:

	CSPSNodeFactory						m_nodeFactory;

};

#define GetCPlayScriptManager()			CPlayScriptManager::GetInstance()
#define g_pPlayScriptManager			GetCPlayScriptManager()

#endif
