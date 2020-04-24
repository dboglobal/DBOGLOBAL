#include "precomp_ntlsound.h"
#include "NtlMusicGroup.h"

#include "NtlSoundDefines.h"
#include "NtlSoundGlobal.h"
#include "NtlSoundEvent.h"
#include "NtlSoundEventGenerator.h"
#include "NtlSound.h"
#include "NtlFadeInOut.h"
#include "NtlSoundManager.h"

CNtlMusicGroup::CNtlMusicGroup(eChannelGroupType eGroup)
:CNtlChannelGroup(eGroup)
,m_fDelayReplayTime(120.f)
,m_fReplayElapsed(0.f)
,m_hPostSound(INVALID_SOUND_HANDLE)
{	
	LinkMsg(g_EventSoundFinishFade);
	LinkMsg(g_EventSoundRestTime);
}

CNtlMusicGroup::~CNtlMusicGroup()
{
	UnLinkMsg(g_EventSoundFinishFade);
	UnLinkMsg(g_EventSoundRestTime);
}

void CNtlMusicGroup::Update(float fElapsed)
{
	if( m_fReplayElapsed > 0.f )
	{
		m_fReplayElapsed -= fElapsed;

		if( m_fReplayElapsed <= 0.f )
		{
			m_fReplayElapsed = 0.f;

			SOUND_ITER it = m_mapGroup.begin();
			if( it != m_mapGroup.end() )
			{
				CNtlSound* pSound = it->second;

				if( GetSoundManager()->Replay(pSound) == SOUNDRESULT_OK )
				{
					pSound->m_pFMODChannel->setChannelGroup(m_pMasterLayer);
				}
				else
				{
					m_fReplayElapsed = m_fDelayReplayTime;
				}				
			}
		}
	}

	CNtlChannelGroup::Update(fElapsed);
}

eStoreResult CNtlMusicGroup::StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta)
{
	int iChannelCount = m_mapGroup.size();
	pSound->m_pFMODChannel->setChannelGroup(m_pMasterLayer);

	if( iChannelCount == 2 )
	{
		CNtlChannelGroup::Stop(m_hPostSound);
		m_hPostSound = INVALID_SOUND_HANDLE;
	}
	else if( iChannelCount == 1 )
	{
		SOUND_ITER it = m_mapGroup.begin();

		if( m_fReplayElapsed > 0.f )
		{
			// replay 대기 상태면 바로 없앤다
			Stop(it->second->m_hHandle);
		}
		else
		{
			// 현재 연주되는 음악은 PostMusic이 되어 Fade Out 된다.			
			FMOD_MODE mode;
			CNtlSound* pCurSound = it->second;

			pCurSound->m_pFMODSound->getMode(&mode);

			mode &= !FMOD_LOOP_NORMAL;
			mode &= FMOD_LOOP_OFF;

			pCurSound->m_pFMODSound->setMode(mode);

			GetFadeInOut()->FadeOut(pCurSound, 0.0f, dBGM_FADE_OUT_TIME);

			m_hPostSound = pCurSound->m_hHandle;
		}		

		m_fReplayElapsed = 0.f;
	}

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

	return CNtlChannelGroup::StoreSound(pSound, pParameta);
}

bool CNtlMusicGroup::Stop(SOUND_HANDLE hHandle)
{
	if( hHandle == m_hPostSound )
	{
		// 즉각 해제
		CNtlChannelGroup::Stop(m_hPostSound);
		m_hPostSound = INVALID_SOUND_HANDLE;

		return true;
	}
	else
	{
		SOUND_ITER it = m_mapGroup.find(hHandle);
		if( it != m_mapGroup.end() )
		{
			// 음악은 Fade out후 해제된다
			FMOD_MODE mode;
			CNtlSound* pSound = it->second;

			pSound->m_pFMODSound->getMode(&mode);

			mode &= !FMOD_LOOP_NORMAL;
			mode &= FMOD_LOOP_OFF;
			pSound->m_pFMODSound->setMode(mode);

			GetFadeInOut()->FadeOut(pSound, 0.0f, dBGM_FADE_OUT_TIME);

			return true;
		}
	}

	return false;
}

void CNtlMusicGroup::ReleaseFinishedSound(float fElapsed)
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
					if( m_mapGroup.size() == 1 )
					{
						if( m_fReplayElapsed == 0.f )
							m_fReplayElapsed = m_fDelayReplayTime;
					}
					else
					{
						pSound->Release();
						it = m_mapGroup.erase(it);
						continue;
					}					
				}
			}			
		}

		++it;
	}
}

VOID CNtlMusicGroup::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CNtlMusicGroup::HandleEvents");

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
						CNtlChannelGroup::Stop(it->first);

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
			m_fDelayReplayTime = pEvent->fRestTime;
		}
	}

	NTL_RETURNVOID();
}