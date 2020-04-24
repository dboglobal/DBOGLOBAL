#include "precomp_dboclient.h"
#include "CharStageState.h"

// core
#include "NtlDebug.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"

#include "DboGlobal.h"
#include "BackboardGui.h"
#include "CharCreateGui.h"
#include "CharSelectGui.h"
#include "ServerSelectGui.h"
#include "CharGuiGroup.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "AlarmManager.h"
#include "MsgBoxManager.h"
#include "LobbyManager.h"


namespace
{
	RwReal	fAlphaElapedTime = .1f;
	RwBool  bTempAlphaTutorial = false;

#define dGAME_SERVER_CONNECT_TIME		dREMAIN_FOR_SERVER_REPLY - dDELAY_BEFORE_VISIBLE_MESSAGEBOX

	enum eGUIAlphaChange
	{		
		GUI_MIN_ALPHA			= 10,
		GUI_MAX_ALPHA			= 255,
		GUI_FADE_IN_TIME		= 1,			// FADE IN 시간(단위 : 초)
		GUI_FADE_OUT_TIME		= 1,			// FADE OUT 시간(단위 : 초)
	};	
};

CCharStageState* CCharStageState::m_pInstance = NULL;

void CCharStageState::CreateInstance()
{
	NTL_ASSERT(m_pInstance == NULL, "CCharStageState::CreateInstance, Already instance created");
	m_pInstance = NTL_NEW CCharStageState;
}

void CCharStageState::DestroyInstance()
{
	NTL_DELETE(m_pInstance);
}

CCharStageState::CCharStageState()
:m_eState(CHAR_STATE_NONE)
,m_eReservateState(CHAR_STATE_NONE)
,m_pTurnOverBackboard(NULL)
,m_pCharCreateGui(NULL)
,m_pCharSelectGui(NULL)
,m_pServerSelectGui(NULL)
,m_fElapedTime(0.f)
,m_bReadyModel(false)
,m_bReadyWorld(false)
,m_bShowMessageBox(false)
,m_bSendGameEnterPacket(false)
{
	NTL_ASSERT(m_pInstance == NULL, "CCharStageState::CCharStageState");
	m_pInstance = this;

	for(RwInt32 i = 0; i < (RwInt32)CHAR_MAX_STATE; i++)
		m_fnUpdate[i] = NULL;

	m_fnUpdate[CHAR_STATE_SERVER_INFORMAION]	= &CCharStageState::FuncUpdateInformation;
	m_fnUpdate[CHAR_STATE_SERVER_ENTER]			= &CCharStageState::FuncUpdateServerEnter;
	m_fnUpdate[CHAR_STATE_SERVER_IDLE]			= &CCharStageState::FuncUpdateServerIdle;
	m_fnUpdate[CHAR_STATE_SERVER_CHANGE]		= &CCharStageState::FuncUpdateServerChange;
	m_fnUpdate[CHAR_STATE_SERVER_EXIT]			= &CCharStageState::FuncUpdateServerExit;

	m_fnUpdate[CHAR_STATE_MAKE_ENTER]			= &CCharStageState::FuncUpdateMakeEnter;
	m_fnUpdate[CHAR_STATE_MAKE_IDLE]			= &CCharStageState::FuncUpdateMakeIdle;	
	m_fnUpdate[CHAR_STATE_MAKE_EXIT]			= &CCharStageState::FuncUpdateMakeExit;
	m_fnUpdate[CHAR_STATE_MAKE_REQUEST]			= &CCharStageState::FuncUpdateMakeRequest;

	m_fnUpdate[CHAR_STATE_SELECT_INFOMATION]	= &CCharStageState::FuncUpdateSelectInfomation;
	m_fnUpdate[CHAR_STATE_SELECT_INFOMATION_FROM_SERVER_LIST]	= &CCharStageState::FuncUpdateSelectInfomationFromServerList;
	m_fnUpdate[CHAR_STATE_SELECT_ENTER]			= &CCharStageState::FuncUpdateSelectEnter;
	m_fnUpdate[CHAR_STATE_SELECT_IDLE]			= &CCharStageState::FuncUpdateSelectIdle;
	m_fnUpdate[CHAR_STATE_SELECT_DEL_CHAR]		= &CCharStageState::FuncUpdateSelectDelChar;
	m_fnUpdate[CHAR_STATE_SELECT_RECOVERY_CHAR]	= &CCharStageState::FuncUpdateSelectRecoveryChar;
	m_fnUpdate[CHAR_STATE_SELECT_RENAME_REQ]	= &CCharStageState::FuncUpdateSelectRenameReq;
	m_fnUpdate[CHAR_STATE_SELECT_EXIT]			= &CCharStageState::FuncUpdateSelectExit;

	m_fnUpdate[CHAR_STATE_RETURN_LOGIN]			= &CCharStageState::FuncUpdateReturnLogin;

	m_fnUpdate[CHAR_STATE_GAME_SERVER_WAIT_CHECK_REQ]	= &CCharStageState::FuncUpdateGameServerWaitCheckReq;
	m_fnUpdate[CHAR_STATE_CHARACTER_SELECT_REQ]	= &CCharStageState::FuncUpdateCharacterSelectReq;
	m_fnUpdate[CHAR_STATE_SUCCESS_ENTER_GAME]	= &CCharStageState::FuncUpdateSuccessEnterGame;
	m_fnUpdate[CHAR_STATE_WAITING_GAME_SERVER_RESPONSE]	= &CCharStageState::FuncUpdateWaitingGameServerResponse;	
	m_fnUpdate[CHAR_STATE_FAIL_ENTER]			= &CCharStageState::FuncUpdateFailEnter;

	m_fnUpdate[CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_ENTER]	= &CCharStageState::FuncUpdateImmediatelyReturnLoginEnter;
	m_fnUpdate[CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_IDLE]		= &CCharStageState::FuncUpdateImmediatelyReturnLoginIdle;
	m_fnUpdate[CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_EXIT]		= &CCharStageState::FuncUpdateImmediatelyReturnLoginExit;

	m_fnUpdate[CHAR_STATE_READY_ENTER_TUTORIAL]	= &CCharStageState::FuncUpdateReadyEnterTutorial;
	m_fnUpdate[CHAR_STATE_WAIT_FOR_TUTORIAL]	= &CCharStageState::FuncUpdateWaitforTutorial;
	m_fnUpdate[CHAR_STATE_WAIT_CANCEL]			= &CCharStageState::FuncUpdateWaitCancel;

	m_fnUpdate[CHAR_STATE_WAIT_GAME_SERVER_CONNECT]			= &CCharStageState::FuncUpdateWaitGameServerConnect;
	m_fnUpdate[CHAR_STATE_CANCEL_WAIT_GAME_SERVER_CONNECT]	= &CCharStageState::FuncUpdateCancelWaitGameServerConnect;

	m_fnUpdate[CHAR_STATE_END]					= &CCharStageState::FuncUpdateStateEnd;	
}

