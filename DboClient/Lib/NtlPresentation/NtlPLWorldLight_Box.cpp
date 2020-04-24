#include "precomp_ntlpresentation.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"

#include "NtlPLWorldLight_Box.h"

CNtlPLWorldLight_Box::CNtlPLWorldLight_Box()
{
	for (int i = 0; i < 6; ++i)
	{
		m_Plane[i].Normalize();
	}

	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
	m_vScale.z = 1.0f;

	m_vRotate.x = 0.0f;
	m_vRotate.y = 0.0f;
	m_vRotate.z = 0.0f;
}

CNtlPLWorldLight_Box::~CNtlPLWorldLight_Box()
{
}

RwBool CNtlPLWorldLight_Box::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLWorldLight_Box::Create");

	SPLWorldLightCreateParam_Box* pCreateParam = (SPLWorldLightCreateParam_Box*)pParam;

#ifdef dNTL_WORLD_TOOL_MODE

	if (dGET_CUR_PALETTE() == ePM_WORLDLIGHT)
	{
		m_pNtlPLObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "WORLD_LIGHT_BOX", pParam));
		DBO_ASSERT(m_pNtlPLObj, "Entity create failed.");
		
		m_pNtlPLObj->SetFadeEnable(FALSE);

		RpNtlClumpSetData(m_pNtlPLObj->GetClump(), this);
	}

#endif

	SetRotate(&pCreateParam->vRotate);
	SetScaleVector(&pCreateParam->vScale);

	SetPosition(pCreateParam->pPos);	
	SetColor(pCreateParam->uiRed, pCreateParam->uiGreen, pCreateParam->uiBlue);

	NTL_RETURN(TRUE);
}

void CNtlPLWorldLight_Box::Destroy()
{
	NTL_FUNCTION("CNtlPLWorldLight_Box::Destroy");

#ifdef dNTL_WORLD_TOOL_MODE

	if(m_pNtlPLObj)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLObj);
		m_pNtlPLObj = NULL;
	}

#endif

	NTL_RETURNVOID();
}

RwBool CNtlPLWorldLight_Box::GetWorldLightColorf(const RwV3d* pvPos, RwRGBAReal* pclrfOut, void* pUserData)
{
	RwReal fDist;
	if (!IsIn(pvPos, &fDist))
	{
		return FALSE;
	}

	*pclrfOut = m_clrfLight;

	return TRUE;
}

void CNtlPLWorldLight_Box::SetPosition(const RwV3d* pvPos)
{
	CNtlPLWorldLight_Base::SetPosition(pvPos);
	CalcWorldLightBoxData();
}

void CNtlPLWorldLight_Box::SetRotate(const RwV3d* pvRot)
{
	m_vRotate = *pvRot;

	CalcWorldLightBoxData();
}

RwV3d* CNtlPLWorldLight_Box::GetRotate()
{
	return &m_vRotate;
}

void CNtlPLWorldLight_Box::SetScaleVector(const RwV3d* pvScale)
{
	m_vScale = *pvScale;

	CalcWorldLightBoxData();

	RwV3d vHalfScale;
	RwV3dScale(&vHalfScale, &m_vScale, 0.5f);
	
	CNtlPLWorldLight_Base::SetRadius(RwV3dLength(&vHalfScale));
}

RwV3d* CNtlPLWorldLight_Box::GetScaleVector()
{
	return &m_vScale;
}

void CNtlPLWorldLight_Box::CalcWorldLightBoxData()
{
	RwMatrix		matTemp;
	RwV3d			vFront, vLeft, vUp;

	RwMatrixRotate(&matTemp, &CNtlPLGlobal::m_vYAxisV3, m_vRotate.y, rwCOMBINEREPLACE);

	RwV3dTransformVector(&vFront, &CNtlPLGlobal::m_vZAxisV3, &matTemp);
	RwV3dTransformVector(&vLeft, &CNtlPLGlobal::m_vXAxisV3, &matTemp);

	RwMatrixRotate(&matTemp, &vLeft, m_vRotate.x, rwCOMBINEREPLACE);

	RwV3dTransformVector(&vFront, &vFront, &matTemp);
	RwV3dTransformVector(&vUp, &CNtlPLGlobal::m_vYAxisV3, &matTemp);

	RwMatrixScale(&matTemp, &m_vScale, rwCOMBINEREPLACE);
	RwMatrixRotate(&matTemp, &CNtlPLGlobal::m_vYAxisV3, m_vRotate.y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matTemp, &vLeft, m_vRotate.x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matTemp, &vFront, m_vRotate.z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&matTemp, &m_Sphere.center, rwCOMBINEPOSTCONCAT);

	static RwV3d vDir[6] = { // 0:Front 1:Back 2:Left 3:Right 4:Up 5:Down
							{0.0f, 0.0f, 0.5f},
							{0.0f, 0.0f, -0.5f},
							{0.5f, 0.0f, 0.0f},
							{-0.5f, 0.0f, 0.0f},
							{0.0f, 0.5f, 0.0f},
							{0.0f, -0.5f, 0.0f},
						};

	for (int i = 0; i < 6; ++i)
	{
		RwV3d vTempPos;
		RwV3d vTempDir;
		RwV3dTransformPoint(&vTempPos, &vDir[i], &matTemp);

		RwV3dSubMacro(&vTempDir, &m_Sphere.center, &vTempPos);
		RwV3dNormalize(&vTempDir, &vTempDir);
		m_Plane[i].PlaneFromPointNormal(&vTempPos, &vTempDir);
	}

#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pNtlPLObj)
	{
		m_pNtlPLObj->SetScale(&m_vScale);
		m_pNtlPLObj->SetRotate(m_vRotate.x, m_vRotate.y, m_vRotate.z);
	}
#endif
}

RwBool CNtlPLWorldLight_Box::IsIn(const RwV3d* pvPos, RwReal* pfOutDistance)
{
	if (!CNtlPLWorldLight_Base::IsIn(pvPos, pfOutDistance))
	{
		return FALSE;
	}

	for (int i = 0; i < 6; ++i)
	{
		if (RwV3dDotProduct(pvPos, &m_Plane[i].GetNormal()) + m_Plane[i].GetConstant() < 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CNtlPLWorldLight_Box::SetVision(RwBool _Flag)
{
#ifdef dNTL_WORLD_TOOL_MODE

	if(_Flag)
	{
		if(!m_pNtlPLObj)
		{
			SPLEntityCreateParam Param;
			Param.pPos = &m_Sphere.center;

			m_pNtlPLObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "WORLD_LIGHT_BOX", &Param));
			DBO_ASSERT(m_pNtlPLObj, "Entity create failed.");

			m_pNtlPLObj->SetFadeEnable(FALSE);

			RpNtlClumpSetData(m_pNtlPLObj->GetClump(), this);

			m_pNtlPLObj->SetPosition(&m_Sphere.center);
			m_pNtlPLObj->SetScale(&m_vScale);
			m_pNtlPLObj->SetRotate(m_vRotate.x, m_vRotate.y, m_vRotate.z);
		}
	}
	else
	{
		if(m_pNtlPLObj)
		{
			GetSceneManager()->DeleteEntity(m_pNtlPLObj);
			m_pNtlPLObj = NULL;
		}
	}

#endif
}