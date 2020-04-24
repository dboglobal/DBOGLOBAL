#include "stdafx.h"
#include "GameServer.h"
#include "DojoManager.h"
#include "ObjectManager.h"
#include "NtlPacketGU.h"
#include "NtlPacketGT.h"
#include "Guild.h"
#include "TableContainerManager.h"
#include "WorldTable.h"
#include "TriggerObject.h"
#include "GameMain.h"
#include "CPlayer.h"
#include "DojoWar.h"


CDojo::CDojo(sDBO_DOJO_DATA & dojoData, sDBO_GUILD_MARK * pMark)
{
	m_brief.byLevel = dojoData.byLevel;
	m_brief.dojoTblidx = dojoData.dojoTblidx;
	m_brief.guildId = dojoData.guildId;
	memcpy(&m_brief.sMark, pMark, sizeof(sDBO_GUILD_MARK));

	m_matchStateInfo.eState = eDBO_DOJO_STATUS_NORMAL;

	memset(&m_warPoint, 0, sizeof(sDOJO_SCRAMBLE_POINT));
	m_warPoint.dojoTblidx = dojoData.dojoTblidx;

	m_dwNextUpdate = 0;
	m_matchStateInfo.tmNextStepTime = 0;
	m_attGuildId = INVALID_GUILDID;

	m_pWorld = NULL;
}

CDojo::~CDojo()
{
}

void CDojo::SetDojoMark(sDBO_GUILD_MARK * pMark)
{
	memcpy(&m_brief.sMark, pMark, sizeof(sDBO_GUILD_MARK));
}

void CDojo::SetDojoData(GUILDID guildId)
{
	m_brief.byLevel = 0;
	m_brief.guildId = guildId;
}

