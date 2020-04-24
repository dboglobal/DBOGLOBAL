#ifndef __SCRIPT_ALGO_DBOG_OBJECT_MANAGER_H__
#define __SCRIPT_ALGO_DBOG_OBJECT_MANAGER_H__

#include "NtlSingleton.h"
#include "ScriptAlgoObject.h"
#include "WorldPlayTable.h"

class CWpsAlgoObject;
class CTqsAlgoObject;
struct sTIMEQUEST_TBLDAT;

class CScriptAlgoObjectManager : public CNtlSingleton<CScriptAlgoObjectManager>
{

public:

	CScriptAlgoObjectManager();
	virtual	~CScriptAlgoObjectManager();

public:

	void								TickProcess(DWORD dwTickDiff);

	DWORD								GenerateHandle();

	CWpsAlgoObject*						CreateWpsObject(TBLIDX scriptTblidx, bool & rbStart, CPlayer* pPlayer = NULL);

	CTqsAlgoObject*						CreateTqsObject(TBLIDX scriptTblidx, sTIMEQUEST_TBLDAT* pTimeQuestTbldat);

	void								DestroyScript(CScriptAlgoObject* pScript);

	CWpsAlgoObject*						GetWpsObject(TBLIDX scriptTblidx);

	CScriptAlgoObject*					Find(DWORD dwHandle);

	void								LogAllActiveScripts();


private:

	DWORD								m_dwHandle;

	std::vector<DWORD>					m_vecHandle;

	std::map<DWORD, CScriptAlgoObject*>	m_setScriptObjects;

	std::vector<CScriptAlgoObject*>		m_setRemoveScriptObjects;

};

#define GetScriptAlgoManager()			CScriptAlgoObjectManager::GetInstance()
#define g_pScriptAlgoManager			GetScriptAlgoManager()

#endif