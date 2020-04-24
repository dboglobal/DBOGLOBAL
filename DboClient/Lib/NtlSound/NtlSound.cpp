#include "precomp_ntlsound.h"
#include "NtlSound.h"

#include "NtlSoundDSP.h"
#include "NtlSoundEventGenerator.h"
#include "NtlSoundGlobal.h"


namespace
{
#ifndef NDEBUG
	std::map<SOUND_HANDLE, std::string>		mapDebugSoundName;
#endif
};


RwFreeList* CNtlSound::m_pFreeList	= NULL;


CNtlSound::CNtlSound(SOUND_HANDLE hHandle, char* pcFileName)
:m_hHandle(hHandle)
,m_pFMODSound(NULL)
,m_pFMODChannel(NULL)
,m_eState(SPS_NONE)
,m_bCreatedfromSoundPool(false)
,pMemoryData(NULL)
{
#ifndef NDEBUG
	NTL_ASSERT(mapDebugSoundName.end() == mapDebugSoundName.find(hHandle), "CNtlSound::CNtlSound, Already exist sound handle");
	mapDebugSoundName[hHandle] = pcFileName;
#endif
	//DBO_WARNING_MESSAGE("Create Music: " << pcFileName << ", Handle: " << hHandle);
	m_strName = pcFileName;

	m_pDSP = NTL_NEW CNtlSoundDSP;
}


CNtlSound::~CNtlSound()
{
#ifndef NDEBUG
	mapDebugSoundName.erase(m_hHandle);
#endif

	if(m_pDSP)
	{
		m_pDSP->Destroy();
		NTL_DELETE(m_pDSP);
	}
}

void* CNtlSound::operator new(size_t size)
{
	if( m_pFreeList == NULL )
		m_pFreeList = RwFreeListCreate(sizeof(CNtlSound), NTL_SOUND_MEMORY_POOL, 16, rwMEMHINTDUR_GLOBAL);

	return RwFreeListAlloc(m_pFreeList, rwMEMHINTDUR_GLOBAL);
}

void CNtlSound::operator delete(void *pObj)
{
	RwFreeListFree(m_pFreeList, pObj);
}

void CNtlSound::Shutdown()
{
	if( !m_pFreeList )
		return;

#ifndef NDEBUG
	NTL_ASSERT(mapDebugSoundName.size() == 0, "CNtlSound::Shutdown, some sound data is lefted");
#endif

	RwFreeListDestroy(m_pFreeList);
	m_pFreeList = NULL;
}

void CNtlSound::Release()
{
	if( m_pFMODSound )
	{
		CNtlSoundEventGenerator::SendReleaseSound(this);

		m_pFMODChannel->stop();
		m_pFMODChannel = NULL;

		if( !m_bCreatedfromSoundPool )
			m_pFMODSound->release();

		m_pFMODSound = NULL;
	}
	//DBO_WARNING_MESSAGE("Stop music Handle: " << m_hHandle);
	FreeMemoryData();
	CNtlSoundGlobal::m_setPlayingSoundHandle.erase(m_hHandle);

	CNtlSound* pThis = this;

	NTL_DELETE( pThis );
}

void CNtlSound::FreeMemoryData()
{
	NTL_ARRAY_DELETE( pMemoryData );
}

void CNtlSound::SetVolume_FadeInit()
{
	m_tVolume.fFadeVolume = Logic_GetFMODValidVolume(1.f);

	if( m_pFMODChannel )
		m_pFMODChannel->setVolume( Logic_CalcPlayVolume(&m_tVolume) );
}

void CNtlSound::SetMinMax(float fMin, float fMax)
{
	if( !m_pFMODChannel )
		return;

	// If the maximum distance value in FMOD is less than or equal to the minimum distance, no attenuation occurs.
	// So the sound keeps coming out of range.
	if( fMin + MIN_DISTANCE_BETWEEN_MINMAX > fMax )
	{
		Logic_NtlSoundLog("CNtlSound::SetMinMax, The min distance is longer far than max distacne", m_strName.c_str());
		fMax = fMin + MIN_DISTANCE_BETWEEN_MINMAX;
	}

	m_tMinMax.fMin	= fMin;
	m_tMinMax.fMax	= fMax;

	m_pFMODChannel->set3DMinMaxDistance(m_tMinMax.fMin * CNtlSoundGlobal::m_fMinMaxRate,
										m_tMinMax.fMax * CNtlSoundGlobal::m_fMinMaxRate);
}