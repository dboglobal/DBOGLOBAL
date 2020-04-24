#ifndef __SCRIPT_ALGO_DBOG_ACTION_ADDNPC_H__
#define __SCRIPT_ALGO_DBOG_ACTION_ADDNPC_H__

#include "ScriptAlgoAction.h"

class CWpsAlgoAction_AddNpc : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_AddNpc(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_AddNpc();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_npcTblidx;

	CNtlVector					m_vLoc;

	CNtlVector					m_vDir;

	bool						m_bUseLocVar;

	std::string					m_strLocVar;

	std::string					m_strDirVar;

	BYTE						m_bySpawnFuncFlag; // eSPAWN_FUNC_FLAG

	DWORD						m_dwPlayScriptNumber;

	DWORD						m_dwPlayScriptScene;

	DWORD						m_dwAISNumber;

	DWORD						m_dwAISScene;

	QWORD						m_qwCharConditionFlag;

	bool						m_bStandAlone;

	PARTYID						m_partyId;

	bool						m_bPartyLeader;

	BYTE						m_byImmortalMode;

};

#endif