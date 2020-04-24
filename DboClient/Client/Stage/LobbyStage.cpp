#include "precomp_dboclient.h"
#include "LobbyStage.h"

// shared
#include "TableContainer.h"
#include "PCTable.h"
#include "NewbieTable.h"
#include "ItemTable.h"


// network
#include "NtlClientNet.h"

// core
#include "NtlDebug.h"
#include "NtlEventQueue.h"
#include "NtlNetSender.h"

// sound 
#include "NtlSoundEventGenerator.h"

// Presentation
#include "NtlPLSceneManager.h"
#include "NtlPLApi.h"
#include "NtlWorldCommon.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLResourcePack.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobProxy.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLApi.h"
#include "NtlStorageManager.h"

// dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "CharSelectGui.h"
#include "NtlCameraManager.h"
#include "CharStageState.h"
#include "DboLogic.h"
#include "LinkItemTrasformList.h"
#include "LobbyManager.h"

// discord
#ifdef USE_DISCORD
#include "Discord.h"
#endif


namespace
{
#define dRACE_NONE				RACE_COUNT

	// zoom mode
#define dZOOM_NONE				0
#define dZOOM_IN				1
#define dZOOM_OUT				2
};

CLobbyStage::CLobbyStage(const char *pStageName)
:CNtlStage(pStageName)
,m_pWorldEntity(NULL)
,m_pAvatar(NULL)
,m_pGuiGroup(NULL)
,m_bWorldAddCamera(FALSE)
,m_byCharHeightType(CHT_CHILD)
,m_fReservatedZoomStep(0.f)
,m_fTotalZoomStep(0.f)
,m_fTotalDistance(0.f)
,m_fTotalDirDistance(0.f)
,m_bOptionApply(FALSE)
{
	m_listZoomMode.clear();

	// human child
	m_CameraData[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.x		= -347.356f;
	m_CameraData[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.y		= 52.825f;
	m_CameraData[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.z		= 325.413f;

	m_CameraData[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_IN].v3Pos.x			= -348.561f;
	m_CameraData[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_IN].v3Pos.y			= 52.54f;
	m_CameraData[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_IN].v3Pos.z			= 326.744f;

	m_v3Dir[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_OUT].x					= -348.027f;
	m_v3Dir[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_OUT].y					= 52.895f;
	m_v3Dir[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_OUT].z					= 326.154f;

	m_v3Dir[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_IN].x					= -349.232f;
	m_v3Dir[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_IN].y					= 52.61f;
	m_v3Dir[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_IN].z					= 327.485f;

	// human adult
	m_CameraData[RACE_HUMAN][CHT_ADULT][ZL_ZOOM_OUT]
	= m_CameraData[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_OUT];

	m_CameraData[RACE_HUMAN][CHT_ADULT][ZL_ZOOM_IN].v3Pos.x			= -348.71f;
	m_CameraData[RACE_HUMAN][CHT_ADULT][ZL_ZOOM_IN].v3Pos.y			= 53.17f;
	m_CameraData[RACE_HUMAN][CHT_ADULT][ZL_ZOOM_IN].v3Pos.z			= 326.908f;

	m_v3Dir[RACE_HUMAN][CHT_ADULT][ZL_ZOOM_OUT]
	= m_v3Dir[RACE_HUMAN][CHT_CHILD][ZL_ZOOM_OUT];

	m_v3Dir[RACE_HUMAN][CHT_ADULT][ZL_ZOOM_IN].x					= -349.381f;
	m_v3Dir[RACE_HUMAN][CHT_ADULT][ZL_ZOOM_IN].y					= 53.24f;
	m_v3Dir[RACE_HUMAN][CHT_ADULT][ZL_ZOOM_IN].z					= 327.649f;

	// namek child
	m_CameraData[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.x		= 474.044f;
	m_CameraData[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.y		= 52.802f;
	m_CameraData[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.z		= 297.184f;

	m_CameraData[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_IN].v3Pos.x			= 473.084f;
	m_CameraData[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_IN].v3Pos.y			= 52.45f;
	m_CameraData[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_IN].v3Pos.z			= 298.589f;

	m_v3Dir[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_OUT].x					= 473.484f;
	m_v3Dir[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_OUT].y					= 52.872f;
	m_v3Dir[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_OUT].z					= 298.004f;

	m_v3Dir[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_IN].x					= 472.524f;
	m_v3Dir[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_IN].y					= 52.52f;
	m_v3Dir[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_IN].z					= 299.409f;

	// namek adult
	m_CameraData[RACE_NAMEK][CHT_ADULT][ZL_ZOOM_OUT]
	= m_CameraData[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_OUT];

	m_CameraData[RACE_NAMEK][CHT_ADULT][ZL_ZOOM_IN].v3Pos.x			= 472.856f;
	m_CameraData[RACE_NAMEK][CHT_ADULT][ZL_ZOOM_IN].v3Pos.y			= 53.422f;
	m_CameraData[RACE_NAMEK][CHT_ADULT][ZL_ZOOM_IN].v3Pos.z			= 298.924f;

	m_v3Dir[RACE_NAMEK][CHT_ADULT][ZL_ZOOM_OUT]
	= m_v3Dir[RACE_NAMEK][CHT_CHILD][ZL_ZOOM_OUT];

	m_v3Dir[RACE_NAMEK][CHT_ADULT][ZL_ZOOM_IN].x					= 472.296f;
	m_v3Dir[RACE_NAMEK][CHT_ADULT][ZL_ZOOM_IN].y					= 53.492f;
	m_v3Dir[RACE_NAMEK][CHT_ADULT][ZL_ZOOM_IN].z					= 299.744f;


	// majin child
	m_CameraData[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.x		= 368.662f;
	m_CameraData[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.y		= 52.966f;
	m_CameraData[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.z		= -368.846f;

	m_CameraData[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_IN].v3Pos.x			= 367.596f;
	m_CameraData[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_IN].v3Pos.y			= 52.71f;
	m_CameraData[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_IN].v3Pos.z			= -367.788f;

	m_v3Dir[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_OUT].x					= 367.953f;
	m_v3Dir[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_OUT].y					= 53.001f;
	m_v3Dir[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_OUT].z					= -368.142f;

	m_v3Dir[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_IN].x					= 366.887f;
	m_v3Dir[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_IN].y					= 52.745f;
	m_v3Dir[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_IN].z					= -367.084f;

	// majin adult
	m_CameraData[RACE_MAJIN][CHT_ADULT][ZL_ZOOM_OUT]
	= m_CameraData[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_OUT];

	m_CameraData[RACE_MAJIN][CHT_ADULT][ZL_ZOOM_IN].v3Pos.x			= 367.859f;
	m_CameraData[RACE_MAJIN][CHT_ADULT][ZL_ZOOM_IN].v3Pos.y			= 53.423f;
	m_CameraData[RACE_MAJIN][CHT_ADULT][ZL_ZOOM_IN].v3Pos.z			= -368.049f;

	m_v3Dir[RACE_MAJIN][CHT_ADULT][ZL_ZOOM_OUT]
	= m_v3Dir[RACE_MAJIN][CHT_CHILD][ZL_ZOOM_OUT];

	m_v3Dir[RACE_MAJIN][CHT_ADULT][ZL_ZOOM_IN].x					= 367.15f;
	m_v3Dir[RACE_MAJIN][CHT_ADULT][ZL_ZOOM_IN].y					= 53.458f;
	m_v3Dir[RACE_MAJIN][CHT_ADULT][ZL_ZOOM_IN].z					= -367.345f;

	// None character
	m_CameraData[dRACE_NONE][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.x		= -293.45f;
	m_CameraData[dRACE_NONE][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.y		= 53.12f;
	m_CameraData[dRACE_NONE][CHT_CHILD][ZL_ZOOM_OUT].v3Pos.z		= -279.074f;

	m_CameraData[dRACE_NONE][CHT_CHILD][ZL_ZOOM_IN].v3Pos.x			= -294.509f;
	m_CameraData[dRACE_NONE][CHT_CHILD][ZL_ZOOM_IN].v3Pos.y			= 53.188f;
	m_CameraData[dRACE_NONE][CHT_CHILD][ZL_ZOOM_IN].v3Pos.z			= -279.966f;

	m_v3Dir[dRACE_NONE][CHT_CHILD][ZL_ZOOM_OUT].x					= -294.21f;
	m_v3Dir[dRACE_NONE][CHT_CHILD][ZL_ZOOM_OUT].y					= 53.189f;
	m_v3Dir[dRACE_NONE][CHT_CHILD][ZL_ZOOM_OUT].z					= -279.714f;

	m_v3Dir[dRACE_NONE][CHT_CHILD][ZL_ZOOM_IN].x					= -295.269f;
	m_v3Dir[dRACE_NONE][CHT_CHILD][ZL_ZOOM_IN].y					= 53.258f;
	m_v3Dir[dRACE_NONE][CHT_CHILD][ZL_ZOOM_IN].z					= -280.606f;
}

CLobbyStage::~CLobbyStage()
{
}

void CLobbyStage::CheckDeletedCharacter(void)
{
	GetLobbyManager()->CleanUp_Deleted_Character();


	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
		return;

	RwUInt8 bySelectedCharacterIndex = pLobby->GetSelectedCharacterIndex();

	if( INVALID_BYTE != bySelectedCharacterIndex )
	{
		if( pLobby->GetCharacter(bySelectedCharacterIndex) )
			return;
	}

	// 지워진 캐릭터가 선택되어 있을 경우 새로운 캐릭터를 선택한다
	if( pLobby->GetCharacterCount() > 0 )
	{
		pLobby->SetSelectedCharacterIndex(0);
		CDboEventGenerator::LobbyEvent(LMT_SELECT_CHARACTER, 0);
	}
	else
	{
		pLobby->SetSelectedCharacterIndex(INVALID_BYTE);
		CDboEventGenerator::LobbyEvent(LMT_SELECT_CHARACTER, INVALID_BYTE);
	}
}

int CLobbyStage::KeyboardDownHandler(unsigned int uiKeyData)
{
	SKeyData *pData = (SKeyData *)uiKeyData;

	if(pData->uiChar == VK_F5 && pData->uiRepCount == 1)
	{
		// refresh specific part of script
		//CNtlDefaultItemTable::GetInstance().Refresh();
		CLinkItemTrasformList::GetInstance().Refresh();
	}

	return TRUE;
}

bool CLobbyStage::Create(void)
{
	NTL_FUNCTION("CLobbyStage::Create" );

	if(Logic_IsDevUser())
	{
		m_hKeyDown = CInputHandler::GetInstance()->LinkKeyDown(this, &CLobbyStage::KeyboardDownHandler);
	}

	CheckDeletedCharacter();

	// event link
	LinkMsg(g_EventUpdateTick);
	LinkMsg(g_EventCharMake);
	LinkMsg(g_EventLobbyMessage);
	LinkMsg(g_EventCharStageStateEnter);
	LinkMsg(g_EventLobbyServerConnect);

	GetSceneManager()->SetDistanceFilter(FALSE);

	// Avatar Option
	m_iAvatarRotState = LMT_ROTATION_STOP;
	m_fRotSpeedPerSec = 20.f;

	// Gui Group
	m_pGuiGroup = NTL_NEW CCharGuiGroup;
	if(!m_pGuiGroup->Create())
	{
		NTL_DELETE(m_pGuiGroup);
		NTL_RETURN(FALSE);
	}

	// Lobby stage state
	CCharStageState::CreateInstance();

	GetCharStageState()->RegisterGUI(m_pGuiGroup);
	GetCharStageState()->ChangeState(CHAR_STATE_NONE);


	CEventTimer::RemoveAll();

	// Request the lobby-related data from the server
	RequestLobbyInfo();

	// option load
	Logic_LoadAccountOption();
	Logic_LoadGameOption();


#ifdef USE_DISCORD
	GetDiscordManager()->LoginCharServer();
#endif

	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);

	NTL_RETURN(TRUE);
}

void CLobbyStage::Destroy(void)
{
	NTL_FUNCTION("CLobbyStage::Destroy" );

	CNtlPostEffectCamera::SetPostEffectFilters( POST_EFFECT_FILTER_NONE );

	if(Logic_IsDevUser())
	{
		CInputHandler::GetInstance()->UnLinkKeyDown(m_hKeyDown);
	}

	GetSceneManager()->SetDistanceFilter(TRUE);

	// event unlink
	UnLinkMsg(g_EventUpdateTick);
	UnLinkMsg(g_EventCharMake);
	UnLinkMsg(g_EventLobbyMessage);
	UnLinkMsg(g_EventCharStageStateEnter);
	UnLinkMsg(g_EventLobbyServerConnect);

	DestroyAvatar();

	if(m_bWorldAddCamera)
	{
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
		m_bWorldAddCamera = FALSE;
	}

	if(m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}

	if(m_pGuiGroup)
	{
		m_pGuiGroup->Destroy();

		NTL_DELETE( m_pGuiGroup );
	}

	CCharStageState::DestroyInstance();

	NTL_RETURNVOID();
}

void CLobbyStage::RequestLobbyInfo()
{
	SConnectData *pConData = GetDboGlobal()->GetConnectData();

	// request character & channel info
	if(pConData->byLastServerFarmID == INVALID_SERVERFARMID )
	{
		// 마지막으로 접속한 서버 정보가 없다면 모든 서버의 목록을 요청한다
		GetDboGlobal()->GetLobbyPacketGenerator()->SendCharServerListReq();	

		GetCharStageState()->ChangeState(CHAR_STATE_SELECT_INFOMATION);

		// 기본 지형 생성
		SetupDefaultCamerabyRace(dRACE_NONE);
		CreateBackGroundStage((RwUInt32)INVALID_INDEX);
		TeleportStage();
	}
	else
	{
		// 마지막으로 접속한 서버가 있을 때에는 그 서버의 정보만을 요청한다
		GetDboGlobal()->GetLobbyPacketGenerator()->SendCharServerListOneReq();

		// 캐릭터 정보를 요청한다. 다음의 3가지 패킷이 응답으로 온다
		// CU_SERVER_CHANNEL_INFO, CU_CHARACTER_INFO_RES, CU_CHARACTER_LOAD_RES	
		GetDboGlobal()->GetLobbyPacketGenerator()->SendCharLoadReq(pConData->uiAccountId, pConData->byLastServerFarmID);

		// 현재 선택된 서버	
		GetLobbyManager()->SetSelectedServerHandle(pConData->byLastServerFarmID);	

		GetCharStageState()->ChangeState(CHAR_STATE_SELECT_INFOMATION);
	}
}

void CLobbyStage::HandleEvents(RWS::CMsg &pMsg)
{	
	if(pMsg.Id == g_EventUpdateTick)
	{
		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		EventProcUpdateTick(fElapsed);
	}
	else if( pMsg.Id == g_EventCharMake )
	{
		SNtlEventCharMake* pCharMake = reinterpret_cast<SNtlEventCharMake*>(pMsg.pData);

		if( pCharMake->bSetDefaultCamera )
			SetupDefaultCamerabyRace(dRACE_NONE);

		CreateDummyAvatar( pCharMake->byRace, pCharMake->byClass, pCharMake->byGender, pCharMake->byFace, pCharMake->byHair, pCharMake->bySkinColor, pCharMake->byHairColor);
		TeleportStage();
	}
	else if( pMsg.Id == g_EventLobbyMessage )
	{
		SDboEventLobbyMessage* pLobbyMessage = reinterpret_cast<SDboEventLobbyMessage*>(pMsg.pData);
		if(pLobbyMessage->byMessage == LMT_SELECT_CHARACTER)
		{
			SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
			if( !pLobby )
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				NTL_RETURNVOID();
			}

			RwUInt32 uiCharIndex = (RwUInt32)pLobbyMessage->fValue;
			if( uiCharIndex == INVALID_INDEX || uiCharIndex >= NTL_MAX_COUNT_USER_CHAR_SLOT ||
				pLobby->GetCharacterCount() == 0 )
			{
				SetupDefaultCamerabyRace(dRACE_NONE);
				CreateBackGroundStage((RwUInt32)INVALID_INDEX);
				TeleportStage();

				DestroyAvatar();
			}
			else
			{
				sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter((RwUInt8)uiCharIndex);
				if( !pLOBBY_CHARACTER )
				{
					DBO_FAIL("Not exist character of index : " << uiCharIndex);
					NTL_RETURNVOID();
				}

				sPC_SUMMARY& charInfo = pLOBBY_CHARACTER->tSummary;

				// CreateDownLoadAvatar함수에서 사용하기 위한 아바타의 위치 지정을 위해 호출
				SetupDefaultCamerabyRace(charInfo.byRace);

				CreateBackGroundStage(charInfo.byRace);
				CreateDownLoadAvatar(&charInfo);

				// 생성된 아바타의 키에 따른 어른, 아이별 카메라 좌표 지정
				SetupDefaultCamerabyRace(charInfo.byRace);
				TeleportStage();

				// 카메라를 기본 위치로
				API_PL_CameraLookAt(CNtlPLGlobal::m_RwCamera, &m_vCameraPos, &m_vCameraPosAt);
			}
		}
		else if(pLobbyMessage->byMessage == LMT_ROTATION_LEFT ||
			pLobbyMessage->byMessage == LMT_ROTATION_RIGHT ||
			pLobbyMessage->byMessage == LMT_ROTATION_STOP)
		{
			m_iAvatarRotState = (RwInt32)pLobbyMessage->byMessage;
			m_fRotSpeedPerSec = pLobbyMessage->fValue;
		}
		else if(pLobbyMessage->byMessage == LMT_ROTATION)
		{
			if( m_pAvatar )
			{
				CNtlSobProxy* pSobProxy = m_pAvatar->GetSobProxy();
				CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
				pCharacter->SetAngleY(pCharacter->GetAngleY() + pLobbyMessage->fValue);
			}
		}
		else if(pLobbyMessage->byMessage == LMT_ROTATION_FOREWARD)
		{
			m_iAvatarRotState = (RwInt32)pLobbyMessage->byMessage;
			if(m_pAvatar)
			{
				CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
				if(pSobProxy)
				{
					CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
					if(pCharacter)
					{
						pCharacter->SetAngleY(m_fAvatarAngleY);
					}
				}
			}
		}
		else if(pLobbyMessage->byMessage == LMT_ZOOM_IN)
		{
			if( m_fReservatedZoomStep >= m_fTotalZoomStep - 1 )
				return;

			sCameraZoom cameraZoom;
			RwV3d v3Pos, v3Dir;

			// 카메라 위치
			RwV3dSubMacro(&v3Pos, &m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_IN].v3Pos,
				&m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos);			

			RwV3dSubMacro(&v3Dir, &m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_IN],
				&m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT]);

			RwV3dNormalize(&v3Pos, &v3Pos);
			RwV3dNormalize(&v3Dir, &v3Dir);

			// Src
			cameraZoom.v3CameraSrcPos = m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos
				+ ( v3Pos * (m_fTotalDistance*m_fReservatedZoomStep/m_fTotalZoomStep) );

			cameraZoom.v3CameraSrcDir = m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT]
			+ ( v3Dir * (m_fTotalDirDistance*m_fReservatedZoomStep/m_fTotalZoomStep) );

			// Dest

			m_fReservatedZoomStep += 1.f;

			cameraZoom.v3CameraDestPos = m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos
				+ ( v3Pos * (m_fTotalDistance*m_fReservatedZoomStep/m_fTotalZoomStep) );

			cameraZoom.v3CameraDestDir = m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT]
			+ ( v3Dir * (m_fTotalDirDistance*m_fReservatedZoomStep/m_fTotalZoomStep) );

			m_listZoomMode.push_back(cameraZoom);
		}
		else if(pLobbyMessage->byMessage == LMT_ZOOM_OUT)
		{
			if( m_fReservatedZoomStep <= 0 )
				return;

			sCameraZoom cameraZoom;
			RwV3d v3Pos, v3Dir;

			// 카메라 위치
			RwV3dSubMacro(&v3Pos, &m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_IN].v3Pos,
				&m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos);			

			RwV3dSubMacro(&v3Dir, &m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_IN],
				&m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT]);

			RwV3dNormalize(&v3Pos, &v3Pos);
			RwV3dNormalize(&v3Dir, &v3Dir);

			// Src
			cameraZoom.v3CameraSrcPos = m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos
				+ ( v3Pos * (m_fTotalDistance*m_fReservatedZoomStep/m_fTotalZoomStep) );

			cameraZoom.v3CameraSrcDir = m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT]
			+ ( v3Dir * (m_fTotalDirDistance*m_fReservatedZoomStep/m_fTotalZoomStep) );

			// Dest

			m_fReservatedZoomStep -= 1.f;

			cameraZoom.v3CameraDestPos = m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos
				+ ( v3Pos * (m_fTotalDistance*m_fReservatedZoomStep/m_fTotalZoomStep) );

			cameraZoom.v3CameraDestDir = m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT]
			+ ( v3Dir * (m_fTotalDirDistance*m_fReservatedZoomStep/m_fTotalZoomStep) );

			m_listZoomMode.push_back(cameraZoom);
		}
		else if(pLobbyMessage->byMessage == LMT_ZOOM_FAR)
		{
		}
		else if(pLobbyMessage->byMessage == LMT_ZOOM_DEFUALT)
		{
			SERVER_HANDLE hServer = GetLobbyManager()->GetSelectedServerHandle();

			if( hServer == INVALID_SERVERFARMID )
			{
				// 서버에 처음 접속한 유저의 경우
				SetupDefaultCamerabyRace(dRACE_NONE);
				TeleportStage();
			}
			else
			{
				CLobby* pLobby = GetLobbyManager()->GetLobby(hServer);
				if( !pLobby )
				{
					DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
					NTL_RETURNVOID();
				}

				RwUInt8				bySelectedCharacterIndex	= pLobby->GetSelectedCharacterIndex();
				sLOBBY_CHARACTER*	pLOBBY_CHARACTER			= pLobby->GetCharacter(bySelectedCharacterIndex);
				if( !pLOBBY_CHARACTER )
					NTL_RETURNVOID();

				SetupDefaultCamerabyRace(pLOBBY_CHARACTER->tSummary.byRace);
				TeleportStage();
			}			

			API_PL_CameraLookAt(CNtlPLGlobal::m_RwCamera, &m_vCameraPos, &m_vCameraPosAt);
		}
		else if(pLobbyMessage->byMessage == LMT_ZOOM_FROM_FAR_TO_DEFUALUT)
		{
		}
		else if(pLobbyMessage->byMessage == LMT_DESTORY_LOBBY_AVATAR)
		{
			DestroyAvatar();
		}
		else if(pLobbyMessage->byMessage == LMT_DESTORY_LOBBY_WORLD)
		{
			DestroyAvatar();
			DestroyBackGroundStage();
		}
	}
	else if( pMsg.Id == g_EventCharStageStateEnter)
	{
		SDboEventCharStageStateEnter* pStageStateEnter = reinterpret_cast<SDboEventCharStageStateEnter*>(pMsg.pData);

		if(pStageStateEnter->byState == CHAR_STATE_MAKE_ENTER)
		{
			SetupDefaultCamerabyRace(dRACE_NONE);
			CreateBackGroundStage((RwUInt32)INVALID_INDEX);
			TeleportStage();
		}
		else if(pStageStateEnter->byState == CHAR_STATE_MAKE_EXIT)
		{
			SERVER_HANDLE hServer = GetLobbyManager()->GetSelectedServerHandle();

			if( hServer == INVALID_SERVERFARMID )
			{
				// 서버에 처음 접속한 유저의 경우
				SetupDefaultCamerabyRace(dRACE_NONE);
				TeleportStage();
			}
		}
	}
	else if( pMsg.Id == g_EventLobbyServerConnect)
	{
		SDboEventServerConnect *pConnect = reinterpret_cast<SDboEventServerConnect*>( pMsg.pData );

		if( pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT )
			GetAlarmManager()->AlarmMessage( "DST_SERVER_DISCONNECT" );
	}
}

