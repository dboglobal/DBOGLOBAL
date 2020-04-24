#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharJump.h"

// shared 
#include "NtlMovement.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"

// presentation
#include "NtlPLCharacter.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSlapi.h"
#include "NtlSobActor.h"
#include "NtlSobCharProxy.h"
#include "NtlSLLogic.h"
#include "NtlFSMDef.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLPacketGenerator.h"

#define  JUMP_MAX_HEIGHT    3.0f


DEFINITION_MEMORY_POOL(CNtlBehaviorCharJump)

CNtlBehaviorCharJump::CNtlBehaviorCharJump()
{
    SetBehaviorId(NTL_BEID_CHAR_JUMP);
    SetBehaviorName(NTL_BENAME_CHAR_JUMP);

    m_byCollMoveImpossCnt	= 0;
    m_bRandingVisualEffect	= FALSE;
    m_bStandRandingEnd		= FALSE;
    m_bCreateBubbleEffect   = FALSE;
    m_pWaterBubbleEffect    = NULL;
    m_fSyncSendTime         = MOVE_SYNC_SEND_TIME;
    m_bNoneDirJump          = FALSE;
}

CNtlBehaviorCharJump::~CNtlBehaviorCharJump()
{
    if(m_pWaterBubbleEffect)
    {
        m_pWaterBubbleEffect->Finish();
        m_pWaterBubbleEffect = NULL;
    }
}

