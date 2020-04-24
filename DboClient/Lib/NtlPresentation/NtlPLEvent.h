/*****************************************************************************
 *
 * File			: NtlPLEvent.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 29	
 * Abstract		: presentation layer event type
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLEVENT_H__
#define __NTL_PLEVENT_H__

#include "ceventhandler.h"

//AnimationEvent 관련
extern RWS::CEventId g_EventAnimEnd;			 //Animation이 끝났을 경우
extern RWS::CEventId g_EventAnimHit;             //Animation Hit Event
extern RWS::CEventId g_EventAnimFootStep;		 //Animation 발바닥 땅에 닿을때
extern RWS::CEventId g_EventWeightTime;			 //Animaiton Weight Event
extern RWS::CEventId g_EventSubWeaponActive;	 //sub weapon을 active 시킨다 ( 예 : 등에 차고 있는 칼을 뽑는다. )
extern RWS::CEventId g_EventSubWeaponDeActive;	 //sub weapon을 deactive 시킨다 ( 예 : 뽑았던 칼을 다시 등에 찬다. )
extern RWS::CEventId g_EventSummon;				 //summmon active 시킨다.( 소환수가 소환물을 소환시키는 timing event. )
extern RWS::CEventId g_EventAlpha;               // Alpha Fade용 이벤트
extern RWS::CEventId g_EventAnimTMQ;             // TMQ 연출용 이벤트
extern RWS::CEventId g_EventExplosion;           // 폭발 이벤트
extern RWS::CEventId g_EventAnimPostEffect;      // Post Effect Event
extern RWS::CEventId g_EventAnimDirect;          // 연출용 이벤트
extern RWS::CEventId g_EventAnimColorChange;     // 색상변경 이벤트
extern RWS::CEventId g_EventAnimStretch;         // 본이 늘어나는 이벤트
extern RWS::CEventId g_EventAnimTrigger;         // 다양하게 사용되는 트리거 이벤트
extern RWS::CEventId g_EventAnimSkillCancel;	 // 스킬animation 도중 animatin이 cancel이 가능한 point.

struct SNtlEventAnimEnd
{
	RwUInt32 uiSerialId;
	RwUInt32 uiBaseAnimKey;
	RwUInt32 uiUpperAnimKey;
};

struct SNtlEventAnimHit
{
	RwUInt32	uiSerialId;
	void		*pData;							//SEventAnimHit(Character\Base  NtlAnimEventData.h)
};


struct SNtlEventFootStep
{
	RwUInt32 uiSerialId;                        ///< 캐릭터 객체의 시리얼 ID
    void*    pData;                             // SEventFootStep

    //RwBool   bLeftFoot;                         ///< 발 종류 (True : 왼쪽, False : 오른쪽)
};

struct SNtlEventWeightTime						//SEventWeightTime(Character\Base NtlAnimEventData.h)
{
	RwUInt32	uiSerialId;
	void		*pData;
};

struct SNtlEventSubWeaponActive					//sub weapon을 active 시킨다 ( 예 : 등에 차고 있는 칼을 뽑는다. )
{
	RwUInt32 uiSerialId;
};

struct SNtlEventSubWeaponDeActive				//sub weapon을 deactive 시킨다 ( 예 : 뽑았던 칼을 다시 등에 찬다. )
{
	RwUInt32 uiSerialId;
};

struct SNtlEventSummon							//summmon active 시킨다.( 소환수가 소환물을 소환시키는 timing event. )
{
	RwUInt32 uiSerialId;
};

struct SNtlEventAlpha                           /// Alpha Fade Event
{
    RwUInt32 uiSerialId;
    void*    pData;                             ///< Alpha Fade Event 객체의 포인터
};

struct SNtlPLEventTMQ
{
    RwUInt32    uiSeiralID;                         ///< 이벤트를 보낸 타임머신 오브젝트의 시리얼 ID
    RwBool      bIn;                                ///< TMQ 진입 여부 (True면 진입/False면 아웃)
};

struct SNtlPlEventExplosion
{
    RwUInt32    uiSerialID;                     ///< 오브젝트의 시리얼 ID
    void*       pData;                          ///< Explosion Event 구조체의 포인터
};

struct SNtlEventPostEffect
{
    RwUInt32    uiSerialID;                     ///< 이벤트를 발생하는 아바타의 ID
    void*       pData;                          ///< Post Effect Event 구조체의 포인터
};

/// 연출용 이벤트
struct SNtlEventDirect
{
    RwUInt32    uiSerialID;                     ///< 이벤트를 발생하는 객체의 ID
    void*       pData;                          ///< SEventDirect 구조체의 포인터
};

/// 컬러 변경 이벤트
struct SNtlEventColorChange
{
    RwUInt32    uiSerialID;
    void*       pData;                          ///< SEventColorChange 구조체의 포인터
};

/// 본늘리기 이벤트
struct SNtlEventStretch
{
    RwUInt32    uiSerialID;
    void*       pData;                          ///< SEventStretch 구조체의 포인터
};

struct SNtlEventTrigger
{
    RwUInt32    uiSerialID;
    void*       pData;                          ///< SEventTrigger 구조체의 포인터
};

struct SNtlEventAnimSkillCancel
{
	RwUInt32    uiSerialID;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// world field switching effects
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern RWS::CEventId NPEI_IS_ANOTHER_FIELD_CHANGED;
extern RWS::CEventId NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY;

extern RWS::CEventId NPEI_IS_ANOTHER_BLOCK_CHANGED;
extern RWS::CEventId NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY;

extern RWS::CEventId NPEI_IS_ANOTHER_OBJECT_CHANGED;
extern RWS::CEventId NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY;

extern RWS::CEventId NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY;
extern RWS::CEventId NPEI_IS_MAPNAME_CHANGE;

// struct sNTL_EVENT_FOG
// {
// 	void *pData;
// };
// 
// struct sNTL_EVENT_VARIATION
// {
// 	void *pData;
// };
// 
struct sNTL_EVENT_MAPNAME
{
 	void *pData;
};
// 
// struct sNTL_EVENT_SECTOR_CHANGED
// {
// 	void *pData;
// };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// trigger
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventTriDoodads;

struct sNTL_EVENT_TRI_DOODADS
{
	// 0 : Create, 1 : Delete
	RwBool				FlgTriDoodads;
	std::vector<void*>	vecDat;
};

extern RWS::CEventId g_EventCreateEventObject;
extern RWS::CEventId g_EventThreadLoadingComplete; // 엔티티의 스레드 로딩이 끝났음을 통지하는 이벤트

/// EVENT Object
struct SNtlPLEventEventObject
{
    RwBool          bCreate;        // TRUE : Create, FALSE : Delete
    void*           pData;          // CNtlPlObject Entity
};

struct SNtlPlThreadLoadingComplete
{
    RwUInt32    uiSerialID;                     ///< 엔티티의 시리얼 ID
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gui Event
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////
/**
* Resolution Resize
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventResize;						// Resize client window

struct SNtlPLEventResize
{
	RwInt32 iWidht;
	RwInt32 iHeight;
	RwBool bRestore_from_Minimize;
};

//////////////////////////////////////////////
/**
* Cinematic view show / hide
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCinematicViewShow;
extern RWS::CEventId g_EventCinematicBalloon;
extern RWS::CEventId g_EventCinematicFlash;
extern RWS::CEventId g_EventCinematicEcho;
extern RWS::CEventId g_EventResetCinematic;				// Cinematic Action을 Reset한다. GUI에서만 호출


//////////////////////////////////////////////
/**
* Cinematic view show / hide
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCinematicFadeOut;           // 시네마틱 연출 시작
extern RWS::CEventId g_EventCinematicFadeIn;	        // 시네마틱 연출 종료

//////////////////////////////////////////////
/**
* Cinematic Break
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCinematicBreak;


//////////////////////////////////////////////
/**
* ProfilerGui show / hide
*/
//////////////////////////////////////////////
extern RWS::CEventId g_EventShowProfilerGui;

