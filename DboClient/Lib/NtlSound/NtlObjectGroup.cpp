#include "precomp_ntlsound.h"
#include "NtlObjectGroup.h"

#include "NtlSound.h"
#include "NtlSoundManager.h"
#include "NtlSoundGlobal.h"
#include "NtlSoundEvent.h"
#include "NtlFadeInOut.h"

CNtlObjectGroup::CNtlObjectGroup(eChannelGroupType eGroup) 
:CNtlChannelGroup(eGroup)
{
	m_mapReleasedSound.clear();
	m_mapSleepingSound.clear();

	LinkMsg(g_EventSoundReleaseObjectGroup);
}

CNtlObjectGroup::~CNtlObjectGroup() 
{	
	Destory();
}

void CNtlObjectGroup::Destory()
{
	NTL_FUNCTION( "CNtlChannelGroup::Destory" );

	for(SOUND_ITER it = m_mapReleasedSound.begin(); it != m_mapReleasedSound.end(); ++it)
	{
		CNtlSound* pSound = (*it).second;		
		pSound->Release();
	}
	m_mapReleasedSound.clear();


	for(SOUND_ITER it = m_mapSleepingSound.begin(); it != m_mapSleepingSound.end(); ++it)
	{
		CNtlSound* pSound = (*it).second;		
		pSound->Release();
	}
	m_mapSleepingSound.clear();


	CNtlChannelGroup::Destory();

	UnLinkMsg(g_EventSoundReleaseObjectGroup);

	NTL_RETURNVOID();
}

bool CNtlObjectGroup::Stop(SOUND_HANDLE hHandle)
{
	DelReleasedSound(hHandle);
	DelSleepingSound(hHandle);

	return CNtlChannelGroup::Stop(hHandle);
}

CNtlSound* CNtlObjectGroup::GetSound(SOUND_HANDLE hHandle)
{
	CNtlSound* pSound = NULL;

	for(SOUND_ITER it = m_mapReleasedSound.begin(); it != m_mapReleasedSound.end(); ++it)
	{
		pSound = (*it).second;

		if( hHandle == pSound->m_hHandle )
			return pSound;
	}

	for(SOUND_ITER it = m_mapSleepingSound.begin(); it != m_mapSleepingSound.end(); ++it)
	{
		pSound = (*it).second;

		if( hHandle == pSound->m_hHandle )
			return pSound;
	}

	pSound = CNtlChannelGroup::GetSound(hHandle);

	return pSound;
}

bool CNtlObjectGroup::ReleaseLowRankChannel() 
{
	SOUND_ITER it = m_mapGroup.begin();
	CNtlSound* pSound = it->second;

	if(pSound->m_pFMODSound)
	{
		GetFadeInOut()->StopImmdiately(pSound->m_pFMODChannel, dFADE_STOP_FLAG_NOT_NOTIFY);

		// 강제종료되는 Sound의 정보저장
		StoreReleasedSound(pSound);

		pSound->m_pFMODChannel->stop();
		pSound->m_pFMODChannel = NULL;

		pSound->m_pFMODSound->release();
		pSound->m_pFMODSound = NULL;

		m_mapGroup.erase(it);

		return true;
	}

	return false;
}

bool CNtlObjectGroup::IsExistSound(SOUND_HANDLE hHandle)
{
	if( CNtlChannelGroup::IsExistSound(hHandle) )
		return true;

	SOUND_ITER it = m_mapReleasedSound.find(hHandle);
	if( it != m_mapReleasedSound.end() )
		return true;

	it = m_mapSleepingSound.find(hHandle);
	if( it != m_mapSleepingSound.end() )
		return true;

	return false;
}