CCharStageState::~CCharStageState()
{
}

CCharStageState* CCharStageState::GetInstance(void)
{
	return m_pInstance;
}

void CCharStageState::RegisterGUI(CCharGuiGroup* pGuiGroup)
{
	m_pTurnOverBackboard	= pGuiGroup->m_pTurnOverBackboard;
	m_pCharCreateGui		= pGuiGroup->m_pCharCreate;
	m_pCharSelectGui		= pGuiGroup->m_pCharSelect;
	m_pServerSelectGui		= pGuiGroup->m_pServerSelect;
}

void CCharStageState::FuncUpdateInformation(RwReal fElapsed)
{
	if( GetLobbyManager()->IsWaitingServerListInfo() )
	{
		m_fElapedTime += fElapsed;

		if( m_fElapedTime > CDT_WAITFOR_SERVER_REPLY )
		{
			// 서버로부터 응답이 없습니다
			GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
		}

		m_pServerSelectGui->SwitchDialog(false);
	}
	else
	{
		ChangeState(CHAR_STATE_SERVER_IDLE);
	}	
}

void CCharStageState::FuncUpdateServerEnter(RwReal fElapsed)
{
	RwUInt8 byAlpha = UpdateAlpha(fElapsed, true);

	m_pTurnOverBackboard->SetAlpha((RwUInt8)(GUI_MAX_ALPHA - byAlpha));

	if(byAlpha == GUI_MAX_ALPHA)
	{
		ChangeState(CHAR_STATE_SERVER_IDLE);
	}
}

void CCharStageState::FuncUpdateServerIdle(RwReal fElapsed)
{
}

void CCharStageState::FuncUpdateServerChange(RwReal fElapsed)
{
}

