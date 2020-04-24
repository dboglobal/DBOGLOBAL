#include "precomp_dboclient.h"
#include "TBCommonDirectionGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlWorldConceptTB.h"
#include "NtlCameraManager.h"

// client
#include "DboGlobal.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stCOMMONINFO

VOID stCOMMONINFO::SizePosCalculation( gui::CComponent* pComponent, CRectangle rtOriginal, RwInt32 nScreenWidth, RwInt32 nScreenHeight, RwBool bResize /* = FALSE  */ )
{
	CRectangle rtRect;

	if( bResize )
	{
		rtRect.SetRectWH( nScreenWidth * rtOriginal.left / TB_DIRECT_BASIS_WIDTH,
						  nScreenHeight * rtOriginal.top / TB_DIRECT_BASIS_HEIGHT,
						  nScreenWidth * rtOriginal.GetWidth() / TB_DIRECT_BASIS_WIDTH,
						  nScreenHeight * rtOriginal.GetHeight() / TB_DIRECT_BASIS_HEIGHT );
	}
	else
	{
		rtRect.SetRectWH( ( nScreenWidth - rtOriginal.GetWidth() ) / 2,
						  ( nScreenHeight - rtOriginal.GetHeight() ) / 2,
							rtOriginal.GetWidth(), rtOriginal.GetHeight() );
	}

	pComponent->SetPosition( rtRect );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stSCROLLINFO

VOID stSCROLLINFO::Create( gui::CFlash* pScroll, gui::CHtmlBox* pText )
{
	if( pScroll )
	{
		bValid = TRUE;
		pflaScroll = pScroll;
		phtmlText = pText;
		rtOriginalScroll = pScroll->GetPosition();

		slotScrollMovieEnd = pflaScroll->SigMovieEnd().Connect( this, &stSCROLLINFO::OnScrollMovieEnd );
		slotScrollFSCallBack = pflaScroll->SigFSCallBack().Connect( this, &stSCROLLINFO::OnScrollFSCallBack );

		pflaScroll->Show( false );
		phtmlText->Show( false );
	}
}

VOID stSCROLLINFO::Destroy(VOID)
{
	pflaScroll->SigMovieEnd().Disconnect( slotScrollMovieEnd );
	pflaScroll->SigFSCallBack().Disconnect( slotScrollFSCallBack );

	bValid = FALSE;
	pflaScroll = NULL;	
}

VOID stSCROLLINFO::Update( RwReal fElapsedTime )
{
	if( bValid )
	{
		pflaScroll->Update( fElapsedTime );
		phtmlText->Update( fElapsedTime );
	}
}

VOID stSCROLLINFO::StartMovie(VOID)
{
	if( bValid )
	{
		pflaScroll->Show( true );
		pflaScroll->RestartMovie();
	}
}

VOID stSCROLLINFO::EndMovie(VOID)
{
	if( bValid )
	{
		pflaScroll->Show( false );	
		pflaScroll->PlayMovie( FALSE );
	}
}

VOID stSCROLLINFO::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	if( IsValid() )
		SizePosCalculation( pflaScroll, rtOriginalScroll, nScreenWidth, nScreenHeight );		
}

VOID stSCROLLINFO::SetHtmlText( const RwChar* szHtmlFileName )
{
	if( bValid )
		phtmlText->SetHtmlFromFile( szHtmlFileName );			
}

VOID stSCROLLINFO::OnScrollMovieEnd( gui::CComponent* pComponent )
{
	EndMovie();
}

VOID stSCROLLINFO::OnScrollFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "TextStart" ) )
	{
		//phtmlText->Show( true );
		//phtmlText->SetPlayPage( 0 );
		//phtmlText->SetIntervalTextEnable( 0.01f );		

		pflaScroll->Invoke( "CloseScroll", "" );
	}
	else if( !strcmp( szCommand, "TextEnd" ) )
	{
		phtmlText->SetIntervalTextDisable();
		phtmlText->Show( false );
	}
}

