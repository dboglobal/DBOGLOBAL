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
#include "NtlPLLightIndoor.h"

#ifdef dNTL_WORLD_TOOL_MODE
RwReal CNtlPLLightIndoor::s_fPLLightIndoorIpTimeIn	= 0.01f;
RwReal CNtlPLLightIndoor::s_fPLLightIndoorIpTimeOut	= 0.01f;
#else
RwReal CNtlPLLightIndoor::s_fPLLightIndoorIpTimeIn	= 0.5f;
RwReal CNtlPLLightIndoor::s_fPLLightIndoorIpTimeOut	= 0.25f;
#endif

CNtlPLLightIndoor::CNtlPLLightIndoor()
:m_fInterpolationTime(-999.0f)
,m_eFadeType(0)
,m_rpLightType(rpNALIGHTTYPE)
,m_bRealLight(FALSE)
,m_pLight(NULL)
,m_fAngle(0.0f)
{
#ifdef dNTL_WORLD_TOOL_MODE
	m_pAtomicCtlPt			= NULL;
	m_bRealLightColor		= FALSE;
	m_fRealLightColorTime	= 0.5f;
#endif
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOTUPDATE | NTL_PLEFLAG_NOT_VISIBLE);
	SetLayer(PLENTITY_LAYER_LIGHT);
	SetClassType(PLENTITY_LIGHT_INDOOR);

	m_fRadius[0] = m_fRadius[1] = m_fRadius[2] = 0.0f;
}

CNtlPLLightIndoor::~CNtlPLLightIndoor()
{
}

RwBool CNtlPLLightIndoor::SetProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLLightIndoor::SetProperty");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLLightIndoor::Create(const SPLEntityCreateParam *pParam)
{
	NTL_FUNCTION("CNtlPLLightIndoor::Create");

	SPLLightIndoorCreateParam* pCreateParam = (SPLLightIndoorCreateParam*)(pParam);

	m_pLight = RpLightCreate(pCreateParam->rpLightType);
	DBO_ASSERT(m_pLight, "Light create failed.");
	
	SetRadius(pCreateParam->fRadius);
	SetAngle(pCreateParam->fAngle);
	SetColor(pCreateParam->rgbafColor);

	RwFrame* pFrame = RwFrameCreate();
	DBO_ASSERT(pFrame, "Frame create failed.");

	RwFrameSetIdentity(pFrame);
	RpLightSetFrame(m_pLight, pFrame);

	SetPosition(pCreateParam->pPos);
	SetRotate(pCreateParam->vRotate.x, pCreateParam->vRotate.y, pCreateParam->vRotate.z);

	//CNtlWorldRpLightManager::GetInstance()->RequestCreateRpLight(m_pLight, this);
	SetRealLight(pCreateParam->bRealLight); // SetRealLight 에서 위의 이벤트를 호출한다.


	m_rpLightType	= pCreateParam->rpLightType;
	m_bRealLight	= pCreateParam->bRealLight;

#ifdef dNTL_WORLD_TOOL_MODE
	switch (m_rpLightType)
	{
	case rpLIGHTDIRECTIONAL:
		m_pAtomicCtlPt = API_PL_LoadAtomic("tool/saber/mesh/Light_Diamond.dff", "");
		break;
	case rpLIGHTPOINT:
		m_pAtomicCtlPt = API_PL_LoadAtomic("tool/saber/mesh/Light_Diamond.dff", "");
		break;
	case rpLIGHTSPOT:
		m_pAtomicCtlPt = API_PL_LoadAtomic("tool/saber/mesh/Light_Box.dff", "");
		break;
	case rpLIGHTSPOTSOFT:
		m_pAtomicCtlPt = API_PL_LoadAtomic("tool/saber/mesh/Light_Sphere.dff", "");
		break;			
	}
	DBO_ASSERT(m_pAtomicCtlPt, "Resource load failed.");
#endif

	NTL_RETURN(TRUE);
}

void CNtlPLLightIndoor::Destroy()
{
	NTL_FUNCTION("CNtlPLLightIndoor::Destroy");

	if (m_pLight)
	{
		if (CNtlWorldRpLightManager::GetInstance())
		{
			CNtlWorldRpLightManager::GetInstance()->RequestDestroyRpLight(m_pLight);
		}

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

	NTL_RETURNVOID();
}

RwBool CNtlPLLightIndoor::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLLightIndoor::Update");

	NTL_RETURN(TRUE);
}

