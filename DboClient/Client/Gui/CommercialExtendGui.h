#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "surfaceGui.h"
#include "Windowby3.h"
#include "NtlItem.h"
#include "NtlSobItem.h"
/**
* \ingroup Client
* \brief 상용화관련 기간 연장 처리,  기간연장확인3(제니, 넷피, 캐쉬) + 구매확인(캐쉬)
*  크기 변동 dialog 캐쉬일때 size가 default, 제니, 넷피서 size가 줄어듬
*
*	CCommercialExtendGui는 기본적으로 기간 연장 처리용 Gui지만
*	
*	제일 먼저 기간제 아이템의 최초 구매 이벤트를 처리하는 가장 중요한  entry point
*
* 
* \date 2009-08-14
* \author woosung_test
*/

class sITEM_TBLDAT;

class CCommercialExtendMode
{
public:
	enum	GUI_SIZE_MODE		{ SIZE_DEFAULT, SIZE_DETAIL_DESC_DELETED	};
	enum	GUI_ARRANGE_MODE	{ DEFAULT_BUTTONS, CASH_EXTEND_BUTTONS		};

	class CAttributeControlSet
	{
	public:
		GUI_SIZE_MODE		eSizeMode;
		GUI_ARRANGE_MODE	eArrangeMode;
		int					iDefaultWndHeight;
		int					iDefaultDescript1Top;
		int					iDefaultCancelButtLeft;
		/// 외부 link
		gui::CDialog*		pDialog;						// dialog 크기 변경
		gui::CStaticBox*	pStatic_stbTitle;				// title변경 가능
		gui::CStaticBox*	pStatic_stbConsumeGoldTitle;
		gui::CStaticBox*	pStatic_stbMyGoldTitle;
		gui::CStaticBox*	pStatic_stbRemainTime;
		gui::CStaticBox*	pStatic_stbAddableTime;
		gui::CStaticBox*	pStatic_stbConsumeGold;
		CSurfaceGui*		pSurConsumeGoldIcon;			// 텍스처 바뀔 수 있음
		gui::CStaticBox*	pStatic_stbMyGold;
		CSurfaceGui*		pSurMyGoldIcon;					// 텍스처 바뀔 수 있음
		gui::CPanel*		pPanel_pnlPanel2;
		gui::COutputBox*		pInput_opbDescriptDetail;
		gui::CStaticBox*	pStatic_stbDescript1;			// 변화 가
		gui::CStaticBox*	pStatic_stbDescript2;			// 변화 가
		gui::CButton*		pButt_btnConfirm;				// 숨겨질수 있음
		gui::CButton*		pButt_btnBuyDuration;			// 숨겨짐
		gui::CButton*		pButt_btnRefillCash;			// 숨겨짐
		gui::CButton*		pButt_btnCancel;				// 위치 변경
	};
protected:
	CAttributeControlSet	m_AttributeControlSet;
	CNtlSobItem*			m_pTargetNtlSobItem;			// 연장위한 Target item

	RwUInt32				m_uiConsumeGold;
	RwUInt32				m_uiMyGold;

public:
	virtual void			Init() = 0;
	virtual RwBool			ConFirm() = 0;
	virtual void			Cancel() = 0;

	virtual void			CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	virtual void			SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);

	void					InitShowComponent();
};

class CCommercialExtendZennyMode : public CCommercialExtendMode
{
public:
	void					Init();
	RwBool					ConFirm();
	void					Cancel();

	void					CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	void					SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);
};

class CCommercialExtendNetphyMode : public CCommercialExtendMode
{
public:
	void					Init();
	RwBool					ConFirm();
	void					Cancel();

	void					CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	void					SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);
};

class CCommercialExtendCashMode : public CCommercialExtendMode
{
public:
	void					Init();
	RwBool					ConFirm();
	void					Cancel();

	void					Buy();
	void					Refill();

	void					CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	void					SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);
};

class CCommercialExtendCashBuyMode : public CCommercialExtendMode
{
public:
	void					Init();
	RwBool					ConFirm();
	void					Cancel();

