////////////////////////////////////////////////////////////////////////////////
// File: RBChallengeGui.h
// Desc: Rank Battle µµÀü Gui
//
// 2007.07.20 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __RB_CHALLENGE_GUI_H__
#define __RB_CHALLENGE_GUI_H__
//
//#include "NtlPLGui.h"
//#include "ceventhandler.h"

//class CRBChallengeItem
//{
//public:
//	//! Constructor and Destructor
//	CRBChallengeItem(VOID);
//	~CRBChallengeItem(VOID);
//
//	//! Operation
//	RwBool	Create( gui::CComponent* pParent, gui::CSurfaceManager* pSurfaceManager, RwInt32 nItemID );
//	VOID	Destroy(VOID);
//
//	VOID	Show( bool bShow = true ) { m_pThis->Show( bShow ); } 
//
//	VOID	SetItemData( WCHAR* pName, RwInt32 nLevel, RwInt32 eClass );
//
//	//! Attributes	
//
//protected:
//	//! Callback
//
//	//! Variables	
//	gui::CStaticBox*	m_pName;
//	gui::CStaticBox*	m_pLevel;
//	gui::CPanel*		m_pClass;		
//
//	gui::CDialog*		m_pThis;
//};
//
//class CRBChallengeGui : public CNtlPLGui, public RWS::CEventHandler
//{
//public:
//	//! Define
//	enum { MAX_CHALLENGE_ITEM = 5 };
//
//	//! Constructor and Destructor
//	CRBChallengeGui(VOID);
//	CRBChallengeGui( const RwChar* pName );
//	~CRBChallengeGui(VOID);
//
//	//! Operation
//	VOID	Init(VOID);
//
//	RwBool	Create(VOID);
//	VOID	Destroy(VOID);
//
//	RwInt32 SwitchDialog( bool bOpen );
//
//	//! Attibutes
//
//	//! Event
//	VOID	HandleEvents( RWS::CMsg& msg );
//
//private:
//	//! Implementation
//	VOID	SetBasicUISetting(VOID);
//	VOID	SetChallengeData( VOID* pData );
//	VOID	SetBoardObjectHandle( RwUInt32 hSerial );
//	VOID	SetWinCount( RwUInt16 wWinCount );
//
//	//! CallBack
//	VOID	OnClickCancel( gui::CComponent* pComponent );
//	VOID	OnClickChallenge( gui::CComponent* pComponent );
//	VOID	OnClickClose( gui::CComponent* pComponent );
//	VOID	OnCaptureMouseDown( const CKey& key );
//
//	gui::CSlot			m_slotClickCancel;
//	gui::CSlot			m_slotClickChallenge;
//	gui::CSlot			m_slotClickClose;
//	gui::CSlot			m_slotCaptureMouseDown;
//
//	//! Variables
//	gui::CStaticBox*	m_pstbDefenderName;
//	gui::CMDStaticBox*	m_pmdbDefenderBattleRecord;
//	gui::CStaticBox*	m_pstbChallengeText;
//	
//	gui::CButton*		m_pbtnClose;
//	gui::CButton*		m_pbtnChallenge;
//	gui::CButton*		m_pbtnCancel;
//
//	CRBChallengeItem	m_ChallengeItem[MAX_CHALLENGE_ITEM];	
//	
//	RwUInt32			m_hSerial;						// RankBattle Board Object Handle;
//	RwUInt32			m_uiRoomID;						
//};


#endif//__RB_CHALLENGE_GUI_H__