#include "precomp_ntlpresentation.h"
#include "NtlPLCinematicObject.h"

// gui
#include "gui_renderer.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"
#include "NtlPLEventGenerator.h"


#define CLICKANI_DELTA_FROM_RB_X	76
#define CLICKANI_DELTA_FROM_RB_Y	37
#define BALLOON_DELTA_ORIGIN_RB_X	587
#define BALLOON_DELTA_ORIGIN_RB_Y	384
#define BALLOON_MARGIN_WIDTH		77
#define BALLOON_MARGIN_HEIGHT		83
#define CINEMATIC_NAME_POS_Y		677

#define CINIMATIC_BAR_HEIGHTRATE	0.16666667f // 768일때 

#define PCSELFOCUS_DELTA_FROM_SELBUTTON_X	5
#define CINEMATIC_BACK_ALPHA		100

#define PCMESSAGE_PCSELECT_WIDTH	671	
#define PCMESSAGE_PCSELECT_HEIGHT	25
#define PCMESSAGE_PCSELECT_OFFSET_Y	1
#define PCMESSAGE_PCSELECT_MIRGIN_X	60
#define PCMESSAGE_PCSELECT_POS_Y	8
#define PCMESSAGE_PCSELECT_WHOLE_HEIGHT 105

CNtlPLCinematicFlash::CNtlPLCinematicFlash( CRectangle rtPos, gui::CComponent* pParent, RwChar* pFilename )
{
	m_pFlash = NTL_NEW gui::CFlash( rtPos, pParent, GetNtlGuiManager()->GetSurfaceManager(), pFilename );
	NTL_ASSERT( m_pFlash, "Flash Object fail to allocate" );
	m_pFlash->SetResizeType( gui::CFlash::EXACTFIT );
	m_pFlash->SetFrameCallback( FALSE );
	m_pFlash->PlayMovie( TRUE );
}

CNtlPLCinematicFlash::~CNtlPLCinematicFlash(VOID)
{
	NTL_DELETE( m_pFlash );
}

RwBool CNtlPLCinematicFlash::Update( RwReal fElapsedTime )
{
	m_pFlash->Update(fElapsedTime);

	return TRUE;
}

#define CINEMATIC_IMAGE_X		456
#define CINEMATIC_IMAGE_Y		0
#define CINEMATIC_IMAGE_WIDTH	768
#define CINEMATIC_IMAGE_HEIGHT	768

CNtlPLCinematicObject* CNtlPLCinematicObject::m_pInstance = NULL;

CNtlPLCinematicObject::CNtlPLCinematicObject(VOID)
{
	m_fCurrentTime = 0.0f;	

	m_nCinematicUpperInPosY = 0;
	m_nCinematicUpperOutPosY = 0;
	m_nCinematicLowerInPosY = 0;
	m_nCinematicLowerOutPosY = 0;

	m_eBalloonType = BALLOON_TYPE_NORMAL;
	m_byPCSelect = 0;
	m_byPCSelectCount = 0;

	m_pUpdateCallback = NULL;

	m_pstbCanSkipHint = NULL;
}

CNtlPLCinematicObject::CNtlPLCinematicObject( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_fCurrentTime = 0.0f;

	m_nCinematicUpperInPosY = 0;
	m_nCinematicUpperOutPosY = 0;
	m_nCinematicLowerInPosY = 0;
	m_nCinematicLowerOutPosY = 0;

	m_eBalloonType = BALLOON_TYPE_NORMAL;

	m_byPCSelect = 0;

	m_pUpdateCallback = NULL;

	m_pstbCanSkipHint = NULL;
}

CNtlPLCinematicObject::~CNtlPLCinematicObject(VOID)
{

}

RwBool CNtlPLCinematicObject::CreateInstance(VOID)
{
	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CNtlPLCinematicObject( "CinematicObject" );
		NTL_ASSERT( m_pInstance, "CNtlPLCinematicObject::CreateInstance : Instance Allocate Fail" );

		if( !m_pInstance->Create() )
		{		
			CNtlPLCinematicObject::DeleteInstance();
			return FALSE;
		}		
		CNtlPLGuiManager::GetInstance()->AddGui( m_pInstance );
	}

	return TRUE;
}

