#include "precomp_dboclient.h"
#include "DBOTitleGui.h"

// core
#include "NtlDebug.h"
#include "NtlSystemInfo.h"

// sound
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// client
#include "DisplayStringManager.h"
#include "MoviePlayer.h"
#include "DialogDefine.h"
#include "DboDef.h"
#include "DboGlobal.h"
#include "LoginStageState.h"
#include "DialogPriority.h"


CDBOTitleGui::CDBOTitleGui(const RwChar *pName)
:CNtlPLGui(pName)
{
}

CDBOTitleGui::~CDBOTitleGui()
{
}

RwBool CDBOTitleGui::Create()
{
	NTL_FUNCTION("CDBOTitleGui::Create");

	if(!CNtlPLGui::Create("gui\\DBOTitle.rsr", "gui\\DBOTitle.srf", "gui\\DBOTitle.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_DEFAULT );

	CRectangle rect;

	// 배경
	m_pFlashBackground = (gui::CFlash*)GetComponent("flaBackground");

	// 동영상 재생 버튼
	m_pCinemaButton = (gui::CButton*)GetComponent("CinemaButton");
	m_pCinemaButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pCinemaButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCinemaButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCinemaButton->SetText( GetDisplayStringManager()->GetString("DST_LOGIN_PLAY_MOVIE") );
	m_slotCinemaButton = m_pCinemaButton->SigClicked().Connect( this, &CDBOTitleGui::OnClicked_CinemaButton);

	// 약관 보기 버튼
	m_pContractButton = (gui::CButton*)GetComponent("ContractButton");
	m_pContractButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pContractButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pContractButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pContractButton->SetText( GetDisplayStringManager()->GetString("DST_LOGIN_CONTRACT") );
	m_slotContractButton = m_pContractButton->SigClicked().Connect( this, &CDBOTitleGui::OnClicked_ContractButton);

	// 게임 서버 접속 버튼
	m_pConnectGameServerButton = (gui::CButton*)GetComponent("ConnectGameServerButton");
	m_pConnectGameServerButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pConnectGameServerButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pConnectGameServerButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pConnectGameServerButton->SetText( GetDisplayStringManager()->GetString("DST_LOGIN") );
	m_slotConnectGameServerButton = m_pConnectGameServerButton->SigClicked().Connect( this, &CDBOTitleGui::OnClicked_ConnectGameServerButton);

	// 나가기 버튼
	m_pExitButton = (gui::CButton*)GetComponent("ExitButton");
	m_pExitButton->SetTextFont(DEFAULT_FONT, 105, DEFAULT_FONT_ATTR);
	m_pExitButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pExitButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pExitButton->SetText( GetDisplayStringManager()->GetString("DST_LOBBY_EXIT") );
	m_slotExitButton = m_pExitButton->SigClicked().Connect( this, &CDBOTitleGui::OnClicked_ExitButton);

	// Version
	rect.SetRectWH(920, 750, 100, 20);
	WCHAR awcBuffer[50] = L"";
	swprintf_s(awcBuffer, L"Ver %d.%d", CLIENT_LVER, CLIENT_RVER);
	m_pVersion = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pVersion->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pVersion->SetText(awcBuffer);
	m_pVersion->Enable(false);

	// 제목 서페이스
	m_srfTitle.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DBOTitle.srf", "srfTitle" ));

	// Locate Component
	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	// sig
	m_slotPaint			= m_pFlashBackground->SigPaint().Connect( this, &CDBOTitleGui::OnPaint );

	GetNtlGuiManager()->AddUpdateFunc( this );

	// event 등록.
	LinkMsg(g_EventLoginGuiEnable, 0);
	LinkMsg(g_EventLogInStageStateEnter, 0);
	LinkMsg(g_EventLogInStageStateExit, 0);
	LinkMsg(g_EventLogInStageTimeOut, 0);
	LinkMsg(g_EventLoginMessage, 0);	
	LinkMsg(g_EventResize, 0);	

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDBOTitleGui::Destroy()
{
	NTL_FUNCTION("CDBOTitleGui::Destroy");

	m_pFlashBackground->Unload();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg(g_EventLoginGuiEnable);
	UnLinkMsg(g_EventLogInStageStateEnter);
	UnLinkMsg(g_EventLogInStageStateExit);
	UnLinkMsg(g_EventLogInStageTimeOut);
	UnLinkMsg(g_EventLoginMessage);	
	UnLinkMsg(g_EventResize);	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDBOTitleGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pFlashBackground->Update(fElapsed);
}

VOID CDBOTitleGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
#define dADJUST_X_GAP	 (29)

	CRectangle rect;	

	rect.left = 0;
	rect.top = 0;
	rect.right = iWidth;
	rect.bottom = iHeight;

	m_pThis->SetPosition(rect);

	m_srfTitle.SetPosition((rect.GetWidth()  - m_srfTitle.GetWidth())/2,
		(rect.GetHeight() - m_srfTitle.GetHeight())/2 - 270);

	m_pFlashBackground->SetPosition(rect);

	rect = m_pCinemaButton->GetPosition();
	m_pCinemaButton->SetPosition(dADJUST_X_GAP, iHeight - 113);
	m_pContractButton->SetPosition(dADJUST_X_GAP, iHeight - 73);

	m_pConnectGameServerButton->SetPosition((iWidth - m_pConnectGameServerButton->GetWidth())/2,
		iHeight - 73);

	m_pExitButton->SetPosition(iWidth - m_pExitButton->GetWidth() - dADJUST_X_GAP, iHeight - 73);

	m_pVersion->SetPosition(iWidth - 104, iHeight - 18);
}

VOID CDBOTitleGui::SwitchEnableButtons(bool bEnable)
{

}

VOID CDBOTitleGui::OnClicked_CinemaButton( gui::CComponent* pComponent )
{
	if( GetMoviePlayer()->Play(dOPENING_MOVIE, MOVIE_SIZE_RATIO_FULL) )
	{
		Logic_SetOptionMoviePlay();
		GetMoviePlayer()->SetCallBack(this, &CDBOTitleGui::CallBackMoviePlayStop);
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_IMMEDIATELY_END_MAIN_THEME);
	}
}

int CDBOTitleGui::CallBackMoviePlayStop()
{
	Logic_SetOptionMovieStop();
	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);
	return TRUE;
}

