#ifndef __SERVER_AISCRIPT_MANAGER__
#define __SERVER_AISCRIPT_MANAGER__

#include "NtlSingleton.h"
#include "AISManager.h"
#include "AISNodeFactory.h"


class CAIScriptManager : public CNtlSingleton<CAIScriptManager>, public CAISManager
{

public:
	CAIScriptManager();
	virtual ~CAIScriptManager();

public:


	virtual CControlScriptNodeFactory*		GetControlScriptNodeFactory() { return static_cast<CControlScriptNodeFactory*>(&m_nodeFactory); }


private:

	CAISNodeFactory							m_nodeFactory;

};

#define GetCAIScriptManager()		CAIScriptManager::GetInstance()
#define g_pAiScriptManager			GetCAIScriptManager()

#endif