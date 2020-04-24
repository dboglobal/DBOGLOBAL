#include "precomp_ntlpresentation.h"

#include "NtlWorldCommon.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlWorldInterface.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"

#include "NtlPLEvent.h"
#include "NtlPLRenderGeometry.h"

#include "NtlPLRenderState.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLLightObject.h"

#ifdef dNTL_WORLD_TOOL_MODE
RwReal CNtlPLLightObject::s_fPLLightObjectIpTimeIn	= 0.01f;
RwReal CNtlPLLightObject::s_fPLLightObjectIpTimeOut	= 0.01f;
#else
RwReal CNtlPLLightObject::s_fPLLightObjectIpTimeIn	= 0.5f;
RwReal CNtlPLLightObject::s_fPLLightObjectIpTimeOut	= 0.25f;
#endif

CNtlPLLightObject::CNtlPLLightObject()
:m_fInterpolationTime(-999.0f)
,m_eFadeType(0)
,m_rpLightType(rpNALIGHTTYPE)
,m_bRealLight(FALSE)
,m_pLight(NULL)
,m_fAngle(0.0f)
,m_bWorldLight(FALSE)
,m_pNtlPLDecal(NULL)
{
#ifdef dNTL_WORLD_TOOL_MODE
	m_pAtomicCtlPt			= NULL;
	m_bRealLightColor		= FALSE;
	m_fRealLightColorTime	= 0.5f;
#endif
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_ATTACH | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOTUPDATE | NTL_PLEFLAG_NOT_VISIBLE);
	SetLayer(PLENTITY_LAYER_LIGHT);
	SetClassType(PLENTITY_LIGHT_OBJECT);

	m_fRadius[0] = m_fRadius[1] = m_fRadius[2] = 0.0f;
}

CNtlPLLightObject::~CNtlPLLightObject()
{
}

RwBool CNtlPLLightObject::SetProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLLightObject::SetProperty");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLLightObject::Create(const SPLEntityCreateParam *pParam)
{
	NTL_FUNCTION("CNtlPLLightObject::Create");

	SPLLightObjectCreateParam* pCreateParam = (SPLLightObjectCreateParam*)(pParam);

	m_pLight = RpLightCreate(pCreateParam->rpLightType);
	DBO_ASSERT(m_pLight, "Light create failed.");

	RwMatrixSetIdentity(&m_matParant);
	
	SetRadius(pCreateParam->fRadius);
	SetAngle(pCreateParam->fAngle);
	SetColor(pCreateParam->rgbafColor);

	RwFrame* pFrame = RwFrameCreate();
	DBO_ASSERT(pFrame, "Frame create failed.");

	RwFrameSetIdentity(pFrame);
	RpLightSetFrame(m_pLight, pFrame);

	SetPosition(pCreateParam->pPos);
	SetRotate(pCreateParam->vRotate.x, pCreateParam->vRotate.y, pCreateParam->vRotate.z);

	m_rpLightType	= pCreateParam->rpLightType;
	m_bRealLight	= pCreateParam->bRealLight;
	m_bWorldLight	= pCreateParam->bWorldLight;

	//CNtlWorldRpLightManager::GetInstance()->RequestCreateRpLight(m_pLight, this);
	SetRealLight(pCreateParam->bRealLight); // SetRealLight 에서 위의 이벤트를 호출한다.

	if (pCreateParam->bDecal && pCreateParam->bRealLight)
	{
		CreateDecal();
	}
	
#ifdef dNTL_WORLD_TOOL_MODE
	switch (m_rpLightType)
	{
	case rpLIGHTDIRECTIONAL:
		m_pAtomicCtlPt = API_PL_LoadAtomic("object\\mesh\\ud6_c_f1_light_001.dff", "");
		break;
	case rpLIGHTPOINT:
		m_pAtomicCtlPt = API_PL_LoadAtomic("object\\mesh\\ud6_c_f1_light_001.dff", "");
		break;
	case rpLIGHTSPOT:
		m_pAtomicCtlPt = API_PL_LoadAtomic("object\\mesh\\world_light_box.dff", "");
		break;
	case rpLIGHTSPOTSOFT:
		m_pAtomicCtlPt = API_PL_LoadAtomic("object\\mesh\\world_light_sphere.dff", "");
		break;			
	}
	DBO_ASSERT(m_pAtomicCtlPt, "Resource load failed.");
#endif

	NTL_RETURN(TRUE);
}

