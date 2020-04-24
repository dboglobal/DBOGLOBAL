#ifndef __NTL_PL_PLANT_H__
#define __NTL_PL_PLANT_H__

#include <RwCore.h>
#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLApi.h"
#include "ceventhandler.h"
#include "NtlWorldSector.h"

class CNtlPLPlantProperty;

class CNtlPLPlant : public CNtlPLEntity, public RWS::CEventHandler
{
public:
	CNtlPLPlant();
	virtual ~CNtlPLPlant();

public:
	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();
	virtual void	HandleEvents(RWS::CMsg &pMsg);

	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);
	virtual RwBool	RenderSector(CNtlWorldSector* pSector);

	virtual RwBool	SetProperty(const CNtlPLProperty *pProperty);

	void	OnLoad(CNtlWorldSector* pSector);
	void	OnUnload(CNtlWorldSector* pSector);
	
	// World Editor에서 사용하기 위한 함수.
#ifdef dNTL_WORLD_TOOL_MODE
	void	OnCreate(CNtlWorldSector* pSector, RwInt32 iTileIdx, RwInt32 iSetIdx);
	void	OnDelete(CNtlWorldSector* pSector, RwInt32 iTileIdx);
	void	OnObjectCount(CNtlWorldSector* pSector, RwInt32 iCount);
#endif

	void	SetClipDistance(RwReal fClipDistance) { m_fClipDistance = fClipDistance; }
	RwReal	GetClipDistance() { return m_fClipDistance; }

protected:
	void	OnGenerator(CNtlWorldSector* pSector, sSECTOR_PLANT_SET_ATTR* pPlantSet, sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj);

	void	LoadResourceSet(RwInt32 iSetIdx);
	void	UnloadResourceSet(RwInt32 iSetIdx);


	RwBool	PlantFrustumTestSphere(const RwSphere* pSphere);
	RwBool	PlantFrustumTestSector(const CNtlWorldSector* pSector);


	sSECTOR_PLANT_SET_ATTR*							GetPlantSetAttrFromSector(CNtlWorldSector* pSector, RwInt32 iSetIdx);
	sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* GetPlantObjFromPlantSet(sSECTOR_PLANT_SET_ATTR* pPlantSet, RwInt32 iTileIdx);

protected:
	struct sNTLPLPlantResource
	{
		RwInt32			iRef;
		CNtlPLResource*	pPLResource;

		sNTLPLPlantResource() : iRef(0), pPLResource(NULL) {}
	};

	typedef std::map<std::string, sNTLPLPlantResource>									RES_MAP;
	typedef	RES_MAP::iterator															RES_MAP_IT;

	typedef std::vector<sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR::TRANSFORM_VEC*>	TRANSFORM_VEC;
	typedef TRANSFORM_VEC::iterator														TRANSFORM_VEC_IT;

	typedef std::map<std::string, TRANSFORM_VEC>										RENDER_OBJ_MAP;
	typedef	RENDER_OBJ_MAP::iterator													RENDER_OBJ_MAP_IT;

	typedef std::map<CNtlWorldSector*, RENDER_OBJ_MAP>									SECTOR_MAP;
	typedef	SECTOR_MAP::iterator														SECTOR_MAP_IT;


	CNtlPLPlantProperty*		m_pProperty;
	CNtlPlane					m_planeCilp;
	RES_MAP						m_mapResource;
	SECTOR_MAP					m_mapSector;
	RwReal						m_fClipDistance;


};
  
#endif