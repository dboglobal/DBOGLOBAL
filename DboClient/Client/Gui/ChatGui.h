/*****************************************************************************
 *
 * File			: ChatGui.h
 * Author		: Peessi
 * Copyright	: (주)NTL
 * Date			: 2007. 5. 10	
 * Abstract		: DBO chatting gui.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __CHAT_GUI_H__
#define __CHAT_GUI_H__

#include "NtlPLGui.h"
#include "SurfaceGui.h"
#include "ceventhandler.h"
#include "Windowby3.h"
#include "ChatDefine.h"


class CChatOptionGui;
class CChatDisplayGui;
class CChatReceiver;
class CChatSender;

//

class CChatGui : public CNtlPLGui , public RWS::CEventHandler
{
public:
	typedef std::list<CChatDisplayGui*>					LIST_DISPLAY_GUI;

	struct sChatOption
	{
		RwBool				bAlwaysInput;				///< 채팅입력후 입력모드가 유지되는지 여부
		bool				bLockChatDisplay;
		bool				bShowChatScroll;
		RwUInt8				byAlpha;
	};

	struct sPositionInfo
	{
		RwInt32				iOldScreenHeight;			///< 클라이언트 Resize시 하단에서 일정한 거리 유지 위해
	};


	//! Constructor & Destructor	
	CChatGui( const RwChar* pName );
	~CChatGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog(bool bOpen);
	RwInt32 ActionMapTabKey(VOID);
	RwInt32 ActionMapChangeTab(RwUInt32 uiOption);

	RwBool	IsFocused(VOID);

	VOID	FocusToInput(VOID);

	VOID	ShowChatButton(eChatType eType, bool bShow);

	VOID	SetChatType(eChatType eType, const WCHAR* wchReceiverName = NULL);
	eChatType GetChatType();

	VOID			SetLastRecieveWhisperName(WCHAR* pwcName);
	std::wstring&	GetLastRecieveWhisperName(VOID) { return m_wstrLastRecieveWhisterName; }

	VOID			SetLastSentWhisperName(WCHAR* pwcName);
	std::wstring&	GetLastSentWhisperName() { return m_wstrLastSentWhisperName; }

	CChatReceiver*	GetReceiver() { return m_pChatReceiver; }

	VOID	SetDismovableDisplayPositioin(RwBool bForce = FALSE);
	
	//! Event
	virtual VOID HandleEvents( RWS::CMsg& msg );

private:
	CChatGui(VOID) {}

	RwBool	AddDisplayGui(BYTE byIndex);
	VOID	RemoveDisplayGui(RwUInt8 byDisplayIndex);	
	VOID	ShowChatGui(bool bShow);
	VOID	SetComponentPosition();

	// Chat option
	VOID	Option_AlwaysInput(bool bEnable);
	VOID	Option_LockChatDisplay(bool bEnable);
	VOID	Option_MoreDisplayGui(BYTE byIndex);
	VOID	Option_LessDisplayGui(BYTE byIndex);
	VOID	Option_Alpha(RwUInt8 byAlpha);
	VOID	Option_DisplayChatLogType(RwUInt8 byDisplayIndex, eChatType eType, RwBool bEnable);

	// Option_DisplayChatLogType 함수를 모두 호출한 뒤에 그 결과를 적용하기 위한 함수
	VOID	Option_RefreshDisplayChatLog(RwUInt8 byDisplayIndex);

	//! Callbacks
	VOID	OnGotFocus(VOID);
	VOID	OnLostFocus(VOID);
	VOID	OnInputReturn(VOID);
	void	OnInputSpaceUp();
	VOID	OnOptionButton( gui::CComponent* pComponent );	
	VOID	OnChatTypeButton( gui::CComponent* pComponent );
	VOID	OnModeSelBtnClick( gui::CComponent* pComponent );
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnResize( RwInt32 iOldW, RwInt32 iOldH );
	VOID	OnESCKeyUp(VOID);
	VOID	OnPaint(VOID);

	void	HideInput();

protected:
	gui::CSlot	m_slotGotFocus;
	gui::CSlot	m_slotLostFocus;
	gui::CSlot	m_slotReturn;
	gui::CSlot	m_slotSpaceUp;
	gui::CSlot	m_slotOption;
	gui::CSlot	m_slotChatButton[NUM_NET_CHAT_TYPE];
	gui::CSlot	m_slotModeSelBtnClick;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotResize;
	gui::CSlot  m_slotIMEChange;
	gui::CSlot	m_slotESCKeyUp;
	gui::CSlot	m_slotPaint;

	gui::CDialog*		m_pManagerDialog;
	gui::CButton*		m_pSelBtn;
	gui::CButton*		m_pOptionBtn;
	gui::CButton*		m_pChatTypeButton[NUM_NET_CHAT_TYPE];
	gui::CPanel*		m_pChatTypePanel[NUM_NET_CHAT_TYPE];
	gui::CStaticBox*	m_pIME;
	gui::CInputBox*		m_pInput;

	gui::CStaticBox*	m_pstbTextType;
	gui::CStaticBox*	m_pstbTextTypeSmall;
	
	CWindowby3			m_InputBackground;

	//! Variables
	LIST_DISPLAY_GUI	m_listDisplayGui;
	CChatOptionGui*		m_pChatOptionGui;	
	CChatReceiver*		m_pChatReceiver;
	CChatSender*		m_pSender;

	std::wstring		m_wstrLastSentWhisperName;	// Character name we last sent a whisper to.
	std::wstring		m_wstrLastRecieveWhisterName;	// Character name we last received a whisper from.

	RwBool				m_bChatModeEndReturn;
	RwBool				m_bRenderBackground;
	RwBool				m_bInitializedOption;			/// 옵션에 따른 초기화를 했다
	eChatType			m_eChatType;
	sChatOption			m_ChatOption;
	sPositionInfo		m_tPositionInfo;	
};

#endif