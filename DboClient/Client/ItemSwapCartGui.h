/******************************************************************************
* File			: ItemSwapCartGui.h
* Author		: Daneos
* Copyright		: Daneos
* Date			: 14-sep-2018
* Abstract		:
*****************************************************************************
* Desc			: Item exchange cart
*				  SellSlot's Serial writes Sob's
*****************************************************************************/

#pragma once

#include "SlotGui.h"
#include "gui_slot.h"
#include "NtlPLGui.h"
#include "NtlSLDef.h"
#include "ceventhandler.h"
#include "eventtimer.h"
#include "gui_button.h"
#include "gui_staticbox.h"
#include "NtlPacketUG.h"
#include "Windowby3.h"

struct sSHOP_SELL_CART;
struct SDboEventShop;

class CItemSwapCartGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CItemSwapCartGui(const RwChar* pName);
	virtual ~CItemSwapCartGui();

	enum eItemSwapCartDefinedValue
	{
		MAX_SLOT = 12,	///< Max sell slots	
	};

	struct  SellSlotInfo
	{
		sSHOP_SELL_CART		NPCShopSellInfo;	///< Information for passing to the server
		CRegularSlotGui		slot;
	};

	RwBool			Create();
	VOID			Destroy();

	VOID			OpenCart(RwUInt32 uiSerial);		///< Open the cart UI
	VOID			CloseCart();						///< Close cart

	RwInt32			SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

	RwUInt32		GetTargetSerial();

	///< Add or subtract items you sell
	///< iSlotY : Trade Slot Index
	///< iCount : amount
	VOID			AddItem(RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx);
	VOID			SubItem(RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx);

protected:
	CItemSwapCartGui() {}
	virtual VOID	HandleEvents(RWS::CMsg &pMsg);

	VOID			ResetCartTexture();

	VOID			Clear();									///< 트레이드 카트의 정보를 지운다.
	VOID			ClearSlot(RwInt32 iSlot);///< 해당 슬롯의 내용을 지운다.

	VOID			AddItemCount(RwInt32 iSlot, RwInt32 iCount, RwUInt32 itemSerial);	///< 슬롯의 아이템 갯수를 더하거나 뺀다.
	VOID			UpdateCount(RwUInt8 bySlot, RwUInt8 byCount);

	VOID			CalcTotalSellPrice();						///< Calculate total sale price

	VOID			RegSellItemByDrag(RwInt32 iSlot);

	VOID			RegBuyItemByEvent(SDboEventShop& TradeInfo);
	VOID			RegSellItemByEvent(RwInt32 iSlot, SDboEventShop& TradeInfo);

	RwInt32			FindEmptySlot();			///< Slot 종류별 비어있는 가장 빠른 순서의 슬롯의 찾는다.
	RwUInt8			FindSlot(RwUInt32 uiSerial);
	RwInt32			PtinSlot(RwInt32 iX, RwInt32 iY);

	VOID			CheckInfoWindow();							///< 상황에 따라 인포윈도우를 닫는다

	VOID			FocusEffect(RwBool bPush, RwInt32 iSlotIdx = -1);		///< 슬롯 포커스 이펙트

	VOID			OnPaint();

	VOID			ClickedSellButton(gui::CComponent* pComponent);		///< 선택한 물품들을 판다.

	VOID			ClickUpButton(gui::CComponent* pComponent);	///< 아이템 갯수 더하기 버튼을 눌렀다
	VOID			ClickDownButton(gui::CComponent* pComponent);	///< 아이템 갯수 빼기 버튼을 눌렀다

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouse;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CSlot			m_slotRightLockButton;
	gui::CSlot			m_slotRightUnlockButton;

	gui::CSlot			m_slotClickedSell;
	gui::CSlot			m_slotUpButton[MAX_SLOT];
	gui::CSlot			m_slotDownButton[MAX_SLOT];

	RwInt8				m_byInfoWindowIndex;

	gui::CTexture*		m_pMoneyIconTexture;

	gui::CButton*		m_pExitButton;				///< 창닫기 버튼

	gui::CButton*		m_pSellButton;				///< 판매 버튼

	gui::CButton*		m_pUpButton[MAX_SLOT];		///< Item number plus button
	gui::CButton*		m_pDownButton[MAX_SLOT];	///< Item number minus button

	gui::CStaticBox*	m_pTotalSellMoney;			///< 총 판매 금액

	CSurfaceGui			m_FocusEffect;				///< 슬롯 포커스 이펙트

	CSurfaceGui			m_SellBar;					///< 팔기 서페이스

	CWindowby3			m_LeftLockSurface;			///< 왼쪽 잠김 서페이스
	CWindowby3			m_RightLockSurface;			///< 오른쪽 잠김 서페이스

	RwBool				m_bFocus;					///< 포커스를 얻었다.
	bool				m_bLeftLock;				///< 왼쪽 물품 등록 잠김
	bool				m_bRightLock;				///< 오른쪽 물품 등록 잠김

	RwUInt32			m_hNPCSerial;				///<

	RwInt32				m_iCurTabIndex;				///< 현재의 탭 인덱스

	SellSlotInfo		m_SellSlotInfo[MAX_SLOT];	///< 오른쪽 슬롯 정보

	RwUInt32			m_uiTotalSellPrice;			///< 총 판매 금액

	RwUInt8				m_byOthersEmptySlot;		///< 유저간 거래시 상대방의 거래 가능한 빈 슬롯수
	RwUInt32			m_uiUserTradePacketCount;	///< 유저간 거래시 업데이트 된 상대방의 패킷의 갯수

	RwInt32				iMouseDownSlotIndex;
};
