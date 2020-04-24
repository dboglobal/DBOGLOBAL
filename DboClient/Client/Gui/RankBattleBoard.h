/******************************************************************************
* File			: RankBattleBoard.h
* Author		: Cho Hae sung
* Copyright		: (주)NTL
* Date			: 2007. 12. 14
* Abstract		: 
*****************************************************************************
* Desc			: 랭크배틀의 순위 게시판
*****************************************************************************/

#ifndef __RANKBATTLE_H__
#define __RANKBATTLE_H__

#include "RankBoardCommon.h"

// RankBattle Define

#define dRANKBOARD_RANKBATTLE_ROWITEM_Y				67
#define dRANKBOARD_RANKBATTLE_ROWITEM_NUMS			10
#define dRANKBOARD_RANKBATTLE_ROWITEM_MARGIN_Y		0
#define dRANKBOARD_RANKBATTLE_ROWITEM_FONT_HEIGHT	90

#define dRANKBOARD_RANKBATTLE_FIRST_PAGE			0
#define dRANKBOARD_RANKBATTLE_FIRST_COMPARE			1

#define dRANKBOARD_RANKBATTLE_UNDERLINE_NUMS		9
#define dRANKBOARD_RANKBATTLE_UNDERLINE_WIDTH		612

struct sRANKBATTLE_ITEM
{
	RwInt32		nRank;												///< 순위
	WCHAR		awcName[NTL_MAX_SIZE_CHAR_NAME+1];			///< 캐릭터 이름
	RwInt32		nLevel;												///< 레벨
	RwUInt8		byClass;											///< 클래스
	WCHAR		awcGuildName[NTL_MAX_SIZE_GUILD_NAME+1];	///< 길드 이름
	RwInt32		nPoint;												///< 점수
	RwUInt16	wStraightWin;										///< 연승
	RwUInt32	nCompare;											///< 비교날짜
};

/**
* \ingroup client
* \brief 랭크배틀의 항목들
*/
class CRankBattleRowItem
{
public:
	CRankBattleRowItem();
	virtual ~CRankBattleRowItem();

	VOID Create( gui::CComponent* pParentGui, RwInt32 nTop );
	VOID Destroy();

	VOID Show( RwBool bShow = TRUE );

	// Set Subject
	VOID SetItem( RwInt32 nRank, const WCHAR* pwcName, RwInt32 nLevel, RwUInt8 byClass, const WCHAR* pwcGuildName, RwInt32 nPoint, RwUInt16 nStraightWin, RwUInt32 nCompare , RwBool bHightLight = FALSE);
	VOID SetCompareInfo( RwInt32 nCompare );
	VOID ClearItem();

	// Set ToolTip
	VOID OnMouseEnterFromClass( gui::CComponent* pComponent );
	VOID OnMouseLeaveFromClass( gui::CComponent* pComponent );

protected:
	gui::CStaticBox*	m_pStbRank;				///< 순위
	gui::CStaticBox*	m_pStbName;				///< 이름
	gui::CStaticBox*	m_pStbLevel;			///< 레벨
	gui::CPanel*		m_pPanClass;			///< 직업
	gui::CStaticBox*	m_pStbGuildName;		///< 길드이름
	gui::CStaticBox*	m_pStbPoint;			///< 점수
	gui::CStaticBox*	m_pStbStraightWin;		///< 연승
	gui::CStaticBox*	m_pStbCompare;			///< 비교
	gui::CPanel*		m_pPanCompareMark;		///< 비교마크
	gui::CDialog*		m_pDlgItem;

	gui::CSlot			m_slotMouseEnterFromClass;
	gui::CSlot			m_slotMouseLeaveFromClass;

	RwInt32				m_nRank;				///< 순위

	RwUInt8				m_byClass;				///< 직업


};

/**
* \ingroup client
* \brief 랭크배틀 순위 게시판
*/
class CRankBattleBoard : public CBoard
{
public:
	// 서버에서 받은 데이타의 유효값
	enum eDataResult
	{
		eDATA_OK = 0,
		eDATA_ONLYCOMPARE,
		eDATA_NONE,
		eDATA_NOTPAGE,
		eDATA_NOTFIND,

		eDATA_NUMS,
		eDATA_INVALID = 0xFF
	};

	// 검색 모드
	enum eSearchMode
	{
		eSEARCH_RANK = 0,	// 랭크로 검색
		eSEARCH_NAME,

