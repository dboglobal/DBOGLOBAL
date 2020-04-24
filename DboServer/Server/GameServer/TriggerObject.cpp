#include "stdafx.h"
#include "TriggerObject.h"
#include "GameServer.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "TriggerManager.h"
#include "NtlResultCode.h"
#include "Npc.h"
#include "WpsAlgoObject.h"
#include "TimeQuest.h"
#include "TqsAlgoObject.h"
#include "NtlRandom.h"
#include "ScriptAlgoObjectManager.h"
#include "BotState_Operating.h"
#include "QuestProbabilityTable.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"
#include "GameMain.h"


CTriggerObject::CTriggerObject() :
	CSpawnObject(OBJTYPE_TOBJECT)
{
	Initialize();
}

CTriggerObject::~CTriggerObject()
{
}


//--------------------------------------------------------------------------------------//
//		INIT
//--------------------------------------------------------------------------------------//
void CTriggerObject::Initialize()
{
	m_pObjTbldat = NULL;
	tobjectState.byState = TOBJECT_STATE_NONE;
	tobjectState.bySubStateFlag = TOBJECT_SUBSTATE_SHOW;

	m_bIsExecuting = false;
	m_executer.pTSTrigger = NULL;
	m_executer.Init();
}

//--------------------------------------------------------------------------------------//
//		CREATE
//--------------------------------------------------------------------------------------//
void CTriggerObject::Create(HOBJECT hHandle)
{
	SetID(hHandle);
}


//--------------------------------------------------------------------------------------//
//		SET TRIGGER OBJECT TABLE DATA
//--------------------------------------------------------------------------------------//
void CTriggerObject::SetToObjTable(sOBJECT_TBLDAT* pObjTblData)
{
	m_pObjTbldat = pObjTblData;

	tobjectState.byState = m_pObjTbldat->byDefMainState;
	tobjectState.bySubStateFlag = m_pObjTbldat->byDefSubState;
	tobjectState.dwStateTime = 0;
}

//--------------------------------------------------------------------------------------//
//		UPDATE OBJECT TRIGGER
//--------------------------------------------------------------------------------------//
void CTriggerObject::TickProcess(DWORD dwTickDiff)
{
	if (!m_bIsExecuting)
		return;

	m_executer.dwWaitTick = UnsignedSafeDecrease<DWORD>(m_executer.dwWaitTick, dwTickDiff);

	if (m_executer.dwWaitTick > 0)
		return;

	CNtlTSCont* pCurCont = (CNtlTSCont*)m_executer.pTSTrigger->GetGroup(NTL_TS_MAIN_GROUP_ID)->GetChildCont(m_executer.tcID);
	if (pCurCont)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_executer.hPc);

		if (pPlayer == NULL || pPlayer->IsInitialized() == false)
		{
			StopExecuting(true, false);
			return;
		}

		switch (pCurCont->GetEntityType())
		{
			case DBO_CONT_TYPE_ID_CONT_GCOND:
			{
				CDboTSContGCond* pCond = (CDboTSContGCond*)pCurCont;
				if (pCond)
				{
					m_executer.tcID = pCond->GetYesLinkID();

					for (int i = 0; i < pCurCont->GetNumOfChildEntity(); i++)
					{
						if (ProcessTsEntity(pCond->GetChildEntity(i), pPlayer) != RESULT_SUCCESS)
						{
							m_executer.tcID = pCond->GetNoLinkID();
							break;
						}
					}
				}
			}
			break;
			case DBO_CONT_TYPE_ID_CONT_GACT:
			{
				CDboTSContGAct* pAct = (CDboTSContGAct*)pCurCont;
				if (pAct)
				{
					m_executer.tcID = pAct->GetNextLinkID();

					for (int i = 0; i < pCurCont->GetNumOfChildEntity(); i++)
					{
						if (ProcessTsEntity(pAct->GetChildEntity(i), pPlayer) != RESULT_SUCCESS)
						{
							m_executer.tcID = pAct->GetErrorLinkID();
							break;
						}
					}
				}
			}
			break;
			case DBO_CONT_TYPE_ID_CONT_END:
			{
				//printf("DBO_CONT_TYPE_ID_CONT_END \n");
				StopExecuting(false, false);
			}
			break;
			case DBO_CONT_TYPE_ID_CONT_SWITCH:
			{
				CDboTSContSwitch* pSwitch = (CDboTSContSwitch*)pCurCont;
				if (pSwitch)
				{
					for (int i = 0; i < pSwitch->GetNumOfChildEntity(); i++)
					{
						ProcessTsEntity(pSwitch->GetChildEntity(i), pPlayer);

						m_executer.tcID = pSwitch->GetTCIdFromBId(m_executer.tcID);
						break;
					}
				}
			}
			break;

			default:
			{
				ERR_LOG(LOG_QUEST, "ERROR: GetEntityType not set %s. Trigger obj %u", pCurCont->GetClassNameA(), m_executer.pTSTrigger->GetID());
				StopExecuting(true, false);
			}
			break;
		}
	}
	else
	{
		StopExecuting(true, false);
	}
}

void CTriggerObject::CopyToObjectInfo(sOBJECT_INFO * pObjectInfo, CHARACTERID playerCharID)
{
	//printf("CopyToObjectInfo %u %u %u \n", GetTblidx(), GetMainState(), GetSubState());
	pObjectInfo->tobjectBrief.objectID = GetTblidx();
	pObjectInfo->tobjectState.byState = GetMainState();
	pObjectInfo->tobjectState.bySubStateFlag = GetSubState();
	pObjectInfo->tobjectState.dwStateTime = GetStateTime();
}


