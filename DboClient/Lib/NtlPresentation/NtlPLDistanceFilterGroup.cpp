#include "precomp_ntlpresentation.h"
#include "NtlPLDistanceFilterGroup.h"

// core
#include "NtlDebug.h"

CNtlPLDistanceFilterGroup::CNtlPLDistanceFilterGroup()
{
	m_bFirst = TRUE;
}

CNtlPLDistanceFilterGroup::~CNtlPLDistanceFilterGroup()
{
	Destroy();
}

RwBool CNtlPLDistanceFilterGroup::Create(void)
{
	return TRUE;
}

void CNtlPLDistanceFilterGroup::Destroy(void)
{
	MapSet *pSet;

	ListGroup::iterator it;
	for(it = m_listGroup.begin(); it != m_listGroup.end(); it++)
	{
		pSet = (*it);
		DeAllocateSet(pSet);
	}

	m_listGroup.clear();
}

CNtlPLDistanceFilterGroup::MapSet* CNtlPLDistanceFilterGroup::AllocateSet(void)
{
	MapSet *pSet = NTL_NEW MapSet;
	m_listGroup.push_back(pSet);

	return pSet;
}

void CNtlPLDistanceFilterGroup::DeAllocateSet(CNtlPLDistanceFilterGroup::MapSet *pSet)
{
	NTL_DELETE(pSet);
}

CNtlPLDistanceFilterGroup::MapSet* CNtlPLDistanceFilterGroup::FindInsert(void)
{
	if(m_listGroup.size() == 0)
		return NULL;

	MapSet *pSet;

	ListGroup::iterator it;
	for(it = m_listGroup.begin(); it != m_listGroup.end(); it++)
	{
		pSet = (*it);
		if(pSet->size() < MAX_DIST_FILTER_GROUP)
			return pSet;
	}

	return NULL;
}


void CNtlPLDistanceFilterGroup::AddEntity(CNtlPLEntity *pPLEntity)
{
	MapSet *pSet = FindInsert();
	if(pSet == NULL)
	{
		pSet = AllocateSet();

		if(m_bFirst)
		{
			m_bFirst = FALSE;
			m_itCurr = m_listGroup.begin();
		}
	}

	(*pSet)[pPLEntity] = pPLEntity;
}

void CNtlPLDistanceFilterGroup::RemoveEntity(CNtlPLEntity *pPLEntity)
{
	RwBool bChangeIterator = FALSE;
	MapSet *pSet;
	MapSet::iterator mapit;
	ListGroup::iterator it;

	for(it = m_listGroup.begin(); it != m_listGroup.end(); it++)
	{
		pSet = (*it);
		mapit = pSet->find(pPLEntity);
		if(mapit != pSet->end())
		{
			pSet->erase(mapit);
			if(pSet->size() == 0)
			{
				DeAllocateSet(pSet);
				if(m_itCurr == it)
					bChangeIterator = TRUE;

				ListGroup::iterator newit = m_listGroup.erase(it);

				if(bChangeIterator)
				{
					m_itCurr = newit;
					if(m_itCurr == m_listGroup.end())
					{
						if(m_listGroup.size() == 0)
							m_bFirst = TRUE;

						m_itCurr = m_listGroup.begin();
					}
				}
			}

			return;
		}
	}
}

const CNtlPLDistanceFilterGroup::MapSet* CNtlPLDistanceFilterGroup::GetNextUpdateSet(void)
{
	if(m_listGroup.size() == 0)
		return NULL;

	m_itCurr++;
	if(m_itCurr == m_listGroup.end())
		m_itCurr = m_listGroup.begin();

	return (*m_itCurr);
}

const CNtlPLDistanceFilterGroup::ListGroup& CNtlPLDistanceFilterGroup::GetEntities(void) const
{
	return m_listGroup;
}

RwInt32 CNtlPLDistanceFilterGroup::GetFilterSetCount(void)
{
	return m_listGroup.size();
}

RwInt32 CNtlPLDistanceFilterGroup::GetEntityCount(void)
{
	RwInt32 iCount = 0;
	MapSet *pSet;
	ListGroup::iterator it;
	for(it = m_listGroup.begin(); it != m_listGroup.end(); it++)
	{
		pSet = (*it);
		iCount += pSet->size();
	}

	return iCount;
}
