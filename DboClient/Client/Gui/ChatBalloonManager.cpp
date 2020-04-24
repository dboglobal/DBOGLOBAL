#include "precomp_dboclient.h"
#include "ChatBalloonManager.h"

// Core
#include "NtlDebug.h"

// Shared
#include "TextAllTable.h"
#include "QuestTextDataTable.h"
#include "TableContainer.h"
#include "PCTable.h"
#include "NpcSpeechTable.h"
#include "NtlTeleport.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSLLogicDef.h"
#include "NtlSobManager.h"
#include "NtlSob.h"
#include "NtlSobProxy.h"
#include "NtlSobCharProxy.h"
#include "NtlSobTriggerObjectProxy.h"
#include "NtlSobDynamicObjectProxy.h"
#include "NtlSobAvatar.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobNpc.h"
#include "NtlSobTriggerObject.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"

// Gui
#include "GuiUtil.h"

// DboClient
#include "DboEvent.h"
#include "DboGlobal.h"
#include "GuiFunctor.h"
#include "DboLogic.h"

#include "AlarmManager.h"
#include "DialogManager.h"
#include "DboLogic.h"

//test
#include "NtlMath.h"

// Define
#define BALLOON_COMMON_FADETIME			0.5f
#define BALLOON_COMMON_FADEIN_VALUE		255
#define BALLOON_COMMON_FADEOUT_VALUE	0
#define BALLOON_COMMON_OFFSET_X			0
#define BALLOON_COMMON_OFFSET_Y			-20
#define BALLOON_COMMON_CORNERSIZE		12

#define BALLOON_CHAT_RANGE		60.0f

#define BALLOON_NPC_RANGE		30.0f
#define BALLOON_NPC_WIDTH		223
#define BALLOON_NPC_HEIGHT		94
#define BALLOON_NPC_OFFSET_X	22		
#define BALLOON_NPC_OFFSET_Y	(BALLOON_COMMON_OFFSET_Y-BALLOON_NPC_HEIGHT)
#define BALLOON_NPC_TEXT_X				8
#define BALLOON_NPC_TEXT_Y				5
#define BALLOON_NPC_TEXT_WIDHT			208	
#define BALLOON_NPC_TEXT_HEIGHT			71
//#define BALLOON_QUEST_WIDTH				229
//#define BALLOON_QUEST_HEIGHT			108
//#define BALLOON_QUEST_TEXT_X			20
//#define BALLOON_QUEST_TEXT_Y			29
//#define BALLOON_QUEST_TEXT_WIDHT		228
//#define BALLOON_QUEST_TEXT_HEIGHT		55
#define BALLOON_QUEST_OFFSET_X			50
#define BALLOON_QUEST_MARK_OFFSET		-20
#define BALLOON_QUEST_CORNERSIZE		18
#define BALLOON_QUEST_IMAGESIZE			48

#define BALLOON_PRIVATESHOP_RANGE		30.0f
#define BALLOON_PRIVATESHOP_WIDTH		226
#define BALLOON_PRIVATESHOP_HEIGHT		24
#define BALLOON_PRIVATESHOP_OFFSET_X	(-BALLOON_PRIVATESHOP_WIDTH/2)		
#define BALLOON_PRIVATESHOP_OFFSET_Y	(BALLOON_COMMON_OFFSET_Y-BALLOON_PRIVATESHOP_HEIGHT)
#define BALLOON_PRIVATESHOP_TEXT_X		23
#define BALLOON_PRIVATESHOP_TEXT_Y		5
#define BALLOON_PRIVATESHOP_TEXT_WIDHT	194
#define BALLOON_PRIVATESHOP_TEXT_HEIGHT	14

#define BALLOON_BATTLEGAUGE_RANGE		60.0f
#define BALLOON_BATTLEGAUGE_WIDTH		106
#define BALLOON_BATTLEGAUGE_HEIGHT		9
#define BALLOON_BATTLEGAUGE_OFFSET_X	(-BALLOON_BATTLEGAUGE_WIDTH/2)		
#define BALLOON_BATTLEGAUGE_OFFSET_Y	(BALLOON_COMMON_OFFSET_Y-BALLOON_BATTLEGAUGE_HEIGHT)
#define BALLOON_BATTLEGAUGE_TEXT_X		-11
#define BALLOON_BATTLEGAUGE_TEXT_Y		-20
#define BALLOON_BATTLEGAUGE_TEXT_WIDHT	128	
#define BALLOON_BATTLEGAUGE_TEXT_HEIGHT	20
#define BALLOON_BATTLEGAUGE_FONT_HEIGHT 120

#define BALLOON_DICE_OFFSET_X			15
#define BALLOON_DICE_OFFSET_Y			-70

#define BALLOON_SUB_PRIVATESHOP_HEIGHT		25
#define BALLOON_SUB_BATTLEGAUGE_HEIGHT		15

#define BALLOON_CHAR_NAME_COLOR RGB( 255, 255, 185 )
#define BALLOON_CHAR_NAME_QUEST_COLOR RGB( 220, 220, 220 )

#define BALLOON_NAMEBAR_WIDTH			180
#define BALLOON_NAMEBAR_HEIGHT			20
#define BALLOON_NAMEBAR_YOFFSET			-5

// BaseBalloon
VOID CBalloonGui::Show3DName( RwBool bShow )
{
	if( m_pOwnerSobObject->GetClassID() == SLCLASS_TRIGGER_OBJECT )
	{
		CNtlSobTriggerObjectProxy* pProxy = reinterpret_cast<CNtlSobTriggerObjectProxy*>( m_pOwnerSobObject->GetSobProxy() );
		if( pProxy )
			pProxy->EnableNameVisible( bShow );
	}
	else if( m_pOwnerSobObject->GetClassID() == SLCLASS_DYNAMIC_OBJECT )
	{
		CNtlSobDynamicObjectProxy* pProxy = reinterpret_cast<CNtlSobDynamicObjectProxy*>( m_pOwnerSobObject->GetSobProxy() );
		if( pProxy )
			pProxy->EnableNameVisible( bShow );
	}
	else
	{
		CNtlSobCharProxy* pProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pOwnerSobObject->GetSobProxy() );
		if( pProxy )
			pProxy->EnablePlayerNameVisible( bShow );
	}
}

VOID CBalloonGui::SetState( RwInt32 eState )
{
	m_fCurrentTime = 0.0f;
	m_eState = eState;
}

RwInt32 CBalloonGui::PriorityCheck( RwUInt32 eType )
{
	// Check Layer
	RwInt32 nResLayer = ( eType >> 12 ) - ( GetType() >> 12 );

	if( nResLayer == 0 )	
	{
		// Check Priority
		RwInt32 nResPriority = eType - GetType();

		if( nResPriority == 0 )
			return EQUAL_PRIORITY;
		else if( nResPriority > 0 )
			return MORE_PRIORITY;
		else 
			return LESS_PRIORITY;
	}
	else if( nResLayer > 0 )
		return UPPER_LAYER;
	else
		return LOWER_LAYER;	
}

// ChatBalloon 
// 1. 20초간 표시. ( 양 끝단 1초간 Blending ) - ok
// 2. PC 반경 60m  - ok
// 3. PC 머리위 표시. - ok 
// 4. 공격 받으면 사라짐.
// 5. 파티대화만 표시 옵션

CChatBalloonGui::CChatBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, const WCHAR* pMessage, RwInt32 eType, RwBool bPartyMsg /* = FALSE  */ )
: CBalloonGui( pOwnerObject, BALLOON_CHAT_DISPTIME, posOffsetDelta, eType ), m_bIsPartyMessage( bPartyMsg )
{
	RwUInt8 byRed = 0, byGreen = 0, byBlue = 0;
	if (bPartyMsg)
	{
		byRed = 105; byGreen = 214; byBlue = 240;
	}
	else
	{
		CNtlSobPlayerAttr* pAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pOwnerObject->GetSobAttr());
		switch (pAttr->GetPcTbl()->byRace)
		{
			case RACE_HUMAN: byRed = 200; byGreen = 220; byBlue = 255; break;
			case RACE_NAMEK: byRed = 200; byGreen = 255; byBlue = 220; break;
			case RACE_MAJIN: byRed = 255; byGreen = 200; byBlue = 220; break;
		}
	}

	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	m_pGuiBalloon = NTL_NEW gui::CBalloon( CRectangle( 0, 0, 200, 80 ), NULL, pSurfaceManager, FALSE );

	CRectangle rect;
	rect.SetRectWH( BALLOON_COMMON_CORNERSIZE, BALLOON_NAMEBAR_YOFFSET, BALLOON_NAMEBAR_WIDTH, BALLOON_NAMEBAR_HEIGHT );
	m_pName = NTL_NEW gui::CStaticBox( rect, m_pGuiBalloon, pSurfaceManager, 0, TRUE );
	m_pName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pName->SetTextColor( RGB( byRed, byGreen, byBlue ) );
	m_pName->SetEffectMode( TE_OUTLINE );
	m_pName->SetEffectValue( 0 );
	m_pName->SetText( Logic_GetName( pOwnerObject ) );

	// Balloon 속성 세팅
	m_pGuiBalloon->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pGuiBalloon->SetTextColor( RGB( 10, 10, 10 ) );
	m_pGuiBalloon->SetMargin( 5, 0 );
	m_pGuiBalloon->SetMinSize( m_pName->GetWidth() + 20, 20 );
	m_pGuiBalloon->Enable( false );

	m_pGuiBalloon->AddSurfaces( pSurfaceManager->GetSurface( "Balloon.srf", "ChatCenter"), 
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatLTC"),
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatRTC"),
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatLBC"),
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatRBC"),
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatLL"),
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatTL"),
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatRL"),
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatBL"),
		pSurfaceManager->GetSurface( "Balloon.srf", "ChatTail") );

	m_pGuiBalloon->SetBalloonColor( byRed, byGreen, byBlue );
	m_pGuiBalloon->SetText( pMessage );	

	m_posOffset.SetPos( BALLOON_COMMON_OFFSET_X, BALLOON_COMMON_OFFSET_Y );
}

CChatBalloonGui::~CChatBalloonGui(VOID)
{
	NTL_DELETE( m_pGuiBalloon );
}

VOID CChatBalloonGui::SetMessage( const WCHAR* pMessage )
{
	m_pGuiBalloon->SetText( pMessage );
	SetState( FADE_IN );
}

VOID CChatBalloonGui::FadeoutMessage(VOID)
{
	SetState( FADE_OUT );
}

RwInt32 CChatBalloonGui::Update( RwReal fElapseTime )
{
	m_fCurrentTime += fElapseTime;

	// Fade 효과
	FadeEffect();

	if( m_eState == HIDE )
		return CBalloonManager::UR_DELETE;

	if( GetGuiFuntor()->GetDistanceFromAvatar( m_pOwnerSobObject->GetPosition() ) > BALLOON_CHAT_RANGE )
		return CBalloonManager::UR_NOTSHOW;

	// 위치 변경
	if( !PositionUpdate() )
		return CBalloonManager::UR_NOTSHOW;

	return CBalloonManager::UR_SHOW;
}

VOID CChatBalloonGui::Paint(VOID)
{
	m_pGuiBalloon->Paint();
}

RwBool CChatBalloonGui::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset = m_posOffset + m_posOffsetDelta;

	CNtlSobProxy* pProxy = m_pOwnerSobObject->GetSobProxy();

	if( pProxy )
	{
		if( GetGuiFuntor()->Calc3DPosTo2D( m_pOwnerSobObject->GetPosition(), pProxy->GetPLEntityHeight(), &posOffset, &posRes ) )
		{
			m_pGuiBalloon->SetOriginNow( posRes.x, posRes.y, FALSE );
			return TRUE;
		}
	}	

	return FALSE;
}

