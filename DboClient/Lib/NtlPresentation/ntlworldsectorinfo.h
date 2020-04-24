/******************************************************************************************************************************
 *
 * ntlworldsectorinfo_h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : 
 *
 ******************************************************************************************************************************/

#ifndef __NTLWORLDSECTORINFO_H__
#define __NTLWORLDSECTORINFO_H__

#include "ntlworldcommon.h"
#include "NtlWorldSector.h"
#include "NtlWorldFileAccessor.h"
#include "NtlWorldFileMemAccessor.h"
#include "FileMem.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

class CNtlWorldFieldManager;

class CNtlInstanceEffect;
class CNtlPLObject;
class CNtlPLSound;
class CNtlPLSoundBGM;
class CNtlPLShoreLine;
class CNtlPLWorldDecal;
class CNtlPLWorldLight_Sphere;
class CNtlPLWorldLight_Box;
class CNtlPLOccluder_Quad;
class CNtlPLHeatHazeObject;
class CNtlPLLightObject;
class CNtlPLDecalVolume;
class CNtlPLGameProperty;
class CNtlPLDojo;

// SectorInfo
// : 화일 또는 화일 메모리에 접근하는 모든 코드는 이곳에 정의 한다.
// : 화일 접근 시에는 dGET_WORLD_PARAM()의 WorldLoadVer, WorldSaveVer을 구분하여 작성한다.
// : 화일 메모리 접근 시에는 dGET_WORLD_PARAM()의 WorldVer으로 작성한다. (단 예외의 경우가 발생 할 수 있다)
class CNtlWorldSectorInfo
{
public:
	CNtlWorldSectorInfo();
	virtual ~CNtlWorldSectorInfo();

public:
	static CNtlWorldSectorInfo*	CreateInstance();
	static void					DestroyInstance();

	static CNtlWorldSectorInfo*	GetInstance();

protected:
	static CNtlWorldSectorInfo* s_pInstance;
	static RwBool				s_bTrueFlag;
	static RwBool				s_bFalseFlag;

public:
#ifdef dNTL_WORLD_TOOL_MODE
	RwBool	NtlWorldSectorGeometryCreate(CNtlWorldSector *pNtlWorldSector);
#endif
	RwBool	NtlWorldSectorGeometryDelete(CNtlWorldSector *pNtlWorldSector);
	RwBool	NtlWorldSectorGeometryDeleteWithoutSwapFileSaving(CNtlWorldSector *pNtlWorldSector);

	RwBool	NtlWorldSectorGeometryLoadFromFile(CNtlWorldSector *pNtlWorldSector, FILE* pFile, sCUR_FIELD_TEX_INFO& CurFieldTexInfo);
	RwBool  NtlWorldSectorGeometrySaveIntoFile(CNtlWorldSector *pNtlWorldSector, FILE* pFile);

	// 인도어
	RwBool	NtlWorldSectorGeometryLoadFromFileIndoor(CNtlWorldSector *pNtlWorldSector, FILE* pFile);
	RwBool	NtlWorldSectorGeometrySaveIntoFileIndoor(CNtlWorldSector *pNtlWorldSector, FILE* pFile);

	// IdxMenu 0 : create, 1 : delete
	RwBool	NtlWorldSectorUpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr, RwInt32 IdxMenu);
	RwBool	NtlWorldSectorUpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr);

