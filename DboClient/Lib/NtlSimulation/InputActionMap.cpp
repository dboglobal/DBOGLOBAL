// precomp
#include "precomp_ntlsimulation.h"
#include "InputActionMap.h"

// core
#include "NtlSysEvent.h"
#include "NtlMovement.h"

// simulation
#include "NtlSLLogicDef.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"
#include "NtlSLPacketGenerator.h"
#include "NtlCameraManager.h"

// framework
#include "NtlApplication.h"

CInputActionMap* CInputActionMap::m_pInstance = 0;

/**
* \brief Construction
*/
CInputActionMap::CInputActionMap()
:m_hKeyDown(0)
,m_hKeyUp(0)
,m_hMouseDown(0)
,m_hMouseUp(0)
,m_pCallKeyboardMove(0)
,m_pCallKeyboardDashMove(0)
,m_pCallAvatarSelect(0)
,m_pCallJump(0)
,m_pCallCharging(0)
,m_pCallBlocking(0)
,m_pCallSitAndStand(0)
,m_pCallAutoRun(0)
,m_pCallSkillQuickSlotDown(0)
,m_pCallSkillQuickSlotUp(0)
,m_pCallSkillQuickSlotExDown(0)
,m_pCallSkillQuickSlotExUp(0)
,m_pCallSkillQuickSlotEx2Down(0)
,m_pCallSkillQuickSlotEx2Up(0)
,m_pCallSkillQuickSlotChange(0)
,m_pCallExit(0)
,m_pCallLooting(0)
,m_pCallDialogAction(0)
,m_pCallTabKey(0)
,m_pCallAutoTarget(0)
,m_pCallAutoAttack(0)
,m_pCallAutoFollow(0)
,m_pCallPartySelect(0)
,m_pCallPartyAutoSelect(0)
,m_pCallScouterUse(0)
,m_pCallTargetMarking(0)
,m_pCallTargetSelecting(0)
,m_pCallChatPageChange(0)
,m_uiMoveFlags(0)
,m_bRBtnDown(0)
,m_uiServerMoveFlags(0)
,m_bActive(TRUE)
,m_bAcceptServer(TRUE)
{
	NTL_PRE(m_pInstance == 0);
	m_pInstance = this;	

	m_sFrontDashMap.byDown = 0;
	m_sFrontDashMap.fTime = 0.0f;
	m_sFrontDashMap.uiMoveFlags = NTL_MOVE_F;
	m_sLeftDashMap.byDown = 0;
	m_sLeftDashMap.fTime = 0.0f;
	m_sLeftDashMap.uiMoveFlags = NTL_MOVE_L;
	m_sRightDashMap.byDown = 0;
	m_sRightDashMap.fTime = 0.0f;
	m_sRightDashMap.uiMoveFlags = NTL_MOVE_R;
	m_sBackDashMap.byDown = 0;
	m_sBackDashMap.fTime = 0.0f;
	m_sBackDashMap.uiMoveFlags = NTL_MOVE_B;

	m_mapFlag.clear();
}

/**
* \brief Destruction
*/
CInputActionMap::~CInputActionMap()
{
	m_pInstance = 0;
	m_mapFlag.clear();
}

/**
* \brief CInputActionMap의 인스턴스 리턴(싱글톤)
*/
CInputActionMap* CInputActionMap::GetInstance(void)
{
	return m_pInstance;
}

