#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlPLWorldState.h"
#include "NtlPLEntityFreeList.h"
#include "NtlPLResource.h"
#include "NtlPLResourceManager.h"
#include "NtlPLWorldEntity.h"
#include "Ntlworldinterface.h"
#include "ntlworldcommon.h"
#include "NtlPLWorldProp.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"
#include "rtpick.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlWorldBrush.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"
#include "NtlMath.h"
#include "NtlPLSky.h"
#include "NtlPLSkyDragon.h"
#include "NtlPLPicking.h"
#include "ntlworlddi.h"
#include "NtlPLGlobal.h"
#include "ntltext.h"
#include "TextAllTable.h"
#include "table.h"
#include "NtlWorldLTManager.h"


void* CNtlPLWorldEntity::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLWorldEntity::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_WORLD));
}

void CNtlPLWorldEntity::operator delete(void* pObj)
{
	NTL_FUNCTION("CNtlPLWorldEntity::operator delete");

	CNtlPLEntityFreeList::Free(PLENTITY_WORLD, pObj);

	NTL_RETURNVOID();
}

RwBool CNtlPLWorldEntity::GetPlayerPosition(RwV3d& Pos)
{
	if(GetWorldReady())
	{
		Pos = m_AvatarPos;
		return TRUE;
	}

	return FALSE;
}

void CNtlPLWorldEntity::SetPlayerPosition(RwV3d& Pos)
{
	::CopyMemory(&m_AvatarPos, &Pos, sizeof(RwV3d));
}

RwBool CNtlPLWorldEntity::SetPortalPosition(RwV3d& Pos, RwBool _RUFInitFlag/* = TRUE*/)
{
	CNtlPLWorldState::InitSpace();
	
	m_pNtlWorldFieldManger->SetPortalPosition(Pos, _RUFInitFlag);

	return TRUE;
}

RwBool CNtlPLWorldEntity::SetDNEffect(RwInt32 Brightness)
{
	if(Brightness < 0 || Brightness > 255)
	{
		return FALSE;
	}

	dGET_WORLD_PARAM()->ClrDayAndNight = (Brightness << 16) | (Brightness << 8) | (Brightness << 0);

	return TRUE;
}

RpAtomic* CNtlPLWorldEntity::GetAtomic(RwV3d& Pos)
{
	RwInt32 IdxSector = m_pNtlWorldFieldManger->GetSectorIdx(Pos);

	if(IdxSector == -1)
	{
		return NULL;
	}
	else
	{
		return m_pNtlWorldFieldManger->GetAtomic(Pos);
	}

	return NULL;
}

void CNtlPLWorldEntity::Init( void )
{
	NTL_FUNCTION("CNtlPLWorldEntity::Init");

#ifdef dNTL_WORLD_TOOL_MODE
	m_pNtlWorld				= NULL;
#endif

	m_pNtlPLResource		= NULL;
	m_pNtlWorldFieldManger	= NULL;
	m_pNtlBrush				= NULL;

	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetClassType(PLENTITY_WORLD);
	SetFlags(NTL_PLEFLAG_ATTACH | NTL_PLEFLAG_NOT_PROPERTY_USED);	

	CNtlMath::MathRwV3dAssign(&m_AvatarPos, 0.0f, 0.0f, 0.0f);

	NTL_RETURNVOID();
}

RwBool CNtlPLWorldEntity::IsAnotherFieldChanged()
{
	if(!m_pNtlWorldFieldManger)
		return FALSE;

	return m_pNtlWorldFieldManger->GetFieldPropVariationStarting();
}

RwBool CNtlPLWorldEntity::Create(const SPLEntityCreateParam *pParam)
{
	NTL_FUNCTION("CNtlPLWorldEntity::Init");

#ifdef dNTL_WORLD_TOOL_MODE
	m_pNtlWorld = RpNtlWorldCreate();
	DBO_ASSERT(m_pNtlWorld, "World create failed.");

	if(m_pNtlWorld)
		CNtlPLGlobal::SetActiveWorld(m_pNtlWorld);
#endif

	NTL_RETURN(TRUE);
}

