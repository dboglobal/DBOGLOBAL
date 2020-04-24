#include "stdafx.h"
#include "WpsScriptAlgoAction_CCBD_stage.h"
#include "WpsNodeAction_CCBD_stage.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "TableContainerManager.h"
#include "ServerConfigTable.h"
#include "NtlCCBD.h"


CWpsScriptAlgoAction_CCBD_stage::CWpsScriptAlgoAction_CCBD_stage(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CCBD_STAGE, "SCRIPTCONTROL_ACTION_CCBD_STAGE")
{
	m_byStage = INVALID_BYTE;
	m_bDirectPlay = true;
	m_dwFailTimer = CCBD_FAIL_TIMER_IN_MS;
	m_bIsEveryoneReady = false;
}


CWpsScriptAlgoAction_CCBD_stage::~CWpsScriptAlgoAction_CCBD_stage()
{
}


bool CWpsScriptAlgoAction_CCBD_stage::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWpsNodeAction_CCBD_stage* pAction = dynamic_cast<CWpsNodeAction_CCBD_stage*>(pControlScriptNode);
	if (pAction)
	{
		m_byStage = pAction->m_byStage;
		m_bDirectPlay = pAction->m_bDirectPlay;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWpsNodeAction_CCBD_stage", pControlScriptNode);
	return false;
}

void CWpsScriptAlgoAction_CCBD_stage::OnEnter()
{
//	NTL_PRINT(PRINT_APP, "enter stage %u \n", m_byStage);

	GetOwner()->SetCCBDStage(m_byStage);

	if (m_bDirectPlay == false) //if boss stage
	{
		TeleportToBoss();
	}
	else //if not boss stage
	{
		if (m_byStage > 1) //if not first stage then spawn at base loc
		{
			if(IsCCBDBossStage(m_byStage - 1) == false) //if previous stage was boss then do not spawn because we use teleport at OnExit
				SpawnNextStage();
		}
	}
}

void CWpsScriptAlgoAction_CCBD_stage::OnExit()
{
//	NTL_PRINT(PRINT_APP, "exit stage %u \n", m_byStage);

	//leave boss room
	if (m_bDirectPlay == false)
	{
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer)
		{
			if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID())
			{
				if (pPlayer->GetParty())
				{
					pPlayer->GetParty()->DecidePartySelect();
					break;
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "User is registered for ccbd but has different world id (%u != %u) !!!", pPlayer->GetWorldID(), GetOwner()->GetWorld()->GetID());
			}

			pPlayer = GetOwner()->GetPlayersNext();
		}
	}
}

int CWpsScriptAlgoAction_CCBD_stage::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_bIsEveryoneReady == false && IsCCBDBossStage(m_byStage)) //wait until everyone arrive in boss stage
	{
		if (IsEveryoneReady())
		{
			m_bIsEveryoneReady = true;

			CNtlPacket packet(sizeof(sGU_BATTLE_DUNGEON_STATE_UPATE_NFY));
			sGU_BATTLE_DUNGEON_STATE_UPATE_NFY* res = (sGU_BATTLE_DUNGEON_STATE_UPATE_NFY *)packet.GetPacketData();
			res->wOpCode = GU_BATTLE_DUNGEON_STATE_UPATE_NFY;
			res->byStage = GetOwner()->GetCCBDStage();
			res->dwLimitTime = 0;
			res->titleTblidx = 41;
			res->subTitleTblidx = 42;
			packet.SetPacketLen(sizeof(sGU_BATTLE_DUNGEON_STATE_UPATE_NFY));
			GetOwner()->Broadcast(&packet);
		}
		else
			return m_status;
	}

	//if ccbd failed then teleport everyone out
	if (GetOwner()->GetCCBDFail())
	{
		if (m_dwFailTimer > 0) //check if over 0 to avoid possible double teleport
		{
			m_dwFailTimer = UnsignedSafeDecrease<DWORD>(m_dwFailTimer, dwTickDiff);

			if (m_dwFailTimer == 0)
			{
				TeleportOut();
			}
		}
		return m_status;
	}
	else
	{
		if (CheckPlayersState())
			return m_status;
	}

	m_status = UpdateSubControlQueue(dwTickDiff, fMultiple); //update actions

	return m_status;
}


