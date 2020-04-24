#pragma once


#include "ntlworldcommon.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldSectorManager.h"
#include "NtlWorldField.h"
#include "rpcollis.h"
#include "NtlWorldSector.h"
#include "NtlSharedType.h"
#include "FileMem.h"
#include "NtlPLSun.h"
#include "NtlSchedule.h"
#include "NtlWorldSchedule.h"
#include "NtlPLSceneManager.h"

#include <vector>
using std::vector;

#define dFIELD_EFFECT_SWITCHING_TIME	(5.0f)
#define dDEPTH_MAP_DIST_MAX				(30.0f)


struct sTARGET_VERT_INFO
{
	CNtlWorldSector		*pNtlWorldSector;
	RwInt32				IdxVert;
	RwReal				ValHeight;
	RwBool				ValHeight2Append;
};

struct sTARGET_VERT_SL_INFO
{
	CNtlWorldSector		*pNtlWorldSector;
	RwInt32				IdxVert;
};


class CNtlPLSky;
class CNtlPLFog;
class CNtlPLWater;
class CNtlPLLight;
class CNtlPLBloom;
class CNtlPLObject;
class CNtlPLPlant;
class CNtlPLWeather;
class C2DAABB;
class CNtlPLPlanetHandler;
class CNtlPLWeatherHandler;

class CNtlWorldFieldManager : public CNtlWorldSectorManager
{
protected:
	friend class CNtlWorldMergeManager;

protected:
	RwInt32				m_iCzTestCnt;

	RpWorld*			m_pRpWorld;

	CNtlWorldScheduler	m_WorldScheduler;

	CNtlPLSky*		m_pSkyEntity;
	CNtlPLSky*		m_pDragonSkyEntity;
	CNtlPLFog*		m_pFogEntity;
	CNtlPLWater*	m_pWaterEntity;
	CNtlPLLight*	m_pLightEntity;
	CNtlPLBloom*    m_pBloomEntity;
	CNtlPLPlant*	m_pPlantEntity;

	CNtlPLPlanetHandler*		m_pPlanetHandler;
	CNtlPLWeatherHandler*		m_pWeatherHandler;

	EActiveWorldType m_ActiveWorldType;

	// moved direction
	eNTL_WORLD_DIR m_eMoved2;

	// fields
	CNtlWorldField *m_pFields;

	// datum index
	RwInt32 m_OldDatumIdx, m_NewDatumIdx;

	// current picked field for texturing
	RwInt32 m_IdxCurField;

	// Map Name Index
	TBLIDX m_CurMapNameIdx;

	RwBool m_FieldPropVariationStarting;
	RwReal m_FieldPropVariationRestTime;

public:
	RwInt32 m_Fields6x6[2][36];

protected:
	RwInt32 m_OldSectorIdx4PVS;

#ifdef dNTL_WORLD_TOOL_MODE

public:
	vector<int> m_vecVisibleField;

	// Field count * Line point count * sector count per field
	sLINE3D	m_Buffer[(6 * 6) * ((64 * 2) * 2) * 4];

	RwInt32	m_VertCount;

#endif

protected:
	RwBool					CreateDragonSky();
	RwBool					CreateSky();
	RwBool					CreateFog();
	RwBool					CreateWater();
	RwBool					CreatePlant();	
			
	virtual	RwBool			UpdateFieldMap(RwV3d& Pos); // 인도어 : virtual 재정의
	void					UpdateDatumDir();
	void					UpdateNeighborFields(RwV3d& Pos);
	void					UpdateLODAttr(RwV3d& Pos);
	void					UpdateLODAttrLvl(RwV3d& Pos);
	void					UpdateDragonSky(RwV3d* pPosAvatar);
	void					UpdateSky(RwV3d* pPosAvatar);
	void					UpdateFog(RwV3d* pPosAvatar);
	void					UpdateShoreLine(RwReal _ElapsedTime);
	virtual void			UpdateMsg(RwV3d& Pos); // 인도어 : virtual 재정의
	void					UpdateSectors();
	virtual void			UpdateVisibleSectors();
	VOID					UpdateCurMapName(RwV3d& Pos);
	VOID					UpdateEntityHandler(RwReal _ElapsedTime);

	RwBool					IsFieldValid(RwInt32 Idx);
	RwBool					IsFieldValid(RwV3d& Pos);
	RwBool					IsThereNewRegion2Load();	
			
	virtual VOID			LoadPVS(); // 인도어 : virtual 재정의
	virtual VOID			RefreshCurSectorPVS(RwInt32 _SectorIdx); // 인도어 : virtual 재정의
	virtual VOID			RefreshOldSectorPVS(RwInt32 _SectorIdx); // 인도어 : virtual 재정의

public:
	CNtlWorldFieldManager(RpWorld *pNtlWorld, RwV3d& SpawnPos, EActiveWorldType _ActiveWorldtype);
	virtual ~CNtlWorldFieldManager();