VOID CChatBalloonGui::FadeEffect(VOID)
{
	if( m_eState == SHOW )
	{
		if( m_fCurrentTime > m_fLifeTime )
			SetState( FADE_OUT );
	}
	else if( m_eState == FADE_IN )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade In Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEOUT_VALUE + fFactor * BALLOON_COMMON_FADEIN_VALUE;

			m_pGuiBalloon->SetAlpha( (BYTE)fResult );
		}
		else
		{
			m_pGuiBalloon->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEIN_VALUE );
			SetState( SHOW );
		}
	}
	else if( m_eState == FADE_OUT )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade Out Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEIN_VALUE + fFactor * BALLOON_COMMON_FADEOUT_VALUE;

			m_pGuiBalloon->SetAlpha( (BYTE)fResult );
		}
		else
		{
			m_pGuiBalloon->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEOUT_VALUE );
			SetState( HIDE );
		}
	}		
}

// NPCBalloon 
// 1. PC radius 30m
// 2. NPC or Monster overhead display.
// 3. Clicked on an NPC or created by the server.
// 4. Image changed according to emotion (not used)


CNPCBalloonGui::CNPCBalloonGui(CNtlSob* pOwnerObject, CPos& posOffsetDelta, const WCHAR* pMessage, RwReal fLifeTime, RwInt32 eType, RwUInt8 byBalloonType, RwBool bTimeProc )
: CBalloonGui( pOwnerObject, fLifeTime, posOffsetDelta, eType ), m_bTimeProc( bTimeProc ), m_byBalloonType( byBalloonType ), m_wstrString( pMessage )
{
	GetBalloonManager()->GetTestText()->SetText( pMessage );
	RwInt32 nLineCount = GetBalloonManager()->GetTestText()->GetLineCount() + 1;
	RwInt32 nWidth = GetBalloonManager()->GetTestText()->GetFitWidthToString();

	std::wstring strMessage = GetBalloonManager()->GetTestText()->GetText();
	gui::RemoveCStyleLineFeed( strMessage );
	gui::ChangeLineFeedToCStyle( strMessage );

	// Resource Name
	RwChar buf[256];

	if( nWidth > 72 || nLineCount > 2 )
	{
		sprintf_s( buf, 256, "textBalloon_0%u_R.swf", byBalloonType + 1 );
		m_bStandardSize = TRUE;
	}
	else
	{
		sprintf_s( buf, 256, "textBalloon_0%u_R_s.swf", byBalloonType + 1 );
		m_bStandardSize = FALSE;
	}

	m_pFlash = NTL_NEW gui::CFlash( CRectangle(), NULL, GetNtlGuiManager()->GetSurfaceManager(), buf, TRUE, TRUE );
	m_pFlash->SetFrameCallback( FALSE );

	if( m_bStandardSize )
	{
		if( nLineCount > 4 )
			nLineCount = 4;		
	}
	else
	{
		if( nLineCount > 2 )
			nLineCount = 2;
	}	

	sprintf_s( buf, 256, "Text_Box0%d.text", nLineCount );
	m_pFlash->SetVariable( buf, strMessage.c_str() );
	m_pFlash->PlayMovie( TRUE );

	m_posOffset.SetPos( BALLOON_NPC_OFFSET_X, BALLOON_NPC_OFFSET_Y );
}

CNPCBalloonGui::~CNPCBalloonGui(VOID)
{
	NTL_DELETE( m_pFlash );	
}

VOID CNPCBalloonGui::SetMessage( const WCHAR* pMessage, RwBool bTimeProc )
{
	m_wstrString = pMessage;

	GetBalloonManager()->GetTestText()->SetText( pMessage );
	RwInt32 nLineCount = GetBalloonManager()->GetTestText()->GetLineCount() + 1;
	RwInt32 nWidth = GetBalloonManager()->GetTestText()->GetFitWidthToString();

	std::wstring strMessage = GetBalloonManager()->GetTestText()->GetText();
	gui::RemoveCStyleLineFeed( strMessage );
	gui::ChangeLineFeedToCStyle( strMessage );

	// Resource Name
	RwChar buf[256];
	RwBool bNeedToReCreate = FALSE;

	if( nWidth > 72 || nLineCount > 2 )
	{
		if( m_bStandardSize )
			bNeedToReCreate = FALSE;
		else
		{
			bNeedToReCreate = TRUE;
			sprintf_s( buf, 256, "textBalloon_0%u_R.swf", m_byBalloonType + 1 );
			m_bStandardSize = TRUE;
		}
	}
	else
	{
		if( m_bStandardSize )
		{
			bNeedToReCreate = TRUE;
			sprintf_s( buf, 256, "textBalloon_0%u_R_s.swf", m_byBalloonType + 1 );
			m_bStandardSize = FALSE;
		}
		else
			bNeedToReCreate = FALSE;		
	}

	if( bNeedToReCreate )
	{
		NTL_DELETE( m_pFlash );
		m_pFlash = NTL_NEW gui::CFlash( CRectangle(), NULL, GetNtlGuiManager()->GetSurfaceManager(), buf, TRUE, TRUE );
		m_pFlash->SetFrameCallback( FALSE );
	}
	else
	{
		m_pFlash->RestartMovie();
	}

	if( m_bStandardSize )
	{
		if( nLineCount > 4 )
			nLineCount = 4;		
	}
	else
	{
		if( nLineCount > 2 )
			nLineCount = 2;
	}	

	sprintf_s( buf, 256, "Text_Box0%d.text", nLineCount );
	m_pFlash->SetVariable( buf, strMessage.c_str() );
	m_pFlash->PlayMovie( TRUE );

	m_bTimeProc = bTimeProc;	
	SetState( FADE_IN );	
}

VOID CNPCBalloonGui::FadeoutMessage(VOID)
{
	m_bTimeProc = TRUE;
	SetState( FADE_OUT );
}

RwInt32 CNPCBalloonGui::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	FadeEffect();

	if( m_eState == HIDE )
		return CBalloonManager::UR_DELETE;

	if( GetGuiFuntor()->GetDistanceFromAvatar( m_pOwnerSobObject->GetPosition() ) > BALLOON_NPC_RANGE )
		return CBalloonManager::UR_NOTSHOW;

	if( !PositionUpdate() )
		return CBalloonManager::UR_NOTSHOW;

	m_pFlash->Update( fElapsedTime );

	return CBalloonManager::UR_SHOW;
}

VOID CNPCBalloonGui::Paint(VOID)
{
	m_pFlash->Paint();
}

RwBool CNPCBalloonGui::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset = m_posOffset + m_posOffsetDelta;

	CNtlSobProxy* pProxy = m_pOwnerSobObject->GetSobProxy();

	if( pProxy )
	{
		if( GetGuiFuntor()->Calc3DPosTo2D( m_pOwnerSobObject->GetPosition(), pProxy->GetPLEntityHeight(), &posOffset, &posRes ) )
		{
			m_pFlash->SetPosition( posRes.x, posRes.y );
			return TRUE;
		}
	}
	return FALSE;	
}

VOID CNPCBalloonGui::FadeEffect(VOID)
{
	if( m_eState == SHOW )
	{
		if( m_fCurrentTime > m_fLifeTime && m_bTimeProc )
			SetState( FADE_OUT );
	}
	else if( m_eState == FADE_IN )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade In Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEOUT_VALUE + fFactor * BALLOON_COMMON_FADEIN_VALUE;

			m_pFlash->SetAlpha( (BYTE)fResult );			
		}
		else
		{
			m_pFlash->SetAlpha( BALLOON_COMMON_FADEIN_VALUE );
			SetState( SHOW );
		}
	}
	else if( m_eState == FADE_OUT )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME && m_bTimeProc )
		{
			// Fade Out Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEIN_VALUE + fFactor * BALLOON_COMMON_FADEOUT_VALUE;

			m_pFlash->SetAlpha( (BYTE)fResult );			
		}
		else
		{
			m_pFlash->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEOUT_VALUE );
			SetState( HIDE );
		}
	}	
}

std::wstring CNPCBalloonGui::GetText(VOID)
{
	return  m_wstrString;
}

// PrivateShopBalloon 
// 1. 표시여부는 명령에의해서만 처리
// 2. PC 반경 30m

CPrivateShopBalloonGui::CPrivateShopBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, RwBool bReady, const WCHAR* pMessage )
: CBalloonGui( pOwnerObject, 0.0f, posOffsetDelta, TYPE_PRIVATESHOP )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	CRectangle rect;

	rect.SetRectWH( 0, 0, BALLOON_PRIVATESHOP_WIDTH, BALLOON_PRIVATESHOP_HEIGHT );
	m_pPanel = NTL_NEW gui::CPanel( rect, NULL, pSurfaceManager);
	m_pPanel->Enable( false );

	m_wbBackground.SetPosition(0, 0);

	rect.SetRectWH( BALLOON_PRIVATESHOP_TEXT_X, BALLOON_PRIVATESHOP_TEXT_Y, BALLOON_PRIVATESHOP_TEXT_WIDHT, BALLOON_PRIVATESHOP_TEXT_HEIGHT );
	m_pText = NTL_NEW gui::CStaticBox( rect, m_pPanel, pSurfaceManager, COMP_TEXT_CENTER, TRUE );
	m_pText->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pText->Enable( false );

	m_slotMovePanel		= m_pPanel->SigMove().Connect(this, &CPrivateShopBalloonGui::OnMovePanel);
	m_slotPaintPanel	= m_pPanel->SigPaint().Connect(this, &CPrivateShopBalloonGui::OnPaintPanel);

	m_posOffset.SetPos( BALLOON_PRIVATESHOP_OFFSET_X, BALLOON_PRIVATESHOP_OFFSET_Y );

	m_pText->SetText( pMessage );
	SetTextBkColor( bReady );
	SetTextColor( bReady );
	SetBackground( bReady );
}

CPrivateShopBalloonGui::~CPrivateShopBalloonGui(VOID)
{
	NTL_DELETE( m_pText );
	NTL_DELETE( m_pPanel );
}

VOID CPrivateShopBalloonGui::SetMessage( const WCHAR* pMessage, RwBool bReady )
{
	m_pText->SetText( pMessage );
	SetTextBkColor( bReady );
	SetTextColor( bReady );
	SetBackground( bReady );
	SetState( FADE_IN );
}

VOID CPrivateShopBalloonGui::FadeoutMessage(VOID)
{
	SetState( FADE_OUT );
}

RwInt32 CPrivateShopBalloonGui::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	FadeEffect();

	if( m_eState == HIDE )
		return CBalloonManager::UR_DELETE;

	if( GetGuiFuntor()->GetDistanceFromAvatar( m_pOwnerSobObject->GetPosition() ) > BALLOON_PRIVATESHOP_RANGE )
		return CBalloonManager::UR_NOTSHOW;

	if( !PositionUpdate() )
		return CBalloonManager::UR_NOTSHOW;

	return CBalloonManager::UR_SHOW;
}

VOID CPrivateShopBalloonGui::Paint(VOID)
{
	m_pPanel->Paint();
}

RwBool CPrivateShopBalloonGui::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset = m_posOffset + m_posOffsetDelta;

	CNtlSobProxy* pProxy = m_pOwnerSobObject->GetSobProxy();

	if( pProxy )
	{
		if( GetGuiFuntor()->Calc3DPosTo2D( m_pOwnerSobObject->GetPosition(), pProxy->GetPLEntityHeight(), &posOffset, &posRes ) )
		{
			m_pPanel->SetPosition( posRes.x, posRes.y );
			return TRUE;
		}
	}
	return FALSE;
}