void CLobbyStage::EventProcUpdateTick(RwReal fElapsed)
{
	//Player Position을 제대로 Update를 해주어야 하늘이 제대로 나온다.
	if(m_pWorldEntity != NULL)
	{
		m_pWorldEntity->SetPlayerPosition(m_vAvatarPos);

		// World 가 Ready 상태일 때 한번만 적용.
		if(m_pWorldEntity->GetWorldReady())
		{
			// 로비에 옵션 적용			
			if( !m_bOptionApply )
			{
				if(GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_SHADER_HDR ))
					CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
				else
					CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);

				m_bOptionApply = TRUE;				
			}
		}
	}

	GetCharStageState()->Update(fElapsed);

	// 카메라 Zoom in/out
	if( m_listZoomMode.size() > 0 )
	{
		std::list<sCameraZoom>::iterator it = m_listZoomMode.begin();
		sCameraZoom& cameraZoom = *it;

		// 업데이트 타임
		cameraZoom.fElapsed += fElapsed;

		if( cameraZoom.fElapsed > dCAMERA_MOVE_TIME )
			cameraZoom.fElapsed = dCAMERA_MOVE_TIME;

		RwV3d v3CameraCurPos = CNtlMath::Interpolation(cameraZoom.v3CameraSrcPos, cameraZoom.v3CameraDestPos, cameraZoom.fElapsed / dCAMERA_MOVE_TIME);
		RwV3d v3CameraCurDir = CNtlMath::Interpolation(cameraZoom.v3CameraSrcDir, cameraZoom.v3CameraDestDir, cameraZoom.fElapsed / dCAMERA_MOVE_TIME);

		API_PL_CameraLookAt(CNtlPLGlobal::m_RwCamera, &v3CameraCurPos, &v3CameraCurDir);

		// 초기화
		if( cameraZoom.fElapsed >= dCAMERA_MOVE_TIME )
			m_listZoomMode.erase(it);
	}

	if(m_pAvatar != NULL && m_iAvatarRotState != LMT_ROTATION_STOP)
	{
		CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
		CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();

		if(pCharacter)
		{
			if(m_iAvatarRotState == LMT_ROTATION_LEFT)
			{
				pCharacter->SetAngleY(pCharacter->GetAngleY() - m_fRotSpeedPerSec * fElapsed);
			}
			else if(m_iAvatarRotState == LMT_ROTATION_RIGHT)
			{
				pCharacter->SetAngleY(pCharacter->GetAngleY() + m_fRotSpeedPerSec * fElapsed);
			}
		}
	}
}

