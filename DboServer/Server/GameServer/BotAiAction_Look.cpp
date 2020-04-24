#include "stdafx.h"
#include "BotAiAction_Look.h"
#include "ObjectManager.h"


CBotAiAction_Look::CBotAiAction_Look(CNpc* pBot, HOBJECT hTarget, DWORD dwWaitTime)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_LOOK, "BOTCONTROL_ACTION_LOOK"),
	m_sAggroData(hTarget, 0, 0)
{
	m_hTarget = hTarget;
	m_bIsAggroData = false;
	m_dwWaitTime = dwWaitTime;
	m_dwTime = 0;
}


CBotAiAction_Look::~CBotAiAction_Look()
{
}


void CBotAiAction_Look::OnEnter()
{
	sBOT_AGGROPOINT* sAggro = GetBot()->GetTargetListManager()->GetAggroPoint(GetBot()->GetTargetHandle());
	if (sAggro)
	{
		m_bIsAggroData = true;
		m_sAggroData = *sAggro;
	}
	else
	{
		m_bIsAggroData = false;
	}
}

void CBotAiAction_Look::OnExit()
{
}


int CBotAiAction_Look::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (!m_bIsAggroData)
	{
		m_status = COMPLETED;
		return m_status;
	}

	if (m_hTarget != GetBot()->GetTargetHandle())
	{
		m_status = COMPLETED;
		return m_status;
	}

	sBOT_AGGROPOINT* sAggro = GetBot()->GetTargetListManager()->GetAggroPoint(GetBot()->GetTargetHandle());
	if (!sAggro)
	{
		m_status = COMPLETED;
		return m_status;
	}

	if (m_sAggroData.hObject == sAggro->hObject && m_sAggroData.dwAggroPoint == sAggro->dwAggroPoint)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
		if (pTarget && pTarget->IsInitialized())
		{
			CNtlVector vLookDir(pTarget->GetCurLoc().operator-(GetBot()->GetCurLoc()));
			vLookDir.y = 0.0f;

			if (vLookDir.SafeNormalize() && m_vBackupLookDir.operator!=(vLookDir))
			{
				m_vBackupLookDir.operator=(vLookDir);
				GetBot()->SetCurDir(vLookDir);
			}

			m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
			if (m_dwWaitTime <= m_dwTime)
			{
				m_dwTime = 0;
				m_status = COMPLETED;
			}
		}
		else
		{
			m_status = COMPLETED;
		}
	}
	else
	{
		m_status = COMPLETED;
	}

	return m_status;
}
