#include "precomp_ntlpresentation.h"

#include "NtlWorldCommon.h"

#include "NtlPLApi.h"
#include "NtlPLRenderState.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlOccluderManager.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLOccluder_Quad.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE
CNtlPLOccluder_Quad::CNtlPLOccluder_Quad()
{
	m_pPlane = NTL_NEW CNtlPlane[EPLANE_OCCLUDER_NUM];

	for (int i = 0; i < EPLANE_OCCLUDER_NUM; ++i)
	{
		m_pPlane[i].Normalize();
	}

	for (int i = 0; i < 4; ++i)
	{
		m_vVertexOriginal[i].x	= 0.0f;
		m_vVertexOriginal[i].y	= 0.0f;
		m_vVertexOriginal[i].z 	= 0.0f;		

#ifdef dNTL_WORLD_TOOL_MODE
		m_sOccluderSwap.vTrans[i].x = 0.0f;
		m_sOccluderSwap.vTrans[i].y = 0.0f;
		m_sOccluderSwap.vTrans[i].z = 0.0f;
#endif
	}

	m_BoundingSphere.center.x	= 0.0f;
	m_BoundingSphere.center.y	= 0.0f;
	m_BoundingSphere.center.z	= 0.0f;
	m_BoundingSphere.radius		= 0.0f;

	sprintf_s(m_acTriggerName, 128, "");

#ifdef dNTL_WORLD_TOOL_MODE
	m_sOccluderSwap.vRotate.x	= 0.0f;
	m_sOccluderSwap.vRotate.y	= 0.0f;
	m_sOccluderSwap.vRotate.z	= 0.0f;
	m_sOccluderSwap.vScale.x	= 10.0f;
	m_sOccluderSwap.vScale.y	= 10.0f;
	m_sOccluderSwap.vScale.z	= 10.0f;

	m_bTriggerCheck				= FALSE;
	m_bTriggerRunable			= FALSE;	

	CalcOccluderData();
#endif
}

CNtlPLOccluder_Quad::~CNtlPLOccluder_Quad()
{
	NTL_ARRAY_DELETE(m_pPlane);
}

RwBool CNtlPLOccluder_Quad::Create(const SPLEntityCreateParam *pParam)
{
	NTL_FUNCTION("CNtlPLOccluder_Quad::Create");

	NTL_PRE(pParam);
	NTL_PRE(pParam->pPos);

	SetPosition(pParam->pPos);

	CNtlOccluderManager::GetInstance()->RegisterOccluder(this);

	NTL_RETURN(TRUE);
}

void CNtlPLOccluder_Quad::Destroy()
{
	NTL_FUNCTION("CNtlPLOccluder_Quad::Destroy");

	CNtlOccluderManager::GetInstance()->UnRegisterOccluder(this);

	NTL_RETURNVOID();
}

RwBool CNtlPLOccluder_Quad::Render()
{
	NTL_FUNCTION("CNtlPLOccluder_Quad::Render");

#ifdef dNTL_WORLD_TOOL_MODE	
	if (dGET_CUR_PALETTE() == ePM_PERFORMANCE_OCCLUSION)
	{
		for (int i = 0; i < 2; ++i)
		{
			RwIm3DVertex aIm3DVertex[4];		

			if (!i)
			{
				RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}

			if (!i || (m_bToolUI && m_iIndexPicked < 0))
			{
				RwIm3DVertexSetPos(&aIm3DVertex[0], m_vVertex[0].x, m_vVertex[0].y, m_vVertex[0].z);
				RwIm3DVertexSetPos(&aIm3DVertex[1], m_vVertex[1].x, m_vVertex[1].y, m_vVertex[1].z);
				RwIm3DVertexSetPos(&aIm3DVertex[2], m_vVertex[3].x, m_vVertex[3].y, m_vVertex[3].z);
				RwIm3DVertexSetPos(&aIm3DVertex[3], m_vVertex[2].x, m_vVertex[2].y, m_vVertex[2].z);

				RwIm3DVertexSetRGBA(&aIm3DVertex[0], 0, 0, 255, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[1], 0, 0, 255, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[2], 0, 0, 255, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[3], 0, 0, 255, 255);

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				if (RwIm3DTransform(aIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd(); 
				}

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLFRONT);

				RwIm3DVertexSetRGBA(&aIm3DVertex[0], 255, 255, 0, 255); 
				RwIm3DVertexSetRGBA(&aIm3DVertex[1], 255, 255, 0, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[2], 255, 255, 0, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[3], 255, 255, 0, 255);

				if (RwIm3DTransform(aIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd(); 
				}
			}

			RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

			if (!IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
			{
				for (int k = 0; k < 4; ++k)
				{
					if (!i || (m_bToolUI && m_iIndexPicked == k))
					{
						RwIm3DVertexSetRGBA(&aIm3DVertex[0], 255, 0, 0, 255);
						RwIm3DVertexSetRGBA(&aIm3DVertex[1], 255, 0, 0, 255);
						RwIm3DVertexSetRGBA(&aIm3DVertex[2], 255, 0, 0, 255);
						RwIm3DVertexSetRGBA(&aIm3DVertex[3], 255, 0, 0, 255);

						RwIm3DVertexSetPos(&aIm3DVertex[0], 0.0f, dPLOCCLUDER_POINT_RADIOS, 0.0f);
						RwIm3DVertexSetPos(&aIm3DVertex[1], 0.0f, 0.0f, dPLOCCLUDER_POINT_RADIOS);
						RwIm3DVertexSetPos(&aIm3DVertex[2], -dPLOCCLUDER_POINT_RADIOS, 0.0f, 0.0f);
						RwIm3DVertexSetPos(&aIm3DVertex[3], 0.0f, -dPLOCCLUDER_POINT_RADIOS, 0.0f);

						for (int j = 0; j < 4; ++j)
						{
							RwMatrix mat;
							RwMatrixRotate(&mat, &CNtlPLGlobal::m_vYAxisV3, 90.0f * (RwReal)j, rwCOMBINEREPLACE);
							RwMatrixTranslate(&mat, &m_vVertex[k], rwCOMBINEPOSTCONCAT);
							if (RwIm3DTransform(aIm3DVertex, 4, &mat, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
							{
								RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
								RwIm3DEnd(); 
							}
						}
					}
				}
			}

			RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);
			RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)FALSE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)rwBLENDINVSRCALPHA);

			if (!m_bToolUI)
			{
				break;
			}
		}
	}
#endif

	NTL_RETURN(TRUE);
}

void CNtlPLOccluder_Quad::SetTriggerName(const RwChar* pName)
{
	sprintf_s(m_acTriggerName, 128, "%s", pName);

	m_bTriggerRunable	= FALSE;
	m_bTriggerCheck		= (RwBool)strlen(m_acTriggerName);
}

void CNtlPLOccluder_Quad::RunableTrigger(const RwChar* pName)
{
	if (m_bTriggerCheck && pName)
	{
		m_bTriggerRunable = !(RwBool)strcmp(m_acTriggerName, pName);
	}
	else
	{
		m_bTriggerRunable = FALSE;
	}
}

