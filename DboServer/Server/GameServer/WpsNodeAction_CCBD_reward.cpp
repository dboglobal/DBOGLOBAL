#include "stdafx.h"
#include "WpsNodeAction_CCBD_reward.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"



CWpsNodeAction_CCBD_reward::CWpsNodeAction_CCBD_reward(const char* lpszName)
	:CControlScriptNodeAction(WPS_NODE_ACTION_CCBD_REWARD, lpszName)
{
	m_itemTblidx = INVALID_TBLIDX;
	m_bLastStage = false;
}

CWpsNodeAction_CCBD_reward::~CWpsNodeAction_CCBD_reward()
{

}



bool CWpsNodeAction_CCBD_reward::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "item tblidx") == 0)
	{
		m_itemTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_CCBD_reward: [%s] not exist \n", name);
	return false;
}

bool CWpsNodeAction_CCBD_reward::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();


	if (_stricmp(name, "last stage") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bLastStage = true;
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_CCBD_reward: [%s] not exist \n", name);
	return false;
}