#include "stdafx.h"
#include "BuffPc.h"
#include "CPlayer.h"
#include "GameServer.h"
#include "NtlPacketGQ.h"
#include "NtlPacketGU.h"

CBuffPc::CBuffPc()
{
	m_pPlayerRef = NULL;
}

CBuffPc::~CBuffPc()
{
	m_pPlayerRef = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffPc::Create(CPlayer *pOwnerRef, sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, eBUFF_TYPE buffType, BYTE byBuffGroup, BYTE* abyEffectType)
{
	if (CBuff::Create(pOwnerRef, pBuffInfo, effectCode, hCaster, buffType, byBuffGroup, abyEffectType))
	{
		m_pPlayerRef = pOwnerRef;
		return true;
	}

	return false;
}


bool CBuffPc::OnRegistered(bool bIsLoadedBuff, bool bNeedToDisplayMessage)
{
	bool bAddToDB = false;

	switch (m_aeEffectCode[0])
	{
		case ACTIVE_SUPER_SAIYAN:
		case ACTIVE_PURE_MAJIN:
		case ACTIVE_GREAT_NAMEK:	m_pPlayerRef->Transform(m_aeEffectCode[0], (TBLIDX)m_sBuffInfo.aBuffParameter[0].buffParameter.fParameter, m_sBuffInfo.bySourceType == DBO_OBJECT_SOURCE_SKILL); break;
		default: bAddToDB = true;  break;
	}

	if (!bIsLoadedBuff && bAddToDB)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		sBUFF_INFO* info = GetBuffInfo();

		CNtlPacket packet(sizeof(sGQ_BUFF_ADD_REQ));
		sGQ_BUFF_ADD_REQ* res = (sGQ_BUFF_ADD_REQ*)packet.GetPacketData();
		res->wOpCode = GQ_BUFF_ADD_REQ;
		res->charId = m_pPlayerRef->GetCharID();
		res->sBuff.sourceTblidx = GetSourceTblidx();
		res->sBuff.buffIndex = GetBuffIndex();
		res->sBuff.bySourceType = GetSourceType();
		res->sBuff.dwInitialDuration = info->dwInitialDuration;
		res->sBuff.dwTimeRemaining = info->dwTimeRemaining;
		res->sBuff.anArgument1[0] = info->aBuffParameter[0].buffParameter.commonConfigTblidx;
		res->sBuff.anArgument2[0] = info->aBuffParameter[1].buffParameter.commonConfigTblidx;
		res->sBuff.anArgument1[1] = m_dwRemainTime[0];
		res->sBuff.anArgument2[1] = m_dwRemainTime[1];
		res->sBuff.anArgument1[2] = info->aBuffParameter[0].buffParameter.dwRemainValue;
		res->sBuff.anArgument2[2] = info->aBuffParameter[1].buffParameter.dwRemainValue;
		res->sBuff.effectValue1 = info->aBuffParameter[0].buffParameter.fParameter;
		res->sBuff.effectValue1 = info->aBuffParameter[1].buffParameter.fParameter;
		res->sBuff.byBuffGroup = GetBuffGroup();
		res->sBuff.bIsDBUpdateFiltering = false;
		packet.SetPacketLen(sizeof(sGQ_BUFF_ADD_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}

	return CBuff::OnRegistered(bIsLoadedBuff, bNeedToDisplayMessage);
}


bool CBuffPc::OnRemoved(CBuff::BUFF_REMOVAL_REASON nReason)
{
	bool bDelFromDB = false;

	switch (m_aeEffectCode[0])
	{
		case ACTIVE_SUPER_SAIYAN:
		case ACTIVE_PURE_MAJIN:
		case ACTIVE_GREAT_NAMEK:	m_pPlayerRef->EndTransformation(); break;
		default: bDelFromDB = true; break;
	}

	if (bDelFromDB)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		CNtlPacket packet(sizeof(sGQ_BUFF_DEL_REQ));
		sGQ_BUFF_DEL_REQ* res = (sGQ_BUFF_DEL_REQ*)packet.GetPacketData();
		res->wOpCode = GQ_BUFF_DEL_REQ;
		res->charId = m_pPlayerRef->GetCharID();
		res->buffIndex = GetBuffIndex();
		packet.SetPacketLen(sizeof(sGQ_BUFF_DEL_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}

	if (m_pPlayerRef->GetParty())
	{
		CNtlPacket packet(sizeof(sGU_BUFF_DROPPED));
		sGU_BUFF_DROPPED * res = (sGU_BUFF_DROPPED*)packet.GetPacketData();
		res->wOpCode = GU_BUFF_DROPPED;
		res->hHandle = m_pOwnerRef->GetID();
		res->buffIndex = GetBuffIndex();
		res->bySourceType = GetSourceType();

		if (nReason == BUFF_REMOVAL_REASON_REPLACED)
			res->bIsByReplacing = true;
		else
			res->bIsByReplacing = false;

		packet.SetPacketLen(sizeof(sGU_BUFF_DROPPED));
		m_pPlayerRef->GetParty()->SendMsgToPartyExceptOne(&packet, m_pPlayerRef->GetID());
	}
	

	return CBuff::OnRemoved(nReason);
}


