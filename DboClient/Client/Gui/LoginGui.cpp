#include "precomp_dboclient.h"
#include "LoginGui.h"

// shared
#include "NtlResultCode.h"

// core
#include "NtlDebug.h"
#include "NtlEventQueue.h"
#include "NtlSystemInfo.h"

// sound 
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "InputActionMap.h"
#include "NtlStorageManager.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "AlarmManager.h"
#include "LogInStageState.h"

#include "DboApplication.h"
#include "MoviePlayer.h"
#include "DialogDefine.h"


CLogInGui::CLogInGui(const RwChar *pName)
:CNtlPLGui(pName)
{
}

CLogInGui::~CLogInGui()
{
}

RwBool CLogInGui::Create()
{
	NTL_FUNCTION("CLogInGui::Create");

	if(!CNtlPLGui::Create("gui\\Login.rsr", "gui\\Login.srf", "gui\\Login.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager()); // this lags a bit because of flaCredit

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_pFrame = (gui::CFrame*)GetComponent("frmParent");

	// 배경
	m_pFlashBackground = (gui::CFlash*)GetComponent("flaBackground");

	m_pFlashCredit = (gui::CFlash*)GetComponent("flaCredit");
	m_pFlashCredit->Show(false);


	// 동영상 재생 버튼
	m_pCinemaButton = (gui::CButton*)GetComponent("CinemaButton");
	m_pCinemaButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pCinemaButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCinemaButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCinemaButton->SetText( GetDisplayStringManager()->GetString("DST_LOGIN_PLAY_MOVIE") );
	m_slotCinemaButton = m_pCinemaButton->SigClicked().Connect( this, &CLogInGui::ClickedCinemaButton);

	// 제작진 보기 버튼
	m_pCreditButton = (gui::CButton*)GetComponent("CreditButton");
	m_pCreditButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pCreditButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCreditButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCreditButton->SetText( GetDisplayStringManager()->GetString("DST_LOGIN_CREDIT") );
	m_slotCreditButton = m_pCreditButton->SigClicked().Connect( this, &CLogInGui::ClickedCreditButton);

	// Game Policy Button
	m_pGamePolicyButton = (gui::CButton*)GetComponent("GamePolicyButton");
	m_pGamePolicyButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pGamePolicyButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pGamePolicyButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pGamePolicyButton->SetText(GetDisplayStringManager()->GetString("DST_LOGIN_CONTRACT"));
	m_slotGamePolicyButton = m_pGamePolicyButton->SigClicked().Connect(this, &CLogInGui::ClickedGamePolicyButton);

	// Create new account Button
	m_pNewAccountButton = (gui::CButton*)GetComponent("NewAccountButton");
	m_pNewAccountButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pNewAccountButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pNewAccountButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pNewAccountButton->SetText( GetDisplayStringManager()->GetString("DST_LOGIN_NEW_ACCOUNT") );
	m_slotNewAccountButton = m_pNewAccountButton->SigClicked().Connect( this, &CLogInGui::ClickedNewAccountButton);

	// Login button
	m_pLoginButton = (gui::CButton*)GetComponent("LoginButton");
	m_pLoginButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pLoginButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pLoginButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pLoginButton->SetText( GetDisplayStringManager()->GetString("DST_LOGIN") );
	m_slotLoginButton = m_pLoginButton->SigClicked().Connect( this, &CLogInGui::ClickedLoginButton);

	// Exit button
	m_pExitButton = (gui::CButton*)GetComponent("ExitButton");
	m_pExitButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pExitButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pExitButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pExitButton->SetText( GetDisplayStringManager()->GetString("DST_LOBBY_EXIT") );
	m_slotExitButton = m_pExitButton->SigClicked().Connect( this, &CLogInGui::ClickedExitButton);

	// Input box background
	m_srfInputDialogBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Login.srf", "srfInputDialogBack" ));

	// 공지사항 밑줄
	m_NoticeUnderLine.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Login.srf", "underLine" ));

	// '계정 이름' 스태틱
	rect.SetRectWH(417, 588, 60, 30);
	m_pAccountName = NTL_NEW gui::CStaticBox( rect, m_pFrame, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pAccountName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pAccountName->SetText( GetDisplayStringManager()->GetString("DST_LOGIN_ID") );
	m_pAccountName->Enable(false);

	// '비밀번호' 스태틱
	rect.SetRectWH(417, 616, 60, 30);
	m_pPassward = NTL_NEW gui::CStaticBox( rect, m_pFrame, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pPassward->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPassward->SetText( GetDisplayStringManager()->GetString("DST_LOGIN_PASSWARD") );
	m_pPassward->Enable(false);

	// 'Dbo 공지사항' 스태틱
	rect.SetRectWH(773, 436, 220, 30);
	m_pNotive = NTL_NEW gui::CStaticBox( rect, m_pFrame, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pNotive->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNotive->SetTextColor(RGB(255, 255, 255));
	m_pNotive->Enable(false);

	// Version
	rect.SetRectWH(920, 750, 100, 20);
	WCHAR awcBuffer[50] = L"";
	swprintf_s(awcBuffer, L"Ver %d.%d", CLIENT_LVER, CLIENT_RVER);
	m_pVersion = NTL_NEW gui::CStaticBox( rect, m_pFrame, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pVersion->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pVersion->SetTextColor(RGB(0, 0, 0));
	m_pVersion->SetText(awcBuffer);
	m_pVersion->Enable(false);

	// Account Input
	m_pAccountInput = (gui::CInputBox*)GetComponent( "AccountInput" );
	m_pAccountInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_pAccountInput->SetMaxLength(NTL_MAX_SIZE_USERID);
	m_slotEnterAccount = m_pAccountInput->SigReturnPressed().Connect(this, &CLogInGui::PressEnder_in_AccountBox);

	// Passward Input
	m_pPasswardInput = (gui::CInputBox*)GetComponent( "PasswardInput" );
	m_pPasswardInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_pPasswardInput->SetMaxLength(NTL_MAX_SIZE_USERPW);
	m_pPasswardInput->SetPasswordMode(TRUE);
	m_slotEnterPassward = m_pPasswardInput->SigReturnPressed().Connect(this, &CLogInGui::PressEnder_in_PasswarsBox);	


	// Locate Component
	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	// sig
	m_slotMove			= m_pThis->SigMove().Connect( this, &CLogInGui::OnMove );
	m_slotPaint			= m_pFlashBackground->SigPaint().Connect( this, &CLogInGui::OnPaint );
	m_creditPaint		= m_pFlashCredit->SigMovieEnd().Connect(this, &CLogInGui::OnCreditPaintEnd);


	GetNtlGuiManager()->AddUpdateFunc( this );

	// Input Handle
	GetInputActionMap()->SetActive( FALSE ); 
	GetInputActionMap()->LinkTabKey( this, &CLogInGui::TabButtonHandle );

	m_handleKeyDown = CInputHandler::GetInstance()->LinkKeyDown(this, &CLogInGui::KeyboardDownHandler);

	// event 등록.
	LinkMsg(g_EventLoginGuiEnable, 0);
	LinkMsg(g_EventLogInStageStateEnter, 0);
	LinkMsg(g_EventLogInStageStateExit, 0);
	LinkMsg(g_EventLogInStageTimeOut, 0);
	LinkMsg(g_EventResize, 0);	

	Show(false);

	NTL_RETURN(TRUE);
}


VOID CLogInGui::Destroy()
{
	NTL_FUNCTION("CLogInGui::Destroy");

	NTL_DELETE(m_pAccountName);
	NTL_DELETE(m_pPassward);
	NTL_DELETE(m_pNotive);
	NTL_DELETE(m_pVersion);

	m_pFlashBackground->Unload();
	m_pFlashCredit->Unload();
	
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	GetInputActionMap()->SetActive( TRUE ); 
	GetInputActionMap()->UnLinkTabKey();

	CInputHandler::GetInstance()->UnLinkKeyDown(m_handleKeyDown);

	UnLinkMsg(g_EventLoginGuiEnable);
	UnLinkMsg(g_EventLogInStageStateEnter);
	UnLinkMsg(g_EventLogInStageStateExit);
	UnLinkMsg(g_EventLogInStageTimeOut);	
	UnLinkMsg(g_EventResize);	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CLogInGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	if (m_pFlashCredit->IsVisible())
		m_pFlashCredit->Update(fElapsed);
	else
		m_pFlashBackground->Update(fElapsed);
}

VOID CLogInGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	CRectangle rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = iWidth;
	rect.bottom = iHeight;

	m_pThis->SetPosition(rect);

	m_pFrame->SetPosition(rect);
	m_pFlashBackground->SetPosition(rect);
	m_pFlashCredit->SetPosition(rect);

	rect = m_pCinemaButton->GetPosition();

	m_pCinemaButton->SetPosition(rect.left, iHeight - 196);
	m_pCreditButton->SetPosition(rect.left, iHeight - 156);
	m_pGamePolicyButton->SetPosition(rect.left, iHeight - 116);
	m_pNewAccountButton->SetPosition(rect.left, iHeight - 70);

	m_pExitButton->SetPosition(iWidth - 176, iHeight - 116);

	m_srfInputDialogBack.SetPosition(iWidth/2 - m_srfInputDialogBack.GetWidth()/2, iHeight - 194);
	m_srfInputDialogBack.GetRect(rect);

	m_pLoginButton->SetPosition(rect.left + 39, rect.top + 83);

	m_pAccountName->SetPosition(rect.left + 16, rect.top + 20);
	m_pPassward->SetPosition(rect.left + 16, rect.top + 49);

	m_pAccountInput->SetPosition(rect.left + 91, rect.top + 28);
	m_pPasswardInput->SetPosition(rect.left + 91, rect.top + 59);


	m_pNotive->SetPosition(iWidth - 251, iHeight - 332);
	m_pVersion->SetPosition(iWidth - 104, iHeight - 18);
	m_NoticeUnderLine.SetPosition(iWidth - 251, iHeight - 311);

	if (IsShow())
	{
		if (GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pAccountInput)
		{
			// If you give focus to the same component again, it will return and write the shortcut
			m_pPasswardInput->SetFocus();
			m_pAccountInput->SetFocus();
		}
		else if (GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pPasswardInput)
		{
			m_pAccountInput->SetFocus();
			m_pPasswardInput->SetFocus();
		}
	}
}

VOID CLogInGui::SwitchEnableButtons(bool bEnable)
{

}

VOID CLogInGui::ClickedCinemaButton( gui::CComponent* pComponent )
{
	// Watch the video

	if( GetMoviePlayer()->Play(dOPENING_MOVIE, MOVIE_SIZE_RATIO_FULL) )
	{
		Logic_SetOptionMoviePlay();
		GetMoviePlayer()->SetCallBack(this, &CLogInGui::CallBackMoviePlayStop);
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_IMMEDIATELY_END_MAIN_THEME);
	}
}

int CLogInGui::CallBackMoviePlayStop()
{
	Logic_SetOptionMovieStop();
	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);
	return TRUE;
}

VOID CLogInGui::ClickedCreditButton( gui::CComponent* pComponent )
{
	m_pFlashCredit->Load("DBO_Credit.swf");

	m_pFlashCredit->RestartMovie();
	m_pFlashCredit->Show(true);

	m_pFrame->Show(false);
}

VOID CLogInGui::ClickedGamePolicyButton(gui::CComponent* pComponent)
{
	// Show game policy
	CDboEventGenerator::LoginEvent(LOGIN_EVENT_SHOW_CONTRACT_PAGE);
}

VOID CLogInGui::ClickedNewAccountButton( gui::CComponent* pComponent )
{
	// Create a new account	
}

VOID CLogInGui::ClickedLoginButton( gui::CComponent* pComponent )
{
	// Sign in

	if(	m_pAccountInput->GetLength() == 0 )
	{
		GetAlarmManager()->AlarmMessage( "DST_INPUT_YOUR_ID" );
		return;
	}

	if( m_pPasswardInput->GetLength() == 0 )
	{
		GetAlarmManager()->AlarmMessage( "DST_INPUT_YOUR_PASSWORD" );
		return;
	}
	
	SwitchEnableButtons(false);

	// Do not erase it (Fluorite)
	// Request login to server
	const WCHAR *pID = m_pAccountInput->GetText();
	const WCHAR *pPW = m_pPasswardInput->GetText();
	SUserData *pUserData = GetDboGlobal()->GetUserData(); 
	wcscpy_s(pUserData->wchUserID, NTL_MAX_SIZE_USERID + 1, pID);
	wcscpy_s(pUserData->wchPassword, NTL_MAX_SIZE_USERPW + 1, pPW);

	const BYTE* pMacAddress = GetSystemMacAddress();

	if( pMacAddress )
	{
		if (GetDboGlobal()->IsAcceptGameContract() // check if terms already accepted
			|| GetDboGlobal()->GetContractVerstion() == (RwUInt32)GetNtlStorageManager()->GetIntData(dSTORAGE_SYSTEM_ETC_CONTRACT))
		{
			GetDboGlobal()->GetLoginPacketGenerator()->SendLonInReq(pID, pPW, CLIENT_LVER, CLIENT_RVER, pMacAddress);
			GetLogInStageState()->ChangeState(LOGIN_STATE_LOGINREQ);
		}
		else
		{
			GetLogInStageState()->ChangeState(LOGIN_STATE_CONTRACT);
		}
	}
	else
	{		
		GetAlarmManager()->AlarmMessage(L"Fail read MAC address", CAlarmManager::ALARM_TYPE_LOBBY_MSGBOX);
	}
}

VOID CLogInGui::ClickedExitButton( gui::CComponent* pComponent )
{
	// Leave game
	PostQuitMessage(0);
}

VOID CLogInGui::PressEnder_in_AccountBox()
{
	if( !IsShow() )
		return;

	// When you press Enter on the account input, the focus moves to the password input
	m_pPasswardInput->SetFocus();
}

VOID CLogInGui::PressEnder_in_PasswarsBox()
{
	if( !IsShow() )
		return;

	// Enter in the password input is the same as pressing the login button
	ClickedLoginButton(NULL);
}

RwInt32 CLogInGui::TabButtonHandle()
{
	if( !IsShow() )
		return 1;

	if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pAccountInput )
	{
		m_pPasswardInput->SetFocus();
	}
	else if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pPasswardInput )
	{
		m_pAccountInput->SetFocus();
	}

	return 1;
}

VOID CLogInGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{

}

VOID CLogInGui::OnPaint()
{
	m_srfInputDialogBack.Render();
	m_NoticeUnderLine.Render();
}

VOID CLogInGui::OnCreditPaintEnd(gui::CComponent* pComponent)
{
	m_pFlashCredit->Show(false);
	m_pFrame->Show(true);
}

VOID CLogInGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CLogInGui::HandleEvents");

	if(msg.Id == g_EventLoginGuiEnable)
	{
		SwitchEnableButtons(true);
	}
	else if(msg.Id == g_EventLogInStageStateEnter)
	{
		LogInStageEnterEventHandler(msg);
	}
	else if(msg.Id == g_EventLogInStageStateExit)
	{
		LogInStageExitEventHandler(msg);
	}
	else if(msg.Id == g_EventLogInStageTimeOut)
	{
		LogInStageTimeOutEventHandler(msg);
	}
	else if(msg.Id == g_EventResize)
	{
		ResizeEventHandler(msg);
	}

	NTL_RETURNVOID();
}

