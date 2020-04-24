#include "stdafx.h"
#include "WPSNodeAction_ExecWps.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_ExecWps::CWPSNodeAction_ExecWps(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_EXEC_WPS, lpszName)
{
}

CWPSNodeAction_ExecWps::~CWPSNodeAction_ExecWps()
{

}

CWPSNodeAction_ExecWps::sPARAMETER_INFO* CWPSNodeAction_ExecWps::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_ExecWps::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "wps tblidx") == 0)
	{
		m_wpsTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_Wait: [%s] not exist \n", name);

	return false;
}
