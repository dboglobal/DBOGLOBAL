#include "stdafx.h"
#include "BotAiState_Faint.h"
#include "BotAiCondition_Speech.h"


CBotAiState_Faint::CBotAiState_Faint(CNpc* pBot)
	:CBotAiState(pBot, BOTCONTROL_FAINT, "BOTCONTROL_FAINT")
{
	m_dwTime = 0;
	m_dwFaint_Leave_Time = 0;
}

CBotAiState_Faint::~CBotAiState_Faint()
{
}


void CBotAiState_Faint::OnEnter()
{
	if (GetBot()->GetSpeechGroup())
	{
		if (GetBot()->GetSpeechGroup()->HasSpeechData(NPC_SPEECH_TYPE_FAINT))
		{
			if (!AddSubControlList(new CBotAiCondition_Speech(GetBot(), NPC_SPEECH_TYPE_FAINT, 0, true, false), false))
			{
				m_status = FAILED;
				return;
			}
		}
	}

	m_dwFaint_Leave_Time = (DWORD)GetBot()->GetAniTbldat()->GetAnimationLength(FAINTING) + 8000;
}

void CBotAiState_Faint::OnExit()
{
}

int CBotAiState_Faint::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CComplexState::UpdateSubControlList(dwTickDiff, fMultiple);

	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime >= m_dwFaint_Leave_Time)
	{
		GetBot()->GetBotController()->ChangeControlState_Leave();
		m_status = COMPLETED;
	}

	return m_status;
}
