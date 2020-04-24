/*****************************************************************************
 *
 * File			: DboEvent.h
 * Author		: Hong Sungbock
 * Copyright	: (주)NTL
 * Date			: 2007. 5. 29
 * Abstract		: Ntl sound event.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

#include "ceventhandler.h"

extern RWS::CEventId g_EventSoundReleaseSound;				// 한 개 채널의 사운드가 해제되었다

extern RWS::CEventId g_EventSoundFinishFade;				// 페이드 인/아웃이 끝났다

extern RWS::CEventId g_EventSoundDSP;						// 효과음 관련 이벤트

extern RWS::CEventId g_EventSoundEventMusic;				// 이벤트음악 채널 이벤트

extern RWS::CEventId g_EventSoundRestTime;					// 반복되는 음악의 다음 플레이까지 휴식 시간

extern RWS::CEventId g_EventSoundKnockdown;					// 넉다운 될 때

extern RWS::CEventId g_EventSoundMinMaxRate;				// 3D 사운드의 전체 Min, Max값을 바꾼다

//////////////////////////////////////////////////////////////////////////
//	Structure
//////////////////////////////////////////////////////////////////////////

enum eDSPEvent
{
	DSP_ADD_REVERV,							///< 특정 채널그룹에 reverv 효과를 적용한다
	DSP_REMOVE_REVERV,						///< 특정 채널그룹에 reverv 효과를 해제힌다
	DSP_REMOVE_ALL,							///< 특정 채널그룹에 모든 효과를 해제한다
};

struct SNtlEventDSP
{
	RwUInt8			byType;
};

enum eEventMusic
{
	EVENT_MUSIC_EVENT_MUSIC_HAD_PLAY,		///< Event 음악이 플레이를 시작했다
	EVENT_MUSIC_ALL_EVENT_MUSIC_FINISH,		///< 모든 Event 음악이 플레이를 종료했다
};

struct SNtlEeventMusic
{
	RwUInt8			byType;
};

struct SNtlEventMusicRestTime
{
	RwUInt8			byChannelGroup;
	RwReal			fRestTime;				///< 단위 : 초
};

struct SNtlEventKnockdown
{
	RwBool			bKnockdown;
};

struct SNtlEventMinMaxRate
{
	RwReal			fMinMaxRate;
};