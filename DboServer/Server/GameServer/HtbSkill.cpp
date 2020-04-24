#include "stdafx.h"
#include "GameServer.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "CPlayer.h" //include htbskillmanager.h -> htbskill.h
#include "TableContainerManager.h"


CHtbSkill::CHtbSkill()
{
	Init();
}


CHtbSkill::~CHtbSkill()
{
}



bool CHtbSkill::Create(sHTB_SET_TBLDAT *pHtbDataRef, CPlayer *pOwnerRef, BYTE skillIndex, DWORD dwTimeRemaining)
{
	m_byHtbSlot = skillIndex;
	m_dwTimeRemaining = dwTimeRemaining;
	m_pHtbDataRef = pHtbDataRef;
	m_pPlayerRef = pOwnerRef;

	return true;
}


void CHtbSkill::Destroy()
{
}


void CHtbSkill::Init()
{
	m_byHtbSlot = INVALID_BYTE;
	m_dwTimeRemaining = INVALID_DWORD;
	m_pHtbDataRef = NULL;
	m_pPlayerRef = NULL;
	m_bAtkFailed = false;
}


void CHtbSkill::TickProcess(DWORD dwTickDiff)
{
	m_dwTimeRemaining = UnsignedSafeDecrease<DWORD>(m_dwTimeRemaining, dwTickDiff);
}


void CHtbSkill::OnLearnHtbSkill()
{
	CNtlPacket packet(sizeof(sGU_HTB_LEARN_RES));
	sGU_HTB_LEARN_RES * res = (sGU_HTB_LEARN_RES *)packet.GetPacketData();
	res->wOpCode = GU_HTB_LEARN_RES;
	res->skillId = GetHtbID();
	res->bySkillSlot = GetSkillIndex();
	res->bySkillFrom = 0;
	res->wResultCode = 500;
	packet.SetPacketLen(sizeof(sGU_HTB_LEARN_RES));
	m_pPlayerRef->SendPacket(&packet);

	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket pQry(sizeof(sGQ_HTB_SKILL_ADD_REQ));
	sGQ_HTB_SKILL_ADD_REQ* rQry = (sGQ_HTB_SKILL_ADD_REQ*)pQry.GetPacketData();
	rQry->wOpCode = GQ_HTB_SKILL_ADD_REQ;
	rQry->charId = m_pPlayerRef->GetCharID();
	rQry->skillIndex = GetSkillIndex();
	rQry->skillId = GetHtbID();
	pQry.SetPacketLen(sizeof(sGQ_HTB_SKILL_ADD_REQ));
	g_pApp->SendTo(app->GetQueryServerSession(), &pQry);
}


int CHtbSkill::CopyToInfo(sHTB_SKILL_INFO * pHtbInfo)
{
	if (pHtbInfo)
	{
		pHtbInfo->bySlotId = m_byHtbSlot;
		pHtbInfo->dwTimeRemaining = m_dwTimeRemaining;
		pHtbInfo->skillId = m_pHtbDataRef->tblidx;

		return sizeof(sHTB_SKILL_INFO);
	}

	return 0;

}


int CHtbSkill::CopyToData(sHTB_SKILL_DATA * pHtbData)
{
	if (pHtbData)
	{
		pHtbData->dwSkillTime = m_dwTimeRemaining;
		pHtbData->skillId = m_pHtbDataRef->tblidx;

		return sizeof(sHTB_SKILL_DATA);
	}

	return 0;
}