	void					CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet);
	void					SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT);
};

class CCommercialExtendGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enumuration
	enum	GUI_EXTEND_MODE		{ ZENNY_EXTEND, NETPY_EXTEND,    CASH_EXTEND, CASH_BUY,	NUM_EXTEND_MODE };

protected:

	GUI_EXTEND_MODE				m_eCurrExtendGuiMode;	/// 이 gui는 4개의 mode로 동작하게 되어있다
	CCommercialExtendMode*		m_pArrayCommercialExtendMode[NUM_EXTEND_MODE];

	RwUInt32					m_uiDurItemIdx;				/// 거래할 기간제 아이템
		
	CNtlSobItem*				m_pTargetNtlSobItem;		/// 기간연장할 아이템
	sITEM_TBLDAT*				m_pCurrITEM_TBLDA;			/// 기간연장을 위한 item의 정보 ref


	class CDataSendBuyDurItemNPC
	{
	public:
		RwUInt32				m_uiNPCSerial;					/// NPC Shop		/// ZENNY_EXTEND	
		sSHOP_BUY_CART			m_ShopBuyCart;										/// ZENNY_EXTEND
	};
	class CDataSendBuyDurItemNetpy
	{
	public:
		RwUInt32				m_uiMerchantTblidx;				/// NETPY Shop		/// NETPY_EXTEND	
		BYTE					m_byMerchanSlotPos;									/// NETPY_EXTEND	
	};
	class CDataSendMoveDurItemFromYardrat
	{
	public:
		RwUInt64				m_uiProductId;					/// Yardrat 창고용		/// CASH_EXTEND, CASH_BUY
	};
	typedef union
	{
		CDataSendBuyDurItemNPC			DataSendBuyDurItemNPC;
		CDataSendBuyDurItemNetpy		DataSendBuyDurItemNetpy;
		CDataSendMoveDurItemFromYardrat	DataSendMoveDurItemFromYardrat;
	} UNI_NET_SEND;

	UNI_NET_SEND				m_NetSendData;		

public:

	CCommercialExtendGui( const RwChar* pName );
	~CCommercialExtendGui(VOID);

	//! Operation
	VOID						Init(VOID);
	virtual RwBool				Create(VOID);
	virtual VOID				Destroy(VOID); 

	VOID						UpdateSurfacePos();

	RwInt32						SwitchDialog(bool bOpen);
	
	VOID						OpenDialog(GUI_EXTEND_MODE eMode, sITEM_TBLDAT* pITEM_TBLDAT, RwUInt32 uiItemIdx);
	VOID						OpenDialog();

	VOID						StartBuyDurItemProcessNPC( RwUInt32 uiItemIdx, RwUInt32 uiNPCSerial, sSHOP_BUY_CART& ShopBuyCart ); /// NPC, 
	VOID						StartBuyDurItemProcessNetpy( RwUInt32 uiItemIdx, RwUInt32 uiMerchantTblidx, BYTE byMerchanSlotPos ); /// Netpy Shop
	sITEM_TBLDAT*				StartBuyDurItemProcessCommonForItem( RwUInt32 uiItemIdx );
	VOID						StartMoveDurItemProcessCash( RwUInt32 uiItemIdx, RwUInt32 uiProductId );						/// Yardrat
	// Net
	VOID						NetSendBuyDurItemNPC();				/// NPC Shop
	VOID						NetSendBuyDurItemNetpy();			/// Netpy Shop
	VOID						NetSendMoveDurItemFromYardrat();	/// Yardrat

	//! Event
	virtual VOID				HandleEvents( RWS::CMsg& msg );
	VOID						HandleEventsSubMsgBox( RWS::CMsg& msg );

	//! Access
	VOID						SetDurationItem(RwUInt32 uiItemIdx);
	VOID						SetExtendTargetItem(CNtlSobItem* pNtlSobItem);
	VOID						SetCurrItemTbl(RwUInt32 uiItemIdx);
	VOID						SetCurrItemTbl(sITEM_TBLDAT* pITEM_TBLDAT);	/// 모든 item 관련 정보 setting
																			/// setting전에 GUI_EXTEND_MODE가 결정되어 있어야 함
