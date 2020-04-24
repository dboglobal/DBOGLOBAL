#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"
#include "NtlDragonBall.h"
#include "NtlRankBattle.h"
#include "NtlMail.h"
#include "NtlAdmin.h"
#include "NtlTimeQuest.h"
#include "NtlTeleport.h"
#include "NtlBudokai.h"
#include "NtlMatch.h"
#include "NtlPetition.h"
#include "NtlDojo.h"
#include "NtlHlsItem.h"
#include "NtlCharTitle.h"
#include "NtlHlsMachine.h"
#include "NtlMascot.h"
#include "NtlEventDef.h"


enum eOPCODE_QG
{
	QG_OPCODE_BEGIN = 39000,

	QG_LOAD_PC_DATA_RES = QG_OPCODE_BEGIN,
	QG_ITEM_CREATE_RES,
	QG_ITEM_MOVE_RES,
	QG_ITEM_MOVE_STACK_RES,
	QG_ITEM_UPDATE_RES,
	QG_ITEM_DELETE_RES,
	QG_ITEM_EQUIP_REPAIR_RES,
	QG_ITEM_REPAIR_RES,
	QG_ITEM_DUR_UPDATE_RES,
	QG_ITEM_USE_RES,
	QG_ITEM_UPGRADE_RES,
	QG_ITEM_IDENTIFY_RES,
	QG_ITEM_CREATE_EX_RES,						// 아이템 생성 확장형(여러개 생성 가능)
	QG_ITEM_DELETE_EX_RES,						// 아이템 삭제 확장형(여러개 생성 가능)
	QG_ITEM_AUTO_EQUIP_RES,						// 아이템 자동 장착(Quest에서 사용)
	QG_ITEM_AUTO_EQUIP_ROLLBACK_RES,			// 아이템 자동 장착 RollBack(Quest에서 사용)

	QG_ITEM_PICK_RES,
	QG_ZENNY_PICK_RES,

	QG_SKILL_RP_BONUS_SETTING_RES,
	QG_SKILL_LOAD_RES,
	QG_BUFF_LOAD_RES,
	QG_HTB_SKILL_LOAD_RES,
	
	QG_LOAD_PC_BANK_DATA_RES,
	QG_LOAD_PC_BANK_DATA,
	QG_BANK_MOVE_RES,
	QG_BANK_MOVE_STACK_RES,
	QG_BANK_ZENNY_RES,
	QG_BANK_BUY_RES,
	QG_BANK_ADD_WITH_COMMAND_RES,
	QG_BANK_ITEM_DELETE_RES,

	QG_PC_UPDATE_POSITION_RES,
	QG_PC_UPDATE_EXP_RES,
	QG_PC_UPDATE_LPEPRP_RES,
	QG_PC_UPDATE_LEVEL_RES,
	QG_SAVE_PC_DATA_RES,
	QG_SAVE_SKILL_DATA_RES,
	QG_SAVE_HTB_DATA_RES,
	QG_SAVE_BUFF_DATA_RES,

	QG_PC_EXIT_RES,
	//QG_MAKE_AUTH_KEY_RES,

	QG_SHOP_BUY_RES,
	QG_SHOP_SELL_RES,	

	QG_PARTY_LOOTING_ITEM_RES,
	QG_PARTY_LOOTING_ZENNY_RES,

	QG_PC_UPDATE_BIND_RES,
	QG_CHAR_CONVERT_CLASS_RES,
	QG_CHAR_PUNISH_RES,						// PUNISH CHARACTER
	QG_CHAR_AWAY_RES,
	QG_CHAR_KEY_UPDATE_RES,

	QG_UPDATE_CHAR_ZENNY_RES,

	QG_QUEST_ITEM_CREATE_RES,					// 퀘스트 아이템 생성
	QG_QUEST_ITEM_DELETE_RES,					// 퀘스트 아이템 삭제
	QG_QUEST_ITEM_MOVE_RES,					// 퀘스트 아이템 이동
	QG_QUEST_PROGRESS_DATA_CREATE_RES,			// 퀘스트 진행 정보 생성
	QG_QUEST_PROGRESS_DATA_DELETE_RES,			// 퀘스트 진행 정보 삭제
	QG_QUEST_COMPLETE_DATA_UPDATE_RES,			// 퀘스트 완료 정보 업데이트
	QG_QUEST_DATA_RESET_RES,					// 퀘스트 정보를 지정된 범위만큼 리셋한다.
	QG_QUEST_STATE_UPDATE_RES,					// 상태의 변경
	QG_QUEST_TSP_UPDATE_RES,					// TS 포인터의 변경
	QG_QUEST_EXCEPTION_TIMER_UPDATE_RES,		// 예외 타이머의 등록 및 삭제-> 등록 및 삭제??
	QG_QUEST_SSM_UPDATE_RES,					// 메모리 슬롯의 업데이트
	QG_QUEST_SERVER_EVENT_UPDATE_RES,			// 서버 이벤트 데이터의 업데이트
	QG_QUEST_EXC_CLIENT_GROUP_RES,				// 클라이언트 실행 그룹 아이디
	QG_QUEST_INFO_UPDATE_RES,					// 퀘스트 정보 출력 데이터의 업데이트
	//------------------------------------------------------------------

	QG_QUICK_SLOT_UPDATE_RES,
	QG_QUICK_SLOT_DEL_RES,

	QG_SAVE_SPAWNED_SUMMON_PET_DATA_RES,		// 소환되어 있는 summon pet의 저장
	QG_SAVE_SPAWNED_ITEM_PET_DATA_RES,			// 소환되어 있는 item pet의 저장
	QG_LOAD_SUMMON_PET_DATA_RES,				// 소환된 summon pet 정보
	QG_LOAD_ITEM_PET_DATA_RES,					// 소환된 item pet 정보
	QG_LOAD_PET_BUFF_RES,						// 소환된 pet이 가지고 있는 buff 정보
	QG_DELETE_ALL_TEMPORARY_PET_DATA_RES,		// 임시로 저장된 펫 정보의 삭제

	QG_TRADE_RES,
	
	QG_SCHEDULE_INFO,
	QG_TUTORIAL_HINT_UPDATE_RES,
	QG_BALL_ITEM_PICK_RES,
	QG_BALL_ITEM_PARTY_PICK_RES,
	QG_BALL_ITEM_REWARD_RES,

	QG_PRIVATESHOP_ITEM_BUYING_RES,				
	QG_PRIVATESHOP_ITEM_INSERT_RES, 			
	QG_PRIVATESHOP_ITEM_DELETE_RES, 			
	QG_PRIVATESHOP_ITEM_UPDATE_RES, 			
	QG_PRIVATESHOP_SHOP_LOADING_RES,			
	QG_PRIVATESHOP_ITEM_DATA_INFO,				
	QG_PRIVATESHOP_CREATE_RES,					
	QG_PRIVATESHOP_UPDATE_RES,					

	QG_RANKBATTLE_SCORE_UPDATE_RES,

	QG_TMQ_DAYRECORD_RESET_RES,			// DayRecord 리셋 요청 응답
	QG_TMQ_DAYRECORD_UPDATE_RES,		// DayRecord 기록 요청 응답
	QG_TMQ_DAYRECORD_LIST_RES,			// DayRecord 리스트 요청(게시판) 응답
	QG_TMQ_DAYRECORD_RES,				// DayRecord 응답

	QG_MAIL_START_RES,
	QG_MAIL_SEND_RES,
	QG_MAIL_READ_RES,
	QG_MAIL_DEL_RES,
	QG_MAIL_RETURN_RES,
	QG_MAIL_RELOAD_RES,
	QG_MAIL_LOAD_RES,
	QG_MAIL_ITEM_RECEIVE_RES,
	QG_MAIL_LOCK_RES,
	QG_MAIL_EVENT_SEND_RES,
	QG_MAIL_MULTI_DEL_RES,

	QG_PORTAL_START_RES,
	QG_PORTAL_ADD_RES,

	QG_WAR_FOG_UPDATE_RES,	

	QG_ITEM_UPGRADE_ALL_RES,

	QG_GUILD_BANK_LOAD_RES,
	QG_GUILD_BANK_MOVE_RES,
	QG_GUILD_BANK_MOVE_STACK_RES,
	QG_GUILD_BANK_ZENNY_RES,
	QG_GUILD_BANK_LOAD_DATA,

	QG_SHOP_ITEM_IDENTIFY_RES,

	QG_BUDOKAI_DATA_RES,
	QG_BUDOKAI_INIT_DATA_NFY,
	QG_BUDOKAI_INIT_DATA_RES,
	QG_BUDOKAI_UPDATE_STATE_RES,
	QG_BUDOKAI_UPDATE_MATCH_STATE_RES,
	QG_BUDOKAI_UPDATE_CLEAR_RES,
	QG_RANKPOINT_RESET_RES,
	QG_RANKBATTLE_ALLOW_RES,

	QG_BUDOKAI_INDIVIDUAL_ALLOW_REGISTER_RES,
	QG_BUDOKAI_JOIN_INDIVIDUAL_RES,
	QG_BUDOKAI_LEAVE_INDIVIDUAL_RES,
	QG_BUDOKAI_INDIVIDUAL_SELECTION_RES,
	QG_BUDOKAI_INDIVIDUAL_LIST_START,
	QG_BUDOKAI_INDIVIDUAL_LIST_DATA,
	QG_BUDOKAI_INDIVIDUAL_LIST_END,
	QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES,	// 토너먼트 진출자 추가
	QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_START,		// 토너먼트 진출자 리스트 시작
	QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_DATA,		// 토너먼트 진출자 리스트 데이타
	QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_END,		// 토너먼트 진출자 리스트 끝
	QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_MATCH_RESULT_RES,	// 토너먼트 경기 결과 추가
	QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_MATCH_RESULT_RES,		// 토너먼트 경기 결과 리스트 요청

