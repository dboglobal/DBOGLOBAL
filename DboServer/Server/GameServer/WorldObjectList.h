#ifndef __WORLD_OBJECT_LIST_H__
#define __WORLD_OBJECT_LIST_H__

#include "WorldObjectLinker.h"


class CWorldObjectList
{

public:

	CWorldObjectList();
	virtual ~CWorldObjectList();

public:

	bool			Add(CWorldObjectLinker *pObjectLinker);
	bool			Remove(CWorldObjectLinker *pObjectLinker);

	CSpawnObject*	GetFirst(eOBJTYPE objType);

	CSpawnObject*	GetNext(CSpawnObject *pSpawnObject);
	CSpawnObject*	GetNext(CWorldObjectLinker *pObjectLinker);

	CSpawnObject*	GetPrev(CSpawnObject *pSpawnObject);
	CSpawnObject*	GetPrev(CWorldObjectLinker *pObjectLinker);

	int				GetObjCount(eOBJTYPE objType);
	int				GetObjMaxCount(eOBJTYPE objType);


private:

	CNtlLinkList	m_objectList[OBJTYPE_LAST];

};


#endif