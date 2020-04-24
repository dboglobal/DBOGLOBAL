#include "precomp_dboclient.h"
#include "BudokaiSideIconGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// Shared
#include "NtlBudokai.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "DialogManager.h"

// Sound
#include "GUISoundDefine.h"

// Simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "DisplayStringManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"

// Dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "ScriptLinkTable.h"
#include "DboScriptLinkDef.h"
#include "DboLogic.h"
#include "DialogManager.h"
 
/******************************************************************************
* Class			: CBudokaiSideViewUnit
* Desc			: 천하제일 무도회 사이드 뷰의 공지 유닛
******************************************************************************/
CBudokaiSideViewUnit::CBudokaiSideViewUnit( gui::CComponent *pParentGui, const WCHAR* pwcString, RwReal fLimitTime )
: m_pStbSideViewUnit(NULL)
{
	m_bLife = TRUE;
	m_fLimitTime = fLimitTime;
	m_fCurrentTime = 0.0f;

	CRectangle rect( 0, 0, dBUDOKAI_NOTICE_UNIT_WIDTH, dBUDOKAI_NOTICE_UNIT_HEIGHT );
	m_pStbSideViewUnit = NTL_NEW gui::CStaticBox( &rect, pParentGui, GetNtlGuiManager()->GetSurfaceManager() );
	m_pStbSideViewUnit->SetTextColor( dBUDOKAI_NOTICE_UNIT_TEXT_COLOR );
	m_pStbSideViewUnit->SetText( pwcString );
}

CBudokaiSideViewUnit::~CBudokaiSideViewUnit()
{
	NTL_DELETE( m_pStbSideViewUnit );
}

void CBudokaiSideViewUnit::Update( RwReal fElapsed )
{
	if( !m_bLife )
		return;

	m_fCurrentTime += fElapsed;
	if( m_fCurrentTime > m_fLimitTime )
		m_bLife = FALSE;
}

void CBudokaiSideViewUnit::SetPositionFromParent( RwInt32 nPosX, RwInt32 nPosY )
{
	if( m_pStbSideViewUnit )
		m_pStbSideViewUnit->SetPosition( nPosX, nPosY );
}

/******************************************************************************
* Class			: CBudokaiSideIconGui
* Desc			: 천하제일 무도회 사이드 아이콘의 클래스
******************************************************************************/

/**
* \brief Construction
*/
CBudokaiSideIconGui::CBudokaiSideIconGui( const RwChar* pName )
: CSideIconBase( pName )
, m_pBtnIcon( NULL )
{

}

/**
* \brief Destruction
*/
CBudokaiSideIconGui::~CBudokaiSideIconGui( VOID )
{
	
}

/**
* \brief 천하제일무도회 아이콘의 리소스를 생성하고 정보들을 초기화해준다.
* \return 성공여부
*/
RwBool CBudokaiSideIconGui::Create( VOID )
{
	NTL_FUNCTION( "CBudokaiSideIconGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiSideIcon.srf", "gui\\BudokaiSideIcon.frm" ) )
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pBtnIcon = (gui::CButton*)GetComponent( "btnIcon" );

	m_slotIconButtonClicked = m_pBtnIcon->SigClicked().Connect( this, &CBudokaiSideIconGui::OnIconButtonClicked );
	m_slotPaint = m_pBtnIcon->SigPaint().Connect( this, &CBudokaiSideIconGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CBudokaiSideIconGui::OnMove );

	// FlickEffect
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "BudokaiSideIcon.srf", "srfBudokaiPulse" );
	m_feEffect.SetSurface( surface );
	m_feEffect.SetTime( 0.0f, 0.5f );
	m_feEffect.SetAlpha( 0, 255 );

	Show( false );

	LinkMsg( g_EventBudokaiNoticeNfy, 0 );
	LinkMsg( g_EventBudokaiStateInfoNfy, 0 );
	LinkMsg( g_EventBudokaiUpdateStateNfy, 0 );
	LinkMsg( g_EventBudokaiUpdateMatchStateNfy, 0 );

	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN( TRUE );
}

