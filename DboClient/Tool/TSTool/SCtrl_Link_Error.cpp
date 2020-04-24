#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Link_Error.h"
#include "Shape_Link.h"
#include "TSDoc.h"
#include "Shape_GAct.h"


/** 
	CSCtrl_Link_Error
*/


CSCtrl_Link_Error::CSCtrl_Link_Error( CTSView* pView )
: CSCtrl_Link( pView )
{
}

void CSCtrl_Link_Error::OnLButtonDown( const CPoint &point, UINT nFlags )
{
	CShape* pShape = GetDoc()->GetShapeAt( point );
	if ( NULL == pShape ) return;

	if ( NULL != DYNAMIC_DOWNCAST( CShape_GAct, pShape ) )
	{
		CSCtrl_Link::OnLButtonDown( point, nFlags );
	}
	else
	{
		AfxMessageBox( _T("해당 컨테이너에 Error 링크는 놓을 수 없습니다") );
	}
}

CShape_Link* CSCtrl_Link_Error::New( const CPoint& point, CShape* pShape )
{
	CShape_Link* pLink = CSCtrl_Link::New( point, pShape );
	pLink->SetBranchType( CShape_Link::eBRANCH_TYPE_ERROR );
	return pLink;
}
