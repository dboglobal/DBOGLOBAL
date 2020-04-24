#pragma once

#include "NtlInstanceTraceSystem.h"

/**
 * \ingroup NtlPresentation
 * \brief Particle및 Mesh System의 꼬리 부분을 표현하기 위한 이펙트 클래스. TraceSystem에서 상속받아서 만든다.
 * \date 2006-12-29
 * \author agebreak
 */
class CNtlInstanceTrailSystem : public CNtlInstanceTraceSystem
{
public:
    CNtlInstanceTrailSystem(void);
    virtual ~CNtlInstanceTrailSystem(void);

    // new/delete override
    void* operator new(size_t size);
    void  operator delete(void* pObj);

    // Create/Delete override
    virtual RwBool  Create(CNtlInstanceComponentSystem* pComponentSystem, SNtlPrtStdEmitterTrail* pEmitterTrail);
    virtual void    Delete();

protected:
    virtual void Init();
    virtual void Reset();

    virtual void   CreateEdge();                                        ///< 새로운 Edge를 추가한다.
    void           SetEdgePoint();                                      ///< Edge 위치를 설정한다.

protected:
    SNtlPrtStdEmitterTrail*         m_pEmitterTrail;                    ///< Trail 생성 정보를 가지고 있는 프로퍼티 객체
    CNtlInstanceComponentSystem*    m_pAttachComponent;                 ///< Trail이 부착될 InstanceComponentSystem 객체
    
    RwV3d                           m_vPrevTrailPoint;                  ///< 이전 프레임에 계산한 Trail Point;
};
