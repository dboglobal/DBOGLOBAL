#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Link_Def.h"
#include "Shape_Link.h"
#include "TSDoc.h"
#include "Shape_GAct.h"
#include "Shape_Reward.h"
#include "Shape_UsrSel.h"
#include "Shape_Switch.h"
#include "Shape_Note.h"


/** 
	CSCtrl_Link_Def
*/


CSCtrl_Link_Def::CSCtrl_Link_Def( CTSView* pView )
: CSCtrl_Link( pView )
{
}

void CSCtrl_Link_Def::OnLButtonDown( const CPoint &point, UINT nFlags )
{
	CShape* pShape = GetDoc()->GetShapeAt( point );
	if ( NULL == pShape ) return;

	if ( NULL != DYNAMIC_DOWNCAST( CShape_GAct, pShape ) ||
		 NULL != DYNAMIC_DOWNCAST( CShape_Reward, pShape ) ||
		 NULL != DYNAMIC_DOWNCAST( CShape_UsrSel, pShape ) ||
		 NULL != DYNAMIC_DOWNCAST( CShape_Switch, pShape ) ||
		 NULL != DYNAMIC_DOWNCAST( CShape_Note, pShape ) )
	{
		CSCtrl_Link::OnLButtonDown( point, nFlags );
	}
	else
	{
		AfxMessageBox( _T("해당 컨테이너에 Defaule 링크는 놓을 수 없습니다") );
	}
}

CShape_Link* CSCtrl_Link_Def::New( const CPoint& point, CShape* pShape )
{
	CShape_Link* pLink = CSCtrl_Link::New( point, pShape );
	pLink->SetBranchType( CShape_Link::eBRANCH_TYPE_DEFAULT );
	return pLink;
}
