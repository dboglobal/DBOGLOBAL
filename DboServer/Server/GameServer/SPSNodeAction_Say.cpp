#include "stdafx.h"
#include "SPSNodeAction_Say.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"


CSPSNodeAction_Say::CSPSNodeAction_Say(const char* lpszName)
: CControlScriptNodeAction(SPS_NODE_ACTION_SAY, lpszName)
{
	m_byDialogType = CHAR_DIALOG_SAY;
	m_textTblidx = INVALID_TBLIDX;
	m_strText.clear();
}

CSPSNodeAction_Say::~CSPSNodeAction_Say()
{
}


CControlScriptNode::sPARAMETER_INFO* CSPSNodeAction_Say::GetParameterMap()
{
	return NULL;
}


bool CSPSNodeAction_Say::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "tblidx") == 0)
	{
		m_textTblidx = (TBLIDX)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_Say: [%s] not exist \n", name);

	return false;
}

bool CSPSNodeAction_Say::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "say") == 0)
			m_byDialogType = CHAR_DIALOG_SAY;
		else if (_stricmp(pNode->GetValue(), "shout") == 0)
			m_byDialogType = CHAR_DIALOG_SHOUT;
		else if (_stricmp(pNode->GetValue(), "think") == 0)
			m_byDialogType = CHAR_DIALOG_THINK;
		else
		{
			printf("CSPSNodeAction_Say::AddParam [%s] not available \n", pNode->GetValue());
			return false;
		}

		return CControlScriptNode::AddParam(pNode);
	}
	else
	{
		if (_stricmp(name, "text") == 0)
		{
			m_strText.operator=(pNode->GetValue());

			return CControlScriptNode::AddParam(pNode);
		}
	}

	printf("CSPSNodeAction_Say: [%s] not exist \n", name);

	return false;
}