#include "precomp_ntlsimulation.h"
#include "NtlSobStatusAnimSyncManager.h"


// simulation
#include "NtlSobManager.h"
#include "NtlSLLogic.h"

DEFINITION_MEMORY_POOL(CNtlSobStatusAnimSyncUnit)


CNtlSobStatusAnimSyncUnit::CNtlSobStatusAnimSyncUnit()
{
	m_hUnit		= INVALID_SERIAL_ID;
	m_iCurValue	= 0;
	m_iMaxValue	= 0;
}


void CNtlSobStatusAnimSyncUnit::SetUIntHandle(SERIAL_HANDLE hUnit)
{
	m_hUnit = hUnit;
}

SERIAL_HANDLE CNtlSobStatusAnimSyncUnit::GetUIntHandle(void)
{
	return m_hUnit;
}


void CNtlSobStatusAnimSyncUnit::SetValue(RwInt32 iCurrValue, RwInt32 iMaxValue)
{
	m_iCurValue	= iCurrValue;
	m_iMaxValue	= iMaxValue;
}


void CNtlSobStatusAnimSyncUnit::GetValue(RwInt32& iCurrvalue, RwInt32& iMaxValue)
{
	iCurrvalue	= m_iCurValue;
	iMaxValue	= m_iMaxValue;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlSobStatusAnimSync)

CNtlSobStatusAnimSync::CNtlSobStatusAnimSync()
{
	m_hSerialId			= INVALID_SERIAL_ID;
	m_hSyncStatus		= INVALID_SYNCEVENT_ID;
	m_pServerSyncUnit	= NULL;
	m_fExceptTime		= 0.0f;
	m_bFinish			= FALSE;
	m_fLifeTime			= 5.0f;
}

CNtlSobStatusAnimSync::~CNtlSobStatusAnimSync()
{
	ListAnimSyncUnit::iterator it;
	for(it = m_listAnimSyncUnit.begin(); it != m_listAnimSyncUnit.end(); it++)
	{
		NTL_DELETE( *it );
	}

	m_listAnimSyncUnit.clear();

	NTL_DELETE( m_pServerSyncUnit );
}

void CNtlSobStatusAnimSync::Update(RwReal fElapsed)
{
	if(m_listAnimSyncUnit.empty())
	{
		m_bFinish = TRUE;
		return;
	}

	m_fExceptTime += fElapsed;
	if(m_fExceptTime >= m_fLifeTime)
	{
		ListAnimSyncUnit::iterator it;
		for(it = m_listAnimSyncUnit.begin(); it != m_listAnimSyncUnit.end(); it++)
		{
			NTL_DELETE( *it );
		}

		m_listAnimSyncUnit.clear();

		m_bFinish = TRUE;
	}
}

void CNtlSobStatusAnimSync::SetSerialId(SERIAL_HANDLE hSerialId)
{
	m_hSerialId = hSerialId;
}


void CNtlSobStatusAnimSync::SetServerSyncUnit(RwInt32 iCurrValue, RwInt32 iMaxValue)
{
	m_pServerSyncUnit = NTL_NEW CNtlSobStatusAnimSyncUnit;
	m_pServerSyncUnit->SetUIntHandle(m_hSyncStatus);
	m_pServerSyncUnit->SetValue(iCurrValue, iMaxValue);
}


void CNtlSobStatusAnimSync::SetSyncHandle(SYNCEVENT_HANDLE hSyncStatus)
{
	m_hSyncStatus = hSyncStatus;
}

SYNCEVENT_HANDLE CNtlSobStatusAnimSync::GetSyncHandle(void)
{
	return m_hSyncStatus;

}

SYNCEVENT_HANDLE CNtlSobStatusAnimSync::AddAnimSyncUnit(RwInt32 iCurrValue)
{
	static SYNCEVENT_HANDLE hUnit = 0;

	hUnit++;

	CNtlSobStatusAnimSyncUnit *pAnimSyncUnit = NTL_NEW CNtlSobStatusAnimSyncUnit;
	pAnimSyncUnit->SetUIntHandle(hUnit);
	pAnimSyncUnit->SetValue(iCurrValue, 0);

	m_listAnimSyncUnit.push_back(pAnimSyncUnit);

	return hUnit;
}

