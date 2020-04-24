#include "stdafx.h"
#include "ControlState.h"
#include "SvrScrVariableMap.h"


CControlState::CControlState(int controlStateID, const char* lpszControlStateName)
:m_controlStateID(controlStateID), m_lpszControlStateName(lpszControlStateName)
{
	m_pParent = NULL;
	m_status = INACTIVATED;
	m_controlStateID = controlStateID;
	m_lpszControlStateName = lpszControlStateName;
	m_dwConditionLoopCount = INVALID_DWORD;
	m_pVariableMap = NULL;
	m_bDoAttachChild = true;
}

CControlState::~CControlState()
{
	if (GetPrev())
	{
		delete GetPrev();
	}
}

void CControlState::Start()
{
	if (IsStatus(INACTIVATED))
	{
		m_status = ACTIVATED;
		OnEnter();
	}
}

void CControlState::Stop()
{
	if (!IsStatus(INACTIVATED))
	{
		OnExit();
		m_status = INACTIVATED;
	}
}

void CControlState::CopyTo(CControlState * destState)
{
	ERR_LOG(LOG_SCRIPT, "can not use this function in parent clasS");
}

bool CControlState::CreateVariableMap()
{
	if (m_pVariableMap)
	{
		ERR_LOG(LOG_SCRIPT, "Variable map already exist");
		return false;
	}

	m_pVariableMap = new CSvrScrVariableMap;

	if (m_pVariableMap)
	{
		return m_pVariableMap->Create();
	}

	return false;
}

CSvrScrVariableMap* CControlState::GetValidVariableMap()
{
	for (CControlState* j = this; j; j = j->GetParent())
	{
		if (j->GetCurrentVariableMap())
			return j->GetCurrentVariableMap();
	}

	return NULL;
}

sSSD_VARIABLE* CControlState::GetVariable(std::string& name, eSSD_VARIABLE_TYPE eVarType)
{
	for (CControlState* j = this; j; j = j->GetParent())
	{
		if (j->GetCurrentVariableMap())
		{
			sSSD_VARIABLE* sVar = j->GetCurrentVariableMap()->GetVariable(name);
			if (sVar)
			{
				if (sVar->eType == eVarType)
					return sVar;
			}
		}
	}

	return NULL;
}

sSSD_VARIABLE* CControlState::GetVariable(std::string& name)
{
	for (CControlState* j = this; j; j = j->GetParent())
	{
		if (j->GetCurrentVariableMap())
		{
			sSSD_VARIABLE* sVar = j->GetCurrentVariableMap()->GetVariable(name);
			if (sVar)
				return sVar;
		}
	}

	return NULL;
}

eSSD_VARIABLE_RESULT CControlState::UpdateVariable(std::string& name, sSSD_VARIABLE& sVariable)
{
	for (CControlState* j = this; j; j = j->GetParent())
	{
		if (j->GetCurrentVariableMap())
		{
			eSSD_VARIABLE_RESULT eRes = j->GetCurrentVariableMap()->Update(name, sVariable);

			if (eRes != SSD_VARIABLE_RESULT_VARIABLE_NOT_EXIST)
			{
				return eRes;
			}
		}
	}

	return SSD_VARIABLE_RESULT_VARIABLE_NOT_EXIST;
}



