	virtual void			Init(RpWorld *pNtlWorld, RwV3d& SpawnPos); // 인도어 : virtual 재정의
	virtual void			Free(); // 인도어 : virtual 재정의

	virtual void			InitSingleInstance();
	virtual void			FreeSingleInstance();


	virtual RpWorld*		GetWorld() { return m_pRpWorld; }
	
	// 인도어
	virtual void			CreateRpWorld();
	virtual void			DestroyRpWorld();

	RwBool					CreateBloom();
	RwBool					CreateLight();

	virtual void			CreateSectorMap(RpWorld *pNtlWorld); // 인도어 virtual 재정의
	
	virtual RwBool			CreateFields(RwInt32 FieldIdx); // 인도어 virtual 재정의
	virtual RwBool			CreateFieldInMemory(RwInt32 FieldIdx); // 인도어 virtual 재정의
	virtual RwBool			CreateFieldFromFile(RwInt32 FieldIdx); // 인도어 virtual 재정의

	RpWorld*				Create(RpWorld *pNtlWorld); // 없는 함수 같은데.
	VOID					DestroyDragoneSky();
	VOID					DestroyBloom();
	void					DestroySky();
	void					DestroyFog();
	void					DestroyWater();
	void					DestroyLight();
	void					DestroyPlant();

	virtual VOID			DeleteFiles4Field(RwInt32 FieldIdx);  // 인도어 virtual 재정의
	virtual RwBool			DeleteFields(RwInt32 FieldIdx); // 인도어 virtual 재정의
	virtual RwBool			DeleteFieldInMemory(RwInt32 FieldIdx, RwBool SaveSwapInToolMode = TRUE);  // 인도어 virtual 재정의
	virtual RwBool			DeleteFieldIntoFile(RwInt32 FieldIdx); // 인도어 virtual 재정의
	virtual RwBool			DeleteFieldThenCreateNew(RwInt32 FieldIdx); // 인도어 virtual 재정의
				
	virtual void			UpdateAnotherField(); // 인도어 virtual 재정의
	void					UpdateCurFieldTexLyr(RwInt32 IdxLyr, RwInt32 IdxMenu);
	void					UpdateCurFieldTexMat(RwInt32 IdxLyr);
	void					UpdateCurFieldTexAdd(RwInt32 IdxLyr);
	virtual void			Update(RwV3d& AvatarPos, RwReal _ElapsedTime); // 인도어 virtual 재정의
	void					Render();
	void					RenderWater(CNtlWorldSector* pNtlWorldSector, RxD3D9InstanceData* pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);

	// could get interpolated height at _Pos. The value could be gotten from some memory or file
	RwBool					GetHeight(RwV3d& _Pos);
	virtual RwBool			GetWorldReady(); // 인도어 : virtual 로 재정의
	EActiveWorldType		GetActiveWorldType() { return m_ActiveWorldType; }
	RwBool					GetFieldSectorIndices(RwInt32 _FieldIdx, RwInt32 _FieldSectorIndices[4]);
	CNtlPLPlanetHandler*	GetPlanetHandler() { return m_pPlanetHandler; }
	CNtlPLWeatherHandler*	GetWeatherHandler() { return m_pWeatherHandler; }
	RwReal					GetMonoFactor();
	eNTL_WORLD_DIR			GetWorldFieldMoveDir() { return m_eMoved2; }

	// override func.
	virtual RwReal			GetWorldSectorHeight(RwV3d& Pos);
	virtual DWORD			GetMapTileProp(RwV3d& Pos);
	virtual BYTE			GetMaterialTileProp(RwV3d& Pos);

