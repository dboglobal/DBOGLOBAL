#pragma once

#include <string>

// sound
#include "fmod/fmod.hpp"
#include "fmod/fmod_common.h"

//#define SOUND_DEBUG_LOG						///< 디버그용 사운드 관련 로그 남기기

class CNtlSoundDSP;


// 거리 ////////////////////////////////////////////////////////////////////////////
#define MIN_DISTANCE_BETWEEN_MINMAX		0.0001f	///< 사운드 연주 최대 거리가 최소 거리보다 기본적으로 더 커야하는 기본수치


// 시간 ////////////////////////////////////////////////////////////////////////////
#define SOUND_FADEINOUT_INTERVAL_TIME	100		///< Fade In/Out 시에 볼륨의 변화가 있는 시간 (단위 :milisecond)
#define DELAY_EFFECT_SOUND_TIME			500		///< 같은 이름의 이펙트 사운드간 플레이 지연 시간

#define dBGM_FADE_OUT_TIME				2000	///< 배경음이 사라지거나 교체될 때의 fade out 시간


// 사운드 핸들 ////////////////////////////////////////////////////////////////////////////
#define INVALID_SOUND_HANDLE				0xffffffff


// 채널 갯수 //////////////////////////////////////////////////////////////////////////
#define MAX_FMOD_CHANNELS				4093	///< Maximum channel (hardware + software) supported by FMOD
#define MAX_DBO_CHANNELS				512		///< Number of channels used in Dbo
#define MAX_EFFECT_CHANNELS				40		///< The number of all channels of the effect that can be played at maximum


// 수치 범위 //////////////////////////////////////////////////////////////////////////
#define dNTLSOUND_VOLUME_DEFAULT				1.0f
#define dNTLSOUND_VOLUME_BACKGROUND_MAX			0.5f
#define dNTLSOUND_VOLUME_EVENT_MAX				0.7f

#define dNTLSOUND_PITCH_DEFAULT					1.0f
#define dNTLSOUND_PITCH_MINIMUM					0.0f
#define dNTLSOUND_PITCH_MAXIMUM					10.0f

// etc
#define NTL_SOUND_MEMORY_POOL			10
#define MAX_SOUND_DUPLICATION			3		///< The maximum number of times a sound of the same name can be played simultaneously

#define d2D_SOUND_MODE		(FMOD_2D)
#define d3D_SOUND_MODE		(FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_3D_WORLDRELATIVE)



typedef unsigned int			SOUND_HANDLE;


enum eSoundResourceType
{
	SRT_CHANNEL,
	SRT_CHANNEL_GROUP,
};

enum eFMODPlayType
{
	FMODPLAYTYPE_3DMODE,						///< 3D 기본 사운드 모드
	FMODPLAYTYPE_3D_TOOLMODE,					///< 3D Tool 사운드 모드

	FMODPLAYTYPE_NUM
};

enum eSoundPlayState
{
	SPS_NONE,									///< 연주가 끝났거나 사운드 정보가 없다
	SPS_PLAYING,								///< 연주중
	SPS_PLAY_FADE_IN,							///< Fade in 중이다
	SPS_PLAY_FADE_OUT,							///< Fade Out 중이다
	SPS_SLEEP,									///< 연주가 중지되어 대기상태이다
};

enum eChannelGroupType
{
	CHANNEL_GROUP_FIRST = 0,

	CHANNEL_GROUP_UI_SOUND = CHANNEL_GROUP_FIRST,///< UI 효과음
	CHANNEL_GROUP_JINGLE_MUSIC,					///< (이벤트 발생시)퀘스트 획득, 퀘스트 완료 등의 짧은 음악
	CHANNEL_GROUP_FLASH_MUSIC,					///< 플래쉬 배경음
	CHANNEL_GROUP_BGM,							///< DBO 메인 배경음악
	CHANNEL_GROUP_AVATAR_VOICE_SOUND,			///< 아바타의 음성
	CHANNEL_GROUP_AVATAR_EFFECT_SOUND,			///< 아바타의 이펙트 사운드
	CHANNEL_GROUP_VOICE_SOUND,					///< 액터의 음성
	CHANNEL_GROUP_EFFECT_SOUND,					///< 이펙트 사운드
	CHANNEL_GROUP_OBJECT_MUSIC,					///< 오브젝트 뮤직, 자동차 소리 등(일정지역에서만 소리가 나며 멀어질수록 볼륨이 줄어든다)
	CHANNEL_GROUP_AMBIENT_MUSIC,				///< 광역 환경 소음(일정지역에서 동일한 볼륨으로 난다)
	CHANNEL_GROUP_WEATHER_EFFECT_SOUND,			///< 날씨효과음(날씨 관련 사운드가 잠시 플레이 된다)
	CHANNEL_GROUP_WEATHER_MUSIC,				///< 날씨음악(임의로 멈출 때까지 무한 반복이 된다)
	CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE,	///< CHANNEL_GROUP_JINGLE_MUSIC 와 같지만 BGM에 영향을 끼치지 않는다

