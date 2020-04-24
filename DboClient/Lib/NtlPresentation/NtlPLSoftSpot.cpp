#include "precomp_ntlpresentation.h"
// CZ-SS
// #include "NtlPLSoftSpot.h"
// #include "NtlPLGlobal.h"
// #include "NtlPLLight.h"
// #include "NtlPLEvent.h"
// #include "NtlWorldInterface.h"
// #include "NtlWorldPaletteDefine.h"
// #include "ntlworldcommon.h"
// #include "NtlPLVisualManager.h"
// #include "NtlPLWorldEntity.h"
// #include "NtlMath.h"
// 
// #ifdef dNTL_WORLD_TOOL_MODE
// RwReal CNtlPLSpot::s_fPLSpotIpTimeIn	= 0.01f;
// RwReal CNtlPLSpot::s_fPLSpotIpTimeOut	= 0.01f;
// #else
// RwReal CNtlPLSpot::s_fPLSpotIpTimeIn	= 0.5f;
// RwReal CNtlPLSpot::s_fPLSpotIpTimeOut	= 0.25f;
// #endif
// 
// 
// CNtlPLSpot::CNtlPLSpot( void )
// :m_fInterpolationTime(-999.0f)
// ,m_eFadeType(0)
// ,m_pLight(NULL)
// ,m_fAngle(dCONE_ANGLE)
// {
// 	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOT_VISIBLE | NTL_PLEFLAG_NOTUPDATE);
// 	SetLayer(PLENTITY_LAYER_LIGHT);
// 	SetClassType(PLENTITY_SPOT_LIGHT);
// 
// 	m_fRadius[0] = m_fRadius[1] = m_fRadius[2] = dCONE_RADIUS;
// 
// 	m_vRotate.x		= 90.0f;
// 	m_vRotate.y		= 0.0f;
// 	m_vRotate.z		= 0.0f;
// 	
// 	m_rgbafColor.red	= 1.0f;
// 	m_rgbafColor.green	= 0.0f;
// 	m_rgbafColor.blue	= 0.0f;
// 	m_rgbafColor.alpha	= 1.0f;
// 	
// 	m_fInterpolationTime = -999.0f;
// }
// 
// CNtlPLSpot::~CNtlPLSpot( void )
// {
// }
// 
// RwBool CNtlPLSpot::Create(const SPLEntityCreateParam * pParam)
// {
// 	m_pLight = RpLightCreate(rpLIGHTSPOTSOFT);
// 	DBO_ASSERT(m_pLight, "Light create failed");
// 		
// 	SetRadius(GetRadius());
// 	SetAngle(GetAngle());
// 	SetColor(GetColor());
// 
// 	RwFrame* pFrame = RwFrameCreate();
// 	DBO_ASSERT(m_pLight, "Frame create failed");
// 
// 	RwFrameSetIdentity(pFrame);
// 	RpLightSetFrame(m_pLight, pFrame);
// 
// 	SetPosition(pParam->pPos);
// 	SetRotate(m_vRotate.x, m_vRotate.y, m_vRotate.z);
// 
// 	if (CNtlWorldRpLightManager::GetInstance())
// 	{
// 		CNtlWorldRpLightManager::GetInstance()->RequestCreateRpLight(m_pLight, this);
// 	}
// 
// 	return TRUE;
// }
// 
// void CNtlPLSpot::Destroy( void ) 
// {
// 	if (m_pLight)
// 	{
// 		if (CNtlWorldRpLightManager::GetInstance())
// 		{
// 			CNtlWorldRpLightManager::GetInstance()->RequestDestroyRpLight(m_pLight);
// 		}
// 
// 		RwFrame* pFrame = RpLightGetFrame(m_pLight);
// 		RpLightSetFrame(m_pLight, NULL);
// 		if (pFrame)
// 		{
// 			RwFrameDestroy(pFrame);
// 		}
// 		RpLightDestroy(m_pLight);
// 		m_pLight = NULL;
// 	}
// }
// 
// void CNtlPLSpot::OnUpdate(RpLight* pLight, RwReal fElapsed)
// {
// 	if (m_pLight)
// 	{		
// 		RwV3d			vCameraPos	= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
// 		RwV3d			vLightPos	= *RwMatrixGetPos(RwFrameGetMatrix(RpLightGetFrame(m_pLight)));
// 		RwV3d			vLightAt	= *RwMatrixGetAt(RwFrameGetMatrix(RpLightGetFrame(m_pLight)));
// 		static RwReal	fDistFar	= 90000.0f;/*(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize) * 2.0f) * *static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize) * 2.0f)*/;
// 		RwV3d			vDist		= vCameraPos - vLightPos;		
// 		RwReal			fDist		= CNtlMath::MathRwV3dSquaredLength(&vDist);
// 		if (fDistFar > fDist) 
// 		{		
// 			RwReal fRadius = GetRadius() * GetRadius();
// 			RwV3d v1, v2;
// 			RwV3dNormalize(&v1, &vDist);
// 			RwV3dNormalize(&v2, &vLightAt);
// 
// 			RwReal fAngle = (acosf(RwV3dDotProduct(&v1, &v2))) * (180.0f / D3DX_PI);
// 			fRadius = (m_fAngle - fAngle) / m_fAngle * fRadius;							
// 			
// 			SetLightValue(fDist - fRadius);
// 
// #ifdef dNTL_WORLD_TOOL_MODE
// 			if (s_pPLSpotSelected == this)
// 			{
// 				SetLightValue(RwRealMINVAL);
// 			}
// #endif
// 			CNtlWorldRpLightManager::GetInstance()->RequestAddRpLight(m_pLight, this);			
// 		}
// 		else
// 		{
// 			CNtlWorldRpLightManager::GetInstance()->RequestRemoveRpLight(m_pLight);
// 		}
// 
// 		if (m_fInterpolationTime >= 0.0f)
// 		{
// 			m_fInterpolationTime -= fElapsed;
// 			if (m_fInterpolationTime < 0.0f)
// 			{
// 				m_fInterpolationTime	= -999.0f;
// 				m_fRadius[0]			= (m_eFadeType == 1 ? GetRadius() : 0.0f);
// 				if (m_fRadius[0] == 0.0f)
// 				{
// 					CNtlWorldRpLightManager::GetInstance()->RequestRemoveRpLight(m_pLight);
// 				}
// 			}
// 			else
// 			{
// 				if (m_eFadeType == 1)	// FADE IN
// 				{
// 					m_fRadius[0] = RpNtlWorldRealPropLerp(m_fRadius[1],	GetRadius(), m_fInterpolationTime, s_fPLSpotIpTimeIn);
// 				}
// 				else if (m_eFadeType == 2) // FADE OUT
// 				{
// 					m_fRadius[0] = RpNtlWorldRealPropLerp(m_fRadius[1], 0.0f, m_fInterpolationTime, s_fPLSpotIpTimeOut);
// 				}
// 			}
// 			RpLightSetRadius(m_pLight, m_fRadius[0]);
// 		}
// 
// 		RwReal TFactor = -static_cast<RwReal>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff) / 255.0f;
// 
// 		RwRGBAReal rgbaLight = m_rgbafColor;
// 		::API_PL_1BColorClamp(&rgbaLight.red, TFactor);
// 		::API_PL_1BColorClamp(&rgbaLight.green, TFactor);
// 		::API_PL_1BColorClamp(&rgbaLight.blue, TFactor);
// 		RpLightSetColor(m_pLight, &rgbaLight);
// 	}
// }
// 
// void CNtlPLSpot::OnRender(RpLight* pLight)
// {
// #ifdef dNTL_WORLD_TOOL_MODE
// 	if(!m_pLight)
// 	{
// 		return;
// 	}
// 
// 	RwMatrix*		pMatrix;
// 	RwV3d*			pRight;
// 	RwV3d*			pUp;
// 	RwV3d*			pAt;
// 	RwV3d*			pPos;
// 	RwV3d			Point;
// 	RwReal			cosValue, sinValue, m_ConeAngleD;
// 	RwV3d			dRight, dUp, dAt;
// 
// 	pMatrix = RwFrameGetLTM(RpLightGetFrame(m_pLight));
// 	pRight	= RwMatrixGetRight(pMatrix);
// 	pUp		= RwMatrixGetUp(pMatrix);
// 	pAt		= RwMatrixGetAt(pMatrix);
// 	pPos	= RwMatrixGetPos(pMatrix);
// 
// 	// calculate m_Cone mesh vertices
// 	for(RwInt32 i = 1; i < dCONE_NUM_VERTICES + 1; i++)
// 	{
// 		cosValue = (RwReal)(RwCos(i / (dCONE_NUM_VERTICES / 2.0f) * rwPI) * RwSin(m_fAngle / 180.0f * rwPI));
// 		sinValue = (RwReal)(RwSin(i / (dCONE_NUM_VERTICES / 2.0f) * rwPI) * RwSin(m_fAngle / 180.0f * rwPI)); 
// 
// 		RwV3dScale(&dUp, pUp, sinValue *  GetRadius());
// 		RwV3dScale(&dRight, pRight, cosValue *  GetRadius());
// 
// 		m_ConeAngleD = (RwReal)RwCos(m_fAngle / 180.0f * rwPI);
// 
// 		RwV3dScale(&dAt, pAt, m_ConeAngleD *  GetRadius());
// 
// 		Point.x = pPos->x + dAt.x + dUp.x + dRight.x;
// 		Point.y = pPos->y + dAt.y + dUp.y + dRight.y;
// 		Point.z = pPos->z + dAt.z + dUp.z + dRight.z;
// 
// 		RwIm3DVertexSetPos(&m_Cone[i], Point.x, Point.y, Point.z);        
// 	}
// 
// 	// Set up vertex list
// 	for(RwInt32 i = 0; i < dCONE_NUM_VERTICES; i++)
// 	{
// 		m_Indices[(i * 3)]		= static_cast<RwImVertexIndex>(0);
// 		m_Indices[(i * 3) + 1]	= static_cast<RwImVertexIndex>(i + 2);
// 		m_Indices[(i * 3) + 2]	= static_cast<RwImVertexIndex>(i + 1);
// 	}
// 
// 	m_Indices[(dCONE_NUM_VERTICES * 3) - 2] = 1;
// 
// 	// Set color & alpha of all points
// 	for(RwInt32 i = 0; i < (dCONE_NUM_VERTICES + 1); i++)
// 	{
// 
// 		if (s_pPLSpotSelected == this)
// 		{
// 			RwIm3DVertexSetRGBA(&m_Cone[i], 255, 0, 0, 255);
// 		}
// 		else if (RpLightGetWorld(m_pLight))
// 		{
// 			RwIm3DVertexSetRGBA(&m_Cone[i], 253, 255, 0, 255);
// 		}
// 		else
// 		{
// 			RwIm3DVertexSetRGBA(&m_Cone[i], 0, 0, 0, 255);
// 		}		
// 	}
// 
// 	// Set m_Cone apex to light position...
// 	RwIm3DVertexSetPos(&m_Cone[0],  pPos->x, pPos->y, pPos->z);
// 
// 	// Set m_Cone apex to same level as m_Cone base 
// 	m_ConeAngleD = (RwReal)RwCos(m_fAngle / 180.0f * rwPI);
// 
// 	RwV3dScale(&dAt, pAt, m_ConeAngleD * GetRadius());
// 
// 	Point.x = pPos->x + dAt.x;
// 	Point.y = pPos->y + dAt.y;
// 	Point.z = pPos->z + dAt.z;
// 
// 	RwIm3DVertexSetPos(&m_Cone[0], Point.x, Point.y, Point.z);
// 	
// 	// Move m_Cone apex by small offset
// 	RwV3dScale(&dAt, pAt, -0.05f);
// 	Point.x = pPos->x + dAt.x;
// 	Point.y = pPos->y + dAt.y;
// 	Point.z = pPos->z + dAt.z;
// 	RwIm3DVertexSetPos(&m_Cone[0], Point.x, Point.y, Point.z);
// 
// 	// Draw Lines
// 	RwD3D9SetTexture(NULL, 0);
// 	if(RwIm3DTransform(m_Cone, dCONE_NUM_VERTICES + 1, NULL, rwIM3D_ALLOPAQUE))
// 	{        
// 		RwIm3DRenderIndexedPrimitive(rwPRIMTYPEPOLYLINE, m_Indices, dCONE_NUM_VERTICES * 3);
// 
// 		RwIm3DEnd();
// 	}
// #endif
// }
// 
// void CNtlPLSpot::OnAddRpLight(RpLight* pRpLight)
// {
// 	if (m_eFadeType != 1)
// 	{
// 		m_fRadius[1]		= m_fRadius[0];
// 		m_eFadeType			= 1;
// 		m_fInterpolationTime= s_fPLSpotIpTimeIn;
// 	}
// }
// 
// void CNtlPLSpot::OnRemoveRpLight(RpLight* pRpLight)
// {
// 	m_fRadius[0]		= 0.0f;
// 	m_eFadeType			= 2;
// 	m_fInterpolationTime= -999.0f;
// 
// 	if (m_pLight)
// 	{
// 		RpLightSetRadius(m_pLight, m_fRadius[0]);
// 	}
// }
// 
// void CNtlPLSpot::OnRequestRemoveRpLight(RpLight* pRpLight)
// {
// 	if (m_eFadeType != 2)
// 	{
// 		m_fRadius[1]		= m_fRadius[0];
// 		m_eFadeType			= 2;
// 		m_fInterpolationTime= s_fPLSpotIpTimeOut;
// 	}
// }
// 
// VOID CNtlPLSpot::SetTransform()
// {
// 	if (!m_pLight)
// 	{
// 		return;
// 	}
// 
// 	RwFrame*	pFrame	= RpLightGetFrame(m_pLight);
// 	RwMatrix*	pMatrix	= RwFrameGetMatrix(pFrame);
// 	RwMatrix	Matrix;
// 	RwV3d		Front, Left, Up;
// 
// 	RwMatrixRotate(&Matrix, &CNtlPLGlobal::m_vYAxisV3, m_vRotate.y, rwCOMBINEREPLACE);
// 	RwV3dTransformVector(&Front, &CNtlPLGlobal::m_vZAxisV3, &Matrix);
// 	RwV3dTransformVector(&Left, &CNtlPLGlobal::m_vXAxisV3, &Matrix);
// 
// 	RwMatrixRotate(&Matrix, &Left, m_vRotate.x, rwCOMBINEREPLACE);
// 	RwV3dTransformVector(&Front, &Front, &Matrix);
// 	RwV3dTransformVector(&Up, &CNtlPLGlobal::m_vYAxisV3, &Matrix);
// 
// 	RwMatrixRotate(&Matrix, &CNtlPLGlobal::m_vZAxisV3, m_vRotate.z, rwCOMBINEREPLACE);        
// 	RwV3dTransformVector(&Left, &Left, &Matrix);
// 	RwV3dTransformVector(&Front, &Front, &Matrix);
// 
// 	RwMatrixSetIdentity(pMatrix);
// 	RwMatrixRotate(pMatrix, &CNtlPLGlobal::m_vYAxisV3, m_vRotate.y, rwCOMBINEREPLACE);
// 	RwMatrixRotate(pMatrix, &Left, m_vRotate.x, rwCOMBINEPOSTCONCAT);
// 	RwMatrixRotate(pMatrix, &Front, m_vRotate.z, rwCOMBINEPOSTCONCAT);
// 	RwMatrixTranslate(pMatrix, &m_vPosition, rwCOMBINEPOSTCONCAT);
// 
// 	RwFrameUpdateObjects(pFrame);
// }
// 
// void CNtlPLSpot::SetPosition(const RwV3d *pPos)
// {
// 	m_vPosition = *pPos;
// 
// 	SetTransform();
// }
// 
// void CNtlPLSpot::SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle)
// {
// 	m_vRotate.x = fXAngle;
// 	m_vRotate.y = fYAngle;
// 	m_vRotate.z = fZAngle;
// 
// 	SetTransform();
// }
// 
// VOID CNtlPLSpot::SetRadius(RwReal fRadius)
// {
// 	m_fRadius[1] = m_fRadius[0];
// 	m_fRadius[2] = fRadius;
// 	if (m_pLight)
// 	{
// 		RpLightSetRadius(m_pLight, m_fRadius[2]);
// 	}
// }
// 
// VOID CNtlPLSpot::SetAngle(RwReal fAngle)
// {
// 	m_fAngle = fAngle;
// 	if (m_pLight)
// 	{
// 		RpLightSetConeAngle(m_pLight, m_fAngle / 180.0f * rwPI);
// 	}	
// }
// 
// VOID CNtlPLSpot::SetColor(RwRGBAReal& rgbafColor)
// {
// 	m_rgbafColor = rgbafColor;
// 
// 	if (m_pLight)
// 	{
// 		RpLightSetColor(m_pLight, &m_rgbafColor);
// 	}
// }
// 
// RwBool CNtlPLSpot::SetProperty( const CNtlPLProperty * pData )
// {
// 	return TRUE;
// }
// 
// RwBool CNtlPLSpot::Update(RwReal fElapsed)
// {	
// 	return TRUE;
// }
// 
// RwBool CNtlPLSpot::Render()
// {
// 	return TRUE;
// }
// 
// #ifdef dNTL_WORLD_TOOL_MODE
// 
// CNtlPLSpot*	CNtlPLSpot::s_pPLSpotSelected = NULL;
// 
// RwBool CNtlPLSpot::Pick(RwReal& _CurDist)
// {
// 	if(!m_pLight)
// 	{
// 		return FALSE;
// 	}
// 
// 	RwMatrix*		pMatrix;
// 	RwV3d*			pRight;
// 	RwV3d*			pUp;
// 	RwV3d*			pAt;
// 	RwV3d*			pPos;
// 	RwV3d			Point;
// 	RwReal			cosValue, sinValue, m_ConeAngleD;
// 	RwV3d			dRight, dUp, dAt;
// 
// 	pMatrix = RwFrameGetLTM(RpLightGetFrame(m_pLight));
// 	pRight	= RwMatrixGetRight(pMatrix);
// 	pUp		= RwMatrixGetUp(pMatrix);
// 	pAt		= RwMatrixGetAt(pMatrix);
// 	pPos	= RwMatrixGetPos(pMatrix);
// 
// 	for(RwInt32 i = 1; i < dCONE_NUM_VERTICES + 1; i++)
// 	{
// 		cosValue = (RwReal)(RwCos(i / (dCONE_NUM_VERTICES / 2.0f) * rwPI) * RwSin(m_fAngle / 180.0f * rwPI));
// 		sinValue = (RwReal)(RwSin(i / (dCONE_NUM_VERTICES / 2.0f) * rwPI) * RwSin(m_fAngle / 180.0f * rwPI)); 
// 
// 		RwV3dScale(&dUp, pUp, sinValue * GetRadius());
// 		RwV3dScale(&dRight, pRight, cosValue * GetRadius());
// 
// 		m_ConeAngleD = (RwReal)RwCos(m_fAngle / 180.0f * rwPI);
// 
// 		RwV3dScale(&dAt, pAt, m_ConeAngleD * GetRadius());
// 
// 		Point.x = pPos->x + dAt.x + dUp.x + dRight.x;
// 		Point.y = pPos->y + dAt.y + dUp.y + dRight.y;
// 		Point.z = pPos->z + dAt.z + dUp.z + dRight.z;
// 
// 		RwIm3DVertexSetPos(&m_Cone[i], Point.x, Point.y, Point.z);        
// 	}
// 
// 	// Set up vertex list
// 	for(RwInt32 i = 0; i < dCONE_NUM_VERTICES; i++)
// 	{
// 		m_Indices[(i * 3)]		= static_cast<RwImVertexIndex>(0);
// 		m_Indices[(i * 3) + 1]	= static_cast<RwImVertexIndex>(i + 2);
// 		m_Indices[(i * 3) + 2]	= static_cast<RwImVertexIndex>(i + 1);
// 	}
// 
// 	m_Indices[(dCONE_NUM_VERTICES * 3) - 2] = 1;
// 
// 	// Set m_Cone apex to light position...
// 	RwIm3DVertexSetPos(&m_Cone[0],  pPos->x, pPos->y, pPos->z);
// 
// 	// Find unit ray vector
// 	RwV2d ScreenPos;
// 	ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
// 	ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;
// 
// 	RwLine cameraRay;
// 	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);
// 
// 	RwV3d RwRayDir, RayDeltha;
// 	RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);
// 	RwV3dSubMacro(&RayDeltha, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, &cameraRay.start);
// 
// 	RwBool Picked = FALSE;
// 	RwReal ResultDist;
// 
// 	_CurDist = RwRealMAXVAL;
// 
// 	for(RwInt32 i = 0; i < dCONE_NUM_VERTICES; ++i)
// 	{
// 		if(RtIntersectionLineTriangle(&cameraRay.start, &RayDeltha,	&m_Cone[m_Indices[(i * 3) + 0]].objVertex,
// 			&m_Cone[m_Indices[(i * 3) + 1]].objVertex,
// 			&m_Cone[m_Indices[(i * 3) + 2]].objVertex,
// 			&ResultDist))
// 		{
// 			Picked = TRUE;
// 
// 			if(ResultDist < _CurDist)
// 			{
// 				_CurDist = ResultDist;
// 			}
// 		}
// 	}
// 
// 	if(Picked)
// 	{
// 		return TRUE;
// 	}
// 
// 	return FALSE;
// }
// 
// void CNtlPLSpot::SetToolUI(CNtlPLSpot* pPLSpot)
// {
// 	s_pPLSpotSelected = pPLSpot;
// }
// 
// #endif