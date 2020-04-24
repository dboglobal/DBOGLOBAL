#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharSwimmingMove.h"

// shared 
#include "NtlMovement.h"

// presentation
#include "NtlPLCharacter.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlFSMDef.h"
#include "NtlSLApi.h"
#include "NtlSobActor.h"
#include "NtlSobCharProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"

DEFINITION_MEMORY_POOL(CNtlBehaviorCharSwimmingMove)

CNtlBehaviorCharSwimmingMove::CNtlBehaviorCharSwimmingMove()
{
    SetBehaviorId(NTL_BEID_CHAR_SWIMMINGMOVE);
    SetBehaviorName(NTL_BENAME_CHAR_SWIMMNGMOVE);

    m_fPosChangeTime = 0.0f;
    m_byPosChangeCnt = 0;
    m_byCollMoveImpossCnt = 0;

    m_pPLSwimFront	= NULL;
    m_pPLSwimBack	= NULL;
}

CNtlBehaviorCharSwimmingMove::~CNtlBehaviorCharSwimmingMove()
{
    DestroySwimFrontEffect();
    DestroySwimBackEffect();
}

void CNtlBehaviorCharSwimmingMove::SetTransform(void)
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
}

void CNtlBehaviorCharSwimmingMove::SetAnim(void)
{
    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

    RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
    RwUInt32 uiNextAnimKey = 0;

    if(m_MoveStuff.byType == NTL_MOVETARGET_DIR)
    {
        if( m_MoveStuff.byMoveFlags == NTL_MOVE_F || m_MoveStuff.byMoveFlags == NTL_MOVE_F_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_F_TURN_R)
        {
            uiNextAnimKey = SWIM_FRONT;
        }
        else if( m_MoveStuff.byMoveFlags == NTL_MOVE_B || m_MoveStuff.byMoveFlags == NTL_MOVE_B_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_B_TURN_R)
        {
            uiNextAnimKey = SWIM_BACK;
        }
		else if (m_MoveStuff.byMoveFlags == NTL_MOVE_L || m_MoveStuff.byMoveFlags == NTL_MOVE_L_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_L_TURN_R)
		{
			uiNextAnimKey = SWIM_FRONT;
		}
		else if (m_MoveStuff.byMoveFlags == NTL_MOVE_R || m_MoveStuff.byMoveFlags == NTL_MOVE_R_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_R_TURN_R)
		{
			uiNextAnimKey = SWIM_FRONT;
		}
        else
        {
            uiNextAnimKey = SWIM_IDLE;
        }
    }
    else if(m_MoveStuff.byType == NTL_MOVETARGET_LOC || m_MoveStuff.byType == NTL_MOVETARGET_ACTOR)
    {
        uiNextAnimKey = SWIM_FRONT;
    }

    if(uiCurrAnimKey != uiNextAnimKey)
    {
        CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
        pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.20f);

        pSobProxy->SetBaseAnimation(uiNextAnimKey);

        CreateSwimFrontEffect();

        // 뒤에 붙는 물장구(?) 이펙트
        if(uiNextAnimKey == SWIM_FRONT)
        {
            CreateSwimBackEffect();
        }
        else
        {
            DestroySwimBackEffect();
        }
    }
}

void CNtlBehaviorCharSwimmingMove::SetPathNode(void)
{
    if(m_MoveStuff.byPathNodeNum)
    {
        for(RwInt32 i = 0; i < m_MoveStuff.byPathNodeNum; i++)
            AddPathNode(m_MoveStuff.arrPathNode[i]);

        RwV3dAssignMacro(&m_MoveStuff.vDest, &m_MoveStuff.arrPathNode[0]);
    }
}

RwBool CNtlBehaviorCharSwimmingMove::IsInMoveRound(void)
{
    if(m_MoveStuff.byType == NTL_MOVETARGET_LOC)
        return IsInDestMoveRound();

    return FALSE;
}

RwBool CNtlBehaviorCharSwimmingMove::IsInDestMoveRound(void)
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

