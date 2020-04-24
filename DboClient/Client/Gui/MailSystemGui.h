#pragma once

#include "NtlSLMailSystem.h"
#include "Windowby3.h"
#include "SlotGui.h"

#define dMAILSYSTEM_UI_SLOT_LEFT					16
#define dMAILSYSTEM_UI_SLOT_TOP						39
#define dMAILSYSTEM_UI_SLOT_GAP						63

#define dMAILSYSTEM_UI_SLOT_ICON_LEFT				4
#define dMAILSYSTEM_UI_SLOT_ICON_TOP				24

#define dMAILSYSTEM_UI_CALC_OFFSET					104

#define dMAILSYSTEM_REFRESH_TIME					60.0f

class CMailSlotGui;
class CMailSystemGui;
class CMailSystemReadGui;
class CMailSystemMsg;

/**
* \ingroup client
* \brief 메일에서 사용될 Message Box의 타입등을 정의한다.
*/
class CMailSystemMsg
{
public:
	enum eMAILSYSTEM_MSG_BUFFER
	{
		BUFFER_SAY_SIZE	= 1024,
		BUFFER_BOX_SIZE = 512,
	};

	enum eMAILSYSTEM_MSG_SAY_TYPE
	{
		MAIL_MSG_SAY_SEND_SUCCEED,
		MAIL_MSG_SAY_ATTACH_ZENNY,
		MAIL_MSG_SAY_MAIL_NOT_LOCK,

		MAIL_MSG_SAY_RECEIVE_ITEM,
		MAIL_MSG_SAY_RECEIVE_ZENNY,
		MAIL_MSG_SAY_RECEIVE_ZENNY_REQ,
		MAIL_MSG_SAY_RECEIVE_ACCEPT_MAIL,
		MAIL_MSG_SAY_ITEM_NOT_ATTACH,
	};

	enum eMAILSYSTEM_MSG_BOX_TYPE
	{
		MAIL_MSG_BOX_PLEASE_TARGETNAME,
		MAIL_MSG_BOX_PLEASE_MAILCONTENT,
		MAIL_MSG_BOX_SEND_BASIC,
		MAIL_MSG_BOX_SEND_ITEM,
		MAIL_MSG_BOX_SEND_ZENNY,
		MAIL_MSG_BOX_SEND_ITEM_ZENNY,
		MAIL_MSG_BOX_SEND_ITEM_ZENNY_REQ,
		MAIL_MSG_BOX_SEND_ZENNY_REQ,
		MAIL_MSG_BOX_AWAY_ON,
		MAIL_MSG_BOX_AWAY_OFF,
		MAIL_MSG_BOX_RECEIVE_ITEM,
		MAIL_MSG_BOX_RECEIVE_ZENNY,
		MAIL_MSG_BOX_RECEIVE_ITEM_ZENNY,
		MAIL_MSG_BOX_RECEIVE_ITEM_ZENNY_REQ,
		MAIL_MSG_BOX_RECEIVE_ZENNY_REQ,
		MAIL_MSG_BOX_DELETE,
		MAIL_MSG_BOX_RETURN,
		MAIL_MSG_BOX_SELECT_REQ_OR_SEND,
		MAIL_MSG_BOX_MAILCONTENT_SLANGDER,
	};

	static void			MsgSay(eMAILSYSTEM_MSG_SAY_TYPE eType, ...);	
	static void			MsgBox(eMAILSYSTEM_MSG_BOX_TYPE eType, ...);
	static void			ModReturnMsg(WCHAR* pwcBuffer, RwUInt32 uiSize, const sMAIL_PROFILE* pMailProfile);
	static void			ModReplyMsg(WCHAR* pwcBuffer, RwUInt32 uiSize, const sMAIL_PROFILE* pMailProfile);
	static const WCHAR*	GetItemName(const sMAIL_PROFILE* pMailProfile, RwBool bItemText);
	static const WCHAR*	GetItemName(TBLIDX itemTblidx);
};