VOID CPrivateShopBalloonGui::FadeEffect(VOID)
{
	if( m_eState == FADE_IN )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade In Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEOUT_VALUE + fFactor * BALLOON_COMMON_FADEIN_VALUE;

			m_wbBackground.SetAlpha( (RwUInt8)fResult );
			m_pPanel->SetAlpha( (BYTE)fResult );
			m_pText->SetAlpha( (BYTE)fResult );
		}
		else
		{
			m_wbBackground.SetAlpha( BALLOON_COMMON_FADEIN_VALUE );
			m_pPanel->SetAlpha( BALLOON_COMMON_FADEIN_VALUE );
			m_pText->SetAlpha( BALLOON_COMMON_FADEIN_VALUE );
			SetState( SHOW );
		}
	}
	else if( m_eState == FADE_OUT )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade Out Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEIN_VALUE + fFactor * BALLOON_COMMON_FADEOUT_VALUE;

			m_wbBackground.SetAlpha( (RwUInt8)fResult );
			m_pPanel->SetAlpha( (BYTE)fResult );
			m_pText->SetAlpha( (BYTE)fResult );
		}
		else
		{
			m_wbBackground.SetAlpha( (RwUInt32)BALLOON_COMMON_FADEOUT_VALUE );
			m_pPanel->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEOUT_VALUE );
			m_pText->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEOUT_VALUE );
			SetState( HIDE );
		}
	}	
}

gui::CSurface CPrivateShopBalloonGui::GetSurface( RwBool bReady )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	if( bReady )
		return pSurfaceManager->GetSurface( "Balloon.srf", "srfPrivateShopReady" );
	else
		return pSurfaceManager->GetSurface( "Balloon.srf", "srfPrivateShopWait" );
}

VOID CPrivateShopBalloonGui::SetTextBkColor( RwBool bReady )
{
	if( bReady )
		m_pText->SetBkColor( RGB( 16, 143, 145 ), FALSE );
	else
		m_pText->SetBkColor( RGB( 180, 40, 92 ), FALSE );		
}

VOID CPrivateShopBalloonGui::SetTextColor( RwBool bReady )
{
	if( bReady )
		m_pText->SetTextColor( RGB( 255, 255, 255 ), FALSE );		
	else
		m_pText->SetTextColor( RGB( 255, 255, 255 ), FALSE );
}

VOID CPrivateShopBalloonGui::SetBackground(RwBool bReady )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	if( bReady )
	{
		m_wbBackground.SetSurface( 0, pSurfaceManager->GetSurface( "Balloon.srf", "srfPrivateShopReadyLeft" ) );
		m_wbBackground.SetSurface( 1, pSurfaceManager->GetSurface( "Balloon.srf", "srfPrivateShopReadyCenter" ) );
		m_wbBackground.SetSurface( 2, pSurfaceManager->GetSurface( "Balloon.srf", "srfPrivateShopReadyRight" ) );
	}
	else
	{
		m_wbBackground.SetSurface( 0, pSurfaceManager->GetSurface( "Balloon.srf", "srfPrivateShopSaleLeft" ) );
		m_wbBackground.SetSurface( 1, pSurfaceManager->GetSurface( "Balloon.srf", "srfPrivateShopSaleCenter" ) );
		m_wbBackground.SetSurface( 2, pSurfaceManager->GetSurface( "Balloon.srf", "srfPrivateShopSaleRight" ) );
	}
	ResizeBackground();
}

void CPrivateShopBalloonGui::OnMovePanel(RwInt32 param1, RwInt32 param2)
{
	CRectangle rect = m_pPanel->GetScreenRect();
	m_wbBackground.SetRectWH(rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
}

void CPrivateShopBalloonGui::OnPaintPanel()
{
	m_wbBackground.Render();
}

void CPrivateShopBalloonGui::ResizeBackground()
{
	RwInt32 iWidth	= m_pText->GetWidth() - BALLOON_PRIVATESHOP_TEXT_WIDHT;
	RwInt32 iOffset = -(iWidth / 2) + BALLOON_PRIVATESHOP_OFFSET_X;

	m_posOffset.x = iOffset;

	m_pPanel->SetSize(BALLOON_PRIVATESHOP_WIDTH + iWidth, BALLOON_PRIVATESHOP_HEIGHT);

	CRectangle rect = m_pPanel->GetScreenRect();
	m_wbBackground.SetRectWH(rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
}

// QuestBalloon 
// 1. 닫기전까지 혹은 1분간 표시.
// 2. PC 반경 60m.
// 3. NPC 머리위 표시.

CQuestBalloonGui::CQuestBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, std::wstring& wstrHtmlData, RwUInt32 uiID, RwBool bTimeProc )
: CBalloonGui( pOwnerObject, BALLOON_NPC_DISPTIME, posOffsetDelta, TYPE_QUEST ), m_uiID( uiID ), m_bTimeProc( bTimeProc )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	m_pBalloon = NTL_NEW gui::CBalloon( CRectangle( 0, 0, 229, 80 ), NULL, pSurfaceManager, FALSE );

	CRectangle rect;
	rect.SetRectWH( BALLOON_QUEST_IMAGESIZE + BALLOON_QUEST_MARK_OFFSET, BALLOON_NAMEBAR_YOFFSET, BALLOON_NAMEBAR_WIDTH, BALLOON_NAMEBAR_HEIGHT );
	m_pName = NTL_NEW gui::CStaticBox( rect, m_pBalloon, pSurfaceManager, 0, TRUE );
	m_pName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pName->SetTextColor( BALLOON_CHAR_NAME_QUEST_COLOR );
	m_pName->SetEffectMode( TE_OUTLINE );
	m_pName->SetEffectValue( 0 );
	m_pName->SetText( Logic_GetName( pOwnerObject ) );

	rect.SetRectWH( BALLOON_QUEST_MARK_OFFSET, BALLOON_QUEST_MARK_OFFSET, BALLOON_QUEST_IMAGESIZE, BALLOON_QUEST_IMAGESIZE );
	m_pMark = NTL_NEW gui::CPanel( rect, m_pBalloon, pSurfaceManager, pSurfaceManager->GetSurface( "Balloon.srf", "QuestImage" ) );

	// Balloon 속성 세팅
	m_pBalloon->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pBalloon->SetTextColor( RGB( 10, 10, 10 ) );
	m_pBalloon->SetMargin( 5, 0 );
	m_pBalloon->SetMinSize( m_pName->GetWidth() + 20, 20 );
	m_pBalloon->Enable( false );

	m_pBalloon->AddSurfaces( pSurfaceManager->GetSurface( "Balloon.srf", "QuestCenter" ), 
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestLTC" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestRTC" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestLBC" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestRBC" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestLL" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestTL" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestRL" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestBL" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "QuestTail" ) );

	m_pBalloon->SetText( gui::GetHtmlFromMemoryString( wstrHtmlData.c_str(), wstrHtmlData.size() ).c_str() );	
	m_posOffset.SetPos( BALLOON_QUEST_OFFSET_X, BALLOON_COMMON_OFFSET_Y );

	//gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	//CRectangle rect;
	//
	//rect.SetRectWH( 0, 0, BALLOON_QUEST_WIDTH, BALLOON_QUEST_HEIGHT );
	//m_pPanel = NTL_NEW gui::CPanel( rect, NULL, pSurfaceManager, pSurfaceManager->GetSurface( "Balloon.srf", "srfQuestProgressBalloon" ) );
	//
	//rect.SetRectWH( BALLOON_QUEST_TEXT_X, BALLOON_QUEST_TEXT_Y, BALLOON_QUEST_TEXT_WIDHT, BALLOON_QUEST_TEXT_HEIGHT );
	//m_pText = NTL_NEW gui::CStaticBox( rect, m_pPanel, pSurfaceManager, COMP_TEXT_LEFT );
	//m_pText->CreateFontStd( DEFAULT_FONT, DBOGUI_QUESTBALLOON_FONTHEIGHT, DEFAULT_FONT_ATTR );
	//m_pText->SetTextColor( RGB( 10, 10, 10 ) );	
	//m_pText->SetText( gui::GetHtmlFromMemoryString( wstrHtmlData.c_str(), wstrHtmlData.size() ).c_str() );
	//m_pText->Enable( false );	

	//m_posOffset.SetPos( BALLOON_NPC_OFFSET_X, BALLOON_NPC_OFFSET_Y );
}

CQuestBalloonGui::~CQuestBalloonGui(VOID)
{
	NTL_DELETE( m_pBalloon );
}

VOID CQuestBalloonGui::SetMessage( std::wstring& wstrHtmlData, RwBool bTimeProc, RwUInt32 uiID )
{
	m_uiID = uiID;
	m_bTimeProc = bTimeProc;
	m_pBalloon->SetText( gui::GetHtmlFromMemoryString( wstrHtmlData.c_str(), wstrHtmlData.size() ).c_str() );
	SetState( FADE_IN );
}

VOID CQuestBalloonGui::FadeoutMessage(VOID)
{
	m_bTimeProc = TRUE;
	SetState( FADE_OUT );
}

RwInt32 CQuestBalloonGui::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	FadeEffect();

	if( m_eState == HIDE )
		return CBalloonManager::UR_DELETE;

	if( GetGuiFuntor()->GetDistanceFromAvatar( m_pOwnerSobObject->GetPosition() ) > BALLOON_NPC_RANGE )
		return CBalloonManager::UR_NOTSHOW;

	if( !PositionUpdate() )
		return CBalloonManager::UR_NOTSHOW;

	return CBalloonManager::UR_SHOW;
}

VOID CQuestBalloonGui::Paint(VOID)
{
	m_pBalloon->Paint();
}

RwBool CQuestBalloonGui::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset = m_posOffset + m_posOffsetDelta;

	CNtlSobProxy* pProxy = m_pOwnerSobObject->GetSobProxy();

	if( pProxy )
	{
		if( GetGuiFuntor()->Calc3DPosTo2D( m_pOwnerSobObject->GetPosition(), pProxy->GetPLEntityHeight(), &posOffset, &posRes ) )
		{
			m_pBalloon->SetOriginNow( posRes.x, posRes.y );
			return TRUE;
		}
	}
	return FALSE;
}

VOID CQuestBalloonGui::FadeEffect(VOID)
{
	if( m_eState == SHOW )
	{
		if( m_fCurrentTime > m_fLifeTime && m_bTimeProc )
			SetState( FADE_OUT );
	}
	else if( m_eState == FADE_IN )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade In Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEOUT_VALUE + fFactor * BALLOON_COMMON_FADEIN_VALUE;

			m_pBalloon->SetAlpha( (BYTE)fResult );			
		}
		else
		{
			m_pBalloon->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEIN_VALUE );			
			SetState( SHOW );
		}
	}	
	else if( m_eState == FADE_OUT )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade Out Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEIN_VALUE + fFactor * BALLOON_COMMON_FADEOUT_VALUE;

			m_pBalloon->SetAlpha( (BYTE)fResult );
		}
		else
		{
			m_pBalloon->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEOUT_VALUE );
			SetState( HIDE );
		}
	}
}

// NPCMarkBalloon 
// 1. 이벤트에의해 표시, 삭제.
// 2. PC 반경 10m.
// 3. NPC 머리위 표시.

