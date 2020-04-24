#ifndef __WORLD_ZONE_H__
#define __WORLD_ZONE_H__

#include "WorldObjectList.h"
#include "WorldZoneTable.h"

class CWorld;
class CNtlPacket;

class CWorldZone
{

public:

	CWorldZone();
	virtual ~CWorldZone();

public:

	bool				Create(sWORLD_ZONE_TBLDAT* pWorldZoneData, CWorld* pWorldRef);
	void				Destroy();

	virtual void		TickProcess(DWORD dwTickDiff, float fMultiple);
	virtual int			CopyToInfo(sWORLD_ZONE_INFO & rZoneInfo);

	CWorldObjectList*	GetObjectList() { return &m_objectList; }

	int					EnterObject(CSpawnObject* pSpawnObject);
	int					LeaveObject(CSpawnObject* pSpawnObject);

	void				AddObject(CSpawnObject* pSpawnObject);
	void				RemoveObject(CSpawnObject* pSpawnObject);

	void				Broadcast(CNtlPacket* pPacket, CSpawnObject* pExcept = NULL);

	const ZONEID		GetZoneId() { return m_pWorldZoneData->tblidx; }
	const bool			ISForbiddenVehicle() { return m_pWorldZoneData->bForbidden_Vehicle; }

	bool				HasFunc(WORD wFuncFlag);

	void				UpdateZoneInfo(bool bIsDark);

protected:

	void				Init();

private:

	sWORLD_ZONE_TBLDAT* m_pWorldZoneData;
	CWorld*				m_pWorldRef;

	CWorldObjectList	m_objectList;

	bool				m_bIsDark;

};


#endif