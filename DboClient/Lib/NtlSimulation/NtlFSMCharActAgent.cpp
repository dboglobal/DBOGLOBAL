#include "precomp_ntlsimulation.h"
#include "NtlFSMCharActAgent.h"

// shared
#include "NtlMovement.h"
#include "NtlCharacterState.h"
#include "NtlResultCode.h"
#include "NtlCharacter.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// sound
#include "WorldSoundDefine.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLHelpers.h"
#include "NtlPLEntity.h"
#include "NtlPLCharacter.h"
#include "NtlSoundManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

#include "NtlFSMDef.h"
#include "NtlSobActor.h"
#include "NtlSobSyncStatus.h"
#include "NtlFSMStateBase.h"
#include "NtlSLLogic.h"
#include "NtlSobProxy.h"
#include "NtlSobAttr.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobManager.h"
#include "NtlSobProxy.h"
#include "NtlSobProxySystemEffect.h"
#include "NtlSobPlayer.h"


DEFINITION_MEMORY_POOL(CNtlFSMCharActAgent)

CNtlFSMCharActAgent::CNtlFSMCharActAgent()
{
	memset( &m_sCharState, 0, sizeof( m_sCharState ) );

	m_ulServerCondition = 0;
	m_bActiveAdjustPosition	= FALSE;
	m_uActMode.uiActMode	= FALSE;

	m_pConditionBleed = NULL;
	m_pConditionPoison = NULL;
	m_pConditionStomach = NULL;
	m_pConditionTaunt		= NULL;
	m_pConditionConfuse		= NULL;
	m_pConditionTerror		= NULL;
	m_pLPDangerColor		= NULL;
	m_pSystemEffectColor	= NULL;    
    m_pSystemInvisible      = NULL;
    m_pSystemGMAlpha        = NULL;
    m_pSystemHidingKi       = NULL;	

	m_byServerState = 0;
}

CNtlFSMCharActAgent::~CNtlFSMCharActAgent()
{
	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();

	if(m_pSystemEffectColor)
	{
		pSobProxy->RemoveVisualSystemEffectColorFlickering(m_pSystemEffectColor);
		m_pSystemEffectColor = NULL;
	}

	if(m_pLPDangerColor)
	{
		pSobProxy->RemoveVisualSystemEffectColorFlickering(m_pLPDangerColor);
		m_pLPDangerColor = NULL;
	}

	if(m_pConditionTaunt)
	{
		GetSceneManager()->DeleteEntity(m_pConditionTaunt);
		m_pConditionTaunt = NULL;
	}

	if (m_pConditionBleed)
	{
		GetSceneManager()->DeleteEntity(m_pConditionBleed);
		m_pConditionBleed = NULL;
	}

	if (m_pConditionPoison)
	{
		GetSceneManager()->DeleteEntity(m_pConditionPoison);
		m_pConditionPoison = NULL;
	}

	if (m_pConditionStomach)
	{
		GetSceneManager()->DeleteEntity(m_pConditionStomach);
		m_pConditionStomach = NULL;
	}

	if(m_pConditionConfuse)
	{
		GetSceneManager()->DeleteEntity(m_pConditionConfuse);
		m_pConditionConfuse = NULL;
	}

	if(m_pConditionTerror)
	{
		GetSceneManager()->DeleteEntity(m_pConditionTerror);
		m_pConditionTerror = NULL;
	}

    if(m_pSystemGMAlpha)
    {
        pSobProxy->RemoveVisualSystemEffectAlpha(m_pSystemGMAlpha);
    }
}


////////////////////////////////////////////////////////////////////////////
//  des : server state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateAnalysis(sCHARSTATE *pCharState)
{
	CopyCharState( &m_sCharState, pCharState );

	RwUInt8 byServerState = pCharState->sCharStateBase.byStateID;
	m_uActMode.bitSitDown = FALSE;

	SetAirMode(pCharState->sCharStateBase.eAirState); // has to be above SetNextStateId(NTL_FSMSID_CREATE);, so flying players wont be on the ground

	switch(byServerState)
	{
	case CHARSTATE_SPAWNING:
		SetNextStateId(NTL_FSMSID_CREATE);
		break;
	case CHARSTATE_STANDING:
		ServerStateCreateStanding(pCharState);
		break;
	case CHARSTATE_CHARGING:
		ServerStateCreateCharging(pCharState);
		break;
	case CHARSTATE_GUARD:
		ServerStateCreateBlocking(pCharState);
		break;
	case CHARSTATE_SITTING:
		ServerStateCreateSitting(pCharState);
		break;
	case CHARSTATE_MOVING:
		ServerStateCreateMoving(pCharState);
		break;
	case CHARSTATE_DESTMOVE:
		ServerStateCreateDestMove(pCharState);
		break;
	case CHARSTATE_DASH_PASSIVE:
		ServerStateCreateDash(pCharState);
		break;
	case CHARSTATE_FOLLOWING:
		ServerStateCreateFollowing(pCharState);
		break;
	case CHARSTATE_FALLING:
		ServerStateCreateFalling(pCharState);
		break;
	case CHARSTATE_TELEPORTING:
		ServerStateCreateTeleporting(pCharState);
		break;
	case CHARSTATE_SLIDING:
		ServerStateCreateSliding(pCharState);
		break;
	case CHARSTATE_KNOCKDOWN:
		ServerStateCreateKnockDown(pCharState);
		break;
	case CHARSTATE_STUNNED:
		ServerStateCreateStun(pCharState);
		break;
	case CHARSTATE_SLEEPING:
		ServerStateCreateSleep(pCharState);
		break;
	case CHARSTATE_PARALYZED:
		ServerStateCreateParalyze(pCharState);
		break;
	case CHARSTATE_FOCUSING:
		ServerStateCreateFocusing(pCharState);
		break;
	case CHARSTATE_CASTING:
		ServerStateCreateCasting(pCharState);
		break;
	case CHARSTATE_SKILL_AFFECTING:
		ServerStateCreateSkillAffecting(pCharState);
		break;
    case CHARSTATE_KEEPING_EFFECT:
        ServerStateKeepingEffect(pCharState);
        break;
	case CHARSTATE_HTB:
		ServerStateCreateHTB(pCharState);
		break;
	case CHARSTATE_SANDBAG:
		ServerStateCreateHTBSandbag(pCharState);
		break;
	case CHARSTATE_CASTING_ITEM:
		ServerStateCreateItemCasting(pCharState);
		break;
	case CHARSTATE_FAINTING:
		ServerStateCreateFainting(pCharState);
		break;
	case CHARSTATE_PRIVATESHOP:
		ServerStateCreatePrivateShop(pCharState);
		break;
	case CHARSTATE_DIRECT_PLAY:
		ServerStateCreateDirection(pCharState);
		break;
	case CHARSTATE_OPERATING:
		ServerStateCreateOperation(pCharState);
		break;
	case CHARSTATE_DESPAWNING:
		ServerStateCreateDespawn(pCharState);
		break;
	case CHARSTATE_CAMPING:
		break;
	case CHARSTATE_LEAVING:
		ServerStateCreateStanding(pCharState);
		break;
    case CHARSTATE_RIDEON:
        ServerStateCreateRideOn(pCharState);
        break;
    case CHARSTATE_TURNING:
        ServerStateCreateTurning(pCharState);
        break;
	case CHARSTATE_AIR_JUMP:
		ServerStateCreateAirJump(pCharState);
		break;
	case CHARSTATE_AIR_DASH_ACCEL:
		ServerStateCreateAirAccel(pCharState);
		break;
	default:
		{
			NTL_ASSERTFAIL("CNtlFSMCharActAgent::ServerStateCreateAnalysis => not process server state");
		}
		break;
	}

	m_byServerState = byServerState;

	CNtlMath::MathRwV3dAssign(&m_vServerPos, pCharState->sCharStateBase.vCurLoc.x, pCharState->sCharStateBase.vCurLoc.y, pCharState->sCharStateBase.vCurLoc.z);
	CNtlMath::MathRwV3dAssign(&m_vServerDir, pCharState->sCharStateBase.vCurDir.x, pCharState->sCharStateBase.vCurDir.y, pCharState->sCharStateBase.vCurDir.z);

	ServerConditionUpdateAnalysis(m_byServerState, pCharState->sCharStateBase.dwConditionFlag);
    ServerAspectUpdateAnalysis(pCharState->sCharStateBase.aspectState, TRUE);

	SetFightingMode(pCharState->sCharStateBase.bFightMode);
}



////////////////////////////////////////////////////////////////////////////
//  des : server standing state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateStanding(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_IDLE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server charging state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateCharging(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_CHARGING);
}

////////////////////////////////////////////////////////////////////////////
//  des : server blocking state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateBlocking(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_BLOCKING);
}


////////////////////////////////////////////////////////////////////////////
//  des : server sit state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateSitting(sCHARSTATE *pServerState)
{
	m_uActMode.bitSitDown = TRUE;
	SetNextStateId(NTL_FSMSID_SIT);
}

////////////////////////////////////////////////////////////////////////////
//  des : server move state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateMoving(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_MOVE);

	// behavior data setting
	sCHARSTATE_MOVING *pMoving = &pServerState->sCharStateDetail.sCharStateMoving;

	if (pMoving->byMoveDirection == NTL_MOVE_NONE)
	{
		DBO_WARNING_MESSAGE("pMoving->byMoveDirection == NTL_MOVE_NONE. ClassID: " << m_pActor->GetClassID());

		pMoving->byMoveDirection = NTL_MOVE_F;
	}

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_DIR;

	if(pServerState->sCharStateBase.eAirState == AIR_STATE_ON)
		pMoveStuff->byForm = NTL_MOVEFORM_FLY;
	else
		pMoveStuff->byForm = NTL_MOVEFORM_GROUND;

	pMoveStuff->byFormFlag = pMoving->byMoveFlag;
	pMoveStuff->byMoveFlags = pMoving->byMoveDirection;
	
	// In the case of the jump flag, this is changed.
	if(pMoveStuff->byMoveFlags == NTL_MOVE_F_TURN_L_JUMP || pMoveStuff->byMoveFlags == NTL_MOVE_F_TURN_R_JUMP)
		pMoveStuff->byMoveFlags = NTL_MOVE_F;
	else if(pMoveStuff->byMoveFlags == NTL_MOVE_B_TURN_L_JUMP || pMoveStuff->byMoveFlags == NTL_MOVE_B_TURN_R_JUMP)
		pMoveStuff->byMoveFlags = NTL_MOVE_B;
	else if (pMoveStuff->byMoveFlags == NTL_MOVE_L_TURN_L_JUMP || pMoveStuff->byMoveFlags == NTL_MOVE_L_TURN_R_JUMP)
		pMoveStuff->byMoveFlags = NTL_MOVE_L;
	else if (pMoveStuff->byMoveFlags == NTL_MOVE_R_TURN_L_JUMP || pMoveStuff->byMoveFlags == NTL_MOVE_R_TURN_R_JUMP)
		pMoveStuff->byMoveFlags = NTL_MOVE_R;

	pMoveStuff->vCurrLoc.x = pServerState->sCharStateBase.vCurLoc.x;
	pMoveStuff->vCurrLoc.y = pServerState->sCharStateBase.vCurLoc.y;
	pMoveStuff->vCurrLoc.z = pServerState->sCharStateBase.vCurLoc.z;
}

