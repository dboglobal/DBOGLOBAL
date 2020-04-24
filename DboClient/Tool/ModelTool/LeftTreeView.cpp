// LeftTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "LeftTreeView.h"

// CLeftTreeView
CLeftTreeView* CLeftTreeView::m_pInstance = NULL;

IMPLEMENT_DYNCREATE(CLeftTreeView, CFormView)

CLeftTreeView::CLeftTreeView()
	: CFormView(CLeftTreeView::IDD)
{
	m_pInstance = this;
}

CLeftTreeView::~CLeftTreeView()
{
}

void CLeftTreeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);	
}

BEGIN_MESSAGE_MAP(CLeftTreeView, CFormView)
	
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CLeftTreeView diagnostics

#ifdef _DEBUG
void CLeftTreeView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftTreeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeftTreeView message handlers


BOOL CLeftTreeView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CLeftTreeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class	


	// Property Sheet를 초기화 한다.
    m_propSheet.AddPage(&m_pageClump);
	m_propSheet.AddPage(&m_pagePC);
    m_propSheet.AddPage(&m_pageMob);
	m_propSheet.AddPage(&m_pageNPC);	
	m_propSheet.AddPage(&m_pageItem);
	m_propSheet.AddPage(&m_pageGround);
	

	m_propSheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_propSheet.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	m_propSheet.ModifyStyle(0, WS_TABSTOP);

	CRect rect;
    GetClientRect(&rect);

	CTabCtrl* tabCtrl = m_propSheet.GetTabControl();
	tabCtrl->MoveWindow(0, 0, rect.Width(), rect.Height());
	m_propSheet.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	m_propSheet.SetActivePage(0);
}

void CLeftTreeView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    if(m_pagePC.GetSafeHwnd())    
    {
        m_pagePC.OnSize(nType, cx, cy);
    }
    if(m_pageMob.GetSafeHwnd())
    {
        m_pageMob.OnSize(nType, cx, cy);
    }
}
