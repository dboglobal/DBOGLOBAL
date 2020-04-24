#include "stdafx.h"
#include "WPSNodeAction_ChangeCharAttribute.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CWPSNodeAction_ChangeCharAttribute::CWPSNodeAction_ChangeCharAttribute(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_CHANGE_CHAR_ATTRIBUTE, lpszName)
{
	m_eTargetType = INVALID_OBJTYPE;
	m_dwTargetIndex = INVALID_DWORD;
	m_bUseNickname = false;
	m_nicknameTblidx = INVALID_TBLIDX;
	m_bySizeRate = 10;
	m_byImmortalMode = eIMMORTAL_MODE_OFF;
	m_byLpBlockRate = INVALID_BYTE;
}

CWPSNodeAction_ChangeCharAttribute::~CWPSNodeAction_ChangeCharAttribute()
{

}

CWPSNodeAction_ChangeCharAttribute::sPARAMETER_INFO* CWPSNodeAction_ChangeCharAttribute::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_ChangeCharAttribute::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target index") == 0)
	{
		m_dwTargetIndex = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "size rate") == 0)
	{
		m_bySizeRate = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_ChangeCharAttribute: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_ChangeCharAttribute::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "mob") == 0)
		{
			m_eTargetType = OBJTYPE_MOB;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_ChangeCharAttribute: [%s] not exist \n", name);
	return false;
}