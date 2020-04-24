/******************************************************************************
* File			: TradeCart.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 8. 1
* Abstract		: 
*****************************************************************************
* Desc			: Trade 시 이용할 쇼핑 카트
*				  BuySlot의 Serial은 아이템테이블 것을 쓰고
*				  SellSlot의 Serial은 Sob의 것을 쓴다
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

struct sSHOP_BUY_CART;
struct sSHOP_SELL_CART;
struct SDboEventShop;

class CTradeCart : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	CTradeCart(const RwChar* pName);
	virtual ~CTradeCart();

	enum eTradeCartDefinedValue
	{		
		MAX_SLOT				= 12,	///< Buy, Sell을 위해 제공되는 각각의 최대 슬롯 갯수		
	};

	enum
	{
		BUY_SLOT,
		SELL_SLOT,

		SLOTKIND_NUM
	};

	enum eTradeCartTextureType
	{
		TRADECART_TEXTURE_NORMAL,
		TRADECART_TEXTURE_TENKAICHI
	};

	struct MouseDownSlot
	{
		RwInt32		iSlotKind;
		RwInt32		iSlotIndex;

		MouseDownSlot()
		{
			iSlotKind = -1;
			iSlotIndex = -1;
		}
	};

	struct BuySlotInfo
	{
		sSHOP_BUY_CART		NPCShopBuyInfo;		///< 서버로 넘기기 위한 정보		
		CRegularSlotGui			slot;
		sITEM_DATA			TakeItem;			///< 유저간 거래시 받을 아이템
		SERIAL_HANDLE		hGiveHandle;		///< 유저간 거래시 받을 아이템의 핸들(서버상에 존재, 현재 자신이 가지고 있지는 않다.)

		BuySlotInfo()
		:hGiveHandle(INVALID_SERIAL_ID)
		{
			memset((void*)&TakeItem, 0, sizeof(TakeItem) );
			TakeItem.itemNo = INVALID_SERIAL_ID;
		}
	};

	struct  SellSlotInfo 
	{		
		sSHOP_SELL_CART		NPCShopSellInfo;	///< 서버로 넘기기 위한 정보
		CRegularSlotGui			slot;
	};

	RwBool			Create();
	VOID			Destroy();

	VOID			OpenCart(eDialogType eType, RwUInt32 uiSerial);		///< 카트 UI를 연다
	VOID			CloseCart(eDialogType eType);						///< 카트를 닫는다.

	RwInt32			SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

	RwUInt32		GetTargetSerial();

	///< 자신이 사고 팔 아이템을 더하거나 뺀다.
	///< iSlotKind : BUY_SLOT, SELL_SLOT
	///< iSlotY : Trade Slot Index
	///< iCount : 갯수
	VOID			AddItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx);
	VOID			SubItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx);

protected:
	CTradeCart() {}
	virtual VOID	HandleEvents( RWS::CMsg &pMsg );

	VOID			ResetCartTexture(eTradeCartTextureType eType);

	VOID			Clear();									///< 트레이드 카트의 정보를 지운다.
	VOID			ClearSlot(RwInt32 iSlotKind, RwInt32 iSlot);///< 해당 슬롯의 내용을 지운다.

	VOID			AddItemCount(RwInt32 iSlotKind, RwInt32 iSlot, RwInt32 iCount, RwUInt32 itemSerial);	///< 슬롯의 아이템 갯수를 더하거나 뺀다.
	VOID			UpdateCount(RwUInt8 bySlotKind, RwUInt8 bySlot, RwUInt8 byCount);

	VOID			CalcTotalBuyPrice();						///< 총 구입액 계산
	VOID			CalcTotalSellPrice();						///< 총 판매액 계산

	///< NPCShop 또는 인벤에서 아이템을 올리는 방법에 따라 분류
	VOID			RegBuyItemByDrag(RwInt32 iSlot);
	VOID			RegSellItemByDrag(RwInt32 iSlot);

	VOID			RegBuyItemByEvent(RwInt32 iSlot, SDboEventShop& TradeInfo);	
	VOID			RegSellItemByEvent(RwInt32 iSlot, SDboEventShop& TradeInfo);

	// 유저 트레이드에서 아이템을 등록할 때
	VOID			RegTakeItemByPacket(RwInt32 iSlot, SERIAL_HANDLE hTakeHandle, sITEM_DATA* pITEM_DATA, RwUInt8 byCount);
	VOID			RegGiveItemByPacket(RwInt32 iSlot, RwUInt32 uiSerial, RwUInt8 byCount);

	RwInt32			FindEmptySlot(RwInt32 iSlotType);			///< Slot 종류별 비어있는 가장 빠른 순서의 슬롯의 찾는다.
	RwUInt8			FindSlot(RwUInt8 bySlotType, RwUInt32 uiSerial);
	RwInt32			PtinSlot(RwInt32 iSlotKind, RwInt32 iX, RwInt32 iY);

	VOID			CheckInfoWindow();							///< 상황에 따라 인포윈도우를 닫는다
	
	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotKind = 0, RwInt32 iSlotIdx = -1 );		///< 슬롯 포커스 이펙트

	VOID			OnPaint();
	VOID			OnPostPaint();

	VOID			ClickedCloseButton(gui::CComponent* pComponent);

	VOID			ClickedZennyButton(gui::CComponent* pComponent);

	VOID			ClickedBuyButton(gui::CComponent* pComponent);		///< 선택한 물품들을 산다.
	VOID			ClickedSellButton(gui::CComponent* pComponent);		///< 선택한 물품들을 판다.

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
	gui::CSlot			m_slotPostPaint;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotRightLockButton;
	gui::CSlot			m_slotRightUnlockButton;
	gui::CSlot			m_slotBtnZenny;
	gui::CSlot			m_slotClickedBuy;
	gui::CSlot			m_slotClickedSell;
	gui::CSlot			m_slotUpButton[SLOTKIND_NUM][MAX_SLOT];
	gui::CSlot			m_slotDownButton[SLOTKIND_NUM][MAX_SLOT];

	RwInt8				m_byInfoWindowIndex;
	MouseDownSlot		m_MouseDownSlot;			///< 마우스로 눌린 슬롯

	gui::CTexture*		m_pMoneyIconTexture;
	
	gui::CButton*		m_pExitButton;				///< 창닫기 버튼
	gui::CButton*		m_pZennyButton;				///< 제니 버튼(유저 트레이드용 오른쪽 제니 버튼)
	gui::CButton*		m_pBuyButton;				///< 구입 버튼
	gui::CButton*		m_pSellButton;				///< 판매 버튼

	gui::CButton*		m_pUpButton[SLOTKIND_NUM][MAX_SLOT];	///< 아이템 갯수 더하기 버튼
	gui::CButton*		m_pDownButton[SLOTKIND_NUM][MAX_SLOT];	///< 아이템 갯수 빼기 버튼

	gui::CStaticBox*	m_pDialogName;				///< 다이얼로그의 이름 스태틱
	gui::CStaticBox*	m_pTotalBuyMoney;			///< 총 구입 금액
	gui::CStaticBox*	m_pTotalSellMoney;			///< 총 판매 금액

	CSurfaceGui			m_FocusEffect;				///< 슬롯 포커스 이펙트
	CSurfaceGui			m_BuyBar;					///< 사기 서페이스
	CSurfaceGui			m_SellBar;					///< 팔기 서페이스
	CSurfaceGui			m_GiveBar;					///< 받기 서페이스
	CSurfaceGui			m_TakeBar;					///< 주기 서페이스
	CSurfaceGui			m_srfZennySlotDestination;	///< 제니 슬롯 Destination 이미지

	CWindowby3			m_LeftLockSurface;			///< 왼쪽 잠김 서페이스
	CWindowby3			m_RightLockSurface;			///< 오른쪽 잠김 서페이스

	RwBool				m_bFocus;					///< 포커스를 얻었다.
	bool				m_bLeftLock;				///< 왼쪽 물품 등록 잠김
	bool				m_bRightLock;				///< 오른쪽 물품 등록 잠김
	RwBool				m_bZennySlotDestination;

	eTradeCartTextureType m_eTextureType;			///< 카트 텍스처 타입
	RwUInt32			m_uiSubjectSerial;			///< TradeCart를 대상 Serial
	eDialogType			m_eDialogType;				///< 어떤 거래를 위한 카트인가
	RwInt32				m_iCurTabIndex;				///< 현재의 탭 인덱스

	BuySlotInfo			m_BuySlotInfo[MAX_SLOT];	///< 왼쪽 슬롯 정보
	SellSlotInfo		m_SellSlotInfo[MAX_SLOT];	///< 오른쪽 슬롯 정보

	RwUInt32			m_uiTotalSellPrice;			///< 총 판매 금액

	RwUInt8				m_byOthersEmptySlot;		///< 유저간 거래시 상대방의 거래 가능한 빈 슬롯수
	RwUInt32			m_uiUserTradePacketCount;	///< 유저간 거래시 업데이트 된 상대방의 패킷의 갯수
};