void CNtlPLLightObject::Destroy()
{
	NTL_FUNCTION("CNtlPLLightObject::Destroy");

	if (m_pLight)
	{
		CNtlWorldRpLightManager::GetInstance()->RequestDestroyRpLight(m_pLight);

#ifdef dNTL_WORLD_TOOL_MODE
		if (m_pAtomicCtlPt)
		{
			API_PL_DeleteAtomic(m_pAtomicCtlPt);
			m_pAtomicCtlPt = NULL;
		}
#endif
		RwFrame* pFrame = RpLightGetFrame(m_pLight);
		RpLightSetFrame(m_pLight, NULL);
		if (pFrame)
		{
			RwFrameDestroy(pFrame);
		}
		RpLightDestroy(m_pLight);
		m_pLight = NULL;
	}

	DeleteDecal();

	NTL_RETURNVOID();
}

RwBool CNtlPLLightObject::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLLightObject::Update");

	NTL_RETURN(TRUE);
}

void CNtlPLLightObject::OnUpdate(RpLight* pLight, RwReal fElapsed)
{
	if (m_pLight)
	{		
		RwV3d			vCameraPos	= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
		RwV3d			vLightPos	= *RwMatrixGetPos(RwFrameGetMatrix(RpLightGetFrame(m_pLight)));
		RwV3d			vLightAt	= *RwMatrixGetAt(RwFrameGetMatrix(RpLightGetFrame(m_pLight)));
		static RwReal	fDistFar	= 90000.0f;/*(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize) * 2.0f) * *static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize) * 2.0f)*/;
		RwV3d			vDist		= vCameraPos - vLightPos;		
		RwReal			fDist		= CNtlMath::MathRwV3dSquaredLength(&vDist);
		if (fDistFar > fDist) 
		{		
			RwReal fRadius = GetRadius() * GetRadius();
			if (GetLightType() == rpLIGHTSPOT || GetLightType() == rpLIGHTSPOTSOFT)
			{	
				RwV3d v1, v2;
				RwV3dNormalize(&v1, &vDist);
				RwV3dNormalize(&v2, &vLightAt);
				 
				RwReal fAngle = (acosf(RwV3dDotProduct(&v1, &v2))) * (180.0f / D3DX_PI);
				fRadius = (m_fAngle - fAngle) / m_fAngle * fRadius;							
			}
			SetLightValue(fDist - fRadius);

#ifdef dNTL_WORLD_TOOL_MODE
			if (s_pPLLightObjectSelected == this)
			{
				SetLightValue(RwRealMINVAL);
			}
#endif
			CNtlWorldRpLightManager::GetInstance()->RequestAddRpLight(m_pLight, this);			
		}
		else
		{
			CNtlWorldRpLightManager::GetInstance()->RequestRemoveRpLight(m_pLight);
		}

		if (m_fInterpolationTime >= 0.0f)
		{
			m_fInterpolationTime -= fElapsed;
			if (m_fInterpolationTime < 0.0f)
			{
				m_fInterpolationTime	= -999.0f;
				m_fRadius[0]			= (m_eFadeType == 1 ? GetRadius() : 0.0f);
				if (m_fRadius[0] == 0.0f)
				{
					CNtlWorldRpLightManager::GetInstance()->RequestRemoveRpLight(m_pLight);
				}
			}
			else
			{
				if (m_eFadeType == 1)	// FADE IN
				{
					m_fRadius[0] = RpNtlWorldRealPropLerp(m_fRadius[1],	GetRadius(), m_fInterpolationTime, s_fPLLightObjectIpTimeIn);
				}
				else if (m_eFadeType == 2) // FADE OUT
				{
					m_fRadius[0] = RpNtlWorldRealPropLerp(m_fRadius[1], 0.0f, m_fInterpolationTime, s_fPLLightObjectIpTimeOut);
				}
			}
			RpLightSetRadius(m_pLight, m_fRadius[0]);
		}

		RwReal TFactor = -static_cast<RwReal>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff) / 255.0f;

		RwRGBAReal rgbaLight = m_rgbafColor;
		::API_PL_1BColorClamp(&rgbaLight.red, TFactor);
		::API_PL_1BColorClamp(&rgbaLight.green, TFactor);
		::API_PL_1BColorClamp(&rgbaLight.blue, TFactor);
		RpLightSetColor(m_pLight, &rgbaLight);
	}
}

void CNtlPLLightObject::OnAddRpLight(RpLight* pRpLight)
{
	if (m_bRealLight)
	{
		if (m_eFadeType != 1)
		{
			m_fRadius[1]		= m_fRadius[0];
			m_eFadeType			= 1;
			m_fInterpolationTime= s_fPLLightObjectIpTimeIn;
		}
	}
}