void CNtlBehaviorCharJump::Enter(void)
{
    ChangeJumpState(JUMPSTATE_START);

    // 나중에 base class enter를 호출한다.
    CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharJump::Exit(void)
{
    CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
    CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	if(pPLCharacter)
		pPLCharacter->SetBlend(BLEND_TWEEN);

    CNtlBehaviorBase::Exit(); 

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	if (pBeData)
	{
		SJumpStuff* pJumpStuff = pBeData->GetJumpStuff();
		if (pJumpStuff)
		{
			pJumpStuff->eState = JUMPSTATE_NONE;
		}
	}
}

void CNtlBehaviorCharJump::Update(RwReal fElapsed)
{
    // Avoid sudden splashes when the frame is low
    fElapsed = min(fElapsed, 0.2f);

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	SJumpStuff *pJumpStuff = pBeData->GetJumpStuff();

    if(m_byJumpState == JUMPSTATE_START)
        UpdateJumpStart(pMoveStuff, pJumpStuff, fElapsed);
    else if(m_byJumpState == JUMPSTATE_LOOP)
        UpdateJumpLoop(pMoveStuff, pJumpStuff, fElapsed);
    else if(m_byJumpState == JUMPSTATE_STAND_LANDING)
        UpdateJumpStandLanding(pMoveStuff, pJumpStuff, fElapsed);
    else if(m_byJumpState == JUMPSTATE_MOVE_LANDING)
        UpdateJumpMoveLanding(pMoveStuff, pJumpStuff, fElapsed);
    else if(m_byJumpState == JUMPSTATE_WATER_LANDING)
        UpdateJumpWaterLanding(pMoveStuff, pJumpStuff, fElapsed);

    // If it is an avatar, it sends a packet matching the server and sync.
    UpdateSendSyncCheck(fElapsed);
}

RwUInt32 CNtlBehaviorCharJump::HandleEvents(RWS::CMsg &pMsg)
{
    NTL_FUNCTION("CNtlBehaviorCharJump::HandleEvents");

    if(pMsg.Id == g_EventAnimEnd)
    {
        if(m_byJumpState == JUMPSTATE_START)
        {
            ChangeJumpState(JUMPSTATE_LOOP);
        }        
        else if(m_byJumpState == JUMPSTATE_STAND_LANDING || m_byJumpState == JUMPSTATE_MOVE_LANDING || m_byJumpState == JUMPSTATE_END)
        {
            Finish();
        }        
    }

    return CNtlBehaviorCharMove::HandleEvents(pMsg);    
}

void CNtlBehaviorCharJump::UpdatePositionMove(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwV3d& vPos, RwBool bIncHeight, RwReal fElapsed, RwReal fSpeedScale /*= 1.0f*/)
{
    RwReal fOrgY = vPos.y;

    if(bIncHeight)
    {
        RwReal vJumpHeight = m_fJumpUpSpeed * fElapsed;
        vJumpHeight = min(JUMP_MAX_HEIGHT, vJumpHeight);
        vPos.y += vJumpHeight;

        m_fJumpUpSpeed = m_fJumpUpSpeed - (JUMP_GRAVITY_SPEED * fElapsed);
    }

    if(pMoveStuff->byMoveFlags != NTL_MOVE_NONE)
	{
        RwV3d vDir = m_pActor->GetDirection();
        RwReal fSpeed = Logic_GetFrontRunSpeed(m_pActor) * fSpeedScale;        

        CNtlVector vHeading, vDest;
        
        // When another player jumps continuously, the movement flag sometimes walks while the state is shifted. Change this to the jump flag.
        if(m_pActor->GetClassID() == SLCLASS_PLAYER)
        {
            if(pMoveStuff->byMoveFlags == NTL_MOVE_F_TURN_L) pMoveStuff->byMoveFlags = NTL_MOVE_F_TURN_L_JUMP;
            if(pMoveStuff->byMoveFlags == NTL_MOVE_F_TURN_R) pMoveStuff->byMoveFlags = NTL_MOVE_F_TURN_R_JUMP;
            if(pMoveStuff->byMoveFlags == NTL_MOVE_B_TURN_L) pMoveStuff->byMoveFlags = NTL_MOVE_B_TURN_L_JUMP;
            if(pMoveStuff->byMoveFlags == NTL_MOVE_B_TURN_R) pMoveStuff->byMoveFlags = NTL_MOVE_B_TURN_R_JUMP;
			if (pMoveStuff->byMoveFlags == NTL_MOVE_L_TURN_L) pMoveStuff->byMoveFlags = NTL_MOVE_L_TURN_L_JUMP;
			if (pMoveStuff->byMoveFlags == NTL_MOVE_L_TURN_R) pMoveStuff->byMoveFlags = NTL_MOVE_L_TURN_R_JUMP;
			if (pMoveStuff->byMoveFlags == NTL_MOVE_R_TURN_L) pMoveStuff->byMoveFlags = NTL_MOVE_R_TURN_L_JUMP;
			if (pMoveStuff->byMoveFlags == NTL_MOVE_R_TURN_R) pMoveStuff->byMoveFlags = NTL_MOVE_R_TURN_R_JUMP;
        }
        
        NtlGetDestination_Jump(vDir.x, vDir.y, vDir.z, fSpeed, vPos.x, vPos.y, vPos.z, pJumpStuff->vJumpDir.x, pJumpStuff->vJumpDir.z, pMoveStuff->byMoveFlags, (DWORD)(fElapsed*1000.f), 1.0f, &vHeading, &vDest);

        RwV3d vNewDir;
        CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, 0.0f, vHeading.z); 
        m_pActor->SetDirection(&vNewDir);

        // 제자리 점프라면 위치 이동은 막는다.
        if(m_bNoneDirJump)
            return;

        vPos.x = vDest.x;
        vPos.z = vDest.z;

		// disabled by daneos
        //// If you are a player, correct Move Sync.
        //if(m_pActor->GetClassID() == SLCLASS_PLAYER)
        //{
        //    UpdateMoveSync(vPos, fElapsed);        
        //    
        //    // 계산한 결과를 적용한다.
        //    vPos = m_pActor->GetPosition();            
        //    return;
        //}

        // 충돌 체크.
        if(m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
        {
            RwReal fCurrHeight = vPos.y;
            RwBool bCollMoveImPossible;
            RwUInt8 byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed, TRUE);

            vPos.y = fCurrHeight;

            if(byColliResult == NTL_CHARACTER_COLLI_OBJECT)
            {
                if(bCollMoveImPossible)
                    m_byCollMoveImpossCnt++;
                else
                    m_byCollMoveImpossCnt = 0;
            }
            else if(byColliResult == NTL_CHARACTER_COLLI_OBJECT_JUMP && m_fJumpUpSpeed > 0.0f)
            {
                vPos.y = fOrgY;
                m_fJumpUpSpeed = 0.0f;
                m_byCollMoveImpossCnt = 0;
            }
        }
    }
    else // 제자리 점프 
    {
        if((m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION) && m_fJumpUpSpeed > 0.0f)
        {
            RwReal fCurrHeight = vPos.y;
            RwBool bCollMoveImPossible;
            RwUInt8 byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, m_fJumpUpSpeed, vPos, bCollMoveImPossible, fElapsed, TRUE);

            if(byColliResult == NTL_CHARACTER_COLLI_OBJECT_JUMP)
            {
                vPos.y = fOrgY;
                m_fJumpUpSpeed = 0.0f;                
            }
            else
            {
                m_bNoneDirJump = TRUE;      
                vPos.y = fCurrHeight;
            }
        }
    }
}