void CLobbyStage::OnTimer(void)
{

}

void CLobbyStage::DestroyBackGroundStage()
{
	if(m_bWorldAddCamera)
	{
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
		m_bWorldAddCamera = FALSE;
	}

	if(m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}
}

void CLobbyStage::SetupDefaultCamerabyRace(RwUInt8 byRace)
{
	// 서버 셀렉트 화면등 캐릭터가 없을 수도 있다
	if(m_pAvatar)
	{
		// 어른과 아이에 따라 카메라를 캐릭터 쪽으로 최대한 당겼을 때 카메라의 위치를
		// 달리한다
		if( Logic_IsChild(m_pAvatar) )
		{
			m_byCharHeightType = CHT_CHILD;
		}
		else
		{
			m_byCharHeightType = CHT_ADULT;			
		}
	}


	if(byRace == RACE_HUMAN)
	{
		m_vAvatarPos.x = -350.37f;
		m_vAvatarPos.y = 52.0f;
		m_vAvatarPos.z = 328.73f;

		m_vCameraPos	= m_CameraData[RACE_HUMAN][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos;
		m_vCameraPosAt	= m_v3Dir[RACE_HUMAN][m_byCharHeightType][ZL_ZOOM_OUT];

		m_fAvatarAngleY = 138.f;// 145.f;
	}
	else if(byRace == RACE_NAMEK)
	{
		m_vAvatarPos.x = 471.69f;
		m_vAvatarPos.y = 52.0f;
		m_vAvatarPos.z = 300.63f;

		m_vCameraPos	= m_CameraData[RACE_NAMEK][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos;
		m_vCameraPosAt	= m_v3Dir[RACE_NAMEK][m_byCharHeightType][ZL_ZOOM_OUT];

		m_fAvatarAngleY = 143.f;// 150.f;
	}
	else if(byRace == RACE_MAJIN)
	{
		m_vAvatarPos.x = 365.617f;
		m_vAvatarPos.y = 52.0f;
		m_vAvatarPos.z = -365.843f;

		m_vCameraPos	= m_CameraData[RACE_MAJIN][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos;
		m_vCameraPosAt	= m_v3Dir[RACE_MAJIN][m_byCharHeightType][ZL_ZOOM_OUT];

		m_fAvatarAngleY = 135.f;// 137.f;
	}
	else
	{
		// If the character does not exist, or if you are making a character
		m_byCharHeightType = CHT_CHILD;

		m_vAvatarPos.x = -296.22f;
		m_vAvatarPos.y = 52.577f;
		m_vAvatarPos.z = -281.39f;

		m_vCameraPos	= m_CameraData[dRACE_NONE][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos;
		m_vCameraPosAt	= m_v3Dir[dRACE_NONE][m_byCharHeightType][ZL_ZOOM_OUT];

		m_fAvatarAngleY = 50.f;// 55.f;
	}

	m_byRace = byRace;	
	m_listZoomMode.clear();
	m_fReservatedZoomStep = 0.f;


	// RACE_COUNT == dRACE_NONE
	if( RACE_COUNT >= m_byRace )
	{
		RwV3d v3Temp;

		// 카메라가 움직일 최대 거리
		RwV3dSubMacro(&v3Temp, &m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT].v3Pos,
			&m_CameraData[m_byRace][m_byCharHeightType][ZL_ZOOM_IN].v3Pos);

		m_fTotalDistance		= RwV3dLength(&v3Temp);
		m_fTotalZoomStep		= (RwReal)(RwUInt8)(m_fTotalDistance / dCAMERA_STEP_DISTANCE);

		// 카메라 시점이 움직일 최대 거리
		RwV3dSubMacro(&v3Temp, &m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_OUT],
			&m_v3Dir[m_byRace][m_byCharHeightType][ZL_ZOOM_IN]);

		m_fTotalDirDistance		= RwV3dLength(&v3Temp);
	}
}

void CLobbyStage::CreateBackGroundStage( RwUInt32 uiRace, RwBool bForce /* = FALSE */)
{
	if( m_pAvatar && bForce == FALSE )
	{
		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(m_pAvatar->GetSobAttr());
		if( pAvatarAttr->GetRace() == uiRace )
		{
			GetCharStageState()->SetReadyWorld(TRUE);
			return;
		}
	}

	if(m_pWorldEntity == NULL)
	{
		_getcwd(dGET_WORLD_PARAM()->CurWorkingFolderName, 256);
		strcpy(dGET_WORLD_PARAM()->CurDefaultTexPath, dGET_WORLD_PARAM()->CurWorkingFolderName);
		strcat(dGET_WORLD_PARAM()->CurDefaultTexPath, "\\texture\\ntlwe\\");

		std::string str = "\\world\\";	
		str += "DBO_Select_Character";

		strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, dGET_WORLD_PARAM()->CurWorkingFolderName);
		strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, str.c_str());

		str += "\\";
		str += "#######.gwp";

		FILE* pFile = NULL;
		SPackResFileData sPackFileData;
		RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(str.c_str(), sPackFileData);

		if(bPack)
		{
			pFile = fopen(sPackFileData.strPackFileName.c_str(), "rb");
			if(pFile)
				fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
		}
		else
		{
			std::string strWorldFileName = ".";
			strWorldFileName += str;
			pFile = fopen(strWorldFileName.c_str(), "rb");
		}

		// create a global world param
		if(pFile)
		{
			LoadFileWorldState(dGET_WORLD_PARAM(), pFile);
			fclose(pFile);
		}

		DBO_ASSERT(dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION), "invalid world resource version. world(" << dGET_WORLD_PARAM()->WorldProjectFolderName << ") support_version(" << dNTL_WORLD_VERSION << ") resource_version(" << dGET_WORLD_PARAM()->WorldVer << ")");

		m_pWorldEntity = static_cast<CNtlPLWorldEntity*>(GetSceneManager()->CreateEntity(PLENTITY_WORLD, "NtlWorld"));
		m_pWorldEntity->CreateWorld(m_vAvatarPos);

		RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
		m_bWorldAddCamera = TRUE;

		GetNtlGameCameraManager()->SetFov(60.f);  // 80 in tw
	}

	GetCharStageState()->SetReadyWorld(TRUE);
}