void CNtlBehaviorCharSwimmingMove::CreateSwimFrontEffect(void)
{
    RwV3d vPos = m_pActor->GetPosition();

    CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
    CNtlPLEntity *pPLEntity;

    if(m_MoveStuff.byMoveFlags == NTL_MOVE_B || m_MoveStuff.byMoveFlags == NTL_MOVE_B_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_B_TURN_R)
        pPLEntity = pSobProxy->CreatePLEffect(NTL_VID_SWIM_BACK , &vPos);
    else if(m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_L || m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_R)
        pPLEntity = pSobProxy->CreatePLEffect(NTL_VID_SWIM_IDLE, &vPos);
    else
        pPLEntity = pSobProxy->CreatePLEffect(NTL_VID_SWIM_MOVE, &vPos);

    if(pPLEntity && !pPLEntity->IsAutoDelete())
    {
        if(m_pPLSwimFront)
        {
            m_pPLSwimFront->Finish();
        }
        
        m_pPLSwimFront = pPLEntity;
    }
}

void CNtlBehaviorCharSwimmingMove::CreateSwimBackEffect(void)
{
    if(m_MoveStuff.byMoveFlags == NTL_MOVE_B || m_MoveStuff.byMoveFlags == NTL_MOVE_B_TURN_L ||
        m_MoveStuff.byMoveFlags == NTL_MOVE_B_TURN_R || m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_L ||
        m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_R)
        return;

    RwV3d vPos = m_pActor->GetPosition();

    CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
    CNtlPLEntity *pPLEntity = pSobProxy->CreatePLEffect(NTL_VID_SWIM_MOVE_02, &vPos);
    if(pPLEntity && !pPLEntity->IsAutoDelete())
    {
        if(m_pPLSwimBack)
        {
            m_pPLSwimBack->Finish(); 
        }

        m_pPLSwimBack = pPLEntity;
    }
}

void CNtlBehaviorCharSwimmingMove::DestroySwimFrontEffect(void)
{
    if(m_pPLSwimFront)
    {
        m_pPLSwimFront->Finish();
        m_pPLSwimFront = NULL;
    }
}

void CNtlBehaviorCharSwimmingMove::DestroySwimBackEffect(void)
{
    if(m_pPLSwimBack)
    {
        m_pPLSwimBack->Finish();
        m_pPLSwimBack = NULL;
    }
}


void CNtlBehaviorCharSwimmingMove::LimitPositionChangeCheck(RwReal fElapsed)
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

void CNtlBehaviorCharSwimmingMove::Enter(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
    memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));

    if(IsInMoveRound())
    {
        Finish();
        return;
    }

    SetPathNode();
    SetTransform();
    SetAnim();    

    // 나중에 base class enter를 호출한다.
    CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharSwimmingMove::Exit(void)
{
    DestroySwimFrontEffect();
    DestroySwimBackEffect();

    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

    CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
    pPLCharacter->SetBlend(BLEND_TWEEN);

    // 나중에 base class enter를 호출한다.
    CNtlBehaviorBase::Exit(); 
}

