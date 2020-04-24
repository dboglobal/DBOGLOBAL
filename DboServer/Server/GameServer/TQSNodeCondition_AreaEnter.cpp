#include "stdafx.h"
#include "TQSNodeCondition_AreaEnter.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeCondition_AreaEnter::CTQSNodeCondition_AreaEnter(const char* lpszName)
:CControlScriptNodeCondition(TQS_NODE_CONDITION_AREA_ENTER, lpszName)
{
	m_fRadius = INVALID_FLOAT;
	m_bAllMember = true;
	m_bTmqFail = true;
	m_dwTimeLimit = 0;
	m_vOrigin.Reset();
}

CTQSNodeCondition_AreaEnter::~CTQSNodeCondition_AreaEnter()
{

}

CTQSNodeCondition_AreaEnter::sPARAMETER_INFO* CTQSNodeCondition_AreaEnter::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeCondition_AreaEnter::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "origin x") == 0)
	{
		m_vOrigin.x = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "origin y") == 0)
	{
		m_vOrigin.y = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "origin z") == 0)
	{
		m_vOrigin.z = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "start x") == 0)
	{
		m_vStart.x = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "start y") == 0)
	{
		m_vStart.y = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "start z") == 0)
	{
		m_vStart.z = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "end x") == 0)
	{
		m_vEnd.x = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "end y") == 0)
	{
		m_vEnd.y = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "end z") == 0)
	{
		m_vEnd.z = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "radius") == 0)
	{
		m_fRadius = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}


	printf("CTQSNodeCondition_AreaEnter: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeCondition_AreaEnter::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "all member") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			m_bAllMember = true;
		else if (_stricmp(pNode->GetValue(), "false") == 0)
			m_bAllMember = false;
		else
		{
			printf("CTQSNodeCondition_AreaEnter::AddParam [%s] not available \n", pNode->GetValue());
			return false;
		}

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "tmq fail") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			m_bTmqFail = true;
		else if (_stricmp(pNode->GetValue(), "false") == 0)
			m_bTmqFail = false;
		else
		{
			printf("CTQSNodeCondition_AreaEnter::AddParam [%s] not available \n", pNode->GetValue());
			return false;
		}

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeCondition_AreaEnter: [%s] not exist \n", name);
	return false;
}