void CLobbyStage::CreateDummyAvatar( BYTE byRace, BYTE byClass, BYTE byGender, BYTE byFace, BYTE byHair, BYTE bySkinColor, BYTE byHairColor)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	RwReal fCurAnimTime = 0.f;
	if(m_pAvatar)
	{
		CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
		CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
		fCurAnimTime = pCharacter->GetBaseCurrentAnimTime();
		DestroyAvatar();
	}

	SERIAL_HANDLE hSerialId = 1;
	pAvatarInfo->uiSerialId = hSerialId++;		

	CPCTable *pPcTable = API_GetTableContainer()->GetPcTable();
	sPC_TBLDAT *pTblData = (sPC_TBLDAT*)pPcTable->GetPcTbldat(byRace, byClass, byGender);   
	pAvatarInfo->sCharPf.tblidx = pTblData->tblidx;	

	pAvatarInfo->sCharPf.sPcShape.byFace = byFace;
	pAvatarInfo->sCharPf.sPcShape.byHair = byHair;
	pAvatarInfo->sCharPf.sPcShape.byHairColor = byHairColor;
	pAvatarInfo->sCharPf.sPcShape.bySkinColor = bySkinColor;
	pAvatarInfo->sCharPf.bIsAdult = FALSE;


	pAvatarInfo->sCharPf.curLp = 100;

	pAvatarInfo->sCharPf.wCurEP = 100;

	pAvatarInfo->sCharPf.curAP = 450000;

	pAvatarInfo->sCharPf.wCurRP = 100;

	pAvatarInfo->sCharPf.byLevel = 1;
	pAvatarInfo->sCharPf.dwCurExp = 1;

	pAvatarInfo->bySkillCount = 0;
	pAvatarInfo->byHTBCount = 0;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  
	pAvatarInfo->sCharState.sCharStateBase.byStateID = CHARSTATE_STANDING;

	pAvatarInfo->sOldWorldInfo.tblidx = 0;
	pAvatarInfo->sOldWorldInfo.worldID = 0;

	pAvatarInfo->sWorldInfo.tblidx = 0;
	pAvatarInfo->sWorldInfo.worldID = 0;

	// Newbie Item Create
	CNewbieTable *pNewbieTable = API_GetTableContainer()->GetNewbieTable();
	sNEWBIE_TBLDAT *pNewbieTblData = reinterpret_cast<sNEWBIE_TBLDAT*>( pNewbieTable->GetNewbieTbldat(byRace, byClass) );
	NTL_ASSERT(pNewbieTblData, "CLobbyStage::CreateDummyAvatar, Not exist Newbie table data of race : " << byRace << " and class : " << byClass);

	pAvatarInfo->wItemCount = 0;
	for(RwInt32 i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
	{
		if(pNewbieTblData->aitem_Tblidx[i] == INVALID_TBLIDX)
			continue;

		if(pNewbieTblData->abyPos[i] == EQUIP_SLOT_TYPE_UNKNOWN)
			continue;

		sITEM_TBLDAT* pItemTbl = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pNewbieTblData->aitem_Tblidx[i]);
		NTL_ASSERT(pItemTbl, "CLobbyStage::CreateDummyAvatar, item tblidx : " << pNewbieTblData->aitem_Tblidx[i] << " does not exist");

		if (pItemTbl->byEquip_Type == EQUIP_SLOT_TYPE_UNKNOWN) // do not create items that can not be equiped
			continue;

		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].handle				= hSerialId++;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].tblidx				= pNewbieTblData->aitem_Tblidx[i];
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byPlace				= CONTAINER_TYPE_EQUIP;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byPos					= pNewbieTblData->abyPos[i];
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byStackcount			= 1;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byRank				= pItemTbl->byRank;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byCurDur				= pItemTbl->byDurability;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].sOptionSet.Init();
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byRestrictState		= ITEM_RESTRICT_STATE_TYPE_NONE;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].bNeedToIdentify		= false;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byGrade				= 0;
		pAvatarInfo->wItemCount++;
	}

	m_pAvatar = reinterpret_cast<CNtlSobAvatar*>(CNtlSLEventGenerator::SobAvatarCreate(SLCLASS_AVATAR, pAvatarInfo->uiSerialId, &pAvatarInfo->sCharState));

	if(m_pAvatar)
	{
		m_pAvatar->SetPosition(&m_vAvatarPos);
		m_pAvatar->SetAngleY(m_fAvatarAngleY);
		CNtlPLCharacter *pCharacterEntity = (CNtlPLCharacter *)m_pAvatar->GetSobProxy()->GetPLMainEntity();
		pCharacterEntity->SetBaseCurrentAnimTime(fCurAnimTime);
		m_pAvatar->GetSobProxy()->AddWorld();
	}

	GetCharStageState()->SetReadyModel(TRUE);
}

