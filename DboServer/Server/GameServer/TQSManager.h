#ifndef __SERVER_TQS_MANAGER__
#define __SERVER_TQS_MANAGER__


#include "NtlSingleton.h"
#include "ControlScriptManager.h"


class CControlScriptNode;


class CTQSManager : public CControlScriptManager
{

public:
	CTQSManager();
	virtual ~CTQSManager();


public:
	virtual bool				Create(const char* lpszScriptPath, CControlScriptManager::eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt);

	virtual CControlScript*		CreateControlScript(const char* lpszScriptFileName);


public:

	bool						AttachScript(CControlState* pControlState, CControlStateFactory * pFactory, DWORD dwScriptNumber, DWORD dwStageNumber);

};


#endif
