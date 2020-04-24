#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl.h"
#include "TSView.h"


/** 
	CSCtrl
*/


CSCtrl::CSCtrl( CTSView* pView )
: m_pView( pView )
{
}

CTSDoc*	CSCtrl::GetDoc( void )
{
	return m_pView->GetDocument();
}
