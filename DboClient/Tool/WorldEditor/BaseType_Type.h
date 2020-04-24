//*****************************************************************************
// File       : BaseType_Type.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __BASE_TYPE_TYPE_H__
#define __BASE_TYPE_TYPE_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward Refenence Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard Template Library Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Const Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef enum EEntityType
{
	E_ENTITY_TYPE_NONE					= 0x0,
	E_ENTITY_TYPE_ATOMIC				= 0x1,
	E_ENTITY_TYPE_CLUMP					= 0x2,
	E_ENTITY_TYPE_LIGHT					= 0x3,
	E_ENTITY_TYPE_CAMERA				= 0x4,
	E_ENTITY_TYPE_EFFECT				= 0x5,
	E_ENTITY_TYPE_TEXTURE				= 0x6,
	E_ENTITY_TYPE_WORLD					= 0x7,
	E_ENTITY_TYPE_GEOMETRY				= 0x8,
	E_ENTITY_TYPE_MESH					= 0x9,
	E_ENTITY_TYPE_FOG					= 0xA,
	E_ENTITY_TYPE_SKY					= 0xB,
	E_ENTITY_TYPE_ENVIROMENT			= 0xC,
	E_ENTITY_TYPE_ANIMATION				= 0xD,
	E_ENTITY_TYPE_PLANE_SECTOR			= 0xE,
	E_ENTITY_TYPE_WORLD_SECTOR			= 0xF,
	E_ENTITY_TYPE_COUNT
} EEntityType;


typedef enum EEntityTypeFilter
{
	E_ENTITY_TYPE_FILTER_NONE			= 0x00000000,
	E_ENTITY_TYPE_FILTER_ATOMIC			= 0x00000002,
	E_ENTITY_TYPE_FILTER_CLUMP			= 0x00000004,
	E_ENTITY_TYPE_FILTER_LIGHT			= 0x00000008,
	E_ENTITY_TYPE_FILTER_CAMERA			= 0x00000010,
	E_ENTITY_TYPE_FILTER_EFFECT			= 0x00000020,
	E_ENTITY_TYPE_FILTER_TEXTURE		= 0x00000040,
	E_ENTITY_TYPE_FILTER_WORLD			= 0x00000080,
	E_ENTITY_TYPE_FILTER_GEOMETRY		= 0x00000100,
	E_ENTITY_TYPE_FILTER_MESH			= 0x00000200,
	E_ENTITY_TYPE_FILTER_FOG			= 0x00000400,
	E_ENTITY_TYPE_FILTER_SKY			= 0x00000800,
	E_ENTITY_TYPE_FILTER_ENVIROMENT		= 0x00001000,
	E_ENTITY_TYPE_FILTER_ANIMATION		= 0x00002000,
	E_ENTITY_TYPE_FILTER_PLANE_SECTOR	= 0x00004000,
	E_ENTITY_TYPE_FILTER_WORLD_SECTOR	= 0x00008000,
	E_ENTITY_TYPE_FILTER_ALL			= 0x0000FFFF
} EEntityTypeFilter;


typedef enum EEntityCategory
{
	E_ENTITY_CATEGORY_NORMAL			= 0,
	E_ENTITY_CATEGORY_INTERNAL			= 1,
	E_ENTITY_CATEGORY_HELPER			= 2,
	E_ENTITY_CATEGORY_EXTERNAL			= 3,
	E_ENTITY_CATEGORY_COUNT
} EEntityCategory;


typedef enum EEntityCategoryFilter
{
	E_ENTITY_CATEGORY_FILTER_NORMAL		= 0x01,
	E_ENTITY_CATEGORY_FILTER_INTERNAL	= 0x02,
	E_ENTITY_CATEGORY_FILTER_HELPER		= 0x04,
	E_ENTITY_CATEGORY_FILTER_EXTERNAL	= 0x08,
	E_ENTITY_CATEGORY_FILTER_ALL		= 0xFF
} EEntityCategoryFilter;


typedef enum EEntityContainerType
{
	E_ENTITY_CONTAINER_TYPE_NONE			= 0,
	E_ENTITY_CONTAINER_TYPE_LIST			= 1,
	E_ENTITY_CONTAINER_TYPE_CUBE			= 2,
	E_ENTITY_CONTAINER_TYPE_HASH_MAP		= 3,
	E_ENTITY_CONTAINER_TYPE_HASH_TABLE		= 4,
	E_ENTITY_CONTAINER_TYPE_PERPERTY_SORT	= 5,
	E_ENTITY_CONTAINER_TYPE_COUNT
} EEntityContainerType;


typedef enum EEntityState
{
	ENTITY_COMMAND_STATE_NONE			= 0,
	ENTITY_COMMAND_STATE_CREATED		= 1,
	ENTITY_COMMAND_STATE_VISIBLE		= 2,
	ENTITY_COMMAND_STATE_INVISIBLE		= 4,
	ENTITY_COMMAND_STATE_DESTROYED		= 8,
	ENTITY_COMMAND_STATE_COUNT
} EEntityState;


