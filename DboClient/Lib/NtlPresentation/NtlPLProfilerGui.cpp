#include "precomp_ntlpresentation.h"
#include "NtlPLProfilerGui.h"

// core
#include "NtlDebug.h"
#include "NtlProfiler.h"

// gui
#include "gui_renderer.h"
#include "gui_define.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"
#include "NtlPLApi.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlProfilerItem
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CNtlPLProfilerItem::CNtlPLProfilerItem()
: m_pThis(NULL)
, m_pStbItemName(NULL)
, m_pStbPercentByParent(NULL)
, m_pStbMSecPerFrame(NULL)
, m_pStbMSecPerCall(NULL)
, m_pStbCallPerFrame(NULL)
{
	
}

/**
* \brief Destruction
*/
CNtlPLProfilerItem::~CNtlPLProfilerItem()
{

}

/**
* \brief Create
* \param pParentComp	(gui::CComponent*) 부모의 Component
* \param rect			(CRectangle&) Item이 생성될 영역
*/
VOID CNtlPLProfilerItem::Create( gui::CComponent* pParentComp, CRectangle& rect )
{
	// Dialog 생성
	m_pThis = NTL_NEW gui::CDialog( &rect, pParentComp, GetNtlGuiManager()->GetSurfaceManager() );

	// Dialog의 자식으로 등록될 Component들 생성
	CRectangle rectChild;
	rectChild.SetRectWH( 0, 0, 270, 20 );
	m_pStbItemName = NTL_NEW gui::CStaticBox( &rectChild, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_LEFT );
	m_pStbItemName->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );

	rectChild.SetRectWH( 270, 0, 75, 20 );
	m_pStbPercentByParent = NTL_NEW gui::CStaticBox( &rectChild, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbPercentByParent->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );

	rectChild.SetRectWH( 365, 0, 75, 20 );
	m_pStbMSecPerFrame = NTL_NEW gui::CStaticBox( &rectChild, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbMSecPerFrame->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );

	rectChild.SetRectWH( 435, 0, 75, 20 );
	m_pStbMSecPerCall = NTL_NEW gui::CStaticBox( &rectChild, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbMSecPerCall->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );

	rectChild.SetRectWH( 560, 0, 75, 20 );
	m_pStbCallPerFrame = NTL_NEW gui::CStaticBox( &rectChild, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbCallPerFrame->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );

	// 현재 Dialog는 입력을 받지 않는다. 입력은 CNtlPLProfilerGui 클래스에서 처리한다.
	m_pThis->Enable( false );
}

/**
* \brief Destroy
*/
VOID CNtlPLProfilerItem::Destroy()
{
	NTL_DELETE( m_pStbCallPerFrame );
	NTL_DELETE( m_pStbMSecPerCall );
	NTL_DELETE( m_pStbMSecPerFrame );
	NTL_DELETE( m_pStbPercentByParent );
	NTL_DELETE( m_pStbItemName );
	NTL_DELETE( m_pThis );
}

/**
* \brief 현재 아이템을 강조한다.
*/
VOID CNtlPLProfilerItem::FocusItem( RwBool bFocus /*= TRUE */ )
{
	m_pThis->GetSurface()->clear();

	if( bFocus )
	{
		gui::CSurface surface;
		SetSurface( surface, dPROFILER_GUI_ITEM_WIDTH,
			dPROFILER_GUI_ITEM_HEIGHT,
			0x00,
			0x00,
			0xff,
			128 );
		m_pThis->AddSurface( surface );
	}
}

/**
* \brief Data를 구조체로 채워넣는다.
* \param bLogged	(RwBool) 부모에게서 % 와 Total에서 % 이외의 값들의 유효 여부
*/
VOID CNtlPLProfilerItem::SetProfileData( SNtlProfileNode& sNode, RwBool bLogged /*= TRUE */ )
{
	if( strlen( sNode.acNodeName ) > 0 )
		m_pStbItemName->SetText( sNode.acNodeName );
	
	CHAR acBuffer[256];

	sprintf_s( acBuffer, 256, "%.4f", sNode.fPercentByParent );
	m_pStbPercentByParent->SetText( acBuffer );

	if( bLogged )
	{
		sprintf_s( acBuffer, 256, "%.4f", sNode.fMSecPerFrame );
		m_pStbMSecPerFrame->SetText( acBuffer );

		sprintf_s( acBuffer, 256, "%.4f", sNode.fMSecPerCall );
		m_pStbMSecPerCall->SetText( acBuffer );

		sprintf_s( acBuffer, 256, "%.4f", sNode.fCallPerFrame );
		m_pStbCallPerFrame->SetText( acBuffer );
	}
}