VOID CNtlPLCinematicObject::DeleteInstance(VOID)
{
	if( m_pInstance )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pInstance );
		m_pInstance->Destroy();
		NTL_DELETE( m_pInstance );		
	}	
}

RwBool CNtlPLCinematicObject::Create(VOID)
{
	NTL_FUNCTION( "CNtlPLCinematicObject::Create" );

	if( !CNtlPLGui::Create( "gui\\Cinematic.rsr", "gui\\Cinematic.srf", "gui\\Cinematic.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pImageFrame = (gui::CFrame*)GetComponent( "frmImage" );
	m_pCinematicBack = (gui::CPanel*)GetComponent( "pnlCinematicBack" );
	m_pCinematicLower = (gui::CPanel*)GetComponent( "pnlCinematicLower" );
	m_pCinematicUpper = (gui::CPanel*)GetComponent( "pnlCinematicUpper" );
	m_pText = (gui::CStaticBox*)GetComponent( "stbDialogText" );
	m_pText->SetBkColor( RGB( 242, 236, 204 ) );
	m_pText->SetTextColor( RGB( 16, 16, 16 ) );
	m_pHtmlText = (gui::CHtmlBox*)GetComponent( "htmlDialogText" );
	m_pHtmlText->SetTextBackgroundColor( RGB( 242, 236, 204 ) );
	m_pHtmlText->SetLineSpace( 12 );
	m_pBalloon = (gui::CDialog*)GetComponent( "dlgBalloon" );
	m_pName = (gui::CStaticBox*)GetComponent( "stbName" );
	m_pNameBox = (gui::CDialog*)GetComponent( "dlgName" );

	m_pfrmPCFrame = (gui::CDialog*)GetComponent( "frmPCDialog" );
	m_phtmlPCMessage = (gui::CHtmlBox*)GetComponent( "htmlPCDialogText" );
	m_phtmlPCMessage->SetTextBackgroundColor( RGB( 242, 236, 204 ) );
	m_phtmlPCMessage->SetLineSpace( 12 );

	m_pdlgPCNameBar = (gui::CDialog*)GetComponent( "dlgPCName" );
	m_pstbPCName = (gui::CStaticBox*)GetComponent( "stbPCName" );
	m_pbtnPCSelect[0] = (gui::CButton*)GetComponent( "btn0" );
	m_pbtnPCSelect[1] = (gui::CButton*)GetComponent( "btn1" );
	m_pbtnPCSelect[2] = (gui::CButton*)GetComponent( "btn2" );
	m_pbtnPCSelect[3] = (gui::CButton*)GetComponent( "btn3" );
	m_ppnlPCSelectFocus = (gui::CPanel*)GetComponent( "pnlPCSelectFocus" );


	m_pstbCanSkipHint = NTL_NEW gui::CStaticBox(CRectangle(0, 0, 200, 50), m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pstbCanSkipHint->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE + 20, DEFAULT_FONT_ATTR);
	m_pstbCanSkipHint->SetTextColor(RGB(255, 168, 68));
	m_pstbCanSkipHint->Enable(false);

	m_pstbNameSkipped = NTL_NEW gui::COutputBox(CRectangle(0, 0, 200, 100), m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pstbNameSkipped->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbNameSkipped->SetTextColor(RGB(255, 255, 255));
	m_pstbNameSkipped->SetMaxLine(6);
	m_pstbNameSkipped->Enable(false);

	m_pCinematicBack->SetColor( 0, 0, 0 );
	m_pCinematicLower->SetColor( 0, 0, 0 );
	m_pCinematicUpper->SetColor( 0, 0, 0 );
	m_surImage.m_SnapShot.rtRect.SetRectWH( CINEMATIC_IMAGE_X, CINEMATIC_IMAGE_Y, CINEMATIC_IMAGE_WIDTH, CINEMATIC_IMAGE_HEIGHT );

	for( RwInt32 i = 0 ; i < CINEMATIC_MAX_PC_SELECT ; ++i )
	{
		m_pbtnPCSelect[i]->SetTextStyle( COMP_TEXT_LEFT | COMP_TEXT_VERTICAL_CENTER );
		m_pbtnPCSelect[i]->SetTextFocusColor( RGB( 255, 219, 71 ) );
		m_pbtnPCSelect[i]->SetTextDownCoordDiff( 0, 2 );
	}

	m_slotMove = m_pImageFrame->SigMove().Connect( this, &CNtlPLCinematicObject::OnMove );
	m_slotPaint = m_pImageFrame->SigPaint().Connect( this, &CNtlPLCinematicObject::OnPaint );

	FitSizeToScreen( GetNtlGuiManager()->GetGuiManager()->GetWidth(), GetNtlGuiManager()->GetGuiManager()->GetHeight() );

	LinkMsg( g_EventResize );

	Show( false );

	NTL_RETURN( TRUE );
}

VOID CNtlPLCinematicObject::Destroy(VOID)
{
	UnLinkMsg( g_EventResize );

	DeleteAllFlashObject();

	UnloadImageTexture();
	
	UnLinkUpdateCallBack();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CNtlPLCinematicObject::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	if( m_pUpdateCallback )
		m_pUpdateCallback->Call( (RwUInt32)( fElapsedTime * 1000 ) );

	std::vector<CNtlPLCinematicFlash*>::iterator it;

	for( it = m_vecFlash.begin() ; it != m_vecFlash.end() ; )
	{
		CNtlPLCinematicFlash* pFlash = (*it);
		
		if( !pFlash->Update( fElapsedTime ) )
		{
			NTL_DELETE( pFlash );
			it = m_vecFlash.erase( it );
			CNtlPLEventGenerator::CinematicFlashHide();
		}
		else
			++it;
	}
}

RwInt32 CNtlPLCinematicObject::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		RaiseTop();
		m_pThis->Popup( true );
	}
	else
	{
		m_pThis->Popup( false );
	}

	return 1;
}

VOID CNtlPLCinematicObject::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );

		FitSizeToScreen( pData->iWidht, pData->iHeight );
	}
}

