#include "stdafx.h"
#include "DragonballScramble.h"
#include "CPlayer.h"
#include "GameServer.h"
#include "NtlPacketGU.h"
#include "NtlPacketGM.h"
#include "NtlPacketGQ.h"
#include "ObjectManager.h"
#include "NtlResultCode.h"
#include "TableContainerManager.h"
#include "DragonballReturnPointTable.h"
#include "NtlRandom.h"
#include "ItemManager.h"
#include "ItemDrop.h"


CDragonballScramble::CDragonballScramble()
{
	m_dwNextUpdateTick = GetTickCount();

	Init();

	m_timeEnd = INVALID_DBOTIME; //dont do this inside init or scramble will restart automatic after it ends within 2 hours
}


CDragonballScramble::~CDragonballScramble()
{
	Destroy();
}


void CDragonballScramble::Init()
{
	m_bOnOff = false;
	m_timeStart = INVALID_DBOTIME;

	memset(m_arrBalls, NULL, sizeof(m_arrBalls));
	m_mapPlayers.clear();
}


void CDragonballScramble::Destroy()
{
}


void CDragonballScramble::Create()
{
}


void CDragonballScramble::TickProcess(DWORD dwTick)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (dwTick < m_dwNextUpdateTick)
		return;

	DBOTIME curTime = app->GetTime();

	if (m_bOnOff == false)
	{
		if (m_timeEnd > curTime) //to avoid that scramble will start again once it has end very fast
			return;

		tm timeStruct = {};
		localtime_s(&timeStruct, &curTime);

		if (timeStruct.tm_wday == 1 && (timeStruct.tm_hour == 12 || timeStruct.tm_hour == 13)) //check if its monday 12, 13 o clock
		{
			m_timeStart = curTime;

			tm timeEndStruct = timeStruct;
			timeEndStruct.tm_sec = 0;
			timeEndStruct.tm_min = 0;
			timeEndStruct.tm_hour = 14;

			m_timeEnd = mktime(&timeEndStruct);

			StartEvent(false);
		}
		else if (timeStruct.tm_wday == 3 && (timeStruct.tm_hour == 0 || timeStruct.tm_hour == 1)) //check if its wednesday 0, 1 o clock
		{
			m_timeStart = curTime;

			tm timeEndStruct = timeStruct;
			timeEndStruct.tm_sec = 0;
			timeEndStruct.tm_min = 0;
			timeEndStruct.tm_hour = 2;

			m_timeEnd = mktime(&timeEndStruct);

			StartEvent(false);
		}
		else if (timeStruct.tm_wday == 5 && (timeStruct.tm_hour == 20 || timeStruct.tm_hour == 21)) //check if its friday 20, 21 o clock
		{
			m_timeStart = curTime;

			tm timeEndStruct = timeStruct;
			timeEndStruct.tm_sec = 0;
			timeEndStruct.tm_min = 0;
			timeEndStruct.tm_hour = 22;

			m_timeEnd = mktime(&timeEndStruct);

			StartEvent(false);
		}
	}
	else
	{
		if (curTime >= m_timeEnd)
		{
			EndEvent();
		}
		else
		{
			SendDragonballLocation();
		}
	}

	m_dwNextUpdateTick = dwTick + 2000; //update again in 2 seconds
}


