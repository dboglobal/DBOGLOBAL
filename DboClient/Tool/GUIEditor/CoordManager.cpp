#include "StdAfx.h"
#include ".\coordmanager.h"

#define MOVEFACTOR 64

CCoordManager::CCoordManager(void)
{
	InitMember();
}

CCoordManager::~CCoordManager(void)
{
	Destroy();
}

void CCoordManager::InitMember()
{
	m_rcImage  = CRect( 0, 0, 0, 0 );
	m_rcView   = CRect( 0, 0, 0, 0 );
	m_rcImage  = CRect( 0, 0, 0, 0 );
	m_ptOffset = CPoint( 0, 0 );
	m_rcClient = CRect( 0, 0, 0, 0 );
	
	m_nMargin = 0;
	m_nMouseDelta = 0;

	m_fScaleFactor = 1.0f;
	m_fFixXScaleFactor = 1.0f;
	m_fFixYScaleFactor = 1.0f;

	m_pView = NULL;
}

void CCoordManager::Reset()
{
	m_rcImage  = CRect( 0, 0, 0, 0 );
	m_ptOffset = CPoint( 0, 0 );
	m_nMouseDelta = 0;
	m_fScaleFactor = 1.0f;
	m_fFixXScaleFactor = 1.0f;
	m_fFixYScaleFactor = 1.0f;
}

void CCoordManager::Create( CScrollView* pView, INT nMargin )
{
	m_pView	  = pView;
	m_nMargin = nMargin;
}

void CCoordManager::SetMouseDelta( INT nDelta )
{
	m_nMouseDelta = m_nMouseDelta + nDelta;

	// Delta 한계
	if( m_nMouseDelta > 1200 )
		m_nMouseDelta = 1200;
	else if( m_nMouseDelta < -360 )
		m_nMouseDelta = -360;

	FLOAT fTemp = (FLOAT)( m_nMouseDelta / 120 );

	if( m_nMouseDelta == 0 )
		m_fScaleFactor = 1.0f;	
	else if( m_nMouseDelta > 0 )
		m_fScaleFactor = fTemp + 1;
	else if( m_nMouseDelta < 0 )
		m_fScaleFactor = 1 / -fTemp;	

	SetFixScaleFactor();
}

CPoint CCoordManager::FixMousePt( CPoint& point )
{
	CPoint ptResult( 0, 0 );
	
	ptResult = point;

	// 여백 공간 적용.
	ptResult.x -= m_nMargin;
	ptResult.y -= m_nMargin;

	// 여백내의 공간으로 수정.
	if( point.x < m_nMargin ) 
		ptResult.x = 0;
	if( point.y < m_nMargin ) 
		ptResult.y = 0;
    if( point.x > m_rcClient.right - m_nMargin + 1 ) 
		ptResult.x = m_rcClient.right - 2 * m_nMargin + 1;
	if( point.y > m_rcClient.bottom - m_nMargin + 1 ) 
		ptResult.y = m_rcClient.bottom - 2 * m_nMargin + 1;

	// ScaleFactor 적용. Offset 적용.
    ptResult.x = (INT)( ptResult.x / m_fFixXScaleFactor ) + m_ptOffset.x;
	ptResult.y = (INT)( ptResult.y / m_fFixYScaleFactor ) + m_ptOffset.y;

	return ptResult;
}

CRect CCoordManager::GetDestRect()
{
	// Result Rect는 width(), height() 값이 right, bottom에 들어있다.
	CRect rcResult( 0, 0, 0, 0 );

	rcResult.left	= m_nMargin;
	rcResult.top	= m_nMargin;
	rcResult.right	= m_rcView.right;
	rcResult.bottom = m_rcView.bottom;

	return rcResult;
}

CRect CCoordManager::GetSrcRect()
{
	// Result Rect는 width(), height() 값이 right, bottom에 들어있다.
	CRect rcResult( 0, 0, 0, 0 );

	rcResult.left	= m_ptOffset.x;
	rcResult.top	= m_ptOffset.y;
	rcResult.right  = (INT)( m_rcView.right / m_fScaleFactor );
	rcResult.bottom = (INT)( m_rcView.bottom / m_fScaleFactor );

	return rcResult;
}

CCoordManager::MARGINRECT CCoordManager::PtInMargin( CPoint& point )
{
	/*if( point.x < 0 || point.y < 0 || point.x > m_rcClient.right || point.y > m_rcClient.bottom )
		return CENTER;*/
	
	// 왼쪽
	if( point.x < m_nMargin )
	{
		if( point.y < m_nMargin )
			return LEFTUP;
		else if( point.y > m_nMargin + m_rcView.bottom )
			return LEFTDOWN;
		else
			return LEFT;
	}
	// 오른쪽
	else if( point. x > m_nMargin + m_rcView.right )
	{
		if( point.y < m_nMargin )
			return RIGHTUP;
		else if( point.y > m_nMargin + m_rcView.bottom )
			return RIGHTDOWN;
		else
			return RIGHT;
	}
	// 중앙
	else
	{
		if( point.y < m_nMargin )
			return UP;
		else if( point.y > m_nMargin + m_rcView.bottom )
			return DOWN;
		else 
			return CENTER;
	}
	
	//// 왼쪽
	//if( point.x < 0 )
	//{
	//	if( point.y < 0 )
	//		return LEFTUP;
	//	else if( point.y > m_rcClient.right )
	//		return LEFTDOWN;
	//	else
	//		return LEFT;
	//}
	//// 오른쪽
	//else if( point. x > m_rcClient.right )
	//{
	//	if( point.y < 0 )
	//		return RIGHTUP;
	//	else if( point.y > 0 + m_rcClient.bottom )
	//		return RIGHTDOWN;
	//	else
	//		return RIGHT;
	//}
	//// 중앙
	//else
	//{
	//	if( point.y < 0 )
	//		return UP;
	//	else if( point.y > 0 + m_rcClient.bottom )
	//		return DOWN;
	//	else 
	//		return CENTER;
	//}
}

