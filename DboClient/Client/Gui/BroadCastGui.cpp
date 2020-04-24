#include "precomp_dboclient.h"
#include "BroadCastGui.h"

// core
#include "NtlDebug.h"

// gui library
#include "gui_renderer.h"
#include "GuiUtil.h"

// Table
#include "TableContainer.h"
#include "NpcTable.h"
#include "MobTable.h"

// simulation layer
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"

// presentation
#include "NtlPLGuiManager.h"

// Dbo
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DialogPriority.h"


CBroadCastGui::CBroadCastGui( const RwChar* pName )
: CNtlPLGui( pName )
, m_pPnlBack(NULL)
, m_pStbName(NULL)
, m_pHtmlText(NULL)
, m_pBroadCastConfig(NULL)
{
	for(RwInt32 i=0; i<dBROAD_BALLOON_SHAPE_NUMS; ++i )
	{
		m_paFlaBalloon[i] = NULL;
	}

	m_byState = STATE_CLOSE;
	m_bCurUnit = FALSE;
	m_bPlay = FALSE;
	m_bSmallText = FALSE;

	m_fElapsed = 0.0f;
	m_fShakeTime = 0.0f;
}

CBroadCastGui::~CBroadCastGui()
{

}

RwBool CBroadCastGui::Create()
{
	NTL_FUNCTION( "CBroadCastGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\BroadCastGui.srf", "gui\\BroadCastGui.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis			= (gui::CDialog*)GetComponent( "dlgMain" );
	m_pPnlBack		= (gui::CPanel*)GetComponent( "backPanel" );
	m_pStbName		= (gui::CStaticBox*)GetComponent( "stbNameBox" );
	m_pHtmlText		= (gui::CHtmlBox*)GetComponent( "htmlText" );
	m_pHtmlText->SetLineSpace( 4 );
	m_paFlaBalloon[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL] = (gui::CFlash*)GetComponent( "flaBalloonNone" );
	m_paFlaBalloon[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_STAR] = (gui::CFlash*)GetComponent( "flaBalloonStar" );
	m_paFlaBalloon[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_THINK] = (gui::CFlash*)GetComponent( "flaBalloonThink" );
	m_paFlaBalloonSmall[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL] = (gui::CFlash*)GetComponent( "flaBalloonSmallNone" );
	m_paFlaBalloonSmall[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_STAR] = (gui::CFlash*)GetComponent( "flaBalloonSmallStar" );
	m_paFlaBalloonSmall[eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_THINK] = (gui::CFlash*)GetComponent( "flaBalloonSmallThink" );
	m_pPnlIllust	= (gui::CPanel*)GetComponent( "pnlIllust" );

	m_surIllust.SetRect( m_pThis->GetScreenRect() );

	// Signal
	m_slotPaint	= m_pPnlIllust->SigPaint().Connect( this, &CBroadCastGui::OnPaint );
	m_slotMove	= m_pThis->SigMove().Connect( this, &CBroadCastGui::OnMove );

	LinkMsg( g_EventBroadMsgBegNfy, 0 );
	LinkMsg( g_EventBroadMsgEndNfy, 0 );
	LinkMsg( g_EventWorldChange, 0 );

	// Function callback
	m_aCallUpdateState[STATE_DIRECTION_IN_START]	= &CBroadCastGui::UpdateDirectionInStart;
	m_aCallUpdateState[STATE_DIRECTION_IN]			= &CBroadCastGui::UpdateDirectionIn;
	m_aCallUpdateState[STATE_DIRECTION_IN_END]		= &CBroadCastGui::UpdateDirectionInEnd;
	m_aCallUpdateState[STATE_OPEN]					= &CBroadCastGui::UpdateOpen;
	m_aCallUpdateState[STATE_DIRECTION_OUT_START]	= &CBroadCastGui::UpdateDirectionOutStart;
	m_aCallUpdateState[STATE_DIRECTION_OUT]			= &CBroadCastGui::UpdateDirectionOut;
	m_aCallUpdateState[STATE_DIRECTION_OUT_END]		= &CBroadCastGui::UpdateDirectionOutEnd;
	m_aCallUpdateState[STATE_CLOSE]					= &CBroadCastGui::UpdateClose;

	// Init ( 화면에 안 보이게 설정 )
	m_pThis->SetPosition( GetDboGlobal()->GetScreenWidth(), 0 );

	GetNtlGuiManager()->AddUpdateFunc( this );

	m_pBroadCastConfig = GetDboGlobal()->GetDBOUIConfig()->GetBroadCastConfig();
	
	Show( false );

	NTL_RETURN(TRUE);
}

VOID CBroadCastGui::Destroy()
{
	NTL_FUNCTION( "CBroadCastGui::Destroy" );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventBroadMsgBegNfy );
	UnLinkMsg( g_EventBroadMsgEndNfy );
	UnLinkMsg( g_EventWorldChange );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 
}

