#include "stdafx.h"
#include "DungeonManager.h" //include ultimatedungeon.h
#include "GameServer.h"
#include "GameMain.h"
#include "TableContainerManager.h"
#include "ScriptAlgoObjectManager.h"
#include "CPlayer.h"


CUltimateDungeon::CUltimateDungeon(PARTYID partyId, sDUNGEON_TBLDAT* pDungeonTbldat) :
m_partyId(partyId), m_pDungeonTbldat(pDungeonTbldat)
{
	m_pWorld = NULL;
}


CUltimateDungeon::~CUltimateDungeon()
{
	Destroy();
}


bool CUltimateDungeon::Create(CPlayer* pPlayer)
{
	/* Info:
		When enter Ultimate Dungeon/BID there is always one script which is attached to world.
	*/

	sWORLD_TBLDAT* pWorld = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pDungeonTbldat->linkWorld);
	if (pWorld == NULL)
	{
		printf("cant find world tblidx %u \n", m_pDungeonTbldat->linkWorld);
		return false;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld(pWorld);
	if (m_pWorld == NULL)
		return false;


	CScriptAlgoObject* pObj = m_pWorld->GetScript(pWorld->wpsLinkIndex);
	if (pObj == NULL)
	{
		ERR_LOG(LOG_SCRIPT, "Could not create wps script. %u world %u", pWorld->wpsLinkIndex, pWorld->tblidx);
		return false;
	}

	pPlayer->GetParty()->EnterDungeon(pPlayer, this, m_pWorld, pObj);

	pObj->Start(); //start dungeon script

	return true;
}


void CUltimateDungeon::Destroy()
{
	if (m_setPlayers.size() > 0)
		ERR_LOG(LOG_GENERAL, "IMPORTANT-ERROR: TRYING TO DELETE ULTIMATE DUNGEON WITH PLAYERS INSIDE!");

	m_setPlayers.clear();

	if (m_pWorld)
	{
		CGameServer* app = (CGameServer*)g_pApp;
	//	printf("CUltimateDungeon::Destroy(): Idx %u \n", m_pDungeonTbldat->tblidx);
		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);
	}
}


void CUltimateDungeon::AddPlayer(HOBJECT hPlayer)
{
	m_setPlayers.insert(hPlayer);
}


void CUltimateDungeon::JoinDungeon(CPlayer* pPlayer)
{
	m_setPlayers.insert(pPlayer->GetID());

	GetWorld()->AddScriptToPlayer(pPlayer);
}


void CUltimateDungeon::LeaveDungeon(HOBJECT hPlayer)
{
	std::set<HOBJECT>::iterator it = m_setPlayers.find(hPlayer);

	if (it == m_setPlayers.end())
	{
	//	printf("Player %u not found in ultimate dungeon \n", hPlayer);
		return;
	}

	m_setPlayers.erase(hPlayer);

	if (m_setPlayers.size() == 0) //if no players left then delete dungeon
	{
	//	printf("CUltimateDungeon::LeaveDungeon(): Idx %u \n", m_pDungeonTbldat->tblidx);
		g_pDungeonManager->DestroyUltimateDungeon(this);
	}
}

