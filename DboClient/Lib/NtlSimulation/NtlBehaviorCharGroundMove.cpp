#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharGroundMove.h"

// shared 
#include "NtlMovement.h"

// inventory
#include "NtlInventory.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"

// framework
#include "NtlCamera.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLPropertyContainer.h"

// simulation
#include "NtlSobActor.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlFSMDef.h"
#include "NtlSobManager.h"
#include "NtlCameraManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSlApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlCameraController.h"

#define FRAME_2 (2.0f / 30.0f)


DEFINITION_MEMORY_POOL(CNtlBehaviorCharGroundMove)

CNtlBehaviorCharGroundMove::CNtlBehaviorCharGroundMove()
{
    SetBehaviorId(NTL_BEID_CHAR_GROUNDMOVE);
    SetBehaviorName(NTL_BENAME_CHAR_GROUNDMOVE);

    m_bSlowMove				= FALSE;
    m_fSlowMoveTime			= 0.0f;
    m_byDashState			= DASH_STATE_MOVE;
    m_pDashEffect			= NULL;
    m_pDashLine				= NULL;
    m_pDashEffectSmoke		= NULL;
    m_fEffectSmokeTime		= 0.0f;
    m_bDashCameraEnable		= FALSE;
    m_fPosChangeTime		= 0.0f;
    m_byPosChangeCnt		= 0;
    m_byCollMoveImpossCnt	= 0;

    m_bServerSyncPos		= FALSE;
    m_bSyncSendCheck		= TRUE;
    m_fSyncSendTime         = MOVE_SYNC_SEND_TIME;

    m_fInterAnimTime        = 0.0f;    
}

CNtlBehaviorCharGroundMove::~CNtlBehaviorCharGroundMove()
{
    DestroyEndDashCamera();
    DestroyDashEffect();
    DestroyDashEffectSmoke();
}


void CNtlBehaviorCharGroundMove::Enter(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());     
    memcpy(&m_MoveStuff, pBeData->GetMoveStuff(), sizeof(SMoveStuff));

    if(IsInMoveRound())
    {
        Finish();
        return;
    }

    SetPathNode();
    SetTransform();	
    CreateStartDashCamera();

    m_fDefaultWalkSpeed = Logic_GetDefaultFrontWalkSpeed(m_pActor);
    m_fDefaultRunSpeed	= Logic_GetDefaultFrontRunSpeed(m_pActor);
    m_fDefaultBackSpeed = Logic_GetDefaultBackSpeed(m_pActor);	

    SetServerSyncPosition(m_MoveStuff.vCurrLoc);

	// disabled by daneos
	//if(!IsMoveSyncState())
    //{
        SetAnim(m_MoveStuff.byMoveFlags);

        // Idle -> Run 보간 애니메이션
        SetIdle2RunAnim();

        //m_fSyncSendTime = MOVE_SYNC_SEND_TIME;
  //  }
    //else
 //   {
        //m_fSyncSendTime = MOVE_SYNC_SEND_TIME;
 //   }


    // Later, call base class enter.
    CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharGroundMove::SetIdle2RunAnim( void ) 
{
    if(m_pActor->GetSobProxy()->GetDisableAniChange() || Logic_IsTransformSpinAttack(m_pActor) || Logic_IsTransformRollingAttack(m_pActor))
        return;

    // 변신시에는 보간 애니메이션을 안한다.
    if(Logic_GetPlayerRace(m_pActor) == RACE_NAMEK && Logic_IsTransform(m_pActor))
        return;

    // Idle 애니메이션 중일때만 전환한다.
    if(!IsIdleAnimationPlaying())
        return;

    // Idle -> Run Interpolation animation
    if(m_MoveStuff.byMoveFlags == NTL_MOVE_F || m_MoveStuff.byType == NTL_MOVETARGET_LOC || m_MoveStuff.byType == NTL_MOVETARGET_ACTOR)
    {
        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());            
            sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
            if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))    // 스태프를 들고 있을때
            {
                pSobProxy->SetBaseAnimation(NML_STAFF_IDLE_RUN_FRONT, FALSE);
            }
            else    // 아무것도 들고 있지 않을때
            {
                pSobProxy->SetBaseAnimation(IDLE_RUN_FRONT, FALSE);   
            }
            m_bInterAnimEnd = FALSE;
        }
    }
}

void CNtlBehaviorCharGroundMove::Exit(void)
{
    DestroyEndDashCamera();
    DestroyDashEffect();
    DestroyDashEffectSmoke();

    if(m_pActor->GetClassID() == SLCLASS_PLAYER)
    {
        CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
        if(pBeData)
        {
            SMoveSyncStuff* pMoveSyncStuff = pBeData->GetMoveSyncStuff();
            pMoveSyncStuff->Clear();
        }        
    }
    

    // 나중에 base class enter를 호출한다.
    CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorCharGroundMove::Update(RwReal fElapsed)
{
    fElapsed = min(fElapsed, 0.2f);

    if(IsFinish() && !m_pActor->GetSobProxy()->GetDisableAniChange()) // 애니메이션 변경안되는 상태에서는 끝내지 않는다. (회전공격을 위해)
        return;

    // 보간 애니메이션 처리 (2Frame 후부터 이동한다)
    if(!m_bInterAnimEnd)
    {
        if(m_fInterAnimTime < FRAME_2)
        {
            m_fInterAnimTime += fElapsed;
            return;
        }        
        else
        {
            m_bInterAnimEnd = TRUE;
            m_fInterAnimTime = 0.0f;            
        }
    }

    UpdateServerSyncPosition(fElapsed);

    RwBool bFinish = FALSE;
    switch(m_MoveStuff.byType)
    {
    case NTL_MOVETARGET_DIR:                
        bFinish = UpdateDirectionMove(fElapsed);        
        break;
    case NTL_MOVETARGET_LOC:
        bFinish = UpdateLocationMove(fElapsed);
        break;
    case NTL_MOVETARGET_ACTOR:
        if(!Logic_IsTransformSpinAttack(m_pActor) && !Logic_IsTransformRollingAttack(m_pActor))  // When spin attack, do not move target.
        {
            bFinish = UpdateTargetMove(fElapsed);        
        }        
        break;
    case NTL_MOVETARGET_DASH:
        UpdateDashMove(fElapsed);
        break;
    case NTL_MOVETARGET_DASH_ACTOR:
        bFinish = UpdateDashTargetMove(fElapsed);
        break;
    default:
        bFinish = TRUE;
    }	

    // Send a Move Sync.
	// disabled by daneos
    //UpdateSendSyncCheck(fElapsed);

    UpdateAnimSpeed();

    if(bFinish && (!Logic_IsTransformSpinAttack(m_pActor) && !Logic_IsTransformRollingAttack(m_pActor)))
    {
        Finish();        
    }
}

RwUInt32 CNtlBehaviorCharGroundMove::HandleEvents(RWS::CMsg &pMsg)
{
    NTL_FUNCTION("CNtlBehaviorCharGroundMove::HandleEvents");

    if(pMsg.Id == g_EventAnimFootStep)
    {
        if(!m_sHStuff.bWaterAttr || m_sHStuff.fWorldHeight > m_sHStuff.fWaterHeight)
        {
            FootStepMaterialProc(pMsg);
        }
        else
        {
            RwV3d vPos = m_pActor->GetPosition();
            CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
            vPos.y = m_sHStuff.fWaterHeight;
            pSobProxy->CreatePLEffect(NTL_VID_WATER_FOOT, &vPos);
        }
    }

    else if(pMsg.Id == g_EventSobEquipChange)
    {
        RwUInt8 byMoveType = m_MoveStuff.byType;
        if(byMoveType == NTL_MOVETARGET_DIR || byMoveType == NTL_MOVETARGET_LOC ||
            byMoveType == NTL_MOVETARGET_ACTOR)
        {
            SetAnim(m_MoveStuff.byMoveFlags);
        }
    }
    else if(pMsg.Id == g_EventAnimEnd)
    {
        SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );
        RwUInt32 uiAnimKey = pAnimEnd->uiBaseAnimKey;

        if(uiAnimKey == IDLE_RUN_FRONT || uiAnimKey == NML_STAFF_IDLE_RUN_FRONT) // Completion processing of interpolation animation
        {
            SetAnim(m_MoveStuff.byMoveFlags);                
            m_bInterAnimEnd = TRUE; 
        }        

        if(m_byDashState == DASH_STATE_LANDING || m_byDashState == DASH_STATE_END)
        {
			if( uiAnimKey == NML_DASH_FRONT || uiAnimKey == NML_DASH_BACK || uiAnimKey == NML_DASH_LEFT || uiAnimKey == NML_DASH_RIGHT)
            {
                Finish();
            }
        }
    }
    else if(pMsg.Id == g_EventSobAdjustMovePosition)
    {
        SNtlEventSobAdjustMovePosition *pAdjustMovePos = reinterpret_cast<SNtlEventSobAdjustMovePosition*>( pMsg.pData );
        SetServerSyncPosition(pAdjustMovePos->vAdjustPos);
    }    
    else if(pMsg.Id == g_EventTransform)
    {
        SNtlEventTransform* pEvent = (SNtlEventTransform*)pMsg.pData;
        if(pEvent->sApsectState.sAspectStateBase.byAspectStateId == ASPECTSTATE_INVALID)     // 변신 해제
        {
            SetAnim(m_MoveStuff.byMoveFlags);
        }
    }

    NTL_RETURN(CNtlBehaviorCharMove::HandleEvents(pMsg));
}