//////////////////////////////////////////////
/**
* Chat Balloon
*/
//////////////////////////////////////////////
extern RWS::CEventId g_EventShowSpectatorBalloon;		
extern RWS::CEventId g_EventHideAllSpectatorBalloon;	

//////////////////////////////////////////////
/**
* Player 관련
*/
//////////////////////////////////////////////
extern RWS::CEventId g_EventShowPlayerName;                     ///< Name Show/Hide

extern RWS::CEventId g_EventDirectPlayCancel;



struct SNtlEventCinematicViewShow
{
	RwBool			bShow;
};

struct SNtlEventCinematicBalloon
{
	enum CINEMATICBALLOONPLAY
	{
		PLAY_BEGIN,
		PLAY_PROGRESS
	};

	RwBool			bShow;
	RwUInt32		hSerialId;
	RwUInt32		uiQuestTextTblId;
	RwUInt8			byBalloonType;
	RwUInt8			byEmotionType;
	RwUInt8			byPlayType;
};

struct SNtlEventCinematicFlash
{
	// Flash 
	enum PLAYTIMETYPEFLAG 
	{
		FLAG_BEGIN	= 0x01,
		FLAG_PLAY	= 0x02,
		FLAG_END	= 0x04,

		BEGIN_PLAY_END	= FLAG_BEGIN | FLAG_PLAY | FLAG_END,	// begin + play + end time
		BEGIN_PLAY		= FLAG_BEGIN | FLAG_PLAY,				// begin + play time
		PLAY_END		= FLAG_PLAY | FLAG_END,					// play + end time
		PLAY			= FLAG_PLAY								// play time
	};

	RwBool			bShow;
	RwChar*			pFilename;
	RwBool			bUseTime;							// True : 시간제 False : 1회연출
	RwReal			fLifeTime;
	void*			pEchoData;
	RwReal			fBeginAlphaTime;
	RwReal			fEndAlphaTime;		
	RwUInt32		flagPlayTimeType;										
};

struct SNtlEventCinematicEcho
{
	void*			hCinematic;
};

struct SNtlEventResetCinematic
{
	enum TYPE { CINEMATIC, QUESTNARRATION, DBCNARRATION };

	RwUInt32	eDemandResetObject;
};

enum ECinematicFadeInOutType
{
	CINEMATIC_FADEINOUT_SCREEN_PANEL,
	CINEMATIC_FADEINOUT_SHUTDOWN_TV,

	CINEMATIC_INVALID
};

struct SNtlEventCinematicFadeInOut
{
	RwUInt8		byType;
	RwReal		fLifeTime;		// 보간 시간.
	RwUInt8		byRed;
	RwUInt8		byGreen;
	RwUInt8		byBlue;
	RwUInt8		byEndAlpha;
};

struct SNtlEventShowProfilerGui
{
	RwBool		bShow;
};

struct SNtlEventShowSpectatorBalloon
{
	RwV3d		vPos;
	WCHAR*		szMessage;
	RwReal		fLifeTime;
	RwReal		fFadeInTime;
	RwUInt8		byBalloonType;
};

struct SNtlEventShowPlayerName
{
	RwUInt32	hSerialId;
	RwBool		bShow;
};

struct SNtlEventCharCreateComplete
{
    RwUInt32    hSerialId;
};


#endif