#ifndef _NTL_NAVI_DEFINE_H_
#define _NTL_NAVI_DEFINE_H_


#include <vector>
#include <map>

#include "./PathEngine/i_pathengine.h"

//////////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////////


// 프로그램에서 사용하는 하위 개념의 속성
enum eDBO_WORLD_ATTR_BASIC
{
	DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE = 0,
	DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE,
	DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP,
	DBO_WORLD_ATTR_BASIC_FORBID_PC_MOVE,
	DBO_WORLD_ATTR_BASIC_FORBID_NPC_MOVE,
	DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE,
	DBO_WORLD_ATTR_BASIC_ENABLE_SWIM,

	DBO_WORLD_ATTR_BASIC_MINI_INDOOR,

	DBO_WORLD_ATTR_BASIC_DAMAGE_SMALL,
	DBO_WORLD_ATTR_BASIC_DAMAGE_LARGE,
	DBO_WORLD_ATTR_BASIC_DRAGONBALL_SCRAMBLE_SAFE_ZONE,
	DBO_WORLD_ATTR_BASIC_FREE_PVP_ZONE,

	DBO_WORLD_ATTR_BASIC_COUNT,

	DBO_WORLD_ATTR_BASIC_FIRST = DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE,
	DBO_WORLD_ATTR_BASIC_LAST = DBO_WORLD_ATTR_BASIC_COUNT - 1,
};


// 지형 툴에서 사용하는 상위 개념의 속성(eDBO_WORLD_ATTR_BASIC의 그룹)
enum eDBO_WORLD_ATTR_GROUP
{
	DBO_WORLD_ATTR_GROUP_TOWN = 0,
	DBO_WORLD_ATTR_GROUP_SWIM,
	DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE,
	DBO_WORLD_ATTR_GROUP_FORBID_PC_SHOP,
	DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE,
	DBO_WORLD_ATTR_GROUP_FORBID_MOB_MOVE,

	DBO_WORLD_ATTR_GROUP_LAVA,
	DBO_WORLD_ATTR_GROUP_LAVA_SWIM,
	DBO_WORLD_ATTR_GROUP_DRAGONBALL_SCRAMBLE_SAFE_ZONE,
	DBO_WORLD_ATTR_GROUP_FREE_PVP_ZONE,

	DBO_WORLD_ATTR_GROUP_MINI_INDOOR_TILE = 13,
	DBO_WORLD_ATTR_GROUP_MINI_INDOOR_GATE,
	DBO_WORLD_ATTR_GROUP_WORLD_SHADOW,
};


#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_PC_BATTLE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_VEHICLE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_PC_SHOP		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_PC_MOVE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_PC_MOVE)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_NPC_MOVE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_NPC_MOVE)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_MOB_MOVE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE)
#define DBO_WORLD_ATTR_BASIC_FLAG_ENABLE_SWIM			(0x00000001 << DBO_WORLD_ATTR_BASIC_ENABLE_SWIM)


#define DBO_WORLD_ATTR_GROUP_FLAG_TOWN					(0x00000001 << DBO_WORLD_ATTR_GROUP_TOWN)
#define DBO_WORLD_ATTR_GROUP_FLAG_SWIM					(0x00000001 << DBO_WORLD_ATTR_GROUP_SWIM)
#define DBO_WORLD_ATTR_GROUP_FLAG_FORBID_PC_BATTLE		(0x00000001 << DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE)
#define DBO_WORLD_ATTR_GROUP_FLAG_FORBID_PC_SHOP		(0x00000001 << DBO_WORLD_ATTR_GROUP_FORBID_PC_SHOP)
#define DBO_WORLD_ATTR_GROUP_FLAG_FORBID_ALL_MOVE		(0x00000001 << DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE)
#define DBO_WORLD_ATTR_GROUP_FLAG_FORBID_MOB_MOVE		(0x00000001 << DBO_WORLD_ATTR_GROUP_FORBID_MOB_MOVE)
#define DBO_WORLD_ATTR_GROUP_FLAG_WORLD_SHADOW			(0x00000001 << DBO_WORLD_ATTR_GROUP_WORLD_SHADOW)


//////////////////////////////////////////////////////////////////////////
// Result
//////////////////////////////////////////////////////////////////////////


enum eCOL_TEST_RESULT
{
	eCOL_TEST_RESULT_FAILED,			// 충돌 검사 실패
	eCOL_TEST_RESULT_INVALID_SRC_POS,	// 시작 위치가 잘못 되었음
	eCOL_TEST_RESULT_INVALID_DEST_POS,	// 종료 위치가 잘못 되었음
	eCOL_TEST_RESULT_NOT_SAME_GROUP,	// 시작점과 목표점의 소속 그룹이 다름
	eCOL_TEST_RESULT_COL,				// 충돌 되었음
	eCOL_TEST_RESULT_NO_COL				// 충돌 안되었음
};


