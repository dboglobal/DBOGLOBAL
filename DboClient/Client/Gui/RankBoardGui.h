/*****************************************************************************
 *
 * File			: RankBoardGui.h
 * Author		: Haesung, Cho
 * Copyright	: (주)NTL
 * Date			: 2007. 12. 11	
 * Abstract		: DBO Rank Board Gui
 *****************************************************************************
 * Desc         : 
 * 랭킹 보드
 * 현재는 랭크배틀의 PVP 순위만 표시되지만, 후에 TMQ나 각종 순위에 관련된 게시판
 * 추가 예정
 *****************************************************************************/

#ifndef __RANK_BOARD_H__
#define __RANK_BOARD_H__

#include "RankBoardCommon.h"

/**
* \ingroup client
* \brief RankBoard
*/
class CRankBoardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CRankBoardGui( const RwChar* pName );
	virtual ~CRankBoardGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg &msg);
	RwInt32		SwitchDialog(bool bOpen);		///< DialogManager에서의 Open/Close

	RwBool		IsEnableButton();
	VOID		EnableAllButton();
	VOID		DisableAllButton();

	VOID		Update( RwReal fElapsed );

	// Callback	
	VOID		OnClickedBtnClose( gui::CComponent* pComponent );
	VOID		OnListToggledCbbTitle( RwBool bToggled, gui::CComponent* pComponent );
	VOID		OnClickedBtnTitle( gui::CComponent* pComponent );
	VOID		OnSelectBoard( RwInt32 nIndex );

protected:
	CBoard*			m_pBoard[eBOARD_NUMS];
	eBoardType		m_eCurrentBoard;

	gui::CStaticBox*	m_pstbTitle;
	gui::CButton*		m_pBtnClose;
	gui::CComboBox*		m_pCbbTitle;
	gui::CStaticBox*	m_pStbDataLoading;

	gui::CSlot		m_slotClickedBtnClose;
	gui::CSlot		m_slotClickedBtnTitle;
	gui::CSlot		m_slotToggledTitle;
	gui::CSlot		m_slotSelectBoard;

	RwReal			m_fElapsedTime;
	RwBool			m_bEnableButton;
}; 

#endif