#include "stdafx.h"
#include "SPSNodeAction_DirectPlay.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CSPSNodeAction_DirectPlay::CSPSNodeAction_DirectPlay(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_DIRECT_PLAY, lpszName)
{
	m_directionTblidx = INVALID_TBLIDX;
	m_dwPlayTime = INVALID_DWORD;
	m_eEventType = INVALID_SPS_EVENT_TYPE;
	m_eventID = INVALID_DWORD;
}

CSPSNodeAction_DirectPlay::~CSPSNodeAction_DirectPlay()
{

}

bool CSPSNodeAction_DirectPlay::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "tblidx") == 0)
	{
		m_directionTblidx = (TBLIDX)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "play time") == 0)
	{
		m_dwPlayTime = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "event id") == 0)
	{
		m_eventID = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_DirectPlay: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeAction_DirectPlay::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event type") == 0)
	{
		const char* sub = pNode->GetValue();

		if (_stricmp(sub, "ts") == 0)
		{
			m_eEventType = SPS_EVENT_TYPE_TS;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "tqs") == 0)
		{
			m_eEventType = SPS_EVENT_TYPE_TQS;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "wps") == 0)
		{
			m_eEventType = SPS_EVENT_TYPE_WPS;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_DirectPlay: sub [%s] not exist \n", name);
		return false;
	}

	printf("CSPSNodeAction_DirectPlay: [%s] not exist \n", name);
	return false;
}