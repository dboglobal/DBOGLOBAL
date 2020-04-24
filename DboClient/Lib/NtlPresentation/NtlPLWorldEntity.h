//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "NtlplAttach.h"
#include "NtlPlFog.h"
#include "NtlPLPicking.h"
#include "Ntlworldcommon.h"
#include "NtlPLSkyDragon.h"
#include "NtlPLSceneManager.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlPLResource;
class CNtlWorldFieldManager;
class CNtlWorldBrush;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct RpWorld;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlPLWorldEntity : public CNtlPLAttach
{
public:
	CNtlPLWorldEntity(void)				{ Init(); }
	virtual ~CNtlPLWorldEntity(void)	{;}

protected:
#ifdef dNTL_WORLD_TOOL_MODE
	RpWorld*					m_pNtlWorld;
#endif

	CNtlPLResource*				m_pNtlPLResource;
	CNtlWorldFieldManager*		m_pNtlWorldFieldManger;
	RwV3d						m_AvatarPos;

public:
	CNtlWorldBrush* m_pNtlBrush;
	RwInt32			m_VertTotCnt;

public:
	void*	operator new(size_t size);
	void	operator delete(void* pObj);

public:
	EActiveWorldType		GetActiveWorldType();
	RwReal					GetMonoFactor();
	TBLIDX					GetMapNameCode(RwV3d& Pos);
	RwInt32					GetWorldFieldIdx(RwV3d& Pos, RwV3d& FieldSPos);
	DWORD					GetMapTileProp(RwV3d& Pos);
	BYTE					GetMaterialTileProp(RwV3d& Pos);
	RwBool					GetWorldShadowColor(RwV3d& vPos, RwRGBA* pOutColor);
	RwBool					GetWorldLightColor(RwV3d& vPos, RwRGBA* pOutColor, RwBool bCalcShadow);
	RwReal					GetWaterHeight(RwV3d& Pos);
	RwBool					GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp);
	RpWorld*				GetWorld();
	RwBool					GetWorldReady();
	RwReal					GetWorldSectorHeight(RwV3d& Pos);
	CNtlWorldFieldManager*	GetWorldFieldMgr() const { return m_pNtlWorldFieldManger; }
	RpAtomic*				GetAtomic(RwV3d& Pos);
	RwBool					GetWorldDecal(RwV3d& vPosition, RwV3d& vSize, RwInt32 nPolyMaxCount, RwInt32& nRenderVertexCount, RwReal& fUpdateVertextRadius, RwIm3DVertex* pVertices, RwReal OffsetY = 0.1f);
	RwBool					GetPlayerPosition(RwV3d& Pos);
	void					SetPlayerPosition(RwV3d& Pos);
	RwBool					SetPortalPosition(RwV3d& Pos, RwBool _RUFInitFlag = TRUE);
	RwBool					SetDNEffect(RwInt32 Brightness);
	virtual RwBool			SetProperty(const CNtlPLProperty* pProperty);

	RwBool					IsAnotherFieldChanged();
	bool					IsWorking();

	RwBool					Pick(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& _IntersectionPt);
	RwBool					Pick(RwV3d& StartPos, RwV3d& EndPos, RwV3d& _IntersectionPt);

	RwBool					PickTerrain(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& _IntersectionPt);
	RwBool					PickTerrain(RwV3d& StartPos, RwV3d& EndPos, RwV3d& _IntersectionPt);

	RwBool					PickTerrainAndWater(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& _IntersectionPt);
	RwBool					PickTerrainAndWater(RwV3d& StartPos, RwV3d& EndPos, RwV3d& _IntersectionPt);
	
	void					Init(void);
	virtual RwBool			Create(const SPLEntityCreateParam *pParam = NULL);
	void					CreateWorldDefaultParam();
	void					CreateWorld(RwV3d& SPos);

	void					UpdateSky(RwV3d *pPosAvatar);
	void					UpdateFog(RwV3d *pPosAvatar);
	virtual RwBool			Update(RwReal fElapsed);

	virtual RwBool			Render();
	void					RenderWater(CNtlWorldSector* pNtlWorldSector, RxD3D9InstanceData* pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);

	virtual void			Destroy();

	void					OnBtnDN(RwReal x, RwReal y);
	void					OnBtnMV(RwReal x, RwReal y);
	void					OnBtnUP(RwReal x, RwReal y);
	void					OnBrushRender();
	void					OnDragonSkyAppearence(RwBool _FlgDragonAppeared);

	void					SetVisiblePlanet(RwBool bVisible);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Decal 최적화를 위해 임시로 썼던 클래스 : 자세한 내용은 GPG2 4의 Dacal 부분.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define dDECAL_EPSILON		(0.5f)
#define dDECAL_VERTICES_MAX (256)


class CDynamicPlane
{
public:
	CDynamicPlane(const RwV3d& vCenter, const RwV3d& vNormal, const RwV3d& vTangent, RwReal fWidth, RwReal fHeight, RwReal fDepth);
	virtual ~CDynamicPlane();

	RwBool	AddPolygon(RwInt32 iVertCount, const RwV3d* vVertex, const RwV3d* vNormal);

	void	ClipMesh(RwInt32 iTriCount, const RwIm3DVertex* vImVertex);
	void	ClipMesh(RwInt32 iTriCount, const RwV3d* vVertex, const RwV3d* vNormal);
	RwInt32 ClipPolygon(RwInt32 iVertCount, const RwV3d* vVertex, const RwV3d* vNormal, RwV3d* vNewVertex, RwV3d* vNewNormal);
	RwInt32 ClipPolygonAgainstPlane(const RwPlane& plane, RwInt32 vVertCount, const RwV3d* vVertex, const RwV3d* vNormal, RwV3d* vNewVertex, RwV3d* vNewNormal);

public:
	RwV3d			m_vCenter;
	RwV3d			m_vNormal;

	RwPlane			m_planeLeft;
	RwPlane			m_planeRight;
	RwPlane			m_planeTop;
	RwPlane			m_planeBottom;
	RwPlane			m_planeFront;
	RwPlane			m_planeBack;

	RwInt32			m_iDecalVertexCount;
	RwInt32			m_iDecalTriangleCount;

	RwV3d			m_vertexArray[dDECAL_VERTICES_MAX];
	RwV2d			m_texcoordArray[dDECAL_VERTICES_MAX];
	RwRGBAReal		m_colorArray[dDECAL_VERTICES_MAX];
	RpTriangle		m_triangleArray[dDECAL_VERTICES_MAX];
};