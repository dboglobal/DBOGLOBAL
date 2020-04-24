#include "stdafx.h"
#include "TQSNodeAction_PlayBgm.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeAction_PlayBgm::CTQSNodeAction_PlayBgm(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_PLAY_BGM, lpszName)
{
	m_bPlay = false;
	m_bLoop = false;
	m_bPrevBgmFadeOut = true;
	m_dwDelay = 0;
}

CTQSNodeAction_PlayBgm::~CTQSNodeAction_PlayBgm()
{

}

CTQSNodeAction_PlayBgm::sPARAMETER_INFO* CTQSNodeAction_PlayBgm::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_PlayBgm::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CTQSNodeAction_PlayBgm: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeAction_PlayBgm::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "play") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
		{
			m_bPlay = true;
			return CControlScriptNode::AddParam(pNode);
		}
		else if (_stricmp(pNode->GetValue(), "false") == 0)
		{
			m_bPlay = false;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("CTQSNodeAction_PlayBgm value '%s' dont exist \n", pNode->GetValue());
	}

	if (_stricmp(name, "name") == 0)
	{
		m_strName = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "loop") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
		{
			m_bLoop = true;
			return CControlScriptNode::AddParam(pNode);
		}
		else if (_stricmp(pNode->GetValue(), "false") == 0)
		{
			m_bLoop = false;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("CTQSNodeAction_PlayBgm value '%s' dont exist \n", pNode->GetValue());
	}

	if (_stricmp(name, "prev bgm fadeout") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
		{
			m_bPrevBgmFadeOut = true;
			return CControlScriptNode::AddParam(pNode);
		}
		else if (_stricmp(pNode->GetValue(), "false") == 0)
		{
			m_bPrevBgmFadeOut = false;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("CTQSNodeAction_PlayBgm value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CTQSNodeAction_PlayBgm: [%s] not exist \n", name);
	return false;
}