void CNtlPLWorldEntity::CreateWorld(RwV3d& SPos)
{
	NTL_PRE(m_pNtlWorldFieldManger == NULL);

	CNtlPLWorldState::Init();

//	DBO_WARNING_MESSAGE("dGET_WORLD_PARAM()->WorldType: " << dGET_WORLD_PARAM()->WorldType);
	if (dGET_WORLD_PARAM()->WorldType == 0)
	{
		m_pNtlWorldFieldManger = NTL_NEW CNtlWorldFieldManager(NULL, SPos, AW_HEGITHFIELD);		
		CNtlPLWorldState::eActiveWorldType = AW_HEGITHFIELD;
	}
	else
	{
		m_pNtlWorldFieldManger = NTL_NEW CNtlWorldFieldManager4RWWorld(NULL, SPos, AW_RWWORLD);
		CNtlPLWorldState::eActiveWorldType = AW_RWWORLD;
	}

	DBO_ASSERT(m_pNtlWorldFieldManger, "Memory alloc failed.");

	m_pNtlWorldFieldManger->Init(NULL, SPos);

#ifdef dNTL_WORLD_TOOL_MODE
	// create circular brush 4 default
	m_pNtlBrush = NTL_NEW CNtlWorldBrush;
	CNtlWorldBrushController *pController = NTL_NEW CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, m_pNtlWorldFieldManger, &CNtlWorldFieldManager::GetWorldSectorHeight);
	m_pNtlBrush->SetController(pController);
#endif

#ifndef dNTL_WORLD_TOOL_MODE

	m_AvatarPos = SPos;

#endif
}

void CNtlPLWorldEntity::Destroy()
{
	if (m_pNtlWorldFieldManger)
	{
		m_pNtlWorldFieldManger->SaveCurFieldsBeforeExit();
		m_pNtlWorldFieldManger->Free();
		NTL_DELETE(m_pNtlWorldFieldManger);
	}

#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pNtlWorld)
	{
		RpWorldDestroy(m_pNtlWorld);
		m_pNtlWorld = NULL;
	}	
#endif

	CNtlPLWorldState::Terminate();

	CNtlPLGlobal::SetActiveWorld(NULL);

	NTL_DELETE(m_pNtlBrush);
}

RwBool CNtlPLWorldEntity::SetProperty(const CNtlPLProperty* pProperty)
{
	NTL_FUNCTION("CNtlPLWorldEntity::SetProperty");

	if(pProperty == NULL)
		NTL_RETURN(FALSE);

	CNtlPLWorldProp * pWorldProperty = static_cast<CNtlPLWorldProp *>(const_cast<CNtlPLProperty *>(pProperty));
	if( pWorldProperty == NULL )
		NTL_RETURN( FALSE );

	NTL_RETURN(TRUE);
}

RwReal CNtlPLWorldEntity::GetMonoFactor()
{
	return m_pNtlWorldFieldManger->GetMonoFactor();
}

TBLIDX CNtlPLWorldEntity::GetMapNameCode(RwV3d& Pos)
{
	sNTL_FIELD_PROP NtlFieldProp;
	if(!m_pNtlWorldFieldManger->GetAFieldProp(Pos, NtlFieldProp))
	{
		return 0xffffffff;
	}

	if(!_stricmp(NtlFieldProp._Name, "null") || !_stricmp(NtlFieldProp._Name, ""))
	{
		return 0xffffffff;
	}

	return static_cast<TBLIDX>(::atoi(NtlFieldProp._Name));
}

RwInt32 CNtlPLWorldEntity::GetWorldFieldIdx(RwV3d& Pos, RwV3d& FieldSPos)
{
	RwInt32 FieldIdx = m_pNtlWorldFieldManger->GetFieldIdx(Pos);

	if(FieldIdx != -1)
	{
		FieldSPos = const_cast<CNtlWorldField*>(m_pNtlWorldFieldManger->GetFields())[FieldIdx].GetSPos();
	}
	else
	{
		FieldSPos.x = FieldSPos.y = FieldSPos.z = 0.0f;
	}

	return FieldIdx;
}

DWORD CNtlPLWorldEntity::GetMapTileProp(RwV3d& Pos)
{
	return m_pNtlWorldFieldManger->GetMapTileProp(Pos);
}

BYTE CNtlPLWorldEntity::GetMaterialTileProp(RwV3d& Pos)
{
	return m_pNtlWorldFieldManger->GetMaterialTileProp(Pos);
}

RwBool CNtlPLWorldEntity::GetWorldShadowColor(RwV3d& vPos, RwRGBA* pOutColor)
{
	return m_pNtlWorldFieldManger->GetWorldShadowColor(vPos, pOutColor);
}

