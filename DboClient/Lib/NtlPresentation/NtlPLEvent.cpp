#include "precomp_ntlpresentation.h"
#include "NtlPLEvent.h"


//Animation Event
RWS::CEventId g_EventAnimEnd;			//Animation이 끝났을 경우
RWS::CEventId g_EventAnimHit;           //Animation Attack Event
RWS::CEventId g_EventAnimFootStep;      //Running 중 Character 발바닥이 땅에 닿을때
RWS::CEventId g_EventWeightTime;		//Animation WeightTimeEvent
RWS::CEventId g_EventSubWeaponActive;	 //sub weapon을 active 시킨다 ( 예 : 등에 차고 있는 칼을 뽑는다. )
RWS::CEventId g_EventSubWeaponDeActive;	 //sub weapon을 deactive 시킨다 ( 예 : 뽑았던 칼을 다시 등에 찬다. )
RWS::CEventId g_EventSummon;			 //summmon active 시킨다.( 소환수가 소환물을 소환시키는 timing event. )
RWS::CEventId g_EventAlpha;               // Alpha Fade용 이벤트
RWS::CEventId g_EventAnimTMQ;            // TMQ 연출용 이벤트
RWS::CEventId g_EventExplosion;           // 폭발 이벤트
RWS::CEventId g_EventAnimPostEffect;      // Post Effect Event
RWS::CEventId g_EventAnimDirect;          // 연출 이벤트
RWS::CEventId g_EventAnimColorChange;     // 색상변경 이벤트
RWS::CEventId g_EventAnimStretch;         // 팔늘리기 이벤트
RWS::CEventId g_EventAnimTrigger;         // 트리거 이벤트
RWS::CEventId g_EventAnimSkillCancel;	  // anim cancel point	


// world field switching effects
RWS::CEventId NPEI_IS_ANOTHER_FIELD_CHANGED;
RWS::CEventId NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY;

RWS::CEventId NPEI_IS_ANOTHER_BLOCK_CHANGED;
RWS::CEventId NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY;

RWS::CEventId NPEI_IS_ANOTHER_OBJECT_CHANGED;
RWS::CEventId NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY;

RWS::CEventId NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY;
RWS::CEventId NPEI_IS_MAPNAME_CHANGE;


// trigger
RWS::CEventId g_EventTriDoodads;	// 트리거 맵오브젝들이 로딩 또는 삭제될때


// Event Object
RWS::CEventId g_EventCreateEventObject; ///< 이벤트 오브젝트가 로딩 또는 삭제 될때
RWS::CEventId g_EventThreadLoadingComplete; // 엔티티의 스레드 로딩이 끝났음을 통지하는 이벤트


// gui event
RWS::CEventId g_EventResize;					// Resize client window

RWS::CEventId g_EventCinematicViewShow;	
RWS::CEventId g_EventCinematicBalloon;
RWS::CEventId g_EventCinematicFlash;
RWS::CEventId g_EventCinematicEcho;
RWS::CEventId g_EventResetCinematic;			// Cinematic Action을 Reset한다. GUI에서만 호출


RWS::CEventId g_EventCinematicFadeOut;         // 시네마틱 연출 시작
RWS::CEventId g_EventCinematicFadeIn;          // 시네마틱 연출 종료

RWS::CEventId g_EventCinematicBreak;			// 시네마틱 연출 break

RWS::CEventId g_EventShowProfilerGui;			// ProfilerGUI Open/Close

RWS::CEventId g_EventShowSpectatorBalloon;
RWS::CEventId g_EventHideAllSpectatorBalloon;

RWS::CEventId g_EventShowPlayerName;			// PlayerName Entity가 Show/Hide

RWS::CEventId g_EventDirectPlayCancel;
