#include "precomp_ntlsound.h"
#include "NtlSoundEvent.h"


RWS::CEventId g_EventSoundReleaseSound;				// 한 개 채널의 사운드가 해제되었다

RWS::CEventId g_EventSoundFinishFade;				// 페이드 인/아웃이 끝났다

RWS::CEventId g_EventSoundDSP;						// 효과음 관련 이벤트

RWS::CEventId g_EventSoundEventMusic;				// 채널 이벤트

RWS::CEventId g_EventSoundRestTime;					// 반복되는 음악의 다음 플레이까지 휴식 시간

RWS::CEventId g_EventSoundKnockdown;				// 넉다운 될 때

RWS::CEventId g_EventSoundMinMaxRate;				// 3D 사운드의 전체 Min, Max값을 바꾼다

RWS::CEventId g_EventSoundAddListRangeBGM;			// Range BGM을 리스트에 추가

RWS::CEventId g_EventSoundDelListRangeBGM;			// Range BGM을 리스트에서 삭제

RWS::CEventId g_EventSoundChangeListRangeBGM;		// Range BGM의 값을 변경

RWS::CEventId g_EventSoundAddListShareBGM;			// Shard BGM을 리스트에 추가

RWS::CEventId g_EventSoundDeleteAllListShareBGM;	// Shard BGM을 리스트에서 모두 삭제

RWS::CEventId g_EventSoundShareBGMPlayRate;			// Shard BGM을 플레이 할 수 있는 확률 조절

RWS::CEventId g_EventSoundReleaseObjectGroup;		// Turn off all sounds in the Object Group