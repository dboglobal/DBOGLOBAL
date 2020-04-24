#include "stdafx.h"
#include "BudokaiManager.h"
#include "QueryServer.h"
#include "NtlPacketQG.h"
#include "NtlResultCode.h"
#include "PlayerCache.h"
#include "Servermanager.h"
#include "Dojo.h"


CBudokaiManager::CBudokaiManager()
{
	Init();
}

CBudokaiManager::~CBudokaiManager()
{
	Destroy();
}

void CBudokaiManager::Init()
{
	memset(&m_sBudokai, -1, sizeof(m_sBudokai));

	smart_ptr<QueryResult> result = GetLogDB.Query("SELECT * FROM budokai");
	if (result)
	{
		Field* f = result->Fetch();
		
		m_sBudokai.wSeasonCount = f[0].GetWORD();
		m_sBudokai.tmDefaultOpenTime = f[1].GetDWORD();
	}

	m_wJoinId = 1;
}

void CBudokaiManager::Destroy()
{
}


void CBudokaiManager::SendInitNfy(HSESSION hSession)
{
	CNtlPacket packet(sizeof(sQG_BUDOKAI_INIT_DATA_NFY));
	sQG_BUDOKAI_INIT_DATA_NFY * res = (sQG_BUDOKAI_INIT_DATA_NFY *)packet.GetPacketData();
	res->wOpCode = QG_BUDOKAI_INIT_DATA_NFY;
	res->wSeasonCount = m_sBudokai.wSeasonCount;
	res->nDefaultOpenTime = m_sBudokai.tmDefaultOpenTime;
	packet.SetPacketLen(sizeof(sQG_BUDOKAI_INIT_DATA_NFY));
	g_pApp->Send(hSession, &packet);
}

void CBudokaiManager::UpdateState(sBUDOKAI_STATE_DATA & stateData)
{
	m_sBudokai.sStateData.byState = stateData.byState;
	m_sBudokai.sStateData.tmNextStepTime = stateData.tmNextStepTime;
}

void CBudokaiManager::UpdateMatchState(BYTE byMatchType, sBUDOKAI_MATCHSTATE_DATA & stateData)
{
	if (byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		m_sBudokai.sIndividualStateData.byState = stateData.byState;
		m_sBudokai.sIndividualStateData.tmNextStepTime = stateData.tmNextStepTime;
	}
	else
	{
		m_sBudokai.sTeamStateData.byState = stateData.byState;
		m_sBudokai.sTeamStateData.tmNextStepTime = stateData.tmNextStepTime;
	}
}

void CBudokaiManager::JoinIndividual(HOBJECT handle, CHARACTERID charId, float fPoint, HSESSION hSession)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_BUDOKAI_JOIN_INDIVIDUAL_RES));
	sQG_BUDOKAI_JOIN_INDIVIDUAL_RES * res = (sQG_BUDOKAI_JOIN_INDIVIDUAL_RES *)packet.GetPacketData();
	res->wOpCode = QG_BUDOKAI_JOIN_INDIVIDUAL_RES;
	res->wResultCode = GAME_SUCCESS;
	res->handle = handle;
	res->charId = charId;
	res->bDojoRecommender = false;
	res->fPoint = fPoint;

	if (CPlayerCache* pPlayer = g_pPlayerCache->GetCharacter(charId))
	{
		if(pPlayer->GetJoinID() == INVALID_JOINID)
		{
			if (IsBudokaiOpen(m_sBudokai.sStateData.byState))
			{
				if (m_sBudokai.sIndividualStateData.byState == BUDOKAI_MATCHSTATE_REGISTER)
				{
					NTL_SAFE_WCSCPY(res->wszCharName, pPlayer->GetCharName());

					// DOJO SEED
					if (pPlayer->GetGuildID() != 0 && m_mapDojoSeed.size() < DBO_MAX_COUNT_DOJO_IN_WORLD)
					{
						if (sDBO_DOJO_DATA* pDojo = g_pDojo->GetDojoData(pPlayer->GetGuildID()))
						{
							std::map<TBLIDX, CHARACTERID>::iterator it = m_mapDojoSeed.find(pDojo->dojoTblidx);

							if (it == m_mapDojoSeed.end() && _wcsicmp(pDojo->wszSeedCharName, pPlayer->GetCharName()) == 0)
							{
								res->bDojoRecommender = true;
								m_mapDojoSeed.insert({ pDojo->dojoTblidx, pPlayer->GetCharID() });

								res->wJoinId = GenerateJoinId(); //generate a join id

								m_mapJoinID.insert({ res->wJoinId , charId});

								pPlayer->SetJoinID(res->wJoinId);

								sBUDOKAI_JOIN_INFO joinInfo;
								joinInfo.byMatchType = BUDOKAI_MATCH_TYPE_INDIVIDIAUL;
								joinInfo.byJoinState = BUDOKAI_JOIN_STATE_PLAY;
								joinInfo.byJoinResult = BUDOKAI_JOIN_RESULT_REGISTER;
								joinInfo.sIndividualInfo.bDojoRecommender = true;
								joinInfo.sIndividualInfo.wJoinId = res->wJoinId;
								NTL_SAFE_WCSCPY(joinInfo.sIndividualInfo.wszCharName, pPlayer->GetCharName());
								m_mapJoinInfo.insert(std::make_pair(res->wJoinId, joinInfo));	//insert into map
							}
						}
					}

					if (res->bDojoRecommender == false)
					{
						if (m_mapJoinInfo.size() < BUDOKAI_MAX_INDIVIDUAL_REGISTER_COUNT)
						{
							res->wJoinId = GenerateJoinId(); //generate a join id

							m_mapJoinID.insert({ res->wJoinId , charId });

							pPlayer->SetJoinID(res->wJoinId);

							sBUDOKAI_JOIN_INFO joinInfo;
							joinInfo.byMatchType = BUDOKAI_MATCH_TYPE_INDIVIDIAUL;
							joinInfo.byJoinState = BUDOKAI_JOIN_STATE_PLAY;
							joinInfo.byJoinResult = BUDOKAI_JOIN_RESULT_REGISTER;
							joinInfo.sIndividualInfo.bDojoRecommender = false;
							joinInfo.sIndividualInfo.wJoinId = res->wJoinId;
							NTL_SAFE_WCSCPY(joinInfo.sIndividualInfo.wszCharName, pPlayer->GetCharName());
							m_mapJoinInfo.insert(std::make_pair(res->wJoinId, joinInfo));	//insert into map
						}
						else res->wResultCode = GAME_BUDOKAI_OVER_COUNT;
					}

				}
				else res->wResultCode = GAME_BUDOKAI_NOT_REGISTER_PERIOD;
			}
			else res->wResultCode = GAME_BUDOKAI_NOT_OPENED;
		}
		else res->wResultCode = GAME_BUDOKAI_CHARACTER_ALREADY_JOIN;
	}
	else res->wResultCode = GAME_DB_CHAR_NOT_FOUND;

	packet.SetPacketLen(sizeof(sQG_BUDOKAI_JOIN_INDIVIDUAL_RES));

	if(res->wResultCode == GAME_SUCCESS)
		g_SrvMgr->Broadcast(&packet);
	else
		app->Send(hSession, &packet);
}

