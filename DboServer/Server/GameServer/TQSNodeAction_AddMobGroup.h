#ifndef __SERVER_TQSONODE_ACTION_ADDMOBGROUP__
#define __SERVER_TQSONODE_ACTION_ADDMOBGROUP__


#include "ControlScriptNodeAction.h"
#include "NtlVector.h"

class CTQSNodeAction_AddMobGroup : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_AddMobGroup(const char* lpszName);
	virtual ~CTQSNodeAction_AddMobGroup();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	void						Init();


public:

	SPAWNGROUPID				m_spawnGroupId;

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

	bool						m_bRespawn;

	DWORD						m_dwPlayScriptNumber;

	DWORD						m_dwPlayScriptScene;

	BYTE						m_bySpawnFuncFlag;

	BYTE						m_byImmortalMode; // by daneos

};


#endif