	QG_BUDOKAI_TEAM_ALLOW_REGISTER_RES,
	QG_BUDOKAI_JOIN_TEAM_RES,
	QG_BUDOKAI_LEAVE_TEAM_RES,
	QG_BUDOKAI_LEAVE_TEAM_MEMBER_RES,
	QG_BUDOKAI_TEAM_SELECTION_RES,
	QG_BUDOKAI_TEAM_LIST_START,
	QG_BUDOKAI_TEAM_LIST_DATA,
	QG_BUDOKAI_TEAM_LIST_END,
	QG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES,		// 토너먼트 진출자 추가
	QG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_START,		// 토너먼트 진출자 리스트 시작
	QG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_DATA,			// 토너먼트 진출자 리스트 데이타
	QG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_END,			// 토너먼트 진출자 리스트 끝
	QG_BUDOKAI_TOURNAMENT_TEAM_ADD_MATCH_RESULT_RES,	// 토너먼트 경기 결과 추가
	QG_BUDOKAI_TOURNAMENT_TEAM_MATCH_RESULT_RES,		// 토너먼트 경기 결과 리스트 요청

	QG_BUDOKAI_JOIN_INFO_RES,
	QG_BUDOKAI_JOIN_STATE_RES,
	QG_BUDOKAI_HISTORY_WRITE_RES,						// 개인전 season history 저장 응답
	QG_BUDOKAI_HISTORY_WINNER_PLAYER_RES,				// 해당 season 의 player info 응답
	QG_BUDOKAI_JOIN_STATE_LIST_RES,						// 리스트 요청 응답
	QG_BUDOKAI_JOIN_STATE_LIST_DATA,					// 리스트 데이터(수량만큼 반복)
	QG_BUDOKAI_SET_OPEN_TIME_RES,						// Set Open Time

	QG_MATCH_REWARD_RES,				// 경기 보상

	QG_SCOUTER_ITEM_SELL_RES,

	QG_SHOP_EVENTITEM_BUY_RES,

	QG_SHOP_GAMBLE_BUY_RES, 

	QG_BUDOKAI_UPDATE_MUDOSA_POINT_RES,
	QG_BUDOKAI_ADD_MUDOSA_POINT_RES,
	
	QG_ITEM_REPLACE_RES,

	QG_UPDATE_MARKING_RES,

	QG_SKILL_ADD_RES,
	QG_SKILL_UPGRADE_RES,
	QG_HTB_SKILL_ADD_RES,
	QG_SKILL_BUY_RES,
	
	QG_SKILL_INIT_RES,

	QG_RECIPE_REG_RES,
	QG_HOIPOIMIX_JOB_SET_RES,
	QG_HOIPOIMIX_JOB_RESET_RES,
	QG_HOIPOIMIX_ITEM_MAKE_RES,

	QG_ITEM_STACK_UPDATE_RES,

	QG_DOJO_BANK_HISTORY_RES,
	QG_DOJO_BANK_ZENNY_UPDATE_RES,
	QG_WORLD_SCHEDULE_SET_RES,

	QG_SWITCH_CHILD_ADULT_RES,
	QG_SET_CHANGE_CLASS_AUTHORITY_RES,

	QG_ITEM_CHANGE_ATTRIBUTE_RES,

	QG_CHANGE_NETP_RES,

	QG_ITEM_CHANGE_DURATIONTIME_RES,

	QG_SHOP_NETPYITEM_BUY_RES,

	QG_DURATION_ITEM_BUY_RES,
	QG_DURATION_RENEW_RES,

	QG_GM_COMMAND_LOG_RES,

	GQ_CASHITEM_HLSHOP_REFRESH_RES,
	QG_CASHITEM_INFO,
	QG_CASHITEM_INFO_RES,
	QG_CASHITEM_MOVE_RES,
	QG_CASHITEM_DEL_RES,
	QG_CASHITEM_UNPACK_RES,
	QG_CASHITEM_BUY_RES,

	QG_GMT_UPDATE_RES,

	QG_PC_DATA_LOAD_RES,
	QG_PC_ITEM_LOAD_RES,
	QG_PC_SKILL_LOAD_RES,		
	QG_PC_BUFF_LOAD_RES,
	QG_PC_HTB_LOAD_RES,
	QG_PC_QUEST_ITEM_LOAD_RES,		
	QG_PC_QUEST_COMPLETE_LOAD_RES,
	QG_PC_QUEST_PROGRESS_LOAD_RES,	
	QG_PC_QUICK_SLOT_LOAD_RES,	
	QG_PC_SHORTCUT_LOAD_RES,
	QG_PC_ITEM_RECIPE_LOAD_RES,
	QG_PC_GMT_LOAD_RES,

	QG_TENKAICHIDAISIJYOU_SERVERDATA_RES,
	QG_TENKAICHIDAISIJYOU_PERIODEND_RES,
	QG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES,
	QG_TENKAICHIDAISIJYOU_BUY_RES,

	QG_HLS_SLOT_MACHINE_RELOAD_RES,
	QG_HLS_SLOT_MACHINE_EXTRACT_RES,
	QG_HLS_SLOT_MACHINE_WINNER_LIST_RES,
	QG_INIT_WAGU_MACHINE_QNTT_RES,
	QG_HLS_SLOT_MACHINE_LIST_RES,

	QG_MAIL_START_INFO,
	QG_MAIL_LOAD_INFO,

	QG_QUICK_TELEPORT_INFO_LOAD_RES,
	QG_QUICK_TELEPORT_UPDATE_RES,
	QG_QUICK_TELEPORT_DEL_RES,
	QG_QUICK_TELEPORT_USE_RES,

	QG_PC_ITEM_COOL_TIME_LOAD_RES,
	QG_SAVE_ITEM_COOL_TIME_DATA_RES,

	QG_CASHINFOREFRESH_RES,
	QG_CASHITEM_SEND_GIFT_RES,

	QG_PC_MASCOT_LOAD_RES,
	QG_MASCOT_REGISTER_RES,

	QG_PC_DATA_LOAD_STRESSTEST_RES,

	QG_MATERIAL_DISASSEMBLE_RES,

	QG_ITEM_SOCKET_INSERT_BEAD_RES,
	QG_ITEM_SOCKET_DESTROY_BEAD_RES,

	QG_CASHITEM_PUBLIC_BANK_ADD_RES,
	QG_ITEM_EXCHANGE_RES,
	QG_CHARACTER_RENAME_RES,
	QG_ITEM_CHANGE_OPTION_RES,
	QG_WAGUWAGUMACHINE_COIN_INCREASE_RES,
	QG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES,
	QG_ITEM_SEAL_RES,
	QG_ITEM_SEAL_EXTRACT_RES,

	QG_DRAGONBALL_SCRAMBLE_END_NFY,

	QG_ITEM_UPGRADE_BY_COUPON_RES,
	QG_ITEM_GRADEINIT_BY_COUPON_RES,

	QG_EVENT_REWARD_LOAD_INFO,
	QG_EVENT_REWARD_LOAD_RES,
	QG_EVENT_REWARD_SELECT_RES,

	QG_MASCOT_SEAL_SET_RES,
	QG_MASCOT_SEAL_CLEAR_RES,

	QG_OPCCODE_END_DUMMY,
	QG_OPCODE_END = QG_OPCCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_QG(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(push, 1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_LOAD_PC_DATA_RES)

END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_CREATE_RES)
HOBJECT					handle;
WORD					wResultCode;
CHARACTERID				charID;
sITEM_DATA				sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_MOVE_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	HOBJECT					hSrcItem;
	HOBJECT					hDstItem;
	BYTE					bySrcPlace;
	BYTE					bySrcPos;
	BYTE					byDstPlace;
	BYTE					byDstPos;
	bool					bRestrictUpdate;
	BYTE					bySrcRestrictState;
	BYTE					byDestRestrictState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_MOVE_STACK_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	HOBJECT					hSrcItem;
	HOBJECT					hDstItem;
	BYTE					bySrcPlace;
	BYTE					bySrcPos;
	BYTE					byDstPlace;
	BYTE					byDstPos;
	BYTE					byStackCount1;
	BYTE					byStackCount2;
	ITEMID					splitItemId;// Created Item ID
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_UPDATE_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	sITEM_DATA				sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_DELETE_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					byPlace;
	BYTE					byPos;
	HOBJECT					hItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_ITEM_EQUIP_REPAIR_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					npcHandle;
	DWORD					dwZenny;
	WORD					wResultCode;
	BYTE					byCount;
	sITEM_REPAIR			asItemData[EQUIP_SLOT_TYPE_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_ITEM_REPAIR_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					npcHandle;
	DWORD					dwZenny;
	WORD					wResultCode;
	sITEM_REPAIR			sItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_ITEM_DUR_UPDATE_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					itemhandle;
	BYTE					byDur;
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_USE_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					byPlace;
	BYTE					byPos;
	HOBJECT					hItem;
	BYTE					byStack;
	DWORD					dwItemUseEventId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_UPGRADE_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	WORD					wResultCode;		// DB 성공 유무
	BYTE					byStack;			// 스톤 사용후 스택갯수					-> 변경될 스택
	BYTE					byGrade;			// 변경될 그레이드						-> 변경될 그레이드
	BYTE					byItemPlace;		// 업그레이드 할 아이템의 컨테이너 위치 
	BYTE					byItemPos;			// 업그레이드 할 아이템의 위치
	BYTE					byStonPlace;		// 호이포이스톤 컨테이너 위치
	BYTE					byStonPos;			// 호이포이스톤 위치
	bool					bType;				// true 삭제 false 업그레이드
	BYTE					byBattleAttribute;	// 업데이트 할 속성
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_IDENTIFY_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					itemhandle;
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_CREATE_EX_RES)
	HOBJECT					handle;
	CHARACTERID				charId;
	WORD					wResultCode;	// 응답 코드
	BYTE					byRequestType;	// 아이템 생성 타입 eITEM_CREATE_TYPE
	uITEM_CREATE_SUB_DATA	uSubData;		// 추가 정보
	BYTE					byUpdateCount;	// Stack 변경갯수
	sITEM_BASIC_DATA		asUpdateData[ITEM_CREATE_EX_MAX_COUNT];
	BYTE					byItemCount;	// 아이템 갯수
	sITEM_DATA				aItem[1];		// 아이템 정보
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_DELETE_EX_RES)
	HOBJECT					handle;
	CHARACTERID				charId;
	WORD					wResultCode;	// 응답 코드
	BYTE					byRequestType;	// 아이템 생성 타입 eITEM_CREATE_TYPE
	uITEM_DELETE_SUB_DATA	uSubData;		// 추가 정보
	BYTE					byItemCount;	// 아이템 갯수
	sITEM_DELETE_DATA		aItem[1];		// 아이템 정보
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_AUTO_EQUIP_RES)
	HOBJECT					handle;
	CHARACTERID				charId;
	WORD					wResultCode;
	sTSM_SERIAL				sTSMSerial;
	sITEM_AUTO_EQUIP_DATA	sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_AUTO_EQUIP_ROLLBACK_RES)
	HOBJECT					handle;
	CHARACTERID				charId;
	WORD					wResultCode;
	sTSM_SERIAL				sTSMSerial;
	sITEM_AUTO_EQUIP_ROLLBACK_DATA	sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_PICK_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					itemhandle;
	bool					bIsNew;
	WORD					wResultCode;
	sITEM_DATA				sItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ZENNY_PICK_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					itemhandle;
	WORD					wResultCode;
	DWORD					dwZenny;
	DWORD					dwOrgZenny;
	DWORD					dwBonusZenny;
