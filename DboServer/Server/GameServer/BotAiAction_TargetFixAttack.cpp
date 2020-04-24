#include "stdafx.h"
#include "BotAiAction_TargetFixAttack.h"
#include "SPSNodeAction_TargetFixAttack.h"
#include "EndCondition_TargetFixAttack.h"



CBotAiAction_TargetFixAttack::CBotAiAction_TargetFixAttack(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_TARGETFIXATTACK, "BOTCONTROL_ACTION_TARGETFIXATTACK")
{
	m_scriptnodeID = INVALID_DWORD;
	m_eObjType = INVALID_OBJTYPE;
	m_tblidxBot = INVALID_TBLIDX;
	m_fScanRange = INVALID_FLOAT;
	m_fEndValue_Percent_Target_LP = INVALID_FLOAT;
	m_eEndType = CSPSNodeAction_TargetFixAttack::INVALID_ENDTYPE;
	m_dwTime = 0;
	m_hTarget = INVALID_HOBJECT;
}


CBotAiAction_TargetFixAttack::~CBotAiAction_TargetFixAttack()
{
}


bool CBotAiAction_TargetFixAttack::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_TargetFixAttack* pAction = dynamic_cast<CSPSNodeAction_TargetFixAttack*>(pControlScriptNode);
	if (pAction)
	{
		m_scriptnodeID = pControlScriptNode->GetNodeIndex();
		m_eObjType = pAction->m_eObjType;
		
		if (m_eObjType >= OBJTYPE_PC && m_eObjType <= OBJTYPE_MOB)
		{
			if (m_eObjType != OBJTYPE_PC)
				m_tblidxBot = pAction->m_tblidxBot;

			m_fScanRange = pAction->m_fScanRange;
			m_eEndType = pAction->m_eEndType;

			if (m_eEndType == CSPSNodeAction_TargetFixAttack::ePERCENT_TARGET_LP)
			{
				m_fEndValue_Percent_Target_LP = pAction->m_fEndValue_Percent_Target_LP;
			}
			else
			{
				ERR_LOG(LOG_SCRIPT, "fail : m_eEndType( %d )", m_eEndType);
				return false;
			}
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "fail : m_eEndType( %d )", m_eEndType);
			return false;
		}

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_TargetFixAttack", pControlScriptNode);
	return false;
}

void CBotAiAction_TargetFixAttack::CopyTo(CControlState* pTo)
{
	CBotAiAction_TargetFixAttack* pAction = dynamic_cast<CBotAiAction_TargetFixAttack*>(pTo);
	if (pAction)
	{
		pAction->m_scriptnodeID = m_scriptnodeID;
		pAction->m_eObjType = m_eObjType;
		pAction->m_tblidxBot = m_tblidxBot;
		pAction->m_fScanRange = m_fScanRange;
		pAction->m_fEndValue_Percent_Target_LP = m_fEndValue_Percent_Target_LP;
		pAction->m_eEndType = m_eEndType;
		pAction->m_dwTime = m_dwTime;
		pAction->m_hTarget = m_hTarget;
	}
}

void CBotAiAction_TargetFixAttack::OnEnter()
{
	if (GetBot()->GetObjType() != OBJTYPE_NPC && GetBot()->GetObjType() != OBJTYPE_MOB)
	{
		ERR_LOG(LOG_BOTAI, "fail : Not Npc or Mob");
		m_status = FAILED;
		return;
	}

	if (m_eObjType == OBJTYPE_PC)
	{
		m_hTarget = GetBot()->ConsiderScanTargetFixedPc((WORD)floor(m_fScanRange));
		if (m_hTarget == INVALID_HOBJECT)
			return;
	}
	else if (m_eObjType == OBJTYPE_NPC || m_eObjType == OBJTYPE_MOB)
	{
		m_hTarget = GetBot()->ConsiderScanObject(m_tblidxBot, (WORD)floor(m_fScanRange));
		if (m_hTarget == INVALID_HOBJECT)
			return;
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : m_eObjType( %d )", m_eObjType);
		m_status = FAILED;
		return;
	}

	CCharacter* pTarget = g_pObjectManager->GetChar(m_eObjType, m_hTarget);
	if (pTarget && pTarget->IsInitialized())
	{
		GetBot()->SetFixedTarget(m_hTarget);

		CEndCondition_TargetFixAttack* pEndCondition = new CEndCondition_TargetFixAttack(GetBot(), m_hTarget, m_eObjType, m_eEndType, m_fEndValue_Percent_Target_LP);
		if (pEndCondition)
		{
			if (!GetBot()->GetBotController()->GetCheckupEndCondition()->AddEndCondition(m_scriptnodeID, pEndCondition))
			{
				GetBot()->SetFixedTarget(INVALID_HOBJECT);
				m_status = FAILED;
			}
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : NULL == pEndCondition");
			GetBot()->SetFixedTarget(INVALID_HOBJECT);
			m_status = FAILED;
		}
	}
}

void CBotAiAction_TargetFixAttack::OnExit()
{
}


int CBotAiAction_TargetFixAttack::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_status = COMPLETED;
	return m_status;
}