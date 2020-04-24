#include "gui_precomp.h"
#include "gui_Balloon.h"
#include "gui_Balloon_Generic.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CBalloon::CBalloon( CComponent* pParent, CSurfaceManager* pSurfaceManager, BOOL bLeft /* = TRUE */, DWORD dwStyle /* = 0  */)
:CComponent( pParent, pSurfaceManager ), m_pImpl( NULL ), m_pStaticBox( NULL )
{
	m_pStaticBox = NTL_NEW CStaticBox( this, pSurfaceManager, dwStyle, TRUE );
	m_pImpl = NTL_NEW CBalloon_Generic( this, bLeft );

	m_SlotSetOptions = SigSetOptions().Connect( this, &CBalloon::OnSetOption );
}

CBalloon::CBalloon( CRectangle& rtPos, CComponent* pParent, CSurfaceManager* pSurfaceManager, BOOL bLeft /* = TRUE */, DWORD dwStyle /* = 0  */)
:CComponent( rtPos, pParent, pSurfaceManager ), m_pImpl( NULL ), m_pStaticBox( NULL )
{
	m_pStaticBox = NTL_NEW CStaticBox( rtPos, this, pSurfaceManager, dwStyle, TRUE );
	m_pImpl = NTL_NEW CBalloon_Generic( this, bLeft );
	
	m_SlotSetOptions = SigSetOptions().Connect( this, &CBalloon::OnSetOption );
}

CBalloon::~CBalloon(VOID)
{
	NTL_DELETE( m_pStaticBox );
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CBalloon::SetOrigin( INT nScreenX, INT nScreenY, BOOL bLeft /* = FALSE  */)
{
	m_pImpl->SetOrigin( nScreenX, nScreenY, bLeft );
}

VOID CBalloon::SetOriginNow( INT nScreenX, INT nScreenY, BOOL bLeft /* = FALSE  */)
{
	m_pImpl->SetOriginNow( nScreenX, nScreenY, bLeft );
}

VOID CBalloon::SetMargin( INT nMarginX, INT nMarginY )
{
	m_pImpl->m_nMarginX = nMarginX;
	m_pImpl->m_nMarginY = nMarginY;
}

VOID CBalloon::SetBalloonColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	m_pImpl->SetBalloonColor( ucRed, ucGreen, ucBlue );
}

VOID CBalloon::CreateFontStd( CHAR* pFaceName, INT nHeight, INT nAttributes )
{
	m_pStaticBox->CreateFontStd( pFaceName, nHeight, nAttributes );
}

VOID CBalloon::SetText( const CHAR* text )
{
	m_pStaticBox->SetText( text );
	m_pImpl->CalcRect();
}

VOID CBalloon::SetText( const WCHAR* text )
{
	m_pStaticBox->SetText( text );
	m_pImpl->CalcRect();
}

VOID CBalloon::SetText( INT number )
{
	m_pStaticBox->SetText( number );
	m_pImpl->CalcRect();
}

VOID CBalloon::AddText( const CHAR* text ) 	
{
	m_pStaticBox->AddText( text );
	m_pImpl->CalcRect();
}

VOID CBalloon::AddText( const WCHAR* text )
{
	m_pStaticBox->AddText( text );
	m_pImpl->CalcRect();
}

VOID CBalloon::AddText( INT number )
{
	m_pStaticBox->AddText( number );
	m_pImpl->CalcRect();
}

VOID CBalloon::Format( CHAR* format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	m_pStaticBox->SetText( text );
	m_pImpl->CalcRect();
}

VOID CBalloon::AddFormat( CHAR* format, ... )
{
	CHAR text[GUI_TEXT_BUFFER_SIZE];

	va_list args;
	va_start( args, format );
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vsprintf_s( text, GUI_TEXT_BUFFER_SIZE, format, args );
#else
	vsprintf( text, format, args );
#endif
	va_end( args );

	m_pStaticBox->AddText( text );
	m_pImpl->CalcRect();
}

VOID CBalloon::SetTextColor( COLORREF color )
{
	m_pStaticBox->SetTextColor( color );
}

VOID CBalloon::SetEffectMode( INT nMode )
{
	m_pStaticBox->SetEffectMode( nMode );
}

VOID CBalloon::SetEffectColor( COLORREF color )
{
	m_pStaticBox->SetEffectColor( color );
}

VOID CBalloon::SEtEffectValue( INT nValue )
{
	m_pStaticBox->SetEffectValue( nValue );
}

VOID CBalloon::SetBkMode( INT nBkMode )
{
	m_pStaticBox->SetBkMode( nBkMode );
}

VOID CBalloon::SetBkColor( COLORREF color )
{
	m_pStaticBox->SetBkColor( color );
}

VOID CBalloon::SetTextStyle( DWORD dwStyle )
{
	m_pStaticBox->SetTextStyle( dwStyle );
}

VOID CBalloon::SetLineSpace( INT nLineSpace )
{
	m_pStaticBox->SetLineSpace( nLineSpace );
}

VOID CBalloon::SetMaxSize( INT nCX, INT nCY )
{
	m_pStaticBox->SetDynamicMaxSize( nCX, nCY );
}

VOID CBalloon::SetMinSize( INT nCX, INT nCY )
{
	m_pStaticBox->SetDynamicMinSize( nCX, nCY );
}

VOID CBalloon::Clear(VOID)
{
	m_pStaticBox->Clear();
}

VOID CBalloon::AddSurfaces( CSurface& surCenter, CSurface& surLTC, CSurface& surRTC, CSurface& surLBC, CSurface& surRBC,
						    CSurface& surLL, CSurface& surTL, CSurface& surRL, CSurface& surBL, CSurface& surTail )
{
	m_pImpl->m_pOutline->AddSurfaces( surCenter, surLTC, surRTC, surLBC, surRBC, surLL, surTL, surRL, surBL, surTail );
}

VOID CBalloon::SetLeftCX( INT nCX )
{
	m_pImpl->m_pOutline->SetLeftCX( nCX ); 
}

VOID CBalloon::SetRightCX( INT nCX )
{ 
	m_pImpl->m_pOutline->SetRightCX( nCX );
}

VOID CBalloon::SetTopCY( INT nCY )
{
	m_pImpl->m_pOutline->SetTopCY( nCY ); 
}

VOID CBalloon::SetBottomCY( INT nCY )
{
	m_pImpl->m_pOutline->SetBottomCY( nCY );
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CBalloon::OnSetOption( const CComponentOptions& options )
{
	// StaticBox SetOption
	CComponentOptions opStatic = options;
	opStatic.RemoveOption( "text_bk_mode" );
	opStatic.RemoveOption( "dynamic" );
	opStatic.RemoveOption( "surface" );
	opStatic.RemoveOption( "margin_x" );
	opStatic.RemoveOption( "margin_y" );

	m_pStaticBox->SigSetOptions()( opStatic );
}

END_GUI