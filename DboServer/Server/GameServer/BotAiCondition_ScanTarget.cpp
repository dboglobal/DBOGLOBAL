#include "stdafx.h"
#include "BotAiCondition_ScanTarget.h"
#include "ObjectManager.h"
#include "char.h"
#include "NtlPacketGU.h"


CBotAiCondition_ScanTarget::CBotAiCondition_ScanTarget(CNpc* pBot, CBotAiCondition_ScanTarget::eBOTAI_SMARTSCAN_TYPE eSmartScantype, BYTE bySmartLevel)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_SCAN_TARGET, "BOTCONTROL_CONDITION_SCAN_TARGET")
{
	m_dwTime = 0;
	m_bySmartScantype = (BYTE)eSmartScantype;
	m_bySmartLevel = bySmartLevel;
}

CBotAiCondition_ScanTarget::~CBotAiCondition_ScanTarget()
{
}


void CBotAiCondition_ScanTarget::OnEnter()
{
	this->m_dwTime = 1000;
}

int CBotAiCondition_ScanTarget::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);

	if (m_dwTime >= 1000)
	{
		m_dwTime = 0;

		if (ScanTarget(m_bySmartScantype == BOTAI_SMARTSCAN_UNDER))
			return CHANGED;
	}

	return m_status;
}


bool CBotAiCondition_ScanTarget::ScanTarget(bool bSmartOffence)
{
	HOBJECT hTarget = GetBot()->ConsiderScanTarget(INVALID_WORD);
	if (hTarget == INVALID_HOBJECT)
		return false;

	CCharacter* pTargetBot = g_pObjectManager->GetChar(hTarget);
	if (!pTargetBot)
		return false;

	if (bSmartOffence)
	{
		if (GetBot()->GetLevel() - m_bySmartLevel < pTargetBot->GetLevel())
			return false;
	}

	if (GetBot()->GetTargetListManager()->GetAggroCount() == 0)
	{
		pTargetBot->ResetMeAttackBot();
		
		CNtlPacket packet(sizeof(sGU_BOT_BOTCAUTION_NFY));
		sGU_BOT_BOTCAUTION_NFY* res = (sGU_BOT_BOTCAUTION_NFY *)packet.GetPacketData();
		res->wOpCode = GU_BOT_BOTCAUTION_NFY;
		res->hBot = GetBot()->GetID();
		packet.SetPacketLen(sizeof(sGU_BOT_BOTCAUTION_NFY));
		GetBot()->Broadcast(&packet);
	}

	DWORD dwAggroPoint = GetBot()->GetTbldat()->wBasic_Aggro_Point + 1;

	GetBot()->GetTargetListManager()->AddAggro(hTarget, dwAggroPoint, false);
	GetBot()->GetBotController()->AddControlState_Fight(hTarget);

	return true;
}