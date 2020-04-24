#ifndef __SERVER_TQSONODE_ACTION_SENDTQSEVENT__
#define __SERVER_TQSONODE_ACTION_SENDTQSEVENT__


#include "ControlScriptNodeAction.h"

enum eOBJTYPE;

class CTQSNodeAction_SendTqsEvent : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_SendTqsEvent(const char* lpszName);
	virtual ~CTQSNodeAction_SendTqsEvent();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

};


#endif