		eSEARCH_NUMS,
		eSEARCH_INVALID = 0xFF
	};

public:
	CRankBattleBoard();
	virtual ~CRankBattleBoard();

	// abstract
	virtual void Create( eBoardType eType , CRankBoardGui* pParent );
	virtual void Destroy();

	virtual void Show( RwBool bShow = TRUE );
	virtual void Enable( RwBool bEnable = TRUE );

	virtual void MessageProc( RWS::CMsg& msg );

	virtual void CurrentPage();

	// Interface
	VOID		ApplyData();
	VOID		RowItemClear();
	VOID		SetDataResult( RwUInt8 byDataResult );

	// Callback
	VOID		OnClickedBtnPrev( gui::CComponent* pComponent );
	VOID		OnClickedBtnNext( gui::CComponent* pComponent );
	
	void		OnInputReturnSearch();
	void		OnClickedBtnSearch(gui::CComponent* pComponent);

	// 서치 관련 콤보박스
	VOID		OnSearchItemSelect( INT nIndex );
	VOID		OnSearchListToggled( RwBool bToggled, gui::CComponent* pComponent );

	// Compare 관련 콤보박스 
	VOID		OnItemSelect( INT nIndex );
	VOID		OnListToggled( RwBool bToggled, gui::CComponent* pComponent );
	VOID		OnClickedCompare( gui::CComponent* pComponent );

	VOID		OnIpbSearchGotFocus();
	VOID		OnIpbSearchLostFocus();
	
protected:
	CRankBoardGui*		m_pParentGui;

	CRankBattleRowItem	m_RankBattleRowItem[dRANKBOARD_RANKBATTLE_ROWITEM_NUMS];

	gui::CDialog*		m_pRankBattleBoardDlg;
	gui::CPanel*		m_pPanUnderLine[dRANKBOARD_RANKBATTLE_UNDERLINE_NUMS];

	gui::CButton*		m_pBtnPrev;
	gui::CButton*		m_pBtnNext;
	gui::CComboBox*		m_pCbbSearchTitle;

	gui::CSlot			m_slotSearchItemSelect;
	gui::CSlot			m_slotSearchListToggled;

	RwInt32				m_nSearchMode;
	RwBool				m_bSearchRank;				///< 순위 모드임

	RwInt32				m_nSearchRankPage;			///< 순위 모드일때의 페이지
	RwInt32				m_nSearchRankIndex;			///< 순위 모드일때 하이라이트 해줄 위치

	gui::CInputBox*		m_pIpbSearch;
	gui::CButton*		m_pBtnSearch;

	gui::CSlot			m_slotClickedBtnPrev;
	gui::CSlot			m_slotClickedBtnNext;
	gui::CSlot			m_slotReturnIptSearch;
	gui::CSlot			m_slotClickedBtnSearch;

	gui::CSlot			m_slotIpbSearchGotFocus;
	gui::CSlot			m_slotIpbSearchLostFocus;

	gui::CStaticBox*	m_pStbRank;					///< 순위
	gui::CStaticBox*	m_pStbName;					///< 이름
	gui::CStaticBox*	m_pStbLevel;				///< 레벨
	gui::CStaticBox*	m_pStbClass;				///< 직업
	gui::CStaticBox*	m_pStbGuildName;			///< 길드이름
	gui::CStaticBox*	m_pStbPoint;				///< 점수
	gui::CStaticBox*	m_pStbStraightWin;			///< 연승
	/*gui::CComboBox*		m_pCbbCompare;				///< 비교날짜( 콤보박스 )*/
	gui::CStaticBox*	m_pStbRankBattleCompare;		///< 변동 사항

	gui::CSlot			m_slotItemSelect;			///< 아이템 셀렉트
	gui::CSlot			m_slotListToggled;			///< 리스트박스 토글
	gui::CSlot			m_slotClickedCompare;		///< 리스트박스의 컴페어 버튼 클릭

	RwInt32				m_nCurrentPage;
	RwInt32				m_nCurrentCompareDay;

	RwUInt8				m_byDataResult;

	RwInt32				m_nRankBattleItemCount;
	sRANKBATTLE_ITEM	m_sRankBattleItem[dRANKBOARD_RANKBATTLE_ROWITEM_NUMS];
	WCHAR				m_awcSearchName[NTL_MAX_SIZE_CHAR_NAME+1];
};


#endif//__RANKBATTLE_H__