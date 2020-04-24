////////////////////////////////////////////////////////////////////////////////
// File: QuestInvetoryGui.h
// Desc: QuestInvetory
//
// 2006.11.7 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUEST_INVENTORY_GUI_H__
#define __QUEST_INVENTORY_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"

#define QUESTINVENTORY_MAXCOL	15
#define QUESTINVENTORY_MAXROW	2

class CQuestListGui;
class CNtlSobQuestItem;
class CNtlSobAvatar;

class CQuestInventoryGui
{
public:
	//! Constructor & Destructor
	CQuestInventoryGui(VOID);
	~CQuestInventoryGui(VOID);

	//! Operation
	VOID	Init(VOID);
	RwBool	Create( CQuestListGui* pQuestListGui );
	VOID	Destroy(VOID);

	VOID	UpdateData(VOID);
	
	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	ClearInventory(VOID);
	VOID	ClearInventorySlot( RwInt32 nSlotIdx, RwInt32 nRow );

	VOID	SetSlotRectHardCode(VOID);
	VOID	ClickEffect( RwBool bPush, RwInt32 nSlotIdx = -1 );
	VOID	ShowPickedUp( RwBool bShow, RwInt32 nSlotIdx = -1 );
	VOID	ShowFocus( RwBool bShow, RwInt32 nSlotIdx = -1 );
	VOID	ShowDisable( RwBool bShow, RwInt32 nSlotIdx );

	RwBool	CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue );
	VOID	DeleteStackNumber( RwInt32 nSlotIdx );
	RwInt32	GetChildSlotIdx( RwInt32 nX, RwInt32 nY );

	RwBool	IsEnablePickUp( RwInt32 nSlotIdx );
	RwBool	IsEnablePutDown( RwInt32 nSlotIdx );

	VOID	UpdateInvenInfo(VOID);

	CNtlSobQuestItem*	GetQuestItemFromIdx( RwInt32 nIdx ); 
	RwUInt32			GetQuestItemSerialFromIdx( RwInt32 nIdx );
	RwInt32				GetQuestItemIdxFromIdx( RwInt32 nIdx );
	RwInt32				GetCurrentPageFromIdx( RwInt32 nIdx );

	//! CallBack
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseUp( const CKey& key );
	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnMouseOut( gui::CComponent* pComonent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnNextInvenBtnClick( gui::CComponent* pComponent );
	VOID	OnPrevInvenBtnClick( gui::CComponent* pComponent );
	VOID	OnClickDiscardButton( gui::CComponent* pComponent );
	
	gui::CSlot	m_slotMouseDown;
	gui::CSlot	m_slotMouseUp;
	gui::CSlot	m_slotMouseMove;
	gui::CSlot	m_slotMouseOut;
	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotClickNextInven;
	gui::CSlot	m_slotClickPrevInven;
	gui::CSlot	m_slotClickDiscardButton;
	
	gui::CDialog*		m_pBack;			// 아이콘 선택영역.
	gui::CButton*		m_pbtnDiscard;
	gui::CButton*		m_pNextInvenBtn;
	gui::CButton*		m_pPrevInvenBtn;
	gui::CStaticBox*	m_pInvenInfo;
	
	CRectangle		m_rtSlot[QUESTINVENTORY_MAXCOL];
	CSurfaceGui		m_surSlot[QUESTINVENTORY_MAXCOL];
	gui::CStaticBox* m_pStackNum[QUESTINVENTORY_MAXCOL];
	CSurfaceGui		m_surDisable[QUESTINVENTORY_MAXCOL][QUESTINVENTORY_MAXROW];

	RwInt32		m_nLSelectedSlotIdx;
	RwInt32		m_nRSelectedSlotIdx;
	RwInt32		m_nMouseOnIdx;
	RwInt32		m_nPushDownIdx;

	RwInt32		m_nInvenPage;

	CSurfaceGui	m_surFocusSlot;
	RwInt32		m_nShowFocus;

	CSurfaceGui m_surPickedUp;
	RwInt32		m_nShowPickedUp;

	CQuestListGui*  m_pQuestListGui;
};

#endif//__QUEST_INVENTORY_GUI_H__
