#pragma once

// Auth Server 100~199
// Character Server 200~299
// Community Server 300~399
// Query Server 400~
// Game Server 500~
// Petition System 10000~


enum eRESULTCODE
{
	//-----------------------------------------------------------------
	// Base ( 0 ~ 100 )
	//-----------------------------------------------------------------
	RESULT_SUCCESS = 0,
	RESULT_FAIL,					// 전반적인 실패
	
	RESULT_LAST,					//always last
	//-----------------------------------------------------------------


	//-----------------------------------------------------------------
	// Auth ( 100 ~ 199 )
	//-----------------------------------------------------------------
	AUTH_SUCCESS = 100,				// 인증서버상의 모든 성공
	AUTH_FAIL,						// 인증서버상의 전반적인 실패
	AUTH_VERSION_FAIL,              // 버젼 불일치
	AUTH_TOO_LONG_ACCOUNT,          // 계정 이름의 길이가 초과되었습니다. 
	AUTH_TOO_LONG_PASSWORD,         // 패스워드의 길이가 초과되었습니다. 
	AUTH_AUTHKEY_FAIL,              // 인증키 인증이 실패하였습니다.
	AUTH_USER_SAMENAME_EXIST,       // 이미 계정명이 존재합니다. 
	AUTH_WRONG_PASSWORD,            // 패스워드가 틀립니다. 
	AUTH_USER_NOT_FOUND,            // 계정명이 존재하지 않습니다.
	AUTH_USER_EXIST,                // 이미 접속중입니다.
	AUTH_USER_EXIST_IN_CHARACTER_SERVER,		// 캐릭터 서버에 이미 접속중입니다.
	AUTH_DB_FAIL,					// DB 로드 에러
	AUTH_NO_AVAILABLE_CHARACTER_SERVER,			// 현재 접속 가능한 캐릭터 서버가 없습니다.
	AUTH_USER_BLOCK,				// 계정이 블록 되었습니다. 자세한 사항은 홈페이지를 문의하십시요.
	AUTH_USER_TEMPORARY_BLOCK,		// 운영작업중이라 접속하실 수 없습니다. 잠시 후 접속을 시도해보십시요.
	AUTH_SERVER_LOCKED,							// 현재 서버가 잠긴 상태여서 접속할 수 없습니다.
	AUTH_COMMON_CANT_DO_THAT_FOR_SOME_REASON,			// 시스템 문제로 작업을 처리할 수 없습니다.
	AUTH_IP_BLOCK,					// 블록된 IP 입니다. (로비에 나오는 메시지. 확인 버튼)

	AUTH_RESULT_LAST,			// always last
	//-----------------------------------------------------------------


	//-----------------------------------------------------------------
	// Character ( 200 ~ 300 )
	//-----------------------------------------------------------------
	CHARACTER_SUCCESS = 200,		// 캐릭터 서버상의 모든 성공
	CHARACTER_FAIL,                 // 캐릭터 서버상의 전반적인 실패
	CHARACTER_AUTHKEY_FAIL,         // 인증키 인증이 실패하였습니다.
	CHARACTER_TOO_LONG_NAME,		// 캐릭터명의 길이가 초과되었습니다.
	CHARACTER_TOO_SHORT_NAME,		// 캐릭터명의 길이가 너무 짧습니다.
	CHARACTER_SAMENAME_EXIST,		// 이미 캐릭터명이 존재합니다.
	CHARACTER_COUNT_OVER,           // 캐릭터 생성갯수가 초과되었습니다.
	CHARACTER_NAME_NOT_EXIST,       // 캐릭터가 존재하지 않습니다.
	CHARACTER_CREATE_VALUE_FAIL,	// 캐릭터 생성 값이 정확하지 않습니다. 
	CHARACTER_USER_EXIST,           // 이미 접속중입니다.
	CHARACTER_USER_EXIST_IN_GAME_SERVER,		// 게임 서버에 이미 플레이중인 캐릭터가 있습니다.
	CHARACTER_USER_ITEM_CREATE_FAIL,// 캐릭터 아이템 생성이 실패하였습니다. 
	CHARACTER_USER_SKILL_CREATE_FAIL,			// 캐릭터 스킬 생성이 실패하였습니다.
	CHARACTER_USER_QUICK_SLOT_CREATE_FAIL,		// 캐릭터 퀵슬롯을 초기화하지 못했습니다.
	CHARACTER_DB_QUERY_FAIL, // 캐릭터 서버에서 DB 쿼리가 실패하였습니다.
	CHARACTER_WRONG_SERVER_FARM_SELECTED,		// 잘못된 서버를 선택하였습니다.
	CHARACTER_USER_BLOCK,			// 캐릭터가 블록 되었습니다. 자세한 사항은 홈페이지를 문의하십시요.
	CHARACTER_BLOCK_STRING_INCLUDED, // 부적합한 스트링이 포함되어 있습니다.
	CHARACTER_TEMPORARY_BLOCK,		// 운영작업중이라 접속하실 수 없습니다. 잠시 후 접속을 시도해보십시요.
	CHARACTER_AUTHKEY_MISMATCH,
	CHARACTER_AUTHKEY_CREATE_FAIL,
	CHARACTER_AUTHKEY_NOT_FOUND,
	CHARACTER_DELETE_GUILD_LEADER_FAIL,			// 길드리더는 캐릭을 지울 수 없습니다.
	CHARACTER_DELETE_BUDOKAI_REGISTER_FAIL,		// 천하제일무도회 신청중 일때는 캐릭터를 삭제할 수 없습니다.
	CHARACTER_SERVER_LOCKED,					// 현재 서버가 잠긴 상태여서 접속할 수 없습니다.
	CHARACTER_DISCONNECTED_FORCEDLY,			// 제재로 인해 강제로 접속이 종료되었습니다.
	CHARACTER_DISCONNECTED_TEMPORARILY,			// 운영자에 의하여 일시적으로 로그아웃 처리가 되었습니다.
	CHARACTER_RACE_NOT_ALLOWED,					// 허용되지 않은 종족 입니다.

	CHARACTER_COMMON_CANT_DO_THAT_FOR_SOME_REASON,			// 시스템 문제로 작업을 처리할 수 없습니다.
	CHARACTER_USER_SHOULD_WAIT_FOR_CONNECT,
	CHARACTER_SERVER_IS_UNDER_CONSTRUCTION,					// 서버 점검 중 입니다.
	CHARACTER_DELETE_CHAR_FAIL,					// 캐릭터 삭제에 실패했습니다.

	//new
	CHARACTER_NO_PREMIUM_SLOT,
	CHARACTER_DELETE_CHAR_FAIL_NOT_MATCH_CODE,
	CHARACTER_COUNT_OVER_AND_DELETE_NFY,
	CHARACTER_DELETE_CHAR_TMP_FAIL,


	CHAR_RESULT_LAST,							// always last

	//-----------------------------------------------------------------


	//-----------------------------------------------------------------
	// Community ( 300 ~ 400 )
	//-----------------------------------------------------------------
	CHAT_SUCCESS = 300,				// 커뮤니티 서버 상의 모든 성공
	COMMUNITY_FAIL,												// 커뮤니티 서버 상의 전반적인 실패

	COMMUNITY_INVALID_AUTH_KEY,									// 인증키 인증이 실패하였습니다.
	COMMUNITY_ALREADY_IN_SERVER,								// 이미 접속중입니다.  
	COMMUNITY_DISCONNECTED_FORCEDLY,							// 제재로 인해 강제로 접속이 종료되었습니다.
	COMMUNITY_DISCONNECTED_TEMPORARILY,							// 운영자에 의하여 일시적으로 로그아웃 처리가 되었습니다.
	COMMUNITY_COMMON_SYSTEM_ERROR,								// 커뮤니티 시스템 에러입니다.
	COMMUNITY_COMMON_NO_SUCH_A_TARGET,							// 대상이 존재하지 않습니다.
	COMMUNITY_COMMON_TARGET_IS_NOT_YOUR_GUILD_MEMBER,			// 대상이 당신의 길드에 속해 있지 않습니다.
	COMMUNITY_COMMON_NEED_MORE_TIME_TO_DO_IT,					// 잠시 후에 다시 시도하십시오.
	COMMUNITY_PARTY_LEADER_DOESNT_EXIST,						// 리더가 커뮤니티 서버에 존재하지 않습니다.
	COMMUNITY_PARTY_SAME_PARTYID_ALREADY_EXIST,					// 같은 ID의 파티가 이미 존재합니다.
	COMMUNITY_PARTY_NO_SUCH_A_PARTY,							// 존재하지 않는 파티입니다.
	COMMUNITY_PARTY_MEMBER_ALREADY_EXIST,						// 이미 파티에 존재하는 멤버입니다.
	COMMUNITY_PARTY_NO_SUCH_A_MEMBER,							// 존재하지 않는 멤버입니다.
	COMMUNITY_GUILD_CREATOR_NOT_FOUND,							// 길드 생성을 요청한 플레이어를 찾을 수 없습니다.
	COMMUNITY_GUILD_GUILD_MEMBER_CANT_HAVE_MORE_GUILD,			// 길드에 가입한 사람은 다른 길드에 중복해서 가입할 수 없습니다.
	COMMUNITY_GUILD_NEED_MORE_PARTY_MEMBER_FOR_NEW_GUILD,		// 길드 생성을 위해서는 파티원이 더 필요합니다.
	COMMUNITY_GUILD_YOU_DONT_BELONG_TO_GUILD,					// 길드에 속해 있지 않습니다.
	COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER,					// 당신은 길드 마스터가 아닙니다.
	COMMUNITY_GUILD_GUILD_IS_BEING_DISBANDED,					// 길드가 해체 상태입니다.
	COMMUNITY_GUILD_GUILD_IS_NOT_BEING_DISBANDED,				// 길드가 해체 상태가 아닙니다.
	COMMUNITY_GUILD_YOU_DONT_HAVE_RIGHT_TO_INVITE,				// 길드원을 초대할 수 있는 권한이 없습니다.
	COMMUNITY_GUILD_TARGET_IS_ALREADY_IN_A_GUILD,				// 상대가 이미 길드에 속해 있습니다.
	COMMUNITY_GUILD_TARGET_ALREADY_HAS_INVITATION,				// 상대방이 이미 길드 초대를 받은 상태입니다.
	COMMUNITY_GUILD_YOU_HAVE_NO_INVITATION,						// 당신은 길드로부터 초대를 받은 상태가 아닙니다.
	COMMUNITY_COMMON_NO_SUCH_A_GUILD,							// 길드가 존재하지 않습니다.
	COMMUNITY_GUILD_YOU_ARE_ALREADY_IN_A_GUILD,					// 이미 길드에 속해 있습니다.
	COMMUNITY_GUILD_MAX_NUMBER_OF_MEMBERS,						// 길드는 더 이상 새 길드원을 받을 수 없습니다.
	COMMUNITY_GUILD_LEADER_CANT_LEAVE,							// 길드 마스터는 길드를 탈퇴할 수 없습니다.
	COMMUNITY_GUILD_YOU_DONT_HAVE_RIGHT_TO_KICK_OUT,			// 길드원을 강제로 탈퇴시킬 수 있는 권한이 없습니다.
	COMMUNITY_GUILD_CANT_KICK_YOURSELF_OUT,						// 자신을 강제로 탈퇴시킬 수 없습니다.
	COMMUNITY_GUILD_TRIED_TO_APPOINT_YOURSELF,					// 자기 자신에 대해서는 임명 기능을 사용할 수 없습니다.
	COMMUNITY_GUILD_TARGET_IS_ALREADY_SECOND_MASTER,			// 대상이 이미 Second Master입니다.
	COMMUNITY_GUILD_MAX_NUMBER_OF_SECOND_MASTERS,				// 길드는 더 이상 새 Second Master를 임명할 수 없습니다.
	COMMUNITY_GUILD_TARGET_IS_NOT_SECOND_MASTER,				// 대상이 길드의 Second Master가 아닙니다.
	COMMUNITY_GUILD_FUNCTION_NOT_FOUND,							// 해당 기능을 사용 할 수 있는 권한이 없습니다.
	COMMUNITY_GUILD_BANK_USING_NOW,								// 길드 창고를 다른 사람이 사용 중 입니다. 조금 뒤에 다시 시도 해주세요.	
	COMMUNITY_FRIEND_CHAR_NOT_FOUND,							// 해당캐릭터가 존재하지 않습니다.
	COMMUNITY_FRIEND_CHAR_ARLEADY_ADDED,						// 이미 등록 된 캐릭입니다.
	COMMUNITY_FRIEND_MAX_COUNT_OVER,							// 등록가능 친구 갯수를 초과 했습니다.
	COMMUNITY_FRIEND_CHAR_EXIST,								// 친구목록에 존재하는 캐릭입니다.
	COMMUNITY_FRIEND_BLACK_CHAR_EXIST,							// Black List 목록에 존재하는 캐릭입니다.
	COMMUNITY_FRIEND_SELF_FAIL,									// 자기 자신은 추가 할수 없습니다.
	COMMUNITY_RANK_BATTLE_LIST_NOT_FOUND,						// 랭크배틀 리스트에 없습니다.
	COMMUNITY_GUILD_BANK_QUERY,									// 길드 창고의 DB 처리중입니다. 잠시후 다시 시도하세요!
	COMMUNITY_GUILD_BANK_CLOSED,								// 길드 창고가 닫혔습니다.
	COMMUNITY_GUILD_KICKOUT,									// 길드에서 추방 당했습니다.
	COMMUNITY_GUILD_LEAVE,										// 길드에서 탈퇴 했습니다.
	COMMUNITY_TMQ_CHAR_NOT_FOUND,								// 해당 캐릭터가 존재하지 않습니다.
	COMMUNITY_TMQ_DATA_NOT_FOUND,								// 해당 데이터가 존재하지 않습니다.
	COMMUNITY_DOJO_MAKE_FAIL,									// 도장 만들기에 실패했습니다.
	COMMUNITY_DOJO_DEL_FAIL,									// 도장 삭제에 실패했습니다.
	COMMUNITY_DOJO_FIND_FAIL,									// 도장 찾기에 실패했습니다.
	COMMUNITY_DOJO_SCRAMBLE_ARLEADY_HAVE,						// 도장 쟁탈전 신청자가 이미 있습니다.
	COMMUNITY_DOJO_SCRAMBLE_WAIT,								// 도장 쟁탈전 신청을 잠시후 다시 시도해주십시요.
	COMMUNITY_DOJO_SCRAMBLE_REJECT_FAIL_FOR_BANK_USE,			// 창고이용중에 도장 쟁탈전 신청거부가 불가능합니다.  
	COMMUNITY_DOJO_SCRAMBLE_REJECT_FAIL_SECOND,					// 도장 쟁탈전 거부는 한번만 가능합니다. 
	COMMUNITY_DOJO_SEED_ALREADY_SETTING,						// 이미 천하제일무도회 시드값등록이 되어있습니다. 삭제후 다시 등록하세요.
	COMMUNITY_DOJO_SEED_NAME_FAIL,								// 해당 캐릭이 없거나 잘못되었습니다.
	COMMUNITY_DOJO_SEED_NOT_SET,								// 천하제일무도회 시드값등록이 되어있지 않습니다.
	COMMUNITY_DOJO_NON_EXISTING_FUNCTION,						// 해당 권한이 없습니다. 
	COMMUNITY_DOJO_YOU_ARE_NOT_OWNNER,							// 도장에 속해 있지 않습니다.
	COMMUNITY_GUILD_FIND_FAIL,									// 길드 찾기에 실패했습니다.
	COMMUNITY_DOJO_BANK_ZENNY_NOT_ENOUGH,						// 창고 제니가 부족합니다.
	COMMUNITY_GUILD_DISBANDE_FAIL_FOR_DOJO,						// 도장에 속한 길드는 삭제할 수 없습니다.
	COMMUNITY_GUILD_DISBANDE_FAIL_FOR_DOJO_SCRAMBLE,			// 도장 쟁탈전중에는 삭제할 수 없습니다.
	COMMUNITY_DOJO_SCRAMBLE_FAIL_FOR_BEING_DISBANDED,			// 길드가 해체 상태에선 도장전을 할 수 없습니다.
	COMMUNITY_GUILD_INVITE_FAIL_FOR_BEING_SCRAMBLE,
	COMMUNITY_RANK_BATTLE_NO_REMAIN_COUNT,
	COMMUNITY_RANK_BATTLE_COUNT_ALREADY_FLUSHED,
	COMMUNITY_TMQ_COUNT_ALREADY_FLUSHED,
	COMMUNITY_COMMON_NOT_VALID_TARGET,


