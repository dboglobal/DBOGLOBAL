#include "precomp_ntlsimulation.h"
#include "NtlFSMCharActState.h"

// shared
#include "NtlSharedType.h"
#include "NtlResultCode.h"
#include "NtlMovement.h"
#include "NtlDirection.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "NtlBattle.h"
#include "ChatCommandTable.h"
#include "ActionTable.h"
#include "DirectionLinkTable.h"
#include "VehicleTable.h"
#include "NtlCharacter.h"

// sound
#include "WorldSoundDefine.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlCharacterData.h"
#include "NtlPLCharacter.h"
#include "NtlPLSceneManager.h"
#include "NtlPLHelpers.h"
#include "NtlPLEventGenerator.h"

// simulation
#include "NtlFSMDef.h"
#include "NtlFSMAgent.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobActor.h"
#include "NtlBehaviorData.h"
#include "NtlBehaviorChar.h"
#include "NtlBehaviorCharMove.h"
#include "NtlBehaviorCharGroundMove.h"
#include "NtlBehaviorCharSwimmingMove.h"
#include "NtlBehaviorCharJump.h"
#include "NtlBehaviorCharAirMove.h"
#include "NtlBehaviorCharAdjustMove.h"
#include "NtlBehaviorCharIdlePattern.h"
#include "NtlSobProxy.h"
#include "NtlSobCharProxy.h"
#include "NtlSobManager.h"
#include "NtlFSMCharActEventProc.h"
#include "NtlSobSyncStatus.h"
#include "NtlFSMCharActEventProc.h"
#include "NtlFSMEventQueuingHelper.h"
#include "NtlSLLuaGlue.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "TableContainer.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobElapsedController.h"
#include "NtlCameraController.h"
#include "NtlSLApi.h"
#include "NtlWorldConceptDBC.h"
#include "NtlCameraManager.h"
#include "NtlSobCharProxyDecoration.h"
#include "NtlFSMCharActAgent.h"
#include "NtlSobAvatar.h"
#include "NtlFSMUtil.h"
#include "NtlSlVisualLogic.h"
#include "NtlFSMLayer.h"
#include "NtlSobVehicle.h"
#include "NtlSobStatusAnimSyncManager.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobManager.h"
#include "SystemEffectTable.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActCreateState)

CNtlFSMCharActCreateState::CNtlFSMCharActCreateState()
{
	SetStateId(NTL_FSMSID_CREATE);
	SetStateName(NTL_FSMSN_CREATE);
	SetFlags(NTL_FSMSF_NOT_ALL);
}

void CNtlFSMCharActCreateState::Enter(void)
{
	m_byReadyCnt = 0;

	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();

	if (m_pActor->IsAirMode())
		pSobProxy->SetBaseAnimation(FLYING_STAND_IDLE);
	else
		pSobProxy->SetBaseAnimation(NML_IDLE_LOOP);

	m_pActor->EnableVisible(FALSE);
	
	CNtlFSMCharActStateBase::Enter();
}

void CNtlFSMCharActCreateState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActCreateState::Update(RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	
	SWorldHeightStuff sHStuff;
	RwBool bReady = Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);

	if(bReady)
	{
		m_byReadyCnt++;
	}

	if(m_byReadyCnt >= 2)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_SPAWN);
	}
}


RwBool CNtlFSMCharActCreateState::IsActorActiveState(void)
{
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSpawnState)


CNtlFSMCharActSpawnState::CNtlFSMCharActSpawnState()
{
	SetStateId(NTL_FSMSID_SPAWN);
	SetStateName(NTL_FSMSN_SPAWN);
	SetFlags(NTL_FSMSF_NOT_ALL);
}

void CNtlFSMCharActSpawnState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharSpawn;
	AddBehavior(pBehavior);
}

void CNtlFSMCharActSpawnState::Exit(void)
{
	if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
	{
		API_GetSLPacketGenerator()->SendCharReady(m_pActor->GetServerSyncAvatarType());

		CNtlSLEventGenerator::CharReady();
	}

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActSpawnState::Update(RwReal fElapsed)
{
	CNtlFSMCharActStateBase::Update(fElapsed);
}

RwBool CNtlFSMCharActSpawnState::IsActorActiveState(void)
{
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActIdleState)

CNtlFSMCharActIdleState::CNtlFSMCharActIdleState()
{
	SetStateId(NTL_FSMSID_IDLE);
	SetStateName(NTL_FSMSN_IDLE);
	SetFlags(NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_RUNJUMP);
	
	m_bWaterIn			= FALSE;
	m_bAdjustMoveCheck	= FALSE;
	m_fGravitySpeed		= 0.0f;
	m_fIdleCurrTime		= 0.0f;
	m_fIdleTime			= NtlRandomNumber(10.0f, 20.0f);
    m_fCalcHeightTime	= 0.0f;
	m_byIdleState = -1;
	m_pPLSwimIdle		= NULL;
	m_pIdlePattern		= NULL;
}

CNtlFSMCharActIdleState::~CNtlFSMCharActIdleState()
{
	if(m_pPLSwimIdle)
	{
		m_pPLSwimIdle->Finish();
		m_pPLSwimIdle = NULL;
	}
    
    NTL_DELETE(m_pIdlePattern);

}

void CNtlFSMCharActIdleState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// 
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->byType = NTL_MOVETARGET_NONE;
	pMoveStuff->byMoveFlags = NTL_MOVE_NONE;	
    

	// monster일 경우, move pattern 적용.
	if( Logic_IsMovePatternApply(m_pActor) )
	{
		m_pIdlePattern = NTL_NEW CNtlBehaviorCharIdlePattern;
		m_pIdlePattern->SetActor(m_pActor);
	}
   
	//
	CheckSwimming();

	if(!m_bWaterIn)
	{
		if(m_pActor->GetSobProxy()->GetDisableAniChange() == false) // required for rolling/spinning
			SetAnim(0);

		if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION && !Logic_IsGroundFlyActor(m_pActor) && !Logic_IsAirMode(m_pActor))
		{
			RwV3d vPos = m_pActor->GetPosition();
			SWorldHeightStuff sHStuff;
			Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);

			if(vPos.y < sHStuff.fFinialHeight)
				ChangeIdleState(STATE_IDLE_LOOP);
 			else if(vPos.y >= sHStuff.fFinialHeight && vPos.y <= sHStuff.fFinialHeight + 0.1f)
				ChangeIdleState(STATE_IDLE_SLIP);
			else
			{
				ChangeIdleState(STATE_IDLE_TOPDOWN);
				DBO_WARNING_MESSAGE("GO IDLE 1");
			}

            // If you are on a trigger object, check the height because the object may disappear.
            if(CNtlPLGlobal::m_pWHEntity)
            {
                CNtlSob* pObj = GetNtlSobManager()->GetSobObject(CNtlPLGlobal::m_pWHEntity->GetSerialID());
                if(pObj && pObj->GetClassID() == SLCLASS_TRIGGER_OBJECT)
                {
					m_pActor->SetActorFlags(m_pActor->GetActorFlags() | SLFLAG_ACTOR_CALC_HEIGHT);
                }                
            }
		}
		else
		{
			ChangeIdleState(STATE_IDLE_LOOP);
		}
	}
}

void CNtlFSMCharActIdleState::Exit(void)
{
	// monster일 경우
	if(m_pIdlePattern)
	{
//		Logic_GetMovePatternSync(m_pActor->GetSerialID(), 0);

		NTL_DELETE( m_pIdlePattern );
	}

    m_pActor->SetActorFlags(m_pActor->GetActorFlags() & ~SLFLAG_ACTOR_CALC_HEIGHT);

	if(m_pPLSwimIdle)
	{
		m_pPLSwimIdle->Finish();
		m_pPLSwimIdle = NULL;
	}

	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	pPLCharacter->SetBlend(BLEND_TWEEN);

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActIdleState::Update(RwReal fElapsed)
{
	// If it is a vehicle driver, it only performs basic update processing
	if ( Logic_IsVehicleDriver( m_pActor ) )
	{
		return;
	}

	if (Logic_IsAirMode(m_pActor))
		return;

	if(m_bWaterIn)
		return;

    // Fear check. In the case of fear, he runs away in a random direction.
    if(Logic_IsCondition(m_pActor, CHARCOND_FLAG_TERROR))    
    {
        UpdateTerror(fElapsed);
        return;      
    }

    if(m_pActor->GetActorFlags() & SLFLAG_ACTOR_CALC_HEIGHT)
        UpdateCalcHeight(fElapsed);

	if(m_byIdleState == STATE_IDLE_TOPDOWN)
		UpdateTopDown(fElapsed);
	else if(m_byIdleState == STATE_IDLE_SLIP)
		UpdateSlipping(fElapsed);
	else
		UpdateLoop(fElapsed);
}

RwUInt32 CNtlFSMCharActIdleState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActIdleState::HandleEvents");

	if(pMsg.Id == g_EventSobEquipChange)
	{
		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
		if(uiCurrAnimKey == NML_IDLE_01 || uiCurrAnimKey == NML_IDLE_02 || 
			uiCurrAnimKey == NML_STAFF_IDLE_01 || uiCurrAnimKey == NML_IDLE_02 || uiCurrAnimKey == SWIM_IDLE || uiCurrAnimKey == FLYING_STAND_IDLE)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_PASS);
		}

        SetAnim(0);
		
		NTL_RETURN(NTL_FSM_EVENTRES_PASS);
	}
	else if(pMsg.Id == g_EventAnimEnd)
	{
        SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
        RwUInt32 uiAnimKey = pAnimEnd->uiBaseAnimKey;
		if(uiAnimKey == NML_IDLE_01 || uiAnimKey == NML_IDLE_02 || 
			uiAnimKey == NML_STAFF_IDLE_01 || uiAnimKey == NML_STAFF_IDLE_02)
		{
			// monster idle pattern coding을 따로 한다.
			if(m_pIdlePattern)
			{
				m_pIdlePattern->NextUnit();
			}
			else
			{
				SetAnim(0);
			}
		}
		else if(uiAnimKey == SOC_AGREE)
		{
			if( Logic_IsGambleActor(m_pActor) )
				CNtlSLEventGenerator::GambleAniEnd(m_pActor->GetSerialID());	
		}
	}
	else if(pMsg.Id == g_EventTerrainClick || pMsg.Id == g_EventKeyboardMove || pMsg.Id == g_EventKeyboardMoveStop)
	{
		if(pMsg.Id == g_EventKeyboardMoveStop)
		{
			if(m_bWaterIn)
			{
				NTL_RETURN(NTL_FSM_EVENTRES_BLOCK); 
			}
			else
			{
				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
				SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
				pMoveStuff->byType = NTL_MOVETARGET_NONE;
				pMoveStuff->byMoveFlags = NTL_MOVE_NONE;

				NTL_RETURN(NTL_FSM_EVENTRES_QUEUING_REFRESH); 
			}
		}

		if(m_byIdleState == STATE_IDLE_TOPDOWN)
		{
			DBO_WARNING_MESSAGE("if(m_byIdleState == STATE_IDLE_TOPDOWN)");
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING); // if we do this, then character some times dont swim. BY DANEOS
		}
	}

	else if(pMsg.Id == g_EventMouseDashMove || pMsg.Id == g_EventKeyboardDashMove)
	{
		if(m_bWaterIn)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
	}
	else if(pMsg.Id == g_EventGambleAniStart)
	{
		m_pActor->GetSobProxy()->SetBaseAnimation(SOC_AGREE, FALSE);
	}
	else if(pMsg.Id == g_EventSobMovePatternSync)
	{
		if(m_pIdlePattern)
		{
			m_pIdlePattern->SyncUnit();
		}
	}
	
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

void CNtlFSMCharActIdleState::SetAnim(RwUInt8 byAnimOrder)
{
	// idle pattern이 있는 monster일 경우에는 적용 안한다.
	if(m_pIdlePattern)
	{
		return;
	}

	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
	RwUInt32 uiNextAnimKey = 0;

    // 나메크 변신 체크
    if(Logic_IsTransformGreatNamek(m_pActor))
    {
        uiNextAnimKey = TRANS_IDLE;
        pSobProxy->SetBaseAnimation(uiNextAnimKey);
        return;
    }

	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

	if(byAnimOrder == 0)
	{
        uiNextAnimKey = Logic_GetIdleAnimationID(m_pActor);        
		if(uiCurrAnimKey != uiNextAnimKey)
			pSobProxy->SetBaseAnimation(uiNextAnimKey);
	}
	else if(byAnimOrder == 1)
	{
		if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			uiNextAnimKey = NML_STAFF_IDLE_01;
		else
			uiNextAnimKey = NML_IDLE_01;
            
        if(Logic_IsDragonBallNPCActor(m_pActor->GetSerialID()))
            return;
        
		if(uiCurrAnimKey != uiNextAnimKey)
			pSobProxy->SetBaseAnimation(uiNextAnimKey, FALSE);
	}
	else
	{
		

		if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			uiNextAnimKey = NML_STAFF_IDLE_02;
		else
			uiNextAnimKey = NML_IDLE_02;

        if(Logic_IsDragonBallNPCActor(m_pActor->GetSerialID()))
            return;

		if(uiCurrAnimKey != uiNextAnimKey)
			pSobProxy->SetBaseAnimation(uiNextAnimKey, FALSE);
	}
}

RwBool CNtlFSMCharActIdleState::CheckSwimming(void)
{
	if(m_bWaterIn)
	{
		return TRUE;
	}

	RwV3d vPos = m_pActor->GetPosition();
	SWorldHeightStuff sHStuff;
	RwBool bWaterIn = Logic_IsSwimmingActor(m_pActor, &vPos, sHStuff);
	if(bWaterIn)
	{
		m_bWaterIn = bWaterIn;
	}

	if(m_bWaterIn)
	{
		vPos.y = sHStuff.fFinialHeight;
		m_pActor->SetPosition(&vPos);

		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
		
		pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.3f);
		pSobProxy->SetBaseAnimation(SWIM_IDLE, TRUE);

		vPos.y = sHStuff.fWaterHeight;
		CNtlPLEntity *pPLEntity = pSobProxy->CreatePLEffect(NTL_VID_SWIM_IDLE, &vPos);
		if(pPLEntity && !pPLEntity->IsAutoDelete())
			m_pPLSwimIdle = pPLEntity;

		RwUInt32 uiFlags = GetFlags();
		uiFlags |= NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SKILLUSE | 
					NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_ATTACK_CAN |
					NTL_FSMSF_NOT_HURT_CAN | NTL_FSMSF_NOT_RUNJUMP;

		SetFlags(uiFlags);
	}

    return m_bWaterIn;
}

void CNtlFSMCharActIdleState::UpdateTopDown(RwReal fElapsed)
{
	m_fGravitySpeed += fElapsed*(-1.0f);

	RwV3d vPos = m_pActor->GetPosition();
	
	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);

	if(vPos.y + m_fGravitySpeed <= sHStuff.fFinialHeight)
	{
		vPos.y = sHStuff.fFinialHeight;

		if(!m_bWaterIn)
			CheckSwimming();

		ChangeIdleState(STATE_IDLE_SLIP);
	}
	else
	{
		vPos.y += m_fGravitySpeed;
	}

	m_pActor->SetPosition(&vPos);

	
}

void CNtlFSMCharActIdleState::UpdateSlipping(RwReal fElapsed)
{
	RwV3d vNewPos, vPos;
	RwBool bSlipping = Logic_CharacterSlipping(m_pActor, vNewPos, fElapsed);    
	if(!bSlipping)
    {
		ChangeIdleState(STATE_IDLE_LOOP);
        return;
    }
	else
	{
		vPos = m_pActor->GetPosition();
		if(vPos.y - vNewPos.y >= 0.3f)
		{
			vNewPos.y = vPos.y;
			DBO_WARNING_MESSAGE("GO IDLE 3");
			ChangeIdleState(STATE_IDLE_TOPDOWN);
            return;
		}		
	}

	if(!CheckSwimming())
	{
        //// 충돌 체크        
        //SWorldHeightStuff sHstuff;
        //Logic_GetWorldHeight(m_pActor, &vNewPos, sHstuff);
        //RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
        //if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
        //{
        //    RwBool bCollMoveImPossible;
        //    byColliResult = Logic_CharacterCollisionEx(m_pActor, &vNewPos, NTL_DASH_SPEED, vNewPos, bCollMoveImPossible, fElapsed);

        //    if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        //    {
        //        vNewPos.y = sHstuff.fFinialHeight;
        //        ChangeIdleState(STATE_IDLE_LOOP);
        //    }
        //    else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        //    {                
        //        ChangeIdleState(STATE_IDLE_LOOP);
        //    }            
        //}

        //// Falling 체크
        //if( m_pActor->GetFlags() & SLFLAG_FALLING_ENABLE && vNewPos.y - sHstuff.fFinialHeight > FALLING_CHECK_LEN)
        //{
        //    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
        //    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
        //    pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_FALLING;

        //    SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

        //    RwV3d vFallingDir = vNewPos - vPos;
        //    RwV3dNormalize(&vFallingDir, &vFallingDir);
        //    pCtrlStuff->uExtra.sFalling.byMoveDirection		= NTL_MOVE_NONE;
        //    pCtrlStuff->uExtra.sFalling.vFallingDir		= vFallingDir;
        //    pCtrlStuff->uExtra.sFalling.fSearchHeight	= sHstuff.fFinialHeight;
        //    pCtrlStuff->uExtra.sFalling.fSpeed          = Logic_GetFrontRunSpeed(m_pActor);
        //    
        //    Finish();
        //    SetNextStateId(NTL_FSMSID_FALLING);            
        //}
    }

    m_pActor->SetPosition(&vNewPos);
}

void CNtlFSMCharActIdleState::UpdateLoop(RwReal fElapsed)
{
	if(m_pIdlePattern)
	{
		m_pIdlePattern->Update(fElapsed);
	}
	else
	{
		m_fIdleCurrTime += fElapsed;
		
		if(m_fIdleCurrTime >= m_fIdleTime)
		{
			m_fIdleCurrTime = 0.0f;

			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
			RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();

			if( uiCurrAnimKey == NML_IDLE_LOOP || uiCurrAnimKey == NML_STAFF_IDLE_LOOP )
			{
				m_fIdleTime = NtlRandomNumber(10.0f, 20.0f);
				SetAnim((RwUInt8)(rand() % 2 + 1));
			}
		}
	}
}

void CNtlFSMCharActIdleState::ChangeIdleState(RwUInt8 byState)
{
	m_byIdleState = byState;

	if(byState == STATE_IDLE_TOPDOWN)
	{
		SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_JUMP |
				NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_ADJUSTMOVE | 
				NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_SKIP_SITDOWN | NTL_FSMSF_NOT_CAHNGE_HEADING);

	}
	else if(byState == STATE_IDLE_SLIP)
	{
		SetFlags(NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_NOT_SOCIALUSE);
		GetFSMAgent()->EventProc();

	}
	else if(byState == STATE_IDLE_LOOP)
	{
		SetFlags(NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_RUNJUMP);

		//RwV3d vPos = m_pActor->GetPosition();

		//SWorldHeightStuff sHStuff;
		/*Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
		vPos.y = sHStuff.fFinialHeight;

		m_pActor->SetPosition(&vPos);*/
	}
}

// Recalculate the height.
void CNtlFSMCharActIdleState::UpdateCalcHeight( RwReal fElapsed ) 
{
    m_fCalcHeightTime += fElapsed;
    if(m_fCalcHeightTime > 0.5f)        // It is calculated in 0.5 second.
    {
        m_fCalcHeightTime = 0.0f;

        RwV3d vPos = m_pActor->GetPosition();
        SWorldHeightStuff sHStuff;
        Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);

        if(abs(vPos.y - sHStuff.fFinialHeight) > 0.3f) // If the difference in actual height is greater than 0.3
        {
            m_pActor->SetActorFlags(m_pActor->GetActorFlags() & ~SLFLAG_ACTOR_CALC_HEIGHT);
            Finish();
			SetNextStateId(NTL_FSMSID_FALLING);
        }
    }
}

void CNtlFSMCharActIdleState::UpdateTerror( RwReal fElapsed ) 
{
    // 자기 자신이거나, 자신의 펫인 경우에만 처리한다.
    if(m_pActor->GetClassID() == SLCLASS_AVATAR || m_pActor->GetOwnerID() == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
    {
        Logic_RandomMove(m_pActor, DBO_MAX_TERROR_MOVEMENT_DISTANCE);        
    }          
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActMoveState)

CNtlFSMCharActMoveState::CNtlFSMCharActMoveState()
{
	SetStateId(NTL_FSMSID_MOVE);
	SetStateName(NTL_FSMSN_MOVE);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP);

	m_hFrameSkipId		= INVAID_HFRAMESKIPID;
	m_fUpdateElapsed	= 0.0f;
}

CNtlFSMCharActMoveState::~CNtlFSMCharActMoveState()
{
	if(m_hFrameSkipId != INVAID_HFRAMESKIPID)
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		pFrameSkipScheduler->RemoveFrameSkip(m_hFrameSkipId);
		m_hFrameSkipId = INVAID_HFRAMESKIPID;
	}
}

void CNtlFSMCharActMoveState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// avatar or summon pet
	if( !(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER) )
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		m_hFrameSkipId = pFrameSkipScheduler->AddScheduler();
	}

	// It determines whether it is a fly move or a ground move.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

	if(pMoveStuff->byForm == NTL_MOVEFORM_GROUND)
	{
		if(m_pActor->GetFlags() & SLFLAG_CAMERASHAKE_ENABLE)
		{
			if(pMoveStuff->byType == NTL_MOVETARGET_DIR || pMoveStuff->byType == NTL_MOVETARGET_LOC ||
				pMoveStuff->byType == NTL_MOVETARGET_ACTOR)
			{
				CNtlSLEventGenerator::CameraSmoothingMove(TRUE);
			}
		}

        CNtlBehaviorCharGroundMove *pBehavior = NTL_NEW CNtlBehaviorCharGroundMove;
        AddBehavior(pBehavior);
	}
	else if (pMoveStuff->byForm == NTL_MOVEFORM_FLY)
	{
		if (m_pActor->GetFlags() & SLFLAG_CAMERASHAKE_ENABLE)
		{
			if (pMoveStuff->byType == NTL_MOVETARGET_DIR || pMoveStuff->byType == NTL_MOVETARGET_LOC ||
				pMoveStuff->byType == NTL_MOVETARGET_ACTOR)
			{
				CNtlSLEventGenerator::CameraSmoothingMove(TRUE);
			}
		}

		CNtlBehaviorCharAirMove *pBehavior = NTL_NEW CNtlBehaviorCharAirMove;
		AddBehavior(pBehavior);
	}
	else if(pMoveStuff->byForm == NTL_MOVEFORM_SPLINE)
	{
		CNtlBehaviorCharSplineMove *pBehavior = NTL_NEW CNtlBehaviorCharSplineMove;
		AddBehavior(pBehavior);
	}
    else if(pMoveStuff->byForm == NTL_MOVEFORM_BUS)
    {
		// Behavior가 이미 생성 되었있는 경우는 생성하지 않는다
		CNtlBehaviorBusMove *pBehavior = NTL_NEW CNtlBehaviorBusMove;
		AddBehavior(pBehavior);
    }
}

void CNtlFSMCharActMoveState::Exit(void)
{
	if(m_pActor->GetFlags() & SLFLAG_CAMERASHAKE_ENABLE)
	{
		CNtlSLEventGenerator::CameraSmoothingMove(FALSE);
	}

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActMoveState::Update(RwReal fElapsed)
{
	m_fUpdateElapsed += fElapsed;

	if(m_hFrameSkipId != INVAID_HFRAMESKIPID)
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		if(pFrameSkipScheduler->IsFrameSkip(m_hFrameSkipId))
			return;
	}

	fElapsed			= m_fUpdateElapsed;
	m_fUpdateElapsed	= 0.0f;


	CNtlFSMCharActStateBase::Update(fElapsed);

	if(!IsFinish())
	{
        // If it is water, go to the swimming state. (If there is no water, the Height is -9999.0f)
		if(GetSceneManager()->GetWorldWaterHeight(m_pActor->GetPosition()) > -9998.0f)
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
            SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
            RwUInt8 byType = pMoveStuff->byType;
            if(byType == NTL_MOVETARGET_DIR || byType == NTL_MOVETARGET_LOC ||
                byType == NTL_MOVETARGET_ACTOR)
            {
                RwV3d vPos = m_pActor->GetPosition();
                SWorldHeightStuff sHStuff;
                if(Logic_IsSwimmingActor(m_pActor, &vPos, sHStuff))
                {
					pMoveStuff->byForm = NTL_MOVEFORM_SWIMMING;

					Finish();
					SetNextStateId(NTL_FSMSID_SWIMMING);
                }
            }
        }
	}
	else
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
		if(pMoveStuff->byMoveResult & NTL_MOVE_RESULT_FALLING)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_FALLING);
		}
	}
}

RwUInt32 CNtlFSMCharActMoveState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActMoveState::HandleEvents");

	
	if(pMsg.Id == g_EventActionMapJump)
	{
		// event queue
		if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
		{
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
			SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
			if(pMoveStuff->byType == NTL_MOVETARGET_LOC)
			{
				AgentQueuingTerrainClick(GetFSMAgent(), pMoveStuff->vDest);
			}
		}
	}
	else if(pMsg.Id == g_EventActionMapHalt)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_IDLE);
	}
	else if(pMsg.Id == g_EventSobSecondDestMove)
	{
		// 버스인 경우
		if ( Logic_IsBus( m_pActor ) )
		{
			FSMEvent_CharActSobSecondDestMove( m_pActor, reinterpret_cast<SNtlEventSobSecondDestMove*>(pMsg.pData) ); 

			NTL_RETURN(CNtlFSMStateBase::HandleEvents(pMsg));
		}
	}
	
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActMoveSwimming)

