#include "precomp_ntlpresentation.h"

#include "NtlWorldCommon.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlWorldInterface.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"
#include "NtlWorldBlockManager.h"

#include "NtlPLRenderState.h"

#include "NtlPLRenderGeometry.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLGameProperty.h"

#ifdef dNTL_WORLD_TOOL_MODE
CNtlPLGameProperty* CNtlPLGameProperty::s_pPLGamePropertySelected = NULL;
#endif

CNtlPLGameProperty::CNtlPLGameProperty()
:m_eShapeType(ENTL_PL_GAME_PROPERTY_SHAPE_NONE)
{
#ifdef dNTL_WORLD_TOOL_MODE
	SetLayer(PLENTITY_LAYER_TOOL);
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
	
#else
	SetLayer(PLENTITY_LAYER_NONE);
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOTUPDATE | NTL_PLEFLAG_NOT_VISIBLE);
#endif
	
	SetClassType(PLENTITY_GAME_PROPERTY);

#ifdef dNTL_WORLD_TOOL_MODE
	m_pAtomicCtlPt = NULL;
#endif

	m_uiProperty = 0x00000000;
	m_vPos.x = m_vPos.y = m_vPos.z = m_vSize.x = m_vSize.y = m_vSize.z = 0.0f;
}

CNtlPLGameProperty::~CNtlPLGameProperty()
{
}

RwBool CNtlPLGameProperty::SetProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLGameProperty::SetProperty");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLGameProperty::Create(const SPLEntityCreateParam *pParam)
{
	NTL_FUNCTION("CNtlPLGameProperty::Create");

	SPLGamePropertyCreateParam* pCreateParam = (SPLGamePropertyCreateParam*)(pParam);

	SetShapeType(pCreateParam->eShapeType);
	SetPosition(pCreateParam->pPos);
	SetSize(&pCreateParam->vSize);

#ifdef dNTL_WORLD_TOOL_MODE
	m_pAtomicCtlPt = API_PL_LoadAtomic("tool/saber/mesh/Light_Diamond.dff", "");
	DBO_ASSERT(m_pAtomicCtlPt, "Resource load failed.");
#endif

	NTL_RETURN(FALSE);
}

void CNtlPLGameProperty::Destroy()
{
	NTL_FUNCTION("CNtlPLGameProperty::Destroy");

#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pAtomicCtlPt)
	{
		API_PL_DeleteAtomic(m_pAtomicCtlPt);
		m_pAtomicCtlPt = NULL;
	}
#endif

	NTL_RETURNVOID();
}