END_PROTOCOL()
//---------------------------------------------------------------
BEGIN_PROTOCOL(QG_SKILL_RP_BONUS_SETTING_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	TBLIDX					skillId;
	BYTE					bySkillSlotIndex;
	BYTE					byRpBonusType;
	bool					bIsRpBonusAuto;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SKILL_LOAD_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	BYTE					byCount;
	sSKILL_DATA				sSkill[NTL_MAX_PC_HAVE_SKILL];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUFF_LOAD_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	BYTE					byCount;
	sBUFF_DATA				sBuff[DBO_MAX_BUFF_CHARACTER_HAS];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_HTB_SKILL_LOAD_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	BYTE					byCount;
	sHTB_SKILL_DATA			asSkill[NTL_HTB_MAX_PC_HAVE_HTB_SKILL];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_LOAD_PC_BANK_DATA_RES)
HOBJECT					handle;
WORD					wResultCode;
CHARACTERID				charID;
HOBJECT					npchandle;
DWORD					dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_LOAD_PC_BANK_DATA)
CHARACTERID				charID;
BYTE					byTotalCount;
BYTE					byCurPacketCount;
BYTE					byItemCount;
sITEM_DATA				asItemData[NTL_MAX_BANK_ITEM_SLOT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BANK_MOVE_RES)
HOBJECT					handle;
HOBJECT					hNpcHandle;
WORD					wResultCode;
CHARACTERID				charID;
HOBJECT					hSrcItem;
HOBJECT					hDstItem;
BYTE					bySrcPlace;
BYTE					bySrcPos;
BYTE					byDstPlace;
BYTE					byDstPos;
bool					bRestrictUpdate;
BYTE					bySrcRestrictState;
BYTE					byDestRestrictState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BANK_MOVE_STACK_RES)
HOBJECT					handle;
HOBJECT					hNpcHandle;
WORD					wResultCode;
CHARACTERID				charID;
HOBJECT					hSrcItem;
HOBJECT					hDstItem;
BYTE					bySrcPlace;
BYTE					bySrcPos;
BYTE					byDstPlace;
BYTE					byDstPos;
BYTE					byStackCount1;
BYTE					byStackCount2;
ITEMID					splitItemId;// Created Item ID
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BANK_ZENNY_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					npchandle;
	WORD					wResultCode;
	DWORD					dwZenny;
	bool					bIsSave;		// 1: 저금 0: 인출
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BANK_BUY_RES)
	HOBJECT					handle;
	HOBJECT					npchandle;
	WORD					wResultCode;
	BYTE					byMerchantTab;
	BYTE					byMerchantPos;
	DWORD					dwZenny;
	TBLIDX					itemNo;					// Template Number
	CHARACTERID				charId;					// Owner Serial
	BYTE					byPlace;				// eCONTAINER_TYPE
	BYTE					byPosition;	
	BYTE					byRank;					// 0 1 2 3 //창고종류로 쓸것임
	ITEMID					itemID;
	BYTE					byDurationType;
	DBOTIME					nUseStartTime;
	DBOTIME					nUseEndTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BANK_ADD_WITH_COMMAND_RES)
HOBJECT					handle;
WORD					wResultCode;
TBLIDX					itemNo;					// Template Number
CHARACTERID				charId;					// Owner Serial
ITEMID					itemID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BANK_ITEM_DELETE_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					byPlace;
	BYTE					byPos;
	HOBJECT					hItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_UPDATE_POSITION_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	float					fPositionX;
	float					fPositionY;
	float					fPositionZ;
	float					fDirX;
	float					fDirZ;
	TBLIDX					worldTblidx;
	WORLDID					worldId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_UPDATE_EXP_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	DWORD					dwEXP;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_UPDATE_LPEPRP_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	WORD					wLP;
	WORD					wEP;
	WORD					wRP;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_UPDATE_LEVEL_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	BYTE					byLevel;
	DWORD					dwEXP;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SAVE_PC_DATA_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SAVE_SKILL_DATA_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SAVE_HTB_DATA_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SAVE_BUFF_DATA_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_EXIT_RES)
	WORD					wResultCode;
	ACCOUNTID				accountId;
	eCHARLEAVING_TYPE		eCharLeavingType;
