////////////////////////////////////////////////////////////////////////////////
// File: QuestProposalGui.h
// Desc: QuestProposal
//
// 2006.10.21 Peessi@hitel.net   
//
// To Be Desired: Quest Progress, Quest Reward Gui와 통합할 것인가 추후 결정.
//	
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_PROPOSAL_GUI_H__
#define __QUEST_PROPOSAL_GUI_H__

#include "QuestCommonGui.h"

class CQuestProposalGui : public CQuestCommonGui
{
public:
	//! Constructor & Destructor
	CQuestProposalGui( const RwChar* pName );
	~CQuestProposalGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );
	VOID	SendEchoEvent( bool bConfirm );

	//! Event
	VOID HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	SetBasicUISetting(VOID);
	VOID	SetQuestData( SNtlEventQuestDirect_Forward* pData );
	
	
	//! Callback
	gui::CSlot		m_slotMouseMove;
	gui::CSlot		m_slotMouseOut;
	gui::CSlot		m_slotPaint;
	gui::CSlot		m_slotMove;
	
	gui::CSlot		m_slotClickExitButton;
	gui::CSlot		m_slotClickConfirmButton;
	gui::CSlot		m_slotClickRejectButton;
	gui::CSlot		m_slotWheelMoveConv;
	gui::CSlot		m_slotCaptureMouseDown;

	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnMouseOut( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nX, RwInt32 nY );

	VOID	OnClickCinfirmButton( gui::CComponent* pComponent );
	VOID	OnClickRejectButton( gui::CComponent* pComponent );
	VOID	OnWheelMoveConv( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID	OnCaptureMouseDown( const CKey& key );

	VOID	OnClickedBtnHelp( gui::CComponent* pComponent );

	//! Variables
	gui::CStaticBox*	m_pstbConversationTitle;
	gui::CHtmlBox*		m_phbxConversation;		// Quest 대화

	gui::CStaticBox*	m_pstbAimTitle;		
	gui::CHtmlBox*		m_phbxAim;				// Quest 목표

	gui::CButton*		m_pbtnQuestConfirm;
	gui::CButton*		m_pbtnQuestReject;

	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotClickedBtnHelp;

	bool				m_bResult;
};

#endif