/**
* \brief Create
*/
RwBool CInputActionMap::Create(void)
{
	NTL_FUNCTION("CInputActionMap::Create");

	// Link
	LinkMsg(g_EventUpdateTick, 0, SLUPDATE_PRIORITY_INPUT);
	LinkMsg(g_EventActionMapLoadInfo, 0, 0x9000);
	LinkMsg(g_EventActionMapUpdateRes, 0, 0x9000);
	LinkMsg(g_EventTLActionMapBackup, 0 );
	LinkMsg(g_EventTLActionMapRestore, 0 );

	// CallBack
	m_hKeyDown = CInputHandler::GetInstance()->LinkKeyDown( this, &CInputActionMap::KeyDownHandler);
	m_hKeyUp = CInputHandler::GetInstance()->LinkKeyUp( this, &CInputActionMap::KeyUpHandler);
	m_hMouseDown = CInputHandler::GetInstance()->LinkMouseDown( this, &CInputActionMap::MouseDownHandler);
	m_hMouseUp = CInputHandler::GetInstance()->LinkMouseUp( this, &CInputActionMap::MouseUpHandler);

	// ActionMapManager 초기화
	InitDefaultActionMap();
	m_ActionMapManager.ClearInputAction();
	m_ActionMapManager.ClearReleaseAction();
	m_ActionMapManager.ClearResult();

	RegisterFlagMap();	// 상태회복을 위한 키를 등록

	// 어플리케이션에 핫키를 등록한다. print screen/sys rq 키를 이용하기 위해
	/*RegisterHotKey( CNtlApplication::GetInstance()->GetHWnd(), NTL_KEY_SNAPSHOT, 0, VK_SNAPSHOT );*/
	
	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CInputActionMap::Destroy(void)
{
	NTL_FUNCTION("CInputActionMap::Destroy");

	// UnLink Event
	UnLinkMsg(g_EventTLActionMapBackup);
	UnLinkMsg(g_EventTLActionMapRestore);
	UnLinkMsg(g_EventActionMapUpdateRes);
	UnLinkMsg(g_EventActionMapLoadInfo);
	UnLinkMsg(g_EventUpdateTick);
	

	// UnLink Callback
	CInputHandler::GetInstance()->UnLinkKeyDown(m_hKeyDown); 
	CInputHandler::GetInstance()->UnLinkKeyUp(m_hKeyUp);
	CInputHandler::GetInstance()->UnLinkMouseDown(m_hMouseDown); 
	CInputHandler::GetInstance()->UnLinkMouseUp(m_hMouseUp);

	// UnLink
	UnLinkKeyboardMove();
	UnLinkKeyboardDashMove();
	UnLinkAvatarSelect();
	UnLinkJump();
	UnLinkCharging();
	UnLinkBlocking();
	UnLinkSitAndStand();
	UnLinkAutoRun();
	UnLinkSkillQuickSlotDown();
	UnLinkSkillQuickSlotUp();
	UnLinkSkillQuickSlotExDown();
	UnLinkSkillQuickSlotExUp();
	UnLinkSkillQuickSlotEx2Down();
	UnLinkSkillQuickSlotEx2Up();
	UnLinkSkillQuickSlotChange();
	UnLinkGameExit();
	UnLinkLooting();
	UnLinkDialogHotKey();
	UnLinkTabKey();
	UnLinkAutoTarget();
	UnLinkAutoAttack();
	UnLinkAutoFollow();
	UnLinkPartySelect();
	UnLinkPartyAutoSelect();
	UnLinkScouterUse();
	UnLinkTargetMarking();
	UnLinkTargetSelecting();
	UnLinkChatPageChane();

	NTL_RETURNVOID();
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 Update에서 경과된 시간
*/
void CInputActionMap::Update(RwReal fElapsed)
{
	// InputActionMap이 비활성화 상태라면 Update하지 않는다.
	if(!m_bActive)
		return;

	// dash move update
	if(m_sFrontDashMap.byDown > 0)
		m_sFrontDashMap.fTime += fElapsed;

	if(m_sLeftDashMap.byDown > 0)
		m_sLeftDashMap.fTime += fElapsed;

	if(m_sRightDashMap.byDown > 0)
		m_sRightDashMap.fTime += fElapsed;

	if(m_sBackDashMap.byDown > 0)
		m_sBackDashMap.fTime += fElapsed;

	// keyboard move update
	//	if(m_uiServerMoveFlags > 0 && m_pCallKeyboardMove)
	//		m_pCallKeyboardMove->Call(m_uiServerMoveFlags);
}

/**
* \brief HandleEvents
* \param pMsg	(RWS::CMsg&) 이벤트의 메시지
*/
void CInputActionMap::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobManager::HandleEvents");

	if(pMsg.Id == g_EventUpdateTick)
	{
		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		Update(fElapsed);
	}
	// 
	else if( g_EventActionMapLoadInfo == pMsg.Id )
		HandleEventActionmapLoadInfo( pMsg );
	else if( g_EventActionMapUpdateRes == pMsg.Id )
		HandleEventsActionMapUpdateRes( pMsg );
	else if( g_EventTLActionMapBackup == pMsg.Id )
	{
		m_ActionMapManager.SetActionMapMode( ACTIONMAP_MODE_DEFAULT );
	}
	else if( g_EventTLActionMapRestore == pMsg.Id )
	{
		m_ActionMapManager.SetActionMapMode( ACTIONMAP_MODE_USERSETTING );
	}

	NTL_RETURNVOID();
}

/**
* \brief 게임에 접속할 때 서버에서 내려주는 단축키 정보를 받는다.
*/
void CInputActionMap::HandleEventActionmapLoadInfo( RWS::CMsg& msg ) 
{
	SNtlEventActionMapLoadInfo* pPacket = (SNtlEventActionMapLoadInfo*)msg.pData;

	// pPacket->byCount가 0 이면 서버에 저장되어 있는 단축키가 없다는 것이다.
	// 클라이언트에 하드 코딩되어 있는 단축키로 셋팅해주고 서버에 적용한다.
	if( 0 == pPacket->byCount )
	{
		m_ActionMapManager.ClearActionMap();

		// ActionMap에 정의되어 있는 기본 액션맵으로 정의한다.
		m_ActionMapManager.InitDefaultActionMap();

		// 모든 Release Action에 관련된 사항이나 결과값 클리어
		m_ActionMapManager.ClearResult();
		m_ActionMapManager.ClearReleaseAction();
		m_ActionMapManager.ClearInputAction();

		// 서버에 저장 및 적용
		ApplyActionMap();

		// 캡쳐( byCount 가 0 이라면 클라이언트의 단축키를 전적으로 신뢰 한다. )
		m_ActionMapManager.CaptureActionMap();
	}
	else
	{
		m_ActionMapManager.ClearActionMap();

		// 서버에서 단축키를 받아서 액션맵을 구성
		for( int i=0; i<pPacket->byCount; ++i )
			m_ActionMapManager.SetCombineKey( pPacket->asData[i].wKey, pPacket->asData[i].wActionID );

		// 캡쳐(서버에 있는 것)
		m_ActionMapManager.CaptureActionMap();

		// 기존에 선언되어 있던 디폴트 값으로 초기화
		m_ActionMapManager.InitDefaultActionMap();

		// 디폴트 초기화 된 것에 다시 한번 Setting
		for( int i=0; i<pPacket->byCount; ++i )
			m_ActionMapManager.SetCombineKey( pPacket->asData[i].wKey, pPacket->asData[i].wActionID );

		// 모든 Release Action에 관련된 사항이나 결과값 클리어
		m_ActionMapManager.ClearResult();
		m_ActionMapManager.ClearReleaseAction();
		m_ActionMapManager.ClearInputAction();

		// 다시 적용
		ApplyActionMap();
	}
}

/**
* \breif 서버에 변경된 단축키 정보를 보내고 난 후의 결과
*/
void CInputActionMap::HandleEventsActionMapUpdateRes( RWS::CMsg& msg ) 
{
	SNtlEventActionMapUpdateRes* pPacket = (SNtlEventActionMapUpdateRes*)msg.pData;

	// 성공
	if( pPacket->bSuccess )
	{
		// 서버에도 저장이 성공되었으면 완벽하게 적용을 하고
		// 서버에 적용할 수 있는 상태로 만들어둔다.
		m_ActionMapManager.CaptureActionMap();
		m_bAcceptServer = TRUE;
	}
	// 실패
	else
	{
		// 실패하였다면 기존에 캡쳐된 단축키를 날려버리고
		// 새롭게 기본 단축키를 구성하여 세팅한다.

		m_ActionMapManager.InitDefaultActionMap();
		m_bAcceptServer = TRUE;
		
		ApplyActionMap();
	}
}

/**
* \brief Initializes the key information of the InputActionMap.
*/
void CInputActionMap::Reset(void)
{
	if( m_uiServerMoveFlags != NTL_MOVE_NONE )
	{
		if(m_pCallKeyboardMove)
			m_pCallKeyboardMove->Call(NTL_MOVE_NONE);
	}

	// If the pusher is pressed, call 0
	if( IsDownAction(ACTION_AVATAR_CHARGE) && m_pCallCharging)
	{
		m_pCallCharging->Call(0);
	}
	SetFlagAction( ACTION_AVATAR_CHARGE, FALSE );

	// If blocking is pressed, call 0
	if( IsDownAction(ACTION_AVATAR_BLOCKING) && m_pCallBlocking)
	{
		m_pCallBlocking->Call(0);
	}
	SetFlagAction( ACTION_AVATAR_BLOCKING, FALSE );

	// 퀵슬롯 회복
	for( int i = ACTION_QUICK_1; i <= ACTION_QUICK_PLUS; ++i )
	{
		// 퀵슬롯이 눌러져 있었다면(?)
		if( IsDownAction( i ) )
		{
			// 0 ~ 11 ( 12개 )
			m_pCallSkillQuickSlotUp->Call( i - ACTION_QUICK_1 );
			SetFlagAction( i, FALSE );
		}
	}

	for( int i = ACTION_QUICK_1_EX; i <= ACTION_QUICK_PLUS_EX; ++i )
	{
		if( IsDownAction( i ) )
		{
			m_pCallSkillQuickSlotExUp->Call( i - ACTION_QUICK_1_EX );
			SetFlagAction( i, FALSE );
		}
	}

	for( int i = ACTION_QUICK_1_EX2; i <= ACTION_QUICK_PLUS_EX2; ++i )
	{
		if( IsDownAction( i ) )
		{
			m_pCallSkillQuickSlotEx2Up->Call( i - ACTION_QUICK_1_EX2 );
			SetFlagAction( i, FALSE );
		}
	}

	m_uiKey1MoveValidFlags	= 0;
	m_uiKey2MoveValidFlags	= 0;
	m_uiMoveFlags			= 0;
	m_bRBtnDown				= FALSE;
	m_uiServerMoveFlags		= NTL_MOVE_NONE;

	m_sFrontDashMap.byDown = 0;
	m_sFrontDashMap.fTime = 0.0f;
	m_sFrontDashMap.uiMoveFlags = NTL_MOVE_F;
	m_sLeftDashMap.byDown = 0;
	m_sLeftDashMap.fTime = 0.0f;
	m_sLeftDashMap.uiMoveFlags = NTL_MOVE_L;
	m_sRightDashMap.byDown = 0;
	m_sRightDashMap.fTime = 0.0f;
	m_sRightDashMap.uiMoveFlags = NTL_MOVE_R;
	m_sBackDashMap.byDown = 0;
	m_sBackDashMap.fTime = 0.0f;
	m_sBackDashMap.uiMoveFlags = NTL_MOVE_B;

	// ActionMap Ignore Remove
	//m_cActionMap.RemoveIgnoreKey();
	m_mapUpdownRef.clear();

	// ActionMap의 Up Down Reference를 클리어한다.
	m_ActionMapManager.ClearInputAction();
}

/**
* \brief 이동에 관련된 자료만 Reset
*/
void CInputActionMap::ResetMoveFlags( void ) 
{
	if( m_uiServerMoveFlags != NTL_MOVE_NONE )
	{
		if(m_pCallKeyboardMove)
			m_pCallKeyboardMove->Call(NTL_MOVE_NONE);
	}

	m_uiKey1MoveValidFlags	= 0;
	m_uiKey2MoveValidFlags	= 0;
	m_uiMoveFlags			= 0;

	m_uiServerMoveFlags		= NTL_MOVE_NONE;

	m_sFrontDashMap.byDown = 0;
	m_sFrontDashMap.fTime = 0.0f;
	m_sFrontDashMap.uiMoveFlags = NTL_MOVE_F;
	m_sLeftDashMap.byDown = 0;
	m_sLeftDashMap.fTime = 0.0f;
	m_sLeftDashMap.uiMoveFlags = NTL_MOVE_L;
	m_sRightDashMap.byDown = 0;
	m_sRightDashMap.fTime = 0.0f;
	m_sRightDashMap.uiMoveFlags = NTL_MOVE_R;
	m_sBackDashMap.byDown = 0;
	m_sBackDashMap.fTime = 0.0f;
	m_sBackDashMap.uiMoveFlags = NTL_MOVE_B;

	// ActionMap의 Up Down Reference를 클리어한다.
	m_ActionMapManager.ClearInputAction();
}

/**
* \brief Key가 Down된 것을 가지고 와서 액션을 찾아낸다.
*/
int CInputActionMap::KeyDownHandler(unsigned int pKeyData)
{
	SKeyData *pData = reinterpret_cast<SKeyData*>( pKeyData );

	unsigned short usAction = ACTION_INVALID;

	// RepCount가 1일때만 실행한다는 것은 한번만 실행한다는 뜻이다.
	if(pData->uiRepCount == 1)
	{
		// TODO: The current tab key uses the hardcoded ACTION_TARGET_AUTO action ID
		// This is the code we added for tab handling in the temporary login focus window since it is mapped.
		// Create a way to work out and back out, or make sure that only tab keys are handled as exceptions.
		if( FALSE == m_bActive )
		{
			if( m_pCallTabKey && pData->uiChar == 0x09 )
			{
				m_pCallTabKey->Call();
			}			
			
			unsigned short usPreAction = ACTION_INVALID;
			usPreAction = m_ActionMapManager.InputHandlerDown( (unsigned char)(pData->uiChar & 0xFF) );

			/*if( usPreAction == ACTION_GLOBAL_SNAPSHOT )
			{
				GetNtlGameCameraManager()->SetCaptureScreenShot();
			}
			else */if( usPreAction == ACTION_CHAT_PGUP )
			{
				if( m_pCallChatPageChange )
					m_pCallChatPageChange->Call( 0 );
			}
			else if( usPreAction == ACTION_CHAT_PGDN )
			{
				if( m_pCallChatPageChange )
					m_pCallChatPageChange->Call( 1 );
			}

			return 1;
		}

		int nKey = pData->uiChar & 0xFF;

		// KeyRef 증가
		KeyReference( (unsigned char)(pData->uiChar & 0xFF) );

		// Returns if the key is ignored.
		if (IsIgnoreKey((unsigned char)(pData->uiChar & 0xFF)))
			return 1;

		// Checks by sending the current key to the action map manager.
		usAction = m_ActionMapManager.InputHandlerDown( (unsigned char)(pData->uiChar & 0xFF) );

		if (usAction == ACTION_INVALID)
		{
			return 1;
		}

		ActionDownMoveHandler( usAction );
		ActionDownDashMoveHandler( usAction );
		ActionDownBehaviorHandler( usAction );
		ActionDownGuiHandler( usAction );
	}

	return 1;
}

/**
* \brief 키가 Up 되었을 때 키를 가지고 와서 액션맵을 가지고 온다.
*/
int CInputActionMap::KeyUpHandler(unsigned int pKeyData)
{
	SKeyData *pData = reinterpret_cast<SKeyData*>( pKeyData );

	unsigned short usAction = ACTION_INVALID;

	std::list< unsigned short > uiActionUpList;
	if( m_bActive == FALSE )
	{
		unsigned short usPreAction = ACTION_INVALID;
		usPreAction = m_ActionMapManager.InputHandlerUp( (unsigned char)(pData->uiChar & 0xFF), uiActionUpList );

		if( usPreAction == ACTION_GLOBAL_SNAPSHOT )
		{
			GetNtlGameCameraManager()->SetCaptureScreenShot();
		}
	}

	// VK_JUNJA는 ALT를 누른 상태에서 = 를 입력했을 경우 ( 하드코딩으로 치환해준다. )
	if ((pData->uiChar & 0xFF) == VK_JUNJA)
	{
		pData->uiChar = NTL_KEY_EQUAL;
	}

	// KeyRef 감소 : 만약 Down 되었지 않는 키라면 무조건 리턴 ( Snap Shot 만은 예외로 한다. )
	if( !KeyReference( (unsigned char)(pData->uiChar & 0xFF), FALSE ) && ( (unsigned char)(pData->uiChar & 0xFF) ) != VK_SNAPSHOT )
		return 1;

	// Return if ignored
	if( IsIgnoreKey( (unsigned char)(pData->uiChar), TRUE ) )
		return 1;

	// If you set the key, it will be parameterized as UP.
	usAction = m_ActionMapManager.InputHandlerUp( (unsigned char)(pData->uiChar & 0xFF), uiActionUpList );

	if( usAction == ACTION_INVALID )
	{
		BYTE byErr = m_ActionMapManager.GetLastResult();

		// If there are no errors, it is the same as a normal INVALID.
		if( byErr == SET_RESULT_INVALID )
		{
			// 만약 해제해줘야 하는 Action들의 List가 있다면 모두 해제해준다.
			if( !uiActionUpList.empty() )
			{
				for each( unsigned short uiActionUp in uiActionUpList )
				{
					ActionUpMoveHandler( uiActionUp );
					ActionUpDashMoveHandler( uiActionUp );
					ActionUpBehaviorHandler( uiActionUp );
					ActionUpGuiHandler( uiActionUp );
				}
			}

			return 1;
		}

		// 정상적으로 셋팅이 되었다면 InputActionMap의 상태를 회복시켜준다.
		Reset();
		
		switch( byErr )
		{
			case SET_RESULT_FIXED_KEY:
			{
				CNtlSLEventGenerator::ActionMapClientNotify( SNtlEventActionMapClientNotify::ACTIONMAP_FIXEDKEY );
			}
			break;
			case SET_RESULT_NOT_COMBINE_ACTION:
			{
				CNtlSLEventGenerator::ActionMapClientNotify( SNtlEventActionMapClientNotify::ACTIONMAP_NOTCOMBINE );
			}
			break;
			case SET_RESULT_ALREADY_KEY:			// 가지고 있던 키를 해제함
			{
				CNtlSLEventGenerator::ActionMapClientNotify( SNtlEventActionMapClientNotify::ACTIONMAP_RELEASE, m_ActionMapManager.GetLastReleaseAction() );
			}
			break;
			case SET_RESULT_SAME_KEY:
			{
				CNtlSLEventGenerator::ActionMapClientNotify( SNtlEventActionMapClientNotify::ACTIONMAP_SAMEKEY );
			}
			break;
			case SET_RESULT_OK:
			{
				CNtlSLEventGenerator::ActionMapClientNotify( SNtlEventActionMapClientNotify::ACTIONMAP_OK );
			}
			break;
		default:
			break;
		}

		return 1;
	}

	ActionUpMoveHandler( usAction );
	ActionUpDashMoveHandler( usAction );
	ActionUpBehaviorHandler( usAction );
	ActionUpGuiHandler( usAction );

	return 1;
}


int CInputActionMap::MouseDownHandler(unsigned int pMouseData)
{

	/*SMouseData *pData = reinterpret_cast<SMouseData*>(pMouseData);*/
	//if(pData->chBtnType == MOUSE_RBTN)  //마우스 눌리는건 후에 구현
	//m_bRBtnDown = TRUE;

	//ConvertServerFlags(m_uiServerMoveFlags);

	return 1;
}

int CInputActionMap::MouseUpHandler(unsigned int pMouseData)
{

	/*SMouseData *pData = reinterpret_cast<SMouseData*>(pMouseData);*/
	//if(pData->chBtnType == MOUSE_RBTN)
	//m_bRBtnDown = FALSE;

	//ConvertServerFlags(m_uiServerMoveFlags);

	return 1;
}

void CInputActionMap::SetActive(RwBool bActive)
{ 
	m_bActive = bActive; 

	// 비활성화라면 이동 Flag를 Reset
	if( bActive == FALSE )
		ResetMoveFlags();
}

void CInputActionMap::InitInputMode() 
{
	m_ActionMapManager.ClearInputAction();
}

RwUInt32 CInputActionMap::GetInputMode() 
{
	return (RwUInt32)m_ActionMapManager.GetInputAction();
}

void CInputActionMap::SetInputMode( RwUInt32 nAction ) 
{
	if( !m_ActionMapManager.SetInputAction( (unsigned short)nAction ) )
	{
		BYTE byErr = m_ActionMapManager.GetLastResult();

		// 에러 검출
		if( byErr == SET_RESULT_OK )
			return;
	}
}

std::wstring CInputActionMap::GetKeyName( RwUInt32 nAction ) 
{
	return m_ActionMapManager.GetKeyName( (RwUInt16)nAction );
}

unsigned short CInputActionMap::FindKey( RwUInt32 nAction ) 
{
	return m_ActionMapManager.FindKey( (RwUInt16)nAction );
}

void CInputActionMap::InitDefaultActionMap() 
{
	m_ActionMapManager.InitDefaultActionMap();
}

/**
* \brief 액션맵의 적용
*
* 자료구조인 CActionMap에서 변경된 점을 체크하고 그것을 기록하여
* 서버로 패킷을 전송한다.
*/
RwBool CInputActionMap::ApplyActionMap()
{
	if( m_bAcceptServer == FALSE )
		return FALSE;

	sSHORTCUT_UPDATE_DATA pData[NTL_SHORTCUT_MAX_COUNT];
	BYTE byCount = 0;
	if( m_ActionMapManager.GetUpdateData( pData , byCount ) )
	{
		// 바뀐 점이 있다면 패킷을 보내고 Result코드가 변경점이 적용되기 전까지 서버에 적용을 할 수 없는
		// 모드로 셋팅한다.
		API_GetSLPacketGenerator()->SendCharKeyUpdateReq( pData, byCount );

		m_bAcceptServer = FALSE;

		return TRUE;
	}

	return FALSE;
}

void CInputActionMap::CancleActionMap() 
{
	if( m_bAcceptServer )
		m_ActionMapManager.RestoreActionMap();
}

/**
* \brief 무시될 키를 등록한다.
* \param byKey		(RwUInt8) 키의 VK_CODE
* \param nRefCount	(int) 무시될 횟수, KeyUpHandler에서 카운트를 체크한다.
* \return 성공여부
*/
RwBool CInputActionMap::RegisterIgnoreKey( RwUInt8 byKey, int nRefCount ) 
{
	IGNOREMAP::iterator it = m_mapIgnore.find( byKey );
	// 이미 존재함
	if( it != m_mapIgnore.end() )
	{
		return FALSE;
	}

	// 존재하지 않는다면 추가한다.
	m_mapIgnore[byKey] = (RwUInt8)nRefCount;
	
	return TRUE;
}

RwBool CInputActionMap::RemoveIgnoreKey( RwUInt8 byKey ) 
{
	IGNOREMAP::iterator it = m_mapIgnore.find( byKey );

	// 존재하지 않음
	if( it == m_mapIgnore.end() )
	{
		return FALSE;
	}

	// 존재하면 삭제
	m_mapIgnore.erase( it );

	return TRUE;
}

RwBool CInputActionMap::IsIgnoreKey( RwUInt8 byKey, RwBool bRef /*= FALSE */ ) 
{
	IGNOREMAP::iterator it = m_mapIgnore.find( byKey );

	// 무시되는 키가 아님.
	if( it == m_mapIgnore.end() )
		return FALSE;
	
	// BYTE값이 INVALID라면 REF를 감소하지 않고 무시
	if( it->second == 0xFF )
		return TRUE;

	// Ref Count 감소
	if( bRef )
		(*it).second--;

	// 0 보다 작거나 같으면 삭제
	if( (*it).second <= 0 )
		m_mapIgnore.erase( it );

	// 무시하라고 알려줌
	return TRUE;
}

void CInputActionMap::ClearIgnoreKey() 
{
	m_mapIgnore.clear();
}

RwUInt8 CInputActionMap::GetActionMapMode()
{
	return m_ActionMapManager.GetActionMapMode();
}

RwInt32 CInputActionMap::GetKeyDownCount(RwUInt32 uiFlags)
{
	RwInt32 iCount = 0;
	for(RwInt32 i = 0; i < 4; ++i)
	{
		if( (uiFlags >> i) & 0x01 )
			iCount++;
	}

	return iCount;
}

RwUInt32 CInputActionMap::ConvertServerFlags(RwUInt32 uiFlags)
{
	//The priority of the keyboard is determined.
	if (uiFlags == NTL_BIT_FRONT_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_F;
	}
	else if (uiFlags == NTL_BIT_TURN_LEFT_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_TURN_L;
	}
	else if(uiFlags == NTL_BIT_BACK_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_B;
	}
	else if(uiFlags == NTL_BIT_TURN_RIGHT_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_TURN_R;
	}
	else if (uiFlags == NTL_BIT_LEFT_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_L;
	}
	else if (uiFlags == NTL_BIT_RIGHT_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_R;
	}
	else if(uiFlags == NTL_BIT_LF_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_F_TURN_L;
	}
	else if(uiFlags == NTL_BIT_LB_MOVE )
	{
		m_uiServerMoveFlags = NTL_MOVE_B_TURN_L;
	}
	else if(uiFlags == NTL_BIT_RF_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_F_TURN_R;
	}
	else if(uiFlags == NTL_BIT_RB_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_B_TURN_R;
	}

	else if (uiFlags == NTL_BIT_FL_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_F_TURN_L;
	}
	else if (uiFlags == NTL_BIT_FR_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_F_TURN_R;
	}
	else if (uiFlags == NTL_BIT_BL_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_B_TURN_L;
	}
	else if (uiFlags == NTL_BIT_BR_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_B_TURN_R;
	}
	else if (uiFlags == NTL_BIT_LL_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_L_TURN_L;
	}
	else if (uiFlags == NTL_BIT_LR_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_L_TURN_R;
	}
	else if (uiFlags == NTL_BIT_RL_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_R_TURN_L;
	}
	else if (uiFlags == NTL_BIT_RR_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_R_TURN_R;
	}
	else if (uiFlags == NTL_BIT_DOWN_MOVE)
	{
		m_uiServerMoveFlags = NTL_MOVE_HEIGHT_DOWN;
	}

	else if(uiFlags == NTL_BIT_FB_MOVE)
	{
		ConvertServerFlags(m_uiKey1MoveValidFlags);
	}
	else
		m_uiServerMoveFlags = NTL_MOVE_NONE;

	return m_uiServerMoveFlags;
}

void CInputActionMap::HitTestDownDbClickDashMap(SInputDashMap& sDashMap)
{
	if(sDashMap.byDown % 2 != 0)
		return;

	if(sDashMap.byDown == 0)
		sDashMap.byDown++;
	else
	{
		if(sDashMap.fTime <= NTL_KEYBOARD_DBCLICK_TIME)
			sDashMap.byDown++;
		else
			sDashMap.byDown = 1;
	}

	sDashMap.fTime = 0.0f;
}


void CInputActionMap::HitTestUpDbClickDashMap(SInputDashMap& sDashMap)
{
	if(sDashMap.fTime <= NTL_KEYBOARD_DBCLICK_TIME)
		sDashMap.byDown++;
	else
		sDashMap.byDown = 0;

	if(sDashMap.byDown >= 4)
	{
		CallDashMove(sDashMap.uiMoveFlags);
		sDashMap.byDown = 0;
	}

	sDashMap.fTime = 0.0f;
}

void CInputActionMap::CallDashMove(RwUInt32 uiServerDashMoveFlags)
{
	// dash 보낸다(0.1 초 안에 또 다시 누르면). 
	if(!m_bActive || !m_pCallKeyboardDashMove)
		return;

	m_pCallKeyboardDashMove->Call(uiServerDashMoveFlags);
}

/**
* \ brief Handler according to move action
*
* When the key is pressed, the bit of the key is set to the current movement, and the
It converts to * flag.
*
* \ param iAction (unsigned int) Action ID
*/
void CInputActionMap::ActionDownMoveHandler( RwUInt32 uiAction ) 
{
	// old server move flags
	RwUInt32 uiOldServerMoveFlags = m_uiServerMoveFlags;

	// keyboard down count check
	RwInt32 iKeyDownCount = GetKeyDownCount(m_uiMoveFlags); 

	if(iKeyDownCount == 1)
	{
		m_uiKey1MoveValidFlags = m_uiMoveFlags;
	}

	else if(iKeyDownCount == 2)
	{
		m_uiKey2MoveValidFlags = m_uiMoveFlags;
	}

	// ACTION에 따른 행동 정의
	switch( uiAction )
	{
	case ACTION_AVATAR_FORWARD:
		m_uiMoveFlags |= NTL_BIT_FRONT_MOVE;
		break;
	case ACTION_AVATAR_BACKWARD:
		m_uiMoveFlags |= NTL_BIT_BACK_MOVE;
		break;
	case ACTION_AVATAR_LEFTTURN:
		m_uiMoveFlags |= NTL_BIT_TURN_LEFT_MOVE;
		break;
	case ACTION_AVATAR_RIGHTTURN:
		m_uiMoveFlags |= NTL_BIT_TURN_RIGHT_MOVE;
		break;
	case ACTION_AVATAR_LEFTSIDE:
		m_uiMoveFlags |= NTL_BIT_LEFT_MOVE;
		break;
	case ACTION_AVATAR_RIGHTSIDE:
		m_uiMoveFlags |= NTL_BIT_RIGHT_MOVE;
		break;

	default:
		break;
	}

	// server flag convert
	if(GetKeyDownCount(m_uiMoveFlags) > 2)
	{
		ConvertServerFlags(m_uiKey2MoveValidFlags);
	}
	else
	{
		ConvertServerFlags(m_uiMoveFlags);
	}

	// If there is a change in the flag that contains the flag to move to the server
	// Send the flag.
	if( m_uiServerMoveFlags != uiOldServerMoveFlags )
	{
		if(m_bActive && m_pCallKeyboardMove)
			m_pCallKeyboardMove->Call(m_uiServerMoveFlags);
	}
}

/**
* \brief 대쉬 이동을 체크한다.
*
* 현재 액션에 맞는 대쉬의 발동을 확인하여 조건이 맞으면 대쉬를 실행한다.
* \param iAction (unsigned int)액션ID
*/
void CInputActionMap::ActionDownDashMoveHandler( RwUInt32 uiAction ) 
{
	switch( uiAction )
	{
	case ACTION_AVATAR_FORWARD:
		HitTestDownDbClickDashMap(m_sFrontDashMap);
		break;
	case ACTION_AVATAR_LEFTTURN:
	case ACTION_AVATAR_LEFTSIDE:
		HitTestDownDbClickDashMap(m_sLeftDashMap);
		break;
	case ACTION_AVATAR_BACKWARD:
		HitTestDownDbClickDashMap(m_sBackDashMap);
		break;
	case ACTION_AVATAR_RIGHTTURN:
	case ACTION_AVATAR_RIGHTSIDE:
		HitTestDownDbClickDashMap(m_sRightDashMap);
		break;
	default:
		break;
	}
}

void CInputActionMap::ActionDownBehaviorHandler( RwUInt32 uiAction ) 
{
	if( FALSE == m_bActive )
		return;

	switch( uiAction )
	{
	case ACTION_AVATAR_SITDOWN:		// sit/standup | fly down
	{
		if (m_pCallJump && IsDownAction(ACTION_AVATAR_SITDOWN) == FALSE)
		{
			SetFlagAction(ACTION_AVATAR_SITDOWN, TRUE);
			m_pCallSitAndStand->Call(TRUE);
		}
		break;
	}
	case ACTION_AVATAR_JUMP:		// jump
		{
			if( m_pCallJump && IsDownAction( ACTION_AVATAR_JUMP ) == FALSE )
			{
				SetFlagAction( ACTION_AVATAR_JUMP, TRUE );
				m_pCallJump->Call(m_uiServerMoveFlags);
			}
			break;
		}
	case ACTION_AVATAR_LOOTING:		// 루팅
		{
			if( m_pCallLooting )
				m_pCallLooting->Call();
			break;
		}
	case ACTION_AVATAR_CHARGE:		// 기 모으기
		{
			if( m_pCallCharging && IsDownAction( ACTION_AVATAR_CHARGE ) == FALSE  )
			{
				SetFlagAction( ACTION_AVATAR_CHARGE, TRUE );
				m_pCallCharging->Call( 1 );
			}
			break;
		}
	case ACTION_AVATAR_BLOCKING:
		{
			if( m_pCallBlocking && IsDownAction( ACTION_AVATAR_BLOCKING ) == FALSE )
			{
				SetFlagAction( ACTION_AVATAR_BLOCKING, TRUE );
				m_pCallBlocking->Call( 1 );
			}
			break;
		}
	}
}

/**
* \brief 키를 눌렀을 때 처리되는 GUI 액션들
*
* 키를 누른 상태에서 발생할 수 있는 GUI액션들을 검사하여 실행하고 
*
* \param iAction (unsigned int)액션ID
*/
void CInputActionMap::ActionDownGuiHandler( RwUInt32 uiAction ) 
{
	if( FALSE == m_bActive )
		return;

	switch( uiAction )
	{
	case ACTION_TARGET_SELF:		// 자기 자신 선택
		{
			if( m_pCallAvatarSelect )
				m_pCallAvatarSelect->Call();
			break;
		}

	case ACTION_QUICK_1:			// 퀵슬롯
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_1 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_1, TRUE );
				m_pCallSkillQuickSlotDown->Call( 0 );
			}
			break;
		}
	case ACTION_QUICK_2:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_2 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_2, TRUE );
				m_pCallSkillQuickSlotDown->Call( 1 );
			}
			break;
		}
	case ACTION_QUICK_3:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_3 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_3, TRUE );
				m_pCallSkillQuickSlotDown->Call( 2 );
			}
			break;
		}
	case ACTION_QUICK_4:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_4 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_4, TRUE );
				m_pCallSkillQuickSlotDown->Call( 3 );
			}
			break;
		}
	case ACTION_QUICK_5:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_5 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_5, TRUE );
				m_pCallSkillQuickSlotDown->Call( 4 );
			}
			break;
		}
	case ACTION_QUICK_6:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_6 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_6, TRUE );
				m_pCallSkillQuickSlotDown->Call( 5 );
			}
			break;
		}
	case ACTION_QUICK_7:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_7 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_7, TRUE );
				m_pCallSkillQuickSlotDown->Call( 6 );
			}
			break;
		}
	case ACTION_QUICK_8:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_8 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_8, TRUE );
				m_pCallSkillQuickSlotDown->Call( 7 );
			}
			break;
		}
	case ACTION_QUICK_9:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_9 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_9, TRUE );
				m_pCallSkillQuickSlotDown->Call( 8 );
			}
			break;
		}
	case ACTION_QUICK_0:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_0 ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_0, TRUE );
				m_pCallSkillQuickSlotDown->Call( 9 );
			}
			break;
		}
	case ACTION_QUICK_MIN:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_MIN ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_MIN, TRUE );
				m_pCallSkillQuickSlotDown->Call( 10 );
			}
			break;
		}
	case ACTION_QUICK_PLUS:
		{
			if( m_pCallSkillQuickSlotDown && IsDownAction( ACTION_QUICK_MIN ) == FALSE )
			{
				SetFlagAction( ACTION_QUICK_PLUS, TRUE );
				m_pCallSkillQuickSlotDown->Call( 11 );
			}
			break;
		}

		// 확장 1번
	case ACTION_QUICK_1_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_1_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 0 );
			}
			break;
		}
	case ACTION_QUICK_2_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_2_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 1 );
			}
			break;
		}
	case ACTION_QUICK_3_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_3_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 2 );
			}
			break;
		}
	case ACTION_QUICK_4_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_4_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 3 );
			}
			break;
		}
	case ACTION_QUICK_5_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_5_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 4 );
			}
			break;
		}
	case ACTION_QUICK_6_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_6_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 5 );
			}
			break;
		}
	case ACTION_QUICK_7_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_7_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 6 );
			}
			break;
		}
	case ACTION_QUICK_8_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_8_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 7 );
			}
			break;
		}
	case ACTION_QUICK_9_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_9_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 8 );
			}
			break;
		}
	case ACTION_QUICK_0_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_0_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 9 );
			}
			break;
		}
	case ACTION_QUICK_MIN_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_MIN_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call( 10 );
			}
			break;
		}
	case ACTION_QUICK_PLUS_EX:
		{
			if( m_pCallSkillQuickSlotExDown )
			{
				SetFlagAction( ACTION_QUICK_PLUS_EX, FALSE );
				m_pCallSkillQuickSlotExDown->Call ( 11 );
			}
			break;
		}

		// 확장 2번
	case ACTION_QUICK_1_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_1_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 0 );
			}
			break;
		}
	case ACTION_QUICK_2_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_2_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 1 );
			}
			break;
		}
	case ACTION_QUICK_3_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_3_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 2 );
			}
			break;
		}
	case ACTION_QUICK_4_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_4_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 3 );
			}
			break;
		}
	case ACTION_QUICK_5_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_5_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 4 );
			}
			break;
		}
	case ACTION_QUICK_6_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_6_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 5 );
			}
			break;
		}
	case ACTION_QUICK_7_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_7_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 6 );
			}
			break;
		}
	case ACTION_QUICK_8_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_8_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 7 );
			}
			break;
		}
	case ACTION_QUICK_9_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_9_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 8 );
			}
			break;
		}
	case ACTION_QUICK_0_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_0_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 9 );
			}
			break;
		}
	case ACTION_QUICK_MIN_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_MIN_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call( 10 );
			}
			break;
		}
	case ACTION_QUICK_PLUS_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Down )
			{
				SetFlagAction( ACTION_QUICK_PLUS_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Down->Call ( 11 );
			}
			break;
		}

	case ACTION_TARGET_1STPARTY:		// 첫번째 파티원 선택
		{
			if( m_pCallPartySelect )
				m_pCallPartySelect->Call( 0 );

			break;
		}
	case ACTION_TARGET_2NDPARTY:		//  두번째 파티원 선택
		{
			if( m_pCallPartySelect )
				m_pCallPartySelect->Call( 1 );

			break;
		}
	case ACTION_TARGET_3RDPARTY:		// 세번째
		{
			if( m_pCallPartySelect )
				m_pCallPartySelect->Call( 2 );

			break;
		}
	case ACTION_TARGET_4THPARTY:		// 네번째
		{
			if( m_pCallPartySelect )
				m_pCallPartySelect->Call( 3 );

			break;
		}
	case ACTION_TARGET_AUTOTARGET_PARTY:
		{
			if( m_pCallPartyAutoSelect )
				m_pCallPartyAutoSelect->Call();

			break;
		}
	default:
		break;
	}
}

