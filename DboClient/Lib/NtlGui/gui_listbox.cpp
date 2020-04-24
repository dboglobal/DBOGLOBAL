#include "gui_precomp.h"
#include "gui_listbox.h"
#include "gui_componentoption.h"
#include "gui_listbox_generic.h"
#include "gui_component_generic.h"
#include "gui_scrollbar.h"
#include "gui_renderer.h"

START_GUI

/////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CListBox::CListBox( CComponent* pParent, CSurfaceManager* pSurfaceManager )
: CComponent( pParent, pSurfaceManager ), m_pImpl( 0 ), m_pScrollBar( NULL )
{
	m_pScrollBar = NTL_NEW CScrollBar( CRectangle( 0, 0, 0, 0 ), false, this, pSurfaceManager );
	m_pImpl = NTL_NEW CListBox_Generic( this, TRUE );
	
	m_SlotSetOptions = SigSetOptions().Connect( this, &CListBox::OnSetOptions );
	
}

CListBox::CListBox( const CRectangle& pos, CComponent* pParent, CSurfaceManager* pSurfaceManager, 
					INT nMaxVisibleItems /* = 1 */, INT nMarginX /* = 0 */, INT nMarginY /* = 0 */,
					INT nScrollBarWidth /* = DEFAULT_SLIDER_WIDTH */, BOOL bDynamicScrollBar /* = FALSE  */)
: CComponent( pos, pParent, pSurfaceManager ), m_pImpl( 0 ), m_pScrollBar( NULL )
{
	CRectangle rtScrollBar;
	CalcSubRect( &rtScrollBar, nScrollBarWidth, nMarginX, nMarginY );	
	
	m_pScrollBar = NTL_NEW CScrollBar( rtScrollBar, false, this, pSurfaceManager );
	m_pImpl = NTL_NEW CListBox_Generic( this, nMaxVisibleItems, bDynamicScrollBar );

	m_SlotSetOptions = SigSetOptions().Connect( this, &CListBox::OnSetOptions );
}

CListBox::CListBox( const CRectangle& pos, CComponent *pParent, CSurfaceManager *pSurfaceManager, 
				    INT nMaxVisibleItems, INT nRowHeight, INT nMarginX, INT nMarginY, INT nScrollBarWidth, BOOL bDynamicScrollBar /* = FALSE  */)
: CComponent( pos, pParent, pSurfaceManager ), m_pImpl( 0 ), m_pScrollBar( NULL )
{
	CRectangle rtListBox = pos;
	rtListBox.bottom = pos.top + nMaxVisibleItems * nRowHeight + 2 * nMarginY;

	CRectangle rtScrollBar;
	CalcSubRect( &rtScrollBar, nScrollBarWidth, nMarginX, nMarginY );	

	m_pScrollBar = NTL_NEW CScrollBar( rtScrollBar, false, this, pSurfaceManager );
	m_pImpl = NTL_NEW CListBox_Generic( this, nMaxVisibleItems, bDynamicScrollBar );
		
	SetPosition( rtListBox );

	m_SlotSetOptions = SigSetOptions().Connect(	this, &CListBox::OnSetOptions );
}

