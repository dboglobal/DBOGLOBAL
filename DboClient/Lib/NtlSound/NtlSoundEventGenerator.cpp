#include "precomp_ntlsound.h"
#include "NtlSoundEventGenerator.h"

#include "NtlEventQueue.h"
#include "NtlSysEvent.h"
#include "NtlSoundEvent.h"
#include "NtlSound.h"
#include "NtlFadeInOut.h"
#include "NtlSoundManager.h"

void CNtlSoundEventGenerator::SendReleaseSound(CNtlSound* pSound)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	RWS::CMsg msg;
	msg.Id = g_EventSoundReleaseSound;
	msg.pData = reinterpret_cast<void*>( pSound );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendFinishFade(sFadeInOut& fadeInOut)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	RWS::CMsg msg;
	msg.Id = g_EventSoundFinishFade;
	msg.pData = reinterpret_cast<void*>( &fadeInOut );
	_SendMsg( msg );
}


void CNtlSoundEventGenerator::SendDSPEvent(RwUInt8 byType)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEventDSP data;
	data.byType = byType;

	RWS::CMsg msg;
	msg.Id = g_EventSoundDSP;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendEventMusic(RwUInt8 byType)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEeventMusic data;
	data.byType = byType;

	RWS::CMsg msg;
	msg.Id = g_EventSoundEventMusic;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendMusicRestItem(RwUInt8 byChannelGroup, RwReal fRestTime, RwUInt8 byBGMType /* = BGM_TYPE_LOCAL_THEME */)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEventMusicRestTime data;
	data.byChannelGroup		= byChannelGroup;
	data.fRestTime			= fRestTime;
	data.byBGMType			= byBGMType;

	RWS::CMsg msg;
	msg.Id = g_EventSoundRestTime;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendKnockdown(RwBool bKnockdown)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEventKnockdown data;
	data.bKnockdown			= bKnockdown;

	RWS::CMsg msg;
	msg.Id = g_EventSoundKnockdown;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendMinMaxRate(RwReal fMinMaxRate)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEventMinMaxRate data;
	data.fMinMaxRate			= fMinMaxRate;

	RWS::CMsg msg;
	msg.Id = g_EventSoundMinMaxRate;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendRangeBGM_AddList(SOUND_HANDLE* phResultHandle, const RwChar* pcFileName,
												  RwReal fRadius, RwV3d* pPosition)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEventAddListRangeBGM data;
	data.phResultHandle	= phResultHandle;
	data.pcFileName		= pcFileName;
	data.fRadius		= fRadius;
	data.pPosition		= pPosition;

	RWS::CMsg msg;
	msg.Id = g_EventSoundAddListRangeBGM;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendRangeBGM_DelList(SOUND_HANDLE hSound)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEventDelListRangeBGM data;
	data.hSound			= hSound;

	RWS::CMsg msg;
	msg.Id = g_EventSoundDelListRangeBGM;
	msg.pData = reinterpret_cast<void*>( &hSound );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendRangeBGM_ChangeParam(SOUND_HANDLE hSound, const RwChar* pcFileName,
													   RwReal fRadius, RwV3d* pPosition)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEventRangeBGMChangeParam data;
	data.hSound			= hSound;
	data.pcFileName		= pcFileName;
	data.fRadius		= fRadius;
	data.pPosition		= pPosition;

	RWS::CMsg msg;
	msg.Id = g_EventSoundChangeListRangeBGM;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendShareBGM_AddList(const RwChar* pcFileName)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	if( !pcFileName )
		return;

	SNtlEventAddListShareBGM data;
	data.pcFileName		= pcFileName;

	RWS::CMsg msg;
	msg.Id = g_EventSoundAddListShareBGM;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendShareBGM_DeleteAllList()
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	RWS::CMsg msg;
	msg.Id = g_EventSoundDeleteAllListShareBGM;
	msg.pData = NULL;
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendShareBGM_PlayRate(RwReal fRate)
{
	if( GetSoundManager()->IsUsableSound() == false )
		return;

	SNtlEventShareBGMPlayRate data;
	data.fPlayRate			= fRate;

	RWS::CMsg msg;
	msg.Id = g_EventSoundShareBGMPlayRate;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSoundEventGenerator::SendReleaseObjectGroup()
{
	if( GetSoundManager()->IsUsableSound() == FALSE )
		return;

	RWS::CMsg msg;
	msg.Id = g_EventSoundReleaseObjectGroup;
	msg.pData = NULL;
	_SendMsg( msg );
}