RwBool CNtlPLWorldEntity::GetWorldLightColor(RwV3d& vPos, RwRGBA* pOutColor, RwBool bCalcShadow)
{
	return m_pNtlWorldFieldManger->GetWorldLightColor(vPos, pOutColor, bCalcShadow);
}

RwReal CNtlPLWorldEntity::GetWaterHeight(RwV3d& Pos)
{
	return m_pNtlWorldFieldManger->GetWaterHeight(Pos);
}

RwBool CNtlPLWorldEntity::GetAFieldProp(RwV3d& Pos, sNTL_FIELD_PROP& NtlFieldProp)
{
	m_pNtlWorldFieldManger->GetAFieldProp(Pos, NtlFieldProp);

	return TRUE;
}

RwBool CNtlPLWorldEntity::GetWorldDecal(RwV3d& vPosition, RwV3d& vSize, RwInt32 nPolyMaxCount, RwInt32& nRenderVertexCount, RwReal& fUpdateVertextRadius, RwIm3DVertex* pVertices, RwReal OffsetY)
{
	NTL_FUNCTION("CNtlPLWorldEntity::GetWorldDecal");

	NTL_RETURN(m_pNtlWorldFieldManger->GetWorldDecal(vPosition, vSize, nPolyMaxCount, nRenderVertexCount, fUpdateVertextRadius, pVertices, OffsetY));
}

RwReal CNtlPLWorldEntity::GetWorldSectorHeight(RwV3d& Pos)
{
	if(!m_pNtlWorldFieldManger)
		return -999.0f;

	return m_pNtlWorldFieldManger->GetWorldSectorHeight(Pos);
}

bool CNtlPLWorldEntity::IsWorking()
{
	if(m_pNtlWorldFieldManger)
		return true;
	else 
		return false;
}

RwBool CNtlPLWorldEntity::PickTerrainAndWater(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& _IntersectionPt)
{
	return m_pNtlWorldFieldManger->Pick(ScreenPosX, ScreenPosY, _IntersectionPt, NULL);
}

RwBool CNtlPLWorldEntity::PickTerrainAndWater(RwV3d& StartPos, RwV3d& EndPos, RwV3d& _IntersectionPt)
{
	return m_pNtlWorldFieldManger->Pick(StartPos, EndPos, _IntersectionPt, NULL);
}

RwBool CNtlPLWorldEntity::PickTerrain(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& _IntersectionPt)
{
	return m_pNtlWorldFieldManger->PickTerrain(ScreenPosX, ScreenPosY, _IntersectionPt);
}

RwBool CNtlPLWorldEntity::PickTerrain(RwV3d& StartPos, RwV3d& EndPos, RwV3d& _IntersectionPt)
{
	return m_pNtlWorldFieldManger->PickTerrain(StartPos, EndPos, _IntersectionPt);
}

RwBool CNtlPLWorldEntity::Pick(RwInt32 ScreenPosX, RwInt32 ScreenPosY, RwV3d& _IntersectionPt)
{
	return m_pNtlWorldFieldManger->Pick(ScreenPosX, ScreenPosY, _IntersectionPt, NULL);
}

RwBool CNtlPLWorldEntity::Pick(RwV3d& StartPos, RwV3d& EndPos, RwV3d& _IntersectionPt)
{
	return m_pNtlWorldFieldManger->Pick(StartPos, EndPos, _IntersectionPt, NULL);
}

void CNtlPLWorldEntity::OnBtnDN(RwReal x, RwReal y)
{
	if(!m_pNtlWorldFieldManger)
		return;

	if(m_pNtlWorldFieldManger)
	{
		RwV3d vPickPos; 
		dGET_COLLISION_INFO_UPDATE() = TRUE;
		m_pNtlWorldFieldManger->Pick((RwInt32)x, (RwInt32)y, vPickPos, dGET_COLLISION_INFO());
		dGET_COLLISION_INFO_UPDATE() = FALSE;
	}

	if(!m_pNtlBrush)
		return;

	switch(dGET_CUR_PALETTE())
	{
	case ePM_TERRAIN:
		{
			if(m_pNtlBrush->GetController())
			{
				if(m_pNtlBrush->GetController()->GetID() == eNBT_RECTANGLE)
				{
					RwV3d Pos;
					CNtlMath::MathRwV3dAssign(&Pos, dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x, 0.0f, dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z);
					reinterpret_cast<CNtlWB_Rectangle<CNtlWorldFieldManager>*>(m_pNtlBrush->GetController())->SetPosS(Pos);
				}
			}

			if(dGET_TERRAIN_MODE() == eMEM_JUST)
				dGET_COLLISION_INFO()->FlgStopColliding = TRUE;
		}
		break;
	case ePM_TEXTURE:
		{
			// 현재 필드 다열로그를 채워야 한다.
		}
		break;
	case ePM_ORNAMENT:
		break;
	case ePM_UNIT:
		break;
	case ePM_REGION:
		break;
	case ePM_TRIGGER:
		break;
	}
}

