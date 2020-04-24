#ifndef __SERVER_WPSNODE_ACTION_SENDSPSEVENT__
#define __SERVER_WPSNODE_ACTION_SENDSPSEVENT__


#include "ControlScriptNodeAction.h"

enum eOBJTYPE;

class CWPSNodeAction_SendSPSEvent : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_SendSPSEvent(const char* lpszName);
	virtual ~CWPSNodeAction_SendSPSEvent();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


private:

	void						Init() {}

public:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

};


#endif