RwBool CNtlBehaviorCharJump::UpdateMoveSync(RwV3d vPos, RwReal fElapsedTime)
{
    if(m_pActor->GetClassID() != SLCLASS_PLAYER)
        return FALSE;

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    SMoveSyncStuff* pMoveSyncStuff = pBeData->GetMoveSyncStuff();

    if(pMoveSyncStuff->m_pMoveSyncCurr == NULL)
    {
        if(pMoveSyncStuff->Next() == NULL)         // 아직 패킷을 못받았다        
            return FALSE;
    }
    else
    {
        if(!pMoveSyncStuff->m_MoveSyncQ.empty())        // 다음 패킷을 받아서 처리한다.
        {
            pMoveSyncStuff->Next();
        }
    }    
    
    // 원래 MoveSync에 맞춰 있어야할 위치를 계산한 후, 그 위치로 가기위한 포스를 결정한다.        
    RwV3d vSyncDir = pMoveSyncStuff->m_pMoveSyncCurr->vLoc - vPos;    
    vSyncDir.y = 0.0f;
    RwReal vSyncDistance = RwV3dLength(&vSyncDir);
    RwV3dNormalize(&vSyncDir, &vSyncDir);    
    vPos += vSyncDir * (vSyncDistance / MOVE_SYNC_SPEED) * fElapsedTime * 2.0f;    

    // 최종 위치
    m_pActor->SetPosition(&vPos);
    //m_pActor->SetPosition(&pMoveSyncStuff->m_pMoveSyncCurr->vLoc);

    return TRUE;
}

void CNtlBehaviorCharJump::UpdateJumpStart(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed)
{
    RwV3d vPos = m_pActor->GetPosition();

	//by daneos.. removed to jump faster like in TW
    /*if(m_fJumpTime <= 0.2f)
    {
        m_fJumpTime += fElapsed;
        UpdatePositionMove(pMoveStuff, pJumpStuff, vPos, FALSE, fElapsed);

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);
        vPos.y = m_sHStuff.fFinialHeight;
    }
    else*/
    {
        RwV3d vNextPos = vPos;        
        UpdatePositionMove(pMoveStuff, pJumpStuff, vNextPos, TRUE, fElapsed);

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

        CNtlMath::MathRwV3dAssign(&vPos, vNextPos.x, vPos.y, vNextPos.z);

        if(vNextPos.y <= m_sHStuff.fFinialHeight)
        {
            ChangeJumpState(JUMPSTATE_END);
            vPos.y = m_sHStuff.fFinialHeight;
        }
        else
        {
            vPos.y = vNextPos.y;
        }
    }

    m_pActor->SetPosition(&vPos);
}