gui::CButton* CNtlPLCinematicObject::GetPCSelect( RwUInt8 byIndex )
{
	NTL_ASSERT( byIndex >= 0 && byIndex < CINEMATIC_MAX_PC_SELECT, "CNtlPLCinematicObject::SetPCSelect : Invalid Select Index" );

	return m_pbtnPCSelect[byIndex];
}

VOID CNtlPLCinematicObject::CinematicFadeIn( RwBool bAlpha, RwBool bDirect )
{
	if( m_fCurrentTime < CINEMATIC_MOVETIME && !bDirect )
	{
		m_pCinematicLower->SetPosition( 0, (INT)gui::GetResultLineInterpolation( m_fCurrentTime, CINEMATIC_MOVETIME, (RwReal)m_nCinematicLowerOutPosY, (RwReal)m_nCinematicLowerInPosY ) );
		m_pCinematicUpper->SetPosition( 0, (INT)gui::GetResultLineInterpolation( m_fCurrentTime, CINEMATIC_MOVETIME, (RwReal)m_nCinematicUpperOutPosY, (RwReal)m_nCinematicUpperInPosY ) );		
															 
		if( bAlpha )
		{
			RwUInt8 ucAlpha = (RwUInt8)gui::GetResultLineInterpolation( m_fCurrentTime, CINEMATIC_MOVETIME, 0.0f, 255.0f );
			m_pCinematicLower->SetAlpha( ucAlpha );
			m_pCinematicUpper->SetAlpha( ucAlpha );
			m_pNameBox->SetAlpha( ucAlpha );
		}
	}
	else
	{
		m_pCinematicUpper->SetPosition( 0, m_nCinematicUpperInPosY );
		m_pCinematicLower->SetPosition( 0, m_nCinematicLowerInPosY );
		
		if( bAlpha )
		{
			m_pCinematicLower->SetAlpha( 255 );
			m_pCinematicUpper->SetAlpha( 255 );
			m_pNameBox->SetAlpha( 255 );
		}
	}	
}

