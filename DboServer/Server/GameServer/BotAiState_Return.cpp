#include "stdafx.h"
#include "BotAiState_Return.h"
#include "BotAiAction_ReturnMove.h"
#include "BotAiAction_Rotate.h"
#include "BotAiCondition_AttackTarget.h"
#include "ApplyAiFunc.h"


CBotAiState_Return::CBotAiState_Return(CNpc* pBot, eBOT_RETURNMODE eReturnMode, CNtlVector& rReturnLoc, CNtlVector& rReturnDir)
	:CBotAiState(pBot, BOTCONTROL_RETURN, "BOTCONTROL_RETURN")
{
	m_vReturnLoc.operator=(rReturnLoc);
	m_vReturnDir.operator=(rReturnDir);
	m_eReturnMode = eReturnMode;
	m_qwCharConditionFlag = 0;

	Init();
}

CBotAiState_Return::~CBotAiState_Return()
{
}

void CBotAiState_Return::Init()
{
	this->m_IsBotInit = false;
	this->m_bIsScan = false;
	this->m_fRunSpeed = 150.0f; //looks like run speed will be gained by 150%
	this->m_fWalkSpeed = 150.0f;//looks like walk speed will be gained by 150%

	BIT_FLAG_SET(m_qwCharConditionFlag, MAKE_BIT_FLAG(CHARCOND_ATTACK_DISALLOW));
	BIT_FLAG_SET(m_qwCharConditionFlag, MAKE_BIT_FLAG(CHARCOND_CLICK_DISABLE));
	BIT_FLAG_SET(m_qwCharConditionFlag, MAKE_BIT_FLAG(CHARCOND_INVINCIBLE));
}


void CBotAiState_Return::OnEnter()
{
	//REMOVE TARGET
	GetBot()->ChangeTarget(INVALID_HOBJECT);

	GetBot()->ClearCrowdControlReduction();

	//CLEAR AGGRO
	GetBot()->GetTargetListManager()->ClearAggro();

	//UPDATE CONDITION
	GetBot()->GetStateManager()->AddConditionFlags(m_qwCharConditionFlag, true);//set char conditions

	//UPDATE MOVEMENT SPEED
	//CPacketSender::Send_SetCharSpeedUp(v12, v41, v50->m_fRunSpeed, v50->m_fWalkSpeed);

	if (GetBot()->GetCurLoc() != m_vReturnLoc)
	{
		CNtlVector vDir(CNtlVector::ZERO);
		if (!m_vReturnDir.IsZero())
			vDir.operator=(m_vReturnDir);

		CBotAiAction_ReturnMove* pReturnMove = new CBotAiAction_ReturnMove(GetBot(), m_vReturnLoc, vDir);
		if (!AddSubControlQueue(pReturnMove, true))
		{
			ERR_LOG(LOG_BOTAI, "false == AddSubControlQueue( new CBotAiAction_ReturnMove(GetBot(), m_vReturnLoc, vDir) )");
		}
	}
	else if (!m_vReturnDir.IsZero())
	{
		CBotAiAction_Rotate* pRotate = new CBotAiAction_Rotate(GetBot(), m_vReturnDir);
		if (!AddSubControlQueue(pRotate, true))
		{
			ERR_LOG(LOG_BOTAI, "false == AddSubControlQueue( new CBotAiAction_Rotate( GetBot(), m_vReturnDir ) )");
		}
	}

	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_DOING_RETURN, true);
}

void CBotAiState_Return::OnExit()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_DOING_RETURN, false);

	GetBot()->GetStateManager()->RemoveConditionFlags(m_qwCharConditionFlag, true);//unset char conditions

	//CPacketSender::Send_SetCharSpeedUp(v8, v13, 100.0, 100.0); //looks like run and walk speed changes back to 100%
}

int CBotAiState_Return::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	int nStatus = CComplexState::OnUpdate(dwTickDiff, fMultiple);

	if (nStatus == COMPLETED)
		m_status = COMPLETED;
	else if (nStatus == CHANGED || nStatus == 8)
	{
		m_status = INACTIVATED;
	}

	return m_status;
}
