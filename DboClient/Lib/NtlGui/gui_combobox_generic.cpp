#include "gui_precomp.h"
#include "gui_combobox_generic.h"
#include "gui_renderer.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CComboBox_Generic::CComboBox_Generic( CComboBox* pSelf )
{
	m_pComboBox = pSelf;
	m_pComboBox->m_pListBox->Show( false );

	m_SlotAbsoluteClick = m_pComboBox->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CComboBox_Generic::OnAbsoluteClick );
	m_SlotStaticClick = m_pComboBox->m_pStaticBox->SigMouseDown().Connect( this, &CComboBox_Generic::OnClick );
	m_SlotButtonClick = m_pComboBox->m_pButton->SigMouseDown().Connect( this, &CComboBox_Generic::OnClick );

	m_SlotItemSelect = m_pComboBox->m_pListBox->SigSelected().Connect( this, &CComboBox_Generic::OnItemSelect );

	m_SlotSetOptions = m_pComboBox->SigSetOptions().Connect( this, &CComboBox_Generic::OnSetOption );
	m_SlotPaint = m_pComboBox->SigPaint().Connect( this, &CComboBox_Generic::OnPaint );
	m_SlotMove = m_pComboBox->SigMove().Connect( this, &CComboBox_Generic::OnMove );
	m_SlotResize = m_pComboBox->SigResize().Connect( this, &CComboBox_Generic::OnResize );
}

CComboBox_Generic::~CComboBox_Generic(VOID)
{
}

////////////////////////////////////////////////////////////////////////////////
// Operations

VOID CComboBox_Generic::AddSurface( CSurface surface )
{
	CRectangle rtScreen = m_pComboBox->GetScreenRect();
	CRectangle rtSnap = surface.m_SnapShot.rtRect;

	surface.m_SnapShot.rtRect.left	= rtScreen.left + rtSnap.left;	
	surface.m_SnapShot.rtRect.top	= rtScreen.top + rtSnap.top;
	surface.m_SnapShot.rtRect.right	= surface.m_SnapShot.rtRect.left + rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom= surface.m_SnapShot.rtRect.top + rtSnap.GetHeight();

	m_listSurface.push_back( surface );
}

////////////////////////////////////////////////////////////////////////////////
// Implementations

VOID CComboBox_Generic::ToggleList(VOID)
{
	CRectangle rtRect = m_pComboBox->GetPosition();

	if( m_pComboBox->m_pListBox->IsVisible() )
	{
		m_pComboBox->m_pListBox->Show( false );
		rtRect.bottom -= m_pComboBox->m_pListBox->GetHeight();
		m_SigListToggled( FALSE, m_pComboBox );
		m_pComboBox->Popup( false );
	}
	else
	{
		m_pComboBox->m_pListBox->Show( true );
		rtRect.bottom += m_pComboBox->m_pListBox->GetHeight();
		m_SigListToggled( TRUE, m_pComboBox );
		m_pComboBox->Popup( true );
	}

	m_pComboBox->SetPosition( rtRect );
}

////////////////////////////////////////////////////////////////////////////////
// Callbacks

VOID CComboBox_Generic::OnAbsoluteClick( const CKey& key )
{
	if( !m_pComboBox->IsVisible() )
		return;

	if( !m_pComboBox->m_pListBox->IsVisible() )
		return;

	if( m_pComboBox->PosInRect( (INT)key.m_fX, (INT)key.m_fY ) != CComponent::INRECT )
	{
		ToggleList();
	}	
}

VOID CComboBox_Generic::OnClick( const CKey& key )
{
	ToggleList();
}

VOID CComboBox_Generic::OnItemSelect( INT nIndex )
{
	m_pComboBox->SetText( m_pComboBox->GetItemText( nIndex ).c_str() );

	ToggleList();
	m_SigSelected( nIndex );
}

VOID CComboBox_Generic::OnSetOption( const CComponentOptions& options )
{
	if( options.Exists("surface_file") )
	{
		std::string file = options.GetValue( "surface_file" ); 
		CSurfaceManager* pSurfaceManager = m_pComboBox->GetSurfaceManager();
		if( pSurfaceManager )
		{
			INT nCount = options.Count("surface");
			for( INT i = 0 ; i < nCount ; ++i )
				AddSurface( pSurfaceManager->GetSurface( file, options.GetValue( "surface", i ) ) ); 
		}
	}
}

VOID CComboBox_Generic::OnPaint(VOID)
{
	CRectangle* pClippingRect = NULL;
	if( m_pComboBox->IsClipped() )	
	{
		pClippingRect = m_pComboBox->GetClippingRect();
	}

	std::list<CSurface>::iterator it;

	for( it = m_listSurface.begin() ; it!= m_listSurface.end() ; ++it )
	{
		g_GuiRenderer.RenderQueue( &(*it).m_SnapShot, (*it).m_pTexture, m_pComboBox->GetRenderTop(),
			(*it).m_fAngle, pClippingRect );		
	}
}

VOID CComboBox_Generic::OnMove( INT nOldX, INT nOldY )
{
	std::list<CSurface>::iterator it;
	CRectangle rtScreen = m_pComboBox->GetScreenRect();
	
	for( it = m_listSurface.begin() ; it != m_listSurface.end() ; ++it )
	{
		CRectangle* pOri = &(*it).m_Original.rtRect;
		CRectangle* pSnap= &(*it).m_SnapShot.rtRect; 
				
		pSnap->left  = rtScreen.left + pOri->left;
		pSnap->top	 = rtScreen.top + pOri->top;
		pSnap->right = pSnap->left + pOri->GetWidth();
		pSnap->bottom= pSnap->top + pOri->GetHeight();
	}
}

VOID CComboBox_Generic::OnResize( INT nOldCX, INT nOldCY )
{
	
}

END_GUI