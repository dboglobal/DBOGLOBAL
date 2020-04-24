#include "precomp_dboclient.h"
// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "InputActionMap.h"
#include "NtlSLEvent.h"

// dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"

// Option
#include "OptionWindowGui.h"
#include "OptionBase.h"
#include "OptionControlTab.h"

// logic
#include "DboLogic.h"


/////////////////////////////////////////////////////////////////////////////
// class : COptionControlCategoryNode

/**
* \brief COptionControlCategoryNode의 생성자
*
* 생성자에 표시될 텍스트와 Action의 Group을 만드는 ID를 지정한다.
*/
COptionControlCategoryNode::COptionControlCategoryNode(CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID)
: CGuiLineTreeNode( pMgr, nID )
, m_pStbTitle( NULL )
, m_pBtnExpand( NULL )
, m_pBtnReduce( NULL )
{
	// CategoryNode StaticBox의 Size와 Text, Color를 설정한다.
	CRectangle rect;
	rect.SetRectWH( dOPTIONCONTROL_CATEGORY_TITLE_X, dOPTIONCONTROL_CATEGORY_TITLE_Y, 
		dOPTIONCONTROL_CATEGORY_TITLE_WIDTH, dOPTIONCONTROL_CATEGORY_TITLE_HEIGHT );
	m_pStbTitle = NTL_NEW gui::CStaticBox( rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pStbTitle->SetText( strTitle.c_str() );
	m_pStbTitle->SetTextColor( dOPTIONCONTROL_CATEGORY_TITLE_COLOR );
	m_pStbTitle->Enable( false );

	// +버튼
	rect.SetRectWH(dOPTIONCONTROL_CATEGORY_BUTTON_X, dOPTIONCONTROL_CATEGORY_BUTTON_Y, dOPTIONCONTROL_CATEGORY_BUTTON_WIDTH, dOPTIONCONTROL_CATEGORY_BUTTON_HEIGHT);
	m_pBtnExpand = NTL_NEW gui::CButton( rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnExpand->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnUp"));
	m_pBtnExpand->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnFoc"));
	m_pBtnExpand->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnDown"));

	// -버튼
	m_pBtnReduce = NTL_NEW gui::CButton(rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnReduce->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnUp"));
	m_pBtnReduce->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnFoc"));
	m_pBtnReduce->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnDown"));

	// Button의 Signal 연결
	m_slotClickedBtnExpand = m_pBtnExpand->SigClicked().Connect(this, &COptionControlCategoryNode::OnClickBtnExpand);
	m_slotClickedBtnReduce = m_pBtnReduce->SigClicked().Connect(this, &COptionControlCategoryNode::OnClickBtnReduce);
}

/**
* \biref COptionControlCategoryNode의 소멸자
*/
COptionControlCategoryNode::~COptionControlCategoryNode()
{
	NTL_DELETE( m_pStbTitle );
	NTL_DELETE( m_pBtnReduce );
	NTL_DELETE( m_pBtnExpand );
}

/**
* \brief CategoryNode를 표시
*
* CGuiLineTreeNode의 ShowProc()을 오버라이드 하여 노드들이 확장됨에 따라 자식 노드들과의 관계를 재 계산하여
* 배치해주고 현재 버튼의 상태가 + 인지 - 인지 구분해서 출력해준다.
*/
void COptionControlCategoryNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();

	m_pStbTitle->Show(true);
	if (IsExpand())
	{
		m_pBtnReduce->Show(true);		
		m_pBtnExpand->Show(false);
		m_pStbTitle->SetTextColor( dOPTIONCONTROL_CATEGORY_TITLE_COLOR_DOWN, TRUE );
	}
	else
	{
		m_pBtnExpand->Show(true);
		m_pBtnReduce->Show(false);
		m_pStbTitle->SetTextColor( dOPTIONCONTROL_CATEGORY_TITLE_COLOR, TRUE );
	}

	// 카테고리 노드들의 위치를 재 계산
	m_pBtnExpand->SetPosition(m_nPosX + dOPTIONCONTROL_CATEGORY_BUTTON_X, m_nPosY + dOPTIONCONTROL_CATEGORY_BUTTON_Y);
	m_pBtnReduce->SetPosition(m_nPosX + dOPTIONCONTROL_CATEGORY_BUTTON_X, m_nPosY + dOPTIONCONTROL_CATEGORY_BUTTON_Y);
	m_pStbTitle->SetPosition(m_nPosX + dOPTIONCONTROL_CATEGORY_TITLE_X, m_nPosY + dOPTIONCONTROL_CATEGORY_TITLE_Y);
}

/**
* \brief CategoryNode를 숨김
*/
void COptionControlCategoryNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pStbTitle->Show(false);
	m_pBtnExpand->Show(false);
	m_pBtnReduce->Show(false);
}

/**
* \brief +버튼을 눌렀을 때의 행동
*/
void COptionControlCategoryNode::OnClickBtnExpand(gui::CComponent* pComponent)
{
	Expand(TRUE);
}

/**
* \brief -버튼을 눌렀을 때의 행동
*/
void COptionControlCategoryNode::OnClickBtnReduce(gui::CComponent* pComponent)
{
	Expand(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// class : COptionControlActionNode

/**
* \brief COptionControlActionNode의 생성자
*
* COptionControlCategoryNode의 자식들로 들어가게 될 Node, 행동의 str과 Action의 ID를 받아서 생성한다.
*/
COptionControlActionNode::COptionControlActionNode(CGuiLineTree* pMgr, std::wstring wstrTitle, RwUInt32 nAction, RwBool bFixed)
: CGuiLineTreeNode( pMgr, nAction )
, m_pStbTitle( NULL )
, m_pBtnSetKey( NULL )
{
	// 초기화값 저장
	m_nAction = nAction;
	m_bFixed = bFixed;

	// ActionNode StaticBox의 Size와 Text, Color를 설정한다.
	CRectangle rect;
	rect.SetRectWH( dOPTIONCONTROL_ACTIONNODE_TITLE_X, dOPTIONCONTROL_ACTIONNODE_TITLE_Y, 
		dOPTIONCONTROL_ACTIONNODE_TITLE_WIDTH, dOPTIONCONTROL_CATEGORY_TITLE_HEIGHT );

	m_pStbTitle = NTL_NEW gui::CStaticBox( rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pStbTitle->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfStaticBGLarge"));
	m_pStbTitle->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbTitle->SetTextColor( dOPTIONCONTROL_ACTIONNODE_TITLE_COLOR );
	m_pStbTitle->SetTextStyle(COMP_TEXT_CENTER);
	m_pStbTitle->SetText( wstrTitle.c_str() );
	
	// 키 지정 버튼(Action Id에 따른 표기를 해줘야 한다.)
	// 임시로 크기가 똑같은 리소스 사용
	rect.SetRectWH( dOPTIONCONTROL_ACTIONNODE_BUTTON_X, dOPTIONCONTROL_ACTIONNODE_BUTTON_Y,
		dOPTIONCONTROL_ACTIONNODE_BUTTON_WIDTH, dOPTIONCONTROL_ACTIONNODE_BUTTON_HEIGHT );
	m_pBtnSetKey = NTL_NEW gui::CButton(rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnSetKey->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfStaticBGSmall"));
	m_pBtnSetKey->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfButtonBGSmallFocus"));
	m_pBtnSetKey->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfButtonBGSmallDown"));
	m_pBtnSetKey->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfButtonBGSmallDisable"));
	m_pBtnSetKey->SetTextStyle(COMP_TEXT_CENTER);
	m_pBtnSetKey->SetTextFont( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );

	// 액션 ID에 따른 문자열을 출력해준다.
	std::wstring wstrKeyName = GetInputActionMap()->GetKeyName( nAction );

	// 비어 있다면 "지정안됨" 출력, 아니라면 키의 이름을 그대로 출력한다.
	if( wstrKeyName.empty() )
		m_pBtnSetKey->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_TEXT_EMPTYKEY" ) );
	else
		m_pBtnSetKey->SetText( wstrKeyName.c_str() );

	// Button의 Signal 연결
	m_slotClickedBtnSetKey = m_pBtnSetKey->SigClicked().Connect(this, &COptionControlActionNode::OnClickBtnSetKey);
}

/**
* \biref COptionControlActionNode의 소멸자
*/
COptionControlActionNode::~COptionControlActionNode()
{
	NTL_DELETE( m_pStbTitle );
	NTL_DELETE( m_pBtnSetKey );
}

/**
* \brief COptionControlActionNode를 표시
*/
void COptionControlActionNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();

	m_pStbTitle->Show(true);
	m_pBtnSetKey->Show(true);
	
	m_pBtnSetKey->SetPosition(m_nPosX + dOPTIONCONTROL_ACTIONNODE_BUTTON_X, m_nPosY + dOPTIONCONTROL_ACTIONNODE_BUTTON_Y);
	m_pStbTitle->SetPosition(m_nPosX + dOPTIONCONTROL_ACTIONNODE_TITLE_X, m_nPosY + dOPTIONCONTROL_ACTIONNODE_TITLE_Y);
}

/**
* \brief COptionControlActionNode를 숨김
*/
void COptionControlActionNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pStbTitle->Show(false);
	m_pBtnSetKey->Show(false);

	if( GetInputActionMap()->GetInputMode() == (RwUInt32)m_nID )
	{
		GetInputActionMap()->InitInputMode();
		m_pBtnSetKey->Enable( true );
		COptionControlList* pMgr = static_cast<COptionControlList *>(m_pMgr);

		pMgr->GetParentTab()->SetGuideDefault();
	}
}

/**
* \brief 키 지정 버튼을 눌렀을 때 발생되는 것
*/
void COptionControlActionNode::OnClickBtnSetKey(gui::CComponent* pComponent)
{
	COptionControlList* pMgr = static_cast<COptionControlList *>(m_pMgr);

	// InputActionMap 이 변경 불가능한 mdoe일 때
	if( GetInputActionMap()->GetActionMapMode() == ACTIONMAP_MODE_DEFAULT )
	{
		pMgr->GetParentTab()->SetGuideNegativeMode();
		return;
	}

	// 변경 불가면 변경불가라고 알려줘야함.
	if( m_bFixed )
	{
		pMgr->GetParentTab()->SetGuideFixed( m_nID );
	}
	else
	{
		pMgr->GetParentTab()->SetGuideText(dOPTIONCONTROL_STATICBOX_COLOR, L"DST_OPTION_CONTROL_TEXT_READYGUIDE" );
		pMgr->UpdateNode();
		GetInputActionMap()->SetInputMode( m_nAction );
	
		// 버튼을 비활성화한다.
		m_pBtnSetKey->Enable(false);
	}
}

void COptionControlActionNode::SetText( std::wstring& strTitle ) 
{
	m_pBtnSetKey->SetText( strTitle );
}

void COptionControlActionNode::Update() 
{
	m_pBtnSetKey->Enable( true );

	// 액션 ID에 따른 문자열을 출력해준다.
	std::wstring wstrKeyName = GetInputActionMap()->GetKeyName( m_nID );

	// 비어 있다면 "지정안됨" 출력, 아니라면 키의 이름을 그대로 출력한다.
	if( wstrKeyName.empty() )
		m_pBtnSetKey->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_TEXT_EMPTYKEY" ) );
	else
		m_pBtnSetKey->SetText( wstrKeyName.c_str() );
}


/////////////////////////////////////////////////////////////////////////////
// class : COptionControlCategoryNode

/**
* \brief COptionControlList의 생성자
*/
COptionControlList::COptionControlList()
{
}

/**
* \brief COptionControlList의 소멸자
*/
COptionControlList::~COptionControlList()
{
}

/**
* \brief COptionControlList에 CGuiLineTree를 만드는 함수
*
* LineTree를 만들고 스크롤바의 속성을 설정한다.
*
* \param rect (CRectangle&)
*/
RwBool COptionControlList::Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, COptionControl* pOptionControl)
{
	if (!CGuiLineTree::Create(rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth))
		return FALSE;

	m_pScroll->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollLayout" ) );
	m_pScroll->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollMedium" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnUp" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnDown" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnFoc" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnDown" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->SetButtonSize(dOPTIONCONTROL_LIST_SCROLL_WIDTH, dOPTIONCONTROL_LIST_SCROLL_HEIGHT);
	m_pScroll->SetSliderSize(dOPTIONCONTROL_LIST_SLIDER_WIDTH, dOPTIONCONTROL_LIST_SLIDER_HEIGHT);

	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &COptionControlList::OnCaptureWheelMove );

	m_pOptionControl = pOptionControl;

	return TRUE;
}

/**
* \brief OptionControl의 항목들을 생성
*/
void COptionControlList::CreateTree() 
{
	//////////////////////////////////////////////////////////////////////////////
	// 아바타 관련 카테고리
	CGuiLineTreeNode* pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_AVATAR" ) ,	// 카테고리 제목
		dOPTIONCONTROL_CATEGORY_AVATAR );												// 카테고리 ID
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );							// CGuiLineTree 루트를 부모로 가진다
	pNode->Expand( true );

	// 열거형들의 순서는 확실히 지켜줘야 한다. 순서대로 있지 않으면 꼬이게 된다.
	for(RwUInt32 nActionID = ACTION_AVATAR_FORWARD; nActionID <= ACTION_AVATAR_RIGHTSIDE; ++nActionID)
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),								// Action 제목
			nActionID );															// Action ID
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_AVATAR );							// 카테고리를 부모로 가진다.
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// 타겟 관련 카테고리
	pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_TARGET") , 
		dOPTIONCONTROL_CATEGORY_TARGET );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	for(RwUInt32 nActionID = ACTION_TARGET_SELF; nActionID <= ACTION_TARGET_4THPARTY; ++nActionID)
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_TARGET );
	}

	pNode = NTL_NEW COptionControlActionNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_TARGET_AUTOPARTY" ),
		ACTION_TARGET_AUTOTARGET_PARTY );
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_TARGET );

	for( RwUInt32 nActionID = ACTION_TARGET_AUTO; nActionID <= ACTION_TARGET_SELECTING_5; ++nActionID )
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_TARGET );
	}

	//////////////////////////////////////////////////////////////////////////////
	// 퀵슬롯 관련 카테고리
	pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_QUICKSLOT") , 
		dOPTIONCONTROL_CATEGORY_QUICKSLOT );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	for(RwUInt32 nActionID = ACTION_QUICK_1; nActionID <= ACTION_QUICK_PLUS; ++nActionID)
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_QUICKSLOT );
	}

	//////////////////////////////////////////////////////////////////////////
	// 확장
	// 80 ~ 159 까지 빈자리 예약
	for(RwUInt32 nActionID = ACTION_QUICK_1_EX; nActionID <= ACTION_QUICK_PLUS_EX2; ++nActionID )
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_QUICKSLOT );
	}

	// 퀵슬롯 조작 관련
	for(RwUInt32 nActionID = ACTION_QUICK_PREV; nActionID <= ACTION_QUICK_5THCAP; ++nActionID )
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum(nActionID),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_QUICKSLOT );
	}

	//////////////////////////////////////////////////////////////////////////////
	// 윈도우 관련 카테고리
	pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_WINDOW") , 
		dOPTIONCONTROL_CATEGORY_WINDOW );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	for(RwUInt32 nActionID = ACTION_WINDOW_PROFILE; nActionID <= ACTION_WINDOW_RANKBOARD; ++nActionID)
	{
		pNode = NTL_NEW COptionControlActionNode(this,
			GetActionDisplayStringNum( nActionID ),
			nActionID );
		CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_WINDOW );
	}

	//////////////////////////////////////////////////////////////////////////////
	// 고정 카테고리
	pNode = NTL_NEW COptionControlCategoryNode(this, 
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_CATEGORY_FIXED") , 
		dOPTIONCONTROL_CATEGORY_FIXED );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	// 미니맵 축소
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_MINIMAP_ZOOMOUT" ),
		ACTION_MINIMAP_ZOOMOUT , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// 미니맵 확대
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_MINIMAP_ZOOMIN" ),
		ACTION_MINIMAP_ZOOMIN , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// 채팅창 이전 페이지
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_CHAT_PGUP" ),
		ACTION_CHAT_PGUP , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// 채팅창 다음 페이지
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_CHAT_PGDN" ),
		ACTION_CHAT_PGDN , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// 취소
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_GLOBAL_CANCLE" ),
		ACTION_GLOBAL_CANCLE , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// 스크린샷
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_GLOBAL_SNAPSHOT" ),
		ACTION_GLOBAL_SNAPSHOT , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );

	// 채팅입력
	pNode = NTL_NEW COptionControlActionNode(this,
		GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_ACTION_GLOBAL_CHAT" ),
		ACTION_GLOBAL_CHAT , true);
	CGuiLineTree::AddNode( pNode, dOPTIONCONTROL_CATEGORY_FIXED );


