#ifndef __SERVER_WPSNODE_ACTION_CHANGEOBJECTSTATE__
#define __SERVER_WPSNODE_ACTION_CHANGEOBJECTSTATE__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_ChangeObjectState : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_ChangeObjectState(const char* lpszName);
	virtual ~CWPSNodeAction_ChangeObjectState();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

private:

	void						Init();

public:

	TBLIDX						m_objectTblidx;

	BYTE						m_mainState;

	BYTE						m_subStateSet;

	BYTE						m_subStateUnset;

};


#endif