RwBool CNtlPLOccluder_Quad::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLOccluder_Base::Update");

	switch (m_ePlaneType)
	{
	case EPLOCCLUDER_PLANE_ONESIDE:
		UpdateOneside(fElapsed);
		break;
	case EPLOCCLUDER_PLANE_TWOSIDE:
		UpdateTwoside(fElapsed);
		break;
	case EPLOCCLUDER_PLANE_BILLBOARD:
		UpdateBillboard(fElapsed);
		break;
	}

	m_bRunable = (!IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS)) && VisibilityTest() && (m_bTriggerCheck ? m_bTriggerRunable : TRUE);

	if (m_bRunable)
	{
		CNtlOccluderManager::GetInstance()->AddOccluderVisiblity(this);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLOccluder_Quad::UpdateOneside(RwReal fElapsed)
{
	RwV3d*	pvCameraPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	for (int i = 0; i < 4; ++i)
	{
		m_vVertex[i] = m_vPos + m_vVertexOriginal[i];
	}

	// 면을 구성한다.
	m_pPlane[EPLANE_OCCLUDER_RIGHT].PlaneFromPoints( pvCameraPos,  &m_vVertex[1], &m_vVertex[0]);	// Left
	m_pPlane[EPLANE_OCCLUDER_LEFT].PlaneFromPoints( pvCameraPos,  &m_vVertex[3], &m_vVertex[2]);	// Right	
	m_pPlane[EPLANE_OCCLUDER_TOP].PlaneFromPoints( pvCameraPos,  &m_vVertex[2], &m_vVertex[1]);		// Up
	m_pPlane[EPLANE_OCCLUDER_BOTTOM].PlaneFromPoints( pvCameraPos,  &m_vVertex[0], &m_vVertex[3]);	// Down
	m_pPlane[EPLANE_OCCLUDER_NEAR].PlaneFromPoints(&m_vVertex[0], &m_vVertex[2], &m_vVertex[1]);	// Front

	return TRUE;
}

RwBool CNtlPLOccluder_Quad::UpdateTwoside(RwReal fElapsed)
{
	RwV3d*	pvCameraPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	for (int i = 0; i < 4; ++i)
	{
		m_vVertex[i] = m_vPos + m_vVertexOriginal[i];
	}

	// 면을 구성한다.
	m_pPlane[EPLANE_OCCLUDER_RIGHT].PlaneFromPoints( pvCameraPos,  &m_vVertex[1], &m_vVertex[0]);	// Left
	m_pPlane[EPLANE_OCCLUDER_LEFT].PlaneFromPoints( pvCameraPos,  &m_vVertex[3], &m_vVertex[2]);	// Right	
	m_pPlane[EPLANE_OCCLUDER_TOP].PlaneFromPoints( pvCameraPos,  &m_vVertex[2], &m_vVertex[1]);		// Up
	m_pPlane[EPLANE_OCCLUDER_BOTTOM].PlaneFromPoints( pvCameraPos,  &m_vVertex[0], &m_vVertex[3]);	// Down
	m_pPlane[EPLANE_OCCLUDER_NEAR].PlaneFromPoints(&m_vVertex[0], &m_vVertex[2], &m_vVertex[1]);	// Front

	RwV3d* pvCameraAt = RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	if (RwV3dDotProduct(&m_pPlane[EPLANE_OCCLUDER_NEAR].GetNormal(), pvCameraAt/*&vDir*/) <= 0)
	{
		for (int i = 0; i < EPLANE_OCCLUDER_NUM; ++i)
		{
			m_pPlane[i].GetNormal()		*= -1.0f;
			m_pPlane[i].GetConstant()	*= -1.0f;
		}
	}
	// 	RwV3d	vDir;
	// 
	// 	vDir.x = pvCameraPos->x - m_vPos.x;
	// 	vDir.y = pvCameraPos->y - m_vPos.y;
	// 	vDir.z = pvCameraPos->z - m_vPos.z;
	// 
	// 	// 현재 양면이기 때문에 면이 뒤쪽이라면 방향을 바꾸어 준다.
	// 	if (RwV3dDotProduct(&m_pPlane[3].GetNormal(), &vDir) >= 0)
	// 	{
	// 		for (int i = 0; i < EPLANE_OCCLUDER_NUM; ++i)
	// 		{
	// 			m_pPlane[i].GetNormal()		*= -1.0f;
	// 			m_pPlane[i].GetConstant()	*= -1.0f;
	// 		}
	// 	}

	return TRUE;
}

RwBool CNtlPLOccluder_Quad::UpdateBillboard(RwReal fElapsed)
{
	RwV3d*	pvCameraPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3d	vDir;

	vDir.x = m_vPos.x - pvCameraPos->x;
	vDir.y = 0.0f;
	vDir.z = m_vPos.z - pvCameraPos->z;

	RwV3dNormalize(&vDir, &vDir);

	RwMatrix mat;

	// 빌보드 매트릭스를 연산한다.
	CNtlMath::MathGetRotationMatrix(&mat, &CNtlPLGlobal::m_vZAxisV3, &vDir);
	*RwMatrixGetPos(&mat) = m_vPos;

	RwV3dTransformPoints(&m_vVertex[0], &m_vVertexOriginal[0], 4, &mat);

	// 면을 구성한다.
	m_pPlane[EPLANE_OCCLUDER_RIGHT].PlaneFromPoints( pvCameraPos,  &m_vVertex[1], &m_vVertex[0]);	// Left
	m_pPlane[EPLANE_OCCLUDER_LEFT].PlaneFromPoints( pvCameraPos,  &m_vVertex[3], &m_vVertex[2]);	// Right	
	m_pPlane[EPLANE_OCCLUDER_TOP].PlaneFromPoints( pvCameraPos,  &m_vVertex[2], &m_vVertex[1]);		// Up
	m_pPlane[EPLANE_OCCLUDER_BOTTOM].PlaneFromPoints( pvCameraPos,  &m_vVertex[0], &m_vVertex[3]);	// Down
	m_pPlane[EPLANE_OCCLUDER_NEAR].PlaneFromPoints(&m_vVertex[0], &m_vVertex[2], &m_vVertex[1]);	// Front

	return TRUE;
}

RwBool CNtlPLOccluder_Quad::VisibilityTest()
{
	if (RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &m_BoundingSphere))
	{
		CNtlPlane	nearPlane;
		RwV3d		vPlanePos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
		RwV3d		vPlaneDir = *RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

		if (RwV3dDotProduct(&m_vPos, &CNtlPLGlobal::m_RwCamera->frustumPlanes[1].plane.normal) - CNtlPLGlobal::m_RwCamera->frustumPlanes[1].plane.distance >= 0.0f)
		{
			return FALSE;
		}

		if (RwV3dDotProduct(&m_pPlane[EPLANE_OCCLUDER_NEAR].GetNormal(), &(vPlanePos - m_vPos)) >= 0.0f)
		{
			return FALSE;
		}



		RwReal fDist = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&vPlanePos);
		if (fDist > 0.0f)
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

RwBool CNtlPLOccluder_Quad::OccluderTestSphere(RwSphere* pSphere)
{
	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PORTAL))
	{
		RwReal fDist;		

		fDist = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return FALSE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return TRUE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return TRUE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return TRUE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return TRUE;
		}

		return FALSE;
	}
	else
	{
		RwReal fDist;

		fDist = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return FALSE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pSphere->center);
		if (fDist <= pSphere->radius)
		{
			return FALSE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pSphere->center);
		if (fDist <= pSphere->radius)
		{
			return FALSE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pSphere->center);
		if (fDist <= pSphere->radius)
		{
			return FALSE;
		}

		// 바닥 면은 Portal이 아닌 경우 굳이 할 필요는 없다. Occluder는 공중에 뛰어서 배치 하지 않는다.
		// 		fDist = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pSphere->center);
		// 		if (fDist <= pSphere->radius)
		// 		{
		// 			return FALSE;
		// 		}
		return TRUE;
	}
	return FALSE;
}

