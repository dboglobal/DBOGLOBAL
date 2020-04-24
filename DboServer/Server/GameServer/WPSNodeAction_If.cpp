#include "stdafx.h"
#include "WPSNodeAction_If.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_If::CWPSNodeAction_If(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_IF, lpszName)
{
	m_bOperationAND = false;
	m_bReverse = true;
}

CWPSNodeAction_If::~CWPSNodeAction_If()
{

}

CWPSNodeAction_If::sPARAMETER_INFO* CWPSNodeAction_If::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_If::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "reverse") == 0)
	{
		if (_stricmp(pNode->GetValue(), "false") == 0)
		{
			m_bReverse = false;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_If: [%s] not exist \n", name);
	return false;
}