VOID CNtlPLCinematicObject::CinematicFadeOut( RwBool bAlpha, RwBool bDirect )
{
	if( m_fCurrentTime < CINEMATIC_MOVETIME && !bDirect )
	{
		m_pCinematicLower->SetPosition( 0, (INT)gui::GetResultLineInterpolation( m_fCurrentTime, CINEMATIC_MOVETIME, (RwReal)m_nCinematicLowerInPosY, (RwReal)m_nCinematicLowerOutPosY ) );
		m_pCinematicUpper->SetPosition( 0, (INT)gui::GetResultLineInterpolation( m_fCurrentTime, CINEMATIC_MOVETIME, (RwReal)m_nCinematicUpperInPosY, (RwReal)m_nCinematicUpperOutPosY ) );		

		if( bAlpha )
		{
			RwUInt8 ucAlpha = (RwUInt8)gui::GetResultLineInterpolation( m_fCurrentTime, CINEMATIC_MOVETIME, 255.0f, 0.0f );
			m_pCinematicLower->SetAlpha( ucAlpha );
			m_pCinematicUpper->SetAlpha( ucAlpha );
			m_pNameBox->SetAlpha( ucAlpha );
		}
	}
	else
	{
		m_pCinematicUpper->SetPosition( 0, m_nCinematicUpperOutPosY );
		m_pCinematicLower->SetPosition( 0, m_nCinematicLowerOutPosY );

		if( bAlpha )
		{
			m_pCinematicLower->SetAlpha( 0 );
			m_pCinematicUpper->SetAlpha( 0 );
			m_pNameBox->SetAlpha( 0 );
		}
	}	
}

VOID CNtlPLCinematicObject::CinematicBackFadeIn( RwBool bDirect, RwReal fCurrentTime /* = 0.0f */ )
{
	if( fCurrentTime < CINEMATIC_MOVETIME && !bDirect )
	{
		RwUInt8 ucBackAlpha = (RwUInt8)gui::GetResultLineInterpolation( fCurrentTime, CINEMATIC_MOVETIME, 0.0f, (RwReal)CINEMATIC_BACK_ALPHA );
		m_pCinematicBack->SetAlpha( ucBackAlpha );
	}
	else
	{
		m_pCinematicBack->SetAlpha( CINEMATIC_BACK_ALPHA );
	}
}

VOID CNtlPLCinematicObject::CinematicBackFadeOut( RwBool bDirect, RwReal fCurrentTime /* = 0.0f */ )
{
	if( fCurrentTime < CINEMATIC_MOVETIME && !bDirect )
	{
		RwUInt8 ucBackAlpha = (RwUInt8)gui::GetResultLineInterpolation( fCurrentTime, CINEMATIC_MOVETIME, (RwReal)CINEMATIC_BACK_ALPHA, 0.0f );
		m_pCinematicBack->SetAlpha( ucBackAlpha );
	}
	else
	{
		m_pCinematicBack->SetAlpha( 0 );
	}
}

RwBool CNtlPLCinematicObject::IsCinematicShow(VOID)
{
	if( m_pCinematicUpper->GetPosition().top > m_nCinematicUpperOutPosY )
		return TRUE;

	return FALSE;
}

VOID CNtlPLCinematicObject::SetBalloonType( RwInt32 eBalloonType )
{
	//std::list<gui::CSurface>* pSurfaceList = m_pBalloon->GetSurface();
	//pSurfaceList->clear();

	//gui::CSurface surface;
	//CRectangle rtScreen = m_pBalloon->GetScreenRect();
	//CRectangle rtClient = m_pBalloon->GetClientRect();

	//// peessitemp 현재 말풍선이 하나임
	//if( eBalloonType == BALLOON_TYPE_NORMAL )
	//{
	//	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfBalloonType1Body" );
	//	surface.m_Original.rtRect.SetRectWH( rtClient.left, rtClient.top, surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetHeight() );
	//	surface.m_SnapShot.rtRect.SetRectWH( rtScreen.left, rtScreen.top, surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetHeight() );
	//	pSurfaceList->push_back( surface );
	//	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfBalloonType1Tail" );
	//	surface.m_Original.rtRect.SetRectWH( rtClient.left + 494, rtClient.top + 219, surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetHeight() );
	//	surface.m_SnapShot.rtRect.SetRectWH( rtScreen.left + 494, rtScreen.top + 219, surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetHeight() );
	//	pSurfaceList->push_back( surface );		
	//}
	//else
	//{
	//	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfBalloonType1Body" );
	//	surface.m_Original.rtRect.SetRectWH( rtClient.left, rtClient.top, surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetHeight() );
	//	surface.m_SnapShot.rtRect.SetRectWH( rtScreen.left, rtScreen.top, surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetHeight() );
	//	pSurfaceList->push_back( surface );
	//	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfBalloonType1Tail" );
	//	surface.m_Original.rtRect.SetRectWH( rtClient.left+ 494, rtClient.top + 219, surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetHeight() );
	//	surface.m_SnapShot.rtRect.SetRectWH( rtScreen.left+ 494, rtScreen.top + 219, surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetHeight() );
	//	pSurfaceList->push_back( surface );		
	//}
}

