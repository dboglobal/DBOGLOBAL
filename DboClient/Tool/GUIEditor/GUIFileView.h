#pragma once

#include "DirTreeCtrlMSC.h"	

#define ID_TREECTRL 10002

// CGUIFileView 뷰입니다.

class CGUIFileView : public CView
{
	DECLARE_DYNCREATE(CGUIFileView)

protected:
	CGUIFileView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGUIFileView();

	CDirTreeCtrlMSC	m_DirTree;

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CDirTreeCtrlMSC* GetTreeCtrl() { return &m_DirTree; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

extern CGUIFileView* g_pFileView;