void CNtlSobStatusAnimSync::PopAnimSyncUnit(void)
{
	if(m_listAnimSyncUnit.empty())
	{
		m_bFinish = TRUE;
		return;
	}

	ListAnimSyncUnit::iterator it = m_listAnimSyncUnit.begin();
	CNtlSobStatusAnimSyncUnit *pSyncUnit = (*it);

	// 여기에서 보낸다.
	// Event를 보낸다. 
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hSerialId);
	if(pSobObj)
	{
		RwInt32 iCurrValue, iMaxValue;
		pSyncUnit->GetValue(iCurrValue, iMaxValue);
		Logic_SetLpPlusMinus(pSobObj, iCurrValue);        
	}

	NTL_DELETE( *it );

	m_listAnimSyncUnit.erase(it);

	if(m_listAnimSyncUnit.empty())
	{
		m_bFinish = TRUE;
	}

	m_fExceptTime = 0.0f;
}

void CNtlSobStatusAnimSync::PopAnimSyncUnitAll(void)
{
	while(!IsFinish())
	{
		PopAnimSyncUnit();
	}
}

void CNtlSobStatusAnimSync::SetAnimSyncUnitLifeTime(RwReal fLifeTime)
{
	m_fLifeTime = fLifeTime;
}

void CNtlSobStatusAnimSync::ProcFinialServerSyncUnit(void)
{
	if(m_pServerSyncUnit)
	{
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hSerialId);
		if(pSobObj)
		{
			RwInt32 iCurrValue, iMaxValue;
			m_pServerSyncUnit->GetValue(iCurrValue, iMaxValue);
			Logic_SetLp(pSobObj, iCurrValue, iMaxValue);
		}
	}
}


RwBool CNtlSobStatusAnimSync::IsFinish(void)
{
	return m_bFinish;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlSobStatusAnimSyncContainer)

CNtlSobStatusAnimSyncContainer::CNtlSobStatusAnimSyncContainer()
{
	m_hSerialId	= INVALID_SERIAL_ID;
	m_bFinish	= FALSE;
}


CNtlSobStatusAnimSyncContainer::~CNtlSobStatusAnimSyncContainer()
{
	ListAnimSync::iterator it;
	for(it = m_listAnimSync.begin(); it != m_listAnimSync.end(); it++)
	{
		NTL_DELETE( *it );
	}

	m_listAnimSync.clear();
}


void CNtlSobStatusAnimSyncContainer::Update(RwReal fElapsed)
{
	RwBool bFinialServerSync = FALSE;
	if(m_listAnimSync.size() == 1)
		bFinialServerSync = TRUE;

	CNtlSobStatusAnimSync *pAnimSync;
	ListAnimSync::iterator it;
	for(it = m_listAnimSync.begin(); it != m_listAnimSync.end(); )
	{
		pAnimSync = (*it);
		pAnimSync->Update(fElapsed);
		if(pAnimSync->IsFinish())
		{
			// 서버 동기 처리를 한다.
			if(bFinialServerSync)
			{
				pAnimSync->ProcFinialServerSyncUnit();
			}

			NTL_DELETE( pAnimSync );

			it = m_listAnimSync.erase(it);
		}
		else
		{
			it++;
		}
	}

	if(m_listAnimSync.empty())
	{
		m_bFinish = TRUE;
	}
}

CNtlSobStatusAnimSync* CNtlSobStatusAnimSyncContainer::FindAnimSync(SYNCEVENT_HANDLE hSyncStatus)
{
	CNtlSobStatusAnimSync *pAnimSync;
	ListAnimSync::iterator it;
	for(it = m_listAnimSync.begin(); it != m_listAnimSync.end(); it++)
	{
		pAnimSync = (*it);
		if(pAnimSync->GetSyncHandle() == hSyncStatus)
			return pAnimSync;
	}
	
	return NULL;
}

void CNtlSobStatusAnimSyncContainer::SetSerialId(SERIAL_HANDLE hSerialId)
{
	m_hSerialId = hSerialId;
}

void CNtlSobStatusAnimSyncContainer::SetFinialServerSyncUnit(SYNCEVENT_HANDLE hSyncStatus, RwInt32 iCurrValue, RwInt32 iMaxValue)
{
	CNtlSobStatusAnimSync *pAnimSync = FindAnimSync(hSyncStatus);
	if(pAnimSync == NULL)
	{
		pAnimSync = NTL_NEW CNtlSobStatusAnimSync;
		pAnimSync->SetSerialId(m_hSerialId);

		m_listAnimSync.push_back(pAnimSync);
	}

	pAnimSync->SetServerSyncUnit(iCurrValue, iMaxValue);
}

SERIAL_HANDLE CNtlSobStatusAnimSyncContainer::AddAnimSyncUnit(SYNCEVENT_HANDLE hSyncStatus, RwInt32 iValue)
{
	CNtlSobStatusAnimSync *pAnimSync = FindAnimSync(hSyncStatus);
	if(pAnimSync == NULL)
	{
		pAnimSync = NTL_NEW CNtlSobStatusAnimSync;
		pAnimSync->SetSyncHandle(hSyncStatus);
		pAnimSync->SetSerialId(m_hSerialId);

		m_listAnimSync.push_back(pAnimSync);
	}

	return pAnimSync->AddAnimSyncUnit(iValue);
}