/**
* \brief 키업에 따른 이동액션 핸들러
*
* 키 업에 따라 발생하는 이동 액션을 처리한다.
*
* \param iAction (unsigned int)액션ID
*/
void CInputActionMap::ActionUpMoveHandler( RwUInt32 uiAction ) 
{
	switch( uiAction )
	{
		case ACTION_AVATAR_FORWARD:	// 전진
		{
			m_uiMoveFlags &= (~NTL_BIT_FRONT_MOVE);

			if (GetKeyDownCount(m_uiMoveFlags) > 2)
			{
				ConvertServerFlags(m_uiKey2MoveValidFlags);
			}
			else
			{
				ConvertServerFlags(m_uiMoveFlags);
			}

			if(m_bActive && m_pCallKeyboardMove)
				m_pCallKeyboardMove->Call(m_uiServerMoveFlags);

			break;
		}
		case ACTION_AVATAR_LEFTTURN: // 좌회전
		{
			m_uiMoveFlags &= (~NTL_BIT_TURN_LEFT_MOVE);

			if(GetKeyDownCount(m_uiMoveFlags) > 2)
				ConvertServerFlags(m_uiKey2MoveValidFlags);
			else
				ConvertServerFlags(m_uiMoveFlags);

			if(m_bActive && m_pCallKeyboardMove)
				m_pCallKeyboardMove->Call(m_uiServerMoveFlags);

			break;
		}
		case ACTION_AVATAR_BACKWARD: // 후진
		{
			m_uiMoveFlags &= (~NTL_BIT_BACK_MOVE);

			if(GetKeyDownCount(m_uiMoveFlags) > 2)
				ConvertServerFlags(m_uiKey2MoveValidFlags);
			else
				ConvertServerFlags(m_uiMoveFlags);

			if(m_bActive && m_pCallKeyboardMove)
				m_pCallKeyboardMove->Call(m_uiServerMoveFlags);
		}
		break;
		case ACTION_AVATAR_RIGHTTURN: // 우회전
		{
			m_uiMoveFlags &= (~NTL_BIT_TURN_RIGHT_MOVE);

			if(GetKeyDownCount(m_uiMoveFlags) > 2)
				ConvertServerFlags(m_uiKey2MoveValidFlags);
			else
				ConvertServerFlags(m_uiMoveFlags);

			if(m_bActive && m_pCallKeyboardMove)
				m_pCallKeyboardMove->Call(m_uiServerMoveFlags);
		}
		break;
		case ACTION_AVATAR_LEFTSIDE:
		{
			m_uiMoveFlags &= (~NTL_BIT_LEFT_MOVE);

			if (GetKeyDownCount(m_uiMoveFlags) > 2)
				ConvertServerFlags(m_uiKey2MoveValidFlags);
			else
				ConvertServerFlags(m_uiMoveFlags);

			if (m_bActive && m_pCallKeyboardMove)
				m_pCallKeyboardMove->Call(m_uiServerMoveFlags);
		}
		break;
		case ACTION_AVATAR_RIGHTSIDE:
		{
			m_uiMoveFlags &= (~NTL_BIT_RIGHT_MOVE);

			if (GetKeyDownCount(m_uiMoveFlags) > 2)
				ConvertServerFlags(m_uiKey2MoveValidFlags);
			else
				ConvertServerFlags(m_uiMoveFlags);

			if (m_bActive && m_pCallKeyboardMove)
				m_pCallKeyboardMove->Call(m_uiServerMoveFlags);
		}
		break;

	default:
		break;
	}
}

