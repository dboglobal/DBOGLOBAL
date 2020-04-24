#ifndef __SERVER_TQSONODE_ACTION_REMOVENPC__
#define __SERVER_TQSONODE_ACTION_REMOVENPC__


#include "ControlScriptNodeAction.h"
enum eSPAWN_REMOVE_TYPE;

class CTQSNodeAction_RemoveNpc : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_RemoveNpc(const char* lpszName);
	virtual ~CTQSNodeAction_RemoveNpc();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	TBLIDX						m_npcTblidx;

	eSPAWN_REMOVE_TYPE			m_eSpawnRemoveType;

};


#endif