	NUM_CHANNEL_GROUP,

	INVALID_CHANNEL_GROUP
};

enum eBGMType
{
	BGM_TYPE_SHARE_THEME,						///< DBO Common Theme
	BGM_TYPE_LOCAL_THEME,						///< Local theme song
	BGM_TYPE_MAIN_THEME,						///< 본 게임안이 아닌 곳에서 플레이 되는 BGM
	BGM_TYPE_RANGE,								///< 일정 범위 안에서 플레이되는 BGM
	BGM_TYPE_PVP,								///< 플레이어 1:1 대전 테마곡
	BGM_TYPE_RANKBATTLE_BATTLE,					///< 랭크배틀
	BGM_TYPE_THEME_BGM,							///< THEME BGM
	BGM_TYPE_SERVER_PLAYED,						///< 서버쪽 요청으로 Play하는 BGM
	BGM_TYPE_CINEMATIC,							///< 시네마틱 BGM	

	NUM_BGM_TYPE,

	INVALID_BGM_TYPE
};

enum eFadeInOutType
{	
	FADE_IN,									///< Fade In 적용
	FADE_OUT									///< Fade Out 적용
};

enum eStoreResult
{
	STORE_READY_TO_PLAY,						///< 바로 플레이 한다
	STORE_LIST,									///< 리스트상으로만 별도 관리한다
	STORE_FAIL,									///< 저장 실패
};

struct sNtlVolume
{
	///< 범위 : 0.0f ~ 1.0f
	float		fMainVolume;
	float		fFadeVolume;
	float		fWhenMoviePlayVolume;			///< 동영상이 플레이 될 때의 볼륨

	sNtlVolume()
	:fMainVolume(1.f)
	,fFadeVolume(1.f)
	,fWhenMoviePlayVolume(1.f) {}
};

struct sNtlPitch
{	
	float		fMainPitch;					///< 범위 : 0.0f ~ 10.f( default : 1.f )
	float		fPitchRate;					///< 범위 : 제한 없음
	float		fSlowMotionPitchRate;		///< 범위 : 제한 없음

	sNtlPitch() : fMainPitch(1.f), fPitchRate(1.f), fSlowMotionPitchRate(1.f) {}
};

struct sNtlMinMax
{
	float		fMin, fMax;

	sNtlMinMax() : fMin(1.f), fMax(1.f + MIN_DISTANCE_BETWEEN_MINMAX) {}
};

struct sNtlSoundPlayParameta
{
	// for 2D
	SOUND_HANDLE		hHandle;			///< Returns the handle of the created Channel.
	int					iChannelGroup;		
	char*				pcFileName;
	
	// extra parameta
	eBGMType			eBGM_Type;
	bool				bBGMReplay;
	bool				bLoop;
	float				fVolume;			///< 0.0 ~ 1.f
	float				fPitch;				///< 0.0 ~ 10.0f

	// for 3D
	float				fXPos;
	float				fYPos;
	float				fZPos;	
	float				fMinDistance;		///< Minimum distance at which volume is maximized
	float				fMaxDistance;		///< Maximum distance at which volume is minimum

	sNtlSoundPlayParameta()
	:hHandle(INVALID_SOUND_HANDLE)
	,iChannelGroup(INVALID_CHANNEL_GROUP)
	,pcFileName(NULL)
	,eBGM_Type(BGM_TYPE_LOCAL_THEME)
	,fXPos(0.f)
	,fYPos(0.f)
	,fZPos(0.f)
	,bBGMReplay(false)
	,bLoop(false)
	,fVolume(dNTLSOUND_VOLUME_DEFAULT)
	,fMinDistance(10.f)
	,fMaxDistance(50.f)
	,fPitch(dNTLSOUND_PITCH_DEFAULT)
	{ }
};

// Convert FMOD's left-hand coordinate system to the renderer's coordinate system
#define dCONVERT_COORDINATE_X(fX)		(-1.f * fX)