////////////////////////////////////////////////////////////////////////////
//  des : server destmove state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateDestMove(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_MOVE);

	// behavior data setting

	sCHARSTATE_DESTMOVE *pDestMove = &pServerState->sCharStateDetail.sCharStateDestMove;

	// Dest location counter는 반드시 1 이상이어야 한다
	if ( 0 == pDestMove->byDestLocCount )
	{
		DBO_WARNING_MESSAGE( "A dest location counter must be one more. ClassID: " << m_pActor->GetClassID());

		SetNextStateId( NTL_FSMSID_IDLE );

		return;
	}

	// Dest location counter는 반드시 DBO_MAX_NEXT_DEST_LOC_COUNT 보다 작거나 같아야 한다
	if ( pDestMove->byDestLocCount > DBO_MAX_NEXT_DEST_LOC_COUNT )
	{
		DBO_WARNING_MESSAGE( "A dest location counter must be less equal than DBO_MAX_NEXT_DEST_LOC_COUNT. ClassID: " << m_pActor->GetClassID() );

		SetNextStateId( NTL_FSMSID_IDLE );

		return;
	}

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_LOC;

	if ( Logic_IsBus(m_pActor) )
	{
		pMoveStuff->byForm = NTL_MOVEFORM_BUS;
	}
	else
	{
		if (m_pActor->IsAirMode())
			pMoveStuff->byForm = NTL_MOVEFORM_FLY;
		else
			pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	}	

	pMoveStuff->byFormFlag = pDestMove->byMoveFlag;

	pMoveStuff->vCurrLoc.x = pServerState->sCharStateBase.vCurLoc.x;
	pMoveStuff->vCurrLoc.y = pServerState->sCharStateBase.vCurLoc.y;
	pMoveStuff->vCurrLoc.z = pServerState->sCharStateBase.vCurLoc.z;

	pMoveStuff->vDest.x = pDestMove->avDestLoc[0].x;
	pMoveStuff->vDest.y = pDestMove->avDestLoc[0].y;
	pMoveStuff->vDest.z = pDestMove->avDestLoc[0].z;

	if ( pDestMove->bHaveSecondDestLoc )
	{
		pMoveStuff->vSendDest.x = pDestMove->vSecondDestLoc.x;
		pMoveStuff->vSendDest.y = pDestMove->vSecondDestLoc.y;
		pMoveStuff->vSendDest.z = pDestMove->vSecondDestLoc.z;
	}
	else
	{
		pMoveStuff->vSendDest = pMoveStuff->vDest;
	}

	pMoveStuff->byPathNodeNum = pDestMove->byDestLocCount;

	for ( int i = 0; i < pDestMove->byDestLocCount; ++i )
	{
		pMoveStuff->arrPathNode[i].x = pDestMove->avDestLoc[i].x;
		pMoveStuff->arrPathNode[i].y = pDestMove->avDestLoc[i].y;
		pMoveStuff->arrPathNode[i].z = pDestMove->avDestLoc[i].z;
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : server follow state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateFollowing(sCHARSTATE *pServerState)
{
	sCHARSTATE_FOLLOWING *pFollow = &pServerState->sCharStateDetail.sCharStateFollwing;

	if(pFollow->byMovementReason == DBO_MOVE_FOLLOW_SKILL_ATTACK)
	{
		SetNextStateId(NTL_FSMSID_SKILL_FOLLOW);
	}
	else if(pFollow->byMovementReason == DBO_MOVE_FOLLOW_AUTO_ATTACK)
	{
		SetNextStateId(NTL_FSMSID_FIGHTING_FOLLOW);
	}
	else
	{
		SetNextStateId(NTL_FSMSID_FOLLOW);
	}
	
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;

	if (m_pActor->IsAirMode())
	{
		pMoveStuff->byForm = NTL_MOVEFORM_FLY;
	}
	else
	{
		pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	}

	pMoveStuff->byFormFlag = pFollow->byMoveFlag;
	pMoveStuff->hFollowSerialId = pFollow->hTarget;
	pMoveStuff->fFollowRange = pFollow->fDistance;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = pFollow->hTarget;
	pCtrlStuff->fFollowRange = pFollow->fDistance;
}

////////////////////////////////////////////////////////////////////////////
//  des : server falling state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateFalling(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_IDLE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server follow state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateDash(sCHARSTATE *pServerState)
{
	sCHARSTATE_DASH_PASSIVE *pDash = &pServerState->sCharStateDetail.sCharStateDashPassive;

	SetNextStateId(NTL_FSMSID_DASH);

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_DASH;

	if (m_pActor->IsAirMode())
	{
		pMoveStuff->byForm = NTL_MOVEFORM_FLY;
		pMoveStuff->byFormFlag = NTL_MOVE_FLAG_FLY_DASH;
	}
	else
	{
		pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
		pMoveStuff->byFormFlag = NTL_MOVE_FLAG_DASH;
	}

	pMoveStuff->byMoveFlags = pDash->byMoveDirection;

	RwV3d vPos = m_pActor->GetPosition();

	RwV3d vDir;
	RwV3d vDest;
	CNtlMath::MathRwV3dAssign(&vDest, pDash->vDestLoc.x, pDash->vDestLoc.y, pDash->vDestLoc.z);
	RwV3dSubMacro(&vDir, &vDest, &vPos);
	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);

	RwV3dAssignMacro(&pMoveStuff->vDir, &vDir);  
	RwV3dAssignMacro(&pMoveStuff->vDest, &vDest);

	RwUInt32 uiMoveFlags = pDash->byMoveDirection;

	if(uiMoveFlags == NTL_MOVE_F)
	{
	}
	else if(uiMoveFlags == NTL_MOVE_B)
	{
		RwV3dScale(&vDir, &vDir, -1.0f);
	}
	else if(uiMoveFlags == NTL_MOVE_L)
	{
		RwV3d vHeading;
		RwV3dCrossProduct(&vHeading, &vDir, &CNtlPLGlobal::m_vYAxisV3);
		RwV3dAssignMacro(&vDir, &vHeading);
	}
	else
	{
		RwV3d vHeading;
		RwV3dCrossProduct(&vHeading, &vDir, &CNtlPLGlobal::m_vYAxisV3);
		RwV3dAssignMacro(&vDir, &vHeading);
		RwV3dScale(&vDir, &vDir, -1.0f);
	}

	m_pActor->SetDirection(&vDir);

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
}

////////////////////////////////////////////////////////////////////////////
//  des : server teleport state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateTeleporting(sCHARSTATE *pServerState)
{
	sCHARSTATE_TELEPORTING *pTeleport = &pServerState->sCharStateDetail.sCharStateTeleporting;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sTeleport.byTeleportType = pTeleport->byTeleportType;

	if(pTeleport->byTeleportType == TELEPORT_TYPE_GAME_IN)
	{
		if(m_pActor->GetClassID() == SLCLASS_AVATAR)
		{
			SetNextStateId(NTL_FSMSID_CREATE);
			return;
		}
	}

	SetNextStateId(NTL_FSMSID_TELEPORT);
}

////////////////////////////////////////////////////////////////////////////
//  des : server sliding state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateSliding(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_IDLE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server stun state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateStun(sCHARSTATE *pServerState)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    pBeData->GetCtrlStuff()->uExtra.sStun.byStunType = pServerState->sCharStateDetail.sCharStateStunned.byStunType;

	SetNextStateId(NTL_FSMSID_STUN);
}

////////////////////////////////////////////////////////////////////////////
//  des : server sleep state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateSleep(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_SLEEP);
}

////////////////////////////////////////////////////////////////////////////
//  des : server paralyze state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateParalyze(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_PARALYZE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server knockdown state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateKnockDown(sCHARSTATE *pServerState)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SHitStuff *pHitStuff = pBeData->GetHitStuff();
	pHitStuff->vPushDir.x = pServerState->sCharStateDetail.sCharStateKnockdown.vShift.x;
	pHitStuff->vPushDir.y = pServerState->sCharStateDetail.sCharStateKnockdown.vShift.y;
	pHitStuff->vPushDir.z = pServerState->sCharStateDetail.sCharStateKnockdown.vShift.z;

	SetNextStateId(NTL_FSMSID_KNOCKDOWN);
}


////////////////////////////////////////////////////////////////////////////
//  des : server focusing state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateFocusing(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_SKILL_FOCUS);
}

////////////////////////////////////////////////////////////////////////////
//  des : server casting state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateCasting(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_SKILL_CASTING);

	// behavior data setting
	sCHARSTATE_CASTING *pCasting	= &pServerState->sCharStateDetail.sCharStateCasting;

	CNtlBeCharData *pBeData			= reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SSkillStuff *pSkillStuff		= pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->hTargetSerialId	= pCasting->hTarget;
    m_pActor->SetTargetID(pCasting->hTarget);
	pSkillStuff->uiSkillTblId		= pCasting->skillId;
	pSkillStuff->fCurrCastingTime	= (RwReal)(pCasting->dwCastingTime-pCasting->dwCastingTimeRemaining)/1000.f;
	pSkillStuff->fMaxCastingTime	= (RwReal)pCasting->dwCastingTime/1000.f;

	if(pSkillStuff->fCurrCastingTime > pSkillStuff->fMaxCastingTime)
		pSkillStuff->fCurrCastingTime = pSkillStuff->fMaxCastingTime;

	if(pSkillStuff->fCurrCastingTime < 0.0f)
		pSkillStuff->fCurrCastingTime = 0.0f;
}

////////////////////////////////////////////////////////////////////////////
//  des : server skill affecting state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateSkillAffecting(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_IDLE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server HTB state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateHTB(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_HTB);

	sCHARSTATE_HTB *pHTB = &pServerState->sCharStateDetail.sCharStateHTB;

	// skill data setting
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
	pHTBStuff->Reset();

	pHTBStuff->hAttackSerialId = m_pActor->GetSerialID();
	pHTBStuff->hTargetSerialId = pHTB->hTarget;
	pHTBStuff->uiSkillTblId = pHTB->HTBId;
	pHTBStuff->byCurrStep = pHTB->byCurStep;
	pHTBStuff->byStepCount = pHTB->byStepCount;
	pHTBStuff->byResultCount = pHTB->byResultCount;
	memcpy(pHTBStuff->sResult, pHTB->aHTBSkillResult, sizeof(sHTBSKILL_RESULT)*pHTB->byResultCount);

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = pHTB->hTarget;
}

////////////////////////////////////////////////////////////////////////////
//  des : server HTB Sandbag state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateHTBSandbag(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_HTB_SANDBAG);
}

////////////////////////////////////////////////////////////////////////////
//  des : server Item casting state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateItemCasting(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_ITEM_CASTING);

	// behavior data setting
	sCHARSTATE_CASTING_ITEM *pItemCasting = &pServerState->sCharStateDetail.sCharStateCastingItem;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->hTargetSerialId = pItemCasting->hTarget;
	pSkillStuff->uiSkillTblId= pItemCasting->itemTblidx;

	pSkillStuff->fCurrCastingTime	= (RwReal)(pItemCasting->dwCastingTime-pItemCasting->dwCastingTimeRemaining)/1000.f;
	pSkillStuff->fMaxCastingTime	= (RwReal)pItemCasting->dwCastingTime/1000.f;

	if(pSkillStuff->fCurrCastingTime > pSkillStuff->fMaxCastingTime)
		pSkillStuff->fCurrCastingTime = pSkillStuff->fMaxCastingTime;

	if(pSkillStuff->fCurrCastingTime < 0.0f)
		pSkillStuff->fCurrCastingTime = 0.0f;
}

////////////////////////////////////////////////////////////////////////////
//  des : server fainting state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateFainting(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_DIE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server private shop state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreatePrivateShop(sCHARSTATE *pServerState)
{
	sCHARSTATE_PRIVATESHOP *pPrivateShop = &pServerState->sCharStateDetail.sCharStatePrivateShop;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	SCtrlStuffPrivateShop *pPrivateShopStuff = &pCtrlStuff->uExtra.sPrivateShop;
		
 	pPrivateShopStuff->bShopOwnerEmpty		= pPrivateShop->sSummaryPrivateShopData.bIsOwnerEmpty;
	pPrivateShopStuff->byShopState			= pPrivateShop->sSummaryPrivateShopData.byShopState;
	memcpy(pPrivateShopStuff->wchShopName, pPrivateShop->sSummaryPrivateShopData.wcPrivateShopName, sizeof(WCHAR)*(NTL_MAX_PRIVATESHOP_NAME-1));
	pPrivateShopStuff->wchShopName[NTL_MAX_PRIVATESHOP_NAME-1] = 0;

	CNtlSLEventGenerator::PrivateShopNameTag(m_pActor->GetSerialID(), 
											pPrivateShopStuff->bShopOwnerEmpty,
											pPrivateShopStuff->byShopState,
											pPrivateShopStuff->wchShopName);

	SetNextStateId(NTL_FSMSID_PRIVATESHOP);
}

