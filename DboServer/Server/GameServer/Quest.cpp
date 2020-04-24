#include "stdafx.h"
#include "GameServer.h"
#include "QuestDropTable.h"
#include "QuestItemTable.h"
#include "CPlayer.h"
#include "Npc.h"
#include "item.h"
#include "ItemManager.h"
#include "TriggerManager.h"
#include "NtlRandom.h"
#include "TriggerObject.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlResultCode.h"
#include "TableContainerManager.h"
#include "DojoManager.h"
#include "DungeonManager.h"
#include "ScriptAlgoObjectManager.h"
#include "WpsAlgoObject.h"
#include "QuestProgress.h"
#include "TimeQuest.h"
#include "TqsAlgoObject.h"
#include "DojoWar.h"


CQuest::CQuest()
{
	m_pMapQuestInProgress.clear();
	setQuestItems.clear();
	m_pkOwner = NULL;

	memset(m_stCQInfo.abyQCInfo, NULL, sizeof(m_stCQInfo.abyQCInfo));

	m_byLastSlot = 0;
	m_vErasedDungeonQuestIDs.clear();
	m_byGeneralQuestsCount = 0;
	m_bySpecialQuestsCount = 0;
}

CQuest::~CQuest()
{
	for (std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.begin(); it != m_pMapQuestInProgress.end(); it++)
	{
		CQuestProgress* pQuest = it->second;
		SAFE_DELETE(pQuest);
	}
	m_pMapQuestInProgress.clear();

	for (TQuestSet::iterator it = setQuestItems.begin(); it != setQuestItems.end(); it++)
	{
		sQUESTITEM_DATA* pItem = *it;
		SAFE_DELETE(pItem);
	}
	setQuestItems.clear();
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CQuest::Create(CPlayer* player)
{
	m_pkOwner = player;
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
CQuestProgress* CQuest::StartQuest(QUESTID tId, NTL_TS_TC_ID tcId)
{
	CQuestProgress* pQuestProgress = new CQuestProgress(m_pkOwner, tId, tcId, GetFreeSlotID());

	m_pMapQuestInProgress[tId] = pQuestProgress;

	UpdateQuestCounter(true, tId);

	return pQuestProgress;
}

//--------------------------------------------------------------------------------------//
//		ERASE PC QUEST
//--------------------------------------------------------------------------------------//
void CQuest::EraseQuest(QUESTID qId)
{
	std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.find(qId);
	if (it != m_pMapQuestInProgress.end())
	{
		CQuestProgress* pQuest = it->second;
		m_pMapQuestInProgress.erase(it);

		if (pQuest->GetWpsScriptID() != INVALID_DWORD && !IsDungeonQuestID(qId))	//if player erase escort quest or something so he leave the wps script | DOES NOT APPLY TO DUNGEON QUESTS
		{
			CWpsAlgoObject* pScript = (CWpsAlgoObject*)m_pkOwner->GetScript(pQuest->GetWpsScriptID());

			if (pScript)
			{
				ERR_LOG(LOG_USER, "Erase Quest (ID:%u,PreTcID:%u,CurTcID:%u) with WPS-Script connected", pQuest->GetProgressInfo()->tId, pQuest->GetProgressInfo()->uData.sQInfoV0.sMainTSP.tcPreId, pQuest->GetProgressInfo()->uData.sQInfoV0.sMainTSP.tcCurId);
				pScript->NotifyPlayerLeave(m_pkOwner->GetID());
			}

			pQuest->SetWpsScriptID(INVALID_DWORD);
		}

		// delete quest items
		sPROGRESS_QUEST_INFO* pProgressInfo = pQuest->GetProgressInfo();

		if (pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId != NTL_TS_TC_ID_INVALID)
		{
			CNtlTSTrigger* pTrig = (CNtlTSTrigger*)g_pTriggerManager->FindQuestFromTS(pProgressInfo->tId);

			CDboTSContGAct* pAct = (CDboTSContGAct*)pTrig->GetGroup(pProgressInfo->uData.sQInfoV0.tgExcCGroup)->GetChildCont(pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId);
			if (pAct != NULL)
			{
				CNtlTSEntity* pEntity = pAct->FindChildEntity(pProgressInfo->uData.sQInfoV0.sSToCEvtData.taId);
				if (pEntity != NULL)
				{
					CDboTSActSToCEvt* pEvt = (CDboTSActSToCEvt*)pEntity;
					const uSTOC_EVT_DATA& evtdata = pEvt->GetEvtData();

					switch (pEvt->GetEvtDataType())
					{
						case eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT:
						{
							for (int i = 0; i < evtdata.MAX_MOB_KILL; i++)
							{
								if (pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] > 0)
								{
									DeleteQuestItem(evtdata.sMobKillItemCnt[i].uiMobLIIdx, pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i]);
								}
							}
						}
						break;

						case eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM:
						{
							for (int i = 0; i < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM; i++)
							{
								if (evtdata.sDeliveryItemCnt[i].uiItemIdx != INVALID_TBLIDX && pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] != 0)
								{
									DeleteQuestItem(evtdata.sDeliveryItemCnt[i].uiItemIdx, evtdata.sDeliveryItemCnt[i].nItemCnt);
								}
							}
						}
						break;

						case eSTOC_EVT_DATA_TYPE_OBJECT_ITEM:
						{
							for (int i = 0; i < uSTOC_EVT_DATA::MAX_OBJECT_ITEM; i++)
							{
								if (evtdata.sObjectItemCnt[i].uiItemIdx != INVALID_TBLIDX && pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] != 0)
								{
									DeleteQuestItem(evtdata.sObjectItemCnt[i].uiItemIdx, evtdata.sObjectItemCnt[i].nItemCnt);
								}
							}
						}
						break;

						case eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT:
						{
							for (int i = 0; i < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM; i++)
							{
								if (evtdata.sPublicMobItemCnt[i].uiItemIdx != INVALID_TBLIDX && pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] != 0)
								{
									DeleteQuestItem(evtdata.sPublicMobItemCnt[i].uiItemIdx, evtdata.sPublicMobItemCnt[i].nItemCnt);
								}
							}
						}
						break;

						case eSTOC_EVT_DATA_TYPE_VISIT:
						{
							for (int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; i++)
							{
								if (evtdata.sVisitEvt[i].uiItemTblIdx != INVALID_TBLIDX && pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] != 0)
								{
									DeleteQuestItem(evtdata.sVisitEvt[i].uiItemTblIdx, 1);
								}
							}
						}
						break;

						default: break;
					}
				}
			}
		}

		if (IsDungeonQuestID(qId))
			m_vErasedDungeonQuestIDs.push_back(qId);

		RemoveSlotID(pQuest->GetProgressInfo()->bySlotId);

		UpdateQuestCounter(false, qId);
		
		SAFE_DELETE(pQuest);
	}
}


//--------------------------------------------------------------------------------------//
//		LOAD QUEST FROM DATABASE
//--------------------------------------------------------------------------------------//
void CQuest::StartQuest(sPROGRESS_QUEST_INFO* questinfo)
{
	CQuestProgress* pQuestProgress = new CQuestProgress(m_pkOwner);

	pQuestProgress->SetProgressInfo(questinfo);
	m_pMapQuestInProgress[questinfo->tId] = pQuestProgress;

	UpdateQuestCounter(true, questinfo->tId);
}


//--------------------------------------------------------------------------------------//
//		GET QUEST PROGRESS INFO
//--------------------------------------------------------------------------------------//
sPROGRESS_QUEST_INFO* CQuest::GetQuestProgressInfo(QUESTID tId)
{
	std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.find(tId);
	if (m_pMapQuestInProgress.end() == it)
		return NULL;

	CQuestProgress* pQuest = it->second;
	if (pQuest == NULL)
		return NULL;

	return pQuest->GetProgressInfo();
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
CQuestProgress* CQuest::GetQuestProgress(QUESTID tId)
{
	std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.find(tId);
	if (m_pMapQuestInProgress.end() == it)
		return NULL;

	return it->second;
}


//--------------------------------------------------------------------------------------//
//		UPDATE QUEST EVT
//--------------------------------------------------------------------------------------//
void CQuest::UpdateQuestEvt(TBLIDX SourceTblidx)
{
	CGameServer* app = (CGameServer*) g_pApp;
	for (std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.begin(); it != m_pMapQuestInProgress.end(); it++)
	{
		CQuestProgress* pQuest = it->second;
		if (pQuest)
		{
			sPROGRESS_QUEST_INFO* pProgressInfo = pQuest->GetProgressInfo();

			if (pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId != NTL_TS_TC_ID_INVALID)
			{
				CNtlTSTrigger* pTrig = (CNtlTSTrigger*)g_pTriggerManager->FindQuestFromTS(pProgressInfo->tId);

				CDboTSContGAct* pAct = (CDboTSContGAct*)pTrig->GetGroup(pProgressInfo->uData.sQInfoV0.tgExcCGroup)->GetChildCont(pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId);
				if(pAct != NULL)
				{
					CNtlTSEntity* pEntity = pAct->FindChildEntity(pProgressInfo->uData.sQInfoV0.sSToCEvtData.taId);
					if(pEntity != NULL)
					{
						CDboTSActSToCEvt* pEvt = (CDboTSActSToCEvt*)pEntity;
						const uSTOC_EVT_DATA& evtdata = pEvt->GetEvtData();

						if( pEvt->GetEvtDataType() == eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT) //KILL MONSTER
						{
							for(int i = 0; i < evtdata.MAX_MOB_KILL; i++)
							{
								if (pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] < (unsigned int)evtdata.sMobKillCnt[i].nMobCnt)
								{
									sMOB_TBLDAT* mob = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(SourceTblidx);
									if(mob)
									{
										if(mob->dwMobGroup == evtdata.sMobKillCnt[i].uiMobIdx)
										{
											pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] += 1;

											CNtlPacket packet(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
											sGU_QUEST_SVREVT_UPDATE_NFY * res = (sGU_QUEST_SVREVT_UPDATE_NFY *)packet.GetPacketData();
											res->wOpCode = GU_QUEST_SVREVT_UPDATE_NFY;
											res->taId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.taId;
											res->tcId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId;
											res->tId = pProgressInfo->tId;
											res->bySlot = i;
											res->bySvrEvtType = eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT;
											res->uEvtData.sMobKillCnt.nCurMobCnt = pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i];
											packet.SetPacketLen(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
											g_pApp->Send(m_pkOwner->GetClientSessionID(), &packet);

											break;
										}
									}
								}
							}
						}
						else if( pEvt->GetEvtDataType() == eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT) // KILL MONSTER & RECEIVE ITEM
						{
							for(int i = 0; i < evtdata.MAX_MOB_KILL_ITEM; i++)
							{
								if (pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] < (unsigned int)evtdata.sMobKillItemCnt[i].nMobLICnt)
								{
									sMOB_TBLDAT* mob = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(SourceTblidx);
									if(mob)
									{
										sQUEST_DROP_TBLDAT* qDropTbldat = (sQUEST_DROP_TBLDAT*)g_pTableContainer->GetQuestDropTable()->FindData( mob->dropQuestTblidx );
										if(qDropTbldat)
										{
											for(int e = 0; e < QUEST_ITEM_DROP_MAX_COUNT; e++)
											{
												if(qDropTbldat->aQuestItemTblidx[e] == evtdata.sMobKillItemCnt[i].uiMobLIIdx)
												{
													if (Dbo_CheckProbabilityF(qDropTbldat->aDropRate[e] * 100.f) == true)
													{
														if (CreateQuestItem(evtdata.sMobKillItemCnt[i].uiMobLIIdx, 1))
														{
															pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] += 1;

															CNtlPacket packet(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
															sGU_QUEST_SVREVT_UPDATE_NFY * res = (sGU_QUEST_SVREVT_UPDATE_NFY *)packet.GetPacketData();
															res->wOpCode = GU_QUEST_SVREVT_UPDATE_NFY;
															res->taId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.taId;
															res->tcId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId;
															res->tId = pProgressInfo->tId;
															res->bySlot = i;
															res->bySvrEvtType = eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT;
															res->uEvtData.sMobKillItemCnt.nCurMobLICnt = pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i];
															packet.SetPacketLen(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
															g_pApp->Send(m_pkOwner->GetClientSessionID(), &packet);
														}
														break;
													}
												}
											}
										}
									}
								}
							}
						}
						else if (pEvt->GetEvtDataType() == eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT) //kill any mob which has a +/- X lv difference
						{
							if (pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[0] < (unsigned int)evtdata.sPublicMobItemCnt[0].nItemCnt)
							{
								if (Dbo_CheckProbabilityF(evtdata.sPublicMobItemCnt[0].fDropRate * 100.f))
								{
									if (evtdata.sPublicMobItemCnt[0].uiRequireItemIdx != INVALID_TBLIDX)
									{
										CItem* pRequiredItem = m_pkOwner->GetPlayerItemContainer()->GetItemByIdx(evtdata.sPublicMobItemCnt[0].uiRequireItemIdx);
										if (pRequiredItem == NULL)
										{
											return;
										}
									}

									sMOB_TBLDAT* pMobTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(SourceTblidx);
									if (pMobTbldat)
									{
										if (m_pkOwner->GetLevel() <= pMobTbldat->byLevel + pEvt->GetEvtSubCondData().sLevelDiffLess.nLevelDiff
											&& m_pkOwner->GetLevel() >= pMobTbldat->byLevel - pEvt->GetEvtSubCondData().sLevelDiffLess.nLevelDiff) //check level difference
										{
											pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[0] += 1;

											CNtlPacket packet(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
											sGU_QUEST_SVREVT_UPDATE_NFY * res = (sGU_QUEST_SVREVT_UPDATE_NFY *)packet.GetPacketData();
											res->wOpCode = GU_QUEST_SVREVT_UPDATE_NFY;
											res->taId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.taId;
											res->tcId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId;
											res->tId = pProgressInfo->tId;
											res->bySlot = 0;
											res->bySvrEvtType = eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT;
											res->uEvtData.sCustomEvtCnt.nCurCnt = pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[0];
											packet.SetPacketLen(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
											g_pApp->Send(m_pkOwner->GetClientSessionID(), &packet);
										}
									}
								}
							}
						}
						else if (pEvt->GetEvtDataType() == eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT)
						{
							for (int i = 0; i < evtdata.MAX_MOB_KILL; i++)
							{
								if (evtdata.sCustomEvtCnt[i].uiCustomEvtID == SourceTblidx)
								{
									if (pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] < (unsigned int)evtdata.sCustomEvtCnt[i].nMaxCnt)
									{
										pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] += 1;

										CNtlPacket packet(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
										sGU_QUEST_SVREVT_UPDATE_NFY * res = (sGU_QUEST_SVREVT_UPDATE_NFY *)packet.GetPacketData();
										res->wOpCode = GU_QUEST_SVREVT_UPDATE_NFY;
										res->taId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.taId;
										res->tcId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId;
										res->tId = pProgressInfo->tId;
										res->bySlot = i;
										res->bySvrEvtType = eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT;
										res->uEvtData.sCustomEvtCnt.nCurCnt = pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i];
										packet.SetPacketLen(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
										g_pApp->Send(m_pkOwner->GetClientSessionID(), &packet);

										return;
									}
								}
							}
						}

						else if( pEvt->GetEvtDataType() ==  eSTOC_EVT_DATA_TYPE_VISIT) // 
						{
						//	//NTL_PRINT(PRINT_APP,"MobId %d nCurMobLICnt %d nMobLICnt %d uiMobLIIdx %d\n",MobId, evtdata.sMobKillItemCnt->nCurMobLICnt, evtdata.sMobKillItemCnt->nMobLICnt, evtdata.sMobKillItemCnt->uiMobLIIdx);
							for(int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; i++)
							{
								if (m_pkOwner->GetWorldTblidx() == pEvt->GetEvtData().sVisitEvt[i].uiWorldTblIdx)
								{
									if (pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] == 0)
									{
										if( pEvt->GetEvtData().sVisitEvt[i].byObjType == OBJTYPE_NPC || pEvt->GetEvtData().sVisitEvt[i].byObjType == OBJTYPE_TOBJECT )
										{
											if(pEvt->GetEvtData().sVisitEvt[i].uiObjTblIdx == SourceTblidx)
											{
												pProgressInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] = SourceTblidx;

												CNtlPacket packet(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
												sGU_QUEST_SVREVT_UPDATE_NFY * res = (sGU_QUEST_SVREVT_UPDATE_NFY *)packet.GetPacketData();
												res->wOpCode = GU_QUEST_SVREVT_UPDATE_NFY;
												res->taId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.taId;
												res->tcId = pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId;
												res->tId = pProgressInfo->tId;
												res->bySlot = i;
												res->bySvrEvtType = eSTOC_EVT_DATA_TYPE_VISIT;
												res->uEvtData.sVisitEvt.bCompleted = true;
												packet.SetPacketLen(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
												g_pApp->Send(m_pkOwner->GetClientSessionID(), &packet);
				
												break;
											}
										}
									}
								}
							}
						}
					}
					else
					{
						ERR_LOG(LOG_QUEST, "QUEST UpdateQuestEvt FindChildEntity Faile taid %d tId %d ", pProgressInfo->uData.sQInfoV0.sSToCEvtData.taId, pProgressInfo->tId);
						pProgressInfo->uData.sQInfoV0.tcQuestInfo = pAct->GetErrorLinkID();
						break;
					}
				}
				else
				{
					ERR_LOG(LOG_QUEST, "QUEST UpdateQuestEvt CDboTSContGAct Failed tcId %d tId %d ", pProgressInfo->uData.sQInfoV0.sSToCEvtData.tcId, pProgressInfo->tId);
					break;
				}
				
			}
		}
	}
}