RwBool CNtlBehaviorCharSwimmingMove::UpdateDirectionMove(RwReal fElapsed)
{
    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDir = m_pActor->GetDirection();

    RwReal fSpeed	= Logic_GetFrontRunSpeed(m_pActor);
    fSpeed			= fSpeed * DBO_SWIMMING_SPEED_RATIO;

    //keyboard 이동을 처리한다
    CNtlVector vHeading, vDest;
    NtlGetDestination_Keyboard(vDir.x, vDir.y, vDir.z, fSpeed, vPos.x, vPos.y, vPos.z, m_MoveStuff.byMoveFlags, (DWORD)(fElapsed*1000.f), 0.5f, &vHeading, &vDest);

    RwV3d vNewPos, vNewDir;
    CNtlMath::MathRwV3dAssign(&vNewPos, vDest.x, vPos.y, vDest.z); 
    CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, 0.0f, vHeading.z); 

    Logic_GetWorldHeight(m_pActor, &vNewPos, m_sHStuff);
    vNewPos.y = m_sHStuff.fFinialHeight;

    if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
    {
        if( !(m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_L ||  m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_R) )
        {
            RwBool bCollMoveImPossible;
            RwUInt8 byColliResult = Logic_CharacterCollisionEx(m_pActor, &vNewPos, fSpeed, vNewPos, bCollMoveImPossible, fElapsed);

            if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
            {
                if(bCollMoveImPossible)
                    m_byCollMoveImpossCnt++;
                else
                    m_byCollMoveImpossCnt = 0;

                Logic_GetWorldHeight(m_pActor, &vNewPos, m_sHStuff);
                vNewPos.y = m_sHStuff.fFinialHeight;
            }

            // 충돌 했는데 어느쪽으로도 이동이 불가능할 경우 멈춰준다.
            if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
            {
                CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
                SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
                pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

                return TRUE;
            }
        }
    }

    m_pActor->SetPosition(&vNewPos);
    m_pActor->SetDirection(&vNewDir);

    // 일정 시간 동안 이동한 거리가 limit 거리 안에 있으면? 멈춘다.
    if( !(m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_L ||  m_MoveStuff.byMoveFlags == NTL_MOVE_TURN_R) )
        LimitPositionChangeCheck(fElapsed);

    return FALSE;
}

RwBool CNtlBehaviorCharSwimmingMove::UpdateLocationMove(RwReal fElapsed)
{
    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDestPos;
    RwV3dAssignMacro(&vDestPos, &m_MoveStuff.vDest); 

    RwV3d vDir;
    RwV3dSubMacro(&vDir, &vDestPos, &vPos); 
    vDir.y = 0.0f;

    // 현재 남은 거리를 구한다.
    RwReal fCurrLen = RwV3dLength(&vDir);

    RwReal fSpeed	= Logic_GetFrontRunSpeed(m_pActor);
    fSpeed			= fSpeed * DBO_SWIMMING_SPEED_RATIO;

    RwV3d vDelta;
    RwV3dNormalize(&vDir, &vDir);
    CNtlMath::MathRwV3dAssign(&vDelta, vDir.x*fElapsed*fSpeed, vDir.y*fElapsed*fSpeed, vDir.z*fElapsed*fSpeed);

    // 현재 남아 있는 거리가 다음 이동할 거리보다 작을 경우.
    if(fCurrLen <= RwV3dLength(&vDelta))
    {
        RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
        if(fElapsed >= 0.5f)
        {
            if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
            {
                RwV3dAssignMacro(&vPos, &vDestPos);
                RwBool bCollMoveImPossible;
                byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed);

                if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
                {
                    if(bCollMoveImPossible)
                        m_byCollMoveImpossCnt++;
                }
            }
        }

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);

        if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
        {
            // 충돌 했는데 어느쪽으로도 이동이 불가능할 경우 멈춰준다.
            if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
            {
                CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
                SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
                pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

                return TRUE;
            }

            LimitPositionChangeCheck(fElapsed);

            return FALSE;
        }

        if(PopPathNode(m_MoveStuff.vDest))
            return FALSE;

        return TRUE;
    }
    else // 현재 남아 있는 거리가 다음 이동할 거리보다 작지 않을 경우.
    {
        vPos.x += vDelta.x;
        vPos.z += vDelta.z;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        // object 충돌 처리.
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
        }
        // 충돌 했는데 어느쪽으로도 이동이 불가능할 경우 멈춰준다.
        if(m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
            SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
            pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_COLLISION;

            return TRUE;
        }

        m_pActor->SetDirection(&vDir);
        m_pActor->SetPosition(&vPos);

        // 일정 시간 동안 이동한 거리가 limit 거리 안에 있으면? 멈춘다.
        if(byColliResult != NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT)
        {
            LimitPositionChangeCheck(fElapsed);
        }

        return FALSE;
    }
}

