#include "stdafx.h"
#include "SvrScrVariableContainer.h"
#include "ControlScriptNode.h"
#include "ControlStateEx.h"
#include "SvrScrVariableMap.h"



CSvrScrVariableContainer::CSvrScrVariableContainer(CControlState* pControlSate)
{
	m_eContainerType = CONTAINER_CONTROLSTATE;
	m_pVarialbeMap = NULL;
	m_pScriptNode = NULL;
	m_pControlState = pControlSate;
}
CSvrScrVariableContainer::CSvrScrVariableContainer(CControlScriptNode* pScriptNode)
{
	m_eContainerType = CONTAINER_SCRIPTNODE;
	m_pVarialbeMap = NULL;
	m_pScriptNode = pScriptNode;
	m_pControlState = NULL;
}

CSvrScrVariableContainer::~CSvrScrVariableContainer()
{

}


sSSD_VARIABLE* CSvrScrVariableContainer::GetVariable(std::string name)
{
	if (m_eContainerType)
	{
		if (m_eContainerType == CONTAINER_SCRIPTNODE)
		{
			if (m_pScriptNode)
			{
				return m_pScriptNode->GetVariable(name);
			}
		}
		else if (m_eContainerType == CONTAINER_CONTROLSTATE && m_pControlState)
		{
			return m_pControlState->GetVariable(name);
		}
	}
	else if (m_pVarialbeMap)
	{
		return m_pVarialbeMap->GetVariable(name);
	}

	return NULL;
}


eSSD_VARIABLE_RESULT CSvrScrVariableContainer::UpdateVariable(std::string name, sSSD_VARIABLE& sVariable)
{
	if (m_eContainerType)
	{
		if (m_eContainerType == CONTAINER_SCRIPTNODE)
		{
			if (m_pScriptNode)
				return m_pScriptNode->UpdateVariable(name, sVariable);
		}
		else if (m_eContainerType == CONTAINER_CONTROLSTATE)
		{
			if (m_pControlState)
				return m_pControlState->UpdateVariable(name, sVariable);
		}
	}
	else if (m_pVarialbeMap)
	{
		return m_pVarialbeMap->Update(name, sVariable);
	}

	return SSD_VARIABLE_RESULT_VARIABLE_NOT_EXIST;
}