	CHAT_RESULT_LAST,							// always last


	//-----------------------------------------------------------------
	// Query ( 400 ~ 500 )
	//-----------------------------------------------------------------
	QUERY_SUCCESS = 400,			// 쿼리서버 상의 모든 성공
	QUERY_FAIL,						// 쿼리서버 상의 전반적인 실패

	QUERY_CANNOT_FIND_SQLUNIT,		// Can not find SQL Unit
	QUERY_EXECUTE_FAIL,				// 쿼리 실행 실패
	QUERY_STORE_FAIL,				// 쿼리 실행 실패
	

	//-----------------------------------------------------------------


	//-----------------------------------------------------------------
	// Game ( 500 ~ )
	//-----------------------------------------------------------------
	GAME_SUCCESS = 500,				// 게임서버 상의 모든 성공
	GAME_FAIL,						// 게임서버 상의 전반적인 실패

	GAME_DB_CHAR_DATA_LOAD_FAIL,	// 캐릭터 데이타 로딩에 실패하였습니다.
	GAME_DB_CHAR_NOT_FOUND,			// 해당하는 캐릭타 데이타가 존재하지 않습니다.

	GAME_INVALID_AUTH_KEY,			// 인증키 인증이 실패하였습니다.
	GAME_USER_EXIST,				// 이미 접속중입니다.
	GAME_SERVER_LOCKED,				// 현재 서버가 잠긴 상태여서 접속할 수 없습니다.
	GAME_WRONG_SERVER_CHANNEL_HAS_BEEN_SPECIFIED,			// 잘못된 서버 채널을 선택하였습니다.

	GAME_CHAR_IS_WRONG_STATE,		//I can not do it now
	GAME_INPUT_PARAMETER_WRONG,		// Invalid input.

	GAME_TARGET_IS_WRONG_STATE,		// Target is not in proper state.
	GAME_TARGET_NOT_FOUND,			// 대상을 찾을 수 없습니다.
	GAME_TARGET_TOO_FAR,			// 대상과의 거리가 너무 멉니다.
	GAME_TARGET_TOO_CLOSE,			// 대상과의 거리가 너무 가깝습니다.
	GAME_TARGET_HAS_NOT_FUNCTION,	// 대상은 해당 기능을 지원하지 않습니다.

	GAME_DISCONNECTED_FORCEDLY,							// 제재로 인해 강제로 접속이 종료되었습니다.
	GAME_DISCONNECTED_TEMPORARILY,						// 운영자에 의하여 일시적으로 로그아웃 처리가 되었습니다.

	GAME_COMMON_YOU_ARE_NOT_IN_A_PARTY,					// 파티에 속해 있지 않습니다.
	GAME_COMMON_YOU_ARE_NOT_A_PARTY_LEADER,				// 파티 리더가 아닙니다.
	GAME_COMMON_ZENNY_CANT_BE_USED_RIGHT_NOW,			// 일시적으로 제니를 사용할 수 없는 상황입니다. 잠시 후 다시 시도하세요.
	GAME_COMMON_TEMPORARY_,
	GAME_COMMON_CANT_DO_THAT_FOR_SOME_REASON,			// 시스템 문제로 작업을 처리할 수 없습니다.
	GAME_COMMON_NON_EXISTING_PET_SPECIFIED,				// 존재하지 않는 펫을 지정하였습니다.

	GAME_COMMON_CANT_DO_THAT_IN_SPAWNING_STATE,				// 월드 진입 도중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_STANDING_STATE,				// 서 있을 때에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_STANDING_JUMPING_STATE,		// 제자리 점프 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_JUMPING_STATE,				// 점프 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_NON_JUMPING_STATE,			// 점프 중이 아닐 때에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_SITTING_STATE,				// 앉아 있을 때에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_FAINTING_STATE,				// 실신한 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_CAMPING_STATE,				// 캠핑 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_LEAVING_STATE,				// 리빙 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_MOVING_STATE,				// 이동 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_MOVING_JUMPING_STATE,		// 점프 이동 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_DESTMOVING_STATE,			// 이동 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_FOLLOWING_STATE,			// 이동 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_FALLING_STATE,				// 떨어지는 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_DASH_STATE,					// 대쉬 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_TELEPORTING_STATE,			// 텔레포트 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_SLIDING_STATE,				// 슬라이딩 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_KNOCKDOWN_STATE,			// This command is not available during knockdown.
	GAME_COMMON_CANT_DO_THAT_IN_FOCUSING_STATE,				// 집중 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_CASTING_STATE,				// During casting the skill is not available commands.
	GAME_COMMON_CANT_DO_THAT_IN_AFFECTING_STATE,			// During casting the skill is not available commands.
	GAME_COMMON_CANT_DO_THAT_IN_KEEPING_EFFECT_STATE,		// The effect of maintaining the status command is not available.
	GAME_COMMON_CANT_DO_THAT_IN_CASTING_ITEM_STATE,			// Some items are not available, use the command.
	GAME_COMMON_CANT_DO_THAT_IN_STUNNED_STATE,				// 기절 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_SLEEPING_STATE,				// 수면 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_PARALYZED_STATE,			// 마비 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_HTB_STATE,					// HTB 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_SANDBAG_STATE,				// HTB 스킬에 걸린 상태에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_CHARGING_STATE,				// 기를 모으는 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_BLOCKING_STATE,				// Block 모드에서는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_DESPAWNING_STATE,			// 월드에서 벗어나는 도중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_PRIVATESHOP_STATE,			// 개인 상점을 열고 있을 때에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_DIRECT_PLAY_STATE,			// 연출 중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_OPERATING_STATE,			// 오브젝트 동작중에는 사용할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_RIDEON_STATE,				// The command can not ride a bullet in the state.
	GAME_COMMON_CANT_DO_THAT_IN_TURNING_STATE,				// In turn, the state is unable to command.

	GAME_COMMON_CANT_DO_THAT_IN_INVISIBLE_CONDITION,		// 투명 상태에서는 사용 할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_HIDING_KI_CONDITION,		// 기 숨기기 상태에서는 사용 할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_TRANSPARENT_CONDITION,		// GM 투명 상태에서는 사용 할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_TERROR_CONDITION,			// 공포에 걸린 상태에서는 사용 할 수 없는 명령입니다.

	GAME_COMMON_CANT_DO_THAT_IN_CONFUSE_CONDITION,			//new 562

	GAME_COMMON_CANT_DO_THAT_IN_AFTEREFFECT_CONDITION,		// 후유증 상태에서는 사용 할 수 없는 명령입니다.
	GAME_COMMON_CANT_DO_THAT_IN_CHARGING_BLOCKED_CONDITION,		// 기 모으기가 불가능한 상태입니다.

	GAME_GAMERULE_REG_CANT_PLAY_IN_FREEBATTLE,				// During the battle can not be free.
	GAME_GAMERULE_REG_CANT_PLAY_IN_RANKBATTLE,				// I can not rank during the battle.
	GAME_GAMERULE_REG_CANT_PLAY_IN_TMQ,						// Some quests can not be a time machine.
	GAME_GAMERULE_REG_CANT_PLAY_IN_SUMMON_DRAGONBALL,		// Dragon can not be summoned during the ball.
	GAME_GAMERULE_REG_CANT_PLAY_IN_PRIVATE_SHOP,			// 개인상정 중에는 할 수 없습니다.
	GAME_GAMERULE_REG_CANT_PLAY_IN_BUDOKAI,					// 천하제일무도회 중에는 할 수 없습니다.

	GAME_GAMERULE_REG_ALREADY_JOINED_IN_FREEBATTLE,			// 이미 프리배틀 참가 중입니다.
	GAME_GAMERULE_REG_ALREADY_JOINED_IN_RANKBATTLE,			// 이미 랭크배틀 참가 중입니다.
	GAME_GAMERULE_REG_ALREADY_JOINED_IN_TMQ,				// 이미 타임머신 퀘스트 참가 중입니다.
	GAME_GAMERULE_REG_ALREADY_SUMMON_DRAGONBALL,			// 이미 드래곤볼 소환 중입니다.
	GAME_GAMERULE_REG_ALREADY_OPENED_PRIVATE_SHOP,			// 이미 개인상점 개설 중입니다.
	GAME_GAMERULE_REG_ALREADY_JOINED_BUDOKAI,				// 이미 천하제일무도회 참가 중입니다.

	GAME_CONTENTS_LOCK,										// 현재 잠겨있는 컨텐츠입니다.
	GAME_CONTENTS_CAN_NOT_PLAY_IN_BUDOKAI_REGISTER_STATE,	// 천하제일무도회 등록기간 중에는 이용할 수 없습니다.

	GAME_WORLD_CAN_NOT_CREATE_DYNAMIC_WORLD,			// 월드를 생성할 수 없습니다.
	GAME_WORLD_NOW_CREATING,							// 잠시 후 다시 시도하십시오.
	GAME_WORLD_ALREADY_WORLD_ENTERED,					// 이미 월드에 진입해 있습니다.
	GAME_WORLD_CANT_ENTER_WORLD,						// 진입할 수 있는 월드가 없습니다.
	GAME_WORLD_ALREADY_WORLD_AREA_ENTERED,				// 이미 월드 영역에 진입해 있습니다.
	GAME_WORLD_CANT_FIND_WORLD_AREA,					// 진입할 월드영역을 찾을 수 없습니다.
	GAME_WORLD_CANT_ENTER_WORLD_AREA,					// 진입할 수 있는월드영역이 없습니다.
	
	GAME_SHORTCUT_KEY_ARLEADY_FOUND,					// 같은 키가 이미 지정되어 있습니다.
	GAME_SHORTCUT_KEY_DEL_NOT_FOUND,					// 삭제하려는 키가 없습니다.
	GAME_SHORTCUT_KEY_MODIFY_NOT_FOUND,					// 수정하려는 키가 없습니다.

	GAME_COMMON_CANT_DO_THAT_NOT_ALLOWED_TARGET,		// 명령 수행이 불가능한 유져 입니다.

	GAME_COMMON_CANT_FIND_OBJECT,						// 해당 오브젝트를 찾을 수 없습니다.

	GAME_CHAR_WRONG_REVIVAL_METHOD,						// 잘못된 부활 방법입니다.
	GAME_CANNOT_CONNECT_DUE_TO_TIMEOUT,					// 타임아웃되어 접속할 수 없습니다.
	GAME_CANNOT_CONNECT_TARGET_CHANNEL_FULL,			// 이동하려는 채널에 여유 공간이 없습니다.
	GAME_CANNOT_CONNECT_DUE_TO_INVAlID_CHARACTER,		// 정상적인 캐릭터가 아닙니다.
	GAME_TARGET_HAS_DIFFERENT_JOB,						// 대상의 직업이 올바르지 않습니다.

	GAME_GMT_NOT_ENOUGH_RESET_POINT,					// GMT 재설정 포인트가 부족합니다.

	//-----------------------------------------------------------------
	GAME_SKILL_NOT_ACTIVE_TYPE,		// Active 타입의 스킬이 아닙니다.
	GAME_SKILL_YOU_DONT_HAVE_THE_SKILL,		// 가지고 있지 않은 스킬입니다.
	GAME_SKILL_NOT_EXISTING_SKILL,			// 존재하지 않는 스킬입니다.
	GAME_SKILL_CANT_CAST_NOW,				// 현재 스킬을 시전할 수 있는 상태가 아닙니다.
	GAME_SKILL_NO_TARGET_APPOINTED,				// 대상을 선택하셔야 합니다.
	GAME_SKILL_INVALID_TARGET_APPOINTED,		// 현재 타겟에 스킬을 사용할 수 없습니다.
	GAME_SKILL_NOT_READY_TO_BE_CAST,		// 스킬의 쿨타임을 기다려야 합니다.
	GAME_SKILL_NOT_ENOUGH_LP,				// LP가 충분하지 않습니다.
	GAME_SKILL_NOT_ENOUGH_EP,				// EP가 충분하지 않습니다.
	GAME_SKILL_NOT_ENOUGH_RP_BALL,						// RP 구슬이 충분하지 않습니다.

	GAME_SKILL_NO_REQUIRED_ITEM,			// 스킬을 사용하기 위해 필요한 아이템을 가지고 있지 않습니다.
	GAME_SKILL_TOO_MANY_LP_TO_USE_SKILL,				// LP가 50% 이하일 때에만 이 스킬을 사용할 수 있습니다.
	GAME_SKILL_TARGET_IS_NOT_KNOCKED_DOWN,				// Target is not in a knock down state.
	GAME_SKILL_RESISTED,								// I resisted the skill.
	GAME_SKILL_TRIED_TO_LEARN_AN_EXISTING_SKILL,		// 이미 가지고 있는 스킬은 배울 수 없습니다.
	GAME_SKILL_TRIED_TO_LEARN_WRONG_CLASS_SKILL,		// 직업에 맞지 않는 스킬은 배울 수 없습니다.
	GAME_SKILL_TOO_HIGH_LEVEL_TO_TRAIN,					// 레벨이 낮아서 아직 스킬을 배울 수 없습니다.
	GAME_SKILL_NOT_ENOUGH_ZENNY,						// Zenny가 모자라 스킬을 배울 수 없습니다.
	GAME_SKILL_NO_EMPTY_SKILL_SLOT,						// 빈 스킬 슬롯이 없습니다.
	GAME_SKILL_YOU_HAVE_NO_LOWER_LEVEL_SKILL,			// 업그레이드를 위해서는 선택한 스킬보다 한 단계 낮은 스킬을 가지고 있어야 합니다.

	GAME_SKILL_ALREADY_MASTERED_SKILL,					// 이미 궁극의 스킬을 습득한 상태입니다.
	GAME_SKILL_NO_PREREQUISITE_SKILLS_YOU_HAVE,			// 스킬 습득을 위해 이미 배우고 있어야 할 스킬을 가지고 있지 않습니다.
	GAME_SKILL_UPGRADE_ONLY_SKILL,		// 업그레이드로만 얻을 수 있는 스킬입니다.
	GAME_SKILL_SHOULD_BE_GAINED_BY_YOURSELF,			// 스스로 배워야 하는 스킬입니다.
	GAME_SKILL_CANT_BE_GAINED_BY_YOURSELF,				// 스스로 배울 수 없는 스킬입니다.
	GAME_SKILL_CANT_USE_FOR_SOME_REASON,		// 시스템 문제로 스킬을 사용할 수 없습니다.
	GAME_SKILL_TARGET_IS_NOT_IN_FRONT_SIDE,		// 타겟이 당신 앞에 위치해야 합니다.
	GAME_SKILL_YOU_ARE_NOT_IN_BACKSIDE_OF_TARGET,		// 대상의 뒤에서만 스킬을 사용할 수 있습니다.
	GAME_SKILL_TARGET_IS_IN_FAINT,		// 타겟이 실신한 상태입니다.
	GAME_SKILL_CAN_BE_CAST_ONLY_ON_PLAYER,		// 플레이어에게만 사용할 수 있는 스킬입니다.