public:
	// Outdoor Sector
	RwBool						OutdoorSectorLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, sCUR_FIELD_TEX_INFO* pCurFieldTexInfo);
	RwBool						OutdoorSectorSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						OutdoorSectorSkipToFile(FILE* pFile);

	BYTE*						OutdoorSectorSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						OutdoorSectorSkipToFileMem(BYTE* pFileMem);

	// Indoor Sector
	RwBool						IndoorSectorLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						IndoorSectorSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						IndoorSectorSkipToFile(FILE* pFile);

	BYTE*						IndoorSectorSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						IndoorSectorSkipToFileMem(BYTE* pFileMem);

	// Sector Mesh : Material/HeightField/PreLights
	RwBool						SectorMeshLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, sCUR_FIELD_TEX_INFO* pCurFieldTexInfo);
	RwBool						SectorMeshSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorMeshSkipToFile(FILE* pFile);

	BYTE*						SectorMeshSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorMeshSkipToFileMem(BYTE* pFileMem);

	RwBool						SectorMaterialLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, RpMaterial* pMaterial, sCUR_FIELD_TEX_INFO* pCurFieldTexInfo);
	RwBool						SectorMaterialSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorMaterialSkipToFile(FILE* pFile);

	BYTE*						SectorMaterialSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorMaterialSaveIntoFileFromFileMemSplatTexMaxBlend(FILE* pFile, BYTE* pFileMem, RwInt32 iStartIdx2Del);
	BYTE*						SectorMaterialSaveIntoFileFromFileMemSplatTexMinBlend(FILE* pFile, BYTE* pFileMem, RwInt32 iIndices2Del[dNTL_SPLAT_LAYER_NUM]);
	BYTE*						SectorMaterialSkipToFileMem(BYTE* pFileMem);

	RwBool						SectorHeightfieldLoadFromFile(FILE* pFile, RwV3d* pVertexList);
	RwBool						SectorHeightfieldSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorHeightfieldSkipToFile(FILE* pFile);
	RwInt32						SectorHeightfieldSkipToFileGetVertex(FILE* pFile, RwInt32 iStartIdx, RwInt32 iCount, RwV3d* pResult);

	BYTE*						SectorHeightfieldSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorHeightfieldSkipToFileMem(BYTE* pFileMem);
	BYTE*						SectorHeightfieldSkipToFileMemGetVertex(BYTE* pFileMem, RwInt32 iStartIdx, RwInt32 iCount, RwV3d* pResult);

	void						SectorHeightfieldSaveIntoFileFromFileMemDiff(FILE* pFile, BYTE* pFileMemDst, BYTE* pFileMemSrc);

	RwBool						SectorPrelightsLoadFromFile(FILE* pFile, RwRGBA* pPrelights);
	RwBool						SectorPrelightsSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorPrelightsSkipToFile(FILE* pFile);

	BYTE*						SectorPrelightsSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorPrelightsSkipToFileMem(BYTE* pFileMem);

	// Sector Object
	RwBool						SectorObjectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, EActiveWorldType eActiveWorldType);
	RwBool						SectorObjectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, EActiveWorldType eActiveWorldType);
	RwInt32						SectorObjectSkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType);

	BYTE*						SectorObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorObjectSaveIntoFileFromFileMemRevisionPos(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwBBox* pBBox);
	BYTE*						SectorObjectSaveIntoFileFromFileMemPEC(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType);
	BYTE*						SectorObjectSaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD);
	BYTE*						SectorObjectSkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType);

	CNtlPLObject*				ObjectLoadFromFile(FILE* pFile, EActiveWorldType eActiveWorldType);
	RwBool						ObjectSaveIntoFile(FILE* pFile, CNtlPLObject* pNtlPLObject, EActiveWorldType eActiveWorldType);	
	RwInt32						ObjectSkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType);

	BYTE*						ObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwV3d* pvOffset = NULL);
	BYTE*						ObjectSaveIntoFileFromFileMemRevisionPos(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwBBox* pBBox);
	BYTE*						ObjectSaveIntoFileFromFileMemPEC(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType);
	BYTE*						ObjectSaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD);
	BYTE*						ObjectSkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType);
	BYTE*						ObjectSkipToFileMemGetEnabledTrigger(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwBool* pResult);
	BYTE*						ObjectSkipToFileMemGetEnabledPEC(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwBool* pResult);

	void						SectorObjectSaveIntoFileFromFileMemDiff(FILE* pFile, BYTE* pFileMemDst, BYTE* pFileMemSrc, EActiveWorldType eActiveWorldType, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);

	// Sector Water
	RwBool						SectorWaterLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorWaterSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorWaterSkipToFile(FILE* pFile);

	BYTE*						SectorWaterSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorWaterSkipToFileMem(BYTE* pFileMem);

	// Sector Shadow
	RwBool						SectorShadowLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorShadowSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorShadowSkipToFile(FILE* pFile);

	BYTE*						SectorShadowSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorShadowSkipToFileMem(BYTE* pFileMem);

	// Sector Effect
	RwBool						SectorEffectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorEffectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorEffectSkipToFile(FILE* pFile);

	BYTE*						SectorEffectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorEffectSkipToFileMem(BYTE* pFileMem);

	CNtlInstanceEffect*			EffectLoadFromFile(FILE* pFile);
	RwBool						EffectSaveIntoFile(FILE* pFile, CNtlInstanceEffect* pNtlInstanceEffect);	
	RwInt32						EffectSkipToFile(FILE* pFile);

	BYTE*						EffectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						EffectSkipToFileMem(BYTE* pFileMem);

	// Sector SoundEffect
	RwBool						SectorSoundEffectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorSoundEffectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorSoundEffectSkipToFile(FILE* pFile);

	BYTE*						SectorSoundEffectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorSoundEffectSkipToFileMem(BYTE* pFileMem);

	CNtlPLSound*				SoundEffectLoadFromFile(FILE* pFile);
	RwBool						SoundEffectSaveIntoFile(FILE* pFile, CNtlPLSound* pNtlPLSound);	
	RwInt32						SoundEffectSkipToFile(FILE* pFile);

	BYTE*						SoundEffectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						SoundEffectSkipToFileMem(BYTE* pFileMem);
	
	CNtlPLSoundBGM*				SoundEffectBGMLoadFromFile(FILE* pFile);
	RwBool						SoundEffectBGMSaveIntoFile(FILE* pFile, CNtlPLSoundBGM* pNtlPLSoundBGM);	
	RwInt32						SoundEffectBGMSkipToFile(FILE* pFile);

	BYTE*						SoundEffectBGMSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						SoundEffectBGMSkipToFileMem(BYTE* pFileMem);

	// Sector Cull
	RwBool						SectorCullLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorCullSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorCullSkipToFile(FILE* pFile);

	BYTE*						SectorCullSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorCullSkipToFileMem(BYTE* pFileMem);

	// Tile Transparency
	RwBool						SectorTileTransparencyLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorTileTransparencySaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorTileTransparencySkipToFile(FILE* pFile);

	BYTE*						SectorTileTransparencySaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorTileTransparencySkipToFileMem(BYTE* pFileMem);

	// Sector ShoreLine
	RwBool						SectorShoreLineLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorShoreLineSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorShoreLineSkipToFile(FILE* pFile);

	BYTE*						SectorShoreLineSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorShoreLineSkipToFileMem(BYTE* pFileMem);

	CNtlPLShoreLine*			ShoreLineLoadFromFile(FILE* pFile, RwV3d* pvPos);
	RwBool						ShoreLineSaveIntoFile(FILE* pFile, CNtlPLShoreLine* pNtlPLShoreLine);	
	RwInt32						ShoreLineSkipToFile(FILE* pFile);

	BYTE*						ShoreLineSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						ShoreLineSkipToFileMem(BYTE* pFileMem);

	// Sector Decal
	RwBool						SectorDecalLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorDecalSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorDecalSkipToFile(FILE* pFile);

	BYTE*						SectorDecalSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorDecalSkipToFileMem(BYTE* pFileMem);

	CNtlPLWorldDecal*			DecalLoadFromFile(FILE* pFile);
	RwBool						DecalSaveIntoFile(FILE* pFile, CNtlPLWorldDecal* pNtlPLWorldDecal);	
	RwInt32						DecalSkipToFile(FILE* pFile);

	BYTE*						DecalSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						DecalSkipToFileMem(BYTE* pFileMem);

	// Sector Plant
	RwBool						SectorPlantLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorPlantSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorPlantSkipToFile(FILE* pFile);

	BYTE*						SectorPlantSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*						SectorPlantSkipToFileMem(BYTE* pFileMem);

	// Sector WorldLight
	RwBool						SectorWorldLightLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorWorldLightSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorWorldLightSkipToFile(FILE* pFile);

	BYTE*						SectorWorldLightSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorWorldLightSkipToFileMem(BYTE* pFileMem);

	CNtlPLWorldLight_Sphere*	WorldLightSphereLoadFromFile(FILE* pFile);
	RwBool						WorldLightSphereSaveIntoFile(FILE* pFile, CNtlPLWorldLight_Sphere* pNtlPLWorldLightSphere);	
	RwInt32						WorldLightSphereSkipToFile(FILE* pFile);

	BYTE*						WorldLightSphereSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						WorldLightSphereSkipToFileMem(BYTE* pFileMem);

	CNtlPLWorldLight_Box*		WorldLightBoxLoadFromFile(FILE* pFile);
	RwBool						WorldLightBoxSaveIntoFile(FILE* pFile, CNtlPLWorldLight_Box* pNtlPLWorldLightBox);	
	RwInt32						WorldLightBoxSkipToFile(FILE* pFile);

	BYTE*						WorldLightBoxSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						WorldLightBoxSkipToFileMem(BYTE* pFileMem);

	// Sector Occluder
	RwBool						SectorOccluderLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorOccluderSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorOccluderSkipToFile(FILE* pFile);

	BYTE*						SectorOccluderSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorOccluderSkipToFileMem(BYTE* pFileMem);

	CNtlPLOccluder_Quad*		OccluderLoadFromFile(FILE* pFile);
	RwBool						OccluderSaveIntoFile(FILE* pFile, CNtlPLOccluder_Quad* pNtlPLOccluder);	
	RwInt32						OccluderSkipToFile(FILE* pFile);

	BYTE*						OccluderSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						OccluderSkipToFileMem(BYTE* pFileMem);

	// Sector HeatHazeObject
	RwBool						SectorHeatHazeObjectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorHeatHazeObjectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorHeatHazeObjectSkipToFile(FILE* pFile);

	BYTE*						SectorHeatHazeObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorHeatHazeObjectSkipToFileMem(BYTE* pFileMem);

	CNtlPLHeatHazeObject*		HeatHazeObjectLoadFromFile(FILE* pFile);
	RwBool						HeatHazeObjectSaveIntoFile(FILE* pFile, CNtlPLHeatHazeObject* pNtlPLHeatHazeObject);	
	RwInt32						HeatHazeObjectSkipToFile(FILE* pFile);

	BYTE*						HeatHazeObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						HeatHazeObjectSkipToFileMem(BYTE* pFileMem);

	// Sector LightObject
	RwBool						SectorLightObjectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorLightObjectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorLightObjectSkipToFile(FILE* pFile);

	BYTE*						SectorLightObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorLightObjectSkipToFileMem(BYTE* pFileMem);

	CNtlPLLightObject*			LightObjectLoadFromFile(FILE* pFile);
	RwBool						LightObjectSaveIntoFile(FILE* pFile, CNtlPLLightObject* pNtlPLLightObject);	
	RwInt32						LightObjectSkipToFile(FILE* pFile);

	BYTE*						LightObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						LightObjectSkipToFileMem(BYTE* pFileMem);

	// Sector DecalVolume
	RwBool						SectorDecalVolumeLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorDecalVolumeSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorDecalVolumeSkipToFile(FILE* pFile);

	BYTE*						SectorDecalVolumeSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorDecalVolumeSkipToFileMem(BYTE* pFileMem);

	CNtlPLDecalVolume*			DecalVolumeLoadFromFile(FILE* pFile);
	RwBool						DecalVolumeSaveIntoFile(FILE* pFile, CNtlPLDecalVolume* pNtlPLDecalVolume);	
	RwInt32						DecalVolumeSkipToFile(FILE* pFile);

	BYTE*						DecalVolumeSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						DecalVolumeSkipToFileMem(BYTE* pFileMem);

	// Sector GameProperty
	RwBool						SectorGamePropertyLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorGamePropertySaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorGamePropertySkipToFile(FILE* pFile);

	BYTE*						SectorGamePropertySaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorGamePropertySkipToFileMem(BYTE* pFileMem);

	CNtlPLGameProperty*			GamePropertyLoadFromFile(FILE* pFile);
	RwBool						GamePropertySaveIntoFile(FILE* pFile, CNtlPLGameProperty* pNtlPLGameProperty);	
	RwInt32						GamePropertySkipToFile(FILE* pFile);

	BYTE*						GamePropertySaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						GamePropertySkipToFileMem(BYTE* pFileMem);

	// Sector Dojo
	RwBool						SectorDojoLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwBool						SectorDojoSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector);
	RwInt32						SectorDojoSkipToFile(FILE* pFile);

	BYTE*						SectorDojoSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField = -1, RwInt32 iIdxDstField = -1, CNtlWorldFieldManager* pWorldFieldMgr = NULL);
	BYTE*						SectorDojoSkipToFileMem(BYTE* pFileMem);

	CNtlPLDojo*					DojoLoadFromFile(FILE* pFile);
	RwBool						DojoSaveIntoFile(FILE* pFile, CNtlPLDojo* pNtlPLDojo);	
	RwInt32						DojoSkipToFile(FILE* pFile);

	BYTE*						DojoSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	BYTE*						DojoSkipToFileMem(BYTE* pFileMem);

