#include "gui_precomp.h"
#include "gui_Balloon_Generic.h"
#include "gui_DynamicOutline.h"

START_GUI

#define BALLOON_MIN_WIDTH 30

////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CBalloon_Generic::CBalloon_Generic( CBalloon* pSelf, BOOL bLeft )
: m_pBalloon( pSelf ), m_pOutline( NULL ), m_nMarginX( 0 ), m_nMarginY( 0 ), m_ptTailLT( 0, 0 )
{
	m_pOutline	 = NTL_NEW CDynamicOutline( pSelf, TRUE );

	m_SlotSetOption = m_pBalloon->SigSetOptions().Connect( this, &CBalloon_Generic::OnSetOption );
	m_SlotMove		= m_pBalloon->SigMove().Connect( this, &CBalloon_Generic::OnMove );
	m_SlotPaint		= m_pBalloon->SigPaint().Connect( this,  &CBalloon_Generic::OnPaint );
	m_SlotSetAlpha  = m_pBalloon->SigSetAlpha().Connect( this, &CBalloon_Generic::OnSetAlpha );
	m_SlotSetColor  = m_pBalloon->SigSetColor().Connect( this, &CBalloon_Generic::OnSetColor );

	SetOrigin( 0, 0, bLeft );
}

CBalloon_Generic::~CBalloon_Generic(VOID)
{
	NTL_DELETE( m_pOutline );
}

////////////////////////////////////////////////////////////////////////////////
// Operations

VOID CBalloon_Generic::SetOrigin( INT nScreenX, INT nScreenY, BOOL bLeft )
{
	if( bLeft )
	{
		m_ptTailLT.x = nScreenX;
		m_fTailFactor = 0.666666f;
	}
	else
	{
		m_ptTailLT.x = nScreenX - m_pOutline->GetTailWidth();
		m_fTailFactor = 0.333333f;
	}
	
	m_ptTailLT.y = nScreenY - m_pOutline->GetTailHeight();
}

VOID CBalloon_Generic::SetOriginNow( INT nScreenX, INT nScreenY, BOOL bLeft )
{
	INT nOriTailLT_X = m_ptTailLT.x;
	INT nOriTailLT_Y = m_ptTailLT.y;
	
	SetOrigin( nScreenX, nScreenY, bLeft );

	INT nDiffX = m_ptTailLT.x - nOriTailLT_X;
	INT nDiffY = m_ptTailLT.y - nOriTailLT_Y;

	CRectangle rect = m_pBalloon->GetScreenRect();
	m_pBalloon->SetPosition( rect.left + nDiffX, rect.top + nDiffY );
}

VOID CBalloon_Generic::CalcRect(VOID)
{
	INT		   nTailX;
	CRectangle rcInRect, rcOutRect, rcParent, rcTemp;

	if( m_pBalloon->m_pStaticBox->GetWidth() == 0 || m_pBalloon->m_pStaticBox->GetHeight() == 0 )
	{
		m_pBalloon->Show( FALSE );
		return;
	}
	else
		m_pBalloon->Show( TRUE );

	INT	nStaticWidth, nStaticHeight;
	nStaticWidth = m_pBalloon->m_pStaticBox->GetWidth() + 2 * m_nMarginX;
	nStaticHeight= m_pBalloon->m_pStaticBox->GetHeight() + 2 * m_nMarginY;
	
	// 최소폭
	// 일단 시간이 급하므로... 나중에 대폭 수정한다. 
	
	if( nStaticWidth + m_pOutline->GetLeftCX() + m_pOutline->GetRightCX() <
		m_pOutline->GetTailWidth() + m_pOutline->GetLeftCornerCX() + m_pOutline->GetRightCornerCX() )
	{
		INT nBeforeWidth = nStaticWidth;
		nStaticWidth = m_pOutline->GetTailWidth() + m_pOutline->GetLeftCornerCX() + m_pOutline->GetRightCornerCX()
					   - m_pOutline->GetLeftCX() - m_pOutline->GetRightCX();

		INT nOffset = (INT)( ( nStaticWidth - nBeforeWidth ) / 2 );

		m_pBalloon->m_pStaticBox->SetPosition( m_pOutline->GetLeftCX() + m_nMarginX + nOffset, m_pOutline->GetTopCY() + m_nMarginY );
		nTailX = m_pOutline->GetLeftCornerCX();
	}
	else
	{
		m_pBalloon->m_pStaticBox->SetPosition( m_pOutline->GetLeftCX() + m_nMarginX, m_pOutline->GetTopCY() + m_nMarginY );

		nTailX = (INT)( nStaticWidth * m_fTailFactor );

		if( nTailX < m_pOutline->GetLeftCornerCX() )
			nTailX = m_pOutline->GetLeftCornerCX();
	}

	rcInRect.left	= m_ptTailLT.x - ( nTailX + m_nMarginX );
	rcInRect.top	= m_ptTailLT.y - nStaticHeight - 1;
	rcInRect.right	= rcInRect.left + nStaticWidth;
	rcInRect.bottom = rcInRect.top + nStaticHeight;

	rcOutRect = m_pOutline->SetInScreenRect( rcInRect, nTailX );

	// 부모 좌표의 상대 위치로 좌표 바꿈.
	if( m_pBalloon->GetParent() )
	{
		rcParent = m_pBalloon->GetParent()->GetScreenRect();
		
		rcTemp.left		= rcOutRect.left - rcParent.left;
		rcTemp.top		= rcOutRect.top - rcParent.top;
		rcTemp.right	= rcTemp.left + rcOutRect.GetWidth();
		rcTemp.bottom	= rcTemp.top + rcOutRect.GetHeight();

		rcOutRect = rcTemp;
	}
	
	m_pBalloon->SetPosition( rcOutRect );
}

VOID CBalloon_Generic::SetBalloonColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	m_pOutline->SetSurfaceColor( ucRed, ucGreen, ucBlue );
}

/////////////////////////////////////////////////////////////////////////////
// callback function

VOID CBalloon_Generic::OnPaint(VOID)
{
	m_pOutline->Paint( m_pBalloon->GetRenderTop() );
}

VOID CBalloon_Generic::OnSetOption( const CComponentOptions& options )
{
	if( options.Exists( "margin_x" ) )
		m_nMarginX = options.GetValueAsInt( "margin_x" );
	if( options.Exists( "margin_y" ) )
		m_nMarginY = options.GetValueAsInt( "margin_y" );

	// Outline SetOption
	m_pOutline->SetSurfaces( options );
}

VOID CBalloon_Generic::OnMove( INT nOldX, INT nOldY )
{
	if( !m_pBalloon->m_pStaticBox || !m_pOutline || !m_pBalloon )
		return;

	CRectangle rect = m_pBalloon->GetScreenRect();
		
	// 스크린 좌표를 넘겨준다.
	m_pOutline->SetMove( rect.left, rect.top );
}

VOID CBalloon_Generic::OnSetAlpha( BYTE ucAlpha )
{
	//
	m_pOutline->SetSurfaceAlpha( ucAlpha );
}

VOID CBalloon_Generic::OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	m_pOutline->SetSurfaceColor( ucRed, ucGreen, ucBlue );
}


END_GUI
