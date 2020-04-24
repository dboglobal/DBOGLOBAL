#include "precomp_ntlpresentation.h"

#include "NtlMath.h"
#include "NtlDebug.h"

#include "NtlWorldSector.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"

#include "NtlPLSceneManager.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLHeatHaze.h"

#include "NtlPLRenderState.h"
#include "NtlPLWorldState.h"

#include "NtlWorldPaletteDefine.h"

CNtlPLHeatHaze::CNtlPLHeatHaze(RwInt32 _ScrWidth, RwInt32 _ScrHeight)
{
	m_pVertices			= NULL;
	m_pIndices			= NULL;
	m_VertexGridDensity = 30;
	m_AccumulatedTime	= 0;
	m_EnableFlag		= TRUE;
	m_ScrWidth			= static_cast<RwReal>(_ScrWidth);
	m_ScrHeight			= static_cast<RwReal>(_ScrHeight);

	m_bUpdateInterpolation = FALSE;

#ifdef dNTL_WORLD_TOOL_MODE
	m_pToolHeatHaze		= NULL;
#endif
	m_pObjHeatHaze		= NULL;

	Create();
}

CNtlPLHeatHaze::~CNtlPLHeatHaze(void)
{
	Destroy();
}

void CNtlPLHeatHaze::ReleaseHandler()
{
	Destroy();
}

void CNtlPLHeatHaze::RestoreHandler(RwCamera* _pRenderTargetCam)
{
	Create();
}

RwBool CNtlPLHeatHaze::Create()
{
	if(m_pVertices && m_pIndices)
	{
		Destroy();
	}

	m_pVertices =	NTL_NEW RwIm2DVertex [m_VertexGridDensity * m_VertexGridDensity];
	m_pIndices	=	NTL_NEW RwImVertexIndex [(m_VertexGridDensity - 1) * (m_VertexGridDensity - 1) * 6];

	RwImVertexIndex* pIndices = m_pIndices;

	for(RwInt32 x = 0; x < m_VertexGridDensity - 1; x++) 
	{
		for(RwInt32 y = 0; y < m_VertexGridDensity - 1; y++) 
		{
			*pIndices = (y * m_VertexGridDensity + x);
			*pIndices++;
			*pIndices = (( y + 1) * m_VertexGridDensity + x);
			*pIndices++;
			*pIndices = (y * m_VertexGridDensity + x + 1);
			*pIndices++;

			*pIndices = (( y + 1 ) * m_VertexGridDensity + x);
			*pIndices++;
			*pIndices = (( y + 1 ) * m_VertexGridDensity + x + 1);
			*pIndices++;
			*pIndices = (y * m_VertexGridDensity + x + 1);
			*pIndices++;
		}
	}

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);

	return TRUE;
}

void CNtlPLHeatHaze::HandleEvents(RWS::CMsg &pMsg)
{	
	if (CheckEventsWorldEffectChanged(pMsg))
	{
		sNTL_FIELD_PROP* pNtlFieldProp = reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);

		m_FieldHeatHaze			= pNtlFieldProp->_NtlHeatHaze;
		m_bUpdateInterpolation	= Interpolation(m_FieldHeatHaze);
	}
}

void CNtlPLHeatHaze::ResizeGrid(RwInt32 iNum)
{
	Destroy();

	m_VertexGridDensity = iNum;

	Create();
}

void CNtlPLHeatHaze::Destroy( void ) 
{
	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

	if(m_pVertices)
	{
		NTL_ARRAY_DELETE(m_pVertices);
	}

	if(m_pIndices)
	{
		NTL_ARRAY_DELETE(m_pIndices);
	}
}


