#ifndef __SERVER_WPSNODE_ACTION_REMOVENPC__
#define __SERVER_WPSNODE_ACTION_REMOVENPC__


#include "ControlScriptNodeAction.h"

enum eSPAWN_REMOVE_TYPE;

class CWPSNodeAction_RemoveNpc : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_RemoveNpc(const char* lpszName);
	virtual ~CWPSNodeAction_RemoveNpc();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	TBLIDX						m_npcTblidx;

	eSPAWN_REMOVE_TYPE			m_eSpawnRemoveType;

};


#endif
