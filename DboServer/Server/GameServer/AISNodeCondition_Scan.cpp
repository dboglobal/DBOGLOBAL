#include "stdafx.h"
#include "AISNodeCondition_Scan.h"



CAISNodeCondition_Scan::CAISNodeCondition_Scan(const char* lpszName)
: CControlScriptNodeCondition(AIS_NODE_CONDITION_SCAN, lpszName)
{
}

CAISNodeCondition_Scan::~CAISNodeCondition_Scan()
{

}

CControlScriptNode::sPARAMETER_INFO* CAISNodeCondition_Scan::GetParameterMap()
{
	return NULL;
}

bool CAISNodeCondition_Scan::AddParam(CControlScriptNodeParam_Number* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}

bool CAISNodeCondition_Scan::AddParam(CControlScriptNodeParam_String* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}