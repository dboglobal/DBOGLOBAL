#include "precomp_ntlpresentation.h"
#include "NtlPLWeatherDefine.h" 


void* SPLWeatherParticle::operator new(size_t size)
{
	return CNtlPLWeatherParticleFreeList::Alloc();
}

void SPLWeatherParticle::operator delete(void *p)
{
	CNtlPLWeatherParticleFreeList::Free(p);
}


//////////////////////////////////////////////////////////////////////////
// CNtlPLWeatherParticleFreeList
//////////////////////////////////////////////////////////////////////////

RwFreeList* CNtlPLWeatherParticleFreeList::m_pFreeList = NULL;

RwBool CNtlPLWeatherParticleFreeList::AllocFreeList()
{
	m_pFreeList = RwFreeListCreate(sizeof(SPLWeatherParticle), 10000, rwMALLOCALIGNMENT, rwMEMHINTDUR_GLOBAL);

	return TRUE;
}

void CNtlPLWeatherParticleFreeList::DeAllocFreeList()
{
	RwFreeListDestroy(m_pFreeList);
}

void* CNtlPLWeatherParticleFreeList::Alloc()
{
	return  RwFreeListAlloc(m_pFreeList, rwMEMHINTDUR_GLOBAL);
}

void CNtlPLWeatherParticleFreeList::Free(void *p)
{
	RwFreeListFree(m_pFreeList, p);
}