void CCharStageState::FuncUpdateServerExit(RwReal fElapsed)
{
	RwUInt8 byAlpha = UpdateAlpha(fElapsed, false);

	m_pTurnOverBackboard->SetAlpha((RwUInt8)(GUI_MAX_ALPHA - byAlpha));

	if(byAlpha == GUI_MIN_ALPHA)
	{
		if( m_eReservateState != CHAR_STATE_NONE )
		{
			ChangeState(m_eReservateState);
		}
		else
		{
			// Return to character selection screen
			ChangeState(CHAR_STATE_SELECT_ENTER);
		}
	}	
}

void CCharStageState::FuncUpdateMakeEnter(RwReal fElapsed)
{	
	if(!m_bReadyWorld)
		return;

	RwUInt8 byAlpha = UpdateAlpha(fElapsed, true);

	m_pTurnOverBackboard->SetAlpha((RwUInt8)(GUI_MAX_ALPHA - byAlpha));

	if(byAlpha == GUI_MAX_ALPHA)
		ChangeState(CHAR_STATE_MAKE_IDLE);
}

void CCharStageState::FuncUpdateMakeIdle(RwReal fElapsed)
{		
}

void CCharStageState::FuncUpdateMakeExit(RwReal fElapsed)
{
	RwUInt8 byAlpha = UpdateAlpha(fElapsed, false);

	m_pTurnOverBackboard->SetAlpha((RwUInt8)(GUI_MAX_ALPHA - byAlpha));

	if(byAlpha == GUI_MIN_ALPHA)
	{	
		if( m_eReservateState != CHAR_STATE_NONE )
			ChangeState(m_eReservateState);
		else
			ChangeState(CHAR_STATE_SELECT_ENTER);
	}
}

void CCharStageState::FuncUpdateMakeRequest(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime > CDT_WAITFOR_SERVER_REPLY )
	{
		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}
}

void CCharStageState::FuncUpdateSelectInfomation(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime > CDT_WAITFOR_SERVER_REPLY )
	{
		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}
}

void CCharStageState::FuncUpdateSelectInfomationFromServerList(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime > CDT_WAITFOR_SERVER_REPLY )
	{
		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );

		GetLobbyManager()->SetWaitChannelInfo(FALSE);

		ChangeState(CHAR_STATE_SERVER_IDLE);
	}
}

void CCharStageState::FuncUpdateSelectEnter(RwReal fElapsed)
{
	if( GetLobbyManager()->IsWaitingChannelInfo() )
	{
		m_fElapedTime += fElapsed;

		if( m_fElapedTime > CDT_WAITFOR_SERVER_REPLY )
		{
			// 서버로 부터 응답이 없습니다
			GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
		}

		m_pCharSelectGui->SwitchDialog(false);
	}
	else if(m_bReadyWorld && m_bReadyModel)
	{
		RwUInt8 byAlpha = UpdateAlpha(fElapsed, true);

		m_pTurnOverBackboard->SetAlpha((RwUInt8)(GUI_MAX_ALPHA - byAlpha));

		if(byAlpha == GUI_MAX_ALPHA)
		{			
			ChangeState(CHAR_STATE_SELECT_IDLE);			
		}
	}
}

void CCharStageState::FuncUpdateSelectIdle(RwReal fElapsed)
{
}

void CCharStageState::FuncUpdateSelectDelChar(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime > CDT_WAITFOR_SERVER_REPLY )
	{
		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}
}

void CCharStageState::FuncUpdateSelectRecoveryChar(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime > CDT_WAITFOR_SERVER_REPLY )
	{
		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}
}

void CCharStageState::FuncUpdateSelectRenameReq(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime > CDT_WAITFOR_SERVER_REPLY )
	{
		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}
}

void CCharStageState::FuncUpdateSelectExit(RwReal fElapsed)
{
	RwUInt8 byAlpha = UpdateAlpha(fElapsed, false);

	if( false == bTempAlphaTutorial )
		m_pTurnOverBackboard->SetAlpha((RwUInt8)(GUI_MAX_ALPHA - byAlpha));

	if(byAlpha == GUI_MIN_ALPHA)
	{		
		ChangeState(m_eReservateState);
	}
}

