#include "gui_precomp.h"
#include "gui_combobox.h"
#include "gui_combobox_generic.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CComboBox::CComboBox( CComponent *pParent,CSurfaceManager *pSurfaceManager )
: CComponent( pParent, pSurfaceManager ), m_pImpl( NULL ), m_pListBox( NULL ), m_pStaticBox( NULL ), m_pButton( NULL )
{
	m_bStaticBoxDisabled = false;

	m_pStaticBox	= NTL_NEW CStaticBox( this, pSurfaceManager );
	m_pButton		= NTL_NEW CButton( this, pSurfaceManager );
	m_pListBox		= NTL_NEW CListBox( this, pSurfaceManager );

	m_pImpl = NTL_NEW CComboBox_Generic( this );

	m_slotSetOption = SigSetOptions().Connect( this, &CComboBox::OnSetOptions );
}

CComboBox::CComboBox( const CRectangle &rtRect, CComponent *pParent, CSurfaceManager *pSurfaceManager, 
					  INT nMarginX /* = 0 */, INT nMarginY /* = 0 */, 
					  INT nButtonWidth /* = COMBOBOX_DEFAULT_BUTTON_WSIZE */, 
					  INT nButtonHeight /* = COMBOBOX_DEFAULT_BUTTON_WSIZE */, 
					  INT nListHeight /* = COMBOBOX_DEFAULT_LIST_HSIZE */, 					  
					  INT nMaxVisibleItems /* = COMBOBOX_DEFAULT_VISIBLEITEM  */)
: CComponent( rtRect, pParent, pSurfaceManager ), m_pImpl( NULL ), m_pListBox( NULL ), m_pStaticBox( NULL ), m_pButton( NULL )
{
	m_bStaticBoxDisabled = false;

	CRectangle rtStatic, rtButton, rtList;

	CalcSubRect( &rtStatic, &rtButton, &rtList, rtRect, nMarginX, nMarginY, nButtonWidth, nButtonHeight, nListHeight );
	
	m_pStaticBox = NTL_NEW CStaticBox( rtStatic, this, pSurfaceManager );
	m_pButton = NTL_NEW CButton( rtButton, "", this, pSurfaceManager );
	m_pListBox = NTL_NEW CListBox( rtList, this, pSurfaceManager, nMaxVisibleItems, nMarginX, nMarginY );

	m_pImpl = NTL_NEW CComboBox_Generic( this );

	m_slotSetOption = SigSetOptions().Connect( this, &CComboBox::OnSetOptions );
}

CComboBox::~CComboBox(VOID)
{
	NTL_DELETE( m_pStaticBox );
	NTL_DELETE( m_pListBox );
	NTL_DELETE( m_pButton );
	NTL_DELETE( m_pImpl );
}

////////////////////////////////////////////////////////////////////////////////
// Attributes

std::wstring CComboBox::GetSelectedItemText(VOID) const
{
	return m_pListBox->GetSelectedItemText();
}

DWORD CComboBox::GetSelectedItemData(VOID) const
{
	return m_pListBox->GetSelectedItem()->m_dwData;
}

INT CComboBox::GetSelectedItemIndex(VOID) const
{
	return m_pListBox->GetSelectedItemIndex();
}

DWORD CComboBox::GetItemData( INT nIndex ) const
{
	return m_pListBox->GetItem(nIndex)->m_dwData;
}

std::wstring CComboBox::GetItemText( INT nIndex ) const
{
	return m_pListBox->GetItemText( nIndex );
}

bool CComboBox::IsSelected( INT nIndex ) const
{
	return m_pListBox->IsSelected( nIndex );
}

std::list<CSurface>* CComboBox::GetSurface(VOID) const
{
	return &m_pImpl->m_listSurface;
}

std::list<CSurface>* CComboBox::GetStaticBoxSurface(VOID) const
{
	if (m_bStaticBoxDisabled)
		return NULL;

	return m_pStaticBox->GetSurface();
}

std::list<CSurface>* CComboBox::GetListBoxSurface(VOID) const
{
	return m_pListBox->GetSurface();
}

std::list<CSurface>* CComboBox::GetSurfaceUp(VOID) const
{
	return m_pButton->GetSurfaceUp();
}
std::list<CSurface>* CComboBox::GetSurfaceDown(VOID) const
{
	return m_pButton->GetSurfaceDown();
}