/**
* \brief 키업에 따른 이동 액션의 대쉬 핸들러
*
* 키업에 따른 이동 액션에 대쉬를 처리해야 하는 부분인지 확인하고 실행한다.
*
* \param iAction (unsigned int)액션ID
*/
void CInputActionMap::ActionUpDashMoveHandler( RwUInt32 uiAction ) 
{
	switch( uiAction )
	{
	case ACTION_AVATAR_FORWARD:
		HitTestUpDbClickDashMap(m_sFrontDashMap);
		break;
	case ACTION_AVATAR_LEFTTURN:
	case ACTION_AVATAR_LEFTSIDE:
		HitTestUpDbClickDashMap(m_sLeftDashMap);
		break;
	case ACTION_AVATAR_BACKWARD:
		HitTestUpDbClickDashMap(m_sBackDashMap);
		break;
	case ACTION_AVATAR_RIGHTTURN:
	case ACTION_AVATAR_RIGHTSIDE:
		HitTestUpDbClickDashMap(m_sRightDashMap);
		break;
	default:
		break;
	}
}

void CInputActionMap::ActionUpBehaviorHandler( RwUInt32 uiAction ) 
{
	if( FALSE == m_bActive )
		return;

	switch( uiAction )
	{
	case ACTION_AVATAR_SITDOWN:
		{
			if (m_pCallSitAndStand)
			{
				SetFlagAction(ACTION_AVATAR_SITDOWN, FALSE);
				m_pCallSitAndStand->Call(FALSE);
			}
		}
		break;
	case ACTION_AVATAR_AUTORUN:
		{
			if( m_pCallAutoRun )
				m_pCallAutoRun->Call();

			break;
		}
	case ACTION_TARGET_AUTO:			// 가까운 적 선택
		{
			if( m_pCallAutoTarget )
				m_pCallAutoTarget->Call();
			break;
		}
	case ACTION_TARGET_AUTOATK:			// 자동 공격
		{
			if( m_pCallAutoAttack )
				m_pCallAutoAttack->Call();
			break;
		}
	case ACTION_TARGET_AUTOFOLLOW:
		{
			if( m_pCallAutoFollow )
				m_pCallAutoFollow->Call();
			break;
		}
	case ACTION_AVATAR_CHARGE:
		{
			if( m_pCallCharging )
			{
				SetFlagAction( ACTION_AVATAR_CHARGE, FALSE );
				m_pCallCharging->Call(0);
			}
			break;
		}
	case ACTION_AVATAR_JUMP:		// 점프 Flag 회복
		{
			if( m_pCallJump )
			{
				SetFlagAction( ACTION_AVATAR_JUMP, FALSE );
				m_pCallJump->Call(NTL_MOVE_UNKNOWN);
			}
			
		}
		break;
	case ACTION_AVATAR_BLOCKING:
		{
			if( m_pCallBlocking )
			{
				SetFlagAction( ACTION_AVATAR_BLOCKING, FALSE );
				m_pCallBlocking->Call(0);
			}
			break;
		}
	case ACTION_AVATAR_SCOUTER:
		{
			if(m_pCallScouterUse)
				m_pCallScouterUse->Call();
			break;
		}
	case ACTION_TARGET_MARKING_1:
		{
			if(m_pCallTargetMarking)
			{
				m_pCallTargetMarking->Call(0);
			}
			break;
		}
	case ACTION_TARGET_MARKING_2:
		{
			if(m_pCallTargetMarking)
			{
				m_pCallTargetMarking->Call(1);
			}
			break;
		}
	case ACTION_TARGET_MARKING_3:
		{
			if(m_pCallTargetMarking)
			{
				m_pCallTargetMarking->Call(2);
			}
			break;
		}
	case ACTION_TARGET_MARKING_4:
		{
			if(m_pCallTargetMarking)
			{
				m_pCallTargetMarking->Call(3);
			}
			break;
		}
	case ACTION_TARGET_MARKING_5:
		{
			if(m_pCallTargetMarking)
			{
				m_pCallTargetMarking->Call(4);
			}
			break;
		}
	case ACTION_TARGET_SELECTING_1:
		{
			if(m_pCallTargetSelecting)
			{
				m_pCallTargetSelecting->Call(0);
			}
			break;
		}
	case ACTION_TARGET_SELECTING_2:
		{
			if(m_pCallTargetSelecting)
			{
				m_pCallTargetSelecting->Call(1);
			}
			break;
		}
	case ACTION_TARGET_SELECTING_3:
		{
			if(m_pCallTargetSelecting)
			{
				m_pCallTargetSelecting->Call(2);
			}
			break;
		}
	case ACTION_TARGET_SELECTING_4:
		{
			if(m_pCallTargetSelecting)
			{
				m_pCallTargetSelecting->Call(3);
			}
			break;
		}
	case ACTION_TARGET_SELECTING_5:
		{
			if(m_pCallTargetSelecting)
			{
				m_pCallTargetSelecting->Call(4);
			}
			break;
		}
	default:
		break;
	}
}