CMarkBalloonGui::CMarkBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, std::wstring& wstrHtmlData )
: CBalloonGui( pOwnerObject, 0.0f, posOffsetDelta, TYPE_NPC_MARK )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	m_pBalloon = NTL_NEW gui::CBalloon( CRectangle( 0, 0, 229, 80 ), NULL, pSurfaceManager, FALSE );

	CRectangle rect;
	rect.SetRectWH( BALLOON_QUEST_IMAGESIZE + BALLOON_QUEST_MARK_OFFSET, BALLOON_NAMEBAR_YOFFSET, BALLOON_NAMEBAR_WIDTH, BALLOON_NAMEBAR_HEIGHT );
	m_pName = NTL_NEW gui::CStaticBox( rect, m_pBalloon, pSurfaceManager, 0, TRUE );
	m_pName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pName->SetTextColor( BALLOON_CHAR_NAME_COLOR );
	m_pName->SetEffectMode( TE_OUTLINE );
	m_pName->SetEffectValue( 0 );
	m_pName->SetText( Logic_GetName( pOwnerObject ) );

	rect.SetRectWH( BALLOON_QUEST_MARK_OFFSET, BALLOON_QUEST_MARK_OFFSET, BALLOON_QUEST_IMAGESIZE, BALLOON_QUEST_IMAGESIZE );
	m_pMark = NTL_NEW gui::CPanel( rect, m_pBalloon, pSurfaceManager, pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkImage" ) );

	// Balloon 속성 세팅
	m_pBalloon->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pBalloon->SetTextColor( RGB( 10, 10, 10 ) );
	m_pBalloon->SetMargin( 5, 0 );
	m_pBalloon->SetMinSize( m_pName->GetWidth() + 20, 20 );
	m_pBalloon->Enable( false );

	m_pBalloon->AddSurfaces( pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkCenter" ), 
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkLTC" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkRTC" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkLBC" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkRBC" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkLL" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkTL" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkRL" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkBL" ),
		pSurfaceManager->GetSurface( "Balloon.srf", "NPCMarkTail" ) );

	m_pBalloon->SetText( gui::GetHtmlFromMemoryString( wstrHtmlData.c_str(), wstrHtmlData.size() ).c_str() );	
	m_posOffset.SetPos( BALLOON_QUEST_OFFSET_X, BALLOON_COMMON_OFFSET_Y );

	//gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	//
	//CRectangle rect;

	//rect.SetRectWH( 0, 0, BALLOON_QUEST_WIDTH, BALLOON_QUEST_HEIGHT );
	//m_pPanel = NTL_NEW gui::CPanel( rect, NULL, pSurfaceManager, pSurfaceManager->GetSurface( "Balloon.srf", "srfQuestProposalBalloon" ) );
	//m_pPanel->Enable( false );

	//rect.SetRectWH( BALLOON_QUEST_TEXT_X, BALLOON_QUEST_TEXT_Y, BALLOON_QUEST_TEXT_WIDHT, BALLOON_QUEST_TEXT_HEIGHT );
	//m_pText = NTL_NEW gui::CStaticBox( rect, m_pPanel, pSurfaceManager, COMP_TEXT_LEFT );
	//m_pText->CreateFontStd( DEFAULT_FONT, DBOGUI_QUESTBALLOON_FONTHEIGHT, DEFAULT_FONT_ATTR );
	//m_pText->SetTextColor( RGB( 10, 10, 10 ) );	
	//m_pText->SetText( gui::GetHtmlFromMemoryString( wstrHtmlData.c_str(), wstrHtmlData.size() ).c_str() );
	//m_pText->Enable( false );	

	//m_posOffset.SetPos( BALLOON_NPC_OFFSET_X, BALLOON_NPC_OFFSET_Y );
}

CMarkBalloonGui::~CMarkBalloonGui(VOID)
{
	NTL_DELETE( m_pBalloon );
}

VOID CMarkBalloonGui::SetMessage( std::wstring& wstrHtmlData )
{
	m_pBalloon->SetText( gui::GetHtmlFromMemoryString( wstrHtmlData.c_str(), wstrHtmlData.size() ).c_str() );
	SetState( FADE_IN );
}

VOID CMarkBalloonGui::FadeoutMessage(VOID)
{
	SetState( FADE_OUT );
}

RwInt32 CMarkBalloonGui::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	FadeEffect();

	if( m_eState == HIDE )
		return CBalloonManager::UR_DELETE;

	if( GetGuiFuntor()->GetDistanceFromAvatar( m_pOwnerSobObject->GetPosition() ) > BALLOON_NPC_RANGE )
		return CBalloonManager::UR_NOTSHOW;

	if( !PositionUpdate() )
		return CBalloonManager::UR_NOTSHOW;

	return CBalloonManager::UR_SHOW;
}

VOID CMarkBalloonGui::Paint(VOID)
{
	m_pBalloon->Paint();
}

RwBool CMarkBalloonGui::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset = m_posOffset + m_posOffsetDelta;

	CNtlSobProxy* pProxy = m_pOwnerSobObject->GetSobProxy();

	if( pProxy )
	{
		if( GetGuiFuntor()->Calc3DPosTo2D( m_pOwnerSobObject->GetPosition(), pProxy->GetPLEntityHeight(), &posOffset, &posRes ) )
		{
			m_pBalloon->SetOriginNow( posRes.x, posRes.y );
			return TRUE;
		}
	}
	return FALSE;
}

VOID CMarkBalloonGui::FadeEffect(VOID)
{
	if( m_eState == FADE_IN )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade In Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEOUT_VALUE + fFactor * BALLOON_COMMON_FADEIN_VALUE;

			m_pBalloon->SetAlpha( (BYTE)fResult );			
		}
		else
		{
			m_pBalloon->SetAlpha( BALLOON_COMMON_FADEIN_VALUE );			
			SetState( SHOW );
		}
	}
	else if( m_eState == FADE_OUT )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade Out Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEIN_VALUE + fFactor * BALLOON_COMMON_FADEOUT_VALUE;

			m_pBalloon->SetAlpha( (BYTE)fResult );			
		}
		else
		{
			m_pBalloon->SetAlpha( BALLOON_COMMON_FADEOUT_VALUE );			
			SetState( HIDE );
		}
	}	
}


// CDiceBalloonGui 
// 1. 이벤트에의해 표시, 삭제.
// 2. PC 반경 10m.
// 3. PC 머리위 표시.

CDiceBalloonGui::CDiceBalloonGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, RwUInt32 uiNumber )
: CBalloonGui( pOwnerObject, BALLOON_CHAT_DISPTIME, posOffsetDelta, TYPE_DICE )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	CRectangle rect;

	gui::CSurface& srfPanelSurface = pSurfaceManager->GetSurface( "Balloon.srf", "srfDiceBalloon" );
	rect.SetRect( srfPanelSurface.m_SnapShot.rtRect.left, 
		srfPanelSurface.m_SnapShot.rtRect.top,
		srfPanelSurface.m_SnapShot.rtRect.right,				  
		srfPanelSurface.m_SnapShot.rtRect.bottom);
	m_pPanel = NTL_NEW gui::CPanel( rect, NULL, pSurfaceManager, srfPanelSurface );
	m_pPanel->Enable( false );

	m_NumberGui.Create(ND_CENTER);
	m_NumberGui.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_0" ));
	m_NumberGui.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_1" ));
	m_NumberGui.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_2" ));
	m_NumberGui.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_3" ));
	m_NumberGui.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_4" ));
	m_NumberGui.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_5" ));
	m_NumberGui.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_6" ));
	m_NumberGui.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_7" ));
	m_NumberGui.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_8" ));
	m_NumberGui.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfDice_9" ));
	m_NumberGui.SetNumber(uiNumber);

	m_posOffset.SetPos( BALLOON_DICE_OFFSET_X, BALLOON_DICE_OFFSET_Y );	
}

CDiceBalloonGui::~CDiceBalloonGui(VOID)
{
	NTL_DELETE( m_pPanel );
}

VOID CDiceBalloonGui::SetMessage( RwUInt32 uiNumber )
{
	m_NumberGui.SetNumber(uiNumber);
	SetState( FADE_IN );
}

VOID CDiceBalloonGui::FadeoutMessage(VOID)
{
	SetState( FADE_OUT );
}

RwInt32 CDiceBalloonGui::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	FadeEffect();

	if( m_eState == HIDE )
		return CBalloonManager::UR_DELETE;

	if( GetGuiFuntor()->GetDistanceFromAvatar( m_pOwnerSobObject->GetPosition() ) > BALLOON_NPC_RANGE )
		return CBalloonManager::UR_NOTSHOW;

	if( !PositionUpdate() )
		return CBalloonManager::UR_NOTSHOW;

	return CBalloonManager::UR_SHOW;
}

VOID CDiceBalloonGui::Paint(VOID)
{
	m_pPanel->Paint();
	m_NumberGui.Render();
}

RwBool CDiceBalloonGui::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset = m_posOffset + m_posOffsetDelta;

	CNtlSobProxy* pProxy = m_pOwnerSobObject->GetSobProxy();

	if( pProxy )
	{
		if( GetGuiFuntor()->Calc3DPosTo2D( m_pOwnerSobObject->GetPosition(), pProxy->GetPLEntityHeight(), &posOffset, &posRes ) )
		{
			m_pPanel->SetPosition( posRes.x, posRes.y );

			CRectangle rtPanel = m_pPanel->GetScreenRect();
			m_NumberGui.SetPosition( rtPanel.left + 47, rtPanel.top + 25 );

			return TRUE;
		}
	}
	return FALSE;
}

VOID CDiceBalloonGui::FadeEffect(VOID)
{
	if( m_eState == SHOW )
	{
		if( m_fCurrentTime > m_fLifeTime )
			SetState( FADE_OUT );
	}
	else if( m_eState == FADE_IN )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade In Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEOUT_VALUE + fFactor * BALLOON_COMMON_FADEIN_VALUE;

			m_pPanel->SetAlpha( (BYTE)fResult );
		}
		else
		{
			m_pPanel->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEIN_VALUE );
			SetState( SHOW );
		}
	}
	else if( m_eState == FADE_OUT )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade Out Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEIN_VALUE + fFactor * BALLOON_COMMON_FADEOUT_VALUE;

			m_pPanel->SetAlpha( (BYTE)fResult );
		}
		else
		{
			m_pPanel->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEOUT_VALUE );
			SetState( HIDE );
		}
	}
}


// BattleGauge 
// 1. 표시여부는 명령에의해서만 처리
// 2. PC 반경 30m

CBattleGaugeGui::CBattleGaugeGui( CNtlSob* pOwnerObject, CPos& posOffsetDelta, const WCHAR* pName, RwUInt32 uiMaxLp, RwUInt32 uiLp, RwBool bMyTeam )
: CBalloonGui( pOwnerObject, 0.0f, posOffsetDelta, TYPE_BATTLEGAUGE )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();

	CRectangle rect;
	gui::CSurface surface;

	if( bMyTeam )
		surface = pSurfaceManager->GetSurface( "GameCommon.srf", "srfRBGauge1Back" );
	else
		surface = pSurfaceManager->GetSurface( "GameCommon.srf", "srfRBGauge2Back" );

	rect.SetRectWH( 0, 0, BALLOON_BATTLEGAUGE_WIDTH, BALLOON_BATTLEGAUGE_HEIGHT );
	m_pBack = NTL_NEW gui::CPanel( rect, NULL, pSurfaceManager, surface );
	m_pBack->Enable( false );

	if( bMyTeam )
		surface = pSurfaceManager->GetSurface( "GameCommon.srf", "srfRBGauge1" );
	else
		surface = pSurfaceManager->GetSurface( "GameCommon.srf", "srfRBGauge2" );

	m_pLPGauge = NTL_NEW gui::CProgressBar( rect, m_pBack, pSurfaceManager, surface );
	m_pLPGauge->Enable( false );
	m_pLPGauge->SetRange( 0, uiMaxLp );
	m_pLPGauge->SetPos( uiLp );

	RwUInt32 namecolor = RGB( 255, 255, 255 );

	if( pOwnerObject->GetClassID() == SLCLASS_AVATAR )
		namecolor = NTL_PLAYER_NAME_COLOR;
	else if( pOwnerObject->GetClassID() == SLCLASS_PLAYER )
	{
		CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
		if( pRankBattle->GetMyTeamMap()->find( pOwnerObject->GetSerialID() ) != pRankBattle->GetMyTeamMap()->end() )
			namecolor = NTL_PARTY_NAME_COLOR;
		else
			namecolor = NTL_MONSTER_NAME_COLOR;
	}
	else if( pOwnerObject->GetClassID() == SLCLASS_MONSTER )
		namecolor = NTL_MONSTER_NAME_COLOR;

	rect.SetRectWH( BALLOON_BATTLEGAUGE_TEXT_X, BALLOON_BATTLEGAUGE_TEXT_Y, BALLOON_BATTLEGAUGE_TEXT_WIDHT, BALLOON_BATTLEGAUGE_TEXT_HEIGHT );
	m_pName = NTL_NEW gui::CStaticBox( rect, m_pBack, pSurfaceManager, COMP_TEXT_CENTER );
	m_pName->CreateFontStd( DEFAULT_FONT, BALLOON_BATTLEGAUGE_FONT_HEIGHT, DEFAULT_FONT_ATTR );
	m_pName->SetTextColor( namecolor );	
	m_pName->SetEffectMode( TE_SHADOW );
	m_pName->SetEffectValue( DEFAULT_SHADOW_EFFECT_VALUE );
	m_pName->SetText( pName );	
	m_pName->Enable( false );

	m_posOffset.SetPos( BALLOON_BATTLEGAUGE_OFFSET_X, BALLOON_BATTLEGAUGE_OFFSET_Y );	
}

