/*****************************************************************************
*
* File			: NtlSobGroup.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2005. 8. 23	
* Abstract		: Simulation layer visual entity id declear
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SLVISUAL_DECLEAR_H__
#define __NTL_SLVISUAL_DECLEAR_H__


// visual effect key name
#define NTL_VID_MARK_COMMON_MOVE            "GME_Cursor_Ground"             // 마우스 이동 위치 effect(공용)
#define NTL_VID_MARK_TARGET					"GME_Cursor_Target"		        // object target mark 머리위
#define NTL_VID_MARK_TARGET_02              "GME_Cursor_Target_02"          // 발밑 데칼
#define NTL_VID_MARK_ATTACK					"GME_Cursor_Attack"		        // object attack mark 머리위
#define NTL_VID_MARK_ATTACK_02              "GME_Cursor_Attack_02"          // 발밑 데칼
#define NTL_VID_MARK_BOUNDARY				"GME_Cursor_Boundary"			// object attack mark
#define NTL_VID_MARK_CAUTION                "GME_Recognize"                 // 몹이 플레이어를 발각했을때 이펙트
#define NTL_VID_MARK_HELPME_REQUESTER       "GME_Mob_SOS_01"                // 도움을 요청하는 몹 이펙트
#define NTL_VID_MARK_HELPME_REQUESTEE       "GME_Mob_SOS_02"                // 도움을 요청받은 몹 이펙트
#define NTL_VID_MARK_TARGET_MARKING         "M_TPP_MEGADODON_TARGET_01"    // 몹에게 타겟으로 설정됐을때의 이펙트 (메가 도동파)

#define NTL_VID_MARK_SHARE_TARGET_0_NORAML	"GME_Cursor_Party_Assign_01"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_1_NORAML	"GME_Cursor_Party_Assign_02"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_2_NORAML	"GME_Cursor_Party_Assign_03"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_3_NORAML	"GME_Cursor_Party_Assign_04"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_4_NORAML	"GME_Cursor_Party_Assign_05"	// 공유 타겟 마크 1

#define NTL_VID_MARK_SHARE_TARGET_0_TARGET	"GME_Cursor_Party_Target_01"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_1_TARGET	"GME_Cursor_Party_Target_02"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_2_TARGET	"GME_Cursor_Party_Target_03"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_3_TARGET	"GME_Cursor_Party_Target_04"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_4_TARGET	"GME_Cursor_Party_Target_05"	// 공유 타겟 마크 1

#define NTL_VID_MARK_SHARE_TARGET_0_ATTACK	"GME_Cursor_Party_Attack_01"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_1_ATTACK	"GME_Cursor_Party_Attack_02"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_2_ATTACK	"GME_Cursor_Party_Attack_03"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_3_ATTACK	"GME_Cursor_Party_Attack_04"	// 공유 타겟 마크 1
#define NTL_VID_MARK_SHARE_TARGET_4_ATTACK	"GME_Cursor_Party_Attack_05"	// 공유 타겟 마크 1

#define NTL_VID_WORLD_IN					"GME_Spawn1"
#define NTL_VID_TELEPORT_SPAWN				"GME_Spawn2"
#define NTL_VID_TELEPORT_DESPAWN			"GME_Disappear"

#define NTL_VID_HIT_PLAYER_PHYSIC			"GME_Target_Physical_Hit"
#define NTL_VID_HIT_PLAYER_PHYSIC_02		"GME_Target_Physical_Hit_02"
#define NTL_VID_HIT_PLAYER_PHYSIC_POWER		"GME_Target_Physical_Power_Hit"
#define NTL_VID_HIT_PLAYER_PHYSIC_CRITICAL	"GME_Target_Physical_Critical_Hit"
#define NTL_VID_HIT_PLAYER_ENERGY           "GME_Target_Energy_Hit"
#define NTL_VID_HIT_PLAYER_ENERGY_02        "GME_Target_Energy_Hit_02"
#define NTL_VID_HIT_PLAYER_ENERGY_POWER     "GME_Target_Energy_Power_Hit"
#define NTL_VID_HIT_PLAYER_ENERGY_CRITICAL  "GME_Target_Energy_Critical_Hit"

#define NTL_VID_HIT_MOB_PHYSIC				"GME_Physical_Hit"
#define NTL_VID_HIT_MOB_PHYSIC_02			"GME_Physical_Hit_02"
#define NTL_VID_HIT_MOB_PHYSIC_POWER		"GME_Physical_Power_Hit"
#define NTL_VID_HIT_MOB_PHYSIC_CRITICAL		"GME_Physical_Critical_Hit"
#define NTL_VID_HIT_MOB_ENERGY              "GME_Energy_Hit"
#define NTL_VID_HIT_MOB_ENERGY_02           "GME_Energy_Hit_02"
#define NTL_VID_HIT_MOB_ENERGY_POWER        "GME_Energy_Power_Hit"
#define NTL_VID_HIT_MOB_ENERGY_CRITICAL     "GME_Energy_Critical_Hit"

#define NTL_VID_TEXT_BLOCK					"TXT_Block_001"
#define NTL_VID_TEXT_DODGE					"TXT_Miss_001"
#define NTL_VID_TEXT_CRITICAL				"TXT_Critical_001"
#define NTL_VID_TEXT_KNOCKDOWN				"TXT_KnockDown_001"
#define NTL_VID_TEXT_RESISTANCE				"TXT_Resistance_001"
#define NTL_VID_TEXT_IMMUNITY				"GME_Mob_Immunity"

#define NTL_VID_TARGET_BLOCK				"GME_Target_Block"
#define NTL_VID_TARGET_KNOCKDOWN			"GME_Target_Knockdown"
#define NTL_VID_TARGET_SLIDING				"GME_Target_Sliding"

#define NTL_VID_LEVEL_UP					"GME_Level_Up"

#define NTL_VID_JUMP_LANDING				"GME_Smoke_JumpLanding"
#define NTL_VID_RUN_01						"GME_Smoke_Run_01"
#define NTL_VID_RUN_02						"GME_Smoke_Run_02"
#define NTL_VID_RUN_03						"GME_Smoke_Run_03"

#define NTL_VID_WATER_FOOT					"GME_SWIM_FOOT"
#define NTL_VID_WATER_JUMP					"GME_SWIM_JUMP"
#define NTL_VID_SWIM_MOVE					"GME_SWIM_MOVE"
#define NTL_VID_SWIM_MOVE_02				"GME_SWIM_MOVE_02"
#define NTL_VID_SWIM_BACK					"GME_SWIM_BACK"
#define NTL_VID_SWIM_IDLE					"GME_SWIM_STOP"
#define NTL_VID_SWIM_BUBBLE                 "GME_SWIM_BUBBLE"

//dash line
#define NTL_VID_DASH_LINE					"GME_DashLine"

//dash auras
#define NTL_VID_DASH_ACTIVE_01				"COM_SKL_AVE_001"
#define NTL_VID_DASH_ACTIVE_02				"COM_SKL_AVE_001_02"
#define NTL_VID_DASH_ACTIVE_BLUE_01			"COM_SKL_AVE_001_1" // blue dash aura
#define NTL_VID_DASH_ACTIVE_BLUE_02			"COM_SKL_AVE_001_02_1" // blue dash aura
#define NTL_VID_DASH_ACTIVE_GREEN_01		"COM_SKL_AVE_001_2" // green dash aura
#define NTL_VID_DASH_ACTIVE_GREEN_02		"COM_SKL_AVE_001_02_2" // green dash aura
#define NTL_VID_DASH_ACTIVE_GREENT_01		"COM_SKL_AVE_001_3" // refined green dash aura
#define NTL_VID_DASH_ACTIVE_YELLOW_01		"COM_SKL_AVE_001_4" // yellow dash aura
#define NTL_VID_DASH_ACTIVE_YELLOW_02		"COM_SKL_AVE_001_02_4" // yellow dash aura
#define NTL_VID_DASH_ACTIVE_RED_01			"COM_SKL_AVE_001_6" // red dash aura
#define NTL_VID_DASH_ACTIVE_RED_02			"COM_SKL_AVE_001_02_6" // red dash aura
#define NTL_VID_DASH_ACTIVE_REDT_01			"COM_SKL_AVE_001_7" // refined red dash aura
#define NTL_VID_DASH_ACTIVE_REDT_02			"COM_SKL_AVE_001_02_7" // refined red dash aura
#define NTL_VID_DASH_ACTIVE_PURPLE_01		"COM_SKL_AVE_001_8" // purple dash aura
#define NTL_VID_DASH_ACTIVE_PURPLE_02		"COM_SKL_AVE_001_02_8" // purple dash aura




#define NTL_VID_NORMAL_SKILL_USE			"GME_Normal_Skill"
#define NTL_VID_SPECIAL_SKILL_USE			"GME_Special_Skill"
#define NTL_VID_DECAL_SKILL_RANGE_DMG       "GME_ATTACK_RANGE"  ///< Range Decal to indicate the skill's range (radius 1)
#define NTL_VID_DECAL_SKILL_RANGE_HEAL      "GME_HEAL_RANGE"  ///< Range Decal to indicate the skill's range (radius 1)

#define NTL_VID_STATE_STUN					"GME_STS_STUN"
#define NTL_VID_STATE_PARALYZE				"GME_STS_PARALYZE"
#define NTL_VID_STATE_SLEEP					"GME_STS_SLEEP"
#define NTL_VID_STATE_TAUNT					"GME_STS_TAUNT"
#define NTL_VID_STATE_CONFUSED				"GME_STS_CONFUSE"
#define NTL_VID_STATE_TERROR				"GME_STS_TERROR"
#define NTL_VID_STATE_HIT_BARRIER			"GME_STS_BARRIER"
#define NTL_VID_STATE_HIT_REFLECTION		"GME_STS_REFLECTION"
#define NTL_VID_STATE_FREEZE                "GME_STS_FREEZE"
#define NTL_VID_STATE_STOMACH				"GME_STS_STOMACH"
#define NTL_VID_STATE_BLEED					"GME_STS_BLEED"
#define NTL_VID_STATE_POISON				"GME_STS_POISON"

//Guard auras
#define NTL_VID_GUARD_SUCCESS				"GME_Guard_Success" // effect while using guard
#define NTL_VID_GUARD_BLUE					"GME_Guard_01" // blue aura when guarding
#define NTL_VID_GUARD_GREEN					"GME_Guard_02" // green aura when guarding
#define NTL_VID_GUARD_GREENT				"GME_Guard_03" // refined green aura when guarding
#define NTL_VID_GUARD_YELLOW				"GME_Guard_04" // yellow aura when guarding
#define NTL_VID_GUARD_YELLOWT				"GME_Guard_05" // refined yellow aura when guarding
#define NTL_VID_GUARD_RED					"GME_Guard_06" // red aura when guarding
#define NTL_VID_GUARD_REDT					"GME_Guard_07" // refined aura when guarding
#define NTL_VID_GUARD_PURPLE				"GME_Guard_08" // purple aura when guarding

#define NTL_VID_BLOCKING_DEFENCE			"GME_Guard_Attack"
#define NTL_VID_BLOCKING_DAMAGE				"TXT_Guard_Normal"
#define NTL_VID_BLOCKING_CURSE				"TXT_Guard_Curse"
#define NTL_VID_BLOCKING_KNOCKDOWN			"TXT_Guard_Knockdown"
#define NTL_VID_BLOCKING_HTB				"TXT_Guard_HTB"

#define NTL_VID_QUEST_MARK_NEW				"GME_Quest_Start_Mark"
#define NTL_VID_QUEST_MARK_PROGRESS			"GME_Quest_Process_Mark"
#define NTL_VID_QUEST_MARK_REWARD			"GME_Quest_Clear_Mark"
#define NTL_VID_QUEST_MARK_VISIT			"GME_Quest_Meet_Mark"

#define NTL_VID_QUEST_ACCEPT_PROPOSAL		"GME_Quest_Start_Effect"
#define NTL_VID_QUEST_ACCEPT_REWARD_01		"GME_Quest_Clear_Effect_01"
#define NTL_VID_QUEST_ACCEPT_REWARD_02		"GME_Quest_Clear_Effect_02"
#define NTL_VID_QUEST_ACCEPT_REWARD_03		"GME_Quest_Clear_Effect_03"
#define NTL_VID_QUEST_ACCEPT_REWARD_04		"GME_Quest_Clear_Effect_04"

// PVP
#define NTL_VID_FREEPVP_MARK				"N_REF_A1"                      ///< 심판 캐릭터 NPC 
#define NTL_VID_FREEPVP_BOUND_EFFECT        "N_REF_001"                     ///< PVP 바운드 이펙트

// TMQ Direction
#define NTL_VID_DIRECTOR_EFFECT             "GME_Direction_Arrow"
#define NTL_VID_DIRECTOR_MARK               "GME_Target_Mark"

//
#define NTL_VID_DROPITEM_EFF				"I_DRP_APR_001"
#define	NTL_VID_LUCKY_DROP					"GME_Lucky_Drop_Effect"

// private shop
#define NTL_VID_PRIVATESHOP					"PC_SHOP_001"

// Rp Bonus Skill
#define NTL_VID_RPBOUNS_SMALL               "GME_RP_Bonus_Act"
#define NTL_VID_RPBONUS_BIG                 "GME_RP_Bonus_Act_02"
#define NTL_VID_RPBALL_UP                   "COM_SKL_AVE_002_02"

// RP Charge Auras
#define NTL_VID_RP_CHARGE					"COM_SKL_AVE_002" // effect while using rp charge skill (grey aura)
#define NTL_VID_RP_CHARGE_BLUE				"COM_SKL_AVE_002_1" // effect while using rp charge skill (blue aura)
#define NTL_VID_RP_CHARGE_GREEN				"COM_SKL_AVE_002_2" // effect while using rp charge skill (green aura)
#define NTL_VID_RP_CHARGE_GREENT			"COM_SKL_AVE_002_3" // effect while using rp charge skill (refined green aura)
#define NTL_VID_RP_CHARGE_YELLOW			"COM_SKL_AVE_002_4" // effect while using rp charge skill (yellow aura)
#define NTL_VID_RP_CHARGE_YELLOWT			"COM_SKL_AVE_002_5" // effect while using rp charge skill (refined yellow aura)
#define NTL_VID_RP_CHARGE_RED				"COM_SKL_AVE_002_6" // effect while using rp charge skill (red aura)
#define NTL_VID_RP_CHARGE_REDT				"COM_SKL_AVE_002_7" // effect while using rp charge skill (refined red aura)
#define NTL_VID_RP_CHARGE_PURPLE			"COM_SKL_AVE_002_8" // effect while using rp charge skill (purple aura)


// Super Saiyan Effect
#define NTL_VID_SUPER_SAIYAN_SMALL          "HUM_Super_Saiyan"
#define NTL_VID_SUPER_SAIYAN_BIG            "HUM_Super_Saiyan_02"

// Pure Majin 
#define NTL_VID_PURE_MAJIN_CHANGE           "MAJ_SKL_AVE_001"
#define NTL_VID_PURE_MAJIN_CHANGE_ORG       "MAJ_SKL_AVE_001_02"

// 계왕권
#define NTL_VID_KAIOUKEN_SMALL              "COM_SKL_AVE_011_01"
#define NTL_VID_KAIOUKEN_BIG                "COM_SKL_AVE_011_02"

// 캔디 변신
#define NTL_VID_CANDY_CHANGE_ORG            "GME_ITM_BOMB"

// Class Change
#define NTL_VID_CLASS_CHANGE                "GME_Level_Up"

// visual animation key name
#define NTL_ANI_QUEST_ACCEPT_PROPOSAL		1511
#define NTL_ANI_QUEST_ACCEPT_REWARD			1503

// visual sound
#define SOUND_HIT1							"GENERAL_HIT_PHYSICAL_1.wav"
#define SOUND_HIT2							"GENERAL_HIT_PHYSICAL_2.wav"
#define SOUND_HIT_POWER						"GENERAL_HIT_PHYSICAL_HEAVY.wav"
#define SOUND_JUMP_KICK						"GENERAL_JUMP_KICK.wav"
#define SOUND_JUMP_LANDING					"GENERAL_JUMP_LANDING.wav"
#define SOUND_CAUTION                       "GENERAL_F_ATT.wav"

// foot step
#define SOUND_FOOTSTEP_1					"GENERAL_FOOTSTEP_WATER1.wav"
#define SOUND_FOOTSTEP_2					"GENERAL_FOOTSTEP_WATER2.wav"
#define NAMING_SOUND_FOOTSTEP				"Footsteps\\SE_FOOTSTEP"		// SE_FOOTSTEP_[MATERIALTYPE(0-7)]_[RAND(0-3)]
#define NAMING_EFFECT_FOOTSTEP				"GME_FOOTSTEP"					// GME_FOOTSTEP_[MATERIALTYPE(0-7)]_[STEP(L,R)]_[RAND(0-1)]

// 칭호 이펙트
#define NTL_VID_TENKAICHI_WINNNER_01        "GME_TENKAICHI_WINNER_01"
#define NTL_VID_TENKAICHI_WINNNER_02        "GME_TENKAICHI_WINNER_02"
#define NTL_VID_TENKAICHI_WINNNER_03        "GME_TENKAICHI_WINNER_03"

// Vehicle
#define NTL_VID_VEHICLE_SPAWN				"GME_CAPSULE_SPAWN_01"			// GME_CAPSULE_SPAWN_01
#define NTL_VID_VEHICLE_RIDEOFF				"GME_CAPSULE_SPAWN_01"			// GME_CAPSULE_SPAWN_01
#define NTL_VID_VEHICLE_HURT_RIDEOFF		"GME_Vehicle_Crash"				// GME_Vehicle_Crash
#define NTL_VID_VEHICLE_RUN_SMOKE			"GME_Vehicle_RunSmoke"			// GME_Vehicle_RunSmoke
#define NTL_VID_VEHICLE_RUN_SPRAY			"GME_Vehicle_RunSpray"			// GME_Vehicle_RunSpray
#define NTL_VID_VEHICLE_LANDING_NORMAL		"GME_Smoke_JumpLanding"			// GME_Smoke_JumpLanding
#define NTL_VID_VEHICLE_LANDING_WATER		"GME_Vehicle_LandingWater"		// GME_Vehicle_LandingWater

// Dojo seal
#define NTL_VID_DOJO_SEAL_NO_TEAM			"obj_dojo_change_01"
#define NTL_VID_DOJO_SEAL_ATTACK_TEAM		"obj_dojo_red_01"
#define NTL_VID_DOJO_SEAL_DEFENCE_TEAM		"obj_dojo_blue_01"



#endif