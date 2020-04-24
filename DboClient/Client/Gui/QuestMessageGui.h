////////////////////////////////////////////////////////////////////////////////
// File: QuestMessageGui.h
// Desc: QuestMessage
//
// 2007.11.7 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_MESSAGE_GUI_H__
#define __QUEST_MESSAGE_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"
#include "DboTSCoreDefine.h"

class CQuestGui;

struct stMESSAGEBUTTONITEM
{
	//static RwBool		m_bSyncState;
	//static RwReal		m_fSyncTime;
	gui::CButton*		m_pBtn;
	//gui::CFlickerEffect	m_Effect;

	stMESSAGEBUTTONITEM::stMESSAGEBUTTONITEM(VOID) : m_pBtn( NULL ) {}

	//VOID Update( RwReal fElapsedTime );
	VOID Enable( RwBool bEnable );		
	//RwReal GetPulseTime(VOID) { return m_Effect.GetCurrentPulseTime(); }
	//RwBool IsEnable(VOID) { return m_Effect.IsWork(); }
};

class CQuestMessageGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	enum { NEXT = 0, PREV, BTNCOUNT };
	
	//! Constructor & Destructor
	CQuestMessageGui( const RwChar* pName );
	~CQuestMessageGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	RwInt32 SwitchDialog( bool bOpen );

	void	LastPageProc();

	//! Event
	VOID HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	SetPageInfo(VOID);
	VOID	InitVisitData(VOID);	
	
	//! Callback
	VOID	OnMove( RwInt32 nX, RwInt32 nY );
	VOID	OnPaint(VOID);
	
	VOID	OnClickNextButton( gui::CComponent* pComponent );
	VOID	OnClickPrevButton( gui::CComponent* pComponent );
	VOID	OnClickCancelButton( gui::CComponent* pComponent );
	VOID	OnClickSkipButton(gui::CComponent* pComponent);
	VOID	OnHtmlPageDone( RwInt32 nPage );
	
	VOID	OnCaptureMouseDown( const CKey& key );

	gui::CSlot		m_slotMove;
	gui::CSlot		m_slotPaint;

	gui::CSlot		m_slotClickNextButton;
	gui::CSlot		m_slotClickPrevButton;
	gui::CSlot		m_slotClickCancelButton;
	gui::CSlot		m_slotClickSkipButton;
	gui::CSlot		m_slotHtmlPageDone;
	
	gui::CSlot		m_slotCaptureMouseDown;

	//! Variables
	gui::CHtmlBox*		m_phbxMessage;
	gui::CButton*		m_pbtnCancel;
	gui::CButton*		m_pbtnSkip;
	gui::CStaticBox*	m_pstbPage;
	gui::CStaticBox*	m_pstbQuestTitle;
	stMESSAGEBUTTONITEM m_MessageButton[BTNCOUNT];

	CQuestGui*			m_pQuestGui;

	RwBool				m_bSendFailTS;

	RwInt32				m_nReadPage;
	RwUInt8				m_eType;		// eQuestMessageType	

	// Visit 퀘스트 관련 데이터
	sQUEST_INFO			m_QuestInfo;
	RwUInt32			m_uiEventGenType;
	RwUInt32			m_uiID;
};

#endif