/*****************************************************************************
*
* File			: NtlPLObject.h
* Author		: HyungSuk Jang
* Copyright	: (аж)NTL
* Date			: 2005. 8. 11	
* Abstract		: Presentation layer object entity class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLDUMMYWORLD_H__
#define __NTL_PLDUMMYWORLD_H__

#include "NtlPLEntity.h"

class CNtlPLResource;

struct SPLDummyWorldCreateParam : public SPLEntityCreateParam
{
	RwBBox bb;
	const RwChar *pWorldName;
	const RwChar *pWorldPath;
};

class CNtlPLDummyWorld : public CNtlPLEntity
{
private:

	RwBBox m_bb;
	RpWorld *m_pWorld;
	CNtlPLResource *m_pWorldRes; 

public:

	CNtlPLDummyWorld();
	virtual ~CNtlPLDummyWorld();


	virtual RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	virtual void Destroy(void);
	virtual RwBool Update(RwReal fElapsed);
	virtual RwBool Render(void);
	virtual RwBool SetProperty(const CNtlPLProperty *pData);

	void*	operator new(size_t size);
	void	operator delete(void* pObj);

public:

	RpWorld* GetWorld(void) const;
};

inline RpWorld* CNtlPLDummyWorld::GetWorld(void) const
{
	return m_pWorld;
}


#endif 