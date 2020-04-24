#include "precomp_ntlsimulation.h"
#include "NtlFSMLookAtManager.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlFSMCharAct.h"
#include "NtlFSMVehicleAct.h"


CNtlFSMLookAtManager *CNtlFSMLookAtManager::m_pInstance = NULL;

CNtlFSMLookAtManager* CNtlFSMLookAtManager::GetInstance(void)
{
	return m_pInstance;
}


CNtlFSMLookAtManager::CNtlFSMLookAtManager()
{
	m_pInstance = this;
	m_pFSMCharAct = NULL;
	m_pFSMVehicleAct = NULL;
}

CNtlFSMLookAtManager::~CNtlFSMLookAtManager()
{
	m_pInstance = NULL;
}


RwBool CNtlFSMLookAtManager::Create(void)
{
	m_pFSMCharAct = NTL_NEW CNtlFSMCharAct;
	m_pFSMCharAct->Create();

	m_pFSMVehicleAct = NTL_NEW CNtlFSMVehicleAct;
	m_pFSMVehicleAct->Create();

	return TRUE;
}

void CNtlFSMLookAtManager::Destroy(void)
{
	if(m_pFSMCharAct)
	{
		m_pFSMCharAct->Destroy();
		NTL_DELETE(m_pFSMCharAct);
		m_pFSMCharAct = NULL;
	}

	if(m_pFSMVehicleAct)
	{
		m_pFSMVehicleAct->Destroy();
		NTL_DELETE(m_pFSMVehicleAct);
		m_pFSMVehicleAct = NULL;
	}
}