void CDojo::TickProcess(DWORD dwTickDiff)
{
	CGameServer * app = (CGameServer*)g_pApp;

	m_dwNextUpdate = UnsignedSafeDecrease<DWORD>(m_dwNextUpdate, dwTickDiff);
	if (m_dwNextUpdate > 0)
		return;

	time_t curTime = app->GetTime();

	if (m_matchStateInfo.eState == eDBO_DOJO_STATUS_START)
	{
		UpdatePoints();
	}

	if (m_matchStateInfo.tmNextStepTime > curTime)
		return;

	//printf("eState %u, tmNextStepTime %I64u, m_dwNextUpdate %u, curTime %I64u \n", m_matchStateInfo.eState, m_matchStateInfo.tmNextStepTime, m_dwNextUpdate, curTime);

	switch (m_matchStateInfo.eState)
	{
		case eDBO_DOJO_STATUS_NORMAL:
		{
			ERR_LOG(LOG_GENERAL, "eDBO_DOJO_STATUS_NORMAL");
			m_matchStateInfo.eState = eDBO_DOJO_STATUS_RECEIVE;

			m_dwNextUpdate = 1000 * 3600 * 2; // 2 hours time to receive war request
			m_matchStateInfo.tmNextStepTime = curTime + 7200; // 7200 sekunden = 2 stunden
			
			CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
			sGT_DOJO_SCRAMBLE_STATE_CHANGE * res = (sGT_DOJO_SCRAMBLE_STATE_CHANGE *)packet.GetPacketData();
			res->wOpCode = GT_DOJO_SCRAMBLE_STATE_CHANGE;
			res->byState = m_matchStateInfo.eState;
			res->dojoTblidx = m_brief.dojoTblidx;
			res->tmNextStepTime = m_matchStateInfo.tmNextStepTime;
			packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
			app->SendTo(app->GetChatServerSession(), &packet);
		}
		break;
		case eDBO_DOJO_STATUS_RECEIVE:
		{
			ERR_LOG(LOG_GENERAL, "eDBO_DOJO_STATUS_RECEIVE");

			//this is only called if no dojo war has been accepted or requested (2h time run out)

			m_matchStateInfo.eState = eDBO_DOJO_STATUS_NORMAL;

			EndWar();

			CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_RESET));
			sGT_DOJO_SCRAMBLE_RESET * res = (sGT_DOJO_SCRAMBLE_RESET *)packet.GetPacketData();
			res->wOpCode = GT_DOJO_SCRAMBLE_RESET;
			res->byState = eDBO_DOJO_STATUS_NORMAL;
			res->dojoTblidx = m_brief.dojoTblidx;
			res->tmNextStepTime = 0;
			packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_RESET));
			app->SendTo(app->GetChatServerSession(), &packet);
		}
		break;
		case eDBO_DOJO_STATUS_REJECT_END:
		{
			ERR_LOG(LOG_GENERAL, "eDBO_DOJO_STATUS_REJECT_END");
		}
		break;
		case eDBO_DOJO_STATUS_STANDBY: //prepare war 30 minutes
		{
			ERR_LOG(LOG_GENERAL, "eDBO_DOJO_STATUS_STANDBY");
			m_matchStateInfo.eState = eDBO_DOJO_STATUS_READY;

			m_matchStateInfo.tmNextStepTime = curTime + 300; // 300 sekunden = 5 minuten

			//this is called when preparation timer end
			if (GetAttGuildID() != INVALID_GUILDID)
			{
				sDOJO_TBLDAT* pDojoTbl = (sDOJO_TBLDAT*)g_pTableContainer->GetDojoTable()->FindData(GetDojoTblidx());
				sWORLD_TBLDAT * pTWorld = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(pDojoTbl->zoneTblidx);

				m_pWorld = app->GetGameMain()->GetWorldManager()->FindWorld(pTWorld->tblidx);

				//teleport all players out which are inside
				TeleportAllOut();

				//spawn seals
				SpawnSeals();

				CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
				sGT_DOJO_SCRAMBLE_STATE_CHANGE * res = (sGT_DOJO_SCRAMBLE_STATE_CHANGE *)packet.GetPacketData();
				res->wOpCode = GT_DOJO_SCRAMBLE_STATE_CHANGE;
				res->byState = m_matchStateInfo.eState;
				res->dojoTblidx = m_brief.dojoTblidx;
				res->tmNextStepTime = m_matchStateInfo.tmNextStepTime;
				packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
				app->SendTo(app->GetChatServerSession(), &packet);

				CNtlVector vDst(pTWorld->vStart1Loc);
				vDst.x = -64.f;
				vDst.y = 84.100f;
				vDst.z = -43.7f;

				CNtlPacket packet2(sizeof(sGT_DOJO_SEND_TELEPORT_PROPOSAL));
				sGT_DOJO_SEND_TELEPORT_PROPOSAL * res2 = (sGT_DOJO_SEND_TELEPORT_PROPOSAL *)packet2.GetPacketData();
				res2->wOpCode = GT_DOJO_SEND_TELEPORT_PROPOSAL;
				res2->attGuildId = m_attGuildId;
				res2->byTeleportType = TELEPORT_TYPE_DOJO;
				res2->defGuildId = GetGuildID();
				res2->destWorldId = pTWorld->tblidx;
				res2->destWorldTblidx = pTWorld->tblidx;
				res2->vDestAttLoc = vDst;
				res2->vDestDefLoc = pTWorld->vStart2Loc;
				res2->wWaitTime = 3600;
				packet2.SetPacketLen(sizeof(sGT_DOJO_SEND_TELEPORT_PROPOSAL));
				app->SendTo(app->GetChatServerSession(), &packet2);
			}
		}
		break;
		case eDBO_DOJO_STATUS_INITIAL:
		{
			ERR_LOG(LOG_GENERAL, "eDBO_DOJO_STATUS_INITIAL");
		}
		break;
		case eDBO_DOJO_STATUS_READY: //start battle
		{
			ERR_LOG(LOG_GENERAL, "eDBO_DOJO_STATUS_READY");
			m_matchStateInfo.eState = eDBO_DOJO_STATUS_START;
			m_matchStateInfo.tmNextStepTime = curTime + 3600; // 3600 sekunden = 60 minuten

			CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
			sGT_DOJO_SCRAMBLE_STATE_CHANGE * res = (sGT_DOJO_SCRAMBLE_STATE_CHANGE *)packet.GetPacketData();
			res->wOpCode = GT_DOJO_SCRAMBLE_STATE_CHANGE;
			res->byState = m_matchStateInfo.eState;
			res->dojoTblidx = m_brief.dojoTblidx;
			res->tmNextStepTime = m_matchStateInfo.tmNextStepTime;
			packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
			app->SendTo(app->GetChatServerSession(), &packet);
		}
		break;
		case eDBO_DOJO_STATUS_START:
		{
			ERR_LOG(LOG_GENERAL, "eDBO_DOJO_STATUS_START");
			m_matchStateInfo.eState = eDBO_DOJO_STATUS_END;
			m_matchStateInfo.tmNextStepTime = curTime + 60;

			CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
			sGT_DOJO_SCRAMBLE_STATE_CHANGE * res = (sGT_DOJO_SCRAMBLE_STATE_CHANGE *)packet.GetPacketData();
			res->wOpCode = GT_DOJO_SCRAMBLE_STATE_CHANGE;
			res->byState = m_matchStateInfo.eState;
			res->dojoTblidx = m_brief.dojoTblidx;
			res->tmNextStepTime = m_matchStateInfo.tmNextStepTime;
			packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
			app->SendTo(app->GetChatServerSession(), &packet);

			//check score and decide winner. If attacker has more points than defender, than attacker receive dojo
			DecideWinner(true);
		}
		break;
		case eDBO_DOJO_STATUS_END:
		{
			ERR_LOG(LOG_GENERAL, "eDBO_DOJO_STATUS_END");
			m_dwNextUpdate = INVALID_DWORD;

			m_matchStateInfo.eState = eDBO_DOJO_STATUS_NORMAL;

			CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
			sGT_DOJO_SCRAMBLE_STATE_CHANGE * res = (sGT_DOJO_SCRAMBLE_STATE_CHANGE *)packet.GetPacketData();
			res->wOpCode = GT_DOJO_SCRAMBLE_STATE_CHANGE;
			res->byState = m_matchStateInfo.eState;
			res->dojoTblidx = m_brief.dojoTblidx;
			res->tmNextStepTime = 0;
			packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
			app->SendTo(app->GetChatServerSession(), &packet);

			//teleport all out
			TeleportAllOut();
		}
		break;
	}
}


