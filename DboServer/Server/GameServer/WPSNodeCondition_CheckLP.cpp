#include "stdafx.h"
#include "WPSNodeCondition_CheckLP.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CWPSNodeCondition_CheckLP::CWPSNodeCondition_CheckLP(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_CHECK_LP, lpszName)
{
	m_eObjType = INVALID_OBJTYPE;
	m_index = INVALID_TBLIDX;
	m_group = INVALID_SPAWNGROUPID;
	m_byPercent = INVALID_BYTE;
}

CWPSNodeCondition_CheckLP::~CWPSNodeCondition_CheckLP()
{

}

bool CWPSNodeCondition_CheckLP::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_index = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "group") == 0)
	{
		m_group = (SPAWNGROUPID)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "lp") == 0)
	{
		m_byPercent = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeCondition_CheckLP: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeCondition_CheckLP::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "mob") == 0)
		{
			m_eObjType = OBJTYPE_MOB;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(pNode->GetValue(), "npc") == 0)
		{
			m_eObjType = OBJTYPE_NPC;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeCondition_CheckLP: [%s] not exist \n", name);
	return false;
}