#ifndef __SERVER_WPSNODE_ACTION_ADDNPC__
#define __SERVER_WPSNODE_ACTION_ADDNPC__


#include "ControlScriptNodeAction.h"

class CNtlVector;

class CWPSNodeAction_AddNpc : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_AddNpc(const char* lpszName);
	virtual ~CWPSNodeAction_AddNpc();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();

private:

	void						Init();

public:

	TBLIDX						m_npcTblidx;

	bool						m_bUseLocVar;

	std::string					m_strLocVar;

	std::string					m_strDirVar;

	CNtlVector					m_vLoc;

	CNtlVector					m_vDir;

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