VOID stSCROLLINFO::OnScrollPageDone( RwInt32 nPage )
{
	// 마지막 페이지가 끝났을 때
	if( nPage + 1 >= phtmlText->GetIntervalPageNums() )
		pflaScroll->Invoke( "CloseScroll", "" );
	else
		phtmlText->SetNextPage();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stONELINE

VOID stONELINE::Create( gui::CFlash* pFirst )
{
	if( pFirst )
	{
		bValid = TRUE;
		pflaFirst = pFirst;		
		rtOriginalFirst = pflaFirst->GetPosition();

		slotFirstMovieEnd = pflaFirst->SigMovieEnd().Connect( this, &stONELINE::OnFirstMovieEnd );
	}	
}

VOID stONELINE::Destroy(VOID)
{
	pflaFirst->SigMovieEnd().Disconnect( slotFirstMovieEnd );		

	bValid = FALSE;
	pflaFirst = NULL;
	byObjectID = INVALID_BYTE;		
}

VOID stONELINE::LoadFlash( const RwChar* szFirst, RwUInt8 byID )
{
	if( bValid )
	{
		pflaFirst->Load( szFirst, TRUE );

		ReCalcPosition( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );

		byObjectID = byID;
		EndMovie();
	}	
}

VOID stONELINE::Update( RwReal fElapsedTime )
{
	if( bValid && pflaFirst->IsPlayMovie())
		pflaFirst->Update( fElapsedTime );	
}

VOID stONELINE::StartMovie(VOID)
{
	if( bValid )
	{
		pflaFirst->Show( true );
		pflaFirst->RestartMovie();
	}
}

VOID stONELINE::EndMovie(VOID)
{
	if( bValid )
	{
		pflaFirst->Show( false );
		pflaFirst->PlayMovie( FALSE );
	}
}

VOID stONELINE::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	if( IsValid() )
		SizePosCalculation( pflaFirst, rtOriginalFirst, nScreenWidth, nScreenHeight );		
}

VOID stONELINE::OnFirstMovieEnd( gui::CComponent* pComponent )
{
	EndMovie();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stTWOLINE

VOID stTWOLINE::Create( gui::CFlash* pFirst, gui::CFlash* pSecond )
{
	if( pFirst && pSecond )
	{
		bValid = TRUE;
		pflaFirst = pFirst;
		pflaSecond = pSecond;

		rtOriginalFirst = pflaFirst->GetPosition();
		rtOriginalSecond = pflaSecond->GetPosition();
				
		slotFirstMovieEnd = pflaFirst->SigMovieEnd().Connect( this, &stTWOLINE::OnFirstMovieEnd );
		slotSecondMovieEnd = pflaSecond->SigMovieEnd().Connect( this, &stTWOLINE::OnSecondMovieEnd );

		pflaFirst->Show( false );
		pflaSecond->Show( false );
	}
}

VOID stTWOLINE::Destroy(VOID)
{
	pflaFirst->SigMovieEnd().Disconnect( slotFirstMovieEnd );
	pflaSecond->SigMovieEnd().Disconnect( slotSecondMovieEnd );

	bValid = FALSE;
	pflaFirst = NULL;
	pflaSecond = NULL;
	byObjectID = INVALID_BYTE;
	bAutoFoward = FALSE;
}

VOID stTWOLINE::LoadFlash( const RwChar* szFirst, const RwChar* szSecond, RwUInt8 byID, RwBool autoFoward )
{
	if( bValid )
	{
		pflaFirst->Load( szFirst, TRUE );
		pflaSecond->Load( szSecond, TRUE );

		ReCalcPosition( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );

		byObjectID = byID;
		bAutoFoward = autoFoward;		

		// 초기화 
		EndMovie();
	}	
}

VOID stTWOLINE::Update( RwReal fElapsedTime )
{
	if( bValid )
	{
		pflaFirst->Update( fElapsedTime );
		pflaSecond->Update( fElapsedTime );
	}
}

VOID stTWOLINE::StartFirstMovie(VOID)
{
	if( bValid )
	{
		pflaFirst->Show( true );
		pflaFirst->PlayMovie( true );
	}
}

VOID stTWOLINE::StartSecondMovie(VOID)
{
	if( bValid )
	{
		pflaSecond->Show( true );
		pflaSecond->PlayMovie( true );
	}
}

VOID stTWOLINE::EndMovie(VOID)
{
	if( bValid )
	{
		pflaFirst->PlayMovie( FALSE );
		pflaFirst->Show( false );
		pflaSecond->PlayMovie( FALSE );
		pflaSecond->Show( false );
	}
}

VOID stTWOLINE::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	if( IsValid() )
	{
		SizePosCalculation( pflaFirst, rtOriginalFirst, nScreenWidth, nScreenHeight );
		SizePosCalculation( pflaSecond, rtOriginalSecond, nScreenWidth, nScreenHeight );
	}	
}