/**
* \brief 키업에 따른 GUI 액션의 핸들러
*
* \param iAction (unsigned int)액션ID
*/
void CInputActionMap::ActionUpGuiHandler( RwUInt32 uiAction ) 
{
	if( FALSE == m_bActive )
		return;

	switch( uiAction )
	{
	case ACTION_GLOBAL_CANCLE:
		{
			if( m_pCallExit )
				m_pCallExit->Call();
			break;
		}
	case ACTION_QUICK_1:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_1, FALSE );
				m_pCallSkillQuickSlotUp->Call( 0 );
			}
			break;
		}
	case ACTION_QUICK_2:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_2, FALSE );
				m_pCallSkillQuickSlotUp->Call( 1 );
			}
			break;
		}
	case ACTION_QUICK_3:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_3, FALSE );
				m_pCallSkillQuickSlotUp->Call( 2 );
			}
			break;
		}
	case ACTION_QUICK_4:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_4, FALSE );
				m_pCallSkillQuickSlotUp->Call( 3 );
			}
			break;
		}
	case ACTION_QUICK_5:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_5, FALSE );
				m_pCallSkillQuickSlotUp->Call( 4 );
			}
			break;
		}
	case ACTION_QUICK_6:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_6, FALSE );
				m_pCallSkillQuickSlotUp->Call( 5 );
			}
			break;
		}
	case ACTION_QUICK_7:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_7, FALSE );
				m_pCallSkillQuickSlotUp->Call( 6 );
			}
			break;
		}
	case ACTION_QUICK_8:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_8, FALSE );
				m_pCallSkillQuickSlotUp->Call( 7 );
			}
			break;
		}
	case ACTION_QUICK_9:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_9, FALSE );
				m_pCallSkillQuickSlotUp->Call( 8 );
			}
			break;
		}
	case ACTION_QUICK_0:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_0, FALSE );
				m_pCallSkillQuickSlotUp->Call( 9 );
			}
			break;
		}
	case ACTION_QUICK_MIN:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_MIN, FALSE );
				m_pCallSkillQuickSlotUp->Call( 10 );
			}
			break;
		}
	case ACTION_QUICK_PLUS:
		{
			if( m_pCallSkillQuickSlotUp )
			{
				SetFlagAction( ACTION_QUICK_PLUS, FALSE );
				m_pCallSkillQuickSlotUp->Call ( 11 );
			}
			break;
		}

		// 확장 1번
	case ACTION_QUICK_1_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_1_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 0 );
			}
			break;
		}
	case ACTION_QUICK_2_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_2_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 1 );
			}
			break;
		}
	case ACTION_QUICK_3_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_3_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 2 );
			}
			break;
		}
	case ACTION_QUICK_4_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_4_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 3 );
			}
			break;
		}
	case ACTION_QUICK_5_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_5_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 4 );
			}
			break;
		}
	case ACTION_QUICK_6_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_6_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 5 );
			}
			break;
		}
	case ACTION_QUICK_7_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_7_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 6 );
			}
			break;
		}
	case ACTION_QUICK_8_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_8_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 7 );
			}
			break;
		}
	case ACTION_QUICK_9_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_9_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 8 );
			}
			break;
		}
	case ACTION_QUICK_0_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_0_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 9 );
			}
			break;
		}
	case ACTION_QUICK_MIN_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_MIN_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call( 10 );
			}
			break;
		}
	case ACTION_QUICK_PLUS_EX:
		{
			if( m_pCallSkillQuickSlotExUp )
			{
				SetFlagAction( ACTION_QUICK_PLUS_EX, FALSE );
				m_pCallSkillQuickSlotExUp->Call ( 11 );
			}
			break;
		}

	// 확장 2번
	case ACTION_QUICK_1_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_1_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 0 );
			}
			break;
		}
	case ACTION_QUICK_2_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_2_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 1 );
			}
			break;
		}
	case ACTION_QUICK_3_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_3_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 2 );
			}
			break;
		}
	case ACTION_QUICK_4_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_4_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 3 );
			}
			break;
		}
	case ACTION_QUICK_5_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_5_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 4 );
			}
			break;
		}
	case ACTION_QUICK_6_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_6_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 5 );
			}
			break;
		}
	case ACTION_QUICK_7_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_7_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 6 );
			}
			break;
		}
	case ACTION_QUICK_8_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_8_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 7 );
			}
			break;
		}
	case ACTION_QUICK_9_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_9_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 8 );
			}
			break;
		}
	case ACTION_QUICK_0_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_0_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 9 );
			}
			break;
		}
	case ACTION_QUICK_MIN_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_MIN_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call( 10 );
			}
			break;
		}
	case ACTION_QUICK_PLUS_EX2:
		{
			if( m_pCallSkillQuickSlotEx2Up )
			{
				SetFlagAction( ACTION_QUICK_PLUS_EX2, FALSE );
				m_pCallSkillQuickSlotEx2Up->Call ( 11 );
			}
			break;
		}

	case ACTION_QUICK_PREV:
		{
			if( m_pCallSkillQuickSlotChange )
			{
				m_pCallSkillQuickSlotChange->Call( 0 );
			}
			break;
		}
	case ACTION_QUICK_NEXT:
		{
			if( m_pCallSkillQuickSlotChange )
			{
				m_pCallSkillQuickSlotChange->Call( 1 );
			}
			break;
		}
	case ACTION_CHAT_PGUP:
		{
			if( m_pCallChatPageChange )
				m_pCallChatPageChange->Call( 0 );
		}
		break;
	case ACTION_CHAT_PGDN:
		{
			if( m_pCallChatPageChange )
				m_pCallChatPageChange->Call( 1 );
		}
		break;
	case ACTION_GLOBAL_SNAPSHOT:		// 스크린 캡쳐 기능
		{
			GetNtlGameCameraManager()->SetCaptureScreenShot();
			break;
		}
	default:
		break;
	}

	// 다이얼로그 액션 처리
	if( m_pCallDialogAction )
		m_pCallDialogAction->Call( uiAction );
}

