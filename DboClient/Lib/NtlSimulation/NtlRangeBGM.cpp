#include "precomp_ntlsimulation.h"
#include "NtlRangeBGM.h"

// core
#include "NtlMath.h"
#include "NtlDebug.h"

// sound
#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"
#include "NtlSoundLogic.h"
#include "NtlBGMGroup.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"

#define dRANGE_BGM_UPDATE_TIME				(2.f)


CNtlRangeBGM::CNtlRangeBGM()
:m_hPlayingSound(INVALID_SOUND_HANDLE)
,m_fUpdateSkipTime(0.f)
{

}

CNtlRangeBGM::~CNtlRangeBGM()
{

}

RwBool CNtlRangeBGM::Create()
{
	NTL_FUNCTION("CNtlRangeBGM::Create");

	NTL_RETURN(TRUE);
}

VOID CNtlRangeBGM::Destory()
{
	NTL_FUNCTION("CNtlRangeBGM::Destroy");

	m_hPlayingSound = INVALID_SOUND_HANDLE;

	MAP_RANGE_BGM::iterator it_Range = m_mapRangeBGM.begin();
	for( ; it_Range != m_mapRangeBGM.end() ; ++it_Range )
	{
		sRANGE_BGM* pRANGE_BGM = it_Range->second;
		NTL_DELETE(pRANGE_BGM);
	}	
	m_mapRangeBGM.clear();

	NTL_RETURNVOID();
}

VOID CNtlRangeBGM::Update(float fElapsed)
{
	m_fUpdateSkipTime -= fElapsed;
	if( 0.f < m_fUpdateSkipTime )
		return;

	m_fUpdateSkipTime = dRANGE_BGM_UPDATE_TIME;


	Update_PlayingRangeBGM(fElapsed);
	Update_PlayableRangeBGM(fElapsed);
}

VOID CNtlRangeBGM::Update_PlayingRangeBGM(RwReal fElapsed)
{
	if( INVALID_SOUND_HANDLE == m_hPlayingSound )
		return;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;


	const RwV3d&			v3AvatarPosition	= pAvatar->GetPosition();
	MAP_RANGE_BGM::iterator	it_Range			= m_mapRangeBGM.begin();

	for( ; it_Range != m_mapRangeBGM.end() ; ++it_Range )
	{
		sRANGE_BGM* pRANGE_BGM = it_Range->second;

		if( pRANGE_BGM->hHandle == m_hPlayingSound )
		{
			if( pRANGE_BGM->fRadius <= CNtlMath::GetLength(v3AvatarPosition, pRANGE_BGM->v3Position) )
			{
				m_hPlayingSound		= INVALID_SOUND_HANDLE;
				pRANGE_BGM->hHandle	= INVALID_SOUND_HANDLE;
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_RAGNE_STOP);
			}

			break;
		}
	}
}

VOID CNtlRangeBGM::Update_PlayableRangeBGM(RwReal fElapsed)
{
	if( INVALID_SOUND_HANDLE != m_hPlayingSound )
		return;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	const RwV3d&			v3AvatarPosition	= pAvatar->GetPosition();
	MAP_RANGE_BGM::iterator	it_Range			= m_mapRangeBGM.begin();


	for( ; it_Range != m_mapRangeBGM.end() ; ++it_Range )
	{
		sRANGE_BGM* pRANGE_BGM = it_Range->second;

		if( pRANGE_BGM->fRadius > CNtlMath::GetLength(v3AvatarPosition, pRANGE_BGM->v3Position) )
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
			tSoundParam.pcFileName		= (char*)pRANGE_BGM->strFileName.c_str();
			tSoundParam.eBGM_Type		= BGM_TYPE_RANGE;
			GetSoundManager()->Play(&tSoundParam);

			if( INVALID_SOUND_HANDLE != tSoundParam.hHandle )
			{
				m_hPlayingSound		= tSoundParam.hHandle;
				pRANGE_BGM->hHandle	= tSoundParam.hHandle;
				break;
			}	
		}
	}
}