RwBool CNtlPLOccluder_Quad::OccluderTestBox(RwBBox* pBox)
{
	// 정확하지 않다. 정확한 판단을 하기 위해서 연산이 추가된다.
	// 하지만 모든 점이 Occluder Frustum 안에 없다면, Render 해야되기 때문에 정확하게 연산할
	// 필요는 없을 것 같다.
	if (pBox->inf.x == pBox->sup.x && pBox->inf.y == pBox->sup.y && pBox->inf.z == pBox->sup.z)
	{
		return FALSE;
	}

	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PORTAL))
	{
		RwReal fDistInf;
		RwReal fDistSup;

		fDistInf = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		return FALSE;
	}
	else
	{
		RwReal fDistInf;
		RwReal fDistSup;

		fDistInf = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pBox->sup);
		if (fDistInf <= 0 || fDistSup <= 0)
		{
			return FALSE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pBox->sup);
		if (fDistInf <= 0 || fDistSup <= 0)
		{
			return FALSE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pBox->sup);
		if (fDistInf <= 0 || fDistSup <= 0)
		{
			return FALSE;
		}

		// 		fDistInf = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pBox->inf);
		// 		fDistSup = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pBox->sup);
		// 		if (fDistInf <= 0 || fDistSup <= 0)
		// 		{
		// 			return FALSE;
		// 		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pBox->sup);
		if (fDistInf <= 0 || fDistSup <= 0)
		{
			return FALSE;
		}

		return TRUE;
	}
	return FALSE;
}

void CNtlPLOccluder_Quad::CalcBoundingSphere()
{
	/*
	2 1
	3 0
	*/

	RwReal fLB2RT = RwV3dLength(&(m_vVertexOriginal[0] - m_vVertexOriginal[2]));
	RwReal fLT2RB = RwV3dLength(&(m_vVertexOriginal[1] - m_vVertexOriginal[3]));

	if (fLB2RT > fLT2RB)
	{
		m_BoundingSphere.center = CNtlMath::Interpolation(m_vVertexOriginal[0], m_vVertexOriginal[2], 0.5f) + m_vPos;
		m_BoundingSphere.radius	= fLB2RT * 0.5f;
	}
	else
	{
		m_BoundingSphere.center = CNtlMath::Interpolation(m_vVertexOriginal[1], m_vVertexOriginal[3], 0.5f) + m_vPos;
		m_BoundingSphere.radius	= fLT2RB * 0.5f;
	}
}

RwBool CNtlPLOccluder_Quad::Pick(RwReal* pfDist, RwInt32* piIndex)
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

	// 버텍스를 선택 했는지 여부를 확인 한다.
	if (piIndex)
	{
		*piIndex = -1;
		for (int i = 0; i < 4; ++i)
		{
			RwV3d	vLength = m_vVertex[i] - rayCamera.start;
			RwReal	fLength = RwV3dLength(&(vLength - (RwV3dDotProduct(&vDirRay, &vLength) * vDirRay)));
			if (fLength <= dPLOCCLUDER_POINT_RADIOS)
			{
				bPicked		= TRUE;
				*pfDist		= RwV3dLength(&vLength);
				*piIndex	= i;
				break;
			}
		}
	}

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

RwBool CNtlPLOccluder_Quad::PVSTest(RwV3d* pvStart, RwV3d* pvEnd)
{
	RwV3d vVertex[4];

	if (m_ePlaneType == EPLOCCLUDER_PLANE_BILLBOARD)
	{
		RwV3d vDir;

		vDir.x = m_vPos.x - pvStart->x;
		vDir.y = 0.0f;
		vDir.z = m_vPos.z - pvStart->z;

		RwV3dNormalize(&vDir, &vDir);

		RwMatrix mat;
		CNtlMath::MathGetRotationMatrix(&mat, &CNtlPLGlobal::m_vZAxisV3, &vDir);
		*RwMatrixGetPos(&mat) = m_vPos;

		RwV3dTransformPoints(&vVertex[0], &m_vVertexOriginal[0], 4, &mat);
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			vVertex[i] = m_vVertexOriginal[i] + m_vPos;
		}		
	}

	// intersection check를 위해서 y 값을 조정한다.
	vVertex[0].y = -2048.0f;
	vVertex[3].y = -2048.0f;

	RwReal	fDist;
	RwV3d	vRayDeltha;
	RwV3dSubMacro(&vRayDeltha, pvEnd, pvStart);

	if(RtIntersectionLineTriangle(pvStart, &vRayDeltha, &vVertex[0], &vVertex[1], &vVertex[2], &fDist))
	{
		return TRUE;
	}
	else if (RtIntersectionLineTriangle(pvStart, &vRayDeltha, &vVertex[0], &vVertex[2], &vVertex[3], &fDist))
	{
		return TRUE;
	}

	// 양면일 경우는 뒷면도 확인해야 한다.
	if (m_ePlaneType == EPLOCCLUDER_PLANE_TWOSIDE)
	{
		if(RtIntersectionLineTriangle(pvStart, &vRayDeltha, &vVertex[2], &vVertex[1], &vVertex[0], &fDist))
		{
			return TRUE;
		}
		else if(RtIntersectionLineTriangle(pvStart, &vRayDeltha, &vVertex[3], &vVertex[2], &vVertex[0], &fDist))
		{
			return TRUE;
		}
	}

	return FALSE;
}

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlPLOccluder_Quad::CalcOccluderData()
{
	RwMatrix		mat;

	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
	{
		// PVS 용 더미 오클루더 일 경우. Scale은 384.0f 가 Max 이다.
		CLAMP(m_sOccluderSwap.vScale.x, 1.0f, 384.0f);
		CLAMP(m_sOccluderSwap.vScale.y, 1.0f, 384.0f);
		m_sOccluderSwap.vScale.z = 1.0f;
	}
	else
	{
		// 일반(사각형) 오클루더 일 경우. Scale은 512.0f 가 Max 이다.
		CLAMP(m_sOccluderSwap.vScale.x, 1.0f, 512.0f);
		CLAMP(m_sOccluderSwap.vScale.y, 1.0f, 512.0f);
		m_sOccluderSwap.vScale.z = 1.0f;
	}

	RwMatrixScale(&mat, &m_sOccluderSwap.vScale, rwCOMBINEREPLACE);

	// 빌보드 일때는 vRotate가 적용 되지 않는다.
	if (GetOccluderPlaneType() != EPLOCCLUDER_PLANE_BILLBOARD)
	{
		RwMatrixRotate(&mat, &CNtlPLGlobal::m_vYAxisV3, m_sOccluderSwap.vRotate.y, rwCOMBINEPOSTCONCAT);
	}

	// Vertex Index :	2 1 : 5 Datum
	//					3 0 
	static RwV3d vVertex[4] = {
		{ -0.5f, -0.5f + dPL_OCCLUDER_OFFSET_Y, 0.0f },
		{ -0.5f,  0.5f + dPL_OCCLUDER_OFFSET_Y, 0.0f },
		{  0.5f,  0.5f + dPL_OCCLUDER_OFFSET_Y, 0.0f },
		{  0.5f, -0.5f + dPL_OCCLUDER_OFFSET_Y, 0.0f },
	};

	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
	{
		RwV3dTransformPoints(&m_vVertexOriginal[0], &vVertex[0], 4, &mat);
	}
	else
	{
		RwV3d vTemp[4];
		for (int i = 0; i < 4; ++i)
		{
			RwV3dAdd(&vTemp[i], &vVertex[i], &m_sOccluderSwap.vTrans[i]);
		}

		RwV3dTransformPoints(&m_vVertexOriginal[0], &vTemp[0], 4, &mat);

		// 일반(사각형) 오클루더 일 경우. Vertex Trans를 지원하기 때문에. Max(512.0f)를 넘었는지 확인하고 교정한다.
		for (int i = 0; i < 4; ++i)
		{
			RwV3d	vDir;
			RwReal	vBackupY;
			RwReal	fLength;

			vDir	= m_vVertexOriginal[i]; vDir.y = 0.0f;
			vBackupY= m_vVertexOriginal[i].y;
			fLength = RwV3dLength(&vDir);
			if (fLength > 256.0f)
			{
				RwV3dNormalize(&vDir, &vDir);
				RwV3dScale(&m_vVertexOriginal[i], &vDir, 256.0f);
				m_vVertexOriginal[i].y = vBackupY;
			}
		}
	}

	CalcBoundingSphere();
}