END_PROTOCOL()
//------------------------------------------------------------------
/*BEGIN_PROTOCOL(QG_MAKE_AUTH_KEY_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	ACCOUNTID				accountId;
	CHARACTERID				charId;
END_PROTOCOL()*/
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SHOP_BUY_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	HOBJECT					hNpchandle;
	CHARACTERID				charId;
	DWORD					dwMoney;
	BYTE					byBuyCount;
	ITEMID					itemID[NTL_MAX_BUY_SHOPPING_CART];
	sSHOP_BUY_INVEN			sInven[NTL_MAX_BUY_SHOPPING_CART];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SHOP_SELL_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	HOBJECT					hNpchandle;
	DWORD					dwMoney;
	CHARACTERID				charId;
	BYTE					bySellCount;
	sSHOP_SELL_INVEN		sInven[NTL_MAX_SELL_SHOPPING_CART];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PARTY_LOOTING_ITEM_RES)
	WORD			wResultCode;
	// The item owner can be found out with using 'sItemData.charId'.
	// 아이템을 가질 플레이어는 'sItemData.charId'로 알아낼 수 있다.
	// by YOSHIKI(2007-03-09)
	HOBJECT			hItem;
	bool			bNeedNewSlot;
	sITEM_DATA		itemData;
	bool			bByAutoDistribution;

	// For Game Log
	ACCOUNTID							winnerAccountId;
	PARTYID								partyId;
	sDBO_ZENNY_DATA		aZennyData[NTL_MAX_MEMBER_IN_PARTY];
	bool				bIsPartyInven;
	BYTE				byItemSlot;
	BYTE				byMemberIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PARTY_LOOTING_ZENNY_RES)
	WORD				wResultCode;
	CHARACTERID			looterCharId;
	HOBJECT				hDroppedZenny;
	DWORD				dwTotalZenny;
	bool				bIsShared;
	BYTE				byMemberCount;
	sZENNY_INFO			aZennyInfo[NTL_MAX_MEMBER_IN_PARTY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_UPDATE_BIND_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	float					fBindX;
	float					fBindY;
	float					fBindZ;
	float					fBindDirX;
	float					fBindDirZ;
	WORLDID					worldId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CHAR_CONVERT_CLASS_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	BYTE					byClass;
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CHAR_PUNISH_RES )
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	CHARACTERID				targetCharID;
	BYTE					byPunishType;		// Punish Type
	DWORD					dwPunishAmount;		// How Many Day or Minutes
	bool					bDateOption;		// 0: Day 1: Minutes 
	bool					bIsOn;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CHAR_AWAY_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	bool					bIsAway;		// On 1: Off: 0
	WORD 					wResultCode;	// 메일이 이미 삭제 되었을 때는 실패
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CHAR_KEY_UPDATE_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	WORD 					wResultCode;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_UPDATE_CHAR_ZENNY_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	DWORD					dwZenny;
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_ITEM_CREATE_RES )
	HOBJECT						handle;
	DWORD						dwTimeStamp;
	CHARACTERID					charId;
	WORD						wResultCode;
	BYTE						byRequestType;	// sub data : Create Type [eQUEST_ITEM_REQUEST_TYPE]
	uQUEST_ITEM_UPDATE_SUB_DATA	uSubData;		// sub data;
	BYTE						byItemCount;
	sQUEST_ITEM_UPDATE_DATA		aItems[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_ITEM_DELETE_RES )
	HOBJECT						handle;
	DWORD						dwTimeStamp;
	CHARACTERID					charId;
	WORD						wResultCode;
	BYTE						byRequestType;	// sub data : Create Type [eQUEST_ITEM_REQUEST_TYPE]
	uQUEST_ITEM_UPDATE_SUB_DATA	uSubData;		// sub data;
	BYTE						byItemCount;
	sQUEST_ITEM_UPDATE_DATA		aItems[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_ITEM_MOVE_RES )
	HOBJECT						handle;
	DWORD						dwTimeStamp;
	CHARACTERID					charId;
	WORD						wResultCode;
	TBLIDX						dwSrcTblidx;
	BYTE						bySrcPos;
	TBLIDX						dwDestTblidx;
	BYTE						byDestPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_PROGRESS_DATA_CREATE_RES )
	HOBJECT					handle;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	WORD					wResultCode;
	sQUEST_PROGRESS_DATA	progressInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_PROGRESS_DATA_DELETE_RES )
	HOBJECT					handle;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	WORD					wResultCode;
	QUESTID					questID; // 퀘스트 아이디
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_COMPLETE_DATA_UPDATE_RES )
	HOBJECT					handle;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	WORD					wResultCode;
	BYTE						byPos;
	sCOMPLETE_QUEST_BIT_INFO	sData;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_DATA_RESET_RES )
	HOBJECT					handle;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	WORD					wResultCode;
	bool					bCompleteQuestData;	// 완료 정보 삭제 여부
	bool					bProgressQuestData;	// 진행 정보 삭제 여부
	QUESTID					startResetQID;		// 삭제 범위
	QUESTID					endResetQID;		// 삭제 범위
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_STATE_UPDATE_RES )			// 상태의 변경
	HOBJECT					handle;
	WORD					wResultCode;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	QUESTID					qId;					// Trigger ID(Quest ID)
	WORD					wTSState;				// 변경 할 상태 값
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_TSP_UPDATE_RES )			// TS 포인터의 변경
	HOBJECT					handle;
	WORD					wResultCode;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	QUESTID					qId;					// Trigger ID(Quest ID)
	sMAIN_TSP				sMainTSP;				// TS 포인터
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_EXCEPTION_TIMER_UPDATE_RES )// 예외 타이머의 업데이트
	HOBJECT					handle;
	WORD					wResultCode;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	QUESTID					qId;					// Trigger ID(Quest ID)
	sEXCEPT_TIMER_SLOT		sExceptTimerSlot;			// 예외 타이머 정보
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_SSM_UPDATE_RES )			// 메모리 슬롯의 업데이트
	HOBJECT					handle;
	WORD					wResultCode;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	QUESTID					qId;					// Trigger ID(Quest ID)
	BYTE					byIdx;
	DWORD					dwValue;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_SERVER_EVENT_UPDATE_RES )	// 서버 이벤트 데이터의 업데이트
	HOBJECT					handle;
	WORD					wResultCode;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	QUESTID					qId;					// Trigger ID(Quest ID)
	BYTE					bySvrEvtType;			// Server Event Type
	sSTOC_EVT_DB_DATA		sSvrEvt;				// Server Event Data
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_EXC_CLIENT_GROUP_RES )// 강제 이동 포인터의 업데이트
	HOBJECT					handle;
	WORD					wResultCode;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	QUESTID					qId;					// Trigger ID(Quest ID)
	NTL_TS_TG_ID			tgExcCGroup;			// 클라이언트 실행 그룹아이디
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUEST_INFO_UPDATE_RES )			// 퀘스트 정보 출력 데이터의 업데이트
	HOBJECT					handle;
	WORD					wResultCode;
	DWORD					dwTimeStamp;
	CHARACTERID				charId;
	QUESTID					qId;					// Trigger ID(Quest ID)
	NTL_TS_TC_ID			tcId;					// Trigger Container ID
	NTL_TS_TA_ID			taId;					// Trigger Action ID
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUICK_SLOT_UPDATE_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	WORD					wResultCode;
	TBLIDX					tblidx;		
	BYTE					bySlotID;				// QuickSlot 아이디
	BYTE					byType;					// 아이템인가 스킬인가 소셜액숀인가?
	BYTE					byPlace;
	BYTE					byPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_QUICK_SLOT_DEL_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	WORD					wResultCode;
	BYTE					bySlotID;				// QuickSlot 아이디
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_SAVE_SPAWNED_SUMMON_PET_DATA_RES )
	HOBJECT					handle;
	CHARACTERID				ownerCharId;
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_SAVE_SPAWNED_ITEM_PET_DATA_RES )
	HOBJECT					handle;
	CHARACTERID				ownerCharId;
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_LOAD_SUMMON_PET_DATA_RES )
	HOBJECT					handle;
	CHARACTERID				ownerCharId;
	sSUMMON_PET_DATA		petData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_LOAD_ITEM_PET_DATA_RES )
	HOBJECT					handle;
	CHARACTERID				ownerCharId;
	sITEM_PET_DATA			petData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_LOAD_PET_BUFF_RES )
	HOBJECT					handle;
	CHARACTERID				ownerCharId;
	BYTE					byCount;
	sPET_BUFF_DATA			aPetBuff[NTL_MAX_BLESS_BUFF_CHARACTER_HAS + NTL_MAX_CURSE_BUFF_CHARACTER_HAS];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_DELETE_ALL_TEMPORARY_PET_DATA_RES )
	HOBJECT					handle;
	CHARACTERID				ownerCharId;
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_TRADE_RES )
	WORD					wResultCode;
	HOBJECT					handle;
	HOBJECT					hTarget;	
	CHARACTERID				charID;
	CHARACTERID				dstcharID;	// 옮겨질 캐릭터 아이디
	BYTE					bySendCount;
	BYTE					byRecvCount;
	sTRADE_INVEN			asSendData[TRADE_INVEN_MAX_COUNT];
	sTRADE_INVEN			asRecvData[TRADE_INVEN_MAX_COUNT];
	DWORD					dwGiveZenny;	// 줄 제니
	DWORD					dwTakeZenny;	// 받을 제니
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_SCHEDULE_INFO )
	WORD					wResultCode;
	BYTE					byCount;
	sSCHEDULE_INFO			asInfo[SCHEDULE_EVENT_TYPE_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_TUTORIAL_HINT_UPDATE_RES )
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	DWORD					dwTutorialHint;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_BALL_ITEM_PICK_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					itemhandle;
	WORD					wResultCode;
	sITEM_DATA				sItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_BALL_ITEM_PARTY_PICK_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					itemhandle;
	WORD					wResultCode;
	sITEM_DATA				sItemData;
	BYTE					byCount;
	CHARACTERID				aCharID[NTL_MAX_MEMBER_IN_PARTY];
	sEMPTY_INVEN			asEmpty[NTL_MAX_MEMBER_IN_PARTY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_BALL_ITEM_REWARD_RES )
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					hTarget; // TS Object
	WORD					wResultCode;
	sITEM_DELETE_DATA		asData[NTL_ITEM_MAX_DRAGONBALL];
	BYTE					byRewardType;		//eDRAGONBALL_REWARD_TYPE
	TBLIDX					skillId;	
	sITEM_DATA				sItemData;
	DWORD					dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PRIVATESHOP_ITEM_BUYING_RES )
	HOBJECT						handle;
	HOBJECT						hTarget;
	WORD						wResultCode;
	CHARACTERID					charID_From;
	CHARACTERID					charID_To;
	DWORD						dwAllZenny;
	BYTE						byItemCount;
	sPRIVATESHOP_ITEM_POS_DATA	asPrivateShopItemPos[NTL_MAX_BUY_SHOPPING_CART];
	sINVEN_ITEM_POS_DATA		asEmpty[NTL_MAX_BUY_SHOPPING_CART];
	BYTE						byPrivateShopState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PRIVATESHOP_ITEM_INSERT_RES )
	HOBJECT						handle;
	WORD						wResultCode;
	CHARACTERID					charID;
	sPRIVATESHOP_ITEM_POS_DATA	sPrivateShopEmpty;
	sINVEN_ITEM_POS_DATA		sInventoryItemPos;
	DWORD						dwZenny;		// 등록 가격
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PRIVATESHOP_ITEM_DELETE_RES )
	HOBJECT						handle;
	WORD						wResultCode;
	CHARACTERID					charID;
	sPRIVATESHOP_ITEM_POS_DATA	sItemPos;
	sINVEN_ITEM_POS_DATA		sInventoryEmpty;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PRIVATESHOP_ITEM_UPDATE_RES )
	HOBJECT						handle;
	WORD						wResultCode;
	CHARACTERID					charID;
	DWORD						dwAfterZenny;
	DWORD						dwBeforeZenny;
	BYTE						byFromPos;
	BYTE						byToPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PRIVATESHOP_SHOP_LOADING_RES )
	HOBJECT						handle;
	WORD						wResultCode;
	CHARACTERID					charID;
	WCHAR						wcPrivateShopName[NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1];
	WCHAR						wcNotice[NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1];
	__int64						nCashShopStartTime; //유료 샵이 된 날짜
	__int64						nCashShopEndTime;	//유료 샵 만료일
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PRIVATESHOP_ITEM_DATA_INFO )
	HOBJECT						handle;
	WORD						wResultCode;
	CHARACTERID					charID;
	sITEM_DATA					sItem;		//PRIVATESHOP_ITEM_DATA::IsSaveItem이 true일 경우 query에서 채워서 game에게 보내야 합니다
	DWORD						dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PRIVATESHOP_CREATE_RES )
	HOBJECT						handle;
	WORD						wResultCode;
	CHARACTERID					charID;
	__int64						nCashShopStartTime; //유료 샵이 된 날짜
	__int64						nCashShopEndTime;	//유료 샵 만료일
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PRIVATESHOP_UPDATE_RES )
	HOBJECT						handle;
	WORD						wResultCode;
	CHARACTERID					charID;
	WCHAR						wcPrivateShopName[NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1];
	WCHAR						wcNotice[NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1];
	bool						bIsOwnerEmpty;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_RANKBATTLE_SCORE_UPDATE_RES )
	HOBJECT						handle;
	WORD						wResultCode;
	CHARACTERID					charID;
	BYTE						byBattleMode;
	sRANKBATTLE_SCORE_INFO		sTotalScore;
	DWORD						dwMudosaPoint;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_TMQ_DAYRECORD_RESET_RES )
	WORD						wResultCode;

	DWORD						dwDay;				// 년월일 (ex.20071116) 과 같이 전송됨
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;		// eTIMEQUEST_DIFFICULTY

	DWORD						dwClearTime;
	bool						bIsBestRecord;		// BestRecord 기록자인가?
	WCHAR						wszPartyName[NTL_MAX_SIZE_PARTY_NAME+1];
	BYTE						byCount;
	sTIMEQUEST_MEMBER_DATA		asMember[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_TMQ_DAYRECORD_UPDATE_RES )
	WORD						wResultCode;
	WORLDID						worldId;		// res 에서 TMQ 에 재 접근 하기 위함
	TBLIDX						tmqTblidx;

	DWORD						dwDayRecordTime;// DayRecord 일 경우 INVALID_DWORD
	bool						bIsBestRecord;	// best record 인가?
	BYTE						byRank;			// 현재 등수 x / 5
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_TMQ_DAYRECORD_LIST_RES )
	HOBJECT						handle;
	CHARACTERID					charId;
	WORD						wResultCode;
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
	BYTE						byTeamCount;
	sTIMEQUEST_TEAM_DATA		asTeam[DBO_TIMEQUEST_DEFAULT_DAYRECORD_RANK_COUNT];		// 용량 주의
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_TMQ_DAYRECORD_RES )
	WORD						wResultCode;
	TBLIDX						tmqTblidx;
	ROOMID						roomId;
	DWORD						adwDayRecord[MAX_TIMEQUEST_DIFFICULTY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_START_RES )
	HOBJECT						handle;
	HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	CHARACTERID					charID;
	WORD 						wResultCode;	// 결과
	bool						bIsAway;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_SEND_RES )
	HOBJECT						handle;
	HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	CHARACTERID					charID;
	WCHAR						wszTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];	// 받을 캐릭 이름
	BYTE						byMailType;	// eMAIL_TYPE	
 	WORD						wResultCode;	// 성공유무
	sINVEN_ITEM_POS_DATA		sItemData;	// 아이템정보
	DWORD						dwZenny;		// Req or Give Zenny
	CHARACTERID					targetCharID;
	MAILID						mailID;
	bool						bIsTemp;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_READ_RES )
	HOBJECT						handle;
	HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	CHARACTERID					charID;
	MAILID						mailID;	// 메일 아이디
	WORD						wResultCode;	// 메일이 이미 삭제 되었을 때는 실패 
	BYTE						byRemainDay;
	DBOTIME						endTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_DEL_RES )
	HOBJECT						handle;
	HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	CHARACTERID					charID;
	MAILID						mailID;	// 메일 아이디
	WORD						wResultCode;	// 메일이 이미 삭제 되었을 때는 실패 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_RETURN_RES )
	HOBJECT						handle;
	HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	CHARACTERID					charID;
	MAILID						mailID;	// 메일 아이디
	WORD						wResultCode;	// 메일이 이미 삭제 되었을 때는 실패 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_RELOAD_RES )
	HOBJECT						handle;
	HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	CHARACTERID					charID;
	WORD						wResultCode;	// 성공유무
	bool						bIsSchedule;	// 0 = reload by user / 1 = reload by system
	BYTE						byCount;	// 현재메일카운트
	MAILID						asMailID[NTL_MAX_MAIL_SLOT_COUNT];
	sMAIL_BRIEF					asMailInfo[NTL_MAX_MAIL_SLOT_COUNT];	// 카운트에 해당하는 저장된 메일

	BYTE						byTotalCount;
	BYTE						byNormalCount;
	BYTE						byManagerCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_LOAD_RES )