/**
* \ingroup client
* \brief MailSystem에서의 Mail Slot등을 정의한다.
*/
class CMailSlotGui : public CNtlPLGui
{
	enum eMAILSYSTEM_SLOT_ICON
	{
		ICON_MAIL,
		ICON_MAIL_ATTACH,
		ICON_MAIL_READ,
		ICON_MAIL_ATTACH_READ,
		ICON_NUM,
	};

protected:
	gui::CButton*			m_pBtnSlot;
	gui::CStaticBox*		m_pStbName;
	gui::CStaticBox*		m_pStbContent;
	gui::CStaticBox*		m_pStbDay;
	gui::CButton*			m_pBtnLock;
	gui::CButton*			m_pBtnCheck;			///< 체크박스

	gui::CSlot				m_slotMove;
	gui::CSlot				m_slotPaintBtnSlot;
	gui::CSlot				m_slotClickedBtnSlot;
	gui::CSlot				m_slotClickedBtnLock;
	gui::CSlot				m_slotToggledBtnCheck;
	
	CSurfaceGui				m_IconMail[ICON_NUM];
	CSurfaceGui*			m_pIconMail;
	bool					m_bLock;
	bool					m_bCheck;
	sMAIL_PROFILE			m_sMailProfile;

public:
	CMailSlotGui(const RwChar* pName);
	virtual ~CMailSlotGui();

	RwBool		Create(gui::CComponent* pParent, RwInt32 iX, RwInt32 iY);
	RwBool		Create();
	void		Destroy();

	void		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	void		OnPaintBtnSlot();
	void		OnClickedBtnSlot(gui::CComponent* pComponent);
	void		OnClickedBtnLock(gui::CComponent* pComponent);
	void		OnToggledBtnCheck(gui::CComponent* pComponent, bool bDown);
	
	void		SetMailSlot(RwUInt32 uiPos, MAILID mailIDReading);
	void		ClearMailSlot();
	void		UpdateMailSlot();

	void		SetLock(bool bLock);
	RwBool		IsLock();
	RwBool		IsCheck();
	void		CheckClear();
	RwUInt32	GetMailID();
};

/**
* \ingroup client
* \brief Mail System GUI
*/
class CMailSystemGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eMAILSYSTEM_TAB
	{
		TAB_MAILBOX, TAB_SENDMAIL, TAB_NUM,
	};
	enum eMAILSYSTEM_SLOT
	{
		SLOT_NUM = 6,
	};

	enum eMAILSYSTEM_ATTACH
	{
		MAILSYSTEM_ATTACH_SEND,
		MAILSYSTEM_ATTACH_REQ,
	};

	enum eMAILSYSTEM_STAMP
	{
		MAILSYSTEM_STAMP_BASIC		= 10,
		MAILSYSTEM_STAMP_ATTACH		= 20,
	};

	enum eMAILSYSTEM_EXPIRED_DAY
	{
		MAILSYSTEM_EXPIRED_DAY_MIN	= 1,
		MAILSYSTEM_EXPIRED_DAY_MAX	= 10,
	};