std::list<CSurface>* CComboBox::GetSurfaceFocus(VOID) const
{
	return m_pButton->GetSurfaceFocus();
}

std::list<CSurface>* CComboBox::GetSurfaceDisable(VOID) const
{
	return m_pButton->GetSurfaceDisable();
}

CButton* CComboBox::GetButton(VOID) const
{
	return m_pButton;
}

CListBox* CComboBox::GetListBox(VOID) const
{
	return m_pListBox;
}

CStaticBox* CComboBox::GetStaticBox(VOID) const
{
	if (m_bStaticBoxDisabled)
		return NULL;

	return m_pStaticBox;
}

VOID CComboBox::ToggleList(VOID)
{
	m_pImpl->ToggleList();
}

////////////////////////////////////////////////////////////////////////////////
// Operations

VOID CComboBox::SetTextFont( CHAR* pFaceName /* = DEFAULT_FONT */, INT nHeight /* = DEFAULT_FONT_HEIGHT */, INT nAttributes /* = 0  */)
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->CreateFontStd( pFaceName, nHeight, nAttributes );
}

VOID CComboBox::SetListFont( CHAR* pFaceName /* = DEFAULT_FONT */, INT nHeight /* = DEFAULT_FONT_HEIGHT */, INT nAttributes /* = 0  */)
{
	m_pListBox->CreateFontStd( pFaceName, nHeight, nAttributes );
}

VOID CComboBox::SetText( const CHAR* text )
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->SetText( text );
}

VOID CComboBox::SetText( const WCHAR* text )
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->SetText( text );
}

VOID CComboBox::ClearText(VOID)
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->Clear();
}

VOID CComboBox::SetTextColor( COLORREF color )
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->SetTextColor( color );
}

VOID CComboBox::SetEffectColor( COLORREF color )
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->SetEffectColor( color );
}

VOID CComboBox::SetEffectMode( INT nMode )
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->SetEffectMode( nMode );
}

VOID CComboBox::SetEffectValue( INT nValue )
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->SetEffectValue( nValue );
}

VOID CComboBox::SetTextStyle( DWORD dwStyle )
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->SetTextStyle( dwStyle );
}

INT CComboBox::AddItem( const CHAR* text, DWORD dwData /* = 0  */)
{
	return m_pListBox->AddItem( text, dwData );
}

INT CComboBox::AddItem( const WCHAR* text, DWORD dwData /* = 0  */)
{
	return m_pListBox->AddItem( text, dwData );
}

INT CComboBox::InsertItem( const CHAR* text, INT nIndex /* = -1 */, DWORD dwData /* = 0  */)
{
	return m_pListBox->InsertItem( text, nIndex, dwData );
}

INT CComboBox::InsertItem( const WCHAR* text, INT nIndex /* = -1 */, DWORD dwData /* = 0  */)
{
	return m_pListBox->InsertItem( text, nIndex, dwData );
}

VOID CComboBox::RemoveItem( INT nIndex )
{
	m_pListBox->RemoveItem( nIndex );
}

VOID CComboBox::ChangeItem( const CHAR* text, INT nIndex, DWORD dwData )
{
	m_pListBox->ChangeItem( text, nIndex, dwData );
}

VOID CComboBox::ChangeItem( const WCHAR *text, INT nIndex, DWORD dwData )
{
	m_pListBox->ChangeItem( text, nIndex, dwData );
}

VOID CComboBox::ClearAllItems(VOID)
{
	m_pListBox->ClearAllItems();
	ClearText();
}

VOID CComboBox::SelectItem( INT nIndex, bool bSelect /* = true  */ )
{
	m_pListBox->SelectItem( nIndex, bSelect );

	if( bSelect )
	{
		SetText( GetItemText( nIndex ).c_str() );
	}
	else
	{
		ClearText();
	}
}

VOID CComboBox::SetItemHeight( INT nHeight )
{
	m_pListBox->SetItemHeight( nHeight );
}

VOID CComboBox::SetMaxVisibleItems( INT nCount )
{
	m_pListBox->SetMaxVisibleItems( nCount );
}

VOID CComboBox::SetTopItem( INT nIndex )
{
	m_pListBox->SetTopItem( nIndex );
}

VOID CComboBox::SetSelectedMaskColor( BYTE uRed, BYTE uGreen, BYTE uBlue, BYTE uAlpha )
{
	m_pListBox->SetSelectedMaskColor( uRed, uGreen, uBlue, uAlpha );
}

