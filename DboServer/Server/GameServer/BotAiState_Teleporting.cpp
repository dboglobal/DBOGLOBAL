#include "stdafx.h"
#include "BotAiState_Teleporting.h"
#include "SummonPet.h"
#include "GameServer.h"
#include "GameMain.h"


CBotAiState_Teleporting::CBotAiState_Teleporting(CNpc* pBot, WORLDID worldId, TBLIDX worldTblidx, CNtlVector& vTeleportLoc, CNtlVector& vTeleportDir)
	:CBotAiState(pBot, BOTCONTROL_TELEPORTING, "BOTCONTROL_TELEPORTING")
{
	m_worldId = worldId;
	m_worldTblidx = worldTblidx;
	m_vTeleportLoc.operator=(vTeleportLoc);
	m_vTeleportDir.operator=(vTeleportDir);
	m_eTeleportProcess = CBotAiState_Teleporting::DESPAWN;
	m_dwCharConditionFlag = 0;
	m_dwTime = 0;

	m_dwCharConditionFlag |= 0x10;
	m_dwCharConditionFlag |= 0x100000;
	m_dwCharConditionFlag |= 4;
}

CBotAiState_Teleporting::~CBotAiState_Teleporting()
{
}


void CBotAiState_Teleporting::OnEnter()
{
	RemoveAllSubControl();

	if (BIT_FLAG_TEST(GetBot()->GetSpawnFuncFlag(), 1))
		m_dwTime = 3000;

	GetBot()->GetTargetListManager()->ClearAggro();

	if (GetBot()->GetObjType() == OBJTYPE_SUMMON_PET)
	{
		CSummonPet* pPet = dynamic_cast<CSummonPet*>(GetBot());
		if (pPet)
		{
			pPet->SetAttackTarget(INVALID_HOBJECT);
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "pBotPet is not CBotPet type., GetBot() = %016x", GetBot());
		}
	}

	//GetBot()->GetStateManager()->AddConditionFlags(m_dwCharConditionFlag, false);
	GetBot()->StartTeleport(m_vTeleportLoc, m_vTeleportDir, m_worldId, TELEPORT_TYPE_DEFAULT);

	((CGameServer*)g_pApp)->GetGameMain()->GetWorldManager()->LeaveObject(GetBot());
}

void CBotAiState_Teleporting::OnExit()
{
	//GetBot()->GetStateManager()->RemoveConditionFlags(m_dwCharConditionFlag, false);

	CGameServer* app = (CGameServer*)g_pApp;

	CWorld* pWorld = app->GetGameMain()->GetWorldManager()->FindWorld(m_worldId);
	if (pWorld)
	{
		GetBot()->SetCurLoc(m_vTeleportLoc);
		GetBot()->SetCurDir(m_vTeleportDir);

		GetBot()->GetStateManager()->ChangeCharState(CHARSTATE_STANDING, NULL, true);

		pWorld->Enter(GetBot());
	}
}

int CBotAiState_Teleporting::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime >= 100)
	{
		GetBot()->GetBotController()->ChangeControlState_Idle();
		m_status = COMPLETED;
	}

	return m_status;
}