	GAME_SKILL_TARGET_IS_NOT_FAINTING,					// 타겟이 실신한 상태가 아닙니다.
	GAME_SKILL_NOT_BATTLE_AVAILABLE_LOCATION,			// 스킬 사용자와 대상 모두 전투가 가능한 지역에 위치해야 합니다.
	GAME_SKILL_CASTING_CANCELED_BY_DAMAGE,				// 공격을 받아 스킬 시전이 취소되었습니다.
	GAME_SKILL_CANT_SET_THAT_RP_BONUS_IN_SKILL,			// 지정한 스킬에서는 설정할 수 없는 RP 보너스 타입입니다.
	GAME_SKILL_CANT_USE_THAT_RP_BONUS_IN_SKILL,			// 지정한 스킬에서는 사용할 수 없는 RP 보너스 타입입니다.
	GAME_SKILL_NO_BUFF_TO_DROP_FOUND,					// 해제될 수 있는 버프를 선택하여야 합니다.
	GAME_SKILL_ALREADY_TRANSFORMED,						// 현재의 변신 상태가 해제되어야 새로운 변신이 적용될 수 있습니다.
	GAME_SKILL_NOT_TRANSFORMED,							// 변신 상태가 아닙니다.
	GAME_SKILL_CANT_USE_SKILL_WHEN_SUPER_SAIYAN,		// 초사이어인 상태에서는 사용할 수 없는 스킬입니다.
	GAME_SKILL_CANT_USE_SKILL_WHEN_PURE_MAJIN,			// 순수 마인 상태에서는 사용할 수 없는 스킬입니다.
	GAME_SKILL_CANT_USE_SKILL_WHEN_GREAT_NAMEK,			// 거대 나메크 상태에서는 사용할 수 없는 스킬입니다.
	GAME_SKILL_CANT_USE_SKILL_WHEN_KAIOKEN,				// 계왕권 상태에서는 사용할 수 없는 스킬입니다.
	GAME_SKILL_CANT_USE_SKILL_WHEN_SPINNING_ATTACK,		// 회전 공격 상태에서는 사용할 수 없는 스킬입니다.
	GAME_SKILL_CANT_USE_SKILL_WHEN_VEHICLE,				// 탈 것 상태에서는 사용할 수 없는 스킬입니다.
	GAME_SKILL_CANT_USE_SKILL_WHEN_NOT_TRANSFORMED,		// 변신하지 않은 상태에서는 사용할 수 없는 스킬입니다.

	GAME_SKILL_CANT_USE_SKILL_BOAST_NOW,				//new 645

	GAME_SKILL_NOT_ENOUGH_SP_POINT,						// SP POINT 가 부족합니다.
	GAME_SKILL_ERASE_FAIL,								// 스킬을 지우다가 실패했습니다.
	GAME_BLOCK_ATTACK_NOW,								// 현재는 제재되어 사용할 수 없습니다. 
	GAME_SKILL_HAVE_UPGRADED_SKILL,						// 스킬중 업그레이드 된 스킬을 습득한 상태입니다.
	GAME_SKILL_CANT_BE_CAST_ON_YOURSELF,				// 자신을 선택하고 사용할 수 없는 스킬입니다.

	GAME_SKILL_CANT_USE_HTB_WHEN_TRANSFORMED,			// 변신 중에는 HTB를 사용할 수 없습니다.
	GAME_SKILL_CASTING_CANCELED_BY_TARGET_NEW_STATE,	// 651 // 대상의 상태가 바뀌어 시전이 취소되었습니다.
    GAME_SKILL_CASTING_CANCELED_OBJECT_OCCLUSION,       // 캐스팅중에 오브젝트에 가려서 취소되었습니다.
	GAME_SKILL_CANT_USE_VEHICLE_WHEN_TRANSFORMED,		// 변신 중에는 탈 것을 이용할 수 없습니다.
	GAME_SKILL_CANT_USE_WHEN_TRANSFORMED,				// 변신 중에는 이용할 수 없습니다.

	GAME_SKILL_CANT_CANCEL_TRANSFORM_WHEN_USING_SKILL,
	GAME_SKILL_CANT_USE_ON_THIS_TARGET,
	GAME_SKILL_NO_SKILL_TO_INIT,
	GAME_SKILL_ALREADY_EXIST_SAME_KIND_BUFF,
	GAME_SKILL_BUFF_IS_FULL,
	GAME_SKILL_CURRENT_BUFF_IS_STRONGER_THEN_NEW,
	GAME_SKILL_ONE_RESET_FAIL,
	GAME_SKILL_GM_BUF_SUCCESS,
	GAME_SKILL_TOO_HIGH_LEVEL_FOR_SKILL_INIT,
	GAME_SKILL_DEBUFF_IS_FULL,
	GAME_SKILL_SUB_BUFF_IS_FULL,
	GAME_SKILL_REVIVAL_AFTEREFFECT_BUFF_IS_FULL,
	GAME_ANOTHER_SKILL_CANT_CAST_NOW,


	//-----------------------------------------------------------------
	GAME_CHAR_ONLY_ADULT_CAN_CHANGE_CLASS,				// 어른 상태에서만 전직을 할 수 있습니다.
	GAME_CHAR_NO_AUTHORITY_FOR_CHANGING_CLASS,					// 전직할 수 있는 자격을 갖추고 있지 않습니다.
	GAME_GM_CANT_DO_THAT_WHEN_YOU_ARE_HIDING,					// 702 // 투명 상태에서는 쓸 수 없는 명령입니다.
	//-----------------------------------------------------------------
	GAME_ITEM_NUM_NULL,
	GAME_ITEM_POSITION_FAIL,
	GAME_ITEM_OWNER_NOT_SAME,
	GAME_ITEM_NOT_SAME,
	GAME_ITEM_OWNER_NULL,
	GAME_ITEM_STACK_FAIL,
	GAME_ITEM_NOT_FOUND,
	GAME_NEEDITEM_NOT_FOUND,
	GAME_NEEDITEM_NOT_FOUND_INVANTORY,
	GAME_ITEM_BAG_IS_NOT_EMPTY,

	GAME_ITEM_ALREADY_EXIST,
	GAME_ITEM_MAX_COUNT_OVER,
	GAME_ITEM_STACK_FULL,
	GAME_EQUIP_SLOT_NOT_EMPTY,
	GAME_ITEM_CLASS_FAIL,
	GAME_ITEM_NEED_MORE_LEVEL,
	GAME_ITEM_NEED_MORE_PARAMETER,
	GAME_ITEM_MAX_WATT_OVER,
	GAME_ITEM_INVEN_FULL,
	GAME_ITEM_LOOTING,

	GAME_ITEM_UNIDENTIFY,
	GAME_ITEM_UNIDENTIFY_FAIL,
	GAME_LOOTING_FAIL,
	GAME_ZENNY_NOT_ENOUGH,
	GAME_ITEM_IS_LOCK,
	GAME_ZENNY_IS_LOCK,
	GAME_REPAIR_VALUE_FAIL,
	GAME_REPAIR_NOT_FOUND,
	GAME_ITEM_YOU_ARE_USING_AN_ITEM,

	GAME_ITEM_NOT_FOR_USE,
	GAME_ITEM_NOT_BATTLE_AVAILABLE_LOCATION,
	GAME_ITEM_CANT_USE_FOR_SOME_REASON,
	GAME_ITEM_NOT_READY_TO_BE_USED,
	GAME_ITEM_TOO_LOW_LEVEL_TO_USE_ITEM,
	GAME_ITEM_NOT_ENOUGH_LP,
	GAME_ITEM_NOT_ENOUGH_EP,
	GAME_ITEM_NOT_ENOUGH_RP_BALL,
	GAME_ITEM_CASTING_CANCELED,
	GAME_ITEM_CANT_USE_NOW,

	GAME_ITEM_DUR_ZERO,
	GAME_SCOUTER_MAX_OVER,
	GAME_ITEM_NOT_ENOUGH,
	GAME_ITEM_UPGRADE_NO_SUBJECT_ITEM,
	GAME_ITEM_UPGRADE_NO_HOIPOI_STONE,
	GAME_ITEM_UPGRADE_WRONG_ITEM_TYPE,
	GAME_ITEM_UPGRADE_ALREADY_MAX_GRADE,
	GAME_ITEM_UPGRADE_FAIL,				
	GAME_ITEM_UPGRADE_FAIL_AND_DEL,
	GAME_BANK_NOT_READY,

	GAME_BANK_ALREADY_EXIST,		// 창고가 이미 존재합니다.
	GAME_SCOUTER_PARTS_NOT_FOUND,	// 해당 스카우터 파츠를 찾지 못했습니다.
	GAME_SCOUTER_JAMMING,			// 전파방해로 인해 표시할 수 없습니다. 
	GAME_SCOUTER_TARGET_FAIL,	 // 표시 대상이 부적절합니다.
	GAME_CHAR_LEVEL_FAIL,		// 레벨이 맞지 않습니다.
	GAME_CHAR_CLASS_FAIL,		// 해당클래스가 맞지 않습니다.
	GAME_CHAR_RACE_FAIL,		// 해당 종족이 맞지 않습니다.
	GAME_ZENNY_OVER,			// 금액이 오버되었습니다.
	GAME_QUEST_ALREADY_EXIST,	// 이미 존재하는 퀘스트입니다.
	GAME_QUEST_COUNT_OVER,		// 수행가능한 퀘스트수가 초과되었습니다.

	GAME_QUEST_NOT_EXIST,		// 존재하지 않는 퀘스트입니다.
	GAME_ITEM_CREATE_COUNT_OVER,// 한번에 생성할 수 있는 갯수를 넘었다.
	GAME_ITEM_DELETE_COUNT_OVER,// 한번에 삭제할 수 있는 갯수를 넘었다.
	GAME_GM_LEVEL_NOT_FOUND,	// GM Level 이 없음
	GAME_GM_PUNISH_NOT_FOUND,	// Punish type error
	GAME_SCOUTER_BODY_IS_NOT_EMPTY, // 스카우터 파츠를 비워야 이동가능합니다.
	GAME_SCOUTER_PARTS_LEVEL_FAIL,	// 스카우터 파츠 레벨이 맞지 않습니다.
	GAME_TRADE_TARGET_WRONG_STATE,		// 상대방이 현재 트레이드를 할 수 없는 상태입니다.
	GAME_TRADE_ALREADY_USE,		// 트레이드 중입니다.
	GAME_TRADE_DENY_USE,		// 트레이드 거절 중입니다.

	GAME_TRADE_DENY,			// 트레이드를 거절하였습니다.
	GAME_TRADE_REPLY_WAIT_OVER,
	GAME_TRADE_WRONG_STATE,		// 트레이드하기에 적절한 상태가 아닙니다.
	GAME_TRADE_ITEM_INVALID,	// 트레이드 불가능한 아이템 입니다.
	GAME_TRADE_ALREADY_CLOSE,	// 트레이드가 이미 잠김상태 입니다.
	GAME_TRADE_ALREADY_OPEN,	// 트레이드가 이미 열림상태 입니다.
	GAME_TRADE_CHANGED_STATE,	// 트레이드 물품 정보가 바뀌었습니다.
	GAME_ITEM_NOT_GO_THERE,		// 옮길수 없는 위치입니다.
	GAME_DRAGONBALL_OBJECT_ARLEADY_USED, // 제단이 사용 중입니다.
	GAME_DRAGONBALL_NOT_FOUND,	 // 드래곤볼을 찾을수 없습니다.