void CNtlPLWorldEntity::OnBtnMV(RwReal x, RwReal y)
{
	if(!m_pNtlWorldFieldManger)
		return;

	if(dGET_TERRAIN_MODE() == eMEM_JUST && dGET_COLLISION_INFO()->FlgStopColliding)
		return;

	if(m_pNtlWorldFieldManger)
	{
		RwV3d vPickPos;
		dGET_COLLISION_INFO_UPDATE() = TRUE;
		m_pNtlWorldFieldManger->Pick((RwInt32)x, (RwInt32)y, vPickPos, dGET_COLLISION_INFO());
		dGET_COLLISION_INFO_UPDATE() = FALSE;
	}

	if(!m_pNtlBrush)
		return;
}

void CNtlPLWorldEntity::OnBtnUP(RwReal x, RwReal y)
{
	if(!m_pNtlWorldFieldManger)
		return;

	if(dGET_TERRAIN_MODE() == eMEM_JUST)
		dGET_COLLISION_INFO()->FlgStopColliding = FALSE;

	if(m_pNtlWorldFieldManger)
	{
		RwV3d vPickPos; 
		dGET_COLLISION_INFO_UPDATE() = TRUE;
		m_pNtlWorldFieldManger->Pick((RwInt32)x, (RwInt32)y, vPickPos, dGET_COLLISION_INFO());
		dGET_COLLISION_INFO_UPDATE() = FALSE;
	}

	if(!m_pNtlBrush)
		return;
}

EActiveWorldType CNtlPLWorldEntity::GetActiveWorldType()
{
	if (!m_pNtlWorldFieldManger)
	{
		return AW_NONE;
	}
	return m_pNtlWorldFieldManger->GetActiveWorldType();
}

RwBool CNtlPLWorldEntity::GetWorldReady()
{
	return m_pNtlWorldFieldManger->GetWorldReady();
}

void CNtlPLWorldEntity::OnBrushRender()
{
	if(!m_pNtlWorldFieldManger)
		return;

#ifdef dNTL_WORLD_TOOL_MODE

	if (GetActiveWorldType() == AW_HEGITHFIELD)
	{
		CNtlWorldFieldManager* pNtlWorldFieldManger = m_pNtlWorldFieldManger;

		if(dGET_WORLD_PARAM()->SectorGuideLine)
		{
			pNtlWorldFieldManger->RenderSectorGuide();
		}

		if(dGET_WORLD_PARAM()->FieldGuideLine)
		{
			pNtlWorldFieldManger->RenderFieldGuide();
		}

		if(dGET_WORLD_PARAM()->SectorGuideLine || dGET_WORLD_PARAM()->FieldGuideLine)
		{
			if(dGET_CUR_PALETTE() == ePM_TEXTURE)
				pNtlWorldFieldManger->RenderFieldGuidePicked();
		}

		if(dGET_CUR_PALETTE() == ePM_TERRAIN || dGET_CUR_PALETTE() == ePM_TEXTURE || dGET_CUR_PALETTE() == ePM_DIFFUSE)
		{
			m_pNtlBrush->Render();
		}
	}
	else if (GetActiveWorldType() == AW_RWWORLD)
	{
		CNtlWorldFieldManager4RWWorld* pNtlWorldFieldManger = (CNtlWorldFieldManager4RWWorld*)m_pNtlWorldFieldManger;

		if(dGET_WORLD_PARAM()->BlockGuideLine)
		{
			pNtlWorldFieldManger->RenderBlockGuide();
		}

		if(dGET_WORLD_PARAM()->SectorGuideLine)
		{
			pNtlWorldFieldManger->RenderSectorGuide();
		}

		if(dGET_WORLD_PARAM()->FieldGuideLine)
		{
			pNtlWorldFieldManger->RenderFieldGuide();
		}
	}					

#endif
}

