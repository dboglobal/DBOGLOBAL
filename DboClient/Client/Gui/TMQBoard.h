/******************************************************************************
* File			: TMQBoard.h
* Author		: Cho Hae sung
* Copyright		: (주)NTL
* Date			: 2008. 4. 13
* Abstract		:  
*****************************************************************************
* Desc			: TMQ의 순위 게시판
*****************************************************************************/

#ifndef __TMQBOARD_H__
#define __TMQBOARD_H__

#include "RankBoardCommon.h"
#include "NtlTimeQuest.h"

#define dRANKBOARD_TMQ_PARTYITEM_UNDERLINE_NUMS 5
#define dRANKBOARD_TMQ_PARTYITEM_NUMS			6
#define dRANKBOARD_TMQ_PARTYMEMBERITEM_NUMS		5

#define dRANKBOARD_TMQ_PARTYITEM_FONT_HEIGHT		90
#define dRANKBOARD_TMQ_PARTYMEMBERITEM_FONT_HEIGHT	90

#define dRANKBOARD_TMQ_PARTYTIEM_HEIGHT				33

class CTMQBoard;

/**
* \ingroup client
* \brief 파티 리스트 아이템
*/
class CTMQPartyListItem
{
public:
	CTMQPartyListItem();
	virtual ~CTMQPartyListItem();

	VOID Create( CTMQBoard* pBoard, gui::CComponent* pParentGui, RwUInt8 byIndex, RwInt32 nTop );
	VOID Destroy();

	// Set Subject
	VOID SetItem( RwInt32 nRank, const WCHAR* pwcPartyName, RwUInt32 uiClearTime );
	VOID ClearItem();

	VOID Enable( RwBool bEnable );

	// Callback
	VOID OnMouseUp( const CKey& key );

	CRectangle GetClientRect() { return m_pDlgPartyListItem->GetClientRect(); }
	CRectangle GetScreenRect() { return m_pDlgPartyListItem->GetScreenRect(); }

protected:
	CTMQBoard*			m_pTMQBoard;

	RwUInt8				m_byIndex;
	RwBool				m_bEnable;

	gui::CDialog*		m_pDlgPartyListItem;
	gui::CStaticBox*	m_pStbPartyName;
	gui::CStaticBox*	m_pStbClearTime;
	gui::CPanel*		m_pPanUnderLine;

	gui::CSlot			m_slotMouseUp;
};

/**
* \ingroup client
* \brief TMQ Party Member 리스트의 아이템
*/
class CTMQPartyMemberItem
{
public:
	CTMQPartyMemberItem();
	virtual ~CTMQPartyMemberItem();

	VOID Create( gui::CComponent* pParentGui, RwInt32 nTop );
	VOID Destroy();

	// Set Subject
	VOID SetItem( const WCHAR* pwcMemberName, RwInt32 nLevel, RwUInt8 byClass );
	VOID ClearItem();

protected:
	gui::CDialog*		m_pDlgPartyMemberItem;

	gui::CStaticBox*	m_pStbPartyMemberName;
	gui::CStaticBox*	m_pStbLevel;
	gui::CPanel*		m_pPanClass;

	RwUInt8				m_byClass;
};

/**
* \ingroup client
* \brief TMQ Board
*/
class CTMQBoard : public CBoard
{
public:
	enum eDataResult
	{
		eDATA_OK = 0,
		eDATA_NONE = 1,
		eDATA_NOMEMBER = 2,

		eDATA_NUMS,
		eDATA_INVALID = eDATA_NUMS
	};

public:
	CTMQBoard();
	virtual ~CTMQBoard();

	// abstract
	virtual void Create( eBoardType eType, CRankBoardGui* pParentGui );
	virtual void Destroy();

	virtual void Show( RwBool bShow = TRUE );
	virtual void Enable( RwBool bEnable = TRUE );

	virtual void MessageProc( RWS::CMsg& msg );

	virtual void CurrentPage();

	// User Interface
	VOID	ClearInterface();
	VOID	ClearPartyList();
	VOID	ClearMemberList();
	VOID	SelectMemberList( RwUInt8 byPartyIndex );
	VOID	SelectDifficult( RwUInt8 byDifficult );

	VOID	SetDifficultButton( RwUInt8 byDifficult );
	VOID	SetDataResult( RwUInt8 byDataResult );

	VOID	ApplyData();

	// Callback
	VOID	OnClickedBtnPrev( gui::CComponent* pComponent );
	VOID	OnClickedBtnNext( gui::CComponent* pComponent );
	VOID	OnListToggled( RwBool bToggled, gui::CComponent* pComponent );
	VOID	OnClickedBtnEasy( gui::CComponent* pComponent );
	VOID	OnClickedBtnNormal( gui::CComponent* pComponent );
	VOID	OnClickedBtnHard( gui::CComponent* pComponent );
	VOID	OnItemSelect( RwInt32 nIndex );

	// Gui
protected:
	CRankBoardGui*		m_pParentGui;

	gui::CDialog*		m_pTMQBoardDlg;	
	gui::CButton*		m_pBtnPrev;
	gui::CButton*		m_pBtnNext;
	gui::CButton*		m_pBtnNormal;
	gui::CButton*		m_pBtnHard;
	gui::CComboBox*		m_pCbbTitle;
	gui::CStaticBox*	m_pStbTMQRank;
	gui::CStaticBox*	m_pStbTMQPartyName;
	gui::CStaticBox*	m_pStbTMQClearTime;
	gui::CStaticBox*	m_pStbPartyMemberInfo;
	gui::CStaticBox*	m_pStbPartyMember;
	gui::CStaticBox*	m_pStbPartyLevel;
	gui::CStaticBox*	m_pStbPartyClass;
	/*gui::CComboBox*		m_pCbbTMQScenarioList;*/
	gui::CPanel*		m_pPanTMQHighLight;

	gui::CSlot			m_slotClickedBtnPrev;
	gui::CSlot			m_slotClickedBtnNext;
	gui::CSlot			m_slotListToggled;
	gui::CSlot			m_slotClickedScenario;
	gui::CSlot			m_slotClickedBtnEasy;
	gui::CSlot			m_slotClickedBtnNormal;
	gui::CSlot			m_slotClickedBtnHard;
	gui::CSlot			m_slotOnItemSelect;

	// Data
protected:
	typedef				std::vector<RwUInt32>	VECTBLIDX;
	VECTBLIDX			m_vecTblIdx;

	RwUInt32			m_nCurrentScenario;
	RwUInt8 			m_nCurrentDifficulty;

	CTMQPartyListItem		m_itemPartyList[dRANKBOARD_TMQ_PARTYITEM_NUMS];
	CTMQPartyMemberItem		m_itemPartyMember[dRANKBOARD_TMQ_PARTYMEMBERITEM_NUMS];

	// 서버에서 받은 정보를 GUI에서 가지고 있기 위하여 패킷과 똑같은 구조체를 BOARD에서 가지고 있는다.
	RwUInt8							m_byTeamCount;
	RwUInt8							m_byMemberCount;
	sTIMEQUEST_TEAM_RANK_DATA		m_sTeamRankData[dRANKBOARD_TMQ_PARTYITEM_NUMS];
	sTIMEQUEST_PARTY_MEMBER_DATA	m_sMemberData[dRANKBOARD_TMQ_PARTYMEMBERITEM_NUMS];

	RwUInt8				m_byDataResult;
};

#endif//__TMQBOARD_H__