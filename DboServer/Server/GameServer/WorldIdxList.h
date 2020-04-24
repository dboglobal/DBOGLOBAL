#ifndef __WORLD_IDX_LIST_H__
#define __WORLD_IDX_LIST_H__

#include <map>
#include "NtlSharedType.h"
class CWorld;

class CWorldIdxList
{

public:

	CWorldIdxList();
	virtual ~CWorldIdxList();

public:

	bool		AddWorld(CWorld *pWorld);
	void		RemoveWorld(CWorld *pWorld);

	CWorld*		GetFirst();
	CWorld*		GetFirst(TBLIDX worldIdx);
	CWorld*		GetNext();
	CWorld*		GetNext(TBLIDX worldIdx);

	bool		Find(CWorld *pWorld);

protected:

	void		Init();

	typedef std::multimap<TBLIDX, CWorld*> WORLDMAP;
	typedef WORLDMAP::value_type WORLDVAL;

	WORLDMAP m_worldMap;

private:

	WORLDMAP::iterator m_iterator;
	WORLDMAP::iterator m_iteratorBeg;
	WORLDMAP::iterator m_iteratorEnd;

};


#endif