void CNtlBehaviorCharGroundMove::UpdateData(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
    memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));

    SetAnim(m_MoveStuff.byMoveFlags);

    m_fSyncSendTime = MOVE_SYNC_SEND_TIME;    
    UpdateSendSyncCheck(m_fSyncSendTime);
    m_fSyncSendTime = MOVE_SYNC_SEND_TIME;  // After calling the above function, set it again because it is 0.0 again.
}

void CNtlBehaviorCharGroundMove::FootStepMaterialProc(RWS::CMsg &pMsg)
{
    RwV3d			vPos		= m_pActor->GetPosition();

    if(m_pActor->GetClassID() == SLCLASS_AVATAR || m_pActor->GetClassID() == SLCLASS_MONSTER)
    {
        SNtlEventFootStep*			pData		= reinterpret_cast<SNtlEventFootStep*>(pMsg.pData);
        SEventFootStep*             pEvent      = reinterpret_cast<SEventFootStep*>(pData->pData);
        CNtlPLEntity*				pEntity;
        RwChar						cSoundResoureName[64];
        RwChar						cEffectResoureName[64];

        BYTE						byMaterial	= 0;
        RwReal						fHeight		= 0.0f;

        // Cz : CNtlPLGlobal::m_pWHEntity 보장성을 때문에 GetWorldHeight를 호출합니다.
        //		Performance에 문제가 될 요지가 보이지 않으므로 사용하기로 하였습니다.
        //		문제의 요지가 있을 경우 진성이를 호출해 주세요.
        if (!(GetSceneManager()->GetWorldHeight(&vPos, fHeight, NULL) && CNtlPLGlobal::m_pWHEntity))
        {
            byMaterial = GetSceneManager()->GetWorldMaterialAttribute(vPos);
        }

        if(pEvent->eFootStepMobType == FOOT_TYPE_LARGE || (m_pActor->GetFlags() & SLFLAG_LARGE_FOOT_SOUND)) // 대형 몹 발자국 소리
        {
            sprintf_s(cSoundResoureName, 64, "%s_L_%u.wav", NAMING_SOUND_FOOTSTEP, NtlRandomNumber(0, 3));

            sNtlSoundPlayParameta tSoundParam;
            tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
            tSoundParam.pcFileName		= cSoundResoureName;
            tSoundParam.bLoop			= FALSE;
            tSoundParam.fVolume			= NtlRandomNumber(0.7f, 1.0f);
            tSoundParam.fPitch			= NtlRandomNumber(0.97f, 1.03f);
            tSoundParam.fXPos			= vPos.x;
            tSoundParam.fYPos			= vPos.y;
            tSoundParam.fZPos			= vPos.z;
            tSoundParam.fMinDistance	= 7.0f;
            tSoundParam.fMaxDistance	= 25.0f;

			GetSoundManager()->Play(&tSoundParam);
        }
        else    // 일반 몹 발자국 소리
        {
            sprintf_s(cSoundResoureName, 64, "%s_%u_%u.wav", NAMING_SOUND_FOOTSTEP, byMaterial, NtlRandomNumber(0, 3));

            sNtlSoundPlayParameta tSoundParam;
            tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
            tSoundParam.pcFileName		= cSoundResoureName;
            tSoundParam.bLoop			= FALSE;
            tSoundParam.fVolume			= NtlRandomNumber(0.7f, 1.0f);
            tSoundParam.fPitch			= NtlRandomNumber(0.97f, 1.03f);
            tSoundParam.fXPos			= vPos.x;
            tSoundParam.fYPos			= vPos.y;
            tSoundParam.fZPos			= vPos.z;
            tSoundParam.fMinDistance	= 2.0f;
            tSoundParam.fMaxDistance	= 10.0f;

			GetSoundManager()->Play(&tSoundParam);
        }

        sprintf_s(cEffectResoureName, 64, "%s_%u_%c_%u", NAMING_EFFECT_FOOTSTEP, byMaterial, (pEvent->eFootStepType ? 'R' : 'L'), NtlRandomNumber(0, 1)); 	 	
        if (CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_EFFECT, cEffectResoureName))
        {
            pEntity	= m_pActor->GetSobProxy()->CreatePLEffect(cEffectResoureName, &vPos);
            if (pEntity)
            {
                pEntity->SetRotate(0.0f, CNtlMath::LineToAngleY(&m_pActor->GetDirection()), 0.0f);
            }
        }		
    }
}

