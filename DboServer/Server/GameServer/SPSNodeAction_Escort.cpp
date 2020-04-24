#include "stdafx.h"
#include "SPSNodeAction_Escort.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeAction_Escort::CSPSNodeAction_Escort(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_ESCORT, lpszName)
{
	m_eEscortType = INVALID_ESCORT_TYPE;
	m_vDestLoc.Reset();
	m_fRadius = 0.0f;
	m_bRunMode = true;
	m_eventID = INVALID_DWORD;
}

CSPSNodeAction_Escort::~CSPSNodeAction_Escort()
{
}

bool CSPSNodeAction_Escort::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "dest x") == 0)
	{
		m_vDestLoc.x = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "dest y") == 0)
	{
		m_vDestLoc.y = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "dest z") == 0)
	{
		m_vDestLoc.z = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "radius") == 0)
	{
		m_fRadius = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "run mode") == 0)
	{
		m_bRunMode = pNode->GetValue() != 0.0f;
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "event id") == 0)
	{
		m_eventID = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_Escort: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeAction_Escort::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "type") == 0)
	{
		const char* sub = pNode->GetValue();
		
		if (_stricmp(sub, "under escort") == 0)
		{
			m_eEscortType = ESCORT_TYPE_UNDER_ESCORT;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "target follow") == 0)
		{
			m_eEscortType = ESCORT_TYPE_TARGET_FOLLOW;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_Calculate: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeAction_Calculate: [%s] not exist \n", name);
	return false;
}