//		ACTION_DEVUSER_ONLYRENDERGUI = 200,	///< 개발용모드 (GUI만 그린다. 임시)
}

/**
* \brief 각각의 노드를 업데이트 해준다.
*
* COptionControlActionNode들의 Update() 함수를 호출한다.
*/
void COptionControlList::UpdateNode() 
{
	for(int i=ACTION_AVATAR_FORWARD; i != ACTION_NUMS; ++i)
	{
		COptionControlActionNode* pActionNode = 
			static_cast<COptionControlActionNode*>(FindNode( i ));

		if( pActionNode )
			pActionNode->Update();
	}
	
}

void COptionControlList::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos ) 
{
	if( !GetParentGui()->IsVisible() )
		return;
	
	if( GetParentGui()->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	RwInt32 nValue = static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->GetValue();
	RwInt32 nMaxValue = static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->GetMaxValue();
	RwInt32 nDelta = nValue - sDelta / GUI_MOUSE_WHEEL_DELTA;

	if( nDelta < 0 )
		nDelta = 0;
	else if( nDelta > nMaxValue )
		nDelta = nMaxValue;

	static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar())->SetValue( nDelta );	
}

COptionControl* COptionControlList::GetParentTab() 
{
	if( m_pOptionControl ) 
		return m_pOptionControl; 
	else 
		return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
// class : COptionControl

COptionControl::COptionControl()
{
	m_pOptionControlList = NULL;	
	m_pDlgControlList = NULL;	
	m_pStbControlTitle = NULL;
	m_pStbControlBack = NULL;
	m_pStbControlText = NULL;	
}

COptionControl::~COptionControl()
{
}

RwBool COptionControl::Create(COptionWindowGui* pOptionWindow)
{
	NTL_FUNCTION("COptionControl::Create");
	COptionBase::Create(pOptionWindow);

	// 조작컨트롤 타이틀
	m_pStbControlTitle = (gui::CStaticBox*)GetComponent("stbControlTitle");
	m_pStbControlTitle->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_CONTROL_TEXT_TITLE" ) );

	// GUI LineTree를 사용하기 위한 가상 Dialog
	m_pDlgControlList = (gui::CDialog*)GetComponent("dlgControlList");

	// Gui Line Tree의 크기를 rect로 설정한다.
	CRectangle rect;
	rect.SetRectWH(dOPTIONCONTROL_LIST_X, dOPTIONCONTROL_LIST_Y, 
		dOPTIONCONTROL_LIST_WIDTH, dOPTIONCONTROL_LIST_HEIGHT);

	// GuiLineTree를 생성한다.
	m_pOptionControlList = NTL_NEW COptionControlList;
	if (!m_pOptionControlList->Create(rect, m_pDlgControlList, 
		dOPTIONCONTROL_LINE_HEIGHT,			// 각 라인의 높이
		dOPTIONCONTROL_LINE_MARGIN,			// 각 라인의 간격
		dOPTIONCONTROL_CHILD_MARGIN_WIDTH,	// 자식들의 간격
		dOPTIONCONTROL_LIST_SLIDER_WIDTH, this))	// 슬라이더의 넓이
		return FALSE;

	// 조작키를 지정할 수 있는 아이템들을 생성
	m_pOptionControlList->CreateTree();

	// TextBox를 생성
	m_pStbControlBack = (gui::CStaticBox*)GetComponent("stbControlBack");
	m_pStbControlText = (gui::CStaticBox*)GetComponent("stbControlText");

	NTL_RETURN(TRUE);
}

void COptionControl::Destroy()
{
	NTL_FUNCTION("COptionControl::Destroy");

	if (m_pOptionControlList)
	{
		m_pOptionControlList->Destroy();
		NTL_DELETE(m_pOptionControlList);
	}

	NTL_RETURNVOID();
}

void COptionControl::Show()
{
	// COptionBase의 ScrollBar를 사용하지 않는다.
	m_pScrollBar->Show(false);

	m_pStbControlTitle->Show(true);
	m_pDlgControlList->Show(true);
	m_pStbControlBack->Show(true);
	
	// 기본 안내 메시지 셋팅
	SetGuideDefault(); 
	m_pStbControlText->Show(true);

	COptionBase::Show();
}

void COptionControl::Hide()
{	
	// COptionBase의 ScrollBar 사용
	m_pScrollBar->Show(true);

	m_pDlgControlList->Show(false);
	m_pStbControlTitle->Show(false);
	m_pStbControlBack->Show(false);
	m_pStbControlText->Show(false);

	GetInputActionMap()->InitInputMode();	   ///< 액션맵 입력 모드의 해제
	//GetInputActionMap()->CancleActionMap();	   ///< 액션맵의 임시 저장된것을 로드
	
	// 노드의 업데이트
	m_pOptionControlList->UpdateNode();
		
	COptionBase::Hide();
}

void COptionControl::OnInit()
{
}

void COptionControl::OnReset()
{
	// 이전에 변경되었던 액션을 해제시켜준다.
	GetInputActionMap()->InitInputMode();
	GetInputActionMap()->InitDefaultActionMap();

	m_pOptionControlList->UpdateNode();
	SetGuideDefault();
}

void COptionControl::OnOk()
{
	GetInputActionMap()->InitInputMode();
	
	// 만약 변경점이 있고 적용이 되었다면 Node를 업데이트한다.
	if( GetInputActionMap()->ApplyActionMap() )
	{
		m_pOptionControlList->UpdateNode();
		SetGuideDefault();
	}
	else
		SetGuideText(dOPTIONCONTROL_STATICBOX_WARNING_COLOR, L"DST_OPTION_CONTROL_TEXT_WAITGUIDE" );
}

void COptionControl::OnCancel()
{
	GetInputActionMap()->CancleActionMap();
}

/**
* \breif OnHandleEvents
*/
void COptionControl::OnHandleEvents( RWS::CMsg &pMsg ) 
{
	// 액션맵에서 정보를 보낸 것을 처리한다.
	if( pMsg.Id == g_EventActionMapClientNotify )
	{
		SNtlEventActionMapClientNotify* pData = reinterpret_cast<SNtlEventActionMapClientNotify*>( pMsg.pData );

		m_pOptionControlList->UpdateNode();

		switch( pData->byType )
		{
		case SNtlEventActionMapClientNotify::ACTIONMAP_OK:							// 서버에 적용됐다고 알림
			SetGuideDefault();
			break;
		case SNtlEventActionMapClientNotify::ACTIONMAP_RELEASE:							// 키가 변경됐다고 알림
			SetGuideChange( pData->wParam1 );
			break;
		case SNtlEventActionMapClientNotify::ACTIONMAP_FIXEDKEY:						// 키가 고정된 키라고 알림
			SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, L"DST_OPTION_CONTROL_TEXT_FIXEDKEYGUIDE" );
			break;
		case SNtlEventActionMapClientNotify::ACTIONMAP_NOTCOMBINE:						// 조합키로는 지정못한다고 알림
			SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, L"DST_OPTION_CONTROL_TEXT_NOTCOMBINEGUIDE" );
			break;
		case SNtlEventActionMapClientNotify::ACTIONMAP_SAMEKEY:
			SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, L"DST_OPTION_CONTROL_TEXT_SAMEKEYGUIDE" );
			break;
		default:
			break;
		}
	}
}

