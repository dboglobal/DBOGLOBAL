#pragma once

#include "resource.h"

// CSystemPane 폼 뷰입니다.

class CSystemPane : public CFormView
{
	DECLARE_DYNCREATE(CSystemPane)

protected:
	CSystemPane();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSystemPane();

public:
	enum { IDD = IDD_SYSTEMPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	static CSystemPane* m_pInstance;
	static CSystemPane*	GetInstance()	{ return m_pInstance; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void	SetBackData( const TCHAR* pBackName, int nOffSetX, int nOffsetY );

	CString m_szBackName;				///< File Name
	CString m_szOffsetX;				///< Offset X
	CString m_szOffsetY;				///< Offset Y

	afx_msg void OnBnClickedSystemBackFind();
	afx_msg void OnBnClickedSystemApply();	
};

static CSystemPane* GetSystemPane()
{
	return CSystemPane::GetInstance();
}