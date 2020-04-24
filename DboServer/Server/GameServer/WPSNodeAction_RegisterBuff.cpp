#include "stdafx.h"
#include "WPSNodeAction_RegisterBuff.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CWPSNodeAction_RegisterBuff::CWPSNodeAction_RegisterBuff(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_REGISTER_BUFF, lpszName)
{
}

CWPSNodeAction_RegisterBuff::~CWPSNodeAction_RegisterBuff()
{

}

CWPSNodeAction_RegisterBuff::sPARAMETER_INFO* CWPSNodeAction_RegisterBuff::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_RegisterBuff::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target index") == 0)
	{
		m_dwTargetIndex = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "buff index") == 0)
	{
		m_SkillId = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_RegisterBuff: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_RegisterBuff::AddParam(CControlScriptNodeParam_String* pNode)
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

	printf("CWPSNodeAction_RegisterBuff: [%s] not exist \n", name);
	return false;
}