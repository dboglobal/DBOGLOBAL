#pragma once

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "NtlSLDef.h"

#include "SlotGui.h"

class CItemBindSealingGui : public CNtlPLGui, public RWS::CEventHandler
{

protected:

	// MAIN
	gui::CStaticBox*	m_pStbTitle;
	gui::CButton*		m_pBtnClose;
	gui::CButton*		m_pBtnSeal;

	gui::CFlash*		m_pFlash;
	gui::CSlot			m_pSlotSealSuccessEffectEnd;

	gui::CStaticBox*	m_stbItemTitleText;
	gui::CStaticBox*	m_stbItemDescText;
	gui::CStaticBox*	m_stbCardTitleText;
	gui::CStaticBox*	m_stbCardDescText;
	gui::CStaticBox*	m_stbMatchNotice;
	gui::CStaticBox*	m_stbNeedCoinText;
	gui::CStaticBox*	m_stbCardCoinText;

	gui::CPanel*		m_pnlItemSlot;
	gui::CPanel*		m_pnlCardSlot;
	gui::CPanel*		m_pnlItemSealedSlot;

	gui::CSlot			m_slotClickedBtnClose;
	gui::CSlot			m_slotClickedBtnSeal;

	gui::CSurface		m_srfItemUpgradeItemEffect;
	gui::CSurface		m_srfItemUpgradeStoneEffect;
	gui::CSurface		m_srfItemUpgradeMouseMove;

	CRegularSlotGui		m_AttachItemSlot;
	CRegularSlotGui		m_AttachSealItemSlot;
	CRegularSlotGui		m_SealedItemSlot;

	gui::CSlot			m_slotMovePnlAttachItem;
	gui::CSlot			m_slotPaintPnlAttachItem;
	gui::CSlot			m_slotMouseDownPnlAttachItem;
	gui::CSlot			m_slotMouseUpPnlAttachItem;
	gui::CSlot			m_slotMouseEnterPnlAttachItem;
	gui::CSlot			m_slotMouseLeavePnlAttachItem;


	gui::CSlot			m_slotMovePnlAttachSealCoinItem;
	gui::CSlot			m_slotPaintPnlAttachSealCoinItem;
	gui::CSlot			m_slotMouseDownPnlAttachSealCoinItem;
	gui::CSlot			m_slotMouseUpPnlAttachSealCoinItem;
	gui::CSlot			m_slotMouseEnterPnlAttachSealCoinItem;
	gui::CSlot			m_slotMouseLeavePnlAttachSealCoinItem;


	gui::CSlot			m_slotMovePnlSealedItem;
	gui::CSlot			m_slotPaintPnlSealedItem;
	gui::CSlot			m_slotMouseUpPnlSealedItem;
	gui::CSlot			m_slotMouseEnterPnlSealedItem;
	gui::CSlot			m_slotMouseLeavePnlSealedItem;

public:

	CItemBindSealingGui(const RwChar* pName);
	virtual ~CItemBindSealingGui();

	RwBool		Create();
	void		Destroy();

	void		Update(RwReal fElapsed);

	void		HandleEvents(RWS::CMsg& msg);
	RwInt32		SwitchDialog(bool bOpen);

	void		OnClickedBtnClose(gui::CComponent* pComponent);
	void		OnClickedBtnSeal(gui::CComponent* pComponent);

	void		OnPaintPnlAttachItem();
	void		OnMovePnlAttachItem(RwInt32 iOldX, RwInt32 iOldY);
	void		OnMouseDownPnlAttachItem(const CKey& key);
	void		OnMouseUpPnlAttachItem(const CKey& key);
	void		OnMouseEnterPnlAttachItem(gui::CComponent* pComponent);
	void		OnMouseLeavePnlAttachItem(gui::CComponent* pComponent);


	void		OnPaintPnlAttachSealCoinItem();
	void		OnMovePnlAttachSealCoinItem(RwInt32 iOldX, RwInt32 iOldY);
	void		OnMouseDownPnlAttachSealCoinItem(const CKey& key);
	void		OnMouseUpPnlAttachSealCoinItem(const CKey& key);
	void		OnMouseEnterPnlAttachSealCoinItem(gui::CComponent* pComponent);
	void		OnMouseLeavePnlAttachSealCoinItem(gui::CComponent* pComponent);


	void		OnPaintPnlSealedItem();
	void		OnMovePnlSealedItem(RwInt32 iOldX, RwInt32 iOldY);
	void		OnMouseUpPnlSealedItem(const CKey& key);
	void		OnMouseEnterPnlSealedItem(gui::CComponent* pComponent);
	void		OnMouseLeavePnlSealedItem(gui::CComponent* pComponent);

private:

	void		SetItem(SERIAL_HANDLE hItem, RwUInt8 uiPlace = 0, RwUInt8 uiPos = 0);
	void		SetSealItem(SERIAL_HANDLE hItem, RwUInt8 uiPlace = 0, RwUInt8 uiPos = 0, RwUInt8 uiStackCount = 0);
	void		SetSealedItem(SERIAL_HANDLE hItem);

	void		UnsetItems();

	void		UpdateSealCoinsAfterSealing();

	void		CheckReady();

	void		ShowItemInfoWindow(RwBool bIsShow);
	void		ShowSealItemInfoWindow(RwBool bIsShow);
	void		ShowSealedItemInfoWindow(RwBool bIsShow);

	void		SetCoinCount(BYTE byVal);
	void		SetNeedCoinCount(BYTE byVal);

	void		OnSealSuccessEffectEnd(gui::CComponent* pComponent);

private:

	bool		m_bIsSealReady;
	BYTE		m_byRequiredCoin;
	BYTE		m_byHasCoin;

	sITEM_POSITION_DATA	m_sItem;
	sITEM_POSITION_DATA	m_sSealCoinItem;
};