void CDojo::SetState(BYTE byState, DOJOTIME tmNextStepTime)
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_matchStateInfo.eState = (eDBO_DOJO_STATUS)byState;
	m_matchStateInfo.tmNextStepTime = tmNextStepTime;
	m_dwNextUpdate = 0; //set to 0 to avoid lock

	if (m_matchStateInfo.eState == eDBO_DOJO_STATUS_NORMAL)
		EndWar();

	else if (app->IsDojoChannel() && m_matchStateInfo.eState == eDBO_DOJO_STATUS_START) //only do the following when its dojo server
	{
		//send info
		SendWarPointNfy();
	}
}

void CDojo::SetUserCount(BYTE byDefCount, BYTE byAttCount)
{
	m_warPoint.byDefCount = byDefCount;
	m_warPoint.byAttCount = byAttCount;
}

bool CDojo::CanEnterDojo(GUILDID guildId)
{
	//printf("CanEnterDojo: DefCount %u, AttCount %u \n", m_warPoint.byDefCount, m_warPoint.byAttCount);
	if (GetGuildID() == guildId)
	{
		if (m_warPoint.byDefCount < DBO_DOJO_SCRAMBLE_MAX_MEMBER)
		{
			++m_warPoint.byDefCount; //increase here just for a little bit security
			return true;
		}
	}
	else if(GetAttGuildID() == guildId)
	{
		if (m_warPoint.byAttCount < DBO_DOJO_SCRAMBLE_MAX_MEMBER)
		{
			++m_warPoint.byAttCount; //increase here just for a little bit security
			return true;
		}
	}

	return false;
}

