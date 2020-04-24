/******************************************************************************
* File			: CWarehouseGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 11. 20
* Abstract		: 
*****************************************************************************
* Desc			: CWarehouseGui
*				  한 캐릭터가 가지는 16칸의 일반창고 3개
*				  한 계정의 모든 캐릭터가 공유하는 16칸의 공유창고를 구현
*				  제니는 공유창고에만 저장되는 것으로써 한 계정의 모든 캐릭터
*				  가 공유한다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// share
#include "NtlItem.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "SlotGui.h"


//////////////////////////////////////////////////////////////////////////
//	CWarehouseGui
//////////////////////////////////////////////////////////////////////////

class CWarehouseGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CWarehouseGui(const RwChar* pName);
	virtual ~CWarehouseGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);		///< DialogManager에서의 Open/Close

	VOID		RegisterWarehouseIndex(RwUInt8 byIndex);
	VOID		ShowDisableSlot(RwBool bShow, RwUInt8 bySlot);

protected:
	CWarehouseGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Init();

	VOID			UpdateItems();

	RwInt8			PtinSlot(RwInt32 iX, RwInt32 iY);				///< 마우스가 영역으로 들어온 슬롯을 찾는다.
	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			SelectEffect( RwBool bPush, RwInt32 iSlotIdx = -1);	
	VOID			CheckInfoWindow();

	VOID			ClickedCloseButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);

	VOID			OnPaint();
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CButton*		m_pExitButton;		///< 창닫기 버튼

	RwInt8				m_byInfoWindowIndex;
	RwInt32				m_iMouseDownSlot;	///< 마우스로 눌린 슬롯의 인덱스
	RwInt32				m_iSelectedSlot;	///< 셀렉트 이펙트 슬롯
	RwInt32				m_iClickEffectedSlot;///< 현재 클릭 이펙트가 생긴 슬롯

	gui::CStaticBox*	m_pDialogName;		///< 다이얼로그의 이름 스태틱

	CSurfaceGui			m_FocusEffect;		///< 슬롯 포커스 이펙트
	CSurfaceGui			m_SelectEffect;		///< 슬롯 셀렉트 이펙트

	RwUInt8				m_byWarehouseIndex;	///< 몇 번째 창고인지

	RwBool				m_bFocus;			///< 포커스를 얻었다.

	CRegularSlotGui			m_Slot[NTL_MAX_BANK_ITEM_SLOT];	///< 아이템 슬롯
};


//////////////////////////////////////////////////////////////////////////
//	CWarehouseCommon
//////////////////////////////////////////////////////////////////////////

class CWarehouseCommonGui : public CWarehouseGui
{
public:
	CWarehouseCommonGui(const RwChar* pName);
	virtual ~CWarehouseCommonGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetNPCHandle(SERIAL_HANDLE hHandle);
	SERIAL_HANDLE GetNPCHandle();

protected:
	CWarehouseCommonGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			ClickedZennyButton(gui::CComponent* pComponent);

	VOID			OnPostPaint();

protected:
	gui::CSlot			m_slotZennyButton;
	gui::CSlot			m_slotPostPaint;

	gui::CButton*		m_pZennyButton;
	gui::CStaticBox*	m_pZenny;
	gui::CTexture*		m_pMoneyIconTexture;

	CSurfaceGui			m_srfZennySlotDestination;	///< 제니 슬롯 Destination 이미지

	RwBool				m_bZennySlotDestination;
	RwUInt32			m_uiZenny;
	SERIAL_HANDLE		m_hNPCSerial;		///< 창고지기 NPC Serial
};


//////////////////////////////////////////////////////////////////////////
//	CWarehouseBar
//////////////////////////////////////////////////////////////////////////

class CWarehouseBarGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CWarehouseBarGui(const RwChar* pName);
	virtual ~CWarehouseBarGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);		///< DialogManager에서의 Open/Close

	VOID		RegisterWarehouse(RwUInt8 byIndex, CWarehouseGui* pWarehouseGui);
	SERIAL_HANDLE	GetNPCSerial();

protected:
	CWarehouseBarGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			CloseWarehouse();

	VOID			Clicked_1_Button(gui::CComponent* pComponent);
	VOID			Clicked_2_Button(gui::CComponent* pComponent);
	VOID			Clicked_3_Button(gui::CComponent* pComponent);
	VOID			Clicked_Common_Button(gui::CComponent* pComponent);
	VOID			ClickedAllButton(gui::CComponent* pComponent);
	VOID			ClickedCloseButton(gui::CComponent* pComponent);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotWarehouseBtn[NTL_MAX_BANKSLOT_COUNT];
	gui::CSlot			m_slot_All_Button;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CButton*		m_pWarehouseBtn[NTL_MAX_BANKSLOT_COUNT];///< 창고 버튼
	gui::CButton*		m_p_All_Button;		///< 모든 창고 버튼
	gui::CButton*		m_pExitButton;		///< 창닫기 버튼
	
	SERIAL_HANDLE		m_hNPCSerial;		///< 창고지기 NPC Serial

	CWarehouseGui*		m_pWareHouseGui[NTL_MAX_BANKSLOT_COUNT];
};