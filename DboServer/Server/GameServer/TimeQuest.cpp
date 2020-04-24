#include "stdafx.h"
#include "DungeonManager.h" //#include "TimeQuest.h"
#include "CPlayer.h"
#include "TableContainerManager.h"
#include "GameServer.h"
#include "GameMain.h"
#include "ScriptAlgoObjectManager.h"
#include "TqsAlgoObject.h"
#include "NtlPacketGU.h"
#include "NtlResultCode.h"
#include "NtlDirection.h"


CTimeQuest::CTimeQuest(PARTYID partyid, sTIMEQUEST_TBLDAT* pTimeQuestTbldat, BYTE byDifficulty, BYTE byMode)
{
	Initalize();

	m_partyId = partyid;

	m_pTimeQuestTbldat = pTimeQuestTbldat;

	m_ruleInfo.byDifficulty = byDifficulty;
	m_ruleInfo.timeQuestTblidx = pTimeQuestTbldat->tblidx;
	m_ruleInfo.byTimeQuestMode = byMode;

	if(byMode == TIMEQUEST_MODE_PARTY)
		m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_WAIT;
	else
		m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_PREPARE;

	m_ruleInfo.sTimeQuestState.sTimeQuestStage.byStageNumber = 0;
	m_ruleInfo.bCountDown = false;
	m_ruleInfo.byStartHour = 0;
	m_ruleInfo.byStartMin = 0;

	m_dwWaitTime = 0;
	m_wTmqPoints = 0;
	m_byCoupons = 0;

	m_dwLeaveTickCount = 0;
	m_dwExitTickCount = 2000;
}


CTimeQuest::~CTimeQuest()
{
	if (m_mapPlayers.size() > 0)
		ERR_LOG(LOG_GENERAL, "IMPORTANT-ERROR: TRYING TO DELETE TIME QUEST DUNGEON WITH PLAYERS INSIDE!");

	m_mapPlayers.clear();

	if (m_pWorld)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);
	}
}


void CTimeQuest::Initalize()
{
	m_pScript = NULL;
	memset(memberCharId, INVALID_CHARACTERID, sizeof(memberCharId));
	m_pTimeQuestTbldat = NULL;
	memset(&m_ruleInfo, NULL, sizeof(m_ruleInfo));
}