bool CDojo::UpdateSeal(WCHAR* wchName, GUILDID guildId, TBLIDX sealIdx, BYTE & rbyMainState)
{
	bool bUpdate = false;

	if (rbyMainState == TOBJECT_STATE_TYPE_C_NO_TEAM)
	{
		bUpdate = true;
	}
	else if (rbyMainState == TOBJECT_STATE_TYPE_C_ATTACK_TEAM)
	{
		if (m_brief.guildId == guildId)
			bUpdate = true;
	}
	else if (rbyMainState == TOBJECT_STATE_TYPE_C_DEFENCE_TEAM)
	{
		if (m_attGuildId == guildId)
			bUpdate = true;
	}

	if (bUpdate)
	{
		CGameServer	* app = (CGameServer*)g_pApp;

		if (m_brief.guildId == guildId)
		{
			if(rbyMainState == TOBJECT_STATE_TYPE_C_ATTACK_TEAM)
				m_warPoint.byAttSealCount -= 1;

			rbyMainState = TOBJECT_STATE_TYPE_C_DEFENCE_TEAM;

			m_warPoint.byDefSealCount += 1;
		}
		else
		{
			if (rbyMainState == TOBJECT_STATE_TYPE_C_DEFENCE_TEAM)
				m_warPoint.byDefSealCount -= 1;

			rbyMainState = TOBJECT_STATE_TYPE_C_ATTACK_TEAM;

			m_warPoint.byAttSealCount += 1;
		}

		CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY));
		sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY * res = (sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY *)packet.GetPacketData();
		res->wOpCode = GT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY;
		res->dojoTblidx = m_brief.dojoTblidx;
		res->guildId = guildId;
		res->objectTblidx = sealIdx;
		packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY));
		app->SendTo(app->GetChatServerSession(), &packet);

		CNtlPacket packet2(sizeof(sGU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY));
		sGU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY * res2 = (sGU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY;
		res2->dojoTblidx = GetDojoTblidx();
		res2->objectTblidx = sealIdx;
		res2->guildId = guildId;
		NTL_SAFE_WCSCPY(res2->wszName, wchName);
		packet2.SetPacketLen(sizeof(sGU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY));
		m_pWorld->Broadcast(&packet2);

		SendWarPointNfy();
	}

	return bUpdate;
}

void CDojo::SendSealState(HOBJECT hSession)
{
	for (std::vector<CTriggerObject*>::iterator it = m_setSeals.begin(); it != m_setSeals.end(); it++)
	{
		CTriggerObject* pObj = *it;
		if (pObj)
		{
			CNtlPacket packet(sizeof(sGU_DOJO_SEAL_CURRENT_STATE_NFY));
			sGU_DOJO_SEAL_CURRENT_STATE_NFY * res = (sGU_DOJO_SEAL_CURRENT_STATE_NFY *)packet.GetPacketData();
			res->wOpCode = GU_DOJO_SEAL_CURRENT_STATE_NFY;
			res->sealObjectTblidx = pObj->GetTblidx();
			res->byState = pObj->GetMainState();
			pObj->GetCurLoc().CopyTo(res->vLoc);
			packet.SetPacketLen(sizeof(sGU_DOJO_SEAL_CURRENT_STATE_NFY));
			g_pApp->Send(hSession, &packet);
		}
	}
}

void CDojo::SendSealUnderAttack(HOBJECT hSeal)
{
	for (std::vector<CTriggerObject*>::iterator it = m_setSeals.begin(); it != m_setSeals.end(); it++)
	{
		CTriggerObject* pObj = *it;
		if (pObj && pObj->GetID() == hSeal)
		{
			if (pObj->GetMainState() == TOBJECT_STATE_TYPE_C_NO_TEAM)
				return;

			CNtlPacket packet(sizeof(sGU_DOJO_SEAL_ATTACK_BEGIN_NFY));
			sGU_DOJO_SEAL_ATTACK_BEGIN_NFY * res = (sGU_DOJO_SEAL_ATTACK_BEGIN_NFY *)packet.GetPacketData();
			res->wOpCode = GU_DOJO_SEAL_ATTACK_BEGIN_NFY;
			res->sealObjectTblidx = pObj->GetTblidx();
			res->bIsBeingStolen = (pObj->GetMainState() == TOBJECT_STATE_TYPE_C_NO_TEAM) ? false : true;
			pObj->GetCurLoc().CopyTo(res->vLoc);
			packet.SetPacketLen(sizeof(sGU_DOJO_SEAL_ATTACK_BEGIN_NFY));
			m_pWorld->Broadcast(&packet);

			return;
		}
	}
}

void CDojo::SendStopSealUnderAttack(HOBJECT hSeal)
{
	for (std::vector<CTriggerObject*>::iterator it = m_setSeals.begin(); it != m_setSeals.end(); it++)
	{
		CTriggerObject* pObj = *it;
		if (pObj && pObj->GetID() == hSeal)
		{
			if (pObj->GetMainState() == TOBJECT_STATE_TYPE_C_NO_TEAM)
				return;

			CNtlPacket packet(sizeof(sGU_DOJO_SEAL_ATTACK_END_NFY));
			sGU_DOJO_SEAL_ATTACK_END_NFY * res = (sGU_DOJO_SEAL_ATTACK_END_NFY *)packet.GetPacketData();
			res->wOpCode = GU_DOJO_SEAL_ATTACK_END_NFY;
			res->sealObjectTblidx = pObj->GetTblidx();
			packet.SetPacketLen(sizeof(sGU_DOJO_SEAL_ATTACK_END_NFY));
			m_pWorld->Broadcast(&packet);

			return;
		}
	}
}

