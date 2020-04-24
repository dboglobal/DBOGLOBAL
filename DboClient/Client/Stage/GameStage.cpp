#include "precomp_dboclient.h"
#include "GameStage.h"

// core 
#include "NtlDebug.h"
#include "NtlMath.h"

// shared
#include "WorldTable.h"

// sound
#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"
#include "NtlPLResourceManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLObject.h"
#include "NtlPLGlobal.h"
#include "NtlPLPlayerName.h"
#include "NtlPLSceneManager.h"
#include "ntlworldcommon.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLDamageBox.h"
#include "NtlPostEffectCamera.h"
#include "NtlPLRenderState.h"
#include "NtlPLResource.h"
#include "NtlPLResourcePack.h"
#include "NtlPLSkySphere.h"
#include "NtlPLResourceScheduling.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobItem.h"
#include "NtlSobActorCtrlData.h"
#include "InputActionMap.h" 
#include "NtlSLLuaGlue.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobAvatarAttr.h"
#include "NtlCameraManager.h"
#include "NtlSLApi.h"
#include "NtlSobManager.h"
#include "NtlWorldConcept.h"
#include "DboTSCMain.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobProxy.h"
#include "NtlDTCinematicManager.h"
#include "NtlSoundMoodManager.h"
#include "NtlStorageGroupScouter.h"
#include "NtlStorageGroupQuest.h"
#include "NtlStorageManager.h"

// Storage
#include "NtlStorageManager.h"

// dbo
#include "DboDef.h"
#include "GameGuiGroup.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboApplication.h"
#include "CursorManager.h"
#include "PacketProc.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "GameLoadingScene.h"
#include "GameLoadingThread.h"
#include "SurfaceGui.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "MiniMapGui.h"
#include "GameStageState.h"
#include "PetitionManager.h"
#include "MsgBoxManager.h"
#include "LobbyManager.h"

// discord
#ifdef USE_DISCORD
#include "Discord.h"
#endif


void CallbackGuiInputBoxFocus(bool bGotFocus)
{
	if(bGotFocus)
	{
		GetInputActionMap()->Reset();
		GetInputActionMap()->SetActive(FALSE);
	}
	else
	{
		GetInputActionMap()->SetActive(TRUE);
	}
}

CGameStage::CGameStage(const char *pStageName)
:CNtlStage(pStageName)
{
	m_pTSCMain					= NULL;
	m_pGuiGroup					= NULL;
	m_pWorldEntity				= NULL;
	m_pAvatar					= NULL;
	m_bWorldReady				= FALSE;
	m_bAvatarReady				= FALSE;
	m_bCharReadyCommunityServer	= FALSE;
	m_bWorldChange				= FALSE;

	m_pLoadingThread			= NULL;
	m_pLoadingScene				= NULL;
	m_pTeleportScene			= NULL;

	m_eUpdageType = EGUT_GAME_ENTER_LOADING;
	m_bCheckResourceScheduling	= TRUE;
}

CGameStage::~CGameStage()
{
}

bool CGameStage::Create(void)
{
	NTL_FUNCTION("CGameStage::Create" );

	Logic_LoadScouterOption();
	Logic_LoadQuestOption();
	Logic_LoadCharacterOption();

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		NTL_RETURN(FALSE);
	}

	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURN(FALSE);
	}

	RwUInt8 byRace = pLOBBY_CHARACTER->tSummary.byRace;

	GetCursorManager()->SetRace( byRace );
	GetCursorManager()->InitGameCursor();

	PreMutiThreadLoading();


#ifdef USE_DISCORD

	char* charname = Ntl_WC2MB(pLOBBY_CHARACTER->tSummary.awchName);
	char* className = Ntl_WC2MB(Logic_GetClassName(pLOBBY_CHARACTER->tSummary.byClass));

	GetDiscordManager()->SetHideName(GetNtlStorageManager()->GetBoolData(dSTORAGE_CHARACTER_ETC_DISCORD_HIDE_NAME), false);
	GetDiscordManager()->LoginChannel(pLobby->GetSelectedChannelIndex(), charname, pLOBBY_CHARACTER->tSummary.byLevel, className, pLOBBY_CHARACTER->tSummary.byClass);

	Ntl_CleanUpHeapString(charname);
	Ntl_CleanUpHeapString(className);

#endif

	
	NTL_RETURN(TRUE);
}

