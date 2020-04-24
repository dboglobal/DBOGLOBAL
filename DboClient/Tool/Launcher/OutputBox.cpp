// OutputBox.cpp : implementation file
//

#include "stdafx.h"
#include "OutputBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBox

COutputBox::COutputBox()
{
    m_bIsLog = FALSE;
}

COutputBox::~COutputBox()
{
}


BEGIN_MESSAGE_MAP(COutputBox, CStatic)
	//{{AFX_MSG_MAP(COutputBox)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBox message handlers

HBRUSH COutputBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called

	pDC->SetBkMode(TRANSPARENT);
    if(m_bIsLog)
    {
        pDC->SetTextColor(RGB(255, 128, 128));
    }
    else
    {
        pDC->SetTextColor(RGB(255, 255, 255));
    }    
	return (HBRUSH)GetStockObject(NULL_BRUSH);
	//return NULL;
}

void COutputBox::EraseBack( CDialog* pDlg ) 
{
    if(!pDlg) 
        return;

    CRect rectDlg, rectStatic, rect;            
    pDlg->GetWindowRect(&rectDlg);
    GetWindowRect(&rectStatic);            
    rect.left = rectStatic.left - rectDlg.left;
    rect.top = rectStatic.top - rectDlg.top;
    rect.right = rect.left + rectStatic.Width();
    rect.bottom = rect.top + rectStatic.Height();            
    pDlg->InvalidateRect(rect);        
}
