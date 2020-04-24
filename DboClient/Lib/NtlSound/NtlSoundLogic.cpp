#include "precomp_ntlsound.h"
#include "NtlSoundLogic.h"

#include "NtlSoundManager.h"
#include "NtlSoundGlobal.h"
#include "NtlSoundEventGenerator.h"
#include "NtlSound.h"
#include "NtlFMODSoundPool.h"
#include "NtlChannelGroup.h"

namespace
{
	char acBuffer[1024] = "";
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load from memory
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CallbackLoadSoundFile g_fnCallback_LoadSound_from_Memory = NULL;

void LinkLoadSound_from_Memory(CallbackLoadSoundFile fn)
{
	g_fnCallback_LoadSound_from_Memory = fn;
}

void UnLinkLoadSound_from_Memory()
{
	g_fnCallback_LoadSound_from_Memory = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IsExistSoundFile
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CallbackIsExistSoundFile g_fnCallback_IsExistSoundFile = NULL;

void Link_IsExistSoundFile(CallbackIsExistSoundFile fn)
{
	g_fnCallback_IsExistSoundFile = fn;
}

void UnLink_IsExistSoundFile()
{
	g_fnCallback_IsExistSoundFile = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// log
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID Logic_NtlSoundLog(const RwChar* pcContition, const RwChar* pcSoundFileName /* = NULL */)
{
	if( pcSoundFileName )
		sprintf_s(acBuffer, 1024, "[Sound] Condition : %s, SoundFile : %s\n", pcContition, pcSoundFileName);
	else
		sprintf_s(acBuffer, 1024, "[Sound] Condition : %s\n", pcContition);
		

	// 애플리케이션 실행중 이전에 발생했던 에러 메세지는 리턴	
	if( Logic_IsExistLog(acBuffer) )
		return;

	CNtlSoundGlobal::m_setMsgError.insert(acBuffer);
	NtlLogFilePrint(acBuffer);
}

VOID Logic_NtlSoundLog(const RwChar* pcContition, RwInt32 iChannelGroup, const RwChar* pcSoundFileName /* = NULL */)
{
	char acChannelGroupName[128] = "";
	sprintf_s(acChannelGroupName, 128, "%s", GetSoundManager()->GetGroupName(iChannelGroup) );

	if( pcSoundFileName )
		sprintf_s(acBuffer, 1024, "[Sound] Condition : %s, Channel group : %s, SoundFile : %s\n", pcContition, acChannelGroupName, pcSoundFileName);
	else
		sprintf_s(acBuffer, 1024, "[Sound] Condition : %s, Channel group : %s\n", pcContition, acChannelGroupName);


	// 애플리케이션 실행중 이전에 발생했던 에러 메세지는 리턴	
	if( Logic_IsExistLog(acBuffer) )
		return;

	CNtlSoundGlobal::m_setMsgError.insert(acBuffer);
	NtlLogFilePrint(acBuffer);
}

VOID Logic_NtlSoundLog(const RwChar* pcContition, FMOD_RESULT result, const RwChar* pcSoundFileName /* = NULL */)
{
	if( result == FMOD_OK )
		return;

	const char* pcResult = FMOD_ErrorString(result);

	if( pcSoundFileName )
		sprintf_s(acBuffer, 1024, "[Sound] Condition : %s, FMOD log : %s, SoundFile : %s\n", pcContition, pcResult, pcSoundFileName);
	else
		sprintf_s(acBuffer, 1024, "[Sound] Condition : %s, FMOD log : %s\n", pcContition, pcResult);


	// 애플리케이션 실행중 이전에 발생했던 에러 메세지는 리턴	
	if( Logic_IsExistLog(acBuffer) )
		return;

	CNtlSoundGlobal::m_setMsgError.insert(acBuffer);
	NtlLogFilePrint(acBuffer);
}

RwBool Logic_IsExistLog(const RwChar* pcText)
{
	SET_ERRORMSG::iterator it = CNtlSoundGlobal::m_setMsgError.find(pcText);
	if( it != CNtlSoundGlobal::m_setMsgError.end() )
		return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sound operation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwReal Logic_GetFMODValidVolume(RwReal fVolume)
{
	if( fVolume > 1.f )
		fVolume = 1.f ;
	else if( fVolume < 0.f )
		fVolume = 0.f;

	return fVolume;
}

RwReal Logic_CalcPlayVolume(sNtlVolume* pNtlVolume)
{
	// 모든 볼륨 인자는 Logic_GetFMODValidVolume() 함수를 거친 후여야 한다
	if( !pNtlVolume )
		return 1.f;

	return pNtlVolume->fMainVolume * pNtlVolume->fFadeVolume * pNtlVolume->fWhenMoviePlayVolume;
}

RwReal Logic_GetFMODValidPitch(RwReal fPitch)
{
	if( fPitch > 10.f )
		fPitch = 10.f;
	else if( fPitch < 0.f )
		fPitch = 0.f;

	return fPitch;
}

RwReal Logic_CalcPlayPitch(sNtlPitch* pNtlPitch)
{
	if( !pNtlPitch )
		return 1.f;

	return pNtlPitch->fMainPitch * pNtlPitch->fPitchRate * pNtlPitch->fSlowMotionPitchRate;
}

RwBool Logic_IsExistFile(const RwChar* pcFileName)
{
	if( !pcFileName )
		return FALSE;

	if( g_fnCallback_IsExistSoundFile )
	{
		return (*g_fnCallback_IsExistSoundFile)(pcFileName);
	}
	else
	{
		struct _finddata_t c_file;
		intptr_t hFile;

		if( (hFile = _findfirst(pcFileName, &c_file )) == -1L )
			return FALSE;

		_findclose( hFile );
		return TRUE;
	}	
}

SOUND_HANDLE Logic_GetNewSoundHandle()
{
	static bool bUseMaxHandle = false;
	static SOUND_HANDLE hSound = INVALID_SOUND_HANDLE;

	if( !CNtlSoundGlobal::m_pFMODSystem )
		return INVALID_SOUND_HANDLE;

	++hSound;

	if( bUseMaxHandle )
	{
		bool bExist = false;

		while(true)
		{
			if( INVALID_SOUND_HANDLE == hSound )
			{
				hSound = Logic_GetNewSoundHandle();
				return hSound;
			}

			for(int i = 0 ; i < NUM_CHANNEL_GROUP ; ++i)
			{				
				bExist = GetSoundManager()->GetChannelGroup(i)->IsExistSound(hSound);

				// 현재 쓰고 있지 않은 핸들을 찾았다
				if( !bExist )
				{
					return hSound;
				}
			}

			++hSound;
		}		
	}
	else
	{
		if( INVALID_SOUND_HANDLE == hSound )
		{
			bUseMaxHandle = true;
			hSound = Logic_GetNewSoundHandle();
		}
	}

	return hSound;
}

FMOD_RESULT API_Create_Stream(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta, FMOD_MODE mode, std::string& strFullName)
{
	// Sound 반복 연주
	if( pParameta->bLoop )				
		mode |= FMOD_LOOP_NORMAL;

	
	// File에 직접 접근
	if( !g_fnCallback_LoadSound_from_Memory )
		return CNtlSoundGlobal::m_pFMODSystem->createStream(strFullName.c_str(), mode, 0, &(pSound->m_pFMODSound) );


	// Pack 모드
	FMOD_CREATESOUNDEXINFO exinfo;
	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);

	(*g_fnCallback_LoadSound_from_Memory)(strFullName.c_str(), (void**)&pSound->pMemoryData, (int*)&exinfo.length);


	// Pack 되지 않은 사운드 파일인지 검사
	if( !pSound->pMemoryData )		
		return FMOD_ERR_MEMORY_CANTPOINT;


	mode |= FMOD_OPENMEMORY;
	return CNtlSoundGlobal::m_pFMODSystem->createStream((const char*)pSound->pMemoryData, mode, &exinfo, &(pSound->m_pFMODSound) );
}

FMOD_RESULT API_Create_Sound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta, FMOD_MODE mode, std::string& strFullName)
{
	// Sound 반복 연주
	if( pParameta->bLoop )				
		mode |= FMOD_LOOP_NORMAL;


	FMOD::Sound* pFMODSound = GetSoundPool()->GetFMODSound( pSound->m_strName.c_str() );

	if( pFMODSound )
	{
		pSound->m_bCreatedfromSoundPool = true;
		pSound->m_pFMODSound			= pFMODSound;

		return FMOD_OK;
	}

	
	// File에 직접 접근
	if( !g_fnCallback_LoadSound_from_Memory )
		return CNtlSoundGlobal::m_pFMODSystem->createSound(strFullName.c_str(), mode, 0, &(pSound->m_pFMODSound) );
	

	// Pack 모드
	void* pData = NULL;
	FMOD_CREATESOUNDEXINFO exinfo;
	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);

	(*g_fnCallback_LoadSound_from_Memory)(strFullName.c_str(), (void**)&pData, (int*)&exinfo.length);


	// Pack 되지 않은 사운드 파일인지 검사
	if( !pData )
		return FMOD_ERR_MEMORY_CANTPOINT;


	mode |= FMOD_OPENMEMORY;
	FMOD_RESULT result = CNtlSoundGlobal::m_pFMODSystem->createSound((const char*)pData, mode, &exinfo, &(pSound->m_pFMODSound) );

	NTL_ARRAY_DELETE( pData );

	return result;
}