void CGameStage::Destroy(void)
{
    GetSceneManager()->SetSoundListener(NULL);

	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);

	NTL_FUNCTION("CGameStage::Destroy" );

	if(m_pStageState)
	{
		NTL_DELETE(m_pStageState);
	}

	GetPetitionManager()->EndGMChatting();

	GetNtlWorldConcept()->DeleteGradeWorldPlayConcept(WORLD_CONCEPT_FIRST_GRADE);
	GetNtlWorldConcept()->DeleteGradeWorldPlayConcept(WORLD_CONCEPT_SECOND_GRADE);

	//GetNtlGuiManager()->RemoveAllUpdateFunc();
	//GetNtlGuiManager()->RemoveAllUpdateBeforeFunc();
	GetMsgBoxManager()->DeleteAllMsgBoxExceptNetConnectBox();

	GetNtlResourceManager()->SetLoadScheduling(FALSE);
	GetNtlResourceManager()->SetDeleteScheduling(FALSE);
	SetLoadObjectSeamlessScheduling(FALSE);

	// loading scene
	if(m_pLoadingScene)
	{
		NTL_DELETE(m_pLoadingScene);
	}

	// teleport direct
	if(m_pTeleportScene)
	{
		NTL_DELETE(m_pTeleportScene);
	}
	
	// 세이브 방식 변경 ( 저장 후 메모리 삭제 )
	Logic_SaveScouterOption();
	Logic_SaveQuestOption();
	Logic_SaveCharacterOption();

	CNtlStorageGroupScouter* pScouterGroup = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
	pScouterGroup->Clear();

	CNtlStorageGroupQuest* pQuestGroup = (CNtlStorageGroupQuest*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_QUEST );
	pQuestGroup->Clear();
	
	// event unlink
	UnLinkMsg(g_EventUpdateTick);
	UnLinkMsg(g_EventWorldChange);
	UnLinkMsg(g_EventGameServerConnect);
	UnLinkMsg(g_EventDebug_for_Developer);
	UnLinkMsg(g_EventEndterWorld);
	UnLinkMsg(g_EventLoading);

	// charset release
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAvatarPos);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAvatarDir);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAvatarFightingMode);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAvatarAirMode);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAvatarMoveSpeed);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAvatarAttackSpeed);
    CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAvatarState);

	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetFieldIdx);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetSectorIdx);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAvatarAngle);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetPlayerCount);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetMonsterCount);

	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetTargetLoc);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetTargetDir);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetTargetMoveSpeed);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetTargetAttackSpeed);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetTargetState);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetFightingMode);
	CNtlCharsetPrint::GetInstance()->Release(m_hCharsetAirMode);

	// unlink keyboard/mouse down
	CInputHandler::GetInstance()->UnLinkKeyDown(m_hKeyboardDown);
	
	// 서버로 나간다고 알림.
	CGamePacketGenerator *pGameNetSender = CDboGlobal::GetInstance()->GetGamePacketGenerator();  
	pGameNetSender->SendGameLeaveReq(); 
	

	// trigger system main destory
	if(m_pTSCMain)
	{
		m_pTSCMain->Delete();
		NTL_DELETE(m_pTSCMain);
	}

	// avatar controller
	m_AvatarController.Destroy();

	API_SLReset();
		
	API_SLResourceTerminate();

	
	// Destroy TextureCamera
	CMapItem::DestroyCamera();

	// world에서 camera remove
	if(CNtlPLGlobal::m_pRpWorld)
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	// woody1019;
	DeleteWorld();

	// gui destroy
	if(m_pGuiGroup)
	{
		m_pGuiGroup->Destroy(); 
		NTL_DELETE(m_pGuiGroup);
	}

	UnLinkGuiInputBoxFocus();

	GetDboGlobal()->Reset();

	GetCursorManager()->InitLobbyCursor();

	NTL_RETURNVOID();
}

void CGameStage::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventUpdateTick)
	{
		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		EventProcUpdateTick(fElapsed);
	}
	else if(pMsg.Id == g_EventWorldChange)
	{
		SNtlEventWorldChange *pWorldChange = reinterpret_cast<SNtlEventWorldChange*>( pMsg.pData );
		EventProcWorldChange(pWorldChange->bWorldChange);
	}
	else if(pMsg.Id == g_EventGameServerConnect)
	{
		SDboEventServerConnect *pConnect = reinterpret_cast<SDboEventServerConnect*>( pMsg.pData );
		if( pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT )
			GetAlarmManager()->AlarmMessage( "DST_SERVER_DISCONNECT" );
	}
	else if(pMsg.Id == g_EventDebug_for_Developer)
	{
		sDboDeveloperData* pEvent = reinterpret_cast<sDboDeveloperData*>(pMsg.pData);

		if( pEvent->uiType == DEVELOPER_RESET_GUI )
		{
			ResetGui();
		}
	}
	else if(pMsg.Id == g_EventEndterWorld)
	{
		CWorldTable* pWorldTable = API_GetTableContainer()->GetWorldTable();
		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
		sWORLD_TBLDAT* pWorldTblData = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTable->FindData(pAvatarInfo->sWorldInfo.tblidx) );
		if( NULL == pWorldTblData )
		{
			DBO_FAIL("g_EventEndterWorld, invalid world table index : " << pAvatarInfo->sWorldInfo.tblidx );
			return;
		}

		GetNtlSoundMoodManager()->Condition_EnterWorld(pWorldTblData);
	}
	else if(pMsg.Id == g_EventLoading)
	{
		SDboEventLoading* pEvent = reinterpret_cast<SDboEventLoading*>(pMsg.pData);

		if( m_pTeleportScene )
			m_pTeleportScene->OnLoadingEvent(pEvent->byMessage);

		if( m_pLoadingScene )
			m_pLoadingScene->OnLoadingEvent(pEvent->byMessage);
	}
}