RpWorldSector* GetFirstHeatHazeObject(RpWorldSector *pWorldSector, void *data)
{	
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
		if (pNtlSector && pNtlSector->pNtlWorldSector)
		{
			sNTL_HEATHAZE** pOut = static_cast<sNTL_HEATHAZE**>(data);
			for (int i = 0; i < (int)pNtlSector->pNtlWorldSector->m_vecNtlPLEntityHeatHaze.size(); ++i)
			{
				CNtlPLHeatHazeObject* pHeatHaze = static_cast<CNtlPLHeatHazeObject*>(pNtlSector->pNtlWorldSector->m_vecNtlPLEntityHeatHaze[i]);
				if (pHeatHaze->FrustumTest())
				{
					*pOut = pHeatHaze->HeatHazeData();
					return NULL;
				}
			}
		}
	}
	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		CNtlWorldFieldManager4RWWorld*	pMgr	= (CNtlWorldFieldManager4RWWorld*)GetSceneManager()->GetWorld()->GetWorldFieldMgr();
		sNTL_HEATHAZE**					pOut	= static_cast<sNTL_HEATHAZE**>(data);
		for (int j = 0; j < (int)pMgr->GetRenderSector()->size(); ++j)
		{
			CNtlWorldSector* pNtlWorldSector = pMgr->GetRenderSector()->at(j);

			for (int i = 0; i < (int)pNtlWorldSector->m_vecNtlPLEntityHeatHaze.size(); ++i)
			{
				CNtlPLHeatHazeObject* pHeatHaze = static_cast<CNtlPLHeatHazeObject*>(pNtlWorldSector->m_vecNtlPLEntityHeatHaze[i]);
				if (pHeatHaze->FrustumTest())
				{
					*pOut = pHeatHaze->HeatHazeData();
					return NULL;
				}
			}
		}
		return NULL;
	}
	return pWorldSector;
}

RwBool CNtlPLHeatHaze::Update(RwReal fElapsedTime, RwCamera* _pRenderTargetCam) 
{
	sNTL_HEATHAZE* pCurHeatHaze = NULL; 

	RwCameraForAllSectorsInFrustum(CNtlPLGlobal::m_RwCamera, GetFirstHeatHazeObject, &pCurHeatHaze);	
	if (pCurHeatHaze)
	{
		m_pObjHeatHaze = pCurHeatHaze;

		m_bUpdateInterpolation = Interpolation(*m_pObjHeatHaze);
	}
	else if (m_pObjHeatHaze)
	{
		m_pObjHeatHaze = NULL;

		m_bUpdateInterpolation = Interpolation(m_FieldHeatHaze);
	}
	
	pCurHeatHaze = &m_CurHeatHaze;
	if (m_bUpdateInterpolation)
	{
		m_bUpdateInterpolation = UpdateInterpolation(fElapsedTime);
	}

#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pToolHeatHaze)
	{
		pCurHeatHaze = m_pToolHeatHaze;
	}
#endif

	if (pCurHeatHaze && (pCurHeatHaze->_HeatHazeExistence || m_bUpdateInterpolation))
	{
		RwIm2DVertex	TmpVert;
		RwIm2DVertex*	pCurVert		= m_pVertices;
		RwReal			line			= 2.0f;
		RwReal			factor			= 0.0f;
		RwReal			pi				= 45.0f;
		RwReal			increase		= 1.0f;
		RwReal			recipCameraZ	= 1.0f / RwCameraGetNearClipPlane(_pRenderTargetCam);
		RwReal			ExtentResult;

		// 0.07f
		m_AccumulatedTime += fElapsedTime * pCurHeatHaze->_MetersPerSec;

		for(int y = 0; y < m_VertexGridDensity; y++) 
		{
			ExtentResult = pCurHeatHaze->_Extent + NtlRandomNumber(-pCurHeatHaze->_Extent_Rand, pCurHeatHaze->_Extent_Rand);
			for(int x = 0; x < m_VertexGridDensity; x++) 
			{
				// pi : m_VertexGridDensity = x : y
				factor = pi * y / m_VertexGridDensity;

				RwIm2DVertexSetScreenX(&TmpVert, x * ((RwReal)m_ScrWidth / (m_VertexGridDensity - 1)));
				RwIm2DVertexSetScreenY(&TmpVert, y * ((RwReal)m_ScrHeight / (m_VertexGridDensity - 1)));
 				RwIm2DVertexSetScreenZ(&TmpVert, RwIm2DGetNearScreenZ());
 				RwIm2DVertexSetRecipCameraZ(&TmpVert, recipCameraZ);
				RwIm2DVertexSetU(&TmpVert, ((RwReal)x) / (RwReal)(m_VertexGridDensity - 1), recipCameraZ);
				RwIm2DVertexSetV(&TmpVert, ((RwReal)y) / (RwReal)(m_VertexGridDensity - 1), recipCameraZ);
				RwIm2DVertexSetIntRGBA(&TmpVert, 255, 255, 255, 255);

 				if(x != 0 && x != m_VertexGridDensity - 1/* && y != 0 && y != m_VertexGridDensity - 1*/)
 				{
 					RwReal tU = (cosf(factor)) *(((m_VertexGridDensity - 1) - y) * ExtentResult) * cosf(m_AccumulatedTime * ((m_VertexGridDensity - 1) - y)) * ((m_VertexGridDensity - 1) - y) * pCurHeatHaze->_Scale * _pRenderTargetCam->recipViewWindow.x;
 					RwReal tV = TmpVert.v;
					TmpVert.x += tU;
 					TmpVert.y += tV;
 				}

				*pCurVert++ = TmpVert;
			}
		}
		return TRUE;
	}

	/*RwSphere sphereTarget;

	RwV3d&	vTargetPos = sphereTarget.center; vTargetPos.x = 4589.0f; vTargetPos.y = -38.0f; vTargetPos.z = 4076.0f;
	RwReal& fTargetRad = sphereTarget.radius; fTargetRad = 100.0f;

	RwLine	ray;
	RwV2d	vCur; vCur.x = vCur.y = 0.0f;
	RwV3d	vDirRay;

	for(int y = 0; y < m_VertexGridDensity; y++) 
	{
		for(int x = 0; x < m_VertexGridDensity; x++) 
		{
			vCur.x = x * (static_cast<RwReal>(m_ScrWidth) / static_cast<RwReal>(m_VertexGridDensity - 1));
			vCur.y = y * (static_cast<RwReal>(m_ScrHeight) / static_cast<RwReal>(m_VertexGridDensity - 1));
			RwIm2DVertexSetScreenX(&TmpVert, vCur.x);
			RwIm2DVertexSetScreenY(&TmpVert, vCur.y);
			RwIm2DVertexSetScreenZ(&TmpVert, RwIm2DGetNearScreenZ());
			RwIm2DVertexSetRecipCameraZ(&TmpVert, 0.0f);
			RwIm2DVertexSetU(&TmpVert, static_cast<RwReal>(x) / static_cast<RwReal>(m_VertexGridDensity - 1), recipCameraZ);
			RwIm2DVertexSetV(&TmpVert, static_cast<RwReal>(y) / static_cast<RwReal>(m_VertexGridDensity - 1), recipCameraZ);
			RwIm2DVertexSetIntRGBA(&TmpVert, 255, 255, 255, 255);

			
			RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &ray, &vCur);

			RwV3dSubMacro(&vDirRay, &ray.end, &ray.start);

			RwV3dNormalize(&vDirRay, &vDirRay);

			RwV3d	vLength;
			RwReal	fLength;
			vLength = vTargetPos - ray.start;
			vLength = (vLength - (RwV3dDotProduct(&vDirRay, &vLength) * vDirRay));
			fLength = RwV3dDotProduct(&vLength, &vLength);

			if(x != 0 && x != m_VertexGridDensity - 1 && y != 0 && y != m_VertexGridDensity - 1)
			{
				if (fLength <= fTargetRad)
				{
					// 방향 * 크기 * 속도
					RwReal tU = ((fTargetRad - fLength) / fTargetRad * 4.0f) * cosf(m_AccumulatedTime * static_cast<RwReal>(y));
					RwReal tV = ((fTargetRad - fLength) / fTargetRad * 4.0f) * cosf(m_AccumulatedTime * static_cast<RwReal>(x));
					TmpVert.x += tU;
					TmpVert.y += tV;
				}
			}

			*pCurVert++ = TmpVert;
		}
	}

	*/

	

	return FALSE;
}