void CDragonballScramble::StartEvent(bool bStartByCommand/* = true*/)
{
	if (bStartByCommand)
	{
		if (m_bOnOff)
			return;

		CGameServer* app = (CGameServer*)g_pApp;

		m_timeStart = app->GetTime();
		m_timeEnd = m_timeStart + 7200;
	}

	m_bOnOff = true;

	// -- Spawn the balls
	for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
	{
		m_arrBalls[i].bOnGround = true;
		m_arrBalls[i].hOwner = INVALID_HOBJECT;

		sDRAGONBALL_RETURN_POINT_TBLDAT* pTbldat = (sDRAGONBALL_RETURN_POINT_TBLDAT*)g_pTableContainer->GetDragonBallReturnPointTable()->FindData((TBLIDX)RandomRange(1, (int)g_pTableContainer->GetDragonBallReturnPointTable()->GetNumberOfTables()));

		m_arrBalls[i].vLoc.x = pTbldat->fField_X;
		m_arrBalls[i].vLoc.y = pTbldat->fField_Y;
		m_arrBalls[i].vLoc.z = pTbldat->fField_Z;

		CItemDrop * pBall = g_pItemManager->CreateSingleDrop(100.f, 200041 + i);
		if (pBall)
		{
			pBall->AddToGround(1, m_arrBalls[i].vLoc);

			m_arrBalls[i].hHandle = pBall->GetID();
			m_arrBalls[i].tblidx = pBall->GetTblidx();
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "FAILED TO CREATE DRAGONBALL SCRAMBLE DROP. TBLIDX %u", 200041 + i);
		}
	}

	ERR_LOG(LOG_GENERAL, "<Dragonball Scramble Event> Start %I64u, End %I64u, Duration in seconds %I64u", m_timeStart, m_timeEnd, m_timeEnd - m_timeStart);

	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
	sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * res = (sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
	res->bIsTypeChange = true;
	res->wSeasonState = eSTART;
	g_pObjectManager->SendPacketToAll(&packet);

	WCHAR* wMsg = L"Dragonball Scramble Event Started!";

	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packetGM(sizeof(sGM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
	sGM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * resGM = (sGM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY*)packetGM.GetPacketData();
	resGM->wOpCode = GM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
	resGM->serverChannelID = app->m_config.byChannel;
	resGM->serverFarmID = app->m_config.byServerID;
	resGM->wSeasonState = eSTART;
	app->SendTo(app->GetMasterServerSession(), &packetGM);

	CNtlPacket packetGQ(sizeof(sGQ_DRAGONBALL_SCRAMBLE_NFY));
	sGQ_DRAGONBALL_SCRAMBLE_NFY * resGQ = (sGQ_DRAGONBALL_SCRAMBLE_NFY*)packetGQ.GetPacketData();
	resGQ->wOpCode = GQ_DRAGONBALL_SCRAMBLE_NFY;
	resGQ->bStart = true;
	app->SendTo(app->GetQueryServerSession(), &packetGQ);
}


void CDragonballScramble::EndEvent(bool bForce/* = false*/)
{
	if (m_bOnOff == false && bForce == false)
		return;


	ERR_LOG(LOG_GENERAL, "<Dragonball Scramble Event> End.");


	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
	sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * res = (sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
	res->bIsTypeChange = true;
	res->wSeasonState = eEND;
	g_pObjectManager->SendPacketToAll(&packet);

	CGameServer* app = (CGameServer*)g_pApp;


	CNtlPacket packetGM(sizeof(sGM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
	sGM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * resGM = (sGM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY*)packetGM.GetPacketData();
	resGM->wOpCode = GM_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
	resGM->serverChannelID = app->m_config.byChannel;
	resGM->serverFarmID = app->m_config.byServerID;
	resGM->wSeasonState = eEND;
	app->SendTo(app->GetMasterServerSession(), &packetGM);

	CNtlPacket packetGQ(sizeof(sGQ_DRAGONBALL_SCRAMBLE_NFY));
	sGQ_DRAGONBALL_SCRAMBLE_NFY * resGQ = (sGQ_DRAGONBALL_SCRAMBLE_NFY*)packetGQ.GetPacketData();
	resGQ->wOpCode = GQ_DRAGONBALL_SCRAMBLE_NFY;
	resGQ->bStart = false;
	app->SendTo(app->GetQueryServerSession(), &packetGQ);

	// ---

	for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
	{
		if (m_arrBalls[i].hOwner != INVALID_HOBJECT)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(m_arrBalls[i].hOwner);
			if (pPlayer)
			{
				pPlayer->SetDragonballScrambleBallFlag(0);

				CItem* pItem = pPlayer->GetPlayerItemContainer()->GetItem(m_arrBalls[i].hHandle);
				if (pItem)
					pItem->SetCount(0, false, true);
				else
				{
					ERR_LOG(LOG_GENERAL, "DRAGONBALL-SCRAMBLE: COULD NOT DELETE BALL FROM PLAYER");
				}
			}

			m_arrBalls[i].hOwner = INVALID_HOBJECT;
		}
		else
		{
			CItemDrop* pBall = g_pItemManager->FindDrop(m_arrBalls[i].hHandle);
			if (pBall)
				g_pItemManager->DestroyItemDropOverTime(pBall);
			else
				ERR_LOG(LOG_GENERAL, "DRAGONBALL-SCRAMBLE: COULD NOT FIND BALL ON GROUND !");
		}
	}

	// -- remove scramble flag from everyone

	for (std::map<CHARACTERID, HOBJECT>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->second);
		if (pPlayer)
		{
			pPlayer->SetDragonballScramble(false);
			pPlayer->SetDragonballScrambleBallFlag(0);
		}
	}

	// -- Init everything

	Init();
}

void CDragonballScramble::EndEventNfy()
{
	if (m_bOnOff == false)
		return;

	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
	sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * res = (sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
	res->bIsTypeChange = true;
	res->wSeasonState = eEND;
	g_pObjectManager->SendPacketToAll(&packet);

	for (std::map<CHARACTERID, HOBJECT>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->second);
		if (pPlayer)
		{
			pPlayer->SetDragonballScramble(false);
			pPlayer->SetDragonballScrambleBallFlag(0);
		}
	}

	Init();
}

void CDragonballScramble::RequestJoin(CPlayer * pPlayer)
{
	WORD wResultCode = SCRAMBLE_CANNOT_NO_SEASON;

	if (m_bOnOff == true)
	{
		if (pPlayer->GetLevel() < DBO_DRAGONBALL_SCRAMBLE_JOIN_LEVEL)
			wResultCode = SCRAMBLE_CANNOT_JOIN_LEVEL_IS_TOO_LOW;
		else if (pPlayer->GetCurWorld() == NULL)
			wResultCode = GAME_WORLD_NOT_FOUND; //to do
		else if(pPlayer->GetWorldID() != 1)//1 = main world
			wResultCode = GAME_FAIL;
		else if(pPlayer->GetCurWorld()->GetRuleType() == GAMERULE_TIMEQUEST)
			wResultCode = SCRAMBLE_CANNOT_JOIN_WHILE_TMQ;
		else if (pPlayer->GetCurWorld()->GetRuleType() == GAMERULE_RANKBATTLE || pPlayer->GetRankBattleRegisterObject() != INVALID_HOBJECT)
			wResultCode = SCRAMBLE_CANNOT_JOIN_WHILE_RANKBATTLE;
		else if (pPlayer->GetCurWorld()->GetRuleType() == GAMERULE_DOJO)
			wResultCode = SCRAMBLE_CANNOT_JOIN_WHILE_DOJO;
		else if (pPlayer->GetCurWorld()->GetRuleType() >= GAMERULE_MINORMATCH && pPlayer->GetCurWorld()->GetRuleType() <= GAMERULE_TEINKAICHIBUDOKAI)
			wResultCode = SCRAMBLE_CANNOT_JOIN_WHILE_TENKAICHIBUDOKAI;
		else if(pPlayer->GetFreeBattleID() != INVALID_DWORD)
			wResultCode = SCRAMBLE_CANNOT_JOIN_WHILE_FREEBATTLE;
		else if(pPlayer->GetAirState() == AIR_STATE_ON)
			wResultCode = GAME_COMMON_CANT_DO_THAT_IN_AIR_MODE_STATE;
		else if(m_mapPlayers.find(pPlayer->GetCharID()) != m_mapPlayers.end())
			wResultCode = GAME_GAMERULE_REG_ALREADY_JOINED_DOJO_SCRAMBLE;
		else
		{
			wResultCode = GAME_SUCCESS;
		}
	}

	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCRAMBLE_JOIN_RES));
	sGU_DRAGONBALL_SCRAMBLE_JOIN_RES * res = (sGU_DRAGONBALL_SCRAMBLE_JOIN_RES *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_JOIN_RES;
	res->wResultCode = wResultCode;
	pPlayer->SendPacket(&packet);

	if (wResultCode == GAME_SUCCESS)
	{
		//NFY EVERYONE
		CNtlPacket packetNfy(sizeof(sGU_DRAGONBALL_SCRAMBLE_JOIN_NFY));
		sGU_DRAGONBALL_SCRAMBLE_JOIN_NFY * res2 = (sGU_DRAGONBALL_SCRAMBLE_JOIN_NFY *)packetNfy.GetPacketData();
		res2->wOpCode = GU_DRAGONBALL_SCRAMBLE_JOIN_NFY;
		res2->handle = pPlayer->GetID();
		BroadCast(&packetNfy);

		CNtlPacket packet3(sizeof(sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
		sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * res3 = (sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY *)packet3.GetPacketData();
		res3->wOpCode = GU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
		res3->bIsTypeChange = true;
		res3->wSeasonState = eREFRESH;
		pPlayer->SendPacket(&packet3);

		pPlayer->SetDragonballScramble(true);

		m_mapPlayers.insert({ pPlayer->GetCharID(), pPlayer->GetID() });

		pPlayer->GetCharAtt()->CalculateAll();
	}
}

void CDragonballScramble::RequestBallLoc(CPlayer * pPlayer, bool bEnable)
{
	WORD wResultCode = GAME_SUCCESS;

	if (m_bOnOff == true)
	{
		if(pPlayer->GetDragonballScramble() == false)
			wResultCode = GAME_GAMERULE_REG_ALREADY_JOINED_DOJO_SCRAMBLE;
	}
	else wResultCode = SCRAMBLE_CANNOT_NO_SEASON;

	if (wResultCode == GAME_SUCCESS)
		SendDragonballLocation(pPlayer);

	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCRAMBLE_BALL_LOC_RES));
	sGU_DRAGONBALL_SCRAMBLE_BALL_LOC_RES * res = (sGU_DRAGONBALL_SCRAMBLE_BALL_LOC_RES *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_BALL_LOC_RES;
	res->wResultCode = wResultCode;
	res->bEnable = bEnable;
	pPlayer->SendPacket(&packet);
}

void CDragonballScramble::SpawnBall(CPlayer* pPlayer, bool bFaint/* = false*/, bool bLogout/* = false*/)
{
	if (bLogout)
		m_mapPlayers.erase(pPlayer->GetCharID());

	if (pPlayer->GetDragonballScrambleBallFlag() == 0)
		return;

	CNtlPacket packetNfy(sizeof(sGU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY));
	sGU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY * res = (sGU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY *)packetNfy.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY;
	res->handle = pPlayer->GetID();
	res->byHasDragonBallFlag = 0;
	BroadCast(&packetNfy);

	pPlayer->SetDragonballScrambleBallFlag(0);

	for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
	{
		if (pPlayer->GetID() == m_arrBalls[i].hOwner)
		{
			CItem* pItem = pPlayer->GetPlayerItemContainer()->GetItem(m_arrBalls[i].hHandle);
			if (pItem && pItem->IsLocked() == false)
			{ 
				pItem->SetCount(0, false, true);

				m_arrBalls[i].hOwner = INVALID_HOBJECT;

				if (bFaint == false)
				{
					sDRAGONBALL_RETURN_POINT_TBLDAT* pTbldat = (sDRAGONBALL_RETURN_POINT_TBLDAT*)g_pTableContainer->GetDragonBallReturnPointTable()->FindData((TBLIDX)RandomRange(1, (int)g_pTableContainer->GetDragonBallReturnPointTable()->GetNumberOfTables()));

					m_arrBalls[i].vLoc.x = pTbldat->fField_X;
					m_arrBalls[i].vLoc.y = pTbldat->fField_Y;
					m_arrBalls[i].vLoc.z = pTbldat->fField_Z;
				}
				else
				{
					pPlayer->GetCurLoc().CopyTo(m_arrBalls[i].vLoc);

					m_arrBalls[i].vLoc.x += 5.f;
				}

				CItemDrop * pBall = g_pItemManager->CreateSingleDrop(100.f, 200041 + i);
				if (pBall)
				{
					m_arrBalls[i].bOnGround = true;

					pBall->AddToGround(1, m_arrBalls[i].vLoc);

					m_arrBalls[i].hHandle = pBall->GetID();
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "FAILED TO CREATE DRAGONBALL SCRAMBLE DROP. TBLIDX %u", 200041 + i);
				}
			}
		}
	}
}

void CDragonballScramble::PickUpBall(CPlayer * pPlayer, HOBJECT hHandle, TBLIDX tblidx)
{
	for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
	{
		sBALLS& ball = m_arrBalls[i];

		if (ball.bOnGround == true && ball.tblidx == tblidx)
		{
			ball.bOnGround = false;
			ball.hHandle = hHandle;
			ball.hOwner = pPlayer->GetID();

		//printf("MAKE_BIT_FLAG(tblidx(%u) - 200040 - 1): %u \n", tblidx, MAKE_BIT_FLAG(tblidx - 200040 - 1));
			pPlayer->SetDragonballScrambleBallFlag(pPlayer->GetDragonballScrambleBallFlag() + MAKE_BIT_FLAG(tblidx - 200040 - 1));

			CNtlPacket packetNfy(sizeof(sGU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY));
			sGU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY * res = (sGU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY *)packetNfy.GetPacketData();
			res->wOpCode = GU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY;
			res->handle = pPlayer->GetID();
			res->byHasDragonBallFlag = pPlayer->GetDragonballScrambleBallFlag();
			BroadCast(&packetNfy);

			break;
		}
	}
}


void CDragonballScramble::BroadCast(CNtlPacket * pPacket)
{
	for (std::map<CHARACTERID, HOBJECT>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->second);
		if (pPlayer && pPlayer->IsInitialized())
			pPlayer->SendPacket(pPacket);
	}
}

void CDragonballScramble::SendDragonballLocation(CPlayer* pPlayer/* = NULL*/)
{
	CNtlPacket packetNfy(sizeof(sGU_DRAGONBALL_SCRAMBLE_BALL_LOC_UPDATE_NFY));
	sGU_DRAGONBALL_SCRAMBLE_BALL_LOC_UPDATE_NFY * res = (sGU_DRAGONBALL_SCRAMBLE_BALL_LOC_UPDATE_NFY *)packetNfy.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_BALL_LOC_UPDATE_NFY;

	for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
	{
		sBALLS& balls = m_arrBalls[i];

		if(balls.bOnGround)
			NtlLocationCompress(&res->vBallLoc[i], balls.vLoc.x, balls.vLoc.y, balls.vLoc.z);
		else
		{
			CPlayer* pBallOwner = g_pObjectManager->GetPC(balls.hOwner);
			if(pBallOwner)
				NtlLocationCompress(&res->vBallLoc[i], pBallOwner->GetCurLoc().x, pBallOwner->GetCurLoc().y, pBallOwner->GetCurLoc().z);
			else
			{
				ERR_LOG(LOG_GENERAL, "DRAGONBALL-SCRAMBLE: FATAL ERROR! Player does not exist anymore.");
			}
		}
	}

	if (pPlayer == NULL)
		BroadCast(&packetNfy);
	else
		pPlayer->SendPacket(&packetNfy);
}


void CDragonballScramble::LoadEvent(CPlayer * pChar)
{
	if (m_bOnOff == false)
		return;
	
	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
	sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * res = (sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
	res->bIsTypeChange = false; //if true = window will pop up. Otherwise no window
	res->wSeasonState = eSTART;
	pChar->SendPacket(&packet);
}

void CDragonballScramble::RefreshEvent(CPlayer * pChar)
{
	CNtlPacket packet(sizeof(sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
	sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * res = (sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
	res->bIsTypeChange = false; //if true = window will pop up. Otherwise no window
	res->wSeasonState = eSTART;
	pChar->SendPacket(&packet);

	CNtlPacket packet2(sizeof(sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY));
	sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY * res2 = (sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY;
	res2->bIsTypeChange = true; //if true = window will pop up. Otherwise no window
	res2->wSeasonState = eREFRESH;
	pChar->SendPacket(&packet2);

	if(pChar->GetDragonballScramble())
		m_mapPlayers.insert({pChar->GetCharID(), pChar->GetID()});
}