CNtlFSMCharActMoveSwimming::CNtlFSMCharActMoveSwimming()
{
	SetStateId(NTL_FSMSID_SWIMMING);
	SetStateName(NTL_FSMSN_SWIMMING);
	SetFlags(NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SKILLUSE | 
			NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_ATTACK_CAN |
			NTL_FSMSF_NOT_HURT_CAN | NTL_FSMSF_NOT_RUNJUMP);
}

void CNtlFSMCharActMoveSwimming::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// fly move인지 ground move인지를 판단한다.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

	if(pMoveStuff->byForm == NTL_MOVEFORM_SWIMMING)
	{
		CNtlBehaviorCharSwimmingMove *pBehavior = NTL_NEW CNtlBehaviorCharSwimmingMove;
		AddBehavior(pBehavior);
	}
}

void CNtlFSMCharActMoveSwimming::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActMoveSwimming::Update(RwReal fElapsed)
{
	CNtlFSMCharActStateBase::Update(fElapsed);

	if(!IsFinish())
	{
		// If it is not water, go to the ground moving state.

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

		RwV3d vPos = m_pActor->GetPosition();
		SWorldHeightStuff sHStuff;
		if(!Logic_IsSwimmingActor(m_pActor, &vPos, sHStuff))
		{
			pMoveStuff->byForm		= NTL_MOVEFORM_GROUND;
			pMoveStuff->vCurrLoc	= vPos;

			Finish();
			SetNextStateId(NTL_FSMSID_MOVE);
		}
	}
}

RwUInt32 CNtlFSMCharActMoveSwimming::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActMoveSwimming::HandleEvents");

	if(pMsg.Id == g_EventMouseDashMove || pMsg.Id == g_EventKeyboardDashMove)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else if(pMsg.Id == g_EventActionMapHalt)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_IDLE);
	}

		
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActAdjustMoveState)

CNtlFSMCharActAdjustMoveState::CNtlFSMCharActAdjustMoveState()
{
	SetStateId(NTL_FSMSID_ADJUSTMOVE);
	SetStateName(NTL_FSMSN_ADJUSTMOVE);
	SetFlags(NTL_FSMSF_NOT_HURT_STATE);
}

void CNtlFSMCharActAdjustMoveState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// fly move인지 ground move인지를 판단한다.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

	// 유효 범위 검색.
	RwV3d vPos = m_pActor->GetPosition(); 
	RwV3d vDir;
	RwV3dSubMacro(&vDir, &pMoveStuff->vDest, &vPos);
	vDir.y = 0.0f;
	RwReal fLen = RwV3dLength(&vDir);
	if(fLen < 0.01f)
		return;

	pMoveStuff->byType = NTL_MOVETARGET_LOC;

	if (m_pActor->IsAirMode())
	{
		pMoveStuff->byForm = NTL_MOVEFORM_FLY;
		pMoveStuff->byFormFlag = NTL_MOVE_FLAG_FLY;
	}
	else
	{
		pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
		pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	}

	if(pMoveStuff->byForm == NTL_MOVEFORM_GROUND)
	{
		CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharGroundAdjustMove;
		AddBehavior(pBehavior);		
	}
	else if (pMoveStuff->byForm == NTL_MOVEFORM_FLY)
	{
	}    
	else
	{
	}
}

void CNtlFSMCharActAdjustMoveState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActDashState)

CNtlFSMCharActDashState::CNtlFSMCharActDashState()
{
	SetStateId(NTL_FSMSID_DASH);
	SetStateName(NTL_FSMSN_DASH);
	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE | 
			NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SIT | 
			NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_SKIP_ATTACK_CLICK);

	m_pGroundMove = NULL;
}

void CNtlFSMCharActDashState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// It determines whether it is a fly move or a ground move.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

	if(pMoveStuff->byForm == NTL_MOVEFORM_GROUND)
	{
		m_pGroundMove = NTL_NEW CNtlBehaviorCharGroundMove;
		AddBehavior(m_pGroundMove);
	}

// fluorspar
// There is a problem because we have to queue various actions during dash.
//    Logic_SetClickDisable(m_pActor, TRUE);
}

void CNtlFSMCharActDashState::Exit(void)
{
//    Logic_SetClickDisable(m_pActor, FALSE);
    
	if(m_pGroundMove)
		CNtlSLEventGenerator::TSSkillUse_Dash( m_pActor->GetSerialID() );

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActDashState::Update(RwReal fElapsed)
{
	CNtlFSMCharActStateBase::Update(fElapsed);

	if(GetBehaviorCount() > 0)
	{
		if(m_pGroundMove && m_pGroundMove->GetDashState() == CNtlBehaviorCharGroundMove::DASH_STATE_END)
		{
			if(GetFSMAgent()->IsQueuingEventExist())
			{
				Finish();
			}
			else
			{
//				Logic_SetClickDisable(m_pActor, FALSE);
				SetFlags(GetFlags() & ~NTL_FSMSF_NOT_MOVE);
			}
		}
	}
}

RwUInt32 CNtlFSMCharActDashState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActDashState::HandleEvents");

	if (m_pGroundMove)
	{
		if (pMsg.Id == g_EventKeyboardMove)
		{
			SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>(pMsg.pData);

			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
			SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

			pMoveStuff->byMoveFlags = pMoveStuff->byMoveFlags;

			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}
		else if (pMsg.Id == g_EventKeyboardMoveStop)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}
		else if (pMsg.Id == g_EventTerrainClick)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}
		else if (pMsg.Id == g_EventSobAttackSelect)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}
		else if (pMsg.Id == g_EventActionMapSkillUse)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}
		else if (pMsg.Id == g_EventActionMapHTBUse)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}
		else if (pMsg.Id == g_EventActionMapItemUse)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSitDownState)

CNtlFSMCharActSitDownState::CNtlFSMCharActSitDownState()
{
	SetStateId(NTL_FSMSID_SITDOWN);
	SetStateName(NTL_FSMSN_SITDOWN);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_SKIP_SITDOWN | NTL_FSMSF_NOT_CAHNGE_HEADING);
}

void CNtlFSMCharActSitDownState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// animation을 setting 한다.
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
	if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
		pSobProxy->SetBaseAnimation(NML_STAFF_SIT_DOWN, FALSE);
	else
		pSobProxy->SetBaseAnimation(SIT_DOWN, FALSE);
}

void CNtlFSMCharActSitDownState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActSitDownState::Update(RwReal fElapsed)
{
	// 여기에서는 behavior가 없기 때문에 굳이 state를 업데이트 할 필요가 없다.
//	CNtlFSMCharActStateBase::Update(fElapsed); 
}

RwUInt32 CNtlFSMCharActSitDownState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSitDownState::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_SIT);
	}
		
	else if(pMsg.Id == g_EventActionMapSitAndStand)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventActionMapCharging)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSitState)

CNtlFSMCharActSitState::CNtlFSMCharActSitState()
{
	SetStateId(NTL_FSMSID_SIT);
	SetStateName(NTL_FSMSN_SIT);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_SKIP_SITDOWN | NTL_FSMSF_NOT_CAHNGE_HEADING);
}

void CNtlFSMCharActSitState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// animation을 setting 한다.
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
	if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
		pSobProxy->SetBaseAnimation(NML_STAFF_SIT_IDLE, TRUE);
	else
		pSobProxy->SetBaseAnimation(SIT_IDLE, TRUE);
}

void CNtlFSMCharActSitState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActSitState::Update(RwReal fElapsed)
{
	// 여기에서는 behavior가 없기 때문에 굳이 state를 업데이트 할 필요가 없다.
//	CNtlFSMCharActStateBase::Update(fElapsed); 
}

RwUInt32 CNtlFSMCharActSitState::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventActionMapCharging)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActStandUpState)

CNtlFSMCharActStandUpState::CNtlFSMCharActStandUpState()
{
	SetStateId(NTL_FSMSID_STANDUP);
	SetStateName(NTL_FSMSN_STANDUP);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_CAHNGE_HEADING);
}

void CNtlFSMCharActStandUpState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// animation을 setting 한다.
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
	if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
		pSobProxy->SetBaseAnimation(NML_STAFF_SIT_UP, FALSE);
	else
		pSobProxy->SetBaseAnimation(SIT_UP, FALSE);
}

void CNtlFSMCharActStandUpState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActStandUpState::Update(RwReal fElapsed)
{
	// 여기에서는 behavior가 없기 때문에 굳이 state를 업데이트 할 필요가 없다.
//	CNtlFSMCharActStateBase::Update(fElapsed); 
}

RwUInt32 CNtlFSMCharActStandUpState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStandUpState::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		Finish();
	}

	else if(pMsg.Id == g_EventActionMapSitAndStand)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventActionMapCharging)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSmoothHeadingState)

CNtlFSMCharActSmoothHeadingState::CNtlFSMCharActSmoothHeadingState()
{
	SetStateId(NTL_FSMSID_SMOOTH_HEADING);
	SetStateName(NTL_FSMSN_SMOOTH_HEADING);

	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_RUNJUMP);
}

void CNtlFSMCharActSmoothHeadingState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	//

	CNtlBehaviorSmoothHeading *pBehavior = NTL_NEW CNtlBehaviorSmoothHeading;
	AddBehavior(pBehavior);
}

void CNtlFSMCharActSmoothHeadingState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActSmoothHeadingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSmoothHeadingState::HandleEvents");

	if(pMsg.Id == g_EventInputChangeHeading)
	{
		SNtlEventInputChangeHading *pChangeHeading = reinterpret_cast<SNtlEventInputChangeHading*>( pMsg.pData );
		if(pChangeHeading->bSoothing)
		{
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
			pCtrlStuff->uExtra.sRotate.fDeltaAngleY = pChangeHeading->fDeltaAngleY;

			pCtrlStuff->uExtra.sRotate.bSetAngleX = pChangeHeading->bSetAngleX;
			pCtrlStuff->uExtra.sRotate.fDeltaAngleX = pChangeHeading->fDeltaAngleX;

			UpdateBehavior();

			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		
	}
	
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActTriggerOperateState)

CNtlFSMCharActTriggerOperateState::CNtlFSMCharActTriggerOperateState()
{
	SetStateId(NTL_FSMSID_TRIGGER_OPERATE);
	SetStateName(NTL_FSMSN_TRIGGER_OPERATE);

	SetFlags(NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT);
}

void CNtlFSMCharActTriggerOperateState::SetAnim(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 
	CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId) );

	if(pTarActor)
	{
		CDirectionLinkTable *pDirectLinkTbl = API_GetTableContainer()->GetDirectionLinkTable();
		sDIRECTION_LINK_TBLDAT *pDirectLinkTblData = reinterpret_cast<sDIRECTION_LINK_TBLDAT*>( pDirectLinkTbl->FindData(pCtrlStuff->uExtra.sOperate.uiTblId) );
		if(pDirectLinkTblData)
		{
			CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();

			if(pDirectLinkTblData->dwAnimationID == INVALID_DWORD)
				pSobProxy->SetBaseAnimation(NML_IDLE_LOOP);
			else
				pSobProxy->SetBaseAnimation(pDirectLinkTblData->dwAnimationID);
		}

		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vTarPos = pTarActor->GetPosition();
		RwV3d vDir;

		RwV3dSubMacro(&vDir, &vTarPos, &vPos);
		vDir.y = 0.0f; 

		RwV3dNormalize(&vDir, &vDir);

		m_pActor->SetDirection(&vDir);
	}
}

void CNtlFSMCharActTriggerOperateState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	m_hTargetSerialId = pCtrlStuff->uExtra.sOperate.hTargetSerialId;

	// animation
	SetAnim();
}

void CNtlFSMCharActTriggerOperateState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActTriggerOperateState::Update(RwReal fElapsed)
{
	// CNtlFSMCharActStateBase::Update 함수를 호출하지 않는다.
	// 자동으로 상태가 끝내는 것을 없앤다..
}

RwUInt32 CNtlFSMCharActTriggerOperateState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActTriggerOperateState::HandleEvents");
	
	if(pMsg.Id == g_EventSobStanding)
	{
		Finish();
        SetNextStateId(NTL_FSMSID_IDLE);        

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}

	else if(pMsg.Id == g_EventSobTargetSelect)
	{
		SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>(pMsg.pData);
	
		if(m_hTargetSerialId == pSobTargetSelect->hSerialId)
		{
			pSobTargetSelect->bSuccess = FALSE;
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
	}
	
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActJumpState)

CNtlFSMCharActJumpState::CNtlFSMCharActJumpState()
{
	SetStateId(NTL_FSMSID_JUMP);
	SetStateName(NTL_FSMSN_JUMP);
	SetFlags(NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_MOVE | 
			NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_NOT_GROUNDMOVE);

	m_hFrameSkipId		= INVAID_HFRAMESKIPID;
	m_fUpdateElapsed	= 0.0f;
}

CNtlFSMCharActJumpState::~CNtlFSMCharActJumpState()
{
	if(m_hFrameSkipId != INVAID_HFRAMESKIPID)
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		pFrameSkipScheduler->RemoveFrameSkip(m_hFrameSkipId);
		m_hFrameSkipId = INVAID_HFRAMESKIPID;
	}
}

void CNtlFSMCharActJumpState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// avatar or summon pet
	if( !(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER) )
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		m_hFrameSkipId = pFrameSkipScheduler->AddScheduler();
	}

	CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharJump;
	AddBehavior(pBehavior);

	m_bMoveQueueing = FALSE;

    if(m_pActor->GetClassID() == SLCLASS_PLAYER)
    {
        SetFlags(GetFlags() & ~NTL_FSMSF_NOT_JUMP);
    }
}

void CNtlFSMCharActJumpState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActJumpState::Update(RwReal fElapsed)
{
	m_fUpdateElapsed += fElapsed;

	if(m_hFrameSkipId != INVAID_HFRAMESKIPID)
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		if(pFrameSkipScheduler->IsFrameSkip(m_hFrameSkipId))
			return;
	}

	fElapsed			= m_fUpdateElapsed;
	m_fUpdateElapsed	= 0.0f;

	CNtlFSMCharActStateBase::Update(fElapsed);
}

RwUInt32 CNtlFSMCharActJumpState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActJumpState::HandleEvents");

	if(pMsg.Id == g_EventKeyboardMove)
	{
		SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>( pMsg.pData );

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();		
		
		BYTE byNewMoveFlag = JumpKeyMapping(pMoveStuff->byMoveFlags, pKeyboardMove->byMoveFlags);

	//	DBO_WARNING_MESSAGE("CNtlFSMCharActJumpState::g_EventKeyboardMove: pMoveStuff->byMoveFlags: " << (int)pMoveStuff->byMoveFlags << " pKeyboardMove->byMoveFlags: " << (int)pKeyboardMove->byMoveFlags << " byNewMoveFlag: " << (int)byNewMoveFlag);

		pMoveStuff->byMoveFlags = byNewMoveFlag;
		
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}
	else if(pMsg.Id == g_EventKeyboardMoveStop)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

		BYTE byNewMoveFlag = NTL_MOVE_NONE;

		if(pMoveStuff->byMoveFlags != NTL_MOVE_NONE)
		{
			byNewMoveFlag = JumpKeyStopMapping(pMoveStuff->byMoveFlags);
		}

	//	DBO_WARNING_MESSAGE("g_EventKeyboardMoveStop: byMoveFlags: " << (int)pMoveStuff->byMoveFlags << " byNewMoveFlag: " << (int)byNewMoveFlag);

		pMoveStuff->byMoveFlags = byNewMoveFlag;
	
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}
	else if(pMsg.Id == g_EventMouseDashMove)
	{
		FSMEvent_CharActMouseDashMove(m_pActor, reinterpret_cast<SNtlEventMouseDashMove*>(pMsg.pData)); 

		Finish();
		SetNextStateId(NTL_FSMSID_DASH);

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}
	else if(pMsg.Id == g_EventKeyboardDashMove)
	{
		FSMEvent_CharActKeyboardDashMove(m_pActor, reinterpret_cast<SNtlEventKeyboardDashMove*>(pMsg.pData)); 

		Finish();
		SetNextStateId(NTL_FSMSID_DASH);

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}
	else if(pMsg.Id == g_EventSobMove)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
		pMoveStuff->Reset();

		SNtlEventSobMove *pMove = reinterpret_cast<SNtlEventSobMove*>( pMsg.pData );

		if(m_bMoveQueueing)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}

		if(!pMove->bMouseMove)
		{
			pMoveStuff->byType = NTL_MOVETARGET_DIR;

			if (m_pActor->IsAirMode())
			{
				pMoveStuff->byForm = NTL_MOVEFORM_FLY;
			}
			else
			{
				pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
			}

			pMoveStuff->byFormFlag = pMove->byFormFlag;
			pMoveStuff->byMoveFlags = pMove->byMoveFlags;
		}

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else if(pMsg.Id == g_EventSobMoveStop)
	{
		if(m_bMoveQueueing)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		}

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventInputChangeHeading)
	{
		SNtlEventInputChangeHading *pChangeHeading = reinterpret_cast<SNtlEventInputChangeHading*>( pMsg.pData );
		if(!pChangeHeading->bSoothing)
		{
			m_pActor->SetDeltaAngleY(pChangeHeading->fDeltaAngleY);

			if(pChangeHeading->bSetAngleX)
				m_pActor->SetDeltaAngleX(pChangeHeading->fDeltaAngleX);

			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventSobJumpEnd)
	{
		// Because you are in a jump state .. If you tell me to finish the jump ... I can not finish it right now.
		// So if you get a move command during the jump ... you have to queue it.

		m_bMoveQueueing = TRUE;

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else if(pMsg.Id == g_EventSobDirectionFloat)
	{
		SNtlEventSobDirectionFloat *pFloat = reinterpret_cast<SNtlEventSobDirectionFloat*>(pMsg.pData);

		FSMEvent_CharActSobDirectionFloat(m_pActor, pFloat);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActAirJumpState)

CNtlFSMCharActAirJumpState::CNtlFSMCharActAirJumpState()
{
	SetStateId(NTL_FSMSID_AIR_JUMP);
	SetStateName(NTL_FSMSN_AIR_JUMP);

	// TO DO
	SetFlags(NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE |
		NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_NOT_GROUNDMOVE);

	m_hFrameSkipId = INVAID_HFRAMESKIPID;
	m_fUpdateElapsed = 0.0f;
}

CNtlFSMCharActAirJumpState::~CNtlFSMCharActAirJumpState()
{
	if (m_hFrameSkipId != INVAID_HFRAMESKIPID)
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		pFrameSkipScheduler->RemoveFrameSkip(m_hFrameSkipId);
		m_hFrameSkipId = INVAID_HFRAMESKIPID;
	}
}

void CNtlFSMCharActAirJumpState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// avatar or summon pet
	if (!(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER))
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		m_hFrameSkipId = pFrameSkipScheduler->AddScheduler();
	}

	CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharAirMove;
	AddBehavior(pBehavior);
}

void CNtlFSMCharActAirJumpState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActAirJumpState::Update(RwReal fElapsed)
{
	m_fUpdateElapsed += fElapsed;

	if (m_hFrameSkipId != INVAID_HFRAMESKIPID)
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		if (pFrameSkipScheduler->IsFrameSkip(m_hFrameSkipId))
			return;
	}

	fElapsed = m_fUpdateElapsed;
	m_fUpdateElapsed = 0.0f;

	CNtlFSMCharActStateBase::Update(fElapsed);
}

RwUInt32 CNtlFSMCharActAirJumpState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActAirJumpState::HandleEvents");

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActFallingState)

CNtlFSMCharActFallingState::CNtlFSMCharActFallingState()
{
	SetStateId(NTL_FSMSID_FALLING);
	SetStateName(NTL_FSMSN_FALLING);
	SetFlags(NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_MOVE | 
			NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_ATTACK_CAN);
}

void CNtlFSMCharActFallingState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	//---------------------------------------------------------

	// event queue
	if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER && m_pActor->IsAirMode() == FALSE) // dont do the following if flying
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
		if(pMoveStuff->byType == NTL_MOVETARGET_LOC)
		{
			AgentQueuingTerrainClick(GetFSMAgent(), pMoveStuff->vDest);
		}
		else if(pMoveStuff->byType == NTL_MOVETARGET_DIR)
		{
			AgentQueuingKeyboardMove(GetFSMAgent(), pMoveStuff->byMoveFlags);
		}
	}

	CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharFalling;
	AddBehavior(pBehavior);
}

void CNtlFSMCharActFallingState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActFallingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActFallingState::HandleEvents");

	if(pMsg.Id == g_EventKeyboardMove)
	{
		SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>( pMsg.pData );

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

		pMoveStuff->byMoveFlags = JumpKeyMapping(pMoveStuff->byMoveFlags, pKeyboardMove->byMoveFlags);
		
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}
	else if(pMsg.Id == g_EventKeyboardMoveStop)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
		pMoveStuff->byType = NTL_MOVETARGET_NONE;
		pMoveStuff->byMoveFlags = NTL_MOVE_NONE;
	
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}
	else if(pMsg.Id == g_EventSobMove)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
		
	}
	else if(pMsg.Id == g_EventSobMoveStop)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}

	else if(pMsg.Id == g_EventInputChangeHeading)
	{
		SNtlEventInputChangeHading *pChangeHeading = reinterpret_cast<SNtlEventInputChangeHading*>( pMsg.pData );
		if(!pChangeHeading->bSoothing)
		{
			m_pActor->SetDeltaAngleY(pChangeHeading->fDeltaAngleY);

			if (pChangeHeading->bSetAngleX)
				m_pActor->SetDeltaAngleX(pChangeHeading->fDeltaAngleX);
		}

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else if(pMsg.Id == g_EventSobDirectionFloat)
	{
		SNtlEventSobDirectionFloat *pFloat = reinterpret_cast<SNtlEventSobDirectionFloat*>(pMsg.pData);

		FSMEvent_CharActSobDirectionFloat(m_pActor, pFloat);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActChargingState)

CNtlFSMCharActChargingState::CNtlFSMCharActChargingState()
{
	SetStateId(NTL_FSMSID_CHARGING);
	SetStateName(NTL_FSMSN_CHARGING);
	SetFlags(NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_CAHNGE_HEADING |
			NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_SKIP_ATTACK_CLICK);

	m_bCharging = TRUE;
	m_fTime		= 0.0f;
}

void CNtlFSMCharActChargingState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
	pSobProxy->SetBaseAnimation(NML_CHARGE_START, FALSE);

	if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
	{
		CNtlSLEventGenerator::ChargingNfy(TRUE);
	}

	pSobProxy->CreateRpChargeEffect();
}

void CNtlFSMCharActChargingState::Exit(void)
{
	if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
	{
		CNtlSLEventGenerator::ChargingNfy(FALSE);
	}

	CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
	pSobProxy->DeleteRpChargeEffect();

	CNtlSLEventGenerator::TSSkillUse_Charging( m_pActor->GetSerialID() );

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActChargingState::Update(RwReal fElapsed)
{
	// It does not process anything, so it does not exit the state.

	if(!m_bCharging)
	{
		m_fTime += fElapsed;
		if(m_fTime >= 2.0f)
		{
			Finish();
		}
	}
}

RwUInt32 CNtlFSMCharActChargingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActChargingState::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
		if(pAnimEnd->uiBaseAnimKey == NML_CHARGE_START)
		{
			CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
			pSobProxy->SetBaseAnimation(NML_CHARGE_LOOP, TRUE);
		}
		else if(pAnimEnd->uiBaseAnimKey == NML_CHARGE_END)
		{
			Finish();
		}
	}
	else if(pMsg.Id == g_EventActionMapCharging)
	{
		SNtlEventActionMapCharging *pCharging  = reinterpret_cast<SNtlEventActionMapCharging*>( pMsg.pData );
		if(pCharging->bStartOrEnd)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		else
		{
			if(m_bCharging)
			{
				m_bCharging = FALSE;
				CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
				pSobProxy->SetBaseAnimation(NML_CHARGE_END, FALSE);
				SetFlags(NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_RUNJUMP);
				NTL_RETURN(NTL_FSM_EVENTRES_PASS);
			}
		}
	}
	else if(pMsg.Id == g_EventSobStanding)
	{
		m_bCharging = FALSE;
		CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
		pSobProxy->SetBaseAnimation(NML_CHARGE_END, FALSE);
		SetFlags(NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_RUNJUMP);
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActBlockingState)

CNtlFSMCharActBlockingState::CNtlFSMCharActBlockingState()
{
	SetStateId(NTL_FSMSID_BLOCKING);
	SetStateName(NTL_FSMSN_BLOCKING);
	SetFlags(NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_CAHNGE_HEADING |
			NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_SKIP_ATTACK_CLICK);

	m_bEquipedStuff	= FALSE;
	m_bGuardCrush	= FALSE;
}

void CNtlFSMCharActBlockingState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
	
	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
	if(pItemTblData)
		m_bEquipedStuff = Logic_IsEquipedStaffWeapon(pItemTblData);

	if(m_bEquipedStuff)
		pSobProxy->SetBaseAnimation(BTL_STAFF_BLOCK_START, FALSE);
	else
		pSobProxy->SetBaseAnimation(BTL_DEF_BLOCK_START, FALSE);

	pSobProxy->CreateGuardEffect();
}