void CNtlPLOccluder_Quad::SaveSwapFile(RwReal x, RwReal y, RwReal z)
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

		if(_chdir("occ") == -1)
		{
			_mkdir("occ");
			_chdir("occ");
		}

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ocq", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iNum = static_cast<RwInt32>(pSector->m_vecNtlPLEntityOccluder_Quad.size());

		fwrite(&iNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLOccluder_Quad* pOccluder = static_cast<CNtlPLOccluder_Quad*>(pSector->m_vecNtlPLEntityOccluder_Quad.at(i));
			fwrite(&pOccluder->m_sOccluderSwap.vScale, sizeof(RwV3d), 1, pFile);
			fwrite(&pOccluder->m_sOccluderSwap.vRotate, sizeof(RwV3d), 1, pFile);
			fwrite(&pOccluder->m_sOccluderSwap.vTrans[0], sizeof(RwV3d)* 4, 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlPLOccluder_Quad::LoadSwapFile(RwReal x, RwReal y, RwReal z)
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
		_chdir("occ");

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ocq", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iNum;

		fread(&iNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iNum; ++i)
		{
			CNtlPLOccluder_Quad* pOccluder = static_cast<CNtlPLOccluder_Quad*>(pSector->m_vecNtlPLEntityOccluder_Quad.at(i));
			fread(&pOccluder->m_sOccluderSwap.vScale, sizeof(RwV3d), 1, pFile);
			fread(&pOccluder->m_sOccluderSwap.vRotate, sizeof(RwV3d), 1, pFile);
			fread(&pOccluder->m_sOccluderSwap.vTrans[0], sizeof(RwV3d)* 4, 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}
#endif

RwBool CNtlPLOccluder_Quad::LoadFromFile(FILE* pFile)
{
	RwInt32	iPlaneType;	
	fread(&iPlaneType, sizeof(RwInt32), 1, pFile);

	RwChar acTemp[128];	
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 iLength = 0;
		fread(&iLength, sizeof(RwUInt32), 1, pFile);
		fread(acTemp, sizeof(RwChar) * iLength, 1, pFile);
		acTemp[iLength] = '\0';
	}	

	for (int j = 0; j < 4; ++j)
	{
		RwV3d vVertex;
		fread(&vVertex, sizeof(RwV3d), 1, pFile);
		SetVertexOriginal(j, &vVertex);
	}

	SetOccluderPlaneType(iPlaneType);
	SetTriggerName(acTemp);
	CalcBoundingSphere();

	return TRUE;
}

RwBool CNtlPLOccluder_Quad::SaveIntoFile(FILE* pFile)
{
	RwInt32 iPlaneType = GetOccluderPlaneType();	
	fwrite(&iPlaneType, sizeof(RwInt32), 1, pFile);

	RwChar acTemp[128];
	strcpy_s(acTemp, 128, GetTriggerName());

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(acTemp, sizeof(RwChar) * 128, 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = strlen(acTemp);

		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(acTemp, sizeof(RwChar) * uiLength, 1, pFile);
	}

	for (RwInt32 i = 0; i < 4; ++i)
	{
		RwV3d vTemp = *GetVertexOriginal(i);
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);
	}

	return TRUE;
}

RwInt32 CNtlPLOccluder_Quad::SkipToFile(FILE* pFile)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		::fseek(pFile, sizeof(RwInt32), SEEK_CUR);

		RwUInt32 uiLength = 0;
		::fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		::fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);

		::fseek(pFile, sizeof(RwV3d) * 4, SEEK_CUR);
	}	

	return ::ftell(pFile);
}

BYTE* CNtlPLOccluder_Quad::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 iPlaneType;
	CopyMemory(&iPlaneType, pFileMem, sizeof(RwInt32)); // iPlaneType
	pFileMem += sizeof(RwInt32);
	::fwrite(&iPlaneType, sizeof(RwInt32), 1, pFile);

	RwChar acTemp[128];
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		CopyMemory(acTemp, pFileMem, sizeof(RwChar) * 128);
		::fwrite(acTemp, sizeof(RwChar) * 128, 1, pFile);
		pFileMem += sizeof(RwChar) * 128;

	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		::fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		CopyMemory(acTemp, pFileMem, sizeof(RwChar) * uiLength);
		acTemp[uiLength] = '\0';

		::fwrite(acTemp, sizeof(RwChar) * uiLength, 1, pFile);
		pFileMem += sizeof(RwChar) * uiLength;
	}

	for (int i = 0; i < 4; ++i)
	{
		RwV3d vTemp;
		CopyMemory(&vTemp, pFileMem, sizeof(RwV3d)); // vVertex
		pFileMem += sizeof(RwV3d);
		::fwrite(&vTemp, sizeof(RwV3d), 1, pFile);
	}

	return pFileMem;
}

BYTE* CNtlPLOccluder_Quad::SkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwInt32);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		pFileMem += sizeof(RwChar) * 128;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);
		pFileMem += sizeof(RwChar) * uiLength;
	}

	for (int i = 0; i < 4; ++i)
	{
		pFileMem += sizeof(RwV3d);
	}

	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

