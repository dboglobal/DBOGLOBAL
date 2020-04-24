#include "stdafx.h"
#include "AISNodeAction_SendEventToWPS.h"



CAISNodeAction_SendEventToWPS::CAISNodeAction_SendEventToWPS(const char* lpszName)
:CControlScriptNodeAction(AIS_NODE_ACTION_SENDEVENTTOWPS, lpszName)
{
}

CAISNodeAction_SendEventToWPS::~CAISNodeAction_SendEventToWPS()
{

}

CControlScriptNode::sPARAMETER_INFO* CAISNodeAction_SendEventToWPS::GetParameterMap()
{
	return NULL;
}

bool CAISNodeAction_SendEventToWPS::AddParam(CControlScriptNodeParam_Number* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}