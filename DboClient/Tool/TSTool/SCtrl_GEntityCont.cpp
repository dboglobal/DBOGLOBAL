#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_GEntityCont.h"
#include "TSDoc.h"
#include "TSView.h"
#include "Shape.h"
#include "Shape_GEntityCont.h"


/** 
	CSCtrl_GEntityCont
*/


CSCtrl_GEntityCont::CSCtrl_GEntityCont( CTSView* pView )
: CSCtrl( pView ),
  m_pShapeGEntityCont( NULL )
{
}

void CSCtrl_GEntityCont::OnLButtonDown( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnLButtonDown( point, nFlags );

	ASSERT( NULL == m_pShapeGEntityCont );

	m_pShapeGEntityCont = New( m_pView->Align2Grid(point) );
	GetDoc()->AddContainerShape( m_pShapeGEntityCont );

	m_pView->SelectShape( m_pShapeGEntityCont );
}

void CSCtrl_GEntityCont::OnLButtonUp( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnLButtonUp( point, nFlags );

	if ( m_pShapeGEntityCont )
	{
		m_pShapeGEntityCont = NULL;
	}
}

void CSCtrl_GEntityCont::OnMouseMove( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnMouseMove( point, nFlags );
}

void CSCtrl_GEntityCont::OnDoubleClick( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnDoubleClick( point, nFlags );
}

void CSCtrl_GEntityCont::OnContextMenu( const CPoint &point )
{
	CSCtrl::OnContextMenu( point );
}

void CSCtrl_GEntityCont::OnDeactivate( void )
{
	CSCtrl::OnDeactivate();

	if ( m_pShapeGEntityCont )
	{
		m_pShapeGEntityCont = NULL;
	}
}

CShape_GEntityCont* CSCtrl_GEntityCont::New( const CPoint& point )
{
	ASSERT( !"Must be overriding" );
	return NULL;
}
