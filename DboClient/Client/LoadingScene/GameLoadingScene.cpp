#include "precomp_dboclient.h"
#include "GameLoadingScene.h"

// shared
#include "TableContainer.h"
#include "TimeQuestTable.h"
#include "NtlSLLuaGlue.h"
#include "DirectionLinkTable.h"
#include "WorldTable.h"
#include "DungeonTable.h"

// framework
#include "NtlTimer.h"

// sound
#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLVisualManager.h"
#include "NtlPLEntityBlend.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSobManager.h"
#include "NtlSLGlobal.h"
#include "NtlWorldConceptTMQ.h"
#include "NtlWorldConceptTutorial.h"
#include "InputHandler.h"
#include "NtlLuaState.h"
#include "NtlDTCinematicManager.h"

// dbo
#include "SurfaceGui.h"
#include "DboDef.h"
#include "DboGlobal.h"
#include "LoadingGui.h"
#include "DialogPriority.h"


CGameNormalTeleportScene::CGameNormalTeleportScene(RwBool bLoadResource /*= TRUE*/)
{
	m_eState			= CHAR_TELEPORT_NONE;
	m_fTime				= 0.0f;
	m_bFinish			= FALSE;

	if ( bLoadResource )
	{
		m_pLoadingGui = NTL_NEW CLoadingGui("LoadingGui");
		if( !m_pLoadingGui->Create() )
		{
			m_pLoadingGui->Destroy();
			NTL_DELETE(m_pLoadingGui);
		}

		m_pLoadingGui->Load_LoadingPage(LOADING_PAGE_FIRST_STEP);


		m_pAlphaBlend = NTL_NEW CNtlPLEntityAlphaWeightBlend;
		m_pAlphaBlend->SetWeight(1.0f, 0.0f, 0.15f, 0.15f, FALSE);
	}
	else
	{
		m_pLoadingGui = NULL;
		m_pAlphaBlend = NULL;
	}
}

CGameNormalTeleportScene::~CGameNormalTeleportScene()
{
	if(m_pAlphaBlend)
	{
		NTL_DELETE(m_pAlphaBlend);
	}

	if(m_pLoadingGui)
	{
		m_pLoadingGui->Destroy();
		NTL_DELETE(m_pLoadingGui);
	}
}


void CGameNormalTeleportScene::Update(RwReal fElapsed)
{
	if(m_bFinish)
		return;

	if(m_pLoadingGui)
		m_pLoadingGui->Update(fElapsed);

	m_fTime += fElapsed;

	if(m_eState == CHAR_TELEPORT_FINISH)
	{
		if(m_pAlphaBlend)
		{
			m_pAlphaBlend->Update(fElapsed);
			RwUInt8 byAlpha = (RwUInt8)( 255.0f * m_pAlphaBlend->GetWeight() );

			if ( m_pLoadingGui )
			{
				m_pLoadingGui->SetAlpha(byAlpha);
			}
						
			if(m_pAlphaBlend->IsFinish())
			{
				NTL_DELETE(m_pAlphaBlend);
				m_bFinish = TRUE;

				return;
			}
		}
	}
}

void CGameNormalTeleportScene::Render(void) 
{
	if(m_pLoadingGui)
		m_pLoadingGui->Render();
}

void CGameNormalTeleportScene::SetState(ECharTeleportState eState)
{
	CGameTeleportSceneNode::SetState(eState);

	if(eState == CHAR_TELEPORT_FINISH)
	{
		m_fTime	= 0.0f;
	}
}

RwBool CGameNormalTeleportScene::IsAvatarReadySend(void)
{
	if(m_eState >= CHAR_TELEPORT_LOAD_END)
		return TRUE;

	return FALSE;
}