protected:
	//! Implementation
	VOID						ChangeExtendMode(GUI_EXTEND_MODE eMode);
	RwBool						SetIcon(const RwChar* pcIconName);

	VOID						OnMouseEnter_pnlItemButt(gui::CComponent* pComponent);			
	VOID						OnMouseLeave_pnlItemButt(gui::CComponent* pComponent);
	VOID						OnClicked_btnConfirm(gui::CComponent* pComponent);
	VOID						OnClicked_btnCancel(gui::CComponent* pComponent);
	VOID						OnClicked_btnBuyDuration(gui::CComponent* pComponent);
	VOID						OnClicked_btnRefillCash(gui::CComponent* pComponent);
	VOID						OnClicked_btnClose(gui::CComponent* pComponent);
	VOID						OnPaint(VOID);
	VOID						OnMove(int nX,int nY);

	// Gui Components
	gui::CStaticBox*			m_pStatic_stbTitle;					// title변경 가능
	gui::CPanel*				m_pPanel_pnlItemButt;
	//gui::CPanel*				m_pPanel_pnlPanel1;
	gui::CStaticBox*			m_pStatic_stbRemainTimeTitle;
	gui::CStaticBox*			m_pStatic_stbAddableTimeTitle;
	gui::CStaticBox*			m_pStatic_stbConsumeGoldTitle;
	gui::CStaticBox*			m_pStatic_stbMyGoldTitle;
	gui::CStaticBox*			m_pStatic_stbRemainTime;
	gui::CStaticBox*			m_pStatic_stbAddableTime;
	gui::CStaticBox*			m_pStatic_stbConsumeGold;
	gui::CPanel*				m_pPanel_pnlPanelConsumeGoldIcon;	
	gui::CStaticBox*			m_pStatic_stbMyGold;
	gui::CPanel*				m_pPanel_pnlPanelMyGoldIcon;		

	gui::CPanel*				m_pPanel_pnlPanel2;					// 숨겨질수 있음
	gui::COutputBox*			m_pInput_opbDescriptDetail;

	gui::CPanel*				m_pPanel_pnlPanelName;
	gui::CStaticBox*			m_pStatic_stbItemName;
	gui::CStaticBox*			m_pStatic_stbDescript1;				// 변화 가
	gui::CStaticBox*			m_pStatic_stbDescript2;				// 변화 가
	gui::CButton*				m_pButt_btnConfirm;					// 숨겨질수 있음
	gui::CButton*				m_pButt_btnCancel;					// 위치변경
	gui::CButton*				m_pButt_btnBuyDuration;				// 숨겨짐
	gui::CButton*				m_pButt_btnRefillCash;				// 숨겨짐
	gui::CButton*				m_pButt_btnClose;

	// Variable
	CSurfaceGui					m_surItemIconSlot;
	CSurfaceGui					m_surEnterSlot;
	CSurfaceGui					m_surConsumeGoldIcon;				// 텍스처 바뀔 수 있음
	CSurfaceGui					m_surMyGoldIcon;					// 텍스처 바뀔 수 있음

	CWindowby3					m_DialogSurface;					///< 크기 변경 가능한 dialog이미지

	// Gui Signals
	gui::CSlot					m_slotGotFocus_pnlItemButt;
	gui::CSlot					m_slotLostFocus_pnlItemButt;
	gui::CSlot					m_slotClicked_btnConfirm;
	gui::CSlot					m_slotClicked_btnCancel;
	gui::CSlot					m_slotClicked_btnBuyDuration;
	gui::CSlot					m_slotClicked_btnRefillCash;
	gui::CSlot					m_slotClicked_btnClose;

	gui::CSlot					m_slotPaint_dlgMain;
	gui::CSlot					m_slotMove_dlgMain;

};