RwBool CNtlPLHeatHaze::Render(RwTexture* _pCurSceneTex, RwCamera* _pRWCam)
{
	static RwRGBA bgColor = {0, 0, 0, 0};
	if (RwCameraClear(_pRWCam, &bgColor, rwCAMERACLEARIMAGE))
	{
		if(RwCameraBeginUpdate(_pRWCam))
		{
			BeginFogState(FALSE);

			static RwInt32		iBreakIndex = 9999;

			RwInt32				iNumIndices = (m_VertexGridDensity - 1) * (m_VertexGridDensity - 1) * 2 * 3;
			RwImVertexIndex*	pIndices	= m_pIndices;

			RwD3D9SetTexture(_pCurSceneTex, 0);
			do 
			{
				if (iNumIndices >= iBreakIndex)
				{
					RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST,
						m_pVertices,
						m_VertexGridDensity * m_VertexGridDensity,
						pIndices,
						iBreakIndex);

					iNumIndices		-= iBreakIndex;
					pIndices		+= iBreakIndex;
				}
				else
				{
					RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST,
						m_pVertices,
						m_VertexGridDensity * m_VertexGridDensity,
						pIndices,
						iNumIndices);
					break;				
				}

			} while (iNumIndices > 0);
			RwD3D9SetTexture(NULL, 0);

			EndFogState();

			RwCameraEndUpdate(_pRWCam);
		}
	}

	return TRUE;
}

RwBool CNtlPLHeatHaze::Interpolation(const sNTL_HEATHAZE& sNtlHeatHaze)
{
	if (m_CurHeatHaze.Compare(sNtlHeatHaze) || (!m_CurHeatHaze._HeatHazeExistence && !sNtlHeatHaze._HeatHazeExistence))
	{
		return FALSE;
	}

	m_OldHeatHaze			= m_CurHeatHaze;
	if (sNtlHeatHaze._HeatHazeExistence)
	{
		m_NewHeatHaze		= sNtlHeatHaze;		
	}
	else
	{
		m_NewHeatHaze		= sNTL_HEATHAZE();
	}

	m_fInterpolationTime	= dNTL_PL_HEATHAZE_INTERPOLATION_TIME;

	return TRUE;
}

RwBool CNtlPLHeatHaze::UpdateInterpolation(RwReal fElapsed)
{
	m_fInterpolationTime -= fElapsed;
	if (m_fInterpolationTime < 0.0f)
	{
		m_CurHeatHaze = m_OldHeatHaze = m_NewHeatHaze;

		return FALSE;
	}
	
	RwReal fDelta = m_fInterpolationTime / dNTL_PL_HEATHAZE_INTERPOLATION_TIME;

	m_CurHeatHaze._MetersPerSec			= CNtlMath::Interpolation(m_NewHeatHaze._MetersPerSec, m_OldHeatHaze._MetersPerSec, fDelta);
	m_CurHeatHaze._Extent				= CNtlMath::Interpolation(m_NewHeatHaze._Extent, m_OldHeatHaze._Extent, fDelta);
	m_CurHeatHaze._Extent_Rand			= CNtlMath::Interpolation(m_NewHeatHaze._Extent_Rand, m_OldHeatHaze._Extent_Rand, fDelta);
	m_CurHeatHaze._Scale				= CNtlMath::Interpolation(m_NewHeatHaze._Scale, m_OldHeatHaze._Scale, fDelta);		

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLHeatHazeObject::CNtlPLHeatHazeObject()
{
#ifdef dNTL_WORLD_TOOL_MODE
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
#else
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOT_VISIBLE);
#endif

	SetLayer(PLENTITY_LAYER_HEATHAZE);
	SetClassType(PLENTITY_HEATHAZE);

	for (int i = 0; i < 4; ++i)
	{
		m_vVertex[i].x = 0.0f;
		m_vVertex[i].y = 0.0f;
		m_vVertex[i].z = 0.0f;
	}
	
	m_Sphere.center.x	= 0.0f;
	m_Sphere.center.y	= 0.0f;
	m_Sphere.center.z	= 0.0f;
	m_Sphere.radius		= 0.0f;
	m_fDistance			= 300.0f;

#ifdef dNTL_WORLD_TOOL_MODE
	m_HeatHazeObjectSwap.vScale.x	= 1.0f;
	m_HeatHazeObjectSwap.vScale.y	= 1.0f;
	m_HeatHazeObjectSwap.vScale.z	= 1.0f;
	m_HeatHazeObjectSwap.vRotate.x	= 0.0f;
	m_HeatHazeObjectSwap.vRotate.y	= 0.0f;
	m_HeatHazeObjectSwap.vRotate.z	= 0.0f;
	m_HeatHazeObjectSwap.vTrans.x	= 0.0f;
	m_HeatHazeObjectSwap.vTrans.y	= 0.0f;
	m_HeatHazeObjectSwap.vTrans.z	= 0.0f;

	m_bToolUI						= FALSE;
#endif
}