//--------------------------------------------------------------------------------------//
//		UPDATE QUEST / TS
//--------------------------------------------------------------------------------------//
void CQuest::Update(DWORD dwTickDiff)
{
	for (std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.begin(); it != m_pMapQuestInProgress.end(); it++)
	{
		CQuestProgress* pQuest = it->second;
		if (pQuest)
		{
			pQuest->TickProcess(dwTickDiff);
		}
	}
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
BYTE CQuest::GetFreeSlotID()
{
	HOBJECT bySlot = INVALID_BYTE;

	if (m_vSlotVec.size() > 0)
	{
		bySlot = m_vSlotVec.front();
		m_vSlotVec.erase(m_vSlotVec.begin());
	}
	else
		bySlot = m_byLastSlot++;

	return bySlot;
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CQuest::RemoveSlotID(BYTE bySlot)
{
	m_vSlotVec.push_back(bySlot);
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CQuest::SaveQuestProgress()
{
	CGameServer* app = (CGameServer*)g_pApp;

	for (std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.begin(); it != m_pMapQuestInProgress.end(); it++)
	{
		CQuestProgress* pQuest = it->second;
		if (pQuest)
		{
			sPROGRESS_QUEST_INFO* quest = pQuest->GetProgressInfo();

			if (!CanStoreQuestInDatabase(quest->tId)) //only normal quests can be stored in database !
				continue;

			if (quest->uData.sQInfoV0.wQState != eTS_SVR_STATE_ERROR)
			{
				CNtlPacket pQry(sizeof(sGQ_QUEST_PROGRESS_DATA_CREATE_REQ));
				sGQ_QUEST_PROGRESS_DATA_CREATE_REQ * rQry = (sGQ_QUEST_PROGRESS_DATA_CREATE_REQ *)pQry.GetPacketData();
				rQry->wOpCode = GQ_QUEST_PROGRESS_DATA_CREATE_REQ;
				rQry->charId = m_pkOwner->GetCharID();
				rQry->handle = m_pkOwner->GetID();
				memcpy(&rQry->progressInfo, quest, sizeof(sPROGRESS_QUEST_INFO));
				rQry->bIsComplete = false;
				pQry.SetPacketLen(sizeof(sGQ_QUEST_PROGRESS_DATA_CREATE_REQ));
				app->SendTo(app->GetQueryServerSession(), &pQry);
			}
		}
	}
}



//--------------------------------------------------------------------------------------//
//		ADD QUEST ITEM
//--------------------------------------------------------------------------------------//
void	CQuest::AddQuestItemData(sQUESTITEM_DATA* data)
{
	sQUESTITEM_DATA* newdata = new sQUESTITEM_DATA;
	memcpy(newdata,data,sizeof(sQUESTITEM_DATA));

	setQuestItems.insert(newdata);
}
void	CQuest::AddQuestItemData(TBLIDX itemid, BYTE count, BYTE pos)
{
	sQUESTITEM_DATA* newdata = new sQUESTITEM_DATA;
	newdata->byCurStackCount = count;
	newdata->byPos = pos;
	newdata->tblidx = itemid;

	setQuestItems.insert(newdata);
}
//--------------------------------------------------------------------------------------//
//		REMOVE QUEST ITEM
//--------------------------------------------------------------------------------------//
void	CQuest::RemQuestItemData(BYTE byPos)
{
	for (TQuestSet::iterator it = setQuestItems.begin(); it != setQuestItems.end(); it++)
	{
		sQUESTITEM_DATA* item = *it;
		if (item->byPos == byPos)
		{
			setQuestItems.erase(it);
			delete item;
			break;
		}
	}
}
//--------------------------------------------------------------------------------------//
//		GET QUEST ITEM DATA
//--------------------------------------------------------------------------------------//
sQUESTITEM_DATA*	CQuest::GetQuestItemData(BYTE byPos)
{
	for (TQuestSet::iterator it = setQuestItems.begin(); it != setQuestItems.end(); it++)
	{
		sQUESTITEM_DATA* item = *it;
		if (item->byPos == byPos)
			return item;
	}

	return NULL;
}
//--------------------------------------------------------------------------------------//
//		GET FREE QUEST INVENTORY POS
//--------------------------------------------------------------------------------------//
BYTE	CQuest::GetFreeQuestInvPos()
{
	for(BYTE pos = 0; pos < NTL_QUEST_INVENTORY_SLOT_COUNT; pos++)
	{
		if(GetQuestItemData(pos) == NULL)
			return pos;
	}

	return INVALID_BYTE;
}
//--------------------------------------------------------------------------------------//
//		RETURN ITEM POS
//--------------------------------------------------------------------------------------//
BYTE	CQuest::ReturnItemPos(TBLIDX ItemTblidx)
{
	for (TQuestSet::iterator it = setQuestItems.begin(); it != setQuestItems.end(); it++)
	{
		sQUESTITEM_DATA* qItem = *it;
		if(qItem->tblidx == ItemTblidx)
			return qItem->byPos;
	}

	return INVALID_BYTE;
}
//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
sQUESTITEM_DATA*	CQuest::GetQuestItemData(TBLIDX ItemTblidx)
{
	for (TQuestSet::iterator it = setQuestItems.begin(); it != setQuestItems.end(); it++)
	{
		sQUESTITEM_DATA* qItem = *it;
		if(qItem->tblidx == ItemTblidx)
			return qItem;
	}

	return NULL;
}
//--------------------------------------------------------------------------------------//
//		CREATE QUEST ITEM. Return true on update and create. return false if create/update failed
//--------------------------------------------------------------------------------------//
bool	CQuest::CreateQuestItem(TBLIDX ItemTblidx, BYTE byCount)
{
	CGameServer* app = (CGameServer*) g_pApp;
	BYTE byUpdateType = eQUEST_ITEM_UPDATE_TYPE_CREATE;
	
	sQUESTITEM_TBLDAT* pQuestItemTbldat = (sQUESTITEM_TBLDAT*)g_pTableContainer->GetQuestItemTable()->FindData(ItemTblidx);
	if (pQuestItemTbldat == NULL)
		return false;

	sQUESTITEM_DATA* qItem = GetQuestItemData(ItemTblidx);
	if (qItem)
	{
		if (qItem->byCurStackCount + byCount < 255) //Quest items max stack is 254
			byUpdateType = eQUEST_ITEM_UPDATE_TYPE_UPDATE;
	}

	if (byUpdateType == eQUEST_ITEM_UPDATE_TYPE_UPDATE)
	{
		qItem->byCurStackCount += byCount; //update map

		CNtlPacket pQry(sizeof(sGQ_QUEST_ITEM_CREATE_REQ));
		sGQ_QUEST_ITEM_CREATE_REQ * rQry = (sGQ_QUEST_ITEM_CREATE_REQ *)pQry.GetPacketData();
		rQry->wOpCode = GQ_QUEST_ITEM_CREATE_REQ;
		rQry->charId = m_pkOwner->GetCharID();
		rQry->handle = m_pkOwner->GetID();
		rQry->aItems.byUpdateType = eQUEST_ITEM_UPDATE_TYPE_UPDATE;
		rQry->aItems.byCurCount = qItem->byCurStackCount;
		rQry->aItems.byPos = qItem->byPos;
		rQry->aItems.itemTblidx = qItem->tblidx;
		pQry.SetPacketLen(sizeof(sGQ_QUEST_ITEM_CREATE_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);


		CNtlPacket packet(sizeof(sGU_QUEST_ITEM_UPDATE_NFY));
		sGU_QUEST_ITEM_UPDATE_NFY * res = (sGU_QUEST_ITEM_UPDATE_NFY *)packet.GetPacketData();
		res->wOpCode = GU_QUEST_ITEM_UPDATE_NFY;
		res->byCurCount = qItem->byCurStackCount;
		res->byPos = qItem->byPos;
		packet.SetPacketLen(sizeof(sGU_QUEST_ITEM_UPDATE_NFY));
		app->Send(m_pkOwner->GetClientSessionID(), &packet);

		return true;
	}
	else
	{
		BYTE byPos = GetFreeQuestInvPos();
		
		if (byPos == INVALID_BYTE)
			return false;

		CNtlPacket pQry(sizeof(sGQ_QUEST_ITEM_CREATE_REQ));
		sGQ_QUEST_ITEM_CREATE_REQ * rQry = (sGQ_QUEST_ITEM_CREATE_REQ *)pQry.GetPacketData();
		rQry->wOpCode = GQ_QUEST_ITEM_CREATE_REQ;
		rQry->charId = m_pkOwner->GetCharID();
		rQry->handle = m_pkOwner->GetID();
		rQry->aItems.byUpdateType = eQUEST_ITEM_UPDATE_TYPE_CREATE;
		rQry->aItems.byCurCount = byCount;
		rQry->aItems.byPos = byPos;
		rQry->aItems.itemTblidx = ItemTblidx;
		pQry.SetPacketLen(sizeof(sGQ_QUEST_ITEM_CREATE_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);

		AddQuestItemData(ItemTblidx, byCount, byPos);

		CNtlPacket packet(sizeof(sGU_QUEST_ITEM_CREATE_NFY));
		sGU_QUEST_ITEM_CREATE_NFY * res = (sGU_QUEST_ITEM_CREATE_NFY *)packet.GetPacketData();
		res->wOpCode = GU_QUEST_ITEM_CREATE_NFY;
		res->byCurCount = byCount;
		res->byPos = byPos;
		res->qItemTblidx = ItemTblidx;
		packet.SetPacketLen(sizeof(sGU_QUEST_ITEM_CREATE_NFY));
		g_pApp->Send(m_pkOwner->GetClientSessionID(), &packet);

		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------------//
//		DELETE QUEST ITEM
//--------------------------------------------------------------------------------------//
void	CQuest::DeleteQuestItem(BYTE byPos)
{
	if (byPos == INVALID_BYTE)
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGU_QUEST_ITEM_DELETE_RES));
	sGU_QUEST_ITEM_DELETE_RES * res = (sGU_QUEST_ITEM_DELETE_RES *)packet.GetPacketData();
	res->wOpCode = GU_QUEST_ITEM_DELETE_RES;
	res->byDeletePos = byPos;
	res->wResultCode = GAME_SUCCESS;
	packet.SetPacketLen(sizeof(sGU_QUEST_ITEM_DELETE_RES));
	app->Send(m_pkOwner->GetClientSessionID(), &packet);

	CNtlPacket packet2(sizeof(sGU_QUEST_ITEM_DELETE_NFY));
	sGU_QUEST_ITEM_DELETE_NFY * res2 = (sGU_QUEST_ITEM_DELETE_NFY *)packet2.GetPacketData();
	res2->wOpCode = GU_QUEST_ITEM_DELETE_NFY;
	res2->byDeletePos = byPos;
	packet2.SetPacketLen(sizeof(sGU_QUEST_ITEM_DELETE_NFY));
	app->Send(m_pkOwner->GetClientSessionID(), &packet2);

	CNtlPacket pQry(sizeof(sGQ_QUEST_ITEM_DELETE_REQ));
	sGQ_QUEST_ITEM_DELETE_REQ * rQry = (sGQ_QUEST_ITEM_DELETE_REQ *)pQry.GetPacketData();
	rQry->wOpCode = GQ_QUEST_ITEM_DELETE_REQ;
	rQry->charId = m_pkOwner->GetCharID();
	rQry->handle = m_pkOwner->GetID();
	rQry->aItems.byUpdateType = eQUEST_ITEM_UPDATE_TYPE_DELETE;
	rQry->aItems.byPos = byPos;
	pQry.SetPacketLen(sizeof(sGQ_QUEST_ITEM_DELETE_REQ));
	app->SendTo(app->GetQueryServerSession(), &pQry);

	RemQuestItemData(byPos);
}
void	CQuest::DeleteQuestItem(TBLIDX ItemTblidx, int nCount)
{
	CGameServer* app = (CGameServer*)g_pApp;

	int itemdelcount = nCount;
	TQuestSet::iterator it = setQuestItems.begin();

	int nLoopCount = 0;

	while (it != setQuestItems.end())
	{
		++nLoopCount;

		if (nLoopCount > 1000)
		{
			ERR_LOG(LOG_SYSTEM,"INFINITE LOOP FOUND");
		}

		sQUESTITEM_DATA* qItem = *it;
		if (qItem && qItem->tblidx == ItemTblidx && itemdelcount > 0)
		{
			if (itemdelcount >= qItem->byCurStackCount) //delete all
			{
				CNtlPacket pQry(sizeof(sGQ_QUEST_ITEM_DELETE_REQ));
				sGQ_QUEST_ITEM_DELETE_REQ * rQry = (sGQ_QUEST_ITEM_DELETE_REQ *)pQry.GetPacketData();
				rQry->wOpCode = GQ_QUEST_ITEM_DELETE_REQ;
				rQry->charId = m_pkOwner->GetCharID();
				rQry->handle = m_pkOwner->GetID();
				rQry->aItems.byUpdateType = eQUEST_ITEM_UPDATE_TYPE_DELETE;
				rQry->aItems.byPos = qItem->byPos;
				pQry.SetPacketLen(sizeof(sGQ_QUEST_ITEM_DELETE_REQ));
				app->SendTo(app->GetQueryServerSession(), &pQry);

				CNtlPacket packet2(sizeof(sGU_QUEST_ITEM_DELETE_NFY));
				sGU_QUEST_ITEM_DELETE_NFY * res2 = (sGU_QUEST_ITEM_DELETE_NFY *)packet2.GetPacketData();
				res2->wOpCode = GU_QUEST_ITEM_DELETE_NFY;
				res2->byDeletePos = qItem->byPos;
				packet2.SetPacketLen(sizeof(sGU_QUEST_ITEM_DELETE_NFY));
				app->Send(m_pkOwner->GetClientSessionID(), &packet2);

				itemdelcount -= qItem->byCurStackCount;
				it = setQuestItems.erase(it);
				SAFE_DELETE(qItem);
			}
			else if (itemdelcount < qItem->byCurStackCount) //update stack
			{
				qItem->byCurStackCount -= itemdelcount; //update map

				CNtlPacket pQry(sizeof(sGQ_QUEST_ITEM_CREATE_REQ));
				sGQ_QUEST_ITEM_CREATE_REQ * rQry = (sGQ_QUEST_ITEM_CREATE_REQ *)pQry.GetPacketData();
				rQry->wOpCode = GQ_QUEST_ITEM_CREATE_REQ;
				rQry->charId = m_pkOwner->GetCharID();
				rQry->handle = m_pkOwner->GetID();
				rQry->aItems.byUpdateType = eQUEST_ITEM_UPDATE_TYPE_UPDATE;
				rQry->aItems.byCurCount = qItem->byCurStackCount;
				rQry->aItems.byPos = qItem->byPos;
				rQry->aItems.itemTblidx = qItem->tblidx;
				pQry.SetPacketLen(sizeof(sGQ_QUEST_ITEM_CREATE_REQ));
				app->SendTo(app->GetQueryServerSession(), &pQry);


				CNtlPacket packet(sizeof(sGU_QUEST_ITEM_UPDATE_NFY));
				sGU_QUEST_ITEM_UPDATE_NFY * res = (sGU_QUEST_ITEM_UPDATE_NFY *)packet.GetPacketData();
				res->wOpCode = GU_QUEST_ITEM_UPDATE_NFY;
				res->byCurCount = qItem->byCurStackCount;
				res->byPos = qItem->byPos;
				packet.SetPacketLen(sizeof(sGU_QUEST_ITEM_UPDATE_NFY));
				app->Send(m_pkOwner->GetClientSessionID(), &packet);

				itemdelcount = 0;
				break;
			}
			else ++it;
		}
		else ++it;
	}
}

//--------------------------------------------------------------------------------------//
//		MOVE QUEST ITEM
//--------------------------------------------------------------------------------------//
void	CQuest::MoveQuestItem(BYTE byPos, BYTE byNewPos)
{
	CGameServer* app = (CGameServer*) g_pApp;
	sQUESTITEM_DATA* destitem = this->GetQuestItemData(byNewPos);
	sQUESTITEM_DATA* srcitem = this->GetQuestItemData(byPos);

	if(srcitem == NULL)
		return;

	if(destitem == NULL) //if dest item null then we only move the item
	{
		CNtlPacket pQry(sizeof(sGQ_QUEST_ITEM_MOVE_REQ));
		sGQ_QUEST_ITEM_MOVE_REQ * rQry = (sGQ_QUEST_ITEM_MOVE_REQ *)pQry.GetPacketData();
		rQry->wOpCode = GQ_QUEST_ITEM_MOVE_REQ;
		rQry->charId = m_pkOwner->GetCharID();
		rQry->handle = m_pkOwner->GetID();
		rQry->byDestPos = byNewPos;
		rQry->bySrcPos = byPos;
		rQry->dwSrcTblidx = srcitem->tblidx;
		pQry.SetPacketLen(sizeof(sGQ_QUEST_ITEM_MOVE_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);

		CNtlPacket packet(sizeof(sGU_QUEST_ITEM_MOVE_RES));
		sGU_QUEST_ITEM_MOVE_RES * res = (sGU_QUEST_ITEM_MOVE_RES *)packet.GetPacketData();
		res->wOpCode = GU_QUEST_ITEM_MOVE_RES;
		res->wResultCode = GAME_SUCCESS;
		res->byDestPos = byNewPos;
		res->bySrcPos = byPos;
		res->dwDestTblidx = INVALID_TBLIDX;
		res->dwSrcTblidx = srcitem->tblidx;
		packet.SetPacketLen(sizeof(sGU_QUEST_ITEM_MOVE_RES));
		app->Send(m_pkOwner->GetClientSessionID(), &packet);

		srcitem->byPos = byNewPos;
	}
	else //else dest item NOT NULL. We switch item.
	{
		CNtlPacket pQry(sizeof(sGQ_QUEST_ITEM_MOVE_REQ));
		sGQ_QUEST_ITEM_MOVE_REQ * rQry = (sGQ_QUEST_ITEM_MOVE_REQ *)pQry.GetPacketData();
		rQry->wOpCode = GQ_QUEST_ITEM_CREATE_REQ;
		rQry->charId = m_pkOwner->GetCharID();
		rQry->handle = m_pkOwner->GetID();
		rQry->byDestPos = byNewPos;
		rQry->bySrcPos = byPos;
		rQry->dwSrcTblidx = srcitem->tblidx;
		rQry->dwDestTblidx = destitem->tblidx;
		pQry.SetPacketLen(sizeof(sGQ_QUEST_ITEM_MOVE_REQ));
		app->SendTo(app->GetQueryServerSession(), &pQry);

		CNtlPacket packet(sizeof(sGU_QUEST_ITEM_MOVE_RES));
		sGU_QUEST_ITEM_MOVE_RES * res = (sGU_QUEST_ITEM_MOVE_RES *)packet.GetPacketData();
		res->wOpCode = GU_QUEST_ITEM_MOVE_RES;
		res->wResultCode = GAME_SUCCESS;
		res->byDestPos = byNewPos;
		res->bySrcPos = byPos;
		res->dwDestTblidx = destitem->tblidx;
		res->dwSrcTblidx = srcitem->tblidx;
		packet.SetPacketLen(sizeof(sGU_QUEST_ITEM_MOVE_RES));
		app->Send(m_pkOwner->GetClientSessionID(), &packet);

		srcitem->byPos = byNewPos;
		destitem->byPos = byPos;
	}
}


//--------------------------------------------------------------------------------------//
//		UPDATE QUEST EVT // CALLED ON DBO_ACT_TYPE_ID_ACT_QITEM
//--------------------------------------------------------------------------------------//
WORD CQuest::UpdateQuestObjectItemEvt(QUESTID qId, TBLIDX ItemTblidx, BYTE byCount)
{
	sPROGRESS_QUEST_INFO* quest = GetQuestProgressInfo(qId);
	if (quest)
	{
		CNtlTSTrigger* pTrig = (CNtlTSTrigger*)g_pTriggerManager->FindQuestFromTS(quest->tId);
		if (pTrig)
		{
			CDboTSContGAct* pAct = (CDboTSContGAct*)pTrig->GetGroup(quest->uData.sQInfoV0.tgExcCGroup)->GetChildCont(quest->uData.sQInfoV0.sSToCEvtData.tcId); //check if create item by stoc event
			if (pAct)
			{
				CNtlTSEntity* pEntity = pAct->FindChildEntity(quest->uData.sQInfoV0.sSToCEvtData.taId);
				if (pEntity)
				{
					CDboTSActSToCEvt* pEvt = (CDboTSActSToCEvt*)pEntity;
					const uSTOC_EVT_DATA& evtdata = pEvt->GetEvtData();

					for (int i = 0; i < evtdata.MAX_OBJECT_ITEM; i++)
					{
						if (evtdata.sObjectItemCnt[i].uiItemIdx == ItemTblidx)
						{
							if (quest->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] >= (unsigned int)evtdata.sObjectItemCnt[i].nItemCnt)
								return GAME_TS_WARNING_ALREADY_VISITED;

							quest->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] += (unsigned int)byCount;

							BYTE byCreateCnt = byCount;

							if (quest->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] > (unsigned int)evtdata.sObjectItemCnt[i].nItemCnt)
							{
								byCreateCnt = quest->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] - (unsigned int)evtdata.sObjectItemCnt[i].nItemCnt;

								quest->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] = (unsigned int)evtdata.sObjectItemCnt[i].nItemCnt;
							}

							if (CreateQuestItem(ItemTblidx, byCreateCnt))
							{
								CNtlPacket packet(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
								sGU_QUEST_SVREVT_UPDATE_NFY * res = (sGU_QUEST_SVREVT_UPDATE_NFY *)packet.GetPacketData();
								res->wOpCode = GU_QUEST_SVREVT_UPDATE_NFY;
								res->taId = quest->uData.sQInfoV0.sSToCEvtData.taId;
								res->tcId = quest->uData.sQInfoV0.sSToCEvtData.tcId;
								res->tId = quest->tId;
								res->bySlot = i;
								res->bySvrEvtType = eSTOC_EVT_DATA_TYPE_OBJECT_ITEM;
								res->uEvtData.sObjectItemCnt.nCurItemCnt = (int)quest->uData.sQInfoV0.sSToCEvtData.m_aUserData[i];
								packet.SetPacketLen(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
								m_pkOwner->SendPacket(&packet);

								return RESULT_SUCCESS;
							}
							else
							{
								quest->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] = UnsignedSafeDecrease<UINT32>(quest->uData.sQInfoV0.sSToCEvtData.m_aUserData[i], (UINT32)byCreateCnt);
								return GAME_TS_WARNING_QUEST_INVENTORY_IS_FULL;
							}
						}
					}
				}
			}
			else if (quest->uData.sQInfoV0.sSToCEvtData.tcId == NTL_TS_TC_ID_INVALID) //If we dont create item by stoc event
			{
				if (CreateQuestItem(ItemTblidx, byCount))
				{
					return RESULT_SUCCESS;
				}
				else
				{
					return GAME_TS_WARNING_QUEST_INVENTORY_IS_FULL;
				}
			}
		}
	}

	return RESULT_FAIL;
}


//--------------------------------------------------------------------------------------//
//		ADD COMPLETED QUEST
//--------------------------------------------------------------------------------------//
void	CQuest::SetQuestCompleteInfo(const sCOMPLETE_QUEST_INFO& stCQInfo)
{
	memcpy( &m_stCQInfo, &stCQInfo, sizeof(stCQInfo) );
}
//--------------------------------------------------------------------------------------//
//		CHECK IF QUEST HAS BEEN FINISHED
//--------------------------------------------------------------------------------------//
bool	CQuest::HasBeenClearQuest( QUESTID tId )
{
	if ( tId >= eQUEST_ID_RANGE_QUEST_MAX )
	{
		//NTL_PRINT(PRINT_APP,"Over the max quest id. Info[%d]", tId );
		return true;
	}
	
	int nShift = (tId % eCOMPLETE_QUEST_QUEST_PER_BYTE) * eCOMPLETE_QUEST_STATE_MAX;

	if (m_stCQInfo.abyQCInfo[tId / eCOMPLETE_QUEST_QUEST_PER_BYTE] & (eCOMPLETE_QUEST_STATE_CLEAR << nShift) )
	{
		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------------//
//		SET QUEST COMPLETED
//--------------------------------------------------------------------------------------//
void CQuest::SetClearQuest( QUESTID tId )
{
	if ( tId >= eQUEST_ID_RANGE_QUEST_MAX )
	{
		//NTL_PRINT(PRINT_APP,"Over the max quest id. Info[%d]", tId );
		return;
	}

	if (HasBeenClearQuest(tId))
		return;

	int nShift = (tId % eCOMPLETE_QUEST_QUEST_PER_BYTE) * eCOMPLETE_QUEST_STATE_MAX;

	BIT_FLAG_SET(m_stCQInfo.abyQCInfo[tId / eCOMPLETE_QUEST_QUEST_PER_BYTE], eCOMPLETE_QUEST_STATE_CLEAR << nShift);
}

void CQuest::UnsetClearQuest(QUESTID tId)
{
	int nShift = (tId % eCOMPLETE_QUEST_QUEST_PER_BYTE) * eCOMPLETE_QUEST_STATE_MAX;

	BIT_FLAG_UNSET(m_stCQInfo.abyQCInfo[tId / eCOMPLETE_QUEST_QUEST_PER_BYTE], eCOMPLETE_QUEST_STATE_CLEAR << nShift);
}


//--------------------------------------------------------------------------------------//
//		CHECK QUEST ENTITY
//--------------------------------------------------------------------------------------//
WORD CQuest::ProgressTsEntity(CNtlTSEntity* pEntity, NTL_TS_T_ID tId, CQuestProgress* pQuestProgress, sOBJTRIGGER* pTrigger/* = NULL*/)
{
	//NTL_PRINT(PRINT_APP, "ENTITY-PROCESS: %s tId %d -", pEntity->GetClassNameA(), tId);

	sPROGRESS_QUEST_INFO* qinfo = NULL;

	if (pQuestProgress)
	{
		qinfo = pQuestProgress->GetProgressInfo();
	}

	switch(pEntity->GetEntityType())
	{
		case DBO_COND_TYPE_ID_CHECK_LVL: //Check pc level
		{
			int nLvl = m_pkOwner->GetLevel();
			CDboTSCheckLvl* pLvl = (CDboTSCheckLvl*)pEntity;

			if (nLvl < pLvl->GetMinLvl() || nLvl > pLvl->GetMaxLvl()) //check if pc lower than min lv or higher than max lv
				return GAME_TS_WARNING_WRONG_CONDITION;
						
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_PCCLS: //check pc class
		{
			CDboTSCheckPCCls* pClass = (CDboTSCheckPCCls*)pEntity;

			if (!(pClass->GetClsFlags() & (1 << m_pkOwner->GetClass())))
			{
				ERR_LOG(LOG_QUEST, "Quest %u class check failed %u and %u", tId, pClass->GetClsFlags(), 1 << m_pkOwner->GetClass());
				return GAME_TS_WARNING_WRONG_CONDITION;
			}
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_PCRACE: //check pc race
		{
			CDboTSCheckPCRace* pRace = (CDboTSCheckPCRace*)pEntity;

			if (!(pRace->GetRaceFlags() & (1 << m_pkOwner->GetRace())))
			{
				ERR_LOG(LOG_QUEST, "Quest %u race check failed %u and %u", tId, pRace->GetRaceFlags(), 1 << m_pkOwner->GetRace());
				return GAME_TS_WARNING_WRONG_CONDITION;
			}
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_QITEM: //check if pc got quest item
		{
			CDboTSCheckQItem* pQItem = (CDboTSCheckQItem*)pEntity;
			int nSum = 0;
			for ( BYTE i = 0; i < NTL_QUEST_INVENTORY_SLOT_COUNT; i++ )
			{
				sQUESTITEM_DATA* qitemdaa = m_pkOwner->GetQuests()->GetQuestItemData( i );
				if ( qitemdaa )
				{
					if ( qitemdaa->tblidx == pQItem->GetItemIdx() )
						nSum += qitemdaa->byCurStackCount;
				}
			}
			
			if (CheckQuestItem(pQItem->GetCheckType(), nSum, pQItem->GetItemCnt()) == false)
				return GAME_TS_WARNING_WRONG_CONDITION;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_CLRQST: //check clear quest
		{
			CDboTSCheckClrQst* pClrQst = (CDboTSCheckClrQst*)pEntity;

			/*info
				"not" is in every quest 0
			*/

			//Temporary disabled.
			

		//	NTL_PRINT(PRINT_APP,"QUEST-START: DBO_COND_TYPE_ID_CHECK_CLRQST GetNumOfNextQidLink %d", pClrQst->GetNumOfNextQidLink());
			std::vector<NTL_TS_T_ID>::const_iterator it;

			//check if previous quests
			const std::vector<NTL_TS_T_ID>& AndList = pClrQst->GetAndIdList();
			if ( !AndList.empty() )
			{
				bool bCleared = true;
				for ( it = AndList.begin(); it != AndList.end(); it++ )
				{
					bCleared = this->HasBeenClearQuest( *it );
						
					if (!bCleared) //check if quest XX is not completed
					{
						return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
			}

			// check next quests | important: the client does not do this check but on server we must check it.
			if(pClrQst->GetFlink() == TRUE)
			{
				if (pClrQst->GetNumOfNextQidLink() > 0)
				{
					bool bNextCleared = false;
					for (int i = 0; i < pClrQst->GetNumOfNextQidLink(); i++)
					{
						if (i == 0)
						{
							bNextCleared = this->HasBeenClearQuest(pClrQst->BeginNextQidLink());

						//	if(bNextCleared)
						//		printf("A Qid %u already cleared... wtf? \n", pClrQst->BeginNextQidLink());
						}
						else
						{
							bNextCleared = this->HasBeenClearQuest(pClrQst->NextNextQidLink());

						//	if (bNextCleared)
						//		printf("B Qid %u already cleared... wtf? \n", pClrQst->NextNextQidLink());
						}
						if (bNextCleared) //check if quest XX has been cleared (should not happen)
						{
							return GAME_TS_WARNING_WRONG_CONDITION;
						}
					}
				}
			}
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_STOCEVT: //Check for example kill mob count, have item count (check server to client evt)
		{
			CDboTSCheckSToCEvt* pCont = (CDboTSCheckSToCEvt*)pEntity;

			if (qinfo == NULL)
			{
				ERR_LOG(LOG_QUEST, "QUEST: Quest-ID %u. qinfo is NULL");
				return RESULT_SUCCESS;
			}

			CNtlTSGroup* pTmpGroup = (CNtlTSGroup*)pEntity->GetParent()->GetParent();
			if (pTmpGroup == NULL)
			{
				ERR_LOG(LOG_QUEST, "QUEST: Quest-ID %u Could not find group parent. TA %u, TC %u", tId, qinfo->uData.sQInfoV0.sSToCEvtData.taId, qinfo->uData.sQInfoV0.sSToCEvtData.tcId);
				return GAME_TS_WARNING_WRONG_CONDITION;
			}

			CDboTSContGAct* pTmpAct = (CDboTSContGAct*)pTmpGroup->GetChildCont(qinfo->uData.sQInfoV0.sSToCEvtData.tcId);
			if (pTmpAct == NULL)
			{
				ERR_LOG(LOG_QUEST, "QUEST: Quest-ID %u Could not find CDboTSContGAct. TA %u, TC %u", tId, qinfo->uData.sQInfoV0.sSToCEvtData.taId, qinfo->uData.sQInfoV0.sSToCEvtData.tcId);
				return GAME_TS_WARNING_WRONG_CONDITION;
			}

			CDboTSActSToCEvt* pActSToCEvt = (CDboTSActSToCEvt*)pTmpAct->FindChildEntity(qinfo->uData.sQInfoV0.sSToCEvtData.taId);
			if (pActSToCEvt == NULL)
			{
				ERR_LOG(LOG_QUEST, "QUEST: Quest-ID %u Could not find CDboTSActSToCEvt. TA %u, TC %u", tId, qinfo->uData.sQInfoV0.sSToCEvtData.taId, qinfo->uData.sQInfoV0.sSToCEvtData.tcId);
				return GAME_TS_WARNING_WRONG_CONDITION;
			}

			switch (pCont->GetSToCEvtDataType() )
			{
				case eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT:
				{
					bool bRet = true;
					for ( int i = 0; i < uSTOC_EVT_DATA::MAX_MOB_KILL; i++ )
					{
						if ( 0xffffffff == pActSToCEvt->GetEvtData().sMobKillCnt[i].uiMobIdx ) continue;

						bRet = (pActSToCEvt->GetEvtData().sMobKillCnt[i].nMobCnt <= (int)qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] ) ? true : false;

						if (bRet == false)
							return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
				break;

				case eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT:
				{
					bool bRet = true;
					for (int i = 0; i < uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM; i++)
					{
						if (0xffffffff == pActSToCEvt->GetEvtData().sMobKillItemCnt[i].uiMobLIIdx) continue;

						bRet = (pActSToCEvt->GetEvtData().sMobKillItemCnt[i].nMobLICnt <= (int)qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i]) ? true : false;

						if (bRet == false)
							return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
				break;

				case eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM:
				{
					bool bRet = true;
					for (int i = 0; i < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM; ++i)
					{
						if (0xffffffff == pActSToCEvt->GetEvtData().sDeliveryItemCnt[i].uiItemIdx) continue;

						bRet = (pActSToCEvt->GetEvtData().sDeliveryItemCnt[i].nItemCnt <= qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i]) ? true : false;

						if (bRet == false)
							return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
				break;

				case eSTOC_EVT_DATA_TYPE_OBJECT_ITEM:
				{
					bool bRet = true;
					for (int i = 0; i < uSTOC_EVT_DATA::MAX_OBJECT_ITEM; i++)
					{
						if (0xffffffff == pActSToCEvt->GetEvtData().sObjectItemCnt[i].uiItemIdx) continue;

						bRet = (pActSToCEvt->GetEvtData().sObjectItemCnt[i].nItemCnt <= qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i]) ? true : false;

						if(bRet == false)
							return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
				break;

				case eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT:
				{
					bool bRet = true;
					for (int i = 0; i < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM; i++)
					{
						if (0xffffffff == pActSToCEvt->GetEvtData().sPublicMobItemCnt[i].uiItemIdx) continue;

						bRet = (pActSToCEvt->GetEvtData().sPublicMobItemCnt[i].nItemCnt <= (int)qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i]) ? true : false;

						if (bRet == false)
							return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
				break;

				case eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT:
				{
					bool bRet = true;
					for (int i = 0; i < uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT; i++)
					{
						if (0xffffffff == pActSToCEvt->GetEvtData().sCustomEvtCnt[i].uiCustomEvtID) continue;

						bRet = (pActSToCEvt->GetEvtData().sCustomEvtCnt[i].nMaxCnt <= (int)qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i]) ? true : false;

						if (bRet == false)
							return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
				break;

				case eSTOC_EVT_DATA_TYPE_VISIT:
				{
					bool bRet = true;
					for (int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; i++)
					{
						if (0xffffffff == pActSToCEvt->GetEvtData().sVisitEvt[i].uiObjTblIdx) continue;

						bRet = (pActSToCEvt->GetEvtData().sVisitEvt[i].uiObjTblIdx == qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i]) ? true : false;

						if (bRet == false)
							return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
				break;

				default:
				break;
					
			}

		}
		break;
		case DBO_COND_TYPE_ID_CHECK_ITEM:
		{
			CDboTSCheckItem* pItem = (CDboTSCheckItem*)pEntity;
			// //NTL_PRINT(PRINT_APP,"QUEST-START: PC CHECK ITEM IDX(%d) AND QTY(%d)", pItem->GetItemIdx(), pItem->GetItemCnt());
			CItem* itemcheck = m_pkOwner->GetPlayerItemContainer()->GetItemByIdx(pItem->GetItemIdx());
			if(itemcheck)
			{
				if(itemcheck->GetCount() < pItem->GetItemCnt() )
					return GAME_TS_WARNING_WRONG_CONDITION;

			} 
			else return GAME_TS_WARNING_CANNOT_FIND_QUEST_ITEM;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_IN_WORLD:
		{
			CDboTSCheckInWorld* pWorld = (CDboTSCheckInWorld*)pEntity;
			if (m_pkOwner->GetWorldTblidx() != pWorld->GetWorldTblIdx())
				return RESULT_FAIL;
		}
		break;

		case DBO_COND_TYPE_ID_CHECK_NPCDEAD:
		{
			CDboTSCheckNPCDead* pCheck = (CDboTSCheckNPCDead*)pEntity;

			if (m_pkOwner->GetCurWorld())
			{
				CNpc* pNpc = m_pkOwner->GetCurWorld()->FindNpc(pCheck->GetNPCTableIndex(0));
				if (pNpc && pNpc->IsFainting() == false)
				{
					return RESULT_FAIL;
				}
			}
		}
		break;

		case DBO_COND_TYPE_ID_CHECK_DIST_WITH_NPC:
		{
			CDboTSCheckDistWithNPC* pCheck = (CDboTSCheckDistWithNPC*)pEntity;

			if (m_pkOwner->GetCurWorld())
			{
				CNpc* pNpc = m_pkOwner->GetCurWorld()->FindNpc(pCheck->GetNPCTableIndex(0));
				if (pNpc)
				{
					if (m_pkOwner->IsInRange(pNpc->GetCurLoc(), pCheck->GetDistance()))
						return RESULT_FAIL;
				}
			}
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_PROG_QUEST:
		{
			CDboTSCheckProgQuest* pQuest = (CDboTSCheckProgQuest*)pEntity;
			if (pQuest->GetNOT() == FALSE)
			{
				if (GetQuestProgressInfo(pQuest->GetQuestID()) == NULL)
					return RESULT_FAIL;
			}
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_REPUTATION:
		{
			DWORD uiRT = m_pkOwner->GetReputation();
			CDboTSCheckReputation* pRT = (CDboTSCheckReputation*)pEntity;

			if ( uiRT < pRT->GetMinReputation() || uiRT > pRT->GetMaxReputation() ) //check if pc lower than min repu or higher than max repu
				return GAME_TS_WARNING_WRONG_CONDITION;

		}break;
		case DBO_COND_TYPE_ID_CHECK_BUDOKAI_STATE:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_COND_TYPE_ID_CHECK_BUDOKAI_STATE - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_COND_TYPE_ID_CHECK_AVATARDEAD:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_COND_TYPE_ID_CHECK_AVATARDEAD - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_COND_TYPE_ID_CHECK_OUTRGN: //check if player is out of the region. Return nothing if not in region
		{
			CDboTSCheckOutRGN* pCond = (CDboTSCheckOutRGN*)pEntity;

			CNtlVector vVec(pCond->GetPosX(), 0.0f, pCond->GetPosZ());

			if (m_pkOwner->GetWorldTblidx() == pCond->GetWorldTblIdx() && m_pkOwner->IsInRange(vVec, pCond->GetRadius())) //first check location.
				return RESULT_FAIL;
		}
		break;
		
		/////////////////////////
		//	CHECK EVENT
		/////////////////////////
		case DBO_EVENT_TYPE_ID_CLICK_NPC:
		{
			CDboTSClickNPC* pNpc = (CDboTSClickNPC*)pEntity;

			if (m_pkOwner->GetCurWorld() == NULL)
			{
				return RESULT_FAIL;
			}

			TBLIDX npcIdx = pNpc->GetNPCIdx();

			for (int l = 0; l < pNpc->GetNumOfNPCIdx(); l++)
			{
				CNpc* pObj = m_pkOwner->GetCurWorld()->FindNpc(npcIdx);
				if (pObj)
				{
					if (m_pkOwner->IsInRange(pObj->GetCurLoc(), DBO_DISTANCE_CHECK_TOLERANCE * 2.f) == true || m_pkOwner->GetTargetHandle() == pObj->GetID()) //dont do IsInRange3 otherwise it will can bug on npcs spawned by command
					{
						return RESULT_SUCCESS;
					}
				}

				npcIdx = pNpc->NextNPCIdx();
			}

			return RESULT_FAIL;

		}
		break;
		case DBO_EVENT_TYPE_ID_CLICK_OBJECT:
		{
			CDboTSClickObject* pClick = (CDboTSClickObject*)pEntity;

			if (pClick->GetWorldIdx() != m_pkOwner->GetWorldTblidx())
			{
				return RESULT_FAIL;
			}

			if(m_pkOwner->GetCurWorld() == NULL)
				return RESULT_FAIL;

			TBLIDX objIdx = pClick->BeginObjectIdx();

			for (int l = 0; l < pClick->GetNumOfObjectIdx(); l++)
			{
				CTriggerObject* pObj = m_pkOwner->GetCurWorld()->FindStaticObjectByIdx(objIdx);
				if (pObj)
				{
					if (m_pkOwner->IsInRange(pObj->GetCurLoc(), DBO_DISTANCE_CHECK_TOLERANCE * 2.f) == true || m_pkOwner->GetTargetHandle() == pObj->GetID()) //dont do IsInRange3 otherwise it will can bug on npcs spawned by command
					{
						return RESULT_SUCCESS;
					}
				}

				objIdx = pClick->NextObjectIdx();
			}

			return RESULT_FAIL;
		}
		break;
		case DBO_EVENT_TYPE_ID_COL_OBJECT:
		{
			CDboTSColObject* pCol = (CDboTSColObject*)pEntity;
			if (pCol)
			{
				//check same world
				if (m_pkOwner->GetWorldTblidx() != pCol->GetWorldIdx())
					return GAME_TS_ERROR_TRIGGER_OBJECT_NOT_EXIST;

				//dont allow while flying
				if (m_pkOwner->GetAirState() == AIR_STATE_ON)
				{
					m_pkOwner->SendCharStateFalling(NTL_MOVE_NONE);
					return GAME_COMMON_CANT_DO_THAT_IN_AIR_MODE_STATE;
				}
			}
			else return RESULT_FAIL;
		}
		break;
		case DBO_EVENT_TYPE_ID_ITEM_USE:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_ITEM_USE - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_ITEM_GET:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_ITEM_GET - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_ITEM_EQUIP:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_ITEM_EQUIP - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_SCOUT_USE:
		{
		}break;
		case DBO_EVENT_TYPE_ID_RCV_SVR_EVT:	//check if received server event | this is very important so users cant cheat and possible crash server
		{
			CDboTSRcvSvrEvt* pRecv = (CDboTSRcvSvrEvt*)pEntity;

			if (m_pkOwner->GetQuests()->GetEventMap()->IsReceived(pRecv->GetEvtID()) == false)
			{
				ERR_LOG(LOG_USER, "Player: %u did not receive eventId:%u. GetWorldTblidx:%u", m_pkOwner->GetCharID(), pRecv->GetEvtID(), m_pkOwner->GetWorldTblidx());
				return RESULT_FAIL;
			}

			if (m_pkOwner->GetCurWorld() && m_pkOwner->GetCurWorld()->GetTbldat()->bDynamic == false) // dont remove event when inside dungeon. Because we will remove events, when leaving dungeon.
			{
				//if found, then we can delete it from event map
				m_pkOwner->GetQuests()->GetEventMap()->ClearScriptEvent(pRecv->GetEvtID());
			}
		}
		break;
		case DBO_EVENT_TYPE_ID_SKILL_USE:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_SKILL_USE - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_COL_RGN: //check if player is at the region
		{
			CDboTSColRgn* pColRgn = (CDboTSColRgn*)pEntity;
		//	NTL_PRINT(PRINT_APP, "QUEST-PROCESS: DBO_EVENT_TYPE_ID_COL_RGN pColRgn->GetWorldIdx() %u, pColRgn->GetColCheckType() %u", pColRgn->GetWorldIdx(), pColRgn->GetColCheckType());

			if (m_pkOwner->GetWorldTblidx() == pColRgn->GetWorldIdx()) //some time it can happen that this function is called while player is teleporting(dont have world)
			{
				switch (pColRgn->GetColCheckType())
				{
					case eCOL_RESULT_TYPE_ENTER:
					{
						if (!pColRgn->IsInAABB(m_pkOwner->GetCurLoc().x, m_pkOwner->GetCurLoc().z))
							return RESULT_FAIL;
					}
					break;

					case eCOL_RESULT_TYPE_LEAVE:
					{
						if (pColRgn->InCheck(m_pkOwner->GetCurLoc().x, m_pkOwner->GetCurLoc().z))
							return RESULT_FAIL;
					}
					break;

					case eCOL_RESULT_TYPE_THROUGH:
					{
						if (!pColRgn->InCheck(m_pkOwner->GetCurLoc().x, m_pkOwner->GetCurLoc().z))
							return RESULT_FAIL;
					}
					break;

					default: printf("CDboTSColRgn: GetColCheckType %u unknown \n", pColRgn->GetColCheckType()); return RESULT_FAIL; break;
				}
			}
			else return RESULT_FAIL;
		}
		break;
		case DBO_EVENT_TYPE_ID_RB:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_RB - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_CLICK_MOB:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_CLICK_MOB - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_BIND_STONE:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_BIND_STONE - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_SEARCH_QUEST:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_SEARCH_QUEST - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_ITEM_UPGRADE:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_ITEM_UPGRADE - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_TELEPORT:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_TELEPORT - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_BUDOKAI:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_BUDOKAI - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_SLOTMACHINE:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_SLOTMACHINE - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_HOIPOIMIX:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_HOIPOIMIX - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_PRIVATESHOP:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_PRIVATESHOP - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_FREEBATTLE:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_FREEBATTLE - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_ITEMIDENTITY:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_ITEMIDENTITY - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_USERMAIL:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_USERMAIL - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_PARTY:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_PARTY - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_CLICK_SIDEICON:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_CLICK_SIDEICON - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_DIALOGOPEN:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_DIALOGOPEN - NEED TO DO tId %d  -", tId);
		}break;
		
		case DBO_EVENT_TYPE_ID_QUEST:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_QUEST - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_EVENT_TYPE_ID_LEVELCHECK:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_EVENT_TYPE_ID_LEVELCHECK - NEED TO DO tId %d  -", tId);
		}break;

		/////////////////////////
		//	CHECK ACTION
		/////////////////////////
		case DBO_ACT_TYPE_ID_ACT_ITEM:
		{
			CDboTSActItem* pAct = (CDboTSActItem*)pEntity;
			if (pAct)
			{
				const stTS_ITEM_INFO& itemInfo = pAct->GetItemInfo(0);
				const stTS_ITEM_INFO& itemInfo2 = pAct->GetItemInfo(1);
				const stTS_ITEM_INFO& itemInfo3 = pAct->GetItemInfo(2);

				if (pAct->GetItemType() == eTSITEM_TYPE_CREATE)
				{
					BYTE byItemCnt = 0;

					for (int i = 0; i < CDboTSActItem::eMAX_TS_ITEM_COUNT; i++)
					{
						if (pAct->GetItemInfo(i).uiItemIdx != 0 && pAct->GetItemInfo(i).uiItemIdx != INVALID_TBLIDX)
							++byItemCnt;
					}

					if (m_pkOwner->GetPlayerItemContainer()->CountEmptyInventory() < byItemCnt)
						return GAME_TS_WARNING_INVENTORY_IS_FULL;

					if (!g_pItemManager->CreateItem(m_pkOwner, itemInfo.uiItemIdx, 1))
					{
						return GAME_TS_WARNING_INVENTORY_IS_FULL;
					}

					if (itemInfo2.uiItemIdx != 0 && itemInfo2.uiItemIdx != INVALID_TBLIDX)
					{
						if (!g_pItemManager->CreateItem(m_pkOwner, itemInfo2.uiItemIdx, 1))
						{
							return GAME_TS_WARNING_INVENTORY_IS_FULL;
						}
					}

					if (itemInfo3.uiItemIdx != 0 && itemInfo3.uiItemIdx != INVALID_TBLIDX)
					{
						if (!g_pItemManager->CreateItem(m_pkOwner, itemInfo3.uiItemIdx, 1))
						{
							return GAME_TS_WARNING_INVENTORY_IS_FULL;
						}
					}
				}
				else
				{
					if (itemInfo.uiItemIdx != 0 && itemInfo.uiItemIdx != INVALID_TBLIDX)
					{
						CItem* pItem = m_pkOwner->GetPlayerItemContainer()->GetItemByIdx(itemInfo.uiItemIdx);
						if (pItem)
						{
							if (pItem->GetPlace() == CONTAINER_TYPE_EQUIP)
							{
								if (!m_pkOwner->UnequipItem(pItem))
								{
									return GAME_TS_WARNING_INVENTORY_IS_LOCK;
								}
							}

							pItem->SetCount(0, false, true);
						}
					}

					if (itemInfo2.uiItemIdx != 0 && itemInfo2.uiItemIdx != INVALID_TBLIDX)
					{
						CItem* pItem = m_pkOwner->GetPlayerItemContainer()->GetItemByIdx(itemInfo2.uiItemIdx);
						if (pItem)
						{
							if (pItem->GetPlace() == CONTAINER_TYPE_EQUIP)
							{
								if (!m_pkOwner->UnequipItem(pItem))
									return GAME_TS_WARNING_INVENTORY_IS_LOCK;
							}

							pItem->SetCount(0, false, true);
						}
					}

					if (itemInfo3.uiItemIdx != 0 && itemInfo3.uiItemIdx != INVALID_TBLIDX)
					{
						CItem* pItem = m_pkOwner->GetPlayerItemContainer()->GetItemByIdx(itemInfo3.uiItemIdx);
						if (pItem)
						{
							if (pItem->GetPlace() == CONTAINER_TYPE_EQUIP)
							{
								if (!m_pkOwner->UnequipItem(pItem))
									return GAME_TS_WARNING_INVENTORY_IS_LOCK;
							}

							pItem->SetCount(0, false, true);
						}
					}
				}
			}
			else 
				return RESULT_FAIL;
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_STOCEVT:
		{
			CGameServer* app = (CGameServer*)g_pApp;
			CDboTSActSToCEvt* pEvt = (CDboTSActSToCEvt*)pEntity;
			//NTL_PRINT(PRINT_APP, "DBO_ACT_TYPE_ID_ACT_STOCEVT GetEvtDataType %u GetEvtType %u GetEvtCondDataType %u, GetEvtSubCondDataType %u", pEvt->GetEvtDataType(), pEvt->GetEvtType(), pEvt->GetEvtCondDataType(), pEvt->GetEvtSubCondDataType());

			if (qinfo == NULL)
			{
				ERR_LOG(LOG_QUEST, "ERROR: !! QINFO IS NULL \n");
				return RESULT_FAIL;
			}

			//start quest
			if(pEvt->GetEvtType() == eSTOC_EVT_TYPE_START)
			{
				qinfo->uData.sQInfoV0.sSToCEvtData.Init();

				switch(pEvt->GetEvtDataType())
				{
					case eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT:
					break;
					case eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT:
					break;
					case eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM:
					{
						for(int i = 0; i < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM; i++)
						{
							if(pEvt->GetEvtData().sDeliveryItemCnt[i].uiItemIdx != INVALID_TBLIDX)
							{
								if (GetQuestItemData(pEvt->GetEvtData().sDeliveryItemCnt[i].uiItemIdx) == NULL)
								{
									if (CreateQuestItem(pEvt->GetEvtData().sDeliveryItemCnt[i].uiItemIdx, pEvt->GetEvtData().sDeliveryItemCnt[i].nItemCnt) == false)
										return GAME_TS_WARNING_QUEST_INVENTORY_IS_FULL;
								}
								qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] += pEvt->GetEvtData().sDeliveryItemCnt[i].nItemCnt;
							}
						}
					}
					break;
					case eSTOC_EVT_DATA_TYPE_OBJECT_ITEM:
					break;
					case eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT: //kill any mob which has a specific level
					{
					}
					break;
					case eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT:
					{
						qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[0] = 0;
					}
					break;
					case eSTOC_EVT_DATA_TYPE_VISIT:
					{
						for(int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; i++)
						{
							qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] = 0;

							if (pEvt->GetEvtData().sVisitEvt[i].uiItemTblIdx != INVALID_TBLIDX)
							{
								if (GetQuestItemData(pEvt->GetEvtData().sVisitEvt[i].uiItemTblIdx) == NULL)
								{
									if (CreateQuestItem(pEvt->GetEvtData().sVisitEvt[i].uiItemTblIdx, 1) == false)
										return GAME_TS_WARNING_QUEST_INVENTORY_IS_FULL;
								}
							}
						}
					}
					break;
					default: ERR_LOG(LOG_QUEST,"DBO_ACT_TYPE_ID_ACT_STOCEVT %d not found", pEvt->GetEvtDataType()); break;
				}

				switch(pEvt->GetEvtCondDataType())
				{
					case eSTOC_EVT_COND_DATA_TYPE_AUTO_EQUIP_ITEM:
					{
						const uSTOC_EVT_COND_DATA& uEvtCondData = pEvt->GetEvtCondData();
						if(m_pkOwner->GetPlayerItemContainer()->GetItem(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_QUEST) == NULL && m_pkOwner->GetPlayerItemContainer()->IsInventoryReserved(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_QUEST) == false)
						{
							sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(uEvtCondData.sAutoEquipItem->uiItemTblIdx);
							if (pItemTbldat)
							{
								m_pkOwner->GetPlayerItemContainer()->AddReservedInventory(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_QUEST); //reserve quest equip slot so none can equip item

								CNtlPacket pQry(sizeof(sGQ_ITEM_AUTO_EQUIP_REQ));
								sGQ_ITEM_AUTO_EQUIP_REQ * resQry = (sGQ_ITEM_AUTO_EQUIP_REQ *)pQry.GetPacketData();
								resQry->wOpCode = GQ_ITEM_AUTO_EQUIP_REQ;
								resQry->charId = m_pkOwner->GetCharID();
								resQry->handle = m_pkOwner->GetID();

								resQry->sItem.byDestPlace = CONTAINER_TYPE_EQUIP;
								resQry->sItem.byDestPos = EQUIP_SLOT_TYPE_QUEST;

								resQry->sItem.sEquipItem.charId = m_pkOwner->GetCharID();
								resQry->sItem.sEquipItem.itemNo = pItemTbldat->tblidx;
								resQry->sItem.sEquipItem.byStackcount = 1;
								resQry->sItem.sEquipItem.byPlace = CONTAINER_TYPE_EQUIP;
								resQry->sItem.sEquipItem.byPosition = EQUIP_SLOT_TYPE_QUEST;
								resQry->sItem.sEquipItem.byCurrentDurability = pItemTbldat->byDurability;
								resQry->sItem.sEquipItem.byRank = pItemTbldat->byRank;
								resQry->sItem.sEquipItem.bNeedToIdentify = false;
								resQry->sItem.sEquipItem.byDurationType = pItemTbldat->byDurationType;
								resQry->sItem.sEquipItem.byRestrictState = GetDefaultRestrictState(pItemTbldat->byRestrictType, pItemTbldat->byItem_Type, true);
								CItem::GenerateOptionSet(false, pItemTbldat, &resQry->sItem.sEquipItem);

								if (pItemTbldat->byDurationType == eDURATIONTYPE_FLATSUM)
								{
									resQry->sItem.sEquipItem.nUseStartTime = time(0);
									resQry->sItem.sEquipItem.nUseEndTime = resQry->sItem.sEquipItem.nUseStartTime + pItemTbldat->dwUseDurationMax;
								}

								pQry.SetPacketLen(sizeof(sGQ_ITEM_AUTO_EQUIP_REQ));
								app->SendTo(app->GetQueryServerSession(), &pQry);
							}
							else return GAME_FAIL;
						}
						else return GAME_TS_WARNING_EQUIP_SLOT_NOT_EMPTY;
					}
					break;
					case eSTOC_EVT_COND_DATA_TYPE_EQUIP_ITEM:
					break;
					case eSTOC_EVT_COND_DATA_TYPE_HAVE_ITEM:
					break;
					case eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM:
					break;
					default: break;
				}

				qinfo->uData.sQInfoV0.sSToCEvtData.taId = pEvt->GetActionId();
				qinfo->uData.sQInfoV0.sSToCEvtData.tcId = ((CNtlTSCont*)pEvt->GetParent())->GetID();

				CNtlPacket startquest(sizeof(sGU_QUEST_SVREVT_START_NFY));
				sGU_QUEST_SVREVT_START_NFY * startquestres = (sGU_QUEST_SVREVT_START_NFY *)startquest.GetPacketData();
				startquestres->wOpCode = GU_QUEST_SVREVT_START_NFY;
				startquestres->taId = qinfo->uData.sQInfoV0.sSToCEvtData.taId;
				startquestres->tcId = qinfo->uData.sQInfoV0.sSToCEvtData.tcId;
				startquestres->tId = tId;
				startquest.SetPacketLen(sizeof(sGU_QUEST_SVREVT_START_NFY));
				g_pApp->Send( m_pkOwner->GetClientSessionID(), &startquest );
			}
			else
			{
				if (pEvt->GetEvtDataType() != eSTOC_EVT_DATA_TYPE_INVALID)
				{
					CNtlTSGroup* pTmpGroup = (CNtlTSGroup*)pEntity->GetParent()->GetParent();
					if (pTmpGroup)
					{
						CDboTSContGAct* pTmpAct = (CDboTSContGAct*)pTmpGroup->GetChildCont(qinfo->uData.sQInfoV0.sSToCEvtData.tcId);
						if (pTmpAct)
						{
							CDboTSActSToCEvt* pActSToCEvt = (CDboTSActSToCEvt*)pTmpAct->FindChildEntity(qinfo->uData.sQInfoV0.sSToCEvtData.taId);
							if (pActSToCEvt)
							{
								const uSTOC_EVT_DATA& evtdata = pActSToCEvt->GetEvtData();

								switch (pEvt->GetEvtDataType())
								{
									case eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT:
									{
										for (int i = 0; i < evtdata.MAX_MOB_KILL; i++)
										{
											if (qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] > 0)
											{
												DeleteQuestItem(evtdata.sMobKillItemCnt[i].uiMobLIIdx, qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i]);
											}
										}
									}
									break;
									case eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM:
									{
										UnequipAndDeleteQuestItem();
									}
									break;
									case eSTOC_EVT_DATA_TYPE_OBJECT_ITEM:
									{
										for (int i = 0; i < uSTOC_EVT_DATA::MAX_OBJECT_ITEM; i++)
										{
											if (evtdata.sObjectItemCnt[i].uiItemIdx != INVALID_TBLIDX && qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] != 0)
											{
												DeleteQuestItem(evtdata.sObjectItemCnt[i].uiItemIdx, evtdata.sObjectItemCnt[i].nItemCnt);
											}
										}
									}
									break;
									case eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT:
									{
										for (int i = 0; i < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM; i++)
										{
											if (evtdata.sPublicMobItemCnt[i].uiItemIdx != INVALID_TBLIDX && qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] != 0)
											{
												DeleteQuestItem(evtdata.sPublicMobItemCnt[i].uiItemIdx, evtdata.sPublicMobItemCnt[i].nItemCnt);
											}
										}
									}
									break;
									case eSTOC_EVT_DATA_TYPE_VISIT:
									{
										for (int i = 0; i < uSTOC_EVT_DATA::MAX_VISIT_EVT; i++)
										{
											if (evtdata.sVisitEvt[i].uiItemTblIdx != INVALID_TBLIDX && qinfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[i] != 0)
											{
												DeleteQuestItem(evtdata.sVisitEvt[i].uiItemTblIdx, 1);
											}
										}
									}
									break;

									default: break;
								}
							}
						}
					}
				}

				switch (pEvt->GetEvtCondDataType())
				{
					case eSTOC_EVT_COND_DATA_TYPE_AUTO_EQUIP_ITEM:
					{
						UnequipAndDeleteQuestItem();
					}
					break;
					case eSTOC_EVT_COND_DATA_TYPE_EQUIP_ITEM:
					break;
					case eSTOC_EVT_COND_DATA_TYPE_HAVE_ITEM:
					break;
					case eSTOC_EVT_COND_DATA_TYPE_HAVE_QITEM:
					break;
					default: break;
				}

				CNtlPacket endEvt(sizeof(sGU_QUEST_SVREVT_END_NFY));
				sGU_QUEST_SVREVT_END_NFY * rEndEvt = (sGU_QUEST_SVREVT_END_NFY *)endEvt.GetPacketData();
				rEndEvt->wOpCode = GU_QUEST_SVREVT_END_NFY;
				rEndEvt->taId = qinfo->uData.sQInfoV0.sSToCEvtData.taId;
				rEndEvt->tcId = qinfo->uData.sQInfoV0.sSToCEvtData.tcId;
				rEndEvt->tId = qinfo->tId;
				endEvt.SetPacketLen(sizeof(sGU_QUEST_SVREVT_END_NFY));
				g_pApp->Send( m_pkOwner->GetClientSessionID(), &endEvt );

				qinfo->uData.sQInfoV0.sSToCEvtData.taId = NTL_TS_TA_ID_INVALID;
				qinfo->uData.sQInfoV0.sSToCEvtData.tcId = NTL_TS_TC_ID_INVALID;
			}

		}
		break;
		case DBO_ACT_TYPE_ID_ACT_QITEM:
		{
			CDboTSActQItem* pQItem = (CDboTSActQItem*)pEntity;

			if (qinfo)
				qinfo->uData.sQInfoV0.taQuestInfo = pQItem->GetActionId();

			switch(pQItem->GetQItemType())
			{
				case eQITEM_TYPE_CREATE:
				{
					for(int i = 0; i < pQItem->eMAX_TS_QITEM_COUNT; i++)
					{
						const stTS_QITEM_INFO& tsqitem = pQItem->GetQItemInfo(i);
						if(tsqitem.uiQItemIdx != INVALID_TBLIDX && tsqitem.uiQItemIdx != 0)
						{
							if(pTrigger)
								return UpdateQuestObjectItemEvt(tId, tsqitem.uiQItemIdx, tsqitem.nQItemCnt); //if act_qitem called when executed object then call this. otherwise call the other function
							else if (!CreateQuestItem(tsqitem.uiQItemIdx, (BYTE)tsqitem.nQItemCnt))
								return GAME_TS_WARNING_QUEST_INVENTORY_IS_FULL;
						}
					}
				}
				break;
				case eQITEM_TYPE_DELETE:
				{
					for(int i = 0; i < pQItem->eMAX_TS_QITEM_COUNT; i++)
					{
						const stTS_QITEM_INFO& tsqitem = pQItem->GetQItemInfo(i);
						if (tsqitem.uiQItemIdx != INVALID_TBLIDX && tsqitem.uiQItemIdx > 0 && tsqitem.nQItemCnt > 0)
						{
							DeleteQuestItem(tsqitem.uiQItemIdx, tsqitem.nQItemCnt);
						}
					}
				}
				break;
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_NPCCONV:
		{
			CDboTSActNPCConv* pAct = (CDboTSActNPCConv*)pEntity;

			if (m_pkOwner->GetCurWorld())
			{
				CNpc* pNpc = m_pkOwner->GetCurWorld()->FindNpc(pAct->GetNPCIdx());
				if (pNpc)
				{
					//crashes the client
				/*	CNtlPacket packet(sizeof(sGU_TS_TRIGGER_OBJECT_DIALOG_NFY));
					sGU_TS_TRIGGER_OBJECT_DIALOG_NFY * res = (sGU_TS_TRIGGER_OBJECT_DIALOG_NFY *)packet.GetPacketData();
					res->wOpCode = GU_TS_TRIGGER_OBJECT_DIALOG_NFY;
					res->byDialogType = pAct->GetNPCConvType();
					res->textTblidx = pAct->GetNPCConv();
					res->hTriggerObject = pNpc->GetID();
					packet.SetPacketLen(sizeof(sGU_TS_TRIGGER_OBJECT_DIALOG_NFY));
					m_pkOwner->SendPacket(&packet); */
				}
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_REGQINFO:
		{
		//	//NTL_PRINT(PRINT_APP,"QUEST-START: DBO_ACT_TYPE_ID_ACT_REGQINFO");
			CDboTSActRegQInfo* pRegQInfo = (CDboTSActRegQInfo*)pEntity;

			qinfo->uData.sQInfoV0.taQuestInfo = pRegQInfo->GetActionId();
			qinfo->uData.sQInfoV0.tgExcCGroup = ((CNtlTSGroup*)pRegQInfo->GetParent()->GetParent())->GetID();
			qinfo->uData.sQInfoV0.tcQuestInfo = ((CNtlTSCont*)pRegQInfo->GetParent())->GetID();

		}
		break;
		case DBO_ACT_TYPE_ID_ACT_DIR:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_DIR - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_OPCAM:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_OPCAM - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_TWAITTS:
		{
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_INSSM:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_INSSM - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_TSSTATE: //mostly/only called when executing quest group from timer
		{
			CDboTSActTSState* pAct = (CDboTSActTSState*)pEntity;

			qinfo->uData.sQInfoV0.wQState = pAct->GetTSState();

			if (!pQuestProgress->IsInProgressState()) //if ts state updated to fail
			{
				if (pQuestProgress->GetWpsScriptID() != INVALID_DWORD)	//if a script is connected to quest then end script
				{
					CScriptAlgoObject* pScript = m_pkOwner->GetScript(pQuestProgress->GetWpsScriptID());

					if (pScript)
						pScript->NotifyPlayerLeave(m_pkOwner->GetID());

					if(pQuestProgress)
						pQuestProgress->SetWpsScriptID(INVALID_DWORD);
				}
			}

			CNtlPacket packet(sizeof(sGU_TS_UPDATE_STATE));
			sGU_TS_UPDATE_STATE * res = (sGU_TS_UPDATE_STATE *)packet.GetPacketData();
			res->wOpCode = GU_TS_UPDATE_STATE;
			res->byTsType = TS_TYPE_QUEST_CS;
			res->byType = pAct->GetType();
			res->tId = tId;
			res->wTSState = pAct->GetTSState();
			packet.SetPacketLen(sizeof(sGU_TS_UPDATE_STATE));
			m_pkOwner->SendPacket(&packet);
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_EXCEPT_TIMER_S: //timer start
		{
			CDboTSActETimerS* pTime = (CDboTSActETimerS*)pEntity;

			if (pTime->GetTimerSort() != eEXCEPT_TIMER_SORT_CLIENT_TIMER)
			{
				for (int i = 0; i < sEXCEPT_TIMER_SLOT::eTIMER_SLOT_MAX; i++)
				{
					if (qinfo->uData.sQInfoV0.sETSlot.asExceptTimer[i].tcId == NTL_TS_TC_ID_INVALID)
					{
						qinfo->uData.sQInfoV0.sETSlot.asExceptTimer[i].taId = pTime->GetActionId();
						qinfo->uData.sQInfoV0.sETSlot.asExceptTimer[i].tcId = ((CNtlTSCont*)pTime->GetParent())->GetID();
						qinfo->uData.sQInfoV0.sETSlot.asExceptTimer[i].uiRemainTime = pTime->GetTime();

						pQuestProgress->SetFailOnLogout(true);
						pQuestProgress->SetTimer(pTime, i);
						break;
					}
				}
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_EXCEPT_TIMER_E: //timer end
		{
			CDboTSActETimerE* pTimerEnd = (CDboTSActETimerE*)pEntity;

			for (int i = 0; i < sEXCEPT_TIMER_SLOT::eTIMER_SLOT_MAX; i++)
			{
				if (qinfo->uData.sQInfoV0.sETSlot.asExceptTimer[i].tcId == pTimerEnd->GetTCId())
				{
					pQuestProgress->SetFailOnLogout(false);
					qinfo->uData.sQInfoV0.sETSlot.asExceptTimer[i].tcId = NTL_TS_TC_ID_INVALID;
					qinfo->uData.sQInfoV0.sETSlot.asExceptTimer[i].taId = NTL_TS_TA_ID_INVALID;
					break;
				}
			}
		
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_PORTAL:
		{
			CDboTSActPortal* pPortal = (CDboTSActPortal*)pEntity;
			if (pPortal)
			{
				if (m_pkOwner->GetAspectStateId() == ASPECTSTATE_SPINNING_ATTACK || m_pkOwner->GetAspectStateId() == ASPECTSTATE_ROLLING_ATTACK)
					return RESULT_FAIL;

				if (m_pkOwner->GetDragonballScrambleBallFlag() == 0)
				{
					CNtlVector destPos, destDir;
					pPortal->GetPosition(destPos.x, destPos.y, destPos.z);
					pPortal->GetDirection(destDir.x, destDir.y, destDir.z);

					WORLDID destWorldID = (pPortal->GetWorldIdx() == INVALID_TBLIDX || m_pkOwner->GetUD() && pPortal->GetPotalType() == ePORTAL_TYPE_TELEPORT) ? m_pkOwner->GetWorldID() : pPortal->GetWorldIdx();

					m_pkOwner->StartTeleport(destPos, destDir, destWorldID, TELEPORT_TYPE_DEFAULT);
				}
				else return RESULT_FAIL;
			}
			else return RESULT_FAIL;
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_CONC_CHECK:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_CONC_CHECK - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_OBJCONV:
		{
			CDboTSActObjConv* pConv = (CDboTSActObjConv*)pEntity;

			CTriggerObject* pObj = m_pkOwner->GetCurWorld()->FindStaticObjectByIdx(pConv->GetObjIdx());
			if (pObj)
			{
				CNtlPacket packet(sizeof(sGU_TS_TRIGGER_OBJECT_DIALOG_NFY));
				sGU_TS_TRIGGER_OBJECT_DIALOG_NFY * res = (sGU_TS_TRIGGER_OBJECT_DIALOG_NFY *)packet.GetPacketData();
				res->wOpCode = GU_TS_TRIGGER_OBJECT_DIALOG_NFY;
				res->hTriggerObject = pObj->GetID();
				res->byDialogType = pConv->GetObjConvType();
				res->textTblidx = pConv->GetObjConv();
				pObj->Broadcast(&packet);
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_HINT:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_HINT - NEED TO DO tId %d  -", tId);
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_SEND_SVR_EVT: //SEND SERVER EVENT example to WPS (recv event from ts)
		{
			CDboTSActSendSvrEvt* pEvt = (CDboTSActSendSvrEvt*)pEntity;
		//	printf("CDboTSActSendSvrEvt: GetActionId %u GetEvtSendType %u GetEvtSendType_Radius %f GetSvrEvtID %u GetSvrEvtTriggerType %u GetSvrEvtType %u GetTblIdx %u \n",
		//		pEvt->GetActionId(), pEvt->GetEvtSendType(), pEvt->GetEvtSendType_Radius(), pEvt->GetSvrEvtID(), pEvt->GetSvrEvtTriggerType(), pEvt->GetSvrEvtType(), pEvt->GetTblIdx());

			if (pEvt->GetSvrEvtTriggerType() == TS_TYPE_QUEST_CS) //most likely send to players
			{
				switch (pEvt->GetSvrEvtType())
				{
					case TRIGGER_SERVER_EVENT_TYPE_SPS:
					{
						CNpc* pNpc = m_pkOwner->GetCurWorld()->FindNpc(pEvt->GetTblIdx());
						if (pNpc && pNpc->IsInitialized())
						{
							CObjMsg_TriggerEvent pMsg;
							pMsg.teid = pEvt->GetSvrEvtID();
							pMsg.hSource = m_pkOwner->GetID();
							pMsg.byTriggerType = TS_TYPE_QUEST_CS;

							pNpc->OnObjectMsg(&pMsg);
						}
					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_TS:
					{
						CNtlPacket packet(sizeof(sGU_TS_UPDATE_EVENT_NFY));
						sGU_TS_UPDATE_EVENT_NFY * res = (sGU_TS_UPDATE_EVENT_NFY *)packet.GetPacketData();
						res->wOpCode = GU_TS_UPDATE_EVENT_NFY;
						res->byTsType = pEvt->GetSvrEvtTriggerType();
						res->teid = pEvt->GetSvrEvtID();
						packet.SetPacketLen(sizeof(sGU_TS_UPDATE_EVENT_NFY));

						switch (pEvt->GetEvtSendType())
						{
							case TS_EVENT_SEND_TYPE_WORLD:
							{
								m_pkOwner->GetCurWorld()->AddEventIdToAllPlayers(pEvt->GetSvrEvtID());
								m_pkOwner->GetCurWorld()->Broadcast(&packet);
							}
							break;
							case TS_EVENT_SEND_TYPE_WORLD_AREA:
							{
								m_pkOwner->GetCurWorld()->AddEventIdToAllPlayers(pEvt->GetSvrEvtID(), m_pkOwner, pEvt->GetEvtSendType_Radius());
								m_pkOwner->GetCurWorld()->Broadcast(&packet, m_pkOwner, pEvt->GetEvtSendType_Radius());
							}
							break;
							case TS_EVENT_SEND_TYPE_PARTY:
							{
								if (m_pkOwner->GetParty())
								{
									m_pkOwner->GetParty()->AddEventIdToAll(pEvt->GetSvrEvtID());
									m_pkOwner->GetParty()->SendMsgToParty(&packet);
								}
								else
								{
									m_pkOwner->GetQuests()->GetEventMap()->OnEventReceived(pEvt->GetSvrEvtID());
									m_pkOwner->SendPacket(&packet);
								}
							}
							break;
							case TS_EVENT_SEND_TYPE_PARTY_AREA:
							{
								if (m_pkOwner->GetParty())
								{
									m_pkOwner->GetParty()->AddEventIdToWithinRange(pEvt->GetSvrEvtID(), m_pkOwner, pEvt->GetEvtSendType_Radius());

									m_pkOwner->GetParty()->BroadcastWithinRange(&packet, m_pkOwner, pEvt->GetEvtSendType_Radius());
								}
								else
								{
									m_pkOwner->GetQuests()->GetEventMap()->OnEventReceived(pEvt->GetSvrEvtID());
									m_pkOwner->SendPacket(&packet);
								}
							}
							break;
							case TS_EVENT_SEND_TYPE_PC:
							{
								m_pkOwner->GetQuests()->GetEventMap()->OnEventReceived(pEvt->GetSvrEvtID());
								m_pkOwner->SendPacket(&packet);
							}
							break;
							case TS_EVENT_SEND_TYPE_OBJECT_TRIGGER:
							ERR_LOG(LOG_USER, "case TS_EVENT_SEND_TYPE_OBJECT_TRIGGER not set yet. Quest ID %u", tId);
							break;
						}
					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_WPS:
					{
						CScriptAlgoObject* pScript = m_pkOwner->GetScript(pEvt->GetTblIdx());
						if (pScript)
						{
							CObjMsg_SendTsEventToWps pMsg;
							pMsg.teId = pEvt->GetSvrEvtID();
							pMsg.byTriggerType = TS_TYPE_QUEST_CS;
							pScript->OnObjectMsg(&pMsg);
						}
					}
					break;

					default: ERR_LOG(LOG_USER, "case pEvt->GetSvrEvtType(%u) not set yet. Quest ID %u", pEvt->GetSvrEvtType(), tId); break;
				}
			}
			else if (pEvt->GetSvrEvtTriggerType() == TS_TYPE_OBJECT_TRIGGER_S)
			{
				CScriptAlgoObject* pScript = m_pkOwner->GetCurWorld()->GetScript(pEvt->GetTblIdx());
				if (pScript)
				{
					CObjMsg_SendTsEventToWps pMsg;
					pMsg.teId = pEvt->GetSvrEvtID();
					pMsg.byTriggerType = TS_TYPE_OBJECT_TRIGGER_S;
					pScript->OnObjectMsg(&pMsg);
				}
			}
			else if (pEvt->GetSvrEvtTriggerType() == TS_TYPE_INVALID)
			{
				switch (pEvt->GetSvrEvtType())
				{
					case TRIGGER_SERVER_EVENT_TYPE_SPS:
					{
						CNpc* pNpc = m_pkOwner->GetCurWorld()->FindNpc(pEvt->GetTblIdx());
						if (pNpc && pNpc->IsInitialized())
						{
							CObjMsg_TriggerEvent pMsg;
							pMsg.teid = pEvt->GetSvrEvtID();
							pMsg.hSource = m_pkOwner->GetID();
							pMsg.byTriggerType = TS_TYPE_QUEST_CS;

							pNpc->OnObjectMsg(&pMsg);
						}
					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_TQS: //send to time quest script
					{
						if (m_pkOwner->GetTMQ() && m_pkOwner->GetTMQ()->GetTQS())
						{
							CObjMsg_TqsEvent pMsg;
							pMsg.eventID = pEvt->GetSvrEvtID();
							m_pkOwner->GetTMQ()->GetTQS()->OnObjectMsg(&pMsg);
						}
					}
					break;

					/*case TRIGGER_SERVER_EVENT_TYPE_TMQ_LEAVE:
					{
					}
					break;*/

					case TRIGGER_SERVER_EVENT_TYPE_WPS: //send to wps
					{
						if (pQuestProgress)
						{
							CScriptAlgoObject* pScript = m_pkOwner->GetScript(pEvt->GetTblIdx());
							if (pScript)
							{
								CObjMsg_SendTsEventToWps pMsg;
								pMsg.teId = pEvt->GetSvrEvtID();
								pMsg.byTriggerType = TS_TYPE_QUEST_CS;
								pScript->OnObjectMsg(&pMsg);
							}
						}
						else
						{
							CScriptAlgoObject* pScript = m_pkOwner->GetCurWorld()->GetScript(pEvt->GetTblIdx());
							if (pScript)
							{
								CObjMsg_SendTsEventToWps pMsg;
								pMsg.teId = pEvt->GetSvrEvtID();
								pMsg.byTriggerType = TS_TYPE_OBJECT_TRIGGER_S;
								pScript->OnObjectMsg(&pMsg);
							}
						}
					}
					break;

					default: 
						ERR_LOG(LOG_USER, "case pEvt->GetSvrEvtType(%u) not set yet. tID %u", pEvt->GetSvrEvtType(), tId); 
					break;
				}
			}


		}
		break;
		case DBO_ACT_TYPE_ID_ACT_TMQ_STAGE:
		{
			CDboTSActTMQStage* pTMQStage = (CDboTSActTMQStage*)pEntity;
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_TMQ_STAGE - GetTMQStage %d", pTMQStage->GetTMQStage());


		}
		break;
		case DBO_ACT_TYPE_ID_ACT_OPEN_WINDOW:
		{
			////NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_OPEN_WINDOW - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_TMQ_TIMEBONUS:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_TMQ_TIMEBONUS - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_TELECAST:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_TELECAST - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_DIRINDICATOR:
		{
			CDboTSActDirIndicator* pAct = (CDboTSActDirIndicator*)pEntity;

			CNtlPacket packet(sizeof(sGU_DIRECTION_INDICATE_NFY));
			sGU_DIRECTION_INDICATE_NFY * res = (sGU_DIRECTION_INDICATE_NFY*)packet.GetPacketData();
			res->wOpCode = GU_DIRECTION_INDICATE_NFY;
			res->bIndicate = pAct->IsSelfDirIndicator();
			res->bAutoHide = pAct->IsDirIndicatorStart();
			res->vLoc.x = pAct->GetDirectionIndicatorData().sPos.fX;
			res->vLoc.y = pAct->GetDirectionIndicatorData().sPos.fY;
			res->vLoc.z = pAct->GetDirectionIndicatorData().sPos.fZ;
			packet.SetPacketLen(sizeof(sGU_DIRECTION_INDICATE_NFY));
			m_pkOwner->SendPacket(&packet);
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_ESCORT:
		{
			CDboTSActEscort* pAct = (CDboTSActEscort*)pEntity;

			CNpc* pNpc = m_pkOwner->GetCurWorld()->FindNpc(pAct->GetNPCTableIndex());
			if (pNpc == NULL || !pNpc->IsInitialized())
			{
				return RESULT_FAIL;
			}

			if (pAct->IsEscortStart()) //start escort
			{
				WORD wTemp = RESULT_SUCCESS;
				pNpc->GetEscortManager()->AddTarget(pAct->IsTSShare(), pAct->GetEscortType(), m_pkOwner->GetID(), m_pkOwner->GetCharID(), m_pkOwner->GetPartyID(), TS_TYPE_QUEST_CS, tId, &wTemp);

				if (wTemp != RESULT_SUCCESS)
				{
					ERR_LOG(LOG_QUEST, "ERROR ADDTARGET TO ESCORT FAILED Result %u ! pAct->IsTSShare(%i), pAct->GetEscortType(%u), m_pkOwner->GetID(%u), m_pkOwner->GetCharID(%u), m_pkOwner->GetPartyID(%u)",
						wTemp, pAct->IsTSShare(), pAct->GetEscortType(), m_pkOwner->GetID(), m_pkOwner->GetCharID(), m_pkOwner->GetPartyID());
					return wTemp;
				}

				CObjMsg_StartEscort pMsg;
				pMsg.m_byEscortType = pAct->GetEscortType();
				pNpc->OnObjectMsg(&pMsg);
			}
			else
			{
				pNpc->GetEscortManager()->Clear(); //clear escort
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_WORLDPLAYSCRIPT:
		{
			CDboTSActWorldPlayScript* pAct = (CDboTSActWorldPlayScript*)pEntity;
		//	printf("DBO_ACT_TYPE_ID_ACT_WORLDPLAYSCRIPT: GetScriptID %u IsStart %i IsUserOutFailed %i\n", pAct->GetScriptID(), pAct->IsStart(), pAct->IsUserOutFailed());

			if (pAct->IsStart())
			{
				if (m_pkOwner->GetScript(pAct->GetScriptID()) == NULL)
				{
					if (m_pkOwner->GetCurWorld())
					{
						bool rbStart = false;
						CWpsAlgoObject* pObj = g_pScriptAlgoManager->CreateWpsObject(pAct->GetScriptID(), rbStart, m_pkOwner);
						if (pObj)
						{
							pObj->SetWorld(m_pkOwner->GetCurWorld());
							pObj->AddPlayer(m_pkOwner->GetID());
							pObj->SetStartedByQuest(true);

							if (pQuestProgress)
							{
								pQuestProgress->SetFailOnLogout(pAct->IsUserOutFailed());

								qinfo->uData.sQInfoV0.wQState = eTS_SVR_STATE_WPS_MODE;

								pQuestProgress->SetWpsScriptID(pAct->GetScriptID());
							}

							m_pkOwner->AddScript(pObj);

							if (rbStart)		//if its new created script then start it
							{
								if (m_pkOwner->GetTMQ()) //if inside TMQ then add script to world (This is needed. Because if we start wps, and NPC have to send something to this script)
									m_pkOwner->GetCurWorld()->AddScript(pObj);

								pObj->Start();
							}
						}
						else return RESULT_FAIL;
					}
					else return RESULT_FAIL;
				}
				else return RESULT_FAIL;
			}
			else
			{
				pQuestProgress->SetFailOnLogout(pAct->IsUserOutFailed());

				if (qinfo->uData.sQInfoV0.wQState == eTS_SVR_STATE_WPS_MODE)
					qinfo->uData.sQInfoV0.wQState = 0;

				pQuestProgress->SetWpsScriptID(INVALID_DWORD);
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_PCCONV:
		{
			CDboTSActPCConv* pPcConv = (CDboTSActPCConv*)pEntity;
			m_pkOwner->GetClientSession()->SendPcTsDialogNfy(pPcConv->GetConvTblIdx());
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_TELMUDOSA:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_TELMUDOSA - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_TELMINORMATCH:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_TELMINORMATCH - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_PIDGN: //enter EXISTING ultimate dungeon or leave
		{
			CDboTSActPIDgn* pAct = (CDboTSActPIDgn*)pEntity;

			if (m_pkOwner->GetAspectStateId() == ASPECTSTATE_SPINNING_ATTACK || m_pkOwner->GetAspectStateId() == ASPECTSTATE_ROLLING_ATTACK)
				return RESULT_FAIL;

			if (pAct->GetDungeonType() == ePARTY_DUNGEON_TYPE_ENTER)
			{
				if (m_pkOwner->GetParty() && m_pkOwner->GetPartyID() != INVALID_PARTYID)
				{
					CUltimateDungeon* pDungeon = g_pDungeonManager->FindUltimateDungeon(m_pkOwner->GetPartyID());
					if (pDungeon)
					{
						if (m_pkOwner->GetLevel() < pDungeon->GetTbldat()->byMinLevel)
							return GAME_WORLD_ENTER_NEED_HIGH_LEVEL;
						else if (m_pkOwner->GetLevel() > pDungeon->GetTbldat()->byMaxLevel)
							return GAME_WORLD_ENTER_NEED_LOW_LEVEL;
						else if (m_pkOwner->GetRankBattleRoomTblidx() != INVALID_TBLIDX)
							return GAME_RANKBATTLE_MEMBER_ALREADY_JOINED_RANKBATTLE;
						else if (m_pkOwner->GetDragonballScrambleBallFlag() > 0)
							return GAME_CAN_NOT_TELEPORT;
						else
						{
							CWorld* pWorld = pDungeon->GetWorld();

						/// NOT NEEDED I GUESS
						//	if (m_pkOwner->GetParty()->IsSomeoneInDynamic(pWorld->GetID()) == true)
						//		return GAME_PARTYMATCHING_ANY_MEMBER_IN_DYNAMIC_WORLD;

							m_pkOwner->StartTeleport(pWorld->GetTbldat()->vStart1Loc, pWorld->GetTbldat()->vStart1Dir, pWorld->GetID(), TELEPORT_TYPE_DUNGEON);

							pDungeon->JoinDungeon(m_pkOwner);
							m_pkOwner->SetUD(pDungeon);
						}
					}
					else return GAME_PARTY_DUNGEON_IS_NOT_CREATED;
				}
				else return GAME_COMMON_YOU_ARE_NOT_IN_A_PARTY;
			}
			else
			{
				CUltimateDungeon* pDungeon = m_pkOwner->GetUD();
				if (pDungeon)
				{
					CWorld* pWorld = pDungeon->GetWorld();

					m_pkOwner->StartTeleport(pWorld->GetTbldat()->outWorldLoc, pWorld->GetTbldat()->outWorldDir, pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_DUNGEON);
				}
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_TLQ:	//enter time leap dungeon
		{
			if (m_pkOwner->GetAspectStateId() == ASPECTSTATE_SPINNING_ATTACK || m_pkOwner->GetAspectStateId() == ASPECTSTATE_ROLLING_ATTACK)
				return RESULT_FAIL;

			CDboTSActTLQ* pTLQ = (CDboTSActTLQ*)pEntity;
			if (pTLQ)
			{
				if (pTLQ->GetDungeonType() == eTLQ_DUNGEON_TYPE_ENTER)
				{
					if (m_pkOwner->GetRankBattleRoomTblidx() != INVALID_TBLIDX)
						return GAME_RANKBATTLE_MEMBER_ALREADY_JOINED_RANKBATTLE;
					else if (m_pkOwner->GetDragonballScrambleBallFlag() > 0)
						return GAME_CAN_NOT_TELEPORT;

				/// NOT NEEDED I GUESS
				//	else if (m_pkOwner->GetParty() && m_pkOwner->GetParty()->IsSomeoneInDynamic(INVALID_WORLDID))
				//		return GAME_PARTYMATCHING_ANY_MEMBER_IN_DYNAMIC_WORLD;

					CTimeLeapDungeon* pDungeon = g_pDungeonManager->CreateTimeLeapDungeon(m_pkOwner, pTLQ->GetDungeonTblIdx());
					if (pDungeon == NULL)
						return RESULT_FAIL;

					CWorld* pWorld = pDungeon->GetWorld();

					m_pkOwner->StartTeleport(pWorld->GetTbldat()->vStart1Loc, pWorld->GetTbldat()->vStart1Dir, pWorld->GetID(), TELEPORT_TYPE_TIMEQUEST);

					m_pkOwner->SetTLQ(pDungeon);
				}
				else
				{
					CTimeLeapDungeon* pDungeon = m_pkOwner->GetTLQ();
					if (pDungeon)
					{
						CWorld* pWorld = pDungeon->GetWorld();

						m_pkOwner->StartTeleport(pWorld->GetTbldat()->outWorldLoc, pWorld->GetTbldat()->outWorldDir, pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_TIMEQUEST);
					}
				}
			}
			else return RESULT_FAIL;
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_BROAD_MSG:
		{
			CDboTSActBroadMsg* pAct = (CDboTSActBroadMsg*)pEntity;

			CNtlPacket packet(sizeof(sGU_BROAD_MESSAGE_BEG_NFY));
			sGU_BROAD_MESSAGE_BEG_NFY * res = (sGU_BROAD_MESSAGE_BEG_NFY *)packet.GetPacketData();
			res->wOpCode = GU_BROAD_MESSAGE_BEG_NFY;
			res->byOwnerCondition = pAct->GetOwnerCondition();
			res->byOwnerType = pAct->GetOwnerType();
			res->byUIBalloonShapeType = pAct->GetUIBalloonShapeType();
			res->byUIDirType = pAct->GetUIDirType();
			res->byUIShowHideTypel = pAct->GetUIShowHideDirType();
			res->byUISpeechDirType = pAct->GetUISpeechDirType();
			res->fDisplayTime = (float)pAct->GetDisplayTime();
			res->uiOwnerTblIdx = pAct->GetOwnerTableIndex();
			res->uiQuestTextTblidx = pAct->GetQuestTextTableIndex();
			packet.SetPacketLen(sizeof(sGU_BROAD_MESSAGE_BEG_NFY));
			m_pkOwner->SendPacket(&packet);
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_MINI_NARRATION:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_MINI_NARRATION - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_REG_C_TIMING:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_REG_C_TIMING - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_EXC_C_GROUP:
		{
			CDboTSActExcCGroup* pAct = (CDboTSActExcCGroup*)pEntity;

			if (pAct->IsRegister())
				pQuestProgress->RunGroup(pAct->GetExcuteClientTSGroupID());
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_SKIP_CONT:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_SKIP_CONT - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_DOJO: //go into dojo
		{
			CDboTSActDojo* pDojo = (CDboTSActDojo*)pEntity;
			if (pDojo)
			{
				if (pDojo->GetDungeonType() == eDOJO_DUNGEON_TYPE_LEAVE && m_pkOwner->GetCurWorld())
				{
					sWORLD_TBLDAT * pTWorld = m_pkOwner->GetCurWorld()->GetTbldat();

					m_pkOwner->TeleportAnotherServer(pTWorld->outWorldLoc, pTWorld->outWorldDir, pTWorld->outWorldTblidx, pTWorld->outWorldTblidx, TELEPORT_TYPE_DOJO, m_pkOwner->GetPrevChannelID(), 0, false);
				}
				else if (CDojo* pDojoWar = g_pDojoWarManager->GetDojo(m_pkOwner->GetGuildID()))
				{
					if (pDojoWar->GetWarState() == eDBO_DOJO_STATUS_READY || pDojoWar->GetWarState() == eDBO_DOJO_STATUS_START)
					{
						if (pDojoWar->GetDojoTblidx() == pDojo->GetDojoTblIdx())
						{
							sDOJO_TBLDAT* dojoTbl = (sDOJO_TBLDAT*)g_pTableContainer->GetDojoTable()->FindData(pDojo->GetDojoTblIdx());

							sWORLD_TBLDAT * pTWorld = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(dojoTbl->zoneTblidx);

							if (pDojo->GetDungeonType() == eDOJO_DUNGEON_TYPE_ENTER)
							{
								if (pDojoWar->GetAttGuildID() == m_pkOwner->GetGuildID())
								{
									CNtlVector vDst(pTWorld->vStart1Loc);
									vDst.x = -64.f;
									vDst.y = 84.100f;
									vDst.z = -43.7f;
									m_pkOwner->TeleportAnotherServer(vDst, pTWorld->vStart1Dir, pTWorld->tblidx, pTWorld->tblidx, TELEPORT_TYPE_DOJO, DOJO_CHANNEL_INDEX, 0, false);
								}
								else
									m_pkOwner->TeleportAnotherServer(pTWorld->vStart2Loc, pTWorld->vStart2Dir, pTWorld->tblidx, pTWorld->tblidx, TELEPORT_TYPE_DOJO, DOJO_CHANNEL_INDEX, 0, false);
							}
							else
							{
								m_pkOwner->TeleportAnotherServer(pTWorld->outWorldLoc, pTWorld->outWorldDir, pTWorld->outWorldTblidx, pTWorld->outWorldTblidx, TELEPORT_TYPE_DOJO, m_pkOwner->GetPrevChannelID(), 0, false);
							}
						}
						else return RESULT_FAIL;
					}
					else return RESULT_FAIL;
				}
				else if (sDBO_DOJO_BRIEF* dojobrief = g_pDojoManager->GetDojoBrief(m_pkOwner->GetGuildID())) // only dojo owner guild member if no war exist
				{
					sDOJO_TBLDAT* dojoTbl = (sDOJO_TBLDAT*)g_pTableContainer->GetDojoTable()->FindData(dojobrief->dojoTblidx);

					if (pDojo->GetDungeonType() == eDOJO_DUNGEON_TYPE_ENTER)
					{
						if (dojobrief->dojoTblidx == pDojo->GetDojoTblIdx())
						{
							sWORLD_TBLDAT * pTWorld = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(dojoTbl->zoneTblidx);

							m_pkOwner->TeleportAnotherServer(pTWorld->vStart2Loc, pTWorld->vStart2Dir, pTWorld->tblidx, pTWorld->tblidx, TELEPORT_TYPE_DOJO, DOJO_CHANNEL_INDEX, 0, false);
						}
						else return GAME_GUILD_DOJO_YOU_DONT_BELONG_TO_THAT_DOJO;
					}
					else
					{
						sWORLD_TBLDAT * pTWorld = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(dojoTbl->zoneTblidx);

						m_pkOwner->TeleportAnotherServer(pTWorld->outWorldLoc, pTWorld->outWorldDir, pTWorld->outWorldTblidx, pTWorld->outWorldTblidx, TELEPORT_TYPE_DOJO, m_pkOwner->GetPrevChannelID(), 0, false);
					}
				}
				else return RESULT_FAIL;
			}
			else return RESULT_FAIL;
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_OUT_MSG:
		{
			CDboTSActOutMsg* pOutMsg = (CDboTSActOutMsg*)pEntity;
			m_pkOwner->GetClientSession()->SendPcTsDialogNfy(pOutMsg->GetIdx());
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_WPSFD:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_WPSFD - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_MAIL:
		{
			//NTL_PRINT(PRINT_APP,"QUEST-PROCESS: DBO_ACT_TYPE_ID_ACT_MAIL - NEED TO DO tId %d  -", tId);
		}break;
		case DBO_ACT_TYPE_ID_ACT_AVATARDEAD:
		{
			CDboTSActAvatarDead* pAct = (CDboTSActAvatarDead*)pEntity;

			if (pQuestProgress)
				pQuestProgress->SetFailOnDeath(pAct->GetStart() == 1);

		}
		break;
		

		default:
		{
			NTL_PRINT(PRINT_APP,"QUEST-PROCESS: (DEFAULT) %s - NEED TO DO tId %d -", pEntity->GetClassNameA(), tId);
		}
	}

	return RESULT_SUCCESS;
}


//--------------------------------------------------------------------------------------//
//		CHECK IF THE QUEST SHOULD BE STORED IN DATABASE
//--------------------------------------------------------------------------------------//
bool CQuest::CanStoreQuestInDatabase(QUESTID tId)
{
	return tId < eQUEST_ID_RANGE_SPECIAL_QUEST_MIN; //if quest id smaller than eQUEST_ID_RANGE_SPECIAL_QUEST_MIN then quest should be stored in database
}

//--------------------------------------------------------------------------------------//
//		CHECK IF THE QUEST SHOULD BE STORED IN DATABASE
//--------------------------------------------------------------------------------------//
bool CQuest::IsDungeonQuestID(QUESTID tId)
{
	return tId >= eQUEST_ID_RANGE_SPECIAL_QUEST_MIN;
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CQuest::PlayerDied()
{
	for (std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.begin(); it != m_pMapQuestInProgress.end(); it++)
	{
		CQuestProgress* pQuest = it->second;
		if (pQuest)
		{
			pQuest->OnPlayerDied();
		}
	}
}

void CQuest::WpsFailed(DWORD dwWpsIndex)
{
	for (std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.begin(); it != m_pMapQuestInProgress.end(); it++)
	{
		CQuestProgress* pQuest = it->second;
		if (pQuest && pQuest->GetWpsScriptID() == dwWpsIndex)
		{
			pQuest->OnWpsFailed();
			break;
		}
	}
}

//--------------------------------------------------------------------------------------//
//		CALLED WHEN LEAVING DUNGEON
//--------------------------------------------------------------------------------------//
void CQuest::LeaveWorld(bool bLogout/* = false*/, bool bLeaveTMQ/* = false*/)
{
	//loop active quest
	for (std::map<QUESTID, CQuestProgress*>::iterator it = m_pMapQuestInProgress.begin(); it != m_pMapQuestInProgress.end(); )
	{
		CQuestProgress* pQuest = it->second;
		if (pQuest)
		{
			if (IsDungeonQuestID(it->first))
			{
				m_vErasedDungeonQuestIDs.push_back(it->first);

				CNtlPacket packet2(sizeof(sGU_QUEST_GIVEUP_RES));
				sGU_QUEST_GIVEUP_RES * res2 = (sGU_QUEST_GIVEUP_RES *)packet2.GetPacketData();
				res2->wOpCode = GU_QUEST_GIVEUP_RES;
				res2->tId = it->first;
				res2->wResultCode = GAME_SUCCESS; //INVALID TO AVOID IN-GAME ANNOUNCEMENT THAT THE QUEST HAS BEEN GIVEN UP
				m_pkOwner->SendPacket(&packet2);

				pQuest->RunGroup(NTL_TS_EXCEPT_GIVEUP_ID);	//run give up group

				UpdateQuestCounter(false, it->first);

				it = m_pMapQuestInProgress.erase(it);

				RemoveSlotID(pQuest->GetProgressInfo()->bySlotId);

				SAFE_DELETE(pQuest);
			}
			else
			{
				if (bLogout && pQuest->IsFailOnLogout())
				{
					if (pQuest->IsInProgressState())
					{
						if(pQuest->GetWpsScriptID())
							pQuest->GetProgressInfo()->uData.sQInfoV0.wQState = eTS_SVR_STATE_USER_OUT_FAILED_WPS;
						else
							pQuest->GetProgressInfo()->uData.sQInfoV0.wQState = eTS_SVR_STATE_FAILED;
					}
				}

				++it;
			}
		}
		else ++it;
	}

	//required for TMQ
	if (!bLogout && bLeaveTMQ)
	{
		CNtlPacket packet(sizeof(sGU_QUEST_RESET_NFY));
		sGU_QUEST_RESET_NFY * res = (sGU_QUEST_RESET_NFY *)packet.GetPacketData();
		res->wOpCode = GU_QUEST_RESET_NFY;
		res->startResetQID = INVALID_QUESTID;
		res->endResetQID = 0;

		for (std::vector<QUESTID>::iterator it = m_vErasedDungeonQuestIDs.begin(); it != m_vErasedDungeonQuestIDs.end(); it++)
		{
			QUESTID qId = *it;

			if (qId < res->startResetQID)
				res->startResetQID = qId;

			if (qId > res->endResetQID)
				res->endResetQID = qId;

			if (HasBeenClearQuest(qId))
				UnsetClearQuest(qId);
		}

		m_pkOwner->SendPacket(&packet);

	//	printf("m_vErasedDungeonQuestIDs.size() = %I64u, res->startResetQID = %u, res->endResetQID = %u\n", m_vErasedDungeonQuestIDs.size(), res->startResetQID, res->endResetQID);
	}

	m_vErasedDungeonQuestIDs.clear();
}

bool CQuest::CheckQuestCounter(QUESTID tId)
{
	//printf("m_byGeneralQuestsCount %u m_bySpecialQuestsCount %u\n", m_byGeneralQuestsCount, m_bySpecialQuestsCount);
	if (tId < eQUEST_ID_RANGE_SPECIAL_QUEST_MIN)
	{
		return m_byGeneralQuestsCount < eMAX_CAN_PROGRESS_GENERAL_QUEST_NUM;
	}
	else
	{
		return m_bySpecialQuestsCount < eMAX_CAN_PROGRESS_SPECIAL_QUEST_NUM;
	}
}

void CQuest::UpdateQuestCounter(bool bIncrease, QUESTID tId)
{
	if (bIncrease)
	{
		if (tId < eQUEST_ID_RANGE_SPECIAL_QUEST_MIN)
			++m_byGeneralQuestsCount;
		else
			++m_bySpecialQuestsCount;
	}
	else
	{
		if (tId < eQUEST_ID_RANGE_SPECIAL_QUEST_MIN)
			--m_byGeneralQuestsCount;
		else
			--m_bySpecialQuestsCount;
	}
}


bool CQuest::CheckQuestItem(eQUEST_ITEM_CHECK_TYPE checktype, int haveCnt, int needCnt)
{
	switch (checktype)
	{
		case eQUEST_ITEM_CHECK_TYPE_EQUAL:
		{
			if (haveCnt != needCnt)
				return false;
		}
			break;
		case eQUEST_ITEM_CHECK_TYPE_MORE:
		{
			if (haveCnt < needCnt)
				return false;
		}
			break;
		case eQUEST_ITEM_CHECK_TYPE_LESS:
		{
			if (haveCnt > needCnt)
				return false;
		}
	}

	return true;
}

void CQuest::UnequipAndDeleteQuestItem()
{
	CItem* item = m_pkOwner->GetPlayerItemContainer()->GetItem(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_QUEST);
	if (item)
	{
		if (m_pkOwner->UnequipItem(item))
			item->SetCount(0, false, true);
		else
			ERR_LOG(LOG_USER, "UnequipAndDeleteQuestItem Failed. Unequip failed. Hacker ?! Player %d", m_pkOwner->GetCharID());
	}
}