#include "stdafx.h"
#include "SPSNodeAction_ChangeAiBitFlag.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeAction_ChangeAiBitFlag::CSPSNodeAction_ChangeAiBitFlag(const char* lpszName, eSCRIPTTYPE eScriptType)
: m_eScriptType(eScriptType), CControlScriptNodeAction(SPS_NODE_ACTION_CHANGEAIBITFLAG, lpszName)
{
	m_dwAi_Bit_Flag = 0;
	m_dwAi_Bit_Mask = 0;
	m_eScriptType = eScriptType;
}

CSPSNodeAction_ChangeAiBitFlag::~CSPSNodeAction_ChangeAiBitFlag()
{

}

CControlScriptNode::sPARAMETER_INFO* CSPSNodeAction_ChangeAiBitFlag::GetParameterMap()
{
	return NULL;
}


bool CSPSNodeAction_ChangeAiBitFlag::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "attack type") == 0)
	{
		m_dwAi_Bit_Flag &= 0xFFFFFFFE;
		m_dwAi_Bit_Flag &= 0xFFFFFFFD;
		m_dwAi_Bit_Flag &= 0xFFFFFFFB;
		m_dwAi_Bit_Flag &= 0xFFFFFFF7;
		m_dwAi_Bit_Mask |= 1;
		m_dwAi_Bit_Mask |= 2;
		m_dwAi_Bit_Mask |= 4;
		m_dwAi_Bit_Mask |= 8;

		const char* value = pNode->GetValue();

		if (_stricmp(value, "defensive") == 0)
		{
			m_dwAi_Bit_Mask |= 1;
			m_dwAi_Bit_Flag |= 1;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "timid") == 0)
		{
			m_dwAi_Bit_Mask |= 2;
			m_dwAi_Bit_Flag |= 2;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "offensive") == 0)
		{
			m_dwAi_Bit_Mask |= 4;
			m_dwAi_Bit_Flag |= 4;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "smart offensive") == 0)
		{
			m_dwAi_Bit_Mask |= 8;
			m_dwAi_Bit_Flag |= 8;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_ChangeAiBitFlag: value [%s] not exist \n", value);
		return false;
	}


	if (_stricmp(name, "flee type") == 0)
	{
		const char* value = pNode->GetValue();

		if (_stricmp(value, "desperation off") == 0)
		{
			m_dwAi_Bit_Mask |= 0x20;
			m_dwAi_Bit_Flag &= 0xFFFFFFDF;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "flee off") == 0)
		{
			m_dwAi_Bit_Mask |= 0x40;
			m_dwAi_Bit_Flag &= 0xFFFFFFBF;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "retreat off") == 0)
		{
			m_dwAi_Bit_Mask |= 0x80;
			m_dwAi_Bit_Flag &= 0xFFFFFF7F;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "desperation on") == 0)
		{
			m_dwAi_Bit_Mask |= 0x20;
			m_dwAi_Bit_Flag |= 0x20;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "flee on") == 0)
		{
			m_dwAi_Bit_Mask |= 0x40;
			m_dwAi_Bit_Flag |= 0x40;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "retreat on") == 0)
		{
			m_dwAi_Bit_Mask |= 0x80;
			m_dwAi_Bit_Flag |= 0x80;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_ChangeAiBitFlag: value [%s] not exist \n", value);
		return false;
	}


	if (_stricmp(name, "targetting type") == 0)
	{
		m_dwAi_Bit_Flag &= 0xFFFFFDFF;
		m_dwAi_Bit_Flag &= 0xFFFFFBFF;
		m_dwAi_Bit_Flag &= 0xFFFFF7FF;
		m_dwAi_Bit_Flag &= 0xFFFFEFFF;
		m_dwAi_Bit_Mask |= 0x200;
		m_dwAi_Bit_Mask |= 0x400;
		m_dwAi_Bit_Mask |= 0x800;
		m_dwAi_Bit_Mask |= 0x1000;

		const char* value = pNode->GetValue();

		if (_stricmp(value, "revenge") == 0)
		{
			m_dwAi_Bit_Flag |= 0x400;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "mean") == 0)
		{
			m_dwAi_Bit_Flag |= 0x800;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "brave") == 0)
		{
			m_dwAi_Bit_Flag |= 0x1000;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_ChangeAiBitFlag: value [%s] not exist \n", value);
		return false;
	}

	if (_stricmp(name, "help type") == 0)
	{
		const char* value = pNode->GetValue();

		if (_stricmp(value, "alliance help off") == 0)
		{
			m_dwAi_Bit_Mask |= 0x8000;
			m_dwAi_Bit_Flag &= 0xFFFF7FFF;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "defend off") == 0)
		{
			m_dwAi_Bit_Mask |= 0x10000;
			m_dwAi_Bit_Flag &= 0xFFFEFFFF;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "alliance help on") == 0)
		{
			m_dwAi_Bit_Mask |= 0x8000u;
			m_dwAi_Bit_Flag |= 0x8000u;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "defend on") == 0)
		{
			m_dwAi_Bit_Mask |= 0x10000;
			m_dwAi_Bit_Flag |= 0x10000;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_ChangeAiBitFlag: value [%s] not exist \n", value);
		return false;
	}


	if (_stricmp(name, "move type") == 0)
	{
		const char* value = pNode->GetValue();

		if (_stricmp(value, "chace on") == 0)
		{
			m_dwAi_Bit_Mask |= 0x100000;
			m_dwAi_Bit_Flag &= 0xFFEFFFFF;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "move on") == 0)
		{
			m_dwAi_Bit_Mask |= 0x200000;
			m_dwAi_Bit_Flag &= 0xFFDFFFFF;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "chace off") == 0)
		{
			m_dwAi_Bit_Mask |= 0x100000;
			m_dwAi_Bit_Flag |= 0x100000;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "move off") == 0)
		{
			m_dwAi_Bit_Mask |= 0x200000;
			m_dwAi_Bit_Flag |= 0x200000;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_ChangeAiBitFlag: value [%s] not exist \n", value);
		return false;
	}


	if (_stricmp(name, "only skill") == 0)
	{
		const char* value = pNode->GetValue();

		if (_stricmp(value, "on") == 0)
		{
			m_dwAi_Bit_Mask |= 0x10;
			m_dwAi_Bit_Flag |= 0x10;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "off") == 0)
		{
			m_dwAi_Bit_Mask |= 0x10;
			m_dwAi_Bit_Flag &= 0xFFFFFFEF;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_ChangeAiBitFlag: value [%s] not exist \n", value);
		return false;
	}


	printf("CSPSNodeAction_ChangeAiBitFlag: [%s] not exist \n", name);
	return false;
}