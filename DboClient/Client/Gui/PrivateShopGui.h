/*****************************************************************************
* File			: PrivateShopGui.h
* Author		: Jinsung, Kim -> Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2007. 7. 4 -> 2007. 12. 19
* Abstract		: DBO 개인상점
*****************************************************************************
* Desc         : 
*****************************************************************************/

#ifndef __PRIVATE_SHOP_GUI_H__
#define __PRIVATE_SHOP_GUI_H__

#pragma once

#include "Windowby3.h"
#include "SlotGui.h"
#include "NtlItem.h"

// 개인상점 아이템의 위치
#define dPRIVATESHOP_UI_ITEM_TOP				42
#define dPRIVATESHOP_UI_ITEM_LEFT				20
#define dPRIVATESHOP_UI_ITEM_GAP				55

#define dPRIVATESHOP_UI_BGLINE_LEFT				9
#define dPRIVATESHOP_UI_BGLINE_TOP				30
#define dPRIVATESHOP_UI_SHOP_BGLINE_WIDTH		303
#define dPRIVATESHOP_UI_SHOP_BGLINE_HEIGHT		347
#define dPRIVATESHOP_UI_NOTE_BGLINE_WIDTH		303
#define dPRIVATESHOP_UI_NOTE_BGLINE_HEIGHT		375
#define dPRIVATESHOP_UI_BUY_AWAY_LEFT			30
#define dPRIVATESHOP_UI_SELL_AWAY_LEFT			115

#define dPRIVATESHOP_UI_ITEM_HEIGHT				DBOGUI_ICON_SIZE
#define dPRIVATESHOP_UI_ITEM_WIDTH				DBOGUI_ICON_SIZE

#define dPRIVATESHOP_UI_CALC_OFFSET				104


class CPrivateShopItemGui;
class CPrivateShopGui;

/**
* \ingroup client
* \brief 개인상점 아이템 슬롯
*/
class CPrivateShopItemGui
{
public:
	CPrivateShopItemGui();
	virtual ~CPrivateShopItemGui();

	// Create & Destroy
	RwBool			Create(const CRectangle& rectArea, CSurfaceGui* pEffectSelect, CSurfaceGui* pEffectFocus, gui::CComponent* pParent, CPrivateShopGui* pPrivateShopGui);
	void			Destroy();

	// Callback
	void			OnPaint();	
	void			OnBgPaint();
	void			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void			OnMouseDown(const CKey& key);
	void			OnMouseUp(const CKey& key);
	void			OnMouseLeave(gui::CComponent* pComponent);
	void			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	void			OnClickedBtnPrice(gui::CComponent* pComponent);

	// Interface
	void			Clear();
	void			Show(bool bShow = true)					{ m_pShopItem->Show(bShow); }

	RwUInt8			GetPos()								{ return m_uiPos; }
	void			SetPos(RwUInt8 uiPos)					{ m_uiPos = uiPos; }

	CRegularSlotGui*		GetIcon()								{ return m_pShopItemIcon; }
	void			SetIcon(RwUInt8 uiPos, RwUInt32 uiSerial, 
							RwUInt32 uiCount, RwUInt32 uiPrice, 
							RwUInt8 uiItemState, eREGULAR_SLOT_TYPE eSerialType);

	SERIAL_HANDLE	GetSerial()								{ return m_pShopItemIcon->GetSerial(); }
	RwUInt32		GetCount()								{ return m_pShopItemIcon->GetCount(); }
	RwUInt32		GetPrice()								{ return m_pShopItemIcon->GetPrice(); }
	sITEM_TBLDAT*	GetItemTable()							{ return m_pShopItemIcon->GetItemTable(); }

	void			SetItemState(RwUInt8 uiItemState)		{ m_uiItemState = uiItemState; }
	RwUInt8			GetItemState()							{ return m_uiItemState; }

	CRectangle		GetShopItemRect();
	void			EnablePrice(bool bEnable = true)				{ m_pShopItemPrice->ClickEnable(bEnable); }


protected:
	// Components
	gui::CDialog*		m_pShopItem;				///< Item의 Dialog
	gui::CPanel*		m_pShopItemBg;				///< Item Slot의 배경
	gui::CStaticBox*	m_pShopItemName;			///< Item 이름
	gui::CButton*		m_pShopItemPrice;			///< Item 가격
	CRegularSlotGui*			m_pShopItemIcon;			///< Item Icon의 Slot

	// Signals
	gui::CSlot			m_slotMove;	
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotBgPaint;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotClickedBtnPrice;			

	// Effects
	CSurfaceGui			m_EffectPick;				///< 픽된 상태의 그림
	CSurfaceGui*		m_pEffectSelect;			///< 선택된 상태의 그림
	CSurfaceGui*		m_pEffectFocus;				///< 포커스 상태의 그림