CNtlPLHeatHazeObject::~CNtlPLHeatHazeObject()
{
}

RwBool CNtlPLHeatHazeObject::Create(const SPLEntityCreateParam *pParam)
{
	NTL_FUNCTION("CNtlPLHeatHazeObject::Create");

#ifdef dNTL_WORLD_TOOL_MODE
	if (pParam->pPos)
	{
		SetPosition(pParam->pPos);
	}
#endif
	
	NTL_RETURN(TRUE);
}

void CNtlPLHeatHazeObject::Destroy()
{
	NTL_FUNCTION("CNtlPLHeatHazeObject::Destroy");

	NTL_RETURNVOID();
}

RwBool CNtlPLHeatHazeObject::Render()
{
	NTL_FUNCTION("CNtlPLHeatHazeObject::Render");

#ifdef dNTL_WORLD_TOOL_MODE	
	if (dGET_CUR_PALETTE() == ePM_HEATHAZE)
	{
		RwIm3DVertex aIm3DVertex[4];

		RwIm3DVertexSetPos(&aIm3DVertex[0], m_vVertex[0].x, m_vVertex[0].y, m_vVertex[0].z);
		RwIm3DVertexSetPos(&aIm3DVertex[1], m_vVertex[1].x, m_vVertex[1].y, m_vVertex[1].z);
		RwIm3DVertexSetPos(&aIm3DVertex[2], m_vVertex[3].x, m_vVertex[3].y, m_vVertex[3].z);
		RwIm3DVertexSetPos(&aIm3DVertex[3], m_vVertex[2].x, m_vVertex[2].y, m_vVertex[2].z);

		if (m_bToolUI)
		{
			RwIm3DVertexSetRGBA(&aIm3DVertex[0], 0, 0, 255, 255);
			RwIm3DVertexSetRGBA(&aIm3DVertex[1], 0, 0, 255, 255);
			RwIm3DVertexSetRGBA(&aIm3DVertex[2], 0, 0, 255, 255);
			RwIm3DVertexSetRGBA(&aIm3DVertex[3], 0, 0, 255, 255);
		}
		else
		{
			RwIm3DVertexSetRGBA(&aIm3DVertex[0], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&aIm3DVertex[1], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&aIm3DVertex[2], 255, 255, 255, 255);
			RwIm3DVertexSetRGBA(&aIm3DVertex[3], 255, 255, 255, 255);
		}

		RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);
		if (RwIm3DTransform(aIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
			RwIm3DEnd(); 
		}
		RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);
	}
#endif

	NTL_RETURN(TRUE);
}

RwBool CNtlPLHeatHazeObject::SetProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLHeatHazeObject::SetProperty");

	NTL_RETURN(TRUE);
}

void CNtlPLHeatHazeObject::CalcBoundingSphere()
{
	/*
	2 1
	3 0
	*/

	RwReal fLB2RT = RwV3dLength(&(m_vVertex[0] - m_vVertex[2]));
	RwReal fLT2RB = RwV3dLength(&(m_vVertex[1] - m_vVertex[3]));

	if (fLB2RT > fLT2RB)
	{
		m_Sphere.center = CNtlMath::Interpolation(m_vVertex[0], m_vVertex[2], 0.5f);
		m_Sphere.radius	= fLB2RT * 0.5f;
	}
	else
	{
		m_Sphere.center = CNtlMath::Interpolation(m_vVertex[1], m_vVertex[3], 0.5f);
		m_Sphere.radius	= fLT2RB * 0.5f;
	}
}

