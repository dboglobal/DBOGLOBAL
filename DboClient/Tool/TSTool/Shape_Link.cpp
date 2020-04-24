#include "StdAfx.h"
#include "Shape_Link.h"
#include "TSDoc.h"
#include "TSView.h"
#include <math.h>
#include "EntityRegisterDiag.h"
#include "Attr_Link.h"
#include "OptionListBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//
//	계산 함수
//
//////////////////////////////////////////////////////////////////////////


void s_CalcSafeBox( const CPoint &p1, const CPoint &p2, int nWidth, CPoint* p )
{
	double dx = p1.x - p2.x;
	double dy = p1.y - p2.y;
	double diag = ::sqrt( dx * dx + dy * dy );

	int ox = 0;
	int oy = 0;

	if ( 0 != diag )
	{
		double rx = dx / diag;
		double ry = dy / diag;
		ox = (int)(nWidth * ry);
		oy = (int)(nWidth * rx);
	}

	p[0] = p1 + CSize(-ox, oy);
	p[1] = p1 + CSize(ox, -oy);
	p[2] = p2 + CSize(ox, -oy);
	p[3] = p2 + CSize(-ox, oy);
}


void s_Transform( CPoint p[], int nCount, const CPoint& A, const CPoint& B, const CRect& R )
{
	double dx = B.x - A.x;
	double dy = B.y - A.y;
	double dLength = ::sqrt( dx * dx + dy * dy );

	if ( 0 == dLength )
	{
		for ( int i = 0; i < nCount; ++i ) p[i] = CPoint( 0, 0 );
		return;
	}

	double rx = dx / dLength;
	double ry = dy / dLength;

	double ox = dx > 0 ? R.right - A.x : R.left - A.x;
	double oy = ox * (dy / dx);

	if ( A.y + oy > R.bottom || A.y + oy < R.top )
	{
		oy = dy > 0 ? R.bottom - A.y : R.top - A.y;
		ox = oy * (dx / dy);
	}

	CPoint O( (int)(A.x + ox), (int)(A.y + oy) );

	for ( int i = 0; i < nCount; ++i )
	{
		double ax = p[i].x;
		double ay = p[i].y;
		p[i].x = O.x + (int)(rx * ax - ry * ay);
		p[i].y = O.y + (int)(ry * ax + rx * ay);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//	CShape_Link
//
//////////////////////////////////////////////////////////////////////////


IMPLEMENT_SERIAL( CShape_Link, CShape, 1 )


void CShape_Link::CAnchor::UpdatePoint( void )
{
	ASSERT( dRx >= 0 && dRx <= 1 );
	ASSERT( dRy >= 0 && dRy <= 1 );

	CRect rt = pShape->GetBoundBox();

	m_point.x = (int)( rt.left + rt.Width() * dRx );
	m_point.y = (int)( rt.top + rt.Height() * dRy );
}


void CShape_Link::CAnchor::MovePoint( const CPoint &point )
{
	dRx = (point.x - pShape->GetBoundBox().left) / (double)pShape->GetBoundBox().Width();
	dRy = (point.y - pShape->GetBoundBox().top) / (double)pShape->GetBoundBox().Height();

	if ( dRx < 0 )	dRx = 0;
	if ( dRx > 1 )	dRx = 1;
	if ( dRy < 0 )	dRy = 0;
	if ( dRy > 1 )	dRy = 1;

	UpdatePoint();
}

CShape_Link::CShape_Link( const CPoint &point, CShape* pShape )
{
	m_ptOrigin = point;
	m_ptTempEnd = point;

	m_bCreated = false;

	m_SrcAnchor.pShape = pShape;
	m_SrcAnchor.MovePoint( point );

	m_DestAnchor.pShape = NULL;

	m_eBranchType = eBRANCH_TYPE_DEFAULT;

	m_bShowBranchName = true;
	m_strBranchName = _T("");
	m_BranchNameOffset = CSize( 0, 10 );

	m_nPenStyle = PS_SOLID;

	SetupTheme();
}

int CShape_Link::GetHandlesCount( void ) const
{
	return 2 + (int)m_NodeList.GetSize();
}

CPoint CShape_Link::GetHandlePoint( HSHAPE handle ) const
{
	ASSERT( handle >= 0 && handle < GetHandlesCount() );

	switch ( handle )
	{
	case 0: return m_SrcAnchor.GetPoint();
	case 1: return m_DestAnchor.GetPoint();
	default: return m_NodeList[handle - 2];
	}

	return CPoint();
}

HSHAPE CShape_Link::GetHandleAt( const CPoint &point ) const
{
	HSHAPE handle = CShape::GetHandleAt( point );
	return handle != -1 ? handle : (CShape::Intersects( point ) ? NEW_NODE : -1);
}

bool CShape_Link::Intersects( const CRect &rect ) const
{
	if ( m_bShowBranchName && GetBranchNameRect().PtInRect( rect.TopLeft() ) )
	{
		return true;
	}

	for ( int i = 0; i <= m_NodeList.GetSize(); ++i )
	{
		if ( IntersectsSeg( rect, i ) )
		{
			return true;
		}
	}

	return false;
}

void CShape_Link::Activate( const CPoint &point )
{
	if ( m_bShowBranchName && GetBranchNameRect().PtInRect(point) )
	{
		if ( GetAsyncKeyState( VK_LCONTROL ) & 0x8000 )
		{
			EditBranchName();
		}
		else
		{
			CString strData;
			strData.Format( _T("%d"), m_eBranchType );

			CAttr_Link AttrPage;
			AttrPage.AddAttr( _T("lnktype"), strData );
			AttrPage.AddAttr( _T("branchname"), m_strBranchName );

			COptionListBox clListBox;
			CEntityRegisterDiag clDiag;
			clDiag.SetListControl( &clListBox );

			clDiag.AddAttrPage( &AttrPage );

			if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
			{
				CString strValue;

				strValue = AttrPage.GetAttr( _T("lnktype") );
				if ( strValue.GetLength() != 0 )
				{
					m_eBranchType = (eBRANCH_TYPE)atoi( strValue.GetString() );
				}

				m_strBranchName = AttrPage.GetAttr( _T("branchname") );

				GetDoc()->OnShapeChange( this );
			}
		}
	}
}

void CShape_Link::Update( void )
{
	if ( m_bCreated )
	{
		CPoint ptOldSrcPoint = m_SrcAnchor.GetPoint();
		CPoint ptOldDestPoint = m_DestAnchor.GetPoint();

		m_SrcAnchor.UpdatePoint();
		m_DestAnchor.UpdatePoint();

		CSize srcDelta = m_SrcAnchor.GetPoint() - ptOldSrcPoint;
		CSize destDelta = m_DestAnchor.GetPoint() - ptOldDestPoint;

		int i = 0;
		for( ; i < m_NodeList.GetSize() / 2; ++i )	m_NodeList[i] += srcDelta;
		for( ; i < m_NodeList.GetSize(); ++i )		m_NodeList[i] += destDelta;

		if ( m_SrcAnchor.pShape->IsZombie() || m_DestAnchor.pShape->IsZombie() )
		{
			MakeZombie();
		}
	}
}

void CShape_Link::BeginDrag( const CPoint &point, HSHAPE handle /*= -1*/ )
{
	CShape::BeginDrag( point, handle );

	if ( NEW_NODE == handle )
	{
		int i;
		CRect rt( point, CSize(0, 0) );
		rt.InflateRect( 1, 1 );

		if ( m_bShowBranchName && GetBranchNameRect().PtInRect(point) )
		{
			m_hDragHandle = HANDLE_DRAG_TEXT;
			return;
		}

		for ( i = 0; i <= m_NodeList.GetSize(); ++i )
		{
			if ( IntersectsSeg( rt, i ) )
			{
				break;
			}
		}

		m_hDragHandle = BreakSeg( i, point );
	}
}

void CShape_Link::Drag( const CPoint& point )
{
	if ( HANDLE_DRAG_TEXT == m_hDragHandle )
	{
		CSize delta = point - m_ptLastPoint;
		m_BranchNameOffset += delta;
		m_ptLastPoint = point;

		GetDoc()->OnShapeChange( this );
		return;
	}

	if ( 0 == m_hDragHandle )		m_SrcAnchor.MovePoint( point );
	else if ( 1 == m_hDragHandle )	m_DestAnchor.MovePoint( point );
	else if ( 1 < m_hDragHandle )	m_NodeList[m_hDragHandle - 2] = point;

	CShape::Drag( point );
}

void CShape_Link::EndDrag( void )
{
	CShape::EndDrag();

	if ( OptimizeSeg() )
	{
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_Link::Render( CDC* pDC ) const
{
	CShape::Render( pDC );

	if ( m_bCreated )
	{
		CPen pen( m_nPenStyle, 0, m_LineColor ), *pOldPen;
		pOldPen = pDC->SelectObject( &pen );

		// Shape
		pDC->MoveTo( m_SrcAnchor.GetPoint() );
		for ( int i = 0; i < m_NodeList.GetSize(); ++i )
		{
			pDC->LineTo( m_NodeList[i] );
		}
		pDC->LineTo( m_DestAnchor.GetPoint() );

		// Arrow
		CPoint p[] = { CPoint(16, 8), CPoint(0, 0), CPoint(16, -8), };

		{
			DestTransform( p, 3 );

			CPen pen( PS_SOLID, 0, m_LineColor ), *pOldPen;
			pOldPen = pDC->SelectObject( &pen );
			pDC->Polyline( p, 3 );
			pDC->SelectObject( pOldPen );
		}

		// BranchName
		if ( m_bShowBranchName )
		{
			CString strBranchInfo;
			switch ( m_eBranchType )
			{
			case eBRANCH_TYPE_DEFAULT:
				strBranchInfo = _T("[DEFAULT] ");
				strBranchInfo += m_strBranchName;
				break;
			case eBRANCH_TYPE_YES:
				strBranchInfo = _T("[YES] ");
				break;
			case eBRANCH_TYPE_NO:
				strBranchInfo = _T("[NO] ");
				break;
			case eBRANCH_TYPE_ERROR:
				strBranchInfo = _T("[ERROR] ");
				break;
			case eBRANCH_TYPE_LOGIN:
				strBranchInfo = _T("[LogIn] ");
				break;
			}

			CFont *pOldFont = pDC->SelectObject( m_pBranchNameFont );
			pDC->SetTextColor( m_LineColor );
			pDC->SetBkMode( TRANSPARENT );
			pDC->DrawText( strBranchInfo, GetBranchNameRect(), DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX );
			pDC->SelectObject( pOldFont );
		}

		pDC->SelectObject( pOldPen );
	}
}

void CShape_Link::RenderTracker( CDC* pDC ) const
{
	int nHandleSize = GetDoc()->GetTheme().GetHandleSize();

	CPen pen( PS_DOT, 0, m_LineColor ), *pOldPen;
	CPen handlePen( PS_SOLID, 0, RGB(100, 100, 255) );
	CBrush* pOldBrush;

	pOldPen = pDC->SelectObject( &pen );
	pOldBrush = (CBrush*)pDC->SelectStockObject( NULL_BRUSH );
	pDC->SetBkMode( TRANSPARENT );

	if ( m_bCreated )
	{
		int i;
		CRect rt;

		pDC->MoveTo( m_SrcAnchor.GetPoint() );
		for ( i = 0; i < m_NodeList.GetSize(); ++i )
		{
			pDC->LineTo( m_NodeList[i] );
		}
		pDC->LineTo( m_DestAnchor.GetPoint() );

		pDC->SelectObject( &handlePen );

		rt = CRect( m_SrcAnchor.GetPoint(), CSize(0, 0) );
		rt.InflateRect( nHandleSize, nHandleSize );
		pDC->Rectangle( rt );

		rt = CRect( m_DestAnchor.GetPoint(), CSize(0, 0) );
		rt.InflateRect( nHandleSize, nHandleSize );
		pDC->Rectangle( rt );

		for ( i = 0; i < m_NodeList.GetSize(); ++i )
		{
			rt = CRect( m_NodeList[i], CSize(0, 0) );
			rt.InflateRect( nHandleSize, nHandleSize );
			pDC->Ellipse( rt );
		}

		if ( m_bShowBranchName )
		{
			CRect rtSelBox = GetBranchNameRect();
			CBrush rectBrush( RGB(100, 100, 255) ), *pOldBrush;
			pOldBrush = pDC->SelectObject( &rectBrush );
			pDC->FrameRect( rtSelBox, &rectBrush );
			pDC->SelectObject( pOldBrush );
		}

	}
	else
	{
		pDC->MoveTo( m_SrcAnchor.GetPoint() );
		pDC->LineTo( m_ptTempEnd );
	}

	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );
}

void CShape_Link::Serialize( CArchive& ar )
{
	CShape::Serialize( ar );

	if ( ar.IsStoring() )
	{
		ar << m_SrcAnchor.pShape;
		ar << m_SrcAnchor.dRx;
		ar << m_SrcAnchor.dRy;

		ar << m_DestAnchor.pShape;
		ar << m_DestAnchor.dRx;
		ar << m_DestAnchor.dRy;

		ar << m_strBranchName;
		ar << m_BranchNameOffset;
		ar << (int)m_bShowBranchName;

		ar << m_eBranchType;
	}
	else
	{
		int nFlag;

		ar >> m_SrcAnchor.pShape;
		ar >> m_SrcAnchor.dRx;
		ar >> m_SrcAnchor.dRy;

		ar >> m_DestAnchor.pShape;
		ar >> m_DestAnchor.dRx;
		ar >> m_DestAnchor.dRy;

		ar >> m_strBranchName;
		ar >> m_BranchNameOffset;
		ar >> nFlag; m_bShowBranchName = nFlag ? true : false;

		ar >> nFlag;
		m_eBranchType = (eBRANCH_TYPE)nFlag;

		m_SrcAnchor.UpdatePoint();
		m_DestAnchor.UpdatePoint();
		m_bCreated = true;
	}

	m_NodeList.Serialize( ar );
	m_SegTypeList.Serialize( ar );

	SetupTheme();
}

void CShape_Link::DragTemp( const CPoint& point )
{
	ASSERT( !m_bCreated );
	m_ptTempEnd = point;
}

void CShape_Link::Finish( const CPoint& point, CShape* pShape )
{
	ASSERT( !m_bCreated );

	m_DestAnchor.pShape = pShape;
	m_DestAnchor.MovePoint( point );
	m_bCreated = true;

	if ( m_DestAnchor.pShape == m_SrcAnchor.pShape )
	{
		BreakSeg( 0, CPoint(m_SrcAnchor.GetPoint().x - 30, m_SrcAnchor.pShape->GetBoundBox().top - 40) );
		BreakSeg( 1, CPoint(m_SrcAnchor.GetPoint().x + 0, m_SrcAnchor.pShape->GetBoundBox().top - 40) );
	}

	GetDoc()->OnShapeChange( this );
}

void CShape_Link::EditBranchName( void )
{
	GetActiveView()->BeginEditText( m_strBranchName, GetBranchNameRect(), m_pBranchNameFont, ES_CENTER );
}

void CShape_Link::SetupTheme( void )
{
	CShape::SetupTheme();

	switch ( m_eBranchType )
	{
	case eBRANCH_TYPE_DEFAULT:
		m_LineColor = GetDoc()->GetTheme().GetLineColor_Def();
		break;
	case eBRANCH_TYPE_YES:
		m_nPenStyle = PS_DASH;
		m_LineColor = GetDoc()->GetTheme().GetLineColor_Yes();
		break;
	case eBRANCH_TYPE_NO:
		m_nPenStyle = PS_DASH;
		m_LineColor = GetDoc()->GetTheme().GetLineColor_No();
		break;
	case eBRANCH_TYPE_ERROR:
		m_nPenStyle = PS_DOT;
		m_LineColor = GetDoc()->GetTheme().GetLineColor_Error();
		break;
	case eBRANCH_TYPE_LOGIN:
		m_nPenStyle = PS_DOT;
		m_LineColor = GetDoc()->GetTheme().GetLineColor_LogIn();
		break;
	default:
		m_LineColor = GetDoc()->GetTheme().GetLineColor();
		break;
	}

	m_pBranchNameFont = GetDoc()->GetTheme().GetBranchNameFont();
}

double CShape_Link::GetSegLength( int nIndex ) const
{
	ASSERT( nIndex >= 0 && nIndex <= m_NodeList.GetSize() );

	CPoint p1 = nIndex == 0 ? m_SrcAnchor.GetPoint() : m_NodeList[nIndex - 1];
	CPoint p2 = nIndex == m_NodeList.GetSize() ? m_DestAnchor.GetPoint() : m_NodeList[nIndex];
	double dx = p1.x - p2.x;
	double dy = p1.y - p2.y;

	return ::sqrt( dx * dx + dy * dy );
}

double CShape_Link::GetSegAngle( int nIndex ) const
{
	ASSERT(nIndex >= 0 && nIndex <= m_NodeList.GetSize());

	CPoint p1 = nIndex == 0 ? m_SrcAnchor.GetPoint() : m_NodeList[nIndex - 1];
	CPoint p2 = nIndex == m_NodeList.GetSize() ? m_DestAnchor.GetPoint() : m_NodeList[nIndex];
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	double length = ::sqrt( dx * dx + dy * dy );

	if ( 0 == length ) return 0;

	return dy > 0 ? ::acos(dx / length) : 2 * 3.141 - ::acos(dx / length);
}

bool CShape_Link::IntersectsSeg( const CRect &rect, int nIndex ) const
{
	if ( NULL == m_DestAnchor.pShape ) return false;

	int nNumNodes = (int)m_NodeList.GetSize();
	ASSERT( nIndex >= 0 && nIndex <= nNumNodes );

	int nnHandleSize = GetDoc()->GetTheme().GetHandleSize();

	CRgn rgn;
	CPoint p[4];
	CPoint p1 = nIndex == 0 ? m_SrcAnchor.GetPoint() : m_NodeList[nIndex - 1];
	CPoint p2 = nIndex == nNumNodes ? m_DestAnchor.GetPoint() : m_NodeList[nIndex];

	s_CalcSafeBox( p1, p2, nnHandleSize, p );
	rgn.CreatePolygonRgn( p, 4, ALTERNATE );
	return rgn.RectInRegion( rect ) ? true : false;
}

int CShape_Link::BreakSeg( int nIndex, const CPoint &point )
{
	m_NodeList.InsertAt( nIndex, point );
	return nIndex + 2;
}

void CShape_Link::RemoveSeg( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex <= m_NodeList.GetSize() );

	if ( nIndex == m_NodeList.GetSize() )
	{
		m_NodeList.RemoveAt( nIndex - 1 );
	}
	else
	{
		m_NodeList.RemoveAt( nIndex );
	}
}

bool CShape_Link::OptimizeSeg( void )
{
	int nMinLength = GetDoc()->GetTheme().GetGridX();
	double dMinAngle = 0.12;
	int i;

	if ( m_NodeList.GetSize() == 0 ) return false;

	for ( i = 0; i <= m_NodeList.GetSize(); ++i )
	{
		if ( GetSegLength(i) < nMinLength )
		{
			RemoveSeg(i);
		}
	}

	for ( i = 0; i < m_NodeList.GetSize(); ++i )
	{
		double relAngle = ::fabs( GetSegAngle(i) - GetSegAngle(i + 1) );
		relAngle = relAngle > 3.14 ? (3.14 * 2 - relAngle) : relAngle;

		if ( relAngle < dMinAngle )
		{
			RemoveSeg( i );
			--i;
		}
	}

	return true;
}

void CShape_Link::SrcTransform( CPoint p[], int nCount ) const
{
	ASSERT( m_bCreated );

	int nIndex = 0;
	CPoint A = m_SrcAnchor.GetPoint();
	CPoint B = nIndex == m_NodeList.GetSize() ? m_DestAnchor.GetPoint() : m_NodeList[nIndex];
	CRect R = m_SrcAnchor.pShape->GetBoundBox();

	s_Transform( p, nCount, A, B, R );
}

void CShape_Link::DestTransform( CPoint p[], int nCount ) const
{
	ASSERT( m_bCreated );

	int nIndex = (int)m_NodeList.GetSize();
	CPoint B = nIndex == 0 ? m_SrcAnchor.GetPoint() : m_NodeList[nIndex - 1];
	CPoint A = m_DestAnchor.GetPoint();
	CRect R = m_DestAnchor.pShape->GetBoundBox();

	s_Transform( p, nCount, A, B, R );
}

CPoint CShape_Link::GetCenterPoint( void ) const
{
	ASSERT( m_bCreated );

	if ( m_NodeList.GetSize() % 2 == 0 )
	{
		int nIndex = (int)m_NodeList.GetSize() / 2;
		CPoint p1 = nIndex == 0 ? m_SrcAnchor.GetPoint() : m_NodeList[nIndex - 1];
		CPoint p2 = nIndex == m_NodeList.GetSize() ? m_DestAnchor.GetPoint() : m_NodeList[nIndex];
		return CPoint( (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 );
	}
	else
	{
		return m_NodeList[m_NodeList.GetSize() / 2];
	}
}

CRect CShape_Link::GetBranchNameRect( void ) const
{
	ASSERT( m_bCreated );

	CRect rt( GetCenterPoint() + m_BranchNameOffset, CSize(0, 0) );
	rt.InflateRect( 100, 8 );
	return rt;
}

