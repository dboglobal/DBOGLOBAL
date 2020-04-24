#include "stdafx.h"
#include "SPSNodeCondition_BSCheckNpc.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeCondition_BSCheckNpc::CSPSNodeCondition_BSCheckNpc(const char* lpszName)
:CControlScriptNodeCondition(SPS_NODE_CONDITION_BS_CHECK_NPC, lpszName)
{
	m_npcTblidx = INVALID_TBLIDX;
	m_bIsExist = true;
}

CSPSNodeCondition_BSCheckNpc::~CSPSNodeCondition_BSCheckNpc()
{
}


bool CSPSNodeCondition_BSCheckNpc::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "npc index") == 0)
	{
		m_npcTblidx = (TBLIDX)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeCondition_BSCheckNpc: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeCondition_BSCheckNpc::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "is exist") == 0)
	{
		const char* sub = pNode->GetName();

		if (_stricmp(sub, "true") == 0)
		{
			m_bIsExist = true;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "false") == 0)
		{
			m_bIsExist = false;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeCondition_BSCheckNpc: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeCondition_BSCheckNpc: [%s] not exist \n", name);
	return false;
}