VOID CBroadCastGui::Update( RwReal fElapsed )
{
	if( !m_bPlay )
		return;

	// Flash가 플레이 중이면 업데이트 해준다.
	for( RwInt32 i=0; i<dBROAD_BALLOON_SHAPE_NUMS; ++i )
	{
		if( m_paFlaBalloon[i]->IsPlayMovie() )
			m_paFlaBalloon[i]->Update( fElapsed );

		if( m_paFlaBalloonSmall[i]->IsPlayMovie() )
			m_paFlaBalloonSmall[i]->Update( fElapsed );
	}

	if( m_pHtmlText->IsVisible() )
		m_pHtmlText->Update( fElapsed );

	// 각 스테이트에 맞춰서 Update 함수 호출
	(this->*m_aCallUpdateState[m_byState])( fElapsed );
}

VOID CBroadCastGui::HandleEvents( RWS::CMsg &msg )
{
	// g_EventBroadMsgBegNfy : BroadCast Message를 시작하라는 Event
	if( msg.Id == g_EventBroadMsgBegNfy )
	{
		SNtlEventBroadMsgBegNfy* pNotify = reinterpret_cast< SNtlEventBroadMsgBegNfy* >( msg.pData );

		// 현재 출력되고 있는 Unit이 없다면 바로 시작한다.
		// 만약 현재 출력되고 있던 Unit이 있다면 현재 출력되고 있던 Type에 따라 새로운 Data로 세팅한다.
		if( !m_bCurUnit )
		{
			SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
			SetCurMsgData( m_CurUnit );

			Play( TRUE );

			SetState( STATE_DIRECTION_IN_START );

			return;
		}
		else
		{
			switch( m_CurUnit.m_byMsgType )
			{
			case eBROAD_MSG_TYPE_TS:
			case eBROAD_MSG_TYPE_EMERGENCY:
				{
					// 같은 타입이라고 지칭하고 현재 있는 것을 완전히 종료하고 새로운 데이타로 세팅한다.
					if( pNotify->byMsgType == eBROAD_MSG_TYPE_TS ||
						pNotify->byMsgType == eBROAD_MSG_TYPE_EMERGENCY )
					{
						SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
						SetCurMsgData( m_CurUnit );

						SetState( STATE_DIRECTION_IN_START );
						
						return;
					}
					else
					{
						DeleteDeqUnit( pNotify->byMsgType );

						// 현재 있는 Unit을 보관하고 새로운 Unit으로 세팅하여 플레이한다.
						SaveCurUnitData();
						SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
						SetCurMsgData( m_CurUnit );

						SetState( STATE_DIRECTION_IN_START );

						return;
					}
				}
				break;
			case eBROAD_MSG_TYPE_MINI_NARRATION:
				{
					if( pNotify->byMsgType != eBROAD_MSG_TYPE_MINI_NARRATION )
					{
						// TS와 Emergency가 있다면 삭제해준다.
						DeleteDeqUnit( eBROAD_MSG_TYPE_TS );
						DeleteDeqUnit( eBROAD_MSG_TYPE_EMERGENCY );
						
						// 현재 있는 Unit을 보관하고 새로운 Unit으로 세팅하여 플레이한다.
						SaveCurUnitData();

						// 새로운 Unit 세팅
						SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
						SetCurMsgData( m_CurUnit );

						SetState( STATE_DIRECTION_IN_START );

						return;
					}
					else
					{
						// 미니 나레이션일 경우 현재 있는 미니 나레이션은 무시하고 새로운 미니 나레이션을 세팅하여
						// 출력한다.
						SetCurUnitData( pNotify->byMsgType, pNotify->defMsgDataList );
						SetCurMsgData( m_CurUnit );

						SetState( STATE_DIRECTION_IN_START );

						return;
					}
				}
				break;
			}
		}
	}
	// g_EventBroadMsgEndNfy : 현재 출력되고 있는 메시지를 삭제하라는 Event
	else if( msg.Id == g_EventBroadMsgEndNfy )
	{
		SNtlEventBroadMsgEndNfy* pNotify = reinterpret_cast< SNtlEventBroadMsgEndNfy* >( msg.pData );

		// 현재 출력 중인 Unit가 삭제하라는 Unit이라면 종료 상태로 가고 예약되어 있는 Unit이라면
		// 예약된 Unit 들 중에 같은 Type들을 모두 다 삭제한다.
		if( m_bCurUnit )
		{
			if( pNotify->byMsgType == m_CurUnit.m_byMsgType )
			{
				m_bCurUnit = FALSE;
				SetState( STATE_DIRECTION_OUT_START );
			}
			else
			{
				DeleteDeqUnit( pNotify->byMsgType );
			}
		}
	}
	// g_EventWorldChange : World가 Change 되었다는 Event. 현재 출력 중인 것을 종료시키고 예약되어 있는 모든 유닛을 삭제한다.
	if( msg.Id == g_EventWorldChange )
	{
		SNtlEventWorldChange* pWorldChange = reinterpret_cast<SNtlEventWorldChange*>(msg.pData);

		if(pWorldChange->bWorldChange)      
		{            
			if( m_bCurUnit )
			{
				m_bCurUnit = FALSE;
				SetState( STATE_DIRECTION_OUT_START );

				DeleteDeqUnitAll();
			}
		}
	}
}