void CGameStage::UpdateLoadingScene(RwReal fElapsed)
{
	if(m_pLoadingScene)
	{
		if(!m_pLoadingScene->Update(fElapsed))
		{
			NTL_DELETE(m_pLoadingScene);
			GetNtlGameCameraManager()->EnableUpdateData(FALSE);
			m_eUpdageType = EGUT_GAME_IDLE;
		}
	}
}

RwBool CGameStage::UpdateLoadingThread(RwReal fElapsed)
{
	// Multi-thread가 동작하고 있지 않는 상태
	if(m_pLoadingThread == NULL)
		return FALSE;

	// Multi-thread loading 완료
	if(m_pLoadingThread->GetLoadState() == CGameLoadingThread::eGAME_LOAD_STATE_LOADED)
	{
		PostMutiThreadLoading();
		return TRUE;
	}

	// Multi-thread loading 중...

	GetSoundManager()->Update(fElapsed);

	return FALSE;
}


RwV3d CGameStage::GetUpdateWorldPosition(void)
{
	RwBool bAvatarCreate = IsAvatarCreate();
	RwV3d vWorldUpdatePos;

	if(GetNtlSLGlobal()->IsWorldUpdateAvaterPosition())
	{
		SGameData *pGameData = GetDboGlobal()->GetGameData();
		if(pGameData->bAvatarReCreate || !bAvatarCreate)
		{
			SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo(); 
			CNtlMath::MathRwV3dAssign(&vWorldUpdatePos, 
									pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x,
									pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y,
									pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z);
		}
		else
		{
			vWorldUpdatePos = m_pAvatar->GetPosition();
		}
	}
	else
	{
		RwV3dAssign(&vWorldUpdatePos, GetNtlGameCameraManager()->GetCameraPos());
	}

	return vWorldUpdatePos;
}

void CGameStage::LoadingEndProc(void)
{
	// In case of not being able to position automatically, we will arrange the initial position once at the start of the game
	GetDialogManager()->Locate_MovableDefaultPosition();

	// Place Gui at the specified location
	GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
		
	
	GetNtlResourceManager()->SetLoadScheduling(TRUE);

	SetLoadObjectSeamlessScheduling(TRUE);
	SetLoadObjectSeamlessSchedulingOneTickTime(0.001f); 
		
	GetSceneManager()->SetThreadLoad( FALSE );
	GetSceneManager()->SetWorldPVSActivation(TRUE);

	CDboEventGenerator::EnterWorld();

	m_bAvatarReady = TRUE;

	// Apply options that will be applied after loading
	/*GetNtlSLSerializeManager()->GetGameOpt()->ApplyGameAfterLoading();*/
	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_ETC, eNTL_STORAGE_APPLY_ALL );
	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_CHAT, eNTL_STORAGE_APPLY_ALL );
	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_CHARACTER, eNTL_STORAGE_APPLY_ALL );
	GetNtlStorageManager()->ApplyStorageType(eNTL_STORAGE_CHARACTER_SCOUTER, eNTL_STORAGE_APPLY_ALL);
}

void CGameStage::UpdateGameEnterLoadingSchedulingWorld(RwReal fElapsed)
{
	RwV3d vWorldUpdatePos = GetUpdateWorldPosition();

	// woody1019
	UpdateWorld(vWorldUpdatePos);

	// The world and thread loading have not finished yet.
	if(m_pLoadingThread || m_pWorldEntity == NULL)
		return;

	// Was the world loaded?
	if(!m_bWorldReady)
	{
		if(!m_pWorldEntity->GetWorldReady())
			return;
		
		m_bWorldReady = TRUE;
	}

	//If it is the first entry into the game, or if it is a teleport, it should be read after all the resources have been read.
	if(m_bCheckResourceScheduling)
	{
		if(GetNtlResourceManager()->IsEmptyLoadScheduling())
		{
			m_bCheckResourceScheduling = FALSE;
			if(!IsAvatarCreate())
			{
				GetSceneManager()->SetThreadLoad( FALSE );
				AvatarCreate();
			}

			// Accept network data.
			CGamePacketGenerator *pGameNetSender = CDboGlobal::GetInstance()->GetGamePacketGenerator();  
			pGameNetSender->SendCharReadyForCommunityServerNfy();
			API_GetSLPacketGenerator()->SendLoadingCompleteNfy();
			API_GetSLPacketGenerator()->SendCharSpawnReady();

			CPacketProc::GetInstance()->ActivePop(TRUE);
			CNtlSLEventGenerator::CharReady();
		}
	}

	// avatar가 아직 생성되지 않았으면?
	if(!IsAvatarCreate())
		return;

	// avatar가 ready 되었는가?
	if(!m_bAvatarReady)
	{
		if(m_pAvatar->IsAvatarReady())
		{
			if(!m_bCheckResourceScheduling)
			{
				if(m_pLoadingScene)
					m_pLoadingScene->GameLoadingFinished();
				
				LoadingEndProc();
			}
		}
	}
}

void CGameStage::UpdateTeleportLoadingNoneScene(RwReal fElapsed)
{
	m_pTeleportScene->SetState(CHAR_TELEPORT_READY);
}