HOBJECT						handle;
HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
CHARACTERID					charID;
WORD						wResultCode;	// 성공유무
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_ITEM_RECEIVE_RES )
	HOBJECT						handle;
	HOBJECT						hObject;
	CHARACTERID					charID;
	WORD						wResultCode;
	MAILID						mailID;	
	sITEM_DATA					sItemData;
	DWORD						dwZenny;		// Req or Give Zenny
	CHARACTERID					fromCharId;
	BYTE						byMailType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_LOCK_RES )
	HOBJECT						handle;
	HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	CHARACTERID					charID;
	MAILID						mailID;	// 메일 아이디	
	bool						bIsLock;		// Lock 1: Unlock: 0
	WORD 						wResultCode;	// 메일이 이미 삭제 되었을 때는 실패
	BYTE						byExpiredDay;	// Unlock시 남은 날짜 갱신에 사용
	DBOTIME						endTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_EVENT_SEND_RES )
	CHARACTERID					charID;
	CHARACTERID					targetCharID;
	BYTE						byMailType;	// eMAIL_TYPE
	BYTE						bySenderType;	// eMAIL_SENDER_TYPE
 	WORD						wResultCode;	// 성공유무
	TBLIDX						itemTblidx;	// 아이템인덱스
	DWORD						dwZenny;		// Req or Give Zenny
	MAILID						mailID;
	ITEMID						itemID;
	bool						bIsTemp;
	BYTE						byDay;		// 유효만료날짜 최대 10일
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_MAIL_MULTI_DEL_RES )
	HOBJECT						handle;
	HOBJECT						hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	CHARACTERID					charID;
	WORD						wResultCode;	// 메일이 이미 삭제 되었을 때는 실패 
	BYTE						byCount;
	MAILID						aMailID[NTL_MAX_COUNT_OF_MULTI_DEL];	// 메일 아이디
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PORTAL_START_RES )
CHARACTERID					charID;
WORD 						wResultCode;
BYTE						byCount;
PORTALID					aPortalID[NTL_PORTAL_MAX_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_PORTAL_ADD_RES )
	HOBJECT						handle;
	CHARACTERID					charID;
	HOBJECT						hNpcHandle;
	WORD 						wResultCode;
	PORTALID					PortalID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_WAR_FOG_UPDATE_RES )
	HOBJECT						handle;
	CHARACTERID					charID;
	HOBJECT						hObject;
	WORD 						wResultCode;
	bool						bIsCheat;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_ITEM_UPGRADE_ALL_RES )
	HOBJECT						handle;
	CHARACTERID					charID;
	WORD 						wResultCode;
	BYTE						abyGrade[NTL_ITEM_UPGRADE_EQUIP_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_GUILD_BANK_LOAD_RES )
	HOBJECT						handle;
	CHARACTERID					charID;
	WORD 						wResultCode;
	GUILDID						guildID;
	HOBJECT						hNpcHandle;
	DWORD						dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_GUILD_BANK_MOVE_RES)
HOBJECT					handle;
HOBJECT					hNpcHandle;
	WORD					wResultCode;
	CHARACTERID				charID;
	HOBJECT					hSrcItem;
	HOBJECT					hDstItem;
	BYTE					bySrcPlace;
	BYTE					bySrcPos;
	BYTE					byDstPlace;
	BYTE					byDstPos;
	bool					bRestrictUpdate;
	BYTE					bySrcRestrictState;
	BYTE					byDestRestrictState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_GUILD_BANK_MOVE_STACK_RES)
	HOBJECT					handle;
HOBJECT					hNpcHandle;
	WORD					wResultCode;
	CHARACTERID				charID;
	HOBJECT					hSrcItem;
	HOBJECT					hDstItem;
	BYTE					bySrcPlace;
	BYTE					bySrcPos;
	BYTE					byDstPlace;
	BYTE					byDstPos;
	BYTE					byStackCount1;
	BYTE					byStackCount2;
	ITEMID					splitItemId;// Created Item ID
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_GUILD_BANK_ZENNY_RES)
	HOBJECT					handle;
	CHARACTERID				charID;
	HOBJECT					npchandle;
	WORD					wResultCode;
	DWORD					dwZenny;
	bool					bIsSave;		// 1: 저금 0: 인출
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_GUILD_BANK_LOAD_DATA)
CHARACTERID				charID;
BYTE					byTotalCount;
BYTE					byCurPacketCount;
BYTE					byItemCount;
sITEM_DATA				asItemData[NTL_MAX_GUILD_ITEM_SLOT / 2];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SHOP_ITEM_IDENTIFY_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	ITEMID					itemID; 
	HOBJECT					hNpchandle; 
	BYTE					byPlace; 
	BYTE					byPos; 
	DWORD					dwZenny;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_DATA_RES)
	WORD							wResultCode;		// DB Data가 존재하지 않을 경우에도 GAME_SUCCESS를 보내준다.
	bool							bIsExistData;		// Db Data가 존재하는가?
	sBUDOKAI_UPDATEDB_INITIALIZE	sBudokaiData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_INIT_DATA_NFY)
	WORD					wSeasonCount;				// 몇회차인지.
	BYTE					byBudokaiStep;				// 현재 진행 단계, eBUDOKAI_STEP_TYPE
	__int64					nDefaultOpenTime;			// 기본 시작시간
	__int64					nNextStepTime;				// 다음 단계의 시작 시간
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_INIT_DATA_RES)
	WORD							wResultCode;
	bool							bIsFirstUpdate;		// true : 서버를 처음 가동하여 DB에 데이타가 없는 경우, false : 새로운 시즌을 시작하게된 경우
	sBUDOKAI_UPDATEDB_INITIALIZE	sInitData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_UPDATE_STATE_RES)
	WORD							wResultCode;
	sBUDOKAI_STATE_DATA				sStateData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_UPDATE_MATCH_STATE_RES)
	WORD							wResultCode;
	BYTE							byMatchType;		// eBUDOKAI_MATCH_TYPE
	sBUDOKAI_MATCHSTATE_DATA		sStateData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_UPDATE_CLEAR_RES)
	WORD							wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_RANKPOINT_RESET_RES)
	WORD							wResultCode;

	BYTE							byMinLevel;
	BYTE							byMaxLevel;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_RANKBATTLE_ALLOW_RES)
	WORD							wResultCode;

	bool							bAllow;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_INDIVIDUAL_ALLOW_REGISTER_RES)
	WORD							wResultCode;
	bool							bAllow;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_JOIN_INDIVIDUAL_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	float					fPoint;
	WORD					wJoinId;
	bool					bDojoRecommender;
	WCHAR					wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_LEAVE_INDIVIDUAL_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	JOINID					joinId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_INDIVIDUAL_SELECTION_RES)
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_INDIVIDUAL_LIST_START)
	WORD					wResultCode;
	WORD					wIndividualAllCount;	// 전체 등록 명수
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_INDIVIDUAL_LIST_DATA)
	WORD								wResultCode;
	WORD								wDataCount;		//
	sBUDOKAI_REGISTER_INDIVIDUAL_DATA	aData[BUDOKAI_MAX_INDIVIDUAL_DB_DATA_PACKET_COUNT];			// max = BUDOKAI_MAX_INDIVIDUAL_DB_DATA_PACKET_COUNT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_INDIVIDUAL_LIST_END)
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_RES)
	WORD						wResultCode;
	sBUDOKAI_JOIN_STATE_DATA	joinData;
	BYTE						byMatchIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_START)
	WORD					wResultCode;
	BYTE					byMaxCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_DATA)
	WORD										wResultCode;
	BYTE										byCount;
	sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA	aEntryData[BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_ENTRY_PACKET_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_LIST_END)
	WORD							wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_MATCH_RESULT_RES)
	WORD							wResultCode;
	sBUDOKAI_TOURNAMENT_MATCH_DATA	sMatchData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_INDIVIDUAL_MATCH_RESULT_RES)
	WORD							wResultCode;
	BYTE							byMatchDataCount;
	sBUDOKAI_TOURNAMENT_MATCH_DATA	aMatchData[BUDOKAI_MAX_TOURNAMENT_INDIVIDUAL_MATCH_RECORD_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TEAM_ALLOW_REGISTER_RES)
	WORD							wResultCode;
	bool							bAllow;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_JOIN_TEAM_RES)
