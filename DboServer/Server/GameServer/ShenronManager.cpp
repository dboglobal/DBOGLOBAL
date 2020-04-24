#include "stdafx.h"
#include "Npc.h"
#include "CPlayer.h"
#include "TriggerObject.h"
#include "item.h"
#include "GameServer.h"
#include "NtlPacketGU.h"
#include "NtlResultCode.h"
#include "TableContainerManager.h"
#include "GameMain.h"
#include "DragonballScramble.h"


CShenronManager::CShenronManager()
{
	dwLastShenronLoop = 0;
}

CShenronManager::~CShenronManager()
{
}

void CShenronManager::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (GetTickCount() >= dwLastShenronLoop + 1000)
	{
		int nInfiniteLoop = 0;

		for (std::map<HOBJECT, sCOLLECT_BRIEF>::iterator it = m_map_Shenron.begin(); it != m_map_Shenron.end(); )
		{
			++nInfiniteLoop;
			if (nInfiniteLoop > 1000)
			{
				ERR_LOG(LOG_SYSTEM, "INFINITE LOOP DETECTED");
			}

			sCOLLECT_BRIEF& shenron = it->second;
			if (&shenron)
			{
				shenron.dwTick += GetTickCount() - this->dwLastShenronLoop;

				if (shenron.byState == COLLECT_STATE_DB_UI_OK && shenron.dwTick >= DBO_MAX_TIMER_DB_UI_OK + 3000) //When shenron timer end
				{
					CPlayer* summoner = g_pObjectManager->FindByChar(shenron.charId);
					if (summoner)
					{
						//As PENALTY remove only one dragon ball.
						CItem* item = summoner->GetPlayerItemContainer()->GetItem(shenron.asData[0].hItem);
						if (item)
						{
							if (item->GetTblidx() < 200041 && item->GetTblidx() > 200047) //dont do penalty if it is the scramble ball
								item->SetCount(item->GetCount() - 1, true, true);

							summoner->SetShenronID(INVALID_HOBJECT);

							CNtlPacket packet(sizeof(sGU_DRAGONBALL_REWARD_RES));
							sGU_DRAGONBALL_REWARD_RES * res = (sGU_DRAGONBALL_REWARD_RES *)packet.GetPacketData();
							res->wOpCode = GU_DRAGONBALL_REWARD_RES;
							res->hObject = it->first;
							res->wResultCode = GAME_SUCCESS;
							packet.SetPacketLen(sizeof(sGU_DRAGONBALL_REWARD_RES));
							g_pApp->Send(summoner->GetClientSessionID(), &packet);

							CNtlPacket packet2(sizeof(sGU_DRAGONBALL_COLLECT_NFY));
							sGU_DRAGONBALL_COLLECT_NFY * res2 = (sGU_DRAGONBALL_COLLECT_NFY *)packet2.GetPacketData();
							res2->wOpCode = GU_DRAGONBALL_COLLECT_NFY;
							res2->hObject = it->first;
							res2->wResultCode = GAME_SUCCESS;
							packet2.SetPacketLen(sizeof(sGU_DRAGONBALL_COLLECT_NFY));
							g_pApp->Send(summoner->GetClientSessionID(), &packet2);
						}
						else
						{
							ERR_LOG(LOG_USER, "ERROR: USER %u COULD NOT FIND DRAGONBALL.", summoner->GetCharID());
						}
					}

					shenron.byState = COLLECT_STATE_DRAGON_OUT;
					shenron.dwTick = 0;
				}
				else if (shenron.byState == COLLECT_STATE_DRAGON_OUT && shenron.dwTick > DBO_MAX_TIMER_DRAGON_DESPAWN)
				{
					CNpc* pShenron = g_pObjectManager->GetNpc(shenron.hDragon);
					if (pShenron)
					{
						pShenron->SendCharStateDespawning(TELEPORT_TYPE_DEFAULT);

						shenron.byState = COLLECT_STATE_DRAGON_DESPAWN;
						shenron.dwTick = 0;

						CWorldZone* pWorldZone = pShenron->GetCurWorldZone();
						pWorldZone->UpdateZoneInfo(false);

						CPlayer* summoner = g_pObjectManager->FindByChar(shenron.charId);
						if (summoner)
						{
							for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
							{
								CItem* item = summoner->GetPlayerItemContainer()->GetItem(shenron.asData[i].hItem);
								if (item)
									item->SetLocked(false);
							}
						}
					}
				}
				else if (shenron.byState == COLLECT_STATE_DRAGON_DESPAWN && shenron.dwTick > DBO_MAX_TIMER_DRAGON_OUT)
				{
					CNpc* pShenron = g_pObjectManager->GetNpc(shenron.hDragon);
					if (pShenron)
					{
						app->GetGameMain()->GetWorldManager()->LeaveObject(pShenron);
						g_pObjectManager->DestroyCharacter(pShenron);
					}

					m_mapUsedAltar.erase(shenron.hAltar);

					it = m_map_Shenron.erase(it);
					continue;
				}
			}
			else
			{
				ERR_LOG(LOG_GENERAL, "ERROR Shenron is null.");
				it = m_map_Shenron.erase(it);
				continue;
			}

			++it;
		}

		dwLastShenronLoop = GetTickCount();
	}
}