RwBool CNtlBehaviorCharGroundMove::IsInDestMoveRound(void)
{
    RwV3d vPos = m_pActor->GetPosition(); 
    RwV3d vDir;
    RwV3dSubMacro(&vDir, &m_MoveStuff.vDest, &vPos);
    vDir.y = 0.0f;
    RwReal fLen = RwV3dLength(&vDir);
    if(fLen < 0.05f)
        return TRUE;

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::IsInFollowMoveRound(void)
{
    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
    if(pTargetActor == NULL)
        return TRUE;

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDestPos = pTargetActor->GetPosition(); 
    // An object that floats on top of the terrain and drifts.
    if(pTargetActor->GetFlags() & SLFLAG_CAN_GROUND_FLY)
        vDestPos.y -= Logic_GetGroundFlyHeight(pTargetActor);

    RwV3d vDir;
    RwV3dSubMacro(&vDir, &vDestPos, &vPos); 

    // Obtain the current distance.
    RwReal fCurrLen = RwV3dLength(&vDir);

    RwReal fInRange = m_MoveStuff.fFollowRange/**FOLLOW_ERROR_RANGE*/;// disable by daneos

    if(fCurrLen <= fInRange)
        return TRUE;

    // 2d Check the distance.
    vDir.y = 0.0f;
    fCurrLen = RwV3dLength(&vDir);
    if(fCurrLen <= fInRange / 2.0f)
        return TRUE;

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::IsInMoveRound(void)
{
    if(m_MoveStuff.byType == NTL_MOVETARGET_LOC)
        return IsInDestMoveRound();
    else if(m_MoveStuff.byType == NTL_MOVETARGET_ACTOR || m_MoveStuff.byType == NTL_MOVETARGET_DASH_ACTOR)
        return IsInFollowMoveRound();

    return FALSE;
}

void CNtlBehaviorCharGroundMove::LimitPositionChangeCheck(RwReal fElapsed)
{
    m_fPosChangeTime += fElapsed;
    m_byPosChangeCnt++;
    if(m_fPosChangeTime >= MOVE_POSITION_CHANGE_CHECK_TIME)
    {
        if(m_byPosChangeCnt >= 3)
        {
            RwV3d vPos = m_pActor->GetPosition();
            RwV3d vSub;
            RwV3dSubMacro(&vSub, &vPos, &m_vChangeOldPos);
            if(RwV3dLength(&vSub) <= MOVE_POSITION_CHANGE_LIMIT_RANGE)
                Finish();
            else
                RwV3dAssignMacro(&m_vChangeOldPos, &vPos);
        }

        m_fPosChangeTime -= MOVE_POSITION_CHANGE_CHECK_TIME;
        m_byPosChangeCnt = 0;
    }
}

void CNtlBehaviorCharGroundMove::SetPathNode(void)
{
    if(m_MoveStuff.byPathNodeNum)
    {
        for(RwInt32 i = 0; i < m_MoveStuff.byPathNodeNum; i++)
            AddPathNode(m_MoveStuff.arrPathNode[i]);

        RwV3dAssignMacro(&m_MoveStuff.vDest, &m_MoveStuff.arrPathNode[0]);
    }
}

void CNtlBehaviorCharGroundMove::SetTransform(void)
{
    RwV3d vPos = m_pActor->GetPosition(); 
    RwV3dAssignMacro(&m_vChangeOldPos, &vPos);

    if(m_MoveStuff.byType == NTL_MOVETARGET_LOC)
    {
        RwV3d vDir;
        RwV3dSubMacro(&vDir, &m_MoveStuff.vDest, &vPos);
        vDir.y = 0.0f;
        RwV3dNormalize(&vDir, &vDir); 

        m_pActor->SetDirection(&vDir); 
    }

    // it is determined whether it is a slow move.
    else if(m_MoveStuff.byType == NTL_MOVETARGET_ACTOR)
    {
        CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
        if(pTargetActor)
        {	
            RwV3d vDestPos = pTargetActor->GetPosition(); 

            // 지형 위를 낮게 떠서 날라다니는 객체일 경우.
            if(pTargetActor->GetFlags() & SLFLAG_CAN_GROUND_FLY)
                vDestPos.y -= Logic_GetGroundFlyHeight(pTargetActor);

            NTL_ASSERT(m_pActor != pTargetActor, "CNtlBehaviorCharGroundMove::SetTransform");

            RwV3d vDir;
            RwV3dSubMacro(&vDir, &vDestPos, &vPos); 

            // Obtain the current distance.
            vDir.y = 0.0f;
            RwReal fCurrLen = RwV3dLength(&vDir);
            if(fCurrLen - m_MoveStuff.fFollowRange < SLOW_MOVE_RANGE)
            {
             //   m_bSlowMove = TRUE; // by daneos
            }

            RwV3dNormalize(&vDir, &vDir); 
            m_pActor->SetDirection(&vDir); 
        }
    }

    else if(m_MoveStuff.byType == NTL_MOVETARGET_DASH)
    {
        m_fDashLadingDist = NTL_DASH_SPEED*DASH_LANDING_TIME;

        Logic_GetWorldHeight(&m_MoveStuff.vDest, m_sHStuff);
        m_MoveStuff.vDest.y = m_sHStuff.fFinialHeight;
        m_fDashDestPosY	= m_MoveStuff.vDest.y;
    }
}

void CNtlBehaviorCharGroundMove::SetAnim(RwUInt8 byMoveFlags)
{
    if(m_pActor->GetSobProxy()->GetDisableAniChange())
        return;

    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
    RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
    RwUInt32 uiNextAnimKey = 0;

    if(m_MoveStuff.byType == NTL_MOVETARGET_DIR)
    {
		sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

        if( byMoveFlags == NTL_MOVE_F || byMoveFlags == NTL_MOVE_F_TURN_L || byMoveFlags == NTL_MOVE_F_TURN_R )
        {
            if(Logic_IsTransformGreatNamek(m_pActor))  // Transformation
            {
                uiNextAnimKey = TRANS_RUN_FRONT;
            }
            else if(Logic_IsTransformSpinAttack(m_pActor))
            {
                uiNextAnimKey = TRANS_IDLE;
            }
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
            else if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
            {
                uiNextAnimKey = NML_STAFF_FRONT_RUN;
            }
            else
            {
                uiNextAnimKey = RUN_FRONT;
            }
        }
        else if( byMoveFlags == NTL_MOVE_B || byMoveFlags == NTL_MOVE_B_TURN_L || byMoveFlags == NTL_MOVE_B_TURN_R )
        {
            if(Logic_IsTransformGreatNamek(m_pActor)) // Transformation
            {
                uiNextAnimKey = TRANS_RUN_BACK;
            }
            else if(Logic_IsTransformSpinAttack(m_pActor))
            {
                uiNextAnimKey = TRANS_IDLE;
            }
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else if (pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			{
				uiNextAnimKey = NML_STAFF_BACK_RUN;
			}
            else
            {
                uiNextAnimKey = RUN_BACK;
            }
        }
		else if (byMoveFlags == NTL_MOVE_L || byMoveFlags == NTL_MOVE_L_TURN_L || byMoveFlags == NTL_MOVE_L_TURN_R)
		{
			sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
			if (Logic_IsTransformSpinAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else
			{
				uiNextAnimKey = NML_MOVE_LEFT;
			}
		}
		else if (byMoveFlags == NTL_MOVE_R || byMoveFlags == NTL_MOVE_R_TURN_L || byMoveFlags == NTL_MOVE_R_TURN_R)
		{
			sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
			if (Logic_IsTransformSpinAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
			else
			{
				uiNextAnimKey = NML_MOVE_RIGHT;
			}
		}
        else if(byMoveFlags == NTL_MOVE_TURN_L)
        {
			if (pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			{
				uiNextAnimKey = NML_STAFF_LEFT_TURN;
			}
			else
			{
				uiNextAnimKey = TURN_LEFT;
			}
        }
        else if(byMoveFlags == NTL_MOVE_TURN_R)
        {
			if (pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
			{
				uiNextAnimKey = NML_STAFF_RIGHT_TURN;
			}
			else
			{
				uiNextAnimKey = TURN_RIGHT;
			}
        }
    }
    else if(m_MoveStuff.byType == NTL_MOVETARGET_LOC || m_MoveStuff.byType == NTL_MOVETARGET_ACTOR)
    {
        RwBool bWalkMove = Logic_IsActorWalkMove(m_pActor, m_MoveStuff.byFormFlag);
        sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);

        if(bWalkMove)
        {
            uiNextAnimKey = WALK_FRONT;
        }
        else
        {
            if(Logic_IsTransformGreatNamek(m_pActor))  // 변신시
            {
                uiNextAnimKey = TRANS_RUN_FRONT;
            }
            else if(Logic_IsTransformSpinAttack(m_pActor))
            {
                uiNextAnimKey = TRANS_IDLE;
            }
			else if (Logic_IsTransformRollingAttack(m_pActor))
			{
				uiNextAnimKey = TRANS_IDLE;
			}
            else if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
            {
                uiNextAnimKey = NML_STAFF_FRONT_RUN;
            }
            else
            {
                uiNextAnimKey = RUN_FRONT;
            }
        }
    }
    else if(m_MoveStuff.byType == NTL_MOVETARGET_DASH)
    {
        if(byMoveFlags == NTL_MOVE_F)
            uiNextAnimKey = NML_DASH_FRONT;
        else if(byMoveFlags == NTL_MOVE_B)
            uiNextAnimKey = NML_DASH_BACK;
        else if(byMoveFlags == NTL_MOVE_L)
            uiNextAnimKey = NML_DASH_LEFT;
        else
            uiNextAnimKey = NML_DASH_RIGHT;

        RwUInt8 bootsGrade = Logic_GetBootsGrade(m_pActor);
        switch (bootsGrade) 
        {
            case (0): 
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_01, NTL_VID_DASH_ACTIVE_02);
                break;
            }
            case (1):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
                break;
            }
            case (2):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
                break;
            }
            case (3):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
                break;
            }
            case (4):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
                break;
            }
            case (5):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREEN_01, NTL_VID_DASH_ACTIVE_GREEN_02);
                break;
            }
            case (6):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREEN_01, NTL_VID_DASH_ACTIVE_GREEN_02);
                break;
            }
            case (7):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREENT_01, NTL_VID_DASH_ACTIVE_GREEN_02);
                break;
            }
            case (8):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREENT_01, NTL_VID_DASH_ACTIVE_GREEN_02);
                break;
            }
            case (9):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
                break;
            }
            case (10):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
                break;
            }
            case (11):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
                break;
            }
            case (12):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
                break;
            }
            case (13):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_RED_01, NTL_VID_DASH_ACTIVE_RED_02);
                break;
            }
            case (14):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_REDT_01, NTL_VID_DASH_ACTIVE_REDT_02);
                break;
            }
            case (15):
            {
                CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_PURPLE_01, NTL_VID_DASH_ACTIVE_PURPLE_02);
                break;
            }
        }

        UpdateDashEffect(byMoveFlags, m_MoveStuff.vDir);
    }
    else if(m_MoveStuff.byType == NTL_MOVETARGET_DASH_ACTOR)
    {
        uiNextAnimKey = NML_DASH_FRONT;

        RwV3d vDir = m_pActor->GetDirection();

        RwUInt8 bootsGrade = Logic_GetBootsGrade(m_pActor);
        switch (bootsGrade)
        {
        case (0):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_01, NTL_VID_DASH_ACTIVE_02);
            break;
        }
        case (1):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
            break;
        }
        case (2):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
            break;
        }
        case (3):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
            break;
        }
        case (4):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_BLUE_01, NTL_VID_DASH_ACTIVE_BLUE_02);
            break;
        }
        case (5):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREEN_01, NTL_VID_DASH_ACTIVE_GREEN_02);
            break;
        }
        case (6):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREEN_01, NTL_VID_DASH_ACTIVE_GREEN_02);
            break;
        }
        case (7):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREENT_01, NTL_VID_DASH_ACTIVE_GREEN_02);
            break;
        }
        case (8):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_GREENT_01, NTL_VID_DASH_ACTIVE_GREEN_02);
            break;
        }
        case (9):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
            break;
        }
        case (10):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
            break;
        }
        case (11):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
            break;
        }
        case (12):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_YELLOW_01, NTL_VID_DASH_ACTIVE_YELLOW_02);
            break;
        }
        case (13):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_RED_01, NTL_VID_DASH_ACTIVE_RED_02);
            break;
        }
        case (14):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_REDT_01, NTL_VID_DASH_ACTIVE_REDT_02);
            break;
        }
        case (15):
        {
            CreateDashEffect(byMoveFlags, NTL_VID_DASH_ACTIVE_PURPLE_01, NTL_VID_DASH_ACTIVE_PURPLE_02);
            break;
        }
        }

        UpdateDashEffect(NTL_MOVE_F, vDir);
    }

    if(uiCurrAnimKey != uiNextAnimKey)
        pSobProxy->SetBaseAnimation(uiNextAnimKey);
}

