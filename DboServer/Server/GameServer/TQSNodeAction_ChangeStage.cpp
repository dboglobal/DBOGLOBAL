#include "stdafx.h"
#include "TQSNodeAction_ChangeStage.h"
#include "ControlScriptNodeParam_Number.h"


CTQSNodeAction_ChangeStage::CTQSNodeAction_ChangeStage(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_CHANGE_STAGE, lpszName)
{
	m_byNextStage = INVALID_BYTE;
}

CTQSNodeAction_ChangeStage::~CTQSNodeAction_ChangeStage()
{

}

CTQSNodeAction_ChangeStage::sPARAMETER_INFO* CTQSNodeAction_ChangeStage::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_ChangeStage::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "stage") == 0)
	{
		m_byNextStage = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_ChangeStage: [%s] not exist \n", name);
	return false;
}
