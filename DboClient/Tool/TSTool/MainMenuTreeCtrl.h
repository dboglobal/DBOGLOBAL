#pragma once


#include "TSToolDefine.h"


// CMainMenuTreeCtrl 뷰입니다.

class CMainMenuTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNCREATE(CMainMenuTreeCtrl)

public:
	CMainMenuTreeCtrl();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMainMenuTreeCtrl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	void								ClearAll( void );

	void								AddMainGroup( void );
	void								DelMainGroup( void );

	void								AddExceptionGroup( NTL_TS_TG_ID tgID );
	void								DelExceptionGroup( NTL_TS_TG_ID tgID );

public:
	afx_msg BOOL						OnEraseBkgnd(CDC* pDC);
	afx_msg void						OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


