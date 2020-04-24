#ifndef __WORLD_OBJECT_LINKER_H__
#define __WORLD_OBJECT_LINKER_H__

#include "NtlLinkList.h"
#include "NtlObject.h"

class CSpawnObject;

class CWorldObjectLinker : public CNtlLinkObject
{

public:

	CWorldObjectLinker();
	virtual ~CWorldObjectLinker();

public:

	void			SetOwner(CSpawnObject *pOwner) { m_pOwner = pOwner; }
	CSpawnObject*	GetOwner() const { return m_pOwner; }

	eOBJTYPE		GetObjType();


private:

	CSpawnObject*	m_pOwner;

};


#endif