////////////////////////////////////////////////////////////////////////////
//  des : server direction state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateDirection(sCHARSTATE *pServerState)
{
	sCHARSTATE_DIRECT_PLAY *pDirectPlay = &pServerState->sCharStateDetail.sCharStateDirectPlay;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sDirection.bAnimUse		= FALSE;
	pCtrlStuff->uExtra.sDirection.byDirectType	= pDirectPlay->byDirectPlayType;
	pCtrlStuff->uExtra.sDirection.uiTblId		= pDirectPlay->directTblidx;
	pCtrlStuff->uExtra.sDirection.bTimeLoop		= FALSE;

	SetNextStateId(NTL_FSMSID_DIRECTION);
}

////////////////////////////////////////////////////////////////////////////
//  des : server direction state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateOperation(sCHARSTATE *pServerState)
{
	sCHARSTATE_OPERATING *pOperation = &pServerState->sCharStateDetail.sCharStateOperating;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->hTargetSerialId					= pOperation->hTargetObject;
	pCtrlStuff->uExtra.sOperate.uiOperateTime	= pOperation->dwOperateTime;
	pCtrlStuff->uExtra.sOperate.uiTblId			= pOperation->directTblidx;

	SetNextStateId(NTL_FSMSID_TRIGGER_OPERATE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server despawn state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateCreateDespawn(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_DESPAWN);
}

////////////////////////////////////////////////////////////////////////////
//  des : server camping state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateCamping(sCHARSTATE *pServerState)
{

}

////////////////////////////////////////////////////////////////////////////
//  des : server leaving state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCreateLeaving(sCHARSTATE *pServerState)
{
	SetNextStateId(NTL_FSMSID_LEAVING);
}


////////////////////////////////////////////////////////////////////////////
//  des : server RideOn state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateCreateRideOn( sCHARSTATE *pServerState ) 
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
    SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
    pCtrlStuff->sRide.hTargetSerialId = pServerState->sCharStateDetail.sCharStateRideOn.hTarget;

    m_pActor->EnableVisible(FALSE);

    SetNextStateId(NTL_FSMSID_ONBUS);
}

////////////////////////////////////////////////////////////////////////////
//  des : server Turning state create 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server char state data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateCreateTurning( sCHARSTATE *pServerState ) 
{
    RwV3d vDestDir;
    vDestDir.x = pServerState->sCharStateDetail.sCharStateTurning.vDestDir.x;
    vDestDir.y = pServerState->sCharStateDetail.sCharStateTurning.vDestDir.y;
    vDestDir.z = pServerState->sCharStateDetail.sCharStateTurning.vDestDir.z;

    // 터닝 상태로 들어오면 방향을 끝난후의 방향으로 설정하고 Idle 상태로 보낸다.    
    m_pActor->SetDirection(&vDestDir);   
    SetNextStateId(NTL_FSMSID_IDLE);
}

void CNtlFSMCharActAgent::ServerStateCreateAirJump(sCHARSTATE * pServerState)
{
	SetNextStateId(NTL_FSMSID_AIR_JUMP);

	// behavior data setting
	sCHARSTATE_AIRJUMPING *pMoving = &pServerState->sCharStateDetail.sCharStateAirJumping;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_AIR_DASH;
	pMoveStuff->byForm = NTL_MOVEFORM_FLY;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_FLY_DASH;
	pMoveStuff->byMoveFlags = pMoving->byMoveDirection;

	pMoveStuff->vCurrLoc.x = pServerState->sCharStateBase.vCurLoc.x;
	pMoveStuff->vCurrLoc.y = pServerState->sCharStateBase.vCurLoc.y;
	pMoveStuff->vCurrLoc.z = pServerState->sCharStateBase.vCurLoc.z;
}

void CNtlFSMCharActAgent::ServerStateCreateAirAccel(sCHARSTATE * pServerState)
{
	SetNextStateId(NTL_FSMSID_AIR_ACCEL);

	// behavior data setting
	sCHARSTATE_AIRACCEL *pMoving = &pServerState->sCharStateDetail.sCharStateAirAccel;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_DIR;
	pMoveStuff->byForm = NTL_MOVEFORM_FLY;
	pMoveStuff->byFormFlag = pMoving->byMoveFlag;
	pMoveStuff->byMoveFlags = pMoving->byMoveDirection;

	pMoveStuff->vCurrLoc.x = pServerState->sCharStateBase.vCurLoc.x;
	pMoveStuff->vCurrLoc.y = pServerState->sCharStateBase.vCurLoc.y;
	pMoveStuff->vCurrLoc.z = pServerState->sCharStateBase.vCurLoc.z;
}

////////////////////////////////////////////////////////////////////////////
//  des : 
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : 
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerAspectCreateStateVehicle(sASPECTSTATE *pServerAspectState)
{
	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->sVehicle.bCreateUpdate = true;

	if ( ASPECTSTATE_INVALID != pServerAspectState->sAspectStateBase.byAspectStateId )
	{
		pCtrlStuff->sVehicle.bRideOn = true;
	}
	else
	{
		pCtrlStuff->sVehicle.bRideOn = false;
	}

	SetNextStateId( NTL_FSMSID_RIDEONOFF );
}

////////////////////////////////////////////////////////////////////////////
//  des : 
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : 
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerAspectStateVehicle(sASPECTSTATE *pServerAspectState)
{
	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->sVehicle.bCreateUpdate = false;

	if ( ASPECTSTATE_INVALID != pServerAspectState->sAspectStateBase.byAspectStateId )
	{
		pCtrlStuff->sVehicle.bRideOn = true;
	}
	else
	{
		pCtrlStuff->sVehicle.bRideOn = false;
	}

//	pCtrlStuff->sVehicle.bEngineOn = pServerAspectState->sAspectStateDetail.sVehicle.bIsEngineOn;

	CNtlSLEventGenerator::SobStateTransition( m_pActor->GetSerialID(), NTL_FSMSID_RIDEONOFF );
}

////////////////////////////////////////////////////////////////////////////
//  des : server state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateUpdateAnalysis(SNtlEventSobServerUpdateState *pServerState)
{
	CopyCharState( &m_sCharState, pServerState->pState );

	sCHARSTATE *pCharState = pServerState->pState;
	RwUInt8 byServerState = pCharState->sCharStateBase.byStateID;

	m_uActMode.bitSitDown = FALSE;
	m_bActiveAdjustPosition = FALSE;

	switch(byServerState)
	{
	case CHARSTATE_SPAWNING:
		ServerStateSpawn(pCharState);
		break;
	case CHARSTATE_STANDING:
		ServerStateStanding(pCharState);
		break;
	case CHARSTATE_CHARGING:
		ServerStateCharging(pCharState);
		break;
	case CHARSTATE_GUARD:
		ServerStateBlocking(pCharState);
		break;
	case CHARSTATE_SITTING:
		ServerStateSitting(pCharState);
		break;
	case CHARSTATE_MOVING:
		ServerStateMoving(pCharState);
		break;
	case CHARSTATE_DESTMOVE:
		ServerStateDestMove(pCharState);
		break;
	case CHARSTATE_FOLLOWING:
		ServerStateFollowing(pCharState);
		break;
	case CHARSTATE_FALLING:
		ServerStateFalling(pCharState);
		break;
	case CHARSTATE_DASH_PASSIVE:
		ServerStateDash(pCharState);
		break;
	case CHARSTATE_TELEPORTING:
		ServerStateTeleporting(pCharState);
		break;
	case CHARSTATE_SLIDING:
		ServerStateSliding(pCharState);
		break;
	case CHARSTATE_KNOCKDOWN:
		ServerStateKnockDown(pCharState);
		break;
	case CHARSTATE_STUNNED:
		ServerStateStun(pCharState);
		break;
	case CHARSTATE_SLEEPING:
		ServerStateSleep(pCharState);
		break;
	case CHARSTATE_PARALYZED:
		ServerStateParalyze(pCharState);
		break;
	case CHARSTATE_FOCUSING:
		ServerStateFocusing(pCharState);
		break;
	case CHARSTATE_CASTING:
		ServerStateCasting(pCharState);
		break;
    case CHARSTATE_KEEPING_EFFECT:
        ServerStateKeepingEffect(pCharState);
        break;
	case CHARSTATE_HTB:
		ServerStateHTB(pCharState);
		break;
	case CHARSTATE_SANDBAG:
		ServerStateHTBSandbag(pCharState);
		break;
	case CHARSTATE_CASTING_ITEM:
		ServerStateItemCasting(pCharState);
		break;
	case CHARSTATE_FAINTING:
		ServerStateFainting(pCharState);
		break;
	case CHARSTATE_PRIVATESHOP:
		ServerStatePrivateShop(pCharState);
		break;
	case CHARSTATE_DIRECT_PLAY:
		ServerStateDirection(pCharState);
		break;
	case CHARSTATE_OPERATING:
		ServerStateOperation(pCharState);
		break;
	case CHARSTATE_DESPAWNING:
		ServerStateDespawn(pCharState);
		break;
	case CHARSTATE_CAMPING:
		ServerStateCamping(pCharState);
		break;
	case CHARSTATE_LEAVING:
		ServerStateLeaving(pCharState);
		break;
    case CHARSTATE_RIDEON:
        ServerStateRideOn(pCharState);
        break;
    case CHARSTATE_TURNING:
        ServerStateTurning(pCharState);
        break;

	case CHARSTATE_AIR_JUMP:
		ServerStateAirJump(pCharState);
		break;
	case CHARSTATE_AIR_DASH_ACCEL:
		ServerStateAirDashAccel(pCharState);
		break;
	}

	m_byServerState = byServerState;

	CNtlMath::MathRwV3dAssign(&m_vServerPos, pCharState->sCharStateBase.vCurLoc.x, pCharState->sCharStateBase.vCurLoc.y, pCharState->sCharStateBase.vCurLoc.z);
	CNtlMath::MathRwV3dAssign(&m_vServerDir, pCharState->sCharStateBase.vCurDir.x, pCharState->sCharStateBase.vCurDir.y, pCharState->sCharStateBase.vCurDir.z);
	
	ServerConditionUpdateAnalysis(m_byServerState, pCharState->sCharStateBase.dwConditionFlag);

	SetFightingMode(pCharState->sCharStateBase.bFightMode);
	SetAirMode(pCharState->sCharStateBase.eAirState);
}


////////////////////////////////////////////////////////////////////////////
//  des : server state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server condition update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerConditionUpdateAnalysis(RwUInt8 byServerState, RwUInt64 qwCondition)
{
	m_sCharState.sCharStateBase.dwConditionFlag = qwCondition;

	ConditionVisualColorFlickering(qwCondition);
	ConditionDiff(qwCondition);

	if(qwCondition & CHARCOND_FLAG_CLICK_DISABLE)
		SetClickDisable(TRUE);
	else
		SetClickDisable(FALSE);

	if (qwCondition & CHARCOND_FLAG_CLIENT_UI_DISABLE)
	{
		SetClientUIDisable(TRUE);
	}
	else
		SetClientUIDisable(FALSE);

	if(qwCondition & CHARCOND_FLAG_ATTACK_DISALLOW)
		SetAttackDisallow(TRUE);
	else 
		SetAttackDisallow(FALSE);

    // 혼란이나 공포일때는 이미 DirectMode가 적용되어 있다.
    if(!(qwCondition & CHARCOND_FLAG_CONFUSED || qwCondition & CHARCOND_FLAG_TERROR))
    {    
	    if(byServerState == CHARSTATE_DIRECT_PLAY)
		    SetDirectMode(TRUE);
	    else
		    SetDirectMode(FALSE);
    }
}