	CPrivateShopGui*	m_pPrivateShopGui;			///< 이 슬롯을 가지는 부모의 Class
	RwBool				m_bClick;					///< 클릭되었나?
	
	RwUInt8				m_uiItemState;				///< 아이템 상태
	CRectangle			m_rectItem;
	RwUInt8				m_uiPos;
};

/**
* \ingroup client
* \brief 개인상점 GUI
*/
class CPrivateShopGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	
	///< 개인상점 UI의 Tab Type
	enum eTAB_TYPE
	{
		TAB_SHOP,	///< 상점
		TAB_NOTE,	///< 공지
		TAB_NUM,	///< TAB의 갯수
	};

	///< 개인상점 UI의 Item Slot
	enum eITEM_SLOT
	{
		SLOT_NUM = 6,	///< 슬롯 갯수
	};

	///< 개인상점 UI의 Type
	enum ePrivateShopType
	{
		PRIVATESHOP_BUY,	///< 손님
		PRIVATESHOP_SELL,	///< 주인
		PRIVATESHOP_NONE,
	};

	// 생성자 & 소멸자
	CPrivateShopGui(const RwChar* pName);
	virtual ~CPrivateShopGui();

	// Create & Destroy
	RwBool	Create();
	RwBool	CreateTabShop(gui::CDialog* pParent);
	RwBool	CreateTabNote(gui::CDialog* pParent);
	void	Destroy();

	// SwitchDialog & HandleEvents
	RwInt32	SwitchDialog(bool bOpen);
	void	HandleEvents(RWS::CMsg& msg);								///< 이벤트
	void	HandleMsgboxResult(void* pData);							///< 메시지 박스 결과

	void	HandleCalcPopupResult(void* pData);							///< 계산기의 입력 결과
	void	HandleSobPrivateShopCommunity(void *pData);					///< 상점의 상태가 변경되었을 경우
	void	HandleRegPrivateShopItem(void* pData);						///< 개인상점 Item Reg
	void	HandleSobInfoUpdate(void* pData);							///< Sob 객채의 정보 Update
	void	HandlePickedUpHide(void* pData);							///< 개인상점 아이템이 픽업 되어서 Hide 되었을 때 

	// HandleEvents : 개인상점 관련
	void	HandlePrivateShopState(void* pData);						///< 개인상점 State
	void	HandlePrivateShopStateVisitor(void *pData);					///< 방문자가 들어왔을 때
	void	HandlePrivateShopItem(void* pData);							///< 개인상점 Item
	void	HandlePrivateShopItemDataInfo(void* pData);					///< 개인상점 Item Update
	void	HandlePrivateShopItemSelect(void* pData);					///< 개인상점 Item Select
	void	HandlePrivateShopSelectNfy(void* pData);
	void	HandlePrivateShopItemBuying(void* pData);					///< 개인상점 Item Buying
	void	HandlePrivateShopItemState(void* pData);					///< 개인상점 Item State
	void	HandlePrivateShopNameTag(void* pData);						///< 개인상점 NameTag Update

	// Event
	void	OnPaintEffect();
	void	OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void	OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex);
	void	OnTabShopMouseLeave(gui::CComponent* pComponent);
	void	OnClickedBtnClose(gui::CComponent* pComponent);
	void	OnCaptureMouseDown(const CKey& key);

	void	OnGotFocus();
	void	OnLostFocus();

	void	OnClickedBtnNext(gui::CComponent* pComponent);
	void	OnClickedBtnPrev(gui::CComponent* pComponent);
	void	OnClickedBtnMail(gui::CComponent* pComponent);
	void	OnClickedBtnAway(gui::CComponent* pComponent);
	void	OnPaintBtnAway();
	void	OnMovedBtnAway(RwInt32 iOldX, RwInt32 iOldY);
	void	OnKeyUp(gui::CComponent* pComponenet, CInputDevice* pDevice, const CKey& key);
	void	OnNoticeCharChanged(int iLength);
	void	OnClickedBtnSale(gui::CComponent* pComponent);

	// ShopItemGui Event
	void	OnShopItemEnter(CPrivateShopItemGui* pShopItem);
	void	OnShopItemLeave(CPrivateShopItemGui* pShopItem);
	void	OnShopItemClicked(CPrivateShopItemGui* pShopItem, const CKey& Key);
	void	OnShopItemPriceClicked(CPrivateShopItemGui* pShopItem, gui::CComponent* pComponent);
	
	// Gui Setting
	void	SetSale(bool bSale);		// 판매개시
	void	SetAway(bool bAway);		// 자리비움
	void	SetNotice(const WCHAR* pText);
	void	SetPage(RwUInt32 iPage, bool bCheckMax = true);

	void	SetUserInterface(ePrivateShopType eType, sPRIVATESHOP_SHOP_DATA* pShopData, bool bIsSale);
	void	SetShopItemSelect(CPrivateShopItemGui* pShopItem);
	void	SetShopItemFocus(CPrivateShopItemGui* pShopItem);

	// Get
	RwUInt8	GetShopState() { return m_ShopData.sSummaryPrivateShopData.byShopState; }

	void	Clear();
	void	ClearShopItem();

	void	UpdateShopItem();
	void	UpdateShopItemData();

	void	SetShopItemData(RwUInt8 uiPrivateShopPos, sPRIVATESHOP_ITEM_DATA* pShopItemData, bool bEvent = true);
	void	ShowInfoWindow(RwBool bIsShow, CPrivateShopItemGui* pShopItem = NULL);

	void	CalcNoticeMaxNum();
	void	CheckSoldOut();