void CShenronManager::SpawnShenron(BYTE byBallType, HOBJECT hAltar, CPlayer* pkSummoner, sITEM_POSITION_DATA* sData)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CTriggerObject* obj = pkSummoner->GetCurWorld()->FindStaticObject(hAltar);
	if (obj)
	{
		sDRAGONBALL_TBLDAT* pDragonballTbldat = g_pTableContainer->GetDragonBallTable()->GetDBTbldatByGroupAndType(obj->GetContent(), byBallType);
		if (pDragonballTbldat)
		{
			sNPC_TBLDAT* pNPCTblData = (sNPC_TBLDAT*)g_pTableContainer->GetNpcTable()->FindData(pDragonballTbldat->dragonNPCTblidx);
			if (pNPCTblData)
			{
				//spawn shenron
				CNpc* pShenron = (CNpc*)g_pObjectManager->CreateCharacter(OBJTYPE_NPC);
				if (pShenron->CreateShenron(pNPCTblData, obj->GetCurLoc(), pDragonballTbldat->fDir))
				{
					ERR_LOG(LOG_GENERAL, "Player %u summon shenron. byBallType = %u, hAltar = %u", pkSummoner->GetCharID(), byBallType, hAltar);
					m_mapUsedAltar.insert({ hAltar, pkSummoner->GetID() });

					//set zone night
					CWorldZone* pWorldZone = pkSummoner->GetCurWorldZone();
					pWorldZone->UpdateZoneInfo(true); 

					//summon
					app->GetGameMain()->GetWorldManager()->EnterObject(pShenron, pkSummoner->GetWorldID());
					pShenron->GetBotController()->ChangeControlState_Enter();

					sCOLLECT_BRIEF sBrief;
					sBrief.charId = pkSummoner->GetCharID();
					sBrief.byState = COLLECT_STATE_DB_UI_OK;
					sBrief.hDragon = pShenron->GetID();
					sBrief.dwTick = 0;
					sBrief.hAltar = hAltar;
					sBrief.byBallType = byBallType;

					for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
					{
						sBrief.asData[i].byPlace = sData[i].byPlace;
						sBrief.asData[i].byPos = sData[i].byPos;
						sBrief.asData[i].hItem = sData[i].hItem;

						CItem* item = pkSummoner->GetPlayerItemContainer()->GetItem(sData[i].hItem);
						item->SetLocked(true);
					}

					m_map_Shenron.insert({ hAltar, sBrief });
					pkSummoner->SetShenronID(pShenron->GetID());

					pShenron->GetStateManager()->ChangeCharState(CHARSTATE_STANDING, NULL, false);
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "Create Shenron failed");
					g_pObjectManager->DestroyCharacter(pShenron);
				}
			}
			else ERR_LOG(LOG_GENERAL, "pNPCTblData is null. db->dragonNPCTblidx %d", pDragonballTbldat->dragonNPCTblidx);
		}
		else ERR_LOG(LOG_GENERAL, "db is null. obj->GetContent() %d, byBallType %d", obj->GetContent(), byBallType);
	}
	else ERR_LOG(LOG_GENERAL, "obj is null. hAltar %d \n", hAltar);
}


void CShenronManager::DespawnShenron(CPlayer* pkSummoner, bool bRemoveBalls/* = true*/)
{
	for (std::map<HOBJECT, sCOLLECT_BRIEF>::iterator it = m_map_Shenron.begin(); it != m_map_Shenron.end(); it++)
	{
		sCOLLECT_BRIEF& shenron = it->second;
		if (&shenron)
		{
			if (shenron.charId == pkSummoner->GetCharID() && shenron.byState == COLLECT_STATE_DB_UI_OK) //only update state if not already despawning
			{
				shenron.byState = COLLECT_STATE_DRAGON_OUT;
				shenron.dwTick = 0;

				if (shenron.byBallType == DRAGON_BALL_TYPE_BATTLE) //check if scramble balls
				{
					g_pDragonballScramble->EndEvent(); //here we will remove items
				}
				else
				{
					if (bRemoveBalls && pkSummoner->IsInitialized())	//only remove when player is not logging out
					{
						for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; i++)
						{
							CItem* item = pkSummoner->GetPlayerItemContainer()->GetItem(shenron.asData[i].hItem);
							if (item)
								item->SetCount(item->GetCount() - 1, true, true);
						}
					}
				}

				pkSummoner->SetShenronID(INVALID_HOBJECT);

				break;
			}
		}
	}
}

BYTE CShenronManager::GetBallType(CHARACTERID charid)
{
	for (std::map<HOBJECT, sCOLLECT_BRIEF>::iterator it = m_map_Shenron.begin(); it != m_map_Shenron.end(); it++)
	{
		sCOLLECT_BRIEF& shenron = it->second;
		if (&shenron)
		{
			if (shenron.charId == charid && shenron.byState == COLLECT_STATE_DB_UI_OK) //only update state if not already despawning
			{
				return shenron.byBallType;
			}
		}
	}

	return INVALID_BYTE;
}



bool CShenronManager::CanSummonShenron(HOBJECT hAltar)
{
	return m_mapUsedAltar.find(hAltar) == m_mapUsedAltar.end();
}