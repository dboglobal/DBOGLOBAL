#ifndef __SERVER_SPS_MANAGER__
#define __SERVER_SPS_MANAGER__


#include "NtlSingleton.h"
#include "ControlScriptManager.h"

class CTableContainer;
class CSPSNodeScene;
class CControlScriptNode;

class CSPSNodeCondition_RegisterBattleScene;


class CSPSManager : public CControlScriptManager
{

public:
	CSPSManager();
	virtual ~CSPSManager();


public: 
	virtual bool								Create(const char* lpszScriptPath, CControlScriptManager::eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt);

	virtual CControlScript*						CreateControlScript(const char* lpszScriptFileName);

	virtual CTableContainer*					GetTableContainer() { return NULL; }


public:

	bool										AttachScript(CControlState* pControlState, CControlStateFactory * pFactory, DWORD dwScriptNumber, DWORD dwSceneNumber);

	CSPSNodeCondition_RegisterBattleScene*		GetFirstRegBattleScene(unsigned int scriptNumber, DWORD dwSceneNumber);
	CSPSNodeCondition_RegisterBattleScene*		GetNextRegBattleScene(CSPSNodeCondition_RegisterBattleScene * pBattleScene);


	CSPSNodeScene*								GetPlayScriptScene(DWORD scriptNumber, DWORD dwSceneNumber);
	CControlScriptNode*							GetPlayScriptNode(DWORD scriptNumber, DWORD dwNodeIndex);

};


#endif
