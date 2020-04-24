#include "precomp_ntlpresentation.h"
#include "NtlPLDummyWorld.h"

// core
#include "NtlDebug.h"

// presentation

#include "NtlPLEntityFreeList.h"
#include "NtlPLResourceManager.h"


CNtlPLDummyWorld::CNtlPLDummyWorld()
{
	m_pWorld = NULL;
	m_pWorldRes = NULL;
	m_bb.inf.x = -1000.0f; m_bb.inf.y = -1000.0f; m_bb.inf.z = -1000.0f;
	m_bb.sup.x = m_bb.sup.y = m_bb.sup.z = 1000.0f;

	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetClassType(PLENTITY_DUMMY_WORLD);
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
}

CNtlPLDummyWorld::~CNtlPLDummyWorld()
{

}


RwBool CNtlPLDummyWorld::Create( const SPLEntityCreateParam * pParam /*= NULL*/ )
{
	const SPLDummyWorldCreateParam *pDummyWorldParam = NULL;

	if(pParam)
	{
		pDummyWorldParam = reinterpret_cast<const SPLDummyWorldCreateParam*>( pParam );
		memcpy(&m_bb, &pDummyWorldParam->bb, sizeof(RwBBox));
	}

	if(pDummyWorldParam)
	{
		m_pWorldRes = GetNtlResourceManager()->LoadWorld(pDummyWorldParam->pWorldName, pDummyWorldParam->pWorldPath);
		m_pWorld = (RpWorld*)m_pWorldRes->GetData();
	}
	else
	{
		m_pWorld = RpWorldCreate(&m_bb);
	}
	DBO_ASSERT(m_pWorld, "World create failed.");

	if(m_pWorld)
		CNtlPLGlobal::SetActiveWorld(m_pWorld);

	return TRUE;
}

void CNtlPLDummyWorld::Destroy(void)
{
	if(m_pWorldRes)
	{
		GetNtlResourceManager()->UnLoad(m_pWorldRes);
		m_pWorldRes = NULL;
		m_pWorld = NULL;
	}
	else
	{
		if(m_pWorld)
			RpWorldDestroy(m_pWorld);
	}

	CNtlPLGlobal::SetActiveWorld(NULL);
}

RwBool CNtlPLDummyWorld::Update(RwReal fElapsed)
{
	return TRUE;
}

RwBool CNtlPLDummyWorld::Render(void)
{
	return TRUE;
}

RwBool CNtlPLDummyWorld::SetProperty(const CNtlPLProperty *pData)
{
	return TRUE;
}


void* CNtlPLDummyWorld::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLDummyWorld::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_DUMMY_WORLD));
}

void CNtlPLDummyWorld::operator delete(void* pObj)
{
	NTL_FUNCTION("CNtlPLDummyWorld::operator delete");

	CNtlPLEntityFreeList::Free(PLENTITY_DUMMY_WORLD, pObj);

	NTL_RETURNVOID();
}