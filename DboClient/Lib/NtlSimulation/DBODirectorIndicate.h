/*****************************************************************************
*
* File			: DBODirectorIndicate
* Author		: agebreak
* Copyright	: (주)NTL
* Date			: 2007. 11. 20	
* Abstract		: 
*****************************************************************************
* Desc         : TS/TMQ에 사용되는 방향 지시 시스템 구현 클래스
*
*****************************************************************************/
#pragma once

#include "ceventhandler.h"
#include "NtlSLGlobal.h"
#include "NtlPLFadeController.h"

class CNtlPLCharacter;
class CNtlInstanceEffect;

/// TS/TMQ에 사용되는 방향 지시 시스템 구현 클래스
class CDBODirectorIndicate
{
public:
    CDBODirectorIndicate(CNtlPLCharacter* pPLCharacter);
    ~CDBODirectorIndicate(void);

    RwBool Create();
    VOID   Destory();
    RwBool Update(RwReal fElapsedTime);
    VOID   HandleEvents(RWS::CMsg &pMsg);
    VOID   SetCharacter(CNtlPLCharacter* pPLCharacter) {m_pCharacter = pPLCharacter;}

protected:
    RwBool      CreateDirector();        ///< 방향 지시 오브젝트를 생성한다.
    RwBool      CreateTargetEffect();    ///< 타게팅 이펙트를 생성한다.       
    RwBool      CalcDistance();          ///< 거리를 계산한다.
    VOID        UpdateDirection();       ///< 오브젝트의 방향을 업데이트한다.
    VOID        SetVisibleEffect(RwBool bVisible);  ///< 이펙트들의 표현 유무를 설정한다.                
    VOID        SetTargetEffectPos(RwV3d vPos);    ///< 타겟 이펙트를 타겟 위치에 위치시킨다.

protected:
    CNtlPLCharacter*                    m_pCharacter;             ///< 플레이어 캐릭터    
    CNtlInstanceEffect*                 m_pDirectorEffect;        ///< 방향 지시를 표시하는 이펙트
    CNtlInstanceEffect*                 m_pTargetEffect;          ///< 타겟을 나타내는 이펙트            
    RwV3d                               m_vTargetPos;             ///< 타겟의 위치
    RwBool                              m_bUpdate;                ///< 업데이트 유무 플래그
};