CBattleGaugeGui::~CBattleGaugeGui(VOID)
{
	NTL_DELETE( m_pName );
	NTL_DELETE( m_pLPGauge );
	NTL_DELETE( m_pBack );
}

VOID CBattleGaugeGui::SetData( RwUInt32 uiMaxLp, RwUInt32 uiLp )
{
	m_pLPGauge->SetRange( 0, uiMaxLp );
	m_pLPGauge->SetPos( uiLp );	
}

VOID CBattleGaugeGui::FadeoutMessage(VOID)
{
	SetState( FADE_OUT );
}

RwInt32 CBattleGaugeGui::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	FadeEffect();

	if( m_eState == HIDE )
		return CBalloonManager::UR_DELETE;

	if( GetGuiFuntor()->GetDistanceFromAvatar( m_pOwnerSobObject->GetPosition() ) > BALLOON_BATTLEGAUGE_RANGE )
		return CBalloonManager::UR_NOTSHOW;

	if( !PositionUpdate() )
		return CBalloonManager::UR_NOTSHOW;

	return CBalloonManager::UR_SHOW;
}

VOID CBattleGaugeGui::Paint(VOID)
{
	m_pBack->Paint();
}

RwBool CBattleGaugeGui::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset = m_posOffset + m_posOffsetDelta;

	CNtlSobProxy* pProxy = m_pOwnerSobObject->GetSobProxy();

	if( pProxy )
	{
		if( GetGuiFuntor()->Calc3DPosTo2D( m_pOwnerSobObject->GetPosition(), pProxy->GetPLEntityHeight(), &posOffset, &posRes ) )
		{
			m_pBack->SetPosition( posRes.x, posRes.y );
			return TRUE;
		}
	}
	return FALSE;
}

VOID CBattleGaugeGui::FadeEffect(VOID)
{
	if( m_eState == FADE_IN )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade In Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEOUT_VALUE + fFactor * BALLOON_COMMON_FADEIN_VALUE;

			m_pBack->SetAlpha( (BYTE)fResult );			
		}
		else
		{
			m_pBack->SetAlpha( BALLOON_COMMON_FADEIN_VALUE );			
			SetState( SHOW );
		}
	}
	else if( m_eState == FADE_OUT )
	{
		if( m_fCurrentTime < BALLOON_COMMON_FADETIME )
		{
			// Fade Out Time
			RwReal fFactor = m_fCurrentTime / BALLOON_COMMON_FADETIME;
			RwReal fResult = ( 1 - fFactor ) * BALLOON_COMMON_FADEIN_VALUE + fFactor * BALLOON_COMMON_FADEOUT_VALUE;

			m_pBack->SetAlpha( (BYTE)fResult );			
		}
		else
		{
			m_pBack->SetAlpha( (RwUInt32)BALLOON_COMMON_FADEOUT_VALUE );			
			SetState( HIDE );
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////
// Manager
CBalloonManager* CBalloonManager::m_pInstance = NULL;
gui::CStaticBox* CBalloonManager::m_pTestText = NULL;

CBalloonManager::CBalloonManager( const RwChar* pName )
: CNtlPLGui( pName ), m_bAvatarDie(false)
{
	m_uiVisibleBalloonType = CBalloonGui::ALL;	
}

CBalloonManager::~CBalloonManager(VOID)
{
}

RwBool CBalloonManager::CreateInstance(VOID)
{
	NTL_FUNCTION( "CBalloonManager : CreateInstance" );

	if( m_pInstance )
		NTL_RETURN( TRUE );

	m_pInstance = NTL_NEW CBalloonManager( "BalloonManager" );
	NTL_ASSERT( m_pInstance, "CBalloonManager::CreateInstance : Instance Allocate Fail" );

	if( !m_pInstance->Create( "", "gui\\Balloon.srf", "gui\\Balloon.frm" ) )
		NTL_RETURN( FALSE );
	CNtlPLGuiManager::GetInstance()->AddGui( m_pInstance );

	m_pInstance->CreateComponents( GetNtlGuiManager()->GetGuiManager() );
	m_pInstance->m_pThis = reinterpret_cast<gui::CDialog*>( m_pInstance->GetComponent( "dlgMain" ) );
	m_pInstance->m_pThis->Show( true );
	m_pInstance->m_pThis->Enable( false );
	m_pInstance->m_pThis->SetPriority( dDIALOGPRIORITY_CHAT_BALLOON );

	// TestText 
	CRectangle rect;
	rect.SetRectWH( 400, 300, 211, 70 );
	m_pInstance->m_pTestText = NTL_NEW gui::CStaticBox( rect, m_pInstance->m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pInstance->m_pTestText->CreateFontStd( DEFAULT_FONT, 95, DEFAULT_FONT_ATTR );
	m_pInstance->m_pTestText->Enable( false );
	m_pInstance->m_pTestText->Show( false );

	// Update 연결
	GetNtlGuiManager()->AddUpdateFunc( m_pInstance );

	// CallBack 연결
	m_pInstance->m_slotPaint = m_pInstance->m_pThis->SigPaint().Connect( m_pInstance, &CBalloonManager::OnPaint );

	// Event 연결
	m_pInstance->LinkMsg( g_EventSobDelete, 0, 0x9000 );
	//m_pInstance->LinkMsg( g_EventCharObjDelete, 0, 0x9000 );	// 객체가 지워지기 전에 지운다. 
	m_pInstance->LinkMsg( g_EventQuestMark, 0 );
	m_pInstance->LinkMsg( g_EventServerCharDialog, 0 );
	m_pInstance->LinkMsg( g_EventSobInfoUpdate, 0 );
	m_pInstance->LinkMsg( g_EventCharReady, 0 );
	m_pInstance->LinkMsg( g_EventTSPCDialog_Nfy, 0 );
	m_pInstance->LinkMsg( g_EventSobProxyGUIUpdate );
	m_pInstance->LinkMsg( g_EventDiceResultShow );
	m_pInstance->LinkMsg( g_EventSobShowNfy );	

	NTL_RETURN( TRUE );
}

VOID CBalloonManager::DeleteInstance(VOID)
{
	if(m_pInstance == NULL)
		return;

	m_pInstance->RemoveAllBalloonData();

	// TestText
	NTL_DELETE( m_pInstance->m_pTestText );

	// Update 연결해제
	GetNtlGuiManager()->RemoveUpdateFunc( m_pInstance );

	// CallBack 연결 해제
	m_pInstance->m_pThis->SigPaint().Disconnect(m_pInstance->m_slotPaint);

	// Event 연결해제
	m_pInstance->UnLinkMsg( g_EventSobDelete );
	//m_pInstance->UnLinkMsg( g_EventCharObjDelete );
	m_pInstance->UnLinkMsg( g_EventQuestMark );
	m_pInstance->UnLinkMsg( g_EventServerCharDialog );
	m_pInstance->UnLinkMsg( g_EventSobInfoUpdate );
	m_pInstance->UnLinkMsg( g_EventCharReady );
	m_pInstance->UnLinkMsg( g_EventTSPCDialog_Nfy );
	m_pInstance->UnLinkMsg( g_EventSobProxyGUIUpdate );
	m_pInstance->UnLinkMsg( g_EventDiceResultShow );
	m_pInstance->UnLinkMsg( g_EventSobShowNfy );

	m_pInstance->DestroyComponents();
	m_pInstance->Destroy();	

	if(m_pInstance)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pInstance );
		NTL_DELETE(m_pInstance);
		m_pInstance = NULL;
	}
}

CBalloonManager* CBalloonManager::GetInstance(VOID)
{
	return m_pInstance;
}

gui::CStaticBox* CBalloonManager::GetTestText(VOID)
{
	return m_pTestText;
}

VOID CBalloonManager::Update( RwReal fElapsed )
{
	m_nDisplayCount = 0;

	if( m_mmapTypeBalloon.empty() )
		return;

	RwInt32 i = 0;
	memset( m_aDiaplsyBalloon, 0, sizeof( m_aDiaplsyBalloon ) );

	for( MMAP_TYPE_BALLOON_IT it = m_mmapTypeBalloon.begin() ; it != m_mmapTypeBalloon.end() ; )
	{
		CBalloonGui* pBalloon = (*it).second;
		RwInt32 nUpdateResult = pBalloon->Update( fElapsed ); 

		if( i >= BALLOON_MAX_PRESENT )
			break;

		if( nUpdateResult == UR_SHOW )
		{
			if( pBalloon->GetType() & m_uiVisibleBalloonType && pBalloon->GetVisible() )
			{
				m_aDiaplsyBalloon[i].fDist = GetGuiFuntor()->GetDistanceFromCamera( pBalloon->GetOwnerObject()->GetPosition() );
				m_aDiaplsyBalloon[i].pBalloon = pBalloon;
				++m_nDisplayCount;
				++it;
				++i;
			}
			else
				++it;
		}
		else if( nUpdateResult == UR_NOTSHOW )	
		{
			++it;
		}
		else
		{
			it = m_mmapTypeBalloon.erase( it );
			RemoveBalloonData( pBalloon, FALSE );							
		}				
	}

	// 거리순 정렬
	qsort( m_aDiaplsyBalloon, m_nDisplayCount, sizeof( stDISPLAYBALLOON ), &CBalloonManager::Compare );
}

RwInt32 CBalloonManager::Compare( const VOID* valLeft, const VOID* valRight )
{
	// 거리가 먼 것을 먼저 찍히도록 앞으로 정렬
	stDISPLAYBALLOON* pLeft = (stDISPLAYBALLOON*)valLeft;
	stDISPLAYBALLOON* pRight= (stDISPLAYBALLOON*)valRight;

	if( pLeft->fDist == pRight->fDist ) 
		return 0;

	if( pLeft->fDist < pRight->fDist )
		return 1;

	return -1;
}

VOID CBalloonManager::AddAvatarBalloon( const WCHAR* pMessage, RwBool bPartyMsg /*= FALSE*/)
{
	CBalloonGui* pBalloon = NULL;
	CPos posOffsetDelta;

	// ObjectClickGui가 떠있을때에는 말풍선 표시 안함.
	if( GetDialogManager()->IsOpenDialog( DIALOG_OBJECT_CLICK ) )
		return;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if(pAvatar->IsVisible() == false && pAvatar->GetVehicleID() == INVALID_SERIAL_ID)
		return;

	if( AlreadyExist(pAvatar) )
	{
		RwBool	bNeedToCreate = TRUE;
		CBalloonGui* pDeleteBalloon = NULL;

		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range(pAvatar);

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_AVATAR_CHAT );

			if( ePriority == CBalloonGui::LESS_PRIORITY )
			{
				pDeleteBalloon = pBalloon;
			}
			else if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				bNeedToCreate = FALSE;
				pBalloon->SetMessage( pMessage );
			}
			else if( ePriority == CBalloonGui::MORE_PRIORITY )
			{
				bNeedToCreate = FALSE;
			}
			else if( ePriority == CBalloonGui::LOWER_LAYER )
			{
				RwInt32 nSubHeight;
				if( pBalloon->GetType() == CBalloonGui::TYPE_PRIVATESHOP )
					nSubHeight = BALLOON_SUB_PRIVATESHOP_HEIGHT;
				else
					nSubHeight = BALLOON_SUB_BATTLEGAUGE_HEIGHT;

				posOffsetDelta.y += pBalloon->GetOffset().y + nSubHeight;				
			}				
		}

		if( pDeleteBalloon )
			RemoveBalloonData( pBalloon );

		if( bNeedToCreate )
		{
			// 생성되어 있던 오브젝트의 오프셋을 수정할 필요는 없다.
			pBalloon = NTL_NEW CChatBalloonGui(pAvatar, posOffsetDelta, pMessage, CBalloonGui::TYPE_AVATAR_CHAT, bPartyMsg);
			AddBalloonData( pBalloon );
		}
	}
	else
	{
		pBalloon = NTL_NEW CChatBalloonGui(pAvatar, posOffsetDelta, pMessage, CBalloonGui::TYPE_AVATAR_CHAT, bPartyMsg);
		AddBalloonData( pBalloon );
	}
}

