#pragma once



// CTSProjectMainMenuBar 폼 뷰입니다.

class CTSProjectMainMenuBar : public CSizingControlBarCF
{
	DECLARE_DYNCREATE(CTSProjectMainMenuBar)

protected:
	CFrameWnd* m_pFrameWnd;
	CCreateContext m_Context;

public:
	CTSProjectMainMenuBar();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTSProjectMainMenuBar();

	virtual BOOL Create( CWnd* pParentWnd,
						 CRuntimeClass* pViewClass,
						 CCreateContext* pContext = NULL,
						 LPCTSTR lpszWindowName = NULL,
						 DWORD dwStyle = WS_CHILD|WS_VISIBLE|CBRS_TOP,
						 UINT nID = AFX_IDW_PANE_FIRST );
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