WORD						wResultCode;
HOBJECT						handle;
CHARACTERID					charId;
WCHAR						wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
BYTE						byMemberCount;
CHARACTERID					aMembers[NTL_MAX_MEMBER_IN_PARTY];
float						fPoint;
WORD						wJoinId;
sBUDOKAI_TEAM_POINT_INFO	aTeamInfo[NTL_MAX_MEMBER_IN_PARTY];		// game server 에서 사용하는데이터 by niam
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_LEAVE_TEAM_RES)
WORD					wResultCode;
HOBJECT					handle;
CHARACTERID				charId;
BYTE					byMemberCount;
CHARACTERID				aMembers[NTL_MAX_MEMBER_IN_PARTY];
WORD					wJoinId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_LEAVE_TEAM_MEMBER_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	CHARACTERID				aMembers[NTL_MAX_MEMBER_IN_PARTY];
	WCHAR					wszMemberName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TEAM_SELECTION_RES)
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TEAM_LIST_START)
	WORD					wResultCode;
	WORD					wTeamAllCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TEAM_LIST_DATA)
	WORD						wResultCode;
	WORD						wDataCount;
	sBUDOKAI_REGISTER_TEAM_DATA	aData[BUDOKAI_MAX_TEAM_DB_DATA_PACKET_COUNT];			// max = BUDOKAI_MAX_TEAM_DB_DATA_PACKET_COUNT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TEAM_LIST_END)
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_RES)
	WORD					wResultCode;
	BYTE					byMatchIndex;
	sBUDOKAI_JOIN_STATE_DATA	joinData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_START)
	WORD							wResultCode;
	BYTE							byMaxCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_DATA)
	WORD								wResultCode;
	BYTE								byMemberCount;
	sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA	aEntryData[BUDOKAI_MAX_TOURNAMENT_TEAM_ENTRY_PACKET_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_TEAM_ENTRY_LIST_END)
	WORD							wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_TEAM_ADD_MATCH_RESULT_RES)
	WORD							wResultCode;
	sBUDOKAI_TOURNAMENT_MATCH_DATA	sMatchData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_TOURNAMENT_TEAM_MATCH_RESULT_RES)
	WORD							wResultCode;
	BYTE							byMatchDataCount;
	sBUDOKAI_TOURNAMENT_MATCH_DATA	aMatchData[BUDOKAI_MAX_TOURNAMENT_TEAM_MATCH_RECORD_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_JOIN_INFO_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;

	BYTE					byMatchType;		// eBUDOKAI_MATCH_TYPE
	BYTE					byJoinState;		// 참가 상태
	BYTE					byJoinResult;		// 참가 결과

	union
	{
		sBUDOKAI_REGISTER_INDIVIDUAL_INFO	sIndividualInfo;
		sBUDOKAI_REGISTER_TEAM_INFO			sTeamInfo;
	};
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_JOIN_STATE_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	bool					bIsClientReq;

	BYTE					byMatchType;		// eBUDOKAI_MATCH_TYPE
	BYTE					byJoinState;		// 참가 상태
	BYTE					byJoinResult;		// 참가 결과

	// 정보가 없을 경우
	// byMatchType = INVALID_BUDOKAI_MATCH_TYPE
	// byJoinState = INVALID_BUDOKAI_JOIN_STATE
	// byJoinResult = INVALID_BUDOKAI_JOIN_RESULT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_HISTORY_WRITE_RES)
	WORD								wResultCode;
	BYTE								byBudokaiType;
	BYTE								byMatchType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_HISTORY_WINNER_PLAYER_RES)
	WORD								wResultCode;

	WORD								wSeasonCount;			// 해당 season 의 player info 요청
	BYTE								byBudokaiType;
	BYTE								byMatchType;			// eBUDOKAI_MATCH_TYPE
	BYTE								byJoinResult;			// JoinResult 보다 크거나 같은 값만 로드한다.

	BYTE								byPlayerCount;
	sBUDOKAI_PREV_SEASON_PLAYER_INFO	aPlayerInfo[BUDOKAI_MAX_SEASON_HISTORY_WINNER_PLAYER_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_JOIN_STATE_LIST_RES)
	WORD								wResultCode;
	BYTE								byMatchType;			// eBUDOKAI_MATCH_TYPE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_JOIN_STATE_LIST_DATA)
	BYTE								byMatchType;			// eBUDOKAI_MATCH_TYPE

	BYTE								byDataCount;
	sBUDOKAI_JOIN_STATE_DATA			aJoinStateData[BUDOkAI_MAX_PLAYER_JOIN_STATE_DATA_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_SET_OPEN_TIME_RES)
	WORD								wResultCode;

	BYTE								byState;				// eBUDOKAI_STATE
	BUDOKAITIME							tmOpenTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_MATCH_REWARD_RES)
	WORD					wResultCode;

	HOBJECT					hPc;
	CHARACTERID				charId;
	BYTE					byReqType;
	DWORD					dwMudosaPoint;
	BYTE					byItemCount;
	sITEM_DATA				aItems[MAX_BUDOKAI_MATCH_REWARD_ITEM_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SCOUTER_ITEM_SELL_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	DWORD					dwZenny;
	ITEMID					itemId; 
	BYTE					byPlace;
	BYTE					byPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SHOP_EVENTITEM_BUY_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	HOBJECT					hNpchandle;
	CHARACTERID				charId;
	DWORD					dwMudosaPoint;
	BYTE					byBuyCount;
	ITEMID					itemID[NTL_MAX_BUY_SHOPPING_CART];
	sSHOP_BUY_INVEN			sInven[NTL_MAX_BUY_SHOPPING_CART];
	BYTE					byDeleteItemCount; 
	sITEM_DELETE_DATA		asDeleteItem[NTL_MAX_BUY_SHOPPING_CART];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SHOP_GAMBLE_BUY_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	HOBJECT					hNpchandle;
	CHARACTERID				charId;
	DWORD					dwMudosaPoint;
	sITEM_DATA				sInven;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_UPDATE_MUDOSA_POINT_RES)
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	DWORD					dwMudosaPoint;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_BUDOKAI_ADD_MUDOSA_POINT_RES)
HOBJECT					handle;
WORD					wResultCode;
CHARACTERID				charId;
DWORD					dwMudosaPoint;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_REPLACE_RES)
	WORD					wResultCode;
	BYTE					byItemProcessType;//eITEM_PROCESS_TYPE
	HOBJECT					handle;
	CHARACTERID				charId;
	sITEM_DELETE_DATA		sDeleteItem;
	sITEM_DATA				sCreateItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_UPDATE_MARKING_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
//	sMARKING				sMarking;
	BYTE					byContentsType;	// eMARKING_CONTENTS_TYPE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SKILL_ADD_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	TBLIDX					skillId;
	BYTE					byNewClass;
	DWORD					dwZenny;
	DWORD					dwSP;
	BYTE					bySkillFrom;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SKILL_UPGRADE_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	TBLIDX					oldSkillId;
	TBLIDX					newSkillId;
	BYTE					bySlot;
	bool					bIsRpBonusAuto;
	DWORD					dwSP;
	bool					bForced;
	bool					bForMax;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_HTB_SKILL_ADD_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	TBLIDX					skillId;
	int						nSkillTime;
	DWORD					dwZenny;
	DWORD					dwSP;
	bool					bTrainerCheck;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SKILL_BUY_RES)