//////////////////////////////////////////////////////////////////////////
// Path engine path and token
//////////////////////////////////////////////////////////////////////////


#define PE_MODEL_DATA_FOLDER				(".\\tool\\ToolShareData\\NaviModel\\")
#define PE_TEXTURE_DATA_FOLDER				(".\\tool\\ToolShareData\\NaviTexture\\")

#define PE_PATHENGINE_FOLDER				("pathengine")
#define PE_PROPERTY_FOLDER					("property")

#define PE_FIELD_GROUP_MESH_EXT				("mf")
#define PE_FIELD_GROUP_COL_PRE_EXT			("cf")
#define PE_FIELD_GROUP_PATH_FIND_PRE_EXT	("pf")

#define PE_DATA_NAVI_MESH_KEY				("_PE_Nav_")
#define PE_DATA_OBS_MESH_KEY				("_PE_Obs_")
#define PE_DATA_MESH_EXT					(".tok")


//////////////////////////////////////////////////////////////////////////
// Path engine <=> world coordination
//////////////////////////////////////////////////////////////////////////


#define PATH_RATIO							(100.f)
#define PATH_COORD_TO_WORLD_COORD( v )		((v) / PATH_RATIO)
#define WORLD_COORD_TO_PATH_COORD( v )		((v) * PATH_RATIO)


//////////////////////////////////////////////////////////////////////////
// Path engine numeric define
//////////////////////////////////////////////////////////////////////////


#define MAX_PATH_HEIGHT						(1500000)

#define PATH_HORIZ_RANGE					(2000)
#define PATH_VERT_RANGE						(5000)
#define PATH_CLOSEST_RANGE					(2000)


//////////////////////////////////////////////////////////////////////////
// Path engine handler
//////////////////////////////////////////////////////////////////////////


struct sNAVI_INST_HANDLER
{
	unsigned int							uiWorldID;
	void*									pInnerData;
};

typedef void* NAVI_INST_HANDLE;


//////////////////////////////////////////////////////////////////////////
// Math numeric define
//////////////////////////////////////////////////////////////////////////


#define NAVI_ZERO_THRESHOLD					(1e-06f)

#define NAVI_PI								((float)(3.14159265358979))

#define NAVI_FLT_MAX						(3.402823466e+38F)
#define NAVI_FLT_MIN						(1.175494351e-38F)


//////////////////////////////////////////////////////////////////////////
// Agent
//////////////////////////////////////////////////////////////////////////


#define NAVI_AGENT_COORD_COUNT				(8)


//////////////////////////////////////////////////////////////////////////
// Loading information
//////////////////////////////////////////////////////////////////////////


enum ePATH_DATA_LOAD_FLAG
{
	ePATH_DATA_LOAD_FLAG_NO_LOADING			= (0x00),
	ePATH_DATA_LOAD_FLAG_LOAD_ONLY_PROPERTY	= (0x01),
	ePATH_DATA_LOAD_FLAG_LOAD_ONLY_WORLD	= (0x02),
	ePATH_DATA_LOAD_FLAG_LOAD_ALL			= ePATH_DATA_LOAD_FLAG_LOAD_ONLY_PROPERTY | ePATH_DATA_LOAD_FLAG_LOAD_ONLY_WORLD
};

typedef std::vector< unsigned int > vecdef_WorldIDList;

// Tool 과 NtlNavi 에서의 통신을 위한 자료구조
typedef std::vector< unsigned int > vecdef_GroupIDList;
typedef std::map< unsigned int, vecdef_GroupIDList > mapdef_ExportList;


//////////////////////////////////////////////////////////////////////////
// Simple math ( Interface parameter )
//////////////////////////////////////////////////////////////////////////


struct sNAVI_POS
{
	float x, y, z;

	sNAVI_POS( void )
	{
	}

	sNAVI_POS( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

typedef std::vector< sNAVI_POS > vecdef_NaviPosList;


struct sNAVI_FAST_POS
{
	cPosition pos;
	iMesh *pGround;
	unsigned int uiGroupId;
};

//////////////////////////////////////////////////////////////////////////
// Data for Rendering
//////////////////////////////////////////////////////////////////////////

struct sNAVI_VERTEX
{
	float x, y, z;
	unsigned int diffuse;

	sNAVI_VERTEX( void )
	{
		diffuse = 0xFFFFFFFF;
	}

	sNAVI_VERTEX( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;

		diffuse = 0xFFFFFFFF;
	}
};

#define NAVI_RENDER_FVF		(D3DFVF_XYZ | D3DFVF_DIFFUSE )

struct sNAVI_PE_VERTEX
{
	long x, y;
	float z;

	sNAVI_PE_VERTEX( void )
	{
	}

	sNAVI_PE_VERTEX( long x, long y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

#endif
