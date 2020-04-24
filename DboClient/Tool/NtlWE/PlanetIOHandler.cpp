#include "StdAfx.h"
#include "PlanetIOHandler.h"
#include "NtlPLPlanet.h"
#include "NtlPLSceneManager.h"
#include "NtlWorldBrush3DCircle.h"
#include "ntlworldcommon.h"
#include "NtlPLSky.h"
#include "NtlWe.h"
#include "PaletteFieldUI.h"
#include "PaletteSkyFloatingEntityDlg.h"
#include "NtlText.h"


CPlanetIOHandler::CPlanetIOHandler(void)
{
	m_pPlanetGroup[0]	= NULL;
	m_pPlanetGroup[1]	= NULL;
	m_pPlanetGroup[2]	= NULL;
	m_pCurPlanet		= NULL;
	m_CurPlanetIdx		= -999;
	m_DistFromPos2Sky[0]	= dNTLPL_PLANET_DIST2SKY_DEFAULT;
	m_DistFromPos2Sky[1]	= dNTLPL_PLANET_DIST2SKY_DEFAULT;
	m_DistFromPos2Sky[2]	= dNTLPL_PLANET_DIST2SKY_DEFAULT;
}

CPlanetIOHandler::~CPlanetIOHandler(void)
{
	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
		DestroyPlanet(i);
}

VOID CPlanetIOHandler::DestroyAll()
{
	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
		DestroyPlanet(i);
}

RwReal CPlanetIOHandler::GetDistPlanet2Sky(CNtlPLPlanet* _pPlanet)
{
	RwReal	DistFromNewPlanet2Sky = -999.0f;
	RwV3d	_MidPos;
	RwReal	_Radius;
	_pPlanet->GetWorldPos(_MidPos, _Radius);

	RwV3d	RayCurLineDir;
	RwLine	LineCam2Planet;
	LineCam2Planet.start	= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	LineCam2Planet.end		= _MidPos;

	RwV3dNormalize(&RayCurLineDir, &(LineCam2Planet.end - LineCam2Planet.start));
	RwV3dIncrementScaled(&LineCam2Planet.end, &RayCurLineDir, 10000.0f);

	RpIntersection Intersection;
	Intersection.t.line	= LineCam2Planet;
	Intersection.type	= rpINTERSECTLINE;

	sNtlMapObjCollisionParam CollisionParamInTri;
	::memset(&CollisionParamInTri, 0, sizeof(sNtlMapObjCollisionParam));
	CollisionParamInTri.DistFromCam = RwRealMAXVAL;

	RpAtomicForAllIntersections(GetSceneManager()->GetSky()->GetAtomic(), &Intersection, Line2SkyAtomicTriCB, &CollisionParamInTri);
	if(CollisionParamInTri.IsCollided)
	{
		DistFromNewPlanet2Sky = static_cast<RwReal>(::sqrtf(CNtlMath::MathRwV3dSquaredLength(&(CollisionParamInTri.PtIntersection - _MidPos))));

		RwV3d RaySunToSkyDir;
		RwV3dNormalize(&RaySunToSkyDir, &(CollisionParamInTri.PtIntersection - _MidPos));
		if (static_cast<RwInt32>(RayCurLineDir.x * 1000.0f) == static_cast<RwInt32>(RaySunToSkyDir.x * 1000.0f) &&
			static_cast<RwInt32>(RayCurLineDir.y * 1000.0f) == static_cast<RwInt32>(RaySunToSkyDir.y * 1000.0f) &&
			static_cast<RwInt32>(RayCurLineDir.z * 1000.0f) == static_cast<RwInt32>(RaySunToSkyDir.z * 1000.0f))
		{
			DistFromNewPlanet2Sky *= -1.0f;
		}

	}

	return DistFromNewPlanet2Sky;
}