	GAME_DRAGONBALL_REWARD_NOT_FOUND,   // 드래곤볼 보상을 찾을 수 없습니다.
	GAME_DRAGONBALL_TIME_OVER,	// 사용시간이 만료되었습니다.
	GAME_DRAGONBALL_SAME_EXIST,		// 동일 한 드래곤볼이 있습니다.
	GAME_QUSET_ITEM_CREATE_COUNT_OVER,// 퀘스트 인벤토리에서 동일한 아이템을 생성할 수 없습니다.
	GAME_ITEM_CANT_BE_USED_ON_YOURSELF,					// 자신을 선택하고 사용할 수 없는 아이템입니다.
	GAME_ITEM_TYPE_NOT_MISMATCHED,	// Item type is not correct.
	GAME_ITEM_RECIPE_ALREADY_EXIST,	// The same recipe already exists.	
	GAME_ITEM_RECIPE_NOT_FOUND,		// The recipe does not exist.
	GAME_ITEM_RECIPE_REGISTER_PROFESSION,	// You must first select the appropriate item mix job.
	GAME_ITEM_RECIPE_LEVEL_MISMATCHED,	// The item recipe level is not correct.
	GAME_ITEM_RECIPE_TYPE_MISMATCHED,	// 해당 아이템 레시피 타입이 맞지 않습니다.
	GAME_ITEM_RECIPE_REGISTER_FAIL,		// 아이템 레시피 등록에 실패했습니다.
	GAME_ZENNY_LOOTING,					// I'm picking up Zenny.
	GAME_ITEM_RECIPE_REGISTER_PROFESSION_FAIL,	// 아이템 믹스 직업이 잘못되었습니다.
	GAME_SCOUTER_MAIN_PARTS_NOT_FOUND,	// 스카우터 메인 파츠를 찾지 못했습니다.
	GAME_ITEM_HOIPOIMIX_FARE_NOT_SET,	// 호이포이머신사용료가 세팅되어있지 않습니다. 
	GAME_ITEM_HOIPOIMIX_FARE_CAN_NOT_SET,	// 호이포이머신사용료 세팅은 캐쉬머신만 가능합니다. 
	GAME_ITEM_HOIPOIMIX_FARE_RATE_FAIL,	// 호이포이머신사용료가 범위가 초과 되었습니다.
	GAME_ITEM_CANNOT_BUY_NOW,			// 구매할 수 있는 아이템이 아닙니다.
	GAME_TRADE_ONLY_PARTY,
	GAME_SKILL_SLOT_FAIL,	
	GAME_SKILL_ID_NULL,
	GAME_SKILL_OWNER_NULL,
	GAME_SKILL_SAME_EXIST,
	GAME_SKILL_NOT_FOUND,
	GAME_ITEM_NOT_PROPER_PLACE_TO_USE,						// It is not a good place to use items.
	GAME_ITEM_YOU_HAVE_NO_RELATED_QUEST_TO_USE,				// I do not have any quests related to items.
	GAME_CANT_DO_THAT_TO_FREE_PVP_ZONE_TARGET,
	GAME_CAN_BE_CAST_ONLY_ON_PLAYER,
	GAME_NO_REVIVAL_AFTEREFFECT_TO_REMOVE,
	GAME_REVIVAL_AFTEREFFECT_IS_TOO_STRONG_TO_BE_CURED,
	GAME_ITEM_CHANGE_BATTLE_ATTRIBUTE_NO_SUBJECT_ITEM,
	GAME_ITEM_CHANGE_BATTLE_ATTRIBUTE_WRONG_ITEM_TYPE,
	GAME_ITEM_CHANGE_BATTLE_ATTRIBUTE_WRONG_SUB_ITEM_TYPE,
	GAME_ITEM_CHANGE_BATTLE_ATTRIBUTE_WRONG_RANK,
	GAME_ITEM_CHANGE_BATTLE_ATTRIBUTE_WRONG_LV,
	GAME_SHOP_NOT_FOUND,
	GAME_SHOP_SO_FAR,
	GAME_SHOP_NOT_CLOSED,					// 상점이 열린상태입니다.
	GAME_SHOP_ITEM_INVALID,					// 팔수 없는 아이템 입니다.
	GAME_MOVE_CANT_GO_THERE,		// 갈 수 없는 위치입니다
	GAME_WORLD_NOT_EXIST,				// 존재하지 않는 월드 입니다.
	GAME_MOVE_CANT_ALLOWED_ENTER,		// 진입이 허용되지 않습니다.
	GAME_WORLD_CANT_MOVE_AGAIN,								// 월드에 재진입 할 수 없는 상태입니다.
	GAME_PARTY_ALREADY_IN_PARTY,						// 이미 파티에 속해 있습니다.
	GAME_PARTY_ANOTHER_PLAYER_IS_INVITING_YOU,				// 다른 플레이어가 이미 당신을 초대하고 있습니다.
	GAME_PARTY_ANOTHER_PARTY_IS_INVITING_YOU,				// 다른 파티가 이미 당신을 초대하고 있습니다.
	GAME_PARTY_NOT_PROPER_PARTY_NAME_LENGTH,				// 파티 이름 길이가 적절하지 않습니다.
	GAME_PARTY_PARTY_NAME_HAS_INVALID_CHARACTER,			// 파티 이름에 사용 불가능한 문자가 포함되어 있습니다.
	GAME_PARTY_NOT_CREATED_FOR_SOME_REASON,					// 시스템 문제로 파티가 생성되지 않았습니다.
	GAME_PARTY_YOU_ARE_NOT_IN_PARTY,						// 파티에 속해 있지 않습니다.
	GAME_PARTY_ONLY_ALLOWED_TO_PARTY_LEADER,				// 파티 리더만 사용할 수 있는 기능입니다.
	GAME_PARTY_NOT_AVAILABLE_OPERATION_RIGHT_NOW,			// 파티가 수행하고 있는 작업이 끝나야 가능합니다.
	GAME_PARTY_NO_SUCH_A_PLAYER,							// 존재하지 않는 플레이어입니다.
	GAME_PARTY_TARGET_ALREADY_IN_PARTY,						// 상대방이 이미 파티에 속해 있습니다.
	GAME_PARTY_TARGET_ALREADY_HAS_INVITATION,				// 상대방이 이미 파티 초대를 받은 상태입니다.
	GAME_PARTY_NO_ROOM_FOR_NEW_MEMBER,						// 더 이상 멤버를 받을 수 없습니다.
	GAME_PARTY_MEMBER_IS_ALREADY_REGISTERED_TIMEQUEST,		// 상대방이 타임머신 퀘스트에 이미 등록되어 있습니다.
	GAME_PARTY_YOU_HAVE_NO_INVITATION,						// 당신은 파티로부터 초대를 받은 상태가 아닙니다.
	GAME_PARTY_INVITOR_PLAYER_IS_IN_A_PARTY,				// 당신을 초대했던 플레이어를 찾을 수 없습니다.
	GAME_PARTY_NO_SUCH_A_PARTY,								// 파티가 존재하기 않습니다.
	GAME_PARTY_COULDNT_JOIN_FOR_SOME_REASON,				// 시스템 문제로 파티에 가입할 수 없었습니다.
	GAME_PARTY_NO_SUCH_A_PLAYER_IN_THE_PARTY,				// 파티에 존재하지 않는 플레이어입니다.
	GAME_PARTY_COULDNT_KICK_OUT_FOR_SOME_REASON,			// 시스템 문제로 멤버를 강제로 탈퇴시킬 수 없었습니다.
	GAME_PARTY_CANT_KICK_OUT_HIMSELF,						// 자기 자신을 강제로 탈퇴시킬 수 없습니다.
	GAME_PARTY_YOU_ARE_ALREADY_A_LEADER,					// 이미 자신이 파티 리더입니다.
	GAME_PARTY_COULDNT_ASSIGN_A_LEADER_FOR_SOME_REASON,		// 시스템 문제로 멤버를 리더로 변경할 수 없었습니다.
	GAME_PARTY_COULDNT_BE_DONE_FOR_SOME_REASON,				// 시스템 문제로 요청이 처리될 수 없었습니다.
	GAME_PARTY_ZENNY_IS_LOCKED_RIGHT_NOW,					// 현재 인벤토리의 제니를 조작할 수 없는 상태입니다.
	GAME_PARTY_THE_ITEM_IS_LOCKED,							// 아이템을 움직일 수 없습니다.
	GAME_PARTY_YOU_DONT_BELONG_TO_THE_PARTY,				// 당신은 파티에 속해 있지 않은 것 같습니다.
	GAME_PARTY_YOU_DONT_HAVE_THAT_MUCH_ZENNY,				// 그만큼의 제니를 가지고 있지 않습니다.
	GAME_PARTY_ONLY_EQUIP_ITEM_AVAILABLE_IN_THAT_SLOT,		// 장착 가능한 아이템만 등록할 수 있는 슬롯입니다.
	GAME_PARTY_ZENNY_DOESNT_SATISFY_THE_CONDITION,			// 제니가 부적의 조건을 만족시키지 않습니다.
	GAME_PARTY_MEMBER_ZENNY_IS_IN_TRANSACTION,				// 제니를 사용할 수 없는 상태의 파티원이 있습니다.
	GAME_PARTY_UNKNOWN_ZENNY_LOOTING_METHOD,				// 잘못된 제니 분배 방식입니다.
	GAME_PARTY_UNKNOWN_ITEM_LOOTING_METHOD,					// 잘못된 아이템 분배 방식입니다.
	GAME_PARTY_CANT_CHANGE_LOOTING_METHOD_YET,				// 다시 전리품 분배 방식을 바꿀 수 있을 때까지 시간이 필요합니다.
	GAME_PARTY_NOBODY_CANT_RECEIVE_ITEM_RIGHT_NOW,			// 아이템을 가질 수 있는 멤버가 아무도 없습니다.
	GAME_PARTY_NO_EMPTY_SPACE_IN_INVENTORY,					// 인벤토리에 빈 공간이 없습니다.
	GAME_PARTY_NO_EMPTY_SPACE_IN_PARTY_INVENTORY,			// 파티 인벤토리에 빈 공간이 없습니다.
	GAME_PARTY_MEMBER_IS_TOO_FAR,							// 멤버가 너무 멀리 떨어져 있습니다. 퀘스트 공유 할때 사용 by niam
	GAME_PARTY_INVITING_IS_NOT_ALLOWED,						// 이 파티에서는 초대가 허용되지 않습니다.
	GAME_PARTY_LEAVING_IS_NOT_ALLOWED,						// 이 파티에서는 탈퇴가 허용되지 않습니다.
	GAME_PARTY_KICKING_OUT_IS_NOT_ALLOWED,					// 이 파티에서는 강제 탈퇴가 허용되지 않습니다.
	GAME_PARTY_APPOINTING_A_LEADER_IS_NOT_ALLOWED,			// 이 파티에서는 파티장 임명이 허용되지 않습니다.
	GAME_PARTY_SHARETARGET_NOTFINDOBJECT,					// 해당 오브젝트가 없습니다 [3/28/2008 SGpro]
	GAME_PARTY_SHARETARGET_SLOTID_FAIL,						// 공유타겟 슬롯ID가 잘못되었습니다[3/28/2008 SGpro]
	GAME_PARTY_DUNGEON_IN_PLAYER_FOUND,						// 1372 // 던전에 아직 남아있는 캐릭터가 있습니다. 
	GAME_PARTY_DICE_FAIL,									// 파티 주사위 결과 실패
	GAME_PARTY_INVEN_ITEM_EXIST,							// 파티인벤에 아이템이 남아있습니다.
	GAME_PARTY_ITEM_DICE_USING_NOW,							// 파티 아이템 다이스가 사용중입니다.
	GAME_PARTY_ITEM_DICE_NOT_USING_NOW,						// 파티 아이템 다이스가 안 사용중입니다.
	GAME_PARTY_DUNGEON_MIN_PLAYER_FAIL,						// 파티 던전의 최소인원이 부족합니다.
	GAME_PARTY_DUNGEON_OUT_RESERVED,						// 파티 던전 나가기 예약 NTL_MIN_DUNGEON_PARTYOUT_TIME 남았습니다..(정의된 값이 바뀌면 반드시 기획쪽 GUI 담당자에게 알려 주세요)
	GAME_PARTY_MEMBER_NOT_SAME,								// 파티인벤에 멤버가 다릅니다.
	GAME_PARTY_DUNGEON_ENTER_PARTY_FAIL,					// 파티던전에 들어가시려면 파티를 생성하십시요.
	GAME_PARTY_DUNGEON_IN_CHANGE_DIFF_NOW,					// 파티던전안에선 난이도 변경이 불가능합니다.
	GAME_PARTY_CANT_INVITE_YOURSELF,						// 자기 자신을 파티에 초대할 수 없습니다.
	GAME_PARTY_CANT_INVITE_IN_WORLD,
	GAME_PARTY_CANT_INVITE_FOR_SOME_REASON,
	GAME_PARTY_DUNGEON_IS_NOT_CREATED,
	GAME_PARTY_DUNGEON_DIFF_NOT_USE,
	GAME_PARTY_CANT_INVITE_TARGET_SANDBAG,
	GAME_PARTY_CANT_JOIN_TARGET_SANDBAG,
	GAME_PARTY_DUNGEON_BEING_INITIALIZED,
	GAME_PARTY_DUNGEON_ENTER_USING_NPC,
	GAME_PARTY_WRONG_CHARM_INVENTORY_SPECIFIED,				// 부적 인벤토리가 잘못 지정되었습니다.
	GAME_PARTY_CHARM_INVENTORY_IS_OPEN,						// 부적 인벤토리가 열려 있습니다.
	GAME_PARTY_CHARM_INVENTORY_IS_CLOSED,					// 부적 인벤토리가 닫혀 있습니다.
	GAME_PARTY_CHARM_INVENTORY_SLOT_IS_NOT_EMPTY,			// 부적 인벤토리 슬롯이 비어 있지 않은 상태입니다.
	GAME_PARTY_CHARM_INVENTORY_SLOT_IS_EMPTY,				// 부적 인벤토리 슬롯이 비어 있는 상태입니다.
	GAME_PARTY_NO_SUCH_A_VICTIM_ITEM,						// 존재하지 않는 제물 아이템입니다.
	GAME_PARTY_UNIDENTIFIED_VICTIM_ITEM,					// 제물 아이템이 감정이 되지 않은 상태입니다.
	GAME_PARTY_VICTIM_ITEM_IS_NOT_YOURS,					// 당신이 등록한 제물 아이템이 아닙니다.
	GAME_PARTY_CANT_UNREGISTER_CHARM_DUE_TO_VITIM,			// 이미 등록한 제물이 있어서 부적을 다시 가져올 수 없습니다.
	GAME_PARTY_VICTIM_ZENNY_HASNT_CHANGED,					// 부적 제물로 등록한 제니의 양이 바뀌지 않았습니다.
	GAME_PARTY_YOU_HAVENT_REGISTERED_VICTIM_ZENNY,			// 제니를 부적 제물로 등록한 적이 없습니다.
	GAME_PARTY_TOO_MUCH_VICTIM_ZENNY_TO_UNREGISTER,			// 등록한 제니의 양보다 많은 양의 제니를 가져오려고 했습니다.
	GAME_PARTY_ONLY_LEADER_CAN_REGISTER_CHARM,				// 부적은 파티 리더만 등록할 수 있습니다.
	GAME_PARTY_ONLY_CHARM_ITEM_AVAILABLE_IN_THAT_SLOT,		// 부적 아이템만 등록할 수 있는 슬롯입니다.
	GAME_PARTY_SAME_CHARM_ALREADY_HAS_BEEN_REGISTERED,		// 이미 활성화되어 있는 부적 아이템입니다.
	GAME_PARTY_CHARM_ITEM_HASNT_BEEN_REGISTERED,			// 현재 부적이 등록되어 있지 않습니다.
	GAME_PARTY_CHARM_SLOT_HAS_AN_ITEM_OF_IMPROPER_TYPE,		// 부적 슬롯에 부적이 아닌 아이템이 등록되어 있습니다.
	GAME_PARTY_CHARM_SLOT_HAS_UNKNOWN_CHARM_ITEM,			// 부적 슬롯에 알 수 없는 부적이 등록되어 있습니다.
	GAME_PARTY_ITEMS_DONT_SATISFY_THE_CONDITION,			// 제물 아이템이 부적의 조건을 만족시키지 않습니다.
	GAME_PARTY_MEMBER_DOESNT_HAVE_ENOUGH_CHARM_POINT,		// 등록한 부적 포인트보다 적은 포인트를 가진 파티원이 있습니다.
	GAME_PARTY_MEMBER_DOESNT_HAVE_ENOUGH_ZENNY,				// 등록한 제니보다 적은 제니를 가진 파티원이 있습니다.
	GAME_PARTY_MEMBER_CHARM_POINT_IS_IN_TRANSACTION,		// 부적 포인트를 사용할 수 없는 상태의 파티원이 있습니다.
	GAME_PARTY_MEMBER_DOESNT_SATISFY_THE_CONDITION,			// 부적 사용 조건을 만족시키지 못한 파티원이 있습니다.
	GAME_PARTY_CHARM_BUFF_REGISTERED_ALREADY,				// 이미 등록된 부적 버프가 있습니다.
	GAME_PARTY_NO_CHARM_BUFF_REGISTERED,					// 등록된 부적 버프가 없습니다.
	GAME_PARTY_INVALID_INVENTORY_SLOT_INDEX,				// 파티 인벤토리의 아이템 위치가 잘못 지정되었습니다.
	GAME_PARTY_TOO_MANY_CONTRIBUTION_POINTS_TO_INVEST,		// 투자할 수 있는 기여도의 범위를 넘었습니다.
	GAME_PARTY_ONLY_EQUIP_ITEM_ALLOWED_FOR_CHARM_POINT,		// 장착 아이템으로만 부적 포인트를 얻을 수 있습니다.
	GAME_PARTY_IDENTIFIED_ITEM_NEEDED_FOR_CHARM_POINT,		// 감정된 아이템으로만 부적 포인트를 얻을 수 있습니다.
	GAME_PARTY_INVEN_INVEST_ZENNY_NOT_ENOUGH,				// 파티인벤에 투자할 돈이 부족합니다.
	GAME_PARTY_INVEN_LOCKED,								// 파티인벤이 잠겼습니다.
	GAME_PARTY_INVEN_ITEM_NON_EXIST,						// 파티인벤 해당 슬롯에 아이템 없습니다.
	GAME_PARTY_INVEN_DISTRIBUTE_METHOD_NOT_MATCH,			// 분배방식이 맞지 않습니다.
	GAME_PARTY_MEMBER_ZENNY_IS_LOCK,						// 파티 멤버중 제니가 잠긴 상태가 있습니다.
	GAME_PARTY_INVEN_LOCK_FIRST,							// 파티인벤이 열려있습니다. 먼저 닫아주십시요.
	GAME_PARTY_INVEN_UNDER_INVEST,							// 파티인벤에 투자된 최대금액 보다 높아야합니다.
	GAME_HTB_NO_HTB_RESULT,							// 진행할 HTB가 없습니다
	GAME_HTB_CANT_FORWARD_ANYMORE,							// 더이상 진행할 HTB단계가 없습니다.
	GAME_HTB_NOT_ENOUGH_RP_BALL,							// RP 구슬이 충분하지 않습니다.
	GAME_HTB_YOU_ALREADY_USED_RP_BALL,						// 이미 RP 구슬을 사용한 상태입니다.
	GAME_HTB_YOU_HAVE_NO_RELATION_WITH_HTB,					// 1404 // HTB와 관련 없는 상태입니다.
	GAME_WORLD_ENTER_NEED_LOW_LEVEL, // 해당 지역에 진입하기에는 낮은 레벨입니다.
	GAME_WORLD_ENTER_NEED_HIGH_LEVEL, // 해당 지역에 진입하기에는 높은 레벨입니다.
	GAME_WORLD_ENTER_NEED_ITEM, // 해당 지역에 진입하기위한 필요  아이템이 없습니다.
	GAME_FREEBATTLE_TARGET_HAS_NO_MATCH, // 대상이 결투를 진행할 수 업습니다.
	GAME_FREEBATTLE_WRONG_PLACE, // 결투를 할 수 없는 지역입니다.
	GAME_FREEBATTLE_ALREADY_HAS_MATCH, // 이미 결투중입니다.
	GAME_FREEBATTLE_TARGET_ALREADY_HAS_MATCH, // 이미 결투중인 대상입니다.
	GAME_FREEBATTLE_CHALLENGE_TIME_REMAIN, // 아직 도전 대기 시간입니다.
	GAME_FREEBATTLE_CHALLENGE_ACCEPT_DENIED, // 결투 신청을 거절당했습니다.
	GAME_FREEBATTLE_CHALLENGE_ACCEPT_TIME_DENIED, // 결투 신청에 응답을 하지 않아 자동으로 거절되었습니다.
	GAME_FREEBATTLE_CHALLENGE_WAIT_TIME_DENIED, // 결투 신청에 대한 응답이 없어 자동으로 거절되었습니다.
	GAME_FREEBATTLE_ERROR_HAS_NO_MATCH, // 에러(에러코드) - 결투를 진행할 수 업습니다.
	GAME_FREEBATTLE_ERROR_TARGET_HAS_NO_MATCH, // 에러(에러코드)  - 대상이 결투를 진행할 수 업습니다.
	GAME_FREEBATTLE_ERROR_WRONG_PEER, // 에러(에러코드) - 잘못된 결투 대상을 가지고 있습니다.
	GAME_FREEBATTLE_ERROR_TARGET_WRONG_PEER, // 에러(에러코드) - 대상이 잘못된 결투 대상을 가지고 있습니다.
	GAME_WORLD_ENTER_NEED_REPUTATION, // 해당 지역에 진입하기위한 필요 명성이 낮습니다.
	GAME_WORLD_NOT_FOUND,			  // 해당 지역을 찾을 수 없습니다.
	GAME_WORLD_ENTER_DISAPPROVAL,	 // 해당 지역에 진입할 수 없습니다.
	GAME_SCOUTER_TARGET_SELF_FAIL,	 // 자기 자신을 대상으로 불가능합니다.
	GAME_SCOUTER_TARGET_FAIL_FOR_NPC,
	GAME_ITEM_CANNOT_DELETE,
	GAME_ITEM_LOOTING_FAIL_FOR_DICELOOT,
	GAME_ITEM_DICE_FAIL_NOT_EQUIPED,
	GAME_ITEM_DICE_FAIL_OVERFLOW_PARTYINVEN,
	GAME_ITEM_DICE_FAIL_NOT_FIND,
	GAME_ITEM_DICE_FAIL_INVALID_DICE,
	GAME_ITEM_DICE_FAIL_INVALID_PARTY,
	GAME_ITEM_DICE_FAIL_LIMITED_STATE,


