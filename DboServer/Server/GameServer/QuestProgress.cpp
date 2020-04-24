#include "stdafx.h"
#include "CPlayer.h" //Quest.h, QuestProgress.h
#include "NtlPacketGU.h"
#include "TriggerManager.h"
#include "WpsAlgoObject.h"
#include "NtlResultCode.h"


CQuestProgress::CQuestProgress(CPlayer* pOwner)
{
	Init();

	m_pOwner = pOwner;
}

CQuestProgress::CQuestProgress(CPlayer* pOwner, QUESTID tId, NTL_TS_TC_ID tcId, BYTE bySlot)
{
	Init();

	m_pOwner = pOwner;

	m_QuestProgressInfo.bySlotId = bySlot;
	m_QuestProgressInfo.tId = tId;
	m_QuestProgressInfo.uData.sQInfoV0.tgExcCGroup = NTL_TS_MAIN_GROUP_ID;
	m_QuestProgressInfo.uData.sQInfoV0.tcQuestInfo = tcId;
	m_QuestProgressInfo.uData.sQInfoV0.taQuestInfo = START_CONTAINER_ID;
	m_QuestProgressInfo.uData.sQInfoV0.sMainTSP.tcCurId = tcId;
	m_QuestProgressInfo.uData.sQInfoV0.sMainTSP.tcPreId = START_CONTAINER_ID;
}

CQuestProgress::~CQuestProgress()
{
}




//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CQuestProgress::Init()
{
	for (int i = 0; i < sSTOC_EVT_DB_DATA::eSTOC_EVT_DB_DATA_USER_DATA_MAX; i++)
		m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i] = 0;

	for (int i = 0; i < sEXCEPT_TIMER_SLOT::eTIMER_SLOT_MAX; i++)
	{
		m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].taId = NTL_TS_TA_ID_INVALID;
		m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].taId = NTL_TS_TC_ID_INVALID;
		m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].uiRemainTime = 0;
	}

	memset(m_pQuestTimer, NULL, sizeof(m_pQuestTimer));

	m_bFailOnDeath = false;
	m_bFailOnPlayerLogout = false;
	m_dwWpsScript = INVALID_DWORD;
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CQuestProgress::TickProcess(DWORD dwTickDiff)
{
	if (!IsInProgressState()) //only process timer if quest is still active !
		return;

	for (int i = 0; i < sEXCEPT_TIMER_SLOT::eTIMER_SLOT_MAX; i++)
	{
		if (m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].tcId != NTL_TS_TC_ID_INVALID)
		{
			m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].uiRemainTime = UnsignedSafeDecrease<DWORD>(m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].uiRemainTime, dwTickDiff);

			if (m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].uiRemainTime == 0)
			{
				if (GetFailOnDeath() == false) //do not end quest if fail on death is true. This timer is only used so player has to survive X seconds.
				{
					CDboTSActETimerS* pTimer = GetTimer(i);
					if (pTimer)
					{
						//reset time because some quests have to run group for checks etc
						m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].uiRemainTime = pTimer->GetTime();

						//run group from timer
						RunGroup(pTimer->GetTGId());
					}
					else
					{
					//	ERR_LOG(LOG_QUEST, "Char %u Could not find CDboTSActETimerS. Quest %u, Timer pos %u, Timer Container ID %u", m_pOwner->GetCharID(), m_QuestProgressInfo.tId, i, m_QuestProgressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[i].tcId);
						//TODO: Find out !!! Quest 53, timer pos 0, container id = 2
					}
				}
			}
		}
	}
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CQuestProgress::IsInProgressState()
{
	return m_QuestProgressInfo.uData.sQInfoV0.wQState == 0
		|| m_QuestProgressInfo.uData.sQInfoV0.wQState == eTS_SVR_STATE_ESCORT
		|| m_QuestProgressInfo.uData.sQInfoV0.wQState == eTS_SVR_STATE_SVR_EVT_WORLD_RANGE
		|| m_QuestProgressInfo.uData.sQInfoV0.wQState == eTS_SVR_STATE_WPS_MODE;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
WORD CQuestProgress::UpdateQuestObjectItem(TBLIDX ItemTblidx, BYTE byCount)
{
	CNtlTSTrigger* pTrig = (CNtlTSTrigger*)g_pTriggerManager->FindQuestFromTS(m_QuestProgressInfo.tId);
	if (pTrig)
	{
		CDboTSContGAct* pAct = (CDboTSContGAct*)pTrig->GetGroup(m_QuestProgressInfo.uData.sQInfoV0.tgExcCGroup)->GetChildCont(m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.tcId); //check if create item by stoc event
		if (pAct)
		{
			CNtlTSEntity* pEntity = pAct->FindChildEntity(m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.taId);
			if (pEntity)
			{
				CDboTSActSToCEvt* pEvt = (CDboTSActSToCEvt*)pEntity;
				const uSTOC_EVT_DATA& evtdata = pEvt->GetEvtData();

				for (int i = 0; i < evtdata.MAX_OBJECT_ITEM; i++)
				{
					if (evtdata.sObjectItemCnt[i].uiItemIdx == ItemTblidx)
					{
						if (m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i] >= (unsigned int)evtdata.sObjectItemCnt[i].nItemCnt)
							return GAME_TS_WARNING_ALREADY_VISITED;

						m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i] += (unsigned int)byCount;

						BYTE byCreateCnt = byCount;

						if (m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i] > (unsigned int)evtdata.sObjectItemCnt[i].nItemCnt)
						{
							byCreateCnt = m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i] - (unsigned int)evtdata.sObjectItemCnt[i].nItemCnt;

							m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i] = (unsigned int)evtdata.sObjectItemCnt[i].nItemCnt;
						}

						if (m_pOwner->GetQuests()->CreateQuestItem(ItemTblidx, byCreateCnt))
						{
							CNtlPacket packet(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
							sGU_QUEST_SVREVT_UPDATE_NFY * res = (sGU_QUEST_SVREVT_UPDATE_NFY *)packet.GetPacketData();
							res->wOpCode = GU_QUEST_SVREVT_UPDATE_NFY;
							res->taId = m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.taId;
							res->tcId = m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.tcId;
							res->tId = m_QuestProgressInfo.tId;
							res->bySlot = (BYTE)i;
							res->bySvrEvtType = eSTOC_EVT_DATA_TYPE_OBJECT_ITEM;
							res->uEvtData.sObjectItemCnt.nCurItemCnt = (int)m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i];
							packet.SetPacketLen(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
							m_pOwner->SendPacket(&packet);

							return RESULT_SUCCESS;
						}
						else
						{
							m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i] = UnsignedSafeDecrease<UINT32>(m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[i], (UINT32)byCreateCnt);
							return GAME_TS_WARNING_QUEST_INVENTORY_IS_FULL;
						}
					}
				}
			}
		}
		else if (m_QuestProgressInfo.uData.sQInfoV0.sSToCEvtData.tcId == NTL_TS_TC_ID_INVALID) //If we dont create item by stoc event
		{
			if (m_pOwner->GetQuests()->CreateQuestItem(ItemTblidx, byCount))
			{
				return RESULT_SUCCESS;
			}
			else
			{
				return GAME_TS_WARNING_QUEST_INVENTORY_IS_FULL;
			}
		}
	}

	return RESULT_FAIL;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CQuestProgress::OnPlayerDied()
{
	if (GetFailOnDeath())
	{
		if (!m_QuestProgressInfo.uData.sQInfoV0.wQState)
		{
			m_QuestProgressInfo.uData.sQInfoV0.wQState = eTS_SVR_STATE_FAILED;

			CNtlPacket packet(sizeof(sGU_TS_UPDATE_STATE));
			sGU_TS_UPDATE_STATE * res = (sGU_TS_UPDATE_STATE *)packet.GetPacketData();
			res->wOpCode = GU_TS_UPDATE_STATE;
			res->tId = m_QuestProgressInfo.tId;
			res->byTsType = TS_TYPE_QUEST_CS;
			res->byType = eTSSTATE_TYPE_ADD;
			res->wTSState = eTS_SVR_STATE_FAILED;
			packet.SetPacketLen(sizeof(sGU_TS_UPDATE_STATE));
			m_pOwner->SendPacket(&packet);
		}
	}
}

