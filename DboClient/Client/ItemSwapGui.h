/******************************************************************************
* File			: NPCShop.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 8. 1
* Abstract		:
*****************************************************************************
* Desc			: NPC 상점
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

#include "NtlItem.h"

// gui
#include "eventtimer.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "Windowby3.h"

struct sNPC_TBLDAT;
class CNtlSobActor;

class CItemSwapGui : public CNtlPLGui, public RWS::CEventHandler
{
#define dMAX_ITEM_PANEL		6			///< Shop의 아이템 패널 갯수

public:
	CItemSwapGui(const RwChar* pName);
	virtual ~CItemSwapGui();

	enum eShopType
	{
		SHOP_TYPE_NORMAL,
		SHOP_TYPE_TENKAICHI,
	};

	struct ItemPanel
	{
		CRegularSlotGui		slot;			///< slot
		gui::CPanel*		pItemPanel;		///< Item panel
		gui::CStaticBox*	pItemName;		///< 이름
		gui::CStaticBox*	pPoint;			///< 필요한 포인트(가격, 천하제일 무도회 포인트 등...)

		gui::CStaticBox*	pstbNeedItemCount;
		CRegularSlotGui		slotNeedItem;
	};

	struct ShopItem
	{
		SERIAL_HANDLE	hItem;
		std::wstring	wstrItemName;
		RwInt32			iCount;
		RwUInt32		uiPrice;

		std::wstring	wstrNeedItemName;

		sITEM_TBLDAT*	pITEM_DATA;
		sITEM_TBLDAT*	pNeedITEM_DATA;
	};

	VOID			Init();
	RwBool			Create();
	VOID			Destroy();

	RwInt32			SwitchDialog(bool bOpen);		///< DialogManager에서의 Open/Close

	SERIAL_HANDLE	GetNPCSerial();

protected:

	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			ResetCartTexture();

	VOID			OpenShop(SERIAL_HANDLE hNPC, sNPC_TBLDAT* pNPC_TBLDAT);	///< NPC 상점을 연다
	VOID			CloseShop();						///< NPC 상점을 닫는다.	

	VOID			ClearShop();						///< Shop의 정보를 초기화 한다.
	VOID			ClearPanels();						///< 패널 내용 전부 초기화 한다.

	VOID			UpdateTabContent(RwUInt8 byIndex);	///< 탭에 맞는 내용을 갱신한다
	RwBool			SetPage(RwInt32 iPage);				///< 현재 페이지 설정
	VOID			SetPanel(RwInt32 iPage);			///< 현재 페이지에 맞는 아이템 리스트를 보여준다.
	VOID			SetPageButton();

	VOID			SetZenny();							///< 자신의 소지금 정보를 업데이트 한다.

	VOID			SetRepairAllTooltip();				///< 현재 전체 수리비용을 계산하여 정보를 표시한다

	RwUInt8			GetPageCount_of_CurTab();			///< 현재 탭에 속한 페이지의 갯수를 반환한다

	RwBool			IsFirstPage();
	RwBool			IsLastPage();

	RwInt8			PtinSlot(RwInt32 iX, RwInt32 iY);
	RwInt8			PtinSlot_of_SmallIcon_in_panel(RwInt32 iX, RwInt32 iY);
	VOID			FocusEffect(RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			SelectEffect(RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			CheckInfoWindow();

	VOID			OnPaint();
	VOID			OnPostPaint();

	VOID			OnSelectChangeTabButton(INT nCurIndex, INT nPrevIndex);	///< 탭 버튼을 눌렀다

	VOID			ClickedPrePageButton(gui::CComponent* pComponent);	///< 이전 버튼을 눌렀다.
	VOID			ClickedNextPageButton(gui::CComponent* pComponent);	///< 다음 버튼을 눌렀다.
	VOID			ClickedCloseButton(gui::CComponent* pComponent);	///< 닫기 버튼을 눌렀다.
	VOID			ClickedRepairButton(gui::CComponent* pComponent);	///< 수리 버튼을 눌렀다.
	VOID			ClickedRepairAllButton(gui::CComponent* pComponent);///< 전체 수리 버튼을 눌렀다.
	VOID			ClickedIdentifyButton(gui::CComponent* pComponent);	///< 아이템 감정 버튼을 눌렀다.

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureMouseDown(const CKey& key);


protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotTab;
	gui::CSlot			m_slotPrePageButton;
	gui::CSlot			m_slotNextPageButton;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotRepairButton;
	gui::CSlot			m_slotRepairAllButton;
	gui::CSlot			m_slotIdentifyButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;
	gui::CSlot			m_slotCaptureMouseDown;

	RwInt8				m_byInfoWindowIndex;
	RwInt32				m_iMouseDownSlot;	///< 마우스로 눌린 슬롯의 인덱스
	RwInt32				m_iSelectedSlot;	///< 셀렉트 이펙트 슬롯
	RwInt32				m_iClickEffectedSlot;

	gui::CTabButton*	m_pTabButton;		//< 탭 버튼

	ItemPanel			m_ItemPanel[dMAX_ITEM_PANEL];	///< ItemPanel

	CWindowby3			m_BackLineSurface;	///< 백라인

	CSurfaceGui			m_FocusEffect;		///< 슬롯 포커스 이펙트
	CSurfaceGui			m_SelectEffect;		///< 슬롯 셀렉트 이펙트
	CSurfaceGui			m_MoneyBackPanel;	///< 소지금 배경
	CSurfaceGui			m_PageBackPanel;	///< 페이지 배경

	gui::CButton*		m_pExitButton;		///< 창닫기 버튼
	gui::CButton*		m_pRepairButton;	///< 수리 버튼
	gui::CButton*		m_pRepairAllButton;	///< 전체 수리 버튼
	gui::CButton*		m_pIdentifyButton;	///< 아이템 감정 버튼	
	gui::CButton*		m_pPrePageButton;	///< 이전 페이지 넘어가기 버튼
	gui::CButton*		m_pNextPageButton;	///< 다음 페이지 넘어가기 버튼

	gui::CStaticBox*	m_pShopTitle;		///< 상점 이름	

	gui::CStaticBox*	m_pPocketMoneytitle;///< 소지금 제목
	gui::CStaticBox*	m_pPocketMoney;		///< 소지금	
	gui::CStaticBox*	m_pCurrentPage;		///< 현재 페이지 표시

	ShopItem			m_aShopItem[NTL_MAX_MERCHANT_TAB_COUNT][NTL_MAX_MERCHANT_COUNT];	/// 각 탭별로 팔 수 있는 아이템 정보

	SERIAL_HANDLE		m_hNPCSerial;		///< NPC 번호

	RwInt32				m_iCurTab;			///< 현재 탭 인덱스
	RwInt32				m_iCurPage;			///< 현재 페이지

	RwBool				m_bFocus;			///< 포커스를 얻었다.
};