/**
* \brief Register actions that require state recovery.
*/
void CInputActionMap::RegisterFlagMap() 
{
	m_mapFlag[ACTION_AVATAR_CHARGE] = FALSE;		// 기 모으기
	m_mapFlag[ACTION_AVATAR_BLOCKING] = FALSE;		// 블록킹
	m_mapFlag[ACTION_AVATAR_SITDOWN] = FALSE;		// sit/stand up & fly down
	m_mapFlag[ACTION_AVATAR_JUMP] = FALSE;			// 점프
	m_mapFlag[ACTION_QUICK_1] = FALSE;				// 퀵슬롯 단축키들
	m_mapFlag[ACTION_QUICK_2] = FALSE;
	m_mapFlag[ACTION_QUICK_3] = FALSE;
	m_mapFlag[ACTION_QUICK_4] = FALSE;
	m_mapFlag[ACTION_QUICK_5] = FALSE;
	m_mapFlag[ACTION_QUICK_6] = FALSE;
	m_mapFlag[ACTION_QUICK_7] = FALSE;
	m_mapFlag[ACTION_QUICK_8] = FALSE;
	m_mapFlag[ACTION_QUICK_9] = FALSE;
	m_mapFlag[ACTION_QUICK_0] = FALSE;
	m_mapFlag[ACTION_QUICK_MIN] = FALSE;
	m_mapFlag[ACTION_QUICK_PLUS] = FALSE;
	m_mapFlag[ACTION_QUICK_1_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_2_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_3_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_4_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_5_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_6_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_7_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_8_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_9_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_0_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_MIN_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_PLUS_EX] = FALSE;
	m_mapFlag[ACTION_QUICK_1_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_2_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_3_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_4_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_5_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_6_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_7_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_8_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_9_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_0_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_MIN_EX2] = FALSE;
	m_mapFlag[ACTION_QUICK_PLUS_EX2] = FALSE;
}

