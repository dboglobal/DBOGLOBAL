#include "stdafx.h"
#include "RankBattle.h"
#include "GameServer.h"
#include "TableContainerManager.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "NtlResultCode.h"
#include "GameMain.h"
#include "TriggerObject.h"
#include "DiceManager.h"



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CRankbattleBattle::CRankbattleBattle(ROOMID roomid, sRANKBATTLE_TBLDAT* pRankTblData)
{
	m_roomId = roomid;
	m_eRoomState = RANKBATTLE_ROOMSTATE_NONE;
	m_BattleState = RANKBATTLE_BATTLESTATE_NONE;
	m_byStage = 0;

	m_pRankTblData = pRankTblData;
	
	m_byOwnerCount = 0;
	m_byChallengerCount = 0;
	m_byTotalOwnerCount = 0;
	m_byTotalChallengerCount = 0;
	m_byInviteOwnerCount = 0;
	m_byInviteChallengerCount = 0;

	m_byOwnerWins = 0;
	m_byChallengerWins = 0;

	m_byUnavailableOwnerCount = 0;
	m_byUnavailableChallengerCount = 0;

	m_byOwnerKillScore = 0;
	m_byChallengerKillScore = 0;
	m_byTotalOwnerKillScore = 0;
	m_byTotalChallengerKillScore = 0;

	ZeroMemory(m_wchOwnerPartyName, NTL_MAX_SIZE_PARTY_NAME + 1);
	ZeroMemory(m_wchChallengerPartyName, NTL_MAX_SIZE_PARTY_NAME + 1);

	m_pWorld = NULL;

	m_dwWaitTime = m_pRankTblData->dwWaitTime * 1000;
	m_dwDirectionTime = m_pRankTblData->dwDirectionTime * 1000;
	m_dwMatchReadyTime = m_pRankTblData->dwMatchReadyTime * 1000;
	m_dwStageReadyTime = m_pRankTblData->dwStageReadyTime * 1000;
	m_dwStageRunTime = m_pRankTblData->dwStageRunTime * 1000;
	m_dwStageFinishTime = m_pRankTblData->dwStageFinishTime * 1000;
	m_dwMatchFinishTime = m_pRankTblData->dwMatchFinishTime * 1000;
	m_dwBossDirectionTime = m_pRankTblData->dwBossDirectionTime * 1000;
	m_dwBossKillTime = m_pRankTblData->dwBossKillTime * 1000;
	m_dwBossEndingTime = m_pRankTblData->dwBossEndingTime * 1000;
	m_dwEndTime = m_pRankTblData->dwEndTime * 1000;

	m_bEnd = false;
	m_bCancelMatch = false;

	//ERR_LOG(LOG_RANKBATTLE, "m_dwWaitTime %u, m_dwDirectionTime %u, m_dwMatchReadyTime %u, m_dwStageRunTime %u, m_dwStageFinishTime %u, m_dwMatchFinishTime %u, m_dwEndTime %u, m_dwBossDirectionTime %u, m_dwBossKillTime %u, m_dwBossEndingTime %u, m_dwStageReadyTime %u", 
	//	m_dwWaitTime, m_dwDirectionTime, m_dwMatchReadyTime, m_dwStageRunTime, m_dwStageFinishTime, m_dwMatchFinishTime, m_dwEndTime, m_dwBossDirectionTime, m_dwBossKillTime, m_dwBossEndingTime, m_dwStageReadyTime);
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CRankbattleBattle::~CRankbattleBattle()
{
	//printf("CRankbattleBattle::~CRankbattleBattle() \n");

	if (m_pWorld)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);
	}

	m_mapPlayers.clear();
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CRankbattleBattle::TickProcess(DWORD dwTickCount)
{
	if (m_bEnd)
	{
		m_dwEndTime = UnsignedSafeDecrease<DWORD>(m_dwEndTime, dwTickCount);

		if (m_dwEndTime == 0)
			return true;

		return false;
	}
		

	CGameServer* app = (CGameServer*)g_pApp;

	switch (m_eRoomState)
	{
		case RANKBATTLE_ROOMSTATE_NONE:
		{
			m_eRoomState = RANKBATTLE_ROOMSTATE_WAIT_MATCH_WORLD;
		}
		break;
		case RANKBATTLE_ROOMSTATE_WAIT_MATCH_WORLD: //create world
		{
			sWORLD_TBLDAT* pWorldTbldat = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pRankTblData->worldTblidx);
			m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld(pWorldTbldat);

			m_eRoomState = RANKBATTLE_ROOMSTATE_MATCH_READY;
		}
		break;
		case RANKBATTLE_ROOMSTATE_MATCH_READY: //send teleport request to everyone
		{
			m_eRoomState = RANKBATTLE_ROOMSTATE_WAIT;
			m_BattleState = RANKBATTLE_BATTLESTATE_WAIT;
		
			for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == it->second)
					{
						sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

						if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
						{
							pRankData->vResetLoc = m_pWorld->GetTbldat()->vStart1Loc;
							pRankData->vResetLoc.x += RandomRangeF(-5, 5);
							pRankData->vResetLoc.y += RandomRangeF(-3, 3);

							if (pPlayer->StartTeleportProposal(NULL, DBO_RANKBATTLE_USER_WAIT_TIME / 1000, TELEPORT_TYPE_RANKBATTLE, TELEPORT_TYPE_RANKBATTLE, m_pWorld->GetIdx(), m_pWorld->GetID(), pRankData->vResetLoc, m_pWorld->GetTbldat()->vStart1Dir))
							{
								++m_byTotalOwnerCount;
								++m_byInviteOwnerCount;
							}
						}
						else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
						{
							pRankData->vResetLoc = m_pWorld->GetTbldat()->vStart2Loc;
							pRankData->vResetLoc.x += RandomRangeF(-5, 5);
							pRankData->vResetLoc.y += RandomRangeF(-3, 3);

							if (pPlayer->StartTeleportProposal(NULL, DBO_RANKBATTLE_USER_WAIT_TIME / 1000, TELEPORT_TYPE_RANKBATTLE, TELEPORT_TYPE_RANKBATTLE, m_pWorld->GetIdx(), m_pWorld->GetID(), pRankData->vResetLoc, m_pWorld->GetTbldat()->vStart2Dir))
							{
								++m_byTotalChallengerCount;
								++m_byInviteChallengerCount;
							}
						}
					}
					else
					{
						ERR_LOG(LOG_RANKBATTLE, "Player in rank does not match character id %u != %u.", pPlayer->GetCharID(), it->second);
					}
				}
				else
				{
					ERR_LOG(LOG_RANKBATTLE, "Can not send teleport request to player %u because he is offline", it->second);
				}
			}
		}
		break;
		case RANKBATTLE_ROOMSTATE_WAIT: //wait..
		{
			CheckBattleBegin(dwTickCount); //wait 2 minutes and check if owner and challenger inside. If not then cancel ranked..
		}
		break;
		case RANKBATTLE_ROOMSTATE_MATCH_RUN: //battle in progress
		{
			switch (m_BattleState)
			{
				case RANKBATTLE_BATTLESTATE_DIRECTION: //used in the beginning when all players inside. Shows who fight vs who
				{
					m_dwDirectionTime = UnsignedSafeDecrease<DWORD>(m_dwDirectionTime, dwTickCount);

					if (m_dwDirectionTime == 0)
					{
						if (IsOneTeamOut()) //check if a whole team left match
						{
							m_bEnd = true;
							CancelMatch();

							return false;
						}

						UpdateBattleStateNfy(RANKBATTLE_BATTLESTATE_STAGE_PREPARE, 0);
					}
				}
				break;
				case RANKBATTLE_BATTLESTATE_STAGE_PREPARE: //player has few seconds to prepade (do eq etc)
				{
					m_dwStageReadyTime = UnsignedSafeDecrease<DWORD>(m_dwStageReadyTime, dwTickCount);

					if (m_dwStageReadyTime == 0)
					{
						if (IsOneTeamOut()) //check if a whole team left match
						{
							m_bEnd = true;
							CancelMatch();

							return false;
						}

						ResetPlayers();

						m_dwMatchReadyTime = m_pRankTblData->dwMatchReadyTime * 1000;
						UpdateBattleStateNfy(RANKBATTLE_BATTLESTATE_STAGE_READY, m_byStage);
					}
				}
				break;
				case RANKBATTLE_BATTLESTATE_STAGE_READY: //round X wait
				{
					m_dwMatchReadyTime = UnsignedSafeDecrease<DWORD>(m_dwMatchReadyTime, dwTickCount);

					if (m_dwMatchReadyTime == 0)
					{
						if (IsOneTeamOut()) //check if a whole team left match. If true then finish match.
						{
							DecideMatchWinner();
							return false;
						}

						UpdatePlayersAttackable();

						m_dwStageRunTime = m_pRankTblData->dwStageRunTime * 1000;
						UpdateBattleStateNfy(RANKBATTLE_BATTLESTATE_STAGE_RUN, m_byStage);
					}
				}
				break;
				case RANKBATTLE_BATTLESTATE_STAGE_RUN: // show fight swf and progress fight
				{
					m_dwStageRunTime = UnsignedSafeDecrease<DWORD>(m_dwStageRunTime, dwTickCount);

					if (m_dwStageRunTime == 0) //battle time end
					{
						TimeOut();
					}
					else
					{
						CheckArena(); //check if someone left arena and set state
						CheckStage(); //check if a players left, set state and decide winner
					}
				}
				break;
				case RANKBATTLE_BATTLESTATE_STAGE_FINISH: //show round winner
				{
					m_dwStageFinishTime = UnsignedSafeDecrease<DWORD>(m_dwStageFinishTime, dwTickCount);

					if (m_dwStageFinishTime == 0)
					{
						ResetPlayers();

						if (m_bCancelMatch == false)
						{
							//printf("m_byOwnerWins %u m_byChallengerWins %u\n", m_byOwnerWins, m_byChallengerWins);
							if (m_pRankTblData->byBattleMode == RANKBATTLE_MODE_INDIVIDUAL)
							{
								if (m_byOwnerWins == 2 || m_byChallengerWins == 2)
								{
									DecideMatchWinner();
									return false;
								}
							}
							else if (m_pRankTblData->byBattleMode == RANKBATTLE_MODE_PARTY)
							{
								if (m_byOwnerWins == 3 || m_byChallengerWins == 3)
								{
									DecideMatchWinner();
									return false;
								}
							}

							if (m_byStage < m_pRankTblData->byBattleCount)
							{
								m_dwMatchReadyTime = m_pRankTblData->dwMatchReadyTime * 1000;
								UpdateBattleStateNfy(RANKBATTLE_BATTLESTATE_STAGE_READY, m_byStage);
							}
							else
							{
								DecideMatchWinner();
							}
						}
						else
						{
							DecideMatchWinner();
						}
					}
				}
				break;
				case RANKBATTLE_BATTLESTATE_MATCH_FINISH: //shows the winners etc
				{
					m_dwMatchFinishTime = UnsignedSafeDecrease<DWORD>(m_dwMatchFinishTime, dwTickCount);

					if (m_dwMatchFinishTime == 0)
					{
						UpdateBattleStateNfy(RANKBATTLE_BATTLESTATE_END, m_byStage);
					}
				}
				break;

				case RANKBATTLE_BATTLESTATE_END:
				{
					TeleportAllOut();
					m_eRoomState = RANKBATTLE_ROOMSTATE_MATCH_END;
				}
				break;
			}
		}
		break;
		case RANKBATTLE_ROOMSTATE_MATCH_END:
		{
			m_bEnd = true;
		}
		break;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattleBattle::UpdateBattleStateNfy(eRANKBATTLE_BATTLESTATE eBattleState, BYTE byStage)
{
	m_BattleState = eBattleState;
	m_byStage = byStage;

	CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY));
	sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY * res = (sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY;
	res->byBattleState = eBattleState;
	res->byStage = byStage;
	packet.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY));
	m_pWorld->Broadcast(&packet);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattleBattle::AddPlayer(CPlayer * pPlayer, eRANKBATTLE_TEAM_TYPE eTeamType)
{
	sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();
	pRankData->eTeamType = eTeamType;

	m_mapPlayers.insert({ pPlayer->GetID(), pPlayer->GetCharID() });

	pPlayer->SetRankBattleRoomId(m_roomId);
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattleBattle::AddParty(CParty * pParty, eRANKBATTLE_TEAM_TYPE eTeamType)
{
	//printf("CRankbattleBattle::AddParty: PartyID %u, MemberCount %u, TeamType %u \n", pParty->GetPartyID(), pParty->GetRankBattleMemberCount(), eTeamType);
	for (BYTE i = 0; i < pParty->GetRankBattleMemberCount(); i++)
	{
		CPlayer* pPlayer = pParty->GetRankBattlePlayer(i);
		if (pPlayer)
		{
			AddPlayer(pPlayer, eTeamType);

			if (eTeamType == RANKBATTLE_TEAM_OWNER)
			{
				NTL_SAFE_WCSCPY(m_wchOwnerPartyName, pParty->GetPartyName());
			}	
			else if (eTeamType == RANKBATTLE_TEAM_CHALLENGER)
			{
				NTL_SAFE_WCSCPY(m_wchChallengerPartyName, pParty->GetPartyName());
			}
		}
	}

	pParty->SetRankBattleRoomID(m_roomId);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattleBattle::EnterBattle(CPlayer* pPlayer)
{
	if (m_byStage > 0 || m_bEnd || m_bCancelMatch) //check if battle already running or already end
	{
		ERR_LOG(LOG_RANKBATTLE, "Player %u tried to enter battle while it has already started/ended or canceled. m_byStage = %u, m_bEnd = %i, m_bCancelMatch = %i", pPlayer->GetCharID(), m_byStage, m_bEnd, m_bCancelMatch);
		return;
	}

	std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.find(pPlayer->GetID());
	if(it != m_mapPlayers.end())
	{
		if (pPlayer->GetCharID() == it->second)
		{
			sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

			if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
			{
				++m_byOwnerCount;
				--m_byInviteOwnerCount;
			}
			else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
			{
				++m_byChallengerCount;
				--m_byInviteChallengerCount;
			}

			pRankData->eState = RANKBATTLE_MEMBER_STATE_NORMAL;

			CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY));
			sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY * res = (sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY *)packet.GetPacketData();
			res->wOpCode = GU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY;
			res->byBattleState = RANKBATTLE_BATTLESTATE_WAIT;
			res->byStage = 0;
			packet.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY));
			pPlayer->SendPacket(&packet);
		}
		else
		{
			ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
		}
	}
	else
	{
		ERR_LOG(LOG_RANKBATTLE, "Player handle %u not found", pPlayer->GetID());
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattleBattle::Cancel(CPlayer* pPlayer)
{
	std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.find(pPlayer->GetID());
	if (it != m_mapPlayers.end())
	{
		if (pPlayer->GetCharID() == it->second)
		{
			sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

			m_mapPlayers.erase(it); //erase player from the map

			if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
			{
				if (pPlayer->GetParty())
					pPlayer->GetParty()->PlayerCancelRankTeleport(pPlayer->GetID());

				--m_byInviteOwnerCount;
				if (m_byInviteOwnerCount == 0 && m_byOwnerCount == 0) //check if everyone declined the invite and none of the team is anymore inside
				{
					m_bEnd = true;

					CancelMatch();
				}
			}
			else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
			{
				if (pPlayer->GetParty())
					pPlayer->GetParty()->PlayerCancelRankTeleport(pPlayer->GetID());

				--m_byInviteChallengerCount;
				if (m_byInviteChallengerCount == 0 && m_byChallengerCount == 0) //check if everyone declined the invite and none of the team is anymore inside
				{
					m_bEnd = true;

					CancelMatch();
				}
			}

			// init players rank data
			pRankData->Init();
		}
		else
		{
			ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
		}
	}
	else
	{
		ERR_LOG(LOG_RANKBATTLE, "Player handle %u not found", pPlayer->GetID());
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattleBattle::CheckBattleBegin(DWORD dwTickCount)
{
	m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickCount);

	if (m_dwWaitTime == 0)
	{
		if (m_bCancelMatch) //do we cancel the match?
		{
			m_bEnd = true;
			CancelMatch();
			return;
		}
		else
		{
			//check if at least one owner and one challenger inside. Then begin. Otherwise cancel
			if (m_byOwnerCount > 0 && m_byChallengerCount > 0)
			{
				m_eRoomState = RANKBATTLE_ROOMSTATE_MATCH_RUN;
			}
			else
			{
				//Fail battle. One team is not inside within time.. Teleport everyone out
				m_bEnd = true;
				CancelMatch();
				return;
			}
		}
	}
	else //check if all already inside or a team declined
	{
		if (m_byOwnerCount > 0 && m_byInviteOwnerCount == 0 && m_byChallengerCount > 0 && m_byInviteChallengerCount == 0) //check if at least one member of each team is inside and has no more teleport invite
		{
			m_eRoomState = RANKBATTLE_ROOMSTATE_MATCH_RUN;
		}
	}

	if (m_eRoomState == RANKBATTLE_ROOMSTATE_MATCH_RUN) //begin battle
	{
		SendTeamInfo();
		UpdateBattleStateNfy(RANKBATTLE_BATTLESTATE_DIRECTION, 0);

		CNtlPacket packet(sizeof(sGU_RANKBATTLE_MATCH_START_NFY));
		sGU_RANKBATTLE_MATCH_START_NFY * res = (sGU_RANKBATTLE_MATCH_START_NFY *)packet.GetPacketData();
		res->wOpCode = GU_RANKBATTLE_MATCH_START_NFY;
		res->wResultCode = GAME_SUCCESS;
		packet.SetPacketLen(sizeof(sGU_RANKBATTLE_MATCH_START_NFY));
		m_pWorld->Broadcast(&packet);
	}
}

void CRankbattleBattle::SendTeamInfo()
{
	WORD wOwnerPartyName = INVALID_WORD;
	WORD wChallengerPartyName = INVALID_WORD;
	sRANKBATTLE_MATCH_MEMBER_INFO memberInfo[NTL_MAX_MEMBER_IN_PARTY * 2];

	sVARIABLE_DATA sData;
	sData.Init(512);

	BYTE byCount = 0;
	for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == it->second)
			{
				sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

				if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER || pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
				{

					memberInfo[byCount].hPc = it->first;
					memberInfo[byCount].byState = pRankData->eState;
					memberInfo[byCount++].byTeam = pRankData->eTeamType;
				}
			}
			else
			{
				ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
			}
		}
		else
		{
			ERR_LOG(LOG_RANKBATTLE, "Cant send team info from player %u because he is offline", it->second);
		}
	}

	WORD wMemberInfo = sData.Write(byCount * sizeof(sRANKBATTLE_MATCH_MEMBER_INFO), memberInfo);

	if (m_pRankTblData->byBattleMode == RANKBATTLE_MODE_PARTY)
	{
		wOwnerPartyName = sData.Write(sizeof(m_wchOwnerPartyName), m_wchOwnerPartyName);
		wChallengerPartyName = sData.Write(sizeof(m_wchChallengerPartyName), m_wchChallengerPartyName);
	}

	WORD wPacketSize = sData.GetPacketSize(sizeof(sGU_RANKBATTLE_BATTLE_TEAM_INFO_NFY));

	ERR_LOG(LOG_RANKBATTLE, "<BATTLE-RANK>byCount %u wMemberInfo %u wOwnerPartyName %u wChallengerPartyName %u wPacketSize %u sData.GetReserveSize() %u sData.GetDataSize() %u sData.GetTotalSize() %u",
		byCount, wMemberInfo, wOwnerPartyName, wChallengerPartyName, wPacketSize, sData.GetReserveSize(), sData.GetDataSize(), sData.GetTotalSize());

	CNtlPacket packet(wPacketSize);
	sGU_RANKBATTLE_BATTLE_TEAM_INFO_NFY * res = (sGU_RANKBATTLE_BATTLE_TEAM_INFO_NFY *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_BATTLE_TEAM_INFO_NFY;
	res->wStraightWinCount = 0;
	res->wStraightKOWinCount = 0;
	res->byCount = byCount;
	res->wMemberInfo = wMemberInfo;
	res->wOwnerPartyName = wOwnerPartyName;
	res->wChallengerPartyName = wChallengerPartyName;
	sData.CopyTo(&res->sData, sData.GetDataSize());
	m_pWorld->Broadcast(&packet);
}

