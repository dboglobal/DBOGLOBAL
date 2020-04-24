#include "precomp_ntlsound.h"
#include "NtlFadeInOut.h"

#include "NtlSoundEvent.h"
#include "NtlSoundEventGenerator.h"
#include "NtlSound.h"
#include "NtlChannelGroup.h"


CNtlFadeInOut* CNtlFadeInOut::m_pInstance = NULL;


CNtlFadeInOut::CNtlFadeInOut()
{	
	m_listFade.clear();
}

CNtlFadeInOut::~CNtlFadeInOut()
{
	m_listFade.clear();
}

bool CNtlFadeInOut::CreateInstance()
{
	NTL_FUNCTION("CNtlFadeInOut::CreateInstance");

	if( m_pInstance )
	{
		NTL_ASSERT(m_pInstance, "CNtlFadeInOut::CreateInstance, instance already exist");
		NTL_RETURN(FALSE);
	}

	m_pInstance = NTL_NEW CNtlFadeInOut;

	m_pInstance->LinkMsg(g_EventSoundReleaseSound);
	
	NTL_RETURN(TRUE);
}

void CNtlFadeInOut::DestroyInstance()
{
	NTL_FUNCTION("CNtlFadeInOut::DestroyInstance");

	if( !m_pInstance )
		NTL_RETURNVOID();


	m_pInstance->UnLinkMsg(g_EventSoundReleaseSound);

	NTL_DELETE(m_pInstance);

	NTL_RETURNVOID();
}

void CNtlFadeInOut::Update()
{	
	if( m_listFade.empty() )
		return;

	FADE_ITER it = m_listFade.begin();
	while( it != m_listFade.end() )
	{
		sFadeInOut& fade = *it;

		if( fade.bFinish )
		{
			it = m_listFade.erase(it);
			continue;
		}

		unsigned long ulCurTime = GetTickCount();

		// 각각의 sFadeInOut의 업데이트 타임을 체크한다
		if( ulCurTime - fade.ulApplyTime >= SOUND_FADEINOUT_INTERVAL_TIME )
		{
			if( fade.eResourceType == SRT_CHANNEL )
			{
				sNtlVolume* pNtlVolume = &(fade.u1_pSound->m_tVolume);

				if( fade.eFadeType == FADE_IN && fade.fDestVolume <= pNtlVolume->fFadeVolume )
				{
					fade.bFinish = true;
				}
				else if( fade.eFadeType == FADE_OUT && fade.fDestVolume >= pNtlVolume->fFadeVolume )
				{
					fade.bFinish = true;
				}

				fade.ulApplyTime			= ulCurTime;

				pNtlVolume->fFadeVolume = Logic_GetFMODValidVolume(pNtlVolume->fFadeVolume + fade.fAdjustVolume);
				fade.u1_pSound->m_pFMODChannel->setVolume( Logic_CalcPlayVolume(pNtlVolume) );
			}
			else if( fade.eResourceType == SRT_CHANNEL_GROUP )
			{
				sNtlVolume* pNtlVolume = fade.u2_pChannelGroup->GetNtlVolume();
 
				if( fade.eFadeType == FADE_IN && fade.fDestVolume <= pNtlVolume->fFadeVolume )
				{
					fade.bFinish = true;
				}
				else if( fade.eFadeType == FADE_OUT && fade.fDestVolume >= pNtlVolume->fFadeVolume )
				{
					fade.bFinish = true;
				}
				
				fade.ulApplyTime			= ulCurTime;

				FMOD::ChannelGroup* pFMODChannelGroup = fade.u2_pChannelGroup->GetFMODChannelGroup();
				pNtlVolume->fFadeVolume = Logic_GetFMODValidVolume(pNtlVolume->fFadeVolume + fade.fAdjustVolume);
				pFMODChannelGroup->setVolume( Logic_CalcPlayVolume(pNtlVolume) );
			}
			else
			{	// 만에 하나 ChannelGroup 클래스 단에서 삭제가 되어 sFadeInOut에서
				// 가지고 있는 포인터가 더이상 데이터를 가리키지 않는다면 삭제한다.
				fade.bFinish = true;
			}
		}

		if( fade.bFinish )
		{
			if( fade.eResourceType == SRT_CHANNEL )
			{					
				fade.u1_pSound->m_eState = SPS_PLAYING;
				CNtlSoundEventGenerator::SendFinishFade(fade);				
			}
		}

		++it;
	}
}

