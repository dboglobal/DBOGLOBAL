#include "precomp_dboclient.h"
// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// Simulation
#include "InputActionMap.h"
#include "NtlSLEvent.h"
#include "NtlStorageManager.h"
#include "NtlStorageMTContainer.h"

// dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"

// Option
#include "OptionWindowGui.h"
#include "OptionBase.h"
#include "OptionChattingTab.h"



/////////////////////////////////////////////////////////////////////////////
// class : COptionChattingCategoryNode

// Option Chatting List 의 카테고리의 +, - 버튼 이즈
#define dOPTIONCHATTING_CATEGORY_BUTTON_X		0
#define dOPTIONCHATTING_CATEGORY_BUTTON_Y		0
#define dOPTIONCHATTING_CATEGORY_BUTTON_WIDTH	140
#define dOPTIONCHATTING_CATEGORY_BUTTON_HEIGHT	20

// Option Chatting List 의 카테고리 속성
#define dOPTIONCHATTING_CATEGORY_TITLE_X 30
#define dOPTIONCHATTING_CATEGORY_TITLE_Y 0
#define dOPTIONCHATTING_CATEGORY_TITLE_WIDTH		100
#define dOPTIONCHATTING_CATEGORY_TITLE_HEIGHT	20
#define dOPTIONCHATTING_CATEGORY_TITLE_COLOR		RGB(255, 255, 255)
#define dOPTIONCHATTING_CATEGORY_TITLE_COLOR_FOCUS	RGB(255, 219, 71)
#define dOPTIONCHATTING_CATEGORY_TITLE_COLOR_DOWN	RGB(255, 219, 71)

/**
* \brief Contruction
* \param pMgr		(CGuiLineTree*) CGuiLineTree의 포인터
* \param strTitle	(std::wstring) 카테고리의 텍스트
* \param nID		(RwInt32) 현재 노드의 ID
*/
COptionChattingCategoryNode::COptionChattingCategoryNode( CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID )
: CGuiLineTreeNode( pMgr, nID )
, m_pBtnExpand( NULL )
, m_pBtnReduce( NULL )
{
	// +버튼
	CRectangle rect;
	rect.SetRectWH(dOPTIONCHATTING_CATEGORY_BUTTON_X, dOPTIONCHATTING_CATEGORY_BUTTON_Y, dOPTIONCHATTING_CATEGORY_BUTTON_WIDTH, dOPTIONCHATTING_CATEGORY_BUTTON_HEIGHT);
	m_pBtnExpand = NTL_NEW gui::CButton( rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnExpand->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnUp"));
	m_pBtnExpand->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnFoc"));
	m_pBtnExpand->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnDown"));
	m_pBtnExpand->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnExpand->SetTextCoord( 25, -2 );
	m_pBtnExpand->SetTextUpColor( dOPTIONCHATTING_CATEGORY_TITLE_COLOR );
	m_pBtnExpand->SetTextFocusColor( dOPTIONCHATTING_CATEGORY_TITLE_COLOR_FOCUS );
	m_pBtnExpand->SetTextDownColor( dOPTIONCHATTING_CATEGORY_TITLE_COLOR_DOWN );
	m_pBtnExpand->SetText( strTitle.c_str() );

	// -버튼
	m_pBtnReduce = NTL_NEW gui::CButton(rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnReduce->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnUp"));
	m_pBtnReduce->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnFoc"));
	m_pBtnReduce->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnDown"));
	m_pBtnReduce->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnReduce->SetTextCoord( 25, -2 );
	m_pBtnReduce->SetTextUpColor( dOPTIONCHATTING_CATEGORY_TITLE_COLOR );
	m_pBtnReduce->SetTextFocusColor( dOPTIONCHATTING_CATEGORY_TITLE_COLOR_FOCUS );
	m_pBtnReduce->SetTextDownColor( dOPTIONCHATTING_CATEGORY_TITLE_COLOR_DOWN );
	m_pBtnReduce->SetText( strTitle.c_str() );

	// Button의 Signal 연결
	m_slotClickedBtnExpand = m_pBtnExpand->SigClicked().Connect(this, &COptionChattingCategoryNode::OnClickBtnExpand);
	m_slotClickedBtnReduce = m_pBtnReduce->SigClicked().Connect(this, &COptionChattingCategoryNode::OnClickBtnReduce);
}

/**
* \brief Destruction
*/
COptionChattingCategoryNode::~COptionChattingCategoryNode()
{
	NTL_DELETE( m_pBtnReduce );
	NTL_DELETE( m_pBtnExpand );
}

/**
* \brief GuiLineTree의 ShowProc을 오버라이드
*/
void COptionChattingCategoryNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();

	if (IsExpand())
	{
		m_pBtnReduce->Show(true);		
		m_pBtnExpand->Show(false);
	}
	else
	{
		m_pBtnExpand->Show(true);
		m_pBtnReduce->Show(false);
	}

	// 카테고리 노드들의 위치를 재 계산
	m_pBtnExpand->SetPosition(m_nPosX + dOPTIONCHATTING_CATEGORY_BUTTON_X, m_nPosY + dOPTIONCHATTING_CATEGORY_BUTTON_Y);
	m_pBtnReduce->SetPosition(m_nPosX + dOPTIONCHATTING_CATEGORY_BUTTON_X, m_nPosY + dOPTIONCHATTING_CATEGORY_BUTTON_Y);
}

/**
* \brief GuiLineTree의 HideProc을 오버라이드
*/
void COptionChattingCategoryNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pBtnExpand->Show(false);
	m_pBtnReduce->Show(false);
}

/**
* \brief 확장 버튼을 클릭하였을 때
* \param pComponent (gui::CComponent*) singal을 보낸 Component
*/
void COptionChattingCategoryNode::OnClickBtnExpand( gui::CComponent* pComponent )
{
	Expand(TRUE);
}

/**
* \brief 감소 버튼을 클릭하였을 때
* \param pComponent (gui::CComponent*) singal을 보낸 Component
*/
void COptionChattingCategoryNode::OnClickBtnReduce( gui::CComponent* pComponent )
{
	Expand(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// class : CCheckBoxNode

// CheckBox의 체크박스 크기
#define dOPTIONCHATTING_CHECKNODE_CHECKBOX_X		5
#define dOPTIONCHATTING_CHECKNODE_CHECKBOX_Y		5
#define dOPTIONCHATTING_CHECKNODE_CHECKBOX_WIDTH	16
#define dOPTIONCHATTING_CHECKNODE_CHECKBOX_HEIGHT	16

// CheckBox의 타이틀 크기
#define dOPTIONCHATTING_CHECKNODE_TITLE_X			30
#define dOPTIONCHATTING_CHECKNODE_TITLE_Y			0
#define dOPTIONCHATTING_CHECKNODE_TITLE_WIDTH		200
#define dOPTIONCHATTING_CHECKNODE_TITLE_HEGIHT		20
#define dOPTIONCHATTING_CHECKNODE_TITLE_COLOR		RGB(255, 255, 255)

CCheckBoxNode::CCheckBoxNode( CGuiLineTree* pMgr, RwInt32 nID, std::wstring strTitle, std::wstring strToolTip )
: CGuiLineTreeNode( pMgr, nID )
, m_pBtnCheck(NULL)
, m_pStbTitle(NULL)
{
	// Check Box
	CRectangle rect;
	rect.SetRectWH( dOPTIONCHATTING_CHECKNODE_CHECKBOX_X, 
		dOPTIONCHATTING_CHECKNODE_CHECKBOX_Y,
		dOPTIONCHATTING_CHECKNODE_CHECKBOX_WIDTH,
		dOPTIONCHATTING_CHECKNODE_CHECKBOX_HEIGHT );
	m_pBtnCheck = NTL_NEW gui::CButton( rect, std::string(), pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pBtnCheck->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfSmallCheckBoxOff"));
	m_pBtnCheck->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfSmallCheckBoxFocus"));
	m_pBtnCheck->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srf", "srfSmallCheckBoxOn"));
	m_pBtnCheck->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("OptionWnd.srF", "srfSmallCheckBoxOn"));
	m_pBtnCheck->SetToggleMode(true);
	
	// Signals.
	m_slotToggledBtnCheck = m_pBtnCheck->SigToggled().Connect( this, &CCheckBoxNode::OnToggledCheckBox );
	

	// Title
	rect.SetRectWH( dOPTIONCHATTING_CHECKNODE_TITLE_X,
		dOPTIONCHATTING_CHECKNODE_TITLE_Y,
		dOPTIONCHATTING_CHECKNODE_TITLE_WIDTH,
		dOPTIONCHATTING_CHECKNODE_TITLE_HEGIHT );
	m_pStbTitle = NTL_NEW gui::CStaticBox( rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pStbTitle->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbTitle->SetTextStyle( COMP_TEXT_LEFT );
	m_pStbTitle->SetText( strTitle.c_str() );
	m_pStbTitle->SetToolTip( strToolTip.c_str() );
}

CCheckBoxNode::~CCheckBoxNode()
{
	NTL_DELETE( m_pStbTitle );
	NTL_DELETE( m_pBtnCheck );
}

void CCheckBoxNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();
	
	m_pBtnCheck->Show(true);
	m_pStbTitle->Show(true);

	m_pBtnCheck->SetPosition( m_nPosX + dOPTIONCHATTING_CHECKNODE_CHECKBOX_X,
		m_nPosY + dOPTIONCHATTING_CHECKNODE_CHECKBOX_Y );
	m_pStbTitle->SetPosition( m_nPosX + dOPTIONCHATTING_CHECKNODE_TITLE_X,
		m_nPosY + dOPTIONCHATTING_CHECKNODE_TITLE_Y );
}

void CCheckBoxNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pBtnCheck->Show(false);
	m_pStbTitle->Show(false);
}

void CCheckBoxNode::OnToggledCheckBox( gui::CComponent* pComponent, bool bDown )
{
	
}


/////////////////////////////////////////////////////////////////////////////
// class : SliderNode

#define dOPTIONCHATTING_SLIDERNODE_TITLE_X			30
#define dOPTIONCHATTING_SLIDERNODE_TITLE_Y			0
#define dOPTIONCHATTING_SLIDERNODE_TITLE_WIDTH		100
#define dOPTIONCHATTING_SLIDERNODE_TITLE_HEIGHT		20
#define dOPTIONCHATTING_SLIDERNODE_TITLE_COLOR		RGB(255, 255, 255)

#define dOPTIONCHATTING_SLIDERNODE_SLIDER_MIN			0
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_MAX			100
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_X				130
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_Y				0
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_WIDTH			125
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_HEIGHT		20
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_BUTTON_WIDTH	12
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_BUTTON_HEIGHT	20
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_SLIDER_WIDTH	10
#define dOPTIONCHATTING_SLIDERNODE_SLIDER_SLIDER_HEIGHT	20

CSliderNode::CSliderNode( CGuiLineTree* pMgr, RwInt32 nID, std::wstring strTitle, std::wstring strToolTip )
: CGuiLineTreeNode( pMgr, nID )
, m_pStbTitle(NULL)
, m_pScrSlider(NULL)
{
	// Title
	CRectangle rect;
	rect.SetRectWH( dOPTIONCHATTING_SLIDERNODE_TITLE_X, 
		dOPTIONCHATTING_SLIDERNODE_TITLE_Y,
		dOPTIONCHATTING_SLIDERNODE_TITLE_WIDTH,
		dOPTIONCHATTING_SLIDERNODE_TITLE_HEIGHT );
	m_pStbTitle = NTL_NEW gui::CStaticBox( &rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pStbTitle->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pStbTitle->SetTextStyle( COMP_TEXT_LEFT );
	m_pStbTitle->SetTextColor( dOPTIONCHATTING_SLIDERNODE_TITLE_COLOR );
	m_pStbTitle->SetText( strTitle.c_str() );
	m_pStbTitle->SetToolTip( strToolTip.c_str() );

	// Slider
	rect.SetRectWH( dOPTIONCHATTING_SLIDERNODE_SLIDER_X,
		dOPTIONCHATTING_SLIDERNODE_SLIDER_Y,
		dOPTIONCHATTING_SLIDERNODE_SLIDER_WIDTH,
		dOPTIONCHATTING_SLIDERNODE_SLIDER_HEIGHT );
	m_pScrSlider = NTL_NEW gui::CScrollBar( &rect, TRUE, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pScrSlider->SetRange( dOPTIONCHATTING_SLIDERNODE_SLIDER_MIN,
		dOPTIONCHATTING_SLIDERNODE_SLIDER_MAX );
	m_pScrSlider->SetValue(0);
	m_pScrSlider->SetVertical( true );
	m_pScrSlider->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHLayout" ) );
	m_pScrSlider->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHSlider" ) );
	m_pScrSlider->GetDecreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHPrev" ) );
	m_pScrSlider->GetDecreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHPrev" ) );
	m_pScrSlider->GetDecreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHPrev" ) );
	m_pScrSlider->GetDecreaseButton()->AddSurfaceDisabled( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHPrev" ) );
	m_pScrSlider->GetIncreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHNext" ) );
	m_pScrSlider->GetIncreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHNext" ) );
	m_pScrSlider->GetIncreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHNext" ) );
	m_pScrSlider->GetIncreaseButton()->AddSurfaceDisabled( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfOptionScrollHNext" ));
	m_pScrSlider->SetSliderSize( dOPTIONCHATTING_SLIDERNODE_SLIDER_SLIDER_WIDTH, dOPTIONCHATTING_SLIDERNODE_SLIDER_SLIDER_HEIGHT );
	m_pScrSlider->SetButtonSize( dOPTIONCHATTING_SLIDERNODE_SLIDER_BUTTON_WIDTH, dOPTIONCHATTING_SLIDERNODE_SLIDER_BUTTON_HEIGHT );
	m_slotChangedScrSlider = m_pScrSlider->SigSliderMoved().Connect( this, &CSliderNode::OnChangeSlider );
	m_slotValueChanged = m_pScrSlider->SigValueChanged().Connect( this, &CSliderNode::OnChangeSlider );
}

CSliderNode::~CSliderNode()
{
	NTL_DELETE( m_pScrSlider );
	NTL_DELETE( m_pStbTitle );
}

void CSliderNode::OnChangeSlider( RwInt32 nValue )
{

}

void CSliderNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();

	m_pStbTitle->Show(true);
	m_pScrSlider->Show(true);

	m_pStbTitle->SetPosition( m_nPosX + dOPTIONCHATTING_SLIDERNODE_TITLE_X,
		m_nPosY + dOPTIONCHATTING_SLIDERNODE_TITLE_Y );
	m_pScrSlider->SetPosition( m_nPosX + dOPTIONCHATTING_SLIDERNODE_SLIDER_X,
		m_nPosY + dOPTIONCHATTING_SLIDERNODE_SLIDER_Y );
}

void CSliderNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pStbTitle->Show(false);
	m_pScrSlider->Show(false);

}
/////////////////////////////////////////////////////////////////////////////
// class : SliderNode

/**
* \brief Construction
*/
COptionChattingList::COptionChattingList()
{
	
}

/**
* \brief Destruction
*/
COptionChattingList::~COptionChattingList()
{

}

/**
* \brief 상속받은 CGuiLineTree의 Create를 호출하고 스크롤 바 세팅, 생성에 필요한 작업들을 한다.
*/
RwBool COptionChattingList::Create( CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, COptionChatting* pOptionChatting /*= NULL*/ )
{
	if( !CGuiLineTree::Create( rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth ) )
		return FALSE;

	m_pScroll->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollLayout" ) );
	m_pScroll->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollMedium" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnUp" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnDown" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnFoc" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnDown" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->SetButtonSize(dOPTIONCHATTING_LIST_SCROLL_WIDTH, dOPTIONCHATTING_LIST_SCROLL_HEIGHT);
	m_pScroll->SetSliderSize(dOPTIONCHATTING_LIST_SLIDER_WIDTH, dOPTIONCHATTING_LIST_SLIDER_HEIGHT);

	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &COptionChattingList::OnCaptureWheelMove );

	m_pOptionChatting = pOptionChatting;

	return TRUE;
}

void COptionChattingList::CreateTree()
{
#define OCLGDS(stringid) GetDisplayStringManager()->GetString(stringid)

	// 공통 옵션의 카테고리
	CGuiLineTreeNode* pNode = NTL_NEW COptionChattingCategoryNode( this,
		OCLGDS("DST_OPTION_CHATTING_TITLE_COMMON"),
		dOPTIONCHATTING_CATEGORY_COMMON );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	// 공통 옵션의 세부 옵션들
	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_INPUTMODE, OCLGDS("DST_OPTION_CHATTING_COMMON_INPUTMODE")
		, OCLGDS("DST_OPTION_TOOLTIP_CHAT_INPUTMODE"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_COMMON );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_WINDOWLOCK, OCLGDS("DST_OPTION_CHATTING_COMMON_LOCK")
		, OCLGDS("DST_OPTION_TOOLTIP_CHAT_MOUSESCROLL" ) );
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_COMMON );

	/*pNode = NTL_NEW CCheckBoxNode( this, eCHAT_WINDOWMSCROLL, OCLGDS(DST_OPTION_CHATTING_COMMON_SCROLL)
		, OCLGDS(DST_OPTION_TOOLTIP_CHAT_MOUSESCROLL));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_COMMON );*/

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_ADDEDWINDOW, OCLGDS("DST_OPTION_CHATTING_COMMON_ADDEDWINDOW")
		, OCLGDS("DST_OPTION_TOOLTIP_CHAT_ADDED_WINDOW"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_COMMON );

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDEDWINDOW2, OCLGDS("DST_OPTION_CHATTING_COMMON_ADDEDWINDOW2")
		, OCLGDS("DST_OPTION_TOOLTIP_CHAT_ADDED2_WINDOW"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_COMMON);

	pNode = NTL_NEW CSliderNode( this, eCHAT_WINDOWOPACITY, OCLGDS("DST_OPTION_CHATTING_COMMON_OPACITY")
		, OCLGDS("DST_OPTION_TOOLTIP_CHAT_WINDOW_OPACITY"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_COMMON );

	// Categories of default chat window options
	pNode = NTL_NEW COptionChattingCategoryNode( this,
		OCLGDS("DST_OPTION_CHATTING_TITLE_BASIC"),
		dOPTIONCHATTING_CATEGORY_BASIC );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	// 기본 채팅창 옵션의 세부 옵션들
	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_BASIC_NORMAL, OCLGDS("DST_OPTION_CHATTING_BASIC_NORMAL"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_NORMAL"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_BASIC );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_BASIC_WISPHER, OCLGDS("DST_OPTION_CHATTING_BASIC_WISPHER"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_WHISPER"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_BASIC );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_BASIC_PARTY, OCLGDS("DST_OPTION_CHATTING_BASIC_PARTY"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_PARTY"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_BASIC );

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_BASIC_FIND_PARTY, OCLGDS("DST_OPTION_CHATTING_BASIC_PARTY_SEARCH"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_PARTY_SEARCH"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_BASIC);

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_BASIC_GUILD, OCLGDS("DST_OPTION_CHATTING_BASIC_GUILD"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_GUILD"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_BASIC );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_BASIC_TRADE, OCLGDS("DST_OPTION_CHATTING_BASIC_TRADE"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_TRADE"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_BASIC );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_BASIC_SHOUT, OCLGDS("DST_OPTION_CHATTING_BASIC_SHOUT"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_SHOUT"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_BASIC );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_BASIC_SYSTEM, OCLGDS("DST_OPTION_CHATTING_BASIC_SYSTEM"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_SYSTEM"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_BASIC );

	// Categories of additional chat window options
	pNode = NTL_NEW COptionChattingCategoryNode( this,
		OCLGDS("DST_OPTION_CHATTING_TITLE_ADDED"),
		dOPTIONCHATTING_CATEGORY_ADDED );
	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
	pNode->Expand( true );

	// 추가 채팅창 옵션의 세부 옵션들
	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_ADDED_NORMAL, OCLGDS("DST_OPTION_CHATTING_ADDED_NORMAL"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_NORMAL"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_ADDED );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_ADDED_WISPHER, OCLGDS("DST_OPTION_CHATTING_ADDED_WISPHER"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_WHISPER"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_ADDED );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_ADDED_PARTY, OCLGDS("DST_OPTION_CHATTING_ADDED_PARTY"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_PARTY"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_ADDED );

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED_FIND_PARTY, OCLGDS("DST_OPTION_CHATTING_ADDED_PARTY_SEARCH"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_PARTY_SEARCH"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED);

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_ADDED_GUILD, OCLGDS("DST_OPTION_CHATTING_ADDED_GUILD"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_GUILD"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_ADDED );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_ADDED_TRADE, OCLGDS("DST_OPTION_CHATTING_ADDED_TRADE"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_TRADE"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_ADDED );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_ADDED_SHOUT, OCLGDS("DST_OPTION_CHATTING_ADDED_SHOUT"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_SHOUT"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_ADDED );

	pNode = NTL_NEW CCheckBoxNode( this, eCHAT_ADDED_SYSTEM, OCLGDS("DST_OPTION_CHATTING_ADDED_SYSTEM"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_SYSTEM"));
	CGuiLineTree::AddNode( pNode, dOPTIONCHATTING_CATEGORY_ADDED );


	// Categories of additional2 chat window options
	pNode = NTL_NEW COptionChattingCategoryNode(this,
		OCLGDS("DST_OPTION_CHATTING_TITLE_ADDED2"),
		dOPTIONCHATTING_CATEGORY_ADDED2);
	CGuiLineTree::AddNode(pNode, GUILINETREE_ROOTNODE_ID);
	pNode->Expand(true);

	// 
	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED2_NORMAL, OCLGDS("DST_OPTION_CHATTING_ADDED_NORMAL"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_NORMAL"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED2);

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED2_WISPHER, OCLGDS("DST_OPTION_CHATTING_ADDED_WISPHER"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_WHISPER"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED2);

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED2_PARTY, OCLGDS("DST_OPTION_CHATTING_ADDED_PARTY"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_PARTY"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED2);

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED2_FIND_PARTY, OCLGDS("DST_OPTION_CHATTING_ADDED_PARTY_SEARCH"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_PARTY_SEARCH"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED2);

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED2_GUILD, OCLGDS("DST_OPTION_CHATTING_ADDED_GUILD"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_GUILD"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED2);

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED2_TRADE, OCLGDS("DST_OPTION_CHATTING_ADDED_TRADE"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_TRADE"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED2);

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED2_SHOUT, OCLGDS("DST_OPTION_CHATTING_ADDED_SHOUT"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_SHOUT"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED2);

	pNode = NTL_NEW CCheckBoxNode(this, eCHAT_ADDED2_SYSTEM, OCLGDS("DST_OPTION_CHATTING_ADDED_SYSTEM"),
		OCLGDS("DST_OPTION_TOOLTIP_CHAT_SYSTEM"));
	CGuiLineTree::AddNode(pNode, dOPTIONCHATTING_CATEGORY_ADDED2);
}

void COptionChattingList::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// class : COptionChatting

COptionChatting::COptionChatting()
{
	m_pOptionChattingList = NULL;
	m_pDlgChattingList = NULL;
}

COptionChatting::~COptionChatting()
{
}

RwBool COptionChatting::Create( COptionWindowGui* pOptionWindow )
{
	NTL_FUNCTION("COptionChatting::Create");

	COptionBase::Create(pOptionWindow);

	m_pDlgChattingList = (gui::CDialog*)GetComponent("dlgChattingList");

	CRectangle rect;
	rect.SetRectWH( dOPTIONCHATTING_LIST_X,
		dOPTIONCHATTING_LIST_Y,
		dOPTIONCHATTING_LIST_WIDTH,
		dOPTIONCHATTING_LIST_HEIGHT );

	m_pOptionChattingList = NTL_NEW COptionChattingList;
	if( !m_pOptionChattingList->Create( rect, m_pDlgChattingList,
		dOPTIONCHATTING_LINE_HEIGHT,			// 각 라인의 높이
		dOPTIONCHATTING_LINE_MARGIN,			// 각 라인의 간격
		dOPTIONCHATTING_CHILD_MARGIN_WIDTH,	// 자식들의 간격
		dOPTIONCHATTING_LIST_SLIDER_WIDTH, this))	// 슬라이더의 넓이
		return FALSE;

	// 조작키를 지정할 수 있는 아이템들을 생성
	m_pOptionChattingList->CreateTree();

	NTL_RETURN(TRUE);
}

void COptionChatting::Destroy()
{
	NTL_FUNCTION("COptionChatting::Destroy");

	if( m_pOptionChattingList )
	{
		m_pOptionChattingList->Destroy();
		NTL_DELETE(m_pOptionChattingList);
	}

	NTL_RETURNVOID();
}

void COptionChatting::Show()
{
	COptionBase::Show();

	m_pScrollBar->Show(false);

	m_pDlgChattingList->Show(true);
}

void COptionChatting::Hide()
{
	COptionBase::Hide();

	m_pScrollBar->Show(true);

	m_pDlgChattingList->Show(false);
}

void COptionChatting::OnInit()
{
	LoadSerializer();
}

void COptionChatting::OnReset()
{
	DefaultOption();	
}

void COptionChatting::OnOk()
{
	SaveSerializer();
	// 적용
	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_CHAT, eNTL_STORAGE_APPLY_ALL );
}

void COptionChatting::OnCancel()
{
	
}

void COptionChatting::OnHandleEvents( RWS::CMsg &pMsg )
{

}

VOID COptionChatting::LoadSerializer()
{
	CCheckBoxNode* pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_INPUTMODE ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_INPUTMODE ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWLOCK ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_WINDOW_LOCK ) );

	/*pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWMSCROLL ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_MOUSE_SCROLL ) );*/

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDEDWINDOW ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_EXTEND_WINDOW ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDEDWINDOW2));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_WINDOW));

	CSliderNode* pSliderNode = static_cast<CSliderNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWOPACITY ));
	pSliderNode->SetValue( GetNtlStorageManager()->GetIntData( dSTORAGE_CHAT_WINDOW_OPACITY ) );


	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_NORMAL ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_BASIC_NORMAL ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_WISPHER ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_BASIC_WISPHER ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_PARTY ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_BASIC_PARTY ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_BASIC_FIND_PARTY));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_BASIC_FIND_PARTY));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_GUILD ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_BASIC_GUILD ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_TRADE ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_BASIC_TRADE ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_SHOUT ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_BASIC_SHOUT ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_SYSTEM ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_BASIC_SYSTEM ) );


	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_NORMAL ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_EXTEND_NORMAL ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_WISPHER ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_EXTEND_WISPHER ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_PARTY ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_EXTEND_PARTY ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED_FIND_PARTY));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND_FIND_PARTY));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_GUILD ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_EXTEND_GUILD ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_TRADE ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_EXTEND_TRADE ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_SHOUT ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_EXTEND_SHOUT ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_SYSTEM ));
	pNode->SetDown( GetNtlStorageManager()->GetBoolData( dSTORAGE_CHAT_EXTEND_SYSTEM ) );


	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_NORMAL));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_NORMAL));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_WISPHER));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_WISPHER));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_PARTY));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_PARTY));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_FIND_PARTY));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_FIND_PARTY));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_GUILD));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_GUILD));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_TRADE));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_TRADE));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_SHOUT));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_SHOUT));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_SYSTEM));
	pNode->SetDown(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHAT_EXTEND2_SYSTEM));
}

VOID COptionChatting::SaveSerializer()
{
	CCheckBoxNode* pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_INPUTMODE ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_INPUTMODE, pNode->GetDown() );
	
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWLOCK ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_WINDOW_LOCK, pNode->GetDown() );
	
	//pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWMSCROLL ));
	//GetNtlStorageManager()->SetData( dSTORAGE_CHAT_MOUSE_SCROLL, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDEDWINDOW ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_EXTEND_WINDOW, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDEDWINDOW2));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_WINDOW, pNode->GetDown());
	
	CSliderNode* pSliderNode = static_cast<CSliderNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWOPACITY ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_WINDOW_OPACITY, pSliderNode->GetValue() );

	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_NORMAL ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_BASIC_NORMAL, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_WISPHER ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_BASIC_WISPHER, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_PARTY ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_BASIC_PARTY, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_BASIC_FIND_PARTY));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_BASIC_FIND_PARTY, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_GUILD ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_BASIC_GUILD, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_TRADE ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_BASIC_TRADE, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_SHOUT ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_BASIC_SHOUT, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_SYSTEM ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_BASIC_SYSTEM, pNode->GetDown() );

	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_NORMAL ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_EXTEND_NORMAL, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_WISPHER ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_EXTEND_WISPHER, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_PARTY ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_EXTEND_PARTY, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED_FIND_PARTY));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND_FIND_PARTY, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_GUILD ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_EXTEND_GUILD, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_TRADE ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_EXTEND_TRADE, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_SHOUT ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_EXTEND_SHOUT, pNode->GetDown() );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_SYSTEM ));
	GetNtlStorageManager()->SetData( dSTORAGE_CHAT_EXTEND_SYSTEM, pNode->GetDown() );

	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_NORMAL));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_NORMAL, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_WISPHER));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_WISPHER, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_PARTY));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_PARTY, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_FIND_PARTY));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_FIND_PARTY, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_GUILD));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_GUILD, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_TRADE));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_TRADE, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_SHOUT));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_SHOUT, pNode->GetDown());

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_SYSTEM));
	GetNtlStorageManager()->SetData(dSTORAGE_CHAT_EXTEND2_SYSTEM, pNode->GetDown());
}

VOID COptionChatting::DefaultOption()
{
	CCheckBoxNode* pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_INPUTMODE ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_INPUTMODE ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWLOCK ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_WINDOW_LOCK ) );

	/*pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWMSCROLL ));
	pNode->SetDown( TRUE );*/

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDEDWINDOW ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_EXTEND_WINDOW ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDEDWINDOW2));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_WINDOW));

	CSliderNode* pSliderNode = static_cast<CSliderNode*>(m_pOptionChattingList->FindNode( eCHAT_WINDOWOPACITY ));
	pSliderNode->SetValue( GetNtlStorageMTContainer()->GetDefaultInt( dSTORAGE_CHAT_WINDOW_OPACITY ) );

	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_NORMAL ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_BASIC_NORMAL ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_WISPHER ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_BASIC_WISPHER ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_PARTY ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_BASIC_PARTY ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_BASIC_FIND_PARTY));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_BASIC_FIND_PARTY));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_GUILD ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_BASIC_GUILD ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_TRADE ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_BASIC_TRADE ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_SHOUT ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_BASIC_SHOUT ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_BASIC_SYSTEM ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_BASIC_SYSTEM ) );

	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_NORMAL ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_EXTEND_NORMAL ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_WISPHER ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_EXTEND_WISPHER ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_PARTY ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_EXTEND_PARTY ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED_FIND_PARTY));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND_FIND_PARTY));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_GUILD ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_EXTEND_GUILD ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_TRADE ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_EXTEND_TRADE ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_SHOUT ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_EXTEND_SHOUT ) );

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode( eCHAT_ADDED_SYSTEM ));
	pNode->SetDown( GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_CHAT_EXTEND_SYSTEM ) );

	//
	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_NORMAL));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_NORMAL));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_WISPHER));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_WISPHER));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_PARTY));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_PARTY));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_FIND_PARTY));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_FIND_PARTY));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_GUILD));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_GUILD));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_TRADE));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_TRADE));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_SHOUT));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_SHOUT));

	pNode = static_cast<CCheckBoxNode*>(m_pOptionChattingList->FindNode(eCHAT_ADDED2_SYSTEM));
	pNode->SetDown(GetNtlStorageMTContainer()->GetDefaultBool(dSTORAGE_CHAT_EXTEND2_SYSTEM));
}