VOID CBalloonManager::AddPlayerBalloon( CNtlSob* pSobObject, const WCHAR* pMessage, RwBool bPartyMsg /*= FALSE*/)
{
	if( GetGuiFuntor()->GetDistanceFromAvatar( pSobObject->GetPosition() ) > BALLOON_CHAT_RANGE )
		return;

	if(pSobObject->IsVisible() == false && pSobObject->GetVehicleID() == INVALID_SERIAL_ID)
		return;

	CBalloonGui* pBalloon = NULL;
	CBalloonGui* pDeleteBalloon = NULL;
	CPos posOffsetDelta;

	if( AlreadyExist( pSobObject ) )
	{
		RwBool	bNeedToCreate = TRUE;

		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_PLAYER_CHAT );

			if( ePriority == CBalloonGui::LESS_PRIORITY )
			{
				pDeleteBalloon = pBalloon;
			}
			else if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				bNeedToCreate = FALSE;			
				pBalloon->SetMessage( pMessage );
			}
			else if( ePriority == CBalloonGui::MORE_PRIORITY )
			{
				bNeedToCreate = FALSE;
			}
			else if( ePriority == CBalloonGui::LOWER_LAYER )
			{
				RwInt32 nSubHeight;
				if( pBalloon->GetType() == CBalloonGui::TYPE_PRIVATESHOP )
					nSubHeight = BALLOON_SUB_PRIVATESHOP_HEIGHT;
				else
					nSubHeight = BALLOON_SUB_BATTLEGAUGE_HEIGHT;

				posOffsetDelta.y += pBalloon->GetOffset().y + nSubHeight;
			}				
		}

		if( pDeleteBalloon )
			RemoveBalloonData( pBalloon );

		if( bNeedToCreate )
		{
			// 생성되어 있던 오브젝트의 오프셋을 수정할 필요는 없다.
			if( IsEnableToCreate( CBalloonGui::TYPE_PLAYER_CHAT ) )
			{
				pBalloon = NTL_NEW CChatBalloonGui( pSobObject, posOffsetDelta, pMessage, CBalloonGui::TYPE_PLAYER_CHAT, bPartyMsg);
				AddBalloonData( pBalloon );
			}
		}
	}
	else
	{
		if( IsEnableToCreate( CBalloonGui::TYPE_PLAYER_CHAT ) )
		{
			pBalloon = NTL_NEW CChatBalloonGui( pSobObject, posOffsetDelta, pMessage, CBalloonGui::TYPE_PLAYER_CHAT, bPartyMsg);
			AddBalloonData( pBalloon );
		}		
	}
}

VOID CBalloonManager::AddNPCBalloon( CNtlSob* pSobObject, const WCHAR* pMessage, RwReal fLifeTime, RwBool bServer /* = FALSE */, RwBool bTimeProc /* = TRUE */, RwUInt8 byBalloonType /* = 6  */)
{
	if( !pSobObject->IsVisible() )
		return;

	CBalloonGui* pBalloon = NULL;
	CBalloonGui* pDeleteBalloon = NULL;
	CPos posOffsetDelta;
	CBalloonGui::TYPE eType = bServer ? CBalloonGui::TYPE_SERVER : CBalloonGui::TYPE_NPC;

	if( AlreadyExist( pSobObject ) )
	{
		RwBool	bNeedToCreate = TRUE;

		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( eType );

			if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				CNPCBalloonGui* pNPCBalloon = reinterpret_cast<CNPCBalloonGui*>( pBalloon );

				if( pNPCBalloon->GetBalloonType() == byBalloonType )
				{
					bNeedToCreate = FALSE;		

					if( wcscmp( reinterpret_cast<CNPCBalloonGui*>( pBalloon )->GetText().c_str(), pMessage ) != 0 )
						pBalloon->SetMessage( pMessage, bTimeProc );					
				}				
				else
				{
					pDeleteBalloon = pBalloon;
				}
			}
			else if( ePriority == CBalloonGui::LESS_PRIORITY )
			{
				pDeleteBalloon = pBalloon;
			}
			else if( ePriority == CBalloonGui::MORE_PRIORITY )
			{
				bNeedToCreate = FALSE;
			}
			else if( ePriority == CBalloonGui::LOWER_LAYER )
			{
				RwInt32 nSubHeight;
				if( pBalloon->GetType() == CBalloonGui::TYPE_PRIVATESHOP )
					nSubHeight = BALLOON_SUB_PRIVATESHOP_HEIGHT;
				else
					nSubHeight = BALLOON_SUB_BATTLEGAUGE_HEIGHT;

				posOffsetDelta.y += pBalloon->GetOffset().y + nSubHeight;
			}				
		}

		if( pDeleteBalloon )
			RemoveBalloonData( pBalloon );				

		if( bNeedToCreate )
		{
			// 생성되어 있던 오브젝트의 오프셋을 수정할 필요는 없다.
			pBalloon = NTL_NEW CNPCBalloonGui( pSobObject, posOffsetDelta, pMessage, fLifeTime, eType, byBalloonType, bTimeProc );
			AddBalloonData( pBalloon );
		}
	}
	else
	{
		pBalloon = NTL_NEW CNPCBalloonGui( pSobObject, posOffsetDelta, pMessage, fLifeTime, eType, byBalloonType, bTimeProc );
		AddBalloonData( pBalloon );
	}
}

VOID CBalloonManager::AddPrivateShopBalloon( CNtlSob* pSobObject, const WCHAR* pMessage, RwBool bReady )
{
	//if( !pSobObject->IsVisible() )
	//	return;

	CBalloonGui* pBalloon = NULL;
	CBalloonGui* pDeleteBalloon = NULL;
	CPos posOffsetDelta;

	if( AlreadyExist( pSobObject ) )
	{
		RwBool	bNeedToCreate = TRUE;

		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_PRIVATESHOP );

			if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				bNeedToCreate = FALSE;			
				pBalloon->SetMessage( pMessage, bReady );
			}
			else if( ePriority == CBalloonGui::LESS_PRIORITY )
			{
				pDeleteBalloon = pBalloon;
			}
			else if( ePriority == CBalloonGui::MORE_PRIORITY)
			{
				bNeedToCreate = FALSE;
			}
			else if( ePriority == CBalloonGui::UPPER_LAYER )
			{
				pBalloon->SetOffsetDelta( CPos( 0, BALLOON_PRIVATESHOP_OFFSET_Y + BALLOON_SUB_PRIVATESHOP_HEIGHT ) );				
			}							
		}

		if( pDeleteBalloon )
			RemoveBalloonData( pDeleteBalloon );

		if( bNeedToCreate )
		{
			pBalloon = NTL_NEW CPrivateShopBalloonGui( pSobObject, posOffsetDelta, bReady, pMessage );
			AddBalloonData( pBalloon );
		}
	}
	else
	{
		pBalloon = NTL_NEW CPrivateShopBalloonGui( pSobObject, posOffsetDelta, bReady, pMessage );
		AddBalloonData( pBalloon );
	}
}

VOID CBalloonManager::AddQuestBalloon( CNtlSob* pSobObject, std::wstring& wstrHtmlData, RwUInt32 uiID, RwBool bTimeProc /* = TRUE  */ )
{
	if( !pSobObject->IsVisible() )
		return;

	CBalloonGui* pBalloon = NULL;
	CBalloonGui* pDeleteBalloon = NULL;
	CPos posOffsetDelta;

	if( AlreadyExist( pSobObject ) )
	{
		RwBool	bNeedToCreate = TRUE;

		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_QUEST );

			if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				bNeedToCreate = FALSE;			
				if( reinterpret_cast<CQuestBalloonGui*>( pBalloon )->GetTextID() != uiID )
					pBalloon->SetMessage( wstrHtmlData, bTimeProc, uiID );
			}
			else if( ePriority == CBalloonGui::LESS_PRIORITY )
			{
				pDeleteBalloon = pBalloon;
			}
			else if( ePriority == CBalloonGui::MORE_PRIORITY )
			{
				bNeedToCreate = FALSE;
			}
			else if( ePriority == CBalloonGui::LOWER_LAYER )
			{
				RwInt32 nSubHeight;
				if( pBalloon->GetType() == CBalloonGui::TYPE_PRIVATESHOP )
					nSubHeight = BALLOON_SUB_PRIVATESHOP_HEIGHT;
				else
					nSubHeight = BALLOON_SUB_BATTLEGAUGE_HEIGHT;

				posOffsetDelta.y += pBalloon->GetOffset().y + nSubHeight;			
			}				
		}

		if( pDeleteBalloon )
			RemoveBalloonData( pBalloon );				

		if( bNeedToCreate )
		{
			// 생성되어 있던 오브젝트의 오프셋을 수정할 필요는 없다.
			pBalloon = NTL_NEW CQuestBalloonGui( pSobObject, posOffsetDelta, wstrHtmlData, uiID, bTimeProc );
			AddBalloonData( pBalloon );
		}
	}
	else
	{
		pBalloon = NTL_NEW CQuestBalloonGui( pSobObject, posOffsetDelta, wstrHtmlData, uiID, bTimeProc );
		AddBalloonData( pBalloon );
	}
}