void CGameStage::UpdateTeleportLoadingReadyScene(RwReal fElapsed)
{
	RwV3d vAvatarPos = m_pAvatar->GetPosition();
		
	// 월드 삭제 후 다시 생성.(update 한다.)
	if(m_bWorldChange)
	{
		RwFrameListSetAutoUpdate(FALSE);
		
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

        CNtlSLEventGenerator::CreateWorld(FALSE);       // 이벤트 발생

		DeleteWorld();

		RwV3d vPos, vDir;

		vPos.x = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurLoc.x;
		vPos.y = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurLoc.y;
		vPos.z = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurLoc.z;

		vDir.x = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurDir.x;
		vDir.y = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurDir.y;
		vDir.z = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurDir.z;

		if ( RwV3dNormalize( &vDir, &vDir ) < 0.0001f )
		{
			vDir = m_pAvatar->GetDirection();
		}

		m_pAvatar->SetPosition( &vPos );
		m_pAvatar->SetDirection( &vDir );

		vAvatarPos = vPos;

		CreateWorld(vAvatarPos);

		GetNtlGameCameraManager()->ResetCamera();

		RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

        CNtlSLEventGenerator::CreateWorld(TRUE);        // 이벤트 발생

		RwFrameListSetAutoUpdate(TRUE);

		// world를 한번 업데이트 한다.
		if(m_pWorldEntity)
		{
			//RwBool bEnable = GetLoadObjectSeamlessScheduling();

			//SetLoadObjectSeamlessScheduling( FALSE );

			m_pWorldEntity->SetPlayerPosition(vAvatarPos);
			m_pWorldEntity->Update(0.01f);

			//SetLoadObjectSeamlessScheduling( bEnable );
		}
	}
	else
	{
		RwV3d vPos, vDir;

		vPos.x = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurLoc.x;
		vPos.y = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurLoc.y;
		vPos.z = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurLoc.z;

		vDir.x = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurDir.x;
		vDir.y = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurDir.y;
		vDir.z = GetNtlSLGlobal()->GetAvatarInfo()->sCharState.sCharStateBase.vCurDir.z;

		if ( RwV3dNormalize( &vDir, &vDir ) < 0.0001f )
		{
			vDir = m_pAvatar->GetDirection();
		}

		m_pAvatar->SetPosition( &vPos );
		m_pAvatar->SetDirection( &vDir );

		vAvatarPos = vPos;

		GetNtlGameCameraManager()->ResetCamera();

		// 월드 지형 teleport 한다.
		if(m_pWorldEntity)
			m_pWorldEntity->SetPortalPosition(vAvatarPos);

		// world를 한번 업데이트 한다.
		if(m_pWorldEntity)
		{
			m_pWorldEntity->SetPlayerPosition(vAvatarPos);
			m_pWorldEntity->Update(0.01f);
		}
	}

	m_pTeleportScene->SetState(CHAR_TELEPORT);

	RwV3d vWorldUpdatePos = GetUpdateWorldPosition();
	UpdateWorld(vWorldUpdatePos);
}

void CGameStage::UpdateTeleportLoadingTeleportScene(RwReal fElapsed)
{
	RwV3d vWorldUpdatePos = GetUpdateWorldPosition();
	UpdateWorld(vWorldUpdatePos);

	// world가 loading 되었는가?
	if(!m_bWorldReady)
	{
		if(!m_pWorldEntity->GetWorldReady())
			return;
		
		m_bWorldReady = TRUE;
	}

	// 게임에 처음 진입하거나, teleport일 경우에는 resource를 다 읽은 다음에 처리한다.
	if(m_bCheckResourceScheduling)
	{
		if(GetNtlResourceManager()->IsEmptyLoadScheduling())
		{
			m_bCheckResourceScheduling = FALSE;
			if(!IsAvatarCreate())
			{
				AvatarCreate();
			}

			// avatar를 재생성해야 하는가?
			if(IsAvatarReCreate())
			{
				AvatarReCreate();
			}

			// 네트웍 데이터를 받아들인다.
//			GetSceneManager()->SetThreadLoad( FALSE );
			m_pTeleportScene->SetState(CHAR_TELEPORT_LOAD_END);
			CPacketProc::GetInstance()->ActivePop(TRUE);
			CNtlSLEventGenerator::CharReady();
		}
	}
}

void CGameStage::UpdateTeleportLoadingLoadEndScene(RwReal fElapsed)
{
	RwV3d vWorldUpdatePos = GetUpdateWorldPosition();
	UpdateWorld(vWorldUpdatePos);

	if(m_pTeleportScene->IsAvatarReadySend())
	{
		if(m_bCharReadyCommunityServer)
		{
			CGamePacketGenerator *pGameNetSender = CDboGlobal::GetInstance()->GetGamePacketGenerator();  
			pGameNetSender->SendCharReadyForCommunityServerNfy();
			m_bCharReadyCommunityServer = FALSE;
		}

		API_GetSLPacketGenerator()->SendLoadingCompleteNfy();
		API_GetSLPacketGenerator()->SendCharSpawnReady();
		m_pTeleportScene->SetState(CHAR_TELEPORT_SPAWN_READY);
	}
}

