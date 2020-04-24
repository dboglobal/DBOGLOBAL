#include "precomp_ntlsound.h"
#include "NtlBGMGroup.h"

// sound
#include "NtlSoundDefines.h"
#include "GUISoundDefine.h"
#include "NtlSoundGlobal.h"
#include "NtlSoundEvent.h"
#include "NtlSoundEventGenerator.h"
#include "NtlSound.h"
#include "NtlFadeInOut.h"
#include "NtlSoundManager.h"
#include "NtlBGMPlayList.h"


#define dDEFAULT_REPLAY_REST_TIME			(10.f)
#define dREST_TIME_AFTER_RANGE_BGM_STOP		(7.f)
#define dREST_TIME_LEAVE_WORLD_CONCEPT		(60.f)


CNtlBGMGroup::CNtlBGMGroup(eChannelGroupType eGroup)
:CNtlChannelGroup(eGroup)
,m_fReplayElapsed(dDEFAULT_REPLAY_REST_TIME)
,m_pBGMPlayList(NULL)
{
	m_tPlaySound.hHandle		= INVALID_SOUND_HANDLE;
	m_tPlaySound.eType			= INVALID_BGM_TYPE;

	m_tPostSound.hHandle		= INVALID_SOUND_HANDLE;
	m_tPostSound.eType			= INVALID_BGM_TYPE;
}

CNtlBGMGroup::~CNtlBGMGroup()
{	
}

bool CNtlBGMGroup::Create(FMOD::ChannelGroup* pChannelGroup, unsigned int uiSoundDuplicatoin /* = 0 */)
{
	m_pBGMPlayList = NTL_NEW CNtlBGMPlayList;
	if( !m_pBGMPlayList->Create() )
		return false;
	
	LinkMsg(g_EventSoundFinishFade);
	LinkMsg(g_EventSoundRestTime);
	LinkMsg(g_EventSoundEventMusic);
	LinkMsg(g_EventSoundAddListShareBGM);
	LinkMsg(g_EventSoundDeleteAllListShareBGM);
	LinkMsg(g_EventSoundShareBGMPlayRate);	

	return CNtlChannelGroup::Create(pChannelGroup, uiSoundDuplicatoin);
}

void CNtlBGMGroup::Destory()
{
	if( m_pBGMPlayList )
	{
		m_pBGMPlayList->Destroy();
		NTL_DELETE(m_pBGMPlayList);
	}

	UnLinkMsg(g_EventSoundFinishFade);
	UnLinkMsg(g_EventSoundRestTime);
	UnLinkMsg(g_EventSoundEventMusic);
	UnLinkMsg(g_EventSoundAddListShareBGM);
	UnLinkMsg(g_EventSoundDeleteAllListShareBGM);
	UnLinkMsg(g_EventSoundShareBGMPlayRate);

	CNtlChannelGroup::Destory();
}

void CNtlBGMGroup::Update(float fElapsed)
{
	Update_Replay(fElapsed);

	CNtlChannelGroup::Update(fElapsed);
}

void CNtlBGMGroup::Update_Replay(float fElapsed)
{
	if( INVALID_SOUND_HANDLE != m_tPlaySound.hHandle )
		return;

	m_fReplayElapsed -= fElapsed;
	if( m_fReplayElapsed > 0.f )
		return;
	else
		m_fReplayElapsed = 0.f;

	sPLAY_BGM_PARAMETA* pPLAY_BGM_PARAMETA = m_pBGMPlayList->GetBGM_TopPriority();
	if( !pPLAY_BGM_PARAMETA )
		return;

	if( FALSE == pPLAY_BGM_PARAMETA->bEnableParameta )
		return;


	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
	tSoundParam.eBGM_Type		= pPLAY_BGM_PARAMETA->eType;
	tSoundParam.pcFileName		= (RwChar*)pPLAY_BGM_PARAMETA->strFileName.c_str();
	tSoundParam.bLoop			= pPLAY_BGM_PARAMETA->bLoop;

	if( SOUNDRESULT_OK == GetSoundManager()->Play(&tSoundParam) )
	{
		m_tPlaySound.hHandle		= tSoundParam.hHandle;
		m_tPlaySound.eType			= pPLAY_BGM_PARAMETA->eType;

		m_fReplayElapsed			= pPLAY_BGM_PARAMETA->fReplayRestTime;
	}
	else
	{
		m_fReplayElapsed			= dDEFAULT_REPLAY_REST_TIME;
	}	
}

