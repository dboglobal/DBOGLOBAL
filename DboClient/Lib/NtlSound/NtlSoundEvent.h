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
#include "NtlSoundDefines.h"

extern RWS::CEventId g_EventSoundReleaseSound;				// 한 개 채널의 사운드가 해제되었다

extern RWS::CEventId g_EventSoundFinishFade;				// 페이드 인/아웃이 끝났다

extern RWS::CEventId g_EventSoundDSP;						// 효과음 관련 이벤트

extern RWS::CEventId g_EventSoundEventMusic;				// 채널 이벤트

extern RWS::CEventId g_EventSoundRestTime;					// 반복되는 음악의 다음 플레이까지 휴식 시간

extern RWS::CEventId g_EventSoundKnockdown;					// 넉다운 될 때

extern RWS::CEventId g_EventSoundMinMaxRate;				// 3D 사운드의 전체 Min, Max값을 바꾼다

extern RWS::CEventId g_EventSoundAddListRangeBGM;			// Range BGM을 리스트에 추가

extern RWS::CEventId g_EventSoundDelListRangeBGM;			// Range BGM을 리스트에서 삭제

extern RWS::CEventId g_EventSoundChangeListRangeBGM;		// Range BGM의 값을 변경

extern RWS::CEventId g_EventSoundAddListShareBGM;			// Shard BGM을 리스트에 추가

extern RWS::CEventId g_EventSoundDeleteAllListShareBGM;		// Shard BGM을 리스트에서 모두 삭제

extern RWS::CEventId g_EventSoundShareBGMPlayRate;			// Shard BGM을 플레이 할 수 있는 확률 조절

extern RWS::CEventId g_EventSoundReleaseObjectGroup;		// Object Group의 모든 사운드를 해제한다

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
	EVENT_MUSIC_EVENT_MUSIC_HAD_PLAY,			///< Event 음악이 플레이를 시작했다
	EVENT_MUSIC_ALL_EVENT_MUSIC_FINISH,			///< 모든 Event 음악이 플레이를 종료했다

	EVENT_MUSIC_LEAVE_MAIN_WORLD_CONCEPT,		///< 메인월드 컨셉에서 다른 컨셉으로 변경이 되었다

	EVENT_MUSIC_START_PVP_BGM,					///< PVP BGM 시작
	EVENT_MUSIC_END_PVP_BGM,					///< PVP BGM 끝

	EVENT_MUSIC_START_PRIVATE_RANKBATTLE_BGM,	///< 랭크배틀 개인전 BGM 시작
	EVENT_MUSIC_END_PRIVATE_RANKBATTLE_BGM,		///< 랭크배틀 개인전 끝

	EVENT_MUSIC_START_PARTY_RANKBATTLE_BGM,		///< 랭크배틀 파티전 BGM 시작
	EVENT_MUSIC_END_PARTY_RANKBATTLE_BGM,		///< 랭크배틀 파티전 끝

	EVENT_MUSIC_END_CINEMATIC_BGM,				///< 시네마틱 BGM 끝

	EVENT_MUSIC_START_MAIN_THEME,				///< 메인테마 BGM 시작
	EVENT_MUSIC_PROLOG_THMEM_BGM_PLAY,			///< Prolog BGM Play
	EVENT_MUSIC_END_MAIN_THEME,					///< 메인테마 끝
	EVENT_MUSIC_IMMEDIATELY_END_MAIN_THEME,		///< 메인테마 FadeOut 없이 바로 종료	

	EVENT_MUSIC_VOLUME_DOWN_BECAUSE_MOVIE_PLAY,	///< Movie를 플레이하기에 Master 볼륨을 줄인다
	EVENT_MUSIC_VOLUME_UP_BECAUSE_MOVIE_PLAY,	///< Movie가 플레이 종료되어 Master 볼륨을 높인다

	EVENT_MUSIC_FINISH_THEME_BGM,				///< THEME BGM이 플레이 중이면 종료하라	

	EVENT_MUSIC_LOOP_THEME_BGM,					///< THEME BGM를 Loop 한다
	EVENT_MUSIC_NOT_LOOP_THEME_BGM,				///< THEME BGM를 한 번만 플레이 한다

	EVENT_MUSIC_TEMP_FINISH_TMQ,				///< 임시 TMQ 종료

	EVENT_MUSIC_TB_DIRECTION_BGM_PLAY,			///< 천하제일 무도회 연출 BGM Play
	EVENT_MUSIC_TB_DIRECTION_BGM_STOP,			///< 천하제일 무도회 연출 BGM Stop

	EVENT_MUSIC_TB_PARTY_BGM_PLAY,				///< 천하제일 무도회 파티전 BGM Play
	EVENT_MUSIC_TB_PARTY_BGM_STOP,				///< 천하제일 무도회 파티전 BGM Stop

	EVENT_MUSIC_TB_PRIVATE_BGM_PLAY,			///< 천하제일 무도회 개인전 BGM Play
	EVENT_MUSIC_TB_PRIVATE_BGM_STOP,			///< 천하제일 무도회 개인전 BGM Stop

	EVENT_MUSIC_RAGNE_STOP,						///< Range BGM Stop
	EVENT_MUSIC_REMOVE_RAGNE_BGM,				///< BGM 그룹에서 Range BGM 리스트를 지웠다

	EVENT_MUSIC_START_SCRAMBLE_READY_BGM,		///< 도장 쟁탈전
	EVENT_MUSIC_STOP_SCRAMBLE_READY_BGM,
	EVENT_MUSIC_START_SCRAMBLE_START_BGM,
	EVENT_MUSIC_END_SCRAMBLE_START_BGM,
};

struct SNtlEeventMusic
{
	RwUInt8			byType;
};

struct SNtlEventMusicRestTime
{
	RwUInt8			byChannelGroup;
	RwReal			fRestTime;				///< 단위 : 초
	RwUInt8			byBGMType;				///< BGM 채널 그룹일 때만 유효하다
};

struct SNtlEventKnockdown
{
	RwBool			bKnockdown;
};

struct SNtlEventMinMaxRate
{
	RwReal			fMinMaxRate;
};

struct SNtlEventAddListRangeBGM
{
	RwUInt32*		phResultHandle;
	const RwChar*	pcFileName;
	RwReal			fRadius;
	RwV3d*			pPosition;
};

struct SNtlEventDelListRangeBGM
{
	RwUInt32		hSound;
};

struct SNtlEventRangeBGMChangeParam
{
	RwUInt32		hSound;
	const RwChar*	pcFileName;
	RwReal			fRadius;
	RwV3d*			pPosition;
};

struct SNtlEventAddListShareBGM
{
	const RwChar*	pcFileName;
};

struct SNtlEventShareBGMPlayRate
{
	RwReal			fPlayRate;
};