void CDojo::SpawnSeals()
{
	CGameServer	* app = (CGameServer*)g_pApp;

	CObjectTable* pObj = g_pTableContainer->GetObjectTable(m_pWorld->GetIdx());
	for (CTable::TABLEIT itObjSpawn = pObj->Begin(); itObjSpawn != pObj->End(); itObjSpawn++)
	{
		sOBJECT_TBLDAT* pObjTblData = (sOBJECT_TBLDAT*)itObjSpawn->second;

		CTriggerObject* pObj = m_pWorld->FindStaticObject(MakeTriggerObjectHandle(m_pWorld->GetTiggerObjectOffset(), pObjTblData->dwSequence));
		if (pObj && pObj->GetTblidx() >= 5 && pObj->GetTblidx() <= 9)
		{
			//main state = decides who own the seal. eTOBJECT_STATE_TYPE_C. 0 = none, 1 = attacker, 2 = defender

			pObj->UpdateToObjectState(TOBJECT_STATE_TYPE_C_NO_TEAM, TOBJECT_SUBSTATE_FLAG_SHOW, GetTickCount());

			CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY));
			sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY * res = (sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY *)packet.GetPacketData();
			res->wOpCode = GT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY;
			res->dojoTblidx = m_brief.dojoTblidx;
			res->guildId = INVALID_GUILDID;
			res->objectTblidx = pObjTblData->tblidx;
			packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY));
			app->SendTo(app->GetChatServerSession(), &packet);

			m_setSeals.push_back(pObj);
		}
	}
}

void CDojo::DespawnSeals()
{
	for (std::vector<CTriggerObject*>::iterator it = m_setSeals.begin(); it != m_setSeals.end(); it++)
	{
		CTriggerObject* pObj = *it;
		if (pObj)
		{
			pObj->UpdateToObjectState(TOBJECT_STATE_TYPE_C_NO_TEAM, 0, GetTickCount());
		}
	}

	m_setSeals.clear();
}

void CDojo::SendWarPointNfy()
{
	CGameServer	* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_POINT_NFY));
	sGT_DOJO_SCRAMBLE_POINT_NFY * res = (sGT_DOJO_SCRAMBLE_POINT_NFY *)packet.GetPacketData();
	res->wOpCode = GT_DOJO_SCRAMBLE_POINT_NFY;
	res->sData = m_warPoint;
	packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_POINT_NFY));
	app->SendTo(app->GetChatServerSession(), &packet);
}

void CDojo::UpdatePoints()
{
	CGameServer* app = (CGameServer*)g_pApp;
	m_dwNextUpdate = 5000; //update in 5 seconds again

	m_warPoint.dwDefCharge += m_warPoint.byDefSealCount * 10;
	if (m_warPoint.dwDefCharge >= 300)
	{
		m_warPoint.dwDefPoint += 8;

		m_warPoint.dwDefCharge = m_warPoint.dwDefCharge - 300;
	}

	m_warPoint.dwAttCharge += m_warPoint.byAttSealCount * 10;
	if (m_warPoint.dwAttCharge >= 300)
	{
		m_warPoint.dwAttPoint += 8;

		m_warPoint.dwAttCharge = m_warPoint.dwAttCharge - 300;
	}

	//send point nfy to chat server
	SendWarPointNfy();

	//check if someone won
	if (m_warPoint.dwDefPoint >= 500 || m_warPoint.dwAttPoint >= 500) //should be 500
	{
		CGameServer	* app = (CGameServer*)g_pApp;

		m_matchStateInfo.eState = eDBO_DOJO_STATUS_END;
		m_matchStateInfo.tmNextStepTime = app->GetTime() + 180; // 180 sekunden = 3 minutes

		CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
		sGT_DOJO_SCRAMBLE_STATE_CHANGE * res = (sGT_DOJO_SCRAMBLE_STATE_CHANGE *)packet.GetPacketData();
		res->wOpCode = GT_DOJO_SCRAMBLE_STATE_CHANGE;
		res->byState = m_matchStateInfo.eState;
		res->dojoTblidx = m_brief.dojoTblidx;
		res->tmNextStepTime = m_matchStateInfo.tmNextStepTime;
		packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_STATE_CHANGE));
		app->SendTo(app->GetChatServerSession(), &packet);

		DecideWinner(false);
	}
}

