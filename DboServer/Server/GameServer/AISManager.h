#ifndef __SERVER_AIS_MANAGER__
#define __SERVER_AIS_MANAGER__


#include "NtlSingleton.h"
#include "ControlScriptManager.h"
#include "AISDefine.h"


class CAISManager : public CControlScriptManager
{

public:
	CAISManager();
	virtual ~CAISManager();


public:
	virtual bool Create(const char* lpszScriptPath, CControlScriptManager::eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt);

	virtual CControlScript*		CreateControlScript(const char* lpszScriptFileName);

	virtual bool				AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwScriptNumber, DWORD dwSceneNumber, eAIS_NODE eAttachAisNode);

};


#endif