void CCharStageState::FuncUpdateReturnLogin(RwReal fElapsed)
{
	// Log Stage to Login Stage
	CDboEventGenerator::StageDelete(CHARACTER_STAGE_NAME);
	CDboEventGenerator::StageCreate(LOGIN_STAGE_NAME);

	// Clear lobby information
	GetLobbyManager()->ClearLobby();

	ChangeState(CHAR_STATE_END);
}

void CCharStageState::FuncUpdateGameServerWaitCheckReq(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime >= dDELAY_BEFORE_VISIBLE_MESSAGEBOX && !m_bShowMessageBox )
	{
		m_bShowMessageBox = true;
		GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, dGAME_SERVER_CONNECT_TIME );
	}

	if( m_fElapedTime > dREMAIN_FOR_SERVER_REPLY )
	{
		m_bShowMessageBox = false;

		// No response from server
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}	
}

void CCharStageState::FuncUpdateCharacterSelectReq(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime >= dDELAY_BEFORE_VISIBLE_MESSAGEBOX && !m_bShowMessageBox )
	{
		m_bShowMessageBox = true;
		GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, dGAME_SERVER_CONNECT_TIME );
	}

	if( m_fElapedTime > dREMAIN_FOR_SERVER_REPLY )
	{
		m_bShowMessageBox = false;

		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}	
}

void CCharStageState::FuncUpdateSuccessEnterGame(RwReal fElapsed)
{
	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		return;
	}

	SConnectData*	pConnectData	= GetDboGlobal()->GetConnectData();
	CHARACTERID		charID			= pLOBBY_CHARACTER->tSummary.charId;

	GetDboGlobal()->GetGamePacketGenerator()->SendGameEnterReq(pConnectData->uiAccountId, charID,
															   pConnectData->sGameCon.chAuthKey, false);

	ChangeState(CHAR_STATE_WAITING_GAME_SERVER_RESPONSE);
}

void CCharStageState::FuncUpdateWaitingGameServerResponse(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime >= dDELAY_BEFORE_VISIBLE_MESSAGEBOX && !m_bShowMessageBox )
	{
		m_bShowMessageBox = true;
		GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, dGAME_SERVER_CONNECT_TIME );
	}

	if( m_fElapedTime > dREMAIN_FOR_SERVER_REPLY )
	{
		m_bShowMessageBox = false;

		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}
}

void CCharStageState::FuncUpdateFailEnter(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime > CDT_IMMEDIATLY_RETURN_LOGIN_MESSAGE )
	{
		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}
}

void CCharStageState::FuncUpdateImmediatelyReturnLoginEnter(RwReal fElapsed)
{
	// 로비 정보 지우기
	GetLobbyManager()->ClearLobby();

	ChangeState(CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_IDLE);
}

void CCharStageState::FuncUpdateImmediatelyReturnLoginIdle(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime > CDT_IMMEDIATLY_RETURN_LOGIN_MESSAGE )
		ChangeState(CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_EXIT);
}

void CCharStageState::FuncUpdateImmediatelyReturnLoginExit(RwReal fElapsed)
{
	if( m_pCharCreateGui->IsShow() )
	{
		ChangeState(CHAR_STATE_MAKE_IDLE);
	}
	else if( m_pCharSelectGui->IsShow() )
	{
		ChangeState(CHAR_STATE_SELECT_IDLE);
	}
	else if( m_pServerSelectGui->IsShow() )
	{
		ChangeState(CHAR_STATE_SERVER_IDLE);
	}
}

void CCharStageState::FuncUpdateReadyEnterTutorial(RwReal fElapsed)
{
	RwUInt8 byAlpha = UpdateAlpha(fElapsed, false);

	m_pTurnOverBackboard->SetAlpha((RwUInt8)(GUI_MAX_ALPHA - byAlpha));

	if(byAlpha == GUI_MIN_ALPHA)
	{
		// 서버로 튜토리얼 진입 패킷을 보낸다
		if(!m_bSendGameEnterPacket)
		{
			SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
			if( !pLobby )
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				return;
			}

			sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
			if( !pLOBBY_CHARACTER )
			{
				DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
				return;
			}

			SConnectData*	pConnectData	= GetDboGlobal()->GetConnectData();
			CHARACTERID		charID			= pLOBBY_CHARACTER->tSummary.charId;

			GetDboGlobal()->GetGamePacketGenerator()->SendGameEnterReq(pConnectData->uiAccountId, charID,
																	   pConnectData->sGameCon.chAuthKey,
																	   true);

			m_bSendGameEnterPacket = TRUE;
		}
	}

	if( m_fElapedTime >= dDELAY_BEFORE_VISIBLE_MESSAGEBOX && !m_bShowMessageBox )
	{
		m_bShowMessageBox = true;
		GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, dGAME_SERVER_CONNECT_TIME );
	}

	if( m_fElapedTime > dREMAIN_FOR_SERVER_REPLY )
	{
		m_bShowMessageBox = false;

		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}
}

