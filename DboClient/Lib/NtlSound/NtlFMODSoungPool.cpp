#include "precomp_ntlsound.h"
#include "NtlFMODSoungPool.h"

#include "GUISoundDefine.h"
#include "NtlSoundGlobal.h"


#define dMAX_SOUNDPOOL_SIZE			10485760	// 20M (1024 * 1024 * 20 )

CNtlFMODSoundPool* CNtlFMODSoundPool::m_pInstance = NULL;


CNtlFMODSoundPool::CNtlFMODSoundPool()
:m_uiFileSize_FMODSound(0)
,m_uiFileSize_ResidentPool(0)
{
}

CNtlFMODSoundPool::~CNtlFMODSoundPool()
{
}

RwBool CNtlFMODSoundPool::CreateInstance()
{
	NTL_FUNCTION("CNtlFMODSoundPool::CreateInstance");

	if( m_pInstance )
	{
		NTL_ASSERT(false, "CNtlFMODSoundPool::CreateInstance, instance already exist");
		NTL_RETURN(FALSE);
	}

	m_pInstance = NTL_NEW CNtlFMODSoundPool;

	m_pInstance->LoadResidentPool();

	NTL_RETURN(TRUE);
}

VOID CNtlFMODSoundPool::DestroyInstance()
{
	NTL_FUNCTION("CNtlFMODSoundPool::DestroyInstance");

	if( !m_pInstance )
		NTL_RETURNVOID();

	MAP_RESIDENT::iterator it_resident = m_pInstance->m_mapResidentPool.begin();
	for( ; it_resident != m_pInstance->m_mapResidentPool.end() ; ++it_resident )
	{
		FMOD::Sound* pFMODSound = it_resident->second;
		pFMODSound->release();
		pFMODSound = NULL;
	}
	m_pInstance->m_mapResidentPool.clear();

	NTL_DELETE(m_pInstance);

	NTL_RETURNVOID();
}

FMOD::Sound* CNtlFMODSoundPool::GetFMODSound(const char* pcFileName)
{
	MAP_RESIDENT::iterator it_resident = m_mapResidentPool.find(pcFileName);
	if( it_resident != m_mapResidentPool.end() )
		return it_resident->second;

	/*
	MAP_FMODSOUND::iterator it_FMOD = m_mapFMODSoundPool.find(pcFileName);
	if( it_FMOD != m_mapFMODSoundPool.end() )
	{
	return it_FMOD->second->pFMODSound;
	}
	else
	{
	RwUInt32 uiFileSize = NtlGetFileSize(pcFileName);
	}
	*/

	return NULL;
}

VOID CNtlFMODSoundPool::LoadResidentPool()
{
	std::string strFullName;
	FMOD_RESULT result;
	FMOD_MODE mode				= d2D_SOUND_MODE;
	FMOD::Sound* pFMODSound		= NULL;
	char acBuffer[4][256] =
	{
		GSD_SYSTEM_BUTTON_FOCUS,
		GSD_SYSTEM_BUTTON_CLICK,
		GSD_SYSTEM_WINDOW_OPEN,
		GSD_SYSTEM_WINDOW_CLOSE
	};


	for( RwUInt8 i = 0 ; i < 4 ; ++i )
	{
		strFullName = CNtlSoundGlobal::m_strFilePath;
		strFullName.append( acBuffer[i] );

		result = CNtlSoundGlobal::m_pFMODSystem->createSound(strFullName.c_str(), mode, 0, &pFMODSound);
		if( result != FMOD_OK )
		{
			Logic_NtlSoundLog("CNtlFMODSoundPool::LoadResidentPool, fail to create sound of name : ", strFullName.c_str());
			continue;
		}

		m_mapResidentPool[ acBuffer[i] ] = pFMODSound;
	}
}