/**
* \brief 나타나는 연출 시작
*/
VOID CBroadCastGui::UpdateDirectionInStart( RwReal fElapsed )
{
	// Illust 를 세팅한다.
	SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );
	SetillustPos( m_pThis->GetScreenRect().left, m_pThis->GetScreenRect().top );
		
	// Dialog를 연다.
	GetDialogManager()->OpenDialog( DIALOG_BROADCAST );

	// HtmlBox에 내용을 세팅한다.
	SetHtmlString( m_sCurData.wstrSpeech.c_str(), m_sCurData.wstrSpeech.length() );
	m_pHtmlText->Show( false );
	
	// 모든 말풍선을 감춘다.
	HideAllBalloon();

	// BroadCast와 화면 상 X 위치와 Y 위치를 계산한다. (UIConfig.xml) 에서 읽어들임
	RwInt32 nXPos = (RwInt32)(m_pBroadCastConfig->fPosXRate * GetDboGlobal()->GetScreenWidth());
	RwInt32 nYPos = (RwInt32)(m_pBroadCastConfig->fPosYRate * GetDboGlobal()->GetScreenHeight());

	// ShowHide Type 따른 초기화를 해준다.
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
		{
			m_pThis->SetPosition( nXPos, nYPos );
			SetillustPos( nXPos, nYPos );
			
			m_pThis->SetAlpha( 255 );
			m_surIllust.SetAlpha( 255 );

			ShowBalloon( (RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );
			m_pHtmlText->Show( true );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			m_pThis->SetPosition( nXPos, nYPos );
			SetillustPos( nXPos, nYPos );
				
			m_pThis->SetAlpha( 0 );
			m_surIllust.SetAlpha( 0 );

			ShowBalloon((RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );
		
			m_conAlpha.SetAccel(0.0f, 255.0f, m_pBroadCastConfig->fBlendStartVel,
				m_pBroadCastConfig->fBlendAccelVel );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			m_pThis->SetPosition( GetDboGlobal()->GetScreenWidth(), nYPos );
			SetillustPos( GetDboGlobal()->GetScreenWidth(), nYPos );

			m_pThis->SetAlpha( 255 );
			m_surIllust.SetAlpha( 255 );
			
			m_conAccel.SetAccel( (RwReal)GetDboGlobal()->GetScreenWidth(), 
				(RwReal)m_pBroadCastConfig->fPosXRate * GetDboGlobal()->GetScreenWidth(), 
				(RwReal)m_pBroadCastConfig->fSlideStartVel, 
				(RwReal)m_pBroadCastConfig->fSlideAccelVel );

		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			m_pThis->SetPosition( (RwInt32)(nXPos)+(RwInt32)m_pBroadCastConfig->fFadeSlidePos, nYPos );
			SetillustPos( (RwInt32)(nXPos)+(RwInt32)m_pBroadCastConfig->fFadeSlidePos, nYPos );

			m_conAccel.SetAccel( (RwReal)(m_pBroadCastConfig->fPosXRate * GetDboGlobal()->GetScreenWidth())+m_pBroadCastConfig->fFadeSlidePos, 
				(RwReal)m_pBroadCastConfig->fPosXRate * GetDboGlobal()->GetScreenWidth(), 
				m_pBroadCastConfig->fFadeSlideStartVel, 
				m_pBroadCastConfig->fFadeSlideAccelVel );

			m_pThis->SetAlpha( 0 );
			m_surIllust.SetAlpha( 0 );
		}
		break;
	}

	SetState( STATE_DIRECTION_IN );
}

/**
* \brief 들어오는 연출
*/
VOID CBroadCastGui::UpdateDirectionIn( RwReal fElapsed )
{
	// Show Hide에 따라서 들어오는 연출 중에 해줘야하는 작업을 수행한다.
	RwInt32 nYPos = (RwInt32)(m_pBroadCastConfig->fPosYRate * GetDboGlobal()->GetScreenHeight());
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
		SetState( STATE_DIRECTION_IN_END );
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			RwBool bUpdate = m_conAlpha.Update( fElapsed );
			m_pThis->SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );
			m_surIllust.SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );

			if( !bUpdate )
				SetState( STATE_DIRECTION_IN_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			RwBool bUpdate = m_conAccel.Update(fElapsed);
			m_pThis->SetPosition((RwInt32)m_conAccel.GetCurrentValue(), nYPos);

			if( !bUpdate )
				SetState( STATE_DIRECTION_IN_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			RwBool bUpdate = m_conAccel.Update(fElapsed);
			m_pThis->SetPosition((RwInt32)m_conAccel.GetCurrentValue(), nYPos );
			
			m_pThis->SetAlpha( (RwUInt8)(m_conAccel.GetCurrentProgress() * 255.f ));
			m_surIllust.SetAlpha( (RwUInt8)(m_conAccel.GetCurrentProgress() * 255.f) );

			if( !bUpdate )
				SetState( STATE_DIRECTION_IN_END );
		}
		break;
	}	
}

