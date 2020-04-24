#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////


#include "NtlWorldBrushController.h"
#include "NtlPLRenderState.h"


////////////////////////////////////////////////////////////////////////////////////////////////////

template <class CBObjClass>
class CNTLWB_Frustum :public CNtlWorldBrushController
{
private:
	CBObjClass* m_pObj;  
	RwReal (CBObjClass::*m_pFunc)(RwV3d& Pos);

public:
	CNTLWB_Frustum(eNTL_BRUSH_TYPE ID, CBObjClass* pObj);
	~CNTLWB_Frustum();
	
public:
	void	SetPosS(RwV2d& vPos);
	void	SetPosE(RwV2d& vPos);

	RwV2d&	GetPosS();
	RwV2d&	GetPosE();

public:
	void	Render();

	void	CalcFrustumData();
	RwBool	FrustumTest(RwV3d& vPos);

private:
	RwV2d	m_vPosS, m_vPosE;
	RwBBox	m_bboxFrustum;
};

template <class CBObjClass>
CNTLWB_Frustum<CBObjClass>::~CNTLWB_Frustum()
{
}

template <class CBObjClass>
CNTLWB_Frustum<CBObjClass>::CNTLWB_Frustum(eNTL_BRUSH_TYPE ID, CBObjClass* pObj)
{
	m_pObj		= pObj;  
	m_ID		= ID;
	m_vPosS.x = m_vPosS.y = m_vPosE.x = m_vPosE.y = 0.0f;
}

template <class CBObjClass>
RwV2d& CNTLWB_Frustum<CBObjClass>::GetPosS()
{
	NTL_FUNCTION("CNTLWB_Frustum<CBObjClass>::GetPosS");

	NTL_RETURN(m_vPosS);
}

template <class CBObjClass>
RwV2d& CNTLWB_Frustum<CBObjClass>::GetPosE()
{
	NTL_FUNCTION("CNTLWB_Frustum<CBObjClass>::GetPosE");

	NTL_RETURN(m_vPosE);
}

template <class CBObjClass>
void CNTLWB_Frustum<CBObjClass>::SetPosS(RwV2d& vPos)
{
	NTL_FUNCTION("CNTLWB_Frustum<CBObjClass>::SetPosS");

	m_vPosS = vPos;

	NTL_RETURNVOID();
}

template <class CBObjClass>
void CNTLWB_Frustum<CBObjClass>::SetPosE(RwV2d& vPos)
{
	NTL_FUNCTION("CNTLWB_Frustum<CBObjClass>::SetPosE");

	m_vPosE = vPos;

	NTL_RETURNVOID();
}

template <class CBObjClass>
void CNTLWB_Frustum<CBObjClass>::CalcFrustumData()
{
	NTL_FUNCTION("CNTLWB_Frustum<CBObjClass>::CalcFrustumData");

 	RwLine		rayTemp; 
	RwV3d		acTemp[2];
	
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &rayTemp, &m_vPosS);
 	RwV3dTransformPoint(&acTemp[0], &rayTemp.start, &CNtlPLGlobal::m_RwCamera->viewMatrix);
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &rayTemp, &m_vPosE);
	RwV3dTransformPoint(&acTemp[1], &rayTemp.start, &CNtlPLGlobal::m_RwCamera->viewMatrix);

	if (RwCameraGetProjection(CNtlPLGlobal::m_RwCamera) == rwPERSPECTIVE)
	{
		acTemp[0].x = acTemp[0].x / acTemp[0].z;
		acTemp[0].y = acTemp[0].y / acTemp[0].z;
		acTemp[0].z = acTemp[0].z / acTemp[0].z;
		acTemp[1].x = acTemp[1].x / acTemp[1].z;
		acTemp[1].y = acTemp[1].y / acTemp[1].z;
		acTemp[1].z = acTemp[1].z / acTemp[1].z;
	}

	RwBBoxCalculate(&m_bboxFrustum, acTemp, 2);

	NTL_RETURNVOID();
}

