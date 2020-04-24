#include "precomp_ntlpresentation.h"
#include "NtlPLPlanet.h"
#include "ntlworldcommon.h"
#include "rwplcore.h"
#include "NtlPLEntityFreeList.h"

void* CNtlPLPlanet::operator new(size_t size)
{
	NTL_FUNCTION(__FUNCTION__);

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_SUN_PLANET));
}

void CNtlPLPlanet::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_SUN_PLANET, pObj);	
}

CNtlPLPlanet::CNtlPLPlanet(void)
{
}

CNtlPLPlanet::~CNtlPLPlanet(void)
{
}

VOID CNtlPLPlanet::Save(FILE* _pFile)
{

}

VOID CNtlPLPlanet::Load(FILE* _pFile)
{

}

VOID CNtlPLPlanet::GetWorldPos(RwV3d& _MidPos, RwReal& _Radius)
{
	RwMatrix	matRender	= *RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
	RwV3d		vPosCamera	= *RwMatrixGetPos(&matRender);

	_MidPos	= vPosCamera + m_vPos;
	_Radius	= m_fScale;
}

VOID CNtlPLPlanet::AddPosition(RwV3d& _AddPos)
{
	m_vPos += _AddPos;
	SetPosition(&m_vPos);
}

VOID CNtlPLPlanet::GetRelativePos(RwV3d& _CurPos)
{
	_CurPos = m_vPos;
}

VOID CNtlPLPlanet::SetRelativePos(RwV3d& _NewWorldPos)	
{
	RwMatrix	matRender	= *RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
	RwV3d		vPosCamera	= *RwMatrixGetPos(&matRender);

	// get the direction vector for the extent between them
	SetPosition(&(_NewWorldPos - vPosCamera));
}

RwBool CNtlPLPlanet::Pick()
{
	RwV3d		MidPos;
	RwReal		Radius = 0;
	RwLine		LineFront;
	RwSphere	Sphere;
	RwReal		CenterDist;

	GetWorldPos(MidPos, Radius);
	Sphere.center = MidPos;
	Sphere.radius = Radius;

	RwV2d ScreenPos;
	ScreenPos.x = (RwReal)dGET_MOUSE()->GetPosClicked().x;
	ScreenPos.y = (RwReal)dGET_MOUSE()->GetPosClicked().y;

	RwLine cameraRay;
	RwCameraCalcPixelRay(CNtlPLGlobal::m_RwCamera, &cameraRay, &ScreenPos);

	RwV3d RwRayDir;
	RwV3dSubMacro(&RwRayDir, &cameraRay.end, &cameraRay.start);
	RwV3dNormalize(&RwRayDir, &RwRayDir);

	LineFront.start = cameraRay.start;

	RwV3d LineEndPos = LineFront.start;
	RwV3dIncrementScaled(&LineEndPos, &RwRayDir, 10000.0f);
	LineFront.end = LineEndPos;

	if(RtLineSphereIntersectionTest(&LineFront, &Sphere, &CenterDist))
	{
		return TRUE;
	}
	
	return FALSE;
}

RwBool CNtlPLPlanet::Compare(sNTL_PLANET& _NtlPlanet)
{
	sNTL_PLANET NtlPlanetProp;
	NtlPlanetProp._RelativePos			= m_vPos;
	NtlPlanetProp._LensFlareExistence	= m_pLensFlare ? 1 : 0;
	NtlPlanetProp._Scale				= m_fScale;
	strcpy_s(NtlPlanetProp._TexName, 10, GetTexName());

	return NtlPlanetProp.Compare(_NtlPlanet);
}

RwBool CNtlPLPlanet::WasFadeOutDone()
{
	if(m_bFading && !m_FadeColor.alpha)
	{
		return TRUE;
	}

	return FALSE;
}