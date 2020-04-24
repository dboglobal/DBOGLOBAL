#include "precomp_ntlsound.h"
#include "NtlFMODSoundPool.h"

#include "GUISoundDefine.h"
#include "NtlSoundGlobal.h"
#include "NtlSoundLogic.h"


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
	char acBuffer[5][256] =
	{
		GSD_SYSTEM_BUTTON_FOCUS,
		GSD_SYSTEM_BUTTON_CLICK,
		GSD_SYSTEM_WINDOW_OPEN,
		GSD_SYSTEM_WINDOW_CLOSE,
		GSD_SYSTEM_KEY_DOWN
	};


	for( RwUInt8 i = 0 ; i < 5 ; ++i )
	{
		strFullName = CNtlSoundGlobal::m_strFilePath;
		strFullName.append( acBuffer[i] );


		if( g_fnCallback_LoadSound_from_Memory )
		{
			void* pData = NULL;
			FMOD_CREATESOUNDEXINFO exinfo;
			memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
			exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);

			(*g_fnCallback_LoadSound_from_Memory)(strFullName.c_str(), (void**)&pData, (int*)&exinfo.length);

			if( pData )
			{
				mode |= FMOD_OPENMEMORY;
				result = CNtlSoundGlobal::m_pFMODSystem->createSound((const char*)pData, mode, &exinfo, &pFMODSound );

				NTL_ARRAY_DELETE( pData );
			}
			else
			{
				result = FMOD_ERR_MEMORY_CANTPOINT;
			}
		}
		else
		{
			result = CNtlSoundGlobal::m_pFMODSystem->createSound(strFullName.c_str(), mode, 0, &pFMODSound );
		}

		if( result != FMOD_OK )
		{
			Logic_NtlSoundLog("CNtlFMODSoundPool::LoadResidentPool, fail to create sound of name : ", strFullName.c_str());
			continue;
		}

		m_mapResidentPool[ acBuffer[i] ] = pFMODSound;
	}
}