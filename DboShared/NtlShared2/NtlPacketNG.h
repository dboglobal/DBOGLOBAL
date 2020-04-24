//***********************************************************************************
//
//	File		:	NtlPacketNG.h
//
//	Begin		:	2006-05-12
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"
#include "NtlActionPattern.h"
#include "NtlMovement.h"

#include "NtlPacketUG.h"


//------------------------------------------------------------------
//	Protocl enumeration
//------------------------------------------------------------------
enum eOPCODE_NG
{
	NG_OPCODE_BEGIN = 37000,
	NG_HEARTBEAT = NG_OPCODE_BEGIN,

	NG_GAME_DATA_REQ, // 현재 게임 데이타 전송 요청
	NG_ENTER_GAME_REQ, // 게임 진입 요청
	NG_LEAVE_GAME_REQ, // 게임 진입 요청
	NG_ENTER_WORLD_REQ, // 오브젝트 월드 진입 요청
	NG_LEAVE_WORLD_REQ, // 오브젝트 월드 진입 요청
	NG_SPS_SERVER_EVENT,

	//NG_CHAR_SPAWN_RES, // 캐릭터 스폰 응답
	NG_CHAR_READY, // 캐릭터 준비 완료
	NG_CHAR_MOVE, // 캐릭터 이동
	NG_CHAR_DESTMOVE, // 캐릭터 목적지 이동
	NG_CHAR_FOLLOWMOVE, // 캐릭터 목표 이동
	NG_CHAR_CHANGE_HEADING, // 캐릭터 방향 회전

	NG_CHAR_TARGET_SELECT, // 타겟 선택
	NG_CHAR_TOGG_FIGHT, // 전투 모드
	NG_CHAR_TOGG_RUN, // 전투 모드
	NG_CHAR_TOGG_ATTACK, // 공격 시작
	NG_CHAR_TOGG_SITDOWN,	// 않기

	NG_CHAR_DIALOG, // 캐릭터 대화
	NG_CHAR_DIRECT_PLAY, // 캐릭터 연출
	NG_CHAR_DIRECT_TURNING,	// 탈것 turing
	NG_CHAR_SET_CONDITION, // 캐릭터 컨디션 변경
	NG_CHAR_BATTLE_MODE_NFY,	// character의 battle의 시작과 종료를 알린다.

	NG_CHAR_START_ESCORT_RES,	
	NG_CHAR_INITIALIZE, // 캐릭터를 초기화한다 [12/13/2007 SGpro]
	NG_CHAR_SETSPEEDPERCENT, //캐릭터에 속도를 퍼센트 만큼 조정한다 [12/13/2007 SGpro]

	NG_BOT_SKILL_REQ, // 스킬 사용 [2/4/2008 SGpro]
	NG_SKILL_TARGET_LIST,		// 스킬 타겟 목록 알림

	NG_BOT_BOTCAUTION_NFY,// Bot 경고 [2/20/2008 SGpro]

	NG_BOT_BOTCAUTION_HELPME_NFY, // Bot Help Me 경고 [5/27/2008 SGpro]

	NG_CHAR_DESPAWN,// Bot 디스폰 요청 [7/28/2008 SGpro]
	NG_CHAR_TELEPORT_REQ,// Bot 텔레포트 요청 [7/28/2008 SGpro]
	NG_CHAR_READY_TO_SPAWN,// [8/4/2008 SGpro]

	NG_CHAR_CHANGE_NICKNAME,// [11/6/2008 SGpro]

	NG_CHAR_MOB_ROLE_CHANGED_NFY,// [11/10/2008 SGpro]

	NG_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY,

	NG_SEND_SPS_EVENT,
	NG_SEND_AIS_EVENT,

	NG_CHANGE_SPS_SCENE_RES,

	NG_CHAR_LOC_AFTER_KNOCKDOWN_NFY,
	NG_CHAR_LOC_AFTER_SLIDING_NFY,
	NG_CHAR_LOC_AFTER_PUSH_NFY,