	//////////////////////////////////////////////////////////////////////////
	//
	// Trigger system 관련 메시지들
	//
	//////////////////////////////////////////////////////////////////////////

	GAME_TS_WARNING_NOT_SATISFY_REWARD_CONDITIONS ,	// 보상 받을 수 있는 조건을 만족하지 못합니다.
	GAME_TS_WARNING_MUST_SELECT_ONE_SEL_REWARD,		// 선택 보상이 존재합니다. 반드시 하나의 선택보상을 선택해야 합니다.
	GAME_TS_WARNING_PROCESSING_PRE_CONFIRM_REQ,		// 이전 진행 요청이 처리중입니다. (진행)
	GAME_TS_WARNING_NOW_TIME_WAIT,					// 이전 진행 요청이 처리중입니다. (시간)
	GAME_TS_WARNING_OVERFLOW_MAX_TS_NUM,			// 실행가능한 최대 트리거 개수가 넘었습니다.

	GAME_TS_WARNING_SYSTEM_WAIT,					// 다른 시스템과 충돌이 일어난 경우. 잠시 기다려 주세요.
	GAME_TS_WARNING_INVENTORY_IS_LOCK,				// 아이템 인벤토리가 lock이 걸려 있음
	GAME_TS_WARNING_INVENTORY_IS_FULL,				// 아이템 인벤토리가 가득찼음
	GAME_TS_WARNING_QUEST_INVENTORY_IS_FULL,		// 퀘스트 인벤토리가 가득 찼음
	GAME_TS_WARNING_INVALID_QUEST_ITEM_DELETE_COUNT,// 퀘스트 아이템의 삭제 요청 갯수가 틀림
	GAME_TS_WARNING_REWARD_FAIL,					// 보상을 하지 못함(아마 이 메시지가 갈일은 없을것임)
	GAME_TS_WARNING_EQUIP_SLOT_NOT_EMPTY,			// 장비 슬롯이 비어있지 않습니다.
	GAME_TS_WARNING_EQUIP_SLOT_LOCK,				// 장비 슬롯이 잠겨있습니다.
	GAME_TS_WARNING_CLASS_CHANGE_CLASS_FAIL,		// 전직 클래스가 다름
	GAME_TS_WARNING_CLASS_CHANGE_LEVEL_FAIL,		// 전직 레벨이 안됨
	GAME_TS_WARNING_ESCORT_EXCEED_MEMBER,			// 멤버 초과
	GAME_TS_WARNING_ESCORT_NOT_SHARED,				// 공유 모드가 아님
	GAME_TS_WARNING_ESCORT_TRIGGER_TYPE_WRONG,		// 트리거 타입이 다름
	GAME_TS_WARNING_ESCORT_TRIGGER_ID_WRONG,		// 트리거 아이디가 다름
	GAME_TS_WARNING_ESCORT_PARTY_WRONG,				// 파티가 다름
	GAME_TS_WARNING_ESCORT_ALREADY_EXIST,			// 이미 등록되어 있음
	GAME_TS_WARNING_TMQ_COUPON_NOT_ENOUGH,			// TMQ에서 쿠폰이 부족함

	GAME_TS_WARNING_WPS_ALREDY_USED_BY_OTHER_PLAYER,// 이미 다른 유저가 수행중입니다.
	GAME_TS_WARNING_WPS_CAN_NOT_JOIN_NOW,			// 지금은 수행 할 수 없습니다.

	GAME_TS_WARNING_CAN_NOT_FIND_VISIT_EVT,			// 관련된 방문 이벤트가 없습니다. for visit event
	GAME_TS_WARNING_ALREADY_VISITED,				// 이미 완료했습니다. for visit event
	GAME_TS_WARNING_WRONG_CONDITION,				// 수행 조건이 맞지 않습니다.
	GAME_TS_WARNING_CANNOT_FIND_QUEST_ITEM,			// 퀘스트 아이템을 찾을 수 없습니다.
	GAME_TS_WARNING_DEL_FORBIDDENDEL_QUEST_ITEM,	// 지울수 없는 퀘스트 아이템입니다.

	GAME_TS_WARNING_IMPROPER_NUMBER_OF_SEL_REWARD,
	GAME_TS_WARNING_ROLLBACK_FAIL,
	GAME_TS_WARNING_ROLLBACK,
	GAME_TS_WARNING_NOT_YET_PROCESSED,

	//-----------------------------------------------------------------
	GAME_PARTYMATCHING_ROLEPLAY_CANCEL,
	GAME_PARTYMATCHING_ROLEPLAY_HAVE_NO_CHOICE,
	GAME_PARTYMATCHING_ROLEPLAY_INVALD,
	GAME_PARTYMATCHING_INVITE_FAIL,
	GAME_PARTYMATCHING_ENTER_DUNGEON_AGREE_FAIL,
	GAME_PARTYMATCHING_ALREADY_REGISTERED,
	GAME_PARTYMATCHING_ANY_MEMBER_IN_DYNAMIC_WORLD,
	GAME_PARTYMATCHING_ROLEPLAY_NOT_SELECTED,
	GAME_PARTYMATCHING_REGISTER_WRONG_STATE,
	GAME_PARTYMATCHING_PARTY_IS_NOT_REGISTER,
	GAME_PARTYMATCHING_PARTY_IS_REGISTER_ALREADY,

	//-----------------------------------------------------------------

	GAME_TS_ERROR_RUN_ERROR,						// 트리거 진행 에러
	GAME_TS_ERROR_CS_SCRIPT_MISSMATCH,				// 다음에 진행해야할 컨테이너 타입이 CS 간에 일치하지 않습니다.
	GAME_TS_ERROR_NO_IMP_CONT_TYPE,					// 구현되지 않은 컨테이너 타입이 사용되었습니다.
	GAME_TS_ERROR_CANNOT_CREATE_TS_OBJECT,			// 트리거 오브젝트 생성 실패.
	GAME_TS_ERROR_CANNOT_FIND_TID,					// 트리거를 찾을 수 없습니다.
	GAME_TS_ERROR_CANNOT_FIND_TCID,					// 컨테이너를 찾을 수 없습니다.
	GAME_TS_ERROR_CANNOT_PROGRESS_REPEAT_QUEST,		// 반복 퀘스트는 오로지 한번만 진행할 수 있습니다

	GAME_TS_ERROR_SYSTEM,							// 일반적인 시스템상의 에러[버그]
	GAME_TS_ERROR_TRIGGER_SYSTEM,					// Trigger System 에서 에러가 난 경우
	GAME_TS_ERROR_QUERY_SERVER,						// 쿼리 서버 에러
	GAME_TS_ERROR_CANNOT_FIND_ITEM_TBLIDX,			// 테이블에서 아이템을 찾을수 없다
	GAME_TS_ERROR_CANNOT_FIND_SKILL_TBLIDX,			// 스킬을 찾을 수 없습니다.
	GAME_TS_ERROR_CANNOT_LEARN_SKILL,				// 스킬을 배울 수 없습니다.
	GAME_TS_ERROR_CANNOT_ADD_QUEST_EVENT_DATA,		// 이벤트 옵저버에 이벤트를 등록할 수 없습니다.
	GAME_TS_ERROR_CANNOT_FIND_QUEST_ITEM,			// 퀘스트 아이템을 찾을 수 없습니다.
	GAME_TS_ERROR_CREATE_ITEM_COUNT_OVER,			// 한번에 생성할 수 있는 제한 갯수를 넘었다.
	GAME_TS_ERROR_DELETE_ITEM_COUNT_OVER,			// 한번에 삭제할 수 있는 제한 갯수를 넘었다
	GAME_TS_ERROR_CANNOT_FIND_PC,					// PC를 찾을 수 없다
	GAME_TS_ERROR_TRIGGER_OBJECT_NOT_EXIST,			// 존재하지 않는 트리거 오브젝트 입니다.
	GAME_TS_ERROR_TRIGGER_OBJECT_INVALID_FUNCFLAG,	// 기능이 올바르지 않은 트리거 오브젝트입니다.

	//-----------------------------------------------------------------
	GAME_PET_CANT_MAKE_MORE_SUMMON_PET,			// 더 이상 소환펫을 부를 수 없습니다.
	GAME_PET_CANT_MAKE_MORE_ITEM_PET,					 // 더 이상 아이템 펫을 부를 수 없습니다.
	GAME_PET_COULDNT_BE_DONE_FOR_SOME_REASON,			// 시스템 문제로 요청을 처리할 수 없습니다.
	GAME_PET_TARGET_IS_NOT_SPAWNED,						// 대상이 소환되어 있지 않습니다.
	//-----------------------------------------------------------------
	GAME_GUILD_NO_GUILD_MANAGER_NPC_FOUND,			// 길드 매니저 NPC를 찾을 수 없습니다.
	GAME_GUILD_NOT_GUILD_MANAGER_NPC,						// 길드 매니저 NPC가 아닙니다.
	GAME_GUILD_GUILD_MANAGER_IS_TOO_FAR,					// 선택한 길드 매니저가 너무 멀어 대화를 할 수 없습니다.
	GAME_GUILD_NEED_MORE_ZENNY_FOR_NEW_GUILD,				// 길드 생성을 위해서는 제니가 더 필요합니다.
	GAME_GUILD_NOT_PROPER_GUILD_NAME_LENGTH,				// 길드 이름 길이가 적절하지 않습니다.
	GAME_GUILD_GUILD_NAME_HAS_INVALID_CHARACTER,			// 길드 이름에 사용 불가능한 문자가 포함되어 있습니다.
	GAME_GUILD_NOT_PROPER_GUILD_NAME,						// 길드 이름이 적절하지 않습니다.
	GAME_GUILD_SAME_GUILD_NAME_EXIST,						// 동일 길드 이름이 존재합니다.
	GAME_GUILD_NOT_EXIST,									// 해당 길드가 존재하지 않습니다.
	GAME_GUILD_NON_EXISTING_GUILD_FUNCTION,					// 알 수 없는 길드 기능입니다.

	GAME_GUILD_ALREADY_HAS_GUILD_FUNCTION,					// 이미 가지고 있는 길드 기능입니다.
	GAME_GUILD_NEED_PREREQUISITE_GUILD_FUNCTION,			// 한 단계 낮은 길드 기능을 획득한 상태여야 합니다.
	GAME_GUILD_NEED_MORE_GUILD_POINT,						// 더 많은 길드 포인트가 필요합니다.
	GAME_GUILD_NEED_MORE_ZENNY,								// 더 많은 제니가 필요합니다.
	GAME_GUILD_NEED_LEVEL_MORE,								// 레벨이 부족합니다.
	GAME_GUILD_GIVE_ZENNY_NOT_MATCHED,						// 길드 포인트를 얻기위해서는 1000으로 나누어 떨어지고 1000 이상이여야 합니다.
	GAME_GUILD_NO_GUILD_FOUND,								// 길드에 가입되어 있지 않습니다.
	GAME_GUILD_MARK_ARLEADY_CREATED,						// 길드마크가 이미 생성 되어 있습니다.
	GAME_GUILD_MARK_NOT_CREATED,							// 길드마크가 생성 되어 있지 않습니다.
	GAME_GUILD_NEED_DOJO_NOT_FOUND,							// 도장을 찾을 수 없습니다.

	GAME_GUILD_MAKE_DOJO_ALREADY_TAKEN,						// 이미 유파가 도장을 소유하고 있어 도장을 설립할 수 없습니다.
	GAME_GUILD_NEED_GUILD_FUNCTION,							// 길드의 기능이 없습니다. 기능획득을 먼저하십시요.
	GAME_GUILD_NO_DOJO_MANAGER_NPC_FOUND,					// 도장 매니저 NPC를 찾을 수 없습니다.
	GAME_GUILD_NOT_DOJO_MANAGER_NPC,						// 도장 매니저 NPC가 아닙니다.
	GAME_GUILD_DOJO_MANAGER_IS_TOO_FAR,						// 선택한 도장 매니저가 너무 멀어 대화를 할 수 없습니다.
	GAME_GUILD_NON_EXISTING_DOJO_FUNCTION,					// 알 수 없는 도장 기능입니다.
	GAME_GUILD_MAKE_DOJO_NOT_TAKEN,							// 도장의 주인이 없습니다.
	GAME_GUILD_DOGI_NOT_CREATED,							// 도복 생성 되어 있지 않습니다.
	GAME_GUILD_DOGI_ARLEADY_CREATED,						// 도복이 이미 생성 되어 있습니다.
	GAME_GUILD_ALREADY_HAS_DOJO_FUNCTION,					// 이미 가지고 있는 도장 기능입니다.

