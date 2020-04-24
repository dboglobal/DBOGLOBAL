#include "stdafx.h"
#include "TimeLeapDungeon.h"
#include "GameServer.h"
#include "GameMain.h"
#include "TableContainerManager.h"
#include "ScriptAlgoObjectManager.h"
#include "CPlayer.h"


CTimeLeapDungeon::CTimeLeapDungeon(CPlayer* pPlayer, sDUNGEON_TBLDAT* pDungeonTbldat) :
m_pOwner(pPlayer), m_pDungeonTbldat(pDungeonTbldat)
{
	m_pWorld = NULL;
}


CTimeLeapDungeon::~CTimeLeapDungeon()
{
	Destroy();
}


bool CTimeLeapDungeon::Create()
{
	/* Info:
		- TLQ 1-3 dont create any WPS script which is attached to world
		- Adult quest count as TLQ
		- Def test center (Master class quest dungeon) count as TLQ and a WPS script is attached to world.
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

	if (pWorld->wpsLinkIndex != INVALID_TBLIDX)
	{
		bool rbStart = false;
		CScriptAlgoObject* pObj = m_pWorld->GetScript(pWorld->wpsLinkIndex);
		if (pObj)
		{
			pObj->AddPlayer(GetOwner()->GetID());

			pObj->Start();
		}
		else return false;
	}

	return true;
}


void CTimeLeapDungeon::Destroy()
{
	if (m_pWorld)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);
	}
}