eStoreResult CNtlBGMGroup::StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta)
{
	if( m_pBGMPlayList->IsExistMorePriorityBGM(pParameta->eBGM_Type) )
	{
		if( BGM_TYPE_SHARE_THEME != pParameta->eBGM_Type )
		{
			if( BGM_TYPE_LOCAL_THEME == pParameta->eBGM_Type )
			{
				m_pBGMPlayList->AddBGMList(BGM_TYPE_LOCAL_THEME, pParameta->pcFileName);
				return STORE_LIST;
			}

			return STORE_FAIL;
		}
	}

	if( BGM_TYPE_LOCAL_THEME < pParameta->eBGM_Type )
	{
		m_pBGMPlayList->SetLock(BGM_TYPE_SHARE_THEME, TRUE);
		m_pBGMPlayList->SetLock(BGM_TYPE_LOCAL_THEME, TRUE);
	}

	int iChannelCount = m_mapGroup.size();
	if( iChannelCount == 2 )
	{
		StopImmediately(m_tPostSound.hHandle);
		Stop(m_tPlaySound.hHandle);
	}
	else if( iChannelCount == 1 )
	{
		Stop(m_tPlaySound.hHandle);
	}

	if( BGM_TYPE_LOCAL_THEME < pParameta->eBGM_Type )
	{
		m_pBGMPlayList->SetLock(BGM_TYPE_SHARE_THEME, FALSE);
		m_pBGMPlayList->SetLock(BGM_TYPE_LOCAL_THEME, FALSE);
	}


	pSound->m_pFMODChannel->setChannelGroup(m_pMasterLayer);

	if( pSound->m_bLoop )
	{
		FMOD_MODE mode;

		pSound->m_pFMODSound->getMode(&mode);

		mode &= !FMOD_LOOP_OFF;
		mode &= FMOD_LOOP_NORMAL;

		pSound->m_pFMODSound->setMode(mode);
	}
	else
	{
		FMOD_MODE mode;

		pSound->m_pFMODSound->getMode(&mode);

		mode = FMOD_DEFAULT;

		pSound->m_pFMODSound->setMode(mode);
	}

	eStoreResult eResult = CNtlChannelGroup::StoreSound(pSound, pParameta);
	
	if( STORE_READY_TO_PLAY == eResult )
	{
		// 따로 이벤트로 받았을 때만 기록
		if( BGM_TYPE_SHARE_THEME != pParameta->eBGM_Type )
			m_pBGMPlayList->AddBGMList(pParameta->eBGM_Type, pParameta->pcFileName);

		m_fReplayElapsed		= m_pBGMPlayList->GetRestTime(pParameta->eBGM_Type);

		m_tPlaySound.hHandle	= pSound->m_hHandle;
		m_tPlaySound.eType		= pParameta->eBGM_Type;
	}

	return eResult;
}

bool CNtlBGMGroup::Stop(SOUND_HANDLE hHandle)
{
	SOUND_ITER it = m_mapGroup.find(hHandle);
	if( it == m_mapGroup.end() )
		return false;

	CNtlSound* pSound = it->second;

	if( hHandle == m_tPostSound.hHandle )
	{
		// 즉각 해제
		CNtlChannelGroup::Stop(m_tPostSound.hHandle);
		ClearBGMList(m_tPostSound.hHandle);
		return true;
	}

	m_tPostSound.hHandle	= m_tPlaySound.hHandle;
	m_tPostSound.eType		= m_tPlaySound.eType;

	ClearBGMList(m_tPlaySound.hHandle);


	// 무한루프 무조건 해제후 Fade out
	FMOD_MODE mode;
	pSound->m_pFMODSound->getMode(&mode);
	mode &= !FMOD_LOOP_NORMAL;
	mode &= FMOD_LOOP_OFF;
	pSound->m_pFMODSound->setMode(mode);


	GetFadeInOut()->FadeOut(pSound, 0.0f, dBGM_FADE_OUT_TIME);

	return false;
}

