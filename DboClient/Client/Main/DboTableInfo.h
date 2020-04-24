/****************************************************************************
 *
 * File			: DboTableInfo.h
 * Author		: Hong SungBock
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 3
 * Abstract		: DBO Table Info for Client
 *****************************************************************************
 * Desc         : 클라이언트에서만 사용되는 테이블의 정보를 정의한다
 *
 *****************************************************************************/

#pragma once

#include "NtlBitFlag.h"


///< 월드맵의 종류에 따라 화면에 표시됨
enum eLandMark_WorldMapType
{
	LT_NONE,
	LT_SECTION,
	LT_ZONE,
	LT_ZONESEC,
	LT_WORLD,
	LT_WORLDSEC,
	LT_WORLDZONE,
	LT_WORLDZONESEC,

	NUM_LT
};

enum eLandMark_WorldMapType_BitFlag
{
	LB_NONE				= MAKE_BIT_FLAG(LT_NONE),
	LB_SECTION			= MAKE_BIT_FLAG(LT_SECTION),
	LB_ZONE				= MAKE_BIT_FLAG(LT_ZONE),
	LB_ZONESEC			= MAKE_BIT_FLAG(LT_ZONESEC),
	LB_WORLD			= MAKE_BIT_FLAG(LT_WORLD),
	LB_WORLDSEC			= MAKE_BIT_FLAG(LT_WORLDSEC),
	LB_WORLDZONE		= MAKE_BIT_FLAG(LT_WORLDZONE),
	LB_WORLDZONESEC		= MAKE_BIT_FLAG(LT_WORLDZONESEC),

	NUM_LB
};

///< 월드맵에 표시되는 방법
enum eLandMark_Display_Part
{
	LDT_NONE,
	LDT_NAME,
	LDT_ICON,
	LDT_NAMEICON,

	NUM_LDT
};

enum eLandMark_Display_Part_BitFlag
{
	LDBF_NONE			= MAKE_BIT_FLAG(LDT_NONE),
	LDBF_NAME			= MAKE_BIT_FLAG(LDT_NAME),
	LDBF_ICON			= MAKE_BIT_FLAG(LDT_ICON),
	LDBF_NAMEICON		= MAKE_BIT_FLAG(LDT_NAMEICON),

	NUM_LDBF
};


///< Landmark classification to be displayed in a special way
enum eLandMark_SpecialType
{
	LANDMARK_SPECIAL_NORMAL,
	LANDMARK_SPECIAL_DUNGEON,

	NUM_LANDMARK_SPECIAL
};


enum eWorldMap_Type
{
	WORLDMAP_TYPE_WORLD,
	WORLDMAP_TYPE_ZONE,
	WORLDMAP_TYPE_CITY,
	WORLDMAP_TYPE_INSTANCE_MAP,
	WORLDMAP_TYPE_ZONE2,

	NUM_WORLDMAP_TYPE,

	INVALID_WORLDMAP_TYPE = 0xff
};