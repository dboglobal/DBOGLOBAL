#pragma once

#include "SlotGui.h"
#include "ceventhandler.h"
#include "Windowby3.h"

class CHLShopBuyGui : public CNtlPLGui, public RWS::CEventHandler
{

private:

	CWindowby3			m_srfBackground;

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMouseEnterItem;
	gui::CSlot			m_slotMouseLeaveItem;

	gui::CStaticBox*	m_pstbTitle;

	gui::CDialog*		m_pdlgEachBuy;
	gui::CPanel*		m_ppnlBlackSlot;
	CSurfaceGui			m_surBackSlot;
	gui::CStaticBox*	m_pstbItemName;
	gui::CStaticBox*	m_pstbItemInfo;

	gui::CPanel*		m_ppnlCashInfoBack;
	gui::CStaticBox*	m_pstbRequireCashTitle;
	gui::CPanel*		m_ppnlRequireCashBack;
	gui::CPanel*		m_ppnlCashMark0;
	gui::CStaticBox*	m_pstbRequireCash;
	gui::CStaticBox*	m_pstbHaveCashTitle;
	gui::CPanel*		m_ppnlHaveCashBack;
	gui::CPanel*		m_ppnlCashMark1;
	gui::CStaticBox*	m_pstbHaveCash;
	gui::CHtmlBox*		m_phtmlGuide;

	gui::CButton*		m_pbtnBuy;
	gui::CSlot			m_slotClickedBtnBuy;

	gui::CButton*		m_pbtnRecharge;
	gui::CSlot			m_slotClickedBtnRecharge;

	gui::CButton*		m_pbtnClose;
	gui::CSlot			m_slotClickedBtnClose;

	CRegularSlotGui		m_ItemSlot;

private:

	TBLIDX				m_hlsItemTblidx;

public:

	CHLShopBuyGui(const RwChar* pName);
	~CHLShopBuyGui();

	//! Operation
	void						Init();
	virtual RwBool				Create();
	virtual void				Destroy();

	RwInt32						SwitchDialog(bool bOpen);

	virtual void				HandleEvents(RWS::CMsg& msg);


private:

	void		OnPaint();
	void		OnMove(RwInt32 iOldX, RwInt32 iOldY);

	void		OnClickedBtnBuy(gui::CComponent* pComponent);
	void		OnClickedBtnRecharge(gui::CComponent* pComponent);
	void		OnClickedBtnClose(gui::CComponent* pComponent);
	void		OnMouseEnterItem(gui::CComponent* pComponent);
	void		OnMouseLeaveItem(gui::CComponent* pComponent);

private:

	void		ShowItemInfoWindow(bool bIsShow);

};