void CNtlBehaviorCharGroundMove::CreateStartDashCamera(void)
{
    if(m_MoveStuff.byType != NTL_MOVETARGET_DASH)
        return;

    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
        return;

    // camera screen motion blur
    GetNtlGameCameraManager()->GetCamera()->SetEnableBlur(TRUE, 0.9f);

    CNtlSLEventGenerator::CameraDash(m_pActor->GetSerialID(), Logic_GetDashSpeed(m_pActor), m_MoveStuff.vDest);

    m_bDashCameraEnable = TRUE;
}

void CNtlBehaviorCharGroundMove::DestroyEndDashCamera(void)
{
    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
        return;

    if(m_MoveStuff.byType != NTL_MOVETARGET_DASH)
        return;

    if(!m_bDashCameraEnable)
        return;

    // camera screen motion blur
    GetNtlGameCameraManager()->GetCamera()->SetEnableBlur(FALSE);

    CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_DASH); 
    m_bDashCameraEnable = FALSE;
}

void CNtlBehaviorCharGroundMove::SetDashAnimEnd(void)
{
    if(m_MoveStuff.byType == NTL_MOVETARGET_DASH)
    {
		RwUInt32 uiNextAnimKey;

		/*if(m_MoveStuff.byMoveFlags == NTL_MOVE_F)
            uiNextAnimKey = FLYING_MOVE_FRONT;
        else if(m_MoveStuff.byMoveFlags == NTL_MOVE_B)
            uiNextAnimKey = FLYING_MOVE_BACK;
        else if(m_MoveStuff.byMoveFlags == NTL_MOVE_L)
            uiNextAnimKey = NML_MOVE_LEFT;
        else
            uiNextAnimKey = NML_MOVE_RIGHT;*/

		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->SetBaseAnimation(pSobProxy->GetBaseAnimationKey(), FALSE);
    }
}

void CNtlBehaviorCharGroundMove::SetServerSyncPosition(RwV3d vSyncPos)
{
    if( m_pActor->GetFlags() & SLFLAG_SERVER_SENDER )
        return;

    RwV3d vSub;
    RwV3d vPos = m_pActor->GetPosition();
    RwV3dSubMacro(&vSub, &vSyncPos, &vPos);

    vSub.y = 0;

    m_bServerSyncPos		= TRUE;
    m_fCurrServerSyncDist	= 0.0f;
    m_fDeltaServerSyncDist	= 0.0f;
    m_fServerSyncDist		= RwV3dLength(&vSub);

    if(m_fServerSyncDist >= 100.0f)
    {
        //		NTL_ASSERTFAIL("CNtlBehaviorCharGroundMove::SetServerSyncPosition");
        m_fServerSyncDist = 0.0f;
    }

    RwV3dNormalize(&m_vServerSyncDir, &vSub);
}

void CNtlBehaviorCharGroundMove::UpdateSendSyncCheck(RwReal fElapsed)
{
    if(!(m_bSyncSendCheck || IsMoveSyncSend()))
        return;

    if( !(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER) )
        return;

    m_fSyncSendTime += fElapsed;

    if(m_fSyncSendTime >= MOVE_SYNC_SEND_TIME)
    {
        if(m_MoveStuff.byType == NTL_MOVETARGET_DIR)
        {
            RwV3d vPos				= m_pActor->GetPosition();
            RwV3d vDir				= m_pActor->GetDirection();

			//API_GetSLPacketGenerator()->SendChangePosition(vPos, vDir);
        }
        else if(m_MoveStuff.byType == NTL_MOVETARGET_LOC)
        {
			// DISABLED BY DANEOS ! DO NOT SEND MOVEMENT PACKET EVERY X MS TO SERVER (LIKE TW)
			/*
            RwV3d vPos				= m_pActor->GetPosition();
            RwUInt8 byAvatarType	= m_pActor->GetServerSyncAvatarType();
            API_GetSLPacketGenerator()->SendMouseMove(vPos, m_MoveStuff.vDest, byAvatarType);
			*/
        }

        m_fSyncSendTime -= MOVE_SYNC_SEND_TIME;
    }
}



void CNtlBehaviorCharGroundMove::UpdateServerSyncPosition(RwReal fElapsed)
{
    if(!m_bServerSyncPos)
        return;

    if(m_fServerSyncDist == m_fCurrServerSyncDist)
    {
        m_bServerSyncPos = FALSE;
        return;
    }

    m_fDeltaServerSyncDist = m_fServerSyncDist * fElapsed;

    if(m_fCurrServerSyncDist + m_fDeltaServerSyncDist > m_fServerSyncDist)
    {
        m_fDeltaServerSyncDist = m_fServerSyncDist - m_fCurrServerSyncDist;
        m_fCurrServerSyncDist = m_fServerSyncDist;
    }
    else
        m_fCurrServerSyncDist += m_fDeltaServerSyncDist;
}

