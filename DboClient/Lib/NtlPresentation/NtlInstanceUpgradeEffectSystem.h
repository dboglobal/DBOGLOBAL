#pragma once

#include "NtlInstanceImVertexSystem.h"

class CNtlPLItem;

///< Grade Effect가 붙는 Bone 쌍의 개수
enum EGradeBoneIndex
{
    GRADE_EFFECT_BONE1,     ///< 첫번째 쌍의 Bone에 붙인다. (칼등의 일반 무기)
    GRADE_EFFECT_BONE2,     ///< 두번째 쌍의 Bone에 붙인다. (글러브, 클로우.)
};

/**
 * \ingroup NtlPresentation
 * \brief Item의 Upgrade Effect를 구현한 클래스 (NOTE: Resource 클래스 없이 단독으로 쓰인다.)
 * Effect Tool에서 제작 되지 않는 Effect
 * \date 2006-12-13
 * \author agebreak
 */
class CNtlInstanceUpgradeEffectSystem : public CNtlInstanceImVertexSystem
{
public:
    CNtlInstanceUpgradeEffectSystem(void);
    virtual ~CNtlInstanceUpgradeEffectSystem(void);

    void* operator new(size_t size);
    void  operator delete(void* pObj);

    virtual RwBool Create(CNtlPLItem* pItem, EGradeBoneIndex eBoneIndex = GRADE_EFFECT_BONE1);
    virtual void   Delete();

    virtual RwBool Update(RwReal fElapsedTime);
    virtual RwBool Render();

    void    SetTexture(RwChar* szTextureName);             ///< Texture를 설정한다.    
protected:
    virtual void Init();
    virtual void Reset();

    RwBool  UpdateVertices(RwReal fElapsedTime);

private:
    // 이 Create 함수는 사용하지 않는다.
    virtual RwBool Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld) {return TRUE;}

protected:
    CNtlPLItem*     m_pItem;                        ///< Effect가 붙은 Item 객체의 포인터    
    RwReal          m_fEffetWidthRatio;
    RwBool          m_bWidthSizeUp;    
    EGradeBoneIndex      m_eGradeBone;                   ///< 어느 본에 붙을지의 설정값
};
