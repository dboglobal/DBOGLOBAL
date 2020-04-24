////////////////////////////////////////////////////////////////////////////////
// File: QuestCommonGui.h
// Desc: Quest의 Proposal, Progress, Reward에 쓰이는 공통적인 요소의 추상화 클래스.
//		 QuestReward를 공통화하고, 
//
// 2006.12.13 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_COMMON_GUI_H__
#define __QUEST_COMMON_GUI_H__

#include "NtlPLGui.h"
#include "SurfaceGui.h"
#include "QuestGuiDef.h"
#include "NtlSLEvent.h"

class CQuestGui;

class CQuestCommonGui : public CNtlPLGui
{
public:
	//! Constructor & Destructor
	CQuestCommonGui( const RwChar* pName );
	virtual ~CQuestCommonGui(VOID);
	
protected:
	//! Implementation
	VOID	SetQuestTitle( RwUInt32 uiTitle, RwUInt32 uiSort, RwInt32 eGrade );
	VOID	SetRewardRectHardcode(VOID);
	VOID	SetRewardSlot( RwInt32 nIdx, sREWARD_INFO* pRewardInfo);
	VOID	DeleteRewardSlot( RwInt32 nIdx );
	VOID	DeleteAllRewardSlot(VOID);

	VOID	ShowInfoWnd( RwInt32 nIdx );
	RwInt32 GetRewardSlotIdx( RwInt32 nX, RwInt32 nY );		// 0 ~ 3 : Select, 4 ~ 7 : Basic

	RwBool	IsAvailableSelect(RwInt32 nIdx);

	RwBool	CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue );
	VOID	DeleteStackNumber( RwInt32 nSlotIdx );

	VOID	InitQuestData(VOID);

	//! Callback
	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnMouseOut( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nX, RwInt32 nY );

	//! Variable
	gui::CStaticBox*	m_pstbTitle;			// Dialog Title
	gui::CButton*		m_pbtnClose;			
	gui::CStaticBox*	m_pstbQuestTitle;
	gui::CStaticBox*	m_pstbQuestGrade;
	gui::CStaticBox*	m_pstbQuestLevel;

	gui::CStaticBox*	m_pstbStandardRewardTitle;
	gui::CStaticBox*	m_pstbSelectRewardTitle;

	gui::CStaticBox*	m_pstbRewardGetExpText;
	gui::CStaticBox*	m_pstbRewardGetExpAmountText;

	gui::CStaticBox*	m_pstbRewardGetZennyText;
	gui::CStaticBox*	m_pstbRewardGetZennyAmountText;

	gui::CStaticBox*	m_pstbRewardSelectText;
	gui::CStaticBox*	m_pstbRewardSelectAmountText;

	CRectangle			m_rtReward[QUEST_MAX_TOTAL_REWARD_SLOT];		// 0 ~ 3 : Select
	CSurfaceGui			m_surReward[QUEST_MAX_TOTAL_REWARD_SLOT];		// 4 ~ 7 : Standard
	gui::CStaticBox*	m_pstbStackCount[QUEST_MAX_TOTAL_REWARD_SLOT];
	sREWARD_INFO		m_sReward[QUEST_MAX_TOTAL_REWARD_SLOT];

	RwInt32				m_nMouseOnIdx;

	CNtlTCUnit*			m_pTCUnit;
	sTS_KEY				m_TSKey;
	RwUInt32			m_uiTitleIdx;

	CQuestGui*			m_pQuestGui;
};

#endif//__QUEST_COMMON_GUI__