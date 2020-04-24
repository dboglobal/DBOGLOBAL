#include "stdafx.h"
#include "AISNodeCondition_RecvEventFromTQS.h"



CAISNodeCondition_RecvEventFromTQS::CAISNodeCondition_RecvEventFromTQS(const char* lpszName)
: CControlScriptNodeCondition(AIS_NODE_CONDITION_RECV_EVENT_FROM_TQS, lpszName)
{
}

CAISNodeCondition_RecvEventFromTQS::~CAISNodeCondition_RecvEventFromTQS()
{

}

CControlScriptNode::sPARAMETER_INFO* CAISNodeCondition_RecvEventFromTQS::GetParameterMap()
{
	return NULL;
}

bool CAISNodeCondition_RecvEventFromTQS::AddParam(CControlScriptNodeParam_Number* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}