/**
* \brief 현재 액션의 상태를 가지고 온다.
* \param iAction	액션
* \return 액션의 상태 ( 없으면 FALSE )
*/
RwBool CInputActionMap::IsDownAction( RwUInt32 uiAction ) 
{
	FLAGMAP::iterator it = m_mapFlag.find( uiAction );
	if( it == m_mapFlag.end() )
	{
		return FALSE;
	}

	return (*it).second;
}

/**
* \brief 액션의 플래그를 지정한다.
* \param iAction	액션의 NUMBER
* \param bDown		다운의 상태
* \return 성공여부
*/
RwBool CInputActionMap::SetFlagAction( RwUInt32 uiAction, RwBool bDown ) 
{
	FLAGMAP::iterator it = m_mapFlag.find( uiAction );
	if ( it == m_mapFlag.end() )
	{
		return FALSE;
	}

	(*it).second = bDown;
	return TRUE;
}

void CInputActionMap::UnLinkKeyboardMove(void)
{
	if(m_pCallKeyboardMove)
		NTL_DELETE(m_pCallKeyboardMove);
}

void CInputActionMap::UnLinkKeyboardDashMove(void)
{
	if(m_pCallKeyboardDashMove)
		NTL_DELETE(m_pCallKeyboardDashMove);
}