RwBool CNtlPLHeatHazeObject::FrustumTest()
{
	if (RpNtlCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &m_Sphere, &m_fDistance))
	{
		RwSphere Sphere;
		Sphere.radius = 0.0f;
		for (int i = 0; i < 4; ++i)
		{
			Sphere.center = m_vVertex[i];
			if (RpNtlCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &m_Sphere, &m_fDistance))
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_TOOL_MODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_TOOL_MODE

RwBool CNtlPLHeatHazeObject::Pick(RwReal* pfDist)
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

	RwBool bPicked	= FALSE;
	RwReal fTemp	= 0.0f;

	// 뒷면 앞면 모두 픽킹이 가능하다.
	if(RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &m_vVertex[0], &m_vVertex[1], &m_vVertex[2], &fTemp))
	{
		bPicked = TRUE;
	}
	else if (RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &m_vVertex[0], &m_vVertex[2], &m_vVertex[3], &fTemp))
	{
		bPicked = TRUE;
	}
	else if(RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &m_vVertex[2], &m_vVertex[1], &m_vVertex[0], &fTemp))
	{
		bPicked = TRUE;
	}
	else if (RtIntersectionLineTriangle(&rayCamera.start, &vRayDeltha, &m_vVertex[3], &m_vVertex[2], &m_vVertex[0], &fTemp))
	{
		bPicked = TRUE;
	}

	if(bPicked)
	{
		*pfDist = fTemp;
	}
	return bPicked;
}

void CNtlPLHeatHazeObject::CalcHeatHazeData()
{
	CLAMP(m_HeatHazeObjectSwap.vScale.x, 1.0f, 128.0f);
	CLAMP(m_HeatHazeObjectSwap.vScale.y, 1.0f, 128.0f);
	m_HeatHazeObjectSwap.vScale.z = 1.0f;

	RwMatrix		mat;
	RwMatrixScale(&mat, &m_HeatHazeObjectSwap.vScale, rwCOMBINEREPLACE);
	RwMatrixRotate(&mat, &CNtlPLGlobal::m_vYAxisV3, m_HeatHazeObjectSwap.vRotate.y, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&mat, &m_HeatHazeObjectSwap.vTrans, rwCOMBINEPOSTCONCAT);

	// Vertex Index :	2 1
	//					3 0 
	static RwV3d vVertex[4] = {
		{ -0.5f, -0.5f + 0.5f, 0.0f },
		{ -0.5f,  0.5f + 0.5f, 0.0f },
		{  0.5f,  0.5f + 0.5f, 0.0f },
		{  0.5f, -0.5f + 0.5f, 0.0f },
	};

	RwV3dTransformPoints(&m_vVertex[0], &vVertex[0], 4, &mat);

	CalcBoundingSphere();
}

void CNtlPLHeatHazeObject::SetToolUI(RwBool bToolUI)
{
	m_bToolUI = bToolUI;
	if (m_bToolUI)
	{
		CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->m_pToolHeatHaze = &m_HeatHaze;
	}
	else
	{
		if (CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->m_pToolHeatHaze == &m_HeatHaze)
		{
			CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->m_pToolHeatHaze = NULL;
		}
	}
}

