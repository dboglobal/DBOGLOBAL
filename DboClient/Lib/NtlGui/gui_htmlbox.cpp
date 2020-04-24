#include "gui_precomp.h"
#include "gui_htmlbox.h"
#include "gui_htmlbox_generic.h"

START_GUI

/**
* \brief Contruction
* \param pParent			(CComponent*) 부모 Component
* \param pSurfaceManager	(pSurfaceManager*) 서페이스 매니저
*/
CHtmlBox::CHtmlBox( CComponent *pParent, CSurfaceManager* pSurfaceManager )
:CComponent( pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pScrollBar = NTL_NEW CScrollBar( CRectangle( 0, 0, 0, 0 ), false, this, pSurfaceManager );

	m_pImpl = NTL_NEW CHtmlBox_Generic( this );

	m_pScrollBar->Show( false );

	m_slotSetOptions = SigSetOptions().Connect( this, &CHtmlBox::OnSetOption );
	m_SlotResize = SigResize().Connect(this, &CHtmlBox::OnResize);
}


CHtmlBox::CHtmlBox( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager, INT nScrollBarWidth /* = DEFAULT_SLIDER_WIDTH  */)
:CComponent( rtRect, pParent, pSurfaceManager ), m_pImpl( NULL )
{
	CRectangle rtScrollBar;
	CalcScrollBarRect( &rtScrollBar, nScrollBarWidth );

	m_pScrollBar = NTL_NEW CScrollBar( rtScrollBar, false, this, pSurfaceManager );
	m_pImpl = NTL_NEW CHtmlBox_Generic( this );
	m_pImpl->CreateTextTexture();

	m_pScrollBar->Show( false );

	m_slotSetOptions = SigSetOptions().Connect( this, &CHtmlBox::OnSetOption );
	m_SlotResize = SigResize().Connect(this, &CHtmlBox::OnResize);
}

/**
* \brief Destruction
*/
CHtmlBox::~CHtmlBox()
{
	NTL_DELETE( m_pImpl );
}

/**
* \brief File에서 Html Tag를 읽어와 파싱한다.
* \param pFileName	(CHAR*) 파일 이름
*/
VOID CHtmlBox::SetHtmlFromFile(const CHAR *pFileName)
{
	m_pImpl->SetHtmlFromFile(pFileName);
}

VOID CHtmlBox::SetHtmlFromFile( const WCHAR* pFileName )
{
	CHAR acBuffer[1024] = " ";
	::WideCharToMultiByte( GetACP(), 0, pFileName, -1, acBuffer, 1024, NULL, NULL);
	m_pImpl->SetHtmlFromFile(acBuffer);
}

/**
* \brief Memory에서 Html Tag를 읽어와 파싱한다. (멀티바이트용)
* \param pMemory	(CHAR*) 문자열
* \param nSize		(INT) 메모리의 사이즈
*/
VOID CHtmlBox::SetHtmlFromMemory(const CHAR *pMemory, INT nSize)
{
	// Multibyte 문자열을 Unicode 로 변환하여 Unicode용 함수를 호출한다.
	WCHAR awcBuffer[1024] = L" ";
	::MultiByteToWideChar( GetACP(), 0, pMemory, -1, awcBuffer, 1024 );
	m_pImpl->SetHtmlFromMemory( awcBuffer, (INT)wcslen(awcBuffer) );
}

/**
* \brief Memory에서 Html Tag를 읽어와 파싱한다. (유니코드용)
* \param pMemory	(WCHAR*) 문자열
* \param nSize		(INT) 메모리의 사이즈
*/
VOID CHtmlBox::SetHtmlFromMemory(const WCHAR *pMemory, INT nSize)
{
	m_pImpl->SetHtmlFromMemory(pMemory, nSize);
}

BOOL CHtmlBox::SetFontRatio(  float fRatio  ) 
{
	return m_pImpl->SetFontRatio( fRatio );
}

VOID CHtmlBox::Clear(VOID)
{
	m_pImpl->Clear();	
}

VOID CHtmlBox::SetStyle( DWORD dwStyle )
{
	m_pImpl->SetStyle(dwStyle);
}

VOID CHtmlBox::SetLineSpace(INT nSpace)
{
	m_pImpl->SetLineSpace(nSpace);
}

VOID CHtmlBox::SetTextBackgroundColor(COLORREF color)
{
	m_pImpl->SetTextBackgroundColor(color);
}

VOID CHtmlBox::AddSurface( CSurface Surface )
{

}

