#include "gui_precomp.h"
#include "gui_tooltip_generic.h"
#include "gui_dynamicoutline.h"
#include "mouse.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CToolTip_Generic::CToolTip_Generic( CToolTip* pSelf )
: m_pToolTip( pSelf ), m_pOutline( NULL ), m_nMarginX( 0 ), m_nMarginY( 0 ),
  m_nMouseMarginX( 0 ), m_nMouseMarginY( 0 )
{
	m_pOutline	 = NTL_NEW CDynamicOutline( pSelf, FALSE );
	m_pToolTip->Show( false );
	m_pToolTip->SetRenderTop( true );

	m_SlotSetOption = m_pToolTip->SigSetOptions().Connect( this, &CToolTip_Generic::OnSetOption );
	m_SlotPaint = m_pToolTip->SigPaint().Connect( this, &CToolTip_Generic::OnPaint );
	m_SlotMove = m_pToolTip->SigMove().Connect( this, &CToolTip_Generic::OnMove );
}

CToolTip_Generic::~CToolTip_Generic(VOID)
{
	NTL_DELETE( m_pOutline );
}

////////////////////////////////////////////////////////////////////////////////
// Oprations:

VOID CToolTip_Generic::UpdateMousePt(VOID)
{
	INT nLeft, nTop;
	INT nOutWidth = m_pToolTip->GetWidth();
	INT nOutHeight = m_pToolTip->GetHeight();
	CPos ptMouse( CMouse::GetX(), CMouse::GetY() );

	if( ptMouse.y + m_nMouseMarginY + nOutHeight >= m_pToolTip->GetGuiManager()->GetHeight() )
	{
		nTop = ptMouse.y - m_nMouseMarginY - nOutHeight;
	}
	else
	{
		nTop = ptMouse.y + m_nMouseMarginY;
	}

	if( ptMouse.x + m_nMouseMarginX + nOutWidth >= m_pToolTip->GetGuiManager()->GetWidth() )
	{
		nLeft = m_pToolTip->GetGuiManager()->GetWidth() - nOutWidth;
	}
	else
	{
		nLeft = ptMouse.x + m_nMouseMarginX;
	}
	
	m_pToolTip->SetPosition( nLeft, nTop );
}

VOID CToolTip_Generic::CalcRect(VOID)// const CPos& ptMouse )
{
	CRectangle rcOutRect, rcParent, rcTemp;
	CStaticBox* pStaticBox = m_pToolTip->m_pStaticBox;

	if( pStaticBox->GetWidth() == 0 || pStaticBox->GetHeight() == 0 )
		return;

	INT	nInWidth, nInHeight, nOutWidth, nOutHeight;
	nInWidth = pStaticBox->GetWidth() + 2 * m_nMarginX;
	nInHeight= pStaticBox->GetHeight() + 2 * m_nMarginY;
	nOutWidth = nInWidth + m_pOutline->GetLeftCX() + m_pOutline->GetRightCX();
	nOutHeight= nInHeight + m_pOutline->GetTopCY() + m_pOutline->GetBottomCY();

	rcOutRect.left	= 0;
	rcOutRect.top	= 0;
	rcOutRect.right	= nOutWidth;
	rcOutRect.bottom= nOutHeight;
	/*
	if( ptMouse.x + m_nMouseMarginX + nOutWidth >= SCR_WIDTH )
	{
		rcOutRect.left = SCR_WIDTH - nOutWidth;
		rcOutRect.right= SCR_WIDTH;
	}
	else
	{
		rcOutRect.left = ptMouse.x + m_nMouseMarginX;
		rcOutRect.right= rcInRect.left + nOutWidth;
	}

	if( ptMouse.y + m_nMouseMarginY + nOutHeight >= SCR_HEIGHT)
	{
		rcOutRect.bottom= ptMouse.y - m_nMouseMarginY;
		rcOutRect.top	= rcOutRect.bottom - nOutHeight;		
	}
	else
	{
		rcOutRect.top	= ptMouse.y + m_nMouseMarginY;
		rcOutRect.bottom= rcOutRect.top + nOutHeight;
	}*/

	m_pOutline->SetOutScreenRect( rcOutRect );

	// 부모 좌표의 상대 위치로 좌표 바꿈.
	if( m_pToolTip->GetParent() )
	{
		rcParent = m_pToolTip->GetParent()->GetScreenRect();

		rcTemp.left		= rcOutRect.left - rcParent.left;
		rcTemp.top		= rcOutRect.top - rcParent.top;
		rcTemp.right	= rcTemp.left + rcOutRect.GetWidth();
		rcTemp.bottom	= rcTemp.top + rcOutRect.GetHeight();

		rcOutRect = rcTemp;
	}

	m_pToolTip->SetPosition( rcOutRect );	
}

////////////////////////////////////////////////////////////////////////////////
// Implementations:

VOID CToolTip_Generic::SetTextPosition(VOID)
{
	// 부모에서의 상대좌표를 넘겨준다. 여백, 선두께가 지정될때마다 실행
	m_pToolTip->m_pStaticBox->SetPosition( m_pOutline->GetLeftCX() + m_nMarginX, m_pOutline->GetTopCY() + m_nMarginY );
}

VOID CToolTip_Generic::Render(VOID)
{
	m_pOutline->Paint( m_pToolTip->GetRenderTop() );
}

////////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CToolTip_Generic::OnPaint(VOID)
{
	Render();
}

VOID CToolTip_Generic::OnSetOption(const CComponentOptions &options)
{
	if( options.Exists( "margin_x" ) )
		m_nMarginX = options.GetValueAsInt( "margin_x" );
	if( options.Exists( "margin_y" ) )
		m_nMarginY = options.GetValueAsInt( "margin_y" );
	if( options.Exists( "mousemargin_x" ) )
		m_nMouseMarginX = options.GetValueAsInt( "mousemargin_x" );
	if( options.Exists( "mousemargin_y" ) )
		m_nMouseMarginY = options.GetValueAsInt( "mousemargin_y" );

	// Outline SetOption
	m_pOutline->SetSurfaces( options );
}

VOID CToolTip_Generic::OnMove( INT nOldX,INT nOldY )
{
	if( !m_pToolTip->m_pStaticBox || !m_pOutline || !m_pToolTip )
		return;

	CRectangle rect = m_pToolTip->GetScreenRect();

	// 스크린 좌표를 넘겨준다.
	m_pOutline->SetMove( rect.left, rect.top );
}


END_GUI