RwBool CNtlBehaviorCharGroundMove::UpdateDirectionMove(RwReal fElapsed)
{
    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDir = m_pActor->GetDirection();
    RwReal fOldActorHeight = vPos.y;
    RwV3d vNewPos, vNewDir;

    RwReal fSpeed	= Logic_GetFrontRunSpeed(m_pActor);
    m_fAnimSpeed	= 1.0f;    
    RwBool bMoveSync = FALSE;

	RwBool bTurn = FALSE;
	
    if(m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_L ||  m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_R)
    {
        bTurn = TRUE;
    }
    else 
    {
        m_fAnimSpeed	= fSpeed / m_fDefaultRunSpeed;        

		// disabled by daneos
        /*if(m_pActor->GetClassID() == SLCLASS_PLAYER)
        {
            bMoveSync = TRUE;
            if(!UpdateMoveSync(fElapsed, &vNewPos, &vNewDir))
                return FALSE;
        }*/
    }

	if(!bMoveSync)
	{
        CNtlVector vHeading, vDest;        

        //Handles keyboard movement
        NtlGetDestination_Keyboard(vDir.x, vDir.y, vDir.z, fSpeed, vPos.x, vPos.y, vPos.z, m_MoveStuff.byMoveFlags, (DWORD)(fElapsed*1000.f), 1.0f, &vHeading, &vDest);

        CNtlMath::MathRwV3dAssign(&vNewPos, vDest.x, vPos.y, vDest.z); 
        CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, 0.0f, vHeading.z); 

        if(m_bServerSyncPos && !bTurn)
        {
            vNewPos.x += m_vServerSyncDir.x * m_fDeltaServerSyncDist;
            vNewPos.z += m_vServerSyncDir.z * m_fDeltaServerSyncDist;
        }        

        Logic_GetWorldHeight(m_pActor, &vNewPos, m_sHStuff);
        vNewPos.y = m_sHStuff.fFinialHeight;
	}
    
    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
    if(!bTurn)
    {
		if (ObjectCollision(vNewPos, fOldActorHeight, fSpeed, fElapsed, &byColliResult))
		{
			if (byColliResult != NTL_CHARACTER_COLLI_NONE)
			{
				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());

				SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
				pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;// NTL_MOVE_RESULT_FALLING; // BY DANEOS

			//	API_GetSLPacketGenerator()->SendCharMoveCollision(m_pActor);

				// by daneos
				/*SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

				pCtrlStuff->uExtra.sFalling.vFallingDir = m_pActor->GetDirection();

				pCtrlStuff->uExtra.sFalling.byMoveDirection = m_MoveStuff.byMoveFlags;
				pCtrlStuff->uExtra.sFalling.fSpeed = (m_MoveStuff.byMoveFlags == NTL_MOVE_B) ? fSpeed * NTL_BACKWARD_MOVEMENT_SPEED_RATE : fSpeed;
				pCtrlStuff->uExtra.sFalling.fSearchHeight = m_sHStuff.fFinialHeight;*/
			}

			return TRUE;
		}
    }

    m_pActor->SetPosition(&vNewPos);
    m_pActor->SetDirection(&vNewDir);

    // 일정 시간 동안 이동한 거리가 limit 거리 안에 있으면? 멈춘다.
    if( !bTurn && byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        LimitPositionChangeCheck(fElapsed);        
    }

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::UpdateLocationMove(RwReal fElapsed)
{
    //-------------------------------------
    // speed 및 anim speed 결정.
    RwBool bWalkMove = Logic_IsActorWalkMove(m_pActor, m_MoveStuff.byFormFlag);
    RwReal fSpeed; 
    if(bWalkMove)
    {
        fSpeed			= Logic_GetFrontWalkSpeed(m_pActor);
        m_fAnimSpeed	= fSpeed / m_fDefaultWalkSpeed;
    }
    else
    {
        fSpeed			= Logic_GetFrontRunSpeed(m_pActor);
        m_fAnimSpeed	= fSpeed / m_fDefaultRunSpeed;
    }

    //-------------------------------------
    // Obtain the current distance.

    RwV3d vPos = m_pActor->GetPosition();
    RwReal fOldActorHeight = vPos.y;
    RwV3d vDestPos = m_MoveStuff.vDest;    
    RwV3d vDir = vDestPos - vPos;    
    vDir.y = 0.0f;
    RwReal fCurrLen = RwV3dLength(&vDir);
    RwV3dNormalize(&vDir, &vDir);    
    RwV3d vDelta = vDir * fElapsed * fSpeed;

    if(m_bServerSyncPos)
    {
        vDelta.x += m_vServerSyncDir.x * m_fDeltaServerSyncDist;
        vDelta.z += m_vServerSyncDir.z * m_fDeltaServerSyncDist;
    }

    //-------------------------------------
    // The current remaining distance is less than the next distance to travel.
    if(fCurrLen <= RwV3dLength(&vDelta))
    {
        RwUInt8 bColliResult = NTL_CHARACTER_COLLI_NONE;
        if(fElapsed >= 0.5f)
        {
            if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
            {
                RwV3dAssignMacro(&vPos, &vDestPos);
                RwBool bCollMoveImPossible;
                bColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);

                if(bColliResult == NTL_CHARACTER_COLLI_OBJECT)
                {
                    if(bCollMoveImPossible)
                    {
                        ++m_byCollMoveImpossCnt;
                    }
                }
            }
        }

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);

        if(bColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            // If there is a collision but neither movement is possible, it stops.
            if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
            {
                CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
                pBeData->GetMoveStuff()->byMoveResult |= NTL_MOVE_RESULT_COLLISION;                

			//	API_GetSLPacketGenerator()->SendCharMoveCollision(m_pActor);

                return TRUE;
            }

            LimitPositionChangeCheck(fElapsed);

            return FALSE;
        }

        if(PopPathNode(m_MoveStuff.vDest))
            return FALSE;

        return TRUE;
    }
    else // The current remaining distance is not less than the next travel distance.
    {
        vPos += vDelta;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        // object 충돌 처리.
        RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
        if(ObjectCollision(vPos, fOldActorHeight, fSpeed, fElapsed, &byColliResult))
		{
			if (byColliResult != NTL_CHARACTER_COLLI_NONE)
			{
				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());

				SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
				pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;// NTL_MOVE_RESULT_FALLING; // BY DANEOS

			//	API_GetSLPacketGenerator()->SendCharMoveCollision(m_pActor);

				// by daneos
				/*SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

				pCtrlStuff->uExtra.sFalling.vFallingDir = m_pActor->GetDirection();

				pCtrlStuff->uExtra.sFalling.byMoveFlags = m_MoveStuff.byMoveFlags;
				pCtrlStuff->uExtra.sFalling.fSpeed = (m_MoveStuff.byMoveFlags == NTL_MOVE_B) ? fSpeed * NTL_BACKWARD_MOVEMENT_SPEED_RATE : fSpeed;
				pCtrlStuff->uExtra.sFalling.fSearchHeight = m_sHStuff.fFinialHeight;*/
			}

			return TRUE;
		}

        m_pActor->SetDirection(&vDir);
        m_pActor->SetPosition(&vPos);

        if(byColliResult == NTL_CHARACTER_COLLI_NONE)
            return FALSE;

        // If the distance traveled within a certain time is within the limit distance? It stops.
        if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            LimitPositionChangeCheck(fElapsed);
        }

        return FALSE;
    }
}

RwBool CNtlBehaviorCharGroundMove::UpdateTargetMove(RwReal fElapsed)
{
    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
    if(pTargetActor == NULL)
        return TRUE;

    //RwReal fFollowRange = m_MoveStuff.fFollowRange * FOLLOW_ERROR_RANGE; // I do not know why I multiply by 0.9. This causes problems with skill activation distance. (by agebreak 2009.08.5)
    RwReal fFollowRange = m_MoveStuff.fFollowRange;

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDestPos = pTargetActor->GetPosition(); 
    RwReal fOldActorHeight = vPos.y;

    // 지형 위를 낮게 떠서 날라다니는 객체일 경우.
    if(pTargetActor->GetFlags() & SLFLAG_CAN_GROUND_FLY)
        vDestPos.y -= Logic_GetGroundFlyHeight(pTargetActor);

    // If the target is an actor to swim, ignore y
	SWorldHeightStuff hStuff;
	if(Logic_IsSwimmingActor(pTargetActor, &pTargetActor->GetPosition(), hStuff))
	{
		vDestPos.y = vPos.y = 0.0f;
	}

    RwV3d vDir = vDestPos - vPos;	    

    // Obtain the current distance.
    RwReal fCurrLen = RwV3dLength(&vDir);
    vDir.y = 0.0f;
    RwV3dNormalize(&vDir, &vDir);

    m_pActor->SetDirection(&vDir);

    if(fCurrLen < fFollowRange)
    {
        vPos.x = vDestPos.x - vDir.x*fFollowRange;
        vPos.z = vDestPos.z - vDir.z*fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);

        return TRUE;
    }

    RwReal fSpeed = SLOW_MOVE_SPEED;
    if(!m_bSlowMove)
    {
        RwBool bWalkMove = Logic_IsActorWalkMove(m_pActor, m_MoveStuff.byFormFlag);
        if(bWalkMove)
        {
            fSpeed			= Logic_GetFrontWalkSpeed(m_pActor);
            m_fAnimSpeed	= fSpeed / m_fDefaultWalkSpeed;
        }
        else
        {
            fSpeed			= Logic_GetFrontRunSpeed(m_pActor);
            m_fAnimSpeed	= fSpeed / m_fDefaultRunSpeed;
        }
    }

    RwV3d vDelta = vDir * fSpeed * fElapsed;	

    // The current remaining distance is less than the moving delta distance.
    RwReal fDeltaLen = RwV3dLength(&vDelta);
    if(fCurrLen < fDeltaLen + fFollowRange)
    {
        vPos.x = vDestPos.x - vDir.x*fFollowRange;
        vPos.z = vDestPos.z - vDir.z*fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);
        return TRUE;
    }

    // 좌표 update
    vPos += vDelta;    

    Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
    vPos.y = m_sHStuff.fFinialHeight;

    // slow move일 경우.
    if(m_bSlowMove)
    {
        m_fSlowMoveTime += fElapsed;
        if(m_fSlowMoveTime > SLOW_MOVE_TIME)
            return TRUE;
    }

    // 충돌 체크.
    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;

    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        RwBool bCollMoveImPossible;
        byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            if(bCollMoveImPossible)
                m_byCollMoveImpossCnt++;
            else
                m_byCollMoveImpossCnt = 0;

            vPos.y = m_sHStuff.fFinialHeight;
        }
        else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            if(m_byDashState == DASH_STATE_MOVE)
            {
                SetDashAnimEnd();
                m_byDashState = DASH_STATE_END;
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }

        // If there is a collision but neither movement is possible, it stops.
        if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
            SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
            pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

		//	API_GetSLPacketGenerator()->SendCharMoveCollision(m_pActor);

            return TRUE;
        }
    }

    // 폴링 체크
    if(CheckFalling(fOldActorHeight, m_sHStuff.fFinialHeight, fSpeed, NTL_MOVE_F))
        return TRUE;

    m_pActor->SetPosition(&vPos);

    if(byColliResult == NTL_CHARACTER_COLLI_NONE)
        return FALSE;

    // 일정 시간 동안 이동한 거리가 limit 거리 안에 있으면? 멈춘다.
    if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        LimitPositionChangeCheck(fElapsed);
    }

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::UpdateDashMove(RwReal fElapsed)
{
    if(m_byDashState == DASH_STATE_END)
    {
        m_fEffectSmokeTime += fElapsed;
        if(m_fEffectSmokeTime > 0.1f)
        {
            DestroyDashEffect();
            DestroyDashEffectSmoke();
        }

        return TRUE;
    }

    RwV3d vPos = m_pActor->GetPosition();
    CNtlVector vNewPos;

    RwV3d vDestPos = m_MoveStuff.vDest;    

    
    RwV3d vDir = vDestPos - vPos;
    RwV3d v2dSub = vDir;
    v2dSub.y = 0.0f;    

    RwReal f2dCurrLen = RwV3dLength(&v2dSub);
    RwReal fCurrLen = RwV3dLength(&vDir);

    if(m_byDashState == DASH_STATE_MOVE)
    {
        if(fCurrLen <= m_fDashLadingDist)
        {
			m_byDashState = DASH_STATE_LANDING;
            SetDashAnimEnd();
        }
    }

	float fDashSpeed = Logic_GetDashSpeed(m_pActor);

    if(f2dCurrLen <= NTL_EPSILON || fCurrLen <= fDashSpeed * fElapsed)
    {
		// The defense code when a lot of elapsed time comes because of loading.
		// Defending code for height
		// The height is determined by the current coordinates of character + height.

        RwBool bSuccess = FALSE;
        RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
        if(fElapsed >= 0.5f)
        {
            if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
            {
                RwV3dAssignMacro(&vPos, &vDestPos);
                RwBool bCollMoveImPossible;

                byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fDashSpeed, vPos, bCollMoveImPossible, fElapsed);

                if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
                {
                    if(bCollMoveImPossible)
                        m_byCollMoveImpossCnt++;
                }
            }
        }

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;
        m_MoveStuff.vDest.y = m_sHStuff.fFinialHeight;
        vDir.y = 0.0f;

        m_pActor->SetPosition(&vPos);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            // If there is a collision but neither movement is possible, it stops.
            if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
            {
                CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
                SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
                pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

			//	API_GetSLPacketGenerator()->SendCharMoveCollision(m_pActor);

                bSuccess = TRUE;
            }
            else
            {
                LimitPositionChangeCheck(fElapsed);
                bSuccess = FALSE;
            }

            return FALSE;
        }
        else
        {
            bSuccess = TRUE;
        }

        if(bSuccess)
        {
            if(m_byDashState == DASH_STATE_MOVE)
            {
                SetDashAnimEnd();
            }

            DestroyEndDashCamera();

            m_byDashState = DASH_STATE_END;
        }
    }

    RwV3dNormalize(&vDir, &vDir); 
    RwReal fSpeed = fDashSpeed * fElapsed;

    // data를 로딩하여 값자기 elapsed time이 크게 들어올 경우 이런 경우가 생긴다
    if(fSpeed >= fCurrLen)
    {
        fSpeed = fCurrLen;
        vPos += vDir * fSpeed;
        vPos.y = max(m_MoveStuff.vDest.y, vPos.y);        
    }
    else
    {
        vPos += vDir * fSpeed;        
        RwReal fCurrPosY = vPos.y;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = max(m_sHStuff.fFinialHeight, vPos.y);        

        if(fCurrPosY < vPos.y && m_MoveStuff.vDest.y < vPos.y)
        {
            m_MoveStuff.vDest.y = vPos.y + fabs(fCurrPosY - vPos.y);
        }
        else
        {
            m_MoveStuff.vDest.y = m_fDashDestPosY;
        }
    }

    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        RwV2d vOld2DPos;
        vOld2DPos.x = vPos.x;
        vOld2DPos.y = vPos.z;

        RwBool bCollMoveImPossible;
        RwV3d vNewPosV3d;
        byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fDashSpeed, vNewPosV3d, bCollMoveImPossible, fElapsed);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            if(bCollMoveImPossible)
                m_byCollMoveImpossCnt++;
            else
                m_byCollMoveImpossCnt = 0;

            vPos.x = vNewPosV3d.x;
            vPos.z = vNewPosV3d.z;

            RwV2d vNew2DPos;
            vNew2DPos.x = vPos.x;
            vNew2DPos.y = vPos.z;
            RwV2dSubMacro(&vNew2DPos, &vNew2DPos, &vOld2DPos);

            RwReal fCollDeltaLen = RwV2dLength(&vNew2DPos);
            vNewPos.y += fCollDeltaLen*4;

            Logic_GetWorldHeight(m_pActor, &vNewPosV3d, m_sHStuff);
            if(vPos.y <= m_sHStuff.fFinialHeight )
                vPos.y = m_sHStuff.fFinialHeight;

            if(m_MoveStuff.vDest.y < vPos.y)
                m_MoveStuff.vDest.y = vPos.y + 0.01f;
        }
        else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            if(m_byDashState == DASH_STATE_MOVE)
            {
                SetDashAnimEnd();
                m_byDashState = DASH_STATE_END;
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }

        // If there is a collision but neither movement is possible, it stops.
        if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
            SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
            pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

		//	API_GetSLPacketGenerator()->SendCharMoveCollision(m_pActor);

            if(m_byDashState == DASH_STATE_MOVE)
                SetDashAnimEnd();

            m_byDashState = DASH_STATE_END;
            return TRUE;
        }
    }

    m_pActor->SetPosition(&vPos);

    // 일정 시간 동안 이동한 거리가 limit 거리 안에 있으면? 멈춘다.
    if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        LimitPositionChangeCheck(fElapsed);
    }

    // effect position update
    if(m_pDashEffect)
    {
        vPos = vPos + m_vDashEffOffset;
        m_pDashEffect->SetPosition(&vPos);
    }

    UpdateDashLineEffect(vPos, vDir);

    if(m_pDashEffectSmoke)
        m_pDashEffectSmoke->SetPosition(&vPos);

    return FALSE;
}


