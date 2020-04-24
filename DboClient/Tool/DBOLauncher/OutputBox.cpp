/*****************************************************************************
*
* File			: OutputBox.cpp
* Author		:
* Copyright		: (аж)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#include "stdafx.h"
#include "OutputBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//
// COutputBox
//
//////////////////////////////////////////////////////////////////////////


COutputBox::COutputBox( void )
{
    m_bIsLog = false;
}

COutputBox::~COutputBox( void )
{
}

void COutputBox::EraseBack( CDialog* pDlg ) 
{
	if ( NULL == pDlg ) 
	{
		return;
	}

	CRect rectDlg;
	pDlg->GetWindowRect( &rectDlg );

	CRect rectStatic;
	GetWindowRect( &rectStatic );

	CRect rect;
	rect.left = rectStatic.left - rectDlg.left;
	rect.top = rectStatic.top - rectDlg.top;
	rect.right = rect.left + rectStatic.Width();
	rect.bottom = rect.top + rectStatic.Height();            

	pDlg->InvalidateRect( rect );        
}

HBRUSH COutputBox::CtlColor( CDC* pDC, UINT nCtlColor ) 
{
	pDC->SetBkMode( TRANSPARENT );

	if ( m_bIsLog )
    {
        pDC->SetTextColor( RGB( 255, 128, 128 ) );
    }
    else
    {
        pDC->SetTextColor( RGB( 255, 255, 255 ) );
	}

	return (HBRUSH)GetStockObject( NULL_BRUSH );
}

BEGIN_MESSAGE_MAP( COutputBox, CStatic )
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