void CGameStage::UpdateTeleportLoadingSpawnReadyScene(RwReal fElapsed)
{
	RwV3d vWorldUpdatePos = GetUpdateWorldPosition();
	UpdateWorld(vWorldUpdatePos);

	// avatar가 ready 되었는가?
	if(!m_bAvatarReady)
	{
		if(m_pAvatar->IsAvatarReady())
		{
			if(!m_bCheckResourceScheduling)
			{
				LoadingEndProc();
				m_pTeleportScene->SetState(CHAR_TELEPORT_FINISH);
			}
		}
	}
}

void CGameStage::UpdateTeleportLoadingLoadFinishScene(RwReal fElapsed)
{
	RwV3d vWorldUpdatePos = GetUpdateWorldPosition();
	UpdateWorld(vWorldUpdatePos);

	m_pTeleportScene->Update(fElapsed);
	if(m_pTeleportScene->IsFinish())
	{
		SetLoadObjectSeamlessScheduling(TRUE);
		SetLoadObjectSeamlessSchedulingOneTickTime(0.001f);
		NTL_DELETE(m_pTeleportScene);
		GetNtlGameCameraManager()->EnableUpdateData(FALSE);
		m_eUpdageType = EGUT_GAME_IDLE;
	}
}

void CGameStage::UpdateDebugDisplay(RwReal fElapsed)
{
	if(!IsAvatarCreate())
		return;

	// debug infomation update
	if(GetDboGlobal()->IsDebugDisplay())
	{
        RwUInt8 byServerState = 0, byClientState = 0;
		RwChar* chClientState = "";
		RwV3d vAvatarPos = m_pAvatar->GetPosition();
		RwV3d vAvatarDir = m_pAvatar->GetDirection();
		RwBool bFightingMode = m_pAvatar->IsFightingMode();
		RwBool bAirMode = m_pAvatar->IsAirMode();
		RwReal fAvatarMoveSpeed = Logic_GetFrontRunSpeed(reinterpret_cast<CNtlSobActor*>( m_pAvatar ));
		RwReal fAvatarAttackSpeed = Logic_GetAttackAnimSpeed(reinterpret_cast<CNtlSobActor*>( m_pAvatar ));

		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAvatarPos, "Avatar Pos : (%3.3f, %3.3f, %3.3f)", vAvatarPos.x, vAvatarPos.y, vAvatarPos.z);  
		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAvatarDir, "Avatar Dir : (%3.3f, %3.3f, %3.3f)", vAvatarDir.x, vAvatarDir.y, vAvatarDir.z);  
		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAvatarFightingMode, "Avatar Fighting Mode : (%d)", bFightingMode);
		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAvatarAirMode, "Avatar Air Mode : (%d)", bAirMode);
		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAvatarMoveSpeed, "Avatar Move Speed : (%f)", fAvatarMoveSpeed);
		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAvatarAttackSpeed, "Avatar Attck Speed : (%f)", fAvatarAttackSpeed);
        CNtlSLEventGenerator::SobGetState(m_pAvatar->GetSerialID(), byServerState, byClientState, chClientState);

        const char *pStateString = NtlGetCharStateString(byServerState);
        if(pStateString)
        {
            CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAvatarState, "Avatar Server State : (%d, %s), Avatar Client State : (%d, %s)", byServerState, pStateString, byClientState, chClientState);
        }
        else
        {
            CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAvatarState, "Avatar Server State : (%d, NONE)", byServerState);  
        }

		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetFieldIdx, "Field Idx : (%d)", GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetFieldIdx(vAvatarPos));  
		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetSectorIdx, "Sector Idx : (%d)", GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vAvatarPos));  
		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetPlayerCount, "Player Count : (%u)", GetNtlSobManager()->GetSobObjectCount(SLCLASS_PLAYER));  
		CNtlCharsetPrint::GetInstance()->Format(m_hCharsetMonsterCount, "Monster Count : (%u)", GetNtlSobManager()->GetSobObjectCount(SLCLASS_MONSTER));  

		SERIAL_HANDLE hTargetSerial = Logic_GetAvatarTargetHandle();
		if(hTargetSerial != INVALID_SERIAL_ID)
		{
			CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hTargetSerial);
			if(pSobObj)
			{
				RwChar* chClientState = "";
				RwV3d vTarPos = pSobObj->GetPosition();
				RwV3d vTarDir = pSobObj->GetDirection();
				RwReal fTargetMoveSpeed = Logic_GetFrontRunSpeed(reinterpret_cast<CNtlSobActor*>( pSobObj ));
				RwReal fTargetAttackSpeed = Logic_GetAttackAnimSpeed(reinterpret_cast<CNtlSobActor*>( pSobObj ));
				
				RwV3d vAvatar2DPos = m_pAvatar->GetPosition();
				RwV3d vTar2DPos = pSobObj->GetPosition();
				CNtlMath::MathRwV3dAssign( &vAvatar2DPos, vAvatar2DPos.x, 0.0f, vAvatar2DPos.z );
				CNtlMath::MathRwV3dAssign( &vTar2DPos, vTar2DPos.x, 0.0f, vTar2DPos.z );

				RwV3d v3DDist = vAvatarPos - vTarPos;
				RwV3d v2DDist = vAvatar2DPos - vTar2DPos;

				CNtlCharsetPrint::GetInstance()->Format(m_hCharsetTargetLoc, 
					"Target Pos : (%3.3f, %3.3f, %3.3f), Target Range : (%3.1fm, 2D), (%3.1fm, 3D)",
					vTarPos.x, vTarPos.y, vTarPos.z, RwV3dLength(&v2DDist), RwV3dLength(&v3DDist));
				CNtlCharsetPrint::GetInstance()->Format(m_hCharsetTargetDir, "Target Dir : (%3.3f, %3.3f, %3.3f)", vTarDir.x, vTarDir.y, vTarDir.z);  
				CNtlCharsetPrint::GetInstance()->Format(m_hCharsetTargetMoveSpeed, "Target Move Speed : %f", fTargetMoveSpeed);
				CNtlCharsetPrint::GetInstance()->Format(m_hCharsetTargetAttackSpeed, "Target Attack Speed : %f", fTargetAttackSpeed);
				
				CNtlSLEventGenerator::SobGetState(pSobObj->GetSerialID(), byServerState, byClientState, chClientState);
				const char *pStateString = NtlGetCharStateString(byServerState);
				if(pStateString)
				{
					CNtlCharsetPrint::GetInstance()->Format(m_hCharsetTargetState, "Target Server State : (%d, %s), Target Client State : (%d, %s)", byServerState, pStateString, byClientState, chClientState);
				}
				else
				{
					CNtlCharsetPrint::GetInstance()->Format(m_hCharsetTargetState, "Target Server State : (%d, NONE)", byServerState);  
				}

				RwBool bFightingMode = FALSE;
				CNtlSLEventGenerator::SobGetFightingMode(pSobObj->GetSerialID(), bFightingMode);
				CNtlCharsetPrint::GetInstance()->Format(m_hCharsetFightingMode, "Target FightingMode : (%d)", bFightingMode);  

				RwBool bAirMode = FALSE;
				CNtlSLEventGenerator::SobGetAirMode(pSobObj->GetSerialID(), bAirMode);
				CNtlCharsetPrint::GetInstance()->Format(m_hCharsetAirMode, "Target Air Mode : (%d)", bAirMode);
			}
		}
	}
}

