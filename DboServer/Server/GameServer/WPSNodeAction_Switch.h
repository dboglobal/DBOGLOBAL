#ifndef __SERVER_WPSNODE_ACTION_SWITCH__
#define __SERVER_WPSNODE_ACTION_SWITCH__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_Switch : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_Switch(const char* lpszName);
	virtual ~CWPSNodeAction_Switch();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

private:

	//virtual bool				AttachScript(CControlState* pControlState, CControlStateFactory* pControlFactory, bool bAttachCurrent);

	//void						AttachCase();

public:

	std::string					m_strCompareVariable;

};


#endif
