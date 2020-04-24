// GUIFileInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIFileInfoView.h"

CGUIFileInfoView* g_pFileInfoView = NULL;
// CGUIFileInfoView

IMPLEMENT_DYNCREATE(CGUIFileInfoView, CFormView)

CGUIFileInfoView::CGUIFileInfoView()
	: CFormView(CGUIFileInfoView::IDD)
	, m_strRsrFilename(_T(""))
	, m_strSrfFilename(_T(""))
	, m_strFrmFilename(_T(""))
	, m_strWorkDir(_T(""))
{
	
}

CGUIFileInfoView::~CGUIFileInfoView()
{
}

void CGUIFileInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_RSRFILE, m_strRsrFilename);
	DDX_Text(pDX, IDC_STATIC_SRFFILE, m_strSrfFilename);
	DDX_Text(pDX, IDC_STATIC_FRMFILE, m_strFrmFilename);
	DDX_Text(pDX, IDC_STATIC_WORKDIR, m_strWorkDir);
}

BEGIN_MESSAGE_MAP(CGUIFileInfoView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGUIFileInfoView diagnostics

#ifdef _DEBUG
void CGUIFileInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGUIFileInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGUIFileInfoView message handlers

void CGUIFileInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

void CGUIFileInfoView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CGUIFileInfoView::SetRsrText( CString& strFilename )
{
	m_strRsrFilename = strFilename;	
	UpdateData( FALSE );
}

void CGUIFileInfoView::SetSrfText( CString& strFilename )
{
	m_strSrfFilename = strFilename;
	UpdateData( FALSE );
}

void CGUIFileInfoView::SetFrmText( CString& strFilename )
{
	m_strFrmFilename = strFilename;
	UpdateData( FALSE );
}

void CGUIFileInfoView::SetWorkDirText( CString& strPath )
{
	m_strWorkDir = strPath;
	UpdateData( FALSE );
}