void CNtlFadeInOut::FadeIn(CNtlSound* pSound, float fDestVolume, unsigned long ulTime)
{
	if( !pSound )
		return;

	if( ulTime < 0 )
		return;

	if( fDestVolume <= pSound->m_tVolume.fFadeVolume )
		return;

	if(pSound->m_pFMODChannel && !IsFadeIn(pSound->m_pFMODChannel) /*&& !IsFadeOut(pSound->m_pFMODChannel)*/)	// if we do fade-out check, then it might happen, that we will never fade-in and then sound is always 0
	{
		sFadeInOut fade;

		fade.eFadeType						= FADE_IN;
		fade.eResourceType					= SRT_CHANNEL;
		fade.bFinish						= false;
		fade.fDestVolume					= Logic_GetFMODValidVolume(fDestVolume);
		fade.fAdjustVolume					= (fade.fDestVolume - pSound->m_tVolume.fFadeVolume) /
											  (ulTime / SOUND_FADEINOUT_INTERVAL_TIME);
		fade.ulApplyTime					= GetTickCount();

		fade.u1_hHandle						= pSound->m_hHandle;
		fade.u1_pSound						= pSound;
		fade.u1_pSound->m_tVolume.fFadeVolume	= pSound->m_tVolume.fMainVolume;		

		m_listFade.push_back(fade);

		pSound->m_eState = SPS_PLAY_FADE_IN;
	}
}

void CNtlFadeInOut::FadeIn(CNtlChannelGroup* pChannelGroup, float fDestVolume, unsigned long ulTime)
{
	if( !pChannelGroup )
		return;

	if( ulTime < 0 )
		return;

	sNtlVolume* pNtlVolume = pChannelGroup->GetNtlVolume();
	if( fDestVolume <= pNtlVolume->fFadeVolume )
		return;

	if(pChannelGroup && !IsFadeIn(pChannelGroup) /*&& !IsFadeOut(pChannelGroup)*/)	// if we do fade-out check, then it might happen, that we will never fade-in and then sound is always 0
	{
		sFadeInOut fade;

		fade.eFadeType						= FADE_IN;
		fade.eResourceType					= SRT_CHANNEL_GROUP;
		fade.bFinish						= false;
		fade.fDestVolume					= Logic_GetFMODValidVolume(fDestVolume);
		fade.fAdjustVolume					= (fade.fDestVolume - pNtlVolume->fFadeVolume) /
											  (ulTime / SOUND_FADEINOUT_INTERVAL_TIME);
		fade.ulApplyTime					= GetTickCount();

		fade.u2_pChannelGroup				= pChannelGroup;

		m_listFade.push_back(fade);
	}
}

void CNtlFadeInOut::FadeOut(CNtlSound* pSound, float fDestVolume, unsigned long ulTime)
{
	if( !pSound )
		return;

	if( ulTime < 0 )
		return;

	if( fDestVolume >= pSound->m_tVolume.fFadeVolume )
		return;

	if(pSound->m_pFMODChannel && !IsFadeIn(pSound->m_pFMODChannel) && !IsFadeOut(pSound->m_pFMODChannel))
	{
		sFadeInOut fade;

		fade.eFadeType						= FADE_OUT;
		fade.eResourceType					= SRT_CHANNEL;
		fade.bFinish						= false;
		fade.fDestVolume					= Logic_GetFMODValidVolume(fDestVolume);
		fade.fAdjustVolume					= (fade.fDestVolume - pSound->m_tVolume.fFadeVolume) /
											  (ulTime / SOUND_FADEINOUT_INTERVAL_TIME);
		fade.ulApplyTime					= GetTickCount();

		fade.u1_hHandle						= pSound->m_hHandle;
		fade.u1_pSound						= pSound;
		fade.u1_pSound->m_tVolume.fFadeVolume	= pSound->m_tVolume.fMainVolume;		

		m_listFade.push_back(fade);

		pSound->m_eState = SPS_PLAY_FADE_OUT;
	}
}

void CNtlFadeInOut::FadeOut(CNtlChannelGroup* pChannelGroup, float fDestVolume, unsigned long ulTime)
{
	if( !pChannelGroup )
		return;

	if( ulTime < 0 )
		return;

	sNtlVolume* pNtlVolume = pChannelGroup->GetNtlVolume();
	if( fDestVolume >= pNtlVolume->fFadeVolume )
		return;

	if(pChannelGroup && !IsFadeIn(pChannelGroup) && !IsFadeOut(pChannelGroup))
	{
		sFadeInOut fade;

		fade.eFadeType						= FADE_OUT;
		fade.eResourceType					= SRT_CHANNEL_GROUP;
		fade.bFinish						= false;
		fade.fDestVolume					= Logic_GetFMODValidVolume(fDestVolume);
		fade.fAdjustVolume					= (fade.fDestVolume - pNtlVolume->fFadeVolume) /
											  (ulTime / SOUND_FADEINOUT_INTERVAL_TIME);
		fade.ulApplyTime					= GetTickCount();

		fade.u2_pChannelGroup				= pChannelGroup;

		m_listFade.push_back(fade);
	}
}

