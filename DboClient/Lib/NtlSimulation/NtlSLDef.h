/*****************************************************************************
 *
 * File			: NtlSLDef.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 10	
 * Abstract		: Simulation object entity base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SLDEF_H__
#define __NTL_SLDEF_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// simulation object class type 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned int				SERIAL_HANDLE;
#define INVALID_SERIAL_ID			0xffffffff

typedef unsigned int				SYNCEVENT_HANDLE;
#define INVALID_SYNCEVENT_ID		0xffffffff

#define INVALID_SLCLASS_ID			0xffffffff
#define MAX_SLCLASS_NAME			32

#define INVALID_INDEX				0xffffffff

typedef void*						CINEMATIC_HANDLE;
#define INVALID_CINEMATIC_ID		0x00000000

//--------------------------------------------------------------
// simulation class id

#define SLCLASS_AVATAR				1 // myself
#define SLCLASS_PLAYER				2 // other players
#define SLCLASS_NPC					3
#define SLCLASS_MONSTER				4
#define SLCLASS_PET					5
#define SLCLASS_WORLD_ITEM			6
#define SLCLASS_SLOT_ITEM			7
#define SLCLASS_SLOT_QUESTITEM		8
#define SLCLASS_PROJECTILE			9
#define SLCLASS_INFLUENCE			10
#define SLCLASS_BUFF				11
#define SLCLASS_SKILL				12
#define SLCLASS_HTBSKILL			13
#define SLCLASS_ACTIONSKILL			14
#define SLCLASS_SYNC_STATUS			15
#define SLCLASS_ITEM_ICON			16
#define SLCLASS_QUESTITEM_ICON		17
#define SLCLASS_BUFF_ICON			18
#define SLCLASS_SKILL_ICON			19
#define SLCLASS_HTBSKILL_ICON		20
#define SLCLASS_ACTIONSKILL_ICON	21
#define SLCLASS_MERCHANT_ITEM		22
#define SLCLASS_TRIGGER_OBJECT		23
#define SLCLASS_EVENT_OBJECT        24
#define SLCLASS_VEHICLE				25
#define SLCLASS_DYNAMIC_OBJECT		26

#define MAX_SLCLASS					26

//--------------------------------------------------------------
// simulation class name

#define SLCLASS_NAME_AVATAR				"Sob_Avatar"
#define SLCLASS_NAME_PLAYER				"Sob_Player"
#define SLCLASS_NAME_NPC				"Sob_Npc"
#define SLCLASS_NAME_MONSTER			"Sob_Monster"
#define SLCLASS_NAME_PET				"Sob_Pet"
#define SLCLASS_NAME_WORLD_ITEM			"Sob_WorldItem"
#define SLCLASS_NAME_SLOT_ITEM			"Sob_SlotItem"
#define SLCLASS_NAME_SLOT_QUESTITEM		"Sob_SlotQuestItem"
#define SLCLASS_NAME_PROJECTILE			"Sob_Projectile"
#define SLCLASS_NAME_INFLUENCE			"Sob_Influence"
#define SLCLASS_NAME_BUFF				"Sob_SlotBuff"
#define SLCLASS_NAME_SKILL				"Sob_SlotSkill"
#define SLCLASS_NAME_HTBSKILL			"Sob_SlotHTBSkill"
#define SLCLASS_NAME_ACTIONSKILL		"Sob_SlotActionSkill"
#define SLCLASS_NAME_DAMAGE_STATUS		"Sob_DamageStatus"
#define SLCLASS_NAME_ITEM_ICON			"Sob_ItemIcon"
#define SLCLASS_NAME_QUESTITEM_ICON		"Sob_QuestItemIcon"
#define SLCLASS_NAME_BUFF_ICON			"Sob_BuffIcon"
#define SLCLASS_NAME_SKILL_ICON			"Sob_SkillIcon"
#define SLCLASS_NAME_HTBSKILL_ICON		"Sob_HTBSkillIcon"
#define SLCLASS_NAME_ACTIONSKILL_ICON	"Sob_ActionSkillIcon"
#define SLCLASS_NAME_MERCHANT_ITEM		"Sob_MerchantItem"
#define SLCLASS_NAME_TRIGGER_OBJECT		"Sob_TriggerObject"
#define SLCLASS_NAME_EVENT_OBJECT       "Sob_EventObject"
#define SLCLASS_NAME_VEHICLE			"Sob_Vehicle"
#define SLCLASS_NAME_DYNAMIC_OBJECT		"Sob_DynamicObject"

//--------------------------------------------------------------
// simulation object fsm controller

#define SLCONTROLLER_PLYAER					0x0001		// avatar control
#define SLCONTROLLER_PLAYER_SYNC			0x0002		// player sync control
#define SLCONTROLLER_SERVER					0x0004		// server가 control
#define SLCONTROLLER_SUMMON_PET				0x0008		// 자기가 control하는 summonn pet
#define SLCONTROLLER_ITEM_PET				0x0010		// 자기가 control하는 item pet
#define SLCONTROLLER_VEHICLE				0x0020		// Vehicle control
#define SLCONTROLLER_VEHICLE_SYNC			0x0040		// Vehicle sysnc control


//--------------------------------------------------------------
// simulation object flags

#define SLFLAG_ADD_UPDATE					0x00000001		// update에 추가하라.
#define SLFLAG_UPDATE_PAUSE					0x00000002		// update를 pause를 하라.
#define SLFLAG_NOT_INPUT					0x00000004		// input에 대한 처리 불가.
#define SLFLAG_NOT_VISIBLE					0x00000008		// Do not render.
#define SLFLAG_CAN_SKILLUSE					0x00000010		// It is an object that can use skill.
#define SLFLAG_CAN_REVIVAL					0x00000020		// Is it possible to resurrect?
#define SLFLAG_LUA_INTERLOCKING				0x00000040		// link with lua.
#define SLFLAG_LUA_TRANSFORM_ENABLE			0x00000080		// Is it possible to transform objects in lua?
#define SLFLAG_NOT_HITSHOCK					0x00000100		// Do not use a hit shock.
#define SLFLAG_CAMERASHAKE_ENABLE			0x00000200		// Is camera shake an available object?
#define SLFLAG_CAN_GROUND_FLY				0x00000400		// An object that flies low along the terrain.
#define SLFLAG_OBJECT_COLLISION				0x00000800		// object The object whose conflict should be handled.
#define SLFLAG_WEIGHT_ELAPSED_TIME			0x00001000		// Quickly update an object to an invisible state (used by HTB)
#define SLFLAG_NOT_WORLD_HEIGHT_CHECK		0x00002000		// Do not check the height.
#define SLFLAG_SERVER_SENDER				0x00004000		// Is it the object to send a packet to the server?
															// Usually it is sent from the fsm player controller, but sometimes it is sent from the state.
															// Use in this case.
#define SLFLAG_TARGET_SELECT_GUI_UPDATE		0x00008000		// Should I update the gui when I select target?
#define SLFLAG_TARGET_NEAR_APPROACH			0x00010000		// When you select a target, you approach the nearest location.
#define SLFLAG_TARGET_TRIGGER_OPERATION		0x00020000		// Should target trigger operation?
#define SLFLAG_NOT_TARGETED					0x00040000		// Is the target an object?
#define SLFLAG_NOT_HIGHLIGHTED				0x00080000		//Is target a highlightable object?
#define SLFLAG_CAN_SWIMMING					0x00100000		// Objects that can swim
#define SLFLAG_HURT_NOT_HEADING_CHANGE		0x00200000		// Do not look for objects to be attacked even if they are attacked.
#define SLFLAG_FALLING_ENABLE				0x00400000		// Is it possible to use the falling state?
#define SLFLAG_CAN_AUTO_FOLLOW_TARGET		0x00800000		// Is it appropriate as an automatic follow-up target?
#define SLFLAG_LARGE_FOOT_SOUND             0x01000000      // Is it an object that sounds like a huge mob footprint?
#define SLFLAG_NOT_ALPHA_CAMERA_COLLI		0x02000000		// Should the camera crash if it crashes?

//--------------------------------------------------------------
// simulation object actor flags

#define SLFLAG_ACTOR_HAVE_LIFE				0x00000001
#define SLFLAG_ACTOR_HAVE_MOVEABLE			0x00000002
#define SLFLAG_ACTOR_HAVE_BATTLE			0x00000004
#define SLFLAG_ACTOR_HAVE_SOCIAL			0x00000008
#define SLFLAG_ACTOR_CALC_HEIGHT            0x00000010      // Set this flag if you need to recalculate the height.

//--------------------------------------------------------------
// 객체 update 우선순위

#define SLUPDATE_PRIORITY_CROWD             11
#define SLUPDATE_PRIORITY_INPUT				10		
#define SLUPDATE_PRIORITY_SOB				9		
#define SLUPDATE_PRIORITY_CINEMATIC			8
#define SLUPDATE_PRIORITY_STAGE				1

//--------------------------------------------------------------
// name define

#define NTL_MAX_BONE_NAME					32
#define NTL_MAX_EFFECT_NAME					40
#define NTL_MAX_DIRECT_NAME					32
#define NTL_MAX_PRIVATESHOP_NAME			33
#define NTL_MAX_SPLINE_NAME					32

//--------------------------------------------------------------
// warehouse index

#define NTL_COMMON_WAREHOUSE				3

//--------------------------------------------------------------
// invalid value

#define NTL_INVALID_LP						0xffffffff
#define NTL_INVALID_EP						0xffffffff
#define NTL_INVALID_RP						0xffffffff
#define NTL_INVALID_EXP						0xffffffff
#define NTL_INVALID_LEVEL					0xff
#define NTL_INVALID_VALUE					0xffffffff

//--------------------------------------------------------------
// team infomation
enum eDBO_TEAM
{
	DBO_TEAM_MY_TEAM,
	DBO_TEAM_ENEMY,
	DBO_TEAM_NEUTRAILITY,

	NUM_DBO_TEAM
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// file / directory definition
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NTL_GAME_TABLE_DATA_XML_FOLDER		"devdata\\table"
#define NTL_GAME_TABLE_DATA_BIN_FOLDER		"data"
#define NTL_GAME_TABLE_DATA_ENC_FOLDER		"data"
#define NTL_GAME_DT_DATA_FILE				"property\\direct.dsf"
#define NTL_GAME_LUA_DATA_FOLDER			"script"


//--------------------------------------------------------------
// game development user key

#define NTL_GAME_DEVUSER_KEY				"devmaster.xml"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// memory pool macro
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// default state machine pool Define the number.
#define NTL_DEFAULT_MEMORY_POOL			50
#define NTL_SMALL_MEMORY_POOL			10
#define NTL_LARGE_MEMORY_POOL			100
#define NTL_HARD_LARGE_MEMORY_POOL		500

#define DECLEAR_MEMORY_POOL(class_name, blocksize) \
public: \
	static RwInt32 m_i##class_name##AddRef; \
	static RwFreeList *m_p##class_name##FreeList; \
	void* operator new(size_t size)  \
	{ \
		if(m_p##class_name##FreeList == NULL) \
		{ \
			m_p##class_name##FreeList = RwFreeListCreate(sizeof(class_name), blocksize, 16, rwMEMHINTDUR_GLOBAL); \
		} \
		m_i##class_name##AddRef++; \
		return RwFreeListAlloc(m_p##class_name##FreeList, rwMEMHINTDUR_GLOBAL); \
	} \
	void operator delete(void *pObj) \
	{ \
		if(m_p##class_name##FreeList) \
		{ \
			RwFreeListFree(m_p##class_name##FreeList, pObj); \
			m_i##class_name##AddRef--; \
			if(m_i##class_name##AddRef <= 0) \
			{ \
				RwFreeListDestroy(m_p##class_name##FreeList); \
				m_p##class_name##FreeList = NULL; \
			} \
		} \
	} 


#define DEFINITION_MEMORY_POOL(class_name) \
	RwInt32 class_name::m_i##class_name##AddRef = 0; \
	RwFreeList* class_name::m_p##class_name##FreeList = 0;


#endif