void CNtlPLLightObject::OnRemoveRpLight(RpLight* pRpLight)
{
	if (m_bRealLight)
	{
		m_fRadius[0]		= 0.0f;
		m_eFadeType			= 2;
		m_fInterpolationTime= -999.0f;

		if (m_pLight)
		{
			RpLightSetRadius(m_pLight, m_fRadius[0]);
		}
	}
}

void CNtlPLLightObject::OnRequestRemoveRpLight(RpLight* pRpLight)
{
	if (m_bRealLight)
	{
		if (m_eFadeType != 2)
		{
			m_fRadius[1]		= m_fRadius[0];
			m_eFadeType			= 2;
			m_fInterpolationTime= s_fPLLightObjectIpTimeOut;
		}
	}
}

void CNtlPLLightObject::SetMatrix(RwMatrix& matWorld)
{
	RwMatrixCopy(&m_matParant, &matWorld);
	SetTransform();
}

RwMatrix& CNtlPLLightObject::GetMatrix()
{
	if (m_pLight)
	{
		return *RwFrameGetMatrix(RpLightGetFrame(m_pLight));

	}
	return CNtlPLGlobal::m_matIden;
}


VOID CNtlPLLightObject::SetTransform()
{	
	if (!m_pLight)
	{
		return;
	}

	RwFrame*	pFrame	= RpLightGetFrame(m_pLight);
	RwMatrix*	pMatrix	= RwFrameGetMatrix(pFrame);
	RwMatrix	Matrix;
	RwV3d		Front, Left, Up;

	RwMatrixRotate(&Matrix, &CNtlPLGlobal::m_vYAxisV3, m_vRotate.y, rwCOMBINEREPLACE);
	RwV3dTransformVector(&Front, &CNtlPLGlobal::m_vZAxisV3, &Matrix);
	RwV3dTransformVector(&Left, &CNtlPLGlobal::m_vXAxisV3, &Matrix);

	RwMatrixRotate(&Matrix, &Left, m_vRotate.x, rwCOMBINEREPLACE);
	RwV3dTransformVector(&Front, &Front, &Matrix);
	RwV3dTransformVector(&Up, &CNtlPLGlobal::m_vYAxisV3, &Matrix);

	RwMatrixRotate(&Matrix, &CNtlPLGlobal::m_vZAxisV3, m_vRotate.z, rwCOMBINEREPLACE);        
	RwV3dTransformVector(&Left, &Left, &Matrix);
	RwV3dTransformVector(&Front, &Front, &Matrix);

	RwMatrixRotate(&Matrix, &CNtlPLGlobal::m_vYAxisV3, m_vRotate.y, rwCOMBINEREPLACE);
	RwMatrixRotate(&Matrix, &Left, m_vRotate.x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&Matrix, &Front, m_vRotate.z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&Matrix, &m_vPosition, rwCOMBINEPOSTCONCAT);

	if (GetAttachParent())
	{
		RwMatrixMultiply(pMatrix, &Matrix, &m_matParant);
	}
	else
	{
		RwMatrixCopy(pMatrix, &Matrix);
	}

	RwFrameUpdateObjects(pFrame);
	UpdateDecal();
}

void CNtlPLLightObject::SetPosition(const RwV3d *pPos)
{
	m_vPosition = *pPos;

	SetTransform();
}

void CNtlPLLightObject::SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle)
{
	m_vRotate.x = fXAngle;
	m_vRotate.y = fYAngle;
	m_vRotate.z = fZAngle;

	SetTransform();
}

VOID CNtlPLLightObject::SetRealLight(RwBool bBool)
{	
	if (m_pLight)
	{
		CNtlWorldRpLightManager::GetInstance()->RequestDestroyRpLight(m_pLight);
		if (bBool)
		{
			RpLightSetFlags(m_pLight, (rpLIGHTLIGHTWORLD | rpLIGHTLIGHTATOMICS));
		}
		else
		{
			DeleteDecal();

			m_bWorldLight = FALSE;
			
			RpLightSetFlags(m_pLight, 0);

			RpLightSetRadius(m_pLight, GetRadius());
		}
		CNtlWorldRpLightManager::GetInstance()->RequestCreateRpLight(m_pLight, this);
	}
	m_bRealLight = bBool;
}

VOID CNtlPLLightObject::SetRadius(RwReal fRadius)
{
	m_fRadius[1] = m_fRadius[0];
	m_fRadius[2] = fRadius;
	if (m_pLight)
	{
		RpLightSetRadius(m_pLight, m_fRadius[2]);
	}
}