void CGameStage::UpdateGameEnterLoading(RwReal fElapsed)
{
	UpdateLoadingScene(fElapsed);

	if(UpdateLoadingThread(fElapsed))
	{
		return;
	}

	UpdateGameEnterLoadingSchedulingWorld(fElapsed);
}

void CGameStage::UpdateGameIdle(RwReal fElapsed)
{
	RwV3d vWorldUpdatePos = GetUpdateWorldPosition();
	UpdateWorld(vWorldUpdatePos);
}

void CGameStage::UpdateGameTeleportLoading(RwReal fElapsed)
{
	if(m_pTeleportScene == NULL)
		return;

	m_pTeleportScene->Update(fElapsed);

	ECharTeleportState	eState = m_pTeleportScene->GetState();

	if(eState == CHAR_TELEPORT_NONE)
	{
		UpdateTeleportLoadingNoneScene(fElapsed);
	}
	else if(eState == CHAR_TELEPORT_READY)
	{
		UpdateTeleportLoadingReadyScene(fElapsed);
	}
	else if(eState == CHAR_TELEPORT)
	{
		UpdateTeleportLoadingTeleportScene(fElapsed);
	}
	else if(eState == CHAR_TELEPORT_LOAD_END)
	{
		UpdateTeleportLoadingLoadEndScene(fElapsed);
	}
	else if(eState == CHAR_TELEPORT_SPAWN_READY)
	{
		UpdateTeleportLoadingSpawnReadyScene(fElapsed);
	}
	else
	{
		UpdateTeleportLoadingLoadFinishScene(fElapsed);
	}
}

void CGameStage::EventProcUpdateTick(RwReal fElapsed)
{
    m_pStageState->Update(fElapsed);

	if(m_eUpdageType == EGUT_GAME_ENTER_LOADING)
	{        
		UpdateGameEnterLoading(fElapsed);        
	}
	else if(m_eUpdageType == EGUT_GAME_TELEPORT_LOADING)
	{
		UpdateGameTeleportLoading(fElapsed);        
	}
	else
	{
		UpdateGameIdle(fElapsed);
	}

    // 상황에 따른 사운드 리스너를 설정한다.
    UpdateSoundListener();
	
	// avatar가 생성되어 있지 않으면?
	if(!IsAvatarCreate())
		return;

	// avatar controller update
	m_AvatarController.Update(fElapsed);

	// world concept update
	GetNtlWorldConcept()->Update(fElapsed);

	// Dialog Update
	GetDialogManager()->Update(fElapsed);

	UpdateDebugDisplay(fElapsed);
}