CNtlPLOccluder_Quad::CNtlPLOccluder_Quad()
{
	m_pPlane = NTL_NEW CNtlPlane[EPLANE_OCCLUDER_NUM];

	for (int i = 0; i < EPLANE_OCCLUDER_NUM; ++i)
	{
		m_pPlane[i].Normalize();
	}

	for (int i = 0; i < 4; ++i)
	{
		m_vVertexOriginal[i].x	= 0.0f;
		m_vVertexOriginal[i].y	= 0.0f;
		m_vVertexOriginal[i].z 	= 0.0f;		

#ifdef dNTL_WORLD_TOOL_MODE
		m_sOccluderSwap.vTrans[i].x = 0.0f;
		m_sOccluderSwap.vTrans[i].y = 0.0f;
		m_sOccluderSwap.vTrans[i].z = 0.0f;
#endif
	}

	m_BoundingSphere.center.x	= 0.0f;
	m_BoundingSphere.center.y	= 0.0f;
	m_BoundingSphere.center.z	= 0.0f;
	m_BoundingSphere.radius		= 0.0f;

	sprintf_s(m_acTriggerName, 128, "");

#ifdef dNTL_WORLD_TOOL_MODE
	m_sOccluderSwap.vRotate.x	= 0.0f;
	m_sOccluderSwap.vRotate.y	= 0.0f;
	m_sOccluderSwap.vRotate.z	= 0.0f;
	m_sOccluderSwap.vScale.x	= 10.0f;
	m_sOccluderSwap.vScale.y	= 10.0f;
	m_sOccluderSwap.vScale.z	= 10.0f;

	m_bTriggerCheck				= FALSE;
	m_bTriggerRunable			= FALSE;	

	CalcOccluderData();
#endif
}

CNtlPLOccluder_Quad::~CNtlPLOccluder_Quad()
{
	NTL_ARRAY_DELETE(m_pPlane);
}

RwBool CNtlPLOccluder_Quad::Create(const SPLEntityCreateParam *pParam)
{
	NTL_FUNCTION("CNtlPLOccluder_Quad::Create");

	NTL_PRE(pParam);
	NTL_PRE(pParam->pPos);

	SetPosition(pParam->pPos);

	CNtlOccluderManager::GetInstance()->RegisterOccluder(this);

	NTL_RETURN(TRUE);
}

void CNtlPLOccluder_Quad::Destroy()
{
	NTL_FUNCTION("CNtlPLOccluder_Quad::Destroy");

	CNtlOccluderManager::GetInstance()->UnRegisterOccluder(this);

	NTL_RETURNVOID();
}