void CNtlBehaviorCharJump::UpdateJumpLoop(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed)
{
    RwV3d vPos = m_pActor->GetPosition();

    RwV3d vNextPos = vPos;    
    UpdatePositionMove(pMoveStuff, pJumpStuff, vNextPos, TRUE, fElapsed);

    CNtlMath::MathRwV3dAssign(&vPos, vNextPos.x, vPos.y, vNextPos.z);

    Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

    // 충돌 했는데 어느쪽으로도 이동이 불가능할 경우 멈춰준다.
    if(pMoveStuff->byMoveFlags != NTL_MOVE_NONE)
    {
        if(vNextPos.y <= m_sHStuff.fFinialHeight + 0.5f || m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            if(Logic_IsSwimming(m_pActor, &vPos, m_sHStuff.fWorldHeight, m_sHStuff.fWaterHeight))
                ChangeJumpState(JUMPSTATE_WATER_LANDING);
            else
                ChangeJumpState(JUMPSTATE_MOVE_LANDING);
        }
    }
    else
    {
        if(vNextPos.y <= m_sHStuff.fFinialHeight + 0.5f || m_byCollMoveImpossCnt >= COLLISION_MOVE_IMPOSSIBLE_COUNT)
        {
            if(Logic_IsSwimming(m_pActor, &vPos, m_sHStuff.fWorldHeight, m_sHStuff.fWaterHeight))
                ChangeJumpState(JUMPSTATE_WATER_LANDING);
            else
                ChangeJumpState(JUMPSTATE_STAND_LANDING);
        }
    }

    if(vNextPos.y < m_sHStuff.fFinialHeight)
    {
        vPos.y = m_sHStuff.fFinialHeight;
    }
    else
    {
        vPos.y = vNextPos.y;
    }

    m_pActor->SetPosition(&vPos);
}

void CNtlBehaviorCharJump::UpdateJumpStandLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed)
{
    if(m_bStandRandingEnd)
        return;

    RwV3d vPos = m_pActor->GetPosition();

    RwV3d vNextPos;
    RwV3dAssignMacro(&vNextPos, &vPos);

    if(!m_bStandRandingEnd)
    {
        UpdatePositionMove(pMoveStuff, pJumpStuff, vNextPos, TRUE, fElapsed);
        CNtlMath::MathRwV3dAssign(&vPos, vNextPos.x, vPos.y, vNextPos.z);

        Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

        if(vPos.y <= m_sHStuff.fFinialHeight)
        {
            vPos.y = m_sHStuff.fFinialHeight;
            m_bStandRandingEnd = TRUE;
        }
    }

    if(!m_bRandingVisualEffect)
    {
        if(m_sHStuff.bWaterAttr && m_sHStuff.fWorldHeight < m_sHStuff.fWaterHeight)
        {
            RwV3d vPosOrg = m_pActor->GetPosition();
            vPosOrg.y = m_sHStuff.fWaterHeight;
            CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
            pSobProxy->CreatePLEffect(NTL_VID_WATER_JUMP, &vPosOrg);

            m_bRandingVisualEffect = TRUE;
        }
    }

    if(vNextPos.y <= m_sHStuff.fFinialHeight)
    {
        vPos.y = m_sHStuff.fFinialHeight;

        if(!m_bRandingVisualEffect && !m_sHStuff.bWaterAttr)
        {
            // 사운드와  effect를 켠다.
            RwV3d vPosOrg = m_pActor->GetPosition();

            sNtlSoundPlayParameta tSoundParam;
            tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
            tSoundParam.pcFileName		= SOUND_JUMP_LANDING;
            tSoundParam.fXPos			= vPosOrg.x;
            tSoundParam.fYPos			= vPosOrg.y;
            tSoundParam.fZPos			= vPosOrg.z;

			GetSoundManager()->Play(&tSoundParam);

            // visual effect를 넣는다.
            CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
            pSobProxy->CreatePLEffect(NTL_VID_JUMP_LANDING, &vPosOrg);

            m_bRandingVisualEffect = TRUE;
        }
    }
    else
    {
        vPos.y = vNextPos.y;
    }

    m_pActor->SetPosition(&vPos);
}