RwBool CNtlPLCinematicObject::SetImageTexture( RwChar* pFilename )
{
	return LoadImageTexture( pFilename );	
}

RwBool CNtlPLCinematicObject::SetImageTexture( RwChar* szPrefix, RwInt32 eState )
{
	RwChar buf[128];

	sprintf_s( buf, 128, "%s_%d.png", szPrefix, eState );

	return SetImageTexture( buf );
}

VOID CNtlPLCinematicObject::SetImageAlpha( RwUInt8 bAlpha )
{
	m_surImage.m_SnapShot.uAlpha = bAlpha;
}

VOID CNtlPLCinematicObject::ShowImage( RwBool bShow )
{
	m_bShowImage = bShow;
}

VOID CNtlPLCinematicObject::SetStaticText( const WCHAR* pText )
{
	m_pText->SetText( pText );

	RwInt32 nTextWidth = m_pText->GetWidth();
	RwInt32 nTextHeight = m_pText->GetHeight();
	
	RwInt32 nBalloonPosX = BALLOON_DELTA_ORIGIN_RB_X - nTextWidth - BALLOON_MARGIN_WIDTH;
	RwInt32 nBalloonPosY = BALLOON_DELTA_ORIGIN_RB_Y - nTextHeight - BALLOON_MARGIN_HEIGHT;

	CRectangle rtBalloon;
	rtBalloon.SetRectWH( nBalloonPosX, nBalloonPosY, nTextWidth + BALLOON_MARGIN_WIDTH, nTextHeight + BALLOON_MARGIN_HEIGHT );
	
	m_pBalloon->SetPosition( rtBalloon );
	
	//m_pClickAni->SetPosition( rtBalloon.GetWidth() - CLICKANI_DELTA_FROM_RB_X, rtBalloon.GetHeight() - CLICKANI_DELTA_FROM_RB_Y );
}

VOID CNtlPLCinematicObject::SetHtmlText( const WCHAR* pText, RwInt32 nSize )
{
	m_pHtmlText->SetHtmlFromMemory( pText, nSize );

	RwInt32 nTextWidth = m_pHtmlText->GetWidth();
	RwInt32 nTextHeight = m_pHtmlText->GetHeight();

	RwInt32 nBalloonPosX = BALLOON_DELTA_ORIGIN_RB_X - nTextWidth - BALLOON_MARGIN_WIDTH;
	RwInt32 nBalloonPosY = BALLOON_DELTA_ORIGIN_RB_Y - nTextHeight - BALLOON_MARGIN_HEIGHT;

	CRectangle rtBalloon;
	rtBalloon.SetRectWH( nBalloonPosX, nBalloonPosY, nTextWidth + BALLOON_MARGIN_WIDTH, nTextHeight + BALLOON_MARGIN_HEIGHT );

	m_pBalloon->SetPosition( rtBalloon );

	//m_pClickAni->SetPosition( rtBalloon.GetWidth() - CLICKANI_DELTA_FROM_RB_X, rtBalloon.GetHeight() - CLICKANI_DELTA_FROM_RB_Y );
}

VOID CNtlPLCinematicObject::SetName( const WCHAR* pName )
{
	m_pName->SetText( pName );
	//RwInt32 nWidth = m_pName->GetFitWidthToString();
	//m_pName->SetWidth( nWidth );
	//m_pNameBox->SetWidth( nWidth );

	//RwInt32 nImageWidth = m_surImage.m_SnapShot.rtRect.GetWidth();
	//
	//m_pNameBox->SetPosition( CINEMATIC_IMAGE_X + ( nImageWidth - nWidth  ) / 2, CINEMATIC_NAME_POS_Y );
}

