#pragma once


#include "NtlPLPlanet.h"


#define dNTLPL_PLANET_DIST2SKY_DEFAULT (30.0f)


class CPlanetIOHandler
{
protected:
	CNtlPLPlanet*	m_pPlanetGroup[dNTLPL_PLANET_MAX_CNT];
	CNtlPLPlanet*	m_pCurPlanet;
	RwInt32			m_CurPlanetIdx;
	RwReal			m_DistFromPos2Sky[dNTLPL_PLANET_MAX_CNT];

protected:
	RwReal	GetDistPlanet2Sky(CNtlPLPlanet* _pPlanet);
	RwBool 	ForcePlanetPosFromCam2Sky(CNtlPLPlanet* _pPlanet, RwReal _Dist);

public:
	CPlanetIOHandler(void);
	~CPlanetIOHandler(void);

	static RpCollisionTriangle* Line2SkyAtomicTriCB(RpIntersection *pIntersection, RpCollisionTriangle *pCollTriangle, RwReal Distance, void *pData);

	RwBool	CreatePlanet(RwInt32 _IdxPlanet, SPLSunCreateParam& _Param);
	RwBool	DestroyPlanet(RwInt32 _IdxPlanet);
	VOID	DestroyAll();

	RwBool			SetProp();
	RwBool			GetProp(RwInt32 _CurFieldIdx);
	RwInt32			GetCurPlanetIdx() { return m_CurPlanetIdx;}
	CNtlPLPlanet*	GetCurPlanet() { return m_pCurPlanet;}
	CNtlPLPlanet*	GetPlanet(RwInt32 iIndex) { return m_pPlanetGroup[iIndex];}

	VOID	Update(RwReal _ElapsedTime);
	VOID	Render();
	VOID	RenderText(RtCharset* _pCharset, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight);
};