// Server로부터 전송받은 Aspect 분석 처리 함수
void CNtlFSMCharActAgent::ServerAspectUpdateAnalysis(sASPECTSTATE& sAspectState, RwBool bCreateUpdate)
{
    if ( m_pActor->GetClassID() != SLCLASS_AVATAR &&
		 m_pActor->GetClassID() != SLCLASS_PLAYER &&
		 m_pActor->GetClassID() != SLCLASS_VEHICLE )
        return;

	sASPECTSTATE sOldAspectState;
	memcpy( &sOldAspectState, &m_sCharState.sCharStateBase.aspectState, sizeof( sASPECTSTATE ) );

	m_sCharState.sCharStateBase.aspectState = sAspectState;

	if ( m_pActor->GetClassID() == SLCLASS_AVATAR ||
		 m_pActor->GetClassID() == SLCLASS_PLAYER )
	{
		((CNtlSobPlayer*)m_pActor)->SetApsectState( sAspectState );
	}

	switch ( sAspectState.sAspectStateBase.byAspectStateId )
	{
		case ASPECTSTATE_SUPER_SAIYAN:
		case ASPECTSTATE_PURE_MAJIN:
        {
            CNtlPLEntity* pEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_PURE_MAJIN_CHANGE, NULL);
            pEffect->SetPosition(&m_pActor->GetPosition());
		}
		case ASPECTSTATE_GREAT_NAMEK:
		case ASPECTSTATE_KAIOKEN:
		{
			// 아바타는 변신을 스스로 처리한다. (회전공격 예외)
			if ( m_pActor->GetClassID() == SLCLASS_AVATAR )
			{
				return;
			}

			CNtlSLEventGenerator::SobTransform( m_pActor->GetSerialID(), sAspectState );
		}
		break;

		case ASPECTSTATE_SPINNING_ATTACK:
		{
			// Spinning attacks are treated as State.
			CNtlSLEventGenerator::SobStateTransition( m_pActor->GetSerialID(), NTL_FSMSID_SPIN_ATTACK );
		}
		break;

		case ASPECTSTATE_VEHICLE:
		{
			if ( bCreateUpdate )
			{
				ServerAspectCreateStateVehicle( &sAspectState );
			}
			else
			{
				ServerAspectStateVehicle( &sAspectState );
			}
		}
		break;

		case ASPECTSTATE_ROLLING_ATTACK:
		{
			// Rolling attacks are treated as State.
			CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_ROLLING_ATTACK);
		}
		break;

		case ASPECTSTATE_INVALID:
		{
			m_pActor->GetSobProxy()->SetDisableAniChange(false);

			// 
			if (Logic_GetActorStateId(m_pActor) == NTL_FSMSID_SPIN_ATTACK || Logic_GetActorStateId(m_pActor) == NTL_FSMSID_ROLLING_ATTACK)
			{
				CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_IDLE);
			}
			else
			{
				// Vehicle 해제
				if ( sOldAspectState.sAspectStateBase.byAspectStateId == ASPECTSTATE_VEHICLE )
				{
					if ( bCreateUpdate )
					{
						ServerAspectCreateStateVehicle( &sAspectState );
					}
					else
					{
						ServerAspectStateVehicle( &sAspectState );
					}
				}
				// Disable Other Transformation
				else
				{
					CNtlSLEventGenerator::SobTransform( m_pActor->GetSerialID(), sAspectState );
				}
			}
		}
		break;

		default:
		{
			char buf[1024];
			sprintf_s( buf, 1024, "Aspect state is wrong. [%d]", sAspectState.sAspectStateBase.byAspectStateId );
			NTL_ASSERTFAIL( buf );
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : server spqwn state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateSpawn(sCHARSTATE *pServerState)
{
	sCHARSTATE_SPAWNING *pSpawn = &pServerState->sCharStateDetail.sCharStateSpawning;
	sCHARSTATE_BASE *pBase = &pServerState->sCharStateBase;

	RwV3d vLoc, vDir;
	CNtlMath::MathRwV3dAssign(&vLoc, pBase->vCurLoc.x, pBase->vCurLoc.y, pBase->vCurLoc.z);
	CNtlMath::MathRwV3dAssign(&vDir, pBase->vCurDir.x, pBase->vCurDir.y, pBase->vCurDir.z);

	m_pActor->SetPosition(&vLoc);
	m_pActor->SetDirection(&vDir);

	SetAirMode(pBase->eAirState); // set air-mode before we call SobStateTransition 

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sTeleport.byTeleportType = pSpawn->byTeleportType;

	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_SPAWN);
}


////////////////////////////////////////////////////////////////////////////
//  des : server standing state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateStanding(sCHARSTATE *pServerState)
{
	sCHARSTATE_BASE *pBase = &pServerState->sCharStateBase;

	RwV3d vLoc, vDir;
	CNtlMath::MathRwV3dAssign(&vLoc, pBase->vCurLoc.x, pBase->vCurLoc.y, pBase->vCurLoc.z);
	CNtlMath::MathRwV3dAssign(&vDir, pBase->vCurDir.x, pBase->vCurDir.y, pBase->vCurDir.z);    

	if(m_byServerState == CHARSTATE_SITTING)
	{
		CNtlSLEventGenerator::SobSit(m_pActor->GetSerialID(), FALSE);
	}
	else if(m_byServerState == CHARSTATE_MOVING || m_byServerState == CHARSTATE_DESTMOVE ||
		m_byServerState == CHARSTATE_FOLLOWING || m_byServerState == CHARSTATE_STANDING ||
		m_byServerState == CHARSTATE_AIR_JUMP || m_byServerState == CHARSTATE_AIR_DASH_ACCEL)
	{
		CNtlSLEventGenerator::SobMoveStop(m_pActor->GetSerialID(), vLoc, vDir, 0);
	}
	else if(m_byServerState == CHARSTATE_KNOCKDOWN || m_byServerState == CHARSTATE_SLIDING ||
		m_byServerState == CHARSTATE_STUNNED || m_byServerState == CHARSTATE_SLEEPING ||
		m_byServerState == CHARSTATE_PARALYZED || m_byServerState == CHARSTATE_HTB ||
		m_byServerState == CHARSTATE_GUARD || m_byServerState == CHARSTATE_SANDBAG ||
		m_byServerState == CHARSTATE_SKILL_AFFECTING)
	{
		//Behavior_AdjustPositionJumping(m_pActor, &vLoc);
		CNtlSLEventGenerator::SobStanding(m_pActor->GetSerialID(), vLoc, vDir);
	}
	else if(m_byServerState == CHARSTATE_TELEPORTING)
	{
		Behavior_AdjustPositionJumping(m_pActor, &vLoc);
		CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_CREATE);
	}
	else if(m_byServerState == CHARSTATE_CASTING)
	{
	//	CNtlSLEventGenerator::SobSkillCancel(m_pActor->GetSerialID(), 
	}
	else if(m_byServerState == CHARSTATE_PRIVATESHOP || m_byServerState == CHARSTATE_OPERATING ||
		m_byServerState == CHARSTATE_DIRECT_PLAY || m_byServerState == CHARSTATE_RIDEON ||
		m_byServerState == CHARSTATE_TURNING || m_byServerState == CHARSTATE_KEEPING_EFFECT)
	{
		CNtlSLEventGenerator::SobStanding(m_pActor->GetSerialID(), vLoc, vDir);
	}
    else if(m_byServerState == CHARSTATE_DASH_PASSIVE)
    {
        // In the dash state, you do not do anything until the end of the dash.
    }
    else if(m_byServerState == CHARSTATE_CHARGING)
    {
        CNtlSLEventGenerator::SobStanding(m_pActor->GetSerialID(), vLoc, vDir);
    }
	else // CHARSTATE_SPAWNING CHARSTATE_DESPAWNING CHARSTATE_FAINTING CHARSTATE_CAMPING CHARSTATE_LEAVING CHARSTATE_FALLING CHARSTATE_FOCUSING CHARSTATE_CASTING_ITEM
	{
		if (m_byServerState == CHARSTATE_FALLING)
		{
			m_pActor->SetAngleX(0); // reset angle
			SetAirMode(AIR_STATE_OFF);
		}

		Behavior_AdjustPositionJumping(m_pActor, &vLoc);
	}

	m_bActiveAdjustPosition = TRUE;
}


////////////////////////////////////////////////////////////////////////////
//  des : server charging state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCharging(sCHARSTATE *pServerState)
{
	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_CHARGING);
}

////////////////////////////////////////////////////////////////////////////
//  des : server blocking state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateBlocking(sCHARSTATE *pServerState)
{
	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_BLOCKING);
}


////////////////////////////////////////////////////////////////////////////
//  des : server sit state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateSitting(sCHARSTATE *pServerState)
{
	m_uActMode.bitSitDown = TRUE;
	CNtlSLEventGenerator::SobSit(m_pActor->GetSerialID(), TRUE);
}


////////////////////////////////////////////////////////////////////////////
//  des : server move state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateMoving(sCHARSTATE *pServerState)
{
	sCHARSTATE_BASE *pBase = &pServerState->sCharStateBase;
	sCHARSTATE_MOVING *pMoving = &pServerState->sCharStateDetail.sCharStateMoving;
	if(pMoving->byMoveDirection == NTL_MOVE_NONE)
	{
		DBO_WARNING_MESSAGE("pMoving->byMoveDirection == NTL_MOVE_NONE. ClassID: " << m_pActor->GetClassID());

		pMoving->byMoveDirection = NTL_MOVE_F;
	}

	RwV3d vLoc, vDir;
	CNtlMath::MathRwV3dAssign(&vLoc, pBase->vCurLoc.x, pBase->vCurLoc.y, pBase->vCurLoc.z);
	CNtlMath::MathRwV3dAssign(&vDir, pBase->vCurDir.x, pBase->vCurDir.y, pBase->vCurDir.z);

	BYTE byMoveFlag = pMoving->byMoveDirection;

	// In the case of the jump flag, this is changed.
	if (byMoveFlag == NTL_MOVE_F_TURN_L_JUMP || byMoveFlag == NTL_MOVE_F_TURN_R_JUMP)
		byMoveFlag = NTL_MOVE_F;
	else if (byMoveFlag == NTL_MOVE_B_TURN_L_JUMP || byMoveFlag == NTL_MOVE_B_TURN_R_JUMP)
		byMoveFlag = NTL_MOVE_B;
	else if (byMoveFlag == NTL_MOVE_L_TURN_L_JUMP || byMoveFlag == NTL_MOVE_L_TURN_R_JUMP)
		byMoveFlag = NTL_MOVE_L;
	else if (byMoveFlag == NTL_MOVE_R_TURN_L_JUMP || byMoveFlag == NTL_MOVE_R_TURN_R_JUMP)
		byMoveFlag = NTL_MOVE_R;

	BYTE byFormFlag = NTL_MOVE_FLAG_RUN;

	if (pBase->eAirState == AIR_STATE_ON) // check if flying
		byFormFlag = NTL_MOVE_FLAG_FLY;

	CNtlSLEventGenerator::SobMove(m_pActor->GetSerialID(), byMoveFlag, byFormFlag, vLoc, vDir, pBase->dwStateTime);
}

