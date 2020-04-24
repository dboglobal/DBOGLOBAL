#include "stdafx.h"
#include "AISNodeCondition_RecvEventFromWPS.h"



CAISNodeCondition_RecvEventFromWPS::CAISNodeCondition_RecvEventFromWPS(const char* lpszName)
: CControlScriptNodeCondition(AIS_NODE_CONDITION_RECV_EVENT_FROM_WPS, lpszName)
{
}

CAISNodeCondition_RecvEventFromWPS::~CAISNodeCondition_RecvEventFromWPS()
{

}

CControlScriptNode::sPARAMETER_INFO* CAISNodeCondition_RecvEventFromWPS::GetParameterMap()
{
	return NULL;
}

bool CAISNodeCondition_RecvEventFromWPS::AddParam(CControlScriptNodeParam_Number* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}