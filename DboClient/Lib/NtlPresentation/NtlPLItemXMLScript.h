#pragma once

#pragma warning( disable : 4996 )

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"
#include "NtlPLItemProperty.h"
#include "NtlPLXMLScriptHelper.h"

#define ITEM_XML_SCRIPT_VER       L"2.0"

/**
 * \ingroup NtlPresentation
 * \brief 아이템 스크립트(XML)을 Save/Load 하는 클래스
 * \date 2006-06-26
 * \author agebreak
 */
class CNtlPLItemXMLScript : public CNtlXMLDoc
{
public:
    CNtlPLItemXMLScript(void);
    virtual ~CNtlPLItemXMLScript(void);

    RwBool SaveItemScript(const char* strFileName, CNtlPLItemProperty* pProperty);      ///< 스크립트 파일을 저장한다.
    RwBool LoadItemScript(const char* strFileName, CNtlPLItemProperty* pProperty);      ///< 스크립트 파일을 로드한다.

protected:
    RwBool SaveHeader(CNtlPLItemProperty* pProperty);             ///< 헤더 부분을 저장한다.
    RwBool SaveBody(CNtlPLItemProperty* pProperty);               ///< Body 부분을 저장한다.        

    RwBool LoadHeader(CNtlPLItemProperty* pProperty);             ///< 헤더 부분을 로드한다.
    RwBool LoadBody(CNtlPLItemProperty* pProperty);               ///< Body 부분을 저장한다


    RwBool SaveLinkEffect(CNtlPLItemProperty* pProperty);    ///< Link Effect Data 부분을 저장한다.
    RwBool LoadLinkEffect(CNtlPLItemProperty* pProperty);    ///< Link Effect Data 부분을 로드한다.

    void   SaveTraceEvent(const SEventTrace* pEventTrace);  ///< Trace Event를 Save 한다.    
    void   LoadTraceEvent(SEventTrace* pEventTrace);           ///< Trace Event를 Load 한다.

    RwBool SaveAnimTable(CNtlPLItemProperty* pProperty);          ///< Item의 AnimTable을 저장한다.
    RwBool LoadAnimTable(CNtlPLItemProperty* pProperty);          ///< Item의 AnimTable을 로드한다.  

    RwBool SaveUpgradeEffect(CNtlPLItemProperty* pProperty);      ///< Item의 UpgradeEffect Proeprty를 저장한다.
    RwBool LoadUpgradeEffect(CNtlPLItemProperty* pProperty);      ///< Item의 UpgradeEffect Property를 로드한다.

protected:
    IXMLDOMElement*     m_pElemRoot;                ///< Root Element
    RwReal              m_fScriptVer;               ///< 현재 로드하고 있는 스크립트의 버전
    CNtlPLXMLScriptHelper m_XMLScriptHelper;        ///< DBO XML Script Helper
};
