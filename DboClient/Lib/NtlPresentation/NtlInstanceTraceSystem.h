#pragma once

#include <list>
#include "NtlInstanceImVertexSystem.h"
#include "NtlAnimEventData.h"

#define WEAPON_BONE_NAME_1      "TracePoint1"
#define WEAPON_BONE_NAME_2      "TracePoint2"

// 궤적에 사용되는 vertex 구조체
struct STraceEdge
{
    RwReal          fLifeTime;       ///< Edge의 생명 시간
    RwIm3DVertex    vVertices[2];    ///< Edge를 구성하는 두개의 Vertex    
};

typedef std::list<STraceEdge*> ListTraceEdge;

/**
 * \ingroup NtlPresentation
 * \brief 궤적(Trace) 이펙트 인스턴스 클래스
 * \date 2006-09-14
 * \author agebreak
 */
class CNtlInstanceTraceSystem : public CNtlInstanceImVertexSystem
{
public:
    CNtlInstanceTraceSystem(void);
    virtual ~CNtlInstanceTraceSystem(void);

    void* operator new(size_t size);
    void  operator delete(void* pObj);

    virtual RwBool Create(SEventTrace* pEventTrace, CNtlPLAttach* pAttach);        ///< TraceEvent 객체로 이펙트를 생성한다.
    virtual void   Delete();

    virtual RwBool Update(RwReal fElapsedTime);
    virtual RwBool Render();

protected:
    virtual void Init();
    virtual void Reset();
    
    void    SetEdgePoint(CNtlPLAttach* pAttach);                  ///< Edge를 찍을 Vertex의 위치를 설정한다.
    virtual RwBool  UpdateVertices(RwReal fElapsedTime);          ///< Vertex들의 정보를 Update한다.
    virtual void    CreateEdge();                                         ///< 새로운 Edge를 추가한다.
    void    CreateSplinePath();                                   ///< Spline Path Point들을 만들어서 버텍스 버퍼에 넣는다.

    virtual void UpdateUV();                                      ///< UV값을 Update한다.
    virtual void UpdateColor();                                   ///< Color값을 Update한다.

private:
    // 이 Create 함수는 사용하지 않는다.
    virtual RwBool Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld) {return TRUE;}

protected:    
    SEventTrace*    m_pEventTrace;              ///< 궤적 정보를 가지고 있는 Event 객체
    CNtlPLAttach*   m_pAttach;                  ///< 궤적을 붙일 캐릭터 or Item

    RwV3d           m_vStartPoint;              ///< 궤적의 시작 Point
    RwV3d           m_vEndPoint;                ///< 궤적의 끝 Point

    ListTraceEdge   m_listTraceEdge;            ///< 현재 궤적을 구성하고 있는 Edge들의 리스트
    ListTraceEdge   m_listControlPoint;         ///< ControlPoint들의 List
    STraceEdge*     m_poolTraceEdge;            ///< Trace Edge 구조체 Pool (Spline에서 Control Point가 된다)
    STraceEdge*     m_poolControlPoint;         ///< 실제 그려질 Spline을 구성하는 구조체 Pool
    RwInt32         m_nPoolIndex;               ///< Pool에서 사용할 Index
    RwInt32         m_nControlPointIndex;       ///< Spline의 Pool Index
    RwReal          m_fEdgeGapTemp;             ///< Edge Gap을 체크하기 위한 변수    
};

typedef std::list<CNtlInstanceTraceSystem*> ListTraceSystem;