	NG_OPCODE_DUMMY,
	NG_OPCODE_END = NG_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_NG(WORD wOpCode);
//------------------------------------------------------------------


//------------------------------------------------------------------
//	Define protocol structure
//------------------------------------------------------------------
#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_GAME_DATA_REQ )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_ENTER_GAME_REQ )
	BYTE				objType; // eOBJTYPE
	union
	{
		sNPC_DATA			sNpcData; // npc
		sMOB_DATA			sMobData; // mob
		sPET_DATA			sPetData;
	};
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_LEAVE_GAME_REQ )
	HOBJECT				handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_ENTER_WORLD_REQ )
	HOBJECT				handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_LEAVE_WORLD_REQ )
	HOBJECT				handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_SPS_SERVER_EVENT )
	HOBJECT			hSubject;				// 받을 캐릭터 핸들
	BYTE			byEventType;			// eSPS_EVENT_TYPE
	BYTE			byTriggerType;			// Trigger Type
	DWORD			eventID;				// event id	
END_PROTOCOL()
//------------------------------------------------------------------
//BEGIN_PROTOCOL( NG_CHAR_SPAWN_RES )
//END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_READY )
	HOBJECT				handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_MOVE )
	HOBJECT				handle;
	sVECTOR3			vCurLoc;
	sVECTOR3			vCurDir;
	BYTE				byMoveDirection;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_DESTMOVE )
	HOBJECT				handle;
	bool				bHaveSecondDestLoc;
	sVECTOR3			vSecondDestLoc;
	BYTE				byDestLocCount;
	sVECTOR3			avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_FOLLOWMOVE )
	HOBJECT				handle;
	HOBJECT				hTarget;
	float				fDistance;
	BYTE				byMovementReason;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_CHANGE_HEADING )
	HOBJECT				handle;
	sVECTOR3			vCurDir;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_TARGET_SELECT )
	HOBJECT				handle;
	HOBJECT				hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_TOGG_FIGHT )
	HOBJECT				handle;
	bool				bFightMode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_TOGG_RUN )
	HOBJECT				handle;
	bool				bRunFlag;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_TOGG_ATTACK )
	HOBJECT				handle;
	bool				bAttackProgress;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_TOGG_SITDOWN )
	HOBJECT				handle;
	bool				bSitDown;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_DIALOG )
	HOBJECT			hSubject;				// 대화 하는 캐릭터 핸들
	BYTE			byDialogType;			// 대화 종류 ( eCHAR_DIALOG_TYPE )
	TBLIDX			textTblidx;				// 대화 내용 테이블 번호 ( INVALID 가 아니면 해당 텍스트를 찾아 출력하고, INVALID면 아래의 텍스트를 출력해 줄 것)
	bool			bIsRefSpeechTable;		// 스피치 테이블을 참조할 것인지
	WORD			wTextLen;				// 대화내용의 길이
	WCHAR			awchText[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1]; // 대화내용
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_DIRECT_PLAY )
	HOBJECT			hSubject;				// 대화 하는 캐릭터 핸들
	TBLIDX			directionTblidx;		// 연출 테이블 인덱스
	BYTE			byDirectPlayType;		// 연출 타입
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_DIRECT_TURNING )
	HOBJECT			hSubject;				// 탈것 handle
	TBLIDX			directionTblidx;		// 연출 테이블 인덱스
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_SET_CONDITION )
	HOBJECT			hSubject;					// 대화 하는 캐릭터 핸들
	DWORD			dwAddCharConditionFlag;		// 추가할 캐릭터 컨디션
	DWORD			dwRemoveCharConditionFlag;	// 제거할 캐릭터 컨디션	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_BATTLE_MODE_NFY )
	HOBJECT			hSubject;
	bool			bIsBattle;					// 전투중인가?
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_START_ESCORT_RES )
	HOBJECT			hSubject;				// 에스코트 주체 캐릭터 ( PC )
	HOBJECT			hSource;				// 에스코트 대상 캐릭터 ( NPC )
	WORD			wResultCode;			// 응답 코드
	BYTE			byTriggerType;			// 트리거 타입
	NTL_TS_T_ID		tid;					// 실행시킬 trigger id
	sTSM_SERIAL		sTSMSerial;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_INITIALIZE )		// [12/13/2007 SGpro]
	HOBJECT			hSubject;				// 초기화하는 캐릭터 핸들
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_SETSPEEDPERCENT )	// [12/13/2007 SGpro]
	HOBJECT			hSubject;				// 초기화하는 캐릭터 핸들
	float			fPercentRunSpeed;		// 달리는 속도 (백분률)
	float			fPercentWalkSpeed;		// 걷는 속도 (백분률)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_BOT_SKILL_REQ )			// [2/4/2008 SGpro]
	HOBJECT				handle;
	BYTE				byAvatarType;		// eDBO_AVATAR_TYPE
	TBLIDX				tblidxSkill;
	HOBJECT				hTarget;
	// If skill effect should be applied on a target character, too, the target character must be included in both byApplyTargetCount and ahApplyTarget.
	// 스킬의 타겟 캐릭터가 스킬 적용 대상인 경우에는 타겟 캐릭터도 byApplyTargetCount와 ahApplyTarget에 포함되어야 한다.
	// by YOSHIKI(2007-01-12)
	BYTE				byApplyTargetCount;
	HOBJECT				ahApplyTarget[NTL_MAX_NUMBER_OF_SKILL_TARGET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_SKILL_TARGET_LIST )
	HOBJECT				hSubject;
	// If skill effect should be applied on a target character, too, the target character must be included in both byApplyTargetCount and ahApplyTarget.
	// 스킬의 타겟 캐릭터가 스킬 적용 대상인 경우에는 타겟 캐릭터도 byApplyTargetCount와 ahApplyTarget에 포함되어야 한다.
	// by YOSHIKI(2007-02-22)
	BYTE				byApplyTargetCount;
	HOBJECT				ahApplyTarget[NTL_MAX_NUMBER_OF_SKILL_TARGET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_BOT_BOTCAUTION_NFY )		//  [2/20/2008 SGpro]
	HOBJECT				handle;
	HOBJECT				hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_BOT_BOTCAUTION_HELPME_NFY ) // [5/27/2008 SGpro]
	HOBJECT				handle;
	HOBJECT				hTarget;
	bool				bIsRequester; // true : SOS를 요청한 자
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_DESPAWN )// Bot 디스폰 요청 [7/28/2008 SGpro]
	HOBJECT			handle;
	BYTE			byTeleportType; // 텔레포트타입
	WORLDID			worldId;
	TBLIDX			worldTblidx;
	CNtlVector		vTeleportLoc;
	CNtlVector		vTeleportDir;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_TELEPORT_REQ )// Bot 텔레포트 요청 [7/28/2008 SGpro]
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_READY_TO_SPAWN )// [8/4/2008 SGpro]
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_CHANGE_NICKNAME )// [11/6/2008 SGpro]
	HOBJECT			handle;
	TBLIDX			tblidxNickName;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_MOB_ROLE_CHANGED_NFY )// [11/10/2008 SGpro]
	HOBJECT			handle;
	BYTE			byBotRoleType;		// eBOTROLETYPE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY )
	HOBJECT				handle;
	ACTIONPATTERN_FIELD unitIdx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_SEND_SPS_EVENT )
	BYTE				byEventType;	// eSSD_SCRIPT_TYPE : SSD_SCRIPT_TYPE_TS | SSD_SCRIPT_TYPE_TQS | SSD_SCRIPT_TYPE_WPS 만 사용
	HOBJECT				hSender;
	BYTE				bySenderType;	// eOBJTYPE : OBJTYPE_NPC | OBJTYPE_MOB
	TBLIDX				senderTblidx;
	// escort event
	HOBJECT				hPc;
	NTL_TS_EVENT_ID		teId;
	// tqs, wps event
	WORLDID				worldId;
	TBLIDX				wpsTblidx;
	DWORD				eventId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_SEND_AIS_EVENT )
	BYTE				byEventType;	// eSSD_SCRIPT_TYPE : SSD_SCRIPT_TYPE_TQS | SSD_SCRIPT_TYPE_WPS 만 사용
	HOBJECT				hSender;
	BYTE				bySenderType;	// eOBJTYPE : OBJTYPE_NPC | OBJTYPE_MOB
	TBLIDX				senderTblidx;
	WORLDID				worldId;
	TBLIDX				wpsTblidx;		// only wps
	DWORD				eventId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( NG_CHANGE_SPS_SCENE_RES )
	WORD				wResultCode;

	HOBJECT				hTarget;
	TBLIDX				targetTblidx;
	DWORD				dwPlayScript;
	DWORD				dwPlayScene;
	WORLDID				worldId;
	TBLIDX				wpsTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(NG_CHAR_LOC_AFTER_KNOCKDOWN_NFY)
	HOBJECT				hSubject;
	sVECTOR3			vCurLoc;
	sVECTOR2			vCurDir;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(NG_CHAR_LOC_AFTER_SLIDING_NFY)
	HOBJECT				hSubject;
	sVECTOR3			vCurLoc;
	sVECTOR2			vCurDir;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(NG_CHAR_LOC_AFTER_PUSH_NFY)
	HOBJECT				hSubject;
	sVECTOR3			vCurLoc;
	sVECTOR2			vCurDir;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()
