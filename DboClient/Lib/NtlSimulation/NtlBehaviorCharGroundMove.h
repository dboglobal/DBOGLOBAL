#pragma once

#include "NtlBehaviorCharMove.h"

/**
 * \ingroup NtlSimulation
 * \brief 일반적인 지형 이동을 처리하는 행동 클래스
 *
 * \date 2009-04-15
 * \author agebreak
 */
class CNtlBehaviorCharGroundMove : public CNtlBehaviorCharMove
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharGroundMove, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharGroundMove)

    enum 
    {
        DASH_STATE_NONE,
        DASH_STATE_MOVE,
        DASH_STATE_LANDING,
        DASH_STATE_END
    };

public:
    CNtlBehaviorCharGroundMove();
    ~CNtlBehaviorCharGroundMove();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
    virtual void UpdateData(void);

    RwUInt32 GetDashState(void) { return m_byDashState; }


protected:
    RwBool	IsInDestMoveRound(void);
    RwBool	IsInFollowMoveRound(void);
    RwBool	IsInMoveRound(void);

    void	SetPathNode(void);
    void	SetTransform(void);
    void	SetAnim(RwUInt8 byMoveFlags);
    void	SetDashAnimEnd(void);
    void    SetIdle2RunAnim(void);                 ///< Idle -> Run 보간 애니메이션을 설정한다.
    void	SetServerSyncPosition(RwV3d vSyncPos);    
    RwBool  IsIdleAnimationPlaying();              ///< Idle 애니메이션 플레이중인지 확인한다.


    RwBool	UpdateDirectionMove(RwReal fElapsed);
    RwBool	UpdateLocationMove(RwReal fElapsed);

    RwBool  ObjectCollision(OUT RwV3d &vPos, RwReal fOldActorHeight, RwReal fSpeed, RwReal fElapsed, OUT RwUInt8* pColliResult);
    RwBool	UpdateTargetMove(RwReal fElapsed);
    RwBool	UpdateDashMove(RwReal fElapsed);
    RwBool	UpdateDashTargetMove(RwReal fElapsed);    

    RwBool  UpdateMoveSync(RwReal fElapsedTime, OUT RwV3d* pDestPos, OUT RwV3d* pNewDir);       ///< 현재의 MoveSync 값을 계산하는 Udpate 함수
    void	UpdateServerSyncPosition(RwReal fElapsed);
    void	UpdateSendSyncCheck(RwReal fElapsed);

    void	CreateStartDashCamera(void);
    void	DestroyEndDashCamera(void);

    void	CreateDashEffect(RwUInt32 uiMoveDirFlags, RwChar *pKey1, RwChar *pKey2);
    void	DestroyDashEffect(void);
    void	DestroyDashEffectSmoke(void);
    void	UpdateDashEffect(RwUInt32 uiMoveDirFlags, RwV3d& vDir);
    void	UpdateDashLineEffect(RwV3d& vPos, RwV3d& vDir);

    void	FootStepMaterialProc(RWS::CMsg &pMsg);

    // position 변화 check
    void	LimitPositionChangeCheck(RwReal fElapsed);
    RwBool  CheckFalling(RwReal fOldActorHeight, RwReal fFinalHeight, RwReal fSpeed, RwUInt8 byMoveFlag, RwV3d* pNewPos = NULL);              ///< 폴링 상태 체크 

protected:

    // move stuff
    SMoveStuff			m_MoveStuff;
    SWorldHeightStuff	m_sHStuff;				/** world height stuff */
    RwBool				m_bSlowMove;			/** If the target is near to the target when the movement is first started ... */
    RwReal				m_fSlowMoveTime;		/** slow move일 경우 누적 시간을 위해 사용 */

    // speed
    RwReal				m_fDefaultWalkSpeed;
    RwReal				m_fDefaultRunSpeed;
    RwReal				m_fDefaultBackSpeed;	


    // dash
    RwUInt8				m_byDashState;
    RwReal				m_fDashDestPosY;
    RwReal				m_fDashLadingDist;

    // dash effect
    CNtlPLEntity		*m_pDashEffect;
    CNtlPLEntity		*m_pDashLine;
    CNtlPLEntity		*m_pDashEffectSmoke;
    RwReal				m_fEffectSmokeTime;
    RwV3d				m_vDashEffOffset;

    // camera
    RwBool				m_bDashCameraEnable;

    // possible 변화 check
    RwReal				m_fPosChangeTime;
    RwUInt8				m_byPosChangeCnt;
    RwV3d				m_vChangeOldPos;

    // collision
    RwUInt8				m_byCollMoveImpossCnt;		// collision move impossible count

    // server 동기 위치 보정.
    RwBool				m_bServerSyncPos;
    RwV3d				m_vServerSyncDir;
    RwReal				m_fServerSyncDist;
    RwReal				m_fCurrServerSyncDist;
    RwReal				m_fDeltaServerSyncDist;

    // move sync
    RwBool				m_bSyncSendCheck;
    RwReal				m_fSyncSendTime;

    // 보간 애니메이션을 위한 변수들
    RwBool              m_bInterAnimEnd;
    RwReal              m_fInterAnimTime;   
};