void CNtlBGMGroup::StopImmediately(SOUND_HANDLE hHandle)
{
	SOUND_ITER it = m_mapGroup.find(hHandle);
	if( it == m_mapGroup.end() )
		return;

	ClearBGMList(hHandle);
	CNtlChannelGroup::Stop(hHandle);
}

void CNtlBGMGroup::ClearBGMList(SOUND_HANDLE hHandle)
{
	if( m_tPlaySound.hHandle == hHandle )
	{
		if( BGM_TYPE_RANGE == m_tPlaySound.eType )
		{
			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_REMOVE_RAGNE_BGM);
		}

		m_pBGMPlayList->ClearBGMList(m_tPlaySound.eType);
		m_tPlaySound.hHandle		= INVALID_SOUND_HANDLE;
		m_tPlaySound.eType			= INVALID_BGM_TYPE;
	}
	else if( m_tPostSound.hHandle == hHandle )
	{
		if( BGM_TYPE_RANGE == m_tPostSound.eType )
		{
			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_REMOVE_RAGNE_BGM);
		}

		m_pBGMPlayList->ClearBGMList(m_tPostSound.eType);
		m_tPostSound.hHandle		= INVALID_SOUND_HANDLE;
		m_tPostSound.eType			= INVALID_BGM_TYPE;
	}
}

void CNtlBGMGroup::ReleaseFinishedSound(float fElapsed)
{
	FMOD_MODE mode;

	SOUND_ITER it = m_mapGroup.begin();
	while( it != m_mapGroup.end() )
	{
		CNtlSound* pSound = it->second;
		if( pSound->m_pFMODChannel && pSound->m_pFMODSound )
		{			
			pSound->m_pFMODChannel->getMode(&mode);

			if( mode ^ FMOD_LOOP_NORMAL )
			{	// 무한반복이 아닐 때
				bool bPlaying;

				// paused 된 사운드도 true 값이 리턴된다
				pSound->m_pFMODChannel->isPlaying(&bPlaying);

				if(!bPlaying)
				{
					if( m_tPlaySound.hHandle == pSound->m_hHandle )
					{
						m_tPlaySound.hHandle	= INVALID_SOUND_HANDLE;
						m_tPlaySound.eType		= INVALID_BGM_TYPE;
					}
					else if( m_tPostSound.hHandle == pSound->m_hHandle )
					{
						m_tPostSound.hHandle	= INVALID_SOUND_HANDLE;
						m_tPostSound.eType		= INVALID_BGM_TYPE;
					}

					pSound->Release();
					it = m_mapGroup.erase(it);
					continue;
				}
			}			
		}

		++it;
	}
}