template <class CBObjClass>
RwBool CNTLWB_Frustum<CBObjClass>::FrustumTest(RwV3d& vPos)
{
	NTL_FUNCTION("CNTLWB_Frustum<CBObjClass>::FrustumTest");

	RwV3d vTemp;
	RwV3dTransformPoint(&vTemp, &vPos, &CNtlPLGlobal::m_RwCamera->viewMatrix);

	if (vTemp.z < 0.0f)
	{
		return FALSE;
	}
	
	if (RwCameraGetProjection(CNtlPLGlobal::m_RwCamera) == rwPERSPECTIVE)
	{
		vTemp.x = vTemp.x / vTemp.z;
		vTemp.y = vTemp.y / vTemp.z;
		vTemp.z = vTemp.z / vTemp.z;
	}

	if (m_bboxFrustum.inf.x < vTemp.x && m_bboxFrustum.inf.y < vTemp.y &&
		m_bboxFrustum.sup.x > vTemp.x && m_bboxFrustum.sup.y > vTemp.y)
	{
		return TRUE;
	}
	
	return FALSE;
}

template <class CBObjClass>
void CNTLWB_Frustum<CBObjClass>::Render()
{
	NTL_FUNCTION("CNTLWB_Frustum<CBObjClass>::Render");

	static RwIm2DVertex aIm2DVertex[4];
	
	BeginFogState(FALSE);
	BeginLightState(FALSE);
	
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

	RwD3D9SetTexture(NULL, 0);

	RwIm2DVertexSetScreenX(&aIm2DVertex[0], m_vPosS.x);
	RwIm2DVertexSetScreenX(&aIm2DVertex[1], m_vPosS.x);
	RwIm2DVertexSetScreenX(&aIm2DVertex[2], m_vPosE.x);
	RwIm2DVertexSetScreenX(&aIm2DVertex[3], m_vPosE.x);

	RwIm2DVertexSetScreenY(&aIm2DVertex[0], m_vPosS.y);
	RwIm2DVertexSetScreenY(&aIm2DVertex[1], m_vPosE.y);
	RwIm2DVertexSetScreenY(&aIm2DVertex[2], m_vPosE.y);
	RwIm2DVertexSetScreenY(&aIm2DVertex[3], m_vPosS.y);

	RwIm2DVertexSetScreenZ(&aIm2DVertex[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetScreenZ(&aIm2DVertex[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetScreenZ(&aIm2DVertex[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetScreenZ(&aIm2DVertex[3], RwIm2DGetNearScreenZ());

	static RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);

	RwIm2DVertexSetRecipCameraZ(&aIm2DVertex[0], recipCameraZ);
	RwIm2DVertexSetRecipCameraZ(&aIm2DVertex[1], recipCameraZ);
	RwIm2DVertexSetRecipCameraZ(&aIm2DVertex[2], recipCameraZ);
	RwIm2DVertexSetRecipCameraZ(&aIm2DVertex[3], recipCameraZ);

	RwIm2DVertexSetIntRGBA(&aIm2DVertex[0], 255, 255, 255, 255);
	RwIm2DVertexSetIntRGBA(&aIm2DVertex[1], 255, 255, 255, 255);
	RwIm2DVertexSetIntRGBA(&aIm2DVertex[2], 255, 255, 255, 255);
	RwIm2DVertexSetIntRGBA(&aIm2DVertex[3], 255, 255, 255, 255);

	RwIm2DRenderLine(aIm2DVertex, 4, 0, 1);
	RwIm2DRenderLine(aIm2DVertex, 4, 1, 2);
	RwIm2DRenderLine(aIm2DVertex, 4, 2, 3);
	RwIm2DRenderLine(aIm2DVertex, 4, 3, 0);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

	EndLightState();
	EndFogState();

	NTL_RETURNVOID();
}