void CRankbattleBattle::UpdatePlayerState(CPlayer* pPlayer, BYTE byState)
{
	if (m_BattleState != RANKBATTLE_BATTLESTATE_STAGE_RUN) // dont allow update state while match is not in progress
	{
		ERR_LOG(LOG_RANKBATTLE, "Could not update character %u state to %u because m_BattleState(%u) != RANKBATTLE_BATTLESTATE_STAGE_RUN", pPlayer->GetCharID(), byState, m_BattleState);
		return;
	}

	std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.find(pPlayer->GetID());
	if (it != m_mapPlayers.end())
	{
		if (pPlayer->GetCharID() == it->second)
		{
			sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

			switch (byState)
			{
				case RANKBATTLE_MEMBER_STATE_ATTACKABLE: //used for when player gets revived
				{
					//decrease unavailable count
					if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
						--m_byUnavailableOwnerCount; 
					else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
						--m_byUnavailableChallengerCount;
				}
				break;

				case RANKBATTLE_MEMBER_STATE_FAINT:
				{
					if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER) //if blue team dies
					{
						++m_byChallengerKillScore; //increase kill score for red team
						++m_byUnavailableOwnerCount; //increase owner unavailable counter
					}
					else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
					{
						++m_byOwnerKillScore; //increase kill score for blue team
						++m_byUnavailableChallengerCount; //increase challenger unavailable counter
					}

					CNtlPacket packet2(sizeof(sGU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY));
					sGU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY * res2 = (sGU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY;
					res2->byChallengerKillScore = m_byChallengerKillScore;
					res2->byOwnerKillScore = m_byOwnerKillScore;
					packet2.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY));
					m_pWorld->Broadcast(&packet2);

					//check if there is a winner
					CheckStageWinner();
				}
				break;

				default: break;
			}

			pRankData->eState = (eRANKBATTLE_MEMBER_STATE)byState;

			CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
			sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY * res = (sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY *)packet.GetPacketData();
			res->wOpCode = GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY;
			res->hPc = pPlayer->GetID();
			res->byPCState = byState;
			packet.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
			m_pWorld->Broadcast(&packet);
		}
		else
		{
			ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
		}
	}
	else
	{
		ERR_LOG(LOG_RANKBATTLE, "Player handle %u not found", pPlayer->GetID());
	}
}

void CRankbattleBattle::ResetPlayers()
{
	for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == it->second)
			{
				sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

				pPlayer->ChangeAttackProgress(false);
				pPlayer->SetAttackTarget(INVALID_HOBJECT);

				if (pRankData->eState != RANKBATTLE_MEMBER_STATE_ATTACKABLE) //check: should we really only send if player is not attack able state?
				{
					CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
					sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY * res = (sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY *)packet.GetPacketData();
					res->wOpCode = GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY;
					res->hPc = it->first;
					res->byPCState = RANKBATTLE_MEMBER_STATE_NORMAL;
					packet.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
					m_pWorld->Broadcast(&packet);
				}

				//remove buffs
				pPlayer->GetBuffManager()->RemoveAllBuff();

				//if transformed then cancel transformation
				if (pPlayer->GetTransformationTbldat())
				{
					pPlayer->CancelTransformation();
				}

				if (pPlayer->GetStateManager()->GetConditionState() > 0)
				{
					pPlayer->GetStateManager()->RemoveConditionFlags(INVALID_QWORD, true);
				}

				//re-calculate all stats
				pPlayer->GetCharAtt()->CalculateAll();

				//restore lp/rp/rp/rp balls
				pPlayer->UpdateCurLP(pPlayer->GetMaxLP(), true, false);
				pPlayer->UpdateCurEP(pPlayer->GetMaxEP(), true, false);
				pPlayer->UpdateRpBall(pPlayer->GetMaxRPBall(), false, false);
				pPlayer->UpdateCurRP(pPlayer->GetCharAtt()->GetMaxRP(), false, false);

				//remove target 
				pPlayer->ChangeTarget(INVALID_HOBJECT);

				//clear cc reduction
				pPlayer->ClearCrowdControlReduction();

				pRankData->eState = RANKBATTLE_MEMBER_STATE_NORMAL;

				if (m_byStage > 0)
				{
					CNtlVector vNewLoc;

					if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
					{
						vNewLoc = pRankData->vResetLoc;
						pPlayer->SetCurDir(m_pWorld->GetTbldat()->vStart1Dir);
					}
					else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
					{
						vNewLoc = pRankData->vResetLoc;
						pPlayer->SetCurDir(m_pWorld->GetTbldat()->vStart2Dir);
					}

					pPlayer->SetIsReviving(false);

					pPlayer->StartTeleport(vNewLoc, pPlayer->GetCurDir(), m_pWorld->GetID(), TELEPORT_TYPE_RANKBATTLE);
				}
			}
			else
			{
				ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
			}
		}
		else
		{
			ERR_LOG(LOG_RANKBATTLE, "Cant reset player %u because he is offline", it->second);
		}
	}

	//reset
	m_byUnavailableOwnerCount = 0;
	m_byUnavailableChallengerCount = 0;

	m_byTotalOwnerKillScore += m_byOwnerKillScore;
	m_byTotalChallengerKillScore += m_byChallengerKillScore;
	m_byOwnerKillScore = 0;
	m_byChallengerKillScore = 0;
}


void CRankbattleBattle::UpdatePlayersAttackable()
{
	for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == it->second)
			{
				sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

				pRankData->eState = RANKBATTLE_MEMBER_STATE_ATTACKABLE;

				pPlayer->SendCharStateStanding(); //set player standing state. This is needed to fix that players are not able to use any skills until they move after first round.

				CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
				sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY * res = (sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY *)packet.GetPacketData();
				res->wOpCode = GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY;
				res->hPc = it->first;
				res->byPCState = RANKBATTLE_MEMBER_STATE_ATTACKABLE;
				packet.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
				m_pWorld->Broadcast(&packet);

			}
			else
			{
				ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
			}
		}
		else
		{
			ERR_LOG(LOG_RANKBATTLE, "Cant set player %u attack-able because he is offline", it->second);
		}
	}
}


void CRankbattleBattle::CheckArena()
{
	for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == it->second)
			{
				sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

				if (pRankData->eState != RANKBATTLE_MEMBER_STATE_ATTACKABLE || pRankData->eTeamType == RANKBATTLE_TEAM_OTHER)
					continue;

				if (pPlayer->GetCurLoc().x < m_pWorld->GetTbldat()->vBattleStartLoc.x && pPlayer->GetCurLoc().z < m_pWorld->GetTbldat()->vBattleStartLoc.z
					&& pPlayer->GetCurLoc().x > m_pWorld->GetTbldat()->vBattleEndLoc.x && pPlayer->GetCurLoc().z > m_pWorld->GetTbldat()->vBattleEndLoc.z) //check if inside arena
					continue;

				pRankData->eState = RANKBATTLE_MEMBER_STATE_OUTOFAREA;

				CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
				sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY * res = (sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY *)packet.GetPacketData();
				res->wOpCode = GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY;
				res->hPc = it->first;
				res->byPCState = RANKBATTLE_MEMBER_STATE_OUTOFAREA;
				packet.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
				m_pWorld->Broadcast(&packet);

				if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
					++m_byUnavailableOwnerCount;
				else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
					++m_byUnavailableChallengerCount;

				//check if there is a winner
				if (CheckStageWinner() == true)
					return; //return if there is a winner
			}
			else
			{
				ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
			}
		}
		else
		{
			ERR_LOG(LOG_RANKBATTLE, "Cant check player %u because he is offline", it->second);
		}
	}
}

void CRankbattleBattle::CheckStage()
{
	if (IsEveryoneOut())
	{
		m_bEnd = true;
		return;
	}

	if (m_byOwnerCount == 0)
	{
		m_bCancelMatch = true;
		UpdateStageWinner(RANKBATTLE_MATCH_WIN_CHALLENGER);
	}
	else if (m_byChallengerCount == 0)
	{
		m_bCancelMatch = true;
		UpdateStageWinner(RANKBATTLE_MATCH_WIN_OWNER);
	}
}

void CRankbattleBattle::TimeOut()
{
	if (m_pRankTblData->byBattleMode == RANKBATTLE_MODE_INDIVIDUAL)
	{
		float fTotalLifePercentOwner = 0;
		float fTotalLifePercentChallenger = 0;

		CPlayer* pPlayerOwner = NULL;
		CPlayer* pPlayerChallenger = NULL;

		for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetCharID() == it->second)
				{
					sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

					if (pRankData->eState == RANKBATTLE_MEMBER_STATE_ATTACKABLE)
					{
						if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
						{
							fTotalLifePercentOwner += pPlayer->GetCurLpInPercent();
							pPlayerOwner = pPlayer;
						}
						else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
						{
							fTotalLifePercentChallenger += pPlayer->GetCurLpInPercent();
							pPlayerChallenger = pPlayer;
						}
					}
				}
				else
				{
					ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
				}
			}
		}

		if (fTotalLifePercentOwner > fTotalLifePercentChallenger)
			UpdateStageWinner(RANKBATTLE_MATCH_WIN_OWNER);
		else if (fTotalLifePercentChallenger > fTotalLifePercentOwner)
			UpdateStageWinner(RANKBATTLE_MATCH_WIN_CHALLENGER);
		else
		{
			CDiceManager dice;

			fTotalLifePercentOwner = (float)dice.RollDice();
			fTotalLifePercentChallenger = (float)dice.RollDice();

			if (pPlayerOwner)
			{
				CNtlPacket packet2(sizeof(sGU_DICE_ROLLED_NFY));
				sGU_DICE_ROLLED_NFY* res2 = (sGU_DICE_ROLLED_NFY*)packet2.GetPacketData();
				res2->wOpCode = GU_DICE_ROLLED_NFY;
				res2->hSubject = pPlayerOwner->GetID();
				res2->wDiceResult = (WORD)fTotalLifePercentOwner;
				packet2.SetPacketLen(sizeof(sGU_DICE_ROLLED_NFY));
				pPlayerOwner->Broadcast(&packet2);
			}

			if (pPlayerChallenger)
			{
				CNtlPacket packet4(sizeof(sGU_DICE_ROLLED_NFY));
				sGU_DICE_ROLLED_NFY* res4 = (sGU_DICE_ROLLED_NFY*)packet4.GetPacketData();
				res4->wOpCode = GU_DICE_ROLLED_NFY;
				res4->hSubject = pPlayerChallenger->GetID();
				res4->wDiceResult = (WORD)fTotalLifePercentChallenger;
				packet4.SetPacketLen(sizeof(sGU_DICE_ROLLED_NFY));
				pPlayerChallenger->Broadcast(&packet4);
			}

			if (fTotalLifePercentOwner > fTotalLifePercentChallenger)
				UpdateStageWinner(RANKBATTLE_MATCH_WIN_OWNER);
			else if (fTotalLifePercentChallenger > fTotalLifePercentOwner)
				UpdateStageWinner(RANKBATTLE_MATCH_WIN_CHALLENGER);
			else
				UpdateStageWinner(RANKBATTLE_MATCH_DRAW);
		}
	}
	else if (m_pRankTblData->byBattleMode == RANKBATTLE_MODE_PARTY)
	{
		int nOwnerCount = 0;
		int nChallengerCount = 0;

		for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
			if (pPlayer)
			{
				if (pPlayer->GetCharID() == it->second)
				{
					sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

					if (pRankData->eState == RANKBATTLE_MEMBER_STATE_ATTACKABLE)
					{
						if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
							++nOwnerCount;
						else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
							++nChallengerCount;
					}
				}
				else
				{
					ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
				}
			}
		}

		if(nOwnerCount > nChallengerCount)
			UpdateStageWinner(RANKBATTLE_MATCH_WIN_OWNER);
		else if (nChallengerCount > nOwnerCount)
			UpdateStageWinner(RANKBATTLE_MATCH_WIN_CHALLENGER);
		else if(nChallengerCount > 0 && nOwnerCount > 0)
		{
			float fTotalLifePercentOwner = 0;
			float fTotalLifePercentChallenger = 0;

			CPlayer* pPlayerOwner = NULL;
			CPlayer* pPlayerChallenger = NULL;

			for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
			{
				CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
				if (pPlayer)
				{
					if (pPlayer->GetCharID() == it->second)
					{
						sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

						if (pRankData->eState == RANKBATTLE_MEMBER_STATE_ATTACKABLE)
						{
							if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
							{
								if (pPlayer->GetParty())
								{
									if (pPlayer->GetParty()->GetPartyLeaderID() == pPlayer->GetID())
										pPlayerOwner = pPlayer;
								}

								fTotalLifePercentOwner += pPlayer->GetCurLpInPercent();
							}
							else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
							{
								if (pPlayer->GetParty())
								{
									if (pPlayer->GetParty()->GetPartyLeaderID() == pPlayer->GetID())
										pPlayerChallenger = pPlayer;
								}

								fTotalLifePercentChallenger += pPlayer->GetCurLpInPercent();
							}
						}
					}
					else
					{
						ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
					}
				}
			}

			if (fTotalLifePercentOwner > fTotalLifePercentChallenger)
				UpdateStageWinner(RANKBATTLE_MATCH_WIN_OWNER);
			else if (fTotalLifePercentChallenger > fTotalLifePercentOwner)
				UpdateStageWinner(RANKBATTLE_MATCH_WIN_CHALLENGER);
			else
			{
				CDiceManager dice;

				fTotalLifePercentOwner = (float)dice.RollDice();
				fTotalLifePercentChallenger = (float)dice.RollDice();

				if (pPlayerOwner)
				{
					CNtlPacket packet2(sizeof(sGU_DICE_ROLLED_NFY));
					sGU_DICE_ROLLED_NFY* res2 = (sGU_DICE_ROLLED_NFY*)packet2.GetPacketData();
					res2->wOpCode = GU_DICE_ROLLED_NFY;
					res2->hSubject = pPlayerOwner->GetID();
					res2->wDiceResult = (WORD)fTotalLifePercentOwner;
					packet2.SetPacketLen(sizeof(sGU_DICE_ROLLED_NFY));
					pPlayerOwner->Broadcast(&packet2, pPlayerOwner);
				}

				if (pPlayerChallenger)
				{
					CNtlPacket packet4(sizeof(sGU_DICE_ROLLED_NFY));
					sGU_DICE_ROLLED_NFY* res4 = (sGU_DICE_ROLLED_NFY*)packet4.GetPacketData();
					res4->wOpCode = GU_DICE_ROLLED_NFY;
					res4->hSubject = pPlayerChallenger->GetID();
					res4->wDiceResult = (WORD)fTotalLifePercentChallenger;
					packet4.SetPacketLen(sizeof(sGU_DICE_ROLLED_NFY));
					pPlayerChallenger->Broadcast(&packet4, pPlayerChallenger);
				}

				if (fTotalLifePercentOwner > fTotalLifePercentChallenger)
					UpdateStageWinner(RANKBATTLE_MATCH_WIN_OWNER);
				else if (fTotalLifePercentChallenger > fTotalLifePercentOwner)
					UpdateStageWinner(RANKBATTLE_MATCH_WIN_CHALLENGER);
				else
					UpdateStageWinner(RANKBATTLE_MATCH_DRAW);
			}
		}
		else
		{
			UpdateStageWinner(RANKBATTLE_MATCH_DRAW);
		}
	}
}