void CNtlObjectGroup::PostUpdate(float fXPos, float fYPos, float fZPos)
{
	float fLength, fMinDistance, fMaxDistance;
	FMOD_VECTOR vPos;	


	// 일정 반경안의 플레이 되지 않은 오브젝트 뮤직 플레이
	SOUND_ITER it = m_mapSleepingSound.begin();
	while( it != m_mapSleepingSound.end() )
	{
		CNtlSound* pSound = it->second;

		fLength = sqrt( (pSound->m_fXPos - fXPos)*(pSound->m_fXPos - fXPos) + 
						(pSound->m_fYPos - fYPos)*(pSound->m_fYPos - fYPos) + 
						(pSound->m_fZPos - fZPos)*(pSound->m_fZPos - fZPos) );

		if( fLength <= pSound->m_fMaxDistance )
		{
			int iResult = GetSoundManager()->ReplayEnvironmentSound(pSound);

			if( iResult == SOUNDRESULT_OK )
			{
				it = m_mapSleepingSound.erase(it);
				continue;
			}			
		}

		++it;
	}

	// 일정 반경 밖으로 벗어난 오브젝트 뮤직 찾기
	SOUND_MAP mapDelSound;
	for( it = m_mapGroup.begin() ; it != m_mapGroup.end() ; ++it )
	{
		CNtlSound* pSound = it->second;

		pSound->m_pFMODChannel->get3DAttributes(&vPos, NULL);
		pSound->m_pFMODChannel->get3DMinMaxDistance(&fMinDistance, &fMaxDistance);

		vPos.x = dCONVERT_COORDINATE_X(vPos.x);

		fLength = sqrt( (vPos.x - fXPos)*(vPos.x - fXPos) + (vPos.y - fYPos)*(vPos.y - fYPos) + (vPos.z - fZPos)*(vPos.z - fZPos) );		
		if( fMaxDistance < fLength )
		{
			mapDelSound[it->first] = it->second;
		}
	}

	SOUND_ITER it_del = mapDelSound.begin();
	for( ; it_del != mapDelSound.end() ; ++it_del )
	{
		CNtlSound* pSound = it_del->second;

		GetFadeInOut()->StopImmdiately(pSound->m_pFMODChannel, dFADE_STOP_FLAG_NOT_NOTIFY);
		
		pSound->m_pFMODChannel->stop();
		pSound->m_pFMODChannel = NULL;

		pSound->m_pFMODSound->release();
		pSound->m_pFMODSound = NULL;

		m_mapGroup.erase(pSound->m_hHandle);

		StoreSleepingSound(pSound);
	}
}

void CNtlObjectGroup::StoreReleasedSound(CNtlSound* pSound)
{
	pSound->m_eState = SPS_SLEEP;
	m_mapReleasedSound[pSound->m_hHandle] = pSound;
}

void CNtlObjectGroup::StoreSleepingSound(CNtlSound* pSound)
{
	pSound->m_eState = SPS_SLEEP;
	m_mapSleepingSound[pSound->m_hHandle] = pSound;
}

void CNtlObjectGroup::DelSleepingSound(SOUND_HANDLE hHandle)
{
	SOUND_ITER it = m_mapSleepingSound.find(hHandle);
	if( it != m_mapSleepingSound.end() )
	{
		CNtlSound* pSound = it->second;
		pSound->Release();

		m_mapSleepingSound.erase(it);
	}
}

void CNtlObjectGroup::DelReleasedSound(SOUND_HANDLE hHandle)
{
	// 만약 soundHandle이 강제종료되어 다시 플레이되길 기다리는 오브젝트 뮤직이라면 삭제한다.
	SOUND_ITER it = m_mapReleasedSound.find(hHandle);
	if( it != m_mapReleasedSound.end() )
	{
		CNtlSound* pSound = it->second;
		pSound->Release();

		m_mapReleasedSound.erase(it);
	}
}

CNtlSound* CNtlObjectGroup::GetReleasedSoundbyPriority()
{
	SOUND_ITER it = m_mapReleasedSound.begin();

	if( it != m_mapReleasedSound.end() )
	{
		CNtlSound* pSound = it->second;
		return pSound;
	}

	return NULL;
}

void CNtlObjectGroup::SuccessRelay()
{
	SOUND_ITER it = m_mapReleasedSound.begin();

	if( it != m_mapReleasedSound.end() )
	{
		m_mapReleasedSound.erase(it);
	}
}

VOID CNtlObjectGroup::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CNtlBGMGroup::HandleEvents");

	if( msg.Id == g_EventSoundReleaseObjectGroup )
	{
		for(SOUND_ITER it = m_mapReleasedSound.begin(); it != m_mapReleasedSound.end(); ++it)
		{
			CNtlSound* pSound = (*it).second;		
			pSound->Release();
		}
		m_mapReleasedSound.clear();


		for(SOUND_ITER it = m_mapSleepingSound.begin(); it != m_mapSleepingSound.end(); ++it)
		{
			CNtlSound* pSound = (*it).second;		
			pSound->Release();
		}
		m_mapSleepingSound.clear();


		SOUND_ITER it = m_mapGroup.begin();
		for( ; it != m_mapGroup.end() ; ++it )
		{
			CNtlSound* pSound = it->second;		
			pSound->Release();
		}
		m_mapGroup.clear();
	}

	NTL_RETURNVOID();
}