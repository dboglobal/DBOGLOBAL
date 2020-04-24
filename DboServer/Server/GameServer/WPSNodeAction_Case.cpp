#include "stdafx.h"
#include "WPSNodeAction_Case.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_Case::CWPSNodeAction_Case(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_CASE, lpszName)
{
}

CWPSNodeAction_Case::~CWPSNodeAction_Case()
{

}

CWPSNodeAction_Case::sPARAMETER_INFO* CWPSNodeAction_Case::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_Case::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "value") == 0)
	{
		m_nCompareValue = (int)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_Case: [%s] not exist \n", name);
	return false;
}