VOID CNtlPLLightObject::SetAngle(RwReal fAngle)
{
	m_fAngle = fAngle;
	if (m_pLight)
	{
		RpLightSetConeAngle(m_pLight, m_fAngle / 180.0f * rwPI);
	}	
}

VOID CNtlPLLightObject::SetColor(RwRGBAReal& rgbafColor)
{
	m_rgbafColor = rgbafColor;
	
	if (m_pLight)
	{
		RpLightSetColor(m_pLight, &m_rgbafColor);
	}
	if (m_pNtlPLDecal)
	{
		m_pNtlPLDecal->SetColor(static_cast<RwUInt8>(m_rgbafColor.red * 255.0f),
								static_cast<RwUInt8>(m_rgbafColor.green * 255.0f),
								static_cast<RwUInt8>(m_rgbafColor.blue * 255.0f));
	}
}

void CNtlPLLightObject::CreateDecal()
{
	if (m_pNtlPLDecal)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLDecal);
		m_pNtlPLDecal = NULL;
	}

	if (m_rpLightType == rpLIGHTPOINT || m_rpLightType == rpLIGHTSPOT || m_rpLightType == rpLIGHTSPOTSOFT)
	{
		SPLDecalCreateParam createParam;
		createParam.fIntersectionRadius			= 1.0f;
		createParam.fScale						= 1.0f;	
		createParam.fVisibleSquaredDist			= 90000.0f;	
		createParam.pTexName					= "light_decal.png";
		createParam.pTexPath					= ".\\texture\\ntlwe\\light\\";
		createParam.fYOffset					= 0.01f;	
		createParam.eDecalType					= DECAL_TERRAIN;
		createParam.nFlagEffect					= E_FLAG_DECAL_RENDER_WORLD;

// 		if (m_rpLightType == rpLIGHTSPOT)
// 		{
// 			RpLightSetFlags(m_pLight, 0);
//  			createParam.nFlagEffect					= E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_RENDER_WORLD;
// 		}
// 		else
// 		{
// 			createParam.nFlagEffect					= E_FLAG_DECAL_RENDER_WORLD;
// 		}

		m_pNtlPLDecal = static_cast<CNtlPLDecal*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL, "NULL", &createParam));
		DBO_ASSERT(m_pNtlPLDecal, "entity create failed.");

		m_pNtlPLDecal->SetAlphaBlend(TRUE, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);    
		m_pNtlPLDecal->SetColor(static_cast<RwUInt8>(m_rgbafColor.red * 255.0f),
								static_cast<RwUInt8>(m_rgbafColor.green * 255.0f),
								static_cast<RwUInt8>(m_rgbafColor.blue * 255.0f));
	}
}

void CNtlPLLightObject::DeleteDecal()
{
	if (m_pNtlPLDecal)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLDecal);
		m_pNtlPLDecal = NULL;
	}
}

void CNtlPLLightObject::UpdateDecal()
{
	if (m_pNtlPLDecal && m_pLight)
	{
		if (RpLightGetWorld(m_pLight))
		{
			switch (m_rpLightType)
			{
			case rpLIGHTPOINT:
				UpdateDecalPoint();
				break;
			case rpLIGHTSPOT:
				UpdateDecalSpot();
				break;
			case rpLIGHTSPOTSOFT:
				UpdateDecalSoftSpot();
				break;
			}
		}
		else
		{
			m_pNtlPLDecal->SetVisible(FALSE);
		}
		
	}
}

void CNtlPLLightObject::UpdateDecalPoint()
{
	RwMatrix* pmat	= RwFrameGetMatrix(RpLightGetFrame(m_pLight));

	RwV3d* pvRight	= RwMatrixGetRight(pmat);
	RwV3d* pvUp		= RwMatrixGetUp(pmat);
	RwV3d* pvAt		= RwMatrixGetAt(pmat);
	RwV3d* pvPos	= RwMatrixGetPos(pmat);

	RwReal fRadius	= RpLightGetRadius(m_pLight);

	RwV3d  vPoint;
	vPoint.x = pvPos->x;
	vPoint.y = pvPos->y - fRadius;
	vPoint.z = pvPos->z;

	RwV3d vColl;
	if (GetSceneManager()->GetWorld()->Pick(*pvPos, vPoint, vColl))
	{
		m_pNtlPLDecal->SetVisible(TRUE);
		m_pNtlPLDecal->SetPosition(&vColl);

		fRadius = fRadius - (vPoint.y - vColl.y);
		m_pNtlPLDecal->SetSize(fRadius * 2.0f, fRadius * 2.0f);	
	}
	else
	{
		m_pNtlPLDecal->SetVisible(FALSE);
	}
}

