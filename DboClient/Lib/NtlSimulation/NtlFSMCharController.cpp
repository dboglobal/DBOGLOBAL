#include "precomp_ntlsimulation.h"
#include "NtlFSMCharController.h"

//shared
#include "NtlMovement.h"
#include "ActionTable.h"
#include "NtlResultCode.h"

//core
#include "NtlDebug.h"
#include "NtlMath.h"

// DboTrigger
#include "DboTSCoreDefine.h"

// simulation
#include "NtlFSMDef.h"
#include "NtlFSMLayer.h"
#include "NtlFSMStateBase.h"
#include "NtlFSMUtil.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMCharActAgent.h"
#include "NtlFSMEventQueuingHelper.h"
#include "NtlPacketGenerator.h"
#include "NtlSLGlobal.h"
#include "NtlSLPacketGenerator.h"
#include "NtlBehaviorData.h"
#include "NtlSobActor.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLVisualLogic.h"
#include "NtlSLApi.h"
#include "NtlClock.h"

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlFSMPlayerController)

CNtlFSMPlayerController::CNtlFSMPlayerController()
{
	SetControllerId(SLCONTROLLER_PLYAER);

	m_pAutoCmdAI = NULL;
	m_pSyncMsgAttacKBegin = NTL_NEW CNtlFSMSyncMsgAttackBegin;
	m_pSyncMsgAttacKBegin->SetAvatarControlId(0);

	m_bAttackRangeCheck = FALSE;
	m_byKeyboardMoveFlags = NTL_MOVE_NONE;

	// event handler function link
	m_mapEventFunc[g_EventSobCreate.Get_pEventId()]						= &CNtlFSMPlayerController::SobCreateEventHandler;
	m_mapEventFunc[g_EventSobServerUpdateState.Get_pEventId()]			= &CNtlFSMPlayerController::SobServerUpdateStateEventHandler;

	m_mapEventFunc[g_EventActionMapHalt.Get_pEventId()]					= &CNtlFSMPlayerController::ActionMapHaltEventHandler;
	m_mapEventFunc[g_EventActionMapSitAndStand.Get_pEventId()]			= &CNtlFSMPlayerController::ActionMapSitAndStandEventHandler;
	m_mapEventFunc[g_EventActionMapJump.Get_pEventId()]					= &CNtlFSMPlayerController::ActionMapJumpEventHandler;
	m_mapEventFunc[g_EventActionMapAirJump.Get_pEventId()]				= &CNtlFSMPlayerController::ActionMapAirJumpEventHandler;
	m_mapEventFunc[g_EventActionMapSkillUse.Get_pEventId()]				= &CNtlFSMPlayerController::ActionMapSkillUseEventHandler;
	m_mapEventFunc[g_EventActionMapHTBUse.Get_pEventId()]				= &CNtlFSMPlayerController::ActionMapHTBUseEventHandler;
	m_mapEventFunc[g_EventActionMapItemUse.Get_pEventId()]				= &CNtlFSMPlayerController::ActionMapItemUseEventHandler;
	m_mapEventFunc[g_EventActionMapSocialUse.Get_pEventId()]			= &CNtlFSMPlayerController::ActionMapSocialUseEventHandler;
	m_mapEventFunc[g_EventActionMapPrivateShopOpen.Get_pEventId()]		= &CNtlFSMPlayerController::ActionMapPrivateShopOpenEventHandler;
	m_mapEventFunc[g_EventActionMapCharging.Get_pEventId()]				= &CNtlFSMPlayerController::ActionMapChargingEventHandler;
	m_mapEventFunc[g_EventActionMapAutoFollow.Get_pEventId()]			= &CNtlFSMPlayerController::ActionMapAutoFollowEventHandler;

	m_mapEventFunc[g_EventTerrainClick.Get_pEventId()]					= &CNtlFSMPlayerController::TerrainClickEventHandler;
	m_mapEventFunc[g_EventKeyboardMove.Get_pEventId()]					= &CNtlFSMPlayerController::KeyboardMoveEventHandler;
	m_mapEventFunc[g_EventKeyboardMoveStop.Get_pEventId()]				= &CNtlFSMPlayerController::KeyboardMoveStopEventHandler;
	m_mapEventFunc[g_EventMouseDashMove.Get_pEventId()]					= &CNtlFSMPlayerController::MouseDashMoveEventHandler;
	m_mapEventFunc[g_EventKeyboardDashMove.Get_pEventId()]				= &CNtlFSMPlayerController::KeyboardDashMoveEventHandler;
	m_mapEventFunc[g_EventAirDashMove.Get_pEventId()] = &CNtlFSMPlayerController::AirDashMoveEventHandler;
	m_mapEventFunc[g_EventAirAccelMove.Get_pEventId()] = &CNtlFSMPlayerController::AirAccelMoveEventHandler;
	m_mapEventFunc[g_EventInputChangeHeading.Get_pEventId()]			= &CNtlFSMPlayerController::InputChangeHeadingEventHandler;
	m_mapEventFunc[g_EventSobTargetSelect.Get_pEventId()]				= &CNtlFSMPlayerController::SobTargetSelectEventHandler;
	m_mapEventFunc[g_EventSobAttackSelect.Get_pEventId()]				= &CNtlFSMPlayerController::SobAttackSelectEventHandler;
    m_mapEventFunc[g_EventSobSendAttackEnd.Get_pEventId()]              = &CNtlFSMPlayerController::SobSendAttackEndHandler;

	m_mapEventFunc[g_EventSobFainting.Get_pEventId()]					= &CNtlFSMPlayerController::SobFaintingEventHandler;

	// notify event handler function link
	m_mapNotifyEventFunc[g_EventActionMapJump.Get_pEventId()]			= &CNtlFSMPlayerController::NotifyActionMapJumpEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapAirJump.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyActionMapAirJumpEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapCharging.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyActionMapChargingEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapBlocking.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyActionMapBlockingEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapSitAndStand.Get_pEventId()]	= &CNtlFSMPlayerController::NotifyActionMapSitAndDownEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapSkillUse.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyActionMapSkillUseEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapSocialUse.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyActionMapSocialUseEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapPrivateShopOpen.Get_pEventId()]= &CNtlFSMPlayerController::NotifyActionMapPrivateShopOpenEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapHTBUse.Get_pEventId()]			= &CNtlFSMPlayerController::NotifyActionMapHTBUseEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapItemUse.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyActionMapItemUseEventHandler;
	m_mapNotifyEventFunc[g_EventActionMapAutoFollow.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyActionMapAutoFollowEventHandler;    

	m_mapNotifyEventFunc[g_EventInputChangeHeading.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyInputChangeHeadingEventHandler;
	m_mapNotifyEventFunc[g_EventSobSkillCancel.Get_pEventId()]			= &CNtlFSMPlayerController::NotifySobSkillCancelEventHandler;
	m_mapNotifyEventFunc[g_EventTerrainClick.Get_pEventId()]			= &CNtlFSMPlayerController::NotifyTerrainClickEventHandler;
	m_mapNotifyEventFunc[g_EventKeyboardMove.Get_pEventId()]			= &CNtlFSMPlayerController::NotifyKeyboardMoveEventHandler;
	m_mapNotifyEventFunc[g_EventKeyboardMoveStop.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyKeyboardMoveStopEventHandler;
	m_mapNotifyEventFunc[g_EventMouseDashMove.Get_pEventId()]			= &CNtlFSMPlayerController::NotifyMouseDashMoveEventHandler;
	m_mapNotifyEventFunc[g_EventKeyboardDashMove.Get_pEventId()]		= &CNtlFSMPlayerController::NotifyKeyboardDashMoveEventHandler;
	m_mapNotifyEventFunc[g_EventAirDashMove.Get_pEventId()] = &CNtlFSMPlayerController::NotifyAirDashMoveEventHandler;
	m_mapNotifyEventFunc[g_EventAirAccelMove.Get_pEventId()] = &CNtlFSMPlayerController::NotifyAirAccelMoveEventHandler;
	m_mapNotifyEventFunc[g_EventSobTargetSelect.Get_pEventId()]			= &CNtlFSMPlayerController::NotifySobTargetSelectEventHandler;
	m_mapNotifyEventFunc[g_EventSobAttackSelect.Get_pEventId()]			= &CNtlFSMPlayerController::NotifySobAttackSelectEventHandler;
	m_mapNotifyEventFunc[g_EventSobFollowMove.Get_pEventId()]			= &CNtlFSMPlayerController::NotifySobFollowMoveEventHandler;
	m_mapNotifyEventFunc[g_EventSobHit.Get_pEventId()]					= &CNtlFSMPlayerController::NotifySobHitEventHandler;    
	m_mapNotifyEventFunc[g_EventSobSpecialAttack.Get_pEventId()] = &CNtlFSMPlayerController::NotifySobSpecialAttackEventHandler;
	m_mapNotifyEventFunc[g_EventSobAnimPlay.Get_pEventId()]				= &CNtlFSMPlayerController::NotifySobAnimPlayEventHandler;   
}


CNtlFSMPlayerController::~CNtlFSMPlayerController()
{
	if(m_pSyncMsgAttacKBegin)
	{
		NTL_DELETE(m_pSyncMsgAttacKBegin);
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : auto command AI 객체 생성.
//  ------------------------------------------------------------------------
//  paramater : 
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::CreateAutoCmdAI(void)
{
	if(m_pAutoCmdAI)
	{
		ReleaseAutoCmdAI();
	}

	m_pAutoCmdAI = NTL_NEW CNtlFSMAutoFollowCmdAI(m_pActor);
}

////////////////////////////////////////////////////////////////////////////
//  des : auto command AI 객체 삭제.
//  ------------------------------------------------------------------------
//  paramater : 
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::ReleaseAutoCmdAI(void)
{
	if(m_pAutoCmdAI)
	{
		NTL_DELETE(m_pAutoCmdAI);
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : auto command AI update.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : tick elapsed time.
////////////////////////////////////////////////////////////////////////////


void CNtlFSMPlayerController::UpdateAutoCmdAI(RwReal fElapsed)
{
	if(m_pAutoCmdAI)
	{
		m_pAutoCmdAI->Update(fElapsed);
		if(m_pAutoCmdAI->IsFinish())
		{
			NTL_DELETE(m_pAutoCmdAI);
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : follow check function.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : tick elapsed time.
////////////////////////////////////////////////////////////////////////////


void CNtlFSMPlayerController::UpdateFollowCheck(RwReal fElapsed)
{
	if(!m_pSyncMsgAttacKBegin->GetAttackBeginFlag() && m_bAttackRangeCheck)
	{
		m_fRangeCheckTime += fElapsed;
		if(m_fRangeCheckTime >= 0.1f)
		{
            m_fRangeCheckTime = 0.0f;

            CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
            if(!pCharActAgent->IsFightingMode())
            {
                return;
            }

			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
			CNtlSob *pTarSob = GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId);
			if(pTarSob == NULL)
				return;

			pCtrlStuff->fFollowRange = Logic_GetAttackRange(m_pActor, pCtrlStuff->hTargetSerialId);

			RwV3d vPos = m_pActor->GetPosition();
			RwV3d vTarPos = pTarSob->GetPosition();

			RwV3d vSub;
			RwV3dSubMacro(&vSub, &vTarPos, &vPos);
			RwReal fLen = RwV3dLength(&vSub);
			if(fabs(pCtrlStuff->fFollowRange - fLen) < 0.1f)
				return;

			CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_FIGHTING_FOLLOW);

			// follow packet을 보낸다.			
			if(!pCharActAgent->IsFightingMode())
			{
				pCharActAgent->SetFightingMode(TRUE);
				API_GetSLPacketGenerator()->SendCharToggleFighting(TRUE, m_pActor->GetServerSyncAvatarType());
			}

			API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_AUTO_ATTACK, m_pActor->GetServerSyncAvatarType());
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : server state update function to analyze.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : update data structer.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ServerStateUpdateAnalysis(SNtlEventSobServerUpdateState *pServerState)
{
	
	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	RwUInt8 byServerOldState = pCharActAgent->GetServerState();
	RwUInt8 byServerState = pServerState->pState->sCharStateBase.byStateID;

	if(m_pActor->GetClassID() == SLCLASS_AVATAR)
	{
		// time clock setting
		if(byServerState == CHARSTATE_SPAWNING || byServerState == CHARSTATE_TELEPORTING)
			API_GetGameClock()->SetPause_AvatarTime(true, GetTickCount());
		else
		{
			if(API_GetGameClock()->IsPause_AvatarTime())
				API_GetGameClock()->SetPause_AvatarTime(false, GetTickCount());
		}
	}

	switch(byServerState)
	{
		case CHARSTATE_SPAWNING:
		{
			if (m_pActor->IsAirMode())
			{
				m_pActor->SetAngleX(0); // reset angle
			}

			sCHARSTATE *pCharState = pServerState->pState;
			sCHARSTATE_SPAWNING *pSpawn = &pCharState->sCharStateDetail.sCharStateSpawning;
			if(pSpawn->byTeleportType == TELEPORT_TYPE_GAME_IN)
				return NTL_FSM_EVENTRES_BLOCK;
			else
				return NTL_FSM_EVENTRES_PASS;
		}
		break;
		case CHARSTATE_STANDING:
		{
			if(byServerOldState == CHARSTATE_SITTING)
			{
				CNtlSLEventGenerator::SobSit(m_pActor->GetSerialID(), FALSE);
			}
			else if(byServerOldState == CHARSTATE_KNOCKDOWN || byServerOldState == CHARSTATE_STUNNED ||
				    byServerOldState == CHARSTATE_SLEEPING || byServerOldState == CHARSTATE_SLIDING ||
					byServerOldState == CHARSTATE_CHARGING || byServerOldState == CHARSTATE_GUARD ||
					byServerOldState == CHARSTATE_SANDBAG || byServerOldState == CHARSTATE_PRIVATESHOP ||
					byServerOldState == CHARSTATE_OPERATING || byServerOldState == CHARSTATE_KEEPING_EFFECT ||
                    byServerOldState == CHARSTATE_PARALYZED)
			{
				RwV3d vPos = m_pActor->GetPosition();
				RwV3d vDir = m_pActor->GetDirection();

				CNtlSLEventGenerator::SobStanding(m_pActor->GetSerialID(), vPos, vDir);
			}
			else if(byServerOldState == CHARSTATE_TELEPORTING)
			{
				CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_CREATE);
			}
			else if(byServerOldState == CHARSTATE_DIRECT_PLAY || byServerOldState == CHARSTATE_RIDEON)
			{
				return NTL_FSM_EVENTRES_PASS;
			}
			else if (byServerOldState == CHARSTATE_FALLING) // 
			{
				if (m_pActor->IsAirMode())
				{
					CNtlFSMCharActAgent* pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
					pActAgent->SetAirMode(AIR_STATE_OFF);

					m_pActor->SetAngleX(0); // reset angle

					CNtlSobCharProxy * pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
					pSobProxy->SetBaseAnimation(Logic_GetIdleAnimationID(m_pActor));
				}
			}
		}
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_CHARGING:
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_GUARD:
		CNtlSLEventGenerator::BlockingNfy(TRUE, 0);
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_SITTING:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_MOVING:
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_DESTMOVE:
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_DASH_PASSIVE:
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_FOLLOWING:
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_FALLING:
		//return NTL_FSM_EVENTRES_BLOCK;
		return NTL_FSM_EVENTRES_PASS; // pass, so it will apply on ourself too 
		break;
	case CHARSTATE_TELEPORTING:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_SLIDING:
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_KNOCKDOWN:
		return NTL_FSM_EVENTRES_PASS; // pass, so it will apply on ourself too
		break;
	case CHARSTATE_STUNNED:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_SLEEPING:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_FOCUSING:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_CASTING:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_HTB:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_SANDBAG:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_CASTING_ITEM:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_FAINTING:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_PRIVATESHOP:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_DIRECT_PLAY:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_OPERATING:
		{
			sCHARSTATE_OPERATING *pOperation = &pServerState->pState->sCharStateDetail.sCharStateOperating;
			CNtlSLEventGenerator::OperateObjectTimeBar(true, pOperation->dwOperateTime);
		}
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_DESPAWNING:
		return NTL_FSM_EVENTRES_PASS;
		break;
	case CHARSTATE_CAMPING:
		return NTL_FSM_EVENTRES_BLOCK;
		break;
	case CHARSTATE_LEAVING:
		return NTL_FSM_EVENTRES_PASS;
		break;
    case CHARSTATE_RIDEON:
        return NTL_FSM_EVENTRES_PASS;
        break;
    case CHARSTATE_KEEPING_EFFECT:
        return NTL_FSM_EVENTRES_PASS;
        break;
    case CHARSTATE_PARALYZED:
        return NTL_FSM_EVENTRES_PASS;
        break;

	case CHARSTATE_AIR_JUMP:
		return NTL_FSM_EVENTRES_BLOCK; // block, so we dont apply it on avatar
		break;
		case CHARSTATE_AIR_DASH_ACCEL:
		{
			// check if we switch from dash to accel
			if (byServerOldState == CHARSTATE_AIR_DASH_ACCEL)
			{
				sCHARSTATE_AIRACCEL* pAirAccel = &pServerState->pState->sCharStateDetail.sCharStateAirAccel;

				CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
				SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

				if (pMoveStuff->byFormFlag == NTL_MOVE_FLAG_FLY_DASH && pAirAccel->byMoveFlag == NTL_MOVE_FLAG_FLY_ACCEL)
				{
					return NTL_FSM_EVENTRES_PASS;
				}
			}

			return NTL_FSM_EVENTRES_BLOCK;
		}
		break;
	}

	return NTL_FSM_EVENTRES_BLOCK;
}


////////////////////////////////////////////////////////////////////////////
//  des : player controller를 update 한다.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : delta time.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::Update(RwReal fElapsed)
{
	UpdateAutoCmdAI(fElapsed);
	m_pSyncMsgAttacKBegin->Update(GetActor(), fElapsed);
	UpdateFollowCheck(fElapsed);
}

////////////////////////////////////////////////////////////////////////////
//  des : event 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::HandleEvents");
		
	MapFuncEventHandler::iterator it = m_mapEventFunc.find(pMsg.Id);
	if(it != m_mapEventFunc.end())
	{
		RwUInt32 uiRes = (this->*(*it).second)(pMsg);
		NTL_RETURN(uiRes);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}


////////////////////////////////////////////////////////////////////////////
//  des : sob create event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::SobCreateEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::SobCreateEventHandler");
	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : server update state event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::SobServerUpdateStateEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::SobServerUpdateStateEventHandler");

	SNtlEventSobServerUpdateState *pUpdateState = reinterpret_cast<SNtlEventSobServerUpdateState*>(pMsg.pData);

	NTL_RETURN(ServerStateUpdateAnalysis(pUpdateState));
}

////////////////////////////////////////////////////////////////////////////
//  des : action map halt event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapHaltEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapHaltEventHandler");

	ReleaseAutoCmdAI();
	
	NTL_RETURN(NTL_FSM_EVENTRES_QUEUING_REFRESH);
}


////////////////////////////////////////////////////////////////////////////
//  des : action map sit and stand event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapSitAndStandEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapSitAndStandEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(pCharActAgent->IsFightingMode())
	{
		pCharActAgent->SetFightingMode(FALSE);
		API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : action map jump event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapJumpEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapJumpEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : action map air jump event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapAirJumpEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapAirJumpEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if (pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : action map skill use event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapSkillUseEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapSkillUseEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : action map HTB use event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapHTBUseEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapHTBUseEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : action map item use event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapItemUseEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapItemUseEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}


////////////////////////////////////////////////////////////////////////////
//  des : action social use event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapSocialUseEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapSocialUseEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : action normal use event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapPrivateShopOpenEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapPrivateShopOpenEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}


////////////////////////////////////////////////////////////////////////////
//  des : action charging event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapChargingEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapChargingEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}


////////////////////////////////////////////////////////////////////////////
//  des : auto follow event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::ActionMapAutoFollowEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::ActionMapAutoFollowEventHandler");

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	SNtlEventActionMapAutoFollow *pAutoFollow = reinterpret_cast<SNtlEventActionMapAutoFollow*>( pMsg.pData );
	CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(pAutoFollow->hTargetSerialId);
	if(pTarSobObj == NULL)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(m_pActor->GetSerialID() == pTarSobObj->GetSerialID())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if( !(pTarSobObj->GetFlags() & SLFLAG_CAN_AUTO_FOLLOW_TARGET) )
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}


////////////////////////////////////////////////////////////////////////////
//  des : action mouse dash move event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::MouseDashMoveEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::MouseDashMoveEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

    if(pCharActAgent->IsFightingMode())
    {
        pCharActAgent->SetFightingMode(FALSE);
        API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
    }

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : action keyboard dash move event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::KeyboardDashMoveEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::KeyboardDashMoveEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

    if(pCharActAgent->IsFightingMode())
    {
        pCharActAgent->SetFightingMode(FALSE);
        API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
    }

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMPlayerController::AirDashMoveEventHandler(RWS::CMsg & pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::AirDashMoveEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if (pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMPlayerController::AirAccelMoveEventHandler(RWS::CMsg& pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::AirAccelMoveEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent* pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if (pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : action input change heading event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::InputChangeHeadingEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::InputChangeHeadingEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : terrain click event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::TerrainClickEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::TerrainClickEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	
	if(pCharActAgent->IsFightingMode())
	{
		pCharActAgent->SetFightingMode(FALSE);
		API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : keyboard move event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////


RwUInt32 CNtlFSMPlayerController::KeyboardMoveEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::KeyboardMoveEventHandler");

	ReleaseAutoCmdAI();

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(pCharActAgent->IsFightingMode())
	{
		pCharActAgent->SetFightingMode(FALSE);
		API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
	}

	SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>( pMsg.pData );
	m_byKeyboardMoveFlags = pKeyboardMove->byMoveFlags;

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : keyboard move stop event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////


RwUInt32 CNtlFSMPlayerController::KeyboardMoveStopEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::KeyboardMoveStopEventHandler");

	ReleaseAutoCmdAI();

	m_byKeyboardMoveFlags = NTL_MOVE_NONE;

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	if(pCharActAgent->IsDirectMode())
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : sob click event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::SobTargetSelectEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::SobTargetSelectEventHandler");

	if(m_pActor->GetClassID() != SLCLASS_AVATAR)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_PASS);
	}

    CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	// by daneos: disabled, so when we press ESC during confusion, the auto attack and movement stops.
    /*if(pCharActAgent->IsDirectMode())
    {
        NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
    }*/

	ReleaseAutoCmdAI();

	//SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>(pMsg.pData);

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	if(pCharActAgent->IsFightingMode())
	{
		pCharActAgent->SetFightingMode(FALSE);
		API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
	}

	RwUInt32 uiCurrStateId = m_pAgent->GetCurrStateId();
	if( uiCurrStateId == NTL_FSMSID_FOLLOW || uiCurrStateId == NTL_FSMSID_FIGHTING_FOLLOW )
	{
		CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_IDLE);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : sob double click event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::SobAttackSelectEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::SobAttackSelectEventHandler");

    CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);    

	ReleaseAutoCmdAI();

	SNtlEventSobAttackSelect *pSobAttackSelect = reinterpret_cast<SNtlEventSobAttackSelect*>(pMsg.pData);
	if(m_pActor->GetSerialID() == pSobAttackSelect->hSerialId)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
		
		if(pCharActAgent->IsFightingMode())
		{
			pCharActAgent->SetFightingMode(FALSE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
		}

		RwUInt32 uiCurrStateId = m_pAgent->GetCurrStateId();
		if( uiCurrStateId == NTL_FSMSID_FOLLOW || uiCurrStateId == NTL_FSMSID_FIGHTING_FOLLOW )
		{
			CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_IDLE);
		}

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

////////////////////////////////////////////////////////////////////////////
//  des : sob faintint event handler
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : CMsg.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMPlayerController::SobFaintingEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMPlayerController::SobFaintingEventHandler");

	ReleaseAutoCmdAI();

	m_pSyncMsgAttacKBegin->SetAttackBeginFlag(FALSE);

	if(m_pActor->GetClassID() == SLCLASS_AVATAR) 
		CNtlSLEventGenerator::SobUpdate(m_pActor->GetSerialID(), EVENT_AIUT_DIE, 0);

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}


////////////////////////////////////////////////////////////////////////////
//  des : update에 의해서 상태가 바뀌었을 경우 notify 해 주는 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state.
//  \param2 : next state.
////////////////////////////////////////////////////////////////////////////

// 상태 update에 의해서 상태가 바뀌는 경우.
void CNtlFSMPlayerController::NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState)
{
	///DBO_WARNING_MESSAGE("CNtlFSMPlayerController::NotifyUpdateChangeState");
	// old state check.
	NotifyOldUpdateStateProc(uiOldState, uiNextState);

	if(uiNextState == NTL_FSMSID_SPAWN)
	{
		NotifyNextUpdateStateSpawnProc(uiOldState);
	}
	else if(uiNextState == NTL_FSMSID_IDLE)
	{
		NotifyNextUpdateStateIdleProc(uiOldState);
	}
	else if(uiNextState == NTL_FSMSID_HURT)
	{
		NotifyNextUpdateStateHurtProc(uiOldState);
	}
	else if(uiNextState == NTL_FSMSID_SKILL_READY)
	{
		NotifyNextUpdateStateSkillReadyProc(uiOldState);
	}
	else if(uiNextState == NTL_FSMSID_HTB_READY)
	{
		NotifyNextUpdateStateHTBReadyProc(uiOldState);
	}
	else if(uiNextState == NTL_FSMSID_FOLLOW)
	{
		NotifyNextUpdateStateChaseFollowProc(uiOldState);
	}
	else if(uiNextState == NTL_FSMSID_FIGHTING_FOLLOW)
	{
		NotifyNextUpdateStateFightingFollowProc(uiOldState);
	}
	else if(uiNextState == NTL_FSMSID_SKILL_FOLLOW)
	{
		NotifyNextUpdateStateSkillFollowProc(uiOldState);
	}
	else if(uiNextState == NTL_FSMSID_FALLING)
	{
		NotifyNextUpdateStateFallingProc(uiOldState);
	}
	
	// attack range check
	NotifyUpdateStateAttackRangeCheck(uiOldState, uiNextState);
}

////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 idle state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyOldUpdateStateProc(RwUInt32 uiOldState, RwUInt32 uiNextState)
{
	//DBO_WARNING_MESSAGE("CNtlFSMPlayerController::NotifyOldUpdateStateProc: uiOldState: " << uiOldState << " uiNextState: " << uiNextState);

	if(uiOldState == NTL_FSMSID_FOLLOW || uiOldState == NTL_FSMSID_FIGHTING_FOLLOW || 
		uiOldState == NTL_FSMSID_SKILL_FOLLOW || uiOldState == NTL_FSMSID_MOVE || uiOldState == NTL_FSMSID_DASH)
	{
		if(uiNextState != NTL_FSMSID_SWIMMING && uiNextState != NTL_FSMSID_FALLING)
		{
			RwV3d vPos = m_pActor->GetPosition();
			RwV3d vDir = m_pActor->GetDirection();

			API_GetSLPacketGenerator()->SendKeyboardMove(vPos, vDir, NTL_MOVE_NONE, m_pActor->GetServerSyncAvatarType());
		}
	}

	if(uiOldState == NTL_FSMSID_FOLLOW)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 
		CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId) );
		if(pTarActor)
		{

			if(Logic_IsCommunityActor(pTarActor) && Logic_CanActorCommunity(m_pActor, pTarActor, pCtrlStuff->fFollowRange))
			{
				Logic_BeginActorCommunity(m_pActor, pTarActor);
			}
		}
	}
	else if(uiOldState == NTL_FSMSID_JUMP)
	{
		CNtlBeCharData *pBeData= reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
		pMoveStuff->Reset();

		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vDir = m_pActor->GetDirection();

		API_GetSLPacketGenerator()->SendJumpEnd();

        CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
        if(pCharActAgent->IsAutoRun()) // When jumping during automatic running
        {
            pMoveStuff->byType = NTL_MOVETARGET_DIR;
            pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
            pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
            pMoveStuff->byMoveFlags = NTL_MOVE_F;

		    API_GetSLPacketGenerator()->SendKeyboardMove(vPos, vDir, NTL_MOVE_F, m_pActor->GetServerSyncAvatarType());
        }
	}
	else if(uiOldState == NTL_FSMSID_CHARGING)
	{
		CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
		if(pCharActAgent->IsCharging())
		{
			pCharActAgent->SetCharging(FALSE);
			API_GetSLPacketGenerator()->SendChargingEnd();
		}
	}
	else if(uiOldState == NTL_FSMSID_FIGHTING_POSE)
	{
		CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
		if(pCharActAgent->IsFightingMode())
		{
			pCharActAgent->SetFightingMode(FALSE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
		}
	}
	else if(uiOldState == NTL_FSMSID_SKILL_ACTION)
	{
		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vDir = m_pActor->GetDirection();
		API_GetSLPacketGenerator()->SendChangePosition(vPos, vDir);
	}

	else if(uiOldState == NTL_FSMSID_TRIGGER_OPERATE)
	{
		CNtlSLEventGenerator::OperateObjectTimeBar(false, 0);
	}

	else if(uiOldState == NTL_FSMSID_FALLING)
	{
		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vDir = m_pActor->GetDirection();

		if (m_pActor->IsAirMode())
		{
			API_GetSLPacketGenerator()->SendAirEnd(vPos, vDir);
		}
		else
		{
			API_GetSLPacketGenerator()->SendFalling(FALSE, vPos, vDir, NTL_MOVE_F);
		}
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 spawn state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyNextUpdateStateSpawnProc(RwUInt32 uiOldState)
{
	m_pSyncMsgAttacKBegin->SetAttackBeginFlag(FALSE);
}

////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 idle state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyNextUpdateStateIdleProc(RwUInt32 uiOldState)
{
    CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	if(uiOldState == NTL_FSMSID_FIGHTING_POSE || uiOldState == NTL_FSMSID_FIGHTING_SHRINK)
	{
		if(pCharActAgent->IsFightingMode())
		{
			pCharActAgent->SetFightingMode(FALSE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
		}
	}
	else if(uiOldState == NTL_FSMSID_SITDOWN || uiOldState == NTL_FSMSID_SIT)
	{
		if(pCharActAgent->IsSitDown())
		{
			pCharActAgent->SetSitDown(FALSE);
			API_GetSLPacketGenerator()->SendSitAndStand(FALSE);  
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 hurt state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////


void CNtlFSMPlayerController::NotifyNextUpdateStateHurtProc(RwUInt32 uiOldState)
{
	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	if(pCharActAgent->IsSitDown())
	{
		pCharActAgent->SetSitDown(FALSE);
		API_GetSLPacketGenerator()->SendSitAndStand(FALSE);  
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 skill ready state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////


void CNtlFSMPlayerController::NotifyNextUpdateStateSkillReadyProc(RwUInt32 uiOldState)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

	API_GetSLPacketGenerator()->SendCharSkillReq(m_pActor, pSkillStuff->hTargetSerialId, pSkillStuff->bySlotIdx, pSkillStuff->byRpBonusType, m_pActor->GetServerSyncAvatarType());
}


////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 HTB ready state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////


void CNtlFSMPlayerController::NotifyNextUpdateStateHTBReadyProc(RwUInt32 uiOldState)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();

	API_GetSLPacketGenerator()->SendCharHTBReq(pHTBStuff->hTargetSerialId, pHTBStuff->bySlotIdx);
}

////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 fighting follow state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////


void CNtlFSMPlayerController::NotifyNextUpdateStateChaseFollowProc(RwUInt32 uiOldState)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 

	API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_FRIENDLY, m_pActor->GetServerSyncAvatarType());
}


////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 fighting follow state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////


void CNtlFSMPlayerController::NotifyNextUpdateStateFightingFollowProc(RwUInt32 uiOldState)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	if(!pCharActAgent->IsFightingMode())
	{
		pCharActAgent->SetFightingMode(TRUE);
		API_GetSLPacketGenerator()->SendCharToggleFighting(TRUE, m_pActor->GetServerSyncAvatarType());
	}

	API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_AUTO_ATTACK, m_pActor->GetServerSyncAvatarType());
}

////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 skill follow state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyNextUpdateStateSkillFollowProc(RwUInt32 uiOldState)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_SKILL_ATTACK, m_pActor->GetServerSyncAvatarType());
}

////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 falling state change 될 경우 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyNextUpdateStateFallingProc(RwUInt32 uiOldState)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwV3d vDir = m_pActor->GetDirection();

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	RwUInt8 byMoveDirection = pCtrlStuff->uExtra.sFalling.byMoveDirection;

	if (m_pActor->IsAirMode())
	{
		API_GetSLPacketGenerator()->SendAirFalling(vPos, vDir, NTL_MOVE_NONE);
	}
	else
	{
		if (byMoveDirection == NTL_MOVE_NONE || byMoveDirection >= NTL_MOVE_MOUSE_MOVEMENT)
			byMoveDirection = NTL_MOVE_F;

		API_GetSLPacketGenerator()->SendFalling(TRUE, vPos, vDir, byMoveDirection);
	}
}



////////////////////////////////////////////////////////////////////////////
//  des : update에 의한 state change시 attack range check.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : previous state
//  \param2 : next state
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyUpdateStateAttackRangeCheck(RwUInt32 uiOldState, RwUInt32 uiNextState)
{
	if(uiNextState == NTL_FSMSID_FIGHTING_POSE || uiNextState == NTL_FSMSID_FIGHTING_SHRINK ||
		uiNextState == NTL_FSMSID_HURT) 
	{
		m_bAttackRangeCheck = TRUE;
		m_fRangeCheckTime = 0.0f;
	}
	else
	{
		m_bAttackRangeCheck = FALSE;
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

// event에 의해서 상태가 바뀌는 경우.
void CNtlFSMPlayerController::NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	RwUInt32 uiCurrState = uiParam1; 
	RwUInt32 uiNextState = uiParam2; 

	// attack range check

	if(uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE)
	{
		// disabled by daneos. Maybe need enable? This is used for air-jump, so we send jump-end packet when we go into air-jump state
		/*if (uiCurrState == NTL_FSMSID_JUMP)
		{
			CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
			SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
			pMoveStuff->Reset();
			DBO_WARNING_MESSAGE("SEND JUMP END. SHOULD WE DO THIS?");
			API_GetSLPacketGenerator()->SendJumpEnd();
		}*/

		if(uiNextState == NTL_FSMSID_FIGHTING_POSE || uiNextState == NTL_FSMSID_FIGHTING_SHRINK || 
			uiNextState == NTL_FSMSID_HURT)
		{
			m_bAttackRangeCheck = TRUE;
			m_fRangeCheckTime = 0.0f;
		}
		else
		{
			m_bAttackRangeCheck = FALSE;
		}

		if(uiCurrState == NTL_FSMSID_TRIGGER_OPERATE)
		{
			CNtlSLEventGenerator::OperateObjectTimeBar(false, 0);
		}
	}

	MapFuncNotifyEventHandler::iterator it = m_mapNotifyEventFunc.find(pMsg.Id);
	if(it != m_mapNotifyEventFunc.end())
	{
		(this->*(*it).second)(pMsg, uiEventResult, uiParam1, uiParam2);
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : ActionMapJump event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapJumpEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;
	
	SNtlEventActionMapJump *pActionMapJump = reinterpret_cast<SNtlEventActionMapJump*>(pMsg.pData);

	RwV3d vDir = m_pActor->GetDirection();
	RwV3d vLoc = m_pActor->GetPosition();

	// by daneos: so we send correct move flag to server
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

	API_GetSLPacketGenerator()->SendJump(pMoveStuff->byMoveFlags, vLoc, vDir);

	// event queueing.
	if(pActionMapJump->uiMoveFlags != NTL_MOVE_NONE)
	{
		AgentQueuingKeyboardMove(m_pAgent, m_byKeyboardMoveFlags);
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : ActionMapAirJump event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapAirJumpEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if (!(uiEventResult == NTL_FSM_EVENTRES_PASS || uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE))
		return;

	SNtlEventActionMapAirJump *pActionMapAirJump = reinterpret_cast<SNtlEventActionMapAirJump*>(pMsg.pData);

	API_GetSLPacketGenerator()->SendAirJump(pActionMapAirJump->uiMoveFlags, m_pActor->GetPosition(), m_pActor->GetDirection());
}


////////////////////////////////////////////////////////////////////////////
//  des : ActionMapCharging event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapChargingEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	SNtlEventActionMapCharging *pCharging = reinterpret_cast<SNtlEventActionMapCharging*>( pMsg.pData );

	CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	if(pCharging->bStartOrEnd)
	{
		if(pActAgent->IsSitDown())
		{
			pActAgent->SetSitDown(FALSE);
			API_GetSLPacketGenerator()->SendSitAndStand(FALSE);  
		}

		if(pActAgent->IsFightingMode())
		{
			pActAgent->SetFightingMode(FALSE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
		}

		pActAgent->SetCharging(TRUE);

		API_GetSLPacketGenerator()->SendCharging();
	}
	else
	{
		pActAgent->SetCharging(FALSE);
		API_GetSLPacketGenerator()->SendChargingEnd();
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : ActionMapBlocking event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapBlockingEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	SNtlEventActionMapBlocking *pBlocking = reinterpret_cast<SNtlEventActionMapBlocking*>( pMsg.pData );

	if(pBlocking->bStartOrEnd)
	{
		CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
		if(pActAgent->IsSitDown())
		{
			pActAgent->SetSitDown(FALSE);
		}

		API_GetSLPacketGenerator()->SendBlocking();
	}
	else
	{
		API_GetSLPacketGenerator()->SendBlockingEnd();
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : ActionMapSitAndDown event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////


void CNtlFSMPlayerController::NotifyActionMapSitAndDownEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	if(uiParam1 == NTL_FSMSID_SKILL_READY || uiParam1 == NTL_FSMSID_SKILL_CASTING)
	{
		// skill cancel 을 보낸다.
	}

	// agent
	CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	pActAgent->SetSitDown(!pActAgent->IsSitDown());
	API_GetSLPacketGenerator()->SendSitAndStand(pActAgent->IsSitDown());  
	
}


////////////////////////////////////////////////////////////////////////////
//  des : ActionSkillUse event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapSkillUseEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	SNtlEventActionMapSkillUse *pActionMapSkillUse = reinterpret_cast<SNtlEventActionMapSkillUse*>(pMsg.pData); 
	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);

	RwUInt32 uiNextState = uiParam2;
	if(uiNextState == NTL_FSMSID_SKILL_FOLLOW)
	{
		API_GetSLPacketGenerator()->SendCharFollowMove(pActionMapSkillUse->hTargetSerialId, pActionMapSkillUse->fFollowRange, DBO_MOVE_FOLLOW_SKILL_ATTACK, m_pActor->GetServerSyncAvatarType());
		
	}
	else if(uiNextState == NTL_FSMSID_SKILL_READY)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
		
		if(pCharActAgent->IsSitDown())
		{
			pCharActAgent->SetSitDown(FALSE); 
			API_GetSLPacketGenerator()->SendSitAndStand(FALSE);  
		}

		API_GetSLPacketGenerator()->SendCharSkillReq(m_pActor, pSkillStuff->hTargetSerialId, pSkillStuff->bySlotIdx, pSkillStuff->byRpBonusType, m_pActor->GetServerSyncAvatarType());
	}

	// war mode로 변경
	if(m_pActor->GetSerialID() != pActionMapSkillUse->hTargetSerialId)
	{
		if(!pCharActAgent->IsFightingMode() && Logic_IsSkillUseAfterAttack(pActionMapSkillUse->hSkillSerialId))
		{
			m_pSyncMsgAttacKBegin->SetTargetSerialId(pActionMapSkillUse->hTargetSerialId);
			pCharActAgent->SetFightingMode(TRUE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(TRUE, m_pActor->GetServerSyncAvatarType());
			Logic_VisualAttackSelect(pActionMapSkillUse->hTargetSerialId);
		}
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : ActionSocialUse event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapSocialUseEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	// social action use를 보낸다.
	RwUInt32 uiOldState = uiParam1;
	if(uiOldState != NTL_FSMSID_IDLE)
	{
		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vDir = m_pActor->GetDirection();
		API_GetSLPacketGenerator()->SendKeyboardMove(vPos, vDir, NTL_MOVE_NONE, m_pActor->GetServerSyncAvatarType());
	}

	CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	if(pActAgent->IsFightingMode())
	{
		pActAgent->SetFightingMode(FALSE);
		API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
	}

	SNtlEventActionMapSocialUse *pSocialUse = reinterpret_cast<SNtlEventActionMapSocialUse*>( pMsg.pData );
	API_GetSLPacketGenerator()->SendCharSocialReq(pSocialUse->uiTblId);
	CNtlSLEventGenerator::SobSocialActionCooling(pSocialUse->uiTblId);
}

//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapPrivateShopOpenEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;
	
	API_GetSLPacketGenerator()->SendPrivateShopCreateReq();			
}


////////////////////////////////////////////////////////////////////////////
//  des : ActionHTBUse event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapHTBUseEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	RwUInt32 uiNextState = uiParam2;
	if(uiNextState == NTL_FSMSID_HTB_FOLLOW)
	{
		SNtlEventActionMapHTBUse *pActionMapHTBUse = reinterpret_cast<SNtlEventActionMapHTBUse*>(pMsg.pData); 
		API_GetSLPacketGenerator()->SendCharFollowMove(pActionMapHTBUse->hTargetSerialId, pActionMapHTBUse->fFollowRange, DBO_MOVE_FOLLOW_HTB_ATTACK, m_pActor->GetServerSyncAvatarType());
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : ActionItemUse event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapItemUseEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	SNtlEventActionMapItemUse *pEvent = reinterpret_cast<SNtlEventActionMapItemUse*>(pMsg.pData); 
	API_GetSLPacketGenerator()->SendCharItemUseReq(m_pActor, pEvent->hTargetSerialId, pEvent->byPlace, pEvent->byPos, pEvent->hRefObject, pEvent->byKeyPlace, pEvent->byKeyPos);
}

////////////////////////////////////////////////////////////////////////////
//  des : ActionItemUse event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyActionMapAutoFollowEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	ReleaseAutoCmdAI();

	CreateAutoCmdAI();

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		
	API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_FRIENDLY, m_pActor->GetServerSyncAvatarType());
}


////////////////////////////////////////////////////////////////////////////
//  des : ActionSkillUse event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyInputChangeHeadingEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	RwUInt32 uiOldState = uiParam1;

	if(uiOldState == NTL_FSMSID_MOVE || uiOldState == NTL_FSMSID_FOLLOW || 
		uiOldState == NTL_FSMSID_FIGHTING_FOLLOW || uiOldState == NTL_FSMSID_SKILL_FOLLOW)
	{
		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vDir = m_pActor->GetDirection();

		CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
		if(pActAgent->IsFightingMode())
		{
			pActAgent->SetFightingMode(FALSE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
		}
		DBO_WARNING_MESSAGE("NotifyInputChangeHeadingEventHandler");
		API_GetSLPacketGenerator()->SendKeyboardMove(vPos, vDir, NTL_MOVE_NONE, m_pActor->GetServerSyncAvatarType());
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : skill cancel event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifySobSkillCancelEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	if(uiParam2 != NTL_FSMSID_SKILL_FOLLOW)
		return;

    // Rp Bonus가 적용되어 있으면 이펙트를 제거한다.
    CNtlSobCharProxy* pProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
    pProxy->DetachRPBonusEffect();   

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		
	API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_SKILL_ATTACK, m_pActor->GetServerSyncAvatarType());
}

////////////////////////////////////////////////////////////////////////////
//  des : TerraionClick event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyTerrainClickEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	pActAgent->SetSitDown(FALSE);

	RwUInt32 uiCurrState = uiParam1; 
	if(uiCurrState == NTL_FSMSID_SKILL_READY || uiCurrState == NTL_FSMSID_SKILL_CASTING)
	{
		// skill cancel 을 보낸다.
	}
	
	SNtlEventTerrainClick *pTerrainClick = reinterpret_cast<SNtlEventTerrainClick*>(pMsg.pData); 
	if(pTerrainClick->bServerSend)
	{
		if (m_pActor->IsAirMode())
		{
			CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
			SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

			API_GetSLPacketGenerator()->SendKeyboardAirMove(m_pActor->GetPosition(), m_pActor->GetDirection(), pMoveStuff->byMoveFlags);
		}
		else
		{
			API_GetSLPacketGenerator()->SendMouseMove(m_pActor->GetPosition(), pTerrainClick->vPickPos, m_pActor->GetServerSyncAvatarType());
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : KeyboardMove event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyKeyboardMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( uiEventResult == NTL_FSM_EVENTRES_BLOCK )
		return;

	CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	pActAgent->SetSitDown(FALSE);

	RwUInt32 uiCurrState = uiParam1; 
	if(uiCurrState == NTL_FSMSID_SKILL_READY || uiCurrState == NTL_FSMSID_SKILL_CASTING)
	{
		// skill cancel 을 보낸다.
	}

	if (uiCurrState == NTL_FSMSID_DASH || uiCurrState == NTL_FSMSID_FALLING) // by daneos
		return;

	SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>(pMsg.pData); 

	RwV3d vPos = m_pActor->GetPosition();
	RwV3d vDir = m_pActor->GetDirection();

	if (uiCurrState == NTL_FSMSID_JUMP /*|| uiCurrState == NTL_FSMSID_FALLING*/)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	//	DBO_WARNING_MESSAGE("move: pMoveStuff->byMoveFlags = " << (int)pMoveStuff->byMoveFlags << " pKeyboardMove->byMoveFlags: " << (int)pKeyboardMove->byMoveFlags);

		if(pMoveStuff->byMoveFlags != NTL_MOVE_NONE)
			API_GetSLPacketGenerator()->SendChangeDirectionFloating(pMoveStuff->byMoveFlags, vDir);
	}
	else
	{
		if (m_pActor->IsAirMode())
			API_GetSLPacketGenerator()->SendKeyboardAirMove(vPos, vDir, pKeyboardMove->byMoveFlags);
		else
			API_GetSLPacketGenerator()->SendKeyboardMove(vPos, vDir, pKeyboardMove->byMoveFlags, m_pActor->GetServerSyncAvatarType());
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : KeyboardMoveStop event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyKeyboardMoveStopEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( uiEventResult == NTL_FSM_EVENTRES_BLOCK )
		return;

	RwUInt32 uiCurrState = uiParam1; 
	if(uiCurrState == NTL_FSMSID_SKILL_READY || uiCurrState == NTL_FSMSID_SKILL_CASTING)
	{
		// skill cancel 을 보낸다.
	}

	SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>(pMsg.pData); 

	RwV3d vPos = m_pActor->GetPosition();
	RwV3d vDir = m_pActor->GetDirection();


	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
//	DBO_WARNING_MESSAGE("movestop: Stuff->byMoveFlags= " << (int)pMoveStuff->byMoveFlags << " keyboard->byMoveFlags= " << (int)pKeyboardMove->byMoveFlags);
	if (uiCurrState == NTL_FSMSID_JUMP || uiCurrState == NTL_FSMSID_FALLING)
	{
		API_GetSLPacketGenerator()->SendChangeDirectionFloating(pMoveStuff->byMoveFlags, vDir);
	}
	else
	{
		if (m_pActor->IsAirMode())
		{
			API_GetSLPacketGenerator()->SendKeyboardAirMove(vPos, vDir, NTL_MOVE_NONE);
		}
		else
		{
			API_GetSLPacketGenerator()->SendKeyboardMove(vPos, vDir, NTL_MOVE_NONE, m_pActor->GetServerSyncAvatarType());
		}
	}
}



////////////////////////////////////////////////////////////////////////////
//  des : Mouse Dash Move event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyMouseDashMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS || uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 

	API_GetSLPacketGenerator()->SendMouseDashMove(pMoveStuff->vDest);

	// event queueing.
	if (pMoveStuff->byMoveFlags != NTL_MOVE_NONE)
	{
		AgentQueuingKeyboardMove(m_pAgent, m_byKeyboardMoveFlags);
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : Keyboard Dash Move event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifyKeyboardDashMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS || uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	SNtlEventKeyboardDashMove *pKeyboardDashMove = reinterpret_cast<SNtlEventKeyboardDashMove*>(pMsg.pData); 

	API_GetSLPacketGenerator()->SendKeyboardDashMove(m_pActor->GetPosition(), m_pActor->GetDirection(), (RwUInt8)pKeyboardDashMove->uiMoveFlags);

	// event queueing.
	if (pKeyboardDashMove->uiMoveFlags != NTL_MOVE_NONE)
	{
		AgentQueuingKeyboardMove(m_pAgent, m_byKeyboardMoveFlags);
	}
}

void CNtlFSMPlayerController::NotifyAirDashMoveEventHandler(RWS::CMsg & pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if (!(uiEventResult == NTL_FSM_EVENTRES_PASS || uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE))
		return;

	SNtlEventAirDashMove *pAirDashMove = reinterpret_cast<SNtlEventAirDashMove*>(pMsg.pData);
	API_GetSLPacketGenerator()->SendAirDashMove(m_pActor->GetPosition(), m_pActor->GetDirection(), (RwUInt8)pAirDashMove->uiMoveFlags);
}

void CNtlFSMPlayerController::NotifyAirAccelMoveEventHandler(RWS::CMsg& pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if (!(uiEventResult == NTL_FSM_EVENTRES_PASS || uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE))
		return;

	SNtlEventAirDashMove * pAirDashMove = reinterpret_cast<SNtlEventAirDashMove*>(pMsg.pData);
	API_GetSLPacketGenerator()->SendAirAccelMove(m_pActor->GetPosition(), m_pActor->GetDirection(), (RwUInt8)pAirDashMove->uiMoveFlags);
}


////////////////////////////////////////////////////////////////////////////
//  des : SobDbClick event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifySobTargetSelectEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>(pMsg.pData);

	RwUInt32 uiNextState = uiParam2;
	if(uiNextState == NTL_FSMSID_FOLLOW)
	{
		m_pSyncMsgAttacKBegin->SetTargetSerialId(pSobTargetSelect->hSerialId);

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 

		API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_FRIENDLY, m_pActor->GetServerSyncAvatarType());
	}
	else
	{
		m_pSyncMsgAttacKBegin->SetTargetSerialId(INVALID_SERIAL_ID);
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : SobDbClick event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifySobAttackSelectEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	SNtlEventSobAttackSelect *pSobAttackSelect = reinterpret_cast<SNtlEventSobAttackSelect*>(pMsg.pData);

	RwUInt32 uiNextState = uiParam2;

	if(uiNextState == NTL_FSMSID_FIGHTING_FOLLOW)
	{
		m_pSyncMsgAttacKBegin->SetTargetSerialId(pSobAttackSelect->hSerialId);

		CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
		if(!pActAgent->IsFightingMode())
		{
			pActAgent->SetFightingMode(TRUE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(TRUE, m_pActor->GetServerSyncAvatarType());
		}
	}
	else if(uiNextState == NTL_FSMSID_FOLLOW)
	{
		m_pSyncMsgAttacKBegin->SetTargetSerialId(pSobAttackSelect->hSerialId);

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 

		API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_FRIENDLY, m_pActor->GetServerSyncAvatarType());
	}
	else
	{
		m_pSyncMsgAttacKBegin->SetTargetSerialId(INVALID_SERIAL_ID);
	}
}


////////////////////////////////////////////////////////////////////////////
//  des : SobFollowMove event에 의한 처리 결과를 notify 해주는 함수. .
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
//  \param2 : event 처리 결과.
//  \param3 : previous state.
//  \param4 : next state.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMPlayerController::NotifySobFollowMoveEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS ||  uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	RwUInt32 uiNextState = uiParam2;

	CNtlFSMCharActAgent *pActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	if(uiNextState == NTL_FSMSID_FOLLOW)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 

		if(pActAgent->IsFightingMode())
		{
			pActAgent->SetFightingMode(FALSE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
		}

		API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_FRIENDLY, m_pActor->GetServerSyncAvatarType());
	}
	else if(uiNextState == NTL_FSMSID_FIGHTING_FOLLOW)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 

		if(!pActAgent->IsFightingMode())
		{
			pActAgent->SetFightingMode(TRUE);
			API_GetSLPacketGenerator()->SendCharToggleFighting(TRUE, m_pActor->GetServerSyncAvatarType());
		}

		API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_AUTO_ATTACK, m_pActor->GetServerSyncAvatarType());
	}
}


void CNtlFSMPlayerController::NotifySobHitEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	ReleaseAutoCmdAI();

	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS || uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;

	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	if(pCharActAgent->IsSitDown())
	{
		pCharActAgent->SetSitDown(FALSE);
		API_GetSLPacketGenerator()->SendSitAndStand(FALSE);  
	}
}

