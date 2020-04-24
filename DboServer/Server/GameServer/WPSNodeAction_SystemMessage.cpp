#include "stdafx.h"
#include "WPSNodeAction_SystemMessage.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlAdmin.h"
#include "NtlStringHandler.h"


CWPSNodeAction_SystemMessage::CWPSNodeAction_SystemMessage(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SYSTEM_MESSAGE, lpszName)
{
	m_textTblidx = INVALID_TBLIDX;
}

CWPSNodeAction_SystemMessage::~CWPSNodeAction_SystemMessage()
{

}

CWPSNodeAction_SystemMessage::sPARAMETER_INFO* CWPSNodeAction_SystemMessage::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_SystemMessage::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "text index") == 0)
	{
		m_textTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_SystemMessage: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_SystemMessage::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "notify") == 0)
	{
		if (_stricmp(pNode->GetValue(), "system") == 0)
		{
			m_eDisplayType = SERVER_TEXT_SYSTEM;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "notice") == 0)
		{
			m_eDisplayType = SERVER_TEXT_NOTICE;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "sysnotice") == 0)
		{
			m_eDisplayType = SERVER_TEXT_SYSNOTICE;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "action") == 0)
		{
			m_eDisplayType = SERVER_TEXT_EMERGENCY;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "caution") == 0)
		{
			m_eDisplayType = SERVER_TEXT_CAUTION;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	if (_stricmp(name, "text") == 0)
	{
		std::wstring wstrText = s2ws(pNode->GetValue());
		NTL_SAFE_WCSCPY(m_awchMessage, wstrText.c_str());

		m_wMessageLengthInUnicode = (WORD)wstrText.length();

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_SystemMessage: [%s] not exist \n", name);
	return false;
}