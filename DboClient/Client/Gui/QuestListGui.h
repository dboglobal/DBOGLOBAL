////////////////////////////////////////////////////////////////////////////////
// File: QuestListGui.h
// Desc: QuestList
//
// 2006.10.11 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_LIST_GUI_H__
#define __QUEST_LIST_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "GuiLineTree.h"
#include "NtlSLEvent.h"

class CQuestList;
class CQuestNode;
class CRegionNode;
class CQuestGui;
class CQuestInventoryGui;

struct stQUESTLISTDATA
{
	sTS_KEY				m_TSKey;			
	RwUInt32			m_uiSortType;
};

class CQuestListGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Constructor & Destructor
	CQuestListGui(VOID);
	CQuestListGui( const RwChar* pName );
	~CQuestListGui(VOID);

	//! Operation
	RwBool	Create( CQuestGui* pQuestGui );
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

	VOID	GetQuestList( std::list<stQUESTLISTDATA>* pList );
	VOID	CancelIndicator( sTS_KEY& sTSKey, RwBool bAuto );
	
	//! Event
	VOID HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID ClearIndicator(VOID);
	VOID RestoreIndicator(VOID);
	
	//! Callback
	gui::CSlot		m_slotClickExitButton;
	gui::CSlot		m_slotCaptureMouseDown;
	gui::CSlot		m_slotCaptureWheelMove;
	
	VOID	OnClickExitButton( gui::CComponent* pComponent );
	VOID	OnClickedBtnHelp( gui::CComponent* pComponent );
	VOID	OnCaptureMouseDown( const CKey& key );
	VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	
	//! Variables
	CQuestList*			m_pQuestList;
	gui::CButton*		m_pbtnClose;
	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbInventoryTitle;
	gui::CStaticBox*	m_pstbNoQuest;
	gui::CStaticBox*	m_pstbQuestTotalCount;
	
	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotClickedBtnHelp;

	CQuestGui*			m_pQuestGui;
	CQuestInventoryGui*	m_pQuestInventory;

	int					m_questCounter;
};

class CQuestList : public CGuiLineTree
{
public:
	CQuestList( CQuestGui* pQuestGui );
	~CQuestList(VOID);

	RwBool	Create( CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth );	
	VOID	RegistQuest( VOID* pData );
	VOID	UnRegistQuest( RwUInt16 tID );
	VOID	UpdateState( VOID* pData );
	
	VOID	ToggleIndicator( sTS_KEY& stTSKey, bool bToggle, RwBool bAuto = TRUE );
	
	typedef std::map<std::wstring,RwInt32>	MAP_REGIONID;
	
protected:
	//! Implementation
	RwInt32	RegionNameToID( std::wstring wstrRegion );

	//! Variables
	CQuestGui*		m_pQuestGui;
	MAP_REGIONID	m_mapRegionID;	
};

class CRegionNode : public CGuiLineTreeNode
{
public:
	CRegionNode( CGuiLineTree* pTree, RwInt32 nRegionID, std::wstring wstrRegionName );
	~CRegionNode(VOID);
	
	//! Override
	VOID	ShowProc(VOID);
	VOID	HideProc(VOID);
	VOID	Expand( RwBool bExpand );

protected:	
	//! Callback
	VOID	OnClickExpand( gui::CComponent* pComponent );
	VOID	OnClickReduce( gui::CComponent* pComponent );

	gui::CSlot			m_slotClickExpand;
	gui::CSlot			m_slotClickReduce;

	//! Variable	
	gui::CButton*		m_pbtnExpand;
	gui::CButton*		m_pbtnReduce;	
	gui::CStaticBox*	m_pstbTitle;
};

class CQuestNode : public CGuiLineTreeNode, public RWS::CEventHandler
{
public:
	CQuestNode( CGuiLineTree* pTree, sTS_KEY stTSKey, std::wstring wstrTitle, RwUInt32 uiType, RwUInt32 uiState, bool bShare, bool bIsNew );				
	~CQuestNode(VOID);

	//! Operation
	VOID	SetState( RwUInt32 uiState );
	
	VOID	HandleEvents( RWS::CMsg& msg );
	
	VOID	SetDownIndicatorBtn( bool bDown );
	VOID	SetEnableIndicatorBtn( bool bEnable );	
	VOID	SetReserveIndicator( RwBool bReserved ) { m_bReserveIndicator = bReserved; }
	
	RwBool  IsIndicatorBtnDown(VOID);
	RwBool  IsReservedIndicator(VOID) { return m_bReserveIndicator; }
	RwUInt32 GetQuestSortType(VOID) { return m_uiSortType; }
	sTS_KEY	GetQuestKey(VOID) { return m_TSKey; }
	
	//! Override
	VOID	ShowProc(VOID);
	VOID	HideProc(VOID);

protected:
	//! Implements
	RwBool	CanShowShareButton(VOID);
				
	//! Callback
	VOID	OnToggleIndicator( gui::CComponent* pComponent, bool bToggle );
	VOID	OnClickShare( gui::CComponent* pComponent );
	VOID	OnTitleClick( gui::CComponent* pComponent );
	VOID	OnPressedCommand( gui::CComponent* pComponent );
	VOID	OnReleasedCommand( gui::CComponent* pComponent );

	gui::CSlot			m_slotToggleIndicator;
	gui::CSlot			m_slotShareClick;
	gui::CSlot			m_slotTitleClick;
	gui::CSlot			m_slotTitlePressed;
	gui::CSlot			m_slotTitleReleased;

	//! Variable
	gui::CButton*		m_pbtnIndicator;		// Indicator 버튼. Toggle식
	gui::CButton*		m_pbtnShare;			// 공유 버튼.
	gui::CButton*		m_pbtnTitle;			// Title 버튼. 
	gui::CPanel*		m_ppnlState;			// Quest 상태
	
	sTS_KEY				m_TSKey;			
	RwUInt32			m_uiState;
	RwUInt32			m_uiSortType;
	std::wstring		m_wstrTitle;			// 원본 보관.

	RwUInt32			m_dwClickTime;			// 
	bool				m_bShare;
	RwBool				m_bIsNew;
	RwBool				m_bReserveIndicator;	// TMQ에서 돌아왔을때 다시 뜰때 Indicator표시 여부
};

#endif