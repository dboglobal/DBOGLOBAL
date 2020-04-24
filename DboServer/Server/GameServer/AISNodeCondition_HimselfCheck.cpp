#include "stdafx.h"
#include "AISNodeCondition_HimselfCheck.h"



CAISNodeCondition_HimselfCheck::CAISNodeCondition_HimselfCheck(const char* lpszName)
: CControlScriptNodeCondition(AIS_NODE_CONDITION_HIMSELF_CHECK, lpszName)
{
}

CAISNodeCondition_HimselfCheck::~CAISNodeCondition_HimselfCheck()
{

}

CControlScriptNode::sPARAMETER_INFO* CAISNodeCondition_HimselfCheck::GetParameterMap()
{
	return NULL;
}

bool CAISNodeCondition_HimselfCheck::AddParam(CControlScriptNodeParam_Number* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}

bool CAISNodeCondition_HimselfCheck::AddParam(CControlScriptNodeParam_String* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}