	GAME_GUILD_NEED_PREREQUISITE_DOJO_FUNCTION,				// 한 단계 낮은 도장 기능을 획득한 상태여야 합니다.
	GAME_GUILD_HOIPOIROCK_ITEM_NOT_FOUND,					// 호이포이락 아이템을 찾지 못했습니다.
	GAME_GUILD_HOIPOIROCK_ITEM_NOT_MATCHED,					// 호이포이락 아이템이 아닙니다.
	GAME_GUILD_HOIPOIROCK_ITEM_COUNT_FAIL,					// 호이포이락 아이템 갯수가 부족합니다.
	GAME_GUILD_DOJO_SCRAMBLE_CHALLENGE_HAVE,				// 도장 쟁탈 신청길드가 이미 있습니다.
	GAME_GUILD_DOJO_YOU_ARE_NOT_A_PARTY_LEADER,				// 도장의 설립은 유파 최고사범만이 가능합니다.
	GAME_GUILD_DOJO_YOU_ARE_NOT_OWNNER,						// 도장의 주인이 아닙니다.
	GAME_GUILD_DOJO_SCRAMBLE_CHALLENGE_STATUS_FAIL,			// 도장쟁탈전 신청가능 상태가 아닙니다. 
	GAME_GUILD_DOJO_SCRAMBLE_CHALLENGE_HAVE_DOJO,			// 도장을 소유한 유파는 쟁탈전을 신청할 수 없습니다.
	GAME_GUILD_DOJO_SCRAMBLE_REJECT_STATUS_FAIL,			// 도장쟁탈전 신청거부가능 상태가 아닙니다. 

	GAME_GUILD_DOJO_NOT_FOUND_TABLE,						// 도장테이블을 찾을수 없습니다. 
	GAME_GUILD_DOJO_NEED_FUNCTION,							// 도장설립 권한이 없습니다. 권한획득을 먼저하십시요.
	GAME_GUILD_DOJO_SCRAMBLE_MAX_COUNT_OVER,				// 도장쟁탈전에 들어갈수 있는 인원 수를 초과 했습니다.	
	GAME_GUILD_DOJO_NOT_FOUND,								// 해당 도장을 찾을 수 없습니다.	
	GAME_GUILD_DOJO_SELF_SCRAMBLE_FAIL,						// 자신이 속한 도장에 쟁탈전을 신청할수 없습니다.	
	GAME_GUILD_DOJO_SCRAMBLE_NOT_A_PARTY_LEADER,			// 쟁탈전의 신청은 최고사범만 할 수 있습니다.
	GAME_GUILD_DOJO_YOU_ARE_NOT_ENOUGH_REPUTATION,			// 쟁탈전을 신청하기에는 유파 포인트가 충분하지 않습니다. 
	GAME_GUILD_DOJO_YOU_ARE_NOT_ENOUGH_ZENNY,				// 쟁탈전을 신청하기에는 소지한 제니가 부족합니다. 
	GAME_GUILD_DOJO_YOU_ARE_ARLREADY_REJECT,				// 쟁탈전을 거부당해 신청할 수 없는 도장입니다. 다음 신청기간에 가능합니다. 
	GAME_GUILD_DOJO_YOU_DONT_BELONG_TO_ANY_DOJO,			// 도장에 속해 있지 않습니다.

	GAME_GUILD_DOJO_YOU_DONT_BELONG_TO_THAT_DOJO,			// 해당 도장에 속해 있지 않습니다.
	GAME_GUILD_DOJO_YOU_ARE_ARLREADY_REJECT_ONE_TIME,		// 쟁탈전 거부는 한번만 가능합니다.
	GAME_GUILD_DOJO_YOU_CANT_ATTACK_YOUR_TEAM_SEAL,			// 자신의 팀에 속한 인장을 공략할 수 없습니다.
	GAME_GUILD_DOJO_SEAL_CANT_BE_ATTACKED,					// 지금은 인장을 공략할 수 없습니다.
	GAME_GUILD_DOJO_TOO_MANY_ATTACKER_ON_SEAL,				// 이미 많은 플레이어가 인장을 공략하고 있습니다.
	GAME_GUILD_YOUR_GUILD_CANT_DO_DOJO_TELEPORT,			// 2155 // 텔레도장 기능을 사용할 수 없는 유파에 속해 있습니다.
	GAME_GUILD_DOJO_FUNCTION_ADD_FIRST,						// 도장의 기능을 먼저 습득하신 후 사용해 주세요. 
	GAME_GUILD_DOJO_DOGI_CHANGE_FUNCTION_NEED,
	GAME_GUILD_DOGI_CHANGE_FUNCTION_NEED,
	GAME_GUILD_DOJO_ALREADY_HAVE,
	GAME_GUILD_DOJO_SCRAMBLE_CHALLENGE_ONLY_ONETIME,
	GAME_GUILD_DOJO_SCRAMBLE_SEAL_INVEN_NOT_FOUND,
	GAME_GUILD_DOJO_SCRAMBLE_CHALLENGER_CANT_DO,

	//-----------------------------------------------------------------
	GAME_ROOM_ENTER_EXCEED_MAX_MEMBER_COUNT,			// 방에 인원이 가득 찼습니다.
	GAME_ROOM_ENTER_EXCEED_MAX_ROOM_COUNT,					// 더이상 방을 만들 수 없습니다.
	GAME_ROOM_ENTER_NEED_MORE_MEMBER,						// 방에 인원이 더 필요 합니다.
	GAME_ROOM_ENTER_TOO_LOW_LEVEL,							// 방에 진입하기에는 레벨이 너무 낮습니다
	GAME_ROOM_ENTER_TOO_HIGHTLEVEL,							// 방에 진입하기에는 레벨이 너무 높습니다
	GAME_ROOM_ENTER_MUST_HAVE_NEED_ITEM,					// 방에 진입하기위한 필요 아이템이 없습니다
	GAME_ROOM_ENTER_MUST_HAVE_NEED_ZENNY,					// 방에 진입하기위한 제니가 부족합니다.
	GAME_ROOM_ENTER_ROOM_NOT_EXIST,							// 방이 존재하지 않습니다.
	GAME_ROOM_ENTER_ROOMMANAGER_NOT_EXIST,					// 게시판이 존재하지 않습니다.
	GAME_ROOM_ENTER_NEED_MORE_PARTY_MEMEBER,				// 파티멤버가 더 필요합니다.
	GAME_ROOM_ENTER_NOT_MATCH_MEMBER,						// 대전 멤버가 아닙니다.
	GAME_ROOM_ENTER_NOT_WAIT_STATE,							// 대기 상태가 아닙니다.
	GAME_ROOM_ENTER_FAIL,									// 방진입 시도가 실패하였습니다
	GAME_ROOM_LEAVE_LIMIT_TIME_ELAPSED,						// 대기룸에서 나갈수 없는 시간 입니다
	GAME_ROOM_LEAVE_FAIL,									// 대기룸 나오기 실패
	GAME_ROOM_NOT_OWNER,									// 방의 소유자가 아닙니다.
	GAME_ROOM_CAN_NOT_FIND_PARTY_MEMBER,					// 파티멤버를 찾을 수 없습니다.

	GAME_RANKBATTLE_NOR_REGISTERED,							// 등록되어 있지 않습니다.
	GAME_RANKBATTLE_CANNOT_LEAVE_NOW,						// 취소 할 수 없습니다.
	GAME_RANKBATTLE_CANNOT_LEAVE_IN_MATCH,					// 경기중에는 취소할 수 없습니다.
	GAME_RANKBATTLE_CANNOT_FIND_ARENA,						// 경기장을 찾을 수 없습니다.
	GAME_RANKBATTLE_CANNOT_FIND_OPPONENT,					// 상대를 찾을 수 없습니다.
	GAME_RANKBATTLE_MEMBER_ALREADY_JOINED_RANKBATTLE,		// 파티원이 이미 랭크배틀 중입니다.
	GAME_RANKBATTLE_ARENA_IS_FULL,							// 비어있는 경기장이 없습니다.
	GAME_RANKBATTLE_OBJECT_IS_TOO_FAR,						// 너무 멀리 떨어져 있습니다.

	GAME_RANKBATTLE_NO_REMAIN_COUNT,						//new 2225

	GAME_MATCH_CAN_NOT_USE_SKILL_IN_THIS_WORLD,				// 현재 월드에서는 사용할 수 없는 스킬입니다.
	GAME_MATCH_CAN_NOT_USE_SKILL_IN_OUTOFAREA,				// 장외일때는 스킬을 사용할 수 없습니다.
	GAME_TIMEQUEST_CANNOT_LEAVE_IN_PARTY_WHEN_PLAYING_RANKBATTLE,	// 랭크배틀 중일때에는 파티에서 나갈 수 없습니다.

	
	//-----------------------------------------------------------------
	GAME_TIMEQUEST_WORLD_NOT_FOUND,					// 타임머신 퀘스트에 해당하는 월드를 찾을 수 없습니다
	GAME_TIMEQUEST_ROOM_NOT_FOUND,							// 타임머신 퀘스트 대기방을 찾을 수 없습니다
	GAME_TIMEQUEST_ALREADY_JOINED,							// 타임머신 퀘스트 대기실에 이미 참여하고 있습니다
	GAME_TIMEQUEST_HAVE_NO_JOIN_ROOM,						// 타임머신 퀘스트 대기실에 참여하고 있지 않습니다
	GAME_TIMEQUEST_CANT_MAKE_PARTY,							// 타임머신 퀘스트에 자동 파티를 생성할 수 없습니다.
	GAME_TIMEQUEST_NEED_MORE_MEMBER,						// 타임머신 퀘스트를 할 인원이 충분하지 않습니다
	GAME_TIMEQUEST_NOT_ALLOWED_MEMBER,						// 타임머신 퀘스트에 진입이 허가되지 않은 멤버 입니다.
	GAME_TIMEQUEST_EXCEED_MAX_MEMBER_COUNT,					// 타임머신 퀘스트 진입최대 인원이 초과되었습니다.
	GAME_TIMEQUEST_TOO_LOW_LEVEL,							// 타임머신 퀘스트를 하기위한 레벨이 너무 낮습니다.
	GAME_TIMEQUEST_TOO_HIGH_LEVEL,							// 타임머신 퀘스트를 하기위한 레벨이 너무 높습니다.
	GAME_TIMEQUEST_MUST_HAVE_NEED_ITEM,						// 타임머신 퀘스트에 진입하기위한 아이템이 없습니다
	GAME_TIMEQUEST_MUST_HAVE_NEED_ZENNY,					// 타임머신 퀘스트에 진입하기위한 제니가 부족합니다.
	GAME_TIMEQUEST_ROOM_NOT_WAIT_STATE,						// 타임머신 퀘스트 대기실에 진입할 수 없습니다.
	GAME_TIMEQUEST_ROOM_PARTY_JOIN_FAIL,					// 타임머신 퀘스트 대기실에 파티등록을 할 수 없습니다.
	GAME_TIMEQUEST_ROOM_PARTY_ALREADY_JOINED,				// 타임머신 퀘스트 대기실에 이미 파티등록이 되어 있습니다.
	GAME_TIMEQUEST_ROOM_OUT_OF_ENTER_RANGE,					// 타임머신 퀘스트의 진입 영역을 벗어났습니다.
	GAME_TIMEQUEST_ROOM_CANNOT_TELEPORT_NOW,				// 지금은 타임머신의 위치로 이동할 수 없습니다.
	GAME_TIMEQUEST_DAYRECORD_IS_ALREADY_RESET,				// 데이레코드 리셋이 이미 처리되었습니다.
	GAME_TIMEQUEST_WORLD_ENTER_FAIL,						// 타임머신 퀘스트 진입이 실패하였습니다.
	GAME_TIMEQUEST_CANNOT_LEAVE_IN_PARTY_WHEN_PLAYING_TMQ,	// 타임머신 퀘스트를 수행 할 때에는 파티에서 나갈 수 없습니다.

	GAME_TIMEQUEST_MEMBER_LIMIT_COUNT_IS_OVER,
	GAME_TIMEQUEST_ERROR,
	GAME_TOO_LOW_MEMBER_LEVEL,
	GAME_TOO_HIGHT_MEMBER_LEVEL,
	GAME_HAVE_NEED_ZENNY_MEMBER,
	GAME_TIMEQUEST_MEMBER_LIMIT_PLUS_COUNT_IS_OVER,

	//-----------------------------------------------------------------
	GAME_TUTORIAL_CHAR_ALREADY_TUTORIAL_ACCOMPLISHED, // 캐릭터가 이미 튜토리얼을 수행했습니다
	GAME_TUTORIAL_CANT_FIND_TUTORIAL_WAIT_ROOM,				// 튜토리얼 대기룸이 존재 하지 않습니다
	GAME_TUTORIAL_CANT_ENTER_TUTORIAL_WAIT_ROOM,			// 튜토리얼 대기룸에 진입할 수 없습니다
	GAME_TUTORIAL_YOU_HAVE_NO_JOIN_ROOM,					// 튜토리얼 대기를 하고 있지 않습니다.

	//-----------------------------------------------------------------
	GAME_PRIVATESHOP_NOTAUTH,												// 권한이 없습니다
	GAME_PRIVATESHOP_NOTRUNSTATE,											// 지금 상태에서는 실행 할 수 없습니다
	GAME_PRIVATESHOP_PRIVATESHOP_NOT_CREATE_PLACE,							// 개인 상점을 열수 없는 장소 입니다
	GAME_PRIVATESHOP_PRIVATESHOP_ANOTHER_PRIVATESHOP,						// 근처에 다른 개인상점이 있어서 개인상점을 열 수 없습니다
	GAME_PRIVATESHOP_PRIVATESHOP_NULL,										// 상점이 없습니다
	GAME_PRIVATESHOP_PRIVATESHOP_INVENTORY_SAVEITEM_OVER,					// 상점 인벤토리에 빈슬롯이 없습니다
	GAME_PRIVATESHOP_PRIVATESHOP_INVENTORY_SLOT_ALREADY_ITEM,				// 상점 인벤토리 슬롯에 이미 아이템이 있습니다
	GAME_PRIVATESHOP_ITEM_NULL,												// 아이템이 없습니다
	GAME_PRIVATESHOP_ITEM_NOTVALID,											// 팔수 없는 아이템입니다
	GAME_PRIVATESHOP_VISITOR_NULL,											// 손님이 없습니다
	GAME_PRIVATESHOP_VISITOR_NOT_MONEY,										// 손님에 돈이 모자릅니다
	GAME_PRIVATESHOP_VISITOR_NOT_INVENTORY_EMPTY,							// 손님에 인벤토리 슬롯에 아이템들이 꽉 차 있습니다
	GAME_PRIVATESHOP_VISITOR_ALREADYENTER,									// 손님이 이미 상점에 입장했습니다
	GAME_PRIVATESHOP_VISITOR_SELECTITEM_NULL,								// 손님이 찜한 아이템이 없습니다
	GAME_PRIVATESHOP_VISITOR_SELECTITEM_OVER,								// 더이상 아이템을 찜할 수 없습니다
	GAME_PRIVATESHOP_VISITOR_SELECTITEM_ALREADY,							// 이미 찜한 아이템입니다
	GAME_PRIVATESHOP_VISITOR_SELECTITEM_ALLSELL,							// 선택한 아이템들을 살수 없습니다
	GAME_PRIVATESHOP_VISITOR_SELECTITEM_INVENSOLT_SHORTOF,					// 인벤토리에 빈 슬롯이 부족합니다
	GAME_PRIVATESHOP_VISITOR_FULL,											// 상점에 손님이 들어갈 수 있는 빈자리가 없습니다
	GAME_PRIVATESHOP_OWNER_MONEY_FULL,										// 상점 주인 돈이 더이상 늘어날수 없습니다
	GAME_PRIVATESHOP_OWNER_THEREISNO,										// 상점 주인이 자리에 없습니다
	GAME_PRIVATESHOP_OWNER_BUSINESS_REFUSED,								// 주인이 흥정을 거부중이다
	GAME_PRIVATESHOP_INVENTORY_NOTINVENTTORY,								// 잘못된 인벤토리 입니다
	GAME_PRIVATESHOP_INVENTORY_SAVEITEM_OVER,								// Inventory에 빈 슬롯이 없습니다
	GAME_PRIVATESHOP_STOP_PROGRESS,											// 종료중입니다
	GAME_PRIVATESHOP_OWNER_BARGAINS,										// 주인이 다름사람과 흥정중입니다
	GAME_PRIVATESHOP_ITEM_TABLE_ITEMID_DIFFERENT,							// 아이템이 동일하지 않습니다
	GAME_PRIVATESHOP_ITEM_SELECTITEM_NOTBUSINESS,							// 이미 선택한 아이템은 흥정할 수 없습니다
	GAME_PRIVATESHOP_OWNER_BUSINESS_NOT_CONSENT,							// 상점 주인이 흥정을 거부했습니다
	GAME_PRIVATESHOP_SAMENOT_PRICEFLUCTUATIONSCOUNTER,						// 주인이 가격 조정한 횟수와 손님이 가격 조정 패킷을 받은 값이 다릅니다.
	GAME_PRIVATESHOP_ZENNY_LOCK,											// 다른 사람과 Item Buy중이여서 Zenny가 Lock이 되었습니다. 처리가 끝날때까지 기다려 주세요
	GAME_PRIVATESHOP_NOT_PROGRESS_COMPLETE,									// 이전 요청이 처리될때까지 끝날때까지 기다려 주세요 (패킷이 Qry Srv에 머물고 있다)
	GAME_PRIVATESHOP_VISITOR_SELECTITEM_ALREADYBUYANDBUSINESSITEM,			// 이미 다른 손님이 구매한 아이템입니다.
	GAME_PRIVATESHOP_PRIVATESHOP_ANOTHER_NPC,								// 근처에 NPC가 있어서 개인상점을 열 수 없습니다

