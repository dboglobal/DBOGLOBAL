#pragma once


#include "ntlworldcommon.h"

/*
WorldFileFormat - SectorInfo, FieldProperty
WorldData를 Memory에 Load하여 조작하기 위해 사용된다.
*/

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifndef dNTL_WORLD_FILE

class CNtlWorldFieldManager;

class CNtlWorldFileMemAccessor
{
public:
	CNtlWorldFileMemAccessor(void);
	~CNtlWorldFileMemAccessor(void);

private:
	RwBool	m_TmpBool;
	RwBool	m_TmpRwInt32;
	RwReal	m_TmpRwReal;
	BYTE	m_TmpBYTE;

public:
	// .wfif####; world field information file skip
	BYTE* Skip_Sector(BYTE* _pFileMem);
	BYTE* Skip_SectorIndoor(BYTE* _pFileMem);

	BYTE* Skip_Material(BYTE* _pFileMem);
	BYTE* Skip_Heightfield(BYTE* _pFileMem);
	BYTE* Skip_Diffuse(BYTE* _pFileMem);
	BYTE* Skip_Doodads(BYTE* _pFileMem);
	BYTE* Skip_Water(BYTE* _pFileMem);
	BYTE* Skip_Shadow(BYTE* _pFileMem);
	BYTE* Skip_Effects(BYTE* _pFileMem);
	BYTE* Skip_SE(BYTE* _pFileMem);
	// CZ-SS
	//BYTE* Skip_SS(BYTE* _pFileMem);
	BYTE* Skip_SectorCull(BYTE* _pFileMem);
	BYTE* Skip_TileTransparency(BYTE* _pFileMem);
	BYTE* Skip_Shoreline(BYTE* _pFileMem);
	BYTE* Skip_Decal(BYTE* _pFileMem);
	BYTE* Skip_Plant(BYTE* _pFileMem);
	BYTE* Skip_WorldLight(BYTE* _pFileMem);
	BYTE* Skip_Occluder(BYTE* _pFileMem);
	BYTE* Skip_HeatHazeObject(BYTE* _pFileMem);
	BYTE* Skip_LightObject(BYTE* _pFileMem);
	BYTE* Skip_Dojo(BYTE* _pFileMem);

	// 인도어
	BYTE* Skip_DoodadsIndoor(BYTE* _pFileMem);
	BYTE* Skip_LightMap(BYTE* _pFileMem);
	BYTE* Skip_DecalVolume(BYTE* _pFileMem);
	BYTE* Skip_GameProperty(BYTE* _pFileMem);
	
	// .wfpf####; world field prop file skip
	BYTE* Skip_Name(BYTE* _pFileMem);
	BYTE* Skip_BitProp(BYTE* _pFileMem);
	BYTE* Skip_Fog(BYTE* _pFileMem);
	BYTE* Skip_Sky(BYTE* _pFileMem);
	BYTE* Skip_RealLight(BYTE* _pFileMem);
	BYTE* Skip_BGM(BYTE* _pFileMem);
	BYTE* Skip_Bloom(BYTE* _pFileMem);
	BYTE* Skip_Planet(BYTE* _pFileMem);
	BYTE* Skip_Specular(BYTE* _pFileMem);
	BYTE* Skip_WorldMaterial(BYTE* _pFileMem);
	BYTE* Skip_FieldColor(BYTE* _pFileMem);
	BYTE* Skip_Weather(BYTE* _pFileMem);
	BYTE* Skip_FieldHeatHaze(BYTE* _pFileMem);

	// 인도어
	BYTE* Skip_BlockGameProperty(BYTE* _pFileMem);

	// interfaces for sector method
	BYTE* OnSM_SplatTexMaxBlend(BYTE* _pFileMem, FILE* _pFile, RwInt32 _StartIdx2Del);
	BYTE* OnSM_SplatTexMinBlend(BYTE* _pFileMem, FILE* _pFile, RwBool _Indices2Del[dNTL_SPLAT_LAYER_NUM]);

	// interfaces for PVS

	// .wfif####; world field information file save
	BYTE*	Save_Sector(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_SectorIndoor(BYTE* _pFileMem, FILE* _pFile);

	BYTE*	Save_Material(BYTE* _pFileMem, FILE* _pFile);
	VOID	Save_Heightfield(BYTE* _pFileMemSrc, BYTE* _pFileMemDst, FILE* _pFile);
	BYTE*	Save_Heightfield(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_Diffuse(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_Doodads(BYTE* _pFileMem, FILE* _pFile);
	VOID	Save_Doodads_NonTrigger(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	VOID	Save_Doodads_Trigger(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	VOID	Save_Doodads_Num(BYTE* _pPosSrcMem, BYTE* _pPosDstMem, FILE* _pFile);
	VOID	Save_Doodads_PEC(BYTE* _pFileMem, FILE* _pFile);
	VOID	Save_Doodads_Num_PEC(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_Doodads_VisibilityDist(BYTE* _pFileMem, FILE* _pFile, RwReal VisibilityDist, RwReal Min, RwReal Max);
	BYTE*	Save_Doodads_ProofsInvalidSectorLink(BYTE* _pFileMem, FILE* _pFile, RwInt32 iFieldIdx, RwInt32 iSectorCnt);
	BYTE*	Save_Water(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_Shadow(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_Effects(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	BYTE*	Save_SE(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	// CZ-SS
	//BYTE*	Save_SS(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	BYTE*	Save_SectorCull(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_TileTransparency(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_Shoreline(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	BYTE*	Save_Decal(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	
	BYTE*	Save_Plant(BYTE* _pFileMem, FILE* _pFile);
	BYTE*	Save_WorldLight(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	BYTE*	Save_Occluder(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	BYTE*	Save_HeatHazeObject(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	BYTE*	Save_LightObject(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	BYTE*	Save_Dojo(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	
	// 인도어
	BYTE*	Save_DoodadsIndoor(BYTE* _pFileMem, FILE* _pFile);	
	BYTE*	Save_DecalVolume(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	BYTE*	Save_GameProperty(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField = -1, RwInt32 IdxDstField = -1, CNtlWorldFieldManager* pFieldMgr = NULL);
	
	// .wfpf####; world field prop file save
	BYTE* Save_Name(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_BitProp(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_Fog(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_Sky(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_RealLight(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_BGM(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_Bloom(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_Planet(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_Specular(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_WorldMaterial(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_FieldColor(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_Weather(BYTE* _pFileMem, FILE* _pFile);
	BYTE* Save_FieldHeatHaze(BYTE* _pFileMem, FILE* _pFile);

	// 인도어
	BYTE* Save_BlockGameProperty(BYTE* _pFileMem, FILE* _pFile);
	
	// only outdoor use
	BYTE* GetSPosOfMaterial(BYTE* _pFileMem);
	BYTE* GetSPosOfHeightfield(BYTE* _pFileMem);
	BYTE* GetSPosOfDoodad(BYTE* _pFileMem);
	BYTE* GetSPosOfWater(BYTE* _pFileMem);
	BYTE* GetSPosOfShadow(BYTE* _pFileMem);
	BYTE* GetSPosOfEffect(BYTE* _pFileMem);
	BYTE* GetSPosOfSE(BYTE* _pFileMem);
};

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif