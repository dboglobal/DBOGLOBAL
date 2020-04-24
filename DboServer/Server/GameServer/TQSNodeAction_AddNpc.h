#ifndef __SERVER_TQSONODE_ACTION_ADDNPC__
#define __SERVER_TQSONODE_ACTION_ADDNPC__


#include "ControlScriptNodeAction.h"
#include "NtlVector.h"

class CTQSNodeAction_AddNpc : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_AddNpc(const char* lpszName);
	virtual ~CTQSNodeAction_AddNpc();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

private:

	void						Init();


public:

	TBLIDX						m_npcTblidx;

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

	BYTE						m_bySpawnFuncFlag;

	DWORD						m_dwPlayScriptNumber;

	DWORD						m_dwPlayScriptScene;

	QWORD						m_qwCharConditionFlag;

};


#endif
