/******************************************************************************
* File			: OptionControlTab.h
* Author		: Cho Hae sung
* Copyright		: (주)NTL
* Date			: 2007. 10. 15 
* Abstract		: 
*****************************************************************************
* Desc			: Tab of Options
*****************************************************************************/

#ifndef _OPTIONCONTROLTAB_H_
#define _OPTIONCONTROLTAB_H_

#pragma once

#include "GuiLineTree.h"

// OptionControlList의 Size
#define dOPTIONCONTROL_LIST_X			0
#define dOPTIONCONTROL_LIST_Y			0
#define dOPTIONCONTROL_LIST_WIDTH		288
#define dOPTIONCONTROL_LIST_HEIGHT		240

// OptionControlList ScrollBar의 Size
#define dOPTIONCONTROL_LIST_SCROLL_WIDTH			12
#define dOPTIONCONTROL_LIST_SCROLL_HEIGHT			19
#define dOPTIONCONTROL_LIST_SLIDER_WIDTH			12
#define dOPTIONCONTROL_LIST_SLIDER_HEIGHT			39

// OptionControl의 StaticBox(설명 부분)
#define dOPTIONCONTROL_STATICBOX_X		0
#define dOPTIONCONTROL_STATICBOX_Y		0
#define dOPTIONCONTROL_STATICBOX_WIDTH	288
#define dOPTIONCONTROL_STATICBOX_HEIGHT 415
#define dOPTIONCONTROL_STATICBOX_COLOR			RGB(255, 219, 71)
#define dOPTIONCONTROL_STATICBOX_WARNING_COLOR	RGB(255, 219, 71)
#define dOPTIONCONTROL_STATICBOX_GUIDE_COLOR	RGB(255, 219, 71)

//#define dOPTIONCONTROL_STATICBOX_WARNING_COLOR	RGB(255, 0, 0)
//#define dOPTIONCONTROL_STATICBOX_GUIDE_COLOR	RGB(0, 255, 255)

// COptionControlCategoryNode [+] Button의 Size
#define dOPTIONCONTROL_CATEGORY_BUTTON_X		0
#define dOPTIONCONTROL_CATEGORY_BUTTON_Y		0
#define dOPTIONCONTROL_CATEGORY_BUTTON_WIDTH	140
#define dOPTIONCONTROL_CATEGORY_BUTTON_HEIGHT	20

// COptionControlCategoryNode Title의 Size
#define dOPTIONCONTROL_CATEGORY_TITLE_X 30
#define dOPTIONCONTROL_CATEGORY_TITLE_Y 0
#define dOPTIONCONTROL_CATEGORY_TITLE_WIDTH		140
#define dOPTIONCONTROL_CATEGORY_TITLE_HEIGHT	20
#define dOPTIONCONTROL_CATEGORY_TITLE_COLOR			RGB(255, 255, 255)
#define dOPTIONCONTROL_CATEGORY_TITLE_COLOR_DOWN	RGB( 255, 219, 71 )

// COptionControlActionNode StaticBox의 Size
#define dOPTIONCONTROL_ACTIONNODE_TITLE_X 0
#define dOPTIONCONTROL_ACTIONNODE_TITLE_Y 0
#define dOPTIONCONTROL_ACTIONNODE_TITLE_WIDTH 147
#define dOPTIONCONTROL_ACTIONNODE_TITLE_HEIGHT 19
#define dOPTIONCONTROL_ACTIONNODE_TITLE_COLOR RGB(255, 255, 255)

// Action Node Statib Box Text의 Coordinate
#define dOPTIONCONTROL_ACTIONNODE_TEXT_COORD_X 10
#define dOPTIONCONTROL_ACTIONNODE_TEXT_COORD_Y 3

// COptionControlActionNode Button의 Size
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_X 150
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_Y 0
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_WIDTH 91
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_HEIGHT 19
#define dOPTIONCONTROL_ACTIONNODE_BUTTON_COLOR RGB(255, 255, 255)

// CGuiLineTree를 구성하는 Item들의 Size
#define dOPTIONCONTROL_LINE_HEIGHT				20
#define dOPTIONCONTROL_LINE_MARGIN				4	
#define dOPTIONCONTROL_CHILD_MARGIN_WIDTH		20

// Action들을 그룹짓는 기준
#define dOPTIONCONTROL_CATEGORY_AVATAR			-2	// 아바타 관련
#define dOPTIONCONTROL_CATEGORY_TARGET			-3	// 타겟 행동
#define dOPTIONCONTROL_CATEGORY_QUICKSLOT		-4	// 퀵슬롯 행동
#define dOPTIONCONTROL_CATEGORY_WINDOW			-5	// 윈도우 관련 행동
#define dOPTIONCONTROL_CATEGORY_FIXED			-6	// 고정(단축키 변경 불가능)

////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : COptionControlCategoryNode, COptionControlActionNode, COptionControlList, COptionControl
// Modified by Kell ( 2007. 10. 01 )
//
// COptionControlList(CGuiLineTree)의 구성
// (RootNode)
//     ├ COptionControlCategoryNode(CGuiLineTreeNode) ( 예: [+] 아바타 행동 관련 )
//     │ ├ COptionControlActionNode(CGuiLineTreeNode)  ( 예:  [아바타 전진]  [ W ] )
//     │ ├ COptionControlActionNode(CGuiLineTreeNode)
//     │ └ ...
//     ├ COptionControlCategoryNode(CGuiLineTreeNode)
//     └ ...
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \ingroup Client
* 조작키 옵션 윈도우의 Action의 Group을 표시하는 NODE
*/
class COptionControlCategoryNode : public CGuiLineTreeNode
{
public:
	COptionControlCategoryNode(CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID);
	virtual ~COptionControlCategoryNode();

	virtual void ShowProc();				///< CGuiLineTreeNode에서의 Override
	virtual void HideProc();				///< CGuiLineTreeNode에서의 Override

	void OnClickBtnExpand(gui::CComponent* pComponent);		///< + 버튼을 클릭할때 실행되는 함수
	void OnClickBtnReduce(gui::CComponent* pComponent);		///< - 버튼을 클릭할때 실행되는 함수

protected:
	gui::CStaticBox* m_pStbTitle;			///< Action Group의 Title
	
	gui::CButton* m_pBtnExpand;				// + Button
	gui::CButton* m_pBtnReduce;				// - Button

	gui::CSlot m_slotClickedBtnExpand;		// + Button's signal
	gui::CSlot m_slotClickedBtnReduce;		// - Button's signal
};

/**
* \ingroup Client
* 조작키 옵션 윈도우 Action Group의 Item들을 표시하는 Node
*/
class COptionControlActionNode : public CGuiLineTreeNode
{
public:
	COptionControlActionNode(CGuiLineTree* pMgr, std::wstring strTitle, RwUInt32 nAction, RwBool bFixed = false);
	virtual ~COptionControlActionNode();

	virtual void ShowProc();
	virtual void HideProc();

	///< 키 이름의 버튼
	void OnClickBtnSetKey(gui::CComponent* pComponent);

	///< 키 버튼의 이름을 지정
	void SetText( std::wstring& strTitle );
	
	void Update();

	void RestoreBtnSetKey() { m_pBtnSetKey->Enable(); }

protected:
	gui::CStaticBox* m_pStbTitle;			///< 액션의 이름
	
	gui::CButton* m_pBtnSetKey;				///< 키 지정 버튼
	
	gui::CSlot m_slotClickedBtnSetKey;		///< 키 지정 버튼의 signal

	RwUInt32 m_nAction;						///< 액션값을 가지고 있는다.
	RwBool m_bFixed;
};

/**
* \ingroup Client
* CGuiLineTreeNode를 상속받는 윈도우 조작키 관련 Node들을 관리하기 위해
* CGuiLineTree를 상속받은 매니저 클래스
*/
class COptionControlList : public CGuiLineTree
{
public:
	COptionControlList();
	virtual ~COptionControlList();

	RwBool	Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, COptionControl* pOptionControl = NULL);

	void CreateTree();									///< 트리를 구축한다.

	///< 트리를 업데이트 한다.
	void UpdateNode();

	///< 스크롤 바를 휠마우스 조작을 하기 위해
	void OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );

	///< 현재의 COptionControlList가 속해있는 탭을 부른다.
	COptionControl* GetParentTab(); 

protected:
	COptionControl*	m_pOptionControl;

	gui::CSlot m_slotCaptureWheelMove;
};

/**
* \ingroup Client
* COptionBase를 상속받아서 옵션 윈도우의 탭 중에 하나인 조작키 관련 탭을 구성
* CGuiLineTree를 포함하여 조작키 입력 지정을 받을 수 있음
*/
class COptionControl : public COptionBase
{
public:
	COptionControl();
	virtual ~COptionControl();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();

	virtual void		Show();
	virtual void		Hide();
		
	virtual void		OnInit();
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

	// 출력될 안내 메시지
public:

	void SetGuideText( RwUInt32 uiColor, const WCHAR* pString );

	void SetGuideDefault();						///< 기본 안내
	void SetGuideChange(RwUInt32 nAction);		///< nAction과 중복됩니다. nAction은 지정안됨으로...
	void SetGuideFixed(RwUInt32 nAction);		///< %s 은 변경 불가능 합니다.
	void SetGuideNegativeMode();				///< 현재 모드에서는 변경이 불가능합니다.
	

protected:
	COptionControlList* m_pOptionControlList;		///< 라인트리의 매니저
	gui::CDialog*		m_pDlgControlList;			///< 라인트리의 부모가 될 Dialog

	gui::CStaticBox*	m_pStbControlTitle;			///< 조작키 변경 창의 제목
	gui::CStaticBox*	m_pStbControlBack;			///< 옵션창의 텍스트 출력창 배경
	gui::CStaticBox*	m_pStbControlText;			///< 옵션창의 텍스트 출력창
};

#endif//OPTIONREFACTOR