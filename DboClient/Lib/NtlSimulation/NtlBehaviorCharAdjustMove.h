#pragma once

#include "NtlBehaviorCharMove.h"

/**
 * \ingroup NtlSimulation
 * \brief 이동이 멈춘후에 서버 위치에 맞게 위치 보간을 처리하는 행동 클래스
 *
 * \date 2009-04-15
 * \author agebreak
 */
class CNtlBehaviorCharGroundAdjustMove : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharGroundAdjustMove, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharGroundAdjustMove)

public:

    CNtlBehaviorCharGroundAdjustMove();
    ~CNtlBehaviorCharGroundAdjustMove();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual void UpdateData(void);
    virtual void SetAnim();

protected:    
    RwBool  IsNotAdjustMove();                  ///< Adjust Move를 하지 않는 상태인지 체크한다.

protected:
    SMoveStuff			m_MoveStuff;
    SWorldHeightStuff	m_sHStuff;				/** world height stuff */
    RwBool				m_bSlidingAdjust;
    RwV3d				m_vAdjustDir;
};
