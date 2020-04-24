// MPPLogView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MrPoPo.h"
#include "MPPLogView.h"


// CMPPLogView

IMPLEMENT_DYNCREATE(CMPPLogView, CRichEditView)

CMPPLogView* CMPPLogView::m_pInstance = NULL;

CMPPLogView::CMPPLogView()
{
    m_pInstance = this;    
}

CMPPLogView::~CMPPLogView()
{
}

BEGIN_MESSAGE_MAP(CMPPLogView, CRichEditView)
END_MESSAGE_MAP()


// CMPPLogView 진단입니다.

#ifdef _DEBUG
void CMPPLogView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CMPPLogView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

void CMPPLogView::AddLog( const CString& strLog ) 
{
    CString strText = strLog;
    strText += L"\n";
    GetRichEditCtrl().ReplaceSel(strText);    

    m_pOwnerPane->Select();
}

#endif
#endif //_DEBUG


// CMPPLogView 메시지 처리기입니다.
