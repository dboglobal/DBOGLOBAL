#pragma once

#include "NtlBehaviorCharMove.h"

/**
 * \ingroup NtlSimulation
 * \brief 캐릭터의 점프 상태에서의 행위 클래스
 *
 * \date 2009-04-17
 * \author agebreak
 */
class CNtlBehaviorCharJump : public CNtlBehaviorCharMove
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharJump, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharJump)

public:

    CNtlBehaviorCharJump();
    ~CNtlBehaviorCharJump();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);    
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

private:
    SWorldHeightStuff	m_sHStuff;				/** world height stuff */

    RwReal              m_fSyncSendTime;
    RwUInt8				m_byJumpState;	        /** jump state */
    RwReal				m_fJumpTime;		    /** jump time */
    RwReal				m_fJumpUpSpeed;	        /** 위로 jump 하는  speed */
    RwUInt8				m_byCollMoveImpossCnt;  /** collision move impossible count */
    RwBool				m_bRandingVisualEffect;	/** landing visual effect를 표현 하였는가? */
    RwBool				m_bStandRandingEnd;
    RwBool              m_bCreateBubbleEffect;  /** 거품 이펙트를 표현하였는가? */
    RwBool              m_bNoneDirJump;         ///< 제자리 점프 여부
    CNtlInstanceEffect* m_pWaterBubbleEffect;   /** 물속으로 빠졌을때 나오는 거품 이펙트 */

    void UpdatePositionMove(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, OUT RwV3d& vPos, RwBool bIncHeight, RwReal fElapsed, RwReal fSpeedScale = 1.0f);

    void UpdateJumpStart(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateJumpLoop(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateJumpStandLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateJumpMoveLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateJumpWaterLanding(SMoveStuff *pMoveStuff, SJumpStuff *pJumpStuff, RwReal fElapsed);
    void UpdateSendSyncCheck(RwReal fElapsedTime);
    RwBool UpdateMoveSync(RwV3d vPos, RwReal fElapsedTime);       ///< Udpate function to calculate the current MoveSync value

    void ChangeJumpState(RwUInt8 byJumpState);
};