CListBox::~CListBox()
{
	NTL_DELETE( m_pScrollBar );
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

INT CListBox::GetItemCount(VOID) const
{
	return m_pImpl->GetItemCount();
}

const std::vector<CListBox_Item*>& CListBox::GetAllItems(VOID) const
{
	return m_pImpl->GetAllItems();
}

std::vector<CListBox_Item*> CListBox::GetSelectedItems(VOID) const
{
	return m_pImpl->GetSelectedItems();
}

std::vector<std::wstring> CListBox::GetSelectedItemsText(VOID) const
{
	return m_pImpl->GetSelectedItemsText();
}

std::vector<INT> CListBox::GetSelectedItemsIndex(VOID) const
{
	return m_pImpl->GetSelectedItemsIndex();
}

CListBox_Item* CListBox::GetSelectedItem(VOID) const
{
	return m_pImpl->GetSelectedItem();
}

std::wstring CListBox::GetSelectedItemText(VOID) const
{
	return m_pImpl->GetSelectedItemText();
}

INT CListBox::GetSelectedItemIndex(VOID) const
{
	return m_pImpl->GetSelectedItemIndex();
}

CListBox_Item* CListBox::GetItem( INT nIndex ) const
{
	return m_pImpl->GetItem( nIndex );
}

std::wstring CListBox::GetItemText( INT nIndex ) const
{
	return m_pImpl->GetItemText( nIndex );
}

bool CListBox::IsSelected( INT nIndex ) const
{
	return m_pImpl->IsSelected( nIndex );
}

bool CListBox::IsMultiSelection(VOID) const
{
	return m_pImpl->m_bMultiSelection;
}

INT CListBox::GetItemHeight(VOID) const
{
	return m_pImpl->m_nRowHeight;
}

INT CListBox::GetTopVisibleItem(VOID) const
{
	return m_pImpl->m_nOffsetRow;
}

INT CListBox::GetMaxVisibleItems(VOID) const
{
	return m_pImpl->m_nVisibleRowCount;
}

bool CListBox::HasScrollBar(VOID) const
{
	return m_pScrollBar->IsVisible();
}

std::list<CSurface>* CListBox::GetSurface(VOID)
{
	return &m_pImpl->m_listSurface;
}

CScrollBar* CListBox::GetScrollBar(VOID) const
{
	return m_pScrollBar;
}
/////////////////////////////////////////////////////////////////////////////
// Operations:

INT CListBox::AddItem( const CHAR* text, DWORD dwData /* = 0  */)
{
	return m_pImpl->AddItem( text, dwData );
}

INT CListBox::AddItem( const WCHAR* text, DWORD dwData /* = 0  */)
{
	return m_pImpl->AddItem( text, dwData );
}

INT CListBox::InsertItem( const CHAR* text, INT nIndex , DWORD dwData /* = 0  */)
{
	return m_pImpl->InsertItem( text, nIndex, dwData );
}

INT CListBox::InsertItem( const WCHAR* text, INT nIndex , DWORD dwData /* = 0  */)
{
	return m_pImpl->InsertItem( text, nIndex, dwData );
}

VOID CListBox::RemoveItem( INT nIndex )
{
	m_pImpl->RemoveItem( nIndex );
}

VOID CListBox::RemoveSelectedItems(VOID)
{
	m_pImpl->RemoveSelectedItems();
}

VOID CListBox::ClearAllItems(VOID)
{
	m_pImpl->ClearAll();
}

VOID CListBox::ClearSelection(VOID)
{
	m_pImpl->ClearSelection();
}

VOID CListBox::ChangeItem( const CHAR* text, INT nIndex, DWORD dwData /* = 0  */)
{
	m_pImpl->ChangeItem( text, nIndex, dwData );
}

VOID CListBox::ChangeItem( const WCHAR* text, INT nIndex, DWORD dwData /* = 0  */)
{
	m_pImpl->ChangeItem( text, nIndex, dwData );
}

VOID CListBox::SelectItem( INT nIndex, bool bSelect /* = true  */)
{
	m_pImpl->SelectAll( false );
	m_pImpl->SelectItem( nIndex, bSelect );	
}

VOID CListBox::SelectAll( bool bSelect /* = true  */)
{
	m_pImpl->SelectAll( bSelect );
}

VOID CListBox::InvertSelection(VOID)
{
	m_pImpl->InvertSelection();
}

VOID CListBox::SetMultiSelection( bool bEnable )
{
	m_pImpl->SetMultiSelection( bEnable );
}

VOID CListBox::SetItemHeight( INT nHeight )
{
	m_pImpl->SetItemHeight( nHeight );
}

VOID CListBox::SetMaxVisibleItems( INT nCount )
{
	m_pImpl->SetMaxVisibleItems( nCount );
}

VOID CListBox::SetTopItem( INT nIndex )
{
	m_pImpl->SetTopItem( nIndex );
}

VOID CListBox::Sort(bool bAscending)
{
	m_pImpl->Sort( bAscending );
}

VOID CListBox::SetTextStyle( DWORD dwStyle )
{
	m_pImpl->m_dwWidthSortStyle = dwStyle;
	m_pImpl->DrawItem();
}

VOID CListBox::SetSelectedMaskColor( BYTE uRed, BYTE uGreen, BYTE uBlue, BYTE uAlpha )
{
	m_pImpl->SetSelectedMaskColor( uRed, uGreen, uBlue, uAlpha );
}

VOID CListBox::SetSelectedTextColor( COLORREF color )
{
	m_pImpl->SetSelectedTextColor( color );
}

VOID CListBox::SetTextColor( COLORREF color )
{
	m_pImpl->SetTextColor( color );
}

VOID CListBox::SetBkColor( COLORREF color )
{
	m_pImpl->SetBkColor( color );
}

VOID CListBox::SetBkMode( INT nBkMode )
{
	m_pImpl->SetBkMode( nBkMode );
}

VOID CListBox::SetEffectMode( INT nMode )
{
	m_pImpl->m_nEffectMode = nMode;	
}

VOID CListBox::SetEffectColor( COLORREF color )
{
	m_pImpl->m_EffectColor = color;	
}

VOID CListBox::SetEffectValue( INT nValue )
{
	m_pImpl->m_nEffectValue = nValue;
}

VOID CListBox::AddSurface( CSurface surface )
{
	m_pImpl->AddSurface( surface );
}

VOID CListBox::AddMaskSurface( CSurface& surface )
{
	m_pImpl->AddMaskSurface( surface );
}

VOID CListBox::CreateFontStd( CHAR* pFaceName, INT nHeight, INT nAttributes )
{
	m_pImpl->CreateFont( pFaceName, nHeight, nAttributes );
}


/////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v1<int> &CListBox::SigHighlighted(VOID)
{
	return m_pImpl->m_SigHighlighted;
}

CSignal_v1<int> &CListBox::SigSelected(VOID)
{
	return m_pImpl->m_SigSelected;
}

CSignal_v0 &CListBox::SigSelectionChanged(VOID)
{
	return m_pImpl->m_SigSelectionChanged;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:
VOID CListBox::CalcSubRect( CRectangle* pScrollBar, INT nScrollBarWidth, INT nMarginX, INT nMarginY )
{
	CRectangle rtRect = GetClientRect();

	pScrollBar->left	= rtRect.right - nScrollBarWidth - nMarginX;
	pScrollBar->top		= rtRect.top + nMarginY;
	pScrollBar->right	= rtRect.right - nMarginX;
	pScrollBar->bottom	= rtRect.bottom - nMarginY;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CListBox::OnSetOptions( const CComponentOptions& options )
{
	CComponentOptions suboptions = options;
	suboptions.RemoveOption( "x" );
	suboptions.RemoveOption( "y" );
	suboptions.RemoveOption( "width" );
	suboptions.RemoveOption( "height" );

	CRectangle rtScrollBar;
	INT nScrollBarWidth = DEFAULT_SLIDER_WIDTH;;
	
	if( suboptions.Exists( "slider_width" ) )
		nScrollBarWidth = suboptions.GetValueAsInt( "slider_width" );
	
	CalcSubRect( &rtScrollBar, nScrollBarWidth, m_pImpl->m_nMarginX, m_pImpl->m_nMarginY );	

	suboptions.AddOption( "x", rtScrollBar.left );
	suboptions.AddOption( "y", rtScrollBar.top );
	suboptions.AddOption( "width", rtScrollBar.GetWidth() );
	suboptions.AddOption( "height", rtScrollBar.GetHeight() );	

	if( suboptions.Exists( "scroll_button_width" ) )
	{
		INT nButtonWidth = suboptions.GetValueAsInt( "scroll_button_width" );
		suboptions.RemoveOption( "scroll_button_width" );
		suboptions.AddOption( "button_width", nButtonWidth );		
	}
	
	if( suboptions.Exists( "scroll_button_height" ) )
	{
		INT nButtonHeight = suboptions.GetValueAsInt( "scroll_button_height" );
		suboptions.RemoveOption( "scroll_button_height" );
		suboptions.AddOption( "button_height", nButtonHeight );		
	}

	m_pScrollBar->SigSetOptions()( suboptions );
	m_pScrollBar->SetRange( 0, GetItemCount() - m_pImpl->m_nVisibleRowCount );

	if(options.Exists( "item" ) )
	{
		INT nCount = options.Count( "item" );
		for( INT i=0 ; i < nCount ; ++i )
			AddItem( options.GetValue( "item", i ).c_str() );
	}
}

END_GUI