VOID CBalloonManager::AddMarkBalloon( CNtlSob* pSobObject, std::wstring& wstrHtmlData )
{
	if( !pSobObject->IsVisible() )
		return;

	CBalloonGui* pBalloon = NULL;
	CBalloonGui* pDeleteBalloon = NULL;
	CPos posOffsetDelta;

	if( AlreadyExist( pSobObject ) )
	{
		RwBool	bNeedToCreate = TRUE;		

		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_DICE );

			if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				bNeedToCreate = FALSE;			
				pBalloon->SetMessage( wstrHtmlData );
			}
			else if( ePriority == CBalloonGui::LESS_PRIORITY )
			{
				pDeleteBalloon = pBalloon;
			}
			else if( ePriority == CBalloonGui::MORE_PRIORITY )
			{
				bNeedToCreate = FALSE;
			}
			else if( ePriority == CBalloonGui::LOWER_LAYER )
			{
				RwInt32 nSubHeight;
				if( pBalloon->GetType() == CBalloonGui::TYPE_PRIVATESHOP )
					nSubHeight = BALLOON_SUB_PRIVATESHOP_HEIGHT;
				else
					nSubHeight = BALLOON_SUB_BATTLEGAUGE_HEIGHT;

				posOffsetDelta.y += pBalloon->GetOffset().y + nSubHeight;			
			}				
		}

		if( pDeleteBalloon )
			RemoveBalloonData( pBalloon );				

		if( bNeedToCreate )
		{
			// 생성되어 있던 오브젝트의 오프셋을 수정할 필요는 없다.
			pBalloon = NTL_NEW CMarkBalloonGui( pSobObject, posOffsetDelta, wstrHtmlData );
			AddBalloonData( pBalloon );
		}
	}
	else
	{
		pBalloon = NTL_NEW CMarkBalloonGui( pSobObject, posOffsetDelta, wstrHtmlData );
		AddBalloonData( pBalloon );		
	}
}

VOID CBalloonManager::AddDiceBalloon( CNtlSob* pSobObject, RwUInt32 uiNumber )
{
	if( !pSobObject->IsVisible() )
		return;

	CBalloonGui* pBalloon = NULL;
	CBalloonGui* pDeleteBalloon = NULL;
	CPos posOffsetDelta;

	if( AlreadyExist( pSobObject ) )
	{
		RwBool	bNeedToCreate = TRUE;		

		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_DICE );

			if( ePriority == CBalloonGui::LESS_PRIORITY )
			{		
				pDeleteBalloon = pBalloon;
			}
			else if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				bNeedToCreate = FALSE;			
				pBalloon->SetMessage( uiNumber );
			}
			else if( ePriority == CBalloonGui::MORE_PRIORITY )
			{
				bNeedToCreate = FALSE;
			}
			else if( ePriority == CBalloonGui::LOWER_LAYER )
			{
				RwInt32 nSubHeight;
				if( pBalloon->GetType() == CBalloonGui::TYPE_PRIVATESHOP )
					nSubHeight = BALLOON_SUB_PRIVATESHOP_HEIGHT;
				else
					nSubHeight = BALLOON_SUB_BATTLEGAUGE_HEIGHT;

				posOffsetDelta.y += pBalloon->GetOffset().y + nSubHeight;			
			}				
		}

		if( pDeleteBalloon )
			RemoveBalloonData( pBalloon );

		if( bNeedToCreate )
		{
			// 생성되어 있던 오브젝트의 오프셋을 수정할 필요는 없다.
			pBalloon = NTL_NEW CDiceBalloonGui( pSobObject, posOffsetDelta, uiNumber );
			AddBalloonData( pBalloon );
		}
	}
	else
	{
		pBalloon = NTL_NEW CDiceBalloonGui( pSobObject, posOffsetDelta, uiNumber );
		AddBalloonData( pBalloon );		
	}
}

VOID CBalloonManager::AddBattleGauge( CNtlSob* pSobObject, const WCHAR* pName, RwUInt32 uiMaxLp, RwUInt32 uiLp, RwBool bMyTeam )
{
	if( !pSobObject->IsVisible() )
		return;

	CBalloonGui* pBalloon = NULL;
	CBalloonGui* pDeleteBalloon = NULL;
	CPos posOffsetDelta;

	if( AlreadyExist( pSobObject ) )
	{
		RwBool	bNeedToCreate = TRUE;

		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_BATTLEGAUGE );

			if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				bNeedToCreate = FALSE;			
				CBattleGaugeGui* pBattleGauge = reinterpret_cast<CBattleGaugeGui*>( pBalloon );
				pBattleGauge->SetData( uiMaxLp, uiLp );
			}
			else if( ePriority == CBalloonGui::MORE_PRIORITY )
			{
				bNeedToCreate = FALSE;
			}
			else if( ePriority == CBalloonGui::LESS_PRIORITY )
			{
				pDeleteBalloon = pBalloon;
			}
			else if( ePriority == CBalloonGui::UPPER_LAYER )
			{
				pBalloon->SetOffsetDelta( CPos( 0, BALLOON_PRIVATESHOP_OFFSET_Y + BALLOON_SUB_BATTLEGAUGE_HEIGHT ) );				
			}				
		}

		if( pDeleteBalloon )
			RemoveBalloonData( pDeleteBalloon );

		if( bNeedToCreate )
		{
			pBalloon = NTL_NEW CBattleGaugeGui( pSobObject, posOffsetDelta, pName, uiMaxLp, uiLp, bMyTeam );
			AddBalloonData( pBalloon );			
		}
	}
	else
	{
		pBalloon = NTL_NEW CBattleGaugeGui( pSobObject, posOffsetDelta, pName, uiMaxLp, uiLp, bMyTeam );
		AddBalloonData( pBalloon );
	}
}

VOID CBalloonManager::FadeOutBalloon( CNtlSob* pSobObject, RwUInt32 eBalloonType )
{
	MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

	for( MMAP_OBJ_BALLOON_IT it = objitpair.first ; it != objitpair.second ; ++it )
	{
		CBalloonGui* pBalloon = (*it).second;

		if( eBalloonType & pBalloon->GetType() )
			pBalloon->FadeoutMessage();
	}
}

VOID CBalloonManager::SetBattleGaugeData( CNtlSob* pSobObject, RwUInt32 uiMaxLp, RwUInt32 uiLp )
{
	CBalloonGui* pBalloon = NULL;

	if( AlreadyExist( pSobObject ) )
	{
		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;
			RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_BATTLEGAUGE );

			if( ePriority == CBalloonGui::EQUAL_PRIORITY )
			{
				CBattleGaugeGui* pBattleGauge = reinterpret_cast<CBattleGaugeGui*>( pBalloon );
				pBattleGauge->SetData( uiMaxLp, uiLp );
			}				
		}
	}
}

VOID CBalloonManager::VisibleBalloon( CNtlSob* pSobObject, RwBool bVisible, RwInt32 flagExceptType )
{
	CBalloonGui* pBalloon = NULL;

	if( AlreadyExist( pSobObject ) )
	{
		MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

		for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
		{
			pBalloon = (*objit).second;

			if( !( pBalloon->GetType() & flagExceptType ) )
				pBalloon->SetVisible( bVisible );
		}
	}
}

CBalloonManager::MMAP_OBJ_BALLOON::size_type CBalloonManager::AlreadyExist( CNtlSob* pSobObject )
{
	return m_mmapObjBalloon.count( pSobObject );
}

VOID CBalloonManager::AddBalloonData( CBalloonGui* pGui )
{
	CNtlSob* pSobObject = pGui->GetOwnerObject();

	if( !AlreadyExist( pSobObject ) )
		pGui->Show3DName( FALSE );

	if( pGui->GetType() == CBalloonGui::TYPE_QUEST )
	{		
		if( pSobObject->GetClassID() == SLCLASS_NPC )
			reinterpret_cast<CNtlSobNpc*>( pSobObject )->ActiveQuestMark( FALSE );
		else if( pSobObject->GetClassID() == SLCLASS_TRIGGER_OBJECT )
			reinterpret_cast<CNtlSobTriggerObject*>( pSobObject )->ActiveQuestMark( FALSE );
	}	

	m_mmapObjBalloon.insert( std::pair<CNtlSob*,CBalloonGui*>( pSobObject, pGui ) );
	m_mmapTypeBalloon.insert( std::pair<RwInt32,CBalloonGui*>( pGui->GetType(), pGui ) );	
}

VOID CBalloonManager::RemoveBalloonData( CBalloonGui* pGui, RwBool bDeleteFromTypeMap /* = TRUE  */)
{
	if( pGui->GetType() == CBalloonGui::TYPE_QUEST )
	{
		if( pGui->GetOwnerObject()->GetClassID() == SLCLASS_NPC )
			reinterpret_cast<CNtlSobNpc*>( pGui->GetOwnerObject() )->ActiveQuestMark( TRUE );
		else
			reinterpret_cast<CNtlSobTriggerObject*>( pGui->GetOwnerObject() )->ActiveQuestMark( TRUE );					
	}	

	if( AlreadyExist( pGui->GetOwnerObject() ) == 1 )
		pGui->Show3DName( TRUE );

	if( bDeleteFromTypeMap )
	{
		MMAP_TYPE_IT_PAIR typeitpair = m_mmapTypeBalloon.equal_range( pGui->GetType() );

		for( MMAP_TYPE_BALLOON_IT typeit = typeitpair.first ; typeit != typeitpair.second ; ++typeit )
		{
			if( (*typeit).second == pGui )
			{ 
				m_mmapTypeBalloon.erase( typeit );		
				break;
			}
		}
	}	

	MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pGui->GetOwnerObject() );

	RwInt32 eBalloonType = pGui->GetType();

	for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; )
	{
		if( eBalloonType & CBalloonGui::SECOND_TYPE_LAYER )
		{
			if( (*objit).second->GetType() & CBalloonGui::FIRST_TYPE_LAYER )
				(*objit).second->SetOffsetDelta( CPos( 0, 0 ) );
		}

		if( (*objit).second == pGui )
		{
			RemoveBalloonDataFromRenderQueue( pGui );
			NTL_DELETE( pGui );
			objit = m_mmapObjBalloon.erase( objit );
		}
		else
			++objit;
	}
}

VOID CBalloonManager::RemoveBalloonData( CNtlSob* pObj )
{
	if( !pObj )
		return;

	MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pObj );

	for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; )
	{
		MMAP_TYPE_IT_PAIR typeitpair = m_mmapTypeBalloon.equal_range( (*objit).second->GetType() );
		CBalloonGui* pBalloon = (*objit).second;

		// 
		if( pBalloon->GetType() == CBalloonGui::TYPE_QUEST )
		{
			if( pBalloon->GetOwnerObject()->GetClassID() == SLCLASS_NPC )
				reinterpret_cast<CNtlSobNpc*>( pBalloon->GetOwnerObject() )->ActiveQuestMark( TRUE );
			else
				reinterpret_cast<CNtlSobTriggerObject*>( pBalloon->GetOwnerObject() )->ActiveQuestMark( TRUE );					
		}	

		if( AlreadyExist( pBalloon->GetOwnerObject() ) == 1 )
			pBalloon->Show3DName( TRUE );
		//

		for( MMAP_TYPE_BALLOON_IT typeit = typeitpair.first ; typeit != typeitpair.second ; )
		{
			CBalloonGui* pTypeBalloon = (*typeit).second;

			if( pTypeBalloon == pBalloon )
				typeit = m_mmapTypeBalloon.erase( typeit );		
			else
				++typeit;
		}

		RemoveBalloonDataFromRenderQueue( pBalloon );
		NTL_DELETE( pBalloon );
		objit = m_mmapObjBalloon.erase( objit );		
	}

	//// peessi : 계속되는 무효포인터 에러를 잡기 위함.  
	//for( MMAP_TYPE_BALLOON_IT it = m_mmapTypeBalloon.begin() ; it != m_mmapTypeBalloon.end() ; ++it )
	//{
	//	CBalloonGui* pTypeBalloon = (*it).second;

	//	if( pTypeBalloon->GetOwnerObject()->GetSerialID() == pObj->GetSerialID() )
	//	{
	//		NTL_ASSERT( NULL, "CBalloonManager::RemoveBalloonData Not Deleted Balloon ID = " << pObj->GetSerialID() );						
	//	}
	//}
}

