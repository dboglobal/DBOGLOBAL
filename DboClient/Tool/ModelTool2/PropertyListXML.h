#pragma once

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"
#include "MTDef.h"

/**
 * \ingroup ModelTool2
 * \brief 클라이언트에서 사용할 캐릭터(MOB 포함), 오브젝트들의 PropertyList.xml을 생성하는 클래스
 * \date 2006-06-09
 * \author agebreak
 */
class CPropertyListXML : public CNtlXMLDoc
{
public:
    enum EProperyListMode
    {
        PROPERTY_CHARACTER,
        PROPERTY_OBJECT,
        PROPERTY_ITEM,
    };

public:
    CPropertyListXML(void);
    virtual ~CPropertyListXML(void);
    
    RwBool Save(const CHAR* szFileName, EProperyListMode eMode, CTreeCtrl* pTreeCtrl, TREEITEM_VECTOR vTreeItem);     ///< PropertyList를 XML형태로 저장한다.

protected:
    RwBool CreateHeader();
    RwBool CreateElemList(CTreeCtrl* pTreeCtrl, TREEITEM_VECTOR vTreeItem);

    RwBool SetIndent();                                             ///< XML에 스타일 시트를 적용한다. (indent.xls)

protected:
    IXMLDOMElement* m_pRootElem;
    IXMLDOMElement* m_pListElem;

    CString         m_strPropetyName;
    CString         m_strDataPath;
    
};