void CNtlPLLightObject::UpdateDecalSpot()
{
	UpdateDecalSoftSpot();
}

void CNtlPLLightObject::UpdateDecalSoftSpot()
{
	RwMatrix* pmat	= RwFrameGetMatrix(RpLightGetFrame(m_pLight));

	RwV3d* pvRight	= RwMatrixGetRight(pmat);
	RwV3d* pvUp		= RwMatrixGetUp(pmat);
	RwV3d* pvAt		= RwMatrixGetAt(pmat);
	RwV3d* pvPos	= RwMatrixGetPos(pmat);

	RwV3d	vPoint;
	RwReal  fCosValue, fSinValue;
	RwV3d   vdRight, vdUp, vdAt;

	RwReal	fConAngle	= RpLightGetConeAngle(m_pLight);
	RwReal	fConeAngleD = (RwReal)RwCos(fConAngle);

	RwReal	fRadius		= RpLightGetRadius(m_pLight);

	RwInt32	iGenDecal = 0;
	RwBBox	bboxDecal;

	static RwInt32 iCollCount = 6;
	for (RwInt32 i = 0; i < iCollCount; ++i)
	{
		fCosValue = (RwReal)(RwCos(i * 2 * rwPI / iCollCount) * RwSin(fConAngle));
		fSinValue = (RwReal)(RwSin(i * 2 * rwPI / iCollCount) * RwSin(fConAngle));

		RwV3dScale(&vdUp, pvUp, fSinValue * fRadius);
		RwV3dScale(&vdRight, pvRight, fCosValue * fRadius);
		RwV3dScale(&vdAt, pvAt, fConeAngleD * fRadius);

		vPoint.x = pvPos->x + vdAt.x + vdUp.x + vdRight.x;
		vPoint.y = pvPos->y + vdAt.y + vdUp.y + vdRight.y;
		vPoint.z = pvPos->z + vdAt.z + vdUp.z + vdRight.z;

		RwV3d vColl;
		if (GetSceneManager()->GetWorld()->Pick(*pvPos, vPoint, vColl))
		{
			if (iGenDecal == 0)
			{
				RwBBoxInitialize(&bboxDecal, &vColl);
			}
			else
			{
				RwBBoxAddPoint(&bboxDecal, &vColl);
			}
			++iGenDecal;
		}
	}

	if (iGenDecal == iCollCount)
	{
		vPoint.x = (bboxDecal.sup.x + bboxDecal.inf.x) * 0.5f;
		vPoint.y = (bboxDecal.sup.y + bboxDecal.inf.y) * 0.5f;
		vPoint.z = (bboxDecal.sup.z + bboxDecal.inf.z) * 0.5f;

		m_pNtlPLDecal->SetVisible(TRUE);
		m_pNtlPLDecal->SetPosition(&vPoint);		

		RwReal fSizeX = bboxDecal.sup.x - bboxDecal.inf.x;
		RwReal fSizeZ = bboxDecal.sup.z - bboxDecal.inf.z;
		
		m_pNtlPLDecal->SetSize(bboxDecal.sup.x - bboxDecal.inf.x, bboxDecal.sup.z - bboxDecal.inf.z);	
	}
	else
	{
		m_pNtlPLDecal->SetVisible(FALSE);
	}
}

RwBool CNtlPLLightObject::IsEnabledDecal()
{
	if (m_pNtlPLDecal)
	{
		return TRUE;
	}
	return FALSE;
}

RwBool CNtlPLLightObject::IsEnabledWorldLight()
{
	return m_bWorldLight;
}