void CBudokaiManager::LeaveIndividual(HOBJECT handle, CHARACTERID charId, HSESSION hSession)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_BUDOKAI_LEAVE_INDIVIDUAL_RES));
	sQG_BUDOKAI_LEAVE_INDIVIDUAL_RES * res = (sQG_BUDOKAI_LEAVE_INDIVIDUAL_RES *)packet.GetPacketData();
	res->wOpCode = QG_BUDOKAI_LEAVE_INDIVIDUAL_RES;
	res->handle = handle;
	res->wResultCode = GAME_SUCCESS;
	res->charId = charId;

	if (IsBudokaiOpen(m_sBudokai.sStateData.byState))
	{
		if (CPlayerCache* pPlayer = g_pPlayerCache->GetCharacter(charId))
		{
			if (pPlayer->GetJoinID() != INVALID_JOINID)
			{
				res->joinId = pPlayer->GetJoinID();

				//free the join id so it can be reused
				EraseJoinId(res->joinId);

				pPlayer->SetJoinID(INVALID_JOINID);

				// DOJO SEED
				if (pPlayer->GetGuildID() != 0)
				{
					if (sDBO_DOJO_DATA* pDojo = g_pDojo->GetDojoData(pPlayer->GetGuildID()))
					{
						std::map<TBLIDX, CHARACTERID>::iterator it = m_mapDojoSeed.find(pDojo->dojoTblidx);

						if (it != m_mapDojoSeed.end() && _wcsicmp(pDojo->wszSeedCharName, pPlayer->GetCharName()) == 0)
						{
							if (it->second == pPlayer->GetCharID())
								m_mapDojoSeed.erase(pDojo->dojoTblidx);
						}
					}
				}

				m_mapJoinInfo.erase(res->joinId);
				m_mapJoinID.erase(res->joinId);
			}
			else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
		}
		else res->wResultCode = GAME_DB_CHAR_NOT_FOUND;
	}

	packet.SetPacketLen(sizeof(sQG_BUDOKAI_LEAVE_INDIVIDUAL_RES));

	if (res->wResultCode == GAME_SUCCESS)
		g_SrvMgr->Broadcast(&packet);
	else
		app->Send(hSession, &packet);
}

void CBudokaiManager::TournamentIndividualAddEntry(JOINID wJoinId, BYTE byMatchIndex, BYTE byWinnerJoinResult, BYTE byLoserResultCondition, BYTE byLoserJoinState)
{
	CNtlPacket packet(sizeof(sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES));
	sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES * res = (sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES *)packet.GetPacketData();
	res->wOpCode = QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES;
	res->byMatchIndex = byMatchIndex;
	res->wResultCode = GAME_SUCCESS;

	std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(wJoinId);
	if (it != m_mapJoinInfo.end())
	{
		std::map<JOINID, CHARACTERID>::iterator it2 = m_mapJoinID.find(wJoinId);
		if (it2 != m_mapJoinID.end())
		{
			if (byWinnerJoinResult != INVALID_BUDOKAI_JOIN_RESULT)
			{
				it->second.byJoinResult = byWinnerJoinResult;
			}
			else
			{
				it->second.byJoinResult = byLoserResultCondition;
				it->second.byJoinState = byLoserJoinState;
			}

			res->joinData.charId = it2->second;
			res->joinData.joinId = wJoinId;
			res->joinData.byJoinState = it->second.byJoinState;
			res->joinData.byJoinResult = it->second.byJoinResult;
		}
		else res->wResultCode = GAME_FAIL;
	}
	else res->wResultCode = GAME_FAIL;
	
	packet.SetPacketLen(sizeof(sQG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES));
	g_SrvMgr->Broadcast(&packet, DOJO_CHANNEL_INDEX);
}

void CBudokaiManager::JoinTeam(HOBJECT handle, CHARACTERID charId, WCHAR * wszTeamName, BYTE byMemberCount, CHARACTERID * aMembers, float fPoint, sBUDOKAI_TEAM_POINT_INFO * aTeamInfo, HSESSION hSession)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_BUDOKAI_JOIN_TEAM_RES));
	sQG_BUDOKAI_JOIN_TEAM_RES * res = (sQG_BUDOKAI_JOIN_TEAM_RES *)packet.GetPacketData();
	res->wOpCode = QG_BUDOKAI_JOIN_TEAM_RES;
	res->wResultCode = GAME_SUCCESS;
	res->handle = handle;
	res->charId = charId;
	res->fPoint = fPoint;
	NTL_SAFE_WCSCPY(res->wszTeamName, wszTeamName);
	res->byMemberCount = byMemberCount;
	memcpy(res->aTeamInfo, aTeamInfo, sizeof(res->aTeamInfo));
	memcpy(res->aMembers, aMembers, sizeof(res->aMembers));

	for (BYTE i = 0; i < byMemberCount; i++)
	{
		CPlayerCache* pMember = g_pPlayerCache->GetCharacter(aMembers[i]);
		if (pMember == NULL || pMember->GetJoinID() != INVALID_JOINID)
		{
			res->wResultCode = GAME_BUDOKAI_MEMBER_ALREADY_JOINED;
			break;
		}
	}

	if (res->wResultCode == GAME_SUCCESS)
	{
		for (std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.begin(); it != m_mapJoinInfo.end(); it++)
		{
			if (wcscmp(it->second.sTeamInfo.wszTeamName, res->wszTeamName) == 0)
			{
				res->wResultCode = GAME_BUDOKAI_INVALID_TEAM_NAME;
				break;
			}
		}
	}

	if(res->wResultCode == GAME_SUCCESS)
	{
		if (IsBudokaiOpen(m_sBudokai.sStateData.byState))
		{
			if (m_sBudokai.sTeamStateData.byState == BUDOKAI_MATCHSTATE_REGISTER)
			{
				if (m_mapJoinInfo.size() < BUDOKAI_MAX_TEAM_REGISTER_COUNT)
				{
					res->wJoinId = GenerateJoinId(); //generate a join id

					for (BYTE i = 0; i < byMemberCount; i++)
					{
						CPlayerCache* pMember = g_pPlayerCache->GetCharacter(aMembers[i]);
						if (pMember)
						{
							pMember->SetJoinID(res->wJoinId);

							m_mapJoinID.insert({ res->wJoinId, aMembers[i] });
						}
					}

					sBUDOKAI_JOIN_INFO joinInfo;
					joinInfo.byMatchType = BUDOKAI_MATCH_TYPE_TEAM;
					joinInfo.byJoinState = BUDOKAI_JOIN_STATE_PLAY;
					joinInfo.byJoinResult = BUDOKAI_JOIN_RESULT_REGISTER;
					joinInfo.sTeamInfo.wJoinId = res->wJoinId;
					NTL_SAFE_WCSCPY(joinInfo.sTeamInfo.wszTeamName, wszTeamName);
					memcpy(joinInfo.sTeamInfo.aMemberInfo, aTeamInfo, sizeof(sBUDOKAI_TEAM_POINT_INFO) * byMemberCount);
					m_mapJoinInfo.insert(std::make_pair(res->wJoinId, joinInfo));	//insert into map
				}
				else res->wResultCode = GAME_BUDOKAI_OVER_COUNT;
			}
			else res->wResultCode = GAME_BUDOKAI_NOT_REGISTER_PERIOD;
		}
		else res->wResultCode = GAME_BUDOKAI_NOT_OPENED;
	}
	else res->wResultCode = GAME_BUDOKAI_CHARACTER_ALREADY_JOIN;

	packet.SetPacketLen(sizeof(sQG_BUDOKAI_JOIN_TEAM_RES));

	if (res->wResultCode == GAME_SUCCESS)
		g_SrvMgr->Broadcast(&packet);
	else
		app->Send(hSession, &packet);
}

void CBudokaiManager::LeaveLeam(HOBJECT handle, CHARACTERID charId, HSESSION hSession)
{
	CQueryServer* app = (CQueryServer*)g_pApp;

	CNtlPacket packet(sizeof(sQG_BUDOKAI_LEAVE_TEAM_RES));
	sQG_BUDOKAI_LEAVE_TEAM_RES * res = (sQG_BUDOKAI_LEAVE_TEAM_RES *)packet.GetPacketData();
	res->wOpCode = QG_BUDOKAI_LEAVE_TEAM_RES;
	res->handle = handle;
	res->wResultCode = GAME_SUCCESS;
	res->charId = charId;
	res->byMemberCount = 0;

	if (IsBudokaiOpen(m_sBudokai.sStateData.byState))
	{
		if (CPlayerCache* pPlayer = g_pPlayerCache->GetCharacter(charId))
		{
			std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(pPlayer->GetJoinID());
			if (it != m_mapJoinInfo.end())
			{
				if (it->second.byMatchType == BUDOKAI_MATCH_TYPE_TEAM)
				{
					res->wJoinId = it->first;

					//free the join id so it can be reused
					EraseJoinId(it->first);

					std::multimap<JOINID, CHARACTERID>::iterator itLow = m_mapJoinID.lower_bound(res->wJoinId);
					std::multimap<JOINID, CHARACTERID>::iterator itUp = m_mapJoinID.upper_bound(res->wJoinId);

					while (itLow != itUp)
					{
						if(res->byMemberCount < NTL_MAX_MEMBER_IN_PARTY)
							res->aMembers[res->byMemberCount++] = itLow->second;

						CPlayerCache* pMember = g_pPlayerCache->GetCharacter(itLow->second);
						if (pMember)
							pMember->SetJoinID(INVALID_JOINID);

						itLow++;
					}

					m_mapJoinID.erase(res->wJoinId);
					m_mapJoinInfo.erase(it->first);
				}
			}
			else res->wResultCode = GAME_BUDOKAI_CHARACTER_NOT_JOINED;
		}
		else res->wResultCode = GAME_DB_CHAR_NOT_FOUND;
	}

	packet.SetPacketLen(sizeof(sQG_BUDOKAI_LEAVE_TEAM_RES));

	if (res->wResultCode == GAME_SUCCESS)
		g_SrvMgr->Broadcast(&packet);
	else
		app->Send(hSession, &packet);
}

void CBudokaiManager::TournamentTeamAddEntry(JOINID wJoinId, BYTE byMatchIndex, BYTE byWinnerJoinResult, BYTE byLoserResultCondition, BYTE byLoserJoinState)
{
	CNtlPacket packet(sizeof(sQG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES));
	sQG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES * res = (sQG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES *)packet.GetPacketData();
	res->wOpCode = QG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES;
	res->byMatchIndex = byMatchIndex;
	res->wResultCode = GAME_SUCCESS;
	res->joinData.joinId = wJoinId;

	std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.find(wJoinId);
	if (it != m_mapJoinInfo.end())
	{
		if (byWinnerJoinResult != INVALID_BUDOKAI_JOIN_RESULT)
		{
			it->second.byJoinResult = byWinnerJoinResult;
		}
		else
		{
			it->second.byJoinResult = byLoserResultCondition;
			it->second.byJoinState = byLoserJoinState;
		}

		res->joinData.byJoinState = it->second.byJoinState;
		res->joinData.byJoinResult = it->second.byJoinResult;
	}
	else res->wResultCode = GAME_FAIL;

	packet.SetPacketLen(sizeof(sQG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES));
	g_SrvMgr->Broadcast(&packet, DOJO_CHANNEL_INDEX);
}