void CNtlFSMPlayerController::NotifySobSpecialAttackEventHandler(RWS::CMsg& pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	ReleaseAutoCmdAI();

	if (!(uiEventResult == NTL_FSM_EVENTRES_PASS || uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE))
		return;

	CNtlFSMCharActAgent * pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	if (pCharActAgent->IsSitDown())
	{
		pCharActAgent->SetSitDown(FALSE); 
		API_GetSLPacketGenerator()->SendSitAndStand(FALSE);
	}
}

void CNtlFSMPlayerController::NotifySobAnimPlayEventHandler(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if( !(uiEventResult == NTL_FSM_EVENTRES_PASS || uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE) )
		return;
	DBO_WARNING_MESSAGE("CNtlFSMPlayerController::NotifySobAnimPlayEventHandler");
	SNtlEventSobAnimPlay *pAnimPlay = reinterpret_cast<SNtlEventSobAnimPlay*>(pMsg.pData);
	API_GetSLPacketGenerator()->SendBudokaiSocialAction(pAnimPlay->uiAnimId);  
}

RwUInt32 CNtlFSMPlayerController::SobSendAttackEndHandler( RWS::CMsg& pMsg )
{
	m_pSyncMsgAttacKBegin->SetAttackBeginFlag(FALSE);
 //	API_GetSLPacketGenerator()->SendCharAttackEnd(m_pSyncMsgAttacKBegin->GetAvatarControlId(), m_pActor->GetServerSyncAvatarType()); // disabled by daneos

    return NTL_FSM_EVENTRES_BLOCK;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMPlayerSyncController)