void CRankbattleBattle::UpdateStageWinner(eRANKBATTLE_MATCH_RESULT eMatchResult)
{
	++m_byStage;

	if (eMatchResult == RANKBATTLE_MATCH_WIN_OWNER)
		++m_byOwnerWins;
	else if(eMatchResult == RANKBATTLE_MATCH_WIN_CHALLENGER)
		++m_byChallengerWins;


	CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY));
	sGU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY * res = (sGU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY;
	res->byBattleResult = eMatchResult;
	res->sStageScore.byOwner = m_byOwnerWins;
	res->sStageScore.byChallenger = m_byChallengerWins;
	packet.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY));
	m_pWorld->Broadcast(&packet);


	m_dwStageFinishTime = m_pRankTblData->dwStageFinishTime * 1000;
	UpdateBattleStateNfy(RANKBATTLE_BATTLESTATE_STAGE_FINISH, m_byStage);
}

void CRankbattleBattle::DecideMatchWinner()
{
	eRANKBATTLE_MATCH_RESULT eResult = RANKBATTLE_MATCH_DRAW;

	if (m_byOwnerWins > m_byChallengerWins)
		eResult = RANKBATTLE_MATCH_WIN_OWNER;
	else if (m_byChallengerWins > m_byOwnerWins)
		eResult = RANKBATTLE_MATCH_WIN_CHALLENGER;

	//printf("winner team %u blue-team kills %u, red-team kills %u \n", eResult, m_byTotalOwnerKillScore, m_byTotalChallengerKillScore);

	//send packet and update player
	for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == it->second)
			{
				sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

				bool bKoWin = m_byOwnerWins == 0 || m_byChallengerWins == 0;
				float fRankPoints = (float)pPlayer->GetLevel() * 4.95f;

				if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
				{
					CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY));
					sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY * res = (sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY *)packet.GetPacketData();
					res->wOpCode = GU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY;
					res->sMatchResult.bPerfectWin = bKoWin;
					res->sMatchResult.byMatchRating = 0;
					res->sMatchResult.byMatchWinner = eResult;
					res->sMatchResult.sScore.byOwner = m_byOwnerWins;
					res->sMatchResult.sScore.byChallenger = m_byChallengerWins;

					if (m_byOwnerWins > 0)
						fRankPoints *= abs((int)m_byOwnerWins - (int)m_byChallengerWins);
					else
						fRankPoints = 0.0f;

					if (eResult != RANKBATTLE_MATCH_WIN_OWNER)
						fRankPoints *= 0.10f;

					res->sMatchResult.dwMudosaPoint = (DWORD)fRankPoints;

					pPlayer->UpdateRankBattleScore(eResult == RANKBATTLE_MATCH_WIN_OWNER ? 1 : 0, eResult == RANKBATTLE_MATCH_DRAW ? 1 : 0, eResult == RANKBATTLE_MATCH_WIN_CHALLENGER ? 1 : 0, res->sMatchResult.dwMudosaPoint, fRankPoints, eResult == RANKBATTLE_MATCH_WIN_OWNER ? bKoWin : false, m_pRankTblData->byBattleMode);

					res->sMatchResult.fRankPoint = fRankPoints;
					pPlayer->SendPacket(&packet);
				}
				else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
				{
					CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY));
					sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY * res = (sGU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY *)packet.GetPacketData();
					res->wOpCode = GU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY;
					res->sMatchResult.bPerfectWin = bKoWin;
					res->sMatchResult.byMatchRating = 0;
					res->sMatchResult.byMatchWinner = eResult;
					res->sMatchResult.sScore.byOwner = m_byOwnerWins;
					res->sMatchResult.sScore.byChallenger = m_byChallengerWins;

					if (m_byChallengerWins > 0)
						fRankPoints *= abs((int)m_byChallengerWins - (int)m_byOwnerWins);
					else
						fRankPoints = 0.0f;

					if (eResult != RANKBATTLE_MATCH_WIN_CHALLENGER)
						fRankPoints *= 0.10f;

					res->sMatchResult.dwMudosaPoint = (DWORD)fRankPoints;

					pPlayer->UpdateRankBattleScore(eResult == RANKBATTLE_MATCH_WIN_CHALLENGER ? 1 : 0, eResult == RANKBATTLE_MATCH_DRAW ? 1 : 0, eResult == RANKBATTLE_MATCH_WIN_OWNER ? 1 : 0, res->sMatchResult.dwMudosaPoint, fRankPoints, eResult == RANKBATTLE_MATCH_WIN_CHALLENGER ? bKoWin : false, m_pRankTblData->byBattleMode);

					res->sMatchResult.fRankPoint = fRankPoints;
					pPlayer->SendPacket(&packet);
				}
			}
			else
			{
				ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
			}
		}
	}

	m_dwMatchFinishTime = m_pRankTblData->dwMatchFinishTime * 1000;
	UpdateBattleStateNfy(RANKBATTLE_BATTLESTATE_MATCH_FINISH, m_byStage);
}


void CRankbattleBattle::TeleportAllOut()
{
	CGameServer* app = (CGameServer*)g_pApp;

	for (std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.begin(); it != m_mapPlayers.end(); it++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(it->first);
		if (pPlayer)
		{
			if (pPlayer->GetCharID() == it->second)
			{
				sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

				CNtlPacket packet(sizeof(sGU_RANKBATTLE_LEAVE_NFY));
				sGU_RANKBATTLE_LEAVE_NFY * res = (sGU_RANKBATTLE_LEAVE_NFY *)packet.GetPacketData();
				res->wOpCode = GU_RANKBATTLE_LEAVE_NFY;
				packet.SetPacketLen(sizeof(sGU_RANKBATTLE_LEAVE_NFY));
				pPlayer->SendPacket(&packet);

				//put this on top to avoid issues when cancel teleport or teleporting
				pPlayer->SetRankBattleRegisterObject(INVALID_HOBJECT);
				pPlayer->SetRankBattleRoomTblidx(INVALID_TBLIDX);
				pPlayer->SetRankBattleRoomId(INVALID_ROOMID);

				if (pPlayer->GetParty())
					pPlayer->GetParty()->ResetRankBattleInfo();

				if (pPlayer->GetWorldID() == m_pWorld->GetID()) // check if player is in ranked world.. Otherwise dont teleport coz it might be that player is still in normal world and wait for user to accept
				{
					if (CWorld* pWorld = app->GetGameMain()->GetWorldManager()->FindWorld(pPlayer->GetBeforeTeleportWorldID()))
					{
						pPlayer->StartTeleport(pPlayer->GetBeforeTeleportLoc(), pPlayer->GetCurDir(), pPlayer->GetBeforeTeleportWorldID(), TELEPORT_TYPE_RANKBATTLE);
					}
					else
					{
						pPlayer->StartTeleport(CNtlVector(pPlayer->GetBindLoc()), pPlayer->GetCurDir(), pPlayer->GetBindWorldID(), TELEPORT_TYPE_RANKBATTLE);
					}
				}
				else // else cancel teleport
				{
					pPlayer->CancelTeleportProposal(TELEPORT_TYPE_RANKBATTLE);
				}

				pRankData->Init();
			}
			else
			{
				ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
			}
		}
		else
		{
			ERR_LOG(LOG_RANKBATTLE, "Cant teleport player %u out because he is offline", it->second);
		}
	}
}

bool CRankbattleBattle::CheckStageWinner()
{
	if (m_byUnavailableOwnerCount >= m_byOwnerCount) //check if all players from team blue are unavailable
	{
		UpdateStageWinner(RANKBATTLE_MATCH_WIN_CHALLENGER);
		return true;
	}
	else if (m_byUnavailableChallengerCount >= m_byChallengerCount) //check if all players from team red are unavailable
	{
		UpdateStageWinner(RANKBATTLE_MATCH_WIN_OWNER);
		return true;
	}

	return false;
}


