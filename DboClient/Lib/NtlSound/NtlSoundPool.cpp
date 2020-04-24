#include "precomp_ntlsound.h"
#include "NtlSoundEvent.h"


RWS::CEventId g_EventSoundReleaseSound;				// 한 개 채널의 사운드가 해제되었다

RWS::CEventId g_EventSoundFinishFade;				// 페이드 인/아웃이 끝났다

RWS::CEventId g_EventSoundDSP;						// 효과음 관련 이벤트

RWS::CEventId g_EventSoundEventMusic;				// 이벤트음악 채널 이벤트

RWS::CEventId g_EventSoundRestTime;					// 반복되는 음악의 다음 플레이까지 휴식 시간

RWS::CEventId g_EventSoundKnockdown;				// 넉다운 될 때

RWS::CEventId g_EventSoundMinMaxRate;				// 3D 사운드의 전체 Min, Max값을 바꾼다