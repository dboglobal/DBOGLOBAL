#include "stdafx.h"
#include "WPSNodeAction_ChangeStage.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_ChangeStage::CWPSNodeAction_ChangeStage(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_CHANGE_STAGE, lpszName)
{
}

CWPSNodeAction_ChangeStage::~CWPSNodeAction_ChangeStage()
{

}

CWPSNodeAction_ChangeStage::sPARAMETER_INFO* CWPSNodeAction_ChangeStage::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_ChangeStage::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "stage") == 0)
	{
		m_byNextStage = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_ChangeStage: [%s] not exist \n", name);
	return false;
}
