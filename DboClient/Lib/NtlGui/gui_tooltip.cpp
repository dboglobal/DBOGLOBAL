#include "gui_precomp.h"
#include "gui_tooltip.h"
#include "gui_tooltip_generic.h"
#include "gui_component_generic.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor
CToolTip::CToolTip( CComponent* pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle /* = 0  */)
:CComponent( pParent, pSurfaceManager ), m_pImpl( NULL ), m_pStaticBox( NULL )
{
	m_pStaticBox = NTL_NEW CStaticBox( this, pSurfaceManager, dwStyle, TRUE );
	m_pStaticBox->SetRenderTop( true );
	m_pImpl = NTL_NEW CToolTip_Generic( this );

	m_SlotSetOptions = SigSetOptions().Connect( this, &CToolTip::OnSetOption );

	Enable( false );
}

CToolTip::CToolTip( const CRectangle& rtPos, CComponent* pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle /* = 0  */)
:CComponent( rtPos, pParent, pSurfaceManager ), m_pImpl( NULL ), m_pStaticBox( NULL )
{
	m_pStaticBox = NTL_NEW CStaticBox( rtPos, this, pSurfaceManager, dwStyle, TRUE );
	m_pStaticBox->SetRenderTop( true );
	m_pImpl = NTL_NEW CToolTip_Generic( this );
	
	m_SlotSetOptions = SigSetOptions().Connect( this, &CToolTip::OnSetOption );

	Enable( false );
}

CToolTip::~CToolTip(VOID)
{
	NTL_DELETE( m_pStaticBox );
	NTL_DELETE( m_pImpl );
}

////////////////////////////////////////////////////////////////////////////////
// Attributes:

BOOL CToolTip::IsOwnerComponent( CComponent* pComp )
{
	if( m_pImpl->m_pOwner == pComp )
		return TRUE;
	
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CToolTip::Show( bool bShow )
{
	CComponent::m_pImpl->m_bVisible = bShow;
}

VOID CToolTip::SetMargin( INT nMarginX, INT nMarginY )
{
	m_pImpl->m_nMarginX = nMarginX;
	m_pImpl->m_nMarginY = nMarginY;
}

VOID CToolTip::SetMouseMargin( INT nMouseMarginX, INT nMouseMarginY )
{
	m_pImpl->m_nMouseMarginX = nMouseMarginX;
	m_pImpl->m_nMouseMarginY = nMouseMarginY;
}

VOID CToolTip::CreateFontStd( CHAR* pFaceName, INT nHeight, INT nAttributes)
{
	m_pStaticBox->CreateFontStd( pFaceName, nHeight, nAttributes );
}

VOID CToolTip::SetTextColor( COLORREF color )
{
	m_pStaticBox->SetTextColor( color );
}

VOID CToolTip::SetBkColor( COLORREF color )
{
	m_pStaticBox->SetBkColor( color );
}

VOID CToolTip::SetBkMode( INT nBkMode )
{
	m_pStaticBox->SetBkMode( nBkMode );
}

VOID CToolTip::SetEffectMode( INT nMode )
{
	m_pStaticBox->SetEffectMode( nMode );
}

VOID CToolTip::SetEffectColor( COLORREF color )
{
	m_pStaticBox->SetEffectColor( color );
}

VOID CToolTip::SetEffectValue( INT nValue )
{
	m_pStaticBox->SetEffectValue( nValue );
}

VOID CToolTip::SetMaxSize( INT nCX, INT nCY )
{
	m_pStaticBox->SetDynamicMaxSize( nCX, nCY );
}

VOID CToolTip::SetMinSize( INT nCX, INT nCY )
{
	m_pStaticBox->SetDynamicMinSize( nCX, nCY );
}

VOID CToolTip::SetTextStyle( DWORD dwStyle )
{
	m_pStaticBox->SetTextStyle( dwStyle );
}

VOID CToolTip::AddSurfaces( CSurface& surCenter, CSurface& surLTC, CSurface& surRTC, CSurface& surLBC, CSurface& surRBC, 
							CSurface& surLL, CSurface& surTL, CSurface& surRL, CSurface& surBL )
{
	m_pImpl->m_pOutline->AddSurfaces( surCenter, surLTC, surRTC, surLBC, surRBC, surLL, surTL, surRL, surBL, CSurface() );
	SetTextPosition();
}

VOID CToolTip::SetTextPosition(VOID)
{
	m_pImpl->SetTextPosition();
}

VOID CToolTip::ShowToolTip( const CHAR* text, CComponent* pOwner, DWORD dwAlign /* = 0  */)
{
	Show( true );

	m_pStaticBox->SetText( text );
	m_pImpl->CalcRect();	
	m_pImpl->m_pOwner = pOwner;

	UpdateMousePt();
}

VOID CToolTip::ShowToolTip( const WCHAR* text, CComponent* pOwner, DWORD dwAlign /* = 0  */)
{
	Show( true );

	m_pStaticBox->SetText( text );
	m_pImpl->CalcRect();	
	m_pImpl->m_pOwner = pOwner;

	UpdateMousePt();
}

VOID CToolTip::HideToolTip( CComponent* pOwner )
{
	if( pOwner == m_pImpl->m_pOwner )
	{
		Show( false );
		m_pImpl->m_pOwner = NULL;
	}	
}

VOID CToolTip::UpdateMousePt(VOID)
{
	m_pImpl->UpdateMousePt();
}

VOID CToolTip::CheckOwnerCompHide(VOID)
{
	if( !m_pImpl->m_pOwner->IsVisibleTruly() )
		HideToolTip( m_pImpl->m_pOwner );	
}

////////////////////////////////////////////////////////////////////////////////
// Tooltips:

////////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CToolTip::OnSetOption( const CComponentOptions& options )
{
	m_pStaticBox->SigSetOptions()( options );

	m_pImpl->SetTextPosition();
}

END_GUI