void CNtlPLLightIndoor::OnUpdate(RpLight* pLight, RwReal fElapsed)
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
			if (s_pPLLightIndoorSelected == this)
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
					m_fRadius[0] = RpNtlWorldRealPropLerp(m_fRadius[1],	GetRadius(), m_fInterpolationTime, s_fPLLightIndoorIpTimeIn);
				}
				else if (m_eFadeType == 2) // FADE OUT
				{
					m_fRadius[0] = RpNtlWorldRealPropLerp(m_fRadius[1], 0.0f, m_fInterpolationTime, s_fPLLightIndoorIpTimeOut);
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

void CNtlPLLightIndoor::OnAddRpLight(RpLight* pRpLight)
{
	if (m_bRealLight)
	{
		if (m_eFadeType != 1)
		{
			m_fRadius[1]		= m_fRadius[0];
			m_eFadeType			= 1;
			m_fInterpolationTime= s_fPLLightIndoorIpTimeIn;
		}
	}
}

void CNtlPLLightIndoor::OnRemoveRpLight(RpLight* pRpLight)
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

void CNtlPLLightIndoor::OnRequestRemoveRpLight(RpLight* pRpLight)
{
	if (m_bRealLight)
	{
		if (m_eFadeType != 2)
		{
			m_fRadius[1]		= m_fRadius[0];
			m_eFadeType			= 2;
			m_fInterpolationTime= s_fPLLightIndoorIpTimeOut;
		}
	}
}

VOID CNtlPLLightIndoor::SetTransform()
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

	RwMatrixSetIdentity(pMatrix);
	RwMatrixRotate(pMatrix, &CNtlPLGlobal::m_vYAxisV3, m_vRotate.y, rwCOMBINEREPLACE);
	RwMatrixRotate(pMatrix, &Left, m_vRotate.x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(pMatrix, &Front, m_vRotate.z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(pMatrix, &m_vPosition, rwCOMBINEPOSTCONCAT);

	RwFrameUpdateObjects(pFrame);
}

void CNtlPLLightIndoor::SetPosition(const RwV3d *pPos)
{
	m_vPosition = *pPos;

	SetTransform();
}

void CNtlPLLightIndoor::SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle)
{
	m_vRotate.x = fXAngle;
	m_vRotate.y = fYAngle;
	m_vRotate.z = fZAngle;

	SetTransform();
}

VOID CNtlPLLightIndoor::SetRealLight(RwBool bBool)
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
			RpLightSetFlags(m_pLight, 0);
			RpLightSetRadius(m_pLight, GetRadius());
		}
		CNtlWorldRpLightManager::GetInstance()->RequestCreateRpLight(m_pLight, this);
	}
	m_bRealLight = bBool;
}

VOID CNtlPLLightIndoor::SetRadius(RwReal fRadius)
{
	m_fRadius[1] = m_fRadius[0];
	m_fRadius[2] = fRadius;
	if (m_pLight)
	{
		RpLightSetRadius(m_pLight, m_fRadius[2]);
	}
}

VOID CNtlPLLightIndoor::SetAngle(RwReal fAngle)
{
	m_fAngle = fAngle;
	if (m_pLight)
	{
		RpLightSetConeAngle(m_pLight, m_fAngle / 180.0f * rwPI);
	}	
}

VOID CNtlPLLightIndoor::SetColor(RwRGBAReal& rgbafColor)
{
	m_rgbafColor = rgbafColor;
	
	if (m_pLight)
	{
		RpLightSetColor(m_pLight, &m_rgbafColor);
	}
}

#ifdef dNTL_WORLD_TOOL_MODE
CNtlPLLightIndoor*	CNtlPLLightIndoor::s_pPLLightIndoorSelected = NULL;
RwBool				CNtlPLLightIndoor::s_bVisibleLine			= TRUE;
RwBool				CNtlPLLightIndoor::s_bVisiblePlane			= FALSE;


void CNtlPLLightIndoor::SetToolUI(CNtlPLLightIndoor* pPLLightIndoor)
{
	s_pPLLightIndoorSelected = pPLLightIndoor;
}

void CNtlPLLightIndoor::SetVisibleUI(RwBool bLine, RwBool bPlane)
{
	s_bVisibleLine	= bLine;
	s_bVisiblePlane	= bPlane;
}

void CNtlPLLightIndoor::GetVisibleUI(RwBool& bLine, RwBool& bPlane)
{
	bLine	= s_bVisibleLine;
	bPlane	= s_bVisiblePlane;
}
#endif

RwBool CNtlPLLightIndoor::Render()
{
	NTL_FUNCTION("CNtlPLLightIndoor::Render");

	NTL_RETURN(TRUE);
}