////////////////////////////////////////////////////////////////////////////
//  des : server dest move state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateDestMove(sCHARSTATE *pServerState)
{
	sCHARSTATE_BASE *pBase = &pServerState->sCharStateBase;
	sCHARSTATE_DESTMOVE *pDestMove = &pServerState->sCharStateDetail.sCharStateDestMove;

	// Dest location counter는 반드시 1 이상이어야 한다
	if ( 0 == pDestMove->byDestLocCount )
	{
		DBO_WARNING_MESSAGE( "A dest location counter must be one more." );

		CNtlSLEventGenerator::SobStateTransition( m_pActor->GetSerialID(), NTL_FSMSID_IDLE );

		return;
	}

	// Dest location counter는 반드시 DBO_MAX_NEXT_DEST_LOC_COUNT 보다 작거나 같아야 한다
	if ( pDestMove->byDestLocCount > DBO_MAX_NEXT_DEST_LOC_COUNT )
	{
		DBO_WARNING_MESSAGE( "A dest location counter must be less equal than DBO_MAX_NEXT_DEST_LOC_COUNT." );

		CNtlSLEventGenerator::SobStateTransition( m_pActor->GetSerialID(), NTL_FSMSID_IDLE );

		return;
	}

	if ( !Logic_IsBus( m_pActor ) )
	{
		RwV3d vCurrLoc;

		CNtlMath::MathRwV3dAssign( &vCurrLoc, pBase->vCurLoc.x, pBase->vCurLoc.y, pBase->vCurLoc.z );

		RwV3d* pNodeList = new RwV3d[ pDestMove->byDestLocCount ];

		if ( NULL == pNodeList )
		{
			DBO_FAIL( "Creating dest location nodes is failed." );
			return;
		}

		for ( int i = 0; i < (int)pDestMove->byDestLocCount; ++i )
		{
			CNtlMath::MathRwV3dAssign( &pNodeList[i], pDestMove->avDestLoc[i].x, pDestMove->avDestLoc[i].y, pDestMove->avDestLoc[i].z );
		}

		CNtlSLEventGenerator::SobDestMove( m_pActor->GetSerialID(), pDestMove->byMoveFlag, vCurrLoc, pNodeList[0], 0, pDestMove->byDestLocCount, pNodeList );

		if ( pNodeList )
		{
			delete [] pNodeList;
		}
	}
	else
	{
		RwV3d vCurrLoc, vDest;

		CNtlMath::MathRwV3dAssign( &vCurrLoc, pBase->vCurLoc.x, pBase->vCurLoc.y, pBase->vCurLoc.z );
		CNtlMath::MathRwV3dAssign( &vDest, pDestMove->avDestLoc[0].x, pDestMove->avDestLoc[0].y, pDestMove->avDestLoc[0].z );

		if ( pDestMove->bHaveSecondDestLoc )
		{
			RwV3d vSecondDest;

			CNtlMath::MathRwV3dAssign( &vSecondDest, pDestMove->vSecondDestLoc.x, pDestMove->vSecondDestLoc.y, pDestMove->vSecondDestLoc.z );

			CNtlSLEventGenerator::SobSecondDestMove( m_pActor->GetSerialID(), pDestMove->byMoveFlag, vCurrLoc, vDest, vSecondDest, pBase->dwStateTime);
		}
		else
		{
			CNtlSLEventGenerator::SobSecondDestMove( m_pActor->GetSerialID(), pDestMove->byMoveFlag, vCurrLoc, vDest, vDest, pBase->dwStateTime);
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : server follow state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateFollowing(sCHARSTATE *pServerState)
{
	sCHARSTATE_BASE *pBase = &pServerState->sCharStateBase;
	sCHARSTATE_FOLLOWING *pFollow = &pServerState->sCharStateDetail.sCharStateFollwing;

	RwV3d vLoc;
	CNtlMath::MathRwV3dAssign(&vLoc, pBase->vCurLoc.x, pBase->vCurLoc.y, pBase->vCurLoc.z);
	
	Behavior_AdjustPositionJumping(m_pActor, &vLoc);

	CNtlSLEventGenerator::SobFollowMove(m_pActor->GetSerialID(), pFollow->hTarget, pFollow->byMoveFlag, vLoc, pFollow->fDistance, 
										pFollow->byMovementReason, pBase->dwStateTime);
}

void CNtlFSMCharActAgent::ServerStateFalling(sCHARSTATE* pServerState)
{
	RwUInt32 uiCurrStateId = GetCurrStateId();
	if (uiCurrStateId == NTL_FSMSID_FALLING)
		return;

	sCHARSTATE_BASE* pBase = &pServerState->sCharStateBase;
	sCHARSTATE_FALLING* pFalling = &pServerState->sCharStateDetail.sCharStateFalling;

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());

	SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_FALLING;

	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sFalling.vFallingDir.x = pBase->vCurDir.x;
	pCtrlStuff->uExtra.sFalling.vFallingDir.y = pBase->vCurDir.y;
	pCtrlStuff->uExtra.sFalling.vFallingDir.z = pBase->vCurDir.z;
	
	pCtrlStuff->uExtra.sFalling.byMoveDirection = pFalling->byMoveDirection;
	pCtrlStuff->uExtra.sFalling.fSpeed = DBO_FALLING_SPEED;

	RwV3d vLoc;
	CNtlMath::MathRwV3dAssign(&vLoc, pBase->vCurLoc.x, pBase->vCurLoc.y, pBase->vCurLoc.z);
	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vLoc, sHStuff);

	pCtrlStuff->uExtra.sFalling.fSearchHeight = sHStuff.fFinialHeight;

	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_FALLING);
}

////////////////////////////////////////////////////////////////////////////
//  des : server dash state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateDash(sCHARSTATE *pServerState)
{
	sCHARSTATE_BASE *pBase = &pServerState->sCharStateBase;
	sCHARSTATE_DASH_PASSIVE *pDash = &pServerState->sCharStateDetail.sCharStateDashPassive;

	RwV3d vDest;
	CNtlMath::MathRwV3dAssign(&vDest, pDash->vDestLoc.x, pDash->vDestLoc.y, pDash->vDestLoc.z);

	CNtlSLEventGenerator::SobDash(m_pActor->GetSerialID(), vDest, pDash->byMoveDirection, pBase->dwStateTime);
}

////////////////////////////////////////////////////////////////////////////
//  des : server teleport state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateTeleporting(sCHARSTATE *pServerState)
{
	sCHARSTATE_TELEPORTING *pTeleport = &pServerState->sCharStateDetail.sCharStateTeleporting;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sTeleport.byTeleportType = pTeleport->byTeleportType;

	if(pTeleport->byTeleportType != TELEPORT_TYPE_GAME_IN)
	{
		CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_TELEPORT);
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : server sliding state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateSliding(sCHARSTATE *pServerState)
{
	RwUInt32 uiCurrStateId = GetCurrStateId();
	if(uiCurrStateId == NTL_FSMSID_MOVE || uiCurrStateId == NTL_FSMSID_ADJUSTMOVE || 
		uiCurrStateId == NTL_FSMSID_FOLLOW || uiCurrStateId == NTL_FSMSID_FIGHTING_FOLLOW ||
		uiCurrStateId == NTL_FSMSID_SKILL_FOLLOW || uiCurrStateId == NTL_FSMSID_HTB_FOLLOW)
	{
		CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_IDLE);
	}
}

////////////////////////////////////////////////////////////////////////////
//  des : server knockdown state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateKnockDown(sCHARSTATE *pServerState)
{
	/*RwUInt32 uiCurrStateId = GetCurrStateId();
	if(uiCurrStateId == NTL_FSMSID_MOVE || uiCurrStateId == NTL_FSMSID_ADJUSTMOVE || 
		uiCurrStateId == NTL_FSMSID_FOLLOW || uiCurrStateId == NTL_FSMSID_FIGHTING_FOLLOW ||
		uiCurrStateId == NTL_FSMSID_SKILL_FOLLOW || uiCurrStateId == NTL_FSMSID_HTB_FOLLOW)
	{
		CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_IDLE);
	}*/

	sCHARSTATE_KNOCKDOWN* pKnockdown = &pServerState->sCharStateDetail.sCharStateKnockdown;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SHitStuff *pHitStuff = pBeData->GetHitStuff();
	pHitStuff->vPushDir.x = pKnockdown->vShift.x;
	pHitStuff->vPushDir.y = pKnockdown->vShift.y;
	pHitStuff->vPushDir.z = pKnockdown->vShift.z;
//	DBO_WARNING_MESSAGE("pHitStuff->vPushDir.x: " << pHitStuff->vPushDir.x << " pHitStuff->vPushDir.y: " << pHitStuff->vPushDir.y << " pHitStuff->vPushDir.z: " << pHitStuff->vPushDir.z);

	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_KNOCKDOWN);
}

////////////////////////////////////////////////////////////////////////////
//  des : server sutn state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateStun(sCHARSTATE *pServerState)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    pBeData->GetCtrlStuff()->uExtra.sStun.byStunType = pServerState->sCharStateDetail.sCharStateStunned.byStunType;

	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_STUN);
}

////////////////////////////////////////////////////////////////////////////
//  des : server sleep state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateSleep(sCHARSTATE *pServerState)
{
	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_SLEEP);
}


////////////////////////////////////////////////////////////////////////////
//  des : server paralyze state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateParalyze(sCHARSTATE *pServerState)
{
	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_PARALYZE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server focus state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateFocusing(sCHARSTATE *pServerState)
{
	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_SKILL_FOCUS);
}

////////////////////////////////////////////////////////////////////////////
//  des : server casting state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCasting(sCHARSTATE *pServerState)
{
	sCHARSTATE_CASTING *pCasting = &pServerState->sCharStateDetail.sCharStateCasting;

	RwReal fCurrCastingTime	= (RwReal)(pCasting->dwCastingTime-pCasting->dwCastingTimeRemaining)/1000.f;
	RwReal fMaxCastingTime	= (RwReal)pCasting->dwCastingTime/1000.f;

	if(fCurrCastingTime > fMaxCastingTime)
		fCurrCastingTime = fMaxCastingTime;

	if(fCurrCastingTime < 0.0f)
		fCurrCastingTime = 0.0f;

	CNtlSLEventGenerator::SobSkillCasting(m_pActor->GetSerialID(), pCasting->hTarget, pCasting->skillId, fCurrCastingTime, fMaxCastingTime);
}

////////////////////////////////////////////////////////////////////////////
//  des : server HTB state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateHTB(sCHARSTATE *pServerState)
{
	CNtlSLEventGenerator::SobHTBSkillAction(m_pActor->GetSerialID(), pServerState);
}


////////////////////////////////////////////////////////////////////////////
//  des : server Sandbag state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateHTBSandbag(sCHARSTATE *pServerState)
{
	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_HTB_SANDBAG);
}

////////////////////////////////////////////////////////////////////////////
//  des : server item casting state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateItemCasting(sCHARSTATE *pServerState)
{
	sCHARSTATE_CASTING_ITEM *pItemCasting = &pServerState->sCharStateDetail.sCharStateCastingItem;

	RwReal fCurrCastingTime	= (RwReal)(pItemCasting->dwCastingTime-pItemCasting->dwCastingTimeRemaining)/1000.f;
	RwReal fMaxCastingTime	= (RwReal)pItemCasting->dwCastingTime/1000.f;

	if(fCurrCastingTime > fMaxCastingTime)
		fCurrCastingTime = fMaxCastingTime;

	if(fCurrCastingTime < 0.0f)
		fCurrCastingTime = 0.0f;

	CNtlSLEventGenerator::SobItemCasting(m_pActor->GetSerialID(), pItemCasting->hTarget, pItemCasting->itemTblidx, fCurrCastingTime, fMaxCastingTime);
}

////////////////////////////////////////////////////////////////////////////
//  des : server faint state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateFainting(sCHARSTATE *pServerState)
{
	if(m_byServerState != CHARSTATE_FAINTING)
	{
		CNtlSobDieSyncStatus *pDieSyncStatus = NTL_NEW CNtlSobDieSyncStatus;
		m_pActor->AddSob(pDieSyncStatus);
	}

	// disabled by daneos: So objects die faster and not a second later
	/*RwUInt32 uiCurrStateId = GetCurrStateId();
	if(uiCurrStateId == NTL_FSMSID_MOVE || uiCurrStateId == NTL_FSMSID_ADJUSTMOVE || 
		uiCurrStateId == NTL_FSMSID_FOLLOW || uiCurrStateId == NTL_FSMSID_FIGHTING_FOLLOW ||
		uiCurrStateId == NTL_FSMSID_SKILL_FOLLOW || uiCurrStateId == NTL_FSMSID_HTB_FOLLOW)
	{
		CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_IDLE);
	}*/
}