VOID CNtlPLCinematicObject::SetFocusOnNPCBar( RwBool bFocusOnNPC )
{
	std::list<gui::CSurface>* pNPCSurfaceList = m_pNameBox->GetSurface();
	std::list<gui::CSurface>* pPCSurfaceList = m_pdlgPCNameBar->GetSurface();

	// peessi: 현재 Alpha값은 Surface만 저장되어 있으므로 문제. Alpha값을 저장하는 인터페이스를 만들지 검토.
	RwUInt8 byPCAlpha, byNPCAlpha;
	
	if( pNPCSurfaceList->empty() )
	{
		byNPCAlpha = 255;
	}
	else		
	{
		byNPCAlpha = pNPCSurfaceList->front().m_SnapShot.uAlpha;
		pNPCSurfaceList->clear();
	}

	if( pPCSurfaceList->empty() )
	{
		byPCAlpha = 255;
	}
	else
	{
		byPCAlpha = pPCSurfaceList->front().m_SnapShot.uAlpha;	
		pPCSurfaceList->clear();
	}	

	if( bFocusOnNPC )
	{
		m_pNameBox->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfNPCNameFocus_1" ) );
		m_pNameBox->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfNPCNameFocus_2" ) );
		m_pdlgPCNameBar->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfPCName_1" ) );
		m_pdlgPCNameBar->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfPCName_2" ) );
	}
	else
	{
		m_pNameBox->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfNPCName_1" ) );
		m_pNameBox->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfNPCName_2" ) );
		m_pdlgPCNameBar->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfPCNameFocus_1" ) );
		m_pdlgPCNameBar->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Cinematic.srf", "srfPCNameFocus_2" ) );
	}

	m_pNameBox->SetAlpha( byNPCAlpha );
	m_pdlgPCNameBar->SetAlpha( byPCAlpha );
}

VOID CNtlPLCinematicObject::SetPCMessage( const WCHAR* pText, RwInt32 nSize )
{
	m_phtmlPCMessage->SetHtmlFromMemory( pText, nSize );
}

VOID CNtlPLCinematicObject::SetPCName( const WCHAR* pName )
{
	m_pstbPCName->SetText( pName );
}

VOID CNtlPLCinematicObject::SetPCSelect( RwUInt8 byIndex, std::wstring strText )
{
	NTL_ASSERT( byIndex >= 0 && byIndex < CINEMATIC_MAX_PC_SELECT, "CNtlPLCinematicObject::SetPCSelect : Invalid Select Index" );

	m_pbtnPCSelect[byIndex]->SetText( strText );
}

VOID CNtlPLCinematicObject::SetPCSelectValue( RwUInt8 byIndex )
{
	gui::CButton* pSelButton = GetPCSelect( byIndex );

	const CRectangle& rtPosition = pSelButton->GetPosition();
	
	m_ppnlPCSelectFocus->SetPosition( rtPosition.left - m_ppnlPCSelectFocus->GetWidth() - PCSELFOCUS_DELTA_FROM_SELBUTTON_X,
									  rtPosition.top + ( pSelButton->GetHeight() - m_ppnlPCSelectFocus->GetHeight() ) / 2 );

	m_byPCSelect = byIndex;
}

VOID CNtlPLCinematicObject::SetPCSelectRegion( RwUInt8 byTotalCount, RwUInt8 byPCSelectRegionHeight, RwBool bShowProc )
{
	for( RwInt32 i = 0 ; i < CINEMATIC_MAX_PC_SELECT ; ++i )
	{
		gui::CButton* pSelectButton = GetPCSelect( i );

		if( i >= byTotalCount )
		{
			if( bShowProc )
			{
				pSelectButton->Show( false );
				pSelectButton->Enable( true );
			}
		}
		else
		{
			RwInt32 nSelectButtonPosY = PCMESSAGE_PCSELECT_POS_Y + byPCSelectRegionHeight / byTotalCount * i +
										( byPCSelectRegionHeight / byTotalCount - PCMESSAGE_PCSELECT_HEIGHT + PCMESSAGE_PCSELECT_OFFSET_Y ) / 2;
			CRectangle rect;
			rect.SetRectWH( PCMESSAGE_PCSELECT_MIRGIN_X, nSelectButtonPosY,	PCMESSAGE_PCSELECT_WIDTH, PCMESSAGE_PCSELECT_HEIGHT );

			pSelectButton->SetPosition( rect );
			if( bShowProc )
			{
				pSelectButton->Show( true );
				pSelectButton->Enable( true );
			}
		}
	}

	m_byPCSelectCount = byTotalCount;
}

