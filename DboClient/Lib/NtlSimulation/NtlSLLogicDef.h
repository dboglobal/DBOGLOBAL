/*****************************************************************************
 *
 * File			: NtlSLLogicDef.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 10	
 * Abstract		: Simulation game logic definition
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SLLOGIC_DEF_H__
#define __NTL_SLLOGIC_DEF_H__

#include "NtlSLDef.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// keyboard move flag
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NTL_BIT_FRONT_MOVE			0x01
#define NTL_BIT_BACK_MOVE			0x04
#define NTL_BIT_LEFT_MOVE			0x10
#define NTL_BIT_RIGHT_MOVE			0x20

#define NTL_BIT_TURN_LEFT_MOVE		0x02
#define NTL_BIT_TURN_RIGHT_MOVE		0x08

#define NTL_BIT_LF_MOVE				0x03 // move front turn left
#define NTL_BIT_LB_MOVE				0x06 // move back turn left
#define NTL_BIT_RF_MOVE				0x09 // move front turn right
#define NTL_BIT_RB_MOVE				0x0c // move back turn right

#define NTL_BIT_FL_MOVE				0x11 // move front and left
#define NTL_BIT_FR_MOVE				0x21 // move front and right
#define NTL_BIT_BL_MOVE				0x14 // move back and left
#define NTL_BIT_BR_MOVE				0x24 // move back and right

#define NTL_BIT_LL_MOVE				0x12 // move left turn left
#define NTL_BIT_LR_MOVE				0x18 // move left turn right
#define NTL_BIT_RL_MOVE				0x22 // move right turn left
#define NTL_BIT_RR_MOVE				0x28 // move right turn right

#define NTL_BIT_DOWN_MOVE			0x07 // move down while flying

#define NTL_BIT_FB_MOVE				0x05


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// game logic define
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// world item pick range
#define NTL_WORLD_ITEM_PICK_RANGE	    	1.0f
#define NTL_WORLD_ITEM_AUTOLOOTING_RANGE	10.0f

////////////////////////////////////////////////////////////////////////
// world height data stuff
typedef struct _SWorldHeightStuff
{
	RwBool bWaterAttr;
	RwReal fFinialHeight;
	RwReal fWorldHeight;
	RwReal fWaterHeight;
}SWorldHeightStuff;

////////////////////////////////////////////////////////////////////////
// skill area sort stuff
struct SSkillAreaSortStuff
{
	RwReal			fDist;
	SERIAL_HANDLE	hSerialId;
};


////////////////////////////////////////////////////////////////////////
// swimming offseet
#define NTL_ACTOR_SWIMMIG_OFFSET_RATE		0.5f


////////////////////////////////////////////////////////////////////////
// npc
#define NTL_NPC_COMMUNITY_OUT_RANGE			5.0f

////////////////////////////////////////////////////////////////////////
// trigger object 
#define NTL_TRIGGER_COMMUNITY_IN_RANGE			3.0f
#define NTL_TRIGGER_COMMUNITY_OUT_RANGE			5.0f

////////////////////////////////////////////////////////////////////////
// dyanmic object 
#define NTL_DYNAMIC_COMMUNITY_IN_RANGE			3.0f;
#define NTL_DYNAMIC_COMMUNITY_OUT_RANGE			5.0f;

////////////////////////////////////////////////////////////////////////
// character edge weight value
#define NTL_CHARACTER_EDGE_WEIGHT_DIST		200.0f

////////////////////////////////////////////////////////////////////////
// name color
#define NTL_BUS_NICK_NAME_COLOR				RGB(176, 254, 90)
#define NTL_NPC_NICK_NAME_COLOR				RGB(255, 255, 104)
#define NTL_PLAYER_NAME_COLOR				RGB(255, 185, 83)	// 2008. 3. 11 by Kell
#define NTL_PARTY_NAME_COLOR				RGB(81,  231, 190)
#define NTL_GUILD_NAME_COLOR				RGB(255, 244, 122)
#define NTL_MONSTER_NAME_COLOR				RGB(255, 255, 0)
#define NTL_MONSTER_NAME_COLOR_STRONG		RGB(255, 0, 0)
#define NTL_MONSTER_NAME_COLOR_WEAKNESS		RGB(0, 255, 0)
#define NTL_NPC_NAME_COLOR					RGB(255, 255, 255)	
#define NTL_PET_NAME_COLOR					RGB(255, 255, 255)	
#define NTL_OBJ_NAME_COLOR					RGB(255, 255, 255)
#define NTL_TITLE_NAME_COLOR				RGB(157,  255, 5)
#define NTL_PVP_NAME_COLOR					NTL_MONSTER_NAME_COLOR_STRONG


////////////////////////////////////////////////////////////////////////
// battle
#define NTL_PHYSIC_ATTACK_TARGET_HEGITH		0.7f
#define NTL_ENERGY_ATTACK_TARGET_HEGITH		0.7f

////////////////////////////////////////////////////////////////////////
// quest mark type
enum EQuestMarkType
{
	EQuestMarkNone,
	EQuestMarkNew,			// quest가 시작이지만 거리가 멀때.
	EQuestMarkProgress,		// quest가 진행중일때
	EQuestMarkReward,		// quest가 보상중일때
	EQuestMarkVisit,		// quest 중 방문 서버 이벤트가 동작 중인 경우
	EQuestMarkBalloonNew,	// quest가 시작이지만, 거리가 가까울때
};

enum EQuestMarkFlag
{
	EQuestMarkFlagNew		=	(1<< EQuestMarkNew),
	EQuestMarkFlagProgress	=	(1<< EQuestMarkProgress),
	EQuestMarkFlagReward	=	(1<< EQuestMarkReward),
	EQuestMarkFlagVisit		=	(1<< EQuestMarkVisit)
};

#define NTL_QUEST_MARK_BALLOON_DIST			25.0f

////////////////////////////////////////////////////////////////////////
// collision

#define NTL_CHARACTER_COLLI_NONE							0
#define NTL_CHARACTER_COLLI_OBJECT							1
#define NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT		2
#define NTL_CHARACTER_COLLI_OBJECT_JUMP                     3                   ///< When colliding in an upward direction

#endif
