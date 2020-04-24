////////////////////////////////////////////////////////////////////////////////
// File: RBBoardGui.h
// Desc: Rank Battle 게시판 Gui
//
// 2007.07.20 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __RB_BOARD_GUI_H__
#define __RB_BOARD_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

class CRBBoardGui;
struct sRANKBATTLE_TBLDAT;
struct sRANKBATTLE_ARENA_INFO;

struct sRBBoardData
{
	UINT32			_uiFieldID;			// 데이터의 TableIndex;
	std::wstring	_wstrFieldName;		
	UINT32			_uiAppliedCount;
	UINT32			_infoIndex;		// tooltip
};

typedef std::vector<sRBBoardData>				VEC_RBBOARDDATA;
typedef std::vector<sRBBoardData>::iterator		VEC_RBBOARDDATA_ITER;

class CRBBoardItem
{
public:
//! Constructor and Destructor
	CRBBoardItem(VOID);
	~CRBBoardItem(VOID);

//! Operation
	RwBool	Create( CRBBoardGui* pBoardGui, gui::CComponent* pParent, gui::CSurfaceManager* pSurfaceManager, RwInt32 nItemID );
	VOID	Destroy(VOID);

	VOID	Show( bool bShow = true ) { m_pThis->Show( bShow ); } 
	VOID	Enable( bool bEnable = true ) { m_pThis->Enable( bEnable ); }
	VOID	Clear(VOID);

	VOID	SetItemData( RwUInt32 uiFieldID, const WCHAR* pName, RwUInt32 uiAppliedCount, const WCHAR* pInfo);

//! Attributes
	RwUInt32		GetFieldID(VOID) { return m_uiFieldID; }
	gui::CButton*	GetItemGui(VOID) { return m_pThis; }
	
protected:
//! Callback
	VOID	OnClickItem( gui::CComponent* pComponent );

	gui::CSlot		m_slotClickItem;
			
//! Variables	
	RwUInt32			m_uiFieldID;				
	RwInt32				m_nItemID;				// 위에서 부터 0 ~ 4, GUI No.

	gui::CStaticBox*	m_pTitle;
	gui::CPanel*		m_pGaugeBack;
	gui::CProgressBar*	m_pGauge;				

	gui::CButton*		m_pThis;
	CRBBoardGui*		m_pBoardGui;			
};

class CRBBoardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
//! Define
	enum { MAX_BOARD_ITEM = 5 };
	
//! Constructor and Destructor
	CRBBoardGui(VOID);
	CRBBoardGui( const RwChar* pName );
	~CRBBoardGui(VOID);

//! Operation
	VOID	Init(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );
	
	RwInt32 SwitchDialog( bool bOpen );
	
//! Attibutes

//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
//! Implementation
	VOID	SetBasicUISetting(VOID);
	VOID	SetBoardData( RwUInt8 byBattleType, RwUInt8 byArenaCount, sRANKBATTLE_ARENA_INFO* pArenaInfo );
	VOID	SetBoardItem( RwInt32 nOffset );
	VOID	SetInfoText(VOID);	
	
	VOID	StateApplySetting(VOID);	
	VOID	StateBattleTypeSetting( RwUInt8 byBattleType );
	RwInt32	GetFocusItemID(VOID);	
	RwUInt32 GetAppliedID( RwUInt8 byBattleType );

	VOID	EnableButtons( bool bEnable );
	VOID	ResetItem(VOID);

//! CallBack
public:
	VOID	OnClickBoardItem( RwInt32 nItemIndex, RwUInt32 uiFieldID );			

private:
	VOID	OnClickIndividual( gui::CComponent* pComponent );
	VOID	OnClickParty( gui::CComponent* pComponent );
	VOID	OnClickRelay(gui::CComponent* pComponent);

	VOID	OnClickChallenge( gui::CComponent* pComponent );
	VOID	OnClickCancel( gui::CComponent* pComponent );
	VOID	OnClickClose( gui::CComponent* pComponent );
	VOID	OnClickHelp(gui::CComponent* pComponent);

	VOID	OnScrollChanged( RwInt32 nNewOffset );

	VOID	OnCaptureMouseDown( const CKey& key );
	VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );

	gui::CSlot			m_slotClickIndividual;
	gui::CSlot			m_slotClickParty;
	gui::CSlot			m_slotClickRelay;

	gui::CSlot			m_slotClickChallenge;
	gui::CSlot			m_slotClickCancel;
	gui::CSlot			m_slotClickClose;
	gui::CSlot			m_slotClickHelp;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollSliderMoved;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotCaptureWheelMove;
	
//! Variables
	gui::CStaticBox*	m_pstbTitle;

	gui::CStaticBox*	m_pstbBattleApplyTitle;
	gui::CStaticBox*	m_pstbBattleFieldTitle;
	gui::CStaticBox*	m_pstbChallengerCount;
	gui::CStaticBox*	m_pstbInfo;

	gui::CStaticBox*	m_pstbRankBattleCount;
	gui::CPanel*		m_ppnlRankBattleType;

	gui::CButton*		m_pbtnIndividual;
	gui::CButton*		m_pbtnParty;

	gui::CButton*		m_pbtnRelay;
	gui::CButton*		m_pbtnRequestPartyMatching;
	gui::CButton*		m_pbtnDirectEnter;
	gui::CButton*		m_pbtnHelp;

	gui::CButton*		m_pbtnCancel;
	gui::CButton*		m_pbtnClose;

	gui::CPanel*		m_ppnlFocusBar;
	gui::CScrollBar*	m_pscbScroll;

	CRBBoardItem		m_BoardItem[MAX_BOARD_ITEM];	
	VEC_RBBOARDDATA		m_vecBoardData;

	RwUInt32			m_hSerial;						// RankBattle Board Object Handle;

	RwUInt8				m_byBattleType;					
	RwInt32				m_nOffset;			
	RwUInt32			m_uiSelectedItemFieldID;		// FieldID	
		
	RwReal				m_fCurrentTime;					// 리스트 업데이트 체크를 위한 시간. 
	RwReal				m_fMatchRemainTime;				// 게임이 시작
};

#endif//__RB_BOARD_GUI_H__