void CRankbattleBattle::LeaveBattle(CPlayer * pPlayer)
{
	if (m_eRoomState == RANKBATTLE_ROOMSTATE_MATCH_RUN) //when player leaves ranked during battle
	{
		std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.find(pPlayer->GetID());
		if (it != m_mapPlayers.end())
		{
			if (pPlayer->GetCharID() == it->second)
			{
				sRANK_BATTLE_DATA* pRankData = pPlayer->GetRankBattleData();

				CNtlPacket packet(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
				sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY * res = (sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY *)packet.GetPacketData();
				res->wOpCode = GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY;
				res->hPc = pPlayer->GetID();
				res->byPCState = RANKBATTLE_MEMBER_STATE_GIVEUP;
				packet.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY));
				m_pWorld->Broadcast(&packet, pPlayer);

				m_mapPlayers.erase(it);

				if (pRankData->eTeamType == RANKBATTLE_TEAM_OWNER)
				{
					--m_byOwnerCount;
					if (m_byOwnerCount == 0 && m_byInviteOwnerCount == 0) //check if everyone left arena and if none left who still has tp invite to arena
					{
						if (m_eRoomState == RANKBATTLE_ROOMSTATE_WAIT && m_byStage == 0) //if match didnt start yet then send cancel
							CancelMatch();
						else
							m_bCancelMatch = true;

						return;
					}
				}
				else if (pRankData->eTeamType == RANKBATTLE_TEAM_CHALLENGER)
				{
					--m_byChallengerCount;
					if (m_byChallengerCount == 0 && m_byInviteChallengerCount == 0) //check if everyone left arena and if none left who still has tp invite to arena
					{
						if (m_eRoomState == RANKBATTLE_ROOMSTATE_WAIT && m_byStage == 0) //if match didnt start yet then send cancel
							CancelMatch();
						else
							m_bCancelMatch = true;

						return;
					}
				}

				//check if there is a winner
				CheckStageWinner();
			}
			else
			{
				ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
			}
		}
	}
	else if (m_eRoomState != RANKBATTLE_ROOMSTATE_MATCH_END) //when player leaves ranked before battle began
	{
		Cancel(pPlayer);
	}
	else
	{
		std::map<HOBJECT, CHARACTERID>::iterator it = m_mapPlayers.find(pPlayer->GetID());
		if (it != m_mapPlayers.end())
		{
			if (pPlayer->GetCharID() == it->second)
			{
				m_mapPlayers.erase(it);
			}
			else
			{
				ERR_LOG(LOG_RANKBATTLE, "Character id does not match %u != %u", pPlayer->GetCharID(), it->second);
			}
		}
	}
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattleBattle::CancelMatch()
{
	m_bCancelMatch = true;

	CNtlPacket packet(sizeof(sGU_RANKBATTLE_MATCH_CANCELED_NFY));
	sGU_RANKBATTLE_MATCH_CANCELED_NFY * res = (sGU_RANKBATTLE_MATCH_CANCELED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_MATCH_CANCELED_NFY;
	packet.SetPacketLen(sizeof(sGU_RANKBATTLE_MATCH_CANCELED_NFY));
	m_pWorld->Broadcast(&packet);

	CNtlPacket packet2(sizeof(sGU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY));
	sGU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY * res2 = (sGU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY;
	packet2.SetPacketLen(sizeof(sGU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY));
	m_pWorld->Broadcast(&packet2);

	TeleportAllOut();
}





//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CRankbattleRoom::CRankbattleRoom(sRANKBATTLE_TBLDAT* tbldat)
{
	m_wRegisterCount = 0;
	m_pRankTblData = tbldat;
}
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CRankbattleRoom::~CRankbattleRoom()
{
	
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattleRoom::TickProcess(DWORD dwTickCount)
{
	//update match rooms
	for (std::map<ROOMID, CRankbattleBattle*>::iterator it = m_mapRankBattleBattle.begin(); it != m_mapRankBattleBattle.end(); )
	{
		CRankbattleBattle* pSubRoom = it->second;

		if (pSubRoom->TickProcess(dwTickCount))
		{
			it = m_mapRankBattleBattle.erase(it);
			SAFE_DELETE(pSubRoom);
		}
		else
		{
			++it;
		}
	}
}

CRankbattleBattle * CRankbattleRoom::GetBattle(ROOMID roomid)
{
	std::map<ROOMID, CRankbattleBattle*>::iterator it = m_mapRankBattleBattle.find(roomid);
	if (it != m_mapRankBattleBattle.end())
		return it->second;

	return NULL;
}

bool CRankbattleRoom::JoinQueue(CPlayer * pPlayer, HOBJECT hObject)
{
	if (GetRegisterCount() >= DBO_RANKBATTLE_MAX_REGISTER * 10) //check if room is already full (max 100 because only 50 worlds can be created)
		return false;

	bool bJoinSuccess = false;

	if (GetBattleMode() == RANKBATTLE_MODE_INDIVIDUAL)
	{
		bJoinSuccess = true;

		pPlayer->SetRankBattleRoomTblidx(GetTblidx()); //set room tblidx
		pPlayer->SetRankBattleRegisterObject(hObject); //set object handle where player registered. User to teleport player out of battle
		m_mapSoloQueue.insert({ pPlayer->GetID(), pPlayer->GetCharID()});

		CNtlPacket packetNfy(sizeof(sGU_RANKBATTLE_JOIN_NFY));
		sGU_RANKBATTLE_JOIN_NFY * resNfy = (sGU_RANKBATTLE_JOIN_NFY *)packetNfy.GetPacketData();
		resNfy->wOpCode = GU_RANKBATTLE_JOIN_NFY;
		resNfy->rankBattleTblidx = GetTblidx();
		packetNfy.SetPacketLen(sizeof(sGU_RANKBATTLE_JOIN_NFY));
		pPlayer->SendPacket(&packetNfy);
	}
	else
	{
		if (pPlayer->GetParty()->RegisterRankBattle(hObject, GetTblidx()))
		{
			m_mapPartyQueue.insert({ pPlayer->GetID(), pPlayer->GetParty() });

			bJoinSuccess = true;
		}
	}

	if(bJoinSuccess)
		++m_wRegisterCount; //increase register counter

	return bJoinSuccess;
}

void CRankbattleRoom::LeaveQueue(CPlayer * pPlayer)
{
	if (GetBattleMode() == RANKBATTLE_MODE_INDIVIDUAL)
	{
		auto it = m_mapSoloQueue.find(pPlayer->GetID());
		if(it != m_mapSoloQueue.end())
		{
			if (pPlayer->GetCharID() == it->second)
			{
				pPlayer->LeaveRankBattleQueue();

				m_mapSoloQueue.erase(it);
				--m_wRegisterCount;	//decrease register counter
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "Characterid (%u != %u) does not match when leaving queue.", pPlayer->GetCharID(), it->second);
			}
		}
	}
	else
	{
		for (auto it = m_mapPartyQueue.begin(); it != m_mapPartyQueue.end(); it++)
		{
			CParty* pParty = it->second;

			if (pPlayer->GetPartyID() == pParty->GetPartyID())
			{
				pParty->LeaveRankBattleQueue();

				m_mapPartyQueue.erase(it);
				--m_wRegisterCount;	//decrease register counter

				break;
			}
		}
	}
}


void CRankbattleRoom::LeaveQueue(PARTYID partyId)
{
	for (auto it = m_mapPartyQueue.begin(); it != m_mapPartyQueue.end(); it++)
	{
		CParty* pParty = it->second;
		if (partyId == pParty->GetPartyID())
		{
			pParty->LeaveRankBattleQueue();

			m_mapPartyQueue.erase(it);
			--m_wRegisterCount;	//decrease register counter

			break;
		}
	}
}


void CRankbattleRoom::UpdateMatchmakingQueue()
{
	int nRemCnt = 0;
	if (GetBattleMode() == RANKBATTLE_MODE_INDIVIDUAL)
	{
		if (m_mapSoloQueue.size() < 2)
			return;

		std::vector<sRANK_SOLO> vecTemp;
		std::unordered_map<HOBJECT, CHARACTERID> vecTmpQueue(m_mapSoloQueue);

		bool bFound = false;

		for (auto fit = vecTmpQueue.begin(); fit != vecTmpQueue.end(); )
		{
			bFound = false;

			CPlayer* pFirst = g_pObjectManager->GetPC(fit->first);
			if (pFirst && pFirst->IsInitialized() && pFirst->GetCharID() == fit->second)
			{
				sRANK_SOLO rank;
				rank.pOwner = pFirst;

				for (auto sit = m_mapSoloQueue.begin(); sit != m_mapSoloQueue.end(); sit++) //find a matching partner
				{
					CPlayer* pSecond = g_pObjectManager->GetPC(sit->first);
					if (pSecond && pSecond->IsInitialized() && pSecond->GetCharID() == sit->second)
					{
						if (pFirst->GetCharID() == pSecond->GetCharID())
							continue;
						if (pFirst->GetLevel() > pSecond->GetLevel() + DBO_RANKBATTLE_MAX_MATCHING_LEVEL
							|| pFirst->GetLevel() < pSecond->GetLevel() - DBO_RANKBATTLE_MAX_MATCHING_LEVEL)
							continue;

						rank.pChallenger = pSecond;

						vecTemp.push_back(rank);

						bFound = true;

						m_mapSoloQueue.erase(sit);
						m_mapSoloQueue.erase(pFirst->GetID());

						vecTmpQueue.erase(pSecond->GetID());
						fit = vecTmpQueue.erase(fit);

						break; //break second loop
					}
				}

				if (bFound == false)
					++fit;
			}
			else
			{
				m_mapSoloQueue.erase(fit->first);
				fit = vecTmpQueue.erase(fit);
			}
		}

		//if queue success then remove found players from queue
		if (vecTemp.size() > 0)
		{
			for (std::vector<sRANK_SOLO>::iterator it = vecTemp.begin(); it != vecTemp.end(); it++)
			{
				sRANK_SOLO* pRank = &*it;

				ROOMID roomid = g_pRankbattleManager->AcquireRoomID();

				CRankbattleBattle* pBattle = new CRankbattleBattle(roomid, m_pRankTblData);

				pBattle->AddPlayer(pRank->pOwner, RANKBATTLE_TEAM_OWNER);
				pBattle->AddPlayer(pRank->pChallenger, RANKBATTLE_TEAM_CHALLENGER);

				m_wRegisterCount -= 2;

				ERR_LOG(LOG_RANKBATTLE, "Create Solo-RankBattle RoomID %u. RankTblidx %u, Char1ID %u (lv %u), Char2ID %u (lv %u)", 
					roomid, m_pRankTblData->tblidx, pRank->pOwner->GetCharID(), pRank->pOwner->GetLevel(), pRank->pChallenger->GetCharID(), pRank->pChallenger->GetLevel());

				m_mapRankBattleBattle.insert(std::make_pair(roomid, pBattle));

				m_mapSoloQueue.erase(pRank->pOwner->GetID());
				m_mapSoloQueue.erase(pRank->pChallenger->GetID());
			}

			vecTemp.clear(); //clear temp vector
		}
	}
	else
	{
		if (m_mapPartyQueue.size() < 2)
			return;

		bool bFound = false;

		std::vector<sRANK_PARTY> vecTemp;
		std::unordered_map<HOBJECT, CParty*> vecTmpQueue(m_mapPartyQueue);

		for (auto fit = vecTmpQueue.begin(); fit != vecTmpQueue.end(); )
		{
			bFound = false;

			CParty* pFirst = fit->second;
			if (pFirst && pFirst->GetPartyLeaderID() == fit->first && pFirst->GetPartyMemberCount() >= 2)
			{
				int nPartyLv = pFirst->GetPartyLevel();

				sRANK_PARTY rank;
				rank.pOwner = pFirst;

				for (auto sit = m_mapPartyQueue.begin(); sit != m_mapPartyQueue.end(); sit++) //find matching partner
				{
					CParty* pSecond = sit->second;
					if (pSecond && pFirst->GetPartyID() != pSecond->GetPartyID() && pSecond->GetPartyLeaderID() == sit->first && pSecond->GetPartyMemberCount() >= 2)
					{
						int nSecondPartyLv = pSecond->GetPartyLevel();

						if (nPartyLv > nSecondPartyLv + DBO_RANKBATTLE_MAX_MATCHING_LEVEL
							|| nPartyLv < nSecondPartyLv - DBO_RANKBATTLE_MAX_MATCHING_LEVEL)
							continue;

						bFound = true;

						rank.pChallenger = pSecond;

						vecTemp.push_back(rank);

						m_mapPartyQueue.erase(sit);
						m_mapPartyQueue.erase(pFirst->GetPartyLeaderID());

						vecTmpQueue.erase(pSecond->GetPartyLeaderID());
						fit = vecTmpQueue.erase(fit);

						break; //break second loop
					}
				}

				if (bFound == false)
					++fit;

			}
			else
			{
				m_mapPartyQueue.erase(fit->first);
				fit = vecTmpQueue.erase(fit);
			}
		}

		//if queue success then remove found players from queue
		if (vecTemp.size() > 0)
		{
			for (std::vector<sRANK_PARTY>::iterator it = vecTemp.begin(); it != vecTemp.end(); it++)
			{
				sRANK_PARTY* pRank = &*it;

				ROOMID roomid = g_pRankbattleManager->AcquireRoomID();

				CRankbattleBattle* pBattle = new CRankbattleBattle(roomid, m_pRankTblData);

				pBattle->AddParty(pRank->pOwner, RANKBATTLE_TEAM_OWNER);
				pBattle->AddParty(pRank->pChallenger, RANKBATTLE_TEAM_CHALLENGER);

				m_wRegisterCount -= 2;

				ERR_LOG(LOG_RANKBATTLE, "Create Party-RankBattle RoomID %u. RankTblidx %u, Party1ID %u (lv %u), Party2ID %u (lv %u)", roomid, m_pRankTblData->tblidx, pRank->pOwner->GetPartyID(), pRank->pOwner->GetPartyLevel(), pRank->pChallenger->GetPartyID(), pRank->pChallenger->GetPartyLevel());

				m_mapRankBattleBattle.insert(std::make_pair(roomid, pBattle));
			}

			vecTemp.clear(); //clear temp vector
		}
	}
}






















////////////////// Manager


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CRankbattle::CRankbattle()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CRankbattle::~CRankbattle()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattle::Destroy()
{
	std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.begin();
	while (it != m_mapRankBattleRoom.end())
	{
		CRankbattleRoom* room = it->second;

		it = m_mapRankBattleRoom.erase(it);

		SAFE_DELETE(room);
	}

	m_mapRankBattleRoom.clear();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattle::Init()
{
	CRankBattleTable* pRankTable = g_pTableContainer->GetRankBattleTable();

	for (CTable::TABLEIT iterTable = pRankTable->Begin(); iterTable != pRankTable->End(); iterTable++)
	{
		sRANKBATTLE_TBLDAT *pData = (sRANKBATTLE_TBLDAT*)(iterTable->second);

		CRankbattleRoom* room = new CRankbattleRoom(pData);

		m_mapRankBattleRoom.insert(std::make_pair(pData->tblidx, room));
	}

	m_BattleRoomId = INVALID_ROOMID;

	m_dwMatchMakingCycle = DBO_RANKBATTLE_MATCH_START_TERM * 1000;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattle::TickProcess(DWORD dwTickCount)
{
	//update match-making
	m_dwMatchMakingCycle = UnsignedSafeDecrease<DWORD>(m_dwMatchMakingCycle, dwTickCount);
	if (m_dwMatchMakingCycle == 0)
	{
		m_dwMatchMakingCycle = DBO_RANKBATTLE_MATCH_START_TERM * 1000;

		for (std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.begin(); it != m_mapRankBattleRoom.end(); it++)
		{
			CRankbattleRoom* pRoom = it->second;

			pRoom->UpdateMatchmakingQueue();
		}
	}

	//update rooms
	for (std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.begin(); it != m_mapRankBattleRoom.end(); it++)
	{
		CRankbattleRoom* pRoom = it->second;

		pRoom->TickProcess(dwTickCount);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattle::LoadRankBattleInfo(CPlayer* pPlayer, BYTE byBattleMode)
{
	BYTE i = 0;

	CNtlPacket packet(sizeof(sGU_RANKBATTLE_INFO_RES));
	sGU_RANKBATTLE_INFO_RES * res = (sGU_RANKBATTLE_INFO_RES *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_INFO_RES;
	res->wResultCode =	GAME_SUCCESS;
	res->byBattleMode = byBattleMode;
	res->dwRemainTime = m_dwMatchMakingCycle / 1000;

	for(std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.begin(); it != m_mapRankBattleRoom.end(); it++)
	{
		CRankbattleRoom* room = it->second;

		if (room->GetBattleMode() == byBattleMode)
		{
			res->asArenaInfo[i].rankBattleTblidx = room->GetTblidx();
			res->asArenaInfo[i].wRegisterCount = room->GetRegisterCount();
			i++;
			break;
		}
	}

	res->byArenaCount = i;
	packet.SetPacketLen(sizeof(sGU_RANKBATTLE_INFO_RES));
	pPlayer->SendPacket(&packet);
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattle::JoinRoom(CPlayer * pPlayer, TBLIDX roomTblidx, HOBJECT hBoardObject)
{
	CGameServer* app = (CGameServer*)g_pApp;

	WORD wResultcode = GAME_SUCCESS;
	BYTE byBattleMode = INVALID_RANKBATTLE_MODE;

	//if (pPlayer->IsGameMaster())
	{
		std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.find(roomTblidx);
		if (it != m_mapRankBattleRoom.end())
		{
			CRankbattleRoom* pRoom = it->second;

			byBattleMode = pRoom->GetBattleMode();

			if (byBattleMode == RANKBATTLE_MODE_INDIVIDUAL)
			{
				if (pPlayer->GetRankBattleRoomTblidx() != INVALID_TBLIDX)
					wResultcode = GAME_RANKBATTLE_MEMBER_ALREADY_JOINED_RANKBATTLE;
				else if (app->GetGsChannel() != 0)
					wResultcode = GAME_FAIL;
				else if (pPlayer->GetRankBattleRoomId() != INVALID_ROOMID)
					wResultcode = GAME_ROOM_ENTER_FAIL;
				else if (pPlayer->GetLevel() < pRoom->GetRequiredMinLevel())
					wResultcode = GAME_ROOM_ENTER_TOO_LOW_LEVEL;
				else if (pPlayer->GetLevel() > pRoom->GetRequiredMaxLevel())
					wResultcode = GAME_ROOM_ENTER_TOO_HIGHTLEVEL;
				else if(pPlayer->GetDragonballScramble() && app->GetGsChannel() == 0)
					wResultcode = SCRAMBLE_CANNOT_DO_WHILE_JOINED;
			}
			else
			{
				if (pPlayer->GetParty() == NULL)
					wResultcode = GAME_COMMON_YOU_ARE_NOT_IN_A_PARTY;
				else if (app->GetGsChannel() != 0)
					wResultcode = GAME_FAIL;
				else if (pPlayer->GetParty()->GetPartyLeaderID() != pPlayer->GetID())
					wResultcode = GAME_PARTY_ONLY_ALLOWED_TO_PARTY_LEADER;
				else if (pPlayer->GetParty()->GetMemberCanEnterRankbattleQueueCount(pRoom->GetRequiredMinLevel(), pRoom->GetRequiredMaxLevel()) < 2)
					wResultcode = GAME_ROOM_ENTER_NEED_MORE_PARTY_MEMEBER;
				else if (pPlayer->GetLevel() < pRoom->GetRequiredMinLevel())
					wResultcode = GAME_ROOM_ENTER_TOO_LOW_LEVEL;
				else if (pPlayer->GetLevel() > pRoom->GetRequiredMaxLevel())
					wResultcode = GAME_ROOM_ENTER_TOO_HIGHTLEVEL;
				else if (pPlayer->GetRankBattleRoomTblidx() != INVALID_TBLIDX) //check if leader already in a queue
				{
					wResultcode = GAME_RANKBATTLE_MEMBER_ALREADY_JOINED_RANKBATTLE;
				}
				else if (pPlayer->GetRankBattleRoomId() != INVALID_ROOMID)
					wResultcode = GAME_ROOM_ENTER_FAIL;
				else if (pPlayer->GetParty()->GetRankBattleRoomTblidx() != INVALID_TBLIDX)
				{
					wResultcode = GAME_RANKBATTLE_MEMBER_ALREADY_JOINED_RANKBATTLE;
				}
			}


			if (wResultcode == GAME_SUCCESS)
			{
				if (pRoom->JoinQueue(pPlayer, hBoardObject) == false)
					wResultcode = GAME_RANKBATTLE_ARENA_IS_FULL;
			}
		}
		else wResultcode = GAME_RANKBATTLE_CANNOT_FIND_ARENA;
	}
	//else wResultcode = GAME_ROOM_ENTER_FAIL;

	BYTE i = 0;

	CNtlPacket packet(sizeof(sGU_RANKBATTLE_JOIN_RES));
	sGU_RANKBATTLE_JOIN_RES * res = (sGU_RANKBATTLE_JOIN_RES *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_JOIN_RES;
	res->wResultCode = wResultcode;
	res->hBoardObject = hBoardObject;
	res->rankBattleTblidx = roomTblidx;
	res->dwRemainTime = m_dwMatchMakingCycle / 1000;

	for (std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.begin(); it != m_mapRankBattleRoom.end(); it++)
	{
		CRankbattleRoom* pRoom = it->second;

		if (pRoom->GetBattleMode() == byBattleMode)
		{
			res->asArenaInfo[i].rankBattleTblidx = pRoom->GetTblidx();
			res->asArenaInfo[i].wRegisterCount = pRoom->GetRegisterCount();
			i++;

			break; //only show 1 for now..
		}
	}
	
	res->byArenaCount = i;
	packet.SetPacketLen(sizeof(sGU_RANKBATTLE_JOIN_RES));
	pPlayer->SendPacket(&packet);
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankbattle::LeaveRoom(CPlayer * pPlayer, TBLIDX roomTblidx, bool bIsLogout/* = false*/)
{
	WORD wResultcode = GAME_SUCCESS;
	
	std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.find(roomTblidx);
	if (it != m_mapRankBattleRoom.end())
	{
		CRankbattleRoom* pRoom = it->second;

		if (bIsLogout == false)
		{
			if (pRoom->GetBattleMode() == RANKBATTLE_MODE_INDIVIDUAL)
			{
				if (pPlayer->GetRankBattleRoomTblidx() == INVALID_TBLIDX || pPlayer->GetRankBattleRoomTblidx() != roomTblidx)
					wResultcode = GAME_RANKBATTLE_NOR_REGISTERED;
				else if (pPlayer->GetRankBattleRoomId() != INVALID_ROOMID)
					wResultcode = GAME_RANKBATTLE_CANNOT_LEAVE_IN_MATCH;
			}
			else
			{
				if (pPlayer->GetParty() == NULL)
					wResultcode = GAME_COMMON_YOU_ARE_NOT_IN_A_PARTY;
				else if (pPlayer->GetParty()->GetPartyLeaderID() != pPlayer->GetID())
					wResultcode = GAME_PARTY_ONLY_ALLOWED_TO_PARTY_LEADER;
				else if (pPlayer->GetParty()->GetRankBattleRoomTblidx() == INVALID_TBLIDX || pPlayer->GetParty()->GetRankBattleRoomTblidx() != roomTblidx)
					wResultcode = GAME_RANKBATTLE_NOR_REGISTERED;
				else if (pPlayer->GetParty()->GetRankBattleRoomId() != INVALID_ROOMID)
					wResultcode = GAME_RANKBATTLE_CANNOT_LEAVE_IN_MATCH;
			}
		}

		if (wResultcode == GAME_SUCCESS)
		{
			pRoom->LeaveQueue(pPlayer);
		}
	}
	else wResultcode = GAME_RANKBATTLE_CANNOT_FIND_ARENA;
	

	CNtlPacket packet(sizeof(sGU_RANKBATTLE_LEAVE_RES));
	sGU_RANKBATTLE_LEAVE_RES * res = (sGU_RANKBATTLE_LEAVE_RES *)packet.GetPacketData();
	res->wOpCode = GU_RANKBATTLE_LEAVE_RES;
	res->wResultCode = wResultcode;
	packet.SetPacketLen(sizeof(sGU_RANKBATTLE_LEAVE_RES));
	pPlayer->SendPacket(&packet);
}


void CRankbattle::LeaveRoom(PARTYID partyId, TBLIDX roomTblidx)
{
	std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.find(roomTblidx);
	if (it != m_mapRankBattleRoom.end())
	{
		CRankbattleRoom* pRoom = it->second;

		pRoom->LeaveQueue(partyId);
	}
}


void CRankbattle::EnterBattle(CPlayer * pPlayer)
{
	std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.find(pPlayer->GetRankBattleRoomTblidx());
	if (it != m_mapRankBattleRoom.end())
	{
		CRankbattleRoom* pRoom = it->second;

		CRankbattleBattle* pBattleRoom = pRoom->GetBattle(pPlayer->GetRankBattleRoomId());
		
		if(pBattleRoom) //battle room might get deleted
			pBattleRoom->EnterBattle(pPlayer);
	}
}


void CRankbattle::CancelTeleport(CPlayer * pPlayer)
{
	std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.find(pPlayer->GetRankBattleRoomTblidx());
	if (it != m_mapRankBattleRoom.end())
	{
		CRankbattleRoom* pRoom = it->second;

		CRankbattleBattle* pBattleRoom = pRoom->GetBattle(pPlayer->GetRankBattleRoomId());
		if (pBattleRoom)
		{
			pBattleRoom->Cancel(pPlayer);
		}

		pPlayer->LeaveRankBattleQueue();
	}
}

void CRankbattle::LeaveBattle(CPlayer * pPlayer)
{
	std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.find(pPlayer->GetRankBattleRoomTblidx());
	if (it != m_mapRankBattleRoom.end())
	{
		CRankbattleRoom* pRoom = it->second;

		CRankbattleBattle* pBattleRoom = pRoom->GetBattle(pPlayer->GetRankBattleRoomId());
		if(pBattleRoom)
			pBattleRoom->LeaveBattle(pPlayer);

		pPlayer->SetRankBattleRegisterObject(INVALID_HOBJECT);
		pPlayer->SetRankBattleRoomId(INVALID_ROOMID);
		pPlayer->SetRankBattleRoomTblidx(INVALID_TBLIDX);
	}
}

bool CRankbattle::DoesBattleStillExist(TBLIDX battleTblidx, ROOMID roomid)
{
	std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.find(battleTblidx);
	if (it != m_mapRankBattleRoom.end())
	{
		CRankbattleRoom* pRoom = it->second;

		CRankbattleBattle* pBattleRoom = pRoom->GetBattle(roomid);
		if (pBattleRoom == NULL)
			return false;

		if (pBattleRoom->IsEnd() || pBattleRoom->IsCancel())
			return false;
	}

	return true;
}

void CRankbattle::UpdatePlayerState(TBLIDX battleTblidx, ROOMID roomid, CPlayer* pPlayer, BYTE byState)
{
	std::map<ROOMID, CRankbattleRoom*>::iterator it = m_mapRankBattleRoom.find(battleTblidx);
	if (it != m_mapRankBattleRoom.end())
	{
		CRankbattleRoom* pRoom = it->second;

		CRankbattleBattle* pBattleRoom = pRoom->GetBattle(roomid);
		if (pBattleRoom)
			pBattleRoom->UpdatePlayerState(pPlayer, byState);
	}
}


