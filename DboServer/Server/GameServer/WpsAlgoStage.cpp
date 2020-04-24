#include "stdafx.h"
#include "WpsAlgoStage.h"
#include "WorldPlayControlFactory.h"
#include "WorldPlayScriptManager.h"
#include "WpsScriptAlgoAction_CCBD_stage.h"


CWpsAlgoStage::CWpsAlgoStage(BYTE byStageNumber, CWpsAlgoObject* pOwner) :
CScriptAlgoStage(byStageNumber, pOwner, SCRIPTCONTROL_STAGE, "SCRIPTCONTROL_STAGE")
{
}


CWpsAlgoStage::~CWpsAlgoStage()
{
}



void CWpsAlgoStage::OnEnter()
{
	if (CreateVariableMap())
	{
		CWorldPlayControlFactory factory(GetOwner());
		if (!g_pWorldPlayScriptManager->AttachScript(this, &factory, GetOwner()->GetScriptID(), GetStageNumber()))
		{
			ERR_LOG(LOG_SCRIPT, "FAIL : CWpsAlgoStage::OnEnter GetStageNumber %u GetScriptID %u\n", GetStageNumber(), GetOwner()->GetScriptID());
			m_status = SYSTEMERROR;
		}

		if (GetOwner()->GetCCBDStartStage() > 1)
		{
			BYTE byCurStage = 1;
			CControlStateComposite* pLinkObject = (CControlStateComposite*)m_subControlQueue.GetFirst();
			if (pLinkObject)
			{
				CControlStateComposite* pNextState = NULL;

				while (pLinkObject)
				{
					pNextState = (CControlStateComposite*)pLinkObject->GetNext();

					if (pLinkObject->GetControlStateID() == SCRIPTCONTROL_ACTION_MOB_LIST && byCurStage < GetOwner()->GetCCBDStartStage())
					{
						m_subControlQueue.Remove(pLinkObject);
						SAFE_DELETE(pLinkObject);
					}
					else if (pLinkObject->GetControlStateID() == SCRIPTCONTROL_ACTION_CCBD_STAGE)
					{
						CWpsScriptAlgoAction_CCBD_stage* pCCBDStage = (CWpsScriptAlgoAction_CCBD_stage*)pLinkObject;

						if (pCCBDStage->m_byStage < GetOwner()->GetCCBDStartStage())
						{
							byCurStage = pCCBDStage->m_byStage;

							if (pCCBDStage->m_byStage == GetOwner()->GetCCBDStartStage() - 1) //check if this is the last stage to skip
							{
								m_subControlQueue.Remove(pLinkObject);
								SAFE_DELETE(pLinkObject);
								break;
							}
							else
							{
								m_subControlQueue.Remove(pLinkObject);
								SAFE_DELETE(pLinkObject);
							}
						}
					}

					pLinkObject = pNextState;
				}
			}
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "false == CreateVariableMap()");
		m_status = SYSTEMERROR;
	}
}


int CWpsAlgoStage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nStatus = UpdateSubControlList(dwTickDiff, fMultiple); //UPDATE CONDITION
	nStatus = UpdateSubControlQueue(dwTickDiff, fMultiple); //UPDATE ACTTION

	if (nStatus == COMPLETED)//end script
	{
		m_status = COMPLETED;
	//	NTL_PRINT(PRINT_APP, "Stage status %u", nStatus);
		GetOwner()->Stop();
	}

	return m_status;
}

void CWpsAlgoStage::AddCCBDPattern(BYTE byIndex, CControlState * pPattern)
{
	RemCCBDPattern(byIndex); //rem old

	m_ccbdPattern.insert({ byIndex , pPattern });
}

CControlState * CWpsAlgoStage::GetCCBDPattern(BYTE byIndex)
{
	std::map<BYTE, CControlState*>::iterator it = m_ccbdPattern.find(byIndex);
	if (it == m_ccbdPattern.end())
		return NULL;

	return it->second;
}

void CWpsAlgoStage::RemCCBDPattern(BYTE byIndex)
{
	m_ccbdPattern.erase(byIndex);
}

void CWpsAlgoStage::InitMobList()
{
	m_vecMobList.reserve(10);
}

void CWpsAlgoStage::SetMobList(CControlScriptNode * pNode, int nIndex)
{
	m_vecMobList[nIndex] = pNode;
}