void CNtlPLWorldEntity::OnDragonSkyAppearence(RwBool _FlgDragonAppeared)
{
	m_pNtlWorldFieldManger->OnDragonSkyAppearence(_FlgDragonAppeared);
}

RwBool CNtlPLWorldEntity::Update(RwReal fElapsed)
{
	if(m_pNtlWorldFieldManger)
	{
#ifndef dNTL_WORLD_TOOL_MODE
		if (!GetSceneManager()->GetAvailablePos(m_AvatarPos))
		{
			DBO_ASSERT((FALSE), "unavailable avatar pos.");
			return TRUE;
		}

		// Camera는 World의 전체크기 밖으로만 나가지 않으면 되므로 AOI 영역을 제외하고 검사하지 않는다.
		RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - 1);
		RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + 1);
		if(m_AvatarPos.x > Max || m_AvatarPos.x < Min || m_AvatarPos.z > Max || m_AvatarPos.z < Min)
		{
			DBO_ASSERT((FALSE), "unavailable camera pos.");
			return TRUE;
		}
#endif

		m_pNtlWorldFieldManger->Update(m_AvatarPos, fElapsed);

		CNtlPLWorldState::Update(m_AvatarPos);
	}

	return TRUE;
}

void CNtlPLWorldEntity::RenderWater(CNtlWorldSector* pNtlWorldSector, RxD3D9InstanceData* pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	m_pNtlWorldFieldManger->RenderWater(pNtlWorldSector, pInstancedData, pResEntryHeader);
}

RwBool CNtlPLWorldEntity::Render( void )
{
#ifdef dNTL_WORLD_TOOL_MODE
	OnBrushRender();
#endif
	if (m_pNtlWorldFieldManger)
	{
		m_pNtlWorldFieldManger->Render();
	}

	return TRUE;
}

void CNtlPLWorldEntity::SetVisiblePlanet(RwBool bVisible)
{
	m_pNtlWorldFieldManger->SetVisiblePlanet(bVisible);
}

