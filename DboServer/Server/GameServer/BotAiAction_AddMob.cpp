#include "stdafx.h"
#include "BotAiAction_AddMob.h"
#include "AISNodeAction_AddMob.h"


CBotAiAction_AddMob::CBotAiAction_AddMob(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_ADDMOB, "BOTCONTROL_ACTION_ADDMOB")
{
	m_mobTblidx = INVALID_TBLIDX;
	m_vLoc.Reset();
	m_vDir.Reset();
	m_byPartyNumber = INVALID_PARTYID;
	m_eAddMobType = CAISNodeAction_AddMob::INVALID_ADDMOBTYPE;
}


CBotAiAction_AddMob::~CBotAiAction_AddMob()
{
}


bool CBotAiAction_AddMob::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CAISNodeAction_AddMob* pAction = dynamic_cast<CAISNodeAction_AddMob*>(pControlScriptNode);
	if (pAction)
	{
		m_mobTblidx = pAction->m_mobTblidx;
		m_vLoc = pAction->m_vLoc;
		m_vDir = pAction->m_vDir;
		m_eAddMobType = pAction->m_eAddMobType;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CAISNodeAction_AddMob", pControlScriptNode);
	return false;
}

void CBotAiAction_AddMob::CopyTo(CControlState* pTo)
{
	CBotAiAction_AddMob* pAction = dynamic_cast<CBotAiAction_AddMob*>(pTo);
	if (pAction)
	{
		memcpy(&pAction->m_sMobData, &m_sMobData, sizeof(sMOB_DATA));
		pAction->m_mobTblidx = m_mobTblidx;
		pAction->m_vLoc = m_vLoc;
		pAction->m_vDir = m_vDir;
		pAction->m_byPartyNumber = m_byPartyNumber;
		pAction->m_eAddMobType = m_eAddMobType;

		return;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CAISNodeAction_AddMob", pTo);
}

void CBotAiAction_AddMob::OnEnter()
{
	InitMobData(m_sMobData);
}

void CBotAiAction_AddMob::OnExit()
{
}


int CBotAiAction_AddMob::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_sMobData.tblidx = m_mobTblidx;
	m_sMobData.worldID = GetBot()->GetWorldID();
	m_sMobData.worldtblidx = GetBot()->GetWorldTblidx();

	if (m_eAddMobType == CAISNodeAction_AddMob::DIRECT)
	{
		m_vLoc.CopyTo(m_sMobData.vCurLoc);
		m_vDir.CopyTo(m_sMobData.vCurDir);
		m_vLoc.CopyTo(m_sMobData.vSpawnLoc);
		m_vDir.CopyTo(m_sMobData.vSpawnDir);

		//spawn

	//	m_status = COMPLETED;
	}
	else if (m_eAddMobType == CAISNodeAction_AddMob::RECALL)
	{
		GetBot()->GetCurLoc().CopyTo(m_sMobData.vCurLoc);
		GetBot()->GetCurDir().CopyTo(m_sMobData.vCurDir);
		GetBot()->GetCurLoc().CopyTo(m_sMobData.vSpawnLoc);
		GetBot()->GetCurDir().CopyTo(m_sMobData.vSpawnDir);

		//spawn

	//	m_status = COMPLETED;
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : AiScript(%d) AiScriptScene(%d) m_eAddMobType(%d)", GetBot()->GetAiScript(), GetBot()->GetAiScriptScene(), m_eAddMobType);
		m_status = FAILED;
	}

	return m_status;
}