void CCoordManager::MarginMove( MARGINRECT eMarginRect )
{
	if( !IsScrollNeed() )
		return;

	INT nFactor = (INT)( MOVEFACTOR / m_fScaleFactor );
	
	switch( eMarginRect )
	{
	case CENTER:						 return;
	case UP:	m_ptOffset.y -= nFactor; break;
	case DOWN:	m_ptOffset.y += nFactor; break;
	case LEFT:	m_ptOffset.x -= nFactor; break;
	case RIGHT: m_ptOffset.x += nFactor; break;
	case LEFTUP:	m_ptOffset.x -= nFactor; m_ptOffset.y -= nFactor; break;
	case LEFTDOWN:	m_ptOffset.x -= nFactor; m_ptOffset.y += nFactor; break;
	case RIGHTUP:	m_ptOffset.x += nFactor; m_ptOffset.y -= nFactor; break;
	case RIGHTDOWN: m_ptOffset.x += nFactor; m_ptOffset.y += nFactor; break;
	}

	FixOffset();
	//char buf[256];
	//sprintf( buf, "MoveoffsetX : %d, MoveoffsetY : %d\n", m_ptOffset.x, m_ptOffset.y );
	//OutputDebugString( buf );
}

BOOL CCoordManager::IsScrollNeed()
{	
	if( IsHScrollNeed() || IsVScrollNeed() )
		return TRUE;

    return FALSE;
}

BOOL CCoordManager::IsHScrollNeed()
{	
	if( (INT)( m_rcImage.Width() * m_fFixXScaleFactor ) > m_rcView.Width() )		
		return TRUE;

	return FALSE;
}

BOOL CCoordManager::IsVScrollNeed()
{	
	if( (INT)( m_rcImage.Height() * m_fFixYScaleFactor ) > m_rcView.Height() )
		return TRUE;

	return FALSE;
}

void CCoordManager::FixOffset()
{
	// offset 보정.
	if( m_ptOffset.x < 0 ) m_ptOffset.x = 0;
	if( m_ptOffset.y < 0 ) m_ptOffset.y = 0;
	if( IsHScrollNeed() )
	{
		if( m_ptOffset.x > (INT)( ( ( m_rcImage.right * m_fFixXScaleFactor ) - m_rcView.right ) / m_fFixXScaleFactor ) )
			m_ptOffset.x = (INT)( ( ( m_rcImage.right * m_fFixXScaleFactor ) - m_rcView.right ) / m_fFixXScaleFactor );
	}
	if( IsVScrollNeed() )
	{
		if( m_ptOffset.y > (INT)( ( ( m_rcImage.bottom * m_fFixYScaleFactor ) - m_rcView.bottom ) / m_fFixYScaleFactor ) )
			m_ptOffset.y = (INT)( ( ( m_rcImage.bottom * m_fFixYScaleFactor ) - m_rcView.bottom ) / m_fFixYScaleFactor );
	}
	
}

void CCoordManager::SetPtToCenter( CPoint& point )
{
	CPoint FixPoint, NewOffset;
	FixPoint = FixMousePt( point );
	NewOffset = CPoint( 0, 0 );
	if( IsHScrollNeed() )
		NewOffset.x = FixPoint.x - (INT)( ( m_rcView.right / ( 2 * m_fFixXScaleFactor ) ) );
	if( IsVScrollNeed() )
		NewOffset.y = FixPoint.y - (INT)( ( m_rcView.bottom / ( 2 * m_fFixYScaleFactor ) ) );

	m_ptOffset = NewOffset;

	FixOffset();
}

void CCoordManager::SetOffset( CPoint& point )
{
	m_ptOffset = point;

	FixOffset();
}

void CCoordManager::MoveOffset( CPoint& point )
{
	if( IsHScrollNeed() )
		m_ptOffset.x += point.x;
	if( IsVScrollNeed() )
		m_ptOffset.y += point.y;

	FixOffset();
}

void CCoordManager::SetClientRect( INT cx, INT cy )
{
	m_rcClient = CRect( 0, 0, cx, cy );
	SetViewRect();	
}

void CCoordManager::SetViewRect()
{
	m_rcView   = CRect( 0, 0, m_rcClient.right - 2 * m_nMargin, m_rcClient.bottom - 2 * m_nMargin );
	SetFixScaleFactor();
}

void CCoordManager::SetMargin( INT nMargin )
{
	m_nMargin = nMargin;
	SetViewRect();
}

void CCoordManager::SetFixScaleFactor()
{
	// 소숫점 6자리 이하에서 내림 현상이 일어나므로 임의 올림. 
	m_fFixXScaleFactor = ( (FLOAT)m_rcView.right / (INT)( m_rcView.right / m_fScaleFactor ) ) + 0.000001f; 
	m_fFixYScaleFactor = ( (FLOAT)m_rcView.bottom / (INT)( m_rcView.bottom / m_fScaleFactor ) ) + 0.000001f;
	FixOffset();
}
