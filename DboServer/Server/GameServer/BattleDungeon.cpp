#include "stdafx.h"
#include "DungeonManager.h" //include BattleDungeon.h
#include "GameServer.h"
#include "GameMain.h"
#include "TableContainerManager.h"
#include "ScriptAlgoObjectManager.h"
#include "WpsAlgoObject.h"
#include "CPlayer.h"
#include "ServerConfigTable.h"


CBattleDungeon::CBattleDungeon(PARTYID partyId) :
	m_partyId(partyId)
{
	m_pWorld = NULL;
}


CBattleDungeon::~CBattleDungeon()
{
	Destroy();
}


bool CBattleDungeon::Create(CPlayer* pPlayer, BYTE byStartStage)
{
	/* Info:
		Battle Dungeon script hosted on World
	*/

	sWORLD_TBLDAT* pWorld = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.battleDungeonWorld);
	if (pWorld == NULL)
	{
		printf("cant find world tblidx %u \n", g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.battleDungeonWorld);
		return false;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld(pWorld);
	if (m_pWorld == NULL)
		return false;


	CWpsAlgoObject* pObj = (CWpsAlgoObject*)m_pWorld->GetScript(pWorld->wpsLinkIndex);
	if (pObj == NULL)
	{
		ERR_LOG(LOG_SCRIPT, "Could not create wps script. %u world %u", pWorld->wpsLinkIndex, pWorld->tblidx);
		return false;
	}

	pObj->SetCCBDStartStage(byStartStage);

	pPlayer->GetParty()->EnterBattleDungeon(pPlayer, this, m_pWorld, pObj);

	pObj->Start(); //start dungeon script

	return true;
}


void CBattleDungeon::Destroy()
{
	if (m_setPlayers.size() > 0)
		ERR_LOG(LOG_GENERAL, "IMPORTANT-ERROR: TRYING TO DELETE BATTLE DUNGEON WITH PLAYERS INSIDE!");

	m_setPlayers.clear();

	if (m_pWorld)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);
	}
}


void CBattleDungeon::AddPlayer(HOBJECT hPlayer)
{
	m_setPlayers.insert(hPlayer);
}


void CBattleDungeon::JoinDungeon(CPlayer* pPlayer)
{
	m_setPlayers.insert(pPlayer->GetID());

	GetWorld()->AddScriptToPlayer(pPlayer);
}


void CBattleDungeon::LeaveDungeon(HOBJECT hPlayer)
{
	auto it = m_setPlayers.find(hPlayer);

	if (it == m_setPlayers.end())
	{
		//	printf("Player %u not found in ultimate dungeon \n", hPlayer);
		return;
	}

	m_setPlayers.erase(hPlayer);

	if (m_setPlayers.size() == 0) //if no players left then delete dungeon
	{
		//	printf("CBattleDungeon::LeaveDungeon(): Idx %u \n", m_pDungeonTbldat->tblidx);
		g_pDungeonManager->DestroyBattleDungeon(this);
	}
}

