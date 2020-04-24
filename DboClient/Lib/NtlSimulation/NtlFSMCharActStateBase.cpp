#include "precomp_ntlsimulation.h"
#include "NtlFSMCharActStateBase.h"

// shared
#include "NtlResultCode.h"
#include "ActionTable.h"
#include "NtlMovement.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlAnimEventData.h"

// simulation
#include "NtlFSMCharActEventProc.h"
#include "NtlBehaviorData.h"
#include "NtlSobActor.h"
#include "NtlSLLuaGlue.h"
#include "NtlSobManager.h"
#include "NtlSobElapsedController.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "NtlSobProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSlGlobal.h"
#include "NtlFSMCharActAgent.h"
#include "NtlInventory.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLVisualLogic.h"

CNtlFSMCharActStateBase::CNtlFSMCharActStateBase()
{
	// event handler function link
	m_mapEventFunc[g_EventActionMapSitAndStand.Get_pEventId()]	= &CNtlFSMCharActStateBase::EventProcActionSitAndStand;
	m_mapEventFunc[g_EventActionMapJump.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcActionMapJump;
	m_mapEventFunc[g_EventActionMapAirJump.Get_pEventId()] = &CNtlFSMCharActStateBase::EventProcActionMapAirJump;
	m_mapEventFunc[g_EventActionMapSkillUse.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcActionMapSkillUse;
	m_mapEventFunc[g_EventActionMapHTBUse.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcActionMapHTBUse;
	m_mapEventFunc[g_EventActionMapItemUse.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcActionMapItemUse;
	m_mapEventFunc[g_EventActionMapSocialUse.Get_pEventId()]	= &CNtlFSMCharActStateBase::EventProcActionMapSocialUse;
	m_mapEventFunc[g_EventActionMapPrivateShopOpen.Get_pEventId()]	= &CNtlFSMCharActStateBase::EventProcActionMapPrivateShopOpen;
	m_mapEventFunc[g_EventActionMapCharging.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcActionMapCharging;
	m_mapEventFunc[g_EventActionMapBlocking.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcActionMapBlocking;
	m_mapEventFunc[g_EventActionMapAutoFollow.Get_pEventId()]	= &CNtlFSMCharActStateBase::EventProcActionMapAutoFollow;

	m_mapEventFunc[g_EventTerrainClick.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcInputTerrainClick;
	m_mapEventFunc[g_EventKeyboardMove.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcInputKeyboardMove;
	m_mapEventFunc[g_EventKeyboardMoveStop.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcInputKeyboardMoveStop;
	m_mapEventFunc[g_EventMouseDashMove.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcInputMouseDashMove;
	m_mapEventFunc[g_EventKeyboardDashMove.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcInputKeyboardDashMove;
	m_mapEventFunc[g_EventAirDashMove.Get_pEventId()]	= &CNtlFSMCharActStateBase::EventProcInputAirDashMove;
	m_mapEventFunc[g_EventAirAccelMove.Get_pEventId()] = &CNtlFSMCharActStateBase::EventProcInputAirAccelMove;
	m_mapEventFunc[g_EventInputChangeHeading.Get_pEventId()]	= &CNtlFSMCharActStateBase::EventProcInputChangeHeading;

	m_mapEventFunc[g_EventSobMove.Get_pEventId()]				= &CNtlFSMCharActStateBase::EventProcSobMove;
	m_mapEventFunc[g_EventSobSecondDestMove.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobSecondDestMove;
	m_mapEventFunc[g_EventSobSplineMove.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobSplineMove;
	m_mapEventFunc[g_EventSobDash.Get_pEventId()]				= &CNtlFSMCharActStateBase::EventProcSobDash;
	m_mapEventFunc[g_EventSobFollowMove.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobFollowMove;
	m_mapEventFunc[g_EventSobMoveStop.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobMoveStop;
	m_mapEventFunc[g_EventSobJump.Get_pEventId()]				= &CNtlFSMCharActStateBase::EventProcSobJump;
	m_mapEventFunc[g_EventSobJumpDirection.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobJumpDirection;
	m_mapEventFunc[g_EventSobJumpEnd.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobJumpEnd;
	m_mapEventFunc[g_EventSobSit.Get_pEventId()]				= &CNtlFSMCharActStateBase::EventProcSobSit;
	m_mapEventFunc[g_EventSobDirectPlay.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobDirectPlay;
	m_mapEventFunc[g_EventSobAnimPlay.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobAnimPlay;
	m_mapEventFunc[g_EventSobPrivateShop.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobPrivateShop;
	m_mapEventFunc[g_EventPrivateShopState.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobPrivateShopState;
	m_mapEventFunc[g_EventSobPushingNfy.Get_pEventId()]         = &CNtlFSMCharActStateBase::EventProcSobPushingNfy;
	m_mapEventFunc[g_EventSobAirJump.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobAirJump;
	m_mapEventFunc[g_EventSobAirAccel.Get_pEventId()] = &CNtlFSMCharActStateBase::EventProcSobAirAccel;

	m_mapEventFunc[g_EventSobTargetSelect.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobTargetSelect;
	m_mapEventFunc[g_EventSobAttackSelect.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobAttackSelect;
	m_mapEventFunc[g_EventSobAttack.Get_pEventId()]				= &CNtlFSMCharActStateBase::EventProcSobAttack;
	m_mapEventFunc[g_EventSobSpecialAttack.Get_pEventId()] = &CNtlFSMCharActStateBase::EventProcSobSpecialAttack;
	m_mapEventFunc[g_EventSobAttacked.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobAttacked;
	m_mapEventFunc[g_EventSobHit.Get_pEventId()]				= &CNtlFSMCharActStateBase::EventProcSobHit;
	m_mapEventFunc[g_EventSobSkillCancel.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobSkillCancel;
	m_mapEventFunc[g_EventSobSkillCasting.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobSkillCasting;
	m_mapEventFunc[g_EventSobSkillAction.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobSkillAction;
	m_mapEventFunc[g_EventSobSkillActioned.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobSkillActioned;

	m_mapEventFunc[g_EventSobHTBSkillAction.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobHTBSkillAction;
	m_mapEventFunc[g_EventSobSocialAction.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobSocialAction;
	m_mapEventFunc[g_EventSobItemCancel.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobItemCancel;
	m_mapEventFunc[g_EventSobItemCasting.Get_pEventId()]		= &CNtlFSMCharActStateBase::EventProcSobItemCasting;
	m_mapEventFunc[g_EventSobFainting.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobFainting;
	m_mapEventFunc[g_EventWeightTime.Get_pEventId()]			= &CNtlFSMCharActStateBase::EventProcSobWeightTime;

	m_mapEventFunc[g_EventAfterEffectRemainTimeNfy.Get_pEventId()]	= &CNtlFSMCharActStateBase::EventProcAfterEffectRemainTime;
	m_mapEventFunc[g_EventSobCondConfused.Get_pEventId()]       = &CNtlFSMCharActStateBase::EventProcCondConfused;
	m_mapEventFunc[g_EventSobCondTerror.Get_pEventId()]         = &CNtlFSMCharActStateBase::EventProcCondTerror;
    m_mapEventFunc[g_EventSobTransformSequela.Get_pEventId()]   = &CNtlFSMCharActStateBase::EventSobTransformSequela;
}

RwBool CNtlFSMCharActStateBase::Create(void)
{
	return CNtlFSMStateBase::Create();
}

void CNtlFSMCharActStateBase::Destroy(void)
{
	CNtlFSMStateBase::Destroy();
}

void CNtlFSMCharActStateBase::Enter(void)
{
	CNtlFSMStateBase::Enter();
}

void CNtlFSMCharActStateBase::Exit(void)
{
	CNtlFSMStateBase::Exit();
}

RwUInt32 CNtlFSMCharActStateBase::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::HandleEvents");

	CNtlFSMStateBase::HandleEvents(pMsg);

	MapFuncEventHandler::iterator it = m_mapEventFunc.find(pMsg.Id);
	if(it != m_mapEventFunc.end())
	{
		RwUInt32 uiRes = (this->*(*it).second)(pMsg);
		NTL_RETURN(uiRes);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlFSMCharActStateBase::Clone(CNtlFSMStateBase *pState)
{
	CNtlFSMStateBase::Clone(pState);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcActionSitAndStand(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionSitAndStand");

	if(GetFlags() & NTL_FSMSF_NOT_SIT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapJump(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapJump");

	if(GetFlags() & NTL_FSMSF_NOT_JUMP)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActActionMapJump(m_pActor, reinterpret_cast<SNtlEventActionMapJump*>( pMsg.pData ), this);

	Finish();
	SetNextStateId(NTL_FSMSID_JUMP);
	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapAirJump(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapAirJump");

	if (GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActActionMapAirJump(m_pActor, reinterpret_cast<SNtlEventActionMapAirJump*>(pMsg.pData), this);

	Finish();
	SetNextStateId(NTL_FSMSID_AIR_JUMP);
	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapSkillUse(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapSkillUse");

	if(GetFlags() & NTL_FSMSF_NOT_SKILLUSE)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	//
	SNtlEventActionMapSkillUse *pSkillUse = reinterpret_cast<SNtlEventActionMapSkillUse*>(pMsg.pData);

	// skill 사용 effect 출력
	Logic_SkillUseVisualEffect(m_pActor, pSkillUse->hSkillSerialId);

	// Event 처리
	FSMEvent_CharActActionMapSkillUse(m_pActor, pSkillUse);

	if(Logic_IsNotFollowMoveSkill(m_pActor, pSkillUse->bySkillSlotIdx))
	{
		SetNextStateId(NTL_FSMSID_SKILL_READY);
	}
	else
	{
		if(pSkillUse->hSerialId == pSkillUse->hTargetSerialId)
			SetNextStateId(NTL_FSMSID_SKILL_READY);
		else
			SetNextStateId(NTL_FSMSID_SKILL_FOLLOW);
	}

	Finish();

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapHTBUse(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapHTBUse");

	if(GetFlags() & NTL_FSMSF_NOT_SKILLUSE)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	SNtlEventActionMapHTBUse *pHTBUse = reinterpret_cast<SNtlEventActionMapHTBUse*>(pMsg.pData);

	FSMEvent_CharActActionMapHTBUse(m_pActor, pHTBUse);

	SetNextStateId(NTL_FSMSID_HTB_FOLLOW);

	Finish();
	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapItemUse(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapItemUse");

	// Disabled by daneos: So we can use items while being stunned/paralyzed etc..
	/*if(GetFlags() & NTL_FSMSF_NOT_SKILLUSE)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}*/

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	SNtlEventActionMapItemUse *pItemUse = reinterpret_cast<SNtlEventActionMapItemUse*>(pMsg.pData);
	if(pItemUse->byCastingTime > 0)
	{
		SetNextStateId(NTL_FSMSID_ITEM_READY);

		Finish();
		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapSocialUse(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapSocialUse");

	if(GetFlags() & NTL_FSMSF_NOT_SOCIALUSE)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActActionMapSocialUse(m_pActor, reinterpret_cast<SNtlEventActionMapSocialUse*>( pMsg.pData ));

	Finish();
	SetNextStateId(NTL_FSMSID_SOCIAL_ACTION);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapPrivateShopOpen(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapSocialUse");

	if(GetFlags() & NTL_FSMSF_NOT_SOCIALUSE)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		CNtlSLEventGenerator::SysMsg(m_pActor->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapCharging(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapCharging");

	if(GetFlags() & NTL_FSMSF_NOT_SKILLUSE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	SNtlEventActionMapCharging *pCharging = reinterpret_cast<SNtlEventActionMapCharging*>( pMsg.pData );
	if(pCharging->bStartOrEnd)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_CHARGING);
	}
	else
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapBlocking(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapBlocking");

	if(GetFlags() & NTL_FSMSF_NOT_SKILLUSE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	SNtlEventActionMapBlocking *pBlocking = reinterpret_cast<SNtlEventActionMapBlocking*>( pMsg.pData );
	if(pBlocking->bStartOrEnd)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_BLOCKING);
	}
	else
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcActionMapAutoFollow(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapAutoFollow");

	if(GetFlags() & NTL_FSMSF_NOT_MOVE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetStateId() == NTL_FSMSID_FOLLOW)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

		SNtlEventActionMapAutoFollow *pAutoFollow = reinterpret_cast<SNtlEventActionMapAutoFollow*>(pMsg.pData);
		if(pAutoFollow->hTargetSerialId == pCtrlStuff->hTargetSerialId)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_PASS);
		}
	}

	FSMEvent_CharActActionMapAutoFollow(m_pActor, reinterpret_cast<SNtlEventActionMapAutoFollow*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_FOLLOW);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcInputTerrainClick(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcTerrainClick");

	if(GetFlags() & NTL_FSMSF_NOT_MOVE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActTerrainClick(m_pActor, reinterpret_cast<SNtlEventTerrainClick*>(pMsg.pData)); 

	// 현재 이동상태가 아니면.
	RwUInt32 uiStateId = GetStateId();
	if( uiStateId == NTL_FSMSID_MOVE || uiStateId == NTL_FSMSID_SWIMMING)
	{
		// behavior data를 update한다.
		UpdateBehavior();
	}
	else
	{
		Finish();

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 

		RwV3d vPos = m_pActor->GetPosition();
		SWorldHeightStuff sHStuff;
		if(Logic_IsSwimmingActor(m_pActor, &vPos, sHStuff))
		{
			pMoveStuff->byForm = NTL_MOVEFORM_SWIMMING;
			SetNextStateId(NTL_FSMSID_SWIMMING);
		}
		else
		{
			if (m_pActor->IsAirMode())
			{
				pMoveStuff->byForm = NTL_MOVEFORM_FLY;
			}
			else
			{
				pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
			}

			SetNextStateId(NTL_FSMSID_MOVE);
		}

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcInputKeyboardMove(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcKeyboardMove");

	if(GetFlags() & NTL_FSMSF_NOT_MOVE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>(pMsg.pData);

	if( GetStateId() == NTL_FSMSID_MOVE)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
		if(pMoveStuff->byType == NTL_MOVETARGET_DIR && pKeyboardMove->byMoveFlags == pMoveStuff->byMoveFlags)
		{
			if(pKeyboardMove->bAvatarAngleChange)
				m_pActor->SetAngleY(pKeyboardMove->fAvatarAngleY);
	
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
	}

	FSMEvent_CharActKeyboardMove(m_pActor, pKeyboardMove); 

	RwUInt32 uiStateId = GetStateId();

	if( uiStateId == NTL_FSMSID_MOVE || uiStateId == NTL_FSMSID_SWIMMING)
	{
		// behavior data를 update한다.
		UpdateBehavior();
	}
	else // 현재 이동상태가 아니면.
	{
		Finish();

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 

		RwV3d vPos = m_pActor->GetPosition();
		SWorldHeightStuff sHStuff;
		if(Logic_IsSwimmingActor(m_pActor, &vPos, sHStuff))
		{
			pMoveStuff->byForm = NTL_MOVEFORM_SWIMMING;
			SetNextStateId(NTL_FSMSID_SWIMMING);
		}
		else
		{
			if (m_pActor->IsAirMode())
			{
				pMoveStuff->byForm = NTL_MOVEFORM_FLY;
			}
			else
			{
				pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
			}

			SetNextStateId(NTL_FSMSID_MOVE);
		}

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcInputKeyboardMoveStop(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcKeyboardMoveStop");

	if(GetFlags() & NTL_FSMSF_NOT_MOVE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}



	FSMEvent_CharActKeyboardMoveStop(m_pActor); 

	Finish();
	SetNextStateId(NTL_FSMSID_IDLE);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcInputKeyboardDashMove(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcKeyboardDashMove");

	if(GetFlags() & NTL_FSMSF_NOT_MOVE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActKeyboardDashMove(m_pActor, reinterpret_cast<SNtlEventKeyboardDashMove*>(pMsg.pData)); 

	Finish();
	SetNextStateId(NTL_FSMSID_DASH);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcInputMouseDashMove(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcMouseDashMove");

	if(GetFlags() & NTL_FSMSF_NOT_MOVE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActMouseDashMove(m_pActor, reinterpret_cast<SNtlEventMouseDashMove*>(pMsg.pData)); 

	Finish();
	SetNextStateId(NTL_FSMSID_DASH);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcInputAirDashMove(RWS::CMsg & pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcInputAirDashMove");

	if (GetFlags() & NTL_FSMSF_NOT_MOVE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if (GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	RwBool bUpdateMove = FSMEvent_CharActAirDashMove(m_pActor, reinterpret_cast<SNtlEventAirDashMove*>(pMsg.pData));

	if (!bUpdateMove)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_PASS); // pass, so we sent packet to the server
	}

	Finish();
	SetNextStateId(NTL_FSMSID_AIR_ACCEL);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcInputAirAccelMove(RWS::CMsg& pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcInputAirAccelMove");

	if (GetFlags() & NTL_FSMSF_NOT_MOVE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if (GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	RwBool bUpdateMove = FSMEvent_CharActAirAccelMove(m_pActor, reinterpret_cast<SNtlEventAirAccelMove*>(pMsg.pData));

	if (!bUpdateMove)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_PASS); // pass, so we sent packet to the server
	}

	Finish();
	SetNextStateId(NTL_FSMSID_AIR_ACCEL);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcInputChangeHeading(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcInputChangeHeading");

	SNtlEventInputChangeHading *pChangeHeading = reinterpret_cast<SNtlEventInputChangeHading*>( pMsg.pData );

	if(GetFlags() & NTL_FSMSF_NOT_CAHNGE_HEADING)
	{
		pChangeHeading->bSuccess = FALSE;
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_INPUT)
	{
		pChangeHeading->bSuccess = FALSE;
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(!pChangeHeading->bSoothing)
	{
		m_pActor->SetDeltaAngleY(pChangeHeading->fDeltaAngleY);

		if (pChangeHeading->bSetAngleX)
			m_pActor->SetDeltaAngleX(pChangeHeading->fDeltaAngleX);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActInputChangeHeading(m_pActor, pChangeHeading);

	Finish();
	SetNextStateId(NTL_FSMSID_SMOOTH_HEADING);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

/// Sob Object를 클릭했을때 호출된다.
RwUInt32 CNtlFSMCharActStateBase::EventProcSobTargetSelect(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobTargetSelect");

	SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>(pMsg.pData);

	// Let's never erase it.
	// target One guy stops while moving ... (Aug 24, 2007)
	// Because of me, people are struggling.
	if(m_pActor->GetSerialID() == pSobTargetSelect->hSerialId)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(!pSobTargetSelect->bFollow)
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);

	RwUInt32 uiFlags = GetFlags();
	if( (uiFlags & NTL_FSMSF_NOT_ATTACK_CAN) || (uiFlags & NTL_FSMSF_NOT_MOVE) )
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	CNtlSobActor *pSobTargetActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pSobTargetSelect->hSerialId) );	
	if(!pSobTargetActor)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_PASS);
	}

	if( Logic_IsCommunityActor(pSobTargetActor) )
	{
		FSMEvent_CharActSobTargetSelect(m_pActor, pSobTargetSelect); 

		// Do not FOLLOW if you are in a community distance, but do it in the AvatarController (by agebreak 2008.01.22)
		if(Logic_InFollowRange((CNtlSobActor*)GetNtlSLGlobal()->GetSobAvatar(), pSobTargetActor, Logic_GetCommunityRange(pSobTargetActor)))
		{
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		else
		{
			Finish();
			SetNextStateId(NTL_FSMSID_FOLLOW);
			NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
		}
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobAttackSelect(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobAttackSelect");

	if(GetFlags() & NTL_FSMSF_NOT_ATTACK_CAN)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	SNtlEventSobAttackSelect *pSobAttackSelect = reinterpret_cast<SNtlEventSobAttackSelect*>(pMsg.pData);

	CNtlSobActor *pSobTarActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pSobAttackSelect->hSerialId));
	NTL_ASSERT(pSobTarActor, __FUNCTION__ << __LINE__ << "SobTargetActor Is NULL!");
	if (!pSobTarActor)
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	// Let's not get rid of it..... 
	// direciton 이 0, 0, 0 나온다.. (2006. 6. 16)
	// Because of me, people are stunned.
	if(m_pActor->GetSerialID() == pSobAttackSelect->hSerialId)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	// When it is equal to existing target
	if(pCtrlStuff->hTargetSerialId == pSobTarActor->GetSerialID())
	{
		if(GetFlags() & NTL_FSMSF_SKIP_ATTACK_CLICK)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
	}

	if( Logic_IsCommunityActor(pSobTarActor) )
	{
		FSMEvent_CharActSobAttackSelect(m_pActor, pSobAttackSelect); 

		Finish();
		SetNextStateId(NTL_FSMSID_FOLLOW);
	}
	else
	{
		FSMEvent_CharActSobAttackSelect(m_pActor, pSobAttackSelect);
		Finish();

		/*CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if (Logic_InFollowRange(pAvatar, pSobTarActor, Logic_GetAttackRange(pAvatar, pSobAttackSelect->hSerialId)))
			SetNextStateId(NTL_FSMSID_FIGHTING_POSE);
		else*/
			SetNextStateId(NTL_FSMSID_FIGHTING_FOLLOW);
	}

	/*if (GetNextStateId() == NTL_FSMSID_FIGHTING_FOLLOW || GetNextStateId() == NTL_FSMSID_FIGHTING_POSE)
	{
		if (GetStateId() == NTL_FSMSID_SKILL_READY || GetStateId() == NTL_FSMSID_SKILL_CASTING)
			NTL_RETURN(NTL_FSM_EVENTRES_PASS);
	}*/

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcSobMove(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobMove");

	FSMEvent_CharActSobMove(m_pActor, reinterpret_cast<SNtlEventSobMove*>(pMsg.pData)); 
	Finish();
	SetNextStateId(NTL_FSMSID_MOVE);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobSecondDestMove(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobSecondDestMove");

	FSMEvent_CharActSobSecondDestMove(m_pActor, reinterpret_cast<SNtlEventSobSecondDestMove*>(pMsg.pData)); 
	Finish();
	SetNextStateId(NTL_FSMSID_MOVE);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobSplineMove(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobSplineMove");

	FSMEvent_CharActSobSplineMove(m_pActor, reinterpret_cast<SNtlEventSobSplineMove*>(pMsg.pData)); 
	Finish();
	SetNextStateId(NTL_FSMSID_MOVE);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcSobDash(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobDash");

	FSMEvent_CharActSobDash(m_pActor, reinterpret_cast<SNtlEventSobDash*>(pMsg.pData)); 
	Finish();
	SetNextStateId(NTL_FSMSID_DASH);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcSobFollowMove(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobFollowMove");

	if(GetFlags() & NTL_FSMSF_NOT_MOVE)
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);

	SNtlEventSobFollowMove *pFollowMove = reinterpret_cast<SNtlEventSobFollowMove*>(pMsg.pData);
	if(pFollowMove->byReason == DBO_MOVE_FOLLOW_SKILL_ATTACK)
	{
		FSMEvent_CharActSobSkillFollowMove(m_pActor, pFollowMove); 
		Finish();
		SetNextStateId(NTL_FSMSID_SKILL_FOLLOW);

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}
	else if(pFollowMove->byReason == DBO_MOVE_FOLLOW_AUTO_ATTACK)
	{
		FSMEvent_CharActSobFightingFollowMove(m_pActor, pFollowMove); 
		Finish();
		SetNextStateId(NTL_FSMSID_FIGHTING_FOLLOW);

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}
	else
	{
		FSMEvent_CharActSobFollowMove(m_pActor, pFollowMove); 
		Finish();
		SetNextStateId(NTL_FSMSID_FOLLOW);

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}
}


RwUInt32 CNtlFSMCharActStateBase::EventProcSobMoveStop(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobMoveStop");

	SNtlEventSobMoveStop *pMoveStop = reinterpret_cast<SNtlEventSobMoveStop*>( pMsg.pData );

	FSMEvent_CharActSobMoveStop(m_pActor, pMoveStop); 
	Finish();

    SetNextStateId(NTL_FSMSID_ADJUSTMOVE);
	
 //   RwReal fLen = CNtlMath::GetLength(m_pActor->GetPosition(), pMoveStop->vLoc);
	//if(fLen >= 0.0f)
	//{
	//	SetNextStateId(NTL_FSMSID_ADJUSTMOVE);
	//}
	//else
	//{
	//	Behavior_AdjustPositionJumping(m_pActor, &pMoveStop->vLoc);
	//	m_pActor->SetDirection(&pMoveStop->vDir);
	//}

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobAttack(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobAttack");

	if(GetFlags() & NTL_FSMSF_NOT_ATTACK_STATE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActAttack(m_pActor, reinterpret_cast<SNtlEventSobAttack*>(pMsg.pData)); 

	Finish();
	SetNextStateId(NTL_FSMSID_FIGHTING);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobSpecialAttack(RWS::CMsg& pMsg)
{
	SNtlEventSobSpecialAttack* pSpecialAttack = reinterpret_cast<SNtlEventSobSpecialAttack*>(pMsg.pData);

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = pSpecialAttack->hDefenderSerialId;

	
	SHitStuff sHit;
	sHit.Reset();
	sHit.uiAttr.bitBlocking = DBO_GUARD_TYPE_INVALID;

	if (pSpecialAttack->byAttackResult == BATTLE_ATTACK_RESULT_HIT)
	{
		sHit.uiAttr.bitResult = NTL_ATTACK_RESULT_SUCCESS;
	}
	else if (pSpecialAttack->byAttackResult == BATTLE_ATTACK_RESULT_CRITICAL_HIT)
	{
		sHit.uiAttr.bitResult = NTL_ATTACK_RESULT_SUCCESS;
		sHit.uiAttr.bitCritical = NTL_ATTACK_ATTR_CRITICAL;
	}
	else if (pSpecialAttack->byAttackResult == BATTLE_ATTACK_RESULT_DODGE)
	{
		sHit.uiAttr.bitResult = NTL_ATTACK_RESULT_DODGE;
	}
	else if (pSpecialAttack->byAttackResult == BATTLE_ATTACK_RESULT_BLOCK)
	{
		sHit.uiAttr.bitResult = NTL_ATTACK_RESULT_BLOCK;
		sHit.uiAttr.bitBlocking = DBO_GUARD_TYPE_DEFENCE;
	}
	else if (pSpecialAttack->byAttackResult == BATTLE_ATTACK_RESULT_KNOCKDOWN)
	{
		sHit.uiAttr.bitResult = NTL_ATTACK_RESULT_KNOCKDOWN;
	}
	else if (pSpecialAttack->byAttackResult == BATTLE_ATTACK_RESULT_SLIDING)
	{
		sHit.uiAttr.bitResult = NTL_ATTACK_RESULT_SLIDING;
	}
	else if (pSpecialAttack->byAttackResult == BATTLE_ATTACK_RESULT_RESISTED)
	{
		sHit.uiAttr.bitResult = NTL_ATTACK_RESULT_RESIST;
	}
	else if (pSpecialAttack->byAttackResult == BATTLE_ATTACK_RESULT_IMMUNE)
	{
		sHit.uiAttr.bitResult = NTL_ATTACK_RESULT_IMMUNE;
	}

	if (pSpecialAttack->bySourceType == NTL_ATTACK_LOGIC_SKILL)
	{
		sHit.hAttackerSerialId = pSpecialAttack->hAttackerSerialId;
		sHit.hDefenderSerialId = pSpecialAttack->hDefenderSerialId;
		sHit.sDamageStuff.iDamage = pSpecialAttack->uiDamage;
		sHit.uiAttrLogicTblId = pSpecialAttack->sourceTblidx;
		sHit.byShotType = BONE_CHARACTER;
		sHit.bySubWeaponShotFlag = BONE_CHARACTER;
	}


	CNtlSLEventGenerator::SobHit(sHit.hDefenderSerialId, &sHit);

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobAttacked(RWS::CMsg &pMsg)
{
	SNtlEventSobAttacked* pAttacked = reinterpret_cast<SNtlEventSobAttacked*>(pMsg.pData);
	m_pActor->SetLastAttackerID(pAttacked->hAttackerSerialId);	/// last attacker 저장

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobHit(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobHit");

	SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
	FSMEvent_CharActHit(m_pActor, pHit); 

	LuaExec_Hurt(pHit->sHitStuff.hAttackerSerialId, pHit->sHitStuff.hDefenderSerialId, &pHit->sHitStuff);

	// 타겟 마킹된 상태라면 해제한다.
	if(m_pActor->GetSerialID() == Logic_GetTargetMarkingID())
	{
		CNtlSLEventGenerator::SobTargetMarkRelease(m_pActor->GetSerialID());
	}

	RwUInt8 byResult = pHit->sHitStuff.uiAttr.bitResult;
	if(byResult == NTL_ATTACK_RESULT_HEALING || byResult == NTL_ATTACK_RESULT_BUFF || byResult == NTL_ATTACK_RESULT_STEAL)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(GetFlags() & NTL_FSMSF_NOT_STATECHANGE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if( ((CNtlFSMCharActAgent*)GetFSMAgent())->GetServerState() == CHARSTATE_HTB )
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK); 
	}

	if(pHit->sHitStuff.uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_KNOCKDOWN);

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}

	if(pHit->sHitStuff.uiAttr.bitBehavior == NTL_ATTACK_SLIDING)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_SLIDING);

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}

	if(GetFlags() & NTL_FSMSF_NOT_HURT_STATE)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	Finish();
	SetNextStateId(NTL_FSMSID_HURT);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcSobSit(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobSit");

	if(GetFlags() & NTL_FSMSF_NOT_SIT)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	SNtlEventSobSit *pSobSit = reinterpret_cast<SNtlEventSobSit*>(pMsg.pData);

	if(pSobSit->bSitDown && GetFlags() & NTL_FSMSF_SKIP_SITDOWN)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	if(!pSobSit->bSitDown && GetFlags() & NTL_FSMSF_SKIP_STANDUP)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActSit(m_pActor, pSobSit);

	Finish();

	if(pSobSit->bSitDown)
		SetNextStateId(NTL_FSMSID_SITDOWN);
	else
		SetNextStateId(NTL_FSMSID_STANDUP);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobDirectPlay(RWS::CMsg &pMsg)
{
	SNtlEventSobDirectPlay *pDirectPlay = reinterpret_cast<SNtlEventSobDirectPlay*>(pMsg.pData);

	FSMEvent_CharActDirectPlay(m_pActor, pDirectPlay);

	SetNextStateId(NTL_FSMSID_DIRECTION);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobAnimPlay(RWS::CMsg &pMsg)
{
	SNtlEventSobAnimPlay *pAnimPlay = reinterpret_cast<SNtlEventSobAnimPlay*>(pMsg.pData);

	FSMEvent_CharActAnimPlay(m_pActor, pAnimPlay);

	SetNextStateId(NTL_FSMSID_DIRECTION);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobPrivateShop(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobPrivateShop");

	SNtlEventSobPrivateShop *pSobPrivateShop = reinterpret_cast<SNtlEventSobPrivateShop*>(pMsg.pData);

	FSMEvent_CharActPrivateShop(m_pActor, pSobPrivateShop);

	Finish();
	SetNextStateId(NTL_FSMSID_PRIVATESHOP);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobPrivateShopState(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobPrivateShopState");

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcSobJump(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcActionMapJump");

	if(GetFlags() & NTL_FSMSF_NOT_JUMP)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActJump(m_pActor, reinterpret_cast<SNtlEventSobJump*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_JUMP);
	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobAirJump(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobAirJump");

	FSMEvent_CharActAirJump(m_pActor, reinterpret_cast<SNtlEventSobAirJump*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_AIR_JUMP);
	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobAirAccel(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobAirAccel");

	RwBool bUpdateMove = FSMEvent_CharActAirAccel(m_pActor, reinterpret_cast<SNtlEventSobAirAccel*>(pMsg.pData));

	if (!bUpdateMove)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_PASS); // pass, so we sent packet to the server
	}

	Finish();
	SetNextStateId(NTL_FSMSID_AIR_ACCEL);
	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobJumpDirection(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobJumpDirection");

	SNtlEventSobJumpDirection *pJumpDiection = reinterpret_cast<SNtlEventSobJumpDirection*>( pMsg.pData );

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SJumpStuff *pJumpStuff = pBeData->GetJumpStuff();

	pJumpStuff->Reset();
	RwV3dAssignMacro(&pJumpStuff->vJumpDir, &pJumpDiection->vJumpDir);

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobJumpEnd(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobJumpEnd");

	NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcSobSkillCancel(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobSkillCancel");

	Finish();

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobSkillCasting(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobSkillCasting");

	FSMEvent_CharActSkillCasting(m_pActor, reinterpret_cast<SNtlEventSobSkillCasting*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_SKILL_CASTING);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobSkillAction(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobSkillAction");

	if (GetStateId() == NTL_FSMSID_DESPAWN)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	FSMEvent_CharActSkillAction(m_pActor, reinterpret_cast<SNtlEventSobSkillAction*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_SKILL_ACTION);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobSkillActioned(RWS::CMsg &pMsg)
{
	SNtlEventSobSkillActioned* pSkillActioned = reinterpret_cast<SNtlEventSobSkillActioned*>(pMsg.pData);
	m_pActor->SetLastAttackerID(pSkillActioned->hAttackerSerialId);	/// last attacker 저장

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobHTBSkillAction(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobHTBSkillAction");

	FSMEvent_CharActHTBSkillAction(m_pActor, reinterpret_cast<SNtlEventSobHTBSkillAction*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_HTB);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobSocialAction(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobSocialAction");

	FSMEvent_CharActSocialAction(m_pActor, reinterpret_cast<SNtlEventSobSocialAction*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_SOCIAL_ACTION);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobItemCancel(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobItemCancel");

	Finish();

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobItemCasting(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobItemCasting");

	FSMEvent_CharActItemCasting(m_pActor, reinterpret_cast<SNtlEventSobItemCasting*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_ITEM_CASTING);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobFainting(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobFainting");

	FSMEvent_CharActFainting(m_pActor, reinterpret_cast<SNtlEventSobFainting*>(pMsg.pData));

	Finish();
	SetNextStateId(NTL_FSMSID_FAINTING);

	NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}


RwUInt32 CNtlFSMCharActStateBase::EventProcSobWeightTime(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::EventProcSobWeightTime");

	SNtlEventWeightTime *pSobWeightTime = reinterpret_cast<SNtlEventWeightTime*>(pMsg.pData);
	SEventWeightTime *pWeightTimeData = reinterpret_cast<SEventWeightTime*>(pSobWeightTime->pData);

	GetNtlSobElapsedControlManager()->CreateController(m_pActor, pWeightTimeData->fLifeTime, pWeightTimeData->fWeightValue);

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcAfterEffectRemainTime( RWS::CMsg& pMsg ) 
{
	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcCondConfused( RWS::CMsg& pMsg ) 
{
	SNtlEventCondConfused* pData = (SNtlEventCondConfused*)pMsg.pData;
	if(pData->hSubject != m_pActor->GetSerialID())
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);

	CNtlFSMCharActAgent* pCharAgent = (CNtlFSMCharActAgent*)GetFSMAgent();

	if(pData->bAffect && pData->hTargetSerialId != INVALID_SERIAL_ID)
	{
		if(!(pCharAgent->GetServerCondition() & CHARCOND_FLAG_CONFUSED))
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);

		//pCharAgent->ConditionConfused(TRUE);

        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            CNtlSLEventGenerator::SobAttackSelect(pData->hTargetSerialId);
        }
	}
	else
	{
		//pCharAgent->ConditionConfused(FALSE);
		Finish();
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcCondTerror( RWS::CMsg& pMsg ) 
{
	SNtlEventCondTerror* pData = (SNtlEventCondTerror*)pMsg.pData;
	if(pData->hSubject != m_pActor->GetSerialID())
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);

	CNtlFSMCharActAgent* pCharAgent = (CNtlFSMCharActAgent*)GetFSMAgent();

	if(pData->bAffect)
	{
		if(!(pCharAgent->GetServerCondition() & CHARCOND_FLAG_TERROR))
			NTL_RETURN(NTL_FSM_EVENTRES_PASS);

		//pCharAgent->ConditionTerror(TRUE);

		// 1. 자신을 공격한 몹의 위치를 가져온다.
		CNtlSobActor* pActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pData->hAttackerSerialId);
		if(!pActor)
			NTL_RETURN(NTL_FSM_EVENTRES_PASS);

		// 2. 아바타의 현재 위치에서 몹과 반대쪽으로 달려간다. (최대 10m)
		RwV3d vDest = m_pActor->GetPosition() - pActor->GetPosition();
		vDest.y = 0;
		RwV3dNormalize(&vDest, &vDest);
		vDest = m_pActor->GetPosition() + vDest * DBO_MAX_TERROR_MOVEMENT_DISTANCE;        
		API_GetSLPacketGenerator()->SendMouseMove(m_pActor->GetPosition(), vDest, m_pActor->GetServerSyncAvatarType());  
		CNtlSLEventGenerator::SobDestMove(m_pActor->GetSerialID(), NTL_MOVE_FLAG_RUN, m_pActor->GetPosition(), vDest, 0);        
	}
	else
	{
		//pCharAgent->ConditionTerror(FALSE);
		Finish();        
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

RwUInt32 CNtlFSMCharActStateBase::EventProcSobPushingNfy( RWS::CMsg& pMsg ) 
{
	SNtlEventSobPushingNfy* pData = (SNtlEventSobPushingNfy*)pMsg.pData;
	if(pData->hSerialId != m_pActor->GetSerialID())
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);

	// 현재 상태를 멈추고 Pushing상태로 전이한다.
	FSMEvent_CharActHit(m_pActor, pData->byAttackResult, pData->wAttackResultValue);    // Hit Effect및 데미지 출력

	if(pData->byAttackResult == BATTLE_ATTACK_RESULT_DODGE) // 회피
	{
		sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
		RwBool bEquipedStaff = FALSE;
		if(pItemTblData)
			bEquipedStaff = Logic_IsEquipedStaffWeapon(pItemTblData);

		if(bEquipedStaff)
			m_pActor->GetSobProxy()->SetBaseAnimation(BTL_STAFF_DODGE, FALSE);
		else
			m_pActor->GetSobProxy()->SetBaseAnimation(BTL_DEF_DODGE, FALSE);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else
	{
		FSMEvent_CharActSobPushing(m_pActor, pData);

		SetNextStateId(NTL_FSMSID_PUSHING);
		Finish();

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}    
}

RwUInt32 CNtlFSMCharActStateBase::EventSobTransformSequela(RWS::CMsg& pMsg)
{
    // 아바타 전용 이벤트
    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
        NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);

    SetNextStateId(NTL_FSMSID_SEQUELA);
    Finish();

    NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
}
