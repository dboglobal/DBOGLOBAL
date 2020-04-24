#include "stdafx.h"
#include "AISNodeAction_SendEventToTQS.h"



CAISNodeAction_SendEventToTQS::CAISNodeAction_SendEventToTQS(const char* lpszName)
:CControlScriptNodeAction(AIS_NODE_ACTION_SENDEVENTTOTQS, lpszName)
{
}

CAISNodeAction_SendEventToTQS::~CAISNodeAction_SendEventToTQS()
{

}

CControlScriptNode::sPARAMETER_INFO* CAISNodeAction_SendEventToTQS::GetParameterMap()
{
	return NULL;
}

bool CAISNodeAction_SendEventToTQS::AddParam(CControlScriptNodeParam_Number* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}