VOID CNtlBGMGroup::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CNtlBGMGroup::HandleEvents");

	if( msg.Id == g_EventSoundFinishFade )
	{
		sFadeInOut* pFade = reinterpret_cast<sFadeInOut*>( msg.pData );

		if( pFade->eResourceType == SRT_CHANNEL )
		{
			SOUND_ITER it = m_mapGroup.begin();
			for( ; it != m_mapGroup.end() ; ++it )
			{
				if( pFade->u1_hHandle == (*it->second).m_hHandle )
				{
					if( pFade->eFadeType == FADE_OUT )
					{
						CNtlChannelGroup::Stop(it->first);
					}

					break;
				}
			}
		}
	}
	else if( msg.Id == g_EventSoundRestTime )
	{
		SNtlEventMusicRestTime* pEvent = reinterpret_cast<SNtlEventMusicRestTime*>( msg.pData );

		if( (eChannelGroupType)pEvent->byChannelGroup == m_eGroup )
		{
			if( pEvent->byBGMType == BGM_TYPE_SHARE_THEME ||
				pEvent->byBGMType == BGM_TYPE_LOCAL_THEME )
			{
				m_pBGMPlayList->SetRestTime(BGM_TYPE_SHARE_THEME, pEvent->fRestTime);
				m_pBGMPlayList->SetRestTime(BGM_TYPE_LOCAL_THEME, pEvent->fRestTime);
			}
			else
			{
				m_pBGMPlayList->SetRestTime((eBGMType)pEvent->byBGMType, pEvent->fRestTime);
			}

			if( m_tPlaySound.hHandle != INVALID_SOUND_HANDLE )
			{
				if( m_tPlaySound.eType == pEvent->byBGMType )
					m_fReplayElapsed = pEvent->fRestTime;
			}
		}
	}
	else if( msg.Id == g_EventSoundEventMusic )
	{
		SNtlEeventMusic* pEvent = reinterpret_cast<SNtlEeventMusic*>( msg.pData );

		switch( pEvent->byType )
		{
		case EVENT_MUSIC_LEAVE_MAIN_WORLD_CONCEPT:
			{
				if( m_tPlaySound.hHandle != INVALID_SOUND_HANDLE )
					Stop(m_tPlaySound.hHandle);

				// A new world concept, a break that waits for loading to end
				m_fReplayElapsed = dREST_TIME_LEAVE_WORLD_CONCEPT;
				
				break;
			}
		case EVENT_MUSIC_START_PVP_BGM:
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_PVP;
				tSoundParam.eBGM_Type		= BGM_TYPE_PVP;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_END_PVP_BGM:
			{
				if( BGM_TYPE_PVP == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);

				break;
			}
		case EVENT_MUSIC_START_PRIVATE_RANKBATTLE_BGM:
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_PRIVATE_RANKBATTLE;
				tSoundParam.eBGM_Type		= BGM_TYPE_RANKBATTLE_BATTLE;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_START_PARTY_RANKBATTLE_BGM:
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_PARTY_RANKBATTLE;
				tSoundParam.eBGM_Type		= BGM_TYPE_RANKBATTLE_BATTLE;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_END_PRIVATE_RANKBATTLE_BGM:
		case EVENT_MUSIC_END_PARTY_RANKBATTLE_BGM:
			{
				if( BGM_TYPE_RANKBATTLE_BATTLE == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);

				break;
			}
		case EVENT_MUSIC_END_CINEMATIC_BGM:
			{
				if( BGM_TYPE_CINEMATIC == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);

				break;
			}
		case EVENT_MUSIC_START_MAIN_THEME:
			{
				if( BGM_TYPE_MAIN_THEME == m_tPlaySound.eType )
					return;

				// The main BGM is repeatedly played until the BGM is separately played in the game stage
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_MAIN_THEME;
				tSoundParam.eBGM_Type		= BGM_TYPE_MAIN_THEME;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_PROLOG_THMEM_BGM_PLAY:
			{
				// The main BGM is repeatedly played until the BGM is separately played in the game stage
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_PROLOG;
				tSoundParam.eBGM_Type		= BGM_TYPE_MAIN_THEME;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_END_MAIN_THEME:
			{
				if( BGM_TYPE_MAIN_THEME == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);

				break;
			}
		case EVENT_MUSIC_IMMEDIATELY_END_MAIN_THEME:
			{
				StopImmediately(m_tPlaySound.hHandle);
				StopImmediately(m_tPostSound.hHandle);
				break;
			}
		case EVENT_MUSIC_FINISH_THEME_BGM:
			{
				if( BGM_TYPE_THEME_BGM == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);

				m_pBGMPlayList->ClearBGMList(BGM_TYPE_THEME_BGM);

				break;
			}
		case EVENT_MUSIC_LOOP_THEME_BGM:
			{
				m_pBGMPlayList->SetLoop(BGM_TYPE_THEME_BGM, false);
				break;
			}
		case EVENT_MUSIC_NOT_LOOP_THEME_BGM:
			{
				m_pBGMPlayList->SetLoop(BGM_TYPE_THEME_BGM, true);
				break;
			}
		case EVENT_MUSIC_TEMP_FINISH_TMQ:
			{
				if( BGM_TYPE_SERVER_PLAYED == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);

				break;
			}
		case EVENT_MUSIC_TB_DIRECTION_BGM_PLAY:
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_TB_STANDBY;
				tSoundParam.eBGM_Type		= BGM_TYPE_LOCAL_THEME;
				tSoundParam.bLoop			= true;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_TB_PARTY_BGM_PLAY:
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_TB_PARTY_BATTLE;
				tSoundParam.eBGM_Type		= BGM_TYPE_LOCAL_THEME;
				tSoundParam.bLoop			= true;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_TB_PRIVATE_BGM_PLAY:
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_TB_PRIVATE_BATTLE;
				tSoundParam.eBGM_Type		= BGM_TYPE_LOCAL_THEME;
				tSoundParam.bLoop			= true;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_TB_DIRECTION_BGM_STOP:
		case EVENT_MUSIC_TB_PARTY_BGM_STOP:
		case EVENT_MUSIC_TB_PRIVATE_BGM_STOP:
			{
				if( BGM_TYPE_LOCAL_THEME == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);
				
				break;
			}
		case EVENT_MUSIC_RAGNE_STOP:
			{
				if( BGM_TYPE_RANGE == m_tPlaySound.eType )
				{					
					Stop(m_tPlaySound.hHandle);
					ClearBGMList(m_tPlaySound.hHandle);

					m_fReplayElapsed = dREST_TIME_AFTER_RANGE_BGM_STOP;

					CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_REMOVE_RAGNE_BGM);
				}
				else if( BGM_TYPE_RANGE == m_tPostSound.eType )
				{
					ClearBGMList(m_tPostSound.hHandle);

					m_fReplayElapsed = dREST_TIME_AFTER_RANGE_BGM_STOP;

					CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_REMOVE_RAGNE_BGM);
				}
				
				break;
			}
		case EVENT_MUSIC_START_SCRAMBLE_READY_BGM:
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_DOJO_SCRAMBLE_READY;
				tSoundParam.eBGM_Type		= BGM_TYPE_MAIN_THEME;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_STOP_SCRAMBLE_READY_BGM:
			{
				if( BGM_TYPE_MAIN_THEME == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);

				break;
			}
		case EVENT_MUSIC_START_SCRAMBLE_START_BGM:
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= GSD_BGM_DOJO_SCRAMBLE_START;
				tSoundParam.eBGM_Type		= BGM_TYPE_MAIN_THEME;
				tSoundParam.bLoop			= true;
				GetSoundManager()->Play(&tSoundParam);
				break;
			}
		case EVENT_MUSIC_END_SCRAMBLE_START_BGM:
			{
				if( BGM_TYPE_MAIN_THEME == m_tPlaySound.eType )
					Stop(m_tPlaySound.hHandle);
				break;
			}
		}
	}
	else if( msg.Id == g_EventSoundAddListShareBGM )
	{
		SNtlEventAddListShareBGM* pEvent = reinterpret_cast<SNtlEventAddListShareBGM*>( msg.pData );
		m_pBGMPlayList->AddBGMList(BGM_TYPE_SHARE_THEME, pEvent->pcFileName);
	}
	else if( msg.Id == g_EventSoundDeleteAllListShareBGM )
	{
		m_pBGMPlayList->ClearBGMList(BGM_TYPE_SHARE_THEME);
	}
	else if( msg.Id == g_EventSoundShareBGMPlayRate )
	{
		SNtlEventShareBGMPlayRate* pEvent = reinterpret_cast<SNtlEventShareBGMPlayRate*>( msg.pData );
		m_pBGMPlayList->SetShareBGMSelectRate(pEvent->fPlayRate);
	}

	NTL_RETURNVOID();
}