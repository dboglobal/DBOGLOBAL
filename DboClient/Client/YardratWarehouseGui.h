#pragma once

#include "SlotGui.h"
#include "ceventhandler.h"

#include "NtlHlsItem.h"


class CYardratWarehouseGui : public CNtlPLGui, public RWS::CEventHandler
{
	#define dYARDRAT_WAREHOUS_SLOTS_PER_PAGE 52
	#define dHLS_WAREHOUSE_TAB_COUNT 3

	struct sHLS_ITEM
	{
		sCASHITEM_BRIEF	sBrief;
		TBLIDX			itemTblidx;
	};

	enum eTAB_TYPE
	{
		eTAB_CONSUMABLE,
		eTAB_CRAFT,
		eTAB_EQUIPMENT,
	};

private:

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;

	gui::CStaticBox*	m_pstbTitle;

	gui::CStaticBox*	m_pstbCurrentPage;
	gui::CStaticBox*	m_pstbPageSlash;
	gui::CStaticBox*	m_pstbMaxPage;

	gui::CButton*		m_pbtnExit;
	gui::CSlot			m_slotClickedBtnExit;

	gui::CTabButton*	m_pTabButton;
	gui::CSlot			m_slotTab;

	gui::CButton*		m_pbtnPrev;
	gui::CSlot			m_slotClickedBtnPrev;

	gui::CButton*		m_pbtnNext;
	gui::CSlot			m_slotClickedBtnNext;

	CSurfaceGui			m_FocusEffect;
	bool				m_bFocus;

private:

	CRegularSlotGui		m_slot[dYARDRAT_WAREHOUS_SLOTS_PER_PAGE];

	std::map<QWORD, sHLS_ITEM*>	m_mapItems[dHLS_WAREHOUSE_TAB_COUNT]; // map because we can store "unlimited" items in each tab
	std::map<QWORD, int>		m_mapVisibleItems;

	int					m_nCurTab;
	int					m_nCurPage;
	int					m_nMaxPage;
	int					m_iMouseDownSlot;

public:

	CYardratWarehouseGui(const RwChar* pName);
	~CYardratWarehouseGui();

	//! Operation
	void						Init();
	virtual RwBool				Create();
	virtual void				Destroy();

	RwInt32						SwitchDialog(bool bOpen);

	virtual void				HandleEvents(RWS::CMsg& msg);

private:

	void		LoadItems(BYTE byCount, sCASHITEM_BRIEF * aInfo);

	void		AddItem(sCASHITEM_BRIEF* pBrief);
	void		DelItem(QWORD qwProductId);

	void		FocusEffect(bool bPush, int iSlotIdx = -1);
	int			PtinSlot(int iX, int iY);

	void		SetPage(int nPage);
	void		UpdateTabContent(int nIndex);

	void		RecreateSlots();

	void		ShowItemInfoWindow(bool bIsShow, int nSlot, sCASHITEM_BRIEF* pInfo);

	sHLS_ITEM*	GetItem(QWORD qwProductId);
	QWORD		GetProductId(int nSlot);

	int			GetTabType(BYTE byItemType);

private:

	void		OnPaint();
	void		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	void		OnMouseLeave(gui::CComponent* pComponent);
	void		OnMouseDown(const CKey& key);
	void		OnMouseUp(const CKey& key);

	void		OnSelectChangeTabButton(INT nCurIndex, INT nPrevIndex);


	void		OnClickedBtnExit(gui::CComponent* pComponent);
	void		OnClickedBtnPrev(gui::CComponent* pComponent);
	void		OnClickedBtnNext(gui::CComponent* pComponent);


};