void CQuestProgress::OnWpsFailed()
{
	if (m_QuestProgressInfo.uData.sQInfoV0.wQState != eTS_SVR_STATE_FAILED)
	{
		m_QuestProgressInfo.uData.sQInfoV0.wQState = eTS_SVR_STATE_FAILED;

		CNtlPacket packet(sizeof(sGU_TS_UPDATE_STATE));
		sGU_TS_UPDATE_STATE * res = (sGU_TS_UPDATE_STATE *)packet.GetPacketData();
		res->wOpCode = GU_TS_UPDATE_STATE;
		res->tId = m_QuestProgressInfo.tId;
		res->byTsType = TS_TYPE_QUEST_CS;
		res->byType = eTSSTATE_TYPE_ADD;
		res->wTSState = eTS_SVR_STATE_FAILED;
		packet.SetPacketLen(sizeof(sGU_TS_UPDATE_STATE));
		m_pOwner->SendPacket(&packet);

		SetWpsScriptID(INVALID_DWORD);
	}
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CQuestProgress::RunGroup(NTL_TS_TG_ID gId)
{
	CNtlTSTrigger* pTrig = (CNtlTSTrigger*)g_pTriggerManager->FindQuestFromTS(m_QuestProgressInfo.tId);
	if (pTrig)
	{
		CNtlTSGroup* group = (CNtlTSGroup*)pTrig->GetGroup(gId);
		if (group == NULL)
			return;

		NTL_TS_TC_ID tcId = START_CONTAINER_ID;

		for (int x = 0; x < group->GetNumOfChildCont(); x++)
		{
			CNtlTSCont* pCurCont = (CNtlTSCont*)group->GetChildCont(tcId);
			if (pCurCont)
			{
				switch (pCurCont->GetEntityType())
				{
					case DBO_CONT_TYPE_ID_CONT_GCOND:
					{
						CDboTSContGCond* pCond = (CDboTSContGCond*)pCurCont;
						tcId = pCond->GetYesLinkID();

						for (int i = 0; i < pCond->GetNumOfChildEntity(); i++)
						{
							if (m_pOwner->GetQuests()->ProgressTsEntity(pCond->GetChildEntity(i), m_QuestProgressInfo.tId, this))
							{
								tcId = pCond->GetNoLinkID();
								break;
							}
						}
					}
					break;

					case DBO_CONT_TYPE_ID_CONT_START:
					{
						CDboTSContStart* pStart = (CDboTSContStart*)pCurCont;

						tcId = pStart->GetYesLinkID();

						for (int i = 0; i < pStart->GetNumOfChildEntity(); i++)
						{
							if (m_pOwner->GetQuests()->ProgressTsEntity(pStart->GetChildEntity(i), m_QuestProgressInfo.tId, this))
							{
								tcId = pStart->GetNoLinkID();
								break;
							}
						}
					}
					break;

					case DBO_CONT_TYPE_ID_CONT_GACT:
					{
						CDboTSContGAct* pAct = (CDboTSContGAct*)pCurCont;

						tcId = pAct->GetNextLinkID();

						for (int i = 0; i < pAct->GetNumOfChildEntity(); i++)
							m_pOwner->GetQuests()->ProgressTsEntity(pAct->GetChildEntity(i), m_QuestProgressInfo.tId, this);
					}
					break;

					case DBO_CONT_TYPE_ID_CONT_END:
					{
						CDboTSContEnd* pEnd = (CDboTSContEnd*)pCurCont;

					//	printf("CQuestProgress::RunGroup(%u): end type %u, tcId %u \n", gId, pEnd->GetEndType(), tcId);
					}
					break;

					default:
					{
						ERR_LOG(LOG_USER, "PLAYER %u PROCESS QUEST %u container id %u group %u FAILED. Cant find pCurCont->GetEntityType() %s", m_pOwner->GetCharID(), m_QuestProgressInfo.tId, tcId, group->GetID(), pCurCont->GetClassNameA());
					}
					break;
				}
			}
			else
			{
				if (tcId != NTL_TS_TC_ID_INVALID)
					ERR_LOG(LOG_USER, "Container not found. quest id %d group id %d container id %d player %u", m_QuestProgressInfo.tId, gId, tcId, m_pOwner->GetCharID());

				break;
			}
		}
	}
}