void CNtlFSMCharActBlockingState::Exit(void)
{
	CNtlSLEventGenerator::TSSkillUse_Guard( m_pActor->GetSerialID() );

	CNtlSobCharProxy* pProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
	pProxy->DeleteGuardEffect();

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActBlockingState::Update(RwReal fElapsed)
{
	// It does not process anything, so it does not exit the state.

	if(m_bGuardCrush)
	{
		m_fCrushTime += fElapsed;
		if(m_fCrushTime >= 0.5f)
		{
			RwUInt32 uiFlags = GetFlags();
			uiFlags = uiFlags & (~NTL_FSMSF_NOT_HURT_STATE);
			SetFlags(uiFlags);
		}
	}
}

RwUInt32 CNtlFSMCharActBlockingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActBlockingState::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
		if(pAnimEnd->uiBaseAnimKey == BTL_DEF_BLOCK_END || pAnimEnd->uiBaseAnimKey == BTL_STAFF_BLOCK_END ||
			pAnimEnd->uiBaseAnimKey == BTL_DEF_GUARD_CRUSH)
		{
			Finish();
		}
	}
	else if(pMsg.Id == g_EventSobStanding)
	{
		if(m_bGuardCrush)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}

		CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
		if(m_bEquipedStuff)
			pSobProxy->SetBaseAnimation(BTL_STAFF_BLOCK_END, FALSE);
		else
			pSobProxy->SetBaseAnimation(BTL_DEF_BLOCK_END, FALSE);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else if(pMsg.Id == g_EventSobGuardCrush)
	{
		CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
		pSobProxy->SetBaseAnimation(BTL_DEF_GUARD_CRUSH, FALSE);

		m_bGuardCrush	= TRUE;
		m_fCrushTime	= 0.0f;

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventActionMapBlocking)
	{
		SNtlEventActionMapBlocking *pBlocking  = reinterpret_cast<SNtlEventActionMapBlocking*>( pMsg.pData );
		if(pBlocking->bStartOrEnd)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		else
		{
			CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
			if( !(pSobProxy->GetBaseAnimationKey() == BTL_STAFF_BLOCK_END ||
				  pSobProxy->GetBaseAnimationKey() == BTL_DEF_BLOCK_END) )
			{
				if(m_bEquipedStuff)
					pSobProxy->SetBaseAnimation(BTL_STAFF_BLOCK_END, FALSE);
				else
					pSobProxy->SetBaseAnimation(BTL_DEF_BLOCK_END, FALSE);

				NTL_RETURN(NTL_FSM_EVENTRES_PASS);
			}
		}
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActFollowState)

CNtlFSMCharActFollowState::CNtlFSMCharActFollowState()
{
	SetStateId(NTL_FSMSID_FOLLOW);
	SetStateName(NTL_FSMSN_FOLLOW);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);

    m_byMoveForm = NTL_MOVEFORM_GROUND;
}

void CNtlFSMCharActFollowState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// fly move인지 ground move인지를 판단한다.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId));

	if(Logic_InFollowRange(m_pActor, pTargetActor, pCtrlStuff->fFollowRange))
	{
		Finish();
		return;
	}

	if(pMoveStuff->byForm == NTL_MOVEFORM_GROUND)
	{
		CNtlBehaviorCharGroundMove *pBehavior = NTL_NEW CNtlBehaviorCharGroundMove;
		AddBehavior(pBehavior);
	}
	else if(pMoveStuff->byForm == NTL_MOVEFORM_SWIMMING)
	{
        CNtlBehaviorCharSwimmingMove* pBehavior = NTL_NEW CNtlBehaviorCharSwimmingMove;
        AddBehavior(pBehavior);
	}
	else if (pMoveStuff->byForm == NTL_MOVEFORM_FLY)
	{
		CNtlBehaviorCharAirMove *pBehavior = NTL_NEW CNtlBehaviorCharAirMove;
		AddBehavior(pBehavior);
	}

    m_byMoveForm = pMoveStuff->byForm;
}

void CNtlFSMCharActFollowState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActFollowState::Update(RwReal fElapsed)
{
	CNtlFSMCharActStateBase::Update(fElapsed);

    // It grasps the land or water and changes the behavior.
    SWorldHeightStuff hStuff;
    if(Logic_IsSwimmingActor(m_pActor, &m_pActor->GetPosition(), hStuff))
    {
        if(m_byMoveForm != NTL_MOVEFORM_SWIMMING)
        {
            RemoveAllBehavior();
            CNtlBehaviorCharSwimmingMove* pBehavior = NTL_NEW CNtlBehaviorCharSwimmingMove;
            AddBehavior(pBehavior);
            m_byMoveForm = NTL_MOVEFORM_SWIMMING;
        }
    }
    else
    {
        if(m_byMoveForm != NTL_MOVEFORM_GROUND)
        {
            RemoveAllBehavior();
            CNtlBehaviorCharGroundMove *pBehavior = NTL_NEW CNtlBehaviorCharGroundMove;
            AddBehavior(pBehavior);
            m_byMoveForm = NTL_MOVEFORM_GROUND;
        }
    }

	if(IsFinish())
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 
		CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId) );
		if(pTarActor)
		{
			if(Logic_IsOperationTriggerActor(pTarActor))
			{
				pCtrlStuff->uExtra.sOperate.uiOperateTime	= 0;
				pCtrlStuff->uExtra.sOperate.uiTblId			= Logic_GetDirectIdTriggerActor(pTarActor);

				SetNextStateId(NTL_FSMSID_TRIGGER_OPERATE);
			}
		}
	}
}

RwUInt32 CNtlFSMCharActFollowState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActFollowState::HandleEvents");
	
	
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActFightingFollowState)

CNtlFSMCharActFightingFollowState::CNtlFSMCharActFightingFollowState()
{
	SetStateId(NTL_FSMSID_FIGHTING_FOLLOW);
	SetStateName(NTL_FSMSN_FIGHTING_FOLLOW);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
}

void CNtlFSMCharActFightingFollowState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// fly move인지 ground move인지를 판단한다.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
	pMoveStuff->hFollowSerialId = pCtrlStuff->hTargetSerialId;
	pMoveStuff->fFollowRange = pCtrlStuff->fFollowRange;

	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId));
	if(Logic_InFollowRange(m_pActor, pTargetActor, pCtrlStuff->fFollowRange))
	{
		Finish();
		return;
	}

    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        API_GetSLPacketGenerator()->SendCharFollowMove(pCtrlStuff->hTargetSerialId, pCtrlStuff->fFollowRange, DBO_MOVE_FOLLOW_AUTO_ATTACK, m_pActor->GetServerSyncAvatarType());
        CNtlSLEventGenerator::SobSendAttackEnd(m_pActor->GetSerialID());
    }

	if(pMoveStuff->byForm == NTL_MOVEFORM_GROUND)
	{
		CNtlBehaviorCharGroundMove *pBehavior = NTL_NEW CNtlBehaviorCharGroundMove;
		AddBehavior(pBehavior);
	}
	else
	{
	}

    m_byMoveForm = pMoveStuff->byForm;

    // 어택모드를 켠다.
    CNtlSLEventGenerator::SobAutoAttackMode(m_pActor->GetSerialID(), TRUE);    
}

void CNtlFSMCharActFightingFollowState::Exit(void)
{
    // 어택 모드를 끈다.
    CNtlSLEventGenerator::SobAutoAttackMode(m_pActor->GetSerialID(), FALSE);

	CNtlFSMCharActStateBase::Exit();
}


RwUInt32 CNtlFSMCharActFightingFollowState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActFightingFollowState::HandleEvents");

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

void CNtlFSMCharActFightingFollowState::Update( RwReal fElapsed ) 
{
    CNtlFSMCharActStateBase::Update(fElapsed);

    // 육지인지 물속인지 파악하여 행동을 변경하여 준다.
    SWorldHeightStuff hStuff;
    if(Logic_IsSwimmingActor(m_pActor, &m_pActor->GetPosition(), hStuff))
    {
        if(m_byMoveForm != NTL_MOVEFORM_SWIMMING)
        {
            RemoveAllBehavior();
            CNtlBehaviorCharSwimmingMove* pBehavior = NTL_NEW CNtlBehaviorCharSwimmingMove();
            AddBehavior(pBehavior);
            m_byMoveForm = NTL_MOVEFORM_SWIMMING;
        }
    }
    else
    {
        if(m_byMoveForm != NTL_MOVEFORM_GROUND)
        {
            RemoveAllBehavior();
            CNtlBehaviorCharGroundMove *pBehavior = NTL_NEW CNtlBehaviorCharGroundMove;
            AddBehavior(pBehavior);
            m_byMoveForm = NTL_MOVEFORM_GROUND;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActFightingPoseState)

CNtlFSMCharActFightingPoseState::CNtlFSMCharActFightingPoseState()
{
	SetStateId(NTL_FSMSID_FIGHTING_POSE);
	SetStateName(NTL_FSMSN_FIGHTING_POSE);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_SKIP_STANDUP);
}

void CNtlFSMCharActFightingPoseState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// time setting
	m_fTime = 0.0f;
    m_bIsPlayInterAni	= FALSE;
	m_bAdjustMoveCheck	= FALSE;
		
	// animation setting
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
    // 변신때는 애니메이션을 안하고 바로 Idle로 간다.
    if(Logic_IsTransformGreatNamek(m_pActor))
    {
        pSobProxy->SetBaseAnimation(TRANS_IDLE);        
    }    
	else if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
		pSobProxy->SetBaseAnimation(BTL_STAFF_FP_LOOP);
	else
		pSobProxy->SetBaseAnimation(BTL_DEF_FP_LOOP);

	// ground fly를 위해서
	// 위치를 다시한번 셋팅한다.
	RwV3d vPos = m_pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
	vPos.y = sHStuff.fFinialHeight;

	m_pActor->SetPosition(&vPos);

	// 위치 보정을 해야하는 class인지를 check한다.
	if(!FSMUtil_IsStandingAdjustMoveClass(m_pActor))
	{
		m_bAdjustMoveCheck = TRUE;
	}
}

void CNtlFSMCharActFightingPoseState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActFightingPoseState::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

	m_fTime += fElapsed;

	// Coordinate correction check.
	/*CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(GetFSMAgent());
	if(pCharActAgent->IsActiveAdjustPosition() && !m_bAdjustMoveCheck)
	{
		if(m_fTime > 0.3f)
		{
			if(FSMUtil_StandingAdjustMove(this, m_pActor))
			{
				Finish();
				SetNextStateId(NTL_FSMSID_MOVE);
				return;
			}

			m_bAdjustMoveCheck = TRUE;
		}
	}*/
	

	if(m_fTime >= 1.0f)
	{
		CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(GetFSMAgent());
		if(!pCharActAgent->IsFightingMode())
		{
			SetPose2IdleAnim();			
			return;
		}

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

		CNtlSobActor *pTarSobActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId) );
		if(pTarSobActor == NULL)
		{
            SetPose2IdleAnim();			
			return;
		}

		if(pTarSobActor->IsDie())
		{
			SetPose2IdleAnim();
			return;
		}

		CNtlFSMStateBase *pBaseState = pTarSobActor->GetCurrentState();
		if(pBaseState == NULL)	
		{
			SetPose2IdleAnim();
			return;
		}

		RwUInt32 uiStateId = pBaseState->GetStateId();
		if(uiStateId == NTL_FSMSID_FAINTING || uiStateId == NTL_FSMSID_DIE)
		{
			SetPose2IdleAnim();
			return;
		}

		if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
		{
			if(!Logic_IsEnemyTargetFromActor(m_pActor, pCtrlStuff->hTargetSerialId))
			{
				SetPose2IdleAnim();
				return;
			}
		}

		m_fTime = 0.0f;
	}

	// Since there is no behavior here, it is not necessary to update the state.
	//	CNtlFSMCharActStateBase::Update(fElapsed); 

}

void CNtlFSMCharActFightingPoseState::SetPose2IdleAnim() 
{
    if(m_bIsPlayInterAni)
        return;

	RwUInt32 uiClassId = m_pActor->GetClassID();

    if(uiClassId == SLCLASS_AVATAR || uiClassId == SLCLASS_PLAYER)
    {
        CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());            
        sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
        // When it transforms, it goes to Idle immediately without animation.        
        if(Logic_IsTransformGreatNamek(m_pActor))        
        {
            pSobProxy->SetBaseAnimation(TRANS_IDLE);
            Finish();
            SetNextStateId(NTL_FSMSID_IDLE);
        }
        else if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))    // While holding a staff
        {
            pSobProxy->SetBaseAnimation(BTL_STAFF_FP_IDLE, FALSE);
        }
        else    // When you are not carrying anything
        {
            pSobProxy->SetBaseAnimation(BTL_DEF_FP_IDLE, FALSE);   
        }        
    }
	else
	{
		Finish();
        SetNextStateId(NTL_FSMSID_IDLE);
	}

    m_bIsPlayInterAni = TRUE;
}

RwUInt32 CNtlFSMCharActFightingPoseState::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventAnimEnd)
    {
        SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
        if(pAnimEnd->uiBaseAnimKey == BTL_DEF_FP_IDLE ||
           pAnimEnd->uiBaseAnimKey == BTL_GUN_FP_IDLE ||
           pAnimEnd->uiBaseAnimKey == BTL_STAFF_FP_IDLE || 
           pAnimEnd->uiBaseAnimKey == BTL_DGUN_FP_IDLE)
        {
            Finish();
            SetNextStateId(NTL_FSMSID_IDLE);
        }
    }
	
    NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CAnimHitDelay)

CAnimHitDelay::CAnimHitDelay()
{
	m_eState	= EAHD_NONE;
	m_pActor	= NULL;
	m_fCurrTime	= 0.0f;
	m_fHitDelay	= 0.1f;
}

CAnimHitDelay::~CAnimHitDelay()
{
	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	pPLCharacter->SetAnimUpdate(TRUE);
}

void CAnimHitDelay::EnterState(EAnimHitDelayState eState)
{
	switch(eState)
	{
	case EAHD_PAUSE:
		{
			m_fCurrTime	= 0.0f;
			CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetAnimUpdate(FALSE);
		}
		break;
	case EAHD_FAST:
		{
			m_fCurrTime	= 0.0f;
			CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetAnimUpdate(TRUE);

			RwReal fAnimSpeed = pPLCharacter->GetAnimSpeed();
			fAnimSpeed = fAnimSpeed * 2.0f;

			pPLCharacter->SetAnimSpeed(fAnimSpeed);
		}
		break;
	case EAHD_FINISH:
		{   
			m_fCurrTime	= 0.0f;
			CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetAnimUpdate(TRUE);

			RwReal fAnimSpeed = pPLCharacter->GetAnimSpeed();
		//	fAnimSpeed = fAnimSpeed / 2.0f;

			pPLCharacter->SetAnimSpeed(fAnimSpeed);
		}
		break;
	}
}

void CAnimHitDelay::ExitState(EAnimHitDelayState eState)
{
	switch(eState)
	{
	case EAHD_PAUSE:
		break;
	case EAHD_FAST:
		break;
	case EAHD_FINISH:
		break;
	}
}


void CAnimHitDelay::ChangeState(EAnimHitDelayState eState)
{
	ExitState(m_eState);

	EnterState(eState);

	m_eState = eState;
}


void CAnimHitDelay::Update(RwReal fElapsed)
{
	if(m_eState == EAHD_PAUSE)
	{
		m_fCurrTime += fElapsed;

		if(m_fCurrTime >= m_fHitDelay)
		{
			ChangeState(EAHD_FAST);
		}
	}
	else if(m_eState == EAHD_FAST)
	{
		m_fCurrTime += fElapsed;

		if(m_fCurrTime >= m_fHitDelay)
		{
			ChangeState(EAHD_FINISH);
		}
	}
}

RwBool CAnimHitDelay::IsFinish(void)
{
	if(m_eState == EAHD_FINISH)
		return TRUE;

	return FALSE;
}

void CAnimHitDelay::SetAnimHitDelay(CNtlSobActor *pActor, RwReal fDelayTime)
{
	m_pActor	= pActor;
	m_fHitDelay = fDelayTime;

	ChangeState(EAHD_PAUSE);
}



DEFINITION_MEMORY_POOL(CNtlFSMCharActFightingState)

CNtlFSMCharActFightingState::CNtlFSMCharActFightingState()
{
	SetStateId(NTL_FSMSID_FIGHTING);
	SetStateName(NTL_FSMSN_FIGHTING);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_ADJUSTMOVE |  
			NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
}

void CNtlFSMCharActFightingState::Enter(void)
{
	NTL_FUNCTION("CNtlFSMCharActFightingState::Enter");

	CNtlFSMCharActStateBase::Enter();

	CNtlBehaviorCharNormalAttack *pNormalAttackBehavior = NTL_NEW CNtlBehaviorCharNormalAttack;
	AddBehavior(pNormalAttackBehavior);

    // Turn on attack mode.
    CNtlSLEventGenerator::SobAutoAttackMode(m_pActor->GetSerialID(), TRUE);

	NTL_RETURNVOID();
}

void CNtlFSMCharActFightingState::Exit(void)
{
	NTL_FUNCTION("CNtlFSMCharActFightingState::Exit");

    // Turn off attack mode.
    CNtlSLEventGenerator::SobAutoAttackMode(m_pActor->GetSerialID(), FALSE);

	CNtlFSMCharActStateBase::Exit();

	NTL_RETURNVOID();
}

void CNtlFSMCharActFightingState::Update(RwReal fElapsed)
{
	CNtlFSMCharActStateBase::Update(fElapsed);
}

RwUInt32 CNtlFSMCharActFightingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActFightingState::HandleEvents");

	if(pMsg.Id == g_EventActionMapSitAndStand)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}
	else if(pMsg.Id == g_EventSobHit)
	{
		SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
		RwUInt8 byBehavior = pHit->sHitStuff.uiAttr.bitBehavior;
		if(byBehavior == NTL_ATTACK_KNOCKDOWN || byBehavior == NTL_ATTACK_SLIDING)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_KNOCKDOWN);
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActFightingShrinkState)

CNtlFSMCharActFightingShrinkState::CNtlFSMCharActFightingShrinkState()
{
	SetStateId(NTL_FSMSID_FIGHTING_SHRINK);
	SetStateName(NTL_FSMSN_FIGHTING_SHRINK);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
}

void CNtlFSMCharActFightingShrinkState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// time setting
	m_fTime = 0.0f;

	// ground fly를 위해서
	// 위치를 다시한번 셋팅한다.
	RwV3d vPos = m_pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
	vPos.y = sHStuff.fFinialHeight;

	m_pActor->SetPosition(&vPos);
}

void CNtlFSMCharActFightingShrinkState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActFightingShrinkState::Update(RwReal fElapsed)
{
	m_fTime += fElapsed;
	if(m_fTime >= 0.2f)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		
		CNtlSobActor *pTarSobActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId) );
		if(pTarSobActor == NULL)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_FIGHTING_POSE);
			return;
		}

		if(pTarSobActor->IsDie())
		{
			Finish();
			SetNextStateId(NTL_FSMSID_FIGHTING_POSE);
			return;
		}

		CNtlFSMStateBase *pBaseState = pTarSobActor->GetCurrentState();
		if(pBaseState == NULL)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_FIGHTING_POSE);
			return;
		}

		RwUInt32 uiStateId = pBaseState->GetStateId();
		if(uiStateId == NTL_FSMSID_FAINTING || uiStateId == NTL_FSMSID_DIE)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_FIGHTING_POSE);
			return;
		}

		if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
		{
			if(!Logic_IsEnemyTargetFromActor(m_pActor, pCtrlStuff->hTargetSerialId))
			{
				Finish();
				SetNextStateId(NTL_FSMSID_FIGHTING_POSE);
				return;
			}
		}


		m_fTime = 0.0f;
	}

	
	// 여기에서는 behavior가 없기 때문에 굳이 state를 업데이트 할 필요가 없다.
	//	CNtlFSMCharActStateBase::Update(fElapsed); 
}

RwUInt32 CNtlFSMCharActFightingShrinkState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActFightingShrinkState::HandleEvents");

	if(pMsg.Id == g_EventActionMapSitAndStand)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_IDLE);
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else
	{
		NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActHurtState)

CNtlFSMCharActHurtState::CNtlFSMCharActHurtState()
{
	SetStateId(NTL_FSMSID_HURT);
	SetStateName(NTL_FSMSN_HURT);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_SKIP_STANDUP);
}

void CNtlFSMCharActHurtState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

    // NPC 상점 이용중이라면 캔슬한다.
    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        if(GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_NPC_COMMU))
        {
            CNtlSLEventGenerator::NPCCommuExit();
        }
    }

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff(); 
	if(pHitStuff->uiAttr.bitResult == NTL_ATTACK_RESULT_BLOCK || pHitStuff->uiAttr.bitResult == NTL_ATTACK_RESULT_DODGE)
	{
		RwInt32 uiStateFlags = GetFlags();
		uiStateFlags |= NTL_FSMSF_NOT_HURT_STATE;
		SetFlags(uiStateFlags);
	}

	RwUInt8 byAttBehavior = pHitStuff->uiAttr.bitBehavior;

	if(byAttBehavior == NTL_ATTACK_NORMAL || byAttBehavior == NTL_ATTACK_PUSH)
	{
		CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharHurt;
		AddBehavior(pBehavior);
	}
}

void CNtlFSMCharActHurtState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActKnockDownState)

CNtlFSMCharActKnockDownState::CNtlFSMCharActKnockDownState()
{
	SetStateId(NTL_FSMSID_KNOCKDOWN);
	SetStateName(NTL_FSMSN_KNOCKDOWN);
	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_JUMP |  NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SIT | 
			NTL_FSMSF_NOT_ADJUSTMOVE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_SKIP_SITDOWN | 
			NTL_FSMSF_SKIP_ATTACK_CLICK);

	m_bDirectEnd = FALSE;
}

void CNtlFSMCharActKnockDownState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// NPC 상점 이용중이라면 캔슬한다.
	if(m_pActor->GetClassID() == SLCLASS_AVATAR)
	{
		if(GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_NPC_COMMU))
		{
			CNtlSLEventGenerator::NPCCommuExit();
		}
	}

	CNtlBehaviorCharKnockDown *pBehavior = NTL_NEW CNtlBehaviorCharKnockDown;

	if(!m_bDirectEnd)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SHitStuff *pHitStuff = pBeData->GetHitStuff(); 
	//	DBO_WARNING_MESSAGE("set kd dir.. pHitStuff->vPushDir.x: " << pHitStuff->vPushDir.x << " pHitStuff->vPushDir.y: " << pHitStuff->vPushDir.y << " pHitStuff->vPushDir.z: " << pHitStuff->vPushDir.z);
		pBehavior->SetKnockDownDir(&pHitStuff->vPushDir);
	}
	else
	{
		// If you go straight to the end of the state.
		// If the state is knockdown when creating a character on the server.
		RwV3d vDir = m_pActor->GetDirection();
		pBehavior->SetKnockDownDir(&vDir);
	}

	AddBehavior(pBehavior);

	if(m_bDirectEnd)
		pBehavior->ChangeKnockDownState(CNtlBehaviorCharKnockDown::KD_KNOCK);	
}

void CNtlFSMCharActKnockDownState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActKnockDownState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActKnockDownState::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		m_bDirectEnd = TRUE;
		NTL_RETURN( NTL_FSM_EVENTRES_PASS );
	}
	else if(pMsg.Id == g_EventSobFainting)
	{
		// faint event일 경우에는 behavior만 event를 보낸다.
		// knockdown되어서 쓰러져 있는 상태로 죽어야 하므로...
		CNtlFSMStateBase::HandleEvents(pMsg);
		FSMEvent_CharActFainting(m_pActor, reinterpret_cast<SNtlEventSobFainting*>( pMsg.pData ));
		SetNextStateId(NTL_FSMSID_DIE);
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}
	else if(pMsg.Id == g_EventSobHit)
	{
		// 여기는 중요한 부분.
		SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
		FSMEvent_CharActHit(m_pActor, pHit); 

		LuaExec_Hurt(pHit->sHitStuff.hAttackerSerialId, pHit->sHitStuff.hDefenderSerialId, &pHit->sHitStuff);

		if(pHit->sHitStuff.uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_KNOCKDOWN);
			NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
		}
		else if(pHit->sHitStuff.uiAttr.bitBehavior == NTL_ATTACK_SLIDING)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_SLIDING);
			NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
		}

		CNtlFSMStateBase::HandleEvents(pMsg);

		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}

	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSlidingState)

CNtlFSMCharActSlidingState::CNtlFSMCharActSlidingState()
{
	SetStateId(NTL_FSMSID_SLIDING);
	SetStateName(NTL_FSMSN_SLIDING);

	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SIT | 
			NTL_FSMSF_NOT_ADJUSTMOVE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_HURT_STATE | 
			NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
}

void CNtlFSMCharActSlidingState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff(); 

	CNtlBehaviorCharSliding *pBehavior = NTL_NEW CNtlBehaviorCharSliding;
	pBehavior->SetSlidingDir(&pHitStuff->vPushDir);
	AddBehavior(pBehavior);
}

void CNtlFSMCharActSlidingState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActSlidingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSlidingState::HandleEvents");

	if(pMsg.Id == g_EventSobFainting)
	{
		// faint event일 경우에는 behavior만 event를 보낸다.
		// sliding이 끝난 다음 죽어야 하므로....
		FSMEvent_CharActFainting(m_pActor, reinterpret_cast<SNtlEventSobFainting*>( pMsg.pData ));
		SetNextStateId(NTL_FSMSID_FAINTING);
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}
	else if(pMsg.Id == g_EventSobMove)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}
	else if(pMsg.Id == g_EventSobFollowMove)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}

	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActStunState)

CNtlFSMCharActStunState::CNtlFSMCharActStunState()
{
	SetStateId(NTL_FSMSID_STUN);
	SetStateName(NTL_FSMSN_STUN);

	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | 
			NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_ADJUSTMOVE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_HURT_STATE |
			NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);

    m_byStunType = DBO_STUN_TYPE_INVALID;
    m_fOrgAnimSpeed = 1.0f;    
    m_bApplyStun = FALSE;
}