VOID CComboBox::SetSelectedTextColor( COLORREF color )
{
	m_pListBox->SetSelectedTextColor( color );
}

VOID CComboBox::SetListTextColor( COLORREF color )
{
	m_pListBox->SetTextColor( color );
}

VOID CComboBox::SetListEffectColor( COLORREF color )
{
	m_pListBox->SetEffectColor( color );
}

VOID CComboBox::SetListTextStyle( DWORD dwStyle )
{
	m_pListBox->SetTextStyle( dwStyle );
}

VOID CComboBox::SetListEffectMode( INT nMode )
{
	m_pListBox->SetEffectMode( nMode );
}

VOID CComboBox::SetListEffectValue( INT nValue )
{
	m_pListBox->SetEffectValue( nValue );
}

VOID CComboBox::AddSurface( CSurface surface )
{
	m_pImpl->AddSurface( surface );
}

VOID CComboBox::AddStaticBoxSurface( CSurface surface )
{
	if (m_bStaticBoxDisabled)
		return;

	m_pStaticBox->AddSurface( surface );
}

VOID CComboBox::AddLIstBoxSurface( CSurface surface )
{
	m_pListBox->AddSurface( surface );
}

VOID CComboBox::AddListBoxMaskSurface( CSurface& surface )
{
	m_pListBox->AddMaskSurface( surface );
}

VOID CComboBox::AddSurfaceButtonUp( CSurface Surface )
{
	m_pButton->AddSurfaceUp( Surface );
}

VOID CComboBox::AddSurfaceButtonDown( CSurface Surface )
{
	m_pButton->AddSurfaceDown( Surface );
}

VOID CComboBox::AddSurfaceButtonFocus( CSurface Surface )
{
	m_pButton->AddSurfaceFocus( Surface );
}

VOID CComboBox::AddSurfaceButtonDisabled( CSurface Surface )
{
	m_pButton->AddSurfaceDisabled( Surface );
}

////////////////////////////////////////////////////////////////////////////////
// Signals

CSignal_v1<INT>& CComboBox::SigSelected(VOID)
{
	return m_pImpl->m_SigSelected;
}

CSignal_v2<BOOL,CComponent*>& CComboBox::SigListToggled(VOID)
{
	return m_pImpl->m_SigListToggled;
}

////////////////////////////////////////////////////////////////////////////////
// Implementation

VOID CComboBox::CalcSubRect( CRectangle* pStaticRect, CRectangle* pButtonRect, CRectangle* pListRect,
							const CRectangle& rtCombo, INT nMarginX, INT nMarginY,
							INT nButtonWidth, INT nButtonHeight, INT nListHeight )
{
	pStaticRect->left	= nMarginX;
	pStaticRect->top	= nMarginY;
	pStaticRect->right	= rtCombo.GetWidth() - nButtonWidth - 2 * nMarginX;
	pStaticRect->bottom = rtCombo.GetHeight() - nMarginY;

	pButtonRect->left	= pStaticRect->right + nMarginX;
	pButtonRect->top	= (INT)( ( rtCombo.GetHeight() - nButtonHeight ) / 2 );
	pButtonRect->right	= pButtonRect->left + nButtonWidth;
	pButtonRect->bottom	= pButtonRect->top + nButtonHeight;

	pListRect->left		= 0;
	pListRect->top		= rtCombo.GetHeight();
	pListRect->right	= rtCombo.GetWidth();
	pListRect->bottom	= pListRect->top + nListHeight;
}

////////////////////////////////////////////////////////////////////////////////
// Callbacks

