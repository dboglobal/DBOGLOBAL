/*****************************************************************************
 *
 * File			: NtlFSMDef.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 18	
 * Abstract		: Finite state machine definition
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_DEF_H__
#define __NTL_FSM_DEF_H__


// FSMSN => finite state machine state name의 약어.

#define NTL_FSMSID_INVALID			0
#define NTL_FSMSID_CREATE			1
#define NTL_FSMSID_SPAWN			2
#define NTL_FSMSID_IDLE				3
#define NTL_FSMSID_MOVE				4
#define NTL_FSMSID_SWIMMING			5
#define NTL_FSMSID_ADJUSTMOVE		6
#define NTL_FSMSID_DASH				7
#define NTL_FSMSID_JUMP				8
#define NTL_FSMSID_FALLING			9
#define NTL_FSMSID_CHARGING			10
#define NTL_FSMSID_BLOCKING			11
#define NTL_FSMSID_SITDOWN			12	// 13 in tw
#define NTL_FSMSID_SIT				13
#define NTL_FSMSID_STANDUP			14
#define NTL_FSMSID_SMOOTH_HEADING	15
#define NTL_FSMSID_TRIGGER_OPERATE	16
#define NTL_FSMSID_FOLLOW           17
#define NTL_FSMSID_FIGHTING_FOLLOW	18
#define NTL_FSMSID_FIGHTING_POSE	19
#define NTL_FSMSID_FIGHTING			20
#define NTL_FSMSID_FIGHTING_SHRINK	21
#define NTL_FSMSID_HURT				22
#define NTL_FSMSID_KNOCKDOWN		23
#define NTL_FSMSID_SLIDING			24
#define NTL_FSMSID_STUN				25
#define NTL_FSMSID_SLEEP			26
#define NTL_FSMSID_PARALYZE			27
#define NTL_FSMSID_SKILL_FOLLOW		28
#define NTL_FSMSID_SKILL_FOCUS		29
#define NTL_FSMSID_SKILL_READY		30
#define NTL_FSMSID_SKILL_CASTING	31
#define NTL_FSMSID_SKILL_ACTION		32
#define NTL_FSMSID_SOCIAL_ACTION	33
#define NTL_FSMSID_HTB_FOLLOW		34
#define NTL_FSMSID_HTB_READY		35
#define NTL_FSMSID_HTB				36
#define NTL_FSMSID_HTB_SANDBAG		37
#define NTL_FSMSID_ITEM_READY		38
#define NTL_FSMSID_ITEM_CASTING		39
#define NTL_FSMSID_FAINTING			40		// in tw its 41
#define NTL_FSMSID_DIE				41		// in tw its 42
#define NTL_FSMSID_TELEPORT			42
#define NTL_FSMSID_PRIVATESHOP		43
#define NTL_FSMSID_DIRECTION		44
#define NTL_FSMSID_DESPAWN			45
#define NTL_FSMSID_LEAVING			46
#define NTL_FSMSID_ONBUS            49                          ///< 버스 탑승 상태
#define NTL_FSMSID_ONVEHICLE        50                          ///< 탈것 탑승 상태
#define NTL_FSMSID_TURNING          51                          ///< 버스가 회전하는 상태
#define NTL_FSMSID_SEQUELA          52                          ///< 초사이야인 변신 휴유증 상태 // 52 in tw
#define NTL_FSMSID_SPIN_ATTACK      53                          ///< 회전 공격중인 상태
#define NTL_FSMSID_PUSHING          54                          ///< 회전 공격을 당해서 Pushing 상태
#define NTL_FSMSID_RIDEONOFF        55                          ///< 탈것에 타거나 내리는 상태
#define NTL_FSMSID_KEEPING_EFFECT   56                          ///< 스킬 Action Loop 상태
#define NTL_FSMSID_AIR_JUMP			57
#define NTL_FSMSID_AIR_ACCEL		58
#define NTL_FSMSID_ROLLING_ATTACK   59

#define NTL_FSMSN_CREATE			"State_Create"
#define NTL_FSMSN_SPAWN				"State_Spawn"
#define NTL_FSMSN_IDLE				"State_Idle"
#define NTL_FSMSN_MOVE				"State_Move"
#define NTL_FSMSN_SWIMMING			"State_Swimming"
#define NTL_FSMSN_ADJUSTMOVE		"State_AdjustMove"
#define NTL_FSMSN_DASH				"State_Dash"
#define NTL_FSMSN_JUMP				"State_Jump"
#define NTL_FSMSN_FALLING			"State_Falling"
#define NTL_FSMSN_CHARGING			"State_Charging"
#define NTL_FSMSN_BLOCKING			"State_Blocking"
#define NTL_FSMSN_SITDOWN			"State_SitDown"
#define NTL_FSMSN_SIT				"State_Sit"
#define NTL_FSMSN_STANDUP			"State_StandUp"
#define NTL_FSMSN_SMOOTH_HEADING	"State_SmoothHeading"
#define NTL_FSMSN_TRIGGER_OPERATE	"State_TriggerOperate"
#define NTL_FSMSN_FOLLOW			"State_Follow"
#define NTL_FSMSN_FIGHTING_FOLLOW	"State_FightingFollow"
#define NTL_FSMSN_FIGHTING_POSE		"State_FightingPose"
#define NTL_FSMSN_FIGHTING			"State_Fighting"
#define NTL_FSMSN_FIGHTING_SHRINK	"State_FightingShrink"
#define NTL_FSMSN_HURT				"State_Hurt"
#define NTL_FSMSN_KNOCKDOWN			"State_KnockDown"
#define NTL_FSMSN_SLIDING			"State_Sliding"
#define NTL_FSMSN_STUN				"State_Stun"
#define NTL_FSMSN_SLEEP				"State_Sleep"
#define NTL_FSMSN_PARALYZE			"State_Paralyze"
#define NTL_FSMSN_SKILL_FOLLOW		"State_SkillFollow"
#define NTL_FSMSN_SKILL_FOCUS		"State_SkillFocus"
#define NTL_FSMSN_SKILL_READY		"State_SkillReady"
#define NTL_FSMSN_SKILL_CASTING		"State_SkillCasting"
#define NTL_FSMSN_SKILL_ACTION		"State_SkillAction"
#define NTL_FSMSN_SOCIAL_ACTION		"State_SocialAction"
#define NTL_FSMSN_HTB_FOLLOW		"State_HTBFollow"
#define NTL_FSMSN_HTB_READY			"State_HTBReady"
#define NTL_FSMSN_HTB				"State_HTB"
#define NTL_FSMSN_HTB_SANDBAG		"State_HTBSandbag"
#define NTL_FSMSN_ITEM_READY		"State_ItemReady"
#define NTL_FSMSN_ITEM_CASTING		"State_ItemCasting"
#define NTL_FSMSN_FAINTING			"State_Fainting"
#define NTL_FSMSN_DIE				"State_Die"
#define NTL_FSMSN_TELEPORT			"State_Teleport"
#define NTL_FSMSN_PRIVATESHOP		"State_PrivateShop"
#define NTL_FSMSN_DIRECTION			"State_Direction"
#define NTL_FSMSN_DESPAWN			"State_Despawn"
#define NTL_FSMSN_LEAVING			"State_Leaving"
#define NTL_FSMSN_ONBUS             "State_OnBus"
#define NTL_FSMSN_ONVEHICLE         "State_OnVehicle"
#define NTL_FSMSN_TURNNING          "State_Turning"
#define NTL_FSMSN_SEQUELA           "State_Sequela"
#define NTL_FSMSN_SPIN_ATTACK       "State_Spin_Attack"
#define NTL_FSMSN_PUSHING           "State_Pushing"
#define NTL_FSMSN_RIDEONOFF         "State_RideOnOff"
#define NTL_FSMSN_KEEPING_EFFECT    "State_KeepingEffect"
#define NTL_FSMSN_AIR_JUMP			"State_AirJump"
#define NTL_FSMSN_AIR_ACCEL			"State_AirAccel"
#define NTL_FSMSN_ROLLING_ATTACK	"State_Rolling_Attack"

// FSMSF => finite state machine state flag의 약어.
#define NTL_FSMSF_NOT_MOVE				0x00000001	// No mouse movement.
#define NTL_FSMSF_NOT_ATTACK_STATE		0x00000002	// Can not switch attack state.
#define NTL_FSMSF_NOT_HURT_STATE		0x00000004	// hurt status can not be switched.
#define NTL_FSMSF_NOT_JUMP				0x00000008	// No jump possible.
#define NTL_FSMSF_NOT_SKILLUSE			0x00000010	// Disable skill.
#define NTL_FSMSF_NOT_SOCIALUSE			0x00000020	// Disable social action.
#define NTL_FSMSF_NOT_SIT				0x00000040	// You can not use sit / stand.
#define NTL_FSMSF_NOT_CAHNGE_HEADING	0x00000080	// You can not change the heading.
#define NTL_FSMSF_NOT_ADJUSTMOVE		0x00000100	// Can not compensate movement ...
#define NTL_FSMSF_NOT_STATECHANGE		0x00000200	//The state can not be transferred.
#define NTL_FSMSF_NOT_ATTACK_CAN		0x00000400	// I can not attack.
#define NTL_FSMSF_NOT_HURT_CAN			0x00000800	// I can not be shot.
#define NTL_FSMSF_NOT_RUNJUMP			0x00001000	// I can not run jump.
#define NTL_FSMSF_NOT_INPUT				0x00002000	// You can not handle events related to input.
#define NTL_FSMSF_NOT_GROUNDMOVE		0x00004000	// It does not move along the height of the world.

#define NTL_FSMSF_SKIP_SITDOWN			0x01000000	// sit down 사용 skip.	
#define NTL_FSMSF_SKIP_STANDUP			0x02000000  // stand up 사용 skip.
#define NTL_FSMSF_SKIP_ATTACK_CLICK		0x04000000  // attack click 사용 skip.


#define NTL_FSMSF_NOT_ALL				0x07007fff

// 비쥬얼 관련 FSM 플래그 (이 플래그들은 NOT_ALL에 포함되지 않는다)
#define NTL_FSMSF_VISUAL_RPBOUNS             0x10000000  // RP Bonus Effect 표시 가능 플래그


// FSM controller type
#define NTL_FSM_INPUT_CONTROLLER	0x00000001
#define NTL_FSM_AI_CONTROLLER		0x00000002
#define NTL_FSM_SERVER_CONTROLLER	0x00000004


// FSM event에 대한 처리 결과.
#define NTL_FSM_EVENTRES_PASS				0x00000001
#define NTL_FSM_EVENTRES_BLOCK				0x00000002
#define NTL_FSM_EVENTRES_CHANGE_STATE		0x00000003
#define NTL_FSM_EVENTRES_QUEUING			0x00000004
#define NTL_FSM_EVENTRES_QUEUING_REFRESH	0x00000005

#endif