	virtual RwReal			GetWaterHeight(RwV3d& Pos);	// 인도어 : virtual 재정의
	RwInt32					GetCurField() { return m_IdxCurField; }
	const CNtlWorldField*	GetFields() { return m_pFields; }
	RwBool					GetVertInMemory(RwV3d& DatumPt, RwV3d& Result);
	RwBool					GetVertFromFile(RwV3d& DatumPt, RwV3d& Result);
	RwBool					GetHeightFromFile(RwV3d& _PosSectorDatum, RwV3d& _PosTile);
	RwInt32					GetFieldIdx(RwV3d& Pos);
	sCUR_FIELD_TEX_INFO*	GetTexAttr();
	sCUR_FIELD_TEX_INFO*	GetTexAttr(RwInt32 IdxField);
	virtual RwBool			GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp); // 인도어 : virtual 재정의
	
	RpAtomic*				GetAtomic(RwV3d& Pos);
	
	RwBool					GetFieldPropVariationStarting();
	CNtlPLLight*			GetNtlPLLight()	{ return m_pLightEntity; }
	CNtlPLBloom*			GetNtlPLBloom() { return m_pBloomEntity; }
	CNtlPLWater*			GetNtlPLWater() { return m_pWaterEntity; }
	RwBool					GetPosVertList(RwV3d& PosCurVert, RwV3d* pPosVertList);
	RwBool					GetClrVertList(RwV3d& PosCurVert, RwRGBA* pClrVertList);
	RwBool					GetNeighborFields(RwInt32 _IdxCurField, vector<RwInt32>& _vecNeighborFields);
	VOID					GetVisible2DAABB(C2DAABB& _2DAABB);
	virtual RwBool			GetWorldShadowColor(RwV3d& Pos, RwRGBA* pOutColor); // 인도어 : virtual 재정의
	virtual RwBool			GetWorldLightColor(RwV3d& Pos, RwRGBA* pOutColor, RwBool bCalcShadow); // 인도어 : virtual 재정의
	RwInt32					GetSelSectorIdxCntInField(RwInt32 _SectorIdx, RwInt32 _FieldIdx);
	RwBool					SetClrVertList(RwV3d& PosCurVert, RwRGBA* pClrVertList);
	RwBool					SetHeight(RwV3d& PosSectorDatum, RwV3d& PosTileDatum);
	virtual RwBool			SetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp, eNTL_FIELD_PROPID NtlFieldPropID); // 인도어 : virtual 재정의
	virtual void			SetAnotherField(RwBool ChangeStraightAway = FALSE); // 인도어 : virtual 재정의
	RwBool					SetPortalPosition(RwV3d& Pos, RwBool _RUFInitFlag = TRUE);	
	VOID					SetPVSActivation(RwBool _Flag);

	void					SetVisiblePlanet(RwBool bVisible);

	virtual RwV3d			GetFieldRelativeCoord(RwInt32 iSField, RwInt32 iEField);

	virtual RwBool			IsFieldLoaded(RwInt32 FieldIdx); // 인도어 : virtual 재정의
	RwBool					RefreshField(RwInt32 FieldIdx);
	RwBool					RefreshPVS();
	RwBool					RefreshFieldAll();
	RwBool					OnDragonSkyAppearence(RwBool _FlgAppeared);
	CNtlPLEntity*			PickEntity();
	
	virtual	RwBool			Pick(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& IntersectionPt, sNtlWorldCollisionInfo* pOutWorldCollsionInfo);
	virtual RwBool			Pick(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt, sNtlWorldCollisionInfo* pOutWorldCollsionInfo);
	virtual	RwBool			PickTerrain(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& IntersectionPt);
	virtual RwBool			PickTerrain(RwV3d& StartPos, RwV3d& EndPos, RwV3d& IntersectionPt); // 인도어 : virtual 재정의

	RwBool					SaveField(RwInt32 FieldIdx);
	RwBool					SaveCurFields();
	virtual RwBool			SaveCurFieldsBeforeExit(); // 인도어 : virtual 재정의
	void					SaveSwapFile(RwInt32 FieldIdx);

	// 인도어
	virtual RwBool			GetWorldDecal(RwV3d& vPosition, RwV3d& vSize, RwInt32 nPolyMaxCount, RwInt32& nRenderVertexCount, RwReal& fUpdateVertextRadius, RwIm3DVertex* pVertices, RwReal OffsetY = 0.1f);

	// 인도어
	virtual void			GetSectopMap(RpWorldSector* pRpWorldSector, std::vector<CNtlWorldSector*>& vecNtlWorldSector);

#ifdef dNTL_WORLD_TOOL_MODE

	virtual void			RenderSectorGuide();
	virtual void			RenderFieldGuide();
	virtual void			RenderFieldGuidePicked();
	virtual void			SetWorldSectorHeightInRectangle(RwV3d& PosS, RwV3d& PosZ, RwReal Height);
	virtual void			OnSetSlopeLighting(RwUInt32 _IdxField);
	virtual void			OnSetSlopeLighting(vector<sTARGET_VERT_SL_INFO>& vecTargetVertSLInfo);
	virtual void			OnSetSlopeLighting(RwInt32 IdxSector);
	
	
	virtual void			OnWaterProc(RwInt32 IdxMenu, RwInt32 IdxSector, sSECTOR_WATER_ATTR& SectorWaterAttr);

#endif

	friend class CNtlWSEField;
	friend class CNtlSCDEField;
};


extern vector<int>			g_vecVisibleSectors;
extern RpAtomic*			WorldIntersectionAtomicCB(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData);