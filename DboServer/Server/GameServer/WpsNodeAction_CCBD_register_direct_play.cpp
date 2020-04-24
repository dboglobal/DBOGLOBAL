#include "stdafx.h"
#include "WpsNodeAction_CCBD_register_direct_play.h"
#include "ControlScriptNodeParam_Number.h"



CWpsNodeAction_CCBD_register_direct_play::CWpsNodeAction_CCBD_register_direct_play(const char* lpszName)
	:CControlScriptNodeAction(WPS_NODE_ACTION_CCBD_REGISTER_DIRECT_PLAY, lpszName)
{
	m_directIndex = INVALID_TBLIDX;
}

CWpsNodeAction_CCBD_register_direct_play::~CWpsNodeAction_CCBD_register_direct_play()
{
}


bool CWpsNodeAction_CCBD_register_direct_play::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "change next stage") == 0)
	{
		m_directIndex = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_CCBD_register_direct_play: [%s] not exist \n", name);
	return false;
}