void COptionControl::SetGuideText( RwUInt32 uiColor, const WCHAR* pString ) 
{
	m_pStbControlText->SetTextColor( uiColor );
	m_pStbControlText->SetText( pString );
}

/**
* \brief 기본적인 단축키 변경의 안내
*/
void COptionControl::SetGuideDefault() 
{
	WCHAR awcTemp[512];
	swprintf_s(awcTemp, 512, GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_GUIDE"),
		GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_EMPTYKEY"));
	
	SetGuideText( dOPTIONCONTROL_STATICBOX_COLOR, awcTemp );
}

/**
* \brief 키가 중복되어 지정안됨으로 해제된 키를 안내
*/
void COptionControl::SetGuideChange(RwUInt32 nAction) 
{
	WCHAR awcTemp[512];
	swprintf_s(awcTemp, 512, 
		GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_CHANGEGUIDE"),
		GetActionDisplayStringNum(nAction),
		GetActionDisplayStringNum(nAction),
		GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_EMPTYKEY"));

	SetGuideText( dOPTIONCONTROL_STATICBOX_WARNING_COLOR, awcTemp );
}

/**
* \brief 변경 불가능한 키라는 것을 안내
*/
void COptionControl::SetGuideFixed( RwUInt32 nAction ) 
{
	WCHAR awcTemp[512];
	swprintf_s( awcTemp, 512, GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_NOTCHANGEGUIDE"),
		GetActionDisplayStringNum( nAction ) );
	
	SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, awcTemp );
}

/**
* \brief 현재 모드에서는 변경이 불가능합니다 라는 것을 안내
*/
void COptionControl::SetGuideNegativeMode()
{
	SetGuideText( dOPTIONCONTROL_STATICBOX_GUIDE_COLOR, 
		GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_TEXT_NEGATIVEMODEGUIDE") );
}