void CNtlFSMCharActStunState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// animation settig
	RwV3d vPos = m_pActor->GetPosition();
	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
	RwReal fCharHeight = pSobProxy->GetPLEntityHeight();
	
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    m_byStunType = pBeData->GetCtrlStuff()->uExtra.sStun.byStunType;

	if (m_byStunType == DBO_STUN_TYPE_GENERAL)
	{
		SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SOCIALUSE |
			NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_ADJUSTMOVE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_HURT_STATE |
			NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
	}
    
    switch(m_byStunType)
    {
		case DBO_STUN_TYPE_GENERAL:
        {
            pSobProxy->SetBaseAnimation(BTL_DEF_STUN);

            // effect setting
            RwV3d vEffectPos, vOffset;
            CNtlMath::MathRwV3dAssign(&vEffectPos, vPos.x, vPos.y + fCharHeight, vPos.z);
            CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, fCharHeight, 0.0f);

            CNtlPLEntity *pPLEntity = pSobProxy->CreatePLEffect(NTL_VID_STATE_STUN, &vEffectPos);
            if(pPLEntity)
            {
                Helper_AttachWorldPos(pSobProxy->GetPLMainEntity(), pPLEntity, vOffset);
                AddPLEntity(pPLEntity);
            }
            
        }
        break;
		case DBO_STUN_TYPE_FROZEN:      // 동작을 멈춘다.        
        {
            m_fOrgAnimSpeed = pSobProxy->GetAnimSpeed();
            pSobProxy->SetAnimSpeed(0.0f);
            
            // 이펙트
            CNtlPLEntity* pEffect = pSobProxy->CreatePLEffect(NTL_VID_STATE_FREEZE, &m_pActor->GetPosition());
            if(pEffect)
            {
                Helper_AttachWorldPos(pSobProxy->GetPLMainEntity(), pEffect, ZeroAxis);
                AddPLEntity(pEffect);
            }            
        }
        break;
		case DBO_STUN_TYPE_TIED:
        {
            // 우선 IDLE 애니메이션
            RwUInt32 uiNextAnimKey = Logic_GetIdleAnimationID(m_pActor);	            
            pSobProxy->SetBaseAnimation(uiNextAnimKey);
        }
        break;
		case DBO_STUN_TYPE_STONE:
        OnStone(TRUE);
        break;
		case DBO_STUN_TYPE_CANDY:
        // Candy does not handle Enter because it shoots the event.      
        break;
    }        
}

void CNtlFSMCharActStunState::Exit(void)
{
    switch(m_byStunType)
    {
		case DBO_STUN_TYPE_FROZEN:        
			m_pActor->GetSobProxy()->SetAnimSpeed(m_fOrgAnimSpeed);
			break;
		case DBO_STUN_TYPE_TIED:
			break;
		case DBO_STUN_TYPE_STONE:
			OnStone(FALSE);
			break;
		case DBO_STUN_TYPE_CANDY:
			OnCandy(FALSE);
			break;
    }
    
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActStunState::Update(RwReal fElapsed)
{
    switch(m_byStunType)
    {
		case DBO_STUN_TYPE_CANDY:
		{
			if (!m_bApplyStun)
			{
				OnCandy(TRUE);
				m_bApplyStun = TRUE;
			}
		}
        break;
    }
}

RwUInt32 CNtlFSMCharActStunState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStunState::HandleEvents");

	if(pMsg.Id == g_EventSobStanding)
	{
		Finish();
	}
	else if(pMsg.Id == g_EventSobHit)
	{
		SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
		RwUInt8 byAttBehavior = pHit->sHitStuff.uiAttr.bitBehavior;
		if(byAttBehavior == NTL_ATTACK_NORMAL || byAttBehavior == NTL_ATTACK_PUSH)
		{
            if(m_byStunType == DBO_STUN_TYPE_GENERAL)
            {
			    CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharHurt;
			    AddBehavior(pBehavior);
            }

            CNtlFSMCharActStateBase::HandleEvents(pMsg);

			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}

		else if(byAttBehavior == NTL_ATTACK_KNOCKDOWN || byAttBehavior == NTL_ATTACK_SLIDING)
		{
			RwUInt8 byBehavior = pHit->sHitStuff.uiAttr.bitBehavior;
			if(byBehavior == NTL_ATTACK_KNOCKDOWN || byBehavior == NTL_ATTACK_SLIDING)
			{
				SetFlags( GetFlags() & ~(NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_STATECHANGE) );
			}
		}
	}
    else if(pMsg.Id == g_EventAnimEnd)
    {
        if(m_byStunType == DBO_STUN_TYPE_CANDY)
        {
            SNtlEventAnimEnd* pEvent = (SNtlEventAnimEnd*)pMsg.pData;
            if(pEvent->uiBaseAnimKey == TRANS_IDLE)
            {
                m_pActor->GetSobProxy()->SetBaseAnimation(NML_IDLE_LOOP);
            }
        }        
    }

	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}

void CNtlFSMCharActStunState::OnStone(RwBool bEffect)
{
    CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();

    if(bEffect)
    {
        // 애니메이션을 멈춘다.
        m_fOrgAnimSpeed = pSobProxy->GetAnimSpeed();
        pSobProxy->SetAnimSpeed(0.0f);

        pSobProxy->SetStoneMaterial(TRUE);
    }
    else
    {
        pSobProxy->SetAnimSpeed(m_fOrgAnimSpeed);
        pSobProxy->SetStoneMaterial(FALSE);
    }
}

void CNtlFSMCharActStunState::OnCandy(RwBool bEffect)
{
    CNtlSLEventGenerator::SobTransformCandy(m_pActor->GetSerialID(), bEffect);
    if(bEffect)
    {
        m_pActor->GetSobProxy()->SetBaseAnimation(TRANS_IDLE, FALSE);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSleepState)

CNtlFSMCharActSleepState::CNtlFSMCharActSleepState()
{
	SetStateId(NTL_FSMSID_SLEEP);
	SetStateName(NTL_FSMSN_SLEEP);

	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE |
			NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_ADJUSTMOVE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_HURT_STATE | 
			NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
}

void CNtlFSMCharActSleepState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// animation settig
	RwV3d vPos = m_pActor->GetPosition();
	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
	RwReal fCharHeight = pSobProxy->GetPLEntityHeight();
	
	pSobProxy->SetBaseAnimation(NML_IDLE_LOOP);

	// effect setting
	RwV3d vEffectPos = vPos;
	vEffectPos.y += fCharHeight;
	CNtlPLEntity *pPLEntity = pSobProxy->CreatePLEffect(NTL_VID_STATE_SLEEP, &vEffectPos);
	AddPLEntity(pPLEntity);
}

void CNtlFSMCharActSleepState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActSleepState::Update(RwReal fElapsed)
{
}

RwUInt32 CNtlFSMCharActSleepState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSleepState::HandleEvents");

	if(pMsg.Id == g_EventSobStanding)
		Finish();
	else if(pMsg.Id == g_EventSobHit)
	{
        SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
        RwUInt8 byBehavior = pHit->sHitStuff.uiAttr.bitBehavior;
		if(byBehavior == NTL_ATTACK_KNOCKDOWN || byBehavior == NTL_ATTACK_SLIDING)
		{
			if(byBehavior == NTL_ATTACK_KNOCKDOWN || byBehavior == NTL_ATTACK_SLIDING)
			{
				SetFlags( GetFlags() & ~(NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_STATECHANGE) );
			}
		}
	}

	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActParalyzeState)

CNtlFSMCharActParalyzeState::CNtlFSMCharActParalyzeState()
{
	SetStateId(NTL_FSMSID_PARALYZE);
	SetStateName(NTL_FSMSN_PARALYZE);

	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_JUMP /*| NTL_FSMSF_NOT_SKILLUSE*/ | NTL_FSMSF_NOT_SOCIALUSE | 
			NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_ADJUSTMOVE | NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_HURT_STATE | 
			NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);

	m_fOrigAnimSpeed = 0.f;
}

void CNtlFSMCharActParalyzeState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// animation settig
	RwV3d vPos = m_pActor->GetPosition();
	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();	

	m_fOrigAnimSpeed = pSobProxy->GetAnimSpeed();
	pSobProxy->SetAnimSpeed(0.f);

	// effect setting
	RwV3d vEffectPos = vPos;
    vEffectPos.y += pSobProxy->GetPLEntityHeight();
	CNtlPLEntity *pPLEffect = pSobProxy->CreatePLEffect(NTL_VID_STATE_PARALYZE, &vEffectPos);
	AddPLEntity(pPLEffect);
}

void CNtlFSMCharActParalyzeState::Exit(void)
{
	CNtlSobProxy* pSobProxy = m_pActor->GetSobProxy();
	pSobProxy->SetAnimSpeed(m_fOrigAnimSpeed);

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActParalyzeState::Update(RwReal fElapsed)
{
}

RwUInt32 CNtlFSMCharActParalyzeState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSleepState::HandleEvents");

	if(pMsg.Id == g_EventSobStanding)
		Finish();

	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSkillFollowState)

CNtlFSMCharActSkillFollowState::CNtlFSMCharActSkillFollowState()
{
	SetStateId(NTL_FSMSID_SKILL_FOLLOW);
	SetStateName(NTL_FSMSN_SKILL_FOLLOW);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP);
    m_byMoveForm = NTL_MOVEFORM_GROUND;

	// NTL_FSMSF_NOT_SKILLUSE
}

void CNtlFSMCharActSkillFollowState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

    if(CheckInFollowRange())
        return;

	// char behavior data 를 얻어온다.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	
	// fly move인지 ground move인지를 판단한다.
	if(pMoveStuff->byForm == NTL_MOVEFORM_GROUND)
	{
		CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharGroundMove;
		AddBehavior(pBehavior);
	}
	else
	{
	}

    m_byMoveForm = pMoveStuff->byForm;
}

void CNtlFSMCharActSkillFollowState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActSkillFollowState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSkillFollowState::HandleEvents");

	if(pMsg.Id == g_EventActionMapSkillUse)
	{
		SNtlEventActionMapSkillUse *pSkillUse = reinterpret_cast<SNtlEventActionMapSkillUse*>(pMsg.pData);

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

		if(pSkillStuff->bySlotIdx == pSkillUse->bySkillSlotIdx)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		else
		{
			Logic_SkillUseVisualEffect(m_pActor, pSkillUse->hSkillSerialId);
			FSMEvent_CharActActionMapSkillUse(m_pActor, pSkillUse);            
		}
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

void CNtlFSMCharActSkillFollowState::Update( RwReal fElapsed ) 
{
    CNtlFSMCharActStateBase::Update(fElapsed);

    // Falling 체크
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
    if(pMoveStuff->byMoveResult & NTL_MOVE_RESULT_FALLING)
    {
        Finish();
        SetNextStateId(NTL_FSMSID_FALLING);
    }

    // 육지인지 물속인지 파악하여 행동을 변경하여 준다.
    SWorldHeightStuff hStuff;
    if(Logic_IsSwimmingActor(m_pActor, &m_pActor->GetPosition(), hStuff))
    {
        if(m_byMoveForm != NTL_MOVEFORM_SWIMMING)
        {
            RemoveAllBehavior();
            CNtlBehaviorCharSwimmingMove* pBehavior = NTL_NEW CNtlBehaviorCharSwimmingMove();
            AddBehavior(pBehavior);
            m_byMoveForm = NTL_MOVEFORM_SWIMMING;
        }
    }
    else
    {
        if(m_byMoveForm != NTL_MOVEFORM_GROUND)
        {
            RemoveAllBehavior();
            CNtlBehaviorCharGroundMove *pBehavior = NTL_NEW CNtlBehaviorCharGroundMove;
            AddBehavior(pBehavior);
            m_byMoveForm = NTL_MOVEFORM_GROUND;
        }
    }
}

RwBool CNtlFSMCharActSkillFollowState::CheckInFollowRange() 
{
    // char behavior data 를 얻어온다.
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());     
    SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId));
    return Logic_InFollowRange(m_pActor, pTargetActor, pCtrlStuff->fFollowRange);        
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSkillFocusState)

CNtlFSMCharActSkillFocusState::CNtlFSMCharActSkillFocusState()
{
	SetStateId(NTL_FSMSID_SKILL_FOCUS);
	SetStateName(NTL_FSMSN_SKILL_FOCUS);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK | NTL_FSMSF_VISUAL_RPBOUNS);
}

void CNtlFSMCharActSkillFocusState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

    // Rp Bonus Effect와 애니메이션을 재생한다.
	CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
	pSobProxy->SetBaseAnimation(NML_SKILL_ABILITY);    
    pSobProxy->AttachRPBonusEffect();
}

void CNtlFSMCharActSkillFocusState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActSkillFocusState::Update(RwReal fElapsed)
{
}

RwUInt32 CNtlFSMCharActSkillFocusState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSkillFocusState::HandleEvents");

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSkillReadyState)

CNtlFSMCharActSkillReadyState::CNtlFSMCharActSkillReadyState()
{
	SetStateId(NTL_FSMSID_SKILL_READY);
	SetStateName(NTL_FSMSN_SKILL_READY);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_SKIP_STANDUP | 
             NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_SKIP_ATTACK_CLICK | NTL_FSMSF_VISUAL_RPBOUNS);
}

void CNtlFSMCharActSkillReadyState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	m_bSkillReq = FALSE;
	m_fTime = 0.0f;	
}

void CNtlFSMCharActSkillReadyState::Update(RwReal fElapsed)
{
	// 여기에서는 behavior가 없기 때문에 굳이 state를 업데이트 할 필요가 없다.
	//	CNtlFSMCharActStateBase::Update(fElapsed); 

	if(m_bSkillReq)
	{
		m_fTime += fElapsed;
		if(m_fTime > 0.5f)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_SKILL_FOLLOW);
		}
	}
}

void CNtlFSMCharActSkillReadyState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActSkillReadyState::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobSkillCancel)
	{
		SNtlEventSobSkillCancel *pSobSkillCancel = reinterpret_cast<SNtlEventSobSkillCancel*>(pMsg.pData);

        // Rp Bonus가 적용되어 있으면 이펙트를 제거한다.
        CNtlSobCharProxy* pProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
        pProxy->DetachRPBonusEffect();   

        // 서브웨폰이 있으면 해제한다.
        CNtlPLEventGenerator::AnimEventSubWeaponDeActive(m_pActor->GetSerialID());

		// 여기서 이유를 밝힌다.
		if(pSobSkillCancel->wReason == GAME_TARGET_TOO_FAR ||
           pSobSkillCancel->wReason == GAME_TARGET_TOO_CLOSE)
		{
			m_bSkillReq = TRUE;
			return NTL_FSM_EVENTRES_BLOCK;
		}
		else
		{
			Finish();
			return NTL_FSM_EVENTRES_CHANGE_STATE;
		}        
	}

	return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlFSMCharActSkillCastingState)

CNtlFSMCharActSkillCastingState::CNtlFSMCharActSkillCastingState()
{
	SetStateId(NTL_FSMSID_SKILL_CASTING);
	SetStateName(NTL_FSMSN_SKILL_CASTING);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK | 
			NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_VISUAL_RPBOUNS);

	m_byCastingState = CASTING_STATE_START;
	m_bSkillSuccess = TRUE;
    m_fOcclusionCheckTime = 0.0f;
}

void CNtlFSMCharActSkillCastingState::SetTrans(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

	if(m_pActor->GetSerialID() == pSkillStuff->hTargetSerialId)
		return;

	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pSkillStuff->hTargetSerialId) );
	if(pTargetActor == NULL)
		return;

	RwV3d vPos = m_pActor->GetPosition();
	RwV3d vTargetPos = pTargetActor->GetPosition();

	RwV3d vDir;
	RwV3dSubMacro(&vDir, &vTargetPos, &vPos);
	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);

	m_pActor->SetDirection(&vDir);
}

void CNtlFSMCharActSkillCastingState::SetAnim(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

	CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
	sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(pSkillStuff->uiSkillTblId) );

	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();

	if(m_byCastingState == CASTING_STATE_START)
	{
		if(pSkillTblData->wCasting_Animation_Start != INVALID_ANIMATIONID)
			pSobProxy->SetBaseAnimation(pSkillTblData->wCasting_Animation_Start, FALSE);
		else
		{
			pSobProxy->SetBaseAnimation(pSkillTblData->wCasting_Animation_Loop, TRUE);
			m_byCastingState = CASTING_STATE_LOOP;
		}
	}
	else
	{
		pSobProxy->SetBaseAnimation(pSkillTblData->wCasting_Animation_Loop, TRUE);
	}
}

void CNtlFSMCharActSkillCastingState::Enter(void)
{
	SetTrans();
	SetAnim();

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
    SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

	if(m_pActor->GetFlags() & SLFLAG_LUA_INTERLOCKING && m_pActor->GetClassID() == SLCLASS_AVATAR)
	{
		LuaExec_SkillCastingEnter(m_pActor->GetSerialID(), pSkillStuff->uiSkillTblId);
	}

	// casting gui를 위한 event를 보낸다.
	if(m_pActor->GetClassID() == SLCLASS_AVATAR)
	{
		CNtlSLEventGenerator::SobCastingDirect( m_pActor, TRUE, TRUE );
	}
	else if( m_pActor->GetClassID() == SLCLASS_MONSTER )
	{
		// Show casting targets (turn off when target hits)
		if( Logic_IsCastingTargetSkill( pSkillStuff->uiSkillTblId ) )
			CNtlSLEventGenerator::SobTargetMarkSelect(m_pActor->GetTargetID());
		// Show Strong Casting UI
		if( Logic_IsStrongCastSkill( pSkillStuff->uiSkillTblId ) )
			CNtlSLEventGenerator::SobCastingDirect( m_pActor, TRUE, TRUE );
	}

    //////////////////////////////////////////////////////////////////////////
    // Range Skill Effect
    CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
    sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(pSkillStuff->uiSkillTblId) );   

    if(pSkillTblData->byApply_Range == DBO_SKILL_APPLY_RANGE_CIRCLE)
    {
        std::string strEffectName;
        RwV3d vPos = ZeroAxis;

        // 
        if(Logic_IsSkillSystemEffectCode(pSkillStuff->uiSkillTblId, SKILL_ACTIVE_TYPE_DH) || 
            Logic_IsSkillSystemEffectCode(pSkillStuff->uiSkillTblId, SKILL_ACTIVE_TYPE_HOT))
        {
            strEffectName = NTL_VID_DECAL_SKILL_RANGE_HEAL;
        }
        else
        {
            strEffectName = NTL_VID_DECAL_SKILL_RANGE_DMG;
        }

        // 
        if(pSkillTblData->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_SELF)
        {
            vPos = m_pActor->GetPosition();
        } 
        else if(pSkillTblData->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_TARGET)
        {
            CNtlSob* pSobTarget = GetNtlSobManager()->GetSobObject(Logic_GetActorTargetSerialId(m_pActor));
            if(pSobTarget)
            {
                vPos = pSobTarget->GetPosition();
            }
        }

        // effect
        CNtlPLEntity* pEffectRange = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, strEffectName.c_str());
        pEffectRange->SetPosition(&vPos);

        // skill range      
		pEffectRange->SetScale(Logic_GetSkillApplyRange(m_pActor, pSkillStuff->uiSkillTblId));
		AddPLEntity(pEffectRange);
    }

	CNtlFSMCharActStateBase::Enter();
}

void CNtlFSMCharActSkillCastingState::Exit(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	if (pBeData)
	{
		SSkillStuff* pSkillStuff = pBeData->GetSkillStuff();

		if (m_pActor->GetFlags() & SLFLAG_LUA_INTERLOCKING && m_pActor->GetClassID() == SLCLASS_AVATAR)
		{
			if (GetNextStateId() != NTL_FSMSID_SKILL_ACTION)
			{
				LuaExec_SkillCastingExit(m_pActor->GetSerialID(), pSkillStuff->uiSkillTblId, FALSE);
			}
		}

		// 캐스팅 타겟 마크
		if (m_pActor->GetTargetID() == Logic_GetTargetMarkingID())
		{
			CNtlSLEventGenerator::SobTargetMarkRelease(Logic_GetTargetMarkingID());
		}

		// casting gui를 위한 event를 보낸다.
		if (m_pActor->GetClassID() == SLCLASS_AVATAR)
		{
			CNtlSLEventGenerator::SobCastingDirect(m_pActor, TRUE, FALSE);
		}
		else if (m_pActor->GetClassID() == SLCLASS_MONSTER && Logic_IsStrongCastSkill(pSkillStuff->uiSkillTblId))
		{
			CNtlSLEventGenerator::SobCastingDirect(m_pActor, TRUE, FALSE);
		}
	}

	CNtlFSMCharActStateBase::Exit();
}

#define CASTING_CHECK_OCCLUSION     0.5f                        ///< 캐스팅중에 오브젝트 오클류젼을 체크할 타임

void CNtlFSMCharActSkillCastingState::Update(RwReal fElapsed)
{
    m_fOcclusionCheckTime += fElapsed;
    if(m_pActor->GetClassID() == SLCLASS_AVATAR && m_fOcclusionCheckTime > CASTING_CHECK_OCCLUSION)
    {
        m_fOcclusionCheckTime = 0.0f;

        // 캐스팅중에 오브젝트 충돌 체크를 한다.        
        if(Logic_isEnemyTargetOcclusion(m_pActor->GetTargetID()))
        {
            // 서버에 캐스팅 캔슬 패킷을 보낸다.
            API_GetSLPacketGenerator()->SendCharSkillCastingCanceledNfy(GAME_SKILL_CASTING_CANCELED_OBJECT_OCCLUSION);
        }
    }

	if( m_pActor->GetClassID() == SLCLASS_AVATAR ||
		m_pActor->GetClassID() == SLCLASS_MONSTER )
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
		SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
		pSkillStuff->fCurrCastingTime += fElapsed;
		if(pSkillStuff->fCurrCastingTime >= pSkillStuff->fMaxCastingTime)
			pSkillStuff->fCurrCastingTime = pSkillStuff->fMaxCastingTime;
	}
}

RwUInt32 CNtlFSMCharActSkillCastingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSkillCastingState::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		m_byCastingState = CASTING_STATE_LOOP;
		SetAnim();
	}
    else if(pMsg.Id == g_EventSobSkillCancel)
    {
        Finish();

        // 서브웨폰이 있으면 해제한다.
        CNtlPLEventGenerator::AnimEventSubWeaponDeActive(m_pActor->GetSerialID());

        return NTL_FSM_EVENTRES_CHANGE_STATE;
    }
	

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlFSMCharActSkillActionState)

CNtlFSMCharActSkillActionState::CNtlFSMCharActSkillActionState()
{
	SetStateId(NTL_FSMSID_SKILL_ACTION);
	SetStateName(NTL_FSMSN_SKILL_ACTION);
	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE |
			NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK | NTL_FSMSF_VISUAL_RPBOUNS);

	m_hPetSerialId		= INVALID_SERIAL_ID;    
	m_bAnimCancelPoint	= FALSE;
    m_bRightFinish      = FALSE;
    m_uiSkillTblId      = 0;
}

void CNtlFSMCharActSkillActionState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();
 
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
    CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
    m_uiSkillTblId = pSkillStuff->uiSkillTblId;

    sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(m_uiSkillTblId) );

	if(pSkillTblData->bDash_Able)
	{
        SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();        
		pMoveStuff->Reset();
		pMoveStuff->byType = NTL_MOVETARGET_DASH_ACTOR;
		pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
		pMoveStuff->byFormFlag = NTL_MOVE_FLAG_DASH;
		pMoveStuff->hFollowSerialId = pSkillStuff->hTargetSerialId;
		pMoveStuff->fFollowRange = (RwReal)pSkillTblData->fUse_Range_Max;
	
        CNtlBehaviorFrame *pFrame = NTL_NEW CNtlBehaviorFrame;        
		CNtlBehaviorCharGroundMove *pGroundMove = NTL_NEW CNtlBehaviorCharGroundMove;        
		pFrame->AddChild(pGroundMove);

		CNtlBehaviorCharNormalAttack *pNormalAttackBehavior = NTL_NEW CNtlBehaviorCharNormalAttack;        
		pFrame->AddChild(pNormalAttackBehavior);

        AddBehavior(pFrame);
	}
	else
	{
		CNtlBehaviorCharNormalAttack *pNormalAttackBehavior = NTL_NEW CNtlBehaviorCharNormalAttack;
		AddBehavior(pNormalAttackBehavior);
	}

    //////////////////////////////////////////////////////////////////////////
    // Skill Effects related Behavior
	for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		sSYSTEM_EFFECT_TBLDAT* pEffectData = (sSYSTEM_EFFECT_TBLDAT*)API_GetTableContainer()->GetSystemEffectTable()->FindData(pSkillTblData->skill_Effect[i]);
		if (!pEffectData)
			continue;

		if (pEffectData->effectCode == ACTIVE_WARP_BACK_ATTACK)
		{
			CNtlBehaviorCharActiveWarpBackAttack* pSkill = NTL_NEW CNtlBehaviorCharActiveWarpBackAttack();
			pSkill->SetFinalLoc(pSkillStuff->vFinalLoc);
			AddBehavior(pSkill);
			break;
		}
		else if (pEffectData->effectCode == ACTIVE_WARP_STUN)
		{
			CNtlBehaviorCharActiveWarp* pSkill = NTL_NEW CNtlBehaviorCharActiveWarp();
			pSkill->SetFinalLoc(pSkillStuff->vFinalLoc);
			AddBehavior(pSkill);
			break;
		}
		else if (pEffectData->effectCode == ACTIVE_THROUGH_ATTACK)
		{
			CNtlBehaviorCharActiveThroughAttack* pSkill = NTL_NEW CNtlBehaviorCharActiveThroughAttack();
			pSkill->SetFinalLoc(pSkillStuff->vFinalLoc);
			AddBehavior(pSkill);
			break;
		}
		else if (pEffectData->effectCode == ACTIVE_MONSTER_TRANSFORM)
		{
			// Mob Transformation only
			CNtlBehaviorCharTransform* pSkill = NTL_NEW CNtlBehaviorCharTransform();
			pSkill->SetAnimation(pSkillTblData->wAction_Animation_Index,
				pSkillTblData->wAction_Loop_Animation_Index);
			AddBehavior(pSkill);

			//CNtlSLEventGenerator::SobMobTransform(m_pActor->GetSerialID(), (TBLIDX)Logic_GetSkillSystemEffectValue(pSkillStuff->uiSkillTblId, ACTIVE_MONSTER_TRANSFORM));
			break;
		}
		else if (pEffectData->effectCode == ACTIVE_SMOKE_MOVE)
		{
			DBO_WARNING_MESSAGE("ACTIVE_SMOKE_MOVE TO DO");
			break;
		}
		else if (pEffectData->effectCode == ACTIVE_PULL)
		{
			CNtlBehaviorCharActivePull* pSkill = NTL_NEW CNtlBehaviorCharActivePull();
			pSkill->SetFinalLoc(pSkillStuff->vFinalSubjectLoc);
			AddBehavior(pSkill);

			break;
		}
	}
	
    //////////////////////////////////////////////////////////////////////////
    // Range Skill Effect

    if(pSkillTblData->byApply_Range == DBO_SKILL_APPLY_RANGE_CIRCLE)
    {
        std::string strEffectName;
        RwV3d vPos = ZeroAxis;

        // 
        if(Logic_IsSkillSystemEffectCode(m_uiSkillTblId, SKILL_ACTIVE_TYPE_DH) || 
           Logic_IsSkillSystemEffectCode(m_uiSkillTblId, SKILL_ACTIVE_TYPE_HOT))
        {
            strEffectName = NTL_VID_DECAL_SKILL_RANGE_HEAL;
        }
        else
        {
            strEffectName = NTL_VID_DECAL_SKILL_RANGE_DMG;
        }

        // 
        if(pSkillTblData->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_SELF)
        {
            vPos = m_pActor->GetPosition();
        } 
        else if(pSkillTblData->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_TARGET)
        {
            CNtlSob* pSobTarget = GetNtlSobManager()->GetSobObject(Logic_GetActorTargetSerialId(m_pActor));
            if(pSobTarget)
            {
                vPos = pSobTarget->GetPosition();
            }
        }

        // effect
        CNtlPLEntity* pEffectRange = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, strEffectName.c_str());
        pEffectRange->SetPosition(&vPos);
        pEffectRange->SetScale(Logic_GetSkillApplyRange(m_pActor, m_uiSkillTblId));
        AddPLEntity(pEffectRange);
    }

	//////////////////////////////////////////////////////////////////////////
	// Auto attack off check.

	if(pSkillTblData->dwFunction_Bit_Flag & SKILL_FUNCTION_FLAG_STOP_AUTO_ATTACK)
	{
		if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
		{
			CNtlFSMCharActAgent *pCharActAgent = reinterpret_cast<CNtlFSMCharActAgent*>(GetFSMAgent());
			if(pCharActAgent->IsFightingMode())
			{
				pCharActAgent->SetFightingMode(FALSE);
				API_GetSLPacketGenerator()->SendCharToggleFighting(FALSE, m_pActor->GetServerSyncAvatarType());
			}
		}
	}

	if(m_pActor->GetFlags() & SLFLAG_LUA_INTERLOCKING  && m_pActor->GetClassID() == SLCLASS_AVATAR)
		LuaExec_SkillActionEnter(m_pActor->GetSerialID(), m_uiSkillTblId);
}

void CNtlFSMCharActSkillActionState::Exit(void)
{
	if(m_hPetSerialId != INVALID_SERIAL_ID)
	{
		CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_hPetSerialId) );
		if(pActor)
		{
			RwUInt32 uiFlags = pActor->GetFlags();
			pActor->SetFlags(uiFlags & ~SLFLAG_UPDATE_PAUSE);

			pActor->EnableVisible(TRUE);
		}
	} 
 
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	if (pBeData)
	{
		SSkillStuff* pSkillStuff = pBeData->GetSkillStuff();
		CNtlAttackData* pAttackData = pBeData->GetAttackData();

		if (m_pActor->GetFlags() & SLFLAG_LUA_INTERLOCKING && m_pActor->GetClassID() == SLCLASS_AVATAR)
			LuaExec_SkillActionExit(m_pActor->GetSerialID(), m_uiSkillTblId);

		CNtlSLEventGenerator::TSSkillUse_Skill(m_pActor->GetSerialID(), pSkillStuff, pAttackData);
	}

    // Rp Bonus가 적용되어 있으면 이펙트를 제거한다.
    CNtlSobCharProxy* pProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
    pProxy->DetachRPBonusEffect();    

    // If you leave the skill state, the sub-weapon is unconditionally deactivated.
    CNtlPLEventGenerator::AnimEventSubWeaponDeActive(m_pActor->GetSerialID());

    // Play all remaining events for when canceled or knocked back.
    pProxy->SetSkillCancel();

    //if(m_bAnimCancelPoint && m_bRightFinish == FALSE)
    //{    
    //}
    
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActSkillActionState::Update(RwReal fElapsed)
{
	CNtlFSMCharActStateBase::Update(fElapsed);
    
    // It was canceled normally without being canceled.  
    if(m_listBehavior.size() <= 0)
    {
        m_bRightFinish = TRUE;
    }
}

RwUInt32 CNtlFSMCharActSkillActionState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSkillActionState::HandleEvents");

	if(pMsg.Id == g_EventTerrainClick || pMsg.Id == g_EventKeyboardMove ||
		pMsg.Id == g_EventKeyboardMoveStop || pMsg.Id == g_EventActionMapSitAndStand ||
		pMsg.Id == g_EventSobAttackSelect)
	{
        if(!m_bAnimCancelPoint)
        {
		    NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
        }
	}
	else if(pMsg.Id == g_EventActionMapSkillUse)
	{
		if(!m_bAnimCancelPoint)
		{
			SNtlEventActionMapSkillUse *pSkillUse = reinterpret_cast<SNtlEventActionMapSkillUse*>(pMsg.pData);

			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
			SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

			if(pSkillStuff->bySlotIdx != pSkillUse->bySkillSlotIdx)
			{
				NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
			}
		}
	}
	else if(pMsg.Id == g_EventSobSummonPetSpawnSync)
	{
		SNtlEventSobSummonPetSpawnSync *pSobSummonPetSpawnSync = reinterpret_cast<SNtlEventSobSummonPetSpawnSync*>(pMsg.pData);
		m_hPetSerialId = pSobSummonPetSpawnSync->hPetSerialId;

		CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_hPetSerialId) );
		if(pActor)
		{
			RwUInt32 uiFlags = pActor->GetFlags();
			pActor->SetFlags(uiFlags | SLFLAG_UPDATE_PAUSE);

			pActor->EnableVisible(FALSE);
		}

		NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
	}
	else if(pMsg.Id == g_EventSummon)
	{
		CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_hPetSerialId) );
		if(pActor)
		{
			RwUInt32 uiFlags = pActor->GetFlags();
			pActor->SetFlags(uiFlags & ~SLFLAG_UPDATE_PAUSE);

			pActor->EnableVisible(TRUE);
			pActor->GetSobProxy()->SetScale(0.01f);
		}

		m_hPetSerialId = INVALID_SERIAL_ID;

		NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
	}
    else if(pMsg.Id == g_EventAnimStretch)
    {
        SNtlEventStretch* pEvent = (SNtlEventStretch*)pMsg.pData;
        if(pEvent->uiSerialID == m_pActor->GetSerialID())
        {
            CNtlBehaviorCharStretchAttack* pBehavior = NTL_NEW CNtlBehaviorCharStretchAttack();            
            AddBehavior(pBehavior);
            pBehavior->HandleEvents(pMsg);
        }

        NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
    }
    else if(pMsg.Id == g_EventSobFollowMove)
    {
        SNtlEventSobFollowMove* pEvent = (SNtlEventSobFollowMove*)pMsg.pData;
        if(pEvent->hSerialId == m_pActor->GetSerialID())
        {
            SetFlags(GetFlags() & ~NTL_FSMSF_NOT_MOVE);
        }
    }	
	else if(pMsg.Id == g_EventAnimSkillCancel)
	{
		//SetFlags(GetFlags() & ~(NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_INPUT) );
        
        // Idle 상태와 같은 모든 입력을 받는다.
        SetFlags(NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_RUNJUMP);
		m_bAnimCancelPoint = TRUE;        
	}
	
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
	
}

//////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharKeepingEffect)

CNtlFSMCharKeepingEffect::CNtlFSMCharKeepingEffect() 
{
    SetStateId(NTL_FSMSID_KEEPING_EFFECT);
    SetStateName(NTL_FSMSN_KEEPING_EFFECT);
	SetFlags(NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_RUNJUMP);

	m_uiAnimLoopId = INVALID_ANIMATIONID;
    m_uiAnimEndId = INVALID_ANIMATIONID;
}

void CNtlFSMCharKeepingEffect::Enter( void ) 
{
	CNtlFSMCharActStateBase::Enter();

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
    SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
    CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
    sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(pSkillStuff->uiSkillTblId));
    NTL_ASSERT(pSkillTblData, "FSM-KeepingEffect-Enter() : Not SkillTBL (Idx:" << pSkillStuff->uiSkillTblId);
    
    if(pSkillTblData)
    {
		m_pActor->GetSobProxy()->SetBaseAnimation(pSkillTblData->wAction_Animation_Index, FALSE);

		m_uiAnimLoopId = pSkillTblData->wAction_Loop_Animation_Index;
		m_uiAnimEndId = pSkillTblData->wAction_End_Animation_Index;
    }
    else
    {
		Finish();
    }     
}

void CNtlFSMCharKeepingEffect::Exit( void ) 
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharKeepingEffect::Update( RwReal fElapsed ) 
{
}

RwUInt32 CNtlFSMCharKeepingEffect::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventSobStanding)
    {
        if(m_uiAnimEndId != INVALID_ANIMATIONID)
        {
            m_pActor->GetSobProxy()->SetBaseAnimation(m_uiAnimEndId, FALSE);
        }
        else
        {
            Finish();
        }        
    }
	else if(pMsg.Id == g_EventAnimEnd)
    {
        SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*)pMsg.pData;
		if (pData->uiBaseAnimKey == m_uiAnimEndId)
		{
			Finish();
		}
		else
		{
			if(GetNextStateId() == NTL_FSMSID_INVALID)
				m_pActor->GetSobProxy()->SetBaseAnimation(m_uiAnimLoopId, TRUE);
		}
    }

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActSocialActionState)

CNtlFSMCharActSocialActionState::CNtlFSMCharActSocialActionState()
{
	SetStateId(NTL_FSMSID_SOCIAL_ACTION);
	SetStateName(NTL_FSMSN_SOCIAL_ACTION);
	//SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | 
	//		NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_SKIP_STANDUP);
}

void CNtlFSMCharActSocialActionState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();
	
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

	CActionTable *pActionTbl = API_GetTableContainer()->GetActionTable();
	sACTION_TBLDAT *pActionTblData = reinterpret_cast<sACTION_TBLDAT*>( pActionTbl->FindData(pSkillStuff->uiSkillTblId) );

	CChatCommandTable *pChatCmdTbl = API_GetTableContainer()->GetChatCommandTable();
	sCHAT_COMMAND_TBLDAT *pChatCmdTblData = reinterpret_cast<sCHAT_COMMAND_TBLDAT*>( pChatCmdTbl->FindData(pActionTblData->chat_Command_Index) );

	CNtlSobCharProxy *pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
	pSobProxy->SetBaseAnimation(pChatCmdTblData->wAction_Animation_Index, FALSE);

    sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
    if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
        pSobProxy->SetVisibleEquipItem(FALSE, EQUIP_SLOT_TYPE_HAND);            
}

void CNtlFSMCharActSocialActionState::Exit(void)
{
    CNtlSobCharProxy *pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();    

    sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
    if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
        pSobProxy->SetVisibleEquipItem(TRUE, EQUIP_SLOT_TYPE_HAND);        

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActSocialActionState::Update(RwReal fElapsed)
{
}

RwUInt32 CNtlFSMCharActSocialActionState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActSocialActionState::HandleEvents");

	/*if(pMsg.Id == g_EventTerrainClick || pMsg.Id == g_EventKeyboardMove ||
		pMsg.Id == g_EventActionMapSitAndStand)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}
	else*/ if(pMsg.Id == g_EventAnimEnd)
	{
		Finish();
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActHTBFollowState)

CNtlFSMCharActHTBFollowState::CNtlFSMCharActHTBFollowState()
{
	SetStateId(NTL_FSMSID_HTB_FOLLOW);
	SetStateName(NTL_FSMSN_HTB_FOLLOW);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
}

void CNtlFSMCharActHTBFollowState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// char behavior data 를 얻어온다.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId));
	if(Logic_InFollowRange(m_pActor, pTargetActor, pCtrlStuff->fFollowRange))
		return;

	// fly move인지 ground move인지를 판단한다.
	if(pMoveStuff->byForm == NTL_MOVEFORM_GROUND)
	{
		CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharGroundMove;
		AddBehavior(pBehavior);
	}
	else
	{
	}
}

void CNtlFSMCharActHTBFollowState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}


RwUInt32 CNtlFSMCharActHTBFollowState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActHTBFollowState::HandleEvents");

	if(pMsg.Id == g_EventActionMapHTBUse)
	{
        SNtlEventActionMapHTBUse* pHTBUse = (SNtlEventActionMapHTBUse*)pMsg.pData;

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

		if(pSkillStuff->bySlotIdx == pHTBUse->bySkillSlotIdx)
		{
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
		else
		{
			Logic_SkillUseVisualEffect(m_pActor, pHTBUse->hSkillSerialId);
			FSMEvent_CharActActionMapHTBUse(m_pActor, pHTBUse);
            NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActHTBReadyState)

CNtlFSMCharActHTBReadyState::CNtlFSMCharActHTBReadyState()
{
	SetStateId(NTL_FSMSID_HTB_READY);
	SetStateName(NTL_FSMSN_HTB_READY);
	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | 
			NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);
}

void CNtlFSMCharActHTBReadyState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	//
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
	if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
		pSobProxy->SetBaseAnimation(BTL_STAFF_FP_LOOP);
	else
		pSobProxy->SetBaseAnimation(BTL_DEF_FP_LOOP);
}

void CNtlFSMCharActHTBReadyState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}


RwUInt32 CNtlFSMCharActHTBReadyState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActHTBReadyState::HandleEvents");

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActHTBState)

CNtlFSMCharActHTBState::CNtlFSMCharActHTBState()
{
	SetStateId(NTL_FSMSID_HTB);
	SetStateName(NTL_FSMSN_HTB);
	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | 
			NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP);

	m_bSimulationMode	= FALSE;
	m_byHTPStepState	= EHTB_PROGRESS;
	m_byHTBNextStep		= 0;
	m_byFirstActiveStep = 0;
	m_bHTBSuccess		= TRUE;
	m_uiSkillTblId		= INVALID_TBLIDX;
	m_byHTBSetType		= HTB_SKILL_NONE;

	m_pHTBElapController = NULL;
	m_pSandbagElapController = NULL;
}

void CNtlFSMCharActHTBState::EnableSimulation(RwBool bEnable)
{
#define HTB_SIMULATION_WEIGHT_ELAPSED_VALUE		10.0f

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();

	if(bEnable)
	{
		// actor의 elasped time 설정한다.
		m_pActor->EnableVisible(FALSE);
		m_pHTBElapController = GetNtlSobElapsedControlManager()->CreateController(m_pActor, 100.0f, HTB_SIMULATION_WEIGHT_ELAPSED_VALUE);
		
		// target actor의 elasped time 설정한다.
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pHTBStuff->hTargetSerialId);
		if(pSobObj)
		{
			pSobObj->EnableVisible(FALSE);
			m_pSandbagElapController = GetNtlSobElapsedControlManager()->CreateController(pSobObj, 100.0f, HTB_SIMULATION_WEIGHT_ELAPSED_VALUE);
		}
	}
	else
	{
		// actor의 elasped time 설정한다.
		m_pActor->EnableVisible(TRUE);
		if(m_pHTBElapController)
		{
			GetNtlSobElapsedControlManager()->DeleteController(m_pHTBElapController);
			m_pHTBElapController = NULL;
		}

		// target actor의 elasped time 설정한다.
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pHTBStuff->hTargetSerialId);
		if(pSobObj)
		{
			pSobObj->EnableVisible(TRUE);
			if(m_pSandbagElapController)
			{
				GetNtlSobElapsedControlManager()->DeleteController(m_pSandbagElapController);
				m_pSandbagElapController = NULL;
			}
		}
	}

	m_bSimulationMode = bEnable;
}

RwReal CNtlFSMCharActHTBState::GetFirstSkillRange(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
	CHTBSetTable *pHTBSetTbl = API_GetTableContainer()->GetHTBSetTable();
	sHTB_SET_TBLDAT *pHTBSetTblData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBSetTbl->FindData(pHTBStuff->uiSkillTblId) );

	for(RwInt32 i = 0; i < pHTBSetTblData->bySetCount; ++i)
	{
		m_uiSkillTblId = pHTBSetTblData->aHTBAction[i].skillTblidx;
		if(m_uiSkillTblId != INVALID_TBLIDX)
		{
			CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(m_uiSkillTblId) );
			sHTBSKILL_RESULT *pResult = pHTBStuff->FindResult((RwUInt8)i);

			RwUInt8 byAttackResult = pResult->sSkillResult.byAttackResult;
			if(byAttackResult == BATTLE_ATTACK_RESULT_DODGE ||	byAttackResult == BATTLE_ATTACK_RESULT_RESISTED ||
				byAttackResult == BATTLE_ATTACK_RESULT_BLOCK || byAttackResult == BATTLE_ATTACK_RESULT_SLIDING)
				m_bHTBSuccess = FALSE;
			
			return Logic_GetSkillUseRange(pSkillTblData);
		}
	}

	return 2;
}

void CNtlFSMCharActHTBState::NextStepProc(void)
{
	// 다음 HTB step을 진행한다.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();

	// lua 연동한다.
	if(m_bHTBSuccess)
	{
		if(m_pActor->GetClassID() == SLCLASS_AVATAR)
		{
			if(m_byHTBNextStep > 0)
				LuaExec_HTBSkillStepExit(m_pActor->GetSerialID(), pHTBStuff->uiSkillTblId, m_uiSkillTblId, m_byHTBNextStep-1);
		}
	}

	// HTB simulation off 시킨다.
	if(m_bSimulationMode && m_byHTBNextStep >= pHTBStuff->byCurrStep)
		EnableSimulation(FALSE);

	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pHTBStuff->hTargetSerialId) );
	if(pTargetActor == NULL)
	{
		m_byHTPStepState = EHTB_FINISH;
		return;
	}

	// 현재 플레이 가능한 HTB step을 구한다.
	if(m_byHTBNextStep >= pHTBStuff->byStepCount)
	{
		m_byHTPStepState = EHTB_FINISH;
		return;
	}

	// 서버로 HTB 진행 상황을 보낸다.
	if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
	{
		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vDir = m_pActor->GetDirection();

		API_GetSLPacketGenerator()->SendCharHTBForward(vPos, vDir);
	}

	// skill을 사용하는 set 이면.
	CHTBSetTable *pHTBSetTbl = API_GetTableContainer()->GetHTBSetTable();
	sHTB_SET_TBLDAT *pHTBSetTblData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBSetTbl->FindData(pHTBStuff->uiSkillTblId) );
    
    if(m_byHTBNextStep >= NTL_HTB_MAX_SKILL_COUNT_IN_SET)
        return;
	RwUInt8 byType = pHTBSetTblData->aHTBAction[m_byHTBNextStep].bySkillType;
	RwUInt32 uiSkillTblIdx = pHTBSetTblData->aHTBAction[m_byHTBNextStep].skillTblidx;

	if(uiSkillTblIdx != INVALID_TBLIDX)
	{
		CNtlAttackData *pAttData = pBeData->GetAttackData();
		pAttData->Reset();
		pAttData->SetAttackStuffCount(1);
		SAttackStuff *pAttStuff = pAttData->GetAttackStuff(0);
		sHTBSKILL_RESULT *pResult = pHTBStuff->FindResult(m_byHTBNextStep);
		sSKILL_RESULT *pActRes = &pResult->sSkillResult;
		sVECTOR3 *pActPush = &pActRes->vShift;

		RwV3d vPushDir;
		CNtlMath::MathRwV3dAssign(&vPushDir, pActPush->x, pActPush->y, pActPush->z);
		RwV3dNormalize(&vPushDir, &vPushDir);

		RwV3d vFinalLoc;
		CNtlMath::MathRwV3dAssign(&vFinalLoc, pActRes->vFinalSubjectLoc.x, pActRes->vFinalSubjectLoc.y, pActRes->vFinalSubjectLoc.z);

		SAttackDamageStuff sDamageStuff;

		if(pActRes->effectResult[0].eResultType == DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL)
		{
			sDamageStuff.iDamage			= (DWORD)((pActRes->effectResult[0]).fResultValue) + (DWORD)((pActRes->effectResult[1]).fResultValue);
			sDamageStuff.iAbsorbedDamage	= 0;
			sDamageStuff.iReflectedDamage	= 0;
		}
		else
		{
			sDamageStuff.iDamage			= (DWORD)((pActRes->effectResult[0]).DD_DOT_fDamage) + (DWORD)((pActRes->effectResult[1]).DD_DOT_fDamage);
			sDamageStuff.iAbsorbedDamage	= (DWORD)((pActRes->effectResult[0]).DD_DOT_fAbsorbedDamage) + (DWORD)((pActRes->effectResult[1]).DD_DOT_fAbsorbedDamage);
			sDamageStuff.iReflectedDamage	= (DWORD)((pActRes->effectResult[0]).DD_DOT_fReflectedDamage) + (DWORD)((pActRes->effectResult[1]).DD_DOT_fReflectedDamage);
		}
		
		FillAttackStuff(pAttStuff, m_pActor->GetSerialID(), pHTBStuff->hTargetSerialId, pActRes->byAttackResult,
						FALSE, 0, pActRes->byBlockedAction, sDamageStuff, vPushDir, NTL_ATTACK_LOGIC_SKILL, FALSE, uiSkillTblIdx, INVALID_SYNCEVENT_ID, vFinalLoc);

		CNtlBehaviorCharNormalAttack *pNormalAttackBehavior = NTL_NEW CNtlBehaviorCharNormalAttack;
		AddBehavior(pNormalAttackBehavior);
	}
	else
	{
		if( byType == HTB_SKILL_DASH)
		{
			SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
			pMoveStuff->Reset();

			pMoveStuff->byType = NTL_MOVETARGET_DASH_ACTOR;
			pMoveStuff->byForm = NTL_MOVEFORM_FLY;
			pMoveStuff->byFormFlag = NTL_MOVE_FLAG_FLY_DASH;

			pMoveStuff->hFollowSerialId = pHTBStuff->hTargetSerialId;
			pMoveStuff->fFollowRange = (RwReal)GetFirstSkillRange();

			CNtlBehaviorCharGroundMove *pGroundMove = NTL_NEW CNtlBehaviorCharGroundMove;
			AddBehavior(pGroundMove);
		}
		else if(byType == HTB_SKILL_HOMING_UP)
		{
			CNtlBehaviorCharHomingUp *pHomingUp = NTL_NEW CNtlBehaviorCharHomingUp;
			AddBehavior(pHomingUp);

			m_byHTBSetType = HTB_SKILL_HOMING_UP;
		}
		else if(byType == HTB_SKILL_HOMING_DOWN || byType == HTB_SKILL_JUMPING_DOWN)
		{
			CNtlBehaviorCharHomingDown *pHomingDown = NTL_NEW CNtlBehaviorCharHomingDown;
			AddBehavior(pHomingDown);

			m_byHTBSetType = HTB_SKILL_HOMING_DOWN;
		}
		else if(byType == HTB_SKILL_JUMPING_UP)
		{
			CNtlBehaviorCharJumpingUp *pJumpingUp = NTL_NEW CNtlBehaviorCharJumpingUp;
			AddBehavior(pJumpingUp);

			m_byHTBSetType = HTB_SKILL_JUMPING_UP;
		}
		else
		{
			NTL_ASSERTFAIL("CNtlFSMCharActHTBState::NextStepProc => not invalid htp skill type" << " HTB id : " << pHTBStuff->uiSkillTblId 
							<< " HTB current step : " << m_byHTBNextStep);
		}
	}

	// HTB camera lua script 호출.
	if(m_bHTBSuccess)
	{
		if(m_pActor->GetClassID() == SLCLASS_AVATAR)
		{
			if(m_byHTBNextStep == m_byFirstActiveStep)
			{
				GetNtlGameCameraManager()->FinishLock();
				LuaExec_HTBCameraStart(pHTBStuff->hAttackSerialId, pHTBStuff->hTargetSerialId, pHTBSetTblData->tblidx);
			}

			LuaExec_HTBSkillStepEnter(m_pActor->GetSerialID(), pHTBStuff->uiSkillTblId, uiSkillTblIdx, m_byHTBNextStep);
		}
	}
	
	m_byHTBNextStep++;
}

RwBool CNtlFSMCharActHTBState::StepCancelProc(void)
{
	if(m_byHTBSetType == HTB_SKILL_JUMPING_UP || m_byHTBSetType == HTB_SKILL_HOMING_UP)
	{
		CNtlBehaviorCharHomingDown *pHomingDown = NTL_NEW CNtlBehaviorCharHomingDown;
		AddBehavior(pHomingDown);

		m_byHTPStepState	= EHTB_CANCEL;
		m_byHTBSetType		= HTB_SKILL_HOMING_DOWN;

		return TRUE;
	}

	return FALSE;
}

RwUInt8 CNtlFSMCharActHTBState::FindFirstActiveStep(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
	CHTBSetTable *pHTBSetTbl = API_GetTableContainer()->GetHTBSetTable();
	sHTB_SET_TBLDAT *pHTBSetTblData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBSetTbl->FindData(pHTBStuff->uiSkillTblId) );

	for(RwInt32 i = 0; i < pHTBSetTblData->bySetCount; ++i)
	{
		RwUInt32 uiSkillTblIdx = pHTBSetTblData->aHTBAction[i].skillTblidx;
		if(uiSkillTblIdx != INVALID_TBLIDX)
			return (RwUInt8)i;
	}

	return 0;
}


RwBool CNtlFSMCharActHTBState::IsFinishHTBStep(void)
{
	return m_byHTPStepState == EHTB_FINISH ? TRUE : FALSE;
}

void CNtlFSMCharActHTBState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();
	
	// 좌표 setting
	RwV3d vPos = m_pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
	vPos.y = sHStuff.fFinialHeight;

	m_pActor->SetPosition(&vPos);

	// simulation step으로 진행해야 하는지 판단한다.
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
	if(pHTBStuff->byCurrStep > 0)
		EnableSimulation(TRUE);

	// 처음으로 active skill이 나오는 step
	m_byFirstActiveStep = FindFirstActiveStep();
	// next step procedure
	NextStepProc();

	// RP Stock Image 띄우기.
	Logic_ShowHTBRPSelectGui(m_pActor, pHTBStuff->hTargetSerialId);
}

void CNtlFSMCharActHTBState::Exit(void)
{
    CNtlCameraShakeController::ResetShakeFactor();

	// sub weapon이 장착되어 있으면, 장착 해제...
	CNtlPLEventGenerator::AnimEventSubWeaponDeActive(m_pActor->GetSerialID());

	// HTB camera delete 
	GetNtlGameCameraManager()->FinishUnLock();
	CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_DT);

	// HTB simulation off
	EnableSimulation(FALSE);

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	if (pBeData)
	{
		SHTBStuff* pHTBStuff = pBeData->GetHTBStuff();
		CNtlSLEventGenerator::TSSkillUse_HTB(m_pActor->GetSerialID(), pHTBStuff->hTargetSerialId, pHTBStuff->uiSkillTblId);
	}

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActHTBState::Update(RwReal fElapsed)
{
	CNtlFSMCharActStateBase::Update(fElapsed);

	if(IsFinish())
	{
		if(m_byHTPStepState == EHTB_PROGRESS)
		{
			NextStepProc();

			if(IsFinishHTBStep())
			{
				if(StepCancelProc())
				{
					ResetFinish();
				}
				else
				{
					if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
					{
						RwV3d vPos = m_pActor->GetPosition();
						RwV3d vDir = m_pActor->GetDirection();
						API_GetSLPacketGenerator()->SendCharHTBForward(vPos, vDir);
					}

                    // HTB가 끝나고도 타겟이 살아있으면 자동 공격한다.
                    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
                    {
                        CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
                        SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
                        CNtlSobActor* pActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pHTBStuff->hTargetSerialId);
                        if(pActor && !pActor->IsDie() && !Logic_isEnemyTargetOcclusion(pActor->GetSerialID()))
                        {
                            CNtlSLEventGenerator::SobAttackSelect(pHTBStuff->hTargetSerialId);
                        }
                    }

					Finish();
				}
			}
			else
			{
				// next step이 존재한다면. htb 상태를 finish 하지 않는다.
				ResetFinish();
			}
		}
		else if(m_byHTPStepState == EHTB_CANCEL)
		{
			if(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER)
			{
				RwV3d vPos = m_pActor->GetPosition();
				RwV3d vDir = m_pActor->GetDirection();
				API_GetSLPacketGenerator()->SendCharHTBForward(vPos, vDir);
			}
		}
	}
}

RwUInt32 CNtlFSMCharActHTBState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActHTBState::HandleEvents");

	if(pMsg.Id == g_EventWeightTime)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
		SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();

		CNtlSob *pSobTarObj = GetNtlSobManager()->GetSobObject(pHTBStuff->hTargetSerialId);
		if(pSobTarObj)
		{
			SNtlEventWeightTime *pSobWeightTime = reinterpret_cast<SNtlEventWeightTime*>(pMsg.pData);
			SEventWeightTime *pWeightTimeData = reinterpret_cast<SEventWeightTime*>(pSobWeightTime->pData);
			GetNtlSobElapsedControlManager()->CreateController(pSobTarObj, pWeightTimeData->fLifeTime, pWeightTimeData->fWeightValue);
		}

	}
	else if(pMsg.Id == g_EventSobTargetSelect)
	{
        if(!IsFinish())
        {
		    NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
        }
	}
	else if(pMsg.Id == g_EventSobAttackSelect)
	{
        if(!IsFinish()) // An automatic attack should be possible at the end.
        {
		    NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
        }
	}

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActHTBSandbagState)

CNtlFSMCharActHTBSandbagState::CNtlFSMCharActHTBSandbagState()
{
	SetStateId(NTL_FSMSID_HTB_SANDBAG);
	SetStateName(NTL_FSMSN_HTB_SANDBAG);
	SetFlags(NTL_FSMSF_NOT_MOVE | NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SIT | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | 
			NTL_FSMSF_NOT_CAHNGE_HEADING | NTL_FSMSF_NOT_HURT_STATE | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK);

	m_bFinishSandbag	= FALSE;
	m_bySetType			= NTL_ATTACK_NORMAL;
    m_bSobFaint         = FALSE;
}

void CNtlFSMCharActHTBSandbagState::DetermineBehavior(RWS::CMsg &pMsg)
{
	RwInt32 iBehaviorCount = GetBehaviorCount();
	NTL_ASSERT(iBehaviorCount <= 1, "CNtlFSMCharActHTBSandbagState::DeterBehavior => behavior count is over 1"); 

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff(); 

	RwUInt8 byAttBehavior = pHitStuff->uiAttr.bitBehavior;
	RwUInt8 byAttResult = pHitStuff->uiAttr.bitResult;

	if( byAttBehavior == NTL_ATTACK_NORMAL || byAttBehavior == NTL_ATTACK_PUSH)
	{
		if(byAttResult == NTL_ATTACK_RESULT_BLOCK || byAttResult == NTL_ATTACK_RESULT_DODGE)
		{
			return;
		}

		CNtlBehaviorBase *pBehaviorBase = GetBehavior(0);
		if(pBehaviorBase)
			pBehaviorBase->HandleEvents(pMsg);
		else
		{
			CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharHurt;
			AddBehavior(pBehavior);
		}
	}
	else if(byAttBehavior == NTL_ATTACK_KNOCKDOWN)
	{
		BehaviorKnockDownProc();
	}
	else if(byAttBehavior == NTL_ATTACK_TOSS_UP)
	{
		BehaviorTossUp();
	}
	else if(byAttBehavior == NTL_ATTACK_TOSS_DOWN)
	{
		BehaviorTossDown();
	}
}

void CNtlFSMCharActHTBSandbagState::BehaviorKnockDownProc(void)
{
	RemoveAllBehavior();
	CNtlBehaviorCharKnockDown *pBehavior = NTL_NEW CNtlBehaviorCharKnockDown;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff(); 
	pBehavior->SetKnockDownDir(&pHitStuff->vPushDir);

	AddBehavior(pBehavior);
}

void CNtlFSMCharActHTBSandbagState::BehaviorTossUp(void)
{
	// 잠시 동안 지형 높이 check와 ground fly flag를 끈다.
	RwUInt32 uiFlags = m_pActor->GetFlags();
	uiFlags = (uiFlags | SLFLAG_NOT_WORLD_HEIGHT_CHECK) & ~SLFLAG_CAN_GROUND_FLY;
	m_pActor->SetFlags( uiFlags);

	RemoveAllBehavior();
	CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharHurtTossUp;
	AddBehavior(pBehavior);

	m_bySetType = NTL_ATTACK_TOSS_UP;
}

void CNtlFSMCharActHTBSandbagState::BehaviorTossDown(void)
{
    // 잠시 동안 지형 높이 check와 ground fly flag를 끈다.
    RwUInt32 uiFlags = m_pActor->GetFlags();
    m_pActor->SetFlags( uiFlags & ~SLFLAG_NOT_WORLD_HEIGHT_CHECK);

	RemoveAllBehavior();
	CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharHurtTossDown;
	AddBehavior(pBehavior);
}

void CNtlFSMCharActHTBSandbagState::BehaviorHomingDown(void)
{
	RwUInt32 uiFlags = m_pActor->GetFlags();
	m_pActor->SetFlags( uiFlags & ~SLFLAG_NOT_WORLD_HEIGHT_CHECK);

	RemoveAllBehavior();

	CNtlBehaviorCharHomingDown *pHomingDown = NTL_NEW CNtlBehaviorCharHomingDown;
	AddBehavior(pHomingDown);
}

void CNtlFSMCharActHTBSandbagState::Enter(void)
{
	// 좌표 setting
	RwV3d vPos = m_pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
	vPos.y = sHStuff.fFinialHeight;

	m_pActor->SetPosition(&vPos);

	// RP Stock Image 띄우기.
	Logic_ShowSandBagRPSelectGUI(m_pActor);

	CNtlFSMCharActStateBase::Enter();
}

void CNtlFSMCharActHTBSandbagState::Exit(void)
{
	RwUInt32 uiFlags = m_pActor->GetFlags();
	m_pActor->SetFlags( uiFlags & ~SLFLAG_NOT_WORLD_HEIGHT_CHECK);

	// ground fly 객체이면 다시 ground fly flag를 켠다.
	if(Logic_IsGroundFlyActor(m_pActor))
	{
		m_pActor->SetFlags(m_pActor->GetFlags() | SLFLAG_CAN_GROUND_FLY);
	}

    if(m_bSobFaint) // 몹이 이미 죽어 있는 경우
    {
        // If you just jump to Faint, you will be able to play the Faint animation again after the knockdown, so it will not be processed with alpha. (Because he was dead anyway)
        m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(1.0f, 0.0f, 1.0f, 0.0f, TRUE);
        SetNextStateId(NTL_FSMSID_FAINTING);        
    }
	
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActHTBSandbagState::Update(RwReal fElapsed)
{
	CNtlFSMCharActStateBase::Update(fElapsed);

	ResetFinish();

	if(m_bFinishSandbag && GetBehaviorCount() == 0)
	{
		Finish();
	}
}

RwUInt32 CNtlFSMCharActHTBSandbagState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActHTBSandbagState::HandleEvents");

	if(pMsg.Id == g_EventSobHit)
	{
		SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
		FSMEvent_CharActHit(m_pActor, pHit); 

		LuaExec_Hurt(pHit->sHitStuff.hAttackerSerialId, pHit->sHitStuff.hDefenderSerialId, &pHit->sHitStuff);

		DetermineBehavior(pMsg);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventSobStanding)
	{
		if(m_bySetType == NTL_ATTACK_TOSS_UP)
		{
			BehaviorHomingDown();
		}

		m_bFinishSandbag = TRUE;
		CNtlFSMCharActStateBase::HandleEvents(pMsg);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventSobTargetSelect)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventSobAttackSelect)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
    else if(pMsg.Id == g_EventSobFainting)
    {
        // HTB 맞는중에 Faint가 들어오면 애니메이션이 멈추고 죽는 문제가 있다.
        // 그래서 HTB 중에서는 Faint 상태로 전이하지 않고 Exit에서 처리한다.
        m_bSobFaint = TRUE;        
        NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
    }

	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActItemReadyState)

CNtlFSMCharActItemReadyState::CNtlFSMCharActItemReadyState()
{
	SetStateId(NTL_FSMSID_ITEM_READY);
	SetStateName(NTL_FSMSN_ITEM_READY);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_SKIP_ATTACK_CLICK);
}

void CNtlFSMCharActItemReadyState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

    RwUInt32 uiIdleAnim = Logic_GetIdleAnimationID(m_pActor);
    pSobProxy->SetBaseAnimation(uiIdleAnim);	
}

void CNtlFSMCharActItemReadyState::Update(RwReal fElapsed)
{
	// 여기에서는 behavior가 없기 때문에 굳이 state를 업데이트 할 필요가 없다.
	//	CNtlFSMCharActStateBase::Update(fElapsed); 
}

void CNtlFSMCharActItemReadyState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActItemReadyState::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobItemCancel)
	{
		Finish();
		return NTL_FSM_EVENTRES_CHANGE_STATE;
	}
	else if(pMsg.Id  == g_EventSobItemUseAction)
	{
		Finish();
		return NTL_FSM_EVENTRES_CHANGE_STATE;
	}
	else if(pMsg.Id == g_EventSobItemUseResFailed)
	{
		Finish();
		return NTL_FSM_EVENTRES_CHANGE_STATE;
	}

	return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActItemCastingState)

CNtlFSMCharActItemCastingState::CNtlFSMCharActItemCastingState()
{
	SetStateId(NTL_FSMSID_ITEM_CASTING);
	SetStateName(NTL_FSMSN_ITEM_CASTING);
	SetFlags(NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_RUNJUMP | NTL_FSMSF_SKIP_STANDUP | NTL_FSMSF_SKIP_ATTACK_CLICK | 
			NTL_FSMSF_NOT_HURT_STATE);

	m_pPLEntity = NULL;
    m_tblUseItemTableIndex = INVALID_TBLIDX;
}

void CNtlFSMCharActItemCastingState::Enter(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
    SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();

    CItemTable *pItemTable = API_GetTableContainer()->GetItemTable();		
    sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData(pSkillStuff->uiSkillTblId) );
    NTL_ASSERT(pItemTblData, "CNtlFSMCharActItemCastingState::Enter => item table data not found" << "(" << pSkillStuff->uiSkillTblId << ")");
    if(!pItemTblData)
        return;

    m_tblUseItemTableIndex = pItemTblData->Use_Item_Tblidx;
    CUseItemTable *pUseItemTbl = API_GetTableContainer()->GetUseItemTable();
    sUSE_ITEM_TBLDAT *pUseItemTblData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( pUseItemTbl->FindData(m_tblUseItemTableIndex) );
    NTL_ASSERT(pUseItemTblData, "CNtlFSMCharActItemCastingState::Enter => use item table data not found" << "(" << pItemTblData->Use_Item_Tblidx << ")");
    if(!pUseItemTblData)
        return;
        
	if(m_pActor->GetClassID() == SLCLASS_AVATAR)
	{
        // casting gui를 위한 event를 보낸다.
		if(strlen(pUseItemTblData->szCasting_Effect) > 0)
		{
			RwV3d vOffset;
			RwReal fHeight = (RwReal)pUseItemTblData->byCastingEffectPosition/(RwReal)100;
			CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, fHeight, 0.0f);
			m_pPLEntity = m_pActor->GetSobProxy()->CreatePLChildEffect(pUseItemTblData->szCasting_Effect, vOffset);
		}

		CNtlSLEventGenerator::SobCastingDirect( m_pActor, FALSE, TRUE );
	}

    // 애니메이션
    
    m_pActor->GetSobProxy()->SetBaseAnimation(pUseItemTblData->wCasting_Animation_Start, FALSE);

	CNtlFSMCharActStateBase::Enter();
}

void CNtlFSMCharActItemCastingState::Exit(void)
{
	if(m_pPLEntity)
	{
		m_pActor->GetSobProxy()->DeletePLChildEffect(m_pPLEntity);
		m_pPLEntity = NULL;
	}

	// casting gui를 위한 event를 보낸다.
	if(m_pActor->GetClassID() == SLCLASS_AVATAR)
	{
		CNtlSLEventGenerator::SobCastingDirect( m_pActor, FALSE, FALSE );
	}

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActItemCastingState::Update(RwReal fElapsed)
{
	if(m_pActor->GetClassID() == SLCLASS_AVATAR)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
		SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
		pSkillStuff->fCurrCastingTime += fElapsed;
		if(pSkillStuff->fCurrCastingTime >= pSkillStuff->fMaxCastingTime)
			pSkillStuff->fCurrCastingTime = pSkillStuff->fMaxCastingTime;
	}
}

RwUInt32 CNtlFSMCharActItemCastingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActItemCastingState::HandleEvents");

	if(pMsg.Id  == g_EventSobItemUseAction)
	{
        if(m_pActor && m_pActor->GetSobProxy())
        {
            sUSE_ITEM_TBLDAT* pUseItemTblData = (sUSE_ITEM_TBLDAT*)API_GetTableContainer()->GetUseItemTable()->FindData(m_tblUseItemTableIndex);
            if(pUseItemTblData && pUseItemTblData->wAction_Animation_Index != INVALID_ANIMATIONID)
            {                
                m_pActor->GetSobProxy()->SetBaseAnimation(pUseItemTblData->wAction_Animation_Index, FALSE);
                NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
            }
        }
        
        Finish();
        return NTL_FSM_EVENTRES_CHANGE_STATE;        
	}
    else if(pMsg.Id == g_EventAnimEnd)
    {
        if(m_pActor && m_pActor->GetSobProxy())
        {
            sUSE_ITEM_TBLDAT* pUseItemTblData = (sUSE_ITEM_TBLDAT*)API_GetTableContainer()->GetUseItemTable()->FindData(m_tblUseItemTableIndex);
            if(pUseItemTblData)
            {
                SNtlEventAnimEnd* pEventAnimEnd = (SNtlEventAnimEnd*)pMsg.pData;
                if(pEventAnimEnd->uiBaseAnimKey == pUseItemTblData->wCasting_Animation_Start)
                {
                    m_pActor->GetSobProxy()->SetBaseAnimation(pUseItemTblData->wCasting_Animation_Loop);
                }
                else if(pEventAnimEnd->uiBaseAnimKey == pUseItemTblData->wAction_Animation_Index)
                {
                    Finish();
                    return NTL_FSM_EVENTRES_CHANGE_STATE;
                }
            }            
        }        
    }
	
	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlFSMCharActFaintingState)

CNtlFSMCharActFaintingState::CNtlFSMCharActFaintingState()
{
	SetStateId(NTL_FSMSID_FAINTING);
	SetStateName(NTL_FSMSN_FAINTING);
	SetFlags(NTL_FSMSF_NOT_ALL & ~NTL_FSMSF_NOT_GROUNDMOVE);
}

void CNtlFSMCharActFaintingState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// 죽을 때 animation sync를 동기시킨다.
	GetNtlSobManager()->GetAnimSyncManager()->RemoveAnimSyncTarget(m_pActor->GetSerialID());
	Logic_SetLp(m_pActor, 0);

	// animation setting
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	pSobProxy->SetBaseAnimation(FAINTING, FALSE);
}

void CNtlFSMCharActFaintingState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();

	if ( m_pActor && SLCLASS_MONSTER == m_pActor->GetClassID() )	/// woosungs_test 20090804
	{
		CNtlSLEventGenerator::MobDie(m_pActor->GetLastAttackerID());
	}
}

void CNtlFSMCharActFaintingState::Update(RwReal fElapsed)
{
	// 여기에서는 behavior가 없기 때문에 굳이 state를 업데이트 할 필요가 없다. 
}


RwUInt32 CNtlFSMCharActFaintingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActDieState::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_DIE);
	}
	else if(pMsg.Id == g_EventSobSkillCancel || pMsg.Id == g_EventSobItemCancel)
	{
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}

	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlFSMCharActDieState)

CNtlFSMCharActDieState::CNtlFSMCharActDieState()
{
	SetStateId(NTL_FSMSID_DIE);
	SetStateName(NTL_FSMSN_DIE);
	SetFlags(NTL_FSMSF_NOT_ALL);

	m_bDirectDie = FALSE;
	m_byDieState = DIE_FADEOUT;
}

void CNtlFSMCharActDieState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// 죽을 때 animation sync를 동기시킨다.
	GetNtlSobManager()->GetAnimSyncManager()->RemoveAnimSyncTarget(m_pActor->GetSerialID());
	Logic_SetLp(m_pActor, 0);

	// character picking을 할 수 없게 한다.
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	
	// ground fly actor이면?
	// ground fly 기능(SLFLAG_CAN_GROUND_FLY)을 빼준다.
	if(Logic_IsGroundFlyActor(m_pActor))
	{
		RwUInt32 uiFlags = m_pActor->GetFlags();
		uiFlags = uiFlags & ~SLFLAG_CAN_GROUND_FLY;
		m_pActor->SetFlags(uiFlags);
	} 

	// 부활 가능한 캐릭터라면?
	if(m_pActor->GetFlags() & SLFLAG_CAN_REVIVAL)
	{
		m_byDieState = DIE_END;

        // 공중에서 점프중에 죽는 경우도 있기 때문에, 위치는 지면으로 이동시킨다. (어색하면 나중에 변경한다)
        RwV3d vPos = m_pActor->GetPosition();
        SWorldHeightStuff sHStuff;
        Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
        vPos.y = sHStuff.fFinialHeight;
        m_pActor->SetPosition(&vPos);
	}
	else
	{
		pPLCharacter->SetPicking(FALSE);

        // 파티 공유 타겟에 포함되어 있으면 제거한다.
        RwInt32 nSlot = Logic_isPartyShareTarget(m_pActor->GetSerialID());
        if(nSlot > -1)
        {
            CNtlSLEventGenerator::ShareTargetRelease(m_pActor->GetSerialID());
        }
	}    


	// animation 실행.
	if(m_bDirectDie)
	{
		pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f);

		pSobProxy->SetBaseAnimation(FAINTING, FALSE, 1.0f);
	}
}

void CNtlFSMCharActDieState::Update(RwReal fElapsed)
{
	if(m_byDieState == DIE_FADEOUT)
	{
		UpdateFadeOut(fElapsed);
	}
}

void CNtlFSMCharActDieState::UpdateFadeOut(RwReal fElapsed)
{
#define DIE_FADEOUT_START_TIME	2.0f
#define DIE_FADEOUT_TIME		3.0f

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->fTime += fElapsed;
	if(pCtrlStuff->fTime >= DIE_FADEOUT_START_TIME)
	{
		CNtlSobCharProxy *pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pActor->GetSobProxy() );
		RwReal fDeltaTime = pCtrlStuff->fTime - DIE_FADEOUT_START_TIME;
		RwUInt8 byAlpha = 255 - (RwUInt8)( 255.0f/DIE_FADEOUT_TIME * fDeltaTime );
		RwReal fInkThickness = DEFAULT_INK_THICKNESS - ( DEFAULT_INK_THICKNESS/DIE_FADEOUT_TIME * fDeltaTime );
		if(fInkThickness <= 0.0f)
			fInkThickness = 0.0f;

		pSobCharProxy->SetAlpha(byAlpha);
		pSobCharProxy->SetInkThickness(fInkThickness);
	}

	if(pCtrlStuff->fTime >= DIE_FADEOUT_START_TIME + DIE_FADEOUT_TIME)
	{
		CNtlSobCharProxy *pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pActor->GetSobProxy() );

		pSobCharProxy->SetAlpha(1);
		pSobCharProxy->SetInkThickness(0.0f);

		// 자신의 pet인 경우.
		if( (m_pActor->GetFlags() & SLFLAG_SERVER_SENDER) && (m_pActor->GetClassID() == SLCLASS_PET) )
		{
			API_GetSLPacketGenerator()->SendPetDismissReq(m_pActor->GetServerSyncAvatarType());
		}

		m_byDieState = DIE_END;
	}
}

void CNtlFSMCharActDieState::Exit(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pActor->GetSobProxy() );
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	pSobProxy->SetAnimBlendNoneOneTime();

	if(m_pActor->GetClassID() != SLCLASS_AVATAR)
	{
		pPLCharacter->SetPicking(TRUE);
	}

	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActDieState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActDieState::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		m_bDirectDie = TRUE;
		m_byDieState = DIE_END;
		NTL_RETURN( NTL_FSM_EVENTRES_PASS );
	}
	else if(pMsg.Id == g_EventSobSkillCancel || pMsg.Id == g_EventSobItemCancel)
	{
		NTL_RETURN( NTL_FSM_EVENTRES_BLOCK );
	}
	
	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlFSMCharActTeleportState)

CNtlFSMCharActTeleportState::CNtlFSMCharActTeleportState()
{
	SetStateId(NTL_FSMSID_TELEPORT);
	SetStateName(NTL_FSMSN_TELEPORT);
	SetFlags(NTL_FSMSF_NOT_ALL);
}

void CNtlFSMCharActTeleportState::Enter(void)
{
	m_pVisualSystemEffect = m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend( 0.f, 10000.f, TRUE );

	GetFSMAgent()->RemoveAllEvents();

	if ( m_pActor->GetClassID() == SLCLASS_AVATAR )
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

		if ( TELEPORT_TYPE_NPC_PORTAL == pCtrlStuff->uExtra.sTeleport.byTeleportType )
		{
			CNtlSLEventGenerator::TSTeleport();
		}
	}

	CNtlFSMCharActStateBase::Enter();
}

void CNtlFSMCharActTeleportState::Update(RwReal fElapsed)
{
	
}

void CNtlFSMCharActTeleportState::Exit(void)
{
	if ( m_pVisualSystemEffect )
	{
		m_pActor->GetSobProxy()->RemoveVisualSystemEffectAlpha( m_pVisualSystemEffect );
		m_pVisualSystemEffect = NULL;
	}

	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActTeleportState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActTeleportState::HandleEvents");
	
	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}

RwBool CNtlFSMCharActTeleportState::IsActorActiveState(void)
{
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActPrivateShopState)

CNtlFSMCharActPrivateShopState::CNtlFSMCharActPrivateShopState()
{
	SetStateId(NTL_FSMSID_PRIVATESHOP);
	SetStateName(NTL_FSMSN_PRIVATESHOP);
	SetFlags(NTL_FSMSF_NOT_ALL & ~NTL_FSMSF_NOT_HURT_CAN);

	m_bDirectShop	= FALSE;
    pSystemEffect   = NULL;
}

void CNtlFSMCharActPrivateShopState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

	// animation 실행.
	if(m_bDirectShop)
	{
		pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f);

		if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			pSobProxy->SetBaseAnimation(NML_STAFF_SIT_IDLE, TRUE);
		else
			pSobProxy->SetBaseAnimation(SIT_IDLE, TRUE);
	}
	else
	{
		if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			pSobProxy->SetBaseAnimation(NML_STAFF_SIT_DOWN, FALSE);
		else
			pSobProxy->SetBaseAnimation(SIT_DOWN, FALSE);
	}

	RwUInt32 uiFlags = m_pActor->GetFlags();
	m_pActor->SetFlags(uiFlags | SLFLAG_TARGET_NEAR_APPROACH);

    pSystemEffect = m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(0.0f, 0.0f, TRUE);
}

void CNtlFSMCharActPrivateShopState::Exit(void)
{
    if(pSystemEffect)
    {
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectAlpha(pSystemEffect);
    }

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	if (pBeData)
	{
		SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

		CNtlSLEventGenerator::PrivateShopNameTag(m_pActor->GetSerialID(),
			pCtrlStuff->uExtra.sPrivateShop.bShopOwnerEmpty,
			PRIVATESHOP_STATE_NONE,
			pCtrlStuff->uExtra.sPrivateShop.wchShopName);
	}

	RwUInt32 uiFlags = m_pActor->GetFlags();
	m_pActor->SetFlags(uiFlags & (~SLFLAG_TARGET_NEAR_APPROACH) );

	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	pPLCharacter->SetBlend(BLEND_TWEEN);

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActPrivateShopState::Update(RwReal fElapsed)
{
}

RwUInt32 CNtlFSMCharActPrivateShopState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActPrivateShopState::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		m_bDirectShop = TRUE;
		NTL_RETURN( NTL_FSM_EVENTRES_PASS );
	}
	else if(pMsg.Id == g_EventAnimEnd)
	{
		SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
		if(pAnimEnd->uiBaseAnimKey == NML_STAFF_SIT_DOWN || pAnimEnd->uiBaseAnimKey == SIT_DOWN)
		{
			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());			
			sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

			if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
				pSobProxy->SetBaseAnimation(NML_STAFF_SIT_IDLE, TRUE);
			else
				pSobProxy->SetBaseAnimation(SIT_IDLE, TRUE);
		}
		else 
		{
			Finish();
		}
	}
	else if(pMsg.Id == g_EventPrivateShopState)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

		RwUInt8 byOldShopState = pCtrlStuff->uExtra.sPrivateShop.byShopState;

		SNtlEventPrivateShopState *pPrivateShopState = reinterpret_cast<SNtlEventPrivateShopState*>( pMsg.pData );
		FSMEvent_CharActPrivateShopState(m_pActor, pPrivateShopState);

		SCtrlStuffPrivateShop *pPrivateShopStuff = &pCtrlStuff->uExtra.sPrivateShop;

		if(byOldShopState != pPrivateShopStuff->byShopState)
		{
			CNtlSLEventGenerator::PrivateShopNameTag(m_pActor->GetSerialID(), 
													pPrivateShopStuff->bShopOwnerEmpty,
													pPrivateShopStuff->byShopState,
													pPrivateShopStuff->wchShopName);
		}

		NTL_RETURN( NTL_FSM_EVENTRES_PASS );
	}
	else if(pMsg.Id == g_EventSobStanding)
	{
		Finish();

		NTL_RETURN( NTL_FSM_EVENTRES_PASS );
	}
	
	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActDirectionState)

CNtlFSMCharActDirectionState::CNtlFSMCharActDirectionState()
{
	SetStateId(NTL_FSMSID_DIRECTION);
	SetStateName(NTL_FSMSN_DIRECTION);
	SetFlags(NTL_FSMSF_NOT_ALL);

	m_bTimeLoop		= FALSE;
	m_fLoopTime		= 0.0f;
	m_fCurrTime		= 0.0f;
	m_byState		= FSMDS_NORMAL_UPDATE;
	m_byFuncFlag	= DIRECTION_FUNC_FLAG_TIMEOUT;
	m_pSpawnAlpha	= NULL;
}

void CNtlFSMCharActDirectionState::Enter(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	m_bTimeLoop = pCtrlStuff->uExtra.sDirection.bTimeLoop;
	m_fLoopTime = pCtrlStuff->uExtra.sDirection.fLoopTime;

	// Turn animation interpolation unconditionally when playing animation play(fluorspar)
	// Ask anyone who thinks otherwise.
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	pSobProxy->SetAnimBlendNoneOneTime();

	if(pCtrlStuff->uExtra.sDirection.bAnimUse)
	{
		if(m_bTimeLoop)
		{
			pSobProxy->SetBaseAnimation(pCtrlStuff->uExtra.sDirection.uiAnimId, TRUE);
		}
		else
		{
			pSobProxy->SetBaseAnimation(pCtrlStuff->uExtra.sDirection.uiAnimId, FALSE);
		}
	}
	else
	{
		CDirectionLinkTable *pDirectLinkTbl = API_GetTableContainer()->GetDirectionLinkTable();
		sDIRECTION_LINK_TBLDAT *pDirectLinkTblData = reinterpret_cast<sDIRECTION_LINK_TBLDAT*>( pDirectLinkTbl->FindData(pCtrlStuff->uExtra.sDirection.uiTblId) );

		if(pDirectLinkTblData && pDirectLinkTblData->dwAnimationID != INVALID_DWORD)
		{
			m_byFuncFlag = pDirectLinkTblData->byFuncFlag;

			if(pDirectLinkTblData->byFuncFlag & DIRECTION_FUNC_FLAG_LOOP)
			{
				pSobProxy->SetBaseAnimation(pDirectLinkTblData->dwAnimationID);
			}
			else
			{
				pSobProxy->SetBaseAnimation(pDirectLinkTblData->dwAnimationID, FALSE);
			}
		}
		else
		{
			if ( pCtrlStuff->uExtra.sDirection.byDirectType == DIRECT_PLAY_VEHICLE )
			{
				m_byFuncFlag = DIRECTION_FUNC_FLAG_KEEPUP;
				pSobProxy->SetBaseAnimation(NML_IDLE_LOOP);
			}
		}
	}
}

void CNtlFSMCharActDirectionState::Exit(void)
{
	m_pActor->GetSobProxy()->SetAnimBlendNoneOneTime();

	CNtlFSMCharActStateBase::Exit();
}


void CNtlFSMCharActDirectionState::NormalUpdate(RwReal fElapsed)
{
	if(m_bTimeLoop)
	{
		m_fCurrTime += fElapsed;

		if(m_fCurrTime >= m_fLoopTime)
		{
			SetNextStateId(NTL_FSMSID_IDLE);
			Finish();
		}
	}
}

void CNtlFSMCharActDirectionState::TMQEnterFailUpdate(RwReal fElapsed)
{
	if(m_pSpawnAlpha == NULL)
	{
		SetNextStateId(NTL_FSMSID_IDLE);
		Finish();
		return;
	}

	if(!m_pActor->GetSobProxy()->IsExitSystemEffect(m_pSpawnAlpha))
	{
		SetNextStateId(NTL_FSMSID_IDLE);
		Finish();
	}
}

void CNtlFSMCharActDirectionState::Update(RwReal fElapsed)
{
	if(m_byState == FSMDS_NORMAL_UPDATE)
	{
		NormalUpdate(fElapsed);
	}
	else if(m_byState == FSMDS_NORMAL_TMQENTER_FAIL)
	{
		TMQEnterFailUpdate(fElapsed);
	}
}

RwUInt32 CNtlFSMCharActDirectionState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActDirectionState::HandleEvents");

	if(pMsg.Id == g_EventSobStanding) 
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

		switch ( pCtrlStuff->uExtra.sDirection.byDirectType )
		{
		case DIRECT_PLAY_TQS_START:
			{
				CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
				m_pSpawnAlpha	= pSobProxy->AddVisualSystemEffectAlphaBlend(0.0f, 1.0f, 0.2f, 0.2f, FALSE);	

                // GM 이면 이펙트를 표시하지 않는다.
                if(m_pActor->GetClassID() == SLCLASS_PLAYER)
                {
                    CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)m_pActor->GetFSMLayer()->GetFSMAgent();
                    if(!pAgent->IsTransparent())
                        m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_TELEPORT_DESPAWN, NULL, TRUE);
                }
                
			}
			break;

		case DIRECT_PLAY_TQS_ARRIVE:
			{
				SetNextStateId(NTL_FSMSID_IDLE);
				Finish();
			}
			break;

		case DIRECT_PLAY_TQS_LEAVE:
			{
				CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
				m_pSpawnAlpha	= pSobProxy->AddVisualSystemEffectAlphaBlend(0.0f, 1.0f, 0.2f, 0.2f, FALSE);				
                
                // GM 이면 이펙트를 표시하지 않는다.
                if(m_pActor->GetClassID() == SLCLASS_PLAYER)
                {
                    CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)m_pActor->GetFSMLayer()->GetFSMAgent();
                    if(!pAgent->IsTransparent())
                        m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_TELEPORT_DESPAWN, NULL, TRUE);
                }
			}
			break;

		case DIRECT_PLAY_VEHICLE:
			{
				SetNextStateId(NTL_FSMSID_IDLE);
				Finish();
			}
			break;

		default:
			{
				SetNextStateId(NTL_FSMSID_IDLE);
				Finish();
			}
			break;
		}
	}

	else if(pMsg.Id == g_EventAnimEnd)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

		switch ( pCtrlStuff->uExtra.sDirection.byDirectType )
		{
			case DIRECT_PLAY_TQS_START:
			break;

			case DIRECT_PLAY_TQS_ARRIVE:
			{
				SetNextStateId(NTL_FSMSID_IDLE);
				Finish();
			}
			break;

			case DIRECT_PLAY_TQS_LEAVE:
			break;

			case DIRECT_PLAY_VEHICLE:
			break;

			default:
			{
				if( !(m_byFuncFlag & DIRECTION_FUNC_FLAG_KEEPUP) )
				{
					SetNextStateId(NTL_FSMSID_IDLE);
					Finish();
				}
			}
			break;
		}
	}

	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActDespawnState)

CNtlFSMCharActDespawnState::CNtlFSMCharActDespawnState()
{
	SetStateId(NTL_FSMSID_DESPAWN);
	SetStateName(NTL_FSMSN_DESPAWN);
	SetFlags(NTL_FSMSF_NOT_ALL);

	m_pDespawn1Alpha	= NULL;
}

void CNtlFSMCharActDespawnState::Enter(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();

	if(Logic_IsDragonBallNPCActor(m_pActor->GetSerialID()))
	{
		m_pDespawn1Alpha	= pSobProxy->AddVisualSystemEffectAlphaBlend(1.0f, 0.0f, 4.5f, 4.5f, FALSE);
		m_byType			= EDESPAWN_DRAGONBALL_NPC;
		m_byState			= DESPAWN_DRAGONBALL_FADEOUT;
	}
	else
	{
		RwUInt8 byTeleportType = pCtrlStuff->uExtra.sTeleport.byTeleportType;
		if(byTeleportType == TELEPORT_TYPE_DEFAULT || byTeleportType == TELEPORT_TYPE_GAME_IN || 
			byTeleportType == TELEPORT_TYPE_GAME_OUT || byTeleportType == TELEPORT_TYPE_TMQ_WORLD_MOVE)
		{
			m_byType	= EDESPAWN_NONE;
			m_byState	= DESPAWN_NONE_START;
		}
		else if(byTeleportType == TELEPORT_TYPE_NPC_PORTAL)
		{
			m_byType	= EDESPAWN_NPC_PORTAL;

			if(m_pActor->GetClassID() == SLCLASS_AVATAR)
			{
				m_byState			= DESPAWN_NPC_PORTAL_CAMERA_RESET;

				CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pCtrlStuff->uExtra.sTeleport.hTeleportNpc) );
				if(pTarActor)
				{
                    pTarActor->GetSobProxy()->SetBaseAnimation(SOC_AGREE, FALSE);
				}
			}
			else
			{
				m_pDespawn1Alpha	= pSobProxy->AddVisualSystemEffectAlphaBlend(1.0f, 0.0f, 0.2f, 0.2f, FALSE);
				m_byState			= DESPAWN_NPC_PORTAL_START;

                CreateDespawnEffect(m_pActor);
			}
		}
		else
		{
			m_pDespawn1Alpha	= pSobProxy->AddVisualSystemEffectAlphaBlend(1.0f, 0.0f, 0.2f, 0.2f, FALSE);
			m_byType			= EDESPAWN_DESPAWN1;
			m_byState			= DESPAWN1_START;
		}
	}

	if(m_byType == EDESPAWN_DESPAWN1)
	{
        CreateDespawnEffect(m_pActor);
	}

	CNtlFSMCharActStateBase::Enter();
}

void CNtlFSMCharActDespawnState::Update(RwReal fElapsed)
{
	if(m_byType == EDESPAWN_NONE)
	{
		if(m_byState == DESPAWN_NONE_START)
		{
			if(m_pActor->GetClassID() == SLCLASS_AVATAR)
			{
				API_GetSLPacketGenerator()->SendCharTeleportReq();
			}

			m_byState = DESPAWN_NONE_END;
		}
		
		// Finish();
	}
	else if(m_byType == EDESPAWN_DESPAWN1)
	{
		UpdateDespawn1(fElapsed);
	}
	else if(m_byType == EDESPAWN_DRAGONBALL_NPC)
	{
		UpdateDragonBallNpc(fElapsed);
	}
	else if(m_byType == EDESPAWN_NPC_PORTAL)
	{
		UpdateNpcPortal(fElapsed);
	}
}

void CNtlFSMCharActDespawnState::UpdateDespawn1(RwReal fElapsed)
{
	if(m_byState == DESPAWN1_END)
		return;

	if(!m_pActor->GetSobProxy()->IsExitSystemEffect(m_pDespawn1Alpha))
	{
		if(m_pActor->GetClassID() == SLCLASS_AVATAR)
		{
			API_GetSLPacketGenerator()->SendCharTeleportReq();
		}

		m_pDespawn1Alpha = NULL;
		m_byState = DESPAWN1_END;
	}
}

void CNtlFSMCharActDespawnState::UpdateDragonBallNpc(RwReal fElapsed)
{
	if(m_byState == DESPAWN_DRAGONBALL_FADEOUT)
		UpdateDragonBallNpcFadeOut(fElapsed);
}

void CNtlFSMCharActDespawnState::UpdateDragonBallNpcFadeOut(RwReal fElapsed)
{
	if(m_byState == DESPAWN_DRAGONBALL_END)
		return;

	if(!m_pActor->GetSobProxy()->IsExitSystemEffect(m_pDespawn1Alpha))
	{
		OnEffectDragonDespawn();
		m_byState = DESPAWN_DRAGONBALL_END;
	}
}

void CNtlFSMCharActDespawnState::UpdateNpcPortal(RwReal fElapsed)
{
	if(m_byState == DESPAWN_NPC_PORTAL_END)
		return;

	if(m_byState == DESPAWN_NPC_PORTAL_CAMERA_RESET)
	{
//		if(GetNtlGameCameraManager()->FindController(CAMERA_CONTROL_NPC) == NULL)
		{
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
			CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pCtrlStuff->uExtra.sTeleport.hTeleportNpc) );
			if(pTarActor)
			{
				if(pTarActor->GetSobProxy()->IsBaseAnimationEnd())
				{
					m_pDespawn1Alpha = m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(1.0f, 0.0f, 0.3f, 0.3f, FALSE);
					pTarActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(1.0f, 0.0f, 0.3f, 10.0f, FALSE);
					m_byState = DESPAWN_NPC_PORTAL_START;
                    
                    CreateDespawnEffect(m_pActor);
                    CreateDespawnEffect(pTarActor);
				}
			}
		}
	}
	else if(m_byState == DESPAWN_NPC_PORTAL_START)
	{
		if(!m_pActor->GetSobProxy()->IsExitSystemEffect(m_pDespawn1Alpha))
		{
			if(m_pActor->GetClassID() == SLCLASS_AVATAR)
			{
				API_GetSLPacketGenerator()->SendCharTeleportReq();
			}

			m_pActor->EnableVisible(FALSE);

			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
			CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pCtrlStuff->uExtra.sTeleport.hTeleportNpc) );
			if(pTarActor)
				pTarActor->EnableVisible(FALSE);

			m_pDespawn1Alpha = NULL;
			m_byState = DESPAWN_NPC_PORTAL_END;
		}
	}
}

void CNtlFSMCharActDespawnState::Exit(void)
{
	if(m_byType == EDESPAWN_DRAGONBALL_NPC)
		ExitDragonBallNpc();


	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActDespawnState::ExitDragonBallNpc(void)
{
    if(m_byState == DESPAWN_DRAGONBALL_END)     // 아직 Alpha 연출이 끝나지 않았는데 Exit가 호출된 경우 이펙트를 보여준다.
    {
        OnEffectDragonDespawn();
    }
}

RwUInt32 CNtlFSMCharActDespawnState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActDespawnState::HandleEvents");

	if (pMsg.Id == g_EventSobAnimPlay)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	/*
	if(pMsg.Id == g_EventSobStanding)
	{
		Finish();
		SetNextStateId(NTL_FSMSID_IDLE);

		NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
	}
	*/
	
	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}   

void CNtlFSMCharActDespawnState::OnEffectDragonDespawn()
{
    CNtlWorldConceptDBC* pDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);
    if(pDBC)
    {
        pDBC->SetMyDragon(FALSE);
    }

    CNtlInstanceEffect* pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, "N_SHR_disappear_001", NULL);            
    pEffect->SetPosition(&m_pActor->GetPosition());
}

void CNtlFSMCharActDespawnState::CreateDespawnEffect( CNtlSobActor* pActor )
{
    if(!m_pActor)
        return;

    // GM 이면 이펙트를 표시하지 않는다.
    if(m_pActor->GetClassID() == SLCLASS_PLAYER)
    {
        CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)m_pActor->GetFSMLayer()->GetFSMAgent();
        if(pAgent->IsTransparent())
            return;
    }

    m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_TELEPORT_DESPAWN, NULL, TRUE);
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlFSMCharActLeavingState)

CNtlFSMCharActLeavingState::CNtlFSMCharActLeavingState()
{
	SetStateId(NTL_FSMSID_LEAVING);
	SetStateName(NTL_FSMSN_LEAVING);
	SetFlags(NTL_FSMSF_NOT_ALL);
}

void CNtlFSMCharActLeavingState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();
}

void CNtlFSMCharActLeavingState::Update(RwReal fElapsed)
{
	
}

void CNtlFSMCharActLeavingState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

RwUInt32 CNtlFSMCharActLeavingState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActLeavingState::HandleEvents");
	
	NTL_RETURN( CNtlFSMCharActStateBase::HandleEvents(pMsg) );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlFSMCharActOnBus)

CNtlFSMCharActOnBus::CNtlFSMCharActOnBus() 
{
    SetStateId(NTL_FSMSID_ONBUS);
    SetStateName(NTL_FSMSN_ONBUS);

    SetFlags(NTL_FSMSF_NOT_ALL);

    m_pTargetActor	= NULL;
	m_byRideState	= BUS_RIDE_ON;
	m_fFallingSpeed	= 0.0f; 

    m_pSystemEffect = NULL;
}

void CNtlFSMCharActOnBus::Enter( void ) 
{
    CNtlFSMCharActStateBase::Enter();

    GetFSMAgent()->RemoveAllEvents();

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
    SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
    m_pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pCtrlStuff->sRide.hTargetSerialId);    
	
    if(m_pTargetActor)
    {
        SetRideOn(TRUE);
    }    
}

void CNtlFSMCharActOnBus::Exit( void ) 
{
    if(m_pSystemEffect)
    {
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectAlpha(m_pSystemEffect);
        m_pSystemEffect = NULL;
    }

    CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);

    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_BUS);
    }
   
    CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActOnBus::Update( RwReal fElapsed ) 
{
	// 버스에 탑승 중이면.
	if(m_byRideState == BUS_RIDE_ON)
	{
		if(!m_pTargetActor)    
		{
			// 캐릭이 생성될때 버스보다 먼저 생성되기 때문에 여기서 처리해야 한다.
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
			m_pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pCtrlStuff->sRide.hTargetSerialId);                    

            if(m_pTargetActor)
            {
                SetRideOn(TRUE, FALSE);
            }            
		}
		else
		{
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
            SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
            m_pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pCtrlStuff->sRide.hTargetSerialId);                    
            NTL_ASSERT(m_pTargetActor, __FUNCTION__<<__LINE__<<"Bus Object is None!");
            if(!m_pTargetActor)
            {
                return;
            }

			m_pActor->SetPosition(&m_pTargetActor->GetPosition());
			m_pActor->SetDirection(&m_pTargetActor->GetDirection());
		}
	}
	else if(m_byRideState == BUS_RIDE_OFF_FALLING)
	{
		SWorldHeightStuff sWHStuff;
		Logic_GetWorldHeight(&m_vRideOffPos, sWHStuff);

		m_fFallingSpeed = m_fFallingSpeed - (11.0f * fElapsed);

		m_vRideOffPos.y += m_fFallingSpeed * fElapsed;

		if(m_vRideOffPos.y < sWHStuff.fFinialHeight)
		{
			CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);
			pSobProxy->SetBaseAnimation(JUMP_LANDING_SHORT, FALSE);

			m_vRideOffPos.y = sWHStuff.fFinialHeight;
			m_byRideState = BUS_DIDE_OFF_LANDING;
		}

		m_pActor->SetPosition(&m_vRideOffPos);
	}
}

RwUInt32 CNtlFSMCharActOnBus::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventSobStanding)
    {
		SNtlEventSobStanding *pSobStanding = reinterpret_cast<SNtlEventSobStanding*>( pMsg.pData );
		m_pActor->SetPosition(&pSobStanding->vLoc);
		m_pActor->SetDirection(&pSobStanding->vDir);

		SetRideOn(FALSE);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
    }
	else if(pMsg.Id == g_EventAnimEnd)
    {
        if(m_byRideState == BUS_DIDE_OFF_LANDING)
        {
            SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
            RwUInt32 uiAnimKey = pAnimEnd->uiBaseAnimKey;
            if(uiAnimKey == JUMP_LANDING_SHORT)
            {
                Finish();
                SetNextStateId(NTL_FSMSID_IDLE);        
            }
        }
	}
    else if(pMsg.Id == g_EventSobStateTransition)
    {
        SNtlEventSobStateTransition* pEvent = (SNtlEventSobStateTransition*)pMsg.pData;
        if(pEvent->uiTransState == NTL_FSMSID_IDLE)
        {
            NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
        }
    }

    return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}

void CNtlFSMCharActOnBus::SetRideOn(RwBool bRideOn, RwBool bCreateEffect /* = TRUE */)
{
    if(bRideOn)     // 탑승 연출
    {
        if(bCreateEffect)
        {
            // GM 이면 이펙트를 표시하지 않는다.
            if(m_pActor->GetClassID() == SLCLASS_PLAYER)
            {
                CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)m_pActor->GetFSMLayer()->GetFSMAgent();
                if(!pAgent->IsTransparent())
                {
                    m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_TELEPORT_DESPAWN, NULL, TRUE);
                }   
            }
        }
        

        if(m_pSystemEffect)
        {
            m_pActor->GetSobProxy()->RemoveVisualSystemEffectAlpha(m_pSystemEffect);
            m_pSystemEffect = NULL;
        }

        m_pSystemEffect = m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(0.0f, 0.0f, TRUE);

        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            CNtlSLEventGenerator::CameraBus(m_pTargetActor);
        }

		// 다른 객체에게 알려준다.
		CNtlSLEventGenerator::SobOnBus(m_pActor->GetSerialID(), bRideOn, m_pTargetActor->GetSerialID());
    }
	else
	{
		m_byRideState = BUS_RIDE_OFF_FALLING;

        if(m_pSystemEffect)
        {
            m_pActor->GetSobProxy()->RemoveVisualSystemEffectAlpha(m_pSystemEffect);
            m_pSystemEffect = NULL;
        }

        if(bCreateEffect)
        {
		    m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_TELEPORT_SPAWN, &m_pActor->GetPosition());
        }

		if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_BUS);
	    }


		// 다른 객체에게 알려준다.
        if(m_pTargetActor)
        {
		    CNtlSLEventGenerator::SobOnBus(m_pActor->GetSerialID(), bRideOn, m_pTargetActor->GetSerialID());
        }

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		pBeData->GetCtrlStuff()->sRide.hTargetSerialId = INVALID_SERIAL_ID;    

		m_pTargetActor = NULL;	

		SWorldHeightStuff sWHStuff;
		m_vRideOffPos = m_pActor->GetPosition();

		Logic_GetWorldHeight(&m_vRideOffPos, sWHStuff);

		m_vRideOffPos.y = sWHStuff.fFinialHeight;
		m_vRideOffPos.y += 2.0f;

		m_pActor->SetPosition(&m_vRideOffPos);

		// animation
		CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
		pSobProxy->SetBaseAnimation(JUMP_HOVER, TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlFSMCharActOnVehicle)

CNtlFSMCharActOnVehicle::CNtlFSMCharActOnVehicle() 
{
	SetStateId(NTL_FSMSID_ONVEHICLE);
	SetStateName(NTL_FSMSN_ONVEHICLE);

	SetFlags(NTL_FSMSF_NOT_ALL);

	m_pTargetActor	= NULL;
	m_byRideState	= VEHICLE_RIDE_ON;
	m_fFallingSpeed	= 0.0f; 
}

void CNtlFSMCharActOnVehicle::Enter( void ) 
{
	CNtlFSMCharActStateBase::Enter();

	GetFSMAgent()->RemoveAllEvents();

	m_pActor->EnableVisible(FALSE);

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	m_pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pCtrlStuff->sRide.hTargetSerialId);    

	if(m_pTargetActor)
	{
		SetRideOn(TRUE);
	}    
}

void CNtlFSMCharActOnVehicle::Exit( void ) 
{
	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);

	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActOnVehicle::Update( RwReal fElapsed ) 
{
	// 버스에 탑승 중이면.
	if(m_byRideState == VEHICLE_RIDE_ON)
	{
		if(!m_pTargetActor)    
		{
			// 캐릭이 생성될때 버스보다 먼저 생성되기 때문에 여기서 처리해야 한다.
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
			m_pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pCtrlStuff->sRide.hTargetSerialId);                    
		}
		else
		{
			m_pActor->SetPosition(&m_pTargetActor->GetPosition());
			m_pActor->SetDirection(&m_pTargetActor->GetDirection());
		}
	}
	else if(m_byRideState == VEHICLE_RIDE_OFF_FALLING)
	{
		SWorldHeightStuff sWHStuff;
		Logic_GetWorldHeight(&m_vRideOffPos, sWHStuff);

		m_fFallingSpeed = m_fFallingSpeed - (11.0f * fElapsed);

		m_vRideOffPos.y += m_fFallingSpeed * fElapsed;

		if(m_vRideOffPos.y < sWHStuff.fFinialHeight)
		{
			CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);
			pSobProxy->SetBaseAnimation(JUMP_LANDING_SHORT, FALSE);

			m_vRideOffPos.y = sWHStuff.fFinialHeight;
			m_byRideState = VEHICLE_DIDE_OFF_LANDING;
		}

		m_pActor->SetPosition(&m_vRideOffPos);
	}
}

RwUInt32 CNtlFSMCharActOnVehicle::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventSobStanding)
    {
		SNtlEventSobStanding *pSobStanding = reinterpret_cast<SNtlEventSobStanding*>( pMsg.pData );
		m_pActor->SetPosition(&pSobStanding->vLoc);
		m_pActor->SetDirection(&pSobStanding->vDir);

		SetRideOn(FALSE);

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
    }
	else if(pMsg.Id == g_EventAnimEnd)
    {
        SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
        RwUInt32 uiAnimKey = pAnimEnd->uiBaseAnimKey;
        if(uiAnimKey == JUMP_LANDING_SHORT)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_IDLE);        
		}
	}

    return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}

void CNtlFSMCharActOnVehicle::SetRideOn(RwBool bRideOn, RwV3d vPos /* = ZeroAxis */)
{
    if(bRideOn)     // 탑승 연출
    {
        // GM 이면 이펙트를 표시하지 않는다.
        if(m_pActor->GetClassID() == SLCLASS_PLAYER)
        {
            CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)m_pActor->GetFSMLayer()->GetFSMAgent();
            if(!pAgent->IsTransparent())
            {
                m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_TELEPORT_DESPAWN, NULL, TRUE);
            }
        }
        
        m_pActor->EnableVisible(FALSE);        

        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            CNtlSLEventGenerator::CameraBus(m_pTargetActor);
        }

		// 다른 객체에게 알려준다.
		CNtlSLEventGenerator::SobOnBus(m_pActor->GetSerialID(), bRideOn, m_pTargetActor->GetSerialID());
    }
    else
    {
        // 플레이어가 생성, 버스는 아직 생성되지 않음=> 내가 로딩중 플레이어가 버스에서 내리는 경우.
        // Actor 체크를 하지 않으면 크래시가 발생한다. 그래서 체크 부분 추가 (by agebreak 09.05.21)
        if(!m_pTargetActor)
            return;

		m_byRideState = VEHICLE_RIDE_OFF_FALLING;

        m_pActor->EnableVisible(TRUE);

		m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_TELEPORT_SPAWN, &m_pActor->GetPosition());

		if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_BUS);
        }

		// 다른 객체에게 알려준다.
		CNtlSLEventGenerator::SobOnBus(m_pActor->GetSerialID(), bRideOn, m_pTargetActor->GetSerialID());

		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		pBeData->GetCtrlStuff()->sRide.hTargetSerialId = INVALID_SERIAL_ID;    

		m_pTargetActor = NULL;	

		SWorldHeightStuff sWHStuff;
		m_vRideOffPos = m_pActor->GetPosition();

		Logic_GetWorldHeight(&m_vRideOffPos, sWHStuff);

		m_vRideOffPos.y = sWHStuff.fFinialHeight;
		m_vRideOffPos.y += 2.0f;

		m_pActor->SetPosition(&m_vRideOffPos);

		// animation
		CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
		pSobProxy->SetBaseAnimation(JUMP_HOVER, TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlFSMCharActTurning)

CNtlFSMCharActTurning::CNtlFSMCharActTurning() 
{
    SetStateId(NTL_FSMSID_TURNING);
    SetStateName(NTL_FSMSN_TURNNING);

    SetFlags(NTL_FSMSF_NOT_ALL);
}

void CNtlFSMCharActTurning::Enter( void ) 
{
    CNtlFSMCharActStateBase::Enter();

    m_pActor->GetSobProxy()->SetBaseAnimation(TURN_LEFT, FALSE);
}

void CNtlFSMCharActTurning::Exit( void ) 
{
	m_pActor->GetSobProxy()->SetAnimBlendNoneOneTime();

    CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActTurning::Update( RwReal fElapsed ) 
{
    
}

RwUInt32 CNtlFSMCharActTurning::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventAnimEnd)
    {
        SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
        RwUInt32 uiAnimKey = pAnimEnd->uiBaseAnimKey;
        if(uiAnimKey == TURN_LEFT)
        {
            RwV3d vDir = m_pActor->GetDirection(); 
            vDir *= -1.0f;
            SetNextDirection(vDir);  
            NTL_RETURN(NTL_FSM_EVENTRES_CHANGE_STATE);
        }
    }
    else if(pMsg.Id == g_EventSobStanding)
    {       
		// 버그 수정(형석)
        NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
    }
	else if(pMsg.Id == g_EventSobSecondDestMove)
	{
		RwV3d vDir = m_pActor->GetDirection(); 
		vDir *= -1.0f;
		SetNextDirection(vDir);  
	}

    return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}

void CNtlFSMCharActTurning::SetNextDirection(RwV3d& vDir)
{
	// 버그 수정(형석)

    m_pActor->SetDirection(&vDir);
    
    Finish();
    SetNextStateId(NTL_FSMSID_IDLE);        
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlFSMCharTransformSequela)
CNtlFSMCharTransformSequela::CNtlFSMCharTransformSequela() 
{
    SetStateId(NTL_FSMSID_SEQUELA);
    SetStateName(NTL_FSMSN_SEQUELA);

    SetFlags(NTL_FSMSF_NOT_ALL & ~NTL_FSMSF_NOT_HURT_CAN);
}

void CNtlFSMCharTransformSequela::Enter() 
{
    CNtlFSMCharActStateBase::Enter();

    if(!CheckSwimming())
    {
        SetAnim(0);        
        
        // 상태 chcek.
        RwV3d vPos = m_pActor->GetPosition();
        SWorldHeightStuff sHStuff;
        Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);

        if(vPos.y < sHStuff.fFinialHeight)
            ChangeIdleState(STATE_IDLE_LOOP);
        else if(vPos.y >= sHStuff.fFinialHeight && vPos.y <= sHStuff.fFinialHeight + 0.1f)
            ChangeIdleState(STATE_IDLE_SLIP);
		else
		{
			DBO_WARNING_MESSAGE("GO IDLE 4");
			ChangeIdleState(STATE_IDLE_TOPDOWN);
		}

        //// If you are on a trigger object, check the height because the object may disappear.
        //if(CNtlPLGlobal::m_pWHEntity)
        //{
        //    CNtlSob* pObj = GetNtlSobManager()->GetSobObject(CNtlPLGlobal::m_pWHEntity->GetSerialID());
        //    if(pObj && pObj->GetClassID() == SLCLASS_TRIGGER_OBJECT)
        //    {
        //        m_pActor->SetActorFlags(m_pActor->GetActorFlags() | SLFLAG_ACTOR_CALC_HEIGHT);
        //    }                
        //}        
    }
}

void CNtlFSMCharTransformSequela::Exit() 
{
    CNtlFSMCharActIdleState::Exit();
}

void CNtlFSMCharTransformSequela::Update( RwReal fElapsed ) 
{
    CNtlFSMCharActIdleState::Update(fElapsed);
}

RwUInt32 CNtlFSMCharTransformSequela::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventSobHit)
    {
        SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
        RwUInt8 byBehavior = pHit->sHitStuff.uiAttr.bitBehavior;
        if(byBehavior == NTL_ATTACK_KNOCKDOWN || byBehavior == NTL_ATTACK_SLIDING)
        {
            Finish();
            SetNextStateId(NTL_FSMSID_KNOCKDOWN);
            NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
        }
    }

    return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}

void CNtlFSMCharTransformSequela::SetAnim( RwUInt8 byAnimOrder ) 
{
    m_pActor->GetSobProxy()->SetBaseAnimation(NML_TRANSFORM_SEQUELA);
}

void CNtlFSMCharTransformSequela::ChangeIdleState( RwUInt8 byState ) 
{
    CNtlFSMCharActIdleState::ChangeIdleState(byState);
    SetFlags(NTL_FSMSF_NOT_ALL);
}
//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlFSMCharSpinAttack)
CNtlFSMCharSpinAttack::CNtlFSMCharSpinAttack() 
{
    SetStateId(NTL_FSMSID_SPIN_ATTACK);
    SetStateName(NTL_FSMSN_SPIN_ATTACK);        
    SetFlags(NTL_FSMSF_NOT_ALL & ~NTL_FSMSF_NOT_HURT_CAN & ~NTL_FSMSF_NOT_MOVE);
}

CNtlFSMCharSpinAttack::~CNtlFSMCharSpinAttack() 
{
    
}

void CNtlFSMCharSpinAttack::Enter() 
{
    CNtlFSMCharActStateBase::Enter();

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
    pMoveStuff->Reset();    // Reset to remove existing movement information.   

	m_pActor->GetSobProxy()->SetDisableAniChange(true);

    m_pMoveBehavior = NTL_NEW CNtlBehaviorCharGroundMove();
	AddBehavior(m_pMoveBehavior);    

    m_pActor->GetSobProxy()->SetBaseAnimation(TRANS_IDLE);
}

void CNtlFSMCharSpinAttack::Exit() 
{
    RemoveAllBehavior();    
	m_pMoveBehavior = NULL;

    API_GetSLPacketGenerator()->SendKeyboardMove(m_pActor->GetPosition(), m_pActor->GetDirection(), NTL_MOVE_NONE, m_pActor->GetServerSyncAvatarType());
}

void CNtlFSMCharSpinAttack::Update( RwReal fElapsed ) 
{
    m_pMoveBehavior->Update(fElapsed);
}

RwUInt32 CNtlFSMCharSpinAttack::HandleEvents( RWS::CMsg &pMsg ) 
{
    // Because the MOVE processing is prevented, the processing according to MOVE is separately performed here.
    if(pMsg.Id == g_EventKeyboardMove)
    {
        SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>(pMsg.pData);
		//maybe needs to be updated.. by daneos
        if(pKeyboardMove->byMoveFlags == NTL_MOVE_TURN_L)
        {
            pKeyboardMove->byMoveFlags = NTL_MOVE_F_TURN_L;
        }
        else if(pKeyboardMove->byMoveFlags == NTL_MOVE_TURN_R)
        {
            pKeyboardMove->byMoveFlags = NTL_MOVE_F_TURN_R;
        }

        FSMEvent_CharActKeyboardMove(m_pActor, pKeyboardMove); 
        UpdateBehavior();

        API_GetSLPacketGenerator()->SendKeyboardMove(m_pActor->GetPosition(), m_pActor->GetDirection(), pKeyboardMove->byMoveFlags, m_pActor->GetServerSyncAvatarType());
    }  
    else if(pMsg.Id == g_EventTerrainClick)
    {
        SNtlEventTerrainClick* pEvent = (SNtlEventTerrainClick*)pMsg.pData;
		FSMEvent_CharActTerrainClick(m_pActor, pEvent);
        UpdateBehavior();
        
        API_GetSLPacketGenerator()->SendMouseMove(m_pActor->GetPosition(), pEvent->vPickPos, m_pActor->GetServerSyncAvatarType());
    }
    else if(pMsg.Id == g_EventSobMove)
    {
        SNtlEventSobMove* pEvent = (SNtlEventSobMove*)pMsg.pData;
        FSMEvent_CharActSobMove(m_pActor, pEvent);
        UpdateBehavior();
    }    
    else if(pMsg.Id == g_EventSobHit)
    {
        SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
        RwUInt8 byBehavior = pHit->sHitStuff.uiAttr.bitBehavior;
        if(byBehavior == NTL_ATTACK_KNOCKDOWN || byBehavior == NTL_ATTACK_SLIDING)
        {
            Finish();
            SetNextStateId(NTL_FSMSID_KNOCKDOWN);
            NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
        }
    }

    return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlFSMCharRollingAttack)
CNtlFSMCharRollingAttack::CNtlFSMCharRollingAttack()
{
	SetStateId(NTL_FSMSID_ROLLING_ATTACK);
	SetStateName(NTL_FSMSN_ROLLING_ATTACK);
	SetFlags(NTL_FSMSF_NOT_ALL & ~NTL_FSMSF_NOT_HURT_CAN & ~NTL_FSMSF_NOT_MOVE);
}

CNtlFSMCharRollingAttack::~CNtlFSMCharRollingAttack()
{

}

void CNtlFSMCharRollingAttack::Enter()
{
	CNtlFSMCharActStateBase::Enter();

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->Reset();    // Reset to remove existing movement information.

	m_pActor->GetSobProxy()->SetDisableAniChange(true);

	m_pMoveBehavior = NTL_NEW CNtlBehaviorCharGroundMove();
	AddBehavior(m_pMoveBehavior);

	m_pActor->GetSobProxy()->SetBaseAnimation(TRANS_IDLE);
}

void CNtlFSMCharRollingAttack::Exit()
{
	RemoveAllBehavior();
	m_pMoveBehavior = NULL;

	API_GetSLPacketGenerator()->SendKeyboardMove(m_pActor->GetPosition(), m_pActor->GetDirection(), NTL_MOVE_NONE, m_pActor->GetServerSyncAvatarType());
}

void CNtlFSMCharRollingAttack::Update(RwReal fElapsed)
{
	m_pMoveBehavior->Update(fElapsed);
}

RwUInt32 CNtlFSMCharRollingAttack::HandleEvents(RWS::CMsg &pMsg)
{
	// Because the MOVE processing is prevented, the processing according to MOVE is separately performed here.
	if (pMsg.Id == g_EventKeyboardMove)
	{
		SNtlEventKeyboardMove *pKeyboardMove = reinterpret_cast<SNtlEventKeyboardMove*>(pMsg.pData);
		//maybe needs to be updated.. by daneos
		if (pKeyboardMove->byMoveFlags == NTL_MOVE_TURN_L)
		{
			pKeyboardMove->byMoveFlags = NTL_MOVE_F_TURN_L;
		}
		else if (pKeyboardMove->byMoveFlags == NTL_MOVE_TURN_R)
		{
			pKeyboardMove->byMoveFlags = NTL_MOVE_F_TURN_R;
		}

		FSMEvent_CharActKeyboardMove(m_pActor, pKeyboardMove);
		UpdateBehavior();

		API_GetSLPacketGenerator()->SendKeyboardMove(m_pActor->GetPosition(), m_pActor->GetDirection(), pKeyboardMove->byMoveFlags, m_pActor->GetServerSyncAvatarType());
	}
	else if (pMsg.Id == g_EventTerrainClick)
	{
		SNtlEventTerrainClick* pEvent = (SNtlEventTerrainClick*)pMsg.pData;
		FSMEvent_CharActTerrainClick(m_pActor, pEvent);
		UpdateBehavior();

		API_GetSLPacketGenerator()->SendMouseMove(m_pActor->GetPosition(), pEvent->vPickPos, m_pActor->GetServerSyncAvatarType());
	}
	else if (pMsg.Id == g_EventSobMove)
	{
		SNtlEventSobMove* pEvent = (SNtlEventSobMove*)pMsg.pData;
		FSMEvent_CharActSobMove(m_pActor, pEvent);
		UpdateBehavior();
	}
	else if (pMsg.Id == g_EventSobHit)
	{
		SNtlEventSobHit *pHit = reinterpret_cast<SNtlEventSobHit*>(pMsg.pData);
		RwUInt8 byBehavior = pHit->sHitStuff.uiAttr.bitBehavior;
		if (byBehavior == NTL_ATTACK_KNOCKDOWN || byBehavior == NTL_ATTACK_SLIDING)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_KNOCKDOWN);
			NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
		}
	}

	return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlFSMCharPushing)
CNtlFSMCharPushing::CNtlFSMCharPushing() 
{
    SetStateId(NTL_FSMSID_PUSHING);
    SetStateName(NTL_FSMSN_PUSHING);        
    SetFlags(NTL_FSMSF_NOT_ALL);
}

CNtlFSMCharPushing::~CNtlFSMCharPushing() 
{

}

void CNtlFSMCharPushing::Enter() 
{
    CNtlFSMCharActStateBase::Enter();

    // 데미지 동작
    CNtlBehaviorBase *pBehavior = NTL_NEW CNtlBehaviorCharHurt;
    AddBehavior(pBehavior);

    // Push 동작    
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SHitStuff *pHitStuff = pBeData->GetHitStuff();
    RwV3d vDir = pBeData->GetCtrlStuff()->uExtra.sPushing.vDestPos - m_pActor->GetPosition();
    RwV3dNormalize(&vDir, &vDir);
    pHitStuff->vPushDir = vDir;

    pBehavior = NTL_NEW CNtlBehaviorCharPush();
    AddBehavior(pBehavior);
}

void CNtlFSMCharPushing::Exit() 
{
    CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharPushing::Update( RwReal fElapsed ) 
{
    CNtlFSMCharActStateBase::Update(fElapsed);
}

RwUInt32 CNtlFSMCharPushing::HandleEvents( RWS::CMsg &pMsg ) 
{
    return CNtlFSMCharActStateBase::HandleEvents(pMsg);
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlFSMCharRideOnOff)
CNtlFSMCharRideOnOff::CNtlFSMCharRideOnOff() 
{
	SetStateId( NTL_FSMSID_RIDEONOFF );
	SetStateName( NTL_FSMSN_RIDEONOFF );        
	SetFlags(NTL_FSMSF_NOT_ALL);
}

CNtlFSMCharRideOnOff::~CNtlFSMCharRideOnOff() 
{
}

void CNtlFSMCharRideOnOff::Enter() 
{
	CNtlFSMCharActStateBase::Enter();

	RwUInt32 uiClassID = m_pActor->GetClassID();
	if ( uiClassID != SLCLASS_AVATAR &&
		 uiClassID != SLCLASS_PLAYER )
	{
		Finish();
		return;
	}

	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( m_pActor->GetBehaviorData() );
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( m_pActor );
	sASPECTSTATE* pAspectState = pSobPlayer->GetAspectState();

	switch ( pAspectState->sAspectStateBase.byAspectStateId )
	{
		case ASPECTSTATE_VEHICLE:
		{
			// Vehicle 객체를 생성한다

			sCHARSTATE* pState = pSobPlayer->GetServerFullState();

			RwV3d vSpawnPos, vSpawnDir;

			if ( pCtrlStuff->sVehicle.bCreateUpdate )
			{
				vSpawnPos = m_pActor->GetPosition();
				vSpawnDir = m_pActor->GetDirection();
			}
			else
			{
				vSpawnPos = pCtrlStuff->sVehicle.vSpawnPos;
				vSpawnDir = vSpawnPos - m_pActor->GetPosition(); vSpawnDir.y = 0.f;
			}

			if ( RwV3dNormalize( &vSpawnDir, &vSpawnDir ) < 0.5f )
			{
				vSpawnDir = m_pActor->GetDirection();
			}

			TBLIDX tblVehicleTblIdx = pState->sCharStateBase.aspectState.sAspectStateDetail.sVehicle.idVehicleTblidx;

			RwBool bOwnerAvatar = ( m_pActor->GetClassID() == SLCLASS_AVATAR ) ? TRUE : FALSE;

			RwBool bOldScheduleLoad = CNtlPLResourceManager::GetInstance()->IsLoadScheduling();
			CNtlPLResourceManager::GetInstance()->SetLoadScheduling( FALSE );

			CNtlSobVehicle* pSobVehicle = (CNtlSobVehicle*)CNtlSLEventGenerator::SobVehicleCreate( SLCLASS_VEHICLE,
																								   pState,
																								   vSpawnPos,
																								   vSpawnDir,
																								   tblVehicleTblIdx,
																								   Logic_IsChild( pSobPlayer ),
																								   bOwnerAvatar );
			CNtlPLResourceManager::GetInstance()->SetLoadScheduling( bOldScheduleLoad );

			if ( NULL == pSobVehicle )
			{
				NTL_ASSERT( pSobVehicle, "CNtlFSMCharRideOnOff::Enter => Creating the SobVehicle failed" << "(" << tblVehicleTblIdx << ")");

				SetNextStateId( NTL_FSMSID_IDLE );
				Finish();
				return;
			}

			// PC 객체와 Vehicle 객체를 연결한다

			CNtlBeCharData* pVehicleBeData = reinterpret_cast< CNtlBeCharData* >( pSobVehicle->GetBehaviorData() );
			SCtrlStuff* pVehicleCtrlStuff = pVehicleBeData->GetCtrlStuff();

			*pVehicleCtrlStuff = *pCtrlStuff;

			pSobPlayer->SetVehicleID( pSobVehicle->GetSerialID() );
			pSobVehicle->SetOwnerID( pSobPlayer->GetSerialID() );

			if ( pCtrlStuff->sVehicle.bCreateUpdate )
			{
				pSobVehicle->AttachDriver( TRUE );
			}
			else
			{
				pSobVehicle->AttachDriver( FALSE );
				pSobVehicle->EnableVisible( FALSE );
			}

			// Vehicle의 상태를 변경한다

			CNtlSLEventGenerator::SobStateTransition( pSobVehicle->GetSerialID(), NTL_FSMSID_RIDEONOFF );
		}
		break;

		case ASPECTSTATE_INVALID:
		{
			// Vehicle를 다음 틱에 제거한다

			SERIAL_HANDLE hVehicleID = m_pActor->GetVehicleID();
			if ( INVALID_SERIAL_ID == hVehicleID )
			{
				Finish();
				return;
			}

			m_pActor->SetVehicleID( INVALID_SERIAL_ID );

			RwV3d vDir = m_pActor->GetDirection();
			vDir.y = 0.f;
			RwV3dNormalize( &vDir, &vDir );

			// PC의 방향을 보정한다
			((CNtlPLCharacter*)m_pActor->GetSobProxy()->GetPLMainEntity())->SetDirection(&vDir);

			CNtlSobVehicle* pVehicle = (CNtlSobVehicle*)GetNtlSobManager()->GetSobObject( hVehicleID );
			pVehicle->AttachDriver( FALSE );

			CNtlSLEventGenerator::SobDeletePostMsg( hVehicleID );

			GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_NPC_SHOP, m_pActor->GetClassID(), TRUE);
		}
		break;
	}

	Finish();
}

void CNtlFSMCharRideOnOff::Exit() 
{
}

void CNtlFSMCharRideOnOff::Update( RwReal fElapsed ) 
{
}

RwUInt32 CNtlFSMCharRideOnOff::HandleEvents( RWS::CMsg &pMsg ) 
{
	return CNtlFSMCharActStateBase::HandleEvents( pMsg );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMCharActAirAccelState)

CNtlFSMCharActAirAccelState::CNtlFSMCharActAirAccelState()
{
	SetStateId(NTL_FSMSID_AIR_ACCEL);
	SetStateName(NTL_FSMSN_AIR_ACCEL);
	SetFlags(NTL_FSMSF_NOT_ATTACK_STATE | NTL_FSMSF_NOT_HURT_STATE |
		NTL_FSMSF_NOT_JUMP | NTL_FSMSF_NOT_SKILLUSE | NTL_FSMSF_NOT_SOCIALUSE | NTL_FSMSF_NOT_SIT |
		NTL_FSMSF_NOT_ATTACK_CAN | NTL_FSMSF_SKIP_ATTACK_CLICK);

	m_hFrameSkipId = INVAID_HFRAMESKIPID;
	m_fUpdateElapsed = 0.0f;
}

CNtlFSMCharActAirAccelState::~CNtlFSMCharActAirAccelState()
{
	if (m_hFrameSkipId != INVAID_HFRAMESKIPID)
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		pFrameSkipScheduler->RemoveFrameSkip(m_hFrameSkipId);
		m_hFrameSkipId = INVAID_HFRAMESKIPID;
	}
}

void CNtlFSMCharActAirAccelState::Enter(void)
{
	CNtlFSMCharActStateBase::Enter();

	// avatar or summon pet
	if (!(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER))
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		m_hFrameSkipId = pFrameSkipScheduler->AddScheduler();
	}

	CNtlBehaviorCharAirMove *pBehavior = NTL_NEW CNtlBehaviorCharAirMove;
	AddBehavior(pBehavior);
}

void CNtlFSMCharActAirAccelState::Exit(void)
{
	CNtlFSMCharActStateBase::Exit();
}

void CNtlFSMCharActAirAccelState::Update(RwReal fElapsed)
{
	m_fUpdateElapsed += fElapsed;

	if (m_hFrameSkipId != INVAID_HFRAMESKIPID)
	{
		CNtlFrameSkipScheduler *pFrameSkipScheduler = GetNtlSobManager()->GetMoveFrameSkipScheduler();
		if (pFrameSkipScheduler->IsFrameSkip(m_hFrameSkipId))
			return;
	}

	fElapsed = m_fUpdateElapsed;
	m_fUpdateElapsed = 0.0f;

	CNtlFSMCharActStateBase::Update(fElapsed);

	if (!IsFinish())
	{

	}
	else
	{
		CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
		if (pMoveStuff->byMoveResult & NTL_MOVE_RESULT_FALLING)
		{
			Finish();
			SetNextStateId(NTL_FSMSID_FALLING);
		}
	}
}

RwUInt32 CNtlFSMCharActAirAccelState::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActAirAccelState::HandleEvents");

	if (pMsg.Id == g_EventActionMapItemUse)
	{
		NTL_RETURN(NTL_FSM_EVENTRES_QUEUING);
	}


	NTL_RETURN(CNtlFSMCharActStateBase::HandleEvents(pMsg));
}