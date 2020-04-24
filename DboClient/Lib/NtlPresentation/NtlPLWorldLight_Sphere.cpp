#include "precomp_ntlpresentation.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"

#include "NtlFieldColorManager.h"
#include "NtlPLWorldLight_Sphere.h"

CNtlPLWorldLight_Sphere::CNtlPLWorldLight_Sphere()
{
}

CNtlPLWorldLight_Sphere::~CNtlPLWorldLight_Sphere()
{
}

RwBool CNtlPLWorldLight_Sphere::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLWorldLight_Sphere::Create");

	SPLWorldLightCreateParam_Sphere* pCreateParam = (SPLWorldLightCreateParam_Sphere*)pParam;

#ifdef dNTL_WORLD_TOOL_MODE

	if (dGET_CUR_PALETTE() == ePM_WORLDLIGHT)
	{
		m_pNtlPLObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "WORLD_LIGHT_SPHERE", pParam));
		DBO_ASSERT(m_pNtlPLObj, "Entity create failed.");

		m_pNtlPLObj->SetFadeEnable(FALSE);

		RpNtlClumpSetData(m_pNtlPLObj->GetClump(), this);
	}

#endif

	SetPosition(pCreateParam->pPos);
	SetRadius(pCreateParam->fRadius);
	SetColor(pCreateParam->uiRed, pCreateParam->uiGreen, pCreateParam->uiBlue);

	NTL_RETURN(TRUE);
}

void CNtlPLWorldLight_Sphere::Destroy()
{
	NTL_FUNCTION("CNtlPLWorldLight_Sphere::Destroy");

#ifdef dNTL_WORLD_TOOL_MODE

	if(m_pNtlPLObj)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLObj);
		m_pNtlPLObj = NULL;
	}

#endif

	NTL_RETURNVOID();
}

RwBool CNtlPLWorldLight_Sphere::GetWorldLightColorf(const RwV3d* pvPos, RwRGBAReal* pclrfOut, void* pUserData)
{
	RwReal fDist;
	if (!IsIn(pvPos, &fDist))
	{
		return FALSE;
	}
	const RwRGBAReal* pClrf = (RwRGBAReal*)pUserData;

	RwReal fDelta = fDist / m_Sphere.radius;
	pclrfOut->red	= m_clrfLight.red + (fDelta * (pClrf->red - m_clrfLight.red));
	pclrfOut->green	= m_clrfLight.green + (fDelta * (pClrf->green - m_clrfLight.green));
	pclrfOut->blue	= m_clrfLight.blue + (fDelta * (pClrf->blue - m_clrfLight.blue));

	CLAMP(pclrfOut->red, 0.0f, 1.0f);
	CLAMP(pclrfOut->green, 0.0f, 1.0f);
	CLAMP(pclrfOut->blue, 0.0f, 1.0f);

	return TRUE;
}

void CNtlPLWorldLight_Sphere::SetRadius(RwReal fRadius)
{
	CNtlPLWorldLight_Base::SetRadius(fRadius);

#ifdef dNTL_WORLD_TOOL_MODE

	if(m_pNtlPLObj)
	{
		// Sphere Model이 지름이 1이기 때문에 연산해 주자.
		RwV3d vScale;
		vScale.x = vScale.y = vScale.z = fRadius * 2.0f;
		m_pNtlPLObj->SetScale(&vScale);		
	}

#endif	
}

void CNtlPLWorldLight_Sphere::SetVision(RwBool _Flag)
{
#ifdef dNTL_WORLD_TOOL_MODE

	if(_Flag)
	{
		if(!m_pNtlPLObj)
		{
			SPLEntityCreateParam Param;
			Param.pPos = &m_Sphere.center;

			m_pNtlPLObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "WORLD_LIGHT_SPHERE", &Param));
			DBO_ASSERT(m_pNtlPLObj, "Entity create failed.");

			m_pNtlPLObj->SetFadeEnable(FALSE);

			RpNtlClumpSetData(m_pNtlPLObj->GetClump(), this);

			m_pNtlPLObj->SetPosition(&m_Sphere.center);	

			RwV3d vScale;
			vScale.x = vScale.y = vScale.z = GetRadius() * 2.0f;
			m_pNtlPLObj->SetScale(&vScale);
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