RwBool CNtlPLOccluder_Quad::Render()
{
	NTL_FUNCTION("CNtlPLOccluder_Quad::Render");

#ifdef dNTL_WORLD_TOOL_MODE	
	if (dGET_CUR_PALETTE() == ePM_PERFORMANCE_OCCLUSION)
	{
 		for (int i = 0; i < 2; ++i)
		{
			RwIm3DVertex aIm3DVertex[4];		

			if (!i)
			{
				RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}

			if (!i || (m_bToolUI && m_iIndexPicked < 0))
			{
				RwIm3DVertexSetPos(&aIm3DVertex[0], m_vVertex[0].x, m_vVertex[0].y, m_vVertex[0].z);
				RwIm3DVertexSetPos(&aIm3DVertex[1], m_vVertex[1].x, m_vVertex[1].y, m_vVertex[1].z);
				RwIm3DVertexSetPos(&aIm3DVertex[2], m_vVertex[3].x, m_vVertex[3].y, m_vVertex[3].z);
				RwIm3DVertexSetPos(&aIm3DVertex[3], m_vVertex[2].x, m_vVertex[2].y, m_vVertex[2].z);

				RwIm3DVertexSetRGBA(&aIm3DVertex[0], 0, 0, 255, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[1], 0, 0, 255, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[2], 0, 0, 255, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[3], 0, 0, 255, 255);

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

				if (RwIm3DTransform(aIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd(); 
				}

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLFRONT);

				RwIm3DVertexSetRGBA(&aIm3DVertex[0], 255, 255, 0, 255); 
				RwIm3DVertexSetRGBA(&aIm3DVertex[1], 255, 255, 0, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[2], 255, 255, 0, 255);
				RwIm3DVertexSetRGBA(&aIm3DVertex[3], 255, 255, 0, 255);

				if (RwIm3DTransform(aIm3DVertex, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
				{
					RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
					RwIm3DEnd(); 
				}
			}
			
			RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

			if (!IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
			{
				for (int k = 0; k < 4; ++k)
				{
					if (!i || (m_bToolUI && m_iIndexPicked == k))
					{
						RwIm3DVertexSetRGBA(&aIm3DVertex[0], 255, 0, 0, 255);
						RwIm3DVertexSetRGBA(&aIm3DVertex[1], 255, 0, 0, 255);
						RwIm3DVertexSetRGBA(&aIm3DVertex[2], 255, 0, 0, 255);
						RwIm3DVertexSetRGBA(&aIm3DVertex[3], 255, 0, 0, 255);

						RwIm3DVertexSetPos(&aIm3DVertex[0], 0.0f, dPLOCCLUDER_POINT_RADIOS, 0.0f);
						RwIm3DVertexSetPos(&aIm3DVertex[1], 0.0f, 0.0f, dPLOCCLUDER_POINT_RADIOS);
						RwIm3DVertexSetPos(&aIm3DVertex[2], -dPLOCCLUDER_POINT_RADIOS, 0.0f, 0.0f);
						RwIm3DVertexSetPos(&aIm3DVertex[3], 0.0f, -dPLOCCLUDER_POINT_RADIOS, 0.0f);

						for (int j = 0; j < 4; ++j)
						{
							RwMatrix mat;
							RwMatrixRotate(&mat, &CNtlPLGlobal::m_vYAxisV3, 90.0f * (RwReal)j, rwCOMBINEREPLACE);
							RwMatrixTranslate(&mat, &m_vVertex[k], rwCOMBINEPOSTCONCAT);
							if (RwIm3DTransform(aIm3DVertex, 4, &mat, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
							{
								RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
								RwIm3DEnd(); 
							}
						}
					}
				}
			}

			RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);
			RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)FALSE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)rwBLENDINVSRCALPHA);

			if (!m_bToolUI)
			{
				break;
			}
		}
	}
#endif

	NTL_RETURN(TRUE);
}

void CNtlPLOccluder_Quad::SetTriggerName(const RwChar* pName)
{
	sprintf_s(m_acTriggerName, 128, "%s", pName);

	m_bTriggerRunable	= FALSE;
	m_bTriggerCheck		= (RwBool)strlen(m_acTriggerName);
}

void CNtlPLOccluder_Quad::RunableTrigger(const RwChar* pName)
{
	if (m_bTriggerCheck && pName)
	{
		m_bTriggerRunable = !(RwBool)strcmp(m_acTriggerName, pName);
	}
	else
	{
		m_bTriggerRunable = FALSE;
	}
}

RwBool CNtlPLOccluder_Quad::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLOccluder_Base::Update");

	switch (m_ePlaneType)
	{
	case EPLOCCLUDER_PLANE_ONESIDE:
		UpdateOneside(fElapsed);
		break;
	case EPLOCCLUDER_PLANE_TWOSIDE:
		UpdateTwoside(fElapsed);
		break;
	case EPLOCCLUDER_PLANE_BILLBOARD:
		UpdateBillboard(fElapsed);
		break;
	}

	m_bRunable = (!IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS)) && VisibilityTest() && (m_bTriggerCheck ? m_bTriggerRunable : TRUE);
	
	if (m_bRunable)
	{
		CNtlOccluderManager::GetInstance()->AddOccluderVisiblity(this);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLOccluder_Quad::UpdateOneside(RwReal fElapsed)
{
	RwV3d*	pvCameraPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	for (int i = 0; i < 4; ++i)
	{
		m_vVertex[i] = m_vPos + m_vVertexOriginal[i];
	}

	// 면을 구성한다.
	m_pPlane[EPLANE_OCCLUDER_RIGHT].PlaneFromPoints( pvCameraPos,  &m_vVertex[1], &m_vVertex[0]);	// Left
	m_pPlane[EPLANE_OCCLUDER_LEFT].PlaneFromPoints( pvCameraPos,  &m_vVertex[3], &m_vVertex[2]);	// Right	
	m_pPlane[EPLANE_OCCLUDER_TOP].PlaneFromPoints( pvCameraPos,  &m_vVertex[2], &m_vVertex[1]);		// Up
	m_pPlane[EPLANE_OCCLUDER_BOTTOM].PlaneFromPoints( pvCameraPos,  &m_vVertex[0], &m_vVertex[3]);	// Down
	m_pPlane[EPLANE_OCCLUDER_NEAR].PlaneFromPoints(&m_vVertex[0], &m_vVertex[2], &m_vVertex[1]);	// Front

 	return TRUE;
}

RwBool CNtlPLOccluder_Quad::UpdateTwoside(RwReal fElapsed)
{
 	RwV3d*	pvCameraPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	for (int i = 0; i < 4; ++i)
	{
		m_vVertex[i] = m_vPos + m_vVertexOriginal[i];
	}

	// 면을 구성한다.
	m_pPlane[EPLANE_OCCLUDER_RIGHT].PlaneFromPoints( pvCameraPos,  &m_vVertex[1], &m_vVertex[0]);	// Left
	m_pPlane[EPLANE_OCCLUDER_LEFT].PlaneFromPoints( pvCameraPos,  &m_vVertex[3], &m_vVertex[2]);	// Right	
	m_pPlane[EPLANE_OCCLUDER_TOP].PlaneFromPoints( pvCameraPos,  &m_vVertex[2], &m_vVertex[1]);		// Up
	m_pPlane[EPLANE_OCCLUDER_BOTTOM].PlaneFromPoints( pvCameraPos,  &m_vVertex[0], &m_vVertex[3]);	// Down
	m_pPlane[EPLANE_OCCLUDER_NEAR].PlaneFromPoints(&m_vVertex[0], &m_vVertex[2], &m_vVertex[1]);	// Front

	RwV3d* pvCameraAt = RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	if (RwV3dDotProduct(&m_pPlane[EPLANE_OCCLUDER_NEAR].GetNormal(), pvCameraAt/*&vDir*/) <= 0)
	{
		for (int i = 0; i < EPLANE_OCCLUDER_NUM; ++i)
		{
			m_pPlane[i].GetNormal()		*= -1.0f;
			m_pPlane[i].GetConstant()	*= -1.0f;
		}
	}
// 	RwV3d	vDir;
// 
// 	vDir.x = pvCameraPos->x - m_vPos.x;
// 	vDir.y = pvCameraPos->y - m_vPos.y;
// 	vDir.z = pvCameraPos->z - m_vPos.z;
// 
// 	// 현재 양면이기 때문에 면이 뒤쪽이라면 방향을 바꾸어 준다.
// 	if (RwV3dDotProduct(&m_pPlane[3].GetNormal(), &vDir) >= 0)
// 	{
// 		for (int i = 0; i < EPLANE_OCCLUDER_NUM; ++i)
// 		{
// 			m_pPlane[i].GetNormal()		*= -1.0f;
// 			m_pPlane[i].GetConstant()	*= -1.0f;
// 		}
// 	}

	return TRUE;
}

RwBool CNtlPLOccluder_Quad::UpdateBillboard(RwReal fElapsed)
{
 	RwV3d*	pvCameraPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3d	vDir;

	vDir.x = m_vPos.x - pvCameraPos->x;
	vDir.y = 0.0f;
	vDir.z = m_vPos.z - pvCameraPos->z;

	RwV3dNormalize(&vDir, &vDir);

	RwMatrix mat;

	// 빌보드 매트릭스를 연산한다.
	CNtlMath::MathGetRotationMatrix(&mat, &CNtlPLGlobal::m_vZAxisV3, &vDir);
	*RwMatrixGetPos(&mat) = m_vPos;

	RwV3dTransformPoints(&m_vVertex[0], &m_vVertexOriginal[0], 4, &mat);

	// 면을 구성한다.
	m_pPlane[EPLANE_OCCLUDER_RIGHT].PlaneFromPoints( pvCameraPos,  &m_vVertex[1], &m_vVertex[0]);	// Left
	m_pPlane[EPLANE_OCCLUDER_LEFT].PlaneFromPoints( pvCameraPos,  &m_vVertex[3], &m_vVertex[2]);	// Right	
	m_pPlane[EPLANE_OCCLUDER_TOP].PlaneFromPoints( pvCameraPos,  &m_vVertex[2], &m_vVertex[1]);		// Up
	m_pPlane[EPLANE_OCCLUDER_BOTTOM].PlaneFromPoints( pvCameraPos,  &m_vVertex[0], &m_vVertex[3]);	// Down
	m_pPlane[EPLANE_OCCLUDER_NEAR].PlaneFromPoints(&m_vVertex[0], &m_vVertex[2], &m_vVertex[1]);	// Front
	
	return TRUE;
}

RwBool CNtlPLOccluder_Quad::VisibilityTest()
{
	if (RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &m_BoundingSphere))
	{
		CNtlPlane	nearPlane;
		RwV3d		vPlanePos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
		RwV3d		vPlaneDir = *RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

		if (RwV3dDotProduct(&m_vPos, &CNtlPLGlobal::m_RwCamera->frustumPlanes[1].plane.normal) - CNtlPLGlobal::m_RwCamera->frustumPlanes[1].plane.distance >= 0.0f)
		{
			return FALSE;
		}

		if (RwV3dDotProduct(&m_pPlane[EPLANE_OCCLUDER_NEAR].GetNormal(), &(vPlanePos - m_vPos)) >= 0.0f)
		{
			return FALSE;
		}


		
		RwReal fDist = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&vPlanePos);
		if (fDist > 0.0f)
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

RwBool CNtlPLOccluder_Quad::OccluderTestSphere(RwSphere* pSphere)
{
	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PORTAL))
	{
		RwReal fDist;		

		fDist = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return FALSE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return TRUE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return TRUE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return TRUE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return TRUE;
		}

		return FALSE;
	}
	else
	{
		RwReal fDist;

		fDist = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pSphere->center);
		if (fDist <= -pSphere->radius)
		{
			return FALSE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pSphere->center);
		if (fDist <= pSphere->radius)
		{
			return FALSE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pSphere->center);
		if (fDist <= pSphere->radius)
		{
			return FALSE;
		}

		fDist = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pSphere->center);
		if (fDist <= pSphere->radius)
		{
			return FALSE;
		}

		// 바닥 면은 Portal이 아닌 경우 굳이 할 필요는 없다. Occluder는 공중에 뛰어서 배치 하지 않는다.
