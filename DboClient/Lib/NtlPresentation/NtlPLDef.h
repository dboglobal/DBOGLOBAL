/*****************************************************************************
*
* File			: NtlPLDef.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2005. 8. 01	
* Abstract		: Presentation layer standard define
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_PLDEF_H__
#define __NTL_PLDEF_H__

#include "NtlCharacter.h"

enum ENtlPLEntityLayer
{
	PLENTITY_LAYER_INVALID,
	PLENTITY_LAYER_NONE,
	PLENTITY_LAYER_FOG,
	PLENTITY_LAYER_SKY,	
	PLENTITY_LAYER_RWWORLD,				// renderware로 포함되는 모든 entity들(woody1019, jhssugi)
	PLENTITY_LAYER_DECAL,
	PLENTITY_LAYER_PLAYERNAME,
	PLENTITY_LAYER_DAMAGEBOX,
	PLENTITY_LAYER_SOUND,
	PLENTITY_LAYER_EFFECT,
	PLENTITY_LAYER_LIGHT,
	PLENTITY_LAYER_SHORELINE,
	PLENTITY_LAYER_WEATHER,
	PLENTITY_LAYER_OCCLUDER,
	PLENTITY_LAYER_HEATHAZE,
	PLENTITY_LAYER_TOOL,
	PLENTITY_LAYER_GUI,
	PLENTITY_LAYER_MAX
};


enum ENtlPLEntityType
{
	PLENTITY_SKY,
	PLENTITY_WORLD,			// woody1019
	PLENTITY_DUMMY_WORLD,	// jhssugi
	PLENTITY_OCCLUDER,
	PLENTITY_WATER,
	PLENTITY_SHORELINE,
	PLENTITY_OBJECT,
	PLENTITY_CHARACTER,
	PLENTITY_ITEM,			//by HoDong
	PLENTITY_EFFECT,	// 10 in tw?
	PLENTITY_SPLINE,		// by agebreak
	PLENTITY_DECAL,			// by jhssugi
	PLENTITY_PLAYER_NAME,
	PLENTITY_DAMAGE_BOX,
	PLENTITY_LIGHT,
	// CZ-SS
	//PLENTITY_SPOT_LIGHT,
	PLENTITY_FOG,
	PLENTITY_GUI,
	PLENTITY_SOUND,
	PLENTITY_SOUND_BGM,
	PLENTITY_BLOOM,
	PLENTITY_SUN,			// by agebreak
	PLENTITY_SUN_PLANET,	// FreeList 문제로 클래스형을 구분하기 위해 추가.
	PLENTITY_PLANT,
	PLENTITY_WORLD_LIGHT,
	PLENTITY_WEATHER,
	PLENTITY_HEATHAZE,
	PLENTITY_LIGHT_OBJECT,	// 28 in tw?
	PLENTITY_GAME_PROPERTY,
	PLENTITY_DECAL_VOLUME,
	PLENTITY_DOJO,

	PLENTITY_END,
	PLENTITY_INVALID_TYPE = 0xffffffff
};

// PLEN => presentation layer entity name의 약어.
#define NTL_PLEN_SKY			"PLSky"
#define NTL_PLEN_WORLD			"PLWorld"		// woody1019
#define NTL_PLEN_DUMMY_WORLD	"PLDummyWorld"	// jhssugi
#define NTL_PLEN_WATER			"PLWater"
#define NTL_PLEN_SHORELINE		"PLShoreLIne"
#define NTL_PLEN_OBJECT			"PLObject"
#define NTL_PLEN_CHARACTER		"PLCharacter"
#define NTL_PLEN_ITEM			"PLItem"
#define NTL_PLEN_EFFECT			"PLEffect"
#define NTL_PLEN_SPLINE			"PLSpline"
#define NTL_PLEN_DECAL			"PLDecal"
#define NTL_PLEN_PLAYER_NAME	"PLPlayerName"
#define NTL_PLEN_DAMAGE_BOX		"PLDamageBox"
#define NTL_PLEN_LIGHT			"PLLight"
// CZ-SS
//#define NTL_PLEN_SPOT_LIGHT		"PLSpotLight"
#define NTL_PLEN_FOG			"PLFog"
#define NTL_PLEN_GUI			"PLGui"
#define NTL_PLEN_SOUND			"PLSound"
#define NTL_PLEN_SOUND_BGM		"PLSoundBGM"
#define NTL_PLEN_BLOOM			"PLBloom"
#define NTL_PLEN_PLANT			"PLPlant"
#define NTL_PLEN_WORLD_LIGHT	"PLWorldLight"
#define NTL_PLEN_WEATHER		"PLWeather"
#define NTL_PLEN_HEATHAZE		"PLHeatHaze"
#define NTL_PLEN_LIGHT_OBJECT	"PLLightObject"
#define NTL_PLEN_GAME_PROPERTY	"PLGameProperty"
#define NTL_PLEN_DECAL_VOLUME	"PLDecalVolume"
#define NTL_PLEN_DOJO			"PLDojo"

/// entity flag
// CNtlPLEntity::SetFlags(RwUInt32 uiFlags); 함수를 사용하여 setting 해 준다.
#define NTL_PLEFLAG_NOTUPDATE					0x00000001	// entity들중 update 가 필요 없을 때 setting 한다(예 : animation이 없는 object).
#define NTL_PLEFLAG_FRUSTUMOUT_NOTUPDATE		0x00000002	// entity들중 camera frustum 밖에 있을 경우 update를 하지 않아도 되는 경우에 setting 해 준다.
#define NTL_PLEFLAG_ALPHA						0x00000004	// entity에 alpha flag를 setting 해 준다.
#define NTL_PLEFLAG_PICKING						0x00000008	// entity들중 mouse picking을 할 수 있는 넘들.( 속도를 위하여...)
#define NTL_PLEFLAG_NOT_ADD_WORLD				0x00000010  // entity들중 RpWorld에 추가하지 않아도 되는 entity인 경우.
#define NTL_PLEFLAG_NOT_PROPERTY_USED			0x00000020	// entity들중 CNtlPLProperty 를 전혀 사용하지 않는 entity 경우.
#define NTL_PLEFLAG_NOT_VISIBLE					0x00000040	// entity들중 rendering on/off flags.
#define NTL_PLEFLAG_ATTACH						0x00000080	// 현재 entity는 attach 가능한 entity 이다.
#define NTL_PLEFLAG_COLLISION					0x00000100  // 현재 entity는 Collision이 가능하고, 올라가는것이 가능하다.
#define NTL_PLEFLAG_WEIGHT_ELAPSED_TIME			0x00000200  // 현재 entity는 update elapsed time 속도를 조절한다.
#define NTL_PLEFLAG_SELECTION					0x00000400	// 트리거가 있는 오브젝트중에서 선택이 되는가 마는가를 결정한다.
#define NTL_PLEFLAG_TRIGGER					    0x00000800	// 트리거가 있는지 없는지를 판단한다.
#define NTL_PLEFLAG_ALTERABLE					0x00001000	// 오브젝트가 동적변형이 있는지를 판단한다.
#define NTL_PLEFLAG_PORTAL						0x00002000	// 포탈기능이 있는지 없는지를 판단한다.
#define NTL_PLEFLAG_PECOLLISION					0x00004000	// Path engine용 2D collision mesh의 사용유무를 판단한다.
#define NTL_PLEFLAG_FADE                        0x00008000  // Fade 적용 유무 플래그
#define NTL_PLEFLAG_SHADOW                      0x00010000  // Entity의 Shadow 표시 유무 플래그
#define NTL_PLEFLAG_NEST_MEMBER                 0x00020000  // Nest Spawn Member
#define NTL_PLEFLAG_RESIZE_PICKING_BBOX         0x00040000  // Flag to dynamically bound the bounding box for picking
#define NTL_PLEFLAG_TOOL_PICK					0x00080000	// 툴에서는 더비오브젝등을 움직이면서 배치하는 경우가 있다. 이경우때문에 픽커와 충돌이 생기게 되는데 이를 방지하기 위해 이 플래그를 쓴다. 이플래그가 없는 경우에만 픽킹이 되도록 NTLPicking에서 처리된다.
#define NTL_PLEFLAG_OBJECT_FORCE_VISIBLE		0x00100000	// 오브젝트가 내부적인 처리(Fade, Distance Culling...)에 영향을 받지않고, 오직 외부에서만 Visible, Alpha값을 조정하기 위한 플래그 (트리거 오브젝트의 상태처리 위해서 사용된다) - 맵툴에서는 적용되지 않는다
#define NTL_PLEFLAG_SHADOW_PROP					0x00200000	// terrain property를 적용해야 할 경우.
#define NTL_PLEFLAG_DUMMY						0X00400000	// ex. GUI전용 character가 생성되는 경우에 사용한다.

// entity cull flags : 우선 순위가 높을 수록 비트수가 낮다.
#define NTL_PLEFLAG_CULLED_PVS							0x00000001 // pvs test
#define NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT			0x00000002 // camera frustum out
#define NTL_PLEFLAG_CULLED_OCCLUDER						0x00000004 // occluder culled

// Pick order
#define NTL_PICK_ORDER_NORMAL					(0)
#define NTL_PICK_ORDER_HIGH						(1)


#define NTL_PLEFLAG_MINIMAP_LAYER_NONE				(0x00000000)
#define NTL_PLEFLAG_MINIMAP_LAYER_ONE				(0x00000001)
#define NTL_PLEFLAG_MINIMAP_LAYER_TWO				(0x00000002)

// item

// Item의 Grade Enum
enum ENtlPLItemGrade
{
	ITEM_GRADE_NONE = 0,
	ITEM_GRADE_1,
	ITEM_GRADE_2,
	ITEM_GRADE_3,
	ITEM_GRADE_4,
	ITEM_GRADE_5,
	ITEM_GRADE_6,
	ITEM_GRADE_7,
	ITEM_GRADE_8,
	ITEM_GRADE_9,
	ITEM_GRADE_10,
	ITEM_GRADE_11,
	ITEM_GRADE_12,
	ITEM_GRADE_13,
	ITEM_GRADE_14,
	ITEM_GRADE_15,
};

// emblem type
enum eEmblemType
{
	EMBLEMTYPE_A,
	EMBLEMTYPE_B,
	EMBLEMTYPE_C,

	NUM_EMBLEMTYPE
};

#define NTL_ITEM_ICON_SIZE				32	
#define NTL_ITEM_SMALL_ICON_SIZE		16	


// icon name
#define MONEYICON_NAME				"I_SYS_Zenny.png"
#define UNIDENTIFIED_ICON_NAME		"i_ucap.png"
#define SEALED_ICON_NAME			"i_lot_blu.png"
#define UNABLE_SLOT_NAME			"Unable_Slot.png"

// effect key name

#define _COMPILE_OPTION_USE_HEIGHT_MAP_				// Use Height Map


#define VISIBLE_BOUND		( 2 )
#define INTERPOLATE_OFFSET	( 1.0f )

#define rwID_WORLD_PLUG_IN_ID	( 0xF3 )

// Path
#define PATH_COLLISION_OBJECT   "Object\\col_mesh\\"
#define PATH_EFFECT_RESOURCE	"Effect\\"
#define PATH_EFFECT_TEXTURE		".\\Texture\\ntlwe\\Planet\\;.\\Texture\\Effect\\;.\\Texture\\character\\;.\\Texture\\Effect\\text\\;.\\Texture\\Effect\\qest_icon\\;"

// XML 암호화 패스워드
#define ENC_DATA_PASSWORD				"@#)!agebreak"

// Rendering flag
#define dFLAG_EXCEPT_GUI			0x00000001

typedef unsigned int BITFLAG;                   // 비트 플래그용 타입



#endif