CNtlFSMPlayerSyncController::CNtlFSMPlayerSyncController()
{
	SetControllerId(SLCONTROLLER_PLAYER_SYNC);
}

void CNtlFSMPlayerSyncController::Update(RwReal fElapsed)
{
}

RwUInt32 CNtlFSMPlayerSyncController::HandleEvents(RWS::CMsg &pMsg)
{
	return NTL_FSM_EVENTRES_PASS;
}

void CNtlFSMPlayerSyncController::NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState)
{
	//DBO_WARNING_MESSAGE("CNtlFSMPlayerSyncController::NotifyUpdateChangeState");
}

void CNtlFSMPlayerSyncController::NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMServerController)

CNtlFSMServerController::CNtlFSMServerController()
{
	SetControllerId(SLCONTROLLER_SERVER);

	m_fRangeCheckTime = 0.0f;
}

void CNtlFSMServerController::Update(RwReal fElapsed)
{
	CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(m_pAgent);
	RwUInt8 byServerState = pCharActAgent->GetServerState();

	// follow 처리.
	if(byServerState == CHARSTATE_FOLLOWING)
	{
		UpdateServerFollowState(fElapsed);
	}
}

RwUInt32 CNtlFSMServerController::HandleEvents(RWS::CMsg &pMsg)
{
	return NTL_FSM_EVENTRES_PASS;
}

void CNtlFSMServerController::NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState)
{
	//DBO_WARNING_MESSAGE("CNtlFSMPlayerSyncController::NotifyUpdateChangeState");
}

void CNtlFSMServerController::NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
}


void CNtlFSMServerController::UpdateServerFollowState(RwReal fElapsed)
{
	RwUInt32 uiCurrStateId = m_pAgent->GetCurrStateId();

	if(uiCurrStateId == NTL_FSMSID_IDLE || uiCurrStateId == NTL_FSMSID_FIGHTING_POSE)
	{
		m_fRangeCheckTime += fElapsed;
		if(m_fRangeCheckTime < 0.25f)
			return;

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		CNtlSob *pTarSob = GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId);
		if(pTarSob == NULL)
			return;

		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vTarPos = pTarSob->GetPosition();

		RwV3d vSub;
		RwV3dSubMacro(&vSub, &vTarPos, &vPos);
		RwReal fLen = RwV3dLength(&vSub);
		if(fLen <= pCtrlStuff->fFollowRange)
			return;

		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
		pMoveStuff->Reset();
		pMoveStuff->byType = NTL_MOVETARGET_ACTOR;	
		pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
		pMoveStuff->hFollowSerialId = pCtrlStuff->hTargetSerialId;

		// 현재 위치가 육지인지 물속인지 파악한다.    
		SWorldHeightStuff hStuff;
		if(Logic_IsSwimmingActor(m_pActor, &m_pActor->GetPosition(), hStuff))
			pMoveStuff->byForm = NTL_MOVEFORM_SWIMMING;
		else
		{
			if (m_pActor->IsAirMode())
			{
				pMoveStuff->byForm = NTL_MOVEFORM_FLY;
				pMoveStuff->byFormFlag = NTL_MOVE_FLAG_FLY;
			}
			else
			{
				pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
			}
		}

		pMoveStuff->fFollowRange = pCtrlStuff->fFollowRange;
		pMoveStuff->vCurrLoc = m_pActor->GetDirection();

		CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_FOLLOW);
		m_fRangeCheckTime = 0.0f;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMSummonPetController)