void CNtlPLLightIndoor::OnRender(RpLight* pLight)
{
#ifdef dNTL_WORLD_TOOL_MODE
	BeginLightState(FALSE);
	BeginFogState(FALSE);	
	
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

	RwRGBA rgbaColor;
	RwRGBAFromRwRGBAReal(&rgbaColor, &m_rgbafColor);
	rgbaColor.alpha = 128;

	if (s_bVisibleLine && RpLightGetWorld(m_pLight))
	{
		switch (m_rpLightType)
		{
		case rpLIGHTDIRECTIONAL:
			RenderGeometryArrow(RwFrameGetLTM(RpLightGetFrame(m_pLight)), &rgbaColor, GetRadius(), FALSE);
			break;
		case rpLIGHTPOINT:
			RenderGeometryBillboardSphere(RwFrameGetLTM(RpLightGetFrame(m_pLight)), &rgbaColor, GetRadius(),
				CNtlPLGlobal::m_RwCamera, !(s_bVisiblePlane && s_pPLLightIndoorSelected == this));
			break;
		case rpLIGHTSPOT:			
		case rpLIGHTSPOTSOFT:
			RenderGeometryCone(RwFrameGetLTM(RpLightGetFrame(m_pLight)), &rgbaColor, RpLightGetConeAngle(m_pLight),
				GetRadius(), !(s_bVisiblePlane && s_pPLLightIndoorSelected == this));
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
	RwFrameTransform(RpAtomicGetFrame(m_pAtomicCtlPt), RwFrameGetMatrix(RpLightGetFrame(m_pLight)), rwCOMBINEREPLACE);

	RwUInt32 uiFlags = RpGeometryGetFlags(pGeometry);
	RpGeometrySetFlags(pGeometry, ((uiFlags | rpGEOMETRYMODULATEMATERIALCOLOR) & ~(rpGEOMETRYLIGHT)));
	RpAtomicRender(m_pAtomicCtlPt);

	if (s_pPLLightIndoorSelected == this)
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
RwBool CNtlPLLightIndoor::Pick(RwReal& fDist)
{
	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;
	
	RpIntersection intersection;
	intersection.type = rpINTERSECTLINE;
	
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &intersection.t.line, &ScreenPos);

	RwFrameTransform(RpAtomicGetFrame(m_pAtomicCtlPt), RwFrameGetMatrix(RpLightGetFrame(m_pLight)), rwCOMBINEREPLACE);
	
	RwReal fDistTemp = RwRealMAXVAL;
	if (RpAtomicForAllIntersections(m_pAtomicCtlPt, &intersection, RtIntersectionAtomic, &fDistTemp))
	{
		if (fDistTemp == RwRealMAXVAL)
		{
			return FALSE;
		}

		RwV3d vTemp;
		RwV3dSub( &vTemp, &intersection.t.line.end, &intersection.t.line.start);
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
void CNtlPLLightIndoor::SaveSwapFile(RwReal x, RwReal y, RwReal z)
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

		RwInt32 iLightIndoorNum = static_cast<RwInt32>(pSector->m_vecNtlPLEntityIndoorLightDummy.size());
		fwrite(&iLightIndoorNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iLightIndoorNum; ++i)
		{
			CNtlPLLightIndoor* pLightIndoor = (CNtlPLLightIndoor*)pSector->m_vecNtlPLEntityIndoorLightDummy.at(i);

			RpLightType eType;
			eType = pLightIndoor->GetLightType();
			fwrite(&eType, sizeof(RpLightType), 1, pFile);

			RwBool bTemp;
			bTemp = pLightIndoor->IsRealLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			RwV3d vTemp;
			vTemp = pLightIndoor->GetPosition();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = *pLightIndoor->GetRotate();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			RwRGBAReal rgbafTemp;
			rgbafTemp = pLightIndoor->GetColor();
			fwrite(&rgbafTemp, sizeof(RwRGBAReal), 1, pFile);

			RwReal fTemp;
			fTemp = pLightIndoor->GetRadius();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			fTemp = pLightIndoor->GetAngle();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);
		}

		fclose(pFile);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlPLLightIndoor::LoadSwapFile(RwReal x, RwReal y, RwReal z)
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

		pSector->m_vecNtlPLEntityIndoorLightDummy.clear();

		RwInt32 iLightIndoorNum;
		fread(&iLightIndoorNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iLightIndoorNum; ++i)
		{
			SPLLightIndoorCreateParam CreateParam;

			fread(&CreateParam.rpLightType, sizeof(RpLightType), 1, pFile);
			fread(&CreateParam.bRealLight, sizeof(RwBool), 1, pFile);

			RwV3d vPos;
			fread(&vPos, sizeof(RwV3d), 1, pFile);
			CreateParam.pPos = &vPos;

			fread(&CreateParam.vRotate, sizeof(RwV3d), 1, pFile);
			fread(&CreateParam.rgbafColor, sizeof(RwRGBAReal), 1, pFile);
			fread(&CreateParam.fRadius, sizeof(RwReal), 1, pFile);
			fread(&CreateParam.fAngle, sizeof(RwReal), 1, pFile);

			CNtlPLLightIndoor* pNtlPLLightIndoor = static_cast<CNtlPLLightIndoor*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_INDOOR, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLLightIndoor, "Entity create failed.");

			pSector->m_vecNtlPLEntityIndoorLightDummy.push_back(pNtlPLLightIndoor);
		}

		fclose(pFile);
	}
}
#endif