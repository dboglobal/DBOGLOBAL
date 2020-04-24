#include "stdafx.h"
#include "TQSNodeAction_DirectPlay.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeAction_DirectPlay::CTQSNodeAction_DirectPlay(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_ADD_DIRECT_PLAY, lpszName)
{
	m_directTblidx = INVALID_TBLIDX;
	m_bSynchronize = false;
	m_bCanSkip = false;
	m_dwWaitTime = DEFAULT_DIRECTION_EXCEPTION_TIME;
	m_bEnding = false;
}

CTQSNodeAction_DirectPlay::~CTQSNodeAction_DirectPlay()
{
}

CTQSNodeAction_DirectPlay::sPARAMETER_INFO* CTQSNodeAction_DirectPlay::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_DirectPlay::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "id") == 0)
	{
		m_directTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "time") == 0)
	{
		m_dwWaitTime = (DWORD)pNode->GetValue() * 1000;
		return CControlScriptNode::AddParam(pNode);
	}



	printf("CTQSNodeAction_DirectPlay: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeAction_DirectPlay::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();

	if (_stricmp(name, "type") == 0)
	{
		if (_stricmp(value, "flash") == 0)
			m_eDirectionType = DIRECTION_FLASH;
		else if (_stricmp(value, "cinematic") == 0)
			m_eDirectionType = DIRECTION_CINEMATIC;
		else if (_stricmp(value, "play") == 0)
			m_eDirectionType = DIRECTION_PLAY;

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "ending") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bEnding = true;

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "sync") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bSynchronize = true;

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "skip") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bCanSkip = true;

		return CControlScriptNode::AddParam(pNode);
	}


	printf("CTQSNodeAction_DirectPlay: [%s] not exist \n", name);
	return false;
}