RwBool CNtlBehaviorCharSwimmingMove::UpdateTargetMove( RwReal fElapsed ) 
{
    CNtlSobActor* pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_MoveStuff.hFollowSerialId));
    if(!pTargetActor)
        return TRUE;

    RwV3d vPos      = m_pActor->GetPosition();
    RwV3d vDestPos  = pTargetActor->GetPosition();
    RwV3d vDir      = vDestPos - vPos;
    RwReal fCurrLen = RwV3dLength(&vDir);
    RwReal fFollowRange = m_MoveStuff.fFollowRange * FOLLOW_ERROR_RANGE;
    RwBool bInRange = FALSE;

    // 캐릭터의 방향 설정
    vDir.y = 0.0f;
    RwV3dNormalize(&vDir, &vDir);
    m_pActor->SetDirection(&vDir);

    if(fCurrLen < fFollowRange)     // 사정거리 이내에 있을때
    {
        bInRange = TRUE;
    }
    else // 사정거리이내에 없을때
    {

        RwReal fSpeed	= Logic_GetFrontRunSpeed(m_pActor);
        fSpeed			= fSpeed * DBO_SWIMMING_SPEED_RATIO;
        RwV3d vDelta = vDir * fSpeed * fElapsed;

        // 현재 남은 거리가 이동 delta 거리보다 작은경우
        if(fCurrLen < RwV3dLength(&vDelta) + fFollowRange)
        {
            bInRange = TRUE;
        }
        else // 타겟 방향으로 이동
        {
            vPos += vDelta;
            Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
            vPos.y = m_sHStuff.fFinialHeight;

            // TODO: 충돌체크

            m_pActor->SetPosition(&vPos);
        }
    }

    if(bInRange) // 범위내에 들어오면 범위 위치에 위치시킨다.
    {
        vPos.x = vDestPos.x - vDir.x * fFollowRange;
        vPos.z = vDestPos.z - vDir.z * fFollowRange;

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;

        m_pActor->SetPosition(&vPos);        

        return TRUE;
    }    

    return FALSE;
}

void CNtlBehaviorCharSwimmingMove::UpdateSwimEffect(void)
{
    RwV3d vPos = m_pActor->GetPosition();
    RwV3d vDir = m_pActor->GetDirection();

    vPos.y = m_sHStuff.fWaterHeight + 0.1f;

    RwMatrix mat;
    RwMatrixSetIdentity(&mat);
    CNtlMath::MathRwMatrixAssign(&mat, &vPos, &vDir);


    if(m_pPLSwimFront)
    {
        //		m_pPLSwimFront->SetPosition(&vPos);

        m_pPLSwimFront->SetMatrix(mat);
    }

    if(m_pPLSwimBack)
    {
        m_pPLSwimBack->SetMatrix(mat);
    }
}

void CNtlBehaviorCharSwimmingMove::Update(RwReal fElapsed)
{
    if(IsFinish())
        return;

    RwBool bFinish = FALSE;
    if(m_MoveStuff.byType == NTL_MOVETARGET_DIR)
        bFinish = UpdateDirectionMove(fElapsed);
    else if(m_MoveStuff.byType == NTL_MOVETARGET_LOC)
        bFinish = UpdateLocationMove(fElapsed);
    else if(m_MoveStuff.byType == NTL_MOVETARGET_ACTOR)
        bFinish = UpdateTargetMove(fElapsed);
    else
        Finish();

    UpdateSwimEffect();

    if(bFinish)
        Finish();
}

RwUInt32 CNtlBehaviorCharSwimmingMove::HandleEvents(RWS::CMsg &pMsg)
{
    NTL_FUNCTION("CNtlBehaviorCharGroundMove::HandleEvents");

    if(pMsg.Id == g_EventSobEquipChange)
    {
        RwUInt8 byMoveType = m_MoveStuff.byType;
        if(byMoveType == NTL_MOVETARGET_DIR || byMoveType == NTL_MOVETARGET_LOC)
        {
            SetAnim();
        }
    }
    else if(pMsg.Id == g_EventAnimEnd)
    {
    }

    NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharSwimmingMove::UpdateData(void)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
    memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));

    SetAnim();
}