	//-----------------------------------------------------------------
	GAME_MAIL_TARGET_AWAY_STATE,						// 메일 받는 사람은 부재중이라 수신불가능합니다.
	GAME_MAIL_NOT_FOUND,									// 메일 찾지 못했습니다.
	GAME_MAIL_ALREADY_READ,									// 메일을 이미 읽은 상태입니다.
	GAME_MAIL_INVALID_DEL,									// 해당메일은 지울 수 없습니다.
	GAME_MAIL_INVALID_RETURN,								// 해당메일은 리턴할 수 없습니다.
	GAME_MAIL_INVALID_ACCEPT,								// 해당메일을 받을 수 없는 상태입니다.
	GAME_MAIL_INVALID_LOCK,									// 해당메일을 잠금설정이 변경 불가능합니다.
	GAME_MAIL_INVALID_ZENNY,								// 보낼 수 있는 제니의 한도를 초과했습니다.
	GAME_MAIL_NOT_EXISTING_PLAYER,							// 해당 캐릭터는 생성되지 않은 캐릭터입니다.
	GAME_MAIL_CANT_SEND_MAIL_TO_YOURSELF,					// 자기 자신에게는 메일을 보낼 수 없습니다.
	GAME_MAIL_MAILING_PARTS_NOT_FOUND,						// 메일 전송칩이 없습니다.
	GAME_MAIL_TRANSMIT_PARTS_NOT_FOUND,						// 물질 전송칩이 없습니다.
	GAME_MAIL_SCOUTER_FAIL,									// 메일전송을 위한 스카우터를 사용할 수 없는 상태입니다.
	//new
	GAME_MAIL_INVALID_CHAR_DEL,
	//-----------------------------------------------------------------
	GAME_PORTAL_ARLEADY_ADDED,						// 이미등록된 포탈입니다.
	GAME_PORTAL_NOT_EXIST,									// 미등록 포탈입니다. 

	GAME_WARFOG_ARLEADY_ADDED,						// 이미등록된 워포그입니다.
	//-----------------------------------------------------------------
	GAME_PROCESSING_DB,						// 디비처리중입니다.
	//-----------------------------------------------------------------
	GAME_RIDE_ON_STATE,						// 현재 타 있는 상태 입니다.
	GAME_VEHICLE_CANNOT_WHILE_DRIVING,						// 운전 중엔 할 수 없습니다.
	GAME_VEHICLE_ENGINE_ALREADY_STARTED,					// 이미 시동이 켜져 있습니다.
	GAME_VEHICLE_ENGINE_ALREADY_STOPED,						// 이미 시동이 꺼져 있습니다.
	GAME_VEHICLE_INVALID_FUEL_ITEM,							// 
	GAME_VEHICLE_FAIL_TO_USE_FUEL_ITEM,						// 연료 아이템 사용에 실패했습니다.
	GAME_VEHICLE_NO_FUEL_ITEM_REMAINING,					// 남은 연료 아이템이 없습니다.
	GAME_VEHICLE_END_BY_USER,								// 유저가 탈것에서 내리기를 요청했습니다.
	GAME_VEHICLE_END_BY_HIT,								// 피격 당했기 때문에 탈것에서 내립니다.
	GAME_VEHICLE_END_BY_CONVERTCLASS,						// 전직으로 인해 탈 것에서 내립니다.
	GAME_VEHICLE_END_BY_TELEPORT,							// 텔레포트로 인해 탈 것에서 내립니다.
	GAME_VEHICLE_END_BY_FORCED,								// 탈 것 상태가 강제로 풀렸습니다.
	GAME_VEHICLE_END_BY_TMQ,								// TMQ 상태에 진입하기 때문에 탈것에서 내립니다.
	GAME_VEHICLE_END_BY_FORBIDDEN_ZONE,						// 탈 것이 허용되지 않는 지역입니다.
	//new
	GAME_VEHICLE_NOT_ON_VEHICLE,
	GAME_VEHICLE_WILL_GET_OFF_SOON,

	//-----------------------------------------------------------------
	GAME_PETITION_NOTAUTH,							//권한이 없습니다
	GAME_PETITION_TOO_LONG_NAME,							//캐릭터명의 길이가 초과되었습니다.
	GAME_PETITION_TOO_SHORT_NAME,							//캐릭터명의 길이가 너무 짧습니다.
	GAME_PETITION_TOO_LONG_QUESTION_CONTENT,				//진정 내용의 길이가 초과되었습니다.
	GAME_PETITION_TOO_SHORT_QUESTION_CONTENT,				//진정 내용의 길이가 너무 짧습니다.
	GAME_PETITION_CATEGORY_1_FAIL,							//카테고리 1의 값이 이상합니다.
	GAME_PETITION_CATEGORY_2_FAIL,							//카테고리 2의 값이 이상합니다.
	GAME_PETITION_CREATE_PETITION_FAILE ,					//진정 생성을 실패했습니다
	GAME_PETITION_DELETE_FAILE_STARTED ,					//이미 상담중에 보류한 진정이기에 삭제할 수 없습니다
	GAME_PETITION_ISNOT_STARTED ,							//진정 상태가 시작이 아닙니다
	GAME_PETITION_ALREADYSTARTED ,							//이미 시작중입니다
	GAME_PETITION_INSERT_ERROR_ZERO_ID ,					//데이타 저장 실패 - ID가 0입니다
	GAME_PETITION_INSERT_FAILE ,							//데이타 저장 실패 (std::map에서 오류를 통보, 알 수 없는 오류
	GAME_PETITION_INSERT_ALLREADY_ID ,						//데이타 저장 실패 (이미 등록되어 있는 ID 입니다
	GAME_PETITION_CREATE_PETITION_ID_FAILE ,				//Petition ID 생성 실패
	GAME_PETITION_NOTRUNSTATE,								//지금 상태에서는 실행 할 수 없습니다
	GAME_PETITION_ACCOUNTNOTPETITION,						//해당 계정에 진정이 없습니다
	GAME_PETITION_ALREADY_INSERT,							//이미 진정이 접수된 Account입니다.
	GAME_PETITION_LOCK,										// Petition이 Lock되어 있습니다
	GAME_PETITION_PETITIONID_INVALID,						// Petition ID가 invalid 입니다.
	GAME_PETITION_ALREADYFINISH,							//이미 완료된 진정입니다.

	//-----------------------------------------------------------------
	GAME_BUDOKAI_INVALID_TEAM_NAME,					// 팀의 이름이 올바르지 않습니다.
	GAME_BUDOKAI_TOO_LONG_TEAM_NAME,						// 팀의 이름이 너무 깁니다.
	GAME_BUDOKAI_TOO_SHORT_TEAM_NAME,						// 팀의 이름이 너무 짧습니다.
	GAME_BUDOKAI_NEED_MORE_MEMBER,							// 팀원이 너무 적습니다.
	GAME_BUDOKAI_NOT_REGISTER_PERIOD,						// 등록 기간이 아닙니다.
	GAME_BUDOKAI_OVER_COUNT,								// 참가인원이 초과되었습니다.
	GAME_BUDOKAI_CHARACTER_ALREADY_JOIN,					// 이미 신청된 상태입니다.
	GAME_BUDOKAI_CHARACTER_NOT_JOINED,						// 신청하지 않은 상태입니다.
	GAME_BUDOKAI_MEMBER_ALREADY_JOINED,						// 이미 신청된 멤버가 있습니다.
	GAME_BUDOKAI_YOU_ARE_NOT_A_TEAM_LEADER,					// 팀 리더가 아닙니다.
	GAME_BUDOKAI_NOT_MATCH_PLAYING,							// 진행중인 경기가 없습니다.
	GAME_BUDOKAI_NOT_OPENED,								// 오픈기간이 아닙니다.
	GAME_BUDOKAI_NOT_MATCH_WORLD,							// 천하제일무도회 경기장이 아닙니다.

	GAME_CAN_NOT_TELEPORT,									// 이동할 수 없습니다.
	GAME_CAN_NOT_TELEPORT_THIS_STATE,						// 이동할 수 없는 상태입니다. 잠시 후 다시 시도하세요.

	GAME_MUDOSA_NOT_OPEND,									// 오픈 기간이 아닙니다.
	GAME_MUDOSA_POINT_CANT_BE_USED_RIGHT_NOW,				// 일시적으로 무도사 포인트를 사용할 수 없는 상황입니다. 잠시 후 다시 시도하세요.
	GAME_MUDOSA_POINT_NOT_ENOUGH,							// 무도사 점수가 충분하지 않습니다.
	GAME_MUDOSA_POINT_MAX_OVER,								// 무도사 점수가 최대를 초과하였습니다.

	GAME_MINORMATCH_CANNOT_TELEPORT_THIS_STATE,				// 현재의 예선전 상테에서는 진입할 수 없습니다.

	GAME_NETP_POINT_CANT_BE_USED_RIGHT_NOW,					// 일시적으로 NetPY 포인트를 사용할 수 없는 상황입니다. 잠시 후 다시 시도하세요.
	GAME_NETP_POINT_MAX_OVER,								// NetPY Point가 최대를 초과하였습니다.
	GAME_NETP_POINT_NOT_ENOUGH,								// NetPY Point가 충분하지 않습니다.
	GAME_NETPY_IS_LOCK,										// NetPY가 잠김 상태입니다.
	GAME_NETPY_NOT_ENOUGH,									// NetPY가 부족합니다.	

	GAME_CASHITEM_CANT_MOVE,								// 해당 캐쉬아이템은 가져올 수 없습니다.
	GAME_CASHITEM_NOT_LOADED,								// 캐쉬아이템 로딩이 완료되지 않았습니다.
	GAME_CASHITEM_NOT_FOUND,								// 해당 캐쉬아이템을 찾을 수 없습니다.
	
	//new
	GAME_BUDOKAI_QUERY_FAIL_EXIST_MATCH_INDEX,
	GAME_BUDOKAI_QUERY_FAIL_INSERT_FAIL,
	GAME_BUDOKAI_CAN_NOT_TELEPORT_JUNIOR,

	//-----------------------------------------------------------------
	// Game, Item 관련. 800번대에서 이어짐. ( 3000 ~ )
	//-----------------------------------------------------------------
	GAME_ITEM_UPGRADE_ITEM_IS_ZERO_GRADE,			// 블랙 호이포이 스톤을 사용할 수 없습니다.
	GAME_DRAGONBALL_NOT_SAME_EXIST,							// 다른 종류의 드래곤볼 입니다.
	GAME_ITEM_DURATIONTIME_ZERO,							// 사용 기간이 끝났습니다.
	GAME_ITEM_RECIPE_CANNOT_RESET_NORMAL_TYPE,				// 기본 제조 기술은 포기할 수 없습니다.
	GAME_ITEM_NOT_USE_THIS_PLACE,							// 여기서 사용할 수 없는 아이템입니다.
	GAME_ITEM_STACK_FAIL_ITEMTYPE_DURATION,					// 사용기간이 있는 아이템은 Stack할 수 없습니다.
	GAME_ITEM_RECIPE_NORMAL_RECIPE_ALREADY_STARTED,			// 기본 제조 기술은 이미 배운 상태입니다.
	GAME_ITEM_RECIPE_SPECIAL_RECIPE_ALREADY_STARTED,		// 특수 제조 기술은 이미 배운 상태입니다.
	GAME_ITEM_RECIPE_CANNOT_RESET_CAUSE_NOT_STARTED_YET,	// 아직 배우지 않은 기술은 초기화 할 수 없습니다.
	GAME_ITEM_RECIPE_CANNOT_SET_YOU_NEED_HIGHER_LEVEL,		// 레벨이 부족하기 때문에 제조기술을 배울 수 없습니다.
	GAME_ITEM_RECIPE_CANNOT_SET_YOU_NEED_MORE_ZENNY,		// 제니가 부족하기 때문에 제조기술을 배울 수 없습니다.
	GAME_ITEM_CANT_USE_CAUSE_CAPSULE_ITEM_ALREADY_IN_USE,	// 캡슐 아이템을 이미 사용중입니다.