void CDojo::DecideWinner(bool bTime)
{
	//check score and decide winner. If attacker has more points than defender, than attacker receive dojo
	CGameServer	* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGT_DOJO_SCRAMBLE_REWARD_REQ));
	sGT_DOJO_SCRAMBLE_REWARD_REQ * res = (sGT_DOJO_SCRAMBLE_REWARD_REQ *)packet.GetPacketData();
	res->wOpCode = GT_DOJO_SCRAMBLE_REWARD_REQ;
	res->dojoTblidx = m_brief.dojoTblidx;
	res->dwAddGuildReputation = 10000;
	res->guildId = m_brief.guildId;

	if (m_warPoint.dwAttPoint > m_warPoint.dwDefPoint)
		res->guildId = GetAttGuildID();

	res->sItem.Init();

	packet.SetPacketLen(sizeof(sGT_DOJO_SCRAMBLE_REWARD_REQ));
	app->SendTo(app->GetChatServerSession(), &packet);


	// despawn seals
	DespawnSeals();
}

void CDojo::TeleportAllOut()
{
	sWORLD_TBLDAT * pTWorld = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pWorld->GetIdx());

	CNtlVector vDst(pTWorld->outWorldLoc);

	int n = 0;
	int nObjCount = m_pWorld->GetObjectList()->GetObjCount(OBJTYPE_PC);
	CPlayer* pExistObject = (CPlayer*)m_pWorld->GetObjectList()->GetFirst(OBJTYPE_PC);

	for (int i = 0; i < nObjCount; i++) //use for instead of "while" to avoid endless loop ;)
	{
		if (pExistObject)
		{
			vDst.x = vDst.x + (rand() % 10);

			pExistObject->TeleportAnotherServer(vDst, pTWorld->outWorldDir, pTWorld->outWorldTblidx, pTWorld->outWorldTblidx, TELEPORT_TYPE_DOJO, pExistObject->GetPrevChannelID(), 0, false);
		}

		if (++n >= MAX_PC)
		{
			ERR_LOG(LOG_GENERAL, "ERROR !! ENDLESS LOOP.");
			break;
		}

		pExistObject = (CPlayer*)m_pWorld->GetObjectList()->GetNext(pExistObject->GetWorldObjectLinker());
	}
}

void CDojo::EndWar()
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_dwNextUpdate = 0;
	m_matchStateInfo.tmNextStepTime = app->GetTime() + (3600 * 24); //block dojo war for next 24h
	memset(&m_warPoint, NULL, sizeof(m_warPoint));
	m_warPoint.dojoTblidx = GetDojoTblidx();
	m_attGuildId = INVALID_GUILDID;

	g_pDojoWarManager->RemoveDojo(GetDojoTblidx());
}



CDojoManager::CDojoManager()
{
	Init();
}

CDojoManager::~CDojoManager()
{
}

void CDojoManager::Init()
{
	m_dwNextUpdateTick = 0;
}

