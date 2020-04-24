#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Link.h"
#include "TSDoc.h"
#include "TSView.h"
#include "Shape.h"
#include "Shape_Link.h"


/** 
	CSCtrl_Link
*/


CSCtrl_Link::CSCtrl_Link( CTSView* pView )
: CSCtrl( pView ),
  m_eType( eTYPE_DIAGONAL ),
  m_pShapeLink( NULL )
{
}

void CSCtrl_Link::OnLButtonDown( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnLButtonDown( point, nFlags );

	ASSERT( NULL == m_pShapeLink );
	CShape* pShape = GetDoc()->GetShapeAt( point );
	if ( NULL == pShape ) return;

	m_pShapeLink = New( point, pShape );
	GetDoc()->AddLinkShape( m_pShapeLink );

	m_pView->SelectShape( m_pShapeLink );
}

void CSCtrl_Link::OnLButtonUp( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnLButtonUp( point, nFlags );

	if ( m_pShapeLink )
	{
		CShape* pShape = GetDoc()->GetShapeAt( point );

		if ( NULL == pShape )
		{
			GetDoc()->RemoveShape( m_pShapeLink );
			m_pShapeLink = NULL;
			return;
		}

		m_pShapeLink->Finish( point, pShape );

		m_pShapeLink->BeginDrag( m_pShapeLink->GetHandlePoint( 0 ), 0 );
		m_pShapeLink->Drag( m_pView->Align2Grid( m_pShapeLink->GetHandlePoint( 0 ) ) );
		m_pShapeLink->EndDrag();

		m_pShapeLink->BeginDrag( m_pShapeLink->GetHandlePoint( 1 ), 1 );
		m_pShapeLink->Drag( m_pView->Align2Grid( m_pShapeLink->GetHandlePoint( 1 ) ) );
		m_pShapeLink->EndDrag();

		m_pShapeLink = NULL;
	}
}

void CSCtrl_Link::OnMouseMove( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnMouseMove( point, nFlags );

	if ( m_pShapeLink )
	{
		m_pShapeLink->DragTemp( point );
		m_pView->Invalidate();
	}
}

void CSCtrl_Link::OnDoubleClick( const CPoint &point, UINT nFlags )
{
	CSCtrl::OnDoubleClick( point, nFlags );
}

void CSCtrl_Link::OnContextMenu( const CPoint &point )
{
	CSCtrl::OnContextMenu( point );
}

void CSCtrl_Link::OnDeactivate( void )
{
	CSCtrl::OnDeactivate();

	if ( m_pShapeLink )
	{
		GetDoc()->RemoveShape( m_pShapeLink );
		m_pShapeLink = NULL;
	}
}

CShape_Link* CSCtrl_Link::New( const CPoint& point, CShape* pShape )
{
	return new CShape_Link( point, pShape );
}