void CWpsScriptAlgoAction_CCBD_stage::SpawnNextStage()
{
	//reset players loc/dir and send direct play

	CNtlPacket packet(sizeof(sGU_CHAR_DIRECT_PLAY));
	sGU_CHAR_DIRECT_PLAY* res = (sGU_CHAR_DIRECT_PLAY *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_DIRECT_PLAY;
	res->bCanSkip = false;
	res->bSynchronize = true; //not sure
	res->byPlayMode = 1; //eDIRECTION_TYPE
	res->directTblidx = g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.directPlay_StageChange;
	packet.SetPacketLen(sizeof(sGU_CHAR_DIRECT_PLAY));

	CNtlVector destLoc(g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.sEnterLoc_NormalStage.sLoc);
	CNtlVector destDir(g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.sEnterLoc_NormalStage.sDir);

	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID())
		{
			if (pPlayer->IsFainting() == false)
			{
				pPlayer->SetCurLoc(destLoc, GetOwner()->GetWorld());
				pPlayer->SetCurDir(destDir);
				pPlayer->SendCharStateSpawning(TELEPORT_TYPE_DUNGEON);

				pPlayer->SendPacket(&packet);
			}
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "User is registered for ccbd but has different world id (%u != %u) !!!", pPlayer->GetWorldID(), GetOwner()->GetWorld()->GetID());
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}
}

void CWpsScriptAlgoAction_CCBD_stage::TeleportToBoss()
{
	//teleport players to boss and send dungeon state
	//if player is fainting then remove from CCBD

	BYTE byBossStageCount = (m_byStage / 5) - 1;

	WORLDID destWorld = GetOwner()->GetWorld()->GetID();
	CNtlVector destLoc(g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.aEnterLoc_BossStage[byBossStageCount].sLoc);
	CNtlVector destDir(g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.aEnterLoc_BossStage[byBossStageCount].sDir);

	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID())
		{
			pPlayer->StartTeleport(destLoc, destDir, destWorld, TELEPORT_TYPE_DEFAULT);
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "User is registered for ccbd but has different world id (%u != %u) !!!", pPlayer->GetWorldID(), GetOwner()->GetWorld()->GetID());
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}
}

void CWpsScriptAlgoAction_CCBD_stage::TeleportOut()
{
	CWorld* pWorld = GetOwner()->GetWorld();
	CNtlVector destLoc(pWorld->GetTbldat()->outWorldLoc);
	CNtlVector destDir(pWorld->GetTbldat()->outWorldDir);
	WORLDID destWorldID = pWorld->GetTbldat()->outWorldTblidx;

	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID())
		{
			//teleport out
			pPlayer->StartTeleport(destLoc, destDir, destWorldID, TELEPORT_TYPE_WORLD_MOVE);
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "User is registered for ccbd but has different world id (%u != %u) !!!", pPlayer->GetWorldID(), GetOwner()->GetWorld()->GetID());
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}
}

bool CWpsScriptAlgoAction_CCBD_stage::CheckPlayersState()
{
	//send dungeon fail nfy, set ccbd fail and return true when everyoene is dead

	DWORD dwPlayerFaintCount = 0;
	DWORD dwPlayerCount = 0;

	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID())
		{
			++dwPlayerCount;

			if (pPlayer->IsFainting())
			{
				++dwPlayerFaintCount;
			}
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "User is registered for ccbd but has different world id (%u != %u) !!!", pPlayer->GetWorldID(), GetOwner()->GetWorld()->GetID());
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}

	if (dwPlayerFaintCount >= dwPlayerCount)
	{
		CNtlPacket packet(sizeof(sGU_BATTLE_DUNGEON_FAIL_NFY));
		sGU_BATTLE_DUNGEON_FAIL_NFY* res = (sGU_BATTLE_DUNGEON_FAIL_NFY *)packet.GetPacketData();
		res->wOpCode = GU_BATTLE_DUNGEON_FAIL_NFY;
		GetOwner()->GetWorld()->Broadcast(&packet);

		GetOwner()->SetCCBDFail(true); //block further ccbd process & tp out in 10 seconds
		return true;
	}

	return false;
}

bool CWpsScriptAlgoAction_CCBD_stage::IsEveryoneReady()
{
	bool bFlag = true;

	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() != GetOwner()->GetWorld()->GetID() || pPlayer->GetCharStateID() == CHARSTATE_DIRECT_PLAY || pPlayer->GetCharStateID() == CHARSTATE_SPAWNING || pPlayer->GetCharStateID() == CHARSTATE_DESPAWNING || pPlayer->GetCharStateID() == CHARSTATE_TELEPORTING)
		{
			bFlag = false;
			break;
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}

	return bFlag;
}