/**
* \brief 천하제일무도회 리소스 및 관련된 데이터를 해제한다.
*/
VOID CBudokaiSideIconGui::Destroy( VOID )
{
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventBudokaiNoticeNfy );
	UnLinkMsg( g_EventBudokaiStateInfoNfy );
	UnLinkMsg( g_EventBudokaiUpdateStateNfy );
	UnLinkMsg( g_EventBudokaiUpdateMatchStateNfy );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

/**
* \brief Update
* \param fElapsedTime	(RwReal) 이전 업데이트에서 경과된 시간
*/
VOID CBudokaiSideIconGui::Update( RwReal fElapsedTime )
{
	m_feEffect.Update( fElapsedTime );
}

/**
* \brief SideView가 닫힐 때 호출된다.
*/
VOID CBudokaiSideIconGui::OnSideViewClosed( VOID )
{
	
}

/**
* \brief 링크된 이벤트를 핸들링한다.
*/
VOID CBudokaiSideIconGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventBudokaiNoticeNfy )
	{
		CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_BUDOKAI, msg.pData );

		// 소식지가 열려져 있지 않은 상태라면 깜빡 거린다.
		if( !GetDialogManager()->IsOpenDialog( DIALOG_BUDOKAI_NEWS ) )
		{
			m_feEffect.StartProc( TRUE );
		}
		
	}
	else if( msg.Id == g_EventBudokaiUpdateStateNfy )
	{
		sNtlEventBudokaiUpdateStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateStateNfy*>(msg.pData);

		if( pNotify->sStateInfo.byState == BUDOKAI_STATE_OPEN_NOTICE ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_JUNIOR_OPEN_NOTICE )
		{
			WCHAR awcBuffer[256];
			// 천하제일 무도회 소년부, 성인부 구분해서 출력
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_OPEN_NOTICE" ), Logic_GetTBTypeString( pNotify->sStateInfo.byState ) );

			CNtlSLEventGenerator::BudokaiNoticeNfy( BUDOKAI_NOTICE_ONLY_CLIENT, 0, 
				reinterpret_cast<VOID*>( awcBuffer ) );
		}
		else if( pNotify->sStateInfo.byState == BUDOKAI_STATE_DOJO_RECOMMEND ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_JUNIOR_DOJO_RECOMMEND )
		{
			if( Logic_I_am_GuildLeader() )
			{
				WCHAR awcBuffer[256];
				// 천하제일 무도회 소년부, 성인부 구분 메시지 출력 -by Kell
				swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_DOJO_RECOMMEND" ), Logic_GetTBTypeString( pNotify->sStateInfo.byState ) );
			
				CNtlSLEventGenerator::BudokaiNoticeNfy( BUDOKAI_NOTICE_ONLY_CLIENT, 0, 
					reinterpret_cast<VOID*>( awcBuffer ) ); 
			}
		}
	}
	else if( msg.Id == g_EventBudokaiUpdateMatchStateNfy )
	{
		sNtlEventBudokaiUpdateMatchStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateMatchStateNfy*>(msg.pData);

		WCHAR awcBuffer[256];
		const WCHAR* strMatchType = L"";
		std::wstring wstrNotify;
		if( pNotify->byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
		{
			strMatchType = GetDisplayStringManager()->GetString("DST_BUDOKAI_NOTICE_SOLO");
		}
		else if( pNotify->byMatchType == BUDOKAI_MATCH_TYPE_TEAM )
		{	
			strMatchType = GetDisplayStringManager()->GetString("DST_BUDOKAI_NOTICE_PARTY");
		}
		else
			return;

		RwUInt8 byMainState = BUDOKAI_STATE_CLOSE;
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( pSobAvatar )
		{
			byMainState = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiMainState();
		}
		else
		{
			byMainState = GetNtlSLGlobal()->GetTBudokaiStateInfo()->sStateInfo.byState;
		}

		switch( pNotify->sStateInfo.byState )
		{
		case BUDOKAI_MATCHSTATE_REGISTER:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_REGISTER" ), Logic_GetTBTypeString( byMainState ), strMatchType);
			break;
		case BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_WAIT_MINOR_MATCH" ), Logic_GetTBTypeString( byMainState ), strMatchType);
			break;
		case BUDOKAI_MATCHSTATE_MINOR_MATCH:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_MINOR_MATCH" ), Logic_GetTBTypeString( byMainState ), strMatchType);
			break;
		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_WAIT_MAJOR_MATCH" ), 
				Logic_GetTBTypeString( byMainState ),
				strMatchType,
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
			break;
		case BUDOKAI_MATCHSTATE_MAJORMATCH_32:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_MAJOR_MATCH" ),
				Logic_GetTBTypeString( byMainState ),
				strMatchType,
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_32" ) );
			break;
		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_WAIT_MAJOR_MATCH" ), 
				Logic_GetTBTypeString( byMainState ),
				strMatchType,
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			break;
		case BUDOKAI_MATCHSTATE_MAJORMATCH_16:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_MAJOR_MATCH" ),
				Logic_GetTBTypeString( byMainState ),
				strMatchType,
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_16" ) );
			break;
		case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_WAIT_MAJOR_MATCH" ), 
				Logic_GetTBTypeString( byMainState ),
				strMatchType,
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			break;
		case BUDOKAI_MATCHSTATE_MAJORMATCH_08:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_MAJOR_MATCH" ),
				Logic_GetTBTypeString( byMainState ),
				strMatchType,
				GetDisplayStringManager()->GetString( "DST_BUDOKAI_PROGRESS_MESSAGE_8" ) );
			break;
		case BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_WAIT_SEMIFINAL_MATCH" ), Logic_GetTBTypeString( byMainState ), strMatchType );
			break;
		case BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_SEMIFINAL_MATCH" ), Logic_GetTBTypeString( byMainState ), strMatchType );
			break;
		case BUDOKAI_MATCHSTATE_WAIT_FINAL_MATCH:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_WAIT_FINAL_MATCH" ), Logic_GetTBTypeString( byMainState ), strMatchType );
			break;
		case BUDOKAI_MATCHSTATE_FINAL_MATCH:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_FINAL_MATCH" ), Logic_GetTBTypeString( byMainState ), strMatchType );
			break;
		case BUDOKAI_MATCHSTATE_MATCH_END:
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_END" ), Logic_GetTBTypeString( byMainState ), strMatchType );
			break;
		}

		CNtlSLEventGenerator::BudokaiNoticeNfy( BUDOKAI_NOTICE_ONLY_CLIENT, 0, reinterpret_cast<VOID*>( awcBuffer ) );
	}
}

