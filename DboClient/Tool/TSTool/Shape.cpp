#include "StdAfx.h"
#include "Shape.h"
#include "TSDoc.h"
#include "TSView.h"
#include "TSFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//
//	CShape
//
//////////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL( CShape, CObject, 1 )


CTSDoc* CShape::GetDoc( void )
{
	return CTSDoc::GetInstance();
}

CTSView* CShape::GetActiveView( void )
{
	return static_cast<CTSView*>( static_cast<CTSFrame*>(AfxGetApp()->m_pMainWnd)->GetActiveView() );
}

CShape::CShape( void )
{
	m_bDragging = false;
	m_bZombie = false;
}

HSHAPE CShape::GetHandleAt( const CPoint &point ) const
{
	int nHandleSize = GetDoc()->GetTheme().GetHandleSize();

	for ( int i = 0; i < GetHandlesCount(); ++i )
	{
		CRect rt( GetHandlePoint(i), CSize(0, 0) );
		rt.InflateRect( nHandleSize, nHandleSize );

		if ( rt.PtInRect( point ) )
		{
			return i;
		}
	}

	return -1;
}

HCURSOR CShape::GetHandleCursor( HSHAPE handle ) const
{
	return AfxGetApp()->LoadStandardCursor(IDC_ARROW);
}

CPoint CShape::GetHandlePoint( HSHAPE handle ) const
{
	ASSERT(false);
	return CPoint();
}

bool CShape::Intersects( const CPoint &point ) const
{
	CRect rt( point, CSize(0, 0) );
	rt.InflateRect( 1, 1 );
	return Intersects( rt );
}

void CShape::BeginDrag( const CPoint &point, HSHAPE handle /*= -1*/ )
{
	ASSERT( !m_bDragging );

	m_ptDragPoint	= point;
	m_hDragHandle	= handle;
	m_ptLastPoint	= point;
	m_bDragging		= true;
}

void CShape::Drag( const CPoint &point )
{
	ASSERT( m_bDragging );

	if ( -1 == m_hDragHandle )
	{
		CSize delta = point - m_ptLastPoint;
		m_ptOrigin += delta;
	}

	m_ptLastPoint = point;

	GetDoc()->OnShapeChange( this );
}

void CShape::EndDrag( void )
{
	ASSERT( m_bDragging );

	m_bDragging = false;
}

void CShape::RenderTracker( CDC* pDC ) const
{
	int nHandleSize = GetDoc()->GetTheme().GetHandleSize();

	for ( int i = 0; i < GetHandlesCount(); ++i )
	{
		CRect rt( GetHandlePoint( i ), CSize(0, 0) );
		rt.InflateRect( nHandleSize, nHandleSize );
		pDC->InvertRect( rt );
	}
}

HCURSOR CShape::OnMouseMove( const CPoint &point, UINT nFlags )
{
	return AfxGetApp()->LoadStandardCursor( IDC_ARROW );
}

void CShape::Serialize( CArchive &ar )
{
	CObject::Serialize( ar );

	if ( ar.IsStoring() )
	{
		ar << m_ptOrigin;
	}
	else
	{
		ar >> m_ptOrigin;
	}
}
