#pragma once

enum eSoundResultType
{	
	SOUNDRESULT_OK,								// 사운드 생성/혹은 List로 보관하기를 성공했다.

	SOUNDRESULT_RECOVERY_FROM_FADE_OUT,			///< Fade out 되어 사라져가는 사운드가 Fade in되어 계속 플레이 된다

	SOUNDRESULT_NOT_CREATED_SOUND,				///< FMOD::Sound 생성 실패

	SOUNDRESULT_FAIL_PLAY,						///< FMOD 에서 playSound 함수를 실패했다

	SOUNDRESULT_INVALID_CHANNELGROUP,			///< eChannelGroupType에 없는 type

	SOUNDRESULT_INVALID_HANDLE_PTR,				///< 잘못된 핸들값이다.

	SOUNDRESULT_OUT_OF_RANGE,					///< 연주 가능한 지역을 벗어낫다.

	SOUNDRESULT_FULL_DUPLICATION,				///< 동시에 연주할 수 있는 같은 이름의 사운드의 횟수를 초과하였다.(같은 사운드 플레이 가능)

	SOUNDRESULT_ALREADY_PLAY_SAME_SOUND,		///< 같은 이름의 사운드가 이미 연주되고 있다.(같은 사운드 플레이 불가)	

	SOUNDRESULT_EMPTY_FILENAME,					///< 플레이하려는 파일 이름이 빠져있다

	SOUNDRESULT_MORE_WAIT_EFFECT_SOUND,			///< 같은 이름의 이펙트 파일이 아직 충분한 시간동안 플레이 되지 않았다

	SOUNDRESULT_MAX_EFFECT_CHANNELS,			///< 지정된 이펙트 채널을 모두 쓰고 있다

	SOUNDRESULT_NOT_EXIST_FILE,					///< 사운드 파일이 존재하지 않는다

	SOUNDRESULT_PROHIBITION_STATE,				///< Mute 기능이 아니라 아예 플레이 하지 않고 있는 상태이다

	SOUNDRESULT_DISABLE_FLASH_MUSIC,			///< 플래쉬 음악그룹은 현재 사용하지 않기로 하였다

	SOUNDRESULT_FAILT_STORE_SOUND,				///< 사운드 내용을 저장하는 곳에서 실패했다

	SOUNDRESULT_NUM
};