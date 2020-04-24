#include "precomp_dboclient.h"
#include "CautionSideIconGui.h"

// Core
#include "NtlDebug.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Dbo
#include "DboGlobal.h"
#include "DisplayStringManager.h"


///////////////////////////////////////////////////////////////////////////////////////////
// CCautionSideViewUnit
///////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CCautionSideViewUnit::CCautionSideViewUnit( gui::CComponent* pParentGui, std::string& uiStringID, const WCHAR* pwcString, RwReal fLimitTime)
{
	m_bLife = TRUE;
	m_uiStringID = uiStringID;
	m_fLimitTime = fLimitTime;
	m_fCurrentTime = 0.0f;
	
	CRectangle rect( 0, 0, dCAUTION_UNIT_WIDTH, dCAUTION_UNIT_HEIGHT );
	m_pStbSideViewUnit = NTL_NEW gui::CStaticBox( &rect, pParentGui, GetNtlGuiManager()->GetSurfaceManager());
	m_pStbSideViewUnit->SetTextColor( dCAUTION_UNIT_TEXT_COLOR );
	m_pStbSideViewUnit->SetText( pwcString );
}

/**
* \brief Destruction
*/
CCautionSideViewUnit::~CCautionSideViewUnit() 
{
	if( m_pStbSideViewUnit )
	{
		NTL_DELETE( m_pStbSideViewUnit );
		m_pStbSideViewUnit = NULL;
	}
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 업데이트에서 경과된 시간
*/
void CCautionSideViewUnit::Update( RwReal fElapsed ) 
{
	if( !m_bLife )
		return;

	m_fCurrentTime += fElapsed;
	
	if( m_fCurrentTime > m_fLimitTime )
		m_bLife = FALSE;
}

/**
* \brief 부모 컴포넌트에서의 위치를 정한다.
* \param nPosX		(RwInt32) X의 시작 좌표
* \param nPosY		(RwInt32) Y의 시작 좌표
*/
void CCautionSideViewUnit::SetPositionFromParent( RwInt32 nPosX, RwInt32 nPosY ) 
{
	if( m_pStbSideViewUnit )
		m_pStbSideViewUnit->SetPosition( nPosX, nPosY );
}

///////////////////////////////////////////////////////////////////////////////////////////
// CCautionSideIconGui
///////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CCautionSideIconGui::CCautionSideIconGui( const RwChar* pName )
: CSideIconBase( pName )
{
	m_nSize = 0;
	m_pIconButton = NULL;
}

/**
* \brief Destruction
*/
CCautionSideIconGui::~CCautionSideIconGui() 
{
}

/**
* \brief Caution SideIcon의 기본적인 컴포넌트를 생성하며 UI의 기능적 슬롯을 연결한다.
* \returns 성공 여부
*/
RwBool CCautionSideIconGui::Create() 
{
	NTL_FUNCTION( "CCautionSideIconGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\CautionSideIcon.srf", "gui\\CautionSideIcon.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pIconButton = (gui::CButton*)GetComponent( "btnIcon" );

	m_slotIconButtonClicked = m_pIconButton->SigClicked().Connect( this, &CCautionSideIconGui::OnIconButtonClicked );
	m_slotMove = m_pThis->SigMove().Connect( this, &CCautionSideIconGui::OnMove );

	// Priority
	SetNumber( dSIDEICON_PRIORITY_CAUTION );

	Show(false);

	NTL_RETURN(TRUE);
}

/**
* \brief Caution SideIcon의 기본적인 컴포넌트 해제 및 클래스에서 사용한 자원을 해제한다.
*/
void CCautionSideIconGui::Destroy() 
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

/**
* \brief 사이드아이콘에서 사이드뷰의 들어있는 유닛의 갯수를 받는다.
* \param pData	(void*) SCautionSideViewUnits*을 void* 으로 변환한 자료
*/
void CCautionSideIconGui::OnSideIconReceive( void* pData ) 
{
	SCautionSideViewUnits* pUnitSize = reinterpret_cast<SCautionSideViewUnits*>( pData );

	m_nSize = pUnitSize->nSize;

	// 사이즈가 0이거나 더 낮으면 닫는다.
	if( m_nSize <= 0 )
	{
		Show(false);
	}
	else
	{
		WCHAR awcBuffer[32];
		swprintf_s( awcBuffer, 32, L"%d", m_nSize );
		m_pIconButton->SetText( awcBuffer );

		Show(true);
	}
}

/**
* \brief 연결된 SideView가 닫 힐 때
*/
void CCautionSideIconGui::OnSideViewClosed() 
{
	/*Show(false);*/
}

/**
* \brief SideIcon을 클릭했을 때 실행되도록 등록한 함수
*/
void CCautionSideIconGui::OnIconButtonClicked( gui::CComponent* pComponent ) 
{
	CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_CAUTION, NULL );
}

/**
* \brief 기본 Dialog가 Move되거나 Resize 됐을 때 호출
* \param nOldX	(RwInt32) 이전 X 좌표
* \param nOldY	(RwInt32) 이전 Y 좌표
*/
void CCautionSideIconGui::OnMove( RwInt32 nOldX, RwInt32 nOldY ) 
{
	CRectangle rect = m_pIconButton->GetScreenRect();
	CSideIconGui::GetInstance()->OnSideViewSend( SIDEVIEW_CAUTION, (void*)&rect);
}

///////////////////////////////////////////////////////////////////////////////////////////
// CCautionSideViewGui
///////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CCautionSideViewGui::CCautionSideViewGui( const RwChar* pName ) 
: CSideViewBase( pName )
{

}

/**
* \brief Destruction
*/
CCautionSideViewGui::~CCautionSideViewGui() 
{

}

/**
* \brief CautionSideView의 컴포넌트와 리소스를 생성하고 필요한 Event를 등록한다.
* \return	성공여부
*/
RwBool CCautionSideViewGui::Create() 
{
	NTL_FUNCTION( "CCautionSideViewGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\CautionSideView.srf", "gui\\CautionSideView.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pStbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pBtnClose = (gui::CButton*)GetComponent( "btnClose" );

	m_winBackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_winBackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CautionSideView.srf", "srfDialogBackUp"));
	m_winBackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CautionSideView.srf", "srfDialogBackCenter"));
	m_winBackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CautionSideView.srf", "srfDialogBackDown"));
	m_winBackPanel.SetSize(250, 160);
	m_winBackPanel.SetPositionfromParent(0, 0);

	m_slotMove = m_pThis->SigMove().Connect(this, &CCautionSideViewGui::OnMove);
	m_slotPaint	= m_pThis->SigPaint().Connect(this, &CCautionSideViewGui::OnPaint);
	m_slotClickedClose = m_pBtnClose->SigClicked().Connect( this, &CCautionSideViewGui::OnCloseButtonClicked );

	// Set Text
	m_pStbTitle->SetText( GetDisplayStringManager()->GetString( "DST_CAUTION_SIDEICON_TITLE" ) );

	Show(false);

	LinkMsg( g_EventCautionSideNotify, 0 );

	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN( TRUE );
}

/**
* \brief CautionSideView에서 사용된 자원과 리소스를 해제한다.
*/
void CCautionSideViewGui::Destroy() 
{
	NTL_FUNCTION( "CCautionSideViewGui::Destroy" );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	if( !m_listUnit.empty() )
	{
		ListUnitsIt it = m_listUnit.begin();
		while( it != m_listUnit.end() )
		{
			DestroyUnit( (CCautionSideViewUnit*)(*it) );
			++it;
		}
		
		m_listUnit.clear();
	}

	UnLinkMsg( g_EventCautionSideNotify );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief Update
*/
void CCautionSideViewGui::Update( RwReal fElapsed ) 
{
	// List에 아무런 Unit도 없다면 Update하지 않는다.	
	if( m_listUnit.empty() )
	{
		/*CSideIconGui::GetInstance()->*/
		return;
	}
	
	// 갱신된 목록이 있는 검사한다.
	RwBool bIsDelete = FALSE;
	ListUnitsIt it = m_listUnit.begin();
	while( it != m_listUnit.end() )
	{
		(*it)->Update( fElapsed );

		if( !(*it)->IsLive() )
		{
			CCautionSideViewUnit* pUnit = (CCautionSideViewUnit*)(*it);
			m_listUnit.erase( it++ );
			DestroyUnit( pUnit );
			bIsDelete = TRUE;
		}

		if( !bIsDelete )
			it++;
	}

	// 사라진거싱 있다면 재 계산
	if( bIsDelete )
	{
		// 높이를 재계산하고 유닛을 재배치
		OnSideViewLocate( m_rectIcon );

		// SideIcon에 갯수를 알려준다.
		SendUnitSizeToIcon();
	}
}

/**
* \brief OnPaint
*/
void CCautionSideViewGui::OnPaint() 
{
	m_winBackPanel.Render();	
}

/**
* \brief OnMove
*/
void CCautionSideViewGui::OnMove( RwInt32 nOldX, RwInt32 nOldY ) 
{
	m_winBackPanel.SetRect( m_pThis->GetScreenRect() );
}

/**
* \brief 닫기 버튼을 클릭했을 때
*/
void CCautionSideViewGui::OnCloseButtonClicked( gui::CComponent* pComponent ) 
{
	OnSideViewClose();
}

/**
* \brief HandleEvents
*/
void CCautionSideViewGui::HandleEvents( RWS::CMsg& msg ) 
{
	if( msg.Id == g_EventCautionSideNotify )
	{
		SDboEventCautionSideNotify* pNotify = reinterpret_cast<SDboEventCautionSideNotify*>( msg.pData );

		// bActive 가 TRUE면 등록
		// FALSE면 삭제 ( 히스토리를 남기는 것이 목적이기 때문에 따로 삭제를 하진 않는다.
		if( pNotify->bActive )
		{
			// 생성을 하고 생성이 성공하면 윈도우 사이즈를 조절한다.
			if( CreateUnit( pNotify->strStringID, pNotify->pString, pNotify->fLifeTime ) )
			{
				SendUnitSizeToIcon();
				OnSideViewLocate( m_rectIcon );
			}
		}
		//else
		//{
		//	// StringID를 삭제해주고 만약 삭제된 것이 있다면 윈도우 사이즈를 조절한다.
		//	if ( DestroyUnit( pNotify->uiStringID ) )
		//	{
		//		SendUnitSizeToIcon();
		//		OnSideViewLocate( m_rectIcon );
		//	}
		//}
	}
}

/**
* \brief ESC 키를 눌렀을 경우
*/
void CCautionSideViewGui::OnPressESC() 
{
	OnSideViewClose();
}

/**
* \brief OnSideViewOpen
*/
void CCautionSideViewGui::OnSideViewOpen( const void* pData ) 
{
	// 보여지고 있다거나 List에 아무것도 없으면 끄고 아니라면 켠다.
	if( m_pThis->IsVisible() || m_listUnit.empty() )
		Show(false);
	else
		Show(true);
}

/**
* \brief OnSideViewReceive
* \param pData	(void*) Side Icon의 크기를 받기 위해서 CRectangle을 전달받는다.
*/
void CCautionSideViewGui::OnSideViewReceive( void* pData ) 
{
	CRectangle* pRect = reinterpret_cast<CRectangle*>(pData);

	m_rectIcon = (*pRect);
}

/**
* \brief OnSideViewClose
*/
void CCautionSideViewGui::OnSideViewClose() 
{
	Show(false);
}

/**
* \brief 유닛들의 높이로 현재의 View의 크기를 정하고 Unit들을 배치한다.
* \param rectSideIcon	(const CRectangle&) Caution SideIcon의 화면 위치 및 크기
*/
void CCautionSideViewGui::OnSideViewLocate( const CRectangle& rectSideIcon ) 
{
	RwInt32 nHeight = 0;

	ListUnitsIt it = m_listUnit.begin();
	RwInt32 nIndex = (RwInt32)m_listUnit.size();
	while( it != m_listUnit.end() )
	{
		nHeight += (*it)->GetHeight();

		(*it)->SetPositionFromParent( dCAUTION_UNIT_MARGIN_X_FROM_VIEW, (nIndex * dCAUTION_UNIT_HEIGHT) + dCAUTION_UNIT_HEIGHT_OFFSET_TOP );

		--nIndex;
		++it;
	}
	
	nHeight += dCAUTION_UNIT_HEIGHT_OFFSET_TOP + dCAUTION_UNIT_HEIGHT_OFFSET_BOTTOM;
	m_pThis->SetHeight( nHeight );
	m_pThis->SetPosition( rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - nHeight );
}

/**
* \brief Unit을 생성한다.
* \param uiStringID		(RwUInt32) 문자열 인덱스 (DisplayStringManager)
* \param pString		(const WCHAR*) 문자열
* \param fLifeTime		(RwReal) 경고 메시지가 살아 있는 시간
*/
RwBool CCautionSideViewGui::CreateUnit(std::string& uiStringID, const WCHAR* pString, RwReal fLifeTime )
{
	// 10개 이상일 경우 하나를 지워준다.
	if( (RwInt32)m_listUnit.size() >= dMAX_CAUTION_SIDE_VIEW_UNIT )
	{
		CCautionSideViewUnit* pUnit = m_listUnit.back();
		m_listUnit.pop_back();
		DestroyUnit( pUnit );
	}
	
	// 생성 후 넣기
	CCautionSideViewUnit* pUnit = NTL_NEW CCautionSideViewUnit( m_pThis, uiStringID, pString, fLifeTime );
	m_listUnit.push_front( pUnit );

	return TRUE;
}

/**
* \brief Unit을 삭제한다.
* \param pUnit	(CCautionSideViewUnit*) Unit을 메모리 해제한다.
*/
RwBool CCautionSideViewGui::DestroyUnit( CCautionSideViewUnit* pUnit ) 
{
	NTL_DELETE( pUnit );

	return TRUE;
}

/**
* \brief uiStringID를 가지고 있는 Unit을 모두 삭제한다.
* \param uiStringID		(RwUInt32) 문자열 인덱스
* \return 성공여부
*/
RwBool CCautionSideViewGui::DestroyUnit(std::string& uiStringID )
{
	if( (RwInt32)m_listUnit.size() <= 0 )
		return FALSE;

	RwBool bIsDelete = FALSE;
	for( ListUnitsIt it = m_listUnit.begin(); it != m_listUnit.end(); ++it)
	{
		if( (*it)->GetStringID() == uiStringID )
		{
			CCautionSideViewUnit* pUnit = (*it);
			it = m_listUnit.erase( it );
			DestroyUnit( pUnit );
			bIsDelete = TRUE;
		}
	}

	return bIsDelete;
}

/**
* \brief SideIcon에 Unit의 갯수를 보낸다.
*/
void CCautionSideViewGui::SendUnitSizeToIcon() 
{
	SCautionSideViewUnits sData;
	sData.nSize = (RwInt32)m_listUnit.size();

	CSideIconGui::GetInstance()->OnSideIconSend( SIDEICON_CAUTION, (void*)&sData ); 
}