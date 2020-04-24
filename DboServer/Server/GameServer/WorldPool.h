#ifndef __WORLD_POOL_H__
#define __WORLD_POOL_H__

#include <map>
#include "NtlSharedType.h"

class CWorld;

class CWorldPool
{

public:

	CWorldPool();
	~CWorldPool();

public:

	void		Destroy();
	bool		PushWorld(TBLIDX tblidx, CWorld *pWorld);

	CWorld*		PopWorld(TBLIDX tblidx);
	CWorld*		PopWorld(TBLIDX tblidx, WORLDID worldID);

	int			GetCount(TBLIDX tblidx);
	int			GetCount();

	bool		IsExist(TBLIDX tblidx, WORLDID worldID);

	void		GetZoneAndCellTotalCount(DWORD *dwZoneCount, DWORD *dwCellCount);

protected:

	void		Init();

	bool		FindWorld(TBLIDX tblidx, CWorld *pWorld);

	typedef std::multimap<TBLIDX, CWorld*> WORLDMAP;
	typedef WORLDMAP::value_type WORLDVAL;

	WORLDMAP m_worldMap;

};


#endif