VOID CHtmlBox::AddScrollSurface( CSurface surDecreaseUp, CSurface surDecreaseDown, CSurface surDecreaseFocus, CSurface surDecreaseDisable,
								CSurface surIncreaseUp, CSurface surIncreaseDown, CSurface surIncreaseFocus, CSurface surIncreaseDisable, 
								CSurface surSlider, CSurface surLayout )
{
	CButton*	pIncreaseButton = m_pScrollBar->GetIncreaseButton();
	CButton*	pDecreaseButton = m_pScrollBar->GetDecreaseButton();

	m_pScrollBar->AddSurfaceLayout( surLayout );
	m_pScrollBar->AddSurfaceSlider( surSlider );
	pIncreaseButton->AddSurfaceUp( surIncreaseUp );
	pIncreaseButton->AddSurfaceDown( surIncreaseDown );
	pIncreaseButton->AddSurfaceFocus( surIncreaseFocus );
	pIncreaseButton->AddSurfaceDisabled( surIncreaseDisable );
	pDecreaseButton->AddSurfaceUp( surDecreaseUp );
	pDecreaseButton->AddSurfaceDown( surDecreaseDown );
	pDecreaseButton->AddSurfaceFocus( surDecreaseFocus );
	pDecreaseButton->AddSurfaceDisabled( surDecreaseDisable );
}

CScrollBar* CHtmlBox::GetScrollBar(VOID) const
{
	return m_pScrollBar;
}

///////////////////////////////////////////////////////////////////////
// Interval Text 인터페이스
///////////////////////////////////////////////////////////////////////

VOID CHtmlBox::SetIntervalTextEnable( float fIntervalTime ) 
{
	m_pImpl->SetIntervalTextEnable( fIntervalTime );
}

VOID CHtmlBox::SetIntervalTextDisable() 
{
	m_pImpl->SetIntervalTextDisable();
}

VOID CHtmlBox::Update(  float fElapsedTime  ) 
{
	m_pImpl->Update( fElapsedTime );
}

INT CHtmlBox::GetIntervalPageNums( VOID ) 
{
	return m_pImpl->GetIntervalPageNums();
}

INT CHtmlBox::GetCurrentPage(VOID)
{
	return m_pImpl->GetCurrentPage();
}

BOOL CHtmlBox::SetNextPage( VOID ) 
{
	return m_pImpl->SetNextPage();
}

BOOL CHtmlBox::SetPrevPage( VOID )
{
	return m_pImpl->SetPrevPage();
}

BOOL CHtmlBox::SetSkipPage( VOID ) 
{
	return m_pImpl->SetSkipPage();
}

BOOL CHtmlBox::SetPlayPage( INT nPage ) 
{
	return m_pImpl->SetPlayPage( nPage );
}

VOID CHtmlBox::SetPause( BOOL bPause /*= TRUE */ ) 
{
	m_pImpl->SetPause( bPause );
}

VOID CHtmlBox::SetImmediate( BOOL bImmediate /*= TRUE */ ) 
{
	m_pImpl->SetImmediate( bImmediate );
}

VOID CHtmlBox::SetPresentNum( INT nNum ) 
{
	m_pImpl->SetPresentNum( nNum );
}

INT CHtmlBox::GetPresentNum() 
{
	return m_pImpl->GetPresentNum();
}

CSignal_v1<int>& CHtmlBox::SigPageDone()
{
	return m_pImpl->m_SigPageDone;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

VOID CHtmlBox::CalcScrollBarRect( CRectangle* prtScrollBar, INT nScrollBarWidth )
{
	CRectangle rtRect = GetClientRect();

	prtScrollBar->left		= rtRect.right - nScrollBarWidth;
	prtScrollBar->top		= rtRect.top;
	prtScrollBar->right		= rtRect.right;
	prtScrollBar->bottom	= rtRect.bottom;
}

VOID CHtmlBox::OnSetOption( const CComponentOptions& options )
{
	CComponentOptions suboptions = options;
	suboptions.RemoveOption( "x" );
	suboptions.RemoveOption( "y" );
	suboptions.RemoveOption( "width" );
	suboptions.RemoveOption( "height" );

	CRectangle rtScrollBar;
	INT nScrollBarWidth = DEFAULT_SLIDER_WIDTH;

	if( suboptions.Exists( "slider_width" ) )
		nScrollBarWidth = suboptions.GetValueAsInt( "slider_width" );

	CalcScrollBarRect( &rtScrollBar, nScrollBarWidth );

	suboptions.AddOption( "x", rtScrollBar.left );
	suboptions.AddOption( "y", rtScrollBar.top );
	suboptions.AddOption( "width", rtScrollBar.GetWidth() );
	suboptions.AddOption( "height", rtScrollBar.GetHeight() );

	m_pScrollBar->SigSetOptions()( suboptions );
	m_pScrollBar->SetRange( 0, 0 );
}

VOID CHtmlBox::OnResize(int nCx, int nCy)
{
	if(m_pScrollBar)
	{
		CRectangle rtScrollBarClient = m_pScrollBar->GetClientRect();
		CRectangle rtScrollBar;
		CalcScrollBarRect(&rtScrollBar, rtScrollBarClient.GetWidth());

		m_pScrollBar->SetPosition(rtScrollBar);
	}
}


END_GUI