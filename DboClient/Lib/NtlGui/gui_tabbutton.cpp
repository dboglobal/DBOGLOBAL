#include "gui_precomp.h"
#include "gui_tabbutton_generic.h"
#include "gui_tabbutton.h"
#include "mouse.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////
// Constructors & Destructors

CTabButton::CTabButton( CComponent* pParent, CSurfaceManager* pSurfaceManager )
: CComponent( pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CTabButton_Generic( this );	
}

CTabButton::CTabButton( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager )
: CComponent( rtRect, pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CTabButton_Generic( this );	
}

CTabButton::~CTabButton(VOID)
{
	NTL_DELETE( m_pImpl );
}

////////////////////////////////////////////////////////////////////////////////
// Attributes:

INT CTabButton::GetButtonCount(VOID) const
{
	return m_pImpl->GetTabCount();
}

INT CTabButton::GetSelectedButtonIndex(VOID) const
{
	return m_pImpl->GetSelectedTabIndex();
}

VOID CTabButton::SetTabSurface( CSurface& left, CSurface& center, CSurface& right, 
							    CSurface& selectedleft, CSurface& selectedcenter, CSurface& selectedright, 
								CSurface& focusleft, CSurface& focuscenter, CSurface& focusright, CSurface& line )
{
	m_pImpl->m_Surface[CTabButton_Generic::LEFT] = left;
	m_pImpl->m_Surface[CTabButton_Generic::CENTER] = center;
	m_pImpl->m_Surface[CTabButton_Generic::RIGHT] = right;
	m_pImpl->m_Surface[CTabButton_Generic::SEL_LEFT] = selectedleft;
	m_pImpl->m_Surface[CTabButton_Generic::SEL_CENTER] = selectedcenter;
	m_pImpl->m_Surface[CTabButton_Generic::SEL_RIGHT] = selectedright;
	m_pImpl->m_Surface[CTabButton_Generic::FOC_LEFT] = focusleft;
	m_pImpl->m_Surface[CTabButton_Generic::FOC_CENTER] = focuscenter;
	m_pImpl->m_Surface[CTabButton_Generic::FOC_RIGHT] = focusright;
	m_pImpl->m_Surface[CTabButton_Generic::LINE] = line;

	m_pImpl->InitLine();
}

VOID CTabButton::SetTextColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	m_pImpl->m_TextColor = RGB( ucRed, ucGreen, ucBlue );
}

VOID CTabButton::SetTextSelectedColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	m_pImpl->m_SelTextColor = RGB( ucRed, ucGreen, ucBlue );
}

VOID CTabButton::SetTextFocusedColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	m_pImpl->m_FocTextColor = RGB( ucRed, ucGreen, ucBlue );
}

VOID CTabButton::SetTextDisabledColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	m_pImpl->m_DisableTextColor = RGB( ucRed, ucGreen, ucBlue );
}

VOID CTabButton::SetTextAlpha( BYTE ucAlpha )
{
	m_pImpl->m_ucTextAlpha = ucAlpha;
}

VOID CTabButton::SetTextSelectedAlpha( BYTE ucAlpha )
{
	m_pImpl->m_ucSelTextAlpha = ucAlpha;
}

VOID CTabButton::SetTextFocusedAlpha( BYTE ucAlpha )
{
	m_pImpl->m_ucFocTextAlpha = ucAlpha;
}

VOID CTabButton::SetTextDisabledAlpha( BYTE ucAlpha )
{
	m_pImpl->m_ucDisableTextAlpha = ucAlpha;
}

VOID CTabButton::SetFont( CHAR* pFaceName, INT nHeight, INT nAttributes )
{
	m_pImpl->m_strFontname = pFaceName;
	m_pImpl->m_dwFontHeight = nHeight;
	m_pImpl->m_dwFontAttr = nAttributes;
}

VOID CTabButton::SetTextAlign( DWORD dwAlign )
{
	m_pImpl->m_dwTextStyle = dwAlign;
}

VOID CTabButton::SetTabTextMargin( INT nMarginX, INT nMarginY )
{
	m_pImpl->m_nTextMarginX = nMarginX;
	m_pImpl->m_nTextMarginY = nMarginY;
}

VOID CTabButton::ApplyAttribute(VOID)
{
	m_pImpl->ApplyAttribute();
}

VOID CTabButton::SetUnselTabWidth( INT nWidth, BOOL bRedraw /* = TRUE  */ )
{
	m_pImpl->m_nUnselWidth = nWidth;
	
	if( bRedraw )
		m_pImpl->CalcTabRect();
}

VOID CTabButton::SetOrient( BOOL bLeft, BOOL bRedraw /* = TRUE  */ )
{
	m_pImpl->m_bLeftOriented = bLeft;

	if( bRedraw )
		m_pImpl->CalcTabRect();
}

INT CTabButton::AddTab( std::string& strTabText, BOOL bRedraw /* = TRUE  */ )
{
	INT nRet = m_pImpl->AddTab( strTabText );

	if( bRedraw )
		m_pImpl->CalcTabRect();
	
	return nRet;
}

INT CTabButton::AddTab( std::wstring& wstrTabText, BOOL bRedraw /* = TRUE  */ )
{
	INT nRet = m_pImpl->AddTab( wstrTabText );

	if( bRedraw )
		m_pImpl->CalcTabRect();

	return nRet;
}

VOID CTabButton::ClearTab(VOID)
{
	m_pImpl->ClearTab();
}

VOID CTabButton::SetFocusSound( const CHAR* szFocusSoundFIlename )
{
	m_pImpl->m_strFocusSound = szFocusSoundFIlename;
}

VOID CTabButton::SetClickSound( const CHAR* szClickSoundFilename )
{
	m_pImpl->m_strClickSound = szClickSoundFilename;
}

////////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CTabButton::SelectTab( INT nIndex, BOOL bDoCallBack /* = FALSE  */ )
{
	m_pImpl->SelectTab( nIndex, bDoCallBack );
}

VOID CTabButton::EnableTab( INT nIndex, BOOL bEnable /* = TRUE  */ )
{
	m_pImpl->EnableTab( nIndex, bEnable );
}

VOID CTabButton::ChangeTabText( INT nIndex, std::string& strText )
{
	m_pImpl->ChangeTabText( nIndex, strText );
}

VOID CTabButton::ChangeTabText( INT nIndex, std::wstring& wstrText )
{
	m_pImpl->ChangeTabText( nIndex, wstrText );
}

////////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v1<INT>& CTabButton::SigSelected(VOID)
{
	return m_pImpl->m_SigSelected;
}

CSignal_v2<INT,INT>& CTabButton::SigSelectChanged(VOID)
{
	return m_pImpl->m_SigSelectChanged;
}

////////////////////////////////////////////////////////////////////////////////
// Callbacks:

END_GUI

