#include "stdafx.h"
#include "TSTool.h"
#include "SCtrl_Select.h"
#include "TSDoc.h"
#include "TSView.h"
#include "Shape.h"


/** 
	CSCtrl_Select
*/


CSCtrl_Select::CSCtrl_Select( CTSView* pView )
: CSCtrl( pView ),
  m_eState( eSTATE_NONE ),
  m_pShape( NULL ),
  m_bLock( false )
{
}

void CSCtrl_Select::OnLButtonDown( const CPoint &point, UINT nFlags )
{
	if ( m_pView->GetSelectionList().GetCount() == 1 &&
		 m_pView->GetSelectionList().GetHead()->GetHandleAt( point ) != -1 )
	{
		m_pShape = m_pView->GetSelectionList().GetHead();
		m_pShape->BeginDrag( point, m_pShape->GetHandleAt( point ) );
		m_eState = eSTATE_DRAGHANDLE;
	}
	else
	{
		CShape* pShape = GetDoc()->GetShapeAt( point );

		if ( pShape )
		{
			if ( nFlags & MK_CONTROL )
			{
				if ( m_pView->IsSelected( pShape ) ) m_pView->UnselectShape( pShape );
				else m_pView->SelectShape( pShape, true );
			}
			else
			{
				if ( !m_pView->IsSelected( pShape ) ) m_pView->SelectShape( pShape );

				POSITION pos = m_pView->GetSelectionList().GetHeadPosition();
				while ( pos )
				{
					m_pView->GetSelectionList().GetNext( pos )->BeginDrag( m_pView->Align2Grid( point ) );
				}

				m_eState = eSTATE_DRAG;
			}
		}
		else
		{
			if ( !(nFlags & MK_CONTROL) ) m_pView->ClearSelections();
			m_eState = eSTATE_NETSELECT;
		}
	}

	m_ptDown = point;
	m_ptLastDown = point;
}

void CSCtrl_Select::OnLButtonUp( const CPoint &point, UINT nFlags )
{
	if ( eSTATE_DRAGHANDLE == m_eState )
	{
		m_pShape->EndDrag();
	}
	else if ( eSTATE_DRAG == m_eState )
	{
		POSITION pos = m_pView->GetSelectionList().GetHeadPosition();
		while ( pos )
		{
			m_pView->GetSelectionList().GetNext( pos )->EndDrag();
		}
	}
	else if ( eSTATE_NETSELECT == m_eState )
	{
		CClientDC dc( m_pView );
		dc.DrawFocusRect( m_pView->Doc2Device( CRect(m_ptDown, m_ptLastDown) ) );
		m_pView->SelectShape( CRect(m_ptDown, point), true );
	}

	m_eState = eSTATE_NONE;
}

void CSCtrl_Select::OnMouseMove( const CPoint &point, UINT nFlags )
{
	CPoint clCalCurPos = point;

	if ( eSTATE_DRAGHANDLE == m_eState && !IsLocked() )
	{
		m_pShape->Drag( m_pView->Align2Grid( clCalCurPos ) );
	}
	else if ( eSTATE_DRAG == m_eState && !IsLocked() )
	{
		clCalCurPos = CalMouseMoveClipper( clCalCurPos );

		POSITION pos = m_pView->GetSelectionList().GetHeadPosition();

		while ( pos )
		{
			m_pView->GetSelectionList().GetNext(pos)->Drag( m_pView->Align2Grid( clCalCurPos ) );
		}
	}
	else if ( eSTATE_NETSELECT == m_eState )
	{
		CClientDC dc( m_pView );
		dc.DrawFocusRect( m_pView->Doc2Device( CRect( m_ptDown, m_ptLastDown ) ) );
		dc.DrawFocusRect( m_pView->Doc2Device( CRect( m_ptDown, clCalCurPos ) ) );
	}

	m_ptLastDown = clCalCurPos;
}

void CSCtrl_Select::OnDoubleClick( const CPoint &point, UINT nFlags )
{
	CShape* pShape = GetDoc()->GetShapeAt( point );
	if ( pShape )
	{
		pShape->Activate( point );
	}
}

void CSCtrl_Select::OnContextMenu( const CPoint &point )
{
}

void CSCtrl_Select::OnDeactivate( void )
{
	m_eState = eSTATE_NONE;
}

CPoint CSCtrl_Select::CalMouseMoveClipper( const CPoint& point )
{
	CPoint clCalPos = point;

	CRect clSumBoundBox;
	clSumBoundBox.left = 0x7fffffff;	clSumBoundBox.right = 0x80000000;
	clSumBoundBox.top = 0x7fffffff;		clSumBoundBox.bottom = 0x80000000;

	POSITION pos = m_pView->GetSelectionList().GetHeadPosition();

	while ( pos )
	{
		CRect clBoundBox = m_pView->GetSelectionList().GetNext( pos )->GetBoundBox();

		clSumBoundBox.left		= clSumBoundBox.left < clBoundBox.left ? clSumBoundBox.left : clBoundBox.left;
		clSumBoundBox.right		= clSumBoundBox.right > clBoundBox.right ? clSumBoundBox.right : clBoundBox.right;
		clSumBoundBox.top		= clSumBoundBox.top < clBoundBox.top ? clSumBoundBox.top : clBoundBox.top;
		clSumBoundBox.bottom	= clSumBoundBox.bottom > clBoundBox.bottom ? clSumBoundBox.bottom : clBoundBox.bottom;
	}

	CSize delta = clCalPos - m_ptLastDown;

	clSumBoundBox.OffsetRect( delta );

	if ( clSumBoundBox.left < 0 )
	{
		clCalPos.x -= clSumBoundBox.left;
	}

	if ( clSumBoundBox.top < 0 )
	{
		clCalPos.y -= clSumBoundBox.top;
	}

	if ( clSumBoundBox.right > GetDoc()->GetExtentX() )
	{
		clCalPos.x -= clSumBoundBox.right - GetDoc()->GetExtentX();
	}

	if ( clSumBoundBox.bottom > GetDoc()->GetExtentY() )
	{
		clCalPos.y -= clSumBoundBox.bottom - GetDoc()->GetExtentY();
	}

	return clCalPos;
}
