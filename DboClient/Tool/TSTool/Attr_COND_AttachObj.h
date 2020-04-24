#pragma once


#include "Attr_Page.h"


// CAttr_COND_AttachObj 대화 상자입니다.

class CAttr_COND_AttachObj : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_COND_AttachObj)

public:
	CAttr_COND_AttachObj(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAttr_COND_AttachObj();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_COND_ATTACH_OBJ_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_dwWorldIdx;
	CString m_strObjectIdx;
};