RwBool CNtlBehaviorCharGroundMove::UpdateDashTargetMove(RwReal fElapsed)
{
    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
    if(pTargetActor == NULL)
        return TRUE;

    RwReal fFollowRange = m_MoveStuff.fFollowRange/**FOLLOW_ERROR_RANGE*/; // disable by daneos

    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDestPos = pTargetActor->GetPosition(); 

    // 지형 위를 낮게 떠서 날라다니는 객체일 경우.
    if(pTargetActor->GetFlags() & SLFLAG_CAN_GROUND_FLY)
        vDestPos.y -= Logic_GetGroundFlyHeight(pTargetActor);

    RwV3d vDir;
    RwV3dSubMacro(&vDir, &vDestPos, &vPos); 

    // 현재 남은 거리를 구한다.
    RwReal fCurrLen = RwV3dLength(&vDir);
    vDir.y = 0.0f;
    RwV3dNormalize(&vDir, &vDir);

    m_pActor->SetDirection(&vDir);

    if(fCurrLen < fFollowRange)
    {
        vPos.x = vDestPos.x - vDir.x*fFollowRange;
        vPos.z = vDestPos.z - vDir.z*fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);

        return TRUE;
    }

	RwReal fSpeed = Logic_GetDashSpeed(m_pActor);
	if(m_MoveStuff.byType == NTL_MOVETARGET_DASH_ACTOR) // if we dash with skill
		fSpeed = NTL_ACTIVE_DASH_SPEED;

    RwV3d vDelta;
    CNtlMath::MathRwV3dAssign(&vDelta, vDir.x * fElapsed * fSpeed, 0.0f, vDir.z*fElapsed*fSpeed);

    // The current remaining distance is less than the moving delta distance.
    RwReal fDeltaLen = RwV3dLength(&vDelta);
    if(fCurrLen < fDeltaLen + fFollowRange)
    {
        vPos.x = vDestPos.x - vDir.x*fFollowRange;
        vPos.z = vDestPos.z - vDir.z*fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);
        return TRUE;
    }

    // position update
    RwV3dAdd(&vPos, &vPos, &vDelta);  

    Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
    vPos.y = m_sHStuff.fFinialHeight;

    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        RwBool bCollMoveImPossible;
        byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            if(bCollMoveImPossible)
                m_byCollMoveImpossCnt++;
            else
                m_byCollMoveImpossCnt = 0;

            Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
            vPos.y = m_sHStuff.fFinialHeight;
        }
        else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            if(m_byDashState == DASH_STATE_MOVE)
            {
                SetDashAnimEnd();
                m_byDashState = DASH_STATE_END;
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }

        // If there is a collision but neither movement is possible, it stops.
        if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
            SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
            pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

		//	API_GetSLPacketGenerator()->SendCharMoveCollision(m_pActor);

            return TRUE;
        }
    }

    // 새로운 좌표 setting
    m_pActor->SetPosition(&vPos);

    // 일정 시간 동안 이동한 거리가 limit 거리 안에 있으면? 멈춘다.
    if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
    {
        LimitPositionChangeCheck(fElapsed);
    }

    // effect update
    // effect position update
    UpdateDashEffect(NTL_MOVE_F, vDir);

    return FALSE;
}


void CNtlBehaviorCharGroundMove::CreateDashEffect(RwUInt32 uiMoveDirFlags, RwChar *pKey1, RwChar *pKey2)
{
    if(!m_pActor)
        return;

    DestroyDashEffect();
    DestroyDashEffectSmoke();

    CNtlPLAttach* pPLChar = (CNtlPLAttach*)m_pActor->GetSobProxy()->GetPLMainEntity();

    m_pDashEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey1);
    pPLChar->Attach((CNtlPLAttach*)m_pDashEffect);

    m_pDashEffectSmoke = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey2);
    pPLChar->Attach((CNtlPLAttach*)m_pDashEffectSmoke);    

    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        m_pDashLine = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_DASH_LINE);
}

void CNtlBehaviorCharGroundMove::DestroyDashEffect(void)
{
    if(m_pDashEffect)
    {
        m_pDashEffect->Finish();
        m_pDashEffect = NULL;
    }

    if(m_pDashLine)
    {
        m_pDashLine->Finish();
        m_pDashLine = NULL;
    }
}

void CNtlBehaviorCharGroundMove::DestroyDashEffectSmoke(void)
{
    if(m_pDashEffectSmoke)
    {
        //		GetSceneManager()->DeleteEntity(m_pDashEffectSmoke);
        m_pDashEffectSmoke->Finish();
        m_pDashEffectSmoke = NULL;
    }
}

void CNtlBehaviorCharGroundMove::UpdateDashEffect(RwUInt32 uiMoveDirFlags, RwV3d& vDir)
{
    RwV3d vPos = m_pActor->GetPosition();

    // 뒤에 따라 붙는 effect
    RwMatrix mat;
    RwMatrixSetIdentity(&mat);

    RwV3dAssignMacro(&mat.at, &vDir);
    RwV3dCrossProduct(&mat.right, &mat.at, &CNtlPLGlobal::m_vYAxisV3);  
    RwV3dNormalize(&mat.right, &mat.right);
    RwV3dCrossProduct(&mat.up, &mat.at, &mat.right);  
    RwV3dNormalize(&mat.up, &mat.up);
    RwV3dAssignMacro(&mat.up, &CNtlPLGlobal::m_vYAxisV3);
    RwMatrixUpdate(&mat);

    // effect 방향 setting.
    if(uiMoveDirFlags == NTL_MOVE_F)
        CNtlMath::MathRwV3dAssign(&m_vDashEffOffset, vDir.x*0.6f, 0.86f, vDir.z*0.6f);
    else if(uiMoveDirFlags == NTL_MOVE_B)
        CNtlMath::MathRwV3dAssign(&m_vDashEffOffset, vDir.x*0.45f, 0.86f, vDir.z*0.45f);
    else if(uiMoveDirFlags == NTL_MOVE_L)
        CNtlMath::MathRwV3dAssign(&m_vDashEffOffset, vDir.x*0.6f, 0.86f, vDir.z*0.6f);
    else
        CNtlMath::MathRwV3dAssign(&m_vDashEffOffset, vDir.x*0.6f, 0.86f, vDir.z*0.6f);

    if(m_pDashEffectSmoke)
        m_pDashEffectSmoke->SetPosition(&vPos);

    if(m_pDashLine)
        m_pDashLine->SetMatrix(mat);

    RwV3dAdd(&vPos, &vPos, &m_vDashEffOffset);
    RwMatrixTranslate(&mat, &vPos, rwCOMBINEPOSTCONCAT);

    if(m_pDashEffect)
        m_pDashEffect->SetMatrix(mat);
}

