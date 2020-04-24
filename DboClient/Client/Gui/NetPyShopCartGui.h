/******************************************************************************
* File			: NetPyShopCartGui.h
* Author		: Cho Haesung
* Copyright		: (주)NTL
* Date			: 2009. 6. 25
* Abstract		: 
*****************************************************************************
* Desc			: Commercial 기능 중 NetPyShop의 Cart GUI 및 기능을 정의하는 Class
*****************************************************************************/

#ifndef __NETPYSHOP_CART_GUI_H__
#define __NETPYSHOP_CART_GUI_H__

#pragma once

// Core
#include "ceventhandler.h"

// Shared
#include "NtlPacketUG.h"

// Gui
#include "eventtimer.h"
#include "gui_slot.h"
#include "gui_button.h"
#include "gui_staticbox.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Dbo
#include "SlotGui.h"
#include "Windowby3.h"

struct sSHOP_BUY_CART;
struct SDboEventNetPyShopEvent;

#define dNETPYSHOP_CART_INVALID_INDEX		(-1)

class CNetPyShopCartGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	CNetPyShopCartGui(const RwChar* pName);
	virtual ~CNetPyShopCartGui();

	enum eNetPyShopCartDefinedValue
	{		
		MAX_SLOT				= 12,	///< Buy, Sell을 위해 제공되는 각각의 최대 슬롯 갯수		
	};

	struct MouseDownSlot
	{
		RwInt32		iSlotKind;
		RwInt32		iSlotIndex;

		MouseDownSlot()
		{
			iSlotKind = dNETPYSHOP_CART_INVALID_INDEX;
			iSlotIndex = dNETPYSHOP_CART_INVALID_INDEX;
		}
	};

	enum
	{
		BUY_SLOT,

		SLOTKIND_NUM
	};

	struct BuySlotInfo
	{
		sSHOP_BUY_CART		NPCShopBuyInfo;		///< 서버로 넘기기 위한 정보		
		CRegularSlotGui			slot;
	};

	RwBool			Create();
	VOID			Destroy();

	VOID			OpenCart();		///< 카트 UI를 연다
	VOID			CloseCart();						///< 카트를 닫는다.

	RwInt32			SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

	///< 자신이 사고 팔 아이템을 더하거나 뺀다.
	///< iSlotKind : BUY_SLOT
	///< iSlotY : Trade Slot Index
	///< iCount : 갯수
	VOID			AddItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount);
	VOID			SubItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount);

protected:
	CNetPyShopCartGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &pMsg );

	VOID			Clear();									///< 트레이드 카트의 정보를 지운다.
	VOID			ClearSlot(RwInt32 iSlotKind, RwInt32 iSlot);///< 해당 슬롯의 내용을 지운다.

	VOID			AddItemCount(RwInt32 iSlotKind, RwInt32 iSlot, RwInt32 iCount);	///< 슬롯의 아이템 갯수를 더하거나 뺀다.
	VOID			UpdateCount(RwUInt8 bySlotKind, RwUInt8 bySlot, RwUInt8 byCount);

	VOID			CalcTotalBuyPrice();						///< 총 구입액 계산

	///< NPCShop 또는 인벤에서 아이템을 올리는 방법에 따라 분류
	VOID			RegBuyItemByDrag(RwInt32 iSlot);	
	VOID			RegBuyItemByEvent(RwInt32 iSlot, SDboEventNetPyShopEvent& TradeInfo);	

	RwInt32			FindEmptySlot(RwInt32 iSlotType);			///< Slot 종류별 비어있는 가장 빠른 순서의 슬롯의 찾는다.
	RwInt32			FindSlot(RwUInt8 bySlotType, RwUInt32 uiSerial);
	RwInt32			PtinSlot(RwInt32 iSlotKind, RwInt32 iX, RwInt32 iY);

	VOID			CheckInfoWindow();							///< 상황에 따라 인포윈도우를 닫는다

	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotKind = 0, RwInt32 iSlotIdx = -1 );		///< 슬롯 포커스 이펙트

	VOID			OnPaint();

	VOID			ClickedCloseButton(gui::CComponent* pComponent);
	VOID			ClickedBuyButton(gui::CComponent* pComponent);		///< 선택한 물품들을 산다.
	
	VOID			ClickUpButton(gui::CComponent* pComponent);	///< 아이템 갯수 더하기 버튼을 눌렀다
	VOID			ClickDownButton(gui::CComponent* pComponent);	///< 아이템 갯수 빼기 버튼을 눌렀다

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
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
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotBtnZenny;
	gui::CSlot			m_slotClickedBuy;
	gui::CSlot			m_slotUpButton[SLOTKIND_NUM][MAX_SLOT];
	gui::CSlot			m_slotDownButton[SLOTKIND_NUM][MAX_SLOT];

	RwInt8				m_byInfoWindowIndex;
	MouseDownSlot		m_MouseDownSlot;			///< 마우스로 눌린 슬롯

	gui::CTexture*		m_pMoneyIconTexture;

	gui::CButton*		m_pExitButton;				///< 창닫기 버튼
	gui::CButton*		m_pBuyButton;				///< 구입 버튼

	gui::CButton*		m_pUpButton[SLOTKIND_NUM][MAX_SLOT];	///< 아이템 갯수 더하기 버튼
	gui::CButton*		m_pDownButton[SLOTKIND_NUM][MAX_SLOT];	///< 아이템 갯수 빼기 버튼

	gui::CStaticBox*	m_pDialogName;				///< 다이얼로그의 이름 스태틱
	gui::CStaticBox*	m_pTotalBuyMoney;			///< 총 구입 금액
	gui::CStaticBox*	m_pTotalSellMoney;			///< 총 판매 금액

	CSurfaceGui			m_FocusEffect;				///< 슬롯 포커스 이펙트
	CSurfaceGui			m_BuyBar;					///< 사기 서페이스

	RwBool				m_bFocus;					///< 포커스를 얻었다.	

	RwInt32				m_iCurTabIndex;				///< 현재의 탭 인덱스

	BuySlotInfo			m_BuySlotInfo[MAX_SLOT];	///< 왼쪽 슬롯 정보
	
	RwUInt32			m_uiTotalBuyPrice;			///< 총 구입 금액
};


#endif