protected:
	// MAIN
	gui::CStaticBox*	m_pStbTitle;
	gui::CButton*		m_pBtnClose;
	gui::CTabButton*	m_pTabButton;
	gui::CDialog*		m_apDlgTab[TAB_NUM];
			
	gui::CSlot			m_slotClickedBtnClose;
	gui::CSlot			m_slotChangedTabButton;
	gui::CSlot			m_slotCaptureMouseDown;
	
	// TAB : MAILBOX
	gui::CButton*		m_pBtnAway;
	gui::CStaticBox*	m_pStbPage;
	gui::CButton*		m_pBtnPrevPage;
	gui::CButton*		m_pBtnNextPage;
	gui::CButton*		m_pBtnReload;
	gui::CButton*		m_pBtnDelete;
	gui::CPanel*		m_pPanEmptyMail;
	gui::CStaticBox*	m_pStbEmptyMail;

	gui::CSlot			m_slotClickedBtnPrevPage;
	gui::CSlot			m_slotClickedBtnNextPage;
	gui::CSlot			m_slotClickedBtnReload;
	gui::CSlot			m_slotClickedBtnDelete;
	gui::CSlot			m_slotToggledBtnAway;

	RwReal				m_fReloadTime;

	// TAB : SENDMAIL
	gui::CStaticBox*	m_pStbDay;
	/*gui::CPanel*		m_pPnlIconDay;*/
	gui::CStaticBox*	m_pStbLeftDay;
	gui::CButton*		m_pBtnUpDay;
	gui::CButton*		m_pBtnDownDay;
	gui::CStaticBox*	m_pStbTargetNameTitle;
	gui::CInputBox*		m_pIpbTargetName;

	gui::CComboBox*		m_pcbbFriendList;
	gui::CSlot			m_slotSelectedFriend;

	gui::CStaticBox*	m_pStbMailContentTitle;
	gui::CInputBox*		m_pIpbMailContent;
	gui::CStaticBox*	m_pStbAttachTitle;
	gui::CButton*		m_pBtnAttachReq;
	gui::CButton*		m_pBtnAttachSend;
	gui::CButton*		m_pBtnAttachZenny;
	gui::CStaticBox*	m_pStbAttachZenny;
	gui::CStaticBox*	m_pStbAttachMessage;
	/*gui::CPanel*		m_pPnlIconZennySend;
	gui::CPanel*		m_pPnlIconZennyReq;*/
	gui::CButton*		m_pBtnSend;
	gui::CButton*		m_pBtnClear;
	gui::CDialog*		m_pDlgAttachItem;
	gui::CDialog*		m_pDlgAttach;
	/*gui::CStaticBox*	m_pStbStamp;*/
	gui::CStaticBox*	m_pStbMaxNumText;

	// CTexture
	gui::CTexture*		m_pMoneyIconTexture;

	gui::CSlot			m_slotKeyUpIpbMailContext;
	gui::CSlot			m_slotKeyUpIpbTargetName;
	gui::CSlot			m_slotCharChangedIpbMailCotent;

	gui::CSlot			m_slotGotFocusIpbMailContent;
	gui::CSlot			m_slotLostFocusIpbMailContent;
	gui::CSlot			m_slotGotFocusIpbTargetName;
	gui::CSlot			m_slotLostFocusIpbTargetName;
	gui::CSlot			m_slotClickedBtnUpDay;
	gui::CSlot			m_slotClickedBtnDownDay;
	gui::CSlot			m_slotToggledBtnAttachSend;
	gui::CSlot			m_slotToggledBtnAttachReq;
	gui::CSlot			m_slotClickedBtnAttachZenny;
	gui::CSlot			m_slotMoveDlgAttachItem;
	gui::CSlot			m_slotPaintDlgAttachItem;
	gui::CSlot			m_slotMouseDownDlgAttachItem;
	gui::CSlot			m_slotMouseUpDlgAttachItem;
	gui::CSlot			m_slotMouseEnterDlgAttachItem;
	gui::CSlot			m_slotMouseLeaveDlgAttachItem;
	gui::CSlot			m_slotPaintPostDlgAttach;
	gui::CSlot			m_slotMoveDlgAttach;


	gui::CSlot			m_slotClickedBtnSend;
	gui::CSlot			m_slotClickedBtnClear;

	CRegularSlotGui			m_AttachItemSlot;
	CSurfaceGui			m_EffectFocus;
	CSurfaceGui			m_AttachDisable;
	bool				m_bMailBoxFocus;
	
	CSurfaceGui			m_EffectZenny;
	RwBool				m_bEffectZenny;

	// UI
	RwUInt32			m_uiIndexTab[TAB_NUM];
	CMailSlotGui*		m_apMailSlot[SLOT_NUM];
	RwUInt32			m_uiAbilityType;
	MAILID				m_mailIDReading;

	// MAILBOX 
	RwUInt32			m_uiMailBoxPageCur;
	RwUInt32			m_uiMailBoxPageMax;
	bool				m_bMailBoxAway;

	// SENDMAIL
	eMAILSYSTEM_STAMP	m_eSendMailStamp;
	RwUInt32			m_uiSendMailDay;
	RwUInt32			m_uiSendMailZenny;
	sITEM_POSITION_DATA	m_sSendMailAttachItem;

	// OHTER
	WCHAR				m_awcTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];

