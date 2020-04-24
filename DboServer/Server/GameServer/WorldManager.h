#ifndef __WORLD_MANAGER_H__
#define __WORLD_MANAGER_H__


#include "WorldList.h"
#include "WorldIdxList.h"
#include "WorldPool.h"

#include "WorldTable.h"
#include "WorldZoneTable.h"


class CSpawnObject;
class CWorldFactory;
class CCharacter;


class CWorldManager
{

public:

	CWorldManager();
	virtual ~CWorldManager();

public:

	int					Create(CWorldFactory *pWorldFactory, CWorldTable *pWorldTable, CWorldZoneTable *pWorldZoneTable);
	void				Destroy();

	virtual CWorld*		CreateWorld(WORLDID worldID, sWORLD_TBLDAT *pTbldat);
	virtual CWorld*		CreateWorld(sWORLD_TBLDAT *pTbldat);

	virtual void		DestroyWorld(CWorld *pWorld);

	virtual void		TickProcess(DWORD dwTickDiff, float fMultiple);

	int					EnterObject(CSpawnObject *pSpawnObject, WORLDID worldID, bool bIsServerStart = false);
	int					LeaveObject(CSpawnObject *pSpawnObject);
	int					ChangeWorld(CSpawnObject *pSpawnObject, WORLDID destID);

	bool				IsInBoundary(WORLDID worldId, CNtlVector& rLoc);

	CWorld*				FindWorld(WORLDID worldId);
	CWorld*				GetFirst();
	CWorld*				GetNext();

	int					GetAvailableCount(TBLIDX tblidx);

	int					GetTotalWorldCount();

	void				GetZoneAndCellTotalCount(DWORD *dwZoneCount, DWORD *dwCellCount);

	CWorldFactory*		GetWorldFatory() { return m_pWorldFactory; }

	WORLDID				GetDefaultWorldID() { return m_defaultWorldID; }
	TBLIDX				GetDefaultWorldTblidx() { return m_defaultWorldTblidx; }

	CNtlVector&			GetDefaultWorldLoc() { return m_vDefaultWorldLoc; }
	CNtlVector&			GetDefaultWorldDir() { return m_vDefaultWorldDir; }

	CWorld*				GetDefaultWorld();

	bool				GetDestLocAfterCollision(CWorld * pCurWorld, CCharacter * pChar, CNtlVector & rvShift, CNtlVector & rvDestLoc, CNtlVector & rvNewDir);

	float				GetAdjustedHeight(WORLDID worldId, float fX, float fY, float fZ, int nVertRange);

protected:

	CWorld*				MakeWorld(sWORLD_TBLDAT *pTbldat);
	CWorld*				MakeWorld(sWORLD_TBLDAT *pTbldat, WORLDID worldID);

	void				Init();

	int					CreateStaticWorld(sWORLD_TBLDAT *pTbldat, CWorldZoneTable *pWorldZoneTable);
	int					CreateDynamicWorld(sWORLD_TBLDAT *pTbldat, CWorldZoneTable *pWorldZoneTable);

	void				GetTextMemoryUseInfo(char *buff, int nBuffSize);

private:

	CWorldList		m_worldList;

	CWorldIdxList	m_worldIdxList;

	CWorldPool		m_worldPool;

	CWorldFactory*	m_pWorldFactory;

	CWorld*			m_pDefaultWorld;

	WORLDID			m_defaultWorldID;
	TBLIDX			m_defaultWorldTblidx;

	CNtlVector		m_vDefaultWorldLoc;
	CNtlVector		m_vDefaultWorldDir;

};

#endif