void CGameStage::EventProcWorldChange(RwBool bWorldChange)
{
	// loading을 시작한다.
	
	m_pTeleportScene = NTL_NEW CGameTeleportScene(bWorldChange);

	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo(); 

	RwV3d vPos, vDir;
	CNtlMath::MathRwV3dAssign(&vPos, 
							pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x,
							pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y,
							pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z);
	CNtlMath::MathRwV3dAssign(&vDir, 
							pAvatarInfo->sCharState.sCharStateBase.vCurDir.x,
							pAvatarInfo->sCharState.sCharStateBase.vCurDir.y,
							pAvatarInfo->sCharState.sCharStateBase.vCurDir.z);

	m_bWorldReady	= FALSE;
	m_bAvatarReady	= FALSE;
	m_bWorldChange	= bWorldChange;
	m_bCheckResourceScheduling = TRUE;
	m_eUpdageType = EGUT_GAME_TELEPORT_LOADING;

	//---------------------------------------------------
	// teleport 할 world 가 다를 경우.
	// avatar resource를 월드에서 뺀다.
	if(m_bWorldChange)
	{
	}
	
	//---------------------------------------------------
	SetLoadObjectSeamlessScheduling(FALSE);
	GetNtlResourceManager()->SetDeleteScheduling(TRUE);
//	GetSceneManager()->SetThreadLoad( TRUE );
	GetSceneManager()->SetWorldPVSActivation(FALSE);
	CPacketProc::GetInstance()->ActivePop(FALSE);
}

void CGameStage::PreMutiThreadLoading( void )
{
	m_pStageState = NTL_NEW CGameStageState;

	GetNtlWorldConcept()->EnableAction(TRUE);
	GetSceneManager()->SetThreadLoad( TRUE );
	GetNtlSobManager()->SetActive( FALSE );

	// Create TextureCamera
	CMapItem::CreateCamera();
	
	// event link
	LinkMsg(g_EventWorldChange, 0);
	LinkMsg(g_EventUpdateTick, 0, SLUPDATE_PRIORITY_STAGE);
	LinkMsg(g_EventDebug_for_Developer);
	LinkMsg(g_EventEndterWorld);
	LinkMsg(g_EventLoading);

	// because of thread loading
	RwFrameListSetAutoUpdate(FALSE);

	// Start loading thread
	m_pLoadingScene = NTL_NEW CGameLoadingScene;

	if( GetDboGlobal()->IsDirectionFirstConnect() )
	{
		m_pLoadingScene->SetFirstEnterMode( TRUE );
	}

	m_pLoadingThread = NTL_NEW CGameLoadingThread( this );

	m_pStageState->SetNPCheck( FALSE );

	GetNtlResourceManager()->SetLoadScheduling(TRUE);
	GetNtlResourceManager()->SetDeleteScheduling(TRUE);
	SetLoadObjectSeamlessScheduling(FALSE);

	// gui link
	LinkGuiInputBoxFocus(CallbackGuiInputBoxFocus);
}

void CGameStage::MutiThreadLoading( void )
{
	// gui create
	// Always be created before avatar creation.
	// The event is called when the avatar is created.

	m_pGuiGroup = NTL_NEW CGameGuiGroup;
	if(!m_pGuiGroup->Create())
	{
		NTL_DELETE(m_pGuiGroup);
	}

	// height field world를 생성한다.
	// avatar 좌표 얻어오기.
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	RwV3d vAvatarPos;
	CNtlMath::MathRwV3dAssign(&vAvatarPos,	
								pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x,
								pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y,
								pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z);

	CreateWorld(vAvatarPos);

	// trigger system setting

	m_pTSCMain = NTL_NEW CDboTSCMain;
	m_pTSCMain->Create();

	// simulation layer resource initial
	API_SLResourceInit();

	// charset print
	m_hCharsetAvatarPos = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetAvatarDir = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetAvatarFightingMode = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetAvatarAirMode = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetAvatarMoveSpeed = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetAvatarAttackSpeed = CNtlCharsetPrint::GetInstance()->Acquire();
    m_hCharsetAvatarState = CNtlCharsetPrint::GetInstance()->Acquire();

	m_hCharsetFieldIdx = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetSectorIdx = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetAvatarAngle = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetPlayerCount = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetMonsterCount = CNtlCharsetPrint::GetInstance()->Acquire();

	m_hCharsetTargetLoc = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetTargetDir = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetTargetMoveSpeed = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetTargetAttackSpeed = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetTargetState = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetFightingMode = CNtlCharsetPrint::GetInstance()->Acquire();
	m_hCharsetAirMode = CNtlCharsetPrint::GetInstance()->Acquire();
}

void CGameStage::PostMutiThreadLoading( void )
{
	// 로딩 Thread를 종료한다
	if ( m_pLoadingThread )
	{
		NTL_DELETE(m_pLoadingThread);

		m_pStageState->SetNPCheck( TRUE );
	}

	// because of thread loading
	// render game graphics data at the time of loading
	// to recover
	RwFrameListSetAutoUpdate(TRUE);
	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	GetNtlSobManager()->SetActive( TRUE );
	
	// Effect 복구
	if(GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_SHADER_HDR ))
		CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
	else
		CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);

	
	// Update world once.
	m_pWorldEntity->Update(0.01f);

	// event link
	LinkMsg(g_EventGameServerConnect, 0);
}

void CGameStage::PostRender(void)
{
	BeginGuiRenderState();

	if(m_pLoadingScene)
		m_pLoadingScene->Render();

	if(m_pTeleportScene)
		m_pTeleportScene->Render();

	EndGuiRenderState();
}

// woody1019
void CGameStage::CreateWorld(RwV3d& vAvatarPos)
{
	// set texture folder path
	_getcwd(dGET_WORLD_PARAM()->CurWorkingFolderName, 256);
	strcpy(dGET_WORLD_PARAM()->CurDefaultTexPath, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->CurDefaultTexPath, "\\texture\\ntlwe\\");

	// set mesh folder path
	std::string str = "\\world\\";
	const RwChar *pWorldName = Logic_GetActiveWorldName();
	str += pWorldName;

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
	m_pWorldEntity->CreateWorld(vAvatarPos);
}

void CGameStage::UpdateWorld(RwV3d& vAvatarPos)
{
	if(m_pWorldEntity == NULL)
		return;

	m_pWorldEntity->SetPlayerPosition(vAvatarPos);
}

void CGameStage::DeleteWorld()
{
	// delete world entity
	if(m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}
}

void CGameStage::AvatarCreate(void)
{
	// avatar create
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	m_pAvatar = reinterpret_cast<CNtlSobAvatar*>(CNtlSLEventGenerator::SobAvatarCreate(SLCLASS_AVATAR, pAvatarInfo->uiSerialId, &pAvatarInfo->sCharState, TRUE));	
	CDboEventGenerator::QuickSlotInfo();	

	// camera 
	GetNtlGameCameraManager()->SetActiveActor(m_pAvatar);

	// avatar controller create
	m_AvatarController.Create();

	// link keyboard down 
	m_hKeyboardDown = CInputHandler::GetInstance()->LinkKeyDown(this, &CGameStage::KeyboardDownHandler);  			
}

void CGameStage::AvatarReCreate(void)
{
	if(m_pAvatar)
	{
		Logic_SobTarget(INVALID_SERIAL_ID, INVALID_BYTE);
		CNtlSLEventGenerator::GameServerChangeOut();
		CNtlSLEventGenerator::SobDelete(m_pAvatar->GetSerialID());
		GetNtlSLGlobal()->SetSobAvatar(NULL);
		m_pAvatar = NULL;
	}

	// avatar create
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	m_pAvatar = reinterpret_cast<CNtlSobAvatar*>(CNtlSLEventGenerator::SobAvatarCreate(SLCLASS_AVATAR, pAvatarInfo->uiSerialId, &pAvatarInfo->sCharState, TRUE));	
	CDboEventGenerator::QuickSlotInfo();	

	// camera 
	GetNtlGameCameraManager()->SetActiveActor(m_pAvatar);

	SGameData *pGameData = GetDboGlobal()->GetGameData();
	pGameData->bAvatarReCreate = FALSE;

	m_bCharReadyCommunityServer = TRUE;
}

RwBool CGameStage::IsAvatarCreate(void)
{
	if(m_pAvatar)
		return TRUE;

	return FALSE;
}

RwBool CGameStage::IsAvatarReCreate(void)
{
	SGameData *pGameData = GetDboGlobal()->GetGameData();
	return pGameData->bAvatarReCreate;
}

RwBool CGameStage::IsActiveLoadingScene()
{
	if( m_pLoadingThread )
		return TRUE;

	return FALSE;
}

int CGameStage::KeyboardDownHandler(unsigned int uiKeyData)
{
	NTL_FUNCTION("CGameStage::KeyboardDownHandler");

 	SKeyData *pData = (SKeyData *)uiKeyData;

	if( pData->uiChar == VK_RETURN &&
		pData->uiRepCount == 1 &&
		GetDboGlobal()->GetGameData()->bChatMode == FALSE )
		CDboEventGenerator::ChatGuiMode(TRUE);

	NTL_RETURN(1);
}

void CGameStage::ResetGui()
{
	if(m_pGuiGroup)
	{
		m_pGuiGroup->Destroy(); 
		NTL_DELETE(m_pGuiGroup);
	}

	if( !m_pGuiGroup )
	{
		m_pGuiGroup = NTL_NEW CGameGuiGroup;
		if(!m_pGuiGroup->Create())
		{
			NTL_DELETE(m_pGuiGroup);
		}
		else
			GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(),
			GetDboGlobal()->GetScreenHeight());
	}
}

void CGameStage::UpdateSoundListener() 
{
    if(IsAvatarCreate())
    {
        if(m_eUpdageType == EGUT_GAME_ENTER_LOADING ||
           m_eUpdageType == EGUT_GAME_TELEPORT_LOADING)
        {        
            GetSceneManager()->SetSoundListener(NULL);
            return;
        }

        // 시네마틱중이면 카메라로 세팅한다.
        if(GetNtlDTCinematicManager()->IsRunning())
        {
            GetSceneManager()->SetSoundListener(NULL);
            return;
        }

        GetSceneManager()->SetSoundListener(m_pAvatar->GetSobProxy()->GetPLMainEntity());
    }
    else
    {
        GetSceneManager()->SetSoundListener(NULL);
    }    
}