//--------------------------------------------------------------------------------------//
//		UPDATE STATE
//--------------------------------------------------------------------------------------//
void CTriggerObject::UpdateToObjectState(BYTE byMainState, BYTE bySubStateFlag, DWORD dwTime)
{

	CNtlPacket packet(sizeof(sGU_TOBJECT_UPDATE_STATE));
	sGU_TOBJECT_UPDATE_STATE * res = (sGU_TOBJECT_UPDATE_STATE *)packet.GetPacketData();
	res->wOpCode = GU_TOBJECT_UPDATE_STATE;
	res->handle = GetID();
	res->tobjectBrief.objectID = GetTblidx();
	res->tobjectState.byState = byMainState;
	res->tobjectState.bySubStateFlag = bySubStateFlag;
	res->tobjectState.dwStateTime = dwTime;
	packet.SetPacketLen(sizeof(sGU_TOBJECT_UPDATE_STATE));

	tobjectState = res->tobjectState;


	if (GetCurWorld() && GetCurWorld()->GetTbldat()->bDynamic)
		GetCurWorld()->Broadcast(&packet);
	else
		Broadcast(&packet);
}


void CTriggerObject::EncodeInsertPacket(CPlayer * pPlayer)
{
	if (tobjectState.byState != m_pObjTbldat->byDefMainState || tobjectState.bySubStateFlag != m_pObjTbldat->byDefSubState) //check if state was changed
	{
		CNtlPacket packet(sizeof(sGU_TOBJECT_UPDATE_STATE));
		sGU_TOBJECT_UPDATE_STATE * res = (sGU_TOBJECT_UPDATE_STATE *)packet.GetPacketData();
		res->wOpCode = GU_TOBJECT_UPDATE_STATE;
		res->handle = GetID();
		res->tobjectBrief.objectID = GetTblidx();
		res->tobjectState.byState = tobjectState.byState;
		res->tobjectState.bySubStateFlag = tobjectState.bySubStateFlag;
		res->tobjectState.dwStateTime = tobjectState.dwStateTime;
		packet.SetPacketLen(sizeof(sGU_TOBJECT_UPDATE_STATE));
		pPlayer->SendPacket(&packet);
	}
}


bool CTriggerObject::StartExecuting(CPlayer * pPlayer, bool bIsQuestIdTriggerId, QUESTID questId)
{
	if (bIsQuestIdTriggerId)
	{
		CNtlTSMain::mapdef_TLIST::iterator it = g_pTriggerManager->m_defObject.find(questId);
		if (it != g_pTriggerManager->m_defObject.end())
		{
			CNtlTSTrigger* pTrig = it->second;

			if (OnStartExecuting(pTrig, pPlayer, questId) == 0)
				return true;
		}
		
		return false;
	}
	else
	{
		for (CNtlTSMain::mapdef_TLIST::iterator it = g_pTriggerManager->m_defObject.begin(); it != g_pTriggerManager->m_defObject.end(); it++)
		{
			CNtlTSTrigger* pTrig = (CNtlTSTrigger*)it->second;

			if (OnStartExecuting(pTrig, pPlayer, questId) != 1)
				break;
		}
	}

	return true;
}

// Return:
// - 0 everything good (loop will stop), 
// - 1 to continue loop, 
// - 2 cancel loop because of fail
int CTriggerObject::OnStartExecuting(CNtlTSTrigger * pTrig, CPlayer * pPlayer, QUESTID questId)
{
	CDboTSContStart* pStart = (CDboTSContStart*)pTrig->GetGroup(NTL_TS_MAIN_GROUP_ID)->GetChildCont(START_CONTAINER_ID);
	if (pStart)
	{
		bool bEnity = true;

		if (pStart->GetNumOfChildEntity() == 0 && questId == INVALID_QUESTID) //if dont have any child entity and questID is INVALID then continue. This is to avoid that when try to execute obj 11101 but 11030 will be executed since 11030 has no child entity. (TMQ1)
			return 1;

		for (int i = 0; i < pStart->GetNumOfChildEntity(); i++)
		{
			CNtlTSEntity* pEntity = pStart->GetChildEntity(i);

			if (ProcessTsEntity(pEntity, pPlayer) != RESULT_SUCCESS)
			{
				m_executer.questID = INVALID_QUESTID;

				bEnity = false;
				return 1;
			}
		}

		if (bEnity)
		{
			m_bIsExecuting = true;
			m_executer.pTSTrigger = pTrig;
			m_executer.hPc = pPlayer->GetID();
			m_executer.dwStartTickCount = GetTickCount64();
			m_executer.dwWaitTick = 0;

			if (m_executer.questID == INVALID_QUESTID)
				m_executer.questID = questId;

			m_executer.taID = 1;
			m_executer.tcID = pStart->GetYesLinkID();

			//NTL_PRINT(PRINT_APP, "StartObjTrigger: tId %u, pStart->GetYesLinkID() %u, questId %u", pStart->GetID(), pStart->GetYesLinkID(), m_executer.questID);

			return 0;
		}
	}

	return 1;
}

