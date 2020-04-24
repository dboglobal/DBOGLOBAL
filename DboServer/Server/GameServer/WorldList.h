#ifndef __WORLD_LIST_H__
#define __WORLD_LIST_H__

#include <map>
#include "NtlSharedType.h"
class CWorld;

class CWorldList
{

public:

	CWorldList();
	virtual ~CWorldList();

public:

	bool		AddWorld(CWorld *pWorld);
	void		RemoveWorld(CWorld *pWorld);

	CWorld*		FindWorld(WORLDID worldID);
	CWorld*		GetFirst();
	CWorld*		GetNext();

	int			GetCount();

protected:

	typedef std::map<WORLDID, CWorld*> WORLDMAP;
	typedef WORLDMAP::value_type WORLDVAL;

	WORLDMAP m_worldMap;

private:

	WORLDMAP::iterator m_iterator;

};


#endif