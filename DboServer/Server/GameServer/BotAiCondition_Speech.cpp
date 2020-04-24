#include "stdafx.h"
#include "BotAiCondition_Speech.h"
#include "NtlRandom.h"
#include "NtlPacketGU.h"


CBotAiCondition_Speech::CBotAiCondition_Speech(CNpc* pBot, eNPC_SPEECH_TYPE eSpeechType, DWORD dwCheckInterval, bool bDisposable, bool bPreCheck)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_SPEECH, "BOTCONTROL_CONDITION_SPEECH")
{
	m_dwTime = 0;
	m_eSpeechType = eSpeechType;
	m_dwCheckInterval = dwCheckInterval;
	m_bDisposable = bDisposable;
	m_bPreCheck = bPreCheck;
}

CBotAiCondition_Speech::~CBotAiCondition_Speech()
{
}


void CBotAiCondition_Speech::OnEnter()
{
	if (m_bPreCheck)
		m_dwTime = m_dwCheckInterval;

	if (GetBot()->GetSpeechGroup())
	{
		if (!GetBot()->GetSpeechGroup()->HasSpeechData(m_eSpeechType))
			m_status = COMPLETED;
	}
	else
	{
		m_status = COMPLETED;
	}
}

int CBotAiCondition_Speech::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);

	if (m_dwCheckInterval <= m_dwTime)
	{
		m_dwTime = 0;

		if (!GetBot()->GetStateManager()->IsCharCondition(CHARCOND_CLIENT_UI_DISABLE))
		{
			if (GetBot()->HasNearbyPlayer(true))
			{
				TBLIDX textTblidx = GetBot()->GetSpeechGroup()->GetSpeechData(m_eSpeechType, RandomRange(1, 100));
				if (textTblidx == INVALID_TBLIDX)
				{
					if (m_bDisposable == true)
						m_status = COMPLETED;
				}
				else
				{
					CNtlPacket packet(sizeof(sGU_CHAR_DIALOG));
					sGU_CHAR_DIALOG * res = (sGU_CHAR_DIALOG *)packet.GetPacketData();
					res->wOpCode = GU_CHAR_DIALOG;
					res->hSubject = GetBot()->GetID();
					res->bIsRefSpeechTable = true;
					res->byDialogType = INVALID_CHAR_DIALOG;
					res->textTblidx = textTblidx;
					packet.SetPacketLen(sizeof(sGU_CHAR_DIALOG));
					GetBot()->Broadcast(&packet);

					if (m_bDisposable)
						m_status = COMPLETED;
				}
			}
		}
	}

	return m_status;
}
