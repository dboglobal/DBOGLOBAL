// GUIControlView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIControlView.h"

CGUIControlView* g_pControlView = NULL;

// CGUIControlView

IMPLEMENT_DYNCREATE(CGUIControlView, CFormView)

CGUIControlView::CGUIControlView()
	: CFormView(CGUIControlView::IDD)
{
}

CGUIControlView::~CGUIControlView()
{
}

void CGUIControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGUIControlView, CFormView)
END_MESSAGE_MAP()


// CGUIControlView 진단입니다.

#ifdef _DEBUG
void CGUIControlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGUIControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CGUIControlView 메시지 처리기입니다.