VOID CNtlPLProfilerItem::SetSurface( gui::CSurface& surface, RwInt32 nWidth, RwInt32 nHeight, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha )
{
	surface.m_Original.rtRect.SetRectWH( 0 , 0, nWidth, nHeight );
	surface.m_Original.uRed = byRed;
	surface.m_Original.uGreen = byGreen;
	surface.m_Original.uBlue = byBlue;
	surface.m_Original.uAlpha = byAlpha;
	surface.m_Original.UVs[0] = 0;
	surface.m_Original.UVs[1] = 0;
	surface.m_Original.UVs[2] = 1;
	surface.m_Original.UVs[3] = 1;
	surface.m_Original.uBlend = 0;

	surface.m_SnapShot.uRed = byRed;
	surface.m_SnapShot.uGreen = byGreen;
	surface.m_SnapShot.uBlue = byBlue;
	surface.m_SnapShot.uAlpha = byAlpha;
	surface.m_SnapShot.UVs[0] = 0;
	surface.m_SnapShot.UVs[1] = 0;
	surface.m_SnapShot.UVs[2] = 1;
	surface.m_SnapShot.UVs[3] = 1;
	surface.m_SnapShot.uBlend = 0;
	surface.m_SnapShot.rtRect.SetRectWH( 0 , 0, nWidth, nHeight );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlProfilerGui
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CNtlPLProfilerGui::CNtlPLProfilerGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_pStbTitle(NULL)
, m_pBtnUpperExplore(NULL)
, m_pBtnReset(NULL)
, m_pBtnClose(NULL)
, m_pStbNodeName(NULL)
, m_pStbParent(NULL)
, m_pStbMSecPerFrame(NULL)
, m_pStbMSecPerCall(NULL)
, m_pStbCallPerFrame(NULL)
, m_pDlgDataField(NULL)
, m_pProfileNode(NULL)
, m_pProfileItem(NULL)
, m_pScrollBar(NULL)
, m_uiSibblingCount(0)
, m_fElaspedTime(0.0f)
, m_byCurrentFocusItem(255)
, m_pCurrentIter(NULL)
, m_bScroll(FALSE)
{
	::ZeroMemory( &m_surPanel, sizeof( gui::CSurface ) );
}

/**
* \brief Destruction
*/
CNtlPLProfilerGui::~CNtlPLProfilerGui()
{
	CProfileManager::Release_Iterator( m_pCurrentIter );
}

/**
* \brief Create
*/
RwBool CNtlPLProfilerGui::Create()
{
	CRectangle rect;
	rect.SetRectWH( 0, 0, dPROFILER_GUI_WINDOW_WIDTH, dPROFILER_GUI_WINDOW_HEIGHT );

	// Dialog
	m_pThis = NTL_NEW gui::CDialog( &rect,  CNtlPLGuiManager::GetInstance()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager() );
	
	SetSurface( m_surPanel, dPROFILER_GUI_WINDOW_WIDTH, dPROFILER_GUI_WINDOW_HEIGHT,
		0, 0, 0, 70 );

	m_pThis->AddSurface( m_surPanel );

	rect.SetRectWH( 5, 5, 570, 20 );
	m_pThis->SetFrameSize( &rect );
	m_pThis->SetMovable( TRUE );

	// 현재 Dialog를 어떠한 상황에서도 입력을 받을 수 있게 우선순위를 가장 높게 설정한다.
	m_pThis->SetPriority( 65534 );

	// Title
	rect.SetRectWH( 5, 5, 300, 20 );
	m_pStbTitle = NTL_NEW gui::CStaticBox( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_LEFT );
	m_pStbTitle->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbTitle->Enable( FALSE );

	// Upper Button
	rect.SetRectWH( 570, 5, 20, 20 );
	m_pBtnUpperExplore = NTL_NEW gui::CButton( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), false );
	m_pBtnUpperExplore->SetTextFont( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pBtnUpperExplore->SetText( "U" );
	m_pBtnUpperExplore->SetTextUpColor( RGB(255, 255, 255) );
	m_pBtnUpperExplore->SetTextDownColor( RGB(255, 210, 70) );
	m_pBtnUpperExplore->SetTextFocusColor( RGB(255, 210, 70) );
	m_pBtnUpperExplore->SetTextDisableColor( RGB(128, 128, 128) );

	// Reset Button
	rect.SetRectWH( 593, 5, 20, 20 );
	m_pBtnReset = NTL_NEW gui::CButton( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), false );
	m_pBtnReset->SetTextFont( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pBtnReset->SetText( "R" );
	m_pBtnReset->SetTextUpColor( RGB(255, 255, 255) );
	m_pBtnReset->SetTextDownColor( RGB(255, 210, 70) );
	m_pBtnReset->SetTextFocusColor( RGB(255, 210, 70) );
	m_pBtnReset->SetTextDisableColor( RGB(128, 128, 128) );

	// Upper Button
	rect.SetRectWH( 616, 5, 20, 20 );
	m_pBtnClose = NTL_NEW gui::CButton( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), false );
	m_pBtnClose->SetTextFont( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pBtnClose->SetText( "X" );
	m_pBtnClose->SetTextUpColor( RGB(255, 255, 255) );
	m_pBtnClose->SetTextDownColor( RGB(255, 210, 70) );
	m_pBtnClose->SetTextFocusColor( RGB(255, 210, 70) );
	m_pBtnClose->SetTextDisableColor( RGB(128, 128, 128) );

	// NodeName
	rect.SetRectWH( 5, 28, 270, 20 );
	m_pStbNodeName = NTL_NEW gui::CStaticBox( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbNodeName->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbNodeName->SetText("NodeName");

	// by parent
	rect.SetRectWH( 270, 28, 75, 20 );
	m_pStbParent = NTL_NEW gui::CStaticBox( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbParent->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbParent->SetText("%parent");

	// ms/f
	rect.SetRectWH( 365, 28, 75, 20 );
	m_pStbMSecPerFrame = NTL_NEW gui::CStaticBox( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbMSecPerFrame->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbMSecPerFrame->SetText("ms/f");

	// ms/call
	rect.SetRectWH( 435, 28, 75, 20 );
	m_pStbMSecPerCall = NTL_NEW gui::CStaticBox( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbMSecPerCall->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbMSecPerCall->SetText("ms/call");

	// calls/f
	rect.SetRectWH( 560, 28, 75, 20 );
	m_pStbCallPerFrame = NTL_NEW gui::CStaticBox( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(),
		COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbCallPerFrame->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbCallPerFrame->SetText("calls/f");

	// Dialog
	rect.SetRectWH( 5, 50, 630, 175 );
	m_pDlgDataField = NTL_NEW gui::CDialog( &rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );

	// Create Scroll Bar
	// Todo : 스크롤 바 작업해야함 ( 2008. 6. 23 )
	rect.SetRectWH( m_pDlgDataField->GetWidth() - 5, 0, 5, m_pDlgDataField->GetHeight() );
	m_pScrollBar = NTL_NEW gui::CScrollBar( rect, 0, 10, false, m_pDlgDataField, GetNtlGuiManager()->GetSurfaceManager() );
	SetSurface( m_surBar, 5, 175, 0x66, 0x33, 0x00, 128);
	m_pScrollBar->AddSurfaceLayout( m_surBar );
	SetSurface( m_surAnchor, 5, 5, 0xCC, 0xFF, 0xFF, 128 );
	m_pScrollBar->AddSurfaceSlider( m_surAnchor );
	m_pScrollBar->SetSliderSize( 5, 5 );
	m_pScrollBar->SetRange( 0, 0 );
	m_pScrollBar->SetValue( 0 );
	
	gui::CSurface surface;
	SetSurface( surface, 5, 5, 0x66, 0x66, 0x99, 255 );
	m_pScrollBar->GetDecreaseButton()->AddSurfaceUp( surface );
	SetSurface( surface, 5, 5, 0x66, 0xFF, 0x99, 255 );
	m_pScrollBar->GetDecreaseButton()->AddSurfaceDown( surface );
	SetSurface( surface, 5, 5, 0x66, 0xCC, 0x99, 255 );
	m_pScrollBar->GetDecreaseButton()->AddSurfaceFocus( surface );
	SetSurface( surface, 5, 5, 0x66, 0x66, 0x99, 255 );
	m_pScrollBar->GetIncreaseButton()->AddSurfaceUp( surface  );
	SetSurface( surface, 5, 5, 0x66, 0xFF, 0x99, 255 );
	m_pScrollBar->GetIncreaseButton()->AddSurfaceDown( surface );
	SetSurface( surface, 5, 5, 0x66, 0xCC, 0x99, 255 );
	m_pScrollBar->GetIncreaseButton()->AddSurfaceFocus( surface );

	m_pScrollBar->SetButtonSize(5, 5);
	m_pScrollBar->SetSliderSize(5, 5);

	m_pScrollBar->Show( true );

	m_slotScrollChanged = m_pScrollBar->SigValueChanged().Connect( this, &CNtlPLProfilerGui::OnScrollChanged );
	m_slotScrollMoved = m_pScrollBar->SigSliderMoved().Connect( this, &CNtlPLProfilerGui::OnScrollChanged );

	// Signals
	m_slotClickedBtnUpperExplore = m_pBtnUpperExplore->SigClicked().Connect( this, &CNtlPLProfilerGui::OnClickedBtnUpperExplore );
	m_slotClickedBtnReset = m_pBtnReset->SigClicked().Connect( this, &CNtlPLProfilerGui::OnClickedBtnReset );
	m_slotClickedBtnClose = m_pBtnClose->SigClicked().Connect( this, &CNtlPLProfilerGui::OnClickedBtnClose );
	m_slotMouseUp = m_pDlgDataField->SigMouseUp().Connect( this, &CNtlPLProfilerGui::OnMouseUp );
	m_slotMouseDown = m_pDlgDataField->SigMouseDown().Connect( this, &CNtlPLProfilerGui::OnMouseDown );
	m_slotMouseMove = m_pDlgDataField->SigMouseMove().Connect( this, &CNtlPLProfilerGui::OnMouseMove );

	// Link
	LinkMsg( g_EventShowProfilerGui, 0 );

	Show(false);
	
	return TRUE;
}

/**
* \brief 업데이트를 한다. dPROFILER_GUI_UPDATE_TIME 마다 Data를 가져오고 GUI를 업데이트한다.
* \param fElapsed		(RwReal) 이전 프레임에서의 경과시간
*/
VOID CNtlPLProfilerGui::Update( RwReal fElapsed )
{
	m_fElaspedTime += fElapsed;

	if( m_fElaspedTime > dPROFILER_GUI_UPDATE_TIME )
	{
		m_fElaspedTime = 0.0f;

		UpdateData();
		UpdatePanel();
	}
}

/**
* \brief Destroy
*/
VOID CNtlPLProfilerGui::Destroy()
{
	UnLinkMsg( g_EventShowProfilerGui );

	DestroyProfilerItems();

	NTL_DELETE( m_pStbParent );
	NTL_DELETE( m_pStbMSecPerFrame );
	NTL_DELETE( m_pStbMSecPerCall );
	NTL_DELETE( m_pStbCallPerFrame );
	NTL_DELETE( m_pScrollBar );
	NTL_DELETE( m_pDlgDataField );
	NTL_DELETE( m_pStbNodeName );
	NTL_DELETE( m_pBtnClose );
	NTL_DELETE( m_pBtnReset );
	NTL_DELETE( m_pBtnUpperExplore );
	NTL_DELETE( m_pStbTitle );
	NTL_DELETE( m_pThis );
}

/**
* \brief HandleEvents
* \param msg	(RWS::CMsg&) 이벤트 메시지 구조체
*/
VOID CNtlPLProfilerGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventShowProfilerGui )
	{
		SNtlEventShowProfilerGui* pShowProfilerGui = reinterpret_cast<SNtlEventShowProfilerGui*>( msg.pData );

		ShowDialog(pShowProfilerGui->bShow);
	}
}

/**
* \brief Open/Close 할 때 해줘야 할 것들을 처리한다.
*/
VOID CNtlPLProfilerGui::ShowDialog( RwBool bShow /*= TRUE*/ )
{
    Show(B2b(bShow));

	if( bShow )
	{
		GetNtlGuiManager()->AddUpdateFunc( this );

		GetCurrentProfileData();

		/*m_pThis->SetPosition( GetNtlGuiManager()->GetGuiManager(*/
		////GetNtlGuiManager()->

		m_pScrollBar->Show( true );
	}
	else
	{
		GetNtlGuiManager()->RemoveUpdateFunc( this );
	}
}

/**
* \brief 현재 노드의 반복자를 자식 노드로 이동하고 데이타를 업데이트한다.
* \param nIdx	(RwInt32) 자식 노드의 Index
*/
RwBool CNtlPLProfilerGui::EnterChild( RwInt32 nIdx )
{
	m_pCurrentIter->Enter_Child( nIdx );

	GetCurrentProfileData();

	return TRUE;
}

/**
* \brief 현재 노드의 반복자를 부모 노드로 이동하고 데이타를 업데이트한다.
*/
RwBool CNtlPLProfilerGui::EnterParent()
{
	m_pCurrentIter->Enter_Parent();

	GetCurrentProfileData();

	return TRUE;
}

/**
* \brief 마우스의 좌표값으로 현재 위치한 아이템을 강조한다.
* \param nFlags		(RwInt32) 마우스의 상태
* \param nX			(RwInt32) X Pos
* \param nY			(RwInt32) Y Pos
*/
VOID CNtlPLProfilerGui::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	for( RwUInt32 i = 0; i < m_uiSibblingCount - m_pScrollBar->GetValue() ; ++i )
	{
		if( m_pProfileItem[i].GetScreenRect().PtInRect( 
			m_pDlgDataField->GetScreenRect().left + nX, m_pDlgDataField->GetScreenRect().top + nY ) )
			m_pProfileItem[i].FocusItem( TRUE );
		else
			m_pProfileItem[i].FocusItem( FALSE );
	}
}

/**
* \brief OnMouseDown
* \param key	(const CKey&) 마우스의 정보 구조체
*/
VOID CNtlPLProfilerGui::OnMouseDown( const CKey& key )
{
	// 마우스의 왼쪽 버튼이 Down 되었을 때 현재 Down된 곳에 위치한 Item이 있다면 그 아이템의
	// Index를 기억한다.
	if( key.m_nID == UD_LEFT_BUTTON )
	{
		CRectangle rect = m_pDlgDataField->GetScreenRect();

		for( RwUInt32 i = 0; i < m_uiSibblingCount - m_pScrollBar->GetValue() ; ++i )
		{
			if( m_pProfileItem[i].GetScreenRect().PtInRect( rect.left + (RwInt32)key.m_fX,
				rect.top + (RwInt32)key.m_fY ) )
			{
				m_byCurrentFocusItem = i + m_pScrollBar->GetValue();

				break;
			}
		}
	}
}

/**
* \brief OnMouseUp
* \param key	(const CKey&) 마우스의 정보 구조체
*/
VOID CNtlPLProfilerGui::OnMouseUp( const CKey& key )
{
	// 마우스가 Up 처리되었을 때 현재 위치한 곳이 아이템이 있는 곳이고 Down 되었을 때의 Index와 일치한다면
	// 현재 위치한 Item의 노드로 반복자를 이동하고 데이타를 업데이트 시킨다.
	if( key.m_nID == UD_LEFT_BUTTON )
	{
		for( RwUInt32 i = 0; i < m_uiSibblingCount - m_pScrollBar->GetValue() ; ++i )
		{
			if( m_pProfileItem[i].GetScreenRect().PtInRect( m_pDlgDataField->GetScreenRect().left + (RwInt32)key.m_fX,
				m_pDlgDataField->GetScreenRect().top + (RwInt32)key.m_fY ) )
			{
				if( m_byCurrentFocusItem == i + m_pScrollBar->GetValue() )
				{
					EnterChild( m_pScrollBar->GetValue() + i );
					
					m_byCurrentFocusItem = 255;

					break;
				}
			}
		}
	}
}

/**
* \brief m_pBtnUpperExplore를 클릭했을 때
*/
VOID CNtlPLProfilerGui::OnClickedBtnUpperExplore( gui::CComponent* pComponent )
{
	EnterParent();
}

/**
* \brief m_pBtnReset을 클릭했을 때
*/
VOID CNtlPLProfilerGui::OnClickedBtnReset( gui::CComponent* pComponent )
{
	CProfileManager::Reset();
}

/**
* \brief m_pBtnClose를 클릭했을 때
*/
VOID CNtlPLProfilerGui::OnClickedBtnClose( gui::CComponent* pComponent )
{
	ShowDialog( FALSE );
}

/**
* \brief 현재 반복자가 위치한 곳에서 정보를 가져온다.
*/
VOID CNtlPLProfilerGui::GetCurrentProfileData()
{
	if(!m_pCurrentIter)
		m_pCurrentIter = CProfileManager::Get_Iterator();

	DestroyProfilerItems();

	m_pCurrentIter->First();
	sprintf_s( m_acParentName, dPROFILER_GUI_PARENT_NAME_BUFFER, "%s", m_pCurrentIter->Get_Current_Parent_Name() );
	
	// 현재 반복자의 Sibbling 노드의 갯수를 센다.
	m_uiSibblingCount = 0;
	while( !m_pCurrentIter->Is_Done() )
	{
		m_uiSibblingCount++;
		m_pCurrentIter->Next();
	}

	// Count에 맞춰서 GUI 항목과 구조체를 생성
	CreateProfilerItems( m_uiSibblingCount );

	// UI의 Scroll Bar 속성을 조정한다.
	AdjustPanelToItems();

	// NtlProfiler에게 Data를 가져온다.
	UpdateData();

	// GUI 에 Data들을 업데이트 한다.
	UpdatePanel();
}

/**
* \brief 노드의 정보를 저장할 구조체와 GUI Item을 생성한다.
* \param uiItemsCount	(RwUInt32) 아이템의 갯수(현재 반복자의 자식 노드들의 갯수)
*/
VOID CNtlPLProfilerGui::CreateProfilerItems(RwUInt32 uiItemsCount)
{
	m_pProfileNode = NTL_NEW SNtlProfileNode[uiItemsCount+1];
	::ZeroMemory( m_pProfileNode, sizeof(SNtlProfileNode) * (uiItemsCount+1) );
	
	m_pProfileItem = NTL_NEW CNtlPLProfilerItem[uiItemsCount+1];
	for( RwUInt32 i = 0; i < uiItemsCount+1; ++i )
	{
		m_pProfileItem[i].Create( m_pDlgDataField, CRectangle( 0, i*dPROFILER_GUI_ITEM_HEIGHT, dPROFILER_GUI_ITEM_WIDTH, (i*dPROFILER_GUI_ITEM_HEIGHT) + dPROFILER_GUI_ITEM_HEIGHT ) );
	}
}

/**
* \brief 동적으로 생성된 정보 구조체와 GUi Item들을 해제한다.
*/
VOID CNtlPLProfilerGui::DestroyProfilerItems()
{
	NTL_ARRAY_DELETE( m_pProfileNode );

	if( m_pProfileItem )
	{
		for( RwUInt32 i = 0; i < m_uiSibblingCount+1; ++i )
		{
			m_pProfileItem[i].Destroy();
		}
	}

	NTL_ARRAY_DELETE( m_pProfileItem );
}

/**
* \brief 현재 반복자가 위치한 Node들의 정보를 가져온다.
*/
VOID CNtlPLProfilerGui::UpdateData()
{
	if(!m_pCurrentIter)
	{
		m_pCurrentIter = CProfileManager::Get_Iterator();
	}

	RwUInt32	uiIndex = 0;
	RwReal		MSecPerCall4Parent;

	// get parent profile data
	if(!m_pCurrentIter->Get_Current_Parent_Total_Calls())
	{
		MSecPerCall4Parent = 0.0000000f;
	}
	else
	{
		MSecPerCall4Parent = m_pCurrentIter->Get_Current_Parent_Total_Time() / static_cast<RwReal>(m_pCurrentIter->Get_Current_Parent_Total_Calls()) * 1000.0f;
	}
	 
	m_pCurrentIter->First();
	RwReal fPerFromParent = 0.0f;
	while(!m_pCurrentIter->Is_Done())
	{
		m_pProfileNode[uiIndex].uiIndex = uiIndex;

		// current node's name
		sprintf_s( m_pProfileNode[uiIndex].acNodeName, dPROFILER_GUI_NAME_BUFFER, "%s", m_pCurrentIter->Get_Current_Name() );

		// millisecond  per frame
		m_pProfileNode[uiIndex].fMSecPerFrame = m_pCurrentIter->Get_Current_Total_Time() / static_cast<RwReal>(CProfileManager::Get_Frame_Count_Since_Reset()) * 1000.0f;

		// millisecond  per call
		m_pProfileNode[uiIndex].fMSecPerCall = m_pCurrentIter->Get_Current_Total_Time() / static_cast<RwReal>(m_pCurrentIter->Get_Current_Total_Calls()) * 1000.0f;

		// call per frame
		m_pProfileNode[uiIndex].fCallPerFrame = static_cast<RwReal>(m_pCurrentIter->Get_Current_Total_Calls()) / static_cast<RwReal>(CProfileManager::Get_Frame_Count_Since_Reset());

		// current child node's % of the parent
		if(!MSecPerCall4Parent)
		{
			m_pProfileNode[uiIndex].fPercentByParent = 0.0f;
		}
		else
		{
			m_pProfileNode[uiIndex].fPercentByParent = m_pProfileNode[uiIndex].fMSecPerCall / MSecPerCall4Parent * 100.0f; 
			fPerFromParent += m_pProfileNode[uiIndex].fPercentByParent;
		}

		m_pCurrentIter->Next();

		uiIndex++;
	}

	// UnLogged
	sprintf_s( m_pProfileNode[uiIndex].acNodeName, dPROFILER_GUI_NAME_BUFFER, "%s", "UnLogged" );
	m_pProfileNode[uiIndex].uiIndex = uiIndex;

	// Todo : Unlogged 된 것들의 수행시간을 넣어야 한다.
	if( MSecPerCall4Parent )
		m_pProfileNode[uiIndex].fPercentByParent = 100.0f - fPerFromParent;
	else
		m_pProfileNode[uiIndex].fPercentByParent = 0.0f;
}

/**
* \brief GUI 를 갱신
*/
VOID CNtlPLProfilerGui::UpdatePanel()
{
	if( m_pProfileItem == NULL ||
		m_pProfileNode == NULL )
		return;

	m_pStbTitle->SetText( m_acParentName );

	
	RwUInt32 uiIndex = m_pScrollBar->GetValue();
	for( RwUInt32 i = 0; i < dPROFILER_GUI_CURRENT_VIEW_NUMS; ++i, ++uiIndex )
	{
		if( m_uiSibblingCount > uiIndex )
		{
			m_pProfileItem[i].SetProfileData( m_pProfileNode[uiIndex] );
		}
		else if ( m_uiSibblingCount == uiIndex )
		{
			m_pProfileItem[i].SetProfileData( m_pProfileNode[uiIndex], FALSE );
		}
	}
}

VOID CNtlPLProfilerGui::AdjustPanelToItems()
{
	if( (RwInt32)m_uiSibblingCount+1 >= 10 )
	{
		m_pScrollBar->SetRange( 0, 10 - (m_uiSibblingCount) );
		m_bScroll = TRUE;
	}
	else
	{
		m_pScrollBar->SetRange( 0, 0 );
		m_bScroll = FALSE;
	}
}

VOID CNtlPLProfilerGui::SetSurface( gui::CSurface& surface, RwInt32 nWidth, RwInt32 nHeight,
								   RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha)
{
	surface.m_Original.rtRect.SetRectWH( 0 , 0, nWidth, nHeight );
	surface.m_Original.uRed = byRed;
	surface.m_Original.uGreen = byGreen;
	surface.m_Original.uBlue = byBlue;
	surface.m_Original.uAlpha = byAlpha;
	surface.m_Original.UVs[0] = 0;
	surface.m_Original.UVs[1] = 0;
	surface.m_Original.UVs[2] = 1;
	surface.m_Original.UVs[3] = 1;
	surface.m_Original.uBlend = 0;

	surface.m_SnapShot.uRed = byRed;
	surface.m_SnapShot.uGreen = byGreen;
	surface.m_SnapShot.uBlue = byGreen;
	surface.m_SnapShot.uAlpha = byAlpha;
	surface.m_SnapShot.UVs[0] = 0;
	surface.m_SnapShot.UVs[1] = 0;
	surface.m_SnapShot.UVs[2] = 1;
	surface.m_SnapShot.UVs[3] = 1;
	surface.m_SnapShot.uBlend = 0;
	surface.m_SnapShot.rtRect.SetRectWH( 0 , 0, nWidth, nHeight );
}

VOID CNtlPLProfilerGui::OnScrollChanged( RwInt32 nNewOffset )
{
	UpdatePanel();
}