void CLobbyStage::CreateDownLoadAvatar(sPC_SUMMARY *pCharInfo)
{
	CPCTable *pPcTable = API_GetTableContainer()->GetPcTable();
	sPC_TBLDAT *pTblData = (sPC_TBLDAT*)pPcTable->GetPcTbldat(pCharInfo->byRace, pCharInfo->byClass, pCharInfo->byGender);   

	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	RwReal fCurAnimTime = 0.f;
	RwUInt32 uiAnimKey = NML_IDLE_LOOP;

	if(m_pAvatar)
	{
		CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
		CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
		fCurAnimTime = pCharacter->GetBaseCurrentAnimTime();
		uiAnimKey = pCharacter->GetCurBaseAnimKey();

		DestroyAvatar();
	}

	pAvatarInfo->sCharPf.tblidx = pTblData->tblidx;	
	pAvatarInfo->uiSerialId = pCharInfo->charId;

	SERIAL_HANDLE hSerialId = pAvatarInfo->uiSerialId;
	hSerialId = hSerialId++;

	pAvatarInfo->sCharPf.sPcShape.byFace = pCharInfo->byFace;
	pAvatarInfo->sCharPf.sPcShape.byHair = pCharInfo->byHair;
	pAvatarInfo->sCharPf.sPcShape.byHairColor = pCharInfo->byHairColor;
	pAvatarInfo->sCharPf.sPcShape.bySkinColor = pCharInfo->bySkinColor;
	pAvatarInfo->sCharPf.bIsAdult = pCharInfo->bIsAdult;


	pAvatarInfo->sCharPf.curLp = 100;
	pAvatarInfo->sCharPf.wCurEP = 100;
	pAvatarInfo->sCharPf.wCurRP = 100;

	pAvatarInfo->sCharPf.byLevel = 1;
	pAvatarInfo->sCharPf.dwCurExp = 1;    

	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  

	pAvatarInfo->wItemCount = 0;
	pAvatarInfo->bySkillCount = 0;
	pAvatarInfo->byHTBCount = 0;

	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  
	pAvatarInfo->sCharState.sCharStateBase.byStateID = CHARSTATE_STANDING;

	pAvatarInfo->sOldWorldInfo.tblidx = 0;
	pAvatarInfo->sOldWorldInfo.worldID = 0;

	pAvatarInfo->sWorldInfo.tblidx = 0;
	pAvatarInfo->sWorldInfo.worldID = 0;

	pAvatarInfo->wItemCount = 0;
	for(RwInt32 i = 0; i < EQUIP_SLOT_TYPE_COUNT; i++)
	{
		if(pCharInfo->sItem[i].tblidx == INVALID_TBLIDX)
			continue;

		sITEM_TBLDAT* pItemTblData = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pCharInfo->sItem[i].tblidx);
		if( pItemTblData == NULL || EQUIP_TYPE_FIRST > pItemTblData->byEquip_Type || EQUIP_TYPE_LAST < pItemTblData->byEquip_Type )
			continue;

		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].handle				= hSerialId++;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].tblidx				= pCharInfo->sItem[i].tblidx;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byPlace				= CONTAINER_TYPE_EQUIP;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byPos				= (BYTE)i;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byStackcount			= (BYTE)1;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byRank				= pCharInfo->sItem[i].byRank;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byCurDur				= 100;			// 여기는 채워 넣으시면 됩니다.
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].sOptionSet.Init();
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byRestrictState = ITEM_RESTRICT_STATE_TYPE_NONE;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].bNeedToIdentify		= false;
		pAvatarInfo->sItemPf[pAvatarInfo->wItemCount].byGrade				= pCharInfo->sItem[i].byGrade;
		pAvatarInfo->wItemCount++;
	}

	m_pAvatar = reinterpret_cast<CNtlSobAvatar*>(CNtlSLEventGenerator::SobAvatarCreate(SLCLASS_AVATAR, pAvatarInfo->uiSerialId, &pAvatarInfo->sCharState));

	if(m_pAvatar)
	{
		m_pAvatar->SetPosition(&m_vAvatarPos);
		m_pAvatar->SetAngleY(m_fAvatarAngleY);
		CNtlPLCharacter *pCharacterEntity = (CNtlPLCharacter *)m_pAvatar->GetSobProxy()->GetPLMainEntity();
		pCharacterEntity->SetBaseCurrentAnimTime(fCurAnimTime);
		//		m_pAvatar->GetSobProxy()->AddWorld();

		m_pAvatar->SetGuildDogiColorIndex( pCharInfo->sDogi.byGuildColor );
		m_pAvatar->SetDojoDogiColorIndex( pCharInfo->sDogi.byDojoColor );

		Logic_SetDogiItemColor((CNtlSobPlayer*)m_pAvatar, m_pAvatar->GetRealDogiColorIndex(), FALSE, TRUE);
	}

	GetCharStageState()->SetReadyModel(TRUE);
}