/**
* \brief 들어오는 연출이 끝날 때
*/
VOID CBroadCastGui::UpdateDirectionInEnd( RwReal fElapsed )
{
	// Interval 상태 회복
	m_pHtmlText->SetIntervalTextDisable();

	// Show/Hide Direction
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			m_pThis->SetAlpha( 255 );
			m_surIllust.SetAlpha( 255 );
			m_pHtmlText->Show( true );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			ShowBalloon( (RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );
			m_pHtmlText->Show( true );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			m_pThis->SetAlpha( 255 );
			m_surIllust.SetAlpha( 255 );
			
			ShowBalloon((RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );
			m_pHtmlText->Show( true );
		}
		break;
	}

	// Text Direction
	switch( m_sCurData.eUISpeechDirType )
	{
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL:
		break;
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING:
		{
			m_conAlpha.SetAccel( 0.f, 255.f, m_pBroadCastConfig->fBlendTextStartVel,
				m_pBroadCastConfig->fBlendTextAccelVel );
			m_pHtmlText->SetAlpha( 0 );
		}
		break;
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE:
		{
			m_pHtmlText->SetIntervalTextEnable( m_pBroadCastConfig->fIntervalTextTime );
			m_pHtmlText->SetPresentNum( (RwInt32)m_pBroadCastConfig->fIntervalPresNum );
		}
		break;
	}

	switch( m_sCurData.eUIDirType )
	{
	case eBROAD_MSG_UI_DIR_TYPE_NORMAL:
		m_surIllust.GetRect( m_rectBackIllust );
		break;
	case eBROAD_MSG_UI_DIR_TYPE_WARNING:
		m_surIllust.GetRect( m_rectBackIllust );
		break;
	case eBROAD_MSG_UI_DIR_TYPE_DANGER:
		m_surIllust.GetRect( m_rectBackIllust );
		break;
	}

	m_fShakeTime = 0.0f;
	m_fElapsed = 0.0f;

	SetState( STATE_OPEN );
}

/**
* \brief 오픈 중의 연출
*/
VOID CBroadCastGui::UpdateOpen( RwReal fElapsed )
{
	// Text Direction ( Blending만 따로 연출 )
	switch( m_sCurData.eUISpeechDirType )
	{
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL:
		break;
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING:
		{
			RwBool bUpdate = m_conAlpha.Update( fElapsed );
			m_pHtmlText->SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );

			if( !bUpdate )
				m_pHtmlText->SetAlpha( 255 );
		}
		break;
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE:
		break;
	}

	// Shake Direction ( Shake는 공통 )
	switch( m_sCurData.eUIDirType )
	{
	case eBROAD_MSG_UI_DIR_TYPE_NORMAL:
		{
			m_fShakeTime += fElapsed;

			RwReal fOffsetX = RwSin( m_pBroadCastConfig->fShakeXFreq[eBROAD_MSG_UI_DIR_TYPE_NORMAL] * m_fShakeTime ) * m_pBroadCastConfig->fShakeXAmp[eBROAD_MSG_UI_DIR_TYPE_NORMAL];
			RwReal fOffsetY = RwSin( m_pBroadCastConfig->fShakeYFreq[eBROAD_MSG_UI_DIR_TYPE_NORMAL] * m_fShakeTime ) * m_pBroadCastConfig->fShakeYAmp[eBROAD_MSG_UI_DIR_TYPE_NORMAL];

			SetillustPos( m_rectBackIllust.left + (RwInt32)fOffsetX,
				m_rectBackIllust.top + (RwInt32)fOffsetY );
		}
		break;
	case eBROAD_MSG_UI_DIR_TYPE_WARNING:
		{
			m_fShakeTime += fElapsed;

			RwReal fOffsetX = RwSin( m_pBroadCastConfig->fShakeXFreq[eBROAD_MSG_UI_DIR_TYPE_WARNING] * m_fShakeTime ) * m_pBroadCastConfig->fShakeXAmp[eBROAD_MSG_UI_DIR_TYPE_WARNING];
			RwReal fOffsetY = RwSin( m_pBroadCastConfig->fShakeYFreq[eBROAD_MSG_UI_DIR_TYPE_WARNING] * m_fShakeTime ) * m_pBroadCastConfig->fShakeYAmp[eBROAD_MSG_UI_DIR_TYPE_WARNING];

			SetillustPos( m_rectBackIllust.left + (RwInt32)fOffsetX,
				m_rectBackIllust.top + (RwInt32)fOffsetY );
		}
		break;
	case eBROAD_MSG_UI_DIR_TYPE_DANGER:
		{
			m_fShakeTime += fElapsed;

			RwReal fOffsetX = RwSin( m_pBroadCastConfig->fShakeXFreq[eBROAD_MSG_UI_DIR_TYPE_DANGER] * m_fShakeTime ) * m_pBroadCastConfig->fShakeXAmp[eBROAD_MSG_UI_DIR_TYPE_DANGER];
			RwReal fOffsetY = RwSin( m_pBroadCastConfig->fShakeYFreq[eBROAD_MSG_UI_DIR_TYPE_DANGER] * m_fShakeTime ) * m_pBroadCastConfig->fShakeYAmp[eBROAD_MSG_UI_DIR_TYPE_DANGER];

			SetillustPos( m_rectBackIllust.left + (RwInt32)fOffsetX,
				m_rectBackIllust.top + (RwInt32)fOffsetY);
		}
		break;
	}

	m_fElapsed += fElapsed;
	
	// 현재 Unit의 타임이 끝나면 넘어간다.
	if( m_sCurData.fDisplayTime < m_fElapsed )
	{
		// 특별한 경우 : 만약 HtmlData가 다음 페이지가 있을 경우 다시 Open 상태로 돌아간다. ( Line에 대비하여 )
		if( m_pHtmlText->SetNextPage() )
		{
			m_fElapsed = 0.0f;
			SetState( STATE_OPEN );
			return;
		}

		vecdef_BroadMsgDataList::iterator it = m_CurUnit.m_vecMsgDataList.begin();
		m_CurUnit.m_vecMsgDataList.erase( it );
		SetState( STATE_DIRECTION_OUT_START );
	}
}

/**
* \brief 끝나는 연출의 시작
*/
VOID CBroadCastGui::UpdateDirectionOutStart( RwReal fElapsed )
{
	// Text Direction 회복 해줘야 할 것들
	switch( m_sCurData.eUISpeechDirType )
	{
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL:
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING:
	case eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE:
		{
			m_pHtmlText->SetIntervalTextDisable();
		}
		break;
	}

	// Shake Direction ( Shake는 공통 )
	switch( m_sCurData.eUIDirType )
	{
	case eBROAD_MSG_UI_DIR_TYPE_NORMAL:
		break;
	case eBROAD_MSG_UI_DIR_TYPE_WARNING:
	case eBROAD_MSG_UI_DIR_TYPE_DANGER:
		{
			SetillustPos( m_rectBackIllust.left, m_rectBackIllust.top );
		}
		break;
	}

	// 만약 현재 유닛의 메시지 리스트에 남아 있는 것이 있다면 일러스트, 텍스트, Balloon 등을 세팅해주고
	// 다시 STATE_OPEN 상태로 간다. ( 데이터가 유효한 경우만 )
	if( !m_CurUnit.m_vecMsgDataList.empty() && m_bCurUnit )
	{
		RwBool bChangeIllust = FALSE;
		sBROAD_MSG_DATA* pOldData = (sBROAD_MSG_DATA*)&(*m_CurUnit.m_vecMsgDataList.begin());
		if( m_sCurData.uiOwnerTblIdx != pOldData->uiOwnerTblIdx )
			bChangeIllust = TRUE;

		SetCurMsgData( m_CurUnit );

		if( bChangeIllust )
		{
			switch( m_CurUnit.m_byMsgType )
			{
			case eBROAD_MSG_TYPE_TS:
			case eBROAD_MSG_TYPE_MINI_NARRATION:
				{
					SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );
				}
				break;
			case eBROAD_MSG_TYPE_EMERGENCY:
				{
					SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );
				}
				break;
			}
		}

		// Text 세팅
		SetHtmlString( m_sCurData.wstrSpeech.c_str(), m_sCurData.wstrSpeech.length() );
		m_pHtmlText->Show( true );

		// Balloon 세팅
		HideAllBalloon();
		ShowBalloon((RwUInt8)m_sCurData.eUIBalloonShapeType, m_bSmallText );

		// Text Direction
		switch( m_sCurData.eUISpeechDirType )
		{
		case eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL:
			{
			}
			break;
		case eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING:
			{
				m_conAlpha.SetAccel( 0.f, 255.f, m_pBroadCastConfig->fBlendTextStartVel,
					m_pBroadCastConfig->fBlendTextAccelVel );
				m_pHtmlText->SetAlpha( 0 );
			}
			break;
		case eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE:
			{
				m_pHtmlText->SetIntervalTextEnable( m_pBroadCastConfig->fIntervalTextTime );
				m_pHtmlText->SetPresentNum( (RwInt32)m_pBroadCastConfig->fIntervalPresNum );
			}
			break;
		}

		m_fElapsed = 0.0f;

		SetState( STATE_OPEN );
		
		return;
	}
	
	// Show / Hide Direction
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			// 현재 상태에서 가감해야 한다.
			m_conAlpha.SetAccel( (RwReal)m_surIllust.GetAlpha(), (RwReal)0, m_pBroadCastConfig->fBlendStartVel,
				m_pBroadCastConfig->fBlendAccelVel );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			// 말풍선 닫기
			HideAllBalloon();
			m_pHtmlText->Show( false );
			// 가속도 컨트롤러를 사용하여 슬라이딩 팩터를 적용
			m_conAccel.SetAccel( (RwReal)m_pThis->GetScreenRect().left, 
				(RwReal)GetDboGlobal()->GetScreenWidth(), 
				m_pBroadCastConfig->fSlideStartVel, 
				m_pBroadCastConfig->fSlideAccelVel);
			SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );( m_pThis->GetScreenRect().left, m_pThis->GetScreenRect().top );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			// 말풍선 닫기
			HideAllBalloon();
			m_pHtmlText->Show( false );
			// 가속도 컨트롤러를 사용하여 슬라이딩 팩터를 적용
			m_conAccel.SetAccel( (RwReal)m_pThis->GetScreenRect().left, 
				(RwReal)(m_pBroadCastConfig->fPosXRate*GetDboGlobal()->GetScreenWidth())+m_pBroadCastConfig->fFadeSlidePos, 
				m_pBroadCastConfig->fFadeSlideStartVel, 
				m_pBroadCastConfig->fFadeSlideAccelVel);

			SetIllust( m_sCurData.uiOwnerTblIdx, m_sCurData.eOwnerCondition );( m_pThis->GetScreenRect().left, m_pThis->GetScreenRect().top );
		}
		break;
	}

	SetState( STATE_DIRECTION_OUT );
}