// 		fDist = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pSphere->center);
// 		if (fDist <= pSphere->radius)
// 		{
// 			return FALSE;
// 		}
		return TRUE;
	}
	return FALSE;
}

RwBool CNtlPLOccluder_Quad::OccluderTestBox(RwBBox* pBox)
{
	// 정확하지 않다. 정확한 판단을 하기 위해서 연산이 추가된다.
	// 하지만 모든 점이 Occluder Frustum 안에 없다면, Render 해야되기 때문에 정확하게 연산할
	// 필요는 없을 것 같다.
	if (pBox->inf.x == pBox->sup.x && pBox->inf.y == pBox->sup.y && pBox->inf.z == pBox->sup.z)
	{
		return FALSE;
	}

	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PORTAL))
	{
		RwReal fDistInf;
		RwReal fDistSup;

		fDistInf = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pBox->sup);
		if (fDistInf <= 0 && fDistSup <= 0)
		{
			return TRUE;
		}
		
		return FALSE;
	}
	else
	{
		RwReal fDistInf;
		RwReal fDistSup;
		
		fDistInf = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_LEFT].DotProduct(&pBox->sup);
		if (fDistInf <= 0 || fDistSup <= 0)
		{
			return FALSE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_RIGHT].DotProduct(&pBox->sup);
		if (fDistInf <= 0 || fDistSup <= 0)
		{
			return FALSE;
		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_TOP].DotProduct(&pBox->sup);
		if (fDistInf <= 0 || fDistSup <= 0)
		{
			return FALSE;
		}

// 		fDistInf = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pBox->inf);
// 		fDistSup = m_pPlane[EPLANE_OCCLUDER_BOTTOM].DotProduct(&pBox->sup);
// 		if (fDistInf <= 0 || fDistSup <= 0)
// 		{
// 			return FALSE;
// 		}

		fDistInf = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pBox->inf);
		fDistSup = m_pPlane[EPLANE_OCCLUDER_NEAR].DotProduct(&pBox->sup);
		if (fDistInf <= 0 || fDistSup <= 0)
		{
			return FALSE;
		}

		return TRUE;
	}
	return FALSE;
}

void CNtlPLOccluder_Quad::CalcBoundingSphere()
{
	/*
	2 1
	3 0
	*/

	RwReal fLB2RT = RwV3dLength(&(m_vVertexOriginal[0] - m_vVertexOriginal[2]));
	RwReal fLT2RB = RwV3dLength(&(m_vVertexOriginal[1] - m_vVertexOriginal[3]));

	if (fLB2RT > fLT2RB)
	{
		m_BoundingSphere.center = CNtlMath::Interpolation(m_vVertexOriginal[0], m_vVertexOriginal[2], 0.5f) + m_vPos;
		m_BoundingSphere.radius	= fLB2RT * 0.5f;
	}
	else
	{
		m_BoundingSphere.center = CNtlMath::Interpolation(m_vVertexOriginal[1], m_vVertexOriginal[3], 0.5f) + m_vPos;
		m_BoundingSphere.radius	= fLT2RB * 0.5f;
	}
}

RwBool CNtlPLOccluder_Quad::Pick(RwReal* pfDist, RwInt32* piIndex)
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

	// 버텍스를 선택 했는지 여부를 확인 한다.
	if (piIndex)
	{
		*piIndex = -1;
		for (int i = 0; i < 4; ++i)
		{
			RwV3d	vLength = m_vVertex[i] - rayCamera.start;
			RwReal	fLength = RwV3dLength(&(vLength - (RwV3dDotProduct(&vDirRay, &vLength) * vDirRay)));
			if (fLength <= dPLOCCLUDER_POINT_RADIOS)
			{
				bPicked		= TRUE;
				*pfDist		= RwV3dLength(&vLength);
				*piIndex	= i;
				break;
			}
		}
	}

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