RpWorld* CNtlPLWorldEntity::GetWorld()
{
#ifdef dNTL_WORLD_TOOL_MODE
	RpWorld* pRpWorld = NULL;
#endif
	if (m_pNtlWorldFieldManger)
	{
#ifdef dNTL_WORLD_TOOL_MODE
		pRpWorld = m_pNtlWorldFieldManger->GetWorld();
#else
		return m_pNtlWorldFieldManger->GetWorld();
#endif
	}
#ifdef dNTL_WORLD_TOOL_MODE
	return (pRpWorld ? pRpWorld : m_pNtlWorld);
#else
	return NULL;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CDynamicPlane::CDynamicPlane(const RwV3d& vCenter, const RwV3d& vNormal, const RwV3d& vTangent, RwReal fWidth, RwReal fHeight, RwReal fDepth)
{
	m_vCenter = vCenter;
	m_vNormal = vNormal;

	RwV3d	vCrossNT;
	RwReal	fDist;

	RwV3dCrossProduct(&vCrossNT, &vNormal, &vTangent);

	// Left / Right
	fDist = RwV3dDotProduct(&vCenter, &vTangent);
	m_planeLeft.normal		= vTangent;
	m_planeLeft.distance	= fWidth * 0.5f - fDist;
	//m_planeRight.normal		= -vTangent;
	RwV3dNegate(&m_planeRight.normal, &vTangent);
	m_planeRight.distance	= fWidth * 0.5f + fDist;

	// Top / Bottom
	fDist = RwV3dDotProduct(&vCenter, &vCrossNT);
	m_planeBottom.normal	= vCrossNT;
	m_planeBottom.distance	= fHeight * 0.5f - fDist;
	//m_planeTop.normal		= -vCrossNT;
	RwV3dNegate(&m_planeTop.normal, &vCrossNT);
	m_planeTop.distance		= fHeight * 0.5f + fDist;

	// Front / Back
	fDist = RwV3dDotProduct(&vCenter, &vNormal);
	m_planeBack.normal		= vNormal;
	m_planeBack.distance	= fDepth - fDist;
	//m_planeFront.normal		= -vNormal;
	RwV3dNegate(&m_planeFront.normal, &vNormal);
	m_planeFront.distance	= fDepth + fDist;

	m_iDecalVertexCount		= 0;
	m_iDecalTriangleCount	= 0;

	RwReal fOneW = 1.0F / fWidth;
	RwReal fOneH = 1.0F / fHeight;
	for (RwInt32 i = 0; i < m_iDecalVertexCount; ++i)
	{
		RwV3d v = m_vertexArray[i] - m_vCenter;
		m_texcoordArray[i].x = RwV3dDotProduct(&v, &vTangent) * fOneW + 0.5f;
		m_texcoordArray[i].y = RwV3dDotProduct(&v, &vCrossNT) * fOneH + 0.5f;
	}
}

CDynamicPlane::~CDynamicPlane()
{
}

RwBool CDynamicPlane::AddPolygon(RwInt32 iVertCount, const RwV3d* vVertex, const RwV3d* vNormal)
{
	RwInt32 iCount = m_iDecalVertexCount;
	if (iCount + iVertCount >= dDECAL_VERTICES_MAX)
	{
		return FALSE;
	}

	RpTriangle* triangle = m_triangleArray + m_iDecalTriangleCount;
	m_iDecalTriangleCount += iVertCount - 2;
	for (RwInt32 i = 2; i < iVertCount; ++i)
	{
		triangle->vertIndex[0] = (RwInt16)(iCount);
		triangle->vertIndex[1] = (RwInt16)(iCount + i - 1);
		triangle->vertIndex[2] = (RwInt16)(iCount + i);
		++triangle;
	}

	RwReal f = 1.0f / (1.0f - dDECAL_EPSILON);
	for (RwInt32 i = 0; i < iVertCount; ++i)
	{
		m_vertexArray[iCount] = vVertex[i];

		RwReal fAlpha = (RwV3dDotProduct(&m_vNormal, &vNormal[i]) / RwV3dLength(&vNormal[i]) - dDECAL_EPSILON) * f;

		m_colorArray[iCount].red	= 1.0f;
		m_colorArray[iCount].green	= 1.0f;
		m_colorArray[iCount].blue	= 1.0f;
		m_colorArray[iCount].alpha	= (fAlpha > 0.0f ? fAlpha : 0.0f);

		++iCount;
	}

	m_iDecalVertexCount = iCount;

	return TRUE;
}

void CDynamicPlane::ClipMesh(RwInt32 iTriCount, const RwV3d* vVertex, const RwV3d* vNormal)
{
	RwV3d vNewVertex[9];
	RwV3d vNewNormal[9];

	for (RwInt32 i = 0; i < iTriCount; ++i)
	{
		RwInt32 i1 = 3 * i;
		RwInt32 i2 = i1 + 1;
		RwInt32 i3 = i2 + 1;

		RwV3d vCross;
		RwV3dCrossProduct(&vCross, &(vVertex[i2] - vVertex[i1]), &(vVertex[i3] - vVertex[i1]));
		if (RwV3dDotProduct(&m_vNormal, &vCross) > dDECAL_EPSILON * RwV3dLength(&vCross))
		{
			vNewVertex[0] = vVertex[i1];
			vNewVertex[1] = vVertex[i2];
			vNewVertex[2] = vVertex[i3];

			vNewNormal[0] = vNormal[i1];
			vNewNormal[1] = vNormal[i2];
			vNewNormal[2] = vNormal[i3];

			RwInt32 iCount = ClipPolygon(3, vNewVertex, vNewNormal, vNewVertex, vNewNormal);
			if ((iCount != 0) && (!AddPolygon(iCount, vNewVertex, vNewNormal)))
			{
				break;
			}
		}
	}
}

void CDynamicPlane::ClipMesh(RwInt32 iTriCount, const RwIm3DVertex* vImVertex)
{
	RwV3d vNewVertex[9];
	RwV3d vNewNormal[9];

	for (RwInt32 i = 0; i < iTriCount; ++i)
	{
		RwInt32 i1 = 3 * i;
		RwInt32 i2 = i1 + 1;
		RwInt32 i3 = i2 + 1;

		RwV3d vCross;
		RwV3dCrossProduct(&vCross, &(vImVertex[i2].objVertex - vImVertex[i1].objVertex), &(vImVertex[i3].objVertex - vImVertex[i1].objVertex));
		if (RwV3dDotProduct(&m_vNormal, &vCross) > dDECAL_EPSILON * RwV3dLength(&vCross))
		{
			vNewVertex[0] = vImVertex[i1].objVertex;
			vNewVertex[1] = vImVertex[i2].objVertex;
			vNewVertex[2] = vImVertex[i3].objVertex;

			vNewNormal[0] = vImVertex[i1].objNormal;
			vNewNormal[1] = vImVertex[i2].objNormal;
			vNewNormal[2] = vImVertex[i3].objNormal;

			RwInt32 iCount = ClipPolygon(3, vNewVertex, vNewNormal, vNewVertex, vNewNormal);
			if ((iCount != 0) && (!AddPolygon(iCount, vNewVertex, vNewNormal)))
			{
				break;
			}
		}
	}
}

RwInt32 CDynamicPlane::ClipPolygon(RwInt32 iVertCount, const RwV3d* vVertex, const RwV3d* vNormal, RwV3d* vNewVertex, RwV3d* vNewNormal)
{
	RwV3d vTempVertex[9];
	RwV3d vTempNormal[9];

	RwInt32 iCount = ClipPolygonAgainstPlane(m_planeLeft, iVertCount, vVertex, vNormal, vTempVertex, vTempNormal);
	if (iCount != 0)
	{
		iCount = ClipPolygonAgainstPlane(m_planeRight, iCount, vTempVertex, vTempNormal, vNewVertex, vNewNormal);
		if (iCount != 0)
		{
			iCount = ClipPolygonAgainstPlane(m_planeBottom, iCount, vNewVertex, vNewNormal, vTempVertex, vTempNormal);
			if (iCount != 0)
			{
				iCount = ClipPolygonAgainstPlane(m_planeTop, iCount, vTempVertex, vTempNormal, vNewVertex, vNewNormal);
				if (iCount != 0)
				{
					iCount = ClipPolygonAgainstPlane(m_planeBack, iCount, vNewVertex, vNewNormal, vTempVertex, vTempNormal);
					if (iCount != 0)
					{
						iCount = ClipPolygonAgainstPlane(m_planeFront, iCount, vTempVertex, vTempNormal, vNewVertex, vNewNormal);
					}
				}
			}
		}
	}

	return iCount;
}

RwInt32 CDynamicPlane::ClipPolygonAgainstPlane(const RwPlane& plane, RwInt32 vVertCount, const RwV3d* vVertex, const RwV3d* vNormal, RwV3d* vNewVertex, RwV3d* vNewNormal)
{
	RwBool bNegative[10];

	RwInt32 iNegativeCount = 0;
	for (RwInt32 i = 0; i < vVertCount; ++i)
	{
		if ((bNegative[i] = ((RwV3dDotProduct(&plane.normal, &vVertex[i]) + plane.distance) < 0.0f)))
		{
			++iNegativeCount;
		}		
	}

	if (iNegativeCount == vVertCount)
	{
		return 0;
	}

	RwInt32 iCount = 0;
	for (long i = 0; i < vVertCount; ++i)
	{
		long iIndex = (i != 0) ? i - 1 : vVertCount - 1;

		if (bNegative[i])
		{
			if (!bNegative[iIndex])
			{
				RwReal fT		= (RwV3dDotProduct(&plane.normal, &vVertex[iIndex]) + plane.distance)
					/ (plane.normal.x * (vVertex[iIndex].x - vVertex[i].x)
					+ plane.normal.y * (vVertex[iIndex].y - vVertex[i].y)
					+ plane.normal.z * (vVertex[iIndex].z - vVertex[i].z));

				vNewVertex[iCount] = vVertex[iIndex] * (1.0f - fT) + vVertex[i] * fT;
				vNewNormal[iCount] = vNormal[iIndex] * (1.0f - fT) + vNormal[i] * fT;

				++iCount;
			}
		}
		else
		{
			if (bNegative[iIndex])
			{
				RwReal fT		= (RwV3dDotProduct(&plane.normal, &vVertex[i]) + plane.distance)
					/ (plane.normal.x * (vVertex[i].x - vVertex[iIndex].x)
					+ plane.normal.y * (vVertex[i].y - vVertex[iIndex].y)
					+ plane.normal.z * (vVertex[i].z - vVertex[iIndex].z));

				vNewVertex[iCount] = vVertex[i] * (1.0f - fT) + vVertex[iIndex] * fT;
				vNewNormal[iCount] = vNormal[i] * (1.0f - fT) + vNormal[iIndex] * fT;

				++iCount;
			}

			vNewVertex[iCount] = vVertex[i];
			vNewNormal[iCount] = vNormal[i];
			++iCount;
		}
	}

	return iCount;
}
