#pragma once

#pragma warning( disable : 4996 )

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"
#include "NtlPLObjectProperty.h"
#include "NtlPLXMLScriptHelper.h"

#define OBJECT_XML_SCRIPT_VER       L"1.0"

/**
 * \ingroup NtlPresentation
 * \brief 오브젝트 스크립트(XML)을 Save/Load 하는 클래스
 * \date 2006-06-07
 * \author agebreak
 */
class CNtlPLObjectXMLScript : public CNtlXMLDoc
{
public:
    CNtlPLObjectXMLScript(void);
    virtual ~CNtlPLObjectXMLScript(void);

    RwBool SaveObjectScript(const char* strFileName, CNtlPLObjectProperty* pProperty);      ///< 스크립트 파일을 저장한다.
    RwBool LoadObjectScript(const char* strFileName, CNtlPLObjectProperty* pProperty);      ///< 스크립트 파일을 로드한다.

protected:    
    RwBool SaveHeader(CNtlPLObjectProperty* pProperty);             ///< 헤더 부분을 저장한다.
    RwBool SaveBody(CNtlPLObjectProperty* pProperty);               ///< 바디 부분을 저장한다.
    RwBool SaveLinkEffect(CNtlPLObjectProperty* pProperty);		    ///< Link Effect Data 부분을 저장한다.
	RwBool SaveAnimTable(CNtlPLObjectProperty* pProperty);       ///< AnimTable 부분을 저장한다.      

    RwBool LoadHeader(CNtlPLObjectProperty* pProperty);             ///< 헤더 부분을 로드한다.
    RwBool LoadBody(CNtlPLObjectProperty* pProperty);               ///< 헤더 부분을 로드한다.
    RwBool LoadLinkEffect(CNtlPLObjectProperty* pProperty);         ///< Link Effect Data 부분을 로드한다.
	RwBool LoadAnimTable(CNtlPLObjectProperty* pProperty);                     ///< AnimTable  부분을 로드한다.

protected:
    IXMLDOMElement*     m_pElemRoot;                ///< Root Element
    CNtlPLXMLScriptHelper m_XMLScriptHelper;        ///< DBO XML Script Helper
};
