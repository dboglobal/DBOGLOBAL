#include "stdafx.h"
#include "SPSNodeAction_ChangeNpcAttribute.h"



CSPSNodeAction_ChangeNpcAttribute::CSPSNodeAction_ChangeNpcAttribute(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_CHANGE_NPC_ATTRIBUTE, lpszName)
{
}

CSPSNodeAction_ChangeNpcAttribute::~CSPSNodeAction_ChangeNpcAttribute()
{

}

bool CSPSNodeAction_ChangeNpcAttribute::AddParam(CControlScriptNodeParam_String* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}