public:
	CMailSystemGui(const RwChar* pName);
	virtual ~CMailSystemGui();

	RwBool		Create();
	RwBool		CreateSendMail(gui::CComponent* pParent);
	RwBool		CreateMailBox(gui::CComponent* pParent);
	void		Destroy();

	void		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);
	void		HandleEvents(RWS::CMsg& msg);

	// MAIN
	void		OnClickedBtnClose(gui::CComponent* pComponent);
	void		OnChangedTabButton(RwInt32 nIndex, RwInt32 nOldIndex);
	void		OnCaptureMouseDown(const CKey& key);

	// TAB : MAILBOX
	void		OnClickedBtnPrevPage(gui::CComponent* pComponent);
	void		OnClickedBtnNextPage(gui::CComponent* pComponent);
	void		OnClickedBtnReload(gui::CComponent* pComponent);
	void		OnClickedBtnDelete(gui::CComponent* pComponent);
	void		OnToggledBtnAway(gui::CComponent* pComponent, bool bToggled);

	// TAB : SENDMAIL
	void		OnGotFocus();
	void		OnLostFocus();

	void		OnKeyUp(gui::CComponent* pComponenet, CInputDevice* pDevice, const CKey& key);
	void		OnCharChangedIpbMailContent(int iLength);

	void		OnClickedBtnUpDay(gui::CComponent* pComponent);
	void		OnClickedBtnDownDay(gui::CComponent* pComponent);
	void		OnToggledBtnAttachSend(gui::CComponent* pComponent, bool bDown);
	void		OnToggledBtnAttachReq(gui::CComponent* pComponent, bool bDown);
	void		OnClickedBtnAttachZenny(gui::CComponent* pComponent);
	void		OnMoveDlgAttachItem(RwInt32 iOldX, RwInt32 iOldY);
	void		OnPaintDlgAttachItem();
	void		OnMoveDlgAttach(RwInt32 iOldX, RwInt32 iOldY);
	void		OnPaintPostDlgAttach();
	void		OnMouseDownDlgAttachItem(const CKey& key);
	void		OnMouseUpDlgAttachItem(const CKey& key);
	void		OnMouseEnterDlgAttachItem(gui::CComponent* pComponent);
	void		OnMouseLeaveDlgAttachItem(gui::CComponent* pComponent);

	void		OnSelectedFriend(INT nSelectIndex);

	void		OnClickedBtnSend(gui::CComponent* pComponent);
	void		OnClickedBtnClear(gui::CComponent* pComponent);

private:
	// OTHER
	void		OpenUserInterface(RwUInt32 uiAbilityType);
	void		ClearUserInterface();
	void		UpdateUserInterface();

	// TAB : MAILBOX
	void		SetMailBoxPage(RwUInt32 uiPage);
	void		SetMailBoxAway(bool bAway);

	void		OpenMailBox();
	void		ClearMailBox();
	void		UpdateMailBox();

	// TAB : SENDMAIL
	void		SetSendMailDay(RwUInt32 uiDay);
	void		SetSendMailItem(SERIAL_HANDLE hItem, RwUInt8 uiPlace = 0, RwUInt8 uiPos = 0, RwUInt8 uiStackCount = 0);
	void		SetSendMailZenny(bool bIsAttachSend, bool bIsAttachReq, RwUInt32 uiZenny = 0);
	void		SetSendMailStamp(eMAILSYSTEM_STAMP eStamp);
	void		SetSendMailTargetName(WCHAR* pwcTargetName);

	RwUInt8		GetSendMailType();
	void		CalcSendMailStamp();
	void		CalcSendMailContentMaxNum();

	void		SendMail();

	void		SelectTab(RwUInt32 uiTabIndex);

	void		OpenSendMail();
	void		ClearSendMail();
	void		UpdateSendMail();

	void		ShowInfoWindow(RwBool bIsShow);
};

