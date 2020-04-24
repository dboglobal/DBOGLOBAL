#include "stdafx.h"
#include "WPSNodeAction_PlayBgm.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_PlayBgm::CWPSNodeAction_PlayBgm(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_PLAY_BGM, lpszName)
{
	m_bPlay = false;
	m_bLoop = false;
	m_bPrevBgmFadeOut = true;
	m_dwDelay = 0;
}

CWPSNodeAction_PlayBgm::~CWPSNodeAction_PlayBgm()
{

}

CWPSNodeAction_PlayBgm::sPARAMETER_INFO* CWPSNodeAction_PlayBgm::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_PlayBgm::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();


	printf("CWPSNodeAction_PlayBgm: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_PlayBgm::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "play") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
		{
			m_bPlay = true;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("CWPSNodeAction_PlayBgm value '%s' dont exist \n", pNode->GetValue());
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
		printf("CWPSNodeAction_PlayBgm value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_PlayBgm: [%s] not exist \n", name);
	return false;
}


bool CWPSNodeAction_PlayBgm::CheckParameter()
{
	return true; //TODO
}