/**
* \brief 사이드 아이콘을 클릭하였을 때 나오는 행동
*/
VOID CBudokaiSideIconGui::OnIconButtonClicked( gui::CComponent* pComponent )
{
	if( m_feEffect.IsWork() )
		m_feEffect.EndProc();
	
	
	GetDialogManager()->SwitchDialog( DIALOG_BUDOKAI_NEWS );
}

/**
* \brief 사이드 아이콘이 Paint될때 호출된다.
*/
VOID CBudokaiSideIconGui::OnPaint( VOID )
{
	// FlickEffect 또한 Paint해준다.
	m_feEffect.Render();
}

/**
* \brief PLGui가 리사이즈되거나 이동 될때 호출된다.
*/
VOID CBudokaiSideIconGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rect = m_pThis->GetScreenRect();

	RwInt32 nCenterX, nCenterY;
	gui::CSurface surface = m_feEffect.GetSurface();
	surface.m_Original.rtRect.GetCenterPosition( &nCenterX, &nCenterY );
	m_feEffect.SetCenterPos( rect.left + nCenterX, rect.top + nCenterY );

	CSideIconGui::GetInstance()->OnSideViewSend( SIDEVIEW_BUDOKAI, (void*)&rect );
}

/******************************************************************************
* Class			: CBudokaiSideViewGui
* Desc			: 천하제일 무도회 사이드 뷰의 클래스
******************************************************************************/

