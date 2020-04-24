////////////////////////////////////////////////////////////////////////////////
// File: QuestGui.h
// Desc: QuestGui
//		 Quest에 관련된 모든 윈도우들의 관리.
//
// 2006.10.19 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_GUI_H__
#define __QUEST_GUI_H__

#include "ceventhandler.h"

class CQuestListGui;
class CQuestProgressGui;
class CQuestProposalGui;
class CQuestRewardGui;
class CQuestIndicatorGui;
class CMultiDialogGui;
class CQuestInventoryGui;
class CQuestNarrationGui;
class CQuestMessageGui;

class CQuestGui : public RWS::CEventHandler
{
public:
	//! Constructor & Destructor
	CQuestGui(VOID);
	~CQuestGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	CQuestListGui*		GetQuestListGui(VOID) { return m_pQuestList; }
	CQuestProgressGui*	GetQuestProgressGui(VOID) { return m_pQuestProgress; }
	CQuestProposalGui*	GetQuestProposalGui(VOID) { return m_pQuestProposal; }
	CQuestIndicatorGui*	GetQuestIndicatorGui(VOID) { return m_pQuestIndicator; }
	CQuestInventoryGui* GetQuestInventoryGui(VOID) { return m_pQuestInventory; }
	CQuestRewardGui*	GetQuestRewardGui(VOID) { return m_pQuestReward; }
	CQuestNarrationGui*	GetQuestNarrationGui(VOID) { return m_pQuestNarration; }
	CQuestMessageGui*	GetQuestMessageGui(VOID) { return m_pQuestMessage;}
	CMultiDialogGui*	GetMultiDialogGui(VOID) { return m_pMultiDialog; }	

	//! Event
	VOID HandleEvents( RWS::CMsg& msg );

private:
	//! Implement
	
	//! Variable
	CQuestListGui*		m_pQuestList;
	CQuestProgressGui*	m_pQuestProgress;
	CQuestProposalGui*	m_pQuestProposal;
	CQuestIndicatorGui*	m_pQuestIndicator;
	CQuestInventoryGui* m_pQuestInventory;
	CQuestRewardGui*	m_pQuestReward;
	CQuestNarrationGui*	m_pQuestNarration;
	CQuestMessageGui*	m_pQuestMessage;
	CMultiDialogGui*	m_pMultiDialog;
};

#endif//__QUEST_GUI_H__