void CCharStageState::FuncUpdateWaitforTutorial(RwReal fElapsed)
{

}

void CCharStageState::FuncUpdateWaitCancel(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime >= dDELAY_BEFORE_VISIBLE_MESSAGEBOX && !m_bShowMessageBox )
	{
		m_bShowMessageBox = true;
		GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, dGAME_SERVER_CONNECT_TIME );
	}

	if( m_fElapedTime > dREMAIN_FOR_SERVER_REPLY )
	{
		m_bShowMessageBox = false;

		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}	
}

void CCharStageState::FuncUpdateWaitGameServerConnect(RwReal fElapsed)
{

}

void CCharStageState::FuncUpdateCancelWaitGameServerConnect(RwReal fElapsed)
{
	m_fElapedTime += fElapsed;

	if( m_fElapedTime >= dDELAY_BEFORE_VISIBLE_MESSAGEBOX && !m_bShowMessageBox )
	{
		m_bShowMessageBox = true;
		GetAlarmManager()->AlarmMessage( "DST_SERVER_CONNECTING", FALSE, NULL, dGAME_SERVER_CONNECT_TIME );
	}

	if( m_fElapedTime > dREMAIN_FOR_SERVER_REPLY )
	{
		m_bShowMessageBox = false;

		// 서버로 부터 응답이 없습니다
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_NO_RELPY_SERVER" );
	}	
}

void CCharStageState::FuncUpdateStateEnd(RwReal fElapsed)
{

}

void CCharStageState::Update(RwReal fElapsed)
{
	if(m_fnUpdate[m_eState])
	{
		(this->*m_fnUpdate[m_eState])(fElapsed);
	}
}

void CCharStageState::EnterState(ECharacterStageState eState)
{
	if( eState == CHAR_STATE_SERVER_IDLE || eState == CHAR_STATE_MAKE_IDLE ||
		eState == CHAR_STATE_SELECT_IDLE )
		GetMsgBoxManager()->DeleteNetConnectBox();

	m_bSendGameEnterPacket = FALSE;

	// 새로운 상태 설정
	m_eState = eState;	

	// 업데이트 타임
	m_fElapedTime = 0.f;

	switch(m_eState)
	{
	case CHAR_STATE_SERVER_ENTER:
		{
			m_pTurnOverBackboard->SetAlpha(GUI_MAX_ALPHA);
			m_pTurnOverBackboard->SetColor(0, 0, 0);
			m_pTurnOverBackboard->Show(true);

			SConnectData *pConData = GetDboGlobal()->GetConnectData();

			// request character & channel info
			if(pConData->byLastServerFarmID == INVALID_SERVERFARMID )
				m_bReadyWorld = false;
			else
				m_bReadyWorld = true;

			CDboEventGenerator::LobbyEvent(LMT_ZOOM_DEFUALT);

			break;
		}
	case CHAR_STATE_MAKE_ENTER:
		{
			m_pTurnOverBackboard->SetAlpha(GUI_MAX_ALPHA);
			m_pTurnOverBackboard->SetColor(0, 0, 0);
			m_pTurnOverBackboard->Show(true);			

			m_bReadyWorld = false;
			m_bReadyModel = false;

			CDboEventGenerator::LobbyEvent(LMT_ZOOM_DEFUALT);

			break;
		}
	case CHAR_STATE_SELECT_ENTER:
		{
			// 캐릭터 내용 UI 갱신
			CDboEventGenerator::LobbyEvent(LMT_UPDATE_CHARACTER_LIST);	

			m_pTurnOverBackboard->SetAlpha(GUI_MAX_ALPHA);
			m_pTurnOverBackboard->SetColor(0, 0, 0);
			m_pTurnOverBackboard->Show(true);

			SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
			if( !pLobby )
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				return;
			}

			if(pLobby->GetCharacterCount() > 0)
				m_bReadyModel = FALSE;
			else
				m_bReadyModel = TRUE;

			m_bReadyWorld = FALSE;

			break;
		}
	case CHAR_STATE_SELECT_EXIT:
		{
			if( false == GetDboGlobal()->IsEnterTutorial() )
			{
				bTempAlphaTutorial = true;

				m_pTurnOverBackboard->SetAlpha(GUI_MIN_ALPHA);
				m_pTurnOverBackboard->SetColor(0, 0, 0);
				m_pTurnOverBackboard->Show(true);
			}
			else
			{
				bTempAlphaTutorial = false;				
			}

			break;
		}
	case CHAR_STATE_MAKE_EXIT:
	case CHAR_STATE_SERVER_EXIT:
	case CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_ENTER:
	case CHAR_STATE_WAIT_GAME_SERVER_CONNECT:
	case CHAR_STATE_READY_ENTER_TUTORIAL:
	case CHAR_STATE_CANCEL_WAIT_GAME_SERVER_CONNECT:
		{
			m_pTurnOverBackboard->SetAlpha(GUI_MIN_ALPHA);
			m_pTurnOverBackboard->SetColor(0, 0, 0);
			m_pTurnOverBackboard->Show(true);		

			break;
		}
	case CHAR_STATE_WAITING_GAME_SERVER_RESPONSE:		
	case CHAR_STATE_WAIT_CANCEL:
	case CHAR_STATE_RETURN_LOGIN:
	case CHAR_STATE_END:
		{
			m_pTurnOverBackboard->SetAlpha(GUI_MAX_ALPHA);
			m_pTurnOverBackboard->SetColor(0, 0, 0);
			m_pTurnOverBackboard->Show(true);

			break;
		}
	case CHAR_STATE_GAME_SERVER_WAIT_CHECK_REQ:
	case CHAR_STATE_CHARACTER_SELECT_REQ:
	case CHAR_STATE_SUCCESS_ENTER_GAME:	
	case CHAR_STATE_WAIT_FOR_TUTORIAL:
		{
			break;
		}
	default:
		{
			m_pTurnOverBackboard->Show(false);
			break;
		}		
	}

	CDboEventGenerator::CharStageStateEnter( (RwUInt8 )eState);
}

