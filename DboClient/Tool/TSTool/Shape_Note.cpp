#include "StdAfx.h"
#include "Shape_Note.h"
#include "TSDoc.h"
#include "TSView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//
//	CShape_Note
//
//////////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL( CShape_Note, CShape, 1 )


CShape_Note::CShape_Note( const CPoint &point )
{
	m_ptOrigin = point;

	m_strNote = _T("New note");

	m_nWidth = 150;
	m_nHeight = 75;

	SetupTheme();
}

CRect CShape_Note::GetBoundBox( void ) const
{
	CRect rt( m_ptOrigin, CSize( m_nWidth, m_nHeight ) );
	return rt;
}

CPoint CShape_Note::GetHandlePoint( HSHAPE handle ) const
{
	ASSERT( handle >= 0 && handle < 4 );

	switch ( handle )
	{
	case 0: return CPoint( m_ptOrigin );
	case 1: return CPoint( m_ptOrigin + CSize( 0, m_nHeight ) );
	case 2: return CPoint( m_ptOrigin + CSize( m_nWidth, m_nHeight ) );
	case 3: return CPoint( m_ptOrigin + CSize( m_nWidth, 0 ) );
	}

	return CPoint();
}

bool CShape_Note::Intersects( const CRect &rect ) const
{
	CRgn rgn;

	CPoint p[5];
	p[0] = m_ptOrigin;
	p[1] = m_ptOrigin + CSize( 0, m_nHeight );
	p[2] = m_ptOrigin + CSize( m_nWidth, m_nHeight );
	p[3] = m_ptOrigin + CSize( m_nWidth, m_nCornerHeight );
	p[4] = m_ptOrigin + CSize( m_nWidth - m_nCornerWidth, 0 );

	rgn.CreatePolygonRgn( p, 5, WINDING );
	return rgn.RectInRegion( rect ) ? true : false;
}

void CShape_Note::Activate( const CPoint &point )
{
	if ( GetTextRect().PtInRect( point ) )
	{
		EditText();
	}
}

void CShape_Note::Drag( const CPoint &point )
{
	const int nMinWidth = m_nCornerWidth + 65;
	const int nMinHeight = m_nCornerHeight + 10;

	CPoint p;

	switch ( m_hDragHandle )
	{
	case 0:
		p = GetHandlePoint( 2 );
		m_nWidth = p.x - point.x;
		m_nHeight = p.y - point.y;
		if ( m_nWidth < nMinWidth ) m_nWidth = nMinWidth;
		if ( m_nHeight < nMinHeight ) m_nHeight = nMinHeight;
		m_ptOrigin = p - CSize(m_nWidth, m_nHeight);
		break;

	case 1:
		p = GetHandlePoint( 3 );
		m_nWidth = p.x - point.x;
		m_nHeight = point.y - p.y;
		if ( m_nWidth < nMinWidth ) m_nWidth = nMinWidth;
		if ( m_nHeight < nMinHeight ) m_nHeight = nMinHeight;
		m_ptOrigin = p - CSize(m_nWidth, 0);
		break;

	case 2:
		p = GetHandlePoint( 0 );
		m_nWidth = point.x - p.x;
		m_nHeight = point.y - p.y;
		if ( m_nWidth < nMinWidth ) m_nWidth = nMinWidth;
		if ( m_nHeight < nMinHeight ) m_nHeight = nMinHeight;
		break;

	case 3:
		p = GetHandlePoint( 1 );
		m_nWidth = point.x - p.x;
		m_nHeight = p.y - point.y;
		if ( m_nWidth < nMinWidth ) m_nWidth = nMinWidth;
		if ( m_nHeight < nMinHeight ) m_nHeight = nMinHeight;
		m_ptOrigin = p - CSize(0, m_nHeight);
		break;
	}

	CShape::Drag( point );
}