/**
* \ingroup client
* \brief MailSystem Read GUI
*/
class CMailSystemReadGui : public CNtlPLGui, public RWS::CEventHandler
{
protected:
	gui::CStaticBox*	m_pStbTitle;
	gui::CButton*		m_pBtnClose;
	gui::CTabButton*	m_pTabButton;
	gui::CStaticBox*	m_pStbDay;
	gui::CPanel*		m_pPnlIconDay;
	gui::CStaticBox*	m_pStbTargetNameTitle;
	gui::CInputBox*		m_pIpbTargetName;
	gui::CStaticBox*	m_pStbMailContentTitle;
	gui::CInputBox*		m_pIpbMailContent;
	gui::CStaticBox*	m_pStbAttachTitle;
	gui::CStaticBox*	m_pStbAttachZenny;
	gui::CPanel*		m_pPnlIconZennySend;
	gui::CPanel*		m_pPnlIconZennyReq;
	gui::CDialog*		m_pDlgAttachItem;
	gui::CStaticBox*	m_pStbAttachCategory;
	gui::CButton*		m_pBtnAttachAction;
	gui::CStaticBox*	m_pStbAttachMessage;
	gui::CDialog*		m_pDlgAttach;
	gui::CButton*		m_pBtnReply;
	gui::CButton*		m_pBtnReturn;
	gui::CButton*		m_pBtnDelete;
	gui::CDialog*		m_pDlgLock;
	gui::CStaticBox*	m_pStbSendDate;

	gui::CSlot			m_slotClickedBtnClose;
	gui::CSlot			m_slotMoveDlgAttachItem;
	gui::CSlot			m_slotPaintDlgAttachItem;
	gui::CSlot			m_SlotMouseEnterDlgAttachItem;
	gui::CSlot			m_SlotMouseLeaveDlgAttachItem;
	gui::CSlot			m_slotClickedBtnReply;
	gui::CSlot			m_slotClickedBtnReturn;
	gui::CSlot			m_slotClickedBtnDelete;
	gui::CSlot			m_slotClickedBtnAttachAction;
	gui::CSlot			m_slotMoveDlgAttach;
	gui::CSlot			m_slotPaintPostDlgAttach;
	gui::CSlot			m_slotMoveBtnAttachAction;
	gui::CSlot			m_slotPaintBtnAttachAction;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CFlickerEffect	m_feAttachAction;

	CRegularSlotGui			m_AttachItemSlot;
	CSurfaceGui			m_AttachItemAccept;
	CSurfaceGui			m_AttachDisable;
	sMAIL_PROFILE		m_sMailProfile;
	bool				m_bAttachAccept;
	MAILID				m_mailIDTemp;
	RwUInt32			m_uiAbilityType;

public:
	CMailSystemReadGui(const RwChar* pName);
	virtual ~CMailSystemReadGui();

	RwBool		Create();
	void		Destroy();

	RwInt32		SwitchDialog(bool bOpen);
	void		HandleEvents(RWS::CMsg& msg);

	void		OnClickedBtnClose(gui::CComponent* pComponent);

	void		OnMoveDlgAttachItem(RwInt32 iOldX, RwInt32 iOldY);
	void		OnPaintDlgAttachItem();
	void		OnMouseEnterDlgAttachItem(gui::CComponent* pComponenet);
	void		OnMouseLeaveDlgAttachItem(gui::CComponent* pComponenet);
	void		OnMoveDlgAttach(RwInt32 iOldX, RwInt32 iOldY);
	void		OnPaintPostDlgAttach();
	void		OnMoveBtnAttachAction(RwInt32 iOldX, RwInt32 iOldY);
	void		OnPaintBtnAttachAction();
	void		OnClickedBtnReply(gui::CComponent* pComponent);
	void		OnClickedBtnReturn(gui::CComponent* pComponent);
	void		OnClickedBtnDelete(gui::CComponent* pComponent);
	void		OnClickedBtnAttachAction(gui::CComponent* pComponent);
	void		OnCaptureMouseDown(const CKey& key);

private:
	void		OpenReadMail(const MAILID& mailID);
	void		ClearReadMail();
	void		UpdateReadMail();

	void		SetReadMailLock(RwBool bIsLock);
	void		SetReadMailContent(WCHAR* pwcMailContent);

	void		DisplayReadMailInit();
	void		DisplayReadMailBasic();
	void		DisplayReadMailItem();
	void		DisplayReadMailZenny();
	void		DisplayReadMailZennyReq();

	void		CheckAttachEnable();
	void		CheckAttachActionEnable();
	void		CheckReplyEnable();
	void		CheckReturnEnable();
	void		CheckDeleteEnable();
	void		CheckAttachActionEffect();

	void		RunableAttachActionEffect(RwBool bRunable);

	void		ShowInfoWindow(RwBool bIsShow);

	//CNtlPLGui
	void		Update(RwReal fElapsed);
};