WORD					wResultCode;
HOBJECT					handle;
CHARACTERID				charId;
HOBJECT					hNpchandle;
BYTE					bySellType;
TBLIDX					skillId;
BYTE					byNewClass;
DWORD					dwZenny;
DWORD					dwSP;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SKILL_INIT_RES)
WORD			wResultCode;
HOBJECT			handle;
CHARACTERID		charId;
BYTE			bySkillResetMethod; // 0 = init by npc / 1 = init by item / 2 = init by class change
ITEMID			itemId;
DWORD			dwSP;
DWORD			dwZeni;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_RECIPE_REG_RES)
WORD					wResultCode;
HOBJECT					handle;
CHARACTERID				charId;
BYTE					byPlace;
BYTE					byPos;
ITEMID					itemId;
TBLIDX					recipeTblidx;
BYTE					byRecipeType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_HOIPOIMIX_JOB_SET_RES )
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	HOBJECT					hNpchandle;
	BYTE					byRecipeType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_HOIPOIMIX_JOB_RESET_RES )
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	HOBJECT					hNpchandle;
	BYTE					byRecipeType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_HOIPOIMIX_ITEM_MAKE_RES)			// 아이템 만들기
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	HOBJECT					objHandle;
	TBLIDX					recipeTblidx;
	DWORD					dwMixExp;				// 결과 EXp
	BYTE					byMixLevel;				// 결과 Level
	bool					bIsNew;
	DWORD					dwSpendZenny;
	sITEM_DATA				sCreateData;			// 생성 아이템
	DWORD					dwExpGained;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_STACK_UPDATE_RES)
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	BYTE					byPlace;
	BYTE					byPos;
	HOBJECT					hItem;
	BYTE					byStackCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_DOJO_BANK_HISTORY_RES)
	WORD							wResultCode;
	HOBJECT							handle;
	CHARACTERID						charId;
	BYTE							byCount;
	sDBO_DOJO_BANK_HISTORY_DATA		asData[DBO_MAX_COUNT_DOJO_BANK_HISTORY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_DOJO_BANK_ZENNY_UPDATE_RES)
	WORD					wResultCode;
	GUILDID					guildId;
	DWORD					dwZenny;
	bool					bIsSave;		// 1: 저금 0: 인출
	BYTE					byType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_WORLD_SCHEDULE_SET_RES )
	WORD					wResultCode;
	sSCHEDULE_INFO			sInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_SWITCH_CHILD_ADULT_RES )
	WORD					wResultCode;
	CHARACTERID				charId;
	HOBJECT					hSubject;
	bool					bIsAdult;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_SET_CHANGE_CLASS_AUTHORITY_RES )
	WORD					wResultCode;
	CHARACTERID				charId;
	HOBJECT					hSubject;
	bool					bCanChangeClass;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_ITEM_CHANGE_ATTRIBUTE_RES )
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charID;
	sITEM_DATA				sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CHANGE_NETP_RES )
	WORD wResultCode;
	CHARACTERID charId; 
	HOBJECT handle;
	NETP netP; 
	BYTE byUpdateType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_ITEM_CHANGE_DURATIONTIME_RES )				
	WORD			wResultCode;
	HOBJECT			handle;
	CHARACTERID		charId;
	sITEM_DATA		sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_SHOP_NETPYITEM_BUY_RES )
	HOBJECT					handle;
	WORD					wResultCode;
	CHARACTERID				charId;
	NETP					netpyPoint;
	BYTE					byBuyCount;
	ITEMID					itemID[NTL_MAX_BUY_SHOPPING_CART];
	sSHOP_BUY_INVEN			sInven[NTL_MAX_BUY_SHOPPING_CART];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_GM_COMMAND_LOG_RES )
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_DURATION_ITEM_BUY_RES )
	HOBJECT					handle;
	CHARACTERID				charId;
	WORD					wResultCode;
	BYTE					byPayType;
	DWORD					dwPayAmount;
	TBLIDX					merchantTblidx;
	BYTE					byPos;
	sITEM_DATA				sItem;		
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_DURATION_RENEW_RES )
	HOBJECT					handle;
	CHARACTERID				charId;
	WORD					wResultCode;
	BYTE					byPayType;
	DWORD					dwPayAmount;
	HOBJECT					hItemHandle;
	sITEM_DURATION			sData;		
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GQ_CASHITEM_HLSHOP_REFRESH_RES)
HOBJECT			handle;
CHARACTERID		charId;
ACCOUNTID		accountId;
WORD			wResultCode;
DWORD			dwRemainAmount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_CASHITEM_INFO)
HOBJECT					handle;
CHARACTERID				charId;
BYTE					byCount;
sCASHITEM_BRIEF			aInfo[DBO_MAX_CASHITEM_COUNT_IN_PACKET_SERVER];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CASHITEM_INFO_RES )
	HOBJECT					handle;
	CHARACTERID				charId;
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CASHITEM_MOVE_RES )
WORD					wResultCode;
HOBJECT					handle;
CHARACTERID				charId;
QWORD					qwProductId;
sSHOP_BUY_INVEN			sData;
ITEMID					itemId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CASHITEM_DEL_RES )
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	QWORD					qwProductId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CASHITEM_UNPACK_RES )
	WORD					wResultCode;
	HOBJECT					handle;
	CHARACTERID				charId;
	QWORD					qwProductId;
	BYTE					byCount;			// UnPack해서 넣어줄 아이템 숫자
	sCASHITEM_BRIEF			aInfo[1];			// UnPack해서 나올 CASHITEM
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_CASHITEM_BUY_RES )
WORD					wResultCode;
HOBJECT					handle;
ACCOUNTID				accountId;
CHARACTERID				characterId;
QWORD					qwProductId;
TBLIDX					HLSitemTblidx;
BYTE					byCount;			// UnPack해서 넣어줄 아이템 숫자
DWORD					dwPrice;
DWORD					dwRemainAmount; //cash remaining
sDBO_TIME				tRegTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QG_GMT_UPDATE_RES )
	WORD					wResultCode;
	ACCOUNTID				accountId;
	CHARACTERID				charId;
	HOBJECT					hPc;
	sGAME_MANIA_TIME_DATA	sUpdateData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_DATA_LOAD_RES)
	WORD						wResultCode;
	ACCOUNTID					accountId;
	sPC_DATA					sPcData;
	sDBO_SERVER_CHANGE_INFO		serverChangeInfo;
	bool						bTutorialFlag;
	sCHAR_WAR_FOG_FLAG			sWarFogInfo;
	sMAIL_NEW_BREIF				sMailBrief;
	sRANKBATTLE_SCORE_INFO		sRankBattleScore;
	BYTE						TitleIndexFlag[NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG];
	WORD						wWaguCoins;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_ITEM_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	WORD					wTotalCount;
	WORD					wTotalPacketCount;
	WORD					wCurPacketCount;
	bool					bHaveNetPosition;
	BYTE					byItemCount;
	sITEM_DATA				asItemData[DBO_ITEM_IN_BAG_COUNT_PER_PACKET]; // Max = NTL_MAX_COUNT_USER_HAVE_INVEN_ITEM
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_SKILL_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					bySkillCount;
	sSKILL_DATA				asSkill[NTL_MAX_PC_HAVE_SKILL];	// Max = NTL_MAX_PC_HAVE_SKILL
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_BUFF_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					byBuffCount;
	sBUFF_DATA				asBuff[DBO_MAX_BUFF_CHARACTER_HAS]; // Max = NTL_MAX_BLESS_BUFF_CHARACTER_HAS + NTL_MAX_CURSE_BUFF_CHARACTER_HAS
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_HTB_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					byHTBSkillCount;
	sHTB_SKILL_DATA			asHTBSkill[NTL_HTB_MAX_PC_HAVE_HTB_SKILL];	// Max = NTL_HTB_MAX_PC_HAVE_HTB_SKILL
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_QUEST_ITEM_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					byQuestInventoryCount;
	sQUEST_INVENTORY_DATA	asQuestInventory[NTL_QUEST_INVENTORY_SLOT_COUNT];	// Max = NTL_QUEST_INVENTORY_SLOT_COUNT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_QUEST_COMPLETE_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	sQUEST_COMPLETE_DATA	sQuestCompleteData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_QUEST_PROGRESS_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					byQuestProgressCount;
	sQUEST_PROGRESS_DATA	asQuestProgress[eMAX_CAN_PROGRESS_QUEST_NUM];	// Max = eMAX_CAN_PROGRESS_QUEST_NUM
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_QUICK_SLOT_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	BYTE					byQuickSlotCount;
	sQUICK_SLOT_DATA		asQuickSlotData[NTL_CHAR_QUICK_SLOT_MAX_COUNT];	// Max = NTL_CHAR_QUICK_SLOT_MAX_COUNT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_SHORTCUT_LOAD_RES)
WORD					wResultCode;
CHARACTERID				charID;
BYTE					byShortcutCount;
sSHORTCUT_DATA			asShortcutData[NTL_SHORTCUT_MAX_COUNT];	// Max = NTL_SHORTCUT_MAX_COUNT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_ITEM_RECIPE_LOAD_RES)
	WORD					wResultCode;
	CHARACTERID				charID;
	WORD					wRecipeCount;
	sRECIPE_DATA			asRecipeData[NTL_ITEM_RECIPE_MAX_COUNT];	// Max = NTL_ITEM_RECIPE_MAX_COUNT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_GMT_LOAD_RES)
	WORD					wResultCode;
	ACCOUNTID				accountId;
	sGAME_MANIA_TIME_DATA	sGMT_Current;
	sGAME_MANIA_TIME_DATA	sGMT_Next;
END_PROTOCOL()
//------------------------------------------------------------------

//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_TENKAICHIDAISIJYOU_SERVERDATA_RES)
	WORD						wResultCode;
	ACCOUNTID					accountId;
	BYTE						byCurPacketCount;
	unsigned int				uiPage;
	unsigned int				uiMaxPage;
	BYTE						byListNum;
	sTENKAICHIDAISIJYOU_DATA	sData[10];
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_TENKAICHIDAISIJYOU_PERIODEND_RES)
	WORD		wResultCode;
	QWORD		nItem;
	MAILID		mailId;
	BOOL		bList;
	CHARACTERID	charId;
	QWORD		itemId;
	DWORD		dwPrice;
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_TENKAICHIDAISIJYOU_SELL_CANCEL_RES)
	BYTE			byServerChannelIndex;
	BYTE			byServerIndex;
	CHARACTERID		charId;
	ACCOUNTID		accountId;
	WORD			wResultCode;
	QWORD			nItem;
//	MAILID			mailId;
//	ITEMID			itemID;
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_TENKAICHIDAISIJYOU_BUY_RES)
	BYTE			byServerChannelIndex;
	BYTE			byServerIndex;
	CHARACTERID		sellerCharId;
	ACCOUNTID		accountId;
	WORD			wResultCode;
	QWORD			nItem;
	DWORD			dwMoney;
//	MAILID			mailId;
//	ITEMID			itemID;
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_HLS_SLOT_MACHINE_RELOAD_RES)
	WORD					wResultCode;
	sHLS_SLOT_MACHINE_DATA	sData;
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_HLS_SLOT_MACHINE_EXTRACT_RES)
	WORD		wResultCode[10];
	CHARACTERID	charId;
	ACCOUNTID	accountId;
	BYTE		byHlsMachineType;
	WORD		wMachineIndex;
	TBLIDX		ItemTblidx[10];
	BYTE		byStackCount[10];
	BYTE		bySetCount[10];
	BYTE		byQnttIndex[10];
	bool		bWinner[10];
	WORD		wWinCount;
	DBOTIME		nExtractTime;
	int			nWinnerIndex;
	BYTE		byExtractCount;
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_HLS_SLOT_MACHINE_WINNER_LIST_RES)
	WORD		wMachineIndex;
	BYTE		byInfoCount;
	WCHAR		wszPlayer[3][NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD		wWinCount[3];
	DBOTIME		nExtractTime[3];
	int			nWinnerIndex[3];
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_INIT_WAGU_MACHINE_QNTT_RES)
	ACCOUNTID	gmAccountId;
	WORD		wResultCode;
	WORD		wMachineIndex;
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_HLS_SLOT_MACHINE_LIST_RES)
	WORD						wResultCode;
	BYTE						byCount;
	sHLS_SLOT_MACHINE_DATA		sData[81];
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_MAIL_START_INFO)
	CHARACTERID		charID;
	BYTE			byTotalCount;
	BYTE			byCurPacketCount;
	BYTE			byCount;
	sMAIL_DATA		asData[NTL_MAX_LOAD_MAIL_PACKET];
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_MAIL_LOAD_INFO)
	CHARACTERID		charID;
	BYTE			byTotalCount;
	BYTE			byCurPacketCount;
	BYTE			byCount;
	sMAIL_DATA		asData[NTL_MAX_LOAD_MAIL_PACKET];