protected:
	// Gui Common Components
	gui::CStaticBox*			m_pTitle;
	gui::CButton*				m_pCloseButton;
	gui::CTabButton*			m_pTabButton;
	gui::CDialog*				m_apDialogTab[TAB_NUM];

	// Gui Common Signals
	gui::CSlot					m_slotMove;
	gui::CSlot					m_slotTabChanged;
	gui::CSlot					m_slotCloseButtonClicked;
	gui::CSlot					m_slotTabShopMouseLeave;
	gui::CSlot					m_slotCaptureMouseDown;

	// TAB_SHOP ( 상점 ) Components
	gui::CButton*				m_pNextButton;
	gui::CButton*				m_pPrevButton;
	gui::CButton*				m_pSaleButton;
	gui::CStaticBox*			m_pZenny;
	gui::CStaticBox*			m_pZennyTitle;
	gui::CStaticBox*			m_pCurrentPage;
	gui::CPanel*				m_pSoldOut;
	gui::CStaticBox*			m_pStbSoldOut;

	gui::CSlot					m_slotNextClicked;
	gui::CSlot					m_slotPrevClicked;
	gui::CSlot					m_slotSaleClicked;
	gui::CSlot					m_slotEffectPaint;

	// TAB_SHOP ( 상점 ) Signals
	CPrivateShopItemGui			m_aShopItem[SLOT_NUM];		///< 개인상점 아이템
	CSurfaceGui					m_EffectSelect;				///< 선택된 상태의 그림
	CSurfaceGui					m_EffectFocus;				///< 포커스 상태의 그림

	// TAB_SHOP ( 상점 ) State & Effect
	ePrivateShopType			m_eShopType;				///< 상점의 상태
	sPRIVATESHOP_ITEM_DATA		m_aShopItemData[NTL_MAX_PRIVATESHOP_INVENTORY];
	sPRIVATESHOP_SHOP_DATA		m_ShopData;

	CPrivateShopItemGui*		m_pItemSelect;		///< 선택된 아이템
	CPrivateShopItemGui*		m_pItemFocus;		///< 포커스 된 아이템

	RwUInt32					m_uiItemCount;		///< 아이템 갯수
	RwUInt32					m_uiPageMax;		///< 최대 페이지 
	RwUInt32					m_uiPageCur;		///< 현재 페이지
	bool						m_bAway;			///< 자리비움
	bool						m_bSale;			///< 판매중

	// TAB_NOTE ( 공지 ) Components
	gui::CStaticBox*			m_pShopNameTitle;
	gui::CInputBox*				m_pShopNameInput;
	gui::CStaticBox*			m_pShopNoticeTitle;
	gui::CInputBox*				m_pShopNoticeInput;
	gui::CDialog*				m_pShopNoticeDialog;
	gui::CButton*				m_pAwayButton;
	gui::CButton*				m_pMailButton;
	gui::CStaticBox*			m_pNoticeMaxNum;

	// TAB_NOTE ( 공지 ) Signals
	gui::CSlot					m_slotNameGotFocus;
	gui::CSlot					m_slotNameLostFocus;
	gui::CSlot					m_slotNoticeGotFocus;
	gui::CSlot					m_slotNoticeLostFocus;
	gui::CSlot					m_slotMailClicked;
	gui::CSlot					m_slotAwayClicked;
	gui::CSlot					m_slotAwayPaint;
	gui::CSlot					m_slotAwayMove;
	gui::CSlot					m_slotNameKeyUp;
	gui::CSlot					m_slotNoticeKeyUp;
	gui::CSlot					m_slotNoticeCharChanged;

	CSurfaceGui					m_surAwayCheck;
	CSurfaceGui					m_surAwayUnCheck;
	
};

#endif//__PRIVATE_SHOP_GUI_H__