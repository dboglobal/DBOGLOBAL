////////////////////////////////////////////////////////////////////////////////
// File: QuestProgressGui.h
// Desc: QuestProgress
//
// 2006.10.21 Peessi@hitel.net   
//
// To Be Desired: Quest Proposal, Quest Reward Gui와 통합할 것인가 추후 결정.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_PROGRESS_GUI_H__
#define __QUEST_PROGRESS_GUI_H__

#include "QuestCommonGui.h"

struct stQUESTGIVEUP
{
	sTS_KEY			sTSKey;	
	WCHAR			QuestTitle[GUI_TEXT_BUFFER_SIZE];
};

class CQuestProgressGui : public CQuestCommonGui
{
public:
	//! Enum
	enum TAB { CONV_TAB, AIM_TAB };

	//! Constructor & Destructor
	CQuestProgressGui( const RwChar* pName );
	~CQuestProgressGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	SetBasicUISetting(VOID);
	VOID	SetQuestData( SNtlEventShowQuestWindow_Nfy* pData );
	VOID	UpdateUIData( eSTOC_EVT_DATA_TYPE eEvtInfoType, uSTOC_EVT_DATA* puEvtInfoData, RwUInt32 uiTimeLimit );
	VOID	SetSenderRecieverData( RwUInt32 uiEventType, RwUInt32 uiEventID, RwBool bStarter );
	VOID	UpdateResultItem( const WCHAR* pInfo, RwInt32 nCurVal, RwInt32 nTargetVal, RwInt32 i );
	VOID	UpdateResultItem( const WCHAR* pInfo, RwBool bComplete, RwInt32 i );

	VOID	CalcResultWidth(VOID);
	VOID	SelectTabPage( TAB eTab );
	VOID	ClearProgressInfo(VOID);

	//! Callback
	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnMouseOut( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nX, RwInt32 nY );

	VOID	OnClickExitButton( gui::CComponent* pComponent );
	VOID	OnClickGiveUpButton( gui::CComponent* pComponent );
	VOID	OnSelectChangeTabButton( INT nCurIndex, INT nPrevIndex );
	VOID	OnWheelMoveConv( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID	OnCaptureMouseDown( const CKey& key );
	VOID	OnClickedBtnHelp( gui::CComponent* pComponent );

	gui::CSlot		m_slotMouseMove;
	gui::CSlot		m_slotMouseOut;
	gui::CSlot		m_slotPaint;
	gui::CSlot		m_slotMove;

	gui::CSlot		m_slotClickExitButton;
	gui::CSlot		m_slotClickGiveUpButton;
	gui::CSlot		m_slotClickCloseButton;
	gui::CSlot		m_slotSelectChangeTabButton;
	gui::CSlot		m_slotWheelMoveConv;
	gui::CSlot		m_slotCaptureMouseDown;
		
	//! Variables	
	gui::CPanel*		m_ppnlConversationBack;
	gui::CHtmlBox*		m_phbxConversation;		// Quest 대화
	
	gui::CPanel*		m_ppnlAimBack;
	gui::CHtmlBox*		m_phbxAim;				// Quest 목표

	gui::CStaticBox*	m_pstbAimTargetTitle;	// 의뢰처, 보고처
	gui::CStaticBox*	m_pstbAimTargetValue;	// 의뢰처, 보고처의 이름
	gui::CStaticBox*	m_pstbAimTitle;			// 목표 타이틀. (탭이외에 의뢰처,보고처가 나올때 밑에 나온다.)
	
	gui::CStaticBox*	m_pstbProgressTitle;
	gui::CStaticBox*	m_pstbTimeTitle;
	gui::CStaticBox*	m_pstbTime;
	gui::CPanel*		m_ppnlTimeBack;
	gui::CPanel*		m_ppnlProgressBack;
	gui::CStaticBox*	m_pstbProgressInfo[QUEST_MAX_GOAL_COUNT];
	gui::CStaticBox*	m_pstbProgressResult[QUEST_MAX_GOAL_COUNT];
	
	gui::CButton*		m_pbtnQuestClose;
	gui::CButton*		m_pbtnQuestGiveUp;
	gui::CTabButton*	m_ptbtTab;

	RwBool				m_bShowTimeUI;

	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotClickedBtnHelp;
};

#endif