	GAME_SCOUTER_CHIP_NOT_EXIST,							// 스카우터 칩이 존재하지 않습니다.
	//new
	GAME_SCOUTER_PC_INFO_JAMMING,
	GAME_SCS_CHECK_FAIL,
	GAME_ITEM_UPGRADE_NO_HOIPOI_STONE_CORE,
	GAME_ITEM_UPGRADE_ITEM_AND_HOIPOI_STONE_DONT_MATCH,
	GAME_ITEM_UPGRADE_CANT_USE_STONE_CORE_WITH_BLACK_STONE,
	GAME_QUICK_TELEPORT_ITEM_NOT_FOUND,
	GAME_QUICK_TELEPORT_ITEM_LOCK,
	GAME_QUICK_TELEPORT_INFO_NOT_FOUND,
	GAME_QUICK_TELEPORT_INFO_NOT_VALID,
	GAME_ITEM_CANT_USE_ADD_CHARACTER_PLUS,
	GAME_EMPTY_ITEM_SLOT_NOT_ENOUGH,
	GAME_SAMENAME_EXIST,
	GAME_CHARACTER_NAME_HAS_INVALID_CHARACTER,
	GAME_CHARACTER_FAIL_BY_PARTY,
	GAME_CHARACTER_FAIL_BY_GUILD,
	GAME_CHARACTER_FAIL_BY_BUDOKAI,
	GAME_CHARACTER_FAIL_BY_LIMITED,
	GAME_GUILD_NOT_PROPER_GUILD_NAME_DOJO,
	GAME_GUILD_NOT_PROPER_GUILD_NAME_CHALLENGE_DOJO,
	GAME_ITEM_USEFUL_ONLY_WHEN_FAINTING,
	GAME_GAMERULE_REG_ALREADY_JOINED_DOJO_SCRAMBLE,
	GAME_GAMERULE_REG_CANT_PLAY_IN_DOJO_SCRAMBLE,
	GAME_CHARACTER_TOO_LONG_NAME,
	GAME_CHARACTER_TOO_SHORT_NAME,
	GAME_NPC_SERVER_IS_DOWN,
	GAME_ITEM_HOIPOI_CANNOT_MAKE_INVEN_FULL,
	GAME_ITEM_NEED_LESS_LEVEL,
	GAME_ITEM_TOO_HIGH_LEVEL_TO_USE_ITEM,
	GAME_ITEM_CANT_RENEWRAL,
	GAME_CANNOT_TRADE_NO_BAGSLOT,
	GAME_CANNOT_VISIT_INVALID_AREA,
	GAME_CANNOT_VISIT_SELF,
	GAME_ITEM_GENDER_DOESNT_MATCH,
	GAME_ITEM_UPGRADE_MUST_USE_STONE_WEAPON,
	GAME_ITEM_UPGRADE_MUST_USE_STONE_ARMOR,
	GAME_ITEM_UPGRADE_MUST_USE_PROPER_LEVEL_STONE,
	GAME_ITEM_UPGRADE_ALREADY_HIGHEST_GRADE,
	GAME_ITEM_UPGRADE_ALREADY_LOWEST_GRADE,
	GAME_ITEM_USE_ONLY_CHARACTER,
	GAME_ITEM_USE_ONLY_PLYAER,
	GAME_ITEM_CANT_USE_INVALID_PLAYER,
	GAME_ITEM_CANT_USE_INVALID_WORLD,
	GAME_ITEM_UPGRADE_COUPON_NOT_FOUND,
	GAME_ITEM_UPGRADE_COUPON_GRADE_FAIL,
	GAME_ITEM_UPGRADE_FAIL_FOR_DURATION_ITEM,
	GAME_ITEM_UPGRADE_FAIL_FOR_GRADE_DATA,
	GAME_ITEM_CANNOT_USE_INVALID_TARGET,
	GAME_ITEM_ALREADY_SEAL,
	GAME_ITEM_CANNOT_SEAL_BY_INVALID_RESTRICT,
	GAME_ITEM_NOT_SEALITEM,
	GAME_ITEM_CANNOT_SEAL_MORE_SEALITEM,
	GAME_ITEM_CANNOT_EXTRACT_SEAL_INVALID_ITEM,
	GAME_ITEM_UPGRADE_FAIL_AND_NODEL_BY_CORE,
	GAME_ITEM_UPGRADE_CANT_USE_STONE_CORE_WITH_SAFE,
	GAME_ITEM_UPGRADE_MUST_USE_PROPER_LEVEL_CORE_STONE,
	GAME_ITEM_NOT_BEAD,
	GAME_ITEM_NOT_SOCKET,
	GAME_ITEM_NOT_INSERT_BEAD_FOR_DURATION,
	GAME_ITEM_NOT_INSERT_BEAD_INVALID_LEVEL,
	GAME_ITEM_INVALID_DESTROY_BEAD,
	GAME_ITEM_INVALID_BEAD_OPTION,
	GAME_ITEM_INVALID_DURATION_BEAD,
	GAME_ITEM_CANNOT_INSERT_BEAD_BY_NO_MATCH_ITEM,
	GAME_ITEM_NOT_DESTROY_BEAD,
	GAME_ITEM_INSERT_BEAD_FAIL_AND_DEL,
	
	//-----------------------------------------------------------------
	CASHITEM_FAIL,
	CASHITEM_FAIL_NOT_DEFINED_SYSTEM,
	CASHITEM_FAIL_NOT_READY_SERVICE,
	CASHITEM_FAIL_NETWORK_ERROR,
	CASHITEM_FAIL_UNKNOWN_ERROR,
	CASHITEM_FAIL_CONFIRM_ERROR,
	CASHITEM_FAIL_NEED_MORE_CASH,
	CASHITEM_FAIL_NOT_EXIST_ITEM,
	CASHITEM_FAIL_NOT_ON_SALE_ITEM,
	CASHITEM_FAIL_CANT_BUY_CURRENT_SERVER,
	CASHITEM_FAIL_BANK_ALREADY_EXIST,
	CASHITEM_FAIL_CANT_FIND_CHARNAME,
	CASHITEM_FAIL_CANT_GIFT_MYSELF,
	CASHITEM_FAIL_CANT_RENEWAL,
	


	//-----------------------------------------------------------------
	REPORT_FAIL,											// 리포팅 실패

	//-----------------------------------------------------------------
	// Petition System ( 10000 ~ )
	//-----------------------------------------------------------------
	PETITION_FAIL,											// Petition 실패

	PETITION_NOT_STARTED_CLEINT_GM_CHAT,					// 클라이언트의 GM 채팅이 시작되지 않았습니다
	PETITION_DIFFERENT_CHAATING_GM_ID,						// 클라이언트가 채팅중인 GM의 ID와 다른 GM ID이다.
	PETITION_NOT_YET_ENTER_WORLD,							// 유저가 월드에 들어가지 않았다
	PETITION_AREADY_GM_CHATTING,							// 유저가 이미 GM 채팅중이다




	//--NEW------------------------------------------------------------
	// Event System ( 11000 ~ )
	//-----------------------------------------------------------------
	EVENT_YOU_ALREADY_HAVE_CHARACTER,
	EVENT_NO_MORE_NEW_CHARACTER_ALLOWED,
	EVENT_OFF_CONTENTS_USED,

	//--NEW------------------------------------------------------------
	// Scramble System ( 12000 ~ )
	//-----------------------------------------------------------------
	SCRAMBLE_CANNOT_DO_WHILE_JOINED,
	SCRAMBLE_CANNOT_DO_WHILE_NOT_JOINED,
	SCRAMBLE_CANNOT_DO_ALREADY_REWARDED,
	SCRAMBLE_CANNOT_DO_TARGET_IS_NOT_JOINED,
	SCRAMBLE_CANNOT_TRADE_TARGET_IS_NOT_PARTY_MEMBER,
	SCRAMBLE_CANNOT_TRADE_TARGET_IS_ALREADY_REWARED,
	SCRAMBLE_CANNOT_INVITE_TARGET_IS_NOT_JOINED,
	SCRAMBLE_CANNOT_INVITE_TARGET_IS_JOINED,
	SCRAMBLE_CANNOT_RIDE_BUS_WHILE_JOINED,
	SCRAMBLE_CANNOT_PICK_WHILE_NOT_JOINED,
	SCRAMBLE_CANNOT_PICK_ALREADY_REWARDED,
	SCRAMBLE_CANNOT_JOIN_LEVEL_IS_TOO_LOW,
	SCRAMBLE_INVALID_BATTLE_DRAGONBALL,
	SCRAMBLE_CANNOT_JOIN_WHILE_TMQ,
	SCRAMBLE_CANNOT_JOIN_WHILE_RANKBATTLE,
	SCRAMBLE_CANNOT_JOIN_WHILE_DOJO,
	SCRAMBLE_CANNOT_JOIN_WHILE_TENKAICHIBUDOKAI,
	SCRAMBLE_CANNOT_JOIN_WHILE_FREEBATTLE,
	SCRAMBLE_CANNOT_REWARD_LIMIT_COUNT_OVER,
	SCRAMBLE_CANNOT_PICK_LIMIT_COUNT_OVER,
	SCRAMBLE_CANNOT_REWARD_WHILE_COOLTIME,
	SCRAMBLE_CANNOT_PICK_WHILE_COOLTIME,
	SCRAMBLE_CANNOT_NO_SEASON,
	SCRAMBLE_CANNOT_REWARD_WHILE_FAINT,
	SCRAMBLE_CANNOT_REWARD_WHILE_SANDBAG,
	SCRAMBLE_CANNOT_REWARD_SEASON_IS_END,


	//--NEW------------------------------------------------------------
	// TENKAICHI DAISIJYOU System ( 13000 ~ )
	//-----------------------------------------------------------------
	TENKAICHIDAISIJYOU_CANNOT_SELL_NO_MONEY,
	TENKAICHIDAISIJYOU_CANNOT_NOT_EXIST,
	TENKAICHIDAISIJYOU_CANNOT_LACK_OF_ITEM_STACK,
	TENKAICHIDAISIJYOU_CANNOT_BUY_NO_MONEY,
	TENKAICHIDAISIJYOU_CANNOT_LIST_IS_NOT,
	TENKAICHIDAISIJYOU_CANNOT_BUY_NOT_MATCH_ITEM,
	TENKAICHIDAISIJYOU_CANNOT_INVALID_ITEM,
	TENKAICHIDAISIJYOU_CANNOT_NOT_SELL_RESTRICT,
	TENKAICHIDAISIJYOU_CANNOT_NOT_SELL_INVALID_TAB_TYPE,
	TENKAICHIDAISIJYOU_CANNOT_NOT_SELL_NOT_TAB_TYPE,
	TENKAICHIDAISIJYOU_CANNOT_SYSTEM_DATA_ERROR,
	TENKAICHIDAISIJYOU_CANNOT_BUY_MYITEM,
	TENKAICHIDAISIJYOU_CANNOT_CANCEL_OTHER_ITEM,
	TENKAICHIDAISIJYOU_CANNOT_SELL_INVALID_TIME,
	TENKAICHIDAISIJYOU_CANNOT_SELL_INVALID_PRICE,
	TENKAICHIDAISIJYOU_CANNOT_FIND_INVALID_LEVEL,
	TENKAICHIDAISIJYOU_CANNOT_SELL_INVALID_DUR,
	TENKAICHIDAISIJYOU_CANNOT_NO_SELL_LIST,
	TENKAICHIDAISIJYOU_CANNOT_LIST_INVALID_ITEMNAME,
	TENKAICHIDAISIJYOU_CANNOT_LIST_COMPLETE,

	//--NEW------------------------------------------------------------
	// Maskot System ( 14000 ~ )
	//-----------------------------------------------------------------
	MASCOT_FAIL,
	MASCOT_ALREADY_EXIST,
	MASCOT_NOT_EXIST,
	MASCOT_WAS_NOT_SUMMONED,
	MASCOT_EXCEED_HAVE_COUNT,
	MASCOT_TBLDAT_FINDING_FAILED,
	MASCOT_ITEM_INVALID,
	MASCOT_RING_ITEM_NOT_EXIST,
	MASCOT_RING_ITEM_ABILITY_NOT_EXIST,
	MASCOT_NOT_ENOUGH_SP,
	MASCOT_BODY_ITEM_IS_INVALID,
	MASCOT_IS_SAME_CUR_MASCOT,
	MASCOT_SP_IS_FULL,
	MASCOT_SP_IS_HUNGRY,
	SUMMONING_MASCOT_CANT_DELETE,
	CANT_MOVE_ITEM_TO_MASCOT_BAG,
	MASCOT_FAIL_ACTIVATE_RING,
	MASCOT_FAIL_NOT_DISASSEMBLE_ITEM,
	MASCOT_FAIL_NOT_ACTIVATE_RING,
	SAME_RING_EXIST_IN_MASCOT_RING_CONTAINER,
	MASCOT_FAIL_INACTIVE_RING_CONTENT,
	MASCOT_FAIL_EXIST_ACTIVE_RING,

	//--NEW------------------------------------------------------------
	// wagu wagu machine System ( 15000 ~ )
	//-----------------------------------------------------------------
	WAGUWAGUMACHINE_FAIL,
	WAGUWAGUMACHINE_NOT_EXIST_MACHINE,
	WAGUWAGUMACHINE_NOT_EXIST_QNTT,
	WAGUWAGUMACHINE_NOT_ENOUGH_COIN,
	WAGUWAGUMACHINE_OVER_MAX_COIN,
	WAGUWAGUMACHINE_INVALID_CHARID,
	WAGUWAGUMACHINE_NOT_LIST_WINNERS,
	WAGUWAGUMACHINE_WINNER_REST,
	WAGUWAGUMACHINE_STOP_MACHINE,
	WAGUWAGUMACHINE_NO_HAVE_ITEMS,
	WAGUWAGUMACHINE_NOT_ENOUGH_ITEMS,
	WAGUWAGUMACHINE_SOME_FAILED_TO_EXTRACT,
	WAGUWAGUMACHINE_OVER_MAX_EVENTCOIN,

	//-----------------------------------------------------------------
	GAME_COMMON_MESSAGE_02,
	GAME_COMMON_ALREADY_REGISTERED,
	GAME_COMMON_NOT_REGISTERED,
	GAME_COMMON_CAN_NOT_DO_HERE,
	GAME_COMMON_CAN_FIND_MEMBER,
	GAME_COMMON_TOO_MANY_PLAYER,
	GAME_COMMON_TOO_FEW_PLAYER,
	GAME_COMMON_HAVE_NOT_THE_REQUIRED_ACHIEVEMENT,
	GAME_COMMON_CAN_NOT_FIND_TABLE_DATA,
	GAME_COMMON_ALREADY_EXIST,
	GAME_COMMON_EXIST_USER_WHO_DO_NOT_AGREE,
	GAME_COMMON_EXIST_USER_WHO_OVER_LIMIT_COUNT,
	GAME_COMMON_LACK_OF_SPACE_REWARD_HAS_BEEN_DELIVERED_BY_MAIL,

	//-----------------------------------------------------------------
	DWC_FAIL,

	//-----------------------------------------------------------------
	GAME_BATTLE_DUNGEON_FAIL,
	GAME_BATTLE_DUNGEON_NOT_READY_ALL,
	GAME_BATTLE_DUNGEON_NOT_IN_RANGE,
	GAME_BATTLE_DUNGEON_NOT_SAME_MEMBER,
	GAME_BATTLE_DUNGEON_NOT_EXIST_MESSAGE,
	GAME_BATTLE_DUNGEON_FAIL_STATE,

	//-----------------------------------------------------------------
	GAME_CLOSED_BOX_FAIL,
	GAME_CLOSED_BOX_KEY_NOT_FOUND,
	GAME_CLOSED_BOX_KEY_NOT_ENOUGH,
	GAME_CLOSED_BOX_KEY_MISMATCH,

	//-----------------------------------------------------------------
	GAME_ITEM_EXCHANGE_FAIL,
	GAME_ITEM_EXCHANGE_NOT_STACKED,
	GAME_ITEM_UPGRADED_EXCHANGE_POWDER_NOT_FOUND,
	GAME_ITEM_UPGRADED_EXCHANGE_POWDER_NEED_MORE,
	GAME_ITEM_UPGRADED_EXCHANGE_OLD_ITEM_GRADE_FAIL,
	GAME_ITEM_UPGRADED_EXCHANGE_NEW_ITEM_GRADE_FAIL,
	GAME_ITEM_UPGRADED_EXCHANGE_LEVEL_FAIL,
	GAME_ITEM_UPGRADED_EXCHANGE_ITEM_FAIL,

	//-----------------------------------------------------------------
	CHARTITLE_NO_HAVE,
	CHARTITLE_YOU_ALREADY_HAVE_THE_TITLE,
	CHARTITLE_TITLE_IS_ON_PROGRESS,

	//-----------------------------------------------------------------
	AIR_FAIL,
	AIR_CANNOT_FLY_JUMP_NOT_ENOUGH_AP,
	GAME_COMMON_CANT_DO_THAT_IN_AIR_MODE_STATE,
	AIR_CANNOT_FLY_ACCEL,

	GAME_SKILL_CANT_USE_SKILL_WHEN_ROLLING_ATTACK,
	GAME_SKILL_CANT_USE_SKILL_WHEN_RABIES,

	GAME_RESULT_LAST, // must be last result


	RESULT_DUMMY = 0xFFFF
	//////////////////////////////////////////////////////////////////////////
};