RwBool CNtlPLLightObject::GetWorldLightColorf(const RwV3d* pvPos, RwRGBAReal* pclrfOut, void* pUserData)
{
	if (m_pLight && m_bWorldLight)
	{
		if (RpLightGetWorld(m_pLight))
		{
			RwV3d			vLightPos	= *RwMatrixGetPos(RwFrameGetMatrix(RpLightGetFrame(m_pLight)));
			RwV3d			vLightAt	= *RwMatrixGetAt(RwFrameGetMatrix(RpLightGetFrame(m_pLight)));					

			RwReal fDist = RwV3dLength(&(vLightPos - *pvPos));
			if (fDist > GetRadius())
			{
				return FALSE;
			}

			if (m_rpLightType == rpLIGHTPOINT || m_rpLightType == rpLIGHTSPOT || m_rpLightType == rpLIGHTSPOTSOFT)
			{
				if (m_rpLightType == rpLIGHTSPOT || m_rpLightType == rpLIGHTSPOTSOFT)
				{										
					RwV3d vDist = *pvPos - vLightPos;

					RwV3d v1, v2;
					RwV3dNormalize(&v1, &vDist);
					RwV3dNormalize(&v2, &vLightAt);

					RwReal fDot		= acosf(RwV3dDotProduct(&v1, &v2));
					RwReal fAngle	= RpLightGetConeAngle(m_pLight);
					
					if (fAngle < fDot)
					{
						return FALSE;
					}
				}
				const RwRGBAReal* pClrf = (RwRGBAReal*)pUserData;

				RwReal fDelta = 0.0f;
				//if (m_rpLightType == rpLIGHTSPOTSOFT)
				{
					fDelta = fDist / GetRadius();
				}

				pclrfOut->red	= m_rgbafColor.red + (fDelta * (pClrf->red - m_rgbafColor.red));
				pclrfOut->green	= m_rgbafColor.green + (fDelta * (pClrf->green - m_rgbafColor.green));
				pclrfOut->blue	= m_rgbafColor.blue + (fDelta * (pClrf->blue - m_rgbafColor.blue));

				CLAMP(pclrfOut->red, 0.0f, 1.0f);
				CLAMP(pclrfOut->green, 0.0f, 1.0f);
				CLAMP(pclrfOut->blue, 0.0f, 1.0f);

				return TRUE;

			}
		}
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef dNTL_WORLD_TOOL_MODE
CNtlPLLightObject*	CNtlPLLightObject::s_pPLLightObjectSelected = NULL;
RwBool				CNtlPLLightObject::s_bVisibleLine			= TRUE;
RwBool				CNtlPLLightObject::s_bVisiblePlane			= FALSE;
RwReal				CNtlPLLightObject::s_fDummyScale			= 1.0f;


void CNtlPLLightObject::SetToolUI(CNtlPLLightObject* pPLLightObject)
{
	s_pPLLightObjectSelected = pPLLightObject;
}

void CNtlPLLightObject::SetVisibleUI(RwBool bLine, RwBool bPlane)
{
	s_bVisibleLine	= bLine;
	s_bVisiblePlane	= bPlane;
}

void CNtlPLLightObject::GetVisibleUI(RwBool& bLine, RwBool& bPlane)
{
	bLine	= s_bVisibleLine;
	bPlane	= s_bVisiblePlane;
}

void CNtlPLLightObject::SetDummyScale(RwReal fScale)
{
	s_fDummyScale = fScale;	
}

RwReal CNtlPLLightObject::GetDummyScale()
{
	return s_fDummyScale;
}
#endif

RwBool CNtlPLLightObject::Render()
{
	NTL_FUNCTION("CNtlPLLightObject::Render");

	NTL_RETURN(TRUE);
}

void CNtlPLLightObject::OnRender(RpLight* pLight)
{
#ifdef dNTL_WORLD_TOOL_MODE

	RwRGBA rgbaColor;
// 	if (GetAttachParent())
// 	{
// 		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) FALSE);
// 		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
// 
// 		rgbaColor.alpha = 255; rgbaColor.red = 0; rgbaColor.green = 0; rgbaColor.blue = 255;
// 
// 		RenderGeometryBillboardSphere(&m_matParant, &rgbaColor, 1.0f, CNtlPLGlobal::m_RwCamera, TRUE);
// 
// 		RwIm3DVertex	aImLine[2];
// 		RwV3d*			pvPosLine = NULL;
// 
// 		pvPosLine = RwMatrixGetPos(&m_matParant);
// 		RwIm3DVertexSetRGBA(&(aImLine[0]), rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
// 		RwIm3DVertexSetPos(&(aImLine[0]), pvPosLine->x, pvPosLine->y, pvPosLine->z);
// 
// 		pvPosLine = RwMatrixGetPos(RwFrameGetMatrix(RpLightGetFrame(m_pLight)));
// 		RwIm3DVertexSetRGBA(&(aImLine[1]), rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
// 		RwIm3DVertexSetPos(&(aImLine[1]), pvPosLine->x, pvPosLine->y, pvPosLine->z);
// 
// 		RwD3D9SetTexture(NULL, 0);
// 		if(RwIm3DTransform(aImLine, 2, NULL, rwIM3D_ALLOPAQUE))
// 		{
// 			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);
// 			RwIm3DEnd();
// 		}
// 
// 		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
// 		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
// 	}

	BeginLightState(FALSE);
	BeginFogState(FALSE);	
	
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
	
	RwRGBAFromRwRGBAReal(&rgbaColor, &m_rgbafColor);
	rgbaColor.alpha = 128;

	if (s_bVisibleLine && RpLightGetWorld(m_pLight))
	{
		switch (m_rpLightType)
		{
		case rpLIGHTDIRECTIONAL:
			RenderGeometryArrow(RwFrameGetMatrix(RpLightGetFrame(m_pLight)), &rgbaColor, GetRadius(), FALSE);
			break;
		case rpLIGHTPOINT:
			RenderGeometryBillboardSphere(RwFrameGetMatrix(RpLightGetFrame(m_pLight)), &rgbaColor, GetRadius(),
				CNtlPLGlobal::m_RwCamera, !(s_bVisiblePlane && s_pPLLightObjectSelected == this));
			break;
		case rpLIGHTSPOT:			
		case rpLIGHTSPOTSOFT:
			RenderGeometryCone(RwFrameGetMatrix(RpLightGetFrame(m_pLight)), &rgbaColor, RpLightGetConeAngle(m_pLight),
				GetRadius(), !(s_bVisiblePlane && s_pPLLightObjectSelected == this));
			break;			
		}
	}

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

	rgbaColor.alpha = 255;
	if (m_bRealLight)
	{
		if (m_fRealLightColorTime < 0.0f)
		{
			m_bRealLightColor		= !m_bRealLightColor;
			m_fRealLightColorTime	= 0.5f;
		}
		else
		{
			m_fRealLightColorTime -= g_GetElapsedTime(); 
		}

		if (!m_bRealLightColor)
		{
			rgbaColor.red = rgbaColor.blue = rgbaColor.green = 100;
		}
	}
	RpGeometry *pGeometry = RpAtomicGetGeometry(m_pAtomicCtlPt);
	for (int i = 0; i < RpGeometryGetNumMaterials(pGeometry); ++i)
	{
		RpMaterial *pMaterial = RpGeometryGetMaterial(pGeometry, i);
		RpMaterialSetColor(pMaterial, &rgbaColor);
	}
	
	//RwFrameTransform(RpAtomicGetFrame(m_pAtomicCtlPt), RwFrameGetMatrix(RpLightGetFrame(m_pLight)), rwCOMBINEREPLACE);

	RwV3d vScaleDummy;
	vScaleDummy.x = vScaleDummy.y = vScaleDummy.z = s_fDummyScale;
	RwMatrixScale(RwFrameGetMatrix(RpAtomicGetFrame(m_pAtomicCtlPt)), &vScaleDummy, rwCOMBINEREPLACE);
	RwMatrixTranslate(RwFrameGetMatrix(RpAtomicGetFrame(m_pAtomicCtlPt)), RwMatrixGetPos(RwFrameGetMatrix(RpLightGetFrame(m_pLight))), rwCOMBINEPOSTCONCAT);
	RwFrameUpdateObjects(RpAtomicGetFrame(m_pAtomicCtlPt));

	RwUInt32 uiFlags = RpGeometryGetFlags(pGeometry);
	RpGeometrySetFlags(pGeometry, ((uiFlags | rpGEOMETRYMODULATEMATERIALCOLOR) & ~(rpGEOMETRYLIGHT)));
	RpAtomicRender(m_pAtomicCtlPt);

	if (s_pPLLightObjectSelected == this)
	{
		rgbaColor.red = 0; rgbaColor.green = 0; rgbaColor.blue = 0;
		API_PL_RenderWireMesh(m_pAtomicCtlPt, &rgbaColor);
	}

	RpGeometrySetFlags(pGeometry, uiFlags);

	EndLightState();
	EndFogState();	

#endif
}

static RpCollisionTriangle* RtIntersectionAtomic( RpIntersection *pIntersection, RpCollisionTriangle *pCollTriangle, RwReal distance, void *data)
{
	RwReal* fDist = (RwReal*)data;
	if (*fDist - distance > 0.0001f)
	{
		 *fDist = distance;
	}

	return pCollTriangle;
}

#ifdef dNTL_WORLD_TOOL_MODE
RwBool CNtlPLLightObject::Pick(RwReal& fDist)
{
	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;
	
	RpIntersection intersection;
	intersection.type = rpINTERSECTLINE;
	
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &intersection.t.line, &ScreenPos);

	//RwFrameTransform(RpAtomicGetFrame(m_pAtomicCtlPt), RwFrameGetMatrix(RpLightGetFrame(m_pLight)), rwCOMBINEREPLACE);

	RwV3d vScaleDummy;
	vScaleDummy.x = vScaleDummy.y = vScaleDummy.z = s_fDummyScale;
	RwMatrixScale(RwFrameGetMatrix(RpAtomicGetFrame(m_pAtomicCtlPt)), &vScaleDummy, rwCOMBINEREPLACE);
	RwMatrixTranslate(RwFrameGetMatrix(RpAtomicGetFrame(m_pAtomicCtlPt)), RwMatrixGetPos(RwFrameGetMatrix(RpLightGetFrame(m_pLight))), rwCOMBINEPOSTCONCAT);
	RwFrameUpdateObjects(RpAtomicGetFrame(m_pAtomicCtlPt));
	
	RwReal fDistTemp = RwRealMAXVAL;
	if (RpAtomicForAllIntersections(m_pAtomicCtlPt, &intersection, RtIntersectionAtomic, &fDistTemp))
	{
		if (fDistTemp == RwRealMAXVAL)
		{
			return FALSE;
		}

		RwV3d vTemp;
		RwV3dSubMacro( &vTemp, &intersection.t.line.end, &intersection.t.line.start);
		RwV3dScale(&vTemp, &vTemp, fDistTemp);
		fDistTemp = RwV3dLength(&vTemp);
		if (fDistTemp < fDist)
		{
			fDist = fDistTemp;
			return TRUE;
		}
	}
	
	return FALSE;
}
#endif

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlPLLightObject::SaveSwapFile(RwReal x, RwReal y, RwReal z)
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

		if(_chdir("ld") == -1)
		{
			_mkdir("ld");
			_chdir("ld");
		}

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ld", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iLightObjectNum = static_cast<RwInt32>(pSector->m_vecNtlPLEntityLightObjectDummy.size());
		fwrite(&iLightObjectNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iLightObjectNum; ++i)
		{
			CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pSector->m_vecNtlPLEntityLightObjectDummy.at(i);

			RpLightType eType;
			eType = pLightObject->GetLightType();
			fwrite(&eType, sizeof(RpLightType), 1, pFile);

			RwBool bTemp;
			bTemp = pLightObject->IsRealLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			RwV3d vTemp;
			vTemp = pLightObject->GetPosition();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = *pLightObject->GetRotate();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			RwRGBAReal rgbafTemp;
			rgbafTemp = pLightObject->GetColor();
			fwrite(&rgbafTemp, sizeof(RwRGBAReal), 1, pFile);

			RwReal fTemp;
			fTemp = pLightObject->GetRadius();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			fTemp = pLightObject->GetAngle();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			// CZ-LO
			bTemp = pLightObject->IsEnabledDecal();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			bTemp = pLightObject->IsEnabledWorldLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlPLLightObject::LoadSwapFile(RwReal x, RwReal y, RwReal z)
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
		_chdir("ld");

		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ld", iSectorIdx);

		FILE* pFile;
		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

		pSector->m_vecNtlPLEntityLightObjectDummy.clear();

		RwInt32 iLightObjectNum;
		fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iLightObjectNum; ++i)
		{
			SPLLightObjectCreateParam CreateParam;

			fread(&CreateParam.rpLightType, sizeof(RpLightType), 1, pFile);
			fread(&CreateParam.bRealLight, sizeof(RwBool), 1, pFile);

			RwV3d vPos;
			fread(&vPos, sizeof(RwV3d), 1, pFile);
			CreateParam.pPos = &vPos;

			fread(&CreateParam.vRotate, sizeof(RwV3d), 1, pFile);
			fread(&CreateParam.rgbafColor, sizeof(RwRGBAReal), 1, pFile);
			fread(&CreateParam.fRadius, sizeof(RwReal), 1, pFile);
			fread(&CreateParam.fAngle, sizeof(RwReal), 1, pFile);

			// CZ-LO
			//CreateParam.bDecal		= FALSE;
			//CreateParam.bWorldLight = FALSE;
 			fread(&CreateParam.bDecal, sizeof(RwBool), 1, pFile);
 			fread(&CreateParam.bWorldLight, sizeof(RwBool), 1, pFile);

			CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLLightObject, "Entity create failed.");

			pSector->m_vecNtlPLEntityLightObjectDummy.push_back(pNtlPLLightObject);
		}

		fclose(pFile);
	}
}
#endif