VOID CComboBox::OnSetOptions( const CComponentOptions& options )
{
	CComponentOptions suboptions = options;
	CRectangle rtStatic, rtButton, rtList;

	INT nMarginX = 0, nMarginY = 0, nTextMarginX = 0, nVisibleCount = 1;
	INT nButtonWidth = COMBOBOX_DEFAULT_BUTTON_WSIZE, nButtonHeight = COMBOBOX_DEFAULT_BUTTON_WSIZE, nListHeight = COMBOBOX_DEFAULT_LIST_HSIZE;
	bool bDisableStaticBox = false;

	if( suboptions.Exists( "margin_x" ) )
		nMarginX = suboptions.GetValueAsInt( "margin_x" );
	if( suboptions.Exists( "margin_y" ) )
		nMarginY = suboptions.GetValueAsInt( "margin_y" );
	if( suboptions.Exists( "list_height" ) )
		nListHeight = suboptions.GetValueAsInt( "list_height" );
	if( suboptions.Exists( "combo_button_width" ) )
		nButtonWidth = suboptions.GetValueAsInt( "combo_button_width" );
	if( suboptions.Exists( "combo_button_height" ) )
		nButtonHeight = suboptions.GetValueAsInt( "combo_button_height" );
	if (suboptions.Exists("text_margin_x"))
		nTextMarginX = suboptions.GetValueAsInt("text_margin_x");
	if (suboptions.Exists("disable_staticbox")) // does not work yet
		bDisableStaticBox = suboptions.GetValueAsBool("disable_staticbox");

	CalcSubRect( &rtStatic, &rtButton, &rtList, GetClientRect(), nMarginX, nMarginY, nButtonWidth, nButtonHeight, nListHeight );

	// StaticBox 
	suboptions.RemoveOption( "x" );
	suboptions.RemoveOption( "y" );
	suboptions.RemoveOption( "width" );
	suboptions.RemoveOption( "height" );
	suboptions.RemoveOption( "surface" );
	suboptions.RemoveOption( "margin_x" );
	suboptions.RemoveOption( "margin_y" );

	suboptions.AddOption( "x", rtStatic.left );
	suboptions.AddOption( "y", rtStatic.top );
	suboptions.AddOption( "width", rtStatic.GetWidth() );
	suboptions.AddOption( "height", rtStatic.GetHeight() );

	if( suboptions.Exists( "static_surface" ) )
		suboptions.AddOption( "surface", suboptions.GetValue( "static_surface" ) );

	if (bDisableStaticBox)
	{
		m_bStaticBoxDisabled = true;

		m_pStaticBox->Show(false);
		m_pStaticBox->Enable(false);

		NTL_DELETE(m_pStaticBox);
	}
	else
	{
		m_pStaticBox->SigSetOptions()(suboptions);
	}

	// ListBox
	suboptions.RemoveOption( "x" );
	suboptions.RemoveOption( "y" );
	suboptions.RemoveOption( "width" );
	suboptions.RemoveOption( "height" );
	suboptions.RemoveOption( "surface" );
	suboptions.RemoveOption( "text_style" );

	suboptions.AddOption( "x", rtList.left );
	suboptions.AddOption( "y", rtList.top );
	suboptions.AddOption( "width", rtList.GetWidth() );
	suboptions.AddOption( "height", rtList.GetHeight() );
	suboptions.AddOption( "margin_x", nMarginX );
	suboptions.AddOption( "margin_y", nMarginY );
	suboptions.AddOption( "text_margin_x", nTextMarginX);

	if (suboptions.Exists("scrollbar_margin_x"))
		suboptions.AddOption("scrollbar_margin_x", suboptions.GetValue("scrollbar_margin_x"));
		
	if( suboptions.Exists( "list_text_style" ) )
		suboptions.AddOption( "text_style", suboptions.GetValue( "list_text_style" ) );
	
	if( suboptions.Exists( "list_surface" ) )
		suboptions.AddOption( "surface", suboptions.GetValue( "list_surface" ) );	

	if( suboptions.Exists( "scroll_button_width" ) )
		suboptions.AddOption( "button_width", suboptions.GetValue( "scroll_button_width" ) );	
	if( suboptions.Exists( "scroll_button_height" ) )
		suboptions.AddOption( "button_height", suboptions.GetValue( "scroll_button_height" ) );	

	m_pListBox->SigSetOptions()( suboptions );

	// Button
	suboptions.RemoveOption( "x" );
	suboptions.RemoveOption( "y" );
	suboptions.RemoveOption( "width" );
	suboptions.RemoveOption( "height" );
	suboptions.RemoveOption( "surface" );
	suboptions.RemoveOption( "text" );

	suboptions.AddOption( "x", rtButton.left );
	suboptions.AddOption( "y", rtButton.top );
	suboptions.AddOption( "width", rtButton.GetWidth() );
	suboptions.AddOption( "height", rtButton.GetHeight() );

	if( suboptions.Exists( "button_text" ) )
		suboptions.AddOption( "text", suboptions.GetValue( "button_text" ) );

	m_pButton->SigSetOptions()( suboptions );
}

END_GUI

