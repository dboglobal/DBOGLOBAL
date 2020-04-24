/*****************************************************************************
 *
 * File			: NtlWorldConceptDefine.h
 * Author		: Hong Sungbock
 * Copyright	: (주)NTL
 * Date			: 2007. 7. 13
 * Abstract		: Simulation world concept defines
 *****************************************************************************
 * Desc         : World Concept에 관련된 사항을 정의해 둔다
 *
 *****************************************************************************/


#ifndef __NTL_WORLDCONCEPT_DEFINE_H__
#define __NTL_WORLDCONCEPT_DEFINE_H__

#define WORLD_CONCEPT_FIRST_GRADE	0x00010000
#define WORLD_CONCEPT_SECOND_GRADE	0x00020000
#define WORLD_CONCEPT_THIRD_GRADE	0x00040000
#define WORLD_CONCEPT_FOURTH_GRADE	0x00080000

/// World Concept 클래스에서 제어하는 상태의 종류
// avooo's comment : Logic_WorldConceptMessage 에 인덱스를 추가해 주세요
enum EWorldPlayConcept
{
	WORLD_PLAY_NONE					= 0,

	//-- 1단계 컨셉--//
	WORLD_PLAY_TUTORIAL				= 0x00010001,		///< 튜터리얼
	WORLD_PLAY_TIME_MACHINE			= 0x00010002,		///< 타임머신 퀘스트
	WORLD_PLAY_RANK_BATTLE			= 0x00010003,		///< 랭크 배틀
	WORLD_PLAY_T_BUDOKAI			= 0x00010004,		///< 천하제일 무도회(배틀상태)
	WORLD_PLAY_PARTY_DUNGEON		= 0x00010005,		///< Party Dungeon
	WORLD_PLAY_TLQ					= 0x00010006,		///< 타임 립 퀘스트 ( TLQ )
	WORLD_PLAY_DOJO_SCRAMBLE		= 0x00010007,		///< 도장 쟁탈전
	WORLD_PLAY_DOJO_DUEL			= 0x00010008,		///< 도장 비무전
	WORLD_PLAY_CCBD					= 0x00010009,
	WORLD_PLAY_DWC					= 0x00010010,

	//-- 2단계 컨셉 --//
	WORLD_PLAY_FREEPVP				= 0x00020000,		///< PVP(Battle) state
	WORLD_PLAY_NPC_COMMU			= 0x00020001,		///< NPC 대화 상태
	WORLD_PLAY_TRADE				= 0x00020002,		///< 유저 Trade 상태
	WORLD_PLAY_DRAGONBALL_COLLECT	= 0x00020003,		///< 드래곤볼을 수집하여, 용신을 불러내는 이벤트 상태
	WORLD_PLAY_PRIVATESHOP			= 0x00020004,		///< 개인상점 상태(판매자)
	WORLD_PLAY_PRIVATESHOP_VISITOR	= 0x00020005,		///< 개인상점 상태(구매자)
	WORLD_PLAY_REMOTE_BEHAVIOUR		= 0x00020006,
	WORLD_PLAY_MASCOTEX_SKILL		= 0x00020007,
	
	//-- 3단계 컨셉 --//
	WORLD_PLAY_DIRECT				= 0x00040006,		///< 연출 상태(cinematic, narraion)

	WORLD_PLAY_DRAGONBALL_SCRAMBLE	= 0x00080001,

	WORLD_PLAY_FREEPVP_ZONE			= 0x00100001,	// free pvp for all
};


/// World Concept의 전이 상태 플래그
enum EWorldState
{
	WORLD_STATE_NONE,
	WORLD_STATE_ENTER,
	WORLD_STATE_IDLE,
	WORLD_STATE_EXIT,

	// 드래곤볼 컬렉션 관련 상태
	WORLD_DBC_ALTAR_UI,
	WORLD_DBC_NIGHT_ON,
	WORLD_DBC_ALTAR_EFFECT,
	WORLD_DBC_SPAWN_DRAGON,
	WORLD_DBC_NARRATION_START,
	WORLD_DBC_REWARD_UI,
	WORLD_DBC_NARRATION_END,
	WORLD_DBC_DESTROY_DRAGON,
	WORLD_DBC_DESTROY_DB,
	WORLD_DBC_NIGHT_OFF,
};

enum EWorldStateNPC
{
	WORLD_NPC_SHOP = WORLD_STATE_EXIT + 1,
	WORLD_NPC_TRAINER,
	WORLD_NPC_ITEM_UPGRADE,
	WORLD_NPC_WAREHOUSE,
	WORLD_NPC_GUILD_WAREHOUSE,
};

/// 아바타 액션 가능 유무를 판단하기 위한 플래그 리스트
enum EAvatarAction
{
	E_ACTION_MOVE,
	E_ACTION_PICK_WORLD,
	E_ACTION_PICK_OBJECT,
	E_ACTION_TARGETTING,
	E_ACTION_ATTACK,
	E_ACTION_CHANGE_HEADING,
	E_ACTION_CAMERA_CONTROL,
	E_ACTION_CHARGING,
	E_ACTION_LOOTING,
	E_ACTION_CAN_COMMUNICATION,
};

enum EWorldStateTMQ
{
	WORLD_STATE_TMQ_WAIT,					// TMQ 생성하고 플레이어 진입 대기
	WORLD_STATE_TMQ_TERRAIN_READY,			// 지형 로딩 완료
	WORLD_STATE_TMQ_PREPARE,				// 모든 플레이어 진입후 컨디션 체크 후 시작 여부 결정
	WORLD_STATE_TMQ_ARRIVE,					// 연출 상태 ( TS부터의 이벤트 대기, 유저 나타나는 시간 이벤트 대기 )
	WORLD_STATE_TMQ_BEGIN,					// 게임 시작 ( 스크립트 제어 )
	WORLD_STATE_TMQ_STAGE_READY,			// 스테이지 준비 ( 스크립트 제어 )
	WORLD_STATE_TMQ_STAGE_START,			// 스테이지 시작 ( 스크립트 제어 )
	WORLD_STATE_TMQ_STAGE_FINISH,			// 스테이지 종료 ( 스크립트 제어 )
	WORLD_STATE_TMQ_END,					// 게임 끝
	WORLD_STATE_TMQ_LEAVE,					// 게임 나가기 상태
	WORLD_STATE_TMQ_FAIL,					// 게임 끝 ( TMQ 실패 )
	WORLD_STATE_TMQ_CLOSE,					// TMQ 종료
};

#endif