protected:
	void						NtlWorldSectorMaterialExtensionCreate(sNtlWorldSector *pNtlSector, RpMaterial *pMaterial);
	//void						NtlWorldSectorMaterialExtensionRead(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RpMaterial *pMaterial, FILE* pFile);

	RwReal						NtlWorldHFLoadFromFile(FILE* pFile, RwReal PosX, RwReal PosY);
};

static CNtlWorldSectorInfo* GetNtlWorldSectorInfo()
{
	return CNtlWorldSectorInfo::GetInstance();
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

// File Memory Accessor for Ntl World Files
extern CNtlWorldFileAccessor	g_FA;
extern CNtlWorldFileMemAccessor	g_FMA;

extern RwBool _RpNtlWorldSectorGeometryCreate(CNtlWorldSector *pNtlWorldSector);
extern RwBool _RpNtlWorldSectorGeometryDelete(CNtlWorldSector *pNtlWorldSector);
extern RwBool _RpNtlWorldSectorGeometryDeleteWithoutSwapFileSaving(CNtlWorldSector *pNtlWorldSector);

extern RwBool _RpNtlWorldSectorGeometryLoadFromFile(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, FILE* pFile);
extern RwBool _RpNtlWorldSectorGeometrySaveIntoFile(RpWorldSector *pWorldSector, FILE* pFile);

// 인도어
extern RwBool _RpNtlWorldSectorGeometryLoadFromFileIndoor(CNtlWorldSector *pNtlWorldSector, FILE* pFile);
extern RwBool _RpNtlWorldSectorGeometrySaveIntoFileIndoor(CNtlWorldSector *pNtlWorldSector, FILE* pFile);

// file position related
extern RwBool   _RpNtlWorldGetFilePosDoodads(FILE* pFile);
extern RwBool	_RpNtlWorldSectorReadMaterialDummy(FILE* pFile);
extern RwBool	_RpNtlWorldSectorReadDummy(FILE* pFile);

// IdxMenu 0 : create, 1 : delete
extern RwBool _RpNtlWorldSectorUpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr, RwInt32 IdxMenu);
extern RwBool _RpNtlWorldSectorUpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr);

extern RwBool	_RpNtlWorldSectorRender(RpWorldSector *pWorldSector);
extern RwBool	_RpNtlInitializeSector(sNtlWorldSector *pNtlWorldSector);
void			_RpNtlWorldSectorMaterialExtensionCreate(sNtlWorldSector *pNtlWorldSector, RpMaterial *pMaterial);
void			_RpNtlWorldSectorMaterialExtensionRead(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RpMaterial *pMaterial, FILE* pFile);

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif

#endif __NTLWORLDSECTORINFO_H__