VOID CBalloonManager::RemoveBalloonDataFromRenderQueue( CBalloonGui* pBalloon )
{
	for( RwInt32 i = 0 ; i < m_nDisplayCount ; ++i )
	{
		if( m_aDiaplsyBalloon[i].pBalloon == pBalloon )
		{
			m_aDiaplsyBalloon[i].pBalloon = NULL;
			m_aDiaplsyBalloon[i].fDist = 0.0f;
		}
	}
}

RwBool CBalloonManager::IsEnableToCreate( RwInt32 eBalloonType )
{
	if( BALLOON_MAX_PRESENT > m_mmapTypeBalloon.count( eBalloonType ) )
		return TRUE;

	return FALSE;
}

VOID CBalloonManager::RemoveAllBalloonData(VOID)
{
	for( MMAP_OBJ_BALLOON_IT it = m_mmapObjBalloon.begin() ; it != m_mmapObjBalloon.end() ; ++it )
	{
		CBalloonGui* pBalloon = (*it).second;
		RemoveBalloonDataFromRenderQueue( pBalloon );
		NTL_DELETE( pBalloon );
	}

	m_mmapTypeBalloon.clear();
	m_mmapObjBalloon.clear();
}

RwBool CBalloonManager::HasNPCBalloon( CNtlSob* pSobObject )
{
	MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

	for( MMAP_OBJ_BALLOON_IT it = objitpair.first ; it != objitpair.second ; ++it )
	{
		if( (*it).second->GetType() == CBalloonGui::TYPE_NPC )
			return TRUE;
	}

	return FALSE;
}

RwBool CBalloonManager::HasQuestBalloon( CNtlSob* pSobObject )
{
	MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pSobObject );

	for( MMAP_OBJ_BALLOON_IT it = objitpair.first ; it != objitpair.second ; ++it )
	{
		if( (*it).second->GetType() == CBalloonGui::TYPE_QUEST )
			return TRUE;
	}

	return FALSE;
}

VOID CBalloonManager::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventQuestMark )
	{
		SNtlEventQuestMark* pMark = reinterpret_cast<SNtlEventQuestMark*>( msg.pData );
		if( pMark->hSerialId == INVALID_SERIAL_ID )
			return;

		CNtlSob* pObject = GetNtlSobManager()->GetSobObject( pMark->hSerialId );
		NTL_ASSERT( pObject, "CBalloonManager::HandleEvents : SobObject must not be NULL!" );	

		if( pMark->byMarkType == EQuestMarkBalloonNew )
		{
			sQUEST_INFO* pQuestInfo = reinterpret_cast<sQUEST_INFO*>( pMark->pQuestInfo );

			if( !pQuestInfo )
			{
				NTL_ASSERTFAIL( "CBalloonManager::HandleEvents : It is not balloon type" );
				return;
			}			

			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
			sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast< sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pQuestInfo->dwQuestStartDialog ) );
			if( !pQuestTextData && pQuestInfo->dwQuestStartDialog != INVALID_TBLIDX ) 
			{
				WCHAR buf[128];
				swprintf_s( buf, 128, L"%u QuestText is not present", pQuestInfo->dwQuestStartDialog );
				AddMarkBalloon( pObject, std::wstring( buf ) );
				return;
			}

			AddMarkBalloon( pObject, pQuestTextData->wstrText );
		}
		else if( pMark->byMarkType == EQuestMarkNone || pMark->byMarkType == EQuestMarkNew )
		{
			if( AlreadyExist( pObject ) )
			{
				CBalloonGui* pBalloon = NULL;
				MMAP_OBJ_IT_PAIR objitpair = m_mmapObjBalloon.equal_range( pObject );

				for( MMAP_OBJ_BALLOON_IT objit = objitpair.first ; objit != objitpair.second ; ++objit )
				{
					pBalloon = (*objit).second;
					RwInt32 ePriority = pBalloon->PriorityCheck( CBalloonGui::TYPE_NPC_MARK );

					if( ePriority == CBalloonGui::EQUAL_PRIORITY )
					{
						pBalloon->FadeoutMessage();
					}
				}			
			}		
		}		
	}
	else if( msg.Id == g_EventServerCharDialog )
	{
		SDboEventServerCharDialog* pDialog = reinterpret_cast<SDboEventServerCharDialog*>( msg.pData );
		if( pDialog->uiSerial == INVALID_SERIAL_ID )
			return;

		CNtlSob* pObject = GetNtlSobManager()->GetSobObject( pDialog->uiSerial );
		if( !pObject )
			return;

		if( pDialog->uiTextTblIdx == INVALID_SERIAL_ID )
		{
			if( pDialog->wTextLen <= 0 )
				return;

			AddNPCBalloon( pObject, pDialog->pTextData, BALLOON_NPC_DISPTIME, TRUE, TRUE, 3 );
		}
		else
		{
			if( pObject->GetClassID() == SLCLASS_NPC )
			{
				CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();

				AddNPCBalloon( pObject, pTextTable->GetText( pDialog->uiTextTblIdx ).c_str(), BALLOON_NPC_DISPTIME, TRUE, TRUE, 3 );
			}
			else if( pObject->GetClassID() == SLCLASS_MONSTER )
			{
				if( pDialog->bIsRefSpeechTable )
				{
					CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetNpcDialogTbl();
					CNpcSpeechTable* pSpeechTable = API_GetTableContainer()->GetNpcSpeechTable();
					sNPC_SPEECH_TBLDAT* pSpeechData = reinterpret_cast<sNPC_SPEECH_TBLDAT*>( pSpeechTable->FindData( pDialog->uiTextTblIdx ) );

					if( pSpeechData )
						AddNPCBalloon( pObject, pTextTable->GetText( pSpeechData->textIndex ).c_str(), pSpeechData->dwDisplayTime / 1000.0f, TRUE, TRUE, pSpeechData->byBallonType );
					else if( pDialog->uiTextTblIdx != INVALID_TBLIDX && Logic_IsUIDevInfoVisible() )
					{		
						WCHAR wBuf[256];
						swprintf_s( wBuf, 256, L"%u SpeechTableIndex Error", pDialog->uiTextTblIdx );
						AddNPCBalloon( pObject, wBuf, BALLOON_NPC_DISPTIME, TRUE, TRUE, 0 );
					}
				}
				else
				{
					CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();

					AddNPCBalloon( pObject, pTextTable->GetText( pDialog->uiTextTblIdx ).c_str(), BALLOON_NPC_DISPTIME, TRUE, TRUE, 3 );
				}
			}			
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( Logic_GetAvatarHandle() == pEvent->hSerialId && pEvent->uiUpdateType == EVENT_AIUT_DIE )
		{
			m_bAvatarDie = true;
		}
		else if( pEvent->uiUpdateType & EVENT_AIUT_ATTR )
		{
			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pEvent->hSerialId );
			GetBalloonManager()->SetBattleGaugeData( pSobObj, Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ) );
		}
	}
	//else if( msg.Id == g_EventCharObjDelete )
	else if( msg.Id == g_EventSobDelete )
	{
		//RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );
		SNtlEventSobDelete* pData  = reinterpret_cast<SNtlEventSobDelete*>( msg.pData );

		//if( *pDeleteSerial == INVALID_SERIAL_ID )
		if( pData->hSerialId == INVALID_SERIAL_ID )
			return;

		//NTL_WARNING( 0, "CBalloonManager::HandleEvent : CharObjDelete [ " << *pDeleteSerial << " ]" );
		//NTL_WARNING( 0, "CBalloonManager::HandleEvent : CharObjDelete [ " << *pDeleteSerial << " ]" );

		//RemoveBalloonData( GetNtlSobManager()->GetSobObject( *pDeleteSerial ) );
		RemoveBalloonData( GetNtlSobManager()->GetSobObject( pData->hSerialId ) );
	}
	// 포포스톤 부활 텍스트.
	else if( msg.Id == g_EventCharReady ) 
	{
		CNtlOtherParam* pOtherParam = GetNtlSLGlobal()->GetSobAvatar()->GetOtherParam();
		sBindInfo* pBindInfo = pOtherParam->GetBindInfo();

		if(Logic_GetActiveWorldId() != pBindInfo->BindedWorldID)
			return;

		if(pBindInfo->byBindType != REVIVAL_TYPE_BIND_POINT)
			return;		

		if( m_bAvatarDie == false )
			return;

		m_bAvatarDie = false;

		SERIAL_HANDLE hSerial = GetNtlSobManager()->GetObjectTriggerSerialFromId( pBindInfo->uiBindedObejcTblIdx );	
		CNtlSobTriggerObject* pSobObj = reinterpret_cast<CNtlSobTriggerObject*>( GetNtlSobManager()->GetSobObject( hSerial ) );

		if( pSobObj )
			AddNPCBalloon( pSobObj, GetDisplayStringManager()->GetString( "DST_POPOSTONE_RESURRECT_SAY" ), BALLOON_NPC_DISPTIME, FALSE, TRUE, 1 );
	}
	else if( msg.Id == g_EventTSPCDialog_Nfy )
	{
		SNtlEventTSPCDialog_Nfy* pData = reinterpret_cast<SNtlEventTSPCDialog_Nfy*>( msg.pData );

		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
		sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast< sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiMessage ) );

		if( !pQuestTextData && pData->uiMessage ) 
		{
			WCHAR buf[128];
			swprintf_s( buf, 128, L"%u QuestText is not present", pData->uiMessage );
			AddAvatarBalloon( buf );
			return;
		}

		AddAvatarBalloon( gui::GetHtmlFromMemoryString( pQuestTextData->wstrText.c_str(), pQuestTextData->wstrText.size() ).c_str() );
	}
	else if( msg.Id == g_EventSobProxyGUIUpdate )
	{
		SNtlEventSobProxyGUIUpdate* pData = reinterpret_cast<SNtlEventSobProxyGUIUpdate*>( msg.pData );

		if( pData->uiUpdateFlags & NTL_PROXYSYSTEM_LP_VISIBLE )
		{
			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pData->hSerialId );

			if( pSobObj )
			{
				if( pData->bShow )
					AddBattleGauge( pSobObj, Logic_GetName( pSobObj ), Logic_GetMaxLp( pSobObj ), Logic_GetLp( pSobObj ), TRUE );
				else
					FadeOutBalloon( pSobObj, CBalloonGui::TYPE_BATTLEGAUGE );
			}
		}		
	}
	else if( msg.Id == g_EventDiceResultShow )	
	{
		SDboEventDiceResultShow* pEvent = reinterpret_cast<SDboEventDiceResultShow*>( msg.pData );

		CNtlSob* pObject = GetNtlSobManager()->GetSobObject( pEvent->hHandle );
		if( pObject )
			AddDiceBalloon( pObject, pEvent->byResultNum );
	}
	else if( msg.Id == g_EventSobShowNfy )
	{
		SNtlEventSobShowNfy* pEvent = reinterpret_cast<SNtlEventSobShowNfy*>( msg.pData );

		if( pEvent->bShow )
		{
			if( pEvent->hSerialId != INVALID_SERIAL_ID )
			{
				CNtlSob* pSob = GetNtlSobManager()->GetSobObject( pEvent->hSerialId );
				if( pSob )
					VisibleBalloon( pSob, TRUE, CBalloonGui::TYPE_PRIVATESHOP );
			}			
		}		
		else
		{
			if( pEvent->hSerialId != INVALID_SERIAL_ID )
			{
				CNtlSob* pSob = GetNtlSobManager()->GetSobObject( pEvent->hSerialId );
				if( pSob )
					VisibleBalloon( pSob, FALSE, CBalloonGui::TYPE_PRIVATESHOP );
			}			
		}
	}	
}

VOID CBalloonManager::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < m_nDisplayCount ; ++i )
	{
		if( m_aDiaplsyBalloon[i].pBalloon )
			m_aDiaplsyBalloon[i].pBalloon->Paint();
	}
}