bool CTimeQuest::TickProcess(DWORD dwTickDiff)
{
	if (m_mapPlayers.size() == 0 && m_ruleInfo.sTimeQuestState.byGameState != TIMEQUEST_GAME_STATE_CLOSE) //if no players anymore inside then set state close to avoid bugs
	{
		if (GetTQS())
			GetTQS()->SetPaused(true);

		m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_CLOSE;
	}

	switch (m_ruleInfo.sTimeQuestState.byGameState)
	{
		case TIMEQUEST_GAME_STATE_WAIT:
		{
			m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);

			if (m_dwWaitTime == 0)
			{
				for (std::map<HOBJECT, CPlayer*>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
				{
					CPlayer* pPlayer = it->second;
					if (pPlayer && pPlayer->IsInitialized())
					{
						pPlayer->SetEnterTmqLoc(); //save players current loc. Used when teleport out.
						pPlayer->StartTeleport(GetWorld()->GetTbldat()->vStart1Loc, GetWorld()->GetTbldat()->vStart1Dir, GetWorld()->GetID(), TELEPORT_TYPE_TMQ_WORLD_MOVE);
					}
				}

				m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_PREPARE;
			}
		}
		break;
		case TIMEQUEST_GAME_STATE_PREPARE: //
		{
			for (std::map<HOBJECT, CPlayer*>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
			{
				CPlayer* pPlayer = it->second;
				if (pPlayer && pPlayer->IsInitialized())
				{
					if (pPlayer->GetWorldID() != GetWorld()->GetID())
						return false;
				}
			}

			m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_ARRIVE;
		}
		break;
		case TIMEQUEST_GAME_STATE_ARRIVE: //send arrive direction
		{
			for (std::map<HOBJECT, CPlayer*>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
			{
				CPlayer* pPlayer = it->second;
				if (pPlayer && pPlayer->IsInitialized())
				{
					if (pPlayer->GetStateManager()->IsCharCondition(CHARCOND_DIRECT_PLAY))
					{
						return false;
					}
				}
			}

			//if (GetTmqTable()->arriveCharacterDirection != INVALID_TBLIDX)
			//{
			//	for (std::map<HOBJECT, CPlayer*>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
			//	{
			//		CPlayer* pPlayer = it->second;
			//		if (pPlayer && pPlayer->IsInitialized())
			//		{
			//			pPlayer->StartDirectPlay(DIRECT_PLAY_TQS_ARRIVE, GetTmqTable()->arriveCharacterDirection, true, false, DIRECTION_CINEMATIC, false);
			//		}
			//	}
			//}

			CNtlPacket packet(sizeof(sGU_TIMEQUEST_UPDATE_GAME_STATE));
			sGU_TIMEQUEST_UPDATE_GAME_STATE * res = (sGU_TIMEQUEST_UPDATE_GAME_STATE *)packet.GetPacketData();
			res->wOpCode = GU_TIMEQUEST_UPDATE_GAME_STATE;
			res->sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_ARRIVE;
			res->sTimeQuestState.sTimeQuestStage.byStageNumber = m_ruleInfo.sTimeQuestState.sTimeQuestStage.byStageNumber;
			GetWorld()->Broadcast(&packet);

			m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_BEGIN;
		}
		break;
		case TIMEQUEST_GAME_STATE_BEGIN: //wait until arrival direction done
		{
			//for (std::map<HOBJECT, CPlayer*>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
			//{
			//	CPlayer* pPlayer = it->second;
			//	if (pPlayer && pPlayer->IsInitialized())
			//	{
			//		if (pPlayer->GetCharStateID() == CHARSTATE_DIRECT_PLAY)
			//		{
			//			return;
			//		}
			//	}
			//}

			CNtlPacket packet(sizeof(sGU_TIMEQUEST_UPDATE_GAME_STATE));
			sGU_TIMEQUEST_UPDATE_GAME_STATE * res = (sGU_TIMEQUEST_UPDATE_GAME_STATE *)packet.GetPacketData();
			res->wOpCode = GU_TIMEQUEST_UPDATE_GAME_STATE;
			res->sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_BEGIN;
			res->sTimeQuestState.sTimeQuestStage.byStageNumber = m_ruleInfo.sTimeQuestState.sTimeQuestStage.byStageNumber;
			GetWorld()->Broadcast(&packet);

			m_pScript->Start();

			m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_STAGE_READY;
		}
		break;
		case TIMEQUEST_GAME_STATE_STAGE_READY:
		{
		}
		break;
		case TIMEQUEST_GAME_STATE_STAGE_START:
		{
		}
		break;
		case TIMEQUEST_GAME_STATE_STAGE_FINISH:
		{
		}
		break;
		case TIMEQUEST_GAME_STATE_END:
		{
			if (GetTmqTable()->leaveCharacterDirection != INVALID_TBLIDX)
			{

				m_dwLeaveTickCount = 13000;
			}

			m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_LEAVE;
		}
		break;
		case TIMEQUEST_GAME_STATE_LEAVE: 
		{
			m_dwLeaveTickCount = UnsignedSafeDecrease<DWORD>(m_dwLeaveTickCount, dwTickDiff);

			if (m_dwLeaveTickCount == 0)
			{
				for (std::map<HOBJECT, CPlayer*>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
				{
					CPlayer* pPlayer = it->second;
					if (pPlayer && pPlayer->IsInitialized() && GetWorld()->GetID() == pPlayer->GetWorldID())
					{
						if (m_ruleInfo.byTimeQuestMode == TIMEQUEST_MODE_PARTY)
							pPlayer->StartTeleport(pPlayer->GetEnterTmqLoc(), pPlayer->GetCurDir(), GetWorld()->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_DEFAULT);
						else
						{
							pPlayer->SendQueryTutorialUpdate(true);
							pPlayer->StartTeleport(CNtlVector(pPlayer->GetBindLoc()), CNtlVector(pPlayer->GetBindDir()), pPlayer->GetBindWorldID(), TELEPORT_TYPE_DEFAULT);
						}
					}
				}

				m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_CLOSE;
			}
		}
		break;
		case TIMEQUEST_GAME_STATE_FAIL:
		{
			m_dwLeaveTickCount = UnsignedSafeDecrease<DWORD>(m_dwLeaveTickCount, dwTickDiff);

			if (m_dwLeaveTickCount == 0)
			{
				for (std::map<HOBJECT, CPlayer*>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
				{
					CPlayer* pPlayer = it->second;
					if (pPlayer && pPlayer->IsInitialized() && GetWorld()->GetID() == pPlayer->GetWorldID())
					{
						pPlayer->StartTeleport(pPlayer->GetEnterTmqLoc(), pPlayer->GetCurDir(), GetWorld()->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_DEFAULT);
					}
				}

				m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_CLOSE;
			}
		}
		break;
		case TIMEQUEST_GAME_STATE_CLOSE:
		{
			m_dwExitTickCount = UnsignedSafeDecrease<DWORD>(m_dwExitTickCount, dwTickDiff);
			if (m_dwExitTickCount == 0 && m_mapPlayers.size() == 0)
			{
				m_mapPlayers.clear();

				return true;
			}
		}
		break;
	}

	return false;
}


bool CTimeQuest::Create(CPlayer* pPlayer)
{
	/* Info:
	- TMQ IS HOSTED ON WORLD
	*/

	sWORLD_TBLDAT* pWorld = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTimeQuestTbldat->sTimeQuestDataset[m_ruleInfo.byDifficulty].worldTblidx);
	if (pWorld == NULL)
	{
		printf("CTimeQuest:Create:: cant find world tblidx %u \n", m_pTimeQuestTbldat->sTimeQuestDataset[m_ruleInfo.byDifficulty].worldTblidx);
		return false;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld(pWorld);
	if (m_pWorld == NULL)
		return false;

	CTqsAlgoObject* pObj = g_pScriptAlgoManager->CreateTqsObject(m_pTimeQuestTbldat->sTimeQuestDataset[m_ruleInfo.byDifficulty].scriptTblidx, m_pTimeQuestTbldat);
	if (pObj)
	{
		pObj->SetWorld(m_pWorld);
		m_pWorld->AddScript(pObj);

		pObj->SetTimeQuest(this);

		m_pScript = pObj;
	}
	else
	{
		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);

		return false;
	}

	if (GetTmqTable()->startCharacterDirection != INVALID_TBLIDX)
		m_dwWaitTime = 21000; //wait 22 seconds before starting teleport


	if (m_ruleInfo.byTimeQuestMode == TIMEQUEST_MODE_PARTY)
	{
		for (int i = 0; i < pPlayer->GetParty()->GetPartyMemberCount(); i++)
		{
			CPlayer* pMember = g_pObjectManager->GetPC(pPlayer->GetParty()->GetMemberInfo(i).hHandle);
			if (pMember && pMember->IsInitialized())
			{
				if (pPlayer->IsInRange(pMember, 100.f))
				{
					AddMember(pMember);
				}
			}
		}
	}
	else
	{
		AddMember(pPlayer);
	}

	return true;
}


void CTimeQuest::UpdateCoupon()
{
	CNtlPacket packet2(sizeof(sGU_TIMEQUEST_UPDATE_COUPON_COUNT));
	sGU_TIMEQUEST_UPDATE_COUPON_COUNT * res2 = (sGU_TIMEQUEST_UPDATE_COUPON_COUNT *)packet2.GetPacketData();
	res2->wOpCode = GU_TIMEQUEST_UPDATE_COUPON_COUNT;
	res2->byCouponCount = m_byCoupons;
	GetWorld()->Broadcast(&packet2);
}


void CTimeQuest::CopyRuleInfoTo(sTIMEQUEST_RULE_INFO * pRuleInfo)
{
	pRuleInfo->bCountDown = m_ruleInfo.bCountDown;
	pRuleInfo->byDifficulty = m_ruleInfo.byDifficulty;
	pRuleInfo->byStartHour = m_ruleInfo.byStartHour;
	pRuleInfo->byStartMin = m_ruleInfo.byStartMin;
	pRuleInfo->byTimeQuestMode = m_ruleInfo.byTimeQuestMode;
	pRuleInfo->sTimeQuestState.byGameState = m_ruleInfo.sTimeQuestState.byGameState;
	pRuleInfo->sTimeQuestState.sTimeQuestStage.byStageNumber = m_ruleInfo.sTimeQuestState.sTimeQuestStage.byStageNumber;
	pRuleInfo->timeQuestTblidx = m_ruleInfo.timeQuestTblidx;
}


void CTimeQuest::AddMember(CPlayer* player)
{
	m_mapPlayers.insert({ player->GetID(), player });
	player->SetTMQ(this);

	m_pScript->AddPlayer(player->GetID());
	player->AddScript(m_pScript);

	if (GetTmqTable()->startCharacterDirection != INVALID_TBLIDX)
	{
		CNtlPacket packet2(sizeof(sGU_TIMEQUEST_ENTER_DIRECTION_NFY));
		sGU_TIMEQUEST_ENTER_DIRECTION_NFY * res2 = (sGU_TIMEQUEST_ENTER_DIRECTION_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_TIMEQUEST_ENTER_DIRECTION_NFY;
		res2->tmqTblidx = GetTmqTable()->tblidx;
		player->SendPacket(&packet2);

		player->GetStateManager()->AddConditionState(CHARCOND_DIRECT_PLAY, NULL, false);
		player->StartDirectPlay(DIRECT_PLAY_TQS_START, GetTmqTable()->startCharacterDirection, true, false, INVALID_DIRECTION_TYPE, false);
	}
}

void CTimeQuest::RemoveMember(HOBJECT hHandle)
{
	m_mapPlayers.erase(hHandle);
}

void CTimeQuest::UpdateTmqPoints()
{
	m_wTmqPoints += 30;

	if (m_wTmqPoints > DBO_TIMEQUEST_COUNPON_CHANGE_POINT)
	{
		if (m_byCoupons < DBO_TIMEQUEST_COUNPON_LIMIT_COUNT)
		{
			m_wTmqPoints = 20;

			m_byCoupons += 1;

			UpdateCoupon();
		}
		else
		{
			m_wTmqPoints = DBO_TIMEQUEST_COUNPON_CHANGE_POINT;
		}
	}

	CNtlPacket packet(sizeof(sGU_TIMEQUEST_UPDATE_TMQ_POINT));
	sGU_TIMEQUEST_UPDATE_TMQ_POINT * res = (sGU_TIMEQUEST_UPDATE_TMQ_POINT *)packet.GetPacketData();
	res->wOpCode = GU_TIMEQUEST_UPDATE_TMQ_POINT;
	res->wTmqPoint = m_wTmqPoints;
	GetWorld()->Broadcast(&packet);
}

void CTimeQuest::UpdateTmqFail()
{
	m_ruleInfo.sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_FAIL;

	m_dwLeaveTickCount = 5000; //teleport out after 5 seconds

	GetTQS()->SetPaused(true); //stop tqs from executing

	CNtlPacket packet(sizeof(sGU_TIMEQUEST_UPDATE_GAME_STATE));
	sGU_TIMEQUEST_UPDATE_GAME_STATE * res = (sGU_TIMEQUEST_UPDATE_GAME_STATE *)packet.GetPacketData();
	res->wOpCode = GU_TIMEQUEST_UPDATE_GAME_STATE;
	res->sTimeQuestState.byGameState = TIMEQUEST_GAME_STATE_FAIL;
	res->sTimeQuestState.sTimeQuestStage.byStageNumber = m_ruleInfo.sTimeQuestState.sTimeQuestStage.byStageNumber;
	GetWorld()->Broadcast(&packet);
}