void CShape_Note::Render( CDC* pDC ) const
{
	CShape::Render( pDC );

	CPen pen( PS_SOLID, 0, m_LineColor );
	CPen* pOldPen;
	CBrush brush( m_FillColor );
	CBrush shadowBrush( m_ShadowColor );
	CBrush* pOldBrush;
	CFont* pOldFont;

	pOldPen = (CPen*)pDC->SelectStockObject( NULL_PEN );
	pOldBrush = pDC->SelectObject(&shadowBrush);
	pOldFont = pDC->SelectObject(m_pTextFont);

	// shadow
	CPoint sp[5];
	sp[0] = m_ptOrigin + CSize( 5, 5 );
	sp[1] = m_ptOrigin + CSize( 5, m_nHeight + 5 );
	sp[2] = m_ptOrigin + CSize( m_nWidth + 5, m_nHeight + 5 );
	sp[3] = m_ptOrigin + CSize( m_nWidth + 5, m_nCornerHeight + 5 );
	sp[4] = m_ptOrigin + CSize( m_nWidth - m_nCornerWidth + 5, 5 );
	pDC->Polygon( sp, 5 );

	// shape
	CPoint p[6];
	p[0] = m_ptOrigin;
	p[1] = m_ptOrigin + CSize( 0, m_nHeight );
	p[2] = m_ptOrigin + CSize( m_nWidth, m_nHeight );
	p[3] = m_ptOrigin + CSize( m_nWidth, m_nCornerHeight );
	p[4] = m_ptOrigin + CSize( m_nWidth - m_nCornerWidth, 0 );
	p[5] = m_ptOrigin + CSize( m_nWidth - m_nCornerWidth, m_nCornerHeight );
	pDC->SelectObject( &pen );
	pDC->SelectObject( &brush );
	pDC->Polygon( p, 5 );
	pDC->MoveTo( p[3] );
	pDC->LineTo( p[5] );
	pDC->LineTo( p[4] );

	// text
	pDC->SetTextColor( m_TextColor );
	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( m_strNote, GetTextRect(), DT_WORDBREAK | DT_NOPREFIX );

	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );
	pDC->SelectObject( pOldFont );
}

void CShape_Note::RenderTracker(CDC *pDC) const
{
	CPen pen( PS_DOT, 0, m_LineColor ), *pOldPen;
	pOldPen = pDC->SelectObject( &pen );
	pDC->SetBkMode( TRANSPARENT );

	CPoint p[6];
	p[0] = m_ptOrigin;
	p[1] = m_ptOrigin + CSize( 0, m_nHeight );
	p[2] = m_ptOrigin + CSize( m_nWidth, m_nHeight );
	p[3] = m_ptOrigin + CSize( m_nWidth, m_nCornerHeight );
	p[4] = m_ptOrigin + CSize( m_nWidth - m_nCornerWidth, 0 );
	p[5] = m_ptOrigin;
	pDC->Polyline( p, 6 );

	pDC->SelectObject( pOldPen );

	CShape::RenderTracker( pDC );
}

void CShape_Note::Serialize( CArchive &ar )
{
	CShape::Serialize( ar );

	if ( ar.IsStoring() )
	{
		ar << m_strNote;
		ar << m_nWidth;
		ar << m_nHeight;
	}
	else
	{
		ar >> m_strNote;
		ar >> m_nWidth;
		ar >> m_nHeight;
	}
}

void CShape_Note::SetupTheme( void )
{
	CShape::SetupTheme();

	m_nCornerWidth = GetDoc()->GetTheme().GetCornerSize();
	m_nCornerHeight = GetDoc()->GetTheme().GetCornerSize();

	m_pTextFont = GetDoc()->GetTheme().GetNoteFont();

	m_TextColor = GetDoc()->GetTheme().GetNoteTextColor();
	m_FillColor = GetDoc()->GetTheme().GetNoteBackgroundColor();
	m_LineColor = GetDoc()->GetTheme().GetLineColor();
	m_ShadowColor = GetDoc()->GetTheme().GetNoteShadowColor();
}

CRect CShape_Note::GetTextRect( void ) const
{
	CRect rt( m_ptOrigin, CSize(m_nWidth - m_nCornerWidth, m_nHeight) );
	rt.DeflateRect( 8, 4, 3, 3 );
	return rt;
}

void CShape_Note::EditText( void )
{
	GetActiveView()->BeginEditText( m_strNote, GetTextRect(), m_pTextFont, ES_MULTILINE );
}
