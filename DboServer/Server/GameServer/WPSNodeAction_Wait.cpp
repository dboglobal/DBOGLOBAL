#include "stdafx.h"
#include "WPSNodeAction_Wait.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_Wait::CWPSNodeAction_Wait(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_WAIT, lpszName)
{
	m_bOperationAND = true;
}

CWPSNodeAction_Wait::~CWPSNodeAction_Wait()
{

}

CWPSNodeAction_Wait::sPARAMETER_INFO* CWPSNodeAction_Wait::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_Wait::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "operation") == 0)
	{
		if (_stricmp(pNode->GetValue(), "and") == 0)
			m_bOperationAND = true;
		else if (_stricmp(pNode->GetValue(), "or") == 0)
			m_bOperationAND = false;

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_Wait: [%s] not exist \n", name);

	return false;
}