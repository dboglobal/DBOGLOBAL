#include "precomp_ntlsimulation.h"
#include "DboTLSystem.h"
#include "DboTLLoader.h"
#include "DboTLController.h"
#include "NtlSLEvent.h"


CDboTLSystem::CDboTLSystem( void )
{
	m_bActive = FALSE;

	m_pTLLoader = NULL;
	m_pTLController = NULL;
}


CDboTLSystem::~CDboTLSystem( void )
{
	Delete();
}


void CDboTLSystem::SetActive( RwBool bActive )
{
	m_bActive = bActive;
}


RwBool CDboTLSystem::Create( void )
{
	m_pTLLoader = NTL_NEW CDboTLLoader;

	if ( m_pTLLoader->Load() )
	{
		m_pTLController = NTL_NEW CDboTLController;

		CDboTLLoader::listdef_TLSyncGroupList* pTLSyncGroupList = m_pTLLoader->GetTLSyncGroupList();
		CDboTLLoader::listdef_TLSyncGroupList::iterator it = pTLSyncGroupList->begin();
		for ( ; it != pTLSyncGroupList->end(); ++it )
		{
			m_pTLController->PushTLSyncGroup( *it );
		}

		return TRUE;
	}
	else
	{
		NTL_DELETE( m_pTLLoader );

		return FALSE;
	}
}


void CDboTLSystem::Update( RwReal fElapsed )
{
	if ( !m_bActive )
	{
		return;
	}

	if ( m_pTLController )
	{
		m_pTLController->Update( fElapsed );
	}
}


void CDboTLSystem::Delete( void )
{
	NTL_DELETE( m_pTLController );
	NTL_DELETE( m_pTLLoader );
}


void CDboTLSystem::Skip( void )
{
	m_pTLController->Skip();
}


RwBool CDboTLSystem::IsSkipping( void )
{
	return m_pTLController->IsSkipping();
}


void CDboTLSystem::HandleEvents( RWS::CMsg &pMsg )
{
	if ( m_pTLController )
	{
		m_pTLController->HandleEvents( pMsg );
	}
}