END_PROTOCOL()
//------------------------------------------------------------------


//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_QUICK_TELEPORT_INFO_LOAD_RES)
	CHARACTERID		charId;
	WORD			wResultCode;
	BYTE			byPlace;
	BYTE			byPos;
	BYTE			byCount;
	sQUICK_TELEPORT_INFO	asData[NTL_QUICK_PORTAL_MAX_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_QUICK_TELEPORT_UPDATE_RES)
	CHARACTERID			charId;
	WORD				wResultCode;
	BYTE				byPlace;
	BYTE				byPos;
	sQUICK_TELEPORT_INFO	asData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_QUICK_TELEPORT_DEL_RES)
	CHARACTERID		charId;
	WORD			wResultCode;
	BYTE			byPlace;
	BYTE			byPos;
	BYTE			bySlot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_QUICK_TELEPORT_USE_RES)
	CHARACTERID		charId;
	WORD			wResultCode;
	BYTE			bySlot;
	sSHOP_SELL_INVEN	sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_ITEM_COOL_TIME_LOAD_RES)
  WORD						wResultCode;
  CHARACTERID				charID;
  BYTE						byItemCoolTimeCount;
  sDBO_ITEM_COOL_TIME		asItemCoolTimeData[NTL_MAX_ITEM_COOL_DOWN];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_SAVE_ITEM_COOL_TIME_DATA_RES)
	WORD		wResultCode;
	HOBJECT		hSubject;
	CHARACTERID	charId;
	END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_CASHINFOREFRESH_RES)
	ACCOUNTID			accountId;
	CHARACTERID			charId;
	DWORD				dwRemainCash;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_CASHITEM_SEND_GIFT_RES)
WORD			wResultCode;
CHARACTERID		SenderCharId;
WCHAR			wchSenderName[NTL_MAX_SIZE_USERID + 1];
ACCOUNTID		DestAccountId; 
WCHAR			wchDestName[NTL_MAX_SIZE_USERID + 1];
SERVERFARMID	DestServerFarmId;
TBLIDX			dwIdxHlsTable;
QWORD			qwProductId;
BYTE			byCount;
DWORD			dwPrice;
DWORD			dwRemainAmount;
sDBO_TIME		tRegTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_MASCOT_LOAD_RES)
WORD				wResultCode;
CHARACTERID			charID;
BYTE				byMascotCount;
sMASCOT_DATA_EX		asMascotData[GMT_MAX_PC_HAVE_MASCOT];
END_PROTOCOL()
//------------------------------------------------------------------



//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_PC_DATA_LOAD_STRESSTEST_RES)
WORD						wResultCode;
ACCOUNTID					accountId;
CHARACTERID					charId;
sPC_DATA					sPcData;
sDBO_SERVER_CHANGE_INFO		serverChangeInfo;
bool						bTutorialFlag;
sCHAR_WAR_FOG_FLAG			sWarFogInfo;
sMAIL_NEW_BREIF				sMailBrief;
sRANKBATTLE_SCORE_INFO		sRankBattleScore;
BYTE						byGMTResetCount;
BYTE						byPunishCount;
unsigned char				TitleIndexFlag[NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG];
int							randid;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_MATERIAL_DISASSEMBLE_RES)
WORD				wResultCode;
HOBJECT				handle;
CHARACTERID			charId;
sITEM_DELETE_DATA	sDeleteItem;
BYTE				byCount;
sITEM_DATA			asCreateItem[ITEM_DISASSEMBLE_MAX_RESULT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_SOCKET_INSERT_BEAD_RES)
WORD				wResultCode;
HOBJECT				handle;
CHARACTERID			charId;
BYTE				byItemPlace;
BYTE				byItemPos;
ITEMID				ItemId;
BYTE				byRestrictState;
BYTE				byDurationType;
BYTE				byBeadPlace;
BYTE				byBeadPos;
ITEMID				BeadItemId;
BYTE				byBeadRemainStack;
sITEM_RANDOM_OPTION	sItemRandomOption[NTL_MAX_OPTION_IN_ITEM];
DBOTIME				nUseStartTime;
DBOTIME				nUseEndTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_SOCKET_DESTROY_BEAD_RES)
WORD				wResultCode;
HOBJECT				handle;
CHARACTERID			charId;
BYTE				byItemPlace;
BYTE				byItemPos;
ITEMID				ItemId;
BYTE				byRestrictState;
BYTE				byDurationType;
bool				bTimeOut;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_CASHITEM_PUBLIC_BANK_ADD_RES)
HOBJECT			handle;
WORD			wResultCode;
TBLIDX			hlsItemNo;
TBLIDX			itemNo;
ACCOUNTID		accountId;
CHARACTERID		charId;
ITEMID			dwProductID;
ITEMID			itemID;
BYTE			byPlace;
BYTE			byPosition;
BYTE			byRank;
BYTE			byDurationType;
DBOTIME			nUseStartTime;
DBOTIME			nUseEndTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_EXCHANGE_RES)
HOBJECT				handle;
CHARACTERID			charId;
DWORD				dwZenny;
DWORD				dwMudosapoint;
WORD				wResultCode;
BYTE				byReplaceCount;
BYTE				byCreateCount;
sITEM_DATA			aCreateItem[ITEM_BULK_CREATE_COUNT]; // ITEM_BULK_CREATE_COUNT
BYTE				byUpdateCount;
sITEM_BASIC_DATA	aUpdateItem[ITEM_BULK_UPDATE_COUNT]; // ITEM_BULK_UPDATE_COUNT
BYTE				byDeleteCount;
sITEM_DELETE_DATA	aDeleteItem[ITEM_BULK_DELETE_COUNT]; // ITEM_BULK_DELETE_COUNT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_CHARACTER_RENAME_RES)
WORD			wResultCode;
HOBJECT			handle;
CHARACTERID		charId;
WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
BYTE			byPlace;
BYTE			byPos;
ITEMID			itemId;
DWORD			dwRemainMin;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_CHANGE_OPTION_RES)
WORD				wResultCode;
HOBJECT				handle;
CHARACTERID			charId;
BYTE				byItemPlace;
BYTE				byItemPos;
ITEMID				ItemId;
BYTE				byKitPlace;
BYTE				byKitPos;
ITEMID				KitItemId;
BYTE				byKitStack;
sITEM_OPTION_SET	sItemOptionSet;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_WAGUWAGUMACHINE_COIN_INCREASE_RES)
WORD				wResultCode;
HOBJECT				handle;
ACCOUNTID			accountId;
CHARACTERID			characterId;
ITEMID				qwProductId;
WORD				wWaguCoin;
WORD				wTotalWaguCoin;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES)
TBLIDX			tblidx;
DWORD			dwTotalCount;
DWORD			dwSettingCount;
DBOTIME			tWpsEndTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_SEAL_RES)
WORD					wResultCode;
HOBJECT					handle;
CHARACTERID				charId;
BYTE					byItemPlace;
BYTE					byItemPos;
ITEMID					itemId;
BYTE					byRestrictState;
BYTE					bySealPlace;
BYTE					bySealPos;
ITEMID					SealitemId;
BYTE					bySealStack;
BYTE					bySealRemainStack;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_SEAL_EXTRACT_RES)
WORD					wResultCode;
HOBJECT					handle;
CHARACTERID				charId;
BYTE					byItemPlace;
BYTE					byItemPos;
ITEMID					itemId;
BYTE					byRestrictState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_DRAGONBALL_SCRAMBLE_END_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_UPGRADE_BY_COUPON_RES)
WORD		wResultCode;
HOBJECT		handle;
CHARACTERID	charId;
ITEMID		itemId;
ITEMID		couponId;
BYTE		byGrade;
BYTE		byItemPlace;
BYTE		byItemPos;
BYTE		byCouponPlace;
BYTE		byCouponPos;
BYTE		byStack;
BYTE		byDurationType;
DBOTIME		nUseStartTime;
DBOTIME		nUseEndTime;
BYTE		byRestrictState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_ITEM_GRADEINIT_BY_COUPON_RES)
WORD		wResultCode;
HOBJECT		handle;
CHARACTERID	charId;
ITEMID		itemId;
BYTE		byItemPlace;
BYTE		byItemPos;
BYTE		byRestrictState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_EVENT_REWARD_LOAD_INFO)
HOBJECT				handle;
CHARACTERID			charId;
BYTE				byCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_EVENT_REWARD_LOAD_RES)
WORD				wResultCode;
HOBJECT				handle;
CHARACTERID			charId;
BYTE				byCount;
sEVENT_PC_NAME		asNameInfo[NTL_MAX_EVENT_REWARD_COUNT_IN_PACKET];
sEVENT_REWARD_INFO	asInfo[NTL_MAX_EVENT_REWARD_COUNT_IN_PACKET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_EVENT_REWARD_SELECT_RES)
WORD				wResultCode;
HOBJECT				handle;
CHARACTERID			charId;
TBLIDX				eventTblidx;
sITEM_DATA			sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_MASCOT_SEAL_SET_RES)
WORD					wResultCode;
HOBJECT					handle;
CHARACTERID				charId;
BYTE					byMascotPos;
BYTE					bySealPlace;
BYTE					bySealPos;
ITEMID					SealitemId;
BYTE					bySealStack;
sITEM_DATA				sNewItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QG_MASCOT_SEAL_CLEAR_RES)
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack(pop)