RwBool CNtlPLOccluder_Quad::PVSTest(RwV3d* pvStart, RwV3d* pvEnd)
{
 	RwV3d vVertex[4];
	
	if (m_ePlaneType == EPLOCCLUDER_PLANE_BILLBOARD)
	{
		RwV3d vDir;

		vDir.x = m_vPos.x - pvStart->x;
		vDir.y = 0.0f;
		vDir.z = m_vPos.z - pvStart->z;

		RwV3dNormalize(&vDir, &vDir);

		RwMatrix mat;
		CNtlMath::MathGetRotationMatrix(&mat, &CNtlPLGlobal::m_vZAxisV3, &vDir);
		*RwMatrixGetPos(&mat) = m_vPos;

		RwV3dTransformPoints(&vVertex[0], &m_vVertexOriginal[0], 4, &mat);
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			vVertex[i] = m_vVertexOriginal[i] + m_vPos;
		}		
	}

	// intersection check를 위해서 y 값을 조정한다.
	vVertex[0].y = -2048.0f;
	vVertex[3].y = -2048.0f;
	
	RwReal	fDist;
	RwV3d	vRayDeltha;
	RwV3dSubMacro(&vRayDeltha, pvEnd, pvStart);

	if(RtIntersectionLineTriangle(pvStart, &vRayDeltha, &vVertex[0], &vVertex[1], &vVertex[2], &fDist))
	{
		return TRUE;
	}
	else if (RtIntersectionLineTriangle(pvStart, &vRayDeltha, &vVertex[0], &vVertex[2], &vVertex[3], &fDist))
	{
		return TRUE;
	}
	
	// 양면일 경우는 뒷면도 확인해야 한다.
	if (m_ePlaneType == EPLOCCLUDER_PLANE_TWOSIDE)
	{
		if(RtIntersectionLineTriangle(pvStart, &vRayDeltha, &vVertex[2], &vVertex[1], &vVertex[0], &fDist))
		{
			return TRUE;
		}
		else if(RtIntersectionLineTriangle(pvStart, &vRayDeltha, &vVertex[3], &vVertex[2], &vVertex[0], &fDist))
		{
			return TRUE;
		}
	}

	return FALSE;
}

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlPLOccluder_Quad::CalcOccluderData()
{
	RwMatrix		mat;

	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
	{
		// PVS 용 더미 오클루더 일 경우. Scale은 384.0f 가 Max 이다.
		CLAMP(m_sOccluderSwap.vScale.x, 1.0f, 384.0f);
		CLAMP(m_sOccluderSwap.vScale.y, 1.0f, 384.0f);
		m_sOccluderSwap.vScale.z = 1.0f;
	}
	else
	{
		// 일반(사각형) 오클루더 일 경우. Scale은 512.0f 가 Max 이다.
		CLAMP(m_sOccluderSwap.vScale.x, 1.0f, 512.0f);
		CLAMP(m_sOccluderSwap.vScale.y, 1.0f, 512.0f);
		m_sOccluderSwap.vScale.z = 1.0f;
	}

	RwMatrixScale(&mat, &m_sOccluderSwap.vScale, rwCOMBINEREPLACE);

	// 빌보드 일때는 vRotate가 적용 되지 않는다.
	if (GetOccluderPlaneType() != EPLOCCLUDER_PLANE_BILLBOARD)
	{
		RwMatrixRotate(&mat, &CNtlPLGlobal::m_vYAxisV3, m_sOccluderSwap.vRotate.y, rwCOMBINEPOSTCONCAT);
	}

	// Vertex Index :	2 1 : 5 Datum
	//					3 0 
	static RwV3d vVertex[4] = {
		{ -0.5f, -0.5f + dPL_OCCLUDER_OFFSET_Y, 0.0f },
		{ -0.5f,  0.5f + dPL_OCCLUDER_OFFSET_Y, 0.0f },
		{  0.5f,  0.5f + dPL_OCCLUDER_OFFSET_Y, 0.0f },
		{  0.5f, -0.5f + dPL_OCCLUDER_OFFSET_Y, 0.0f },
	};

	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
	{
		RwV3dTransformPoints(&m_vVertexOriginal[0], &vVertex[0], 4, &mat);
	}
	else
	{
		RwV3d vTemp[4];
		for (int i = 0; i < 4; ++i)
		{
			RwV3dAdd(&vTemp[i], &vVertex[i], &m_sOccluderSwap.vTrans[i]);
		}

		RwV3dTransformPoints(&m_vVertexOriginal[0], &vTemp[0], 4, &mat);

		// 일반(사각형) 오클루더 일 경우. Vertex Trans를 지원하기 때문에. Max(512.0f)를 넘었는지 확인하고 교정한다.
		for (int i = 0; i < 4; ++i)
		{
			RwV3d	vDir;
			RwReal	vBackupY;
			RwReal	fLength;

			vDir	= m_vVertexOriginal[i]; vDir.y = 0.0f;
			vBackupY= m_vVertexOriginal[i].y;
			fLength = RwV3dLength(&vDir);
			if (fLength > 256.0f)
			{
				RwV3dNormalize(&vDir, &vDir);
				RwV3dScale(&m_vVertexOriginal[i], &vDir, 256.0f);
				m_vVertexOriginal[i].y = vBackupY;
			}
		}
	}

	CalcBoundingSphere();
}

void CNtlPLOccluder_Quad::SaveSwapFile(RwReal x, RwReal y, RwReal z)
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

		if(_chdir("occ") == -1)
		{
			_mkdir("occ");
			_chdir("occ");
		}

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ocq", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}
		
		RwInt32 iNum = static_cast<RwInt32>(pSector->m_vecNtlPLEntityOccluder_Quad.size());

		fwrite(&iNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLOccluder_Quad* pOccluder = static_cast<CNtlPLOccluder_Quad*>(pSector->m_vecNtlPLEntityOccluder_Quad.at(i));
			fwrite(&pOccluder->m_sOccluderSwap.vScale, sizeof(RwV3d), 1, pFile);
			fwrite(&pOccluder->m_sOccluderSwap.vRotate, sizeof(RwV3d), 1, pFile);
			fwrite(&pOccluder->m_sOccluderSwap.vTrans[0], sizeof(RwV3d)* 4, 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlPLOccluder_Quad::LoadSwapFile(RwReal x, RwReal y, RwReal z)
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
		_chdir("occ");

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ocq", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iNum;

		fread(&iNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iNum; ++i)
		{
			CNtlPLOccluder_Quad* pOccluder = static_cast<CNtlPLOccluder_Quad*>(pSector->m_vecNtlPLEntityOccluder_Quad.at(i));
			fread(&pOccluder->m_sOccluderSwap.vScale, sizeof(RwV3d), 1, pFile);
			fread(&pOccluder->m_sOccluderSwap.vRotate, sizeof(RwV3d), 1, pFile);
			fread(&pOccluder->m_sOccluderSwap.vTrans[0], sizeof(RwV3d)* 4, 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}
#endif

RwBool CNtlPLOccluder_Quad::LoadFromFile(FILE* pFile)
{
	RwInt32	iPlaneType;	
	fread(&iPlaneType, sizeof(RwInt32), 1, pFile);

	RwChar acTemp[128];	
	fread(acTemp, sizeof(RwChar) * 128, 1, pFile);

	for (int j = 0; j < 4; ++j)
	{
		RwV3d vVertex;
		fread(&vVertex, sizeof(RwV3d), 1, pFile);
		SetVertexOriginal(j, &vVertex);
	}
	
	SetOccluderPlaneType(iPlaneType);
	SetTriggerName(acTemp);
	CalcBoundingSphere();

	return TRUE;
}

RwBool CNtlPLOccluder_Quad::SaveIntoFile(FILE* pFile)
{
	RwInt32 iPlaneType = GetOccluderPlaneType();	
	fwrite(&iPlaneType, sizeof(RwInt32), 1, pFile);

	RwChar acTemp[128];
	strcpy_s(acTemp, 128, GetTriggerName());
	fwrite(acTemp, sizeof(RwChar) * 128, 1, pFile);

	for (RwInt32 i = 0; i < 4; ++i)
	{
		RwV3d vTemp = *GetVertexOriginal(i);
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);
	}

	return TRUE;
}

RwInt32 CNtlPLOccluder_Quad::SkipToFile(FILE* pFile)
{
	RwInt32 iRange = sizeof(RwInt32) + (sizeof(RwChar) * 128) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d);

	::fseek(pFile, iRange, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlPLOccluder_Quad::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 iPlaneType;
	CopyMemory(&iPlaneType, pFileMem, sizeof(RwInt32)); // iPlaneType
	pFileMem += sizeof(RwInt32);
	::fwrite(&iPlaneType, sizeof(RwInt32), 1, pFile);

	RwChar acTemp[128];
	CopyMemory(acTemp, pFileMem, sizeof(RwChar) * 128);
	pFileMem += sizeof(RwChar) * 128;
	::fwrite(acTemp, sizeof(RwChar) * 128, 1, pFile);

	for (int i = 0; i < 4; ++i)
	{
		RwV3d vTemp;
		CopyMemory(&vTemp, pFileMem, sizeof(RwV3d)); // vVertex
		pFileMem += sizeof(RwV3d);
		::fwrite(&vTemp, sizeof(RwV3d), 1, pFile);
	}

	return pFileMem;
}

BYTE* CNtlPLOccluder_Quad::SkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwInt32);

	pFileMem += sizeof(RwChar) * 128;

	for (int i = 0; i < 4; ++i)
	{
		pFileMem += sizeof(RwV3d);
	}

	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif