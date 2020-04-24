////////////////////////////////////////////////////////////////////////////////
// File: QuestIndicatorGui.h
// Desc: QuestIndicator
//
// 2006.11.22 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_INDICATOR_GUI_H__
#define __QUEST_INDICATOR_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlSLEvent.h"
#include "QuestGuiDef.h"
#include "Windowby3.h"

#define QUEST_INDICATOR_QUESTTITLE_HEIGHT	23
#define QUEST_INDICATOR_MAX_HEIGHT			345
#define QUEST_INDICATOR_QUESTTITLE_MARGIN	5

class CQuestGui;

struct stINDICATOR_ITEM
{
	gui::CDialog*		m_pFrame;
	CWindowby3			m_Background;
	gui::CButton*		m_pTitle;
	gui::CButton*		m_pbtnExpend;
	gui::CPanel*		m_pState;
	gui::CStaticBox*	m_pGoalInfo[QUEST_MAX_GOAL_COUNT];
	gui::CStaticBox*	m_pGoalResult[QUEST_MAX_GOAL_COUNT];
	gui::CProgressBar*	m_pNewGoalResult[QUEST_MAX_GOAL_COUNT];
	gui::CStaticBox*	m_pTime;

	RwInt32				m_nFrameHeight;	
	sTS_KEY				m_TSKey;			

	RwUInt32			m_uiQuestTitle;
	RwUInt32			m_uiQuestGoal;

	gui::CSlot			m_slotClickTitle;
	gui::CSlot			m_slotMouseEnter;
	gui::CSlot			m_slotMouseLeave;

	VOID	OnClickTitle( gui::CComponent* pComponent );
	VOID	OnMouseEnterTitle( gui::CComponent* pComponent );
	VOID	OnMouseLeaveTitle( gui::CComponent* pComponent );

	void				OnClickExpend(gui::CComponent* pComponent);
	gui::CSlot			m_slotClickExpend;

	bool				m_bQuestInfoShown;
};

class CQuestIndicatorGui : public CNtlPLGui
{	
public:
	//! Constructor & Destructor
	CQuestIndicatorGui(VOID);
	CQuestIndicatorGui( const RwChar* pName );
	~CQuestIndicatorGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

	RwBool	IsIndicatedQuest( RwUInt32 nID );
	
	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	typedef std::list<SNtlEventShowIndicator_Nfy*> LIST_DATA;

private:
	//! Implementation
	VOID	Init(VOID);
	VOID	Refresh(VOID);						
	VOID	DrawData( RwInt32 nIdx, SNtlEventShowIndicator_Nfy* pIndicatorData );
	VOID	UpdateData( SNtlEventUpdateQuestProgressInfo_Nfy* pData );
	VOID	UpdateState( sTS_KEY& sTSKey, RwUInt32 uiQuestState );
	VOID	UpdateResultItem( const WCHAR* pInfo, RwInt32 nCurVal, RwInt32 nTargetVal, RwInt32 i, RwInt32 j );
	VOID	UpdateResultItem( const WCHAR* pInfo, RwBool bComplete, RwInt32 i, RwInt32 j );
	VOID	CalculatePosition(VOID);	
	RwBool	IsAvaliableHeight( SNtlEventShowIndicator_Nfy* pData );
	RwBool	IsAvaliableHeight(VOID);

	RwInt32 FindData( RwUInt32 nID, SNtlEventShowIndicator_Nfy** ppData );	// index ∏Æ≈œ.
	VOID	CreateData( SNtlEventShowIndicator_Nfy* pData );
	VOID	DeleteData( RwUInt32 nID );
	VOID	DeleteAllData(VOID);	

	void	SetListItem(int nOffset); // scrollbar

	//! Callback
	void	OnPaint();
	void	OnClickMinimize( gui::CComponent* pComponent );
	void	OnClickAllExtendOn(gui::CComponent* pComponent);
	void	OnClickAllExtendOff(gui::CComponent* pComponent);
	void	OnClickQuestListOpen(gui::CComponent* pComponent);
	void	OnScrollChanged(int nNewOffset);
	void	OnMouseWheel(int nFlag, RwInt16 sDelta, CPos& pos);

	//! Variables
	gui::CSlot			m_slotPaint;

	gui::CSlot			m_slotClickMinimize;
	gui::CSlot			m_slotClickQuestListOpen;
	gui::CSlot			m_slotClickAllExtendOn;
	gui::CSlot			m_slotClickAllExtendOff;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollSliderMoved;
	gui::CSlot			m_slotMouseWheel;

	gui::CButton*		m_pbtnMinimize;
	gui::CButton*		m_pbtnAllExtendOn;
	gui::CButton*		m_pbtnAllExtendOff;
	gui::CStaticBox*	m_pstbTitle;

	gui::CPanel*		m_ppnClippingArea;
	gui::CPanel*		m_ppnIndicatorList;

	gui::CScrollBar*	m_pscrBar;

	gui::CPanel*		m_ppnTitlePanel;

	gui::CButton*		m_pbtnQuestListOpen;

	stINDICATOR_ITEM	m_IndicatorItem[eMAX_CAN_PROGRESS_QUEST_NUM];
	RwInt32				m_nTotalItemHeight;
	
	LIST_DATA			m_listData;	

	CQuestGui*			m_pQuestGui;	// for Refrerence

private:

	int					m_nScrollPos;
};

#endif