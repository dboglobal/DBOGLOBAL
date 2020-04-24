#include "precomp_ntlsimulation.h"
#include "NtlSobInfluence.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSobSyncStatus.h"


DEFINITION_MEMORY_POOL(CNtlSobInfluence)


CNtlSobInfluence::CNtlSobInfluence()
{
}

CNtlSobInfluence::~CNtlSobInfluence()
{
}

void CNtlSobInfluence::Destroy(void)
{
	CNtlSobSyncStatus *pSyncStatus;
	ListSyncStatus::iterator it;
	for(it = m_listSyncStatus.begin(); it != m_listSyncStatus.end(); it++)
	{
		pSyncStatus = (*it);
		pSyncStatus->Destroy();
		pSyncStatus->SetSobParent(NULL);

		NTL_DELETE( pSyncStatus );
	}

	m_listSyncStatus.clear();
	
	CNtlSob::Destroy();
}


void CNtlSobInfluence::Update(RwReal fElapsed)
{
	// sync simulation object update
	
	CNtlSobSyncStatus *pSyncStatus;

	ListSyncStatus::iterator it;
	for(it = m_listSyncStatus.begin(); it != m_listSyncStatus.end(); )
	{
		pSyncStatus = (*it);
		pSyncStatus->Update(fElapsed);
		if(pSyncStatus->IsFinish())
		{
			it = m_listSyncStatus.erase(it);
			pSyncStatus->EventGenerator();
			pSyncStatus->Destroy();
			pSyncStatus->SetSobParent(NULL);

			NTL_DELETE( pSyncStatus );
		}
		else 
			++it;
	}

	if(m_listSyncStatus.size() == 1)
	{
		it = m_listSyncStatus.begin();
		(*it)->Active();
	}
}


void CNtlSobInfluence::AddSyncStatus(CNtlSobSyncStatus *pSobSyncStatus)
{
	pSobSyncStatus->SetSobParent(GetSobParent());
	m_listSyncStatus.push_back(pSobSyncStatus);
}

void CNtlSobInfluence::RemoveSyncStatus(CNtlSobSyncStatus *pSobSyncStatus)
{
	ListSyncStatus::iterator it;
	for(it = m_listSyncStatus.begin(); it != m_listSyncStatus.end(); it++)
	{
		if( (*it) == pSobSyncStatus )
		{
			m_listSyncStatus.erase(it);
			break;
		}
	}
}

void CNtlSobInfluence::AddSob(CNtlSob *pSobObj)
{
	switch(pSobObj->GetClassID())
	{
	case SLCLASS_SYNC_STATUS:
		AddSyncStatus(reinterpret_cast<CNtlSobSyncStatus*>( pSobObj ));
		break;
	}

	CNtlSob::AddSob(pSobObj);
}

void CNtlSobInfluence::RemoveSob(CNtlSob *pSobObj)
{
	switch(pSobObj->GetClassID())
	{
	case SLCLASS_SYNC_STATUS:
		RemoveSyncStatus(reinterpret_cast<CNtlSobSyncStatus*>( pSobObj ));
		break;
	}

	CNtlSob::RemoveSob(pSobObj);
}

