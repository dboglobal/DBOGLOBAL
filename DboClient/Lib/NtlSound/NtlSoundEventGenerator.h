/*****************************************************************************
 *
 * File			: NtlSoundEventGenerator.h
 * Author		: Hong Sungbock
 * Copyright	: (аж)NTL
 * Date			: 2007. 5. 29
 * Abstract		: Ntl sound event generator
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

#include "NtlSoundEvent.h"
#include "NtlSoundDefines.h"

class CNtlSound;
struct sFadeInOut;

class CNtlSoundEventGenerator
{
public:
	static void SendReleaseSound(CNtlSound* pSound);

	static void SendFinishFade(sFadeInOut& fadeInOut);

	static void SendDSPEvent(RwUInt8 byType);

	static void SendEventMusic(RwUInt8 byType);

	static void SendMusicRestItem(RwUInt8 byChannelGroup, RwReal fRestTime, RwUInt8 byBGMType = BGM_TYPE_LOCAL_THEME);

	static void SendKnockdown(RwBool bKnockdown);

	static void SendMinMaxRate(RwReal fMinMaxRate);

	static void SendRangeBGM_AddList(SOUND_HANDLE* phResultHandle, const RwChar* pcFileName, RwReal fRadius, RwV3d* pPosition);

	static void SendRangeBGM_DelList(SOUND_HANDLE hSound);

	static void SendRangeBGM_ChangeParam(SOUND_HANDLE hSound, const RwChar* pcFileName, RwReal fRadius, RwV3d* pPosition);

	static void SendShareBGM_AddList(const RwChar* pcFileName);

	static void SendShareBGM_DeleteAllList();

	static void SendShareBGM_PlayRate(RwReal fRate);

	static void SendReleaseObjectGroup();
};