////////////////////////////////////////////////////////////////////////////
//  des : server private shop state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStatePrivateShop(sCHARSTATE *pServerState)
{
	sCHARSTATE_PRIVATESHOP *pPrivateShop = &pServerState->sCharStateDetail.sCharStatePrivateShop;
	CNtlSLEventGenerator::SobPrivateShop(m_pActor->GetSerialID(), pPrivateShop->sSummaryPrivateShopData.wcPrivateShopName, 
										pPrivateShop->sSummaryPrivateShopData.bIsOwnerEmpty, pPrivateShop->sSummaryPrivateShopData.byShopState);

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	SCtrlStuffPrivateShop *pPrivateShopStuff = &pCtrlStuff->uExtra.sPrivateShop;

	CNtlSLEventGenerator::PrivateShopNameTag(m_pActor->GetSerialID(), 
											pPrivateShopStuff->bShopOwnerEmpty,
											pPrivateShopStuff->byShopState,
											pPrivateShopStuff->wchShopName);
}


////////////////////////////////////////////////////////////////////////////
//  des : server direction state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateDirection(sCHARSTATE *pServerState)
{
	sCHARSTATE_DIRECT_PLAY *pDirectPlay = &pServerState->sCharStateDetail.sCharStateDirectPlay;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sDirection.bAnimUse		= FALSE;
	pCtrlStuff->uExtra.sDirection.byDirectType	= pDirectPlay->byDirectPlayType;
	pCtrlStuff->uExtra.sDirection.uiTblId		= pDirectPlay->directTblidx;
	pCtrlStuff->uExtra.sDirection.bTimeLoop		= FALSE;

	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_DIRECTION);
}

////////////////////////////////////////////////////////////////////////////
//  des : server operation state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateOperation(sCHARSTATE *pServerState)
{
	RwUInt32 uiCurrStateId = GetCurrStateId();
	if(uiCurrStateId == NTL_FSMSID_TRIGGER_OPERATE)
		return;

	sCHARSTATE_OPERATING *pOperation = &pServerState->sCharStateDetail.sCharStateOperating;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->hTargetSerialId					= pOperation->hTargetObject;
	pCtrlStuff->uExtra.sOperate.uiOperateTime	= pOperation->dwOperateTime;
	pCtrlStuff->uExtra.sOperate.uiTblId			= pOperation->directTblidx;

	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_TRIGGER_OPERATE);
}

////////////////////////////////////////////////////////////////////////////
//  des : server despawn state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateDespawn(sCHARSTATE *pServerState)
{
	sCHARSTATE_DESPAWNING *pDespawn = &pServerState->sCharStateDetail.sCharStateDespawning;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sTeleport.byTeleportType = pDespawn->byTeleportType;

	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_DESPAWN);
}

////////////////////////////////////////////////////////////////////////////
//  des : server camping state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateCamping(sCHARSTATE *pServerState)
{

}

////////////////////////////////////////////////////////////////////////////
//  des : server leaving state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::ServerStateLeaving(sCHARSTATE *pServerState)
{
	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_LEAVING);
}

////////////////////////////////////////////////////////////////////////////
//  des : server RideOn state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateRideOn( sCHARSTATE *pServerState ) 
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
    SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
    pCtrlStuff->sRide.hTargetSerialId = pServerState->sCharStateDetail.sCharStateRideOn.hTarget;

    CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_ONBUS);
}


////////////////////////////////////////////////////////////////////////////
//  des : server Turning state update 분석 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : server state update data structer
////////////////////////////////////////////////////////////////////////////
void CNtlFSMCharActAgent::ServerStateTurning( sCHARSTATE *pServerState ) 
{
	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_TURNING);
}

void CNtlFSMCharActAgent::ServerStateKeepingEffect( sCHARSTATE* pServerState ) 
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

	pSkillStuff->uiSkillTblId = pServerState->sCharStateDetail.sCharStateKeepingEffect.skillId;

	CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_KEEPING_EFFECT);
}

void CNtlFSMCharActAgent::ServerStateAirJump(sCHARSTATE * pServerState)
{
	sCHARSTATE_BASE *pBase = &pServerState->sCharStateBase;
	sCHARSTATE_AIRJUMPING *pMoving = &pServerState->sCharStateDetail.sCharStateAirJumping;
	if (pMoving->byMoveDirection < NTL_MOVE_HEIGHT_UP || pMoving->byMoveDirection > NTL_MOVE_HEIGHT_UP_R)
	{
		NTL_ASSERTFAIL("CNtlFSMCharActAgent::ServerStateAirJump wrong move direction: " << (int)pMoving->byMoveDirection);
	}

	RwV3d vDir;
	CNtlMath::MathRwV3dAssign(&vDir, pBase->vCurDir.x, pBase->vCurDir.y, pBase->vCurDir.z);

	CNtlSLEventGenerator::SobAirJump(m_pActor->GetSerialID(), pMoving->byMoveDirection, vDir);
}

void CNtlFSMCharActAgent::ServerStateAirDashAccel(sCHARSTATE * pServerState)
{
	sCHARSTATE_BASE *pBase = &pServerState->sCharStateBase;
	sCHARSTATE_AIRACCEL *pMoving = &pServerState->sCharStateDetail.sCharStateAirAccel;
	if (pMoving->byMoveDirection != NTL_MOVE_F && pMoving->byMoveDirection != NTL_MOVE_F_TURN_L && pMoving->byMoveDirection != NTL_MOVE_F_TURN_R)
	{
		NTL_ASSERTFAIL("CNtlFSMCharActAgent::ServerStateAirDashAccel");
	}

	RwV3d vDir, vLoc;
	CNtlMath::MathRwV3dAssign(&vDir, pBase->vCurDir.x, pBase->vCurDir.y, pBase->vCurDir.z);
	CNtlMath::MathRwV3dAssign(&vLoc, pBase->vCurLoc.x, pBase->vCurLoc.y, pBase->vCurLoc.z);

	CNtlSLEventGenerator::SobAirAccel(m_pActor->GetSerialID(), pMoving->byMoveFlag, pMoving->byMoveDirection, vLoc, vDir);
}

////////////////////////////////////////////////////////////////////////////
//  des : update 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : delta time.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMCharActAgent::Update(RwReal fElapsed)
{
	CNtlFSMAgent::Update(fElapsed); 
}