VOID stTWOLINE::OnFirstMovieEnd( gui::CComponent* pComponent )
{
	if( bAutoFoward )
	{
		pflaSecond->Show( true );
		pflaSecond->PlayMovie( TRUE );
	}	
}

VOID stTWOLINE::OnSecondMovieEnd( gui::CComponent* pComponent )
{
	EndMovie();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBCommonDirectionGui

CTBCommonDirectionGui::CTBCommonDirectionGui( const RwChar* pName )
: CNtlPLGui( pName )
{

}

CTBCommonDirectionGui::~CTBCommonDirectionGui(VOID)
{

}

VOID CTBCommonDirectionGui::Update( RwReal fElapsed )
{
	//m_stScroll.Update( fElapsed );
	m_stOneLine.Update( fElapsed );
	m_stTwoLine.Update( fElapsed );	
}

VOID CTBCommonDirectionGui::HandleEvents( RWS::CMsg& msg )
{
	
}

VOID CTBCommonDirectionGui::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	//m_stScroll.ReCalcPosition( nScreenWidth, nScreenHeight );
	m_stOneLine.ReCalcPosition( nScreenWidth, nScreenHeight );
	m_stTwoLine.ReCalcPosition( nScreenWidth, nScreenHeight );
	m_pThis->SetSize( nScreenWidth, nScreenHeight );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stCOMMONMEMBERINFO

stCOMMONMEMBERINFO::~stCOMMONMEMBERINFO(VOID)
{
	
}

VOID stCOMMONMEMBERINFO::Show( bool bShow )
{
	pDialog->Show( bShow );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stINDIVIDUALINFO

#define MEMBERINFO_FONTCOLOR RGB( 255, 189, 77 )

stINDIVIDUALINFO::stINDIVIDUALINFO(VOID)
: pstbName( NULL ), pstbLevel( NULL ), pstbLevelValue( NULL ), pstbBattleRecord( NULL ), pstbBattleRecordValue( NULL ),
  pstbGuildName( NULL ), pstbGuildNameValue( NULL ), ppnlClass( NULL )
{
	
}

stINDIVIDUALINFO::~stINDIVIDUALINFO(VOID)
{
	
}

#define INDIVIDUAL_INFO_ITEM_TITLE_X		23
#define INDIVIDUAL_INFO_ITEM_TITLE_WIDTH	120
#define INDIVIDUAL_INFO_ITEM_TITLE_HEIGHT	18

#define INDIVIDUAL_INFO_ITEM_VALUE_X		176
#define INDIVIDUAL_INFO_ITEM_VALUE_WIDTH	170
#define INDIVIDUAL_INFO_ITEM_VALUE_HEIGHT	18

#define INDIVIDUAL_INFO_ITEM1_Y				86
#define INDIVIDUAL_INFO_ITEM2_Y				122
#define INDIVIDUAL_INFO_ITEM3_Y				164

#define INDIVIDUAL_INFO_ITEM_FONT_SIZE		115

VOID stINDIVIDUALINFO::Create( gui::CComponent* pParent )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	CRectangle rect;

	pDialog = NTL_NEW gui::CDialog( pParent, pSurfaceManager );

	rect.SetRectWH( 120, 40, 200, 22 );
	pstbName = NTL_NEW gui::CStaticBox( rect, pDialog, pSurfaceManager, COMP_TEXT_LEFT | COMP_TEXT_UP );
	pstbName->CreateFontStd( DEFAULT_FONT, 135, 0 );
	pstbName->SetTextColor( MEMBERINFO_FONTCOLOR );
	pstbName->Enable( false );

	rect.SetRectWH( INDIVIDUAL_INFO_ITEM_TITLE_X, INDIVIDUAL_INFO_ITEM1_Y, INDIVIDUAL_INFO_ITEM_TITLE_WIDTH, INDIVIDUAL_INFO_ITEM_TITLE_HEIGHT );
	pstbLevel = NTL_NEW gui::CStaticBox( rect, pDialog, pSurfaceManager, COMP_TEXT_RIGHT | COMP_TEXT_UP );
	pstbLevel->CreateFontStd( DEFAULT_FONT, INDIVIDUAL_INFO_ITEM_FONT_SIZE, 0 );
	pstbLevel->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_LEVEL" ) );
	pstbLevel->Enable( false );
	
	rect.SetRectWH( INDIVIDUAL_INFO_ITEM_VALUE_X, INDIVIDUAL_INFO_ITEM1_Y, INDIVIDUAL_INFO_ITEM_VALUE_WIDTH, INDIVIDUAL_INFO_ITEM_VALUE_HEIGHT );
	pstbLevelValue = NTL_NEW gui::CStaticBox( rect, pDialog, pSurfaceManager, COMP_TEXT_LEFT | COMP_TEXT_UP );
	pstbLevelValue->CreateFontStd( DEFAULT_FONT, INDIVIDUAL_INFO_ITEM_FONT_SIZE, 0 );
	pstbLevelValue->SetTextColor( MEMBERINFO_FONTCOLOR );
	pstbLevelValue->Enable( false );

	rect.SetRectWH( INDIVIDUAL_INFO_ITEM_TITLE_X, INDIVIDUAL_INFO_ITEM2_Y, INDIVIDUAL_INFO_ITEM_TITLE_WIDTH, INDIVIDUAL_INFO_ITEM_TITLE_HEIGHT );
	pstbBattleRecord = NTL_NEW gui::CStaticBox( rect, pDialog, pSurfaceManager, COMP_TEXT_RIGHT | COMP_TEXT_UP );
	pstbBattleRecord->CreateFontStd( DEFAULT_FONT, INDIVIDUAL_INFO_ITEM_FONT_SIZE, 0 );
	pstbBattleRecord->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_PRIVATE_RECORD" ) );
	pstbBattleRecord->Enable( false );

	rect.SetRectWH( INDIVIDUAL_INFO_ITEM_VALUE_X, INDIVIDUAL_INFO_ITEM2_Y, INDIVIDUAL_INFO_ITEM_VALUE_WIDTH, INDIVIDUAL_INFO_ITEM_VALUE_HEIGHT );
	pstbBattleRecordValue = NTL_NEW gui::CStaticBox( rect, pDialog, pSurfaceManager, COMP_TEXT_LEFT | COMP_TEXT_UP );
	pstbBattleRecordValue->CreateFontStd( DEFAULT_FONT, INDIVIDUAL_INFO_ITEM_FONT_SIZE, 0 );
	pstbBattleRecordValue->SetTextColor( MEMBERINFO_FONTCOLOR );
	pstbBattleRecordValue->Enable( false );

	rect.SetRectWH( INDIVIDUAL_INFO_ITEM_TITLE_X, INDIVIDUAL_INFO_ITEM3_Y, INDIVIDUAL_INFO_ITEM_TITLE_WIDTH, INDIVIDUAL_INFO_ITEM_TITLE_HEIGHT );
	pstbGuildName = NTL_NEW gui::CStaticBox( rect, pDialog, pSurfaceManager, COMP_TEXT_RIGHT | COMP_TEXT_UP );
	pstbGuildName->CreateFontStd( DEFAULT_FONT, INDIVIDUAL_INFO_ITEM_FONT_SIZE, 0 );
	pstbGuildName->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_GUILD_NAME" ) );
	pstbGuildName->Enable( false );

	rect.SetRectWH( INDIVIDUAL_INFO_ITEM_VALUE_X, INDIVIDUAL_INFO_ITEM3_Y, INDIVIDUAL_INFO_ITEM_VALUE_WIDTH, INDIVIDUAL_INFO_ITEM_VALUE_HEIGHT );
	pstbGuildNameValue = NTL_NEW gui::CStaticBox( rect, pDialog, pSurfaceManager, COMP_TEXT_LEFT | COMP_TEXT_UP );
	pstbGuildNameValue->CreateFontStd( DEFAULT_FONT, INDIVIDUAL_INFO_ITEM_FONT_SIZE, 0 );
	pstbGuildNameValue->SetTextColor( MEMBERINFO_FONTCOLOR );
	pstbGuildNameValue->Enable( false );

	rect.SetRectWH( 92, 40, 19, 19 );
	ppnlClass = NTL_NEW gui::CPanel( rect, pDialog, pSurfaceManager );
		
	Show( false );
}

VOID stINDIVIDUALINFO::SetBluePos(VOID)
{
	pDialog->SetPosition( 625, 337 );

	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "stINDIVIDUALINFO::SetBluePos : must World concept is valid" );		

	stTBudokaiTeam* pBlueTeam = pTBWorldConcept->GetBlueTeam();
	
	if( !pBlueTeam )
		return;

	VEC_TBTEAMMEMBER_ITER iter = pBlueTeam->vecpMember.begin();
	if( iter == pBlueTeam->vecpMember.end() )
		return;

	stTBudokaiMember* pMember = *iter;
	
	SetData( pMember );	
}