RwBool CPlanetIOHandler::CreatePlanet(RwInt32 _IdxPlanet, SPLSunCreateParam& _Param)
{
	DestroyPlanet(_IdxPlanet);

	m_pPlanetGroup[_IdxPlanet] = static_cast<CNtlPLPlanet*>(GetSceneManager()->CreateEntity(PLENTITY_SUN, "", &_Param));
	DBO_ASSERT(m_pPlanetGroup[_IdxPlanet], "Entity create failed.");

	m_pPlanetGroup[_IdxPlanet]->SetPosition(_Param.pPos);
	m_pPlanetGroup[_IdxPlanet]->SetScale(_Param.Scale);

	m_DistFromPos2Sky[_IdxPlanet] = GetDistPlanet2Sky(m_pPlanetGroup[_IdxPlanet]);
	CLAMP(m_DistFromPos2Sky[_IdxPlanet], 1.0f, 2048.0f);
	if(!ForcePlanetPosFromCam2Sky(m_pPlanetGroup[_IdxPlanet], m_DistFromPos2Sky[_IdxPlanet]))
	{
		AfxMessageBox("can't make a planet on the ground.");
		DestroyPlanet(_IdxPlanet);

		return FALSE;
	}

	return TRUE;
}

RwBool CPlanetIOHandler::DestroyPlanet(RwInt32 _IdxPlanet)
{
	if(m_pPlanetGroup[_IdxPlanet])
	{
		GetSceneManager()->DeleteEntity(m_pPlanetGroup[_IdxPlanet]);	
		m_pPlanetGroup[_IdxPlanet]	= NULL;
		m_pCurPlanet				= NULL;
		m_CurPlanetIdx				= -999;
	}

	return TRUE;
}

RwBool CPlanetIOHandler::SetProp()
{
	return TRUE;
}

RwBool CPlanetIOHandler::GetProp(RwInt32 _CurFieldIdx)
{
	return TRUE;
}

RpCollisionTriangle* CPlanetIOHandler::Line2SkyAtomicTriCB(RpIntersection *pIntersection, RpCollisionTriangle *pCollTriangle, RwReal Distance, void *pData)
{
	sNtlMapObjCollisionParam *pCollisionParam = static_cast<sNtlMapObjCollisionParam*>(pData);

	if(pCollisionParam->DistFromCam > Distance)
	{
		pCollisionParam->DistFromCam		= Distance;
		pCollisionParam->IsCollided			= TRUE;
		pCollisionParam->PtIntersection.x	= pIntersection->t.line.start.x + Distance * (pIntersection->t.line.end.x - pIntersection->t.line.start.x);
		pCollisionParam->PtIntersection.y	= pIntersection->t.line.start.y + Distance * (pIntersection->t.line.end.y - pIntersection->t.line.start.y);
		pCollisionParam->PtIntersection.z	= pIntersection->t.line.start.z + Distance * (pIntersection->t.line.end.z - pIntersection->t.line.start.z);
	}

	return pCollTriangle;
}