void CCharStageState::ExitState(ECharacterStageState eState)
{
	CDboEventGenerator::CharStageStateExit( (RwUInt8 )m_eState);
}

void CCharStageState::ReservateState(ECharacterStageState eState)
{
	m_eReservateState = eState;
}

void CCharStageState::ChangeState(ECharacterStageState eState)
{	
	m_bShowMessageBox = false;

	ExitState(m_eState);

	EnterState(eState);

	if( m_eReservateState == eState )
		m_eReservateState = CHAR_STATE_NONE;

	if( eState == CHAR_STATE_SELECT_ENTER )
		CDboEventGenerator::LobbyEvent(LMT_ZOOM_FROM_FAR_TO_DEFUALUT);
}

ECharacterStageState CCharStageState::GetCurrentState()
{
	return m_eState;
}

ECharacterStageState CCharStageState::GetReservateState()
{
	return m_eReservateState;
}

RwUInt8 CCharStageState::GetSelectedChannelIndex()
{
	if( !m_pCharSelectGui )
		return INVALID_BYTE;

	return m_pCharSelectGui->GetSelectedChannelIndex();
}

RwBool CCharStageState::IsState(ECharacterStageState eState)
{
	if( m_eState == eState )
		return true;

	return false;
}

RwUInt8 CCharStageState::UpdateAlpha(RwReal fElaped, RwBool bPlusAlpha)
{
	RwReal fRate;
	RwUInt8 byAlpha;

	m_fElapedTime += fElaped;

	if(bPlusAlpha)
	{
		fRate = m_fElapedTime / GUI_FADE_IN_TIME;

		if(fRate > 1.0f)
			fRate = 1.0f;

		byAlpha = (RwUInt8)((GUI_MAX_ALPHA - GUI_MIN_ALPHA) * fRate + GUI_MIN_ALPHA);
	}
	else
	{
		fRate = m_fElapedTime / GUI_FADE_OUT_TIME;

		if(fRate > 1.0f)
			fRate = 1.0f;

		byAlpha = (RwUInt8)(GUI_MAX_ALPHA - (GUI_MAX_ALPHA - GUI_MIN_ALPHA) * fRate);
	}

	return byAlpha;
}