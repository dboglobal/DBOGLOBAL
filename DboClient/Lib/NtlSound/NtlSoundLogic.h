#pragma once

#include <stdio.h>
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#include "NtlSound.h"

class CNtlSound;

struct sNtlVolume;
struct sNtlPitch;
struct sNtlSoundPlayParameta;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load from memory
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void (*CallbackLoadSoundFile)(const RwChar* pFileName, void** pData, int* iSize);
extern CallbackLoadSoundFile g_fnCallback_LoadSound_from_Memory;

void LinkLoadSound_from_Memory(CallbackLoadSoundFile fn);
void UnLinkLoadSound_from_Memory();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IsExistSoundFile
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef bool (*CallbackIsExistSoundFile)(const RwChar* pFileName);
extern CallbackIsExistSoundFile g_fnCallback_IsExistSoundFile;

void Link_IsExistSoundFile(CallbackIsExistSoundFile fn);
void UnLink_IsExistSoundFile();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// log
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID Logic_NtlSoundLog(const RwChar* pcContition, const RwChar* pcSoundFileName = NULL);
VOID Logic_NtlSoundLog(const RwChar* pcContition, RwInt32 iChannelGroup, const RwChar* pcSoundFileName = NULL);
VOID Logic_NtlSoundLog(const RwChar* pcContition, FMOD_RESULT result, const RwChar* pcSoundFileName = NULL);

RwBool Logic_IsExistLog(const RwChar* pcText);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sound operation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FMOD에서 지원하는 볼륨으로 변환한다
RwReal Logic_GetFMODValidVolume(RwReal fVolume);

// 실제 FMOD에서 쓰이는 볼륨을 결정한다
RwReal Logic_CalcPlayVolume(sNtlVolume* pNtlVolume);

// FMOD에서 지원하는 Pitch으로 변환한다
RwReal Logic_GetFMODValidPitch(RwReal fPitch);

// 실제 FMOD에서 쓰이는 Pitch를 결정한다
RwReal Logic_CalcPlayPitch(sNtlPitch* pNtlPitch);

RwBool Logic_IsExistFile(const RwChar* pcFileName);

SOUND_HANDLE Logic_GetNewSoundHandle();

FMOD_RESULT API_Create_Stream(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta, FMOD_MODE mode, std::string& strFullName);
FMOD_RESULT API_Create_Sound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta, FMOD_MODE mode, std::string& strFullName);