VOID CNtlRangeBGM::RangeBGM_Register(RANGE_BGM_KEY key, const RwChar* pcFileName, RwReal fRadius, RwV3d* pPosition)
{
	sRANGE_BGM* pRANGE_BGM = GetRangeBGM(key);
	if( pRANGE_BGM )
	{
		DBO_FAIL("Already exist same key : " << key);
		return;
	}

	pRANGE_BGM = NTL_NEW sRANGE_BGM;

	pRANGE_BGM->hHandle			= INVALID_SOUND_HANDLE;
	pRANGE_BGM->strFileName		= pcFileName;
	pRANGE_BGM->fRadius			= fRadius;
	pRANGE_BGM->v3Position		= *pPosition;

	m_mapRangeBGM[key] = pRANGE_BGM;
}

VOID CNtlRangeBGM::RangeBGM_Unregister(RANGE_BGM_KEY key)
{
	sRANGE_BGM* pRANGE_BGM = GetRangeBGM(key);
	if( !pRANGE_BGM )
		return;

	if( INVALID_SOUND_HANDLE != m_hPlayingSound && pRANGE_BGM->hHandle == m_hPlayingSound )
	{
		m_hPlayingSound = INVALID_SOUND_HANDLE;
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_RAGNE_STOP);
	}

	NTL_DELETE(pRANGE_BGM);
	m_mapRangeBGM.erase(key);
}

VOID CNtlRangeBGM::Released_RangeSound()
{
	if( INVALID_SOUND_HANDLE == m_hPlayingSound )
		return;


	MAP_RANGE_BGM::iterator it_Range = m_mapRangeBGM.begin();

	for( ; it_Range != m_mapRangeBGM.end() ; ++it_Range )
	{
		sRANGE_BGM* pRANGE_BGM = it_Range->second;

		if( pRANGE_BGM->hHandle == m_hPlayingSound )
		{
			pRANGE_BGM->hHandle = INVALID_SOUND_HANDLE;
			break;
		}
	}

	m_hPlayingSound = INVALID_SOUND_HANDLE;
}

CNtlRangeBGM::sRANGE_BGM* CNtlRangeBGM::GetRangeBGM(RANGE_BGM_KEY key)
{
	MAP_RANGE_BGM::iterator it_Range = m_mapRangeBGM.find(key);
	if( it_Range == m_mapRangeBGM.end() )
		return NULL;

	return it_Range->second;
}

VOID CNtlRangeBGM::HandleEvents(RWS::CMsg &pMsg)
{	
	NTL_FUNCTION("CNtlRangeBGM::HandleEvents");

	if( pMsg.Id == g_EventSoundAddListRangeBGM )
	{
		SNtlEventAddListRangeBGM* pEvent = reinterpret_cast<SNtlEventAddListRangeBGM*>( pMsg.pData );
		RANGE_BGM_KEY key = Logic_GetNewSoundHandle();

		*pEvent->phResultHandle = key;

		RangeBGM_Register(key, pEvent->pcFileName, pEvent->fRadius, pEvent->pPosition);		
	}
	else if( pMsg.Id == g_EventSoundDelListRangeBGM )
	{
		SNtlEventDelListRangeBGM* pEvent = reinterpret_cast<SNtlEventDelListRangeBGM*>( pMsg.pData );	
		RANGE_BGM_KEY key = pEvent->hSound;

		RangeBGM_Unregister(key);
	}
	else if( pMsg.Id == g_EventSoundEventMusic )
	{
		SNtlEeventMusic* pEvent = reinterpret_cast<SNtlEeventMusic*>( pMsg.pData );

		if( EVENT_MUSIC_REMOVE_RAGNE_BGM == pEvent->byType )
			Released_RangeSound();
	}

	NTL_RETURNVOID();
}