void CInputActionMap::UnLinkAvatarSelect(void)
{
	if(m_pCallAvatarSelect)
		NTL_DELETE(m_pCallAvatarSelect);
}

void CInputActionMap::UnLinkJump(void)
{
	if(m_pCallJump)
		NTL_DELETE(m_pCallJump);
}

void CInputActionMap::UnLinkCharging(void)
{
	if(m_pCallCharging)
		NTL_DELETE(m_pCallCharging);
}

void CInputActionMap::UnLinkBlocking(void)
{
	if(m_pCallBlocking)
		NTL_DELETE(m_pCallBlocking);
}

void CInputActionMap::UnLinkSitAndStand(void)
{
	if(m_pCallSitAndStand)
		NTL_DELETE(m_pCallSitAndStand);
}

void CInputActionMap::UnLinkAutoRun(void)
{
	if(m_pCallAutoRun)
		NTL_DELETE(m_pCallAutoRun);
}

void CInputActionMap::UnLinkSkillQuickSlotDown(void)
{
	if(m_pCallSkillQuickSlotDown)
		NTL_DELETE(m_pCallSkillQuickSlotDown);
}

void CInputActionMap::UnLinkSkillQuickSlotUp(void)
{
	if(m_pCallSkillQuickSlotUp)
		NTL_DELETE(m_pCallSkillQuickSlotUp);
}

void CInputActionMap::UnLinkSkillQuickSlotExDown(void)
{
	if(m_pCallSkillQuickSlotExDown)
		NTL_DELETE(m_pCallSkillQuickSlotExDown);
}

void CInputActionMap::UnLinkSkillQuickSlotExUp(void)
{
	if(m_pCallSkillQuickSlotExUp)
		NTL_DELETE(m_pCallSkillQuickSlotExUp);
}

void CInputActionMap::UnLinkSkillQuickSlotEx2Down(void)
{
	if(m_pCallSkillQuickSlotEx2Down)
		NTL_DELETE(m_pCallSkillQuickSlotEx2Down);
}

void CInputActionMap::UnLinkSkillQuickSlotEx2Up(void)
{
	if(m_pCallSkillQuickSlotEx2Up)
		NTL_DELETE(m_pCallSkillQuickSlotEx2Up);
}

void CInputActionMap::UnLinkSkillQuickSlotChange(void)
{
	if(m_pCallSkillQuickSlotChange)
		NTL_DELETE(m_pCallSkillQuickSlotChange);
}

void CInputActionMap::UnLinkGameExit(void)
{
	if(m_pCallExit)
		NTL_DELETE(m_pCallExit);
}

void CInputActionMap::UnLinkLooting(void)
{
	if(m_pCallLooting)
		NTL_DELETE(m_pCallLooting);
}

void CInputActionMap::UnLinkDialogHotKey()
{
	if(m_pCallDialogAction)
		NTL_DELETE(m_pCallDialogAction);
}

void CInputActionMap::UnLinkTabKey(void)
{
	if(m_pCallTabKey)
		NTL_DELETE(m_pCallTabKey);
}

void CInputActionMap::UnLinkAutoTarget(void)
{
	if(m_pCallAutoTarget)
		NTL_DELETE(m_pCallAutoTarget);
}

void CInputActionMap::UnLinkAutoAttack(void)
{
	if(m_pCallAutoAttack)
		NTL_DELETE(m_pCallAutoAttack);
}

void CInputActionMap::UnLinkAutoFollow(void)
{
	if(m_pCallAutoFollow)
		NTL_DELETE(m_pCallAutoFollow);
}

void CInputActionMap::UnLinkPartySelect(void)
{
	if(m_pCallPartySelect)
		NTL_DELETE(m_pCallPartySelect);
}

void CInputActionMap::UnLinkPartyAutoSelect(void)
{
	if(m_pCallPartyAutoSelect)
		NTL_DELETE(m_pCallPartyAutoSelect);
}

void CInputActionMap::UnLinkScouterUse(void) 
{
    if(m_pCallScouterUse)
        NTL_DELETE(m_pCallScouterUse);
}

void CInputActionMap::UnLinkTargetMarking(void)
{
	if(m_pCallTargetMarking)
		NTL_DELETE(m_pCallTargetMarking);
}

void CInputActionMap::UnLinkTargetSelecting(void)
{
	if(m_pCallTargetSelecting)
		NTL_DELETE(m_pCallTargetSelecting);
}

void CInputActionMap::UnLinkChatPageChane( void )
{
	if(m_pCallChatPageChange )
		NTL_DELETE(m_pCallChatPageChange);
}

/**
* \brief 키의 Up/Down 횟수를 기록한다.
* \param byChar		(RwUInt8) 키의 데이터
* \param bDown		(RwBool) 키의 Up/Down 여부
* \return 키가 이미 맵에 존재해서 Ref 증/감이 일어났을 경우 TRUE
*/
RwBool CInputActionMap::KeyReference( RwUInt8 byChar, RwBool bDown /* = TRUE */)
{
	// 이미 키가 존재할 경우
	UPDOWNREF::iterator it = m_mapUpdownRef.find( byChar );
	if( it != m_mapUpdownRef.end() )
	{
		// Ref 증가
		if( bDown )
		{
			(*it).second++;
		}
		else
		{
			// Ref 감소
			(*it).second--;
			if( (*it).second <= 0 )
				m_mapUpdownRef.erase( it );
		}

		return TRUE;
	}
	// 키가 존재하지 않을 경우
	else
	{
		if( bDown )
			m_mapUpdownRef[byChar] = 1;

		return FALSE;
	}
}