RwBool CNtlPLGameProperty::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLGameProperty::Update");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLGameProperty::Render()
{
	NTL_FUNCTION("CNtlPLGameProperty::Render");

#ifdef dNTL_WORLD_TOOL_MODE
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		NTL_RETURN(TRUE);
	}

	if (dGET_CUR_PALETTE() != ePM_GAMEPROPERTY)
	{
		NTL_RETURN(TRUE);
	}

	RwRGBA rgbaColor;
	rgbaColor.alpha = 255; rgbaColor.red = 255; rgbaColor.green = 255; rgbaColor.blue = 255;
	if (s_pPLGamePropertySelected == this)
	{
		rgbaColor.red = 255; rgbaColor.green = 0; rgbaColor.blue = 0;
	}

	CNtlWorldFieldManager4RWWorld* pWFMgr = (CNtlWorldFieldManager4RWWorld*)GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	RwInt32 iBlockIdx = pWFMgr->GetWorldBlockManager()->GetBlockIdx(m_vPos);

	RwV3d	vCheck[4];
	vCheck[0].x = m_vPos.x - m_vSize.x;
	vCheck[0].y = m_vPos.y;
	vCheck[0].z = m_vPos.z - m_vSize.z;

	vCheck[1].x = m_vPos.x - m_vSize.x;
	vCheck[1].y = m_vPos.y;
	vCheck[1].z = m_vPos.z + m_vSize.z;

	vCheck[2].x = m_vPos.x + m_vSize.x;
	vCheck[2].y = m_vPos.y;
	vCheck[2].z = m_vPos.z + m_vSize.z;

	vCheck[3].x = m_vPos.x + m_vSize.x;
	vCheck[3].y = m_vPos.y;
	vCheck[3].z = m_vPos.z - m_vSize.z;

	for (int i = 0; i < 4; ++i)
	{
		if (iBlockIdx != pWFMgr->GetWorldBlockManager()->GetBlockIdx(vCheck[i]))
		{
			rgbaColor.red = 255; rgbaColor.green = 255; rgbaColor.blue = 0;
			break;
		}
	}

	BeginFogState(FALSE);
	BeginLightState(FALSE);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

	RwMatrix	mat;
	RwV3d		vPos = m_vPos; vPos.y += 0.1f;
	RwMatrixTranslate(&mat, &vPos, rwCOMBINEREPLACE);
	if (m_eShapeType == ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE)
	{
		RenderGeometrySpherePlane(&mat, &rgbaColor, m_vSize.x);
	}
	else if (m_eShapeType == ENTL_PL_GAME_PROPERTY_SHAPE_PLANE)
	{
		RenderGeometryQuadPlane(&mat, &rgbaColor, m_vSize.x, m_vSize.z);
	}
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	
	EndLightState();
	EndFogState();


#endif

	NTL_RETURN(TRUE);
}

#ifdef dNTL_WORLD_TOOL_MODE
RwBool CNtlPLGameProperty::Pick(RwReal& fDist)
{
	RwV2d ptPickScreen;
	ptPickScreen.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ptPickScreen.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

	RwLine rayCamera;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &rayCamera, &ptPickScreen);

	RwV3d vDirRay, vRayDeltha;
	RwV3dSubMacro(&vDirRay, &rayCamera.end, &rayCamera.start);
	RwV3dSubMacro(&vRayDeltha, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, &rayCamera.start);

	RwV3dNormalize(&vDirRay, &vDirRay);

	RwBool	bPicked	= FALSE;
	RwReal	fTemp	= 0.0f;

	RwV3d	vVertex[4];
	vVertex[0].x = m_vPos.x - m_vSize.x;
	vVertex[0].y = m_vPos.y;
	vVertex[0].z = m_vPos.z - m_vSize.z;

	vVertex[1].x = m_vPos.x - m_vSize.x;
	vVertex[1].y = m_vPos.y;
	vVertex[1].z = m_vPos.z + m_vSize.z;

	vVertex[2].x = m_vPos.x + m_vSize.x;
	vVertex[2].y = m_vPos.y;
	vVertex[2].z = m_vPos.z + m_vSize.z;

	vVertex[3].x = m_vPos.x + m_vSize.x;
	vVertex[3].y = m_vPos.y;
	vVertex[3].z = m_vPos.z - m_vSize.z;

	if(RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &vVertex[0], &vVertex[1], &vVertex[2], &fTemp))
	{
		bPicked = TRUE;
	}
	else if (RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &vVertex[0], &vVertex[2], &vVertex[3], &fTemp))
	{
		bPicked = TRUE;
	}
	else if (RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &vVertex[2], &vVertex[1], &vVertex[0], &fTemp))
	{
		bPicked = TRUE;
	}
	else if (RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &vVertex[3], &vVertex[2], &vVertex[0], &fTemp))
	{
		bPicked = TRUE;
	}
	
	if (bPicked && m_eShapeType == ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE)
	{
		RwV3d vDist = rayCamera.start + (vRayDeltha * fTemp) - m_vPos;
		if (m_vSize.x < RwV3dLength(&vDist))
		{
			bPicked = FALSE;
		}
	}

	if (bPicked)
	{
		fDist = fTemp;
	}
	return bPicked;
}
#endif