/**
* \brief 끝나는 연출 중
*/
VOID CBroadCastGui::UpdateDirectionOut( RwReal fElapsed )
{
	switch( m_sCurData.eUIShowHideType )
	{
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL:
		{
			SetState( STATE_DIRECTION_OUT_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE:
		{
			RwBool bUpdate = m_conAlpha.Update(fElapsed);
			m_pThis->SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );
			m_surIllust.SetAlpha( (RwUInt8)m_conAlpha.GetCurrentValue() );

			if( !bUpdate )
				SetState( STATE_DIRECTION_OUT_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE:
		{
			RwBool bUpdate = m_conAccel.Update(fElapsed);
			m_pThis->SetPosition((RwInt32)m_conAccel.GetCurrentValue(), (RwInt32)(GetDboGlobal()->GetScreenHeight() * m_pBroadCastConfig->fPosYRate));

			if( !bUpdate )
				SetState( STATE_DIRECTION_OUT_END );
		}
		break;
	case eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE:
		{
			RwBool bUpdate = m_conAccel.Update(fElapsed);
			m_pThis->SetPosition((RwInt32)m_conAccel.GetCurrentValue(), (RwInt32)(GetDboGlobal()->GetScreenHeight() * m_pBroadCastConfig->fPosYRate));
			m_pThis->SetAlpha( (RwUInt8)((1.0f-m_conAccel.GetCurrentProgress()) * 255.f ) );
			m_surIllust.SetAlpha( (RwUInt8)((1.0f-m_conAccel.GetCurrentProgress()) * 255.f ) );

			if( !bUpdate )
				SetState( STATE_DIRECTION_OUT_END );
		}
		break;
	}	
}

/**
* \brief 끝나는 연출 끝
*/
VOID CBroadCastGui::UpdateDirectionOutEnd( RwReal fElapsed )
{
	// 현재 Unit중 남은 것이 있다면 새로운 Unit을 꺼내온다.
	if( !m_deqUnit.empty() )
	{
		deqBroadUnit::iterator it = m_deqUnit.begin();
		SetCurUnitData( (*it).m_byMsgType, (*it).m_vecMsgDataList );
		SetCurMsgData( (*it) );
		m_deqUnit.pop_front();

		GetDialogManager()->CloseDialog( DIALOG_BROADCAST );
		SetState( STATE_DIRECTION_IN_START );
	}
	else
	{
		GetDialogManager()->CloseDialog( DIALOG_BROADCAST );
		SetState( STATE_CLOSE );
	}
}

/**
* \brief 끝나는 중
*/
VOID CBroadCastGui::UpdateClose( RwReal fElapsed )
{
	// 현재 데이타 무효함 & 플레이 멈춤
	m_bCurUnit = FALSE;
	m_pStbName->Clear();
	
	Play( FALSE );
}

RwInt32 CBroadCastGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	return TRUE;
}

VOID CBroadCastGui::SetState( RwUInt8 byState )
{
	// Debug
	/*WCHAR* szBroadCast[] = { L"STATE_DIRECTION_IN_START", L"STATE_DIRECTION_IN", L"STATE_DIRECTION_IN_END",
		L"STATE_OPEN", L"STATE_DIRECTION_OUT_START", L"STATE_DIRECTION_OUT", L"STATE_DIRECTION_OUT_END",
		L"STATE_CLOSE" };

	WCHAR awcBuffer[256];
	swprintf_s( awcBuffer, 256, L"BroadCast State Change : %s",  szBroadCast[byState] ); 
	CNtlSLEventGenerator::SysMsg( awcBuffer, SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_WARN );*/

	m_byState = byState;
}

/**
* \brief 현재 출력되는 Unit을 세팅한다.
*/
VOID CBroadCastGui::SetCurUnitData( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecList )
{
	m_CurUnit.SetData( byMsgType, vecList );
	m_bCurUnit = TRUE;

	// Dialog 우선 순위 적용
	
	RwUInt16 wPriority = dDIALOGPRIORITY_BROADCAST_TS;
	switch( byMsgType )
	{
	case eBROAD_MSG_TYPE_TS:
		wPriority = dDIALOGPRIORITY_BROADCAST_TS;
		break;
	case eBROAD_MSG_TYPE_EMERGENCY:
		wPriority = dDIALOGPRIORITY_BROADCAST_EMERGENCY;
		break;
	case eBROAD_MSG_TYPE_MINI_NARRATION:
		wPriority = dDIALOGPRIORITY_BROADCAST_MININARRATION;
		break;
	}
	
	m_pThis->SetPriority( wPriority );
}


/**
* \brief Unit의 가장 첫번째 Vector의 Data를 꺼내서 현재 Data에 저장한다.
*/
VOID CBroadCastGui::SetCurMsgData( CBroadCastUnit& unit )
{
	vecdef_BroadMsgDataList::iterator it = unit.m_vecMsgDataList.begin();

	sBROAD_MSG_DATA* pData = (sBROAD_MSG_DATA*)&(*it);

	m_sCurData.eUIDirType				= pData->eUIDirType;
	m_sCurData.eUIShowHideType			= pData->eUIShowHideType;
	m_sCurData.eUIBalloonShapeType		= pData->eUIBalloonShapeType;
	m_sCurData.eUISpeechDirType			= pData->eUISpeechDirType;
	m_sCurData.eOwnerCondition			= pData->eOwnerCondition;
	m_sCurData.eOwnerType				= pData->eOwnerType;
	m_sCurData.uiOwnerTblIdx			= pData->uiOwnerTblIdx;
	m_sCurData.wstrSpeech				= pData->wstrSpeech;
	m_sCurData.fDisplayTime				= pData->fDisplayTime;

	m_pStbName->SetText( L"?" );

	// 이름 지정
	if( m_sCurData.eOwnerType == eBROAD_MSG_OWNER_TYPE_NPC )
	{
		sNPC_TBLDAT* pNPCData = reinterpret_cast<sNPC_TBLDAT*>( API_GetTableContainer()->GetNpcTable()->FindData( m_sCurData.uiOwnerTblIdx ) );	
		if( pNPCData )
		{
			CTextTable* pNPCText = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
			if( pNPCText )
				m_pStbName->SetText( pNPCText->GetText( pNPCData->Name ).c_str() );
		}
	}
	else if( m_sCurData.eOwnerType == eBROAD_MSG_OWNER_TYPE_MOB )
	{
		sMOB_TBLDAT* pMobData = reinterpret_cast<sMOB_TBLDAT*>( API_GetTableContainer()->GetMobTable()->FindData( m_sCurData.uiOwnerTblIdx ) );
		if( pMobData )
		{
			CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();
			if( pMobText )
				m_pStbName->SetText( pMobText->GetText( pMobData->Name ).c_str() );
		}
	}
	else
	{
		DBO_FAIL( "CBroadCastGui::SetCurMsgData - Invalid Owner type" );
		return;
	}
}

/**
* \brief Html String을 입력한다. ( 크기 계산 )
*/
VOID CBroadCastGui::SetHtmlString( const WCHAR* pString, RwInt32 nSize )
{
	std::wstring wstrOriginText = gui::GetHtmlFromMemoryString( pString, nSize );

	// Large
	if( wstrOriginText.length() > dBROAD_HTML_TEXT_STANDARD )
	{
		m_pHtmlText->SetPosition( dBROAD_HTML_TEXT_LARGE_POSITION_X, dBROAD_HTML_TEXT_POSITION_Y );
		m_pHtmlText->SetSize( dBROAD_HTML_TEXT_LARGE_WIDTH, dBROAD_HTML_TEXT_HEIGHT );
		m_bSmallText = FALSE;
	}
	// Small
	else
	{
		m_pHtmlText->SetPosition( dBROAD_HTML_TEXT_SMALL_POSITION_X, dBROAD_HTML_TEXT_POSITION_Y );
		m_pHtmlText->SetSize( dBROAD_HTML_TEXT_SMALL_WIDTH, dBROAD_HTML_TEXT_HEIGHT );
		m_bSmallText = TRUE;
	}

	m_pHtmlText->SetHtmlFromMemory( pString , nSize);
}

/**
* \brief 현재 있는 Unit을 보관할 때는 Update된 타임을 저장해놓아야 한다.
*/
VOID CBroadCastGui::SaveCurUnitData()
{
	//// debugging
	//CNtlSLEventGenerator::SysMsg( L"SaveCurUnitData", SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_WARN );

	m_deqUnit.push_front( m_CurUnit );
}

/**
* \brief Deq에 보관되어 메시지를 삭제한다.
*/
VOID CBroadCastGui::DeleteDeqUnit( RwInt8 byMsgType )
{
	// 다를 경우라면 보관중인 Unit중에서 같은 타입이 있나 찾아보고 있다면 삭제를 해준다음
	deqBroadUnit::iterator it = m_deqUnit.begin();
	while( it != m_deqUnit.end() )
	{
		if( (*it).m_byMsgType == byMsgType )
		{
			it = m_deqUnit.erase( it );
			continue;
		}

		it++;
	}
}

/**
* \brief 저장되어 있던 모든 Unit을 삭제한다.
*/
VOID CBroadCastGui::DeleteDeqUnitAll()
{
	// 
	deqBroadUnit::iterator it = m_deqUnit.begin();
	while( it != m_deqUnit.end() )
	{
		it = m_deqUnit.erase( it );
	}
}

VOID CBroadCastGui::ShowBalloon( RwUInt8 byBalloonShape, RwBool bSmall /* = FALSE */, RwBool bShow /* = TRUE */)
{
	if( bSmall )
	{
		m_paFlaBalloonSmall[byBalloonShape]->Show(B2b(bShow) );
		m_paFlaBalloonSmall[byBalloonShape]->PlayMovie( bShow );
	}
	else
	{
		m_paFlaBalloon[byBalloonShape]->Show( B2b(bShow) );
		m_paFlaBalloon[byBalloonShape]->PlayMovie( bShow );
	}
}

VOID CBroadCastGui::HideAllBalloon()
{
	for( RwInt32 i =0; i< dBROAD_BALLOON_SHAPE_NUMS; ++i )
	{
		m_paFlaBalloon[i]->PlayMovie( FALSE );
		m_paFlaBalloon[i]->Show( false );

		m_paFlaBalloonSmall[i]->PlayMovie( FALSE );
		m_paFlaBalloonSmall[i]->Show( false );
	}
}

/**
* \brief Illust를 File name으로 세팅한다.
*/
VOID CBroadCastGui::SetIllust( const RwChar* pIllustName )
{
	std::string strFilename = "illust\\";
	strFilename += pIllustName;

	if( !m_surIllust.SetTexture( strFilename.c_str() ) )
	{
		char acBuf[256];
		sprintf_s( acBuf, 256, "CBroadCastGui::SetIllust - Invalid illust file name - %s", strFilename.c_str() );
		DBO_FAIL( acBuf );
		std::string strDef = "illust\\";
		strDef += dBROAD_BALLOON_BTS_DEFAULT_IMG;
		m_surIllust.SetTexture( strDef.c_str() );
	}

	gui::CTexture* pTex = m_surIllust.GetTexture();
	if( pTex )
		m_surIllust.SetSize( pTex->GetWidth(), pTex->GetHeight() );
}

/**
* \brief Illust를 Index와 condition으로 구성한다.
*/
VOID CBroadCastGui::SetIllust( RwUInt32 uiOwnerTblIdx, RwUInt32 uiOwnerCondition )
{
	// Illust Setting
	// File name Setting : (TBLIDX)_(OwnerCondition)_m.png
	char acBuffer[256];
	sprintf_s( acBuffer, "%u_%d_m.png", m_sCurData.uiOwnerTblIdx, (int)m_sCurData.eOwnerCondition );

	std::string strFilename = "illust\\";
	strFilename += acBuffer;

	if( !m_surIllust.SetTexture( strFilename.c_str() ) )
	{
		char acBuf[256];
		sprintf_s( acBuf, 256, "CBroadCastGui::SetIllust - Invalid illust file name - %s", strFilename.c_str() );
		DBO_FAIL( acBuf );
		std::string strDef = "illust\\";
		strDef += dBROAD_BALLOON_BTS_DEFAULT_IMG;
		m_surIllust.SetTexture( strDef.c_str() );
	}

	gui::CTexture* pTex = m_surIllust.GetTexture();
	if( pTex )
		m_surIllust.SetSize( pTex->GetWidth(), pTex->GetHeight() );
}

VOID CBroadCastGui::SetillustPos( RwInt32 nX, RwInt32 nY )
{
	// 좌표가 들어오는 기준은 m_pThis의 left, top으로 들어온다.
	CRectangle rtDialog = m_pThis->GetScreenRect();
	RwInt32 nDlgWidth = rtDialog.GetWidth();
	RwInt32 nDlgHeight = rtDialog.GetHeight();

	CRectangle rtIllust;
	m_surIllust.GetRect( rtIllust );
	RwInt32 nIllWidth = rtIllust.GetWidth();
	RwInt32 nIllHeight = rtIllust.GetHeight();
	
	// 현재 컴포넌트의 중간에 위치할 수 있도록.
	RwInt32 nXOffset = (nDlgWidth - nIllWidth)/2;
	RwInt32 nYOffset = (nDlgHeight - nIllHeight);
	
	m_surIllust.SetPosition( nX + nXOffset, nY + nYOffset);
}


VOID CBroadCastGui::Play( RwBool bPlay )
{
	m_bPlay = bPlay;
}

RwBool CBroadCastGui::IsPlay()
{
	return m_bPlay;
}

VOID CBroadCastGui::OnPaint()
{
	m_surIllust.Render();
}

VOID CBroadCastGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	m_surIllust.SetRect( m_pThis->GetScreenRect() );
}


