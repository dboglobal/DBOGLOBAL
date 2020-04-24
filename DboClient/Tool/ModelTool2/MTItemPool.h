#pragma once

#include "MTItem.h"

typedef std::map<std::string, CMTItem*> MapItem;
/**
 * \ingroup ModelTool2
 * \brief Model Tool에서 생성된 Item들을 관리하는 Pool 클래스. Item은 TreeItem과 ItemView Pane 두군데서 같이 사용하기 때문에
 * Pool 클래스를 통해서 사용한다.
 * \date 2006-11-02
 * \author agebreak
 */
class CMTItemPool
{
public:
    CMTItemPool(void);
    virtual ~CMTItemPool(void);

    static CMTItemPool* GetInstance() {return m_pInstance;}

    void        AddItem(CMTItem* pItem);        ///< Item을 Map에 추가한다.
    CMTItem*    GetItem(RwChar* strItemName);   ///< Map에서 Item을 가져온다.

protected:
    static CMTItemPool* m_pInstance;

    MapItem     m_mapItem;          ///< 생성된 Item들의 Map

};
