#include "stdafx.h"
#include "TQSNodeCondition_NpcFaint.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeCondition_NpcFaint::CTQSNodeCondition_NpcFaint(const char* lpszName)
:CControlScriptNodeCondition(TQS_NODE_CONDITION_NPC_FAINT, lpszName)
{
}

CTQSNodeCondition_NpcFaint::~CTQSNodeCondition_NpcFaint()
{

}

CTQSNodeCondition_NpcFaint::sPARAMETER_INFO* CTQSNodeCondition_NpcFaint::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeCondition_NpcFaint::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "npc index") == 0)
	{
		m_npcTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeCondition_NpcFaint: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeCondition_NpcFaint::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();


	if (_stricmp(name, "tmq fail") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bTmqFail = true;
		else if (_stricmp(value, "false") == 0)
			m_bTmqFail = false;
		else
			printf("CTQSNodeCondition_NpcFaint: name[%s] value '%s' not exist \n", name, value);

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeCondition_NpcFaint: [%s] not exist \n", name);
	return false;
}