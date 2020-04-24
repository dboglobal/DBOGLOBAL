/*****************************************************************************
 *
 * File			: ItemOptionRestructureGui.h
 * Author		: Daneos
 * Copyright	: DBOG
 * Date			: 11/3/2018
 * Abstract		: 
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/

//#ifndef __ITEMOPTIONRESTRUCTURE_GUI_H__
//#define __ITEMOPTIONRESTRUCTURE_GUI_H__
#pragma once

#include "ceventhandler.h"
#include "SurfaceGui.h"


class CNtlSobItem;
class CNtlSobItemAttr;

class CItemOptionRestructureGui : public CNtlPLGui, public RWS::CEventHandler
{

#define ITEM_OPTION_MAX_COUNT 6

enum SLOT { SLOT_ITEM, SLOT_BOX, NUM_SLOT };
enum eFOCUSEFFECT { SLOT_FOCUS_NONE, SLOT_FOCUS_CAN_MOVE, SLOT_FOCUS };
enum STATE { STATE_NONE, STATE_READY, STATE_PROCESS, STATE_PACKETWAIT, STATE_RESULT };

public:
	CItemOptionRestructureGui(const RwChar *pName);
	~CItemOptionRestructureGui();

	void			Init();
	RwBool			Create();
	void			Destroy();

	RwInt32			SwitchDialog(bool bOpen);

protected:

	virtual void	HandleEvents(RWS::CMsg &pMsg);


protected:

	void				SetSlotRectHardCode();

	void				SetItemCurrentStats();
	void				EnableSelectOption(bool bFlag);

	bool				CreateStackNumber(RwInt32 nValue);
	void				DeleteStackNumber();

	void				SetResult(void* pOptionSet, BYTE byBoxCount);

	bool				SetItemSlot(RwUInt32 hSerialID, RwUInt32 eSrcplace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiDestSlotIdx, RwBool bNeedToIconMoveEnd);
	void				UpdateSlotIcon();

	void				SetItem(CNtlSobItem* pItem);
	void				SetBoxItem(RwInt32 nStackCount, CNtlSobItem* pItem);
	void				DeleteItem(bool bReplace = false);
	void				DeleteBoxItem(bool bReplace = false);

	void				CheckReady();
	void				SetState(STATE eState);

	void				ShowIconDestination(BOOL isPick);
	void				DisableSlot(RwBool bDisable);

	bool				IsItem(CNtlSobItemAttr* pItemAttr);
	bool				IsBoxItem(CNtlSobItemAttr* pItemAttr);
	bool				IsValidItem(CNtlSobItemAttr* pItemAttr, bool bSendError = false);
	bool				IsValidBoxItem(CNtlSobItemAttr* pItemAttr, bool bSendError = false);

	bool				IsValidIconPickup(RwInt32 nClickIdx);
	bool				IsValidRightBtnProc(RwInt32 nClickIdx);

	void				SendPacket();

protected:

	//!Callback

	void				OnPaint();
	void				OnMouseUp(const CKey& key);
	void				OnMove(RwInt32 nXPos, RwInt32 nYPos);

	void				OnItemIconMouseEnter(gui::CComponent* pComponent);
	void				OnItemIconMouseLeave(gui::CComponent* pComponent);
	void				OnItemIconMouseDown(const CKey& key);

	void				OnBoxItemIconMouseEnter(gui::CComponent* pComponent);
	void				OnBoxItemIconMouseLeave(gui::CComponent* pComponent);
	void				OnBoxItemIconMouseDown(const CKey& key);

	void				OnClickedBtnClose(gui::CComponent* pComponent);
	void				OnClickedBtnBuy(gui::CComponent* pComponent);
	void				OnClickedBtnRestructure(gui::CComponent* pComponent);
	void				OnClickedBtnCloseWide(gui::CComponent* pComponent);

	void				OnSelectedOption(INT nSelectIndex);

protected:

	//!Variables

	CNtlSobItem*		m_pItem;
	CNtlSobItem*		m_pBoxItem;

	CSurfaceGui			m_surSlot[NUM_SLOT];
	CSurfaceGui			m_asurDisableSlot[NUM_SLOT];
	CSurfaceGui			m_asurFocus[NUM_SLOT];
	RwInt32				m_anFocusEffect[NUM_SLOT];
	gui::CStaticBox*	m_pstbStackCount;
	RwInt32				m_nBoxStackNum;

	RwInt32				m_nLSelectedSlotIdx;
	RwInt32				m_nRSelectedSlotIdx;

	bool				m_bCanSelectOption; // is special equipment box?

	STATE				m_eState;

	WORD				m_wOptionIndex;


	gui::CButton*		m_pbtnClose;
	gui::CButton*		m_pbtnBuyItem;
	gui::CButton*		m_pbtnRestructure;
	gui::CButton*		m_pbtnCloseWide;

	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbTargetItemTitle;
	gui::CStaticBox*	m_pstbOptionKitTitle;
	gui::CStaticBox*	m_pstbTargetOptionTitle;
	gui::CStaticBox*	m_pstbRestructureOptionTitle;

	gui::CComboBox*		m_pcbbSelectOption;

	gui::CPanel*		m_ppnlOptionKitItemSlot;
	gui::CPanel*		m_ppnlTargetItemSlot;

	gui::CPanel*		m_ppnlArrowBlue;
	gui::CPanel*		m_ppnlArrowYellow;
	gui::CPanel*		m_ppnlArrowRed;
	gui::CPanel*		m_ppnlStarCircle;

	gui::CStaticBox*	m_pstbBeforeOptionName[ITEM_OPTION_MAX_COUNT];
	gui::CStaticBox*	m_pstbBeforeOptionValue[ITEM_OPTION_MAX_COUNT];
	gui::CStaticBox*	m_pstbAfterOptionName[ITEM_OPTION_MAX_COUNT];
	gui::CStaticBox*	m_pstbAfterOptionValue[ITEM_OPTION_MAX_COUNT];

	// !Slots
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot          m_slotCloseBtnClick;
	gui::CSlot          m_slotBuyItemBtnClick;
	gui::CSlot          m_slotRestructureBtnClick;
	gui::CSlot          m_slotCloseWideBtnClick;

	gui::CSlot          m_slotItemIconMouseEnter;
	gui::CSlot          m_slotItemIconMouseLeave;
	gui::CSlot          m_slotItemIconMouseDown;
	gui::CSlot          m_slotBoxItemIconMouseEnter;
	gui::CSlot          m_slotBoxItemIconMouseLeave;
	gui::CSlot          m_slotBoxItemIconMouseDown;

	gui::CSlot          m_slotSelectedOption;
};

//#endif