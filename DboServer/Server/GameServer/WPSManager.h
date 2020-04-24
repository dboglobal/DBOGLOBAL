#ifndef __SERVER_WPS_MANAGER__
#define __SERVER_WPS_MANAGER__


#include "NtlSingleton.h"
#include "ControlScriptManager.h"


class CControlScriptNode;
class CWorldPlayControlFactory;

class CWPSManager : public CControlScriptManager
{

public:
	CWPSManager();
	virtual ~CWPSManager();


public:
	virtual bool				Create(const char* lpszScriptPath, CControlScriptManager::eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt);

	virtual CControlScript*		CreateControlScript(const char* lpszScriptFileName);


public:

	bool						AttachScript(CControlState* pControlState, CControlStateFactory * pFactory, DWORD dwScriptNumber, DWORD dwStageNumber);

	CControlScriptNode*			GetWorldPlayScriptNode(DWORD scriptNumber, DWORD dwNodeIndex);

};


#endif
