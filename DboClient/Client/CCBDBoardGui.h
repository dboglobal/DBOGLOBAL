#pragma once

#include "SlotGui.h"
#include "ceventhandler.h"

class CCCBDBoardGui : public CNtlPLGui, public RWS::CEventHandler
{

private:

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;

	gui::CStaticBox*	m_psttTitle;

	gui::CStaticBox*	m_pstbCCBDSetup;

	gui::CHtmlBox*		m_phbxExplain;

	CSurfaceGui			m_surNeedItem;
	gui::CPanel*		m_ppnlAttachItem;
	gui::CSlot			m_slotMouseEnterItem;
	gui::CSlot			m_slotMouseLeaveItem;
	gui::CSlot			m_slotMouseDownAttachItem;
	gui::CSlot			m_slotMouseUpAttachItem;

	gui::CPanel*		m_ppnlBar;

	gui::CStaticBox*	m_pstbBarText;

	gui::CButton*		m_pbtnPartyMatching;
	gui::CSlot			m_slotClickedBtnPartyMatching;

	gui::CButton*		m_pbtnEnter;
	gui::CSlot			m_slotClickedBtnEnter;

	gui::CButton*		m_pbtnCancel;
	gui::CSlot			m_slotClickedBtnCancel;


	CSurfaceGui			m_EffectFocus;

	CRegularSlotGui		m_ItemSlot;

private:

	HOBJECT				m_hNPCSerialID;
	bool				m_bItemSlotFocus;
	sITEM_POSITION_DATA	m_sItem;

public:

	CCCBDBoardGui(const RwChar* pName);
	~CCCBDBoardGui();

	//! Operation
	void						Init();
	virtual RwBool				Create();
	virtual void				Destroy();

	RwInt32						SwitchDialog(bool bOpen);

	virtual void				HandleEvents(RWS::CMsg& msg);

private:

	void		OnPaint();
	void		OnMove(RwInt32 iOldX, RwInt32 iOldY);

	void		OnClickedBtnPartyMatching(gui::CComponent* pComponent);
	void		OnClickedBtnEnter(gui::CComponent* pComponent);
	void		OnClickedBtnCancel(gui::CComponent* pComponent);

	void		OnMouseEnterItem(gui::CComponent* pComponent);
	void		OnMouseLeaveItem(gui::CComponent* pComponent);
	void		OnMouseDownAttachItem(const CKey& key);
	void		OnMouseUpAttachItem(const CKey& key);

private:

	void		SetBarText(int nLevel);
	void		SetItem(HOBJECT hItem, BYTE byPlace, BYTE byPos);

private:

	void		ShowItemInfoWindow(bool bIsShow);
};