RwBool CPlanetIOHandler::ForcePlanetPosFromCam2Sky(CNtlPLPlanet* _pPlanet, RwReal _Dist)
{
	NTL_ASSERTE(_pPlanet);
	NTL_ASSERTE(_Dist > 0.0f);

	RwV3d	_MidPos;
	RwReal	_Radius;
	_pPlanet->GetWorldPos(_MidPos, _Radius);

	RwV3d	RayCurLineDir;
	RwLine	LineCam2Planet;
	LineCam2Planet.start	= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	LineCam2Planet.end		= _MidPos;

	RwV3dNormalize(&RayCurLineDir, &(LineCam2Planet.end - LineCam2Planet.start));
	RwV3dIncrementScaled(&LineCam2Planet.end, &RayCurLineDir, 10000.0f);

	RpIntersection Intersection;
	Intersection.t.line	= LineCam2Planet;
	Intersection.type	= rpINTERSECTLINE;

	sNtlMapObjCollisionParam CollisionParamInTri;
	::memset(&CollisionParamInTri, 0, sizeof(sNtlMapObjCollisionParam));
	CollisionParamInTri.DistFromCam = RwRealMAXVAL;

	RpAtomicForAllIntersections(GetSceneManager()->GetSky()->GetAtomic(), &Intersection, Line2SkyAtomicTriCB, &CollisionParamInTri);
	if(CollisionParamInTri.IsCollided)
	{
		RwReal DistFromNewPlanet2Sky = static_cast<RwReal>(::sqrtf(CNtlMath::MathRwV3dSquaredLength(&(CollisionParamInTri.PtIntersection - _MidPos))));

		RayCurLineDir.x = -RayCurLineDir.x;
		RayCurLineDir.y = -RayCurLineDir.y;
		RayCurLineDir.z = -RayCurLineDir.z;

		RwV3d Result = CollisionParamInTri.PtIntersection;
		RwV3dIncrementScaled(&Result, &RayCurLineDir, -_Dist);
		
		_pPlanet->SetRelativePos(Result);
	}
	else
	{
		((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->Refresh();

		return FALSE;
	}

	((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->Refresh();

	return TRUE;
}

VOID CPlanetIOHandler::Update(RwReal ElapsedTime)
{
	RwBool Picked = FALSE;

	// select a planet
	if(dGET_MOUSE()->GetLBOnce())
	{
		for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
		{
			if(m_pPlanetGroup[i] && m_pPlanetGroup[i]->Pick())
			{
				m_pCurPlanet	= m_pPlanetGroup[i];
				m_CurPlanetIdx	= i;
				Picked			= TRUE;

				break;
			}
		}

		if(!Picked)
		{
			m_pCurPlanet	= NULL;
			m_CurPlanetIdx	= -999;
		}
	}

	// move the planet
	if(m_pCurPlanet)
	{	
		if(dGET_MOUSE()->GetBehavior(MB_LBDN) && dGET_MOUSE()->GetBehavior(MB_MOVE))
		{
			RwV3d	_WldPos;
			RwReal	_Radius;
			m_pCurPlanet->GetWorldPos(_WldPos, _Radius);

			RwV3d vFront	= { 0.0f, -1.0f, 0.0f };
			RwV3d vRight	= *RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
			RwV3dNormalize(&vFront, &vFront);
			RwV3dNormalize(&vRight, &vRight);

			float DistX = dGET_MOUSE()->GetXMovementWithRate(1.0f);
			float DistY = dGET_MOUSE()->GetYMovementWithRate(1.0f);
			RwV3dIncrementScaled(&_WldPos, &vRight, -DistX);
			RwV3dIncrementScaled(&_WldPos, &vFront, DistY);
			m_pCurPlanet->SetRelativePos(_WldPos);

			ForcePlanetPosFromCam2Sky(m_pCurPlanet, m_DistFromPos2Sky[m_CurPlanetIdx]);
		}
		else if(dGET_MOUSE()->GetWheelMovement())
		{
			RwReal WheelVal = dGET_MOUSE()->GetWheelMovement();
			if(WheelVal)
			{
				if(WheelVal > 0)
				{
					m_DistFromPos2Sky[m_CurPlanetIdx] += 10.0f;
				}
				else
				{
					m_DistFromPos2Sky[m_CurPlanetIdx] -= 10.0f;
				}

				CLAMP(m_DistFromPos2Sky[m_CurPlanetIdx], 1.0f, 2048.0f);
			}

			ForcePlanetPosFromCam2Sky(m_pCurPlanet, m_DistFromPos2Sky[m_CurPlanetIdx]);
		}
	}
}

VOID CPlanetIOHandler::Render()
{
	if(m_pCurPlanet && m_pCurPlanet->GetVisibility())
	{
		RwV3d		MidPos;
		RwReal		Radius = 0;
		RwSphere	Sphere;

		m_pCurPlanet->GetWorldPos(MidPos, Radius);
		Sphere.center = MidPos;
		Sphere.radius = Radius;

		CNtlWorldBrush3DCircle::Render(&Sphere);
	}
}

VOID CPlanetIOHandler::RenderText(RtCharset* _pCharset, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight)
{
	RwV3d	MidPos;
	RwReal	Radius = 0;
	RwChar	caption[20] = {0,};

	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
	{
		if(m_pPlanetGroup[i])
		{
			m_pPlanetGroup[i]->GetWorldPos(MidPos, Radius);

			RwV3d Pos = MidPos;

			RwV3d v2dPoint;
			RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
			RwV3dTransformPoints(&v2dPoint, &Pos, 1, pMatView);
			v2dPoint.x = (v2dPoint.x / v2dPoint.z) * LenWindowWidth + 10.0f;
			v2dPoint.y = (v2dPoint.y / v2dPoint.z) * LenWindowHeight + 10.0f;

			RwRGBA ClrFont;
			ClrFont.red		= 255;
			ClrFont.green	= 255;
			ClrFont.blue	= 255;
			RtCharsetSetColors(_pCharset, &ClrFont, &dGETAPP().m_BackgroundColor);
			RsSprintf(caption, RWSTRING("%s"), m_pPlanetGroup[i]->GetTexName());
			RsCharsetPrint(_pCharset, caption, (RwInt32)v2dPoint.x, (RwInt32)v2dPoint.y);
		}
	}
}