void CBudokaiManager::HistoryWrite(BYTE byBudokaiType, BYTE byMatchType)
{
	if (byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL)
	{
		for (std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.begin(); it != m_mapJoinInfo.end(); it++)
		{
			if (it->second.byJoinResult == BUDOKAI_JOIN_RESULT_WINNER)
			{
				std::multimap<JOINID, CHARACTERID>::iterator it2 = m_mapJoinID.find(it->first);
				if(it2 != m_mapJoinID.end())
					GetLogDB.Execute("INSERT INTO budokai_winners (BudokaiNumber,Type,MatchType,WinnerCharID1) VALUES (%u, %u, %u, %u)", m_sBudokai.wSeasonCount, byBudokaiType, byMatchType, it2->second);

				break;

			}
		}
	}
	else
	{
		for (std::map<JOINID, sBUDOKAI_JOIN_INFO>::iterator it = m_mapJoinInfo.begin(); it != m_mapJoinInfo.end(); it++)
		{
			if (it->second.byJoinResult == BUDOKAI_JOIN_RESULT_WINNER)
			{
				std::multimap<JOINID, CHARACTERID>::iterator itLow = m_mapJoinID.lower_bound(it->first);
				std::multimap<JOINID, CHARACTERID>::iterator itUp = m_mapJoinID.upper_bound(it->first);

				CHARACTERID	aMembers[NTL_MAX_MEMBER_IN_PARTY];
				DWORD i = 0;

				while (itLow != itUp)
				{
					aMembers[i] = itLow->second;

					++i;
					++itLow;

					if (i == NTL_MAX_MEMBER_IN_PARTY)
						break;
				}

				GetLogDB.Execute("INSERT INTO budokai_winners (BudokaiNumber,Type,MatchType,WinnerCharID1,WinnerCharID2,WinnerCharID3,WinnerCharID4,WinnerCharID5) VALUES (%u, %u, %u, %u, %u, %u, %u, %u)", 
					m_sBudokai.wSeasonCount, byBudokaiType, byMatchType, aMembers[0], aMembers[1], aMembers[2], aMembers[3], aMembers[4]);

				break;
			}
		}
	}

	//IMPRTANT: THIS SHOULD BE DONE WHEN ALL BUDOKAI FINISHED
	if (byMatchType == BUDOKAI_MATCH_TYPE_TEAM && byBudokaiType == BUDOKAI_TYPE_ADULT) //solo adult is the last one. So we update when solo adult ends
	{
		m_sBudokai.wSeasonCount += 1;
		GetLogDB.Execute("UPDATE budokai SET SeasonCount=%u", m_sBudokai.wSeasonCount);
	}

	//clear everything except m_sBudokai. So we are ready for next budokai
	Clear();
}

void CBudokaiManager::Clear()
{
	CNtlPacket packet(sizeof(sQG_BUDOKAI_HISTORY_WRITE_RES));
	sQG_BUDOKAI_HISTORY_WRITE_RES * res = (sQG_BUDOKAI_HISTORY_WRITE_RES *)packet.GetPacketData();
	res->wOpCode = QG_BUDOKAI_HISTORY_WRITE_RES;
	res->wResultCode = GAME_SUCCESS;
	packet.SetPacketLen(sizeof(sQG_BUDOKAI_HISTORY_WRITE_RES));
	g_SrvMgr->Broadcast(&packet, DOJO_CHANNEL_INDEX);


	m_wJoinId = 1;
	m_vevJoinId.clear();

	for (std::multimap<JOINID, CHARACTERID>::iterator it = m_mapJoinID.begin(); it != m_mapJoinID.end(); it++)
	{
		CPlayerCache* pPlayer = g_pPlayerCache->GetCharacter(it->second);
		if (pPlayer)
			pPlayer->SetJoinID(INVALID_JOINID);
	}

	m_mapJoinID.clear();
	m_mapJoinInfo.clear();
	m_mapDojoSeed.clear();

	memset(&m_sBudokai.sIndividualStateData, -1, sizeof(m_sBudokai.sIndividualStateData));
	memset(&m_sBudokai.sStateData, -1, sizeof(m_sBudokai.sStateData));
	memset(&m_sBudokai.sTeamStateData, -1, sizeof(m_sBudokai.tmDefaultOpenTime));
}

JOINID CBudokaiManager::GenerateJoinId()
{
	JOINID wID = INVALID_JOINID;

	if (m_vevJoinId.size() > 0)
	{
		wID = m_vevJoinId.front();
		m_vevJoinId.erase(m_vevJoinId.begin());
	}
	else
		wID = m_wJoinId++;

	return wID;
}

void CBudokaiManager::EraseJoinId(JOINID wId)
{
	m_vevJoinId.push_back(wId);
}