void CNtlPLHeatHazeObject::SaveSwapFile(RwReal x, RwReal y, RwReal z)
{
	RwV3d	vSectorPos;

	vSectorPos.x = x;
	vSectorPos.y = y;
	vSectorPos.z = z;

	RwInt32	iSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vSectorPos);
	if (iSectorIdx != -1)
	{
		CNtlWorldSector* pSector = &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		if(_chdir("swap") == -1)
		{
			_mkdir("swap");
			_chdir("swap");
		}		

		if(_chdir("hho") == -1)
		{
			_mkdir("hho");
			_chdir("hho");
		}

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.hho", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iNum = static_cast<RwInt32>(pSector->m_vecNtlPLEntityHeatHaze.size());

		fwrite(&iNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLHeatHazeObject* pHeatHaze = static_cast<CNtlPLHeatHazeObject*>(pSector->m_vecNtlPLEntityHeatHaze.at(i)); 

			fwrite(&pHeatHaze->m_HeatHazeObjectSwap.vScale, sizeof(RwV3d), 1, pFile);
			fwrite(&pHeatHaze->m_HeatHazeObjectSwap.vRotate, sizeof(RwV3d), 1, pFile);
			fwrite(&pHeatHaze->m_HeatHazeObjectSwap.vTrans, sizeof(RwV3d), 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlPLHeatHazeObject::LoadSwapFile(RwReal x, RwReal y, RwReal z)
{
	RwV3d	vSectorPos;

	vSectorPos.x = x;
	vSectorPos.y = y;
	vSectorPos.z = z;

	RwInt32	iSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vSectorPos);
	if (iSectorIdx != -1)
	{
		CNtlWorldSector* pSector = &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("swap");
		_chdir("hho");

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.hho", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iNum;

		fread(&iNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLHeatHazeObject* pHeatHaze = static_cast<CNtlPLHeatHazeObject*>(pSector->m_vecNtlPLEntityHeatHaze.at(i)); 

			fread(&pHeatHaze->m_HeatHazeObjectSwap.vScale, sizeof(RwV3d), 1, pFile);
			fread(&pHeatHaze->m_HeatHazeObjectSwap.vRotate, sizeof(RwV3d), 1, pFile);
			fread(&pHeatHaze->m_HeatHazeObjectSwap.vTrans, sizeof(RwV3d), 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_TOOL_MODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool CNtlPLHeatHazeObject::LoadFromFile(FILE* pFile)
{
	for (RwInt32 i = 0; i < 4; ++i)
	{
		RwV3d vVertex;
		fread(&vVertex, sizeof(RwV3d), 1, pFile);
		SetVertex(i, &vVertex);
	}

	RwReal fDistance;
	fread(&fDistance, sizeof(RwReal), 1, pFile);
	SetDistance(fDistance);

	fread(&m_HeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);
	CalcBoundingSphere();

	return TRUE;
}

RwBool CNtlPLHeatHazeObject::SaveIntoFile(FILE* pFile)
{
	for (RwInt32 i = 0; i < 4; ++i)
	{
		RwV3d vTemp = *GetVertex(i);
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);
	}

	RwReal fDistance = GetDistance();
	fwrite(&fDistance, sizeof(RwReal), 1, pFile);

	fwrite(&m_HeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

	return TRUE;
}

RwInt32 CNtlPLHeatHazeObject::SkipToFile(FILE* pFile)
{
	RwInt32 SkipRange = sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwReal) + sizeof(sNTL_HEATHAZE);

	::fseek(pFile, SkipRange, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlPLHeatHazeObject::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	for (int i = 0; i < 4; ++i)
	{
		RwV3d vVertex;
		CopyMemory(&vVertex, pFileMem, sizeof(RwV3d));
		pFileMem += sizeof(RwV3d);
		if (pvOffset)
		{
			vVertex.x += pvOffset->x;
			vVertex.y += pvOffset->y;
			vVertex.z += pvOffset->z;
		}
		::fwrite(&vVertex, sizeof(RwV3d), 1, pFile);
	}

	RwReal fDistance;
	CopyMemory(&fDistance, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	::fwrite(&fDistance, sizeof(RwReal), 1, pFile);

	sNTL_HEATHAZE HeatHaze;
	CopyMemory(&HeatHaze, pFileMem, sizeof(sNTL_HEATHAZE));
	pFileMem += sizeof(sNTL_HEATHAZE);
	::fwrite(&HeatHaze, sizeof(sNTL_HEATHAZE), 1, pFile);

	return pFileMem;
}

BYTE* CNtlPLHeatHazeObject::SkipToFileMem(BYTE* pFileMem)
{
	for (int i = 0; i < 4; ++i)
	{
		pFileMem += sizeof(RwV3d);
	}

	pFileMem += sizeof(RwReal);

	pFileMem += sizeof(sNTL_HEATHAZE);

	return pFileMem;
}