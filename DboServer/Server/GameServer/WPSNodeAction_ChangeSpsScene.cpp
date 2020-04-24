#include "stdafx.h"
#include "WPSNodeAction_ChangeSpsScene.h"
#include "ControlScriptNodeParam_Number.h"



CWPSNodeAction_ChangeSpsScene::CWPSNodeAction_ChangeSpsScene(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_CHANGE_SPS_SCENE, lpszName)
{
}

CWPSNodeAction_ChangeSpsScene::~CWPSNodeAction_ChangeSpsScene()
{

}

CWPSNodeAction_ChangeSpsScene::sPARAMETER_INFO* CWPSNodeAction_ChangeSpsScene::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_ChangeSpsScene::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_npcTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "play script") == 0)
	{
		m_dwPlayScript = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "play scene") == 0)
	{
		m_dwPlayScene = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_ChangeSpsScene: [%s] not exist \n", name);
	return false;
}
