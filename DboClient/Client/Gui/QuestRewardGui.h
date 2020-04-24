////////////////////////////////////////////////////////////////////////////////
// File: QuestRewardGui.h
// Desc: QuestReward
//
// 2006.10.21 Peessi@hitel.net   
//
// To Be Desired: Quest Progress, Quest Reward Gui와 통합할 것인가 추후 결정.
//	
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_REWARD_GUI_H__
#define __QUEST_REWARD_GUI_H__

#include "QuestCommonGui.h"

class CQuestRewardGui : public CQuestCommonGui
{
public:
	//! Constructor & Destructor
	CQuestRewardGui( const RwChar* pName );
	~CQuestRewardGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

	//! Event
	VOID HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	SetBasicUISetting(VOID);
	VOID	SetQuestData( SNtlEventQuestDirect_Forward* pData );
	VOID	SendEchoEvent( bool bConfirm );
	
	VOID	SelectIndex( RwInt32 nIdx );
		
	//! Callback
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseUp( const CKey& key );
	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnMouseOut( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nX, RwInt32 nY );

	VOID	OnClickExitButton( gui::CComponent* pComponent );
	VOID	OnClickRewardButton( gui::CComponent* pComponent );
	VOID	OnWheelMoveConv( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID	OnCaptureMouseDown( const CKey& key );

	gui::CSlot		m_slotMouseDown;
	gui::CSlot		m_slotMouseUp;
	gui::CSlot		m_slotMouseMove;
	gui::CSlot		m_slotMouseOut;
	gui::CSlot		m_slotPaint;
	gui::CSlot		m_slotMove;

	gui::CSlot		m_slotClickExitButton;
	gui::CSlot		m_slotClickRewardButton;
	gui::CSlot		m_slotClickCancelButton;
	gui::CSlot		m_slotWheelMoveConv;
	gui::CSlot		m_slotCaptureMouseDown;

	//! Variables
	gui::CStaticBox*	m_pstbConversationTitle;
	gui::CHtmlBox*		m_phbxConversation;		// Quest 대화

	gui::CButton*		m_pbtnQuestReward;
	gui::CButton*		m_pbtnRewardCancel;
			
	RwInt32				m_nLClickIdx;

	CSurfaceGui			m_surSelectEffect;
	RwInt32				m_nSelectIndex;

	RwBool				m_bCanSelectReward;

	bool				m_bResult;
};

#endif