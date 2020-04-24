/*****************************************************************************
 *
 * File			: NtlBehaviorDef.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 22	
 * Abstract		: actor behavior definition stuff
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIORDEF_H__
#define __NTL_BEHAVIORDEF_H__

#define NTL_MAX_BEHAVIOR_NAME		32

#define NTL_BEHID_INVALID			0

// character behavior id
#define NTL_BEID_CHAR_SPAWN				1000
#define NTL_BEID_CHAR_GROUNDMOVE		1001
#define NTL_BEID_CHAR_GROUNDADJUSTMOVE	1002
#define NTL_BEID_CHAR_SWIMMINGMOVE		1003
#define NTL_BEID_CHAR_JUMP				1004
#define NTL_BEID_CHAR_SMOOTHHEADING		1005
#define NTL_BEID_CHAR_NORMALATTACK		1006
#define NTL_BEID_CHAR_FALLDOWNATTACK	1007
#define NTL_BEID_CHAR_PUSHFOLLOW		1008
#define NTL_BEID_CHAR_PUSH				1009
#define NTL_BEID_CHAR_SLIDING			1010
#define NTL_BEID_CHAR_SHOCK				1011
#define NTL_BEID_CHAR_KNOCKDOWN			1012
#define NTL_BEID_CHAR_HURT				1013
#define NTL_BEID_CHAR_HURTTOSS_UP		1014
#define NTL_BEID_CHAR_HURTTOSS_DOWN		1015
#define NTL_BEID_CHAR_HOMING_UP			1016
#define NTL_BEID_CHAR_HOMING_DOWN		1017
#define NTL_BEID_CHAR_JUMPING_UP		1018
#define NTL_BEID_CHAR_JUMPING_DOWN		1019
#define NTL_BEID_CHAR_BUS_MOVE			1020
#define NTL_BEID_CHAR_AIR_MOVE			1021

// vehicle behavior id
#define NTL_BEID_VEHICLE_MOVE			1100

// MASCOT
#define NTL_BEID_MASCOT_IDLE			1200
#define NTL_BEID_MASCOT_DIRECT_ANIM		1201
#define NTL_BEID_MASCOT_MOVE			1202


// character behavior name
#define NTL_BENAME_CHAR_SPAWN				"Behavior_Spawn"
#define NTL_BENAME_CHAR_GROUNDMOVE			"Behavior_GroundMove"
#define NTL_BENAME_CHAR_GROUNDADJUSTMOVE	"Behavior_GroundAdjustMove"
#define NTL_BENAME_CHAR_SWIMMNGMOVE			"Behavior_SwimmingMove"
#define NTL_BENAME_CHAR_JUMP				"Behavior_Jump"
#define NTL_BENAME_CHAR_SMOOTHHEADING		"Behavior_SmoothHeading"
#define NTL_BENAME_CHAR_NORMALATTACK		"Behavior_NormalAttack"
#define NTL_BENAME_CHAR_FALLDOWNATTACK		"Behavior_FallDownAttack"
#define NTL_BENAME_CHAR_PUSHFOLLOW			"Behavior_PushFollow"
#define NTL_BENAME_CHAR_PUSH				"Behavior_Push"
#define NTL_BENAME_CHAR_SLIDING				"Behavior_Sliding"
#define NTL_BENAME_CHAR_SHOCK				"Behavior_Shock"
#define NTL_BENAME_CHAR_KNOCKDOWN			"Behavior_KnownDown"
#define NTL_BENAME_CHAR_HURT				"Behavior_Hurt"
#define NTL_BENAME_CHAR_HURTTOSS_UP			"Behavior_HurtTossUp"
#define NTL_BENAME_CHAR_HURTTOSS_DOWN		"Behavior_HurtTossDown"
#define NTL_BENAME_CHAR_HOMING_UP			"Behavior_HomingUp"
#define NTL_BENAME_CHAR_HOMING_DOWN			"Behavior_HomingDown"
#define NTL_BENAME_CHAR_JUMPING_UP			"Behavior_JumpingUp"
#define NTL_BENAME_CHAR_JUMPING_DOWN		"Behavior_JumpingDown"
#define NTL_BENAME_CHAR_BUS_MOVE			"Behavior_BusMove"
#define NTL_BENAME_CHAR_AIR_MOVE			"Behavior_Air_Move"

#define NTL_BENAME_VEHICLE_MOVE				"Behavior_VehicleMove"

// new
#define	NTL_BENAME_STRETCH_ATTACK			"Behavior_StretchAttack"
#define	NTL_BENAME_LINKED_TARGET_EFFECT		"Behavior_LinkedTargetEffect"
#define	NTL_BENAME_BATTLE_ADJUST_MOVE		"Behavior_BattleAdjustMove"
#define	NTL_BENAME_MASCOT_IDLE				"Behavior_MascotIdle"
#define	NTL_BENAME_MASCOT_DIRECT_ANIM		"Behavior_MascotDirectAnim"
#define	NTL_BENAME_MASCOT_MOVE				"Behavior_MascotMove"


// projectile behavior id
#define NTL_BEID_PROJ_BALL					2000
#define NTL_BEID_PROJ_BEAM					2001
#define NTL_BEID_PROJ_HISSHIDAN				2002
#define NTL_BEID_PROJ_HELLZONE_GRENADE		2003
#define NTL_BEID_PROJ_MULTI_HISSHIDAN		2004
#define NTL_BEID_PROJ_MAGARE                2005
#define NTL_BEID_PROJ_STEAL					2010



// projectile behavior name
#define NTL_BENAME_PROJ_BALL				"Behavior_ProjBall"
#define NTL_BENAME_PROJ_BEAM				"Behavior_ProjBeam"
#define NTL_BENAME_PROJ_HISSHIDAN			"Behavior_ProjHisshidan"
#define NTL_BENAME_PROJ_HELLZONE_GRENADE	"Behavior_ProjHellzone"
#define NTL_BENAME_PROJ_STEAL				"Behavior_ProjSteal"
#define NTL_BENAME_PROJ_MULTI_HISSHIDAN		"Behavior_ProjMultiHisshidan"
#define NTL_BENAME_PROJ_MAGARE              "Behaviot_ProjMagare"

// actor 좌표 보정에 대한 정보
#define NTL_POSITION_JUMPING_LIMIT		10.0f

#endif