WORD CTriggerObject::ProcessTsEntity(CNtlTSEntity * pEntity, CPlayer* pPlayer)
{
	//NTL_PRINT(PRINT_APP, "ENTITY-PROCESS: %s", pEntity->GetClassNameA());

	switch (pEntity->GetEntityType())
	{
		case DBO_COND_TYPE_ID_CHECK_LVL: //Check pc level
		{
			CDboTSCheckLvl* pLvl = (CDboTSCheckLvl*)pEntity;

			if (pPlayer->GetLevel() < pLvl->GetMinLvl() || pPlayer->GetLevel() > pLvl->GetMaxLvl()) //check if pc lower than min lv or higher than max lv
				return GAME_TS_WARNING_WRONG_CONDITION;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_PCCLS: //check pc class
		{
			CDboTSCheckPCCls* pClass = (CDboTSCheckPCCls*)pEntity;

			if (!(pClass->GetClsFlags() & 1 << pPlayer->GetClass()))
				return GAME_TS_WARNING_WRONG_CONDITION;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_PCRACE: //check pc race
		{
			CDboTSCheckPCRace* pRace = (CDboTSCheckPCRace*)pEntity;

			if (!(pRace->GetRaceFlags() & 1 << pPlayer->GetRace()))
				return GAME_TS_WARNING_WRONG_CONDITION;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_QITEM: //check if pc got quest item
		{
			CDboTSCheckQItem* pQItem = (CDboTSCheckQItem*)pEntity;

			int nSum = 0;

			for (BYTE i = 0; i < NTL_QUEST_INVENTORY_SLOT_COUNT; i++)
			{
				sQUESTITEM_DATA* qitemdaa = pPlayer->GetQuests()->GetQuestItemData(i);
				if (qitemdaa)
				{
					if (qitemdaa->tblidx == pQItem->GetItemIdx())
						nSum += qitemdaa->byCurStackCount;
				}
			}

			if (pPlayer->GetQuests()->CheckQuestItem(pQItem->GetCheckType(), nSum, pQItem->GetItemCnt()) == false)
				return GAME_TS_WARNING_WRONG_CONDITION;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_CLRQST: //check clear quest
		{
			CDboTSCheckClrQst* pClrQst = (CDboTSCheckClrQst*)pEntity;

			/*info
			"not" is in every quest 0
			*/

			//	NTL_PRINT(PRINT_APP,"QUEST-START: DBO_COND_TYPE_ID_CHECK_CLRQST GetNumOfNextQidLink %d", pClrQst->GetNumOfNextQidLink());
			std::vector<NTL_TS_T_ID>::const_iterator it;

			//check if previous quests
			const std::vector<NTL_TS_T_ID>& AndList = pClrQst->GetAndIdList();
			if (!AndList.empty())
			{
				bool bCleared = true;
				for (it = AndList.begin(); it != AndList.end(); it++)
				{
					bCleared = pPlayer->GetQuests()->HasBeenClearQuest(*it);

					if (!bCleared) //check if quest XX is not completed
					{
						return GAME_TS_WARNING_WRONG_CONDITION;
					}
				}
			}

			// check next quests | important: the client does not do this check but on server we must check it.
			if (pClrQst->GetFlink() == TRUE)
			{
				if (pClrQst->GetNumOfNextQidLink() > 0)
				{
					bool bNextCleared = false;
					for (int i = 0; i < pClrQst->GetNumOfNextQidLink(); i++)
					{
						if (i == 0)
							bNextCleared = pPlayer->GetQuests()->HasBeenClearQuest(pClrQst->BeginNextQidLink());
						else
							bNextCleared = pPlayer->GetQuests()->HasBeenClearQuest(pClrQst->NextNextQidLink());

						if (bNextCleared) //check if quest XX has been cleared (should not happen)
						{
							return GAME_TS_WARNING_WRONG_CONDITION;
						}
					}
				}
			}
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_ITEM:
		{
			CDboTSCheckItem* pCheckItem = (CDboTSCheckItem*)pEntity;

			CItem* pItem = pPlayer->GetPlayerItemContainer()->GetItemByIdx(pCheckItem->GetItemIdx());
			if (pItem)
			{
				if (pItem->GetCount() < pCheckItem->GetItemCnt())
					return GAME_TS_WARNING_WRONG_CONDITION;

			}
			else return GAME_TS_WARNING_CANNOT_FIND_QUEST_ITEM;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_WITEM: //check if has item on quest slot?
		{
			CDboTSCheckWItem* pCheck = (CDboTSCheckWItem*)pEntity;

			if (pPlayer == NULL || pPlayer->IsInitialized() == false)
				return RESULT_FAIL;
			
			CItem* pItem = pPlayer->GetPlayerItemContainer()->GetItem(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_QUEST);
			if (pItem == NULL || pItem->GetTblidx() != pCheck->GetItemIdx())
				return RESULT_FAIL;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_OBJITEM:	// CHECK IF HAS QUEST PROGRESS (CDboTSContStart child)
		{
			CDboTSCheckObjItem* pCheckObjItem = (CDboTSCheckObjItem*)pEntity;

			if (pPlayer->GetQuests()->GetQuestProgress(pCheckObjItem->GetQuestId()) == NULL)
				return RESULT_FAIL;

			if (m_executer.questID == INVALID_QUESTID)
				m_executer.questID = pCheckObjItem->GetQuestId();
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_OBJ_STATE: //check object state. Note: It can be ANY object and not this one.
		{
			CDboTSCheckObjState* pCond = (CDboTSCheckObjState*)pEntity;

			if (pPlayer->GetWorldTblidx() == pCond->GetWorldIdx())
			{
				CTriggerObject* obj = pPlayer->GetCurWorld()->FindStaticObjectByIdx(pCond->GetObjectIdx());
				if (obj)
				{
					if (obj->GetMainState() == pCond->GetObjectMainState())
					{
						if (pCond->GetObjectShowHide() == CDboTSCheckObjState::SUB_STATE_OBJECT_IGNORE || BIT_FLAG_TEST(obj->GetSubState(), pCond->GetObjectShowHide()))
							return RESULT_SUCCESS;
					}
				}
			}

			return RESULT_FAIL;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_IN_WORLD:
		{
			CDboTSCheckInWorld* pWorld = (CDboTSCheckInWorld*)pEntity;

			if (pPlayer->GetWorldTblidx() != pWorld->GetWorldTblIdx())
				return RESULT_FAIL;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_OPERATEOBJECT:	//CHECK IF PLAYER IS STILL OPERATING THE OBJECT. IF NOT THEN CANCEL OPERATING
		{
			CDboTSCheckOPObject* pCheckOpObject = (CDboTSCheckOPObject*)pEntity;

			if (pPlayer && pPlayer->IsInitialized())
			{
				CBotState_Operating* pOperatingState = (CBotState_Operating*)pPlayer->GetStateManager()->GetCharState();
				if (pOperatingState)
				{
					if (pPlayer->GetCharStateID() == CHARSTATE_OPERATING && pOperatingState->GetTarget() == this->GetID())
						return RESULT_SUCCESS;
				}
			}

			return RESULT_FAIL;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_HASCOUPON:
		{
			CDboTSCheckHasCoupon* pCheck = (CDboTSCheckHasCoupon*)pEntity;

			if (pPlayer == NULL || pPlayer->IsInitialized() == false)
				return RESULT_FAIL;

			if (pPlayer->GetTMQ() == NULL)
				return RESULT_FAIL;

			if (pPlayer->GetTMQ()->GetCoupons() == 0)
				return RESULT_FAIL;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_ATTACH_OBJ: // CDboTSContStart child 
		{
			CDboTSCheckAttachObj* pAttachObj = (CDboTSCheckAttachObj*)pEntity;

			if (pAttachObj->HasObjectIdx(GetTblidx()) == false || pAttachObj->GetWorldIdx() != pPlayer->GetWorldTblidx())
			{
				return RESULT_FAIL;
			}

			if (pPlayer == NULL || pPlayer->IsExecutingTriggerObjectWithItem() == false)
				return RESULT_FAIL;
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_PROG_QUEST: // CDboTSContStart child 
		{
			CDboTSCheckProgQuest* pQuest = (CDboTSCheckProgQuest*)pEntity;

			if (pPlayer->GetQuests()->GetQuestProgressInfo(pQuest->GetQuestID()) == NULL)
				return RESULT_FAIL;

			if (m_executer.questID == INVALID_QUESTID)
				m_executer.questID = pQuest->GetQuestID();
		}
		break;
		case DBO_COND_TYPE_ID_CHECK_CUSTOMEVENT: //check if has quest (custom event)
		{
			CDboTSCheckCustomEvent* pCond = (CDboTSCheckCustomEvent*)pEntity;

			if (pPlayer->GetQuests()->GetQuestProgress(pCond->GetQuestID()) == NULL)
				return RESULT_FAIL;

			if (m_executer.questID == INVALID_QUESTID)
				m_executer.questID = pCond->GetQuestID();
		}
		break;

		/////////////////////////
		//	CHECK EVENT
		/////////////////////////

		case DBO_EVENT_TYPE_ID_CLICK_OBJECT: // CDboTSContStart child
		{
			CDboTSClickObject* pClick = (CDboTSClickObject*)pEntity;
			if (pClick->HasObjectIdx(GetTblidx()) == false || pClick->GetWorldIdx() != pPlayer->GetWorldTblidx())
			{
				return RESULT_FAIL;
			}
		}
		break;

		/////////////////////////
		//	CHECK ACTION
		/////////////////////////
		case DBO_ACT_TYPE_ID_ACT_ITEM:
		{
			if (pPlayer && pPlayer->IsInitialized())
			{
				CDboTSActItem* pAct = (CDboTSActItem*)pEntity;
				
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


					if (pPlayer->GetPlayerItemContainer()->CountEmptyInventory() < byItemCnt)
						return GAME_TS_WARNING_INVENTORY_IS_FULL;

					if (!g_pItemManager->CreateItem(pPlayer, itemInfo.uiItemIdx, 1))
					{
						return GAME_TS_WARNING_INVENTORY_IS_FULL;
					}

					if (itemInfo2.uiItemIdx != 0 && itemInfo2.uiItemIdx != INVALID_TBLIDX)
					{
						if (!g_pItemManager->CreateItem(pPlayer, itemInfo2.uiItemIdx, 1))
						{
							return GAME_TS_WARNING_INVENTORY_IS_FULL;
						}
					}

					if (itemInfo3.uiItemIdx != 0 && itemInfo3.uiItemIdx != INVALID_TBLIDX)
					{
						if (!g_pItemManager->CreateItem(pPlayer, itemInfo3.uiItemIdx, 1))
						{
							return GAME_TS_WARNING_INVENTORY_IS_FULL;
						}
					}
				}
				else
				{
					CItem* pItem = pPlayer->GetPlayerItemContainer()->GetItemByIdx(itemInfo.uiItemIdx);
					if (pItem)
					{
						if (pItem->GetPlace() == CONTAINER_TYPE_EQUIP)
						{
							if (!pPlayer->UnequipItem(pItem))
								return GAME_TS_WARNING_INVENTORY_IS_LOCK;
						}

						pItem->SetCount(0, false, true);
						pItem = NULL;
					}

					if (itemInfo2.uiItemIdx != 0 && itemInfo2.uiItemIdx != INVALID_TBLIDX)
					{
						pItem = pPlayer->GetPlayerItemContainer()->GetItemByIdx(itemInfo2.uiItemIdx);
						if (pItem)
						{
							if (pItem->GetPlace() == CONTAINER_TYPE_EQUIP)
							{
								if (!pPlayer->UnequipItem(pItem))
									return GAME_TS_WARNING_INVENTORY_IS_LOCK;
							}

							pItem->SetCount(0, false, true);
							pItem = NULL;
						}
					}

					if (itemInfo3.uiItemIdx != 0 && itemInfo3.uiItemIdx != INVALID_TBLIDX)
					{
						pItem = pPlayer->GetPlayerItemContainer()->GetItemByIdx(itemInfo3.uiItemIdx);
						if (pItem)
						{
							if (pItem->GetPlace() == CONTAINER_TYPE_EQUIP)
							{
								if (!pPlayer->UnequipItem(pItem))
									return GAME_TS_WARNING_INVENTORY_IS_LOCK;
							}

							pItem->SetCount(0, false, true);
						}
					}
				}
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_QITEM:
		{
			CDboTSActQItem* pQItem = (CDboTSActQItem*)pEntity;

			if (pPlayer == NULL || pPlayer->IsInitialized() == false)
				return RESULT_FAIL;

			CQuestProgress * pQuestProgress = pPlayer->GetQuests()->GetQuestProgress(m_executer.questID);

			if (pQuestProgress == NULL)
			{
				ERR_LOG(LOG_GENERAL, "Quest progress not found. TriggerOBJ script %u, questid = %u", m_executer.pTSTrigger->GetID(), m_executer.questID);
				return RESULT_FAIL;
			}

			switch (pQItem->GetQItemType())
			{
				case eQITEM_TYPE_CREATE:
				{
					for (int i = 0; i < pQItem->eMAX_TS_QITEM_COUNT; i++)
					{
						const stTS_QITEM_INFO& tsqitem = pQItem->GetQItemInfo(i);
						if (tsqitem.uiQItemIdx != INVALID_TBLIDX && tsqitem.uiQItemIdx != 0)
						{
							WORD wQItemRes = pQuestProgress->UpdateQuestObjectItem(tsqitem.uiQItemIdx, tsqitem.nQItemCnt);
							if (wQItemRes != RESULT_SUCCESS)
								return wQItemRes;
						}
					}
				}
				break;
				case eQITEM_TYPE_DELETE:
				{
					for (int i = 0; i < pQItem->eMAX_TS_QITEM_COUNT; i++)
					{
						const stTS_QITEM_INFO& tsqitem = pQItem->GetQItemInfo(i);
						if (tsqitem.uiQItemIdx != INVALID_TBLIDX && tsqitem.uiQItemIdx > 0 && tsqitem.nQItemCnt > 0)
						{
							pPlayer->GetQuests()->DeleteQuestItem(tsqitem.uiQItemIdx, tsqitem.nQItemCnt);
						}
					}
				}
				break;
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_TWAITTS:
		{
			CDboTSActTWaitTS* pActWaitTs = (CDboTSActTWaitTS*)pEntity;

			m_executer.dwWaitTick = (DWORD)pActWaitTs->GetWaitTime();
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_OBJSTATE:
		{
			CDboTSActObjState* pActObjState = (CDboTSActObjState*)pEntity;

			CTriggerObject* pObj = this;

			if (pActObjState->GetObjectIdx() != INVALID_TBLIDX && pObj->GetTblidx() != pActObjState->GetObjectIdx())
			{
				pObj = GetCurWorld()->FindStaticObjectByIdx(pActObjState->GetObjectIdx());

				if (pObj == NULL)
				{
					CGameServer* app = (CGameServer*)g_pApp;
					CWorld* pObjWorld = app->GetGameMain()->GetWorldManager()->FindWorld(pActObjState->GetWorldIdx());
					if(pObjWorld)
						pObj = pObjWorld->FindStaticObjectByIdx(pActObjState->GetObjectIdx());

					if (pObj == NULL)
					{
						ERR_LOG(LOG_GENERAL, "ERROR: Can not find trigger object. Idx %u, World %u, search in world %u", pActObjState->GetObjectIdx(), pActObjState->GetWorldIdx(), GetCurWorld()->GetIdx());
						return RESULT_SUCCESS;
					}
				}
			}

			BYTE byMainState = pObj->GetMainState();
			if (pActObjState->GetObjectMainState() != CDboTSActObjState::eMAIN_STATE_IGNORE)
			{
				byMainState = pActObjState->GetObjectMainState();
			}

			BYTE bySubState = pObj->GetSubState();

			if (pActObjState->GetObjectShowHide() != CDboTSActObjState::SUB_STATE_OBJECT_SH_IGNORE)
			{
				if (pActObjState->GetObjectShowHide() == CDboTSActObjState::SUB_STATE_OBJECT_SH_HIDE)
					BIT_FLAG_UNSET(bySubState, MAKE_BIT_FLAG(TOBJECT_SUBSTATE_SHOW)); //unset flag
				else
					BIT_FLAG_SET(bySubState, MAKE_BIT_FLAG(TOBJECT_SUBSTATE_SHOW)); //set flag
			}

			if (pActObjState->GetObjectCheckUncheck() != CDboTSActObjState::eSUB_STATE_OBJECT_UC_IGNORE)
			{
				if (pActObjState->GetObjectCheckUncheck() == CDboTSActObjState::eSUB_STATE_OBJECT_UC_CHECK)
					BIT_FLAG_UNSET(bySubState, MAKE_BIT_FLAG(TOBJECT_SUBSTATE_UNSELECT)); //unset flag
				else
					BIT_FLAG_SET(bySubState, MAKE_BIT_FLAG(TOBJECT_SUBSTATE_UNSELECT)); //set flag
			}

			pObj->UpdateToObjectState(byMainState, bySubState, GetTickCount());
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_OBJCONV:
		{
			CDboTSActObjConv* pConv = (CDboTSActObjConv*)pEntity;

			if (pPlayer && pPlayer->IsInitialized())
			{
				CNtlPacket packet(sizeof(sGU_TS_TRIGGER_OBJECT_DIALOG_NFY));
				sGU_TS_TRIGGER_OBJECT_DIALOG_NFY * res = (sGU_TS_TRIGGER_OBJECT_DIALOG_NFY *)packet.GetPacketData();
				res->wOpCode = GU_TS_TRIGGER_OBJECT_DIALOG_NFY;
				res->hTriggerObject = GetID();
				res->byDialogType = pConv->GetObjConvType();
				res->textTblidx = pConv->GetObjConv();
				packet.SetPacketLen(sizeof(sGU_TS_TRIGGER_OBJECT_DIALOG_NFY));
				Broadcast(&packet);
			}
			else return RESULT_FAIL;
		}
		break;

		case DBO_ACT_TYPE_ID_ACT_SEND_SVR_EVT: //SEND SERVER EVENT example to WPS
		{
			CDboTSActSendSvrEvt* pEvt = (CDboTSActSendSvrEvt*)pEntity;
		//	printf("OBJ: CDboTSActSendSvrEvt: GetActionId %u GetEvtSendType %u GetEvtSendType_Radius %f GetSvrEvtID %u GetSvrEvtTriggerType %u GetSvrEvtType %u GetTblIdx %u \n",
		//		pEvt->GetActionId(), pEvt->GetEvtSendType(), pEvt->GetEvtSendType_Radius(), pEvt->GetSvrEvtID(), pEvt->GetSvrEvtTriggerType(), pEvt->GetSvrEvtType(), pEvt->GetTblIdx());

			if (pEvt->GetSvrEvtTriggerType() == TS_TYPE_QUEST_CS) //most likely send to players
			{
				switch (pEvt->GetSvrEvtType())
				{
					case TRIGGER_SERVER_EVENT_TYPE_SPS:
					{
						CNpc* pNpc = pPlayer->GetCurWorld()->FindNpc(pEvt->GetTblIdx());
						if (pNpc && pNpc->IsInitialized())
						{
							CObjMsg_TriggerEvent pMsg;
							pMsg.teid = pEvt->GetSvrEvtID();
							pMsg.hSource = pPlayer->GetID();
							pMsg.byTriggerType = TS_TYPE_QUEST_CS;

							pNpc->OnObjectMsg(&pMsg);
						}
					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_TQS:
					{
						if (pPlayer && pPlayer->IsInitialized())
						{
							if (pPlayer->GetTMQ() && pPlayer->GetTMQ()->GetTQS())
							{
								CObjMsg_TqsEvent pMsg;
								pMsg.eventID = pEvt->GetSvrEvtID();

								pPlayer->GetTMQ()->GetTQS()->OnObjectMsg(&pMsg);
							}
							else return RESULT_FAIL;
						}
						else return RESULT_FAIL;
					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_TS:
					{
						if (pPlayer == NULL || pPlayer->IsInitialized() == false)
							return RESULT_FAIL;

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
								pPlayer->GetCurWorld()->AddEventIdToAllPlayers(pEvt->GetSvrEvtID());

								pPlayer->GetCurWorld()->Broadcast(&packet);
							}
							break;
							case TS_EVENT_SEND_TYPE_PARTY:
							{
								if (pPlayer->GetParty())
								{
									pPlayer->GetParty()->AddEventIdToAll(pEvt->GetSvrEvtID());

									pPlayer->GetParty()->SendMsgToParty(&packet);
								}
								else
								{
									pPlayer->GetQuests()->GetEventMap()->OnEventReceived(pEvt->GetSvrEvtID());
									pPlayer->SendPacket(&packet);
								}
							}
							break;
							case TS_EVENT_SEND_TYPE_PARTY_AREA:
							{
								if (pPlayer->GetParty())
								{
									pPlayer->GetParty()->AddEventIdToWithinRange(pEvt->GetSvrEvtID(), pPlayer, pEvt->GetEvtSendType_Radius());

									pPlayer->GetParty()->BroadcastWithinRange(&packet, pPlayer, pEvt->GetEvtSendType_Radius());
								}
								else
								{
									pPlayer->GetQuests()->GetEventMap()->OnEventReceived(pEvt->GetSvrEvtID());
									pPlayer->SendPacket(&packet);
								}
							}
							break;
							case TS_EVENT_SEND_TYPE_PC:
							{
								pPlayer->GetQuests()->GetEventMap()->OnEventReceived(pEvt->GetSvrEvtID());
								pPlayer->SendPacket(&packet);
							}
							break;
						}
					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_WPS:
					{
						if (pPlayer == NULL || pPlayer->IsInitialized() == false)
							return RESULT_FAIL;

						CScriptAlgoObject* pScript = pPlayer->GetScript(pEvt->GetTblIdx());
						if (pScript)
						{
							CObjMsg_SendTsEventToWps pMsg;
							pMsg.teId = pEvt->GetSvrEvtID();
							pMsg.byTriggerType = TS_TYPE_QUEST_CS;
							pScript->OnObjectMsg(&pMsg);
						}
					}
					break;

					default: ERR_LOG(LOG_USER, "case pEvt->GetSvrEvtType(%u) not set yet.", pEvt->GetSvrEvtType()); break;
				}
			}
			else if (pEvt->GetSvrEvtTriggerType() == TS_TYPE_INVALID)
			{
				switch (pEvt->GetSvrEvtType())
				{
					case TRIGGER_SERVER_EVENT_TYPE_SPS:
					{
						CNpc* pNpc = pPlayer->GetCurWorld()->FindNpc(pEvt->GetTblIdx());
						if (pNpc && pNpc->IsInitialized())
						{
							CObjMsg_TriggerEvent pMsg;
							pMsg.teid = pEvt->GetSvrEvtID();
							pMsg.hSource = pPlayer->GetID();
							pMsg.byTriggerType = TS_TYPE_QUEST_CS;

							pNpc->OnObjectMsg(&pMsg);
						}
					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_TQS: //send to time quest script
					{
						if (pPlayer->GetTMQ() && pPlayer->GetTMQ()->GetTQS())
						{
							CObjMsg_TqsEvent pMsg;
							pMsg.eventID = pEvt->GetSvrEvtID();
							pPlayer->GetTMQ()->GetTQS()->OnObjectMsg(&pMsg);
						}
					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_TMQ_START:
					{

					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_TMQ_ARRIVE:
					{

					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_TMQ_LEAVE:
					{

					}
					break;

					case TRIGGER_SERVER_EVENT_TYPE_WPS: //send to wps
					{
						if (pPlayer == NULL || pPlayer->IsInitialized() == false)
							return RESULT_FAIL;

						CScriptAlgoObject* pScript = pPlayer->GetCurWorld()->GetScript(pEvt->GetTblIdx());
						if (pScript)
						{
							CObjMsg_SendTsEventToWps pMsg;
							pMsg.teId = pEvt->GetSvrEvtID();
							pMsg.byTriggerType = TS_TYPE_OBJECT_TRIGGER_S;
							pScript->OnObjectMsg(&pMsg);
						}
					}
					break;

					default: ERR_LOG(LOG_USER, "case pEvt->GetSvrEvtType(%u) not set yet.", pEvt->GetSvrEvtType()); break;
				}
			}


		}
		break;
		case DBO_ACT_TYPE_ID_ACT_OPERATEOBJECT:
		{
			CDboTSActOPObject* pActOpObject = (CDboTSActOPObject*)pEntity;

			if (pPlayer == NULL || pPlayer->IsInitialized() == false)
				return RESULT_FAIL;

			if (pActOpObject->IsApply() == true)
			{
				m_executer.dwWaitTick = (DWORD)pActOpObject->GetOperateTime();

				pPlayer->SendCharStateOperating(pActOpObject->GetDirectionTableIndex(), (DWORD)pActOpObject->GetOperateTime(), GetID());
			}
			else
			{
				pPlayer->SendCharStateStanding();
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_DROP:
		{
			CDboTSActDrop* pAct = (CDboTSActDrop*)pEntity;

			sQUEST_PROBABILITY_TBLDAT* pProbabilityData = (sQUEST_PROBABILITY_TBLDAT*)g_pTableContainer->GetQuestProbabilityTable()->FindData(pAct->GetTableIndex());
			if (pProbabilityData)
			{
				if (pProbabilityData->eUseType != PROBABILITY_USE_TYPE_TS)
					return RESULT_FAIL;

				bool bRewardReceived = false;
				int nCount = 0;
				int nReward[NTL_QUEST_PROBABILITY_MAX_COUNT];
				memset(nReward, NULL, sizeof(nReward));

			LOOP_REPEAT:
				for (int c = 0; c < pProbabilityData->byCount; c++)
				{
					if (nCount >= NTL_QUEST_PROBABILITY_MAX_COUNT)
						nCount = NTL_QUEST_PROBABILITY_MAX_COUNT - 1;

					if (Dbo_CheckProbability(pProbabilityData->asProbabilityData[c].dwRate / 10000))
					{
						nReward[nCount++] = c;
						bRewardReceived = true;
						continue;
					}

					if (c == pProbabilityData->byCount - 1 && !bRewardReceived)
					{
						ERR_LOG(LOG_GENERAL, "Repeat loop.");
						goto LOOP_REPEAT;
					}
				}

				if (bRewardReceived == true)
				{
					int nRandomReward = RandomRange(0, nCount-1);

					if (nRandomReward >= NTL_QUEST_PROBABILITY_MAX_COUNT)
						nRandomReward = NTL_QUEST_PROBABILITY_MAX_COUNT - 1;

					sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(pProbabilityData->asProbabilityData[nReward[nRandomReward]].tblidx);
					if (pSkillTbldat == NULL)
					{
						ERR_LOG(LOG_GENERAL, "Skill tblidx %u not found. Obj script %u", pProbabilityData->asProbabilityData[nReward[nRandomReward]].tblidx, m_executer.pTSTrigger->GetID());
						return RESULT_FAIL;
					}

					sDBO_BUFF_PARAMETER aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
					eSYSTEM_EFFECT_CODE aeEffectCode[NTL_MAX_EFFECT_IN_SKILL];

					for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
					{
						aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;
						aBuffParameter[i].buffParameter.fParameter = (float)pSkillTbldat->aSkill_Effect_Value[i];
						aBuffParameter[i].buffParameter.dwRemainValue = (DWORD)pSkillTbldat->aSkill_Effect_Value[i];

						aeEffectCode[i] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pSkillTbldat->skill_Effect[i]);
					}

					DWORD dwKeepTime = pSkillTbldat->dwKeepTimeInMilliSecs;

					//GIVE BUFF
					if (pAct->GetPermissionType() == eQUEST_DROP_PERMISSION_TYPE_PERSONAL || pPlayer->GetPartyID() == INVALID_PARTYID) //if personal or dont have party
					{
						pPlayer->GetBuffManager()->RegisterBuff(dwKeepTime, aeEffectCode, aBuffParameter, INVALID_HOBJECT, BUFF_TYPE_BLESS, pSkillTbldat);
					}
					else
					{
						for (int i = 0; i < pPlayer->GetParty()->GetPartyMemberCount(); i++)
						{
							CPlayer* pMember = g_pObjectManager->GetPC(pPlayer->GetParty()->GetMemberInfo(i).hHandle);
							if (pMember && pMember->IsInitialized())
							{
								if (pPlayer->GetWorldID() == pMember->GetWorldID())
								{
									pMember->GetBuffManager()->RegisterBuff(dwKeepTime, aeEffectCode, aBuffParameter, INVALID_HOBJECT, BUFF_TYPE_BLESS, pSkillTbldat);
								}
							}
						}
					}
				}

			}
			else return RESULT_FAIL;
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_RMV_COUPON:
		{
			CDboTSActRmvCoupon* pAct = (CDboTSActRmvCoupon*)pEntity;

			if (pPlayer == NULL || pPlayer->IsInitialized() == false)
				return RESULT_FAIL;

			if (pPlayer->GetTMQ() == NULL)
				return RESULT_FAIL;

			pPlayer->GetTMQ()->DecreaseCoupon();

			pPlayer->GetTMQ()->UpdateCoupon();
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_SWPROBSF:
		{
			CDboTSActSWProbSF* pProb = (CDboTSActSWProbSF*)pEntity;

			if (Dbo_CheckProbabilityF(pProb->GetProbility() * 100.0f))
				m_executer.tcID = pProb->GetSuccessBID();
			else
				m_executer.tcID = pProb->GetFailBID();
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_PCCONV:
		{
			CDboTSActPCConv* pPcConv = (CDboTSActPCConv*)pEntity;

			if (pPlayer && pPlayer->IsInitialized())
			{
				pPlayer->GetClientSession()->SendPcTsDialogNfy(pPcConv->GetConvTblIdx());
			}
		}
		break;
		case DBO_ACT_TYPE_ID_ACT_CUSTOMEVT: // TODO
		{
			CDboTSActCustomEvt* pAct = (CDboTSActCustomEvt*)pEntity;

			if (pPlayer == NULL || pPlayer->IsInitialized() == false)
				return RESULT_FAIL;

			sPROGRESS_QUEST_INFO * pQuestInfo = pPlayer->GetQuests()->GetQuestProgressInfo(m_executer.questID);

			if (pQuestInfo == NULL)
			{
				ERR_LOG(LOG_GENERAL, "Quest progress not found. TriggerOBJ script %u, questid = %u", m_executer.pTSTrigger->GetID(), m_executer.questID);
				return RESULT_FAIL;
			}

			if (pPlayer == NULL || pPlayer->IsInitialized() == false)
				return RESULT_FAIL;


			pQuestInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[0] += 1;

			CNtlPacket packet(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
			sGU_QUEST_SVREVT_UPDATE_NFY * res = (sGU_QUEST_SVREVT_UPDATE_NFY *)packet.GetPacketData();
			res->wOpCode = GU_QUEST_SVREVT_UPDATE_NFY;
			res->taId = pQuestInfo->uData.sQInfoV0.sSToCEvtData.taId;
			res->tcId = pQuestInfo->uData.sQInfoV0.sSToCEvtData.tcId;
			res->tId = pQuestInfo->tId;
			res->bySlot = 0;
			res->bySvrEvtType = eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT;
			res->uEvtData.sCustomEvtCnt.nCurCnt = pQuestInfo->uData.sQInfoV0.sSToCEvtData.m_aUserData[0];
			packet.SetPacketLen(sizeof(sGU_QUEST_SVREVT_UPDATE_NFY));
			pPlayer->SendPacket(&packet);
		}
		break;

		case DBO_ACT_TYPE_ID_ACT_OBJ_WPS: //start wps script by object
		{
			CDboTSActObjWPS* pAct = (CDboTSActObjWPS*)pEntity;

			if (pPlayer == NULL || pPlayer->IsInitialized() == false)
				return RESULT_FAIL;
			
			if (pPlayer->GetScript(pAct->GetObjWPS()) == NULL)
			{
				if (pPlayer->GetCurWorld())
				{
					bool rbStart = false;
					CWpsAlgoObject* pObj = g_pScriptAlgoManager->CreateWpsObject(pAct->GetObjWPS(), rbStart);
					if (pObj)
					{
						pObj->SetWorld(pPlayer->GetCurWorld());

						if (pObj->GetExecuterType() == WORLDPLAY_EXECUTER_OBJECTTRIGGER)
						{
							pObj->GetWorld()->AddScript(pObj);
						}

						CQuestProgress * pQuestProgress = pPlayer->GetQuests()->GetQuestProgress(m_executer.questID);

						if (pQuestProgress == NULL && m_executer.questID != INVALID_QUESTID)
						{
							ERR_LOG(LOG_GENERAL, "Quest progress not found but quest id is not invalid. TriggerOBJ script %u, questid = %u", m_executer.pTSTrigger->GetID(), m_executer.questID);
							return RESULT_FAIL;
						}
	
						if (pQuestProgress)
						{
							pObj->AddPlayer(pPlayer->GetID());
							pPlayer->AddScript(pObj);

							pQuestProgress->GetProgressInfo()->uData.sQInfoV0.wQState = eTS_SVR_STATE_WPS_MODE;
							pQuestProgress->SetWpsScriptID(pAct->GetObjWPS());
						}

						if (rbStart)		//if its new created script then start it
							pObj->Start();
					}
					else return RESULT_FAIL;
				}
				else return RESULT_FAIL;
			}
			else return RESULT_FAIL;
		}
		break;


		default:
		{
			NTL_PRINT(PRINT_APP, "OBJTRIGGER-PROCESS: (DEFAULT) %s ", pEntity->GetClassNameA());
		}
	}

	return RESULT_SUCCESS;
}

void CTriggerObject::StopExecuting(bool bReset, bool bUpdate)
{
	m_bIsExecuting = false;
	m_executer.Init();

	if (bReset)
	{
		SetToObjTable(GetTbldat());

		if (bUpdate)
		{
			CNtlPacket packet(sizeof(sGU_TOBJECT_UPDATE_STATE));
			sGU_TOBJECT_UPDATE_STATE * res = (sGU_TOBJECT_UPDATE_STATE *)packet.GetPacketData();
			res->wOpCode = GU_TOBJECT_UPDATE_STATE;
			res->handle = GetID();
			res->tobjectBrief.objectID = GetTblidx();
			res->tobjectState.byState = GetTbldat()->byDefMainState;
			res->tobjectState.bySubStateFlag = GetTbldat()->byDefSubState;
			res->tobjectState.dwStateTime = 0;
			packet.SetPacketLen(sizeof(sGU_TOBJECT_UPDATE_STATE));
			Broadcast(&packet);
		}
	}
}

