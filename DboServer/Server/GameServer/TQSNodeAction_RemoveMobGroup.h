#ifndef __SERVER_TQSONODE_ACTION_REMOVEMOBGROUP__
#define __SERVER_TQSONODE_ACTION_REMOVEMOBGROUP__


#include "ControlScriptNodeAction.h"
enum eSPAWN_REMOVE_TYPE;

class CTQSNodeAction_RemoveMobGroup : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_RemoveMobGroup(const char* lpszName);
	virtual ~CTQSNodeAction_RemoveMobGroup();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	SPAWNGROUPID				m_spawnGroupId;

	eSPAWN_REMOVE_TYPE			m_eSpawnRemoveType;

};


#endif