void CNtlBehaviorCharJump::UpdateJumpMoveLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed)
{
    RwV3d vPos = m_pActor->GetPosition();

    RwV3d vNextPos;
    RwV3dAssignMacro(&vNextPos, &vPos);
    UpdatePositionMove(pMoveStuff, pJumpStuff, vNextPos, TRUE, fElapsed);

    CNtlMath::MathRwV3dAssign(&vPos, vNextPos.x, vPos.y, vNextPos.z);

    Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

    if(!m_bRandingVisualEffect)
    {
        if(m_sHStuff.bWaterAttr && m_sHStuff.fWorldHeight < m_sHStuff.fWaterHeight)
        {
            RwV3d vPosOrg = m_pActor->GetPosition();
            vPosOrg.y = m_sHStuff.fWaterHeight;
            CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
            pSobProxy->CreatePLEffect(NTL_VID_WATER_JUMP, &vPosOrg);

            m_bRandingVisualEffect = TRUE;
        }
    }

    if(vNextPos.y <= m_sHStuff.fFinialHeight)
    {
        vPos.y = m_sHStuff.fFinialHeight;

        if(!m_bRandingVisualEffect)
        {
            // 사운드와  effect를 켠다.
            RwV3d vPosOrg = m_pActor->GetPosition();

            sNtlSoundPlayParameta tSoundParam;
            tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
            tSoundParam.pcFileName		= SOUND_JUMP_LANDING;
            tSoundParam.fXPos			= vPosOrg.x;
            tSoundParam.fYPos			= vPosOrg.y;
            tSoundParam.fZPos			= vPosOrg.z;

			 GetSoundManager()->Play(&tSoundParam);

            // visual effect를 넣는다.
            CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
            //pSobProxy->CreatePLEffect(NTL_VID_JUMP_LANDING, &vPosOrg);
            pSobProxy->CreatePLEffect(NTL_VID_JUMP_LANDING, &vPos);

            m_bRandingVisualEffect = TRUE;
        }
    }
    else
    {
        vPos.y = vNextPos.y;
    }

    m_pActor->SetPosition(&vPos);
}


void CNtlBehaviorCharJump::UpdateJumpWaterLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed)
{
    RwV3d vPos = m_pActor->GetPosition();

    vPos.y += m_fJumpUpSpeed * fElapsed;

    Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

    if(m_fJumpUpSpeed < 0.0f)
    {
        if(vPos.y > m_sHStuff.fWorldHeight)
        {
            RwV3d vNextPos;
            RwV3dAssignMacro(&vNextPos, &vPos);

            RwV3d vDir = m_pActor->GetDirection();
            RwReal fSpeed = Logic_GetFrontRunSpeed(m_pActor) * 0.3f;

            CNtlVector vHeading, vDest;
            NtlGetDestination_Jump(vDir.x, vDir.y, vDir.z, fSpeed, vNextPos.x, vNextPos.y, vNextPos.z, pJumpStuff->vJumpDir.x, pJumpStuff->vJumpDir.z, pMoveStuff->byMoveFlags, (DWORD)(fElapsed*1000.f), 1.0f, &vHeading, &vDest);

            RwV3d vNewDir;
            CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, 0.0f, vHeading.z); 
            m_pActor->SetDirection(&vNewDir);

            RwV3d vDestPos;
            CNtlMath::MathRwV3dAssign(&vDestPos, vDest.x, vPos.y, vDest.z);

            RwBool bMoveArea = Logic_IsMoveWorldArea(m_pActor, vPos, vDestPos);
            if(bMoveArea)
            {
                vPos.x = vDest.x;
                vPos.z = vDest.z;
            }

            // 거품 이펙트를 표현한다.
            if(!m_bCreateBubbleEffect)
            {
                m_pWaterBubbleEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_SWIM_BUBBLE);
                m_pWaterBubbleEffect->SetPosition(&vPos);
                m_pWaterBubbleEffect->SetScale(m_pActor->GetSobProxy()->GetScale());
                m_bCreateBubbleEffect = TRUE;

            }
        }

        m_fJumpUpSpeed = m_fJumpUpSpeed + (JUMP_GRAVITY_SPEED * fElapsed * (5.0f) );
    }
    else
        m_fJumpUpSpeed = m_fJumpUpSpeed + (JUMP_GRAVITY_SPEED * fElapsed * 0.2f );

    if(vPos.y <= m_sHStuff.fWorldHeight)
        vPos.y = m_sHStuff.fWorldHeight;

    if(m_fJumpUpSpeed > 0.0f)
    {
        RwReal fSwimmingHeight = Logic_GetSwimmingHeight(m_pActor, &vPos);

        if(vPos.y >= fSwimmingHeight)
        {
            vPos.y = fSwimmingHeight;            
            Finish();

            // 거품 이펙트를 없앤다.
            if(m_pWaterBubbleEffect)
            {
                m_pWaterBubbleEffect->Finish();
                m_bCreateBubbleEffect = FALSE;
            }
        }
    }

    m_pActor->SetPosition(&vPos);
}

