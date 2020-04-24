#pragma once

#include "NtlXMLDoc.h"

/**
 * \ingroup Client
 * \brief 추가 정보를 에러리포트에 기록하는 클래스
 * \date 2007-01-25
 * \author agebreak
 */
class CDboExtraErrorReport : public CNtlXMLDoc
{
public:
    CDboExtraErrorReport(void);
    virtual ~CDboExtraErrorReport(void);

    RwBool Open();                                          ///< 파일을 오픈한다.
    RwBool Save(const char* szFileName);                    ///< 파일을 기록하고 저장한다.

    void WriteInfo(RwChar* szNodeName, RwChar* szInfo);     ///< 파일에 정보를 기록한다.
    void WriteInfo(RwChar* szNodeName, WCHAR* szInfo);      ///< 파일에 정보를 기록한다.
    static void SetDXDiagLog();                             ///< Dxdiag 로그를 기록한다.

protected:
    RwBool SetIndent();                                             ///< XML에 스타일 시트를 적용한다. (indent.xls)

protected:
    IXMLDOMElement*     m_pRootElem;                ///< Root Element
};
