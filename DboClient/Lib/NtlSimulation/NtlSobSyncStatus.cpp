#include "precomp_ntlsimulation.h"
#include "NtlSobSyncStatus.h"

// core
#include "NtlDebug.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"

// presentation
#include "NtlPLHelpers.h"
#include "NtlPLSceneManager.h"

// simulation
#include "ntlSLEventFunc.h"
#include "NtlSobActor.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSobFactory.h"

CNtlSobSyncStatus::CNtlSobSyncStatus()
{
	m_bActive = FALSE;
	m_bFinish = FALSE;

	SetClassID(SLCLASS_SYNC_STATUS);
}

CNtlSobSyncStatus::~CNtlSobSyncStatus()
{
	m_bActive = FALSE;
	m_bFinish = FALSE;
}

RwBool CNtlSobSyncStatus::Create(void)
{
	CNtlSob::Create();

	return TRUE;
}

void CNtlSobSyncStatus::Destroy(void)
{
	NTL_FUNCTION("CNtlSobStatus::Destroy");

	CNtlSob::Destroy(); 
    
	NTL_RETURNVOID();
}

RwBool CNtlSobSyncStatus::IsFinish(void)
{
	return m_bFinish;
}

void CNtlSobSyncStatus::Update(RwReal fElapsed)
{
	if(m_bActive)
	{
		m_bFinish = TRUE;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlSobAttackSyncStatus)

CNtlSobAttackSyncStatus::CNtlSobAttackSyncStatus()
{
	m_fActiveTime = 0.0f;
	m_fCurrTime = 0.0f;
}

CNtlSobAttackSyncStatus::~CNtlSobAttackSyncStatus()
{

}

void CNtlSobAttackSyncStatus::Destroy(void)
{
	CNtlSobSyncStatus::Destroy();
}

RwBool CNtlSobAttackSyncStatus::IsFinish(void)
{
	if(m_fCurrTime >= m_fActiveTime)
		return TRUE;

	return FALSE;
}

void CNtlSobAttackSyncStatus::Update(RwReal fElapsed)
{
	m_fCurrTime+= fElapsed;
}

void CNtlSobAttackSyncStatus::ActiveTime(RwReal fTime)
{
	m_fActiveTime = fTime;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlSobDieSyncStatus)

CNtlSobDieSyncStatus::CNtlSobDieSyncStatus()
{
	
}

CNtlSobDieSyncStatus::~CNtlSobDieSyncStatus()
{
	
}


void CNtlSobDieSyncStatus::Destroy(void)
{
	
}

void CNtlSobDieSyncStatus::EventGenerator(void)
{
	CNtlSLEventGenerator::SobFainting(GetSobParent()->GetSerialID());
	// 이벤트를 보낸다.
	CNtlSobSyncStatus::Destroy();
}

void CNtlSobDieSyncStatus::Update( RwReal fElapsed ) 
{
    CNtlSobSyncStatus::Update(fElapsed);

    if(Logic_GetLp(GetSobParent()) == 0)
    {
        EventGenerator();
        m_bFinish = TRUE;
    }
}