void CNtlBehaviorCharJump::ChangeJumpState(RwUInt8 byJumpState)
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());     
    SJumpStuff *pJumpStuff = pBeData->GetJumpStuff();

    if(byJumpState == JUMPSTATE_START)
    {
        CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
        pSobProxy->SetBaseAnimation(JUMP_START, FALSE);        

        m_fJumpTime = 0.0f;
        m_fJumpUpSpeed = JUMP_UP_VELOCITY;

        //////////////////////////////////////////////////////////////////////////
        if(m_pActor->GetClassID() == SLCLASS_PLAYER)
        {
            m_fJumpUpSpeed *= 1.1f;
        }

        //////////////////////////////////////////////////////////////////////////

        RwV3d vPos = m_pActor->GetPosition();

        sNtlSoundPlayParameta tSoundParam;
        tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
        tSoundParam.pcFileName		= SOUND_JUMP_KICK;
        tSoundParam.fXPos			= vPos.x;
        tSoundParam.fYPos			= vPos.y;
        tSoundParam.fZPos			= vPos.z;

		GetSoundManager()->Play(&tSoundParam);
    }
    else if(byJumpState == JUMPSTATE_LOOP)
    {
        CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
        CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
        pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);

        pSobProxy->SetBaseAnimation(JUMP_HOVER, TRUE);
    }
    else if(byJumpState == JUMPSTATE_STAND_LANDING)
    {
        CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
        CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
        pPLCharacter->SetBlend(BLEND_TWEEN);
        pSobProxy->SetBaseAnimation(JUMP_LANDING_LONG, FALSE);
    }
    else if(byJumpState == JUMPSTATE_MOVE_LANDING)
    {
        CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
        CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
        pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);

        pSobProxy->SetBaseAnimation(JUMP_LANDING_SHORT, FALSE);
    }
    else if(byJumpState == JUMPSTATE_WATER_LANDING)
    {
        // 물 effect를 표현한다. 
        RwV3d vPos = m_pActor->GetPosition();
        vPos.y = m_sHStuff.fWaterHeight;
        CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
        pSobProxy->CreatePLEffect(NTL_VID_WATER_JUMP, &vPos);

        m_bRandingVisualEffect = TRUE;
    }
    else if(byJumpState == JUMPSTATE_END)
    {
        CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
        pSobProxy->SetBaseAnimation(JUMP_LANDING_SHORT, FALSE);
        CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
        pPLCharacter->SetBlend(BLEND_TWEEN);
    }

    m_byJumpState = byJumpState;
    pJumpStuff->eState = (ECharJumpState)byJumpState;
}

/// 서버에 MoveSync 패킷을 보내서 동기를 맞춘다
void CNtlBehaviorCharJump::UpdateSendSyncCheck( RwReal fElapsedTime ) 
{
    if(m_pActor->GetClassID() != SLCLASS_AVATAR)
        return;

    m_fSyncSendTime += fElapsedTime;
    if(m_fSyncSendTime >= MOVE_SYNC_SEND_TIME)
    {
		//API_GetSLPacketGenerator()->SendChangePosition(m_pActor->GetPosition(), m_pActor->GetDirection());

        m_fSyncSendTime -= MOVE_SYNC_SEND_TIME;
    }
}