/**
* \brief Construction
*/
CBudokaiSideViewGui::CBudokaiSideViewGui( const RwChar* pName )
: CSideViewBase( pName )
, m_pBtnClose(NULL)
, m_pStbTitle(NULL)
{

}

/**
* \brief Destruction
*/
CBudokaiSideViewGui::~CBudokaiSideViewGui( VOID )
{

}

/**
* \brief 천하제일 무도회 사이드뷰의 리소스 및 정보를 할당하고 초기화한다.
*/
RwBool CBudokaiSideViewGui::Create( VOID )
{
	NTL_FUNCTION( "CBudokaiSideViewGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BudokaiSideView.srf", "gui\\BudokaiSideView.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pBtnClose = (gui::CButton*)GetComponent( "btnClose" );
	m_pStbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pStbText = (gui::CStaticBox*)GetComponent("stbText");

	m_backPanel.SetType( CWindowby3::WT_HORIZONTAL );
	m_backPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BudokaiSideView.srf", "srfDialogBackUp"));
	m_backPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BudokaiSideView.srf", "srfDialogBackCenter"));
	m_backPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("BudokaiSideView.srf", "srfDialogBackDown"));
	m_backPanel.SetSize(281, 500);
	m_backPanel.SetPositionfromParent(0, 0);

	m_slotMove = m_pThis->SigMove().Connect( this, &CBudokaiSideViewGui::OnMove );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CBudokaiSideViewGui::OnPaint );
	m_slotClose = m_pBtnClose->SigClicked().Connect( this, &CBudokaiSideViewGui::OnClickClose );

	m_pStbTitle->SetText( GetDisplayStringManager()->GetString( "DST_BUDOKAI_SIDEICON_TITLE" ) );
	m_pBtnClose->SetPosition( m_pThis->GetWidth() - 19, 6 );
	
	Show( false );

	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN( TRUE );
}

/**
* \brief 천하제일 무도회 사이드뷰의 리소스 및 정보를 해제한다.
*/
VOID CBudokaiSideViewGui::Destroy( VOID )
{
	NTL_FUNCTION( "CBudokaiSideViewGui::Destroy" );

	GetNtlGuiManager()->RemoveUpdateFunc( this );



	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CBudokaiSideViewGui::Update( RwReal fElapsedTime )
{
	if( m_listUnit.empty() )
	{
		if( m_pThis->IsVisible() )
			OnSideViewClose();

		return;
	}

	RwBool bIsDelete = FALSE;
	ListUnitsIt it = m_listUnit.begin();
	while( it != m_listUnit.end() )
	{
		(*it)->Update( fElapsedTime );

		if( !(*it)->IsLive() )
		{
			CBudokaiSideViewUnit* pUnit = (CBudokaiSideViewUnit*)(*it);
			m_listUnit.erase( it++ );
			DestroyUnit( pUnit );
			bIsDelete = TRUE;
		}

		if( !bIsDelete )
			it++;
	}

	if( bIsDelete )
	{
		OnSideViewLocate( m_rectIcon );
	}
}

/**
* \brief ESC Key의 행동
*/
VOID CBudokaiSideViewGui::OnPressESC( VOID )
{

}

/**
* \brief CSideIconGui에서 현재의 Budokai SideView를 Open 하라고 명령을 내린다.
*/
VOID CBudokaiSideViewGui::OnSideViewOpen( const VOID* pData )
{
	SNtlEventBudokaiNoticeNfy* pNotify = (SNtlEventBudokaiNoticeNfy*)pData;

	if( pNotify->byNoticeType == BUDOKAI_NOTICE_SYSTEM )
	{
		CTextTable* pCSText = API_GetTableContainer()->GetTextAllTable()->GetCSTextTble();

		if( CreateUnit( pCSText->GetText( pNotify->tblidxNotice ).c_str(), dBUDOKAI_NOTICE_SYSTEM_TIME ) )
		{
			OnSideViewLocate( m_rectIcon );

			if( !m_pThis->IsVisible() )
				Show( true );
		}
	}
	else if( pNotify->byNoticeType == BUDOKAI_NOTICE_ONLY_CLIENT )
	{
		if( CreateUnit( reinterpret_cast<WCHAR*>(pNotify->pData), dBUDOKAI_NOTICE_SYSTEM_TIME ) )
		{
			OnSideViewLocate( m_rectIcon );

			if( !m_pThis->IsVisible() )
				Show( true );
		}
	}
}

/**
* \brief Open과 반대
*/
VOID CBudokaiSideViewGui::OnSideViewClose(VOID)
{
	Show( false );
}

/**
* \brief 현재의 SideView의 위치를 조절한다.
* \para rtSideIcon		(const CRectangle*) 사이드 아이콘의 Rect 영역
*/
VOID CBudokaiSideViewGui::OnSideViewLocate( const CRectangle& rtSideIcon )
{
	RwInt32 nHeight = 0;

	ListUnitsIt it = m_listUnit.begin();
	RwInt32 nIndex = (RwInt32)m_listUnit.size();
	while( it != m_listUnit.end() )
	{
		nHeight += (*it)->GetHeight();

		(*it)->SetPositionFromParent( dBUDOKAI_NOTICE_UNIT_MARGIN_X_FROM_VIEW, ( nIndex * dBUDOKAI_NOTICE_UNIT_HEIGHT )
			+ dBUDOKAI_NOTICE_UNIT_HEIGHT_OFFSET_TOP );

		--nIndex;
		++it;
	}

	nHeight += dBUDOKAI_NOTICE_UNIT_HEIGHT_OFFSET_TOP + dBUDOKAI_NOTICE_UNIT_HEIGHT_OFFSET_BOTTOM;
	m_pThis->SetHeight( nHeight );
	m_pThis->SetPosition( rtSideIcon.left - m_pThis->GetWidth() + rtSideIcon.GetWidth(), rtSideIcon.top - nHeight );
}

/**
* \brief Dialog가 그려질 때 같이 그려준다.
*/
VOID CBudokaiSideViewGui::OnPaint()
{
	m_backPanel.Render();
}

/**
* \brief Dialog가 이동될 때 같이 이동한다.
*/
VOID CBudokaiSideViewGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	m_backPanel.SetRect( m_pThis->GetScreenRect() );
}

