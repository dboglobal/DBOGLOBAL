#pragma once
#include "NtlPLItem.h"
#include "MTCharacterInfo.h"

/**
 * \ingroup ModelTool2
 * \brief Model Tool에 쓰이는 Item 클래스
 * \date 2006-06-26
 * \author agebreak
 */
class CMTItem : public CNtlPLItem
{
public:
    CMTItem(void);
    virtual ~CMTItem(void);

    /// Memory Pool을 사용하지 않기 때문에 new/delete를 Overriding 한다.
    void* operator new(size_t size);
    void operator delete(void *pObj);

    virtual RwBool  Create(const RwChar* szDffFileName, RwBool bEquip = FALSE);                         ///< Clump 파일로부터 객체를 생성한다.
    virtual void    Destroy(void);    

    virtual void	SetRemoveEquip();    
    virtual void    SetVisible(RwBool bVisible);
    virtual void    AddWorld(void);    
    virtual void    RemoveWorld();

    CMTClump* GetClumpInfo() {return &m_MTClump;}                               ///< MTClump 객체를 반환한다.

    // Link Effect 관련
    CNtlInstanceEffect* GetLinkEffectFromName(RwChar* szName);              ///< Name에 해당하는 LinkEffect를 반환한다.

protected:
    CMTClump    m_MTClump;                                                      ///< Clump 정보 객체    
    RwBool      m_bEquipView;                                                   ///< 장착뷰용으로 생성된 아이템인지 아닌지 유무
};
