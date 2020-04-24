#include "stdafx.h"
#include "SPSNodeAction_TargetFixAttack.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CSPSNodeAction_TargetFixAttack::CSPSNodeAction_TargetFixAttack(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_TARGETFIXATTACK, lpszName)
{
	m_eObjType = INVALID_OBJTYPE;
	m_tblidxBot = INVALID_TBLIDX;
	m_fScanRange = INVALID_FLOAT;
	m_fEndValue_Percent_Target_LP = INVALID_FLOAT;
	m_eEndType = INVALID_ENDTYPE;
}

CSPSNodeAction_TargetFixAttack::~CSPSNodeAction_TargetFixAttack()
{
}


bool CSPSNodeAction_TargetFixAttack::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "bot tblidx") == 0)
	{
		m_tblidxBot = (TBLIDX)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "scan range") == 0)
	{
		m_fScanRange = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "end value") == 0)
	{
		if (m_eEndType == ePERCENT_TARGET_LP)
		{
			m_fEndValue_Percent_Target_LP = (float)pNode->GetValue();
			return CControlScriptNode::AddParam(pNode);
		}
	}

	printf("CSPSNodeAction_Escort: [%s] not exist \n", name);
	return false;
}


bool CSPSNodeAction_TargetFixAttack::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "obj type") == 0)
	{
		const char* sub = pNode->GetValue();

		if (_stricmp(sub, "pc") == 0)
		{
			m_eObjType = OBJTYPE_PC;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "npc") == 0)
		{
			m_eObjType = OBJTYPE_NPC;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "mob") == 0)
		{
			m_eObjType = OBJTYPE_MOB;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_TargetFixAttack: sub [%s] not exist \n", sub);
		return false;
	}
	else if (_stricmp(name, "end type") == 0)
	{
		const char* sub = pNode->GetValue();

		if (_stricmp(sub, "percent target lp") == 0)
		{
			m_eEndType = ePERCENT_TARGET_LP;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_TargetFixAttack: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeAction_TargetFixAttack: [%s] not exist \n", name);
	return false;
}