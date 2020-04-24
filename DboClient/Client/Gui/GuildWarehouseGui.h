/******************************************************************************
* File			: CGuildWarehouseGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 2. 22
* Abstract		: 
*****************************************************************************
* Desc			: 기존의 작성된 CWarehouse 를 활용하지 않는다.
*				  차후 GuildWarehouse만이 기획이 다르게 변할 경우 등을 생각하여
*				  CWarehouse 클래스와는 연관성 없이 제작한다
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


struct sGuildWarehouseSlot;

//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseGui
//////////////////////////////////////////////////////////////////////////

class CGuildWarehouseGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CGuildWarehouseGui(const RwChar* pName);
	virtual ~CGuildWarehouseGui();

	virtual RwBool Create();
	virtual VOID Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	VOID		RegisterWarehouseIndex(RwUInt8 byIndex);
	VOID		SetItem(sGuildWarehouseSlot* pSlotData, RwUInt8 byPos);

protected:
	CGuildWarehouseGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Init();

	VOID			SetupItems();

	RwInt8			PtinSlot(RwInt32 iX, RwInt32 iY);				///< 마우스가 영역으로 들어온 슬롯을 찾는다.
	VOID			FocusEffect( RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			SelectEffect( RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID			ShowDisableSlot(RwBool bShow, RwUInt8 bySlot);
	VOID			CheckInfoWindow();

	VOID			ClickedCloseButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	virtual VOID	OnMove( RwInt32 iOldX, RwInt32 iOldY );
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

	CRegularSlotGui			m_Slot[NTL_MAX_GUILD_ITEM_SLOT];	///< 아이템 슬롯
};


//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseAddon
//////////////////////////////////////////////////////////////////////////

class CGuildWarehouseAddonGui : public CGuildWarehouseGui
{
public:
	CGuildWarehouseAddonGui(const RwChar* pName);
	virtual ~CGuildWarehouseAddonGui();

	virtual RwBool Create();
	virtual VOID Destroy();

	VOID	SetZenny(RwUInt32 uiZenny);

protected:
	CGuildWarehouseAddonGui() {}
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
};


//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseBar
//////////////////////////////////////////////////////////////////////////

class CGuildWarehouseBar : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CGuildWarehouseBar(const RwChar* pName);
	virtual ~CGuildWarehouseBar();

	static VOID		CreateInstance();
	static VOID		DestroyInstance();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);		///< DialogManager에서의 Open/Close

protected:
	CGuildWarehouseBar() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Clicked_1_Button(gui::CComponent* pComponent);
	VOID			Clicked_2_Button(gui::CComponent* pComponent);
	VOID			Clicked_3_Button(gui::CComponent* pComponent);
	VOID			ClickedAllButton(gui::CComponent* pComponent);
	VOID			ClickedCloseButton(gui::CComponent* pComponent);

	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	static CGuildWarehouseBar*	m_pInstance;

	gui::CSlot			m_slotWarehouseBtn[NTL_MAX_GUILD_BANK_COUNT];
	gui::CSlot			m_slot_All_Button;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CButton*		m_pWarehouseBtn[NTL_MAX_GUILD_BANK_COUNT];///< 창고 버튼
	gui::CButton*		m_p_All_Button;		///< 모든 창고 버튼
	gui::CButton*		m_pExitButton;		///< 창닫기 버튼

	CGuildWarehouseGui*		m_pWareHouseGui[NTL_MAX_GUILD_BANK_COUNT];
};