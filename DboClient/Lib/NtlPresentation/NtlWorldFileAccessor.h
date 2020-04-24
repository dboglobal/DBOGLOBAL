#pragma once

/*
WorldFileFormat - SectorInfo
SectorInfo File을 조작 하기 위해 사용된다.
*/
#include "ntlworldcommon.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifndef dNTL_WORLD_FILE

class CNtlWorldFileAccessor
{
public:
	CNtlWorldFileAccessor(void);
	~CNtlWorldFileAccessor(void);

public:
	RwInt32 Skip_Sector(FILE* pFile);
	RwInt32 Skip_SectorIndoor(FILE* pFile);

	RwInt32 Skip_Material(FILE* pFile);
	RwInt32 Skip_Heightfield(FILE* pFile);
	RwInt32 Skip_Diffuse(FILE* pFile);
	RwInt32 Skip_Doodads(FILE* pFile);
	RwInt32 Skip_DoodadsIndoor(FILE* pFile);
	RwInt32 Skip_Water(FILE* pFile);
	RwInt32 Skip_Shadow(FILE* pFile);
	RwInt32 Skip_Effects(FILE* pFile);
	RwInt32 Skip_SE(FILE* pFile);
	// CZ-SS
	//RwInt32 Skip_SS(FILE* pFile);
	RwInt32 Skip_SectorCull(FILE* pFile);
	RwInt32 Skip_TileTransparency(FILE* pFile);
	RwInt32 Skip_Shoreline(FILE* pFile);
	RwInt32 Skip_Decal(FILE* pFile);
	RwInt32 Skip_Plant(FILE* pFile);
	RwInt32 Skip_WorldLight(FILE* pFile);
	RwInt32 Skip_Occluder(FILE* pFile);
	RwInt32 Skip_HeatHazeObject(FILE* pFile);
	RwInt32 Skip_LightObject(FILE* pFile);
	RwInt32 Skip_DecalVolume(FILE* pFile);
	RwInt32 Skip_GameProperty(FILE* pFile);
	RwInt32 Skip_Dojo(FILE* pFile);
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif