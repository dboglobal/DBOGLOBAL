#ifndef __SERVER_TQSONODE_ACTION_ADDMOB__
#define __SERVER_TQSONODE_ACTION_ADDMOB__


#include "ControlScriptNodeAction.h"
#include "NtlVector.h"

class CTQSNodeAction_AddMob : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_AddMob(const char* lpszName);
	virtual ~CTQSNodeAction_AddMob();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

private:

	void						Init();


public:

	TBLIDX						m_mobTblidx;

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

	bool						m_bRespawn;

	DWORD						m_dwPlayScriptNumber;

	DWORD						m_dwPlayScriptScene;

	PARTYID						m_partyId;

	bool						m_bPartyLeader;

	BYTE						m_bySpawnFuncFlag;

};


#endif
