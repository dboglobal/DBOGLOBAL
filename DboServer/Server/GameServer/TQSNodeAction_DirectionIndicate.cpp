#include "stdafx.h"
#include "TQSNodeAction_DirectionIndicate.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlDirection.h"


CTQSNodeAction_DirectionIndicate::CTQSNodeAction_DirectionIndicate(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_DIRECTION_INDICATE, lpszName)
{
	m_bIndicate = false;
	m_bAutoHide = true;
	m_tblidx = INVALID_TBLIDX;
	m_vLoc.Reset();
}

CTQSNodeAction_DirectionIndicate::~CTQSNodeAction_DirectionIndicate()
{

}

CTQSNodeAction_DirectionIndicate::sPARAMETER_INFO* CTQSNodeAction_DirectionIndicate::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_DirectionIndicate::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "loc x") == 0)
	{
		m_vLoc.x = (float)pNode->GetValue();

		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "loc y") == 0)
	{
		m_vLoc.y = (float)pNode->GetValue();

		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "loc z") == 0)
	{
		m_vLoc.z = (float)pNode->GetValue();

		return CControlScriptNode::AddParam(pNode);
	}


	printf("CTQSNodeAction_DirectionIndicate: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeAction_DirectionIndicate::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();

	if (_stricmp(name, "apply") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bIndicate = true;

		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "indicate type") == 0)
	{
		if (_stricmp(value, "object") == 0)
			m_eIndicateType = DIRECTION_INDICATE_TYPE_TOBJECT;
		else if (_stricmp(value, "npc") == 0)
			m_eIndicateType = DIRECTION_INDICATE_TYPE_NPC;
		else if (_stricmp(value, "point") == 0)
			m_eIndicateType = DIRECTION_INDICATE_TYPE_POINT;
		else
			printf("CTQSNodeAction_DirectionIndicate: could not find value %s \n", value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "auto hide") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bAutoHide = true;
		else if (_stricmp(value, "false") == 0)
			m_bAutoHide = false;
		else
			printf("CTQSNodeAction_DirectionIndicate: 'auto hide' could not find value %s \n", value);

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_DirectionIndicate: [%s] not exist \n", name);
	return false;
}