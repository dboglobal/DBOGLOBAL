////////////////////////////////////////////////////////////////////////////////
// File: ItemChangeBattleAttributeGui.h
// Desc: ItemChangeBattleAttributeGui
//
// 2006.11.10 Peessi@hitel.net   
// 2018-8-10 daneos
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEM_CHANGE_BATTLE_ATTRIBUTE_GUI_H__
#define __ITEM_CHANGE_BATTLE_ATTRIBUTE_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"

class CNtlSobItemAttr;
class CNtlSobItem;
class CNtlSobNpc;
struct sITEM_TBLDAT;

struct stSLOTEFFECT
{
	enum eSLOTEFFECTSTATE { SLOT_EFFECT_NONE = 0, SLOT_EFFECT_ON, SLOT_EFFECT_OFF, SLOT_EFFECT_ON_COMPLETE };

	stSLOTEFFECT(VOID)
	{
		SetState( SLOT_EFFECT_NONE );
	}	

	VOID SetState( eSLOTEFFECTSTATE eState )
	{
		if( eState == SLOT_EFFECT_NONE )
			m_surSlotEffect.Show( FALSE );
		else
			m_surSlotEffect.Show( TRUE );
				
		m_fCurrentTime = 0.0f;
		m_eSlotEffect = eState;
	}

	VOID Update( RwReal fElapsed )
	{
		if( m_eSlotEffect == stSLOTEFFECT::SLOT_EFFECT_NONE ||
			m_eSlotEffect == stSLOTEFFECT::SLOT_EFFECT_ON_COMPLETE )
			return;

		m_fCurrentTime += fElapsed;		
		RwReal fBegin, fEnd;
		RwReal fTime = 1.0f;

		if( m_eSlotEffect == stSLOTEFFECT::SLOT_EFFECT_ON )
		{
			fBegin = 0; 
			fEnd = 255;			
		}
		else // if( m_aeSlotEffect[i] == SLOT_EFFECT_OFF )
		{
			fBegin = 255;
			fEnd = 0;
		}

		if( m_fCurrentTime < fTime )
		{
			m_surSlotEffect.SetAlpha( (RwUInt8)gui::GetResultLineInterpolation( m_fCurrentTime, fTime, fBegin, fEnd ) );			
		}
		else
		{
			m_surSlotEffect.SetAlpha( (RwUInt8)fEnd );

			if( m_eSlotEffect == stSLOTEFFECT::SLOT_EFFECT_OFF )
				SetState( SLOT_EFFECT_NONE );
			else
				SetState( SLOT_EFFECT_ON_COMPLETE );
		}		
	}

	CRectangle			m_rtEffectSlot;
	CSurfaceGui			m_surSlotEffect;
	eSLOTEFFECTSTATE	m_eSlotEffect;
	RwReal				m_fCurrentTime;
};

class CItemChangeBattleAttributeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	enum SLOT { SLOT_HONEST = 0, SLOT_STRANGE, SLOT_WILD, SLOT_ELEGANT, SLOT_FUNNY, NUM_SLOTEFFECT, SLOT_ITEM = NUM_SLOTEFFECT, NUM_SLOT };
	enum STATE { STATE_NONE, STATE_UPGRADE_READY, TATE_UPGRADE_CONFIRM, STATE_UPGRADE_PROCESS, STATE_PACKETWAIT, STATE_RESULT };
	enum eFOCUSEFFECT { SLOT_FOCUS_NONE = 0x00, SLOT_FOCUS_CAN_MOVE = 0x01, SLOT_FOCUS = 0x02 };	

	//! Constructor & Desturctor
	CItemChangeBattleAttributeGui(VOID);
	CItemChangeBattleAttributeGui( const RwChar* pName );
	~CItemChangeBattleAttributeGui(VOID);

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
		
	RwInt32 GetChildSlotIdx( RwInt32 nX, RwInt32 nY );

	bool	SetItemSlot( RwUInt32 hSerialID, RwUInt32 eSrcplace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiDestSlotIdx, RwBool bNeedToIconMoveEnd );
	VOID	UpdateSlotIcon(VOID);

	VOID	SetUpgradeItem( CNtlSobItem* pUpgradeItem );
	VOID	SetOfferItem( RwInt32 nSlotIdx, CNtlSobItem* pHoipoiStone );
	VOID	DeleteUpgradeItem(VOID);
	VOID	DeleteOfferItem(bool bReplace = false);

	VOID	SetResult(WORD wResultcode, RwUInt8 byBattleAttribute);
	VOID	SetState( STATE eState );
	
	VOID	ShowInfoWnd( RwBool bShow, RwInt32 nMouseOnIdx = -1 );
	void	SendItemChangeBattleAttributePacket();

	VOID	ShowIconDestination( RwBool isPick );
	VOID	DisableSlot( RwBool bDisable );
	VOID	ClickEffect( RwBool bPush, RwInt32 nSlotIdx = -1 );
	VOID	SetAttributeSlotEffect( RwInt32 nSlotIdx, RwBool bInSlot );
	
	RwBool	IsUpgradableItem( CNtlSobItemAttr* pItemAttr );
	RwBool	IsValidOfferItem( CNtlSobItemAttr* pItemAttr );

	RwBool	IsValidIconPickup( RwInt32 nClickIdx );
	RwBool	IsValidRightBtnProc( RwInt32 nClickIdx );
	
	RwBool  IsValidIdx( RwInt32 nIdx );
	RwBool	IsEmptySlot( RwInt32 nSlotIdx );
	RwBool	IsUpgradeItemSlot( RwInt32 nSlotIdx );
	RwBool	IsOfferItemSlot( RwInt32 nSlotIdx );

	BYTE	GetAttributeFromSlot();
	
	CNtlSobItem* GetSlotObject( RwInt32 nSlotIdx );
			
	//! Callback
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseUp( const CKey& key );
	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnMouseOut( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnClickExitBtn( gui::CComponent* pComponent );
	VOID	OnClickCompoundBtn( gui::CComponent* pComponent );
	VOID	OnClickCloseResult( gui::CComponent* pComponent );
	VOID	OnCaptureMouseDown( const CKey& key );
	VOID	OnCaptureMouseWheel( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID	OnChangeAttributeEffectEnd( gui::CComponent* pComponent );
	void	OnChangeAttributeResultEffectEnd(gui::CComponent* pComponent);
	
	gui::CSlot	m_slotMouseMove;
	gui::CSlot	m_slotMouseOut;
	gui::CSlot	m_slotMouseDown;
	gui::CSlot	m_slotMouseUp;
	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotClickExitBtn;
	gui::CSlot	m_slotClickCompoundBtn;
	gui::CSlot	m_slotClickCloseResult;
	gui::CSlot	m_slotCaptureMouseDown;
	gui::CSlot	m_slotCaptureMouseWheel;
	gui::CSlot	m_slotChangeAttributeEffectEnd;
	gui::CSlot	m_slotChangeAttributeEffectResultEnd;
	
	//! Variable
	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbInfoTitle;

	gui::CFlash*		m_pflsUpgradeEffect;
	gui::CHtmlBox*		m_phtmlInfoText;
	
	gui::CButton*		m_pbtnExit;
	gui::CButton*		m_pbtnCompound;
	gui::CButton*		m_pbtnQuickLink;

	gui::CDialog*		m_pdlgResult;
	gui::CFlash*		m_pflsResult;
	gui::CPanel*		m_ppnlUpgradeItemIcon;
	gui::CButton*		m_pbtnCloseResult;
	gui::CStaticBox*	m_pstbUpgradeItemName;
	gui::CStaticBox*	m_pstbResultInfo;
	
	CRectangle			m_rtSlot[NUM_SLOT];
	CSurfaceGui			m_surSlot[NUM_SLOT];
	CSurfaceGui			m_asurDisableSlot[NUM_SLOT];
		
	CNtlSobItem*		m_pUpgradeItem;
	CNtlSobItem*		m_pOfferItem;

	RwInt32				m_uiOfferItemParentSlotIdx;
	RwInt32				m_uiOfferItemSlotIdx;

	RwInt32				m_nOfferItemBattleSlotIdx;
	
	RwInt32				m_nLSelectedSlotIdx;
	RwInt32				m_nRSelectedSlotIdx;
	RwInt32				m_nMouseOnIdx;
	RwInt32				m_nPushDownIndex;
	
	CSurfaceGui			m_asurFocus[NUM_SLOT];
	RwInt32				m_anFocusEffect[NUM_SLOT];

	stSLOTEFFECT		m_aSlotEffect[NUM_SLOT];
	stSLOTEFFECT		m_TempItemSlot;
	stSLOTEFFECT		m_BackFocus;
		
	STATE				m_eState;

	RwReal				m_fCurrentTime;

	RwUInt32			m_hNPCSerial;
};

#endif//__ITEM_UPGRADE_GUI_H__