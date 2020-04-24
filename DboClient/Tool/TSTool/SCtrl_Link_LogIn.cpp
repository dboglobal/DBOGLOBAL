#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Link_LogIn.h"
#include "Shape_Link.h"
#include "TSDoc.h"
#include "Shape_Narration.h"
#include "Shape_UsrSel.h"
#include "Shape_UnifiedNarration.h"


/** 
	CSCtrl_Link_LogIn
*/


CSCtrl_Link_LogIn::CSCtrl_Link_LogIn( CTSView* pView )
: CSCtrl_Link( pView )
{
}

void CSCtrl_Link_LogIn::OnLButtonDown( const CPoint &point, UINT nFlags )
{
	CShape* pShape = GetDoc()->GetShapeAt( point );
	if ( NULL == pShape ) return;

	if ( NULL != DYNAMIC_DOWNCAST( CShape_Narration, pShape ) )
	{
		CSCtrl_Link::OnLButtonDown( point, nFlags );
	}
	else if ( NULL != DYNAMIC_DOWNCAST( CShape_UnifiedNarration, pShape ) )
	{
		CSCtrl_Link::OnLButtonDown( point, nFlags );
	}
	else if ( NULL != DYNAMIC_DOWNCAST( CShape_UsrSel, pShape ) )
	{
		CSCtrl_Link::OnLButtonDown( point, nFlags );
	}
	else
	{
		AfxMessageBox( _T("해당 컨테이너에 LogIn 링크는 놓을 수 없습니다") );
	}
}

CShape_Link* CSCtrl_Link_LogIn::New( const CPoint& point, CShape* pShape )
{
	CShape_Link* pLink = CSCtrl_Link::New( point, pShape );
	pLink->SetBranchType( CShape_Link::eBRANCH_TYPE_LOGIN );
	return pLink;
}