typedef enum EEntityContainerTypeFilter
{
	E_ENTITY_CONTAINER_TYPE_FILTER_NONE	= 0x00,
	E_ENTITY_CONTAINER_TYPE_FILTER_LIST	= 0x02,
	E_ENTITY_CONTAINER_TYPE_FILTER_CUBE	= 0x04,
	E_ENTITY_CONTAINER_TYPE_FILTER_ALL	= 0xFF
} EEntityContainerTypeFilter;


typedef enum EItemType
{
	E_ITEM_TYPE_NONE		= 0,
	E_ITEM_TYPE_PROPERTY	= 1,
	E_ITEM_TYPE_ENTITY		= 2,
	E_ITEM_TYPE_COUNT
} EItemType;


typedef enum EEntityCommandType
{
	ENTITY_COMMAND_TYPE_NONE,

	ENTITY_COMMAND_TYPE_CREATE,
	ENTITY_COMMAND_TYPE_DESTROY,

	ENTITY_COMMAND_TYPE_ATTACH,
	ENTITY_COMMAND_TYPE_DETACH,

	ENTITY_COMMAND_TYPE_MOVE,
	ENTITY_COMMAND_TYPE_ROTATE,
	ENTITY_COMMAND_TYPE_SCALE,

	ENTITY_COMMAND_TYPE_SELECT,
	ENTITY_COMMAND_TYPE_UNSELECT,

	ENTITY_COMMAND_TYPE_REPORT,
	ENTITY_COMMAND_TYPE_VERIFY,

	ENTITY_COMMAND_TYPE_ETCS,

	ENTITY_COMMAND_TYPE_COUNT
} EEntityCommandType;


typedef enum EEntityCommandCategory
{
	ENTITY_COMMAND_CATEGORY_NONE,

	ENTITY_COMMAND_CATEGORY_TEMP,
	ENTITY_COMMAND_CATEGORY_UNDO,
	ENTITY_COMMAND_CATEGORY_REDO,

	ENTITY_COMMAND_CATEGORY_COUNT
} EEntityCommandCategory;


#define ENTITY_RENDER_PRIORITY_NONE				0x00
#define ENTITY_RENDER_PRIORITY_FIRST			0x01
#define ENTITY_RENDER_PRIORITY_CAMERA_DEFAULT	0x18
#define ENTITY_RENDER_PRIORITY_LIGHT_DEFAULT	0x48
#define ENTITY_RENDER_PRIORITY_NORMAL_DEFAULT	0x88
#define ENTITY_RENDER_PRIORITY_EXTERNAL_DEFAULT	0xA8
#define ENTITY_RENDER_PRIORITY_LAST				0xFE
#define ENTITY_RENDER_PRIORITY_NOT_RENDERED		0xFF


typedef enum EEntityEventType
{
	E_ENTITY_EVENT_TYPE_NONE				= 0x00,
	E_ENTITY_EVENT_TYPE_LIGHT_COLOR			= 0x01,
	E_ENTITY_EVENT_TYPE_COUNT
} EEntityEventType;


typedef enum EEntityEventApplyType
{
	E_ENTITY_EVENT_APPLY_TYPE_NONE,
	E_ENTITY_EVENT_APPLY_TYPE_DIRECT,
	E_ENTITY_EVENT_APPLY_TYPE_OVER_TIME,
	E_ENTITY_EVENT_APPLY_TYPE_COUNT
} EEntityEventApplyType;


typedef enum EEntityAlphaLevel
{
	E_ENTITY_ALPHA_LEVEL_NONE			= 0,
	E_ENTITY_ALPHA_LEVEL_TREE			= 16,
} EEntityAlphaLevel;


typedef enum EUVAnimationLayer
{
	E_UV_ANIMATION_LAYER_BASE			= 0,
	E_UV_ANIMATION_LAYER_DUAL			= 1,
	E_UV_ANIMATION_LAYER_COUNT			= 2
} EUVAnimationLayer;


//-----------------------------------------------------------------------------
// Macro & Template Function Table
//-----------------------------------------------------------------------------

inline EEntityTypeFilter ConvertEntityTypeToFilter( EEntityType eType )
{
	if( eType > E_ENTITY_TYPE_NONE && eType < E_ENTITY_TYPE_COUNT )
	{
		return static_cast<EEntityTypeFilter>(0x01 << static_cast<RwInt8>(eType));
	}

    return E_ENTITY_TYPE_FILTER_NONE;
}


inline RwBool IsEntityTypeInFilter( EEntityType eType, EEntityTypeFilter eTypeFilter )
{
	if( ConvertEntityTypeToFilter( eType ) & eTypeFilter )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


inline EEntityCategoryFilter ConvertEntityCategoryToFilter( EEntityCategory eCategory )
{
	if( eCategory >= E_ENTITY_CATEGORY_NORMAL && eCategory < E_ENTITY_CATEGORY_COUNT )
	{
		return static_cast<EEntityCategoryFilter>(0x01 << static_cast<RwInt8>(eCategory));
	}

    return E_ENTITY_CATEGORY_FILTER_NORMAL;
}


inline RwBool IsEntityCategoryInFilter( EEntityCategory eCategory, EEntityCategoryFilter eCategoryFilter )
{
	if( ConvertEntityCategoryToFilter( eCategory ) & eCategoryFilter )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


#endif //__BASE_TYPE_TYPE_H__

//*****************************************************************************
//
// End of File : BaseType_Type.h
//
//*****************************************************************************