void	CDojoManager::UpdateDojoMark(GUILDID guildId, sDBO_GUILD_MARK* mark)
{
	TMAP_DOJO::iterator it = m_map_Dojo.find(guildId);
	if (it != m_map_Dojo.end())
	{
		CDojo* pDojo = it->second;
		pDojo->SetDojoMark(mark);

		CNtlPacket packet(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
		sGU_DOJO_MARK_CHANGED_NFY * res = (sGU_DOJO_MARK_CHANGED_NFY *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_MARK_CHANGED_NFY;
		res->dojoTblidx = pDojo->GetDojoTblidx();
		res->guildId = guildId;
		memcpy(&res->sMark, mark, sizeof(sDBO_GUILD_MARK));
		packet.SetPacketLen(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
		g_pObjectManager->SendPacketToAll(&packet);
	}
}

CDojo* CDojoManager::GetDojo(GUILDID guildId)
{
	TMAP_DOJO::iterator it = m_map_Dojo.find(guildId);
	if (it != m_map_Dojo.end())
	{
		return it->second;
	}

	return NULL;
}

CDojo * CDojoManager::GetDojoWithTblidx(TBLIDX dojoTblidx)
{
	for (std::map<GUILDID, CDojo*>::const_iterator it = GetDojoSetBegin(); it != GetDojoSetEnd(); it++)
	{
		CDojo* pDojo = it->second;
		if (pDojo && pDojo->GetDojoTblidx() == dojoTblidx)
			return pDojo;
	}

	return nullptr;
}

sDBO_DOJO_BRIEF* CDojoManager::GetDojoBrief(GUILDID guildId)
{
	TMAP_DOJO::iterator it = m_map_Dojo.find(guildId);
	if (it != m_map_Dojo.end())
	{
		return &it->second->GetDojoBrief();
	}

	return NULL;
}

void	CDojoManager::AddDojo(sDBO_DOJO_DATA& dojoData, sDBO_GUILD_MARK* pMark)
{
	if (m_map_Dojo.find(dojoData.guildId) == m_map_Dojo.end())
	{
		CDojo* pDojo = new CDojo(dojoData, pMark);

		m_map_Dojo.insert(std::make_pair(dojoData.guildId, pDojo));

		CNtlPacket packet(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
		sGU_DOJO_MARK_CHANGED_NFY * res = (sGU_DOJO_MARK_CHANGED_NFY *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_MARK_CHANGED_NFY;
		res->dojoTblidx = dojoData.dojoTblidx;
		res->guildId = dojoData.guildId;
		memcpy(&res->sMark, pMark, sizeof(sDBO_GUILD_MARK));
		packet.SetPacketLen(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
		g_pObjectManager->SendPacketToAll(&packet);
	}
}

void	CDojoManager::DelDojo(GUILDID guildid)
{
	/*
		TODO: Is it required to send sGU_DOJO_MARK_CHANGED_NFY ?
	*/

	TMAP_DOJO::iterator it = m_map_Dojo.find(guildid);
	if (it != m_map_Dojo.end())
	{
		CDojo* pDojo = it->second;

		CNtlPacket packet(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
		sGU_DOJO_MARK_CHANGED_NFY * res = (sGU_DOJO_MARK_CHANGED_NFY *)packet.GetPacketData();
		res->wOpCode = GU_DOJO_MARK_CHANGED_NFY;
		res->dojoTblidx = pDojo->GetDojoTblidx();
		res->guildId = INVALID_GUILDID;
		memset(&res->sMark, INVALID_BYTE, sizeof(sDBO_GUILD_MARK));
		packet.SetPacketLen(sizeof(sGU_DOJO_MARK_CHANGED_NFY));
		g_pObjectManager->SendPacketToAll(&packet);

		m_map_Dojo.erase(it);
		delete pDojo;
	}
}

void CDojoManager::FillDojoData(sDBO_DOJO_BRIEF * pBrief, BYTE & rCount)
{
	for (std::map<GUILDID, CDojo*>::const_iterator it = GetDojoSetBegin(); it != GetDojoSetEnd(); it++)
	{
		CDojo* pDojo = it->second;
		if (pDojo)
			memcpy(&pBrief[rCount++], &pDojo->GetDojoBrief(), sizeof(sDBO_DOJO_BRIEF));
	}
}

void CDojoManager::RemoveDojo(GUILDID guildId)
{
	m_map_Dojo.erase(guildId);
}

void CDojoManager::AddDojo(GUILDID guildId, CDojo * pDojo)
{
	m_map_Dojo.insert(std::make_pair(guildId, pDojo));
}


void CDojoManager::TickProcess(DWORD dwTickDiff)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (dwTickDiff < m_dwNextUpdateTick)
		return;

	DBOTIME curTime = app->GetTime();

	tm timeStruct = {};
	localtime_s(&timeStruct, &curTime);

	if (timeStruct.tm_wday == 0 && (timeStruct.tm_hour >= 16 && timeStruct.tm_hour <= 20)) //check if its sunday
	{
		for (std::map<GUILDID, CDojo*>::const_iterator it = GetDojoSetBegin(); it != GetDojoSetEnd(); it++)
		{
			CDojo* pDojo = it->second;
			if (pDojo)
				pDojo->TickProcess(dwTickDiff);
		}
	}
}

