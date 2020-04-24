////////////////////////////////////////////////////////////////////////////////
// File: ItemUpgradeGui.h
// Desc: ItemUpgradeGui
//
// 2006.11.10 Peessi@hitel.net   
// 10/8/2018 - DANEOS
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEM_UPGRADE_GUI_H__
#define __ITEM_UPGRADE_GUI_H__

#include "ceventhandler.h"
#include "SurfaceGui.h"


class CNtlSobItemAttr;
class CNtlSobItem;
class CNtlSobNpc;
struct sITEM_TBLDAT;

class CItemUpgradeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	enum SLOT { SLOT_STONE, SLOT_PROTECTION, SLOT_ITEM, NUM_SLOT };
	enum STATE { STATE_NONE, STATE_UPGRADE_READYFAIL_NOMOREUPGRADE, STATE_UPGRADE_READYFAIL_NOMORESTONES,
				 STATE_UPGRADE_READY, STATE_UPGRADE_PROCESS, STATE_PACKETWAIT, STATE_RESULT };
	enum eFOCUSEFFECT { SLOT_FOCUS_NONE = 0x00, SLOT_FOCUS_CAN_MOVE = 0x01, SLOT_FOCUS = 0x02 };	

	//! Constructor & Desturctor
	CItemUpgradeGui(VOID);
	CItemUpgradeGui( const RwChar* pName );
	~CItemUpgradeGui(VOID);

	//! Operation
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	
	VOID	DestroyResultDialog(VOID);

	VOID	Update( RwReal fElapsed );
	
	RwInt32	SwitchDialog( bool bOpen );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	SetSlotRectHardCode(VOID);
	VOID	SetBasicUISetting(VOID);
		
	RwBool	CreateStackNumber(RwInt32 nSlotIdx, RwInt32 nValue);
	VOID	DeleteStackNumber(RwInt32 nSlotIdx);

	VOID	SetItemSlot( RwUInt32 hSerialID, RwUInt32 eSrcplace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiDestSlotIdx, RwBool bNeedToIconMoveEnd );
	VOID	UpdateSlotIcon(VOID);

	VOID	SetUpgradeItem( CNtlSobItem* pUpgradeItem );
	VOID	SetHoipoiStone(RwInt32 nStackCount, CNtlSobItem* pHoipoiStone);
	VOID	SetGuardStone(RwInt32 nStackCount, CNtlSobItem* pGuardStone);
	VOID	DeleteUpgradeItem();
	VOID	DeleteHoipoiStone(bool bReplace = false);
	VOID	DeleteGuardStone(bool bReplace = false);

	VOID	SetResult(WORD wResultcode, BYTE byGrade, WORD wResultMessageCode, BYTE byStoneCount, BYTE byGuardStoneCount);
	VOID	SetState( STATE eState );
	
	VOID	SendUpgradePacket(VOID);

	VOID	ShowIconDestination( RwBool isPick );
	VOID	DisableSlot(RwBool bDisable);
	
	RwBool	IsUpgradableItem( CNtlSobItemAttr* pItemAttr );
	RwBool	IsHoipoiStone( CNtlSobItemAttr* pItemAttr );
	RwBool	IsGuardStone(CNtlSobItemAttr* pItemAttr);
	RwBool	IsValidUpgradeItem(CNtlSobItemAttr* pItemAttr, bool bSendError = false);
	RwBool	IsValidHoipoiStone(CNtlSobItemAttr* pItemAttr, bool bSendError = false);
	RwBool	IsValidProtectionStone(CNtlSobItemAttr* pItemAttr, bool bSendError = false);

	RwBool	IsValidIconPickup( RwInt32 nClickIdx );
	RwBool	IsValidRightBtnProc( RwInt32 nClickIdx );
	
	RwBool	IsEmptySlot( RwInt32 nSlotIdx );
	RwBool	IsUpgradeItemSlot( RwInt32 nSlotIdx );
	RwBool	IsHoipoiStoneSlot( RwInt32 nSlotIdx );
	RwBool	IsGuardStoneSlot(RwInt32 nSlotIdx);
	
	CNtlSobItem* GetSlotObject( RwInt32 nSlotIdx );
			
	//! Callback
	VOID	OnMouseUp( const CKey& key );

	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnClickExitBtn( gui::CComponent* pComponent );
	VOID	OnClickCompoundBtn( gui::CComponent* pComponent );
	VOID	OnClickCloseResult( gui::CComponent* pComponent );
	VOID	OnCaptureMouseDown( const CKey& key );
	VOID	OnCaptureMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);

	VOID	OnUpgradeItemIconMouseEnter( gui::CComponent* pComponent );
	VOID	OnUpgradeItemIconMouseLeave( gui::CComponent* pComponent );
	VOID	OnUpgradeItemIconMouseDown(const CKey& key);

	VOID	OnGuardItemIconMouseEnter(gui::CComponent* pComponent);
	VOID	OnGuardItemIconMouseLeave(gui::CComponent* pComponent);
	VOID	OnGuardItemIconMouseDown(const CKey& key);

	VOID	OnStoneItemIconMouseEnter(gui::CComponent* pComponent);
	VOID	OnStoneItemIconMouseLeave(gui::CComponent* pComponent);
	VOID	OnStoneItemIconMouseDown(const CKey& key);

	VOID	OnUpgradeResultEffectEnd( gui::CComponent* pComponent );

	
	gui::CSlot	m_slotMouseUp;
	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotClickExitBtn;
	gui::CSlot	m_slotClickCompoundBtn;

	gui::CSlot	m_slotCaptureMouseDown;
	gui::CSlot	m_slotCaptureMouseWheel;

	gui::CSlot	m_slotUpgradeItemIconMouseEnter;
	gui::CSlot	m_slotUpgradeItemIconMouseLeave;
	gui::CSlot	m_slotUpgradeItemIconMouseDown;

	gui::CSlot	m_slotGuardItemIconMouseEnter;
	gui::CSlot	m_slotGuardItemIconMouseLeave;
	gui::CSlot	m_slotGuardItemIconMouseDown;

	gui::CSlot	m_slotStoneItemIconMouseEnter;
	gui::CSlot	m_slotStoneItemIconMouseLeave;
	gui::CSlot	m_slotStoneItemIconMouseDown;

	gui::CSlot	m_slotUpgradeEffectResultEnd;
	gui::CSlot	m_slotClickCloseResult;
	
	//! Variable
	gui::CStaticBox*	m_pstbInfoTitle;
	
	gui::CButton*		m_pbtnExit;
	gui::CButton*		m_pbtnSelect;

	gui::CHtmlBox*		m_phtmlInfoText;

	gui::CPanel*		m_ppnlUpgradedItemSlot;
	gui::CPanel*		m_ppnlUpgradedGuardStoneSlot;
	gui::CPanel*		m_ppnlUpgradedStoneSlot;

	gui::CDialog*		m_pdlgResult;
	gui::CFlash*		m_pflsResult;
	gui::CButton*		m_pbtnCloseResult;

	CSurfaceGui			m_surSlot[NUM_SLOT];
	CSurfaceGui			m_asurDisableSlot[NUM_SLOT];
		
	CNtlSobItem*		m_pUpgradeItem;
	CNtlSobItem*		m_pStoneItem;
	CNtlSobItem*		m_pGuardItem;

	RwInt32				m_nStoneStackNum;
	RwInt32				m_nGuardStoneStackNum;
	
	RwInt32				m_nLSelectedSlotIdx;
	RwInt32				m_nRSelectedSlotIdx;
	
	CSurfaceGui			m_asurFocus[NUM_SLOT];
	RwInt32				m_anFocusEffect[NUM_SLOT];

	gui::CStaticBox*	m_pstbStackCount[NUM_SLOT - 1];
		
	STATE				m_eState;

	RwReal				m_fCurrentTime;

	RwUInt32			m_hNPCSerial;
};

#endif//__ITEM_UPGRADE_GUI_H__