////////////////////////////////////////////////////////////////////////////
//  des : event 처리 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : event message.
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMCharActAgent::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActAgent::HandleEvents");

	RwUInt32 uiRes = CNtlFSMAgent::HandleEvents(pMsg);  
	if(uiRes != NTL_FSM_EVENTRES_PASS)
	{
		if(pMsg.Id == g_EventSobServerUpdateState)
		{
			SNtlEventSobServerUpdateState *pSobServerState = reinterpret_cast<SNtlEventSobServerUpdateState*>( pMsg.pData );

			CopyCharState(&m_sCharState, pSobServerState->pState);

			sCHARSTATE_BASE *pBaseState = &pSobServerState->pState->sCharStateBase;

			SetFightingMode(pBaseState->bFightMode);
			SetAirMode(pBaseState->eAirState);

			m_byServerState = pBaseState->byStateID;
		}

		NTL_RETURN(uiRes);
	}
	else
	{
		if(pMsg.Id == g_EventSobCreate)
		{
			SNtlEventSobCharCreate *pSobCreate = reinterpret_cast<SNtlEventSobCharCreate*>( pMsg.pData );
			ServerStateCreateAnalysis(pSobCreate->pCharState);
			NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
		}
		else if(pMsg.Id == g_EventSobSit)
		{
			SNtlEventSobSit *pSobSit = reinterpret_cast<SNtlEventSobSit*>( pMsg.pData );
			m_uActMode.bitSitDown = pSobSit->bSitDown;
		}
		else if(pMsg.Id == g_EventSobFightingMode)
		{
			SNtlEventSobFightingMode *pFightingMode = reinterpret_cast<SNtlEventSobFightingMode*>( pMsg.pData );
			SetFightingMode(pFightingMode->bFightingMode);
		}
		else if(pMsg.Id == g_EventSobHit)
		{
			RwBool bBarrier = FALSE;
			RwBool bReflection = FALSE;

			if(m_ulServerCondition & CHARCOND_FLAG_BARRIER)
				bBarrier = TRUE;

			if(m_ulServerCondition & CHARCOND_FLAG_DAMAGE_REFLECTION)
				bReflection = TRUE;

			if(bBarrier || bReflection)
			{
				RwV3d vPos = m_pActor->GetPosition();

				CNtlPLEntity *pPLEntity;
				if(bBarrier)
				{
					pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_STATE_HIT_BARRIER);
					if(pPLEntity)
						pPLEntity->SetPosition(&vPos);
				}

				if(bReflection)
				{
					pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_STATE_HIT_REFLECTION);
					if(pPLEntity)
						pPLEntity->SetPosition(&vPos);
				}
			}

			// reflection damage를 출력한다.
			// 여기는 중요한 부분.
			if(bReflection)
			{
				SNtlEventSobHit *pHit		= reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
				SHitStuff *pHitStuff		= &pHit->sHitStuff;
				CNtlSobActor *pAttackActor	= reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pHitStuff->hAttackerSerialId) );
                if(pAttackActor)
                {
                    RwV3d vPos = pAttackActor->GetPosition();
                    RwUInt8 byResult = pHitStuff->uiAttr.bitResult;
                    if( !(byResult == NTL_ATTACK_RESULT_HEALING || byResult == NTL_ATTACK_RESULT_BUFF ||
                        byResult == NTL_ATTACK_RESULT_STEAL || byResult == NTL_ATTACK_RESULT_DODGE) )
                    {
                        CNtlSobProxy *pSobProxy = pAttackActor->GetSobProxy();
                        pSobProxy->CreatePLDamageBox(vPos, pHitStuff->sDamageStuff.iReflectedDamage);
                    }
                }                
			}
		}
		else if(pMsg.Id == g_EventSobStateTransition)
		{
			SNtlEventSobStateTransition *pStateTrans = reinterpret_cast<SNtlEventSobStateTransition*>( pMsg.pData );
			SetNextStateId(pStateTrans->uiTransState);
			NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
		}
		else if(pMsg.Id == g_EventSobServerUpdateState)
		{
			SNtlEventSobServerUpdateState *pSobServerState = reinterpret_cast<SNtlEventSobServerUpdateState*>( pMsg.pData );

			ServerStateUpdateAnalysis(pSobServerState);
		}
        else if(pMsg.Id == g_EventSobServerUpdateAspect)
        {
            SNtlEventSobServerUpdateAspect* pSobServerAspect = reinterpret_cast<SNtlEventSobServerUpdateAspect*>(pMsg.pData);
            ServerAspectUpdateAnalysis(pSobServerAspect->sApectState, FALSE);
        }
		else if(pMsg.Id == g_EventSobServerUpdateCondition)
		{
			SNtlEventSobServerUpdateCondition *pSobServerState = reinterpret_cast<SNtlEventSobServerUpdateCondition*>( pMsg.pData );
			ServerConditionUpdateAnalysis(m_byServerState, pSobServerState->dwCondition);
		}
		else if(pMsg.Id == g_EventSobGetState)
		{
			SNtlEventSobGetState *pSobGetState = reinterpret_cast<SNtlEventSobGetState*>( pMsg.pData );
			pSobGetState->byServerState = m_byServerState;
			pSobGetState->byClientState = (RwUInt8)m_pActor->GetCurrentState()->GetStateId();
			pSobGetState->chClientState = (char*)m_pActor->GetCurrentState()->GetStateName();
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		else if(pMsg.Id == g_EventSobGetFightingMode)
		{
			SNtlEventSobGetFightingMode *pSobGetFightingMode = reinterpret_cast<SNtlEventSobGetFightingMode*>( pMsg.pData );
			pSobGetFightingMode->bFightingMode = (RwUInt8)IsFightingMode();
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		else if (pMsg.Id == g_EventSobGetAirMode)
		{
			SNtlEventSobGetAirMode* pSobMode = reinterpret_cast<SNtlEventSobGetAirMode*>(pMsg.pData);
			pSobMode->bAirMode = (RwUInt8)IsAirMode();
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		else if(pMsg.Id == g_EventSobVehicleStart)
		{
			SNtlEventSobVehicleStart* pData = (SNtlEventSobVehicleStart*)pMsg.pData;

			CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
			SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();
			pCtrlStuff->sVehicle.vSpawnPos = /*pBeData->GetMoveStuff()->vCurrLoc;*/ m_pActor->GetPosition(); // BY DANEOS because vCurrLoc = 0,0,0
			pCtrlStuff->sVehicle.hVehicleItem = pData->hVehicleItem;
			pCtrlStuff->sVehicle.idxVehicleItem = pData->tblVehicleItem;
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
        else if(pMsg.Id == g_EventSobUpdateLPStatusNfy) // 빈사 상태 체크
        {
            SNtlEventSobUpdateLPStatusNfy* pNfy = (SNtlEventSobUpdateLPStatusNfy*)pMsg.pData;
            if(IsEmergency() != pNfy->bEmergency)
            {
                SetEmergency(pNfy->bEmergency);
                CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
                
                if(pNfy->bEmergency)
                {
                    if(m_pSystemEffectColor)
                    {
                        pSobProxy->RemoveVisualSystemEffectColorFlickering(m_pSystemEffectColor);
                        m_pSystemEffectColor = NULL;
                    }

                    if(!m_pLPDangerColor)
                    {
                        m_pLPDangerColor = pSobProxy->AddVisualSystemEffectColorFlickering(255, 0, 0, 1.0f);

                        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
                        {
                            sNtlSoundPlayParameta tSoundParam;
                            tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
                            tSoundParam.pcFileName		= WORLD_SOUND_HEART_BEAT;
                            tSoundParam.bLoop			= true;

							GetSoundManager()->Play(&tSoundParam);

                            m_pLPDangerColor->SetExtraSound(tSoundParam.hHandle);
                        }                   
                    }
                }
                else
                {
                    if(m_pLPDangerColor)
                    {
                        m_pActor->GetSobProxy()->RemoveVisualSystemEffectColorFlickering(m_pLPDangerColor);
                        m_pLPDangerColor = NULL;
                    }

                    ConditionVisualColorFlickeringPop();
                }
            }
        }
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}



////////////////////////////////////////////////////////////////////////////
//  des : 현재 상태가 끝났을 때 next 상태를 결정한다.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : current state
////////////////////////////////////////////////////////////////////////////

RwUInt32 CNtlFSMCharActAgent::AcquireNextState(RwUInt32 uiCurrStateId)
{
	switch(uiCurrStateId)
	{
	case NTL_FSMSID_INVALID:
		return NTL_FSMSID_CREATE;
	case NTL_FSMSID_SPAWN:
		return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_MOVE:
		if( IsFightingMode() )
			return NTL_FSMSID_FIGHTING_POSE;
		else
			return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_ADJUSTMOVE:
		if( IsFightingMode() )
			return NTL_FSMSID_FIGHTING_POSE;
		else
			return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_JUMP:
		return AcquireJumpProc();
		break;
	case NTL_FSMSID_CHARGING:
		return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_SMOOTH_HEADING:
		return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_FOLLOW:
		{
			if(IsFightingMode())
				return NTL_FSMSID_FIGHTING_POSE;
			else
				return NTL_FSMSID_IDLE;
		}
		break;
	case NTL_FSMSID_FIGHTING_FOLLOW:
		{
			return NTL_FSMSID_FIGHTING_POSE;
		}
		break;
	case NTL_FSMSID_FIGHTING_POSE:
		{
			if( IsFightingMode() )
				return NTL_FSMSID_FIGHTING_FOLLOW;
			else
				return NTL_FSMSID_IDLE;
		}
		break;
	case NTL_FSMSID_FIGHTING:
		return AcquireFightingProc();
		break;
	case NTL_FSMSID_HURT:
		{
			if( IsFightingMode() )
				return NTL_FSMSID_FIGHTING_POSE;
			else
				return NTL_FSMSID_IDLE;
		}
		break;
	case NTL_FSMSID_STANDUP:
		return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_SKILL_FOLLOW:
		return NTL_FSMSID_SKILL_READY;
		break;
	case NTL_FSMSID_SKILL_READY: case NTL_FSMSID_KNOCKDOWN: case NTL_FSMSID_SLIDING:
	case NTL_FSMSID_STUN: case NTL_FSMSID_SLEEP: case NTL_FSMSID_PARALYZE:
		if( IsFightingMode() )
			return NTL_FSMSID_FIGHTING_POSE;
		else
			return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_FIGHTING_SHRINK:
		if( IsFightingMode() )
			return NTL_FSMSID_FIGHTING_FOLLOW;
		else
			return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_SKILL_ACTION:
		return AcquireSkillActionProc();
		break;
	case NTL_FSMSID_HTB_FOLLOW:
		return NTL_FSMSID_HTB_READY;
		break;
	case NTL_FSMSID_HTB:
		return AcquireHTBProc();
		break;
	case NTL_FSMSID_HTB_SANDBAG:
		return AcquireSandbagProc();
		break;
	case NTL_FSMSID_AIR_JUMP:
		return NTL_FSMSID_IDLE;
		break;
	case NTL_FSMSID_AIR_ACCEL:
		return NTL_FSMSID_IDLE;
		break;
	default:
		if( IsFightingMode() )
			return NTL_FSMSID_FIGHTING_POSE;
		else
			return NTL_FSMSID_IDLE;
		break;
	}
}

void CNtlFSMCharActAgent::NotifyUpdateChangeState(RwUInt32 uiOldState, RwUInt32 uiNextState)
{
	if(m_pActor->GetClassID() == SLCLASS_AVATAR)
		Logic_AvatarStateChange();
	else
		Logic_AvatarTargetStateChange(m_pActor->GetSerialID());

	CNtlFSMAgent::NotifyUpdateChangeState(uiOldState, uiNextState);
}

void CNtlFSMCharActAgent::NotifyEvents(RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2)
{
	if(uiEventResult == NTL_FSM_EVENTRES_CHANGE_STATE)
	{
		RwUInt32 uiCurrStateId = uiParam1;
		RwUInt32 uiNextStateId = uiParam2;

		if(uiCurrStateId == NTL_FSMSID_CREATE && uiNextStateId != NTL_FSMSID_SPAWN)
		{
			m_pActor->EnableVisible(TRUE);
		}

		if(m_pActor->GetClassID() == SLCLASS_AVATAR)
			Logic_AvatarStateChange();
		else
			Logic_AvatarTargetStateChange(m_pActor->GetSerialID());
	}

	CNtlFSMAgent::NotifyEvents(pMsg, uiEventResult, uiParam1, uiParam2);
}


RwUInt32 CNtlFSMCharActAgent::AcquireFightingProc(void)
{
	if( IsFightingMode() )
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		CNtlAttackData *pAttData = pBeData->GetAttackData();
		SAttackStuff *pAttackStuff = pAttData->GetAttackStuff(0);

		if(pAttackStuff->uiType.bitChainAttack)
			return NTL_FSMSID_FIGHTING_SHRINK;
		else
			return NTL_FSMSID_FIGHTING_POSE;
	}
	else
	{
		return NTL_FSMSID_IDLE;
	}
}

RwUInt32 CNtlFSMCharActAgent::AcquireSkillActionProc(void)
{
	if( IsFightingMode() )
	{
		// move stuff 속성 setting
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 

		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		SSkillStuff *pSkillStuff = pBeData->GetSkillStuff(); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

		if(m_pActor->GetSerialID() != pSkillStuff->hTargetSerialId)
		{
			pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
			pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
			pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
			pMoveStuff->hFollowSerialId = pSkillStuff->hTargetSerialId;
			pMoveStuff->fFollowRange = Logic_GetAttackRange(m_pActor, pSkillStuff->hTargetSerialId);

			pCtrlStuff->hTargetSerialId = pSkillStuff->hTargetSerialId;
			pCtrlStuff->fFollowRange = pMoveStuff->fFollowRange;
		}
		
		return NTL_FSMSID_FIGHTING_POSE;
	}
	else
		return NTL_FSMSID_FIGHTING_POSE;        // 파이팅 모드가 아닐때도 파이팅포즈->보간을 한다.
}

RwUInt32 CNtlFSMCharActAgent::AcquireHTBProc(void)
{
	if( IsFightingMode() )
	{
		// move stuff 속성 setting
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 

		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		SHTBStuff *pHTBStuff = pBeData->GetHTBStuff(); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

		if(m_pActor->GetSerialID() != pHTBStuff->hTargetSerialId)
		{
			pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
			pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
			pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
			pMoveStuff->hFollowSerialId = pHTBStuff->hTargetSerialId;
			pMoveStuff->fFollowRange = Logic_GetAttackRange(m_pActor, pHTBStuff->hTargetSerialId);

			pCtrlStuff->hTargetSerialId = pHTBStuff->hTargetSerialId;
			pCtrlStuff->fFollowRange = pMoveStuff->fFollowRange;
		}
		
		return NTL_FSMSID_FIGHTING_POSE;
	}
	else
		return NTL_FSMSID_IDLE;
}

RwUInt32 CNtlFSMCharActAgent::AcquireSandbagProc(void)
{
	if( IsFightingMode() )
		return NTL_FSMSID_FIGHTING_POSE;
	else
		return NTL_FSMSID_IDLE;
}

RwUInt32 CNtlFSMCharActAgent::AcquireJumpProc(void)
{
	if( IsAutoRun() )
	{
		//CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 

		//SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
		//pMoveStuff->byType = NTL_MOVETARGET_DIR;
		//pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
		//pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
		//pMoveStuff->byMoveFlags = NTL_MOVE_F;

		//RwV3d vPos = m_pActor->GetPosition();
  //      SWorldHeightStuff sHStuff;
  //      if(Logic_IsSwimmingActor(m_pActor, &vPos, sHStuff))
  //      {
  //          pMoveStuff->byForm = NTL_MOVEFORM_SWIMMING;
		//	return NTL_FSMSID_SWIMMING;
		//}



        return NTL_FSMSID_MOVE;
	}
	else
    {
		return NTL_FSMSID_IDLE;
    }
}


RwBool CNtlFSMCharActAgent::IsDie(void)
{
	if(m_byServerState == CHARSTATE_FAINTING)
		return TRUE;

	return FALSE;
}

void CNtlFSMCharActAgent::ConditionVisualColorFlickering(RwUInt64 uiServerCondition)
{
	RwUInt64 uiDiffCondition = m_ulServerCondition ^ uiServerCondition;

	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();

	// We judge whether there is something new.

	if (uiDiffCondition & CHARCOND_FLAG_BLEEDING)
	{
		if (uiServerCondition & CHARCOND_FLAG_BLEEDING)
		{
			if (m_pSystemEffectColor)
				pSobProxy->RemoveVisualSystemEffectColorFlickering(m_pSystemEffectColor);

			m_pSystemEffectColor = m_pActor->GetSobProxy()->AddVisualSystemEffectColorFlickering(255, 255, 0, 2.0f);

			return;
		}
	}

	if(uiDiffCondition & CHARCOND_FLAG_POISON)
	{
		if(uiServerCondition & CHARCOND_FLAG_POISON)
		{
			if(m_pSystemEffectColor)
				pSobProxy->RemoveVisualSystemEffectColorFlickering(m_pSystemEffectColor);

			m_pSystemEffectColor = m_pActor->GetSobProxy()->AddVisualSystemEffectColorFlickering(102, 0, 102, 2.0f);

			return;
		}
	}

	if(uiDiffCondition & CHARCOND_FLAG_STOMACHACHE)
	{
		if(uiServerCondition & CHARCOND_FLAG_STOMACHACHE)
		{
			if(m_pSystemEffectColor)
				pSobProxy->RemoveVisualSystemEffectColorFlickering(m_pSystemEffectColor);

			m_pSystemEffectColor = m_pActor->GetSobProxy()->AddVisualSystemEffectColorFlickering(0, 0, 255, 2.0f);

			return;
		}
	}

	// Is the condition cleared?
	if(!(m_sCharState.sCharStateBase.dwConditionFlag & CHARCOND_FLAG_BLEEDING) && !(m_sCharState.sCharStateBase.dwConditionFlag & CHARCOND_FLAG_POISON) &&
		!(m_sCharState.sCharStateBase.dwConditionFlag & CHARCOND_FLAG_STOMACHACHE) )
	{
		if(m_pSystemEffectColor)
			pSobProxy->RemoveVisualSystemEffectColorFlickering(m_pSystemEffectColor);

		m_pSystemEffectColor = NULL;
	}
}

void CNtlFSMCharActAgent::ConditionVisualColorFlickeringPop(void)
{
	// We judge whether there is something new.

	if (m_ulServerCondition & CHARCOND_FLAG_BLEEDING)
	{
		m_pSystemEffectColor = m_pActor->GetSobProxy()->AddVisualSystemEffectColorFlickering(255, 255, 0, 2.0f);
		return;
	}

	if(m_ulServerCondition & CHARCOND_FLAG_POISON)
	{
		m_pSystemEffectColor = m_pActor->GetSobProxy()->AddVisualSystemEffectColorFlickering(102, 0, 102, 2.0f);
		return;
	}

	if(m_ulServerCondition & CHARCOND_FLAG_STOMACHACHE)
	{
		m_pSystemEffectColor = m_pActor->GetSobProxy()->AddVisualSystemEffectColorFlickering(0, 0, 255, 2.0f);
		return;
	}
}

CNtlPLEntity* CNtlFSMCharActAgent::CreateVisualSystemEffect(const RwChar *pKey)
{
	RwV3d vPos = m_pActor->GetPosition();
	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
	RwReal fCharHeight = pSobProxy->GetPLEntityHeight() / 1.5f;

	RwV3d vEffectPos, vOffset;
	CNtlMath::MathRwV3dAssign(&vEffectPos, vPos.x, vPos.y + fCharHeight, vPos.z);
	CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, fCharHeight, 0.0f);

	CNtlPLEntity *pPLEntity = pSobProxy->CreatePLEffect(pKey, &vEffectPos);
	if(pPLEntity)
	{
		CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
		Helper_AttachWorldPos(pPLCharacter, pPLEntity, vOffset);
	}

	return pPLEntity;
}

void CNtlFSMCharActAgent::ConditionDiff(RwUInt64 uiServerCondition)
{
	RwUInt64 uiDiffCondition = m_ulServerCondition ^ uiServerCondition;
	m_ulServerCondition = uiServerCondition;

    if(m_ulServerCondition == 0 && uiDiffCondition == 0)
    {
        ConditionAfterEffect(FALSE);
        ConditionConfused(FALSE);
        ConditionTerror(FALSE);
        ConditionTaunt(FALSE);     
		ConditionBleed(FALSE);
		ConditionPoison(FALSE);
		ConditionStomach(FALSE);
        return;
    }

    if(uiDiffCondition & CHARCOND_FLAG_AFTEREFFECT)
    {
        ConditionAfterEffect(uiServerCondition & CHARCOND_FLAG_AFTEREFFECT);
    }

	if(uiDiffCondition & CHARCOND_FLAG_TAUNT)
	{
        ConditionTaunt(uiServerCondition & CHARCOND_FLAG_TAUNT);
	}

	if (uiDiffCondition & CHARCOND_FLAG_BLEEDING)
	{
		ConditionBleed(uiServerCondition & CHARCOND_FLAG_BLEEDING);
	}

	if (uiDiffCondition & CHARCOND_FLAG_POISON)
	{
		ConditionPoison(uiServerCondition & CHARCOND_FLAG_POISON);
	}

	if (uiDiffCondition & CHARCOND_FLAG_STOMACHACHE)
	{
		ConditionStomach(uiServerCondition & CHARCOND_FLAG_STOMACHACHE);
	}

    if(uiDiffCondition & CHARCOND_FLAG_CONFUSED)
    {
        ConditionConfused(uiServerCondition & CHARCOND_FLAG_CONFUSED);
    }

    if(uiDiffCondition & CHARCOND_FLAG_TERROR)
    {
        ConditionTerror(uiServerCondition & CHARCOND_FLAG_TERROR);		
    }

    if(uiDiffCondition & CHARCOND_FLAG_TRANSPARENT)
    {
        ConditionTransparent((uiServerCondition & CHARCOND_FLAG_TRANSPARENT)? TRUE: FALSE);
    }

    if(uiDiffCondition & CHARCOND_FLAG_INVISIBLE)
    {
        ConditionInvisible((uiServerCondition & CHARCOND_FLAG_INVISIBLE)? TRUE : FALSE);
    }

    if(uiDiffCondition & CHARCOND_FLAG_HIDING_KI)
    {
        ConditionHidingKi((uiServerCondition & CHARCOND_FLAG_HIDING_KI)? TRUE : FALSE);
    }

	if (uiDiffCondition & CHARCOND_FLAG_TAIYOU_KEN)
	{
		// TO DO
	}

	if (uiDiffCondition & CHARCOND_FLAG_FAKE_DEATH)
	{
		if (uiServerCondition & CHARCOND_FLAG_FAKE_DEATH)
		{
			if (Logic_GetAvatarTargetHandle() != INVALID_SERIAL_ID)
				Logic_SobTarget(INVALID_SERIAL_ID, INVALID_BYTE);
		}
	}
}

void CNtlFSMCharActAgent::ConditionAfterEffect(RwBool bAffect)
{
    if(m_pActor->GetClassID() != SLCLASS_AVATAR && m_pActor->GetClassID() != SLCLASS_PLAYER)
        return;

    if(bAffect)
    {
        // Actual deferment processing is processed when transitioning to Idle State, and only the flag is checked here.
        if(m_pActor->GetCurrentState() /*&& m_pActor->GetCurrentState()->GetStateId() == NTL_FSMSID_IDLE*/)
        {
            // It only waits for idle state when it is in the state of restrained state and idle state in other state.
            CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_SEQUELA);        
        }        

        SetTransformSequela(TRUE);
    }
    else if(IsTransformSequela())
    {
        SetTransformSequela(FALSE);
        if(Logic_CanTranslateIdleState(m_pActor))
        {
            CNtlSLEventGenerator::SobStateTransition(m_pActor->GetSerialID(), NTL_FSMSID_IDLE);
        }
    }
}

void CNtlFSMCharActAgent::ConditionConfused(RwBool bAffect)
{
    if(bAffect)
    {
        // confused effect 생성
        if(!m_pConditionConfuse)
        {
            m_pConditionConfuse = CreateVisualSystemEffect(NTL_VID_STATE_CONFUSED);
            SetDirectMode(TRUE);

            // 실제 처리는 이벤트를 받으면 처리
        }
    }
    else
    {
        if(m_pConditionConfuse)
        {
            GetSceneManager()->DeleteEntity(m_pConditionConfuse);
            m_pConditionConfuse = NULL;

            SetDirectMode(FALSE);

            CNtlSLEventGenerator::SobCondConfused(FALSE, m_pActor->GetSerialID(), INVALID_SERIAL_ID);
        }
    }
}

void CNtlFSMCharActAgent::ConditionTerror(RwBool bAffect)
{
    if(bAffect)
    {
        // terror effect 생성
        if(!m_pConditionTerror)
        {
            m_pConditionTerror = CreateVisualSystemEffect(NTL_VID_STATE_TERROR);
            SetDirectMode(TRUE);
        }
    }
    else
    {
        if(m_pConditionTerror)
        {
            GetSceneManager()->DeleteEntity(m_pConditionTerror);
            m_pConditionTerror = NULL;

            SetDirectMode(FALSE);

            CNtlSLEventGenerator::SobCondTerror(FALSE, m_pActor->GetSerialID(), INVALID_SERIAL_ID);
        }
    }
}

void CNtlFSMCharActAgent::ConditionTaunt( RwBool bAffect ) 
{
    if(bAffect)
    {
        // taunt effect 생성
        if(!m_pConditionTaunt)
        {
            m_pConditionTaunt = CreateVisualSystemEffect(NTL_VID_STATE_TAUNT);
        }        
    }
    else if(m_pConditionTaunt)
    {
        GetSceneManager()->DeleteEntity(m_pConditionTaunt);
        m_pConditionTaunt = NULL;        
    }    
}

void CNtlFSMCharActAgent::ConditionTransparent( RwBool bAffect ) 
{
    if(IsTransparent() == bAffect)
        return;

    SetTransparent(bAffect);

    if(bAffect)
    {
        // 아바타는 보이지 않는게 아니라, 알파만 먹여준다.
        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            m_pSystemGMAlpha = m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(0.5f, 0.0f, TRUE);
            m_pActor->GetSobProxy()->SetInkThicknessWeight(0.5f);
        }
        else
        {
            m_pSystemGMAlpha = m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(0.0f, 0.0f, TRUE);
            m_pActor->GetSobProxy()->SetInkThicknessWeight(0.0f);            
        }        
    }
    else 
    {
        if(m_pSystemGMAlpha)
        {
            m_pActor->GetSobProxy()->RemoveVisualSystemEffectAlpha(m_pSystemGMAlpha);
            m_pActor->GetSobProxy()->SetInkThicknessWeight(1.0f);
            m_pSystemGMAlpha = NULL;
        }            
    }
}

void CNtlFSMCharActAgent::ConditionInvisible( RwBool bAffect ) 
{
    if(IsInvisible() == bAffect)
        return;

    SetInvisible(bAffect);

    if(bAffect)
    {
        m_pSystemInvisible = m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(0.0f, 1.0f, TRUE);
    }
    else if(m_pSystemInvisible)
    {
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectAlpha(m_pSystemInvisible);
    }
}

void CNtlFSMCharActAgent::ConditionHidingKi( RwBool bAffect ) 
{
    // 아바타만 기숨기기시에 Alpha를 처리한다.
    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        if(IsHidingKi() == bAffect)
            return;

        SetHidingKi(bAffect);

        if(bAffect)
        {
            m_pSystemHidingKi = m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(0.7f, 1.0f, TRUE);
        }
        else if(m_pSystemHidingKi)
        {
            m_pActor->GetSobProxy()->RemoveVisualSystemEffectAlpha(m_pSystemHidingKi);
        }
    }

    CNtlSLEventGenerator::SobCondHidingKi(bAffect, m_pActor->GetSerialID());
}

void CNtlFSMCharActAgent::ConditionBleed(RwBool bAffect)
{
	if (bAffect)
	{
		if (!m_pConditionBleed)
		{
			m_pConditionBleed = CreateVisualSystemEffect(NTL_VID_STATE_BLEED);
		}
	}
	else if (m_pConditionBleed)
	{
		GetSceneManager()->DeleteEntity(m_pConditionBleed);
		m_pConditionBleed = NULL;
	}
}

void CNtlFSMCharActAgent::ConditionPoison(RwBool bAffect)
{
	if (bAffect)
	{
		if (!m_pConditionPoison)
		{
			m_pConditionPoison = CreateVisualSystemEffect(NTL_VID_STATE_POISON);
		}
	}
	else if (m_pConditionPoison)
	{
		GetSceneManager()->DeleteEntity(m_pConditionPoison);
		m_pConditionPoison = NULL;
	}
}

void CNtlFSMCharActAgent::ConditionStomach(RwBool bAffect)
{
	if (bAffect)
	{
		if (!m_pConditionStomach)
		{
			m_pConditionStomach = CreateVisualSystemEffect(NTL_VID_STATE_STOMACH);
		}
	}
	else if (m_pConditionStomach)
	{
		GetSceneManager()->DeleteEntity(m_pConditionStomach);
		m_pConditionStomach = NULL;
	}
}

void CNtlFSMCharActAgent::SetTransformSequela(RwBool bSequela)
{
    m_uActMode.bitSequela = bSequela;

    // 아바타라면 변신 휴유증 이벤트를 보낸다.
    if(bSequela && m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        CNtlSLEventGenerator::SobTransformSequela(m_pActor->GetSerialID());        
    }
}