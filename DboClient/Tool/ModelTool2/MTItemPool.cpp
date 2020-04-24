#include "StdAfx.h"
#include "MTItemPool.h"
#include "NtlPLSceneManager.h"

CMTItemPool* CMTItemPool::m_pInstance = NULL;

CMTItemPool::CMTItemPool(void)
{
    m_pInstance = this;
}

CMTItemPool::~CMTItemPool(void)
{
    MapItem::iterator it = m_mapItem.begin();
    for(; it != m_mapItem.end(); ++it)
    {
        CMTItem* pItem = (*it).second;
        GetSceneManager()->DeleteEntity(pItem);
        pItem = NULL;
    }

    m_mapItem.clear();
}

void CMTItemPool::AddItem( CMTItem* pItem ) 
{
    // Mesh Name을 Key로 하여 중복된 데이터는 추가하지 않는다.
    // NOTE : 만약 같은 Mesh를 쓰는 두개의 서로 다른 Item이 있다면 문제가 있다. (현재 그런 경우가 없기때문에, 우선 이렇게 가정한다.-_-;)
    if(m_mapItem.find(pItem->GetProperty()->GetName()) == m_mapItem.end())
    {
        // Visual Manager에 추가한다.
        GetSceneManager()->AddPLEntity(pItem);

        m_mapItem[pItem->GetProperty()->GetName()] = pItem;
    }
}

CMTItem* CMTItemPool::GetItem(RwChar* strItemName)
{
    if(m_mapItem.find(strItemName) == m_mapItem.end())
    {
        return NULL;
    }

    return m_mapItem[strItemName];   
}