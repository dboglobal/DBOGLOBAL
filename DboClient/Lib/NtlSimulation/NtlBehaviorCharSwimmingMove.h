#pragma once

#include "NtlBehaviorCharMove.h"

class CNtlBehaviorCharSwimmingMove : public CNtlBehaviorCharMove
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharSwimmingMove, NTL_DEFAULT_MEMORY_POOL)
        DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharSwimmingMove)

protected:

    // move stuff
    SMoveStuff			m_MoveStuff;
    SWorldHeightStuff	m_sHStuff;				/** world height stuff */

    CNtlPLEntity		*m_pPLSwimFront;
    CNtlPLEntity		*m_pPLSwimBack;

    // possible 변화 check
    RwReal m_fPosChangeTime;
    RwUInt8 m_byPosChangeCnt;
    RwV3d m_vChangeOldPos;

    // collision
    RwUInt8 m_byCollMoveImpossCnt;		// collision move impossible count

protected:

    void	SetTransform(void);
    void	SetAnim(void);
    void	SetPathNode(void);

    RwBool	IsInMoveRound(void);
    RwBool	IsInDestMoveRound(void);

    void	CreateSwimFrontEffect(void);
    void	CreateSwimBackEffect(void);

    void	DestroySwimFrontEffect(void);
    void	DestroySwimBackEffect(void);

    // position 변화 check
    void	LimitPositionChangeCheck(RwReal fElapsed);

    RwBool	UpdateDirectionMove(RwReal fElapsed);
    RwBool	UpdateLocationMove(RwReal fElapsed);
    RwBool  UpdateTargetMove(RwReal fElapsed);              ///< 물에서 타겟을 따라갈때

    void	UpdateSwimEffect(void);

public:

    CNtlBehaviorCharSwimmingMove();
    ~CNtlBehaviorCharSwimmingMove();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
    virtual void UpdateData(void);
};