#pragma once

#include "NtlInstanceImVertexSystem.h"
#include "NtlResourceLineSystem.h"

/// LineSystem의 평면을 구성하는 정보를 가진 구조체
struct SLineSystemVertex
{
    RwV3d vStrartPoint;     ///< 시작점의 위치                 
    RwV3d vUpdatePoint;     ///< 업데이트 되는 끝점의 위치
    RwV3d vDir;             ///< 나아가는 방향
    RwIm3DVertex imVertices[6]; ///< Line을 구성하는 6개의 버텍스의 포인터
    RwReal fLifeTime;       ///< 라인의 Life Time
    RwV2d vLineSize;        ///< 라인의 Size (Size Action 때문에 필요하다)
};

typedef std::list<SLineSystemVertex*> ListLineSystemVertex;

/**
 * \ingroup NtlPresentation
 * \brief 라인 이펙트 인스턴스 클래스
 * \date 2006-09-21
 * \author agebreak
 */
class CNtlInstanceLineSystem : public CNtlInstanceImVertexSystem
{
public:
    CNtlInstanceLineSystem(void);
    virtual ~CNtlInstanceLineSystem(void);

    void* operator new(size_t size);
    void  operator delete(void* pObj);

    virtual RwBool Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld);
    virtual void   Delete();

    virtual RwBool Update(RwReal fElapsedTime);
    virtual RwBool Render();    

    RwInt32 GetPolyCount() {return (int)m_listLineSystemVertex.size();} ///< 현재의 폴리곤 갯수를 반환한다.

protected:
    virtual void Init();
    virtual void Reset();    
    virtual void UpdateColor(RwReal fDeltaTime);
    virtual void UpdateSize(RwReal fDeltaTime);

    void UpdateVertices(RwReal fElapsedTime);
    void BuildEmitterStandard();
    void CreateLine();                                              ///< 새로운 라인을 생성한다.
    void CreatePlane(SLineSystemVertex* pLineSystemVertex, const RwV3d& vStartAxis, const RwV3d& vEndAxis); ///< 두점의 정보로 평면을 생성하여 라인을 만든다.
    void UpdateZBillBoard(SLineSystemVertex* pLineSystemVertex);    ///< z축 빌보드로 면을 생성한다.    
    void UpdateNoneZBillBoard(SLineSystemVertex* pLineSystemVertex);///< Z축 빌보드가 적용되지 않은 면을 생성한다.
    void UpdateShake(SLineSystemVertex* pLineSystemVertex);         ///< Shake를 적용한다.    
    void UpdateRotate(RwReal fDeltaTime);                           ///< Rotate를 적용한다.
    
protected:
    CNtlResourceLineSystem*     m_pResourceLineSystem;          ///< 생성 정보를 가지고 있는 라인 리소스 클래스
    ListLineSystemVertex        m_listLineSystemVertex;         ///< LineSystem을 구성하는 면들의 리스트
    SLineSystemVertex*          m_poolLineSystemVertex;         ///< LineSystem Vertex Memory Pool의 포인터
    RwInt32                     m_nPoolIndex;                   ///< Memory Pool의 인덱스
    RwReal                      m_fCreateGapTime;               ///< 생성 주기를 비교하기위한 변수
    RwInt32                     m_nRoateSeed;                   ///< Rotate Bias에 사용될 Seed값               
};