void CNtlFadeInOut::StopImmdiately(FMOD::Channel* m_pFMODChannel, RwUInt8 byFlag /* = 0 */)
{
	FADE_ITER it = m_listFade.begin();
	for( ; it != m_listFade.end() ; ++it )
	{
		sFadeInOut& fade = *it;

		if( fade.eResourceType == SRT_CHANNEL &&
			fade.u1_pSound->m_pFMODChannel == m_pFMODChannel )
		{
			if( (byFlag & dFADE_STOP_FLAG_NOT_NOTIFY) == false )
			{
				fade.u1_pSound->m_eState = SPS_PLAYING;
				CNtlSoundEventGenerator::SendFinishFade(fade);
			}
			
			m_listFade.erase(it);

			break;
		}
	}
}

void CNtlFadeInOut::StopImmdiately(CNtlChannelGroup* pChannelGroup, RwUInt8 byFlag /* = 0 */)
{
	FADE_ITER it = m_listFade.begin();
	for( ; it != m_listFade.end() ; ++it )
	{
		sFadeInOut& fade = *it;

		if( fade.eResourceType == SRT_CHANNEL_GROUP &&
			fade.u2_pChannelGroup == pChannelGroup )
		{
			if( (byFlag & dFADE_STOP_FLAG_NOT_NOTIFY) == false )
			{
				fade.u1_pSound->m_eState = SPS_PLAYING;
				CNtlSoundEventGenerator::SendFinishFade(fade);
			}

			m_listFade.erase(it);
			break;
		}		
	}
}

bool CNtlFadeInOut::IsFadeIn(FMOD::Channel* pChannel)
{
	FADE_ITER it = m_listFade.begin();
	for( ; it != m_listFade.end() ; ++it )
	{
		sFadeInOut& fade = *it;

		if( fade.eResourceType != SRT_CHANNEL )
			continue;

		if( fade.u1_pSound->m_pFMODChannel != pChannel )
			continue;

		if( fade.eFadeType != FADE_IN )
			return false;

		if( fade.bFinish )
			return false;
		else
			return true;
	}

	return false;
}

bool CNtlFadeInOut::IsFadeOut(FMOD::Channel* pChannel)
{
	FADE_ITER it = m_listFade.begin();
	for( ; it != m_listFade.end() ; ++it )
	{
		sFadeInOut& fade = *it;

		if( fade.eResourceType != SRT_CHANNEL )
			continue;

		if( fade.u1_pSound->m_pFMODChannel != pChannel )
			continue;

		if( fade.eFadeType != FADE_OUT )
			return false;

		if( fade.bFinish )
			return false;
		else
			return true;
	}

	return false;
}

bool CNtlFadeInOut::IsFadeIn(CNtlChannelGroup* pChannelGroup)
{
	FADE_ITER it = m_listFade.begin();
	for( ; it != m_listFade.end() ; ++it )
	{
		sFadeInOut& fade = *it;

		if( fade.eResourceType != SRT_CHANNEL_GROUP )
			continue;
			
		if( fade.u2_pChannelGroup != pChannelGroup )
			continue;

		if( fade.eFadeType != FADE_IN )
			return false;

		if( fade.bFinish )
			return false;
		else
			return true;
	}

	return false;
}

bool CNtlFadeInOut::IsFadeOut(CNtlChannelGroup* pChannelGroup)
{
	FADE_ITER it = m_listFade.begin();
	for( ; it != m_listFade.end() ; ++it )
	{
		sFadeInOut& fade = *it;

		if( fade.eResourceType != SRT_CHANNEL_GROUP )
			continue;

		if( fade.u2_pChannelGroup != pChannelGroup )
			continue;

		if( fade.eFadeType != FADE_OUT )
			return false;
			
		if( fade.bFinish )
			return false;
		else
			return true;
	}

	return false;
}

VOID CNtlFadeInOut::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CNtlFadeInOut::HandleEvents");

	if( msg.Id == g_EventSoundReleaseSound )
	{
		CNtlSound* pReleasingSound = reinterpret_cast<CNtlSound*>( msg.pData );
		FADE_ITER it = m_listFade.begin();
		for( ; it != m_listFade.end() ; ++it )
		{
			sFadeInOut& fade = *it;

			if( fade.eResourceType != SRT_CHANNEL )
				continue;

			if( fade.bFinish )
				continue;

			if( fade.u1_hHandle == pReleasingSound->m_hHandle )
			{
				fade.u1_pSound->SetVolume_FadeInit();
				fade.bFinish = true;
				break;
			}
		}
	}

	NTL_RETURNVOID();
}