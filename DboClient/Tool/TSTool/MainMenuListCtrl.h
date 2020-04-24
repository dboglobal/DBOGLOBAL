#pragma once


#include "TSToolDefine.h"


// CMainMenuListCtrl 뷰입니다.

class CMainMenuListCtrl : public CListCtrl
{
	DECLARE_DYNCREATE(CMainMenuListCtrl)

public:
	CMainMenuListCtrl();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMainMenuListCtrl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CArray< CString, const CString& >	m_aStringList;

public:
	void								Init( void );
	void								ClearAll( void );

	void								AddList( const CString& strName );

public:
	afx_msg BOOL						OnEraseBkgnd(CDC* pDC);
	afx_msg void						OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