VOID stINDIVIDUALINFO::SetRedPos(VOID)
{
	pDialog->SetPosition( 20, 337 );

	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "stINDIVIDUALINFO::SetBluePos : must World concept is valid" );		

	stTBudokaiTeam* pRedTeam = pTBWorldConcept->GetRedTeam();

	if( !pRedTeam )
		return;

	VEC_TBTEAMMEMBER_ITER iter = pRedTeam->vecpMember.begin();
	if( iter == pRedTeam->vecpMember.end() )
		return;

	stTBudokaiMember* pMember = *iter;

	SetData( pMember );
}

VOID stINDIVIDUALINFO::SetData( stTBudokaiMember* pMember )
{
	pstbName->SetText( pMember->wstrName.c_str() );

	ppnlClass->ClearSurface();
	ppnlClass->AddSurface( Logic_GetPCClassIconSurface( pMember->byClass, FALSE ) );
	ppnlClass->SetToolTip( Logic_GetClassName( pMember->byClass ) );

	pstbLevelValue->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_LEVEL_VALUE" ), pMember->byLevel );
	
	if( pMember->wstrGuildName.size() )
		pstbGuildNameValue->SetText( pMember->wstrGuildName.c_str() );
	else
		pstbGuildNameValue->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_NOT_IN_GUILD" ) );

	pstbBattleRecordValue->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_RECORD_DATA" ),
								   pMember->wWinCount + pMember->wLoseCount + pMember->wDrawCount, pMember->wWinCount, pMember->wLoseCount );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stPARTYINFO

stPARTYMEMBERITEM::stPARTYMEMBERITEM(VOID)
: pItemDialog( NULL ), ppnlClass( NULL ), pstbName( NULL ), pstbLevel( NULL ), pstbRankPoint( NULL )
{

}

#define PARTY_INFO_ITEM_HEIGHT		18
#define PARTY_INFO_ITEM_FONT_SIZE	115

VOID stPARTYMEMBERITEM::Create( gui::CComponent* pParent )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	CRectangle rect;

	pItemDialog = NTL_NEW gui::CDialog( pParent, pSurfaceManager );

	rect.SetRectWH( 92, 0, 160, PARTY_INFO_ITEM_HEIGHT );
	pstbName = NTL_NEW gui::CStaticBox( rect, pItemDialog, pSurfaceManager, COMP_TEXT_LEFT | COMP_TEXT_UP );
	pstbName->CreateFontStd( DEFAULT_FONT, PARTY_INFO_ITEM_FONT_SIZE, 0 );
	pstbName->Enable( false );
	rect.SetRectWH( 259, 0, 70, PARTY_INFO_ITEM_HEIGHT );
	pstbLevel = NTL_NEW gui::CStaticBox( rect, pItemDialog, pSurfaceManager, COMP_TEXT_LEFT | COMP_TEXT_UP );
	pstbLevel->CreateFontStd( DEFAULT_FONT, PARTY_INFO_ITEM_FONT_SIZE, 0 );
	pstbLevel->SetTextColor( MEMBERINFO_FONTCOLOR );	
	pstbLevel->Enable( false );
	rect.SetRectWH( 56, 0, 19, 19 );
	ppnlClass = NTL_NEW gui::CPanel( rect, pItemDialog, pSurfaceManager );
}

VOID stPARTYMEMBERITEM::SetPos( RwUInt8 byIndex )
{
	pItemDialog->SetPosition( 0, 35 + byIndex * 36 );
}

VOID stPARTYMEMBERITEM::SetData( stTBudokaiMember* pMember )
{
	pstbName->SetText( pMember->wstrName.c_str() );

	ppnlClass->ClearSurface();
	ppnlClass->AddSurface( Logic_GetPCClassIconSurface( pMember->byClass, FALSE ) );
	ppnlClass->SetToolTip( Logic_GetClassName( pMember->byClass ) );

	// 서버데이터 필요
	pstbLevel->Format( GetDisplayStringManager()->GetString( "DST_BUDOKAI_TEAMINFO_LEVEL_VALUE" ), pMember->byLevel );	
}