void CLobbyStage::TeleportStage()
{
	// World의 Positon 변경이 한번에 2번 변경이 되면
	// World의 Entity에 문제가 생기므로 Avatar를 기준으로
	// 삼아 Avatar의 위치가 변경이 생길 경우만 위치 변경을 한단( by HoDong )
	if( m_vAvatarPos.x != m_vAvatarUpdatePos.x ||
		m_vAvatarPos.y != m_vAvatarUpdatePos.y ||
		m_vAvatarPos.z != m_vAvatarUpdatePos.z )
	{
		m_pWorldEntity->SetPortalPosition(m_vAvatarPos);
		m_pWorldEntity->SetPlayerPosition(m_vAvatarPos);
		m_pWorldEntity->Update(0.8f);
		API_PL_CameraLookAt(CNtlPLGlobal::m_RwCamera, &m_vCameraPos, &m_vCameraPosAt);

		m_vAvatarUpdatePos = m_vAvatarPos;
	}
}

void CLobbyStage::DestroyAvatar()
{
	if(m_pAvatar)
	{
		SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
		CNtlSLEventGenerator::SobDelete(pAvatarInfo->uiSerialId);
		m_pAvatar = NULL;
		GetNtlSLGlobal()->SetSobAvatar(NULL);
	}	
}


