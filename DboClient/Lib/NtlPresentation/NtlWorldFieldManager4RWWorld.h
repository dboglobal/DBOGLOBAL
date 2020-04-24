#pragma once

#include <rppvs.h>

#include "ntlworldcommon.h"
#include "NtlWorldFieldManager.h"

class CNtlWorldFieldManager4RWWorld;
class CNtlWorldBlockMananger;

class CNtlWorldFieldManager4RWWorld : public CNtlWorldFieldManager
{
public:
	friend class CNtlWorldBlockManager;

public:
	CNtlWorldFieldManager4RWWorld(RpWorld *pNtlWorld, RwV3d& SpawnPos, EActiveWorldType _ActiveWorldtype);
	virtual ~CNtlWorldFieldManager4RWWorld();

	virtual void			Init(RpWorld *pNtlWorld, RwV3d& SpawnPos);
	virtual void			Free();

public:
	virtual RwBool			SaveCurFieldsBeforeExit();
	virtual RwBool			SaveCurFields();

	//virtual RwBool			GetWorldReady();
	virtual void			GetSectopMap(RpWorldSector* pRpWorldSector, std::vector<CNtlWorldSector*>& vecNtlWorldSector);

	virtual RwReal			GetWorldSectorHeight(RwV3d& Pos);

	virtual DWORD			GetMapTileProp(RwV3d& Pos);
	virtual BYTE			GetMaterialTileProp(RwV3d& Pos);
	virtual RwBool			GetWorldShadowColor(RwV3d& Pos, RwRGBA* pOutColor);

	virtual RwBool			IsFieldLoaded(RwInt32 FieldIdx);

	virtual RwBool			GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp);
	virtual RwBool			SetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp, eNTL_FIELD_PROPID NtlFieldPropID);
	virtual RwBool			GetABlockProp(RwV3d& Pos, sNTL_BLOCK_PROP& NtlBlockProp);
	virtual RwBool			SetABlockProp(RwV3d& Pos, sNTL_BLOCK_PROP& NtlBlockProp, eNTL_BLOCK_PROPID NtlBlockPropID);

	virtual void			SetAnotherField(RwBool ChangeStraightAway = FALSE);

	virtual void			CreateSectorMap(RpWorld *pNtlWorld);

	virtual RwBool			CreateFields(RwInt32 FieldIdx);
	virtual RwBool			CreateFieldInMemory(RwInt32 FieldIdx);
	virtual RwBool			CreateFieldFromFile(RwInt32 FieldIdx);

	virtual void			DeleteFiles4Field(RwInt32 FieldIdx);
	virtual RwBool			DeleteFields(RwInt32 FieldIdx);
	virtual RwBool			DeleteFieldInMemory(RwInt32 FieldIdx, RwBool SaveSwapInToolMode = TRUE);
	virtual RwBool 			DeleteFieldIntoFile(RwInt32 FieldIdx);
	virtual RwBool			DeleteFieldThenCreateNew(RwInt32 FieldIdx);
	
	virtual void			Update(RwV3d& AvatarPos, RwReal _ElapsedTime);
	virtual void			UpdateMsg(RwV3d& Pos);
	virtual RwBool			UpdateFieldMap(RwV3d& Pos);
	virtual void			UpdateVisibleSectors();

	virtual RwBool			Pick(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt, sNtlWorldCollisionInfo* pOutWorldCollsionInfo);
	virtual RwBool			PickTerrain(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt);

#ifdef dNTL_WORLD_TOOL_MODE
	virtual void			RenderBlockBBox();
	virtual void			RenderBlockGuide();
	virtual void			RenderSectorGuide();
	virtual void			RenderFieldGuide();
#endif

	RwBool					GetWorldDecal(RwV3d& vPosition, RwV3d& vSize, RwInt32 nPolyMaxCount, RwInt32& nRenderVertexCount, RwReal& fUpdateVertextRadius, RwIm3DVertex* pVertices, RwReal OffsetY);

	virtual RwBool			LoadProperty();
	virtual RwBool			SaveProperty();

protected:
	virtual void			LoadPVS();
	virtual void			RefreshCurSectorPVS(RwInt32 _SectorIdx);
	virtual void			RefreshOldSectorPVS(RwInt32 _SectorIdx);

	virtual void			CreateRpWorld();
	virtual void			DestroyRpWorld();

	void					SetWorldSectorIndoor(RpWorldSector *pWorldSector);

public:
	typedef std::vector<CNtlWorldSector*>							WORLD_SECTOR_VEC;
	typedef WORLD_SECTOR_VEC::iterator								WORLD_SECTOR_VEC_IT;

	struct SRpWorldSectorIndoor
	{
		WORLD_SECTOR_VEC	vecNtlWorldSector;
		RwSphere			sphereBounding;
	};

	typedef std::map<RpWorldSector*, SRpWorldSectorIndoor*>			INDOOR_SECTOR_MAP;
	typedef INDOOR_SECTOR_MAP::iterator								INDOOR_SECTOR_MAP_IT;

public:
	CNtlWorldBlockMananger*	GetWorldBlockManager() {return m_pWorldBlockMananger; }
	
	void					BeginRenderSector();
	void					AddRenderSector(RpWorldSector* pRpWorldSector);
	void					EndRenderSector();

	const WORLD_SECTOR_VEC*	GetRenderSector() {	return &m_vecVisibleSectors; }

	RwBool					RpSectorOccluderQuery(RpWorldSector* pSector);

protected:
	CNtlWorldBlockMananger*	m_pWorldBlockMananger;

private:
	INDOOR_SECTOR_MAP		m_mapSectors;
	WORLD_SECTOR_VEC		m_vecVisibleSectors;

public:
#ifdef dNTL_WORLD_TOOL_MODE
	virtual void			OnWaterProc(RwInt32 IdxMenu, RwInt32 IdxSector, sSECTOR_WATER_ATTR& SectorWaterAttr);
#endif
};