RwBool CGameNormalTeleportScene::IsFinish(void) 
{
	return m_bFinish;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CGameTLQTeleportScene::CGameTLQTeleportScene(RwBool bLoadResource /*= TRUE*/)
: CGameNormalTeleportScene( bLoadResource )
{
	m_eSubState = ETLQTS_MAIN_RENDER;
	m_byCounter = 0;
}

CGameTLQTeleportScene::~CGameTLQTeleportScene()
{
}

void CGameTLQTeleportScene::Update(RwReal fElapsed)
{
	if ( !m_bFinish )
	{
		if ( m_pLoadingGui )
		{
			m_pLoadingGui->Update( fElapsed );
		}

		m_fTime += fElapsed;

		if ( m_eState == CHAR_TELEPORT_LOAD_END )
		{
			m_byCounter++;

			if ( m_pAlphaBlend && m_byCounter > 20 )
			{
				m_pAlphaBlend->Update( fElapsed );

				RwUInt8 byAlpha = (RwUInt8)( 255.0f * m_pAlphaBlend->GetWeight() );

				if ( m_pLoadingGui )
				{
					m_pLoadingGui->SetAlpha( byAlpha );
				}

				if ( m_pAlphaBlend->IsFinish() )
				{
					NTL_DELETE( m_pAlphaBlend );
					m_bFinish = TRUE;
				}
			}
		}
	}
	else
	{
		UpdateSubState( fElapsed );
	}
}

void CGameTLQTeleportScene::Render(void)
{
	if ( !m_bFinish )
	{
		CGameNormalTeleportScene::Render();
	}
}

RwBool CGameTLQTeleportScene::IsAvatarReadySend(void)
{
	return m_bFinish;
}

RwBool CGameTLQTeleportScene::IsFinish(void)
{
	return (m_eSubState == ETLQTS_CINEMATIC_FINISH) ? TRUE : FALSE;
}

void CGameTLQTeleportScene::SetState(ECharTeleportState eState)
{
	CGameNormalTeleportScene::SetState( eState );

	if (CHAR_TELEPORT_FINISH == eState )
	{
		SetSubState( ETLQTS_CINEMATIC_RENDER );
	}
}

void CGameTLQTeleportScene::SetSubState(ETLQTeleportScene eSubState)
{
	m_eSubState = eSubState;

	if ( ETLQTS_CINEMATIC_RENDER == m_eSubState )
	{
		if ( !LoadCinematic() )
		{
			SetSubState( ETLQTS_CINEMATIC_FINISH );
		}
	}
}

bool CGameTLQTeleportScene::LoadCinematic(void)
{
	sWORLD_INFO* pWorldInfo = Logic_GetActiveWorldInfo();

	if ( GAMERULE_TLQ != pWorldInfo->sRuleInfo.byRuleType )
	{
		return false;
	}

	sWORLD_TBLDAT* pWorldTblData = (sWORLD_TBLDAT*)API_GetTableContainer()->GetWorldTable()->FindData( pWorldInfo->tblidx );

	if ( NULL == pWorldTblData )
	{
		return false;
	}

	if ( pWorldTblData->byWorldRuleType != GAMERULE_TLQ )
	{
		return false;
	}

	sDUNGEON_TBLDAT* pDungeonData = (sDUNGEON_TBLDAT*)API_GetTableContainer()->GetDungeonTable()->FindData( pWorldTblData->worldRuleTbldx );

	if ( NULL == pDungeonData )
	{
		return false;
	}

	if ( INVALID_TBLIDX == pDungeonData->openCine )
	{
		return false;
	}

	CDirectionLinkTable* pLinkTbl = API_GetTableContainer()->GetDirectionLinkTable();

	if ( NULL == pLinkTbl )
	{
		return false;
	}

	sDIRECTION_LINK_TBLDAT* pLinkTblData = reinterpret_cast<sDIRECTION_LINK_TBLDAT*>( pLinkTbl->FindData( pDungeonData->openCine ) );

	if ( NULL == pLinkTblData )
	{
		return false;
	}

	std::string strFunc = pLinkTblData->szFunctionName;
	strFunc += "()";

	CNtlLuaState *pLuaState = GetLuaState();
	if ( !pLuaState->RunString( strFunc.c_str() ) )
	{
		return false;
	}

	return true;
}

void CGameTLQTeleportScene::UpdateSubState(RwReal fElapsed)
{
	switch ( m_eSubState )
	{
		case ETLQTS_MAIN_RENDER:
		{
			SetSubState( ETLQTS_CINEMATIC_RENDER );
		}
		break;

		case ETLQTS_CINEMATIC_RENDER:
		{
			if ( !CNtlDTCinematicManager::GetInstance()->IsRunning() )
			{
				SetSubState( ETLQTS_CINEMATIC_FINISH );
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CGameTMQTeleportScene::CGameTMQTeleportScene()
{
	m_eState				= CHAR_TELEPORT_NONE;
	m_byResCount			= 0;
	m_byResState			= ETMQTS_MAIN_RENDER;
	m_bSceneMovieEnd		= FALSE;
	m_byCinematicNextState	= ETMQTS_CINEMATIC_FINISH;
	m_bFinish				= FALSE;

	m_pLoadingGui			= NULL;

	m_pLoadingGui = NTL_NEW CLoadingGui("LoadingGui");
	if( !m_pLoadingGui->Create() )
	{
		m_pLoadingGui->Destroy();
		NTL_DELETE(m_pLoadingGui);
	}

	m_pLoadingGui->Load_LoadingPage(LOADING_PAGE_FIRST_STEP);

	for( RwUInt8 i = 0 ; i < NUM_CHANNEL_GROUP ; ++i )
	{
		if( CHANNEL_GROUP_BGM == i )
			continue;

		m_fSavedChannelGroupVolume[i] = GetSoundManager()->GetGroupVolume(i);
		GetSoundManager()->SetGroupVolume(i, 0.f);
	}
}

CGameTMQTeleportScene::~CGameTMQTeleportScene()
{
	for( RwUInt8 i = 0 ; i < NUM_CHANNEL_GROUP ; ++i )
	{
		if( CHANNEL_GROUP_BGM == i )
			continue;
		
		GetSoundManager()->SetGroupVolume(i, m_fSavedChannelGroupVolume[i]);
	}

	if( m_pLoadingGui )
	{
		m_pLoadingGui->Destroy();
		NTL_DELETE(m_pLoadingGui);
	}
}

bool CGameTMQTeleportScene::LoadCinematic(void)
{
	sWORLD_INFO* pWorldInfo = Logic_GetActiveWorldInfo();
	if ( GAMERULE_TIMEQUEST != pWorldInfo->sRuleInfo.byRuleType )
	{
		return false;
	}

	sTIMEQUEST_TBLDAT* pTMQTblData = (sTIMEQUEST_TBLDAT*)API_GetTableContainer()->GetTimeQuestTable()->FindData( pWorldInfo->sRuleInfo.sTimeQuestRuleInfo.timeQuestTblidx );

	if ( NULL == pTMQTblData )
	{
		return false;
	}

	if ( INVALID_TBLIDX == pTMQTblData->openCine )
	{
		return false;
	}

	CDirectionLinkTable* pLinkTbl = API_GetTableContainer()->GetDirectionLinkTable();

	if ( NULL == pLinkTbl )
	{
		return false;
	}

	sDIRECTION_LINK_TBLDAT *pLinkTblData = reinterpret_cast<sDIRECTION_LINK_TBLDAT*>( pLinkTbl->FindData( pTMQTblData->openCine ) );

	if ( NULL == pLinkTblData )
	{
		return false;
	}

	std::string strFunc = pLinkTblData->szFunctionName;
	strFunc += "()";

	CNtlLuaState *pLuaState = GetLuaState();
	if ( !pLuaState->RunString( strFunc.c_str() ) )
	{
		return false;
	}

	return true;
}

void CGameTMQTeleportScene::Update(RwReal fElapsed)
{
	if(m_bFinish)
		return;

	if(m_eState == CHAR_TELEPORT_FINISH)
	{
		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);

		if ( pWorldConcept->GetConceptType() == WORLD_PLAY_TIME_MACHINE )
		{
			((CNtlWorldConceptTMQ*)pWorldConcept)->SetTMQState_from_Cleint(WORLD_STATE_TMQ_TERRAIN_READY);
		}
		else if ( pWorldConcept->GetConceptType() == WORLD_PLAY_TUTORIAL )
		{
			((CNtlWorldConceptTutorial*)pWorldConcept)->SetTMQState_from_Cleint(WORLD_STATE_TMQ_TERRAIN_READY);
		}
		else
		{
			NTL_ASSERTFAIL( "CGameTMQTeleportScene::Update, Wrong world concept : " << pWorldConcept->GetConceptType() );
		}

		m_bFinish = TRUE;

		return;
	}

	if( m_pLoadingGui )
		m_pLoadingGui->Update(fElapsed);

	if ( m_byResState == ETMQTS_MAIN_RENDER )
	{
	}
	else if ( m_byResState == ETMQTS_AFTER_RENDER )
	{
	}
	else if ( m_byResState == ETMQTS_CINEMATIC_ENTER )
	{
		m_byResCount++;

		if ( m_byResCount > 20 )
		{
			m_byResCount = 0;
			m_byResState = ETMQTS_CINEMATIC_RUN;

			if( m_pLoadingGui )
			{
				m_pLoadingGui->Destroy();
				NTL_DELETE(m_pLoadingGui);
			}
		}
	}
	else if ( m_byResState == ETMQTS_CINEMATIC_RUN )
	{
		if ( !CNtlDTCinematicManager::GetInstance()->IsRunning() )
		{
			m_byResState = ETMQTS_CINEMATIC_FINISH;
		}
	}
	else if ( m_byResState == ETMQTS_CINEMATIC_FINISH )
	{
		m_bSceneMovieEnd = TRUE;
	}
}

void CGameTMQTeleportScene::Render(void)
{
	if( m_pLoadingGui )
		m_pLoadingGui->Render();
}

void CGameTMQTeleportScene::SetState(ECharTeleportState eState)
{
	CGameTeleportSceneNode::SetState(eState);

	if ( eState == CHAR_TELEPORT_LOAD_END )
	{
		if( m_pLoadingGui )
			m_pLoadingGui->Load_LoadingPage(LOADING_PAGE_SECOND_STEP);

		m_byResState = ETMQTS_AFTER_RENDER;
	}
}

RwBool CGameTMQTeleportScene::IsAvatarReadySend(void)
{
	return m_bSceneMovieEnd;
}

RwBool CGameTMQTeleportScene::IsFinish(void)
{
	return m_bFinish;
}

void CGameTMQTeleportScene::OnLoadingEvent(RwUInt8 byMessage)
{
	if( LOADING_EVENT_SECOND_MOVIE_END == byMessage )
	{
		m_byResState = ETMQTS_CINEMATIC_ENTER;
		m_byCinematicNextState = LoadCinematic() ? (RwUInt8)ETMQTS_CINEMATIC_RUN : (RwUInt8)ETMQTS_CINEMATIC_FINISH;
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CGameCCBDTeleportScene::CGameCCBDTeleportScene(RwBool bLoadResource/* = TRUE*/)
	: CGameNormalTeleportScene(bLoadResource)
{
	m_eSubState = ECCBDTS_MAIN_RENDER;
	m_byCounter = 0;
}

CGameCCBDTeleportScene::~CGameCCBDTeleportScene()
{
}

void CGameCCBDTeleportScene::Update(RwReal fElapsed)
{
	if (!m_bFinish)
	{
		if (m_pLoadingGui)
		{
			m_pLoadingGui->Update(fElapsed);
		}

		m_fTime += fElapsed;

		if (m_eState == CHAR_TELEPORT_LOAD_END)
		{
			m_byCounter++;

			if (m_pAlphaBlend && m_byCounter > 20)
			{
				m_pAlphaBlend->Update(fElapsed);

				RwUInt8 byAlpha = (RwUInt8)(255.0f * m_pAlphaBlend->GetWeight());

				if (m_pLoadingGui)
				{
					m_pLoadingGui->SetAlpha(byAlpha);
				}

				if (m_pAlphaBlend->IsFinish())
				{
					NTL_DELETE(m_pAlphaBlend);
					m_bFinish = TRUE;
				}
			}
		}
	}
	else
	{
		UpdateSubState(fElapsed);
	}
}

void CGameCCBDTeleportScene::Render()
{
	if (!m_bFinish)
	{
		CGameNormalTeleportScene::Render();
	}
}

RwBool CGameCCBDTeleportScene::IsAvatarReadySend()
{
	return m_bFinish;
}

RwBool CGameCCBDTeleportScene::IsFinish()
{
	return (m_eSubState == ECCBDTS_CINEMATIC_FINISH) ? TRUE : FALSE;
}

void CGameCCBDTeleportScene::SetState(ECharTeleportState eState)
{
	CGameNormalTeleportScene::SetState(eState);

	if (eState == CHAR_TELEPORT_LOAD_END)
	{
		SetSubState(ECCBDTS_CINEMATIC_RENDER);
	}
}

void CGameCCBDTeleportScene::SetSubState(ECCBDTeleportScene eSubState)
{
	m_eSubState = eSubState;

	if (m_eSubState == ECCBDTS_CINEMATIC_RENDER)
	{
		if (!LoadCinematic())
		{
			SetSubState(ECCBDTS_CINEMATIC_FINISH);
		}
	}
}

bool CGameCCBDTeleportScene::LoadCinematic()
{
	sWORLD_INFO* pWorldInfo = Logic_GetActiveWorldInfo();
	if (GAMERULE_CCBATTLEDUNGEON != pWorldInfo->sRuleInfo.byRuleType)
	{
		return false;
	}

	CDirectionLinkTable* pLinkTbl = API_GetTableContainer()->GetDirectionLinkTable();

	if (NULL == pLinkTbl)
	{
		return false;
	}

	sDIRECTION_LINK_TBLDAT *pLinkTblData = reinterpret_cast<sDIRECTION_LINK_TBLDAT*>(pLinkTbl->FindData(1074));

	if (NULL == pLinkTblData)
	{
		return false;
	}

	std::string strFunc = pLinkTblData->szFunctionName;
	strFunc += "()";

	CNtlLuaState *pLuaState = GetLuaState();
	if (!pLuaState->RunString(strFunc.c_str()))
	{
		return false;
	}

	return true;
}

void CGameCCBDTeleportScene::UpdateSubState(RwReal fElapsed)
{
	switch (m_eSubState)
	{
		case ECCBDTS_MAIN_RENDER:
		{
			SetSubState(ECCBDTS_CINEMATIC_RENDER);
		}
		break;

		case ECCBDTS_CINEMATIC_RENDER:
		{
			if (!CNtlDTCinematicManager::GetInstance()->IsRunning())
			{
				SetSubState(ECCBDTS_CINEMATIC_FINISH);
			}
		}
		break;
	}
}



//////////////////////////////////////////////////////////////////////////
//
//	CGameTeleportScene
//
//////////////////////////////////////////////////////////////////////////


CGameTeleportScene::CGameTeleportScene()
{
	GetSoundManager()->SetProhibition( CHANNEL_GROUP_UI_SOUND, TRUE );
}

CGameTeleportScene::CGameTeleportScene(RwBool bWorldChange)
{
	GetSoundManager()->SetProhibition( CHANNEL_GROUP_UI_SOUND, TRUE );

	m_pSceneNode		= NULL;
	m_byWorldRuleType	= Logic_GetActiveWorldRuleType();

	if ( bWorldChange )
	{
		if ( m_byWorldRuleType == GAMERULE_TIMEQUEST)
		{
			m_pSceneNode = NTL_NEW CGameTMQTeleportScene;
		}
		else if ( m_byWorldRuleType == GAMERULE_TLQ)
		{
			m_pSceneNode = NTL_NEW CGameTLQTeleportScene;
		}
		else if (m_byWorldRuleType == GAMERULE_CCBATTLEDUNGEON)
		{
			m_pSceneNode = NTL_NEW CGameCCBDTeleportScene;
		}
		else
		{
			m_pSceneNode = NTL_NEW CGameNormalTeleportScene;
		}
	}
	else
	{
		m_pSceneNode = NTL_NEW CGameNormalTeleportScene;
	}
}

CGameTeleportScene::~CGameTeleportScene()
{
	GetSoundManager()->SetProhibition( CHANNEL_GROUP_UI_SOUND, FALSE );

	if(m_pSceneNode)
	{
		NTL_DELETE(m_pSceneNode);
	}
}


void CGameTeleportScene::Update(RwReal fElapsed)
{
	if(m_pSceneNode)
		m_pSceneNode->Update(fElapsed);
}

void CGameTeleportScene::Render(void)
{
	if(m_pSceneNode)
		m_pSceneNode->Render();
}

RwBool CGameTeleportScene::IsAvatarReadySend(void)
{
	if(m_pSceneNode)
		return m_pSceneNode->IsAvatarReadySend();

	return TRUE;
}

RwBool CGameTeleportScene::IsFinish(void)
{
	if(m_pSceneNode)
		return m_pSceneNode->IsFinish();

	return TRUE;
}

void CGameTeleportScene::SetState(ECharTeleportState eState)
{
	if(m_pSceneNode)
		m_pSceneNode->SetState(eState);
}

ECharTeleportState CGameTeleportScene::GetState(void)
{
	if(m_pSceneNode)
		return m_pSceneNode->GetState();

	return CHAR_TELEPORT_NONE;
}

void CGameTeleportScene::OnLoadingEvent(RwUInt8 byMessage)
{
	m_pSceneNode->OnLoadingEvent(byMessage);	
}





//////////////////////////////////////////////////////////////////////////
//
//	CGameLoadingScene
//
//////////////////////////////////////////////////////////////////////////


CGameLoadingScene::CGameLoadingScene( void )
{
	GetNtlGuiManager()->GetGuiManager()->DisableInput(this);
	
	GetSoundManager()->SetProhibition( CHANNEL_GROUP_UI_SOUND, TRUE );

	m_eInnerOldState		= EGAMELOADSTATE_NONE;
	m_eInnerState			= EGAMELOADSTATE_NONE;

	m_bFirstEnter			= FALSE;

	m_pLoadingGui			= NTL_NEW CLoadingGui("LoadingGui");
	if( !m_pLoadingGui->Create() )
	{
		m_pLoadingGui->Destroy();
		NTL_DELETE(m_pLoadingGui);
	}

	SetInnerState( EGAMELOADSTATE_FIRST_STEP_PREPARE );
}


CGameLoadingScene::~CGameLoadingScene( void )
{
	GetSoundManager()->SetProhibition( CHANNEL_GROUP_UI_SOUND, FALSE );
	
	if( m_pLoadingGui )
	{
		m_pLoadingGui->Destroy();
		NTL_DELETE(m_pLoadingGui);
	}

	GetNtlGuiManager()->GetGuiManager()->EnableInput(this);

	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_END_MAIN_THEME);


	// 게임 로딩이 끝났을 때 로딩중 서버와의 연결이 끈겼다면 그에 대한 처리를 한다
	SConnectData* pConnectData = GetDboGlobal()->GetConnectData();
	if( pConnectData )
	{
		if( SConnectAtomic::SOCKET_STATE_DISCONNECT == pConnectData->sGameCon.byConnState )
		{
			CDboEventGenerator::GameServerConnect(SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT);
		}
	}	
}


void CGameLoadingScene::SetFirstEnterMode( RwBool bFirstEnter )
{
	m_bFirstEnter = bFirstEnter;
}


RwBool CGameLoadingScene::Update( RwReal fElapsed )
{
	if ( IsFinish() )
	{
		return FALSE;
	}

	m_pLoadingGui->Update(fElapsed);

	return TRUE;
}


void CGameLoadingScene::Render( void )
{
	if( m_pLoadingGui )
		m_pLoadingGui->Render();
}


RwBool CGameLoadingScene::IsFinish( void )
{
	return (EGAMELOADSTATE_FINISH == m_eInnerState) ? TRUE : FALSE;
}


void CGameLoadingScene::GameLoadingFinished( void )
{
	GetNtlGuiManager()->GetGuiManager()->EnableInput(this);

	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController( WORLD_CONCEPT_FIRST_GRADE );

	if ( pWorldConcept )
	{
		switch ( pWorldConcept->GetConceptType() )
		{
		case WORLD_PLAY_TUTORIAL:
			{
				SetInnerState( EGAMELOADSTATE_SECOND_STEP_PREPARE );
			}
			break;

		default:
			{
				SetInnerState( EGAMELOADSTATE_FINISH );
			}
			break;
		}
	}
	else
	{
		if ( m_bFirstEnter )
		{
			SetInnerState( EGAMELOADSTATE_SECOND_STEP_PREPARE );
		}
		else
		{
			SetInnerState( EGAMELOADSTATE_FINISH );
		}
	}
}


void CGameLoadingScene::SetInnerState( EGameLoadingState eInnerState )
{
	m_eInnerOldState = m_eInnerState;
	m_eInnerState = eInnerState;

	switch ( eInnerState )
	{
		case EGAMELOADSTATE_NONE:
		break;

		case EGAMELOADSTATE_FIRST_STEP_PREPARE:
		{
			for( RwUInt8 i = 0 ; i < NUM_CHANNEL_GROUP ; ++i )
			{
				if( CHANNEL_GROUP_BGM == i )
					continue;

				GetSoundManager()->FadeOut(i, 0.f, dBGM_FADE_OUT_TIME);
			}

			CInputHandler::GetInstance()->SetActive( FALSE );

			if( m_pLoadingGui )
				m_pLoadingGui->Load_LoadingPage(LOADING_PAGE_FIRST_STEP);

			SetInnerState( EGAMELOADSTATE_FIRST_STEP );
		}
		break;

		case EGAMELOADSTATE_FIRST_STEP:
		{
		}
		break;

		case EGAMELOADSTATE_SECOND_STEP_PREPARE:
		{
			if( m_pLoadingGui )
				m_pLoadingGui->Load_LoadingPage(LOADING_PAGE_SECOND_STEP);

			SetInnerState( EGAMELOADSTATE_SECOND_STEP );
		}
		break;

		case EGAMELOADSTATE_SECOND_STEP:
		{
			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_PROLOG_THMEM_BGM_PLAY);
		}
		break;

		case EGAMELOADSTATE_THIRD_STEP_PREPARE:
		{
			if( m_pLoadingGui )
				m_pLoadingGui->Load_LoadingPage(LOADING_PAGE_THIRD_STEP);

			SetInnerState( EGAMELOADSTATE_THIRD_STEP );
		}
		break;

		case EGAMELOADSTATE_THIRD_STEP:
		{			
		}
		break;

		case EGAMELOADSTATE_FINISH:
		{
			for( RwUInt8 i = 0 ; i < NUM_CHANNEL_GROUP ; ++i )
			{
				if( CHANNEL_GROUP_BGM == i )
					continue;

				GetSoundManager()->FadeIn(i, 1.f, dBGM_FADE_OUT_TIME);
			}

			CInputHandler::GetInstance()->SetActive( TRUE );

			CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController( WORLD_CONCEPT_FIRST_GRADE );

			if ( pWorldConcept )
			{
				switch ( pWorldConcept->GetConceptType() )
				{
					case WORLD_PLAY_TIME_MACHINE:
					{
						((CNtlWorldConceptTMQ*)pWorldConcept)->SetTMQState_from_Cleint(WORLD_STATE_TMQ_TERRAIN_READY);
					}
					break;

					case WORLD_PLAY_TUTORIAL:
					{
						((CNtlWorldConceptTutorial*)pWorldConcept)->SetTMQState_from_Cleint(WORLD_STATE_TMQ_TERRAIN_READY);
					}
					break;
				}
			}
		}
		break;
	}
}

void CGameLoadingScene::OnLoadingEvent(RwUInt8 byMessage)
{
	if( LOADING_EVENT_SECOND_MOVIE_END == byMessage )
	{
		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController( WORLD_CONCEPT_FIRST_GRADE );

		if ( pWorldConcept )
		{
			switch ( pWorldConcept->GetConceptType() )
			{
			case WORLD_PLAY_TUTORIAL:
				{
					SetInnerState( EGAMELOADSTATE_THIRD_STEP_PREPARE );
				}
				break;

			default:
				{
					SetInnerState( EGAMELOADSTATE_FINISH );
				}
				break;
			}
		}
		else
		{
			SetInnerState( EGAMELOADSTATE_FINISH );
		}
	}
	else if( LOADING_EVENT_THIRD_MOVIE_END == byMessage )
	{
		SetInnerState( EGAMELOADSTATE_FINISH );
	}
}