/**
* \brief Close 버튼을 클릭하였을 때
* \param pComponent	(gui::CComponent*) Close 버튼
*/
VOID CBudokaiSideViewGui::OnClickClose( gui::CComponent* pComponent )
{
	OnSideViewClose();
}

/**
* \brief 테이블 인덱스에서 데이터를 가져와서 출력한다.
* \param tblidx	(RwUInt32) 테이블 인덱스
*/
VOID CBudokaiSideViewGui::ShowNotice( RwUInt32 tblidx )
{
	Show(true);
}

/**
* \brief 스트링을 직접 출력한다.
* \param pwcString		(const WCHAR*) 스트링 출력
*/
VOID CBudokaiSideViewGui::ShowNotice( const WCHAR* pwcString )
{
	m_pStbText->SetText( pwcString );

	Show( true );
}

RwBool CBudokaiSideViewGui::CreateUnit( const WCHAR* pString, RwReal fLifeTime )
{
	if( (RwInt32)m_listUnit.size() >= dBUDOKAI_NOTICE_UNIT_MAX )
	{
		CBudokaiSideViewUnit* pUnit = m_listUnit.back();
		m_listUnit.pop_back();
		DestroyUnit( pUnit );
	}
	else if( (RwInt32)m_listUnit.size() < 1 )
	{
		Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
	}


	CBudokaiSideViewUnit* pUnit = NTL_NEW CBudokaiSideViewUnit( m_pThis, pString, fLifeTime );
	m_listUnit.push_front( pUnit );

	return TRUE;
}

RwBool CBudokaiSideViewGui::DestroyUnit( CBudokaiSideViewUnit* pUnit )
{
	NTL_DELETE( pUnit );

	return TRUE;
}

VOID CBudokaiSideViewGui::OnSideViewReceive( void* pData )
{
	CRectangle* pRect = reinterpret_cast< CRectangle* > ( pData );

	m_rectIcon = ( *pRect );
}