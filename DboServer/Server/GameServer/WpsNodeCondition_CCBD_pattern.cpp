#include "stdafx.h"
#include "WpsNodeCondition_CCBD_pattern.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"


CWpsNodeCondition_CCBD_pattern::CWpsNodeCondition_CCBD_pattern(const char* lpszName)
	:CControlScriptNodeCondition(WPS_NODE_CONDITION_CCBD_PATTERN, lpszName)
{
	m_byIndex = INVALID_BYTE;
	m_titleIdx = INVALID_TBLIDX;
	m_subTitleIdx = INVALID_TBLIDX;
	m_bUseTimeLimit = false;
}

CWpsNodeCondition_CCBD_pattern::~CWpsNodeCondition_CCBD_pattern()
{

}



bool CWpsNodeCondition_CCBD_pattern::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_byIndex = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "title tblidx") == 0)
	{
		m_titleIdx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "sub title tblidx") == 0)
	{
		m_subTitleIdx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_CCBD_pattern: [%s] not exist \n", name);
	return false;
}

bool CWpsNodeCondition_CCBD_pattern::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();

	if (_stricmp(name, "use limit timer") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bUseTimeLimit = true;
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_CCBD_pattern: [%s] not exist \n", name);
	return false;
}


