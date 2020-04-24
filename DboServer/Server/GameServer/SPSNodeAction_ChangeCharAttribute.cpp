#include "stdafx.h"
#include "SPSNodeAction_ChangeCharAttribute.h"



CSPSNodeAction_ChangeCharAttribute::CSPSNodeAction_ChangeCharAttribute(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_CHANGE_CHAR_ATTRIBUTE, lpszName)
{
}

CSPSNodeAction_ChangeCharAttribute::~CSPSNodeAction_ChangeCharAttribute()
{

}

CControlScriptNode::sPARAMETER_INFO* CSPSNodeAction_ChangeCharAttribute::GetParameterMap()
{
	return NULL;
}

bool CSPSNodeAction_ChangeCharAttribute::AddParam(CControlScriptNodeParam_Number* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}

bool CSPSNodeAction_ChangeCharAttribute::AddParam(CControlScriptNodeParam_String* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}