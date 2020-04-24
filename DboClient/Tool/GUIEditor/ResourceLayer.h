#pragma once

#include "Control/RollupCtrl.h"

// CResourceLayer 뷰입니다.

class CResourceLayer : public CView
{
	DECLARE_DYNCREATE(CResourceLayer)

public:
	enum RESOURCEDLG { /*RESOURCEFILEDLG = 0,*/ RESOURCELISTDLG , RESOURCEATTRIBUTEDLG };

protected:
	CResourceLayer();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CResourceLayer();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CDialog*	GetRollupDlg( RESOURCEDLG eID );	

protected:
	DECLARE_MESSAGE_MAP()

protected:
	//variables
	CRollupCtrl	m_RollUpCtrl;
	BOOL		m_bInitialized;			// RollUpCtrl이 Init되면 TRUE;

public:
		
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual void OnInitialUpdate();
};


