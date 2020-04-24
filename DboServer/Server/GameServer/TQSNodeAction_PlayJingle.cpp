#include "stdafx.h"
#include "TQSNodeAction_PlayJingle.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeAction_PlayJingle::CTQSNodeAction_PlayJingle(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_PLAY_JINGLE, lpszName)
{
	m_bApplyFadeInOut = true;
}

CTQSNodeAction_PlayJingle::~CTQSNodeAction_PlayJingle()
{

}

CTQSNodeAction_PlayJingle::sPARAMETER_INFO* CTQSNodeAction_PlayJingle::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_PlayJingle::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "name") == 0)
	{
		m_strName = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_PlayJingle: [%s] not exist \n", name);
	return false;
}