VOID CLogInGui::LogInStageEnterEventHandler(RWS::CMsg &msg)
{
	SDboEventLogInStageStateEnter *pStageStateEnter = reinterpret_cast<SDboEventLogInStageStateEnter*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageStateEnter->byState;
	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		m_pLoginButton->ClickEnable(false);
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);
		m_pLoginButton->ClickEnable(false);
		m_pAccountInput->SetFocus();
		Show(true);		
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		GetAlarmManager()->AlarmMessage( "DST_CHAR_SERVER_CONNECT_FAIL" );
		break;
	case LOGIN_STATE_IDLE:
		m_pLoginButton->ClickEnable(true);
		break;
	case LOGIN_STATE_CONTRACT:
	case LOGIN_STATE_LOGINREQ:
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		m_pLoginButton->ClickEnable(false);
		break;
	}
}

VOID CLogInGui::LogInStageExitEventHandler(RWS::CMsg &msg)
{
	SDboEventLogInStageStateExit *pStageStateExit = reinterpret_cast<SDboEventLogInStageStateExit*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageStateExit->byState;
	//DBO_WARNING_MESSAGE("State: " << eState);
	switch(eState)
	{
	case LOGIN_STATE_LOGO:

		m_pLoginButton->ClickEnable(true);
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		m_pLoginButton->ClickEnable(true);
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		m_pLoginButton->ClickEnable(true);
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		m_pLoginButton->ClickEnable(true);
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		break;
	}
}

VOID CLogInGui::LogInStageTimeOutEventHandler(RWS::CMsg &msg)
{
	SDboEventLogInStageTimeOut *pStageTimeOut = reinterpret_cast<SDboEventLogInStageTimeOut*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageTimeOut->byState;
	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		break;
	case LOGIN_STATE_SERVER_CONNECT: 
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		break;
	}
}

VOID CLogInGui::ResizeEventHandler(RWS::CMsg &msg)
{
	SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );		
	LocateComponent(pPacket->iWidht, pPacket->iHeight);	
}

int CLogInGui::KeyboardDownHandler(unsigned int uiKeyData)
{
	SKeyData* pData = (SKeyData*)uiKeyData;

	if (pData->uiChar == NTL_KEY_ESCAPE)
	{
		if (m_pFlashCredit->IsVisible())
		{
			m_pFlashCredit->Unload();
			m_pFlashCredit->Show(false);
			m_pFrame->Show(true);
		}
	}

	return TRUE;
}