CNtlFSMSummonPetController::CNtlFSMSummonPetController() 
{
	m_pPetAI = NTL_NEW CNtlSummonPetAI;
	m_pPetAI->Create(this);

	SetControllerId(SLCONTROLLER_SUMMON_PET);
	m_pSyncMsgAttacKBegin->SetAvatarControlId(1);
}

CNtlFSMSummonPetController::~CNtlFSMSummonPetController()
{
	if(m_pPetAI)
	{
		NTL_DELETE(m_pPetAI);
	}
}

void CNtlFSMSummonPetController::Update(RwReal fElapsed)
{
	CNtlFSMPlayerController::Update(fElapsed);

	m_pPetAI->Update(fElapsed);
}

RwUInt32 CNtlFSMSummonPetController::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMSummonPetController::HandleEvents");
	
	if(pMsg.Id == g_EventSobPetBeginAttack)
	{
		SNtlEventSobPetBeginAttack *pPetBeginAttack = reinterpret_cast<SNtlEventSobPetBeginAttack*>( pMsg.pData );
		if(m_pActor->CanAttack())
		{
			m_pPetAI->UserCommandAttack(pPetBeginAttack->hDefenderSerialId);
		}

		NTL_RETURN(NTL_FSM_EVENTRES_PASS);
	}
	else
	{
		NTL_RETURN(CNtlFSMPlayerController::HandleEvents(pMsg));
	}
}


void CNtlFSMSummonPetController::NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState)
{
	/*if(uiNextState == NTL_FSMSID_TELEPORT)
	{
		m_pPetAI->ChangeState(CNtlSummonPetAI::ESPS_IDLE);
	}*/
	//DBO_WARNING_MESSAGE("CNtlFSMPlayerSyncController::NotifyUpdateChangeState");
	if(uiNextState == NTL_FSMSID_TELEPORT || uiNextState == NTL_FSMSID_IDLE )
	{
		m_pPetAI->ChangeState(CNtlSummonPetAI::ESPS_IDLE);
	}

	CNtlFSMPlayerController::NotifyUpdateChangeState(uiOldState, uiNextState);
}


