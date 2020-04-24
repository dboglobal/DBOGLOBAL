#include "precomp_ntlpresentation.h"

#include "NtlMath.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"

#include "NtlPLWorldLight_Base.h"

CNtlPLWorldLight_Base::CNtlPLWorldLight_Base()
{
#ifdef dNTL_WORLD_TOOL_MODE
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
#else
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOTUPDATE | NTL_PLEFLAG_NOT_VISIBLE);
#endif
		
	SetLayer(PLENTITY_LAYER_NONE);
	SetClassType(PLENTITY_WORLD_LIGHT);

	m_Sphere.center.x	= 0.0f;
	m_Sphere.center.y	= 0.0f;
	m_Sphere.center.z	= 0.0f;
	m_Sphere.radius		= 0.0f;

	m_clrfLight.alpha	= 1.0f;
	m_clrfLight.red		= 1.0f;
	m_clrfLight.green	= 1.0f;
	m_clrfLight.blue	= 1.0f;

#ifdef dNTL_WORLD_TOOL_MODE
	m_pNtlPLObj = NULL;
#endif
}

CNtlPLWorldLight_Base::~CNtlPLWorldLight_Base()
{
#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pNtlPLObj)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLObj);
		m_pNtlPLObj = NULL;
	}
#endif
}

void CNtlPLWorldLight_Base::SetPosition(const RwV3d* pvPos)
{
	m_Sphere.center = *pvPos;

#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pNtlPLObj)
	{
		m_pNtlPLObj->SetPosition(pvPos);
	}
#endif
}

RwV3d CNtlPLWorldLight_Base::GetPosition()
{
	return m_Sphere.center;
}

void CNtlPLWorldLight_Base::SetRadius(RwReal fRadius)
{
	m_Sphere.radius = fRadius;
}

RwReal CNtlPLWorldLight_Base::GetRadius()
{
	return m_Sphere.radius;
}

void CNtlPLWorldLight_Base::SetColor(RwUInt8 uiRed, RwUInt8 uiGreen, RwUInt8 uiBlue)
{
	RwRGBA rgba;
	rgba.alpha	= 255;
	rgba.red	= uiRed;
	rgba.green	= uiGreen;
	rgba.blue	= uiBlue;

	RwRGBARealFromRwRGBA(&m_clrfLight, &rgba);
}

RwRGBA CNtlPLWorldLight_Base::GetColor()
{
	RwRGBA rgba;

	RwRGBAFromRwRGBAReal(&rgba, &m_clrfLight);

	return rgba;
}

void CNtlPLWorldLight_Base::SetColorf(RwReal fRed, RwReal fGreen, RwReal fBlue)
{
	m_clrfLight.red		= fRed;
	m_clrfLight.green	= fGreen;
	m_clrfLight.blue	= fBlue;
}

RwRGBAReal CNtlPLWorldLight_Base::GetColorf()
{
	return m_clrfLight;
}

RwBool CNtlPLWorldLight_Base::IsIn(const RwV3d* pvPos, RwReal* pfOutDistance)
{
	return IsInSphere(pvPos, pfOutDistance);
}

RwBool CNtlPLWorldLight_Base::IsInSphere(const RwV3d* pvPos, RwReal* pfOutDistance)
{
	RwReal fDist = RwV3dLength(&(m_Sphere.center - *pvPos));
	if (fDist > m_Sphere.radius)
	{
		return FALSE;
	}

	if (pfOutDistance)
	{
		*pfOutDistance = fDist;
	}

	return TRUE;
}

RwBool CNtlPLWorldLight_Base::SetProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLWorldLight_Base::SetProperty");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLWorldLight_Base::Render()
{
	NTL_FUNCTION("CNtlPLWorldLight_Base::Render");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLWorldLight_Base::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLWorldLight_Base::Update");
	NTL_RETURN(TRUE);
}