void CNtlBehaviorCharGroundMove::UpdateDashLineEffect(RwV3d& vPos, RwV3d& vDir)
{
    if(m_pDashLine)
    {
        m_pDashLine->SetPosition(&vPos);

        // 카메라 방향 얻어오기
        RwMatrix *pMatrix = RwFrameGetMatrix( RwCameraGetFrame( CNtlPLGlobal::m_RwCamera ) );
        RwV3d *pCamDir = RwMatrixGetAt( pMatrix );

        RwReal fValue = RwV3dDotProduct(&vDir, pCamDir);
        if(fValue < -0.9f)
            m_pDashLine->SetAlpha(80);
        else
            m_pDashLine->SetAlpha(255);
    }

}

RwBool CNtlBehaviorCharGroundMove::UpdateMoveSync(RwReal fElapsedTime, OUT RwV3d* pDestPos, OUT RwV3d* pNewDir)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveSyncStuff* pMoveSyncStuff = pBeData->GetMoveSyncStuff();

    if(pMoveSyncStuff->m_pMoveSyncCurr == NULL)
    {
        if(pMoveSyncStuff->Next() == NULL)         // I have not received the packet yet.
            return FALSE;
    }
    else
    {
        if(!pMoveSyncStuff->m_MoveSyncQ.empty())        // Receive and process the next packet.
        {
            pMoveSyncStuff->Next();
        }
    }   
    
    RwUInt8 byMoveFlags = pBeData->GetMoveStuff()->byMoveFlags;
    RwV3d vPos = m_pActor->GetPosition();      
    RwV3d vDestPos, vDestDir, vMoveDir;    
    RwV3dNormalize(&vMoveDir, &m_pActor->GetDirection());

    RwReal fSpeed = GetMoveSpeed(pMoveSyncStuff->m_pMoveSyncCurr->byMoveFlag);                

    //keyboard 이동을 처리한다	
    CNtlVector vHeading, vDest;
    NtlGetDestination_Keyboard(vMoveDir.x, vMoveDir.y, vMoveDir.z, fSpeed, vPos.x, vPos.y, vPos.z, byMoveFlags, (DWORD)(fElapsedTime * 1000.f), 1.0f, &vHeading, &vDest);
    CNtlMath::MathRwV3dAssign(&vDestPos, vDest.x, vDest.y, vDest.z);      
    CNtlMath::MathRwV3dAssign(&vDestDir, vHeading.x, 0.0f, vHeading.z);

    // 원래 MoveSync에 맞춰 있어야할 위치를 계산한 후, 그 위치로 가기위한 포스를 결정한다.    
    RwV3d vSyncDir = pMoveSyncStuff->m_pMoveSyncCurr->vLoc - vDestPos;
    vSyncDir.y = 0.0f;
    RwV3dNormalize(&vSyncDir, &vSyncDir);
    RwReal fSyncDistance = CNtlMath::GetLength(pMoveSyncStuff->m_pMoveSyncCurr->vLoc, vDestPos);        
    RwV3d vMoveDest = vSyncDir * (fSyncDistance / MOVE_SYNC_SPEED) * fElapsedTime;    
    vDestPos += vMoveDest;

    // 방향 결정
    *pNewDir = pMoveSyncStuff->m_pMoveSyncCurr->vDir;
    //*pNewDir = vSyncDir;  // If you set the direction to go, you will have problems when you go back, and there will be collisions in other parts.
    
    
    // 최종 위치    
    Logic_GetWorldHeight(m_pActor, &vDestPos, m_sHStuff);
    vDestPos.y = m_sHStuff.fFinialHeight;
    *pDestPos = vDestPos;    

    SetAnim(pMoveSyncStuff->m_pMoveSyncCurr->byMoveFlag);

    return TRUE;
}

RwBool CNtlBehaviorCharGroundMove::CheckFalling(RwReal fOldActorHeight, RwReal fFinalHeight, RwReal fSpeed, RwUInt8 byMoveFlag, RwV3d* pNewPos /* = NULL */)
{
	if( m_pActor->GetFlags() & SLFLAG_FALLING_ENABLE && fOldActorHeight - fFinalHeight > FALLING_CHECK_LEN)
	{
        CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 

        SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
        pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_FALLING;

        SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

        if(pNewPos)
        {
            RwV3d vSub = *pNewPos - m_pActor->GetPosition();        
            vSub.y = 0.0f;
            RwV3dNormalize(&pCtrlStuff->uExtra.sFalling.vFallingDir, &vSub);
        }
        else
        {
            pCtrlStuff->uExtra.sFalling.vFallingDir = m_pActor->GetDirection();
        }

        pCtrlStuff->uExtra.sFalling.byMoveDirection = byMoveFlag;
        pCtrlStuff->uExtra.sFalling.fSpeed = (byMoveFlag == NTL_MOVE_B)? fSpeed * NTL_BACKWARD_MOVEMENT_SPEED_RATE: fSpeed;
        pCtrlStuff->uExtra.sFalling.fSearchHeight = m_sHStuff.fFinialHeight;

        return TRUE;
    }

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::ObjectCollision(OUT RwV3d &vPos, RwReal fOldActorHeight, RwReal fSpeed, RwReal fElapsed, OUT RwUInt8* pColliResult)
{
    RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;

    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        //RwV2d vOld2DPos;
        //vOld2DPos.x = vPos.x;
        //vOld2DPos.y = vPos.z;

        RwBool bCollMoveImPossible;
        byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);
        *pColliResult = byColliResult;

        // If there is a collision due to the slope of the terrain or the property, it stops in place.
        if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            return TRUE;
        }

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            m_byCollMoveImpossCnt = bCollMoveImPossible ? ++m_byCollMoveImpossCnt : 0;

            // I do not know why this code is ?? (by agebreak 09.08.13)

			// - lleo52 09.08.18
			// The reason for entering the following code is to prevent it from falling into an object.
			// Test results after comment processing show no problem
			// First of all, you should comment out the code below and watch the situation.

			//RwV2d vNew2DPos;
			//vNew2DPos.x = vPos.x;
			//vNew2DPos.y = vPos.z;
			//RwReal fOffset = CNtlMath::GetLength(vNew2DPos, vOld2DPos) * 2;
			//vPos.y += fOffset;

            Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
            vPos.y = m_sHStuff.fFinialHeight;
            //////////////////////////////////////////////////////////////////////////
        }
        else if(byColliResult == NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT && Logic_IsDirectMode(m_pActor))
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());                 
            pBeData->GetMoveStuff()->byMoveResult |= NTL_MOVE_RESULT_COLLISION;   

            return TRUE;                
        }            
    }
    
    // If there is a collision but neither movement is possible, it stops.
    if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
    {
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
		pBeData->GetMoveStuff()->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

        return TRUE;
    }

    // falling state check
    //if(CheckFalling(fOldActorHeight, m_sHStuff.fFinialHeight, fSpeed, m_MoveStuff.byMoveFlags, &vPos))
     //   return TRUE;

    return FALSE;
}

RwBool CNtlBehaviorCharGroundMove::IsIdleAnimationPlaying()
{
    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
    RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();

    if(uiCurrAnimKey == TRANS_IDLE || uiCurrAnimKey == NML_IDLE_01 || 
       uiCurrAnimKey == NML_IDLE_LOOP || uiCurrAnimKey == NML_IDLE_02 ||
       uiCurrAnimKey == NML_STAFF_IDLE_LOOP || uiCurrAnimKey == NML_STAFF_IDLE_01 ||
       uiCurrAnimKey == NML_STAFF_IDLE_02 || uiCurrAnimKey == NML_DGUN_IDLE_LOOP || 
       uiCurrAnimKey == NML_DGUN_IDLE_01 || uiCurrAnimKey == NML_DGUN_IDLE_02)
    {
        return TRUE;
    }

    return FALSE;
}