VOID CNtlPLCinematicObject::DisableAllPCSelect(VOID)
{
	for( RwInt32 i = 0 ; i < CINEMATIC_MAX_PC_SELECT ; ++i )
	{
		gui::CButton* pSelectButton = GetPCSelect( i );
		pSelectButton->Enable( false );
	}
}

VOID CNtlPLCinematicObject::AddFlashObject( CNtlPLCinematicFlash* pFlash )
{
	m_vecFlash.push_back( pFlash );
}

VOID CNtlPLCinematicObject::DeleteAllFlashObject(VOID)
{
	std::vector<CNtlPLCinematicFlash*>::iterator it;

	for( it = m_vecFlash.begin() ; it != m_vecFlash.end() ; ++it )
	{
		CNtlPLCinematicFlash* pFlash = (*it);
		NTL_DELETE( pFlash );
	}

	m_vecFlash.clear();
}

RwBool CNtlPLCinematicObject::IsFlashObjectUpdated(VOID)
{
	if( m_vecFlash.size() == 0 )
		return FALSE;
	else
		return TRUE;	
}

VOID CNtlPLCinematicObject::InitShowSetting(VOID)
{
	// 각 Begin State에서 한번씩 호출 후 조정. This는 DialogManager에서 조정.
	m_pCinematicUpper->Show( true );
	m_pCinematicLower->Show( true );
	m_pHtmlText->Show( true );
	m_pText->Show( true );
	m_pName->Show( true );
	m_pImageFrame->Show( true );
	ShowImage( TRUE );
	m_pBalloon->Show( true );	

	// PC Dialog는 Quest에서만 사용하므로 기본적으로 Show False
	m_pfrmPCFrame->Show( false );
}

VOID CNtlPLCinematicObject::FitSizeToScreen( RwInt32 nWidth, RwInt32 nHeight )
{
	// 횡으로만 가운데 정렬, 종으로는 맨 밑에.
	RwInt32 nNewXPos, nNewYPos;	
	nNewXPos = ( nWidth - CINEMATIC_DEFAULT_WIDTH ) / 2;
	nNewYPos = nHeight - CINEMATIC_DEFAULT_HEIGHT;

	// 몇픽셀 정도의 오차를 피할수 없다.
	RwInt32 nBarHeight = (RwInt32)(nHeight * CINIMATIC_BAR_HEIGHTRATE + 0.5f);

	// Cinematic LowerBar Fix Pos
	CRectangle rtLowerBar = m_pCinematicLower->GetPosition();
	RwInt32 nBarYPos = rtLowerBar.bottom * nBarHeight / rtLowerBar.GetHeight();
	if( nBarYPos < nHeight )
		nBarYPos = nHeight;
	rtLowerBar.bottom = nBarYPos;
	rtLowerBar.top = nBarYPos - nBarHeight;
	m_pCinematicLower->SetPosition( rtLowerBar );

	CRectangle rtUpperBar = m_pCinematicUpper->GetPosition();
	nBarYPos = rtUpperBar.top * nBarHeight / rtUpperBar.GetHeight();
	if( nBarYPos > 0 )
		nBarYPos = 0;
	rtUpperBar.top = nBarYPos;
	rtUpperBar.bottom = nBarYPos + nBarHeight;
	m_pCinematicUpper->SetPosition( rtUpperBar );

	m_pThis->SetSize( nWidth, nHeight );
	m_pImageFrame->SetPosition( nNewXPos, nNewYPos );

	// Cinematic Bar Pos Factor
	m_nCinematicLowerInPosY = nHeight - nBarHeight;
	m_nCinematicLowerOutPosY = nHeight;
	m_nCinematicUpperInPosY = 0;
	m_nCinematicUpperOutPosY = -nBarHeight;

	// Cinematic Bar size
	std::list<gui::CSurface>* surfacelist = m_pCinematicLower->GetSurface();
	std::list<gui::CSurface>::iterator it;
	gui::CSurface* pSurface = NULL;

	for( it = surfacelist->begin() ; it != surfacelist->end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.rtRect.right = nWidth;			
		pSurface->m_Original.rtRect.right = nWidth;
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top + nBarHeight;
		pSurface->m_Original.rtRect.bottom = nBarHeight;
	}

	surfacelist = m_pCinematicUpper->GetSurface();
	for( it = surfacelist->begin() ; it != surfacelist->end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.rtRect.right = nWidth;
		pSurface->m_Original.rtRect.right = nWidth;
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top + nBarHeight;
		pSurface->m_Original.rtRect.bottom = nBarHeight;
	}

	surfacelist = m_pCinematicBack->GetSurface();
	for( it = surfacelist->begin() ; it != surfacelist->end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.rtRect.right = nWidth;
		pSurface->m_Original.rtRect.right = nWidth;
		pSurface->m_SnapShot.rtRect.bottom = nHeight;
		pSurface->m_Original.rtRect.bottom = nHeight;
	}

	// Flash
	std::vector<CNtlPLCinematicFlash*>::iterator itf;

	for( itf = m_vecFlash.begin() ; itf != m_vecFlash.end() ; ++itf )
	{
		CNtlPLCinematicFlash* pFlash = (*itf);
		pFlash->GetFlashObject()->SetSize( nWidth, nHeight );
	}	

	// PCMessage
	CRectangle rtPCFrame;
	rtPCFrame = m_pfrmPCFrame->GetPosition();
	rtPCFrame.SetRectWH( nNewXPos, nHeight - nBarHeight, rtPCFrame.GetWidth(), nBarHeight );
	m_pfrmPCFrame->SetPosition( rtPCFrame );

	RwInt32 nPCFrameTextRegionHeight = nBarHeight - 19;	// MagicalNumber : 이미지로 가리는 부분이 19 pixel 만큼.
	
	CRectangle rtPCMessage = m_phtmlPCMessage->GetPosition();
	RwInt32 nPCMessagePosY = ( nPCFrameTextRegionHeight - rtPCMessage.GetHeight() ) / 2 + 8; // MagicalNumber : 윗쪽 이미지는 8Pixel
	if( nPCMessagePosY < 8 )
		nPCMessagePosY = 8;
	m_phtmlPCMessage->SetPosition( rtPCMessage.left, nPCMessagePosY );
	SetPCSelectRegion( m_byPCSelectCount, nPCFrameTextRegionHeight, FALSE );
	SetPCSelectValue( m_byPCSelect );


	m_pstbCanSkipHint->SetPosition(nWidth / 2 - m_pstbCanSkipHint->GetWidth() / 2, 50);
	m_pstbNameSkipped->SetPosition(30, nHeight - 150);
}

RwBool CNtlPLCinematicObject::LoadImageTexture( RwChar* pFilename )
{
	std::string strFilename;

	strFilename = "illust\\";
	strFilename += pFilename;

	UnloadImageTexture();

	gui::CTexture* pTexture = g_TextureMgr.CreateTexture( strFilename.c_str() );

	if( !pTexture )
		return FALSE;


	m_surImage.m_pTexture = pTexture;
	return TRUE;
}

VOID CNtlPLCinematicObject::UnloadImageTexture(VOID)
{
	if( m_surImage.m_pTexture )
	{
		g_TextureMgr.DeleteTexture( m_surImage.m_pTexture );
		m_surImage.m_pTexture = NULL;
	}
}

VOID CNtlPLCinematicObject::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rect = m_pImageFrame->GetScreenRect();

	RwInt32 nWidth = m_surImage.m_SnapShot.rtRect.GetWidth();
	RwInt32 nHeight = m_surImage.m_SnapShot.rtRect.GetHeight();

	m_surImage.m_SnapShot.rtRect.SetRectWH( rect.left + CINEMATIC_IMAGE_X, rect.top + CINEMATIC_IMAGE_Y, nWidth, nHeight );	
}

VOID CNtlPLCinematicObject::OnPaint(VOID)
{
	if( m_bShowImage )
		g_GuiRenderer.RenderQueue( &m_surImage.m_SnapShot, m_surImage.m_pTexture, false, m_surImage.m_fAngle );	
}

