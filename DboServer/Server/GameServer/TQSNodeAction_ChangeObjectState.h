#ifndef __SERVER_TQSONODE_ACTION_CHANGEOBJECTSTATE__
#define __SERVER_TQSONODE_ACTION_CHANGEOBJECTSTATE__


#include "ControlScriptNodeAction.h"


class CTQSNodeAction_ChangeObjectState : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_ChangeObjectState(const char* lpszName);
	virtual ~CTQSNodeAction_ChangeObjectState();


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