void CNtlSobStatusAnimSyncContainer::PopAnimSyncUnit(SYNCEVENT_HANDLE hSyncStatus)
{
	CNtlSobStatusAnimSync *pAnimSync;
	ListAnimSync::iterator it;
	
	RwBool bFinialServerSync = FALSE;
	if(m_listAnimSync.size() == 1)
		bFinialServerSync = TRUE;

	for(it = m_listAnimSync.begin(); it != m_listAnimSync.end(); )
	{
		pAnimSync = (*it);
		if(pAnimSync->GetSyncHandle() == hSyncStatus)
		{
			pAnimSync->PopAnimSyncUnit();
			if(pAnimSync->IsFinish())
			{
				// 서버 동기 처리를 한다.
				if(bFinialServerSync)
				{
					pAnimSync->ProcFinialServerSyncUnit();
				}

				NTL_DELETE( pAnimSync );

				it = m_listAnimSync.erase(it);
			}
			else
			{
				it++;
			}
		}
		else
		{
			it++;
		}
	}

	if(m_listAnimSync.empty())
	{
		m_bFinish = TRUE;
	}
}

void CNtlSobStatusAnimSyncContainer::PopAnimSyncUnitAll(SYNCEVENT_HANDLE hSyncStatus)
{
	CNtlSobStatusAnimSync *pAnimSync;
	ListAnimSync::iterator it;
	
	RwBool bFinialServerSync = FALSE;
	if(m_listAnimSync.size() == 1)
		bFinialServerSync = TRUE;

	for(it = m_listAnimSync.begin(); it != m_listAnimSync.end(); )
	{
		pAnimSync = (*it);
		if(pAnimSync->GetSyncHandle() == hSyncStatus)
		{
			pAnimSync->PopAnimSyncUnitAll();
			if(pAnimSync->IsFinish())
			{
				// 서버 동기 처리를 한다.
				if(bFinialServerSync)
				{
					pAnimSync->ProcFinialServerSyncUnit();
				}

				NTL_DELETE( pAnimSync );

				it = m_listAnimSync.erase(it);
			}
			else
			{
				it++;
			}
		}
		else
		{
			it++;
		}
	}

	if(m_listAnimSync.empty())
	{
		m_bFinish = TRUE;
	}
}

void CNtlSobStatusAnimSyncContainer::SetAnimSyncUnitLifeTime(SYNCEVENT_HANDLE hSyncStatus, RwReal fLifeTime)
{
	CNtlSobStatusAnimSync *pAnimSync = FindAnimSync(hSyncStatus);
	if(pAnimSync)
	{
		pAnimSync->SetAnimSyncUnitLifeTime(fLifeTime);
	}
}