VOID stPARTYMEMBERITEM::Show( bool bShow )
{
	pItemDialog->Show( bShow );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stPARTYINFO

stPARTYINFO::stPARTYINFO(VOID)
{
	
}

stPARTYINFO::~stPARTYINFO(VOID)
{
	
}

VOID stPARTYINFO::Create( gui::CComponent* pParent )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	pDialog = NTL_NEW gui::CDialog( pParent, pSurfaceManager );

	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		aPartyMemberItem[i].Create( pDialog );
		aPartyMemberItem[i].Show( false );
	}
	
	Show( false );
}

VOID stPARTYINFO::SetBluePos(VOID)
{
	pDialog->SetPosition( 625, 340 );

	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "stINDIVIDUALINFO::SetBluePos : must World concept is valid" );		

	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
		aPartyMemberItem[i].Show( false );
	
	stTBudokaiTeam* pBlueTeam = pTBWorldConcept->GetBlueTeam();

	if( !pBlueTeam )
		return;

	VEC_TBTEAMMEMBER_ITER iter = pBlueTeam->vecpMember.begin();

	for( RwUInt8 i = 0 ; iter != pBlueTeam->vecpMember.end() ; ++iter, ++i )
	{
		stTBudokaiMember* pMember = *iter;
		aPartyMemberItem[i].SetData( pMember );		
		aPartyMemberItem[i].SetPos( i );
		aPartyMemberItem[i].Show( true );
	}	
}