VOID CDBOTitleGui::OnClicked_ContractButton( gui::CComponent* pComponent )
{
	// 약관 보기
	CDboEventGenerator::LoginEvent(LOGIN_EVENT_SHOW_CONTRACT_PAGE);
}

VOID CDBOTitleGui::OnClicked_ConnectGameServerButton( gui::CComponent* pComponent )
{
	// 게임 서버 접속하기
	SwitchEnableButtons(false);

	GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_CONNECT);
}

VOID CDBOTitleGui::OnClicked_ExitButton( gui::CComponent* pComponent )
{
	// 게임 나가기
	SUserData* pUserData = GetDboGlobal()->GetUserData();
	CLoginPacketGenerator* pLoginPacketGenerator = GetDboGlobal()->GetLoginPacketGenerator(); 

	pLoginPacketGenerator->SendLogOutReq(pUserData->wchUserID, false);

	PostQuitMessage(0);
}

VOID CDBOTitleGui::OnPaint()
{
	m_srfTitle.Render();
}

VOID CDBOTitleGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDBOTitleGui::HandleEvents");

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
	else if(msg.Id == g_EventLoginMessage)
	{
		SDboEventLoginMessage* pEvent = reinterpret_cast<SDboEventLoginMessage*>( msg.pData );

		if( LOGIN_EVENT_START_INTRO_MOVIE_PLAY == pEvent->byMessage )
		{
			m_pConnectGameServerButton->ClickEnable(true);
		}
		else if( LOGIN_EVENT_FINISH_LOADING_AND_MOVIE == pEvent->byMessage )
		{
			Show(true);
		}
	}
	else if(msg.Id == g_EventResize)
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );		
		LocateComponent(pPacket->iWidht, pPacket->iHeight);	
	}
}

VOID CDBOTitleGui::LogInStageEnterEventHandler(RWS::CMsg &msg)
{
	SDboEventLogInStageStateEnter *pStageStateEnter = reinterpret_cast<SDboEventLogInStageStateEnter*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageStateEnter->byState;

	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		m_pConnectGameServerButton->ClickEnable(false);
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		m_pConnectGameServerButton->ClickEnable(false);		
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		GetAlarmManager()->AlarmMessage( "DST_CHAR_SERVER_CONNECT_FAIL" );
		break;
	case LOGIN_STATE_IDLE:
		m_pConnectGameServerButton->ClickEnable(true);
		break;
	case LOGIN_STATE_LOGINREQ:
		m_pConnectGameServerButton->ClickEnable(false);
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		m_pConnectGameServerButton->ClickEnable(false);
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		m_pConnectGameServerButton->ClickEnable(false);
		break;
	}
}

VOID CDBOTitleGui::LogInStageExitEventHandler(RWS::CMsg &msg)
{
	SDboEventLogInStageStateExit *pStageStateExit = reinterpret_cast<SDboEventLogInStageStateExit*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageStateExit->byState;
	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		m_pConnectGameServerButton->ClickEnable(true);		
		break;
	case LOGIN_STATE_SERVER_CONNECT:
		m_pConnectGameServerButton->ClickEnable(true);		
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		m_pConnectGameServerButton->ClickEnable(true);
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		m_pConnectGameServerButton->ClickEnable(true);
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		break;
	}
}

VOID CDBOTitleGui::LogInStageTimeOutEventHandler(RWS::CMsg &msg)
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