RwBool CNtlSobStatusAnimSyncContainer::IsFinish(void)
{
	return m_bFinish;
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
CNtlSobStatusAnimSyncManager::CNtlSobStatusAnimSyncManager()
{
}

CNtlSobStatusAnimSyncManager::~CNtlSobStatusAnimSyncManager()
{
	MapAnimSync::iterator it;
	for(it = m_mapLPAnimSync.begin(); it != m_mapLPAnimSync.end(); it++)
	{
		NTL_DELETE( (*it).second );
	}

	m_mapLPAnimSync.clear();
}

void CNtlSobStatusAnimSyncManager::Update(RwReal fElasped)
{
	MapAnimSync::iterator it;
	CNtlSobStatusAnimSyncContainer *pContainer;

	for(it = m_mapLPAnimSync.begin(); it != m_mapLPAnimSync.end(); )
	{
		pContainer = (*it).second;
		pContainer->Update(fElasped);
		if(pContainer->IsFinish())
		{
			NTL_DELETE( (*it).second );

			it = m_mapLPAnimSync.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void CNtlSobStatusAnimSyncManager::SetFinialServerSyncUnit(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus, RwInt32 iCurrValue, RwInt32 iMaxValue)
{
	if(!IsActive())
		return;

	if(!IsExistTarget(hSerialId))
		return;

	CNtlSobStatusAnimSyncContainer *pContainer = NULL;
	MapAnimSync::iterator it = m_mapLPAnimSync.find(hSerialId);
	if(it == m_mapLPAnimSync.end())
	{
		pContainer = NTL_NEW CNtlSobStatusAnimSyncContainer;
		pContainer->SetSerialId(hSerialId);
		m_mapLPAnimSync[hSerialId] = pContainer;
	}
	else
	{
		pContainer = (*it).second;
	}

	pContainer->SetFinialServerSyncUnit(hSyncStatus, iCurrValue, iMaxValue);
}

SERIAL_HANDLE CNtlSobStatusAnimSyncManager::AddLPAnimSyncUnit(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus, RwInt32 iValue)
{
	if(!IsActive())
		return INVALID_SERIAL_ID;

	if(!IsExistTarget(hSerialId))
		return INVALID_SERIAL_ID;

	CNtlSobStatusAnimSyncContainer *pContainer = NULL;
	MapAnimSync::iterator it = m_mapLPAnimSync.find(hSerialId);
	if(it != m_mapLPAnimSync.end())
	{
		pContainer = (*it).second; 
	}
	else
	{
		pContainer = NTL_NEW CNtlSobStatusAnimSyncContainer;
		pContainer->SetSerialId(hSerialId);
		m_mapLPAnimSync[hSerialId] = pContainer;
	}

	return pContainer->AddAnimSyncUnit(hSyncStatus, iValue);
}

void CNtlSobStatusAnimSyncManager::PopLPAnimSyncUnit(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus)
{
	if(!IsActive())
		return;

	if(!IsExistTarget(hSerialId))
		return;

	CNtlSobStatusAnimSyncContainer *pContainer = NULL;
	MapAnimSync::iterator it = m_mapLPAnimSync.find(hSerialId);
	if(it == m_mapLPAnimSync.end())
		return;

	pContainer = (*it).second; 
	pContainer->PopAnimSyncUnit(hSyncStatus);

	if(pContainer->IsFinish())
	{
		NTL_DELETE( pContainer );
		m_mapLPAnimSync.erase(hSerialId);
	}
}

void CNtlSobStatusAnimSyncManager::PopLPAnimSyncUnitAll(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus)
{
	if(!IsActive())
		return;

	if(!IsExistTarget(hSerialId))
		return;

	CNtlSobStatusAnimSyncContainer *pContainer = NULL;
	MapAnimSync::iterator it = m_mapLPAnimSync.find(hSerialId);
	if(it == m_mapLPAnimSync.end())
		return;

	pContainer = (*it).second; 
	pContainer->PopAnimSyncUnitAll(hSyncStatus);

	if(pContainer->IsFinish())
	{
		NTL_DELETE( pContainer );

		m_mapLPAnimSync.erase(hSerialId);
	}
}

void CNtlSobStatusAnimSyncManager::SetLPAnimSyncUnitLifeTime(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus, RwReal fLifeTime)
{
	if(!IsActive())
		return;

	if(!IsExistTarget(hSerialId))
		return;

	CNtlSobStatusAnimSyncContainer *pContainer = NULL;
	MapAnimSync::iterator it = m_mapLPAnimSync.find(hSerialId);
	if(it == m_mapLPAnimSync.end())
		return;

	pContainer = (*it).second; 
	pContainer->SetAnimSyncUnitLifeTime(hSyncStatus, fLifeTime);
}

void CNtlSobStatusAnimSyncManager::RemoveLPAnimSyncContainer(SERIAL_HANDLE hSerialId)
{
	if(!IsActive())
		return;

	MapAnimSync::iterator it = m_mapLPAnimSync.find(hSerialId);
	if(it != m_mapLPAnimSync.end())
	{
		NTL_DELETE( (*it).second );

		m_mapLPAnimSync.erase(it);
	}
}

RwBool CNtlSobStatusAnimSyncManager::IsActive(void)
{
	return g_bActiveAnimSync;
}


RwBool CNtlSobStatusAnimSyncManager::IsExistTarget(SERIAL_HANDLE hSerialId)
{
	ListAnimSyncTarget::iterator it;
	for(it = m_listAnimSyncTarget.begin(); it != m_listAnimSyncTarget.end(); it++)
	{
		if( (*it) == hSerialId )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CNtlSobStatusAnimSyncManager::AddAnimSyncTarget(SERIAL_HANDLE hSerialId)
{
	if(!IsActive())
		return;

	if(IsExistTarget(hSerialId))
		return;

	m_listAnimSyncTarget.push_back(hSerialId);
}

void CNtlSobStatusAnimSyncManager::RemoveAnimSyncTarget(SERIAL_HANDLE hSerialId)
{
	if(!IsActive())
		return;
	
	ListAnimSyncTarget::iterator it;
	for(it = m_listAnimSyncTarget.begin(); it != m_listAnimSyncTarget.end(); )
	{
		if( (*it) == hSerialId )
		{
			RemoveLPAnimSyncContainer(hSerialId);
			it = m_listAnimSyncTarget.erase(it);
		}
		else
		{
			it++;
		}
	}
}