VOID stPARTYINFO::SetRedPos(VOID)
{
	pDialog->SetPosition( 20, 340 );

	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
	NTL_ASSERT( pTBWorldConcept, "stINDIVIDUALINFO::SetRedPos : must World concept is valid" );		

	for( RwInt32 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
		aPartyMemberItem[i].Show( false );

	stTBudokaiTeam* pRedTeam = pTBWorldConcept->GetRedTeam();

	if( !pRedTeam )
		return;

	VEC_TBTEAMMEMBER_ITER iter = pRedTeam->vecpMember.begin();

	for( RwUInt8 i = 0 ; iter != pRedTeam->vecpMember.end() ; ++iter, ++i )
	{
		stTBudokaiMember* pMember = *iter;
		aPartyMemberItem[i].SetData( pMember );		
		aPartyMemberItem[i].SetPos( i );
		aPartyMemberItem[i].Show( true );
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stMEMBERINFO

stMEMBERINFO::~stMEMBERINFO(VOID)
{
	NTL_DELETE( pMemberInfo );
}

VOID stMEMBERINFO::Create( gui::CFlash* pTeam )
{
	if( pTeam )
	{
		bValid = TRUE;
		pflaTeam = pTeam;
		rtOriginalTeam = pflaTeam->GetPosition();

		pflaTeam->Show( false );

		slotMovieEnd = pflaTeam->SigMovieEnd().Connect( this, &stMEMBERINFO::OnMovieEnd );
		slotFSCallBack = pflaTeam->SigFSCallBack().Connect( this, &stMEMBERINFO::OnFSCallBack );				
	}
}

VOID stMEMBERINFO::Destroy(VOID)
{
	pflaTeam->SigMovieEnd().Disconnect( slotMovieEnd );
	pflaTeam->SigFSCallBack().Disconnect( slotFSCallBack );

	bValid = FALSE;
	pflaTeam = NULL;
}

VOID stMEMBERINFO::LoadFlash( const RwChar* szFile )
{
	if( bValid )
	{
		pflaTeam->Load( szFile, TRUE );
		ReCalcPosition( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );	
	}
}

VOID stMEMBERINFO::SetMemberInfo( stCOMMONMEMBERINFO* pInfo )
{
	NTL_DELETE( pMemberInfo );
	pMemberInfo = pInfo;
}

VOID stMEMBERINFO::Update( RwReal fElapsedTime )
{
	if( bValid )
		pflaTeam->Update( fElapsedTime );
}

VOID stMEMBERINFO::StartMovie(VOID)
{
	if( bValid )
	{
		pflaTeam->Show( true );
		pflaTeam->RestartMovie();
	}
}

VOID stMEMBERINFO::EndMovie(VOID)
{
	if( bValid )
	{
		pflaTeam->PlayMovie( FALSE );
		pflaTeam->Show( false );
	}
}

VOID stMEMBERINFO::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	if( IsValid() )
		SizePosCalculation( pflaTeam, rtOriginalTeam, nScreenWidth, nScreenHeight );
}

VOID stMEMBERINFO::OnMovieEnd( gui::CComponent* pComponent )
{
	EndMovie();
}

VOID stMEMBERINFO::OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "TextStart" ) )
	{
		if( pMemberInfo )
			pMemberInfo->Show( true );
	}
	else if( !strcmp( szCommand, "TextEnd" ) )
	{
		if( pMemberInfo )
			pMemberInfo->Show( false );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stSCOREINFO

VOID stSCOREINFO::Create( gui::CFlash* pScore )
{
	if( pScore )
	{
		bValid = TRUE;
		pflaScore = pScore;
		rtOriginalScore = pflaScore->GetPosition();
		
		pflaScore->Show( false );
	}
}

VOID stSCOREINFO::Destroy(VOID)
{
	bValid = FALSE;
	pflaScore = NULL;
}

VOID stSCOREINFO::LoadFlash( const RwChar* szFile )
{
	if (bValid)
	{
		pflaScore->Load(szFile, TRUE);
		ReCalcPosition(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

		EndMovie();
	}
}

VOID stSCOREINFO::Update( RwReal fElapsedTime )
{
	if( bValid)
		pflaScore->Update( fElapsedTime );
}

VOID stSCOREINFO::StartMovie(VOID)
{
	if( bValid)
	{
		pflaScore->RestartMovie();
		pflaScore->Show(true);
	}
}

VOID stSCOREINFO::EndMovie(VOID)
{
	if( bValid )
	{
		pflaScore->PlayMovie(FALSE);
		pflaScore->Show(false);
	}
}


VOID stSCOREINFO::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	if( IsValid() )
		SizePosCalculation( pflaScore, rtOriginalScore, nScreenWidth, nScreenHeight );	
}

VOID stSCOREINFO::SetScore( RwInt32 nBlue, RwInt32 nRed )
{
	if( bValid )
	{
		pflaScore->Invoke( "Set_Score", "%d,%d", nRed, nBlue );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBMainCommonDirectionGui

CTBMainCommonDirectionGui::CTBMainCommonDirectionGui( const RwChar* pName )
: CTBCommonDirectionGui( pName )
{

}

CTBMainCommonDirectionGui::~CTBMainCommonDirectionGui(VOID)
{

}

VOID CTBMainCommonDirectionGui::Update( RwReal fElapsed )
{
	CTBCommonDirectionGui::Update( fElapsed );

	m_stBlueMember.Update( fElapsed );
	m_stRedMember.Update( fElapsed );
	m_stScore.Update( fElapsed );
}

VOID CTBMainCommonDirectionGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		ReCalcPosition( pData->iWidht, pData->iHeight );		
	}
}

VOID CTBMainCommonDirectionGui::ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	CTBCommonDirectionGui::ReCalcPosition( nScreenWidth, nScreenHeight );

	m_stBlueMember.ReCalcPosition( nScreenWidth, nScreenHeight );
	m_stRedMember.ReCalcPosition( nScreenWidth, nScreenHeight );
	m_stScore.ReCalcPosition( nScreenWidth, nScreenHeight );	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBFlashName

const RwChar* CTBFlashName::GetDepth( RwUInt8 byDepth )
{
	switch( byDepth )
	{
	case BUDOKAI_MATCH_DEPTH_2 : return "TB_Final.swf";
	case BUDOKAI_MATCH_DEPTH_4 : return "TB_SemiFinal.swf";
	case BUDOKAI_MATCH_DEPTH_8 : return "TB_8.swf";
	case BUDOKAI_MATCH_DEPTH_16: return "TB_16.swf";
	case BUDOKAI_MATCH_DEPTH_32: return "TB_32.swf";
	}

	return "TB_Final.swf";
}

const RwChar* CTBFlashName::GetNormalRound( RwUInt8 byRound )
{
	static RwChar buf[256];
	sprintf_s( buf, 256, "TB_Normal_Round%u.swf", byRound );

	return buf;
}

const RwChar* CTBFlashName::GetSpecialRound( RwUInt8 byRound )
{
	static RwChar buf[256];
	sprintf_s( buf, 256, "TB_Special_Round%u.swf", byRound );

	return buf;
}