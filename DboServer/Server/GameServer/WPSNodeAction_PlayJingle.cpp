#include "stdafx.h"
#include "WPSNodeAction_PlayJingle.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_PlayJingle::CWPSNodeAction_PlayJingle(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_PLAY_JINGLE, lpszName)
{
	m_bApplyFadeInOut = true;
}

CWPSNodeAction_PlayJingle::~CWPSNodeAction_PlayJingle()
{

}

CWPSNodeAction_PlayJingle::sPARAMETER_INFO* CWPSNodeAction_PlayJingle::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_PlayJingle::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "name") == 0)
	{
		m_strName = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_PlayJingle: [%s] not exist \n", name);
	return false;
}


bool CWPSNodeAction_PlayJingle::CheckParameter()
{
	return true; //TODO
}